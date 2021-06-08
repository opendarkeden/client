// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

CListCtrlEx::CListCtrlEx()
{
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers

void CListCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (GetFocus() != this) SetFocus();
	
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CListCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (GetFocus() != this) SetFocus();
	
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CListCtrlEx::ShowSubCtrl(int nItem, int nSubItem,DWORD dwCtrlType/* = COLUMN_STYLE_EDIT*/)
{
    // The returned pointer should not be saved
	
    // Make sure that the item is visible
    if (!EnsureVisible (nItem, TRUE)) return;
	
    // Make sure that nCol is valid
    CHeaderCtrl* pHeader = (CHeaderCtrl*) GetDlgItem(0);
    int nColumnCount = pHeader->GetItemCount();
    if (nSubItem >= nColumnCount || GetColumnWidth (nSubItem) < 5)
	{
		return;
	}
	
    // Get the column offset
    int Offset = 0;
    for (int iColumn = 0; iColumn < nSubItem; iColumn++)
		Offset += GetColumnWidth (iColumn);
	
    CRect Rect;
    GetItemRect (nItem, &Rect, LVIR_BOUNDS);
	
    // Now scroll if we need to expose the column
    CRect ClientRect;
    GetClientRect (&ClientRect);
    if (Offset + Rect.left < 0 || Offset + Rect.left > ClientRect.right)
    {
		CSize Size;
		if (Offset + Rect.left > 0)
			Size.cx = -(Offset - Rect.left);
		else
			Size.cx = Offset - Rect.left;
		Size.cy = 0;
		Scroll (Size);
		Rect.left -= Size.cx;
    }
	
    // Get nSubItem alignment
    LV_COLUMN lvCol;
    lvCol.mask = LVCF_FMT;
    GetColumn (nSubItem, &lvCol);
    DWORD dwStyle;
    if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
    else if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
    else dwStyle = ES_CENTER;
	
    Rect.left += Offset+4;
    Rect.right = Rect.left + GetColumnWidth (nSubItem) - 3;
    if (Rect.right > ClientRect.right)
		Rect.right = ClientRect.right;
	
    dwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;

	CString strCulText=GetItemText (nItem, nSubItem);
	if(dwCtrlType==COLUMN_STYLE_EDIT)
	{
		CEdit *pEdit = new CListCellEdit (nItem, nSubItem, strCulText);
		pEdit->Create (dwStyle, Rect, this, 101);
	}
	if(dwCtrlType==COLUMN_STYLE_COMBOBOX)
	{
		//显示组合框
		CRect rc(Rect.left-2,Rect.top-2,Rect.right,Rect.bottom+100);
		CComboBox * pCombo = new CListCellCombo(nItem,nSubItem,strCulText);
		pCombo->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,	rc,this, 1);
		if(m_arColumnInfo[nSubItem]->m_parComboString)
		{
			for(int i=0;i<m_arColumnInfo[nSubItem]->m_parComboString->GetSize();i++)
			{
				pCombo->AddString((*(m_arColumnInfo[nSubItem]->m_parComboString)).GetAt(i));
			}
		}
 		int nIndex=pCombo->FindString(0,strCulText);
		if(nIndex!=CB_ERR)
		{
			pCombo->SetCurSel(nIndex);
		}
		else
		{
			pCombo->SetCurSel(-1);
		}
	}	
}

void CListCtrlEx::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	LV_ITEM	*plvItem = &pDispInfo->item;
	
    if (plvItem->pszText != NULL)
    {
		SetItemText (plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
    }
	
	*pResult = 0;
}

int CListCtrlEx::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, DWORD nStyle,CStringArray * pStringArray, int nSubItem)
{
	int nIndex=CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat ,nWidth ,nSubItem );
	if(nIndex!=-1)
	{
		LPSUBITEMINFO pSII=new SUBITEMINFO;
		pSII->m_dwColumnType=nStyle;
		pSII->m_parComboString=pStringArray;
		m_arColumnInfo.InsertAt(nIndex,pSII);
	}
	return nIndex;
}

int CListCtrlEx::InsertColumn( int nCol, const LVCOLUMN* pColumn, DWORD nStyle,CStringArray * pStringArray)
{
	int nIndex=CListCtrl::InsertColumn(nCol,pColumn);
	if(nIndex!=-1)
	{
		LPSUBITEMINFO pSII=new SUBITEMINFO;
		pSII->m_dwColumnType=nStyle;
		pSII->m_parComboString=pStringArray;
		m_arColumnInfo.InsertAt(nIndex,pSII);
	}
	return nIndex;
}

BOOL CListCtrlEx::DeleteColumn(int nCol )
{
	BOOL bResult=CListCtrl::DeleteColumn(nCol);
	if(bResult)
	{
		delete m_arColumnInfo.GetAt(nCol);
		m_arColumnInfo.RemoveAt(nCol);
	}
	return bResult;
}

void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CListCtrl::OnLButtonDown(nFlags, point);
	// TODO: Add your message handler code here and/or call default
    int nItem;
    int nSubItem;
    if ((nItem = HitTestEx (point, nSubItem)) ==-1)
    {
		return;
	}
	
	switch(m_arColumnInfo[nSubItem]->m_dwColumnType)
	{
	case COLUMN_STYLE_GENERAL:
		return;
		break;
	case COLUMN_STYLE_EDIT:
		ShowSubCtrl(nItem,nSubItem,COLUMN_STYLE_EDIT);
		break;
	case COLUMN_STYLE_COMBOBOX:
		ShowSubCtrl(nItem,nSubItem,COLUMN_STYLE_COMBOBOX);
		break;
	default:
		break;
	}
}

int CListCtrlEx::HitTestEx (CPoint& Point, int& nSubItem)
{
	nSubItem = 0;
	int ColumnNum = 0;
    int Row = HitTest (Point, NULL);
	
    // Make sure that the ListView is in LVS_REPORT
    if ((GetWindowLong (m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return Row;
	
    // Get the top and bottom row visible
    Row = GetTopIndex();
    int Bottom = Row + GetCountPerPage();
    if (Bottom > GetItemCount())
		Bottom = GetItemCount();
    
    // Get the number of columns
    CHeaderCtrl* pHeader = (CHeaderCtrl*) GetDlgItem(0);
    int nColumnCount = pHeader->GetItemCount();
	
    // Loop through the visible rows
    for(; Row <= Bottom; Row++)
    {
		// Get bounding rect of item and check whether point falls in it.
		CRect Rect;
		GetItemRect (Row, &Rect, LVIR_BOUNDS);
		if (Rect.PtInRect (Point))
		{
			// Now find the column
			for (ColumnNum = 0; ColumnNum < nColumnCount; ColumnNum++)
			{
				int ColWidth = GetColumnWidth (ColumnNum);
				if (Point.x >= Rect.left && Point.x <= (Rect.left + ColWidth))
				{
					nSubItem = ColumnNum;
					return Row;
				}
				Rect.left += ColWidth;
			}
		}
    }
	
    return -1;
}

void CListCtrlEx::OnDestroy() 
{
	
	// TODO: Add your message handler code here
	int nCount=m_arColumnInfo.GetSize();
	for(int i=0;i<nCount;i++)
	{
		delete m_arColumnInfo.GetAt(i);
	}
	m_arColumnInfo.RemoveAll();
	
	CListCtrl::OnDestroy();
	
}

/////////////////////////////////////////////////////////////////////////////
// CListCellEdit

CListCellEdit::	CListCellEdit(int nItem, int nSubItem, CString strInitText)
{
    m_nItem = nItem;
    m_nSubItem = nSubItem;
    m_strInitText = strInitText;
	m_bEscape=FALSE;
}

CListCellEdit::~CListCellEdit()
{
}


BEGIN_MESSAGE_MAP(CListCellEdit, CEdit)
	//{{AFX_MSG_MAP(CListCellEdit)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCellEdit message handlers

void CListCellEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	SetListItemText();

    DestroyWindow();
}

void CListCellEdit::OnNcDestroy() 
{
	CEdit::OnNcDestroy();
	
	// TODO: Add your message handler code here
    delete this;
}

BOOL CListCellEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN
			|| pMsg->wParam == VK_DELETE
			|| pMsg->wParam == VK_ESCAPE
			|| GetKeyState( VK_CONTROL)
			)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;		    	// DO NOT process further
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

void CListCellEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if( nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if( nChar == VK_ESCAPE )
			m_bEscape = TRUE;
		GetParent()->SetFocus();

		return;
	}

	// Resize edit control if needed
	// Get text extent
	CString str;

	GetWindowText( str );
	CWindowDC dc(this);
	CFont *pFont = GetParent()->GetFont();
	CFont *pFontDC = dc.SelectObject( pFont );
	CSize size = dc.GetTextExtent( str );
	dc.SelectObject( pFontDC );
	size.cx += 5;			   	// add some extra buffer

	// Get client rect
	CRect rect, parentrect;
	GetClientRect( &rect );
	GetParent()->GetClientRect( &parentrect );

	// Transform rect to parent coordinates
	ClientToScreen( &rect );
	GetParent()->ScreenToClient( &rect );

	// Check whether control needs to be resized
	// and whether there is space to grow
	if( size.cx > rect.Width() )
	{
		if( size.cx + rect.left < parentrect.right )
			rect.right = rect.left + size.cx;
		else
			rect.right = parentrect.right;
		MoveWindow( &rect );
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

int CListCellEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Set the proper font
	CFont* font = GetParent()->GetFont();
	SetFont(font);

	SetWindowText( m_strInitText );
	SetFocus();
	SetSel( 0, -1 );
	
	return 0;
}

void CListCellEdit::SetListItemText()
{
    CString Text;
    GetWindowText (Text);

    // Send Notification to parent of ListView ctrl
    LV_DISPINFO dispinfo;
    dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
    dispinfo.hdr.idFrom = GetDlgCtrlID();
    dispinfo.hdr.code = LVN_ENDLABELEDIT;

    dispinfo.item.mask = LVIF_TEXT;
    dispinfo.item.iItem = m_nItem;
    dispinfo.item.iSubItem = m_nSubItem;
    dispinfo.item.pszText = m_bEscape ? NULL : LPTSTR ((LPCTSTR) Text);
    dispinfo.item.cchTextMax = Text.GetLength();
    GetParent()->GetParent()->SendMessage (WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM) &dispinfo);
	//如果已经修改则向主窗体发送消息
	if(Text.Compare(m_strInitText) !=0 )
	{
		GetParent()->GetParent()->SendMessage(WM_ITEM_CHANGED,(WPARAM)m_nItem,(LPARAM)m_nSubItem);
	}

}
/////////////////////////////////////////////////////////////////////////////
// CListCellCombo

CListCellCombo::CListCellCombo()
{
}

CListCellCombo::CListCellCombo(int nItem,int nSubItem,CString strWindowText)
{
	m_iRowIndex=nItem;
	m_iColumnIndex=nSubItem;
	m_strWindowText=strWindowText;
}

CListCellCombo::~CListCellCombo()
{
}


BEGIN_MESSAGE_MAP(CListCellCombo, CComboBox)
	//{{AFX_MSG_MAP(CListCellCombo)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCellCombo message handlers

BOOL CListCellCombo::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	// If the message if for "Enter" or "Esc"
	// Do not process
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			// DO NOT process further
			return TRUE;				
		}
	}
	
	return CComboBox::PreTranslateMessage(pMsg);
}

void CListCellCombo::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	CString str;
	GetWindowText(str);
	
	// Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;
	
	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iRowIndex;
	dispinfo.item.iSubItem = m_iColumnIndex;
	dispinfo.item.pszText = LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = str.GetLength();
	
	GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);
	//如果已经修改则向主窗体发送消息
	if(str.Compare(m_strWindowText) !=0 )
	{
		GetParent()->GetParent()->SendMessage(WM_ITEM_CHANGED,(WPARAM)m_iRowIndex,(LPARAM)m_iColumnIndex);
	}	
	// Close the control
	PostMessage(WM_CLOSE);
	
}

int CListCellCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);	
	SetFocus();
	
	return 0;
}

void CListCellCombo::OnNcDestroy() 
{
	CComboBox::OnNcDestroy();
	
	// TODO: Add your message handler code here
	delete this;
}
