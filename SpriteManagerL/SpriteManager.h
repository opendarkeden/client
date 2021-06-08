// SpriteManager.h : main header file for the SPRITEMANAGER application
//

#if !defined(AFX_SPRITEMANAGER_H__90E8B5C6_45C9_11D4_AC04_0050DABAC3D4__INCLUDED_)
#define AFX_SPRITEMANAGER_H__90E8B5C6_45C9_11D4_AC04_0050DABAC3D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
 
/////////////////////////////////////////////////////////////////////////////
// CSpriteManagerApp:
// See SpriteManager.cpp for the implementation of this class
//

class CSpriteManagerApp : public CWinApp
{
public:
	CSpriteManagerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpriteManagerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSpriteManagerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPRITEMANAGER_H__90E8B5C6_45C9_11D4_AC04_0050DABAC3D4__INCLUDED_)
