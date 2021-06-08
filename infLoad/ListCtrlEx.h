/************************************************************************
作者：luxintian(mail:luxintian@sina.com,QQ:383467500)

本类是在
Lee Nowotny的gxListCtrl类
(详见例子http://www.codeproject.com/listctrl/listeditor.asp)
和
Author的CComboListCtrl类
(
的基础上经过改就而成主要修改如下:
1,重载了ClistCtrl::InsertColumn两个函数,在该函数中添加了两个参数用来决定
某一项是使用文本框还是组合框,如果是组合框,则用一个CStringArray类的指针作
为参数,用的时候只要在主程序添加一个CStringArray的变量,并为该变量添加字符
串,就可以在相应的的组合框中添加了CStringArray变量中的字符串数组
2,在列表控件中的内容改变后向主窗体发送了了WM_ITEM_CHANGED消息,消息中
WPARAM参数保存所改变的项,LPARAM参数保存所改变的子项

************************************************************************/
#if !defined(AFX_LISTCTRLEX_H__20F0FD03_A78E_484D_BBFC_BA357CC87C68__INCLUDED_)
#define AFX_LISTCTRLEX_H__20F0FD03_A78E_484D_BBFC_BA357CC87C68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlEx.h : header file
//
#include <afxtempl.h>

#define COLUMN_STYLE_GENERAL		0x00000000
#define COLUMN_STYLE_EDIT			0x00000001
#define COLUMN_STYLE_COMBOBOX		0x00000002

#define WM_ITEM_CHANGED				WM_USER+8
//////////////////////////////////////////////////////////////////////////
typedef struct tagSUBITEMINFO
{
	tagSUBITEMINFO(){m_dwColumnType=0;m_parComboString=NULL;}
	DWORD			m_dwColumnType;
	CStringArray*	m_parComboString;
} SUBITEMINFO,*LPSUBITEMINFO;

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window

class CListCtrlEx : public CListCtrl
{
// Construction
public:
	CListCtrlEx();

// Attributes
public:

// Operations
public:
	int InsertColumn( int nCol, const LVCOLUMN* pColumn, DWORD nStyle = COLUMN_STYLE_GENERAL,CStringArray * pStringArray = NULL );
	int InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, DWORD nStyle = COLUMN_STYLE_GENERAL ,CStringArray * pStringArray = NULL, int nSubItem = -1 );
	BOOL DeleteColumn( int nCol );
		
	void ShowSubCtrl(int nItem, int nSubItem,DWORD dwCtrlType = COLUMN_STYLE_EDIT);
	int HitTestEx (CPoint& Point, int& nSubItem);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlEx();

	// Generated message map functions
protected:
	CArray<LPSUBITEMINFO,LPSUBITEMINFO> m_arColumnInfo;
	
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////

// CListCellEdit window

class CListCellEdit : public CEdit
{
	// Construction
public:
	CListCellEdit(int nItem, int nSubItem, CString strInitTex);
	void SetListItemText();
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCellEdit)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CListCellEdit();
	
protected:
	int			m_nItem;
    int			m_nSubItem;
    CString		m_strInitText;
    BOOL		m_bEscape;
	
	// Generated message map functions
	//{{AFX_MSG(CListCellEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CListCellCombo window

class CListCellCombo : public CComboBox
{
// Construction
public:
	CListCellCombo();
	CListCellCombo(int nItem,int nSubItem,CString strWindowText);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCellCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCellCombo();

	// Generated message map functions
protected:	
	int m_iRowIndex;// Index of the item in the list control	
	int m_iColumnIndex;// Index of the subitem in the list control		
	CString m_strWindowText;// Previous selected string value in the combo control
	
	//{{AFX_MSG(CListCellCombo)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H__20F0FD03_A78E_484D_BBFC_BA357CC87C68__INCLUDED_)
