//////////////////////////////////////////////////////////////////////
//
// Filename    : GCKnocksTargetBackOK2Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCKnocksTargetBackOK2.h"
#include "ClientDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCKnocksTargetBackOK2Handler::execute ( GCKnocksTargetBackOK2 * pPacket , Player * pPlayer )
	 throw ( Error )
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
		
		return;
	}	


	int skillType = pPacket->getSkillType();	//pCreature->GetBasicActionInfo(),	// 기본 공격 == 총!? 	
	
	if( skillType >= g_pActionInfoTable->GetSize() )
	{
		DEBUG_ADD_FORMAT("[GCKnocksTargetBackOK2] SkillType Error %d",skillType );
		return;
	}
	
	//------------------------------------------------------
	// 물러나는방향(direction)적용..
	//------------------------------------------------------
	unsigned short x = pPacket->getX();
	unsigned short y = pPacket->getY();
		

	//------------------------------------------------------
	// 대상이 되는 creature를 얻는다.
	//------------------------------------------------------
	MCreature* pCreature = g_pZone->GetCreature( pPacket->getObjectID() );
	
	if (pCreature==NULL)
	{
		// 그런 creature가 없을 경우
		DEBUG_ADD_FORMAT("There's no such creature : ID=%d, Skill=%d", pPacket->getObjectID(), pPacket->getSkillType());
		
		// 바로 적용
		g_pPlayer->SetServerPosition( x, y );		
		g_pPlayer->MovePosition( x, y );
		g_pPlayer->SetStop();

		// 결과 바로 표현
		g_pPlayer->PacketSpecialActionResult( 
							pPacket->getSkillType() + (*g_pActionInfoTable).GetMinResultActionInfo(),
							OBJECTID_NULL,
							x,
							y
		);
	}
	else
	{	
		// 이동한 후의 좌표가 온다.
		//MCreature::GetPositionToDirection(x, y, pPacket->getDir());


		//------------------------------------------------------
		// 행동하는 Creature가 player를 바라보도록 한다.
		//------------------------------------------------------
		pCreature->SetDirectionToPosition( g_pPlayer->GetX(), g_pPlayer->GetY() );

		//------------------------------------------------------
		// 결과 설정
		//------------------------------------------------------
		// Creature가 Player를 공격하는 모습
		//g_pPlayer->PacketSpecialActionResult( SKILL_ATTACK_MELEE + (*g_pActionInfoTable).GetMinResultActionInfo() );

		/*
		MActionResult* pResult = new MActionResult;
		pResult->Add( new MActionResultNodeActionInfo( 
									skillType,		// 기본 공격 == 총!? 
									pPacket->getObjectID(), 
									g_pPlayer->GetID(), 
									g_pPlayer->GetX(),
									g_pPlayer->GetY()
						) 
					);

		// Player의 좌표가 바뀌는 결과
		pResult->Add( new MActionResultNodeChangePosition( g_pPlayer->GetID(), x, y) );
		*/

		//------------------------------------------------------
		// Creature가 행동을 취하도록 한다.
		//------------------------------------------------------
		pCreature->PacketSpecialActionToOther(
						skillType,
						g_pPlayer->GetID(), 
						NULL	//pResult
		);

		//------------------------------------------------------
		// 동기화 문제 때문에..
		// 결과를 바로 표현한다.
		//------------------------------------------------------
		// 바로 적용
		g_pPlayer->SetServerPosition( x, y );		
		g_pPlayer->MovePosition( x, y );
		g_pPlayer->SetStop();

		// 결과 바로 표현
		g_pPlayer->PacketSpecialActionResult( 
							pPacket->getSkillType() + (*g_pActionInfoTable).GetMinResultActionInfo(),
							pCreature->GetID(),
							pCreature->GetX(),
							pCreature->GetY()
		);
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
