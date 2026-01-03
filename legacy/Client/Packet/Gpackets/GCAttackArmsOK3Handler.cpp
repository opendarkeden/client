//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAttackArmsOK3Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAttackArmsOK3.h"
#include "ClientDef.h"
#include "MActionInfoTable.h"
#include "MSlayerGear.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAttackArmsOK3Handler::execute ( GCAttackArmsOK3 * pPacket , Player * pPlayer )
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
	
	if (pCreature==NULL)
	{
		// 그런 creature가 없을 경우
		DEBUG_ADD_FORMAT("There's no such creature : ID=%d, Skill=%d", pPacket->getObjectID(), pPacket->getSkillType());				
		
		return;
	}

	//------------------------------------------------------
	// 행동하는 Creature가 TargetCreature를 바라보도록 한다.
	//------------------------------------------------------
	pCreature->SetDirectionToPosition( pPacket->getTargetX(), pPacket->getTargetY() );

	//------------------------------------------------------
	// Creature가 행동을 취하도록 한다.
	// 상대가 zone에 없는 경우이다.
	//------------------------------------------------------
	if( pPacket->getSkillType() == SKILL_JABBING_VEIN || pPacket->getSkillType() == SKILL_MOLE_SHOT ||
					pPacket->getSkillType() == SKILL_TRIDENT || pPacket->getSkillType() == SKILL_QUICK_FIRE ||
					pPacket->getSkillType() == SKILL_ULTIMATE_BLOW || pPacket->getSkillType() == SKILL_HARPOON_BOMB)
	{
		pCreature->PacketSpecialActionToNobody(
						pPacket->getSkillType()	,	// 총 공격
						pPacket->getTargetX(),
						pPacket->getTargetY()						
		);
	} else
	{
		pCreature->PacketSpecialActionToNobody(
						pCreature->GetBasicActionInfo()	,	// 총 공격
						pPacket->getTargetX(),
						pPacket->getTargetY()						
		);
	}



#endif

	__END_CATCH
}
