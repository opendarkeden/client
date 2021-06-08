#if !defined(AFX_ACTIONEFFECTSPRITETYPEDLG_H__F798022A_0D08_4CD5_822F_1CB5AA5C0926__INCLUDED_)
#define AFX_ACTIONEFFECTSPRITETYPEDLG_H__F798022A_0D08_4CD5_822F_1CB5AA5C0926__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionEffectSpriteTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ActionEffectSpriteTypeDlg dialog

class ActionEffectSpriteTypeDlg : public CDialog
{
// Construction
public:
	void DrawAll();
	char m_path[300];
	void Init();
	ActionEffectSpriteTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ActionEffectSpriteTypeDlg)
	enum { IDD = IDD_OLE_PAGE02 };
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ActionEffectSpriteTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ActionEffectSpriteTypeDlg)
	afx_msg void OnRead();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONEFFECTSPRITETYPEDLG_H__F798022A_0D08_4CD5_822F_1CB5AA5C0926__INCLUDED_)
