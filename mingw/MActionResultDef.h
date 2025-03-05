//----------------------------------------------------------------------
// MActionResultDef.h
//----------------------------------------------------------------------

#ifndef __MACTIONRESULTDEF_H__
#define __MACTIONRESULTDEF_H__

//----------------------------------------------------------------------
//
//                       EffectGenerator 관련
//
//----------------------------------------------------------------------

typedef	unsigned short TYPE_ACTIONRESULTID;
#define	SIZE_ACTIONRESULTID			2
#define	ACTIONRESULTID_NULL			0xFFFF

	
//------------------------------------------------------
// ActionInfo의 결과 - Server에서 검증된 경우
//------------------------------------------------------
enum	ACTIONRESULTNODE_TYPE
{
	ACTIONRESULTNODE_NULL = 0,

	ACTIONRESULTNODE_ACTIONINFO,			// 다른 ActionInfo를 유발하는 경우			
	ACTIONRESULTNODE_CHANGE_POSITION,		// 캐릭터 위치를 변하게 하는 경우
	ACTIONRESULTNODE_CHANGE_STATUS,			// 캐릭터의 상태를 변하게 하는 경우
	ACTIONRESULTNODE_CREATURE_DIE,			// 어떤 캐릭터가 죽는다.
	ACTIONRESULTNODE_CREATURE_BURROW,		// 어떤 캐릭터가 숨는다.
	ACTIONRESULTNODE_CREATURE_INVISIBLE,	// 어떤 캐릭터가 안 보인다.
	ACTIONRESULTNODE_CREATURE_VISIBLE,		// 어떤 캐릭터가 보인다.
	ACTIONRESULTNODE_CREATURE_SNIPPING,		// snipping mode로..
	ACTIONRESULTNODE_ADD_EFFECTSTATUS,		// 캐릭터에 effectstatus를 붙인다.
	ACTIONRESULTNODE_CHANGE_CREATURE_TYPE,	// Creature type을 바꾼다.
	ACTIONRESULTNODE_SUMMON_CASKET,			// 관 소환
	ACTIONRESULTNODE_OPEN_CASKET,			// 관에서 나가기
	ACTIONRESULTNODE_CREATURE_TURNING,		// 캐릭터 돌리기
	ACTIONRESULTNODE_REMOVE_ITEM_IN_INVENTORY,			// 인벤토리 아이템 없애기
	ACTIONRESULTNODE_CHANGE_ITEM_OPTION_IN_INVENTORY,	// 인벤토리 아이템 옵션 바꾸기
	ACTIONRESULTNODE_FAKE_DIE,				// 가짜 캐릭터가 죽는다.
	ACTIONRESULTNODE_EVENT,					// 이벤트를 붙인다
	ACTIONRESULTNODE_CHANGE_ITEM_GENDER,

	ACTIONRESULTNODE_MAX
};


#endif