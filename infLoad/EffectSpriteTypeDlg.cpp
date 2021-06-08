// EffectSpriteTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "infLoad.h"
#include "EffectSpriteTypeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EffectSpriteTypeDlg dialog
CList<EFFECTSPRITETYPETABLE_INFO,EFFECTSPRITETYPETABLE_INFO&> m_ESTList;

EffectSpriteTypeDlg::EffectSpriteTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(EffectSpriteTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(EffectSpriteTypeDlg)
	m_RepeatFrame = FALSE;
	m_FindNum = 0;
	m_PAIRINFO = 0;
	m_ismod = FALSE;
	//}}AFX_DATA_INIT
}


void EffectSpriteTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EffectSpriteTypeDlg)
	DDX_Control(pDX, IDC_LIST2, m_InfoList);
	DDX_Control(pDX, IDC_LISTPairFrameIDList, m_PairFrameIDList);
	DDX_Control(pDX, IDC_COMBO1, m_comBitType);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT_FIND, m_FindNum);
	DDX_Text(pDX, IDC_PAIR, m_PAIRINFO);
	DDX_Check(pDX, IDC_CHECK1, m_ismod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EffectSpriteTypeDlg, CDialog)
	//{{AFX_MSG_MAP(EffectSpriteTypeDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnRead)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_EN_CHANGE(IDC_EDIT_FIND, OnChangeEditFind)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_WRITE, OnWrite)
	ON_BN_CLICKED(IDC_MOD, OnMod)
	ON_LBN_SELCHANGE(IDC_LISTPairFrameIDList, OnSelchangeLISTPairFrameIDList)
	ON_BN_CLICKED(IDC_ADDINFO, OnAddinfo)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EffectSpriteTypeDlg message handlers

void EffectSpriteTypeDlg::OnRead() 
{
	Init();

	//////////////////////////////////////////////////////////////////////////
	// Load EffectSprinteType.inf
	//////////////////////////////////////////////////////////////////////////

	class ifstream effectSpriteTypeTable2;//(g_pFileDef->getProperty("FILE_INFO_EFFECTSPRITETYPE"), ios::binary);
	if (!FileOpenBinary(FILE_EST, effectSpriteTypeTable2))
		return;
	(*g_pEffectSpriteTypeTable).LoadFromFile(effectSpriteTypeTable2);
	effectSpriteTypeTable2.close();

	DrawAll();
	m_list.SetCurSel(0);
}

void EffectSpriteTypeDlg::Init()
{
	if (g_pEffectSpriteTypeTable !=NULL)
	{
		delete g_pEffectSpriteTypeTable;
		g_pEffectSpriteTypeTable=NULL;
	}
	g_pEffectSpriteTypeTable = new EFFECTSPRITETYPE_TABLE;
	//清除文件列表
	m_list.ResetContent();
}

void EffectSpriteTypeDlg::DrawAll()
{
	char szTmp[300];
	int a=0,b=0,c=0,d=0;

	for (int i=0;i<g_pEffectSpriteTypeTable->GetSize();i++)
	{
		

		if ( g_pEffectSpriteTypeTable->Get(i).BltType ==0 ) 
		{
			sprintf(szTmp,"%04d (NORMAL)",i);
			a++;
		}
		if ( g_pEffectSpriteTypeTable->Get(i).BltType ==1 ) 
		{
			sprintf(szTmp,"%04d (EFFECT)",i);
			b++;
		}
		if ( g_pEffectSpriteTypeTable->Get(i).BltType ==2 ) 
		{
			sprintf(szTmp,"%04d (SHADOW)",i);
			c++;
		}
		if ( g_pEffectSpriteTypeTable->Get(i).BltType ==3 ) 
		{
			sprintf(szTmp,"%04d (SCREEN)",i);
			d++;
		}

		m_list.AddString(szTmp);
	}
	sprintf(szTmp,"Size:%d  NORMAL:%d  EFFECT:%d  SHADOW:%d  SCREEN:%d",
		    g_pEffectSpriteTypeTable->GetSize(),a,b,c,d);
	SetDlgItemText(IDC_STATIC_SIZE,szTmp);

	Draw(0);
}


void EffectSpriteTypeDlg::Draw(int n)
{
  //Draw("32E5004650");
  if (g_pEffectSpriteTypeTable->GetSize()>0)
  {
	  m_InfoList.DeleteAllItems();
	  CString m_csztemp;
	  
	  EFFECTSPRITETYPETABLE_INFO pEf;
	  pEf = g_pEffectSpriteTypeTable->Get(n);

	  m_comBitType.SetCurSel(pEf.BltType);
	  //FrameID
	  //////////////////////////////////////////////////////////////////////////
	  m_csztemp.Format("%d",pEf.FrameID);
	  m_InfoList.InsertItem(0,"FrameID");
	  m_InfoList.SetItemText(0,1,m_csztemp);

	  //////////////////////////////////////////////////////////////////////////
	  m_csztemp.Format("%d",pEf.RepeatFrame);
	  m_InfoList.InsertItem(1,"RepeatFrame");
	  m_InfoList.SetItemText(1,1,m_csztemp);

	  //////////////////////////////////////////////////////////////////////////
	  m_csztemp.Format("%d",pEf.bPairFrameBack);
	  m_InfoList.InsertItem(2,"bPairFrameBack");
	  m_InfoList.SetItemText(2,1,m_csztemp);

	  //////////////////////////////////////////////////////////////////////////
	  m_csztemp.Format("%d",pEf.ActionEffectFrameID);
	  m_InfoList.InsertItem(3,"ActionEffectFrameID");
	  m_InfoList.SetItemText(3,1,m_csztemp);


	  //////////////////////////////////////////////////////////////////////////
	  m_csztemp.Format("%d",pEf.FemaleEffectSpriteType);
	  m_InfoList.InsertItem(4,"FemaleEffectSpriteType");
	  m_InfoList.SetItemText(4,1,m_csztemp);

	//----------------------------------------------------------
	// Pair FrameID List
	//----------------------------------------------------------
	m_PairFrameIDList.ResetContent();
	char szlist[50];
	BYTE numPair = pEf.PairFrameIDList.size();
    if (numPair)
    {
		FRAMEID_LIST::const_iterator iFrame = pEf.PairFrameIDList.begin();
		while (iFrame != pEf.PairFrameIDList.end())
		{
			TYPE_FRAMEID frameID = *iFrame;
			sprintf(szlist,"%d",frameID);
			m_PairFrameIDList.AddString(szlist);
			iFrame++;
		}
    }
  }
	UpdateData(FALSE);


}

BOOL EffectSpriteTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_comBitType.Clear();
	m_comBitType.AddString("BLT_NORMAL");
	m_comBitType.AddString("BLT_EFFECT");
	m_comBitType.AddString("BLT_SHADOW");
	m_comBitType.AddString("BLT_SCREEN");

	//////////////////////////////////////////////////////////////////////////
	//初始化列表 
	ListView_SetExtendedListViewStyle(m_InfoList.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	m_InfoList.InsertColumn(0,"Item Name",LVCFMT_LEFT,140);
	m_InfoList.InsertColumn(1,"Item Info",LVCFMT_LEFT,180,COLUMN_STYLE_EDIT);

	//////////////////////////////////////////////////////////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void EffectSpriteTypeDlg::OnSelchangeList1() 
{
	Draw(m_list.GetCurSel());
}

void EffectSpriteTypeDlg::OnChangeEditFind() 
{
	UpdateData(TRUE);
	if (m_FindNum >=g_pEffectSpriteTypeTable->GetSize())
	{
		return ;
	}
	m_list.SetCurSel(m_FindNum);
	
	OnSelchangeList1();
}

void EffectSpriteTypeDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (g_pEffectSpriteTypeTable != NULL)
	{
		g_pEffectSpriteTypeTable->Release();
		delete g_pEffectSpriteTypeTable;
		g_pEffectSpriteTypeTable = NULL;
	}
}

//写出
void EffectSpriteTypeDlg::OnWrite() 
{
	//保存
	if (AfxMessageBox("确认将修改写入文件么？",MB_YESNO)  == IDYES)
	{
		ofstream m_out(FILE_EST,ios::binary);
		//保存现有数据
		g_pEffectSpriteTypeTable->SaveToFile(m_out);

		if (!m_ESTList.IsEmpty())
		{
			POSITION pos = m_ESTList.GetHeadPosition();
			for (int i = 0;i < m_ESTList.GetCount();i++)
			{
				EFFECTSPRITETYPETABLE_INFO temp = m_ESTList.GetNext(pos);
				temp.SaveToFile(m_out);
			}

			m_out.seekp(ios::beg);
			int dwSize = g_pEffectSpriteTypeTable->GetSize() + m_ESTList.GetCount();
			m_out.write((const char *)&dwSize,2);

			m_ESTList.RemoveAll();
		}

		m_out.close();

		//重新读入
		OnRead();
	}
	
}

//修改一个数据
void EffectSpriteTypeDlg::OnMod() 
{
	if (m_ismod)
	{
		CGenData m_gen(m_InfoList);
		EFFECTSPRITETYPETABLE_INFO* pEf = &(g_pEffectSpriteTypeTable->Get(m_list.GetCurSel()));

		pEf->FrameID = m_gen.GetListInt(0);
		pEf->RepeatFrame = m_gen.GetListBYTE(1);
		pEf->bPairFrameBack = m_gen.GetListBYTE(2);
		pEf->ActionEffectFrameID = m_gen.GetListInt(3);
		pEf->FemaleEffectSpriteType = m_gen.GetListInt(4);
		pEf->BltType = (BLT_TYPE)m_comBitType.GetCurSel();
	}
	else
		OnAddinfo();
}

//获得内容
void EffectSpriteTypeDlg::OnSelchangeLISTPairFrameIDList() 
{
	CString Temp;
	m_PairFrameIDList.GetText(m_PairFrameIDList.GetCurSel(),Temp);
	m_PAIRINFO = atoi(Temp);
	UpdateData(FALSE);
}

//计数器要增加
void EffectSpriteTypeDlg::OnAddinfo() 
{
	CGenData m_gen(m_InfoList);
	CString m_temp;
	EFFECTSPRITETYPETABLE_INFO pEf;
 	pEf.FrameID = m_gen.GetListInt(0);
	pEf.RepeatFrame = m_gen.GetListBYTE(1);
	pEf.bPairFrameBack = m_gen.GetListBYTE(2);
	pEf.ActionEffectFrameID = m_gen.GetListInt(3);
	pEf.FemaleEffectSpriteType = m_gen.GetListInt(4);
	pEf.BltType = (BLT_TYPE)m_comBitType.GetCurSel();
	
	int isize = pEf.PairFrameIDList.size();
	pEf.PairFrameIDList.clear();
	for (int i = 0;i < m_PairFrameIDList.GetCount();i++)
	{
		m_PairFrameIDList.GetText(i,m_temp);
		TYPE_FRAMEID m_tf = atoi(m_temp);
		pEf.PairFrameIDList.push_back(m_tf);
	}

	m_ESTList.AddTail(pEf);
}

//删除选种项连表的内容
void EffectSpriteTypeDlg::OnDelete() 
{
	//UpdateData();
	if (m_ismod)
	{
		EFFECTSPRITETYPETABLE_INFO* pEf = &(g_pEffectSpriteTypeTable->Get(m_list.GetCurSel()));
		pEf->PairFrameIDList.remove(m_PAIRINFO);
	}

	m_PairFrameIDList.DeleteString(m_PairFrameIDList.GetCurSel());
}


void EffectSpriteTypeDlg::OnAdd() 
{
	UpdateData();
	CString Temp;

	if (m_ismod)
	{
		EFFECTSPRITETYPETABLE_INFO* pEf = &(g_pEffectSpriteTypeTable->Get(m_list.GetCurSel()));
		pEf->PairFrameIDList.push_back(m_PAIRINFO);
	}
	
	Temp.Format("%d",m_PAIRINFO);
	m_PairFrameIDList.AddString(Temp);
}

void EffectSpriteTypeDlg::OnCheck1() 
{
	m_ismod = !m_ismod;
}
