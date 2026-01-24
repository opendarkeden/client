/*-----------------------------------------------------------------------------

	CSpriteSurface_Adapter.cpp

	SDL2 backend adapter implementation for CSpriteSurface.
	This file contains all BltSprite* methods implemented using SpriteLibBackend.

	NOTE: This file is included by CSpriteSurface_SDL.cpp
	      Do not compile separately.

	2025.01.14

-----------------------------------------------------------------------------*/

/* No #ifdef SPRITELIB_BACKEND_SDL here - included by SDL-only file */

#include "client_PCH.h"
#include "CSprite.h"
#include "CAlphaSprite.h"
#include "CIndexSprite.h"
#include "CShadowSprite.h"
#include "CSpriteSurface.h"
#include "CFilter.h"

#include "SpriteLibBackend.h"

/* ============================================================================
 * Helper Functions
 * ============================================================================ */

/**
 * Get or create backend sprite from CSprite
 * Handles lazy creation and synchronization
 */
static spritectl_sprite_t get_backend_sprite(CSprite* pSprite)
{
	if (!pSprite || !pSprite->IsInit()) {
		return SPRITECTL_INVALID_SPRITE;
	}

	/* Lazy creation: create backend sprite if doesn't exist */
	if (pSprite->GetBackendSprite() == SPRITECTL_INVALID_SPRITE) {
		WORD width = pSprite->GetWidth();
		WORD height = pSprite->GetHeight();

		size_t pixel_count = width * height;
		size_t data_size = pixel_count * sizeof(WORD);

		/* Allocate and decompress RLE pixel data */
		WORD* pixels = (WORD*)malloc(data_size);
		if (!pixels) {
			return SPRITECTL_INVALID_SPRITE;
		}

		/* Decompress RLE data line by line */
		for (WORD y = 0; y < height; y++) {
			WORD* src_line = pSprite->GetPixelLine(y);
			WORD* dst_line = pixels + (y * width);

			/* RLE decompression */
			WORD* pSrc = src_line;
			WORD* pDest = dst_line;
			int count = *pSrc++;  /* Number of runs */

			if (count > 0) {
				for (int j = 0; j < count; j++) {
					pDest += *pSrc++;  /* Skip transparent pixels */
					int colorCount = *pSrc++;  /* Number of color pixels */

					/* Copy color pixels */
					memcpy(pDest, pSrc, colorCount * sizeof(WORD));
					pDest += colorCount;
					pSrc += colorCount;
				}
			}
		}

		/* Create backend sprite */
		spritectl_sprite_t new_sprite = spritectl_create_sprite(
			width, height, SPRITECTL_FORMAT_RGB565,
			pixels, data_size);

		free(pixels);
		pSprite->SetBackendSprite(new_sprite);
		pSprite->SetBackendDirty(false);
	}
	/* Sync if dirty */
	else if (pSprite->IsBackendDirty()) {
		/* Destroy old sprite and recreate */
		spritectl_destroy_sprite(pSprite->GetBackendSprite());
		pSprite->SetBackendSprite(SPRITECTL_INVALID_SPRITE);

		/* Recreate (will be created on next call) */
		return get_backend_sprite(pSprite);
	}

	return pSprite->GetBackendSprite();
}

/**
 * Get or create backend sprite from CAlphaSprite
 * Handles lazy creation and synchronization
 * NOTE: CAlphaSprite uses RLE compression with alpha channel
 */
static spritectl_sprite_t get_backend_alpha_sprite(CAlphaSprite* pSprite)
{
	if (!pSprite || !pSprite->IsInit()) {
		return SPRITECTL_INVALID_SPRITE;
	}

	/* Lazy creation: create backend sprite if doesn't exist */
	if (pSprite->GetBackendSprite() == SPRITECTL_INVALID_SPRITE) {
		WORD width = pSprite->GetWidth();
		WORD height = pSprite->GetHeight();

		size_t pixel_count = width * height;
		size_t data_size = pixel_count * sizeof(WORD);

		/* Allocate and decompress pixel data */
		WORD* pixels = (WORD*)malloc(data_size);
		if (!pixels) {
			return SPRITECTL_INVALID_SPRITE;
		}

		/* Decompress RLE format to raw pixels */
		/* Initialize with transparent color */
		WORD colorkey = CAlphaSprite::GetColorkey();
		for (size_t i = 0; i < pixel_count; i++) {
			pixels[i] = colorkey;
		}

		/* Decompress each line */
		for (WORD y = 0; y < height; y++) {
			WORD* pPixels = pSprite->GetPixelLine(y);
			WORD* dst_line = pixels + (y * width);

			int count = *pPixels++;  // RLE run count
			int x = 0;

			if (count > 0) {
				for (int i = 0; i < count; i++) {
					int transCount = *pPixels++;   // transparent pixel count
					int colorCount = *pPixels++;   // colored pixel count

					x += transCount;  // skip transparent pixels

					/* Copy colored pixels with alpha */
					for (int j = 0; j < colorCount; j++) {
						WORD alpha2 = *pPixels++;  // alpha value
						WORD color = *pPixels++;   // color value

						if (x < width) {
							/* Store as pre-multiplied alpha or similar format */
							/* For now, just store the color directly */
							dst_line[x] = color;
						}
						x++;
					}
				}
			}
		}

		/* Create backend sprite */
		spritectl_sprite_t new_sprite = spritectl_create_sprite(
			width, height, SPRITECTL_FORMAT_RGB565,
			pixels, data_size);

		free(pixels);
		pSprite->SetBackendSprite(new_sprite);
		pSprite->SetBackendDirty(false);
	}
	/* Sync if dirty */
	else if (pSprite->IsBackendDirty()) {
		/* Destroy old sprite and recreate */
		spritectl_destroy_sprite(pSprite->GetBackendSprite());
		pSprite->SetBackendSprite(SPRITECTL_INVALID_SPRITE);

		/* Recreate (will be created on next call) */
		return get_backend_alpha_sprite(pSprite);
	}

	return pSprite->GetBackendSprite();
}

/**
 * Get or create backend sprite from CShadowSprite
 * Handles lazy creation and synchronization
 */
static spritectl_sprite_t get_backend_shadow_sprite(CShadowSprite* pSprite)
{
	if (!pSprite || !pSprite->IsInit()) {
		return SPRITECTL_INVALID_SPRITE;
	}

	/* Lazy creation: create backend sprite if doesn't exist */
	if (pSprite->GetBackendSprite() == SPRITECTL_INVALID_SPRITE) {
		WORD width = pSprite->GetWidth();
		WORD height = pSprite->GetHeight();

		size_t pixel_count = width * height;
		size_t data_size = pixel_count * sizeof(WORD);

		/* Allocate and decode pixel data */
		WORD* pixels = (WORD*)malloc(data_size);
		if (!pixels) {
			return SPRITECTL_INVALID_SPRITE;
		}

		memset(pixels, 0, data_size);
		pSprite->Blt(pixels, width * sizeof(WORD));

		/* Create backend sprite */
		spritectl_sprite_t new_sprite = spritectl_create_sprite(
			width, height, SPRITECTL_FORMAT_RGB565,
			pixels, data_size);

		free(pixels);
		pSprite->SetBackendSprite(new_sprite);
		pSprite->SetBackendDirty(false);
	}
	/* Sync if dirty */
	else if (pSprite->IsBackendDirty()) {
		/* Destroy old sprite and recreate */
		spritectl_destroy_sprite(pSprite->GetBackendSprite());
		pSprite->SetBackendSprite(SPRITECTL_INVALID_SPRITE);

		/* Recreate (will be created on next call) */
		return get_backend_shadow_sprite(pSprite);
	}

	return pSprite->GetBackendSprite();
}

/**
 * Get or create backend sprite from CIndexSprite
 * Handles lazy creation and synchronization
 */
static spritectl_sprite_t get_backend_index_sprite(CIndexSprite* pSprite)
{
	if (!pSprite || !pSprite->IsInit()) {
		return SPRITECTL_INVALID_SPRITE;
	}

	/* Lazy creation: create backend sprite if doesn't exist */
	if (pSprite->GetBackendSprite() == SPRITECTL_INVALID_SPRITE) {
		WORD width = pSprite->GetWidth();
		WORD height = pSprite->GetHeight();

		size_t pixel_count = width * height;
		size_t data_size = pixel_count * sizeof(WORD);

		/* Allocate and decode pixel data (index sprites are RLE-compressed) */
		WORD* pixels = (WORD*)malloc(data_size);
		if (!pixels) {
			return SPRITECTL_INVALID_SPRITE;
		}
		memset(pixels, 0, data_size);
		pSprite->Blt(pixels, width * sizeof(WORD));

		/* Create backend sprite */
		spritectl_sprite_t new_sprite = spritectl_create_sprite(
			width, height, SPRITECTL_FORMAT_RGB565,
			pixels, data_size);

		free(pixels);
		pSprite->SetBackendSprite(new_sprite);
		pSprite->SetBackendDirty(false);
	}
	/* Sync if dirty */
	else if (pSprite->IsBackendDirty()) {
		/* Destroy old sprite and recreate */
		spritectl_destroy_sprite(pSprite->GetBackendSprite());
		pSprite->SetBackendSprite(SPRITECTL_INVALID_SPRITE);

		/* Recreate (will be created on next call) */
		return get_backend_index_sprite(pSprite);
	}

	return pSprite->GetBackendSprite();
}

/* ============================================================================
 * BltSprite Methods
 * ============================================================================ */

void CSpriteSurface::BltSprite(POINT* pPoint, CSprite* pSprite) {
	if (!pPoint || !pSprite) {
		return;
	}

	/* Get backend sprite */
	spritectl_sprite_t backend_sprite = get_backend_sprite(pSprite);
	if (!backend_sprite) {
		return;
	}

	/* Blit to backend surface */
	int flags = 0;
	int alpha = 255;
	spritectl_blt_sprite(m_backend_surface, pPoint->x, pPoint->y,
	                    backend_sprite, flags, alpha);
}

void CSpriteSurface::BltSpriteNoClip(POINT* pPoint, CSprite* pSprite) {
	/* For now, same as BltSprite */
	BltSprite(pPoint, pSprite);
}

void CSpriteSurface::BltSpriteHalf(POINT* pPoint, CSprite* pSprite) {
	if (!pPoint || !pSprite) {
		return;
	}

	/* Get backend sprite */
	spritectl_sprite_t backend_sprite = get_backend_sprite(pSprite);
	if (!backend_sprite) {
		return;
	}

	/* Scale factor: 128 = 0.5x */
	int scale = 128;
	int flags = 0;
	spritectl_blt_sprite_scaled(m_backend_surface, pPoint->x, pPoint->y,
	                            backend_sprite, scale, flags);
}

void CSpriteSurface::BltSpriteAlpha(POINT* pPoint, CSprite* pSprite, BYTE alphaDepth) {
	if (!pPoint || !pSprite) {
		return;
	}

	/* Get backend sprite */
	spritectl_sprite_t backend_sprite = get_backend_sprite(pSprite);
	if (!backend_sprite) {
		return;
	}

	/* Blit with alpha */
	int flags = SPRITECTL_BLT_ALPHA;
	spritectl_blt_sprite(m_backend_surface, pPoint->x, pPoint->y,
	                    backend_sprite, flags, alphaDepth);
}

void CSpriteSurface::BltSpriteScale(POINT* pPoint, CSprite* pSprite, BYTE scale) {
	if (!pPoint || !pSprite) {
		return;
	}

	/* Get backend sprite */
	spritectl_sprite_t backend_sprite = get_backend_sprite(pSprite);
	if (!backend_sprite) {
		return;
	}

	/* Scale parameter: 256 = 1x, 128 = 0.5x */
	int scale_factor = scale;
	int flags = 0;
	spritectl_blt_sprite_scaled(m_backend_surface, pPoint->x, pPoint->y,
	                            backend_sprite, scale_factor, flags);
}

/* ============================================================================
 * Stub implementations for other BltSprite variants
 * These will be implemented in future iterations
 * ============================================================================ */

void CSpriteSurface::BltSpriteColor(POINT* pPoint, CSprite* pSprite, BYTE rgb) {
	/* TODO: Implement color tinting */
	BltSprite(pPoint, pSprite);
}

void CSpriteSurface::BltSpriteDarkness(POINT* pPoint, CSprite* pSprite, BYTE DarkBits) {
	/* TODO: Implement darkness effect */
	BltSprite(pPoint, pSprite);
}

void CSpriteSurface::BltSpriteColorSet(POINT* pPoint, CSprite* pSprite, WORD colorSet) {
	/* TODO: Implement color set */
	BltSprite(pPoint, pSprite);
}

void CSpriteSurface::BltSpriteEffect(POINT* pPoint, CSprite* pSprite) {
	/* TODO: Implement effect */
	BltSprite(pPoint, pSprite);
}

void CSpriteSurface::BltSpriteAlpha4444SmallNotTrans(POINT* pPoint, CSprite* pSprite, BYTE alpha, BYTE shift) {
	/* TODO: Implement */
	BltSpriteAlpha(pPoint, pSprite, alpha);
}

void CSpriteSurface::BltSpriteAlpha4444NotTrans(POINT* pPoint, CSprite* pSprite, BYTE alpha) {
	/* TODO: Implement */
	BltSpriteAlpha(pPoint, pSprite, alpha);
}

void CSpriteSurface::BltSprite1555SmallNotTrans(POINT* pPoint, CSprite* pSprite, BYTE shift) {
	/* TODO: Implement */
	BltSprite(pPoint, pSprite);
}

void CSpriteSurface::BltSprite1555NotTrans(POINT* pPoint, CSprite* pSprite) {
	/* TODO: Implement */
	BltSprite(pPoint, pSprite);
}

void CSpriteSurface::BltSpritePalEffect(POINT* pPoint, CSpritePal* pSprite, MPalette &pal) {
	/* TODO: Implement palette effect */
}

void CSpriteSurface::BltSpritePal1555SmallNotTrans(POINT* pPoint, CSpritePal* pSprite, BYTE shift, MPalette &pal) {
	/* TODO: Implement */
}

void CSpriteSurface::BltSpritePal1555NotTrans(POINT* pPoint, CSpritePal* pSprite, MPalette &pal) {
	/* TODO: Implement */
}

void CSpriteSurface::BltSpriteAlphaFilter(POINT* pPoint, CSprite* pSprite) {
	/* TODO: Implement alpha filter */
	BltSprite(pPoint, pSprite);
}

void CSpriteSurface::BltSpriteAlphaFilterDarkness(POINT* pPoint, CSprite* pSprite, BYTE DarkBits) {
	/* TODO: Implement */
	BltSpriteDarkness(pPoint, pSprite, DarkBits);
}

void CSpriteSurface::BltSpriteDarkerFilter(POINT* pPoint, CSprite* pSprite) {
	/* TODO: Implement darker filter */
	BltSprite(pPoint, pSprite);
}

/* ============================================================================
 * Alpha Sprite Methods
 * ============================================================================ */

void CSpriteSurface::BltAlphaSprite(POINT* pPoint, CAlphaSprite* pSprite) {
	if (!pPoint || !pSprite) {
		return;
	}

	/* Get backend sprite */
	spritectl_sprite_t backend_sprite = get_backend_alpha_sprite(pSprite);
	if (!backend_sprite) {
		return;
	}

	/* Blit to backend surface */
	int flags = SPRITECTL_BLT_ALPHA;
	int alpha = 255;
	spritectl_blt_sprite(m_backend_surface, pPoint->x, pPoint->y,
	                    backend_sprite, flags, alpha);
}

void CSpriteSurface::BltAlphaSpriteAlpha(POINT* pPoint, CAlphaSprite* pSprite, BYTE alpha) {
	if (!pPoint || !pSprite) {
		return;
	}

	/* Get backend sprite */
	spritectl_sprite_t backend_sprite = get_backend_alpha_sprite(pSprite);
	if (!backend_sprite) {
		return;
	}

	/* Blit to backend surface with alpha */
	int flags = SPRITECTL_BLT_ALPHA;
	spritectl_blt_sprite(m_backend_surface, pPoint->x, pPoint->y,
	                    backend_sprite, flags, alpha);
}

void CSpriteSurface::BltAlphaSprite4444(POINT* pPoint, CAlphaSprite* pSprite) {
	/* TODO: Implement 4444 format conversion */
	/* For now, use regular blit */
	BltAlphaSprite(pPoint, pSprite);
}

void CSpriteSurface::BltAlphaSprite4444NotTrans(POINT* pPoint, CAlphaSprite* pSprite) {
	/* TODO: Implement 4444 NotTrans */
	BltAlphaSprite(pPoint, pSprite);
}

void CSpriteSurface::BltAlphaSprite4444SmallNotTrans(POINT* pPoint, CAlphaSprite* pSprite, BYTE shift) {
	/* TODO: Implement scaling */
	BltAlphaSprite(pPoint, pSprite);
}

void CSpriteSurface::BltAlphaSpritePal(POINT* pPoint, CAlphaSpritePal* pSprite, MPalette &pal) {
	/* TODO: Implement palette support */
}

void CSpriteSurface::BltAlphaSpritePalAlpha(POINT* pPoint, CAlphaSpritePal* pSprite, BYTE alpha, MPalette &pal) {
	/* TODO: Implement */
}

void CSpriteSurface::BltAlphaSpritePal4444(POINT* pPoint, CAlphaSpritePal* pSprite, MPalette &pal) {
	/* TODO: Implement */
}

void CSpriteSurface::BltAlphaSpritePal4444NotTrans(POINT* pPoint, CAlphaSpritePal* pSprite, MPalette &pal) {
	/* TODO: Implement */
}

void CSpriteSurface::BltAlphaSpritePal4444SmallNotTrans(POINT* pPoint, CAlphaSpritePal* pSprite, BYTE shift, MPalette &pal) {
	/* TODO: Implement */
}

/* ============================================================================
 * Index Sprite Methods
 * ============================================================================ */

void CSpriteSurface::BltIndexSprite(POINT* pPoint, CIndexSprite* pSprite) {
	if (!pPoint || !pSprite) {
		return;
	}

	/* Get backend sprite */
	spritectl_sprite_t backend_sprite = get_backend_index_sprite(pSprite);
	if (!backend_sprite) {
		return;
	}

	/* Blit to backend surface */
	int flags = 0;
	int alpha = 255;
	spritectl_blt_sprite(m_backend_surface, pPoint->x, pPoint->y,
	                    backend_sprite, flags, alpha);
}

void CSpriteSurface::BltIndexSpriteDarkness(POINT* pPoint, CIndexSprite* pSprite, BYTE DarkBits) {
	/* TODO: Implement darkness effect */
	BltIndexSprite(pPoint, pSprite);
}

void CSpriteSurface::BltIndexSpriteAlpha(POINT* pPoint, CIndexSprite* pSprite, BYTE alpha) {
	if (!pPoint || !pSprite) {
		return;
	}

	/* Get backend sprite */
	spritectl_sprite_t backend_sprite = get_backend_index_sprite(pSprite);
	if (!backend_sprite) {
		return;
	}

	/* Blit to backend surface with alpha */
	int flags = SPRITECTL_BLT_ALPHA;
	spritectl_blt_sprite(m_backend_surface, pPoint->x, pPoint->y,
	                    backend_sprite, flags, alpha);
}

void CSpriteSurface::BltIndexSpriteColor(POINT* pPoint, CIndexSprite* pSprite, BYTE rgb) {
	/* TODO: Implement color tinting */
	BltIndexSprite(pPoint, pSprite);
}

void CSpriteSurface::BltIndexSpriteColorSet(POINT* pPoint, CIndexSprite* pSprite, WORD colorSet) {
	/* TODO: Implement color set */
	BltIndexSprite(pPoint, pSprite);
}

void CSpriteSurface::BltIndexSpriteEffect(POINT* pPoint, CIndexSprite* pSprite) {
	/* TODO: Implement effect */
	BltIndexSprite(pPoint, pSprite);
}

void CSpriteSurface::BltIndexSpriteBrightness(POINT* pPoint, CIndexSprite* pSprite, BYTE BrightBits) {
	/* TODO: Implement brightness */
	BltIndexSprite(pPoint, pSprite);
}

/* ============================================================================
 * Shadow Sprite Methods
 * ============================================================================ */

void CSpriteSurface::BltShadowSprite(POINT* pPoint, CShadowSprite* pSprite) {
	if (!pPoint || !pSprite) {
		return;
	}

	/* Get backend sprite */
	spritectl_sprite_t backend_sprite = get_backend_shadow_sprite(pSprite);
	if (!backend_sprite) {
		return;
	}

	/* Blit to backend surface */
	int flags = 0;
	int alpha = 255;
	spritectl_blt_sprite(m_backend_surface, pPoint->x, pPoint->y,
	                    backend_sprite, flags, alpha);
}

void CSpriteSurface::BltShadowSpriteSmall(POINT* pPoint, CShadowSprite* pSprite, BYTE shift) {
	/* TODO: Implement scaling */
	BltShadowSprite(pPoint, pSprite);
}

void CSpriteSurface::BltShadowSpriteDarkness(POINT* pPoint, CShadowSprite* pSprite, BYTE DarkBits) {
	/* TODO: Implement darkness effect */
	BltShadowSprite(pPoint, pSprite);
}

void CSpriteSurface::BltShadowSprite4444(POINT* pPoint, CShadowSprite* pSprite, WORD pixel) {
	/* TODO: Implement 4444 format */
	BltShadowSprite(pPoint, pSprite);
}

void CSpriteSurface::BltShadowSpriteSmall4444(POINT* pPoint, CShadowSprite* pSprite, WORD pixel, BYTE shift) {
	/* TODO: Implement scaling + 4444 */
	BltShadowSprite(pPoint, pSprite);
}

/* ============================================================================
 * Sprite Outline Methods
 * ============================================================================ */

void CSpriteSurface::BltSpriteOutline(CSpriteOutlineManager *pSOM, WORD color) {
	/* TODO: Implement sprite outline */
}

void CSpriteSurface::BltSpriteOutlineOnly(CSpriteOutlineManager* pSOM, WORD color) {
	/* TODO: Implement */
}

void CSpriteSurface::BltSpriteOutlineDarkness(CSpriteOutlineManager* pSOM, WORD color, BYTE DarkBits) {
	/* TODO: Implement */
}
