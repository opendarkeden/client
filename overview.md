## 一 天之炼狱客户端概述

### 1.1 客户端架构

天之炼狱的客户端源代码主体可以包括3个方面: 底层支持库， UI库， 游戏客户端

底层支持库包括:

* DirectXLib（DirectX/DirectDraw相关接口）
* D3Dlib（Direct3D相关接口）
* MZLib（数据压缩库， 修改自zlib）
* mp3lib（MP3音乐格式支持库）
* JpegLib（JPEG图像格式支持库， 原版使用修改后的jpeglib 5.x， 我已经修改为官方的jpeglib 6.x）
* VolumeLib（系统音量控制支持库）
* BasicS（包括基础的异常处理， 内存管理等）
* mother（基础数学运算库）
* GL（图像文件支持库， 包括tga和pcx）
* Timer（定时器支持库）
* Rbt（RaBbiT Library， 天炼自定的打包文件格式支持库）
* DEUtil（天炼客户端的人物数据算法， 例如， 例如通过基本3围等属性获得当前人物的攻击速度）
* WinLib（和基本Win32 Window的相关封装）
* SpriteLib（天炼精灵库， 包括各种精灵文件格式的处理和绘制相关支持）
* FrameLib（天炼精灵动画库， 包括动画相关处理和控制的支持）

底层库部分还包括其他一些第三方库（无需编译）， 例如: RAR文件格式支持库， ogg格式声音文件支持库等等。

UI库部分包括:

* Widget（各种UI控件的实现）
* Imm（力反馈输入设备接口）
* Hangul（东方语系输入法实现）
* UI基本逻辑以及实例程序

游戏客户端包括:

* 游戏封包及相关封包处理
* 实际游戏逻辑/主程序

### 1.2 客户端编译环境

客户端代码基于Microsoft Visual Studio v6.0编写， 可以容易调整到Microsoft Visual Studio 2003（主要是STL版本方面的变化）。

客户端源代码使用到的第三方库如下:

* DirectX v7.0 SDK（已经调整到可以在dx8， dx9下编译）
* JPEG Library
* RAR Archive Library
* Immersion’s Feelit API
* CPP-Unit（C++ unit testing framework）
* Zlib（Zip Archive）
* G-Audio
* Ogg Sound Library


## 二 天之炼狱客户端源代码底层支持库模块

### 2.1 底层支持库模块概述

实际游戏底层支持库分布在以下三个不同位置:

* 第一， Client目录下， 作为Client模块的子工程， 包括重要的SpiritLib， FrameLib， WindowLib， DirectXLib， D3Dlib等;
* 第二， Lib目录下， 比较简单的一些基础库的源代码， 例如BasicS， mother等;
* 第三， 不在整理的源代码中， 但是基本上可以在Disk2/Disk1中找到， 例如Timer， GL等。

其中某些库是网上可以找到的free或非free的库， 以源代码的方式加入， 实际往往可以用更新的版本加以替换， 包括CPP-Unit， Jpeglib， Zlib， Feelit Library等等

### 2.2 关键类库分析I – DirectX相关接口库

一， DirectX Library

* DirectX Library是天炼DirectX相关的接口封装类库， 涉及DirectDraw， DirectSound， DirectMusic， DirectInput等部分。
* CDirectDraw.cpp: 封装DirectDraw的相关接口和简单调用逻辑到CDirectDraw类。
* CDirectDrawSurface.cpp: 封装DirectDraw的关键概念Surface到CDirectDrawSurface类。
* CDirectInput.cpp: 封装DirectInput的相关接口和简单调用逻辑到CDirectInput类， 已经改为dx8/dx9支持。
* CDirectMusic.cpp: 封装DirectMusic的相关接口和简单调用逻辑到CDirectMusic类。
* CDirectSound.cpp/CDirectSoundStream.cpp: 封装DirectSound的相关接口和简单调用逻辑到CDirectSound类， 包含mp3和ogg的支持。

二， Direct3D Library

天炼客户端中使用D3Dlib的作用是使用3D硬件加速2D绘制速度和效果， 例如2D图像blt， alpha透明等运算等等。

### 2.3 关键类库分析II – 天炼精灵库

* SpriteLib是天炼客户端中非常重要的底层类库， 包括各种精灵文件格式的处理和绘制相关支持。
* CSpriteSurface.cpp: 精灵Surface类， 基于CDirectDrawSurface， 支持了各种天炼精灵类的绘制以及一些基本特效如: AlphaBlending
* CAlphaSprite.cpp: AlphaSprite类， 半透明精灵类。
* CAlphaSpritePack.cpp: AlphaSprite打包文件的读写， 扩展名.aspk。
* CFilter.cpp: 滤镜类， 用于某些图像处理/图像特效算法。
* CFileIndexTable.cpp: 文件的索引表类， 打包文件处理相关。
* CIndexSprite.cpp: IndexSprite类， 索引精灵类。
* CIndexSpritePack.cpp: IndexSprite打包文件的读写， 扩展名.ispk.
* CSprite.cpp: 基本精灵类。
* CSpritePack.cpp: 基本精灵类打包文件的读写， 扩展名.spk。
* CSpritePal.cpp: 基本精灵调色板类。
* MPalettePack.cpp: 调色板打包文件的读写。
* CSpritePalBase.cpp: 各种精灵调色板类的父

## 三 天之炼狱客户端源代码UI库模块

天炼UI库独立于游戏客户端源代码， 包括一个完整的示例程序和静态库， 静态库用于游戏客户端。

UI部分基本上可以分为下列四部分:

根目录: UI基本逻辑以及实例程序

* Widget: 各种UI控件的实现
* Imm: Immersion公司的力反馈输入设备接口API
* Hangul: 东方语系输入法实现

Hangul部分:

* Ci.cpp: 实现了两个类: CI\_CHINESE和CI\_KOREAN。 但是他对于输入法的处理并不完善， 停留在老版本的IMM API。
* FL2.cpp: 东方语系（DBCS， Double-Byte Character Set）文字的绘制。

Widget部分:

* u_window.cpp: window控件基本逻辑和消息处理。
* u_button.cpp: 按钮控件基本逻辑和消息处理。
* u_edit.cpp: 编辑框控件基本逻辑和消息处理。
* u_scrollbar.cpp: 滚动条控件基本逻辑和消息处理。

UI基本逻辑和实例程序部分:

* VS\_UI\_Base.cpp: 天炼UI控件父类。
* VS\_UI\_BBS.cpp: 天炼BBS控件（并未实际实现）。
* VS\_UI\_Description.cpp: 天炼描述文字框控件。
* VS\_UI\_Dialog.cpp: 天炼对话框控件。
* VS\_UI\_ELEVATOR.cpp: 天炼魔灵大地之角的界面。
* VS\_UI\_Exchange.cpp: 天炼物品交易界面。
* VS\_UI\_ExtraDialog.cpp: 天炼特殊对话框界面。
* VS\_UI\_Game.cpp: 天炼游戏整体界面组织。
* VS\_UI\_GameCommon.cpp/vs_ui_gamecommon2。cpp: 天炼游戏整体界面逻辑。
* VS\_UI\_GameOusters.cpp/VS_UI_GameSlayer.cpp/VS_UI_GameVampire.cpp: 三个种族的整体界面。
* VS\_UI\_Item.cpp: 物品绘制相关处理。
* VS\_UI\_Message.cpp: 信息文字（例如公告）显示相关处理。
* VS\_UI\_mouse_pointer.cpp: 鼠标指针。
* VS\_UI\_progress.cpp: 天炼进度条控件。
* VS\_UI\_Shop.cpp: 天炼商店对话界面。
* VS\_UI\_skill_tree.cpp: 天炼技能树界面。
* VS\_UI\_SKILL_VIEW.cpp: 天炼技能信息显示界面。
* VS\_UI\_Storage.cpp: 天炼商店货架界面。
* VS\_UI\_Title.cpp: 进入游戏前的一些界面（登陆框， 服务器选择框， 人物选择框等）。
* VS\_UI\_Tutorial.cpp: 游戏帮助界面。
