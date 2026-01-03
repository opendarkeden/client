//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSkillToObjectOK6Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSkillToObjectOK6.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSkillToObjectOK6Handler::execute ( GCSkillToObjectOK6 * pPacket , Player * pPlayer )
	 throw ( Error )
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
		int skillID = pPacket->getSkillType();

		if( g_pActionInfoTable->GetSize() <= skillID )
		{
			DEBUG_ADD_FORMAT("[Error] Exceed SkillType %d",skillID);
			SendBugReport("[ErrorGCSTOOK6H] Exceed SkillType %d", skillID );
			return;
		}


		if( (*g_pActionInfoTable)[skillID].IsUseActionStep() && pPacket->getGrade() > 0)
			skillID = (*g_pActionInfoTable)[skillID].GetActionStep( pPacket->getGrade() - 1);
	
		//------------------------------------------------------
		// 사용자가 있는 경우
		//------------------------------------------------------
		// 내(Player)가 누군가가 사용한 SKill을 맞은 경우..
		// [ TEST CODE ]
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
		else
		{
			//------------------------------------------------------
			// EffectStatus가 있다면 붙인다.
			//------------------------------------------------------
			EFFECTSTATUS es = (*g_pActionInfoTable)[skillID].GetEffectStatus();
			
			if (es!=EFFECTSTATUS_NULL)
			{

				pResult->Add( new MActionResultNodeAddEffectStatus( g_pPlayer->GetID(), 
																		es, 
																		delayFrame ) );
			}
		}

		pResult->Add( new MActionResultNodeActionInfo( 
								skillID, 
								OBJECTID_NULL,//pCreature->GetID(), 
								g_pPlayer->GetID(), 
								g_pPlayer->GetX(),
								g_pPlayer->GetY(),
								delayFrame ) );
		



		// [ TEST CODE ]
		//
		// 결과를 생성&저장해서 보내야 한다.
		//
		// 방향을 바라보기
		//pCreature->SetDirectionToPosition(g_pPlayer->GetX(), g_pPlayer->GetY());

		//Duration_t	m_Duration;
		//pCreature->PacketSpecialActionToOther(
		//					pPacket->getSkillType(), 
		//					pPacket->getObjectID(),
		//					pResult						// 결과
		//);
		int direction = GetDirectionToPosition(
							pPacket->getX(), pPacket->getY(),
							g_pPlayer->GetX(), g_pPlayer->GetY());

		ExecuteActionInfoFromMainNode(
					pPacket->getSkillType() + (*g_pActionInfoTable).GetMinResultActionInfo(),										// 사용 기술 번호
				
					pPacket->getX(), pPacket->getY(), 0, // 기술 사용하는 사람(?) 
					direction,														// 사용 방향
					
					g_pPlayer->GetID(),												// 목표에 대한 정보
					g_pPlayer->GetX(), g_pPlayer->GetY(), 0,
					
					delayFrame,													// 기술의 (남은) 지속 시간		
					
					pResult,										// 결과 정보

					false);			// 기술 첨부터 시작한다.
		
	}
		
	/*
	//------------------------------------------------------
	// Player가 기술을 당했을 때의 모습..
	//------------------------------------------------------
	int resultActionInfo =  pPacket->getSkillType() + (*g_pActionInfoTable).GetMinResultActionInfo();
	g_pPlayer->PacketSpecialActionResult( 
						resultActionInfo,
						g_pPlayer->GetID(),
						g_pPlayer->GetX(),
						g_pPlayer->GetY()
	);

	//------------------------------------------------------------
	// Delay Frame 설정
	//------------------------------------------------------------
	g_pPlayer->SetEffectDelayFrame( resultActionInfo, delayFrame );
	*/



	//------------------------------------------------------------------
	// UI에 보이는 것을 바꿔준다.
	// 비교연산하는거보다 이게 더 빠르지 않을까.. 음.. - -;
	//------------------------------------------------------------------
	//UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );
	//UI_SetMP( g_pPlayer->GetMP(), g_pPlayer->GetMAX_MP() );

#endif

	__END_CATCH
}
