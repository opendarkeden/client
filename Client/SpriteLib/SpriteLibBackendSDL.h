/*-----------------------------------------------------------------------------

	SpriteLibBackendSDL.h

	Internal SDL2 backend implementation structures for SpriteLib.
	These structures are opaque to the public interface.

	2025.01.14

-----------------------------------------------------------------------------*/

#ifndef __SPRITELIBBACKENDSDL_H__
#define __SPRITELIBBACKENDSDL_H__

#include "SpriteLibBackend.h"

#ifdef SPRITELIB_BACKEND_SDL

/* Open extern "C" block for C linkage (matching where types are declared) */
#ifdef __cplusplus
extern "C" {
#endif

#include <SDL2/SDL.h>

/* ============================================================================
 * Internal Surface Structure
 * ============================================================================ */

struct spritectl_surface_s {
	SDL_Surface* surface;       /* SDL surface for pixel access */
	SDL_Texture* texture;       /* SDL texture for rendering (optional) */
	SDL_Renderer* renderer;     /* SDL renderer (if texture used) */
	int width;
	int height;
	int format;                 /* SPRITECTL_FORMAT_* */
	int locked;                 /* Lock count */
	int ref_count;              /* Reference count */
};

/* ============================================================================
 * Internal Sprite Structure
 * ============================================================================ */

struct spritectl_sprite_s {
	int width;
	int height;
	int format;                 /* SPRITECTL_FORMAT_* */
	uint16_t* pixels;           /* Pixel data (565 or 555 format) */
	uint32_t* rgba_pixels;      /* Decoded RGBA32 pixels (cached) */
	size_t data_size;           /* Size of pixel data */
	int ref_count;              /* Reference count */
};

/* ============================================================================
 * Internal Sprite Pack Structure
 * ============================================================================ */

struct spritectl_pack_s {
	char* filename;             /* SPK file path */
	FILE* file;                 /* File handle for lazy loading */
	int sprite_count;           /* Number of sprites in pack */
	spritectl_sprite_t* sprites; /* Array of sprite handles */
	int* loaded_flags;          /* Which sprites are loaded (for lazy loading) */
	uint32_t* file_offsets;     /* File offsets for each sprite */
	int lazy_loading;           /* Whether lazy loading is enabled */
	int ref_count;              /* Reference count */
};

/* ============================================================================
 * Internal Helper Functions
 * ============================================================================ */

/**
 * Create SDL surface with specified pixel format
 * @param width Surface width
 * @param height Surface height
 * @param format Pixel format (SPRITECTL_FORMAT_*)
 * @return SDL_Surface pointer or NULL on failure
 */
SDL_Surface* spritectl_sdl_create_surface(int width, int height, int format);

/**
 * Get SDL pixel format enum from SPRITECTL format
 * @param format SPRITECTL_FORMAT_* value
 * @return SDL_PixelFormatEnum value
 */
uint32_t spritectl_sdl_get_pixelformat(int format);

/**
 * Convert color key value to SDL color key
 * @param format SPRITECTL_FORMAT_* value
 * @param colorkey Color key in source format
 * @return Uint32 color key for SDL
 */
Uint32 spritectl_sdl_get_colorkey(int format, Uint16 colorkey);

/**
 * Lock SDL surface and return pixel pointer
 * @param surface spritectl surface handle
 * @param info Output surface info
 * @return 0 on success, non-zero on failure
 */
int spritectl_sdl_lock_surface(spritectl_surface_t surface,
                               spritectl_surface_info_t* info);

/**
 * Unlock SDL surface
 * @param surface spritectl surface handle
 * @return 0 on success, non-zero on failure
 */
int spritectl_sdl_unlock_surface(spritectl_surface_t surface);

/* ============================================================================
 * Sprite Decoding (RLE)
 * ============================================================================ */

/**
 * Load sprite from file (RLE encoded)
 * @param file File pointer positioned at sprite data
 * @param sprite_out Output sprite handle
 * @param colorkey Color key value for transparency
 * @return 0 on success, non-zero on failure
 */
int spritectl_load_sprite_from_file(FILE* file, spritectl_sprite_t* sprite_out,
                                    uint16_t colorkey);

/**
 * Decode RLE-compressed sprite data
 * @param compressed Compressed data
 * @param compressed_size Size of compressed data
 * @param width Sprite width
 * @param height Sprite height
 * @param pixels_out Output pixel buffer (must be pre-allocated)
 * @param pixels_size Size of output buffer
 * @return 0 on success, non-zero on failure
 */
int spritectl_decode_rle_sprite(const uint8_t* compressed, size_t compressed_size,
                                int width, int height,
                                uint16_t* pixels_out, size_t pixels_size);

/* ============================================================================
 * Pixel Format Conversion (Internal)
 * ============================================================================ */

/**
 * Extract RGB components from RGB565 pixel
 */
static inline void spritectl_565_to_rgb(uint16_t pixel, uint8_t* r, uint8_t* g, uint8_t* b) {
	*r = ((pixel >> 11) & 0x1F) << 3;  /* 5 bits -> 8 bits */
	*g = ((pixel >> 5) & 0x3F) << 2;   /* 6 bits -> 8 bits */
	*b = (pixel & 0x1F) << 3;          /* 5 bits -> 8 bits */
}

/**
 * Extract RGB components from RGB555 pixel
 */
static inline void spritectl_555_to_rgb(uint16_t pixel, uint8_t* r, uint8_t* g, uint8_t* b) {
	*r = ((pixel >> 10) & 0x1F) << 3;  /* 5 bits -> 8 bits */
	*g = ((pixel >> 5) & 0x1F) << 3;   /* 5 bits -> 8 bits */
	*b = (pixel & 0x1F) << 3;          /* 5 bits -> 8 bits */
}

/**
 * Pack RGB components to RGBA32
 */
static inline uint32_t spritectl_rgb_to_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return (uint32_t)a << 24 | (uint32_t)b << 16 | (uint32_t)g << 8 | (uint32_t)r;
}

#ifdef __cplusplus
}
#endif

#endif /* SPRITELIB_BACKEND_SDL */

#endif /* __SPRITELIBBACKENDSDL_H__ */
