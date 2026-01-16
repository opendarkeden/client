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
	, m_transparency(0)
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
	m_transparency = 0;
}

/* ============================================================================
 * DirectX Compatibility Methods
 * ============================================================================ */

bool CSpriteSurface::InitOffsurface(int width, int height)
{
	/* Off-screen surface is the same as a regular surface in SDL2 */
	return Init(width, height);
}

void CSpriteSurface::SetTransparency(int value)
{
	m_transparency = value;
}

int CSpriteSurface::GetTransparency() const
{
	return m_transparency;
}

void CSpriteSurface::GDI_Text(int x, int y, const char* text, DWORD color)
{
	/* TODO: Implement text rendering using SDL2_ttf or similar
	 * For now, this is a stub - text rendering is not implemented in SDL backend
	 */
	(void)x; (void)y; (void)text; (void)color;
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

void CSpriteSurface::BltHalf(POINT* pPoint, CSpriteSurface* SourceSurface, RECT* pRect)
{
	/* TODO: Implement */
}

void CSpriteSurface::BltNoColorkey(POINT* pPoint, CSpriteSurface* SourceSurface, RECT* pRect)
{
	/* TODO: Implement BltNoColorkey for SDL backend */
	// Stub: Currently not implemented for SDL backend
}

void CSpriteSurface::BltDarkness(POINT* pPoint, CSpriteSurface* SourceSurface, RECT* pRect, BYTE DarkBits)
{
	/* TODO: Implement */
}

void CSpriteSurface::BltBrightness(POINT* pPoint, CSpriteSurface* SourceSurface, RECT* pRect, BYTE BrightBits)
{
	/* TODO: Implement */
}

void CSpriteSurface::BltDarknessFilter(POINT* pPoint, CSpriteSurface* SourceSurface, RECT* pRect, WORD TransColor)
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

/* ============================================================================
 * Lock/Unlock Methods (Stub implementations for compatibility)
 * ============================================================================ */

bool CSpriteSurface::LockSDL()
{
	/* Stub: For SDL backend, we don't need explicit locking
	 * The spritectl_lock_surface is called internally when needed
	 */
	return true;
}

void CSpriteSurface::UnlockSDL()
{
	/* Stub: No explicit unlocking needed for SDL backend */
}

bool CSpriteSurface::IsLock()
{
	/* Stub: Surface is never in locked state in SDL backend */
	return false;
}

/* ============================================================================
 * Get Surface Info (for compatibility with Windows code)
 * ============================================================================ */

void CSpriteSurface::GetSurfaceInfo(S_SURFACEINFO* info)
{
	/* Fill surface info structure for SDL backend */
	if (m_backend_surface == SPRITECTL_INVALID_SURFACE) {
		info->p_surface = nullptr;
		info->width = 0;
		info->height = 0;
		info->pitch = 0;
		return;
	}
	
	/* Lock surface to get info */
	spritectl_surface_info_t sdl_info;
	if (spritectl_lock_surface(m_backend_surface, &sdl_info) == 0) {
		info->p_surface = sdl_info.pixels;
		info->width = sdl_info.width;
		info->height = sdl_info.height;
		info->pitch = sdl_info.pitch;
		spritectl_unlock_surface(m_backend_surface);
	} else {
		info->p_surface = nullptr;
		info->width = m_width;
		info->height = m_height;
		info->pitch = m_width * 2; /* RGB565 = 2 bytes per pixel */
	}
}

/* ============================================================================
 * GetDDSD Compatibility Wrapper (for Windows API compatibility)
 * ============================================================================ */

S_SURFACEINFO* CSpriteSurface::GetDDSD()
{
	/* Static buffer for surface info - returned as pointer for compatibility */
	static S_SURFACEINFO ddsd_buffer;
	GetSurfaceInfo(&ddsd_buffer);
	return &ddsd_buffer;
}

/* ============================================================================
 * Clipping Methods (compatibility with CDirectDrawSurface)
 * ============================================================================ */

void CSpriteSurface::SetClip(RECT* rect)
{
	/* Stub: SDL backend doesn't use clipping rectangles in the same way */
	/* SDL uses SDL_Rect for clipping with SDL_RenderSetClipRect */
}

void CSpriteSurface::SetClipNULL()
{
	/* Stub: Reset clipping - not applicable for SDL backend */
}

/* ============================================================================
 * Blt Method (compatibility with CDirectDrawSurface)
 * ============================================================================ */

void CSpriteSurface::Blt(POINT* pPoint, CSpriteSurface* SourceSurface, RECT* pRect)
{
	/* Stub: Basic blit from source surface to this surface */
	/* In practice, this should copy pixels from SourceSurface to this surface */
	if (!pPoint || !SourceSurface) {
		return;
	}

	/* Get source and destination info */
	S_SURFACEINFO src_info, dst_info;
	SourceSurface->GetSurfaceInfo(&src_info);
	this->GetSurfaceInfo(&dst_info);

	if (!src_info.p_surface || !dst_info.p_surface) {
		return;
	}

	/* Calculate dimensions */
	int src_x = pRect ? pRect->left : 0;
	int src_y = pRect ? pRect->top : 0;
	int src_w = pRect ? (pRect->right - pRect->left) : src_info.width;
	int src_h = pRect ? (pRect->bottom - pRect->top) : src_info.height;

	/* Clamp to source surface bounds */
	if (src_x + src_w > src_info.width) src_w = src_info.width - src_x;
	if (src_y + src_h > src_info.height) src_h = src_info.height - src_y;

	/* Clamp to destination surface bounds */
	if (pPoint->x + src_w > dst_info.width) src_w = dst_info.width - pPoint->x;
	if (pPoint->y + src_h > dst_info.height) src_h = dst_info.height - pPoint->y;

	/* Copy pixels line by line */
	WORD* src_pixels = (WORD*)src_info.p_surface;
	WORD* dst_pixels = (WORD*)dst_info.p_surface;

	for (int y = 0; y < src_h; y++) {
		for (int x = 0; x < src_w; x++) {
			int src_offset = (src_y + y) * (src_info.pitch / 2) + (src_x + x);
			int dst_offset = (pPoint->y + y) * (dst_info.pitch / 2) + (pPoint->x + x);
			dst_pixels[dst_offset] = src_pixels[src_offset];
		}
	}
}

/* ============================================================================
 * FillSurface Method (compatibility with CDirectDrawSurface)
 * ============================================================================ */

void CSpriteSurface::FillSurface(WORD color)
{
	/* Fill the entire surface with the specified color */
	if (m_backend_surface == SPRITECTL_INVALID_SURFACE) {
		return;
	}

	/* Lock surface to get pixel data */
	spritectl_surface_info_t info;
	if (spritectl_lock_surface(m_backend_surface, &info) == 0) {
		WORD* pixels = (WORD*)info.pixels;
		int pixel_count = info.width * info.height;

		/* Fill all pixels with the specified color */
		for (int i = 0; i < pixel_count; i++) {
			pixels[i] = color;
		}

		spritectl_unlock_surface(m_backend_surface);
	}
}

/* ============================================================================
 * Gamma Correction
 * ============================================================================ */

void CSpriteSurface::Gamma4Pixel565(void *pDest, int len, int p)
{
	// TODO: [SDL_BACKEND] Implement optimized gamma correction algorithm
	// Current implementation uses basic RGB scaling
	// Original Windows implementation uses x86 assembly
	WORD* dest = (WORD*)pDest;
	int light = p;
	
	for (int i = 0; i < len; i++) {
		WORD pixel = dest[i];
		
		// Extract RGB565 components
		int r = (pixel >> 11) & 0x1F;
		int g = (pixel >> 5) & 0x3F;
		int b = pixel & 0x1F;
		
		// Apply gamma correction
		r = (r * light) >> 5;
		g = (g * light) >> 5;
		b = (b * light) >> 5;
		
		// Clamp values
		if (r > 31) r = 31;
		if (g > 63) g = 63;
		if (b > 31) b = 31;
		
		// Recombine to RGB565
		dest[i] = (r << 11) | (g << 5) | b;
	}
}

/* ============================================================================
 * Gamma4Pixel555 - Alias to Gamma4Pixel565 for compatibility
 * RGB555 and RGB565 have same structure (5-6-5 vs 5-5-5)
 * SDL backend uses RGB565 only, so Gamma4Pixel555 maps to Gamma4Pixel565
 * ============================================================================ */
void CSpriteSurface::Gamma4Pixel555(void *pDest, int len, int p)
{
	// RGB555 and RGB565 are structurally similar
	// In SDL backend, we always use RGB565, so just call Gamma4Pixel565
	Gamma4Pixel565(pDest, len, p);
}
