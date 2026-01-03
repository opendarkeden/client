//////////////////////////////////////////////////////////////////////
// 
// Filename    : CGVisible.h 
// Written By  : crazydog
// Description : 박쥐나 늑대에서 원래모습으로 돌아오고싶을때..
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CG_VISIBLE_H__
#define __CG_VISIBLE_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"


//////////////////////////////////////////////////////////////////////
//
// class CGVisible;
//
//////////////////////////////////////////////////////////////////////

class CGVisible : public Packet {

public :
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_CG_VISIBLE; }
	
	// get packet's body size
	// *OPTIMIZATION HINT*
	// const static CGVisiblePacketSize 를 정의해서 리턴하라.
	PacketSize_t getPacketSize () const throw () { return 0; }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CGVisible"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif

};


//////////////////////////////////////////////////////////////////////
//
// class CGVisibleFactory;
//
// Factory for CGVisible
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGVisibleFactory : public PacketFactory {

public :
	
	// create packet
	Packet * createPacket () throw () { return new CGVisible(); }

	// get packet name
	std::string getPacketName () const throw () { return "CGVisible"; }
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CG_VISIBLE; }

	// get packet's max body size
	// *OPTIMIZATION HINT*
	// const static CGVisiblePacketSize 를 정의해서 리턴하라.
	PacketSize_t getPacketMaxSize () const throw () { return 0; }

};

#endif


//////////////////////////////////////////////////////////////////////
//
// class CGVisibleHandler;
//
//////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
	class CGVisibleHandler {
		
	public :

		// execute packet's handler
		static void execute ( CGVisible * pPacket , Player * player ) throw ( ProtocolException , Error );
	};
#endif

#endif