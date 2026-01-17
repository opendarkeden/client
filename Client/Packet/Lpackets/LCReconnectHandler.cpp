//--------------------------------------------------------------------------------
//
// Filename    : LCReconnectHandler.cpp
// Written By  : Reiot
// Description : 
//
//--------------------------------------------------------------------------------

// include files

#include "Client_PCH.h"
#include "LCReconnect.h"
#include "ClientDef.h"
#include "ServerInfoFileParser.h"

#ifdef __GAME_CLIENT__
	#include "ClientPlayer.h"
	#include "Cpackets/CGConnect.h"
	#include "Cpackets/CGPortCheck.h"
	#include "UserInformation.h"
	#include "Properties.h"
	#include "ClientCommunicationManager.h"
	//add by viva
	#include "Cpackets/CGConnectSetKey.h"
	//end
#endif

#ifdef OUTPUT_DEBUG
	#include <time.h>

	struct tempStruct
	{
		SYSTEMTIME st;
		DWORD reconnectTickCount;
		DWORD sendCGConnectTickCount;
	};
#endif

//#include "ACProtect.h"

extern int g_Dimension;
extern bool		UpdateSocketOutput();
extern BYTE g_macAddress[6];
//--------------------------------------------------------------------------------
// 로그인서버로부터 게임 서버의 주소와 포트, 그리고 인증키를 받은 즉시
// 게임 서버로 연결한 후, 인증키를 담은 CGConnect 패킷을 전송한다.
//--------------------------------------------------------------------------------
void LCReconnectHandler::execute ( LCReconnect * pPacket , Player * pPlayer )
	 
throw ( ProtocolException , Error )
{
	__BEGIN_TRY

		/*
#ifdef __GAME_CLIENT__

	ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

	// 로그인 서버와의 연결을 종료한다
	// 이때 로그인 서버는 LCReconnect 패킷을 보내면서 연결을 종료한다는 사실에 유의하라.
	cout << "Disconnecting from login server" << endl;
	pClientPlayer->disconnect();

	// LCReconnect 패킷에 들어있는 정보를 사용해서, 게임 서버로 연결한다.
	cout << "Reconnecting to " << pPacket->getGameServerIP() << ":" << pPacket->getGameServerPort() << endl;

	try {

		pClientPlayer->getSocket()->reconnect( pPacket->getGameServerIP() , pPacket->getGameServerPort() );

		// reconnect하게 되면 소켓이 새로 만들어지게 된다.
		// 따라서, 이 소켓 역시 옵션을 새로 지정해줘야 한다.
		pClientPlayer->getSocket()->setNonBlocking();
		pClientPlayer->getSocket()->setLinger(0);

	} catch ( ConnectException & ce ) {
		throw Error(ce.toString());
	}

	// 연결이 이루어지면, 바로 CGConnect 패킷을 전송한다.
	// 이전에 Select 한 PC의 타입과 이름을 클라이언트 플레이어 객체에 저장해둔다.
	cout << "Sending CGConnect with Key(" << pPacket->getKey() << ")" << endl;

	CGConnect cgConnect;
	cgConnect.setKey( pPacket->getKey() );
	cgConnect.setPCType( pClientPlayer->getPCType() );
	cgConnect.setPCName( pClientPlayer->getPCName() );

	pClientPlayer->sendPacket( &cgConnect );
	pClientPlayer->setPlayerStatus( CPS_AFTER_SENDING_CG_CONNECT );

#endif
 */
#ifdef OUTPUT_DEBUG
	FILE *fp = NULL;
	DWORD tickCount = 0;


	std::list<struct tempStruct> tempStructList;
	struct tempStruct currentTempStruct;

	fp = fopen("Log\\ConnectTime.txt", "rt");
	if(fp != NULL)
	{
		char readTemp[512];
		fgets(readTemp, 512, fp);
		while(fgets(readTemp, 512, fp) != NULL)
		{
			if(readTemp[0] == 'r')break;
			
			struct tempStruct ts;

			DWORD reconnectTickCount = 0, sendCGConnectTickCount = 0;
			sscanf(readTemp, "%4d/%2d/%2d %2d:%2d:%2d\t%8d\t%8d\n", 
				&ts.st.wYear, &ts.st.wMonth, &ts.st.wDay, &ts.st.wHour, &ts.st.wMinute, &ts.st.wSecond,
				&ts.reconnectTickCount,
				&ts.sendCGConnectTickCount);

			tempStructList.push_back(ts);
		}

		fclose(fp);
	}

// 	fp = fopen("Log\\ConnectTime.txt", "wt");
//	if(fp != NULL)
//	{
// 		fprintf(fp, "CurrentTime\tReconnect TickCount\tSend cgConnect TickCount\n");

		SYSTEMTIME st;
		GetLocalTime(&st);

		currentTempStruct.st = st;
//		fprintf(fp, "%4d/%2d/%2d %2d:%2d:%2d\t", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
//		fclose(fp);
// 	}
#endif

	ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

	// 로그인 서버와의 연결을 종료한다
	// 이때 로그인 서버는 LCReconnect 패킷을 보내면서 연결을 종료한다는 사실에 유의하라.
	
	pClientPlayer->disconnect();

	// LCReconnect 패킷에 들어있는 정보를 사용해서, 게임 서버로 연결한다.
	DEBUG_ADD_FORMAT("Reconnecting to %s:%d", 
										pPacket->getGameServerIP().c_str(), 
										pPacket->getGameServerPort());
	
	try {
#ifdef OUTPUT_DEBUG
		tickCount = timeGetTime();//GetTickCount();
#endif
		pClientPlayer->getSocket()->reconnect( pPacket->getGameServerIP() , pPacket->getGameServerPort() );
#ifdef OUTPUT_DEBUG
		currentTempStruct.reconnectTickCount = timeGetTime()-tickCount;
#endif
		// reconnect하게 되면 소켓이 새로 만들어지게 된다.
		// 따라서, 이 소켓 역시 옵션을 새로 지정해줘야 한다.
		pClientPlayer->getSocket()->setNonBlocking();
		pClientPlayer->getSocket()->setLinger(0);

	} catch ( ConnectException & ce ) {
		throw Error(ce.toString());
	}

	// 연결이 이루어지면, 바로 CGConnect 패킷을 전송한다.
	// 이전에 Select 한 PC의 타입과 이름을 클라이언트 플레이어 객체에 저장해둔다.
	DEBUG_ADD_FORMAT("Sending CGConnect with Key(%ld)", 
												pPacket->getKey());

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
	CGConnect cgConnect;
	cgConnect.setKey( pPacket->getKey() );
	cgConnect.setPCType( pClientPlayer->getPCType() );
	cgConnect.setPCName( pClientPlayer->getPCName() );
	cgConnect.setMacAddress( g_macAddress );
	
#ifdef OUTPUT_DEBUG
	tickCount = timeGetTime();
#endif
	pClientPlayer->sendPacket( &cgConnect );
	pClientPlayer->setPlayerStatus( CPS_AFTER_SENDING_CG_CONNECT );	

	// 바로 보낸다.
//	EMBEDDED_BEGIN;
	UpdateSocketOutput();
//	EMBEDDED_END;
	
#ifdef OUTPUT_DEBUG
	currentTempStruct.sendCGConnectTickCount = timeGetTime()-tickCount;
#endif
	
	// 2002.6.28 [UDP수정]
	// 서버에 UDP port를 알려주기 위해서
	CGPortCheck cgPortCheck;
	cgPortCheck.setPCName( g_pUserInformation->CharacterID.GetString() );

	std::string ServerAddress;
	uint ServerPort;

	if( g_pUserInformation->bKorean )
	{
		ServerAddress = g_pConfigKorean->getProperty("LoginServerAddress");
		ServerPort = g_pConfigKorean->getPropertyInt("LoginServerCheckPort");
		//add by sonic 2006.4.10 쇱꿎角뤠槨굶뒈뒈囹
	}
	else
	{
		ServerAddress = g_pConfigForeign->getProperty( g_Dimension, "LoginServerAddress" );	
		ServerPort = g_pConfigForeign->getPropertyInt(g_Dimension, "LoginServerCheckPort");
	}
	
			
	DEBUG_ADD("[ ClientPacket] Send CGPortCheck ");
	g_pClientCommunicationManager->sendPacket( ServerAddress,
		ServerPort,
		&cgPortCheck );

	DEBUG_ADD("[ MODE ] START SETMODE MODE_WAIT_UPDATEINFO");
	SetMode( MODE_WAIT_UPDATEINFO );
	DEBUG_ADD("[ MODE ] END SETMODE MODE_WAIT_UPDATEINFO");

#ifdef OUTPUT_DEBUG
 	fp = fopen("Log\\ConnectTime.txt", "wt");
	if(fp != NULL)
	{
		tempStructList.push_back(currentTempStruct);

 		fprintf(fp, "CurrentTime\tReconnect TickCount\tSend cgConnect TickCount\n");
		
		DWORD minReconnectTickCount = 0xffffffff, minCGTickCount = 0xffffffff, maxReconnectTickCount = 0, maxCGTickCount = 0, reconnectSum = 0, CGSum = 0;
		int size = tempStructList.size();

		while(!tempStructList.empty())
		{
			tempStruct ts;
			ts = tempStructList.front();
			tempStructList.pop_front();

			fprintf(fp, "%4d/%2d/%2d %2d:%2d:%2d\t%8d\t%8d\n", 
				ts.st.wYear, ts.st.wMonth, ts.st.wDay, ts.st.wHour, ts.st.wMinute, ts.st.wSecond,
				ts.reconnectTickCount,
				ts.sendCGConnectTickCount);

			if(ts.reconnectTickCount < minReconnectTickCount)minReconnectTickCount = ts.reconnectTickCount;
			if(ts.reconnectTickCount > maxReconnectTickCount)maxReconnectTickCount = ts.reconnectTickCount;
			if(ts.sendCGConnectTickCount < minCGTickCount)minCGTickCount = ts.sendCGConnectTickCount;
			if(ts.sendCGConnectTickCount > maxCGTickCount)maxCGTickCount = ts.sendCGConnectTickCount;

			reconnectSum += ts.reconnectTickCount;
			CGSum += ts.sendCGConnectTickCount;

		}

		fprintf(fp, "reconnect TickCount Min : %d Max : %d Avr : %d\n", minReconnectTickCount, maxReconnectTickCount, reconnectSum/size);
		fprintf(fp, "send cgConnect TickCount Min : %d Max : %d Avr : %d\n", minCGTickCount, maxCGTickCount, CGSum/size);
		fclose(fp);
	}
#endif
	__END_CATCH
}
