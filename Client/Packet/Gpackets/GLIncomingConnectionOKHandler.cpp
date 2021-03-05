//--------------------------------------------------------------------------------
//
// Filename    : GLIncomingConnectionOKHandler.cpp
// Written By  : Reiot
// Description :
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GLIncomingConnectionOK.h"

#ifdef __LOGIN_SERVER__

	#include "LoginPlayer.h"
	#include "LoginPlayerManager.h"
	#include "Assert.h"

	#include "Lpackets/LCReconnect.h"

#endif

//--------------------------------------------------------------------------------
// 
// GLIncomingConnectionOKHander::execute()
// 
// 게임 서버로부터 GLIncomingConnectionOK 패킷이 날아오면, 로그인 서버는 이 허가가 
// 어느 플레이어에 대한 허가인지 찾아내야 한다. 그 후, 이 플레이어에게 LCReconnect
// 패킷을 던져줘야 한다.
// 
//--------------------------------------------------------------------------------
void GLIncomingConnectionOKHandler::execute ( GLIncomingConnectionOK * pPacket )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __LOGIN_SERVER__

	try {	

		// 플레이어 아이디를 사용해서 플레이어 객체에 접근한다.
		LoginPlayer * pLoginPlayer = g_pLoginPlayerManager->getPlayer( pPacket->getPlayerID() );

		//cout << "PlayerStatus : " << (int)pLoginPlayer->getPlayerStatus() << endl;

		Assert( pLoginPlayer->getPlayerStatus() == LPS_AFTER_SENDING_LG_INCOMING_CONNECTION );

		cout << "+--------------------------------+" << endl
			 << "| Please Reconnect to GameServer |" << endl
			 << "+--------------------------------+" << endl;

        // 클라이언트에게 게임 서버로 재접속하라고 알려준다.
        LCReconnect lcReconnect;
        lcReconnect.setGameServerIP( pPacket->getHost() );
        lcReconnect.setGameServerPort( pPacket->getTCPPort() );
        lcReconnect.setKey( pPacket->getKey() );

		// LCReconnect 패킷을 전송한다.
        pLoginPlayer->sendPacket( &lcReconnect );

		// 연결을 종료한다.
		pLoginPlayer->disconnect( UNDISCONNECTED );

		// LPM에서 삭제한다.
		g_pLoginPlayerManager->deletePlayer( pLoginPlayer->getSocket()->getSOCKET() );

		// LoginPlayer 객체를 삭제한다.
		delete pLoginPlayer;

	} catch ( NoSuchElementException & nsee ) {
		cout << "Player not exist or already disconnected" << endl;
	}

#endif
		
	__END_CATCH
}
