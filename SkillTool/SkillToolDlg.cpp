// SkillToolDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SkillTool.h"
#include "SkillToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillToolDlg dialog

CSkillToolDlg::CSkillToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkillToolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSkillToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkillToolDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSkillToolDlg, CDialog)
	//{{AFX_MSG_MAP(CSkillToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillToolDlg message handlers

BOOL CSkillToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//初始化变量
	SkillInit();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSkillToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSkillToolDlg::OnPaint() 
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
HCURSOR CSkillToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//初始化函数
void CSkillToolDlg::SkillInit()
{
	if (m_SkillInfoManager!=NULL)
	{
		m_SkillInfoManager =NULL;
	}
}

void CSkillToolDlg::Release()
{
	if (m_SkillInfoManager!=NULL)
	{
		delete m_SkillInfoManager;
	}
}
typedef unsigned short TYPE_ACTIONINFO;

void CSkillToolDlg::OnOK() 
{
	// TODO: Add extra validation here
	/*
	CString SkillInfoPath="data\\info\\skillinfo.inf";
	m_SkillInfoManager =new CSkillinfo;
	m_SkillInfoManager->OpenFromFile(SkillInfoPath);
	delete m_SkillInfoManager;
	*/
	CEffectSpriteTypeManager EffectSpriteType;	//EffectSpriteType.inf
	CEffectstatusManager	 EffectStatus;		//EffectStatus.inf
	CSkillinfo				 SkillInfo;			//SkillInfo.inf



	//EffectSpriteType.LoadFromFile("data\\info\\EffectSpriteType.inf");

	/*
	int num,i;
	BYTE ee[2]={0x0d,0x0a};
	BYTE tmp[8];
	CFile p;
	CFile out("SKillType.log",CFile::modeCreate| CFile::modeReadWrite);
	CFile outDK2("Action.inf",CFile::modeCreate | CFile::modeReadWrite);
	p.Open("data\\info\\Action.inf",CFile::modeRead);
	ActionKrToDk2*	Act;
	ActionKrToDk2*    Act2;
	p.Read(&tmp,8);
	p.Read(&num,4);

	outDK2.Write(&tmp,8);
	outDK2.Write(&num,4);
	Act=new ActionKrToDk2[num];
	char  buf[300];

	for (i=0;i<num;i++)
	{
		Act[i].LoadFormKrFile(&p,TRUE);
		//Act[i].SaveNameDefLog(&out,i);
		Act[i].SaveFormFile(&outDK2,false);
		/*
		if (i==377)
		{
			i=i;
		}
			p.Read(&namelen,4);
			name = new char[namelen+1];
			p.Read(name,namelen);
			name[namelen]=NULL;
			sprintf(buf,"%d		%s",i,name);
			out.Write(buf,strlen(buf));
			out.Write(&ee,2);
			delete[]name;
		p.Read(&tmp,68);
		//p.Read(&tmp,43);
		p.Read(&tmp,29);//41
		//new kr  17 + 4 + 32=53
		
			p.Read(&tmp,51);
			namelen=0;
			p.Read(&namelen,1);
			p.Read(&tmp,1);
			
		// end 
		
		char flag = 0;
		p.Read((char*)&flag, sizeof(char) );
		bool m_bUseActionStep;
		m_bUseActionStep = (flag & 0x2) != 0;
		if( m_bUseActionStep )
		{
			for( int x = 0;x<5; x++)
			{
				p.Read(&tmp,sizeof( TYPE_ACTIONINFO ) );
			}
		}
		// new kr
		
			for (int c=0;c<namelen;c++)
			{
				p.Read(&tmp,8);
			}
			
			
		// end
		p.Read(&tmp,sizeof( TYPE_ACTIONINFO ) );
		//p.Read(&tmp,2);
		p.Read(&tmp,2+1);
		int nsi;
		p.Read(&nsi,4);
		for (int h=0;h<nsi;h++)
		{
			p.Read(&tmp,14);
		}
		*/
	//}


	
	SkillInfo.OpenFromFile("data\\info\\skillinfo.inf");
	CFile out("data\\info\\SKillLog.txt",CFile::modeCreate | CFile::modeReadWrite);
	CString  SkillLog;
	char   tmp[255];
	lpsSkillnode	lpnode;
	for (int i=0;i<SkillInfo.NumofSkill;i++)
	{

		lpnode = SkillInfo.Getnode(i);
		sprintf(tmp,"SkillID: %d EN: %s HN: %s\n",lpnode->skillID,lpnode->name,lpnode->Hname);
		SkillLog+=tmp;
	}
	out.Write(SkillLog.GetBuffer(SkillLog.GetLength()),SkillLog.GetLength());
	out.Close();

	CDialog::OnOK();
	
}

