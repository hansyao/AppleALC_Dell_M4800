# Dell Precision M4800 AppleALC声卡驱动修复教程

## 前言

对于我的**Dell Precision M4800**来说，用[acidanthera](https://github.com/acidanthera)的[AppleALC.kext](https://github.com/acidanthera/AppleALC)并不能完全驱动，换了好几个layout-id都没鸟用，具体表现为:
1. Dell Dock 底座的两个声音插孔不能用
2. 左面板的耳机有严重杂音，耳机基本不能正常使用

因此，不得已只好自己修复，特此记录下修复过程如下。

&nbsp;

## 准备工作

首先根据Apple的PinConfig规范自己用Excel（WPS兼容）做了个PinConfigData配置模板工具[pinconfigs.xlsx](https://github.com/badfellow/AppleALC_Dell_M4800/raw/master/pinconfigs.xlsx)以备用。

&nbsp;

### Linux环境 (需先确保Linux下声音正常)

&nbsp;
  
   **工具准备:** 

   安装`hda-tools`:
   此工具我已经同步到我的仓库[AppleALC_Dell_M4800](https://github.com/badfellow/AppleALC_Dell_M4800). Linux下能用到的其实只有`codecgraph`和`verbit.sh`，其他大部分Mac下才能用, 以下有几步也可以在Mac下执行。
   ```
   git clone https://github.com/badfellow/AppleALC_Dell_M4800
   chmod 755 -R ./AppleALC_Dell_M4800/hda-tools
   sudo cp ./AppleALC_Dell_M4800/hda-tools/* /usr/local/bin/
   rm -rf ./AppleALC_Dell_M4800        //删除
   ```
   安装`WPS`: 命令行安装或者直接到[WPS官网](https://linux.wps.cn)下载。(如果只想在Windows或者Mac上用Microsoft Office编辑Excel文件，此步骤略)
   ```
   sudo dnf install https://wdl1.cache.wps.cn/wps/download/ep/Linux2019/10161/wps-office-11.1.0.10161-1.x86_64.rpm    //fedora
   sudo apt install https://wdl1.cache.wps.cn/wps/download/ep/Linux2019/10161/wps-office_11.1.0.10161_amd64.deb    //ubuntu or debian
   ```

1. **提取Codec**
   
   我本来安装了Fedora双系统，所以就直接在Fedora下提取了. 为了纯净提取，先清除NVAM再进入BIOS恢复出厂设置，再在BIOS里手工配置下Fedora的EFI进入Fedora系统。
   进入终端Shell环境
   ```
   cp /proc/asound/card1/codec* ./   // 提取声卡codec到当前路径 (我的板载声卡在card1，有的可能是card0)

   ```

2. **查找所有节点** （此步骤也可在Mac下执行）
   
   整理所有节点并记录到Excel模板`pinconfigs.xlsx`里, Linux环境可以安装个`WPS`。输入命令`verbit.sh codec#0`得到以下屏幕打印信息
   ```
   % ../hda-tools/verbit.sh codec#0

   Verbs from Linux Codec Dump File: codec#0

   Codec: Realtek ALC3226   Address: 0   DevID: 283902610 (0x10ec0292)

      Jack   Color  Description                  Node     PinDefault             Original Verbs
   --------------------------------------------------------------------------------------------------------
   Digital Unknown  Mic at Int N/A              18 0x12   0x90a60140   01271c40 01271d01 01271ea6 01271f90
      1/8   Black  Speaker at Ext Rear         19 0x13   0x411111f0   01371cf0 01371d11 01371e11 01371f41
   Analog Unknown  Speaker at Int N/A          20 0x14   0x90170110   01471c10 01471d01 01471e17 01471f90
      1/8   Green  HP Out at Ext Front         21 0x15   0x0221401f   01571c1f 01571d40 01571e21 01571f02
      1/8   Green  Line Out at Ext Rear        22 0x16   0x01014020   01671c20 01671d40 01671e01 01671f01
   Mic at Ext Front    0x18 0x18                        44142641 01871c31 01871d90     01871ea1 01871f02  
      1/8    Pink  Mic at Ext Rear             25 0x19   0x01a1903e   01971c3e 01971d90 01971ea1 01971f01
      1/8   Black  Speaker at Ext Rear         26 0x1a   0x411111f0   01a71cf0 01a71d11 01a71e11 01a71f41
      1/8   Black  Speaker at Ext Rear         27 0x1b   0x411111f0   01b71cf0 01b71d11 01b71e11 01b71f41
   Unknown Unknown  Modem Hand at Ext N/A       29 0x1d   0x40700001   01d71c01 01d71d00 01d71e70 01d71f40
      1/8   Black  Speaker at Ext Rear         30 0x1e   0x411111f0   01e71cf0 01e71d11 01e71e11 01e71f41
   --------------------------------------------------------------------------------------------------------


      Jack   Color  Description                  Node     PinDefault             Modified Verbs
   --------------------------------------------------------------------------------------------------------
   Digital Unknown  Mic at Int N/A              18 0x12   0x90a60140   01271c40 01271d00 01271ea6 01271f90
   Analog Unknown  Speaker at Int N/A          20 0x14   0x90170110   01471c10 01471d00 01471e17 01471f90
      1/8   Green  HP Out at Ext Front         21 0x15   0x0221401f   01571c50 01571d40 01571e21 01571f01
      1/8   Green  Line Out at Ext Rear        22 0x16   0x01014020   01671c20 01671d40 01671e01 01671f01
   Mic at Ext Front    0x18 0x18                        44142641 01871c30 01871d90     01871ea1 01871f01  
      1/8    Pink  Mic at Ext Rear             25 0x19   0x01a1903e   01971c60 01971d90 01971ea1 01971f01
   Unknown Unknown  Modem Hand at Ext N/A       29 0x1d   0x40700001   01d71c70 01d71d00 01d71e70 01d71f40
   --------------------------------------------------------------------------------------------------------

   Modified Verbs in One Line: 01271c40 01271d00 01271ea6 01271f90 01471c10 01471d00 01471e17 01471f90 01571c50 01571d40 01571e21 01571f01 01671c20 01671d40 01671e01 01671f01 01871c30 01871d90 01871ea1 01871f01 01971c60 01971d90 01971ea1 01971f01 01d71c70 01d71d00 01d71e70 01d71f40
   --------------------------------------------------------------------------------------------------------

   ```
   以上第一部分即列出来来了所有节点为 `0x12, 0x13, 0x14, 0x15, 0x16, 0x18, 0x19, 0x1a, 0x1b, 0x1d, 0x1e`，记录到Excel模板的`PinConfig生成模板`工作表的第一列`节点（HEX)`<十六进制度>， 第二列`节点(DEC)`<十进制>Excel公式会自动生成。
   同时我们得到`Address: 0`，记录到`地址`列。这里的其他`pinDefault/ModifiedVerbs`等信息我们都不需要。

3. **查找有效节点**
   
   然后再执行命令`dmesg | grep hdaaudio` 得到有效节点:
   ```
      % dmesg | grep hdaudio
   [    6.890268] snd_hda_codec_realtek hdaudioC1D0: autoconfig for ALC3226: line_outs=1 (0x16/0x0/0x0/0x0/0x0) type:line
   [    6.890276] snd_hda_codec_realtek hdaudioC1D0:    speaker_outs=1 (0x14/0x0/0x0/0x0/0x0)
   [    6.890279] snd_hda_codec_realtek hdaudioC1D0:    hp_outs=1 (0x15/0x0/0x0/0x0/0x0)
   [    6.890282] snd_hda_codec_realtek hdaudioC1D0:    mono: mono_out=0x0
   [    6.890283] snd_hda_codec_realtek hdaudioC1D0:    inputs:
   [    6.890285] snd_hda_codec_realtek hdaudioC1D0:      Front Mic=0x18
   [    6.890288] snd_hda_codec_realtek hdaudioC1D0:      Dock Mic=0x19
   [    6.890290] snd_hda_codec_realtek hdaudioC1D0:      Headset Mic=0x1a
   [    6.890292] snd_hda_codec_realtek hdaudioC1D0:      Internal Mic=0x12
   ```
   
   得到有效节点为`0x12, 0x14, 0x15, 0x16, 0x18, 0x19, 0x1a`, 记录到Excel模板`是否有效`列, 有效填`是`，无效填`否`。

4. **查找EAPD节点**  （此步骤也可以在Mac下执行）
   
   下一步再执行命令`cat codec#0 | grep "EAPD 0x" | wc -l` 得到返回值3，我们需要找到3个EAPD节点。运行`vim codec#0`查找EAPD所在的节点。
   ```
   % cat codec#0 | grep "EAPD 0x" | wc -l
         3
      vim codec#0
      :EAPD       //直接输入:EAPD回车查找，按’n'键查找下一个

   ```
   最终得到EAPD节点为`0x14, 0x15, 0x16`，填入Excel模板`EAPD`列, 有填`是`，无填`否`。

5. **整理有效路径**    (此步骤也可以在Mac下执行）
   
   执行命令`codecgraph codec\#0`， 生成`codec#0.svg`，直接可以用浏览器打开，也可导入Excel。下图是我的Dell Precision M4800的路径图。
   ![我的Dell Precision M4800声音路径图](https://raw.githubusercontent.com/badfellow/AppleALC_Dell_M4800/master/codec%230.svg)<img src="https://raw.githubusercontent.com/badfellow/AppleALC_Dell_M4800/master/codec%230.svg">
   为了方便我们导入Excel，新建一个工作表`HDA Map`，插入图片，将刚才生成的`codec#0.svg`插入到工作表`HDA Map`。

   现在可以看到`codec#0.svg`节点都是十六进制，由于路径需要10进制，先将所有16进制记录到 `PinConfig生成模板` Excel工作表的`进制转换`单元格，Excel公式`HEX2DEC`会一次性批量计算出对应的十进制数字备用。

   注意箭头方向，从左到右是输入到输出。所以左边麦克风>中间混响设备>右边耳机和喇叭。据此我们可以整理出声音输入路径。

   &nbsp;
      
   最终，历经以上步骤1到步骤5，我们在Excel模板里整理出以下表格.

   | 节点(HEX)	| 节点(DEC)	| 设备名称	| 说明 | 路径 | 地址 | EAPD | 是否有效 |
   | :------: | :------: | :------ | ------ | ------ | :------: | :------: | :-----: |
   | 0x12 | 18 | Digital Unknown Mic at Int N/A | 内置麦克风 | 18>34>9  | 0 | 否 | 是 |
   | 0x13 |	19	| 1/8  Black  Speaker at Ext Rear |	无效 |    | 0 | 否 | 否 |
   | 0x14 | 20	| Analog Unknown Speaker at Int N/A	| 内置喇叭 |  2>12>20  | 0 | 是 | 是 |
   | 0x15 |	21	| 1/8   Green  HP Out at Ext Front  |	左耳麦二合一3.5mm耳机插孔 |  3>13>21, 2>12>21  | 0 | 是 | 是 |
   | 0x16 |	22	| 1/8   Green  Line Out at Ext Rear	| 戴尔Dock底座耳机插孔 |  2>12>22, 3>13>22  | 0 | 是 | 是 |
   | 0x18 |	24	| Mic at Ext Front	| 左耳麦麦克风插孔 | 24>35>8 | 0 | 否 | 是 |
   | 0x19 |	25	| 1/8  Pink  Mic at Ext Rear 	| 戴尔Dock底座3.5mm麦克风插孔 |  25>34>9  | 0 | 否 | 是 |
   | 0x1a |	26	| 1/8  Black  Speaker at Ext Rear| 左耳麦二合一3.5mm麦克风插孔 |  26>34>9  | 0 |否 | 是 |
   | 0x1b |	27	| 1/8  Black  Speaker at Ext Rear | 无效 |    | 0 | 否 | 否 |
   | 0x1d |	29	| Modem Hand at Ext	| 无效 |    | 0 | 否 | 否 |
   | 0x1e |	30	| 1/8  Black  Speaker at Ext Rear |	无效 |    | 0 | 否 | 否 |

   &nbsp;

1. **定制PinConfig**
   
   Excel模板工作表`PinConfig生成模板`的以下几列，按照每个物理接口的实际情况，通过下拉菜单选择合适的类型(如下)， 此Excel模板就会自动生成符合苹果设计规范的`Fina ConfigData`和`WakeConfigData`两个参数。具体操作参见[pinconfigs.xlsx](https://github.com/badfellow/AppleALC_Dell_M4800/raw/master/pinconfigs.xlsx)。
   
    | 节点 | 说明 | 默认关联 |	序列	| 插孔颜色	| 插孔侦测	| 设备类型	| 连接类型	| 端口	| 端口位置 |
    | --- | --- | --- | --- | --- | --- | --- |  --- | --- | --- |
   | 0x12 |内置麦克风|1. Mic	 | 0.后左与后右 | 	0=Unknown | 	1=Jack Detect Disabled ［内部设备］ | 	A=Mic | 	6=Digital | 	9=Internal Fixed Function Device ［内 置设备］ | 	0=N/A
   | 0x13 |无效|F. 屏蔽	 | 0.后左与后右 | 	0=Unknown | 	0=Jack Detect Override ［外接设备］ | 	0=Line Out | 	0=Unknow	 | 4=External No Connection ［屏蔽设备］ | 	0=N/A
   | 0x14 |内置喇叭|4. IntSpeaker	 | 0.后左与后右 | 	0=Unknown | 	1=Jack Detect Disabled ［内部设备］ | 	1=Speaker | 	7=0ther Analog	 | 9=Internal Fixed Function Device ［内 置设备］ | 	0=N/A
   | 0x15 |左耳麦二合一3.5mm耳机插孔|5. Headphone	 | 0.后左与后右 | 	4=Green | 	0=Jack Detect Override ［外接设备］ | 	2=HeadPhone Out | 	B=Combination | 	0=External Jack ［外 接设备］ | 	2=Front
   | 0x16 |戴尔Dock底座耳机插孔|8. Line Out	 | 0.后左与后右 | 	4=Green | 	0=Jack Detect Override ［外接设备］ | 	0=Line Out | 	1=1/8 Stereo/Mono ［也即3.5mm接口］ | 	0=External Jack ［外 接设备］ | 	1=Rear
   | 0x18 |左耳麦麦克风插孔|2. Line In	 | 0.后左与后右 | 	2=Gray | 	0=Jack Detect Override ［外接设备］ | 	8=Line In | 	1=1/8 Stereo/Mono ［也即3.5mm接口］ | 	0=External Jack ［外 接设备］ | 	2=Front
   | 0x19 |戴尔Dock底座3.5mm麦克风插孔|2. Line In	 | 0.后左与后右 | 	9=Pink | 	0=Jack Detect Override ［外接设备］ | 	8=Line In | 	1=1/8 Stereo/Mono ［也即3.5mm接口］ | 	0=External Jack ［外 接设备］ | 	1=Rear
   | 0x1a |左耳麦二合一3.5mm麦克风插孔|2. Line In	 | 0.后左与后右 | 	1=Black | 	0=Jack Detect Override ［外接设备］ | 	F=0ther | 	B=Combination | 	0=External Jack ［外 接设备］	 | 2=Front
   | 0x1b |无效|F. 屏蔽	 | 0.后左与后右	 | 0=Unknown | 	0=Jack Detect Override ［外接设备］ | 	0=Line Out | 	0=Unknow | 	4=External No Connection ［屏蔽设备］ | 	0=N/A
   | 0x1d |无效|F. 屏蔽	 | 0.后左与后右	 | 0=Unknown | 	0=Jack Detect Override ［外接设备］ | 	0=Line Out | 	0=Unknow	 | 4=External No Connection ［屏蔽设备］ | 	0=N/A
   | 0x1e |无效| F. 屏蔽	 | 0.后左与后右	 | 0=Unknown | 	0=Jack Detect Override ［外接设备］ | 	0=Line Out | 	0=Unknow | 	4=External No Connection ［屏蔽设备］ | 	0=N/A 

&nbsp;

至此，我们得到`Fina ConfigData`和`WakeConfigData`如下:
```
Fina ConfigData      <01271C10 01271D01 01271EA6 01271F90 01371CF0 01371D00 01371E00 01371F40 01471C40 01471D01 01471E17 01471F90 01470C02 01571C50 01571D40 01571E2B 01571F02 01570C02 01671C80 01671D40 01671E01 01671F01 01670C02 01871C20 01871D20 01871E81 01871F02 01971C20 01971D90 01971E81 01971F01 01A71C20 01A71D10 01A71EFB 01A71F02 01B71CF0 01B71D00 01B71E00 01B71F40 01D71CF0 01D71D00 01D71E00 01D71F40 01E71CF0 01E71D00 01E71E00 01E71F40>

WakeConfigData    <01470C02 01570C02 01670C02>
```

Linux上能干的活儿我们都干完了，只待进入Mac改写AppleALC了， 因此让我们保存下我们的劳动成果，把这个Excel文件放在Mac系统也能访问的地方，重启电脑进入Mac。

&nbsp;

### Mac环境

&nbsp;

由于我们已经成功得到了`Fina ConfigData`和`WakeConfigData`，在Mac上操作就很简单了，唯一能做的事情就是改写AppleALC并编译。

1. 同步AppleALC源码
   ```
   git clone git@github.com:acidanthera/AppleALC.git
   
   ```
2. 用 `Xcode` 打开AppleALC项目`AppleALC.xcodeproj`，根据自己的系统版本配置一下环境，测试编译通过。

3. 在`AppleALC/Resources/PinConfigs.kext/Contents/info.plist/IOKitPersonalities/HDAConfigDefault/`下添加一组数据，新增一个原ALC292下没有用过的`layout-id`，我这里新增了layout-id 59.
   ```
   AFGLowPowerState  Data     <03000000>
   Codec             String   ALC 292 for Dell M4800 with Dock
   CodecID           Number   283,902,610                            //it's for ALC292
   Comment           String   ALC 292 for Dell M4800 with Dock
   ConfigData        Data     从Excel模板里生成的Final ConfigData复制过来即可
   FuncGroup         Number   1
   LayoutID          Number   59
   WakeConfigData    Data     从Excel模板里生成的Final ConfigData复制过来即可
   WakeVerbReinit    Boolean  1
   ```
4. 在`AppleALC/Resources/ALC292/Info.plist/Files/Layouts/`下添加`layout59.xml.zlib`，同时在`AppleALC/Resources/ALC292/Info.plist/Files/Platforms/`下添加`Platforms59.xml.zlib`。
   (注:此处`ID`非`layoutid`，但建议与`layoutid`保持一致)
   ```
   // AppleALC/Resources/ALC292/Info.plist/Files/Layouts/
   Comments    String      ALC 292 for Dell M4800 with Dock
   Id          Number      59
   Path        String      layout59.xml.zlib

   // AppleALC/Resources/ALC292/Info.plist/Files/Platforms/
   Comments    String      ALC 292 for Dell M4800 with Dock
   Id          Number      59
   Path        String      Platforms59.xml.zlib
   ```
5. 在`AppleALC/Resources/ALC292/`路径中复制其他layout和Platforms文件改名为`layout59.xml`和`Platforms59.xml`
6. 根据我们前面Excel文件生成的路径, 改写 `Platforms59.xml`文件，填入我们自己的路径。(具体改写方法可参考其他Platforms文件的例子)
7. 修改`layout59.xml`文件里的`layoutid`和`PathMapID`为59。(注: `PathMapID`不一定需要和`layoutid`保持一致,但必须和`AppleALC/Resources/ALC292/Info.plist/Files/Layouts/`与`AppleALC/Resources/ALC292/Info.plist/Files/Platforms/`的`ID`保持一致)
   ```
   <key>LayoutID</key>
	<integer>59</integer>

   <key>PathMapID</key>
   <integer>59</integer>

   ```
8. 自此，驱动修改已经全部完成，如果需要精简只留下你需要的声卡型号ALC292，可以删除`AppleALC/Resources`其他所有非ALC292的文件，同时删除`AppleALC/Resources/PinConfigs.kext/Contents/info.plist/IOKitPersonalities/HDAConfigDefault/`下非ALC292的几百个PinConfig配置。

9. 编译打包，将生成的`AppleALC.kext`替换掉/EFI/OC/Kexts/下原来的驱动, 重启测试成功。


&nbsp;


## 致谢

&nbsp;

部分参考
[黑果小兵的部落阁](https://blog.daliansky.net/Use-AppleALC-sound-card-to-drive-the-correct-posture-of-AppleHDA.html)