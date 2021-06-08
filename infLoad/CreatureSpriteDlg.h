#if !defined(AFX_CREATURESPRITEDLG_H__86293CF2_CC12_49FD_8B68_C50DB12AC6D7__INCLUDED_)
#define AFX_CREATURESPRITEDLG_H__86293CF2_CC12_49FD_8B68_C50DB12AC6D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreatureSpriteDlg.h : header file
//
#include "ListCtrlEx.h"
/////////////////////////////////////////////////////////////////////////////
// CreatureSpriteDlg dialog

class CreatureSpriteDlg : public CDialog
{
// Construction
public:
	CreatureSpriteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CreatureSpriteDlg)
	enum { IDD = IDD_DIALOG_CREATURESPRITE };
	CListCtrlEx m_InfoList;
	CListBox	m_list;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CreatureSpriteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CreatureSpriteDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonRead();
	afx_msg void OnSelchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void DrawAll();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATURESPRITEDLG_H__86293CF2_CC12_49FD_8B68_C50DB12AC6D7__INCLUDED_)
