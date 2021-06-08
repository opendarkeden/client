# Microsoft Developer Studio Project File - Name="MapSoundManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MapSoundManager - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MapSoundManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MapSoundManager.mak" CFG="MapSoundManager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MapSoundManager - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MapSoundManager - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MapSoundManager", YDNAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MapSoundManager - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FD /c
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
# ADD LINK32 dx.lib dxguid.lib ddraw.lib dsound.lib winmm.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\lib"

!ELSEIF  "$(CFG)" == "MapSoundManager - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\lib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FD /GZ /c
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
# ADD LINK32 dx.lib dxguid.lib ddraw.lib dsound.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\lib"

!ENDIF 

# Begin Target

# Name "MapSoundManager - Win32 Release"
# Name "MapSoundManager - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MapSoundManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MapSoundManager.rc
# End Source File
# Begin Source File

SOURCE=.\MapSoundManagerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NameDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddDialog.h
# End Source File
# Begin Source File

SOURCE=.\MapSoundManager.h
# End Source File
# Begin Source File

SOURCE=.\MapSoundManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\NameDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\MapSoundManager.ico
# End Source File
# Begin Source File

SOURCE=.\res\MapSoundManager.rc2
# End Source File
# End Group
# Begin Group "ex"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CPartManager.h
# End Source File
# Begin Source File

SOURCE=.\CSoundPartManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CSoundPartManager.h
# End Source File
# Begin Source File

SOURCE=.\CTypeMap.h
# End Source File
# Begin Source File

SOURCE=.\CTypeTable.h
# End Source File
# Begin Source File

SOURCE=.\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\Image.h
# End Source File
# Begin Source File

SOURCE=.\ImageFileBmp.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageFileTga.cpp
# End Source File
# Begin Source File

SOURCE=.\MSoundTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MSoundTable.h
# End Source File
# Begin Source File

SOURCE=.\MString.cpp
# End Source File
# Begin Source File

SOURCE=.\MString.h
# End Source File
# Begin Source File

SOURCE=.\MTypeDef.h
# End Source File
# Begin Source File

SOURCE=.\MZoneSound.cpp
# End Source File
# Begin Source File

SOURCE=.\MZoneSound.h
# End Source File
# Begin Source File

SOURCE=.\ShowTimeChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowTimeChecker.h
# End Source File
# Begin Source File

SOURCE=.\SoundDef.h
# End Source File
# Begin Source File

SOURCE=.\Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite.h
# End Source File
# Begin Source File

SOURCE=.\SpritePack.cpp
# End Source File
# Begin Source File

SOURCE=.\SpritePack.h
# End Source File
# Begin Source File

SOURCE=.\ZoneSoundInfoEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ZoneSoundInfoEdit.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
