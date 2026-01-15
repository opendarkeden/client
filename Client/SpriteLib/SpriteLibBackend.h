/*-----------------------------------------------------------------------------

	SpriteLibBackend.h

	Unified backend interface for SpriteLib cross-platform support.
	Provides C interface for Windows (DirectDraw) and SDL2 backends.

	2025.01.14

-----------------------------------------------------------------------------*/

#ifndef __SPRITELIBBACKEND_H__
#define __SPRITELIBBACKEND_H__

#include "../basic/Platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Rectangle Types (for non-Windows platforms)
 * ============================================================================ */

#ifndef PLATFORM_WINDOWS
#ifndef LONG
typedef long LONG;
#endif

/* Only define if not already defined (e.g., by client_PCH.h) */
#ifndef RECT_DEFINED
#define RECT_DEFINED
typedef struct tagRECT {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT, *PRECT, *LPRECT;
#endif
#endif

/* ============================================================================
 * Backend Selection
 * ============================================================================ */

/* Define which backend to use */
#ifdef SPRITELIB_BACKEND_SDL
	#include <SDL.h>
#else
	/* Windows/DirectDraw backend (original) */
#endif

/* ============================================================================
 * Type Definitions
 * ============================================================================ */

/* Opaque handles for backend objects */
typedef struct spritectl_surface_s* spritectl_surface_t;
typedef struct spritectl_sprite_s* spritectl_sprite_t;
typedef struct spritectl_pack_s* spritectl_pack_t;

/* Invalid handles */
#define SPRITECTL_INVALID_SURFACE	((spritectl_surface_t)NULL)
#define SPRITECTL_INVALID_SPRITE	((spritectl_sprite_t)NULL)
#define SPRITECTL_INVALID_PACK		((spritectl_pack_t)NULL)

/* Pixel format constants */
#define SPRITECTL_FORMAT_RGB565		0
#define SPRITECTL_FORMAT_RGB555		1
#define SPRITECTL_FORMAT_RGBA32		2

/* Surface info structure */
typedef struct {
	int width;
	int height;
	int pitch;
	void* pixels;
	int format;  /* SPRITECTL_FORMAT_* */
} spritectl_surface_info_t;

/* Sprite info structure */
typedef struct {
	int width;
	int height;
	int format;  /* SPRITECTL_FORMAT_* */
	size_t data_size;
} spritectl_sprite_info_t;

/* Blitting flags */
#define SPRITECTL_BLT_NONE			0x0000
#define SPRITECTL_BLT_FLIP_LR		0x0001  /* Flip left-right */
#define SPRITECTL_BLT_FLIP_UD		0x0002  /* Flip up-down */
#define SPRITECTL_BLT_ALPHA			0x0004  /* Alpha blending */
#define SPRITECTL_BLT_COLORKEY		0x0008  /* Use color key */

/* ============================================================================
 * Surface Functions
 * ============================================================================ */

/**
 * Create a surface for rendering
 * @param width Surface width in pixels
 * @param height Surface height in pixels
 * @param format Pixel format (SPRITECTL_FORMAT_*)
 * @return Surface handle or SPRITECTL_INVALID_SURFACE on failure
 */
spritectl_surface_t spritectl_create_surface(int width, int height, int format);

/**
 * Destroy a surface
 * @param surface Surface handle
 */
void spritectl_destroy_surface(spritectl_surface_t surface);

/**
 * Lock surface for direct pixel access
 * @param surface Surface handle
 * @param info Output structure containing surface information
 * @return 0 on success, non-zero on failure
 */
int spritectl_lock_surface(spritectl_surface_t surface, spritectl_surface_info_t* info);

/**
 * Unlock surface after pixel access
 * @param surface Surface handle
 * @return 0 on success, non-zero on failure
 */
int spritectl_unlock_surface(spritectl_surface_t surface);

/**
 * Clear surface to a color
 * @param surface Surface handle
 * @param color Color value (format-dependent)
 * @return 0 on success, non-zero on failure
 */
int spritectl_clear_surface(spritectl_surface_t surface, uint32_t color);

/**
 * Get surface dimensions
 * @param surface Surface handle
 * @param width Output width (can be NULL)
 * @param height Output height (can be NULL)
 * @return 0 on success, non-zero on failure
 */
int spritectl_get_surface_size(spritectl_surface_t surface, int* width, int* height);

/* ============================================================================
 * Sprite Functions
 * ============================================================================ */

/**
 * Create a sprite from raw pixel data
 * @param width Sprite width in pixels
 * @param height Sprite height in pixels
 * @param format Pixel format (SPRITECTL_FORMAT_*)
 * @param pixels Pixel data pointer
 * @param data_size Size of pixel data in bytes
 * @return Sprite handle or SPRITECTL_INVALID_SPRITE on failure
 */
spritectl_sprite_t spritectl_create_sprite(int width, int height, int format,
                                           const void* pixels, size_t data_size);

/**
 * Destroy a sprite
 * @param sprite Sprite handle
 */
void spritectl_destroy_sprite(spritectl_sprite_t sprite);

/**
 * Get sprite information
 * @param sprite Sprite handle
 * @param info Output sprite information
 * @return 0 on success, non-zero on failure
 */
int spritectl_get_sprite_info(spritectl_sprite_t sprite, spritectl_sprite_info_t* info);

/**
 * Get sprite pixel data
 * @param sprite Sprite handle
 * @param buffer Buffer to copy pixel data to (can be NULL to query size)
 * @param buffer_size Buffer size in bytes
 * @return Required buffer size on success, 0 on failure
 */
size_t spritectl_get_sprite_data(spritectl_sprite_t sprite, void* buffer, size_t buffer_size);

/* ============================================================================
 * Blitting Functions
 * ============================================================================ */

/**
 * Blit sprite to surface
 * @param dest Destination surface handle
 * @param x Destination X position
 * @param y Destination Y position
 * @param sprite Source sprite handle
 * @param flags Blitting flags (SPRITECTL_BLT_*)
 * @param alpha Alpha value (0-255, used if SPRITECTL_BLT_ALPHA set)
 * @return 0 on success, non-zero on failure
 */
int spritectl_blt_sprite(spritectl_surface_t dest, int x, int y,
                         spritectl_sprite_t sprite, int flags, int alpha);

/**
 * Blit sprite to surface with scaling
 * @param dest Destination surface handle
 * @param x Destination X position
 * @param y Destination Y position
 * @param sprite Source sprite handle
 * @param scale Scale factor (256 = 1x, 128 = 0.5x, 512 = 2x)
 * @param flags Blitting flags (SPRITECTL_BLT_*)
 * @return 0 on success, non-zero on failure
 */
int spritectl_blt_sprite_scaled(spritectl_surface_t dest, int x, int y,
                                spritectl_sprite_t sprite, int scale, int flags);

/**
 * Blit surface to surface
 * @param dest Destination surface handle
 * @param dest_rect Destination rectangle (NULL for entire surface)
 * @param src Source surface handle
 * @param src_rect Source rectangle (NULL for entire surface)
 * @return 0 on success, non-zero on failure
 */
int spritectl_blt_surface(spritectl_surface_t dest,
                          const RECT* dest_rect,
                          spritectl_surface_t src,
                          const RECT* src_rect);

/* ============================================================================
 * Sprite Pack Functions
 * ============================================================================ */

/**
 * Load sprite pack from SPK file
 * @param filename Path to SPK file
 * @param pack Output pack handle
 * @return 0 on success, non-zero on failure
 */
int spritectl_load_pack(const char* filename, spritectl_pack_t* pack);

/**
 * Free sprite pack resources
 * @param pack Pack handle
 */
void spritectl_free_pack(spritectl_pack_t pack);

/**
 * Get sprite from pack
 * @param pack Pack handle
 * @param index Sprite index
 * @param sprite Output sprite handle
 * @return 0 on success, non-zero on failure
 */
int spritectl_get_sprite_from_pack(spritectl_pack_t pack, int index,
                                   spritectl_sprite_t* sprite);

/**
 * Get sprite count from pack
 * @param pack Pack handle
 * @return Number of sprites in pack, or 0 on failure
 */
int spritectl_get_pack_count(spritectl_pack_t pack);

/* ============================================================================
 * Pixel Conversion Functions
 * ============================================================================ */

/**
 * Convert RGB565 to RGB555
 * @param src Source pixels (RGB565)
 * @param dest Destination buffer (RGB555)
 * @param pixel_count Number of pixels to convert
 */
void spritectl_convert_565_to_555(const uint16_t* src, uint16_t* dest, int pixel_count);

/**
 * Convert RGB555 to RGB565
 * @param src Source pixels (RGB555)
 * @param dest Destination buffer (RGB565)
 * @param pixel_count Number of pixels to convert
 */
void spritectl_convert_555_to_565(const uint16_t* src, uint16_t* dest, int pixel_count);

/**
 * Convert RGB565 to RGBA32
 * @param src Source pixels (RGB565)
 * @param dest Destination buffer (RGBA32)
 * @param pixel_count Number of pixels to convert
 * @param colorkey Color key value for transparency (0xFFFF = none)
 */
void spritectl_convert_565_to_rgba(const uint16_t* src, uint32_t* dest,
                                   int pixel_count, uint16_t colorkey);

/**
 * Convert RGB555 to RGBA32
 * @param src Source pixels (RGB555)
 * @param dest Destination buffer (RGBA32)
 * @param pixel_count Number of pixels to convert
 * @param colorkey Color key value for transparency (0xFFFF = none)
 */
void spritectl_convert_555_to_rgba(const uint16_t* src, uint32_t* dest,
                                   int pixel_count, uint16_t colorkey);

/* ============================================================================
 * Initialization
 * ============================================================================ */

/**
 * Initialize SpriteLib backend
 * Call this at program startup before using any other functions
 * @return 0 on success, non-zero on failure
 */
int spritectl_init(void);

/**
 * Shutdown SpriteLib backend
 * Call this at program shutdown
 */
void spritectl_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* __SPRITELIBBACKEND_H__ */
