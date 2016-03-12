//
//  kern_resource.hpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#ifndef kern_resource_hpp
#define kern_resource_hpp

#include "kern_util.hpp"
#include "kern_patcher.hpp"

#include <sys/types.h>
#include <stdint.h>

/**
 *  IORegistry path sequence for finding IOHDACodec info and layout number
 *  correspounds to CodecLookup.plist resource file
 */
struct CodecLookupInfo {
	const char **tree;
	size_t treeSize;
	size_t controllerNum;
	bool detect {false};
};

struct KextPatch {
	KernelPatcher::LookupPatch patch;
	uint32_t minKernel {KernelPatcher::KernelAny};
	uint32_t maxKernel {KernelPatcher::KernelAny};
};

/**
 *  Corresponds to a Controllers.plist entry
 */
struct ControllerModInfo {
	const char *name {nullptr};
	uint32_t vendor {0};
	uint32_t device {0};
	const uint32_t *revisions {nullptr};
	size_t revisionNum {0};
	
	const KextPatch *patches {nullptr};
	size_t patchNum {0};
};

/**
 *  Corresponds to Info.plist resource file of each codec
 */
struct CodecModInfo {
	struct File {
		const uint8_t *data {nullptr};
		uint32_t dataLength {0};
		uint32_t minKernel {KernelPatcher::KernelAny};
		uint32_t maxKernel {KernelPatcher::KernelAny};
		uint32_t layout {0};
	};

	const char *name {nullptr};
	uint16_t codec {0};
	const uint32_t *revisions {nullptr};
	size_t revisionNum {0};
	
	const File *platforms {nullptr};
	size_t platformNum {0};
	const File *layouts {nullptr};
	size_t layoutNum {0};
	const KextPatch *patches {nullptr};
	size_t patchNum {0};
};

/**
 *  Contains all the supported codecs by a specific vendor
 *  Corresponds to Vendors.plist resource file
 */
struct VendorModInfo {
	const char *name {nullptr};
	uint16_t vendor {0};
	const CodecModInfo *codecs {nullptr};
	const size_t codecsNum {0};
};

/**
 *  Generated resource data
 */
extern CodecLookupInfo codecLookup[];
extern const size_t codecLookupSize;

extern KernelPatcher::KextInfo kextList[];
extern const size_t kextListSize;

extern ControllerModInfo controllerMod[];
extern const size_t controllerModSize;

extern VendorModInfo vendorMod[];
extern const size_t vendorModSize;


#endif /* kern_resource_hpp */
