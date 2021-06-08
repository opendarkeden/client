// MakeSprDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteManager.h"
#include "MakeSprDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMakeSprDlg dialog


CMakeSprDlg::CMakeSprDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeSprDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMakeSprDlg)
	m_bSlayerMan = FALSE;
	m_bSlayerWoman = FALSE;
	m_bNPC = FALSE;
	m_npcName = _T("");
	m_bMidLevelMob = FALSE;
	m_midLevelMobName = _T("");
	m_bVampireMan = FALSE;
	m_bVampireWoman = FALSE;
	m_bOusters = FALSE;
	m_bPromotionOusters = FALSE;
	m_bPromotionSlayerMan = FALSE;
	m_bPromotionSlayerWoman = FALSE;
	m_bPromotionVampireMan = FALSE;
	m_bPromotionVampireWoman = FALSE;
	//}}AFX_DATA_INIT
}


void CMakeSprDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMakeSprDlg)
	DDX_Check(pDX, IDC_CHECK_SLAYER_MAN, m_bSlayerMan);
	DDX_Check(pDX, IDC_CHECK_SLAYER_WOMAN, m_bSlayerWoman);
	DDX_Check(pDX, IDC_CHECK_NPC, m_bNPC);
	DDX_Text(pDX, IDC_EDIT_NPC_NAME, m_npcName);
	DDX_Check(pDX, IDC_CHECK_MIDLEVEL_MOB, m_bMidLevelMob);
	DDX_Text(pDX, IDC_EDIT_MIDLEVEL_MOB_NAME, m_midLevelMobName);
	DDX_Check(pDX, IDC_CHECK_VAMPIRE, m_bVampireMan);
	DDX_Check(pDX, IDC_CHECK_VAMPIRE2, m_bVampireWoman);
	DDX_Check(pDX, IDC_OUSTERS, m_bOusters);
	DDX_Check(pDX, IDC_PROMOTION_OUSTERS, m_bPromotionOusters);
	DDX_Check(pDX, IDC_PROMOTION_SLAYER_MAN, m_bPromotionSlayerMan);
	DDX_Check(pDX, IDC_PROMOTION_SLAYER_WOMAN, m_bPromotionSlayerWoman);
	DDX_Check(pDX, IDC_PROMOTION_VAMPIRE_MAN, m_bPromotionVampireMan);
	DDX_Check(pDX, IDC_PROMOTION_VAMPIRE_WOMAN, m_bPromotionVampireWoman);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMakeSprDlg, CDialog)
	//{{AFX_MSG_MAP(CMakeSprDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FILE_FOR_MID_MOB, OnButtonOpenFileForMidMob)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FILE_FOR_NPC, OnButtonOpenFileForNpc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeSprDlg message handlers

void CMakeSprDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void CMakeSprDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CMakeSprDlg::OnButtonOpenFileForMidMob() 
{
	CFileDialog dlg( false);
	if( dlg.DoModal() == IDOK)
	{
		UpdateData( TRUE);
		m_midLevelMobName = dlg.GetPathName();
		UpdateData( FALSE);
	}
	
}

void CMakeSprDlg::OnButtonOpenFileForNpc() 
{
	CFileDialog dlg( false);
	if( dlg.DoModal() == IDOK)
	{
		UpdateData( TRUE);
		m_npcName = dlg.GetPathName();
		UpdateData( FALSE);
	}
}

