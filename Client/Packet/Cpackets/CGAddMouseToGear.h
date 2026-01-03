//////////////////////////////////////////////////////////////////////
// 
// Filename    : CGAddMouseToGear.h 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CG_ADD_MOUSE_TO_GEAR_H__
#define __CG_ADD_MOUSE_TO_GEAR_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class CGAddMouseToGear;
//
//////////////////////////////////////////////////////////////////////

class CGAddMouseToGear : public Packet {
public :

	// constructor
	CGAddMouseToGear() throw();

	// destructor
	~CGAddMouseToGear() throw();

public :
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_CG_ADD_MOUSE_TO_GEAR; }
	
	// get packet's body size
	// *OPTIMIZATION HINT*
	// const static CGAddMouseToGearPacketSize 를 정의해서 리턴하라.
	PacketSize_t getPacketSize () const throw () { return szObjectID + szSlotID; }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CGAddMouseToGear"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif
	
public :

	// get / set ObjectID
	ObjectID_t getObjectID() throw () { return m_ObjectID; }
	void setObjectID( ObjectID_t ObjectID ) throw() { m_ObjectID = ObjectID; }

	SlotID_t getSlotID() const throw() { return m_SlotID; }
	void setSlotID( SlotID_t SlotID ) throw() { m_SlotID = SlotID; }

private :
	
	// ObjectID
	ObjectID_t m_ObjectID;

	// SlotID
	SlotID_t m_SlotID;

};


//////////////////////////////////////////////////////////////////////
//
// class CGAddMouseToGearFactory;
//
// Factory for CGAddMouseToGear
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGAddMouseToGearFactory : public PacketFactory {

public :
	
	// create packet
	Packet * createPacket () throw () { return new CGAddMouseToGear(); }

	
	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CGAddMouseToGear"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CG_ADD_MOUSE_TO_GEAR; }

	// get packet's max body size
	// *OPTIMIZATION HINT*
	// const static CGAddMouseToGearPacketSize 를 정의해서 리턴하라.
	PacketSize_t getPacketMaxSize () const throw () { return szObjectID + szSlotID; }

};

#endif

//////////////////////////////////////////////////////////////////////
//
// class CGAddMouseToGearHandler;
//
//////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
	class CGAddMouseToGearHandler {
		
	public :

		// execute packet's handler
		static void execute ( CGAddMouseToGear * pPacket , Player * player ) throw ( ProtocolException , Error );
	};

#endif
#endif
