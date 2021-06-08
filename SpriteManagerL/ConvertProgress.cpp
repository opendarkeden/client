// ConvertProgress.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteManager.h"
#include "ConvertProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConvertProgress dialog


CConvertProgress::CConvertProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CConvertProgress::IDD, pParent)
{
	m_bCanceled = FALSE;
	//{{AFX_DATA_INIT(CConvertProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConvertProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConvertProgress)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConvertProgress, CDialog)
	//{{AFX_MSG_MAP(CConvertProgress)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_PROGRESS, OnProgress)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConvertProgress message handlers

void CConvertProgress::OnProgress(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CConvertProgress::SetText(CString& strMessage)
{
	SetDlgItemText( IDC_STATIC_INFO, strMessage);
}

void CConvertProgress::SetProgress(int cur, int max)
{
	m_progress.SetRange32( 0, max);
	m_progress.SetPos( cur);
}

void CConvertProgress::OnCancel2() 
{
	m_bCanceled = TRUE;
	CDialog::OnCancel();
	// TODO: Add your control notification handler code here	
}
