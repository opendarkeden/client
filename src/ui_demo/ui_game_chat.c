/**
 * @file ui_game_chat.c
 * @brief Game Chat Implementation
 */

#include "ui_game_chat.h"
#include "spritepack.h"
#include "ui_surface.h"
#include "sprite.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_CHAT_LINES 10
#define MAX_LINE_LEN 256

struct UI_GameChat {
    UI_GlobalResource* gr;
    SpritePack* spk_chat;
    
    char messages[MAX_CHAT_LINES][MAX_LINE_LEN];
    int line_count;
    int scroll_offset;
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

UI_GameChat* ui_game_chat_create(UI_GlobalResource* global_resource) {
    UI_GameChat* chat = (UI_GameChat*)malloc(sizeof(UI_GameChat));
    if (!chat) return NULL;
    
    memset(chat, 0, sizeof(UI_GameChat));
    chat->gr = global_resource;
    
    chat->spk_chat = load_pack("DarkEden/Data/Ui/spk/ChattingSlayer.spk");
    
    /* Add welcome message */
    ui_game_chat_add_message(chat, "Welcome to DarkEden!");
    
    return chat;
}

void ui_game_chat_destroy(UI_GameChat* chat) {
    if (chat) {
        if (chat->spk_chat) {
            spritepack_free(chat->spk_chat);
            free(chat->spk_chat);
        }
        free(chat);
    }
}

void ui_game_chat_render(UI_GameChat* chat, int x, int y) {
    if (!chat) return;

    /* Draw Chat Background */
    if (chat->spk_chat) {
        draw_sprite(chat->gr, chat->spk_chat, 0, x, y);
    }
    
    /* Draw Messages (Placeholder Text Rendering) */
    /* UI_Demo doesn't have a text rendering system yet readily available in global resource?
       Actually legacy has 'g_Print' or similar. 
       For now, we just rely on console print or verify via structure.
       Or if ui_global_resource has font support?
       SDL_ttf is not explicitly initialized in main struct?
       We'll skip text rendering for visual demo unless we find a font.
    */
}

void ui_game_chat_process(UI_GameChat* chat) {
    (void)chat;
}

void ui_game_chat_add_message(UI_GameChat* chat, const char* message) {
    if (!chat || !message) return;
    
    /* Shift lines up */
    if (chat->line_count < MAX_CHAT_LINES) {
        strncpy(chat->messages[chat->line_count], message, MAX_LINE_LEN - 1);
        chat->line_count++;
    } else {
        /* Rotate */
        for (int i = 0; i < MAX_CHAT_LINES - 1; i++) {
            strcpy(chat->messages[i], chat->messages[i+1]);
        }
        strncpy(chat->messages[MAX_CHAT_LINES - 1], message, MAX_LINE_LEN - 1);
    }
    printf("CHAT: %s\n", message);
}
