// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteManager.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog


CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bCanceled = FALSE;
}


void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CProgressDlg)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg message handlers

void CProgressDlg::SetText(CString& strMessage)
{
	SetDlgItemText( IDC_STATIC_INFO, strMessage);
}

void CProgressDlg::SetProgress(int cur, int max)
{
	m_progress.SetRange32( 0, max);
	m_progress.SetPos( cur);
}

void CProgressDlg::OnCancel1() 
{
	m_bCanceled = TRUE;
	CDialog::OnCancel();
	// TODO: Add your control notification handler code here
	
}
