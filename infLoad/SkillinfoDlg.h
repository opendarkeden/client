#if !defined(AFX_SKILLINFODLG_H__78BDF262_3838_4F4F_B9CA_552278F6A3EB__INCLUDED_)
#define AFX_SKILLINFODLG_H__78BDF262_3838_4F4F_B9CA_552278F6A3EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkillinfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkillinfoDlg dialog
#include "ListCtrlEx.h"

class CSkillinfoDlg : public CDialog
{
// Construction
public:
	void DrawAll();
	CSkillinfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSkillinfoDlg)
	enum { IDD = IDD_SKILLINFO_OLE };
	CListBox	m_TypeList;
	CComboBox	m_Elist;
	CListBox	m_list;
	CString	m_Find;
	int		m_minDamage;
	int		m_maxDamage;
	int		m_minDelay;
	int		m_maxDelay;
	int		m_minDuration;
	int		m_maxDuration;
	int		m_mp;
	int		m_maxRange;
	int		m_maxExp;
	int		m_minRange;
	int		m_SkillPoint;
	int		m_LevelUpPoint;
	int		m_Water;
	int		m_Earth;
	int		m_Wind;
	int		m_Fire;
	int		m_sum;
	int		m_Wristlet;
	int		m_Stone1;
	int		m_Stone2;
	int		m_Stone3;
	int		m_EDInt;
	BOOL	m_CanDelete;
	CString	m_HName;
	CString	m_Name;
	BOOL	m_ismod;
	UINT	m_TypeNum;
	int		m_Stone4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillinfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSkillinfoDlg)
	afx_msg void OnSelchangeList1();
	afx_msg void OnRead();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck1();
	afx_msg void OnAdd();
	afx_msg void OnMod();
	afx_msg void OnAddinfo();
	afx_msg void OnWrite();
	afx_msg void OnDelete();
	afx_msg void OnSelchangeList2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLINFODLG_H__78BDF262_3838_4F4F_B9CA_552278F6A3EB__INCLUDED_)
