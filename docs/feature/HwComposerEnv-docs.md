# **DrmHwc2-Env-XML配置文件说明**

文件标识：RK-PC-YF-0003

发布版本：V1.2.0

日期：2024-10-21

文件密级：□绝密   □秘密   □内部资料   ■公开

---

**免责声明**

本文档按“现状”提供，瑞芯微电子股份有限公司（“本公司”，下同）不对本文档的任何陈述、信息和内容的准确性、可靠性、完整性、适销性、特定目的性和非侵权性提供任何明示或暗示的声明或保证。本文档仅作为使用指导的参考。

由于产品版本升级或其他原因，本文档将可能在未经任何通知的情况下，不定期进行更新或修改。

**商标声明**

“Rockchip”、“瑞芯微”、“瑞芯”均为本公司的注册商标，归本公司所有。

本文档可能提及的其他所有注册商标或商标，由其各自拥有者所有。

**版权所有** **© 2024** **瑞芯微电子股份有限公司**

超越合理使用范畴，非经本公司书面许可，任何单位和个人不得擅自摘抄、复制本文档内容的部分或全部，并不得以任何形式传播。

瑞芯微电子股份有限公司

Rockchip Electronics Co., Ltd.

地址：     福建省福州市铜盘路软件园A区18号

网址：     [www.rock-chips.com](http://www.rock-chips.com)

客户服务电话： +86-4007-700-590

客户服务传真： +86-591-83951833

客户服务邮箱： [fae@rock-chips.com](mailto:fae@rock-chips.com)

---

**前言**

本文档主要介绍 DrmHwc2 HwComposerEnv.xml 配置文件说明与具体显示效果

**产品版本**

| **芯片名称** | **系统版本**     |
| ------------ | ---------------- |
| RK3568       | Android  11 / 12 / 13 / 14 |
| RK3588       | Android  11 / 12 / 13 / 14 |
| RK3576       | Android  14                 |

**读者对象**

本文档主要适用于以下工程师：

- 技术支持工程师
- 软件开发工程师

**修订记录**

| **日期**   | **版本** | **作者**  | **修改说明** |
| ---------- | -------- | --------- | ------------ |
| 2022/03/31 | 1.0.0    | GPU图形组 | 初始版本     |
| 2022/03/31 | 1.1.0    | GPU图形组 | 增加旋转与动态切换功能支持 |
| 2024/10/21 | 1.2.0 | GPU图形组 | 1. 新增：支持动态切换同显/拼接/主屏切换功能<br />2. 新增：支持拼接模式屏幕旋转功能 |

**目 录**

[TOC]

## 1 概述

本文档主要介绍 DrmHwc2 HwComposerEnv.xml 配置文件说明，该 xml 配置文件目前主要支持以下功能：

1. 动态切换主屏功能
2. 动态切换以下显示模式：
   - 正常显示模式
   - 多屏拼接模式（支持旋转）
   - 同屏分割模式

**平台支持说明**

| **芯片名称** | **系统版本**               | DrmHwc2版本 | 主屏切换 | 多屏拼接 | 同屏分割 | 备注 |
| ------------ | -------------------------- | ----------- | -------- | -------- | -------- | ---- |
| RK3568       | Android  11 / 12 / 13 / 14 | 1.5.169     | 支持     | 支持     | 支持     |      |
| RK3588       | Android  11 / 12 / 13 / 14 | 1.5.169     | 支持     | 支持     | 支持     |      |
| RK3576       | Android  14                | 1.5.169     | 支持     | 支持     | 支持     |      |

 DrmHwc2 版本号获取命令如下：

```shell
$ adb shell getprop | grep ghwc
[vendor.ghwc.version]: [HWC2-1.5.169]
```

## 2 功能说明

以RK3588芯片为例，外接屏幕为信息如下：

- HDMI-A-1：3840x2160p60
- HDMI-A-2：3840x2160p60
- HDMI-A-3： 3840x2160p60
- DP-1：1920x720p60

#### 2.1 正常显示模式

正常显示模式为各屏幕独立送显，内容互不关联，根据实际应用的请求可实现 **四屏同显** 或者 **四屏异显** 如下图：

```
+----------------+ +----------------+
|                | |                |
|    HDMI-A-1    | |    HDMI-A-2    |
|                | |                |
+----------------+ +----------------+

+----------------+
|                | +----------------+
|    HDMI-A-3    | |      DP-1      |
|                | |                |
+----------------+ +----------------+

```

大多数Android产品均为上述的送线模式。

#### 2.2 动态切换主屏模式

动态切换主屏功能开发需求如下：

设备开机未接实际物理屏，需要根据用户首次热插拔接入的设备进行动态切换

- **当前存在问题：**开机未接实际物理屏幕，则系统任意指定HDMI-A-1作为主屏，分辨率为缺省值 1920x1080p60，后续客户接入DP-1口分辨率为 1920x720p60，DP-1作为副屏接入，由于主屏分辨率为1920x1080与2560x1200宽高比不同，就会导致DP-1显示内容出现黑边，影响观感。
- **解决方案：**开机未接实际物理屏幕，则系统任意指定HDMI-A-1作为主屏，分辨率为缺省值 1920x1080p60，后续客户接入DP-1口分辨率为 1920x720p60，系统识别主屏未连接，启动进行主屏切换，将DP-1设置为主屏，并且将系统UI渲染分辨率切换为 1920x720，解决黑边问题。

具体切换如下示意图：

```shell
+----------------+
|                |      +----------------+
|    HDMI-A-1    |  =>  |      DP-1      |
| (Disconnected) |      |   (Connected)  |
|                |      +----------------+
+----------------+
```

#### 2.3 多屏拼接模式

多屏拼接模式为各屏幕对同一张大画布的图像不同区域进行显示，实现多屏拼接功能，如下图：

```shell
IMAGE: 11520x2160
+----------------+----------------+----------------+
|   ------------------------------------------>    |
|                                                  |
|                                                  |
+----------------+----------------+----------------+

Display: 屏幕分别显示 IMAGE 不同区域，组成三屏拼接
    3840x2160    +   3840x2160    +   3840x2160
+----------------+----------------+----------------+
|   -------------|----------------|----------->    |
|    HDMI-A-1    |   HDMI-A-2     |      DP-1      |
|                |                |                |
+----------------+----------------+----------------+
```

目前版本还支持多屏拼接旋转，即每个拼接屏幕可支持独立旋转，如下图所示：

```shell
IMAGE: 2160x11520
+----------+
|        ^ |
|        | |
|        | |
|        | |
+        | +
|        | |
|        | |
|        | |
|        | |
+        | +
|        | |
|        | |
|        | |
|          |
+----------+

Display: 屏幕分别显示 IMAGE 不同区域，组成三屏拼接
  3840x2160(R90) + 3840x2160(R90) +  3840x2160(R90)
+----------------+----------------+----------------+
|  <-------------|----------------|-------------   |
|    HDMI-A-1    |   HDMI-A-2     |      DP-1      |
|                |                |                |
+----------------+----------------+----------------+
```

可配合底层 Vop Split 功能，可进一步将一路图像左右分割为两路图像信号，分别点亮两路后级的物理屏幕，可实现的拼接功能典型组合如下：

- Vop Split功能关闭：

| 横向拼接                 | 竖向拼接                 |
| ------------------------ | ------------------------ |
| **3×1 ( 11520 x 2160 )** | **1×3 ( 2160 x 11520 )** |
| **2x1（7680 x 2160）**   | **1x2（2160 x 7680）**   |

- Vop Split功能使能：

| 横向拼接                 | 竖向拼接                 |
| ------------------------ | ------------------------ |
| **7×1 ( 13440 × 1080 )** | **1x7 ( 1080 x 13440 )** |
| **6×1 ( 11520 × 1080 )** | **1x6 ( 1080 × 11520 )** |
| **3×2 ( 5760 × 2160 )**  | **2x3 ( 2160 × 5760 )**  |
| **2×3 ( 3840 × 3240 )**  | **3x2 ( 3240 × 3840 )**  |

#### 2.4 同屏分割模式

同屏分割模式，为在一个独立的 3840x2160p60 屏幕中，向上层软件上报两个独立的 1920x2160p60 屏幕，如下图：

```
  3840x2160p60            1920x2160-Left      1920x2160-Right       3840x2160p60
+----------------+         +--------+         +--------+       +-----------------+
|                |         | -----> |         | -----> |       | -----> | -----> |
|    HDMI-A-1    |   =>    |        |    +    |        |   =   |        |        |
|                |         |        |         |        |       |        |        |
+----------------+         +--------+         +--------+       +-----------------+
```

通常此功能需要配合底层Vop Split 功能，将 Left / Right 图像送显到独立的硬件物理屏，实现单VP点亮两个屏幕功能（类似MIPI双通道）。基于此功能 RK3588 可最大实现 7 屏幕异显。

典型的分辨率如下：

| 异显模式    | 典型分辨率               |
| ----------- | ------------------------ |
| **7屏异显** | **7×1 ( 13440 × 1080 )** |
| **6屏异显** | **6×1 ( 11520 × 1080 )** |

## 3 配置说明

### 3.1 XML配置文件说明

XML 配置文件遵循以下流程：

1. 切换XML配置文件路径
2. 设置XML更新标志，通知Composer服务更新XML配置

##### 3.1.1 XML 文件切换方法

目前配置是通过设备端 HwComposerEnv.xml 文件配置，此文件由系统 Composer 服务解析并完成显示模式切换，文件位置如下：

```shell
# SDK 工程文件默认路径：
hardware/rockchip/hwcomposer/drmhwc2/configs/HwComposerEnv.xml

# 设备端文件默认路径：
/vendor/etc/HwComposerEnv.xml
```

设备端的文件路径可以通过以下 Android Property 属性修改：

```c++
// hardware/rockchip/hwcomposer/drmhwc2/include/rockchip/drmxml.h
#define DRM_XML_PATH_NAME "vendor.hwc.env_xml_path"  // default value is "/vendor/etc/HwComposerEnv.xml"
#define DRM_XML_PATH_SYS_NAME "persist.sys.hwc.env_xml_path"  // 如果是system的服务，请设置 sys 属性
#define DRM_XML_PATH_VENDOR_NAME "persist.vendor.hwc.env_xml_path"  // 如果是 vendor 服务，请设置 vendor 属性
```

用户可根据实际的项目，修改上述属性值中的其中一个来实现切换xml配置文件的目的，根据system/vendor服务来选择对应属性进行设置。

**建议：**将需要切换的显示模式分别保存为不同的 xml 文件，通过修改实际读取的xml文件路径，实现显示方案的切换。

adb 修改命令如下：

```shell
adb shell setprop persist.sys.hwc.env_xml_path "/vendor/etc/HwComposerEnv2.xml"
```

##### 3.1.2 XML文件生效方法

修改完成上述的XML文件路径后，需要配置XML更新标志，标志采用**递增**的 Android Property 来实现通知，具体属性值如下：

```c++
// hardware/rockchip/hwcomposer/drmhwc2/include/rockchip/drmxml.h
// 针对 system / apk 服务可通过以下属性设置：
#define DRM_XML_SYS_UPDATE "sys.hwc.display_pipeline_timeline" // 如果是 system 的服务，请设置 sys 属性
#define DRM_XML_VENDOR_UPDATE "vendor.hwc.display_pipeline_timeline" // 如果是 vendor 服务，请设置 vendor 属性
```

设置要求：建议每次需求更新均在原属性值的基础上+1，再设置回属性。即要求 timeline 前后具体的value值有差异即可，adb 命令举例如下：

```shell
## 第一次修改 xml 文件路径
adb shell setprop persist.sys.hwc.env_xml_path "/vendor/etc/HwComposerEnv.xml"  ## 修改 xml 路径
adb shell setprop sys.hwc.display_pipeline_timeline 1 ## 原始值未设置，默认为0，故设置1，存在更新系统就会去更新xml文件

## 第二次修改 xml 文件路径
adb shell setprop persist.sys.hwc.env_xml_path "/vendor/etc/HwComposerEnv2.xml"  ## 修改 xml 路径
adb shell setprop sys.hwc.display_pipeline_timeline 2 ## 修改为2，前后存在变化，则系统尝试更新 xml 配置
```

#####  3.1.2 XML参数说明

- **配置示例：**

```xml
<?xml version="1.0" encoding="utf-8"?>
<!-- HwComposerEnv module xml -->
<HwComposerEnv Version="1.2.0" Enable="1">
  <DsiplayMode Mode="1" FbWidth="11520" FbHeight="1080">
    <Connector>
      <Type>HDMI-A</Type>
      <TypeId>1</TypeId>
      <SrcX>0</SrcX>    <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>3840</SrcW> <!-- Framebuffer Width -->
      <SrcH>1080</SrcH> <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved -->
      <DstY>0</DstY>    <!-- unuse , reserved -->
      <DstW>0</DstW>    <!-- unuse , reserved -->
      <DstH>0</DstH>    <!-- unuse , reserved -->
      <Transform>0</Transform><!-- Screen Transform -->
      <Primary>0</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>0</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
    <Connector>
      <Type>HDMI-A</Type>
      <TypeId>2</TypeId>
      <SrcX>3840</SrcX> <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>1920</SrcW> <!-- Framebuffer Width -->
      <SrcH>1080</SrcH> <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved-->
      <DstY>0</DstY>    <!-- unuse , reserved-->
      <DstW>0</DstW>    <!-- unuse , reserved-->
      <DstH>0</DstH>    <!-- unuse , reserved-->
      <Transform>0</Transform><!-- Screen Transform -->
      <Primary>0</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>0</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
    <Connector>
      <Type>HDMI-A</Type>
      <TypeId>3</TypeId>
      <SrcX>5760</SrcX> <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>1920</SrcW> <!-- Framebuffer Width -->
      <SrcH>1080</SrcH> <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved-->
      <DstY>0</DstY>    <!-- unuse , reserved-->
      <DstW>0</DstW>    <!-- unuse , reserved-->
      <DstH>0</DstH>    <!-- unuse , reserved-->
      <Transform>0</Transform> <!-- Screen Transform -->
      <Primary>0</Primary>     <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>0</Extend>       <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
    <Connector>
      <Type>DP</Type>
      <TypeId>1</TypeId>
      <SrcX>7680</SrcX> <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>3840</SrcW> <!-- Framebuffer Width -->
      <SrcH>1080</SrcH> <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved-->
      <DstY>0</DstY>    <!-- unuse , reserved-->
      <DstW>0</DstW>    <!-- unuse , reserved-->
      <DstH>0</DstH>    <!-- unuse , reserved-->
      <Transform>0</Transform>  <!-- Screen Transform -->
      <Primary>0</Primary>      <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>0</Extend>        <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
  </DsiplayMode>
</HwComposerEnv>

```

- **HwComposerEnv 参数说明：**

|字段名| 字段说明 | 可选值 | 典型值 |
|---|---|---|---|
|**Version**| XML 文件版本号 | 1.2.0 |  |
|**Enable**| XML 文件使能标志 | 0：不使用XML文件<br />1：使能该文件 |  |

|DsiplayMode：| 字段说明 | 可选值 | 备注 |
|---|---|---|---|
|**Mode**|**显示模式**|**0**: 正常模式<br />**1**: 多屏拼接 <br />**2**: 多屏异显||
|**FbWidth**|**渲染水平分辨率**|5760|仅多屏拼接模式生效|
|**FbHeight**|**渲染垂直分辨率**|2160|仅多屏拼接模式生效|

|Connector：|字段说明|典型值|备注|
|-|-|-|-|
|**Type**|**ConnectorType**|HDMI-A|可通过modetest查询，见注1|
|**TypeId**|**TypeID**|1|可通过modetest查询|
|**SrcX**|**FrameBuffer x偏移**|0||
|**SrcY**|**FrameBuffer x偏移**|0||
|**SrcW**|**FrameBuffer 宽**|1920||
|**SrcH**|**FrameBuffer 高**|1080||
|**DstX**|**预留目标区域x偏移**|0|暂未使用|
|**DstY**|**预留目标区域y偏移**|0|暂未使用|
|**DstW**|**预留目标区域宽**|0|暂未使用|
|**DstH**|**预留目标区域高**|0|暂未使用|
|**Transform**|**屏幕旋转**|0|见注2|
|**Primary**|**主屏配置:**|1|见注3|
|**Extend**|**副屏配置:**|0|见注3|

* **注1-ConnectorType字段补充说明：**可使用modetest工具获取ConnectorType和TypeID：\
  工具源码位于external/libdrm/tests/modetest\
  编译后目标文件位于$OUT/data/nativetest64/modetest/modetest

  ```
  adb shell modetest -c
  Connectors:
  id      encoder status          name            size (mm)       modes   encoders
  409     408     connected       HDMI-A-1        700x390         26      408
  ```

* **注2-Transform补充说明：**旋转配置如下，旋转的应用顺序为：先应用翻转，后应用旋转

  * **1**: 水平翻转
  * **2**: 垂直翻转
  * **4**: 90°旋转
  * **3**: 180°旋转（水平翻转+垂直翻转）
  * **7**: 270°旋转（水平翻转+垂直翻转+90°旋转）

* **注3-主屏副屏补充说明：**Primary/Extend 有效值为 > 0，值越小，优先级越高。例如1/2，同时连接，1成为主屏。

  * 动态切换主屏/同屏异显：仅修改Primary/Extend参数即可实现主屏切换；
  * 多屏拼接：建议固定一个屏幕作为拼接主屏，其余均设置为副屏幕；


### 3.2 XML配置示例

#### 3.2.1 6屏拼接（3x2）

##### 3.2.1.1 硬件环境

终端设备为6台1080p HDMI电视，芯片内部注册为4个Connector设备，对应的ConnectorType-TypeId与分辨率如下：

| ConnectorType-TypeId | 设备分辨率   | Vop-SplitMode | 备注                                     |
| -------------------- | ------------ | ------------- | ---------------------------------------- |
| HDMI-A-1             | 3840x1080p60 | 开启          | Vop-SplitMode将分割为2路1920x1080p60信号 |
| HDMI-A-2             | 1920x1080p60 | 关闭          | 无                                       |
| DP-1                 | 3840x1080p60 | 开启          | Vop-SplitMode将分割为2路1920x1080p60信号 |
| HDMI-A-3             | 1920x1080p60 | 关闭          | 无                                       |

拼接示意图如下：

```shell
## 其中HDMI-A-1/DP-1 使能 SplitMode, 将3840x1080p60分割为左右两个1920x1080p60输出
## 故最终拼接成 1920x1080p60 3x2 的拼接输出
## 下图方框代表一路 1920x1080p60 输出
+----------------+----------------+----------------+
|                |                |                |
|    HDMI-A-1    |    HDMI-A-1    |    HDMI-A-2    |
|     (Left)     |     (Right)    |                |
+----------------+----------------+----------------+
|                |                |                |
|      DP-1      |     DP-1       |    HDMI-A-3    |
|     (Left)     |    (Right)     |                |
+----------------+----------------+----------------+
```

##### 3.2.1.2 XML 配置(详细介绍)

下面以水平模式 3x2 模式说明配置文件：

```xml
<?xml version="1.0" encoding="utf-8"?>
<!-- HwComposerEnv module xml -->
<HwComposerEnv Version="1.2.0" Enable="1">
  <!--
    DsiplayMode:
      Mode: 0=None 1=Slicing 2=Presentation
      FbWidth: Framebuffer Width
      FbHeight: Framebuffer Height
      ConnectorCnt: display count
  -->
  <DsiplayMode Mode="1" FbWidth="5760" FbHeight="2160">
    <Connector>
      <Type>HDMI-A</Type>
      <TypeId>1</TypeId>
      <SrcX>0</SrcX>    <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>3840</SrcW> <!-- Framebuffer Width -->
      <SrcH>1080</SrcH> <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved -->
      <DstY>0</DstY>    <!-- unuse , reserved -->
      <DstW>0</DstW>    <!-- unuse , reserved -->
      <DstH>0</DstH>    <!-- unuse , reserved -->
      <Transform>0</Transform><!-- Screen Transform -->
      <Primary>1</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>0</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
    <Connector>
      <Type>HDMI-A</Type>
      <TypeId>2</TypeId>
      <SrcX>3840</SrcX> <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>1920</SrcW> <!-- Framebuffer Width -->
      <SrcH>1080</SrcH> <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved-->
      <DstY>0</DstY>    <!-- unuse , reserved-->
      <DstW>0</DstW>    <!-- unuse , reserved-->
      <DstH>0</DstH>    <!-- unuse , reserved-->
      <Transform>0</Transform><!-- Screen Transform -->
      <Primary>0</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>1</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
    <Connector>
      <Type>DP</Type>
      <TypeId>1</TypeId>
      <SrcX>0</SrcX>    <!-- Framebuffer x 0ffset -->
      <SrcY>1080</SrcY> <!-- Framebuffer y 0ffset -->
      <SrcW>3840</SrcW> <!-- Framebuffer Width -->
      <SrcH>1080</SrcH> <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved-->
      <DstY>0</DstY>    <!-- unuse , reserved-->
      <DstW>0</DstW>    <!-- unuse , reserved-->
      <DstH>0</DstH>    <!-- unuse , reserved-->
      <Transform>0</Transform><!-- Screen Transform -->
      <Primary>0</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>1</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
    <Connector>
      <Type>HDMI-A</Type>
      <TypeId>3</TypeId>
      <SrcX>3840</SrcX> <!-- Framebuffer x 0ffset -->
      <SrcY>1080</SrcY> <!-- Framebuffer y 0ffset -->
      <SrcW>1920</SrcW> <!-- Framebuffer Width -->
      <SrcH>1080</SrcH> <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved-->
      <DstY>0</DstY>    <!-- unuse , reserved-->
      <DstW>0</DstW>    <!-- unuse , reserved-->
      <DstH>0</DstH>    <!-- unuse , reserved-->
      <Transform>0</Transform><!-- Screen Transform -->
      <Primary>0</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>1</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
  </DsiplayMode>
</HwComposerEnv>
```

- **DsiplayMode：**

```xml
 <DsiplayMode Mode="1" FbWidth="5760" FbHeight="2160">
```

Mode 设置为1=Slicing，拼接模式

FbWidth / FbHeight，系统渲染分辨率，完整的图像尺寸，具体数值通过以下方式计算得来：

```
FbWidth = 1920 * 3 = 5760  // 3x2 布局的拼接屏幕
FbHeight= 1080 * 2 = 2160  // 3x2 布局的拼接屏幕
```

- **Connector：**

  ```xml
      <Connector>
        <Type>HDMI-A</Type>
        <TypeId>1</TypeId>
        <SrcX>0</SrcX>    <!-- Framebuffer x 0ffset -->
        <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
        <SrcW>3840</SrcW> <!-- Framebuffer Width -->
        <SrcH>1080</SrcH> <!-- Framebuffer Height-->
        <DstX>0</DstX>    <!-- unuse , reserved -->
        <DstY>0</DstY>    <!-- unuse , reserved -->
        <DstW>0</DstW>    <!-- unuse , reserved -->
        <DstH>0</DstH>    <!-- unuse , reserved -->
        <Transform>0</Transform>     <!-- Screen Transform -->
      </Connector>
  ```

  - **ConnectorType 与 TypeId 信息配置：**

  Connector配置需要的所有信息均可通过 modetest 工具获得，modetest的具体信息如下：

  ```shell
  # modetest 编译
  mmm external/libdrm/tests/modetest
  # modetest 输出目录
  $OUT/data/nativetest64/modetest/modetest  # 64位
  $OUT/data/nativetest/modetest/modetest    # 32位

  # 获取Drm Driver注册的所有Connector信息
  adb shell modetest -c > modetest-connector.log

  # 具体日志如下：
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Connectors:
  id      encoder status          name            size (mm)       modes   encoders
  409     408     connected       HDMI-A-1        700x390         26      408
   modes:
   index name      refresh (Hz) hdisp hss  hse  htot vdisp vss  vse  vtot
   #0    3840x1080 60.00 3840  4016 4104 4400 1080  1084 1089 1125 297000 flags: ...
   (...)

  419      418      connected        HDMI-A-2         510x290          10      418
   modes:
   index name      refresh (Hz) hdisp hss  hse  htot vdisp vss  vse  vtot
      #0    1920x1080 60.00        1920  2008 2052 2200 1080  1084 1089 1125 148500 flags: ...
   (...)

  421      420      connected        HDMI-A-3         510x290          10      420
   modes:
   index name      refresh (Hz) hdisp hss  hse  htot vdisp vss  vse  vtot
      #0    1920x1080 60.00        1920  2008 2052 2200 1080  1084 1089 1125 148500 flags: ...
   (...)

  423      422      connected        DP-1             1020x290        12      422
   modes:
   index name     refresh (Hz) hdisp hss  hse  htot vdisp vss  vse  vtot
   #1   3840x1080 60.00        3840  4016 4104 4400 1080  1084 1089 1125 297000 flags: ..
              - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  #  ConnectorType 与 TypeId 从 Connector name 描述中获取：HDMI-A-1 / HDMI-A-2 / HDMI-A-3 / DP-1
  #  对应关系如下：
  #     HDMI-A-1：ConnectorType = HDMI-A , TypeId = 1
  #          HDMI-A-2：ConnectorType = HDMI-A , TypeId = 2
  #          HDMI-A-3：ConnectorType = HDMI-A , TypeId = 3
  #          DP-1    ：ConnectorType = DP     , TypeId = 1
  ```

  - **SrcX/Y/W/H 与 DstX/Y/W/H 图形参数配置：**

  DstX/Y/W/H 参数目前 Version 1.2.0 版本没有使用，故当前版本仅介绍 SrcX/Y/W/H 配置：

  系统渲染的分辨率设置为 5760x2160，那么对应Connector 显示的图像区域如下：

  ```
  (0,0)                          (3840,0)       (5760,0)
  +------------------------------+--------------+
  |                              |              |
  |           HDMI-A-1           |   HDMI-A-2   |
  |(0,1080)                      |(3840,1080)   |(5760,1080)
  +------------------------------+--------------+
  |                              |              |
  |             DP-1             |   HDMI-A-3   |
  |(0,2160)                      |(3840,2160)   |(5760,2160)
  +------------------------------+--------------+
  ```

  根据上图，就能够很容易的得出4个 Connector 的 Src Info坐标：

  | ConnectorType-TypeId | SrcX | SrcY | SrcW | SrcH |
  | -------------------- | ---- | ---- | ---- | ---- |
  | HDMI-A-1             | 0    | 0    | 3840 | 1080 |
  | HDMI-A-2             | 3840 | 0    | 1920 | 1080 |
  | DP-1                 | 0    | 1080 | 3840 | 1080 |
  | HDMI-A-3             | 3840 | 1080 | 1920 | 1080 |

  - **Transform：**当前应用场景不需要设置旋转，故设置为 0；
  - **Primary：** HDMI-A-1 这是为拼接主屏
  - **Extend：**其余均设置为拼接副屏幕

#### 3.2.2 6屏异显

##### 3.2.2.1 硬件环境

终端设备为6台1080p HDMI电视，芯片内部注册为4个Connector设备，对应的ConnectorType-TypeId与分辨率如下：

| ConnectorType-TypeId | 设备分辨率   | Vop-SplitMode | 备注                                     |
| -------------------- | ------------ | ------------- | ---------------------------------------- |
| HDMI-A-1             | 3840x1080p60 | 开启          | Vop-SplitMode将分割为2路1920x1080p60信号 |
| HDMI-A-2             | 1920x1080p60 | 关闭          | 无                                       |
| DP-1                 | 3840x1080p60 | 开启          | Vop-SplitMode将分割为2路1920x1080p60信号 |
| HDMI-A-3             | 1920x1080p60 | 关闭          | 无                                       |

异显示意图如下：

```shell
## 其中HDMI-A-1/DP-1 使能 SplitMode, 将3840x1080p60分割为左右两个1920x1080p60输出
## 故最终拼接成 1920x1080p60 3x2 的拼接输出
## 下图方框代表一路 1920x1080p60 输出
+----------------+ +----------------+ +----------------+
|                | |                | |                |
|    HDMI-A-1    | |    HDMI-A-1    | |    HDMI-A-2    |
|       (1)      | |       (2)      | |       (3)      |
+----------------+ +----------------+ +----------------+
+----------------+ +----------------+ +----------------+
|                | |                | |                |
|      DP-1      | |     DP-1       | |    HDMI-A-3    |
|       (4)      | |      (5)       | |       (6)      |
+----------------+ +----------------+ +----------------+
```

##### 3.2.2.2 XML 配置

由于 HDMI-A-1 / DP-1 使能 Vop-SplitMode，故需要系统端构造左右半屏幕的同屏异显画面，故XML配置仅需要配置 HDMI-A-1 / DP-1 即可

```xml
<?xml version="1.0" encoding="utf-8"?>
<!-- HwComposerEnv module xml -->
<HwComposerEnv Version="1.2.0" Enable="1">
  <!--
    DsiplayMode:
      Mode: 0=None 1=Slicing 2=Presentation
      FbWidth: Framebuffer Width
      FbHeight: Framebuffer Height
      ConnectorCnt: display count
  -->
  <DsiplayMode Mode="2" FbWidth="0" FbHeight="0">
    <Connector>
      <Type>HDMI-A</Type>
      <TypeId>1</TypeId>
      <SrcX>0</SrcX>    <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>0</SrcW>    <!-- Framebuffer Width -->
      <SrcH>0</SrcH>    <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved -->
      <DstY>0</DstY>    <!-- unuse , reserved -->
      <DstW>0</DstW>    <!-- unuse , reserved -->
      <DstH>0</DstH>    <!-- unuse , reserved -->
      <Transform>0</Transform><!-- Screen Transform -->
      <Primary>1</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>0</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
    <Connector>
      <Type>DP</Type>
      <TypeId>1</TypeId>
      <SrcX>0</SrcX>    <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>0</SrcW>    <!-- Framebuffer Width -->
      <SrcH>0</SrcH>    <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved-->
      <DstY>0</DstY>    <!-- unuse , reserved-->
      <DstW>0</DstW>    <!-- unuse , reserved-->
      <DstH>0</DstH>    <!-- unuse , reserved-->
      <Transform>0</Transform><!-- Screen Transform -->
      <Primary>0</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>1</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
  </DsiplayMode>
</HwComposerEnv>
```

#### 3.2.3 主屏切换

##### 3.2.3.1 硬件环境

终端设备为6台1080p HDMI电视，芯片内部注册为4个Connector设备，对应的ConnectorType-TypeId与分辨率如下：

| ConnectorType-TypeId | 设备分辨率   | 备注 |
| -------------------- | ------------ | ---- |
| HDMI-A-1             | 3840x2160p60 | 无   |
| DP-1                 | 2560x1200p60 | 无   |

```shell
## 主屏由HDMI-A-1 切换为 DP-1
+----------------+
|                |     +----------------+
|    HDMI-A-1    | =>  |      DP-1      |
|   (Connected)  |     |   (Connected)  |
|                |     +----------------+
+----------------+
```

##### 3.2.3.2 XML 配置

需要使用两组XML配置：

- /vendor/etc/HwComposerEnv1.xml

```xml
<?xml version="1.0" encoding="utf-8"?>
<!-- HwComposerEnv module xml -->
<HwComposerEnv Version="1.2.0" Enable="1">
  <!--
    DsiplayMode:
      Mode: 0=None 1=Slicing 2=Presentation
      FbWidth: Framebuffer Width
      FbHeight: Framebuffer Height
      ConnectorCnt: display count
  -->
  <DsiplayMode Mode="0" FbWidth="0" FbHeight="0">
    <Connector>
      <Type>HDMI-A</Type>
      <TypeId>1</TypeId>
      <SrcX>0</SrcX>    <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>0</SrcW>    <!-- Framebuffer Width -->
      <SrcH>0</SrcH>    <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved -->
      <DstY>0</DstY>    <!-- unuse , reserved -->
      <DstW>0</DstW>    <!-- unuse , reserved -->
      <DstH>0</DstH>    <!-- unuse , reserved -->
      <Transform>0</Transform><!-- Screen Transform -->
      <Primary>1</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>0</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
    <Connector>
      <Type>DP</Type>
      <TypeId>1</TypeId>
      <SrcX>0</SrcX>    <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>0</SrcW>    <!-- Framebuffer Width -->
      <SrcH>0</SrcH>    <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved-->
      <DstY>0</DstY>    <!-- unuse , reserved-->
      <DstW>0</DstW>    <!-- unuse , reserved-->
      <DstH>0</DstH>    <!-- unuse , reserved-->
      <Transform>0</Transform><!-- Screen Transform -->
      <Primary>0</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>1</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
  </DsiplayMode>
</HwComposerEnv>
```

- /vendor/etc/HwComposerEnv2.xml

```xml
<?xml version="1.0" encoding="utf-8"?>
<!-- HwComposerEnv module xml -->
<HwComposerEnv Version="1.2.0" Enable="1">
  <!--
    DsiplayMode:
      Mode: 0=None 1=Slicing 2=Presentation
      FbWidth: Framebuffer Width
      FbHeight: Framebuffer Height
      ConnectorCnt: display count
  -->
  <DsiplayMode Mode="0" FbWidth="0" FbHeight="0">
    <Connector>
      <Type>HDMI-A</Type>
      <TypeId>1</TypeId>
      <SrcX>0</SrcX>    <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>0</SrcW>    <!-- Framebuffer Width -->
      <SrcH>0</SrcH>    <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved -->
      <DstY>0</DstY>    <!-- unuse , reserved -->
      <DstW>0</DstW>    <!-- unuse , reserved -->
      <DstH>0</DstH>    <!-- unuse , reserved -->
      <Transform>0</Transform><!-- Screen Transform -->
      <Primary>0</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>1</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
    <Connector>
      <Type>DP</Type>
      <TypeId>1</TypeId>
      <SrcX>0</SrcX>    <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>0</SrcW>    <!-- Framebuffer Width -->
      <SrcH>0</SrcH>    <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved-->
      <DstY>0</DstY>    <!-- unuse , reserved-->
      <DstW>0</DstW>    <!-- unuse , reserved-->
      <DstH>0</DstH>    <!-- unuse , reserved-->
      <Transform>0</Transform><!-- Screen Transform -->
      <Primary>1</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>0</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
  </DsiplayMode>
</HwComposerEnv>
```

#### 3.2.3 4屏旋转拼接（2x2）

##### 3.2.1.1 硬件环境

终端设备为4台1080p HDMI电视，芯片内部注册为4个Connector设备，对应的ConnectorType-TypeId与分辨率如下：

| ConnectorType-TypeId | 设备分辨率   | Vop-SplitMode | 备注 |
| -------------------- | ------------ | ------------- | ---- |
| HDMI-A-1             | 1920x1080p60 | 关闭          | 无   |
| HDMI-A-2             | 1920x1080p60 | 关闭          | 无   |
| DP-1                 | 1920x1080p60 | 关闭          | 无   |
| HDMI-A-3             | 1920x1080p60 | 关闭          | 无   |

拼接示意图如下，所有1920x1080，竖向摆放：

```shell
## 下图方框代表一路 1920x1080p60 输出，竖向摆放
+-----------+-----------+
|           |           |
| HDMI-A-1  | HDMI-A-2  |
|           |           |
|           |           |
|           |           |
+-----------+-----------+
|           |           |
|    DP-1   | HDMI-A-3  |
|           |           |
|           |           |
|           |           |
+-----------+-----------+
```

##### 3.2.1.2 XML 配置(详细介绍)

下面以水平模式 2x2 模式说明配置文件：

```xml
<?xml version="1.0" encoding="utf-8"?>
<!-- HwComposerEnv module xml -->
<HwComposerEnv Version="1.2.0" Enable="1">
  <!--
    DsiplayMode:
      Mode: 0=None 1=Slicing 2=Presentation
      FbWidth: Framebuffer Width
      FbHeight: Framebuffer Height
      ConnectorCnt: display count
  -->
  <DsiplayMode Mode="1" FbWidth="2160" FbHeight="3840">
    <Connector>
      <Type>HDMI-A</Type>
      <TypeId>1</TypeId>
      <SrcX>0</SrcX>    <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>1080</SrcW> <!-- Framebuffer Width -->
      <SrcH>1920</SrcH> <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved -->
      <DstY>0</DstY>    <!-- unuse , reserved -->
      <DstW>0</DstW>    <!-- unuse , reserved -->
      <DstH>0</DstH>    <!-- unuse , reserved -->
      <Transform>4</Transform><!-- Screen Transform -->
      <Primary>1</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>0</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
    <Connector>
      <Type>HDMI-A</Type>
      <TypeId>2</TypeId>
      <SrcX>1080</SrcX> <!-- Framebuffer x 0ffset -->
      <SrcY>0</SrcY>    <!-- Framebuffer y 0ffset -->
      <SrcW>1080</SrcW> <!-- Framebuffer Width -->
      <SrcH>1920</SrcH> <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved-->
      <DstY>0</DstY>    <!-- unuse , reserved-->
      <DstW>0</DstW>    <!-- unuse , reserved-->
      <DstH>0</DstH>    <!-- unuse , reserved-->
      <Transform>4</Transform><!-- Screen Transform -->
      <Primary>0</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>1</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
    <Connector>
      <Type>DP</Type>
      <TypeId>1</TypeId>
      <SrcX>0</SrcX>    <!-- Framebuffer x 0ffset -->
      <SrcY>1920</SrcY> <!-- Framebuffer y 0ffset -->
      <SrcW>1080</SrcW> <!-- Framebuffer Width -->
      <SrcH>1920</SrcH> <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved-->
      <DstY>0</DstY>    <!-- unuse , reserved-->
      <DstW>0</DstW>    <!-- unuse , reserved-->
      <DstH>0</DstH>    <!-- unuse , reserved-->
      <Transform>4</Transform><!-- Screen Transform -->
      <Primary>0</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>1</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
    <Connector>
      <Type>HDMI-A</Type>
      <TypeId>3</TypeId>
      <SrcX>1080</SrcX> <!-- Framebuffer x 0ffset -->
      <SrcY>1920</SrcY> <!-- Framebuffer y 0ffset -->
      <SrcW>1080</SrcW> <!-- Framebuffer Width -->
      <SrcH>1920</SrcH> <!-- Framebuffer Height-->
      <DstX>0</DstX>    <!-- unuse , reserved-->
      <DstY>0</DstY>    <!-- unuse , reserved-->
      <DstW>0</DstW>    <!-- unuse , reserved-->
      <DstH>0</DstH>    <!-- unuse , reserved-->
      <Transform>4</Transform><!-- Screen Transform -->
      <Primary>0</Primary>    <!-- " >0 " Is Primary, value is priority, The smaller the value, the higher the priority. -->
      <Extend>1</Extend>      <!-- " >0 " Is Extend,  value is priority, The smaller the value, the higher the priority. -->
    </Connector>
  </DsiplayMode>
</HwComposerEnv>
```

### 4 集成说明

集成补丁地址（补丁简报）：https://redmine.rockchip.com.cn/issues/515274  （持续更新）


