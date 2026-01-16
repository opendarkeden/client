//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSkillFailed2Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSkillFailed2.h"
#include "ClientDef.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSkillFailed2Handler::execute ( GCSkillFailed2 * pPacket , Player * pPlayer )
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
	MCreature* pTargetCreature = g_pZone->GetCreature( pPacket->getTargetObjectID() );
	
	//------------------------------------------------------
	// Creature가 행동을 취하도록 한다.
	//------------------------------------------------------
	int skilltype = pPacket->getSkillType();
	
//	if(skilltype == SKILL_BLOOD_DRAIN)
//	{
//		DEBUG_ADD("흡혈 실패ㅋㅋ");
//	}
	if( skilltype >= g_pActionInfoTable->GetSize() )
		return;
	
	if(pTargetCreature == NULL && pCreature != NULL && pCreature->IsOusters() && skilltype == SKILL_ABSORB_SOUL)
	{
		MItem *pItem = g_pZone->GetItem( pPacket->getTargetObjectID() );
		if(pItem != NULL)
		{
			pTargetCreature = ((MCorpse *)pItem)->GetCreature();
		}
	}

	if (pCreature==NULL)
	{
		// 그런 creature가 없을 경우
		DEBUG_ADD_FORMAT("There's no such creature : ID=%d, Skill=%d", pPacket->getObjectID(), SKILL_ATTACK_MELEE);				
		
		return;
	}

	//------------------------------------------------------
	// 현재의 무기에 따라서 공격 기술을 결정하는 경우..
	//------------------------------------------------------
	if (skilltype==SKILL_ATTACK_MELEE
		|| skilltype==SKILL_ATTACK_ARMS)
	{
		skilltype = pCreature->GetBasicActionInfo();
	}

	//
	if (skilltype >= g_pActionInfoTable->GetMinResultActionInfo())
	{
		DEBUG_ADD_FORMAT("[Error] SkillType Error = %d", skilltype);
		return;
	}
	
	if( pPacket->getGrade() > 0 && (*g_pActionInfoTable)[skilltype].IsUseActionStep() )
		skilltype = (*g_pActionInfoTable)[skilltype].GetActionStep( pPacket->getGrade() - 1 );


	DEBUG_ADD_FORMAT("SkillType = %d", skilltype);

	int resultSkillID = g_pActionInfoTable->GetMinResultActionInfo() + skilltype;

	if (pCreature->IsOusters() && skilltype==SKILL_ABSORB_SOUL)
	{
		pCreature->SetStopAbsorbSoul();
	}
	
	// 2005, 1, 10, sobeit add start - 784 몬스터 수류탄 던지기 스킬 실패시 어떤 동작도 하지 않게..
	if(skilltype == SKILL_GRENADE_ATTACK) 
		return;
	// 2005, 1, 10, sobeit add end

	//------------------------------------------------------
	// Other
	//------------------------------------------------------
	if ((*g_pActionInfoTable)[skilltype].IsTargetOther())
	{		
		if (skilltype==SKILL_BLOOD_DRAIN || skilltype == SKILL_BITE_OF_DEATH )
		{
			pCreature->SetStopBloodDrain();
			//DEBUG_ADD("흡혈 테스트ㅋㅋ");
		}
		
		// 맞는 애가 없는 경우..  
		if (pTargetCreature==NULL)
		{
			// 그런 creature가 없을 경우
			DEBUG_ADD_FORMAT("There's no such creature : TargetID=%d, Skill=%d", pPacket->getTargetObjectID(), SKILL_ATTACK_MELEE);

			//------------------------------------------------------
			// Creature가 행동을 취하도록 한다.
			//------------------------------------------------------
			pCreature->PacketSpecialActionToNobody(
							skilltype,
							pCreature->GetX(),
							pCreature->GetY()						
			);			
			
			return;
		}
		
		// 자신에게 쓰는 경우? - -;
		if (pCreature==pTargetCreature)
		{
			pCreature->PacketSpecialActionToSelf(
						skilltype, 				
						NULL
			);

			return;
		}
		
		//------------------------------------------------------
		// 행동하는 Creature가 TargetCreature를 바라보도록 한다.
		//------------------------------------------------------
		pCreature->SetDirectionToPosition( pTargetCreature->GetX(), pTargetCreature->GetY() );

		pCreature->PacketSpecialActionToOther(
						skilltype, 
						pPacket->getTargetObjectID(), 
						NULL
		);		
		
	}
	//------------------------------------------------------
	// Self
	//------------------------------------------------------
	else if ((*g_pActionInfoTable)[skilltype].IsTargetSelf())
	{
		pCreature->PacketSpecialActionToSelf(
						skilltype, 				
						NULL
		);
	}
	//------------------------------------------------------
	// Zone
	//------------------------------------------------------
	else if ((*g_pActionInfoTable)[skilltype].IsTargetZone())
	{
		pCreature->PacketSpecialActionToSector(
						skilltype, 
						pCreature->GetX(),
						pCreature->GetY(),
						NULL
		);
	}

#endif

	__END_CATCH
}
