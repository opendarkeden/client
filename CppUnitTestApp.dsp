# Microsoft Developer Studio Project File - Name="CppUnitTestApp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CppUnitTestApp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CppUnitTestApp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CppUnitTestApp.mak" CFG="CppUnitTestApp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CppUnitTestApp - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CppUnitTestApp - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/VS_UI/CppUnitTestApp", PKKAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CppUnitTestApp - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../../include" /I "../.." /I "suites" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "CPPUNIT_USE_TYPEINFO" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ../../../Lib/cppunit.lib ../../../Lib/testrunner.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "CppUnitTestApp - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../../../include" /I "../.." /I "suites" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "CPPUNIT_USE_TYPEINFO" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../../../Lib/cppunitd.lib ../../../Lib/testrunnerd.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CppUnitTestApp - Win32 Release"
# Name "CppUnitTestApp - Win32 Debug"
# Begin Group "GUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CppUnitTestApp.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUnitTestApp.h
# End Source File
# Begin Source File

SOURCE=.\CppUnitTestApp.rc
# End Source File
# Begin Source File

SOURCE=.\CppUnitTestAppDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUnitTestAppDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CppUnitTestApp.ico
# End Source File
# Begin Source File

SOURCE=.\res\CppUnitTestApp.rc2
# End Source File
# End Group
# Begin Group "DLL Dependencies"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\lib\testrunner.dll

!IF  "$(CFG)" == "CppUnitTestApp - Win32 Release"

# Begin Custom Build - $(IntDir)\$(InputName).dll
IntDir=.\Release
InputPath=..\..\..\lib\testrunner.dll
InputName=testrunner

"$(IntDir)\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(IntDir)\$(InputName).dll

# End Custom Build

!ELSEIF  "$(CFG)" == "CppUnitTestApp - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\lib\testrunnercd.dll
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\testrunnerd.dll

!IF  "$(CFG)" == "CppUnitTestApp - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CppUnitTestApp - Win32 Debug"

# Begin Custom Build - $(IntDir)\$(InputName).dll
IntDir=.\Debug
InputPath=..\..\..\lib\testrunnerd.dll
InputName=testrunnerd

"$(IntDir)\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(IntDir)\$(InputName).dll

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\lib\TestRunnerDSPlugIn.dll
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\TestRunnerDSPlugInD.dll
# End Source File
# End Group
# Begin Group "Tests"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Tests\ExampleTest.cpp
# End Source File
# Begin Source File

SOURCE=.\Tests\ExampleTest.h
# End Source File
# End Group
# Begin Group "Suites"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Suites\CoreSuite.h
# End Source File
# Begin Source File

SOURCE=.\Suites\CppUnitTestSuite.cpp
# End Source File
# Begin Source File

SOURCE=.\Suites\CppUnitTestSuite.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\cppunittest\Makefile.am
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
