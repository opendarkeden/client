// ActionEffectSpriteTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "infLoad.h"
#include "ActionEffectSpriteTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ActionEffectSpriteTypeDlg dialog


ActionEffectSpriteTypeDlg::ActionEffectSpriteTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ActionEffectSpriteTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ActionEffectSpriteTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ActionEffectSpriteTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ActionEffectSpriteTypeDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ActionEffectSpriteTypeDlg, CDialog)
	//{{AFX_MSG_MAP(ActionEffectSpriteTypeDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnRead)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ActionEffectSpriteTypeDlg message handlers
void ActionEffectSpriteTypeDlg::Init()
{
	if (g_pActionEffectSpriteTypeTable !=NULL)
	{
		delete g_pActionEffectSpriteTypeTable;
		g_pActionEffectSpriteTypeTable=NULL;
	}
	g_pActionEffectSpriteTypeTable = new MActionEffectSpriteTypeTable;
	//清除文件列表
	m_list.ResetContent();
}

void ActionEffectSpriteTypeDlg::OnRead() 
{
	sprintf(m_path,"d:\\ttdk2\\data\\info\\ActionEffectSpriteType.inf");
	Init();

	//////////////////////////////////////////////////////////////////////////
	// Load EffectSprinteType.inf
	//////////////////////////////////////////////////////////////////////////

	class ifstream actionEffectSpriteTypeTable2;//(g_pFileDef->getProperty("FILE_INFO_EFFECTSPRITETYPE"), ios::binary);
	if (!FileOpenBinary(m_path, actionEffectSpriteTypeTable2))
		return;
	g_pActionEffectSpriteTypeTable->LoadFromFile(actionEffectSpriteTypeTable2);
	actionEffectSpriteTypeTable2.close();
	DrawAll();

	::EnableWindow(::GetDlgItem(this->m_hWnd,IDC_BUTTON1),false);
	
}



void ActionEffectSpriteTypeDlg::DrawAll()
{
	CString m_temp;
	for (int i=0;i<g_pActionEffectSpriteTypeTable->GetSize();i++)
	{
		ACTION_FRAMEID_ARRAY a =g_pActionEffectSpriteTypeTable->Get(i);

		m_temp.Format("%d",a.Get(i).FrameID);
		m_list.AddString(m_temp);
	}

}

void ActionEffectSpriteTypeDlg::OnDestroy() 
{
	CDialog::OnDestroy();	
}
