// VS_UI_Message.cpp

#include "client_PCH.h"

#pragma warning(disable:4786)

#define DEBUG_OUTPUT
#include "VS_UI_Message.h"
#include "MGameStringTable.H"
//#include "ex\debuginfo.h"
//-----------------------------------------------------------------------------
// Global
//-----------------------------------------------------------------------------
C_VS_UI_DIALOG *		g_msg_not_available_menu;
C_VS_UI_DIALOG *		g_msg_wrong_id_password;
C_VS_UI_DIALOG *		g_msg_invalid_id_password;
C_VS_UI_DIALOG *		g_msg_wrong_ssn;
C_VS_UI_DIALOG *		g_msg_invalid_ssn;
C_VS_UI_DIALOG *		g_msg_empty_name;
C_VS_UI_DIALOG *		g_msg_left_bonus_point;
//C_VS_UI_DIALOG *		g_msg_server_disconnect;
C_VS_UI_DIALOG *		g_msg_server_connect_failed;
C_VS_UI_DIALOG *		g_msg_not_select_char;
C_VS_UI_DIALOG *		g_msg_char_delete_completed;
C_VS_UI_DIALOG *		g_msg_all_slot_not_char;
C_VS_UI_DIALOG *		g_msg_all_slot_created;
C_VS_UI_DIALOG *		g_msg_character_create_failed;
C_VS_UI_DIALOG *		g_msg_new_user_not_completed;
C_VS_UI_DIALOG *		g_msg_id_aleady_exist;
C_VS_UI_DIALOG *		g_msg_id_no_aleady_exist;
C_VS_UI_DIALOG *		g_msg_password_mismatch;
//C_VS_UI_DIALOG *		g_msg_not_empty_slot;
//C_VS_UI_DIALOG *		g_msg_character_used_id;
//C_VS_UI_DIALOG *		g_msg_character_empty_id;

//-----------------------------------------------------------------------------
// Exec functions
//-----------------------------------------------------------------------------
void g_ExecF(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{

}

//-----------------------------------------------------------------------------
// g_InitMessage
//
// 
//-----------------------------------------------------------------------------
void g_InitMessage()
{
	g_msg_not_available_menu = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_not_available_menu[] ={(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_SUPPORT_MENU].GetString()};
	g_msg_not_available_menu->SetMessage(pp_dmsg_not_available_menu, 1, SMO_NOFIT);

	g_msg_wrong_id_password = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_wrong_id_password[] ={ (*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_INPUT_ID_OR_PASSWORD].GetString()};
	g_msg_wrong_id_password->SetMessage(pp_dmsg_wrong_id_password, 1, SMO_NOFIT);

	g_msg_invalid_id_password = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_invalid_id_password[] ={ (*g_pGameStringTable)[STRING_ERROR_INVALID_ID_PASSWORD].GetString()};
	g_msg_invalid_id_password->SetMessage(pp_dmsg_invalid_id_password, 1, SMO_NOFIT);

	g_msg_wrong_ssn = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_wrong_ssn[] ={(*g_pGameStringTable)[UI_STRING_MESSAGE_WRONG_SSN].GetString()};
	g_msg_wrong_ssn->SetMessage(pp_dmsg_wrong_ssn, 1, SMO_NOFIT);

	g_msg_invalid_ssn = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_invalid_ssn[] ={(*g_pGameStringTable)[STRING_ERROR_INVALID_SSN].GetString()};
	g_msg_invalid_ssn->SetMessage(pp_dmsg_invalid_ssn, 1, SMO_NOFIT);

	g_msg_empty_name = new C_VS_UI_DIALOG(-1, -1, 1, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_empty_name[] ={(*g_pGameStringTable)[UI_STRING_MESSAGE_INPUT_NAME].GetString()};
	g_msg_empty_name->SetMessage(pp_dmsg_empty_name, 1, SMO_NOFIT);

	g_msg_left_bonus_point = new C_VS_UI_DIALOG(-1, -1, 1, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_left_bonus_point[] ={(*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_BONUS_POINT].GetString()};
	g_msg_left_bonus_point->SetMessage(pp_dmsg_left_bonus_point, 1, SMO_NOFIT);
	
//	g_msg_server_disconnect = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
//	static char * pp_dmsg_server_disconnect[] ={ {"서버와의 연결이 끊어졌습니다."}};
//	g_msg_server_disconnect->SetMessage(pp_dmsg_server_disconnect, 1, SMO_NOFIT);

	g_msg_server_connect_failed = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_server_connect_failed[] ={ (*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_CONNECT_SERVER].GetString()};
	g_msg_server_connect_failed->SetMessage(pp_dmsg_server_connect_failed, 1, SMO_NOFIT);

	g_msg_not_select_char = new C_VS_UI_DIALOG(-1, -1, 1, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_not_select_char[] ={(*g_pGameStringTable)[UI_STRING_MESSAGE_SELECT_CHARACTER].GetString()};
	g_msg_not_select_char->SetMessage(pp_dmsg_not_select_char, 1, SMO_NOFIT);

	g_msg_char_delete_completed = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_char_delete_completed[] ={ (*g_pGameStringTable)[UI_STRING_MESSAGE_DELETE_CHARACTER].GetString()};
	g_msg_char_delete_completed->SetMessage(pp_dmsg_char_delete_completed, 1, SMO_NOFIT);

	g_msg_all_slot_not_char = new C_VS_UI_DIALOG(-1, -1, 4, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_all_slot_not_char[] ={(*g_pGameStringTable)[UI_STRING_MESSAGE_ALL_SLOT_EMPTY].GetString()};
	g_msg_all_slot_not_char->SetMessage(pp_dmsg_all_slot_not_char, 1, SMO_NOFIT);

	g_msg_all_slot_created = new C_VS_UI_DIALOG(-1, -1, 3, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_all_slot_created[] ={(*g_pGameStringTable)[UI_STRING_MESSAGE_NO_MORE_CREATE_CHARACTER].GetString()};
	g_msg_all_slot_created->SetMessage(pp_dmsg_all_slot_created, 1, SMO_NOFIT);

	g_msg_character_create_failed = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_character_create_failed[] ={(*g_pGameStringTable)[UI_STRING_MESSAGE_FAIL_CREATE_CHARACTER].GetString()};
	g_msg_character_create_failed->SetMessage(pp_dmsg_character_create_failed, 1, SMO_NOFIT);

//	g_msg_character_used_id = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
//	static char * pp_dmsg_character_used_id[] = {"이미 사용하고 있습니다."};
//	g_msg_character_used_id->SetMessage(pp_dmsg_character_used_id, 1, SMO_NOFIT);

//	g_msg_character_empty_id = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
//	static char * pp_dmsg_character_empty_id[] = {"사용하실 수 있습니다."};
//	g_msg_character_empty_id->SetMessage(pp_dmsg_character_empty_id, 1, SMO_NOFIT);

	g_msg_new_user_not_completed = new C_VS_UI_DIALOG(-1, -1, 3, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_new_user_not_completed[] ={ (*g_pGameStringTable)[UI_STRING_MESSAGE_NO_INPUT_NEED_INFO].GetString()};
	g_msg_new_user_not_completed->SetMessage(pp_dmsg_new_user_not_completed, 1, SMO_NOFIT);
		
	g_msg_id_aleady_exist = new C_VS_UI_DIALOG(-1, -1, 1, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_id_aleady_exist[] ={(*g_pGameStringTable)[UI_STRING_MESSAGE_ALREADY_USE_ID].GetString()};
	g_msg_id_aleady_exist->SetMessage(pp_dmsg_id_aleady_exist, 1, SMO_NOFIT);

	g_msg_id_no_aleady_exist = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_id_no_aleady_exist[] ={(*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_USE_ID].GetString()};
	g_msg_id_no_aleady_exist->SetMessage(pp_dmsg_id_no_aleady_exist, 1, SMO_NOFIT);

	g_msg_password_mismatch = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
	static char * pp_dmsg_password_mismatch[] ={(*g_pGameStringTable)[UI_STRING_MESSAGE_RE_INPUT_PASSWORD].GetString()};
	g_msg_password_mismatch->SetMessage(pp_dmsg_password_mismatch, 1, SMO_NOFIT);

//	g_msg_not_empty_slot = new C_VS_UI_DIALOG(-1, -1, 2, 0, g_ExecF, DIALOG_OK);
//	static char * pp_dmsg_not_empty_slot[] = {"캐릭터가 이미 생성되어 있습니다."};
//	g_msg_not_empty_slot->SetMessage(pp_dmsg_not_empty_slot, 1, SMO_NOFIT);
}

//-----------------------------------------------------------------------------
// g_FreeMessage
//
// 
//-----------------------------------------------------------------------------
void g_FreeMessage()
{
	DeleteNew(g_msg_not_available_menu);
	DeleteNew(g_msg_wrong_id_password);
	DeleteNew(g_msg_invalid_id_password);
	DeleteNew(g_msg_wrong_ssn);
	DeleteNew(g_msg_invalid_ssn);
	DeleteNew(g_msg_empty_name);
	DeleteNew(g_msg_left_bonus_point);
//	DeleteNew(g_msg_server_disconnect);
	DeleteNew(g_msg_server_connect_failed);
	DeleteNew(g_msg_not_select_char);
	DeleteNew(g_msg_char_delete_completed);
	DeleteNew(g_msg_all_slot_not_char);
	DeleteNew(g_msg_all_slot_created);
	DeleteNew(g_msg_character_create_failed);
	DeleteNew(g_msg_new_user_not_completed);
	DeleteNew(g_msg_id_aleady_exist);
	DeleteNew(g_msg_id_no_aleady_exist);
	DeleteNew(g_msg_password_mismatch);
//	DeleteNew(g_msg_character_used_id);
//	DeleteNew(g_msg_character_empty_id);
	
//	DeleteNew(g_msg_not_empty_slot);
}
