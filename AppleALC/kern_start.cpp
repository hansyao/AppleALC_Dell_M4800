//
//  kern_start.cpp
//  AppleALC
//
//  Copyright © 2016-2017 vit9696. All rights reserved.
//

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>

#include "kern_alc.hpp"

static AlcEnabler alc;

static const char *bootargOff[] {
	"-alcoff"
};

static const char *bootargDebug[] {
	"-alcdbg"
};

static const char *bootargBeta[] {
	"-alcbeta"
};

PluginConfiguration ADDPR(config) {
	xStringify(PRODUCT_NAME),
	parseModuleVersion(xStringify(MODULE_VERSION)),
	bootargOff,
	sizeof(bootargOff)/sizeof(bootargOff[0]),
	bootargDebug,
	sizeof(bootargDebug)/sizeof(bootargDebug[0]),
	bootargBeta,
	sizeof(bootargBeta)/sizeof(bootargBeta[0]),
	KernelVersion::MountainLion,
	KernelVersion::Sierra,
	[]() {
		alc.init();
	}
};





