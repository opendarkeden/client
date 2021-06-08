
#pragma once


const char* strVampirePart[VAMPIRE_PART_MAX] = {
							"코트1",
							"코트2",
							"코트3",
};

const DWORD vampireActionIndex[ VAMPIRE_ACTION_MAX] =
{
	ACTION_STOP,
	ACTION_MOVE,
	ACTION_ATTACK,	
	ACTION_SKILL,			// magic
	ACTION_DAMAGED,	
	ACTION_DRAINED,	
	ACTION_DIE,		
	ACTION_DRAIN,	
};


const DWORD npcActionIndex[ NPC_ACTION_MAX] =
{
	ACTION_STOP,
	ACTION_MOVE,
	ACTION_ATTACK,	
	ACTION_SKILL,			// magic
	ACTION_DAMAGED,	
	ACTION_DRAINED,	
	ACTION_DIE,			
};

const DWORD g_AdvancementClassVampireActionIndex[ ACTION_ADVANCEMENT_MAX ] =
{
	ACTION_ADVANCEMENT_STOP,
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
};

const char* strAdvancementClassVampirePart[ AC_PART_MAX ] = 
{
	"옷",
	"무기",
};

const char* strAdvancementClassVampireAction[ACTION_ADVANCEMENT_MAX] = {
	"정지",
	"걷기",
	"대미지",
	"죽기",
	"흡혈",
	"흡혈당하기",

	"공격SLOW",
	"공격NORMAL",
	"공격FAST",

	"스킬SLOW",
	"스킬NORMAL",
	"스킬FAST",

	"마법시전",
	"마법공격",
	"특수동작",
	"무기생성",
	"무기소멸",
};

const char* strAdvancementClassVampireActionForSearch[ ACTION_ADVANCEMENT_MAX ] =
{
	"정지",
	"걷기",
	"대미지",
	"죽기",
	"흡혈",
	"흡혈당하기",

	"공격",
	"공격",
	"공격",

	"스킬",
	"스킬",
	"스킬",

	"마법시전",
	"마법공격",
	"특수동작",
	"무기생성",
	"무기소멸",
};