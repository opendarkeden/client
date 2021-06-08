#ifndef __VS_UI_UIMESSAGE_H__
#define __VS_UI_UIMESSAGE_H__

// message
enum UI_MESSAGE
{
	INVALID_MESSAGE,

	UI_STORAGE_BUY,
	UI_SELECT_STORAGE_SLOT,
	UI_RUNNING_GAMEMENU,
	UI_FINISH_LEVELUP_BUTTON,
	UI_CLICK_BONUS_POINT,
	UI_INFO,

//	UI_MINIMAP_TOGGLE,
	UI_DROP_MONEY,
	UI_EXCHANGE_REQUEST_CANCEL,
	UI_EXCHANGE_ACCEPT,
	UI_EXCHANGE_MONEY,
	UI_WITHDRAW_MONEY,
	UI_ITEM_SELECT_EXCHANGE,	
	UI_DEPOSIT_MONEY,
	UI_CHECK_EXIST_ID,
	UI_RUN_NEWUSER_REGISTRATION,
	UI_INFO_CLOSE,
	UI_GAMEMENU_CONTINUE,
/*
	//
	// PDS
	//
	UI_CHANGE_PCS_CONNECTED_SLOT,		// 이미 연결된 다른 사람을 click하였다.
	UI_PLEASE_PCS_CONNECT_ME,			// 사람이 있는 wait room을 click하였다.
	UI_PCS_CONNECTOR_GRANTED,			// wait room에 있는 connector와 연결을 승낙하였다.
	UI_QUIT_PCS_ONLINE_MODE,			// PCS online mode에서 quit 버튼을 눌렀다.
	UI_END_PCS,								// sending 하고 있을 때, end를 눌렀다.
	UI_SEND_PCS_NUMBER,
	UI_PDS_CLOSED,							// slayer pds가 닫혔다. !현재 연결은 유지한다.

	UI_PLEASE_SET_SLAYER_VALUE,		// slayer pds에서 gage menu를 실행하였다.
*/
//	UI_LEARN_SLAYER_SKILL,				// skill tree에서 배울 수 있는 skill을 선택하였다.
//	UI_CLOSE_SKILL_TREE,
//	UI_CLOSE_SKILL_VIEW,

	UI_BACKGROUND_MOUSE_FOCUS,			// Mouse pointer가 UI Window에서 이동없이 일을 때, 그 Window가 사라질 경우 Client에 전달.
	UI_REMOVE_BACKGROUND_MOUSE_FOCUS,// Game Menu와 같은 topmost Window가 출력되었을 때는 background focus를 제거하는 것이 좋다.

	//
	// 상점 Message
	//
	UI_BUY_ITEM,

	UI_SELL_ITEM,
	UI_REPAIR_ITEM,
	UI_SILVERING_ITEM,
	UI_ITEM_SELL_FINISHED,
	UI_ITEM_REPAIR_FINISHED,
	UI_ITEM_SILVERING_FINISHED,

	UI_CLOSE_SHOP,
	UI_CLOSE_STORAGE,
	UI_CLOSE_EXCHANGE,
	UI_CANCEL_EXCHANGE,
	UI_OK_EXCHANGE,

	UI_CHARACTER_MANAGER_FINISHED,	// Characters Window에서 초기화면으로 갈 때 알려준다.
	UI_TERMINATION,						// 종료
	UI_LOGIN,								// 사용자 login 요청
	UI_LOGOUT,								
//	UI_NEW_USER_REGISTRATION,			// 새 사용자 등록 요청
	UI_CONNECT,							   // Game connection.
	UI_CHAT_RETURN,						// 채팅창 입력.
//	UI_CHAT_SELECT_NAME,				// 채팅창에서 선택 주석 by larosel
	UI_SELECT_SKILL,						// Skill selection.
	UI_CANCEL_SELECT_SKILL,				// Skill 선택이 취소되었다.

	//
	// Item message
	//

	// drop
	UI_ITEM_DROP_TO_CLIENT,				// Item을 바닥에 떨어뜨렸다.
	UI_ITEM_DROP_TO_INVENTORY, 		// Inventory에 놓는다.
	//UI_ITEM_DROP_TO_TRADEGRID,			// 교환창 그리드에 놓는다.
	UI_ITEM_DROP_TO_GEAR,				// Gear에 놓는다.
	UI_ITEM_DROP_TO_QUICKSLOT,			// Quick Slot에 놓는다.

	// pick up
	// Client에서 집을 때는 Client -> UI 이다.
	//UI_ITEM_PICKUP_FROM_TRADEGRID,
	UI_ITEM_PICKUP_FROM_INVENTORY,
	UI_ITEM_PICKUP_FROM_GEAR,
	UI_ITEM_PICKUP_FROM_QUICKSLOT,

	// insert to other Item
	//UI_ITEM_INSERT_FROM_TRADEGRID,
	UI_ITEM_INSERT_FROM_INVENTORY,
	UI_ITEM_INSERT_FROM_GEAR,
	UI_ITEM_INSERT_FROM_QUICKSLOT,  // 샇이는 경우 - 아직 지원안함 -

	// use
	UI_ITEM_USE,							// Inventory에서 사용.
	UI_ITEM_USE_QUICKSLOT,				// Quick Item Slot에서 사용.

	//
	// Character management
	//
	UI_NEW_CHARACTER,
	UI_DELETE_CHARACTER,

	// etc
	UI_CLOSE_BBS,

	// game option
	UI_CHANGE_OPTION,
	UI_CLOSE_OPTION,

	// title option
//	UI_CHANGE_TITLE_OPTION,
//	UI_CLOSE_TITLE_OPTION,	

	// repleace	
	//UI_CHANGE_ITEM_IN_INVENTORY,	// Inventory에서 Item이 교체되었다.
	//UI_REMOVE_ITEM_FROM_GEAR,		// Gear에서 Item을 빼낸다.

	//tutorial by larosel
	UI_CLOSE_BOOKCASE,
	UI_CLOSE_BRIEFING,
	UI_CLOSE_COMPUTER,
	//UI_CONFIRM_TUTORIAL_EXIT,		// 주석 by sigi
	UI_CLOSE_TUTORIAL_EXIT,
	//UI_TUTORIAL_EXIT_ACCEPT,		// 주석 by sigi
	//UI_TUTORIAL_EXIT_SELECT,		// 주석 by sigi

	UI_CLOSE_DESC_DIALOG,
	UI_CLOSE_ELEVATOR,
	UI_SELECT_ELEVATOR,

	UI_SELL_ALL_ITEM,	//해골 모두 팔기
	
//	UI_USE_SERUM,	// 혈청사용하기

	UI_ITEM_TO_QUICKITEMSLOT, // focus_grid_x, focus_grid_y, (MItem *)p_item

	UI_CLOSE_SLAYER_PORTAL,//슬레이어 포탈 취소
	UI_SLAYER_PORTAL, //워프하쟈 left = zoneid right's hiword = x right's LOWORD = y

	UI_FINISH_REQUEST_PARTY_BUTTON,	// 파티요청 닫기 left == TRUE : timeout
	UI_PARTY_REQUEST_CANCEL, 
	UI_PARTY_ACCEPT,

	UI_CLOSE_PARTY_MANAGER,
	UI_AWAY_PARTY, // 파티 탈퇴 추방 left :: -1 : 탈퇴 0~4 : 추방

	UI_FINISH_REQUEST_DIE_BUTTON,	// 다이-_-요청 닫기 left == TRUE : timeout

	UI_CLOSE_TEAM_LIST,
	UI_CLOSE_TEAM_INFO,
	UI_CLOSE_TEAM_MEMBER_INFO,
	UI_CLOSE_TEAM_REGIST,
	UI_CLOSE_TEAM_MEMBER_LIST,
	
	UI_REQUEST_GUILD_INFO,
	UI_REQUEST_GUILD_MEMBER_LIST,
	UI_MODIFY_GUILD_MEMBER,

	UI_SELECT_TEAM_MEMBER_LIST,			// void_ptr = CHAR_NAME
	UI_SELECT_READY_TEAM_LIST,		// void_ptr = TEAM_NAME
	UI_SELECT_REGIST_TEAM_LIST,		// void_ptr = TEAM_NAME
//add by viva
	UI_CLOSE_FRIEND_CHATTING_INFO,
	UI_OPEN_FRIEND_CHATTING_INFO,
	///////friend message
	UI_FRIEND_CHATTING_SEND_MESSAGE,
	UI_FRIEND_CHATTING_UPDATE,
	UI_FRIEND_CHATTING_ADD_FRIEND,
	///////ask_friend_request
	UI_FRIEND_REQUEST_ACCEPT,
	UI_FRIEND_ASK_CLOSE,
	////////ask_friend_delete
	UI_FRIEND_DELETE_ASK,
	UI_FRIEND_DELETE_ACCEPT,
//end
	UI_JOIN_READY_TEAM,				// void_ptr = TEAM_NAME
	UI_JOIN_REGIST_TEAM,			// void_ptr = TEAM_NAME

	UI_REGIST_GUILD_MEMBER,			// void_ptr = introduction max:150byte 창 닫아줄것!
	UI_REGIST_GUILD_TEAM,			// left = TEAM_NAME, void_ptr = introduction max:150byte 창 닫아줄것!

	UI_CLOSE_FILE_DIALOG,			// left = mode, void_ptr = file_name

	UI_CONNECT_SERVER,				// left == true : group false : server right : id
									// request_server_list 안보내므로 접속하면 list보내줄것
	UI_CLOSE_SERVER_SELECT,			// left == true : group false : server

	UI_NEWCHARACTER_CHECK,				// void_ptr : name

	UI_ENCHANT_ACCEPT,
	UI_ENCHANT_CANCEL,
	
	UI_MESSAGE_BOX,					// left = 메시지 종류  STRING_MESSAGE_CANNOT_REPAIR

	//UI_OTHER_INFO,					// Other Player Info Box
	UI_CLOSE_OTHER_INFO,			//            //
	UI_MODIFY_TEAM_INFO,			// 소개글 변경  left = guild id, ptr = intro
	UI_MODIFY_GUILD_MEMBER_INTRO,	// 내 소개 글 변경 left = , ptr = intro

	UI_CLOSE_TRACE_WINDOW,			// TRACE 창 닫기.
	UI_SEND_NAME_FOR_SOUL_CHAIN,	// TRACE 이름 보내기 ptr 에

	// 넷마블용 수정
	UI_RUN_CONNECT,
	UI_SELECT_GRADE_SKILL,			// Grade Skill 보냄. left = id

	UI_USE_XMAS_TREE,				// left = ItemPointer, void_ptr = message
	UI_CLOSE_XMAS_CARD_WINDOW,
	UI_SEND_BRING_FEE,				// left = money				to client
	UI_CLOSE_BRING_FEE_WINDOW,		// close window

	UI_CLOSE_WAR_LIST_WINDOW,

	UI_CLOSE_BLOOD_BIBLE_STATUS,

	UI_SEND_NAME_FOR_COUPLE,		// left = type(커플 신청, 해지) void_ptr = name
	UI_CLOSE_INPUT_NAME,
	
	UI_ITEM_USE_GEAR,							// Gear에서 사용.	left : focus slot    void_ptr : item_ptr
	UI_GO_BILING_PAGE,						// 빌링페이지로 가자!

	UI_CLOSE_POPUP_MESSAGE,
	UI_CLOSE_QUEST_STATUS,
	UI_CLOSE_LOTTERY_CARD,
	UI_LOTTERY_CARD_STATUS,				// left = step, right = GiftID
	UI_FINISH_SCRATCH_LOTTERY,
	
	UI_CLOSE_IMAGE_NOTICE,
	
	UI_SELECT_ITEM_FROM_SHOP,
	UI_CLOSE_ITEM_LIST,
	UI_CLOSE_BULLETIN_BOARD_WINDOW,
	
	UI_TRANS_ITEM_CANCEL,
	UI_TRANS_ITEM_ACCEPT,

	UI_FINISH_REQUEST_RESURRECT_BUTTON,		// 부활

	UI_USE_MIXING_FORGE,
	UI_CLOSE_MIXING_FORGE,

	UI_CLOSE_REMOVE_OPTION,
	UI_SEND_REMOVE_OPTION,

	UI_CLOSE_OUSTERS_SKILL_INFO,
	UI_OUSTERS_LEARN_SKILL,
	
	UI_RUN_LEVELUP,
	
	UI_CLOSE_HORN,
	UI_OUSTERS_DOWN_SKILL,
	
	UI_CLOSE_FINDING_MINE,
	UI_CLOSE_NEMONEMO,
	UI_CLOSE_PUSHPUSH,
	UI_CLOSE_CRAZY_MINE,
	UI_CLOSE_ARROW_TILE,
	UI_CLEAR_STAGE,					// left = (minigame|stage) right = time
	UI_CLEAR_ALL_STAGE,
	UI_REQUEST_FORCE_DIE,			// 강제로 죽어!
	UI_ADD_ITEM_TO_CODE_SHEET,		// left = x , right = y, ptr = item
	
	UI_SEND_BUG_REPORT,				// left = line, ptr = string
	UI_GO_TO_BEGINNER_ZONE,

	UI_POPUP_MESSAGE_OK,			// left = ID, right = selectedID
	UI_CLOSE_SHRINE_MINIMAP,		// left ?
	UI_WARP_REGEN_TOWER,			// left id num;

	UI_CLOSE_MAILBOX,
	
	UI_CLOSE_PET_INFO,
	UI_PET_GAMBLE,
	UI_CLOSE_USE_PET_FOOD,
	
	// 2004, 5, 11 sobeit add start - 펫 보관소 관련
	UI_CLOSE_PETSTORAGE,
	UI_CLOSE_KEEP_PETITEM,
	UI_CLOSE_GET_KEEP_PETITEM,
	UI_KEEP_PETITEM,
	UI_GET_KEEP_PETITEM,
	// 2004, 5, 11 sobeit add end

	// 2004, 5, 27, sobeit add start - sms 관련
	UI_CLOSE_SMS_MESSAGE,
	UI_SEND_SMS_MESSAGE,
	UI_SMS_OPEN_LIST,
	UI_SMS_RECORD,
	UI_SMS_DELETE,
	UI_CLOSE_SMS_LIST,
	UI_CLOSE_SMS_RECORD,
	UI_SMS_ADD_SEND_LIST,
	// 2004, 5, 27, sobeit add end - sms 관련

	// 2004, 6, 15 sobeit add start- naming 관련
	UI_CLOSE_NAMING,
	UI_CHANGE_CUSTOM_NAMING,
	UI_SELECT_NAMING,
	UI_CLOSE_NAMING_CHANGE,
	UI_RUN_NAMING_CHANGE,
	// 2004, 6, 15 sobeit add end
	
	// 2004, 7, 5, sobeit add start
	UI_CLOSE_QUEST_MANAGER,
	UI_CLOSE_QUEST_LIST,
	UI_CLOSE_QUEST_DETAIL,
	UI_CLOSE_QUEST_MISSION,
	UI_CLOSE_QUEST_ITEM,
	UI_CLOSE_QUEST_ICON,
	UI_GQUEST_ACCEPT,
	UI_GQUEST_GIVEUP,
	
	// 2004, 7, 5, sobeit add end

	UI_ITEM_USE_GQUEST_ITEM,

	UI_REQUEST_UNION,
	UI_REQUEST_UNION_ACCEPT,
	UI_REQUEST_UNION_DENY,
	UI_REQUEST_UNION_REQUEST_DENY,
	UI_REQUEST_UNION_REQUEST_INFO,
	UI_REQUEST_UNION_EXPERGUILD,
	UI_REQUEST_UNION_QUIT,
	UI_REQUEST_UNION_QUIT_ACCEPT,
	UI_REQUEST_UNION_QUIT_DENY,

	UI_REQUEST_UNION_REQUEST_GUILD_LIST,

	UI_RECALL_BY_NAME,
	
	UI_MODIFY_TAX,
	UI_APPOINT_SUBMASTER,
	
	UI_CLOSE_PERSNALSHOP,
	UI_SELECT_PERSNALSHOP_SLOT,
	UI_CLOSE_PERSNALSHOP_MESSAGE,
	UI_INPUT_PERSNALSHOP_MESSAGE,
	
	UI_DISPLAY_ITEM,                     // 465  개인상점에 아이템 진열
    UI_UNDISPLAY_ITEM,                   // 466  진열된 아이템 내리기
    UI_STORE_SIGN,                       // 467  개인상점 간판 문자열
    UI_STORE_OPEN,                       // 468  개인상점 오픈
    UI_STORE_CLOSE,                      // 469  개인상점 닫기	
	UI_REQUEST_STORE_INFO,               // 470  현재 상점 정보 요청	
	UI_MY_STORE_INFO,                    // 471  자기 개인상점 정보 보내주기	
    UI_OTHER_STORE_INFO,                 // 472  다른사람 개인상점 정보 보내주기	
	
    UI_BUY_STORE_ITEM,                   // 473  다른 사람 상점에서 아이템 구입하기	
    UI_REMOVE_STORE_ITEM,                // 474  상점에서 아이템이 제거됨	
    UI_ADD_STORE_ITEM,                   // 475  상점에 아이템이 추가됨	

	// 2004, 11, 30, sobeit add start
	UI_CLOSE_POWER_JJANG,
	UI_REQUEST_POWER_JJANG_POINT,
	UI_POWER_JJANG_GAMBLE,
	// 2004, 11, 30, sobeit add end
	
	UI_CLOSE_SWAPADVANCEMENTITEM,
	UI_SWAPADVANCEMENTITEM,
	UI_LEARN_ADVANCE_SKILL,

	UI_CAMPAIGN_HELP, // 불우이웃돕기 성금 입력 ok

	UI_RUN_NEXT_GQUEST_EXCUTE_ELEMENT, // 다음 퀘스트 메세지 보여주기 
	
	UI_GQUEST_SET_ACTION,		// 퀘스트 시 특정 동작을 보여줘야 할 경우 
	UI_GQUEST_ENDING_EVENT,		// 퀘스트 종료시 엔딩 이벤트가 있을 경우
	
	UI_REQUEST_EVENT_ITEM,		// 휴면 계정 아이템 받기 이벤트 시 아이템 받을 요청을 한다.

	UI_CLOSE_INVENTORY_SUB,		// sub inventory 를 닫는다.
	UI_ITEM_DROP_TO_INVENTORY_SUB, // sub inventory 에 아이템 드롭
	UI_ITEM_PICKUP_FROM_INVENTORY_SUB,// sub inventory 에 아이템 픽업

	UI_ITEM_USE_SUBINVENTORY,
	MAX_UI_MESSAGE
};

#endif