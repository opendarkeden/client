// InputCount.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteManager.h"
#include "InputCount.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputCount dialog


CInputCount::CInputCount(CWnd* pParent /*=NULL*/)
	: CDialog(CInputCount::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputCount)
	m_dwCount = 0;
	//}}AFX_DATA_INIT
}


void CInputCount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputCount)
	DDX_Text(pDX, IDC_EDIT1, m_dwCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputCount, CDialog)
	//{{AFX_MSG_MAP(CInputCount)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputCount message handlers
