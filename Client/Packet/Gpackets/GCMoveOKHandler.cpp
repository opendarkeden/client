//--------------------------------------------------------------------------------
//
// Filename    : GCMoveOKHandler.cpp
// Written By  : elca, Reiot
// Description :
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCMoveOK.h"
#include "ClientDef.h"
#include "VS_UI.h"


#ifdef __GAME_CLIENT__
	#include "ClientPlayer.h"
#endif

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCMoveOKHandler::execute ( GCMoveOK * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__
	
	ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

	pClientPlayer->setX( pPacket->getX() );
	pClientPlayer->setY( pPacket->getY() );
	pClientPlayer->setDir( pPacket->getDir() );

	//cout << "Move to (" << (int)pPacket->getX() << "," << (int)pPacket->getY() << ")" << endl;
	

	//--------------------------------------------------
	// 검증된 Tile에 대한 이동
	//--------------------------------------------------
	g_pPlayer->PacketMoveOK(pPacket->getX(), pPacket->getY(), pPacket->getDir());

#endif

	__END_CATCH
}
