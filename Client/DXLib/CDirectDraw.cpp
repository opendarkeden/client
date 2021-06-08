//----------------------------------------------------------------------
// CDirectDraw.cpp
//----------------------------------------------------------------------

#include "CDirectDraw.h"

/*-----------------------------------------------------------------------------
  GLOBALS
-----------------------------------------------------------------------------*/
//----------------------------------------------------------------------
//
// init Static members
//
//----------------------------------------------------------------------
LPDIRECTDRAW7					CDirectDraw::m_pDD					= NULL;
LPDIRECTDRAWSURFACE7			CDirectDraw::m_pDDSPrimary			= NULL;
LPDIRECTDRAWSURFACE7			CDirectDraw::m_pDDSBack				= NULL;
DDSURFACEDESC2					CDirectDraw::m_ddsd;

LPDIRECTDRAWGAMMACONTROL	CDirectDraw::m_pDDGammaControl	= NULL;

HWND								CDirectDraw::m_hWnd					= NULL;
//bool							CDirectDraw::m_bActive				= false;

bool								CDirectDraw::m_bFullscreen			= true;
WORD								CDirectDraw::m_ScreenWidth			= 0;
WORD								CDirectDraw::m_ScreenHeight			= 0;
bool								CDirectDraw::m_b565					= true;
bool								CDirectDraw::m_b3D					= true;
bool								CDirectDraw::m_bMMX					= false;
bool								CDirectDraw::m_bGammaControl		= false;
DDGAMMARAMP							CDirectDraw::m_DDGammaRamp;
WORD								CDirectDraw::m_GammaStep = 100;
WORD								CDirectDraw::m_AddGammaStep[3] = {0, 0, 0};

RECT								CDirectDraw::m_rcWindow;
RECT								CDirectDraw::m_rcScreen;
RECT								CDirectDraw::m_rcViewport;

// Mask
WORD								CDirectDraw::s_wMASK_SHIFT[5];
DWORD								CDirectDraw::s_dwMASK_SHIFT[5];
QWORD								CDirectDraw::s_qwMASK_SHIFT[5];

WORD								CDirectDraw::s_wMASK_RGB[6];
DWORD								CDirectDraw::s_dwMASK_RGB[6];
QWORD								CDirectDraw::s_qwMASK_RGB[6];

QWORD								CDirectDraw::s_qwMASK_ALPHA0;
QWORD								CDirectDraw::s_qwMASK_ALPHA1;
DWORD								CDirectDraw::s_dwMASK_ALPHA0;
DWORD								CDirectDraw::s_dwMASK_ALPHA1;
WORD								CDirectDraw::s_wMASK_ALPHA0;
WORD								CDirectDraw::s_wMASK_ALPHA1;

BYTE								CDirectDraw::s_bSHIFT_R; 
BYTE								CDirectDraw::s_bSHIFT_G;
BYTE								CDirectDraw::s_bSHIFT_B;

// 1 Byte ShiftCount for 4444
BYTE								CDirectDraw::s_bSHIFT4_R; 
BYTE								CDirectDraw::s_bSHIFT4_G;
BYTE								CDirectDraw::s_bSHIFT4_B;

// 기본 색상
WORD								CDirectDraw::RED	= 0xF800;
WORD								CDirectDraw::GREEN	= 0x07D0;
WORD								CDirectDraw::BLUE	= 0x001F;
WORD								CDirectDraw::WHITE	= 0xFFFF;

bool								CDirectDraw::s_bUseIMEHandle = false;


//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
CDirectDraw::CDirectDraw()
{
	// ~empty~
}

CDirectDraw::~CDirectDraw()
{
	//ReleaseAll(); // 외부에서 해줘야 한다.
}

/*-----------------------------------------------------------------------------
- Init
- DirectDraw를 초기화한다.

  <인자 설명>
  HWND hWnd									= window handle
  WORD wWidth, WORD wHeight			= surface size
  enum CDirectDraw::SCREENMODE mode = screen mode option
-----------------------------------------------------------------------------*/
bool CDirectDraw::Init(HWND hWnd, WORD wWidth, WORD wHeight, enum CDirectDraw::SCREENMODE mode, bool b3D, bool bUseIMEWindow)
{
	m_hWnd	= hWnd;

	// DX Object Create
	if (DirectDrawCreateEx(NULL, (VOID**)&m_pDD, IID_IDirectDraw7, NULL) != DD_OK)
	{
		DirectDrawFailed("DirectDrawCreate Error!");
		return false;
	}

	// 3D Primary Surface?
	m_b3D = b3D;

	s_bUseIMEHandle = bUseIMEWindow;

	bool bOK;
	//-------------------------------------
	//
	// Full Screen Mode
	//
	//-------------------------------------
	if (mode == FULLSCREEN)
	{
		bOK = InitFullscreen(wWidth, wHeight);
	}

	//-------------------------------------
	//
	// Window Mode
	//
	//-------------------------------------
	else
	{
		bOK = InitWindowMode(wWidth, wHeight);
	}

	if (bOK)
	{
		//-------------------------------------
		// check for Support MMX?
		//-------------------------------------
		m_bMMX			= CheckMMX();

		//-------------------------------------
		// check for Support Gamma Control?
		//-------------------------------------
		m_bGammaControl = CheckGammaControl();

		//-------------------------------------
		// check for VideoCard 5:6:5  or 5:5:5?
		//-------------------------------------
		m_b565			= Check565();

		// 5:6:5  or 5:5:5에 맞는 mask를 setting한다.
		InitMask(m_b565);
	}

	return bOK;
}

//----------------------------------------------------------------------
// DirectDraw Initialization
//----------------------------------------------------------------------
bool CDirectDraw::InitFullscreen(WORD wWidth, WORD wHeight)
{
	DDSURFACEDESC2	ddsd;
	DDSCAPS2		ddscaps;	

	//------------------------------------------------------
	// Get exclusive mode
	//------------------------------------------------------
	if (m_pDD->SetCooperativeLevel(m_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN) != DD_OK)
	{
		DirectDrawFailed("SetCooperativeLevel FAILED");
		return false;	
	}


	//------------------------------------------------------
	// Set the video mode to  ( dwWidth x dwHeight x bpp )
	// default bpp = 16
	//------------------------------------------------------
	if (m_pDD->SetDisplayMode(wWidth, wHeight, 16, 0, 0) != DD_OK)
	{
		DirectDrawFailed("SetDisplayMode FAILED");
		return false;	
	}

	//------------------------------------------------------
	// Create the primary surface with 1 back buffer
	//------------------------------------------------------
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
						   DDSCAPS_FLIP | //DDSCAPS_VIDEOMEMORY |
						   DDSCAPS_COMPLEX;

	//------------------------------------------------------
	// D3D를 사용한다면..
	//------------------------------------------------------
	if (m_b3D)
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_3DDEVICE;
		ddsd.dwBackBufferCount = 2;
	}
	else
	{
	    ddsd.dwBackBufferCount = 1;
	}

	if (m_pDD->CreateSurface(&ddsd, &m_pDDSPrimary, NULL) != DD_OK )
	{
		DirectDrawFailed("CreateSurface FAILED");
		return false;
	}

	//------------------------------------------------------
	// Get a pointer to the back buffer	
	//------------------------------------------------------
	ZeroMemory(&ddscaps, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	if (m_pDDSPrimary->GetAttachedSurface(&ddscaps, &m_pDDSBack)!=DD_OK)
	{
		DirectDrawFailed("GetAttachedSurface FAILED");
		return false;	
	}

	if(s_bUseIMEHandle)
	{
		m_pDDSBack->AddRef();
		
		DDCAPS ddcaps;
		ZeroMemory(&ddcaps,sizeof(DDCAPS));
		m_pDD->GetCaps(&ddcaps,NULL);
		/*if( (ddcaps.dwCaps2 & DDCAPS2_CANRENDERWINDOWED) == 0)
		{
			DirectDrawFailed("This display card can not render GDI.");
			return false;
		}*/
		//------------------------------------------------------
		// Create the window clipper
		//------------------------------------------------------
		LPDIRECTDRAWCLIPPER		pDDClipper;	// DirectDraw Clipper
		if ( m_pDD->CreateClipper(0, &pDDClipper, NULL) != DD_OK )
		{
			DirectDrawFailed("Clipper Create Error!");
			return false;
		}
		
		if ( pDDClipper->SetHWnd(0, m_hWnd) != DD_OK )
		{
			DirectDrawFailed("Clipper Setting Error! - Window -");
			return false;
		}
		
		if ( m_pDDSPrimary->SetClipper(pDDClipper) != DD_OK )
		{
			DirectDrawFailed("Clipper Setting Error! - Primary Surface -");
			return false;
		}
		
		pDDClipper->Release();
		pDDClipper = NULL;
	}	

	//------------------------------------------------------
	// member init..
	//------------------------------------------------------
	m_bFullscreen	= true;

	m_ScreenWidth			= wWidth;
	m_ScreenHeight			= wHeight;

   return true;
}

//----------------------------------------------------------------------
// WindowMode DirectDraw Initialization
//----------------------------------------------------------------------
bool CDirectDraw::InitWindowMode(WORD wWidth, WORD wHeight)
{
	// Get the dimensions of the viewport and screen bounds
	// Store the rectangle which contains the renderer
	//SetRect(&m_rcViewport, 0, 0, wWidth, wHeight );
	//memcpy(&m_rcScreen, &m_rcViewport, sizeof(m_rcScreen) );	

	// Save the window size/pos for switching modes
	GetWindowRect(m_hWnd, &m_rcWindow);

	DDSURFACEDESC2	ddsd;
	LPDIRECTDRAWCLIPPER		pDDClipper;	// DirectDraw Clipper

	if (m_pDD->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL) != DD_OK)
	{
		DirectDrawFailed("CooperativeLevel mode Error!");
		return false;
	}

	//------------------------------------------------------
	// Get the dimensions of the viewport and screen bounds
	//------------------------------------------------------
   	GetClientRect(m_hWnd, &m_rcViewport);
   	GetClientRect(m_hWnd, &m_rcScreen);
   	ClientToScreen(m_hWnd, (POINT*)&m_rcScreen.left);
   	ClientToScreen(m_hWnd, (POINT*)&m_rcScreen.right);


	//------------------------------------------------------
	// Create Primary Surface
	//------------------------------------------------------
	::ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize			= sizeof(ddsd);
	ddsd.dwFlags		= DDSD_CAPS;
	ddsd.ddsCaps.dwCaps	= DDSCAPS_PRIMARYSURFACE;	

	if (m_pDD->CreateSurface(&ddsd, &m_pDDSPrimary, NULL) != DD_OK )
	{
		DirectDrawFailed("Create PrimarySurface Error!");
		return false;
	}

	//------------------------------------------------------
	// Create Back Surface
	//------------------------------------------------------
   ::ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize			= sizeof(ddsd);
	ddsd.dwFlags		= DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
	ddsd.dwWidth		= wWidth;
	ddsd.dwHeight		= wHeight;

	//------------------------------------------------------
	// D3D를 사용한다면..
	//------------------------------------------------------
	if (m_b3D)
	{		
		ddsd.ddsCaps.dwCaps |= DDSCAPS_3DDEVICE;		
	}

	if (m_pDD->CreateSurface(&ddsd, &m_pDDSBack, NULL) != DD_OK )
	{
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
		if (m_pDD->CreateSurface(&ddsd, &m_pDDSBack, NULL) != DD_OK )
		{
			DirectDrawFailed("Create BackSurface Error!");
			return false;
		}
	}

	//------------------------------------------------------
	// Create the window clipper
	//------------------------------------------------------
	if ( m_pDD->CreateClipper(0, &pDDClipper, NULL) != DD_OK )
	{
		DirectDrawFailed("Clipper Create Error!");
		return false;
	}

	if ( pDDClipper->SetHWnd(0, m_hWnd) != DD_OK )
	{
		DirectDrawFailed("Clipper Setting Error! - Window -");
		return false;
	}

	if ( m_pDDSPrimary->SetClipper(pDDClipper) != DD_OK )
	{
		DirectDrawFailed("Clipper Setting Error! - Primary Surface -");
		return false;
	}

	pDDClipper->Release();
	pDDClipper = NULL;

	//------------------------------------------------------
	// data member init...
	//------------------------------------------------------
	m_bFullscreen	= false;

	m_ScreenWidth			= wWidth;
	m_ScreenHeight			= wHeight;

	return true;
}

//----------------------------------------------------------------------
// Release Primary & Back Surface
//----------------------------------------------------------------------
void CDirectDraw::ReleaseSurface()
{
    if (m_pDD != NULL)
    {
		m_pDD->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);

		if (m_pDDSBack != NULL)
        {
            m_pDDSBack->Release();
            m_pDDSBack = NULL;
        }

        if (m_pDDSPrimary != NULL)
        {
            m_pDDSPrimary->Release();
            m_pDDSPrimary = NULL;
        }
    }
}

//----------------------------------------------------------------------
// Release All
//----------------------------------------------------------------------
void CDirectDraw::ReleaseAll()
{	
	if (m_pDDGammaControl!=NULL)
	{
		m_pDDGammaControl->SetGammaRamp(0, &m_DDGammaRamp);	
		m_pDDGammaControl->Release();
		m_pDDGammaControl = NULL;
	}

	// Release Primary & Back Surface
	ReleaseSurface();	

	if (m_pDD != NULL)
	{
		m_pDD->Release();
		m_pDD = NULL;
	}
}

//----------------------------------------------------------------------
// Restore All Surfaces
//----------------------------------------------------------------------
bool CDirectDraw::RestoreAllSurfaces()
{
	//
	// IDirectDraw7::RestoreAllSurfaces()
	// `생성된 모든 표면을 Restore한다. offscreen까지도 하는 지는 확인하지 않았다.
	//
	if (m_pDD != NULL)
	{
		HRESULT hr = m_pDD->RestoreAllSurfaces();

		m_pDDSPrimary->Restore();
		m_pDDSBack->Restore();

		switch (hr)
		{
			case DD_OK :
				return true;

			case DDERR_INVALIDOBJECT :
				DirectDrawFailed("Restore All Surfaces Error! - InvalidObject");

			case DDERR_INVALIDPARAMS :
				DirectDrawFailed("Restore All Surfaces Error! - InvalidParams");
		}
	}

	return false;
}

//----------------------------------------------------------------------
// Flip Surface : Back --> Primary
//----------------------------------------------------------------------
void CDirectDraw::Flip()
{
	HRESULT hRet;

	while (1)
	{
		//-------------------------------------------------------
		// Fullscreen Mode를 사용하고 있을 경우
		//-------------------------------------------------------
		if (m_bFullscreen)
		{
			if( s_bUseIMEHandle )
				hRet = m_pDDSPrimary->Blt( NULL, m_pDDSBack, NULL, DDBLT_WAIT, NULL );
			else
				hRet = m_pDDSPrimary->Flip(NULL, DDFLIP_WAIT);
		}

		//-------------------------------------------------------
		// Window Mode를 사용하고 있을 경우
		//-------------------------------------------------------
		else
		{			
			hRet = m_pDDSPrimary->Blt(&m_rcScreen, m_pDDSBack, &m_rcViewport, DDBLT_WAIT, NULL);
		}

		if (hRet == DD_OK)
			break;

		//-------------------------------------------------------
		// Lost 된 Surface 복구
		//-------------------------------------------------------	
		if (hRet == DDERR_SURFACELOST)
		{
			RestoreAllSurfaces();
		}

		if (hRet != DDERR_WASSTILLDRAWING)
			break;

	}
}

//----------------------------------------------------------------------
// Flip to GDI Surface
//----------------------------------------------------------------------
void CDirectDraw::FlipToGDISurface()
{
	m_pDD->FlipToGDISurface();
}

//----------------------------------------------------------------------
// Init Mask
//----------------------------------------------------------------------
// Lock을 사용해 Draw를 할때, 필요한 Mask변수들을 초기화한다.
//----------------------------------------------------------------------
void CDirectDraw::InitMask(bool b565)
{
 	//---------------------------------------------
	// 5:6:5
	//---------------------------------------------
	if (b565)
	{	
		//---------------------------------------------
		// Shift Mask
		//---------------------------------------------
		s_qwMASK_SHIFT[0]	= 0xFFFFFFFFFFFFFFFF;
		s_dwMASK_SHIFT[0]	= 0xFFFFFFFF;
		s_wMASK_SHIFT[0]	= 0xFFFF;

		// 01111 011111 01111
		// 0111 1011 1110 1111
		//    7    B    E    F
		s_qwMASK_SHIFT[1]	= 0x7BEF7BEF7BEF7BEF;
		s_dwMASK_SHIFT[1]	= 0x7BEF7BEF;
		s_wMASK_SHIFT[1]	= 0x7BEF;
		
		// 00111 001111 00111
		// 0011 1001 1110 0111
		//    3    9    E    7
		s_qwMASK_SHIFT[2]	= 0x39E739E739E739E7;
		s_dwMASK_SHIFT[2]	= 0x39E739E7;
		s_wMASK_SHIFT[2]	= 0x39E7;
		
		// 00011 000111 00011
		// 0001 1000 1110 0011
		//    1    8    E    3
		s_qwMASK_SHIFT[3]	= 0x18E318E318E318E3;
		s_dwMASK_SHIFT[3]	= 0x18E318E3;
		s_wMASK_SHIFT[3]	= 0x18E3;
		
		// 00001 000011 00001
		// 0000 1000 0110 0001
		//    0    8    6    1
		s_qwMASK_SHIFT[4]	= 0x0861086108610861;
		s_dwMASK_SHIFT[4]	= 0x08610861;
		s_wMASK_SHIFT[4]	= 0x0861;

		//---------------------------------------------
		// RGB Mask
		//---------------------------------------------
		s_qwMASK_RGB[0]	= 0xF800F800F800F800;	// R
		s_dwMASK_RGB[0]	= 0xF800F800;
		s_wMASK_RGB[0]	= 0xF800;

		s_qwMASK_RGB[1]	= 0x07E007E007E007E0;	// G
		s_dwMASK_RGB[1]	= 0x07E007E0;
		s_wMASK_RGB[1]	= 0x07E0;

		s_qwMASK_RGB[2]	= 0x001F001F001F001F;	// B
		s_dwMASK_RGB[2]	= 0x001F001F;
		s_wMASK_RGB[2]	= 0x001F;

		s_qwMASK_RGB[3]	= 0xFFE0FFE0FFE0FFE0;	// RG
		s_dwMASK_RGB[3]	= 0xFFE0FFE0;
		s_wMASK_RGB[3]	= 0xFFE0;

		s_qwMASK_RGB[4]	= 0xF81FF81FF81FF81F;	// RB
		s_dwMASK_RGB[4]	= 0xF81FF81F;
		s_wMASK_RGB[4]	= 0xF81F;

		s_qwMASK_RGB[5]	= 0x07FF07FF07FF07FF;	// GB
		s_dwMASK_RGB[5]	= 0x07FF07FF;
		s_wMASK_RGB[5]	= 0x07FF;

		

		//---------------------------------------------
		// ALPHA Mask
		//---------------------------------------------
		// 1111 1000 0001 1111 
		// 0000 0111 1110 0000
		s_qwMASK_ALPHA0 = 0xF81F07E0F81F07E0;
		s_qwMASK_ALPHA1 = 0x07E0F81F07E0F81F;
		s_dwMASK_ALPHA0 = 0xF81F07E0;
		s_dwMASK_ALPHA1 = 0x07E0F81F;
		s_wMASK_ALPHA0	= 0xF81F;
		s_wMASK_ALPHA1	= 0x07E0;

		//---------------------------------------------
		// 1 Byte RGB ShiftCount & Mask
		//---------------------------------------------
		s_bSHIFT_R		= 11; 		
		s_bSHIFT_G		= 6; 		
		s_bSHIFT_B		= 0; 		

		//---------------------------------------------
		// 1 Byte RGB ShiftCount & Mask for 4444
		//---------------------------------------------
		s_bSHIFT4_R		= 12; 		
		s_bSHIFT4_G		= 7; 		
		s_bSHIFT4_B		= 1; 		

		//---------------------------------------------
		// 기본 색상
		//---------------------------------------------
		RED				= 0xF800;
		GREEN			= 0x07E0;
		BLUE			= 0x001F;
		WHITE			= 0xFFFF;
	}		
		
	//---------------------------------------------
	// 5:5:5
	//---------------------------------------------
	else
	{	
		//---------------------------------------------
		// Shift Mask
		//---------------------------------------------
		s_qwMASK_SHIFT[0]	= 0x7FFF7FFF7FFF7FFF;
		s_dwMASK_SHIFT[0]	= 0x7FFF7FFF;
		s_wMASK_SHIFT[0]	= 0x7FFF;

		// 0 01111 01111 01111
		// 0011 1101 1110 1111
		//    3    D    E    F
		s_qwMASK_SHIFT[1]	= 0x3DEF3DEF3DEF3DEF;
		s_dwMASK_SHIFT[1]	= 0x3DEF3DEF;
		s_wMASK_SHIFT[1]	= 0x3DEF;
		
		// 0 00111 00111 00111
		// 0001 1100 1110 0111
		//    1    C    E    7
		s_qwMASK_SHIFT[2]	= 0x1CE71CE71CE71CE7;
		s_dwMASK_SHIFT[2]	= 0x1CE71CE7;
		s_wMASK_SHIFT[2]	= 0x1CE7;
		
		// 0 00011 00011 00011
		// 0000 1100 0110 0011
		//    0    C    6    3
		s_qwMASK_SHIFT[3]	= 0x0C630C630C630C63;
		s_dwMASK_SHIFT[3]	= 0x0C630C63;
		s_wMASK_SHIFT[3]	= 0x0C63;
		
		// 0 00001 00001 00001
		// 0000 0100 0010 0001
		//    0    4    2    1
		s_qwMASK_SHIFT[4]	= 0x0421042104210421;
		s_dwMASK_SHIFT[4]	= 0x04210421;
		s_wMASK_SHIFT[4]	= 0x0421;	

		//---------------------------------------------
		// RGB Mask
		//---------------------------------------------
		s_qwMASK_RGB[0]	= 0x7C007C007C007C00;	// R
		s_dwMASK_RGB[0]	= 0x7C007C00;
		s_wMASK_RGB[0]	= 0x7C00;

		s_qwMASK_RGB[1]	= 0x03E003E003E003E0;	// G
		s_dwMASK_RGB[1]	= 0x03E003E0;
		s_wMASK_RGB[1]	= 0x03E0;

		s_qwMASK_RGB[2]	= 0x001F001F001F001F;	// B
		s_dwMASK_RGB[2]	= 0x001F001F;
		s_wMASK_RGB[2]	= 0x001F;

		s_qwMASK_RGB[3]	= 0x7FE07FE07FE07FE0;	// RG
		s_dwMASK_RGB[3]	= 0x7FE07FE0;
		s_wMASK_RGB[3]	= 0x7FE0;

		s_qwMASK_RGB[4]	= 0x7C1F7C1F7C1F7C1F;	// RB
		s_dwMASK_RGB[4]	= 0x7C1F7C1F;
		s_wMASK_RGB[4]	= 0x7C1F;

		s_qwMASK_RGB[5]	= 0x03FF03FF03FF03FF;	// GB
		s_dwMASK_RGB[5]	= 0x03FF03FF;
		s_wMASK_RGB[5]	= 0x03FF;

		//---------------------------------------------
		// ALPHA Mask
		//---------------------------------------------
		s_qwMASK_ALPHA0 = 0x7C1F03E07C1F03E0;
		s_qwMASK_ALPHA1 = 0x03E07C1F03E07C1F;
		s_dwMASK_ALPHA0 = 0x7C1F03E0;
		s_dwMASK_ALPHA1 = 0x03E07C1F;
		s_wMASK_ALPHA0	= 0x7C1F;
		s_wMASK_ALPHA1	= 0x03E0;

		//---------------------------------------------
		// 1 Byte RGB ShiftCount & Mask
		//---------------------------------------------
		s_bSHIFT_R		= 10; 		
		s_bSHIFT_G		= 5; 		
		s_bSHIFT_B		= 0; 

		//---------------------------------------------
		// 1 Byte RGB ShiftCount & Mask for 4444
		//---------------------------------------------
		s_bSHIFT4_R		= 11; 		
		s_bSHIFT4_G		= 6;		
		s_bSHIFT4_B		= 1;	
		
		//---------------------------------------------
		// 기본 색상
		//---------------------------------------------
		RED				= 0x7C00;
		GREEN			= 0x03E0;
		BLUE			= 0x001F;
		WHITE			= 0x7FFF;
	} 
}

//----------------------------------------------------------------------
// Gamma Control을 지원하는지에 대한 check
//
// `FullMode시 잘못된 화면크기 지정시에 대응하는 예외처리가 안됨.
//----------------------------------------------------------------------
bool CDirectDraw::CheckGammaControl()
{
	if (m_pDDGammaControl!=NULL)
	{
		return true;
	}

	DDCAPS	driver;

	::ZeroMemory(&driver, sizeof(driver));
	driver.dwSize = sizeof(driver);

	// DirectDraw의 Cap..을 읽어온다.
	if ( m_pDD->GetCaps(&driver, NULL) != DD_OK )
	{
		DirectDrawFailed("GetCaps() Error");		
	}

	// Hardware Support?
	if (driver.dwCaps2 & DDCAPS2_PRIMARYGAMMA)
	{		
		// DX Object Interface Query
		if (FAILED( m_pDDSPrimary->QueryInterface(IID_IDirectDrawGammaControl, (LPVOID *)&m_pDDGammaControl)) )
		{
			DirectDrawFailed("Gamma Control QueryInterface FAILED!");
			return false;
		}

		// 현재의 GammaRamp를 저장한다.
		m_pDDGammaControl->GetGammaRamp(0, &m_DDGammaRamp);

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// GammaRamp 값을 원래의 것으로 바꾼다.
//----------------------------------------------------------------------
// test용 함수
//----------------------------------------------------------------------
void CDirectDraw::RestoreGammaRamp()
{	
	if (m_pDDGammaControl!=NULL)
	{
		m_pDDGammaControl->SetGammaRamp(0, &m_DDGammaRamp);	
	}
}

//----------------------------------------------------------------------
// GammaRamp 값을 바꾼다.
//----------------------------------------------------------------------
// 0(어두움) ~ 100(현재) ~ 200(밝음)
// 테스트 해본 결과...
// 50 ~ 150 정도 사이의 값만 쓰는게 좋다고 생각된다
//----------------------------------------------------------------------
void CDirectDraw::SetGammaRamp(WORD step)
{
	if(step == 0xffff)
		step = m_GammaStep;
	else
		m_GammaStep = step;

	if (m_pDDGammaControl!=NULL)
	{
		DDGAMMARAMP	ddgr;

		ZeroMemory(&ddgr, sizeof(ddgr));

		int maxValue[3];
		int stepValue[3];

		for(int i = 0; i < 3; i++)
		{
			WORD addGammaStep = max(-100, min(100, m_AddGammaStep[i]));
			if(m_AddGammaStep[i] > 0)
				step = m_GammaStep + (200 - m_GammaStep)*addGammaStep/100;
			else
				step = m_GammaStep + m_GammaStep*addGammaStep/100;
			
			if(step<100)
			{
				maxValue[i] = 0;
				stepValue[i] = 100 - step;
			}
			else
			{
				maxValue[i] = 0xFFFF;
				stepValue[i] = step - 100;
			}
		}

		WORD r, g, b;
		 
		for (i=0; i<256; i++)
		{
			r = m_DDGammaRamp.red[i];
			g = m_DDGammaRamp.green[i];
			b = m_DDGammaRamp.blue[i];

			ddgr.red[i] = (WORD)(r + (maxValue[0]-r)*stepValue[0]/100);
			ddgr.green[i] = (WORD)(g + (maxValue[1]-g)*stepValue[1]/100);
			ddgr.blue[i] = (WORD)(b + (maxValue[2]-b)*stepValue[2]/100);
		}	

		m_pDDGammaControl->SetGammaRamp(0, &ddgr);
	}
}

void	CDirectDraw::SetAddGammaRamp(WORD rStep, WORD gStep, WORD bStep)
{
	m_AddGammaStep[0] = rStep;
	m_AddGammaStep[1] = gStep;
	m_AddGammaStep[2] = bStep;

	SetGammaRamp();
}

//----------------------------------------------------------------------
// Check 5:5:5 or 5:6:5?
//----------------------------------------------------------------------
// 임의의 Surface를 하나 만들어서 
// 한 점을 찍어보고 5:5:5인지 5:6:5인지 판단한다.
//----------------------------------------------------------------------
bool CDirectDraw::Check565()
{
	/*
	LPDIRECTDRAWSURFACE7	pDDSurface = NULL;
	DDSURFACEDESC2			ddsd;	
	
	// Surface에 대한 정보 설정
	// m_pSurface를 OffScreen으로 설정한다.
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;// | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth  = 1;
	ddsd.dwHeight = 1;
	
	if ( m_pDD->CreateSurface( &ddsd, &pDDSurface, NULL )  !=  DD_OK )	
	{ 
		DirectDrawFailed("DD-Create OffSurface for Check5:6:5 Error!"); 
		return FALSE; 
	}

	// check for 16bit 5:6:5 or 5:5:5
	HDC hDC;	

	// (0,0)에  한 점을 출력한다.
	pDDSurface->GetDC(&hDC);
	SetPixel(hDC,0,0,RGB(255,255,255));
	pDDSurface->ReleaseDC(hDC);

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	// Lock을 걸고 출력한 점을 읽어온다.
	pDDSurface->Lock(NULL, &ddsd, 0, NULL);
	*/
	/*
	HRESULT hr;
	while (1)
	{
		hr = pDDSurface->Lock(NULL, &ddsd, 0, NULL);
		if( hr == DD_OK )
		{
			break;
		}
		if( hr == DDERR_SURFACELOST )
		{
			HRESULT ret = pDDSurface->Restore();
			
			if (ret == false)
				break;
			else
				hr = DD_OK;
		}
		if( hr != DDERR_WASSTILLDRAWING )
		{
			break;
		}
	}

	int a;
	switch (hr)
	{
		case DD_OK : a = 0; break;
		case DDERR_INVALIDOBJECT   : a = 0; break;
		case DDERR_INVALIDPARAMS   : a = 0; break;
		case DDERR_OUTOFMEMORY   : a = 0; break;
		case DDERR_SURFACEBUSY   : a = 0; break;
		case DDERR_SURFACELOST   : a = 0; break;
		case DDERR_WASSTILLDRAWING   : a = 0; break;
	}
	*/

	//unsigned short* lpSurface = (WORD*)ddsd.lpSurface;
	//pDDSurface->Unlock(NULL);

	// release 해 준다.
	//pDDSurface->Release();

	// (0,0)의 점으로 판단
	//return  *lpSurface == 0xFFFF;

	
	DDSURFACEDESC2 ddsd;

	m_pDDSBack->GetSurfaceDesc(&ddsd);
	ddsd.dwSize = sizeof(ddsd);  

	m_pDDSBack->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
	m_pDDSBack->Unlock(NULL);

	if( (ddsd.ddpfPixelFormat.dwRBitMask == 0xf800)
			&& (ddsd.ddpfPixelFormat.dwGBitMask==0x07e0)
			&& (ddsd.ddpfPixelFormat.dwBBitMask == 0x001f) )
	{
		return TRUE;			// 565
	}
	
	return FALSE;			// 555 
}

//----------------------------------------------------------------------
// MMX를 지원하는 system인가?
//----------------------------------------------------------------------
bool CDirectDraw::CheckMMX()
{
    SYSTEM_INFO si;
    int nCPUFeatures=0;
    GetSystemInfo(&si);
    if (si.dwProcessorType != PROCESSOR_INTEL_386 && si.dwProcessorType != PROCESSOR_INTEL_486)
    {
        try
        {
            __asm
            {
                ; we must push/pop the registers << CPUID>>  writes to, as the
				; optimiser doesn't know about << CPUID>> , and so doesn't expect
				; these registers to change.
                push eax
                push ebx
                push ecx
                push edx

                ; << CPUID>> 
                ; eax=0,1,2 -> CPU info in eax,ebx,ecx,edx
                mov eax,1
                _emit 0x0f
                _emit 0xa2
                mov nCPUFeatures,edx

                pop edx
                pop ecx
                pop ebx
                pop eax
            }
        }
        catch(...) // just to be sure...
        {
			return false;
        }
    }
    return (nCPUFeatures & 0x00800000) != 0;
}

//----------------------------------------------------------------------
// Ends DirectDraw and notice...
//----------------------------------------------------------------------
void CDirectDraw::DirectDrawFailed(const char * str) 
{
	// DirectDraw를 끝낸다.
	ReleaseAll();

	MessageBox(m_hWnd, str, NULL, MB_OK);
	PostMessage(m_hWnd, WM_CLOSE, 0, 0);
}

/*-----------------------------------------------------------------------------
- OnMove
- Window mode일 경우 Window move할 때(WM_MOVE) 실행한다.
-----------------------------------------------------------------------------*/
void CDirectDraw::OnMove()
{
	// Retrieve the window position after a move       
	if (!m_bFullscreen)
	{                
		GetWindowRect(m_hWnd, &m_rcWindow);            	
		GetClientRect(m_hWnd, &m_rcViewport);
		GetClientRect(m_hWnd, &m_rcScreen);
		ClientToScreen(m_hWnd, (POINT*)&m_rcScreen.left);
		ClientToScreen(m_hWnd, (POINT*)&m_rcScreen.right);            
	}
}

/*-----------------------------------------------------------------------------
- Get_Count_Rbit
- red의 bit mask count를 구한다.
-----------------------------------------------------------------------------*/
int CDirectDraw::Get_Count_Rbit()
{
	DWORD i, count_r, t_r;

	count_r = 0;
	t_r = Get_R_Bitmask();
	for (i = 0; i < Get_BPP(); i++)
	{
		if (t_r & (DWORD)(1<<i)) count_r++;
	}

	return count_r;
}

/*-----------------------------------------------------------------------------
- Get_Count_Gbit
- green의 bit mask count를 구한다.
-----------------------------------------------------------------------------*/
int CDirectDraw::Get_Count_Gbit()
{
	DWORD i, count_g, t_g;

	count_g = 0;
	t_g = Get_G_Bitmask();
	for (i = 0; i < Get_BPP(); i++)
	{
		if (t_g & (DWORD)(1<<i)) count_g++;
	}

	return count_g;
}

/*-----------------------------------------------------------------------------
- Get_Count_Bbit
- blut의 bit mask count를 구한다.
-----------------------------------------------------------------------------*/
int CDirectDraw::Get_Count_Bbit()
{
	DWORD i, count_b, t_b;

	count_b = 0;
	t_b = Get_B_Bitmask();
	for (i = 0; i < Get_BPP(); i++)
	{
		if (t_b & (DWORD)(1<<i)) count_b++;
	}

	return count_b;
}

/*-----------------------------------------------------------------------------
- Get_R_Bitmask
- red의 bit mask를 구한다.
-----------------------------------------------------------------------------*/
DWORD CDirectDraw::Get_R_Bitmask()
{
	DDPIXELFORMAT ddpf;

	ddpf.dwSize = sizeof(ddpf);
	m_pDDSPrimary->GetPixelFormat(&ddpf);
	return ddpf.dwRBitMask;
}

/*-----------------------------------------------------------------------------
- Get_G_Bitmask
- green의 bit mask를 구한다.
-----------------------------------------------------------------------------*/
DWORD CDirectDraw::Get_G_Bitmask()
{
	DDPIXELFORMAT ddpf;

	ddpf.dwSize = sizeof(ddpf);
	m_pDDSPrimary->GetPixelFormat(&ddpf);
	return ddpf.dwGBitMask;
}

/*-----------------------------------------------------------------------------
- Get_B_Bitmask
- blue의 bit mask를 구한다.
-----------------------------------------------------------------------------*/
DWORD CDirectDraw::Get_B_Bitmask()
{
	DDPIXELFORMAT ddpf;

	ddpf.dwSize = sizeof(ddpf);
	m_pDDSPrimary->GetPixelFormat(&ddpf);
	return ddpf.dwBBitMask;
}

/*-----------------------------------------------------------------------------
- Get_BPP
- Get bit per pixel
-----------------------------------------------------------------------------*/
DWORD CDirectDraw::Get_BPP()
{
   HRESULT        hr;
   DDPIXELFORMAT  ddpf;

   ddpf.dwSize = sizeof(ddpf);
   hr = m_pDDSPrimary->GetPixelFormat(&ddpf);
   if (hr != DD_OK) DirectDrawFailed("Get_BPP() Error!");

   return ddpf.dwRGBBitCount;
}