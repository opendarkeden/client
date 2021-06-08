// infLoadDlg.h : header file
//

#if !defined(AFX_INFLOADDLG_H__F7EDADDB_38A0_4436_B3C0_BAC15B58101B__INCLUDED_)
#define AFX_INFLOADDLG_H__F7EDADDB_38A0_4436_B3C0_BAC15B58101B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "EffectSpriteTypeDlg.h"
#include "ActionEffectSpriteTypeDlg.h"
#include "EffectstatusDlg.h"
#include "SkillDlg.h"
#include "SkillinfoDlg.h"
#include "ActionDlg.h"
#include "CreatureDlg.h"
#include "CreatureSpriteDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CInfLoadDlg dialog


class CInfLoadDlg : public CDialog
{
// Construction
public:
	void InitFrom();
	CInfLoadDlg(CWnd* pParent = NULL);	// standard constructor


private:
	EffectSpriteTypeDlg m_page1;
	ActionEffectSpriteTypeDlg m_page2;
	EffectstatusDlg		m_page3;
	CSkillDlg m_page4;
	CSkillinfoDlg m_page5;
	CActionDlg m_page6;
	CreatureDlg m_page7;
	CreatureSpriteDlg m_page8;
// Dialog Data
	//{{AFX_DATA(CInfLoadDlg)
	enum { IDD = IDD_INFLOAD_DIALOG };
	CTabCtrl	m_FileTab;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfLoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CInfLoadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFLOADDLG_H__F7EDADDB_38A0_4436_B3C0_BAC15B58101B__INCLUDED_)
