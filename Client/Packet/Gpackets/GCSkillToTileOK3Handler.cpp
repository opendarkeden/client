//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSkillToTileOK3Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSkillToTileOK3.h"
#include "ClientDef.h"
#include "SkillDef.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSkillToTileOK3Handler::execute ( GCSkillToTileOK3 * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

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
		MCreature* pCreature = g_pZone->GetCreature( pPacket->getObjectID() );


		// Creature가 Tile에 뭔가를?...
		if (pCreature != NULL)
		{	
			int skillID = pPacket->getSkillType();
			
			if( g_pActionInfoTable->GetSize() <= skillID )
			{
				SendBugReport("[GCSTTOK3H] exceed skilltype %d",skillID);
				return;
			}
			// 2004, 9, 9, sobeit add start
//			if(skillID == SKILL_BURNING_SOUL_LAUNCH)
//			{
//				skillID = CLIENT_BURNING_SOL_1+pPacket->getGrade();
//				ExecuteActionInfoFromMainNode(skillID,pPacket->getX(), pPacket->getY(), 0,0,0	,	
//					pPacket->getX(), pPacket->getY(), 0, 0, NULL, false);
//				return;
//			}
			// 2004, 9, 9, sobeit add end

			if( (*g_pActionInfoTable)[skillID].IsUseActionStep() && pPacket->getGrade() > 0 )
				skillID = (*g_pActionInfoTable)[skillID].GetActionStep( pPacket->getGrade() - 1) == ACTIONINFO_NULL ? 
									skillID : (*g_pActionInfoTable)[skillID].GetActionStep( pPacket->getGrade() - 1);

			//------------------------------------------------------
			// Creature가 행동하는 모습
			//------------------------------------------------------
			//Duration_t	m_Duration;
			pCreature->PacketSpecialActionToSector(
								skillID, 
								pPacket->getX(), 
								pPacket->getY(),
								NULL		// 결과
			);		

			// 방향 보기
			pCreature->SetDirectionToPosition(pPacket->getX(), pPacket->getY());		
		}
		else
		{
			DEBUG_ADD_FORMAT("[Error] There is no such Creature id=%d", pPacket->getObjectID());
		}
	}

#endif

	__END_CATCH
}
