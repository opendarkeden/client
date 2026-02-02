//----------------------------------------------------------------------
// CDirectDraw.cpp
//
// SDL2 Implementation (Cross-platform)
// Windows DirectX implementation removed - using SDL2 on all platforms
// NOTE: Static member definitions are in CDirectDraw_StaticMembers.cpp
//----------------------------------------------------------------------

#include "CDirectDraw.h"

//-----------------------------------------------------------------------------
// Static member initialization for DirectDraw objects
//-----------------------------------------------------------------------------
#ifdef PLATFORM_WINDOWS
// Keep Windows types for compatibility on Windows builds
LPDIRECTDRAW7					CDirectDraw::m_pDD					= NULL;
LPDIRECTDRAWSURFACE7			CDirectDraw::m_pDDSPrimary			= NULL;
LPDIRECTDRAWSURFACE7			CDirectDraw::m_pDDSBack				= NULL;
LPDIRECTDRAWGAMMACONTROL	CDirectDraw::m_pDDGammaControl		= NULL;
#else
// On non-Windows platforms, these are stub types
LPDIRECTDRAW7					CDirectDraw::m_pDD					= NULL;
LPDIRECTDRAWSURFACE7			CDirectDraw::m_pDDSPrimary			= NULL;
LPDIRECTDRAWSURFACE7			CDirectDraw::m_pDDSBack				= NULL;
LPDIRECTDRAWGAMMACONTROL	CDirectDraw::m_pDDGammaControl		= NULL;
#endif

HWND								CDirectDraw::m_hWnd					= NULL;

bool								CDirectDraw::m_bFullscreen			= true;
WORD								CDirectDraw::m_ScreenWidth			= 0;
WORD								CDirectDraw::m_ScreenHeight			= 0;
bool								CDirectDraw::m_b565					= true;
bool								CDirectDraw::m_b3D					= true;
bool								CDirectDraw::m_bMMX					= false;
bool								CDirectDraw::m_bGammaControl		= false;
DDGAMMARAMP						CDirectDraw::m_DDGammaRamp;
WORD								CDirectDraw::m_GammaStep				= 0;
WORD								CDirectDraw::m_AddGammaStep[3];

RECT								CDirectDraw::m_rcWindow;
RECT								CDirectDraw::m_rcScreen;
RECT								CDirectDraw::m_rcViewport;

// Note: Color mask static members are defined in CDirectDraw_StaticMembers.cpp

//-----------------------------------------------------------------------------
// Constructor/Destructor (stub - not implemented)
//-----------------------------------------------------------------------------
CDirectDraw::CDirectDraw()
{
}

CDirectDraw::~CDirectDraw()
{
}

//-----------------------------------------------------------------------------
// InitMask
//-----------------------------------------------------------------------------
void CDirectDraw::InitMask(bool b565)
{
	// 5:6:5 format for SDL2
	s_wMASK_SHIFT[0] = 11;
	s_wMASK_SHIFT[1] = 5;
	s_wMASK_SHIFT[2] = 0;
	s_wMASK_SHIFT[3] = 0;
	s_wMASK_SHIFT[4] = 0;

	s_dwMASK_SHIFT[0] = 0xF800;
	s_dwMASK_SHIFT[1] = 0x07E0;
	s_dwMASK_SHIFT[2] = 0x001F;
	s_dwMASK_SHIFT[3] = 0;
	s_dwMASK_SHIFT[4] = 0;

	s_wMASK_RGB[0] = 0;
	s_wMASK_RGB[1] = 11;
	s_wMASK_RGB[2] = 5;
	s_wMASK_RGB[3] = 0;
	s_wMASK_RGB[4] = 0;
	s_wMASK_RGB[5] = 0;

	s_dwMASK_RGB[0] = 0x0000F800;
	s_dwMASK_RGB[1] = 0x000007E0;
	s_dwMASK_RGB[2] = 0x0000001F;
	s_dwMASK_RGB[3] = 0;
	s_dwMASK_RGB[4] = 0;
	s_dwMASK_RGB[5] = 0;

	s_qwMASK_RGB[0] = 0x000000000000F800;
	s_qwMASK_RGB[1] = 0x0000000000007E0;
	s_qwMASK_RGB[2] = 0x00000000000001F;
	s_qwMASK_RGB[3] = 0;
	s_qwMASK_RGB[4] = 0;
	s_qwMASK_RGB[5] = 0;

	s_bSHIFT_R = 3;
	s_bSHIFT_G = 2;
	s_bSHIFT_B = 3;
	s_bSHIFT_A = 4;

	s_dwMASK_SHIFT_COUNT[0] = 5;
	s_dwMASK_SHIFT_COUNT[1] = 6;
	s_dwMASK_SHIFT_COUNT[2] = 5;
	s_dwMASK_SHIFT_COUNT[3] = 0;
	s_dwMASK_SHIFT_COUNT[4] = 0;

	s_dwMASK_RGB_COUNT[0] = 0;
	s_dwMASK_RGB_COUNT[1] = 5;
	s_dwMASK_RGB_COUNT[2] = 6;
	s_dwMASK_RGB_COUNT[3] = 0;
	s_dwMASK_RGB_COUNT[4] = 0;
	s_dwMASK_RGB_COUNT[5] = 0;

	(void)b565;  // Parameter kept for compatibility
}

//-----------------------------------------------------------------------------
// Bitmask methods for SDL2
//-----------------------------------------------------------------------------

int CDirectDraw::Get_Count_Rbit()
{
	// For 5:6:5 format, R uses 5 bits
	return 5;
}

int CDirectDraw::Get_Count_Gbit()
{
	// For 5:6:5 format, G uses 6 bits
	return 6;
}

int CDirectDraw::Get_Count_Bbit()
{
	// For 5:6:5 format, B uses 5 bits
	return 5;
}

DWORD CDirectDraw::Get_R_Bitmask()
{
	// 5:6:5 format: R is at bits 11-15
	return 0xF800;
}

DWORD CDirectDraw::Get_G_Bitmask()
{
	// 5:6:5 format: G is at bits 5-10
	return 0x07E0;
}

DWORD CDirectDraw::Get_B_Bitmask()
{
	// 5:6:5 format: B is at bits 0-4
	return 0x001F;
}

DWORD CDirectDraw::Get_BPP()
{
	// SDL2 typically uses 16-bit color
	return 16;
}
