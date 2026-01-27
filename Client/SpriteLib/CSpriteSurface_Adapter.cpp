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
 * Debug Configuration
 * ============================================================================ */

// Enable detailed debug logging for Sprite adapter
#ifndef SPRITE_ADAPTER_DEBUG
#define SPRITE_ADAPTER_DEBUG 0
#endif

// Enable tracking of backend sprite lifecycle
#ifndef SPRITE_ADAPTER_DEBUG_LIFECYCLE
#define SPRITE_ADAPTER_DEBUG_LIFECYCLE 0
#endif

#if SPRITE_ADAPTER_DEBUG
#define SA_DEBUG(fmt, ...) \
	fprintf(stderr, "[SpriteAdapter] %s:%d: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define SA_DEBUG(fmt, ...) do {} while(0)
#endif

#if SPRITE_ADAPTER_DEBUG_LIFECYCLE
#define SA_DEBUG_LIFECYCLE(fmt, ...) \
	fprintf(stderr, "[SpriteAdapter LIFECYCLE] %s:%d: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define SA_DEBUG_LIFECYCLE(fmt, ...) do {} while(0)
#endif

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
		static int notInitCount = 0;
		if (notInitCount < 3) {
			printf("[get_backend_sprite] ERROR: pSprite=%p, IsInit=%d\n", pSprite, pSprite ? pSprite->IsInit() : 0);
			notInitCount++;
		}
		return SPRITECTL_INVALID_SPRITE;
	}

	/* Lazy creation: create backend sprite if doesn't exist */
	if (pSprite->GetBackendSprite() == SPRITECTL_INVALID_SPRITE) {
		WORD width = pSprite->GetWidth();
		WORD height = pSprite->GetHeight();

		/* Create backend sprite with RLE data for correct transparency */
		spritectl_sprite_t new_sprite = spritectl_create_sprite_rle(width, height);
		if (!new_sprite) {
			return SPRITECTL_INVALID_SPRITE;
		}

		/* Copy RLE data from CSprite to backend sprite */
		for (WORD y = 0; y < height; y++) {
			WORD* src_line = pSprite->GetPixelLine(y);

			/* Get RLE data size */
			WORD* pSrc = src_line;
			int count = *pSrc++;  /* Number of runs */

			/* Calculate total RLE data size (count + runs data) */
			int rle_size = 1;  /* count byte */
			if (count > 0) {
				for (int j = 0; j < count; j++) {
					pSrc++;  /* Skip transCount */
					int colorCount = *pSrc++;  /* Color pixels */
					rle_size += 2 + colorCount;  /* trans + color count + pixel data */
					pSrc += colorCount;
				}
			}

			/* Set RLE data using helper function */
			if (rle_size > 1) {  /* Non-empty scanline */
				if (spritectl_sprite_set_scanline_rle(new_sprite, y, src_line, rle_size) != 0) {
					/* Cleanup and fall back */
					spritectl_destroy_sprite(new_sprite);
					return SPRITECTL_INVALID_SPRITE;
				}
			}
		}

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
	SA_DEBUG_LIFECYCLE("get_backend_shadow_sprite: pSprite=%p, IsInit=%d",
	                   (void*)pSprite, pSprite ? pSprite->IsInit() : 0);

	if (!pSprite || !pSprite->IsInit()) {
		return SPRITECTL_INVALID_SPRITE;
	}

	/* Lazy creation: create backend sprite if doesn't exist */
	if (pSprite->GetBackendSprite() == SPRITECTL_INVALID_SPRITE) {
		WORD width = pSprite->GetWidth();
		WORD height = pSprite->GetHeight();

		size_t pixel_count = width * height;
		size_t data_size = pixel_count * sizeof(WORD);

		SA_DEBUG_LIFECYCLE("get_backend_shadow_sprite: Creating backend sprite, size=%dx%d (%zu pixels, %zu bytes)",
		                   width, height, pixel_count, data_size);

		/* Allocate and decode pixel data */
		WORD* pixels = (WORD*)malloc(data_size);
		if (!pixels) {
			SA_DEBUG_LIFECYCLE("get_backend_shadow_sprite: Failed to allocate pixel buffer");
			return SPRITECTL_INVALID_SPRITE;
		}

		SA_DEBUG_LIFECYCLE("get_backend_shadow_sprite: Allocated temp pixels=%p", (void*)pixels);

		memset(pixels, 0, data_size);
		pSprite->Blt(pixels, width * sizeof(WORD));

		/* Create backend sprite */
		spritectl_sprite_t new_sprite = spritectl_create_sprite(
			width, height, SPRITECTL_FORMAT_RGB565,
			pixels, data_size);

		SA_DEBUG_LIFECYCLE("get_backend_shadow_sprite: Created backend sprite=%p from temp pixels=%p",
		                   (void*)new_sprite, (void*)pixels);

		// Free temp pixels AFTER creating the sprite (the sprite copies the data)
		free(pixels);
		pixels = NULL;  // Prevent dangling pointer

		pSprite->SetBackendSprite(new_sprite);
		pSprite->SetBackendDirty(false);

		SA_DEBUG_LIFECYCLE("get_backend_shadow_sprite: Set backend sprite=%p for CShadowSprite=%p",
		                   (void*)new_sprite, (void*)pSprite);
	}
	/* Sync if dirty */
	else if (pSprite->IsBackendDirty()) {
		SA_DEBUG_LIFECYCLE("get_backend_shadow_sprite: Backend dirty, destroying old sprite=%p",
		                   (void*)pSprite->GetBackendSprite());
		/* Destroy old sprite and recreate */
		spritectl_destroy_sprite(pSprite->GetBackendSprite());
		pSprite->SetBackendSprite(SPRITECTL_INVALID_SPRITE);

		/* Recreate (will be created on next call) */
		return get_backend_shadow_sprite(pSprite);
	}
	else {
		SA_DEBUG_LIFECYCLE("get_backend_shadow_sprite: Reusing existing backend sprite=%p",
		                   (void*)pSprite->GetBackendSprite());
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
		static int invalidCount = 0;
		if (invalidCount < 3) {
			printf("[BltSprite] ERROR: get_backend_sprite returned invalid sprite! IsInit=%d\n", pSprite->IsInit());
			invalidCount++;
		}
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

void CSpriteSurface::BltSpriteScale(POINT* pPoint, CSprite* pSprite, int scale) {
	if (!pPoint || !pSprite) {
		return;
	}

	/* Get backend sprite */
	spritectl_sprite_t backend_sprite = get_backend_sprite(pSprite);
	if (!backend_sprite) {
		return;
	}

	/* Scale parameter: 256 = 1x, 128 = 0.5x, 512 = 2x */
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
	static int debugCount = 0;

	if (!pPoint || !pSprite) {
		if (debugCount < 3) {
			printf("[BltAlphaSpritePal] ERROR: Invalid parameters (pPoint=%p, pSprite=%p)\n", pPoint, pSprite);
			debugCount++;
		}
		return;
	}

	/* Check if sprite is initialized */
	if (pSprite->IsNotInit()) {
		if (debugCount < 3) {
			printf("[BltAlphaSpritePal] ERROR: Sprite not initialized\n");
			debugCount++;
		}
		return;
	}

	/* Basic clipping check - skip if completely outside surface */
	int spriteWidth = pSprite->GetWidth();
	int spriteHeight = pSprite->GetHeight();

	/* Get surface dimensions */
	int surfaceWidth = m_width;
	int surfaceHeight = m_height;

	/* Check if sprite is completely outside the surface */
	bool outsideBounds = (pPoint->x >= surfaceWidth) ||
	                     (pPoint->y >= surfaceHeight) ||
	                     (pPoint->x + spriteWidth <= 0) ||
	                     (pPoint->y + spriteHeight <= 0);

	if (outsideBounds) {
		/* Sprite is completely outside, skip rendering */
		static int skipCount = 0;
		if (skipCount < 5) {
			printf("[BltAlphaSpritePal] WARNING: Sprite at (%d,%d) size=%dx%d outside surface %dx%d, skipping\n",
			       pPoint->x, pPoint->y, spriteWidth, spriteHeight, surfaceWidth, surfaceHeight);
			skipCount++;
		}
		return;
	}

	/* Additional check: if sprite starts outside surface bounds, skip for now */
	/* TODO: Implement proper partial clipping */
	if (pPoint->x < 0 || pPoint->y < 0 ||
	    pPoint->x + spriteWidth > surfaceWidth ||
	    pPoint->y + spriteHeight > surfaceHeight) {
		static int partialCount = 0;
		if (partialCount < 5) {
			printf("[BltAlphaSpritePal] WARNING: Partial clipping at (%d,%d) size=%dx%d, skipping (TODO: implement)\n",
			       pPoint->x, pPoint->y, spriteWidth, spriteHeight);
			partialCount++;
		}
		return;
	}

	/* Debug log */
	if (debugCount < 10) {
		printf("[BltAlphaSpritePal] Rendering sprite at (%d,%d), size=%dx%d\n",
		       pPoint->x, pPoint->y, spriteWidth, spriteHeight);
		debugCount++;
	}

	/* Lock backend surface for direct pixel access */
	spritectl_surface_info_t surface_info;
	if (spritectl_lock_surface(m_backend_surface, &surface_info) != 0) {
		static int lockFailCount = 0;
		if (lockFailCount < 3) {
			printf("[BltAlphaSpritePal] ERROR: Failed to lock surface\n");
			lockFailCount++;
		}
		return;
	}

	/* Get pixel pointer and pitch (pitch is in bytes, like Windows) */
	WORD* pixels = (WORD*)surface_info.pixels;
	int pitch = surface_info.pitch;

	/* Calculate destination pointer with offset */
	WORD* pDest = (WORD*)((BYTE*)pixels + pPoint->y * pitch + (pPoint->x << 1));

	/* Call sprite's Blt method to render with palette */
	/* Pass pitch in bytes (same as Windows version) */
	/* TODO: Implement proper clipping for partially visible sprites */
	pSprite->Blt(pDest, pitch, pal);

	/* Debug log after render */
	if (debugCount <= 10) {
		printf("[BltAlphaSpritePal] Render complete\n");
	}

	/* Unlock surface */
	spritectl_unlock_surface(m_backend_surface);
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
