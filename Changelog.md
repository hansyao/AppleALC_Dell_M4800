AppleALC Changelog
==================
#### v1.0.11
- Added ALC668 layout-id 29 for Asus N750JV by DroID969
- ALC887VD Optimization Layout 11 officially will Support AC97 Front Panel like Windows Driver
- ALC887VD Optimization Added Layout 11, 12, 13, 17
- ALC887VD Optimization Layout 11 Front Mic and Back Mic Fix
- ALC887VD Optimization Layout 12 LinIn Fix
- ALC887VD Optimization Added AMD Support in all Layouts
- ALC887VD Optimization replace several custom platforms.xml.zlib with common platformsID.xml.zlib

#### v1.0.10
- Added ALC665 codec support layout-id 12, 13 by insanelydeepak
- Added ALC292 codec support layout-id 12
- Added ALC283 layout-id 11 for Aspire V3-371 by Slbomber
- Added IDT92HD75B3X5 layout-id 11 with a custom pinconfig by olderst
- Prohibited kext unloading in policy mode since our policy is registered as static and cannot be unregistered
- Bumped module versions to 10.8 to avoid accidental loading on 10.7
- Added No-sleep, no-click patches into ALC892 info.plist
- Added ALC670 codec support layout-id 12 by Alex Auditore
- Added revision-id in ALC889 info.plist
- Added ALC260 codec support layout-id 12
- Added Creative CA0132 codec support layout-id 11 and 12
- Added CirrusLogic CS4213 codec support layout-id 28 by insanelydeepak
- Added revision 0x100202 into ALC888 info.plist
- Changed not working layout12 and platforms12 for Creative CA0132

#### v1.0.9
- Replace ALC888 patch 11d4198b->10ec0885 it prevents Sound Asserions on layout-id 1, 2, 3
- Deleted unnecessary ALC888 patch 11d41983
- Changed patch for Intel HD4600 HDMI (ig-platform-id 0x0d220003)
- Changed ALC275 Resources
- Deleted not working Intel HD4000 LVDS, HDMI (ig-platform-id 0x0166000A) Laptop only patch
- Added Conexant CX20583 codec support layout-id 3 by Mirone
- Added Conexant CX20585 codec support layout-id 3 by Mirone
- Added Conexant CX20588 codec support layout-id 3 by Mirone
- Added Conexant CX20724 codec support layout-id 3 by Mirone
- Added Conexant CX20752 codec support layout-id 3 by Mirone
- Added Conexant CX20756 codec support layout-id 3 by Mirone
- Added Conexant CX20757 codec support layout-id 3 by Mirone
- Added NoSleep/NoClick pathes to Conexant CX20590 info.plist
- Added ALC280 layout-id 4 Combo Jack Support by Mirone
- Added ALC1150 support for OS X 10.8
- Changed Comment lines for Toleda resources in 1150 info.plist
- Added IDT92HD93BXX codec support layout-id 12

#### v1.0.8
- Added ALC885 Toleda resources layout-id 1
- Added ALC887 Toleda resources layout-id 1, 2, 3
- Fix mistake in CX20751/2 info.plist and change layout-id to 28
- Added ALC888 Toleda resources layout-id 1, 2, 3
- Added ALC668 layout 20 for Asus G551JM 
- Added ALC275 layout-id 3 Mirone resources
- Added ALC283 layout-id 3 Mirone resources
- Added ALC284 layout-id 3 Mirone resources
- Added ALC288 layout-id 3 Mirone resources
- Added ALC290 layout-id 3 Mirone resources
- Added IDT92HD66C3/65 layout-id 3 Mirone resources
- Added IDT92HD71B7X layout-id 3 Mirone resources
- Added IDT92HD75B2X5 layout-id 3 Mirone resources
- Added IDT92HD75B3X5 layout-id 3 Mirone resources
- Added IDT92HD99BXX layout-id 3 Mirone resources
- Added IDT92HD87B1 layout-id 3 Mirone resources
- Change IDT92HD81B1X5 to Mirone resources layout-id 3
- Change IDT92HD91BXX  to Mirone resources layout-id 3
- AppleALC should not abort loading if it fails to find some kext

#### v1.0.7
- Added 0x100004 revision to ALC269
- Added custom ALC269 resources for SONY VAIO VPCEB3M1R layout-id 20
- Added custom ALC282 resources for Acer Aspire E1-572G layout-id 28
- Added into ALC269 info.plist no-sleep, no-click, headphone detect patches
- Added ALC282 resources for Acer Aspire on IvyBridge by Andrey1970 layout-id 27
- Added Mirone resources to ALC892 thanks to cecekpawon
- Added ALC668 resources for DELL Precision M3800 by Syscl
- Allowed providing non-existent layouts
- Added IDT92HD81B1X5 codec support layout-id 12 by RehabMan
- Added IDT92HD87B1/3 codec support layout-id 12 by RehabMan
- Added IDT92HD87B2/4 codec support layout-id 12 by RehabMan
- Added IDT92HD91BXX codec support layout-id 12 by RehabMan
- Added IDT92HD95 codec support layout-id 12 by RehabMan
- Added Conexant CX20751/2 codec support layout-id 12 by RehabMan
- Added ALC280 codec support layout-id 3 by Mirone
- Fixed CX20590 platform12.xml.zlib > platforms12.xml.zlib
- Reduced kext size by optimizing every platforms.xml.zlib in Resources folder
- Fixed codec-id in PinConfigs info.plist for IDT 92HD87B1/3 should be 287143633

#### v1.0.6
- Reduced kext size by optimising capstone build options
- Added Model key with possible Laptop/Desktop/Any values to Controllers.plist (matches IODT/comptaible)
- Added VIA VT2020/2021 codec support layout-id 5, 7 Mirone resources
- Added Z97 HDEF controller patch
- Added Intel HD4000 HDMI support ig-platform-id 0x0166000A for Laptop and Desktop
- Added ALC668 layout-id 28 (for Asus N750Jk int.mic and sub working)
- Added ALC272 layout-id 3 Mirone resources
- Added ALC1150 layout-id 3 (Toleda update)
- Added missing data for ALC1150 resources
- Added ALC233 codec support layout-id 3 Mirone resources
- Added ALC235 codec support layout-id 3 Mirone resources
- Added ALC255 codec support layout-id 3 Mirone resources
- Added ALC268 codec support layout-id 3 Mirone resources
- Fixed incorrect ALC282 codec-id (should be 642)

#### v1.0.5
- Fixed AppleHDAController patching regression
- Fixed incorrect ALC269 codec-id (should be 617)
- Added embedded resource file deduplication reducing the size
- Added ALC887-VD, layout-id 11-15 codec support (by InsanelyDeepak)
- Added ALC883, layout-id 7 codec support (by Andrey1970)
- Changed resources and pinconfig for ALC888 deleted layout 1 using layout 5 for 3 ports and layout 7 for 5/6 ports 
- Added CX20590 Mirone resources layout-id 3
- Added ALC1150 Mirone resources layout-id 5 for 3 ports and layout-id 7 for 5/6 ports 
- Added ALC887-VD Mirone resources layout-id 5 for 3 ports and layout-id 7 for 5/6 ports 
- Added ALC882 codec support Mirone resources layout-id 5 for 3 ports and layout-id 7 for 5/6 ports
- Changed ALC668 resources and pinconfig Mirone resources
- Added ALC663 Mirone resources v1 - layout-id 3, v2 - layout-id 4
- Changed ALC662 resources to Mirone resources layout-id 5 for 3 ports and layout-id 7 for 5/6 ports 
- Added ALC282 Mirone resources v1 - layout-id 3, v2 - layout-id 4
- Sync PinConfig info.plist deleted unnesessary items
- Sync layout names (1,2,3 - Toleda Desktop, 1-8 - Mirone Laptop Patches, 5,7 - Mirone Desktop, 10-99 - Custom User layouts)
- Sync platforms names (platformsT - Toleda, PlatformsM - Mirone, Platforms10-99 - custom user platforms)
- Fix error in ALC282 PinConfig
- Added ALC270 Mirone resources layout-id 3, 4
- Added ALC269, ALC269-VB, ALC269-VC Mirone resources layout-id 1, 2, 3, 4, 5, 6, 7, 8
- Changed ALC269 layout-id 27 pinconfig
- Added revision 0x100001 into ALC888 info.plist and sync platforms name to common view
- Added VIA VT1802 revision 0x100000 layout-id 3 codec support Mirone resources
- Added AD1988B layout-id 5, 7 (3 ports, 5/6 Ports) Mirone resources and patch for El Capitan 8319D411->00000000
- Added AD2000B codec support layout-id 5, 7 (3 ports, 5/6 Ports) Mirone resources
- Change ALC270 layout28.xml.zlib and platforms28xml.zlib workaround for external microphone

#### v1.0.4
- Fixed a rare kernel panic due to uninitialised kern_proc
- Fixed a rare loading failure due to IOHDA value bruteforce necessity
- Added retry possibility for -alcpolicy
- Set -alcpolicy mode the default mode
- Added patch for AMD Radeon R7 370 HDMI (by John998)
- Changed platforms12.xml.zlib for ALC889 set MinimumSampleRate to 48000 (by Sergey_Galan)
- Added ALC269VC for Lenovo V850 codec support (by Ar4erR07)
- Added ALC270, layout-id 27 codec support (by m-dudarev)
- Added ALC282 DSP features and config improvements (by savvamitrofanov)
- Added ALC662, layout-id 12 codec support (by AxenovGosha)
- Added ALC663, layout-id 28 codec support (by m-dudarev)

#### v1.0.3
- Fixed AAPL,ig-platform-id naming
- Added preliminary start modes support
- Added TrustedBSD start via -alcpolicy and IOService::start via -alciokit arguments
- Allowed kext patches with 0 count replacing all entries (use this with a reason)
- Fixed ALC270 regressions
- Added ALC889, layout-id 12 codec support (by Sergey_Galan)
- Added ALC887-VD, layout 99 codec support (by Constanta)
- Removed unnecessary NVIDIA GT240 HDMI patches

#### v1.0.2
- Added Controller.plist with controller-specific patches
- Added Intel X99 and Intel HD4600 controller patches
- Reverted device-id comparison code, it was redundant
- Added prelinkedkernel reading (used exclusively in OS X Installer/Recovery HD)
- Added AppleIntelFramebufferAzul patches to fix HD4600 audio
- Added ALC269 10.9.x/10.10.x patches (by Andrey1970)
- Added ALC269 for Asus K53SJ (by alex1960)
- Added ALC270 codec support (by m-dudarev)
- Added ALC270 for Asus K56CB (by m-dudarev)
- Added ALC668 for Asus N750Jk (by alex1960)
- Added ALC885 (by alex1960)
- Added NVIDIA GT240 HDMI patches (by Vandroiy)
- Added Intel HD4000 patches (by Andrey1970)

#### v1.0.1
- boot-args presence is now optional
- Platforms must conform to layout-id from now on
- Workaround a race-condition kernel panic when accessing current_thread credential
- Fixed a kernel panic when kaslr is disabled
- Renamed control boot arguments to -alcdbg and -alcoff
- Added optional device-id comparison to patch and file matching
- Added PinConfs plugin injecting pin configurations
- Added AD1988B codec support (by Rodion)
- Added ALC269VC (Lenovo Z580) codec support (by John)
- Added ALC269 (Acer Aspire) codec support (by Andrey1970)
- Added ALC282 codec support (by S.Mitrofanov)
- Added ALC887 codec support (by x-tropic)
- Added ALC888 codec support (by Vandroiy)
- Added ALC889 codec support (by Ukr55)
- Added ALC892 codec support (by FredWst, m-dudarev)
- Added ALC898 codec support (by Andrey1970)
- Added CX20590 codec support (by usr-sse2)

#### v1.0.0
- Initial release
