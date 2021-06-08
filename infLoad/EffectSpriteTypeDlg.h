#if !defined(AFX_EFFECTSPRITETYPEDLG_H__9D9BBDAA_4DB7_4AFE_A412_A93F2C8CF8F0__INCLUDED_)
#define AFX_EFFECTSPRITETYPEDLG_H__9D9BBDAA_4DB7_4AFE_A412_A93F2C8CF8F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EffectSpriteTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EffectSpriteTypeDlg dialog
#include "ListCtrlEx.h"

class EffectSpriteTypeDlg : public CDialog
{
// Construction
public:
	typedef unsigned short		TYPE_FRAMEID;
	typedef std::list<TYPE_FRAMEID>		FRAMEID_LIST;
	char m_path[MAX_PATH];
	void DrawAll();
	void Init();
	EffectSpriteTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EffectSpriteTypeDlg)
	enum { IDD = IDD_OLE_PAGE01 };
	CListCtrlEx	m_InfoList;
	CListBox	m_PairFrameIDList;
	CComboBox	m_comBitType;
	CListBox	m_list;
	BOOL	m_RepeatFrame;
	int		m_FindNum;
	short	m_PAIRINFO;
	BOOL	m_ismod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EffectSpriteTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EffectSpriteTypeDlg)
	afx_msg void OnRead();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg void OnChangeEditFind();
	afx_msg void OnDestroy();
	afx_msg void OnWrite();
	afx_msg void OnMod();
	afx_msg void OnSelchangeLISTPairFrameIDList();
	afx_msg void OnAddinfo();
	afx_msg void OnDelete();
	afx_msg void OnAdd();
	afx_msg void OnCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Draw(int n);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFFECTSPRITETYPEDLG_H__9D9BBDAA_4DB7_4AFE_A412_A93F2C8CF8F0__INCLUDED_)
