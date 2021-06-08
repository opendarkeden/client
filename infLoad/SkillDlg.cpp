// SkillDlg.cpp : implementation file
//

#include "stdafx.h"
#include "infLoad.h"
#include "SkillDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkillDlg dialog
CList<SKILLINFO_NODE,SKILLINFO_NODE&> m_SKILLList;

CSkillDlg::CSkillDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkillDlg)
	m_level = 0;
	m_Name = _T("");
	m_x = 0;
	m_passive = FALSE;
	m_bActive = FALSE;
	m_HName = _T("");
	m_mp = 0;
	m_y = 0;
	m_Findit = _T("");
	m_spriteid = 0;
	m_ismod = FALSE;
	m_NextNum = 0;
	//}}AFX_DATA_INIT
}


void CSkillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkillDlg)
	DDX_Control(pDX, IDC_LIST2, m_NextList);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_COMBO1, m_ELEMENTAL_DOMAIN);
	DDX_Text(pDX, IDC_EDIT2, m_level);
	DDX_Text(pDX, IDC_EDIT3, m_Name);
	DDX_Text(pDX, IDC_EDIT4, m_x);
	DDX_Text(pDX, IDC_EDIT10, m_passive);
	DDX_Text(pDX, IDC_EDIT12, m_bActive);
	DDX_Text(pDX, IDC_EDIT6, m_HName);
	DDX_Text(pDX, IDC_EDIT9, m_mp);
	DDX_Text(pDX, IDC_EDIT8, m_y);
	DDX_Text(pDX, IDC_EDIT1, m_Findit);
	DDX_Text(pDX, IDC_EDIT5, m_spriteid);
	DDX_Check(pDX, IDC_CHECK1, m_ismod);
	DDX_Text(pDX, IDC_EDIT13, m_NextNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkillDlg, CDialog)
	//{{AFX_MSG_MAP(CSkillDlg)
	ON_BN_CLICKED(IDC_READ, OnRead)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_LBN_SELCHANGE(IDC_LIST2, OnSelchangeList2)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_MOD, OnMod)
	ON_BN_CLICKED(IDC_ADDINFO, OnAddinfo)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillDlg message handlers

BOOL CSkillDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ELEMENTAL_DOMAIN.Clear();
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_NULL");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_APPRENTICE");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_ADEPT");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_EXPERT");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_MASTER");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_GRAND_MASTER");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_SLAYER_BLADE_ADVANCEMENT");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_SLAYER_SWORD_ADVANCEMENT");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_SLAYER_GUN_BOMB_ADVANCEMENT");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_SLAYER_HEAL_ADVANCEMENT");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_SLAYER_ENCHANT_ADVANCEMENT");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_VAMPIRE_POISON");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_VAMPIRE_ACID");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_VAMPIRE_CURSE");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_VAMPIRE_SUMMON");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_VAMPIRE_BLOOD");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_VAMPIRE_INNATE");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_VAMPIRE_ADVANCEMENT");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_OUSTERS_COMBAT");

	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_OUSTERS_ELEMENTAL");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_OUSTERS_ETC");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_OUSTERS_COMMON_ADVANCEMENT");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_OUSTERS_COMBAT_ADVANCEMENT");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_OUSTERS_EARTH_ADVANCEMENT");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_OUSTERS_FIRE_ADVANCEMENT");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_OUSTERS_WATER_ADVANCEMENT");
	m_ELEMENTAL_DOMAIN.AddString("SKILL_STEP_ETC");

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSkillDlg::OnRead() 
{
	//初始化内存
	m_list.ResetContent();
	if (g_pSkillInfoTable != NULL)
	{
		delete g_pSkillInfoTable;
		g_pSkillInfoTable = NULL;
	}

	g_pSkillInfoTable = new MSkillInfoTable;

	//LAOD FILE
	class ifstream skillTable2;
	if (!FileOpenBinary(FILE_ST, skillTable2))
		return;
	g_pSkillInfoTable->LoadFromFile(skillTable2);
	skillTable2.close();

	//OutAll
	DrawAll();

	m_list.SetCurSel(0);
	OnSelchangeList1();
}

//显示所有信息
void CSkillDlg::DrawAll()
{
	//循环显示
	for (int i = 0;i < g_pSkillInfoTable->GetSize();i++)
	{
		if (g_pSkillInfoTable->Get(i).GetHName() == NULL)
			m_list.AddString(" Not Defined ("")");
		else
			m_list.AddString(g_pSkillInfoTable->Get(i).GetHName());
	}

	CString m_outsize;

	m_outsize.Format("Size:%d,Index:%d",g_pSkillInfoTable->GetSize(),0);

	SetDlgItemText(IDC_SIZEOY,m_outsize);
}

//选择某个数据
void CSkillDlg::OnSelchangeList1() 
{
	m_NextList.ResetContent();
	SKILLINFO_NODE m_Skill = g_pSkillInfoTable->Get(m_list.GetCurSel());
	m_level = m_Skill.GetLevel();
	m_Name = m_Skill.GetName();
	m_HName = m_Skill.GetHName();
	m_x = m_Skill.GetX();
	m_y = m_Skill.GetY();
	m_spriteid = m_Skill.GetSpriteID();
	m_mp = m_Skill.GetMP();
	m_bActive = m_Skill.IsActive();
	m_passive = m_Skill.IsPassive();
	m_ELEMENTAL_DOMAIN.SetCurSel(m_Skill.GetSkillStep());
	SKILLINFO_NODE::SKILLID_LIST m_sklist = m_Skill.GetNextSkillList();

	SKILLINFO_NODE::SKILLID_LIST::const_iterator iSkillID = m_sklist.begin();

	while (iSkillID != m_sklist.end())
	{
		TYPE_ACTIONINFO id = *iSkillID;
		CString m_stemp;
		m_stemp.Format("%d",id);
		m_NextList.AddString(m_stemp);
		iSkillID++;
	}

	CString m_outsize;

	m_outsize.Format("Size:%d,Index:%d",g_pSkillInfoTable->GetSize(),m_list.GetCurSel());

	SetDlgItemText(IDC_SIZEOY,m_outsize);
	UpdateData(FALSE);
}

void CSkillDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (g_pSkillInfoTable != NULL)
	{
		delete g_pSkillInfoTable;
		g_pSkillInfoTable = NULL;
	}
	
}

void CSkillDlg::OnCheck1() 
{
	m_ismod = !m_ismod;
}

//选择列表后UPDATE数据到EDIT
void CSkillDlg::OnSelchangeList2() 
{
	CString csz_temp;
	m_NextList.GetText(m_NextList.GetCurSel(),csz_temp);
	
	m_NextNum = atoi(csz_temp);
	
	UpdateData(FALSE);
}

//保存数据
void CSkillDlg::OnSave() 
{
	if (AfxMessageBox("确认要保存么?",MB_YESNO) == IDYES)
	{
		ofstream m_out(FILE_ST,ios::binary);
		g_pSkillInfoTable->SaveToFile(m_out);

		if (!m_SKILLList.IsEmpty())
		{
			POSITION pos = m_SKILLList.GetHeadPosition();
			for (int i = 0;i < m_SKILLList.GetCount();i++)
			{
				SKILLINFO_NODE m_skill = m_SKILLList.GetNext(pos);
				m_skill.SaveToFile(m_out);
			}

			m_out.seekp(ios::beg);
			int dwsize = g_pSkillInfoTable->GetSize() + m_SKILLList.GetCount();
			m_out.write((const char*)&dwsize,4);
			
			m_SKILLList.RemoveAll();
		}

		m_out.close();

		OnRead();
	}
	
}

//修改数据
void CSkillDlg::OnMod() 
{
	UpdateData();
	SKILLINFO_NODE *SN_TEMP = &(g_pSkillInfoTable->Get(m_list.GetCurSel()));

	SN_TEMP->Set(m_level,m_Name,m_x,m_y,m_spriteid,m_HName);
	if (m_passive)
		SN_TEMP->SetPassive();
	else
		SN_TEMP->UnSetPassive();

	if (m_bActive)
		SN_TEMP->SetActive();
	else
		SN_TEMP->UnSetActive();

	SN_TEMP->SetMP(m_mp);

	SN_TEMP->SetSkillStep((SKILL_STEP)m_ELEMENTAL_DOMAIN.GetCurSel());
}

//增加数据
void CSkillDlg::OnAddinfo() 
{
	UpdateData();
	SKILLINFO_NODE SN_TEMP;
	CString cszNextSkill;
	SN_TEMP.Set(m_level,m_Name,m_x,m_y,m_spriteid,m_HName);
	if (m_passive)
		SN_TEMP.SetPassive();
	else
		SN_TEMP.UnSetPassive();

	if (m_bActive)
		SN_TEMP.SetActive();
	else
		SN_TEMP.UnSetActive();

	SN_TEMP.SetMP(m_mp);

	SN_TEMP.SetSkillStep((SKILL_STEP)m_ELEMENTAL_DOMAIN.GetCurSel());
	SKILLINFO_NODE::SKILLID_LIST *m_sklist = (SKILLINFO_NODE::SKILLID_LIST *)&(SN_TEMP.GetNextSkillList());
	
	for (int i =0;i < m_NextList.GetCount();i++)
	{
		m_NextList.GetText(i,cszNextSkill);
		ACTIONINFO atTemp = (ACTIONINFO)atoi(cszNextSkill);
		m_sklist->push_back(atTemp);
	}


	m_SKILLList.AddTail(SN_TEMP);

}

//删除
void CSkillDlg::OnDelete() 
{
	UpdateData();
	if (m_ismod)
	{
		SKILLINFO_NODE *SN_TEMP = &(g_pSkillInfoTable->Get(m_list.GetCurSel()));
		SKILLINFO_NODE::SKILLID_LIST *m_sklist = (SKILLINFO_NODE::SKILLID_LIST *)&(SN_TEMP->GetNextSkillList());
		ACTIONINFO m_ACINFO = (ACTIONINFO)m_NextNum;
		m_sklist->remove(m_ACINFO);
	}

	m_NextList.DeleteString(m_NextList.GetCurSel());
}

void CSkillDlg::OnAdd() 
{
	UpdateData();
	if (m_ismod)
	{
		SKILLINFO_NODE *SN_TEMP = &(g_pSkillInfoTable->Get(m_list.GetCurSel()));
		SKILLINFO_NODE::SKILLID_LIST *m_sklist = (SKILLINFO_NODE::SKILLID_LIST *)&(SN_TEMP->GetNextSkillList());
		ACTIONINFO m_ACINFO = (ACTIONINFO)m_NextNum;
		m_sklist->push_back(m_ACINFO);
	}

	CString m_info;
	m_info.Format("%d",m_NextNum);
	m_NextList.AddString(m_info);
}
