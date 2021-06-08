// infLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "infLoad.h"
#include "infLoadDlg.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfLoadDlg dialog

CInfLoadDlg::CInfLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfLoadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInfLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfLoadDlg)
	DDX_Control(pDX, IDC_TAB1, m_FileTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInfLoadDlg, CDialog)
	//{{AFX_MSG_MAP(CInfLoadDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfLoadDlg message handlers

BOOL CInfLoadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//≥ı ºªØ±Í«©
	InitFrom();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CInfLoadDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CInfLoadDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CInfLoadDlg::InitFrom()
{
		TCITEM item;
		item.mask = TCIF_TEXT;
		item.pszText = "EffectSpriteType";
		m_FileTab.InsertItem (0,&item);
		item.pszText = "ActionEffectSpriteType";
		m_FileTab.InsertItem (1,&item);
		item.pszText = "Effectstatus";
		m_FileTab.InsertItem (2,&item);
		item.pszText = "Skill";
		m_FileTab.InsertItem (3,&item);	
		item.pszText = "SkillInfo";
		m_FileTab.InsertItem (4,&item);
		item.pszText = "Action";
		m_FileTab.InsertItem (5,&item);
		item.pszText = "Creature";
		m_FileTab.InsertItem(6, &item);
		item.pszText = "CreatureSprite";
		m_FileTab.InsertItem(7, &item);
		CRect r;
		m_FileTab.GetClientRect (&r);

		m_page1.Create(IDD_OLE_PAGE01,&m_FileTab);
		m_page2.Create(IDD_OLE_PAGE02,&m_FileTab);
		m_page3.Create(IDD_OLE_PAGE03,&m_FileTab);
		m_page4.Create(IDD_SKILL_OLE,&m_FileTab);
		m_page5.Create(IDD_SKILLINFO_OLE,&m_FileTab);
		m_page6.Create(IDD_ACTION_OLE,&m_FileTab);	
		m_page7.Create(IDD_DIALOG_CREATURE, &m_FileTab);
		m_page8.Create(IDD_DIALOG_CREATURESPRITE, &m_FileTab);
		m_page1.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_SHOWWINDOW);
		m_page2.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
		m_page3.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
		m_page4.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
		m_page5.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
		m_page6.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
		m_page7.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
		m_page8.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);

}

void CInfLoadDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int index =m_FileTab.GetCurSel();
	CRect r;
	m_FileTab.GetClientRect (&r);
	switch (index)
	{
		case 0://EffectSpriteType.inf
			m_page1.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_SHOWWINDOW);
			m_page2.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page3.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page4.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page5.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page6.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page7.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page8.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			break;
		case 1://ActionEffectSpriteType.inf
			m_page1.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page2.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_SHOWWINDOW);
			m_page3.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page4.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page5.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page6.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page7.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page8.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			break;
		case 2://effectstatus.inf
			m_page1.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page2.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page3.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_SHOWWINDOW);
			m_page4.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page5.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page6.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page7.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page8.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			break;
		case 3://skill.inf
			m_page1.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page2.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page3.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page4.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_SHOWWINDOW);
			m_page5.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page6.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page7.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page8.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			break;
		case 4://skillinfo.inf
			m_page1.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page2.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page3.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page4.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page5.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_SHOWWINDOW);
			m_page6.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page7.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page8.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			break;
		case 5://action.inf
			m_page1.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page2.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page3.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page4.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page5.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page6.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_SHOWWINDOW);
			m_page7.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page8.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			break;
		case 6:
			m_page1.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page2.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page3.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page4.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page5.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page6.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page7.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_SHOWWINDOW);
			m_page8.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			break;
		case 7:
			m_page1.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page2.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page3.SetWindowPos (NULL,5,22,r.right -10,r.bottom -30,SWP_HIDEWINDOW);
			m_page4.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page5.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page6.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page7.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_HIDEWINDOW);
			m_page8.SetWindowPos(NULL,5,22,r.right - 10,r.bottom -30,SWP_SHOWWINDOW);
			break;
	}
	*pResult = 0;
	
}
