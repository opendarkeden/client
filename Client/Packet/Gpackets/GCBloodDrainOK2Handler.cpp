//////////////////////////////////////////////////////////////////////
//
// Filename    : GCBloodDrainOK2Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
// include files
#include "GCBloodDrainOK2.h"
#include "ClientDef.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCBloodDrainOK2Handler::execute ( GCBloodDrainOK2 * pPacket , Player * pPlayer )
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

		// 내가 Creature에게 흡혈당하구 있다구 알려준다
		g_pPlayer->SetDrainCreatureID( pPacket->getObjectID() );
		
		// Creature에게 Damage 입힘
		if (pCreature == NULL)
		{
			// 바로 당하는 모습
			g_pPlayer->PacketSpecialActionResult( 
										RESULT_SKILL_BLOOD_DRAIN,
										g_pPlayer->GetID(),	
										g_pPlayer->GetX(),
										g_pPlayer->GetY()
			 );
		}
		else
		{
			pCreature->ClearStopBloodDrain();

			// 내(Player)가 누군가가 사용한 SKill을 맞은 경우..
			// [ TEST CODE ]
			MActionResult* pResult = new MActionResult;
			pResult->Add( new MActionResultNodeActionInfo( 
										SKILL_BLOOD_DRAIN,
										pCreature->GetID(), 
										g_pPlayer->GetID(), 
										g_pPlayer->GetX(),
										g_pPlayer->GetY()
										 ) );		

			// [ TEST CODE ]
			//
			// 결과를 생성&저장해서 보내야 한다.
			//
			// 방향을 바라보기
			pCreature->SetDirectionToPosition(g_pPlayer->GetX(), g_pPlayer->GetY());

			//Duration_t	m_Duration;
			pCreature->PacketSpecialActionToOther(
								SKILL_BLOOD_DRAIN,
								g_pPlayer->GetID(),
								pResult						// 결과
			);
		}
	}	
	
	//------------------------------------------------------
	// Player가 기술을 당했을 때의 모습..
	//------------------------------------------------------
	/*
	int resultActionInfo =  SKILL_BLOOD_DRAIN + g_ActionInfoTable.GetMinResultActionInfo();
	g_pPlayer->PacketSpecialActionResult( 
						resultActionInfo,
						g_pPlayer->GetID(),
						g_pPlayer->GetX(),
						g_pPlayer->GetY()
				);
	*/
	//------------------------------------------------------------
	// Delay Frame 설정
	//------------------------------------------------------------
	//g_pPlayer->SetEffectDelayFrame( resultActionInfo, delayFrame );

	//------------------------------------------------------------------
	// 상태값을 바꾼다.
	//------------------------------------------------------------------
	AffectModifyInfo(g_pPlayer, pPacket);
	
	//------------------------------------------------------------------
	// 물린 상태..
	//------------------------------------------------------------------
	if (g_pPlayer->GetEFFECT_STAT()!=EFFECTSTATUS_NULL)
	{
		int delayFrame = ConvertDurationToFrame( g_pPlayer->GetDURATION() );

		g_pPlayer->AddEffectStatus((EFFECTSTATUS)g_pPlayer->GetEFFECT_STAT(), delayFrame);	
		
		g_pPlayer->SetStatus( MODIFY_EFFECT_STAT, EFFECTSTATUS_NULL );
	}

	//------------------------------------------------------------------
	// UI에 보이는 것을 바꿔준다.
	// 비교연산하는거보다 이게 더 빠르지 않을까.. 음.. - -;
	//------------------------------------------------------------------
	//UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );
	//UI_SetMP( g_pPlayer->GetMP(), g_pPlayer->GetMAX_MP() );
	
#endif

	__END_CATCH
}
