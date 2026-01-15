//----------------------------------------------------------------------
// ClientConfig.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "ClientConfig.h"
#include "MusicDef.h"
#include <fstream>
using namespace std;

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
ClientConfig*	g_pClientConfig = NULL;

//----------------------------------------------------------------------
// define functions
//----------------------------------------------------------------------
#define READ_CHECK_EOF( value, temp, bytes )		\
		{											\
			file.read((char*)&temp, bytes);			\
			if (!file.eof())						\
			{										\
				value = temp;						\
			}										\
			else									\
			{										\
				return true;						\
			}										\
		}

//----------------------------------------------------------------------
// 
// constructor
//
//----------------------------------------------------------------------
ClientConfig::ClientConfig()
{	
	//--------------------------------------------------------------
	// FPS - 적용은 안된다.
	//--------------------------------------------------------------
	FPS						= 16;
 
	// ScreenShot의 개수
	MAX_SCREENSHOT			= 100;

	// 죽고 나서 다시 살아나는데까지의 delay시간 10초
	DELAY_PLAYER_DEAD		= 10 * 1000;	

	DELAY_PLAYER_RESURRECT_TIMEOUT = 60 * 1000;		// 1분
	
	// client에서 움직일 수 있는 sector수
	MAX_CLIENT_MOVE			= 6;

	// client에서 움직일 수 있는 sector수
	MAX_CREATURE_MOVE_BUFFER	= 2;
	
	// 화면의 최대 Light개수
	MAX_LIGHT_DRAW			= 50;

	// Load하는 Sound 개수
	MAX_SOUNDPART			= 100;

	MAX_REQUEST_SERVICE		= 10;

	//--------------------------------------------------------------
	// 한번에 udpate하는 최대 회수
	//--------------------------------------------------------------
	// 한번에 최대 6 Frame의 의미는
	// 6 Frame마다 적어도 한번은 그려준다는 의미가 된다.
	//--------------------------------------------------------------
	MAX_UPDATE_ONETIME		= 6;

	//--------------------------------------------------------------
	// '한번에 update하는 최대 회수'를 수행하는 회수.. - -;;
	//--------------------------------------------------------------
	// 이렇게 하면..
	// 한번에 최대 6 Frame씩, 최대 "10"회니까..
	// 60 Frame동안 표현될 것이.. 순간적~~으로 표현되게 된다.
	// 10 Frame만 보이겠찌.. 
	//--------------------------------------------------------------
	MAX_UPDATE_ONETIME_COUNT		= 10;		

	DELAY_GLOBAL_SAY				= 12*1000;	// 12초

	//--------------------------------------------------------------
	// message scroll delay시간
	//--------------------------------------------------------------
	DELAY_SYSTEMMESSAGE		= 5000;
	DELAY_GAMEMESSAGE		= 5000;

	//--------------------------------------------------------------
	// 화면에 출력되는 item이름 개수
	//--------------------------------------------------------------
	MAX_DRAWITEMNAME			= 20;

	//--------------------------------------------------------------
	// packet기다리는 시간
	//--------------------------------------------------------------
	MAX_WAIT_PACKET			= 15000;	// 15초

	//--------------------------------------------------------------
	// 한 loop에 처리되는 packet의 최대 수
	//--------------------------------------------------------------
	MAX_PROCESS_PACKET			= 11;		// 15개 처리

	//--------------------------------------------------------------
	// 초기화면의 음악
	//--------------------------------------------------------------
	MUSIC_THEME				= MUSIC_TREASURE;

	//--------------------------------------------------------------
	// 채팅 String 줄 수
	//--------------------------------------------------------------
	MAX_CHATSTRING				= 5;
	MAX_CHATSTRING_MINUS_1		= 4;
	MAX_CHATSTRING_LENGTH		= 20;
	MAX_CHATSTRINGLENGTH_PLUS1	= 21;

	//--------------------------------------------------------------
	// 채팅 유지 시간
	//--------------------------------------------------------------
	DELAY_CHATSTRING_KEEP		= 1500;		// 흰색의 지속 시간
	DELAY_CHATSTRING_FADE		= 250;		// 어두워지는 단계의 시간
	VALUE_CHATSTRING_FADE		= 8;		// 어두워지는 값
	MIN_CHATSTRING_COLOR256		= 96;		// 가장 어두운 채팅 String색
	
	//--------------------------------------------------------
	// TexturePart Manager의 Part 개수
	//--------------------------------------------------------
	MAX_TEXTUREPART_EFFECT				= 25;
	MAX_TEXTUREPART_CREATURESHADOW		= 100;
	MAX_TEXTUREPART_IMAGEOBJECTSHADOW	= 100;
	MAX_TEXTUREPART_ADDON_SHADOW			= 200;
	MAX_TEXTUREPART_IMAGEOBJECTSPK		= 20;
	MAX_TEXTUREPART_SCREENEFFECT		= 25;

	//--------------------------------------------------------
	// Font 크기
	//--------------------------------------------------------
	FONT_HEIGHT				= 20;
	FONT_ITEM_HEIGHT			= 15;

	//--------------------------------------------------------
	// HP Bar
	//--------------------------------------------------------
	MAX_HP_BAR_PIXEL			= 120;		// hp bar의 크기
	POSITION_HP_BAR			= -10;		// 이름의 위치(hp bar 위치)

	//--------------------------------------------------------
	// 아이템 부서지는거에 따른 색깔 표시
	//--------------------------------------------------------
	PERCENTAGE_ITEM_SOMEWHAT_BROKEN	= 25;		// 25%남았을때 오렌지색
	PERCENTAGE_ITEM_ALMOST_BROKEN		= 10;		// 10%남았을때 빨간색

	//--------------------------------------------------------------
	// Effect붙어서 캐릭터 색깔 바뀔때 가끔씩 원래 모습 보여주는 frame
	//--------------------------------------------------------------
	FRAME_DRAW_ORIGINAL_SPRITE			= 8;		// 4frame마다 한번씩

	//--------------------------------------------------------
	// 색깔 - 5:6:5를 저장해야 한다.
	//--------------------------------------------------------
	COLOR_OUTLINE_INTERACTIONOBJECT	= (0 << 11) | (31 << 6) | 0;	// GREEN

	// item
	COLOR_NAME_ITEM					= RGB(31<<3, 31<<3, 31<<3);	// WHITE
	COLOR_NAME_ITEM_OPTION			= RGB(31<<3, 31<<3, 5<<3);	// 거의 blue
	COLOR_NAME_ITEM_RARE_OPTION		= RGB(19<<3, 19<<3, 31<<3);	// 거의 blue
	COLOR_OUTLINE_ITEM				= (23 << 11) | (23 << 6) | 23;

	// 종족별
	COLOR_NAME_VAMPIRE				= RGB(31<<3, 10<<3, 10<<3);	// RED
	COLOR_NAME_SLAYER				= RGB(23<<3, 23<<3, 31<<3);	// BLUE
	COLOR_NAME_NPC					= RGB(5<<3, 27<<3, 5<<3);	// GREEN

	// 공격 가능?
	COLOR_OUTLINE_NPC				= (5 << 11) | (23 << 6) | 5;
	COLOR_OUTLINE_ATTACK_POSSIBLE	= (23 << 11) | (5 << 6) | 5;
	COLOR_OUTLINE_ATTACK_IMPOSSIBLE	= (5 << 11) | (5 << 6) | 18;

	// hp bar
	COLOR_HP_BAR_R					= 15;
	COLOR_HP_BAR_G					= 16;
	COLOR_HP_BAR_B					= 15;

	//---------------------------------------------------------------------
	// 새 사용자 등록 모드
	//---------------------------------------------------------------------
	//	NUR_CLIENT,				// 새 사용자 등록 가능
	//	NUR_HOMEPAGE,			// 홈페이지를 띄워주면서 등록
	//	NUR_MESSAGE_HOMEPAGE,	// 홈페이지에서 등록하라는 message
	//	NUR_DENY				// 새 사용자 등록 불가			
	NEW_USER_REGISTERATION_MODE	= ClientConfig::NUR_HOMEPAGE;
//	URL_HOMEPAGE					= "http://www.t2cn.com";				// 홈페이지 main
//	URL_HOMEPAGE_NEW_USER			= "http://member.t2cn.com/reg";		// 새 사용자 등록 URL
//	URL_HOMEPAGE_BILING				= "http://member.t2cn.com/billing";
	URL_HOMEPAGE					= "http://www.darkeden.com";				// 홈페이지 main
	URL_HOMEPAGE_NEW_USER			= "http://www.darkeden.com/pg/new_reg.html";		// 새 사용자 등록 URL
	URL_HOMEPAGE_BILING				= "http://www.darkeden.com/pg/buy_index.html";

	//---------------------------------------------------------------------
	// 성향에 따른 색깔
	//---------------------------------------------------------------------
	COLOR_NAME_GOOD_MORE		= RGB(50, 50, 200);	
	COLOR_NAME_GOOD				= RGB(25, 210, 220);
	COLOR_NAME_NEUTRAL			= RGB(128, 128, 128);
	COLOR_NAME_EVIL				= RGB(200, 50, 50);
	COLOR_NAME_EVIL_MORE		= RGB(110, 25, 25);

	//--------------------------------------------------------
	// 피 흘리기
	//--------------------------------------------------------
	BLOOD_DROP_HP_PERCENT		= 30;		// 피 흘리기 시작하는 HP %
	BLOOD_DROP_GAP_TIME			= 3000;		// 다음에 피 흘리기 시작하는 시간 gap
	BLOOD_DROP_RANDOM_TIME		= 3000;		// 피 흘리는 시간 = GAP + RANDOM

	//--------------------------------------------------------
	//
	//--------------------------------------------------------
	MAX_TEXTUREPART_EFFECTSHADOW	= 3;

	//--------------------------------------------------------
	// trade후 item떨어지는게 가능해지는 시간
	//--------------------------------------------------------
	AFTER_TRADE_ITEM_DROP_DELAY = 3000;	

	//--------------------------------------------------------
	// 파티 join후 다른사람 kick할 수 있는 시간
	//--------------------------------------------------------
	AFTER_PARTY_KICK_DELAY	= 60*60*1000;	// 1시간 = 60분 * 60초 

	//--------------------------------------------------------------
	// Client통신에 사용하는 UDP Port
	//--------------------------------------------------------------
	CLIENT_COMMUNICATION_UDP_PORT	= 9858;

	CLIENT_COMMUNICATION_STATUS_DELAY = 5000;

	//--------------------------------------------------------------
	// trade할 때 cancel후 OK를 누를 때 delay 시간
	//--------------------------------------------------------------
	TRADE_ACCEPT_DELAY_TIME = 5000;

	//--------------------------------------------------------------
	// Vampire 리젠양
	//--------------------------------------------------------------
	REGEN_AMOUNT_BURROW		= 4;
	REGEN_AMOUNT_CASKET		= 10;
	REGEN_AMOUNT_VAMPIRE	= 2;

	//--------------------------------------------------------------
	// 초당 wave sound 소리 내는 개수 제한
	//--------------------------------------------------------------
	MAX_SOUND_PER_SECOND	= 6;

	REPEAT_TIME				= 10*60*1000;				// 자동공격 제한시간 10분
	LOCK_TIME				= 10*60*1000;				// Caps Lock자동공격 제한시간 10분
	
	// 유니크 아이템 칼라셋
	UniqueItemColorSet		= 405;
	QuestItemColorSet		= 345;

	// HPModifyList 개수 제한및 시간제한(msec)
	HPModifyListMax			= 10;
	HPModifyListTime		= 10000;
	TRACE_CHARACTER_LIMIT_TIME		= 5*60*1000;		// 캐릭터따라가기 제한시간 5분
}

ClientConfig::~ClientConfig()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void

ClientConfig::SaveToFile(const char* filename)
{
	std::ofstream file(filename, std::ios::binary);

	file.write((const char*)&FPS, 4);

	file.write((const char*)&MAX_SCREENSHOT, 4);

	file.write((const char*)&DELAY_PLAYER_DEAD, 4);
	file.write((const char*)&MAX_CLIENT_MOVE, 4);
	file.write((const char*)&MAX_CREATURE_MOVE_BUFFER, 4);
	file.write((const char*)&MAX_LIGHT_DRAW, 4);

	file.write((const char*)&MAX_SOUNDPART, 4);

	file.write((const char*)&MAX_UPDATE_ONETIME, 4);
	file.write((const char*)&MAX_UPDATE_ONETIME_COUNT, 4);

	file.write((const char*)&DELAY_GLOBAL_SAY, 4);	

	file.write((const char*)&DELAY_SYSTEMMESSAGE, 4);	
	file.write((const char*)&DELAY_GAMEMESSAGE, 4);	

	file.write((const char*)&MAX_DRAWITEMNAME, 4);	

	file.write((const char*)&MAX_WAIT_PACKET, 4);
	file.write((const char*)&MAX_PROCESS_PACKET, 4);
	
	file.write((const char*)&MUSIC_THEME, 4);
	
	//--------------------------------------------------------------
	// 채팅 String 줄 수
	//--------------------------------------------------------------
	file.write((const char*)&MAX_CHATSTRING, 4);
	file.write((const char*)&MAX_CHATSTRING_MINUS_1, 4);
	file.write((const char*)&MAX_CHATSTRING_LENGTH, 4);
	file.write((const char*)&MAX_CHATSTRINGLENGTH_PLUS1, 4);

	//--------------------------------------------------------------
	// 채팅 보여지는 시간..
	//--------------------------------------------------------------
	file.write((const char*)&DELAY_CHATSTRING_KEEP, 4);		// 흰색으로 표현하는 시간
	file.write((const char*)&DELAY_CHATSTRING_FADE, 4);		// 어두워지는 단계의 시간
	file.write((const char*)&VALUE_CHATSTRING_FADE, 4);		// 어두워지는 값
	file.write((const char*)&MIN_CHATSTRING_COLOR256, 4);		// 가장 어두운 채팅 String색

	//--------------------------------------------------------
	// TexturePart Manager의 Part 개수
	//--------------------------------------------------------
	file.write((const char*)&MAX_TEXTUREPART_EFFECT, 4);
	file.write((const char*)&MAX_TEXTUREPART_CREATURESHADOW, 4);
	file.write((const char*)&MAX_TEXTUREPART_IMAGEOBJECTSHADOW, 4);
	file.write((const char*)&MAX_TEXTUREPART_ADDON_SHADOW, 4);
	file.write((const char*)&MAX_TEXTUREPART_IMAGEOBJECTSPK, 4);

	//--------------------------------------------------------
	// Font 크기
	//--------------------------------------------------------
	file.write((const char*)&FONT_HEIGHT, 4);
	file.write((const char*)&FONT_ITEM_HEIGHT, 4);

	//--------------------------------------------------------
	// HP Bar
	//--------------------------------------------------------
	file.write((const char*)&MAX_HP_BAR_PIXEL, 4);			// hp bar의 크기
	file.write((const char*)&POSITION_HP_BAR, 4);		// 이름의 위치(hp bar 위치)

	//--------------------------------------------------------
	// item 부서진 정도(%)
	//--------------------------------------------------------
	file.write((const char*)&PERCENTAGE_ITEM_SOMEWHAT_BROKEN, 4);
	file.write((const char*)&PERCENTAGE_ITEM_ALMOST_BROKEN, 4);

	//--------------------------------------------------------------
	// 캐릭터에 붙는 Effect보여줄때 가끔 원래 색깔 보여주는 Frame
	//--------------------------------------------------------------
	file.write((const char*)&FRAME_DRAW_ORIGINAL_SPRITE, 4);
		
	//--------------------------------------------------------
	// 색깔
	//--------------------------------------------------------
	file.write((const char*)&COLOR_OUTLINE_INTERACTIONOBJECT, 2);

	// item
	file.write((const char*)&COLOR_NAME_ITEM, 4);
	file.write((const char*)&COLOR_NAME_ITEM_OPTION, 4);
	file.write((const char*)&COLOR_NAME_ITEM_RARE_OPTION, 4);
	file.write((const char*)&COLOR_OUTLINE_ITEM, 2);

	// 종족별
	file.write((const char*)&COLOR_NAME_VAMPIRE, 4);
	file.write((const char*)&COLOR_NAME_SLAYER, 4);
	file.write((const char*)&COLOR_NAME_NPC, 4);

	// 공격 가능?
	file.write((const char*)&COLOR_OUTLINE_NPC, 2);
	file.write((const char*)&COLOR_OUTLINE_ATTACK_POSSIBLE, 2);
	file.write((const char*)&COLOR_OUTLINE_ATTACK_IMPOSSIBLE, 2);

	// HP bar의 색깔
	file.write((const char*)&COLOR_HP_BAR_R, 1);
	file.write((const char*)&COLOR_HP_BAR_G, 1);
	file.write((const char*)&COLOR_HP_BAR_B, 1);

	//--------------------------------------------------------
	// 새 사용자 등록 mode
	//--------------------------------------------------------
	file.write((const char*)&NEW_USER_REGISTERATION_MODE, sizeof(NUR_MODE));
	URL_HOMEPAGE.SaveToFile( file );				// 홈페이지 main
	URL_HOMEPAGE_NEW_USER.SaveToFile( file );		// 새 사용자 등록 URL
	URL_HOMEPAGE_BILING.SaveToFile( file );

	file.write((const char*)&COLOR_NAME_GOOD_MORE, 4);
	file.write((const char*)&COLOR_NAME_GOOD, 4);
	file.write((const char*)&COLOR_NAME_NEUTRAL, 4);
	file.write((const char*)&COLOR_NAME_EVIL, 4);
	file.write((const char*)&COLOR_NAME_EVIL_MORE, 4);	

	file.write((const char*)&BLOOD_DROP_HP_PERCENT, 4);
	file.write((const char*)&BLOOD_DROP_GAP_TIME, 4);
	file.write((const char*)&BLOOD_DROP_RANDOM_TIME, 4);	
	file.write((const char*)&MAX_TEXTUREPART_EFFECTSHADOW, 4);
	file.write((const char*)&MAX_TEXTUREPART_SCREENEFFECT, 4);	

	file.write((const char*)&MAX_REQUEST_SERVICE, 4);

	file.write((const char*)&AFTER_TRADE_ITEM_DROP_DELAY, 4);

	file.write((const char*)&DELAY_PLAYER_RESURRECT_TIMEOUT, 4);
	
	file.write((const char*)&AFTER_PARTY_KICK_DELAY, 4);

	//--------------------------------------------------------------
	// Client통신에 사용하는 UDP Port
	//--------------------------------------------------------------
	file.write((const char*)&CLIENT_COMMUNICATION_UDP_PORT, 4);
	file.write((const char*)&CLIENT_COMMUNICATION_STATUS_DELAY, 4);

	//--------------------------------------------------------------
	// trade 후 accept 누를 수 있는 delay
	//--------------------------------------------------------------
	file.write((const char*)&TRADE_ACCEPT_DELAY_TIME, 4);

	//--------------------------------------------------------------
	// vampire 리젠
	//--------------------------------------------------------------
	file.write((const char*)&REGEN_AMOUNT_BURROW, 1);
	file.write((const char*)&REGEN_AMOUNT_CASKET, 1);
	file.write((const char*)&REGEN_AMOUNT_VAMPIRE, 1);	

	//--------------------------------------------------------------
	// 초당 wave sound 소리 내는 개수 제한
	//--------------------------------------------------------------
	file.write((const char*)&MAX_SOUND_PER_SECOND, 1);	

	// 마우스 눌림 제한
	file.write((const char*)&REPEAT_TIME, 4);
	file.write((const char*)&LOCK_TIME, 4);

	// 유니크 아이템 칼라셋
	file.write((const char*)&UniqueItemColorSet, 4);
	file.write((const char*)&QuestItemColorSet, 4);

	// HPModifyList 개수 제한및 시간제한(sec)
	file.write((const char*)&HPModifyListMax, 1);
	file.write((const char*)&HPModifyListTime, 4);
	file.write((const char*)&TRACE_CHARACTER_LIMIT_TIME, 4);
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
bool	
ClientConfig::LoadFromFile(const char* filename)
{
	std::ifstream file(filename, std::ios::binary);	

	if (!file || !file.is_open())
	{
		return false;
	}
	
	file.read((char*)&FPS, 4);

	file.read((char*)&MAX_SCREENSHOT, 4);

	file.read((char*)&DELAY_PLAYER_DEAD, 4);
	file.read((char*)&MAX_CLIENT_MOVE, 4);
	file.read((char*)&MAX_CREATURE_MOVE_BUFFER, 4);
	file.read((char*)&MAX_LIGHT_DRAW, 4);

	file.read((char*)&MAX_SOUNDPART, 4);

	file.read((char*)&MAX_UPDATE_ONETIME, 4);
	file.read((char*)&MAX_UPDATE_ONETIME_COUNT, 4);

	file.read((char*)&DELAY_GLOBAL_SAY, 4);	

	file.read((char*)&DELAY_SYSTEMMESSAGE, 4);	
	file.read((char*)&DELAY_GAMEMESSAGE, 4);	

	file.read((char*)&MAX_DRAWITEMNAME, 4);

	file.read((char*)&MAX_WAIT_PACKET, 4);
	file.read((char*)&MAX_PROCESS_PACKET, 4);

	file.read((char*)&MUSIC_THEME, 4);

	//--------------------------------------------------------------
	// 채팅 String 줄 수
	//--------------------------------------------------------------
	file.read((char*)&MAX_CHATSTRING, 4);
	file.read((char*)&MAX_CHATSTRING_MINUS_1, 4);
	file.read((char*)&MAX_CHATSTRING_LENGTH, 4);
	file.read((char*)&MAX_CHATSTRINGLENGTH_PLUS1, 4);

	//--------------------------------------------------------------
	// 채팅 보여지는 시간..
	//--------------------------------------------------------------
	file.read((char*)&DELAY_CHATSTRING_KEEP, 4);		// 흰색으로 표현하는 시간
	file.read((char*)&DELAY_CHATSTRING_FADE, 4);		// 어두워지는 단계의 시간
	file.read((char*)&VALUE_CHATSTRING_FADE, 4);		// 어두워지는 값
	file.read((char*)&MIN_CHATSTRING_COLOR256, 4);		// 가장 어두운 채팅 String색

	//--------------------------------------------------------
	// TexturePart Manager의 Part 개수
	//--------------------------------------------------------
	file.read((char*)&MAX_TEXTUREPART_EFFECT, 4);
	file.read((char*)&MAX_TEXTUREPART_CREATURESHADOW, 4);
	file.read((char*)&MAX_TEXTUREPART_IMAGEOBJECTSHADOW, 4);
	file.read((char*)&MAX_TEXTUREPART_ADDON_SHADOW, 4);
	file.read((char*)&MAX_TEXTUREPART_IMAGEOBJECTSPK, 4);

	//--------------------------------------------------------
	// Font 크기
	//--------------------------------------------------------
	file.read((char*)&FONT_HEIGHT, 4);
	file.read((char*)&FONT_ITEM_HEIGHT, 4);

	//--------------------------------------------------------
	// HP Bar
	//--------------------------------------------------------
	file.read((char*)&MAX_HP_BAR_PIXEL, 4);			// hp bar의 크기
	file.read((char*)&POSITION_HP_BAR, 4);		// 이름의 위치(hp bar 위치)

	//--------------------------------------------------------
	// item 부서진 정도(%)
	//--------------------------------------------------------
	file.read((char*)&PERCENTAGE_ITEM_SOMEWHAT_BROKEN, 4);
	file.read((char*)&PERCENTAGE_ITEM_ALMOST_BROKEN, 4);

	//--------------------------------------------------------------
	// 캐릭터에 붙는 Effect보여줄때 가끔 원래 색깔 보여주는 Frame
	//--------------------------------------------------------------
	file.read((char*)&FRAME_DRAW_ORIGINAL_SPRITE, 4);

	//--------------------------------------------------------
	// 색깔
	//--------------------------------------------------------
	file.read((char*)&COLOR_OUTLINE_INTERACTIONOBJECT, 2);

	// item
	file.read((char*)&COLOR_NAME_ITEM, 4);
	file.read((char*)&COLOR_NAME_ITEM_OPTION, 4);
	file.read((char*)&COLOR_NAME_ITEM_RARE_OPTION, 4);
	file.read((char*)&COLOR_OUTLINE_ITEM, 2);

	// 종족별
	file.read((char*)&COLOR_NAME_VAMPIRE, 4);
	file.read((char*)&COLOR_NAME_SLAYER, 4);
	file.read((char*)&COLOR_NAME_NPC, 4);

	// 공격 가능?
	file.read((char*)&COLOR_OUTLINE_NPC, 2);
	file.read((char*)&COLOR_OUTLINE_ATTACK_POSSIBLE, 2);
	file.read((char*)&COLOR_OUTLINE_ATTACK_IMPOSSIBLE, 2);

	// HP bar의 색깔
	file.read((char*)&COLOR_HP_BAR_R, 1);
	file.read((char*)&COLOR_HP_BAR_G, 1);
	file.read((char*)&COLOR_HP_BAR_B, 1);

	//--------------------------------------------------------
	// 새 사용자 등록 mode
	//--------------------------------------------------------
	file.read((char*)&NEW_USER_REGISTERATION_MODE, sizeof(NUR_MODE));
	URL_HOMEPAGE.LoadFromFile( file );				// 홈페이지 main
	URL_HOMEPAGE_NEW_USER.LoadFromFile( file );		// 새 사용자 등록 URL
	URL_HOMEPAGE_BILING.LoadFromFile( file );

	URL_HOMEPAGE					= "http://bbstest.web11.zcidc.com/bbs/index.asp";				// 홈페이지 main
	URL_HOMEPAGE_NEW_USER			= "http://bbstest.web11.zcidc.com/bbs/index.asp";		// 새 사용자 등록 URL
	URL_HOMEPAGE_BILING				= "http://bbstest.web11.zcidc.com/bbs/index.asp";
	
	//--------------------------------------------------------
	// 성향에 따른 색깔..
	//--------------------------------------------------------
	DWORD temp;
	READ_CHECK_EOF( COLOR_NAME_GOOD_MORE, temp, 4 )
	READ_CHECK_EOF( COLOR_NAME_GOOD, temp, 4 )
	READ_CHECK_EOF( COLOR_NAME_NEUTRAL, temp, 4 )
	READ_CHECK_EOF( COLOR_NAME_EVIL, temp, 4 )
	READ_CHECK_EOF( COLOR_NAME_EVIL_MORE, temp, 4 )	

	READ_CHECK_EOF( BLOOD_DROP_HP_PERCENT, temp, 4 )
	READ_CHECK_EOF( BLOOD_DROP_GAP_TIME, temp, 4 )
	READ_CHECK_EOF( BLOOD_DROP_RANDOM_TIME, temp, 4 )	
	READ_CHECK_EOF( MAX_TEXTUREPART_EFFECTSHADOW, temp, 4 )
	READ_CHECK_EOF( MAX_TEXTUREPART_SCREENEFFECT, temp, 4 )	

	READ_CHECK_EOF( MAX_REQUEST_SERVICE, temp, 4 )

	READ_CHECK_EOF( AFTER_TRADE_ITEM_DROP_DELAY, temp, 4 )

	READ_CHECK_EOF( DELAY_PLAYER_RESURRECT_TIMEOUT, temp, 4 )
	
	READ_CHECK_EOF( AFTER_PARTY_KICK_DELAY, temp, 4 )

	//--------------------------------------------------------------
	// Client통신에 사용하는 UDP Port
	//--------------------------------------------------------------
	READ_CHECK_EOF( CLIENT_COMMUNICATION_UDP_PORT, temp, 4 )

	READ_CHECK_EOF( CLIENT_COMMUNICATION_STATUS_DELAY, temp, 4 )
	
	//--------------------------------------------------------------
	// trade 후 accept 누를 수 있는 delay
	//--------------------------------------------------------------
	READ_CHECK_EOF( TRADE_ACCEPT_DELAY_TIME, temp, 4)

	//--------------------------------------------------------------
	// vampire 리젠양
	//--------------------------------------------------------------
	BYTE temp1;
	READ_CHECK_EOF( REGEN_AMOUNT_BURROW, temp1, 1)
	READ_CHECK_EOF( REGEN_AMOUNT_CASKET, temp1, 1)
	READ_CHECK_EOF( REGEN_AMOUNT_VAMPIRE, temp1, 1)	

	//--------------------------------------------------------------
	// 초당 wave sound 소리 내는 개수 제한
	//--------------------------------------------------------------	
	READ_CHECK_EOF( MAX_SOUND_PER_SECOND, temp1, 1)

	// 마우스 눌림 제한
	READ_CHECK_EOF( REPEAT_TIME, temp, 4)
	READ_CHECK_EOF( LOCK_TIME, temp, 4)

	// 유니크 아이템 칼라셋
	READ_CHECK_EOF( UniqueItemColorSet, temp, 4);
	READ_CHECK_EOF( QuestItemColorSet, temp, 4);

	
	// HPModifyList 개수 제한및 시간 제한(sec)
	READ_CHECK_EOF( HPModifyListMax, temp1, 1);
	READ_CHECK_EOF( HPModifyListTime, temp, 4);
	READ_CHECK_EOF( TRACE_CHARACTER_LIMIT_TIME, temp, 4 );
	
	return true;
}
