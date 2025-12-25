/**
 * @file index_sprite.h
 * @brief IndexedSprite for color-mapped creature rendering
 * 
 * Requirements: 1.4, 1.5, 1.6
 * 
 * IndexedSprite stores pixel data as indices into ColorSet tables,
 * allowing runtime color customization for creatures.
 */

#ifndef INDEX_SPRITE_H
#define INDEX_SPRITE_H

#include <SDL.h>
#include <stdint.h>
#include <stdio.h>
#include "colorset.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * IndexSprite Structure
 * ============================================================================ */

/**
 * IndexedSprite structure
 * 
 * Stores pixel data in a compressed format where each pixel is either:
 * - Transparent (INDEX_TRANS)
 * - Indexed color (setNumber << 8 | gradation)
 * - Direct color (RGB565)
 * 
 * The data is stored per-scanline in a run-length encoded format.
 */
typedef struct {
    uint16_t width;          /**< Width in pixels */
    uint16_t height;         /**< Height in pixels */
    uint16_t** pixels;       /**< Scanline data array */
    int is_init;             /**< Whether sprite is initialized */
} IndexSprite;

/* ============================================================================
 * Functions
 * ============================================================================ */

/**
 * Initialize an IndexSprite structure
 * 
 * @param sprite Pointer to IndexSprite
 */
void index_sprite_init(IndexSprite* sprite);

/**
 * Release IndexSprite resources
 * 
 * @param sprite Pointer to IndexSprite
 */
void index_sprite_release(IndexSprite* sprite);

/**
 * Check if sprite is initialized
 * 
 * @param sprite Pointer to IndexSprite
 * @return 1 if initialized, 0 otherwise
 */
int index_sprite_is_init(const IndexSprite* sprite);

/**
 * Get sprite width
 * 
 * @param sprite Pointer to IndexSprite
 * @return Width in pixels
 */
uint16_t index_sprite_get_width(const IndexSprite* sprite);

/**
 * Get sprite height
 * 
 * @param sprite Pointer to IndexSprite
 * @return Height in pixels
 */
uint16_t index_sprite_get_height(const IndexSprite* sprite);

/**
 * Load IndexSprite from file (RGB565 format)
 * 
 * @param sprite Pointer to IndexSprite
 * @param file File handle positioned at sprite data
 * @return 1 on success, 0 on failure
 * 
 * Requirements: 1.4
 */
int index_sprite_load_from_file(IndexSprite* sprite, FILE* file);

/**
 * Render IndexSprite to pixel buffer using current ColorSet
 * 
 * @param sprite Pointer to IndexSprite
 * @param dest Destination RGB565 pixel buffer
 * @param pitch Destination buffer pitch in bytes
 * 
 * Requirements: 1.5, 1.6
 */
void index_sprite_blt(const IndexSprite* sprite, uint16_t* dest, uint16_t pitch);

/**
 * Render IndexSprite with specific ColorSet override
 * 
 * @param sprite Pointer to IndexSprite
 * @param dest Destination RGB565 pixel buffer
 * @param pitch Destination buffer pitch in bytes
 * @param colorSet ColorSet index to use for all indexed pixels
 */
void index_sprite_blt_colorset(const IndexSprite* sprite, uint16_t* dest, 
                               uint16_t pitch, uint16_t colorSet);

/**
 * Render IndexSprite with darkness effect
 * 
 * @param sprite Pointer to IndexSprite
 * @param dest Destination RGB565 pixel buffer
 * @param pitch Destination buffer pitch in bytes
 * @param darkBits Darkness level (0-4)
 */
void index_sprite_blt_darkness(const IndexSprite* sprite, uint16_t* dest,
                               uint16_t pitch, uint8_t darkBits);

/**
 * Render IndexSprite with alpha blending
 * 
 * @param sprite Pointer to IndexSprite
 * @param dest Destination RGB565 pixel buffer
 * @param pitch Destination buffer pitch in bytes
 * @param alpha Alpha value (0-255)
 */
void index_sprite_blt_alpha(const IndexSprite* sprite, uint16_t* dest,
                            uint16_t pitch, uint8_t alpha);

/**
 * Render IndexSprite with additive effect
 * 
 * @param sprite Pointer to IndexSprite
 * @param dest Destination RGB565 pixel buffer
 * @param pitch Destination buffer pitch in bytes
 */
void index_sprite_blt_effect(const IndexSprite* sprite, uint16_t* dest,
                             uint16_t pitch);

/**
 * Render IndexSprite to SDL texture
 * 
 * @param sprite Pointer to IndexSprite
 * @param renderer SDL renderer
 * @param x X position
 * @param y Y position
 * @return 0 on success, negative on error
 */
int index_sprite_render(const IndexSprite* sprite, SDL_Renderer* renderer,
                        int x, int y);

/**
 * Render IndexSprite to SDL texture with specific ColorSet
 * 
 * @param sprite Pointer to IndexSprite
 * @param renderer SDL renderer
 * @param x X position
 * @param y Y position
 * @param colorSet ColorSet index to use
 * @return 0 on success, negative on error
 */
int index_sprite_render_colorset(const IndexSprite* sprite, SDL_Renderer* renderer,
                                 int x, int y, uint16_t colorSet);

/**
 * Create SDL texture from IndexSprite
 * 
 * @param sprite Pointer to IndexSprite
 * @param renderer SDL renderer
 * @return SDL_Texture pointer, or NULL on error
 */
SDL_Texture* index_sprite_create_texture(const IndexSprite* sprite, 
                                         SDL_Renderer* renderer);

/**
 * Create SDL texture from IndexSprite with specific ColorSet
 * 
 * @param sprite Pointer to IndexSprite
 * @param renderer SDL renderer
 * @param colorSet ColorSet index to use
 * @return SDL_Texture pointer, or NULL on error
 */
SDL_Texture* index_sprite_create_texture_colorset(const IndexSprite* sprite,
                                                  SDL_Renderer* renderer,
                                                  uint16_t colorSet);

#ifdef __cplusplus
}
#endif

#endif /* INDEX_SPRITE_H */
