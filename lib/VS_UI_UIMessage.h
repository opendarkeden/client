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

	MAX_UI_MESSAGE
};

#endif