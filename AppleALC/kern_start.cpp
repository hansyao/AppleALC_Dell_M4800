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
AlcEnabler AppleALC::enabler;
mac_policy_ops AppleALC::policyOps  {
	.mpo_policy_initbsd					= policyInitBSD,
	.mpo_vnode_check_access				= policyCheckAccess
};

void AppleALC::policyInitBSD(mac_policy_conf *conf) {
	// Do nothing for now
}

int AppleALC::policyCheckAccess(kauth_cred_t cred, vnode *vp, label *label, int acc_mode) {
	static bool tried {false};

	if (vnode_isvroot(vp)) {
		if (!tried) {
			DBGLOG("init @ initialising enabler");
			if (!enabler.init()) enabler.deinit();
			tried = true;
		}
	} else {
		//DBGLOG("init @ found non-root vp");
	}
	
	return 0;
}

bool AppleALC::init(OSDictionary *dict) {
	if (!IOService::init(dict)) {
		SYSLOG("init @ failed to initalise the parent");
		return false;
	}
	
	getBootArguments();
	
	if (isDisabled) {
		SYSLOG("init @ found a disabling argument or no arguments, exiting");
		return false;
	}
	
	if (mode == StartMode::Policy) {
		DBGLOG("init @ initialising AppleALC with policy mode");
		
		if (mac_policy_register(&policyConf, &policyHandle, NULL)) {
			SYSLOG("init @ failed to register the policy");
			return false;
		}
	}
	
	return true;
}

bool AppleALC::start(IOService *provider) {
	if (mode == StartMode::IOKit) {
		DBGLOG("init @ initialising AppleALC with IOKit mode");
		
		if (!IOService::start(provider)) {
			SYSLOG("init @ failed to start the parent");
			return false;
		}
		
		bool res = enabler.init();
		if (!res) enabler.deinit();
		return res;
	} else {
		return true;
	}
}

void AppleALC::stop(IOService *provider) {
	if (mode == StartMode::Policy && policyHandle) {
		mac_policy_unregister(policyHandle);
	}
	
	enabler.deinit();
	IOService::stop(provider);
	
	DBGLOG("init @ stopped");
}

void AppleALC::getBootArguments() {
	isDisabled = false;
	char buf[16];

	isDisabled |= PE_parse_boot_argn(bootargOff, buf, sizeof(buf));
	isDisabled |= PE_parse_boot_argn("-s", buf, sizeof(buf));
	isDisabled |= PE_parse_boot_argn("-x", buf, sizeof(buf));
	
	debugEnabled = PE_parse_boot_argn(bootargDebug, buf, sizeof(buf));
	
	lowMemory = PE_parse_boot_argn(bootargLowMem, buf, sizeof(buf));
	
	if (PE_parse_boot_argn(bootargPolicy, buf, sizeof(buf))) {
		mode = StartMode::Policy;
	} else if (PE_parse_boot_argn(bootargIOKit, buf, sizeof(buf))) {
		mode = StartMode::IOKit;
	}
		
	DBGLOG("init @ boot arguments disabled %d, debug %d", isDisabled, debugEnabled);
}
