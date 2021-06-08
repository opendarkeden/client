#if !defined(AFX_DLGSEARCHOPTION_H__2B9B9461_535C_11D4_AC04_0050DABAC3D4__INCLUDED_)
#define AFX_DLGSEARCHOPTION_H__2B9B9461_535C_11D4_AC04_0050DABAC3D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSearchOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSearchOption dialog

class CDlgSearchOption : public CDialog
{
// Construction
public:
	CDlgSearchOption(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSearchOption)
	enum { IDD = IDD_DIALOG_SEARCH };
	BOOL	m_bUpdateAniData;
	BOOL	m_bUpdateImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSearchOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSearchOption)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSEARCHOPTION_H__2B9B9461_535C_11D4_AC04_0050DABAC3D4__INCLUDED_)
