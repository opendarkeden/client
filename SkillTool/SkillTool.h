// SkillTool.h : main header file for the SKILLTOOL application
//

#if !defined(AFX_SKILLTOOL_H__85887D74_0682_4D9E_B737_D62D35A468B3__INCLUDED_)
#define AFX_SKILLTOOL_H__85887D74_0682_4D9E_B737_D62D35A468B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSkillToolApp:
// See SkillTool.cpp for the implementation of this class
//

class CSkillToolApp : public CWinApp
{
public:
	CSkillToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSkillToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLTOOL_H__85887D74_0682_4D9E_B737_D62D35A468B3__INCLUDED_)
