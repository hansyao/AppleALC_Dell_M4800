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
	
	if (isDisabled) {
		SYSLOG("init @ found a disabling argument or no arguments, exiting");
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
	isDisabled = false;
	char buf[16];

	isDisabled |= PE_parse_boot_argn(booatargOff, buf, sizeof(buf));
	isDisabled |= PE_parse_boot_argn("-s", buf, sizeof(buf));
	isDisabled |= PE_parse_boot_argn("-x", buf, sizeof(buf));
	
	debugEnabled = PE_parse_boot_argn(booatargDebug, buf, sizeof(buf));
	
	lowMemory = PE_parse_boot_argn(bootargLowMem, buf, sizeof(buf));
		
	DBGLOG("init @ boot arguments disabled %d, debug %d", isDisabled, debugEnabled);
}
