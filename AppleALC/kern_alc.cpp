//
//  kern_alc.cpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#include "kern_alc.hpp"
#include "kern_iokit.hpp"
#include "kern_resources.hpp"

#include <mach/vm_map.h>
#include <IOKit/IORegistryEntry.h>
#include <IOKit/IOService.h>

//TODO: get rid of this
static AlcEnabler *that {nullptr};

bool AlcEnabler::init() {
	patcher.init();
	
	if (patcher.getError() != KernelPatcher::Error::NoError) {
		DBGLOG("alc @ failed to initialise kernel patcher");
		patcher.clearError();
		return false;
	}
	
	return loadHDAKext();
}

void AlcEnabler::deinit() {
	patcher.deinit();
	codecs.deinit();
}

void AlcEnabler::layoutLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context) {
	if (that && that->orgLayoutLoadCallback) {
		that->updateResource(Resource::Layout, resourceData, resourceDataLength);
		that->orgLayoutLoadCallback(requestTag, result, resourceData, resourceDataLength, context);
	} else {
		SYSLOG("alc @ layout callback arrived at nowhere");
	}
}

void AlcEnabler::platformLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context) {
	if (that && that->orgPlatformLoadCallback) {
		that->updateResource(Resource::Platform, resourceData, resourceDataLength);
		that->orgPlatformLoadCallback(requestTag, result, resourceData, resourceDataLength, context);
	} else {
		SYSLOG("alc @ platform callback arrived at nowhere");
	}
}

bool AlcEnabler::loadHDAKext() {
	if (that) return true;
	
	patcher.loadKinfo(kextAppleHDA);
	if (patcher.getError() != KernelPatcher::Error::NoError) {
		SYSLOG("alc @ failed to load AppleHDA kext file");
		patcher.clearError();
		return false;
	}
	
	patcher.setupKextListening();
	
	if (patcher.getError() != KernelPatcher::Error::NoError) {
		SYSLOG("alc @ failed to setup kext hooking");
		patcher.clearError();
		return false;
	}
	
	auto handler = KernelPatcher::KextHandler::create(kextAppleHDA->id, kextAppleHDA->loadIndex,
	[](KernelPatcher::KextHandler *h) {
		if (h) {
			that->processKext(h->index, h->address);
		} else {
			SYSLOG("alc @ notification callback arrived at nowhere");
		}
	});
	
	if (!handler) {
		SYSLOG("alc @ failed to allocate KextHandler");
		return false;
	}
	
	patcher.waitOnKext(handler);
	
	if (patcher.getError() != KernelPatcher::Error::NoError) {
		SYSLOG("alc @ failed to wait on kext");
		patcher.clearError();
		KernelPatcher::KextHandler::deleter(handler);
		return false;
	}
	
	that = this;
	return true;
}

void AlcEnabler::processKext(size_t index, mach_vm_address_t address) {
	// We could have done some of this earlier by requiring com.apple.iokit.IOHDAFamily to load first
	// However, IOHDAFamily has insane version compatibilities over the OS X versions, I am lazy to use
	// TrustedBSD apis, and we might want to patch other kexts one day, so better to be ready asap and
	// keep this in one place...
	if (index == kextAppleHDA->loadIndex && !grabCodecs()) {
		SYSLOG("alc @ failed to find a suitable codec, we have nothing to do");
		return;
	}
	
	patcher.updateRunningInfo(index, address);
	
	bool routeCallbacks {false};
	
	for (size_t i = 0, num = codecs.size(); i < num; i++) {
		auto &info = codecs[i]->info;
		if (!info) {
			SYSLOG("alc @ missing CodecModInfo for %zu codec", i);
			continue;
		}
		
		if (index == kextAppleHDA->loadIndex && info->platforms > 0 && info->layoutNum > 0) {
			DBGLOG("alc @ will route callbacks resource loading callbacks");
			routeCallbacks = true;
		}
		
		for (size_t p = 0; p < info->patchNum; p++) {
			auto &patch = info->patches[p];
			if (patch.patch.kext->loadIndex == index) {
				if (patcher.compatibleKernel(patch.minKernel, patch.maxKernel)) {
					patcher.applyLookupPatch(&patch.patch);
					// Do not really care for the errors for now
					patcher.clearError();
				}
			} else if (patch.patch.kext->loadIndex == KernelPatcher::KextInfo::Unloaded) {
				auto loadIndex = patcher.loadKinfo(patch.patch.kext);
				if (patcher.getError() != KernelPatcher::Error::NoError) {
					patch.patch.kext->loadIndex = loadIndex;
					
					// A future recursion here
					auto handler = KernelPatcher::KextHandler::create(patch.patch.kext->id, patch.patch.kext->loadIndex,
						[](KernelPatcher::KextHandler *h) {
						if (h) {
							that->processKext(h->index, h->address);
						} else {
							SYSLOG("alc @ extra notification callback arrived at nowhere");
						}
					});
					
					if (handler) {
						patcher.waitOnKext(handler);
					} else {
						SYSLOG("alc @ failed to allocate an extra KextHandler");
					}
				}
				// Do not really care for the errors for now
				patcher.clearError();
			}
		}
	}
	
	if (routeCallbacks) {
		auto layout = patcher.solveSymbol(index, "__ZN14AppleHDADriver18layoutLoadCallbackEjiPKvjPv");
		auto platform = patcher.solveSymbol(index, "__ZN14AppleHDADriver20platformLoadCallbackEjiPKvjPv");
		if (!layout || !platform) {
			SYSLOG("alc @ failed to find AppleHDA layout or platform callback symbols (%llX, %llX)", layout, platform);
			return;
		}
		
		orgLayoutLoadCallback = reinterpret_cast<t_callback>(patcher.routeFunction(layout, reinterpret_cast<mach_vm_address_t>(layoutLoadCallback), true));
		if (patcher.getError() != KernelPatcher::Error::NoError) {
			SYSLOG("alc @ failed to hook layout callback");
			return;
		}
		
		orgPlatformLoadCallback = reinterpret_cast<t_callback>(patcher.routeFunction(platform, reinterpret_cast<mach_vm_address_t>(platformLoadCallback), true));
		if (patcher.getError() != KernelPatcher::Error::NoError) {
			SYSLOG("alc @ failed to hook platform callback");
			return;
		}
	}
}

void AlcEnabler::updateResource(Resource type, const void * &resourceData, uint32_t &resourceDataLength) {
	for (size_t i = 0, s = codecs.size(); i < s; i++) {
		auto info = codecs[i]->info;
		if (!info) {
			SYSLOG("alc @ missing CodecModInfo for %zu codec at resource updating", i);
			continue;
		}
		
		if (type == Resource::Platform && info->platforms) {
			for (size_t p = 0; p < info->platformNum; p++) {
				if (patcher.compatibleKernel(info->platforms[p].minKernel, info->platforms[p].maxKernel)) {
					DBGLOG("Found platform at %zu index", p);
					resourceData = info->platforms[p].data;
					resourceDataLength = info->platforms[p].dataLength;
				}
			}
		} else if (type == Resource::Layout && info->layouts) {
			for (size_t l = 0; l < info->layoutNum; l++) {
				if (codecs[i]->layout == info->layouts[l].idx &&
					patcher.compatibleKernel(info->layouts[l].minKernel, info->layouts[l].maxKernel)) {
					DBGLOG("Found layout at %zu index", l);
					resourceData = info->layouts[l].data;
					resourceDataLength = info->layouts[l].dataLength;
				}
			}
		}
	}
}

bool AlcEnabler::grabCodecs() {
	if (!that) {
		SYSLOG("alc @ you should call grabCodecs right before AppleHDA loading");
		return false;
	}
	
	for (size_t lookup = 0; lookup < codecLookupSize; lookup++) {
		
		that->tmpLayout = -1;
	
		// Not using recursive lookup due to multiple possible entries
		auto sect = IOUtil::findEntryByPrefix("/AppleACPIPlatformExpert", "PCI", gIOServicePlane);

		size_t i {0};
		
		while (sect && i < codecLookup[lookup].treeSize) {
			sect = IOUtil::findEntryByPrefix(sect, codecLookup[lookup].tree[i], gIOServicePlane,
											 i+1 == codecLookup[lookup].treeSize ? [](IORegistryEntry *e) {
				if (that->tmpLayout < 0) {
					SYSLOG("alc @ invalid layout-id was previously found %d", that->tmpLayout);
					return;
				}
				
				auto ven = e->getProperty("IOHDACodecVendorID");
				auto rev = e->getProperty("IOHDACodecRevisionID");
				
				if (!ven || !rev) {
					SYSLOG("alc @ codec entry misses properties, skipping");
					return;
				}
				
				auto venNum = OSDynamicCast(OSNumber, ven);
				auto revNum = OSDynamicCast(OSNumber, rev);
				
				if (!venNum || !revNum) {
					SYSLOG("alc @ codec entry contains invalid properties, skipping");
					return;
				}
				
				auto ci = AlcEnabler::CodecInfo::create(venNum->unsigned64BitValue(), revNum->unsigned32BitValue(), that->tmpLayout);
				if (ci) {
					if (!that->codecs.push_back(ci)) {
						SYSLOG("alc @ failed to store codec info for %X %X", ci->vendor, ci->codec);
						AlcEnabler::CodecInfo::deleter(ci);
					}
				} else {
					SYSLOG("alc @ failed to create codec info for %X %X", ci->vendor, ci->codec);
				}
					
			} : nullptr);
			
			if (i == codecLookup[lookup].layoutNum) {
				if (sect) {
					auto lid = sect->getProperty("layout-id");
					if (lid) {
						auto lidNum = OSDynamicCast(OSData, lid);
						if (lidNum && lidNum->getLength() > 0) {
							tmpLayout = static_cast<const uint8_t *>(lidNum->getBytesNoCopy())[0];
							DBGLOG("alc @ found layout-id %d in %s", tmpLayout, codecLookup[lookup].tree[i]);
							i++;
							continue;
						} else {
							SYSLOG("alc @ %s contains invalid layout-id", codecLookup[lookup].tree[i]);
						}
					}
				}
				SYSLOG("alc @ no layout found in %s, aborting", codecLookup[lookup].tree[i]);
				break;
			}
			
			i++;
		}
	}

	return validateCodecs();
}

bool AlcEnabler::validateCodecs() {
	size_t i = 0;
	
	while (i < codecs.size()) {
		bool suitable {false};
		
		// Check vendor
		size_t vIdx {0};
		while (vIdx < vendorModSize && vendorMod[vIdx].vendor != codecs[i]->vendor)
			vIdx++;
		
		if (vIdx != vendorModSize) {
			// Check codec
			size_t cIdx {0};
			while (cIdx < vendorMod[vIdx].codecsNum &&
				   vendorMod[vIdx].codecs[cIdx].codec != codecs[i]->codec)
				cIdx++;
			
			if (cIdx != vendorMod[vIdx].codecsNum) {
				// Check revision if present
				size_t rIdx {0};
				while (rIdx < vendorMod[vIdx].codecs[cIdx].revisionNum &&
					   vendorMod[vIdx].codecs[cIdx].revisions[rIdx] != codecs[i]->revision)
					rIdx++;
				
				if (rIdx != vendorMod[vIdx].codecs[cIdx].revisionNum ||
					vendorMod[vIdx].codecs[cIdx].revisionNum == 0) {
					codecs[i]->info = &vendorMod[vIdx].codecs[cIdx];
					suitable = true;
					
				}
				
				DBGLOG("alc @ found %s %s %s codec revision 0x%X",
					   suitable ? "supported" : "unsupported", vendorMod[vIdx].name,
					   vendorMod[vIdx].codecs[cIdx].name, codecs[i]->revision);
			} else {
				DBGLOG("alc @ found unsupported %s codec 0x%X", vendorMod[vIdx].name, codecs[i]->codec);
			}
		} else {
			DBGLOG("alc @ found unsupported codec vendor 0x%X", codecs[i]->vendor);
		}
		
		if (suitable)
			i++;
		else
			codecs.erase(i);
	}

	return codecs.size() > 0;
}