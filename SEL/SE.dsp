# Microsoft Developer Studio Project File - Name="SE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SE.mak" CFG="SE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SE - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SE - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SEL", KIMAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SE - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O1 /I "..\UI\include" /I "include" /I "..\sprlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 \designed\project\sprlib\RELEASE\sp.lib DX.lib FR.lib GL.lib BasicS.lib dinput.lib timer.lib ps.lib ui_debug.lib ddraw.lib dxguid.lib FL.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /pdb:none /machine:I386 /out:"SE.exe" /libpath:"..\sprlib" /libpath:"\designed\lib"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "SE - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\UI\include" /I "include" /I "..\sprlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 \designed\project\sprlib\debug\sp.lib DX.lib FR.lib GL.lib BasicS.lib dinput.lib timer.lib ps.lib ui_debug.lib ddraw.lib dxguid.lib FL.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /out:"SE.exe" /pdbtype:sept /libpath:"..\sprlib" /libpath:"\designed\lib" /libpath:"\designed\project\MapeditorL"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "SE - Win32 Release"
# Name "SE - Win32 Debug"
# Begin Group "include"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\include\cutting.h
# End Source File
# Begin Source File

SOURCE=.\include\EffectManager.h
# End Source File
# Begin Source File

SOURCE=.\include\filepath.h
# End Source File
# Begin Source File

SOURCE=.\include\filetype.h
# End Source File
# Begin Source File

SOURCE=.\include\FramePack.h
# End Source File
# Begin Source File

SOURCE=.\include\open_file.h
# End Source File
# Begin Source File

SOURCE=.\include\ProcessFile.h
# End Source File
# Begin Source File

SOURCE=.\include\SaveFile.h
# End Source File
# Begin Source File

SOURCE=.\include\SE_animator.h
# End Source File
# Begin Source File

SOURCE=.\include\SE_define.h
# End Source File
# Begin Source File

SOURCE=.\include\SE_effect.h
# End Source File
# Begin Source File

SOURCE=.\include\SE_exec_id.h
# End Source File
# Begin Source File

SOURCE=.\include\SE_framepack.h
# End Source File
# Begin Source File

SOURCE=.\include\SE_strings.h
# End Source File
# Begin Source File

SOURCE=.\include\SE_WindowProc.h
# End Source File
# Begin Source File

SOURCE=.\include\spriteEditor.h
# End Source File
# Begin Source File

SOURCE=.\include\SpriteFrame.h
# End Source File
# Begin Source File

SOURCE=.\include\SpritePack.h
# End Source File
# Begin Source File

SOURCE=.\include\SpritePackUtil.h
# End Source File
# Begin Source File

SOURCE=.\include\Utility.h
# End Source File
# Begin Source File

SOURCE=.\include\window_id.h
# End Source File
# End Group
# Begin Group "src"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\src\cutting.cpp
# End Source File
# Begin Source File

SOURCE=.\src\EffectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\filetype.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FramePack.cpp
# End Source File
# Begin Source File

SOURCE=.\src\open_file.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ProcessFile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SaveFile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SE_animator.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SE_effect.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SE_framepack.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SE_WindowProc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\spriteEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SpriteFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SpritePack.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SpritePackUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Utility.cpp
# End Source File
# Begin Source File

SOURCE=.\src\window_id.cpp
# End Source File
# Begin Source File

SOURCE=.\src\WinMain.cpp
# End Source File
# End Group
# End Target
# End Project
