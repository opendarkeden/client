#if !defined(AFX_MAKESPRDLG_H__B7111641_5326_11D4_AC04_0050DABAC3D4__INCLUDED_)
#define AFX_MAKESPRDLG_H__B7111641_5326_11D4_AC04_0050DABAC3D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MakeSprDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMakeSprDlg dialog

class CMakeSprDlg : public CDialog
{
// Construction
public:
	CMakeSprDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMakeSprDlg)
	enum { IDD = IDD_DIALOG_MAKE_SPRITE };
	BOOL	m_bSlayerMan;
	BOOL	m_bSlayerWoman;
	BOOL	m_bNPC;
	CString	m_npcName;
	BOOL	m_bMidLevelMob;
	CString	m_midLevelMobName;
	BOOL	m_bVampireMan;
	BOOL	m_bVampireWoman;
	BOOL	m_bOusters;
	BOOL	m_bPromotionOusters;
	BOOL	m_bPromotionSlayerMan;
	BOOL	m_bPromotionSlayerWoman;
	BOOL	m_bPromotionVampireMan;
	BOOL	m_bPromotionVampireWoman;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMakeSprDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMakeSprDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnButtonOpenFileForMidMob();
	afx_msg void OnButtonOpenFileForNpc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKESPRDLG_H__B7111641_5326_11D4_AC04_0050DABAC3D4__INCLUDED_)
