//----------------------------------------------------------------------
// CDirectDrawSurface.cpp
//
// SDL2 Implementation (Cross-platform)
// Windows DirectDraw implementation removed - using SDL2 on all platforms
// NOTE: This class is deprecated. Use SDL_Surface directly instead.
//----------------------------------------------------------------------

#include "CDirectDrawSurface.h"

//-----------------------------------------------------------------------------
// Static member initialization for SDL2
//-----------------------------------------------------------------------------
void (*CDirectDrawSurface::s_GammaFunction)(void *pDest, int len, int p) = nullptr;

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------

CDirectDrawSurface::CDirectDrawSurface()
{
	m_pDDSurface = nullptr;
	m_Width = 0;
	m_Height = 0;
	m_ClipLeft = 0;
	m_ClipTop = 0;
	m_ClipRight = 0;
	m_ClipBottom = 0;
	m_bBackSurface = false;
	m_bLock = false;
}

CDirectDrawSurface::~CDirectDrawSurface()
{
	// Stub - SDL surfaces are managed elsewhere
}

//-----------------------------------------------------------------------------
// Surface Initialization (Stubs - deprecated)
//-----------------------------------------------------------------------------

bool CDirectDrawSurface::InitBacksurface()
{
	// Not implemented - use SDL surfaces directly
	return false;
}

bool CDirectDrawSurface::InitOffsurface(int wWidth, int wHeight, DWORD dwCaps)
{
	m_Width = wWidth;
	m_Height = wHeight;
	m_ClipRight = wWidth;
	m_ClipBottom = wHeight;
	(void)dwCaps;
	return true;
}

bool CDirectDrawSurface::InitTextureSurface(int wWidth, int wHeight, DWORD dwCaps, const LPDDPIXELFORMAT pDDPF)
{
	// Not implemented - use SDL textures directly
	(void)wWidth;
	(void)wHeight;
	(void)dwCaps;
	(void)pDDPF;
	return false;
}

bool CDirectDrawSurface::InitTextureSurfaceFromBMP(LPCSTR szBitmap, DWORD dwCaps)
{
	// Not implemented - use SDL_image to load BMPs
	(void)szBitmap;
	(void)dwCaps;
	return false;
}

bool CDirectDrawSurface::InitFromBMP(LPCSTR szBitmap, DWORD dwCaps)
{
	// Not implemented - use SDL_image to load BMPs
	(void)szBitmap;
	(void)dwCaps;
	return false;
}

bool CDirectDrawSurface::ReLoadBMP(LPCSTR szBitmap)
{
	// Not implemented
	(void)szBitmap;
	return false;
}

bool CDirectDrawSurface::Restore()
{
	// Not implemented - SDL surfaces don't need restore
	return false;
}

//-----------------------------------------------------------------------------
// Surface Operations (Stubs - deprecated)
//-----------------------------------------------------------------------------

void CDirectDrawSurface::SetTransparency(DWORD dwValue)
{
	// Not implemented - use SDL blend modes instead
	(void)dwValue;
}

void CDirectDrawSurface::FillSurface(WORD color)
{
	// Not implemented - use SDL_FillRect instead
	(void)color;
}

void CDirectDrawSurface::FillRect(RECT* pRect, WORD color)
{
	// Not implemented - use SDL_FillRect instead
	(void)pRect;
	(void)color;
}

//-----------------------------------------------------------------------------
// Lock/Unlock (Minimal stub for compatibility)
//-----------------------------------------------------------------------------

bool CDirectDrawSurface::Lock()
{
	ZeroMemory(&m_ddsd, sizeof(m_ddsd));
	m_ddsd.dwSize = sizeof(m_ddsd);
	m_ddsd.lPitch = m_Width * 2;  // Assume 16-bit color
	m_ddsd.lpSurface = nullptr;  // No actual surface memory
	m_bLock = true;
	return true;
}

bool CDirectDrawSurface::Unlock()
{
	m_bLock = false;
	return true;
}

void CDirectDrawSurface::LockW(WORD*& lpSurface, WORD& lPitch)
{
	// Stub - no actual surface to lock
	lpSurface = nullptr;
	lPitch = m_Width * 2;
	m_bLock = true;
}

//-----------------------------------------------------------------------------
// Blitting (Stubs - deprecated)
//-----------------------------------------------------------------------------

void CDirectDrawSurface::Blt(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT* pRect)
{
	// Not implemented - use SDL_BlitSurface instead
	(void)pPoint;
	(void)SourceSurface;
	(void)pRect;
}

void CDirectDrawSurface::BltNoColorkey(POINT* pPoint, CDirectDrawSurface* SourceSurface, RECT* pRect)
{
	// Not implemented - use SDL_BlitSurface instead
	(void)pPoint;
	(void)SourceSurface;
	(void)pRect;
}

void CDirectDrawSurface::Blt(RECT* pDestRect, CDirectDrawSurface* SourceSurface, RECT* pSourceRect)
{
	// Not implemented - use SDL_BlitSurface instead
	(void)pDestRect;
	(void)SourceSurface;
	(void)pSourceRect;
}

void CDirectDrawSurface::BltPrimarySurface(POINT* pPoint, RECT* pRect)
{
	// Not implemented
	(void)pPoint;
	(void)pRect;
}

//-----------------------------------------------------------------------------
// Gamma Correction (Not implemented - use shader effects instead)
//-----------------------------------------------------------------------------

void CDirectDrawSurface::GammaBox555(RECT* pRect, int p)
{
	// Not implemented - use SDL2 shader effects
	(void)pRect;
	(void)p;
}

void CDirectDrawSurface::GammaBox565(RECT* pRect, int p)
{
	// Not implemented - use SDL2 shader effects
	(void)pRect;
	(void)p;
}

void CDirectDrawSurface::Gamma4Pixel565(void *pDest, int len, int p)
{
	// Not implemented - use SDL2 shader effects
	(void)pDest;
	(void)len;
	(void)p;
}

void CDirectDrawSurface::Gamma4Pixel555(void *pDest, int len, int p)
{
	// Not implemented - use SDL2 shader effects
	(void)pDest;
	(void)len;
	(void)p;
}

//-----------------------------------------------------------------------------
// GDI Text (Not implemented - use SDL2 text rendering instead)
//-----------------------------------------------------------------------------

void CDirectDrawSurface::ShowFPS(int x, int y, COLORREF fcolor, COLORREF bcolor)
{
	// Not implemented - use SDL2 text rendering
	(void)x;
	(void)y;
	(void)fcolor;
	(void)bcolor;
}

void CDirectDrawSurface::GDI_Text(int x, int y, const char *str, COLORREF fcolor, COLORREF bcolor, bool option)
{
	// Not implemented - use SDL2 text rendering
	(void)x;
	(void)y;
	(void)str;
	(void)fcolor;
	(void)bcolor;
	(void)option;
}

//-----------------------------------------------------------------------------
// BMP Save/Load (Not implemented - use SDL_image instead)
//-----------------------------------------------------------------------------

bool CDirectDrawSurface::SaveToBMP(const char* szFilename)
{
	// Not implemented - use SDL_image PNG/BMP saving
	(void)szFilename;
	return false;
}

bool CDirectDrawSurface::CopyBitmap(HBITMAP hbm, int x, int y, int dx, int dy)
{
	// Not implemented - Windows GDI only
	(void)hbm;
	(void)x;
	(void)y;
	(void)dx;
	(void)dy;
	return false;
}
