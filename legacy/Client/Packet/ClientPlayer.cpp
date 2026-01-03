//--------------------------------------------------------------------------------
// 
// Filename    : ClientPlyaer.cpp 
// Written By  : Reiot
// 
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "ClientPlayer.h"
#include "Assert.h"
#include "Packet.h"
#include "PacketFactoryManager.h"
#include "PacketValidator.h"
#include "ClientConfig.h"
#include "SocketEncryptInputStream.h"
#include "SocketEncryptOutputStream.h"
#include "DebugInfo.h"
//#include "MPlayer.h"
#include "MZone.h"
#include "UserInformation.h"
//#include "minTR.H" 
#include <fstream>

#include "DebugKit.h"
extern CMessageStringTable g_MessageStringTable;
extern BOOL g_bMsgOutPutFlag;
extern BOOL g_bMsgDetailFlag;
extern BOOL g_bMsgContentFlag;

extern void PrintMessageDetail(ofstream file, char *strMsg, int length);

void	SendBugReport(const char *bug, ...);
//--------------------------------------------------------------------------------
//
// constructor
//
//--------------------------------------------------------------------------------
ClientPlayer::ClientPlayer ( Socket * pSocket )
	 throw ( Error )
: m_PlayerStatus(CPS_NONE)
{
	__BEGIN_TRY
		
	Assert( pSocket != NULL );
	m_pSocket = pSocket;

#ifdef __USE_ENCRYPTER__
	// create socket input stream
	m_pInputStream = new SocketEncryptInputStream( m_pSocket, 32768 );

	Assert( m_pInputStream != NULL );
	
	// create socket output stream
	m_pOutputStream = new SocketEncryptOutputStream( m_pSocket, 32768 );

	Assert( m_pOutputStream != NULL );
#else
	// create socket input stream
	m_pInputStream = new SocketInputStream( m_pSocket, 32768 );

	Assert( m_pInputStream != NULL );
	
	// create socket output stream
	m_pOutputStream = new SocketOutputStream( m_pSocket, 32768 );

	Assert( m_pOutputStream != NULL );
#endif
	__END_CATCH
}


//--------------------------------------------------------------------------------
//
// destructor
//
//--------------------------------------------------------------------------------
ClientPlayer::~ClientPlayer ()
	 throw ( Error )
{
	__BEGIN_TRY
		
	// 그 어떤 플레이어 객체가 삭제될 때에도, 그 상태는 로그아웃이어야 한다.
	// 즉 어떤 플레이어를 접속 종료 시키려면, 그 상태를 로그아웃으로 만들어야 한다.
	Assert( m_PlayerStatus == CPS_END_SESSION );

	// delete all previous packets
	while ( ! m_PacketHistory.empty() ) {
		Packet * pPacket = m_PacketHistory.front();
		delete pPacket;
		m_PacketHistory.pop_front();
	}
	
	__END_CATCH
}

void ClientPlayer::setPlayerStatus ( PlayerStatus playerStatus ) throw () { 
	m_PlayerStatus = playerStatus; 
	DEBUG_ADD_FORMAT_ERR("[ClientPlayer::setPlayerStatus] %d", playerStatus);
}


//--------------------------------------------------------------------------------
//
// parse packet and execute handler for the packet
//
//--------------------------------------------------------------------------------
void ClientPlayer::processCommand () 
     throw ( IOException , Error )
{
	__BEGIN_TRY

	Packet * pPacket;

	try {
		try {

			// 헤더를 임시저장할 버퍼 생성
			char header[szPacketHeader];
			PacketID_t packetID = 0;
			PacketSize_t packetSize = 0;		
			pPacket = NULL;

			//---------------------------------------------------------
			// 이번 Loop에서 처리한 packet의 개수
			//---------------------------------------------------------
			// packet이 너무 많은 경우.. 
			// 다 처리하다보면 .. 시간이 많이 걸려서 client가 안 돌아간다
			// 그래서 일정 개수만 처리한다.
			//---------------------------------------------------------
			int maxProcessPacket = g_pClientConfig->MAX_PROCESS_PACKET;
			int processedPacket = 0;
 
			// 입력버퍼에 들어있는 완전한 패킷들을 모조리 처리한다.
			while ( true ) {
			
				// 입력스트림에서 패킷헤더크기만큼 읽어본다.
				// 만약 지정한 크기만큼 스트림에서 읽을 수 없다면,
				// Insufficient 예외가 발생하고, 루프를 빠져나간다.
				if (m_pInputStream->peek( header , szPacketHeader ) == false) {
					break;
				}

				// 패킷아이디 및 패킷크기를 알아낸다.
				// 이때 패킷크기는 헤더를 포함한다.

				memcpy( &packetID   , &header[0] , szPacketID ); 	
				memcpy( &packetSize , &header[szPacketID] , szPacketSize );
				byte seq = header[szPacketID+szPacketSize];

#ifdef __DEBUG_OUTPUT__
				ofstream file("packetID.log", ios::out | ios::app);
				file << "*** RECEIVED PacketID=" << packetID << ", PacketSize=" << packetSize << ", Seq=" << (int)seq << endl;
				if (packetID == 0) {
					file << " StreamLength " << m_pInputStream->length() << endl;
				}
				file.close();
#endif

/*
#ifdef DEBUG_INFO
				if(g_bMsgOutPutFlag)
				{
					SYSTEMTIME curTime;
					GetLocalTime(&curTime);
					char strTime[64];
					memset(strTime,0,sizeof(strTime));
					sprintf(strTime,"(%02d-%02d %02d:%02d:%02d)",
						(BYTE)curTime.wMonth,(BYTE)curTime.wDay,
						(BYTE)curTime.wHour,(BYTE)curTime.wMinute,(BYTE)curTime.wSecond);
					ofstream file("MessageLog.log", ios::out | ios::app);
					file << strTime << "<-- RCV[ " << g_MessageStringTable.GetMessageName(packetID) << " ]" << ", \tPacketID=" << packetID << ", PacketSize=" << packetSize << endl;
					if(g_bMsgContentFlag)
					{
						char strMsg[1024];
						int nMsgLength = szPacketHeader + packetSize;
						if(nMsgLength>64)
							nMsgLength = 64;
						m_pInputStream->peek(strMsg,nMsgLength);
						PrintMessageDetail(file,strMsg,nMsgLength);
					}
					file.close();
				}
#endif
*/
				//#ifdef	__DEBUG_OUTPUT__
					//	DEBUG_ADD_FORMAT("ID=%d (%s), size=%d", packetID, g_pPacketFactoryManager->getPacketName( packetID ), packetSize);
				//#else
					//	DEBUG_ADD_FORMAT("ID=%d, size=%d", packetID, packetSize);
				//#endif

				// 패킷 아이디가 이상하면 프로토콜 에러로 간주한다.


				if ( packetID >= Packet::PACKET_MAX ) 
				{					
					DEBUG_ADD_FORMAT_ERR("[PacketError-ClientPlayer::processCommand] exceed MAX=%d. packetID=%u", Packet::PACKET_MAX, packetID);
					SendBugReport("Exceed PacketID:%d",packetID);
					
					throw InvalidProtocolException("[PacketError-ClientPlayer::processCommand] exceed MAX packetID");
				}

/*
				#ifdef __DEBUG_OUTPUT__
				if (packetID != 0) {
					DEBUG_ADD_FORMAT("[RECEIVE] [ID=%u] %s", packetID, g_pPacketFactoryManager->getPacketName(packetID).c_str());
				} else {
					DEBUG_ADD_FORMAT("[RECEIVE] wrong packet ID = 0 here?");
				}
				#endif
*/				
				BOOL bExecute = TRUE;

				try {
					if ( ! g_pPacketValidator->isValidPacketID( getPlayerStatus() , packetID ) )
					{
						//---------------------------------------------------------------
						// Logout해서 캐릭터 선택화면으로 가는 경우
						//---------------------------------------------------------------
						if (getPlayerStatus()==CPS_WAITING_FOR_GC_RECONNECT_LOGIN)
						{
							DEBUG_ADD_ERR("[PacketError] ignore Packet when RECONNECT");

							// 읽어내고 execute는 하지 않는다.
							bExecute = FALSE;
							
							// 으헤헤.. 무한~~ -_-;
							maxProcessPacket = 0xFFFF;
						}
						//---------------------------------------------------------------
						// 일반적인 경우는 접속을 끊는다.
						//---------------------------------------------------------------
						else
						{
							DEBUG_ADD_FORMAT("[PacketError] invalid packet ORDER %d", getPlayerStatus());
							throw InvalidProtocolException("invalid packet ORDER");
						}				
					}
					
					
						
				//} catch (InvalidProtocolException& t) {
				} catch (IgnorePacketException& t)	{
					
					DEBUG_ADD_ERR(t.toString().c_str());

					bExecute = FALSE;

					//add by viva
					if(packetID == Packet::PACKET_GC_FRIEND_CHATTING)
							bExecute = TRUE;
				}

//				DEBUG_ADD_FORMAT_ERR("*** RECEIVED Read OK0 PacketID=%u, Size=%d",  packetID, packetSize);
				// 패킷 크기가 너무 크면 프로토콜 에러로 간주한다.
				if ( packetSize > g_pPacketFactoryManager->getPacketMaxSize( packetID ) )
				{
					DEBUG_ADD_FORMAT_ERR("[PacketError] too large packet SIZE: %d/%d", packetSize, g_pPacketFactoryManager->getPacketMaxSize( packetID ));		
					SendBugReport("too large PacketSize ID)%d %d/%d", packetID, packetSize, g_pPacketFactoryManager->getPacketMaxSize( packetID ) );
					throw InvalidProtocolException("too large packet SIZE");
				}
				
				// 입력버퍼내에 패킷크기만큼의 데이타가 들어있는지 확인한다.
				// 최적화시 break 를 사용하면 된다. (여기서는 일단 exception을 쓸 것이다.)
				if ( m_pInputStream->length() < szPacketHeader + packetSize )
				{
					DEBUG_ADD_FORMAT_ERR("[PacketError] InsufficientDataException: %d/%d", m_pInputStream->length(), szPacketHeader + packetSize);
		
					throw InsufficientDataException();
				}
				
				// 여기까지 왔다면 입력버퍼에는 완전한 패킷 하나 이상이 들어있다는 뜻이다.
				// 패킷팩토리매니저로부터 패킷아이디를 사용해서 패킷 스트럭처를 생성하면 된다.
				// 패킷아이디가 잘못될 경우는 패킷팩토리매니저에서 처리한다.
				pPacket = g_pPacketFactoryManager->createPacket( packetID );

				// 이제 이 패킷스트럭처를 초기화한다.
				// 패킷하위클래스에 정의된 read()가 virtual 메커니즘에 의해서 호출되어
				// 자동적으로 초기화된다.
				m_pInputStream->read( pPacket );

				// 이제 이 패킷스트럭처를 가지고 패킷핸들러를 수행하면 된다.
				// 패킷아이디가 잘못될 경우는 패킷핸들러매니저에서 처리한다.			

				if (bExecute)
				{
					#ifdef __DEBUG_OUTPUT__
						DEBUG_ADD(pPacket->toString().c_str());
					#endif

//					_MinTrace("Incomming Packet ID : %d\n",pPacket->getPacketID());

					pPacket->execute( this );

					//DEBUG_ADD_FORMAT("[Executed] %s", pPacket->toString().c_str());
					DEBUG_ADD("[PacketExecute OK1]");				
				}
				
				
				// 현재 패킷을 패킷 히스토리의 맨 뒤에 넣는다.
				m_PacketHistory.push_back(pPacket);
				pPacket = NULL;

				// 패킷을 nPacketHistory 개만큼만 저장한다.
				while ( m_PacketHistory.size() > nPacketHistory ) {
					Packet * oldPacket = m_PacketHistory.front();
					delete oldPacket;
					m_PacketHistory.pop_front();
				}
				DEBUG_ADD("[PacketExecute OK2]");
				
				//---------------------------------------------------------	
				// 한번에 처리하는 packet의 한계 개수를 넘어간 경우
				// 나머지는 다음 turn에 처리한다.
				//---------------------------------------------------------
				if (++processedPacket > maxProcessPacket)
				{
					DEBUG_ADD_FORMAT("[PacketSkip] So many Packets. MaxProcessPacket:%d, CurrentPacket:%d, File:%s, Line:%s",maxProcessPacket,processedPacket, __FILE__,__LINE__);
					//DEBUG_ADD_FORMAT_WAR(szBuf);

					break;
				}			
			}

		} catch ( InsufficientDataException& ) {

			// 단지 루프의 탈출 조건일 뿐이다. 상위로 전달할 필요는 없다.
		}

	} catch (Throwable&)	{
		
		if (pPacket!=NULL)
		{
			delete pPacket;
			pPacket = NULL;
		}

		throw;
	}

	__END_CATCH
}
		    

//--------------------------------------------------------------------------------
// disconnect player
//--------------------------------------------------------------------------------
void ClientPlayer::disconnect ( bool bDisconnected )
	throw ( Error )
{
	__BEGIN_TRY

	if ( bDisconnected == UNDISCONNECTED ) {

		// 클라이언트에게 GCDisconnect 패킷을 전송한다.
		//GCDisconnect gcDisconnect;
		//sendPacket( gcDisconnect );

		// 출력 버퍼에 남아있는 데이타를 전송한다.
		try {
			m_pOutputStream->flush();
		} catch (ConnectException& t)	{
			DEBUG_ADD_ERR(t.toString().c_str());
		} catch (Throwable& t) {
			DEBUG_ADD_ERR(t.toString().c_str());
		}

	}

	// 소켓 연결을 닫는다.
	try {
		m_pSocket->close();
	} catch (Throwable& ) {
		// 그냥 무시
	}

	// 플레이어의 상태를 로그아웃으로 만든다.
	//cout<< "PlayerStatus : " << (int)m_PlayerStatus << endl;
	// 2001.10.5 주석처리 ..으흠..
	//Assert( m_PlayerStatus != CPS_END_SESSION );
	m_PlayerStatus = CPS_END_SESSION;
	// add by Coffee 2007-7-15
	m_pOutputStream->InitSeq();
	__END_CATCH
}


//--------------------------------------------------------------------------------
//
// 최근 N 번째의 패킷을 리턴한다.
// 
// N == 0 일 경우, 가장 최근의 패킷을 리턴하게 된다.
//
// 최대 nPacketHistory - 1 까지 지정할 수 있다. 
//
//--------------------------------------------------------------------------------
Packet * ClientPlayer::getOldPacket ( uint prev )
	throw ( OutOfBoundException , NoSuchElementException )
{
	__BEGIN_TRY

	if ( prev >= nPacketHistory )
		throw OutOfBoundException();

	// if prev == 0 , return m_PacketHistory[9]
	// if prev == 9 , return m_PacketHistory[0]
	Packet * pPacket = m_PacketHistory[ nPacketHistory - prev - 1 ];

	if ( pPacket == NULL )
		throw NoSuchElementException("packet history is empty");

	return pPacket;

	__END_CATCH
}


//--------------------------------------------------------------------------------
//
// 특정 패킷아이디를 가진 가장 최근의 패킷을 리턴한다.
//
//--------------------------------------------------------------------------------
Packet * ClientPlayer::getOldPacket ( PacketID_t packetID )
	throw ( NoSuchElementException )
{
	__BEGIN_TRY

	Packet * pPacket = NULL;

	for ( std::deque<Packet*>::reverse_iterator ritr = m_PacketHistory.rbegin() ;
			ritr != m_PacketHistory.rend() ;
			ritr ++ ) {
		if ( (*ritr)->getPacketID() == packetID ) {
			pPacket = (*ritr);
			break;
		}	
	}

	if ( pPacket == NULL )
		throw NoSuchElementException();

	return pPacket;

	__END_CATCH
}


//--------------------------------------------------------------------------------
//
// get debug std::string
//
//--------------------------------------------------------------------------------
std::string ClientPlayer::toString () const
       throw ( Error )
{
	__BEGIN_TRY
		
	StringStream msg;
	
	msg << "ClientPlayer("
		<< "SocketID:" << m_pSocket->getSOCKET() 
		<< ",Host:" << m_pSocket->getHost() 
		<< ")" ;

	return msg.toString();

	__END_CATCH
}



// 암호화 코드를 설정한다.

void ClientPlayer::setEncryptCode()
    throw (Error)
{
#ifdef __USE_ENCRYPTER__
    __BEGIN_TRY

//	Assert(g_pPlayer!=NULL);
	Assert(g_pZone!=NULL);

	// 일단은 ObjectID를 이용한다.
//	ObjectID_t objectID = g_pPlayer->GetID();
	ZoneID_t zoneID = g_pZone->GetID();
	int serverID = g_pUserInformation->ServerID;

//	if (objectID!=0)
	{
//		uchar code = (uchar)(objectID / zoneID + objectID);		
		uchar code;

		if( g_pUserInformation->IsNetmarble )
			code = (uchar)( ( ( ( zoneID ) >> 8 ) ^ ( zoneID ) ) ^ ( ( ( serverID ) + 1 ) << 4 ) );
		else if ( g_pUserInformation->bChinese )
//			code = (uchar) ( ( ( ( ( serverID ) + 1 ) << 4 ) | ( zoneID ) ) ^ ( ( zoneID ) >> 8 ) );
			code = (uchar)( ( ( ( zoneID ) >> 8 ) ^ ( zoneID ) ) ^ ( ( ( serverID ) + 1 ) << 4 ) );
		else if ( g_pUserInformation->bEnglish )
			code = (uchar)( ( ( ( zoneID ) >> 8 ) ^ ( zoneID ) ) ^ ( ( ( serverID ) + 1 ) * 51 ) );
		else
			code = (uchar)( ( ( ( zoneID ) >> 8 ) ^ ( zoneID ) ) ^ ( ( ( serverID ) + 1 ) << 4 ) );

		SocketEncryptOutputStream* pEOS = dynamic_cast<SocketEncryptOutputStream*>(m_pOutputStream);
		Assert(pEOS!=NULL);

		SocketEncryptInputStream* pEIS = dynamic_cast<SocketEncryptInputStream*>(m_pInputStream);
		Assert(pEIS!=NULL);

		pEOS->setEncryptCode(code);
		pEIS->setEncryptCode(code);
	}

	__END_CATCH
#endif
}
