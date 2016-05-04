//
//  kern_policy.cpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#include "kern_policy.hpp"
#include "kern_util.hpp"

bool Policy::registerPolicy() {
	return mac_policy_register(&policyConf, &policyHandle, nullptr) == KERN_SUCCESS;
}

bool Policy::unregisterPolicy() {
	if (!(policyConf.mpc_loadtime_flags & MPC_LOADTIME_FLAG_UNLOADOK)) {
		SYSLOG("policy @ this policy is not allowed to be unregistered");
	}

	return mac_policy_unregister(policyHandle) == KERN_SUCCESS;
}