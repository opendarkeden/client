//--------------------------------------------------------------------------------
// ClientCommunicationManager.h
//--------------------------------------------------------------------------------
// 다른 클라이언트로부터의 정보를 받아들어 
//--------------------------------------------------------------------------------
#include "Client_PCH.h"
#include "ClientCommunicationManager.h"
#include "Datagram.h"
#include "DatagramPacket.h"
#include "ClientConfig.h"
#include "PacketValidator.h"
#include "DebugInfo.h"
#include "MTestDef.h"

//--------------------------------------------------------------------------------
// Global
//--------------------------------------------------------------------------------
ClientCommunicationManager*	g_pClientCommunicationManager = NULL;

void	SendBugReport(const char *bug, ...);

//--------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------
ClientCommunicationManager::ClientCommunicationManager ()
    throw ( Error )
: m_pDatagramSocket(NULL)
{
    __BEGIN_TRY

	try {
		// create datagram server socket
		m_pDatagramSocket = new DatagramSocket( g_pClientConfig->CLIENT_COMMUNICATION_UDP_PORT );
		
		SocketAPI::setsocketnonblocking_ex( m_pDatagramSocket->getSOCKET(), true );

//		m_pDatagramSocket->
	} catch (Throwable& t)	{
		DEBUG_ADD_FORMAT_ERR("[Error] CCM-%s", t.toString().c_str());
	}

    __END_CATCH
}

//--------------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------------
ClientCommunicationManager::~ClientCommunicationManager ()
    throw ( Error )
{
    __BEGIN_TRY

    if ( m_pDatagramSocket != NULL ) {
        delete m_pDatagramSocket;
        m_pDatagramSocket = NULL;
    }

    __END_CATCH
}


//--------------------------------------------------------------------------------
// send datagram to datagram-socket
//--------------------------------------------------------------------------------
void ClientCommunicationManager::sendDatagram ( Datagram * pDatagram )
    throw ( ConnectException , Error )
{
    __BEGIN_TRY

    try
    {
        m_pDatagramSocket->send( pDatagram );
    }
    catch ( ConnectException )
    {
		throw ConnectException( "ClientCommunicationManager::sendDatagram 상위로 던진다");
    }

    __END_CATCH
}


//--------------------------------------------------------------------------------
// send datagram-packet to datagram-socket
//--------------------------------------------------------------------------------
void ClientCommunicationManager::sendPacket ( const std::string& host , uint port , DatagramPacket * pPacket )
    throw ( ConnectException , Error )
{
    __BEGIN_TRY
    __BEGIN_DEBUG

	if (host.size()==0)
	{
		DEBUG_ADD("[Error] ClientCommunicationManager-sendPacket-host NULL");
		return;
	}

    try {

        // 데이터그램 객체를 하나 두고, 전송할 peer 의 호스트와 포트를 지정한다.
        Datagram datagram;

		datagram.setHost(host);
        datagram.setPort(port);

        // 데이터그램 패킷을 데이터그램에 집어넣는다.
        datagram.write(pPacket);

        // 데이터그램 소켓을 통해서 데이터그램을 전송한다.
        m_pDatagramSocket->send( &datagram );

		#ifdef __METROTECH_TEST__
			g_UDPTest.UDPPacketSend ++;
		#endif

		#ifdef __DEBUG_OUTPUT__
			DEBUG_ADD_FORMAT("[To] %s(%d)", host.c_str(), port);
			DEBUG_ADD_FORMAT("[Send] %s", pPacket->toString().c_str());
		#endif

    } catch ( Throwable & t ) {
		// -_- 어차피 튕기니까 스트링으로
		if( strstr( t.toString().c_str(), "InvalidProtocolException") != NULL )
			if( !strstr( t.toString().c_str(), "(datagram)" ) == NULL )
				SendBugReport( t.toString().c_str() );
		
        DEBUG_ADD( t.toString().c_str() );        
    }

    __END_DEBUG
    __END_CATCH
}

//--------------------------------------------------------------------------------
// Update
//--------------------------------------------------------------------------------
void
ClientCommunicationManager::Update()
{	
	const int maxPacket = g_pClientConfig->MAX_PROCESS_PACKET;
	
	for (int i=0; i<maxPacket; i++)
	{
		//DEBUG_ADD_FORMAT("[CC-Update] %d", i);

		Datagram*       pDatagram       = NULL;
		DatagramPacket* pDatagramPacket = NULL;
	
		try
		{
			// 데이터그램 객체를 끄집어낸다.
			pDatagram = m_pDatagramSocket->receive();

			if (pDatagram==NULL)
				break;

			DEBUG_ADD("[CCM-Update] something");
			
			// 데이터그램 패킷 객체를 끄집어낸다.
			pDatagram->read( pDatagramPacket );

			#ifdef __METROTECH_TEST__
				g_UDPTest.UDPPacketRead ++;
			#endif

			if (pDatagramPacket!=NULL)
			{
				#ifdef __DEBUG_OUTPUT__
					DEBUG_ADD_FORMAT("[RECEIVE] %s", pDatagramPacket->toString().c_str());
				#endif

				// 걍 한번 체크..
				if ( !g_pPacketValidator->isValidPacketID( CPS_CLIENT_COMMUNICATION_NORMAL, pDatagramPacket->getPacketID() ))
				{
					throw InvalidProtocolException("invalid packet ORDER");
				}			

				// 끄집어낸 데이터그램 패킷 객체를 실행한다.
				DEBUG_ADD_FORMAT("[From] %s(%d)", pDatagramPacket->getHost().c_str(),
													pDatagramPacket->getPort());

				pDatagramPacket->execute(NULL);

				#ifdef __METROTECH_TEST__
					g_UDPTest.UDPPacketExecute ++;
				#endif

				// 데이터그램 패킷 객체를 삭제한다.
				delete pDatagramPacket;
			}

			// 데이터그램 객체를 삭제한다.
			delete pDatagram;
			
		}
		catch ( Throwable & t )
		{
			// -_- 어차피 튕기니까 스트링으로
			if( strstr( t.toString().c_str(), "InvalidProtocolException") != NULL )
				if( !strstr( t.toString().c_str(), "(datagram)" ) == NULL )
					SendBugReport( t.toString().c_str() );
		
			DEBUG_ADD( t.toString().c_str() );

			if (pDatagramPacket!=NULL)
				delete pDatagramPacket;

			if (pDatagram!=NULL)
				delete pDatagram;			
		}
	}

}