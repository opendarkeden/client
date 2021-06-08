#pragma once

const char* 	strOustersAction[ OUSTERS_ACTION_MAX ] = {
	"샤크람정지",
	"샤크람걷기",
	"샤크람공격",
	"마법시전",
	"대미지",
	"흡혈당하기",
	"죽기",
	"정지",
	"걷기",
	"샤크람기술",
	"마법공격",
	"흡영",
	"빠른이동정지",
	"빠른이동이동",
	"샤크람공격Slow",
	"샤크람공격Fast",
	"샤크람기술Slow",
	"샤크람기술Fast",
};

const char* strOustersActionForSearch[ OUSTERS_ACTION_MAX ] =
{
	"샤크람정지",
	"샤크람걷기",
	"샤크람공격",
	"마법시전",
	"대미지",
	"흡혈당하기",
	"죽기",
	"정지",
	"걷기",
	"샤크람기술",
	"마법공격",
	"흡영",
	"정지",
	"이동",
	"샤크람공격",
	"샤크람공격",
	"샤크람기술",
	"샤크람기술",
};

const char* strOustersPart[OUSTERS_PART_MAX] = {
	"샤크람",
	"옷1",
	"옷2",
	"옷3",
	"옷4",
};

const DWORD oustersActionIndex[ OUSTERS_ACTION_MAX ] = 
{
	OUSTERS_ACTION_SHAK_STOP,
	OUSTERS_ACTION_SHAK_MOVE,
	OUSTERS_ACTION_SHAK_ATTACK,
	OUSTERS_ACTION_SKILL,
	OUSTERS_ACTION_DAMAGED,
	OUSTERS_ACTION_DRAINED,
	OUSTERS_ACTION_DIE,
	
	OUSTERS_ACTION_STOP,
	OUSTERS_ACTION_MOVE,
	OUSTERS_ACTION_SHAK_SKILL,	
	OUSTERS_ACTION_MAGIC_ATTACK,
	OUSTERS_ACTION_GETTING_SOUL,
	OUSTERS_ACTION_FAST_MOVE_STOP,
	OUSTERS_ACTION_FAST_MOVE,
	OUSTERS_ACTION_SHAK_ATTACK_SLOW,
	OUSTERS_ACTION_SHAK_ATTACK_FAST,
	OUSTERS_ACTION_SHAK_SKILL_SLOW,
	OUSTERS_ACTION_SHAK_SKILL_FAST,
};

const char* strPromotionOustersAction[ ACTION_OUSTERS_PROMOTION_MAX ] =
{
	"차크람정지",
	"차크람걷기",
	"리스틀릿정지",
	"리스틀릿걷기",

	"공격SLOW",
	"공격NORMAL",
	"공격FAST",
	"스킬SLOW",
	"스킬NORMAL",
	"스킬FAST",

	"마법공격",

	"마법시전",
	"흡영",
	"흡혈당하기",
	"죽기",
	"대미지",
	"특수동작",
	"빠른이동정지",
	"빠른이동이동",
};

const char* strPromotionOustersActionForSearch[ ACTION_OUSTERS_PROMOTION_MAX ] =
{
	"차크람정지",
	"차크람걷기",
	"리스틀릿정지",
	"리스틀릿걷기",

	"공격",
	"공격",
	"공격",
	"스킬",
	"스킬",
	"스킬",

	"마법공격",

	"마법시전",
	"흡영",
	"흡혈당하기",
	"죽기",
	"대미지",
	"특수동작",
	"빠른이동",
	"빠른이동",
};

const DWORD g_PromotionOustersActionIndex[ ACTION_OUSTERS_PROMOTION_MAX ] = 
{
	ACTION_OUSTERS_PROMOTION_CHAKRAM_STOP,
	ACTION_OUSTERS_PROMOTION_CHAKRAM_MOVE,
	ACTION_OUSTERS_PROMOTION_WRISTLET_STOP,
	ACTION_OUSTERS_PROMOTION_WRISTLET_MOVE,

	ACTION_OUSTERS_PROMOTION_ATTACK_SLOW,
	ACTION_OUSTERS_PROMOTION_ATTACK_NORMAL,
	ACTION_OUSTERS_PROMOTION_ATTACK_FAST,
	ACTION_OUSTERS_PROMOTION_SKILL_SLOW,
	ACTION_OUSTERS_PROMOTION_SKILL_NORMAL,
	ACTION_OUSTERS_PROMOTION_SKILL_FAST,

	ACTION_OUSTERS_PROMOTION_MAGIC_ATTACK,

	ACTION_OUSTERS_PROMOTION_MAGIC,
	ACTION_OUSTERS_PROMOTION_ABSORB_SOUL,
	ACTION_OUSTERS_PROMOTION_DRAINED,
	ACTION_OUSTERS_PROMOTION_DIE,
	ACTION_OUSTERS_PROMOTION_DAMAGED,
	ACTION_OUSTERS_PROMOTION_SPECIAL,	

	ACTION_OUSTERS_PROMOTION_FAST_MOVE_STOP,
	ACTION_OUSTERS_PROMOTION_FAST_MOVE,
};

const char* strPromotionOustersPart[PO_PART_MAX] = {
	"샤크람",
	"옷1",	
};
