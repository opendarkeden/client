//----------------------------------------------------------------------
// ClientConfig.h
//----------------------------------------------------------------------
// Client의 정보 중에서 변할 수 있는 것
//----------------------------------------------------------------------

#ifndef	__CLIENTCONFIG_H__
#define	__CLIENTCONFIG_H__

#include <Windows.h>
#include "MString.h"

class ClientConfig {
	public :
		enum NUR_MODE
		{
			NUR_CLIENT,				// 새 사용자 등록 가능
			NUR_HOMEPAGE,			// 홈페이지를 띄워주면서 등록
			NUR_MESSAGE_HOMEPAGE,	// 홈페이지에서 등록하라는 message
			NUR_DENY				// 새 사용자 등록 불가			
		};

	public :
		ClientConfig();
		~ClientConfig();

		//--------------------------------------------------------
		// File I/O
		//--------------------------------------------------------
		void	SaveToFile(const char* filename);
		bool	LoadFromFile(const char* filename);

	public :
		//--------------------------------------------------------------
		// 초당 Update Frame 수
		//--------------------------------------------------------------
		int		FPS;						// 적용 시키지는 않는다.

		int		MAX_SCREENSHOT;				// ScreenShot화일 최대 개수
		int		DELAY_PLAYER_DEAD;			// player가 죽고나서의 delay
		int		DELAY_PLAYER_RESURRECT_TIMEOUT;			// player가 죽고나서의 강제 부활까지의 시간
		int		MAX_CLIENT_MOVE;			// client에서 움직일 수 있는 sector수
		int		MAX_CREATURE_MOVE_BUFFER;	// creature들 움직이는 max move buffer 
		int		MAX_LIGHT_DRAW;				// 화면 상의 최대 light개수

		int		MAX_SOUNDPART;				// load하는 wav개수

		//--------------------------------------------------------------
		// RequestService
		//--------------------------------------------------------------
		int		MAX_REQUEST_SERVICE;		// RequestServerPlayer 접속 수

		//--------------------------------------------------------------
		// packet관련
		//--------------------------------------------------------------
		int		MAX_WAIT_PACKET;			// packet기다리는 시간
		int		MAX_PROCESS_PACKET;			// 한번에 처리하는 packet개수
		
		//--------------------------------------------------------------
		// 최대 한번에 update되는 회수
		//--------------------------------------------------------------
		int		MAX_UPDATE_ONETIME;
		int		MAX_UPDATE_ONETIME_COUNT;	// onetime_update의 최대 회수(갑자기 달리기? -_-;)

		//--------------------------------------------------------------
		// 외치기 delay 시간
		//--------------------------------------------------------------
		int		DELAY_GLOBAL_SAY;		

		//--------------------------------------------------------------
		// title화면 음악
		//--------------------------------------------------------------
		int		MUSIC_THEME;

		//--------------------------------------------------------------
		// message scroll 시간
		//--------------------------------------------------------------
		int		DELAY_SYSTEMMESSAGE;
		int		DELAY_GAMEMESSAGE;

		//--------------------------------------------------------------
		// 화면에 출력되는 item이름 개수
		//--------------------------------------------------------------
		int		MAX_DRAWITEMNAME;

		//--------------------------------------------------------------
		// 채팅 String 줄 수
		//--------------------------------------------------------------
		int		MAX_CHATSTRING;
		int		MAX_CHATSTRING_MINUS_1;
		int		MAX_CHATSTRING_LENGTH;
		int		MAX_CHATSTRINGLENGTH_PLUS1;

		//--------------------------------------------------------------
		// 채팅 보여지는 시간..
		//--------------------------------------------------------------
		int		DELAY_CHATSTRING_KEEP;		// 흰색으로 표현하는 시간
		int		DELAY_CHATSTRING_FADE;		// 어두워지는 단계의 시간
		int		VALUE_CHATSTRING_FADE;		// 어두워지는 값
		int		MIN_CHATSTRING_COLOR256;	// 가장 어두운 채팅 String색

		//--------------------------------------------------------
		// TexturePart Manager의 Part 개수
		//--------------------------------------------------------
		int		MAX_TEXTUREPART_EFFECT;
		int		MAX_TEXTUREPART_SCREENEFFECT;
		int		MAX_TEXTUREPART_CREATURESHADOW;
		int		MAX_TEXTUREPART_IMAGEOBJECTSHADOW;
		int		MAX_TEXTUREPART_ADDON_SHADOW;
		int		MAX_TEXTUREPART_IMAGEOBJECTSPK;
		int		MAX_TEXTUREPART_EFFECTSHADOW;

		//--------------------------------------------------------
		// Font 크기
		//--------------------------------------------------------
		int		FONT_HEIGHT;
		int		FONT_ITEM_HEIGHT;

		//--------------------------------------------------------
		// 캐릭터 이름 + HP 표시
		//--------------------------------------------------------
		int		MAX_HP_BAR_PIXEL;			// hp bar의 크기
		int		POSITION_HP_BAR;		// 이름의 위치(hp bar 위치)

		//--------------------------------------------------------------
		// Item 부서지는 상태 표시
		//--------------------------------------------------------------
		int		PERCENTAGE_ITEM_SOMEWHAT_BROKEN;
		int		PERCENTAGE_ITEM_ALMOST_BROKEN;

		//--------------------------------------------------------------
		// 캐릭터에 붙는 Effect보여줄때 가끔 원래 색깔 보여주는 Frame
		//--------------------------------------------------------------
		int		FRAME_DRAW_ORIGINAL_SPRITE;

		//--------------------------------------------------------
		// 색깔
		//--------------------------------------------------------			
		WORD	COLOR_OUTLINE_INTERACTIONOBJECT;

		// item
		COLORREF	COLOR_NAME_ITEM;
		COLORREF	COLOR_NAME_ITEM_OPTION;
		COLORREF	COLOR_NAME_ITEM_RARE_OPTION;
		WORD		COLOR_OUTLINE_ITEM;

		// 종족별
		COLORREF	COLOR_NAME_VAMPIRE;
		COLORREF	COLOR_NAME_SLAYER;
		COLORREF	COLOR_NAME_NPC;

		// 공격 가능?
		WORD		COLOR_OUTLINE_NPC;
		WORD		COLOR_OUTLINE_ATTACK_POSSIBLE;
		WORD		COLOR_OUTLINE_ATTACK_IMPOSSIBLE;

		// HP bar의 색깔
		BYTE		COLOR_HP_BAR_R;
		BYTE		COLOR_HP_BAR_G;
		BYTE		COLOR_HP_BAR_B;

		//--------------------------------------------------------
		// 새 사용자 등록 mode
		//--------------------------------------------------------
		NUR_MODE	NEW_USER_REGISTERATION_MODE;
		MString		URL_HOMEPAGE;				// 홈페이지 main
		MString		URL_HOMEPAGE_NEW_USER;		// 새 사용자 등록 URL
		MString		URL_HOMEPAGE_BILING;

		//--------------------------------------------------------
		// 성향에 따른 색깔 - 2001.8.2
		//--------------------------------------------------------
		COLORREF	COLOR_NAME_GOOD_MORE;
		COLORREF	COLOR_NAME_GOOD;
		COLORREF	COLOR_NAME_NEUTRAL;
		COLORREF	COLOR_NAME_EVIL;
		COLORREF	COLOR_NAME_EVIL_MORE;

		//--------------------------------------------------------
		// 피 흘리기
		//--------------------------------------------------------
		int			BLOOD_DROP_HP_PERCENT;		// 피 흘리기 시작하는 HP %
		DWORD		BLOOD_DROP_GAP_TIME;		// 다음에 피 흘리기 시작하는 시간 gap
		DWORD		BLOOD_DROP_RANDOM_TIME;		// 피 흘리는 시간 = GAP + RANDOM

		//--------------------------------------------------------
		// 시간
		//--------------------------------------------------------
		DWORD		AFTER_TRADE_ITEM_DROP_DELAY;	// trade후 item떨어지는게 가능해지는 시간
		DWORD		AFTER_PARTY_KICK_DELAY;			// 파티 join후 다른사람 kick할 수 있는 시간
		DWORD		TRACE_CHARACTER_LIMIT_TIME;		// 캐릭터 따라가기 제한 시간.

		//--------------------------------------------------------
		// Repeat & Lock
		//--------------------------------------------------------
		DWORD		REPEAT_TIME;				// 자동공격 제한시간
		DWORD		LOCK_TIME;					// Caps Lock자동공격 제한시간

		//--------------------------------------------------------------
		// Client통신에 사용하는 UDP Port
		//--------------------------------------------------------------
		DWORD		CLIENT_COMMUNICATION_UDP_PORT;

		//--------------------------------------------------------------
		// Client통신과 관련된 delay들
		//--------------------------------------------------------------
		DWORD		CLIENT_COMMUNICATION_STATUS_DELAY;

		//--------------------------------------------------------------
		// Trade할 때 delay 시간
		//--------------------------------------------------------------
		DWORD		TRADE_ACCEPT_DELAY_TIME;

		//--------------------------------------------------------------
		// Regen량
		//--------------------------------------------------------------
		BYTE		REGEN_AMOUNT_BURROW;
		BYTE		REGEN_AMOUNT_CASKET;
		BYTE		REGEN_AMOUNT_VAMPIRE;

		//--------------------------------------------------------------
		// 초당 소리낼 수 있는 wave 개수 제한
		//--------------------------------------------------------------
		BYTE		MAX_SOUND_PER_SECOND;

		DWORD		UniqueItemColorSet;
		DWORD		QuestItemColorSet;

		//--------------------------------------------------------------
		// HPModifyList 개수제한 및 시간제한
		//--------------------------------------------------------------
		BYTE		HPModifyListMax;
		DWORD		HPModifyListTime;
};


extern	ClientConfig*	g_pClientConfig;

#endif

