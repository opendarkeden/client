// ActionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "infLoad.h"
#include "ActionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 
//////////////////////////////////////////////////////////////////////////
//链表
CPtrList m_aclist;
#define MAX_HEAD 48
#define MAX_NODE 9

enum ITEMLIST
{
	E_Name = 0,
	E_Action,
	E_EffectSpriteType,
	E_EffectSpriteTypeFemale,
	E_IsUseRepeatFrame,
	E_StartFrame1,
	E_StartFrame2,
	E_StartFrame3,
	E_CastingFrame1,
	E_CastingFrame2,
	E_CastingFrame3,
	E_CastingStartFrame1,
	E_CastingStartFrame2,
	E_CastingStartFrame3,
	E_RepeatStartFrame1,
	E_RepeatStartFrame2,
	E_RepeatStartFrame3,
	E_RepeatEndFrame1,
	E_RepeatEndFrame2,
	E_RepeatEndFrame3,
	E_RepeatLimit,
	E_CastingEffectToSelf,
	E_CastingActionInfo,
	E_CastingAction,
	E_Range,
	E_fTarget,
	E_fStart,
	E_fUserType,
	E_fWeaponType,
	E_AffectCurrentWeaponRange,
	E_AffectCurrentWeaponSound,
	E_AffectCurrentWeaponDelay,
	E_AffectCurrentWeaponAction,
	E_AffectCurrentWeaponCastingStartFrame,
	E_AffectCurrentWeaponCastingFrames,
	E_AffectCurrentWeaponActionEffectSpriteType,
	E_fOption,
	E_PlusActionInfo,
	E_Delay,
	E_Value,
	E_SoundID,
	E_MainNode,
	E_ActionResultID,
	E_ActionResultValue,
	E_bAttack,
	E_fSelectCreature,
	E_MasterySkillStep,
	E_bIgnoreFailDelay,
};
/////////////////////////////////////////////////////////////////////////////
// CActionDlg dialog


CActionDlg::CActionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CActionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CActionDlg)
	m_NODENUM = 0;
	//}}AFX_DATA_INIT
}


void CActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionDlg)
	DDX_Control(pDX, IDC_EFFSTA, m_effsta);
	DDX_Control(pDX, IDC_APCK, m_apackcombo);
	DDX_Control(pDX, IDC_LIST2, m_InfoList);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT2, m_NODENUM);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionDlg, CDialog)
	//{{AFX_MSG_MAP(CActionDlg)
	ON_BN_CLICKED(IDC_READ, OnRead)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_WRITE, OnWrite)
	ON_BN_CLICKED(IDC_ADDNODE, OnAddnode)
	ON_BN_CLICKED(IDC_ADDINFO, OnAddinfo)
	ON_BN_CLICKED(IDC_MOD, OnMod)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionDlg message handlers

//读取
void CActionDlg::OnRead() 
{
	if (g_pActionInfoTable != NULL)
	{
		g_pActionInfoTable->Release();
		delete g_pActionInfoTable;
		g_pActionInfoTable = NULL;
	}

	g_pActionInfoTable = new MActionInfoTable;

	class ifstream actionInfoFile2;
	if (!FileOpenBinary(FILE_AC, actionInfoFile2))
		return;
	g_pActionInfoTable->LoadFromFile(actionInfoFile2);
	actionInfoFile2.close();

	//绘制
	DrawAll();
	m_list.SetCurSel(0);

	OnSelchangeList1();
}

void CActionDlg::DrawAll()
{
//	CString strTemp="";

	for(int i = 0;i < g_pActionInfoTable->GetSize();i++)
	{
		m_list.AddString(g_pActionInfoTable->Get(i).GetName());
//		if(strcmp(g_pActionInfoTable->Get(i).GetName(),"...Not Defined...")==0)
//		{
//			char chartemp[8];
//			strTemp+="ACTION_UNKOWN_VIVA_";
//			strTemp+=itoa(i,chartemp,10);
//			strTemp+=",\r\n";
//		}
//		else
//		{
//		strTemp+=g_pActionInfoTable->Get(i).GetName();
//		strTemp+=",\r\n";
//		}
	}
//	SetDlgItemText(IDC_EDIT3, strTemp);

	CString m_sizeout;
	m_sizeout.Format("Size:%d,Index:%d",g_pActionInfoTable->GetSize(),0);
	SetDlgItemText(IDC_STATIC2_SIZE,m_sizeout);
}

//显示信息
void CActionDlg::OnSelchangeList1() 
{
	m_InfoList.DeleteAllItems();
	CString m_TempOut;
	long m_ltemp;
	BYTE m_btemp;

	CString m_sizeout;
	m_sizeout.Format("Size:%d,Index:%d",g_pActionInfoTable->GetSize(),m_list.GetCurSel());
	SetDlgItemText(IDC_STATIC2_SIZE,m_sizeout);
	
	//////////////////////////////////////////////////////////////////////////
	m_TempOut = g_pActionInfoTable->Get(m_list.GetCurSel()).GetName();
	m_InfoList.InsertItem(0,"Name");
	m_InfoList.SetItemText(0,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_btemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetAction();
	m_TempOut.Format("%d",m_btemp);
	m_InfoList.InsertItem(1,"Action");
	m_InfoList.SetItemText(1,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetActionEffectSpriteType();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(2,"EffectSpriteType");
	m_InfoList.SetItemText(2,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetActionEffectSpriteTypeFemale();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(3,"EffectSpriteTypeFemale");
	m_InfoList.SetItemText(3,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_btemp = g_pActionInfoTable->Get(m_list.GetCurSel()).IsUseRepeatFrame();
	m_TempOut.Format("%d",m_btemp);
	m_InfoList.InsertItem(4,"IsUseRepeatFrame");
	m_InfoList.SetItemText(4,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetStartFrame(0);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(5,"StartFrame1");
	m_InfoList.SetItemText(5,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetStartFrame(1);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(6,"StartFrame2");
	m_InfoList.SetItemText(6,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetStartFrame(2);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(7,"StartFrame3");
	m_InfoList.SetItemText(7,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetCastingFrames(0);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(8,"CastingFrame1");
	m_InfoList.SetItemText(8,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetCastingFrames(1);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(9,"CastingFrame2");
	m_InfoList.SetItemText(9,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetCastingFrames(2);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(10,"CastingFrame3");
	m_InfoList.SetItemText(10,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetCastingStartFrame(0);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(11,"CastingStartFrame1");
	m_InfoList.SetItemText(11,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetCastingStartFrame(1);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(12,"CastingStartFrame2");
	m_InfoList.SetItemText(12,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////	
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetCastingStartFrame(2);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(13,"CastingStartFrame3");
	m_InfoList.SetItemText(13,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetRepeatStartFrame(0);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(14,"RepeatStartFrame1");
	m_InfoList.SetItemText(14,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetRepeatStartFrame(1);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(15,"RepeatStartFrame2");
	m_InfoList.SetItemText(15,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetRepeatStartFrame(2);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(16,"RepeatStartFrame3");
	m_InfoList.SetItemText(16,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetRepeatEndFrame(0);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(17,"RepeatEndFrame1");
	m_InfoList.SetItemText(17,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetRepeatEndFrame(1);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(18,"RepeatEndFrame2");
	m_InfoList.SetItemText(18,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetRepeatEndFrame(2);
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(19,"RepeatEndFrame3");
	m_InfoList.SetItemText(19,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetRepeatLimit();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(20,"RepeatLimit");
	m_InfoList.SetItemText(20,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_btemp = g_pActionInfoTable->Get(m_list.GetCurSel()).IsCastingEffectToSelf();
	m_TempOut.Format("%d",m_btemp);
	m_InfoList.InsertItem(21,"CastingEffectToSelf");
	m_InfoList.SetItemText(21,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetCastingActionInfo();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(22,"CastingActionInfo");
	m_InfoList.SetItemText(22,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_btemp = g_pActionInfoTable->Get(m_list.GetCurSel()).IsCastingAction();
	m_TempOut.Format("%d",m_btemp);
	m_InfoList.InsertItem(23,"CastingAction");
	m_InfoList.SetItemText(23,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_btemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetRange();
	m_TempOut.Format("%d",m_btemp);
	m_InfoList.InsertItem(24,"Range");
	m_InfoList.SetItemText(24,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetTarget();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(25,"fTarget");
	m_InfoList.SetItemText(25,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetStart();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(26,"fStart");
	m_InfoList.SetItemText(26,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetUser();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(27,"fUserType");
	m_InfoList.SetItemText(27,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetWeaponType();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(28,"fWeaponType");
	m_InfoList.SetItemText(28,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).IsAffectCurrentWeaponRange();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(29,"AffectCurrentWeaponRange");
	m_InfoList.SetItemText(29,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).IsAffectCurrentWeaponSound();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(30,"AffectCurrentWeaponSound");
	m_InfoList.SetItemText(30,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).IsAffectCurrentWeaponDelay();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(31,"AffectCurrentWeaponDelay");
	m_InfoList.SetItemText(31,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).IsAffectCurrentWeaponAction();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(32,"AffectCurrentWeaponAction");
	m_InfoList.SetItemText(32,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).IsAffectCurrentWeaponCastingStartFrame();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(33,"AffectCurrentWeaponCastingStartFrame");
	m_InfoList.SetItemText(33,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).IsAffectCurrentWeaponCastingFrames();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(34,"AffectCurrentWeaponCastingFrames");
	m_InfoList.SetItemText(34,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).IsAffectCurrentWeaponActionEffectSpriteType();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(35,"AffectCurrentWeaponActionEffectSpriteType");
	m_InfoList.SetItemText(35,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).HasOption();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(36,"fOption");
	m_InfoList.SetItemText(36,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetAffectCurrentWeaponActionInfoPlus();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(37,"PlusActionInfo");
	m_InfoList.SetItemText(37,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetDelay();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(38,"Delay");
	m_InfoList.SetItemText(38,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetValue();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(39,"Value");
	m_InfoList.SetItemText(39,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetSoundID();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(40,"SoundID");
	m_InfoList.SetItemText(40,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetMainNode();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(41,"MainNode");
	m_InfoList.SetItemText(41,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetActionResultID();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(42,"ActionResultID");
	m_InfoList.SetItemText(42,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetActionResultValue();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(43,"ActionResultValue");
	m_InfoList.SetItemText(43,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).IsAttack();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(44,"bAttack");
	m_InfoList.SetItemText(44,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetActionTarget();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(45,"fSelectCreature");
	m_InfoList.SetItemText(45,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).GetMasterySkillStep();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(46,"MasterySkillStep");
	m_InfoList.SetItemText(46,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	m_ltemp = g_pActionInfoTable->Get(m_list.GetCurSel()).IsIgnoreSkillFailDelay();
	m_TempOut.Format("%d",m_ltemp);
	m_InfoList.InsertItem(47,"bIgnoreFailDelay");
	m_InfoList.SetItemText(47,1,m_TempOut);
	//////////////////////////////////////////////////////////////////////////
	BYTE ptined = (BYTE)g_pActionInfoTable->Get(m_list.GetCurSel()).GetPacketType();
	m_apackcombo.SetCurSel(ptined);

	//////////////////////////////////////////////////////////////////////////
	m_effsta.ResetContent();
	m_TempOut.Format("%d",(int)g_pActionInfoTable->Get(m_list.GetCurSel()).GetEffectStatus());

	m_effsta.AddString(m_TempOut);
	m_effsta.SetCurSel(0);
	//////////////////////////////////////////////////////////////////////////
    //检测非常规数据
	CTypeTable<ACTION_INFO_NODE> *m_AIN = (CTypeTable<ACTION_INFO_NODE> *)&(g_pActionInfoTable->Get(m_list.GetCurSel()));
	DWORD dwAll = MAX_HEAD + (m_AIN->GetSize() * MAX_NODE);
	int j = 0;
	for(int i = m_InfoList.GetItemCount();i < dwAll;i+=MAX_NODE)
	{
		//////////////////////////////////////////////////////////////////////////
		m_ltemp = m_AIN->Get(j).EffectGeneratorID;
		m_TempOut.Format("%d",m_ltemp);
		m_InfoList.InsertItem(i,"EffectGeneratorID");
		m_InfoList.SetItemText(i,1,m_TempOut);
		//////////////////////////////////////////////////////////////////////////
		m_ltemp = m_AIN->Get(j).EffectSpriteType;
		m_TempOut.Format("%d",m_ltemp);
		m_InfoList.InsertItem(i+1,"EffectSpriteType");
		m_InfoList.SetItemText(i+1,1,m_TempOut);
		//////////////////////////////////////////////////////////////////////////
		m_ltemp = m_AIN->Get(j).bDelayNode;
		m_TempOut.Format("%d",m_ltemp);
		m_InfoList.InsertItem(i+2,"bDelayNode");
		m_InfoList.SetItemText(i+2,1,m_TempOut);
		//////////////////////////////////////////////////////////////////////////
		m_ltemp = m_AIN->Get(j).bResultTime;
		m_TempOut.Format("%d",m_ltemp);
		m_InfoList.InsertItem(i+3,"bResultTime");
		m_InfoList.SetItemText(i+3,1,m_TempOut);
		//////////////////////////////////////////////////////////////////////////
		m_ltemp = m_AIN->Get(j).bUseCoord;
		m_TempOut.Format("%d",m_ltemp);
		m_InfoList.InsertItem(i+4,"bUseCoord");
		m_InfoList.SetItemText(i+4,1,m_TempOut);
		//////////////////////////////////////////////////////////////////////////
		m_ltemp = m_AIN->Get(j).Count;
		m_TempOut.Format("%d",m_ltemp);
		m_InfoList.InsertItem(i+5,"Count");
		m_InfoList.SetItemText(i+5,1,m_TempOut);
		//////////////////////////////////////////////////////////////////////////
		m_ltemp = m_AIN->Get(j).SoundID;
		m_TempOut.Format("%d",m_ltemp);
		m_InfoList.InsertItem(i+6,"SoundID");
		m_InfoList.SetItemText(i+6,1,m_TempOut);
		//////////////////////////////////////////////////////////////////////////
		m_ltemp = m_AIN->Get(j).Step;
		m_TempOut.Format("%d",m_ltemp);
		m_InfoList.InsertItem(i+7,"Step");
		m_InfoList.SetItemText(i+7,1,m_TempOut);
		//////////////////////////////////////////////////////////////////////////
 		m_ltemp = m_AIN->Get(j).LinkCount;
 		m_TempOut.Format("%d",m_ltemp);
 		m_InfoList.InsertItem(i+8,"LinkCount");
 		m_InfoList.SetItemText(i+8,1,m_TempOut);
		///////////////l///////////////////////////////////////////////////////////
		j++;
	}
}

//销毁资源
void CActionDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (g_pActionInfoTable != NULL)
	{
		g_pActionInfoTable->Release();
		delete g_pActionInfoTable;
		g_pActionInfoTable = NULL;
	}	
}

//初始化内容
BOOL CActionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ListView_SetExtendedListViewStyle(m_InfoList.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	m_InfoList.InsertColumn(0,"Item Name",LVCFMT_LEFT,240);
	m_InfoList.InsertColumn(1,"Item Info",LVCFMT_LEFT,200,COLUMN_STYLE_EDIT);

	m_apackcombo.ResetContent();
	m_apackcombo.AddString("ACTIONINFO_PACKET_NONE");
	m_apackcombo.AddString("ACTIONINFO_PACKET_SELF");
	m_apackcombo.AddString("AACTIONINFO_PACKET_OTHER");
	m_apackcombo.AddString("ACTIONINFO_PACKET_ZONE");
	m_apackcombo.AddString("ACTIONINFO_PACKET_ITEM");
	m_apackcombo.AddString("ACTIONINFO_PACKET_BLOOD_DRAIN");
	m_apackcombo.AddString("ACTIONINFO_PACKET_THROW_BOMB");
	m_apackcombo.AddString("ACTIONINFO_PACKET_UNTRANSFORM");
	m_apackcombo.AddString("ACTIONINFO_PACKET_VISIBLE");
	m_apackcombo.AddString("ACTIONINFO_PACKET_ABSORB_SOUL");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//写出数据
void CActionDlg::OnWrite() 
{
	if (AfxMessageBox("确认修改么？",MB_YESNO) == IDYES)
	{
		ofstream m_out(FILE_AC,ios::binary);
		
		g_pActionInfoTable->SaveToFile(m_out);
		
		if (!m_aclist.IsEmpty())
		{
			POSITION pos = m_aclist.GetHeadPosition();
			
			for (int i = 0;i < m_aclist.GetCount();i++)
			{
				ToActionInfo *m_ainfo = (ToActionInfo *)m_aclist.GetNext(pos);
				m_ainfo->SaveToFile(m_out);
				
				//m_aclist.GetNext(pos)->SaveToFile(m_out);
			}

			m_out.seekp(8,ios::beg);
			DWORD dwSize = m_aclist.GetCount() + g_pActionInfoTable->GetSize();
			m_out.write((const char*)&dwSize,4);
		}
		
		m_out.close();

		AfxMessageBox("保存完成,请重起程序后读取!");
		//OnRead();
	}
}

void CActionDlg::OnAddnode() 
{
	UpdateData();
	int num = m_InfoList.GetItemCount();
	int outinfo = 0;
	CString m_TempOut;

	if (num <= 0)
	{
		AfxMessageBox("请先载入数据之后再试!");
		return;
	}

	if (num > MAX_HEAD)
	{
		//计算已存在的结构
		outinfo = num - MAX_HEAD;
		outinfo = outinfo / MAX_NODE;	
	}

	sumnum = outinfo + m_NODENUM;

	outinfo = MAX_HEAD +(sumnum * MAX_NODE);

	for (int i = m_InfoList.GetItemCount();i < outinfo;i+=9)
	{

		//////////////////////////////////////////////////////////////////////////
 		m_InfoList.InsertItem(i,"EffectGeneratorID");
 		m_InfoList.SetItemText(i,1,"0");
		//////////////////////////////////////////////////////////////////////////
 		m_InfoList.InsertItem(i+1,"EffectSpriteType");
 		m_InfoList.SetItemText(i+1,1,"0");
		//////////////////////////////////////////////////////////////////////////
 		m_InfoList.InsertItem(i+2,"bDelayNode");
 		m_InfoList.SetItemText(i+2,1,"0");
		//////////////////////////////////////////////////////////////////////////
		m_InfoList.InsertItem(i+3,"bResultTime");
 		m_InfoList.SetItemText(i+3,1,"0");
		//////////////////////////////////////////////////////////////////////////
		m_InfoList.InsertItem(i+4,"bUseCoord");
 		m_InfoList.SetItemText(i+4,1,"0");
		//////////////////////////////////////////////////////////////////////////
		m_InfoList.InsertItem(i+5,"Count");
 		m_InfoList.SetItemText(i+5,1,"0");
		//////////////////////////////////////////////////////////////////////////
		m_InfoList.InsertItem(i+6,"SoundID");
 		m_InfoList.SetItemText(i+6,1,"0");
		//////////////////////////////////////////////////////////////////////////
		m_InfoList.InsertItem(i+7,"Step");
 		m_InfoList.SetItemText(i+7,1,"0");
		//////////////////////////////////////////////////////////////////////////
		m_InfoList.InsertItem(i+8,"LinkCount");
 		m_InfoList.SetItemText(i+8,1,"0");
	}
	
}

void CActionDlg::OnAddinfo() 
{
		/* 开始设置信息 */
	CString m_NodeString;
	DWORD dwTemp = 0;
	BOOL bTemp = FALSE;
	ToActionInfo *ACINFO_A = new ToActionInfo;

	//////////////////////////////////////////////////////////////////////////
	//获得数据信息
	CGenData m_gen(m_InfoList);	

	m_NodeString = m_gen.GetListCString(E_Name);

	ACINFO_A->Set(m_NodeString,m_gen.GetListInt(E_Action),m_gen.GetListInt(E_EffectSpriteType),m_gen.GetListInt(E_Range),
		          m_gen.GetListBYTE(E_fTarget),m_gen.GetListBYTE(E_fStart),m_gen.GetListBYTE(E_Delay),m_gen.GetListInt(E_Value));

	if (m_gen.GetListBool(E_IsUseRepeatFrame))
		ACINFO_A->UseRepeatFrame();
	else
		ACINFO_A->NoUseRepeatFrame();

	for (int i = 0;i < 3;i++)
	{
		ACINFO_A->SetStartFrame(i,m_gen.GetListInt(E_StartFrame1 + i));
		ACINFO_A->SetCastingStartFrame(i,m_gen.GetListInt(E_CastingStartFrame1 + i));
		ACINFO_A->SetCastingFrames(i,m_gen.GetListInt(E_CastingFrame1 + i));
		DWORD dwTm = m_gen.GetListInt(E_RepeatStartFrame1 + i) - m_gen.GetListInt(E_RepeatEndFrame1 + i);
		dwTm++;
		ACINFO_A->SetRepeatFrame(i,m_gen.GetListInt(E_RepeatStartFrame1 + i),dwTm);
	}

	ACINFO_A->SetRepeatLimit(m_gen.GetListInt(E_RepeatLimit));
	if (m_gen.GetListBool(E_CastingEffectToSelf))
		ACINFO_A->SetCastingEffectToSelf();
	else
		ACINFO_A->SetCastingEffectToOther();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	ACINFO_A->SetCastingActionInfo(m_gen.GetListInt(E_CastingActionInfo));
	if (m_gen.GetListBool(E_CastingAction))
		ACINFO_A->SetCastingAction();
	else
		ACINFO_A->UnSetCastingAction();

	ACINFO_A->SetUser(m_gen.GetListInt(E_fUserType));
	ACINFO_A->SetWeaponType(m_gen.GetListInt(E_fWeaponType));

	if (m_gen.GetListBool(E_AffectCurrentWeaponRange))
		ACINFO_A->SetAffectCurrentWeaponRange();
	//else
		//ACINFO_A->UnSetCurrentWeapon();

	if (m_gen.GetListBool(E_AffectCurrentWeaponSound))
		ACINFO_A->SetAffectCurrentWeaponSound();
	//else
		//ACINFO_A->UnSetCurrentWeapon();

	if (m_gen.GetListBool(E_AffectCurrentWeaponDelay))
		ACINFO_A->SetAffectCurrentWeaponDelay();
	//else
		//ACINFO_A->UnSetCurrentWeapon();

	if (m_gen.GetListBool(E_bAttack))
		ACINFO_A->SetAttack();
	else
		ACINFO_A->UnSetAttack();

	ACINFO_A->SetPacketType((ACTIONINFO_PACKET)m_apackcombo.GetCurSel());
	CString cszeff;
	m_effsta.GetWindowText(cszeff);
	DWORD dwEff = atoi(cszeff);
	ACINFO_A->SetEffectStatus((EFFECTSTATUS)dwEff);


	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	ACINFO_A->SetSoundID(m_gen.GetListInt(E_SoundID));

	ACINFO_A->SetMainNode(m_gen.GetListInt(E_MainNode));

	ACINFO_A->SetAffectCurrentWeaponActionInfoPlus(m_gen.GetListInt(E_PlusActionInfo));

	ACINFO_A->SetActionResult(m_gen.GetListInt(E_ActionResultID),m_gen.GetListInt(E_ActionResultValue));

	ACINFO_A->SetSelectCreature(m_gen.GetListBYTE(E_fSelectCreature));

	ACINFO_A->SetMasterySkillStep(m_gen.GetListBYTE(E_MasterySkillStep));

	ACINFO_A->SetSkillFailDelay(m_gen.GetListBool(E_bIgnoreFailDelay));
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	CTypeTable<ACTION_INFO_NODE> *m_AIN = (CTypeTable<ACTION_INFO_NODE> *)(ACINFO_A);

	if (m_InfoList.GetItemCount() > MAX_HEAD)
	{
		DWORD allinfo = m_InfoList.GetItemCount() - MAX_HEAD;
		allinfo = allinfo / MAX_NODE;
		m_AIN->Init(allinfo);

		dwTemp = 0;
		for (int i = MAX_HEAD;i < m_InfoList.GetItemCount();i+=9)
		{
			WORD *dwOINFO = &(m_AIN->Get(dwTemp).EffectGeneratorID);
			m_AIN->Get(dwTemp).EffectGeneratorID = m_gen.GetListInt(i);
			m_AIN->Get(dwTemp).EffectSpriteType = m_gen.GetListInt(i+1);
			m_AIN->Get(dwTemp).bDelayNode = m_gen.GetListBool(i+2);
			m_AIN->Get(dwTemp).bResultTime = m_gen.GetListBool(i+3);
			m_AIN->Get(dwTemp).bUseCoord = m_gen.GetListBool(i+4);
			m_AIN->Get(dwTemp).Count = m_gen.GetListInt(i+5);
			m_AIN->Get(dwTemp).SoundID = m_gen.GetListInt(i+6);
			m_AIN->Get(dwTemp).Step = m_gen.GetListInt(i+7);
			m_AIN->Get(dwTemp).LinkCount = m_gen.GetListInt(i+8);

			dwTemp++;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	m_aclist.AddTail(ACINFO_A);

}

//修改信息
void CActionDlg::OnMod() 
{
	/* 开始设置信息 */
	CString m_NodeString;
	DWORD dwTemp = 0;
	BOOL bTemp = FALSE;
	ToActionInfo *ACINFO_A = (ToActionInfo *)&(g_pActionInfoTable->Get(m_list.GetCurSel()));

	//////////////////////////////////////////////////////////////////////////
	//获得数据信息
	CGenData m_gen(m_InfoList);	

	m_NodeString = m_gen.GetListCString(E_Name);

	ACINFO_A->Set(m_NodeString,m_gen.GetListInt(E_Action),m_gen.GetListInt(E_EffectSpriteType),m_gen.GetListInt(E_Range),
		          m_gen.GetListBYTE(E_fTarget),m_gen.GetListBYTE(E_fStart),m_gen.GetListBYTE(E_Delay),m_gen.GetListInt(E_Value));

	if (m_gen.GetListBool(E_IsUseRepeatFrame))
		ACINFO_A->UseRepeatFrame();
	else
		ACINFO_A->NoUseRepeatFrame();

	for (int i = 0;i < 3;i++)
	{
		ACINFO_A->SetStartFrame(i,m_gen.GetListInt(E_StartFrame1 + i));
		ACINFO_A->SetCastingStartFrame(i,m_gen.GetListInt(E_CastingStartFrame1 + i));
		ACINFO_A->SetCastingFrames(i,m_gen.GetListInt(E_CastingFrame1 + i));
		DWORD dwTm = m_gen.GetListInt(E_RepeatStartFrame1 + i) - m_gen.GetListInt(E_RepeatEndFrame1 + i);
		dwTm++;
		ACINFO_A->SetRepeatFrame(i,m_gen.GetListInt(E_RepeatStartFrame1 + i),dwTm);
	}

	ACINFO_A->SetRepeatLimit(m_gen.GetListInt(E_RepeatLimit));
	if (m_gen.GetListBool(E_CastingEffectToSelf))
		ACINFO_A->SetCastingEffectToSelf();
	else
		ACINFO_A->SetCastingEffectToOther();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	ACINFO_A->SetCastingActionInfo(m_gen.GetListInt(E_CastingActionInfo));
	if (m_gen.GetListBool(E_CastingAction)) 
		ACINFO_A->SetCastingAction();
	else
		ACINFO_A->UnSetCastingAction();

	ACINFO_A->SetUser(m_gen.GetListInt(E_fUserType));
	ACINFO_A->SetWeaponType(m_gen.GetListInt(E_fWeaponType));

	if (m_gen.GetListBool(E_AffectCurrentWeaponRange))
		ACINFO_A->SetAffectCurrentWeaponRange();
	//else
		//ACINFO_A->UnSetCurrentWeapon();

	if (m_gen.GetListBool(E_AffectCurrentWeaponSound))
		ACINFO_A->SetAffectCurrentWeaponSound();
	//else
		//ACINFO_A->UnSetCurrentWeapon();

	if (m_gen.GetListBool(E_AffectCurrentWeaponDelay))
		ACINFO_A->SetAffectCurrentWeaponDelay();
	//else
		//ACINFO_A->UnSetCurrentWeapon();

	if (m_gen.GetListBool(E_bAttack))
		ACINFO_A->SetAttack();
	else
		ACINFO_A->UnSetAttack();

	ACINFO_A->SetPacketType((ACTIONINFO_PACKET)m_apackcombo.GetCurSel());
	CString cszeff;
	m_effsta.GetWindowText(cszeff);
	DWORD dwEff = atoi(cszeff);
	ACINFO_A->SetEffectStatus((EFFECTSTATUS)dwEff);

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	ACINFO_A->SetSoundID(m_gen.GetListInt(E_SoundID));

	ACINFO_A->SetMainNode(m_gen.GetListInt(E_MainNode));

	ACINFO_A->SetAffectCurrentWeaponActionInfoPlus(m_gen.GetListInt(E_PlusActionInfo));

	ACINFO_A->SetActionResult(m_gen.GetListInt(E_ActionResultID),m_gen.GetListInt(E_ActionResultValue));

	ACINFO_A->SetSelectCreature(m_gen.GetListBYTE(E_fSelectCreature));

	ACINFO_A->SetMasterySkillStep(m_gen.GetListBYTE(E_MasterySkillStep));

	ACINFO_A->SetSkillFailDelay(m_gen.GetListBool(E_bIgnoreFailDelay));
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	CTypeTable<ACTION_INFO_NODE> *m_AIN = (CTypeTable<ACTION_INFO_NODE> *)&(g_pActionInfoTable->Get(m_list.GetCurSel()));
	if (m_InfoList.GetItemCount() > MAX_HEAD)
	{
		DWORD allinfo = m_InfoList.GetItemCount() - MAX_HEAD;
		allinfo = allinfo / MAX_NODE;
		if (allinfo > m_AIN->GetSize())
		{
			m_AIN->Init(allinfo);
		}

		dwTemp = 0;
		for (int i = MAX_HEAD;i < m_InfoList.GetItemCount();i+=9)
		{
			m_AIN->Get(dwTemp).EffectGeneratorID = m_gen.GetListInt(i);
			m_AIN->Get(dwTemp).EffectSpriteType = m_gen.GetListInt(i+1);
			m_AIN->Get(dwTemp).bDelayNode = m_gen.GetListBool(i+2);
			m_AIN->Get(dwTemp).bResultTime = m_gen.GetListBool(i+3);
			m_AIN->Get(dwTemp).bUseCoord = m_gen.GetListBool(i+4);
			m_AIN->Get(dwTemp).Count = m_gen.GetListInt(i+5);
			m_AIN->Get(dwTemp).SoundID = m_gen.GetListInt(i+6);
			m_AIN->Get(dwTemp).Step = m_gen.GetListInt(i+7);
			m_AIN->Get(dwTemp).LinkCount = m_gen.GetListInt(i+8);

			dwTemp++;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	
}
