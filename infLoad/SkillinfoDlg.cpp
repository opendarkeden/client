// SkillinfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "infLoad.h"
#include "SkillinfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkillinfoDlg dialog
CList<SKILLINFO_NODE,SKILLINFO_NODE&> m_SKILLINFOList;


CSkillinfoDlg::CSkillinfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillinfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkillinfoDlg)
	m_Find = _T("");
	m_minDamage = 0;
	m_maxDamage = 0;
	m_minDelay = 0;
	m_maxDelay = 0;
	m_minDuration = 0;
	m_maxDuration = 0;
	m_mp = 0;
	m_maxRange = 0;
	m_maxExp = 0;
	m_minRange = 0;
	m_SkillPoint = 0;
	m_LevelUpPoint = 0;
	m_Water = 0;
	m_Earth = 0;
	m_Wind = 0;
	m_Fire = 0;
	m_sum = 0;
	m_Wristlet = 0;
	m_Stone1 = 0;
	m_Stone2 = 0;
	m_Stone3 = 0;
	m_EDInt = 0;
	m_CanDelete = FALSE;
	m_HName = _T("");
	m_Name = _T("");
	m_ismod = FALSE;
	m_TypeNum = 0;
	m_Stone4 = 0;
	//}}AFX_DATA_INIT
}


void CSkillinfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkillinfoDlg)
	DDX_Control(pDX, IDC_LIST2, m_TypeList);
	DDX_Control(pDX, IDC_COMBO1, m_Elist);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_Find);
	DDX_Text(pDX, IDC_EDIT3, m_minDamage);
	DDX_Text(pDX, IDC_EDIT4, m_maxDamage);
	DDX_Text(pDX, IDC_EDIT5, m_minDelay);
	DDX_Text(pDX, IDC_EDIT6, m_maxDelay);
	DDX_Text(pDX, IDC_EDIT7, m_minDuration);
	DDX_Text(pDX, IDC_EDIT11, m_maxDuration);
	DDX_Text(pDX, IDC_EDIT8, m_mp);
	DDX_Text(pDX, IDC_EDIT10, m_maxRange);
	DDX_Text(pDX, IDC_EDIT12, m_maxExp);
	DDX_Text(pDX, IDC_EDIT9, m_minRange);
	DDX_Text(pDX, IDC_EDIT13, m_SkillPoint);
	DDX_Text(pDX, IDC_EDIT14, m_LevelUpPoint);
	DDX_Text(pDX, IDC_EDIT15, m_Water);
	DDX_Text(pDX, IDC_EDIT16, m_Earth);
	DDX_Text(pDX, IDC_EDIT17, m_Wind);
	DDX_Text(pDX, IDC_EDIT18, m_Fire);
	DDX_Text(pDX, IDC_EDIT19, m_sum);
	DDX_Text(pDX, IDC_EDIT20, m_Wristlet);
	DDX_Text(pDX, IDC_EDIT21, m_Stone1);
	DDX_Text(pDX, IDC_EDIT22, m_Stone2);
	DDX_Text(pDX, IDC_EDIT23, m_Stone3);
	DDX_Text(pDX, IDC_EDIT24, m_EDInt);
	DDX_Text(pDX, IDC_EDIT25, m_CanDelete);
	DDX_Text(pDX, IDC_EDIT26, m_HName);
	DDX_Text(pDX, IDC_EDIT27, m_Name);
	DDX_Text(pDX, IDC_EDIT28, m_TypeNum);
	DDX_Text(pDX, IDC_EDIT29, m_Stone4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkillinfoDlg, CDialog)
	//{{AFX_MSG_MAP(CSkillinfoDlg)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_READ, OnRead)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MOD, OnMod)
	ON_BN_CLICKED(IDC_ADDINFO, OnAddinfo)
	ON_BN_CLICKED(IDC_WRITE, OnWrite)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_LBN_SELCHANGE(IDC_LIST2, OnSelchangeList2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillinfoDlg message handlers
CMySkillInfo *g_pSkillInfoTable2;
//选择一个信息
void CSkillinfoDlg::OnSelchangeList1() 
{
	SKILLINFO_NODE m_node = g_pSkillInfoTable2->Get(m_list.GetCurSel());
	m_Elist.SetCurSel(g_pSkillInfoTable2->Get(m_list.GetCurSel()).DomainType);
	m_minDelay = m_node.minDelay;
	m_HName = m_node.GetHName();
	m_Name = m_node.GetName();
	m_maxDelay = m_node.maxDelay;
	m_maxDuration = m_node.maxDuration;
	m_minDuration = m_node.minDuration;
	m_minDamage = m_node.minDamage;
	m_maxDamage = m_node.maxDamage;
	m_mp = m_node.GetMP();
	m_minRange = m_node.minRange;
	m_maxRange = m_node.maxRange;
	m_maxExp = m_node.maxExp;

	if (m_node.DomainType == SKILLDOMAIN_OUSTERS)
	{
		m_SkillPoint = m_node.SkillPoint;
		m_LevelUpPoint = m_node.LevelUpPoint;
		m_Wind = m_node.Wind;
		m_Earth = m_node.Earth;
		m_Water = m_node.Water;
		m_Fire = m_node.Fire;
		m_sum = m_node.Sum;
		m_Wristlet = m_node.Wristlet;	

		m_Stone1 = m_node.Stone1;
		m_Stone2 = m_node.Stone2;
		m_Stone3 = m_node.Stone3;
		m_Stone4 = m_node.Stone4;

		m_CanDelete = m_node.CanDelete;
		m_EDInt = m_node.ElementalDomain;

		SKILLINFO_NODE::SKILLTYPE_LIST::const_iterator skilid = m_node.SkillTypeList.begin();

		m_TypeList.ResetContent();	
		while (skilid != m_node.SkillTypeList.end())
		{
			int idinfo = *skilid;
			CString m_tempit;

			m_tempit.Format("%d",idinfo);

			m_TypeList.AddString(m_tempit);

			skilid++;
		}
		
	}

	CString m_outsize;

	m_outsize.Format("Size:%d,Index:%d",g_pSkillInfoTable2->GetSize(),m_list.GetCurSel());

	SetDlgItemText(IDC_SIZESI,m_outsize);

	UpdateData(FALSE);
}

//LoadFile
void CSkillinfoDlg::OnRead() 
{
	//初始化内存
	m_list.ResetContent();
	if (g_pSkillInfoTable2 != NULL)
	{
		delete g_pSkillInfoTable2;
		g_pSkillInfoTable2 = NULL;
	}

	g_pSkillInfoTable2 = new CMySkillInfo;

	//LAOD FILE
	class ifstream serverSkillInfoFile;
	if (!FileOpenBinary(FILE_SI, serverSkillInfoFile))
		return;
	g_pSkillInfoTable2->LoadSKILLInfoFile( serverSkillInfoFile );
	serverSkillInfoFile.close();

	DrawAll();
	m_list.SetCurSel(0);
	OnSelchangeList1();
}

void CSkillinfoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (g_pSkillInfoTable2 != NULL)
	{
		g_pSkillInfoTable2->Release();
		delete g_pSkillInfoTable2;
		g_pSkillInfoTable2 = NULL;
	}
	
}

int CSkillinfoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CSkillinfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//初始化列表
	m_ismod = FALSE;
	m_Stone4 = 0;
	m_Elist.Clear();
	m_Elist.AddString("SKILLDOMAIN_BLADE");
	m_Elist.AddString("SKILLDOMAIN_SWORD");
	m_Elist.AddString("SKILLDOMAIN_GUN");
	m_Elist.AddString("SKILLDOMAIN_HEAL");
	m_Elist.AddString("SKILLDOMAIN_ENCHANT");
	m_Elist.AddString("SKILLDOMAIN_ETC");
	m_Elist.AddString("SKILLDOMAIN_VAMPIRE");
	m_Elist.AddString("SKILLDOMAIN_OUSTERS");
	m_Elist.AddString("MAX_SKILLDOMAIN");

	//////////////////////////////////////////////////////////////////////////
	//初始化列表 
// 	ListView_SetExtendedListViewStyle(m_InfoList.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
// 	
// 	m_InfoList.InsertColumn(0,"Item Name",LVCFMT_LEFT,140);
// 	m_InfoList.InsertColumn(1,"Item Info",LVCFMT_LEFT,180,COLUMN_STYLE_EDIT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//显示信息
void CSkillinfoDlg::DrawAll()
{
	for(int i = 0;i < g_pSkillInfoTable2->GetSize();i++)
	{
		if (g_pSkillInfoTable2->Get(i).GetHName() == NULL)
			m_list.AddString("Not Defined ...");
		else
			m_list.AddString(g_pSkillInfoTable2->Get(i).GetHName());
	}
	
	CString m_outsize;

	m_outsize.Format("Size:%d,Index:%d",g_pSkillInfoTable2->GetSize(),m_list.GetCurSel());

	SetDlgItemText(IDC_SIZESI,m_outsize);
}

void CSkillinfoDlg::OnCheck1() 
{
	m_ismod = !m_ismod;
}

//增加
void CSkillinfoDlg::OnAdd() 
{
	UpdateData();
	SKILLINFO_NODE *m_node = &(g_pSkillInfoTable2->Get(m_list.GetCurSel()));
	if (m_Elist.GetCurSel() == SKILLDOMAIN_OUSTERS)
	{
		if (m_ismod)
			m_node->SkillTypeList.push_back(m_TypeNum);

		CString cszout;

		cszout.Format("%d",m_TypeNum);
		m_TypeList.AddString(cszout);
	}
	else
		AfxMessageBox("DomainType 类型不正确!");
}

//修改数据
void CSkillinfoDlg::OnMod() 
{
	UpdateData();
	SKILLINFO_NODE *m_node = &(g_pSkillInfoTable2->Get(m_list.GetCurSel()));
	
	m_node->minDamage = m_minDamage;
	m_node->maxDamage = m_maxDamage;
	m_node->DomainType = m_Elist.GetCurSel();
	m_node->minRange = m_minRange;
	m_node->maxRange = m_maxRange;
	m_node->minDelay = m_minDelay;
	m_node->maxDelay = m_maxDelay;
	m_node->minDuration = m_minDuration;
	m_node->maxDuration = m_maxDuration;
	m_node->SetMP(m_mp);
	m_node->maxExp = m_maxExp;
	m_node->Set(0,m_Name,0,0,0,m_HName);
	if (m_node->DomainType == SKILLDOMAIN_OUSTERS)
	{
		m_node->LevelUpPoint = m_LevelUpPoint;
		m_node->SkillPoint = m_SkillPoint;
		m_node->CanDelete = m_CanDelete;
		m_node->Fire = m_Fire;
		m_node->Earth = m_Earth;
		m_node->Water = m_Water;
		m_node->Wind = m_Wind;
		m_node->Sum = m_sum;
		m_node->Wristlet = m_Wristlet;
		m_node->ElementalDomain = m_EDInt;
		m_node->Stone1 = m_Stone1;
		m_node->Stone2 = m_Stone2;
		m_node->Stone3 = m_Stone3;
		m_node->Stone4 = m_Stone4;
	}
}

//添加信息
void CSkillinfoDlg::OnAddinfo() 
{
	UpdateData();
	SKILLINFO_NODE m_node;
	
	m_node.minDamage = m_minDamage;
	m_node.maxDamage = m_maxDamage;
	m_node.DomainType = m_Elist.GetCurSel();
	m_node.minRange = m_minRange;
	m_node.maxRange = m_maxRange;
	m_node.minDelay = m_minDelay;
	m_node.maxDelay = m_maxDelay;
	m_node.minDuration = m_minDuration;
	m_node.maxDuration = m_maxDuration;
	m_node.SetMP(m_mp);
	m_node.maxExp = m_maxExp;
	m_node.Set(0,m_Name,0,0,0,m_HName);
	if (m_node.DomainType == SKILLDOMAIN_OUSTERS)
	{
		m_node.LevelUpPoint = m_LevelUpPoint;
		m_node.SkillPoint = m_SkillPoint;
		m_node.CanDelete = m_CanDelete;
		m_node.Fire = m_Fire;
		m_node.Earth = m_Earth;
		m_node.Water = m_Water;
		m_node.Wind = m_Wind;
		m_node.Sum = m_sum;
		m_node.Wristlet = m_Wristlet;
		m_node.ElementalDomain = m_EDInt;
		m_node.Stone1 = m_Stone1;
		m_node.Stone2 = m_Stone2;
		m_node.Stone3 = m_Stone3;
		m_node.Stone4 = m_Stone4;

		for (int i = 0;i < m_TypeList.GetCount();i++)
		{
			CString PuckString;
			m_TypeList.GetText(i,PuckString);
			int iout = atoi(PuckString);
			m_node.SkillTypeList.push_back(iout);
		}
	}

	m_SKILLINFOList.AddTail(m_node);
}

void CSkillinfoDlg::OnWrite() 
{
	if (AfxMessageBox("确认保存?",MB_YESNO) == IDYES)
	{
		ofstream m_out(FILE_SI,ios::binary);

		g_pSkillInfoTable2->SaveAllToFile(m_out);

		if (!m_SKILLINFOList.IsEmpty())
		{
			POSITION pos = m_SKILLINFOList.GetHeadPosition();
			for (int i = 0;i <  m_SKILLINFOList.GetCount();i++)
			{
				DWORD sized = g_pSkillInfoTable2->dwSize + i;
				m_out.write((const char*)&sized,4);
				SKILLINFO_NODE m_temp = m_SKILLINFOList.GetNext(pos);
				g_pSkillInfoTable2->SaveOneToFile(&m_temp,m_out);
			}

			m_out.seekp(ios::beg);
			DWORD dwSize = m_SKILLINFOList.GetCount() + g_pSkillInfoTable2->dwSize;
			m_out.write((const char*)&dwSize,4);
		}

		m_out.close();

		OnRead();
	}
}

void CSkillinfoDlg::OnDelete() 
{
	UpdateData();
	SKILLINFO_NODE *m_node = &(g_pSkillInfoTable2->Get(m_list.GetCurSel()));
	if (m_Elist.GetCurSel() == SKILLDOMAIN_OUSTERS)
	{
		if (m_ismod)
			m_node->SkillTypeList.remove(m_TypeNum);

		m_TypeList.DeleteString(m_TypeList.GetCurSel());
	}
	else
		AfxMessageBox("DomainType 类型不正确!");
	
}

void CSkillinfoDlg::OnSelchangeList2() 
{
	CString PuckString;
	m_TypeList.GetText(m_TypeList.GetCurSel(),PuckString);
	m_TypeNum = atoi(PuckString);
}
