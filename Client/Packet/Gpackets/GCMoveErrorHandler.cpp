//////////////////////////////////////////////////////////////////////
//
// Filename    : GCMoveErrorHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCMoveError.h"
#include "ClientDef.h"

#ifdef __GAME_CLIENT__
	#include "ClientPlayer.h"
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCMoveErrorHandler::execute ( GCMoveError * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__
/*
	ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

	pClientPlayer->setX( pPacket->getX() );
	pClientPlayer->setY( pPacket->getY() );

	cout << "Move Back To (" << (int)pPacket->getX() << "," << (int)pPacket->getY() << ")" << endl;
	
#elif __WINDOWS__
*/

	// 서버 좌표 설정
	// 2001.10.8 제거
	g_pPlayer->SetServerPosition( pPacket->getX(), pPacket->getY() );

	// MoveOK message가 하나 안 날아온다는걸 알 수 있다.
	g_pPlayer->PacketMoveNO();
	

#endif

	__END_CATCH
}
