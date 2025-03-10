//////////////////////////////////////////////////////////////////////
// 
// Socket.h 
// 
// by Reiot
// 
//////////////////////////////////////////////////////////////////////

#ifndef __SOCKET_H__
#define __SOCKET_H__

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "Types.h"
#include "Exception.h"
#include "SocketImpl.h"


//////////////////////////////////////////////////////////////////////
//
// class Socket
//
// TCP Client Socket
//
// *Reiot's Note*
//
// 버퍼 크기를 지정하려면, connect() 전에 지정해야 한다.
//
//////////////////////////////////////////////////////////////////////

class Socket {

//////////////////////////////////////////////////
// constructor/destructor
//////////////////////////////////////////////////
public :
	
	// constructor
	Socket () throw ();
	Socket ( const std::string & host , uint port ) throw ();
	Socket ( SocketImpl * impl ) throw ();
	
	// destructor
	virtual ~Socket () throw ( );

	
//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public :
	
	// close connection
	void close () throw ( ) { m_pSocketImpl->close(); }
	
	// try connect to remote host
	void connect () throw ( ) { m_pSocketImpl->connect(); }
	void connect ( const std::string & host , uint port ) throw ( ) { m_pSocketImpl->connect(host,port); }

	// close previous connection and connect to another socket
	void reconnect ( const std::string & host , uint port ) throw ( );
	
	// send data to peer
	uint send ( const void * buf , uint len , uint flags = 0 ) throw ( ) { return m_pSocketImpl->send(buf,len,flags); }
	
	// receive data from peer
	uint receive ( void * buf , uint len , uint flags = 0 ) throw ( ) { return m_pSocketImpl->receive(buf,len,flags); }
	
	//
	uint available () const throw ( ) { return m_pSocketImpl->available(); }


//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public :
 
    // get/set socket's linger status
    uint getLinger () const throw ( ) { return m_pSocketImpl->getLinger(); }
    void setLinger ( uint lingertime ) throw ( ) { m_pSocketImpl->setLinger(lingertime); }
 
    // get/set socket's nonblocking status
    bool isNonBlocking () const throw ( ) { return m_pSocketImpl->isNonBlocking(); }
    void setNonBlocking ( bool on = true ) throw ( ) { m_pSocketImpl->setNonBlocking(on); }
 
    // get/set receive buffer size
    uint getReceiveBufferSize () const throw ( ) { return m_pSocketImpl->getReceiveBufferSize(); }
    void setReceiveBufferSize ( uint size ) throw ( ) { m_pSocketImpl->setReceiveBufferSize(size); }
 
    // get/set send buffer size
    uint getSendBufferSize () const throw ( ) { return m_pSocketImpl->getSendBufferSize(); }
    void setSendBufferSize ( uint size ) throw ( ) { m_pSocketImpl->setSendBufferSize(size); }
 
	// get host & port
	std::string getHost () const throw () { return m_pSocketImpl->getHost(); }
	uint getPort () const throw () { return m_pSocketImpl->getPort(); }

	// check if socket is valid
	bool isValid () const throw () { return m_pSocketImpl->isValid(); }

	// get socket descriptor
	SOCKET getSOCKET () const throw () { return m_pSocketImpl->getSOCKET(); }

	// return debug string ( FD , HOST , PORT )
	std::string toString () const throw ();
	

//////////////////////////////////////////////////
// attributes
//////////////////////////////////////////////////
private :
	
	// socket implementation object
	SocketImpl * m_pSocketImpl;

};

#endif
