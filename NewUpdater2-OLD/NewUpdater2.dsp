# Microsoft Developer Studio Project File - Name="NewUpdater2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NewUpdater2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NewUpdater2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NewUpdater2.mak" CFG="NewUpdater2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NewUpdater2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NewUpdater2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/NewUpdater2", SLKAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NewUpdater2 - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O1 /I "..\NewPatchManager" /I "..\client" /I "..\client\SpriteLib" /I "..\client\WinLib" /I "..\updater" /I "Packet" /I "DSetup" /I "SPKFileLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "__WINDOWS__" /D "__UPDATE_CLIENT__" /D "__WIN32__" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Shlwapi.lib wininet.lib msimg32.lib Ws2_32.lib WinMM.lib /nologo /subsystem:windows /map /machine:I386 /force /out:"c:\program files\netmarble\netmarbledarkeden\updater.exe"
# SUBTRACT LINK32 /incremental:yes /nodefaultlib

!ELSEIF  "$(CFG)" == "NewUpdater2 - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\client" /I "..\client\SpriteLib" /I "..\client\WinLib" /I "..\updater" /I "Packet" /I "DSetup" /I "SPKFileLib" /D "__DEBUG_OUTPUT__" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__WINDOWS__" /D "__UPDATE_CLIENT__" /D "__WIN32__" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Shlwapi.lib wininet.lib msimg32.lib Ws2_32.lib WinMM.lib /nologo /subsystem:windows /pdb:"Debug/darkeden.pdb" /debug /machine:I386 /force /out:"c:\program files\netmarble\netmarbledarkeden\updater.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "NewUpdater2 - Win32 Release"
# Name "NewUpdater2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Packet source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Packet\Assert.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\CUBeginUpdate.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\CUBeginUpdateHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\CUEndUpdate.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\CUEndUpdateHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\CURequest.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\CURequestHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\FileAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\GameTime.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Properties.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Resource.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\ResourceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Socket.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketInputStream.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\StringStream.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\UCUpdate.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\UCUpdateHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\UCUpdateList.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\UCUpdateListHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\Update.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\UpdateClient.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\UpdateClientPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet\UpdateManager.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\client\SpriteLib\CFileIndexTable.cpp
# End Source File
# Begin Source File

SOURCE=..\client\CMessageArray.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MInternetConnection.cpp
# End Source File
# Begin Source File

SOURCE=..\client\MString.cpp
# End Source File
# Begin Source File

SOURCE=.\NewUpdater2.cpp
# End Source File
# Begin Source File

SOURCE=.\NewUpdater2.rc
# End Source File
# Begin Source File

SOURCE=.\NewUpdater2Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressST.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UpdateUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Packet header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Packet\Assert.h
# End Source File
# Begin Source File

SOURCE=.\Packet\CUBeginUpdate.h
# End Source File
# Begin Source File

SOURCE=.\Packet\CUEndUpdate.h
# End Source File
# Begin Source File

SOURCE=.\Packet\CURequest.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Exception.h
# End Source File
# Begin Source File

SOURCE=.\Packet\FileAPI.h
# End Source File
# Begin Source File

SOURCE=.\Packet\GameTime.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Packet.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PacketFactory.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Player.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PlayerInfo.h
# End Source File
# Begin Source File

SOURCE=.\Packet\PlayerStatus.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Properties.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Packet\ResourceManager.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Socket.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketAPI.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketImpl.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketInputStream.h
# End Source File
# Begin Source File

SOURCE=.\Packet\SocketOutputStream.h
# End Source File
# Begin Source File

SOURCE=.\Packet\StringStream.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Types.h
# End Source File
# Begin Source File

SOURCE=.\Packet\UCUpdate.h
# End Source File
# Begin Source File

SOURCE=.\Packet\UCUpdateList.h
# End Source File
# Begin Source File

SOURCE=.\Packet\Update.h
# End Source File
# Begin Source File

SOURCE=.\Packet\UpdateClient.h
# End Source File
# Begin Source File

SOURCE=.\Packet\UpdateClientPlayer.h
# End Source File
# Begin Source File

SOURCE=.\Packet\UpdateDef.h
# End Source File
# Begin Source File

SOURCE=.\Packet\UpdateManager.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\client\SpriteLib\CFileIndexTable.h
# End Source File
# Begin Source File

SOURCE=..\client\CMessageArray.h
# End Source File
# Begin Source File

SOURCE=.\DebugInfo.h
# End Source File
# Begin Source File

SOURCE=..\client\MInternetConnection.h
# End Source File
# Begin Source File

SOURCE=.\NewUpdater2.h
# End Source File
# Begin Source File

SOURCE=.\NewUpdater2Dlg.h
# End Source File
# Begin Source File

SOURCE=.\PatchInfo.h
# End Source File
# Begin Source File

SOURCE=.\ProgressST.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Updater.h
# End Source File
# Begin Source File

SOURCE=.\UpdateUtility.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Resource\bar.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\bar_file.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\bar_file2.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\bar_total.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\bran_img_on.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\bran_off.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\bran_on.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\D0.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\D0D.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\D0F.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\D0H.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\D0P.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\D0U.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\D1.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\D1D.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\D1F.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\D1H.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\D1P.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\D1U.bmp
# End Source File
# Begin Source File

SOURCE=.\darkeden8.ico
# End Source File
# Begin Source File

SOURCE=.\Resource\Exit.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\exit_img_off.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\exit_off.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\exit_on.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\EXITD.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\EXITF.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\ExitHilighted.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\ExitPush.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\EXITU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NewUpdater2.ico
# End Source File
# Begin Source File

SOURCE=.\res\NewUpdater2.rc2
# End Source File
# Begin Source File

SOURCE=.\Resource\peles_img_on.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\peles_off.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\peles_on.bmp
# End Source File
# Begin Source File

SOURCE=.\progress.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\Progress.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\SKULL.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\SKULL1.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\start_img_off.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\start_img_on.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\start_off.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\start_on.bmp
# End Source File
# Begin Source File

SOURCE=".\Resource\Window copy.bmp"
# End Source File
# Begin Source File

SOURCE=.\Resource\Window.bmp
# End Source File
# Begin Source File

SOURCE=".\Resource\까치\Window.bmp"
# End Source File
# Begin Source File

SOURCE=.\Window.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\window1.bmp
# End Source File
# Begin Source File

SOURCE=".\업데이트.bmp"
# End Source File
# Begin Source File

SOURCE=".\Resource\업데이트창.bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section NewUpdater2 : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
# Section NewUpdater2 : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
