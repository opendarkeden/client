
/*----------------------------------------------------------
	
		UI STRING Ãß°¡ - 2002.08-31				by sonee
	
	°ÔÀÓ³»¿¡ µé¾î°¡´Â Description µéÀÌ´Ù.
	String ÀÌ µé¾î°£´Ù¸é ÀÌ°÷¿¡¼­ °Ë»öÀ» ÇØ¼­ ÂüÁ¶ÇØ°¡°Å³ª,
	°Ë»öÇØ¼­ ¾øÀ¸¸é ÀÌ°÷¿¡ Ãß°¡ÈÄ ÂüÁ¶ÇØ°¡µµ·Ï ÇÏÀÚ.
	´Ù¸¥ ¾ð¾î ¹öÁ¯À¸·Î ¼öÁ¤ÀÛ¾÷ÀÌ Á»´õ ¼ö¿ùÇÏ´Ù.

	(°Ë»öÀº CPP ÆÄÀÏ¿¡¼­ ÇÏ´Â°ÍÀÌ ÁÁÀ»µí)

  ----------------------------------------------------------*/

//----------------------------------------------------------------------
// MGameStringTable.h
//----------------------------------------------------------------------

#ifndef __MGAMESTRINGTABLE_H__
#define	__MGAMESTRINGTABLE_H__

#include "MStringArray.h"

extern MStringArray*		g_pGameStringTable;
//2004, 6, 18, sobeit add start - about nick name string table
extern MStringArray*		g_pNickNameStringTable;
//2004, 6, 18, sobeit add end - about nick name string table
#ifndef __GAME_CLIENT__
	extern void				InitGameStringTable();
#endif

//----------------------------------------------------------------------
//----------------------------------------------------------------------
enum GAME_STRINGID
{
	STRING_NETWORK_CONDITION_BAD,
	STRING_RESURRECTION_AFTER_SECONDS,
	STRING_DRAW_ZONE_NAME,
	STRING_DRAW_GAME_DATE,
	STRING_DRAW_GAME_TIME,
	STRING_DRAW_ITEM_NAME_MONEY,

	STRING_USER_REGISTER_DENY,
	STRING_USER_REGISTER_HOMEPAGE,
	STRING_USER_REGISTER_EMPTY_FIELD,
	STRING_USER_REGISTER_INVALID_ID,
	STRING_USER_REGISTER_ID_LENGTH,
	STRING_USER_REGISTER_ID_SPECIAL,
	STRING_USER_REGISTER_PASSWORD_LENGTH,
	STRING_USER_REGISTER_PASSWORD_SPECIAL,
	STRING_USER_REGISTER_PASSWORD_NUMBER,
	STRING_USER_REGISTER_NAME_LENGTH,
	STRING_USER_REGISTER_INVALID_SSN,
	STRING_USER_REGISTER_SSN_FORMAT,

	STRING_MESSAGE_CANNOT_BUY_NO_SPACE,
	STRING_MESSAGE_CANNOT_BUY_NO_MONEY,
	STRING_MESSAGE_CANNOT_BUY_NO_ITEM,
	STRING_MESSAGE_CANNOT_SELL,
	STRING_MESSAGE_CANNOT_REPAIR,
	STRING_MESSAGE_CANNOT_SILVERING,
	STRING_MESSAGE_CANNOT_BUY_MORE,
	STRING_MESSAGE_CANNOT_STORAGE,
	STRING_MESSAGE_STORAGE_BUY,
	STRING_MESSAGE_NO_STORAGE,
	STRING_MESSAGE_TRADE_REJECTED,
	STRING_MESSAGE_TRADE_NOBODY,		
	STRING_MESSAGE_TRADE_CANNOT_ON_MOTORCYCLE,
	STRING_MESSAGE_TRADE_SAFETY_ZONE_ONLY,
	STRING_MESSAGE_TRADE_BUSY,
	STRING_MESSAGE_CANNOT_TRADE,
	STRING_MESSAGE_CANNOT_TRADE_ALREADY_TRADING,
	STRING_MESSAGE_CANNOT_TRADE_NO_SPACE,
	STRING_MESSAGE_SKILL_DIFFER_DOMAIN,
	STRING_MESSAGE_SKILL_EXCEED_LEVEL,
	STRING_MESSAGE_SKILL_CANNOT_LEARN,
	STRING_MESSAGE_SKILL_NOT_SUPPORT,
	STRING_MESSAGE_FIND_MOTOR_NO_WHERE,
	STRING_MESSAGE_FIND_MOTOR_NO_KEY,
	STRING_MESSAGE_FIND_MOTOR_OK,
	STRING_MESSAGE_CANNOT_USE_BONUS_POINT,
	STRING_MESSAGE_WHAT_SKILL_TO_LEARN,
	STRING_MESSAGE_NO_SKILL_TO_LEARN,
	STRING_MESSAGE_NEW_SKILL_AVAILABLE,
	STRING_MESSAGE_NEW_DOMAIN_LEVEL_1,
	STRING_MESSAGE_NEW_DOMAIN_LEVEL_2,
	STRING_MESSAGE_CANNOT_MOVE_SAFETY_ZONE_RELIC,			// ¼º¹°À» µé°í ÀÖÀ¸¸é, ÀÚ±âÆíÀÇ ¾ÈÀüÁö´ë¿¡´Â µé¾î°¥ ¼ö ¾ø½À´Ï´Ù!

	STRING_MESSAGE_TEAM_REGIST_FAIL_ALREADY_JOIN,			// ¾îµð ÇÑ¹ø º¼±î? ÀÚ³×´Â ÀÌ¹Ì <team_name> ÆÀ ¼Ò¼ÓÀÌ¶ó°í ³ª¿Í ÀÖ±º
	STRING_MESSAGE_TEAM_REGIST_FAIL_QUIT_TIMEOUT,			// ÀÚ³×´Â ´Ù¸¥ ÆÀÀ» Å»ÅðÇÑÁö ¾ó¸¶ µÇÁö ¾Ê¾Ò±º. Á¶±Ý ´õ ½ÅÁßÇÏ°Ô »ý°¢ÇÏ°í Çàµ¿ÇÏ°Ô
	STRING_MESSAGE_TEAM_REGIST_FAIL_CANCEL_TIMEOUT,		// ÀÚ³×´Â ÆÀÀ» Ãë¼Ò ´çÇÑ Áö ¾ó¸¶ µÇÁö ¾Ê¾Ò±º. µî·Ï Á¶°Ç¿¡ ºÎÇÕÇÏµµ·Ï ÀÚ½ÅÀ» Á» ´õ ±â¸£°í ¿À°Ô. Á» ´õ ½ÅÁßÇÏ°Ô ±âÈ¸¸¦ ¿³º¸µµ·Ï ÇÏ°Ô
	STRING_MESSAGE_TEAM_REGIST_FAIL_LEVEL,				// ÈÇ¸¢ÇÏÁö¸¸ ¸®´õ·Î½áÀÇ ÀÚÁúÀº Á» ºÎÁ·ÇÑ °Í °°±º. ½Ç·ÂÀ» ´Û°í ¿À°Ô³ª
	STRING_MESSAGE_TEAM_REGIST_FAIL_MONEY,				// ÆÀÀ» ¸¸µé·Á¸é ¸¹Àº µ·ÀÌ ÇÊ¿äÇÏ´Ù³×. ÀÚ³×´Â µ·ÀÌ ¾ø¾î º¸ÀÌ´Â±º...
	STRING_MESSAGE_TEAM_REGIST_FAIL_FAME,					// <player_name>ÀÌ¶ó.. ³­ ¾ÆÁ÷ ±×·± ÀÌ¸§Àº µé¾îº¸Áöµµ ¸øÇß´Ù³×. ±×°Ç ÀÚ³×°¡ Ç²³»±â¶ó´Â °ÍÀ» ÀÇ¹ÌÇÏÁö. ½Ç·ÂÀ» ´Û°í ´Ù½Ã Ã£¾Æ¿À°Ô³ª
	STRING_MESSAGE_TEAM_REGIST_FAIL_NAME,					// ÆÀ ÀÌ¸§ÀÌ ÀÌ¹Ì ¾²ÀÌ°í ÀÖ±º, ´Ù¸¥ ÀÌ¸§À» »ý°¢ÇØ º¸°Ô
	STRING_MESSAGE_TEAM_REGIST_FAIL_DENY,					// <team name>ÆÀ¿¡¼­ ÀÚ³×¸¦ °ÅºÎÇÏÁö ¾Ê¾Ò´Â°¡. ´Ù¸¥ ÆÀÀ» Ã£¾Æº¸°Ô

	STRING_MESSAGE_TEAM_STARTING_FAIL_ALREADY_JOIN,		// ´ç½ÅÀº ÀÌ¹Ì ´Ù¸¥ ÆÀ¿¡ °¡ÀÔµÇ¾î ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_TEAM_STARTING_FAIL_QUIT_TIMEOUT,		// ÀÚ³×´Â ´Ù¸¥ ÆÀÀ» Å»ÅðÇÑÁö ¾ó¸¶ µÇÁö ¾Ê¾Ò±º. Á¶±Ý ´õ ½ÅÁßÇÏ°Ô »ý°¢ÇÏ°í Çàµ¿ÇÏ°Ô
	STRING_MESSAGE_TEAM_STARTING_FAIL_CANCEL_TIMEOUT,		// ÀÚ³×´Â ÆÀÀ» Ãë¼Ò ´çÇÑ Áö ¾ó¸¶ µÇÁö ¾Ê¾Ò±º. Á¶°Ç¿¡ ºÎÇÕÇÏµµ·Ï ÀÚ½ÅÀ» Á» ´õ ±â¸£°í ¿À°Ô. Á» ´õ ½ÅÁßÇÏ°Ô ±âÈ¸¸¦ ¿³º¸µµ·Ï ÇÏ°Ô
	STRING_MESSAGE_TEAM_STARTING_FAIL_LEVEL,				// ÀÚ³×´Â ¾ÆÁ÷ ºÎÁ·ÇÑ Á¡ÀÌ ¸¹¾Æ º¸ÀÌ´Â±¸¸¸. Á¶±Ý ´õ ½Ç·ÂÀ» ´Û°í Ã£°Ô³ª.
	STRING_MESSAGE_TEAM_STARTING_FAIL_MONEY,				// <player_name>, ÆÀ µî·ÏÀ» À§ÇØ¼­´Â µ·ÀÌ ´õ ÇÊ¿äÇÏ´Ù³×
	STRING_MESSAGE_TEAM_STARTING_FAIL_FAME,				// <player_name>ÀÌ¶ó.. ³­ ¾ÆÁ÷ ±×·± ÀÌ¸§Àº µé¾îº¸Áöµµ ¸øÇß´Ù³×. ±×°Ç ÀÚ³×°¡ Ç²³»±â¶ó´Â °ÍÀ» ÀÇ¹ÌÇÏÁö. ½Ç·ÂÀ» ´Û°í ´Ù½Ã Ã£¾Æ¿À°Ô³ª
	STRING_MESSAGE_TEAM_STARTING_FAIL_DENY,					// <team name>ÆÀ¿¡¼­ ÀÚ³×¸¦ °ÅºÎÇÏÁö ¾Ê¾Ò´Â°¡. ´Ù¸¥ ÆÀÀ» Ã£¾Æº¸°Ô

	STRING_MESSAGE_CLAN_REGIST_FAIL_ALREADY_JOIN,			// ¾îµð ÇÑ¹ø º¼±î? ÀÚ³×´Â ÀÌ¹Ì <clan_name> Å¬·£°ú ¼­¾àÀ» ¸ÎÀº »óÅÂ¶ó°í ³ª¿Í ¾Ò±º
	STRING_MESSAGE_CLAN_REGIST_FAIL_QUIT_TIMEOUT,			// ÀÚ³×´Â ´Ù¸¥ Å¬·£À» Å»ÅðÇÑÁö ¾ó¸¶µÇÁö ¾Ê¾Ò±º. ³ª¹« ÀÌ¸®Àú¸® ¿Å°Ü ´Ù´Ï´Â °ÍÀº ÁÁÁö ¾ÊÁö. ½ÅÁßÀ» ±âÇÏ°Ô
	STRING_MESSAGE_CLAN_REGIST_FAIL_CANCEL_TIMEOUT,		// ÀÚ³×´Â Å¬·£À» Ãë¼Ò ´çÇÑ Áö ¾ó¸¶ µÇÁö ¾Ê¾Ò±º. Á» ´õ ½ÅÁßÇÏ°Ô ±âÈ¸¸¦ ¿³º¸µµ·Ï ÇÏ°Ô
	STRING_MESSAGE_CLAN_REGIST_FAIL_LEVEL,				// ÈÇ¸¢ÇÏÁö¸¸ ¿ìµÎ¸Ó¸®·Î½áÀÇ ÀÚÁúÀº Á» ºÎÁ·ÇÑ °Í °°±º. ½Ç·ÂÀ» ´Û°í ¿À°Ô³ª.
	STRING_MESSAGE_CLAN_REGIST_FAIL_MONEY,				// Å¬·£À» µî·ÏÇÏ·Á¸é ¸¹Àº µ·ÀÌ ÇÊ¿äÇÏ´Ù³×. ÀÚ³×´Â µ·ÀÌ ¾ø¾îº¸ÀÌ´Â±º...
	STRING_MESSAGE_CLAN_REGIST_FAIL_FAME,					// <player_name>ÀÌ¶ó.. ¾ÆÁ÷ ¾î¸° ¹ìÆÄÀÌ¾îÀÎ°¡º¸±º. ´õ ¸¹Àº ÇÇ¸¦ ¸¶½Ã°í ´Ù½Ã Ã£¾Æ¿À°Ô³ª.
	STRING_MESSAGE_CLAN_REGIST_FAIL_NAME,					// Å¬·£ ÀÌ¸§ÀÌ ÀÌ¹Ì ¾²ÀÌ°í ÀÖ±º, ´Ù¸¥ ÀÌ¸§À» »ý°¢ÇØ º¸°Ô
	STRING_MESSAGE_CLAN_REGIST_FAIL_DENY,					// <team name>Å¬·£¿¡¼­ ÀÚ³×¸¦ °ÅºÎÇÏÁö ¾Ê¾Ò´Â°¡. ´Ù¸¥ Å¬·£À» Ã£¾Æº¸°Ô

	STRING_MESSAGE_CLAN_STARTING_FAIL_ALREADY_JOIN,		// ´ç½ÅÀº ÀÌ¹Ì ´Ù¸¥ Å¬·£¿¡ °¡ÀÔµÇ¾î ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_CLAN_STARTING_FAIL_QUIT_TIMEOUT,		// ÀÚ³×´Â ´Ù¸¥ Å¬·£À» Å»ÅðÇÑÁö ¾ó¸¶µÇÁö ¾Ê¾Ò±º. ³ª¹« ÀÌ¸®Àú¸® ¿Å°Ü ´Ù´Ï´Â °ÍÀº ÁÁÁö ¾ÊÁö. ½ÅÁßÀ» ±âÇÏ°Ô
	STRING_MESSAGE_CLAN_STARTING_FAIL_CANCEL_TIMEOUT,		// ÀÚ³×´Â Å¬·£À» Ãë¼Ò ´çÇÑ Áö ¾ó¸¶ µÇÁö ¾Ê¾Ò±º. Á» ´õ ½ÅÁßÇÏ°Ô ±âÈ¸¸¦ ¿³º¸µµ·Ï ÇÏ°Ô
	STRING_MESSAGE_CLAN_STARTING_FAIL_LEVEL,				// ÈÇ¸¢ÇÑ Á¶·ÂÀÚ°¡ µÇ±â¿¡´Â ÀÚÁúÀÌ Á» ºÎÁ·ÇÑ °Í °°±º. °¡¼­ Á»´õ ¼ö·ÃÀ» ÇÏ°í ¿À°Ô³ª
	STRING_MESSAGE_CLAN_STARTING_FAIL_MONEY,				// ±â¼úÀÌ ¿ùµîÇØµµ Å¬·£À» À¯ÁöÇÒ ¼ö ÀÖ´Â µ·ÀÌ ºÎÁ·ÇÏ¸é ±× Å¬·£Àº ¹«³ÊÁö±â ½±»óÀÌÁö
	STRING_MESSAGE_CLAN_STARTING_FAIL_FAME,				// <player_name>ÀÌ¶ó.. ³­ ¾ÆÁ÷ ±×·± ÀÌ¸§Àº µé¾îº¸Áöµµ ¸øÇß´Ù³×. ±×°Ç ÀÚ³×°¡ Ç²³»±â¶ó´Â °ÍÀ» ÀÇ¹ÌÇÏÁö. ½Ç·ÂÀ» ´Û°í ´Ù½Ã Ã£¾Æ¿À°Ô³ª
	STRING_MESSAGE_CLAN_STARTING_FAIL_DENY,					// <team name>Å¬·£¿¡¼­ ÀÚ³×¸¦ °ÅºÎÇÏÁö ¾Ê¾Ò´Â°¡. ´Ù¸¥ Å¬·£À» Ã£¾Æº¸°Ô

	STRING_MESSAGE_GUILD_REGIST_FAIL_INTRO,					// ¼Ò°³¸¦ ÀÔ·ÂÇÏ¼¼¿ä

	STRING_STATUS_HP_MAX_1,
	STRING_STATUS_HP_MAX_2,
	STRING_STATUS_MP_MAX_1,
	STRING_STATUS_MP_MAX_2,
	STRING_STATUS_STR_1,
	STRING_STATUS_STR_2,
	STRING_STATUS_DEX_1,
	STRING_STATUS_DEX_2,
	STRING_STATUS_INT_1,
	STRING_STATUS_INT_2,
	STRING_STATUS_LEVEL,
	STRING_LEARN_SKILL,
	STRING_CHANGE_ALIGNMENT,
	STRING_CHANGE_ALIGNMENT2,
	STRING_CHANGE_TO_BAD_ALIGNMENT,
	STRING_CHANGE_TO_GOOD_ALIGNMENT,

	STRING_ERROR_INVALID_ID_PASSWORD,
	STRING_ERROR_ALREADY_CONNECTED,
	STRING_ERROR_ALREADY_REGISTER_ID,
	STRING_ERROR_ALREADY_REGISTER_SSN,
	STRING_ERROR_EMPTY_ID,
	STRING_ERROR_SMALL_ID_LENGTH,
	STRING_ERROR_EMPTY_PASSWORD,
	STRING_ERROR_SMALL_PASSWORD_LENGTH,
	STRING_ERROR_EMPTY_NAME,
	STRING_ERROR_EMPTY_SSN,
	STRING_ERROR_INVALID_SSN,
	STRING_ERROR_NOT_FOUND_PLAYER,
	STRING_ERROR_NOT_FOUND_ID,
	STRING_ERROR_LOGIN_DENY,
	STRING_ERROR_ETC_ERROR,
	STRING_ERROR_NOT_ALLOW_ACCOUNT,
	STRING_ERROR_NOT_PAY_ACCOUNT,

	STRING_MESSAGE_ITEM_BROKEN,	
	STRING_MESSAGE_WHISPER_FAILED,
	STRING_MESSAGE_WHISPER_SELF,
	STRING_MESSAGE_CHAT_IGNORE,
	STRING_MESSAGE_CHAT_ACCEPT,
	STRING_MESSAGE_CHAT_IGNORE_ALL,
	STRING_MESSAGE_CHAT_ACCEPT_ALL,
	STRING_MESSAGE_CHAT_BE_GOOD,
	STRING_MESSAGE_CHAT_ACCEPT_CURSE,
	STRING_MESSAGE_CHAT_FILTER_CURSE,
	STRING_MESSAGE_CANNOT_LOGOUT_DIED,
	STRING_MESSAGE_CANNOT_GLOBAL_SAY,

	STRING_MESSAGE_WAIT,

	STRING_MESSAGE_CONNECTING_SERVER,

	STRING_MESSAGE_DONATION_OK,
	STRING_MESSAGE_DONATION_FAIL,

	STRING_MESSAGE_PARTY_REJECTED,
	STRING_MESSAGE_PARTY_NOBODY,		
	STRING_MESSAGE_PARTY_SAFETY_ZONE_ONLY,
	STRING_MESSAGE_PARTY_BUSY,
	STRING_MESSAGE_RACE_DIFFER,
	STRING_MESSAGE_NOT_NORMAL_FORM,
	STRING_MESSAGE_NO_AUTHORITY,
	STRING_MESSAGE_ERROR_PARTY,
	STRING_MESSAGE_KICK_PARTY_MEMBER_OK,
	STRING_MESSAGE_KICKED_FROM_PARTY,
	STRING_MESSAGE_KICK_PARTY_MEMBER,
	STRING_MESSAGE_REMOVE_PARTY,
	STRING_MESSAGE_REMOVE_PARTY_HIMSELF,
	STRING_MESSAGE_REMOVE_PARTY_MYSELF,
	STRING_MESSAGE_IN_ANOTHER_PARTY,
	STRING_MESSAGE_PARTY_FULL,
	STRING_MESSAGE_SOMEONE_JOINED_PARTY,

	STRING_MESSAGE_LOGOUT_AFTER_SECOND,

	STRING_MESSAGE_CANNOT_BUY_NO_STAR,
	STRING_MESSAGE_CANNOT_BUY,

	STRING_MESSAGE_CAN_REGISTER_NAME,

	STRING_MESSAGE_HELP_KEY,
	STRING_MESSAGE_HOW_TO_GET_BASIC_WEAPON,
	STRING_MESSAGE_WAIT_FOR_CHARACTER_SELECT_MODE,

	STRING_MESSAGE_ITEM_TO_ITEM_IMPOSIBLE,

	
	UI_STRING_MESSAGE_ONLY_PICK_UP_ITEM_ONE,       // "ÇÑ °³¾¿ µé°í ÇÏ¼Å¾ß ÇÕ´Ï´Ù.";
	UI_STRING_MESSAGE_CAN_ENCHANT,					// Enchant ÇÒ ¼ö ÀÖ½À´Ï´Ù.
	UI_STRING_MESSAGE_SELL_CONFIRM,					// ÀÌ ¾ÆÀÌÅÛÀ» ÆÄ½Ã°Ú½À´Ï±î?
	UI_STRING_MESSAGE_ALL_SELL_CONFIRM,				// ÆÄ½Ã°Ú½À´Ï±î?
	UI_STRING_MESSAGE_ALL_PRICE,					// ¸ðµÎ $%s ÀÔ´Ï´Ù.
	UI_STRING_MESSAGE_REPAIR_CONFIRM,				// °íÄ¡½Ã°Ú½À´Ï±î?
	UI_STRING_MESSAGE_CLICK_TRADE_ITEM,				// °Å·¡ÇÒ ¾ÆÀÌÅÛÀ» Å¬¸¯ÇÏ¼¼¿ä
	UI_STRING_MESSAGE_CLICK_REPAIR_ITEM,			// °íÄ¥ ¾ÆÀÌÅÛÀ» Å¬¸¯ÇÏ¼¼¿ä
	UI_STRING_MESSAGE_CLICK_OK_BUTTON_TO_END,		// ³¡³»½Ã·Á¸é OK ¹öÆ°À» ´©¸£¼¼¿ä.
	UI_STRING_MESSAGE_THIS_ITEM_REPAIR_CONFIRM,		// ÀÌ ¾ÆÀÌÅÛÀ» °íÄ¥±î¿ä?
	UI_STRING_MESSAGE_THIS_ITEM_CHARGE_CONFIRM,		// ÀÌ ¾ÆÀÌÅÛÀ» ÃæÀüÇÒ±î¿ä?
	UI_STRING_MESSAGE_THIS_ITEM_SILVERING_CONFIRM,	// ÀÌ ¾ÆÀÌÅÛÀ» Àºµµ±Ý ÇÒ±î¿ä?	
	UI_STRING_MESSAGE_CLICK_SILVERING_ITEM,			// Àºµµ±ÝÇÒ ¾ÆÀÌÅÛÀ» Å¬¸¯ÇÏ¼¼¿ä.
	UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW,			// Ã¢À» ¹ÝÅõ¸íÀ¸·Î Ãâ·ÂÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW,			// Ã¢À» ¹ÝÅõ¸íÀ¸·Î Ãâ·ÂÇÏÁö ¾Ê½À´Ï´Ù.
	UI_STRING_MESSAGE_CAN_EQUIP,					// ÀåÂø ÇÒ ¼ö ÀÖ½À´Ï´Ù.
	UI_STRING_MESSAGE_CANNOT_EQUIP,					// ÀåÂø ÇÒ ¼ö ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_THROW_MONEY,					// "µ·À» ¹ö¸³´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_ITEM_DESCRIPTION,		// "¾ÆÀÌÅÛ ¼³¸íÀ» º¾´Ï´Ù",
	UI_STRING_MESSAGE_DEPOSIT_MONEY,				// "µ·À» º¸°üÇÕ´Ï´Ù",
	UI_STRING_MESSAGE_INVITE_PARTY,					// ÆÄÆ¼ °¡ÀÔÃÊÃ» ¸Þ¼¼Áö°¡ µé¾î¿Í ÀÖ½À´Ï´Ù.
	UI_STRING_MESSAGE_REQUEST_PARTY,				// ÆÄÆ¼ °¡ÀÔ½ÅÃ» ¸Þ¼¼Áö°¡ µé¾î¿Í ÀÖ½À´Ï´Ù.
	UI_STRING_MESSAGE_RESURRECTION,					// "ÁöÁ¤ À§Ä¡¿¡¼­ ºÎÈ°ÇÑ´Ù"
	UI_STRING_MESSAGE_EVACUATION,					// "ÁöÁ¤ À§Ä¡·Î ÈÄ¼ÛµÈ´Ù"
	UI_STRING_MESSAGE_CANNOT_UP_LEVEL,				// ´õÀÌ»ó ·¹º§¾÷ÀÌ µÇÁö ¾Ê½À´Ï´Ù
	UI_STRING_MESSAGE_CANNOT_UP_STAT,				// ´õÀÌ»ó ´É·ÂÄ¡°¡ ¿Ã¶ó°¡Áö ¾Ê½À´Ï´Ù.
	UI_STRING_MESSAGE_TEAM_INFO_JOIN,				// °¡ÀÔÇÕ´Ï´Ù.

	// UI TIP
	UI_STRING_MESSAGE_TIP_SELL_ALL_VAMPIRE_HEAD,	// TIP : ¹ìÆÄÀÌ¾îÀÇ ¸Ó¸®µéÀ» ÇÑ²¨¹ø¿¡ ÆÈ·Á¸é ¿À¸¥ÂÊ Å¬¸¯À» ÇÏ¼¼¿ä.
	UI_STRING_MESSAGE_TIP_REPAIR_ALL_ITEM,			// TIP : ÀåºñµéÀ» ÇÑ²¨¹ø¿¡ °íÄ¡·Á¸é ¿À¸¥ÂÊ Å¬¸¯À» ÇÏ¼¼¿ä.


	UI_STRING_MESSAGE_AUTO_HIDE_ON,					// ÀÚµ¿¼û±è ÄÔ
	UI_STRING_MESSAGE_AUTO_HIDE_OFF,				// ÀÚµ¿¼û±è ²û
	UI_STRING_MESSAGE_GAME_MENU,					// °ÔÀÓ¸Þ´º
	UI_STRING_MESSAGE_MENU,							// ¸Þ´º(Menu)
	UI_STRING_MESSAGE_EXP,							// °æÇèÄ¡(Exp)
	UI_STRING_MESSAGE_TEAM,							// ÆÀ(Team)
	UI_STRING_MESSAGE_MESSAGE,						// ¸Þ¼¼Áö(Message)
	UI_STRING_MESSAGE_HELP,							// µµ¿ò¸»(Help)
	UI_STRING_MESSAGE_INVENTORY,					// ¾ÆÀÌÅÛÃ¢(Inventory)
	UI_STRING_MESSAGE_GEAR_WINDOW,					// ÀåÂøÃ¢(GearWindow)
	UI_STRING_MESSAGE_INFO_WINDOW,					// Á¤º¸Ã¢(InfoWindow)
	UI_STRING_MESSAGE_PARTY_WINDOW,					// ÆÄÆ¼Ã¢(PartyWindow)
	UI_STRING_MESSAGE_QUEST_WINDOW,					// Äù½ºÆ®(QuestWindow)
	UI_STRING_MESSAGE_TEAM_INFO,					// ÆÀ Á¤º¸(Team Info)
	UI_STRING_MESSAGE_TEAM_MEMBER_LIST,				// ÆÀ¿ø ¸®½ºÆ®(Team Member List)
	UI_STRING_MESSAGE_BASIC_HELP,					// ±âº» µµ¿ò¸»
	UI_STRING_MESSAGE_CHATTING_HELP,				// Ã¤ÆÃ µµ¿ò¸»
	UI_STRING_MESSAGE_WAR_HELP,						// ÀüÅõ µµ¿ò¸»
	UI_STRING_MESSAGE_SKILL_HELP,					// ½ºÅ³ µµ¿ò¸»
	UI_STRING_MESSAGE_TEAM_HELP,					// ÆÀ µµ¿ò¸»
	UI_STRING_MESSAGE_CLAN,							// Å¬·£(Clan)
	UI_STRING_MESSAGE_CLAN_INFO,					// Å¬·£ Á¤º¸(Clan Info)
	UI_STRING_MESSAGE_CLAN_MEMBER_LIST,				// Å¬·£¿ø ¸®½ºÆ®(Clan Member List)
	UI_STRING_MESSAGE_CLAN_HELP,					// Å¬·£ µµ¿ò¸»
	UI_STRING_MESSAGE_MAGIC_HELP,					// ¸¶¹ý µµ¿ò¸»

	// Simple Info
	UI_STRING_MESSAGE_CURRENT_EXP,					// ÇöÀç °æÇèÄ¡
	UI_STRING_MESSAGE_NEXT_LEVEL,					// ´ÙÀ½ ·¹º§
	UI_STRING_MESSAGE_LEFT_EXP,						// ³²Àº °æÇèÄ¡

	// Inventory
	UI_STRING_MESSAGE_EQUIP_HELM,					// ¸ðÀÚ¸¦ ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_NECKLACE,				// ¸ñ°ÉÀÌ¸¦ ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_ARMOR,					// °©¿ÊÀ» ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_SHIELD,					// ¹æÆÐ¸¦ ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_WEAPON,					// ¹«±â¸¦ ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_GLOVE,					// Àå°©À» ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_BELT,					// º§Æ®¸¦ ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_TROUSER,				// ¹ÙÁö¸¦ ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_BRACELET,				// ÆÈÂî¸¦ ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_RING,					// ¹ÝÁö¸¦ ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_SHOES,					// ½Å¹ßÀ» ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_VAMPIRE_COAT,			// ¿ÊÀ» ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_EARRING,				// ±Í°ÉÀÌ¸¦ ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_AMULET,					// ¾Æ¹Ä·¿À» ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_FIRST_GEAR_SET,				// Ã¹¹øÂ° ¹«±â¼Â
	UI_STRING_MESSAGE_SECOND_GEAR_SET,				// µÎ¹øÂ° ¹«±â¼Â

	// Chating Description
	UI_STRING_MESSAGE_SPECIAL_CHARACTER,			//	"Æ¯¼ö¹®ÀÚ(Control+X)",
	UI_STRING_MESSAGE_SET_LETTER_COLOR,				//	"±Û¾¾»öÁöÁ¤",
	UI_STRING_MESSAGE_CHANGE_INPUT_LANGUAGE,		//	"ÇÑ/¿µ ÀüÈ¯",
	UI_STRING_MESSAGE_SHOW_WHISPER_ID,				//	"±Ó¼Ó¸» Çß´ø ¾ÆÀÌµð º¸±â",
	UI_STRING_MESSAGE_NORMAL_CHATING,				//	"ÀÏ¹ÝÃ¤ÆÃ ÇÏ±â(Control+C)",
	UI_STRING_MESSAGE_ZONE_CHATTING,				//	"Á¸Ã¤ÆÃ ÇÏ±â(Control+Z)",
	UI_STRING_MESSAGE_WHISPER_CHATTING,				//	"±Ó¼Ó¸» ÇÏ±â(Control+W)",
	UI_STRING_MESSAGE_PARTY_CHATTING,				//	"ÆÄÆ¼Ã¤ÆÃ ÇÏ±â",
	UI_STRING_MESSAGE_TEAM_CHATTING,				//	"ÆÀÃ¤ÆÃ ÇÏ±â(Control+G)",
	UI_STRING_MESSAGE_SHOW_NORMAL_CHATTING,			//	"ÀÏ¹ÝÃ¤ÆÃ º¸±â",
	UI_STRING_MESSAGE_SHOW_ZONE_CHATTING,			//	"Á¸Ã¤ÆÃ º¸±â",
	UI_STRING_MESSAGE_SHOW_WHISPER_CHATTING,		//	"±Ó¼Ó¸» º¸±â",
	UI_STRING_MESSAGE_SHOW_PARTY_CHATTING,			//	"ÆÄÆ¼Ã¤ÆÃ º¸±â",
	UI_STRING_MESSAGE_SHOW_TEAM_CHATTING,			//	"ÆÀÃ¤ÆÃ º¸±â",
	UI_STRING_MESSAGE_NOT_SHOW_NORMAL_CHATTING,		//	"ÀÏ¹ÝÃ¤ÆÃ ¾Èº¸±â",
	UI_STRING_MESSAGE_NOT_SHOW_ZONE_CHATTING,		//	"Á¸Ã¤ÆÃ ¾Èº¸±â",
	UI_STRING_MESSAGE_NOT_SHOW_WHISPER_CHATTING,	//	"±Ó¼Ó¸» ¾Èº¸±â",
	UI_STRING_MESSAGE_NOT_SHOW_PARTY_CHATTING,		//	"ÆÄÆ¼Ã¤ÆÃ ¾Èº¸±â",
	UI_STRING_MESSAGE_NOT_SHOW_TEAM_CHATTING,		//	"ÆÀÃ¤ÆÃ ¾Èº¸±â",
	UI_STRING_MESSAGE_NOT_SHOW_CLAN_CHATTING,		//	"Å¬·£Ã¤ÆÃ ¾Èº¸±â",
	UI_STRING_MESSAGE_SHOW_CLAN_CHATTING,			//	"Å¬·£Ã¤ÆÃ º¸±â",
	UI_STRING_MESSAGE_CLAN_CHATTING,				//	"Å¬·£Ã¤ÆÃ ÇÏ±â",
	UI_STRING_MESSAGE_EXPEL,						// "Å»Åð",
	UI_STRING_MESSAGE_SECEDE,						// "Ãß¹æ",
	
	// Party
	UI_STRING_MESSAGE_SET_INVITE_DENY_MODE,			// ÃÊÃ» °ÅºÎ¸ðµå ¼³Á¤
	UI_STRING_MESSAGE_CANCEL_INVITE_DENY_MODE,		// ÃÊÃ» °ÅºÎ¸ðµå ÇØÁ¦
	UI_STRING_MESSAGE_SET_JOIN_DENY_MODE,			// "°¡ÀÔ °ÅºÎ¸ðµå ¼³Á¤",
	UI_STRING_MESSAGE_CANCEL_JOIN_DENY_MODE,		// "°¡ÀÔ °ÅºÎ¸ðµå ÇØÁ¦",
	

	// Level Up	
	UI_STRING_MESSAGE_UP_STR,						//		"ÈûÀ» ¿Ã¸³´Ï´Ù",
	UI_STRING_MESSAGE_UP_DEX,						//		"¹ÎÃ¸¼ºÀ» ¿Ã¸³´Ï´Ù",
	UI_STRING_MESSAGE_UP_INT,						//		"Áö½ÄÀ» ¿Ã¸³´Ï´Ù",

	
	// INFO WINDOW
	UI_STRING_MESSAGE_SHOW_SKILL_INFO_WINDOW,		// "½ºÅ³ Á¤º¸Ã¢À» º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_CHARACTER_INFO_WINDOW,	// "Ä³¸¯ÅÍ Á¤º¸Ã¢À» º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_MAGIC_INFO_WINDOW,		// "¸¶¹ý Á¤º¸Ã¢À» º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_ADVANTE_INFO_WINODW,		// "2Â÷ÀüÁ÷ Á¤º¸Ã¢À» º¾´Ï´Ù.".
	UI_STRING_MESSAGE_SHOW_BLADE_INFO,				//		"µµ°è¿­ÀÇ Á¤º¸¸¦ º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_SWORD_INFO,				//		"°Ë°è¿­ÀÇ Á¤º¸¸¦ º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_GUN_INFO,				//		"ÃÑ°è¿­ÀÇ Á¤º¸¸¦ º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_HEAL_INFO,				//		"È¸º¹°è¿­ÀÇ Á¤º¸¸¦ º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_ENCHANT_INFO,			//		"Ãàº¹°è¿­ÀÇ Á¤º¸¸¦ º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_ALL_INFO,				//		"ÀüÃ¼°è¿­ÀÇ Á¤º¸¸¦ º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_POISON_INFO,				//		"µ¶°è¿­ÀÇ Á¤º¸¸¦ º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_ACID_INFO,				//		"»ê°è¿­ÀÇ Á¤º¸¸¦ º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_CURSE_INFO,				//		"ÀúÁÖ°è¿­ÀÇ Á¤º¸¸¦ º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_SUMMON_INFO,				//		"¼ÒÈ¯°è¿­ÀÇ Á¤º¸¸¦ º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_BLOOD_INFO,				//		"Ç÷°è¿­ÀÇ Á¤º¸¸¦ º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_ESSENCE_INFO,			//		"°íÀ¯°è¿­ÀÇ Á¤º¸¸¦ º¾´Ï´Ù",
	
	

	// SHOW HELP WINDOW
	UI_STRING_MESSAGE_SHOW_HELP_MAGIC_INFO_WINDOW,	//		"¸¶¹ý Á¤º¸Ã¢ÀÇ µµ¿ò¸»À» º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_HELP_SKILL_INFO_WINDOW,	//		"½ºÅ³ Á¤º¸Ã¢ÀÇ µµ¿ò¸»À» º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_HELP_INVENTORY_WINDOW,	// "InventoryÃ¢ÀÇ µµ¿ò¸»À» º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_HELP_PARTY_MANAGER,		// "PartyManagerÃ¢ÀÇ µµ¿ò¸»À» º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_HELP_CHARACTER_INFO_WINDOW,	//		"Ä³¸¯ÅÍ Á¤º¸Ã¢ÀÇ µµ¿ò¸»À» º¾´Ï´Ù",	
	UI_STRING_MESSAGE_SHOW_HELP_GEAR_WINDOW,			// GearÃ¢ÀÇ µµ¿ò¸»À» º¾´Ï´Ù.
	UI_STRING_MESSAGE_SHOW_HELP_STORAGE_WINDOW,			//"StorageÃ¢ÀÇ µµ¿ò¸»À» º¾´Ï´Ù.",
	UI_STRING_MESSAGE_SHOW_HELP_SHOP_WINDOW,			//"ShopÃ¢ÀÇ µµ¿ò¸»À» º¾´Ï´Ù",
	UI_STRING_MESSAGE_DESC_SHOW_EXCHANGE_WINDOW,			//"ExchangeÃ¢ÀÇ µµ¿ò¸»À» º¾´Ï´Ù",
	


	// CLOSE WINDOW
	UI_STRING_MESSAGE_CLOSE_INVENTORY_WINDOW,			// "InventoryÃ¢À» ´Ý½À´Ï´Ù",	
	UI_STRING_MESSAGE_CLOSE_PARTY_MANAGER,				// "PartyManagerÃ¢À» ´Ý½À´Ï´Ù",	
	UI_STRING_MESSAGE_CLOSE_CHARACTER_INFO_WINDOW,		//		"Ä³¸¯ÅÍ Á¤º¸Ã¢À» ´Ý½À´Ï´Ù",
	UI_STRING_MESSAGE_CLOSE_GEAR_WINDOW,				// Gear Ã¢À» ´Ý½À´Ï´Ù.
	UI_STRING_MESSAGE_CLOSE_MAGIC_INFO_WINDOW,			//		"¸¶¹ý Á¤º¸Ã¢À» ´Ý½À´Ï´Ù",
	UI_STRING_MESSAGE_CLOSE_SKILL_INFO_WINDOW,			//		"½ºÅ³ Á¤º¸Ã¢À» ´Ý½À´Ï´Ù",
	UI_STRING_MESSAGE_CLOSE_DESC_WINDOW,				//  ¼³¸íÃ¢À» ´Ý½À´Ï´Ù
	UI_STRING_MESSAGE_CLOSE_STORAGE_WINDOW,				//"StorageÃ¢À» ´Ý½À´Ï´Ù.",
	UI_STRING_MESSAGE_CLOSE_SHOP_WINDOW,				//"ShopÃ¢À» ´Ý½À´Ï´Ù",
	UI_STRING_MESSAGE_CLOSE_BOOKCASE,					// Ã¥ÀåÀ» ´Ý½À´Ï´Ù.
	UI_STRING_MESSAGE_CLOSE_BOOK,						// "Ã¥À» ´Ý½À´Ï´Ù",
	
	
	
	UI_STRING_MESSAGE_NOT_JOIN_ANY_TEAM,				// µî·ÏµÈ ÆÀ ¾øÀ½
	UI_STRING_MESSAGE_NOT_JOIN_ANY_CLAN,				// µî·ÏµÈ Å¬·£ ¾øÀ½
	UI_STRING_MESSAGE_INFRA_RED_HELMET,					// ÀÎÇÁ¶ó·¹µå ½ºÄ³´× Çï¸ä
	UI_STRING_MESSAGE_INFRA_HELMET,						// ÀÎÇÁ¶ó ½ºÄ³´× Çï¸ä
	UI_STRING_MESSAGE_QUIT_COMPUTER,					// "ÄÄÇ»ÅÍ¸¦ Á¾·áÇÕ´Ï´Ù"

	UI_STRING_MESSAGE_SHOW_NEXT_PAGE,					//"ÀÌÀü ÆäÀÌÁö¸¦ º¾´Ï´Ù",
	UI_STRING_MESSAGE_SHOW_PREV_PAGE,					//"´ÙÀ½ ÆäÀÌÁö¸¦ º¾´Ï´Ù",


	UI_STRING_MESSAGE_ENG_STR,						// STR
	UI_STRING_MESSAGE_ENG_DEX,						// DEX
	UI_STRING_MESSAGE_ENG_INT,						// INT
	
	UI_STRING_MESSAGE_MASTER_NAME,					// ¿î¿µÀÚ    
	UI_STRING_MESSAGE_LIMIT_STRING_COUNT,			// µµ¹èÇÏÁö ¸¶¼¼¿ä!!!
	UI_STRING_MESSAGE_HELP_MESSAGE,					// µµ¿ò¸»
	UI_STRING_MESSAGE_STR,							// Èû
	UI_STRING_MESSAGE_DEX,							// ¹ÎÃ¸¼º
	UI_STRING_MESSAGE_INT,							// Áö½Ä
	UI_STRING_MESSAGE_HP,							// »ý¸í·Â
	UI_STRING_MESSAGE_MP,							// ¸¶¹ý·Â
	UI_STRING_MESSAGE_ACCURACY,						// ¸íÁß·ü
	UI_STRING_MESSAGE_DAMAGE,						// µ¥¹ÌÁö
	UI_STRING_MESSAGE_AVOID,						// È¸ÇÇÀ²
	UI_STRING_MESSAGE_DEFENCE,						// ¹æ¾îÀ²
	UI_STRING_MESSAGE_ALIGNMENT,					// ¼ºÇâ

	UI_STRING_MESSAGE_ALIGN_VERY_BAD,				// ¸Å¿ì ¾ÇÇÔ
	UI_STRING_MESSAGE_ALIGN_BAD,					// ¾ÇÇÔ
	UI_STRING_MESSAGE_ALIGN_NORMAL,					// Áß¸³
	UI_STRING_MESSAGE_ALIGN_GOOD,					// ¼±ÇÔ
	UI_STRING_MESSAGE_ALIGN_VERY_GOOD,				// ¸Å¿ì ¼±ÇÔ

	UI_STRING_MESSAGE_STR_PURE,						// "¼ø   ¼ö   Èû : %d",
	UI_STRING_MESSAGE_DEX_PURE,						// "¼ø¼ö  ¹ÎÃ¸¼º : %d",
	UI_STRING_MESSAGE_INT_PURE,						// "¼ø ¼ö  Áö ½Ä : %d",
	UI_STRING_MESSAGE_STATUS_SUM_PURE,				// "¼ø¼ö´É·ÂÄ¡ÇÕ : %d",
	UI_STRING_MESSAGE_STATUS_SUM,					// "´É ·Â Ä¡  ÇÕ : %d",

	UI_STRING_MESSAGE_SPEED_SLOW,					// ´À¸²
	UI_STRING_MESSAGE_SPEED_NORMAL,					// º¸Åë
	UI_STRING_MESSAGE_SPEED_FAST,					// ºü¸§

	UI_STRING_MESSAGE_ATTACK_SPEED,					// "°ø°Ý¼Óµµ : %s[%d]"

	// HpBar	
	UI_STRING_MESSAGE_PASSIVE,						// "Passive"
	UI_STRING_MESSAGE_HAN_PASSIVE,					// ÆÐ½Ãºê

	UI_STRING_MESSAGE_HPBAR_LEVEL_DESCRIPTION,		// "LEVEL:%d"
	UI_STRING_MESSAGE_HPBAR_EXP_DESCRIPTION,		// "EXP:%s/%s (³²Àº °æÇèÄ¡:%s)"
	UI_STRING_MESSAGE_HPBAR_MP_DESCRIPTION,			// "MP:%d/%d"
	UI_STRING_MESSAGE_HPBAR_HP_DESCRIPTION_WITH_SILVERING,			// "HP:%d/%d(S:%d)"
	UI_STRING_MESSAGE_HPBAR_HP_DESCRIPTION,							// "HP:%d/%d"	
	UI_STRING_MESSAGE_HPBAR_CHANGE_VAMPIRE_DAY,		// "%dÀÏ %d½Ã°£ %dºÐ ÈÄ¿¡ ¹ìÆÄÀÌ¾î·Î º¯ÇÕ´Ï´Ù."
	UI_STRING_MESSAGE_HPBAR_CHANGE_VAMPIRE_HOUR,	// "%d½Ã°£ %dºÐ ÈÄ¿¡ ¹ìÆÄÀÌ¾î·Î º¯ÇÕ´Ï´Ù."
	UI_STRING_MESSAGE_HPBAR_CHANGE_VAMPIRE_MINUTE,	// "%dºÐ ÈÄ¿¡ ¹ìÆÄÀÌ¾î·Î º¯ÇÕ´Ï´Ù."
	UI_STRING_MESSAGE_HPBAR_CHANGE_VAMPIRE_SOON,	// "°ð ¹ìÆÄÀÌ¾î·Î º¯ÇÕ´Ï´Ù."
	UI_STRING_MESSAGE_HPBAR_SET_LARGE,				// "HP Bar¸¦ ¼¼·Î·Î º¾´Ï´Ù",
	UI_STRING_MESSAGE_HPBAR_SET_SMALL,				// "HP Bar¸¦ ÀÛ°Ô º¾´Ï´Ù",
	UI_STRING_MESSAGE_HPBAR_SET_WIDTH,				// "HP Bar¸¦ °¡·Î·Î º¾´Ï´Ù",
	UI_STRING_MESSAGE_HPBAR_SET_HEIGHT,				// "HP Bar¸¦ Å©°Ô º¾´Ï´Ù",

	
	UI_STRING_MESSAGE_SHOW_WIDTH,					// °¡·Î·Î º¾´Ï´Ù.
	UI_STRING_MESSAGE_SHOW_HEIGHT,					// ¼¼·Î·Î º¾´Ï´Ù.

	// Guild
	UI_STRING_MESSAGE_SHOW_HELP,					//"µµ¿ò¸»À» º¾´Ï´Ù.",
	UI_STRING_MESSAGE_CLOSE_WINDOW,					//"Ã¢À» ´Ý½À´Ï´Ù.",
	UI_STRING_MESSAGE_REGIST,						//"µî·ÏÇÕ´Ï´Ù.",
	UI_STRING_MESSAGE_LIST_UP,						//"À­¸ñ·Ï",
	UI_STRING_MESSAGE_LIST_DOWN,					//"¾Æ·¡¸ñ·Ï",
	UI_STRING_MESSAGE_REFRESH_LIST,					//"¸ñ·ÏÀ» ´Ù½Ã ¹Þ¾Æ¿É´Ï´Ù.",
	UI_STRING_MESSAGE_SORT_TEAM_NAME,				//		"ÆÀÀÌ¸§À¸·Î Á¤·ÄÇÕ´Ï´Ù.",
	UI_STRING_MESSAGE_SORT_LEADER_NAME,				//"¸®´õÀÌ¸§À¸·Î Á¤·ÄÇÕ´Ï´Ù.",
	UI_STRING_MESSAGE_SORT_EXPIRE_DATE,				//"¸¸±âÀÏ·Î Á¤·ÄÇÕ´Ï´Ù.",
	UI_STRING_MESSAGE_SORT_NUMBER_MEMBER,			//"¸â¹ö¼ö·Î Á¤·ÄÇÕ´Ï´Ù.",
	UI_STRING_MESSAGE_SORT_RANKING,					//"¼øÀ§·Î Á¤·ÄÇÕ´Ï´Ù.",

	UI_STRING_MESSAGE_CHANGE_FIND_MODE,				//"°Ë»ö ¹æ¹ý º¯°æ",
	UI_STRING_MESSAGE_FIND,							//"°Ë»öÇÏ±â",

	UI_STRING_MESSAGE_TEAM_GRADE_MASTER,			//"¸¶½ºÅÍ",
	UI_STRING_MESSAGE_TEAM_GRADE_SUB_MASTER,		//"¼­ºê ¸¶½ºÅÍ",
	UI_STRING_MESSAGE_TEAM_GRADE_WAIT,				//"µî·Ï ´ë±âÀÚ"
	UI_STRING_MESSAGE_TEAM_GRADE_MEMBER,			//"È¸¿ø"

	UI_STRING_MESSAGE_TEAM_INFO_GRADE,				//Grade : %s
	UI_STRING_MESSAGE_TEAM_INFO_NAME,				//Name : %s
	UI_STRING_MESSAGE_TEAM_INFO_LEADER,				//Leader : %s
	UI_STRING_MESSAGE_TEAM_INFO_REG_FEE,			//Reg. Fee : $%s
	UI_STRING_MESSAGE_TEAM_INFO_MEMBERS,			//Members : "
	UI_STRING_MESSAGE_TEAM_INFO_INTRODUCTION,		//Introduction : "
	UI_STRING_MESSAGE_TEAM_INFO_CLAN_INTRODUCTION,	//Clan Introduction : "
	UI_STRING_MESSAGE_TEAM_INFO_TEAM_INTRODUCTION,	//Team Introduction : "
	UI_STRING_MESSAGE_TEAM_INFO_RANKING,			//Ranking : %d

	UI_STRING_MESSAGE_TEAM_INFO_TEAM_NAME,			//Team Name : %s"
	UI_STRING_MESSAGE_TEAM_INFO_CLAN_NAME,			//Clan Name : %s"
	UI_STRING_MESSAGE_TEAM_INFO_REGISTRATION_FEE,	//Registration Fee : $%s"
	UI_STRING_MESSAGE_TEAM_INFO_SELF_INTRODUCTION,	//Self Introduction : "

	UI_STRING_MESSAGE_UP,							//À§·Î
	UI_STRING_MESSAGE_DOWN,							//¾Æ·¡·Î
	UI_STRING_MESSAGE_TEAM_INFO_EXPEL,				//"Ãß¹æÇÕ´Ï´Ù.",
	UI_STRING_MESSAGE_TEAM_INFO_JOIN_ACCEPT,		//"°¡ÀÔÇã°¡",
	UI_STRING_MESSAGE_TEAM_INFO_JOIN_DENY,			//"°¡ÀÔ°ÅºÎ",
	UI_STRING_MESSAGE_CANCEL,						// Ãë¼Ò

	
	// Zone Name

	UI_STRING_MESSAGE_LIMBOLAIR,					//¸²º¸¼º
	UI_STRING_MESSAGE_ESLANIA,						//¿¡½½¶ó´Ï¾È
	UI_STRING_MESSAGE_RODIN,						//·Îµò»ê
	UI_STRING_MESSAGE_DROBETA,						//µå·Îº£Å¸
	UI_STRING_MESSAGE_PERONA,						//Æä·Î³ª±¹µµ
	UI_STRING_MESSAGE_TIMORE,						//Æ¼¸ð¸£ È£¼ö 

	UI_STRING_MESSAGE_ZONEINFO_XY,					// X:%d Y:%d

	// Accelerator Name
	UI_STRING_MESSAGE_ACCELERATOR_NULL,				//"AcceleratorNULL",
	UI_STRING_MESSAGE_ACCELERATOR_F1,				//"F1",
	UI_STRING_MESSAGE_ACCELERATOR_F2,				//"F2",
	UI_STRING_MESSAGE_ACCELERATOR_F3,				//"F3",
	UI_STRING_MESSAGE_ACCELERATOR_F4,				//"F4",
	UI_STRING_MESSAGE_ACCELERATOR_F5,				//"F5",
	UI_STRING_MESSAGE_ACCELERATOR_F6,				//"F6",
	UI_STRING_MESSAGE_ACCELERATOR_F7,				//"F7",
	UI_STRING_MESSAGE_ACCELERATOR_F8,				//"F8",
	UI_STRING_MESSAGE_ACCELERATOR_F9,				//"F9",
	UI_STRING_MESSAGE_ACCELERATOR_F10,				//"F10",
	UI_STRING_MESSAGE_ACCELERATOR_F11,				//"F11",
	UI_STRING_MESSAGE_ACCELERATOR_F12,				//"F12",
	UI_STRING_MESSAGE_ACCELERATOR_ESC,				//"ESC",
	UI_STRING_MESSAGE_ACCELERATOR_SKILL,			//"SKILL",
	UI_STRING_MESSAGE_ACCELERATOR_INVENTORY,		//"INVENTORY",
	UI_STRING_MESSAGE_ACCELERATOR_GEAR,				//"GEAR",
	UI_STRING_MESSAGE_ACCELERATOR_CHARINFO,			//"CHARINFO",
	UI_STRING_MESSAGE_ACCELERATOR_SKILLINFO,		//"SKILLINFO",
	UI_STRING_MESSAGE_ACCELERATOR_MINIMAP,			//"MINIMAP",
	UI_STRING_MESSAGE_ACCELERATOR_PARTY,			//"PARTY",
	UI_STRING_MESSAGE_ACCELERATOR_MARK,				//"MARK",
	UI_STRING_MESSAGE_ACCELERATOR_HELP,				//"HELP",
	UI_STRING_MESSAGE_ACCELERATOR_QUICKITEM_SLOT,	//"QUICKITEM_SLOT",
	UI_STRING_MESSAGE_ACCELERATOR_EXTEND_CHAT,		//"EXTEND_CHAT",
	UI_STRING_MESSAGE_ACCELERATOR_CHAR,				//"CHAT",
	UI_STRING_MESSAGE_ACCELERATOR_GUILD_CHAT,		//"GUILD_CHAT",
	UI_STRING_MESSAGE_ACCELERATOR_ZONE_CHAT,		//"ZONE_CHAT",
	UI_STRING_MESSAGE_ACCELERATOR_WHISPER,			//"WHISPER",

	// ITEM CLASS NAME

	UI_STRING_MESSAGE_ITEM_CLASS_SWORD,				//°Ë(SWORD)
	UI_STRING_MESSAGE_ITEM_CLASS_BLADE,				//µµ(BLADE)
	UI_STRING_MESSAGE_ITEM_CLASS_CROSS,				//½ÊÀÚ°¡(CROSS)
	UI_STRING_MESSAGE_ITEM_CLASS_MACE,				//¸ÞÀÌ½º(MACE)
	UI_STRING_MESSAGE_ITEM_CLASS_MINE,				//Áö·Ú(MINE)
	UI_STRING_MESSAGE_ITEM_CLASS_BOMB,				//ÆøÅº(BOMB)
	UI_STRING_MESSAGE_ITEM_CLASS_MINE_MATERIAL,		//Áö·Ú Àç·á(MATERIAL)
	UI_STRING_MESSAGE_ITEM_CLASS_BOMB_MATERIAL,		//ÆøÅº Àç·á(MATERIAL)
	UI_STRING_MESSAGE_ITEM_CLASS_SG,				//»êÅºÃÑ(SG)
	UI_STRING_MESSAGE_ITEM_CLASS_SMG,				//ÀÚµ¿¼ÒÃÑ(SMG)
	UI_STRING_MESSAGE_ITEM_CLASS_AR,				//¼ÒÃÑ
	UI_STRING_MESSAGE_ITEM_CLASS_SR,				//Àú°ÝÃÑ
	UI_STRING_MESSAGE_DESC_CLASS,					//°è¿­ : "
	UI_STRING_MESSAGE_DESC_DURABILITY,				//³»±¸¼º : "
	UI_STRING_MESSAGE_DESC_SILVERING,				//Àºµµ±Ý : "
	UI_STRING_MESSAGE_DESC_DAMAGE,					//µ¥¹ÌÁö : "
	UI_STRING_MESSAGE_DESC_CRITICALHIT,				//Å©¸®Æ¼ÄÃ È÷Æ® : "
	UI_STRING_MESSAGE_DESC_DEFENSE,					//µðÆæ½º : "
	UI_STRING_MESSAGE_DESC_PROTECTION,				//ÇÁ·ÎÅØ¼Ç : "
	UI_STRING_MESSAGE_DESC_ACCURACY,				//¸íÁß·ü : "
	UI_STRING_MESSAGE_DESC_HP,						//HP : "
	UI_STRING_MESSAGE_DESC_MP,						//MP : "
	UI_STRING_MESSAGE_DESC_RANGE,					//»çÁ¤°Å¸® : "
	UI_STRING_MESSAGE_DESC_MAGAZINE_NUM,			//ÅºÈ¯¼ö : "
	UI_STRING_MESSAGE_DESC_POCKET_NUM,				//ÁÖ¸Ó´Ï °³¼ö : "
	UI_STRING_MESSAGE_DESC_ITEM_NUM,				//¾ÆÀÌÅÛ °³¼ö : "
	UI_STRING_MESSAGE_DESC_LEFT_NUM,				//³²Àº »ç¿ëÈ¸¼ö : "
	UI_STRING_MESSAGE_DESC_ARRIVAL_LOCATION,		//µµÂøÀ§Ä¡ : "
	UI_STRING_MESSAGE_DESC_NOT_EXIST,				//¾øÀ½

	UI_STRING_MESSAGE_DESC_NUMBER,						// °³
	UI_STRING_MESSAGE_DESC_MAGAZINE_COUNT,				// ¹ß
	UI_STRING_MESSAGE_DESC_TILE_PIECE,					// Ä­
	UI_STRING_MESSAGE_DESC_PARTY_NAME,					// ÆÄÆ¼

	UI_STRING_MESSAGE_DESC_OPTION,						// ¿É¼Ç : "
	UI_STRING_MESSAGE_DESC_REQUIRE,						// ¿ä±¸Á¶°Ç : "
	UI_STRING_MESSAGE_DESC_REQUIRE_STAT,					// %d ÀÌ»ó
	UI_STRING_MESSAGE_DESC_ONLY_MALE,					// ³²¼º¿ë
	UI_STRING_MESSAGE_DESC_ONLY_FEMALE,					// ¿©¼º¿ë
	UI_STRING_MESSAGE_DESC_ONLY_SLAYER,					// ½½·¹ÀÌ¾î Àü¿ë
	UI_STRING_MESSAGE_DESC_ONLY_VAMPIRE,				// ¹ìÆÄÀÌ¾î Àü¿ë
	UI_STRING_MESSAGE_DESC_ALL_STAT_SUM,				// ´É·ÂÄ¡ ÃÑÇÕ 
	UI_STRING_MESSAGE_DESC_LEVEL,						// ·¹º§
	UI_STRING_MESSAGE_DESC_PRICE,						// °¡°Ý : "
	UI_STRING_MESSAGE_DESC_SILVERING_PRICE,				//Àºµµ±Ý °¡°Ý : "
	UI_STRING_MESSAGE_DESC_CHARGE_PRICE,				// ÃæÀü°¡°Ý : "
	UI_STRING_MESSAGE_DESC_REPAIR_PRICE,				// ¼ö¸®°¡°Ý : "

	UI_STRING_MESSAGE_CONSUME_HP,						// HP¼Ò¸ð·® : "
	UI_STRING_MESSAGE_CONSUME_MP,						// MP¼Ò¸ð·® : "

	UI_STRING_MESSAGE_REQUIRE_LEVEL,					// ¿ä±¸ ·¹º§ : "
	UI_STRING_MESSAGE_DESC_EXP,							// °æÇèÄ¡ : "
	UI_STRING_MESSAGE_DESC_LIMIT_LEVEL,					//¼ºÀåÁ¦ÇÑ·¹º§ : "
	UI_STRING_MESSAGE_DESC_CANNOT_LEARN_SKILL,			// \"¾ÆÁ÷ ¹è¿ï ¼ö ¾ø½À´Ï´Ù.\"
	UI_STRING_MESSAGE_DESC_CAN_LEARN_SKILL,				// \"¹è¿ï ¼ö ÀÖ½À´Ï´Ù.\"

	UI_STRING_MESSAGE_MOVE,								// ÀÌµ¿ÇÕ´Ï´Ù
	UI_STRING_MESSAGE_CANCEL_MOVE,						// ÀÌµ¿À» Ãë¼ÒÇÕ´Ï´Ù
	UI_STRING_MESSAGE_MOVE_F1,							// 1ÃþÀ¸·Î °©´Ï´Ù
	UI_STRING_MESSAGE_MOVE_F2,							// 2ÃþÀ¸·Î °©´Ï´Ù
	UI_STRING_MESSAGE_MOVE_F3,							// 3ÃþÀ¸·Î °©´Ï´Ù
	UI_STRING_MESSAGE_MOVE_F4,							// 4ÃþÀ¸·Î °©´Ï´Ù
	UI_STRING_MESSAGE_MOVE_B1,							// ÁöÇÏ1ÃþÀ¸·Î °©´Ï´Ù

	UI_STRING_MESSAGE_DESC_CLICK_EXCHANGE,					//	"±³È¯ÇÏ½Ã·Á¸é ´­·¯ÁÖ¼¼¿ä",
	UI_STRING_MESSAGE_DESC_CANCEL_OK_BUTTON,				// "OKÇÑ°ÍÀ» Ãë¼ÒÇÕ´Ï´Ù",
	UI_STRING_MESSAGE_DESC_CANCEL_EXCHANGE,					// "±³È¯ÇÏ±â·Î ÇÑ °É Ãë¼ÒÇÕ´Ï´Ù",
	UI_STRING_MESSAGE_DESC_BRING_MONEY,						// "±³È¯ÇÏ±â·Î ÇÑ µ·À» µÇÃ£½À´Ï´Ù",
	UI_STRING_MESSAGE_DESC_EXCHANGE_MONEY,					// "µ·À» ±³È¯ÇÕ´Ï´Ù",
	UI_STRING_MESSAGE_DESC_EXCHANGE_YOUR_MONEY,				//"»ó´ë¹æÀÌ ±³È¯ÇÏ±â·Î ÇÑ µ·",

	UI_STRING_MESSAGE_DESC_LEFT_MONEY_AFTER_EXCHANGE,		// "±³È¯ÇÏ°í ³ª¸é ³²´Â ±Ý¾×",
	UI_STRING_MESSAGE_DESC_WILL_EXCHANGE_MONEY,				// "±³È¯ÇÒ ±Ý¾×",

	UI_STRING_MESSAGE_TRHOW_MONEY_IN_DIALOG,				//{  "¹ö¸± ±Ý¾×À» ÀÔ·ÂÇÏ¼¼¿ä." } ,
	UI_STRING_MESSAGE_SAVE_MONEY_IN_DIALOG,					//	{  "º¸°üÇÒ ±Ý¾×À» ÀÔ·ÂÇÏ¼¼¿ä." } ,
	UI_STRING_MESSAGE_BRING_MONEY_IN_DIALOG,				//{  "Ã£À» ±Ý¾×À» ÀÔ·ÂÇÏ¼¼¿ä." } ,
	UI_STRING_MESSAGE_TRADE_MONEY_IN_DIALOG,				//{  "°Å·¡ÇÒ ±Ý¾×À» ÀÔ·ÂÇÏ¼¼¿ä." } ,
	UI_STRING_MESSAGE_DIVIDE_MONEY_IN_DIALOG,				//{  "»©³¾ ±Ý¾×À» ÀÔ·ÂÇÏ¼¼¿ä." } ,
	
	UI_STRING_MESSAGE_BUY_ITEM,							// ÀÌ ¾ÆÀÌÅÛÀ» »ç½Ã°Ú½À´Ï±î?
	UI_STRING_MESSAGE_BUY_ITEM_NUM,						// {"ÀÌ ¾ÆÀÌÅÛ            °³¸¦ »ç½Ã°Ú½À´Ï±î?"};

	UI_STRING_MESSAGE_ASK_DIALOG_BUY_STORAGE,			//{  "º¸°üÇÔÀ» $%d¿¡ »ç½Ã°Ú½À´Ï±î?"  },	
	UI_STRING_MESSAGE_ASK_DIALOG_TRADE_OTHER_PLAYER,	//{  "%s ´Ô°ú °Å·¡ÇÏ°Ú½À´Ï±î?"  },
	UI_STRING_MESSAGE_ASK_DIALOG_WAIT_OTHER_PLAYER,		//{  "%s ´ÔÀÇ ÀÀ´äÀ» ±â´Ù¸³´Ï´Ù." ,  
	UI_STRING_MESSAGE_ASK_DIALOG_CANCEL,				//"Ãë¼ÒÇÏ½Ã·Á¸é CancelÀ» ´©¸£¼¼¿ä."   },
	UI_STRING_MESSAGE_ASK_DIALOG_REQUEST_JOIN,			//{  "%s ´ÔÀÌ ´ç½ÅÀÇ %s¿¡ °¡ÀÔÀ» ¿äÃ»ÇÏ°í ÀÖ½À´Ï´Ù."  },
	UI_STRING_MESSAGE_ASK_DIALOG_INVITE,				//{  "%s ´ÔÀÌ ´ç½ÅÀ» %s¿¡ ÃÊÃ»ÇÏ¼Ì½À´Ï´Ù."  },
	UI_STRING_MESSAGE_CANNOT_FIND_RESULT,				//  "°Ë»ö °á°ú¸¦ Ã£À» ¼ö ¾ø½À´Ï´Ù."  },
	UI_STRING_MESSAGE_ENCHANT_CONFIRM,					//{  "ÀÌ ¾ÆÀÌÅÛÀ» EnchantÇÏ½Ã°Ú½À´Ï±î?", 

	UI_STRING_MESSAGE_DESC_DIALOG_ITEM_CLASS_SG,		//"\t°è¿­ : SG"
	UI_STRING_MESSAGE_DESC_DIALOG_ITEM_CLASS_SMG,		//"\t°è¿­ : SMG"
	UI_STRING_MESSAGE_DESC_DIALOG_ITEM_CLASS_AR,		//"\t°è¿­ : AR"
	UI_STRING_MESSAGE_DESC_DIALOG_ITEM_CLASS_SR,		//"\t°è¿­ : SR"

	UI_STRING_MESSAGE_DESC_DIALOG_DURABILITY,			//"\t³»±¸¼º : %d"
	UI_STRING_MESSAGE_DESC_DIALOG_SILVERING,			//"\tÀºµµ±Ý : %d"
	UI_STRING_MESSAGE_DESC_DIALOG_DAMAGE,				//"\tµ¥¹ÌÁö : %d~%d"
	UI_STRING_MESSAGE_DESC_DIALOG_CRITICALHIT,			//"\tÅ©¸®Æ¼ÄÃ È÷Æ® : %d"
	UI_STRING_MESSAGE_DESC_DIALOG_DEFENSE,				//"\tµðÆæ½º : %d"
	UI_STRING_MESSAGE_DESC_DIALOG_PROTECTION,			//"\tÇÁ·ÎÅØ¼Ç : %d"
	UI_STRING_MESSAGE_DESC_DIALOG_ACCURACY,				//"\t¸íÁß·ü : %d"
	UI_STRING_MESSAGE_DESC_DIALOG_HP,					//"\tHP : +%d"
	UI_STRING_MESSAGE_DESC_DIALOG_MP,					//"\tMP : +%d"
	UI_STRING_MESSAGE_DESC_DIALOG_RANGE,				//"\t»çÁ¤°Å¸® : %d Ä­"
	UI_STRING_MESSAGE_DESC_DIALOG_MAGAZINE_NUM,			//"\tÅºÈ¯¼ö : %d ¹ß"
	UI_STRING_MESSAGE_DESC_DIALOG_POCKET_NUM,			//"\tÁÖ¸Ó´Ï °³¼ö : %d °³"
	UI_STRING_MESSAGE_DESC_DIALOG_OPTION,				//"\t¿É¼Ç : %s +%d"
	UI_STRING_MESSAGE_DESC_DIALOG_OPTION_EMPTY,			//"\t       %s +%d"
	UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_EMPTY,		//"\t             ");
	UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE,				//"\t¿ä±¸Á¶°Ç : ");
	UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_STR,			//STR %d ÀÌ»ó
	UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_DEX,			//STR %d ÀÌ»ó
	UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_INT,			//STR %d ÀÌ»ó
	UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_ALL_STAT_SUM,	//´É·ÂÄ¡ ÃÑÇÕ %d ÀÌ»ó"
	UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_LEVEL,		//·¹º§ %d ÀÌ»ó
	UI_STRING_MESSAGE_DESC_DIALOG_PRICE,				//"\t°¡°Ý : %s"	

	UI_STRING_MESSAGE_FILE_DIALOG_SELECT_PROFILE_PICTURE,	//"ÇÁ·ÎÇÊ¿¡ ¾²ÀÏ »çÁøÀ» ¼±ÅÃÇÏ¼¼¿ä."
	UI_STRING_MESSAGE_FILE_DIALOG_SELECT_FILE,				//"Àü¼ÛÇÒ ÆÄÀÏµéÀ» ¼±ÅÃÇÏ¼¼¿ä(º¹¼ö¼±ÅÃCTRL)."
	UI_STRING_MESSAGE_FILE_DIALOG_SELECT_FILE_OK,			//"ÇØ´ç ÆÄÀÏÀ» ¼±ÅÃÇÕ´Ï´Ù.",
	UI_STRING_MESSAGE_FILE_DIALOG_CANCEL,					//"Ãë¼ÒÇÏ°í Ã¢À» ´Ý½À´Ï´Ù.",

	UI_STRING_MESSAGE_ENG_DOMAIN_BLADE,					//BLADE
	UI_STRING_MESSAGE_ENG_DOMAIN_SWORD,					//SWORD
	UI_STRING_MESSAGE_ENG_DOMAIN_ENCHANT,				//ENCHANT
	UI_STRING_MESSAGE_ENG_DOMAIN_GUN,					//GUN
	UI_STRING_MESSAGE_ENG_DOMAIN_HEAL,					//HEAL

	UI_STRING_MESSAGE_NOT_SUPPORT_MENU,					//= {"¾ÆÁ÷ Áö¿øµÇÁö ¾Ê´Â ¸Þ´ºÀÔ´Ï´Ù."};
	UI_STRING_MESSAGE_NOT_INPUT_ID_OR_PASSWORD,			//= {"ID ¶Ç´Â Password¸¦ ÀÔ·ÂÇÏÁö ¾Ê¾Ò½À´Ï´Ù."};
	UI_STRING_MESSAGE_WRONG_SSN,						//= {"ÁÖ¹Îµî·Ï¹øÈ£ÀÔ·ÂÀÌ ¹Ù¸£Áö ¾Ê½À´Ï´Ù."};
	UI_STRING_MESSAGE_INPUT_NAME,						//= {"ÀÌ¸§À» ÀÔ·ÂÇÏ¼¼¿ä."};
	UI_STRING_MESSAGE_CANNOT_CONNECT_SERVER,			//= {"¼­¹ö¿¡ ¿¬°áÇÒ ¼ö ¾ø½À´Ï´Ù."};
	UI_STRING_MESSAGE_SELECT_CHARACTER,					//= {"Ä³¸¯ÅÍ¸¦ ¼±ÅÃÇÏ¼¼¿ä."};
	UI_STRING_MESSAGE_DELETE_CHARACTER,					//= {"Ä³¸¯ÅÍ°¡ »èÁ¦µÇ¾ú½À´Ï´Ù."};
	UI_STRING_MESSAGE_ALL_SLOT_EMPTY,					//= {"Ä³¸¯ÅÍ¸¦ »õ·Î ¸¸µé°í, ¸¸µé¾îÁø Ä³¸¯ÅÍ¸¦ ¼±ÅÃÇÏ¼Å¾ß ÇÕ´Ï´Ù."};
	UI_STRING_MESSAGE_NO_MORE_CREATE_CHARACTER,			//= {"Ä³¸¯ÅÍ¸¦ ´õ ÀÌ»ó ¸¸µé ¼ö ¾ø½À´Ï´Ù."};
	UI_STRING_MESSAGE_FAIL_CREATE_CHARACTER,			//= {"Ä³¸¯ÅÍ »ý¼º¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."};
	UI_STRING_MESSAGE_NO_INPUT_NEED_INFO,				//= {"ÇÊ¼öÇ×¸ñÀÌ ¸ðµÎ ÀÔ·ÂµÇÁö ¾Ê¾Ò½À´Ï´Ù."};
	UI_STRING_MESSAGE_ALREADY_USE_ID,					//= {"»ç¿ëÁßÀÎ ID ÀÔ´Ï´Ù."};
	UI_STRING_MESSAGE_CAN_USE_ID,						//= {"»ç¿ëÇÏ½Ç ¼ö ÀÖ´Â ID ÀÔ´Ï´Ù."};
	UI_STRING_MESSAGE_RE_INPUT_PASSWORD,				//= {"ºñ¹Ð¹øÈ£¸¦ ´Ù½Ã ÀÔ·ÂÇÏ¼¼¿ä."};
	


	//Monster Name
	UI_STRING_MESSAGE_MONSTER_NAME_TNDEAD,				//	"ÅÍ´×µ¥µå",
	UI_STRING_MESSAGE_MONSTER_NAME_ARKHAN,				//	"¾ËÄ­",
	UI_STRING_MESSAGE_MONSTER_NAME_ESTROIDER,		//"¿¡½ºÆ®·ÎÀÌ´õ",
	UI_STRING_MESSAGE_MONSTER_NAME_GOLEMER,				//	"°ñ·¹¸Ó",
	UI_STRING_MESSAGE_MONSTER_NAME_DARKSCREAMER,		//	"´ÙÅ©½ºÅ©¸®¸Ó",
	UI_STRING_MESSAGE_MONSTER_NAME_DEADBODY,			//	"µ¥µå¹Ùµð",
	UI_STRING_MESSAGE_MONSTER_NAME_MODERAS,				//	"¸ðµ¥¶ó½º",
	UI_STRING_MESSAGE_MONSTER_NAME_VANDALIZER,			//	"¹ê´ú¶óÀÌÀú",
	UI_STRING_MESSAGE_MONSTER_NAME_DIRTYSTRIDER,		//	"´õÆ¼½ºÆ®¶óÀÌ´õ",
	UI_STRING_MESSAGE_MONSTER_NAME_HELLWIZARD,			//	"ÇïÀ§ÀÚµå",
	UI_STRING_MESSAGE_MONSTER_NAME_TNSOUL,				//	"ÅÍ´×¼Ò¿ï",
	UI_STRING_MESSAGE_MONSTER_NAME_IRONTEETH,			//	"¾ÆÀÌ¾ðÆ¼¾²",
	UI_STRING_MESSAGE_MONSTER_NAME_REDEYE,				//	"·¹µå¾ÆÀÌ",
	UI_STRING_MESSAGE_MONSTER_NAME_CRIMSONSLAUGTHER,	//	"Å©¸²½¼½½·ÎÅÍ",
	UI_STRING_MESSAGE_MONSTER_NAME_HELLGUARDIAN,		//	"Çï°¡µð¾ð",
	UI_STRING_MESSAGE_MONSTER_NAME_SOLDIER,				//	"¼ÖÁ®",
	UI_STRING_MESSAGE_MONSTER_NAME_RIPPER,				//	"¸®ÆÛ",
	UI_STRING_MESSAGE_MONSTER_NAME_BIGFANG,				//	"ºòÆØ",
	UI_STRING_MESSAGE_MONSTER_NAME_LORDCHAOS,			//	"·ÎµåÄ«¿À½º",
	UI_STRING_MESSAGE_MONSTER_NAME_CHAOSGUARDIAN,		//	"Ä«¿À½º°¡µð¾ð",
	UI_STRING_MESSAGE_MONSTER_NAME_HOBBLE,				//	"È£ºí",
	UI_STRING_MESSAGE_MONSTER_NAME_CHAOSNIGHT,			//	"Ä«¿À½º³ªÀÌÆ®",
	UI_STRING_MESSAGE_MONSTER_NAME_WIDOWS,				//	"À§µµ¿ì½º",
	UI_STRING_MESSAGE_MONSTER_NAME_KID,					//	"Å°µå",
	UI_STRING_MESSAGE_MONSTER_NAME_SHADOWWING,			//	"¼¨µµ¿ìÀ®",

	//Server Status
	UI_STRING_MESSAGE_SERVER_STATUS_VERY_GOOD,			//¸Å¿ì ÁÁÀ½
	UI_STRING_MESSAGE_SERVER_STATUS_GOOD,				//ÁÁÀ½
	UI_STRING_MESSAGE_SERVER_STATUS_NORMAL,				//¿øÈ°
	UI_STRING_MESSAGE_SERVER_STATUS_BAD,				//º¸Åë
	UI_STRING_MESSAGE_SERVER_STATUS_VERY_BAD,			//Å¸¼­¹ö ÀÌ¿ë±ÇÀå
	UI_STRING_MESSAGE_SERVER_STATUS_DOWN,				//¼­¹ö ´Ù¿î
	UI_STRING_MESSAGE_SERVER_STATUS_OPEN,				//¿­¸²
	UI_STRING_MESSAGE_SERVER_STATUS_CLOSE,				//´ÝÈû


	// ItemType
	UI_STRING_MESSAGE_ITEM_TYPE_NORMAL,					//"ÀÏ¹Ý ¾ÆÀÌÅÛ",
	UI_STRING_MESSAGE_ITEM_TYPE_SPECIAL,				//"Æ¯¼ö ¾ÆÀÌÅÛ",
	UI_STRING_MESSAGE_ITEM_TYPE_MISTERIOUS,				//"¾Ë¼ö¾ø´Â ¾ÆÀÌÅÛ",
	
	
	// Storage
	UI_STRING_MESSAGE_STORAGE_FIRST,					//"Ã¹¹øÂ° º¸°üÇÔ",
	UI_STRING_MESSAGE_STORAGE_SECOND,					//"µÎ¹øÂ° º¸°üÇÔ",
	UI_STRING_MESSAGE_STORAGE_THIRD,					//"¼¼¹øÂ° º¸°üÇÔ",
	UI_STRING_MESSAGE_BRING_MONEY_FROM_STORAGE,			//"µ·À» Ã£½À´Ï´Ù."

	// Char Manager
	UI_STRING_MESSAGE_CHAR_DELETE,						// »èÁ¦ÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_CHAR_CANCEL,						// Ãë¼ÒÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_CHAR_DELETE_CONFIRM,				// ÀÌ Ä³¸¯ÅÍ¸¦ Á¤¸» »èÁ¦ÇÏ½Ã°Ú½À´Ï±î?
	UI_STRING_MESSAGE_RE_INPUT_CORRECT_SSN,				//ÁÖ¹Îµî·Ï¹øÈ£¸¦ Á¤È®È÷ ÀÔ·ÂÇÏ¼¼¿ä.
	UI_STRING_MESSAGE_CHAR_MANAGER_NAME,					//"ÀÌ¸§ : ",
	UI_STRING_MESSAGE_CHAR_MANAGER_LEVEL,					//"·¹º§ : ",
	UI_STRING_MESSAGE_CHAR_MANAGER_STR_PURE,				//"¼ø¼öÈû : ",
	UI_STRING_MESSAGE_CHAR_MANAGER_DEX_PURE,				//"¼ø¼ö¹ÎÃ¸¼º : ",
	UI_STRING_MESSAGE_CHAR_MANAGER_INT_PURE,				//"¼ø¼öÁö½Ä : ",
	UI_STRING_MESSAGE_CHAR_MANAGER_SWORD_LEVEL,				//"°Ë µµ¸ÞÀÎ ·¹º§ : ",
	UI_STRING_MESSAGE_CHAR_MANAGER_BLADE_LEVEL,				//"µµ µµ¸ÞÀÎ ·¹º§ : ",
	UI_STRING_MESSAGE_CHAR_MANAGER_GUN_LEVEL,				//"ÃÑ µµ¸ÞÀÎ ·¹º§ : ",
	UI_STRING_MESSAGE_CHAR_MANAGER_HEAL_LEVEL,				//"È¸º¹ µµ¸ÞÀÎ ·¹º§ : ",
	UI_STRING_MESSAGE_CHAR_MANAGER_ENCHANT_LEVEL,			//"Ãàº¹ µµ¸ÞÀÎ ·¹º§ : ",
	UI_STRING_MESSAGE_CHAR_MANAGER_FAME,					//"¸í¼º : ",
	UI_STRING_MESSAGE_CHAR_MANAGER_CREATEMSG1,				//"Create¸¦ ´©¸£½Ã¸é »õ Ä³¸¯ÅÍ¸¦",
	UI_STRING_MESSAGE_CHAR_MANAGER_CREATEMSG2,				//"¸¸µå½Ç ¼ö ÀÖ½À´Ï´Ù.",
	UI_STRING_MESSAGE_CHAR_MANAGER_GRADE,					//= "°è±Þ : ";
	UI_STRING_MESSAGE_CHAR_MANAGER_GRADE_EXP,				// °è±Þ °æÇèÄ¡


	// Option Menu
	UI_STRING_MESSAGE_OPTION_MENU_ENTER_CHATTING,			//¿£ÅÍÃ¤ÆÃ
	UI_STRING_MESSAGE_OPTION_MENU_NORMAL_CHATTING,			//ÀÏ¹ÝÃ¤ÆÃ
	UI_STRING_MESSAGE_OPTION_MENU_3D_ACCEL,					//"3D °¡¼Ó »ç¿ëÇÏ±â",
	UI_STRING_MESSAGE_OPTION_MENU_ALPHA_HPBAR,				//"Åõ¸íÇÑ HP(MP)¹Ù",
	UI_STRING_MESSAGE_OPTION_MENU_SHED_BLOOD,				//"(HP 30% ÀÌÇÏÀÏ ¶§) ÇÇÈê¸®±â",
	UI_STRING_MESSAGE_OPTION_MENU_HIDE_SOFT,				//"Ã¢ ÀÚµ¿¼û±è ºÎµå·´°Ô",
	UI_STRING_MESSAGE_OPTION_MENU_GAME_BRIGHT,				//"°ÔÀÓ ¹à±â",
	UI_STRING_MESSAGE_OPTION_MENU_CHATTING_TALK,			//"Ã¤ÆÃ ¸»Ç³¼±À¸·Î º¸±â",
	UI_STRING_MESSAGE_OPTION_MENU_PUT_FPS,					//"FPSÃâ·Â",
	UI_STRING_MESSAGE_OPTION_MENU_WINDOW_ALPHA,				//"±âº»Ã¢ ¹ÝÅõ¸íÇÏ±â",
	UI_STRING_MESSAGE_OPTION_MENU_DENSITY_ALPHA,			//"¹ÝÅõ¸í ³óµµ",
	UI_STRING_MESSAGE_OPTION_MENU_SOUND_VOLUME,				//"È¿°úÀ½ Å©±â",
	UI_STRING_MESSAGE_OPTION_MENU_MUSIC_VOLUME,				//"¹è°æÀ½¾Ç Å©±â",
	UI_STRING_MESSAGE_OPTION_MENU_SHOW_BASIC_HELP,			//"ÃÊº¸ÀÚ¿ë µµ¿ò¸» º¸±â",
	UI_STRING_MESSAGE_OPTION_MENU_NO_LISTEN_BAD_TALK,		//"³ª»Û¸» µèÁö ¾Ê±â",
	UI_STRING_MESSAGE_OPTION_MENU_LOAD_ALL_IMAGE,			//"¸ÊÀÌµ¿ÇÒ ¶§ ¸ó½ºÅÍ ÀÌ¹ÌÁö ºÒ·¯¿À±â",
	UI_STRING_MESSAGE_OPTION_MENU_CHATTING_COLOR_WHITE,		//"Ã¤ÆÃ ¸ðµÎ Èò»öÀ¸·Î º¸±â",
	UI_STRING_MESSAGE_OPTION_MENU_RUN_TEEN_VERSION,			//"Æ¾¹öÁ¯À¸·Î º¸±â",
	UI_STRING_MESSAGE_OPTION_MENU_OPEN_WINDOW_WHEN_WHISPER,	//"±Ó¼Ó¸» ¿ÔÀ»¶§ Ã¤ÆÃÃ¢ ¶ç¿ì±â",
	UI_STRING_MESSAGE_OPTION_MENU_ACCEL_NAME,				//"±â´É : ",
	UI_STRING_MESSAGE_OPTION_MENU_ACCEL_KEY,				//"´ÜÃàÅ° : "
	UI_STRING_MESSAGE_OPTION_MENU_MSG1,						// "»õ·Î ÁöÁ¤ÇÒ ´ÜÃàÅ°¸¦ ÀÔ·ÂÇÏ¼¼¿ä.",
	UI_STRING_MESSAGE_OPTION_MENU_MSG2,						// "Ãë¼ÒÇÏ°í ½ÍÀ¸½Ã¸é ESCÅ°¸¦ ´©¸£¼¼¿ä.",
	UI_STRING_MESSAGE_OPTION_NOT_SEND_MY_INFO,				// "³» Ä³¸¯ÅÍ Á¤º¸ ºñ°ø°³"

	// Grade
	UI_STRING_MESSAGE_GRADE_PIVATE,								//	"ÇÇº£ÀÌÆ®";
	UI_STRING_MESSAGE_GRADE_SERENT,								//	"¼­·±Æ®";
	UI_STRING_MESSAGE_GRADE_FEACEL,								//	"ÇÇ¼¿";
	UI_STRING_MESSAGE_GRADE_LITENA,								//	"¸®Å×³ª";
	UI_STRING_MESSAGE_GRADE_KAINEL,								//	"ÄÉÀÌ³Î";
	UI_STRING_MESSAGE_GRADE_GENEAL,								//	"Áö´Ï¾ó";
	UI_STRING_MESSAGE_GRADE_FORE_GENEAL,						//	"Æ÷¾î Áö´Ï¾ó";
	UI_STRING_MESSAGE_GRADE_MAJORIS_GENEAL,						//	"¸ÞÀÌÀú¸®½º Áö´Ï¾ó";
	UI_STRING_MESSAGE_GRADE_CLOEL_GENEAL,						//	"Å¬·Î¿¤ Áö´Ï¾ó";
	UI_STRING_MESSAGE_GRADE_MARSHAL ,							//	"¸¶¼£";

	UI_STRING_MESSAGE_GRADE_RITTER,								//¸®ÅÍ(ÑÈÞÍ)
	UI_STRING_MESSAGE_GRADE_REICHSRITTER,						//¶óÈ÷½º¸®ÅÍ(òÁáÕÑÈÞÍ)
	UI_STRING_MESSAGE_GRADE_BARONET,							//¹Ù·Î³×Æ®(ðôÑûíÉ)
	UI_STRING_MESSAGE_GRADE_PREYHER,							//¶óÀÌÇì¸£(Ñû)
	UI_STRING_MESSAGE_GRADE_GRAF,								//±×¶óÇÁ(Û×)
	UI_STRING_MESSAGE_GRADE_MARKGRAF,							//¸¶¸£Å©±×¶óÇÁ(Ü«ÌÑÛ×)
	UI_STRING_MESSAGE_GRADE_PFALZGRAF,							//ÆÈÃ÷±×¶óÇÁ(ÏàñéÛ×)
	UI_STRING_MESSAGE_GRADE_FURST,								//Ç¶¸£½ºÆ®(ý¥)
	UI_STRING_MESSAGE_GRADE_HERZOG,								//Çì¸£ÃÊÅ©(Íë)
	UI_STRING_MESSAGE_GRADE_LANDESHER,							//¶õµ¥½ºÇì¸£(èÝ)



	// Client Info
	UI_STRING_MESSAGE_CLIENT_VERSION,							//Version
	UI_STRING_MESSAGE_NETMARBLE_CLIENT_VERSION,

	UI_STRING_MESSAGE_OTHER_INFO_STR_PURE,						// "¼ø¼ö   Èû   : "
	UI_STRING_MESSAGE_OTHER_INFO_DEX_PURE,						// "¼ø¼ö ¹ÎÃ¸¼º : "
	UI_STRING_MESSAGE_OTHER_INFO_INT_PURE,						// "¼ø¼ö  Áö´É  : "

	UI_STRING_MESSAGE_OTHER_INFO_STR_CUR,						// "ÇöÀç   Èû   : "
	UI_STRING_MESSAGE_OTHER_INFO_DEX_CUR,						// "ÇöÀç ¹ÎÃ¸¼º : "
	UI_STRING_MESSAGE_OTHER_INFO_INT_CUR,						// "ÇöÀç  Áö´É  : "
	UI_STRING_MESSAGE_OTHER_INFO_FAME,							// "¸í¼º";
	UI_STRING_MESSAGE_OTHER_INFO_TEAM_NAME,						// "ÆÀÀÌ¸§ : ";
	UI_STRING_MESSAGE_OTHER_INFO_CLAN_NAME,						// Å¬·£ ÀÌ¸§ : 

	UI_STRING_MESSAGE_TIP_CHANGE_PICTURE_CLICK_HERE,			// "ÀÚ±â »çÁøÀ» ¹Ù²Ü ¼ö ÀÖ½À´Ï´Ù";

	STRING_MESSAGE_UP_TO_GRADE,									// °è±ÞÀÌ ½Â±ÞÇß½À´Ï´Ù.
	UI_STRING_MESSAGE_GRADE_NAME,								// "°è±Þ ÀÌ¸§ :";
	UI_STRING_MESSAGE_GRADE_LEVEL,								// "°è±Þ ·¹º§ :";

	UI_STRING_MESSAGE_ENG_DOMAIN_BLADE2,						//"BLADE";
	UI_STRING_MESSAGE_ENG_DOMAIN_SWORD2,						//"SWORD";
	UI_STRING_MESSAGE_ENG_DOMAIN_ENCHANT2,						//"ENCHANT";
	UI_STRING_MESSAGE_ENG_DOMAIN_GUN2,							//"GUN";
	UI_STRING_MESSAGE_ENG_DOMAIN_HEAL2,							//"HEAL";

	UI_STRING_MESSAGE_DESC_CLASS2,								//"°è¿­";
	STRING_MESSAGE_ITEM_TO_ITEM_FAIL_NO_PREMIUM_SLAYER,			// ÇÁ¸®¹Ì¾ö »ç¿ëÀÚÀÌ¾î¾ß ÇÏ°Å³ª, ±æµå³»¿¡¼± »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_ITEM_TO_ITEM_FAIL_NO_PREMIUM_VAMPIRE,		// ÇÁ¸®¹Ì¾ö »ç¿ëÀÚÀÌ¾î¾ß ÇÏ°Å³ª, ¸¶À»³»¿¡¼± »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù.

	STRING_MESSAGE_DISMISS_AFTER_SECOND,							// "%dÃÊ ÈÄ¿¡ ÂÑ°Ü³³´Ï´Ù."
	UI_STRING_MESSAGE_MODIFY_INFO,							// ÆÀ ¼Ò°³ ¸»À» º¯°æÇÕ´Ï´Ù.
	
	UI_STRING_MESSAGE_START_TRACE,								// ÃßÀû ½ÃÀÛ
	UI_STRING_MESSAGE_CANCEL_TRACE,								// ÃßÀû Ãë¼Ò
	UI_STRING_MESSAGE_TRACE,									// %sÄ³¸¯ÅÍ¸¦ ÃßÀûÁßÀÔ´Ï´Ù.
	UI_STRING_MESSAGE_CANNOT_TRACE,								// %sÄ³¸¯ÅÍ¸¦ Ã£À» ¼ö ¾ø½À´Ï´Ù.

	UI_STRING_MESSAGE_OPTION_DO_NOT_WAR_MSG,					// ÀüÀï°ü·Ã ¸Þ½ÃÁö º¸Áö ¾Ê±â
	UI_STRING_MESSAGE_OPTION_DO_NOT_LAIR_MSG,					// ·¹¾î¸¶½ºÅÍ°ü·Ã ¸Þ½ÃÁö º¸Áö ¾Ê±â

	UI_STRING_MESSAGE_SHOW_GRADE1_INFO_WINDOW,					// °è±Þ ½ºÅ³Ã¢À» º¾´Ï´Ù.
	UI_STRING_MESSAGE_SHOW_GRADE2_INFO_WINDOW,					// °è±Þ ½ºÅ³Ã¢À» º¾´Ï´Ù.

	UI_STRING_MESSAGE_INCREASE_CRITICAL_10,						// Critical °ø°Ý·Â 10 Áõ°¡
	UI_STRING_MESSAGE_INCREASE_DEFENSE_5,						// Defense 5 Áõ°¡
	UI_STRING_MESSAGE_INCREASE_TOHIT_5,							// To hit 5 Áõ°¡
	UI_STRING_MESSAGE_INCREASE_HP_10,							// HP 10 Áõ°¡
	UI_STRING_MESSAGE_INCREASE_MP_15,							// MP 15 Áõ°¡
	UI_STRING_MESSAGE_INCREASE_DAMAGE_3,						// µ¥¹ÌÁö 3 Áõ°¡
	UI_STRING_MESSAGE_INCREASE_ATTACKSPEED_15,					// °ø°Ý¼Óµµ 15 Áõ°¡
	UI_STRING_MESSAGE_INCREASE_HP_20,							// HP 20 Áõ°¡
	UI_STRING_MESSAGE_INCREASE_DEFENSE_10,						// µðÆæ½º 10 Áõ°¡
	UI_STRING_MESSAGE_INCREASE_PROTECTION_10,					// Protection 10 Áõ°¡
	UI_STRING_MESSAGE_DECREASE_HP_EXHAUSTION_10_PERCENT,		// ½ºÅ³ »ç¿ë½Ã HP ¼Ò¸ð·® 10% °¨¼Ò
	UI_STRING_MESSAGE_INCREASE_ENCHANT_DAMAGE_10_PERCENT,		// Enchant °è¿­ °ø°Ý±â¼ú µ¥¹ÌÁö 10% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_HEAL_DAMAGE_10_PERCENT,			// Heal °è¿­ °ø°Ý±â¼ú µ¥¹ÌÁö 10% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_NEAR_ATTACK_DAMAGE_10_PERCENT,	// ±ÙÁ¢°ø°Ý°è¿­ ½ºÅ³ µ¥¹ÌÁö 10% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_HP_RESTORE_SPEED_15_PERCENT,		// HP È¸º¹¼Óµµ 15% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_RESISTANCE_ACID_15_PERCENT,		// Acid ÀúÇ× 15% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_RESISTANCE_BLOODY_15_PERCENT,	// Bloody ÀúÇ× 15% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_RESISTANCE_CURSE_15_PERCENT,		// Curse ÀúÇ× 15% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_RESISTANCE_POISON_15_PERCENT,	// Poison ÀúÇ× 15% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_DAMAGE_STORM_20_PERCENT,			// Storm °è¿­ ½ºÅ³ µ¥¹ÌÁö 20% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_RANGE_STORM_5_BY_5,				// Storm °è¿­ ½ºÅ³ ¹üÀ§ 5*5·Î Áõ°¡
	UI_STRING_MESSAGE_INCREASE_SUCCESS_RATIO_POISON_10_PERCENT,	// Poison °è¿­ ¼º°ø·ü 10% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_SUCCESS_RATIO_ACID_10_PERCENT,	// Acid °è¿­ ¼º°ø·ü 10% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_SUCCESS_RATIO_CURSE_10_PERCENT,	// Curse °è¿­ ¼º°ø·ü 10% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_SUCCESS_RATIO_BLOODY_10_PERCENT,	// Bloody °è¿­ ¼º°ø·ü 10% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_SUCCESS_RATIO_INNATE_10_PERCENT,	// Innate °è¿­ ¼º°ø·ü 10% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_SUCCESS_RATIO_SUMMON_10_PERCENT,	// Summon °è¿­ ¼º°ø·ü 10% Áõ°¡
	UI_STRING_MESSAGE_DECREASE_MP_EXHAUSTION_10_PERCENT,		// ½ºÅ³ »ç¿ë½Ã MP ¼Ò¸ð·® 10% °¨¼Ò
	UI_STRING_MESSAGE_INCREASE_HP_STEAL_2_PERCENT,				// HP ½ºÆ¿ 2% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_MP_STEAL_2_PERCENT,				// MP ½ºÆ¿ 2% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_LUCKY_2,							// Lucky 2 Áõ°¡
	UI_STRING_MESSAGE_INCREASE_KEEP_TIME_ACID_SWAMP_20_PERCENT,	// Acid Swamp Áö¼Ó½Ã°£ 20% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_KEEP_TIME_PARALYZE_20_PERCENT,	// Paralyze Áö¼Ó½Ã°£ 20% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_KEEP_TIME_DARKNESS_30_PERCENT,	// Darkness Áö¼Ó½Ã°£ 30% Áõ°¡
	UI_STRING_MESSAGE_INCREASE_RANGE_DARKNESS_5_BY_5,			// Darkness ¹üÀ§ 5*5·Î Áõ°¡
	UI_STRING_MESSAGE_CANNOT_UP_GRADE,							// ´õÀÌ»ó °è±ÞÀÌ ¿À¸£Áö ¾Ê½À´Ï´Ù.

	STRING_MESSAGE_SELECT_PC_CANNOT_PLAY,						// À¯·á°èÁ¤ÀÌ ¾Æ´Ï¶ó¼­ °ÔÀÓÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_SELECT_PC_NOT_BILLING_CHECK,					// ¾ÆÁ÷ °áÁ¦ Á¤º¸°¡ È®ÀÎµÇÁö ¾Ê¾Ò½À´Ï´Ù. Àá½Ã¸¸ ±â´Ù·ÁÁÖ¼¼¿ä.
	STRING_MESSAGE_SELECT_PC_CANNOT_PLAY_BY_ATTR,				// Ä³¸¯ÅÍ ´É·ÂÄ¡°¡ ¹«·á ¼­ºñ½º Á¦ÇÑ¿¡ µµ´ÞÇØ¼­ À¯·á ¼­ºñ½º¸¦ »ç¿ëÇÏ¼Å¾ß¸¸ Á¢¼ÓÇÒ ¼ö ÀÖ½À´Ï´Ù.

	UI_STRING_MESSAGE_NEVER_CANNOT_LEARN_SKILL,					// ¹è¿ï ¼ö ¾ø´Â ½ºÅ³ÀÔ´Ï´Ù.
	UI_STRING_MESSAGE_ALREADY_LEARNED_SKILL,					// ÀÌ¹Ì ¹è¿î ½ºÅ³ÀÔ´Ï´Ù.
	UI_STRING_MESSAGE_CANNOT_LEARN_SKILL_YET,					// ¾ÆÁ÷ ¹è¿ï ¼ö ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_CAN_LEARN_SKILL_NOW,						// Áö±Ý ¹è¿ï ¼ö ÀÖ½À´Ï´Ù.
	UI_STRING_MESSAGE_ACCELERATOR_GRADE1,
	UI_STRING_MESSAGE_ACCELERATOR_GRADE2,
	UI_STRING_MESSAGE_LEARN_GRADE_SKILL_CONFIRM,				// ÀÌ ½ºÅ³À» ¹è¿ì¸é ÇöÀç °è±ÞÀÇ ´Ù¸¥ °è±Þ ½ºÅ³À» ¹è¿ï ¼ö ¾ø½À´Ï´Ù. Á¤¸»·Î ¹è¿ì½Ã°Ú½À´Ï±î?

	UI_STRING_MESSAGE_TREE_OK,									// Æ®¸®¸¦ »ç¿ëÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_TREE_CANCEL,								// Æ®¸® »ç¿ëÀ» Ãë¼ÒÇÕ´Ï´Ù.

    STRING_MESSAGE_TRADE_GIFT_BOX_OK,             // ¼±¹° »óÀÚ ±³È¯ ¼º°ø
    STRING_MESSAGE_TRADE_GIFT_BOX_NO_ITEM,        // ¼±¹° »óÀÚ ¾ø´Ù
    STRING_MESSAGE_TRADE_GIFT_BOX_ALREADY_TRADE,  // ¼±¹° »óÀÚ¸¦ ÀÌ¹Ì ÇÑ¹ø ±³È¯Çß´Ù.

	STRING_MESSAGE_XMAS_TREE_CANNOT_USE,			// ÁÖÀ§ÀÇ ´Ù¸¥ Æ®¸®¿Í ³Ê¹« °¡±õ½À´Ï´Ù.
	STRING_MESSAGE_XMAS_CARD_CANNOT_USE,			// ºó Ç×¸ñÀÌ ÀÖ½À´Ï´Ù. ¸ðµç Ç×¸ñÀ» Ã¤¿öÁÖ¼¼¿ä.
	STRING_MESSAGE_NOT_USE_SAFETY_POSITION,			// ¾ÈÀüÁö´ë¿¡¼­´Â »ç¿ëÇÒ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_NOT_USE_SAFETY_ZONE,				// ¸¶À»¾ÈÀÌ³ª ±æµå¾È¿¡¼­´Â »ç¿ëÇÒ¼ö ¾ø½À´Ï´Ù.

	UI_STRING_MESSAGE_ENCHANT_CONFIRM_2,					//{  "½ÇÆÐ ÇÏ¸é ¾ÆÀÌÅÛÀÌ ºÎ¼­Áú ¼ö ÀÖ½À´Ï´Ù.

	UI_STRING_MESSAGE_DEPOSIT_LIMIT,				// º¸°üÇÔ¿¡ 20¾ï ÀÌ»ó º¸°üÇÒ ¼ö ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_WITHDRAW_LIMIT,				// 20¾ï ÀÌ»ó ¼ÒÁöÇÒ ¼ö ¾ø½À´Ï´Ù.

	// 2003.1.14
	STRING_MESSAGE_EVENT_PREMIUM_HALF_SLAYER,		// ÇöÀç Á¸¿¡¼­´Â ¾ÚÇÃÀ» Àý¹Ý °¡°ÝÀ¸·Î ±¸ÀÔÇÏ½Ç ¼ö ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_EVENT_PREMIUM_HALF_VAMPIRE,		// ÇöÀç Á¸¿¡¼­´Â Ç÷Ã»À» Àý¹Ý °¡°ÝÀ¸·Î ±¸ÀÔÇÏ½Ç ¼ö ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_EVENT_PREMIUM_HALF_SLAYER_END,	// ¾ÚÇÃÀ» Àý¹Ý °¡°ÝÀ¸·Î ±¸ÀÔÇÒ ¼ö ÀÖ´Â ÀÌº¥Æ®°¡ Á¾·áµÇ¾ú½À´Ï´Ù.
	STRING_MESSAGE_EVENT_PREMIUM_HALF_VAMPIRE_END,	// Ç÷Ã»À» Àý¹Ý °¡°ÝÀ¸·Î ±¸ÀÔÇÒ ¼ö ÀÖ´Â ÀÌº¥Æ®°¡ Á¾·áµÇ¾ú½À´Ï´Ù.

	STRING_MESSAGE_REWARD_OK,						// º¸»óÀ» ¹Þ¾Ò½À´Ï´Ù.
	STRING_MESSAGE_REWARD_FAIL,						// º¸»óÀ» ¹ÞÀ» ¼ö ¾ø½À´Ï´Ù.
    STRING_MESSAGE_NO_EMPTY_SLOT,					// ºó ÀÚ¸®°¡ ¾ø½À´Ï´Ù.

	UI_STRING_MESSAGE_OPTION_DO_NOT_HOLY_LAND_MSG,	// ¾Æ´ãÀÇ ¼ºÁö °ü·Ã ¸Þ½ÃÁö º¸Áö ¾Ê±â
	UI_STRING_MESSAGE_HOLY_LAND_TOTAL_FEE,			// ÃÑ ¼¼±Ý
	UI_STRING_MESSAGE_HOLY_LAND_CAN_BRING_FEE,		// °¡Á®¿Ã ¼¼±Ý
	UI_STRING_MESSAGE_HOLY_LAND_TOTAL_FEE_DESC,		// °¡Á®¿Ã ¼ö ÀÖ´Â ÃÑ ¼¼±Ý
	UI_STRING_MESSAGE_HOLY_LAND_INPUT_BRING_FEE,	// °¡Á®¿Ã ¼¼±ÝÀ» ÀÔ·ÂÇÏ¼¼¿ä
	UI_STRING_MESSAGE_HOLY_LAND_CLICK_INPUT_FEE,	// Å¬¸¯ÇÏ½Ã¸é °¡Á®¿Ã ¼¼±ÝÀ» ÀÔ·ÂÇÕ´Ï´Ù
	UI_STRING_MESSAGE_HOLY_LAND_OK,					// ÀÔ·ÂÇÑ ¼¼±ÝÀ» °¡Á®¿É´Ï´Ù
	
	STRING_MESSAGE_NO_TEAM,							// ÆÀ¿¡ ¼Ò¼ÓµÇ¾î ÀÖÁö ¾Ê½À´Ï´Ù.
	STRING_MESSAGE_NO_CLAN,							// Å¬·£¿¡ ¼Ò¼ÓµÇ¾î ÀÖÁö ¾Ê½À´Ï´Ù.
	STRING_MESSAGE_NOT_TEAM_MASTER,					// ÆÀ ¸¶½ºÅÍ°¡ ¾Æ´Õ´Ï´Ù.
	STRING_MESSAGE_NOT_CLAN_MASTER,					// Å¬·£ ¸¶½ºÅÍ°¡ ¾Æ´Õ´Ï´Ù.
	STRING_MESSAGE_TEAM_HAS_NO_CASTLE,				// ÆÀÀÌ °¡Áø ¼ºÀÌ ¾ø½À´Ï´Ù.
	STRING_MESSAGE_CLAN_HAS_NO_CASTLE,				// Å¬·£ÀÌ °¡Áø ¼ºÀÌ ¾ø½À´Ï´Ù.
	STRING_MESSAGE_TEAM_NOT_YOUR_CASTLE,			// ÆÀÀÌ ¼ÒÀ¯ÇÑ ¼ºÀÌ ¾Æ´Õ´Ï´Ù.
	STRING_MESSAGE_CLAN_NOT_YOUR_CASTLE,			// Å¬·£ÀÌ ¼ÒÀ¯ÇÑ ¼ºÀÌ ¾Æ´Õ´Ï´Ù.

	STRING_MESSAGE_SUCCESS_BRING_FEE,				// ¼¼±ÝÀ» Ã£¾Ò½À´Ï´Ù.
	STRING_MESSAGE_FAIL_BRING_FEE,					// ¼¼±ÝÀ» Ã£´Âµ¥ ½ÇÆÐÇÏ¿´½À´Ï´Ù.
	
	UI_STRING_MESSAGE_BRING_FEE_MSG,				//ÀÌ ¼ºÀ» ¼ÒÀ¯ÇÑ ÆÀÀÌ³ª Å¬·£ ¸¶½ºÅÍ´Â ÀÌ ¼º¿¡¼­ ¾òÀº ¼¼±ÝÀ» °¡Á®°¥ ¼ö ÀÖ½À´Ï´Ù.°¡Á®¿Ã ¼¼±ÝÀÇ ¿À¸¥ÂÊ¿¡ ÀÖ´Â ¹öÆ°À» ´­·¯ ÃÖ°í ÃÑ ¼¼±Ý¸¸Å­ °¡Á®°¥ ¼ö ÀÖ½À´Ï´Ù.ÀÚ½ÅÀÌ °¡Á®°¥ µ·°ú ÀÚ½ÅÀÌ °¡Áö°í ÀÖ´Â µ·ÀÇ ÇÕÀÌ 20¾ïÀ» ³ÑÀ» ¼ö ¾ø½À´Ï´Ù.

	UI_STRING_MESSAGE_BRING_FEE_LIMIT,				//°¡Á®°¥ ¼¼±Ý°ú ÇöÀç ¼ÒÁöÁßÀÎ µ·ÀÇ ÇÕÀÌ 20¾ï¿øÀ» ³ÑÀ» ¼ö ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_RACE_WAR,						// Á¾Á· ÀüÀï
	
	STRING_MESSAGE_WAR_SCHEDULE_FULL,						// ÀüÀï ½ºÄÉÁìÀÌ ²Ë Ã¡½À´Ï´Ù.
	STRING_MESSAGE_WAR_ALREADY_REGISTERED,					// ÀÌ¹Ì ÀüÀï ½ÅÃ»À» Çß½À´Ï´Ù.
	STRING_MESSAGE_WAR_NOT_ENOUGH_MONEY,					// µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù.
	STRING_MESSAGE_WAR_REGISTRATION_OK,						// ÀüÀï ½ºÄÉÁì¿¡ µî·Ï µÇ¾ú½À´Ï´Ù.

	STRING_MESSAGE_CANNOT_MOVE_SAFETY_ZONE_BLOOD_BIBLE,		// ÇÇÀÇ¼º¼­¸¦ µé°í ÀÖÀ¸¸é, ÀÚ±âÆíÀÇ ¾ÈÀüÁö´ë¿¡´Â µé¾î°¥ ¼ö ¾ø½À´Ï´Ù!
	STRING_MESSAGE_ALREADY_HAS_CASTLE,						//ÀÌ¹Ì ¼ºÀ» °¡Áö°í ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_WAR_UNAVAILABLE,							//Áö±ÝÀº ÀüÀï ½ÅÃ»ÀÌ µÇÁö ¾Ê½À´Ï´Ù.";

	UI_STRING_MESSAGE_STATUS_TIME_FORMAT,					// %d ½Ã %d ºÐ
	UI_STRING_MESSAGE_STATUS_LEFT_TIME,						// ³²Àº ½Ã°£
	UI_STRING_MESSAGE_GUILD_WAR,							// µ¿Á· ÀüÀï

	STRING_MESSAGE_BLOOD_BIBLE_BONUS_ARMEGA,				// ±â¼ú »ç¿ë½Ã MP(HP)¼Ò¸ð·® 50% °¨¼Ò  
	STRING_MESSAGE_BLOOD_BIBLE_BONUS_MIHOLE,				// Å¸Á¾Á·ÀÇ ¾ð¾î Åë¿ª  
	STRING_MESSAGE_BLOOD_BIBLE_BONUS_KIRO,					// INT+5, DEX +5  
	STRING_MESSAGE_BLOOD_BIBLE_BONUS_INI,					// ¹°¸®°ø°Ý µ¥¹ÌÁö +10  
	STRING_MESSAGE_BLOOD_BIBLE_BONUS_GREGORI,				// ¹°¾à°ª 50% ÇÒÀÎ  
	STRING_MESSAGE_BLOOD_BIBLE_BONUS_CONCILIA,				// HP +50  
	STRING_MESSAGE_BLOOD_BIBLE_BONUS_LEGIOS,				// ¸¶¹ý°ø°Ýµ¥¹ÌÁö +10  
	STRING_MESSAGE_BLOOD_BIBLE_BONUS_HILLEL,				// Çà¿î +10  
	STRING_MESSAGE_BLOOD_BIBLE_BONUS_JAVE,					// °×ºí°¡°Ý 50% ÇÒÀÎ  	
	STRING_MESSAGE_BLOOD_BIBLE_BONUS_NEMA,					// ½Ã¾ß +5, ¸ðµç ´É·ÂÄ¡ +3  
	STRING_MESSAGE_BLOOD_BIBLE_BONUS_AROSA,					// ¸ðµç ÀúÇ×·Â +9  
	STRING_MESSAGE_BLOOD_BIBLE_BONUS_CHASPA,				// STR +5, INT +5


	UI_STRING_MESSAGE_BLOOD_BIBLE_ARMEGA,				// ¾Æ¸£¸Þ°¡
	UI_STRING_MESSAGE_BLOOD_BIBLE_MIHOLE,				// ¹ÌÈ£·¹
	UI_STRING_MESSAGE_BLOOD_BIBLE_KIRO,					// Å°·Î
	UI_STRING_MESSAGE_BLOOD_BIBLE_INI,					// ¾ÆÀÌ´Ï
	UI_STRING_MESSAGE_BLOOD_BIBLE_GREGORI,				// ±×·¹°í¸®
	UI_STRING_MESSAGE_BLOOD_BIBLE_CONCILIA,				// ÄÜÄ¥¸®¾Æ
	UI_STRING_MESSAGE_BLOOD_BIBLE_LEGIOS,				// ·¹Áö¿À½º
	UI_STRING_MESSAGE_BLOOD_BIBLE_HILLEL,				// Èú¸±
	UI_STRING_MESSAGE_BLOOD_BIBLE_JAVE,					// Àðºê	
	UI_STRING_MESSAGE_BLOOD_BIBLE_NEMA,					// ³×¸¶
	UI_STRING_MESSAGE_BLOOD_BIBLE_AROSA,				// ¾Æ·Î»ç
	UI_STRING_MESSAGE_BLOOD_BIBLE_CHASPA,				// Â÷½ºÆÄ


	UI_STRING_MESSAGE_BLOOD_BIBLE_ARMEGA_ENG,				// ARMEGA
	UI_STRING_MESSAGE_BLOOD_BIBLE_MIHOLE_ENG,				// MIHOLE
	UI_STRING_MESSAGE_BLOOD_BIBLE_KIRO_ENG,					// KIRO
	UI_STRING_MESSAGE_BLOOD_BIBLE_INI_ENG,					// INI
	UI_STRING_MESSAGE_BLOOD_BIBLE_GREGORI_ENG,				// GREGORI
	UI_STRING_MESSAGE_BLOOD_BIBLE_CONCILIA_ENG,				// CONCILIA
	UI_STRING_MESSAGE_BLOOD_BIBLE_LEGIOS_ENG,				// LEGIOS
	UI_STRING_MESSAGE_BLOOD_BIBLE_HILLEL_ENG,				// HILLEL
	UI_STRING_MESSAGE_BLOOD_BIBLE_JAVE_ENG,					// JAVE
	UI_STRING_MESSAGE_BLOOD_BIBLE_NEMA_ENG,					// NEMA
	UI_STRING_MESSAGE_BLOOD_BIBLE_AROSA_ENG,				// AROSA
	UI_STRING_MESSAGE_BLOOD_BIBLE_CHASPA_ENG,				// CHASPA

	UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_DROP,				// ¶¥¿¡ ¶³¾îÁ® ÀÖ½À´Ï´Ù
	UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_HAS_SLAYER,		// ½½·¹ÀÌ¾î°¡ °¡Áö°í ÀÖ½À´Ï´Ù
	UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_HAS_VAMPIRE,		// ¹ìÆÄÀÌ¾î°¡ °¡Áö°í ÀÖ½À´Ï´Ù
	UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_NONE,				// Á¤º¸¸¦ ¾Ë ¼ö ¾ø½À´Ï´Ù
	UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_SLAYER,			// ½½·¹ÀÌ¾îÀÇ ¼öÈ£¼º´Ü¿¡ ÀÖ½À´Ï´Ù
	UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_VAMPIRE,			// ¹ìÆÄÀÌ¾îÀÇ ¼öÈ£¼º´Ü¿¡ ÀÖ½À´Ï´Ù
	
	UI_STRING_MESSAGE_BLOOD_BIBLE_DESC_POSITION,			// "À§Ä¡ : ";
	UI_STRING_MESSAGE_BLOOD_BIBLE_DESC_STATUS,				// "»óÅÂ : ";
	UI_STRING_MESSAGE_BLOOD_BIBLE_DESC_PLAYER,				// "µé°íÀÖ´Â Ä³¸¯ÅÍ : ";	

	UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_ATTACK_GUILD,		// °ø°Ý ±æµå
	UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_DEFENSE_GUILD,		// ¹æ¾î ±æµå
	
	STRING_MESSAGE_RACE_WAR_JOIN_FAILED,					// <»ç¿ëÀÚ> ´Ô ·¹º§´ëÀÇ Á¾Á· ÀüÀï ½ÅÃ»ÀÎ¿øÀÌ ²Ë Ã¡½À´Ï´Ù.
	STRING_MESSAGE_RACE_WAR_JOIN_OK,						// Á¾Á· ÀüÀï Âü°¡ ½ÅÃ»À» Çß½À´Ï´Ù.

	UI_STRING_MESSAGE_BLOOD_BIBLE_DESC_SHRINE_RACE,			// "ÃÖÁ¾ ¼ÒÀ¯ : ";

	STRING_MESSAGE_SLAYER,									// ½½·¹ÀÌ¾î
	STRING_MESSAGE_VAMPIRE,									// ¹ìÆÄÀÌ¾î

	STRING_MESSAGE_RACE_WAR_GO_FIRST_SERVER,				// Á¾Á· ÀüÀïÀº °¢ ¿ùµåÀÇ Ã¹¹øÂ° ¼­¹ö¿¡¼­¸¸ ½ÅÃ» ¹× Âü°¡°¡ °¡´ÉÇÕ´Ï´Ù.

	STRING_MESSAGE_GIVE_EVENT_ITEM_FAIL_NOW,				// Áö±ÝÀº ÀÌº¥Æ® ¾ÆÀÌÅÛÀ» ¹ÞÀ» ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_GIVE_EVENT_ITEM_FAIL,					// ÀÌº¥Æ® ¾ÆÀÌÅÛÀ» ¹ÞÀ» ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_GIVE_EVENT_ITEM_OK,						// ÀÌº¥Æ®¿¡ µû¸¥ ¾ÆÀÌÅÛÀ» ¹Þ¾Ò½À´Ï´Ù.
	STRING_MESSAGE_GIVE_PREMIUM_USER_ONLY,					// ÇÁ¸®¹Ì¾ö¼­ºñ½º »ç¿ëÀÚ¸¸ ¹ÞÀ» ¼ö ÀÖ½À´Ï´Ù.

	STRING_MESSAGE_COUPLE_MEET_SUCCESS,						// Ä¿ÇÃÀÌ ¼º»çµÇ¾ú½À´Ï´Ù.
	STRING_MESSAGE_COUPLE_CANNOT_MEET,						// Ä¿ÇÃÀÌ ¼º»çµÉ ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_MEET_WAIT_TIME_EXPIRED,					// ½Ã°£ÀÌ Áö³ª¼­ ½ÅÃ»ÀÌ Ãë¼ÒµÇ¾ú½À´Ï´Ù.

	STRING_MESSAGE_COUPLE_APART_SUCCESS,					// Çì¾îÁ³½À´Ï´Ù.
	STRING_MESSAGE_NOT_COUPLE,								// Ä¿ÇÃÀÌ ¾Æ´Ï¶ó¼­ Çì¾îÁú ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_APART_WAIT_TIME_EXPIRED,					// ½Ã°£ÀÌ Áö³ª¼­ ½ÅÃ»ÀÌ Ãë¼ÒµÇ¾ú½À´Ï´Ù.

	UI_STRING_MESSAGE_HOPE_COUPLE_MSG,						//Ä¿ÇÃÀº ¼­·Î°£ÀÇ ÇÕÀÇ ÇÏ¿¡ ÀÌ·ç¾îÁ®¾ß ÇÕ´Ï´Ù. ½ÅÃ» ¹æ¹ýÀº, Ä¿ÇÃÀÌ µÇ±æ ¿øÇÏ´Â ¿¬ÀÎÀÇ ÀÌ¸§À» ÀÔ·ÂÇÑ ÈÄ ½ÅÃ» ¹ÞÀº ¿¬ÀÎÀÌ 1ºÐ ¾È¿¡ Àú¿¡°Ô ¿Í¼­ µ¿ÀÇ ÇÏ½Ã¸é, Ä¿ÇÃÀÌ ¼º¸³µË´Ï´Ù. ÀÌ »çÇ×µéÀ» ±â¾ïÇÏ½Ê½Ã¿À.    ,  ±×·³, ¿¬ÀÎÀÌ µÇ±æ ¿øÇÏ½Ã´Â ºÐÀÇ ÀÌ¸§À» ÀÔ·ÂÇÏ¿© ÁÖ½Ê½Ã¿À.
	UI_STRING_MESSAGE_BREAK_UP_COUPLE_MSG,					//ÇÕÀÇ °áº°Àº ¼­·Î°£ÀÇ ÇÕÀÇ ÇÏ¿¡ ÀÌ·ç¾î Á®¾ß ÇÏ¸ç, ÇöÀç Çì¾îÁö°íÀÚ ÇÏ´Â ¿¬ÀÎÀÇ ÀÌ¸§À» ÀÔ·ÂÇÑ ÈÄ ¿¬ÀÎÀÌ 1ºÐ ¾È¿¡ Àú¿¡°Ô ¿Í¼­ µ¿ÀÇ ÇÏ½Ã¸é, °áº°ÀÌ ¼º¸³µË´Ï´Ù. Çì¾îÁü°ú ÇÔ²² µÎ »ç¶÷ÀÇ »ç¶ûÀÇ ÁõÇ¥ÀÎ Ä¿ÇÃ¸µÀº »ç¶óÁý´Ï´Ù.  ÀÌ »çÇ×µéÀ» ±â¾ïÇÏ½Ê½Ã¿À. ±×·³, ¿¬ÀÎÀÇ ÀÌ¸§À» ÀÔ·ÂÇÏ¿© ÁÖ½Ê½Ã¿À.
	UI_STRING_MESSAGE_COMPLETE,						//¿Ï·á
	UI_STRING_MESSAGE_OTHER_PLAYER_NAME,					// »ó´ë¹æÀÇ Ä³¸¯ÅÍ ÀÌ¸§

	UI_STRING_MESSAGE_FORCE_BREAK_UP_COUPLE,				// ÀÏ¹æÀûÀÎ Çì¾îÁüÀº »ó´ë¹æÀÇ µ¿ÀÇ ¾øÀÌ °áº°ÀÌ °¡´ÉÇÏ³ª, ÀÚ½ÅÀÇ ¼ºÇâ¿¡¼­ »ó´ç·®ÀÌ °¨¼ÒÇÕ´Ï´Ù. ÇöÀç ÀÚ½ÅÀÇ ¿¬ÀÎÀÇ ÀÌ¸§À» ÀÔ·ÂÇÏ½Ã¸é, °áº°ÀÌ ¼º¸³µË´Ï´Ù. °áº° ¼º¸³°ú ÇÔ²² µÎ »ç¶÷ÀÇ »ç¶ûÀÇ ÁõÇ¥ÀÎ Ä¿ÇÃ¸µÀº »ç¶óÁý´Ï´Ù.  ÀÌ »çÇ×µéÀ» ±â¾ïÇÏ½Ê½Ã¿À.    ,  ±×·³, ¿¬ÀÎÀÇ ÀÌ¸§À» ÀÔ·ÂÇÏ¿© ÁÖ½Ê½Ã¿À.

	STRING_MESSAGE_COUPLE,									// Ä¿ÇÃ
	STRING_MESSAGE_MOVE_DELAY_SEC,							// %d ÃÊ ÈÄ¿¡ ÀÌµ¿ÇÕ´Ï´Ù.
	STRING_MESSAGE_COUPLE_MOVE_START,						// Ä¿ÇÃ Ä³¸¯ÅÍ°¡ ¾îµð¿¡ ÀÖ´ÂÁö Ã£°í ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_COUPLE_CAN_NOT_FIND,						// Ä¿ÇÃ Ä³¸¯ÅÍ¸¦ Ã£À» ¼ö ¾ø½À´Ï´Ù.

	STRING_MESSAGE_COUPLE_NOT_EVENT_TERM,			// Ä¿ÇÃ ÀÌº¥Æ® ±â°£ÀÌ ¾Æ´Õ´Ï´Ù.
	STRING_MESSAGE_COUPLE_ALREADY_WAITING,				// ÀÌ¹Ì »ó´ë¸¦ ±â´Ù¸®°í ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_COUPLE_LOGOFF,						// »ó´ë¹æÀÌ ·Î±×ÀÎ ÇÏÁö ¾Ê¾Ò½À´Ï´Ù.
	STRING_MESSAGE_COUPLE_DIFFERENT_RACE,				// ¼­·Î ´Ù¸¥ Á¾Á·ÀÔ´Ï´Ù.
	STRING_MESSAGE_COUPLE_SAME_SEX,					// ³²³à »çÀÌ¿¡¸¸ Ä¿ÇÃÀÌ ¼º¸³µË´Ï´Ù.
	STRING_MESSAGE_COUPLE_NOT_PAY_PLAYER,				// À¯·á »ç¿ëÀÚ°¡ ¾Æ´Õ´Ï´Ù.
	STRING_MESSAGE_COUPLE_ALREADY_COUPLE,				// ÀÌ¹Ì Ä¿ÇÃÀÔ´Ï´Ù.
	STRING_MESSAGE_COUPLE_WAS_COUPLE,					// Ä¿ÇÃÀ» ¸ÎÀº ÀûÀÌ ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_COUPLE_NOT_ENOUGH_GOLD,				// ¼ÒÁö±ÝÀÌ ºÎÁ·ÇÕ´Ï´Ù.
	STRING_MESSAGE_COUPLE_NOT_ENOUGH_ATTR,				// ´É·ÂÄ¡°¡ ºÎÁ·ÇÕ´Ï´Ù.
	STRING_MESSAGE_COUPLE_NOT_ENOUGH_LEVEL,			// ·¹º§ÀÌ ºÎÁ·ÇÕ´Ï´Ù.
	STRING_MESSAGE_COUPLE_INVENTORY_FULL,				// Ä¿ÇÃ¸µÀ» ¹ÞÀ» °ø°£ÀÌ ºÎÁ·ÇÕ´Ï´Ù.
	STRING_MESSAGE_COUPLE_NO_WAITING,					// ´ç½ÅÀ» ±â´Ù¸®´Â ÆÄÆ®³Ê°¡ ¾ø½À´Ï´Ù.
	STRING_MESSAGE_COUPLE_NOT_COUPLE,					// Ä¿ÇÃÀÌ ¾Æ´Õ´Ï´Ù.

	UI_STRING_MESSAGE_LOVE_CHAIN,						// Ä¿ÇÃÀÌ ÀÖ´Â°÷À¸·Î ÀÌµ¿ÇÒ ¼ö ÀÖ´Â ½ºÅ³ÀÔ´Ï´Ù.
	UI_STRING_MESSAGE_WILL_YOU_GO_BILING_PAGE,			// °áÁ¦ ÆäÀÌÁö·Î ÀÌµ¿ÇÏ½Ã°Ú½À´Ï±î?
	UI_STRING_MESSAGE_GO_BILING_PAGE,					// °áÁ¦ ÆäÀÌÁö·Î ÀÌµ¿ÇÕ´Ï´Ù.
	STRING_MESSAGE_LOGOUT_BY_PAYTIME,					// À¯·á »ç¿ë ½Ã°£ÀÌ ¸¸·áµÇ¾ú½À´Ï´Ù.
	STRING_MESSAGE_LOGOUT_BY_FREEPLAY_LEVEL,			// ´É·ÂÄ¡°¡ ¹«·á ¼­ºñ½º Á¦ÇÑ¿¡ µµ´ÞÇß½À´Ï´Ù.
	STRING_MESSAGE_LOGOUT_BY_LEVEL,						// ·¹º§ÀÌ ¹«·á ¼­ºñ½º Á¦ÇÑ¿¡ µµ´ÞÇß½À´Ï´Ù.
	
	UI_STRING_MESSAGE_CANNOT_UP_LEVEL_BY_FAME,			// ¸í¼ºÀÌ ³·¾Æ ·¹º§¾÷À» ÇÒ ¼ö ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_NEED_FAME,						// ÇÊ¿ä ¸í¼ºÄ¡

	
	UI_STRING_MESSAGE_QUEST_HAN,						// Äù½ºÆ®
	UI_STRING_MESSAGE_QUEST_ENG,						// Quest
	UI_STRING_MESSAGE_DAY,								// %dÀÏ
	UI_STRING_MESSAGE_HOUR,								// %d½Ã°£
	UI_STRING_MESSAGE_MINUTE,							// %dºÐ
	UI_STRING_MESSAGE_SECOND,							// %dÃÊ
	UI_STRING_MESSAGE_LEFT_TIME,						// ³²Àº »ç¿ë ½Ã°£ :
	UI_STRING_MESSAGE_EXPIRED_ITEM,						// ·Î±×¾Æ¿ôÇÏ½Ã¸é »ç¶óÁý´Ï´Ù.
	
	UI_STRING_MESSAGE_SELECT_QUEST_SLAYER,				// ½½·¹ÀÌ¾î Äù½ºÆ® ¼±ÅÃ ¸Þ½ÃÁö
	UI_STRING_MESSAGE_SELECT_QUEST_VAMPIRE,				// ¹ìÆÄÀÌ¾î Äù½ºÆ® ¼±ÅÃ ¸Þ½ÃÁö
	UI_STRING_MESSAGE_CANCEL_SELECT_QUEST,				// ´ÙÀ½ ±âÈ¸¿¡ Âü¿©ÇÏÁö.
	UI_STRING_MESSAGE_CANCEL_LEARN_SKILL,				// ´ÙÀ½¿¡ ¹è¿ì°Ú½À´Ï´Ù.
	UI_STRING_MESSAGE_NUMBER_OF_ANIMALS,				// ¸¶¸®-_-a

	UI_STRING_MESSAGE_EXPIRED_TIME_MONSTER_KILL_QUEST,			// ¸ó½ºÅÍ »ç³É Äù½ºÆ®ÀÇ ½Ã°£ÀÌ Á¾·áµÇ¾ú½À´Ï´Ù.
	UI_STRING_MESSAGE_FINISH_MONSTER_KILL_QUEST,				// ¸ñÀû ¸ó½ºÅÍ¸¦ ¸ðµÎ »ç³ÉÇÏ¿´½À´Ï´Ù.
	UI_STRING_MESSAGE_CAN_REQUITAL_FROM_NPC,					// NPC¿¡°Ô º¸»óÀ» ¹ÞÀ» ¼ö ÀÖ½À´Ï´Ù.
	UI_STRING_MESSAGE_START_MONSTER_KILL_QUEST,					// ¸ó½ºÅÍ »ç³É Äù½ºÆ®°¡ ½ÃÀÛµÇ¾ú½À´Ï´Ù.
	UI_STRING_MESSAGE_FAIL_ALREADY_START_MONSTER_KILL_QUEST,	// ÀÌ¹Ì Äù½ºÆ®°¡ ÁøÇàÁßÀÔ´Ï´Ù.
	UI_STRING_MESSAGE_FAIL_MONSTER_KILL_QUEST_BY_STATUS,		// ´É·ÂÄ¡°¡ ¸ÂÁö ¾Ê¾Æ Äù½ºÆ®¸¦ ÁøÇàÇÒ¼ö ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_SUCCESS_MONSTER_KILL_QUEST,				// ¸ó½ºÅÍ »ç³É Äù½ºÆ®¸¦ ¼º°øÇÏ¿´½À´Ï´Ù.
	UI_STRING_MESSAGE_NOT_YET_COMPLETE_MONSTER_KILL_QUEST,		// ¸ó½ºÅÍ »ç³É Äù½ºÆ®°¡ ¾ÆÁ÷ ¿Ï·áµÇÁö ¾Ê¾Ò½À´Ï´Ù.
	UI_STRING_MESSAGE_INVENTORY_FULL_MONSTER_KILL_QUEST,		// ÀÎº¥Åä¸®¿¡ °ø°£ÀÌ ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_NOT_IN_QUEST,								// Äù½ºÆ®ÁßÀÌ ¾Æ´Õ´Ï´Ù.
	UI_STRING_MESSAGE_FAIL_QUEST_EXPIRED_TIME,					// Á¦ÇÑ½Ã°£ÀÌ ÃÊ°úµÇ¾î Äù½ºÆ®¸¦ ½ÇÆÐÇß½À´Ï´Ù.	
	UI_STRING_MESSAGE_TIME_LIMIT,								// Á¦ÇÑ½Ã°£ : 
	UI_STRING_MESSAGE_CANNOT_APPLY_QUEST,						// Áö±ÝÀº Äù½ºÆ®¸¦ ½ÅÃ»ÇÒ ¼ö ¾ø½À´Ï´Ù.
	
	UI_STRING_MESSAGE_COMPLETE_QUEST,
	UI_STRING_MESSAGE_FAIL_TIME_OVER_QUEST,
	UI_STRING_MESSAGE_MEET_NPC,
	
	UI_STRING_MESSAGE_FAIL_INVALID_NPC,
	UI_STRING_MESSAGE_FAIL_BUG,
	
	UI_STRING_MESSAGE_GATHER_ITEM,								// % Ã£±â
	STRING_MESSAGE_MONSTER_KILL_QUEST_STRING_SET,				// ±×·³ %s %d¸¶¸®¸¦ %s ³»¿¡ Àâ¾Æ¿Àµµ·Ï ÇÏ°Ô.
	STRING_MESSAGE_MONSTER_KILL_QUEST_STRING_SET_VAMPIRE,		// ±×·³ %s %d¸¶¸®¸¦ %s ³»¿¡ Àâ¾Æ¿Àµµ·Ï ÇÏ½Ê½Ã¿À.
	STRING_MESSAGE_CANCEL_MONSTER_KILL_QUEST,					// Àåºñ¸¦ Á¡°ËÇÑ ÈÄ¿¡ ´Ù½Ã µ¹¾Æ¿À°Ú½À´Ï´Ù.
	STRING_MESSAGE_YES_I_SEE,									// ¿¹, ¾Ë°Ú½À´Ï´Ù.

	
	UI_STRING_MESSAGE_QUEST_MONSTER_KILL,						// ¸ó½ºÅÍ »ç³É Äù½ºÆ®
	UI_STRING_MESSAGE_QUEST_MEET_NPC,							// NPC ¸¸³ª±â Äù½ºÆ®
	UI_STRING_MESSAGE_QUEST_GATHER_ITEM,						// ¾ÆÀÌÅÛ ±¸ÇÏ±â Äù½ºÆ®
	
	UI_STRING_MESSAGE_QUEST_DESCRIPTION_TIME_TOTAL,				// ÃÑ Á¦ÇÑ½Ã°£ :
	UI_STRING_MESSAGE_QUEST_DESCRIPTION_TIME_ELAPSE,			// °æ °ú ½Ã °£ :
	UI_STRING_MESSAGE_QUEST_DESCRIPTION_TIME_REMAIN,			// ³² Àº ½Ã °£ :
	UI_STRING_MESSAGE_QUEST_DESCRIPTION_TIME_NO_REMAIN,			// ¾øÀ½
	UI_STRING_MESSAGE_FAIL_QUEST,								// Äù½ºÆ® ½ÇÆÐ
	UI_STRING_MESSAGE_SELECT_EVENT_GIFT,						// %d´Ü°è ÀÌº¥Æ® »óÇ°Áß ÇÏ³ª¸¦ °í¸£¼¼¿ä
	UI_STRING_MESSAGE_PLEASE_SCRATCH_IMAGE,						// »ó´ÜÀÇ ÀÌ¹ÌÁö¸¦ ±Ü¾î ÁÖ¼¼¿ä.
	UI_STRING_MESSAGE_WIN_A_PRIZE_SAME_IMAGE,					//"°°Àº ±×¸²ÀÌ 3°³¸é ´çÃ·ÀÔ´Ï´Ù."
	UI_STRING_MESSAGE_CONGRATULATIONS,							// ÃàÇÏÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_WIN_A_PRIZE,								// ¿¡ ´çÃ· µÇ¼Ì½À´Ï´Ù.

	STRING_MESSAGE_MEET_NPC_SLAYER,								// ¾Æ¿ì½ºÅÍÁî·Î ÃßÃøµÇ´Â ±â¹¦ÇÑ »ý±è»õÀÇ »ç¶÷À» º¸¾Ò´Ù´Â ÁÖ¹ÎÀº ¿¡½½¶ó´Ï¾Æ ºÎ±Ù¿¡ °ÅÁÖÇÏ´Â °ÍÀ¸·Î º¸°íµÇ¾ú´Ù. ¾Ë°ÚÀ¸¸é Áö±Ý ´çÀå ¸ñ°ÝÀÚ¸¦ Ã£¾Æ ÃÖ´ëÇÑ ¸¹Àº Á¤º¸¸¦ ¾ò¾î¿Àµµ·Ï.
	STRING_MESSAGE_MEET_NPC_VAMPIRE,							// ¾Æ¿ì½ºÅÍÁî·Î ÃßÃøµÇ´Â ±â¹¦ÇÑ »ý±è»õÀÇ »ç¶÷À» º¸¾Ò´Ù´Â ÁÖ¹Î µÑÀÌ ¸²º¸¼º ºÎ±ÙÀ» °Ìµµ ¾øÀÌ µ¹¾Æ´Ù´Ï°í ÀÖ´Ù°í ÇÏ³×¿ä. Áö±Ý ´çÀå ±×µéÀ» Ã£¾Æ ÃÖ´ëÇÑ ¸¹Àº Á¤º¸¸¦ ¾ò¾î¿Àµµ·Ï ÇÏ¼¼¿ä.

	STRING_MESSAGE_CANCEL_QUEST_VAMPIRE,						// ¸¶·ÂÀ» º¸ÃæÇÑ ÈÄ ´Ù½Ã µ¹¾Æ¿À°Ú½À´Ï´Ù.

	UI_STRING_MESSAGE_FAIL_LOTTERY,
	UI_STRING_MESSAGE_WIN_A_PRIZE2,								// »óÇ°¿¡ ´çÃ· µÇ¼Ì½À´Ï´Ù.
	UI_STRING_MESSAGE_USER_1,									// ´Ô²²¼­

	STRING_MESSAGE_RIPATY_SCRIPT_1,
	STRING_MESSAGE_RIPATY_SCRIPT_2,
	STRING_MESSAGE_RIPATY_SCRIPT_3,
	STRING_MESSAGE_RIPATY_SCRIPT_4,
	STRING_MESSAGE_RIPATY_SCRIPT_5,
	STRING_MESSAGE_RIPATY_SCRIPT_6,
	STRING_MESSAGE_RIPATY_SCRIPT_7,
	STRING_MESSAGE_RIPATY_SCRIPT_8,
	STRING_MESSAGE_RIPATY_SCRIPT_9,

	STRING_MESSAGE_AMATA_SCRIPT_1,
	STRING_MESSAGE_AMATA_SCRIPT_2,
	STRING_MESSAGE_AMATA_SCRIPT_3,
	STRING_MESSAGE_AMATA_SCRIPT_4,
	STRING_MESSAGE_AMATA_SCRIPT_5,
	STRING_MESSAGE_AMATA_SCRIPT_6,
	STRING_MESSAGE_AMATA_SCRIPT_7,
	STRING_MESSAGE_AMATA_SCRIPT_8,
	STRING_MESSAGE_AMATA_SCRIPT_9,
	
	UI_STRING_MESSAGE_QUEST_NAME_FIND_ANCIENT_DOCUMENT,					// °í´ë ¹®Çå Ã£±â Äù½ºÆ®
	UI_STRING_MESSAGE_QUEST_NAME_FIND_ANCIENT_MAP,						// °í´ë Áöµµ Ã£±â Äù½ºÆ®
	UI_STRING_MESSAGE_QUEST_NAME_FIND_SOUL_STONE,						// Á¤·É¼® Ã£±â Äù½ºÆ®

	UI_STRING_MESSAGE_NOT_IN_QUEST2,							// ÇöÀç ÁøÇàÁßÀÎ Äù½ºÆ®°¡ ¾ø½À´Ï´Ù.
//	UI_STRING_MESSAGE_EVENT_QUEST_2,
//	UI_STRING_MESSAGE_EVENT_QUEST_3,
//	UI_STRING_MESSAGE_EVENT_QUEST_4,
//	UI_STRING_MESSAGE_EVENT_QUEST_5,
	
	UI_STRING_MESSAGE_TAKE_OUT_OK,							// ¾ÆÀÌÅÛÀ» ÀÎº¥Åä¸®·Î °¡Á®¿Ô½À´Ï´Ù.
	UI_STRING_MESSAGE_TAKE_OUT_FAIL,						// ¾ÆÀÌÅÛÀ» ÀÎº¥Åä¸®·Î °¡Á®¿ÀÁö ¸øÇß½À´Ï´Ù.	
	STRING_MESSAGE_CANNOT_TAKE_OUT_ITEM_FROM_SHOP,			// Áö±ÝÀº ±¸ÀÔÇÑ ¾ÆÀÌÅÛÀ» Ã£À» ¼ö ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_CONFIRM_SELECT_ITEM_FROM_SHOP,		// ¼±ÅÃÇÏ½Å ¾ÆÀÌÅÛÀ» °¡Á®¿À½Ã°Ú½À´Ï±î?
	UI_STRING_MESSAGE_ITEM_SHOP,							// ¾ÆÀÌÅÛ ÀÌ¸§À» Å¬¸¯ÇÏ½Ã¸é ¼±ÅÃÇÑ ¾ÆÀÌÅÛÀ» °¡Á®¿É´Ï´Ù.

	STRING_MESSAGE_CLEAR_RANK_BONUS_OK,           // ¼±ÅÃÇÏ½Å °è±ÞÀÇ ½ºÅ³ÀÌ »èÁ¦ µÇ¾ú½À´Ï´Ù.
    STRING_MESSAGE_NO_RANK_BONUS,                 // ÇØ´ç ÀÚ°ÝÀÌ ¾ø½À´Ï´Ù.
    STRING_MESSAGE_ALREADY_CLEAR_RANK_BONUS,      // ¼±ÅÃÇÏ½Å °è±Þ ½ºÅ³À» »èÁ¦ÇÑ ÀûÀÌ ÀÖ½À´Ï´Ù.

	UI_STRING_MESSAGE_BULLETIN_BOARD_OK,					// °Ô½ÃÆÇÀ» »ç¿ëÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_BULLETIN_BOARD_CANCEL,				// °Ô½ÃÆÇ »ç¿ëÀ» Ãë¼ÒÇÕ´Ï´Ù.

	STRING_MESSAGE_BULLETIN_BOARD_CANNOT_USE,				// = "ÁÖÀ§ÀÇ ´Ù¸¥ Æ®¸®¿Í ³Ê¹« °¡±õ½À´Ï´Ù.";
	UI_STRING_MESSAGE_CAN_TRANS,
	
	UI_STRING_MESSAGE_TRANS_ITEM,
	UI_STRING_MESSAGE_RESURRECT_BY_ELIXIR,
	UI_STRING_MESSAGE_RESURRECT_BY_SCROLL,
	UI_STRING_CANNOT_USE,

	UI_STRING_MESSAGE_MIXING_FORGE_OK,							//¹Í½ÌÆ÷Áö¸¦ »ç¿ëÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_MIXING_FORGE_CANCEL,						//¹Í½ÌÆ÷Áö »ç¿ëÀ» Ãë¼ÒÇÕ´Ï´Ù.

	UI_STRING_MESSAGE_COMPLETE_MERGE_ITEM,						//µÎ ¾ÆÀÌÅÛÀÌ ¼º°øÀûÀ¸·Î ÇÕÃÄÁ³½À´Ï´Ù.
	UI_STRING_MESSAGE_FAIL_MERGE_ITEM,							//¾ÆÀÌÅÛÀ» ÇÕÄ¡´Âµ¥ ½ÇÆÐÇÏ¿´½À´Ï´Ù.
	STRING_MESSAGE_USE_GUILD_MEMBER_ONLY,						//¼ºÀ» ¼ÒÀ¯ÇÑ ±æµå¿ø¸¸ »ç¿ë °¡´ÉÇÕ´Ï´Ù.
	STRING_MESSAGE_CANNOT_USE_RIDE_MOTORCYCLE,					//¿ÀÅä¹ÙÀÌ¸¦ Å¸°í »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_CANNOT_MIXING_SPECIAL_ITEM,				//À¯´ÏÅ© ¾ÆÀÌÅÛÀÌ³ª ¸®¹ÌÆ¼µå ¾ÆÀÌÅÛÀº ¹Í½ÌÇÒ ¼ö ¾ø½À´Ï´Ù.

	UI_STRING_MESSAGE_CANNOT_MIXING_OPTION_COUNT,				//¿É¼ÇÀÌ ÇÑ°³ÀÎ ¾ÆÀÌÅÛ¸¸ ¹Í½ÌÇÒ ¼ö ÀÖ½À´Ï´Ù.
	UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_CLASS,					//¾ÆÀÌÅÛ Á¾·ù°¡ ´Þ¶ó¼­ ¹Í½ÌÇÒ ¼ö ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_TYPE,					//¾ÆÀÌÅÛ Å¸ÀÔÀÌ ´Þ¶ó¼­ ¹Í½ÌÇÒ ¼ö ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_OPTION,				//µÎ ¾ÆÀÌÅÛÀÇ ¿É¼ÇÀÌ °°¾Æ¼­ ¼±ÅÃÇÒ ¼ö ¾ø½À´Ï´Ù.
	
	UI_STRING_MESSAGE_WHAT_OPTION_REMOVE,						//¾î¶² ¿É¼ÇÀ» Á¦°ÅÇÏ½Ã°Ú½À´Ï±î?";
	UI_STRING_MESSAGE_CONFIRM_REMOVE_OPTION,					//¼±ÅÃÇÏ½Å ¿É¼ÇÀ» Á¤¸»·Î Á¦°ÅÇÏ½Ã°Ú½À´Ï±î?";
	UI_STRING_MESSAGE_CAN_REMOVE_OPTION,						//¾ÆÀÌÅÛÀÇ ¿É¼ÇÀ» Á¦°ÅÇÒ ¼ö ÀÖ½À´Ï´Ù.";
	UI_STRING_MESSGAE_CANNOT_REMOVE_OPTION,						//¾ÆÀÌÅÛÀÇ ¿É¼ÇÀ» Á¦°ÅÇÒ ¼ö ¾ø½À´Ï´Ù.";

	UI_STRING_MESSAGE_SELECT_OPTION,							//s ¿É¼ÇÀ»";
	UI_STRING_MESSAGE_QUESTION_REMOVE_OPTION,					//Á¦°Å ÇÏ½Ã°Ú½À´Ï±î?";
	STRING_MESSAGE_FAILED_REMOVE_OPTION,						//¿É¼ÇÀ» Áö¿ì´Âµ¥ ½ÇÆÐÇÏ¿´½À´Ï´Ù.";
	STRING_MESSAGE_SUCCESS_REMOVE_OPTION,						//¼±ÅÃÇÑ ¿É¼ÇÀÌ Á¦°ÅµÇ¾ú½À´Ï´Ù.";

	UI_STRING_MESSAGE_DESC_OTHER_TRIBE,					// Å¸Á¾Á· Àü¿ë
	UI_STRING_MESSAGE_CONSUME_EP,						// EP¼Ò¸ð·® : 

	UI_STRING_MESSAGE_EQUIP_OUSTERS_CIRCLET,			//	¼­Å¬¸´À» ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_OUSTERS_COAT,				//	¿ÊÀ» ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_OUSTERS_WEAPON,				//	Ã­Å©¶÷/¸®½ºÆ²¸´À» ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_OUSTERS_BOOTS,				//	ºÎÃ÷¸¦ ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_OUSTERS_ARMSBAND,			//	¾Ï½º¹êµåÀ» ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_OUSTERS_RING,				//	¹ÝÁö¸¦ ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_OUSTERS_PENDENT,			//	¸ñ°ÉÀÌ¸¦ ÀåÂøÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_EQUIP_OUSTERS_STONE,				//	Á¤·É¼®À» ÀåÂøÇÕ´Ï´Ù.

	UI_STRING_MESSAGE_OUSTERS_STONE,				// ºÒÀÇ Á¤·É ·¹º§ : 
	
	UI_STRING_MESSAGE_GRADE_MALCHUT,					// ¸¶¸£ÄíÆ®
	UI_STRING_MESSAGE_GRADE_YESOD,						// ¿¹¼Òµå
	UI_STRING_MESSAGE_GRADE_HOD,						// È£µå
	UI_STRING_MESSAGE_GRADE_NETRETH,					// ³×Â÷Å©
	UI_STRING_MESSAGE_GRADE_TIPHRETH,					// Æ¼ÆÄ·¹Æ®
	UI_STRING_MESSAGE_GRADE_GEBURAH,					// °Ôºê¶ó
	UI_STRING_MESSAGE_GRADE_CHESED,						// ÃÇ¼¼µå
	UI_STRING_MESSAGE_GRADE_BINAH,						// ¹ÙÀÌ³ª
	UI_STRING_MESSAGE_GRADE_CHOKMA,						// È£Å©¸¶
	UI_STRING_MESSAGE_GRADE_KEATHER,					// ÄÉÅ×¸£

	UI_STRING_MESSAGE_SKILL_LEVEL,						// Skill Level : %d
	UI_STRING_MESSAGE_REQUIRE_ELEMENTAL_LEVEL,			// ¿ä±¸ ¼Ó¼º ·¹º§ (%s)(%d)
	UI_STRING_MESSAGE_LEARN_SKILL,						// ÀÌ ½ºÅ³À» ¹è¿ì½Ã°Ú½À´Ï±î?
	UI_STRING_MESSAGE_LEARN_SKILL2,						// // ÀÌ ½ºÅ³À» ÀÍÈ÷½Ã¸é °è¿­ÀÌ °áÁ¤µË´Ï´Ù. ´ÙÀ½ ½ºÅ³ºÎÅÍ´Â ÀÌ °è¿­¿¡ ÇØ´çµÇ´Â ½ºÅ³¸¸ ¹è¿ï ¼ö ÀÖ°Ô µË´Ï´Ù.
	UI_STRING_MESSAGE_SKILL_LEVEL_UP,					// ÀÌ ½ºÅ³ÀÇ ·¹º§À» ¿Ã¸®½Ã°Ú½À´Ï±î?
	UI_STRING_MESSAGE_REQUIRE_SKILL_POINT,				// ÇÊ¿ä ½ºÅ³ Æ÷ÀÎÆ® : %d Point

	UI_STRING_MESSAGE_ELEMENTAL_FIRE,					// ºÒ
	UI_STRING_MESSAGE_ELEMENTAL_WATER,					// ¹°
	UI_STRING_MESSAGE_ELEMENTAL_EARTH,					// ´ëÁö
	UI_STRING_MESSAGE_ELEMENTAL_WIND,					// ¹Ù¶÷
	UI_STRING_MESSAGE_ELEMENTAL_SUM,					// ÇÕ
	
	UI_STRING_MESSAGE_EP,								// ¿µ·Â

	UI_STRING_MESSAGE_NOT_SHOW_GUILD_CHATTING,				//	"±æµåÃ¤ÆÃ ¾Èº¸±â",
	UI_STRING_MESSAGE_SHOW_GUILD_CHATTING,					//	"±æµåÃ¤ÆÃ º¸±â",
	UI_STRING_MESSAGE_GUILD_CHATTING,						//	"±æµåÃ¤ÆÃ ÇÏ±â",
	
	UI_STRING_MESSAGE_RANK_BONUS_WOOD_SKIN,					// = "Protection 15 Áõ°¡";
    UI_STRING_MESSAGE_RANK_BONUS_WIND_SENSE,				// = "Defense 10 Áõ°¡";
    UI_STRING_MESSAGE_RANK_BONUS_HOMING_EYE,				// = "To Hit 10 Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_LIFE_ENERGY,				// = "HP 15 Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_SOUL_ENERGY,				// = "EP 25 Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_STONE_MAUL,				// = "ÀüÅõ±â¼ú °è¿­ Damage 5Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_SWIFT_ARM,					// = "°ø°Ý¼Óµµ 20 Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_FIRE_ENDOW,				// = "Fire °è¿­ °ø°Ý¸¶¹ý Damage 3Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_WATER_ENDOW,				// = "Water °è¿­ °ø°Ý¸¶¹ý Damage 3Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_EARTH_ENDOW,				// = "Earth °è¿­ °ø°Ý¸¶¹ý Damage 3Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_ANTI_ACID_SKIN,			// = "Acid ÀúÇ× 15% Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_ANTI_BLOODY_SKIN,			// = "Bloody ÀúÇ× 15% Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_ANTI_CURSE_SKIN,			// = "Curse ÀúÇ× 15% Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_ANTI_POISON_SKIN,			// = "Poison ÀúÇ× 15% Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_ANTI_SILVER_DAMAGE_SKIN,	// = "Àº µ¥¹ÌÁö 20% °¨¼Ò";
	UI_STRING_MESSAGE_RANK_BONUS_BLESS_OF_NATURE,			// = "½ºÅ³»ç¿ë½Ã EP ¼Ò¸ð·® 20% °¨¼Ò";
	UI_STRING_MESSAGE_RANK_BONUS_LIFE_ABSORB,				// = "HP ½ºÆ¿ 2% Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_SOUL_ABSORB,				// = "EP ½ºÆ¿ 2% Áõ°¡";
	UI_STRING_MESSAGE_RANK_BONUS_MYSTIC_RULE,				// = "Lucky 2 Áõ°¡";

	UI_STRING_MESSAGE_HPBAR_EP_DESCRIPTION,					// "EP:%d/%d"

	UI_STRING_MESSAGE_LEFT_BONUS_POINT,						// º¸³Ê½º Æ÷ÀÎÆ®¸¦ ºÐ¹èÇÏ¼Å¾ß ÇÕ´Ï´Ù.

	UI_STRING_MESSAGE_GUILD,								// ±æµå(Guild)
	UI_STRING_MESSAGE_GUILD_INFO,							// ±æµå Á¤º¸(Guild Info)
	UI_STRING_MESSAGE_GUILD_MEMBER_LIST,					// ±æµå¿ø ¸®½ºÆ®(Guild Member List)
	UI_STRING_MESSAGE_GUILD_HELP,							// ±æµå µµ¿ò¸»
	
	UI_STRING_MESSAGE_ELEMENTAL_FIRE_DESCRIPTION,			// Fire:%d";
	UI_STRING_MESSAGE_ELEMENTAL_WATER_DESCRIPTION,			// Water:%d";
	UI_STRING_MESSAGE_ELEMENTAL_EARTH_DESCRIPTION,			// Earth:%d";
	UI_STRING_MESSAGE_ELEMENTAL_WIND_DESCRIPTION,			// Wind:%d";
	
	UI_STRING_MESSAGE_DESC_EP,								// EP :

	UI_STRING_MESSAGE_PREV_MAP,								// ÀÌÀü ¸ÊÀ» º¾´Ï´Ù.
	UI_STRING_MESSAGE_NEXT_MAP,								// ´ÙÀ½ ¸ÊÀ» º¾´Ï´Ù.
	UI_STRING_MESSAGE_CLOSE_HORN,							// ´ëÁö Á¤·ÉÀÇ »ÔÀ» Ãë¼ÒÇÕ´Ï´Ù.
	
	UI_STRING_MESSAGE_DESC_ATTACK_SPEED,					// °ø°Ý¼Óµµ : "

	STRING_STATUS_EP_MAX_1,									// EP ÃÖ´ëÄ¡°¡ %d°¡ µÇ¾ú½À´Ï´Ù.
	STRING_STATUS_EP_MAX_2,									// EP ÃÖ´ëÄ¡°¡ %dÀÌ µÇ¾ú½À´Ï´Ù.
	UI_STRING_MESSAGE_DESC_MAGIC_DAMAGE,					// ¸¶¹ý µ¥¹ÌÁö :
	
	STRING_MESSAGE_OPEN_LAIR,								// %s°¡ ¿­·È½À´Ï´Ù.
	STRING_MESSAGE_CLOSED_LAIR,								// %s°¡ ´ÝÇû½À´Ï´Ù.
	STRING_MESSAGE_LEFT_TIME_LAIR,							// %sÀÇ ÃâÀÔ °¡´É ½Ã°£ÀÌ %dºÐ ³²¾Ò½À´Ï´Ù.
	
	STRING_MESSAGE_CONTRACT_GNOMES_HORN,					// ½Ã¿À¶÷¿¡°Ô °¡¼­ °è¾àÀ» ¸ÎÀ¸¼Å¾ß »ç¿ëÇÏ½Ç ¼ö ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_CONTRACT_GNOMES_HORN_OK,					// ´ëÁö Á¤·ÉÀÇ »ÔÀ» »ç¿ëÇÏ±â À§ÇÑ °è¾àÀ» ¸Î¾ú½À´Ï´Ù.
	
	STRING_MESSAGE_CANNOT_DOWN_SKILL,						// ½ºÅ³·¹º§À» ³·Ãâ ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_NOT_ENOUGH_MONEY_FOR_DOWN_SKILL,			// ¼ÒÁö±ÝÀÌ ºÎÁ·ÇÏ¿© ½ºÅ³·¹º§À» ³·Ãâ ¼ö ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_CONFIRM_DOWN_SKILL,					// %s ½ºÅ³ÀÇ ½ºÅ³ ·¹º§ÀÌ %d ¿¡¼­ %d·Î º¯°æµË´Ï´Ù. ¼Ò¿äºñ¿ëÀº $%d ÀÔ´Ï´Ù. º¯°æÇÏ½Ã°Ú½À´Ï±î?
	STRING_MESSAGE_SUCCESS_CHANGE,							// ¼º°øÀûÀ¸·Î º¯°æµÇ¾ú½À´Ï´Ù.
	UI_STRING_MESSAGE_WITHDRAW_POINT,						// È¸¼ö ½ºÅ³ Æ÷ÀÎÆ® : %d
	UI_STRING_MESSAGE_DESC_DOWN_SKILL,						// ÀÌ ½ºÅ³ÀÇ ·¹º§À» ³·Ãß½Ã°Ú½À´Ï±î?
	STRING_MESSAGE_NOT_OUSTERS,								// ¾Æ¿ì½ºÅÍÁî°¡ ¾Æ´Õ´Ï´Ù.
	STRING_MESSAGE_TOO_LOW_SKILL_LEVEL,						// ½ºÅ³·¹º§ÀÌ ³Ê¹« ³·½À´Ï´Ù.
	STRING_MESSAGE_TOO_HIGH_SKILL_LEVEL,					// ½ºÅ³·¹º§ÀÌ ³Ê¹« ³ô½À´Ï´Ù.
	STRING_MESSAGE_INVALID_SKILL,							// Àß¸øµÈ ½ºÅ³ÀÔ´Ï´Ù.
	STRING_MESSAGE_NOT_LEARNED_SKILL,						// ¾ÆÁ÷ ¹è¿ìÁö ¾ÊÀº ½ºÅ³ÀÔ´Ï´Ù.
	UI_STRING_MESSAGE_CONFIRM_UP_TO_LAST_SKILL_LEVEL,			// ½ºÅ³·¹º§À» ÃÖ°í´Ü°èÀÎ 30±îÁö ¿Ã¸± °æ¿ì ÀÌ¸¦ È¸¼öÇÒ ¼ö ¾ø½À´Ï´Ù. 30·¹º§·Î »ó½Â½ÃÅ°µµ·Ï ÇÏ°Ú½À´Ï±î? 
	STRING_MESSAGE_CANNOT_USE_OUSTERS,						// ¾Æ¿ì½ºÅÍÁî´Â »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_MIXING_FORGE_FAILED_SAME_OPTION_GROUP,	// ¾ÆÀÌÅÛÀÇ ¿É¼Ç ±×·ìÀÌ °°¾Æ¼­ ¼¯À» ¼ö ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_CONFIRM_CHANGE_SEX,					// Á¤¸»·Î ¼ºº°À» ÀüÈ¯ÇÏ½Ã°Ú½À´Ï±î?
	STRING_MESSAGE_CANNOT_CHANGE_SEX_BY_WEAR,				// ¿ÊÀ» ÀÔÀº»óÅÂ¿¡¼­´Â ¼ºº°À» ÀüÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_CANNOT_CHANGE_SEX_BY_COUPLE,				// Ä¿ÇÃÀÌ ÀÖ´Â Ä³¸¯ÅÍ´Â ¼ºº°À» ÀüÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù.

	UI_STRING_MESSAGE_EVENT_QUEST2_1,						// 1.ÁöÇýÀÇ ½ÉÆÇ\\n¸ñÀû:ÁÖ¾îÁø ÆÛÁñ ¼öÇà
	UI_STRING_MESSAGE_EVENT_QUEST2_2,						// 2.°á°èÀÇ ¿­¼è\\n¸ñÀû:·¹º§¿¡ µû¶ó ÁöÁ¤µÈ ±¤¹° ¼öÁý
	UI_STRING_MESSAGE_EVENT_QUEST2_3,						// 3.º¸ÀÌÁö ¾Ê´Â º®\\n¸ñÀû:¹Ì·Î¸¦ Åë°úÇÏ¿© ¸®ÇÇ´Ï¿ò È¹µæ
	UI_STRING_MESSAGE_EVENT_QUEST2_4,						// 4.¸»ÇÏ´Â ÀÎÇü\\nÁöÁ¤µÈ ´øÀü¿¡¼­ ÆÛÁñ Á¶°¢À» Ã£¾Æ Á¶ÇÕ
	UI_STRING_MESSAGE_EVENT_QUEST2_5,						// 5.¹Ì·¡¸¦ ¿©´Â¹®\\nÆ¯Á¤ Å¬·¡½ºÀÇ ¸ó½ºÅÍµé¿¡°Ô¼­ ¸¶¼®À» Ã£¾Æ ÄÚµåÇ¥ Á¶ÇÕ

	STRING_MESSAGE_SELECT_MINI_GAME,						// µÎ °¡Áö ½ÃÇèÀÌ ¸¶·ÃµÇ¾î ÀÖ½À´Ï´Ù. ÀÌÁß ÇÑ°¡Áö¸¦ ¼±ÅÃÇÏ½Ê½Ã¿À.
	STRING_MESSAGE_SELECT_ARROW_TILES,						// Arrow Tiles. È­»ìÇ¥¸¦ µû¶ó °ñ ÁöÁ¡±îÁö ¹«»çÈ÷ ÀÌµ¿ÇÏ´Â °ÔÀÓÀÔ´Ï´Ù.
	STRING_MESSAGE_SELECT_CRAZY_MINE,						// Crazy Mine. ¼ýÀÚ ¾ß±¸¿Í Áö·Ú Ã£±â°¡ ÇÕÃÄÁø ÆÛÁñ°ÔÀÓÀÔ´Ï´Ù.

	STRING_MESSAGE_GET_RIFINIUM,							// ¸®ÇÇ´Ï¿òÀ» È¹µæÇÏ¿´½À´Ï´Ù.
	UI_STRING_MESSAGE_QUEST_STATUS_ARROW_TILES,				// Arrow Tiles
	UI_STRING_MESSAGE_QUEST_STATUS_CRAZY_MINE,				// Crazy Mine
	UI_STRING_MESSAGE_SUCCESS_MINIGAME,						// "All Stage Clear!"

	STRING_MESSAGE_GUILD_REGIST_FAIL_ALREADY_JOIN,		// ¾îµð ÇÑ¹ø º¼±î? ÀÚ³×´Â ÀÌ¹Ì <guild_name> ±æµå ¼Ò¼ÓÀÌ¶ó°í ³ª¿Í ÀÖ±º
	STRING_MESSAGE_GUILD_REGIST_FAIL_QUIT_TIMEOUT,		// ÀÚ³×´Â ´Ù¸¥ ±æµå¸¦ Å»ÅðÇÑÁö ¾ó¸¶ µÇÁö ¾Ê¾Ò±º. Á¶±Ý ´õ ½ÅÁßÇÏ°Ô »ý°¢ÇÏ°í Çàµ¿ÇÏ°Ô
	STRING_MESSAGE_GUILD_REGIST_FAIL_CANCEL_TIMEOUT,		// ÀÚ³×´Â ±æµå¸¦ Ãë¼Ò ´çÇÑ Áö ¾ó¸¶ µÇÁö ¾Ê¾Ò±º. µî·Ï Á¶°Ç¿¡ ºÎÇÕÇÏµµ·Ï ÀÚ½ÅÀ» Á» ´õ ±â¸£°í ¿À°Ô. Á» ´õ ½ÅÁßÇÏ°Ô ±âÈ¸¸¦ ¿³º¸µµ·Ï ÇÏ°Ô
	STRING_MESSAGE_GUILD_REGIST_FAIL_LEVEL,				// ÈÇ¸¢ÇÏÁö¸¸ ¸®´õ·Î½áÀÇ ÀÚÁúÀº Á» ºÎÁ·ÇÑ °Í °°±º. ½Ç·ÂÀ» ´Û°í ¿À°Ô³ª
	STRING_MESSAGE_GUILD_REGIST_FAIL_MONEY,				// ±æµå¸¦ ¸¸µé·Á¸é ¸¹Àº µ·ÀÌ ÇÊ¿äÇÏ´Ù³×. ÀÚ³×´Â µ·ÀÌ ¾ø¾î º¸ÀÌ´Â±º...
	STRING_MESSAGE_GUILD_REGIST_FAIL_FAME,				// <player_name>ÀÌ¶ó.. ³­ ¾ÆÁ÷ ±×·± ÀÌ¸§Àº µé¾îº¸Áöµµ ¸øÇß´Ù³×. ±×°Ç ÀÚ³×°¡ Ç²³»±â¶ó´Â °ÍÀ» ÀÇ¹ÌÇÏÁö. ½Ç·ÂÀ» ´Û°í ´Ù½Ã Ã£¾Æ¿À°Ô³ª
	STRING_MESSAGE_GUILD_REGIST_FAIL_NAME,				// ±æµå ÀÌ¸§ÀÌ ÀÌ¹Ì ¾²ÀÌ°í ÀÖ±º, ´Ù¸¥ ÀÌ¸§À» »ý°¢ÇØ º¸°Ô
	STRING_MESSAGE_GUILD_REGIST_FAIL_DENY,				// °ÅºÎµÇ¾ú½À´Ï´Ù.

	STRING_MESSAGE_GUILD_STARTING_FAIL_ALREADY_JOIN,		// ´ç½ÅÀº ÀÌ¹Ì ´Ù¸¥ ±æµå¿¡ °¡ÀÔµÇ¾î ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_GUILD_STARTING_FAIL_QUIT_TIMEOUT,		// ÀÚ³×´Â ´Ù¸¥ ±æµå¸¦ Å»ÅðÇÑÁö ¾ó¸¶ µÇÁö ¾Ê¾Ò±º. Á¶±Ý ´õ ½ÅÁßÇÏ°Ô »ý°¢ÇÏ°í Çàµ¿ÇÏ°Ô
	STRING_MESSAGE_GUILD_STARTING_FAIL_CANCEL_TIMEOUT,	// ÀÚ³×´Â ±æµå¸¦ Ãë¼Ò ´çÇÑ Áö ¾ó¸¶ µÇÁö ¾Ê¾Ò±º. Á¶°Ç¿¡ ºÎÇÕÇÏµµ·Ï ÀÚ½ÅÀ» Á» ´õ ±â¸£°í ¿À°Ô. Á» ´õ ½ÅÁßÇÏ°Ô ±âÈ¸¸¦ ¿³º¸µµ·Ï ÇÏ°Ô
	STRING_MESSAGE_GUILD_STARTING_FAIL_LEVEL,				// ÀÚ³×´Â ¾ÆÁ÷ ºÎÁ·ÇÑ Á¡ÀÌ ¸¹¾Æ º¸ÀÌ´Â±¸¸¸. Á¶±Ý ´õ ½Ç·ÂÀ» ´Û°í Ã£°Ô³ª.
	STRING_MESSAGE_GUILD_STARTING_FAIL_MONEY,				// <player_name>, ±æµå µî·ÏÀ» À§ÇØ¼­´Â µ·ÀÌ ´õ ÇÊ¿äÇÏ´Ù³×
	STRING_MESSAGE_GUILD_STARTING_FAIL_FAME,				// <player_name>ÀÌ¶ó.. ³­ ¾ÆÁ÷ ±×·± ÀÌ¸§Àº µé¾îº¸Áöµµ ¸øÇß´Ù³×. ±×°Ç ÀÚ³×°¡ Ç²³»±â¶ó´Â °ÍÀ» ÀÇ¹ÌÇÏÁö. ½Ç·ÂÀ» ´Û°í ´Ù½Ã Ã£¾Æ¿À°Ô³ª
	
	UI_STRING_MESSAGE_CLICK_TO_SHOW_DETIAL,					// Å¬¸¯ÇÏ½Ã¸é ¼¼ºÎ ³»¿ëÀ» º¸½Ç ¼ö ÀÖ½À´Ï´Ù.
	
	STRING_MESSAGE_OUSTERS,									// ¾Æ¿ì½ºÅÍÁî";
	
	STRING_MESSAGE_EVENT_FLAG_WAR_READY,					// 5ºÐµÚ¿¡ Capture the Flag! ÀÌº¥Æ®¸¦ ½Ç½ÃÇÕ´Ï´Ù.";
	STRING_MESSAGE_EVENT_FLAG_WAR_START,					// Capture the Flag! ÀÌº¥Æ®°¡ ½ÃÀÛµÇ¾ú½À´Ï´Ù.";
	STRING_MESSAGE_EVENT_FLAG_WAR_FINISH,					// Capture the Flag! ÀÌº¥Æ®°¡ %sÀÇ ½Â¸®·Î ³¡³µ½À´Ï´Ù. 3ºÐ µÚ ¾ÆÀÌÅÛÀÌ ÅÍÁý´Ï´Ù.";
	STRING_MESSAGE_EVENT_FLAG_WAR_WINNER,					// %s Á¾Á·ÀÌ %d°³ÀÇ ±ê¹ßÀ» ¸ð¾Æ¼­ ½Â¸®ÇÏ¿´½À´Ï´Ù.";
	STRING_MESSAGE_EVENT_FLAG_WILL_POUR_ITEM_AFTER_3MIN,	// "3ºÐÈÄ¿¡ ±ê¹ßÁö¿ª¿¡¼­ ¾ÆÀÌÅÛÀÌ ÅÍÁý´Ï´Ù.";
	STRING_MESSAGE_EVENT_FLAG_POURED_ITEM,					// Capture the Flag ÀÌº¥Æ® ¾ÆÀÌÅÛÀÌ ³ª¿Ô½À´Ï´Ù.

	UI_STRING_MESSAGE_GET_EVENT_FLAG_STATUS,				// È¹µæÇÑ ±ê¹ß¼ö : %d
	
	UI_STRING_MESSAGE_MUTE,									// ¿î¿µÀÚ¿¡ ÀÇÇØ¼­ Ã¤ÆÃÀÌ ±ÝÁöµÇ¾ú½À´Ï´Ù.
	STRING_MESSAGE_CANNOT_MOVE_SAFETY_ZONE_FLAG,			// ±ê¹ßÀ» µé°í ¾ÈÀüÁö´ë·Î µé¾î°¥ ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_CANNOT_DROP_ITEM_BY_FLAG,				// ±ê´ë ÁÖº¯¿¡´Â ¾ÆÀÌÅÛÀ» ¹ö¸± ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_POUR_ITEM_AFTER_SECOND,					// %d ÃÊ ÈÄ¿¡ ±ê¹ßÁö¿ª¿¡¼­ ¾ÆÀÌÅÛÀÌ ÅÍÁý´Ï´Ù.

	UI_STRING_MESSAGE_CANNOT_ACTION_MOTORCYCLE_FLAG,		// ±ê´ë ÁÖº¯¿¡´Â ¿ÀÅä¹ÙÀÌ¿¡¼­ ³»¸®°Å³ª Å» ¼ö ¾ø½À´Ï´Ù.
	
	UI_STRING_MESSAGE_TEAM_INFO_GUILD_NAME,					// Guild Name : %s"
	UI_STRING_MESSAGE_TEAM_INFO_GUILD_INTRODUCTION,			// Guild Introduction : "
	UI_STRING_MESSAGE_OTHER_INFO_GUILD_NAME,				// ±æµå ÀÌ¸§ : 
	UI_STRING_MESSAGE_NOT_JOIN_ANY_GUILD,					// µî·ÏµÈ ±æµå ¾øÀ½
	
	UI_STRING_MESSAGE_USE_ETERNITY_FOR_RESURRECT,			// Eternity ½ºÅ³À» »ç¿ëÇÏ¿© »ì¾Æ³³´Ï´Ù.
	UI_STRING_MESSAGE_CANNOT_RELOAD_BY_VIVID_MAGAZINE,		// VIVID MAGAZINE À» ¹è¿ö¾ß ÀåÀüÇÒ ¼ö ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_RESURRECT_AFTER_SECONDS,					// %d ÃÊ ÈÄ¿¡ ºÎÈ°ÇÕ´Ï´Ù.
	
	STRING_MESSAGE_CANNOT_TRADE_SUMMON_SYLPH,				// ¹Ù¶÷ÀÇ Á¤·ÉÀ» Åº »óÅÂ¿¡¼­´Â °Å·¡ÇÒ ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_CANNOT_USE_SUMMON_SYLPH,					// ¹Ù¶÷ÀÇ Á¤·ÉÀ» Åº »óÅÂ¿¡¼­ »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù.
	
	STRING_MESSAGE_MODIFY_SKILL_LEVEL_1,					// %s ½ºÅ³·¹º§ÀÌ %dÀÌ µÇ¾ú½À´Ï´Ù.
	STRING_MESSAGE_MODIFY_SKILL_LEVEL_2,					// %s ½ºÅ³·¹º§ÀÌ %d°¡ µÇ¾ú½À´Ï´Ù.

	// 2003.09.29
	UI_STRING_MESSAGE_GO_TO_BEGINNER_ZONE,					// ÃÊº¸ Àü¿ë »ç³ÉÅÍ·Î ÀÌµ¿ÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_LEFT_PREMIUM_DAYS,					// ÇÁ¸®¹Ì¾ö ¼­ºñ½º Á¾·áÀÏ±îÁö %dÀÏ ³²¾Ò½À´Ï´Ù.
	UI_STRING_MESSAGE_NOT_PREMIUM_USER,						// ÇÁ¸®¹Ì¾ö ¼­ºñ½º »ç¿ëÀÚ°¡ ¾Æ´Õ´Ï´Ù.
	UI_STRING_MESSAGE_EXPIRE_PREMIUM_SERVICE_TODAY,			// ÇÁ¸®¹Ì¾ö ¼­ºñ½º´Â ¿À´Ã±îÁö ÀÔ´Ï´Ù.

	STRING_MESSAGE_EXPIRE_PREMIUM_SERVICE_MESSAGE_1,		// ÇÁ¸®¹Ì¾ö ¼­ºñ½º ±â°£ÀÌ ³¡³ª±â Àü¿¡ ¿¬¼Ó°áÁ¦¸¦ ÇÒ °æ¿ì ´Ùµ§ ¸¶ÀÏ¸®Áö¿Í 
	STRING_MESSAGE_EXPIRE_PREMIUM_SERVICE_MESSAGE_2,		// ¾ÆÀÌÅÛ º¹±Çµî ´Ù¾çÇÑ ÇýÅÃÀ» ¹ÞÀ¸½Ç ¼ö ÀÖ½À´Ï´Ù. 
	STRING_MESSAGE_EXPIRE_PREMIUM_SERVICE_MESSAGE_3,		// ÀÚ¼¼ÇÑ ³»¿ëÀº ´ÙÅ©¿¡µ§ È¨ÆäÀÌÁö(www.darkeden.com)¿¡¼­ È®ÀÎÇÏ½Ç ¼ö ÀÖ½À´Ï´Ù. 

	STRING_MESSAGE_LEVEL_WAR_ZONE_NAME,						// Ä®¸®°í ´øÀü

	UI_STRING_MESSAGE_SWEEPER_BONUS_1,						// ¸ðµç ´É·ÂÄ¡ + 2
	UI_STRING_MESSAGE_SWEEPER_BONUS_2,						// Ã¼·Â(HP) + 20
	UI_STRING_MESSAGE_SWEEPER_BONUS_3,						// µ¥¹ÌÁö + 3
	UI_STRING_MESSAGE_SWEEPER_BONUS_4,						// Áö½Ä + 7, ¹ÎÃ¸¼º + 7
	UI_STRING_MESSAGE_SWEEPER_BONUS_5,						// Ã¼·Â(HP) + 50
	UI_STRING_MESSAGE_SWEEPER_BONUS_6,						// Çà¿î + 7
	UI_STRING_MESSAGE_SWEEPER_BONUS_7,						// ¸¶¹ý °ø°Ý µ¥¹ÌÁö + 10
	UI_STRING_MESSAGE_SWEEPER_BONUS_8,						// Áö½Ä + 7, Èû + 7
	UI_STRING_MESSAGE_SWEEPER_BONUS_9,						// Ã¼·Â(HP) Èí¼ö + 15
	UI_STRING_MESSAGE_SWEEPER_BONUS_10,						// ¹°¸® °ø°Ý µ¥¹ÌÁö + 10
	UI_STRING_MESSAGE_SWEEPER_BONUS_11,						// ¸ðµç ÀúÇ×·Â + 7
	UI_STRING_MESSAGE_SWEEPER_BONUS_12,						// ½Ã¾ß + 5, ¸ðµç ´É·ÂÄ¡ + 4

	UI_STRING_MESSAGE_JOIN_LEVEL_WAR,						// ÀüÀï¿¡ Âü°¡ÇÕ´Ï´Ù.
	
	UI_STRING_MESSAGE_CLICK_TO_WARP_REGEN_TOWER,			// Å¬¸¯ÇÏ½Ã¸é ÇØ´ç ¸®Á¨ Á¸ Å¸¿ö·Î ÀÌµ¿ÇÕ´Ï´Ù.

	UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_HAS_OUSTERS,		// ½½·¹ÀÌ¾î°¡ °¡Áö°í ÀÖ½À´Ï´Ù
	UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_OUSTERS,			// ½½·¹ÀÌ¾îÀÇ ¼öÈ£¼º´Ü¿¡ ÀÖ½À´Ï´Ù

	UI_STRING_MESSAGE_MAILBOX,									// ÆíÁöÇÔ(Mail Box)
	UI_STRING_MESSAGE_CLOSE_MAILBOX,							// ÆíÁöÇÔÀ» ´Ý½À´Ï´Ù.
	UI_STRING_MESSAGE_MAILBOX_TAB_MAIL,							// ÆíÁö¸¦ È®ÀÎÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_MAILBOX_TAB_HELP,							// µµ¿ò¸»À» È®ÀÎÇÕ´Ï´Ù.
	UI_STRING_MESSAGE_MAILBOX_TAB_MEMO,							// ÂÊÁö¸¦ È®ÀÎÇÕ´Ï´Ù.

	STRING_MESSAGE_SUCCESS_CHANGED_BAT_COLOR,				// ¹ÚÁã»öÀÌ º¯°æµÇ¾ú½À´Ï´Ù.

	UI_STRING_MESSAGE_LEVEL_WAR,							// ·¹º§ ÀüÀï
	
	UI_STRING_MESSAGE_LEFT_FAMILY_DAYS,						// ÆÐ¹Ð¸® ¼­ºñ½º Á¾·áÀÏ±îÁö %dÀÏ ³²¾Ò½À´Ï´Ù.";
	UI_STRING_MESSAGE_EXPIRE_FAMILY_TODAY,					// ÆÐ¹Ð¸® ¼­ºñ½º´Â ¿À´Ã±îÁö ÀÔ´Ï´Ù.";

	UI_STRING_MESSAGE_TOO_FAR,								// ´õ °¡±îÀÌ °¡¼Å¾ß ÇÕ´Ï´Ù.";

	UI_STRING_MESSAGE_PET_DESC_DURABILITY,					// ³²Àº ¸ÔÀÌ¾ç : 
	UI_STRING_MESSAGE_PET_ATTR,								// ¼Ó¼º : 
	
	UI_STRING_MESSAGE_SHOW_PET_INFO_WINDOW,					// "¾Ö¿Ïµ¿¹° Á¤º¸Ã¢À» º¾´Ï´Ù",
	
	STRING_MESSAGE_ENCHANT_FAIL,							// ÀÎÃ¾Æ®°¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù.
	UI_STRING_MESSAGE_PET_DESC_DURABILITY_2,				// ³²Àº ¸ÔÀÌ¾ç 

	UI_STRING_MESSAGE_PET_RESSURECT,						// ÆêÀ» ºÎÈ° ½ÃÅ°½Ã°Ú½À´Ï±î?
	UI_STRING_MESSAGE_CLOSE_PET_INFO,						// ¾Ö¿Ïµ¿¹° Á¤º¸Ã¢À» ´Ý½À´Ï´Ù.
	
	UI_STRING_MESSAGE_CAN_ENCHANT_PET,						// ¾Ö¿Ïµ¿¹°À» ÈÆ·Ã½ÃÅ³ ¼ö ÀÖ½À´Ï´Ù.
	UI_STRING_MESSAGE_CANNOT_ENCHANT_PET,					// ·¹º§ÀÌ ¿Ã¶úÀ»¶§¸¸ ÈÆ·Ã½ÃÅ³ ¼ö ÀÖ½À´Ï´Ù.

	STRING_MESSAGE_NEW_PET_LEVEL_1,							// ¾Ö¿Ïµ¿¹° ·¹º§ÀÌ %d°¡ µÇ¾ú½À´Ï´Ù.";
	STRING_MESSAGE_NEW_PET_LEVEL_2,							// ¾Ö¿Ïµ¿¹° ·¹º§ÀÌ %dÀÌ µÇ¾ú½À´Ï´Ù.";

	STRING_MESSAGE_OPTION_NAME_LUCK_3,						// ·°Å°
	STRING_MESSAGE_OPTION_NAME_LUCK_4,						// ¹Ì´Ï¿Â

	STRING_MESSAGE_OPTION_NAME_ATTR_3,						// ³ÊÆ®
	STRING_MESSAGE_OPTION_NAME_ATTR_4,						// Å©·±Ä¡

	STRING_MESSAGE_PET_DIE_WARNING,							// %sÀÇ ¸ÔÀÌ°¡ %s ºÐ·®¹Û¿¡ ³²Áö ¾Ê¾Ò½À´Ï´Ù.
	STRING_MESSAGE_PET_REQUEST_REFILL,						// ¸ÔÀÌ¸¦ ÃæÀüÇÏ½Ã±â ¹Ù¶ø´Ï´Ù.
	STRING_MESSAGE_PET_DIE,									// %sÀÌ »ç¸ÁÇÏ¿´½À´Ï´Ù.
	STRING_MESSAGE_PET_SUMMON,								// %sÀ» ¼ÒÈ¯ÇÏ¿´½À´Ï´Ù.
	STRING_MESSAGE_PET_CAN_GET_ATTR,						// %s¿¡°Ô ¼Ó¼ºÀ» ºÎ¿©ÇÏ½Ç ¼ö ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_PET_GAMBLE_OK,							// %s¿¡°Ô 2Â÷ ´É·ÂÀ» ºÎ¿©ÇÏ¼Ì½À´Ï´Ù.
	STRING_MESSAGE_PET_GAMBLE_DESC,							// ¾ÆÀÌÅÛ°ú ´õºÒ¾î ¸ó½ºÅÍÀÇ ¸Ó¸®µµ ÇÔ²² ·çÆÃÇÒ ¼ö ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_PET_GAMBLE_DESC_TEEN,					// ¾ÆÀÌÅÛ°ú ´õºÒ¾î ¼Ò¿ï ½ºÅæµµ ÇÔ²² ·çÆÃÇÒ ¼ö ÀÖ½À´Ï´Ù.
	STRING_MESSAGE_PET_CAN_GET_OPTION,						// %s¿¡°Ô ¿É¼ÇÀ» ºÎ¿©ÇÏ½Ç ¼ö ÀÖ½À´Ï´Ù.
	UI_STRING_MESSAGE_HPBAR_EXP_DESCRIPTION_NEW,			// "³²Àº °æÇèÄ¡ : %s (%s%)"
	STRING_MESSAGE_PET_GAMBLE_FAIL,							// 2Â÷ ´É·Â ºÎ¿©°¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù.

	STRING_MESSAGE_OPTION_ENAME_LUCK_3,						// ·°Å°
	STRING_MESSAGE_OPTION_ENAME_LUCK_4,						// ¹Ì´Ï¿Â

	STRING_MESSAGE_OPTION_ENAME_ATTR_3,						// ³ÊÆ®
	STRING_MESSAGE_OPTION_ENAME_ATTR_4,						// Å©·±Ä¡

	UI_STRING_MESSAGE_CAN_PET_REVIVAL,						// ºÎÈ°½ÃÅ³ ¼ö ÀÖ½À´Ï´Ù.
	UI_STRING_MESSAGE_CANNOT_PET_REVIVAL,					// Á×Àº °æ¿ì¿¡¸¸ »ç¿ëÇÒ ¼ö ÀÖ½À´Ï´Ù.

	UI_STRING_MESSAGE_PET_CAN_CUT_HEAD,						// 2Â÷´É·Â ÀÖÀ½
	UI_STRING_MESSAGE_PET_CANNOT_CUT_HEAD,					// 2Â÷´É·Â ¾øÀ½
	
	UI_STRING_MESSAGE_REMOVE_PET_OPTION,					// ¾Ö¿Ïµ¿¹°ÀÇ ¿É¼ÇÀ» Áö¿ì½Ã°Ú½À´Ï±î?

	STRING_MESSAGE_CANNOT_EVENT_GIFT_BOX,

	STRING_MESSAGE_EVENT_NETMARBLE_1,
	STRING_MESSAGE_EVENT_NETMARBLE_2,
	STRING_MESSAGE_EVENT_NETMARBLE_3,
	STRING_MESSAGE_EVENT_NETMARBLE_4,
	
	UI_STRING_MESSAGE_CAN_USE_PET_FOOD,
	UI_STRING_MESSAGE_DETACH_PET_FOOD,
	UI_STRING_MESSAGE_USE_PET_FOOD,
	UI_STRING_MESSAGE_CANNOT_DETACH,

	UI_STRING_MESSAGE_COMPUTER_STRING_0,
	UI_STRING_MESSAGE_COMPUTER_STRING_1,
	UI_STRING_MESSAGE_COMPUTER_STRING_2,
	UI_STRING_MESSAGE_COMPUTER_STRING_3,
	UI_STRING_MESSAGE_COMPUTER_STRING_4,
	UI_STRING_MESSAGE_COMPUTER_STRING_5,
	UI_STRING_MESSAGE_COMPUTER_STRING_6,
	UI_STRING_MESSAGE_COMPUTER_STRING_7,
	UI_STRING_MESSAGE_COMPUTER_STRING_8,
	UI_STRING_MESSAGE_COMPUTER_STRING_9,
	UI_STRING_MESSAGE_COMPUTER_STRING_10,
	UI_STRING_MESSAGE_COMPUTER_STRING_11,
	UI_STRING_MESSAGE_COMPUTER_STRING_12,
	UI_STRING_MESSAGE_COMPUTER_STRING_13,
	UI_STRING_MESSAGE_COMPUTER_STRING_14,
	UI_STRING_MESSAGE_COMPUTER_STRING_15,
	UI_STRING_MESSAGE_COMPUTER_STRING_16,
	UI_STRING_MESSAGE_COMPUTER_STRING_17,
	UI_STRING_MESSAGE_COMPUTER_STRING_18,
	UI_STRING_MESSAGE_COMPUTER_STRING_19,
	UI_STRING_MESSAGE_COMPUTER_STRING_20,
	UI_STRING_MESSAGE_COMPUTER_STRING_21,
	UI_STRING_MESSAGE_COMPUTER_STRING_22,
	UI_STRING_MESSAGE_COMPUTER_STRING_23,
	UI_STRING_MESSAGE_COMPUTER_STRING_24,
	UI_STRING_MESSAGE_COMPUTER_STRING_25,
	UI_STRING_MESSAGE_COMPUTER_STRING_26,
	UI_STRING_MESSAGE_COMPUTER_STRING_27,
	UI_STRING_MESSAGE_COMPUTER_STRING_28,
	UI_STRING_MESSAGE_COMPUTER_STRING_29,

	UI_STRING_MESSAGE_BOOK_NAME_0,
	UI_STRING_MESSAGE_BOOK_NAME_1,
	UI_STRING_MESSAGE_BOOK_NAME_2,
	UI_STRING_MESSAGE_BOOK_NAME_3,
	UI_STRING_MESSAGE_BOOK_NAME_4,
	UI_STRING_MESSAGE_BOOK_NAME_5,
	UI_STRING_MESSAGE_BOOK_NAME_6,
	UI_STRING_MESSAGE_BOOK_NAME_7,
	UI_STRING_MESSAGE_BOOK_NAME_8,
	UI_STRING_MESSAGE_BOOK_NAME_9,
	UI_STRING_MESSAGE_BOOK_NAME_10,
	UI_STRING_MESSAGE_BOOK_NAME_11,
	UI_STRING_MESSAGE_BOOK_NAME_12,
	UI_STRING_MESSAGE_BOOK_NAME_13,
	UI_STRING_MESSAGE_BOOK_NAME_14,
	UI_STRING_MESSAGE_BOOK_NAME_15,
	UI_STRING_MESSAGE_BOOK_NAME_16,
	UI_STRING_MESSAGE_BOOK_NAME_17,
	UI_STRING_MESSAGE_BOOK_NAME_18,
	UI_STRING_MESSAGE_BOOK_NAME_19,
	UI_STRING_MESSAGE_BOOK_NAME_20,

	//Monster Name
	UI_STRING_MESSAGE_MONSTER_ENAME_TNDEAD,				//	"ÅÍ´×µ¥µå",
	UI_STRING_MESSAGE_MONSTER_ENAME_ARKHAN,				//	"¾ËÄ­",
	UI_STRING_MESSAGE_MONSTER_ENAME_ESTROIDER,		//"¿¡½ºÆ®·ÎÀÌ´õ",
	UI_STRING_MESSAGE_MONSTER_ENAME_GOLEMER,				//	"°ñ·¹¸Ó",
	UI_STRING_MESSAGE_MONSTER_ENAME_DARKSCREAMER,		//	"´ÙÅ©½ºÅ©¸®¸Ó",
	UI_STRING_MESSAGE_MONSTER_ENAME_DEADBODY,			//	"µ¥µå¹Ùµð",
	UI_STRING_MESSAGE_MONSTER_ENAME_MODERAS,				//	"¸ðµ¥¶ó½º",
	UI_STRING_MESSAGE_MONSTER_ENAME_VANDALIZER,			//	"¹ê´ú¶óÀÌÀú",
	UI_STRING_MESSAGE_MONSTER_ENAME_DIRTYSTRIDER,		//	"´õÆ¼½ºÆ®¶óÀÌ´õ",
	UI_STRING_MESSAGE_MONSTER_ENAME_HELLWIZARD,			//	"ÇïÀ§ÀÚµå",
	UI_STRING_MESSAGE_MONSTER_ENAME_TNSOUL,				//	"ÅÍ´×¼Ò¿ï",
	UI_STRING_MESSAGE_MONSTER_ENAME_IRONTEETH,			//	"¾ÆÀÌ¾ðÆ¼¾²",
	UI_STRING_MESSAGE_MONSTER_ENAME_REDEYE,				//	"·¹µå¾ÆÀÌ",
	UI_STRING_MESSAGE_MONSTER_ENAME_CRIMSONSLAUGTHER,	//	"Å©¸²½¼½½·ÎÅÍ",
	UI_STRING_MESSAGE_MONSTER_ENAME_HELLGUARDIAN,		//	"Çï°¡µð¾ð",
	UI_STRING_MESSAGE_MONSTER_ENAME_SOLDIER,				//	"¼ÖÁ®",
	UI_STRING_MESSAGE_MONSTER_ENAME_RIPPER,				//	"¸®ÆÛ",
	UI_STRING_MESSAGE_MONSTER_ENAME_BIGFANG,				//	"ºòÆØ",
	UI_STRING_MESSAGE_MONSTER_ENAME_LORDCHAOS,			//	"·ÎµåÄ«¿À½º",
	UI_STRING_MESSAGE_MONSTER_ENAME_CHAOSGUARDIAN,		//	"Ä«¿À½º°¡µð¾ð",
	UI_STRING_MESSAGE_MONSTER_ENAME_HOBBLE,				//	"È£ºí",
	UI_STRING_MESSAGE_MONSTER_ENAME_CHAOSNIGHT,			//	"Ä«¿À½º³ªÀÌÆ®",
	UI_STRING_MESSAGE_MONSTER_ENAME_WIDOWS,				//	"À§µµ¿ì½º",
	UI_STRING_MESSAGE_MONSTER_ENAME_KID,					//	"Å°µå",
	UI_STRING_MESSAGE_MONSTER_ENAME_SHADOWWING,			//	"¼¨µµ¿ìÀ®",

	UI_STRING_MESSAGE_HAN_DOMAIN_BLADE, // ]="µµ";
	UI_STRING_MESSAGE_HAN_DOMAIN_SWORD, // ]="°Ë";
	UI_STRING_MESSAGE_HAN_DOMAIN_ENCHANT, // ]="ÀÎÃ¦Æ®";
	UI_STRING_MESSAGE_HAN_DOMAIN_GUN, // ]	="ÃÑ";
	UI_STRING_MESSAGE_HAN_DOMAIN_HEAL, // ]="Èú";
	UI_STRING_MESSAGE_HAN_DOMAIN_ETC, // ]="±âÅ¸";
	UI_STRING_MESSAGE_HAN_DOMAIN_VAMPIRE, // ]="¹ìÆÄÀÌ¾î";
	UI_STRING_MESSAGE_HAN_DOMAIN_OUSTERS, // ]="¾Æ¿ì½ºÅÍÁî";
	
	UI_STRING_MESSAGE_ENG_DOMAIN_ETC, // ]="Etc";
	UI_STRING_MESSAGE_ENG_DOMAIN_VAMPIRE, // ]="Vampire";
	UI_STRING_MESSAGE_ENG_DOMAIN_OUSTERS, // ]="Ousters";
	
	STRING_MESSAGE_SOUL_STONE,
	
	UI_STRING_MESSAGE_CAN_PET_MUTANT,	// º¯½Å ½ÃÅ³ ¼ö ÀÖ½À´Ï´Ù.
	UI_STRING_MESSAGE_CANNOT_PET_MUTANT,	// º¯½Å ½ÃÅ³ ¼ö ¾ø½À´Ï´Ù.
	UI_STRING_MESSAGE_PET_MUTANT,

	// 2004, 03, 26 sobeit add start
	STRING_ERROR_CANNOT_AUTHORIZE_BILLING,   // ºô¸µ Á¤º¸¸¦ Ã£À» ¼ö ¾ø½À´Ï´Ù.
    STRING_ERROR_CANNOT_CREATE_PC_BILLING,    // À¯·á »ç¿ëÀÚ°¡ ¾Æ´Ï¶ó¼­ Ä³¸¯ÅÍ¸¦ ¸ø ¸¸µì´Ï´Ù.
	// 2004, 03, 26 sobeit add end
	
	UI_STRING_MESSAGE_DESC_PET_DEAD_DAY,	// Á×ÀºÁö %dÀÏ Áö³µ½À´Ï´Ù.

	// 2004, 04, 3 sobeit add start
	UI_STRING_MESSAGE_DESC_LUCKY,	// "Çà¿î : "
	// 2004, 04, 3 sobeit add end

	// 2004, 4, 13 sobeit add start
	UI_STRING_MESSAGE_PET_QUEST_NPC_SLAYER, // "±×·ç¹ö";
	UI_STRING_MESSAGE_PET_QUEST_NPC_VAMPIRE, // "Ä«ÆÄÆ¼´Ï";
	UI_STRING_MESSAGE_PET_QUEST_NPC_OUSTERS,//  "¾Æ¸¶Å¸";

	UI_STRING_MESSAGE_PET_QUEST_SLAYER,		// ¼¾ÅÍ¿ì·Î Äù½ºÆ®
	UI_STRING_MESSAGE_PET_QUEST_VAMPIRE,	// ½ºÅÍÁö Äù½ºÆ®
	UI_STRING_MESSAGE_PET_QUEST_OUSTERS,	// ÇÈ½Ã Äù½ºÆ®
	UI_STRING_MESSAGE_PET_QUEST_CLEAR,		// %s¿¡°Ô µ¹¾Æ°¡½Ê½Ã¿À.

	UI_STRING_MESSAGE_CAN_3RD_ENCHANT_PET, //3Â÷ ´É·ÂÀ» ºÎ¿©ÇÒ ¼ö ÀÖ½À´Ï´Ù.
	UI_STRING_MESSAGE_CANNOT_3RD_ENCHANT_PET, //"¾ÆÁ÷Àº 3Â÷ ´É·ÂÀ» ºÎ¿©ÇÒ ¼ö ¾ø½À´Ï´Ù.";
	STRING_MESSAGE_PET_3RD_GAMBLE_FAIL,//"3Â÷ ´É·Â ºÎ¿©°¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù.";
	STRING_MESSAGE_PET_3RD_GAMBLE_OK,//"%s¿¡°Ô 3Â÷ ´É·ÂÀ» ºÎ¿©ÇÏ¼Ì½À´Ï´Ù.";
	// 2004, 4, 13 sobeit add end
	
	UI_STRING_MESSAGE_CANNOT_SUMMON_2ND_PET, // Level 40 ÀÌÇÏ´Â 2Â÷ ÆêÀ» ¼ÒÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù.
	STRING_MESSAGE_SEARCHING_MINE,			// Ã£¾Æ¾ßÇÒ Áö·Ú¼ö : %d Ã¼Å©ÇÑ Áö·Ú ¼ö : %d
	UI_STRING_MESSAGE_CANNOT_MIXING_GRADE_VALUE, //"¾ÆÀÌÅÛ°£ µî±ÞÂ÷°¡ 2µî±ÞÀ» ³ÑÀ¸¸é ¹Í½ÌÇÒ ¼ö ¾ø½À´Ï´Ù.";

	// 2004, 4, 29 sobeit add start
	STRING_MESSAGE_MINIGAME_GAME_OVER,	//"game over";
	STRING_MESSAGE_MINIGAME_ALL_STAGE_CLEAR, // "All stage clear";
	// 2004, 4, 29 sobeit add end

	// 2004, 5, 6, sobeit add start
	UI_STRING_MESSAGE_SELECT_QUEST_OUSTERS, //"¿ä±Ù·¡ ¸ó½ºÅÍ ¼ö°¡ ±ÞÁõÇÏ°í ÀÖ½À´Ï´Ù. ÀÌ ¸ó½ºÅÍµé·Î ÀÎÇØ¼­ °á°èÀÇ ÈûÀÌ Á¡Á¡ ¾àÇØÁö°í ÀÖ½À´Ï´Ù. ¿ì¸® Á¾Á·ÀÇ ºÎÈ°À» À§ÇØ¼­¶óµµ ÀÌ ¸ó½ºÅÍµéÀ» Ã³¸®ÇÏ°í °á°è¸¦ ÁöÄÑ³»¾ß ÇØ¾ß ÇÕ´Ï´Ù. ÀÌ ÀÓ¹«´Â Áßµµ¿¡ Á¢¼ÓÀ» Á¾·áÇÏ°Ô µÇ¸é ¹«È¿°¡ µÇ´Ï ÁÖÀÇÇØ¾ß ÇÏ¸ç, Äù½ºÆ®¿¡ ÇØ´çÇÏ´Â ¸ó½ºÅÍ ¼ö¸¸Å­ »ç³ÉÇÏ°Ô µÇ°í µ¹¾Æ ¿À½Å´Ù¸é Á¦°¡ ±ÍÇÑ ¼±¹°À» µå¸®µµ·Ï ÇÏ°Ú½À´Ï´Ù. ÀÚ, ±×·³ ¾î¶² Äù½ºÆ®¸¦ ¼±ÅÃ ÇÏ½Ç·¡¿ä?";
	// 2004, 5, 6, sobeit add end
	// 2004, 6, 22 By Csm 
	STRING_ERROR_CHILDGUARD_DENYED, // "User who age under 18 will not able to play after 22.00 O'clock.";
	// end 
	STRING_MESSAGE_KEEP_PETITEM,//] = "ÆêÀ» º¸°ü ÇÏ½Ã°Ú½À´Ï±î?";
	STRING_MESSAGE_GET_KEEP_PETITEM,//] = "%s ÆêÀ» Ã£À¸½Ã°Ú½À´Ï±î?";
	
	STRING_MESSAGE_EXIST_ITEM_ALREADY, //"ÀÌ¹Ì ´Ù¸¥ ¾ÆÀÌÅÛÀÌ ÀÖ½À´Ï´Ù.";

	UI_STRING_MESSAGE_ITEM_GRADE , //"  ¥°¥±¥²¥³¥´¥µ¥¶¥·¥¸¥¹";
	UI_STRING_MESSAGE_ITEM_GRADE_DESC, // µî±Þ :

	UI_STRING_MESSAGE_SMS_SEND_OK,//"¸Þ¼¼Áö¸¦ ¼º°øÀûÀ¸·Î º¸³Â½À´Ï´Ù.";
	UI_STRING_MESSAGE_SMS_SEND_FAIL,//"¸Þ¼¼Áö¸¦ º¸³»´Âµ¥ ½ÇÆÐÇß½À´Ï´Ù.";
	UI_STRING_MESSAGE_SMS_ADD_FAIL,//"¸®½ºÆ®¿¡ Ãß°¡ÇÏ´Âµ¥ ½ÇÆÐÇß½À´Ï´Ù.";
	UI_STRING_MESSAGE_SMS_DELETE_FAIL,//"¸®½ºÆ®¿¡¼­ »èÁ¦ÇÏ´Âµ¥ ½ÇÆÐÇß½À´Ï´Ù.";
	
	UI_STRING_MESSAGE_SMS_FAIL_MAX_NUM_EXCEEDED,//"ÀúÀå °¡´É ¹øÈ£°¡ ÃÊ°úµÇ¾ú½À´Ï´Ù.";
	UI_STRING_MESSAGE_SMS_FAIL_INVALID_DATA,//"Àß¸øµÈ Á¤º¸ ÀÔ´Ï´Ù.";
	UI_STRING_MESSAGE_SMS_FAIL_NO_SUCH_EID,//"Á¤º¸¸¦ Ã£À» ¼ö ¾ø½À´Ï´Ù.";
	UI_STRING_MESSAGE_SMS_FAIL_NOT_ENOUGH_CHARGE,//"ÃæÀü·®ÀÌ ¸ðÀÚ¶ø´Ï´Ù.";
	UI_STRING_MESSAGE_SMS_WINDOW,//"SMS(SMS Widow)";
	UI_STRING_HELP_SMS_SEND,//] = "¸Þ¼¼Áö¸¦ º¸³À´Ï´Ù.";
	UI_STRING_HELP_SMS_VIEW_LIST,//] = "ÀüÈ­ ¹øÈ£ºÎ¸¦ º¾´Ï´Ù.";
	UI_STRING_HELP_SMS_WINDOW,//] = "Æ¯¼ö ¹®ÀÚ¸¦ ¼±ÅÃ ÇÕ´Ï´Ù.";
	UI_STRING_HELP_SMS_ADDSEND,//] = "¼±ÅÃÇÑ ¹øÈ£¸¦ º¸³»±â ¸ñ·Ï¿¡ Ãß°¡ ÇÕ´Ï´Ù.";
	UI_STRING_HELP_SMS_DELETE,//] = "¼±ÅÃÇÑ ¹øÈ£¸¦ »èÁ¦ ÇÕ´Ï´Ù.";
	UI_STRING_HELP_SMS_NEW,//] = "½Å±Ô ¹øÈ£¸¦ µî·Ï ÇÕ´Ï´Ù.";

	UI_STRING_MESSAGE_USE_SMSITEM, //  = "SMS ¾ÆÀÌÅÛÀ» »ç¿ëÇÏ½Ã°Ú½À´Ï±î?";
	UI_STRING_MESSAGE_NAMING_WINDOW, //"NAMING(NAMING Window)";
	UI_STRING_MESSAGE_CHANGE_PET_NICKNAME,//] = "ÆêÀÇ ´Ð³×ÀÓÀ» º¯°æÇÕ´Ï´Ù.";
	UI_STRING_MESSAGE_CHANGE_PLAYER_NICKNAME,//] = "À¯Àú ÀÚÀ¯ ´Ð³×ÀÓÀ» º¯°æÇÕ´Ï´Ù.";
	UI_STRING_MESSAGE_ADD_PLAYER_NICKNAME,//] = "´Ð³×ÀÓÀ» Ãß°¡ ÇÕ´Ï´Ù";
	UI_STRING_MESSAGE_SELECT_PLAYER_NICKNAME,//] = "¼±ÅÃµÈ ´Ð³×ÀÓÀ¸·Î º¯°æÇÕ´Ï´Ù.";
	UI_STRING_MESSAGE_NICKNAME_CHANGE_OK, //"´Ð³×ÀÓÀÌ º¯°æµÇ¾ú½À´Ï´Ù.";
	UI_STRING_MESSAGE_NICKNAME_SELECT_FAIL_FORCED,//] = "¿î¿µÀÚ°¡ °­Á¦ ÁöÁ¤ÇÑ ´Ð³×ÀÓÀº º¯°æÇÒ ¼ö ¾ø½À´Ï´Ù.";
	UI_STRING_MESSAGE_USE_NAMINGITEM, //"³×ÀÌ¹Ö ¾ÆÀÌÅÛÀ» »ç¿ëÇÏ½Ã°Ú½À´Ï±î?";

	UI_STRING_MESSAGE_PET_NAMING_SUMMON,//] = "ÆêÀÌ ¼ÒÈ¯Áß ÀÏ¶§¸¸ ´Ð³×ÀÓÀ» º¯°æÇÒ ¼ö ÀÖ½À´Ï´Ù.";
	UI_STRING_MESSAGE_PET_NAMING_WOLVERINE,//] = "¿ï¹ö¸°Àº Ææ ¾ÆÀÌÅÛ ¾øÀÌ ´Ð³×ÀÓÀ» º¯°æÇÒ ¼ö ÀÖ½À´Ï´Ù.";
	UI_STRING_MESSAGE_FAIL_SEARCH_ITEM,//] = "¾ÆÀÌÅÛÀ» Ã£À» ¼ö ¾ø½À´Ï´Ù.";

	UI_STRING_CORE_ZAP_BLACK,//] = "¹°¸® °ø°Ý·Â +%d";
	UI_STRING_CORE_ZAP_RED,//] = "¸¶¹ý °ø°Ý·Â +%d";
	UI_STRING_CORE_ZAP_BLUE,//] = "¹°¸® ¹æ¾î·Â +%d";
	UI_STRING_CORE_ZAP_GREEN,//] = "¸¶¹ý ¹æ¾î·Â +%d";

	UI_STRING_CORE_ZAP_REWARD_ALL_STAT,//] = "¸ðµç ´É·ÂÄ¡ +%d";
	UI_STRING_CORE_ZAP_REWARD_ALL_REG,//] = "¸ðµç ÀúÇ× +%d";
	// 2004, 7,1 sobeit add start

	UI_STRING_MESSAGE_DESC_DIALOG_OPTION_EMPTY2, //"\t       %s";
	UI_STRING_MESSAGE_DESC_DIALOG_OPTION2, //"\t¿É¼Ç : %s";

	UI_STRING_GQUEST_CAN_NOT,//] = "¼öÇàºÒ°¡";
	UI_STRING_GQUEST_CAN_ACCEPT,//] = "¼öÇà°¡´É";
	UI_STRING_GQUEST_DOING,//] = "¼öÇàÁß";
	UI_STRING_GQUEST_SUCCESS,//] = "¼º°ø";
	UI_STRING_GQUEST_COMPLETE,//] = "¿Ï·á";
	UI_STRING_GQUEST_FAIL,//] = "½ÇÆÐ";
	UI_STRING_GQUEST_CAN_REPLAY,//] = "Àç¼öÇà°¡";
	UI_STRING_GQUEST_MISSION,// "ÀÓ¹«%d:%s";

	UI_STRING_GQUEST_BUTTON_ACCEPT,//] = "Äù½ºÆ®¸¦ ¼öÇàÇÕ´Ï´Ù.";
	UI_STRING_GQUEST_BUTTON_GIVEUP,//] = "Äù½ºÆ®¸¦ Æ÷±âÇÕ´Ï´Ù.";
	UI_STRING_GQUEST_TAB_PROCESS,//] = "ÁøÇàÁßÀÎ ¸ñ·ÏÀ» º¾´Ï´Ù.";
	UI_STRING_GQUEST_TAB_COMPLETE,//] = "¿Ï·áµÈ ¸ñ·ÏÀ» º¾´Ï´Ù.";

	UI_STRING_NOTICE_EVENT_GOLD_MEDALS,// = "±Ý¸Þ´ÞÀ» %d°³ ¸ð¾Ò½À´Ï´Ù.";
	STRING_ERROR_KEY_EXPIRED,//] = "ÀÎÁõÅ°°¡ À¯È¿ÇÏÁö ¾Ê½À´Ï´Ù. À¥¿¡¼­ ´Ù½Ã ·Î±×ÀÎ ÇØÁÖ½Ã½Ê¿ä.";
	STRING_ERROR_NOT_FOUND_KEY,//] = "ÀÎÁõÅ°¸¦ Ã£À» ¼ö ¾ø½À´Ï´Ù. À¥¿¡¼­ ´Ù½Ã ·Î±×ÀÎ ÇØÁÖ½Ã½Ê¿ä.";

	UI_STRING_GQUEST_UPDATE, // = "Äù½ºÆ® Á¤º¸°¡ ¾÷µ¥ÀÌÆ® µÇ¾ú½À´Ï´Ù.";

	UI_STRING_CHANGE_EVENTITEM_PRICE,//= "ÀÌº¥Æ® ¾ÆÀÌÅÛ °¡°ÝÀÌ %d·Î º¯°æµÇ¾ú½À´Ï´Ù.";
	
	STRING_MESSAGE_TOO_MANY_GUILD_REGISTERED,//] = "°ø¼º ½ÅÃ»ÇÑ ±æµå°¡ ³Ê¹« ¸¹¾Æ¼­ ½ÅÃ»ÇÒ ¼ö ¾ø½À´Ï´Ù.";
	STRING_MESSAGE_REINFORCE_DENYED,//] = "¼öºñÃø ÂüÀü ½ÅÃ»ÀÌ ÀÌ¹Ì °ÅºÎ´çÇØ¼­ Àç½ÅÃ» ÇÒ ¼ö ¾ø½À´Ï´Ù.";
	STRING_MESSAGE_ALREADY_REINFORCE_ACCEPTED,//] = "¼öºñÃø ÂüÀü ½ÅÃ»ÀÌ Çã°¡µÈ ±æµå°¡ ÀÌ¹Ì ÀÖ¾î¼­ ½ÅÃ»ÇÒ ¼ö ¾ø½À´Ï´Ù.";
	STRING_MESSAGE_NO_WAR_REGISTERED,//] = "°ø¼º ½ÅÃ»ÇÑ ±æµå°¡ ¾ø¾î¼­ ¼öºñÃø ÂüÀü ½ÅÃ»À» ÇÒ ¼ö ¾ø½À´Ï´Ù.";
	STRING_MESSAGE_CANNOT_ACCEPT,//] = "¼öºñÃø ÂüÀü ½ÅÃ»À» ¼ö¶ôÇÒ ¼ö ¾ø½À´Ï´Ù";
	STRING_MESSAGE_ACCEPT_OK,//] = "ÂüÀü ½ÅÃ»À» ¼ö¶ôÇÏ¿´½À´Ï´Ù.";
	
	STRING_MESSAGE_CANNOT_DENY,							// ¼öºñÃø ÂüÀü ½ÅÃ»À» °ÅºÎÇÒ ¼ö ¾ø½À´Ï´Ù.
    STRING_MESSAGE_DENY_OK,                               // ÂüÀü ½ÅÃ»À» °ÅºÎÇÏ¿´½À´Ï´Ù.

	STRING_MESSAGE_SIEGE_POTAL_200,//] = "Æ®·¦Á¸À¸·Î ÀÌµ¿";
	STRING_MESSAGE_SIEGE_POTAL_201,//] = "³»¼º¹® ¾ÕÀ¸·Î ÀÌµ¿";
	STRING_MESSAGE_SIEGE_POTAL_202,//] = "³»¼º ¾ÈÀ¸·Î ÀÌµ¿";
	STRING_MESSAGE_SIEGE_POTAL_203,//] = "³»¼º ¾ÈÀ¸·Î ÀÌµ¿";
	STRING_MESSAGE_LOGIN_ERROR_NONPK, //= "Ä³¸¯ÅÍÀÇ ·¹º§ÀÌ ³ô¾Æ Á¢¼ÓÇÒ ¼ö ¾ø½À´Ï´Ù. ´Ù¸¥ ¼­¹ö¸¦ ÀÌ¿ëÇØ ÁÖ½Ê½Ã¿ä.";

	STRING_MESSAGE_DAUM_EVENT_1,//] = "´ÙÅ©¿¡µ§ÀÌ ÁØºñÇÑ ÃÊº¸ÀÚ ÀÌº¥Æ®! °ÔÀÓ¿¡ ÀÍ¼÷ÇÏÁö ¸øÇÑ ÃÊº¸À¯Àú¸¦ À§ÇÏ¿© ´ÙÅ©¿¡µ§¿¡¼­ Á¶±×¸¶ÇÑ ¼±¹°°ú ÇÔ²² º¹±ÇÀ» µå¸³´Ï´Ù. ÀÌº¥Æ®¸¦ ¿Ï·áÇÏ°í ¾òÀº º¹±ÇÀ¸·Î ´Ù¾çÇÏ°Ô ÁØºñµÈ ¼±¹°ÀÇ ÁÖÀÎÀÌ µÉ ¼ö ÀÖ½À´Ï´Ù. ¶ÇÇÑ ´ÙÀ½ ·¹º§¿¡ µµ´ÞÇÏ½Ã¸é º¸´Ù ´Ù¾çÇÑ ¼±¹°ÀÌ ÁØºñµÇ¾î ÀÖ½À´Ï´Ù. ÀÌº¥Æ® ¾ÆÀÌÅÛÀ» ¸ðµÎ ¸ð¾Æ °í¼º´ÉÀÇ ¾ÆÀÌÅÛÀ» ´À²¸º¸½Ã±â ¹Ù¶ø´Ï´Ù. ÀÌº¥Æ®´Â 8°³ÀÇ ÀÌº¥Æ®·Î ÀÌ·ç¾îÁ® ÀÖÀ¸¸ç °¢ ·¹º§¿¡ µû¶ó Áö±ÞµË´Ï´Ù. Äù½ºÆ® Á¤º¸ Ã¢ ´ÜÃàÅ°´Â Ctrl + Q ÀÔ´Ï´Ù. ´ÙÅ©¿¡µ§ÀÇ ¼¼°è¿¡¼­ Áñ°Å¿î ½Ã°£ÀÌ µÇ½Ã±æ ¹Ù¶ó¸ç ³ª¸¸ÀÇ Ä³¸¯ÅÍµµ Å°¿ì°í ¼±¹°µµ ¹ÞÀ» ¼ö ÀÖ´Â ±âÈ¸¸¦ ³õÄ¡Áö ¸¶¼¼¿ä.";
	STRING_MESSAGE_DAUM_EVENT_2,//] = "´ÙÅ©¿¡µ§ÀÌ ÁØºñÇÑ ÃÊº¸ÀÚ ÀÌº¥Æ®! %Lev% ¿¡ µµ´ÞÇÏ¼Ì±º¿ä. %¼ö·É»óÀÚ»ö% »óÀÚ¸¦ ³Ö¾îµå·È½À´Ï´Ù. Ctrl + Q À» ´­·¯ È®ÀÎÇØ º¸¼¼¿ä. ÀÌÀü¿¡ ¼ö·ÉÇÏ½Å »óÀÚ°¡ ÀÖ´Ù¸é ¾ÆÀÌÅÛÀ» ¼ö·ÉÇÏ½Ç ¼ö ÀÖ½À´Ï´Ù. ÀÚ ¿©·¯ºÐµé²² µå¸° ¾ÆÀÌÅÛÀº ½Ã°£Á¦ÇÑÀÌ °É·ÁÀÖ´Â ¾ÆÀÌÅÛÀÔ´Ï´Ù. ±âÁ¸ Âø¿ë Á¦ÇÑÀÌ ÀÖ´Â ¾ÆÀÌÅÛµé º¸´Ù ³ôÀº ¼öÁØÀÇ ¾ÆÀÌÅÛÀ¸·Î½á ÀÌº¥Æ®¸¦ ÅëÇØ Áö±ÞµÈ ÀÌº¥Æ® ¾ÆÀÌÅÛÀÎ °ü°è·Î °Å·¡¿Í ±³È¯ÀÌ ºÒ°¡´ÉÇÏ´ä´Ï´Ù. Â÷ÈÄ º¸´Ù ´õ ÁÁÀº ¾ÆÀÌÅÛÀ» ±¸ÇÏ½Ã¸é NPC¸¦ ÅëÇÏ¿© ÆÇ¸Å¸¦ ÇÏ½Ç ¼ö ÀÖ½À´Ï´Ù. ¾ÆÀÌÅÛ ÇÏ´Ü¿¡ º¸½Ã¸é ¾ÕÀ¸·Î ³²¾Æ ÀÖ´Â ½Ã°£ÀÌ Ç¥½ÃµË´Ï´Ù. ¾ÆÂ÷ ÀÌ ½Ã°£Àº ¿©·¯ºÐµéÀÌ ´ÙÅ©¿¡µ§ ¿ùµå¸¦ ¶°³ª °è¼Åµµ ½Ç½Ã°£À¸·Î Àû¿ëµÇ±â ¶§¹®¿¡ ÀÌ ½Ã°£À» À¯¿ëÇÏ°Ô »ç¿ëÇÏ½Ã±â ¹Ù¶ø´Ï´Ù. ±×·³ °è¼ÓÇØ¼­ Áñ°Å¿î ½Ã°£µÇ½Ã±â ¹Ù¶ø´Ï´Ù.";
	STRING_MESSAGE_DAUM_EVENT_3,//] = "%Lev% ¿¡ µµ´ÞÇÏ¼Ì±º¿ä. %¼ö·É»óÀÚ»ö% »óÀÚ¸¦ ³Ö¾îµå·È½À´Ï´Ù. Ctrl + Q À» ´­·¯ È®ÀÎÇØ º¸¼¼¿ä.";
	STRING_MESSAGE_DAUM_EVENT_BOX1,//] = "»¡°­»ö";
	STRING_MESSAGE_DAUM_EVENT_BOX2,//] = "ÁÖÈ²»ö";
	STRING_MESSAGE_DAUM_EVENT_BOX3,//] = "³ë¶û»ö";
	STRING_MESSAGE_DAUM_EVENT_BOX4,//] = "ÃÊ·Ï»ö";
	STRING_MESSAGE_DAUM_EVENT_BOX5,//] = "ÆÄ¶û»ö";
	STRING_MESSAGE_DAUM_EVENT_BOX6,//] = "³²»ö";
	STRING_MESSAGE_DAUM_EVENT_BOX7,//] = "º¸¶ó»ö";
	STRING_MESSAGE_DAUM_EVENT_BOX8,//] = "°ËÁ¤»ö";
		
	UI_STRING_MESSAGE_RANK_BONUS_PERCEPTION,//] = "¸ð´É +2";
    UI_STRING_MESSAGE_RANK_BONUS_STONE_OF_SAGE,//] = "Áö½Ä(INT) +5";
    UI_STRING_MESSAGE_RANK_BONUS_FOOT_OF_RANGER,//] = "¹ÎÃ¸¼º(DEX) +5";
    UI_STRING_MESSAGE_RANK_BONUS_WARRIORS_FIST,//] = "Èû(STR) +5";

    UI_STRING_MESSAGE_RANK_BONUS_ACID_INQUIRY,//] = "¾Ö½Ãµå(Acid) ÀúÇ× +10%";
    UI_STRING_MESSAGE_RANK_BONUS_BLOODY_INQUIRY,//] = "ºí·¯µå(Blood) ÀúÇ× +10%";
    UI_STRING_MESSAGE_RANK_BONUS_CURSE_INQUIRY,//] = "Ä¿½º(Curse) ÀúÇ×+10%";
    UI_STRING_MESSAGE_RANK_BONUS_POISON_INQUIRY,//] = "Æ÷ÀÌÁð(Poison) ÀúÇ× +10%";
	UI_STRING_MESSAGE_RANK_BONUS_INQUIRY_MASTERY,//] = "¸ðµç ÀúÇ× +3%";

    UI_STRING_MESSAGE_RANK_BONUS_POWER_OF_SPIRIT,//] = "¹æ¾îÀ²(Protection) 5%Áõ°¡";
    UI_STRING_MESSAGE_RANK_BONUS_WIND_OF_SPIRIT,//] = "È¸ÇÇÀ²(Defense) 5%Áõ°¡";
    UI_STRING_MESSAGE_RANK_BONUS_PIXIES_EYES,//] = "¸íÁß·ü(To hit) 5%Áõ°¡";
    UI_STRING_MESSAGE_RANK_BONUS_GROUND_OF_SPIRIT,//] = "¸¶¹ý·Â(MP) 5%Áõ°¡";
    UI_STRING_MESSAGE_RANK_BONUS_FIRE_OF_SPIRIT,//] = "Å©¸®Æ¼ÄÃ(Critical) °ø°Ý·Â 5%Áõ°¡";

    UI_STRING_MESSAGE_RANK_BONUS_EVOLUTION_IMMORTAL_HEART,//] = "»ý¸í·Â(HP) 5% Áõ°¡";
    UI_STRING_MESSAGE_RANK_BONUS_BEHEMOTH_ARMOR_2,//] = "È¸ÇÇÀ²(Defense) 5%Áõ°¡ ";
    UI_STRING_MESSAGE_RANK_BONUS_DRAGON_EYE_2,//] = "¸íÁß·ü(To hit) 5%Áõ°¡";
    UI_STRING_MESSAGE_RANK_BONUS_EVOLUTION_RELIANCE_BRAIN,//] = "¸¶¹ý·Â(MP) 5%Áõ°¡";
    UI_STRING_MESSAGE_RANK_BONUS_HEAT_CONTROL,//] = "Å©¸®Æ¼ÄÃ(Critical) °ø°Ý·Â 5%Áõ°¡";

	UI_STRING_MESSAGE_RANK_BONUS_ACID_MASTERY,//] = "»ó´ë¹æ ¾Ö½Ãµå(Acid) ÀúÇ× 10% °¨¼Ò";
    UI_STRING_MESSAGE_RANK_BONUS_BLOODY_MASTERY,//] = "»ó´ë¹æ ºí·¯µå(Blood) ÀúÇ× 10% °¨¼Ò";
    UI_STRING_MESSAGE_RANK_BONUS_CURSE_MASTERY,//] = "»ó´ë¹æ Ä¿½º(Curse) ÀúÇ× 10%°¨¼Ò";
    UI_STRING_MESSAGE_RANK_BONUS_POISON_MASTERY,//] = "»ó´ë¹æ Æ÷ÀÌÁð(Poison) ÀúÇ× 10%°¨¼Ò";
    UI_STRING_MESSAGE_RANK_BONUS_SKILL_MASTERY,//] = "»ó´ë¹æ ¸ðµç ÀúÇ× 3% °¨¼Ò";

    UI_STRING_MESSAGE_RANK_BONUS_SALAMANDERS_KNOWLEDGE,//] = "ºÒ°è¿­ Á¤·É +1";
    UI_STRING_MESSAGE_RANK_BONUS_UNDINES_KNOWLEDGE,//] = "¹°°è¿­ Á¤·É +1";
    UI_STRING_MESSAGE_RANK_BONUS_GNOMES_KNOWLEDGE,//] = "´ëÁö°è¿­ Á¤·É +1";
	
	UI_STRING_MESSAGE_SIEGE_ATTACK,//] = "(°ø°ÝÃø)";
	UI_STRING_MESSAGE_SIEGE_DEFENSE,//] = "(¼öºñÃø)";
	
	UI_STRING_MESSAGE_NOT_LOGINED,//] = "(ºñÁ¢¼ÓÁß)";
	UI_STRING_MESSAGE_GUILD_LIST_ID,//] = "¾ÆÀÌµð";
	UI_STRING_MESSAGE_GUILD_LIST_SERVER,//] = "Á¢¼Ó ¼­¹ö";
	UI_STRING_MESSAGE_GUILD_LIST_GRADE,//] = "µî±Þ";

	UI_STRING_MESSAGE_TEAM_COMMAND_WINDOW,//] = "ÆÀ ¸í·É(Team Command)";
	UI_STRING_MESSAGE_TEAM_LIST_WINDOW,//] = "ÆÀ ¸ñ·Ï(Team List)";
	UI_STRING_MESSAGE_TEAM_WAIT_LIST_WINDOW,//] = "´ë±â ÆÀ ¸ñ·Ï(Wait Team List)";
	UI_STRING_MESSAGE_TEAM_UNION_WINDOW,//] = "ÆÀ ¿¬ÇÕ(Team Union)";

	UI_STRING_MESSAGE_CLAN_COMMAND_WINDOW,//] = "Å¬·£ ¸í·É(Clan Command)";
	UI_STRING_MESSAGE_CLAN_LIST_WINDOW,//] = "Å¬·£ ¸ñ·Ï(Clan List)";
	UI_STRING_MESSAGE_CLAN_WAIT_LIST_WINDOW,//] = "´ë±â Å¬·£ ¸ñ·Ï(Wait Clan List)";
	UI_STRING_MESSAGE_CLAN_UNION_WINDOW,//] = "Å¬·£ ¿¬ÇÕ(Clan Union)";

	UI_STRING_MESSAGE_GUILD_COMMAND_WINDOW,//] = "±æµå ¸í·É(Guild Command)";
	UI_STRING_MESSAGE_GUILD_LIST_WINDOW,//] = "±æµå ¸ñ·Ï(Guild List)";
	UI_STRING_MESSAGE_GUILD_WAIT_LIST_WINDOW,//] = "´ë±â ±æµå ¸ñ·Ï(Wait Guild List)";
	UI_STRING_MESSAGE_GUILD_UNION_WINDOW,//] = "±æµå ¿¬ÇÕ(Guild Union)";

	UI_STRING_MESSAGE_TOTAL_GUILD_DESC1,
	UI_STRING_MESSAGE_TOTAL_GUILD_DESC2,
	UI_STRING_MESSAGE_TOTAL_GUILD_DESC3,
	UI_STRING_MESSAGE_TOTAL_GUILD_ALREADY_IN_UNION,
	UI_STRING_MESSAGE_TOTAL_GUILD_ALREADY_OFFER_SOMETHING,
	UI_STRING_MESSAGE_TOTAL_GUILD_TARGET_IS_NOT_MASTER,   
	UI_STRING_MESSAGE_TOTAL_GUILD_NOT_IN_UNION,
	UI_STRING_MESSAGE_TOTAL_GUILD_MASTER_CANNOT_QUIT,
	UI_STRING_MESSAGE_TOTAL_GUILD_NO_TARGET_UNION,
	UI_STRING_MESSAGE_TOTAL_GUILD_NOT_YOUR_UNION,
	UI_STRING_MESSAGE_TOTAL_GUILD_SOURCE_IS_NOT_MASTER,
	UI_STRING_MESSAGE_TOTAL_GUILD_JOIN_ASK,
	UI_STRING_MESSAGE_TOTAL_GUILD_MESSAGE_OK,
	UI_STRING_MESSAGE_TOTAL_GUILD_MESSAGE_REFUSE,
	UI_STRING_MESSAGE_TOTAL_GUILD_MESSAGE_SUCCESS,
	UI_STRING_MESSAGE_TOTAL_GUILD_LEAVE_ASK,
	UI_STRING_MESSAGE_TOTAL_GUILD_LEAVE_MSG, 
	UI_STRING_MESSAGE_TOTAL_GUILD_LEAVE_MSG2,
	UI_STRING_MESSAGE_TOTAL_GUILD_LEAVE_OK,
	UI_STRING_MESSAGE_TOTAL_GUILD_LEAVE_CANCEL,
	UI_STRING_MESSAGE_TOTAL_GUILD_DEPORT_ASK,
	UI_STRING_MESSAGE_TOTAL_GUILD_DEPORT_OK,		

	UI_STRING_MESSAGE_TOTAL_UNION_JOIN_MSG,//] =  "%s ±æµå°¡ ¿¬ÇÕ±æµå °¡ÀÔÀ» ½ÅÃ»ÇÕ´Ï´Ù.";
	UI_STRING_MESSAGE_TOTAL_UNION_DEPORT_MSG,//] =  "%s ±æµå°¡ ¿¬ÇÕ±æµå Å»Åð¸¦ ½ÅÃ»ÇÕ´Ï´Ù.";
	UI_STRING_MESSAGE_TOTAL_UNION_PENALTY,//] =  "¿¬ÇÕ±æµå¿¡ °­Á¦·Î Å»ÅðÇÑ ±â·ÏÀÌ ÀÖ½À´Ï´Ù.";
	
	UI_STRING_MESSAGE_ASKING_RECALL,//] =  "%s ´ÔÀ» È£ÃâÇÏ½Ã°Ú½À´Ï±î?";
	UI_STRING_LEARN_SKILL_LEVEL,//] =  "½ºÅ³ ¹è¿ò ·¹º§ : %d ·¹º§";
	UI_STRING_MESSAGE_TOTAL_GUILD_LEAVE_ACCEPT,//] =  "¿¬ÇÕ Å»Åð¸¦ ¼ö¶ôÇÕ´Ï´Ù";
	UI_STRING_MESSAGE_TOTAL_GUILD_LEAVE_DENY,//] =  "¿¬ÇÕ Å»Åð¸¦ °ÅÀýÇÕ´Ï´Ù.";

	UI_STRING_MESSAGE_REQUEST_UNION_ERROR_1,//] =  "±æµå ¸¶½ºÅÍ¸¸ÀÌ ¿¬ÇÕ½ÅÃ»À» ÇÒ ¼ö ÀÖ½À´Ï´Ù.";
	UI_STRING_MESSAGE_REQUEST_UNION_ERROR_2,//] =  "ÀÌ¹Ì ¿¬ÇÕ¿¡ °¡ÀÔµÇ¾î ÀÖ½À´Ï´Ù.";
	UI_STRING_MESSAGE_REQUEST_UNION_ERROR_3,//] =  "¿¬ÇÕÀÇ ¸¶½ºÅÍ°¡ ¾Æ´Õ´Ï´Ù.";
	UI_STRING_MESSAGE_UNION_ERROR_NO_SLOT,// =  "¿¬ÇÕÀÇ ºó ÀÚ¸®°¡ ¾ø½À´Ï´Ù.";
	
	UI_STRING_MESSAGE_EQUIP_BLOOD_BIBLE,//] = "ºí·¯µå¹ÙÀÌºí ÀÎÀåÀ» ÀåÂøÇÕ´Ï´Ù.";
	UI_STRING_MESSAGE_RENT_BLOOD_BIBLE,//] = "ÇÇÀÇ ¼º¼­ Â¡Ç¥¸¦ ºô¸³´Ï´Ù.";
	UI_STRING_MESSAGE_RENT_BLOOD_BIBLE2,//] = "%s Â¡Ç¥¸¦ ºô¸³´Ï´Ù.";
	UI_STRING_MESSAGE_RENT_LATER_BLOOD_BIBLE,//] = "´ÙÀ½¿¡ ºô¸®°Ú½À´Ï´Ù.";

	UI_STRING_MESSAGE_RANGER_SAY,//] = "Ranger/";
	UI_STRING_MESSAGE_MODIFY_TAX_OK,//] = "¼¼À²ÀÌ ¼º°øÀûÀ¸·Î º¯°æ µÇ¾ú½À´Ï´Ù.";
	UI_STRING_MESSAGE_MODIFY_TAX_FAIL,//] = "¼¼À² º¯°æ¿¡ ½ÇÆÐ ÇÏ¿´½À´Ï´Ù.";
	UI_STRING_MESSAGE_MODIFY_TAX,//] = "º¯°æÇÒ ¼¼À²À» ÀÔ·ÂÇØ ÁÖ¼¼¿ä.";
	UI_STRING_MESSAGE_RANGER_SAY2,//] = "·¹ÀÎÁ®";

	UI_STRING_MESSAGE_REMOVE_CURSE_1,//] = "¢½";
	UI_STRING_MESSAGE_REMOVE_CURSE_2,//] = "»ç¶û";
	UI_STRING_MESSAGE_REMOVE_CURSE_3,//] = "»ç¶ûÇØ";
	UI_STRING_MESSAGE_REMOVE_CURSE_4,//] = "»ç¶ûÇØ¿ä";

	STRING_MESSAGE_RACE_WAR_STARTED_IN_OTHER_SERVER,//] = "Ã¹¹øÂ° ¼­¹ö¿¡¼­ Á¾Á·ÀüÀÌ ½ÃÀÛµÆ½À´Ï´Ù.";

	UI_STRING_MESSAGE_APPOINT_SUBMASTER, // "¼­ºê ¸¶½ºÅÍ·Î ÀÓ¸íÇÕ´Ï´Ù.";
	
	UI_STRING_MESSAGE_CAN_SKILL_DELETE,//] =  "½ºÅ³ ¹è¿ò Æ÷ÀÎÆ® È¸¼ö °¡´É";
	UI_STRING_MESSAGE_CANNOT_SKILL_DELETE,//] =  "½ºÅ³ ¹è¿ò Æ÷ÀÎÆ® È¸¼ö ºÒ°¡´É";

	STRING_MESSAGE_CANNOT_SKILLTREE_DELETE,//] =  "Æ÷ÀÎÆ®¸¦ È¸¼öÇÒ ¼ö ¾ø½À´Ï´Ù.";
	UI_STRING_MESSAGE_TOO_MANY_MEMBER,//] =  "¸â¹ö°¡ 50¸íÀ» ³Ñ¾î °¡ÀÔÇÒ ¼ö ¾ø½À´Ï´Ù.";
	UI_STRING_MESSAGE_CONFIRM_DOWN_SKILL2,//] =  "%s ½ºÅ³ÀÇ ½ºÅ³ ¹è¿ò Æ÷ÀÎÆ®¸¦ È¸¼ö ÇÕ´Ï´Ù. ¼Ò¸ð ºñ¿äÀº %dÀÔ´Ï´Ù. º¯°æÇÏ½Ã°Ú½À´Ï±î?";

	UI_STRING_MESSAGE_UNION_CHATTING,//] =  "¿¬ÇÕÃ¤ÆÃ ÇÏ±â";
	UI_STRING_MESSAGE_SHOW_UNION_CHATTING,//] =  "¿¬ÇÕÃ¤ÆÃ º¸±â";
	UI_STRING_MESSAGE_NOT_SHOW_UNION_CHATTING,//] =  "¿¬ÇÕÃ¤ÆÃ ¾Èº¸±â";

	UI_STRING_MESSAGE_USE_SIEGE_FOR_RESURRECT,//] = "³»¼º¹® ¾Õ¿¡¼­ ºÎÈ°ÇÕ´Ï´Ù.";

	UI_STRING_POWER_JJANG_GET_POINT,//] = "ÆÄ¿öÂ¯ Æ÷ÀÎÆ® °¡Á®¿À±â";
	UI_STRING_POWER_JJANG_POINT,//] = "³» ÆÄ¿öÂ¯ ´©Àû Æ÷ÀÎÆ®";
	UI_STRING_POWER_JJANG_AVAILABLE,//] = "±³È¯ °¡´ÉÇÑ Æ÷ÀÎÆ®";
	UI_STRING_POWER_JJANG_NUMBER_1,//] = "ÇÚµåÆù";
	UI_STRING_POWER_JJANG_NUMBER_2,//] = "¹øÈ£";
	UI_STRING_POWER_JJANG_GET_POINT_HELP,//] = "ÆÄ¿öÂ¯ Æ÷ÀÎÆ®¸¦ °¡Á®¿É´Ï´Ù.";
	UI_STRING_POWER_JJANG_EXCHANGE_HELP,//] = "ÆÄ¿öÂ¯ Æ÷ÀÎÆ®·Î ¾ÆÀÌÅÛÀ» ±³È¯ÇÕ´Ï´Ù.";
	UI_STRING_POWER_JJANG_REQUEST_OK,//] = "ÆÄ¿öÂ¯ Æ÷ÀÎÆ®°¡ ¼º°øÀûÀ¸·Î Àû¿ëµÇ¾ú½À´Ï´Ù. Àü¼ÛµÈ Æ÷ÀÎÆ® : %d Á¡";

	UI_STRING_POWER_JJANG_ERROR_NO_MEMBER,//] = "ÆÄ¿öÂ¯ È¸¿øÀÌ ¾Æ´Õ´Ï´Ù.";
	UI_STRING_POWER_JJANG_ERROR_SERVER_ERROR,//] = "ÆÄ¿öÂ¯ ¼­¹ö¿¡ ÀÌ»óÀÌ ÀÖ½À´Ï´Ù.´Ù½Ã ÇÑ¹ø ½ÃµµÇØº¸½Ã°í ¾ÈµÇ¸é www.powerzzang.com ¿¡¼­ È®ÀÎÇÏ½Ã±â ¹Ù¶ø´Ï´Ù.";
	UI_STRING_POWER_JJANG_ERROR_PROCESS_ERROR,//] = "ÆÄ¿öÂ¯ DB¿¡ ÀÌ»óÀÌ ÀÖ½À´Ï´Ù.´Ù½Ã ÇÑ¹ø ½ÃµµÇØº¸½Ã°í ¾ÈµÇ¸é www.powerzzang.com ¿¡¼­ È®ÀÎÇÏ½Ã±â ¹Ù¶ø´Ï´Ù.";
	UI_STRING_POWER_JJANG_ERROR_NO_POINT,//] = "´©ÀûµÈ ÆÄ¿öÂ¯ Æ÷ÀÎÆ®°¡ ¾ø½À´Ï´Ù.";
	UI_STRING_POWER_JJANG_ERROR_NO_MATCHING,//] = "ÀÏÄ¡µÈ Á¤º¸¸¦ Ã£À» ¼ö ¾ø½À´Ï´Ù.";
	UI_STRING_POWER_JJANG_ERROR_CONNECT,//] = "ÆÄ¿öÂ¯ ¼­¹ö¿Í ¿¬°á¿¡ ÀÌ»óÀÌ ÀÖ½À´Ï´Ù.";

	UI_STRING_MESSAGE_UTIL,//] = "±â´É(UTIL)";
	UI_STRING_MESSAGE_PERSONAL_STORE,//] = "°³ÀÎ »óÁ¡";
	UI_STRING_MESSAGE_POWER_JJANG,//] = "ÆÄ¿öÂ¯";

	//add by viva for NONKOWNMESSAGE
	UI_STRING_MESSAGE_NONKOWN1,//×°±¸ÉÌ³ÇµÀ¾ß
	UI_STRING_MESSAGE_NONKOWN2,//¹ºÂòÉÌ³ÇµÀ¾ß
	//end

	UI_STRING_MESSAGE_SELL_MONEY_IN_DIALOG,	  //	{  "¾ÆÀÌÅÛ ÆÈ °¡°ÝÀ» ÀÔ·ÂÇÏ¼¼¿ä." } ,
	UI_STRING_MESSAGE_PERSNALSHOP_MESSAGE,	  //	{  "¾ÆÀÌÅÛ ±¤°í¸Þ¼¼Áö " } ,
	UI_STRING_MESSAGE_PERSNALSHOP_OK,	  //	    "°³ÀÎ»óÁ¡À» ¿ÀÇÂÇÕ´Ï´Ù."
	UI_STRING_MESSAGE_PERSNALSHOP_CANCEL,	  //	"°³ÀÎ»óÁ¡À» ´Ý½À´Ï´Ù."
	UI_STRING_MESSAGE_PERSNALSHOP_WRITE_MESSAGE,	//"°³ÀÎ»óÁ¡ ±¤°í¸Þ¼¼Áö ÀÛ¼º"
	
	UI_STRING_MESSAGE_CANNOT_FIND_STORE,//] = "ÇØ´ç ÆÇ¸ÅÀÚ¸¦ Ã£À» ¼ö ¾ø½À´Ï´Ù.";
	UI_STRING_MESSAGE_STORE_CLOSED,//] = "»óÁ¡ÀÌ ÀÌ¹Ì ´ÝÇû½À´Ï´Ù.";
	UI_STRING_MESSAGE_ITEM_NOT_FOUND,//] = "ÇØ´ç ¾ÆÀÌÅÛÀÌ ÀÌ¹Ì ÆÇ¸ÅµÇ¾ú°Å³ª ÆÇ¸ÅÀÚ¿¡ ÀÇÇØ Ã¶È¸µÇ¾ú½À´Ï´Ù.";
	UI_STRING_MESSAGE_TOO_MUCH_MONEY,//] = "ÆÇ¸ÅÀÚ°¡ ³Ê¹« ¸¹Àº µ·À» °¡Áö°í ÀÖ¾î¼­ »ì ¼ö ¾ø½À´Ï´Ù.";
	UI_STRING_MESSAGE_ALREADY_DISPLAYED,//] = "ÀÌ¹Ì Áø¿­µÈ ¾ÆÀÌÅÛÀÔ´Ï´Ù.";
	UI_STRING_MESSAGE_PERSNAL_DEFAULT_MESSGE,//] = "´Ù¾çÇÑ »óÇ°ÀÌ ÀÖ½À´Ï´Ù.";
	
	UI_STRING_MESSAGE_GAMEMONEY_WITH_HANGUL,//] = "°ÔÀÓ ¸Ó´Ï ÇÑ±Û º´Çà Ç¥½Ã";
	UI_STRING_MESSAGE_ADVANTE_0,//È£·ç½º 	 (Horus)
	UI_STRING_MESSAGE_ADVANTE_1,//¼¼Æ® 	 (Seth)
	UI_STRING_MESSAGE_ADVANTE_2,//¸¶¾ÆÆ® 	 (Maat)
	UI_STRING_MESSAGE_ADVANTE_3,//¿À½Ã¸®½º (Osiris)
	UI_STRING_MESSAGE_ADVANTE_4,//ÅäÆ®	 (Thoth)
	UI_STRING_MESSAGE_ADVANTE_5,//´©Æ® 	 (Nut)
	UI_STRING_MESSAGE_ADVANTE_6,//°Ôºê 	 (Geb )
	UI_STRING_MESSAGE_ADVANTE_7,//½´	  (Shu)
	UI_STRING_MESSAGE_ADVANTE_8,//¶ó 	 (Ra)
	UI_STRING_MESSAGE_ADVANTE_9,//ÇÁÅ¸ 	 (Ptah)
	UI_STRING_MESSAGE_ADVANTE_10,//´« 	 (Nun)

	UI_STRING_MESSAGE_EQUIP_PDA,//] = "pda ¸¦ ÀåÂøÇÕ´Ï´Ù.";
	UI_STRING_MESSAGE_EQUIP_SHOULDER,//] = "¼î¿Ã´õ¸¦ ÀåÃ¢ÇÕ´Ï´Ù.";

	UI_STRING_MESSAGE_EQUIP_DERMIS,//] = "¹®½ÅÀ» ÀåÂøÇÕ´Ï´Ù.";
	UI_STRING_MESSAGE_EQUIP_PERSONA,//] = "°¡¸éÀ» ÀåÃ¢ÇÕ´Ï´Ù.";

	UI_STRING_MESSAGE_EQUIP_FASCIA,//] = "Çã¸®Àå½ÄÀ» ÀåÂøÇÕ´Ï´Ù.";
	UI_STRING_MESSAGE_EQUIP_MITTEN,//] = "Àå°©À» ÀåÃ¢ÇÕ´Ï´Ù.";
	UI_STRING_MESSAGE_CHINGHO,//] = "ÄªÈ£.";
	
	UI_STRING_MESSAGE_SWAP_ADVANCEMENT_ITEM,//] =	"½ÂÁ÷ ¾ÆÀÌÅÛÀ¸·Î ±³È¯ÇÏ½Ç ¾ÆÀÌÅÛÀ» Å¬¸¯ÇØÁÖ¼¼¿ä.";
	UI_STRING_MESSAGE_SWAP_CONFIRM,//] = "½ÂÁ÷ ¾ÆÀÌÅÛÀ¸·Î ±³È¯ ÇÏ½Ã°Ú½À´Ï±î?";
	UI_STRING_MESSAGE_SWAP_ERROR,//] = "½ÂÁ÷ÇÏÁö ¾Ê¾Æ¼­ ¾ÆÀÌÅÛ ±³È¯ÀÌ ¾ÈµË´Ï´Ù.";
	UI_STRING_MESSAGE_SWAP_ADVANCEMENT_ITEM_ERROR,//] = "½ÂÁ÷¾ÆÀÌÅÛÀ¸·Î ±³È¯ÇÒ ¼ö ¾ø½À´Ï´Ù.";

	UI_STRING_MESSAGE_CAMPAIGN_HELP_REQUEST,//] = "±âºÎ ÇÏ½Ç ¼º±ÝÀ» ÀÔ·ÂÇØ ÁÖ½Ê½Ã¿ä.";
	UI_STRING_MESSAGE_CAMPAIGN_HELP_THANKS,//] = "±âºÎÇØ ÁÖ½Å ±Ý¾×Àº ¿ÂÁ¤ÀÇ ¼Õ±æÀÌ ÇÊ¿äÇÑ ºÒ¿ìÇÑ ÀÌ¿ôÀ» À§ÇØ ¼ÒÁßÇÏ°Ô ¾²¿©Áú °ÍÀÔ´Ï´Ù. ¸ð±Ý¿¡ Âü¿©ÇØ ÁÖ¼Å¼­ °¨»çÇÕ´Ï´Ù.";
	UI_STRING_MESSAGE_CAMPAIGN_HELP_UNITS_SLAYER,//] = "¸¸¿ø";
	UI_STRING_MESSAGE_CAMPAIGN_HELP_UNITS_VAMPIRE,//] = "¸¸¿ø";
	UI_STRING_MESSAGE_CAMPAIGN_HELP_UNITS_OUSTERS,//] = "¸¸¿ø";

	UI_STRING_MESSAGE_REQUIRE_ADVANCEMENT_LEVEL_0,//] = "È£·ç½º %dµî±Þ ÀÌ»ó";
	UI_STRING_MESSAGE_REQUIRE_ADVANCEMENT_LEVEL_1,//] = "¼¼Æ® %dµî±Þ ÀÌ»ó";
	UI_STRING_MESSAGE_REQUIRE_ADVANCEMENT_LEVEL_2,//] = "¸¶¾ÆÆ® %dµî±Þ ÀÌ»ó";
	UI_STRING_MESSAGE_REQUIRE_ADVANCEMENT_LEVEL_3,//] = "¿À½Ã¸®½º %dµî±Þ ÀÌ»ó";
	UI_STRING_MESSAGE_REQUIRE_ADVANCEMENT_LEVEL_4,//] = "ÅäÆ® %dµî±Þ ÀÌ»ó";
	UI_STRING_MESSAGE_REQUIRE_ADVANCEMENT_LEVEL_5,//] = "´©Æ® %dµî±Þ ÀÌ»ó";
	UI_STRING_MESSAGE_REQUIRE_ADVANCEMENT_LEVEL_6,//] = "°Ôºê %dµî±Þ ÀÌ»ó";
	UI_STRING_MESSAGE_REQUIRE_ADVANCEMENT_LEVEL_7,//] = "½´ %dµî±Þ ÀÌ»ó";
	UI_STRING_MESSAGE_REQUIRE_ADVANCEMENT_LEVEL_8,//] = "¶ó %dµî±Þ ÀÌ»ó";
	UI_STRING_MESSAGE_REQUIRE_ADVANCEMENT_LEVEL_9,//] = "ÇÁÅ¸ %dµî±Þ ÀÌ»ó";
	UI_STRING_MESSAGE_REQUIRE_ADVANCEMENT_LEVEL_10,//] = "´« %dµî±Þ ÀÌ»ó";
	
	UI_STRING_MESSAGE_ADVANCEMENT_JOB_BLADE,//] = "½ºÇÃ¸®ÅÍ";
	UI_STRING_MESSAGE_ADVANCEMENT_JOB_SWORD,//] = "µðÆæ´õ";
	UI_STRING_MESSAGE_ADVANCEMENT_JOB_SOLDER,//] = "Çìºñ½´ÅÍ";
	UI_STRING_MESSAGE_ADVANCEMENT_JOB_HEAL,//] = "ÇÁ¸®½ºÆ®";
	UI_STRING_MESSAGE_ADVANCEMENT_JOB_ENCHANT,//] = "±×·£ÅÍ";
	UI_STRING_MESSAGE_ADVANCEMENT_JOB_VAMPIRE,//] = "¹ìÇÁ³ëºí";
	UI_STRING_MESSAGE_ADVANCEMENT_JOB_COMBAT,//] = "Äí½ºÅä½º";
	UI_STRING_MESSAGE_ADVANCEMENT_JOB_EARTH,//] = "ÀÌ±×´Ï¼­ ";
	UI_STRING_MESSAGE_ADVANCEMENT_JOB_FIRE,//] = "¾ÆÄí¾È ";
	UI_STRING_MESSAGE_ADVANCEMENT_JOB_WATER,//] = "Å×¶ó³ë½º ";

	UI_STRING_MESSAGE_DO_NOT_SHOW_PERSNALSHOP_MSG,//] = "°³ÀÎ »óÁ¡ ¸Þ¼¼Áö ¾È º¸±â";

	UI_STRING_MESSAGE_REQUEST_GET_EVENT_ITEM,//] = "Å×½ºÆ®:ÀÌº¥Æ® ¾ÆÀÌÅÛÀ» ¹ÞÀ¸½Ã°Ú½À´Ï±î?";

	UI_STRING_MESSAGE_GET_EVENT_ITEM_RECEIVE_OK,//] = "Å×½ºÆ®:ÀÌº¥Æ® ¾ÆÀÌÅÛÀ» ¹Þ¾Ò½À´Ï´Ù.";
	UI_STRING_MESSAGE_GET_EVENT_ITEM_RECEIVE_ALREADY,//] = "Å×½ºÆ®:ÀÌ¹Ì ÀÌº¥Æ® ¾ÆÀÌÅÛÀ» ¼ö·ÉÇÏ¼Ì½À´Ï´Ù.";
	UI_STRING_MESSAGE_GET_EVENT_ITEM_RECEIVE_FAIL,//] = "Å×½ºÆ®:ÀÌº¥Æ® ¾ÆÀÌÅÛ ¹Þ±â ½ÇÆÐ.";
	UI_STRING_MESSAGE_GET_EVENT_ITEM_NOT_EVENT_USER,//] = "Å×½ºÆ®:ÀÌº¥Æ® ÇØ´ç À¯Àú°¡ ¾Æ´Õ´Ï´Ù.";

	UI_STRING_MESSAGE_GET_EVENT_ITEM_NOTICE,//] = "Å×½ºÆ®:ÈÞ¸é °èÁ¤ ¾ÆÀÌÅÛ ¹Þ±â ÀÌº¥Æ®¸¦ ÇÏ½Ç ¼ö ÀÖ½À´Ï´Ù.";


	UI_STRING_MESSAGE_FAIL_OPEN_WEBPAGE,//] = "À¥ ÆäÀÌÁö¸¦ ¿­ ¼ö ¾ø½À´Ï´Ù.";
	UI_STRING_MESSAGE_CANNOT_USE_ADVANCEMENTCLASS,//] = "½ÂÁ÷ÇÑ À¯Àú´Â »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù.";

	UI_STRING_MESSAGE_CANNOT_USE_HIGH_GRADE,//] = "6±Þ ÀÌ»ó ¾ÆÀÌÅÛÀº ÀÎÃ¦Æ® ÇÒ ¼ö ¾ø½À´Ï´Ù.";
	UI_STRING_MESSAGE_CANNOT_USE_ADVANCEMENT_ITEM,//] = "½ÂÁ÷ Àü¿ë ¾ÆÀÌÅÛÀº ÀÎÃ¦Æ® ÇÒ ¼ö ¾ø½À´Ï´Ù.";
	UI_STRING_MESSAGE_CANNOT_USE_OVER_TWO_OPTION,//] = "¿É¼ÇÀÌ 2°³ ÀÌ»óÀÎ ¾ÆÀÌÅÛÀº ÀÎÃ¦Æ® ÇÒ ¼ö ¾ø½À´Ï´Ù.";
	STRING_ERROR_IP_DENY,
	UI_STRING_MESSAGE_CHECK_VERSION_ERROR,
	STRING_STATUS_NOT_FIND_SKILL_CRAD,
	UI_STRING_MESSAGE_SYSTEM,
	UI_STRING_MESSAGE_PLAYER_SAY,
	UI_STRING_NO_ITEM_MESSAGE,				//Ã»ÓÐÎïÆ·
	//add by viva : friend button description
//	UI_STRING_MESSAGE_FRIEND,
//	UI_STRING_MESSAGE_FRIEND_LIST,
//	UI_STRING_MESSAGE_FRIEND_BLACKLIST,
//	UI_STRING_MESSAGE_FRIEND_TAG,
//	UI_STRING_MESSAGE_FRIEND_UP,
//	UI_STRING_MESSAGE_FRIEND_DOWN,
//	UI_STRING_MESSAGE_FRIEND_FIND,
//	UI_STRING_MESSAGE_FRIEND_ADD,
//	UI_STRING_MESSAGE_FRIEND_SETUP,
//	//add by viva : ask_friend_dialog
//	UI_STRING_ASK_FRIEND_REQUEST,
//	UI_STRING_ASK_FRIEND_REFUSE,
//	UI_STRING_ASK_FRIEND_WAIT,
//	UI_STRING_ASK_FRIEND_EXSIT,
//	UI_STRING_ASK_FRIEND_BLACK,
//	UI_STRING_ASK_FRIEND_DELETE,
	//end
	MAX_GAME_STRING,
};



#endif
