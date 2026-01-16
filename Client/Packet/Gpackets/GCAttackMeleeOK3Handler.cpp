//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAttackMeleeOK3Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAttackMeleeOK3.h"
#include "ClientDef.h"
#include "MActionInfoTable.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAttackMeleeOK3Handler::execute ( GCAttackMeleeOK3 * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
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
		
		DEBUG_ADD_FORMAT("There's no such creature : ID=%d, Skill=%d", pPacket->getObjectID(), SKILL_ATTACK_MELEE);

		pTargetCreature->PacketSpecialActionResult( 
				RESULT_SKILL_ATTACK_MELEE,				//pPacket->getSkillType()+g_ActionInfoTable.GetMinResultActionInfo()
				pTargetCreature->GetID(),
				pTargetCreature->GetX(),
				pTargetCreature->GetY()
		);
		
		return;
	}
	//------------------------------------------------------
	// 때리는 사람만 있는 경우는 때리는 동작만 보여준다.
	//------------------------------------------------------
	else if (pTargetCreature==NULL)
	{
		
		DEBUG_ADD_FORMAT("There's no such creature : TargetID=%d, Skill=%d", pPacket->getTargetObjectID(), SKILL_ATTACK_MELEE);

		//------------------------------------------------------
		// Creature가 행동을 취하도록 한다.
		//------------------------------------------------------
		pCreature->PacketSpecialActionToNobody(
						pCreature->GetBasicActionInfo(), 
						pCreature->GetX(),
						pCreature->GetY()						
		);

		// PacketSpecialActionToNobody에다가 MActionResult를 추가해서
		// 피가 튀는걸 보여주는 것도 괜찮을 것이당..
		// 근데, targetCreature가 없기 때문에..
		// 정확한 좌표를 서버에서 받아오는게 먼저일까... 으흠~~		
		
		return;
	}

	//------------------------------------------------------
	// 결과(다른 캐릭터가 맞는 모습)를 설정한다.
	//------------------------------------------------------
	MActionResult* pResult = new MActionResult;
	pResult->Add( new MActionResultNodeActionInfo( 
								pCreature->GetBasicActionInfo(),
								pPacket->getObjectID(), 
								pPacket->getTargetObjectID(), 
								pTargetCreature->GetX(),
								pTargetCreature->GetY()
								 ) 
				);

	//------------------------------------------------------
	// 행동하는 Creature가 TargetCreature를 바라보도록 한다.
	//------------------------------------------------------
	pCreature->SetDirectionToPosition( pTargetCreature->GetX(), pTargetCreature->GetY() );

	//------------------------------------------------------
	// Creature가 행동을 취하도록 한다.
	//------------------------------------------------------
	pCreature->PacketSpecialActionToOther(
					pCreature->GetBasicActionInfo(), 
					pPacket->getTargetObjectID(), 
					pResult
	);

#endif

	__END_CATCH
}
