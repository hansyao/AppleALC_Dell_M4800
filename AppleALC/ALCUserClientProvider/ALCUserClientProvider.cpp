//
//  ALCUserClientProvider.cpp
//  AppleALC
//
//  Created by Nick on 10/14/20.
//  Copyright © 2020 vit9696. All rights reserved.
//

#include "ALCUserClientProvider.hpp"

OSDefineMetaClassAndStructors(ALCUserClientProvider, IOService);

bool ALCUserClientProvider::start(IOService* provider) {
	if (!super::start(provider))
		return false;
	
	auto matchingDict = IOService::nameMatching(kIOHDACodecDevice);
	if (!matchingDict) {
		DBGLOG("client", "failed to allocate matching dictionary");
		return false;
	}
	
	mHDACodecDevice = IOService::waitForMatchingService(matchingDict, 100000000); // Wait for 0.1s
	matchingDict->release();
	
	if (!mHDACodecDevice) {
		DBGLOG("client", "timeout in waiting for IOHDACodecDevice, will retry");
		return false;
	}
	
	// We are ready for verbs
	DBGLOG("client", "ALCUserClient is ready for hda-verbs");
	setProperty("ReadyForALCVerbs", kOSBooleanTrue);
	readyForVerbs = true;
	
	// Publish the service
	registerService();
	
	return true;
}

void ALCUserClientProvider::stop(IOService* provider) {
	super::stop(provider);
	
	OSSafeReleaseNULL(mHDACodecDevice);
}

uint64_t ALCUserClientProvider::sendHdaCommand(uint16_t nid, uint16_t verb, uint16_t param) {
	if (!readyForVerbs) {
		DBGLOG("client", "provider not ready to accept hda-verb commands");
		return kIOReturnError;
	}
	
	auto sharedAlc = AlcEnabler::getShared();
	
	if (!sharedAlc) {
		DBGLOG("client", "unable to get shared AlcEnabler instance");
		return kIOReturnError;
	}
	
	unsigned ret = 0;
	sharedAlc->IOHDACodecDevice_executeVerb(reinterpret_cast<void*>(mHDACodecDevice), nid, verb, param, &ret, true);
	DBGLOG("client", "send HDA command nid=0x%X, verb=0x%X, param=0x%X, result=0x%08x", nid, verb, param, ret);
	
	return ret;
}
