//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSkillToSelfOK2Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSkillToSelfOK2.h"
#include "ClientDef.h"
#include "PacketFunction2.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSkillToSelfOK2Handler::execute ( GCSkillToSelfOK2 * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		

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


		//------------------------------------------------------
		//
		// Creature가 기술 사용 
		//
		//------------------------------------------------------
		if (pCreature != NULL)
		{	
			// [ TEST CODE ]
			//
			// 결과를 생성&저장해서 보내야 한다.
			//
			// 누가(Player)가 스스로에게 SKill을 사용한 경우..
			// [ TEST CODE ]

			int skillID = pPacket->getSkillType();
			
			if( g_pActionInfoTable->GetSize() <= skillID )
			{
				DEBUG_ADD_FORMAT("[Error] Exceed SkillType %d",skillID);
				SendBugReport("[Error:GCSTSOK2H] Exceed SkillType %d", skillID );

				return;
			}			
			
			if( (*g_pActionInfoTable)[skillID].IsUseActionStep() && pPacket->getGrade() > 0)
				skillID = (*g_pActionInfoTable)[skillID].GetActionStep( pPacket->getGrade() - 1);
				
			MActionResult* pResult = new MActionResult;

			DWORD delayFrame = ConvertDurationToFrame( pPacket->getDuration() );

			pResult->Add( new MActionResultNodeActionInfo( 
									skillID, 
									pCreature->GetID(), 
									pCreature->GetID(), 
									pCreature->GetX(),
									pCreature->GetY(),
									delayFrame ) );

			//------------------------------------------------------
			// EffectStatus가 있다면 붙인다.
			//------------------------------------------------------
			EFFECTSTATUS es = (*g_pActionInfoTable)[skillID].GetEffectStatus();
			
			if (es!=EFFECTSTATUS_NULL)
			{
				pResult->Add( new MActionResultNodeAddEffectStatus( pCreature->GetID(), 
																		es, 
																		delayFrame ) );
			}

			//------------------------------------------------------
			//
			// skill에 결과가 있으면 같이 적용 시킨다.
			//
			//------------------------------------------------------
			MActionResultNode* pActionResultNode = CreateActionResultNode(pCreature, skillID);

			//------------------------------------------------------
			// NULL이 아니면 같이 적용
			//------------------------------------------------------
			if (pActionResultNode!=NULL)
			{
				pResult->Add( pActionResultNode );
			}

			//Duration_t	m_Duration;
			pCreature->PacketSpecialActionToSelf( 
									pPacket->getSkillType(),
									pResult	// 결과
				);
		}
	
	}


	__END_CATCH
}
