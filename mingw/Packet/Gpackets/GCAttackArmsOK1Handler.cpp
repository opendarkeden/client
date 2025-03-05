//////////////////////////////////////////////////////////////////////
//
// Filename    : GCAttackArmsOK1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAttackArmsOK1.h"
#include "ClientDef.h"
#include "MActionInfoTable.h"
#include "MSlayerGear.h"
#include "Skilldef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCAttackArmsOK1Handler::execute ( GCAttackArmsOK1 * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#if __GAME_CLIENT__

	
	// message

	//------------------------------------------------------------------
	// Player가 Skill을 성공시킨 경우에 날아오는 Packet이므로
	// 결과를 반영시켜야 한다.
	//------------------------------------------------------------------
	// 상태값을 바꾼다.
	//------------------------------------------------------------------
	AffectModifyInfo(g_pPlayer, pPacket);

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
		/*
		if (pCreature != NULL)
		{
			// 바로 맞는 동작
			// SKILL_ATTACK_GUN1~4?에 대한 결과를 표현해준다.
			pCreature->PacketSpecialActionResult( 
				g_pPlayer->GetBasicActionInfo() + g_ActionInfoTable.GetMinResultActionInfo(),
				g_pPlayer->GetID(),
				g_pPlayer->GetX(),
				g_pPlayer->GetY()
			);
			
		}
		*/

		//------------------------------------------------------
		// 맞은 경우만 맞은 동작은 보인다.
		//------------------------------------------------------
		if (pPacket->getSkillSuccess())
		{
			// action이 끝나고 맞는 동작
			if (pCreature!=NULL)
			{
				MActionResult* pResult = new MActionResult;

				if( pPacket->getSkillType() == SKILL_JABBING_VEIN || pPacket->getSkillType() == SKILL_MOLE_SHOT ||
					pPacket->getSkillType() == SKILL_TRIDENT || pPacket->getSkillType() == SKILL_QUICK_FIRE || 
					pPacket->getSkillType() == SKILL_ULTIMATE_BLOW  || pPacket->getSkillType() == SKILL_HARPOON_BOMB)
				{
					pResult->Add( new MActionResultNodeActionInfo( 
												pPacket->getSkillType(),
												g_pPlayer->GetID(), 
												pCreature->GetID(), 
												pCreature->GetX(),
												pCreature->GetY()
												 ) 
								);
				} else
				{			
					pResult->Add( new MActionResultNodeActionInfo( 
												g_pPlayer->GetBasicActionInfo(),
												g_pPlayer->GetID(), 
												pCreature->GetID(), 
												pCreature->GetX(),
												pCreature->GetY()
												 ) 
								);
				}

				//g_pPlayer->PacketAddActionResult( effectID, pResult );
				g_pPlayer->PacketAddActionResult( 0, pResult );
			}
		}
					
		//------------------------------------------------------
		// 총알 빼준다.
		//------------------------------------------------------
		if (g_pCurrentMagazine==NULL)
		{
			DEBUG_ADD("[Error] Current Magazine Not Exist!");			
		}
		else
		{
			g_pCurrentMagazine->SetNumber( pPacket->getBullet() );
		}

	}	
		

#endif

	__END_CATCH
}
