/**
 * @file shadow_sprite.h
 * @brief ShadowSprite for creature shadow rendering
 * 
 * Requirements: 2.3, 2.4
 * 
 * ShadowSprite stores shadow data in a compressed run-length format.
 * Each scanline contains pairs of (transparent_count, shadow_count).
 * Shadow pixels are rendered by darkening the destination pixels.
 */

#ifndef SHADOW_SPRITE_H
#define SHADOW_SPRITE_H

#include <SDL.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * ShadowSprite Structure
 * ============================================================================ */

/**
 * ShadowSprite structure
 * 
 * Stores shadow data in a compressed format where each scanline contains:
 *   [count] - Number of (transparent, shadow) pairs
 *   For each pair:
 *     [transCount] - Number of transparent pixels
 *     [shadowCount] - Number of shadow pixels
 * 
 * Shadow pixels are rendered by darkening the destination.
 */
typedef struct {
    uint16_t width;          /**< Width in pixels */
    uint16_t height;         /**< Height in pixels */
    uint16_t** pixels;       /**< Scanline data array */
    int is_init;             /**< Whether sprite is initialized */
} ShadowSprite;

/* ============================================================================
 * Functions
 * ============================================================================ */

/**
 * Initialize a ShadowSprite structure
 * 
 * @param sprite Pointer to ShadowSprite
 */
void shadow_sprite_init(ShadowSprite* sprite);

/**
 * Release ShadowSprite resources
 * 
 * @param sprite Pointer to ShadowSprite
 */
void shadow_sprite_release(ShadowSprite* sprite);

/**
 * Check if sprite is initialized
 * 
 * @param sprite Pointer to ShadowSprite
 * @return 1 if initialized, 0 otherwise
 */
int shadow_sprite_is_init(const ShadowSprite* sprite);

/**
 * Get sprite width
 * 
 * @param sprite Pointer to ShadowSprite
 * @return Width in pixels
 */
uint16_t shadow_sprite_get_width(const ShadowSprite* sprite);

/**
 * Get sprite height
 * 
 * @param sprite Pointer to ShadowSprite
 * @return Height in pixels
 */
uint16_t shadow_sprite_get_height(const ShadowSprite* sprite);

/**
 * Load ShadowSprite from file
 * 
 * File format per scanline:
 *   [len: 2 bytes] - Length of scanline data in WORDs
 *   [data: len*2 bytes] - Scanline data
 * 
 * Scanline data format:
 *   [count] - Number of (trans, shadow) pairs
 *   For each pair:
 *     [transCount] - Transparent pixel count
 *     [shadowCount] - Shadow pixel count
 * 
 * @param sprite Pointer to ShadowSprite
 * @param file File handle positioned at sprite data
 * @return 1 on success, 0 on failure
 * 
 * Requirements: 2.3
 */
int shadow_sprite_load_from_file(ShadowSprite* sprite, FILE* file);

/**
 * Render shadow to RGB565 pixel buffer (full black shadow)
 * 
 * Shadow pixels are set to black (0x0000).
 * 
 * @param sprite Pointer to ShadowSprite
 * @param dest Destination RGB565 pixel buffer
 * @param pitch Destination buffer pitch in bytes
 * 
 * Requirements: 2.4
 */
void shadow_sprite_blt(const ShadowSprite* sprite, uint16_t* dest, uint16_t pitch);

/**
 * Render shadow with darkness effect
 * 
 * Shadow pixels darken the destination by shifting RGB values.
 * 
 * @param sprite Pointer to ShadowSprite
 * @param dest Destination RGB565 pixel buffer
 * @param pitch Destination buffer pitch in bytes
 * @param darkBits Darkness level (0-4), higher = darker
 * 
 * Requirements: 2.4
 */
void shadow_sprite_blt_darkness(const ShadowSprite* sprite, uint16_t* dest,
                                uint16_t pitch, uint8_t darkBits);

/**
 * Render shadow with alpha blending
 * 
 * Shadow pixels are blended with destination using alpha value.
 * 
 * @param sprite Pointer to ShadowSprite
 * @param dest Destination RGB565 pixel buffer
 * @param pitch Destination buffer pitch in bytes
 * @param alpha Alpha value (0-255), 0 = transparent, 255 = opaque shadow
 * 
 * Requirements: 2.4
 */
void shadow_sprite_blt_alpha(const ShadowSprite* sprite, uint16_t* dest,
                             uint16_t pitch, uint8_t alpha);

/**
 * Render shadow to SDL renderer
 * 
 * Creates a texture with shadow mask and renders it.
 * 
 * @param sprite Pointer to ShadowSprite
 * @param renderer SDL renderer
 * @param x X position
 * @param y Y position
 * @return 0 on success, negative on error
 */
int shadow_sprite_render(const ShadowSprite* sprite, SDL_Renderer* renderer,
                         int x, int y);

/**
 * Render shadow to SDL renderer with alpha
 * 
 * @param sprite Pointer to ShadowSprite
 * @param renderer SDL renderer
 * @param x X position
 * @param y Y position
 * @param alpha Alpha value (0-255)
 * @return 0 on success, negative on error
 */
int shadow_sprite_render_alpha(const ShadowSprite* sprite, SDL_Renderer* renderer,
                               int x, int y, uint8_t alpha);

/**
 * Create SDL texture from ShadowSprite
 * 
 * @param sprite Pointer to ShadowSprite
 * @param renderer SDL renderer
 * @param alpha Alpha value for shadow (0-255)
 * @return SDL_Texture pointer, or NULL on error
 */
SDL_Texture* shadow_sprite_create_texture(const ShadowSprite* sprite,
                                          SDL_Renderer* renderer,
                                          uint8_t alpha);

#ifdef __cplusplus
}
#endif

#endif /* SHADOW_SPRITE_H */
