//////////////////////////////////////////////////////////////////////
//
// Filename   : DatagramSocket.cpp
// Written By : reiot@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "DatagramSocket.h"
#include "Assert.h"
#include "FileAPI.h"
#include "DebugInfo.h"
#include "MTestDef.h"

//////////////////////////////////////////////////////////////////////
//
// constructor for UDP Client Socket
//
// UDP 클라이언트 소켓은 단지 nonamed 소켓만 생성해 두면 된다.
// 왜냐하면, 서버로 send할 때마다 Datagram의 주소를 지정해두면
// 되기 때문이다.
//
//////////////////////////////////////////////////////////////////////
DatagramSocket::DatagramSocket () 
	throw ( Error )
: m_SocketID(INVALID_SOCKET)
{
	__BEGIN_TRY 

	__BEGIN_DEBUG
	m_SocketID = SocketAPI::socket_ex( AF_INET , SOCK_DGRAM , 0 );
	__END_DEBUG

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// constructor for UDP Server Socket
//
// UDP 서버 소켓은 소켓을 생성하고, port 를 바인딩시키면 준비가 완료된다.
//
//////////////////////////////////////////////////////////////////////
DatagramSocket::DatagramSocket ( uint port ) 
	throw ( Error )
: m_SocketID(INVALID_SOCKET)
{
	__BEGIN_TRY 

	m_SocketID = SocketAPI::socket_ex( AF_INET , SOCK_DGRAM , 0 );

	// clear memory
	memset( &m_SockAddr , 0 , szSOCKADDR_IN );
	m_SockAddr.sin_family      = AF_INET;
	m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_SockAddr.sin_port        = htons(port);

	int opt = 1;
	SocketAPI::setsockopt_ex( m_SocketID, SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt) );

	// bind address to socket
	SocketAPI::bind_ex( m_SocketID , (SOCKADDR*)&m_SockAddr , szSOCKADDR_IN );

	// set host
	const char* pIP = inet_ntoa( m_SockAddr.sin_addr );
	int port2 = m_SockAddr.sin_port;

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
DatagramSocket::~DatagramSocket ()
	throw ( Error )
{
	__BEGIN_TRY
	
	if ( m_SocketID != INVALID_SOCKET )
	{
		try {
			SocketAPI::closesocket_ex( m_SocketID );
		} catch (Throwable& t) {
			DEBUG_ADD( t.toString().c_str() );
		}
	}

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// send datagram to peer
//////////////////////////////////////////////////////////////////////
uint DatagramSocket::send ( Datagram * pDatagram )
	throw ( ConnectException , Error )
{
	__BEGIN_TRY 

	Assert( pDatagram != NULL );

	int nSent = SocketAPI::sendto_ex( m_SocketID , pDatagram->getData() , pDatagram->getLength() , 0 , pDatagram->getAddress() , szSOCKADDR_IN );

	return (uint)nSent;

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// receive datagram from peer
//
// 만약에 이 클래스를 blocking 으로 사용한다면, (즉 select 기반으로)
// 아마도 nReceived 가 0 이하인 경우는 없으리라고 판단된다.
//
//////////////////////////////////////////////////////////////////////
Datagram * DatagramSocket::receive ()
	throw ( ConnectException , Error )
{
	__BEGIN_TRY 

	Datagram * pDatagram = NULL;

	SOCKADDR_IN SockAddr;
	uint _szSOCKADDR_IN = szSOCKADDR_IN;

	// 읽을게 있는지 체크한다.
	ulong available = SocketAPI::availablesocket_ex( m_SocketID );		
	
	if (available > 0)
	{
		#ifdef __METROTECH_TEST__
			g_UDPTest.UDPPacketAvailable ++;
		#endif

		DEBUG_ADD_FORMAT("[DatagramSocket] available=%d", available);

		// 내부 버퍼에다가 복사해둔다.
		int nReceived = SocketAPI::recvfrom_ex( m_SocketID , m_Buffer , DATAGRAM_SOCKET_BUFFER_LEN , 0 , (SOCKADDR*)&SockAddr , &_szSOCKADDR_IN );

		if ( nReceived > 0 ) 
		{
			#ifdef __METROTECH_TEST__
				g_UDPTest.UDPPacketReceive ++;
			#endif

			DEBUG_ADD_FORMAT("[DatagramSocket] received=%d", nReceived);

			pDatagram = new Datagram();
			pDatagram->setData( m_Buffer , nReceived );
			pDatagram->setAddress( &SockAddr );
		}
	}	

	return pDatagram;

	__END_CATCH
}
