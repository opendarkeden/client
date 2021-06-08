#if !defined(AFX_CONVERTPROGRESS_H__A0A4BF65_57D4_4E9B_B098_50F915422D4C__INCLUDED_)
#define AFX_CONVERTPROGRESS_H__A0A4BF65_57D4_4E9B_B098_50F915422D4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConvertProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConvertProgress dialog

class CConvertProgress : public CDialog
{
// Construction
public:
	CConvertProgress(CWnd* pParent = NULL);   // standard constructor
	BOOL m_bCanceled;
	void SetProgress( int cur, int max);
	void SetText( CString& strMessage);
	

// Dialog Data
	//{{AFX_DATA(CConvertProgress)
	enum { IDD = IDD_DIALOG_PROGRESS1 };
	CProgressCtrl	m_progress;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConvertProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL



// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConvertProgress)
	afx_msg void OnProgress(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCancel2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONVERTPROGRESS_H__A0A4BF65_57D4_4E9B_B098_50F915422D4C__INCLUDED_)
