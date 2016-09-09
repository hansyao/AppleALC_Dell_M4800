//
//  kern_iokit.cpp
//  KernelCommon
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#include "kern_iokit.hpp"
#include "kern_util.hpp"
#include "kern_patcher.hpp"

#include <libkern/c++/OSSerialize.h>
#include <IOKit/IORegistryEntry.h>
#include <IOKit/IOService.h>
#include <IOKit/IODeviceTreeSupport.h>

namespace IOUtil {

	OSSerialize *getProperty(IORegistryEntry *entry, const char *property) {
		auto value = entry->getProperty(property);
		if (value) {
			KernelPatcher::releaseMemoryLock();
			auto s = OSSerialize::withCapacity(PAGE_SIZE);
			KernelPatcher::obtainMemoryLock();
			if (value->serialize(s)) {
				return s;
			} else {
				SYSLOG("ioutil @ failed to serialise %s property", property);
				KernelPatcher::releaseMemoryLock();
				s->release();
				KernelPatcher::obtainMemoryLock();
			}
		} else {
			DBGLOG("ioutil @ failed to get %s property", property);
		}
		return nullptr;
	}
	
	int getComputerModel() {
		auto entry = IORegistryEntry::fromPath("/", gIODTPlane);
		if (entry) {
			auto prop =  entry->getProperty("compatible");
			if (prop) {
				auto data = OSDynamicCast(OSData, prop);
				if (data) {
					//TODO: make this more reliable
					if (strstr(static_cast<const char *>(data->getBytesNoCopy()), "Book", strlen("Book"))) {
						return ComputerModel::ComputerLaptop;
					} else {
						return ComputerModel::ComputerDesktop;
					}
				} else {
					DBGLOG("ioutil @ compatible property has invalid format");
				}
			} else {
				DBGLOG("ioutil @ failed to get compatible property");
			}
		}
		DBGLOG("ioutil @ failed to get DT entry");
		return ComputerModel::ComputerAny;
	}
	
	IORegistryEntry *findEntryByPrefix(const char *path, const char *prefix, const IORegistryPlane *plane, bool (*proc)(IORegistryEntry *), bool brute) {
		KernelPatcher::releaseMemoryLock();
		auto entry = IORegistryEntry::fromPath(path, plane);
		KernelPatcher::obtainMemoryLock();
		if (entry) {
			auto res = findEntryByPrefix(entry, prefix, plane, proc, brute);
			KernelPatcher::releaseMemoryLock();
			entry->release();
			KernelPatcher::obtainMemoryLock();
			return res;
		}
		DBGLOG("ioutil @ failed to get %s entry", path);
		return nullptr;
	}
	

	IORegistryEntry *findEntryByPrefix(IORegistryEntry *entry, const char *prefix, const IORegistryPlane *plane, bool (*proc)(IORegistryEntry *), bool brute) {
		bool found {false};
		IORegistryEntry *res {nullptr};
		
		size_t bruteCount {0};
		
		do {
			bruteCount++;
			KernelPatcher::releaseMemoryLock();
			auto iterator = entry->getChildIterator(plane);
			KernelPatcher::obtainMemoryLock();
			
			if (iterator) {
				size_t len = strlen(prefix);
				while (KernelPatcher::releaseMemoryLock(), (res = OSDynamicCast(IORegistryEntry, iterator->getNextObject())) != nullptr) {
					const char *resname = res->getName();
					KernelPatcher::obtainMemoryLock();
					
					//DBGLOG("ioutil @ iterating over %s", resname);
					if (!strncmp(prefix, resname, len)) {
						found = proc ? proc(res) : true;
						if (found) {
							if (bruteCount > 1)
								DBGLOG("ioutil @ bruted %s value in %zu attempts", prefix, bruteCount);
							if (!proc) {
								KernelPatcher::releaseMemoryLock();
								break;
							}
						}
					}
				}
				
				iterator->release();
				KernelPatcher::obtainMemoryLock();
				
			} else {
				SYSLOG("ioutil @ failed to iterate over entry");
				return nullptr;
			}
			
		} while (brute && bruteCount < bruteMax && !found);
		
		if (!found)
			DBGLOG("ioutil @ failed to find %s", prefix);
		return proc ? nullptr : res;
	}
}
