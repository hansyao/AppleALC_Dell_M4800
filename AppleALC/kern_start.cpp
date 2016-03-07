//
//  kern_start.cpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#include "kern_start.hpp"
#include "kern_iokit.hpp"
#include "kern_alc.hpp"
#include "kern_util.hpp"

#include <IOKit/IOLib.h>
#include <IOKit/IORegistryEntry.h>
#include <mach/mach_types.h>

OSDefineMetaClassAndStructors(AppleALC, IOService)

AlcEnabler enabler;

bool AppleALC::start(IOService *provider) {
	DBGLOG("init @ starting AppleALC");
	
	if (!IOService::start(provider)) {
		SYSLOG("init @ failed to start the parent");
		return false;
	}
	
	getBootArguments();
	
	if (!isEnabled) {
		DBGLOG("init @ no boot arguments or found a disabling argument, exiting");
		return false;
	}

	bool res = enabler.init();
	if (!res) enabler.deinit();
	return res;
}

void AppleALC::stop(IOService *provider) {
	enabler.deinit();
	IOService::stop(provider);
	
	DBGLOG("init @ stopped");
}

void AppleALC::getBootArguments() {
	OSSerialize *s = IOUtil::retrieveBootArguments();
			
	if (s) {
		bool disabled {false};
		disabled |= strstr(s->text(), booatargOff, strlen(booatargOff)) != nullptr;
		disabled |= strstr(s->text(), "-s", strlen("-s")) != nullptr;
		disabled |= strstr(s->text(), "-x", strlen("-x")) != nullptr;
	
		isEnabled = !disabled;
		debugEnabled = strstr(s->text(), booatargDebug, strlen(booatargDebug));
		
		DBGLOG("init @ boot arguments value is %s (enabled %d, debug %d)", s->text(), isEnabled, debugEnabled);
		
		s->release();
	} else {
		DBGLOG("init @ no boot arguments available");
	}
}
