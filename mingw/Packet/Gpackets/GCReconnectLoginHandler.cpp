//--------------------------------------------------------------------------------
//
// Filename    : GCReconnectLoginHandler.cpp
// Written By  : Reiot
// Description : 
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCReconnectLogin.h"
#include "ClientDef.h"

#ifdef __GAME_CLIENT__
	#include "ClientPlayer.h"
	#include "Cpackets/CLReconnectLogin.h"
	#include "UserInformation.h"
	//add by viva
	#include "Cpackets/CGConnectSetKey.h"
	//end
#endif
//add by viva
bool UpdateSocketOutput();
//end
//--------------------------------------------------------------------------------
// 로그인서버로부터 게임 서버의 주소와 포트, 그리고 인증키를 받은 즉시
// 게임 서버로 연결한 후, 인증키를 담은 CGConnect 패킷을 전송한다.
//--------------------------------------------------------------------------------
void GCReconnectLoginHandler::execute ( GCReconnectLogin * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

 
	ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

	// 로그인 서버와의 연결을 종료한다
	// 이때 로그인 서버는 LCReconnect 패킷을 보내면서 연결을 종료한다는 사실에 유의하라.
	
	pClientPlayer->disconnect();

	// GCReconnectLogin 패킷에 들어있는 정보를 사용해서, login 서버로 연결한다.
	DEBUG_ADD_FORMAT("Reconnecting to %s:%d", 
									pPacket->getLoginServerIP().c_str(), 
									pPacket->getLoginServerPort());
	
	try {

		pClientPlayer->getSocket()->reconnect( pPacket->getLoginServerIP() , pPacket->getLoginServerPort() );

		// reconnect하게 되면 소켓이 새로 만들어지게 된다.
		// 따라서, 이 소켓 역시 옵션을 새로 지정해줘야 한다.
		pClientPlayer->getSocket()->setNonBlocking();
		pClientPlayer->getSocket()->setLinger(0);

	} catch ( ConnectException & ce ) {
		throw Error(ce.toString());
	}

	// 연결이 이루어지면, 바로 CLReconnectLogin 패킷을 전송한다.
	// 이전에 Select 한 PC의 타입과 이름을 클라이언트 플레이어 객체에 저장해둔다.
	DEBUG_ADD_FORMAT("Sending CLReconnectLogin with Key(%ld)", 	pPacket->getKey());
		
	//add by viva	
	CGConnectSetKey cgConnectSetKey;
	cgConnectSetKey.setEncryptKey(rand());
	cgConnectSetKey.setHashKey(rand());
	pClientPlayer->delKey();
	pClientPlayer->sendPacket(&cgConnectSetKey);
	UpdateSocketOutput();
	Sleep(500);
	cgConnectSetKey.execute(pClientPlayer);
	//end

	// 재접속..
	CLReconnectLogin _CLReconnectLogin;
	_CLReconnectLogin.setKey( pPacket->getKey() );
	_CLReconnectLogin.SetLoginMode(g_pUserInformation->IsAutoLogIn);
	
	pClientPlayer->sendPacket( &_CLReconnectLogin );
	pClientPlayer->setPlayerStatus( CPS_AFTER_SENDING_CL_GET_PC_LIST );	

	// pc std::list를 기다린다.
	SetMode( MODE_WAIT_PCLIST  );

#endif
	__END_CATCH
}
