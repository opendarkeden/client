//////////////////////////////////////////////////////////////////////
//
// Filename    : GCKnocksTargetBackOK1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCKnocksTargetBackOK1.h"
#include "ClientDef.h"
#include "MSlayerGear.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCKnocksTargetBackOK1Handler::execute ( GCKnocksTargetBackOK1 * pPacket , Player * pPlayer )
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
		// 맞은 경우만 맞은 동작은 보인다.
		//------------------------------------------------------
		if (pPacket->getSkillSuccess())
		{
			// Creature에게 Damage 입힘
			if (pCreature != NULL)
			{			
				unsigned short x = pPacket->getX();
				unsigned short y = pPacket->getY();

				// 물러나는방향(direction)적용..
				// 이동한 후의 좌표가 온다.
				//MCreature::GetPositionToDirection(x, y, pPacket->getDir());

				MActionResult* pResult = new MActionResult;

				pResult->Add( new MActionResultNodeChangePosition(
										pPacket->getObjectID(),
										x, y)
							);

			

				// Creature의 좌표를 바로 바꾼다.
				//pCreature->SetPosition( x, y );
				//pCreature->SetServerPosition( x, y );
				//pCreature->SetStop();

				// 결과 바로 표현
				//pCreature->PacketSpecialActionResult( 
				//					RESULT_SKILL_KNOCKS_TARGET_BACK//pPacket->getSkillType() + g_ActionInfoTable.GetMinResultActionInfo()
				//);			

				g_pPlayer->PacketAddActionResult( 0 , pResult);

				//------------------------------------------------------
				// 동기화 문제 때문에..
				// 서버 위치는 바로 지정한다.
				//------------------------------------------------------
				pCreature->SetServerPosition( x, y );
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

#endif

	__END_CATCH
}
