/**
 * @file ui_surface.h
 * @brief SDL2 surface wrapper for UI rendering
 * 
 * Provides a lockable texture surface for sprite blitting and primitive drawing.
 * This is the C equivalent of CSpriteSurfaceSDL.
 * 
 * Requirements: 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
 */

#ifndef UI_SURFACE_H
#define UI_SURFACE_H

#include <SDL.h>
#include <stdint.h>
#include "sprite.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * UI Surface structure - wraps SDL_Texture for UI rendering
 */
typedef struct {
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int width;
    int height;
    int pitch;
    void* pixels;       /* Valid only when locked */
    int locked;
} UI_Surface;

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

/**
 * Initialize a UI surface with a streaming texture
 * @param surface Pointer to surface structure
 * @param renderer SDL renderer to use
 * @param width Surface width in pixels
 * @param height Surface height in pixels
 * @return 0 on success, negative error code on failure
 */
int ui_surface_init(UI_Surface* surface, SDL_Renderer* renderer, int width, int height);

/**
 * Free surface resources
 * @param surface Pointer to surface structure
 */
void ui_surface_free(UI_Surface* surface);

/* ============================================================================
 * Lock/Unlock for pixel access
 * ============================================================================ */

/**
 * Lock surface for direct pixel access
 * @param surface Pointer to surface structure
 * @return 0 on success, negative error code on failure
 */
int ui_surface_lock(UI_Surface* surface);

/**
 * Unlock surface after pixel access
 * @param surface Pointer to surface structure
 * @return 0 on success, negative error code on failure
 */
int ui_surface_unlock(UI_Surface* surface);

/**
 * Check if surface is locked
 * @param surface Pointer to surface structure
 * @return 1 if locked, 0 otherwise
 */
int ui_surface_is_locked(UI_Surface* surface);

/* ============================================================================
 * Sprite blitting
 * ============================================================================ */

/**
 * Blit a decoded sprite to the surface
 * @param surface Target surface
 * @param x X position
 * @param y Y position
 * @param sprite Decoded sprite to blit
 */
void ui_surface_blit_sprite(UI_Surface* surface, int x, int y, DecodedSprite* sprite);

/**
 * Blit sprite with alpha blending
 * @param surface Target surface
 * @param x X position
 * @param y Y position
 * @param sprite Decoded sprite to blit
 * @param alpha Alpha value (0-255, 0=transparent, 255=opaque)
 */
void ui_surface_blit_sprite_alpha(UI_Surface* surface, int x, int y, DecodedSprite* sprite, uint8_t alpha);

/**
 * Blit sprite with color tinting
 * @param surface Target surface
 * @param x X position
 * @param y Y position
 * @param sprite Decoded sprite to blit
 * @param color RGB color to tint (0x00RRGGBB format)
 */
void ui_surface_blit_sprite_color(UI_Surface* surface, int x, int y, DecodedSprite* sprite, uint32_t color);

/**
 * Blit sprite with darkness effect
 * @param surface Target surface
 * @param x X position
 * @param y Y position
 * @param sprite Decoded sprite to blit
 * @param darkness Darkness level (0=normal, 255=black)
 */
void ui_surface_blit_sprite_darkness(UI_Surface* surface, int x, int y, DecodedSprite* sprite, uint8_t darkness);

/* ============================================================================
 * Primitive drawing
 * ============================================================================ */

/**
 * Fill a rectangle with a solid color
 * @param surface Target surface
 * @param rect Rectangle to fill (NULL for entire surface)
 * @param color RGBA color (0xRRGGBBAA format)
 */
void ui_surface_fill_rect(UI_Surface* surface, SDL_Rect* rect, uint32_t color);

/**
 * Draw a horizontal line
 * @param surface Target surface
 * @param x Starting X position
 * @param y Y position
 * @param length Line length in pixels
 * @param color RGBA color (0xRRGGBBAA format)
 */
void ui_surface_hline(UI_Surface* surface, int x, int y, int length, uint32_t color);

/**
 * Draw a vertical line
 * @param surface Target surface
 * @param x X position
 * @param y Starting Y position
 * @param length Line length in pixels
 * @param color RGBA color (0xRRGGBBAA format)
 */
void ui_surface_vline(UI_Surface* surface, int x, int y, int length, uint32_t color);

/**
 * Draw a rectangle outline
 * @param surface Target surface
 * @param rect Rectangle to draw
 * @param color RGBA color (0xRRGGBBAA format)
 */
void ui_surface_draw_rect(UI_Surface* surface, SDL_Rect* rect, uint32_t color);

/**
 * Clear the entire surface to a color
 * @param surface Target surface
 * @param color RGBA color (0xRRGGBBAA format)
 */
void ui_surface_clear(UI_Surface* surface, uint32_t color);

/* ============================================================================
 * Accessors
 * ============================================================================ */

/**
 * Get the SDL texture for rendering
 * @param surface Pointer to surface structure
 * @return SDL_Texture pointer
 */
SDL_Texture* ui_surface_get_texture(UI_Surface* surface);

/**
 * Get surface dimensions
 * @param surface Pointer to surface structure
 * @param width Output width (can be NULL)
 * @param height Output height (can be NULL)
 */
void ui_surface_get_size(UI_Surface* surface, int* width, int* height);

#ifdef __cplusplus
}
#endif

#endif /* UI_SURFACE_H */
