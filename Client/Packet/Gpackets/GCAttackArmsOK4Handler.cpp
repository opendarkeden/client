//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAttackArmsOK4Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAttackArmsOK4.h"
#include "ClientDef.h"
#include "MActionInfoTable.h"
#include "MSlayerGear.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAttackArmsOK4Handler::execute ( GCAttackArmsOK4 * pPacket , Player * pPlayer )
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
	MCreature* pTargetCreature = g_pZone->GetCreature( pPacket->getTargetObjectID() );
	
	if (pTargetCreature==NULL)
	{
		// 그런 creature가 없을 경우
		DEBUG_ADD_FORMAT("There's no such creature : TargetID=%d, Skill=%d", pPacket->getTargetObjectID(), SKILL_ATTACK_MELEE);
		
		return;
	}

	//------------------------------------------------------
	// target creature가 결과를 당하는 모습을 바로 보여준다.
	//------------------------------------------------------
	
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
	}
	else
	{
		pTargetCreature->PacketSpecialActionResult( 
					RESULT_SKILL_ATTACK_GUN_SR,				//pPacket->getSkillType()+g_ActionInfoTable.GetMinResultActionInfo()
					pTargetCreature->GetID(),
					pTargetCreature->GetX(),
					pTargetCreature->GetY()
		);
	}



#endif

	__END_CATCH
}
