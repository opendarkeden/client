//--------------------------------------------------------------------------------
//
// Filename    : GCUnburrowOKHandler.cpp
// Written By  : elca, Reiot
// Description :
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCUnburrowOK.h"
#include "ClientDef.h"
#include "SkillDef.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCUnburrowOKHandler::execute ( GCUnburrowOK * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	/*
	ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

	// 다음 좌표를 플레이어에 지정해 놓는다.
	pClientPlayer->setX( pPacket->getX() );
	pClientPlayer->setY( pPacket->getY() );
	pClientPlayer->setDir( pPacket->getDir() );

	cout << "Unburrow OK to (" << (int)pPacket->getX() << "," << (int)pPacket->getY() << ","
		<< Dir2String[pPacket->getDir()] << ")" << endl;
	*/


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

	//-----------------------------------------------------------------
	// 위치 설정
	//-----------------------------------------------------------------
	g_pPlayer->SetServerPosition( pPacket->getX(), pPacket->getY() );	
	g_pPlayer->MovePosition( pPacket->getX(), pPacket->getY() );
	g_pPlayer->SetDirection( pPacket->getDir() );

	//-----------------------------------------------------------------
	// 지상으로 나온다.
	//-----------------------------------------------------------------
	g_pPlayer->SetGroundCreature();

	//--------------------------------------------------
	// Burrow에서 빠져나오는 Effect == Burrowing하는 Effect
	//--------------------------------------------------		
	ExecuteActionInfoFromMainNode(
		RESULT_MAGIC_HIDE,										// 사용 기술 번호
	
		g_pPlayer->GetX(), g_pPlayer->GetY(), 0,
		g_pPlayer->GetDirection(),														// 사용 방향
		
		OBJECTID_NULL,												// 목표에 대한 정보
		g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 
		
		0,													// 기술의 (남은) 지속 시간		
		
		NULL,
		
		false);			// 기술 첨부터 시작한다.

#endif

	__END_CATCH
}
