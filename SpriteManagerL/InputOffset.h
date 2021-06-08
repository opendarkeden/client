#if !defined(AFX_INPUTOFFSET_H__A80119C1_B753_11D4_AC05_0050DABAC3D4__INCLUDED_)
#define AFX_INPUTOFFSET_H__A80119C1_B753_11D4_AC05_0050DABAC3D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputOffset.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputOffset dialog

class CInputOffset : public CDialog
{
// Construction
public:
	CInputOffset(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputOffset)
	enum { IDD = IDD_DIALOG1 };
	int		m_x;
	int		m_y;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputOffset)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputOffset)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTOFFSET_H__A80119C1_B753_11D4_AC05_0050DABAC3D4__INCLUDED_)
