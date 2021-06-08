#if !defined(AFX_SKILLDLG_H__4E0B945B_3D6D_4B75_A8A4_744D7B6EFEF0__INCLUDED_)
#define AFX_SKILLDLG_H__4E0B945B_3D6D_4B75_A8A4_744D7B6EFEF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkillDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkillDlg dialog

class CSkillDlg : public CDialog
{
// Construction
public:
	void DrawAll();
	CSkillDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSkillDlg)
	enum { IDD = IDD_SKILL_OLE };
	CListBox	m_NextList;
	CListBox	m_list;
	CComboBox	m_ELEMENTAL_DOMAIN;
	int		m_level;
	CString	m_Name;
	int		m_x;
	BOOL	m_passive;
	BOOL	m_bActive;
	CString	m_HName;
	int		m_mp;
	int		m_y;
	CString	m_Findit;
	int		m_spriteid;
	BOOL	m_ismod;
	UINT	m_NextNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSkillDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRead();
	afx_msg void OnSelchangeList1();
	afx_msg void OnDestroy();
	afx_msg void OnCheck1();
	afx_msg void OnSelchangeList2();
	afx_msg void OnSave();
	afx_msg void OnMod();
	afx_msg void OnAddinfo();
	afx_msg void OnDelete();
	afx_msg void OnAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLDLG_H__4E0B945B_3D6D_4B75_A8A4_744D7B6EFEF0__INCLUDED_)
