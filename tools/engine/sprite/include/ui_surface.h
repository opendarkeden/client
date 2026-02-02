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

#include <stdint.h>
#include "sprite.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UI_Surface UI_Surface;

typedef struct {
    int x, y;
    int w, h;
} UI_Rect;

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
 * Allocate and initialize a UI surface
 * @param renderer SDL renderer to use
 * @param width Surface width in pixels
 * @param height Surface height in pixels
 * @return Pointer to allocated UI_Surface, or NULL on failure
 */
UI_Surface* ui_surface_create(SDL_Renderer* renderer, int width, int height);

SDL_Renderer* ui_surface_get_render(UI_Surface *surface);

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
void ui_surface_fill_rect(UI_Surface* surface, UI_Rect* rect, uint32_t color);

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
void ui_surface_draw_rect(UI_Surface* surface, UI_Rect* rect, uint32_t color);

/**
 * Clear the entire surface to a color
 * @param surface Target surface
 * @param color RGBA color (0xRRGGBBAA format)
 */
void ui_surface_clear(UI_Surface* surface, uint32_t color);

/**
 * Blit from one UI_Surface to another.
 * @param dst Destination surface
 * @param dst_rect Destination rectangle (NULL for full dst)
 * @param src Source surface
 * @param src_rect Source rectangle (NULL for full src)
 */
void ui_surface_blit_surface(UI_Surface* dst, const UI_Rect* dst_rect,
                             UI_Surface* src, const UI_Rect* src_rect);

struct SDL_Texture;

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
