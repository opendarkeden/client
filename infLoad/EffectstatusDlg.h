#if !defined(AFX_EFFECTSTATUSDLG_H__37AFEBB3_CD89_45D3_9C16_8B5CFADA5D97__INCLUDED_)
#define AFX_EFFECTSTATUSDLG_H__37AFEBB3_CD89_45D3_9C16_8B5CFADA5D97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EffectstatusDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EffectstatusDlg dialog

class EffectstatusDlg : public CDialog
{
// Construction
public:
	void Draw(int n);
	void DrawAll();
	void Init();
	EffectstatusDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EffectstatusDlg)
	enum { IDD = IDD_OLE_PAGE03 };
	CComboBox	m_compart;
	CListBox	m_list;
	BOOL	m_bUseEffectSprite;
	BOOL	m_bAttachGround;
	UINT	m_EffectSpriteType;
	UINT	m_EffectColor;
	UINT	m_ActionInfo;
	UINT	m_OriginalActionInfo;
	int		m_SoundID;
	int		m_FindIt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EffectstatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EffectstatusDlg)
	afx_msg void OnRead();
	afx_msg void OnSelchangeList1();
	afx_msg void OnChangeEditFind();
	afx_msg void OnDestroy();
	afx_msg void OnWrite();
	afx_msg void OnMod();
	afx_msg void OnAddinfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFFECTSTATUSDLG_H__37AFEBB3_CD89_45D3_9C16_8B5CFADA5D97__INCLUDED_)
