// infLoad.h : main header file for the INFLOAD application
//

#if !defined(AFX_INFLOAD_H__13BCEA06_CBCE_4919_83D9_A311BF18DC40__INCLUDED_)
#define AFX_INFLOAD_H__13BCEA06_CBCE_4919_83D9_A311BF18DC40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CInfLoadApp:
// See infLoad.cpp for the implementation of this class
//

class CInfLoadApp : public CWinApp
{
public:
	CInfLoadApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfLoadApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CInfLoadApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFLOAD_H__13BCEA06_CBCE_4919_83D9_A311BF18DC40__INCLUDED_)
