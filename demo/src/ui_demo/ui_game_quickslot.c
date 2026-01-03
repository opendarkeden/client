/**
 * @file ui_game_quickslot.c
 * @brief Quick Item Slot Implementation
 */

#include "ui_game_quickslot.h"
#include "spritepack.h"
#include "ui_surface.h"
#include "sprite.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct UI_QuickSlot {
    UI_GlobalResource* gr;
    SpritePack* spk_quickitem;
    
    /* State */
    int active_slot; /* 0-7 */
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

UI_QuickSlot* ui_game_quickslot_create(UI_GlobalResource* global_resource) {
    UI_QuickSlot* qs = (UI_QuickSlot*)malloc(sizeof(UI_QuickSlot));
    if (!qs) return NULL;
    
    memset(qs, 0, sizeof(UI_QuickSlot));
    qs->gr = global_resource;
    
    qs->spk_quickitem = load_pack("DarkEden/Data/Ui/spk/QuickitemSlayer.spk");
    
    return qs;
}

void ui_game_quickslot_destroy(UI_QuickSlot* quickslot) {
    if (quickslot) {
        if (quickslot->spk_quickitem) {
            spritepack_free(quickslot->spk_quickitem);
            free(quickslot->spk_quickitem);
        }
        free(quickslot);
    }
}

void ui_game_quickslot_render(UI_QuickSlot* quickslot, int x, int y) {
    if (!quickslot || !quickslot->spk_quickitem) return;

    /* Render 8 slots */
    /* Assuming index 0 needs to be rendered 8 times or there are separate frames? */
    /* Checking VS_UI_GameSlayer.h enum:
       SLOT, SLOT_ALPHA, STATCH_EDGE, START_EDGE...
       Usually a repeating background slot.
    */
    
    // Draw edges?
    // Let's assume index 0 is the slot background.
    int slot_w = 30; // From header info: QUICKSLOT_W = 30
    
    for (int i = 0; i < 8; ++i) {
        draw_sprite(quickslot->gr, quickslot->spk_quickitem, 0, x + (i * slot_w), y);
        
        // Draw number? Or assumes it's implied by position.
        // If we had a font, we'd draw '1'..'8'.
    }
    
    // Draw edges if needed (START_EDGE, END_EDGE indices 3, 5 based on enum order assumption?)
    // Without exact indices, I'll stick to slots for now.
}

void ui_game_quickslot_process(UI_QuickSlot* quickslot) {
    // Check input 1-8 keys?
    // Input handling to be wired up later via Game Update loop
    (void)quickslot;
}
