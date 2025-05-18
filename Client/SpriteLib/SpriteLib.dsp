# Microsoft Developer Studio Project File - Name="SpriteLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SpriteLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SpriteLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SpriteLib.mak" CFG="SpriteLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SpriteLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SpriteLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Client/SpriteLib", LXFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SpriteLib - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O1 /Ob2 /I "..\dxlib" /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "__TEST_SUB_INVENTORY__" /FD /c
# SUBTRACT CPP /Z<none> /YX /Yc /Yu
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\SP.lib"

!ELSEIF  "$(CFG)" == "SpriteLib - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\dxlib" /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "__TEST_SUB_INVENTORY__" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\SP.lib"

!ENDIF 

# Begin Target

# Name "SpriteLib - Win32 Release"
# Name "SpriteLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CAlphaSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\CAlphaSprite555.cpp
# End Source File
# Begin Source File

SOURCE=.\CAlphaSprite565.cpp
# End Source File
# Begin Source File

SOURCE=.\CAlphaSpritePal.cpp
# End Source File
# Begin Source File

SOURCE=.\CFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\CIndexSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\CIndexSprite555.cpp
# End Source File
# Begin Source File

SOURCE=.\CIndexSprite565.cpp
# End Source File
# Begin Source File

SOURCE=.\CIndexSpritePack.cpp
# End Source File
# Begin Source File

SOURCE=.\CShadowSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\CSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\CSprite555.cpp
# End Source File
# Begin Source File

SOURCE=.\CSprite565.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpriteDef.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpriteOutlineManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpritePack.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpritePackList.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpritePal.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpritePalBase.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpriteSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpriteSetManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpriteSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\CStorageSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\MPalette.cpp
# End Source File
# Begin Source File

SOURCE=.\MPalettePack.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CAlphaSpritePal.h
# End Source File
# Begin Source File

SOURCE=.\CFileIndexTable.h
# End Source File
# Begin Source File

SOURCE=.\CFilter.h
# End Source File
# Begin Source File

SOURCE=.\CIndexSprite.h
# End Source File
# Begin Source File

SOURCE=.\CIndexSprite555.h
# End Source File
# Begin Source File

SOURCE=.\CIndexSprite565.h
# End Source File
# Begin Source File

SOURCE=.\CIndexSpritePack.h
# End Source File
# Begin Source File

SOURCE=.\CSetManager.h
# End Source File
# Begin Source File

SOURCE=.\CShadowSprite.h
# End Source File
# Begin Source File

SOURCE=.\CSprite.h
# End Source File
# Begin Source File

SOURCE=.\CSprite555.h
# End Source File
# Begin Source File

SOURCE=.\CSprite565.h
# End Source File
# Begin Source File

SOURCE=.\CSpriteDef.h
# End Source File
# Begin Source File

SOURCE=.\CSpriteOutlineManager.h
# End Source File
# Begin Source File

SOURCE=.\CSpritePack.h
# End Source File
# Begin Source File

SOURCE=.\CSpritePackList.h
# End Source File
# Begin Source File

SOURCE=.\CSpritePackList555.h
# End Source File
# Begin Source File

SOURCE=.\CSpritePackList565.h
# End Source File
# Begin Source File

SOURCE=.\CSpritePal.h
# End Source File
# Begin Source File

SOURCE=.\CSpritePalBase.h
# End Source File
# Begin Source File

SOURCE=.\CSpriteSet.h
# End Source File
# Begin Source File

SOURCE=.\CSpriteSetManager.h
# End Source File
# Begin Source File

SOURCE=.\CSpriteSurface.h
# End Source File
# Begin Source File

SOURCE=.\CStorageSurface.h
# End Source File
# Begin Source File

SOURCE=.\CTypePack.h
# End Source File
# Begin Source File

SOURCE=.\CTypePackVector.h
# End Source File
# Begin Source File

SOURCE=.\DrawTypeDef.h
# End Source File
# Begin Source File

SOURCE=.\MPalette.h
# End Source File
# Begin Source File

SOURCE=.\MPalettePack.h
# End Source File
# Begin Source File

SOURCE=.\SP.h
# End Source File
# Begin Source File

SOURCE=.\TArray.h
# End Source File
# End Group
# End Target
# End Project
