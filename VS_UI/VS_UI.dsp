# Microsoft Developer Studio Project File - Name="VS_UI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=VS_UI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VS_UI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VS_UI.mak" CFG="VS_UI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VS_UI - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VS_UI - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/", aaaaaaaa"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VS_UI - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /Od /I "\Game(800)\designed\project\client" /I "\Game(800)\designed\project\client\dxlib" /I "\Game(800)\designed\project\client\d3dlib" /I "\Game(800)\designed\project\client\spriteLib" /I "\Game(800)\designed\project\client\frameLib" /I "src\widget" /I "src\hangul" /I "src\header" /I "src\imm" /I "\Game(800)\designed\project\lib" /I "..\client\sxml" /I "..\xerces\include" /I "..\basic" /I "\Game(800)\designed\project\VS_UI\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "__TEST_SUB_INVENTORY__" /FAcs /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 dsound.lib winmm.lib imm32.lib PS.lib BasicS.lib Timer.lib rbt.lib dinput.lib ddraw.lib dxguid.lib sp.lib fr.lib dx.lib gl.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib rar_release.lib ifc22.lib jpeg.lib /nologo /subsystem:windows /incremental:yes /debug /machine:I386 /libpath:"..\lib" /libpath:"..\lib\ui" /libpath:"h:\study\lib" /libpath:"..\client\jpegLib" /libpath:"..\Client\DirectXLib\Release"
# SUBTRACT LINK32 /map

!ELSEIF  "$(CFG)" == "VS_UI - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "\Game(800)\designed\project\client" /I "\Game(800)\designed\project\client\dxlib" /I "\Game(800)\designed\project\client\d3dlib" /I "\Game(800)\designed\project\client\spriteLib" /I "\Game(800)\designed\project\client\frameLib" /I "src\widget" /I "src\hangul" /I "src\header" /I "src\imm" /I "\Game(800)\designed\project\lib" /I "..\client\sxml" /I "..\xerces\include" /I "..\basic" /I "\Game(800)\designed\project\VS_UI\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__TEST_SUB_INVENTORY__" /Fr /Yu"client_PCH.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dsound.lib winmm.lib imm32.lib PS.lib BasicS.lib Timer.lib dinput.lib ddraw.lib dxguid.lib ..\client\spritelib\debug\sp.lib ..\client\framelib\debug\fr.lib ..\client\DirectXLib\debug\dx.lib gl.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib rar_release.lib ifc22.lib ..\client\jpeglib\jpeg.lib xerces-c_2D.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /nodefaultlib:"libc.lib" /nodefaultlib:"libcmtd.lib" /pdbtype:sept /libpath:"..\lib\\" /libpath:"..\lib\ui" /libpath:"h:\study\lib" /libpath:"..\xerces\lib"
# SUBTRACT LINK32 /profile /map

!ENDIF 

# Begin Target

# Name "VS_UI - Win32 Release"
# Name "VS_UI - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\AcceleratorDef.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AcceleratorManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\Client_PCH.cpp
# ADD CPP /Yc"client_PCH.h"
# End Source File
# Begin Source File

SOURCE=.\src\KeyAccelerator.cpp
# End Source File
# Begin Source File

SOURCE=..\client\LeakMemoryDumper.cpp
# End Source File
# Begin Source File

SOURCE=.\MitemTableInit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SkinManager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_bbs.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_DESC.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_Description.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_descriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_Dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_ELEVATOR.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_Exception.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_Exchange.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_ExtraDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_Game.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_GameCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vs_ui_gamecommon2.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_GameOusters.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_GameSlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_GameVampire.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_GlobalResource.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_item.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_Message.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_mouse_pointer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_PetStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_progress.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_Shop.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_skill_tree.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_SKILL_VIEW.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_Storage.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_title.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_TITLE_SHOWCHAR.CPP
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_Tutorial.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_ui_result_receiver.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_util.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_WebBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VS_UI_widget.cpp
# End Source File
# Begin Source File

SOURCE=.\WinMain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\header\AcceleratorDef.h
# End Source File
# Begin Source File

SOURCE=.\src\header\AcceleratorManager.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\CImm.h
# End Source File
# Begin Source File

SOURCE=..\client\Client_PCH.h
# End Source File
# Begin Source File

SOURCE=.\src\header\KeyAccelerator.h
# End Source File
# Begin Source File

SOURCE=..\client\LeakMemoryDumper.h
# End Source File
# Begin Source File

SOURCE=.\src\header\SkinManager.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_Base.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_bbs.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_DESC.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_Description.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_descriptor.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_Dialog.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_ELEVATOR.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_Exception.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_Exchange.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_ExtraDialog.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_filepath.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_Game.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_GameCommon.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_GameCommon2.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_GameOusters.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_GameSlayer.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_GameVampire.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_GlobalResource.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_Helper.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_item.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_Message.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_mouse_pointer.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_PetStorage.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_progress.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_Shop.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_skill_tree.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_SKILL_VIEW.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_Storage.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_title.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_Tutorial.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_ui_result_receiver.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_UIMessage.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_util.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_WebBrowser.h
# End Source File
# Begin Source File

SOURCE=.\src\header\VS_UI_widget.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Ex"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\client\ClientConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\client\ClientConfig.h
# End Source File
# Begin Source File

SOURCE=..\client\ClientFunction.cpp
# End Source File
# Begin Source File

SOURCE=..\client\ClientFunction.h
# End Source File
# Begin Source File

SOURCE=..\client\CMessageArray.cpp
# End Source File
# Begin Source File

SOURCE=..\client\CMessageArray.h
# End Source File
# Begin Source File

SOURCE=..\client\CSoundPartManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\CSoundPartManager.h
# End Source File
# Begin Source File

SOURCE=..\client\CTypeTable.h
# End Source File
# Begin Source File

SOURCE=..\client\DebugInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\client\DebugInfo.h
# End Source File
# Begin Source File

SOURCE=..\client\ExperienceTable.cpp
# End Source File
# Begin Source File

SOURCE=..\client\ExperienceTable.h
# End Source File
# Begin Source File

SOURCE=..\client\ExpInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\client\ExpInfo.h
# End Source File
# Begin Source File

SOURCE=..\client\FameInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\client\FameInfo.h
# End Source File
# Begin Source File

SOURCE=..\client\MCreatureTable.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MCreatureTable.h
# End Source File
# Begin Source File

SOURCE=..\client\MEventManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MEventManager.h
# End Source File
# Begin Source File

SOURCE=..\client\MFunctionManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MFunctionManager.h
# End Source File
# Begin Source File

SOURCE=..\client\MGameStringTable.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MGameStringTable.h
# End Source File
# Begin Source File

SOURCE=..\client\MGridItemManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MGridItemManager.h
# End Source File
# Begin Source File

SOURCE=..\client\MGuildInfoMapper.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MGuildInfoMapper.h
# End Source File
# Begin Source File

SOURCE=..\client\MGuildMarkManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MGuildMarkManager.h
# End Source File
# Begin Source File

SOURCE=..\client\MHelpMessageManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MHelpMessageManager.h
# End Source File
# Begin Source File

SOURCE=..\client\MInventory.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MInventory.h
# End Source File
# Begin Source File

SOURCE=..\client\MItem.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MItem.h
# End Source File
# Begin Source File

SOURCE=..\client\MItemManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MItemManager.h
# End Source File
# Begin Source File

SOURCE=..\client\MItemOptionTable.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MItemOptionTable.h
# End Source File
# Begin Source File

SOURCE=..\client\MItemTable.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MItemTable.h
# End Source File
# Begin Source File

SOURCE=..\client\MMoneyManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MMoneyManager.h
# End Source File
# Begin Source File

SOURCE=..\client\MMonsterKillQuestInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MMonsterKillQuestInfo.h
# End Source File
# Begin Source File

SOURCE=..\client\MNPCTable.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MNPCTable.h
# End Source File
# Begin Source File

SOURCE=..\client\MObject.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MObject.h
# End Source File
# Begin Source File

SOURCE=..\client\MOustersGear.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MOustersGear.h
# End Source File
# Begin Source File

SOURCE=..\client\MParty.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MParty.h
# End Source File
# Begin Source File

SOURCE=..\client\MPlayerGear.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MPlayerGear.h
# End Source File
# Begin Source File

SOURCE=..\client\MPriceManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MPriceManager.h
# End Source File
# Begin Source File

SOURCE=..\client\MQuestInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MQuestInfo.h
# End Source File
# Begin Source File

SOURCE=..\client\MQuickSlot.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MQuickSlot.h
# End Source File
# Begin Source File

SOURCE=..\client\MShop.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MShop.h
# End Source File
# Begin Source File

SOURCE=..\client\MShopShelf.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MShopShelf.h
# End Source File
# Begin Source File

SOURCE=..\client\MShopTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MShopTemplateTable.h
# End Source File
# Begin Source File

SOURCE=..\client\MSkillInfoTable.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MSkillManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MSlayerGear.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MSlayerGear.h
# End Source File
# Begin Source File

SOURCE=..\client\MSlotItemManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MSlotItemManager.h
# End Source File
# Begin Source File

SOURCE=..\client\MSortedItemManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MSortedItemManager.h
# End Source File
# Begin Source File

SOURCE=..\client\MSoundTable.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MSoundTable.h
# End Source File
# Begin Source File

SOURCE=..\client\MStorage.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MStorage.h
# End Source File
# Begin Source File

SOURCE=..\client\MString.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MString.h
# End Source File
# Begin Source File

SOURCE=..\client\MStringArray.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MStringArray.h
# End Source File
# Begin Source File

SOURCE=..\client\MTimeItemManager.CPP
# End Source File
# Begin Source File

SOURCE=..\client\MTimeItemManager.h
# End Source File
# Begin Source File

SOURCE=..\client\MTradeManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MTradeManager.h
# End Source File
# Begin Source File

SOURCE=..\client\MVampireGear.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MVampireGear.h
# End Source File
# Begin Source File

SOURCE=..\client\MZoneTable.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MZoneTable.h
# End Source File
# Begin Source File

SOURCE=..\client\ProfileManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\ProfileManager.h
# End Source File
# Begin Source File

SOURCE=..\client\Profiler.cpp
# End Source File
# Begin Source File

SOURCE=..\client\Profiler.h
# End Source File
# Begin Source File

SOURCE=..\client\packet\Properties.cpp
# End Source File
# Begin Source File

SOURCE=..\client\packet\Properties.h
# End Source File
# Begin Source File

SOURCE=..\client\RankBonusDef.h
# End Source File
# Begin Source File

SOURCE=..\client\RankBonusTable.cpp
# End Source File
# Begin Source File

SOURCE=..\client\RankBonusTable.h
# End Source File
# Begin Source File

SOURCE=..\client\ShrineInfoManager.cpp
# End Source File
# Begin Source File

SOURCE=..\client\ShrineInfoManager.h
# End Source File
# Begin Source File

SOURCE=..\client\packet\StringStream.cpp
# End Source File
# Begin Source File

SOURCE=..\client\packet\StringStream.h
# End Source File
# Begin Source File

SOURCE=..\client\SXml\SXml.cpp
# End Source File
# Begin Source File

SOURCE=..\client\SXml\SXml.h
# End Source File
# Begin Source File

SOURCE=..\client\SystemAvailabilities.cpp
# End Source File
# Begin Source File

SOURCE=..\client\UserInformation.cpp
# End Source File
# Begin Source File

SOURCE=..\client\UserInformation.h
# End Source File
# Begin Source File

SOURCE=..\client\UserOption.cpp
# End Source File
# Begin Source File

SOURCE=..\client\UserOption.h
# End Source File
# Begin Source File

SOURCE=..\client\UtilityFunction.cpp
# End Source File
# Begin Source File

SOURCE=..\client\UtilityFunction.h
# End Source File
# End Group
# Begin Group "Widget"

# PROP Default_Filter "h;cpp"
# Begin Source File

SOURCE=.\src\widget\mother.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widget\mother.h
# End Source File
# Begin Source File

SOURCE=.\src\widget\PI.H
# End Source File
# Begin Source File

SOURCE=.\src\widget\pi_core.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widget\pi_core.h
# End Source File
# Begin Source File

SOURCE=.\src\widget\SimpleDataList.h
# End Source File
# Begin Source File

SOURCE=.\src\widget\u_button.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widget\u_button.h
# End Source File
# Begin Source File

SOURCE=.\src\widget\U_edit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widget\U_edit.h
# End Source File
# Begin Source File

SOURCE=.\src\widget\u_scrollbar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widget\u_scrollbar.h
# End Source File
# Begin Source File

SOURCE=.\src\widget\u_window.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widget\u_window.h
# End Source File
# Begin Source File

SOURCE=.\src\widget\unicorn.h
# End Source File
# End Group
# Begin Group "hangul"

# PROP Default_Filter "h;cpp"
# Begin Source File

SOURCE=.\src\hangul\Ci.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hangul\Ci.h
# End Source File
# Begin Source File

SOURCE=.\src\hangul\FL2.cpp
# End Source File
# Begin Source File

SOURCE=.\src\hangul\Fl2.h
# End Source File
# End Group
# Begin Group "CImm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\Imm\CImm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Imm\FeelitAPI.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\IFC.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\IFCErrors.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmBaseTypes.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmBox.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmCompoundEffect.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmCondition.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmConstant.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmDamper.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmDevice.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmDevices.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmDXDevice.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmEffect.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmEffectSuite.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmEllipse.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmEnclosure.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmFriction.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmGrid.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmIFR.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmInertia.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmMouse.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmPeriodic.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmProjects.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmRamp.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmSpring.h
# End Source File
# Begin Source File

SOURCE=.\src\Imm\ImmTexture.h
# End Source File
# End Group
# End Target
# End Project
