////////////////////////////////////////////////////////////////////////
//
// SocketImpl.h
//
// by Reiot
//
////////////////////////////////////////////////////////////////////////

#ifndef __SOCKET_IMPL_H__
#define __SOCKET_IMPL_H__

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "Types.h"
#include "Exception.h"
#include "SocketAPI.h"

//////////////////////////////////////////////////
// forward declarations
//////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//
// class SocketImpl
//
// TCP Socket Implementation Class
//
////////////////////////////////////////////////////////////////////////

class SocketImpl {
	
//////////////////////////////////////////////////
// constructor/destructor
//////////////////////////////////////////////////
public :
	
	// constructor
	SocketImpl () throw ();
	SocketImpl ( uint port ) throw ();
	SocketImpl ( const std::string & host , uint port ) throw ();

	// copy constructor
	SocketImpl ( const SocketImpl & impl ) throw ();

	// virtual destructor
	virtual ~SocketImpl () throw ( );

//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public :
	
	// create socket
	void create () throw ( );
	
	// close connection
	void close () throw ( );
	
	// bind socket
	void bind () throw ( );
	void bind ( uint port ) throw ( );
	
	// listen
	void listen ( uint backlog ) throw ( );
	
	// connect to remote host
	void connect () throw ( );
	void connect ( const std::string & host , uint port ) throw ( );
	
	// accept new connection 
	SocketImpl * accept () throw ( );
	
	// send data to peer
	uint send ( const void * buf , uint len , uint flags = 0 ) throw ( );
	
	// receive data from peer
	uint receive ( void * buf , uint len , uint flags = 0 ) throw ( );
	
	// how much available?
	uint available () const throw ( );
	

//////////////////////////////////////////////////
// socket option specific methods
//////////////////////////////////////////////////
public :

	// get/set socket's linger status
	uint getLinger () const throw ( );
	void setLinger ( uint lingertime ) throw ( );
	
	// get/set socket's reuse address status
	bool isReuseAddr () const throw ( );
	void setReuseAddr ( bool on = true ) throw ( );
	
	// get/set socket's nonblocking status
	bool isNonBlocking () const throw ( );
	void setNonBlocking ( bool on = true ) throw ( );
	
	// get/set receive buffer size
	uint getReceiveBufferSize () const throw ( );
	void setReceiveBufferSize ( uint size ) throw ( );
	
	// get/set send buffer size
	uint getSendBufferSize () const throw ( );
	void setSendBufferSize ( uint size ) throw ( );
	

//////////////////////////////////////////////////
// socket information specific methods
//////////////////////////////////////////////////
public :

    // get/set host address and port of this socket
    std::string getHost () const throw () { return m_Host; }
	uint getPort () const throw () { return m_Port; }

	// check if socket is valid
	bool isValid () const throw () { return m_SocketID != INVALID_SOCKET; }
	
	// get socket descriptor
	SOCKET getSOCKET () const throw () { return m_SocketID; }


//////////////////////////////////////////////////
// protected methods
//////////////////////////////////////////////////
protected :

    // get/set host address from socket address structure
    std::string _getHost () const throw ();
    void _setHost ( const std::string & host ) throw ();
			    
    // get/set port from socket address structure
	uint _getPort () const throw ();
	void _setPort ( uint port ) throw ();
	
	
//////////////////////////////////////////////////
// attributes
//////////////////////////////////////////////////
protected :
	
	// socket descriptor
	SOCKET m_SocketID;
	
	// socket address structure
	SOCKADDR_IN m_SockAddr;
	
	// peer host
	std::string m_Host;
	
	// peer port
	uint m_Port;

public:
	char m_key;
	void EnData(char* buf,uint len) throw();
};

#endif
