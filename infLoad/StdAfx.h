// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma warning(disable:4786)

#if !defined(AFX_STDAFX_H__2A576DAD_1DD0_4E99_A834_E69A927471B7__INCLUDED_)
#define AFX_STDAFX_H__2A576DAD_1DD0_4E99_A834_E69A927471B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxtempl.h>

#include <iostream.h>
#include <fstream.h>

#include "CTypeTable.h"

#include <string>
#include <assert.h>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <bitset>
#include <algorithm>
#include <windows.h>
#include <MMSystem.h>
#include <Digitalv.h>
#include <DDraw.h>
#include <stdio.h>
#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include "GenData.h"
#include "ToActionInfo.h"
#include "MySkillInfo.h"

#include "../Client/MEffectSpriteTypeTable.h"
#include "../Client/MEffectStatusTable.h"
#include "../Client/MSkillManager.h"
#include "../Client/MActionInfoTable.h"
//#include "../Client/MCreatureTable.h"
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////
//∫Í«¯”Ú
#define FILE_EST "D:\\ttdk2\\data\\info\\EffectSpriteType.inf"
#define FILE_ES "D:\\ttdk2\\data\\info\\EffectStatus.inf"
#define FILE_ST "D:\\ttdk2\\data\\info\\skill.inf"
#define FILE_AC "D:\\ttdk2\\data\\info\\Action.inf"
#define FILE_SI "D:\\ttdk2\\data\\info\\skillinfo.inf"
#define FILE_CREATURE "D:\\ttdk2\\data\\info\\creature.inf"
#define FILE_CREATURESPRITE "D:\\ttdk2\\data\\info\\creaturesprite.inf"

bool FileOpenBinary(const char* filename, class ifstream& file);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2A576DAD_1DD0_4E99_A834_E69A927471B7__INCLUDED_)
