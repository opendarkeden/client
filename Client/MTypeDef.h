//----------------------------------------------------------------------
// MTypeDef.h
//----------------------------------------------------------------------

#ifndef	__MTYPEDEF_H__
#define	__MTYPEDEF_H__

#pragma warning(disable:4786)

//------------------------------------------------------------------
// NULL값
//------------------------------------------------------------------
#define	ZONEID_NULL						0xFFFF
#define	OBJECTID_NULL					0xFFFFFFFF		// 아무것도 없다는 의미..
#define	INTERACTIONOBJECTTYPE_NULL		0xFFFF
#define	SECTORPOSITION_NULL				0xFFFF

#define	ITEMTYPE_NULL					0xFFFF
#define	ITEM_OPTION_NULL				0xFFFF
#define	ITEM_DURATION_NULL				0xFFFFFFFF
#define	ITEM_PRICE_NULL					0xFFFFFFFF
#define	ITEM_NUMBER_NULL				0xFFFFFFFF
#define	ITEM_WEIGHT_NULL				0xFFFF


//------------------------------------------------------------------
// sizeof( TYPE_... )
//------------------------------------------------------------------
#define	SIZE_OBJECTID					4
#define	SIZE_ZONEID						2
//#define	SIZE_ITEMCLASS					2
#define	SIZE_INTERACTIONOBJECTTYPE		2
#define	SIZE_SECTORPOSITION				2

#define	SIZE_ITEMTYPE					2
#define	SIZE_ITEM_OPTION				2
#define	SIZE_ITEM_DURATION				4
#define	SIZE_ITEM_PRICE					4
#define	SIZE_ITEM_NUMBER				4
#define	SIZE_ITEM_WEIGHT				2


//------------------------------------------------------------------
// typedef
//------------------------------------------------------------------
typedef	unsigned short	TYPE_ZONEID;
typedef	unsigned int	TYPE_OBJECTID;
//typedef	unsigned short	TYPE_ITEMCLASS;
typedef	unsigned short	TYPE_INTERACTIONOBJECTTYPE;
typedef	unsigned short	TYPE_SECTORPOSITION;

typedef	unsigned short	TYPE_ITEMTYPE;
typedef	unsigned char	TYPE_ITEM_OPTION;
typedef	unsigned int	TYPE_ITEM_DURATION;
//typedef	unsigned short	TYPE_ITEM_DURATION;
typedef	unsigned int	TYPE_ITEM_PRICE;
typedef	unsigned int	TYPE_ITEM_NUMBER;
typedef	unsigned short	TYPE_ITEM_WEIGHT;


//----------------------------------------------------------------------
//
//                 Creature 관련
//
//----------------------------------------------------------------------

#define	CREATURETYPE_NULL				0xFFFF

#define	SIZE_CREATURETYPE				2

typedef	unsigned short	TYPE_CREATURETYPE;

//----------------------------------------------------------------------
// 방향에 대한 값
//----------------------------------------------------------------------
enum DIRECTIONS
{			
	DIRECTION_LEFT = 0,
	DIRECTION_LEFTDOWN,
	DIRECTION_DOWN,
	DIRECTION_RIGHTDOWN,
	DIRECTION_RIGHT,
	DIRECTION_RIGHTUP,
	DIRECTION_UP,
	DIRECTION_LEFTUP,
	DIRECTION_MAX,	
	DIRECTION_NULL
};

#define	MAX_DIRECTION	DIRECTION_MAX

//----------------------------------------------------------------------
// 행동에 대한 값
//----------------------------------------------------------------------
// VAMPIRE와 SLAYER가 같은 값을 가지는 이유는
// FramePack에서의 Action번호 때문이다..
//
#define	ACTION_STAND					0
#define	ACTION_MOVE						1
#define	ACTION_ATTACK					2
#define	ACTION_MAGIC					3
#define	ACTION_DAMAGED					4
#define	ACTION_DRAINED					5
#define	ACTION_DIE						6
#define	ACTION_VAMPIRE_DRAIN		7			// 같은 값이다.
#define	ACTION_SLAYER_GUN_SR			7			// 저격용(TR)
#define	ACTION_SLAYER_GUN_AR			8			// 소총(AG)
#define	ACTION_SLAYER_SWORD				9			// 칼
#define	ACTION_SLAYER_BLADE				10			// 도
#define	ACTION_SLAYER_THROW_WEAPON		11	
#define	ACTION_SLAYER_MOTOR_MOVE		12
#define	ACTION_SLAYER_GUN_SG			13			// 샷건(SG)
#define	ACTION_SLAYER_GUN_SMG			14			// 자동소총(SMG)
#define	ACTION_SLAYER_SWORD_2			15			// 칼 특수동작
#define	ACTION_SLAYER_BLADE_2			16			// 도 특수동작
#define	ACTION_SLAYER_THROW_POTION		17
#define	ACTION_SLAYER_MOTOR_STAND		18
#define	ACTION_VAMPIRE_ATTACK_SLOW		8			// vampire전용
#define	ACTION_VAMPIRE_ATTACK_FAST		9			// vampire전용

// 2001.7.6
#define	ACTION_VAMPIRE_DIE_BODY			10			// vampire전용 - 머리없고 목만 있는 것
#define	ACTION_SLAYER_GUN_SR_SLOW		19			// 저격용(TR)
#define	ACTION_SLAYER_GUN_SR_FAST		20			// 저격용(TR)
#define	ACTION_SLAYER_GUN_AR_SLOW		21			// 소총(AG)
#define	ACTION_SLAYER_GUN_AR_FAST		22	 		// 소총(AG)
#define	ACTION_SLAYER_GUN_SG_SLOW		23			// 샷건(SG)
#define	ACTION_SLAYER_GUN_SG_FAST		24			// 샷건(SG)
#define	ACTION_SLAYER_GUN_SMG_SLOW		25			// 자동소총(SMG)
#define	ACTION_SLAYER_GUN_SMG_FAST		26			// 자동소총(SMG)
#define	ACTION_SLAYER_SWORD_SLOW		27			// 칼
#define	ACTION_SLAYER_SWORD_FAST		28			// 칼
#define	ACTION_SLAYER_BLADE_SLOW		29			// 도
#define	ACTION_SLAYER_BLADE_FAST		30			// 도
#define	ACTION_SLAYER_SWORD_2_SLOW		31			// 칼 특수동작
#define	ACTION_SLAYER_SWORD_2_FAST		32			// 칼 특수동작
#define	ACTION_SLAYER_BLADE_2_SLOW		33			// 도 특수동작
#define	ACTION_SLAYER_BLADE_2_FAST		34			// 도 특수동작

#define ACTION_OUSTERS_STAND			7			// 서있기
#define ACTION_OUSTERS_MOVE				8			// 걷기
#define ACTION_OUSTERS_CHAKRAM			9			// 차크람 공격
#define ACTION_OUSTERS_MAGIC_ATTACK		10			// 마법 공격
#define ACTION_OUSTERS_DRAIN			11			// 흡영
#define ACTION_OUSTERS_FAST_MOVE_STAND	12			// 공중 정지
#define ACTION_OUSTERS_FAST_MOVE_MOVE	13			// 공중 움직임
#define ACTION_OUSTERS_ATTACK_SLOW		14			// 일반 공격 느림
#define ACTION_OUSTERS_ATTACK_FAST		15			// 일반 공격 빠름
#define ACTION_OUSTERS_CHAKRAM_SLOW		16			// 차크람 공격 느림
#define ACTION_OUSTERS_CHAKRAM_FAST		17			// 차크람 공격 빠름

#define	ACTION_MAX_VAMPIRE			11
#define	ACTION_MAX_SLAYER			35 //by viva
#define	ACTION_MAX_OUSTERS			18
#define	ACTION_MAX_NPC				2
#define	ACTION_MAX					19


//----------------------------------------------------------------------
// Addon위치
//----------------------------------------------------------------------
enum ADDON
{
	ADDON_HAIR,			// 머리
	ADDON_HELM,			// 모자
	ADDON_COAT,			// 상의
	ADDON_TROUSER,		// 하의
	//ADDON_SHOES,		// 신발
	//ADDON_LEFTGLOVE,		// 왼손 장갑
	//ADDON_RIGHTGLOVE,		// 오른손 장갑
	ADDON_LEFTHAND,		// 왼손			
	ADDON_RIGHTHAND,	// 오른손
	//ADDON_CLOAK,		// 외투
	ADDON_MOTOR,		// 오토바이
	ADDON_MAX,
	ADDON_NULL	= 0xFFFF,
};



//----------------------------------------------------------------------
//
//                          Effect 관련
//
//----------------------------------------------------------------------
#define	ACTIONINFO_NULL					0xFFFF
#define	SIZE_ACTIONINFO					2
typedef	unsigned short	TYPE_ACTIONINFO;


//----------------------------------------------------------------------
//
//                       EffectGenerator 관련
//
//----------------------------------------------------------------------
typedef	unsigned short TYPE_EFFECTGENERATORID;
#define	SIZE_EFFECTGENERATORID			2
#define	EFFECTGENERATORID_NULL			0xFFFF



//----------------------------------------------------------------------
//
//                       Sound / Music 관련
//
//----------------------------------------------------------------------

typedef unsigned short TYPE_SOUNDID;
#define	SIZE_SOUNDID	2
#define	SOUNDID_NULL	0xFFFF

typedef unsigned short TYPE_MUSICID;
#define	SIZE_MUSICID	2
#define	MUSICID_NULL	0xFFFF


//----------------------------------------------------------------------
// EffectSpriteType 정의
//----------------------------------------------------------------------
typedef	unsigned short				TYPE_EFFECTSPRITETYPE;
#define	SIZE_EFFECTSPRITETYPE		2
#define	EFFECTSPRITETYPE_NULL		0xFFFF


//----------------------------------------------------------------------
// link Count
//----------------------------------------------------------------------
#define	MAX_LINKCOUNT				0xFFFF

enum {
	ACTION_NEW_STAND,		//0
	ACTION_NEW_MOVE,		//1
	ACTION_NEW_ATTACK,		//2
	ACTION_NEW_MAGIC,		//3
	ACTION_NEW_DAMAGED,		//4
	ACTION_NEW_DRAINED,		//5
	ACTION_NEW_DIE,		//6
	ACTION_NEW_SLAYER_GUN_SR,	//7
	ACTION_NEW_SLAYER_GUN_AR,	//8
	ACTION_NEW_SLAYER_GUN_SG,			//9	add
	ACTION_NEW_SLAYER_GUN_SMG,			//10	add
	ACTION_NEW_SLAYER_SWORD,	//11
	ACTION_NEW_SLAYER_BLADE,	//12
	ACTION_NEW_SLAYER_SWORD_2,		//13	add
	ACTION_NEW_SLAYER_BLADE_2,		//14	add
	ACTION_NEW_SLAYER_MOTOR_MOVE,	//15	-ACTION_SLAYER_THROW_WEAPON
	ACTION_NEW_SLAYER_MOTOR_STAND,	//16	add
	ACTION_NEW_SLAYER_GUN_SR_SLOW,	//17
	ACTION_NEW_SLAYER_GUN_SR_FAST,	//18
	ACTION_NEW_SLAYER_GUN_AR_SLOW,	// 19
	ACTION_NEW_SLAYER_GUN_AR_FAST,	// 20
	ACTION_NEW_SLAYER_GUN_SG_SLOW,	// 21
	ACTION_NEW_SLAYER_GUN_SG_FAST,	// 22
	ACTION_NEW_SLAYER_GUN_SMG_SLOW,	// 23
	ACTION_NEW_SLAYER_GUN_SMG_FAST,	// 24
	ACTION_NEW_SLAYER_SWORD_SLOW,	// 25
	ACTION_NEW_SLAYER_SWORD_FAST,	// 26
	ACTION_NEW_SLAYER_BLADE_SLOW,	// 27
	ACTION_NEW_SLAYER_BLADE_FAST,	// 28
	ACTION_NEW_SLAYER_SWORD_2_SLOW,	// 29
	ACTION_NEW_SLAYER_SWORD_2_FAST,	// 30
	ACTION_NEW_SLAYER_BLADE_2_SLOW,	// 31
	ACTION_NEW_SLAYER_BLADE_2_FAST,	// 32

	ACTION_NEW_SLAYER_STOP_GUN=33,
	ACTION_NEW_SLAYER_MOVE_GUN=36,

	ACTION_NEW_SLAYER_MAX=50
};

enum {
	ACTION_ADVANCEMENT_SLAYER_STOP_SWORD = 100,
	ACTION_ADVANCEMENT_SLAYER_STOP_BLADE,
	ACTION_ADVANCEMENT_SLAYER_STOP_GUN,
	ACTION_ADVANCEMENT_SLAYER_STOP_MACE_AND_CROSS,
	
	ACTION_ADVANCEMENT_SLAYER_MOVE_SWORD,
	ACTION_ADVANCEMENT_SLAYER_MOVE_BLADE,
	ACTION_ADVANCEMENT_SLAYER_MOVE_GUN,
	ACTION_ADVANCEMENT_SLAYER_MOVE_MACE_AND_CROSS,

	ACTION_ADVANCEMENT_SLAYER_ATTACK_SWORD_SLOW,
	ACTION_ADVANCEMENT_SLAYER_ATTACK_SWORD_NORMAL,
	ACTION_ADVANCEMENT_SLAYER_ATTACK_SWORD_FAST,
	
	ACTION_ADVANCEMENT_SLAYER_ATTACK_BLADE_SLOW,
	ACTION_ADVANCEMENT_SLAYER_ATTACK_BLADE_NORMAL,
	ACTION_ADVANCEMENT_SLAYER_ATTACK_BLADE_FAST,
	
	ACTION_ADVANCEMENT_SLAYER_ATTACK_AR_GUN_SLOW,
	ACTION_ADVANCEMENT_SLAYER_ATTACK_AR_GUN_NORMAL,
	ACTION_ADVANCEMENT_SLAYER_ATTACK_AR_GUN_FAST,
	
	ACTION_ADVANCEMENT_SLAYER_ATTACK_SR_GUN_SLOW,
	ACTION_ADVANCEMENT_SLAYER_ATTACK_SR_GUN_NORMAL,
	ACTION_ADVANCEMENT_SLAYER_ATTACK_SR_GUN_FAST,

	ACTION_ADVANCEMENT_SLAYER_SKILL_SWORD_SLOW,
	ACTION_ADVANCEMENT_SLAYER_SKILL_SWORD_NORMAL,
	ACTION_ADVANCEMENT_SLAYER_SKILL_SWORD_FAST,

	ACTION_ADVANCEMENT_SLAYER_SKILL_BLADE_SLOW,
	ACTION_ADVANCEMENT_SLAYER_SKILL_BLADE_NORMAL,
	ACTION_ADVANCEMENT_SLAYER_SKILL_BLADE_FAST,

	ACTION_ADVANCEMENT_SLAYER_SKILL_GUN_SLOW,
	ACTION_ADVANCEMENT_SLAYER_SKILL_GUN_NORMAL,
	ACTION_ADVANCEMENT_SLAYER_SKILL_GUN_FAST,
	
	ACTION_ADVANCEMENT_SLAYER_MAGIC,
	ACTION_ADVANCEMENT_SLAYER_MAGIC_ATTACK,

	ACTION_ADVANCEMENT_SLAYER_DRAINED,
	ACTION_ADVANCEMENT_SLAYER_DAMAGED_SWORD,
	ACTION_ADVANCEMENT_SLAYER_DAMAGED_BLADE,
	ACTION_ADVANCEMENT_SLAYER_DAMAGED_GUN,
	ACTION_ADVANCEMENT_SLAYER_DAMAGED_CROSS_MACE,
	ACTION_ADVANCEMENT_SLAYER_DIE,

	ACTION_ADVANCEMENT_SLAYER_BIKE_MOVE,
	ACTION_ADVANCEMENT_SLAYER_BIKE_STOP,

	ACTION_ADVANCEMENT_SLAYER_SPECIAL,
	ACTION_ADVANCEMENT_SLAYER_SPECIAL_2,
	ACTION_ADVANCEMENT_SLAYER_SLIDING,
	ACTION_ADVANCEMENT_SLAYER_SLIDING_END,

	ACTION_ADVANCEMENT_SLAYER_MAX,
};

enum
{
	ACTION_ADVANCEMENT_OUSTERS_CHAKRAM_STOP = 100,
	ACTION_ADVANCEMENT_OUSTERS_CHAKRAM_MOVE,
	ACTION_ADVANCEMENT_OUSTERS_WRISTLET_STOP,
	ACTION_ADVANCEMENT_OUSTERS_WRISTLET_MOVE,

	ACTION_ADVANCEMENT_OUSTERS_ATTACK_SLOW,
	ACTION_ADVANCEMENT_OUSTERS_ATTACK_NORMAL,
	ACTION_ADVANCEMENT_OUSTERS_ATTACK_FAST,
	ACTION_ADVANCEMENT_OUSTERS_SKILL_SLOW,
	ACTION_ADVANCEMENT_OUSTERS_SKILL_NORMAL,
	ACTION_ADVANCEMENT_OUSTERS_SKILL_FAST,

	ACTION_ADVANCEMENT_OUSTERS_MAGIC_ATTACK,

	ACTION_ADVANCEMENT_OUSTERS_MAGIC,
	ACTION_ADVANCEMENT_OUSTERS_ABSORB_SOUL,
	ACTION_ADVANCEMENT_OUSTERS_DRAINED,
	ACTION_ADVANCEMENT_OUSTERS_DIE,
	ACTION_ADVANCEMENT_OUSTERS_DAMAGED,
	ACTION_ADVANCEMENT_OUSTERS_SPECIAL,

	ACTION_ADVANCEMENT_OUSTERS_FAST_MOVE_STOP,
	ACTION_ADVANCEMENT_OUSTERS_FAST_MOVE,

	ACTION_ADVANCEMENT_OUSTERS_MAX
};

enum {
	ACTION_ADVANCEMENT_STOP = 100,
	ACTION_ADVANCEMENT_MOVE,
	ACTION_ADVANCEMENT_DAMAGED,
	ACTION_ADVANCEMENT_DIE,
	ACTION_ADVANCEMENT_DRAIN,
	ACTION_ADVANCEMENT_DRAINED,

	ACTION_ADVANCEMENT_ATTACK_SLOW,
	ACTION_ADVANCEMENT_ATTACK_NORMAL,
	ACTION_ADVANCEMENT_ATTACK_FAST,

	ACTION_ADVANCEMENT_SKILL_SLOW,
	ACTION_ADVANCEMENT_SKILL_NORMAL,
	ACTION_ADVANCEMENT_SKILL_FAST,

	ACTION_ADVANCEMENT_MAGIC,
	ACTION_ADVANCEMENT_MAGIC_ATTACK,
	ACTION_ADVANCEMENT_SPECIAL,
	ACTION_ADVANCEMENT_CREATE_WEAPON,
	ACTION_ADVANCEMENT_DESTROY_WEAPON,
	
	ACTION_ADVANCEMENT_MAX,
};

enum AC_SLAYER_PART
{
	AC_BODY = 0,
	AC_HELMET,
	AC_SWORD,
	AC_BLADE,
	AC_CROSS,
	AC_MACE,
	AC_GUN_AR,
	AC_GUN_SR,
	AC_SHIELD,
	AC_BIKE_1,
	AC_BIKE_2,
	AC_SHOULDER,

	AC_PART_MAX,
};

enum AC_ADDON
{
	AC_ADDON_BODY,		// 몸통
	AC_ADDON_HELM,		// 헬멧
//	AC_ADDON_COAT,			// add by viva
//	AC_ADDON_TROUSER,		//add by viva
	AC_ADDON_LEFTHAND,	// 왼손
	AC_ADDON_RIGHTHAND,	// 오른손
	AC_ADDON_MOTOR,		// 오토바이
	AC_ADDON_SHOULDER,	// 견장
	AC_ADDON_MAX,
	AC_ADDON_NULL	= 0xFFFF,
};

#define ADVANCEMENT_ACTION_START	100

#endif
