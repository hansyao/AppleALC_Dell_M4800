//
//  kern_iokit.hpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#ifndef kern_iokit_hpp
#define kern_iokit_hpp

#include <libkern/c++/OSSerialize.h>
#include <IOKit/IORegistryEntry.h>

namespace IOUtil {

	/**
	 *  Retrieve property object
	 *
	 *  @param entry    IORegistry entry
	 *  @param property property name
	 *
	 *  @return property object (must be released) or nullptr
	 */
	OSSerialize *getProperty(IORegistryEntry *entry, const char *property);
	
	/**
	 *  Retrieve an ioreg entry by path/prefix
	 *
	 *  @param path    an exact lookup path
	 *  @param prefix  entry prefix at path
	 *  @param plane   plane to lookup in
	 *  @param proc    process every found entry with the method
	 *
	 *  @return entry pointer (must NOT be released) or nullptr (on failure or in proc mode)
	 */
	IORegistryEntry *findEntryByPrefix(const char *path, const char *prefix, const IORegistryPlane *plane, void (*proc)(IORegistryEntry *)=nullptr);
	
	/**
	 *  Retrieve an ioreg entry by path/prefix
	 *
	 *  @param entry   an ioreg entry to look in
	 *  @param prefix  entry prefix at path
	 *  @param plane   plane to lookup in
	 *  @param proc    process every found entry with the method
	 *
	 *  @return entry pointer (must NOT be released) or nullptr (on failure or in proc mode)
	 */
	IORegistryEntry *findEntryByPrefix(IORegistryEntry *entry, const char *prefix, const IORegistryPlane *plane, void (*proc)(IORegistryEntry *)=nullptr);
}

#endif /* kern_iokit_hpp */
