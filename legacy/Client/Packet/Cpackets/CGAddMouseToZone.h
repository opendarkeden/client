//////////////////////////////////////////////////////////////////////
// 
// Filename    : CGAddMouseToZone.h 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CG_ADD_MOUSE_TO_ZONE_H__
#define __CG_ADD_MOUSE_TO_ZONE_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class CGAddMouseToZone;
//
//////////////////////////////////////////////////////////////////////

class CGAddMouseToZone : public Packet {
public :

	// constructor
	CGAddMouseToZone() throw();

	// destructor
	~CGAddMouseToZone() throw();

public :
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_CG_ADD_MOUSE_TO_ZONE; }
	
	// get packet's body size
	// *OPTIMIZATION HINT*
	// const static CGAddMouseToZonePacketSize 를 정의해서 리턴하라.
	PacketSize_t getPacketSize () const throw () { return szObjectID; }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CGAddMouseToZone"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif
	
public :

	// get / set ObjectID
	ObjectID_t getObjectID() throw () { return m_ObjectID; }
	void setObjectID( ObjectID_t ObjectID ) throw() { m_ObjectID = ObjectID; }

private :
	
	// ObjectID
	ObjectID_t m_ObjectID;

};


//////////////////////////////////////////////////////////////////////
//
// class CGAddMouseToZoneFactory;
//
// Factory for CGAddMouseToZone
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGAddMouseToZoneFactory : public PacketFactory {

public :
	
	// create packet
	Packet * createPacket () throw () { return new CGAddMouseToZone(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CGAddMouseToZone"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CG_ADD_MOUSE_TO_ZONE; }

	// get packet's max body size
	// *OPTIMIZATION HINT*
	// const static CGAddMouseToZonePacketSize 를 정의해서 리턴하라.
	PacketSize_t getPacketMaxSize () const throw () { return szObjectID; }

};

#endif

//////////////////////////////////////////////////////////////////////
//
// class CGAddMouseToZoneHandler;
//
//////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
	class CGAddMouseToZoneHandler {
		
	public :

		// execute packet's handler
		static void execute ( CGAddMouseToZone * pPacket , Player * player ) throw ( ProtocolException , Error );
	};

#endif
#endif
