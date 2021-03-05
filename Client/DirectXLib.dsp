# Microsoft Developer Studio Project File - Name="DirectXLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=DirectXLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DirectXLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DirectXLib.mak" CFG="DirectXLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DirectXLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "DirectXLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Client/DirectXLib", FVFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DirectXLib - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O1 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"DirectXlib_PCH.h" /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\DX.lib"

!ELSEIF  "$(CFG)" == "DirectXLib - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"DirectXlib_PCH.h" /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\DX.lib"

!ENDIF 

# Begin Target

# Name "DirectXLib - Win32 Release"
# Name "DirectXLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BIT_RES.CPP
# End Source File
# Begin Source File

SOURCE=.\CDirectDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\CDirectDrawSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\CDirectInput.cpp
# End Source File
# Begin Source File

SOURCE=.\CDirectMusic.cpp
# End Source File
# Begin Source File

SOURCE=.\CDirectSetupGetVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\CDirectSound.cpp
# End Source File
# Begin Source File

SOURCE=.\CDirectSoundStream.cpp
# End Source File
# Begin Source File

SOURCE=..\CMP3.cpp
# End Source File
# Begin Source File

SOURCE=..\COGGSTREAM.CPP
# End Source File
# Begin Source File

SOURCE=.\DirectXLib_PCH.cpp
# ADD CPP /Yc"DirectXlib_PCH.h"
# End Source File
# Begin Source File

SOURCE=.\header.cpp
# End Source File
# Begin Source File

SOURCE=.\huffman.cpp
# End Source File
# Begin Source File

SOURCE=.\mp3.cpp
# End Source File
# Begin Source File

SOURCE=.\reader.cpp
# End Source File
# Begin Source File

SOURCE=.\soundbuf.cpp
# End Source File
# Begin Source File

SOURCE=.\subdecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\synfilt.cpp
# End Source File
# Begin Source File

SOURCE=.\vorbis.lib
# End Source File
# Begin Source File

SOURCE=.\vorbisfile.lib
# End Source File
# Begin Source File

SOURCE=.\ogg.lib
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BIT_RES.H
# End Source File
# Begin Source File

SOURCE=.\CDirectDraw.h
# End Source File
# Begin Source File

SOURCE=.\CDirectDrawSurface.h
# End Source File
# Begin Source File

SOURCE=.\CDirectInput.h
# End Source File
# Begin Source File

SOURCE=.\CDirectMusic.h
# End Source File
# Begin Source File

SOURCE=.\CDirectSetup.h
# End Source File
# Begin Source File

SOURCE=.\CDirectSound.h
# End Source File
# Begin Source File

SOURCE=.\CDirectSoundStream.h
# End Source File
# Begin Source File

SOURCE=..\CMP3.h
# End Source File
# Begin Source File

SOURCE=..\DirectXLib\codec.h
# End Source File
# Begin Source File

SOURCE=..\COGGSTREAM.H
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=..\DirectXLib\config_types.h
# End Source File
# Begin Source File

SOURCE=.\DirectXLib_PCH.h
# End Source File
# Begin Source File

SOURCE=.\DXLib.h
# End Source File
# Begin Source File

SOURCE=.\header.h
# End Source File
# Begin Source File

SOURCE=.\huffman.h
# End Source File
# Begin Source File

SOURCE=.\l3types.h
# End Source File
# Begin Source File

SOURCE=.\mp3.h
# End Source File
# Begin Source File

SOURCE=..\DirectXLib\ogg.h
# End Source File
# Begin Source File

SOURCE=..\DirectXLib\os_types.h
# End Source File
# Begin Source File

SOURCE=.\reader.h
# End Source File
# Begin Source File

SOURCE=.\soundbuf.h
# End Source File
# Begin Source File

SOURCE=..\DirectXLib\vorbisenc.h
# End Source File
# Begin Source File

SOURCE=..\DirectXLib\vorbisfile.h
# End Source File
# End Group
# End Target
# End Project
