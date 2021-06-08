# Microsoft Developer Studio Project File - Name="SpriteManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SpriteManager - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SpriteManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SpriteManager.mak" CFG="SpriteManager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SpriteManager - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SpriteManager - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SpriteManagerL", GKMAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SpriteManager - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\lib_ole" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\lib_ole\dx_spritemanager.lib ddraw.lib dxguid.lib ..\lib_ole\gl2.lib ..\lib_ole\fr.lib ..\lib_ole\sp_release.lib winmm.lib /nologo /subsystem:windows /incremental:yes /machine:I386 /nodefaultlib:"libcmt.lib" /nodefaultlib:"libicmt.lib" /libpath:"..\lib_ole"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "SpriteManager - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\lib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fr /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\lib\sp_RELEASE.lib ..\lib\dx_spritemanager.lib ddraw.lib dxguid.lib ..\lib\gl2.lib ..\lib\fr.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /nodefaultlib:"libcmtd.lib" /pdbtype:sept /libpath:"..\lib"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "SpriteManager - Win32 Release"
# Name "SpriteManager - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AniDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CDib.cpp
# End Source File
# Begin Source File

SOURCE=.\ConvertProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSearchOption.cpp
# End Source File
# Begin Source File

SOURCE=.\InputCount.cpp
# End Source File
# Begin Source File

SOURCE=.\InputOffset.cpp
# End Source File
# Begin Source File

SOURCE=.\MakeSprDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\smartcutting.cpp
# End Source File
# Begin Source File

SOURCE=.\SpriteManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SpriteManager.rc
# End Source File
# Begin Source File

SOURCE=.\SpriteManagerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActionEnum.h
# End Source File
# Begin Source File

SOURCE=.\AniDlg.h
# End Source File
# Begin Source File

SOURCE=.\cdib.h
# End Source File
# Begin Source File

SOURCE=.\ConvertProgress.h
# End Source File
# Begin Source File

SOURCE=.\CUndo.h
# End Source File
# Begin Source File

SOURCE=.\DIBlist.h
# End Source File
# Begin Source File

SOURCE=.\DlgSearchOption.h
# End Source File
# Begin Source File

SOURCE=.\InputCount.h
# End Source File
# Begin Source File

SOURCE=.\InputOffset.h
# End Source File
# Begin Source File

SOURCE=.\MakeSprDlg.h
# End Source File
# Begin Source File

SOURCE=.\OustersActionPart.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\PromotionSlayerActionPart.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SlayerActionPart.h
# End Source File
# Begin Source File

SOURCE=.\smartcutting.h
# End Source File
# Begin Source File

SOURCE=.\sprite.h
# End Source File
# Begin Source File

SOURCE=.\SpriteManager.h
# End Source File
# Begin Source File

SOURCE=.\SpriteManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VampireActionPart.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bg1.bmp
# End Source File
# Begin Source File

SOURCE=.\bg2.bmp
# End Source File
# Begin Source File

SOURCE=.\bg3.bmp
# End Source File
# Begin Source File

SOURCE=.\bg4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_d.cur
# End Source File
# Begin Source File

SOURCE=.\res\SpriteManager.ico
# End Source File
# Begin Source File

SOURCE=.\res\SpriteManager.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
