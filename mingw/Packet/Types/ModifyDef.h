#ifndef	__MODIFYDEF_H__
#define	__MODIFYDEF_H__



enum ModifyType {

	MODIFY_BASIC_STR = 0,        // 기본 STR을 변경
	MODIFY_CURRENT_STR,          // 현재 STR을 변경
	MODIFY_MAX_STR,              // 맥스 STR을 변경
	MODIFY_STR_EXP_REMAIN,              // STR 경험치를 변경

	MODIFY_BASIC_DEX,            // 기본 DEX를 변경
	MODIFY_CURRENT_DEX,          // 현재 DEX를 변경
	MODIFY_MAX_DEX,              // 맥스 DEX를 변경
	MODIFY_DEX_EXP_REMAIN,              // DEX 경험치를 변경

	MODIFY_BASIC_INT,            // 기본 INT를 변경
	MODIFY_CURRENT_INT,          // 현재 INT를 변경
	MODIFY_MAX_INT,              // 맥스 INT를 변경
	MODIFY_INT_EXP_REMAIN,              // INT 경험치를 변경

	MODIFY_CURRENT_HP,           // 현재 HP를 변경
	MODIFY_MAX_HP,               // 맥스 HP를 변경

	MODIFY_CURRENT_MP,           // 현재 MP를 변경
	MODIFY_MAX_MP,               // 맥스 MP를 변경

	MODIFY_MIN_DAMAGE,           // 최소 데미지를 변경
	MODIFY_MAX_DAMAGE,           // 맥스 데미지를 변경

	MODIFY_DEFENSE,              // 디펜스를 변경
	MODIFY_PROTECTION,           // 프로텍션을 변경
	MODIFY_TOHIT,                // 명중률을 변경

	MODIFY_VISION,               // 시야를 변경
	MODIFY_FAME,                 // 명성을 변경
	MODIFY_GOLD,                 // 소지금을 변경

	MODIFY_SWORD_DOMAIN_LEVEL,   // 검 계열 레벨을 변경
	MODIFY_SWORD_DOMAIN_EXP_REMAIN,     // 검 계열 레벨을 변경
	MODIFY_SWORD_DOMAIN_GOAL_EXP,// 검 계열 레벨을 변경
	MODIFY_BLADE_DOMAIN_LEVEL,   // 도 계열 레벨을 변경
	MODIFY_BLADE_DOMAIN_EXP_REMAIN,     // 도 계열 레벨을 변경
	MODIFY_BLADE_DOMAIN_GOAL_EXP,// 도 계열 레벨을 변경
	MODIFY_HEAL_DOMAIN_LEVEL,    // 힐 계열 레벨을 변경
	MODIFY_HEAL_DOMAIN_EXP_REMAIN,      // 힐 계열 레벨을 변경
	MODIFY_HEAL_DOMAIN_GOAL_EXP, // 힐 계열 레벨을 변경
	MODIFY_ENCHANT_DOMAIN_LEVEL, // 인챈트 계열 레벨을 변경
	MODIFY_ENCHANT_DOMAIN_EXP_REMAIN,   // 인챈트 계열 레벨을 변경
	MODIFY_ENCHANT_DOMAIN_GOAL_EXP, // 인챈트 계열 레벨을 변경
	MODIFY_GUN_DOMAIN_LEVEL,     // 총 계열 레벨을 변경
	MODIFY_GUN_DOMAIN_EXP_REMAIN,       // 총 계열 레벨을 변경
	MODIFY_GUN_DOMAIN_GOAL_EXP,  // 총 계열 레벨을 변경
	MODIFY_ETC_DOMAIN_LEVEL,     // 기타 계열 레벨을 변경
	MODIFY_ETC_DOMAIN_EXP_REMAIN,       // 기타 계열 레벨을 변경
	MODIFY_ETC_DOMAIN_GOAL_EXP,  // 기타 계열 레벨을 변경

	MODIFY_SKILL_LEVEL,          // 특정 스킬 레벨을 변경
	MODIFY_LEVEL,                // 뱀파이어 레벨을 변경
	MODIFY_EFFECT_STAT,          // 이펙트 상태를 변경
	MODIFY_DURATION,             // 이펙트 지속 시간을 변경
	MODIFY_BULLET,               // 들고 있는 총알의 총알 수를 변경
	MODIFY_BONUS_POINT,          // 뱀파이어의 보너스 포인트를 변경
	MODIFY_DURABILITY,           // 들고 있는 아이템 중 하나의 내구성을 변경
	MODIFY_NOTORIETY,            // 악명치를 변경
	MODIFY_VAMP_EXP_REMAIN,             // 뱀파이어의 경험치를 변경

	MODIFY_SILVER_DAMAGE,        // 은 데미지를 변경
	MODIFY_ATTACK_SPEED,         // 공격 속도를 변경
	MODIFY_ALIGNMENT,			// 성향

	MODIFY_SILVER_DURABILITY,	// 은도금 남은거..

	MODIFY_REGEN_RATE,           // 단위 시간당 리젠양을 변경 일단쌩-_-
	MODIFY_GUILDID,					// 길드 아이디 바꼈을때

	MODIFY_RANK,				 // 계급 단계
	MODIFY_RANK_EXP_REMAIN,			 // 계급 경험치

	MODIFY_OUSTERS_EXP_REMAIN,          // 아우스터스의 경험치를 변경
	MODIFY_SKILL_BONUS_POINT,    // 아우스터스의 스킬 보너스 포인트를 변경
	
	MODIFY_ELEMENTAL_FIRE,
	MODIFY_ELEMENTAL_WATER,
	MODIFY_ELEMENTAL_EARTH,
	MODIFY_ELEMENTAL_WIND,
	
	MODIFY_SKILL_EXP,			// 스킬 경험치 변경
	
    MODIFY_PET_HP,
    MODIFY_PET_EXP_REMAIN,               // 펫 관련
	
	MODIFY_LAST_TARGET,

	// 2004, 10, 19, sobeit add start
	MODIFY_UNIONID,
	MODIFY_UNIONGRADE,
	// 2004, 10, 19, sobeit add end
	
    MODIFY_ADVANCEMENT_CLASS_LEVEL, // 승직 레벨
    MODIFY_ADVANCEMENT_CLASS_GOAL_EXP,  // 승직 경험치

	MODIFY_MAX
};


#define	MAX_MODIFY		MODIFY_MAX
#define	MODIFY_NULL		0xFFFFFFFF

#endif