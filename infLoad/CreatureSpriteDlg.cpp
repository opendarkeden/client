// CreatureSpriteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "infLoad.h"
#include "CreatureSpriteDlg.h"
#include "MCreatureSpriteTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CreatureSpriteDlg dialog


CreatureSpriteDlg::CreatureSpriteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CreatureSpriteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CreatureSpriteDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CreatureSpriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CreatureSpriteDlg)
	DDX_Control(pDX, IDC_LIST2, m_InfoList);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CreatureSpriteDlg, CDialog)
	//{{AFX_MSG_MAP(CreatureSpriteDlg)
	ON_BN_CLICKED(IDC_BUTTON_READ, OnButtonRead)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CreatureSpriteDlg message handlers
BOOL CreatureSpriteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	ListView_SetExtendedListViewStyle(m_InfoList.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	m_InfoList.InsertColumn(0,"Item Name",LVCFMT_LEFT,240);
	m_InfoList.InsertColumn(1,"Item Info",LVCFMT_LEFT,200,COLUMN_STYLE_EDIT);
	return TRUE;
}

void CreatureSpriteDlg::OnButtonRead() 
{
	if (g_pCreatureSpriteTable != NULL)
	{
		g_pActionInfoTable->Release();
		delete g_pActionInfoTable;
		g_pActionInfoTable = NULL;
	}

	g_pCreatureSpriteTable = new CREATURESPRITE_TABLE;

	class ifstream creatureInfoFile2;
	if (!FileOpenBinary(FILE_CREATURESPRITE, creatureInfoFile2))
		return;
	g_pCreatureSpriteTable->LoadFromFile(creatureInfoFile2);
	creatureInfoFile2.close();

	//»æÖÆ
	DrawAll();
	
}
void CreatureSpriteDlg::DrawAll()
{
	CString m_sizeout;
	for(int i=0; i<(*g_pCreatureSpriteTable).GetSize(); i++)
	{
		m_sizeout.Format("SpriteType:%d", i);
		m_list.AddString(m_sizeout);
	}
	m_sizeout.Format("Size:%d,Index:%d",(*g_pCreatureSpriteTable).GetSize(),0);
	SetDlgItemText(IDC_SIZESI,m_sizeout);
}

void CreatureSpriteDlg::OnSelchangeList1() 
{
	m_InfoList.DeleteAllItems();
	CString m_TempOut;
	
	CString m_sizeout;
	m_sizeout.Format("Size:%d,Index:%d",(*g_pCreatureSpriteTable).GetSize(),m_list.GetCurSel());
	SetDlgItemText(IDC_SIZESI,m_sizeout);
	//////////////////FrameID
	m_TempOut.Format("%d", (*g_pCreatureSpriteTable).Get(m_list.GetCurSel()).FrameID);
	m_InfoList.InsertItem(0, "FrameID");
	m_InfoList.SetItemText(0,1,m_TempOut);
	//////////////////SpriteFilePosition
	m_TempOut.Format("%d", (*g_pCreatureSpriteTable).Get(m_list.GetCurSel()).SpriteFilePosition);
	m_InfoList.InsertItem(1, "SpriteFilePosition");
	m_InfoList.SetItemText(1,1,m_TempOut);
	//////////////////SpriteShadowFilePosition
	m_TempOut.Format("%d", (*g_pCreatureSpriteTable).Get(m_list.GetCurSel()).SpriteShadowFilePosition);
	m_InfoList.InsertItem(2, "SpriteShadowFilePosition");
	m_InfoList.SetItemText(2,1,m_TempOut);
	//////////////////FirstSpriteID
	m_TempOut.Format("%d", (*g_pCreatureSpriteTable).Get(m_list.GetCurSel()).FirstSpriteID);
	m_InfoList.InsertItem(3, "FirstSpriteID");
	m_InfoList.SetItemText(3,1,m_TempOut);
	//////////////////LastSpriteID
	m_TempOut.Format("%d", (*g_pCreatureSpriteTable).Get(m_list.GetCurSel()).LastSpriteID);
	m_InfoList.InsertItem(4, "LastSpriteID");
	m_InfoList.SetItemText(4,1,m_TempOut);
	//////////////////FirstShadowSpriteID
	m_TempOut.Format("%d", (*g_pCreatureSpriteTable).Get(m_list.GetCurSel()).FirstShadowSpriteID);
	m_InfoList.InsertItem(5, "FirstShadowSpriteID");
	m_InfoList.SetItemText(5,1,m_TempOut);
	//////////////////LastShadowSpriteID
	m_TempOut.Format("%d", (*g_pCreatureSpriteTable).Get(m_list.GetCurSel()).LastShadowSpriteID);
	m_InfoList.InsertItem(6, "LastShadowSpriteID");
	m_InfoList.SetItemText(6,1,m_TempOut);
	//////////////////CreatureType
	m_TempOut.Format("%d", (*g_pCreatureSpriteTable).Get(m_list.GetCurSel()).CreatureType);
	m_InfoList.InsertItem(7, "CreatureType");
	m_InfoList.SetItemText(7,1,m_TempOut);
}
