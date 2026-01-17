//////////////////////////////////////////////////////////////////////
//
// ServerSocket.h
//
// by Reiot
//
//////////////////////////////////////////////////////////////////////

#ifndef __SERVER_SOCKET__
#define __SERVER_SOCKET__

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "Types.h"
#include "Exception.h"
#include "SocketImpl.h"
#include "Socket.h"


//////////////////////////////////////////////////////////////////////
//
// class ServerSocket
//
//////////////////////////////////////////////////////////////////////

class ServerSocket {

//////////////////////////////////////////////////
// constructor/destructor
//////////////////////////////////////////////////
public :
	
	// constructor
	ServerSocket ( uint port , uint backlog = 5 ) throw ( BindException , Error );
	
	// destructor
	~ServerSocket () throw ( ProtocolException , Error );
	

//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public :
	
	// close socket
	void close () throw ( ProtocolException , Error );
	
	// accept new connection
	Socket * accept () throw ( ProtocolException , Error );

	// get/set socket's linger status
    uint getLinger () const throw ( ProtocolException , Error ) { return m_Impl->getLinger(); }
    void setLinger ( uint lingertime ) throw ( ProtocolException , Error ) { m_Impl->setLinger(lingertime); }
 
    // get/set socket's reuse address status
    bool isReuseAddr () const throw ( ProtocolException , Error ) { return m_Impl->isReuseAddr(); }
    void setReuseAddr ( bool on = true ) throw ( ProtocolException , Error ) { m_Impl->setReuseAddr(on); }
 
    // get/set socket's nonblocking status
    bool isNonBlocking () const throw ( ProtocolException , Error ) { return m_Impl->isNonBlocking(); }
    void setNonBlocking ( bool on = true ) throw ( ProtocolException , Error ) { m_Impl->setNonBlocking(on); }
 
    // get/set receive buffer size
    uint getReceiveBufferSize () const throw ( ProtocolException , Error ) { return m_Impl->getReceiveBufferSize(); }
    void setReceiveBufferSize ( uint size ) throw ( ProtocolException , Error ) { m_Impl->setReceiveBufferSize(size); }
 
    // get/set send buffer size
    uint getSendBufferSize () const throw ( ProtocolException , Error ) { return m_Impl->getSendBufferSize(); }
    void setSendBufferSize ( uint size ) throw ( ProtocolException , Error ) { m_Impl->setSendBufferSize(size); }
 
	SOCKET getSOCKET () throw () { return m_Impl->getSOCKET(); }

//////////////////////////////////////////////////
// attributes
//////////////////////////////////////////////////
protected :
	
	// socket implementation object
	SocketImpl * m_Impl;
	
};
#endif
