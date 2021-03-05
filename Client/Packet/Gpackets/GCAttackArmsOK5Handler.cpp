//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAttackArmsOK5Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAttackArmsOK5.h"
#include "ClientDef.h"
#include "MActionInfoTable.h"
#include "MSlayerGear.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAttackArmsOK5Handler::execute ( GCAttackArmsOK5 * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#if __GAME_CLIENT__

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
	MCreature* pTargetCreature = g_pZone->GetCreature( pPacket->getTargetObjectID() );
	
	if (pCreature==NULL && pTargetCreature==NULL)
	{
		return;
	}

	//------------------------------------------------------
	// 맞는 사람만 있는 경우는 맞는 동작만 보여주면 된다.
	//------------------------------------------------------
	if (pCreature==NULL)
	{
		
		DEBUG_ADD_FORMAT("There's no such creature : ID=%d, Skill=%d", pPacket->getObjectID(), pPacket->getSkillType() );

		if( pPacket->getSkillType() == SKILL_JABBING_VEIN || pPacket->getSkillType() == SKILL_MOLE_SHOT ||
					pPacket->getSkillType() == SKILL_TRIDENT || pPacket->getSkillType() == SKILL_QUICK_FIRE ||
					pPacket->getSkillType() == SKILL_ULTIMATE_BLOW || pPacket->getSkillType() == SKILL_HARPOON_BOMB)
		{
			pTargetCreature->PacketSpecialActionResult( 
					pPacket->getSkillType()+g_pActionInfoTable->GetMinResultActionInfo(),
					pTargetCreature->GetID(),
					pTargetCreature->GetX(),
					pTargetCreature->GetY()
			);
		} else
		{
			pTargetCreature->PacketSpecialActionResult( 
					RESULT_SKILL_ATTACK_GUN_AR,				//pPacket->getSkillType()+g_ActionInfoTable.GetMinResultActionInfo()
					pTargetCreature->GetID(),
					pTargetCreature->GetX(),
					pTargetCreature->GetY()
			);
		}
		
		return;
	}
	//------------------------------------------------------
	// 때리는 사람만 있는 경우는 때리는 동작만 보여준다.
	//------------------------------------------------------
	else if (pTargetCreature==NULL)
	{
		
		DEBUG_ADD_FORMAT("There's no such creature : TargetID=%d, Skill=%d", pPacket->getTargetObjectID(), SKILL_ATTACK_MELEE);

		if( pPacket->getSkillType() == SKILL_JABBING_VEIN || pPacket->getSkillType() == SKILL_MOLE_SHOT ||
					pPacket->getSkillType() == SKILL_TRIDENT || pPacket->getSkillType() == SKILL_ULTIMATE_BLOW)
		{
			pCreature->PacketSpecialActionToNobody(
							pPacket->getSkillType(), 
							pCreature->GetX(),
							pCreature->GetY()						
			);
		} else
		{
			//------------------------------------------------------
			// Creature가 행동을 취하도록 한다.
			//------------------------------------------------------
			pCreature->PacketSpecialActionToNobody(
							pCreature->GetBasicActionInfo(), 
							pCreature->GetX(),
							pCreature->GetY()						
			);
		}
		
		return;
	}


	//------------------------------------------------------
	// 결과(다른 캐릭터가 맞는 모습)를 설정한다.
	//------------------------------------------------------
	MActionResult* pResult = NULL;

	TYPE_ACTIONINFO actionInfo = pPacket->getSkillType();
	
	if (pPacket->getSkillSuccess())
	{
		pResult = new MActionResult;

		if( actionInfo == SKILL_JABBING_VEIN || actionInfo == SKILL_MOLE_SHOT || actionInfo == SKILL_TRIDENT ||
			actionInfo == SKILL_QUICK_FIRE  || actionInfo == SKILL_ULTIMATE_BLOW || pPacket->getSkillType() == SKILL_HARPOON_BOMB)
		{
			pResult->Add( new MActionResultNodeActionInfo( 
										actionInfo,
										pPacket->getObjectID(), 
										pPacket->getTargetObjectID(), 
										pTargetCreature->GetX(),
										pTargetCreature->GetY()								
								 ) 
						);
		} else
		{
			pResult->Add( new MActionResultNodeActionInfo( 
										pCreature->GetBasicActionInfo(),	// 총 공격
										pPacket->getObjectID(), 
										pPacket->getTargetObjectID(), 
										pTargetCreature->GetX(),
										pTargetCreature->GetY()								
								 ) 
						);
		}
	}

	//------------------------------------------------------
	// 행동하는 Creature가 TargetCreature를 바라보도록 한다.
	//------------------------------------------------------
	pCreature->SetDirectionToPosition( pTargetCreature->GetX(), pTargetCreature->GetY() );

	//------------------------------------------------------
	// Creature가 행동을 취하도록 한다.
	//------------------------------------------------------

	if( actionInfo == SKILL_JABBING_VEIN || actionInfo == SKILL_MOLE_SHOT || actionInfo == SKILL_TRIDENT ||
		actionInfo == SKILL_QUICK_FIRE || actionInfo == SKILL_ULTIMATE_BLOW || pPacket->getSkillType() == SKILL_HARPOON_BOMB)
	{
		pCreature->PacketSpecialActionToOther(
						// 총 공격
						actionInfo,
						pPacket->getTargetObjectID(), 
						pResult
		);
	} else
	{
		pCreature->PacketSpecialActionToOther(
						// 총 공격
						pCreature->GetBasicActionInfo()	, 
						pPacket->getTargetObjectID(), 
						pResult
		);
	}


#endif

	__END_CATCH
}
