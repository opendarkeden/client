//----------------------------------------------------------------------
// CDirectDrawSurface.cpp
//----------------------------------------------------------------------
#include "DirectXlib_PCH.h"
#include "CDirectDrawSurface.h"
//#include "DebugInfo.h"
#ifdef _DEBUG
	#define OUTPUT_DEBUG2
#endif

//#include "2D.h"
//#include <assert.h>

//----------------------------------------------------------------------
// gamma - -; not my code~~ - from sigi
//----------------------------------------------------------------------
#define GAMMA_MASK565_1PIXEL1				00000000000000001111100000011111b
#define GAMMA_MASK565_1PIXEL2				00000000000000000000011111100000b
#define GAMMA_MASK565_2PIXEL1				11111000000111110000011111100000b
#define GAMMA_MASK565_2PIXEL2				00000111111000001111100000011111b
#define GAMMA_MASK565_4PIXEL_R				11111000000000001111100000000000b
#define GAMMA_MASK565_4PIXEL_G				00000111111000000000011111100000b
#define GAMMA_MASK565_4PIXEL_B				00000000000111110000000000011111b

#define GAMMA_MASK555_1PIXEL1				00000000000000000111110000011111b
#define GAMMA_MASK555_1PIXEL2				00000000000000000000001111100000b
#define GAMMA_MASK555_2PIXEL1				01111100000111110000001111100000b
#define GAMMA_MASK555_2PIXEL2				00000011111000000111110000011111b
#define GAMMA_MASK555_4PIXEL_R				01111100000000000111110000000000b
#define GAMMA_MASK555_4PIXEL_G				00000011111000000000001111100000b
#define GAMMA_MASK555_4PIXEL_B				00000000000111110000000000011111b

//----------------------------------------------------------------------
// static
//----------------------------------------------------------------------
void		(*CDirectDrawSurface::s_GammaFunction)(void *pDest, int len, int p) = Gamma4Pixel565;

#ifdef OUTPUT_DEBUG2
	int g_SurfaceCount = 0;
#endif

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
CDirectDrawSurface::CDirectDrawSurface()
{
	m_pDDSurface		= NULL;
	m_Width				= 0;
	m_Height			= 0;
	m_ClipLeft			= 0;
	m_ClipTop			= 0;
	m_ClipRight			= 0;
	m_ClipBottom		= 0;
	m_bBackSurface		= false;
	m_bLock				= false;

	#ifdef OUTPUT_DEBUG2
		g_SurfaceCount++;
	#endif
}

CDirectDrawSurface::~CDirectDrawSurface()
{
	// BackSurface의 경우는 
	// CDirectDraw에서 Release하기 때문에 Release해서는 안된다.
	if (!m_bBackSurface && m_pDDSurface != NULL)
	{
		m_pDDSurface->Release();
		m_pDDSurface = NULL;
	}

	#ifdef OUTPUT_DEBUG2
		g_SurfaceCount--;
	#endif
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// BackBuffer로 사용하는 Surface이다.
// CDirectDraw의 Init에서 생성된 걸 이용한다.
//
// `이미 초기화되어 있으면 false를 반환한다.
//----------------------------------------------------------------------
bool CDirectDrawSurface::InitBacksurface()
{
	bool check = false;
	if (m_pDDSurface)
		check = true;

	m_pDDSurface	= m_pDDSBack;
	m_Width	= CDirectDraw::m_ScreenWidth;
	m_Height	= CDirectDraw::m_ScreenHeight;
	m_ClipRight		= m_Width;
	m_ClipBottom	= m_Height;

	m_bBackSurface	= true;

	if (check)
		return false;
	else
		return true;
}

//----------------------------------------------------------------------
// dwCaps는 DDSCAPS_VIDEOMEMORY(default)나
//          DDSCAPS_SYSTEMMEMORY로 쓸 수 있다.
//
// system memory는 Lock/Unlock을 이용할 경우 빠르지만 Blt 등은 느리고..
// video memory는 그 반대다. (중요!)
// 
// Lock과 Blt의 사용 비율을 고려해서 Surface의 memory 위치를 결정해야 한다.
//
// 이미 초기화되어 있으면 false를 반환한다.
//----------------------------------------------------------------------
bool CDirectDrawSurface::InitOffsurface(int wWidth, int wHeight, DWORD dwCaps)
{
	bool check = false;
	if (m_pDDSurface)
	{
		m_pDDSurface->Release();
		m_pDDSurface = NULL;
		check = true;
	}

	DDSURFACEDESC2	ddsd;	
	
	// Surface에 대한 정보 설정
	// m_pSurface를 OffScreen으로 설정한다.
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | dwCaps;

	/*
	if (dwCaps & DDSCAPS_VIDEOMEMORY 
		&& wWidth > m_ScreenWidth || wHeight > m_ScreenHeight)
	{
		ddsd.ddsCaps.dwCaps2 = DDCAPS2_WIDESURFACES;
	}
	*/

	ddsd.dwWidth  = wWidth;
	ddsd.dwHeight = wHeight;
	
	if (m_pDD->CreateSurface(&ddsd, &m_pDDSurface, NULL) != DD_OK)
	{ 
		//char str[80];
		//sprintf(str, "DD-Create OffSurface Error! (%d, %d)", wWidth, wHeight);
		//DirectDrawFailed(str);
		return false; 
	}

	// member init...
	m_Width	= wWidth;
	m_Height	= wHeight;
	m_ClipRight		= wWidth;
	m_ClipBottom	= wHeight;	
	m_bBackSurface	= false;

	// 투명색을 기본으로 설정해둔다.
	SetTransparency(0);

	if (check)
		return false;
	else
		return true;
}

//----------------------------------------------------------------------
// BMP file을 load한다.
//
// `내부에서 offscreen을 만들어 load한다. 
//----------------------------------------------------------------------
bool 
CDirectDrawSurface::InitTextureSurfaceFromBMP(LPCSTR szBitmap, DWORD dwCaps)
{
	// 이미 사용중이면 지우고 Load한다.
	if (m_pDDSurface != NULL)
	{
		m_pDDSurface->Release();
		m_pDDSurface = NULL;
	}

	HBITMAP                 hbm;
   BITMAP                  bm;

   //
   //  Try to load the bitmap as a resource, if that fails, try it as a file
   //
   hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0,
                             0, LR_CREATEDIBSECTION);
   if (hbm == NULL)
       hbm = (HBITMAP) LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
                                 LR_LOADFROMFILE | LR_CREATEDIBSECTION);
   if (hbm == NULL)
       return NULL;
   //
   // Get size of the bitmap
   //
   GetObject(hbm, sizeof(bm), &bm);

	InitTextureSurface(bm.bmWidth, bm.bmHeight, dwCaps);

   CopyBitmap(hbm, 0, 0, 0, 0);
   DeleteObject(hbm);
	
	return true;
}

//----------------------------------------------------------------------
//
// Texture로 사용할 Surface를 생성한다.
//
// 이미 초기화되어 있으면 false를 반환한다.
//----------------------------------------------------------------------
bool 
CDirectDrawSurface::InitTextureSurface(int wWidth, int wHeight, DWORD dwCaps, const LPDDPIXELFORMAT pDDPF)
{
	#ifdef OUTPUT_DEBUG
		if (g_pDebugMessage)
		{
			DEBUG_ADD_FORMAT("InitTextureSurface: (%d, %d)", wWidth, wHeight);
		}
	#endif

	bool check = false;
	if (m_pDDSurface)
	{
		m_pDDSurface->Release();
		m_pDDSurface = NULL;
		check = true;
	}

	#ifdef OUTPUT_DEBUG
		if (g_pDebugMessage)
		{
			DEBUG_ADD("Before ZeroMemory");
		}
	#endif

	DDSURFACEDESC2	ddsd;	
	
	// Surface에 대한 정보 설정
	// m_pSurface를 OffScreen으로 설정한다.
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof( ddsd );

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;// | DDSD_CKSRCBLT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | dwCaps;
    ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	ddsd.dwWidth  = wWidth;
	ddsd.dwHeight = wHeight;

	#ifdef OUTPUT_DEBUG
		if (g_pDebugMessage)
		{
			DEBUG_ADD("Set PixelFormat");
		}
	#endif

	// pixelformat을 임의로 정하는 경우
	if (pDDPF!=NULL)
	{
		ddsd.dwFlags |= DDSD_PIXELFORMAT;		
		memcpy(&ddsd.ddpfPixelFormat, pDDPF, sizeof(DDPIXELFORMAT));
	}	

	#ifdef OUTPUT_DEBUG
		if (g_pDebugMessage)
		{
			DEBUG_ADD("Before CreateSurface");
		}
	#endif

	HRESULT hr = m_pDD->CreateSurface(&ddsd, &m_pDDSurface, NULL);
	if (hr != DD_OK)
	{ 
		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("CreateSurface Error");
			
				switch (hr)
				{
					case DDERR_INCOMPATIBLEPRIMARY		: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_INCOMPATIBLEPRIMARY"); break;
					case DDERR_INVALIDCAPS				: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_INVALIDCAPS"); break;
					case DDERR_INVALIDOBJECT			: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_INVALIDOBJECT"); break;
					case DDERR_INVALIDPARAMS			: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_INVALIDPARAMS"); break;
					case DDERR_INVALIDPIXELFORMAT		: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_INVALIDPIXELFORMAT"); break;
					case DDERR_NOALPHAHW				: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_NOALPHAHW"); break;
					case DDERR_NOCOOPERATIVELEVELSET	: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_NOCOOPERATIVELEVELSET"); break;
					case DDERR_NODIRECTDRAWHW			: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_NODIRECTDRAWHW"); break;
					case DDERR_NOEMULATION				: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_NOEMULATION"); break;
					case DDERR_NOEXCLUSIVEMODE			: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_NOEXCLUSIVEMODE"); break;
					case DDERR_NOFLIPHW					: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_NOFLIPHW"); break;
					case DDERR_NOMIPMAPHW				: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_NOMIPMAPHW"); break;
					case DDERR_NOOVERLAYHW				: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_NOOVERLAYHW"); break;
					case DDERR_NOZBUFFERHW				: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_NOZBUFFERHW"); break;
					case DDERR_OUTOFMEMORY				: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_OUTOFMEMORY"); break;
					case DDERR_OUTOFVIDEOMEMORY			: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_OUTOFVIDEOMEMORY"); break;
					case DDERR_PRIMARYSURFACEALREADYEXISTS  : DEBUG_ADD("DD-Create TextureSurface Error : DDERR_PRIMARYSURFACEALREADYEXISTS"); break;
					case DDERR_UNSUPPORTEDMODE			: DEBUG_ADD("DD-Create TextureSurface Error : DDERR_UNSUPPORTEDMODE"); break;
				};
			}
		#endif

		/*
		switch (hr)
		{
			case DDERR_INCOMPATIBLEPRIMARY		: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_INCOMPATIBLEPRIMARY"); break;
			case DDERR_INVALIDCAPS				: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_INVALIDCAPS"); break;
			case DDERR_INVALIDOBJECT			: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_INVALIDOBJECT"); break;
			case DDERR_INVALIDPARAMS			: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_INVALIDPARAMS"); break;
			case DDERR_INVALIDPIXELFORMAT		: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_INVALIDPIXELFORMAT"); break;
			case DDERR_NOALPHAHW				: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_NOALPHAHW"); break;
			case DDERR_NOCOOPERATIVELEVELSET	: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_NOCOOPERATIVELEVELSET"); break;
			case DDERR_NODIRECTDRAWHW			: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_NODIRECTDRAWHW"); break;
			case DDERR_NOEMULATION				: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_NOEMULATION"); break;
			case DDERR_NOEXCLUSIVEMODE			: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_NOEXCLUSIVEMODE"); break;
			case DDERR_NOFLIPHW					: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_NOFLIPHW"); break;
			case DDERR_NOMIPMAPHW				: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_NOMIPMAPHW"); break;
			case DDERR_NOOVERLAYHW				: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_NOOVERLAYHW"); break;
			case DDERR_NOZBUFFERHW				: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_NOZBUFFERHW"); break;
			case DDERR_OUTOFMEMORY				: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_OUTOFMEMORY"); break;
			case DDERR_OUTOFVIDEOMEMORY			: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_OUTOFVIDEOMEMORY"); break;
			case DDERR_PRIMARYSURFACEALREADYEXISTS  : DirectDrawFailed("DD-Create TextureSurface Error : DDERR_PRIMARYSURFACEALREADYEXISTS"); break;
			case DDERR_UNSUPPORTEDMODE			: DirectDrawFailed("DD-Create TextureSurface Error : DDERR_UNSUPPORTEDMODE"); break;
		};
		*/
		 
		return false; 
	}
	else
	{
		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("After CreateSurface");
			}
		#endif
	}

	// member init...
	m_Width	= wWidth;
	m_Height	= wHeight;
	m_ClipRight		= m_Width;
	m_ClipBottom	= m_Height;
	m_bBackSurface	= false;

	if (check)
		return false;
	else
		return true;
}

//----------------------------------------------------------------------
// Restore Surface
//----------------------------------------------------------------------
bool CDirectDrawSurface::Restore()
{
	// restore primary surface
	//if (m_pDDSPrimary->IsLost() == DDERR_SURFACELOST)
	//	if (m_pDDSPrimary->Restore() != DD_OK)
	//		return false;
	RestoreAllSurfaces(); // 내부에서 error 처리.

	HRESULT hr;
	// restore current surface
	//if (m_pDDSurface->IsLost() == DDERR_SURFACELOST)
	//{
	
	// restore 100번 시도... - -;
	for (int i=0; i<100; i++)
	{
		hr = m_pDDSurface->Restore();

		if ( hr == DD_OK)
		{
			return true;
		}
	}
	
	/*
	switch (hr)
	{
		case DDERR_GENERIC : DirectDrawFailed("DXSurface Restore:DDERR_GENERIC"); break;
		case DDERR_IMPLICITLYCREATED : DirectDrawFailed("DXSurface Restore:DDERR_IMPLICITLYCREATED"); break;
		case DDERR_INCOMPATIBLEPRIMARY : DirectDrawFailed("DXSurface Restore:DDERR_INCOMPATIBLEPRIMARY"); break;
		case DDERR_INVALIDOBJECT : DirectDrawFailed("DXSurface Restore:DDERR_INVALIDOBJECT"); break;
		case DDERR_INVALIDPARAMS : DirectDrawFailed("DXSurface Restore:DDERR_INVALIDPARAMS"); break;
		case DDERR_NOEXCLUSIVEMODE : DirectDrawFailed("DXSurface Restore:DDERR_NOEXCLUSIVEMODE"); break;
		case DDERR_OUTOFMEMORY : DirectDrawFailed("DXSurface Restore:DDERR_OUTOFMEMORY"); break;
		case DDERR_UNSUPPORTED : DirectDrawFailed("DXSurface Restore:DDERR_UNSUPPORTED"); break;
		case DDERR_WRONGMODE : DirectDrawFailed("DXSurface Restore:DDERR_WRONGMODE"); break;
	}
	*/
	//}
	
	return false;
}


//----------------------------------------------------------------------
// BMP file을 load한다.
//
// `내부에서 offscreen을 만들어 load한다. 
//----------------------------------------------------------------------
bool CDirectDrawSurface::InitFromBMP(LPCSTR szBitmap, DWORD dwCaps)
{
	// 이미 사용중이면 지우고 Load한다.
	if (m_pDDSurface != NULL)
	{
		m_pDDSurface->Release();
		m_pDDSurface = NULL;
	}

	HBITMAP                 hbm;
   BITMAP                  bm;

   //
   //  Try to load the bitmap as a resource, if that fails, try it as a file
   //
   hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0,
                             0, LR_CREATEDIBSECTION);
   if (hbm == NULL)
       hbm = (HBITMAP) LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
                                 LR_LOADFROMFILE | LR_CREATEDIBSECTION);
   if (hbm == NULL)
       return NULL;
   //
   // Get size of the bitmap
   //
   GetObject(hbm, sizeof(bm), &bm);

   if (!InitOffsurface(bm.bmWidth, bm.bmHeight, dwCaps))
   {
		DirectDrawFailed("Load BMP Failed!");
		return false;
   }

   CopyBitmap(hbm, 0, 0, 0, 0);
   DeleteObject(hbm);
	
	return true;
}


//----------------------------------------------------------------------
// 한번 Load되었던 BMP file을 다시 Load한다.
//
// `음... 이거 잘 안되는 거 같다. - 2000.1.5.
//----------------------------------------------------------------------
bool CDirectDrawSurface::ReLoadBMP(LPCSTR szBitmap)
{
	//-----------------------------------------------------------------------------
	// Name: DDReLoadBitmap()
	// Desc: Load a bitmap from a file or resource into a directdraw surface.
	//       normaly used to re-load a surface after a restore.
	//-----------------------------------------------------------------------------

   HBITMAP                 hbm;
   HRESULT                 hr;

   //
   //  Try to load the bitmap as a resource, if that fails, try it as a file
   //
   hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0,
                             0, LR_CREATEDIBSECTION);
   if (hbm == NULL)
       hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
                                 LR_LOADFROMFILE | LR_CREATEDIBSECTION);
   if (hbm == NULL)
   {
       OutputDebugString("handle is null\n");
       return false;
   }

   hr = CopyBitmap(hbm, 0, 0, 0, 0);
   if (hr != DD_OK)
   {
      //DirectDrawFailed("Load BMP("+CString(szBitmap)+") Failed!");
		DirectDrawFailed("Re-Load BMP Failed!");
		return false;
   }
   DeleteObject(hbm);

	return true;
}

//----------------------------------------------------------------------
// Bitmap을 Surface로 Copy한다.
//----------------------------------------------------------------------
bool CDirectDrawSurface::CopyBitmap(HBITMAP hbm, int x, int y, int dx, int dy)
{
   HDC                     hdcImage;
   HDC                     hdc;
   BITMAP                  bm;
   DDSURFACEDESC2          ddsd;
   HRESULT                 hr;

   if (hbm == NULL || m_pDDSurface == NULL)
       return false;
   //
   // Make sure this surface is restored.
   //
   m_pDDSurface->Restore();
   //
   // Select bitmap into a memoryDC so we can use it.
   //
   hdcImage = CreateCompatibleDC(NULL);
   if (!hdcImage)
       OutputDebugString("createcompatible dc failed\n");
   SelectObject(hdcImage, hbm);
   //
   // Get size of the bitmap
   //
   GetObject(hbm, sizeof(bm), &bm);
   dx = dx == 0 ? bm.bmWidth : dx;     // Use the passed size, unless zero
   dy = dy == 0 ? bm.bmHeight : dy;
   //
   // Get size of surface.
   //
   ddsd.dwSize = sizeof(ddsd);
   ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
   m_pDDSurface->GetSurfaceDesc(&ddsd);

   if ((hr = m_pDDSurface->GetDC(&hdc)) == DD_OK)
   {
       StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y,
                  dx, dy, SRCCOPY);
       m_pDDSurface->ReleaseDC(hdc);
   }
   DeleteDC(hdcImage);

   return true;
}

//----------------------------------------------------------------------
// transparency color를 dwValue로 설정한다. (필수!!)
//----------------------------------------------------------------------
void CDirectDrawSurface::SetTransparency(DWORD dwValue)
{
	DDCOLORKEY ddck;

	// 투명색을 dwValue로 설정
	// ( Low에서 High까지 )
	ddck.dwColorSpaceLowValue  = dwValue;
	ddck.dwColorSpaceHighValue = dwValue;

	m_pDDSurface->SetColorKey(DDCKEY_SRCBLT, &ddck);
}

//----------------------------------------------------------------------
// Surface 전체를 한 색깔로 채우기
//----------------------------------------------------------------------
void CDirectDrawSurface::FillSurface(WORD color)
{
	DDBLTFX		ddbltfx;

	ddbltfx.dwSize = sizeof(DDBLTFX);
	ddbltfx.dwFillColor = color;

	m_pDDSurface->Blt(NULL, NULL, NULL, 
									DDBLT_COLORFILL | DDBLT_WAIT, 
									&ddbltfx);
/*
	if (FAILED(m_pDDSurface->Blt(NULL, NULL, NULL, 
								DDBLT_COLORFILL | DDBLT_WAIT, 
								&ddbltfx)))
	{
		DirectDrawFailed("Direct Draw ClearSurface Error!");
	}*/
}

//----------------------------------------------------------------------
// 사각형 그리기
//----------------------------------------------------------------------
void CDirectDrawSurface::FillRect(RECT* pRect, WORD color)
{
	RECT rect = *pRect;
	// pRect clipping
	if (rect.left > m_ClipRight) return;
	if (rect.top > m_ClipBottom) return;
	if (rect.left < m_ClipLeft) rect.left=m_ClipLeft;
	if (rect.right > m_ClipRight) rect.right=m_ClipRight;
	if (rect.top < m_ClipTop) rect.top=m_ClipTop;
	if (rect.bottom > m_ClipBottom) rect.bottom=m_ClipBottom;

	DDBLTFX	ddbltfx;

	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = color;

	m_pDDSurface->Blt(&rect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);
}

//----------------------------------------------------------------------
// Lock() : Surface에 직접 그리기 위한 정보
//
// lpSurface : surface memory 시작 위치
// lPitch    : 한 줄 수 
//
// Surface에 대해서 Lock()을 사용한 후는 반드시 Unlock()을 해줘야 한다.
//
// `실패하면 false를 반환하는 데, 이 때는 Unlock()을 해주지 말아야 한다.
//----------------------------------------------------------------------

bool CDirectDrawSurface::Lock()
{
	HRESULT			hr;
	bool				ret;

	ZeroMemory(&m_ddsd, sizeof(m_ddsd));
	m_ddsd.dwSize = sizeof(m_ddsd);

	while (1)
   {
		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("Before Lock()");
			}
		#endif

		//hr = m_pDDSurface->Lock(NULL, &m_ddsd,  0, NULL);
		hr = m_pDDSurface->Lock(NULL, &m_ddsd,  DDLOCK_WAIT, NULL);

		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage)
			{
				DEBUG_ADD("After Lock()");
			}
		#endif

		if( hr == DD_OK )
		{
			#ifdef OUTPUT_DEBUG
				if (g_pDebugMessage)
				{
					DEBUG_ADD("DD OK");
				}
			#endif
			break;
		}
		else if( hr == DDERR_SURFACELOST )
		{
			#ifdef OUTPUT_DEBUG
				if (g_pDebugMessage)
				{
					DEBUG_ADD("Surface Lost & Before Restore");
				}
			#endif
			ret = RestoreAllSurfaces();

			if (ret)
			{
				#ifdef OUTPUT_DEBUG
					if (g_pDebugMessage)
					{
						DEBUG_ADD("Restore OK");
					}
				#endif
			}
			else
			{
				#ifdef OUTPUT_DEBUG
					if (g_pDebugMessage)
					{
						DEBUG_ADD("Restore Failed");
					}
				#endif
			}

			//if (ret == false)
			//	break;
			//else
			continue;//hr = DD_OK;
		}
		else if( hr != DDERR_WASSTILLDRAWING )
		{
			#ifdef OUTPUT_DEBUG
				if (g_pDebugMessage)
				{
					DEBUG_ADD("What??");
				}
			#endif

			break;
		}
	}

	if (hr == DD_OK)
	{
		m_bLock = true;
		return true;
	}
	
	return false;
}

//----------------------------------------------------------------------
// LockW
//----------------------------------------------------------------------
void CDirectDrawSurface::LockW(WORD*& lpSurface, WORD& lPitch)
{
	HRESULT			hRet;

	ZeroMemory(&m_ddsd, sizeof(m_ddsd));
	m_ddsd.dwSize = sizeof(m_ddsd);

	bool exitLoop = false;

	do 
	{
		hRet = m_pDDSurface->Lock(NULL, &m_ddsd, 0, NULL);
		if (hRet == DDERR_SURFACELOST)
		{
			Restore();
			hRet = m_pDDSurface->Lock(NULL, &m_ddsd, 0, NULL);		
		}		
		
		else if(hRet != DDERR_WASSTILLDRAWING) exitLoop = true;

	} while(!exitLoop);

	lpSurface	= (WORD*)m_ddsd.lpSurface;
	lPitch		= (WORD)m_ddsd.lPitch;

	m_bLock = true;
}
/*
//----------------------------------------------------------------------
// LockDW
//----------------------------------------------------------------------
void CDirectDrawSurface::LockDW(DWORD*& lpSurface, WORD& lPitch)
{
	HRESULT			hRet;
	DDSURFACEDESC2	ddsdDesc;

	ddsdDesc.dwSize = sizeof(DDSURFACEDESC);


	bool exitLoop = false;

	do 
	{
		hRet = m_pDDSurface->Lock(NULL, &ddsdDesc, 0, NULL);
		if (hRet == DDERR_SURFACELOST)
		{
			Restore();
			hRet = m_pDDSurface->Lock(NULL, &ddsdDesc, 0, NULL);		
		}		

		// 이거는 없어야 되는 code일까??
		else if(hRet != DDERR_WASSTILLDRAWING) exitLoop = true;

	} while(!exitLoop);

	lpSurface	= (DWORD*)ddsdDesc.lpSurface;
	lPitch		= (WORD)ddsdDesc.lPitch;
}

//----------------------------------------------------------------------
// LockDW
//----------------------------------------------------------------------
void CDirectDrawSurface::LockQW(QWORD*& lpSurface, WORD& lPitch)
{
	HRESULT			hRet;
	DDSURFACEDESC2	ddsdDesc;

	ddsdDesc.dwSize = sizeof(DDSURFACEDESC);


	bool exitLoop = false;

	do 
	{
		hRet = m_pDDSurface->Lock(NULL, &ddsdDesc, 0, NULL);
		if (hRet == DDERR_SURFACELOST)
		{
			Restore();
			hRet = m_pDDSurface->Lock(NULL, &ddsdDesc, 0, NULL);		
		}		

		// 이거는 없어야 되는 code일까??
		else if(hRet != DDERR_WASSTILLDRAWING) exitLoop = true;

	} while(!exitLoop);

	lpSurface	= (QWORD*)ddsdDesc.lpSurface;
	lPitch		= (WORD)ddsdDesc.lPitch;
}*/

//----------------------------------------------------------------------
// Unlock()
//----------------------------------------------------------------------
bool CDirectDrawSurface::Unlock()
{
	/*
	// Unlock the offscreen surface
	if (m_pDDSurface->Unlock(NULL) != DD_OK)
	{
		DirectDrawFailed("DD-Unlock Surface Error!");
	}
*/
	HRESULT	hr;
	bool		ret;

	//hr = m_pDDSurface->Unlock(NULL);
	
	while ( 1 )
	{
		hr = m_pDDSurface->Unlock(NULL);
		if( hr == DD_OK || hr == DDERR_NOTLOCKED )
		{
			break;
		}
		if( hr == DDERR_SURFACELOST )
		{
			ret = Restore();
			if (ret == false)
				break;
			else
				hr = DD_OK;
		}
	}
	
	if (hr == DD_OK)
	{
		m_bLock = false;
		return true;
	}
	
	return false;
}

/*-----------------------------------------------------------------------------
- GammaBox 555
- 0 ~ 32까지..인가?? - -;;
-----------------------------------------------------------------------------*/
void CDirectDrawSurface::GammaBox555(RECT* pRect, int p)
{
	// pRect clipping
	if (pRect->left > m_ClipRight) return;
	if (pRect->top > m_ClipBottom) return;
	if (pRect->left < m_ClipLeft) pRect->left=m_ClipLeft;
	if (pRect->right > m_ClipRight) pRect->right=m_ClipRight;
	if (pRect->top < m_ClipTop) pRect->top=m_ClipTop;
	if (pRect->bottom > m_ClipBottom) pRect->bottom=m_ClipBottom;

	long pitch = m_ddsd.lPitch;   
    WORD* lpDest = (WORD*)((BYTE*)m_ddsd.lpSurface + 
						pRect->top*pitch + (pRect->left<<1));

    int dLen=pRect->right - pRect->left;

	int i= pRect->bottom - pRect->top;

	if (i > 0)
	{
		do 
		{		
			Gamma4Pixel555(lpDest, dLen, p);
			lpDest = (WORD*)((BYTE*)lpDest + pitch);
		} while (--i);
	}
}

/*-----------------------------------------------------------------------------
- GammaBox 565
- 0 ~ 32까지..인가?? - -;;
-----------------------------------------------------------------------------*/
void CDirectDrawSurface::GammaBox565(RECT* pRect, int p)
{
	if (pRect->bottom < m_ClipTop 
		|| pRect->top > m_ClipBottom
		|| pRect->right < m_ClipLeft 
		|| pRect->left > m_ClipRight) return;

	if (pRect->left < m_ClipLeft) pRect->left=m_ClipLeft;
	if (pRect->right > m_ClipRight) pRect->right=m_ClipRight;
	if (pRect->top < m_ClipTop) pRect->top=m_ClipTop;
	if (pRect->bottom > m_ClipBottom) pRect->bottom=m_ClipBottom;
	

	long pitch = m_ddsd.lPitch;
    WORD* lpDest = (WORD*)((BYTE*)m_ddsd.lpSurface + 
						pRect->top*pitch + (pRect->left<<1));

    int dLen=pRect->right - pRect->left;

	int i= pRect->bottom - pRect->top;

	if (i > 0)
	{
		do 
		{		
			Gamma4Pixel565(lpDest, dLen, p);
			lpDest = (WORD*)((BYTE*)lpDest + pitch);
		} while (--i);
	}
}

/*-----------------------------------------------------------------------------
- Gamma4Pixel
-
-----------------------------------------------------------------------------*/
void CDirectDrawSurface::Gamma4Pixel565(void *pDest, int len, int p)
{
	__asm
	{
		mov edi, pDest
		mov ecx, len
		shr ecx, 1
		jnc GAMMA_2PIXEL
;;one pixel
		mov edx, p
		mov eax, [edi]

		push eax
		push edx

		and eax, GAMMA_MASK565_1PIXEL1
		mul edx
		shr eax, 5
		and eax, GAMMA_MASK565_1PIXEL1

		mov ebx, eax
		pop edx
		pop eax

		and eax, GAMMA_MASK565_1PIXEL2
		mul edx
		shr eax, 5
		and eax, GAMMA_MASK565_1PIXEL2

		or eax, ebx
		mov [edi], ax
		add edi, 2

GAMMA_2PIXEL:
		shr ecx, 1
		jnc GAMMA_4PIXEL

;;two pixel
		mov edx, p
		mov eax, [edi]

		push eax
		push edx

		and eax, GAMMA_MASK565_2PIXEL1
		shr eax, 5
		mul edx
		and eax, GAMMA_MASK565_2PIXEL1

		mov ebx, eax
		pop edx
		pop eax

		and eax, GAMMA_MASK565_2PIXEL2
		mul edx
		shr eax, 5
		and eax, GAMMA_MASK565_2PIXEL2

		or eax, ebx
		mov [edi], eax
		add edi, 4

GAMMA_4PIXEL:
		test ecx, ecx
		jz GAMMA_END

		movd mm0, p
		punpcklwd mm0, mm0
		punpckldq mm0, mm0

		mov eax, GAMMA_MASK565_4PIXEL_R
		mov ebx, GAMMA_MASK565_4PIXEL_G
		mov edx, GAMMA_MASK565_4PIXEL_B

		movd mm1, eax
		movd mm2, ebx
		movd mm3, edx

		punpckldq mm1, mm1
		punpckldq mm2, mm2
		punpckldq mm3, mm3

GAMMA_4PIXEL_LOOP:
		movq mm4, [edi]
		movq mm5, mm4
		movq mm6, mm4

		pand mm4, mm1 ;; red
		pand mm5, mm2 ;; green

		pand mm6, mm3 ;; blue
		psrlw mm4, 5

		pmullw mm4, mm0
		psrlw mm5, 5

		pand mm4, mm1
		pmullw mm5, mm0

		pand mm5, mm2
		pmullw mm6, mm0

		psrlw mm6, 5
		pand mm6, mm3

		por mm4, mm5
		por mm4, mm6

		movq [edi], mm4

		add edi, 8
		dec ecx
		jnz GAMMA_4PIXEL_LOOP

GAMMA_END:
		emms
	}
}


/*-----------------------------------------------------------------------------
- Gamma4Pixel
-
-----------------------------------------------------------------------------*/
void CDirectDrawSurface::Gamma4Pixel555(void *pDest, int len, int p)
{
	__asm
	{
		mov edi, pDest
		mov ecx, len
		shr ecx, 1
		jnc GAMMA_2PIXEL
;;one pixel
		mov edx, p
		mov eax, [edi]

		push eax
		push edx

		and eax, GAMMA_MASK555_1PIXEL1
		mul edx
		shr eax, 5
		and eax, GAMMA_MASK555_1PIXEL1

		mov ebx, eax
		pop edx
		pop eax

		and eax, GAMMA_MASK555_1PIXEL2
		mul edx
		shr eax, 5
		and eax, GAMMA_MASK555_1PIXEL2

		or eax, ebx
		mov [edi], ax
		add edi, 2

GAMMA_2PIXEL:
		shr ecx, 1
		jnc GAMMA_4PIXEL

;;two pixel
		mov edx, p
		mov eax, [edi]

		push eax
		push edx

		and eax, GAMMA_MASK555_2PIXEL1
		shr eax, 5
		mul edx
		and eax, GAMMA_MASK555_2PIXEL1

		mov ebx, eax
		pop edx
		pop eax

		and eax, GAMMA_MASK555_2PIXEL2
		mul edx
		shr eax, 5
		and eax, GAMMA_MASK555_2PIXEL2

		or eax, ebx
		mov [edi], eax
		add edi, 4

GAMMA_4PIXEL:
		test ecx, ecx
		jz GAMMA_END

		movd mm0, p
		punpcklwd mm0, mm0
		punpckldq mm0, mm0

		mov eax, GAMMA_MASK555_4PIXEL_R
		mov ebx, GAMMA_MASK555_4PIXEL_G
		mov edx, GAMMA_MASK555_4PIXEL_B

		movd mm1, eax
		movd mm2, ebx
		movd mm3, edx

		punpckldq mm1, mm1
		punpckldq mm2, mm2
		punpckldq mm3, mm3

GAMMA_4PIXEL_LOOP:
		movq mm4, [edi]
		movq mm5, mm4
		movq mm6, mm4

		pand mm4, mm1 ;; red
		pand mm5, mm2 ;; green

		pand mm6, mm3 ;; blue
		psrlw mm4, 5

		pmullw mm4, mm0
		psrlw mm5, 5

		pand mm4, mm1
		pmullw mm5, mm0

		pand mm5, mm2
		pmullw mm6, mm0

		psrlw mm6, 5
		pand mm6, mm3

		por mm4, mm5
		por mm4, mm6

		movq [edi], mm4

		add edi, 8
		dec ecx
		jnz GAMMA_4PIXEL_LOOP

GAMMA_END:
		emms
	}
}

//----------------------------------------------------------------------
// Copy : pDDsourceSurface의 rect 영역을 
//        pDDTargetSurface의 point 위치로 Copy한다.
//
// `SetTransparency()로 투명색을 지정한다.
// `그냥 blt하려면 BltNoColorkey()를 사용한다.
//----------------------------------------------------------------------
void 
CDirectDrawSurface::Blt(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT*  pRect)
{
	// TargetSurface의 영역을 넘어가는지에 대한 체크
	if (pPoint->x>=(int)m_ClipRight || pPoint->y>=(int)m_ClipBottom) 
		return;

	// width clipping
	if (pPoint->x<m_ClipLeft) 
	{
		pRect->left = pRect->left-pPoint->x + m_ClipLeft;
		pPoint->x = m_ClipLeft;
	} 
	else if ((int)m_ClipRight-pPoint->x < pRect->right-pRect->left) 
	{
		pRect->right = pRect->left+ m_ClipRight-pPoint->x;
	}

	// height clipping
	if (pPoint->y<m_ClipTop) 
	{
		pRect->top = pRect->top-pPoint->y + m_ClipTop;
		pPoint->y = m_ClipTop;
	} 
	else if ((int)m_ClipBottom-pPoint->y < pRect->bottom-pRect->top) 
	{
		pRect->bottom = pRect->top+ m_ClipBottom-pPoint->y;
	}
	
	// Blit the current frame to the destination surface
	HRESULT hRet;

	while (1) {
		hRet = m_pDDSurface->BltFast(pPoint->x, pPoint->y,
									SourceSurface->m_pDDSurface, 
									pRect, 
									DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
  		if (hRet == DD_OK) break;

		if (hRet == DDERR_SURFACELOST) 
		{		
			if (!Restore() || !SourceSurface->Restore()) break;
		}
		else if (hRet != DDERR_WASSTILLDRAWING) 
				break;
	}
}

//----------------------------------------------------------------------
// Copy : pDDsourceSurface의 rect 영역을 
//        pDDTargetSurface의 point 위치로 Copy한다.
//----------------------------------------------------------------------
void
CDirectDrawSurface::BltNoColorkey(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT*  pRect)
{
	// TargetSurface의 영역을 넘어가는지에 대한 체크
	if (pPoint->x>=(int)m_ClipRight || pPoint->y>=(int)m_ClipBottom) 
		return;

	// width clipping
	if (pPoint->x<m_ClipLeft) 
	{
		pRect->left += -pPoint->x + m_ClipLeft;
		pPoint->x = m_ClipLeft;
	} 
	if ((int)m_ClipRight-pPoint->x < pRect->right-pRect->left) 
	{
		pRect->right = pRect->left+ m_ClipRight-pPoint->x;
	}

	// height clipping
	if (pPoint->y<m_ClipTop) 
	{
		pRect->top += -pPoint->y + m_ClipTop;
		pPoint->y = m_ClipTop;
	} 
	if ((int)m_ClipBottom-pPoint->y < pRect->bottom-pRect->top) 
	{
		pRect->bottom = pRect->top+ m_ClipBottom-pPoint->y;
	}
	
	// Blit the current frame to the destination surface
	HRESULT hRet;

	while (1) {
		hRet = m_pDDSurface->BltFast(pPoint->x, pPoint->y,
									SourceSurface->m_pDDSurface, 
									pRect, 
									DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
  		if (hRet == DD_OK) break;

		if (hRet == DDERR_SURFACELOST) 
		{		
			if (!Restore() || !SourceSurface->Restore()) break;
		}
		else if (hRet != DDERR_WASSTILLDRAWING) 
				break;
	}
}


//----------------------------------------------------------------------
// Copy : pDDsourceSurface의 rect 영역을 
//        pDDTargetSurface의 point 위치로 Copy한다.
//
// `SetTransparency()로 투명색을 지정한다.
// `그냥 blt하려면 BltNoColorkey()를 사용한다.
// 임시로(-_-;) 투명색 무시.. ㅋㅋ - -;
//----------------------------------------------------------------------
void 
CDirectDrawSurface::Blt(RECT* pDestRect, CDirectDrawSurface* SourceSurface, RECT* pSourceRect)
{
	// Blit the current frame to the destination surface
	HRESULT hRet;
	
	DDBLTFX ddbltfx;
	
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwDDFX = DDBLTFX_ARITHSTRETCHY | DDBLT_DDFX; 

	
	while (1) {
		hRet = m_pDDSurface->Blt( pDestRect,
								  SourceSurface->m_pDDSurface,  
								  pSourceRect,
								  DDBLT_WAIT | DDBLT_KEYSRC,
								  &ddbltfx
								);

  		if (hRet == DD_OK) break;

		if (hRet == DDERR_SURFACELOST) 
		{		
			if (!Restore() || !SourceSurface->Restore()) break;
		}
		else if (hRet != DDERR_WASSTILLDRAWING) 
				break;
	}
}


//----------------------------------------------------------------------
// 현재 surface의 pPoint에 PrimarySurface의 pRect를 blt한다.
//----------------------------------------------------------------------
void	
CDirectDrawSurface::BltPrimarySurface(POINT* pPoint, RECT* pRect)
{
	// TargetSurface의 영역을 넘어가는지에 대한 체크
	if (pPoint->x>=(int)m_ClipRight || pPoint->y>=(int)m_ClipBottom) 
		return;

	// width clipping
	if (pPoint->x<m_ClipLeft) 
	{
		pRect->left = pRect->left-pPoint->x + m_ClipLeft;
		pPoint->x = m_ClipLeft;
	} 
	else if ((int)m_ClipRight-pPoint->x < pRect->right-pRect->left) 
	{
		pRect->right = pRect->left+ m_ClipRight-pPoint->x;
	}

	// height clipping
	if (pPoint->y<m_ClipTop) 
	{
		pRect->top = pRect->top-pPoint->y + m_ClipTop;
		pPoint->y = m_ClipTop;
	} 
	else if ((int)m_ClipBottom-pPoint->y < pRect->bottom-pRect->top) 
	{
		pRect->bottom = pRect->top+ m_ClipBottom-pPoint->y;
	}
	
	// Blit the current frame to the destination surface
	HRESULT hRet;

	// 창모드일 경우에..
	if (!m_bFullscreen)
	{
		//m_pDDSPrimary->Blt(&m_rcScreen, m_pDDSBack, &m_rcViewport, DDBLT_WAIT, NULL);
		pPoint->x += m_rcScreen.left;
		pPoint->y += m_rcScreen.top;

		pRect->left += m_rcScreen.left;
		pRect->right += m_rcScreen.left;
		pRect->top += m_rcScreen.top;
		pRect->bottom += m_rcScreen.top;
	}

	while (1) {
		hRet = m_pDDSurface->BltFast(pPoint->x, pPoint->y,
									m_pDDSPrimary, 
									pRect, 
									DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
		if (hRet == DD_OK) 
		{
			break;
		}

		if (hRet == DDERR_SURFACELOST) 
		{		
			if (!Restore() || !m_pDDSPrimary->Restore()) 
			{
				break;
			}
		}
		else if (hRet != DDERR_WASSTILLDRAWING) 
		{
				break;		
		}
	}
}

//----------------------------------------------------------------------
// ShowFPS
//
//----------------------------------------------------------------------
void 
CDirectDrawSurface::ShowFPS(int x, int y, COLORREF fcolor, COLORREF bcolor)
{
	static DWORD	thisTickTime, oldTickTime;
	static DWORD	frame, fps;
	static char		framebuf[20];

	frame++;
	thisTickTime = GetTickCount();
	if (abs(thisTickTime - oldTickTime) >= 1000)
	{
		fps = frame;
		frame = 0;
		oldTickTime = GetTickCount();
   }
 
   sprintf(framebuf, "FPS = %3d", fps);
	GDI_Text(x, y, framebuf, fcolor, bcolor, false);
}

//----------------------------------------------------------------------
// GDI_Text
// option은 배경 투명 여부이다.
//----------------------------------------------------------------------
void 
CDirectDrawSurface::GDI_Text(int x, int y, const char *str, 
					 COLORREF fcolor, COLORREF bcolor, bool option)
{
	HDC hdc;

	if (!str) return;

   m_pDDSurface->GetDC(&hdc);
   SetTextColor(hdc, fcolor);

	if (option)
		SetBkMode(hdc, TRANSPARENT);
	else
		SetBkColor(hdc, bcolor);

	TextOut(hdc, x, y, str, strlen(str));

	m_pDDSurface->ReleaseDC(hdc);
}


//----------------------------------------------------------------------
// Save BMP
//----------------------------------------------------------------------
//
// NOTE: The FrontBuffer parameter is needed only for 8-bit surfaces so that the system 
//       palette can be accessed, for  15-,16-,24-bit surfaces it can be NULL
//
//----------------------------------------------------------------------
bool 
CDirectDrawSurface::SaveToBMP(const char * szFilename)
{
	HANDLE file_out;

	//------------------------------------------------------------
	// Create the outputfile...
	//------------------------------------------------------------
	file_out = CreateFile(szFilename, 
							GENERIC_WRITE, 
							FILE_SHARE_WRITE, 
							NULL, 
							CREATE_ALWAYS, 
							FILE_ATTRIBUTE_NORMAL, 
							NULL);

	if (file_out == INVALID_HANDLE_VALUE)
	{
		//DirectDrawFailed("Surface - Could not open output file for Bitmap: Screenshot.bmp!");
		return false;
	}
	
	//------------------------------------------------------------
	// Save To File
	//------------------------------------------------------------
	HRESULT           rval;
	DWORD             numwrite;
	BITMAPFILEHEADER  fh;
	BITMAPINFOHEADER  bi;
	DWORD             outpixel;
	int               outbyte, loop, loop2, BufferIndex;
	BYTE              *WriteBuffer; 
	DDSURFACEDESC2    ddsd;
	int              Width, Height, Pitch, Gap;

	//------------------------------------------------------------
	// First we need a ddsdription of the surface
	//------------------------------------------------------------
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	rval = m_pDDSurface->GetSurfaceDesc(&ddsd);
	if (rval != DD_OK)
	{
		  //DirectDrawFailed("Couldn't get surface ddsd for bitmap save");
		  return false; 
	}

	//------------------------------------------------------------
	// Setup output buffer stuff, 
	// since Windows has paging and we're in flat mode, 
	// I just made it as big as the bitmap 
	//------------------------------------------------------------
	BufferIndex = 0;
	Gap = ddsd.dwWidth%2;
	Width = ddsd.dwWidth+Gap;
	Height = ddsd.dwHeight;
	Pitch = ddsd.lPitch;
	WriteBuffer = new BYTE [Width* Height * 3];  //width*height*24-bit

	//------------------------------------------------------------
	// Write the file header
	//------------------------------------------------------------
	((char *)&(fh.bfType))[0] = 'B';
	((char *)&(fh.bfType))[1] = 'M';
	fh.bfSize = (long)(sizeof (BITMAPINFOHEADER)+sizeof (BITMAPFILEHEADER)+Width*Height*3); //Size in BYTES
	fh.bfReserved1 = 0;
	fh.bfReserved2 = 0;
	fh.bfOffBits = sizeof (BITMAPINFOHEADER)+sizeof (BITMAPFILEHEADER);
	bi.biSize = sizeof (BITMAPINFOHEADER);
	bi.biWidth =Width-Gap;
	bi.biHeight =Height;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 10000;
	bi.biYPelsPerMeter = 10000;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	WriteFile (file_out, (char *) &fh, sizeof (BITMAPFILEHEADER),&numwrite,NULL);
	WriteFile (file_out, (char *) &bi, sizeof (BITMAPINFOHEADER),&numwrite,NULL);
	
	//------------------------------------------------------------
	// 32 Bit
	//------------------------------------------------------------
	if (ddsd.ddpfPixelFormat.dwRGBBitCount==32)    
	{
		//lock the surface and start filling the output
		//buffer
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);

		rval = m_pDDSurface->Lock(NULL,&ddsd, DDLOCK_WAIT,NULL);
		if (rval != DD_OK)
		{
			//DirectDrawFailed("Couldn't lock source");
			delete [] WriteBuffer;
			return false;
		}

		BYTE *Bitmap_in = (BYTE*)ddsd.lpSurface;

		for (loop =Height-1;loop>=0;loop--)    //Loop bottom up
		for (loop2=0;loop2<Width;loop2++)
		{
			outpixel = *((DWORD *)(Bitmap_in+loop2*4 + loop * Pitch)); //Load a word

			//Load up the Blue component and output it
			outbyte = (((outpixel)&0x000000ff));//blue
			WriteBuffer [BufferIndex++] = outbyte;

			//Load up the green component and output it 
			outbyte = (((outpixel>>8)&0x000000ff)); 
				WriteBuffer [BufferIndex++] = outbyte;

			//Load up the red component and output it 
			outbyte = (((outpixel>>16)&0x000000ff));
			WriteBuffer [BufferIndex++] = outbyte;
		}


		//At this point the buffer should be full, so just write it out
		WriteFile (file_out, WriteBuffer,BufferIndex,&numwrite,NULL);

		//Now unlock the surface and we're done
		m_pDDSurface->Unlock(NULL);
	}
	//------------------------------------------------------------
	// 24 bit
	//------------------------------------------------------------
	if (ddsd.ddpfPixelFormat.dwRGBBitCount==24) 
	{
		//So easy just lock the surface and output

		//lock the surface and start filling the output
		//buffer

		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);

		rval = m_pDDSurface->Lock(NULL,&ddsd, DDLOCK_WAIT,NULL);
		if (rval != DD_OK)
		{
			//DirectDrawFailed("Couldn't lock source");
			delete [] WriteBuffer;
			return false;
		}

		BYTE *Bitmap_in = (BYTE*)ddsd.lpSurface;

		for (loop =Height-1;loop>=0;loop--)    //Loop bottom up
		for (loop2=0;loop2<Width;loop2++)
		{

			//Load up the Blue component and output it
			WriteBuffer [BufferIndex++] = *(Bitmap_in+loop2*3+2 + loop * Pitch); //Bug fix 6-5

			//Load up the green component and output it 
			WriteBuffer [BufferIndex++] = *(Bitmap_in+loop2*3+ 1 + loop * Pitch); //Bug fix 6-5

			//Load up the red component and output it 
			WriteBuffer [BufferIndex++] = *(Bitmap_in+loop2*3 + loop * Pitch);
		}

		//At this point the buffer should be full, so just write it out
		WriteFile (file_out, WriteBuffer,BufferIndex,&numwrite,NULL);

		//Now unlock the surface and we're done
		m_pDDSurface->Unlock(NULL);
	}
	//------------------------------------------------------------
	// 16 Bit
	//------------------------------------------------------------
	else if (ddsd.ddpfPixelFormat.dwRGBBitCount==16)     
	{
		//lock the surface and start filling the output
		//buffer
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);

		rval = m_pDDSurface->Lock(NULL,&ddsd, DDLOCK_WAIT,NULL);
		if (rval != DD_OK)
		{
				//DirectDrawFailed("Couldn't lock source");
				delete [] WriteBuffer;
				return false;
		}

		BYTE *Bitmap_in = (BYTE*)ddsd.lpSurface;

		/*
		According to DirectX docs, dwRGBBitCount is 2,4,8,16,24,32, BUT what about 15-bit surfaces
		(5,5,5) I don't really know if its needed but here we check the green bitmask to see
		if 5 or 6 bits are used for green.

		If the green bitmask equals 565 mode, do 16-bit mode, otherwise do 15-bit mode
		NOTE: We are reversing the component order (ie. BGR instead of RGB)
			and we are outputting it bottom up because BMP files are backwards and upside down.
		*/

		//------------
		// 5:6:5
		//------------
		if (ddsd.ddpfPixelFormat.dwGBitMask == 0x07E0)
		{
			for (loop =Height-1;loop>=0;loop--)    //Loop bottom up
			{
				for (loop2=0;loop2<Width;loop2++)
				{

					if(loop2 >= Width-Gap)
					{
						WriteBuffer [BufferIndex++] = NULL;
						WriteBuffer [BufferIndex++] = NULL;
						WriteBuffer [BufferIndex++] = NULL;
					}
					else
					{
						
						outpixel = *((WORD *)(Bitmap_in+(loop2<<1) + loop * Pitch)); //Load a word
						
						//Load up the Blue component and output it
						outbyte = (((outpixel)&0x001f)<<3);//blue
						WriteBuffer [BufferIndex++] = outbyte;
						
						//Load up the green component and output it 
						outbyte = (((outpixel>>5)&0x003f)<<2); 
						WriteBuffer [BufferIndex++] = outbyte;
						
						//Load up the red component and output it 
						outbyte = (((outpixel>>11)&0x001f)<<3);
						WriteBuffer [BufferIndex++] = outbyte;
					}
				}
			}
		}
		//----------
		// 5:5:5
		//----------
		else //Assume 555 mode. 15-bit mode
		{
			for (loop =Height-1;loop>=0;loop--)    //Loop bottom up
			{
				for (loop2=0;loop2<Width;loop2++)
				{
					if(loop2 >= Width-Gap)
					{
						WriteBuffer [BufferIndex++] = NULL;
						WriteBuffer [BufferIndex++] = NULL;
						WriteBuffer [BufferIndex++] = NULL;
					}
					else
					{
						
						outpixel = *((WORD *)(Bitmap_in+loop2*2 + loop * Pitch)); //Load a word
						
						//Load up the Blue component and output it
						outbyte = (8*((outpixel)&0x001f));//blue
						WriteBuffer [BufferIndex++] = outbyte;
						
						//Load up the green component and output it 
						outbyte = (8*((outpixel>>5)&0x001f)); 
						WriteBuffer [BufferIndex++] = outbyte;
						
						//Load up the red component and output it 
						outbyte = (8*((outpixel>>10)&0x001f));  //BUG FIX here
						WriteBuffer [BufferIndex++] = outbyte;
					}
				}
			}
		}

		//At this point the buffer should be full, so just write it out
		WriteFile (file_out, WriteBuffer,BufferIndex,&numwrite,NULL);

		//Now unlock the surface and we're done
		m_pDDSurface->Unlock(NULL);

	}
	//------------------------------------------------------------
	// 8 Bit
	//------------------------------------------------------------
	else if (ddsd.ddpfPixelFormat.dwRGBBitCount==8) //8 bit surfaces
	{
		//Get the system palette so we can index each pixel to its corresponding color, this
		//is what the m_pDDSPrimary parameter is needed for

		if (m_pDDSPrimary == NULL)
		{
			//DirectDrawFailed("No Front Buffer for 8-bit BMP save");
			delete [] WriteBuffer;
			return false;
		}

		LPDIRECTDRAWPALETTE Pal;
		char bytepal [256*4];

		rval = m_pDDSPrimary->GetPalette(&Pal);
		if (rval != DD_OK)
		{
			//DirectDrawFailed("Surface - Couldn't get palette for 8-bit Bitmap Save");
			delete [] WriteBuffer;
			return false;
		}

		Pal->GetEntries (0,0,256,(tagPALETTEENTRY *)&(bytepal[0]));
		Pal->Release();

		//lock the surface and start filling the output
		//buffer

		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		rval = m_pDDSurface->Lock(NULL,&ddsd, DDLOCK_WAIT,NULL);
		if (rval != DD_OK)
		{
			//DirectDrawFailed("Couldn't lock source");
			delete [] WriteBuffer;
			return false;
		}

		BYTE *Bitmap_in = (BYTE*)ddsd.lpSurface;


		//Ok, now that we've got the palette and the 24-bit entries, we just look up the color and output it
		//NOTE: At the same time we are reversing the component order (ie. BGR instead of RGB)
		//      and we are outputting it bottom up. 
		for (loop =Height-1;loop>=0;loop--)    //Loop bottom up
		{
			for (loop2=0;loop2<Width;loop2++)
			{
				outpixel = *(Bitmap_in+loop2 + loop * Pitch); //Load a byte from the surface

				//Load up the Blue component and output it
				outbyte = bytepal[outpixel*4+2];//blue
				WriteBuffer [BufferIndex++] = outbyte;

				//Load up the Green component and output it
				outbyte = bytepal[outpixel*4+1];//green
				WriteBuffer [BufferIndex++] = outbyte;

				//Load up the Red component and output it
				outbyte = bytepal[outpixel*4];//red
				WriteBuffer [BufferIndex++] = outbyte;
			}
		}

		//At this point the buffer should be full, so just write it out
		WriteFile (file_out, WriteBuffer,BufferIndex,&numwrite,NULL);

		//Now unlock the surface and we're done
		m_pDDSurface->Unlock(NULL);
	}

	delete [] WriteBuffer;


	CloseHandle(file_out);

	return true;
}
