# Microsoft Developer Studio Project File - Name="NSStarter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NSStarter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NSStarter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NSStarter.mak" CFG="NSStarter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NSStarter - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NSStarter - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NSStarter - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 wininet.lib /nologo /subsystem:windows /incremental:yes /map:"bin/MFStarter_release.map" /machine:I386 /out:"d:\ttdk2\updater.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "NSStarter - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wininet.lib /nologo /subsystem:windows /debug /machine:I386 /out:"d:\ttdk2\updater_d.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "NSStarter - Win32 Release"
# Name "NSStarter - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Module\Binstr.cpp
# End Source File
# Begin Source File

SOURCE=.\BMyAPIFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Module\cCRC32.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckLocalFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CountRect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOption.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSet.cpp
# End Source File
# Begin Source File

SOURCE=.\GAreaConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\GAreaList.cpp
# End Source File
# Begin Source File

SOURCE=.\GenericHTTPClient.cpp
# End Source File
# Begin Source File

SOURCE=.\GFtpProcessor.cpp
# End Source File
# Begin Source File

SOURCE=.\GGameCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\GHttpProcessor.cpp
# End Source File
# Begin Source File

SOURCE=.\GMyPing.cpp
# End Source File
# Begin Source File

SOURCE=.\GServerList.cpp
# End Source File
# Begin Source File

SOURCE=.\MD5.h
# End Source File
# Begin Source File

SOURCE=.\MyListBox1.cpp
# End Source File
# Begin Source File

SOURCE=.\MyListBoxCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MyListButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MyLogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MyOffenUseFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\MySocket.cpp
# End Source File
# Begin Source File

SOURCE=.\MyStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\NSStarter.cpp
# End Source File
# Begin Source File

SOURCE=.\NSStarter.rc
# End Source File
# Begin Source File

SOURCE=.\NSStarterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadData.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Client\ServerInfoFileParser.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\Client\ServerInfoFileParser.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Module\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=.\Module\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=.\Module\tinyxmlparser.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.cpp
# End Source File
# Begin Source File

SOURCE=.\Module\XUnzip.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Module\Binstr.h
# End Source File
# Begin Source File

SOURCE=.\Module\Binstr.inl
# End Source File
# Begin Source File

SOURCE=.\BMyAPIFile.h
# End Source File
# Begin Source File

SOURCE=.\Module\cCRC32.h
# End Source File
# Begin Source File

SOURCE=.\CheckLocalFile.h
# End Source File
# Begin Source File

SOURCE=.\CountRect.h
# End Source File
# Begin Source File

SOURCE=.\DlgOption.h
# End Source File
# Begin Source File

SOURCE=.\DlgSet.h
# End Source File
# Begin Source File

SOURCE=.\GAreaConfig.h
# End Source File
# Begin Source File

SOURCE=.\GAreaList.h
# End Source File
# Begin Source File

SOURCE=.\GenericHTTPClient.h
# End Source File
# Begin Source File

SOURCE=.\GFtpProcessor.h
# End Source File
# Begin Source File

SOURCE=.\GGameCheck.h
# End Source File
# Begin Source File

SOURCE=.\GHttpProcessor.h
# End Source File
# Begin Source File

SOURCE=.\GMyPing.h
# End Source File
# Begin Source File

SOURCE=.\GServerList.h
# End Source File
# Begin Source File

SOURCE=.\MyListBox1.h
# End Source File
# Begin Source File

SOURCE=.\MyListBoxCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MyListButton.h
# End Source File
# Begin Source File

SOURCE=.\MyLogFile.h
# End Source File
# Begin Source File

SOURCE=.\MyOffenUseFunction.h
# End Source File
# Begin Source File

SOURCE=.\MySocket.h
# End Source File
# Begin Source File

SOURCE=.\MyStatic.h
# End Source File
# Begin Source File

SOURCE=.\NSStarter.h
# End Source File
# Begin Source File

SOURCE=.\NSStarterDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReadData.h
# End Source File
# Begin Source File

SOURCE=.\Registry.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Module\tinystr.h
# End Source File
# Begin Source File

SOURCE=.\Module\tinyxml.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.h
# End Source File
# Begin Source File

SOURCE=.\Module\XUnzip.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\128.ico
# End Source File
# Begin Source File

SOURCE=.\bin\Data\Update\image\list1.bmp
# End Source File
# Begin Source File

SOURCE=.\bin\Data\Update\image\list2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo_TPE.bmp
# End Source File
# Begin Source File

SOURCE=.\bin\cn\BG\luncher.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mf.ico
# End Source File
# Begin Source File

SOURCE=.\res\NSStarter.ico
# End Source File
# Begin Source File

SOURCE=.\res\NSStarter.rc2
# End Source File
# Begin Source File

SOURCE=.\NSStarter.rgs
# End Source File
# Begin Source File

SOURCE=".\res\PangYa Update.ico"
# End Source File
# Begin Source File

SOURCE=.\bin\cn\BG\Serverlist_BG.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=..\Path\Path\Check.dsw
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section NSStarter : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
# Section NSStarter : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
