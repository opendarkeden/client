// CreatureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "infLoad.h"
#include "CreatureDlg.h"
#include "MCreatureTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CreatureDlg dialog


CreatureDlg::CreatureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CreatureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CreatureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CreatureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CreatureDlg)
	DDX_Control(pDX, IDC_LIST2, m_InfoList);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CreatureDlg, CDialog)
	//{{AFX_MSG_MAP(CreatureDlg)
	ON_BN_CLICKED(IDC_BUTTON_READ, OnButtonRead)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CreatureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ListView_SetExtendedListViewStyle(m_InfoList.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	m_InfoList.InsertColumn(0,"Item Name",LVCFMT_LEFT,240);
	m_InfoList.InsertColumn(1,"Item Info",LVCFMT_LEFT,200,COLUMN_STYLE_EDIT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CreatureDlg message handlers

void CreatureDlg::OnOK() 
{
	MessageBox("123");
	
//	CDialog::OnOK();
}

void CreatureDlg::OnButtonRead() 
{
	if (g_pCreatureTable != NULL)
	{
		g_pActionInfoTable->Release();
		delete g_pActionInfoTable;
		g_pActionInfoTable = NULL;
	}

	g_pCreatureTable = new CREATURE_TABLE;

	class ifstream creatureInfoFile2;
	if (!FileOpenBinary(FILE_CREATURE, creatureInfoFile2))
		return;
	g_pCreatureTable->LoadFromFile(creatureInfoFile2);
	creatureInfoFile2.close();

	//by viva
/*	class ofstream writefile;
	writefile.open("creature.txt");
	char strtmp[40960];
	for(int i = 0; i<g_pCreatureTable->GetSize(); i++)
	{
		sprintf(strtmp, "%s\r\n", g_pCreatureTable->Get(i).Name.GetString());
		writefile.write(strtmp, strlen(strtmp));
	}
	writefile.close();*/
	//»æÖÆ
	DrawAll();
//	m_list.SetCurSel(0);

//	OnSelchangeList1();
	
}

void CreatureDlg::DrawAll()
{
	for(int i=0; i<(*g_pCreatureTable).GetSize(); i++)
	{
		try{
		m_list.AddString(g_pCreatureTable->Get(i).Name);
		}catch(...){}
	}
	CString m_sizeout;
	m_sizeout.Format("Size:%d,Index:%d",(*g_pCreatureTable).GetSize(),0);
	SetDlgItemText(IDC_SIZESI,m_sizeout);
//	int DataNum[280];
//	for(i=0; i<280; i++)
//		DataNum[i]=0;
//	for(int j=0; j<(*g_pCreatureTable).GetSize(); j++)
//	{
//		for(i=0;i<(*g_pCreatureTable).Get(j).SpriteTypes.GetSize(); i++)
//		{
//			int k = (*g_pCreatureTable).Get(j).SpriteTypes.Get(i);
//			DataNum[k] = 1;
//		}
//	}
//	CString strTemp, str;
//	for(i = 0; i<280;i++)
//	{
//		if(DataNum[i] == 0)
//		{
//			str.Format("%d ", i);
//			strTemp+=str;
//		}
		
//	}
//	MessageBox(strTemp);
}

void CreatureDlg::OnSelchangeList1() 
{
	m_InfoList.DeleteAllItems();
	CString m_TempOut;
	
	CString m_sizeout;
	m_sizeout.Format("Size:%d,Index:%d",(*g_pCreatureTable).GetSize(),m_list.GetCurSel());
	SetDlgItemText(IDC_SIZESI,m_sizeout);
//////////////////////Name
	m_TempOut = (*g_pCreatureTable).Get(m_list.GetCurSel()).Name;
	m_InfoList.InsertItem(0, "Name");
	m_InfoList.SetItemText(0,1,m_TempOut);
	/////////////////SpriteTypes(stcount)
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).SpriteTypes.GetSize());
	m_InfoList.InsertItem(1, "SpriteTypes(stcount)");
	m_InfoList.SetItemText(1,1,m_TempOut);
	/////////////////SpriteTypes(int)
	m_TempOut = "";
	for(int i=0;i<(*g_pCreatureTable).Get(m_list.GetCurSel()).SpriteTypes.GetSize(); i++)
	{
		CString str;
		str.Format("%d|", (*g_pCreatureTable).Get(m_list.GetCurSel()).SpriteTypes.Get(i));
		m_TempOut+=str;
	}
	m_InfoList.InsertItem(2, "SpriteTypes(int)");
	m_InfoList.SetItemText(2, 1, m_TempOut);
	/////////////////bMale
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).bMale);
	m_InfoList.InsertItem(3, "bMale");
	m_InfoList.SetItemText(3,1,m_TempOut);
	/////////////////CreatureTribe
	m_TempOut.Format("%d", (BYTE)(*g_pCreatureTable).Get(m_list.GetCurSel()).GetCreatureTribe());
	m_InfoList.InsertItem(4, "CreatureTribe");
	m_InfoList.SetItemText(4,1,m_TempOut);
	/////////////////MoveTimes
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).MoveTimes);
	m_InfoList.InsertItem(5,"MoveTimes");
	m_InfoList.SetItemText(5,1,m_TempOut);
	/////////////////MoveRatio
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).MoveRatio);
	m_InfoList.InsertItem(6, "MoveRatio");
	m_InfoList.SetItemText(6,1,m_TempOut);
	/////////////////MoveTimesMotor
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).MoveTimesMotor);
	m_InfoList.InsertItem(7, "MoveTimesMotor");
	m_InfoList.SetItemText(7,1,m_TempOut);
	/////////////////Height
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).Height);
	m_InfoList.InsertItem(8, "Height");
	m_InfoList.SetItemText(8,1,m_TempOut);
	/////////////////DeadHeight
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).DeadHeight);
	m_InfoList.InsertItem(9, "DeadHeight");
	m_InfoList.SetItemText(9,1,m_TempOut);
	/////////////////DeadActionInfo
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).DeadActionInfo);
	m_InfoList.InsertItem(10, "DeadActionInfo");
	m_InfoList.SetItemText(10,1,m_TempOut);
	/////////////////ColorSet
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).ColorSet);
	m_InfoList.InsertItem(11, "ColorSet");
	m_InfoList.SetItemText(11,1,m_TempOut);
	/////////////////bFlyingCreature
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).bFlyingCreature);
	m_InfoList.InsertItem(12, "bFlyingCreature");
	m_InfoList.SetItemText(12,1,m_TempOut);
	/////////////////FlyingHeight
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).FlyingHeight);
	m_InfoList.InsertItem(13, "FlyingHeight");
	m_InfoList.SetItemText(13,1,m_TempOut);
	/////////////////bHeadCut
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).bHeadCut);
	m_InfoList.InsertItem(14, "bHeadCut");
	m_InfoList.SetItemText(14,1,m_TempOut);
	/////////////////HPBarWidth
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).HPBarWidth);
	m_InfoList.InsertItem(15, "HPBarWidth");
	m_InfoList.SetItemText(15,1,m_TempOut);
	/////////////////ChangeColorSet
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).ChangeColorSet);
	m_InfoList.InsertItem(16, "ChangeColorSet");
	m_InfoList.SetItemText(16,1,m_TempOut);
	/////////////////ShadowCount
	m_TempOut.Format("%d", (*g_pCreatureTable).Get(m_list.GetCurSel()).ShadowCount);
	m_InfoList.InsertItem(17, "ShadowCount");
	m_InfoList.SetItemText(17,1,m_TempOut);
	/////////////////ActionSound
	m_TempOut.Format("size:%d : ",  (*g_pCreatureTable).Get(m_list.GetCurSel()).GetActionMax());
	for(i=0;i< (*g_pCreatureTable).Get(m_list.GetCurSel()).GetActionMax();i++)
	{
		CString str;
		str.Format("%d|",  (*g_pCreatureTable).Get(m_list.GetCurSel()).GetActionSound(i));
		m_TempOut+=str;
	}
	m_InfoList.InsertItem(18, "ActionSound");
	m_InfoList.SetItemText(18,1,m_TempOut);
	//////////////////ActionCount
	m_TempOut.Format("size:%d : ", (*g_pCreatureTable).Get(m_list.GetCurSel()).GetActionMax());
	for(i=0;i<(*g_pCreatureTable).Get(m_list.GetCurSel()).GetActionMax();i++)
	{
		CString str;
		str.Format("%d|", (*g_pCreatureTable).Get(m_list.GetCurSel()).GetActionCount(i));
		m_TempOut+=str;
	}
	m_InfoList.InsertItem(19, "ActionCount");
	m_InfoList.SetItemText(19,1,m_TempOut);
	//////////////////ItemWearInfo
	if((*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo!=NULL)
	{
		/////////////////////////skinColor
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->skinColor);
		m_InfoList.InsertItem(20,"ItemWearInfo:skinColor");
		m_InfoList.SetItemText(20,1,m_TempOut);
		/////////////////////////hairColor
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->hairColor);
		m_InfoList.InsertItem(21,"ItemWearInfo:hairColor");
		m_InfoList.SetItemText(21,1,m_TempOut);
		/////////////////////////jacketColor
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->jacketColor);
		m_InfoList.InsertItem(22,"ItemWearInfo:jacketColor");
		m_InfoList.SetItemText(22,1,m_TempOut);
		/////////////////////////pantsColor
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->pantsColor);
		m_InfoList.InsertItem(23,"ItemWearInfo:pantsColor");
		m_InfoList.SetItemText(23,1,m_TempOut);
		/////////////////////////helmetColor
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->helmetColor);
		m_InfoList.InsertItem(24,"ItemWearInfo:helmetColor");
		m_InfoList.SetItemText(24,1,m_TempOut);
		/////////////////////////weaponColor
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->weaponColor);
		m_InfoList.InsertItem(25,"ItemWearInfo:weaponColor");
		m_InfoList.SetItemText(25,1,m_TempOut);
		/////////////////////////shieldColor
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->shieldColor);
		m_InfoList.InsertItem(26,"ItemWearInfo:shieldColor");
		m_InfoList.SetItemText(26,1,m_TempOut);
		/////////////////////////motorcycleColor
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->motorcycleColor);
		m_InfoList.InsertItem(27,"ItemWearInfo:motorcycleColor");
		m_InfoList.SetItemText(27,1,m_TempOut);
		/////////////////////////hair
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->hair);
		m_InfoList.InsertItem(28,"ItemWearInfo:hair");
		m_InfoList.SetItemText(28,1,m_TempOut);
		/////////////////////////jacket
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->jacket);
		m_InfoList.InsertItem(29,"ItemWearInfo:jacket");
		m_InfoList.SetItemText(29,1,m_TempOut);
		/////////////////////////pants
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->pants);
		m_InfoList.InsertItem(30,"ItemWearInfo:pants");
		m_InfoList.SetItemText(30,1,m_TempOut);
		/////////////////////////helmet
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->helmet);
		m_InfoList.InsertItem(31,"ItemWearInfo:helmet");
		m_InfoList.SetItemText(31,1,m_TempOut);
		/////////////////////////weapon
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->weapon);
		m_InfoList.InsertItem(32,"ItemWearInfo:weapon");
		m_InfoList.SetItemText(32,1,m_TempOut);
		/////////////////////////shield
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->shield);
		m_InfoList.InsertItem(33,"ItemWearInfo:shield");
		m_InfoList.SetItemText(33,1,m_TempOut);
		/////////////////////////motorcycle
		m_TempOut.Format("%d",(*g_pCreatureTable).Get(m_list.GetCurSel()).pItemWearInfo->motorcycle);
		m_InfoList.InsertItem(34,"ItemWearInfo:motorcycle");
		m_InfoList.SetItemText(34,1,m_TempOut);
	}
	


}
