//--------------------------------------------------------------------------------
//
// Filename    : GCFakeMoveHandler.cpp
// Written By  : Reiot
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCFakeMove.h"
#include "Player.h"
#include "ClientDef.h"
#include "MFakeCreature.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCFakeMoveHandler::execute ( GCFakeMove * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	MCreature *pCreature = g_pZone->GetCreature(pPacket->getObjectID());
	
	if(pCreature == NULL) return;

	MFakeCreature* pFakeCreature = NewFakeCreature( pCreature, pCreature->GetX(), pCreature->GetY() );
	
	pFakeCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_FAST_MOVE_ACTION );

	pFakeCreature->SetFakeCreatureFastMoveAction(pPacket->getToX(), pPacket->getToY(), 0, 0);
//	pFakeCreature->SetAction( ACTION_MOVE );
//	pFakeCreature->SetFakePosition( pCreature->GetX(), pCreature->GetY() );
	
	//------------------------------------------------------
	// Fake Creature를 Zone에 추가
	//------------------------------------------------------
	if (!g_pZone->AddFakeCreature( pFakeCreature ))
	{
		delete pFakeCreature;
	}

#endif

	__END_CATCH
}
