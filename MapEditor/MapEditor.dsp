# Microsoft Developer Studio Project File - Name="MapEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MapEditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MapEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MapEditor.mak" CFG="MapEditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MapEditor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MapEditor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MapEditor", HUDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MapEditor - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "\game(800)\designed\project\client\spritelib" /I "include" /I "include\interface" /I "..\UI\include" /I "." /I ".." /I "..\MapSoundManager" /I "\game(800)\designed\project\lib_ole" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /i "d:\designed\project\mapeditor\include" /i "d:\designed\project\mapeditor\include\interface" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 \designed\project\client\spritelib\RELEASE\sp.lib tree.obj BasicS.lib dx_RELEASE.lib dsound.lib fr.lib ui_RELEASE.lib gl.lib dinput.lib timer.lib ps.lib FL.lib dxguid.lib ddraw.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /incremental:yes /debug /machine:I386 /out:"\∏ æ∆¿Ã≈€\∏ ø°µ≈ÕE\MapEditor.exe" /libpath:"\designed\lib" /libpath:"\designed\project\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MapEditor - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "\designed\project\client\spritelib" /I "include" /I "include\interface" /I "..\UI\include" /I "." /I ".." /I "..\MapSoundManager" /I "\designed\project\lib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_MAP_EDITOR" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 \designed\project\client\spritelib\debug\sp.lib tree.obj BasicS.lib dx_DEBUG.lib dsound.lib fr.lib ui_DEBUG.lib gl.lib sp_debug.lib dinput.lib timer.lib ps.lib FL.lib dxguid.lib ddraw.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"libcmt.lib" /out:"\∏ æ∆¿Ã≈€\∏ ø°µ≈ÕE\MapEditorD.exe" /pdbtype:sept /libpath:"\designed\lib" /libpath:"\designed\project\lib"
# SUBTRACT LINK32 /pdb:none /incremental:no /map

!ENDIF 

# Begin Target

# Name "MapEditor - Win32 Release"
# Name "MapEditor - Win32 Debug"
# Begin Group "include"

# PROP Default_Filter "*.h"
# Begin Group "interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\interface\CPositionList.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\CTypeTable.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MAnimationObject.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MFileDef.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MImageObject.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MInteractionObject.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MInteractionObjectTable.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MObject.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MPortal.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MSectorEdit.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MSectorInfo.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MShadowAnimationObject.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MString.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MTopViewEdit.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MTypeDef.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MViewDef.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MZoneDef.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MZoneEdit.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\ZoneFileBody.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\ZoneFileHeader.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\include\AttachObject.h
# End Source File
# Begin Source File

SOURCE=.\include\AttachTile.h
# End Source File
# Begin Source File

SOURCE=.\include\Help.h
# End Source File
# Begin Source File

SOURCE=.\include\mapEditor.h
# End Source File
# Begin Source File

SOURCE=.\include\MapView.h
# End Source File
# Begin Source File

SOURCE=.\include\ME_define.h
# End Source File
# Begin Source File

SOURCE=.\include\ME_exec_id.h
# End Source File
# Begin Source File

SOURCE=.\include\ME_string.h
# End Source File
# Begin Source File

SOURCE=.\include\minimap.h
# End Source File
# Begin Source File

SOURCE=.\include\interface\MShadowObject.h
# End Source File
# Begin Source File

SOURCE=.\OBJ.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\SEQUENCE.H
# End Source File
# Begin Source File

SOURCE=..\Se\include\SpritePackUtil.h
# End Source File
# Begin Source File

SOURCE=.\include\TileGroupViewer.h
# End Source File
# End Group
# Begin Group "src"

# PROP Default_Filter "*.cpp"
# Begin Group "interface No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\interface\MAnimationObject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\MImageObject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\MInteractionObject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\MInteractionObjectTable.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\MObject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\MPortal.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\MSectorEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\MSectorInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\MShadowAnimationObject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\MString.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\MTopViewEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\MZoneEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\ZoneFileBody.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\ZoneFileHeader.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\AttachObject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AttachTile.cpp
# End Source File
# Begin Source File

SOURCE=.\filetype.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Help.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mapEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MapView.cpp
# End Source File
# Begin Source File

SOURCE=.\src\minimap.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface\MShadowObject.cpp
# End Source File
# Begin Source File

SOURCE=.\obj.cpp
# End Source File
# Begin Source File

SOURCE=.\SpritePackUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TileGroupViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\WinMain.cpp
# End Source File
# End Group
# Begin Group "ex"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CPartManager.h
# End Source File
# Begin Source File

SOURCE=.\CShadowSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\CShadowSprite.h
# End Source File
# Begin Source File

SOURCE=.\CShadowSpritePack.cpp
# End Source File
# Begin Source File

SOURCE=.\CShadowSpritePack.h
# End Source File
# Begin Source File

SOURCE=.\CSoundPartManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CSoundPartManager.h
# End Source File
# Begin Source File

SOURCE=.\CSpritePack.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpritePack.h
# End Source File
# Begin Source File

SOURCE=.\CSpritePackList.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpritePackList.h
# End Source File
# Begin Source File

SOURCE=.\CSpritePackList555.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpritePackList555.h
# End Source File
# Begin Source File

SOURCE=.\CSpritePackList565.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpritePackList565.h
# End Source File
# Begin Source File

SOURCE=.\CTypeMap.h
# End Source File
# Begin Source File

SOURCE=.\MSoundTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MSoundTable.h
# End Source File
# Begin Source File

SOURCE=.\MTypeDef.h
# End Source File
# Begin Source File

SOURCE=.\MZoneSound.cpp
# End Source File
# Begin Source File

SOURCE=.\MZoneSound.h
# End Source File
# Begin Source File

SOURCE=.\MZoneSoundManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MZoneSoundManager.h
# End Source File
# Begin Source File

SOURCE=.\SectorSoundInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SectorSoundInfo.h
# End Source File
# Begin Source File

SOURCE=.\ShowTimeChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowTimeChecker.h
# End Source File
# Begin Source File

SOURCE=.\SoundDef.h
# End Source File
# Begin Source File

SOURCE=.\ZoneSoundInfoEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ZoneSoundInfoEdit.h
# End Source File
# End Group
# End Target
# End Project
