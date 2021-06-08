# Microsoft Developer Studio Project File - Name="lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lib.mak" CFG="lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/VS_UI/lib", OFKAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "lib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O1 /I "\Game(800)\designed\project\client" /I "\Game(800)\designed\project\client\dxlib" /I "\Game(800)\designed\project\client\spriteLib" /I "\Game(800)\designed\project\client\frameLib" /I "..\src\widget" /I "..\src\hangul" /I "..\src\header" /I "..\src\imm" /I "\Game(800)\designed\project\basic" /I "..\..\lib" /I "..\..\basic" /I "\Game(800)\designed\project\client\sxml" /I "\Game(800)\designed\project\VS_UI" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"\Game(800)\designed\project\lib\ui\VS_UI_release.lib"

!ELSEIF  "$(CFG)" == "lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "\game(800)\designed\project\client" /I "\game(800)\designed\project\client\dxlib" /I "\game(800)\designed\project\client\spriteLib" /I "\game(800)\designed\project\client\frameLib" /I "..\src\widget" /I "..\src\hangul" /I "..\src\header" /I "..\src\imm" /I "\designed\basic" /I "..\..\basic" /I "..\..\lib\ui" /I "\game(800)\designed\project\client\sxml" /I "\game(800)\designed\project\VS_UI" /I "\game(800)\designed\project\basic" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fr /Yu"client_PCH.h" /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"\Game(800)\designed\project\lib\ui\VS_UI_debug.lib"

!ENDIF 

# Begin Target

# Name "lib - Win32 Release"
# Name "lib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\AcceleratorDef.cpp
# End Source File
# Begin Source File

SOURCE=..\src\AcceleratorManager.cpp
# End Source File
# Begin Source File

SOURCE=..\src\hangul\Ci.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Imm\CImm.cpp
# End Source File
# Begin Source File

SOURCE="K:\game(800)\designed\project\VS_UI\lib\Client_PCH.cpp"
# ADD CPP /Yc"client_PCH.h"
# End Source File
# Begin Source File

SOURCE=..\src\hangul\FL2.cpp
# End Source File
# Begin Source File

SOURCE=..\src\KeyAccelerator.cpp
# End Source File
# Begin Source File

SOURCE=..\src\widget\mother.cpp
# End Source File
# Begin Source File

SOURCE=..\src\widget\pi_core.cpp
# End Source File
# Begin Source File

SOURCE=..\src\SkinManager.cpp
# End Source File
# Begin Source File

SOURCE=..\SXml.cpp
# End Source File
# Begin Source File

SOURCE=..\src\widget\u_button.cpp
# End Source File
# Begin Source File

SOURCE=..\src\widget\U_edit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\widget\u_scrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\src\widget\u_window.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Vs_ui.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Base.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_BBS.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_DESC.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Description.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Descriptor.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Dialog.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_ELEVATOR.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Exception.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Exchange.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_ExtraDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Game.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_GameCommon.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vs_ui_gamecommon2.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_GameOusters.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_GameSlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_GameVampire.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_GlobalResource.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Helper.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Item.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Message.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_mouse_pointer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_PetStorage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_progress.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Shop.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_skill_tree.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Storage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Title.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_TITLE_SHOWCHAR.CPP
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_Tutorial.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_ui_result_receiver.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_util.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_WebBrowser.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VS_UI_widget.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\header\AcceleratorDef.h
# End Source File
# Begin Source File

SOURCE=..\src\header\AcceleratorManager.h
# End Source File
# Begin Source File

SOURCE=..\src\hangul\Ci.h
# End Source File
# Begin Source File

SOURCE="K:\game(800)\designed\project\VS_UI\lib\Client_PCH.h"
# End Source File
# Begin Source File

SOURCE=..\src\hangul\Fl2.h
# End Source File
# Begin Source File

SOURCE=..\src\header\KeyAccelerator.h
# End Source File
# Begin Source File

SOURCE=..\src\widget\mother.h
# End Source File
# Begin Source File

SOURCE=..\src\widget\PI.H
# End Source File
# Begin Source File

SOURCE=..\src\widget\pi_core.h
# End Source File
# Begin Source File

SOURCE=..\src\widget\SimpleDataList.h
# End Source File
# Begin Source File

SOURCE=..\src\header\SkinManager.h
# End Source File
# Begin Source File

SOURCE=..\src\widget\u_button.h
# End Source File
# Begin Source File

SOURCE=..\src\widget\U_edit.h
# End Source File
# Begin Source File

SOURCE=..\src\widget\u_scrollbar.h
# End Source File
# Begin Source File

SOURCE=..\src\widget\u_window.h
# End Source File
# Begin Source File

SOURCE=..\src\widget\unicorn.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_Base.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_bbs.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_DESC.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_Description.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_descriptor.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_Dialog.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_ELEVATOR.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_Exchange.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_ExtraDialog.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_filepath.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_Game.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_GameCommon.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_GameCommon2.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_GameOusters.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_GameSlayer.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_GameVampire.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_GlobalResource.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_Helper.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_item.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_Message.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_mouse_pointer.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_PetStorage.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_progress.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_Shop.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_skill_tree.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_Storage.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_title.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_Tutorial.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_ui_result_receiver.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_UIMessage.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_util.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_WebBrowser.h
# End Source File
# Begin Source File

SOURCE=..\src\header\VS_UI_widget.h
# End Source File
# End Group
# End Target
# End Project
