//
//  kern_alc.hpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#ifndef kern_alc_hpp
#define kern_alc_hpp

#include "kern_patcher.hpp"
#include "kern_resources.hpp"

class AlcEnabler {
public:
	bool init();
	void deinit();
	
private:
	KernelPatcher patcher;
	
	/**
	 *  Load AppleHDA and prepare callbacks for processing
	 *
	 *  @return true on success
	 */
	bool loadHDAKext();
	
	/**
	 *  Patch AppleHDA or another kext if needed and prepare other patches
	 *
	 *  @param index   kinfo handle
	 *  @param address kinfo load address
	 *  @param size    kinfo memory size
	 */
	void processKext(size_t index, mach_vm_address_t address, size_t size);
	
	/**
	 *  ResourceLoad callback type
	 */
	using t_callback = void (*)(uint32_t, kern_return_t, const void *, uint32_t, void *);
	
	/**
	 *  Hooked ResourceLoad callbacks returning correct layout/platform
	 */
	static void layoutLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context);
	static void platformLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context);

	/**
	 *  Trampolines for original method invocations
	 */
	t_callback orgLayoutLoadCallback {nullptr};
	t_callback orgPlatformLoadCallback {nullptr};
	
	/**
	 *  current detected layout-id/device-id used in grabCodecs
	 *  Ideally they are not here
	 */
	uint32_t tmpLayout;
	uint32_t tmpDevice;
	
	/**
	 *  Detects audio codecs
	 *
	 *  @return see validateCodecs
	 */
	bool grabCodecs();
	
	/**
	 *  Validates found codecs
	 *
	 *  @return true if anything suitable found
	 */
	bool validateCodecs();

	/**
	 *  Supported resource types
	 */
	enum class Resource {
		Layout,
		Platform
	};

	/**
	 *  Update resource request parameters with hooked data if necessary
	 *
	 *  @param type               resource type
	 *  @param resourceData       resource data reference
	 *  @param resourceDataLength resource data length reference
	 */
	void updateResource(Resource type, const void * &resourceData, uint32_t &resourceDataLength);

	/**
	 *  Codec identification and modification info
	 */
	class CodecInfo {
		CodecInfo(uint64_t ven, uint32_t rev, uint32_t lid, uint32_t did) :
		revision(rev), layout(lid), device(did) {
			vendor = (ven & 0xFFFF0000) >> 16;
			codec = ven & 0xFFFF;
		}
	public:
		static CodecInfo *create(uint64_t ven, uint32_t rev, uint32_t lid, uint32_t did) {
			return new CodecInfo(ven, rev, lid, did);
		}
		static void deleter(CodecInfo *info) { delete info; }
		
		const CodecModInfo *info {nullptr};
		uint32_t revision;
		uint16_t vendor;
		uint16_t codec;
		uint32_t layout;
		uint32_t device;
	};
	
	/**
	 *  Detected and validated codec infos
	 */
	evector<CodecInfo *, CodecInfo::deleter> codecs;
};

#endif /* kern_alc_hpp */
