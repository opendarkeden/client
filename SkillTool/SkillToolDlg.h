// SkillToolDlg.h : header file
//

#if !defined(AFX_SKILLTOOLDLG_H__19CD903E_6E71_476E_A30F_6FDA45A33EF3__INCLUDED_)
#define AFX_SKILLTOOLDLG_H__19CD903E_6E71_476E_A30F_6FDA45A33EF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////////////////////////////////////////////////
#include "module/Skillinfo.h"			//data/info/SkillInfo.inf
#include "CEffectSpriteTypeManager.h"	//data/info/EffectSpriteType.inf	
#include "EffectstatusManager.h"		//data/info/EffectStatus.inf
#include "ActionKrToDk2.h"
//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CSkillToolDlg dialog
class CSkillToolDlg : public CDialog
{
// Construction
public:
	void Release();
	void SkillInit();
	CSkillToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSkillToolDlg)
	enum { IDD = IDD_SKILLTOOL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSkillToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CSkillinfo*	m_SkillInfoManager;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLTOOLDLG_H__19CD903E_6E71_476E_A30F_6FDA45A33EF3__INCLUDED_)
