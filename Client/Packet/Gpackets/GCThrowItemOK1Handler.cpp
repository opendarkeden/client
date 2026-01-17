//////////////////////////////////////////////////////////////////////
//
// Filename    : GCThrowItemOK1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCThrowItemOK1.h"
#include "ClientDef.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCThrowItemOK1Handler::execute ( GCThrowItemOK1 * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


	TYPE_ACTIONINFO skillType = MAGIC_THROW_HOLY_WATER;//g_pPlayer->GetWaitVerifyActionInfo();

	//------------------------------------------------------------------
	// Player가 기다리던 skill의 성공유무를 검증받았다.
	//------------------------------------------------------------------
	if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_SKILL_SUCCESS)
	{		
		g_pPlayer->SetWaitVerifyNULL();
	}
	else
	{
		DEBUG_ADD("[Error] Player is not WaitVerifySkillSuccess");
	}

	//------------------------------------------------------------------
	// Player가 Skill을 성공시킨 경우에 날아오는 Packet이므로
	// 결과를 반영시켜야 한다.
	//------------------------------------------------------------------
	// 상태값을 바꾼다.
	//------------------------------------------------------------------
	/*
	int size = pPacket->getListNum();

	for (int i=0; i<size; i++)
	{
		int part = pPacket->popFrontListElement();
		int value = pPacket->popFrontListElement();
		
		//------------------------------------------------------------------
		// Player에게 상태를 적용시킨다.
		//------------------------------------------------------------------
		g_pPlayer->SetStatus( part, value );		
	}
	*/

	//------------------------------------------------------------------
	// UI에 보이는 것을 바꿔준다.
	// 비교연산하는거보다 이게 더 빠르지 않을까.. 음.. - -;
	//------------------------------------------------------------------
	//UI_SetHP( g_pPlayer->GetHP(), g_pPlayer->GetMAX_HP() );
	//UI_SetMP( g_pPlayer->GetMP(), g_pPlayer->GetMAX_MP() );

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

		// [ TEST CODE ]
		// 진행중인 Effect이면 Result로 추가해야 한다.
		// Result를 생성하고..
		// g_pPlayer->>AddActionResult( ... )

		// Creature에게 Damage 입힘
		if (pCreature != NULL)
		{			
			MActionResult* pResult = new MActionResult;

			// 맞은 creature가 때린 player의 방향을 바라보기
			//pCreature->SetDirectionToPosition(g_pPlayer->GetX(), g_pPlayer->GetY());

			//int delayFrame = ConvertDurationToFrame( pPacket->getDuration() );

			// 결과 표현
			//pCreature->PacketSpecialActionResult( pPacket->getSkillType() + g_ActionInfoTable.GetMinResultActionInfo());
			pResult->Add( new MActionResultNodeActionInfo( 
											skillType,
											g_pPlayer->GetID(), 
											pCreature->GetID(), 
											pCreature->GetX(),
											pCreature->GetY())
											//delayFrame ) 
							);	

			
			// [ TEST CODE ] 
			// 맞는 결과 damage를 표현해 준다.			
			/*
			MActionResult* pResult = new MActionResult;
			pResult->Add( new MActionResultNodeActionInfo( g_pPlayer->GetID(), g_pPlayer->GetID(), SKILL_ATTACK_MELEE ) );			

			g_pPlayer->PacketAddActionResult(pPacket->getCEffectID(), pResult);
			*/

			// Player의 현재 진행 중인 기술의 결과로 추가시킨다.
			//g_pPlayer->PacketAddActionResult( pPacket->getCEffectID(), pResult );		
			g_pPlayer->PacketAddActionResult( 0, pResult );		
		}
		
	}
		

#endif

	__END_CATCH
}
