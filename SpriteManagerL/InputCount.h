#if !defined(AFX_INPUTCOUNT_H__7A830501_B5B2_11D4_AC05_0050DABAC3D4__INCLUDED_)
#define AFX_INPUTCOUNT_H__7A830501_B5B2_11D4_AC05_0050DABAC3D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputCount.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputCount dialog

class CInputCount : public CDialog
{
// Construction
public:
	CInputCount(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputCount)
	enum { IDD = IDD_DIALOG_INPUT_COUNT };
	UINT	m_dwCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputCount)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputCount)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTCOUNT_H__7A830501_B5B2_11D4_AC05_0050DABAC3D4__INCLUDED_)
