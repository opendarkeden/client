// SpriteManagerDlg.cpp : implementation file
//
#include "stdafx.h"
#include "SpriteManager.h"
#include "SpriteManagerDlg.h"

#include <fstream.h>
#include "..\lib_ole\CIndexSprite.h"
#include "CDirectDraw.h"

#include "convertprogress.h"

#include "SlayerActionPart.h"
#include "VampireActionPart.h"
#include "OustersActionPart.h"
#include "PromotionSlayerActionPart.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

const char* strCreatureType[TYPE_MAX] = { "남자 슬레이어",
						     "여자 슬레이어",
							 "PC뱀프 남자",
							 "PC뱀프 여자",
							 "NPC",
							 "중급몹",
							 "아우스터즈",
							 "전직남슬",
							 "전직여슬",
							 "전직남뱀",
							 "전직여뱀",
							 "전직아우"

};

const char* strDirection[ DIR_MAX] = { "좌",
								 "좌하",
								 "하",
								 "우하",
								 "우",
								 "우상",
								 "상",
								 "좌상",
};

// creature type의 가능 action을 규정..
#define SLAYER_ACTION			0x01
#define VAMPIRE_ACTION			0x02
#define COMMON_ACTION			0xff

DWORD midLevelMobActionIndex[ MIDLEVEL_MOB_ACTION_MAX] =
{
	ACTION_STOP,
	ACTION_MOVE,
	ACTION_ATTACK,	
	ACTION_SKILL,			// magic
	ACTION_DAMAGED,	
	ACTION_DRAINED,	
	ACTION_DIE,		
	ACTION_DRAIN,
};

BOOL IsOustersType(int type);


/////////////////////////////////////////////////////////////////////////////
// CSpriteManagerDlg dialog

CSpriteManagerDlg::CSpriteManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpriteManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpriteManagerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	m_iSelectedImageID = -1;	// no select
	m_iSelectedAniID = -1;
	m_bDragging = FALSE;
	m_bDrawSelectionRect = FALSE;

	m_bConfirmDelete = TRUE;
	m_fSizeRatio = 1.3f;		// 클수록 축소.

	m_myData.m_iAniLength = 0;
	m_myData.m_iBmpsCount = 0;

	m_dwType = 0; 
	m_dwAction = 0;
	m_dwPart = 0;
	m_dwDirection = 0;

	memset( m_strDirectoryName, 0, sizeof( m_strDirectoryName));

	memset( m_myData.m_iAniIndex, 0, sizeof( m_myData.m_iAniIndex));

	memset( m_myData.m_pCopyFlag, 0, sizeof( m_myData.m_pCopyFlag));

//	m_strData = "";
//	m_strAniData = "";
//	m_strOffsetData = "";

	m_bDirty = FALSE;
	m_bCopied = FALSE;
	m_iCopiedIndex = -1;
	m_bExtended = TRUE;
	m_bUndoStop = false;

	try {
//	m_strData.GetBufferSetLength( 5000*1024);	// memory 할당.
												// memory할당을 크게 하지 않으면..
												// concatenate 시 삑사리가 가끔 나는 듯...

//	m_strShadowData.GetBufferSetLength( 5000*1024);	// memory 할당.
												// memory할당을 크게 하지 않으면..
												// concatenate 시 삑사리가 가끔 나는 듯...
	} catch( CMemoryException* pExcepction)
	{
		char errMsg[256];
		pExcepction->GetErrorMessage( errMsg, 256);
	}



	int length = m_strData.GetLength();

//	m_pItemIDList = 0;
}

void CSpriteManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpriteManagerDlg)
	DDX_Control(pDX, IDC_ANIMATION_LIST, m_AniList);
	DDX_Control(pDX, IDC_COMBO_PART, m_comboPart);
//	DDX_Control(pDX, IDC_SLIDER_IMAGES, m_imageSliders);
	DDX_Control(pDX, IDC_SLIDER_ANIMATION, m_aniSlider);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_COMBO_DIRECTION, m_comboDirection);
	DDX_Control(pDX, IDC_COMBO_ACTION, m_comboAction);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSpriteManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CSpriteManagerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_COMMAND(ID_MENU_DELETE_ALL_ANIS, OnMenuDeleteAllAnis)
	ON_COMMAND(ID_MENU_DELETE_ANI, OnMenuDeleteAni)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_DIRECTORY, OnButtonDirectory)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	ON_CBN_SELCHANGE(IDC_COMBO_ACTION, OnSelchangeComboAction)
	ON_CBN_SELCHANGE(IDC_COMBO_DIRECTION, OnSelchangeComboDirection)
	ON_CBN_SELCHANGE(IDC_COMBO_PART, OnSelchangeComboPart)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_COMMAND(ID_MENU_SEND, OnMenuSend)
	ON_COMMAND(ID_MENU_SEND_ALL, OnMenuSendAll)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_MAKE_SPRITE, OnButtonMakeSprite)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_WM_CLOSE()
	ON_COMMAND(ID_REPEAT_COPY, OnRepeatCopy)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_VKEYTOITEM()
	ON_BN_CLICKED(IDC_BUTTON_UNDO, OnButtonUndo)
	ON_BN_CLICKED(IDC_BUTTON_REDO, OnButtonRedo)
	ON_COMMAND(ID_COPY, OnCopy)
	ON_COMMAND(ID_PASTE, OnPaste)
	ON_LBN_SELCHANGE(IDC_ANIMATION_LIST, OnSelchangeAnimationList)
	ON_BN_CLICKED(IDC_LISTUP, OnListup)
	ON_BN_CLICKED(IDC_LISTDOWN, OnListdown)
	ON_BN_CLICKED(IDC_LISTDELETE, OnListdelete)
	ON_BN_CLICKED(IDC_LISTCOPY, OnListcopy)
	ON_BN_CLICKED(IDC_LISTPASTE, OnListpaste)
	ON_BN_CLICKED(IDC_LISTCUT, OnListcut)
	ON_COMMAND(ID_UNDO, OnButtonUndo)
	ON_BN_CLICKED(IDC_LISTADD, OnListadd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpriteManagerDlg message handlers

BOOL CSpriteManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Load Accelerator
	HINSTANCE hInstance = AfxGetInstanceHandle();
	HACCEL hAccel = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDR_ACCELERATOR1));
	
	
	////////////////////////////////////////////////////
	// Load cursor
	////////////////////////////////////////////////////

	m_hDragCursor = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_CURSOR_DRAG));
	m_hDragCursorAdd = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_CURSOR_DRAG_ADD));

#define STATIC_GAP	15

	CDirectDraw::InitMask(false);
	CWnd* pWnd;
	// get images rect
//	CWnd* pWnd = (CWnd*)GetDlgItem( IDC_STATIC_IMAGES);
//	pWnd->GetClientRect( &m_imageRect);
//	pWnd->ClientToScreen( &m_imageRect);
//	ScreenToClient( &m_imageRect);
//	m_imageSection = m_imageRect;
//	// gap제거.
//	m_imageRect.left += STATIC_GAP;
//	m_imageRect.right -= STATIC_GAP;
//	m_imageRect.top += STATIC_GAP;
//	m_imageRect.bottom -= STATIC_GAP;


	// get animation rect
	pWnd = (CWnd*)GetDlgItem( IDC_STATIC_ANIMATION);
	pWnd->GetClientRect( &m_aniRect);
	pWnd->ClientToScreen( &m_aniRect);
	ScreenToClient( &m_aniRect);
	m_aniSection = m_aniRect;

	// gap제거.
	m_aniRect.left += STATIC_GAP;
	m_aniRect.right -= STATIC_GAP;
	m_aniRect.top += STATIC_GAP;
	m_aniRect.bottom -= STATIC_GAP;
	
	////////////////////////////////////////////////////
	// Add Timer
	////////////////////////////////////////////////////
	SetTimer( TIMER_ID, (UINT)1, NULL);
	
	ResetCombos();

	// create animation dialog
	m_aniDlg.Create( IDD_DIALOG_ANI, this);

	// check 32 bit color...
	DEVMODE devMode;
	EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &devMode);
	if( devMode.dmBitsPerPel != 32 && devMode.dmBitsPerPel != 24)
	{
		MessageBox( "색상을 32/24 bit로 바꾸지 않으면 문제가 있습니다.");
	}

	//CIndexSprite::SetColorSet();
	ifstream file("IndexTable555", ios::binary);
	BOOL bRet = CIndexSprite::LoadIndexTableFromFile( file );
	file.close();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSpriteManagerDlg::OnPaint() 
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
		UpdateInfo();
		m_aniSlider.SetRange( 0, m_myData.m_iAniLength-1, TRUE);
		CDialog::OnPaint();

//////////////////////
		CDC	*pDC = GetDC();
		
		CRect ClientRect;
		GetClientRect( &ClientRect );
		
		CDC MemDC;
		CBitmap bitmap;

		bitmap.CreateCompatibleBitmap(pDC, ClientRect.right, ClientRect.bottom);
		MemDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBitmap = (CBitmap *)MemDC.SelectObject(&bitmap);

		MemDC.BitBlt(ClientRect.left, ClientRect.top, ClientRect.right-ClientRect.left, ClientRect.bottom-ClientRect.top, pDC, 0, 0, SRCCOPY);
		MemDC.PatBlt(m_aniSection.left, m_aniSection.top, m_aniSection.right-m_aniSection.left, m_aniSection.bottom-m_aniSection.top, BLACKNESS);
//		MemDC.PatBlt(ClientRect.left, ClientRect.top, ClientRect.right-ClientRect.left, ClientRect.bottom-ClientRect.top, BLACKNESS);

		HDC hDC = MemDC.GetSafeHdc();

//////////////////////
		if( hDC)
		{
			////////////////////////////////////////////////////
			// draw image & still ani
			////////////////////////////////////////////////////
			{
				// set clip box...
				CRgn rgn;
				rgn.CreateRectRgn( m_aniRect.left, m_aniRect.top, m_aniRect.right, m_aniRect.bottom);
				HRGN hOldRgn = (HRGN)SelectObject( hDC, rgn);

				int iImageID;
				POSITION pos;


				int iImageX;
				int iImageWidth = 0;
				int iImageHeight = 0;

//				int iImageID = m_imageSliders.GetPos();
//				POSITION pos = m_imageList.FindIndex( iImageID);	// zero base이므로 -1
//
//
//				int iImageX = m_imageRect.left;
//				int iImageWidth = 0;
//				int iImageHeight = 0;
//				while( pos)
//				{
//
//					CDib* pDib = m_imageList.GetNext( pos);
//					if( pDib)
//					{
//
//
//						// 크기 조정.
//						iImageWidth = (int)(pDib->m_dwWidth/m_fSizeRatio);
//						if( (iImageX + iImageWidth) - m_imageRect.right > 0)
//							iImageWidth -= (iImageX + iImageWidth) - m_imageRect.right;
//
//						iImageHeight = (int)(pDib->m_dwHeight/m_fSizeRatio);
//						if( (m_imageRect.top + iImageHeight) - m_imageRect.bottom > 0)
//							iImageHeight -= (m_imageRect.top + iImageHeight) - m_imageRect.bottom;
//
//
//
//						pDib->BltDC( hDC, iImageX, m_imageRect.top, iImageWidth, iImageHeight, 
//							0, 0, (int)(iImageWidth*m_fSizeRatio), (int)(iImageHeight*m_fSizeRatio));
//
//						char strID[256];
//						sprintf( strID, "%d", iImageID);
//						::TextOut( hDC, iImageX + 5 , m_imageRect.top +5, strID, strlen( strID));
//
//						// draw selection rect.
//						if( iImageID == m_iSelectedImageID)
//						{
//							RECT rect;
//
//							SetRect( &rect, iImageX, m_imageRect.top, 
//								iImageX+iImageWidth, m_imageRect.top+iImageHeight);
//
//							HPEN hPen = ::CreatePen( PS_SOLID, 4, RGB( 50, 255, 50));
//							HPEN hOldPen = (HPEN)::SelectObject( hDC, hPen);
//
//							MoveToEx( hDC, rect.left+2, rect.top+2, NULL);
//							LineTo( hDC, rect.left+2, rect.bottom-2);
//							LineTo( hDC, rect.right-2, rect.bottom-2);
//							LineTo( hDC, rect.right-2, rect.top+2);
//							LineTo( hDC, rect.left+2, rect.top+2);
//			//				::Rectangle( hDC, m_selectionRect.left+2, m_selectionRect.top+2,
//			//					m_selectionRect.right-1, m_selectionRect.bottom-1);
//
//			//				::FrameRect( hDC, &m_selectionRect, NULL);
//
//			//				::DrawEdge( hDC, &m_selectionRect, EDGE_RAISED, BF_FLAT);
//
//			//				::DeleteObject( hBrush);
//							::SelectObject( hDC, hOldPen);
//							::DeleteObject( hPen);
//
//	//						::DeleteObject( hBrush);
//						}
//
//						iImageX += (int)(pDib->m_dwWidth/m_fSizeRatio + imageGap);
//
//						iImageID++;
//
//						if( iImageX > m_imageRect.right) break;	// 보이는 것만 그린다.
//					}
//					else
//					{
////						MessageBox( "Error");
//					}
//				}

			
				////////////////////////////////////////////////////
				// draw ani frames
				////////////////////////////////////////////////////
				iImageID = m_aniSlider.GetPos();
				pos = m_myData.m_aniList.FindIndex( m_myData.m_iAniIndex[ iImageID]);	


				iImageX = m_aniRect.left;
				iImageWidth = 0;
				iImageHeight = 0;
				while( pos)
				{
//					pos = m_myData.m_aniList.FindIndex( );
					CDib* pDib = m_myData.m_aniList.GetAt2( m_myData.m_iAniIndex[ iImageID]);

					if( pDib)
					{
						// 크기 조정.
						iImageWidth = (int)(pDib->m_dwWidth/m_fSizeRatio);
						if( (iImageX + iImageWidth) - m_aniRect.right > 0)
							iImageWidth -= (iImageX + iImageWidth) - m_aniRect.right;

						iImageHeight = (int)(pDib->m_dwHeight/m_fSizeRatio);
						if( (m_aniRect.top + iImageHeight) - m_aniRect.bottom > 0)
							iImageHeight -= (m_aniRect.top + iImageHeight) - m_aniRect.bottom;



						pDib->BltDC( hDC, iImageX, m_aniRect.top, iImageWidth, iImageHeight, 
							0, 0, iImageWidth, iImageHeight);

//						pDib->TransparentBltDC( hDC, iImageX, m_aniRect.top, iImageWidth, iImageHeight, 
//							0, 0, (int)(iImageWidth*m_fSizeRatio), (int)(iImageHeight*m_fSizeRatio));



						char strID[256];
						const char* szPathName = pDib->GetFileName();
						const char* szFileName = strrchr( szPathName, '\\');
						if( szFileName == NULL)
							szFileName = szPathName;
						else
							szFileName++;

						sprintf( strID, "%d-%s", m_myData.m_iAniIndex[ iImageID], szFileName);
						
//						if( m_myData.m_pCopyFlag[ iImageID])
//						{
//							::SetTextColor( hDC, RGB( 255, 0, 0));
//						}
//						else
//						{
//							::SetTextColor( hDC, RGB( 0, 0, 0));
//						}
						::TextOut( hDC, iImageX + 5, m_aniRect.top +5, strID, strlen( strID));

						// draw selection rect.
						if( iImageID == m_iSelectedAniID)
						{
							RECT rect;
							HBRUSH hBrush = ::CreateHatchBrush( HS_CROSS, 0);

							SetRect( &rect, iImageX, m_aniRect.top, 
								iImageX+iImageWidth, m_aniRect.top+iImageHeight);

							HPEN hPen = ::CreatePen( PS_SOLID, 4, RGB( 50, 255, 50));
							HPEN hOldPen = (HPEN)::SelectObject( hDC, hPen);

							MoveToEx( hDC, rect.left+2, rect.top+2, NULL);
							LineTo( hDC, rect.left+2, rect.bottom-2);
							LineTo( hDC, rect.right-2, rect.bottom-2);
							LineTo( hDC, rect.right-2, rect.top+2);
							LineTo( hDC, rect.left+2, rect.top+2);
						}

						iImageX += (int)(pDib->m_dwWidth/m_fSizeRatio + imageGap);

						iImageID++;

						if( iImageX > m_aniRect.right || iImageID >= m_myData.m_iAniLength) break;	// 보이는 것만 그린다.
					}
					else
					{
//						MessageBox( "Error");
					}

				}

				// draw selection rect
				if( m_bDrawSelectionRect)
				{
	//				HBRUSH hBrush = ::CreateHatchBrush( HS_CROSS, RGB( 255, 0, 0));
	//				HBRUSH hBrush = ::CreateSolidBrush( RGB( 255, 255, 0));
	//				::FillRect( hDC, &m_selectionRect, hBrush);
	//				::InvertRect( hDC, &m_selectionRect);
					HPEN hPen = ::CreatePen( PS_SOLID, 4, RGB( 255, 255, 0));
					HPEN hOldPen = (HPEN)::SelectObject( hDC, hPen);

					MoveToEx( hDC, m_selectionRect.left+2, m_selectionRect.top+2, NULL);
					LineTo( hDC, m_selectionRect.left+2, m_selectionRect.bottom-2);
					LineTo( hDC, m_selectionRect.right-2, m_selectionRect.bottom-2);
					LineTo( hDC, m_selectionRect.right-2, m_selectionRect.top+2);
					LineTo( hDC, m_selectionRect.left+2, m_selectionRect.top+2);
	//				::Rectangle( hDC, m_selectionRect.left+2, m_selectionRect.top+2,
	//					m_selectionRect.right-1, m_selectionRect.bottom-1);

	//				::FrameRect( hDC, &m_selectionRect, NULL);

	//				::DrawEdge( hDC, &m_selectionRect, EDGE_RAISED, BF_FLAT);

	//				::DeleteObject( hBrush);
					::SelectObject( hDC, hOldPen);
					::DeleteObject( hPen);
						
			
				}

			SelectObject( hDC, hOldRgn );

			}


//			pDC->BitBlt(m_aniSection.left, m_aniSection.top, m_aniSection.right - m_aniSection.left, m_aniSection.bottom - m_aniSection.top, &MemDC, m_aniSection.left, m_aniSection.top, SRCCOPY);
			pDC->BitBlt(ClientRect.left, ClientRect.top, ClientRect.right-ClientRect.left, ClientRect.bottom-ClientRect.top, &MemDC, 0,0, SRCCOPY);
		
			MemDC.SelectObject(pOldBitmap);

			ReleaseDC( pDC );

		}


	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSpriteManagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSpriteManagerDlg::OnButtonExit() 
{
	SaveCurrentSet();

	SendMessage( WM_CLOSE);
}



BOOL CSpriteManagerDlg::DestroyWindow() 
{

	


	return CDialog::DestroyWindow();
}

void CSpriteManagerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( (CSliderCtrl*)pScrollBar == &m_imageSliders) 
		InvalidateImage();
	else if( (CSliderCtrl*)pScrollBar == &m_aniSlider)
		InvalidateAni();


	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSpriteManagerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int id;
	int g = GetImgID( point, id);
	
	if( g == ANIMATION_SECTION)
	{
		if( id != -1)
		{
			m_bDragging = TRUE;
//				SetCursor( m_hDragCursor);
			m_iSelectedAniID = id/2;

			m_iDragFrom = ANIMATION_SECTION;

			InvalidateAni();
		}
	}
	else if( g== IMAGE_SECTION)
	{
		if( id != -1)
		{
			m_bDragging = TRUE;
//				SetCursor( m_hDragCursor);

			m_iSelectedImageID = id;	

			m_iDragFrom = IMAGE_SECTION;

			InvalidateImage( FALSE);
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CSpriteManagerDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{

	if( m_bDragging)
	{
		// drag & drop
		int imgID = -1;
		int section = GetImgID( point, imgID);

		if( m_iDragFrom == IMAGE_SECTION && section == IMAGE_SECTION)
		{
			// 무시
		}
		else if( m_iDragFrom == IMAGE_SECTION && section == ANIMATION_SECTION)
		{
			CDib* pSrcDib = NULL;
			pSrcDib  = m_imageList.GetAt2( m_iSelectedImageID);
			if( pSrcDib)
			{
				m_undo.SaveState( m_myData);

				if( imgID == -1)	// add tail
					AddToAni( pSrcDib->GetFileName(), -1, imgID%2);
				else
					AddToAni( pSrcDib->GetFileName(), imgID/2, imgID%2);

				InvalidateAni();
			}

		}
		else if( m_iDragFrom == ANIMATION_SECTION && section == IMAGE_SECTION)
		{
			// delete?

		}
		else if( m_iDragFrom == ANIMATION_SECTION && section == ANIMATION_SECTION)
		{
			CDib* pSrcDib = m_myData.m_aniList.GetAt2( m_myData.m_iAniIndex[ m_iSelectedAniID]);
			if( !((m_iSelectedAniID == imgID/2) && (imgID%2 == 0)))	// 같은 넘이 아닌 경우만..
			{
				m_undo.SaveState( m_myData);

				int iTargetPos = imgID/2;
				BOOL bAfter = imgID%2;

				if( GetAsyncKeyState( VK_CONTROL))
				{
					// add
					if( imgID == -1)	// add tail
						AddToAni( pSrcDib->GetFileName(), -1, bAfter);
					else
						AddToAni( pSrcDib->GetFileName(), iTargetPos, bAfter);
				}
				else
				{
					// change order...
					OnMenuDeleteAni();
//					memmove( m_myData.m_iAniIndex + m_iSelectedAniID, m_myData.m_iAniIndex + m_iSelectedAniID+1, 
//						sizeof(int)*(m_myData.m_iAniLength - m_iSelectedAniID-1));
//
//					m_myData.m_iAniLength--;
//
//					iTargetPos;


					// add
					if( imgID == -1)	// add tail
						AddToAni( pSrcDib->GetFileName(), -1, TRUE);
					else
						AddToAni( pSrcDib->GetFileName(), iTargetPos, FALSE);
				}

				InvalidateAni();
			}
		}
	}

	m_bDragging = FALSE;
//		SetCursor( LoadCursor(NULL, IDC_ARROW));

	// 그려진 selection mark를 지워야 하므로..
	if( m_bDrawSelectionRect)
	{
		m_bDrawSelectionRect = FALSE;
		Invalidate(FALSE);	// 음.. image가 아닌 영역에도 그릴 수 있으므로..예를 들어 gap
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CSpriteManagerDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{

	// dragging 해제..
	m_bDragging = FALSE;
//		SetCursor( LoadCursor(NULL, IDC_ARROW));

	// 그려진 selection mark를 지워야 하므로..
	if( m_bDrawSelectionRect)
	{
		m_bDrawSelectionRect = FALSE;
		Invalidate(FALSE);	// 음.. image가 아닌 영역에도 그릴 수 있으므로..예를 들어 gap
	}


	///////////////////////////////////////////
	// select image
	///////////////////////////////////////////
	int id;
	int g = GetImgID( point, id);
	if( g == ANIMATION_SECTION)
	{
		if( id != -1)
		{

//				m_bDragging = TRUE;
//				SetCursor( m_hDragCursor);
			m_iSelectedAniID = id/2;

//				m_iDragFrom = ANIMATION_SECTION;

			InvalidateAni();

			/////////////////////////////////////
			//	Show Popup Menu
			////////////////////////////////////
			CPoint menuPoint = point;
			ClientToScreen( &menuPoint);

			CMenu menu;
			menu.LoadMenu( IDR_MENU1);
			CMenu* pMenu = menu.GetSubMenu( 0);
			
			// Set Copy&Paste Enable
			if( m_bCopied)
				pMenu->EnableMenuItem( ID_PASTE, MF_ENABLED|MF_BYCOMMAND);
			else
				pMenu->EnableMenuItem( ID_PASTE, MF_DISABLED|MF_GRAYED|MF_BYCOMMAND);

			pMenu->TrackPopupMenu( TPM_RIGHTBUTTON | TPM_CENTERALIGN, menuPoint.x, menuPoint.y, this);
			

		}
	}
	else if( g== IMAGE_SECTION)
	{
		if( id != -1)
		{
//				m_bDragging = TRUE;
//				SetCursor( m_hDragCursor);

			m_iSelectedImageID = id;	

//				m_iDragFrom = IMAGE_SECTION;

			InvalidateImage( FALSE);

			/////////////////////////////////////
			//	Show Popup Menu
			////////////////////////////////////
			CPoint menuPoint = point;
			ClientToScreen( &menuPoint);

			CMenu menu;
			menu.LoadMenu( IDR_MENU1);
			CMenu* pMenu = menu.GetSubMenu( 1);
			pMenu->TrackPopupMenu( TPM_RIGHTBUTTON | TPM_CENTERALIGN, menuPoint.x, menuPoint.y, this);

		}
	}

	CDialog::OnRButtonDown(nFlags, point);

}

void CSpriteManagerDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	m_bDragging = FALSE;
	
	CDialog::OnRButtonUp(nFlags, point);
}

void CSpriteManagerDlg::OnMouseMove(UINT nFlags, CPoint point) 
{

	// 현재 포인터위의 물체 표시
	if( m_bDragging)
	{
//			SetCursor( m_hDragCursor);
		int id = -1;
		RECT selectionRect;
		int section = GetImgID( point, id, &selectionRect);

		if( section == ANIMATION_SECTION)
		{

			m_bDrawSelectionRect = TRUE;

			if( memcmp( &selectionRect, &m_selectionRect, sizeof(RECT)) )
			{
				m_selectionRect = selectionRect;
				InvalidateAni();
			}
		}
		else 
		{
			if( m_bDrawSelectionRect)
			{
				InvalidateAni();
			}
			m_bDrawSelectionRect = FALSE;
			memset( &m_selectionRect, 0, sizeof( RECT));

		}

	}

	
	CDialog::OnMouseMove(nFlags, point);
}


//=======================================================================================
//		이름 : GetImgID
//		설명 : point를 통해 선택된 image의 id를 return
//		animation영역인 경우 2로 나누어야 정확한 id가 나온다.
//		이유는 중간에 끼워넣기를 위해서 index을 하나 건너씩
//=======================================================================================
int CSpriteManagerDlg::GetImgID(const CPoint &point, int &imgID, RECT* pRect)
{
	imgID = -1;


	/////////////////////////////////////////////////////////
	//	Image Section에서 검사.
	/////////////////////////////////////////////////////////
	if( PtInRect( &m_imageRect, point) == TRUE)
	{
		// 선택된 image id를 얻는다.
		int iImageID = m_imageSliders.GetPos();
		POSITION pos = m_imageList.FindIndex( iImageID);	


		int iImageX = m_imageRect.left;
		int iImageWidth = 0;
		int iImageHeight = 0;
		while( pos)
		{

			CDib* pDib = m_imageList.GetNext( pos);

			// 크기 조정.
			iImageWidth = (int)(pDib->m_dwWidth/m_fSizeRatio);
			if( (iImageX + iImageWidth) - m_imageRect.right > 0)
				iImageWidth -= (iImageX + iImageWidth) - m_imageRect.right;


			iImageHeight = (int)(pDib->m_dwHeight/m_fSizeRatio);
			if( (m_imageRect.top + iImageHeight) - m_imageRect.bottom > 0)
				iImageHeight -= (m_imageRect.top + iImageHeight) - m_imageRect.bottom;



			// check
			RECT rect;


			SetRect( &rect, iImageX, m_imageRect.top, 
				iImageX+iImageWidth, m_imageRect.top+iImageHeight);

			if( PtInRect( &rect, point) == TRUE)
			{
				imgID = iImageID;
				if( pRect)
					*pRect = rect;
				break;
			}


			iImageX += (int)(pDib->m_dwWidth/m_fSizeRatio + imageGap);

			iImageID++;

			if( iImageX > m_imageRect.right) break;	// 보이는 것만 
		}
		return IMAGE_SECTION;

	}
	/////////////////////////////////////////////////////////
	//	Animation Section에서 검사.
	/////////////////////////////////////////////////////////
	else if( PtInRect( &m_aniRect, point) == TRUE)
	{
		// 선택된 image id를 얻는다.
		int iImageID = m_aniSlider.GetPos();
		POSITION pos = m_myData.m_aniList.FindIndex( m_myData.m_iAniIndex[ iImageID]);	

		// 아무것도 없는 경우를 대비하여 전체 영역으로 설정.
		if( pRect)
			SetRect( pRect, m_aniRect.left, m_aniRect.top, 
				m_aniRect.right, m_aniRect.bottom);


		int iImageX = m_aniRect.left;
		int iImageWidth = 0;
		int iImageHeight = 0;
		while( pos)
		{

			CDib* pDib = m_myData.m_aniList.GetAt2( m_myData.m_iAniIndex[iImageID]);
//			pos = m_myData.m_aniList

			// 크기 조정.
			iImageWidth = (int)(pDib->m_dwWidth/m_fSizeRatio);
			if( (iImageX + iImageWidth) - m_aniRect.right > 0)
				iImageWidth -= (iImageX + iImageWidth) - m_aniRect.right;

			iImageHeight = (int)(pDib->m_dwHeight/m_fSizeRatio);
			if( (m_aniRect.top + iImageHeight) - m_aniRect.bottom > 0)
				iImageHeight -= (m_aniRect.top + iImageHeight) - m_aniRect.bottom;

			// check
			RECT rect;

			SetRect( &rect, iImageX, m_aniRect.top, 
				iImageX+iImageWidth, m_aniRect.top+iImageHeight);

			if( PtInRect( &rect, point) == TRUE)
			{
				imgID = iImageID*2;
				if( pRect)
					*pRect = rect;
				break;
			}
			else
			{
				// check gap..
				if( pos == NULL)	// 마지막이면.. 남은 모든 영역을 포함.
					SetRect( &rect, iImageX+iImageWidth, m_aniRect.top, 
						m_aniRect.right, m_aniRect.top+iImageHeight);
				else
					SetRect( &rect, iImageX+iImageWidth, m_aniRect.top, 
						iImageX+iImageWidth + imageGap, m_aniRect.top+iImageHeight);

				if( PtInRect( &rect, point) == TRUE)
				{
					imgID = iImageID*2+1;
					if( pRect)
						*pRect = rect;
					break;
				}
			}


			iImageX += (int)(pDib->m_dwWidth/m_fSizeRatio + imageGap);

			iImageID++;

			if( iImageX > m_aniRect.right || iImageID >= m_myData.m_iAniLength) break;	// 보이는 것만 
		}
		return ANIMATION_SECTION;
	}

	return -1;
}


void CSpriteManagerDlg::OnButtonPlay() 
{


	m_aniDlg.ShowWindow( SW_SHOW);
//	aniDlg.DoModal();

/*
	if( m_bPlayingAni)
	{
		m_bPlayingAni = FALSE;
		SetDlgItemText( IDC_BUTTON_PLAY, "Play");
		Invalidate();
	}
	else
	{
		m_bPlayingAni = TRUE;
//		m_myData.m_iAniLength = m_myData.m_aniList.GetCount();
		m_iAniTime = 0;
		SetDlgItemText( IDC_BUTTON_PLAY, "Stop");
	}
*/	
}

void CSpriteManagerDlg::InvalidateAni(BOOL bErase)
{
	InvalidateRect( &m_aniSection, bErase);

//	UpdateInfo();
}

void CSpriteManagerDlg::InvalidateImage(BOOL bErase)
{
	InvalidateRect( &m_imageSection, bErase);
}

void CSpriteManagerDlg::OnMenuDeleteAllAnis() 
{
	if( !m_bConfirmDelete || MessageBox( "지울까요?", "확인", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		m_undo.SaveState( m_myData);

		m_myData.m_aniList.RemoveAll();

		m_myData.m_iAniLength = 0;
		memset( m_myData.m_iAniIndex, 0, sizeof( m_myData.m_iAniIndex));

		m_AniList.ResetContent();

		InvalidateAni();
		m_aniSlider.SetRange( 0, 0, TRUE);

		m_bDirty = TRUE;
	}
	
}

void CSpriteManagerDlg::OnMenuDeleteAni() 
{
/*
	POSITION pos = m_myData.m_aniList.FindIndex( m_iSelectedAniID);
	if( pos == NULL)
		return;
*/

//	if( !m_bConfirmDelete || MessageBox( strID, "삭제", MB_YESNO | MB_ICONQUESTION) == IDYES)
//	{
		if(m_bUndoStop == false)m_undo.SaveState( m_myData);

		int delID = -1;
		// fix me.. m_myData.m_iAniIndex를 조사하여 ref가 없으면 삭제..

/*	무조건 안지운다..
	다른 디렉션에 대해서도 모두 지워야하고..
	어차피 지워도 다른 곳에서 사용된다고 가정..

		// find reference
		for( int i = 0; i< m_myData.m_iAniLength; i++)
		{
			if( i != m_iSelectedAniID && m_myData.m_iAniIndex[ m_iSelectedAniID] == m_myData.m_iAniIndex[ i] )
			{
				break;
			}
		}
		if( i == m_myData.m_iAniLength)	// 찾지 못했으므로 그림 자체를 지운다.
		{
			delID = m_myData.m_iAniIndex[ m_iSelectedAniID];
			m_myData.m_aniList.RemoveAt( m_myData.m_iAniIndex[ m_iSelectedAniID]);
		}
*/
		


		memmove( m_myData.m_iAniIndex + m_iSelectedAniID, m_myData.m_iAniIndex + m_iSelectedAniID+1, 
			sizeof(int)*(m_myData.m_iAniLength - m_iSelectedAniID -1));

		memmove( m_myData.m_pCopyFlag + m_iSelectedAniID, m_myData.m_pCopyFlag + m_iSelectedAniID+1, 
			sizeof(char)*(m_myData.m_iAniLength - m_iSelectedAniID -1));

		m_myData.m_iAniLength--;

		//LIST에서 삭제
		m_AniList.DeleteString(m_iSelectedAniID);
	
/*	bmp를 지우지 않으므로.. 할 이유가 없다.
		/////////////////////////////////////////////////////////////////////////////
		// m_myData.m_aniList에서 삭제가 일어난 경우, m_myData.m_iAniIndex의 내용을 수정해야 한다.
		if( delID != -1)
		{
			// m_myData.m_iAniIndex reindex
			for( i = 0; i< m_myData.m_iAniLength; i++)
			{
				// 지워진 id와  같거나 큰 것을 가리키는 것을 
				// 수정해야 한다.
				if( m_myData.m_iAniIndex[i] >= delID) 
					m_myData.m_iAniIndex[i]--;
			}
			m_myData.m_iBmpsCount--;
		}
*/
		m_bDirty = TRUE;

		
		InvalidateAni();
		m_aniSlider.SetRange( 0, m_myData.m_iAniLength-1, TRUE);

//	}

}

void CSpriteManagerDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	
	
	////////////////////////////////////////////////////
	// Kill Timer
	////////////////////////////////////////////////////
	KillTimer( TIMER_ID);

	if( m_bDirty && MessageBox( "저장할까요?", NULL, MB_ICONQUESTION|MB_YESNO) == IDYES)
	{
		SaveDataFile();

	}


	DeleteObject( m_hDragCursor);
	DeleteObject( m_hDragCursorAdd);


	FlushAniList();
	FlushImageList();

	
}

void CSpriteManagerDlg::OnTimer(UINT nIDEvent) 
{

	if( GetActiveWindow() == this)
	{
		///////////////////////////////////////////////////
		// scroll ani slider
		///////////////////////////////////////////////////
		#define SCROLL_TIME		1
		static DWORD dwTickCount = 0;
		if( dwTickCount++ > SCROLL_TIME)
		{
			POINT mousePt;
			GetCursorPos( &mousePt);
			ScreenToClient( &mousePt);

			int pos = m_aniSlider.GetPos();

			//임계 영역에 있는지 확인..
			RECT rect = m_aniRect;

			rect.right = rect.left + 20;
			if( PtInRect( &rect, mousePt))	// left scroll
			{
				if( pos > 0)
				{
					pos--;

					m_aniSlider.SetPos( pos);
					InvalidateAni();
				}
			}

			rect = m_aniRect;
			rect.left = m_aniRect.right - 20;

			if( PtInRect( &rect, mousePt))	// right scroll
			{
				if( pos < m_myData.m_iAniLength - 1)
				{
					pos++;

					m_aniSlider.SetPos( pos);
					InvalidateAni();
				}
			}


			dwTickCount = 0;
		}
extern CSpriteManagerApp theApp;
		theApp.OnIdle(1);
	}
	CDialog::OnTimer(nIDEvent);
}



//=======================================================================================
//		
//		combo박스 변화를 적용.
//		
//=======================================================================================
void CSpriteManagerDlg::ResetCombos()
{
	int i = 0;

	static int type = 0;
	static BOOL bFirst = TRUE;
	if( bFirst)
	{
		bFirst = FALSE;

		m_comboType.ResetContent();
		for( i = 0; i< TYPE_MAX; i++)
			m_comboType.AddString( strCreatureType[i]);

		m_comboType.SetCurSel( 0);

		m_comboPart.ResetContent();
		for( i = 0; i< SLAYER_PART_MAX; i++)
			m_comboPart.AddString( strSlayerPart[i]);
		m_comboPart.SetCurSel( 0);

		m_comboAction.ResetContent();
		for( i = 0; i< ACTION_MAX; i++)
			m_comboAction.AddString( strAction[i]);
		m_comboAction.SetCurSel( 0);

		m_comboDirection.ResetContent();
		for( i = 0; i< DIR_MAX; i++)
			m_comboDirection.AddString( strDirection[i]);
		m_comboDirection.SetCurSel( 0);

	}
//	else
	{
		CString prevAction, prevPart;
		m_comboAction.GetLBText( m_comboAction.GetCurSel(), prevAction);
		m_comboPart.GetLBText( m_comboPart.GetCurSel(), prevPart);

		if( IsSlayerType( m_dwType))	// slayer인 경우..
		{

			type = m_comboType.GetCurSel();
			m_comboType.ResetContent();
			for( i = 0; i< TYPE_MAX; i++)
				m_comboType.AddString( strCreatureType[i]);
			m_comboType.SetCurSel( type);						// type combo를 바꾼경우 현재설정은 변경X

			m_comboAction.ResetContent();
			for( i = 0; i< SLAYER_ACTION_MAX; i++)
			{
//				if( actionMask[ i] & SLAYER_ACTION)
//				{
					m_comboAction.AddString( strAction[slayerActionIndex[i]]);
//				}
			}
			// find new selection
			type = 0;
			for( i = 0; i< m_comboAction.GetCount(); i++)
			{
				CString lbText; m_comboAction.GetLBText( i, lbText);
				if( prevAction == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboAction.SetCurSel( type);

			m_comboPart.ResetContent();
			for(i = 0; i < SLAYER_PART_MAX; i++)
			{
				m_comboPart.AddString( strSlayerPart[i] );
			}
			// find new selection
			type = 0;
			for( i = 0; i< m_comboPart.GetCount(); i++)
			{
				CString lbText; m_comboPart.GetLBText( i, lbText);
				if( prevPart == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboPart.SetCurSel( type);

			m_comboPart.EnableWindow( TRUE);


		}
		else if(IsVampireType(m_dwType))	// 뱀파이어인 경우.
		{

			type = m_comboType.GetCurSel();
			m_comboType.ResetContent();
			for( i = 0; i< TYPE_MAX; i++)
				m_comboType.AddString( strCreatureType[i]);
			m_comboType.SetCurSel( type);						// type combo를 바꾼경우 현재설정은 변경X


			type = 0;
			m_comboAction.ResetContent();
			for( i = 0; i< VAMPIRE_ACTION_MAX; i++)
			{
					m_comboAction.AddString( strAction[vampireActionIndex[i]]);
			}
			// find new selection
			type = 0;
			for( i = 0; i< m_comboAction.GetCount(); i++)
			{
				CString lbText; m_comboAction.GetLBText( i, lbText);
				if( prevAction == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboAction.SetCurSel( type);

			m_comboPart.ResetContent();
			for(i = 0; i < VAMPIRE_PART_MAX; i++)
			{
				m_comboPart.AddString( strVampirePart[i] );
			}
			// find new selection
			type = 0;
			for( i = 0; i< m_comboPart.GetCount(); i++)
			{
				CString lbText; m_comboPart.GetLBText( i, lbText);
				if( prevPart == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboPart.SetCurSel( type);

			m_comboPart.EnableWindow( TRUE);


		}
		else if(IsOustersType(m_dwType))	// 뱀파이어인 경우.
		{

			type = m_comboType.GetCurSel();
			m_comboType.ResetContent();
			for( i = 0; i< TYPE_MAX; i++)
				m_comboType.AddString( strCreatureType[i]);
			m_comboType.SetCurSel( type);						// type combo를 바꾼경우 현재설정은 변경X
			type = 0;
			m_comboAction.ResetContent();
			for( i = 0; i< OUSTERS_ACTION_MAX; i++)
			{
					m_comboAction.AddString( strOustersAction[oustersActionIndex[i]]);
			}
			// find new selection
			type = 0;
			for( i = 0; i< m_comboAction.GetCount(); i++)
			{
				CString lbText; m_comboAction.GetLBText( i, lbText);
				if( prevAction == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboAction.SetCurSel( type);

			m_comboPart.ResetContent();
			for(i = 0; i < OUSTERS_PART_MAX; i++)
			{
				m_comboPart.AddString( strOustersPart[i] );
			}
			// find new selection
			type = 0;
			for( i = 0; i< m_comboPart.GetCount(); i++)
			{
				CString lbText; m_comboPart.GetLBText( i, lbText);
				if( prevPart == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboPart.SetCurSel( type);

			m_comboPart.EnableWindow( TRUE);

		}
		else if(IsNPCType(m_dwType))	//NPC인경우
		{

			type = m_comboType.GetCurSel();
			m_comboType.ResetContent();
			for( i = 0; i< TYPE_MAX; i++)
				m_comboType.AddString( strCreatureType[i]);
			m_comboType.SetCurSel( type);						// type combo를 바꾼경우 현재설정은 변경X


			type = 0;
			m_comboAction.ResetContent();
			for( i = 0; i< NPC_ACTION_MAX; i++)
			{
					m_comboAction.AddString( strAction[npcActionIndex[i]]);
			}
			// find new selection
			type = 0;
			for( i = 0; i< m_comboAction.GetCount(); i++)
			{
				CString lbText; m_comboAction.GetLBText( i, lbText);
				if( prevAction == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboAction.SetCurSel( type);


			m_comboPart.EnableWindow( FALSE);
		}
		else if( IsPromotionSlayer( m_dwType))	// slayer인 경우..
		{
			type = m_comboType.GetCurSel();
			m_comboType.ResetContent();
			for( i = 0; i< TYPE_MAX; i++)
				m_comboType.AddString( strCreatureType[i]);
			m_comboType.SetCurSel( type);						// type combo를 바꾼경우 현재설정은 변경X

			m_comboAction.ResetContent();
			for( i = 0; i< ACTION_SLAYER_PROMOTION_MAX; i++)
				m_comboAction.AddString( strPromotionSlayerAction[g_PromotionSlayerActionIndex[i]]);

			// find new selection
			type = 0;
			for( i = 0; i< m_comboAction.GetCount(); i++)
			{
				CString lbText; m_comboAction.GetLBText( i, lbText);
				if( prevAction == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboAction.SetCurSel( type);

			m_comboPart.ResetContent();
			for(i = 0; i < PS_PART_MAX; i++)
			{
				m_comboPart.AddString( strPromotionSlayerPart[i] );
			}
			// find new selection
			type = 0;
			for( i = 0; i< m_comboPart.GetCount(); i++)
			{
				CString lbText; m_comboPart.GetLBText( i, lbText);
				if( prevPart == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboPart.SetCurSel( type);

			m_comboPart.EnableWindow( TRUE);


		}
		else if(IsPromotionOusters(m_dwType))	// 뱀파이어인 경우.
		{

			type = m_comboType.GetCurSel();
			m_comboType.ResetContent();
			for( i = 0; i< TYPE_MAX; i++)
				m_comboType.AddString( strCreatureType[i]);
			m_comboType.SetCurSel( type);						// type combo를 바꾼경우 현재설정은 변경X
			type = 0;
			m_comboAction.ResetContent();
			for( i = 0; i< ACTION_OUSTERS_PROMOTION_MAX; i++)
			{
				m_comboAction.AddString( strPromotionOustersAction[g_PromotionOustersActionIndex[i]]);
			}
			// find new selection
			type = 0;
			for( i = 0; i< m_comboAction.GetCount(); i++)
			{
				CString lbText; m_comboAction.GetLBText( i, lbText);
				if( prevAction == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboAction.SetCurSel( type);

			m_comboPart.ResetContent();
			for(i = 0; i < PO_PART_MAX; i++)
			{
				m_comboPart.AddString( strOustersPart[i] );
			}
			// find new selection
			type = 0;
			for( i = 0; i< m_comboPart.GetCount(); i++)
			{
				CString lbText; m_comboPart.GetLBText( i, lbText);
				if( prevPart == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboPart.SetCurSel( type);

			m_comboPart.EnableWindow( TRUE);

		}
		else if( IsPromotionVampire( m_dwType ) )
		{
			type = m_comboType.GetCurSel();
			m_comboType.ResetContent();
			for( i = 0; i< TYPE_MAX; i++)
				m_comboType.AddString( strCreatureType[i]);
			m_comboType.SetCurSel( type);						// type combo를 바꾼경우 현재설정은 변경X
			type = 0;
			m_comboAction.ResetContent();
			for( i = 0; i< ACTION_ADVANCEMENT_MAX; i++)
			{
				m_comboAction.AddString( strAdvancementClassVampireAction[g_AdvancementClassVampireActionIndex[i]]);
			}
			// find new selection
			type = 0;
			for( i = 0; i< m_comboAction.GetCount(); i++)
			{
				CString lbText; m_comboAction.GetLBText( i, lbText);
				if( prevAction == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboAction.SetCurSel( type);

			m_comboPart.ResetContent();
			for(i = 0; i < AC_PART_MAX; i++)
			{
				m_comboPart.AddString( strAdvancementClassVampirePart[i] );
			}
			// find new selection
			type = 0;
			for( i = 0; i< m_comboPart.GetCount(); i++)
			{
				CString lbText; m_comboPart.GetLBText( i, lbText);
				if( prevPart == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboPart.SetCurSel( type);

			m_comboPart.EnableWindow( TRUE);
		}
		else
		{

			type = m_comboType.GetCurSel();
			m_comboType.ResetContent();
			for( i = 0; i< TYPE_MAX; i++)
				m_comboType.AddString( strCreatureType[i]);
			m_comboType.SetCurSel( type);						// type combo를 바꾼경우 현재설정은 변경X


			type = 0;
			m_comboAction.ResetContent();
			for( i = 0; i< MIDLEVEL_MOB_ACTION_MAX; i++)
			{
					m_comboAction.AddString( strAction[midLevelMobActionIndex[i]]);
			}
			// find new selection
			type = 0;
			for( i = 0; i< m_comboAction.GetCount(); i++)
			{
				CString lbText; m_comboAction.GetLBText( i, lbText);
				if( prevAction == lbText)
				{
					type = i;
					break;
				}
			}
			m_comboAction.SetCurSel( type);


			m_comboPart.EnableWindow( FALSE);
		}
	}



/*
	switch( m_dwType)
	{
	case CREATURE_SLAYER_MAN:
	case CREATURE_SLAYER_WOMAN:
		{
			m_comboAction.ResetContent();
			for( i = 0; i< SLAYER_MAX_ACTION
		}
		break;
	}
	*/
}


/*
//=======================================================================
//	이름 : DoSpecialAction
//	설명 : 오토바이 동작에 관한 자동처리...


//				파트		몸땡이			오토바이		몸땡이아닌부분
//			액션
//
//			오토바이정지	4개				이동1번*4		1개*4
//			오토바이이동	정지1번*4		4개				정지1번*4
//			
//
//=======================================================================
void CSpriteManagerDlg::DoSpecialAction( DWORD type)
{
	CString searchStr, newData;
	DWORD i, dir;
	int t;
	for( DWORD part = 0; part <PART_MAX; part++)
	{
		if( isBodyPart( part))
		{
		
			///////////////////////////////////////
			//	몸땡이
#ifdef _DEBUG
			// ani frame data에 추가.
			searchStr = GetSearchString( type, ACTION_BIKE_STOP, part);

			newData = searchStr;
			newData += (BYTE)BIKE_FRAME_COUNT;
			for( i = 0; i< BIKE_FRAME_COUNT; i++)
			{
				newData += (BYTE)(1);	// +1 -> string이므로.....
			}
			if( (t = m_strAniData.Find( searchStr, 0)) == -1)
			{
			}
			else
			{
				// 제거.
				CString temp = m_strAniData[t ];
				int dataLen = (BYTE)m_strAniData[t + searchStr.GetLength()];
				m_strAniData.Delete( t, dataLen + searchStr.GetLength()+1);	// +1은 dataLen 
			}
#endif


			// ani frame data에 추가.
			searchStr = GetSearchString( type, ACTION_BIKE_MOVE, part);

			newData = searchStr;
			newData += (BYTE)BIKE_FRAME_COUNT;
			for( i = 0; i< BIKE_FRAME_COUNT; i++)
			{
				newData += (BYTE)(1);	// +1 -> string이므로.....
			}
			if( (t = m_strAniData.Find( searchStr, 0)) == -1)
			{
			}
			else
			{
				// 제거.
				int dataLen = (BYTE)m_strAniData[t + searchStr.GetLength()];
				m_strAniData.Delete( t, dataLen + searchStr.GetLength()+1);	// +1은 dataLen 
			}
			// 추가.
			m_strAniData += newData;


			// bmp filename data에 추가.
			for( dir = 0; dir < DIR_MAX; dir++)
			{
				CString fileName;
				// stop의 bmp를 찾아서.. 첫번째 bmp filename을 얻는다.
				searchStr = GetSearchString( type, ACTION_BIKE_STOP, part, dir);

				if( (t = m_strData.Find( searchStr, 0)) == -1)
				{
				}
				else
				{
					// 읽기.
					t+= searchStr.GetLength();
					m_myData.m_iBmpsCount = (int)m_strData[t++];

					t++;	// skip '<'
					int fileNameLen = m_strData.Find( '>', t) - t;
					fileName = m_strData.Mid( t, fileNameLen);
				}
				if( fileName != "")
				{
					// 저장.
					searchStr = GetSearchString( type, ACTION_BIKE_MOVE, part, dir);;
					newData = searchStr;
					newData += (BYTE)1;	// 수 

					newData += "<";
					newData += fileName;
					newData += ">";
					newData += "\n";

					int t;
					if( (t = m_strData.Find( searchStr, 0)) == -1)
					{
						// 끝부분에 추가.
						m_strData += newData;
					}
					else
					{
						// 찾은 경우 
						int end;
						// 제거.
						if( ( end = m_strData.Find("\n", t)) != -1)
						{
							m_strData.Delete( t, end - t + 1);
						}
						else
						{
							MessageBox( "Parsing Error : SaveCurrentSet");
							return;
						}
					}
					// 추가.
					m_strData += newData;
				}

				// 그림자.
				searchStr = GetSearchString( type, ACTION_BIKE_STOP, part, dir);
				if( (t = m_strShadowData.Find( searchStr, 0)) == -1)
				{
				}
				else
				{
					// 읽기.
					t+= searchStr.GetLength();
					m_myData.m_iBmpsCount = (int)m_strData[t++];

					t++;	// skip '<'
					int fileNameLen = m_strShadowData.Find( '>', t) - t;
					fileName = m_strShadowData.Mid( t, fileNameLen);
				}
				if( fileName != "")
				{
					// 저장.
					searchStr = GetSearchString( type, ACTION_BIKE_MOVE, part, dir);
					newData = searchStr;
					newData += (BYTE)1;	// 수 

					newData += "<";
					newData += fileName;
					newData += ">";
					newData += "\n";

					int t;
					if( (t = m_strShadowData.Find( searchStr, 0)) == -1)
					{
						// 끝부분에 추가.
						m_strShadowData += newData;
					}
					else
					{
						// 찾은 경우 
						int end;
						// 제거.
						if( ( end = m_strShadowData.Find("\n", t)) != -1)
						{
							m_strShadowData.Delete( t, end - t + 1);
						}
						else
						{
							MessageBox( "Parsing Error : SaveCurrentSet");
							return;
						}
					}
					// 추가.
					m_strShadowData += newData;
				}
			}
		}
		else if( part == PART_BIKE)
		{
			///////////////////////////////////////
			//	오토바이

			// ani frame data에 추가.
			searchStr = GetSearchString( type, ACTION_BIKE_STOP, part);

			newData = searchStr;
			newData += (BYTE)BIKE_FRAME_COUNT;
			for( i = 0; i< BIKE_FRAME_COUNT; i++)
			{
				newData += (BYTE)(1);	// +1 -> string이므로.....
			}
			if( (t = m_strAniData.Find( searchStr, 0)) == -1)
			{
			}
			else
			{
				// 제거.
				int dataLen = (BYTE)m_strAniData[t + searchStr.GetLength()];
				m_strAniData.Delete( t, dataLen + searchStr.GetLength()+1);	// +1은 dataLen 
			}
			// 추가.
			m_strAniData += newData;


			// bmp filename data에 추가.
			for( dir = 0; dir < DIR_MAX; dir++)
			{
				CString fileName;
				// stop의 bmp를 찾아서.. 첫번째 bmp filename을 얻는다.
				searchStr = GetSearchString( type, ACTION_BIKE_MOVE, part, dir);

				if( (t = m_strData.Find( searchStr, 0)) == -1)
				{
				}
				else
				{
					// 읽기.
					t+= searchStr.GetLength();
					m_myData.m_iBmpsCount = (int)m_strData[t++];

					t++;	// skip '<'
					int fileNameLen = m_strData.Find( '>', t) - t;
					fileName = m_strData.Mid( t, fileNameLen);
				}
				if( fileName != "")
				{
					// 저장.
					searchStr = GetSearchString( type, ACTION_BIKE_STOP, part, dir);;
					newData = searchStr;
					newData += (BYTE)1;	// 수 

					newData += "<";
					newData += fileName;
					newData += ">";
					newData += "\n";

					int t;
					if( (t = m_strData.Find( searchStr, 0)) == -1)
					{
						// 끝부분에 추가.
						m_strData += newData;
					}
					else
					{
						// 찾은 경우 
						int end;
						// 제거.
						if( ( end = m_strData.Find("\n", t)) != -1)
						{
							m_strData.Delete( t, end - t + 1);
						}
						else
						{
							MessageBox( "Parsing Error : SaveCurrentSet");
							return;
						}
					}
					// 추가.
					m_strData += newData;
				}

				// 그림자.
				searchStr = GetSearchString( type, ACTION_BIKE_MOVE, part, dir);
				if( (t = m_strShadowData.Find( searchStr, 0)) == -1)
				{
				}
				else
				{
					// 읽기.
					t+= searchStr.GetLength();
					m_myData.m_iBmpsCount = (int)m_strData[t++];

					t++;	// skip '<'
					int fileNameLen = m_strShadowData.Find( '>', t) - t;
					fileName = m_strShadowData.Mid( t, fileNameLen);
				}
				if( fileName != "")
				{
					// 저장.
					searchStr = GetSearchString( type, ACTION_BIKE_STOP, part, dir);;
					newData = searchStr;
					newData += (BYTE)1;	// 수 

					newData += "<";
					newData += fileName;
					newData += ">";
					newData += "\n";

					int t;
					if( (t = m_strShadowData.Find( searchStr, 0)) == -1)
					{
						// 끝부분에 추가.
						m_strShadowData += newData;
					}
					else
					{
						// 찾은 경우 
						int end;
						// 제거.
						if( ( end = m_strShadowData.Find("\n", t)) != -1)
						{
							m_strShadowData.Delete( t, end - t + 1);
						}
						else
						{
							MessageBox( "Parsing Error : SaveCurrentSet");
							return;
						}
					}
					// 추가.
					m_strShadowData += newData;
				}
			}
		}
		else
		{
			///////////////////////////////////////
			//	그외..(무기)

			/////////////////////////////////////////
			//	bike stop에 ani frame추가.
			//
			searchStr = GetSearchString( type, ACTION_BIKE_STOP, part);
			newData = searchStr;
			newData += (BYTE)BIKE_FRAME_COUNT;
			for( i = 0; i< BIKE_FRAME_COUNT; i++)
			{
				newData += (BYTE)(1);	// +1 -> string이므로.....
			}
			if( (t = m_strAniData.Find( searchStr, 0)) == -1)
			{
			}
			else
			{
				// 제거.
				int dataLen = (BYTE)m_strAniData[t + searchStr.GetLength()];
				m_strAniData.Delete( t, dataLen + searchStr.GetLength()+1);	// +1은 dataLen 
			}
			// 추가.
			m_strAniData += newData;

			/////////////////////////////////////////
			//	bike move에 ani frame추가.
			//
			searchStr = GetSearchString( type, ACTION_BIKE_MOVE, part);
			newData = searchStr;
			newData += (BYTE)BIKE_FRAME_COUNT;
			for( i = 0; i< BIKE_FRAME_COUNT; i++)
			{
				newData += (BYTE)(1);	// +1 -> string이므로.....
			}
			if( (t = m_strAniData.Find( searchStr, 0)) == -1)
			{
			}
			else
			{
				// 제거.
				int dataLen = (BYTE)m_strAniData[t + searchStr.GetLength()];
				m_strAniData.Delete( t, dataLen + searchStr.GetLength()+1);	// +1은 dataLen 
			}
			// 추가.
			m_strAniData += newData;



			// bmp filename data에 추가.
			for( dir = 0; dir < DIR_MAX; dir++)
			{
				CString fileName;
				// stop의 bmp를 찾아서.. 첫번째 bmp filename을 얻는다.
				searchStr = GetSearchString( type, ACTION_BIKE_STOP, part, dir);

				if( (t = m_strData.Find( searchStr, 0)) == -1)
				{
				}
				else
				{
					// 읽기.
					t+= searchStr.GetLength();
					m_myData.m_iBmpsCount = (int)m_strData[t++];

					t++;	// skip '<'
					int fileNameLen = m_strData.Find( '>', t) - t;
					fileName = m_strData.Mid( t, fileNameLen);
				}
				if( fileName != "")
				{
					// 저장.
					searchStr = GetSearchString( type, ACTION_BIKE_MOVE, part, dir);;
					newData = searchStr;
					newData += (BYTE)1;	// 수 

					newData += "<";
					newData += fileName;
					newData += ">";
					newData += "\n";

					int t;
					if( (t = m_strData.Find( searchStr, 0)) == -1)
					{
						// 끝부분에 추가.
						m_strData += newData;
					}
					else
					{
						// 찾은 경우 
						int end;
						// 제거.
						if( ( end = m_strData.Find("\n", t)) != -1)
						{
							m_strData.Delete( t, end - t + 1);
						}
						else
						{
							MessageBox( "Parsing Error : SaveCurrentSet");
							return;
						}
					}
					// 추가.
					m_strData += newData;
				}
				// 그림자.
				searchStr = GetSearchString( type, ACTION_BIKE_STOP, part, dir);
				if( (t = m_strShadowData.Find( searchStr, 0)) == -1)
				{
				}
				else
				{
					// 읽기.
					t+= searchStr.GetLength();
					m_myData.m_iBmpsCount = (int)m_strData[t++];

					t++;	// skip '<'
					int fileNameLen = m_strShadowData.Find( '>', t) - t;
					fileName = m_strShadowData.Mid( t, fileNameLen);
				}
				if( fileName != "")
				{
					// 저장.
					searchStr = GetSearchString( type, ACTION_BIKE_MOVE, part, dir);;
					newData = searchStr;
					newData += (BYTE)1;	// 수 

					newData += "<";
					newData += fileName;
					newData += ">";
					newData += "\n";

					int t;
					if( (t = m_strShadowData.Find( searchStr, 0)) == -1)
					{
						// 끝부분에 추가.
						m_strShadowData += newData;
					}
					else
					{
						// 찾은 경우 
						int end;
						// 제거.
						if( ( end = m_strShadowData.Find("\n", t)) != -1)
						{
							m_strShadowData.Delete( t, end - t + 1);
						}
						else
						{
							MessageBox( "Parsing Error : SaveCurrentSet");
							return;
						}
					}
					// 추가.
					m_strShadowData += newData;
				}
			}
		}
	}
}
*/
//=======================================================================================
//		
//		현재 set을 save..
//		
//=======================================================================================
void CSpriteManagerDlg::SaveCurrentSet()
{
	if( m_myData.m_iAniLength == 0 || m_myData.m_iBmpsCount == 0) 
		return;

	int i;



	// 검색어 찾기.
	CString searchStr = GetSearchString( m_dwType, m_dwAction, m_dwPart, m_dwDirection);


	// 새로운 data를 만든다.
	CString newData = searchStr;
	newData += (BYTE)m_myData.m_iBmpsCount;	// 수 
	for( i = 0; i< m_myData.m_iBmpsCount; i++)
	{
		POSITION pos = m_myData.m_aniList.FindIndex( i);
		newData += "<";
		newData += m_myData.m_aniList.GetAt( pos)->GetFileName();
		newData += ">";
	}
	newData += "\n";


	int t;
	if( (t = m_strData.Find( searchStr, 0)) == -1)
	{
		// 끝부분에 추가.
		m_strData += newData;
	}
	else
	{
		// 찾은 경우 
		int end;
		// 제거.
		if( ( end = m_strData.Find("\n", t)) != -1)
		{
			m_strData.Delete( t, end - t + 1);
		}
		else
		{
			MessageBox( "Parsing Error : SaveCurrentSet");
			return;
		}
	}
	// 추가.
	m_strData += newData;


	// save ani index data
	searchStr = GetSearchString( m_dwType, m_dwAction, m_dwPart);

	newData = searchStr;
	newData += (BYTE)m_myData.m_iAniLength;
	for( i = 0; i< m_myData.m_iAniLength; i++)
	{
		newData += (BYTE)(m_myData.m_iAniIndex[ i]+1);	// +1 -> string이므로.....
	}


	if( (t = m_strAniData.Find( searchStr, 0)) == -1)
	{
		// fix me 임시로 추가해야쥐..
	}
	else
	{
		// 찾은 경우 

		// 제거.
		int dataLen = (BYTE)m_strAniData[t + searchStr.GetLength()];
		m_strAniData.Delete( t, dataLen + searchStr.GetLength()+1);	// +1은 dataLen 
	}

	// 추가.
	m_strAniData += newData;



	
	// -------------------------------------------------
	// Save animation frame copy flag
	// -------------------------------------------------
	searchStr = GetSearchString( m_dwType, m_dwAction, m_dwPart);
	newData = searchStr;
	newData += (BYTE)m_myData.m_iAniLength;
	for( i = 0; i< m_myData.m_iAniLength; i++)
	{
		newData += (char)(m_myData.m_pCopyFlag[ i]+1);	// +1 -> string이므로.....
	}

	if( (t = m_strCopyFlagData.Find( searchStr, 0)) == -1)
	{
//		m_strCopyFlagData += newData;
	}
	else
	{
		// 찾은 경우 
		// 제거.
		int dataLen = (BYTE)m_strCopyFlagData[t + searchStr.GetLength()];
		m_strCopyFlagData.Delete( t, dataLen + searchStr.GetLength()+1);	// +1은 dataLen 
	}
	// 추가.
	m_strCopyFlagData += newData;



	
	// -------------------------------------------------
	// Save animation offset data
	// -------------------------------------------------
	searchStr = GetSearchString( m_dwType);
	newData = searchStr;
	char digitStr[256];
	itoa( m_aniDlg.m_iCreatureXOffset, digitStr, 10);
	newData +=  digitStr;
	newData += ' ';
	itoa( m_aniDlg.m_iCreatureYOffset, digitStr, 10);
	newData += digitStr;
	newData += '\n';
	if( (t = m_strOffsetData.Find( searchStr, 0)) == -1)	// 없는 경우
	{
	}
	else
	{
		// 찾은 경우 
		int end;
		// 제거.
		if( ( end = m_strOffsetData.Find("\n", t)) != -1)
		{
			m_strOffsetData.Delete( t, end - t + 1);
		}
		else
		{
			MessageBox( "Parsing Error : SaveCurrentSet");
			return;
		}
	}
	// 추가.
	m_strOffsetData += newData;


}

char g_szCurrentDirectoryForBrowseForFolder[MAX_PATH];
int CALLBACK BrowseCallbackProc(
    HWND hwnd, 
    UINT uMsg, 
    LPARAM lParam, 
    LPARAM lpData
    )
{
	switch( uMsg)
	{
		case BFFM_INITIALIZED :
			{
				SendMessage( hwnd, BFFM_SETSELECTION , TRUE, (long)g_szCurrentDirectoryForBrowseForFolder);
			}
			break;
	}
	return 0;
}



void CSpriteManagerDlg::OnButtonDirectory() 
{

	static BOOL bFirst = true;
	static ITEMIDLIST* pItemIDList = NULL;
	static BROWSEINFO bi;

	if( bFirst)
	{
		bFirst = false;

		LPMALLOC pMalloc = NULL;
		// Get's the Shell's default allocator
		if (NOERROR != ::SHGetMalloc(&pMalloc)) AfxThrowMemoryException();

		bi.hwndOwner = *this;
		bi.pidlRoot = pItemIDList;
		bi.pszDisplayName = NULL;
	//	bi.lpszTitle = "Select Directory";
		bi.lpszTitle = NULL;
		bi.ulFlags = 0;
		bi.lpfn = BrowseCallbackProc;
		bi.lParam = 0;
		bi.iImage = 0;

		// 현재 실행 패스를 저장.
		char szFileName[ MAX_PATH];
		GetModuleFileName( NULL, szFileName, MAX_PATH);
		memset( g_szCurrentDirectoryForBrowseForFolder, 0, sizeof( g_szCurrentDirectoryForBrowseForFolder));
		strncpy( g_szCurrentDirectoryForBrowseForFolder, szFileName, strrchr( szFileName, '\\') - szFileName);

	}

	if( (pItemIDList = SHBrowseForFolder( &bi)) != NULL)
	{
		SHGetPathFromIDList( pItemIDList, m_strDirectoryName);
		// 다음에 사용할 수 있게 저장.
		strcpy( g_szCurrentDirectoryForBrowseForFolder, m_strDirectoryName);


		FlushImageList();

		// find all available files..
		char fileName[256];
		sprintf( fileName, "%s\\%s", m_strDirectoryName, "*.bmp");


		CString bmpNames[200];	 // fix me 200개 넘으면?
		CFileFind ffBMP;
		BYTE btBMPCount = 0;
		CString fileNames;
		BOOL bFoundBMP = ffBMP.FindFile( fileName);

		while( bFoundBMP)
		{
			if( btBMPCount>= 200)
			{
				MessageBox( "Too Many Bmps");
				SendMessage( WM_CLOSE);
				return;
			}

			bFoundBMP = ffBMP.FindNextFile();

			if( ffBMP.GetFileName().Find( "I_") == -1 && ffBMP.GetFileName().Find( "i_") == -1 )
			{
				bmpNames[ btBMPCount++] = ffBMP.GetFilePath();
				continue;
			}

			char stemp[10], temp[10];

			for( int i=1; i<256; i++ )
			{
				wsprintf(stemp,"i%d_",i+1);
				wsprintf(temp,"I%d_",i+1);

				if( ffBMP.GetFileName().Find( stemp ) == -1 && ffBMP.GetFileName().Find( temp ) == -1 )
				{
					bmpNames[ btBMPCount++] = ffBMP.GetFilePath();
					break;
				}
			}			
		}

		// sorting bmp files
		for( int s1 = 0; s1< btBMPCount-1; s1++)
		{
			for( int s2 = 0; s2 < btBMPCount-1; s2++)
			{
				if( bmpNames[ s2] > bmpNames[ s2+1])	
				{
					// swap 
					CString temp = bmpNames[ s2];
					bmpNames[ s2] = bmpNames[ s2+1];
					bmpNames[ s2+1] = temp;
				}
			}
		}



		for( int i = 0; i< btBMPCount; i++)
		{
			CDib* pDib = new CDib;
			if( pDib->LoadBitmapFile( bmpNames[ i], FALSE) == FALSE)
			{
				char errorStr[256];
				sprintf( errorStr, "%s를 읽을 수 없습니다.", bmpNames[ i]);
				MessageBox( errorStr);
			}
			else
			{
				m_imageList.AddTail( pDib);
			}
		}


		// chek image slider 
//		m_imageSliders.SetRange( 0, m_imageList.GetCount()-1);	// 암것도 없으면?

		SetDlgItemText( IDC_BUTTON_DIRECTORY, m_strDirectoryName);

		Invalidate( FALSE);//false
	}

	// Release the shell's allocator 
//	if( pItemIDList)
//		pMalloc->Free(pItemIDList);

//	pMalloc->Release();
//	pMalloc = NULL;
	

}

BOOL CSpriteManagerDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if( m_bDragging)
	{
		if( GetAsyncKeyState( VK_CONTROL))
			SetCursor( m_hDragCursor);
		else
			SetCursor( m_hDragCursorAdd);
	}
	else
		SetCursor( LoadCursor( NULL, IDC_ARROW));

	return FALSE;
	
//	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CSpriteManagerDlg::OnButtonSave() 
{
	SaveCurrentSet();

	SaveDataFile();
 
}

void CSpriteManagerDlg::LoadCurrentSet()
{
	// list 초기화..
	FlushAniList();



	// 검색어 찾기.
	CString searchStr = GetSearchString( m_dwType, m_dwAction, m_dwPart, m_dwDirection);


	int t, i;
	if( (t = m_strData.Find( searchStr, 0)) == -1)
	{
		// data없다.
	}
	else
	{
//		LPCSTR lpStr = m_strData.GetBuffer(1000000);

		// 읽기.
		t+= searchStr.GetLength();
		CString fileName;
		m_myData.m_iBmpsCount = (int)m_strData[t++];
		for( i = 0; i< m_myData.m_iBmpsCount; i++)
		{
			t++;	// skip '<'
			int fileNameLen = m_strData.Find( '>', t) - t;
			fileName = m_strData.Mid( t, fileNameLen);
			// load ani list..
			CDib* pDib = new CDib;
			pDib->LoadBitmapFile( fileName, FALSE);

			// fix me
			m_myData.m_aniList.AddTail( pDib);
			
			t+= fileName.GetLength()+1;
		}

	}

	// read ani index data
	searchStr = GetSearchString( m_dwType, m_dwAction, m_dwPart);
	if( (t = m_strAniData.Find( searchStr, 0)) == -1)
	{
		
	}
	else
	{
//			int len = m_strAniData.GetLength();


		t+= searchStr.GetLength();
		CString fileName;
		BYTE count = (BYTE)m_strAniData[t++];
		int anis = m_myData.m_aniList.GetCount();
		for( i = 0; i< count; i++)
		{
			int aniID = (int)m_strAniData[t++] - 1;

			if( anis!= 0 && aniID >= anis)	// 적합성 판별..
			{
				MessageBox( "Animation Frame값 이상.");
				aniID = 0;
			}

			m_myData.m_iAniIndex[ i] = aniID;	// animation frame의 수를 증가시키면서..

			//LIST에 추가
			CDib* pDib = m_myData.m_aniList.GetAt2( aniID );
			if(pDib == NULL)
			{
				Invalidate();
				return;
			}

			char strID[256];
			const char* szPathName = pDib->GetFileName();
			const char* szFileName = strrchr( szPathName, '\\');
			if( szFileName == NULL)
				szFileName = szPathName;
			else
				szFileName++;
			
			sprintf( strID, "%d-%s", aniID, szFileName);
			m_AniList.AddString(strID);


		}
		m_myData.m_iAniLength = count;
	}


	
	// -------------------------------------------------
	// retrieve animation copy flag data
	// -------------------------------------------------
	searchStr = GetSearchString( m_dwType, m_dwAction, m_dwPart);
	if( (t = m_strCopyFlagData.Find( searchStr, 0)) == -1)
	{
		// 존재하지 않는 경우 reset.
		memset( m_myData.m_pCopyFlag, 0, sizeof( m_myData.m_pCopyFlag));

		// m_strCopyFlagData에 새로이 추가.
		CString newData = searchStr;
		newData += (BYTE)m_myData.m_iAniLength;
		for( i = 0; i< m_myData.m_iAniLength; i++)
		{
			newData += (char)(m_myData.m_pCopyFlag[ i]+1);	// +1 -> string이므로.....
		}
		m_strCopyFlagData += newData;
		
	}
	else
	{
//			int len = m_strAniData.GetLength();

		t+= searchStr.GetLength();
		CString fileName;
		BYTE count = m_strCopyFlagData[t++];
		int anis = m_myData.m_aniList.GetCount();
		for( i = 0; i< count; i++)
		{
			char cCopyFlag = (char)m_strCopyFlagData[t++] - 1;


			m_myData.m_pCopyFlag[ i] = cCopyFlag;
		}
	}




	// load offset data
	searchStr = GetSearchString( m_dwType);

	if( (t = m_strOffsetData.Find( searchStr, 0)) == -1)	// 없는 경우
	{
		// 냅둔다.
	}
	else
	{
		char* data = m_strOffsetData.GetBuffer( 0);
		data += t + searchStr.GetLength();
		int xOffset = DEFAULT_XOFFSET, yOffset = DEFAULT_YOFFSET;	// 안전을 위해 기본값으로..

		if( sscanf( data, "%d %d\n", &xOffset, &yOffset) == 2)
		{
			m_aniDlg.m_iCreatureXOffset = xOffset;
			m_aniDlg.m_iCreatureYOffset = yOffset;
		}
		
	}

//	memset( m_myData.m_pCopyFlag, 0, sizeof( m_myData.m_pCopyFlag));


	m_aniSlider.SetRange( 0, m_myData.m_iAniLength-1, TRUE);
	Invalidate(FALSE);

//	UpdateInfo();

}




//=======================================================================================
//		
//		ani list를 비우고, list내의 dib도 삭제.
//		
//=======================================================================================
void CSpriteManagerDlg::FlushAniList()
{



	m_myData.m_aniList.RemoveAll();

	m_myData.m_iBmpsCount = 0;
	m_myData.m_iAniLength = 0;

	m_AniList.ResetContent();

}

void CSpriteManagerDlg::FlushImageList()
{
	// release 
	m_imageList.RemoveAll();


}

void CSpriteManagerDlg::OnSelchangeComboAction() 
{
	SaveCurrentSet();

	m_dwAction = m_comboAction.GetCurSel();
	if(IsSlayerType(m_dwType))
		m_dwAction = slayerActionIndex[m_dwAction];
	else if(IsVampireType(m_dwType))
		m_dwAction = vampireActionIndex[m_dwAction];
	else if(IsOustersType(m_dwType))
		m_dwAction = oustersActionIndex[m_dwAction];
	else if(IsNPCType(m_dwType))
		m_dwAction = npcActionIndex[m_dwAction];
	else if(IsMidLevelMobType(m_dwType))
		m_dwAction = midLevelMobActionIndex[m_dwAction];
	else if(IsPromotionSlayer(m_dwType) )
		m_dwAction = g_PromotionSlayerActionIndex[ m_dwAction ];
	else if( IsPromotionOusters( m_dwType ) )
		m_dwAction = g_PromotionOustersActionIndex[ m_dwAction ];
	else if( IsPromotionVampire( m_dwType ) )
		m_dwAction = g_AdvancementClassVampireActionIndex[ m_dwAction ];
	
/*
	// action의 순서가 type에 따라 틀려질 수 있으므로...
	// 찾아야 한다.
	CString str;
	m_comboAction.GetLBText( m_comboAction.GetCurSel(), str);

	for( int i = 0; i< ACTION_MAX; i++)
	{
		if(  str == strAction[ i])
		{
			m_dwAction = i;
			break;
		}
	}
	if( i == ACTION_MAX)
	{
		MessageBox( "Error.");
	}
*/

	LoadCurrentSet();	

	m_aniDlg.Load( TRUE);
	
}

void CSpriteManagerDlg::OnSelchangeComboDirection() 
{
	SaveCurrentSet();
	m_dwDirection = m_comboDirection.GetCurSel();
	LoadCurrentSet();	

	m_aniDlg.Load( TRUE);
	
}

void CSpriteManagerDlg::OnSelchangeComboPart() 
{
	SaveCurrentSet();
	m_dwPart = m_comboPart.GetCurSel();
	LoadCurrentSet();	
	
}

void CSpriteManagerDlg::OnSelchangeComboType() 
{
	SaveCurrentSet();
	m_dwType = m_comboType.GetCurSel();

	ResetCombos();	// combo box재설정.

	LoadCurrentSet();	

	// animation window의 combo 재설정.
	m_aniDlg.ResetCombos();

	m_aniDlg.Load( TRUE);
	
}

#include "dlgsearchoption.h"

//=======================================================================================
//		
//		directory를 조사하여 data를 대충 생성한다.
//		
//=======================================================================================
void CSpriteManagerDlg::OnButtonSearch() 
{
	CDlgSearchOption dlg;

	dlg.m_bUpdateImage = TRUE;
	dlg.m_bUpdateAniData = TRUE;

	if( dlg.DoModal() == IDOK)
	{
		SetCursor( LoadCursor(NULL, IDC_WAIT)); 

		// search normal bmp
		SearchBMP( m_strData, dlg.m_bUpdateAniData, FALSE);

		// search shadow bmp
		SearchBMP( m_strShadowData, FALSE, TRUE);

		m_bDirty = TRUE;
		LoadCurrentSet();

		SetCursor( LoadCursor(NULL, IDC_ARROW)); 

		m_aniDlg.Load( TRUE);
	}
}


bool CSpriteManagerDlg::isBodyPart( int part)
{
	if( part == PART_HEAD1 || part == PART_HEAD2 || part == PART_HEAD3 ||
		part == PART_HELMET1 || part == PART_HELMET2 ||// part == PART_HELMET3 ||
		part == PART_UPPER_CLOTHES1 || part == PART_UPPER_CLOTHES2 || //part == PART_UPPER_CLOTHES3 || part == PART_UPPER_CLOTHES4 ||
		part == PART_LOWER_CLOTHES1 || part == PART_LOWER_CLOTHES2)// || part == PART_LOWER_CLOTHES3 || part == PART_LOWER_CLOTHES4)
	{
		return true;
	}
	return false;

}


////////////////////////////////////////////////////
// 일관성을 위해 편의상 만든 함수.
// action은 action enum...
////////////////////////////////////////////////////
CString CSpriteManagerDlg::GetSearchString(DWORD type, DWORD action, DWORD part, DWORD direction)
{
	CString searchStr;

	if( IsSlayerType( type))
	{
		if( action < ACTION_MAX)
		{
			searchStr.Format( "<%s %s %s %s>", strCreatureType[ type], strAction[ action]
											, strSlayerPart[ part], strDirection[ direction]);
		}
		else
		{
			MessageBox( "GetSearchString Error.");
		}
	}
	else if( IsVampireType( type))
	{
		if( action < ACTION_MAX)
		{
			searchStr.Format( "<%s %s %s %s>", strCreatureType[ type], strAction[ action]
											, strVampirePart[ part], strDirection[ direction]);
		}
		else
		{
			MessageBox( "GetSearchString Error.");
		}
	}
	else if (IsOustersType( type ) )
	{
		if( action < ACTION_MAX )
		{
			searchStr.Format( "<%s %s %s %s>", strCreatureType[type], strOustersAction[action],
				strOustersPart[part], strDirection[direction] );
		} 
		else
		{
			MessageBox(" GetSearchString Error. [Ousters]");
		}
	}
	else if ( IsPromotionSlayer( type ) )
	{
		if( action < ACTION_SLAYER_PROMOTION_MAX)
		{
			searchStr.Format( "<%s %s %s %s>", strCreatureType[ type ], strPromotionSlayerAction[ action ],
				strPromotionSlayerPart[ part ], strDirection[ direction ] );
		}
		else
		{
			MessageBox( "GetSearchString Error. [PromotionSlayer]");
		}
	}
	else if( IsPromotionOusters( type ) )
	{
		if( action < ACTION_OUSTERS_PROMOTION_MAX )
		{
			searchStr.Format( "<%s %s %s %s>", strCreatureType[ type ], strPromotionOustersAction[ action ],
				strPromotionOustersPart[ part ], strDirection[ direction ] );
		}
		else
		{
			MessageBox( "GetSearchString Error. [PromotionSlayer]");
		}
	}
	else if( IsPromotionVampire( type ) )
	{
		if( action < ACTION_ADVANCEMENT_MAX )
		{
			searchStr.Format( "<%s %s %s %s>", strCreatureType[ type ], strAdvancementClassVampireAction[ action ],
				strAdvancementClassVampirePart[ part ], strDirection[ direction ] );
		}
		else
		{
			MessageBox( "GetSearchString Error. [AdvancementVampire] " );
		}
	}
	else
	{
		if( action < ACTION_MAX)
		{
			searchStr.Format( "<%s %s %s>", strCreatureType[ type], strAction[ action],
											strDirection[ direction]);
		}
		else
		{
			MessageBox( "GetSearchString Error.");
		}
	}

	return searchStr;

}



////////////////////////////////////////////////////
// 일관성을 위해 편의상 만든 함수.
////////////////////////////////////////////////////
CString CSpriteManagerDlg::GetSearchString(DWORD type, DWORD action, DWORD part)
{
	CString searchStr;
	if( IsSlayerType( type) || IsVampireType(type))
	{
		if( action < ACTION_MAX)
		{
/*
			if( part == PART_BIKE)
				searchStr.Format( "<%s %s %s>", strCreatureType[ type], strAction[ action], strSlayerPart[ part]);
			else if( isBodyPart( part))
				searchStr.Format( "<%s %s>", strCreatureType[ type], strAction[ action]);
			else 
				searchStr.Format( "<%s %s %s>", strCreatureType[ type], strAction[ action], "기타");
				*/
			searchStr.Format( "<%s %s>", strCreatureType[ type], strAction[ action]);
		}
		else
		{
			MessageBox( "GetSearchString Error.");
		}
	}
	else if (IsOustersType( type ) )
	{
		if( action< OUSTERS_ACTION_MAX )
		{
			searchStr.Format( "<%s %s>", strCreatureType[type], strOustersAction[action]);
		}
		else
		{
			MessageBox( "GetSearchString Error. [Ousters ..]");
		}
	}
	else if( IsPromotionSlayer( type ) )
	{
		if( action < ACTION_SLAYER_PROMOTION_MAX )
		{
			searchStr.Format( "<%s %s>", strCreatureType[ type ], strPromotionSlayerAction[ action ] );
		}
		else
		{
			MessageBox( "GetSearchString ERror. [Promotion Slayer..]");
		}
	}
	else if( IsPromotionOusters( type ) )
	{
		if( action < ACTION_OUSTERS_PROMOTION_MAX )
		{
			searchStr.Format( "<%s %s>", strCreatureType[ type ], strPromotionOustersAction[ action ] );
		}
		else
		{
			MessageBox( "GetSearchString Error, [Promotion Ousters..]" );
		}
	}
	else if( IsPromotionVampire( type ) )
	{
		if( action < ACTION_ADVANCEMENT_MAX )
		{
			searchStr.Format( "<%s %s>", strCreatureType[ type ], strAdvancementClassVampireAction[ action ] );
		}
		else
		{
			MessageBox( "GetSearchString Error, [Advancement Vampire..]" );
		}
	}
	else
	{
		if( action < ACTION_MAX)
		{
			searchStr.Format( "<%s %s>", strCreatureType[ type], strAction[ action]);
		}
		else
		{
			MessageBox( "GetSearchString Error.");
		}
	}


	return searchStr;

}


CString CSpriteManagerDlg::GetSearchString(DWORD type)
{
	CString searchStr;

	searchStr.Format( "<%s>", strCreatureType[ type]);

	return searchStr;

}



void CSpriteManagerDlg::OnMenuSend() 
{
	POSITION pos = m_imageList.FindIndex( m_iSelectedImageID);
	if( pos)
	{
		m_undo.SaveState( m_myData);

		CDib* pSrcDib = m_imageList.GetAt( pos);

		AddToAni( pSrcDib->GetFileName(), -1);

		InvalidateAni();
	}
	
}

void CSpriteManagerDlg::OnMenuSendAll() 
{

	OnMenuDeleteAllAnis();

	m_undo.SaveState( m_myData);

	POSITION pos = m_imageList.GetHeadPosition();;

	while( pos)
	{
		CDib* pSrcDib = m_imageList.GetNext( pos);

		AddToAni( pSrcDib->GetFileName(), -1);
	}
	InvalidateAni();
	
	
}




//=======================================================================================
//		
//		bmp file을 ani list에 추가..
//		화일이 이미 존재하면 ani index list에만 추가..
//=======================================================================================
int CSpriteManagerDlg::AddToAni(LPCSTR filename, int index, BOOL bAfter)
{
	// add to animation.

	// m_myData.m_aniList에 있는 file인지 확인.
	POSITION tempPos = m_myData.m_aniList.GetHeadPosition();
	CDib* pDib;
	int aniID = 0;	// aniList의 index..
	while( tempPos)
	{
		pDib = m_myData.m_aniList.GetAt( tempPos);
		if( strcmp( pDib->GetFileName(), filename) == 0)
		{
			//찾았다...
			break;
		}
		aniID++;
		m_myData.m_aniList.GetNext( tempPos);	// get next tempPos
	}

	// ani list에 없으므로 추가..
	if( tempPos == NULL)
	{
		CDib* pNewDib = new CDib;
		if( pNewDib->LoadBitmapFile( filename, FALSE) == FALSE)
		{
			MessageBox( "BMP load error.");
		}
		m_myData.m_aniList.AddTail( pNewDib);
		m_myData.m_iBmpsCount++;

		//LIST에 추가
		CDib* pDib = m_myData.m_aniList.GetAt2( aniID );

		char strID[256];
		const char* szPathName = pDib->GetFileName();
		const char* szFileName = strrchr( szPathName, '\\');
		if( szFileName == NULL)
			szFileName = szPathName;
		else
			szFileName++;
			
		sprintf( strID, "%d-%s", aniID, szFileName);
		m_AniList.AddString(strID);

/*
		// update m_strData
		int t;
		if( (t = m_strData.Find( GetSearchString( m_dwType, m_dwAction, m_dwPart, m_dwDirection), 0)) != -1)
		{
			int i = m_strData.Find( "\n", t) - 1;
			CString strNewFileName;
			strNewFileName.Format( "<%s>", filename);

			m_strData.Insert( i, strNewFileName);
		}
		else
		{
			MessageBox( "Error.");
		}
		*/
	}

	int insertAt = 0;

	if( index != -1)
	{
		if( bAfter)	
		{
			insertAt = index + 1;
//						m_myData.m_aniList.InsertAfter( newPos, pNewDib);
		}
		else
		{
			insertAt = index;
//						m_myData.m_aniList.InsertBefore( newPos, pNewDib);
		}
		// 한 칸 밀어내기...
		memmove( m_myData.m_iAniIndex + insertAt + 1, m_myData.m_iAniIndex + insertAt, sizeof(int)*(m_myData.m_iAniLength - insertAt));
		memmove( m_myData.m_pCopyFlag + insertAt + 1, m_myData.m_pCopyFlag + insertAt, 
			sizeof(char)*(m_myData.m_iAniLength - insertAt));
	}
	else
		insertAt = m_myData.m_iAniLength;	// 마지막에 추가.

	// fix me... array폭파 조사...

	m_myData.m_iAniIndex[ insertAt] = aniID;

	//LIST에 추가
	pDib = m_myData.m_aniList.GetAt2( aniID );
	
	char strID[256];
	const char* szPathName = pDib->GetFileName();
	const char* szFileName = strrchr( szPathName, '\\');
	if( szFileName == NULL)
		szFileName = szPathName;
	else
		szFileName++;
	
	sprintf( strID, "%d-%s", aniID, szFileName);
	m_AniList.InsertString(insertAt, strID);

	m_myData.m_pCopyFlag[ insertAt] = TRUE;

	m_myData.m_iAniLength++;

	m_aniSlider.SetRange( 0, m_myData.m_iAniLength-1, TRUE);

	m_bDirty = TRUE;

//	InvalidateAni();

	return 1;
}

BOOL CSpriteManagerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
/*
	// ani scroll
	if( PtInRect( &m_aniRect, pt))
	{
		int pos = m_aniSlider.GetPos();
		if( zDelta>0)
			m_aniSlider.SetPos( pos+1);
		else
			m_aniSlider.SetPos( pos-1);
		InvalidateAni();

		return FALSE;
	}
	else if( PtInRect( &m_imageRect, pt))
	{
		return FALSE;
	}
	else
		return CDialog::OnMouseWheel(nFlags, zDelta, pt);
	*/

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CSpriteManagerDlg::SaveDataFile()
{


	// -------------------------------------------------
	// Set current directory for FileDialog
	// -------------------------------------------------
	char currentDirectory[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, currentDirectory);
	strcat( currentDirectory, "\\spr.dat");

	CFileDialog fileDlg( FALSE, "dat", currentDirectory);

	if( fileDlg.DoModal() == IDOK)
	{
		CFile dataFile;

		if( dataFile.Open( fileDlg.GetPathName(), CFile::modeWrite | CFile::modeCreate))
		{
			DWORD dwLength = 0;
			// -------------------------------------------------
			// Write animation bmp data
			// -------------------------------------------------
			dwLength = m_strData.GetLength();
			dataFile.Write( &dwLength, sizeof( DWORD));
			dataFile.Write( m_strData, dwLength);


			// -------------------------------------------------
			// Write animation shadow bmp data
			// -------------------------------------------------
			dwLength = m_strShadowData.GetLength();
			dataFile.Write( &dwLength, sizeof( DWORD));
			dataFile.Write( m_strShadowData, dwLength);


			// -------------------------------------------------
			// Write animation frame data
			// -------------------------------------------------
			dwLength = m_strAniData.GetLength();
			dataFile.Write( &dwLength, sizeof( DWORD));
			dataFile.Write( m_strAniData, dwLength);


			// -------------------------------------------------
			// Write animation offset point data
			// -------------------------------------------------
			dwLength = m_strOffsetData.GetLength();
			dataFile.Write( &dwLength, sizeof( DWORD));
			dataFile.Write( m_strOffsetData, dwLength);


			// -------------------------------------------------
			// Write animation frame copy flag data
			// -------------------------------------------------
			dwLength = m_strCopyFlagData.GetLength();
			dataFile.Write( &dwLength, sizeof( DWORD));
			dataFile.Write( m_strCopyFlagData, dwLength);

			dataFile.Close();
		}
		else
		{
			CString str;
			str.Format( "%s 화일 열기 실패.", DATAFILE_NAME);
			MessageBox( str);
			return;
		}





		m_bDirty = FALSE;
	}

}

///////////////////////////////////////////////////////////
// static info control에 정보 표시..
///////////////////////////////////////////////////////////
void CSpriteManagerDlg::UpdateInfo()
{
	char info[256];
	memset( info, 0, sizeof(info));
	sprintf( info, "프레임 %d, 이미지 %d", m_myData.m_iAniLength, m_myData.m_aniList.GetCount());
	SetDlgItemText( IDC_STATIC_INFO, info);
}



BOOL IsSlayerType( int type)
{
	if( type == TYPE_SLAYER_MAN || type == TYPE_SLAYER_WOMAN)
		return TRUE;
	return FALSE;
}

BOOL IsVampireType(int type)
{
	if(type == TYPE_VAMPIRE_PC_MAN || type == TYPE_VAMPIRE_PC_WOMAN)
		return TRUE;
	return FALSE;
}

BOOL IsOustersType(int type)
{
	if( type == TYPE_OUSTERS )
		return TRUE;

	return FALSE;
}

BOOL IsNPCType(int type)
{
	if(type == TYPE_NPC)
		return TRUE;
	return FALSE;
}

BOOL IsMidLevelMobType(int type)
{
	if(type == TYPE_MIDLEVEL_MOB)
		return TRUE;
	return FALSE;
}

BOOL IsPromotionSlayer( int type )
{
	if( type == TYPE_PROMOTION_SLAYER_MAN || type == TYPE_PROMOTION_SLAYER_WOMAN )
		return TRUE;

	return FALSE;
}

BOOL IsPromotionOusters( int type )
{
	if( type == TYPE_PROMOTION_OUSTERS )
		return TRUE;

	return FALSE;
}

BOOL IsPromotionVampire( int type )
{
	if( type == TYPE_PROMOTION_VAMPIRE_PC_MAN || type == TYPE_PROMOTION_VAMPIRE_PC_WOMAN )
		return TRUE;

	return FALSE;
}


#include "..\lib_ole\sp.h"
#include "sprite.h"
#include "MakeSprDlg.h"
#include "..\lib_ole\FR.h" 




void AddSprite( CString fileName, CSpritePackList555& spk)
{
}



#include "ProgressDlg.h"


void CSpriteManagerDlg::OnButtonMakeSprite() 
{
	SaveCurrentSet();

	CMakeSprDlg dlg;
	dlg.m_npcName = "NPC";
	dlg.m_midLevelMobName = "MOB";
	if( dlg.DoModal() == IDOK)
	{
		if( dlg.m_bSlayerMan)
		{
			// make spr
			MakeSprite( TYPE_SLAYER_MAN, m_strData, "AddonMale");
			// make shadow spr
			MakeShadowSprite( TYPE_SLAYER_MAN, m_strShadowData, "AddonMale-S");
		}
		if( dlg.m_bSlayerWoman)
		{
			MakeSprite( TYPE_SLAYER_WOMAN, m_strData, "AddonFemale");
			// Make shadow
			MakeShadowSprite( TYPE_SLAYER_WOMAN, m_strShadowData, "AddonFemale-S");
		}
		if( dlg.m_bVampireMan)
		{
			MakeSprite( TYPE_VAMPIRE_PC_MAN, m_strData, "PCVampMan");
			// Make shadow
			MakeShadowSprite( TYPE_VAMPIRE_PC_MAN, m_strShadowData, "PCVampMan-S");
		}
		if( dlg.m_bVampireWoman)
		{
			MakeSprite( TYPE_VAMPIRE_PC_WOMAN, m_strData, "PCVampWoman");
			// Make shadow
			MakeShadowSprite( TYPE_VAMPIRE_PC_WOMAN, m_strShadowData, "PCVampWoman-S");
		}

		if( dlg.m_bOusters )
		{
			MakeSprite( TYPE_OUSTERS, m_strData, "Ousters");
			MakeShadowSprite( TYPE_OUSTERS, m_strShadowData, "Ousters-S");
		}

		if( dlg.m_bPromotionOusters )
		{
			MakeSprite( TYPE_PROMOTION_OUSTERS, m_strData, "PromotionOusters" );
			MakeShadowSprite( TYPE_PROMOTION_OUSTERS, m_strShadowData, "PromotionOusters-S" );
		}

		if( dlg.m_bPromotionSlayerMan )
		{
			MakeSprite( TYPE_PROMOTION_SLAYER_MAN, m_strData, "PromotionSlayerMan" );
			MakeShadowSprite( TYPE_PROMOTION_SLAYER_MAN, m_strShadowData, "PromotionSlayerMan-S" );
		}

		if( dlg.m_bPromotionSlayerWoman )
		{
			MakeSprite( TYPE_PROMOTION_SLAYER_WOMAN, m_strData, "PromotionSlayerWoman" );
			MakeShadowSprite( TYPE_PROMOTION_SLAYER_WOMAN, m_strShadowData, "PromotionSlayerWoman-S" );
		}

		if( dlg.m_bPromotionVampireMan )
		{
			MakeSprite( TYPE_PROMOTION_VAMPIRE_PC_MAN, m_strData, "PromotionVampireMan" );
			MakeShadowSprite( TYPE_PROMOTION_VAMPIRE_PC_MAN, m_strShadowData, "PromotionVampireMan-S" );
		}

		if( dlg.m_bPromotionVampireWoman )
		{
			MakeSprite( TYPE_PROMOTION_VAMPIRE_PC_WOMAN, m_strData, "PromotionVampireWoman" );
			MakeShadowSprite( TYPE_PROMOTION_VAMPIRE_PC_WOMAN, m_strShadowData, "PromotionVampireWoman-S" );
		}

		if( dlg.m_bNPC)
		{
			if( dlg.m_npcName.GetLength() == 0)
				dlg.m_npcName = "NPC";

			// 확장자 제거.
			dlg.m_npcName = dlg.m_npcName.Left( dlg.m_npcName.GetLength() - dlg.m_npcName.ReverseFind('.') - 1);

			MakeSprite( TYPE_NPC, m_strData, dlg.m_npcName);
			// Make shadow
			MakeShadowSprite( TYPE_NPC, m_strShadowData, dlg.m_npcName+"-S");
		}

		if( dlg.m_bMidLevelMob)
		{
			if( dlg.m_midLevelMobName.GetLength() == 0)
				dlg.m_midLevelMobName = "MOB";

			dlg.m_midLevelMobName = dlg.m_midLevelMobName.Left( dlg.m_midLevelMobName.GetLength() - dlg.m_midLevelMobName.ReverseFind('.') - 1);

			MakeSprite( TYPE_MIDLEVEL_MOB, m_strData, dlg.m_midLevelMobName);
			// Make shadow
			MakeShadowSprite( TYPE_MIDLEVEL_MOB, m_strShadowData, dlg.m_midLevelMobName+"-S");
		}

	}



/*
	ifstream packFile("test.frr", ios::binary);
	CFrame frame;
	frame.LoadFromFile( 
	packFile.close();
*/

	
}
#define _MAKE_SPK

extern int MakeFrame( CSpritePackList555& spkList, const CDib& dib, S_RECT& rect);
extern BOOL MakeFrame( CIndexSprite& sprite, char * , char * , char * );

#define MAX_DIBS	60000

void CSpriteManagerDlg::MakeSprite(DWORD dwType, CString& strData, CString fileName)
{
	CString searchStr;

	// TODO: Add your control notification handler code here
	CIndexSpritePack spk;
#ifdef _MAKE_SPK
	CSpritePackList555 spkTest;
#endif

//	ifstream file( "Creature.ispk", ios::binary);
//	spk.LoadFromFile( file);


	CCreatureFramePack cfpk;


	CString dibNameArray[MAX_DIBS];	// spk의 반복 사용을 위해서..
	DWORD dibNameArrayCounter = 0;

	CProgressDlg progressDlg;

	progressDlg.Create( IDD_DIALOG_PROGRESS, this);
	progressDlg.ShowWindow( SW_SHOW);


/*
	FIX ME m_myData.m_iAniIndex를 조사하여, indexing의 결함을 조사한 후, 저장하는 것이 좋을 것 같은데.. 귀찮으니까..

	BOOL* bUsed = NULL;
*/
	int t, i;
	int type = dwType, part, action, direction;


	int aniData[MAX_ANIS_FRAME];
//	int frameIndex[ MAX_ANIS_FRAME];	// spk id와 frame 을 연결해주기 위한 임시..
	int aniLength = 0;



	///////////////////////////////////////////////////////
	//	load offset data
	///////////////////////////////////////////////////////
	int xOffset = DEFAULT_XOFFSET, yOffset = DEFAULT_YOFFSET;
/*
	// load offset data
	searchStr = GetSearchString( type);

	if( (t = m_strOffsetData.Find( searchStr, 0)) == -1)	// 없는 경우
	{
		// 냅둔다.
	}
	else
	{
		char* data = m_strOffsetData.GetBuffer( 0);
		data += t + searchStr.GetLength();

		if( sscanf( data, "%d %d", &xOffset, &yOffset) == 2)
		{
		}
	}
*/
	int partMax = 0;	// slayer인가 vampire인가에 따라 트리의 구조가 바뀌므로..
						// slyaer인 경우는 part를 의미, 아닌 경우는 object의 종류가 되겠다..
	int actionMax = 0;

	const DWORD* pActionIndex = NULL;

	if( IsSlayerType( type))
	{
		partMax = SLAYER_PART_MAX;	// slayer인 경우 part의 수만큼..
		actionMax = SLAYER_ACTION_MAX;
		pActionIndex = slayerActionIndex;
	}
	else if( IsVampireType(type))
	{
		partMax = VAMPIRE_PART_MAX;	// 
		actionMax = VAMPIRE_ACTION_MAX;
		pActionIndex = vampireActionIndex;
	}
	else if( type == TYPE_NPC)
	{
		partMax = 1;
		actionMax = NPC_ACTION_MAX;
		pActionIndex = npcActionIndex;
	}
	else if( type == TYPE_MIDLEVEL_MOB)
	{
		partMax = 1;
		actionMax = MIDLEVEL_MOB_ACTION_MAX;
		pActionIndex = midLevelMobActionIndex;
	}
	else if (IsOustersType(type))
	{
		partMax = OUSTERS_PART_MAX;
		actionMax = OUSTERS_ACTION_MAX;
		pActionIndex = oustersActionIndex;
	} 
	else if ( IsPromotionSlayer( type ) )
	{
		partMax = PS_PART_MAX;		
		actionMax = ACTION_SLAYER_PROMOTION_MAX;
		pActionIndex = g_PromotionSlayerActionIndex;
	}
	else if ( IsPromotionOusters( type ) )
	{
		partMax = PO_PART_MAX;
		actionMax = ACTION_OUSTERS_PROMOTION_MAX;
		pActionIndex = g_PromotionOustersActionIndex;
	}
	else if( IsPromotionVampire( type ) )
	{
		partMax = AC_PART_MAX;
		actionMax = ACTION_ADVANCEMENT_MAX;
		pActionIndex = g_AdvancementClassVampireActionIndex;
	}
	else
	{
		//?
	}

	DWORD dwImages = 0;

	DWORD currentI = 0;
	DWORD maxI = partMax*actionMax*DIR_MAX;

	cfpk.Init( partMax); // init cfpk name level

	for( part = 0; part < partMax; part++)
	{
		if(IsSlayerType(type) || IsVampireType(type) || IsOustersType(type) || IsPromotionSlayer( type )
			|| IsPromotionOusters( type ) || IsPromotionVampire( type ))
		{
			searchStr = GetSearchString( type);

			if( (t = m_strOffsetData.Find( searchStr, 0)) == -1)	// 없는 경우
			{
				// 냅둔다.
			}
			else
			{
				char* data = m_strOffsetData.GetBuffer( 0);
				data += t + searchStr.GetLength();
				if( sscanf( data, "%d %d", &xOffset, &yOffset) == 2)
				{
				}
			}		
		}
		else
		{
			searchStr = GetSearchString( type+part);

			if( (t = m_strOffsetData.Find( searchStr, 0)) == -1)	// 없는 경우
			{
				// 냅둔다.
			}
			else
			{
				char* data = m_strOffsetData.GetBuffer( 0);
				data += t + searchStr.GetLength();
				if( sscanf( data, "%d %d", &xOffset, &yOffset) == 2)
				{
				}
			}
		}
		cfpk[part].Init( actionMax); // init cfpk name level
		for( action = 0; action < actionMax; action++)
		{
			/////////////////////////////////////////////////////////////////
			//
			//	animation 반복 data를 얻는다.
			//
			/////////////////////////////////////////////////////////////////
			// read ani index data
			aniLength = 0;
			memset( aniData, 0, MAX_ANIS_FRAME*sizeof(int));

			if( IsSlayerType( type) || IsVampireType(type) || IsOustersType(type) || IsPromotionSlayer( type )
				|| IsPromotionOusters( type ) || IsPromotionVampire( type ) )
				searchStr = GetSearchString( type, pActionIndex[ action], part);
			else
				searchStr = GetSearchString( type+part, pActionIndex[action], part);

			if( (t = m_strAniData.Find( searchStr, 0)) == -1)
			{
//				if( IsSlayerType( type) || IsVampireType(type))
//					goto actionOut;
				cfpk[part][action].Init( DIR_MAX); // init cfpk name level
				continue;
				
			}
			else
			{
				t+= searchStr.GetLength();
				CString fileName;
				aniLength = (BYTE)m_strAniData[t++];

				if( aniLength > MAX_ANIS_FRAME)
				{
					MessageBox( "Error. Plz Stop!");
				}

				for( i = 0; i< aniLength; i++)
				{
					int aniID = (int)m_strAniData[t++] - 1;
					aniData[ i] = aniID;
				}
			}

			cfpk[part][action].Init( DIR_MAX); // init cfpk name level
			for( direction = 0; direction < DIR_MAX; direction++)
			{
				CString tempStr;

				currentI = part*actionMax*DIR_MAX + action*DIR_MAX + direction;

				tempStr.Format( "%d/%d", currentI, maxI);
				progressDlg.SetText( tempStr);
				progressDlg.SetProgress( currentI, maxI);


				FRAME frameArray[200];	// 최고 200개의 그림이 있을수 있다.
				DWORD frameArrayCounter = 0;

				// 검색어 찾기.
				if( IsSlayerType( type) || IsVampireType(type) || IsOustersType(type) || IsPromotionSlayer( type )
					|| IsPromotionOusters( type ) || IsPromotionVampire( type ) )
					searchStr = GetSearchString( type, pActionIndex[action], part, direction);
				else
					searchStr = GetSearchString( type + part, pActionIndex[action], 0, direction);

				if( (t = strData.Find( searchStr, 0)) == -1)
				{
//					goto directionOut;
					continue;
				}
				else
				{


					t+= searchStr.GetLength();
					CString fileName;
					int iBmpsCount = (int)strData[t++];
					for( i = 0; i< iBmpsCount; i++)
					{
						t++;	// skip '<'
						int fileNameLen = strData.Find( '>', t) - t;
						fileName = strData.Mid( t, fileNameLen);


						//////////////////////////////////////////////////
						// 이미 sprite로 만들어진 file인지 확인..
						//////////////////////////////////////////////////
						FRAME frame;
						frame.sprite_id = -1;
						// 찾기
						for( DWORD tt = 0; tt < dibNameArrayCounter; tt++)
						{
							if( dibNameArray[ tt] == fileName)
							{
								frame.sprite_id = tt;
								break;
							}
						}
						// 못찾았으면.
						if( tt == dibNameArrayCounter)
						{
							frame.sprite_id = dwImages;
							dwImages++;
							if( dibNameArrayCounter >= MAX_DIBS)
							{
								MessageBox( "스프라이트를 만들 수 있는 BMP수를 초과.");
							}
							else
								dibNameArray[ dibNameArrayCounter++] = fileName;
						}

						
						S_RECT srect =GetFrame( frame, fileName.GetBuffer(0), xOffset, yOffset);

/*
					
						CSpriteSurface imageSurface;
						CSpriteSurface indexSurface1;
						
						WORD *lpImage, imagePitch;
						WORD *lpIndex1, indexPitch1;

						imageSurface.InitFromBMP( fileName, DDSCAPS_SYSTEMMEMORY);	
						CString indexFilename = "i_" + fileName;
						indexSurface1.InitFromBMP( indexFilename, DDSCAPS_SYSTEMMEMORY);	

						imageSurface.LockW(lpImage, imagePitch);
						indexSurface1.LockW(lpIndex1, indexPitch1);

						RECT rect;

						CSprite::GetTightColorRect(lpImage, imagePitch,
													imageSurface.GetWidth(), imageSurface.GetHeight(),
													0x001F,
													rect);


						lpImage = (WORD*)((BYTE*)lpImage + imagePitch*rect.top + (rect.left << 1));
						lpIndex1 = (WORD*)((BYTE*)lpIndex1 + indexPitch1*rect.top + (rect.left << 1));

						CIndexSprite::SetColorkey( 0x001F );

						spk[n++].SetPixel(lpImage, imagePitch,
									lpIndex1, indexPitch1,
									NULL, 0, //lpIndex2, indexPitch2,
									rect.right-rect.left, rect.bottom-rect.top);
						
						imageSurface.Unlock();
						indexSurface1.Unlock();
*/











						if(  srect.x == -1)
						{
							CString str = fileName;
							str += " 변환 오류";
							MessageBox( str);
//									return;
						}


						if( frameArrayCounter >= 200)
						{
							char sz_temp [200];
							strcpy(sz_temp, searchStr);
							strcat(sz_temp, "너무 많은 bmp가 사용되었습니다.");
							MessageBox( sz_temp);
						}
						else
							frameArray[ frameArrayCounter++] = frame;

						t+= fileName.GetLength()+1;
					}

					// set frame info
					if( aniLength != 0)
					{
						cfpk[part][action][direction].Init( aniLength);

						for( int t = 0; t< aniLength; t++)
						{
							if( aniData[t] >= iBmpsCount)
							{
								char sz_temp [200];
								strcpy(sz_temp, searchStr);
								strcat(sz_temp, "같은 동작이지만 BMP화일수가 틀립니다.");
								MessageBox( sz_temp);
							}
							else
							{
								FRAME& frame = frameArray[ aniData[t]];
								cfpk[part][action][direction][t].Set( frame.sprite_id, frame.rx, frame.ry);
							}
						}

					}
					else
					{
						MessageBox( "Error:No ani data"); // 그림은 있는데.. ani data는 없다.
					}

//					curJob.Format( "%s-%s-%s-%s 변환중", strCreatureType[ type], strSlayerPart[ part],
//						strAction[ action], strDirection[ direction]);

				}
			}
//directionOut:;
		}

	}


	////////////////////////////
	//	Sprite를 만든다.
	//
//	CIndexSprite::SetColorkey( 0x001F );

	spk.Init( dwImages, FALSE);

	for( DWORD dwCurImage = 0; dwCurImage < dwImages; dwCurImage++)
	{
		CString& dibName = dibNameArray[ dwCurImage];

		progressDlg.SetText( dibName);
		progressDlg.SetProgress( dwCurImage, dwImages);

		int nRS = dibName.ReverseFind( '\\');
		
		CString indexNameArray[256];
		CString temp;

		indexNameArray[0] = dibName.Left( nRS) + "\\I_" + dibName.Right( dibName.GetLength() - nRS - 1);

		for( i = 1; i < 256; i++ )
		{
			wsprintf(temp.GetBuffer(0), "\\I%d_", i+1);

			indexNameArray[i] = dibName.Left( nRS) + temp.GetBuffer(0) + dibName.Right( dibName.GetLength() - nRS - 1);
		}

		///////////////////////////////////////////////////
		//	머리카락 부분은 seletion이 원 화일과 완전
		//	동일 하므로 이를 처리한다.
		//	"머리"라는 키워드로..
		if( dibName.Find( "머리") != -1)
		{
			indexNameArray[0] = dibName; 
		}


		int bl_dither = 0;
		if(IsSlayerType(type))
			bl_dither = 1;
		else
			bl_dither = 2;

		if(dibName.Find("옷", 0) != -1)
		{
			int a = 0;
		}
		if( MakeFrame( spk[ dwCurImage], dibName.GetBuffer(0), indexNameArray, bl_dither) == FALSE)
		{
			MessageBox( "Make Frame Error!");
			break;
		}
#ifdef _MAKE_SPK
		S_RECT rect;
		CDib dib;
		dib.LoadBitmapFile( dibName, TRUE);
		MakeFrame( spkTest, dib, rect, true );
#endif
	}
#ifdef _MAKE_SPK
	{
		ofstream spk_file( fileName + ".spk", ios::binary);
		ofstream spki_file( fileName + ".spki", ios::binary);
		spkTest.SaveToFile(spk_file, spki_file);
		spk_file.close();
		spki_file.close();
		spkTest.Release();
	}
#endif
	progressDlg.DestroyWindow();


/*
	CString spkFileName;
	CString cfpkFileName;
	CString spkiFileName;
	CString cfpkiFileName;

	switch( type)
	{
	case TYPE_SLAYER_MAN:
		spkFileName = "sl-man.spk";
		cfpkFileName = "sl-man.cfpk";
		spkiFileName = "sl-man.spki";
		cfpkiFileName = "sl-man.cfpki";
		break;
	case TYPE_SLAYER_WOMAN:
		spkFileName = "sl-woman.spk";
		cfpkFileName = "sl-woman.cfpk";
		spkiFileName = "sl-woman.spki";
		cfpkiFileName = "sl-woman.cfpki";
		break;
	default:	// vampire..
		spkFileName = "vamps.spk";
		cfpkFileName = "vamps.cfpk";
		spkiFileName = "vamps.spki";
		cfpkiFileName = "vamps.cfpki";
		break;
	}
*/


	{
		// Save SPK
		ofstream spk_file( fileName + ".ispk", ios::binary);
		ofstream spki_file( fileName + ".ispki", ios::binary);
		spk.SaveToFile(spk_file, spki_file);
		spk_file.close();
		spki_file.close();
		


		/////////////////////////////////////////
		// Make Frame Data
		/////////////////////////////////////////

	   // Save CFPK
		
		ofstream packFile( fileName + ".cfpk", ios::binary);
		ofstream indexFile( fileName + ".cfpki", ios::binary);

		cfpk.SaveToFile(packFile);
		packFile.close();
		indexFile.close();
	}

	spk.Release();
	cfpk.Release();

}



void CSpriteManagerDlg::MakeShadowSprite(DWORD dwType, CString& strData, CString fileName)
{
	CString searchStr;

	// TODO: Add your control notification handler code here
	CShadowSpritePack spk;


	CCreatureFramePack cfpk;


	CString dibNameArray[MAX_DIBS];	// spk의 반복 사용을 위해서..
	DWORD dibNameArrayCounter = 0;


	CProgressDlg progressDlg;


	progressDlg.Create( IDD_DIALOG_PROGRESS, this);
	progressDlg.ShowWindow( SW_SHOW);


/*
	FIX ME m_myData.m_iAniIndex를 조사하여, indexing의 결함을 조사한 후, 저장하는 것이 좋을 것 같은데.. 귀찮으니까..

	BOOL* bUsed = NULL;
*/
	int t, i;
	int type = dwType, part, action, direction;


	int aniData[MAX_ANIS_FRAME];
//	int frameIndex[ MAX_ANIS_FRAME];	// spk id와 frame 을 연결해주기 위한 임시..
	int aniLength = 0;



	///////////////////////////////////////////////////////
	//	load offset data
	///////////////////////////////////////////////////////
	int xOffset = DEFAULT_XOFFSET, yOffset = DEFAULT_YOFFSET;
/*
	// load offset data
	searchStr = GetSearchString( type);

	if( (t = m_strOffsetData.Find( searchStr, 0)) == -1)	// 없는 경우
	{
		// 냅둔다.
	}
	else
	{
		char* data = m_strOffsetData.GetBuffer( 0);
		data += t + searchStr.GetLength();

		if( sscanf( data, "%d %d", &xOffset, &yOffset) == 2)
		{
		}
		
	}
*/
	int partMax = 0;	// slayer인가 vampire인가에 따라 트리의 구조가 바뀌므로..
						// slyaer인 경우는 part를 의미, 아닌 경우는 object의 종류가 되겠다..
	int actionMax = 0;

	const DWORD* pActionIndex = NULL;
	if( IsSlayerType( type))
	{
		partMax = SLAYER_PART_MAX;	// slayer인 경우 part의 수만큼..
		actionMax = SLAYER_ACTION_MAX;
		pActionIndex = slayerActionIndex;
	}
	else if( IsVampireType(type))
	{
								// vampire인 경우 vampire의 종류만큼..
		partMax = VAMPIRE_PART_MAX;	// 전체 종류중에.. slayer 2개를 제외한 만큼...
		actionMax = VAMPIRE_ACTION_MAX;
		pActionIndex = vampireActionIndex;

	}
	else if( type == TYPE_NPC)
	{
		partMax = 1;
		actionMax = NPC_ACTION_MAX;
		pActionIndex = npcActionIndex;
	}
	else if( type == TYPE_MIDLEVEL_MOB)
	{
		partMax = 1;
		actionMax = MIDLEVEL_MOB_ACTION_MAX;
		pActionIndex = midLevelMobActionIndex;
	} else if( IsOustersType(type) )
	{
		partMax = OUSTERS_PART_MAX;
		actionMax = OUSTERS_ACTION_MAX;
		pActionIndex = oustersActionIndex;
	}
	else if( IsPromotionSlayer( type ) )
	{
		partMax = PS_PART_MAX;
		actionMax = ACTION_SLAYER_PROMOTION_MAX;
		pActionIndex = g_PromotionSlayerActionIndex;
	}
	else if( IsPromotionOusters( type ) )
	{
		partMax = PO_PART_MAX;
		actionMax = ACTION_OUSTERS_PROMOTION_MAX;
		pActionIndex = g_PromotionOustersActionIndex;
	}
	else if( IsPromotionVampire( type ) )
	{
		partMax = AC_PART_MAX;
		actionMax = ACTION_ADVANCEMENT_MAX;
		pActionIndex = g_AdvancementClassVampireActionIndex;
	}


	DWORD dwImages = 0;

	DWORD currentI = 0;
	DWORD maxI = partMax*actionMax*DIR_MAX;

	cfpk.Init( partMax); // init cfpk name level

	for( part = 0; part < partMax; part++)
	{
		if( !IsSlayerType( type) && !IsVampireType(type) && !IsOustersType(type) && !IsPromotionSlayer( type )
			&& !IsPromotionOusters( type ) && !IsPromotionVampire( type) )
		{
			searchStr = GetSearchString( type+part);

			if( (t = m_strOffsetData.Find( searchStr, 0)) == -1)	// 없는 경우
			{
				// 냅둔다.
			}
			else
			{
				char* data = m_strOffsetData.GetBuffer( 0);
				data += t + searchStr.GetLength();
				if( sscanf( data, "%d %d", &xOffset, &yOffset) == 2)
				{
				}
			}
		}
		else
		{
			searchStr = GetSearchString( type);

			if( (t = m_strOffsetData.Find( searchStr, 0)) == -1)	// 없는 경우
			{
				// 냅둔다.
			}
			else
			{
				char* data = m_strOffsetData.GetBuffer( 0);
				data += t + searchStr.GetLength();
				if( sscanf( data, "%d %d", &xOffset, &yOffset) == 2)
				{
				}
			}
		}

		cfpk[part].Init( actionMax); // init cfpk name level
		for( action = 0; action < actionMax; action++)
		{
			/////////////////////////////////////////////////////////////////
			//
			//	animation 반복 data를 얻는다.
			//
			/////////////////////////////////////////////////////////////////
			// read ani index data
			aniLength = 0;
			memset( aniData, 0, MAX_ANIS_FRAME*sizeof(int));

			if( IsSlayerType( type) || IsVampireType(type) || IsOustersType(type) || IsPromotionSlayer( type )
				|| IsPromotionOusters( type ) || IsPromotionVampire( type ))
				searchStr = GetSearchString( type, pActionIndex[ action], part);
			else
				searchStr = GetSearchString( type+part, pActionIndex[action], part);

			if( (t = m_strAniData.Find( searchStr, 0)) == -1)
			{
				if( IsSlayerType( type) || IsVampireType(type) || IsOustersType(type) || IsPromotionSlayer( type )
					|| IsPromotionOusters( type ) || IsPromotionVampire( type ))
				{
					cfpk[part][action].Init( DIR_MAX); // init cfpk name level
					continue;
				}
//					goto actionOut2;
				
			}
			else
			{
				t+= searchStr.GetLength();
				CString fileName;
				aniLength = (BYTE)m_strAniData[t++];

				if( aniLength > MAX_ANIS_FRAME)
				{
					MessageBox( "Error. Plz Stop!");
				}

				for( i = 0; i< aniLength; i++)
				{
					int aniID = (int)m_strAniData[t++] - 1;
					aniData[ i] = aniID;
				}
			}

			cfpk[part][action].Init( DIR_MAX); // init cfpk name level
			for( direction = 0; direction < DIR_MAX; direction++)
			{
				CString tempStr;

				currentI = part*actionMax*DIR_MAX + action*DIR_MAX + direction;

				tempStr.Format( "%d/%d", currentI, maxI);
				progressDlg.SetText( tempStr);
				progressDlg.SetProgress( currentI, maxI);

				FRAME frameArray[200];	// 최고 200개의 그림이 있을수 있다.
				DWORD frameArrayCounter = 0;

				// 검색어 찾기.
				if( IsSlayerType( type) || IsVampireType(type) || IsOustersType(type) || IsPromotionSlayer( type )
					|| IsPromotionOusters( type ) || IsPromotionVampire( type) )
					searchStr = GetSearchString( type, pActionIndex[action], part, direction);
				else
					searchStr = GetSearchString( type + part, pActionIndex[action], 0, direction);

				if( (t = strData.Find( searchStr, 0)) == -1)
				{
					continue;
//					goto directionOut2;
				}
				else
				{


					t+= searchStr.GetLength();
					CString fileName;
					int iBmpsCount = (int)strData[t++];
					for( i = 0; i< iBmpsCount; i++)
					{
						t++;	// skip '<'
						int fileNameLen = strData.Find( '>', t) - t;
						fileName = strData.Mid( t, fileNameLen);


						//////////////////////////////////////////////////
						// 이미 sprite로 만들어진 file인지 확인..
						//////////////////////////////////////////////////
						FRAME frame;
						frame.sprite_id = -1;
						// 찾기
						for( DWORD tt = 0; tt < dibNameArrayCounter; tt++)
						{
							if( dibNameArray[ tt] == fileName)
							{
								frame.sprite_id = tt;
								break;
							}
						}
						// 못찾았으면.
						if( tt == dibNameArrayCounter)
						{
							frame.sprite_id = dwImages;
							dwImages++;
							if( dibNameArrayCounter >= MAX_DIBS)
							{
								MessageBox( "스프라이트를 만들 수 있는 BMP수를 초과.");
							}
							else
								dibNameArray[ dibNameArrayCounter++] = fileName;
						}
						
						S_RECT srect =GetFrame( frame, fileName.GetBuffer(0), xOffset, yOffset);

/*
					
						CSpriteSurface imageSurface;
						CSpriteSurface indexSurface1;
						
						WORD *lpImage, imagePitch;
						WORD *lpIndex1, indexPitch1;

						imageSurface.InitFromBMP( fileName, DDSCAPS_SYSTEMMEMORY);	
						CString indexFilename = "i_" + fileName;
						indexSurface1.InitFromBMP( indexFilename, DDSCAPS_SYSTEMMEMORY);	

						imageSurface.LockW(lpImage, imagePitch);
						indexSurface1.LockW(lpIndex1, indexPitch1);

						RECT rect;

						CSprite::GetTightColorRect(lpImage, imagePitch,
													imageSurface.GetWidth(), imageSurface.GetHeight(),
													0x001F,
													rect);


						lpImage = (WORD*)((BYTE*)lpImage + imagePitch*rect.top + (rect.left << 1));
						lpIndex1 = (WORD*)((BYTE*)lpIndex1 + indexPitch1*rect.top + (rect.left << 1));

						CIndexSprite::SetColorkey( 0x001F );

						spk[n++].SetPixel(lpImage, imagePitch,
									lpIndex1, indexPitch1,
									NULL, 0, //lpIndex2, indexPitch2,
									rect.right-rect.left, rect.bottom-rect.top);
						
						imageSurface.Unlock();
						indexSurface1.Unlock();
*/











						if(  srect.x == -1)
						{
							CString str = fileName;
							str += " 변환 오류";
							MessageBox( str);
//									return;
						}

						if( frameArrayCounter >= 200)
						{
								char sz_temp [200];
								strcpy(sz_temp, searchStr);
								strcat(sz_temp, "너무 많은 BMP가 사용되었습니다.");
								MessageBox( sz_temp);
						}
						else
							frameArray[ frameArrayCounter++] = frame;

						t+= fileName.GetLength()+1;
					}

					// set frame info
					if( aniLength != 0)
					{
						cfpk[part][action][direction].Init( aniLength);

						for( int t = 0; t< aniLength; t++)
						{
							if( aniData[t] >= iBmpsCount)
							{
								char sz_temp [200];
								strcpy(sz_temp, searchStr);
								strcat(sz_temp, "같은 동작이지만 BMP화일수가 틀립니다.");
								MessageBox( sz_temp);
							}
							else
							{
								FRAME& frame = frameArray[ aniData[t]];
								cfpk[part][action][direction][t].Set( frame.sprite_id, frame.rx, frame.ry);
							}
						}

					}
					else
					{
						MessageBox( "Error:No ani data"); // 그림은 있는데.. ani data는 없다.
					}

//					curJob.Format( "%s-%s-%s-%s 변환중", strCreatureType[ type], strSlayerPart[ part],
//						strAction[ action], strDirection[ direction]);

				}
			}
//directionOut2:;
		}
	}


	////////////////////////////
	//	Sprite를 만든다.
	//
	spk.Init( dwImages);
	for( DWORD dwCurImage = 0; dwCurImage < dwImages; dwCurImage++)
	{
		CString& dibName = dibNameArray[ dwCurImage];

		progressDlg.SetText( dibName);
		progressDlg.SetProgress( dwCurImage, dwImages);

extern BOOL MakeFrame( CShadowSprite& sprite, char * filename);
		if( MakeFrame( spk[ dwCurImage], dibName.GetBuffer(0)) == FALSE)
		{
			MessageBox( "Make Frame Error!");
			break;
		}
	}



	progressDlg.DestroyWindow();


	{
		// Save SPK
		ofstream spk_file( fileName + ".sspk", ios::binary);
		ofstream spki_file( fileName + ".sspki", ios::binary);
		spk.SaveToFile(spk_file, spki_file);
		spk_file.close();
		spki_file.close();
		


		/////////////////////////////////////////
		// Make Frame Data
		/////////////////////////////////////////

	   // Save CFPK
		
		ofstream packFile( fileName + ".cfpk", ios::binary);
		ofstream indexFile( fileName + ".cfpki", ios::binary);

		cfpk.SaveToFile(packFile);
		packFile.close();
		indexFile.close();
	}

	spk.Release();
	cfpk.Release();

}





















void CSpriteManagerDlg::SearchBMP( CString& strData, BOOL bUpdateAniData, BOOL bUpdateShadow)
{
	int iType, iAction, iPart, iDirection;
	CFileFind ffType;
//	searchingDir += "\\*.*";
	CString searchingDir;
	CFileFind testFF;

	// find creature directory
	for( iType = 0; iType< TYPE_MAX; iType++)
	{

		// directory가 없으면 skip.
		searchingDir.Format( "%s\\%s", 
			m_strDirectoryName, strCreatureType[iType]);

		if( testFF.FindFile( searchingDir) == FALSE) continue;
		else testFF.Close();


		if( IsSlayerType( iType))
		{
			for( iPart = 0; iPart< SLAYER_PART_MAX; iPart++)
			for( iAction = 0; iAction< ACTION_MAX; iAction++)
			{
				// -------------------------------------------------
				// 샷건action과 자동소총action, 소총action과 저격용총action은 
				// 총자체를 제외하고
				// 나머지 이미지는 완전 동일하므로.. 
				// -------------------------------------------------
				CString actionDirec;
/*				if( ( iAction == ACTION_GUN1 || iAction == ACTION_GUN2 
						|| iAction == ACTION_GUN3 || iAction == ACTION_GUN4) &&
					!(iPart == PART_MACHINEGUN || iPart == PART_SHOTGUN ||
						iPart == PART_RIFLE || iPart == PART_SNIPEGUN) )
				{
					actionDirec = strActionForSearch[ iAction];
				}
				else
				{
					actionDirec = strAction[ iAction];
				}

				// 검2&도2 는 검1,도1에서 복사..
				if( iAction == ACTION_SWORD1 || iAction == ACTION_SWORD2 
					|| iAction == ACTION_SWORD1A || iAction == ACTION_SWORD2A)
					actionDirec = strActionForSearch[ iAction];

				// 오토바이 이동은 오토바이 정지에서..
				if( iAction == ACTION_BIKE_MOVE)
					actionDirec = strActionForSearch[ iAction];

				// 던지기2는 던지기1에서..
				if( iAction == ACTION_THROW2)
*/					actionDirec = strActionForSearch[ iAction];
/*
				if( (iAction == ACTION_AR || iAction == ACTION_AR_SLOW || iAction == ACTION_AR_FAST)
				&& (iPart == PART_SR || iPart == PART_AR || iPart == PART_SG || iPart == PART_SMG) )
					actionDirec = "TR";

				if( (iAction == ACTION_SMG || iAction == ACTION_SMG_SLOW || iAction == ACTION_SMG_FAST)
				&& (iPart == PART_SR || iPart == PART_AR || iPart == PART_SG || iPart == PART_SMG) )
					actionDirec = "SG";
					*/

				if( (iAction == ACTION_AR || iAction == ACTION_AR_SLOW || iAction == ACTION_AR_FAST)
				&& !(iPart == PART_SR || iPart == PART_AR || iPart == PART_SG || iPart == PART_SMG) )
					actionDirec = "TR";

				if( (iAction == ACTION_SMG || iAction == ACTION_SMG_SLOW || iAction == ACTION_SMG_FAST)
				&& !(iPart == PART_SR || iPart == PART_AR || iPart == PART_SG || iPart == PART_SMG) )
					actionDirec = "SG";

				for( iDirection = 0; iDirection< DIR_MAX; iDirection++)
				{
						// 없으면 skip.
						searchingDir.Format( "%s\\%s\\%s\\%s\\%s", 
							m_strDirectoryName, strCreatureType[iType], strSlayerPart[ iPart], actionDirec, strDirection[ iDirection]);
						if( testFF.FindFile( searchingDir) == FALSE) break;	// 찾는 디렉이 없으면 파트/액션 자체가 없으므로 다음 액션으로 간다.
						else testFF.Close();


						// -------------------------------------------------
						// 이미 존재하지 않는 경우만 새로 기록한다.
						// -------------------------------------------------
						CString idStr = GetSearchString( iType, iAction, iPart, iDirection);
						int t;
find_again:
						if( (t = strData.Find( idStr)) != -1)
						{
							int end;
							// 제거.
							if( ( end = strData.Find("\n", t)) != -1)
							{
								strData.Delete( t, end - t + 1);
								goto find_again;	// 여러개 있을 수도 있다. 예전의 버그에 의해서..
							}
						}

//						if( true)
						{
							// -------------------------------------------------
							// 찾을 디렉터리 이름을 만든다.
							// -------------------------------------------------
							CString searchingFile;

							searchingFile.Format( "%s\\%s\\%s\\%s\\%s\\*.bmp",
								m_strDirectoryName, strCreatureType[iType], 
								strSlayerPart[iPart], actionDirec, strDirection[iDirection]);


							// -------------------------------------------------
							// bmp들을 찾아서 이름을 기록한다.
							// -------------------------------------------------
							CString bmpNames[200];	 // fix me 200개 넘으면?
							CFileFind ffBMP;
							BYTE btBMPCount = 0;
							CString fileNames;
							BOOL bFoundBMP = ffBMP.FindFile( searchingFile);
							if( bFoundBMP)
							{
								while( bFoundBMP)
								{
									if( btBMPCount>= 200)
									{
										MessageBox( "Too Many Bmps");
										return;
									}
									bFoundBMP = ffBMP.FindNextFile();
									CString fileName = ffBMP.GetFilePath();
									CString name = ffBMP.GetFileName();

									char stemp[10], temp[10];
									bool search = false;
									for( int i=1; i<256; i++)
									{
										wsprintf(stemp, "i%d_", i+1);
										wsprintf(temp,  "I%d_", i+1);
										if(name.Find(temp) != -1 || name.Find(stemp) != -1)
										{											
											search = true;
										}
									}

									if( (name.Find("I_") == -1 && name.Find( "i_") == -1 && !search) && 
										( (bUpdateShadow && (name.Find("그림자") != -1) ) 
										|| (!bUpdateShadow && (name.Find("그림자") == -1) ) ) ) bmpNames[ btBMPCount++] = fileName;										
								}

								
								// -------------------------------------------------
								// bmp들을 이름순으로 정렬한다.
								// -------------------------------------------------
								for( int s1 = 0; s1< btBMPCount-1; s1++)
								{
									for( int s2 = 0; s2 < btBMPCount-1; s2++)
									{
										if( bmpNames[ s2] > bmpNames[ s2+1])	
										{
											// swap 
											CString temp = bmpNames[ s2];
											bmpNames[ s2] = bmpNames[ s2+1];
											bmpNames[ s2+1] = temp;
										}
									}
								}

								if( btBMPCount)
								{
									// for bike
									// 같은 그림으로 4개 복사..
									//
									//		몸	오토	무기
									//이동 1111 1234    1111
									//정지 1234 1111    1111
									//
									if( btBMPCount == BIKE_FRAME_COUNT)
									{
										if( (iAction == ACTION_BIKE_MOVE1 && iPart != PART_BIKE1) //|| (iAction == ACTION_BIKE_MOVE2 )//&& iPart != PART_BIKE2)
											|| ((iAction == ACTION_BIKE_STOP1 )//|| iAction == ACTION_BIKE_STOP2) 
											&& !isBodyPart( iPart)) )
										{
											for( int _temp = 0; _temp < BIKE_FRAME_COUNT; _temp++)
												bmpNames[_temp] = bmpNames[0];
										}
									}
								}

								// -------------------------------------------------
								// 
								// -------------------------------------------------
								for( s1 = 0; s1< btBMPCount; s1++)
								{
									fileNames += "<";
									fileNames += bmpNames[ s1];
									fileNames += ">";
								}


								CString newData;
								if( btBMPCount)
								{
									// data에 삽입.
									newData += btBMPCount;
									newData += fileNames;
									newData += "\n";

									strData += idStr;
									strData += newData;


									// -------------------------------------------------
									// Animation Frame Data저장.
									// -------------------------------------------------
									if( bUpdateAniData)
									{
										// 기본 ani index저장...
										idStr = GetSearchString( iType, iAction, iPart);
										// 없는 경우 저장.
										if( m_strAniData.Find( idStr, 0) == -1)
										{
											if( iAction == ACTION_BIKE_MOVE1 )//|| iAction == ACTION_BIKE_MOVE2 )
											{
												newData = BIKE_FRAME_COUNT;
												for( int t = 0; t< BIKE_FRAME_COUNT; t++)
													newData += (BYTE)(t+1);
											}
											else
											{
												newData = btBMPCount;
												for( int t = 0; t< btBMPCount; t++)
												{
													// 오토바이 처리..
														newData+= (BYTE)(t+1);
												}
											}
											m_strAniData += idStr;
											m_strAniData += newData;

										}
									}
								}
							}
						}
				}
			}
			
//			DoSpecialAction( iType);


		}
		else if( IsVampireType( iType))
		{
			for( iPart = 0; iPart< VAMPIRE_PART_MAX; iPart++)
			for( iAction = 0; iAction< ACTION_MAX; iAction++)
			{
				for( iDirection = 0; iDirection< DIR_MAX; iDirection++)
				{
						// 없으면 skip.
						searchingDir.Format( "%s\\%s\\%s\\%s\\%s", 
							m_strDirectoryName, strCreatureType[iType], strVampirePart[ iPart], strActionForSearch[ iAction], strDirection[ iDirection]);
						if( testFF.FindFile( searchingDir) == FALSE) break;	// 찾는 디렉이 없으면 파트/액션 자체가 없으므로 다음 액션으로 간다.
						else testFF.Close();

						// -------------------------------------------------
						// 이미 존재하지 않는 경우만 새로 기록한다.
						// -------------------------------------------------
						CString idStr = GetSearchString( iType, iAction, iPart, iDirection);

						int t;
find_again2:
						if( (t = strData.Find( idStr)) != -1)
						{
							int end;
							// 제거.
							if( ( end = strData.Find("\n", t)) != -1)
							{
								strData.Delete( t, end - t + 1);
								goto find_again2;
							}
						}
//						if( true)
						{


							// -------------------------------------------------
							// 찾을 디렉터리 이름을 만든다.
							// -------------------------------------------------
							CString searchingFile;
							searchingFile.Format( "%s\\%s\\%s\\%s\\%s\\*.bmp",
								m_strDirectoryName, strCreatureType[iType], strVampirePart[iPart],
								strActionForSearch[ iAction], strDirection[iDirection]);


							// -------------------------------------------------
							// bmp들을 찾아서 이름을 기록한다.
							// -------------------------------------------------
							CString bmpNames[200];	 // fix me 200개 넘으면?
							CFileFind ffBMP;
							BYTE btBMPCount = 0;
							CString fileNames;
							BOOL bFoundBMP = ffBMP.FindFile( searchingFile);
							if( bFoundBMP)
							{
								while( bFoundBMP)
								{
									if( btBMPCount>= 200)
									{
										MessageBox( "Too Many Bmps");
										return;
									}
									bFoundBMP = ffBMP.FindNextFile();
									CString fileName = ffBMP.GetFilePath();
									CString name = ffBMP.GetFileName();
									char stemp[10], temp[10];
									bool search = false;
									for( int i=1; i<256; i++)
									{
										wsprintf(stemp, "i%d_", i+1);
										wsprintf(temp,  "I%d_", i+1);
										if(name.Find(temp) != -1 || name.Find(stemp) != -1)
										{											
											search = true;
										}
									}

									if( (name.Find("I_") == -1 && name.Find( "i_") == -1 && !search) && 
									( (bUpdateShadow && (name.Find("그림자") != -1) ) 
										|| (!bUpdateShadow && (name.Find("그림자") == -1) ) ) )
										bmpNames[ btBMPCount++] = fileName;
								}

								
								// -------------------------------------------------
								// bmp들을 이름순으로 정렬한다.
								// -------------------------------------------------
								for( int s1 = 0; s1< btBMPCount-1; s1++)
								{
									for( int s2 = 0; s2 < btBMPCount-1; s2++)
									{
										if( bmpNames[ s2] > bmpNames[ s2+1])	
										{
											// swap 
											CString temp = bmpNames[ s2];
											bmpNames[ s2] = bmpNames[ s2+1];
											bmpNames[ s2+1] = temp;
										}
									}
								}
								

								// -------------------------------------------------
								// 
								// -------------------------------------------------
								for( s1 = 0; s1< btBMPCount; s1++)
								{
									fileNames += "<";
									fileNames += bmpNames[ s1];
									fileNames += ">";
								}


								// -------------------------------------------------
								// Animation Data저장.
								// -------------------------------------------------
								CString newData;
								if( btBMPCount)
								{
									// data에 삽입.
									newData += btBMPCount;
									newData += fileNames;
									newData += "\n";

									strData += idStr;
									strData += newData;


									// -------------------------------------------------
									// Animation Frame Data저장.
									// -------------------------------------------------
									if( bUpdateAniData)
									{
										// 기본 ani index저장...
										idStr = GetSearchString( iType, iAction, iPart);
										// 없는 경우 저장.
										if( m_strAniData.Find( idStr, 0) == -1)
										{
											newData = btBMPCount;
											for( int t = 0; t< btBMPCount; t++)
											{
												newData+= (BYTE)(t+1);
											}
											m_strAniData += idStr;
											m_strAniData += newData;

										}
									}
								}
							}
						}
				}
			}
		}
		else if( IsOustersType( iType))
		{
			for( iPart = 0; iPart< OUSTERS_PART_MAX; iPart++)
			for( iAction = 0; iAction< OUSTERS_ACTION_MAX; iAction++)
			{
				for( iDirection = 0; iDirection< DIR_MAX; iDirection++)
				{
						// 없으면 skip.
						searchingDir.Format( "%s\\%s\\%s\\%s\\%s", 
							m_strDirectoryName, strCreatureType[iType], strOustersPart[ iPart], strOustersActionForSearch[ iAction], strDirection[ iDirection]);
						if( testFF.FindFile( searchingDir) == FALSE) break;	// 찾는 디렉이 없으면 파트/액션 자체가 없으므로 다음 액션으로 간다.
						else testFF.Close();

						// -------------------------------------------------
						// 이미 존재하지 않는 경우만 새로 기록한다.
						// -------------------------------------------------
						CString idStr = GetSearchString( iType, iAction, iPart, iDirection);

						int t;
find_again3:
						if( (t = strData.Find( idStr)) != -1)
						{
							int end;
							// 제거.
							if( ( end = strData.Find("\n", t)) != -1)
							{
								strData.Delete( t, end - t + 1);
								goto find_again3;
							}
						}
//						if( true)
						{


							// -------------------------------------------------
							// 찾을 디렉터리 이름을 만든다.
							// -------------------------------------------------
							CString searchingFile;
							searchingFile.Format( "%s\\%s\\%s\\%s\\%s\\*.bmp",
								m_strDirectoryName, strCreatureType[iType], strOustersPart[iPart],
								strOustersActionForSearch[ iAction], strDirection[iDirection]);


							// -------------------------------------------------
							// bmp들을 찾아서 이름을 기록한다.
							// -------------------------------------------------
							CString bmpNames[200];	 // fix me 200개 넘으면?
							CFileFind ffBMP;
							BYTE btBMPCount = 0;
							CString fileNames;
							BOOL bFoundBMP = ffBMP.FindFile( searchingFile);
							if( bFoundBMP)
							{
								while( bFoundBMP)
								{
									if( btBMPCount>= 200)
									{
										MessageBox( "Too Many Bmps");
										return;
									}
									bFoundBMP = ffBMP.FindNextFile();
									CString fileName = ffBMP.GetFilePath();
									CString name = ffBMP.GetFileName();
									char stemp[10], temp[10];
									bool search = false;
									for( int i=1; i<256; i++)
									{
										wsprintf(stemp, "i%d_", i+1);
										wsprintf(temp,  "I%d_", i+1);
										if(name.Find(temp) != -1 || name.Find(stemp) != -1)
										{											
											search = true;
										}
									}

									if( (name.Find("I_") == -1 && name.Find( "i_") == -1 && !search) && 
									( (bUpdateShadow && (name.Find("그림자") != -1) ) 
										|| (!bUpdateShadow && (name.Find("그림자") == -1) ) ) )
										bmpNames[ btBMPCount++] = fileName;
								}

								
								// -------------------------------------------------
								// bmp들을 이름순으로 정렬한다.
								// -------------------------------------------------
								for( int s1 = 0; s1< btBMPCount-1; s1++)
								{
									for( int s2 = 0; s2 < btBMPCount-1; s2++)
									{
										if( bmpNames[ s2] > bmpNames[ s2+1])	
										{
											// swap 
											CString temp = bmpNames[ s2];
											bmpNames[ s2] = bmpNames[ s2+1];
											bmpNames[ s2+1] = temp;
										}
									}
								}
								

								// -------------------------------------------------
								// 
								// -------------------------------------------------
								for( s1 = 0; s1< btBMPCount; s1++)
								{
									fileNames += "<";
									fileNames += bmpNames[ s1];
									fileNames += ">";
								}


								// -------------------------------------------------
								// Animation Data저장.
								// -------------------------------------------------
								CString newData;
								if( btBMPCount)
								{
									// data에 삽입.
									newData += btBMPCount;
									newData += fileNames;
									newData += "\n";

									strData += idStr;
									strData += newData;


									// -------------------------------------------------
									// Animation Frame Data저장.
									// -------------------------------------------------
									if( bUpdateAniData)
									{
										// 기본 ani index저장...
										idStr = GetSearchString( iType, iAction, iPart);
										// 없는 경우 저장.
										if( m_strAniData.Find( idStr, 0) == -1)
										{
											newData = btBMPCount;
											for( int t = 0; t< btBMPCount; t++)
											{
												newData+= (BYTE)(t+1);
											}
											m_strAniData += idStr;
											m_strAniData += newData;

										}
									}
								}
							}
						}
				}
			}
		}
		else if( IsPromotionSlayer( iType))
		{
			for( iPart = 0; iPart< PS_PART_MAX; iPart++)
			{
				for( iAction = 0; iAction< ACTION_SLAYER_PROMOTION_MAX; iAction++)
				{
					
					CString actionDirec;
					actionDirec = strPromotionSlayerActionForSearch[ iAction];
					
					for( iDirection = 0; iDirection< DIR_MAX; iDirection++)
					{
						// 없으면 skip.
						searchingDir.Format( "%s\\%s\\%s\\%s\\%s", 
							m_strDirectoryName, 
							strCreatureType[iType], 
							strPromotionSlayerPart[ iPart ],							
							actionDirec, 
							strDirection[ iDirection]);
						
						if( testFF.FindFile( searchingDir) == FALSE) break;	
						// 찾는 디렉이 없으면 파트/액션 자체가 없으므로 다음 액션으로 간다.
						else testFF.Close();
						
						
						// -------------------------------------------------
						// 이미 존재하지 않는 경우만 새로 기록한다.
						// -------------------------------------------------
						CString idStr = GetSearchString( iType, iAction, iPart, iDirection);
						int t;
						
						while(1)
						{
							if( (t = strData.Find( idStr)) != -1)
							{
								int end;
								// 제거.
								if( ( end = strData.Find("\n", t)) != -1)
								{
									strData.Delete( t, end - t + 1);
									continue;	// 여러개 있을 수도 있다. 예전의 버그에 의해서..
								}
							}
							break;
						}
						
						// -------------------------------------------------
						// 찾을 디렉터리 이름을 만든다.
						// -------------------------------------------------
						CString searchingFile;
						
						searchingFile.Format( "%s\\%s\\%s\\%s\\%s\\*.bmp",
							m_strDirectoryName, 
							strCreatureType[iType], 
							//strSlayerPart[iPart], 
							strPromotionSlayerPart[ iPart ],
							actionDirec, 
							strDirection[iDirection]);
						
						
						// -------------------------------------------------
						// bmp들을 찾아서 이름을 기록한다.
						// -------------------------------------------------
						CString bmpNames[200];	 // fix me 200개 넘으면?
						CFileFind ffBMP;
						BYTE btBMPCount = 0;
						CString fileNames;
						BOOL bFoundBMP = ffBMP.FindFile( searchingFile);
						if( bFoundBMP)
						{
							while( bFoundBMP)
							{
								if( btBMPCount>= 200)
								{
									MessageBox( "Too Many Bmps");
									return;
								}
								bFoundBMP = ffBMP.FindNextFile();
								CString fileName = ffBMP.GetFilePath();
								CString name = ffBMP.GetFileName();
								
								char stemp[10], temp[10];
								bool search = false;
								for( int i=1; i<256; i++)
								{
									wsprintf(stemp, "i%d_", i+1);
									wsprintf(temp,  "I%d_", i+1);
									if(name.Find(temp) != -1 || name.Find(stemp) != -1)
									{											
										search = true;
									}
								}
								
								if( (name.Find("I_") == -1 && name.Find( "i_") == -1 && !search) && 
									( (bUpdateShadow && (name.Find("그림자") != -1) ) 
									|| (!bUpdateShadow && (name.Find("그림자") == -1) ) ) ) 
									bmpNames[ btBMPCount++] = fileName;										
							}
							
							
							// -------------------------------------------------
							// bmp들을 이름순으로 정렬한다.
							// -------------------------------------------------
							for( int s1 = 0; s1< btBMPCount-1; s1++)
							{
								for( int s2 = 0; s2 < btBMPCount-1; s2++)
								{
									if( bmpNames[ s2] > bmpNames[ s2+1])	
									{
										// swap 
										CString temp = bmpNames[ s2];
										bmpNames[ s2] = bmpNames[ s2+1];
										bmpNames[ s2+1] = temp;
									}
								}
							}
							
							// -------------------------------------------------
							// 
							// -------------------------------------------------
							for( s1 = 0; s1< btBMPCount; s1++)
							{
								fileNames += "<";
								fileNames += bmpNames[ s1];
								fileNames += ">";
							}
							
							
							CString newData;
							if( btBMPCount)
							{
								// data에 삽입.
								newData += btBMPCount;
								newData += fileNames;
								newData += "\n";
								
								strData += idStr;
								strData += newData;
								
								
								// -------------------------------------------------
								// Animation Frame Data저장.
								// -------------------------------------------------
								if( bUpdateAniData)
								{
									// 기본 ani index저장...
									idStr = GetSearchString( iType, iAction, iPart);
									// 없는 경우 저장.
									if( m_strAniData.Find( idStr, 0) == -1)
									{
										
										newData = btBMPCount;
										for( int t = 0; t< btBMPCount; t++)
										{
											// 오토바이 처리..
											newData+= (BYTE)(t+1);
										}
										m_strAniData += idStr;
										m_strAniData += newData;
										
									}
								}
							}
						}
					}
				}
			}
		}
		else if( IsPromotionOusters( iType ) )
		{
			for( iPart = 0; iPart< PO_PART_MAX; iPart++)
			for( iAction = 0; iAction< ACTION_OUSTERS_PROMOTION_MAX; iAction++)
			{
				for( iDirection = 0; iDirection< DIR_MAX; iDirection++)
				{
						// 없으면 skip.
						searchingDir.Format( "%s\\%s\\%s\\%s\\%s", 
							m_strDirectoryName, strCreatureType[iType], 
							strPromotionOustersPart[ iPart], 
							strPromotionOustersActionForSearch[ iAction], 
							strDirection[ iDirection]);

						if( testFF.FindFile( searchingDir) == FALSE) break;	// 찾는 디렉이 없으면 파트/액션 자체가 없으므로 다음 액션으로 간다.
						else testFF.Close();

						// -------------------------------------------------
						// 이미 존재하지 않는 경우만 새로 기록한다.
						// -------------------------------------------------
						CString idStr = GetSearchString( iType, iAction, iPart, iDirection);

						int t;
find_again4:
						if( (t = strData.Find( idStr)) != -1)
						{
							int end;
							// 제거.
							if( ( end = strData.Find("\n", t)) != -1)
							{
								strData.Delete( t, end - t + 1);
								goto find_again4;
							}
						}
//						if( true)
						{


							// -------------------------------------------------
							// 찾을 디렉터리 이름을 만든다.
							// -------------------------------------------------
							CString searchingFile;
							searchingFile.Format( "%s\\%s\\%s\\%s\\%s\\*.bmp",
								m_strDirectoryName, strCreatureType[iType], strPromotionOustersPart[iPart],
								strPromotionOustersActionForSearch[ iAction], strDirection[iDirection]);


							// -------------------------------------------------
							// bmp들을 찾아서 이름을 기록한다.
							// -------------------------------------------------
							CString bmpNames[200];	 // fix me 200개 넘으면?
							CFileFind ffBMP;
							BYTE btBMPCount = 0;
							CString fileNames;
							BOOL bFoundBMP = ffBMP.FindFile( searchingFile);
							if( bFoundBMP)
							{
								while( bFoundBMP)
								{
									if( btBMPCount>= 200)
									{
										MessageBox( "Too Many Bmps");
										return;
									}
									bFoundBMP = ffBMP.FindNextFile();
									CString fileName = ffBMP.GetFilePath();
									CString name = ffBMP.GetFileName();
									char stemp[10], temp[10];
									bool search = false;
									for( int i=1; i<256; i++)
									{
										wsprintf(stemp, "i%d_", i+1);
										wsprintf(temp,  "I%d_", i+1);
										if(name.Find(temp) != -1 || name.Find(stemp) != -1)
										{											
											search = true;
										}
									}

									if( (name.Find("I_") == -1 && name.Find( "i_") == -1 && !search) && 
									( (bUpdateShadow && (name.Find("그림자") != -1) ) 
										|| (!bUpdateShadow && (name.Find("그림자") == -1) ) ) )
										bmpNames[ btBMPCount++] = fileName;
								}

								
								// -------------------------------------------------
								// bmp들을 이름순으로 정렬한다.
								// -------------------------------------------------
								for( int s1 = 0; s1< btBMPCount-1; s1++)
								{
									for( int s2 = 0; s2 < btBMPCount-1; s2++)
									{
										if( bmpNames[ s2] > bmpNames[ s2+1])	
										{
											// swap 
											CString temp = bmpNames[ s2];
											bmpNames[ s2] = bmpNames[ s2+1];
											bmpNames[ s2+1] = temp;
										}
									}
								}
								

								// -------------------------------------------------
								// 
								// -------------------------------------------------
								for( s1 = 0; s1< btBMPCount; s1++)
								{
									fileNames += "<";
									fileNames += bmpNames[ s1];
									fileNames += ">";
								}


								// -------------------------------------------------
								// Animation Data저장.
								// -------------------------------------------------
								CString newData;
								if( btBMPCount)
								{
									// data에 삽입.
									newData += btBMPCount;
									newData += fileNames;
									newData += "\n";

									strData += idStr;
									strData += newData;


									// -------------------------------------------------
									// Animation Frame Data저장.
									// -------------------------------------------------
									if( bUpdateAniData)
									{
										// 기본 ani index저장...
										idStr = GetSearchString( iType, iAction, iPart);
										// 없는 경우 저장.
										if( m_strAniData.Find( idStr, 0) == -1)
										{
											newData = btBMPCount;
											for( int t = 0; t< btBMPCount; t++)
											{
												newData+= (BYTE)(t+1);
											}
											m_strAniData += idStr;
											m_strAniData += newData;

										}
									}
								}
							}
						}
				}
			}
		}
		else if( IsPromotionVampire( iType ) )
		{
			for( iPart = 0; iPart< AC_PART_MAX; iPart++)
			for( iAction = 0; iAction< ACTION_ADVANCEMENT_MAX; iAction++)
			{
				for( iDirection = 0; iDirection< DIR_MAX; iDirection++)
				{
						// 없으면 skip.
						searchingDir.Format( "%s\\%s\\%s\\%s\\%s", 
							m_strDirectoryName, strCreatureType[iType], 
							strAdvancementClassVampirePart[ iPart], 
							strAdvancementClassVampireActionForSearch[ iAction], 
							strDirection[ iDirection]);

						if( testFF.FindFile( searchingDir) == FALSE) break;	// 찾는 디렉이 없으면 파트/액션 자체가 없으므로 다음 액션으로 간다.
						else testFF.Close();

						// -------------------------------------------------
						// 이미 존재하지 않는 경우만 새로 기록한다.
						// -------------------------------------------------
						CString idStr = GetSearchString( iType, iAction, iPart, iDirection);

						int t;
find_again44:
						if( (t = strData.Find( idStr)) != -1)
						{
							int end;
							// 제거.
							if( ( end = strData.Find("\n", t)) != -1)
							{
								strData.Delete( t, end - t + 1);
								goto find_again44;
							}
						}
//						if( true)
						{


							// -------------------------------------------------
							// 찾을 디렉터리 이름을 만든다.
							// -------------------------------------------------
							CString searchingFile;
							searchingFile.Format( "%s\\%s\\%s\\%s\\%s\\*.bmp",
								m_strDirectoryName, strCreatureType[iType], strAdvancementClassVampirePart[iPart],
								strAdvancementClassVampireActionForSearch[ iAction], strDirection[iDirection]);


							// -------------------------------------------------
							// bmp들을 찾아서 이름을 기록한다.
							// -------------------------------------------------
							CString bmpNames[200];	 // fix me 200개 넘으면?
							CFileFind ffBMP;
							BYTE btBMPCount = 0;
							CString fileNames;
							BOOL bFoundBMP = ffBMP.FindFile( searchingFile);
							if( bFoundBMP)
							{
								while( bFoundBMP)
								{
									if( btBMPCount>= 200)
									{
										MessageBox( "Too Many Bmps");
										return;
									}
									bFoundBMP = ffBMP.FindNextFile();
									CString fileName = ffBMP.GetFilePath();
									CString name = ffBMP.GetFileName();
									char stemp[10], temp[10];
									bool search = false;
									for( int i=1; i<256; i++)
									{
										wsprintf(stemp, "i%d_", i+1);
										wsprintf(temp,  "I%d_", i+1);
										if(name.Find(temp) != -1 || name.Find(stemp) != -1)
										{											
											search = true;
										}
									}

									if( (name.Find("I_") == -1 && name.Find( "i_") == -1 && !search) && 
									( (bUpdateShadow && (name.Find("그림자") != -1) ) 
										|| (!bUpdateShadow && (name.Find("그림자") == -1) ) ) )
										bmpNames[ btBMPCount++] = fileName;
								}

								
								// -------------------------------------------------
								// bmp들을 이름순으로 정렬한다.
								// -------------------------------------------------
								for( int s1 = 0; s1< btBMPCount-1; s1++)
								{
									for( int s2 = 0; s2 < btBMPCount-1; s2++)
									{
										if( bmpNames[ s2] > bmpNames[ s2+1])	
										{
											// swap 
											CString temp = bmpNames[ s2];
											bmpNames[ s2] = bmpNames[ s2+1];
											bmpNames[ s2+1] = temp;
										}
									}
								}
								

								// -------------------------------------------------
								// 
								// -------------------------------------------------
								for( s1 = 0; s1< btBMPCount; s1++)
								{
									fileNames += "<";
									fileNames += bmpNames[ s1];
									fileNames += ">";
								}


								// -------------------------------------------------
								// Animation Data저장.
								// -------------------------------------------------
								CString newData;
								if( btBMPCount)
								{
									// data에 삽입.
									newData += btBMPCount;
									newData += fileNames;
									newData += "\n";

									strData += idStr;
									strData += newData;


									// -------------------------------------------------
									// Animation Frame Data저장.
									// -------------------------------------------------
									if( bUpdateAniData)
									{
										// 기본 ani index저장...
										idStr = GetSearchString( iType, iAction, iPart);
										// 없는 경우 저장.
										if( m_strAniData.Find( idStr, 0) == -1)
										{
											newData = btBMPCount;
											for( int t = 0; t< btBMPCount; t++)
											{
												newData+= (BYTE)(t+1);
											}
											m_strAniData += idStr;
											m_strAniData += newData;

										}
									}
								}
							}
						}
				}
			}
		}
		else 
		{
			for( iPart = 0; iPart< VAMPIRE_PART_MAX; iPart++)
			for( iAction = 0; iAction< ACTION_MAX; iAction++)
			{
				for( iDirection = 0; iDirection< DIR_MAX; iDirection++)
				{
						// 없으면 skip.
						searchingDir.Format( "%s\\%s\\%s\\%s", 
							m_strDirectoryName, strCreatureType[iType], strActionForSearch[ iAction], strDirection[ iDirection]);
						if( testFF.FindFile( searchingDir) == FALSE) break;	// 찾는 디렉이 없으면 파트/액션 자체가 없으므로 다음 액션으로 간다.
						else testFF.Close();

						// -------------------------------------------------
						// 이미 존재하지 않는 경우만 새로 기록한다.
						// -------------------------------------------------
						CString idStr = GetSearchString( iType, iAction, iPart, iDirection);

						int t;
find_again33:
						if( (t = strData.Find( idStr)) != -1)
						{
							int end;
							// 제거.
							if( ( end = strData.Find("\n", t)) != -1)
							{
								strData.Delete( t, end - t + 1);
								goto find_again33;
							}
						}
//						if( true)
						{


							// -------------------------------------------------
							// 찾을 디렉터리 이름을 만든다.
							// -------------------------------------------------
							CString searchingFile;
							searchingFile.Format( "%s\\%s\\%s\\%s\\*.bmp",
								m_strDirectoryName, strCreatureType[iType], 
								strActionForSearch[ iAction], strDirection[iDirection]);


							// -------------------------------------------------
							// bmp들을 찾아서 이름을 기록한다.
							// -------------------------------------------------
							CString bmpNames[200];	 // fix me 200개 넘으면?
							CFileFind ffBMP;
							BYTE btBMPCount = 0;
							CString fileNames;
							BOOL bFoundBMP = ffBMP.FindFile( searchingFile);
							if( bFoundBMP)
							{
								while( bFoundBMP)
								{
									if( btBMPCount>= 200)
									{
										MessageBox( "Too Many Bmps");
										return;
									}
									bFoundBMP = ffBMP.FindNextFile();
									CString fileName = ffBMP.GetFilePath();
									CString name = ffBMP.GetFileName();
									char stemp[10], temp[10];
									bool search = false;
									for( int i=1; i<256; i++)
									{
										wsprintf(stemp, "i%d_", i+1);
										wsprintf(temp,  "I%d_", i+1);
										if(name.Find(temp) != -1 || name.Find(stemp) != -1)
										{											
											search = true;
										}
									}

									if( (name.Find("I_") == -1 && name.Find( "i_") == -1 && !search) && 
									( (bUpdateShadow && (name.Find("그림자") != -1) ) 
										|| (!bUpdateShadow && (name.Find("그림자") == -1) ) ) )
										bmpNames[ btBMPCount++] = fileName;
								}

								
								// -------------------------------------------------
								// bmp들을 이름순으로 정렬한다.
								// -------------------------------------------------
								for( int s1 = 0; s1< btBMPCount-1; s1++)
								{
									for( int s2 = 0; s2 < btBMPCount-1; s2++)
									{
										if( bmpNames[ s2] > bmpNames[ s2+1])	
										{
											// swap 
											CString temp = bmpNames[ s2];
											bmpNames[ s2] = bmpNames[ s2+1];
											bmpNames[ s2+1] = temp;
										}
									}
								}
								

								// -------------------------------------------------
								// 
								// -------------------------------------------------
								for( s1 = 0; s1< btBMPCount; s1++)
								{
									fileNames += "<";
									fileNames += bmpNames[ s1];
									fileNames += ">";
								}


								// -------------------------------------------------
								// Animation Data저장.
								// -------------------------------------------------
								CString newData;
								if( btBMPCount)
								{
									// data에 삽입.
									newData += btBMPCount;
									newData += fileNames;
									newData += "\n";

									strData += idStr;
									strData += newData;


									// -------------------------------------------------
									// Animation Frame Data저장.
									// -------------------------------------------------
									if( bUpdateAniData)
									{
										// 기본 ani index저장...
										idStr = GetSearchString( iType, iAction, iPart);
										// 없는 경우 저장.
										if( m_strAniData.Find( idStr, 0) == -1)
										{
											newData = btBMPCount;
											for( int t = 0; t< btBMPCount; t++)
											{
												newData+= (BYTE)(t+1);
											}
											m_strAniData += idStr;
											m_strAniData += newData;

										}
									}
								}
							}
						}
				}
			}


		}


	}
}

void CSpriteManagerDlg::OnButtonLoad() 
{
	LoadDataFile();
}

void CSpriteManagerDlg::LoadDataFile()
{


	// -------------------------------------------------
	// Set current directory for FileDialog
	// -------------------------------------------------
	char currentDirectory[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, currentDirectory);
	strcat( currentDirectory, "\\*.dat");

//	CProgressDlg progressDlg;
//	progressDlg.Create( IDD_DIALOG_PROGRESS1, this);
//	progressDlg.ShowWindow( SW_SHOW);

	CFileDialog fileDlg( TRUE, "dat", currentDirectory);
//	CFileDialog fileDlg( TRUE, "dat", DATAFILE_NAME);
	if( fileDlg.DoModal() == IDOK)
	{

		////////////////////////////////////////////////////
		// data files read
		////////////////////////////////////////////////////

		
		CFile dataFile;

		if( dataFile.Open( fileDlg.GetPathName(), CFile::modeRead))
		{
			DWORD dwDataLength = 0;

			if( MessageBox("원래 존재하는 데이타에 더할까요?", "확인", MB_YESNO) == IDNO)
			{
				std::string srcData= "I:\\";
				std::string tarData= "D:\\다크에덴작업\\";
				std::string temp;
				int i=0;
				CFile tempFile;
				
				// -------------------------------------------------
				// Read animation bmp data
				// -------------------------------------------------
				dataFile.Read( &dwDataLength, sizeof(DWORD));
				dataFile.Read( m_strData.GetBufferSetLength( dwDataLength), dwDataLength);

// 				temp = m_strData;
//				progressDlg.SetText(CString("FirstFile"));
//				DWORD firstTime = GetTickCount();
//				DWORD LeftTime = 0;
//				int firstdata=0;
//				
//				while((i = temp.find(srcData)) != -1)
//				{
//					temp.replace(temp.begin()+i,temp.begin()+i+srcData.size(),tarData);
//					char ttt[200];
//					sprintf(ttt,"%d/%d (left %d:%d)",i,temp.size(),LeftTime/1000/60,(LeftTime/1000)%60);
//					progressDlg.SetText(CString(ttt));
//					progressDlg.SetProgress(i,dwDataLength);
//					
//					if((i-firstdata) >= 5000)
//					{
//						LeftTime = ((GetTickCount() - firstTime) * (temp.size()-i)) / (i-firstdata);
//						firstdata = i;
//						firstTime = GetTickCount();
//						progressDlg.ShowWindow( SW_SHOW);
//					}
//				}
//				m_strData = temp.c_str();
//				

				// -------------------------------------------------
				// Read animation shadow bmp data
				// -------------------------------------------------
				dataFile.Read( &dwDataLength, sizeof(DWORD));
				dataFile.Read( m_strShadowData.GetBufferSetLength( dwDataLength), dwDataLength);

//				temp = m_strShadowData;
//				progressDlg.SetText(CString("2File"));
//				while((i = temp.find(srcData)) != -1)
//				{
//					temp.replace(temp.begin()+i,temp.begin()+i+srcData.size(),tarData);
//					char ttt[200];
//					sprintf(ttt,"%d/%d (left %d:%d)",i,dwDataLength,LeftTime/1000/60,(LeftTime/1000)%60);
//					progressDlg.SetText(CString(ttt));
//					progressDlg.SetProgress(i,dwDataLength);
//					
//					if((i-firstdata) >= 5000)
//					{
//						LeftTime = ((GetTickCount() - firstTime) * (dwDataLength-i)) / (i-firstdata);
//						firstdata = i;
//						firstTime = GetTickCount();
//						progressDlg.ShowWindow( SW_SHOW);
//					}
//				}
//
//				m_strShadowData = temp.c_str();
				
				// -------------------------------------------------
				// Read animation frame data
				// -------------------------------------------------
				dataFile.Read( &dwDataLength, sizeof(DWORD));
				dataFile.Read( m_strAniData.GetBufferSetLength( dwDataLength), dwDataLength);

//				temp = m_strAniData;
//				progressDlg.SetText(CString("3File"));
//				while((i = temp.find(srcData)) != -1)
//				{
//					temp.replace(temp.begin()+i,temp.begin()+i+srcData.size(),tarData);
//					char ttt[200];
//					sprintf(ttt,"%d/%d (left %d:%d)",i,dwDataLength,LeftTime/1000/60,(LeftTime/1000)%60);
//					progressDlg.SetText(CString(ttt));
//					progressDlg.SetProgress(i,dwDataLength);
//					
//					if((i-firstdata) >= 5000)
//					{
//						LeftTime = ((GetTickCount() - firstTime) * (dwDataLength-i)) / (i-firstdata);
//						firstdata = i;
//						firstTime = GetTickCount();
//						progressDlg.ShowWindow( SW_SHOW);
//					}
//				}
//
//				m_strAniData = temp.c_str();

				// -------------------------------------------------
				// Read animation offset point data
				// -------------------------------------------------
				dataFile.Read( &dwDataLength, sizeof(DWORD));
				dataFile.Read( m_strOffsetData.GetBufferSetLength( dwDataLength), dwDataLength);

//				temp = m_strOffsetData;
//				progressDlg.SetText(CString("4File"));
//				while((i = temp.find(srcData)) != -1)
//				{
//					temp.replace(temp.begin()+i,temp.begin()+i+srcData.size(),tarData);
//					char ttt[200];
//					sprintf(ttt,"%d/%d (left %d:%d)",i,dwDataLength,LeftTime/1000/60,(LeftTime/1000)%60);
//					progressDlg.SetText(CString(ttt));
//					progressDlg.SetProgress(i,dwDataLength);
//					
//					if((i-firstdata) >= 5000)
//					{
//						LeftTime = ((GetTickCount() - firstTime) * (dwDataLength-i)) / (i-firstdata);
//						firstdata = i;
//						firstTime = GetTickCount();
//						progressDlg.ShowWindow( SW_SHOW);
//					}
//				}
//
//				m_strOffsetData = temp.c_str();


				// -------------------------------------------------
				// Read animation frame copy flag data
				// -------------------------------------------------
				dataFile.Read( &dwDataLength, sizeof(DWORD));
				dataFile.Read( m_strCopyFlagData.GetBufferSetLength( dwDataLength), dwDataLength);

//				temp = m_strCopyFlagData;
//				progressDlg.SetText(CString("5File"));
//				while((i = temp.find(srcData)) != -1)
//				{
//					temp.replace(temp.begin()+i,temp.begin()+i+srcData.size(),tarData);
//					char ttt[200];
//					sprintf(ttt,"%d/%d (left %d:%d)",i,dwDataLength,LeftTime/1000/60,(LeftTime/1000)%60);
//					progressDlg.SetText(CString(ttt));
//					progressDlg.SetProgress(i,dwDataLength);
//					
//					if((i-firstdata) >= 5000)
//					{
//						LeftTime = ((GetTickCount() - firstTime) * (dwDataLength-i)) / (i-firstdata);
//						firstdata = i;
//						firstTime = GetTickCount();
//						progressDlg.ShowWindow( SW_SHOW);
//					}
//				}
//
//				m_strCopyFlagData = temp.c_str();
//				
//				tempFile.Close();
			}
			else
			{

				// 합치기.....
				int srcSize;

				// -------------------------------------------------
				// Read animation bmp data
				// -------------------------------------------------
				dataFile.Read( &dwDataLength, sizeof(DWORD));
				srcSize = m_strData.GetLength();
				m_strData.GetBufferSetLength( srcSize + dwDataLength);
				dataFile.Read( m_strData.GetBuffer(0) + srcSize, dwDataLength);


				// -------------------------------------------------
				// Read animation shadow bmp data
				// -------------------------------------------------
				dataFile.Read( &dwDataLength, sizeof(DWORD));
				srcSize = m_strShadowData.GetLength();
				m_strShadowData.GetBufferSetLength( srcSize + dwDataLength);
				dataFile.Read( m_strShadowData.GetBuffer(0) + srcSize, dwDataLength);

				// -------------------------------------------------
				// Read animation frame data
				// -------------------------------------------------
				dataFile.Read( &dwDataLength, sizeof(DWORD));
				srcSize = m_strAniData.GetLength();
				m_strAniData.GetBufferSetLength( srcSize + dwDataLength);
				dataFile.Read( m_strAniData.GetBuffer(0) + srcSize, dwDataLength);


				// -------------------------------------------------
				// Read animation offset point data
				// -------------------------------------------------
				dataFile.Read( &dwDataLength, sizeof(DWORD));
				srcSize = m_strOffsetData.GetLength();
				m_strOffsetData.GetBufferSetLength( srcSize + dwDataLength);
				dataFile.Read( m_strOffsetData.GetBuffer(0) + srcSize, dwDataLength);


				// -------------------------------------------------
				// Read animation frame copy flag data
				// -------------------------------------------------
				dataFile.Read( &dwDataLength, sizeof(DWORD));
				srcSize = m_strCopyFlagData.GetLength();
				m_strCopyFlagData.GetBufferSetLength( srcSize + dwDataLength);
				dataFile.Read( m_strCopyFlagData.GetBuffer(0) + srcSize, dwDataLength);

			}

			dataFile.Close();
			// load data from file
			LoadCurrentSet();
		}
		else
		{
			CString str;
			str.Format( "%s 화일 열기 실패.", DATAFILE_NAME);
			MessageBox( str);
			return;
		}



	}
	else
	{
		MessageBox( "화일 열기 취소");
	}
	//progressDlg.DestroyWindow();
}

void CSpriteManagerDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}
#include "InputCount.h"

void CSpriteManagerDlg::OnRepeatCopy() 
{
	CInputCount  dlg;
	dlg.m_dwCount = 1;
	if( dlg.DoModal() == IDOK && dlg.m_dwCount != 0)
	{
		m_undo.SaveState( m_myData);

		CDib* pSrcDib = m_myData.m_aniList.GetAt2( m_myData.m_iAniIndex[ m_iSelectedAniID]);

		for( DWORD j = 0; j < dlg.m_dwCount; j++)
			AddToAni( pSrcDib->GetFileName(), m_iSelectedAniID, true);
		
	}
	
}

#include"InputOffset.h"
/*
void CSpriteManagerDlg::OnTemp() 
{
	// recalculate all cpfk

	CFileDialog dlg( true, "cfpk", "*.cfpk");
	if( dlg.DoModal() == IDOK)
	{
		CCreatureFramePack cfpk;
		// open
		ifstream packFile( dlg.GetPathName(), ios::binary);
//		ifstream indexFile( dlg.GetPathName(), ios::binary);



		int xOffset = DEFAULT_XOFFSET, yOffset = DEFAULT_YOFFSET;	// 안전을 위해 기본값으로..
		CInputOffset offsetDlg;
		offsetDlg.m_x = xOffset; offsetDlg.m_y = yOffset;
//		offsetDlg.SetWindowText( dlg.GetFileName());

		if( offsetDlg.DoModal() == IDOK)
		{
			xOffset = offsetDlg.m_x; yOffset = offsetDlg.m_y;

			cfpk.LoadFromFile(packFile);
	//		int part = cfpk.GetSize();
			for( int part = 0; part < cfpk.GetSize(); part++)
			{
				for( int action = 0; action < cfpk[part].GetSize(); action++)
					for( int direction = 0; direction < cfpk[part][action].GetSize(); direction++)
						for( int f = 0; f < cfpk[part][action][direction].GetSize(); f++)
						{
							int x = cfpk[part][action][direction][f].GetCX();
							int y = cfpk[part][action][direction][f].GetCY();
							int frameid = cfpk[part][action][direction][f].GetSpriteID();
							cfpk[part][action][direction][f].Set( frameid, x - xOffset, y - yOffset);
						}
			}
			packFile.close();
	//		indexFile.close();

		   // Save CFPK
			ofstream packFile1( dlg.GetFileTitle()+ ".cfpk", ios::binary);
			ofstream indexFile( dlg.GetFileTitle() + ".cfpki", ios::binary);

			cfpk.SaveToFile(packFile1, indexFile);
			packFile.close();
			indexFile.close();

		}

	}

//	cfpk[part][action][direction][t].Set( frame.sprite_id, frame.rx, frame.ry);
	
}
*/
void CSpriteManagerDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
//	OnTemp();
	
}


int CSpriteManagerDlg::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog::OnVKeyToItem(nKey, pListBox, nIndex);
}

LRESULT CSpriteManagerDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( message == WM_KEYDOWN)
	{
		message = WM_KEYDOWN;
	}
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CSpriteManagerDlg::OnButtonUndo() 
{
	if( m_undo.CanUndo())
		m_undo.Undo( m_myData);

//	GetDlgItem( IDC_BUTTON_UNDO)->EnableWindow( m_undo.CanUndo());
//	GetDlgItem( IDC_BUTTON_REDO)->EnableWindow( m_undo.CanRedo());

	m_AniList.ResetContent();
	CDib* pSrcDib;

	for(int i = 0; i < m_myData.m_iAniLength; i++)
	{
		pSrcDib = m_myData.m_aniList.GetAt2( m_myData.m_iAniIndex[i] );

		char strID[256];
		const char* szPathName = pSrcDib->GetFileName();
		const char* szFileName = strrchr( szPathName, '\\');
		if( szFileName == NULL)
			szFileName = szPathName;
		else
			szFileName++;
			
		sprintf( strID, "%d-%s", m_myData.m_iAniIndex[i], szFileName);

		m_AniList.AddString(strID);
	}

	Invalidate(FALSE);
}

void CSpriteManagerDlg::OnButtonRedo() 
{
	if( m_undo.CanRedo())
		m_undo.Redo( m_myData);

//	GetDlgItem( IDC_BUTTON_UNDO)->EnableWindow( m_undo.CanUndo());
//	GetDlgItem( IDC_BUTTON_REDO)->EnableWindow( m_undo.CanRedo());

	m_AniList.ResetContent();
	CDib* pSrcDib;

	for(int i = 0; i < m_myData.m_iAniLength; i++)
	{
		pSrcDib = m_myData.m_aniList.GetAt2( m_myData.m_iAniIndex[i] );

		char strID[256];
		const char* szPathName = pSrcDib->GetFileName();
		const char* szFileName = strrchr( szPathName, '\\');
		if( szFileName == NULL)
			szFileName = szPathName;
		else
			szFileName++;
			
		sprintf( strID, "%d-%s", m_myData.m_iAniIndex[i], szFileName);

		m_AniList.AddString(strID);
	}

	Invalidate(FALSE);
}

void CSpriteManagerDlg::OnCopy() 
{
	if( m_iSelectedAniID != -1 
		&& m_iSelectedAniID < m_myData.m_iAniLength)
	{
		m_iCopiedIndex = m_iSelectedAniID;
		m_bCopied = TRUE;
	}
	
}

void CSpriteManagerDlg::OnPaste() 
{
	if( m_iCopiedIndex != -1 && m_bCopied)
	{
		CDib* pSrcDib = m_myData.m_aniList.GetAt2( m_myData.m_iAniIndex[ m_iCopiedIndex]);

		m_undo.SaveState( m_myData);

		// add
		AddToAni( pSrcDib->GetFileName(), m_iSelectedAniID, false);
	}
}

void CSpriteManagerDlg::OnSelchangeAnimationList() 
{
	// TODO: Add your control notification handler code here
	int sel = m_AniList.GetCurSel();
	m_aniSlider.SetPos( sel );
	InvalidateAni();
	m_iSelectedAniID = sel;
}

#define MAX_ITEM 200

void CSpriteManagerDlg::OnListup() 
{
	m_undo.SaveState( m_myData);
	m_bUndoStop = true;

	int items[MAX_ITEM];
	int items_after[MAX_ITEM];
	ZeroMemory(items, MAX_ITEM);
	ZeroMemory(items_after, MAX_ITEM);

	int i, select_item_count, item_count;

	CDib* pSrcDib;

	select_item_count = m_AniList.GetSelItems( MAX_ITEM, items );
	item_count = m_AniList.GetCount();

	for(i = 0; i < select_item_count; i++)
	{
		if(items[i] != 0 && !(i != 0 && items[i-1] == items_after[i-1] && items[i]-1 == items[i-1]))
		{
			pSrcDib = m_myData.m_aniList.GetAt2( m_myData.m_iAniIndex[ items[i]]);
			m_iSelectedAniID = items[i];
			OnMenuDeleteAni();
			AddToAni( pSrcDib->GetFileName(), items[i]-1, false);
			items_after[i] = items[i]-1;
		}else items_after[i] = items[i];
	}

	for(i = 0; i < select_item_count; i++)
	{
		m_AniList.SetSel(items_after[i]);
	}
	m_bUndoStop = false;

	OnSelchangeAnimationList();
}

void CSpriteManagerDlg::OnListdown() 
{
	m_undo.SaveState( m_myData);
	m_bUndoStop = true;

	int items[MAX_ITEM];
	int items_after[MAX_ITEM];
	ZeroMemory(items, MAX_ITEM);
	ZeroMemory(items_after, MAX_ITEM);

	int i, select_item_count, item_count;

	CDib* pSrcDib;

	select_item_count = m_AniList.GetSelItems( MAX_ITEM, items );
	item_count = m_AniList.GetCount();

	for(i = select_item_count-1; i >= 0; i--)
	{
		if(items[i] +1 != item_count && !(i != select_item_count-1 && items[i+1] == items_after[i+1] && items[i]+1 == items[i+1]))
		{
			pSrcDib = m_myData.m_aniList.GetAt2( m_myData.m_iAniIndex[ items[i]]);
			m_iSelectedAniID = items[i];
			OnMenuDeleteAni();
			AddToAni( pSrcDib->GetFileName(), items[i]+1, false);
			items_after[i] = items[i]+1;
		}else items_after[i] = items[i];
	}

	for(i = 0; i < select_item_count; i++)
	{
		m_AniList.SetSel(items_after[i]);
	}
	m_bUndoStop = false;

	OnSelchangeAnimationList();

}

void CSpriteManagerDlg::OnListdelete() 
{
	m_undo.SaveState( m_myData);
	m_bUndoStop = true;
	// TODO: Add your control notification handler code here
	int items[MAX_ITEM];
	int len_item;

	len_item = m_AniList.GetSelItems( MAX_ITEM, items );

	for(int i = len_item-1; i >= 0; i--)
	{
		m_iSelectedAniID = items[i];
		OnMenuDeleteAni();
	}

	m_bUndoStop = false;
}

void CSpriteManagerDlg::OnListcopy() 
{
	// TODO: Add your control notification handler code here
	m_szCopyData.clear();
	CString szTemp;
	
	int items[MAX_ITEM];
	ZeroMemory(items, MAX_ITEM);

	int i, select_item_count;

	CDib* pSrcDib;

	select_item_count = m_AniList.GetSelItems( MAX_ITEM, items );

	for(i = select_item_count -1; i >= 0; i--)
	{
		pSrcDib = m_myData.m_aniList.GetAt2( m_myData.m_iAniIndex[ items[i]]);
		szTemp = pSrcDib->GetFileName();
		m_szCopyData.push_back(szTemp);
	}

}

void CSpriteManagerDlg::OnListpaste() 
{
	// TODO: Add your control notification handler code here
	if(m_szCopyData.empty())return;

	int items[MAX_ITEM];
	ZeroMemory(items, MAX_ITEM);

	int i;

	m_AniList.SelItemRange( false, 0, m_AniList.GetCount()-1 );

	m_undo.SaveState( m_myData);

	int count = m_szCopyData.size();
	for(i = 0; i < count; i++)
		AddToAni( m_szCopyData[i], m_iSelectedAniID, false);

	for(i = 0; i < count; i++)
		m_AniList.SetSel(m_iSelectedAniID+i);

	OnSelchangeAnimationList();

}

void CSpriteManagerDlg::OnListcut() 
{
	// TODO: Add your control notification handler code here
	m_szCopyData.clear();
	CString szTemp;
	
	int items[MAX_ITEM];
	ZeroMemory(items, MAX_ITEM);

	int i, select_item_count;

	CDib* pSrcDib;

	select_item_count = m_AniList.GetSelItems( MAX_ITEM, items );

	m_undo.SaveState( m_myData);
	m_bUndoStop = true;

	for(i = select_item_count -1; i >= 0; i--)
	{
		pSrcDib = m_myData.m_aniList.GetAt2( m_myData.m_iAniIndex[ items[i]]);
		szTemp = pSrcDib->GetFileName();
		
		m_szCopyData.push_back(szTemp);
		
		m_iSelectedAniID = items[i];
		OnMenuDeleteAni();
	}
	m_bUndoStop = false;
	
}


void CSpriteManagerDlg::OnListadd() 
{
	// TODO: Add your control notification handler code here
	int items[MAX_ITEM];
	ZeroMemory(items, MAX_ITEM);

	m_undo.SaveState( m_myData);

	char szFilter[] = "BMP파일(*.bmp)|*.bmp|모든파일(*.*)|*.*||";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFilter);
	if(dlg.DoModal() == IDOK)
	{

	AddToAni( dlg.GetPathName(), m_AniList.GetCount(), FALSE);

	m_AniList.SetSel(m_AniList.GetCount()-1);

	OnSelchangeAnimationList();
	}
	
}
