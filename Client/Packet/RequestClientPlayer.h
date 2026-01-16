//--------------------------------------------------------------------------------
// 
// Filename    : RequestClientPlayer.h 
// Written by  : sigi
// 
//--------------------------------------------------------------------------------

#ifndef __REQUEST_CLIENT_PLAYER_H__
#define __REQUEST_CLIENT_PLAYER_H__

// include files
#include "Player.h"
#include "PlayerStatus.h"
#include "Packet.h"
#include "types/RequestTypes.h"

//--------------------------------------------------------------------------------
//
// class RequestClientPlayer
//
// 다른 클라이언트에 접속해서 뭔가 요청한다..
// 하나의 RequestServerPlayer와 접속하게 된다.
//
//--------------------------------------------------------------------------------

class RequestClientPlayer : public Player {

public :
	
	// constructor
	RequestClientPlayer ( Socket * pSocket ) throw ( Error );
	
	// destructor
	~RequestClientPlayer () throw ( Error );

public :

	// read socket's receive buffer and fill input buffer
	//virtual void processInput () throw ( IOException , Error );
	
	// parse packet and execute handler for the packet
	virtual void processCommand () throw ( IOException , Error );
	
	// flush output buffer to socket's send buffer
	//virtual void processOutput () throw ( IOException , Error );
	
	// send packet to player's output buffer
	//virtual void sendPacket ( Packet * packet ) throw ( Error );
	
	uint getInputStreamLength () const throw();
	uint readInputStream ( char * buf , uint len ) 	throw ( ProtocolException , Error );


	void				setRequestServerName(const char* pName)	{ m_RequestServerName = pName; }
	const std::string&	getRequestServerName() const	{ return m_RequestServerName; }

	void				setRequestServerIP(const char* pIP)	{ m_RequestServerIP = pIP; }
	const std::string&	getRequestServerIP() const	{ return m_RequestServerIP; }

	// disconnect
	// 정식 로그아웃의 경우 disconnect(UNDISCONNECTED)
	virtual void disconnect ( bool bDisconnected = DISCONNECTED ) throw ( Error );
	
	// get debug string
	virtual std::string toString () const throw ( Error );
	
public :

	// get player's status
	PlayerStatus getPlayerStatus () const throw () { return m_PlayerStatus; }

	// set player's status
	void setPlayerStatus ( PlayerStatus playerStatus ) throw () { m_PlayerStatus = playerStatus; }

	void		setRequestMode(REQUEST_CLIENT_MODE mode)	{ m_RequestMode = mode; }
	REQUEST_CLIENT_MODE			getRequestMode() const	{ return m_RequestMode; }

private :

	std::string					m_RequestServerName;
	std::string					m_RequestServerIP;

	// Request하고 있는 것
	REQUEST_CLIENT_MODE			m_RequestMode;

	DWORD						m_ExpireTime;

	// player status
	PlayerStatus				m_PlayerStatus;
};

#endif
