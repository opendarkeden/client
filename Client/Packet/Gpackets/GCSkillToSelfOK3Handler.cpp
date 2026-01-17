//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSkillToSelfOK3Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSkillToSelfOK3.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSkillToSelfOK3Handler::execute ( GCSkillToSelfOK3 * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
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
		//MCreature* pCreature = g_pZone->GetCreature( pPacket->getObjectID() );


		//------------------------------------------------------
		//
		// 어느 지점에서 기술 사용이 된다.
		//
		//------------------------------------------------------			

		int skillID = pPacket->getSkillType();
		
		if( g_pActionInfoTable->GetSize() <= skillID )
		{
			DEBUG_ADD_FORMAT("[Error] Exceed SkillType %d",skillID);
			SendBugReport("[Error:GCSTSOK3H] Exceed SkillType %d", skillID );
			return;
		}


		if( (*g_pActionInfoTable)[skillID].IsUseActionStep() && pPacket->getGrade() > 0)
			skillID = (*g_pActionInfoTable)[skillID].GetActionStep( pPacket->getGrade() - 1);
		
		DWORD delayFrame = ConvertDurationToFrame( pPacket->getDuration() );

		/*
		
		MActionResult* pResult = NULL;

		//------------------------------------------------------
		// EffectStatus가 있다면 붙인다.
		//------------------------------------------------------
		EFFECTSTATUS es = (*g_pActionInfoTable)[skillID].GetEffectStatus();
		
		if (es!=EFFECTSTATUS_NULL)
		{
			pResult = new MActionResult;
			pResult->Add( new MActionResultNodeAddEffectStatus( g_Player->GetID(), 
																	es, 
																	delayFrame ) );
		}
	
		
		pResult->Add( new MActionResultNodeActionInfo( 
								skillID, 
								OBJECTID_NULL,//pCreature->GetID(), 
								OBJECTID_NULL,//pCreature->GetID(), 
								pPacket->getX(),//pCreature->GetX(),
								pPacket->getY(),//pCreature->GetY(),
								delayFrame ) );
		*/

		//------------------------------------------------------
		//
		// skill에 결과가 있으면 같이 적용 시킨다.
		//
		//------------------------------------------------------
		/*
		MActionResultNode* pActionResultNode = NULL;
		switch ((*g_pActionInfoTable)[skillID].GetActionResultID())
		{
			//------------------------------------------------------
			// Burrow
			//------------------------------------------------------
			case ACTIONRESULTNODE_CREATURE_BURROW :
				pActionResultNode = new MActionResultNodeCreatureBurrow(pCreature->GetID());
			break;
		}
		*/

		//------------------------------------------------------
		// NULL이 아니면 같이 적용
		//------------------------------------------------------
		/*
		if (pActionResultNode!=NULL)
		{
			pResult->Add( pActionResultNode );
		}
		*/

		//Duration_t	m_Duration;
		//pCreature->PacketSpecialActionToSelf( 
		//						pPacket->getSkillType(),
		//						pResult	// 결과
		//	);

		//------------------------------------------------------
		// 사용 위치만 있는 경우...
		//------------------------------------------------------
		ExecuteActionInfoFromMainNode(
			skillID + (*g_pActionInfoTable).GetMinResultActionInfo(),
		
			pPacket->getX(), pPacket->getY(), 0, // 기술 사용하는 사람(?) 
			0,														// 사용 방향
			
			OBJECTID_NULL,												// 목표에 대한 정보
			pPacket->getX(), pPacket->getY(), 0, 
			
			delayFrame,													// 기술의 (남은) 지속 시간		
			
			NULL,

			false);			// 기술 첨부터 시작한다.
	}

#endif

	__END_CATCH
}
