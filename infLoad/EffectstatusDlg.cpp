// EffectstatusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "infLoad.h"
#include "EffectstatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EffectstatusDlg dialog
CList<EFFECTSTATUS_NODE,EFFECTSTATUS_NODE&> m_ESList;

EffectstatusDlg::EffectstatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(EffectstatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(EffectstatusDlg)
	m_bUseEffectSprite = 0;
	m_bAttachGround = 0;
	m_EffectSpriteType = 0;
	m_EffectColor = 0;
	m_ActionInfo = 0;
	m_OriginalActionInfo = 0;
	m_SoundID = 0;
	m_FindIt = 0;
	//}}AFX_DATA_INIT
}


void EffectstatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EffectstatusDlg)
	DDX_Control(pDX, IDC_COMEffectColorPart, m_compart);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_bUseEffectSprite);
	DDX_Text(pDX, IDC_EDIT2, m_bAttachGround);
	DDX_Text(pDX, IDC_EDIT3, m_EffectSpriteType);
	DDX_Text(pDX, IDC_EDIT4, m_EffectColor);
	DDX_Text(pDX, IDC_EDIT5, m_ActionInfo);
	DDX_Text(pDX, IDC_EDIT6, m_OriginalActionInfo);
	DDX_Text(pDX, IDC_EDIT7, m_SoundID);
	DDX_Text(pDX, IDC_EDIT_FIND, m_FindIt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EffectstatusDlg, CDialog)
	//{{AFX_MSG_MAP(EffectstatusDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnRead)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_EN_CHANGE(IDC_EDIT_FIND, OnChangeEditFind)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_WRITE, OnWrite)
	ON_BN_CLICKED(IDC_MOD, OnMod)
	ON_BN_CLICKED(IDC_ADDINFO, OnAddinfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EffectstatusDlg message handlers


void EffectstatusDlg::OnRead() 
{
	Init();

	class ifstream EffectStatusTable2;//(g_pFileDef->getProperty("FILE_INFO_EffectStatus"), ios::binary);
	if (!FileOpenBinary(FILE_ES, EffectStatusTable2))
		return ;
	(*g_pEffectStatusTable).LoadFromFile(EffectStatusTable2);
	EffectStatusTable2.close();
	DrawAll();
	m_list.SetCurSel(0);	
}

void EffectstatusDlg::Init()
{
	if (g_pEffectStatusTable !=NULL)
	{
		delete g_pEffectStatusTable;
		g_pEffectStatusTable=NULL;
	}
	g_pEffectStatusTable = new EFFECTSTATUS_TABLE;
	//清除文件列表
	m_list.ResetContent();
	m_compart.ResetContent();
	m_compart.AddString("ADDON_HAIR");
	m_compart.AddString("ADDON_HELM");
	m_compart.AddString("ADDON_COAT");
	m_compart.AddString("ADDON_TROUSER");
	m_compart.AddString("ADDON_LEFTHAND");
	m_compart.AddString("ADDON_RIGHTHAND");
	m_compart.AddString("ADDON_MOTOR");
	m_compart.AddString("ADDON_MAX");
	m_compart.AddString("ADDON_NULL=0xFFFF");
}

void EffectstatusDlg::DrawAll()
{
	char sztmp[300];
	for (int i=0;i<g_pEffectStatusTable->GetSize();i++)
	{
		sprintf(sztmp,"%04d",i);
		m_list.AddString(sztmp);
	}
	Draw(0);
}

void EffectstatusDlg::Draw(int n)
{
	UpdateData(TRUE);
	char sztmp[300];
	if (n>g_pEffectStatusTable->GetSize()-1)
	{
		return;
	}
	EFFECTSTATUS_NODE pEF;
	pEF = g_pEffectStatusTable->Get(n);
	m_bUseEffectSprite = pEF.bUseEffectSprite;
	m_bAttachGround = pEF.bAttachGround;
	m_EffectSpriteType = pEF.EffectSpriteType;
	m_EffectColor = pEF.EffectColor;
	m_ActionInfo = pEF.ActionInfo;
	m_OriginalActionInfo = pEF.OriginalActionInfo;
	m_SoundID = pEF.SoundID;

	if (pEF.EffectColorPart ==0xFF)
	{
		m_compart.SetCurSel(8);
	}else
	{
		m_compart.SetCurSel(pEF.EffectColorPart);
	}

	sprintf(sztmp,"Size:%d %02x %02x %04x %04x %04x %04x %08x",
			g_pEffectStatusTable->GetSize(),
			m_bUseEffectSprite,
			m_bAttachGround,
			m_EffectSpriteType,
			m_EffectColor,
			m_ActionInfo,
			m_OriginalActionInfo,
			m_SoundID);
	SetDlgItemText(IDC_STATIC_SIZE,sztmp);
	UpdateData(FALSE);	

}

void EffectstatusDlg::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	Draw(m_list.GetCurSel());
}

void EffectstatusDlg::OnChangeEditFind() 
{
    UpdateData();
	if(m_FindIt >= g_pEffectStatusTable->GetSize())
	{
		return;
	}

	m_list.SetCurSel(m_FindIt);

	OnSelchangeList1();
}

void EffectstatusDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (g_pEffectStatusTable != NULL)
	{
		g_pEffectStatusTable->Release();
		delete g_pEffectStatusTable;
		g_pEffectStatusTable = NULL;
	}
	
}

//写出文件
void EffectstatusDlg::OnWrite() 
{
 	if (AfxMessageBox("确认要保存文件么？",MB_YESNO) == IDYES)
 	{
		ofstream m_out(FILE_ES,ios::binary);
 		g_pEffectStatusTable->SaveToFile(m_out);
 
 		//写出增加部分
 		if (!m_ESList.IsEmpty())
 		{
 			POSITION pos = m_ESList.GetHeadPosition();
 
 			for(int i = 0;i < m_ESList.GetCount();i++)
 			{
 				EFFECTSTATUS_NODE eftemp = m_ESList.GetNext(pos);
 
				eftemp.SaveToFile(m_out);
 			}
 
			//跳回头 写出长度
 			m_out.seekp(ios::beg);
 			int dwsize = g_pEffectStatusTable->GetSize() + m_ESList.GetCount();
 			m_out.write((const char*)&dwsize,4);

			m_ESList.RemoveAll();
 		}
 
 		m_out.close();
		OnRead();
 	}
}

//修改数据
void EffectstatusDlg::OnMod() 
{
	UpdateData();
	EFFECTSTATUS_NODE *EnTEMP = &(g_pEffectStatusTable->Get(m_list.GetCurSel()));
	
	EnTEMP->bUseEffectSprite = m_bUseEffectSprite;
	EnTEMP->bAttachGround = m_bAttachGround;
	EnTEMP->EffectColor = m_EffectColor;
	EnTEMP->EffectSpriteType = m_EffectSpriteType;
	EnTEMP->ActionInfo = m_ActionInfo;
	EnTEMP->OriginalActionInfo = m_OriginalActionInfo;
	EnTEMP->SoundID = m_SoundID;
	EnTEMP->EffectColorPart = (ADDON)m_compart.GetCurSel();

}

void EffectstatusDlg::OnAddinfo() 
{
	UpdateData();
	EFFECTSTATUS_NODE new_EnTEMP;

	new_EnTEMP.bUseEffectSprite = m_bUseEffectSprite;
	new_EnTEMP.bAttachGround = m_bAttachGround;
	new_EnTEMP.EffectColor = m_EffectColor;
	new_EnTEMP.EffectSpriteType = m_EffectSpriteType;
	new_EnTEMP.ActionInfo = m_ActionInfo;
	new_EnTEMP.OriginalActionInfo = m_OriginalActionInfo;
	new_EnTEMP.SoundID = m_SoundID;
	new_EnTEMP.EffectColorPart = (ADDON)m_compart.GetCurSel();

	m_ESList.AddTail(new_EnTEMP);
}
