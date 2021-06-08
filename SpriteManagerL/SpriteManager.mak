# Microsoft Developer Studio Generated NMAKE File, Based on SpriteManager.dsp
!IF "$(CFG)" == ""
CFG=SpriteManager - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SpriteManager - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SpriteManager - Win32 Release" && "$(CFG)" != "SpriteManager - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SpriteManager.mak" CFG="SpriteManager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SpriteManager - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SpriteManager - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SpriteManager - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\SpriteManager.exe" "$(OUTDIR)\SpriteManager.pch" "$(OUTDIR)\SpriteManager.bsc"


CLEAN :
	-@erase "$(INTDIR)\AniDlg.obj"
	-@erase "$(INTDIR)\AniDlg.sbr"
	-@erase "$(INTDIR)\CDib.obj"
	-@erase "$(INTDIR)\CDib.sbr"
	-@erase "$(INTDIR)\ConvertProgress.obj"
	-@erase "$(INTDIR)\ConvertProgress.sbr"
	-@erase "$(INTDIR)\DlgSearchOption.obj"
	-@erase "$(INTDIR)\DlgSearchOption.sbr"
	-@erase "$(INTDIR)\InputCount.obj"
	-@erase "$(INTDIR)\InputCount.sbr"
	-@erase "$(INTDIR)\InputOffset.obj"
	-@erase "$(INTDIR)\InputOffset.sbr"
	-@erase "$(INTDIR)\MakeSprDlg.obj"
	-@erase "$(INTDIR)\MakeSprDlg.sbr"
	-@erase "$(INTDIR)\ProgressDlg.obj"
	-@erase "$(INTDIR)\ProgressDlg.sbr"
	-@erase "$(INTDIR)\smartcutting.obj"
	-@erase "$(INTDIR)\smartcutting.sbr"
	-@erase "$(INTDIR)\SpriteManager.obj"
	-@erase "$(INTDIR)\SpriteManager.pch"
	-@erase "$(INTDIR)\SpriteManager.res"
	-@erase "$(INTDIR)\SpriteManager.sbr"
	-@erase "$(INTDIR)\SpriteManagerDlg.obj"
	-@erase "$(INTDIR)\SpriteManagerDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SpriteManager.bsc"
	-@erase "$(OUTDIR)\SpriteManager.exe"
	-@erase "$(OUTDIR)\SpriteManager.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SpriteManager.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SpriteManager.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AniDlg.sbr" \
	"$(INTDIR)\CDib.sbr" \
	"$(INTDIR)\ConvertProgress.sbr" \
	"$(INTDIR)\DlgSearchOption.sbr" \
	"$(INTDIR)\InputCount.sbr" \
	"$(INTDIR)\InputOffset.sbr" \
	"$(INTDIR)\MakeSprDlg.sbr" \
	"$(INTDIR)\ProgressDlg.sbr" \
	"$(INTDIR)\smartcutting.sbr" \
	"$(INTDIR)\SpriteManager.sbr" \
	"$(INTDIR)\SpriteManagerDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\SpriteManager.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=..\lib\dx_spritemanager.lib ddraw.lib dxguid.lib ..\lib\gl2.lib ..\lib\fr.lib ..\lib\sp_release.lib winmm.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\SpriteManager.pdb" /machine:I386 /nodefaultlib:"libcmt.lib" /nodefaultlib:"libicmt.lib" /out:"$(OUTDIR)\SpriteManager.exe" /libpath:"..\lib" 
LINK32_OBJS= \
	"$(INTDIR)\AniDlg.obj" \
	"$(INTDIR)\CDib.obj" \
	"$(INTDIR)\ConvertProgress.obj" \
	"$(INTDIR)\DlgSearchOption.obj" \
	"$(INTDIR)\InputCount.obj" \
	"$(INTDIR)\InputOffset.obj" \
	"$(INTDIR)\MakeSprDlg.obj" \
	"$(INTDIR)\ProgressDlg.obj" \
	"$(INTDIR)\smartcutting.obj" \
	"$(INTDIR)\SpriteManager.obj" \
	"$(INTDIR)\SpriteManagerDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SpriteManager.res"

"$(OUTDIR)\SpriteManager.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SpriteManager - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\SpriteManager.exe" "$(OUTDIR)\SpriteManager.pch" "$(OUTDIR)\SpriteManager.bsc"


CLEAN :
	-@erase "$(INTDIR)\AniDlg.obj"
	-@erase "$(INTDIR)\AniDlg.sbr"
	-@erase "$(INTDIR)\CDib.obj"
	-@erase "$(INTDIR)\CDib.sbr"
	-@erase "$(INTDIR)\ConvertProgress.obj"
	-@erase "$(INTDIR)\ConvertProgress.sbr"
	-@erase "$(INTDIR)\DlgSearchOption.obj"
	-@erase "$(INTDIR)\DlgSearchOption.sbr"
	-@erase "$(INTDIR)\InputCount.obj"
	-@erase "$(INTDIR)\InputCount.sbr"
	-@erase "$(INTDIR)\InputOffset.obj"
	-@erase "$(INTDIR)\InputOffset.sbr"
	-@erase "$(INTDIR)\MakeSprDlg.obj"
	-@erase "$(INTDIR)\MakeSprDlg.sbr"
	-@erase "$(INTDIR)\ProgressDlg.obj"
	-@erase "$(INTDIR)\ProgressDlg.sbr"
	-@erase "$(INTDIR)\smartcutting.obj"
	-@erase "$(INTDIR)\smartcutting.sbr"
	-@erase "$(INTDIR)\SpriteManager.obj"
	-@erase "$(INTDIR)\SpriteManager.pch"
	-@erase "$(INTDIR)\SpriteManager.res"
	-@erase "$(INTDIR)\SpriteManager.sbr"
	-@erase "$(INTDIR)\SpriteManagerDlg.obj"
	-@erase "$(INTDIR)\SpriteManagerDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SpriteManager.bsc"
	-@erase "$(OUTDIR)\SpriteManager.exe"
	-@erase "$(OUTDIR)\SpriteManager.ilk"
	-@erase "$(OUTDIR)\SpriteManager.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\lib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fr"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SpriteManager.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SpriteManager.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AniDlg.sbr" \
	"$(INTDIR)\CDib.sbr" \
	"$(INTDIR)\ConvertProgress.sbr" \
	"$(INTDIR)\DlgSearchOption.sbr" \
	"$(INTDIR)\InputCount.sbr" \
	"$(INTDIR)\InputOffset.sbr" \
	"$(INTDIR)\MakeSprDlg.sbr" \
	"$(INTDIR)\ProgressDlg.sbr" \
	"$(INTDIR)\smartcutting.sbr" \
	"$(INTDIR)\SpriteManager.sbr" \
	"$(INTDIR)\SpriteManagerDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\SpriteManager.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=..\lib\sp_RELEASE.lib ..\lib\dx_spritemanager.lib ddraw.lib dxguid.lib ..\lib\gl2.lib ..\lib\fr.lib winmm.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\SpriteManager.pdb" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /nodefaultlib:"libcmtd.lib" /out:"$(OUTDIR)\SpriteManager.exe" /pdbtype:sept /libpath:"..\lib" 
LINK32_OBJS= \
	"$(INTDIR)\AniDlg.obj" \
	"$(INTDIR)\CDib.obj" \
	"$(INTDIR)\ConvertProgress.obj" \
	"$(INTDIR)\DlgSearchOption.obj" \
	"$(INTDIR)\InputCount.obj" \
	"$(INTDIR)\InputOffset.obj" \
	"$(INTDIR)\MakeSprDlg.obj" \
	"$(INTDIR)\ProgressDlg.obj" \
	"$(INTDIR)\smartcutting.obj" \
	"$(INTDIR)\SpriteManager.obj" \
	"$(INTDIR)\SpriteManagerDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SpriteManager.res"

"$(OUTDIR)\SpriteManager.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("SpriteManager.dep")
!INCLUDE "SpriteManager.dep"
!ELSE 
!MESSAGE Warning: cannot find "SpriteManager.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SpriteManager - Win32 Release" || "$(CFG)" == "SpriteManager - Win32 Debug"
SOURCE=.\AniDlg.cpp

"$(INTDIR)\AniDlg.obj"	"$(INTDIR)\AniDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\CDib.cpp

"$(INTDIR)\CDib.obj"	"$(INTDIR)\CDib.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ConvertProgress.cpp

"$(INTDIR)\ConvertProgress.obj"	"$(INTDIR)\ConvertProgress.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgSearchOption.cpp

"$(INTDIR)\DlgSearchOption.obj"	"$(INTDIR)\DlgSearchOption.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\InputCount.cpp

"$(INTDIR)\InputCount.obj"	"$(INTDIR)\InputCount.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\InputOffset.cpp

"$(INTDIR)\InputOffset.obj"	"$(INTDIR)\InputOffset.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MakeSprDlg.cpp

"$(INTDIR)\MakeSprDlg.obj"	"$(INTDIR)\MakeSprDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ProgressDlg.cpp

"$(INTDIR)\ProgressDlg.obj"	"$(INTDIR)\ProgressDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\smartcutting.cpp

"$(INTDIR)\smartcutting.obj"	"$(INTDIR)\smartcutting.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SpriteManager.cpp

"$(INTDIR)\SpriteManager.obj"	"$(INTDIR)\SpriteManager.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SpriteManager.rc

"$(INTDIR)\SpriteManager.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\SpriteManagerDlg.cpp

"$(INTDIR)\SpriteManagerDlg.obj"	"$(INTDIR)\SpriteManagerDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "SpriteManager - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\SpriteManager.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\SpriteManager.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SpriteManager - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\lib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\SpriteManager.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\SpriteManager.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

