//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSkillToTileOK4Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSkillToTileOK4.h"
#include "ClientDef.h"
#include "PacketFunction2.h"
#include "SkillDef.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSkillToTileOK4Handler::execute ( GCSkillToTileOK4 * pPacket , Player * pPlayer )
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
		int skillID = pPacket->getSkillType();
		if( g_pActionInfoTable->GetSize() <= skillID )
		{
			DEBUG_ADD_FORMAT("[Error] Exceed SkillType %d",skillID);
			SendBugReport("[Error:GCSTTOK4H] Exceed SkillType %d", skillID);
			return;
		}
//		// 2004, 9, 9, sobeit add start
//		if(skillID == SKILL_BURNING_SOUL_LAUNCH)
//		{
//			skillID = CLIENT_BURNING_SOL_1+pPacket->getGrade();
//			ExecuteActionInfoFromMainNode(skillID,pPacket->getX(), pPacket->getY(), 0,0,0	,	
//				pPacket->getX(), pPacket->getY(), 0, ConvertDurationToFrame( pPacket->getDuration() ), NULL, false);
//			return;
//		}
//		// 2004, 9, 9, sobeit add end
		if( (*g_pActionInfoTable)[skillID].IsUseActionStep() && pPacket->getGrade() > 0)
			skillID = (*g_pActionInfoTable)[skillID].GetActionStep( pPacket->getGrade() - 1);

			//------------------------------------------------------
		// 결과 생성
		//------------------------------------------------------
		MActionResult* pResult = new MActionResult;
	
		DWORD delayFrame = ConvertDurationToFrame( pPacket->getDuration() );

		int size = pPacket->getCListNum();

		for (int i=0; i<size; i++)
		{
			//pResult->Add( new MActionResultNodeActionInfo( 
			//					OBJECTID_NULL,		// 사용자 없어도 되는걸까?
			//					pPacket->popCListElement(), 
			//					skillType,
			//					delayFrame ) );
			//------------------------------------------------------
			//
			// skill에 결과가 있으면 적용 시킨다.
			//
			//------------------------------------------------------
			int targetID = pPacket->popCListElement();

			MCreature* pTargetCreature = g_pZone->GetCreature( targetID );

			if (pTargetCreature!=NULL)
			{
				//------------------------------------------------------
				// EffectStatus가 있다면 붙인다.
				//------------------------------------------------------
				EFFECTSTATUS es = (*g_pActionInfoTable)[skillID].GetEffectStatus();
							
				if (es!=EFFECTSTATUS_NULL)
				{
					pResult->Add( new MActionResultNodeAddEffectStatus( targetID, es, delayFrame ) );
				}

				MActionResultNode* pActionResultNode = NULL;

				switch ((*g_pActionInfoTable)[skillID].GetActionResultID())
				{
					//------------------------------------------------------
					// 다른 ActionInfo 실행
					//------------------------------------------------------
					case ACTIONRESULTNODE_ACTIONINFO :
						if( (*g_pActionInfoTable)[skillID].IsUseActionGrade() )
						{
							pActionResultNode =  new MActionResultNodeActionInfo( 
								(*g_pActionInfoTable)[skillID].GetActionResultValue(),
								OBJECTID_NULL, // 사용자 없다 - -;
								targetID, 
								pTargetCreature->GetX(),
								pTargetCreature->GetY(),
								delayFrame, pPacket->getRange());
						} else
						{
							pActionResultNode =  new MActionResultNodeActionInfo( 
								(*g_pActionInfoTable)[skillID].GetActionResultValue(),
								OBJECTID_NULL, // 사용자 없다 - -;
								targetID, 
								pTargetCreature->GetX(),
								pTargetCreature->GetY(),
								delayFrame);
						}
					break;

					//------------------------------------------------------
					// Burrow 등..
					//------------------------------------------------------
					default :
						pActionResultNode = CreateActionResultNode(pTargetCreature, skillID);					
				}

				//------------------------------------------------------
				// NULL이 아니면 같이 적용
				//------------------------------------------------------
				if (pActionResultNode!=NULL)
				{
					pResult->Add( pActionResultNode );
				}
			}
		}

		//------------------------------------------------------
		// Sector에 바로 적용
		//------------------------------------------------------
		int resultActionInfo = skillID + (*g_pActionInfoTable).GetMinResultActionInfo();


		int direction;

		//------------------------------------------------------
		// range를 direction에 적용시키는 경우
		//------------------------------------------------------
		if ((*g_pActionInfoTable)[skillID].IsOptionRangeToDirection())
		{
			direction = pPacket->getRange();
		}
		else
		{
			direction = DIRECTION_DOWN;
		}

		SkillToSector(resultActionInfo, 
						pPacket->getX(),
						pPacket->getY(),
						direction, 
						delayFrame,
						pResult);
	}

#endif

	__END_CATCH
}
