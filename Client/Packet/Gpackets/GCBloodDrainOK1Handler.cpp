//////////////////////////////////////////////////////////////////////
//
// Filename    : GCBloodDrainOK1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
// include files
#include "GCBloodDrainOK1.h"
#include "ClientDef.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCBloodDrainOK1Handler::execute ( GCBloodDrainOK1 * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


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
	AffectModifyInfo(g_pPlayer, pPacket);

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

		// Creature에게 Damage 입힘
		if (pCreature != NULL)
		{	
			// Creature에게 내가 흡혈하고 있다고 알려준다
			pCreature->SetDrainCreatureID( g_pPlayer->GetID() );

			// 결과 바로 표현
			pCreature->PacketSpecialActionResult( 
					SKILL_BLOOD_DRAIN + (*g_pActionInfoTable).GetMinResultActionInfo(),
					pCreature->GetID(),
					pCreature->GetX(),
					pCreature->GetY()
			);	

			int delayFrame = 691200;	// 12시간*60분*60초*16frame = 12*60*60*16;
			pCreature->AddEffectStatus( EFFECTSTATUS_BLOOD_DRAIN, delayFrame );	
			
//			// 2004, 6, 9 sobeit add start - 테스트, 흡혈 성공 메세지 온담에 동작하게..-_-
//			if(g_pPlayer->IsVampire())
//			{
//				g_pPlayer->SetDirectionToPosition(pCreature->GetX(), pCreature->GetY());
//				g_pPlayer->SetAction(ACTION_VAMPIRE_DRAIN);
//				
//			}
//			// 2004, 6, 9 sobeit add end
		}
	}
	
	g_pPlayer->ClearStopBloodDrain();

#endif

	__END_CATCH
}
