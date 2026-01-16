//--------------------------------------------------------------------------------
// 
// Filename    : ClientPlayer.h 
// Written by  : Reiot
// 
//--------------------------------------------------------------------------------

#ifndef __CLIENT_PLAYER_H__
#define __CLIENT_PLAYER_H__

// include files
#include "Player.h"
#include "PlayerStatus.h"
#include <deque>
#include "Packet.h"

//--------------------------------------------------------------------------------
//
// class ClientPlayer
//
// 클라이언트용 플레이어 클래스
//
//--------------------------------------------------------------------------------

// 저장해놓을 이전 패킷의 개수
const BYTE nPacketHistory = 10;

class ClientPlayer : public Player {

public :
	
	// constructor
	ClientPlayer ( Socket * pSocket ) throw ( ProtocolException , Error );
	
	// destructor
	~ClientPlayer () throw ( ProtocolException , Error );

public :

	// read socket's receive buffer and fill input buffer
	//virtual void processInput () throw ( IOException , Error );
	
	// parse packet and execute handler for the packet
	virtual void processCommand () throw ( IOException , Error );
	
	// flush output buffer to socket's send buffer
	//virtual void processOutput () throw ( IOException , Error );
	
	// send packet to player's output buffer
	//virtual void sendPacket ( Packet * packet ) throw ( ProtocolException , Error );

	// disconnect
	// 정식 로그아웃의 경우 disconnect(UNDISCONNECTED)
	virtual void disconnect ( bool bDisconnected = DISCONNECTED ) throw ( ProtocolException , Error );
	
	// get debug std::string
	virtual std::string toString () const throw ( ProtocolException , Error );
	
public :

	// return recent N-th packet
	// 최근 전송된 N 번째 패킷을 리턴한다.
	Packet * getOldPacket ( uint prev = 0 ) throw ( OutOfBoundException , NoSuchElementException );

	// return recent packet which has packetID
	// 특정 ID를 가진 패킷 중 가장 최근의 패킷을 리턴한다.
	Packet * getOldPacket ( PacketID_t packetID ) throw ( NoSuchElementException );

	// get player's status
	PlayerStatus getPlayerStatus () const throw () { return m_PlayerStatus; }

	// set player's status
	void setPlayerStatus ( PlayerStatus playerStatus ) throw ();

	// get/set PC Type
	PCType getPCType () const throw () { return m_PCType; }
	void setPCType ( PCType pcType ) throw () { m_PCType = pcType; }

	// get/set PC Name
	std::string getPCName () const throw () { return m_PCName; }
	void setPCName ( const std::string & pcName ) throw () { m_PCName = pcName; }

	// get/set X,Y,Dir
	Coord_t getX () const throw () { return m_X; }
	void setX ( Coord_t x ) throw () { m_X = x; }

	Coord_t getY () const throw () { return m_Y; }
	void setY ( Coord_t y ) throw () { m_Y = y; }

	Dir_t getDir () const throw () { return m_Dir; }
	void setDir ( Dir_t dir ) throw () { m_Dir = dir; }

	void setXY ( Coord_t x , Coord_t y ) throw () { m_X = x; m_Y = y; }

	// 암호화 코드를 설정한다.
	void setEncryptCode() throw( Error );

private :

	// Selected PC Type, Name
	PCType m_PCType;
	std::string m_PCName;

	Coord_t m_X, m_Y, m_Dir;
	
	// previous packet queue
	std::deque<Packet *> m_PacketHistory;

	// player status
	PlayerStatus m_PlayerStatus;

};

#endif
