//----------------------------------------------------------------------
//
// Filename    : GCSetPositionHandler.cc
// Written By  : Reiot
// Description :
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCSetPosition.h"
#include "ClientDef.h"

#ifdef __GAME_CLIENT__
	#include "ClientPlayer.h"
#endif

//----------------------------------------------------------------------
// 
// GCSetPositionHander::execute
//
//----------------------------------------------------------------------
void GCSetPositionHandler::execute ( GCSetPosition * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

	DEBUG_ADD_FORMAT("Set Position To ( %d, %d ) to Dir(%d)", (int)pPacket->getX(), (int)pPacket->getY(), (int)pPacket->getDir());		
	
	pClientPlayer->setXY( pPacket->getX() , pPacket->getY() );

	pClientPlayer->setPlayerStatus( CPS_NORMAL ); 


	//--------------------------------------------------------
	// Player의 위치 지정
	//--------------------------------------------------------
	InitPlayer(	pPacket->getX(), 
				pPacket->getY(),
				pPacket->getDir());

	//--------------------------------------------------------
	// 게임 시작..
	//--------------------------------------------------------
	SetMode( MODE_GAME );

#endif
	
	__END_CATCH
}
