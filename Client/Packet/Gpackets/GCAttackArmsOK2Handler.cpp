//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAttackArmsOK2Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAttackArmsOK2.h"
#include "ClientDef.h"
#include "MActionInfoTable.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAttackArmsOK2Handler::execute ( GCAttackArmsOK2 * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	// message

	//------------------------------------------------------
	// Zone이 아직 생성되지 않은 경우
	//------------------------------------------------------
	if (g_pZone==NULL)
	{
		// message
		DEBUG_ADD("[Error] Zone is Not Init.. yet.");			
		
		return;
	}	

	//------------------------------------------------------
	// 대상이 되는 creature를 얻는다.
	//------------------------------------------------------
	MCreature* pCreature = g_pZone->GetCreature( pPacket->getObjectID() );
	
	if (pCreature==NULL)
	{
		// 그런 creature가 없을 경우
		DEBUG_ADD_FORMAT("There's no such creature : ID=%d, Skill=%d", pPacket->getObjectID(), SKILL_ATTACK_MELEE);				
		
		if( pPacket->getSkillType() == SKILL_JABBING_VEIN || pPacket->getSkillType() == SKILL_MOLE_SHOT ||
					pPacket->getSkillType() == SKILL_TRIDENT || pPacket->getSkillType() == SKILL_QUICK_FIRE ||
					pPacket->getSkillType() == SKILL_ULTIMATE_BLOW || pPacket->getSkillType() == SKILL_HARPOON_BOMB)
		{
			TYPE_ACTIONINFO resultActionInfo = pPacket->getSkillType() + g_pActionInfoTable->GetMinResultActionInfo();
			// 바로 맞는 모습을 보이게 한다.
			g_pPlayer->PacketSpecialActionResult( 
									resultActionInfo,	// 기본 공격 == 총!? 
									g_pPlayer->GetID(), 
									g_pPlayer->GetX(),
									g_pPlayer->GetY()								
					);
		} else
		{
			// 바로 맞는 모습을 보이게 한다.
			g_pPlayer->PacketSpecialActionResult( 
									RESULT_SKILL_ATTACK_GUN_AR,	// 기본 공격 == 총!? 
									g_pPlayer->GetID(), 
									g_pPlayer->GetX(),
									g_pPlayer->GetY()								
					);
		}
	}
	else
	{
		TYPE_ACTIONINFO actionInfo = pPacket->getSkillType();
		//------------------------------------------------------
		// 행동하는 Creature가 player를 바라보도록 한다.
		//------------------------------------------------------
		pCreature->SetDirectionToPosition( g_pPlayer->GetX(), g_pPlayer->GetY() );

		//------------------------------------------------------
		// Creature가 Player를 공격하는 모습
		//------------------------------------------------------
		//g_pPlayer->PacketSpecialActionResult( SKILL_ATTACK_MELEE + g_ActionInfoTable.GetMinResultActionInfo() );

		if( actionInfo == SKILL_JABBING_VEIN || actionInfo == SKILL_TRIDENT || actionInfo == SKILL_MOLE_SHOT ||
			actionInfo == SKILL_QUICK_FIRE || actionInfo == SKILL_ULTIMATE_BLOW || pPacket->getSkillType() == SKILL_HARPOON_BOMB)
		{
			MActionResult* pResult = new MActionResult;
			pResult->Add( new MActionResultNodeActionInfo( 
										actionInfo,	// 기본 공격 == 총!? 
										pPacket->getObjectID(), 
										g_pPlayer->GetID(), 
										g_pPlayer->GetX(),
										g_pPlayer->GetY()								
										) 
						);
			//------------------------------------------------------
			// Creature가 행동을 취하도록 한다.
			//------------------------------------------------------
			pCreature->PacketSpecialActionToOther(
							actionInfo,	// 기본 공격 == 총!? 
							g_pPlayer->GetID(), 
							pResult
			);
		} else
		{
			MActionResult* pResult = new MActionResult;
			pResult->Add( new MActionResultNodeActionInfo( 
										pCreature->GetBasicActionInfo(),	// 기본 공격 == 총!? 
										pPacket->getObjectID(), 
										g_pPlayer->GetID(), 
										g_pPlayer->GetX(),
										g_pPlayer->GetY()								
										) 
						);
			//------------------------------------------------------
			// Creature가 행동을 취하도록 한다.
			//------------------------------------------------------
			pCreature->PacketSpecialActionToOther(
							pCreature->GetBasicActionInfo(),	// 기본 공격 == 총!? 
							g_pPlayer->GetID(), 
							pResult
			);
		}
	}

	//------------------------------------------------------------------
	// 상태값을 바꾼다.
	//------------------------------------------------------------------
	AffectModifyInfo(g_pPlayer, pPacket);


	//------------------------------------------------------------------
	// UI에 보이는 것을 바꿔준다.
	// 비교연산하는거보다 이게 더 빠르지 않을까.. 음.. - -;
	//------------------------------------------------------------------
	//UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );
	//UI_SetMP( g_pPlayer->GetMP(), g_pPlayer->GetMAX_MP() );
	

#endif

	__END_CATCH
}
