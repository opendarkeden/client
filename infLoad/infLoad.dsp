# Microsoft Developer Studio Project File - Name="infLoad" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=infLoad - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "infLoad.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "infLoad.mak" CFG="infLoad - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "infLoad - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "infLoad - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "infLoad - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../client" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "infLoad - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "infLoad - Win32 Release"
# Name "infLoad - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionEffectSpriteTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreatureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreatureSpriteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EffectSpriteTypeDlg.cpp

!IF  "$(CFG)" == "infLoad - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "infLoad - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EffectstatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Client\ExpInfo.cpp

!IF  "$(CFG)" == "infLoad - Win32 Release"

!ELSEIF  "$(CFG)" == "infLoad - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GenData.cpp
# End Source File
# Begin Source File

SOURCE=.\infLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\infLoad.rc
# End Source File
# Begin Source File

SOURCE=.\infLoadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=..\Client\MActionInfoTable.cpp

!IF  "$(CFG)" == "infLoad - Win32 Release"

!ELSEIF  "$(CFG)" == "infLoad - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MCreatureSpriteTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MCreatureTable.cpp
# End Source File
# Begin Source File

SOURCE=..\Client\MEffectSpriteTypeTable.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Client\MEffectStatusTable.cpp

!IF  "$(CFG)" == "infLoad - Win32 Release"

!ELSEIF  "$(CFG)" == "infLoad - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Client\MSkillInfoTable.cpp

!IF  "$(CFG)" == "infLoad - Win32 Release"

!ELSEIF  "$(CFG)" == "infLoad - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Client\MSkillManager.cpp

!IF  "$(CFG)" == "infLoad - Win32 Release"

!ELSEIF  "$(CFG)" == "infLoad - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Client\MString.cpp

!IF  "$(CFG)" == "infLoad - Win32 Release"

!ELSEIF  "$(CFG)" == "infLoad - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MySkillInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\Client\SkillDef.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\SkillDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillinfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToActionInfo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionEffectSpriteTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreatureDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreatureSpriteDlg.h
# End Source File
# Begin Source File

SOURCE=.\CTypeTable.h
# End Source File
# Begin Source File

SOURCE=.\EffectSpriteTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\EffectstatusDlg.h
# End Source File
# Begin Source File

SOURCE=..\Client\ExpInfo.h
# End Source File
# Begin Source File

SOURCE=.\GenData.h
# End Source File
# Begin Source File

SOURCE=.\infLoad.h
# End Source File
# Begin Source File

SOURCE=.\infLoadDlg.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=..\Client\MActionInfoTable.h
# End Source File
# Begin Source File

SOURCE=.\MCreatureSpriteTable.h
# End Source File
# Begin Source File

SOURCE=.\MCreatureTable.h
# End Source File
# Begin Source File

SOURCE=..\Client\MEffectSpriteTypeTable.h
# End Source File
# Begin Source File

SOURCE=..\Client\MEffectStatusDef.h
# End Source File
# Begin Source File

SOURCE=..\Client\MEffectStatusTable.h
# End Source File
# Begin Source File

SOURCE=..\Client\MSkillManager.h
# End Source File
# Begin Source File

SOURCE=..\Client\MString.h
# End Source File
# Begin Source File

SOURCE=.\MySkillInfo.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\Client\SkillDef.h
# End Source File
# Begin Source File

SOURCE=.\SkillDlg.h
# End Source File
# Begin Source File

SOURCE=.\SkillinfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToActionInfo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\infLoad.ico
# End Source File
# Begin Source File

SOURCE=.\res\infLoad.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
