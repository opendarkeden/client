//--------------------------------------------------------------------------------
//
// Filename    : GCUntransformOKHandler.cpp
// Written By  : elca, Reiot
// Description :
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCUntransformOK.h"
#include "ClientDef.h"
#include "SkillDef.h"
#include "MVampireGear.h"
#include "UserInformation.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCUntransformOKHandler::execute ( GCUntransformOK * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	bool bWerWolf = g_pPlayer->GetCreatureType() == CREATURETYPE_WER_WOLF;


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

	int x = pPacket->getX();
	int y = pPacket->getY();
	int dir = pPacket->getDir();

	int creatureType = g_pPlayer->IsMale()?CREATURETYPE_VAMPIRE_MALE1:CREATURETYPE_VAMPIRE_FEMALE1;
	MItem *pItem = g_pVampireGear->GetItem(MVampireGear::GEAR_VAMPIRE_COAT);
//	_MinTrace("Adasd2:%d %d\n",creatureType,g_pPlayer->IsMale());

	if(pItem != NULL)
	{
		if(g_pPlayer->IsMale())
		{
			creatureType = pItem->GetAddonMaleFrameID();
		}
		else
		{
			creatureType = pItem->GetAddonFemaleFrameID();
		}
//		_MinTrace("Adasd:%d %d\n",creatureType,g_pPlayer->IsMale());
	}

	if (g_pPlayer->GetCompetence()==0)
	{
		creatureType = CREATURETYPE_VAMPIRE_OPERATOR;
	}
	
	MActionResult* pResult = new MActionResult;

	pResult->Add( new MActionResultNodeChangeCreatureType( g_pPlayer->GetID(), creatureType ) );

	//--------------------------------------------------
	// 뱀파로 돌아간다.
	//--------------------------------------------------								
	ExecuteActionInfoFromMainNode(
		RESULT_MAGIC_UN_TRANSFORM,										// 사용 기술 번호
	
		x, y, 0,
		dir,
		
		OBJECTID_NULL,												// 목표에 대한 정보
		x, y, 0, 
		
		0,													// 기술의 (남은) 지속 시간		
		
		pResult, //NULL,
		
		false);			// 기술 첨부터 시작한다.

	g_pPlayer->SetDelay( 1000 );


	g_pPlayer->SetServerPosition( x, y );
	g_pPlayer->MovePosition( x, y );
	g_pPlayer->SetDirection( dir );
	g_pPlayer->SetCurrentDirection( dir );
	
	if( bWerWolf )
	{
		g_pPlayer->SetBodyColor1( g_pUserInformation->SkinColor );
		if( pItem != NULL )
			g_pPlayer->SetBodyColor2( pItem->GetItemOptionColorSet() );
		else
			g_pPlayer->SetBodyColor2( 377 );
	}
	

#endif

	__END_CATCH
}
