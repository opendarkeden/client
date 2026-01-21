//////////////////////////////////////////////////////////////////////
//
// ServerSocket.cpp
//
// by Reiot
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "Client_PCH.h"
#include "ServerSocket.h"

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
ServerSocket::ServerSocket ( uint port , uint backlog ) 
	throw ( BindException , Error )
: m_Impl(NULL)
{
	__BEGIN_TRY
		
	// create socket implementation object
	m_Impl = new SocketImpl(port);
	
	// create socket
	m_Impl->create();
	
	// reuse address before Bind()
	// 바인드 하기 전에 주소 리유즈를 시스템에 알려야 한다.
	m_Impl->setReuseAddr();
	
	// bind address to socket
	// 이미 port가 m_Impl에 저장되어 있으므로, 파라미터없는 Bind()를 호출해도 된다.
	m_Impl->bind();
	
	// set listening queue size
	m_Impl->listen(backlog);

	__END_CATCH
}
	
//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
ServerSocket::~ServerSocket ()
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	if ( m_Impl != NULL ) {
		m_Impl->close();
		delete m_Impl;
		m_Impl = NULL;
	}

	__END_CATCH
}
	

//////////////////////////////////////////////////////////////////////
// close socket
//////////////////////////////////////////////////////////////////////
void ServerSocket::close ()
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	m_Impl->close();

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////
// accept new connection
//////////////////////////////////////////////////////////////////////
Socket * ServerSocket::accept ()
	throw ( ProtocolException , Error )
{
	Socket * Client = NULL;

	try {

		SocketImpl * impl = m_Impl->accept();

		if ( impl == NULL )
			return NULL;  // No connection pending

		Client = new Socket(impl);

	} catch ( NonBlockingIOException ) {
		// ignore - no connection pending
		return NULL;
	} catch ( Throwable & t ) {
		// Log other Throwable exceptions and return NULL
		fprintf(stderr, "ServerSocket::accept() caught Throwable: %s\n", t.toString().c_str());
		return NULL;
	} catch ( std::exception & e ) {
		// Log standard exceptions and return NULL
		fprintf(stderr, "ServerSocket::accept() caught std::exception: %s\n", e.what());
		return NULL;
	} catch ( ... ) {
		// Catch any other exceptions and return NULL
		fprintf(stderr, "ServerSocket::accept() caught unknown exception\n");
		return NULL;
	}

	return Client;
}
