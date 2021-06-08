#pragma once

// SLAYER_PART가 어떤 addon type인지 규정..
const BYTE SlayerPartType[SLAYER_PART_MAX] = 
{
	ADDON_HELM,
	ADDON_COAT,
	ADDON_TROUSER,
	ADDON_RIGHTHAND,
	ADDON_RIGHTHAND,
	ADDON_LEFTHAND,
	ADDON_LEFTHAND,
	ADDON_LEFTHAND,
	ADDON_RIGHTHAND,
	ADDON_RIGHTHAND,
	ADDON_RIGHTHAND,
	ADDON_HAIR,
	ADDON_HAIR,
	ADDON_HAIR,
	ADDON_HELM,
	ADDON_COAT,
	ADDON_TROUSER,
	ADDON_RIGHTHAND,
	ADDON_RIGHTHAND,
	ADDON_MOTOR,
	ADDON_COAT,
	ADDON_TROUSER,
#ifdef _TEMP_FOR_INTERFACE
	ADDON_HAIR,	// 임시.
	ADDON_HAIR,
	ADDON_HAIR,
#endif
};

const char* strAction[ ACTION_MAX ] = {
	"정지",			// ACTION_STOP	 = 	0,
	"걷기",			// ACTION_MOVE,
	"공격",			// ACTION_ATTACK,	
	"마법",			// ACTION_SKILL,			// magic
	"대미지",			// ACTION_DAMAGED,	
	"흡혈당하기",			// ACTION_DRAINED,	
	"죽기",			// ACTION_DIE,		
	"흡혈",				// ACTION_DRAIN,			// 
	"TR",			// ACTION_SR,
	"AR",			// ACTION_AR,
	"검기본",			// ACTION_SWORD,
	"도기본",			// ACTION_BLADE,
	"던지기1",			// ACTION_THROW1,
	"오토바이이동",			// ACTION_BIKE_MOVE1,
	"SG",			// ACTION_SG,
	"SMG",			// ACTION_SMG,
	"검기술",			// ACTION_SWORD2,
	"도기술",			// ACTION_BLADE2,
	"던지기2",			// ACTION_THROW2,
	"오토바이정지",			// ACTION_BIKE_STOP1,	
	"TR SLOW",			// ACTION_SR_SLOW,
	"TR FAST",			// ACTION_SR_FAST,
	"AR SLOW",			// ACTION_AR_SLOW,
	"AR FAST",			// ACTION_AR_FAST,
	"SG SLOW",			// ACTION_SG_SLOW,
	"SG FAST",			// ACTION_SG_FAST,
	"SMG SLOW",			// ACTION_SMG_SLOW,
	"SMG FAST",				// ACTION_SMG_FAST,
	"검기본SLOW",			// ACTION_SWORD_SLOW,
	"검기본FAST",			// ACTION_SWORD_FAST,
	"도기본SLOW",			// ACTION_BLADE_SLOW,
	"도기본FAST",			// ACTION_BLADE_FAST,
	"검기술SLOW",			// ACTION_SWORD2_SLOW,
	"검기술FAST",			// ACTION_SWORD2_FAST,
	"도기술SLOW",			// ACTION_BLADE2_SLOW,
	"도기술FAST",			// ACTION_BLADE2_FAST,
};

// -------------------------------------------------
// directory찾기를 행할 때, 샷건과 자동소총
//	소총과 저격용총을 동일시하기 위해서..
// -------------------------------------------------
const char* strActionForSearch[ ACTION_MAX] = { 
	"정지",
	"걷기",
	"공격",
	"마법",
	"대미지",
	"흡혈당하기",
	"죽기",
	"흡혈",	
	"TR",
	"AR",
	"검",
	"도",
	"던지기",
	"오토바이",
	"SG",
	"SMG",
	"검2",
	"도2",
	"던지기",
	"오토바이",
	"TR",
	"TR",
	"AR",
	"AR",
	"SG",
	"SG",
	"SMG",
	"SMG",	
	"검",
	"검",
	"도",
	"도",
	"검",
	"검",
	"도",
	"도",
};

const char* strSlayerPart[SLAYER_PART_MAX] = {
							"구투구",		// 0
							"구상의",
							"구하의",
							"SMG",
							"도",
							"방패",
							"드래곤방패",
							"브론즈방패",
							"SG",
							"검",
							"AR",
							"머리1",
							"머리2",
							"머리3",
							"신투구",
							"신상의",
							"신하의",
							"십자무기",
							"TR",
							"오토바이",
							"맨상의",
							"맨하의",
};

///////////////////////////////////////////////////////
//	슬레이어인 경우와 아닌 경우 action의 수와 index가 틀리므로..
///////////////////////////////////////////////////////
const DWORD slayerActionIndex[ SLAYER_ACTION_MAX] =
{
	ACTION_STOP,
	ACTION_MOVE,
	ACTION_ATTACK,	
	ACTION_SKILL,			// magic
	ACTION_DAMAGED,	
	ACTION_DRAINED,	
	ACTION_DIE,		

	ACTION_SR,
	ACTION_AR,		// 소총
	ACTION_SWORD,		// 검기본
	ACTION_BLADE,	// 도기본
	ACTION_THROW1,			// 수류탄.
	ACTION_BIKE_MOVE1,		// 오토바이 이동1
	ACTION_SG,		// 샷건
	ACTION_SMG,		// 자동 소총.

	ACTION_SWORD2,
	ACTION_BLADE2,
	ACTION_THROW2,
	ACTION_BIKE_STOP1,
	ACTION_SR_SLOW,
	ACTION_SR_FAST,
	ACTION_AR_SLOW,
	ACTION_AR_FAST,
	ACTION_SG_SLOW,
	ACTION_SG_FAST,
	ACTION_SMG_SLOW,
	ACTION_SMG_FAST,
	ACTION_SWORD_SLOW,
	ACTION_SWORD_FAST,
	ACTION_BLADE_SLOW,
	ACTION_BLADE_FAST,
	ACTION_SWORD2_SLOW,
	ACTION_SWORD2_FAST,
	ACTION_BLADE2_SLOW,
	ACTION_BLADE2_FAST,
	
};

const BYTE s_AddonOrder[DIR_MAX][ADDON_MAX] = 
{
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_RIGHTHAND, ADDON_LEFTHAND },	// LEFT
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_RIGHTHAND, ADDON_LEFTHAND },	// LEFTDOWN
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_LEFTHAND, ADDON_RIGHTHAND },	// DOWN	
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_LEFTHAND, ADDON_RIGHTHAND },	// RIGHTDOWN	
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_LEFTHAND, ADDON_RIGHTHAND },	// RIGHT
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_LEFTHAND, ADDON_RIGHTHAND },	// RIGHTUP
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_RIGHTHAND, ADDON_LEFTHAND },	// UP
	{ ADDON_MOTOR, ADDON_TROUSER, ADDON_COAT, ADDON_HAIR, ADDON_HELM, ADDON_RIGHTHAND, ADDON_LEFTHAND }	// LEFTUP
};
