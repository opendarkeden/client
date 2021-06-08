#if !defined(AFX_CREATUREDLG_H__8FB07BB9_EDEE_412E_AC5D_BBF95B709DF1__INCLUDED_)
#define AFX_CREATUREDLG_H__8FB07BB9_EDEE_412E_AC5D_BBF95B709DF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// reatureDlg.h : header file
//
#include "ListCtrlEx.h"
/////////////////////////////////////////////////////////////////////////////
// CreatureDlg dialog
class CreatureDlg : public CDialog
{
// Construction
public:
	CreatureDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CreatureDlg)
	enum { IDD = IDD_DIALOG_CREATURE };
	CListCtrlEx	m_InfoList;
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CreatureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CreatureDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog(); 
	afx_msg void OnButtonRead();
	afx_msg void OnSelchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void DrawAll();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REATUREDLG_H__8FB07BB9_EDEE_412E_AC5D_BBF95B709DF1__INCLUDED_)
