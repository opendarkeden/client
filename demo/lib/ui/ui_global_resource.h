/**
 * @file ui_global_resource.h
 * @brief Shared UI resources (sprites, textures)
 * 
 * Manages global sprite packs for dialog boxes and buttons.
 * 
 * Requirements: 6.1, 6.2, 6.3, 6.4, 6.5, 6.6
 */

#ifndef UI_GLOBAL_RESOURCE_H
#define UI_GLOBAL_RESOURCE_H

#include <SDL.h>
#include "ui_surface.h"
#include "spritepack.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Sprite indices for dialog box assembly
 * ============================================================================ */

#define AB_BACK       0   /* Background tile */
#define AB_RIGHT      2   /* Right edge */
#define AB_DOWN       3   /* Bottom edge */
#define AB_RIGHTDOWN  4   /* Bottom-right corner */

/* ============================================================================
 * Sprite indices for buttons
 * ============================================================================ */

#define AB_BUTTON_OK               0
#define AB_BUTTON_CANCEL           1
#define AB_BUTTON_HILIGHTED_OFFSET 3   /* Add to base for hover state */
#define AB_BUTTON_PUSHED_OFFSET    6   /* Add to base for pressed state */

/* ============================================================================
 * Global resource structure
 * ============================================================================ */

typedef struct {
    SDL_Renderer* renderer;
    UI_Surface* surface;        /* Target surface for drawing */
    SpritePack dialog_pack;
    SpritePack button_pack;
    int dialog_loaded;
    int button_loaded;
    int initialized;
} UI_GlobalResource;

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

/**
 * Initialize global resources
 * @param gr Pointer to global resource structure
 * @param renderer SDL renderer
 * @param surface Target surface for drawing
 * @param dialog_pack_path Path to dialog sprite pack (can be NULL)
 * @param button_pack_path Path to button sprite pack (can be NULL)
 * @return 0 on success (even if packs fail to load - fallback available)
 */
int ui_global_resource_init(UI_GlobalResource* gr, SDL_Renderer* renderer, UI_Surface* surface,
                            const char* dialog_pack_path, const char* button_pack_path);

/**
 * Free global resources
 * @param gr Pointer to global resource structure
 */
void ui_global_resource_free(UI_GlobalResource* gr);

/* ============================================================================
 * Drawing
 * ============================================================================ */

/**
 * Draw dialog background
 * @param gr Pointer to global resource structure
 * @param x X position
 * @param y Y position
 * @param w Width
 * @param h Height
 * @param use_alpha Whether to use alpha blending
 */
void ui_global_resource_draw_dialog(UI_GlobalResource* gr, int x, int y, int w, int h, int use_alpha);

/**
 * Draw outbox (simple border)
 * @param gr Pointer to global resource structure
 * @param x X position
 * @param y Y position
 * @param w Width
 * @param h Height
 */
void ui_global_resource_draw_outbox(UI_GlobalResource* gr, int x, int y, int w, int h);

/* ============================================================================
 * Accessors
 * ============================================================================ */

/**
 * Get button sprite pack
 * @param gr Pointer to global resource structure
 * @return Pointer to button sprite pack, or NULL if not loaded
 */
SpritePack* ui_global_resource_get_button_pack(UI_GlobalResource* gr);

/**
 * Get dialog sprite pack
 * @param gr Pointer to global resource structure
 * @return Pointer to dialog sprite pack, or NULL if not loaded
 */
SpritePack* ui_global_resource_get_dialog_pack(UI_GlobalResource* gr);

/**
 * Get target surface
 * @param gr Pointer to global resource structure
 * @return Pointer to target surface
 */
UI_Surface* ui_global_resource_get_surface(UI_GlobalResource* gr);

#ifdef __cplusplus
}
#endif

#endif /* UI_GLOBAL_RESOURCE_H */
