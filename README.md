AppleALC
========

[![Build Status](https://github.com/badfellow/AppleALC_Dell_M4800/workflows/CI/badge.svg?branch=master)](https://github.com/badfellow/AppleALC_Dell_M4800/actions) [![Scan Status](https://scan.coverity.com/projects/16166/badge.svg?flat=1)](https://scan.coverity.com/projects/16166)

An open source kernel extension enabling native macOS HD audio for not officially supported codecs without any filesystem modifications.

It's specifically optimized for ALC292 (3226) based upon [acidanthera's repo](https://github.com/acidanthera/AppleALC).

NOTE: only keep ALC292 codes, please do not use for non-ALC292 codes.

Advantage:
- reduced AppleALC.kext size from 2M to 136kB
- shorten the loading time during boot up

*Codes/layout-id Support:*

| layout-ID|Model Name|Comments|
| :-------: | :---- | :----- |
| 12 | common | for general model |
| 15 | LENOVO THINKPAD T450_T450s_X240 | |
| 18 | Dell Precision M4800 | without dock station support|
| 28 | Lenovo T440 | |
| 32 | Lenovo T450s | |
| 55 | Dell Latitude E6540 | with dock station support |
| 59 | Dell Precision M4800 | with dock station support | 

<br />


English (Current)  
[简体中文](https://github.com/badfellow/AppleALC_Dell_M4800/blob/master/README_CN.md)  

#### Features
- Digital and analog audio support starting from the OS installation
- Recovery HD/macOS Installer audio support
- Automated codec detection
- Unsupported audio controller enabling (internal and external)
- Arbitrary kext patching
- Custom platform/layout injection
- Works with SIP / El Capitan
- Currently compatible with 10.8-11

#### Credits
- [Apple](https://www.apple.com) for macOS  
- [Onyx The Black Cat](https://github.com/gdbinit/onyx-the-black-cat) by [fG!](https://reverse.put.as) for the base of the kernel patcher
- [capstone](https://github.com/aquynh/capstone) by [Nguyen Anh Quynh](https://github.com/aquynh) for the disassembler module
- [toleda](https://github.com/toleda), [Mirone](https://github.com/Mirone) and certain others for audio patches and layouts
- [Pike R. Alpha](https://github.com/Piker-Alpha) for [lzvn](https://github.com/Piker-Alpha/LZVN) decompression and certain HDMI patches
- [07151129](https://github.com/07151129) for some code parts and suggestions
- [roddy20](https://github.com/roddy20) for training and research dumps to a patching of codecs
- [vit9696](https://github.com/vit9696) for writing the software and maintaining it
- [Andrey1970AppleLife](https://github.com/Andrey1970AppleLife), [vandroiy2013](https://github.com/vandroiy2013) for maintaining the codec database

#### Installation
The minimal instruction is available on the [wiki](https://github.com/vit9696/AppleALC/wiki).  
The prebuilt binaries are available on [releases](https://github.com/vit9696/AppleALC/releases) page.

#### Contribution
To support more audio codecs in the binary packages you are asked to submit your configurations. Please read the [wiki](https://github.com/vit9696/AppleALC/wiki) for more details. For the contributors with programming skills the headers are filled with AppleDOC comments.

#### Support and discussion
[InsanelyMac topic](http://www.insanelymac.com/forum/topic/311293-applealc-—-dynamic-applehda-patching/) in English  
[AppleLife topic](https://applelife.ru/threads/applealc-dinamicheskij-patching-applehda.1171672/) in Russian

#### Donations
Writing and supporting code is fun but it takes time. If you want to thank the author for his work consider contributing, bugreporting, or providing the support to other users.
