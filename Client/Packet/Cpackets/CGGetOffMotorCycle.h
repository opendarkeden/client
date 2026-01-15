//////////////////////////////////////////////////////////////////////
// 
// Filename    : CGGetOffMotorCycle.h 
// Written By  : elca@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CG_GET_OFF_MOTORCYCLE_H__
#define __CG_GET_OFF_MOTORCYCLE_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class CGGetOffMotorCycle;
//
//////////////////////////////////////////////////////////////////////

class CGGetOffMotorCycle : public Packet {

public :
	
	// constructor
	CGGetOffMotorCycle () throw ();
	
	// destructor
	~CGGetOffMotorCycle () throw ();

	
public :
	
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_CG_GET_OFF_MOTORCYCLE; }
	
	// get packet's body size
	PacketSize_t getPacketSize () const throw () { return szObjectID; }

	// get/set ObjectID
	ObjectID_t getObjectID() const throw()  { return m_ObjectID; }
	void setObjectID( ObjectID_t ObjectID ) throw() { m_ObjectID = ObjectID; }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CGGetOffMotorCycle"; }

		// get packet's debug std::string
		std::string toString () const throw ();
	#endif

private :

	// ObjectID
	ObjectID_t m_ObjectID;
};


//////////////////////////////////////////////////////////////////////
//
// class CGGetOffMotorCycleFactory;
//
// Factory for CGGetOffMotorCycle
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGGetOffMotorCycleFactory : public PacketFactory {

public :
	
	// constructor
	CGGetOffMotorCycleFactory () throw () {}
	
	// destructor
	virtual ~CGGetOffMotorCycleFactory () throw () {}

	
public :
	
	// create packet
	Packet * createPacket () throw () { return new CGGetOffMotorCycle(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CGGetOffMotorCycle"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CG_GET_OFF_MOTORCYCLE; }

	// get Packet Max Size
	PacketSize_t getPacketMaxSize() const throw() { return szObjectID; }

};

#endif


//////////////////////////////////////////////////////////////////////
//
// class CGGetOffMotorCycleHandler;
//
//////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
	class CGGetOffMotorCycleHandler {

	public :

		// execute packet's handler
		static void execute ( CGGetOffMotorCycle * pCGGetOffMotorCycle , Player * pPlayer ) throw ( Error );

	};
#endif

#endif
