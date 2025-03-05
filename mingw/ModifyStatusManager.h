//-----------------------------------------------------------------------------
// ModifyStatusManager.h
//-----------------------------------------------------------------------------

#ifndef __MODIFYSTATUSMANAGER_H__
#define __MODIFYSTATUSMANAGER_H__

#include <Windows.h>
#include "MFunctionManager.h"

class MODIFY_VALUE {
	public :
		DWORD	oldValue;
		DWORD	newValue;
};

class ModifyStatusManager : public MFunctionManager {
	public :
		ModifyStatusManager();
		~ModifyStatusManager();

	public :
		static void Function_MODIFY_CURRENT_HP(void* pVoid);
		static void Function_MODIFY_CURRENT_MP(void* pVoid);
		static void Function_MODIFY_MAX_HP(void* pVoid);
		static void Function_MODIFY_MAX_MP(void* pVoid);
		static void Function_MODIFY_DURABILITY(void* pVoid);
		static void Function_MODIFY_STR_EXP_REMAIN(void* pVoid);
		static void Function_MODIFY_DEX_EXP_REMAIN(void* pVoid);
		static void Function_MODIFY_INT_EXP_REMAIN(void* pVoid);
		static void Function_MODIFY_VAMP_EXP_REMAIN(void* pVoid);
		static void Function_MODIFY_BULLET(void* pVoid);
		static void Function_MODIFY_SILVER_DURABILITY(void* pVoid);
		static void Function_MODIFY_VISION(void* pVoid);
		static void Function_MODIFY_GOLD(void* pVoid);
		static void Function_MODIFY_CURRENT_STR(void* pVoid);
		static void Function_MODIFY_CURRENT_DEX(void* pVoid);
		static void Function_MODIFY_CURRENT_INT(void* pVoid);
		static void Function_MODIFY_FAME(void* pVoid);
		static void Function_MODIFY_TOHIT(void* pVoid);
		static void Function_MODIFY_DEFENSE(void* pVoid);
		static void Function_MODIFY_PROTECTION(void* pVoid);
		static void Function_MODIFY_BONUS_POINT(void* pVoid);
		static void Function_MODIFY_ATTACK_SPEED(void* pVoid);
		static void Function_MODIFY_SWORD_DOMAIN_LEVEL(void* pVoid);
		static void Function_MODIFY_BLADE_DOMAIN_LEVEL(void* pVoid);
		static void Function_MODIFY_ENCHANT_DOMAIN_LEVEL(void* pVoid);
		static void Function_MODIFY_HEAL_DOMAIN_LEVEL(void* pVoid);
		static void Function_MODIFY_GUN_DOMAIN_LEVEL(void* pVoid);
		static void Function_MODIFY_LEVEL(void* pVoid);
		static void Function_MODIFY_ALIGNMENT(void* pVoid);
		static void Function_MODIFY_BASIC_STR(void* pVoid);
		static void Function_MODIFY_MAX_STR(void* pVoid);
		static void Function_MODIFY_BASIC_DEX(void* pVoid);
		static void Function_MODIFY_MAX_DEX(void* pVoid);
		static void Function_MODIFY_BASIC_INT(void* pVoid);
		static void Function_MODIFY_MAX_INT(void* pVoid);
		static void Function_MODIFY_SWORD_DOMAIN_EXP_REMAIN(void* pVoid);
		static void Function_MODIFY_BLADE_DOMAIN_EXP_REMAIN(void* pVoid);
		static void Function_MODIFY_HEAL_DOMAIN_EXP_REMAIN(void* pVoid);
		static void Function_MODIFY_ENCHANT_DOMAIN_EXP_REMAIN(void* pVoid);
		static void Function_MODIFY_GUN_DOMAIN_EXP_REMAIN(void* pVoid);
		static void Function_MODIFY_ETC_DOMAIN_EXP_REMAIN(void* pVoid);
		static void Function_MODIFY_MIN_DAMAGE(void* pVoid);
		static void Function_MODIFY_MAX_DAMAGE(void* pVoid);
		static void Function_MODIFY_SILVER_DAMAGE(void* pVoid);
		static void Function_MODIFY_GUILDID(void* pVoid);
		static void Function_MODIFY_RANK(void *pVoid);
		static void Function_MODIFY_RANK_EXP_REMAIN(void *pVoid);
		static void Function_MODIFY_OUSTERS_EXP_REMAIN(void *pVoid);
		static void Function_MODIFY_SKILL_POINT(void *pVoid);
		static void Function_MODIFY_ELEMENTAL_FIRE(void *pVoid);
		static void Function_MODIFY_ELEMENTAL_WATER(void *pVoid);
		static void Function_MODIFY_ELEMENTAL_EARTH(void *pVoid);
		static void Function_MODIFY_ELEMENTAL_WIND(void *pVoid);
		static void Function_MODIFY_SKILL_LEVEL(void *pVoid);
		static void Function_MODIFY_SKILL_EXP(void *pVoid);
		static void Function_MODIFY_PET_HP(void *pVoid);
		static void Function_MODIFY_PET_EXP_REMAIN(void *pVoid);
		static void Function_MODIFY_UNIONID(void *pVoid);
		static void Function_MODIFY_UNIONGRADE(void *pVoid);
		static void Function_MODIFY_ADVANCEMENT_CLASS_LEVEL(void *pVoid);
		static void Function_MODIFY_ADVANCEMENT_CLASS_GOAL_EXP(void *pVoid);
		
		

};

extern ModifyStatusManager*		g_pModifyStatusManager;

#endif

