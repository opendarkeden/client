//-----------------------------------------------------------------------------
// ModifyStatusManager.cpp
//-----------------------------------------------------------------------------
#include "Client_PCH.h"
#include "MZone.h"
#include "MTopView.h"
#include "MTypeDef.h"
#include "ModifyStatusManager.h"
#include "packet\types\ModifyDef.h"
#include "UIFunction.h"
#include "MPlayer.h"
#include "MSkillManager.h"
#include "ClientDef.h"
#include "MGameStringTable.h"
#include "MSlayerGear.h"
#include "MVampireGear.h"
#include "MOustersGear.h"
#include "ServerInfo.h"
#include "DebugInfo.h"
#include "UserInformation.h"
#include "ExperienceTable.h"
#include "VS_UI.h"
#include "VS_UI_Mouse_pointer.h"
#include "MFakeCreature.h"
#include "MHelpDef.h"
//-----------------------------------------------------------------------------
// Global
//-----------------------------------------------------------------------------
ModifyStatusManager*		g_pModifyStatusManager = NULL;

//-----------------------------------------------------------------------------
// define Functions
//-----------------------------------------------------------------------------
#define	EXTRACT_MODIFY_VALUE( pVoid )							\
																\
			MODIFY_VALUE* pValue	= (MODIFY_VALUE*)pVoid;		\
			DWORD	oldValue		= pValue->oldValue;			\
			DWORD	value			= pValue->newValue;			\


//----------------------------------------------------------------------
//
//							Global Functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Can Learn NewDomainSkill
//----------------------------------------------------------------------
bool
CanLearnDomainSkill(int domain)
{
	MSkillDomain& skillDomain = (*g_pSkillManager)[domain];
	int domainLevel = skillDomain.GetDomainLevel();
	
	//--------------------------------------------------
	// 새 기술을 배울 수 있는지 체크
	//--------------------------------------------------
	skillDomain.SetBegin();

	while (skillDomain.IsNotEnd())
	{
		ACTIONINFO					ai		= skillDomain.GetSkillID();
		MSkillDomain::SKILLSTATUS	status	= skillDomain.GetSkillStatus();

		int learnLevel = (*g_pSkillInfoTable)[ai].GetLearnLevel();		

		if (learnLevel>=0
			&& status != MSkillDomain::SKILLSTATUS_LEARNED
			&& learnLevel <= domainLevel
			&& ai != SKILL_MAGIC_ELUSION
			&& ai != SKILL_ILLUSION_OF_AVENGE
			&& ai != SKILL_WILL_OF_LIFE
			&& ai != SKILL_POISON_MESH
			&& ai != SKILL_DELEO_EFFICIO
			&& ai != SKILL_REPUTO_FACTUM)
		{
			DEBUG_ADD_FORMAT("[CanLearnNewSkill] skill=%s. learnLev=%d. domainLev=%d", (*g_pSkillInfoTable)[ai].GetName(), learnLevel, domainLevel);
			
			// 새로운 기술을 배울 수 있는 경우
			// 그냥 배울 수 있는지만 알면 된다.
			return true;
		}

		skillDomain.Next();
	}

	return false;
}

//----------------------------------------------------------------------
// Set Domain Level
//----------------------------------------------------------------------
// 이 domain의 새로운 기술을 배울 수 있는가?
//----------------------------------------------------------------------
void
SetDomainLevel( int domain, int domainLevel )
{
	MSkillDomain& skillDomain = (*g_pSkillManager)[domain];
	skillDomain.SetDomainLevel( domainLevel );

	if (g_Mode==MODE_GAME)
	{
		//--------------------------------------------------
		// 도메인 레벨이 얼마가 되었다고 표시
		//--------------------------------------------------
		int num1 = domainLevel % 10;
		if (num1==2 || num1==4 || num1==5 || num1==9)
		{
			g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_NEW_DOMAIN_LEVEL_1].GetString(), (*g_pGameStringTable)[SKILLDOMAIN_NAME[domain]].GetString(), domainLevel);
		}
		else
		{
			g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_NEW_DOMAIN_LEVEL_2].GetString(), (*g_pGameStringTable)[SKILLDOMAIN_NAME[domain]].GetString(), domainLevel);
		}

		//--------------------------------------------------
		// 새 기술을 배울 수 있는지 체크
		//--------------------------------------------------
		if (CanLearnDomainSkill(domain))
		{
			g_pGameMessage->Add((*g_pGameStringTable)[STRING_MESSAGE_NEW_SKILL_AVAILABLE].GetString());
		}	
		// 2004, 5, 6 sobeit add start
		switch(domainLevel)
		{
		case 5:
			ExecuteHelpEvent(HELP_EVENT_LEARN_SKILL);
			break;
		case 30:
			ExecuteHelpEvent(HELP_EVENT_LEVEL_30);
			break;
		case 31:
			ExecuteHelpEvent(HELP_EVENT_LEVEL_31);
			break;
		case 32:
			ExecuteHelpEvent(HELP_EVENT_LEVEL_32);
			break;
		}
		// 2004, 5, 6 sobeit add end
	}
}


//-----------------------------------------------------------------------------
//
// constructor
//
//-----------------------------------------------------------------------------
ModifyStatusManager::ModifyStatusManager()
{
	MFunctionManager::Init( MODIFY_MAX );

	MFunctionManager::SetFunction(MODIFY_CURRENT_HP, Function_MODIFY_CURRENT_HP);
	MFunctionManager::SetFunction(MODIFY_CURRENT_MP, Function_MODIFY_CURRENT_MP);
	MFunctionManager::SetFunction(MODIFY_MAX_HP, Function_MODIFY_MAX_HP);
	MFunctionManager::SetFunction(MODIFY_MAX_MP, Function_MODIFY_MAX_MP);
	MFunctionManager::SetFunction(MODIFY_DURABILITY, Function_MODIFY_DURABILITY);
	MFunctionManager::SetFunction(MODIFY_STR_EXP_REMAIN, Function_MODIFY_STR_EXP_REMAIN);
	MFunctionManager::SetFunction(MODIFY_DEX_EXP_REMAIN, Function_MODIFY_DEX_EXP_REMAIN);
	MFunctionManager::SetFunction(MODIFY_INT_EXP_REMAIN, Function_MODIFY_INT_EXP_REMAIN);
	MFunctionManager::SetFunction(MODIFY_VAMP_EXP_REMAIN, Function_MODIFY_VAMP_EXP_REMAIN);
	MFunctionManager::SetFunction(MODIFY_BULLET, Function_MODIFY_BULLET);
	MFunctionManager::SetFunction(MODIFY_SILVER_DURABILITY, Function_MODIFY_SILVER_DURABILITY);
	MFunctionManager::SetFunction(MODIFY_VISION, Function_MODIFY_VISION);
	MFunctionManager::SetFunction(MODIFY_GOLD, Function_MODIFY_GOLD);
	MFunctionManager::SetFunction(MODIFY_CURRENT_STR, Function_MODIFY_CURRENT_STR);
	MFunctionManager::SetFunction(MODIFY_CURRENT_DEX, Function_MODIFY_CURRENT_DEX);
	MFunctionManager::SetFunction(MODIFY_CURRENT_INT, Function_MODIFY_CURRENT_INT);
	MFunctionManager::SetFunction(MODIFY_FAME, Function_MODIFY_FAME);
	MFunctionManager::SetFunction(MODIFY_TOHIT, Function_MODIFY_TOHIT);
	MFunctionManager::SetFunction(MODIFY_DEFENSE, Function_MODIFY_DEFENSE);
	MFunctionManager::SetFunction(MODIFY_PROTECTION, Function_MODIFY_PROTECTION);
	MFunctionManager::SetFunction(MODIFY_BONUS_POINT, Function_MODIFY_BONUS_POINT);
	MFunctionManager::SetFunction(MODIFY_ATTACK_SPEED, Function_MODIFY_ATTACK_SPEED);
	MFunctionManager::SetFunction(MODIFY_SWORD_DOMAIN_LEVEL, Function_MODIFY_SWORD_DOMAIN_LEVEL);
	MFunctionManager::SetFunction(MODIFY_BLADE_DOMAIN_LEVEL, Function_MODIFY_BLADE_DOMAIN_LEVEL);
	MFunctionManager::SetFunction(MODIFY_ENCHANT_DOMAIN_LEVEL, Function_MODIFY_ENCHANT_DOMAIN_LEVEL);
	MFunctionManager::SetFunction(MODIFY_HEAL_DOMAIN_LEVEL, Function_MODIFY_HEAL_DOMAIN_LEVEL);
	MFunctionManager::SetFunction(MODIFY_GUN_DOMAIN_LEVEL, Function_MODIFY_GUN_DOMAIN_LEVEL);
	MFunctionManager::SetFunction(MODIFY_LEVEL, Function_MODIFY_LEVEL);
	MFunctionManager::SetFunction(MODIFY_ALIGNMENT, Function_MODIFY_ALIGNMENT);
	MFunctionManager::SetFunction(MODIFY_BASIC_STR, Function_MODIFY_BASIC_STR);
	MFunctionManager::SetFunction(MODIFY_MAX_STR, Function_MODIFY_MAX_STR);
	MFunctionManager::SetFunction(MODIFY_BASIC_DEX, Function_MODIFY_BASIC_DEX);
	MFunctionManager::SetFunction(MODIFY_MAX_DEX, Function_MODIFY_MAX_DEX);
	MFunctionManager::SetFunction(MODIFY_BASIC_INT, Function_MODIFY_BASIC_INT);
	MFunctionManager::SetFunction(MODIFY_MAX_INT, Function_MODIFY_MAX_INT);
	MFunctionManager::SetFunction(MODIFY_SWORD_DOMAIN_EXP_REMAIN, Function_MODIFY_SWORD_DOMAIN_EXP_REMAIN);
	MFunctionManager::SetFunction(MODIFY_BLADE_DOMAIN_EXP_REMAIN, Function_MODIFY_BLADE_DOMAIN_EXP_REMAIN);
	MFunctionManager::SetFunction(MODIFY_HEAL_DOMAIN_EXP_REMAIN, Function_MODIFY_HEAL_DOMAIN_EXP_REMAIN);
	MFunctionManager::SetFunction(MODIFY_ENCHANT_DOMAIN_EXP_REMAIN, Function_MODIFY_ENCHANT_DOMAIN_EXP_REMAIN);
	MFunctionManager::SetFunction(MODIFY_GUN_DOMAIN_EXP_REMAIN, Function_MODIFY_GUN_DOMAIN_EXP_REMAIN);
	MFunctionManager::SetFunction(MODIFY_ETC_DOMAIN_EXP_REMAIN, Function_MODIFY_ETC_DOMAIN_EXP_REMAIN);
	MFunctionManager::SetFunction(MODIFY_MIN_DAMAGE, Function_MODIFY_MIN_DAMAGE);
	MFunctionManager::SetFunction(MODIFY_MAX_DAMAGE, Function_MODIFY_MAX_DAMAGE);
	MFunctionManager::SetFunction(MODIFY_SILVER_DAMAGE, Function_MODIFY_SILVER_DAMAGE);
	MFunctionManager::SetFunction(MODIFY_GUILDID, Function_MODIFY_GUILDID);
	MFunctionManager::SetFunction(MODIFY_RANK, Function_MODIFY_RANK);
	MFunctionManager::SetFunction(MODIFY_RANK_EXP_REMAIN, Function_MODIFY_RANK_EXP_REMAIN);
	MFunctionManager::SetFunction(MODIFY_OUSTERS_EXP_REMAIN, Function_MODIFY_OUSTERS_EXP_REMAIN);
	MFunctionManager::SetFunction(MODIFY_SKILL_BONUS_POINT, Function_MODIFY_SKILL_POINT);
	MFunctionManager::SetFunction(MODIFY_ELEMENTAL_FIRE, Function_MODIFY_ELEMENTAL_FIRE);
	MFunctionManager::SetFunction(MODIFY_ELEMENTAL_WATER, Function_MODIFY_ELEMENTAL_WATER);
	MFunctionManager::SetFunction(MODIFY_ELEMENTAL_EARTH, Function_MODIFY_ELEMENTAL_EARTH);
	MFunctionManager::SetFunction(MODIFY_ELEMENTAL_WIND, Function_MODIFY_ELEMENTAL_WIND);
	MFunctionManager::SetFunction(MODIFY_SKILL_LEVEL, Function_MODIFY_SKILL_LEVEL);
	MFunctionManager::SetFunction(MODIFY_SKILL_EXP, Function_MODIFY_SKILL_EXP);
	MFunctionManager::SetFunction(MODIFY_PET_HP, Function_MODIFY_PET_HP);
	MFunctionManager::SetFunction(MODIFY_PET_EXP_REMAIN, Function_MODIFY_PET_EXP_REMAIN);

	MFunctionManager::SetFunction(MODIFY_UNIONID, Function_MODIFY_UNIONID);
	MFunctionManager::SetFunction(MODIFY_UNIONGRADE, Function_MODIFY_UNIONGRADE);
	
	MFunctionManager::SetFunction(MODIFY_ADVANCEMENT_CLASS_LEVEL, Function_MODIFY_ADVANCEMENT_CLASS_LEVEL);
	MFunctionManager::SetFunction(MODIFY_ADVANCEMENT_CLASS_GOAL_EXP, Function_MODIFY_ADVANCEMENT_CLASS_GOAL_EXP);
}

ModifyStatusManager::~ModifyStatusManager()
{
}

//-----------------------------------------------------------------------------
// MODIFY_CURRENT_HP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_CURRENT_HP(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	pValue->newValue = value = min(g_pPlayer->GetMAX_HP()-g_pPlayer->GetSilverDamage(), value);	

	UI_SetHP( value, g_pPlayer->GetMAX_HP() );
	UI_SetCharInfoHP( value );		
	
	// skill마다 MP가 충분한지 체크
	if (g_pPlayer->IsVampire())
	{
		g_pSkillAvailable->CheckMP();
	}
}

//-----------------------------------------------------------------------------
// MODIFY_CURRENT_HP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_CURRENT_MP(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	//pValue->newValue = value = min(g_pPlayer->GetMAX_MP(), value);
	pValue->newValue = value;

	DEBUG_ADD_FORMAT("[UI] Set MP  (%d)", value); 
	UI_SetMP( value, g_pPlayer->GetMAX_MP() );	
	UI_SetCharInfoMP( value );

	// skill마다 MP가 충분한지 체크
	if (!g_pPlayer->IsVampire())
	{
		g_pSkillAvailable->CheckMP();
	}
}

//-----------------------------------------------------------------------------
// MODIFY_MAX_HP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_MAX_HP(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	UI_SetHP( g_pPlayer->GetHP(), value );	
	UI_SetCharInfoHP_max( value );

	if (g_Mode==MODE_GAME)
	{
		int num1 = value % 10;
		if (num1==2 || num1==4 || num1==5 || num1==9)
		{
			g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_STATUS_HP_MAX_1].GetString(), value);
		}
		else
		{
			g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_STATUS_HP_MAX_2].GetString(), value);
		}
	}
}

//-----------------------------------------------------------------------------
// MODIFY_MAX_MP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_MAX_MP(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	UI_SetMP( g_pPlayer->GetMP(), value );	
	UI_SetCharInfoMP_max( value );

	// skill마다 MP가 충분한지 체크
	//(*g_pSkillAvailable).CheckMP();

	if (g_Mode==MODE_GAME)
	{
		int num1 = value % 10;
		int stringID;

		if (num1==2 || num1==4 || num1==5 || num1==9)
		{
			stringID = STRING_STATUS_MP_MAX_1;
			if(g_pPlayer->IsOusters())
				stringID = STRING_STATUS_EP_MAX_1;
		}
		else
		{
			stringID = STRING_STATUS_MP_MAX_2;
			if(g_pPlayer->IsOusters())
				stringID = STRING_STATUS_EP_MAX_2;
		}
		g_pGameMessage->AddFormat((*g_pGameStringTable)[stringID].GetString(), value);
	}
}

//-----------------------------------------------------------------------------
// MODIFY_DURABILITY
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_DURABILITY(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )


	// 2005, 2, 23, sobeit modify start - 1BYTE | 3BYTE 로 수정
	int slotID		= (value & 0xFF000000) >> 24;
	int	durability	= value & 0x00FFFFFF;	// 빼준다.
//	int slotID		= (value & 0xFFFF0000) >> 16;
//	int	durability	= value & 0x0000FFFF;	// 빼준다.
	// 2005, 2, 23, sobeit modify end

	#ifdef	OUTPUT_DEBUG
		DEBUG_ADD_FORMAT("[MODIFY_DURABILITY] slot=%d, value=%d", (int)slotID, (int)durability);

		bool bModified;
	#endif			

	
	switch(g_pPlayer->GetRace())
	{
	case RACE_SLAYER:
	{
		//-------------------------------------------------
		// 오른손에 무기 들고 있는데.
		// 서버에서는 왼손이라고 날아오기도 한다. - -;
		//-------------------------------------------------
		const MItem* pItem = g_pSlayerGear->GetItem( (MSlayerGear::GEAR_SLAYER)slotID );

		if (pItem!=NULL)
		{
			// 양손 무기 이면.. 무조건 오른손으로 slot 고정
			if (pItem->GetGearSlot()==MItem::SLOT_TWOHAND)
			{
				slotID = (BYTE)MSlayerGear::GEAR_SLAYER_RIGHTHAND;
			}									

			#ifdef	OUTPUT_DEBUG
				bModified = 
			#endif

			//pItem->GetCurrentDurability();

			g_pSlayerGear->ModifyDurability(slotID, durability);
		}
	}
	break;

	case RACE_VAMPIRE:
	{
		//-------------------------------------------------
		// 오른손에 무기 들고 있는데.
		// 서버에서는 왼손이라고 날아오기도 한다. - -;
		//-------------------------------------------------
		const MItem* pItem = g_pVampireGear->GetItem( (MVampireGear::GEAR_VAMPIRE)slotID );

		if (pItem!=NULL)
		{
			// 양손 무기 이면.. 무조건 오른손으로 slot 고정
			if (pItem->GetGearSlot()==MItem::SLOT_VAMPIRE_TWOHAND)
			{
				slotID = (BYTE)MVampireGear::GEAR_VAMPIRE_RIGHTHAND;
			}									

			#ifdef	OUTPUT_DEBUG
				bModified = 
			#endif

			//pItem->GetCurrentDurability();

			g_pVampireGear->ModifyDurability(slotID, durability);
		}
	}
	break;

	case RACE_OUSTERS:
		{
			//-------------------------------------------------
			// 오른손에 무기 들고 있는데.
			// 서버에서는 왼손이라고 날아오기도 한다. - -;
			//-------------------------------------------------
			const MItem* pItem = g_pOustersGear->GetItem( (MOustersGear::GEAR_OUSTERS)slotID );
			
			if (pItem!=NULL)
			{
				// 양손 무기 이면.. 무조건 오른손으로 slot 고정
				if (pItem->GetGearSlot()==MItem::SLOT_OUSTERS_TWOHAND)
				{
					slotID = (BYTE)MOustersGear::GEAR_OUSTERS_RIGHTHAND;
				}									
				
#ifdef	OUTPUT_DEBUG
				bModified = 
#endif
					
					//pItem->GetCurrentDurability();
					
					g_pOustersGear->ModifyDurability(slotID, durability);
			}
		}
		break;
	}

	#ifdef	OUTPUT_DEBUG
		if (!bModified)
		{
			DEBUG_ADD_FORMAT("[Error] Can't Modify Durability. slot=%d, value=%d", (int)slotID, (int)durability);	
		}
	#endif
}
	
//-----------------------------------------------------------------------------
// MODIFY_STR_EXP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_STR_EXP_REMAIN(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	g_char_slot_ingame.STR_EXP_REMAIN = value;
}	

//-----------------------------------------------------------------------------
// MODIFY_DEX_EXP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_DEX_EXP_REMAIN(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	g_char_slot_ingame.DEX_EXP_REMAIN = value;
}

//-----------------------------------------------------------------------------
// MODIFY_INT_EXP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_INT_EXP_REMAIN(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	g_char_slot_ingame.INT_EXP_REMAIN = value;
}
	
//-----------------------------------------------------------------------------
// MODIFY_VAMP_EXP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_VAMP_EXP_REMAIN(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	g_char_slot_ingame.EXP_REMAIN = value;
}
	
//-----------------------------------------------------------------------------
// MODIFY_BULLET
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_BULLET(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
//	g_pSystemMessage->Add("총알 받다.");

	if (g_pCurrentMagazine!=NULL)
	{
		g_pCurrentMagazine->SetNumber( value );
	}
}	

//-----------------------------------------------------------------------------
// MODIFY_SILVER_DURABILITY
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_SILVER_DURABILITY(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	if (g_pPlayer->IsSlayer())
	{
		const MItem* pWeapon = g_pSlayerGear->GetItem( MSlayerGear::GEAR_SLAYER_RIGHTHAND );

		if (pWeapon!=NULL)
		{
			MItem* pModifyWeapon = g_pSlayerGear->GetItemToModify( pWeapon->GetID() );

			if (pModifyWeapon!=NULL)
			{
				pModifyWeapon->SetSilver( value );
			}
		}
	}
}	

//-----------------------------------------------------------------------------
// MODIFY_VISION
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_VISION(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	//--------------------------------------------------------
	// (m_ServerX, m_ServerY)에서부터 
	// Player의 시야에 포함되지 않는 Creature를 Zone에서 제거시킨다.
	//--------------------------------------------------------
	g_pPlayer->SetSight( value );
	g_pZone->KeepObjectInSight(g_pPlayer->GetServerX(), 
								g_pPlayer->GetServerY(), 
								g_pPlayer->GetSight());
}

//-----------------------------------------------------------------------------
// MODIFY_GOLD
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_GOLD(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	g_pMoneyManager->SetMoney( value );

}

//-----------------------------------------------------------------------------
// MODIFY_CURRENT_STR
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_CURRENT_STR(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	DEBUG_ADD_FORMAT("MODIFY_CURRENT_STR = %d", value);
			
	if (g_Mode==MODE_GAME)
	{
		int num1 = value % 10;
		if (num1==2 || num1==4 || num1==5 || num1==9)
		{
			g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_STATUS_STR_1].GetString(), value);
		}
		else
		{
			g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_STATUS_STR_2].GetString(), value);
		}
	}
	
	//C_VS_UI_SLAYER_PDS::m_str		= value;
	//C_VS_UI_SLAYER_PDS::m_str_max	= value;
	UI_SetCharInfoSTR( value );
	
	//------------------------------------------
	// 다른 값 바뀌는 것 계산
	//------------------------------------------
	MPlayerGear *pGear = g_pPlayer->GetGear();

	g_pInventory->CheckAffectStatusAll();
	pGear->CheckAffectStatusAll();				

	//------------------------------------------
	// 바뀌는 수치 계산
	//------------------------------------------
	g_pPlayer->CalculateStatus();
}

//-----------------------------------------------------------------------------
// MODIFY_CURRENT_DEX
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_CURRENT_DEX(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	if (g_Mode==MODE_GAME)
	{
		int num1 = value % 10;
		if (num1==2 || num1==4 || num1==5 || num1==9)
		{
			g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_STATUS_DEX_1].GetString(), value);
		}
		else
		{
			g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_STATUS_DEX_2].GetString(), value);
		}
	}
	//C_VS_UI_SLAYER_PDS::m_dex		= value;
	//C_VS_UI_SLAYER_PDS::m_dex_max	= value;
	UI_SetCharInfoDEX( value );

	//------------------------------------------
	// 다른 값 바뀌는 것 계산
	//------------------------------------------
	MPlayerGear *pGear = g_pPlayer->GetGear();
	
	g_pInventory->CheckAffectStatusAll();
	pGear->CheckAffectStatusAll();				
		
	//------------------------------------------
	// 바뀌는 수치 계산
	//------------------------------------------
	g_pPlayer->CalculateStatus();					
}

//-----------------------------------------------------------------------------
// MODIFY_CURRENT_INT
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_CURRENT_INT(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	if (g_Mode==MODE_GAME)
	{
		int num1 = value % 10;
		if (num1==2 || num1==4 || num1==5 || num1==9)
		{
			g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_STATUS_INT_1].GetString(), value);
		}
		else
		{
			g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_STATUS_INT_2].GetString(), value);
		}
	}
	//C_VS_UI_SLAYER_PDS::m_int		= value;
	//C_VS_UI_SLAYER_PDS::m_int_max	= value;
	UI_SetCharInfoINT( value );

	//------------------------------------------
	// 다른 값 바뀌는 것 계산
	//------------------------------------------
	switch(g_pPlayer->GetRace())
	{
		case RACE_SLAYER:
			//------------------------------------------
			// 사용가능한지 체크한다.
			//------------------------------------------
			g_pInventory->CheckAffectStatusAll();
			g_pSlayerGear->CheckAffectStatusAll();
			break;
			
		case RACE_VAMPIRE:
			g_pInventory->CheckAffectStatusAll();
			g_pVampireGear->CheckAffectStatusAll();
			break;
			
		case RACE_OUSTERS:
			g_pInventory->CheckAffectStatusAll();
			g_pOustersGear->CheckAffectStatusAll();
			break;
	}

	//------------------------------------------
	// 바뀌는 수치 계산
	//------------------------------------------
	g_pPlayer->CalculateStatus();		
}

//-----------------------------------------------------------------------------
// MODIFY_FAME
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_FAME(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
  
	UI_SetCharInfoFAME( value );
}

//-----------------------------------------------------------------------------
// MODIFY_TOHIT
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_TOHIT(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
  
	UI_SetCharInfoTOHIT( value );
}

//-----------------------------------------------------------------------------
// MODIFY_DEFENSE
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_DEFENSE(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
  
	UI_SetCharInfoDefense( value );
}

//-----------------------------------------------------------------------------
// MODIFY_PROTECTION
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_PROTECTION(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
  
	UI_SetCharInfoProtection( value );
}
	
//-----------------------------------------------------------------------------
// MODIFY_BONUS_POINT
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_BONUS_POINT(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
  
	if (value!=0)
	{
		UI_LevelUp();
	}

	UI_SetBonusPoint( value );

	// [도움말] bonus point
//	__BEGIN_HELP_EVENT
//		ExecuteHelpEvent( HE_STATUS_BONUS_POINT );	
//	__END_HELP_EVENT
}
	
//-----------------------------------------------------------------------------
// MODIFY_ATTACK_SPEED
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_ATTACK_SPEED(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
  
	g_pPlayer->SetWeaponSpeed( value );
}

//-----------------------------------------------------------------------------
// MODIFY_SWORD_DOMAIN_LEVEL
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_SWORD_DOMAIN_LEVEL(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	SetDomainLevel( SKILLDOMAIN_SWORD, value );
}

//-----------------------------------------------------------------------------
// MODIFY_BLADE_DOMAIN_LEVEL
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_BLADE_DOMAIN_LEVEL(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	SetDomainLevel( SKILLDOMAIN_BLADE, value );
}

//-----------------------------------------------------------------------------
// MODIFY_ENCHANT_DOMAIN_LEVEL
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_ENCHANT_DOMAIN_LEVEL(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	SetDomainLevel( SKILLDOMAIN_ENCHANT, value );
}

//-----------------------------------------------------------------------------
// MODIFY_HEAL_DOMAIN_LEVEL
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_HEAL_DOMAIN_LEVEL(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	SetDomainLevel( SKILLDOMAIN_HEAL, value );
}


//-----------------------------------------------------------------------------
// MODIFY_GUN_DOMAIN_LEVEL
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_GUN_DOMAIN_LEVEL(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	SetDomainLevel( SKILLDOMAIN_GUN, value );
}

//-----------------------------------------------------------------------------
// MODIFY_LEVEL
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_LEVEL(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	(*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].SetDomainLevel( value );

	if (g_Mode==MODE_GAME)
	{
		g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_STATUS_LEVEL].GetString(), value);				

		if (CanLearnDomainSkill(SKILLDOMAIN_VAMPIRE))
		{
			g_pGameMessage->Add((*g_pGameStringTable)[STRING_MESSAGE_NEW_SKILL_AVAILABLE].GetString());
		}
		// 2004, 5, 6 sobeit add start
		switch(value)
		{
		case 5:
			ExecuteHelpEvent(HELP_EVENT_LEARN_SKILL);
			break;
		case 30:
			ExecuteHelpEvent(HELP_EVENT_LEVEL_30);
			break;
		case 31:
			ExecuteHelpEvent(HELP_EVENT_LEVEL_31);
			break;
		case 32:
			ExecuteHelpEvent(HELP_EVENT_LEVEL_32);
			break;
		}
		// 2004, 5, 6 sobeit add end

		UI_SetCharInfoLevel( value );
	}

	//------------------------------------------
	// 사용가능한지 체크한다.
	//------------------------------------------
	MPlayerGear *pGear = g_pPlayer->GetGear();
	
	g_pInventory->CheckAffectStatusAll();
	pGear->CheckAffectStatusAll();				

}

	
//-----------------------------------------------------------------------------
// MODIFY_ALIGNMENT
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_ALIGNMENT(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
			
	const int numAlignment = 5;

	//#ifdef	OUTPUT_DEBUG
	//	if (g_pGameMessage!=NULL)
	//		g_pGameMessage->AddFormat("oldValue=%d. newAlignment=%d, modifyAlignment=%d", oldValue, value, -(~(short)value + 1));
	//#endif		

	//--------------------------------------------------
	// UI에 설정
	//--------------------------------------------------
	short uiAlignment = value;
	if (uiAlignment > 10000)
	{	
		uiAlignment = - (short)(~uiAlignment + 1);
	}
	g_char_slot_ingame.alignment_num = uiAlignment;

	//--------------------------------------------------
	// -10000 ~ 10000을 0~4로 바꾼다.
	//--------------------------------------------------
	int alignment;
	int oldAlignment;

	if (value > 10000)
	{	
		pValue->newValue = value = - (short)(~value + 1);
	}

	if (oldValue > 10000)
	{	
		oldValue = - (short)(~oldValue + 1);
	}

	alignment = ConvertAlignment( value );
	oldAlignment = ConvertAlignment( oldValue );

	//--------------------------------------------------
	// 성향이 조금 바꼈다는 메세지
	//--------------------------------------------------
	if (g_Mode==MODE_GAME)
	{
		if (value > oldValue)
		{
			g_pGameMessage->Add((*g_pGameStringTable)[STRING_CHANGE_TO_GOOD_ALIGNMENT].GetString());
		}
		else if (value < oldValue)
		{
			g_pGameMessage->Add((*g_pGameStringTable)[STRING_CHANGE_TO_BAD_ALIGNMENT].GetString());
			
			// 2004, 5, 6 sobeit add start
			ExecuteHelpEvent(HELP_EVENT_FAME);
			// 2004, 5, 6 sobeit add end
		}
	}


	//#ifdef	OUTPUT_DEBUG
	//	if (g_pGameMessage!=NULL)
	//		g_pGameMessage->AddFormat("newValue=%d", alignment);
	//#endif

	//--------------------------------------------------
	// 성향의 단계가 바뀐 경우..
	//--------------------------------------------------
	if (oldAlignment != alignment)
	{	
		const char* alignmentName[numAlignment] =
		{
			"Evil-",
			"Evil",
			"Neutral",
			"Good",
			"Good+"
		};

		int stringSelect[numAlignment] =
		{
			STRING_CHANGE_ALIGNMENT2,
			STRING_CHANGE_ALIGNMENT2,
			STRING_CHANGE_ALIGNMENT2,
			STRING_CHANGE_ALIGNMENT,
			STRING_CHANGE_ALIGNMENT
		};

		if (g_Mode==MODE_GAME)
		{
			char str[256];
			sprintf(str, (*g_pGameStringTable)[stringSelect[alignment]].GetString(), alignmentName[alignment]);
			g_pGameMessage->Add(str);					

			g_char_slot_ingame.alignment = (ALIGNMENT)alignment;
		}				
	}
}

//-----------------------------------------------------------------------------
// MODIFY_BASIC_STR
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_BASIC_STR(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	g_char_slot_ingame.STR_PURE = value;
}

//-----------------------------------------------------------------------------
// MODIFY_MAX_STR
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_MAX_STR(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	DEBUG_ADD_FORMAT("MODIFY_MAX_STR = %d", value);
			
	g_char_slot_ingame.STR_MAX = value;			
}
		
//-----------------------------------------------------------------------------
// MODIFY_BASIC_DEX
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_BASIC_DEX(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	g_char_slot_ingame.DEX_PURE = value;	

	g_pPlayer->CheckRegen();
}	

//-----------------------------------------------------------------------------
// MODIFY_MAX_DEX
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_MAX_DEX(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	g_char_slot_ingame.DEX_MAX = value;			
}
	
//-----------------------------------------------------------------------------
// MODIFY_BASIC_INT
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_BASIC_INT(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
	
	g_char_slot_ingame.INT_PURE = value;
}

//-----------------------------------------------------------------------------
// MODIFY_MAX_INT
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_MAX_INT(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
	
	g_char_slot_ingame.INT_MAXX= value;
}

//-----------------------------------------------------------------------------
// MODIFY_SWORD_DOMAIN_EXP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_SWORD_DOMAIN_EXP_REMAIN(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	(*g_pSkillManager)[SKILLDOMAIN_SWORD].SetDomainExpRemain( value );
}

//-----------------------------------------------------------------------------
// MODIFY_BLADE_DOMAIN_EXP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_BLADE_DOMAIN_EXP_REMAIN(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	(*g_pSkillManager)[SKILLDOMAIN_BLADE].SetDomainExpRemain( value );
}

//-----------------------------------------------------------------------------
// MODIFY_HEAL_DOMAIN_EXP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_HEAL_DOMAIN_EXP_REMAIN(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	(*g_pSkillManager)[SKILLDOMAIN_HEAL].SetDomainExpRemain( value );
}

//-----------------------------------------------------------------------------
// MODIFY_ENCHANT_DOMAIN_EXP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_ENCHANT_DOMAIN_EXP_REMAIN(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	(*g_pSkillManager)[SKILLDOMAIN_ENCHANT].SetDomainExpRemain( value );
}

//-----------------------------------------------------------------------------
// MODIFY_GUN_DOMAIN_EXP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_GUN_DOMAIN_EXP_REMAIN(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	(*g_pSkillManager)[SKILLDOMAIN_GUN].SetDomainExpRemain( value );
}

//-----------------------------------------------------------------------------
// MODIFY_ETC_DOMAIN_EXP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_ETC_DOMAIN_EXP_REMAIN(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	(*g_pSkillManager)[SKILLDOMAIN_ETC].SetDomainExpRemain( value );
}
	
//-----------------------------------------------------------------------------
// MODIFY_MIN_DAMAGE
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_MIN_DAMAGE(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
			
	// 이거 말고 그냥 damage만 계산하도록 바꿔야 한다.. 
	// 일단 잊을까봐 임시로.. - -;;
	g_pPlayer->CalculateStatus();
}

//-----------------------------------------------------------------------------
// MODIFY_MAX_DAMAGE
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_MAX_DAMAGE(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
			
	g_pPlayer->CalculateStatus();
}

//-----------------------------------------------------------------------------
// MODIFY_SILVER_DAMAGE
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_SILVER_DAMAGE(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )			
	
	UI_SetCharInfoSilverDamage( value );

	UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );	
}

//-----------------------------------------------------------------------------
// MODIFY_GUILDID
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_GUILDID(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )			

	g_pPlayer->SetGuildNumber(value);

	g_char_slot_ingame.GUILD_ID = value;
}

//-----------------------------------------------------------------------------
// MODIFY_RANK
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_RANK(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )			

	g_char_slot_ingame.GRADE = value;

	if(value > oldValue)
		g_pGameMessage->Add((*g_pGameStringTable)[STRING_MESSAGE_UP_TO_GRADE].GetString());
}

//-----------------------------------------------------------------------------
// MODIFY_RANK_EXP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_RANK_EXP_REMAIN(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )			

//	int accum = 0;
//	accum = g_pExperienceTable->GetRankInfo(g_char_slot_ingame.GRADE, g_pPlayer->GetRace()).AccumExp;
//	g_char_slot_ingame.GRADE_EXP_REMAIN = accum-value;
	g_char_slot_ingame.GRADE_EXP_REMAIN = value;
}

//-----------------------------------------------------------------------------
// MODIFY_OUSTERS_EXP
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_OUSTERS_EXP_REMAIN(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )			

	g_char_slot_ingame.EXP_REMAIN = value;
}


//-----------------------------------------------------------------------------
// MODIFY_BONUS_POINT
//-----------------------------------------------------------------------------
void		
ModifyStatusManager::Function_MODIFY_SKILL_POINT(void* pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
		
//		if (value!=0)
//		{
//			UI_LevelUp();
//		}
		
		UI_SetSkillPoint( value );
		
		// [도움말] bonus point
//		__BEGIN_HELP_EVENT
//			ExecuteHelpEvent( HE_STATUS_BONUS_POINT );	
//		__END_HELP_EVENT
}

void
ModifyStatusManager::Function_MODIFY_ELEMENTAL_FIRE(void *pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	UI_SetElementalValue(ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE, value);
}

void
ModifyStatusManager::Function_MODIFY_ELEMENTAL_WATER(void *pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
		
	UI_SetElementalValue(ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER, value);
}

void
ModifyStatusManager::Function_MODIFY_ELEMENTAL_EARTH(void *pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
		
	UI_SetElementalValue(ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH, value);
}

void
ModifyStatusManager::Function_MODIFY_ELEMENTAL_WIND(void *pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
		
	UI_SetElementalValue(ITEMTABLE_INFO::ELEMENTAL_TYPE_WIND, value);
}

void 
ModifyStatusManager::Function_MODIFY_SKILL_LEVEL(void *pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	SkillType_t		SkillID = HIWORD( value );
	WORD			SkillLev = LOWORD( value );

	if( SkillID >= 0 && SkillID < MAX_ACTIONINFO && g_pSkillInfoTable != NULL)
	{
		(*g_pSkillInfoTable)[SkillID].SetExpLevel( SkillLev );

		if (g_Mode==MODE_GAME)
		{
			int num1 = SkillLev % 10;
			if (num1==2 || num1==4 || num1==5 || num1==9)
			{
				g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_MODIFY_SKILL_LEVEL_2].GetString(), 
					(*g_pSkillInfoTable)[SkillID].GetHName(), SkillLev );
			}
			else
			{
				g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_MODIFY_SKILL_LEVEL_1].GetString(), 
					(*g_pSkillInfoTable)[SkillID].GetHName(), SkillLev);
			}
		}
	}
}

void 
ModifyStatusManager::Function_MODIFY_SKILL_EXP(void *pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	SkillType_t		SkillID = HIWORD( value );
	int				SkillExp = LOWORD( value ) * 10;

	if( SkillID >= 0 && SkillID < MAX_ACTIONINFO && g_pSkillInfoTable != NULL)
		(*g_pSkillInfoTable)[SkillID].SetSkillExp( SkillExp );
}

void 
ModifyStatusManager::Function_MODIFY_PET_HP(void *pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
	TYPE_OBJECTID id = g_pPlayer->GetID();
	if(id == OBJECTID_NULL)
		return;
	MFakeCreature *pPet = (MFakeCreature *)g_pZone->GetFakeCreature(g_pPlayer->GetPetID());
	if(pPet == NULL)
		return;
	MPetItem *pPetItem = pPet->GetPetItem();
	if(pPetItem == NULL)
		return;
	if(pPetItem != NULL)
	{
		pPetItem->SetCurrentDurability(value);

		// 펫 죽기
		if(value == 60)
		{
			std::string petName = pPetItem->GetPetName().c_str();
			char szTemp[512];
			sprintf(szTemp, (*g_pGameStringTable)[UI_STRING_MESSAGE_HOUR].GetString(), 1);

			g_pSystemMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_PET_DIE_WARNING].GetString(), petName.c_str(), szTemp);
			g_pSystemMessage->Add((*g_pGameStringTable)[STRING_MESSAGE_PET_REQUEST_REFILL].GetString());
		}
		else if(value == 10)
		{
			std::string petName = pPetItem->GetPetName().c_str();
			char szTemp[512];
			sprintf(szTemp, (*g_pGameStringTable)[UI_STRING_MESSAGE_MINUTE].GetString(), 10);

			g_pSystemMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_PET_DIE_WARNING].GetString(), petName.c_str(), szTemp);
			g_pSystemMessage->Add((*g_pGameStringTable)[STRING_MESSAGE_PET_REQUEST_REFILL].GetString());
		}
		else if(value == 0)
		{
			TYPE_OBJECTID petID = g_pPlayer->GetPetID();
			if(petID != OBJECTID_NULL)
			{
				MCreature *pCreature = g_pZone->GetFakeCreature(petID);

				pCreature->SetDead();
			}
		}
	}
}

void 
ModifyStatusManager::Function_MODIFY_PET_EXP_REMAIN(void *pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
	TYPE_OBJECTID id = g_pPlayer->GetID();
	if(id == OBJECTID_NULL)
		return;
	MFakeCreature *pPet = (MFakeCreature *)g_pZone->GetFakeCreature(g_pPlayer->GetPetID());
	if(pPet == NULL)
		return;
	MPetItem *pPetItem = pPet->GetPetItem();
	if(pPetItem == NULL)
		return;
	if(pPetItem != NULL)
	{
		int accum = 0;
		accum = g_pExperienceTable->GetPetExp(pPetItem->GetNumber()).AccumExp;
		pPetItem->SetPetExpRemain(accum-value);
	}
}


void 
ModifyStatusManager::Function_MODIFY_UNIONID(void *pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )

	if(g_pPlayer == NULL)
		return;
	g_pPlayer->SetUnionGuildID(value);
	g_pUserInformation->dwUnionID = value;

}

void 
ModifyStatusManager::Function_MODIFY_UNIONGRADE(void *pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
	if(g_pPlayer == NULL)
		return;

	g_pUserInformation->bUnionGrade = value;

}


void 
ModifyStatusManager::Function_MODIFY_ADVANCEMENT_CLASS_LEVEL(void *pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
	if(g_pPlayer == NULL)
		return;
	

	g_char_slot_ingame.m_AdvancementLevel = value;
}

void 
ModifyStatusManager::Function_MODIFY_ADVANCEMENT_CLASS_GOAL_EXP(void *pVoid)
{
	EXTRACT_MODIFY_VALUE( pVoid )
	if(g_pPlayer == NULL)
		return;

	g_char_slot_ingame.EXP_REMAIN = value;


}
