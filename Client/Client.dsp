# Microsoft Developer Studio Project File - Name="Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Client - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Client.mak" CFG="Client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Client - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Client - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Client - Win32 ReleaseLog" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Client", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Client - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GR /GX /Zi /O1 /Ob2 /I "\Game(800)\designed\project\client" /I "DXLib" /I "SpriteLib" /I "FrameLib" /I "D3DLib" /I "VolumeLib" /I "Packet" /I "CWinMain Library" /I "lib" /I "Updater" /I "MZLib" /I "sxml" /I "\Game(800)\designed\project\vs_ui\src\hangul" /I "\Game(800)\designed\project\vs_ui\src\header" /I "\Game(800)\designed\project\vs_ui\src\Imm" /I "\Game(800)\designed\project\vs_ui\src\widget" /I "..\xerces\include" /I "\Game(800)\designed\project\VS_UI" /I "\Game(800)\designed\project\basic" /D "OUTPUT_DEBUG" /D "__DEBUG_OUTPUT__" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__WINDOWS__" /D "__GAME_CLIENT__" /D "__WIN32__" /D "__NEW_SKILL__" /D "__TEST_SUB_INVENTORY__" /D "__FRIEND_SYSTEM_VIVA__" /FAcs /Fr /Yu"Client_PCH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 oleaut32.lib jpegLib\jpeg.lib Rar_release.lib DEUtil\release\DEUtil.lib WinLib\Release\WinLib.lib VolumeLib\Release\Volume.lib D3DLib\Release\DX3D.lib VS_UI_Release.lib DirectXLib\Release\DX.lib SpriteLib\Release\SP.lib framelib\Release\FR.lib IFC22.lib MZLib\Release\ZLib.lib imm32.lib GL.lib Timer.lib PS.lib BasicS.lib rbt.lib Libcmt.lib Libcimt.lib Libcpmt.lib Oldnames.lib DSetup.lib DDraw.lib DInput.lib DSound.lib DXGUID.lib WinMM.lib WSock32.lib user32.lib gdi32.lib advapi32.lib uuid.lib shell32.lib kernel32.lib Comctl32.lib ole32.lib netapi32.lib /nologo /subsystem:windows /map /machine:I386 /nodefaultlib /out:"\ttdk2\fengshen.exe" /libpath:"\Game(800)\designed\project\lib\ui" /libpath:"\Game(800)\designed\project\lib" /libpath:"..\xerces\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Client - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "\Game(800)\designed\project\client" /I "DXLib" /I "SpriteLib" /I "FrameLib" /I "D3DLib" /I "VolumeLib" /I "Packet" /I "CWinMain Library" /I "lib" /I "Updater" /I "MZLib" /I "sxml" /I "\Game(800)\designed\project\vs_ui\src\hangul" /I "\Game(800)\designed\project\vs_ui\src\header" /I "\Game(800)\designed\project\vs_ui\src\Imm" /I "\Game(800)\designed\project\vs_ui\src\widget" /I "..\xerces\include" /I "\Game(800)\designed\project\VS_UI" /I "\Game(800)\designed\project\basic" /D "__DEBUG_OUTPUT__" /D "OUTPUT_DEBUG" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__WINDOWS__" /D "__GAME_CLIENT__" /D "__WIN32__" /D "__TEST_SUB_INVENTORY__" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 npgamelib.lib oleaut32.lib jpegLib\jpeg.lib IFC22.lib Rar_Debug.lib DEUtil\debug\DEUtil.lib Winlib\Debug\WinLib.lib VolumeLib\Debug\Volume.lib D3DLib\Debug\DX3D.lib VS_UI_Debug.lib DirectXLib\Debug\DX.lib SpriteLib\Debug\SP.lib framelib\Debug\FR.lib MZLib\Debug\ZLib.lib Comctl32.lib Timer.lib imm32.lib WSock32.lib GL.lib PS.lib BasicS.lib rbt.lib Libcmtd.lib LibCimtd.lib Libcpmtd.lib Oldnames.lib DSetup.lib DDraw.lib DInput.lib DSound.lib DXGUID.lib WinMM.lib WSock32.lib kernel32.lib user32.lib gdi32.lib advapi32.lib ole32.lib uuid.lib shell32.lib netapi32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib /out:"\ttdk2\fs_Debug.exe" /pdbtype:sept /libpath:"\Game(800)\designed\project\lib\ui" /libpath:"\Game(800)\designed\project\lib" /libpath:"..\xerces\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Client - Win32 ReleaseLog"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Client___Win32_ReleaseLog"
# PROP BASE Intermediate_Dir "Client___Win32_ReleaseLog"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseLog"
# PROP Intermediate_Dir "ReleaseLog"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /O1 /Ob2 /I "\designed\project\client" /I "DXLib" /I "SpriteLib" /I "FrameLib" /I "D3DLib" /I "VolumeLib" /I "Packet" /I "WinLib" /I "lib" /I "Updater" /I "MZLib" /I "\designed\project\NEW - vs_ui\src\hangul" /I "\designed\project\NEW - vs_ui\src\header" /I "\designed\project\NEW - vs_ui\src\Imm" /I "\designed\project\NEW - vs_ui\src\widget" /I "\designed\lib\ui" /D "__DEBUG_OUTPUT__" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "__WINDOWS__" /D "__GAME_CLIENT__" /D "__WIN32__" /YX /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MT /W3 /GR /GX /ZI /I "\designed\project\client" /I "DXLib" /I "SpriteLib" /I "FrameLib" /I "D3DLib" /I "VolumeLib" /I "Packet" /I "WinLib" /I "lib" /I "Updater" /I "MZLib" /I "sxml" /I "\designed\project\NEW - vs_ui\src\hangul" /I "\designed\project\NEW - vs_ui\src\header" /I "\designed\project\NEW - vs_ui\src\Imm" /I "\designed\project\NEW - vs_ui\src\widget" /I "\designed\lib\ui" /I "..\xerces\include" /D "__DEBUG_OUTPUT__" /D "OUTPUT_DEBUG" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "__WINDOWS__" /D "__GAME_CLIENT__" /D "__WIN32__" /Yu"Client_PCH.h" /FD /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 NPGAMELIB.LIB jpegLib\jpeg_release.lib Rar_release.lib DEUtil\release\DEUtil.lib WinLib\Release\WinLib.lib VolumeLib\Release\Volume.lib D3DLib\Release\DX3D.lib VS_UI_Release.lib DXLib\Release\DX.lib SpriteLib\Release\SP.lib FrameLib\Release\FR.lib IFC22.lib MZLib\Release\ZLib.lib imm32.lib GL.lib Timer.lib PS.lib BasicS.lib rbt.lib Libcmt.lib Libcimt.lib Libcpmt.lib Oldnames.lib DSetup.lib DDraw.lib DInput.lib DSound.lib DXGUID.lib WinMM.lib WSock32.lib user32.lib gdi32.lib advapi32.lib uuid.lib shell32.lib kernel32.lib Comctl32.lib ole32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib /out:"\darkedentest\darkeden.exe" /libpath:"\designed\lib\ui" /libpath:"\designed\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 NPGameLib.lib jpegLib\jpeg_release.lib Rar_release.lib DEUtil\release\DEUtil.lib WinLib\Release\WinLib.lib VolumeLib\Release\Volume.lib D3DLib\Release\DX3D.lib VS_UI_Release.lib DXLib\Release\DX.lib SpriteLib\Release\SP.lib FrameLib\Release\FR.lib IFC22.lib MZLib\Release\ZLib.lib imm32.lib GL.lib Timer.lib PS.lib BasicS.lib rbt.lib Libcmt.lib Libcimt.lib Libcpmt.lib Oldnames.lib DSetup.lib DDraw.lib DInput.lib DSound.lib DXGUID.lib WinMM.lib WSock32.lib user32.lib gdi32.lib advapi32.lib uuid.lib shell32.lib kernel32.lib Comctl32.lib ole32.lib xerces-c_2.lib netapi32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib /out:"\darkedentest\darkedenlog.exe" /libpath:"\designed\lib\ui" /libpath:"\designed\lib" /libpath:"..\xerces\lib"
# SUBTRACT LINK32 /pdb:none /incremental:yes /map /debug

!ENDIF 

# Begin Target

# Name "Client - Win32 Release"
# Name "Client - Win32 Debug"
# Name "Client - Win32 ReleaseLog"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "GameObject Source"

# PROP Default_Filter ""
# Begin Group "EffectGenerator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MAroundZoneEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MAttachCreatureEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MAttachCreatureEffectGroundGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MAttachCreatureOrbitEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MAttachOrbitEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\MAttachZoneAroundEffectGenerator.CPP
# End Source File
# Begin Source File

SOURCE=.\MAttachZoneEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MAttachZoneSelectableEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MAttackCreatureEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MAttackCreatureHomingEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MAttackCreatureParabolaEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MAttackZoneBombEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MAttackZoneEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MAttackZoneParabolaEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MAttackZoneRectEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MBloodyBreakerEffectGenerator.CPP
# End Source File
# Begin Source File

SOURCE=.\MBloodyWallEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MBloodyWaveEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MEffectGeneratorTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MEffectTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\MFallingEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MFirePiecingEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MFollowPathEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MMeteorDropEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MMultipleFallingEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MPlayingWithFireEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MRippleZoneEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MRippleZonePixelEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MRippleZoneWideEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MRisingEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MSkipEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MSpreadOutEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopInventoryEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopZoneCrossEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopZoneEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopZoneEmptyCrossEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopZoneEmptyHorizontalEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopZoneEmptyRectEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopZoneEmptyVerticalWallEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopZoneMultipleEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopZoneRandomEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopZoneRectEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopZoneRhombusEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopZoneSelectableEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopZoneWallEffectGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MStopZoneXEffectGenerator.cpp
# End Source File
# End Group
# Begin Group "Object class source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MAnimationObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MCreature.cpp
# End Source File
# Begin Source File

SOURCE=.\MCreatureWear.cpp
# End Source File
# Begin Source File

SOURCE=.\MFakeCreature.cpp
# End Source File
# Begin Source File

SOURCE=.\MImageObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteractionObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MItem.cpp
# End Source File
# Begin Source File

SOURCE=.\MNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\MObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MPortal.cpp
# End Source File
# Begin Source File

SOURCE=.\MShadowAnimationObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MShadowObject.cpp
# End Source File
# End Group
# Begin Group "Effect class source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MAttachEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\MChaseEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\MEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\MEffectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MGuidanceEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\MHomingEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\MLinearEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\MMovingEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\MParabolaEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\MScreenEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\MScreenEffectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MSkipEffect.cpp
# End Source File
# End Group
# Begin Group "Information class No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BloodBibleSignInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CUsingColorSetTable.cpp
# End Source File
# Begin Source File

SOURCE=.\ExperienceTable.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MActionInfoTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MActionResult.cpp
# End Source File
# Begin Source File

SOURCE=.\MCreatureSpriteTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MCreatureTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MEffectSpriteTypeTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MEffectStatusTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MGameStringTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MInteractionObjectTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MItemOptionTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MItemTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MLevelNameTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MMusicTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MNPCScriptTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MNPCTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterNameTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MSkillInfoTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MSkillManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MSoundTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MZoneTable.cpp
# End Source File
# Begin Source File

SOURCE=.\NicknameInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PCConfigTable.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\QuestStatusInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\RankBonusTable.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerInformation.cpp
# End Source File
# Begin Source File

SOURCE=.\TempInformation.cpp
# End Source File
# Begin Source File

SOURCE=.\UserInformation.cpp
# End Source File
# Begin Source File

SOURCE=.\UserOption.cpp
# End Source File
# End Group
# Begin Group "ItemManager source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MGridItemManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\MItemManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MOustersGear.cpp
# End Source File
# Begin Source File

SOURCE=.\MPlayerGear.cpp
# End Source File
# Begin Source File

SOURCE=.\MQuickSlot.cpp
# End Source File
# Begin Source File

SOURCE=.\MSlayerGear.cpp
# End Source File
# Begin Source File

SOURCE=.\MSlotItemManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MSortedItemManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MVampireGear.cpp
# End Source File
# End Group
# Begin Group "Shop Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MMoneyManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MPriceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MShop.cpp
# End Source File
# Begin Source File

SOURCE=.\MShopShelf.cpp
# End Source File
# Begin Source File

SOURCE=.\MShopTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\MStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\MTradeManager.cpp
# End Source File
# End Group
# Begin Group "World source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MGameTime.cpp
# End Source File
# Begin Source File

SOURCE=.\MHelicopterManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MSector.cpp
# End Source File
# Begin Source File

SOURCE=.\MWeather.cpp
# End Source File
# Begin Source File

SOURCE=.\MZoneSound.cpp
# End Source File
# Begin Source File

SOURCE=.\MZoneSoundManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SectorSoundInfo.cpp
# End Source File
# End Group
# Begin Group "Help source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MCompareManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MHelpDisplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\MHelpManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MHelpMessageManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MHelpStringTable.cpp
# End Source File
# End Group
# Begin Group "Party&Guild"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GuildInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildMemberInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MGuildInfoMapper.cpp
# End Source File
# Begin Source File

SOURCE=.\MGuildMarkManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MParty.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\MChatManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MJusticeAttackManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MObjectSelector.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyStatusManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MRequestMode.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundNode.cpp
# End Source File
# Begin Source File

SOURCE=.\TalkBox.cpp
# End Source File
# Begin Source File

SOURCE=.\UIDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\UIMessageManager.cpp
# End Source File
# End Group
# Begin Group "Utility source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AppendPatchInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CAvi.cpp
# End Source File
# Begin Source File

SOURCE=.\CMessageArray.cpp
# End Source File
# Begin Source File

SOURCE=.\CrashReport.cpp
# End Source File
# Begin Source File

SOURCE=.\CShadowPartManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CSoundPartManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpriteTexturePartManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CTexturePartManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CToken.cpp
# End Source File
# Begin Source File

SOURCE=.\FameInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\GetWinVer.cpp
# End Source File
# Begin Source File

SOURCE=.\MathTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MCrashReportManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoryPool.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoryPool.h
# End Source File
# Begin Source File

SOURCE=.\MEventManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MFunctionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MLoadingSPKWorkNode.cpp
# End Source File
# Begin Source File

SOURCE=.\MMonsterKillQuestInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MMusic.cpp
# End Source File
# Begin Source File

SOURCE=.\MQuestInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MString.cpp
# End Source File
# Begin Source File

SOURCE=.\MStringArray.cpp
# End Source File
# Begin Source File

SOURCE=.\MStringList.cpp
# End Source File
# Begin Source File

SOURCE=.\MStringMap.cpp
# End Source File
# Begin Source File

SOURCE=.\MTimeItemManager.CPP
# End Source File
# Begin Source File

SOURCE=.\MWarManager.CPP
# End Source File
# Begin Source File

SOURCE=.\MWorkNode.cpp
# End Source File
# Begin Source File

SOURCE=.\MWorkThread.cpp
# End Source File
# Begin Source File

SOURCE=.\Netmarble.cpp
# End Source File
# Begin Source File

SOURCE=.\NPGameLib.h
# End Source File
# Begin Source File

SOURCE=.\nProtect.cpp
# End Source File
# Begin Source File

SOURCE=.\PackFileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Profiler.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowTimeChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemAvailabilities.cpp
# End Source File
# Begin Source File

SOURCE=.\UtilityFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\WavePackFileManager.cpp
# End Source File
# End Group
# Begin Group "GameLoop source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\APICheck.cpp
# End Source File
# Begin Source File

SOURCE=.\CGameUpdate.cpp
# End Source File
# Begin Source File

SOURCE=.\Client.cpp
# End Source File
# Begin Source File

SOURCE=.\COpeningUpdate.cpp
# End Source File
# Begin Source File

SOURCE=.\CWaitPacketUpdate.cpp
# End Source File
# Begin Source File

SOURCE=.\CWaitUIUpdate.cpp
# End Source File
# Begin Source File

SOURCE=.\GameInit.cpp
# End Source File
# Begin Source File

SOURCE=.\GameInitInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\GameMain.cpp
# End Source File
# Begin Source File

SOURCE=.\GameUI.cpp
# End Source File
# End Group
# Begin Group "Player/Zone/View source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DrawCreatureDivineGuidance.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawCreatureEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawCreatureShadow.cpp
# End Source File
# Begin Source File

SOURCE=.\MPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\MStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\MTopView.cpp
# End Source File
# Begin Source File

SOURCE=.\MTopViewDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\MTopViewDraw.inl
# End Source File
# Begin Source File

SOURCE=.\MTopviewExceptionProc.cpp
# End Source File
# Begin Source File

SOURCE=.\MZone.cpp
# End Source File
# Begin Source File

SOURCE=.\ZoneFileHeader.cpp
# End Source File
# End Group
# Begin Group "Packet source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Packet\Assert.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\BloodBibleBonusInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\packet\cpackets\CGAbsorbSoul.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAcceptUnion.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAcceptUnionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddGearToMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddInventoryToMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddItemToCodeSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddItemToCodeSheetHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddItemToItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddMouseToGear.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddMouseToInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddMouseToQuickSlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddMouseToZone.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddQuickSlotToMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddSMSAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddZoneToInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddZoneToMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAppointSubmaster.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAppointSubmasterHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAuthKey.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGBloodDrain.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGBuyStoreItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGBuyStoreItemHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGCastingSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\packet\cpackets\CGCrashReport.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDeleteSMSAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDenyUnion.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDenyUnionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDepositPet.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDepositPetHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDialUp.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDisplayItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDisplayItemHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDissectionCorpse.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDonationMoney.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDonationMoneyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDownSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDownSkillHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDropMoney.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGExpelGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGExpelGuildHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGFailQuest.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGFailQuestHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGGetEventItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGGetOffMotorCycle.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGGlobalChat.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGGQuestAccept.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGGQuestCancel.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGGuildChat.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGJoinGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGLearnSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGLogout.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGLotterySelect.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGLotterySelectHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGMakeItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGMixItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGMixItemHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGModifyGuildIntro.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGModifyGuildMember.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGModifyGuildMemberIntro.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGModifyNickname.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGModifyTaxRatio.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGMouseToStash.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGMove.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGNPCAskAnswer.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGNPCTalk.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPartyInvite.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPartyLeave.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPartyPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPartySay.cpp
# End Source File
# Begin Source File

SOURCE=.\packet\cpackets\CGPetGamble.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPhoneDisconnect.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPhoneSay.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPickupMoney.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPortCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGQuitUnion.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGQuitUnionAccept.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGQuitUnionAcceptHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGQuitUnionDeny.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGQuitUnionDenyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGQuitUnionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRangerSay.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRangerSayHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGReady.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRegistGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRelicToObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGReloadFromInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGReloadFromQuickSlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestGuildList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestGuildListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestGuildMemberList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestIP.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestNewbieItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestPowerPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestRepair.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestStoreInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestStoreInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestUnion.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestUnionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestUnionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestUnionInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGResurrect.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRideMotorCycle.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSay.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectBloodBible.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectBloodBibleHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectGuildMember.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectNickname.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectPortal.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectQuest.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectQuestHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectRankBonus.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectRegenZone.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectRegenZoneHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectTileEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectWayPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSetSlayerHotKey.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSetVampireHotKey.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGShopRequestBuy.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGShopRequestList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGShopRequestSell.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSilverCoating.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSkillToInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSkillToNamed.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSkillToObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSkillToSelf.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSkillToTile.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSMSAddressList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSMSSend.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStashDeposit.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStashList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStashRequestBuy.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStashToMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStashWithdraw.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStoreClose.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStoreCloseHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStoreOpen.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStoreOpenHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStoreSign.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStoreSignHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSubmitScore.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTakeOutGood.cpp
# End Source File
# Begin Source File

SOURCE=.\packet\cpackets\CGTameMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGThrowBomb.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGThrowItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTradeAddItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTradeFinish.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTradeMoney.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTradePrepare.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTradeRemoveItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTryJoinGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTypeStringList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUnburrow.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUndisplayItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUndisplayItemHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUntransform.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUseBonusPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUseItemFromGear.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUseItemFromGQuestInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUseItemFromGQuestInventoryHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUseItemFromInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUseMessageItemFromInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUsePotionFromInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUsePotionFromQuickSlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUsePowerPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGVerifyTime.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGVisible.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGWhisper.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGWithdrawPet.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGWithdrawPetHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGWithdrawTax.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGWithdrawTaxHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLChangeServer.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLCreatePC.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLDeletePC.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLGetPCList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLGetServerList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLGetWorldList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\ClientPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLLogout.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLQueryCharacterName.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLQueryPlayerID.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLReconnectLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLRegisterPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLSelectPC.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLSelectServer.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLSelectWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLVersionCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRConnectHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRDisconnect.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRDisconnectHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRRequest.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRRequest2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRRequest2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRRequestHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRWhisper.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRWhisperHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Upackets\CURequestLoginMode.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Upackets\CURequestLoginModeHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Datagram.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\DatagramSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\EffectInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Encrypter.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\ExtraInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\ExtraSlotInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\FileAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\GameTime.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCActiveGuildList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCActiveGuildListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddBat.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddBatHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddBurrowingCreature.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddBurrowingCreatureHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddEffectHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddEffectToTile.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddEffectToTileHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddGearToInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddGearToInventoryHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddGearToZone.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddGearToZoneHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddHelicopter.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddHelicopterHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddInjuriousCreature.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddInjuriousCreatureHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddInstalledMineToZone.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddInstalledMineToZoneHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddItemToInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddItemToItemVerify.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddItemToItemVerifyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddItemToZone.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddMonsterCorpse.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddMonsterCorpseHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddMonsterFromBurrowing.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddMonsterFromBurrowingHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddMonsterFromTransformation.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddMonsterFromTransformationHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddMonsterHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddNewItemToZone.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddNewItemToZoneHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddNickname.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddNicknameHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddNPCHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddOusters.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddOustersCorpse.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddOustersCorpseHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddOustersHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddressListVerify.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddressListVerifyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddSlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddSlayerCorpse.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddSlayerCorpseHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddSlayerHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddStoreItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddStoreItemHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampire.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampireCorpse.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampireCorpseHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampireFromBurrowing.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampireFromBurrowingHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampireFromTransformation.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampireFromTransformationHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampireHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampirePortal.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampirePortalHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddWolf.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddWolfHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK3.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK3Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK4.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK4Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK5.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK5Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackMeleeOK1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackMeleeOK1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackMeleeOK2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackMeleeOK2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackMeleeOK3.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackMeleeOK3Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAuthKey.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAuthKeyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodBibleList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodBibleListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodBibleSignInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodBibleSignInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodBibleStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodBibleStatusHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodDrainOK1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodDrainOK1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodDrainOK2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodDrainOK2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodDrainOK3.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodDrainOK3Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCannotAdd.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCannotAddHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCannotUse.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCannotUseHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCastingSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCastingSkillHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCChangeDarkLight.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCChangeDarkLightHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCChangeInventoryItemNum.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCChangeShape.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCChangeShapeHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCChangeWeather.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCChangeWeatherHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCreateItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCreateItemHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCreatureDied.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCreatureDiedHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCrossCounterOK1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCrossCounterOK1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCrossCounterOK2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCrossCounterOK2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCrossCounterOK3.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCrossCounterOK3Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDeleteandPickUpOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDeleteandPickUpOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDeleteEffectFromTile.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDeleteEffectFromTileHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDeleteInventoryItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDeleteInventoryItemHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDeleteObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDeleteObjectHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDisconnect.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDisconnectHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDownSkillFailed.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDownSkillFailedHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDownSkillOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDownSkillOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDropItemToZone.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDropItemToZoneHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCEnterVampirePortal.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCEnterVampirePortalHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCExecuteElement.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCExecuteElementHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCFakeMove.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCFakeMoveHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCFastMove.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCFastMoveHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCFlagWarStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCFlagWarStatusHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCFriendChatting.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCFriendChattingHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGetDamage.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGetDamageHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGetOffMotorCycle.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGetOffMotorCycleFailed.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGetOffMotorCycleFailedHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGetOffMotorCycleHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGetOffMotorCycleOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGetOffMotorCycleOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGlobalChat.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGlobalChatHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGoodsList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGoodsListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGQuestInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGQuestInventoryHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGQuestStatusInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGQuestStatusInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGQuestStatusModify.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGQuestStatusModifyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGuildChat.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGuildChatHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGuildMemberList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGuildMemberListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGuildResponse.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGuildResponseHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHolyLandBonusInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHolyLandBonusInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHPRecoveryEndToOthers.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHPRecoveryEndToOthersHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHPRecoveryEndToSelf.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHPRecoveryEndToSelfHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHPRecoveryStartToOthers.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHPRecoveryStartToOthersHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHPRecoveryStartToSelf.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHPRecoveryStartToSelfHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKickMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKickMessageHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnockBack.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnockBackHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnocksTargetBackOK1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnocksTargetBackOK1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnocksTargetBackOK2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnocksTargetBackOK2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnocksTargetBackOK4.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnocksTargetBackOK4Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnocksTargetBackOK5.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnocksTargetBackOK5Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCLearnSkillFailed.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCLearnSkillFailedHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCLearnSkillOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCLearnSkillOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCLearnSkillReady.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCLearnSkillReadyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCLightning.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCLightningHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMakeItemFail.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMakeItemFailHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMakeItemOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMakeItemOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMineExplosionOK1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMineExplosionOK1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMineExplosionOK2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMineExplosionOK2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMiniGameScores.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMiniGameScoresHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCModifyGuildMemberInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCModifyGuildMemberInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCModifyInformation.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCModifyInformationHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCModifyNickname.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCModifyNicknameHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMonsterKillQuestInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMonsterKillQuestInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMorph1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMorph1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMorphSlayer2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMorphSlayer2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMorphVampire2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMorphVampire2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMove.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMoveError.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMoveErrorHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMoveHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMoveOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMoveOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMPRecoveryEnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMPRecoveryEndHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMPRecoveryStart.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMPRecoveryStartHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMyStoreInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMyStoreInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNicknameList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNicknameListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNicknameVerify.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNicknameVerifyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNoticeEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNoticeEventHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\packet\gpackets\GCNotifyWin.cpp
# End Source File
# Begin Source File

SOURCE=.\packet\gpackets\GCNotifyWinHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCAsk.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCAskDynamic.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCAskDynamicHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCAskHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCAskVariable.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCAskVariableHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCResponse.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCResponseHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCSay.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCSayDynamic.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCSayDynamicHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCSayHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCOtherGuildName.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCOtherGuildNameHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCOtherModifyInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCOtherStoreInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCOtherStoreInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyError.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyErrorHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyInvite.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyInviteHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyJoined.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyJoinedHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyLeave.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyLeaveHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyPositionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartySay.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartySayHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\packet\gpackets\GCPetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\packet\gpackets\GCPetInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPetStashList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPetStashListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPetStashVerify.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPetStashVerifyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\packet\gpackets\GCPetUseSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\packet\gpackets\GCPetUseSkillHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPhoneConnected.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPhoneConnectedHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPhoneConnectionFailed.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPhoneConnectionFailedHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPhoneDisconnected.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPhoneDisconnectedHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPhoneSay.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPhoneSayHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCQuestStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCQuestStatusHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRankBonusInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRankBonusInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRealWearingInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRealWearingInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCReconnect.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCReconnectHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCReconnectLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCReconnectLoginHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRegenZoneStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRegenZoneStatusHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCReloadOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCReloadOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveCorpseHead.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveCorpseHeadHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveEffectHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveFromGear.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveFromGearHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveInjuriousCreature.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveInjuriousCreatureHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveStoreItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveStoreItemHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRequestedIP.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRequestedIPHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRequestFailed.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRequestFailedHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRequestPowerPointResult.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRequestPowerPointResultHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRideMotorCycle.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRideMotorCycleFailed.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRideMotorCycleFailedHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRideMotorCycleHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRideMotorCycleOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRideMotorCycleOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRing.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRingHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSay.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSayHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSearchMotorcycleFail.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSearchMotorcycleFailHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSearchMotorcycleOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSearchMotorcycleOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSelectQuestID.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSelectQuestIDHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSelectRankBonusFailed.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSelectRankBonusFailedHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSelectRankBonusOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSelectRankBonusOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSetPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSetPositionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopBought.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopBoughtHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopBuyFail.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopBuyFailHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopBuyOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopBuyOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopListMysterious.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopListMysteriousHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopMarketCondition.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopMarketConditionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopSellFail.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopSellFailHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopSellOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopSellOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopSold.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopSoldHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopVersionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowGuildInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowGuildInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowGuildJoin.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowGuildJoinHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowGuildMemberInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowGuildMemberInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowMessageBoxHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowUnionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowUnionInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowWaitGuildInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowWaitGuildInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillFailed1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillFailed1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillFailed2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillFailed2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToInventoryOK1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToInventoryOK1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToInventoryOK2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToInventoryOK2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK3.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK3Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK4.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK4Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK5.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK5Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK6.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK6Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToSelfOK1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToSelfOK1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToSelfOK2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToSelfOK2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToSelfOK3.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToSelfOK3Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK3.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK3Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK4.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK4Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK5.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK5Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK6.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK6Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSMSAddressList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSMSAddressListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCStashList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCStashListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCStashSell.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCStashSellHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCStatusCurrentHP.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCStatusCurrentHPHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSubInventoryInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSubInventoryInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSweeperBonusInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSweeperBonusInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSystemAvailabilities.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSystemAvailabilitiesHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSystemMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSystemMessageHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTakeOff.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTakeOffHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTakeOutFail.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTakeOutFailHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTakeOutOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTakeOutOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTeachSkillInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTeachSkillInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowBombOK1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowBombOK1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowBombOK2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowBombOK2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowBombOK3.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowBombOK3Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowItemOK1.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowItemOK1Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowItemOK2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowItemOK2Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowItemOK3.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowItemOK3Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTimeLimitItemInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTimeLimitItemInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeAddItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeAddItemHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeError.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeErrorHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeFinish.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeFinishHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeMoney.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeMoneyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradePrepare.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradePrepareHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeRemoveItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeRemoveItemHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeVerify.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeVerifyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUnburrowFail.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUnburrowFailHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUnburrowOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUnburrowOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUnionOfferList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUnionOfferListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUntransformFail.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUntransformFailHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUntransformOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUntransformOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUpdateInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUpdateInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUseBonusPointFail.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUseBonusPointFailHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUseBonusPointOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUseBonusPointOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUseOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUseOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUsePowerPointResult.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUsePowerPointResultHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCVisibleFail.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCVisibleFailHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCVisibleOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCVisibleOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWaitGuildList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWaitGuildListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWarList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWarListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWarScheduleList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWarScheduleListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWhisper.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWhisperFailed.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWhisperFailedHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWhisperHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\GearInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\GearSlotInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GLIncomingConnectionError.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GLIncomingConnectionErrorHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GLIncomingConnectionOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GLIncomingConnectionOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\GuildWarInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\InventoryInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\InventorySlotInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCCreatePCError.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCCreatePCErrorHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCCreatePCOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCCreatePCOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCDeletePCError.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCDeletePCErrorHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCDeletePCOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCDeletePCOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCLoginError.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCLoginErrorHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCLoginOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCLoginOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCPCList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCPCListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCQueryResultCharacterName.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCQueryResultCharacterNameHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCQueryResultPlayerID.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCQueryResultPlayerIDHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCReconnect.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCReconnectHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCRegisterPlayerError.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCRegisterPlayerErrorHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCRegisterPlayerOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCRegisterPlayerOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCSelectPCError.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCSelectPCErrorHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCServerList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCServerListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCVersionCheckError.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCVersionCheckErrorHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCVersionCheckOK.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCVersionCheckOKHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCWorldList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCWorldListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\packet\LevelWarInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LGIncomingConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LGIncomingConnectionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\ModifyInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\NPCInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\OustersSkillInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\PacketFactoryManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\PacketIDSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\PacketValidator.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\PCItemInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\PCOustersInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\PCOustersInfo2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\PCOustersInfo3.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\PCSlayerInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\PCSlayerInfo2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\PCSlayerInfo3.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\PCVampireInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\PCVampireInfo2.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\PCVampireInfo3.cpp
# End Source File
# Begin Source File

SOURCE=.\packet\PetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Properties.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\RaceWarInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCCharacterInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCCharacterInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCConnectVerify.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCConnectVerifyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCPositionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCPositionInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCPositionInfoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCRequestedFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCRequestedFileHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCRequestVerify.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCRequestVerifyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCSay.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCSayHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCStatusHP.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCStatusHPHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\RideMotorcycleInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\RideMotorcycleSlotInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\ScriptParameter.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\ServerGroupInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\ServerSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SlayerSkillInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Socket.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketEncryptInputStream.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketEncryptOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketInputStream.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\StringStream.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SubItemInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SubOustersSkillInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SubServerInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SubSlayerSkillInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SubVampireSkillInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\SweeperBonusInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\TextInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Upackets\UCRequestLoginMode.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Upackets\UCRequestLoginModeHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\VampireSkillInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\WarInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\WorldInfo.cpp
# End Source File
# End Group
# Begin Group "Patch source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Updater\Update.cpp
# End Source File
# Begin Source File

SOURCE=.\Updater\UpdateManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Updater\UpdateUtility.cpp
# End Source File
# End Group
# Begin Group "ZLib source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MZLib\MZLib.cpp
# End Source File
# End Group
# Begin Group "RequestPlayer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Packet\ClientCommunicationManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\RequestClientPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\RequestClientPlayerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\RequestFileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\RequestFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\RequestServerPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\RequestServerPlayerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\RequestUserManager.cpp
# End Source File
# Begin Source File

SOURCE=.\WhisperManager.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\spritelib\CAlphaSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\SpriteLib\CFileIndexTable.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGConnectSetKey.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGConnectSetKeyHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\VS_UI\lib\Client_PCH.cpp
# ADD CPP /Yc"Client_PCH.h"
# End Source File
# Begin Source File

SOURCE=.\ClientConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\CMP3.cpp
# End Source File
# Begin Source File

SOURCE=.\CNormalSpriteTexturePartManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CSystemInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\vs_ui\DebugInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugKit.cpp
# End Source File
# Begin Source File

SOURCE=.\EXECryptor.cpp

!IF  "$(CFG)" == "Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "Client - Win32 ReleaseLog"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GAME1024.cpp
# End Source File
# Begin Source File

SOURCE=.\md5.cpp
# End Source File
# Begin Source File

SOURCE=.\MTestDef.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherClass\RequestClientPacketFactoryManager.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherClass\RequestServerPacketFactoryManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Resource.rc
# End Source File
# Begin Source File

SOURCE=.\ServerInfoFileParser.cpp
# End Source File
# Begin Source File

SOURCE=.\ShrineInfoManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillDef.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Group "GameObject Header"

# PROP Default_Filter ""
# Begin Group "EffectGenerator No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MAroundZoneEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MAttachCreatureEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MAttachCreatureEffectGroundGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MAttachCreatureOrbitEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MAttachEffect.h
# End Source File
# Begin Source File

SOURCE=.\MAttachZoneAroundEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MAttachZoneEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MAttachZoneSelectableEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MAttackCreatureEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MAttackCreatureHomingEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MAttackCreatureParabolaEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MAttackZoneBombEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MAttackZoneEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MAttackZoneParabolaEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MAttackZoneRectEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MBloodyBreakerEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MBloodyWallEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MBloodyWaveEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MEffectGeneratorTable.h
# End Source File
# Begin Source File

SOURCE=.\MEffectTarget.h
# End Source File
# Begin Source File

SOURCE=.\MFallingEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MFirePiecingEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MFollowPathEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MMeteorDropEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MMultipleFallingEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MPlayingWithFireEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MRippleZoneEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MRippleZonePixelEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MRippleZoneWideEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MRisingEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MSkipEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MSpreadOutEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopInventoryEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopZoneCrossEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopZoneEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopZoneEmptyCrossEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopZoneEmptyHorizontalWallEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopZoneEmptyRectEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopZoneEmptyVerticalEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopZoneMultipleEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopZoneRandomEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopZoneRectEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopZoneRhombusEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopZoneSelectableEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopZoneWallEffectGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MStopZoneXEffectGenerator.h
# End Source File
# End Group
# Begin Group "Effect class"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MActionResult.h
# End Source File
# Begin Source File

SOURCE=.\MChaseEffect.h
# End Source File
# Begin Source File

SOURCE=.\MEffect.h
# End Source File
# Begin Source File

SOURCE=.\MEffectManager.h
# End Source File
# Begin Source File

SOURCE=.\MGuidanceEffect.h
# End Source File
# Begin Source File

SOURCE=.\MHomingEffect.h
# End Source File
# Begin Source File

SOURCE=.\MLinearEffect.h
# End Source File
# Begin Source File

SOURCE=.\MMovingEffect.h
# End Source File
# Begin Source File

SOURCE=.\MParabolaEffect.h
# End Source File
# Begin Source File

SOURCE=.\MScreenEffect.h
# End Source File
# Begin Source File

SOURCE=.\MScreenEffectManager.h
# End Source File
# Begin Source File

SOURCE=.\MSkipEffect.h
# End Source File
# End Group
# Begin Group "Object class"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameObject.h
# End Source File
# Begin Source File

SOURCE=.\MAnimationObject.h
# End Source File
# Begin Source File

SOURCE=.\MAttachOrbitEffect.h
# End Source File
# Begin Source File

SOURCE=.\MCreature.h
# End Source File
# Begin Source File

SOURCE=.\MCreatureWear.h
# End Source File
# Begin Source File

SOURCE=.\MFakeCreature.h
# End Source File
# Begin Source File

SOURCE=.\MImageObject.h
# End Source File
# Begin Source File

SOURCE=.\MInteractionObject.h
# End Source File
# Begin Source File

SOURCE=.\MInteractionObjectTable.h
# End Source File
# Begin Source File

SOURCE=.\MItem.h
# End Source File
# Begin Source File

SOURCE=.\MNPC.h
# End Source File
# Begin Source File

SOURCE=.\MObject.h
# End Source File
# Begin Source File

SOURCE=.\MPortal.h
# End Source File
# Begin Source File

SOURCE=.\MShadowAnimationObject.h
# End Source File
# Begin Source File

SOURCE=.\MShadowObject.h
# End Source File
# End Group
# Begin Group "Information class"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BloodBibleSignInfo.h
# End Source File
# Begin Source File

SOURCE=.\CTypeTable.h
# End Source File
# Begin Source File

SOURCE=.\CUsingColorSetTable.h
# End Source File
# Begin Source File

SOURCE=.\ExperienceTable.h
# End Source File
# Begin Source File

SOURCE=.\ExpInfo.h
# End Source File
# Begin Source File

SOURCE=.\MActionInfoTable.h
# End Source File
# Begin Source File

SOURCE=.\MCreatureSpriteTable.h
# End Source File
# Begin Source File

SOURCE=.\MCreatureTable.h
# End Source File
# Begin Source File

SOURCE=.\MEffectSpriteTypeTable.h
# End Source File
# Begin Source File

SOURCE=.\MEffectStatusTable.h
# End Source File
# Begin Source File

SOURCE=.\MGameStringTable.h
# End Source File
# Begin Source File

SOURCE=.\MItemOptionTable.h
# End Source File
# Begin Source File

SOURCE=.\MItemTable.h
# End Source File
# Begin Source File

SOURCE=.\MLevelNameTable.h
# End Source File
# Begin Source File

SOURCE=.\MMusicTable.h
# End Source File
# Begin Source File

SOURCE=.\MNPCScriptTable.h
# End Source File
# Begin Source File

SOURCE=.\MNPCTable.h
# End Source File
# Begin Source File

SOURCE=.\MonsterNameTable.h
# End Source File
# Begin Source File

SOURCE=.\MSkillManager.h
# End Source File
# Begin Source File

SOURCE=.\MSoundTable.h
# End Source File
# Begin Source File

SOURCE=.\MZoneTable.h
# End Source File
# Begin Source File

SOURCE=.\NicknameInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\QuestStatusInfo.h
# End Source File
# Begin Source File

SOURCE=.\RankBonusTable.h
# End Source File
# Begin Source File

SOURCE=.\ServerInformation.h
# End Source File
# Begin Source File

SOURCE=.\TempInformation.h
# End Source File
# Begin Source File

SOURCE=.\UserInformation.h
# End Source File
# Begin Source File

SOURCE=.\UserOption.h
# End Source File
# End Group
# Begin Group "ItemManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MGridItemManager.h
# End Source File
# Begin Source File

SOURCE=.\MInventory.h
# End Source File
# Begin Source File

SOURCE=.\MItemFinder.h
# End Source File
# Begin Source File

SOURCE=.\MItemManager.h
# End Source File
# Begin Source File

SOURCE=.\MOustersGear.h
# End Source File
# Begin Source File

SOURCE=.\MPlayerGear.h
# End Source File
# Begin Source File

SOURCE=.\MQuickSlot.h
# End Source File
# Begin Source File

SOURCE=.\MSlayerGear.h
# End Source File
# Begin Source File

SOURCE=.\MSlotItemManager.h
# End Source File
# Begin Source File

SOURCE=.\MSortedItemManager.h
# End Source File
# Begin Source File

SOURCE=.\MVampireGear.h
# End Source File
# End Group
# Begin Group "Shop"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MMoneyManager.h
# End Source File
# Begin Source File

SOURCE=.\MPriceManager.h
# End Source File
# Begin Source File

SOURCE=.\MShop.h
# End Source File
# Begin Source File

SOURCE=.\MShopShelf.h
# End Source File
# Begin Source File

SOURCE=.\MShopTemplateTable.h
# End Source File
# Begin Source File

SOURCE=.\MStorage.h
# End Source File
# Begin Source File

SOURCE=.\MTradeManager.h
# End Source File
# End Group
# Begin Group "Help header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MCompareManager.h
# End Source File
# Begin Source File

SOURCE=.\MHelpDisplayer.h
# End Source File
# Begin Source File

SOURCE=.\MHelpManager.h
# End Source File
# Begin Source File

SOURCE=.\MHelpMessageManager.h
# End Source File
# Begin Source File

SOURCE=.\MHelpStringTable.h
# End Source File
# End Group
# Begin Group "World header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MGameTime.h
# End Source File
# Begin Source File

SOURCE=.\MHelicopterManager.h
# End Source File
# Begin Source File

SOURCE=.\MSector.h
# End Source File
# Begin Source File

SOURCE=.\MSectorInfo.h
# End Source File
# Begin Source File

SOURCE=.\MWeather.h
# End Source File
# Begin Source File

SOURCE=.\MZoneSound.h
# End Source File
# Begin Source File

SOURCE=.\MZoneSoundManager.h
# End Source File
# End Group
# Begin Group "Party&Guild header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GuildInfo.h
# End Source File
# Begin Source File

SOURCE=.\GuildMemberInfo.h
# End Source File
# Begin Source File

SOURCE=.\MGuildInfoMapper.h
# End Source File
# Begin Source File

SOURCE=.\MGuildMarkManager.h
# End Source File
# Begin Source File

SOURCE=.\MParty.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\MChatManager.h
# End Source File
# Begin Source File

SOURCE=.\MJusticeAttackManager.h
# End Source File
# Begin Source File

SOURCE=.\MObjectSelector.h
# End Source File
# Begin Source File

SOURCE=.\ModifyStatusManager.h
# End Source File
# Begin Source File

SOURCE=.\MRequestMode.h
# End Source File
# Begin Source File

SOURCE=.\SectorSoundInfo.h
# End Source File
# Begin Source File

SOURCE=.\TalkBox.h
# End Source File
# Begin Source File

SOURCE=.\UIMessageManager.h
# End Source File
# End Group
# Begin Group "GameLoop"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\APICheck.h
# End Source File
# Begin Source File

SOURCE=.\CGameUpdate.h
# End Source File
# Begin Source File

SOURCE=.\Client.h
# End Source File
# Begin Source File

SOURCE=.\COpeningUpdate.h
# End Source File
# Begin Source File

SOURCE=.\CWaitPacketUpdate.h
# End Source File
# Begin Source File

SOURCE=.\CWaitUIUpdate.h
# End Source File
# End Group
# Begin Group "Utility"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AppendPatchInfo.h
# End Source File
# Begin Source File

SOURCE=.\CAvi.h
# End Source File
# Begin Source File

SOURCE=.\CDataTable.h
# End Source File
# Begin Source File

SOURCE=.\CMessageArray.h
# End Source File
# Begin Source File

SOURCE=.\COrderedList.h
# End Source File
# Begin Source File

SOURCE=.\CPartManager.h
# End Source File
# Begin Source File

SOURCE=.\CPositionList.h
# End Source File
# Begin Source File

SOURCE=.\CrashReport.h
# End Source File
# Begin Source File

SOURCE=.\CShadowPartManager.h
# End Source File
# Begin Source File

SOURCE=.\CSoundPartManager.h
# End Source File
# Begin Source File

SOURCE=.\CSpriteTexturePartManager.h
# End Source File
# Begin Source File

SOURCE=.\CTexturePartManager.h
# End Source File
# Begin Source File

SOURCE=.\CToken.h
# End Source File
# Begin Source File

SOURCE=.\CTypeMap.h
# End Source File
# Begin Source File

SOURCE=.\CTypeMap2.h
# End Source File
# Begin Source File

SOURCE=.\DllInfo.h
# End Source File
# Begin Source File

SOURCE=.\FameInfo.h
# End Source File
# Begin Source File

SOURCE=.\MathTable.h
# End Source File
# Begin Source File

SOURCE=.\MCrashReportManager.h
# End Source File
# Begin Source File

SOURCE=.\MFunctionManager.h
# End Source File
# Begin Source File

SOURCE=.\MLoadingSPKWorkNode.h
# End Source File
# Begin Source File

SOURCE=.\MMonsterKillQuestInfo.h
# End Source File
# Begin Source File

SOURCE=.\MMusic.h
# End Source File
# Begin Source File

SOURCE=.\MString.h
# End Source File
# Begin Source File

SOURCE=.\MStringArray.h
# End Source File
# Begin Source File

SOURCE=.\MStringList.h
# End Source File
# Begin Source File

SOURCE=.\MStringMap.h
# End Source File
# Begin Source File

SOURCE=.\MTimeItemManager.h
# End Source File
# Begin Source File

SOURCE=.\MWorkNode.h
# End Source File
# Begin Source File

SOURCE=.\MWorkThread.h
# End Source File
# Begin Source File

SOURCE=.\npfgmsdk.h
# End Source File
# Begin Source File

SOURCE=.\nProtect.h
# End Source File
# Begin Source File

SOURCE=.\PackFileManager.h
# End Source File
# Begin Source File

SOURCE=.\PCConfigTable.h
# End Source File
# Begin Source File

SOURCE=.\Profiler.h
# End Source File
# Begin Source File

SOURCE=.\RaceType.h
# End Source File
# Begin Source File

SOURCE=.\ShowTimeChecker.h
# End Source File
# Begin Source File

SOURCE=.\UtilityFunction.h
# End Source File
# Begin Source File

SOURCE=.\Packet\ValueList.h
# End Source File
# Begin Source File

SOURCE=.\WavePackFileManager.h
# End Source File
# End Group
# Begin Group "Player/Zone/View"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MPlayer.h
# End Source File
# Begin Source File

SOURCE=.\MStatus.h
# End Source File
# Begin Source File

SOURCE=.\MViewDef.h
# End Source File
# Begin Source File

SOURCE=.\MZone.h
# End Source File
# Begin Source File

SOURCE=.\MZoneDef.h
# End Source File
# Begin Source File

SOURCE=.\MZoneInfo.h
# End Source File
# Begin Source File

SOURCE=.\ZoneFileHeader.h
# End Source File
# End Group
# Begin Group "Packet Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Packet\Assert.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\BloodBibleBonusInfo.h
# End Source File
# Begin Source File

SOURCE=.\packet\cpackets\CGAbsorbSoul.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAcceptUnion.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddGearToMouse.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddInventoryToMouse.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddItemToCodeSheet.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddItemToItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddMouseToGear.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddMouseToInventory.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddMouseToQuickSlot.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddMouseToZone.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddQuickSlotToMouse.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddSMSAddress.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddZoneToInventory.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAddZoneToMouse.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAppointSubmaster.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAttack.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGAuthKey.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGBloodDrain.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGBuyStoreItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGCastingSkill.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGConnect.h
# End Source File
# Begin Source File

SOURCE=.\packet\cpackets\CGCrashReport.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDeleteSMSAddress.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDenyUnion.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDepositPet.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDialUp.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDisplayItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDissectionCorpse.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDonationMoney.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDownSkill.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGDropMoney.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGExpelGuild.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGFailQuest.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGGetEventItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGGetOffMotorCycle.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGGlobalChat.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGGQuestAccept.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGGQuestCancel.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGGuildChat.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGJoinGuild.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGLearnSkill.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGLogout.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGLotterySelect.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGMakeItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGMixItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGModifyGuildIntro.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGModifyGuildMember.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGModifyGuildMemberIntro.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGModifyNickname.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGModifyTaxRatio.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGMouseToStash.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGMove.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGNPCAskAnswer.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGNPCTalk.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPartyInvite.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPartyLeave.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPartyPosition.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPartySay.h
# End Source File
# Begin Source File

SOURCE=.\packet\cpackets\CGPetGamble.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPhoneDisconnect.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPhoneSay.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPickupMoney.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGPortCheck.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGQuitUnion.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGQuitUnionAccept.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGQuitUnionDeny.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRangerSay.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGReady.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRegistGuild.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRelicToObject.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGReloadFromInventory.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGReloadFromQuickSlot.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestGuildList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestGuildMemberList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestIP.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestNewbieItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestPowerPoint.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestRepair.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestStoreInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestUnion.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRequestUnionInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGResurrect.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGRideMotorCycle.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSay.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectBloodBible.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectGuild.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectGuildMember.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectNickname.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectPortal.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectQuest.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectRankBonus.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectRegenZone.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectTileEffect.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSelectWayPoint.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSetSlayerHotKey.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSetVampireHotKey.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGShopRequestBuy.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGShopRequestList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGShopRequestSell.h
# End Source File
# Begin Source File

SOURCE=.\packet\cpackets\CGSilverCoating.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSkillToInventory.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSkillToNamed.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSkillToObject.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSkillToSelf.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSkillToTile.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSMSAddressList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSMSSend.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStashDeposit.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStashList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStashRequestBuy.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStashToMouse.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStashWithdraw.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStoreClose.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStoreOpen.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGStoreSign.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGSubmitScore.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTakeOutGood.h
# End Source File
# Begin Source File

SOURCE=.\packet\cpackets\CGTameMonster.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGThrowBomb.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGThrowItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTradeAddItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTradeFinish.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTradeMoney.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTradePrepare.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTradeRemoveItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTryJoinGuild.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGTypeStringList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUnburrow.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUndisplayItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUntransform.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUseBonusPoint.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUseItemFromGear.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUseItemFromGQuestInventory.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUseItemFromInventory.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUseMessageItemFromInventory.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUsePotionFromInventory.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUsePotionFromQuickSlot.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGUsePowerPoint.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGVerifyTime.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGVisible.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGWhisper.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGWithdrawPet.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGWithdrawTax.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLChangeServer.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLCreatePC.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLDeletePC.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLGetPCList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLGetServerList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLGetWorldList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\ClientPlayer.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLLogin.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLLogout.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLQueryCharacterName.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLQueryPlayerID.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLReconnectLogin.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLRegisterPlayer.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLSelectPC.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLSelectServer.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLSelectWorld.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CLVersionCheck.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRConnect.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRDisconnect.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Types\CreatureTypes.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRRequest.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRRequest2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\CRWhisper.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Upackets\CURequestLoginMode.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Datagram.h
# End Source File
# Begin Source File

SOURCE=.\Packet\DatagramPacket.h
# End Source File
# Begin Source File

SOURCE=.\Packet\DatagramSocket.h
# End Source File
# Begin Source File

SOURCE=.\Packet\EffectInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Encrypter.h
# End Source File
# Begin Source File

SOURCE=.\Packet\EncryptUtility.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Exception.h
# End Source File
# Begin Source File

SOURCE=.\Packet\ExtraInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\ExtraSlotInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\FileAPI.h
# End Source File
# Begin Source File

SOURCE=.\Packet\GameTime.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCActiveGuildList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddBat.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddBurrowingCreature.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddEffect.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddEffectToTile.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddGearToInventory.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddGearToZone.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddHelicopter.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddInjuriousCreature.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddInstalledMineToZone.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddItemToInventory.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddItemToItemVerify.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddItemToZone.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddMonster.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddMonsterCorpse.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddMonsterFromBurrowing.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddMonsterFromTransformation.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddNewItemToZone.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddNickname.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddNPC.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddOusters.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddOustersCorpse.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddressListVerify.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddSlayer.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddSlayerCorpse.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddStoreItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampire.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampireCorpse.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampireFromBurrowing.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampireFromTransformation.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddVampirePortal.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAddWolf.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttack.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK3.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK4.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackArmsOK5.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackMeleeOK1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackMeleeOK2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAttackMeleeOK3.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCAuthKey.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodBibleList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodBibleSignInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodBibleStatus.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodDrainOK1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodDrainOK2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCBloodDrainOK3.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCannotAdd.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCannotUse.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCastingSkill.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCChangeDarkLight.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCChangeInventoryItemNum.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCChangeShape.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCChangeWeather.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCreateItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCreatureDied.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCrossCounterOK1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCrossCounterOK2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCCrossCounterOK3.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDeleteandPickUpOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDeleteEffectFromTile.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDeleteInventoryItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDeleteObject.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDisconnect.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDownSkillFailed.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDownSkillOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCDropItemToZone.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCEnterVampirePortal.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCExecuteElement.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCFakeMove.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCFastMove.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCFlagWarStatus.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCFriendChatting.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGetDamage.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGetOffMotorCycle.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGetOffMotorCycleFailed.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGetOffMotorCycleOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGlobalChat.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGoodsList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGQuestInventory.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGQuestStatusInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGQuestStatusModify.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGuildChat.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGuildMemberList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCGuildResponse.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHolyLandBonusInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHPRecoveryEndToOthers.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHPRecoveryEndToSelf.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHPRecoveryStartToOthers.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCHPRecoveryStartToSelf.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKickMessage.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnockBack.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnocksTargetBackOK1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnocksTargetBackOK2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnocksTargetBackOK4.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCKnocksTargetBackOK5.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCLearnSkillFailed.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCLearnSkillOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCLearnSkillReady.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCLightning.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMakeItemFail.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMakeItemOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMineExplosionOK1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMineExplosionOK2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMiniGameScores.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCModifyGuildMemberInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCModifyInformation.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCModifyNickname.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMonsterKillQuestInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMorph1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMorphSlayer2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMorphVampire2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMove.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMoveError.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMoveOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMPRecoveryEnd.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMPRecoveryStart.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCMyStoreInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNicknameList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNicknameVerify.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNoticeEvent.h
# End Source File
# Begin Source File

SOURCE=.\packet\gpackets\GCNotifyWin.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCAsk.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCAskDynamic.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCAskVariable.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCResponse.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCSay.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCNPCSayDynamic.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCOtherGuildName.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCOtherModifyInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCOtherModifyInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCOtherStoreInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyError.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyInvite.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyJoined.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyLeave.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartyPosition.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPartySay.h
# End Source File
# Begin Source File

SOURCE=.\packet\gpackets\GCPetInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPetStashList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPetStashVerify.h
# End Source File
# Begin Source File

SOURCE=.\packet\gpackets\GCPetUseSkill.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPhoneConnected.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPhoneConnectionFailed.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPhoneDisconnected.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCPhoneSay.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCQuestStatus.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRankBonusInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRealWearingInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCReconnect.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCReconnectLogin.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRegenZoneStatus.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCReloadOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveCorpseHead.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveEffect.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveFromGear.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveInjuriousCreature.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRemoveStoreItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRequestedIP.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRequestFailed.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRequestPowerPointResult.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRideMotorCycle.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRideMotorCycleFailed.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRideMotorCycleOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCRing.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSay.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSearchMotorcycleFail.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSearchMotorcycleOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSelectQuestID.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSelectRankBonusFailed.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSelectRankBonusOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSetPosition.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopBought.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopBuyFail.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopBuyOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopListMysterious.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopMarketCondition.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopSellFail.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopSellOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopSold.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShopVersion.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowGuildInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowGuildJoin.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowGuildMemberInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowUnionInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCShowWaitGuildInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillFailed1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillFailed2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToInventoryOK1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToInventoryOK2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK3.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK4.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK5.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToObjectOK6.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToSelfOK1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToSelfOK2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToSelfOK3.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK3.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK4.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK5.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSkillToTileOK6.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSMSAddressList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCStashList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCStashSell.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCStatusCurrentHP.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSubInventoryInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSweeperBonusInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSystemAvailabilities.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCSystemMessage.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTakeOff.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTakeOutFail.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTakeOutOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTeachSkillInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowBombOK1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowBombOK2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowBombOK3.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowItemOK1.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowItemOK2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCThrowItemOK3.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTimeLimitItemInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeAddItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeError.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeFinish.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeMoney.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradePrepare.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeRemoveItem.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCTradeVerify.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUnburrowFail.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUnburrowOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUnionOfferList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUntransformFail.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUntransformOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUpdateInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUseBonusPointFail.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUseBonusPointOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUseOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCUsePowerPointResult.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCVisibleFail.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCVisibleOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWaitGuildList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWarList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWarScheduleList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWhisper.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GCWhisperFailed.h
# End Source File
# Begin Source File

SOURCE=.\Packet\GearInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\GearSlotInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GLIncomingConnectionError.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\GLIncomingConnectionOK.h
# End Source File
# Begin Source File

SOURCE=.\packet\types\GuildTypes.h
# End Source File
# Begin Source File

SOURCE=.\Packet\GuildWarInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\InventoryInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\InventorySlotInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Types\ItemTypes.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCCreatePCError.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCCreatePCOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCDeletePCError.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCDeletePCOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCLoginError.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCLoginOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCPCList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCQueryResultCharacterName.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCQueryResultPlayerID.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCReconnect.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCRegisterPlayerError.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCRegisterPlayerOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCSelectPCError.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCServerList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCVersionCheckError.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCVersionCheckOK.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LCWorldList.h
# End Source File
# Begin Source File

SOURCE=.\packet\LevelWarInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Lpackets\LGIncomingConnection.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Types\ModifyDef.h
# End Source File
# Begin Source File

SOURCE=.\Packet\ModifyInfo.h
# End Source File
# Begin Source File

SOURCE=.\MWarManager.H
# End Source File
# Begin Source File

SOURCE=.\Packet\NPCInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Types\ObjectTypes.h
# End Source File
# Begin Source File

SOURCE=.\Packet\OustersSkillInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Packet.h
# End Source File
# Begin Source File

SOURCE=.\PacketDef.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PacketFactory.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PacketFactoryManager.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PacketIDSet.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Types\PacketItemDef.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PacketValidator.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PCInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PCItemInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PCOustersInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PCOustersInfo2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PCOustersInfo3.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PCSkillInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PCSlayerInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PCSlayerInfo2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PCSlayerInfo3.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PCVampireInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PCVampireInfo2.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PCVampireInfo3.h
# End Source File
# Begin Source File

SOURCE=.\packet\PetInfo.h
# End Source File
# Begin Source File

SOURCE=.\packet\types\PetTypes.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Player.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PlayerInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PlayerStatus.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Types\PlayerTypes.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Properties.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Types\QuestTypes.h
# End Source File
# Begin Source File

SOURCE=.\Packet\RaceWarInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCCharacterInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCConnectVerify.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCRequestedFile.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCRequestVerify.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCSay.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Rpackets\RCStatusHP.h
# End Source File
# Begin Source File

SOURCE=.\Packet\RideMotorcycleInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\RideMotorcycleSlotInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\ScriptParameter.h
# End Source File
# Begin Source File

SOURCE=.\Packet\ServerGroupInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\ServerSocket.h
# End Source File
# Begin Source File

SOURCE=.\packet\types\ShopTypes.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SlayerSkillInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Socket.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketAPI.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketEncryptInputStream.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketEncryptOutputStream.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketImpl.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketInputStream.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketOutputStream.h
# End Source File
# Begin Source File

SOURCE=.\StoreInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\StringStream.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SubItemInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SubOustersSkillInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SubServerInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SubSlayerSkillInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SubVampireSkillInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Gpackets\SweeperBonusInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Types\SystemTypes.h
# End Source File
# Begin Source File

SOURCE=.\Packet\TextInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Types.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Upackets\UCRequestLoginMode.h
# End Source File
# Begin Source File

SOURCE=.\Packet\VampireSkillInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\WarInfo.h
# End Source File
# Begin Source File

SOURCE=.\packet\types\WarTypes.h
# End Source File
# Begin Source File

SOURCE=.\Packet\WorldInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Types\ZoneTypes.h
# End Source File
# End Group
# Begin Group "Patch"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Updater\Update.h
# End Source File
# Begin Source File

SOURCE=.\Updater\UpdateDef.h
# End Source File
# Begin Source File

SOURCE=.\Updater\UpdateManager.h
# End Source File
# Begin Source File

SOURCE=.\Updater\UpdateUtility.h
# End Source File
# End Group
# Begin Group "RequestPlayer header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Packet\ClientCommunicationManager.h
# End Source File
# Begin Source File

SOURCE=.\ProfileManager.h
# End Source File
# Begin Source File

SOURCE=.\Packet\RequestClientPlayer.h
# End Source File
# Begin Source File

SOURCE=.\Packet\RequestClientPlayerManager.h
# End Source File
# Begin Source File

SOURCE=.\RequestFileManager.h
# End Source File
# Begin Source File

SOURCE=.\RequestFunction.h
# End Source File
# Begin Source File

SOURCE=.\Packet\RequestServerPlayer.h
# End Source File
# Begin Source File

SOURCE=.\Packet\RequestServerPlayerManager.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Types\RequestTypes.h
# End Source File
# Begin Source File

SOURCE=.\RequestUserManager.h
# End Source File
# Begin Source File

SOURCE=.\WhisperManager.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ACProtect.h
# End Source File
# Begin Source File

SOURCE=.\AddonDef.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Cpackets\CGConnectSetKey.h
# End Source File
# Begin Source File

SOURCE=..\VS_UI\lib\Client_PCH.h
# End Source File
# Begin Source File

SOURCE=.\ClientConfig.h
# End Source File
# Begin Source File

SOURCE=.\ClientDef.h
# End Source File
# Begin Source File

SOURCE=.\ClientFunction.h
# End Source File
# Begin Source File

SOURCE=.\CSystemInfo.h
# End Source File
# Begin Source File

SOURCE=.\DebugInfo.h
# End Source File
# Begin Source File

SOURCE=.\DebugKit.h
# End Source File
# Begin Source File

SOURCE=.\DrawItemNameNode.h
# End Source File
# Begin Source File

SOURCE=.\DrawTextNode.h
# End Source File
# Begin Source File

SOURCE=.\EffectSpriteTypeDef.h
# End Source File
# Begin Source File

SOURCE=.\ItemClassDef.h
# End Source File
# Begin Source File

SOURCE=.\MActionResultDef.h
# End Source File
# Begin Source File

SOURCE=.\md5.h
# End Source File
# Begin Source File

SOURCE=.\MEffectGeneratorDef.h
# End Source File
# Begin Source File

SOURCE=.\MEffectStatusDef.h
# End Source File
# Begin Source File

SOURCE=.\MEventManager.h
# End Source File
# Begin Source File

SOURCE=.\MFileDef.h
# End Source File
# Begin Source File

SOURCE=.\MGameDef.h
# End Source File
# Begin Source File

SOURCE=.\MGuildType.h
# End Source File
# Begin Source File

SOURCE=.\MHelpDef.h
# End Source File
# Begin Source File

SOURCE=.\MQuestInfo.h
# End Source File
# Begin Source File

SOURCE=.\MTestDef.h
# End Source File
# Begin Source File

SOURCE=.\MTopView.h
# End Source File
# Begin Source File

SOURCE=.\MTypeDef.h
# End Source File
# Begin Source File

SOURCE=.\MusicDef.h
# End Source File
# Begin Source File

SOURCE=.\NMCrypt.h
# End Source File
# Begin Source File

SOURCE=.\PacketFunction.h
# End Source File
# Begin Source File

SOURCE=.\PacketFunction2.h
# End Source File
# Begin Source File

SOURCE=.\RankBonusDef.h
# End Source File
# Begin Source File

SOURCE=.\OtherClass\RequestClientPacketFactoryManager.h
# End Source File
# Begin Source File

SOURCE=.\OtherClass\RequestServerPacketFactoryManager.h
# End Source File
# Begin Source File

SOURCE=.\ServerInfo.h
# End Source File
# Begin Source File

SOURCE=.\ServerInfoFileParser.h
# End Source File
# Begin Source File

SOURCE=.\SkillDef.h
# End Source File
# Begin Source File

SOURCE=.\SoundDef.h
# End Source File
# Begin Source File

SOURCE=.\SoundSetting.h
# End Source File
# Begin Source File

SOURCE=.\SpriteIDDef.h
# End Source File
# Begin Source File

SOURCE=.\UIDialog.h
# End Source File
# Begin Source File

SOURCE=.\UIFunction.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DXLib\CDirectDraw.h
# End Source File
# Begin Source File

SOURCE=.\SpriteLib\CFileIndexTable.h
# End Source File
# Begin Source File

SOURCE=.\darkeden.ico
# End Source File
# Begin Source File

SOURCE=.\darkeden1.ico
# End Source File
# Begin Source File

SOURCE=.\EXECryptor.h
# End Source File
# Begin Source File

SOURCE=.\fengshen.ico
# End Source File
# Begin Source File

SOURCE=.\GAME1024.h
# End Source File
# Begin Source File

SOURCE=.\ShrineInfoManager.h
# End Source File
# Begin Source File

SOURCE=.\SoundNode.h
# End Source File
# Begin Source File

SOURCE=.\SystemAvailabilities.h
# End Source File
# Begin Source File

SOURCE=.\NPGameLib.lib
# End Source File
# Begin Source File

SOURCE=.\NMCrypt.lib
# End Source File
# End Target
# End Project
