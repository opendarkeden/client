# Microsoft Developer Studio Project File - Name="EffectManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=EffectManager - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EffectManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EffectManager.mak" CFG="EffectManager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EffectManager - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EffectManager - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EffectManager", TGMAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EffectManager - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O1 /I "..\client\spritelib" /I "..\lib_ole" /I "..\client" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 \game(800)\designed\project\client\spriteLib\release\sp.lib \game(800)\designed\project\lib\gl.lib \game(800)\designed\project\client\DirectXLib\release\dx.lib dxguid.lib ddraw.lib \game(800)\designed\project\client\framelib\release\fr.lib ..\lib_ole\Image_release.lib ..\Lib_ole\jpeg_release.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386

!ELSEIF  "$(CFG)" == "EffectManager - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\client\spritelib" /I "..\lib_ole" /I "..\client" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 \game(800)\designed\project\client\spriteLib\release\sp.lib \game(800)\designed\project\lib\gl.lib \game(800)\designed\project\client\DirectXLib\release\dx.lib dxguid.lib ddraw.lib \game(800)\designed\project\client\framelib\release\fr.lib ..\Lib_ole\Image_release.lib ..\Lib_ole\jpeg_release.lib winmm.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "EffectManager - Win32 Release"
# Name "EffectManager - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\client\SpriteLib\CAlphaSpritePal.cpp
# End Source File
# Begin Source File

SOURCE=.\CBdebug.cpp
# End Source File
# Begin Source File

SOURCE=.\CDitherUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\CIndexSprite.cpp
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\CIndexSprite555.cpp
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\CIndexSprite565.cpp
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\CSpritePal.cpp
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\CSpritePalBase.cpp
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\CSpriteSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpriteTemp.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogInput.cpp
# End Source File
# Begin Source File

SOURCE=.\EffectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\EffectManager.rc
# End Source File
# Begin Source File

SOURCE=.\EffectManagerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageFileBmp.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertBlankDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\MPalette.cpp
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\MPalettePack.cpp
# End Source File
# Begin Source File

SOURCE=.\NameDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PaletteDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\smartcutting.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\client\SpriteLib\CAlphaSpritePal.h
# End Source File
# Begin Source File

SOURCE=.\CBdebug.h
# End Source File
# Begin Source File

SOURCE=.\CDitherUtil.h
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\CSpritePal.h
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\CSpritePalBase.h
# End Source File
# Begin Source File

SOURCE=.\CSpriteTemp.h
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\CTypePack.h
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\CTypePackVector.h
# End Source File
# Begin Source File

SOURCE=.\DialogInput.h
# End Source File
# Begin Source File

SOURCE=.\EffectManager.h
# End Source File
# Begin Source File

SOURCE=.\EffectManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\Image.h
# End Source File
# Begin Source File

SOURCE=.\InsertBlankDialog.h
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\MPalette.h
# End Source File
# Begin Source File

SOURCE=..\client\SpriteLib\MPalettePack.h
# End Source File
# Begin Source File

SOURCE=.\NameDialog.h
# End Source File
# Begin Source File

SOURCE=.\PaletteDialog.h
# End Source File
# Begin Source File

SOURCE=.\PreviewDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\smartcutting.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\arrowcop.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\EffectManager.ico
# End Source File
# Begin Source File

SOURCE=.\res\EffectManager.rc2
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\pointer1.cur
# End Source File
# Begin Source File

SOURCE=.\res\pointer_.cur
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
