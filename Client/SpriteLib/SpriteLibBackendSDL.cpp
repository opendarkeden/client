/*-----------------------------------------------------------------------------

	SpriteLibBackendSDL.cpp

	SDL2 backend implementation for SpriteLib.
	Implements the unified backend interface using SDL2.

	2025.01.14

-----------------------------------------------------------------------------*/

#include "SpriteLibBackendSDL.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ============================================================================
 * Global State
 * ============================================================================ */

static int g_spritectl_initialized = 0;
static SDL_Renderer* g_spritectl_default_renderer = NULL;

/* ============================================================================
 * Initialization
 * ============================================================================ */

int spritectl_init(void) {
	if (g_spritectl_initialized) {
		return 0;  /* Already initialized */
	}

	/* Initialize SDL video subsystem */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "SpriteLib Backend: SDL_Init failed: %s\n", SDL_GetError());
		return -1;
	}

	g_spritectl_initialized = 1;
	return 0;
}

void spritectl_shutdown(void) {
	if (!g_spritectl_initialized) {
		return;
	}

	/* Clean up default renderer */
	if (g_spritectl_default_renderer) {
		SDL_DestroyRenderer(g_spritectl_default_renderer);
		g_spritectl_default_renderer = NULL;
	}

	SDL_Quit();
	g_spritectl_initialized = 0;
}

/* ============================================================================
 * Surface Functions
 * ============================================================================ */

SDL_Surface* spritectl_sdl_create_surface(int width, int height, int format) {
	uint32_t sdl_format;
	int depth;

	/* Get SDL pixel format */
	sdl_format = spritectl_sdl_get_pixelformat(format);
	if (sdl_format == SDL_PIXELFORMAT_UNKNOWN) {
		fprintf(stderr, "SpriteLib Backend: Unknown pixel format %d\n", format);
		return NULL;
	}

	/* Calculate depth */
	switch (format) {
		case SPRITECTL_FORMAT_RGB565:
		case SPRITECTL_FORMAT_RGB555:
			depth = 16;
			break;
		case SPRITECTL_FORMAT_RGBA32:
			depth = 32;
			break;
		default:
			depth = 16;
			break;
	}

	/* Create RGB surface */
	return SDL_CreateRGBSurface(0, width, height, depth,
	                            0, 0, 0, 0);
}

uint32_t spritectl_sdl_get_pixelformat(int format) {
	switch (format) {
		case SPRITECTL_FORMAT_RGB565:
			return SDL_PIXELFORMAT_RGB565;
		case SPRITECTL_FORMAT_RGB555:
			return SDL_PIXELFORMAT_RGB555;
		case SPRITECTL_FORMAT_RGBA32:
			return SDL_PIXELFORMAT_RGBA32;
		default:
			return SDL_PIXELFORMAT_UNKNOWN;
	}
}

spritectl_surface_t spritectl_create_surface(int width, int height, int format) {
	spritectl_surface_t surface;

	if (!g_spritectl_initialized) {
		fprintf(stderr, "SpriteLib Backend: Not initialized\n");
		return SPRITECTL_INVALID_SURFACE;
	}

	/* Allocate surface structure */
	surface = (spritectl_surface_t)malloc(sizeof(struct spritectl_surface_s));
	if (!surface) {
		fprintf(stderr, "SpriteLib Backend: Failed to allocate surface\n");
		return SPRITECTL_INVALID_SURFACE;
	}

	/* Create SDL surface */
	surface->surface = spritectl_sdl_create_surface(width, height, format);
	if (!surface->surface) {
		free(surface);
		return SPRITECTL_INVALID_SURFACE;
	}

	/* Initialize fields */
	surface->texture = NULL;
	surface->renderer = NULL;
	surface->width = width;
	surface->height = height;
	surface->format = format;
	surface->locked = 0;
	surface->ref_count = 1;

	return surface;
}

void spritectl_destroy_surface(spritectl_surface_t surface) {
	if (!surface) {
		return;
	}

	surface->ref_count--;
	if (surface->ref_count > 0) {
		return;  /* Still referenced */
	}

	/* Free SDL resources */
	if (surface->texture) {
		SDL_DestroyTexture(surface->texture);
	}
	if (surface->surface) {
		SDL_FreeSurface(surface->surface);
	}

	free(surface);
}

int spritectl_lock_surface(spritectl_surface_t surface, spritectl_surface_info_t* info) {
	if (!surface || !info) {
		return -1;
	}

	/* Lock SDL surface */
	if (SDL_LockSurface(surface->surface) != 0) {
		fprintf(stderr, "SpriteLib Backend: Failed to lock surface: %s\n", SDL_GetError());
		return -1;
	}

	surface->locked++;

	/* Fill info structure */
	info->width = surface->width;
	info->height = surface->height;
	info->pitch = surface->surface->pitch;
	info->pixels = surface->surface->pixels;
	info->format = surface->format;

	return 0;
}

int spritectl_unlock_surface(spritectl_surface_t surface) {
	if (!surface || surface->locked == 0) {
		return -1;
	}

	SDL_UnlockSurface(surface->surface);
	surface->locked--;

	return 0;
}

int spritectl_clear_surface(spritectl_surface_t surface, uint32_t color) {
	SDL_Rect rect;

	if (!surface) {
		return -1;
	}

	/* Fill entire surface with color */
	rect.x = 0;
	rect.y = 0;
	rect.w = surface->width;
	rect.h = surface->height;

	SDL_FillRect(surface->surface, &rect, (Uint32)color);

	return 0;
}

int spritectl_get_surface_size(spritectl_surface_t surface, int* width, int* height) {
	if (!surface) {
		return -1;
	}

	if (width) *width = surface->width;
	if (height) *height = surface->height;

	return 0;
}

/* ============================================================================
 * Sprite Functions
 * ============================================================================ */

spritectl_sprite_t spritectl_create_sprite(int width, int height, int format,
                                           const void* pixels, size_t data_size) {
	spritectl_sprite_t sprite;

	if (!pixels || data_size == 0) {
		return SPRITECTL_INVALID_SPRITE;
	}

	/* Allocate sprite structure */
	sprite = (spritectl_sprite_t)malloc(sizeof(struct spritectl_sprite_s));
	if (!sprite) {
		fprintf(stderr, "SpriteLib Backend: Failed to allocate sprite\n");
		return SPRITECTL_INVALID_SPRITE;
	}

	/* Initialize fields */
	sprite->width = width;
	sprite->height = height;
	sprite->format = format;
	sprite->data_size = data_size;
	sprite->rgba_pixels = NULL;
	sprite->ref_count = 1;

	/* Allocate and copy pixel data */
	sprite->pixels = (uint16_t*)malloc(data_size);
	if (!sprite->pixels) {
		free(sprite);
		return SPRITECTL_INVALID_SPRITE;
	}
	memcpy(sprite->pixels, pixels, data_size);

	return sprite;
}

void spritectl_destroy_sprite(spritectl_sprite_t sprite) {
	if (!sprite) {
		return;
	}

	sprite->ref_count--;
	if (sprite->ref_count > 0) {
		return;  /* Still referenced */
	}

	/* Free pixel data */
	if (sprite->pixels) {
		free(sprite->pixels);
	}
	if (sprite->rgba_pixels) {
		free(sprite->rgba_pixels);
	}

	free(sprite);
}

int spritectl_get_sprite_info(spritectl_sprite_t sprite, spritectl_sprite_info_t* info) {
	if (!sprite || !info) {
		return -1;
	}

	info->width = sprite->width;
	info->height = sprite->height;
	info->format = sprite->format;
	info->data_size = sprite->data_size;

	return 0;
}

size_t spritectl_get_sprite_data(spritectl_sprite_t sprite, void* buffer, size_t buffer_size) {
	if (!sprite) {
		return 0;
	}

	if (!buffer) {
		return sprite->data_size;  /* Return required size */
	}

	if (buffer_size < sprite->data_size) {
		return 0;  /* Buffer too small */
	}

	memcpy(buffer, sprite->pixels, sprite->data_size);
	return sprite->data_size;
}

/* ============================================================================
 * Blitting Functions
 * ============================================================================ */

int spritectl_blt_sprite(spritectl_surface_t dest, int x, int y,
                         spritectl_sprite_t sprite, int flags, int alpha) {
	SDL_Rect dest_rect;
	SDL_Surface* src_surface = NULL;
	uint32_t* rgba_pixels = NULL;
	int result = -1;

	if (!dest || !sprite) {
		return -1;
	}

	/* Lock destination surface */
	if (SDL_MUSTLOCK(dest->surface)) {
		SDL_LockSurface(dest->surface);
	}

	/* Create temporary surface from sprite pixels */
	src_surface = SDL_CreateRGBSurface(0, sprite->width, sprite->height, 32,
	                                  0xFF, 0xFF00, 0xFF0000, 0xFF000000);
	if (!src_surface) {
		if (SDL_MUSTLOCK(dest->surface)) {
			SDL_UnlockSurface(dest->surface);
		}
		return -1;
	}

	/* Convert sprite pixels to RGBA32 */
	rgba_pixels = (uint32_t*)malloc(sprite->width * sprite->height * sizeof(uint32_t));
	if (!rgba_pixels) {
		SDL_FreeSurface(src_surface);
		if (SDL_MUSTLOCK(dest->surface)) {
			SDL_UnlockSurface(dest->surface);
		}
		return -1;
	}

	/* Convert based on sprite format */
	if (sprite->format == SPRITECTL_FORMAT_RGB565) {
		spritectl_convert_565_to_rgba(sprite->pixels, rgba_pixels,
		                              sprite->width * sprite->height, 0xFFFF);
	} else if (sprite->format == SPRITECTL_FORMAT_RGB555) {
		spritectl_convert_555_to_rgba(sprite->pixels, rgba_pixels,
		                              sprite->width * sprite->height, 0xFFFF);
	}

	/* Copy to temporary surface */
	SDL_LockSurface(src_surface);
	uint32_t* src_pixels = (uint32_t*)src_surface->pixels;
	memcpy(src_pixels, rgba_pixels, sprite->width * sprite->height * sizeof(uint32_t));
	SDL_UnlockSurface(src_surface);

	free(rgba_pixels);

	/* Handle alpha blending */
	if (flags & SPRITECTL_BLT_ALPHA) {
		SDL_SetSurfaceAlphaMod(src_surface, alpha);
	}

	/* Set up destination rectangle */
	dest_rect.x = x;
	dest_rect.y = y;
	dest_rect.w = sprite->width;
	dest_rect.h = sprite->height;

	/* Blit sprite to destination */
	if (SDL_BlitSurface(src_surface, NULL, dest->surface, &dest_rect) != 0) {
		fprintf(stderr, "SpriteLib Backend: SDL_BlitSurface failed: %s\n", SDL_GetError());
		result = -1;
	} else {
		result = 0;
	}

	/* Cleanup */
	SDL_FreeSurface(src_surface);
	if (SDL_MUSTLOCK(dest->surface)) {
		SDL_UnlockSurface(dest->surface);
	}

	return result;
}

int spritectl_blt_sprite_scaled(spritectl_surface_t dest, int x, int y,
                                spritectl_sprite_t sprite, int scale, int flags) {
	SDL_Surface* src_surface = NULL;
	SDL_Surface* scaled_surface = NULL;
	uint32_t* rgba_pixels = NULL;
	int result = -1;
	int scaled_width, scaled_height;

	if (!dest || !sprite) {
		return -1;
	}

	/* Calculate scaled dimensions (scale is in 1/256 units) */
	scaled_width = (sprite->width * scale) / 256;
	scaled_height = (sprite->height * scale) / 256;

	if (scaled_width <= 0 || scaled_height <= 0) {
		return 0;  /* Too small to see */
	}

	/* Create temporary surface from sprite pixels */
	src_surface = SDL_CreateRGBSurface(0, sprite->width, sprite->height, 32,
	                                  0xFF, 0xFF00, 0xFF0000, 0xFF000000);
	if (!src_surface) {
		return -1;
	}

	/* Convert sprite pixels to RGBA32 */
	rgba_pixels = (uint32_t*)malloc(sprite->width * sprite->height * sizeof(uint32_t));
	if (!rgba_pixels) {
		SDL_FreeSurface(src_surface);
		return -1;
	}

	/* Convert based on sprite format */
	if (sprite->format == SPRITECTL_FORMAT_RGB565) {
		spritectl_convert_565_to_rgba(sprite->pixels, rgba_pixels,
		                              sprite->width * sprite->height, 0xFFFF);
	} else if (sprite->format == SPRITECTL_FORMAT_RGB555) {
		spritectl_convert_555_to_rgba(sprite->pixels, rgba_pixels,
		                              sprite->width * sprite->height, 0xFFFF);
	}

	/* Copy to temporary surface */
	SDL_LockSurface(src_surface);
	uint32_t* src_pixels = (uint32_t*)src_surface->pixels;
	memcpy(src_pixels, rgba_pixels, sprite->width * sprite->height * sizeof(uint32_t));
	SDL_UnlockSurface(src_surface);

	free(rgba_pixels);

	/* Create scaled surface */
	scaled_surface = SDL_CreateRGBSurface(0, scaled_width, scaled_height, 32,
	                                    0xFF, 0xFF00, 0xFF0000, 0xFF000000);
	if (!scaled_surface) {
		SDL_FreeSurface(src_surface);
		return -1;
	}

	/* Scale the surface */
	SDL_SoftStretch(src_surface, NULL, scaled_surface, NULL);

	/* Blit scaled sprite to destination */
	SDL_Rect dest_rect;
	dest_rect.x = x;
	dest_rect.y = y;
	dest_rect.w = scaled_width;
	dest_rect.h = scaled_height;

	if (SDL_BlitSurface(scaled_surface, NULL, dest->surface, &dest_rect) != 0) {
		fprintf(stderr, "SpriteLib Backend: SDL_BlitSurface failed: %s\n", SDL_GetError());
		result = -1;
	} else {
		result = 0;
	}

	/* Cleanup */
	SDL_FreeSurface(scaled_surface);
	SDL_FreeSurface(src_surface);

	return result;
}

int spritectl_blt_surface(spritectl_surface_t dest,
                          const RECT* dest_rect,
                          spritectl_surface_t src,
                          const RECT* src_rect) {
	SDL_Rect sdl_dest_rect, sdl_src_rect;

	if (!dest || !src) {
		return -1;
	}

	/* Convert RECT to SDL_Rect */
	if (dest_rect) {
		sdl_dest_rect.x = dest_rect->left;
		sdl_dest_rect.y = dest_rect->top;
		sdl_dest_rect.w = dest_rect->right - dest_rect->left;
		sdl_dest_rect.h = dest_rect->bottom - dest_rect->top;
	} else {
		sdl_dest_rect.x = 0;
		sdl_dest_rect.y = 0;
		sdl_dest_rect.w = dest->width;
		sdl_dest_rect.h = dest->height;
	}

	if (src_rect) {
		sdl_src_rect.x = src_rect->left;
		sdl_src_rect.y = src_rect->top;
		sdl_src_rect.w = src_rect->right - src_rect->left;
		sdl_src_rect.h = src_rect->bottom - src_rect->top;
	} else {
		sdl_src_rect.x = 0;
		sdl_src_rect.y = 0;
		sdl_src_rect.w = src->width;
		sdl_src_rect.h = src->height;
	}

	/* Blit surface to surface */
	if (SDL_BlitSurface(src->surface, &sdl_src_rect, dest->surface, &sdl_dest_rect) != 0) {
		fprintf(stderr, "SpriteLib Backend: SDL_BlitSurface failed: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

/* ============================================================================
 * Sprite Pack Functions
 * ============================================================================ */

int spritectl_load_pack(const char* filename, spritectl_pack_t* pack) {
	FILE* file;
	uint16_t count;
	spritectl_pack_t pack_ptr = NULL;
	int result = -1;

	if (!filename || !pack) {
		return -1;
	}

	/* Open pack file */
	file = fopen(filename, "rb");
	if (!file) {
		fprintf(stderr, "SpriteLib Backend: Failed to open pack file: %s\n", filename);
		return -2;
	}

	/* Read sprite count (2 bytes, little-endian) */
	if (fread(&count, 2, 1, file) != 1) {
		fclose(file);
		return -4;
	}

	/* Allocate pack structure */
	pack_ptr = (spritectl_pack_t)malloc(sizeof(struct spritectl_pack_s));
	if (!pack_ptr) {
		fclose(file);
		return -3;
	}

	/* Initialize pack structure */
	pack_ptr->filename = strdup(filename);
	pack_ptr->file = file;
	pack_ptr->sprite_count = count;
	pack_ptr->lazy_loading = 0;  /* Full load */
	pack_ptr->ref_count = 1;

	/* Allocate sprite array */
	if (count > 0) {
		pack_ptr->sprites = (spritectl_sprite_t*)calloc(count, sizeof(spritectl_sprite_t));
		if (!pack_ptr->sprites) {
			free(pack_ptr->filename);
			free(pack_ptr);
			fclose(file);
			return -3;
		}

		pack_ptr->loaded_flags = (int*)calloc(count, sizeof(int));
		if (!pack_ptr->loaded_flags) {
			free(pack_ptr->sprites);
			free(pack_ptr->filename);
			free(pack_ptr);
			fclose(file);
			return -3;
		}

		pack_ptr->file_offsets = NULL;  /* Not needed for full load */
	} else {
		pack_ptr->sprites = NULL;
		pack_ptr->loaded_flags = NULL;
		pack_ptr->file_offsets = NULL;
	}

	/* Load each sprite */
	uint16_t colorkey = 0xFFFF;  /* Default color key */
	for (uint16_t i = 0; i < count; i++) {
		int load_result = spritectl_load_sprite_from_file(file, &pack_ptr->sprites[i], colorkey);
		if (load_result != 0) {
			pack_ptr->sprites[i] = SPRITECTL_INVALID_SPRITE;
			pack_ptr->loaded_flags[i] = 0;
		} else {
			pack_ptr->loaded_flags[i] = 1;
		}
	}

	fclose(file);
	pack_ptr->file = NULL;

	*pack = pack_ptr;
	return 0;
}

void spritectl_free_pack(spritectl_pack_t pack) {
	if (!pack) {
		return;
	}

	/* Free all sprites */
	if (pack->sprites) {
		for (int i = 0; i < pack->sprite_count; i++) {
			if (pack->sprites[i] != SPRITECTL_INVALID_SPRITE) {
				spritectl_destroy_sprite(pack->sprites[i]);
			}
		}
		free(pack->sprites);
		pack->sprites = NULL;
	}

	/* Free other resources */
	if (pack->filename) {
		free(pack->filename);
		pack->filename = NULL;
	}
	if (pack->loaded_flags) {
		free(pack->loaded_flags);
		pack->loaded_flags = NULL;
	}
	if (pack->file_offsets) {
		free(pack->file_offsets);
		pack->file_offsets = NULL;
	}
	if (pack->file) {
		fclose(pack->file);
		pack->file = NULL;
	}

	free(pack);
}

int spritectl_get_sprite_from_pack(spritectl_pack_t pack, int index,
                                   spritectl_sprite_t* sprite) {
	if (!pack || !sprite) {
		return -1;
	}

	/* Check index bounds */
	if (index < 0 || index >= pack->sprite_count) {
		return -2;
	}

	/* Return sprite (or invalid handle if not loaded) */
	*sprite = pack->sprites[index];
	return 0;
}

int spritectl_get_pack_count(spritectl_pack_t pack) {
	if (!pack) {
		return 0;
	}
	return pack->sprite_count;
}

/* ============================================================================
 * Pixel Conversion Functions
 * ============================================================================ */

void spritectl_convert_565_to_555(const uint16_t* src, uint16_t* dest, int pixel_count) {
	int i;
	for (i = 0; i < pixel_count; i++) {
		uint16_t pixel = src[i];
		/* Convert 565 to 555: drop the low bit of green */
		dest[i] = ((pixel & 0xFFC0) >> 1) | (pixel & 0x001F);
	}
}

void spritectl_convert_555_to_565(const uint16_t* src, uint16_t* dest, int pixel_count) {
	int i;
	for (i = 0; i < pixel_count; i++) {
		uint16_t pixel = src[i];
		/* Convert 555 to 565: insert a zero bit for green's low bit */
		dest[i] = ((pixel & 0x7FE0) << 1) | (pixel & 0x001F);
	}
}

void spritectl_convert_565_to_rgba(const uint16_t* src, uint32_t* dest,
                                   int pixel_count, uint16_t colorkey) {
	int i;
	for (i = 0; i < pixel_count; i++) {
		uint16_t pixel = src[i];
		uint8_t r, g, b, a;

		if (pixel == colorkey) {
			/* Transparent pixel */
			a = 0;
		} else {
			a = 255;
		}

		spritectl_565_to_rgb(pixel, &r, &g, &b);
		dest[i] = spritectl_rgb_to_rgba(r, g, b, a);
	}
}

void spritectl_convert_555_to_rgba(const uint16_t* src, uint32_t* dest,
                                   int pixel_count, uint16_t colorkey) {
	int i;
	for (i = 0; i < pixel_count; i++) {
		uint16_t pixel = src[i];
		uint8_t r, g, b, a;

		if (pixel == colorkey) {
			/* Transparent pixel */
			a = 0;
		} else {
			a = 255;
		}

		spritectl_555_to_rgb(pixel, &r, &g, &b);
		dest[i] = spritectl_rgb_to_rgba(r, g, b, a);
	}
}

/* ============================================================================
 * RLE Sprite Decoding
 * ============================================================================ */

/**
 * Decode RLE-compressed sprite data to RGB565 pixels
 *
 * RLE Format (per scanline):
 *   - Length (2 bytes, uint16_t) - number of WORDs in RLE data
 *   - RLE Data (Length * 2 bytes):
 *     - Count (1 WORD) - number of segments
 *     - For each segment:
 *       - Trans Count (1 WORD) - transparent pixel count
 *       - Color Count (1 WORD) - color pixel count
 *       - Pixels (Color Count WORDs) - RGB565 pixel data
 */
int spritectl_decode_rle_data(const uint16_t* rle_data, int rle_len,
                               uint16_t* pixels_out, int width) {
	int rle_index = 0;
	int x = 0;

	/* Read segment count */
	if (rle_len == 0) {
		return 0;  /* Empty scanline */
	}

	int count = rle_data[rle_index++];
	rle_index = 1;

	/* Process each segment */
	for (int seg = 0; seg < count && x < width; seg++) {
		if (rle_index >= rle_len) break;

		/* Read transparent and color counts */
		int trans_count = rle_data[rle_index++];
		int color_count = rle_data[rle_index++];

		/* Skip transparent pixels (leave as 0) */
		x += trans_count;

		/* Copy color pixels */
		for (int c = 0; c < color_count && x < width; c++) {
			if (rle_index >= rle_len) break;
			pixels_out[x] = rle_data[rle_index++];
			x++;
		}
	}

	return 0;
}

int spritectl_decode_rle_sprite(const uint8_t* compressed, size_t compressed_size,
                                int width, int height,
                                uint16_t* pixels_out, size_t pixels_size) {
	FILE* file;
	uint16_t* scanline_lengths = NULL;
	uint8_t** scanlines = NULL;
	int result = -1;

	/* For now, we'll load from FILE pointer instead of raw buffer */
	/* This is a simpler implementation that matches the original */
	fprintf(stderr, "SpriteLib Backend: Use spritectl_load_sprite_from_file instead\n");
	return -1;
}

/**
 * Load sprite from file (RLE encoded)
 * File format:
 *   - Width (2 bytes, uint16_t)
 *   - Height (2 bytes, uint16_t)
 *   - For each scanline:
 *     - Length (2 bytes, uint16_t)
 *     - RLE data (Length * 2 bytes)
 */
int spritectl_load_sprite_from_file(FILE* file, spritectl_sprite_t* sprite_out,
                                    uint16_t colorkey) {
	uint16_t width = 0, height = 0;
	uint16_t* scanline_lengths = NULL;
	uint16_t** scanline_rle = NULL;
	spritectl_sprite_t sprite = NULL;
	size_t pixel_count = 0;
	uint16_t* pixels = NULL;
	int result = -1;

	if (!file || !sprite_out) {
		return -1;
	}

	/* Read width and height */
	if (fread(&width, 2, 1, file) != 1) goto cleanup;
	if (fread(&height, 2, 1, file) != 1) goto cleanup;

	/* Handle empty sprite */
	if (width == 0 || height == 0) {
		*sprite_out = SPRITECTL_INVALID_SPRITE;
		return 0;
	}

	pixel_count = width * height;

	/* Allocate scanline length array */
	scanline_lengths = (uint16_t*)calloc(height, sizeof(uint16_t));
	if (!scanline_lengths) goto cleanup;

	/* Allocate RLE data array */
	scanline_rle = (uint16_t**)calloc(height, sizeof(uint16_t*));
	if (!scanline_rle) goto cleanup;

	/* Read each scanline */
	for (int y = 0; y < height; y++) {
		/* Read scanline data length */
		if (fread(&scanline_lengths[y], 2, 1, file) != 1) goto cleanup;

		if (scanline_lengths[y] > 0) {
			/* Allocate and read RLE data */
			scanline_rle[y] = (uint16_t*)malloc(scanline_lengths[y] * sizeof(uint16_t));
			if (!scanline_rle[y]) goto cleanup;

			if (fread(scanline_rle[y], 2, scanline_lengths[y], file) != scanline_lengths[y]) {
				free(scanline_rle[y]);
				scanline_rle[y] = NULL;
				goto cleanup;
			}
		}
	}

	/* Decode RLE to raw pixels */
	pixels = (uint16_t*)calloc(pixel_count, sizeof(uint16_t));
	if (!pixels) goto cleanup;

	/* Decode each scanline */
	for (int y = 0; y < height; y++) {
		uint16_t* row = pixels + (y * width);

		if (scanline_rle[y] && scanline_lengths[y] > 0) {
			/* Decode RLE data */
			int rle_index = 0;
			int x = 0;

			/* Read segment count */
			int count = scanline_rle[y][rle_index++];

			/* Process each segment */
			for (int seg = 0; seg < count && x < width; seg++) {
				int trans_count = scanline_rle[y][rle_index++];
				int color_count = scanline_rle[y][rle_index++];

				/* Skip transparent pixels (already 0) */
				x += trans_count;

				/* Copy color pixels */
				for (int c = 0; c < color_count && x < width; c++) {
					row[x] = scanline_rle[y][rle_index++];
					x++;
				}
			}
		}
	}

	/* Create sprite structure */
	sprite = spritectl_create_sprite(width, height, SPRITECTL_FORMAT_RGB565,
	                                  pixels, pixel_count * sizeof(uint16_t));
	if (!sprite) {
		free(pixels);
		goto cleanup;
	}

	free(pixels);  /* Sprite copied the data */
	*sprite_out = sprite;
	result = 0;

cleanup:
	if (scanline_rle) {
		for (int y = 0; y < height; y++) {
			if (scanline_rle[y]) {
				free(scanline_rle[y]);
			}
		}
		free(scanline_rle);
	}
	if (scanline_lengths) {
		free(scanline_lengths);
	}

	return result;
}
