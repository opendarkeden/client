# Microsoft Developer Studio Project File - Name="Patcher" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Patcher - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Patcher.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Patcher.mak" CFG="Patcher - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Patcher - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Patcher - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Patcher", OQKAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Patcher - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O1 /I "..\NewPatchManager" /I "..\client" /I "..\client\mzlib" /I "..\client\updater" /I "..\client\spritelib" /I "..\client\packet" /I "..\client\dxlib" /D "__WINDOWS__" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\client\mzlib\release\ZLib.lib Comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../client/jpegLib\jpeg.lib ../client/dxlib/release/dx.lib DDraw.lib DXGUID.lib ../client/spritelib/release/sp.lib /nologo /subsystem:windows /machine:I386 /out:"c:\darkeden\darkedentest\PatcherRelease.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Patcher - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\NewPatchManager" /I "..\client" /I "..\client\mzlib" /I "..\client\updater" /I "..\client\packet" /I "..\client\spritelib" /I "..\client\dxlib" /D "__WINDOWS__" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ZLib.lib Comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../client/jpegLib\jpeg.lib ../client/DirectXLib/debug/dx.lib DDraw.lib DXGUID.lib ../client/spritelib/debug/sp.lib /nologo /subsystem:windows /incremental:no /map /debug /machine:I386 /out:"bin\PatcherDebug.exe" /pdbtype:sept /libpath:"..\client\mzlib\debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Patcher - Win32 Release"
# Name "Patcher - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Client\AppendPatchInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\Client\SpriteLib\CFileIndexTable.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MGuildInfoMapper.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MGuildMarkManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Client\MString.cpp
# End Source File
# Begin Source File

SOURCE=..\Client\MZLib\MZLib.cpp
# End Source File
# Begin Source File

SOURCE=..\client\Packet\Properties.cpp
# End Source File
# Begin Source File

SOURCE=.\Resource.rc
# End Source File
# Begin Source File

SOURCE=..\client\Packet\StringStream.cpp
# End Source File
# Begin Source File

SOURCE=..\Client\Updater\Update.cpp
# End Source File
# Begin Source File

SOURCE=..\Client\Updater\UpdateManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Client\Updater\UpdateUtility.cpp
# End Source File
# Begin Source File

SOURCE=..\client\UtilityFunction.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Client\AppendPatchInfo.h
# End Source File
# Begin Source File

SOURCE=..\Client\SpriteLib\CFileIndexTable.h
# End Source File
# Begin Source File

SOURCE=..\client\MGuildInfoMapper.h
# End Source File
# Begin Source File

SOURCE=..\client\MGuildMarkManager.h
# End Source File
# Begin Source File

SOURCE=..\Client\MString.h
# End Source File
# Begin Source File

SOURCE=..\Client\MZLib\MZlib.h
# End Source File
# Begin Source File

SOURCE=..\client\Packet\Properties.h
# End Source File
# Begin Source File

SOURCE=..\client\Packet\StringStream.h
# End Source File
# Begin Source File

SOURCE=..\Client\Updater\Update.h
# End Source File
# Begin Source File

SOURCE=..\Client\Updater\UpdateDef.h
# End Source File
# Begin Source File

SOURCE=..\Client\Updater\UpdateManager.h
# End Source File
# Begin Source File

SOURCE=..\Client\Updater\UpdateUtility.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\darkeden8.ico
# End Source File
# Begin Source File

SOURCE=.\icon2.ico
# End Source File
# End Group
# End Target
# End Project
