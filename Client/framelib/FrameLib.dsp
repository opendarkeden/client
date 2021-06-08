# Microsoft Developer Studio Project File - Name="FrameLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=FrameLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FrameLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FrameLib.mak" CFG="FrameLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FrameLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "FrameLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Client/FrameLib", VWFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FrameLib - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O1 /I "..\SpriteLib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "__TEST_SUB_INVENTORY__" /Yu"Frame_PCH.h" /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\FrameLib\Release\FR.lib"

!ELSEIF  "$(CFG)" == "FrameLib - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\SpriteLib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "__TEST_SUB_INVENTORY__" /Yu"Frame_PCH.h" /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\FR.lib"

!ENDIF 

# Begin Target

# Name "FrameLib - Win32 Release"
# Name "FrameLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CAnimationFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CAnimationFrameSetManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CCreatureFrameSetManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CFramePack.cpp
# End Source File
# Begin Source File

SOURCE=.\CFrameSetManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Frame_PCH.cpp
# ADD CPP /Yc"Frame_PCH.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CAnimationFrame.h
# End Source File
# Begin Source File

SOURCE=.\CAnimationFrameSetManager.h
# End Source File
# Begin Source File

SOURCE=.\CCreatureFrameSetManager.h
# End Source File
# Begin Source File

SOURCE=.\CFrame.h
# End Source File
# Begin Source File

SOURCE=.\CFramePack.h
# End Source File
# Begin Source File

SOURCE=.\CFrameSet.h
# End Source File
# Begin Source File

SOURCE=.\CFrameSetManager.h
# End Source File
# Begin Source File

SOURCE=.\FR.h
# End Source File
# Begin Source File

SOURCE=.\Frame_PCH.h
# End Source File
# End Group
# End Target
# End Project
