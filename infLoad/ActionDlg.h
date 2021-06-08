#if !defined(AFX_ACTIONDLG_H__51D2146A_C457_48DA_808A_B8E3DEB49BE8__INCLUDED_)
#define AFX_ACTIONDLG_H__51D2146A_C457_48DA_808A_B8E3DEB49BE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionDlg.h : header file
//
#include "ListCtrlEx.h"
/////////////////////////////////////////////////////////////////////////////
// CActionDlg dialog

class CActionDlg : public CDialog
{
// Construction
public:
	void DrawAll();
	CActionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionDlg)
	enum { IDD = IDD_ACTION_OLE };
	CComboBox	m_effsta;
	CComboBox	m_apackcombo;
	CListCtrlEx	m_InfoList;
	CListBox	m_list;
	short   Index;
	int sumnum;
	UINT	m_NODENUM;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionDlg)
	afx_msg void OnRead();
	afx_msg void OnSelchangeList1();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnWrite();
	afx_msg void OnAddnode();
	afx_msg void OnAddinfo();
	afx_msg void OnMod();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONDLG_H__51D2146A_C457_48DA_808A_B8E3DEB49BE8__INCLUDED_)
