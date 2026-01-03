//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSkillToObjectOK3Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSkillToObjectOK3.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSkillToObjectOK3Handler::execute ( GCSkillToObjectOK3 * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#if __GAME_CLIENT__

	
	//------------------------------------------------------
	// Zone이 아직 생성되지 않은 경우
	//------------------------------------------------------
	if (g_pZone==NULL)
	{
		// message
		DEBUG_ADD("[Error] Zone is Not Init.. yet.");			
	}
	//------------------------------------------------------
	// 정상.. 
	//------------------------------------------------------
	else
	{
		MCreature* pUserCreature = g_pZone->GetCreature( pPacket->getObjectID() );
		
		// Creature에게 Damage 입힘
		if (pUserCreature != NULL)
		{
			int skillID = pPacket->getSkillType();

			if( g_pActionInfoTable->GetSize() <= skillID )
			{
				DEBUG_ADD_FORMAT("[Error] Exceed SkillType %d",skillID);
				SendBugReport("[Error:GCSTOOK3H] Exceed SkillType %d", skillID );
				return;
			}


			// 현재 무기의 적용을 받는 기술이면..
			if ((*g_pActionInfoTable)[skillID].IsAffectCurrentWeaponAction())
			{
				skillID = pUserCreature->GetBasicActionInfo();

				if (skillID >= g_pActionInfoTable->GetMinResultActionInfo())
				{
					DEBUG_ADD_FORMAT("[Error] SkillType Error = %d", skillID);
					return;
				}
			}
			if( (*g_pActionInfoTable)[skillID].IsUseActionStep() && pPacket->getGrade() > 0)
				skillID = (*g_pActionInfoTable)[skillID].GetActionStep( pPacket->getGrade() - 1);
	
			// 서로 바라보기
			pUserCreature->SetDirectionToPosition(pPacket->getTargetX(), pPacket->getTargetY());
			
			// 그 방향으로 기술 사용하는 모습만 보여주기..
			pUserCreature->PacketSpecialActionToNobody(
								skillID, 
								pPacket->getTargetX(),
								pPacket->getTargetY()
			);		
		}
	}	

#endif

	__END_CATCH
}
