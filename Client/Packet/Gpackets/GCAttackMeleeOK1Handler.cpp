//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAttackMeleeOK1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAttackMeleeOK1.h"
#include "ClientDef.h"
#include "MActionInfoTable.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAttackMeleeOK1Handler::execute ( GCAttackMeleeOK1 * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__
	
	// message

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
	// 상태값을 바꾼다.
	//------------------------------------------------------------------
	AffectModifyInfo(g_pPlayer, pPacket);

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
			// SKILL_ATTACK_MELEE에 대한 결과를 표현해준다.
			/*
			// 바로 맞는 모습
			pCreature->PacketSpecialActionResult( 
								g_pPlayer->GetBasicActionInfo() + g_ActionInfoTable.GetMinResultActionInfo(),
								pCreature->GetID(),
								pCreature->GetX(),
								pCreature->GetY()
								);
				*/
			// action이 끝나고 맞는 동작
			if (pCreature!=NULL)
			{
				MActionResult* pResult = new MActionResult;
			
				pResult->Add( new MActionResultNodeActionInfo( 
											g_pPlayer->GetBasicActionInfo(),
											g_pPlayer->GetID(), 
											pCreature->GetID(), 
											pCreature->GetX(),
											pCreature->GetY()
											 ) 
							);

				//g_pPlayer->PacketAddActionResult( effectID, pResult );
				g_pPlayer->PacketAddActionResult( 0, pResult );
			}
		}
	}	
		

#endif

	__END_CATCH
}
