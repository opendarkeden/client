/*-----------------------------------------------------------------------------

	VS_UI_Message.h

	Error/Info & etc message collection.

	2000.11.7. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_MESSAGE_H__
#define __VS_UI_MESSAGE_H__

#include "VS_UI_Dialog.h"

//-----------------------------------------------------------------------------
// Public
//-----------------------------------------------------------------------------
void	g_InitMessage();
void	g_FreeMessage();

extern C_VS_UI_DIALOG *		g_msg_not_available_menu;
extern C_VS_UI_DIALOG *		g_msg_wrong_id_password;
extern C_VS_UI_DIALOG *		g_msg_invalid_id_password;
extern C_VS_UI_DIALOG *		g_msg_wrong_ssn;
extern C_VS_UI_DIALOG *		g_msg_invalid_ssn;
extern C_VS_UI_DIALOG *		g_msg_empty_name;
extern C_VS_UI_DIALOG *		g_msg_left_bonus_point;
//extern C_VS_UI_DIALOG *		g_msg_server_disconnect;
extern C_VS_UI_DIALOG *		g_msg_server_connect_failed;
extern C_VS_UI_DIALOG *		g_msg_not_select_char;
extern C_VS_UI_DIALOG *		g_msg_char_delete_completed;
extern C_VS_UI_DIALOG *		g_msg_all_slot_not_char;
extern C_VS_UI_DIALOG *		g_msg_all_slot_created;
extern C_VS_UI_DIALOG *		g_msg_character_create_failed;
extern C_VS_UI_DIALOG *		g_msg_new_user_not_completed;
extern C_VS_UI_DIALOG *		g_msg_id_aleady_exist;
extern C_VS_UI_DIALOG *		g_msg_id_no_aleady_exist;
extern C_VS_UI_DIALOG *		g_msg_password_mismatch;
//extern C_VS_UI_DIALOG *		g_msg_not_empty_slot;
//extern C_VS_UI_DIALOG *		g_msg_character_used_id;
//extern C_VS_UI_DIALOG *		g_msg_character_empty_id;

#endif