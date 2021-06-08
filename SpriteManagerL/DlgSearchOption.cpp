// DlgSearchOption.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteManager.h"
#include "DlgSearchOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSearchOption dialog


CDlgSearchOption::CDlgSearchOption(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSearchOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSearchOption)
	m_bUpdateAniData = FALSE;
	m_bUpdateImage = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgSearchOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSearchOption)
	DDX_Check(pDX, IDC_CHECK_ANI, m_bUpdateAniData);
	DDX_Check(pDX, IDC_CHECK_IMAGE, m_bUpdateImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSearchOption, CDialog)
	//{{AFX_MSG_MAP(CDlgSearchOption)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSearchOption message handlers
