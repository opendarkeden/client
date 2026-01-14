/*-----------------------------------------------------------------------------

	CSpriteSurface_SDL.cpp

	SDL2 backend implementation for CSpriteSurface.
	This file contains all CSpriteSurface methods implemented using SpriteLibBackend.
	Does not depend on CDirectDraw or any Windows-specific code.

	2025.01.14

-----------------------------------------------------------------------------*/

#ifdef SPRITELIB_BACKEND_SDL

#include "client_PCH.h"
#include "CSprite.h"
#include "CAlphaSprite.h"
#include "CIndexSprite.h"
#include "CShadowSprite.h"
#include "CSpriteOutlineManager.h"
#include "CFilter.h"
#include "CSpriteSurface.h"
#include "SpriteLibBackend.h"

/* ============================================================================
 * Static Member Initialization
 * ============================================================================ */

int CSpriteSurface::s_Value1 = 1;
int CSpriteSurface::s_Value2 = 31;
int CSpriteSurface::s_Value3 = 1;

FUNCTION_MEMCPYEFFECT CSpriteSurface::s_pMemcpyEffectFunction = NULL;
FUNCTION_MEMCPYEFFECT CSpriteSurface::s_pMemcpyEffectFunctionTable[MAX_EFFECT] = {0};
FUNCTION_MEMCPYPALEFFECT CSpriteSurface::s_pMemcpyPalEffectFunction = NULL;
FUNCTION_MEMCPYPALEFFECT CSpriteSurface::s_pMemcpyPalEffectFunctionTable[MAX_EFFECT] = {0};

WORD CSpriteSurface::s_EffectScreenTableR[32][32] = {0};
WORD CSpriteSurface::s_EffectScreenTableG[32][32] = {0};
WORD CSpriteSurface::s_EffectScreenTableB[32][32] = {0};

/* ============================================================================
 * Constructor / Destructor
 * ============================================================================ */

CSpriteSurface::CSpriteSurface()
	: m_backend_surface(SPRITECTL_INVALID_SURFACE)
	, m_width(0)
	, m_height(0)
{
}

CSpriteSurface::~CSpriteSurface()
{
	Release();
}

/* ============================================================================
 * Surface Initialization
 * ============================================================================ */

bool CSpriteSurface::Init(int width, int height)
{
	/* Cleanup existing surface */
	Release();

	/* Create backend surface */
	m_backend_surface = spritectl_create_surface(width, height, SPRITECTL_FORMAT_RGB565);
	if (m_backend_surface == SPRITECTL_INVALID_SURFACE) {
		return false;
	}

	m_width = width;
	m_height = height;
	return true;
}

bool CSpriteSurface::InitFromFile(const char* filename)
{
	/* TODO: Load from BMP file */
	return false;
}

void CSpriteSurface::Release()
{
	if (m_backend_surface != SPRITECTL_INVALID_SURFACE) {
		spritectl_destroy_surface(m_backend_surface);
		m_backend_surface = SPRITECTL_INVALID_SURFACE;
	}
	m_width = 0;
	m_height = 0;
}

/* ============================================================================
 * Drawing Functions
 * ============================================================================ */

void CSpriteSurface::DrawRect(RECT* rect, WORD color)
{
	if (!rect || m_backend_surface == SPRITECTL_INVALID_SURFACE) {
		return;
	}

	/* Convert RGB565 color to RGBA32 */
	uint32_t rgba_color;
	spritectl_convert_565_to_rgba(&color, &rgba_color, 1, 0);

	/* Fill rect using backend */
	SDL_Rect sdl_rect;
	sdl_rect.x = rect->left;
	sdl_rect.y = rect->top;
	sdl_rect.w = rect->right - rect->left;
	sdl_rect.h = rect->bottom - rect->top;

	spritectl_surface_info_t info;
	if (spritectl_lock_surface(m_backend_surface, &info) == 0) {
		SDL_Surface* surf = (SDL_Surface*)info.pixels;  /* Cast to SDL surface */
		SDL_FillRect(surf, &sdl_rect, rgba_color);
		spritectl_unlock_surface(m_backend_surface);
	}
}

void CSpriteSurface::FillRect(RECT* rect, WORD color)
{
	DrawRect(rect, color);
}

void CSpriteSurface::HLine(int x, int y, int length, WORD color)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + length;
	rect.bottom = y + 1;
	DrawRect(&rect, color);
}

void CSpriteSurface::VLine(int x, int y, int length, WORD color)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + 1;
	rect.bottom = y + length;
	DrawRect(&rect, color);
}

void CSpriteSurface::Line(int x1, int y1, int x2, int y2, WORD color)
{
	/* TODO: Implement line drawing */
	/* For now, just draw horizontal/vertical lines */
	if (y1 == y2) {
		int x = (x1 < x2) ? x1 : x2;
		int len = (x2 - x1);
		if (len < 0) len = -len;
		HLine(x, y1, len, color);
	} else if (x1 == x2) {
		int y = (y1 < y2) ? y1 : y2;
		int len = (y2 - y1);
		if (len < 0) len = -len;
		VLine(x1, y, len, color);
	}
}

/* ============================================================================
 * Stub Implementations (TODO)
 * ============================================================================ */

void CSpriteSurface::BltHalf(POINT* pPoint, class CDirectDrawSurface* SourceSurface, RECT* pRect)
{
	/* TODO: Implement */
}

void CSpriteSurface::BltDarkness(POINT* pPoint, class CDirectDrawSurface* SourceSurface, RECT* pRect, BYTE DarkBits)
{
	/* TODO: Implement */
}

void CSpriteSurface::BltBrightness(POINT* pPoint, class CDirectDrawSurface* SourceSurface, RECT* pRect, BYTE BrightBits)
{
	/* TODO: Implement */
}

void CSpriteSurface::BltDarknessFilter(POINT* pPoint, class CDirectDrawSurface* SourceSurface, RECT* pRect, WORD TransColor)
{
	/* TODO: Implement */
}

void CSpriteSurface::ChangeBrightnessBit(RECT* pRect, BYTE DarkBits)
{
	/* TODO: Implement */
}

void CSpriteSurface::BltColorAlpha(RECT* pRect, WORD color, BYTE alpha2)
{
	/* TODO: Implement */
}

void CSpriteSurface::InitEffectTable()
{
	/* Initialize effect tables if needed */
}

void CSpriteSurface::memcpyHalf(WORD* pDest, WORD* pSource, WORD pixels)
{
	/* TODO: Implement */
}

void CSpriteSurface::memcpyAlpha(WORD* pDest, WORD* pSource, WORD pixels)
{
	/* TODO: Implement */
}

void CSpriteSurface::memcpyColor(WORD* pDest, WORD* pSource, WORD pixels)
{
	/* TODO: Implement */
}

void CSpriteSurface::memcpyScale(WORD* pDest, WORD destPitch, WORD* pSource, WORD pixels)
{
	/* TODO: Implement */
}

void CSpriteSurface::memcpyDarkness(WORD* pDest, WORD* pSource, WORD pixels)
{
	/* TODO: Implement */
}

void CSpriteSurface::memcpyBrightness(WORD* pDest, WORD* pSource, WORD pixels)
{
	/* TODO: Implement */
}

/* ============================================================================
 * Clipping Helper
 * ============================================================================ */

bool CSpriteSurface::ClippingRectToPoint(RECT*& pRect, POINT*& pPoint)
{
	/* TODO: Implement clipping */
	return true;
}

/* ============================================================================
 * Include Adapter Implementation (BltSprite methods)
 * ============================================================================ */

/* Include the adapter code for BltSprite methods */
#include "CSpriteSurface_Adapter.cpp"

#endif /* SPRITELIB_BACKEND_SDL */
