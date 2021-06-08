// AniDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteManager.h"

#include "AniDlg.h"
#include "SpriteManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BG_WIDTH		600
#define BG_HEIGHT		600

#define TILE_X (BG_WIDTH/2)
#define TILE_Y (BG_HEIGHT/2)
#define TILE_WIDTH 48
#define TILE_HEIGHT 24


BOOL IsOustersType(int type);



void TransparentBltDC( HDC hDestDC, int XDest, int YDest, int nDestWidth, int nDestHeight, 
								HDC hSrcDC, int XSrc, int YSrc, int nSrcWidth, int nSrcHeight, COLORREF tpColor)
{

	if( hDestDC == NULL || hSrcDC == NULL) return;

	if( nDestWidth == 0) return;
	if( nDestHeight == 0) return;
	if( nSrcWidth == 0) return;
	if( nSrcHeight == 0) return;

	int iUsage = DIB_RGB_COLORS;


	HDC	hMemDC = NULL, hMaskDC = NULL;

	// create dcs
	hMaskDC = CreateCompatibleDC( hDestDC);
	hMemDC = CreateCompatibleDC( hDestDC);


	HBITMAP hBMP = NULL;	// hMemDC가 select할 bitmap
	HBITMAP hOldBMP = NULL, hOldBMP2 = NULL;
	HBITMAP hMaskBMP = NULL;	// mask bitmap

	// create bitmap for memory dc
	hBMP = CreateCompatibleBitmap( hDestDC, nSrcWidth, nSrcHeight);
	// select bitmap
	hOldBMP = (HBITMAP)SelectObject( hMemDC, hBMP);
	// copy source to memory dc
	BitBlt( hMemDC, 0, 0, nSrcWidth, nSrcHeight, hSrcDC, XSrc, YSrc, SRCCOPY );

	// create mask bitmap
	hMaskBMP = CreateBitmap( nSrcWidth, nSrcHeight, 1, 1, NULL);
	// select mask bitmap
	hOldBMP2 = (HBITMAP)SelectObject( hMaskDC, hMaskBMP);

	// set background color
	SetBkColor( hMemDC, tpColor);

	// make mask dc from memory dc.
	BitBlt( hMaskDC, 0, 0, nSrcWidth, nSrcHeight, hMemDC, 0, 0, SRCCOPY);

	// copy mask dc to memory dc
	SetBkColor( hMemDC, RGB( 0, 0, 0));
	SetTextColor( hMemDC, RGB( 255, 255, 255));
	BitBlt( hMemDC, 0, 0, nDestWidth, nDestHeight, hMaskDC, 0, 0, SRCAND);


	// 
	SetBkColor( hDestDC, RGB( 255, 255, 255));
	SetTextColor( hDestDC, RGB( 0, 0, 0));
	BitBlt( hDestDC, XDest, YDest, nDestWidth, nDestHeight, hMaskDC, 0, 0, SRCAND);


	// Combine the foreground with the background
	::BitBlt( hDestDC, XDest, YDest, nDestWidth, nDestHeight, hMemDC, 0, 0, SRCPAINT);


	if( hOldBMP) 
		::SelectObject( hMemDC, hOldBMP);
	if( hOldBMP2)
		::SelectObject( hMaskDC, hOldBMP2);

	DeleteObject( hBMP);
	DeleteObject( hMaskBMP);

	DeleteDC( hMemDC);
	DeleteDC( hMaskDC);

}







/////////////////////////////////////////////////////////////////////////////
// CAniDlg dialog


CAniDlg::CAniDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAniDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAniDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bResetFPS = TRUE;
	m_bPlayingAni = TRUE;
	m_iAniTime = 0;
	m_pParentWnd = (CSpriteManagerDlg*)pParent;

	memset( m_addons, 0, sizeof( m_addons));
	memset( m_ACAddons, 0, sizeof( m_ACAddons ) );

	m_hBMP = NULL;
	m_hOldBMP = NULL;

	m_fHeightRatio = 1.0f;
	m_fWidthRatio = 1.0f;

	m_bResize = FALSE;
	m_bDrawTile = TRUE;
//	CheckDlgButton( IDC_CHECK_SHOW_TILE, 1);
	

	m_bDragging = FALSE;

	m_iCreatureXOffset = DEFAULT_XOFFSET;
	m_iCreatureYOffset = DEFAULT_YOFFSET;

	m_curPt.x = 0;
	m_curPt.y = 0;

	m_fromPt.x = 0;
	m_fromPt.y = 0;

	m_dwBGBMP = 0;

	m_pMemDC = NULL;

	m_iFPS = DEFAULT_FPS;

	m_timerID = 0;

	int size = sizeof( m_pHOldBmps);

	m_iCurAnis = 0;
	memset( m_pHOldBmps, 0, sizeof( m_pHOldBmps));

	m_bShowWindow = FALSE;
}

CAniDlg::~CAniDlg()
{

}


void CALLBACK AniTimer(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CAniDlg* pAniDlg = (CAniDlg*)dwUser;

	if( pAniDlg && pAniDlg->m_bShowWindow)
	{
		pAniDlg->CheckTime();
	}
}

BOOL CAniDlg::Create( UINT nIDTemplate, CWnd* pParentWnd)
{
	m_pParentWnd = (CSpriteManagerDlg*)pParentWnd;



	return CDialog::Create( nIDTemplate, pParentWnd);

}


/*
#define BG_WIDTH		200
#define BG_HEIGHT		200
*/

BOOL CAniDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

//	m_pParentWnd = (CSpriteManagerDlg*)GetParent();	// modaless이므로..


	// add string combo boxes.
	for( int i=0; i< ADDON_MAX; i++)
	{
		m_comboList[i].AddString( "없슴");
		m_comboList[i].SetCurSel( 0);
	}

	for( i = 0; i < ADDON_AC_MAX; ++i )
	{
		m_ACComboList[i].AddString( "없슴");
		m_ACComboList[i].SetCurSel( 0 );
	}

	for( i=0; i< SLAYER_PART_MAX; i++)
	{
		m_comboList[ SlayerPartType[ i]].AddString( strSlayerPart[ i]);
	}

	for( i=0; i< VAMPIRE_PART_MAX; i++)
	{
		m_comboVampire.AddString( strVampirePart[ i]);
		m_comboVampire.SetCurSel( 0);
	}
	
	for( i=1;i< OUSTERS_PART_MAX; i++)	// 챠크람은 선택안되게
	{
		m_comboOusters.AddString( strOustersPart[i] );
		m_comboOusters.SetCurSel(0);
	}

	m_comboACVampireWeapon.AddString( "없슴" );
	m_comboACVampireWeapon.AddString( "무기" );
	m_comboACVampireWeapon.SetCurSel( 0 );

	for( i = 0; i < PS_PART_MAX; ++i )
	{
//		if(ACSlayerPartType[i] == ADDON_AC_BODY ) continue;
		m_ACComboList[ ACSlayerPartType[i]].AddString( strPromotionSlayerPart[ i ] );
	}

	////////////////////////////////////////////////////
	// Add Timer
	////////////////////////////////////////////////////
//	SetTimer( TIMER_ID+1, (UINT)(1000.0f/(float)DEFAULT_FPS), NULL);
//	SetTimer( TIMER_ID+1, 0, NULL);

	// load bg bitmap
/*	m_bgBitmap[0].LoadBitmap( MAKEINTRESOURCE(IDB_BITMAP1));
	m_bgBitmap[1].LoadBitmap( MAKEINTRESOURCE(IDB_BITMAP2));
	m_bgBitmap[2].LoadBitmap( MAKEINTRESOURCE(IDB_BITMAP3));
	m_bgBitmap[3].LoadBitmap( MAKEINTRESOURCE(IDB_BITMAP4));
*/
	BITMAP bmp;

	m_bgBitmap[0].LoadBitmap( IDB_BITMAP1);
	m_bgBitmap[0].GetBitmap( &bmp);
	m_bgBitmap[0].SetBitmapDimension( bmp.bmWidth, bmp.bmHeight);

	m_bgBitmap[1].LoadBitmap( IDB_BITMAP2);
	m_bgBitmap[1].GetBitmap( &bmp);
	m_bgBitmap[1].SetBitmapDimension( bmp.bmWidth, bmp.bmHeight);

	m_bgBitmap[2].LoadBitmap( IDB_BITMAP3);
	m_bgBitmap[2].GetBitmap( &bmp);
	m_bgBitmap[2].SetBitmapDimension( bmp.bmWidth, bmp.bmHeight);

	m_bgBitmap[3].LoadBitmap( IDB_BITMAP4);
	m_bgBitmap[3].GetBitmap( &bmp);
	m_bgBitmap[3].SetBitmapDimension( bmp.bmWidth, bmp.bmHeight);



	CComboBox* pBGCombo = (CComboBox*)GetDlgItem( IDC_COMBO_BG);
	pBGCombo->AddString( "BG 1");
	pBGCombo->AddString( "BG 2");
	pBGCombo->AddString( "BG 3");
	pBGCombo->AddString( "BG 4");
	pBGCombo->AddString( "Custom");
	pBGCombo->SetCurSel( 0);

	// Initialize fps edit control
	char strTemp[256];
	itoa( DEFAULT_FPS, strTemp, 10);
	m_editFPS.SetWindowText( strTemp);

	// get animation rect
	CWnd* pWnd = (CWnd*)GetDlgItem( IDC_RECT);
	pWnd->GetClientRect( &m_aniRect);
	pWnd->ClientToScreen( &m_aniRect);
	ScreenToClient( &m_aniRect);

	// make buffer dc
	CDC* pDC = GetDC();
	m_backDC.CreateCompatibleDC( pDC);
	// create bitmap for memory dc
	m_hBMP = CreateCompatibleBitmap( pDC->GetSafeHdc(), BG_WIDTH, BG_HEIGHT);
	// select bitmap
	m_hOldBMP = (HBITMAP)SelectObject( m_backDC, m_hBMP);


	ReleaseDC( pDC);

	// 기본 몸땡이 선정.
	m_comboList[ ADDON_HAIR].SetCurSel( 1);
	m_comboList[ ADDON_COAT].SetCurSel( 1);
	m_comboList[ ADDON_TROUSER].SetCurSel( 1);
	Load();


	// check 32 bit color...
	DEVMODE devMode;
	EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &devMode);
#define GAME_X_RES		800.0f
#define GAME_Y_RES		600.0f
/*
	m_fHeightRatio = (float)devMode.dmPelsHeight / GAME_X_RES;
	m_fWidthRatio = (float)devMode.dmPelsWidth / GAME_Y_RES;
*/
	m_fHeightRatio = 2.0f;
	m_fWidthRatio = 2.0f;
	


	// -------------------------------------------------
	// Set Multimedia Timer
	// -------------------------------------------------
	m_timerID = timeSetEvent( 20, 100, (LPTIMECALLBACK)AniTimer, 
		(DWORD)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);

	// set check 
	CheckDlgButton( IDC_CHECK_SHOW_TILE, m_bDrawTile);


	return TRUE;  // return TRUE  unless you set the focus to a control

}


void CAniDlg::OnPaint() 
{
	CDialog::OnPaint();

//	BITMAP bmp;
	if( m_bgBitmap[ m_dwBGBMP].GetSafeHandle() == 0 || m_pMemDC == NULL || 	m_pParentWnd->m_myData.m_iAniLength == 0) return;

	DrawScene();



//	m_backDC.SelectObject( pOldBmp);	// fix me 음 알 수 없는 에러가..

}



void CAniDlg::OnDestroy() 
{
	CDialog::OnDestroy();


	// -------------------------------------------------
	// Kill Multimedia Timer
	// -------------------------------------------------
	if( m_timerID)
	{
		if( timeKillEvent( m_timerID) != TIMERR_NOERROR)
		{
			m_timerID = m_timerID;

		}
		m_timerID = 0;
	}



	if( m_hOldBMP) 
		m_backDC.SelectObject( m_hOldBMP);

	DeleteObject( m_hBMP);

	m_backDC.DeleteDC();


	if( m_pMemDC)
	{
		for( int i = 0; i< m_iCurAnis; i++)
		{
			HBITMAP hBmp = (HBITMAP)m_pMemDC[i].SelectObject( m_pHOldBmps[ i]);
			DeleteObject( hBmp);
		}
		delete[] m_pMemDC;
	}

	m_pMemDC = NULL;

}


void CAniDlg::OnButtonExit()
{
//	OnOK();
	ShowWindow( SW_HIDE);
}




void CAniDlg::OnButtonPlay()
{
	if( m_bPlayingAni)
	{
		SetDlgItemText( IDC_BUTTON_PLAY, "Play");
		m_bPlayingAni = FALSE;
	}
	else
	{
		SetDlgItemText( IDC_BUTTON_PLAY, "Stop");
		m_bPlayingAni = TRUE;
	}
}

void CAniDlg::OnButtonNext()
{
	if( m_bPlayingAni)
	{
		SetDlgItemText( IDC_BUTTON_PLAY, "Play");
		m_bPlayingAni = FALSE;
	}
	m_iAniTime++;

	if( m_pParentWnd->m_myData.m_iAniLength)
		m_iAniTime = m_iAniTime % m_pParentWnd->m_myData.m_iAniLength;

	Invalidate( FALSE);

}


void CAniDlg::OnButtonBg()
{
	ChangeBg();
}

void CAniDlg::OnButtonPrev()
{
	if( m_bPlayingAni)
	{
		SetDlgItemText( IDC_BUTTON_PLAY, "Play");
		m_bPlayingAni = FALSE;
	}
	m_iAniTime--;

	if( m_iAniTime<0) m_iAniTime = m_pParentWnd->m_myData.m_iAniLength-1;

//	m_iAniTime = m_iAniTime % m_pParentWnd->m_myData.m_iAniLength;

	Invalidate( FALSE);

}

void CAniDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAniDlg)
	DDX_Control(pDX, IDC_COMBO_HEAD, m_comboList[ADDON_HAIR]);
	DDX_Control(pDX, IDC_COMBO_HAT, m_comboList[ADDON_HELM]);
	DDX_Control(pDX, IDC_COMBO_SHIRT, m_comboList[ADDON_COAT]);
	DDX_Control(pDX, IDC_COMBO_PANTS, m_comboList[ADDON_TROUSER]);
//	DDX_Control(pDX, IDC_COMBO_BOOTS, m_comboList[ADDON_BOOTS]);
	DDX_Control(pDX, IDC_COMBO_RIGHT_HAND, m_comboList[ADDON_RIGHTHAND]);
	DDX_Control(pDX, IDC_COMBO_LEFT_HAND, m_comboList[ADDON_LEFTHAND]);
//	DDX_Control(pDX, IDC_COMBO_CLOAK, m_comboList[ADDON_CLOAK]);
	DDX_Control(pDX, IDC_COMBO_MOTOR, m_comboList[ADDON_MOTOR]);
	DDX_Control(pDX, IDC_COMBO_VAMPIRE, m_comboVampire);
	DDX_Control(pDX, IDC_COMBO_OUSTERS, m_comboOusters);
	DDX_Control(pDX, IDC_COMBO_OUSTERS_WEAPON, m_comboOustersWeapon);
/*	DDX_Control(pDX, IDC_AC_HELMET, m_comboACHelmet );
	DDX_Control(pDX, IDC_AC_LEFTHAND, m_comboACLefthand );
	DDX_Control(pDX, IDC_AC_RIGHTHAND, m_comboACRighthand );
	DDX_Control(pDX, IDC_AC_SHOULDER, m_comboACShoulder );
	DDX_Control(pDX, IDC_AC_MOTOR, m_comboACMotor );
*/
	DDX_Control(pDX, IDC_AC_HELMET, m_ACComboList[ ADDON_AC_HELMET ] );
	DDX_Control(pDX, IDC_AC_LEFTHAND, m_ACComboList[ ADDON_AC_LEFTHAND ] );
	DDX_Control(pDX, IDC_AC_RIGHTHAND, m_ACComboList[ ADDON_AC_RIGHTHAND ] );
	DDX_Control(pDX, IDC_AC_SHOULDER, m_ACComboList[ ADDON_AC_SHOULDER ] );
	DDX_Control(pDX, IDC_AC_MOTOR, m_ACComboList[ ADDON_AC_MOTOR ] );
	DDX_Control(pDX, IDC_AC_BODY, m_ACComboList[ ADDON_AC_BODY ] );
	DDX_Control(pDX, IDC_EDIT_FPS, m_editFPS);
//	DDX_Control(pDX, IDC_ACVAMP_WEAPON );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAniDlg, CDialog)
	//{{AFX_MSG_MAP(CAniDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_BOOTS, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_BG, OnSelchangeComboBG)
	ON_CBN_SELCHANGE(IDC_COMBO_CLOAK, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_HAT, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_HEAD, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_LEFT_HAND, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_MOTOR, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_PANTS, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_RIGHT_HAND, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_SHIRT, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_VAMPIRE, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_OUSTERS, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_OUSTERS_WEAPON, OnSelchangeCombo)

	ON_CBN_SELCHANGE(IDC_AC_HELMET, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_AC_LEFTHAND, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_AC_RIGHTHAND, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_AC_SHOULDER, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_AC_MOTOR, OnSelchangeCombo)

	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_BG, OnButtonBg)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_CHECK_RESIZE, OnCheckResize)
	ON_BN_CLICKED(IDC_CHECK_SHOW_TILE, OnCheckTile)

	ON_EN_CHANGE(IDC_EDIT_FPS, OnChangeEditFPS)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAniDlg message handlers



//=======================================================================================
//		
//	Desc:	필요한 dib를 loading한다.
//	Params:	bForce가 true면 무조건 load
//			false면 필요한 경우만 load
//=======================================================================================
void CAniDlg::Load( BOOL bForce)
{
	if( m_pParentWnd->m_myData.m_iAniLength == 0) 
	{
//		m_bPlayingAni = FALSE;
		return;
	}

	int i =0;

	static int type = -1;	

	// slayer load상태에서 slayer아닌 것을 load할 경우 모든 part를 삭제하지 않으면 남은 part들이 
	// 보이게 된다.....
	// ************** 특별한 경우에 있어서, slayer아닌 것을 load한 상태에서, slayer를 load할때도 
	// part삭제가 필요...
	if( ( IsSlayerType( type) && IsSlayerType( m_pParentWnd->m_dwType) == FALSE)
		|| ( IsSlayerType( type) == FALSE && IsSlayerType( m_pParentWnd->m_dwType)) )
	{
		for( i=0; i< ADDON_MAX; i++)
		{
			m_dibList[ i].RemoveAll();
			m_shadowDibList[i].RemoveAll();
		}
	}

	type = m_pParentWnd->m_dwType;

	int action = m_pParentWnd->m_dwAction;
	int part = 0;

	int direction = m_pParentWnd->m_dwDirection;

	// slayer인 경우.
	if( IsSlayerType( type))
	{
		for( i = 0; i < ADDON_MAX; i++)
		{
			if( bForce || m_addons[ i] != m_comboList[ i].GetCurSel())
			{
				m_addons[ i] = m_comboList[ i].GetCurSel();

				// find part id
				for( int j = 0; j < SLAYER_PART_MAX; j++)
				{
					CString lbText;
					m_comboList[ i].GetLBText( m_addons[ i], lbText);
					if( lbText == strSlayerPart[j])	// j가 가리키는 것이 part의 id..
						break;
				}
				if( j >= SLAYER_PART_MAX)	// "없슴" 선택한 경우.
				{
					m_dibList[ i].RemoveAll();	
					m_shadowDibList[ i].RemoveAll();
					continue;	// error
				}
				part = j;

				// load images...
				CString searchString;
				searchString = m_pParentWnd->GetSearchString( type, action, part, direction);

				int at = m_pParentWnd->m_strData.Find( searchString);
				if( at < 0) continue;	// 찾을 수 없는 경우 다음 옷으로..
				LPCSTR filenames = m_pParentWnd->m_strData.GetBuffer(0);

//				if( part == PART_BIKE && action == ACTION_BIKE_MOVE 
//					|| part != PART_BIKE && action == ACTION_BIKE_STOP)
//					m_dibList[ i].LoadDIBs( &filenames[ at + searchString.GetLength()], BIKE_FRAME_COUNT);
//				else
					m_dibList[ i].LoadDIBs( &filenames[ at + searchString.GetLength()]);


				// load shadow..
				at = m_pParentWnd->m_strShadowData.Find( searchString);
				if( at < 0) continue;	// 찾을 수 없는 경우 다음 옷으로..
				filenames = m_pParentWnd->m_strShadowData.GetBuffer(0);
//				if( part == PART_BIKE && action == ACTION_BIKE_MOVE 
//					|| part != PART_BIKE && action == ACTION_BIKE_STOP)
//					m_shadowDibList[ i].LoadDIBs( &filenames[ at + searchString.GetLength()], BIKE_FRAME_COUNT );
//				else
					m_shadowDibList[ i].LoadDIBs( &filenames[ at + searchString.GetLength()] );

			}
		}
	}
	else if(IsVampireType(type))	// 뱀파이어인경우
	{
		part = m_comboVampire.GetCurSel();
		// load images...
		CString searchString;
		searchString = m_pParentWnd->GetSearchString( type, action, part, direction);
		int at = m_pParentWnd->m_strData.Find( searchString);
		if( at < 0) return;	// error..

		// PC는 part가 없으므로..... 0번 index사용.
		LPCSTR filenames = m_pParentWnd->m_strData.GetBuffer(0);
		m_dibList[ 0].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );

		// load shadow
		at = m_pParentWnd->m_strShadowData.Find( searchString);
		if( at >= 0) 
		{
			// PC는 part가 없으므로..... 0번 index사용.
			filenames = m_pParentWnd->m_strShadowData.GetBuffer(0);
			m_shadowDibList[ 0].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );
		}
	}
	else if (IsOustersType( type ) )
	{
		// 1번에 무조건 챠크람을 불러온다

		part = m_comboOusters.GetCurSel()+1;

		CString searchString;
		searchString = m_pParentWnd->GetSearchString( type, action, part, direction );
		int at = m_pParentWnd->m_strData.Find( searchString );
		if( at < 0 ) return ;
		LPCSTR filenames = m_pParentWnd->m_strData.GetBuffer(0);
		m_dibList[ 0].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );

		// load shadow
		at = m_pParentWnd->m_strShadowData.Find( searchString);
		if( at >= 0) 
		{
			// PC는 part가 없으므로..... 0번 index사용.
			filenames = m_pParentWnd->m_strShadowData.GetBuffer(0);
			m_shadowDibList[ 0].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );
		}

		// 1번에 챠크람 불러오기
		searchString = m_pParentWnd->GetSearchString( type, action, 0, direction );
		at = m_pParentWnd->m_strData.Find( searchString );
		if( at >= 0 )
		{
			filenames = m_pParentWnd->m_strData.GetBuffer(0);
			m_dibList[ 1].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );
			
			// load shadow
			at = m_pParentWnd->m_strShadowData.Find( searchString);
			if( at >= 1) 
			{
				// PC는 part가 없으므로..... 0번 index사용.
				filenames = m_pParentWnd->m_strShadowData.GetBuffer(0);
				m_shadowDibList[ 1].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );
			}
		}
		else
		{
			m_dibList[1].RemoveAll();
			m_shadowDibList[1].RemoveAll();
		}

	}
	else if( IsPromotionSlayer( type) )
	{
		for( i = 0; i < ADDON_AC_MAX; i++)
		{
			if( bForce || m_ACAddons[ i] != m_ACComboList[ i].GetCurSel())
			{
				m_ACAddons[ i] = m_ACComboList[ i].GetCurSel();

				if( m_ACAddons[ i] == -1 ) continue;

				// find part id
				for( int j = 0; j < PS_PART_MAX; j++)
				{
					CString lbText;
					m_ACComboList[ i].GetLBText( m_ACAddons[ i], lbText);
					if( lbText == strPromotionSlayerPart[j])	// j가 가리키는 것이 part의 id..
						break;
				}
				if( j >= PS_PART_MAX)	// "없슴" 선택한 경우.
				{
					m_dibList[ i].RemoveAll();	
					m_shadowDibList[ i].RemoveAll();
					continue;	// error
				}
				part = j;

				// load images...
				CString searchString;
				searchString = m_pParentWnd->GetSearchString( type, action, part, direction);

				int at = m_pParentWnd->m_strData.Find( searchString);
				if( at < 0) continue;	// 찾을 수 없는 경우 다음 옷으로..
				LPCSTR filenames = m_pParentWnd->m_strData.GetBuffer(0);

//				if( part == PART_BIKE && action == ACTION_BIKE_MOVE 
//					|| part != PART_BIKE && action == ACTION_BIKE_STOP)
//					m_dibList[ i].LoadDIBs( &filenames[ at + searchString.GetLength()], BIKE_FRAME_COUNT);
//				else
					m_dibList[ i].LoadDIBs( &filenames[ at + searchString.GetLength()]);


				// load shadow..
				at = m_pParentWnd->m_strShadowData.Find( searchString);
				if( at < 0) continue;	// 찾을 수 없는 경우 다음 옷으로..
				filenames = m_pParentWnd->m_strShadowData.GetBuffer(0);
//				if( part == PART_BIKE && action == ACTION_BIKE_MOVE 
//					|| part != PART_BIKE && action == ACTION_BIKE_STOP)
//					m_shadowDibList[ i].LoadDIBs( &filenames[ at + searchString.GetLength()], BIKE_FRAME_COUNT );
//				else
					m_shadowDibList[ i].LoadDIBs( &filenames[ at + searchString.GetLength()] );

			}
		}
	}
	else if( IsPromotionOusters( type ) )
	{
		// 1번에 무조건 챠크람을 불러온다

		part = m_comboOusters.GetCurSel()+1;

		CString searchString;
		searchString = m_pParentWnd->GetSearchString( type, action, part, direction );
		int at = m_pParentWnd->m_strData.Find( searchString );
		if( at < 0 ) return ;
		LPCSTR filenames = m_pParentWnd->m_strData.GetBuffer(0);
		m_dibList[ 0].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );

		// load shadow
		at = m_pParentWnd->m_strShadowData.Find( searchString);
		if( at >= 0) 
		{
			// PC는 part가 없으므로..... 0번 index사용.
			filenames = m_pParentWnd->m_strShadowData.GetBuffer(0);
			m_shadowDibList[ 0].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );
		}

		// 1번에 챠크람 불러오기
		searchString = m_pParentWnd->GetSearchString( type, action, 0, direction );
		at = m_pParentWnd->m_strData.Find( searchString );
		if( at >= 0 )
		{
			filenames = m_pParentWnd->m_strData.GetBuffer(0);
			m_dibList[ 1].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );
			
			// load shadow
			at = m_pParentWnd->m_strShadowData.Find( searchString);
			if( at >= 1) 
			{
				// PC는 part가 없으므로..... 0번 index사용.
				filenames = m_pParentWnd->m_strShadowData.GetBuffer(0);
				m_shadowDibList[ 1].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );
			}
		}
		else
		{
			m_dibList[1].RemoveAll();
			m_shadowDibList[1].RemoveAll();
		}

	}
	else if( IsPromotionVampire( type ) )
	{
		part = m_comboVampire.GetCurSel();
		// load images...
		CString searchString;
		searchString = m_pParentWnd->GetSearchString( type, action, part, direction);
		int at = m_pParentWnd->m_strData.Find( searchString);
		if( at < 0) return;	// error..

		// PC는 part가 없으므로..... 0번 index사용.
		LPCSTR filenames = m_pParentWnd->m_strData.GetBuffer(0);
		m_dibList[ 0].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );

		// load shadow
		at = m_pParentWnd->m_strShadowData.Find( searchString);
		if( at >= 0) 
		{
			// PC는 part가 없으므로..... 0번 index사용.
			filenames = m_pParentWnd->m_strShadowData.GetBuffer(0);
			m_shadowDibList[ 0].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );
		}


		searchString = m_pParentWnd->GetSearchString( type, action, 1, direction );
		at = m_pParentWnd->m_strData.Find( searchString );
		if( at >= 0 )
		{
			filenames = m_pParentWnd->m_strData.GetBuffer(0);
			m_dibList[ 1].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );
			
			// load shadow
			at = m_pParentWnd->m_strShadowData.Find( searchString);
			if( at >= 1) 
			{
				// PC는 part가 없으므로..... 0번 index사용.
				filenames = m_pParentWnd->m_strShadowData.GetBuffer(0);
				m_shadowDibList[ 1].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );
			}
		}
		else
		{
			m_dibList[1].RemoveAll();
			m_shadowDibList[1].RemoveAll();
		}
	}
	else
	// 다른 넘들인 경우 vampires..
	{
		// load images...
		CString searchString;
		searchString = m_pParentWnd->GetSearchString( type, action, part, direction);
		int at = m_pParentWnd->m_strData.Find( searchString);
		if( at < 0) return;	// error..

		// PC는 part가 없으므로..... 0번 index사용.
		LPCSTR filenames = m_pParentWnd->m_strData.GetBuffer(0);
		m_dibList[ 0].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );

		// load shadow
		at = m_pParentWnd->m_strShadowData.Find( searchString);
		if( at >= 0) 
		{
			// PC는 part가 없으므로..... 0번 index사용.
			filenames = m_pParentWnd->m_strShadowData.GetBuffer(0);
			m_shadowDibList[ 0].LoadDIBs( &(filenames[ at + searchString.GetLength()]) );
		}
	}

	m_iAniTime = 0;

	MakeAnimations();

	m_bResetFPS = TRUE;

}

void CAniDlg::OnSelchangeCombo() 
{
	Load();

//	if( m_bPlayingAni
	Invalidate( FALSE);
}



void CAniDlg::OnSelchangeComboBG() 
{
	BOOL bTrans = FALSE;
//	m_dwBGBMP = GetDlgItemInt( IDC_COMBO_BG, (int*)&m_dwBGBMP, FALSE);
	m_dwBGBMP = ((CComboBox*)GetDlgItem( IDC_COMBO_BG))->GetCurSel();

	if( m_dwBGBMP == 4)	// custom selected
	{
		ChangeBg();
	}

//	m_dwBGBMP = GetDlgItemInt( IDC_COMBO_BG, &bTrans, FALSE);
	Invalidate();
}

/*
CSpriteManagerDlg* CAniDlg::GetParent()
{
	return (CSpriteManagerDlg*)CWnd::GetParent();
}
*/

void CAniDlg::UpdateInfoStatic()
{
	CString strInfo;
	strInfo.Format( "%d/%d %5.1f", m_iAniTime+1, m_pParentWnd->m_myData.m_iAniLength, m_fRealFPS);
	SetDlgItemText( IDC_STATIC_INFO, strInfo);
}



void CAniDlg::OnCheckResize() 
{
//	GetDlgItemInt( IDC_CHECK_RESIZE, &m_bResize);// shit

	m_bResize = !m_bResize;

	Invalidate();

	
}



void CAniDlg::OnCheckTile() 
{
//	GetDlgItemInt( IDC_CHECK_RESIZE, &m_bResize);// shit

	m_bDrawTile = !m_bDrawTile;

//	Invalidate();
	
}

void CAniDlg::ResetCombos()
{
	if( IsSlayerType( m_pParentWnd->m_dwType))
	{
		for( int i=0; i< ADDON_MAX; i++)
		{
			m_comboList[ i].EnableWindow( TRUE);
		}

	}
	if( IsVampireType( m_pParentWnd->m_dwType))
	{
		for( int i=0; i< VAMPIRE_PART_MAX; i++)
		{
			m_comboVampire.EnableWindow( TRUE);
		}
	}
	if( IsOustersType( m_pParentWnd->m_dwType) )
	{
		for(int i=0;i<OUSTERS_PART_MAX ; i++)
		{
			m_comboOusters.EnableWindow(TRUE);
		}
	}
	else
	{
		for( int i=0; i< ADDON_MAX; i++)
		{
			m_comboList[ i].EnableWindow( TRUE);
		}
	}
}

void CAniDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO
	m_bDragging = TRUE;

	m_curPt = m_fromPt = point;

	CDialog::OnLButtonDown(nFlags, point);
}

void CAniDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO
	int direction = 0;
	int gap = 50;
	int center_x = TILE_X+TILE_WIDTH/2, center_y = TILE_Y+TILE_HEIGHT/2;
//	if(m_bResize)
//	{
//		gap = (float)gap * m_fWidthRatio;
//		center_x = (float)center_x * m_fWidthRatio;
//		center_y = (float)center_y * m_fHeightRatio;
//	}
	center_x += m_aniRect.left;
	center_y += m_aniRect.top;

	if(point.x < center_x-gap)				// 왼쪽
	{
		if(point.y < center_y -gap)		// 좌상
			direction = 7;
		else if(point.y > center_y +gap)	// 좌하
			direction = 1;
		else							// 좌
			direction = 0;
	}
	else if(point.x > center_x+gap)		// 오른쪽
	{
		if(point.y < center_y -gap)		// 우상
			direction = 5;
		else if(point.y > center_y +gap)	// 우하
			direction = 3;
		else							// 우
			direction = 4;
	}
	else								// 중간
	{
		if(point.y < center_y)			// 상
			direction = 6;
		else							// 하
			direction = 2;
	}
		

	m_pParentWnd->m_comboDirection.SetCurSel(direction);
	m_pParentWnd->OnSelchangeComboDirection();
	
	CDialog::OnRButtonDown(nFlags, point);
}


void CAniDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO

	if( m_bDragging)
	{
		m_iCreatureXOffset -= point.x - m_fromPt.x;
		m_iCreatureYOffset -= point.y - m_fromPt.y;
	}

	m_curPt = m_fromPt = point;

	m_bDragging = FALSE;

	CDialog::OnLButtonUp(nFlags, point);
}

void CAniDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( m_bDragging)
	{
//		m_iCreatureXOffset -= point.x - m_fromPt.x;
//		m_iCreatureYOffset = point.y - m_fromPt.y;
		m_curPt = point;

		Invalidate(FALSE);

		m_pParentWnd->m_bDirty = TRUE;	// offset변화가 있었으므로 더러워졌다...
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CAniDlg::OnChangeEditFPS()
{
	CString str;
	m_editFPS.GetWindowText( str);
	m_iFPS = atoi( str);

	m_bResetFPS = TRUE;

	// Reset timer

//	KillTimer( TIMER_ID+1);
//	SetTimer( TIMER_ID+1, (UINT)(1000.0f/(float)m_iFPS), NULL);

}

void CAniDlg::ChangeBg()
{
	CFileDialog dlg( TRUE, "BMP", "*.bmp");
	if( dlg.DoModal() == IDOK)
	{
/*
		CDib dib;
		dib.LoadBitmapFile( dlg.GetPathName(), FALSE);

		m_bgBitmap[4].DeleteObject();

		m_bgBitmap[4].CreateBitmap( dib.m_dwWidth, dib.m_dwHeight, 1, dib.GetBPP(),
			NULL);

		m_bgBitmap[4].SetBitmapDimension( dib.m_dwWidth, dib.m_dwHeight);
*/


		m_bgBitmap[4].DeleteObject();

		HBITMAP hBitmap = NULL; 
		hBitmap = (HBITMAP)LoadImage(NULL, dlg.GetPathName(), IMAGE_BITMAP, 0, 0, 
//			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
			LR_LOADFROMFILE | LR_DEFAULTSIZE); 

		BITMAP bmp;
		m_bgBitmap[4].Attach( hBitmap);
		m_bgBitmap[4].GetBitmap( &bmp);
		m_bgBitmap[4].SetBitmapDimension( bmp.bmWidth, bmp.bmHeight);


/*

		m_bgBitmap[4].DeleteObject();

		HBITMAP hBitmap = NULL; 
		hBitmap = (HBITMAP)LoadImage(NULL, dlg.GetPathName(), IMAGE_BITMAP, 0, 0, 
//			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
			LR_LOADFROMFILE | LR_DEFAULTSIZE); 

		BITMAP bmp;
		CBitmap tempBmp;
		tempBmp.Attach( hBitmap);
		tempBmp.GetBitmap( &bmp);

		m_bgBitmap[4].CreateBitmapIndirect( &bmp);
//		m_bgBitmap[4].GetBitmap( &bmp);
//		m_bgBitmap[4].SetBitmapDimension( bmp.bmWidth, bmp.bmHeight);

*/


/*
		CDib dib;
		dib.LoadBitmapFile( dlg.GetPathName(), FALSE);

		BITMAP bmp;
		bmp.bmBits = dib.GetDibBitsPtr();
		bmp.bmBitsPixel =dib.GetBPP();
		bmp.bmHeight = dib.m_dwHeight;
		bmp.bmWidth = dib.m_dwWidth;
		bmp.bmPlanes = dib.GetDibInfoHeaderPtr()->biPlanes;
		bmp.bmType = 0;
		bmp.bmWidthBytes = dib.GetLineOffsetByte();

		m_bgBitmap[4].DeleteObject();
		m_bgBitmap[4].CreateBitmapIndirect( &bmp);
		m_bgBitmap[4].SetBitmapDimension( dib.m_dwWidth, dib.m_dwHeight);
*/


		m_dwBGBMP = 4;
		CComboBox* pBGCombo = (CComboBox*)GetDlgItem( IDC_COMBO_BG);
		pBGCombo->SetCurSel( m_dwBGBMP);	// select custom..

		Invalidate(TRUE);
	}
}

void CAniDlg::MakeAnimations()
{
	int i;


	if( m_pMemDC)
	{
		for( i = 0; i< m_iCurAnis; i++)
		{
			HBITMAP hBmp = (HBITMAP)m_pMemDC[i].SelectObject( m_pHOldBmps[ i]);
			DeleteObject( hBmp);
		}
		delete[] m_pMemDC;
	}

	m_pMemDC = NULL;

	m_iCurAnis = m_pParentWnd->m_myData.m_aniList.GetCount();	// 사용될 그림의 수.

	// 오토바이를 위해서..
//	if( m_pParentWnd->m_dwAction == ACTION_BIKE_MOVE || 
//		m_pParentWnd->m_dwAction == ACTION_BIKE_STOP )
//		m_iCurAnis = BIKE_FRAME_COUNT;

	if( m_iCurAnis == 0) return;

	CDC* pDC = GetDC();
	m_pMemDC = new CDC[ m_iCurAnis];

	for( i = 0; i< m_iCurAnis; i++)
	{
		m_pMemDC[i].CreateCompatibleDC( pDC);

		HBITMAP hBMP = CreateCompatibleBitmap( pDC->GetSafeHdc(), BG_WIDTH, BG_HEIGHT);
		// select bitmap
		m_pHOldBmps[i] = (HBITMAP)SelectObject( m_pMemDC[i], hBMP);

		// Clear
		CBrush br( RGB( 0, 255, 0));
		CPen pen( PS_SOLID, 1, RGB(0, 255, 0));
		CBrush* pOldBrush = m_pMemDC[i].SelectObject( &br);
		CPen* pOldPen = m_pMemDC[i].SelectObject( &pen);

		m_pMemDC[i].Rectangle(0, 0, BG_WIDTH, BG_HEIGHT);

		m_pMemDC[i].SelectObject( pOldBrush);
		m_pMemDC[i].SelectObject( pOldPen);
	}

	ReleaseDC( pDC);
	




	////////////////////////////////////////////////////////
	//	Make animation cut
	////////////////////////////////////////////////////////
	int dir = m_pParentWnd->m_dwDirection;
	const BYTE* pDrawOrder = s_AddonOrder[dir];

	for( int ani = 0; ani < m_iCurAnis; ani++)
	{
		// put shadow
		for( i = 0; i< ADDON_MAX; i++)
		{
			ADDON addon = (ADDON)pDrawOrder[ i];

			CDib* pDib = m_shadowDibList[ addon].GetAt2( ani);
			if( pDib)
			{
				pDib->TransparentBltDC( m_pMemDC[ani], 0, 0, 
					pDib->m_dwWidth, pDib->m_dwHeight, 
					0, 0, pDib->m_dwWidth, pDib->m_dwHeight);

			}
		}

		// put original
		for( i = 0; i< ADDON_MAX; i++)
		{
			ADDON addon = (ADDON)pDrawOrder[ i];

			CDib* pDib = m_dibList[ addon].GetAt2( ani);
			if( pDib)
			{
				pDib->TransparentBltDC( m_pMemDC[ani], 0, 0, 
					pDib->m_dwWidth, pDib->m_dwHeight, 
					0, 0, pDib->m_dwWidth, pDib->m_dwHeight);

			}
		}
	}

}

void CAniDlg::CheckTime()
{
	////////////////////////////////////////////////////
	// process ani
	////////////////////////////////////////////////////
	if( m_bPlayingAni && m_pParentWnd->m_myData.m_iAniLength != 0 )
	{
		static DWORD dwOldTickCount = 0;
		DWORD dwTickCount;

		dwTickCount = GetTickCount();


		///////////////////////////////////////////////
		// Calculate fps
		///////////////////////////////////////////////
//		static DWORD dwUpdates = 0;
//		static DWORD oldTickCount = GetTickCount();
		DWORD newTickCount = GetTickCount();

		if( m_bResetFPS || (newTickCount - m_dwOldTickCount > 10000) )
		{
			m_bResetFPS = FALSE;

			m_dwOldTickCount = GetTickCount();
			m_dwUpdates = 0;
		}


		float fFPS = ((float)m_dwUpdates / (float) (newTickCount - m_dwOldTickCount))*1000.0f;


		if( fFPS <= (float)m_iFPS)
		{
			// udpate
			m_dwUpdates++;
//			oldTickCount = newTickCount;

			++m_iAniTime;

			if( m_pParentWnd->m_myData.m_iAniLength)
				m_iAniTime = m_iAniTime % m_pParentWnd->m_myData.m_iAniLength;


//			DrawScene();
			Invalidate( FALSE);

//			dwOldTickCount = dwTickCount;
		}
	}

}

void CAniDlg::DrawScene()
{

	////////////////////////////////////////////////////
	// draw real animation 
	////////////////////////////////////////////////////
	HDC hDC = ::GetDC( GetSafeHwnd());

	if( hDC == NULL || m_pMemDC == NULL)
		return;


	// set clip box...
	CRgn rgn;
	rgn.CreateRectRgn( m_aniRect.left, m_aniRect.top, m_aniRect.right, m_aniRect.bottom);
	HRGN hOldRgn = (HRGN)SelectObject( hDC, rgn);


	// draw bg
	CDC bgDC;
	bgDC.CreateCompatibleDC( &m_backDC);
	CBitmap* pOldBmp = bgDC.SelectObject( &m_bgBitmap[ m_dwBGBMP]);
	CSize bmpSize = m_bgBitmap[ m_dwBGBMP].GetBitmapDimension();

	bmpSize.cx = BG_WIDTH;
	bmpSize.cy = BG_HEIGHT;

	m_backDC.FillSolidRect( 0, 0, BG_WIDTH, BG_HEIGHT, 0 );
	m_backDC.BitBlt( 0, 0, bmpSize.cx, bmpSize.cy, &bgDC, 0, 0, SRCCOPY);
	bgDC.SelectObject( pOldBmp);
	bgDC.DeleteDC();

//	m_bgDib.BltDC( m_backDC, 0, 0, 0, 0, 0, 0, 0, 0);

//	if( m_bDragging)
	if( m_bDrawTile)
	{
		// draw tile & center
		RECT tileRect;
		SetRect( &tileRect, TILE_X, TILE_Y, TILE_X + TILE_WIDTH, TILE_Y + TILE_HEIGHT);
	//	FrameRect( m_backDC, &tileRect, brush);
		InvertRect( m_backDC, &tileRect);

		CBrush brush;
		brush.CreateSolidBrush( RGB( 255, 0, 0));
		CBrush* pOldBrush = m_backDC.SelectObject( &brush);
		Rectangle( m_backDC, TILE_X + TILE_WIDTH/2-2, TILE_Y+TILE_HEIGHT/2-2, 
			TILE_X + TILE_WIDTH/2+1, TILE_Y+TILE_HEIGHT/2+1);

		m_backDC.SelectObject( pOldBrush);
	}

	int* pAniIndex = m_pParentWnd->m_myData.m_iAniIndex;

//	int bikeIndex[] = { 0, 1, 2, 3};
//	// 오토바이를 위해서..
//	if( m_pParentWnd->m_dwAction == ACTION_BIKE_MOVE || 
//		m_pParentWnd->m_dwAction == ACTION_BIKE_STOP )
//		pAniIndex = bikeIndex;



	if( m_pMemDC[ pAniIndex[ m_iAniTime]].GetSafeHdc())
	{
		int creatureX = 0;
		int creatureY = 0;

		creatureX = TILE_X - (m_iCreatureXOffset - (m_curPt.x - m_fromPt.x));
		creatureY = TILE_Y - (m_iCreatureYOffset - (m_curPt.y - m_fromPt.y));

//		COLORREF tpColor  = m_pMemDC[ m_iAniTime].GetBkColor();
		COLORREF tpColor  = RGB( 0, 255, 0);
		TransparentBltDC( m_backDC, creatureX, creatureY, BG_WIDTH, BG_HEIGHT, m_pMemDC[ pAniIndex[ m_iAniTime]],
			0, 0, BG_WIDTH, BG_HEIGHT, tpColor);
	}


	if( m_bResize)
		StretchBlt( hDC, -((int)(bmpSize.cx*m_fWidthRatio) - (m_aniRect.right - m_aniRect.left))/2, -((int)(bmpSize.cx*m_fWidthRatio) - (m_aniRect.bottom - m_aniRect.top))/2, (int)(bmpSize.cx*m_fWidthRatio), (int)(bmpSize.cy*m_fHeightRatio)
			, m_backDC, 0, 0, BG_WIDTH, BG_HEIGHT, SRCCOPY);
	else
		BitBlt( hDC, m_aniRect.left, m_aniRect.top, bmpSize.cx, bmpSize.cy, m_backDC, 0, 0, SRCCOPY);

	SelectObject( hDC, hOldRgn );

	::ReleaseDC( GetSafeHwnd(), hDC);


	///////////////////////////////////////////////
	// Calculate real fps
	///////////////////////////////////////////////
	static DWORD dwUpdates = 0;
	dwUpdates++;
	static DWORD oldTickCount = 0;
	DWORD newTickCount = GetTickCount();
	if( newTickCount - oldTickCount > 1000)
	{
		m_fRealFPS = ((float)dwUpdates / (float) (newTickCount - oldTickCount))*1000.0f;

		dwUpdates = 0;
		oldTickCount = newTickCount;
	}





	UpdateInfoStatic();

}


void CAniDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	m_bShowWindow = bShow;
	
}



void CAniDlg::OnClose() 
{

	
	CDialog::OnClose();
}
