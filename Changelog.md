AppleALC Changelog
==================

#### v1.0.5
- Fixed AppleHDAController patching regression
- Fixed incorrect ALC269 codec-id (should be 617)
- Added ALC887-VD, layout-id 11-15 codec support (by InsanelyDeepak)
- Added ALC883, layout-id 7 codec support (by Andrey1970)
- Changed resources and pinconfig for ALC888 deleted layout 1 using layout 5 for 3 ports and layout 7 for 5/6 ports 
- Added CX20590 Mirone resources layout-id 3
- Added ALC1150 Mirone resources layout-id 5 5 for 3 ports and layout-id 7 for 5/6 ports 
- Added ALC887-VD Mirone resources layout-id 5 5 for 3 ports and layout-id 7 for 5/6 ports 
- Added ALC882 codec support Mirone resources layout-id 5 5 for 3 ports and layout-id 7 for 5/6 ports
- Changed ALC668 resources and pinconfig Mirone resources
- Added ALC663 Mirone resources v1 - layout-id 3, v2 - layout-id 4
- Changed ALC662 resources to Mirone resources layout-id 5 5 for 3 ports and layout-id 7 for 5/6 ports 
- Added ALC282 Mirone resources v1 - layout-id 3, v2 - layout-id 4
- Sync PinConfig info.plist deleted unnesessary items
- Sync layout names (1,2,3 - Toleda Desktop, 3,4 - Mirone Laptop, 5,7 - Mirone Desktop, 10-99 - Custom User layouts)
- Sync platforms names (platformsT - Toleda, PlatformsM - Mirone, Platforms10-99 - custom user platforms)

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
