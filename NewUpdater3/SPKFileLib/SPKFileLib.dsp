# Microsoft Developer Studio Project File - Name="SPKFileLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SPKFileLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SPKFileLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SPKFileLib.mak" CFG="SPKFileLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SPKFileLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SPKFileLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/darkeden/Client/ДњТы/NewUpdater3/SPKFileLib", XPUAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SPKFileLib - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O1 /I "..\..\client" /I "..\..\client\SpriteLib" /I "..\..\client\WinLib" /I "..\..\updater" /I "Packet" /I "DSetup" /I "SPKFileLib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SPKFileLib - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\client" /I "..\..\client\SpriteLib" /I "..\..\client\WinLib" /I "..\..\updater" /I "Packet" /I "DSetup" /I "SPKFileLib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "SPKFileLib - Win32 Release"
# Name "SPKFileLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CAlphaSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\CAlphaSpritePack.cpp
# End Source File
# Begin Source File

SOURCE=.\CIndexSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\CIndexSpritePack.cpp
# End Source File
# Begin Source File

SOURCE=.\CShadowSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\CShadowSpritePack.cpp
# End Source File
# Begin Source File

SOURCE=.\CSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpriteIDTable.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpritePack.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CAlphaSprite.h
# End Source File
# Begin Source File

SOURCE=.\CAlphaSpritePack.h
# End Source File
# Begin Source File

SOURCE=.\CIndexSprite.h
# End Source File
# Begin Source File

SOURCE=.\CIndexSpritePack.h
# End Source File
# Begin Source File

SOURCE=.\CShadowSprite.h
# End Source File
# Begin Source File

SOURCE=.\CShadowSpritePack.h
# End Source File
# Begin Source File

SOURCE=.\CSprite.h
# End Source File
# Begin Source File

SOURCE=.\CSpriteBase.h
# End Source File
# Begin Source File

SOURCE=.\CSpriteIDTable.h
# End Source File
# Begin Source File

SOURCE=.\CSpritePack.h
# End Source File
# Begin Source File

SOURCE=.\CSpritePackBase.h
# End Source File
# Begin Source File

SOURCE=.\DrawTypeDef.h
# End Source File
# Begin Source File

SOURCE=.\SPKFileLib.h
# End Source File
# End Group
# End Target
# End Project
