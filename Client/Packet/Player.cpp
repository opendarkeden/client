//////////////////////////////////////////////////////////////////////
// 
// Filename    : Plyaer.cpp 
// Written By  : reiot@ewestsoft.com
// Description : 플레이어 베이스 클래스
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "Player.h"
#include "Socket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "Assert.h"
#include "Packet.h"
#include "PacketFactoryManager.h"
#include "DebugInfo.h"

void	SendBugReport(const char *bug, ...);

//////////////////////////////////////////////////////////////////////
//
// constructor
//
//////////////////////////////////////////////////////////////////////
Player::Player ()
	 throw ( Error )
: m_pSocket(NULL), m_pInputStream(NULL), m_pOutputStream(NULL)
{
	__BEGIN_TRY

	Assert( m_pSocket == NULL );

	// create socket input stream
	m_pInputStream = NULL;

	Assert( m_pInputStream == NULL );
	
	// create socket output stream
	m_pOutputStream = NULL;

	Assert( m_pOutputStream == NULL );

	//add by viva
	pHashTable = NULL;
	//end

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// constructor
//
//////////////////////////////////////////////////////////////////////
Player::Player ( Socket * pSocket )
	 throw ( ProtocolException , Error )
: m_pSocket(pSocket), m_pInputStream(NULL), m_pOutputStream(NULL)
{
	__BEGIN_TRY
		
	Assert( m_pSocket != NULL );

	// create socket input stream
	m_pInputStream = new SocketInputStream( m_pSocket, 32768 );

	Assert( m_pInputStream != NULL );
	
	// create socket output stream
	m_pOutputStream = new SocketOutputStream( m_pSocket, 32768 );

	Assert( m_pOutputStream != NULL );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// destructor
//
//////////////////////////////////////////////////////////////////////
Player::~Player ()
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	// delete socket input stream
	if ( m_pInputStream != NULL ) {
		delete m_pInputStream;
		m_pInputStream = NULL;
	}

	// delete socket output stream
	if ( m_pOutputStream != NULL ) {
		delete m_pOutputStream;
		m_pOutputStream = NULL;
	}

	// delete socket
	if ( m_pSocket != NULL ) {
		m_pSocket->close();
		delete m_pSocket;
		m_pSocket = NULL;
	}

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////
void Player::processInput ()
	throw ( IOException , Error )
{
	__BEGIN_TRY

	try {
		m_pInputStream->fill();
	} catch ( NonBlockingIOException ) {
	}

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// parse packet and execute handler for the packet
//
//////////////////////////////////////////////////////////////////////
void Player::processCommand () 
     throw ( IOException , Error )
{
	__BEGIN_TRY

	try {

		// 헤더를 임시저장할 버퍼 생성
		char header[szPacketHeader];
		PacketID_t packetID;
		PacketSize_t packetSize;
		// add by Coffee 藤속룐관埼죗
		SequenceSize_t packetSequence;

		Packet * pPacket;

		// 입력버퍼에 들어있는 완전한 패킷들을 모조리 처리한다.
		while ( true ) {
		
			// 입력스트림에서 패킷헤더크기만큼 읽어본다.
			// 만약 지정한 크기만큼 스트림에서 읽을 수 없다면,
			// Insufficient 예외가 발생하고, 루프를 빠져나간다.
			m_pInputStream->peek( header , szPacketHeader );

			// 패킷아이디 및 패킷크기를 알아낸다.
			// 이때 패킷크기는 헤더를 포함한다.
			memcpy( &packetID   , &header[0] , szPacketID );	
			memcpy( &packetSize , &header[szPacketID] , szPacketSize );
			memcpy( &packetSequence , &header[szPacketID+szPacketSize] , szSequenceSize );

			// 패킷 아이디가 이상하면 프로토콜 에러로 간주한다.
			if ( packetID >= Packet::PACKET_MAX )
				throw InvalidProtocolException("[Player::processCommand]invalid packet id");
			
			// 패킷 크기가 너무 크면 프로토콜 에러로 간주한다.
			if ( packetSize > g_pPacketFactoryManager->getPacketMaxSize(packetID) )
			{
				SendBugReport("too large PacketSize ID)%d %d/%d", packetID, packetSize, g_pPacketFactoryManager->getPacketMaxSize( packetID ) );
				throw InvalidProtocolException("too large packet size");
			}
			
			// 입력버퍼내에 패킷크기만큼의 데이타가 들어있는지 확인한다.
			// 최적화시 break 를 사용하면 된다. (여기서는 일단 exception을 쓸 것이다.)
			if ( m_pInputStream->length() < szPacketHeader + packetSize )
				throw InsufficientDataException();
			
			// 여기까지 왔다면 입력버퍼에는 완전한 패킷 하나 이상이 들어있다는 뜻이다.
			// 패킷팩토리매니저로부터 패킷아이디를 사용해서 패킷 스트럭처를 생성하면 된다.
			// 패킷아이디가 잘못될 경우는 패킷팩토리매니저에서 처리한다.
			pPacket = g_pPacketFactoryManager->createPacket( packetID );

			// 이제 이 패킷스트럭처를 초기화한다.
			// 패킷하위클래스에 정의된 read()가 virtual 메커니즘에 의해서 호출되어
			// 자동적으로 초기화된다.
			m_pInputStream->read( pPacket );
/*
	#ifdef __DEBUG_OUTPUT__
			FILE* fp = fopen("packet.log", "a");
			fprintf (fp, "Read: %s\n", pPacket->toString());
			fclose(fp);
	#endif
*/		
			// 이제 이 패킷스트럭처를 가지고 패킷핸들러를 수행하면 된다.
			// 패킷아이디가 잘못될 경우는 패킷핸들러매니저에서 처리한다.
			pPacket->execute( this );
			
			// 패킷을 삭제한다
			delete pPacket;

		}

	} catch ( NoSuchElementException & nsee ) {

		// PacketFactoryManager::createPacket(PacketID_t)
		// PacketFactoryManager::getPacketMaxSize(PacketID_t)
		// 에서 던질 가능성이 있다.
		throw Error( nsee.toString() );

	} catch ( InsufficientDataException ) {

		// do nothing

	} 

	__END_CATCH
}
		    

//////////////////////////////////////////////////////////////////////
//
// flush output buffer to socket's send buffer
//
//////////////////////////////////////////////////////////////////////
void Player::processOutput ()
	throw ( IOException , Error )
{
	__BEGIN_TRY

	m_pOutputStream->flush();

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// send packet to player's output buffer
//
//////////////////////////////////////////////////////////////////////
void Player::sendPacket ( Packet * pPacket )
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	#ifdef __DEBUG_OUTPUT__
		DEBUG_ADD_FORMAT("[Send] %s", pPacket->toString().c_str() );							
	#endif

	m_pOutputStream->write( pPacket );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// disconnect ( close socket )
//
//////////////////////////////////////////////////////////////////////
void Player::disconnect ( bool bDisconnected )
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	// 정당하게 로그아웃한 경우에는 출력 버퍼를 플러시할 수 있다.
	// 그러나, 불법적인 디스를 걸었다면 소켓이 닫겼으므로
	// 플러시할 경우 SIG_PIPE 을 받게 된다.
	if ( bDisconnected == UNDISCONNECTED ) {

		//
		m_pOutputStream->flush();

	}

	m_pSocket->close();

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// set socket
//////////////////////////////////////////////////////////////////////
void Player::setSocket ( Socket * pSocket )
	throw ()
{
	__BEGIN_TRY

	m_pSocket = pSocket;

	if ( m_pInputStream != NULL ) {
		delete m_pInputStream;
		m_pInputStream = new SocketInputStream( m_pSocket );
	}

	if ( m_pOutputStream != NULL ) {
		delete m_pOutputStream;
		m_pOutputStream = new SocketOutputStream( m_pSocket );
	}

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// get debug string
//
//////////////////////////////////////////////////////////////////////
std::string Player::toString () const
       throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	StringStream msg;
	
	msg << "Player("
		<< "SocketID:" << m_pSocket->getSOCKET() 
		<< ",Host:" << m_pSocket->getHost() 
		<< ",ID:" << m_ID
		<< ")" ;

	return msg.toString();

	__END_CATCH
}
	//add by viva
void Player::setKey(WORD EncryptKey, WORD HashKey) 
	throw()
{
	pHashTable = new BYTE[512];
	BYTE key = (HashKey + 4658)&0x00FF;
	for(int i = 0; i<512; i++)
	{
		key = (key+0xCC)^(key * 0x3)^key;
		pHashTable[i] = key;
	}

	EncryptKey = EncryptKey % 512;
	if( m_pInputStream != NULL)
		m_pInputStream->setKey(EncryptKey, pHashTable);
	if( m_pOutputStream != NULL)
		m_pOutputStream->setKey(EncryptKey, pHashTable);
}
void Player::delKey() 
	throw()
{
	if(pHashTable)
	{
		delete[] pHashTable;
		pHashTable = NULL;
	}
	if( m_pInputStream != NULL)
		m_pInputStream->setKey(0, NULL);
	if( m_pOutputStream != NULL)
		m_pOutputStream->setKey(0, NULL);
}
	//end