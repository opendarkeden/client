/**
 * @file ui_global_resource.c
 * @brief Shared UI resources implementation
 * 
 * Requirements: 6.1, 6.2, 6.3, 6.4, 6.5, 6.6
 */

#include "ui_global_resource.h"
#include "sprite.h"
#include <string.h>
#include <stdio.h>

/* Fallback colors (RGBA format) */
#define DIALOG_BG_COLOR     0x202040FF
#define DIALOG_BORDER_COLOR 0x8080A0FF

/* ============================================================================
 * Lifecycle
 * ============================================================================ */

int ui_global_resource_init(UI_GlobalResource* gr, SDL_Renderer* renderer, UI_Surface* surface,
                            const char* dialog_pack_path, const char* button_pack_path) {
    if (!gr || !renderer) {
        return -1;
    }

    memset(gr, 0, sizeof(UI_GlobalResource));
    gr->renderer = renderer;
    gr->surface = surface;
    gr->dialog_loaded = 0;
    gr->button_loaded = 0;

    /* Try to load dialog pack */
    if (dialog_pack_path) {
        if (spritepack_load(&gr->dialog_pack, dialog_pack_path) == 0) {
            gr->dialog_loaded = 1;
        } else {
            fprintf(stderr, "Warning: Failed to load dialog pack: %s\n", dialog_pack_path);
        }
    }

    /* Try to load button pack */
    if (button_pack_path) {
        if (spritepack_load(&gr->button_pack, button_pack_path) == 0) {
            gr->button_loaded = 1;
        } else {
            fprintf(stderr, "Warning: Failed to load button pack: %s\n", button_pack_path);
        }
    }

    gr->initialized = 1;
    return 0;
}

void ui_global_resource_free(UI_GlobalResource* gr) {
    if (!gr) {
        return;
    }

    if (gr->dialog_loaded) {
        spritepack_free(&gr->dialog_pack);
    }

    if (gr->button_loaded) {
        spritepack_free(&gr->button_pack);
    }

    memset(gr, 0, sizeof(UI_GlobalResource));
}

/* ============================================================================
 * Helper: Decode and render sprite
 * ============================================================================ */

static void render_sprite_at(UI_GlobalResource* gr, Sprite* sprite, int x, int y) {
    if (!gr || !gr->surface || !sprite || !sprite->is_valid) {
        return;
    }

    DecodedSprite decoded = {0};
    if (sprite_decode(sprite, &decoded, 0) == 0) {
        if (decoded_sprite_create_texture(&decoded, gr->renderer) == 0) {
            ui_surface_blit_sprite(gr->surface, x, y, &decoded);
        }
        decoded_sprite_free(&decoded);
    }
}

/* ============================================================================
 * Drawing
 * ============================================================================ */

void ui_global_resource_draw_dialog(UI_GlobalResource* gr, int x, int y, int w, int h, int use_alpha) {
    if (!gr || !gr->surface) {
        return;
    }

    (void)use_alpha; /* TODO: implement alpha support */

    /* If no dialog sprites loaded, use fallback */
    if (!gr->dialog_loaded) {
        SDL_Rect rect = { x, y, w, h };
        ui_surface_fill_rect(gr->surface, &rect, DIALOG_BG_COLOR);
        ui_surface_draw_rect(gr->surface, &rect, DIALOG_BORDER_COLOR);
        return;
    }

    /* Get sprites */
    Sprite* bg_sprite = spritepack_get(&gr->dialog_pack, AB_BACK);
    Sprite* right_sprite = spritepack_get(&gr->dialog_pack, AB_RIGHT);
    Sprite* down_sprite = spritepack_get(&gr->dialog_pack, AB_DOWN);
    Sprite* corner_sprite = spritepack_get(&gr->dialog_pack, AB_RIGHTDOWN);

    /* Get sprite dimensions for tiling */
    int tile_w = bg_sprite && bg_sprite->is_valid ? bg_sprite->width : 16;
    int tile_h = bg_sprite && bg_sprite->is_valid ? bg_sprite->height : 16;

    /* Tile background */
    if (bg_sprite && bg_sprite->is_valid) {
        for (int ty = y; ty < y + h - tile_h; ty += tile_h) {
            for (int tx = x; tx < x + w - tile_w; tx += tile_w) {
                render_sprite_at(gr, bg_sprite, tx, ty);
            }
        }
    } else {
        /* Fallback: fill with solid color */
        SDL_Rect bg_rect = { x, y, w - tile_w, h - tile_h };
        ui_surface_fill_rect(gr->surface, &bg_rect, DIALOG_BG_COLOR);
    }

    /* Draw right edge */
    if (right_sprite && right_sprite->is_valid) {
        int edge_x = x + w - right_sprite->width;
        for (int ty = y; ty < y + h - tile_h; ty += right_sprite->height) {
            render_sprite_at(gr, right_sprite, edge_x, ty);
        }
    }

    /* Draw bottom edge */
    if (down_sprite && down_sprite->is_valid) {
        int edge_y = y + h - down_sprite->height;
        for (int tx = x; tx < x + w - tile_w; tx += down_sprite->width) {
            render_sprite_at(gr, down_sprite, tx, edge_y);
        }
    }

    /* Draw corner */
    if (corner_sprite && corner_sprite->is_valid) {
        int corner_x = x + w - corner_sprite->width;
        int corner_y = y + h - corner_sprite->height;
        render_sprite_at(gr, corner_sprite, corner_x, corner_y);
    }
}

void ui_global_resource_draw_outbox(UI_GlobalResource* gr, int x, int y, int w, int h) {
    if (!gr || !gr->surface) {
        return;
    }

    SDL_Rect rect = { x, y, w, h };
    ui_surface_draw_rect(gr->surface, &rect, DIALOG_BORDER_COLOR);
}

/* ============================================================================
 * Accessors
 * ============================================================================ */

SpritePack* ui_global_resource_get_button_pack(UI_GlobalResource* gr) {
    if (!gr || !gr->button_loaded) {
        return NULL;
    }
    return &gr->button_pack;
}

SpritePack* ui_global_resource_get_dialog_pack(UI_GlobalResource* gr) {
    if (!gr || !gr->dialog_loaded) {
        return NULL;
    }
    return &gr->dialog_pack;
}

UI_Surface* ui_global_resource_get_surface(UI_GlobalResource* gr) {
    return gr ? gr->surface : NULL;
}
