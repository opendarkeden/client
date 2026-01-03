/**
 * @file ui_game_chat.h
 * @brief Game Chat Interface
 */

#ifndef UI_GAME_CHAT_H
#define UI_GAME_CHAT_H

#include "ui_window.h"
#include "ui_global_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UI_GameChat UI_GameChat;

UI_GameChat* ui_game_chat_create(UI_GlobalResource* global_resource);
void ui_game_chat_destroy(UI_GameChat* chat);

void ui_game_chat_render(UI_GameChat* chat, int x, int y);
void ui_game_chat_process(UI_GameChat* chat);

void ui_game_chat_add_message(UI_GameChat* chat, const char* message);

#ifdef __cplusplus
}
#endif

#endif /* UI_GAME_CHAT_H */
