//
//  kern_iokit.cpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#include "kern_iokit.hpp"
#include "kern_util.hpp"

#include <libkern/c++/OSSerialize.h>
#include <IOKit/IORegistryEntry.h>
#include <IOKit/IOService.h>
#include <IOKit/IODeviceTreeSupport.h>

namespace IOUtil {
	OSSerialize *retrieveBootArguments() {
		// Maybe use PE_parse_boot_arg interface?
		auto entry = IORegistryEntry::fromPath("/options", gIODTPlane);
		if (entry) {
			auto s = getProperty(entry, "boot-args");
			entry->release();
			return s;
		}
		DBGLOG("ioutil @ failed to get options entry");
		return nullptr;
	}
	
	OSSerialize *getProperty(IORegistryEntry *entry, const char *property) {
		auto value = entry->getProperty(property);
		if (value) {
			auto s = OSSerialize::withCapacity(PAGE_SIZE);
			if (value->serialize(s)) {
				return s;
			} else {
				SYSLOG("ioutil @ failed to serialise %s property", property);
				s->release();
			}
		} else {
			SYSLOG("ioutil @ failed to get %s property", property);
		}
		return nullptr;
	}
	
	IORegistryEntry *findEntryByPrefix(const char *path, const char *prefix, const IORegistryPlane *plane, void (*proc)(IORegistryEntry *)) {
		auto entry = IORegistryEntry::fromPath(path, plane);
		if (entry) {
			auto res = findEntryByPrefix(entry, prefix, plane, proc);
			entry->release();
			return res;
		}
		SYSLOG("ioutil @ failed to get %s entry", path);
		return nullptr;
	}
	

	IORegistryEntry *findEntryByPrefix(IORegistryEntry *entry, const char *prefix, const IORegistryPlane *plane, void (*proc)(IORegistryEntry *)) {
		auto iterator = entry->getChildIterator(plane);
		bool found {false};
		
		if (iterator) {
			IORegistryEntry *res {nullptr};
			size_t len = strlen(prefix);
			while ((res = OSDynamicCast(IORegistryEntry, iterator->getNextObject())) != nullptr) {
				//DBGLOG("ioutil @ iterating over %s", res->getName());
				if (!strncmp(prefix, res->getName(), len)) {
					found = true;
					if (proc)
						proc(res);
					else
						break;
				}
			}
			iterator->release();
			if (!found)
				SYSLOG("ioutil @ failed to find %s", prefix);
			return proc ? nullptr : res;
		}
		
		SYSLOG("ioutil @ failed to iterate over entry");
		return nullptr;
	}
}