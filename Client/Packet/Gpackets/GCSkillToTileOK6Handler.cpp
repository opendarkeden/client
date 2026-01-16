//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSkillToTileOK6Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSkillToTileOK6.h"
#include "ClientDef.h"
#include "PacketFunction2.h"
#include "SkillDef.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSkillToTileOK6Handler::execute ( GCSkillToTileOK6 * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	//------------------------------------------------------------------
	// 상태값을 바꾼다.
	//------------------------------------------------------------------
	AffectModifyInfo(g_pPlayer, pPacket);

	int delayFrame = ConvertDurationToFrame( pPacket->getDuration() );

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
		//------------------------------------------------------
		// Player가 사용한 기술이라고 packet이 날아온 경우
		// --> Error다
		//------------------------------------------------------
		
		//------------------------------------------------------
		// 사용자가 없는 기술이다.
		//------------------------------------------------------
		int startX = pPacket->getOrgX();
		int startY = pPacket->getOrgY();
	
		int skillID = pPacket->getSkillType();

		if( g_pActionInfoTable->GetSize() <= skillID )
		{
			DEBUG_ADD_FORMAT("[Error] Exceed SkillType %d",skillID);
			SendBugReport("[Error:GCSTTOK6H] Exceed SkillType %d", skillID );
			return;
		}
		// 2004, 9, 9, sobeit add start
		if(skillID == SKILL_BURNING_SOUL_LAUNCH)
		{
			int TempskillID = RESULT_CLIENT_BURNING_SOL_1+pPacket->getGrade();
			ExecuteActionInfoFromMainNode(TempskillID,pPacket->getX(), pPacket->getY(), 0,0,0	,	
				pPacket->getX(), pPacket->getY(), 0, ConvertDurationToFrame( pPacket->getDuration() ), NULL, false);
		//	return;
		}
		// 2004, 9, 9, sobeit add end
		
		if( (*g_pActionInfoTable)[skillID].IsUseActionStep() &&pPacket->getGrade() > 0 )
			skillID = (*g_pActionInfoTable)[skillID].GetActionStep( pPacket->getGrade() - 1);

		//------------------------------------------------------
		// TileOK2로 인한 결과 
		//------------------------------------------------------				
		int size = pPacket->getCListNum();

		MActionResult* pResult = new MActionResult;

		//------------------------------------------------------------------
		// effect status를 적용시킨다.
		//------------------------------------------------------------------
		if (g_pPlayer->GetEFFECT_STAT()!=EFFECTSTATUS_NULL)
		{
			//int esDelayFrame = ConvertDurationToFrame( g_pPlayer->GetDURATION() );
			pResult->Add( new MActionResultNodeAddEffectStatus( 
										g_pPlayer->GetID(), 
										(EFFECTSTATUS)g_pPlayer->GetEFFECT_STAT(), 
										delayFrame ));

			g_pPlayer->SetStatus( MODIFY_EFFECT_STAT, EFFECTSTATUS_NULL );
		}

		/*
		
		pResult->Add( new MActionResultNodeActionInfo( 
											skillID,
											pCreature->GetID(), 
											pCreature->GetID(), 
											pPacket->getX(),
											pPacket->getY(),
											delayFrame ) 
							);	
		*/

		if (size!=0)
		{
			// 결과를 당하는 각각의 creature에 대해서 결과 표현
			for (int i=0; i<size; i++)
			{
				//MCreature* pTargetCreature = g_pZone->GetCreature( pPacket->getCListElement() );
					
				// Creature에게 Damage 입힘
				//if (pTargetCreature != NULL)
				//{
					// 내(Player)가 누군가가 사용한 SKill을 맞은 경우..
					// [ TEST CODE ]
					
				//	pResult->Add( new MActionResultNodeActionInfo( 
				//								pCreature->GetID(), 
				//								pPacket->popCListElement(), 
				//								pPacket->getSkillType(),
				//								delayFrame ) 
				//				);
				//}
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

					//------------------------------------------------------
					//
					// skill에 결과가 있으면 적용 시킨다.
					//
					//------------------------------------------------------
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
									OBJECTID_NULL,//pCreature->GetID(), 
									targetID, 
									pTargetCreature->GetX(),
									pTargetCreature->GetY(),
									delayFrame, pPacket->getRange() );
							} else
							{
								pActionResultNode =  new MActionResultNodeActionInfo( 
									(*g_pActionInfoTable)[skillID].GetActionResultValue(),
									OBJECTID_NULL,//pCreature->GetID(), 
									targetID, 
									pTargetCreature->GetX(),
									pTargetCreature->GetY(),
									delayFrame);
							}
						break;

						//------------------------------------------------------
						// Burrow등등.
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
		
		}

		//------------------------------------------------------
		// range를 direction에 적용시키는 경우
		//------------------------------------------------------
		int direction = pPacket->getRange();

		if (direction >= 8) direction = 0;

		//------------------------------------------------------
		// TileOK2로 인한 결과 추가
		//------------------------------------------------------
		//Duration_t	m_Duration;
		//pCreature->PacketSpecialActionToSector(
		//					pPacket->getSkillType(), 
		//					pPacket->getX(), pPacket->getY(),
		//					pResult						// 결과
		//);			
	

		//------------------------------------------------------
		// 어느 시점에서 바로 시작되게 한다.
		//------------------------------------------------------
		ExecuteActionInfoFromMainNode(
				skillID + (*g_pActionInfoTable).GetMinResultActionInfo(),

				startX, startY, 0,
		
				direction,														// 사용 방향
		
				OBJECTID_NULL,												// 목표에 대한 정보
		
				pPacket->getX(), pPacket->getY(), 0, 
		
				delayFrame,													// 기술의 (남은) 지속 시간		
		
				pResult,
				
				false);			// 기술 첨부터 시작한다.
		
	}


	//------------------------------------------------------------
	// Delay Frame 설정
	//------------------------------------------------------------
	//g_pPlayer->SetEffectDelayFrame( resultActionInfo, delayFrame );




	//------------------------------------------------------------------
	// UI에 보이는 것을 바꿔준다.
	// 비교연산하는거보다 이게 더 빠르지 않을까.. 음.. - -;
	//------------------------------------------------------------------
	//UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );
	//UI_SetMP( g_pPlayer->GetMP(), g_pPlayer->GetMAX_MP() );

#endif

	__END_CATCH
}
