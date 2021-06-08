// InputOffset.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteManager.h"
#include "InputOffset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputOffset dialog


CInputOffset::CInputOffset(CWnd* pParent /*=NULL*/)
	: CDialog(CInputOffset::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputOffset)
	m_x = 0;
	m_y = 0;
	//}}AFX_DATA_INIT
}


void CInputOffset::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputOffset)
	DDX_Text(pDX, IDC_EDIT1, m_x);
	DDX_Text(pDX, IDC_EDIT2, m_y);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputOffset, CDialog)
	//{{AFX_MSG_MAP(CInputOffset)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputOffset message handlers
