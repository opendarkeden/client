/**
 * @file ui_game_minimap.c
 * @brief Game Minimap Implementation
 */

#include "ui_game_minimap.h"
#include "spritepack.h"
#include "ui_surface.h"
#include "sprite.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct UI_GameMinimap {
    UI_GlobalResource* gr;
    SpritePack* spk_minimap;
    
    /* State */
    // Current Zone Name, Coords, Dots?
};

/* Helper to load packs safely */
static SpritePack* load_pack(const char* path) {
    SpritePack* pack = (SpritePack*)malloc(sizeof(SpritePack));
    if (!pack) return NULL;
    memset(pack, 0, sizeof(SpritePack));
    if (spritepack_load(pack, path) != 0) {
        printf("Failed to load pack: %s\n", path);
        free(pack);
        return NULL;
    }
    return pack;
}

static void draw_sprite(UI_GlobalResource* gr, SpritePack* pack, int index, int x, int y) {
    if (!pack || !gr) return;
    Sprite* s = spritepack_get(pack, (uint16_t)index);
    if (!s) return;
    
    DecodedSprite decoded = {0};
    if (sprite_decode(s, &decoded, 0) == 0) {
        if (decoded_sprite_create_texture(&decoded, gr->renderer) == 0) {
            ui_surface_blit_sprite(gr->surface, x, y, &decoded);
        }
        decoded_sprite_free(&decoded);
    }
}

UI_GameMinimap* ui_game_minimap_create(UI_GlobalResource* global_resource) {
    UI_GameMinimap* mm = (UI_GameMinimap*)malloc(sizeof(UI_GameMinimap));
    if (!mm) return NULL;
    
    memset(mm, 0, sizeof(UI_GameMinimap));
    mm->gr = global_resource;
    
    mm->spk_minimap = load_pack("DarkEden/Data/Ui/spk/MinimapSlayer.spk");
    
    return mm;
}

void ui_game_minimap_destroy(UI_GameMinimap* minimap) {
    if (minimap) {
        if (minimap->spk_minimap) {
            spritepack_free(minimap->spk_minimap);
            free(minimap->spk_minimap);
        }
        free(minimap);
    }
}

void ui_game_minimap_render(UI_GameMinimap* minimap, int x, int y) {
    if (!minimap) return;

    /* Draw Minimap Frame */
    if (minimap->spk_minimap) {
        draw_sprite(minimap->gr, minimap->spk_minimap, 0, x, y);
    }
    
    /* Draw Content (Dots) placeholder */
}

void ui_game_minimap_process(UI_GameMinimap* minimap) {
    (void)minimap;
}
