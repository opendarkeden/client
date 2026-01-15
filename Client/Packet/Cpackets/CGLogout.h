//////////////////////////////////////////////////////////////////////
// 
// Filename    : CGLogout.h 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CG_LOGOUT_H__
#define __CG_LOGOUT_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class CGLogout;
//
// 클라이언트가 서버에게 로그아웃을 알리는 패킷이다.
// data field 가 없기 때문에 getSize()는 0 을 리턴하며, read(), write() 
// 메쏘드는 아무런 역할도 하지 않는다.
//
//////////////////////////////////////////////////////////////////////

class CGLogout : public Packet {

public :
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_CG_LOGOUT; }
	
	// get packet's body size
	PacketSize_t getPacketSize () const throw () { return 0; }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CGLogout"; }
		
		// get packet's debug std::string
		std::string toString () const throw () { return "CGLogout"; }
	#endif

};


//////////////////////////////////////////////////////////////////////
//
// class CGLogoutFactory;
//
// Factory for CGLogout
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGLogoutFactory : public PacketFactory {

public :
	
	// create packet
	Packet * createPacket () throw () { return new CGLogout(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CGLogout"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CG_LOGOUT; }

	// get packet's max body size
	PacketSize_t getPacketMaxSize () const throw () { return 0; }

};

#endif


//////////////////////////////////////////////////////////////////////
//
// class CGLogoutHandler;
//
//////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
	class CGLogoutHandler {

	public :

		// execute packet's handler
		static void execute ( CGLogout * pPacket , Player * player ) throw ( ProtocolException , Error );

	};
#endif

#endif
