//
//  kern_start.hpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#ifndef kern_start_hpp
#define kern_start_hpp

#include <IOKit/IOService.h>

class AppleALC : public IOService {
	OSDeclareDefaultStructors(AppleALC)
	
	/**
	 *  Possible boot arguments
	 */
	static constexpr const char *booatargOff {"alc_off"};     // Disable the kext
	static constexpr const char *booatargDebug {"alc_dbg"};   // Enable debug logging
	
	/**
	 *  Retrieve boot arguments
	 */
	void getBootArguments();
	
	/**
	 *  Disable the extension by default
	 */
	bool isEnabled {false};
public:
	bool start(IOService *provider) override;
	void stop(IOService *provider) override;
};

#endif /* kern_start_hpp */