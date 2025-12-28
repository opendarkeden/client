//--------------------------------------------------------------------------------
// 
// Filename    : GCAddBurrowingCreature.h 
// Written By  : crazydog 
// 
//--------------------------------------------------------------------------------

#ifndef __GC_ADD_BURROWING_CREATURE_H__
#define __GC_ADD_BURROWING_CREATURE_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"

//--------------------------------------------------------------------------------
//
// class GCAddBurrowingCreature;
//
// 시야 안에 BurrowingCreature 가 들어왔을 경우, 이 패킷에 BurrowingCreature 정보를 담아서 전송받는다.
//
//--------------------------------------------------------------------------------

class GCAddBurrowingCreature : public Packet {

public :
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_GC_ADD_BURROWING_CREATURE; }
	
	// get packet's body size
	// *OPTIMIZATION HINT*
	// const static GCAddBurrowingCreaturePacketSize 를 정의, 리턴하라.
	PacketSize_t getPacketSize () const throw () 
	{ 
		return szObjectID 
			+ szBYTE + m_Name.size() 
			+ szCoord + szCoord; 
	}

	#ifdef __DEBUG_OUTPUT__
		// get packet's name
		std::string getPacketName () const throw () { return "GCAddBurrowingCreature"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif	


public :

	// get/set object id
	ObjectID_t getObjectID () const throw () { return m_ObjectID; }
	void setObjectID ( ObjectID_t creatureID ) throw () { m_ObjectID = creatureID; }

	// get/set name
	std::string getName () const throw () { return m_Name; }
	void setName ( std::string name ) throw () { m_Name = name; }


	// get/set X
	Coord_t getX () const throw () { return m_X; }
	void setX ( Coord_t x ) throw () { m_X = x; }
	
	// get/set Y
	Coord_t getY () const throw () { return m_Y; }
	void setY ( Coord_t y ) throw () { m_Y = y; }



private :
	
    // 존 레벨에서 유니크한 아이디로 객체 구분을 위해서 사용한다.
    ObjectID_t m_ObjectID;

    // BurrowingCreature의 이름
    std::string m_Name;

   	Coord_t m_X;				// X 좌표
	Coord_t m_Y;				// Y 좌표
};


//////////////////////////////////////////////////////////////////////
//
// class GCAddBurrowingCreatureFactory;
//
// Factory for GCAddBurrowingCreature
//
//////////////////////////////////////////////////////////////////////

class GCAddBurrowingCreatureFactory : public PacketFactory {

public :
	
	// create packet
	Packet * createPacket () throw () { return new GCAddBurrowingCreature(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "GCAddBurrowingCreature"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_GC_ADD_BURROWING_CREATURE; }

	// get packet's body size
	// *OPTIMIZATION HINT*
	// const static GCAddBurrowingCreaturePacketSize 를 정의, 리턴하라.
	PacketSize_t getPacketMaxSize () const throw () 
	{
		return szObjectID 
			+ szBYTE + 20
			+ szCoord + szCoord; 
	}

};


//////////////////////////////////////////////////////////////////////
//
// class GCAddBurrowingCreatureHandler;
//
//////////////////////////////////////////////////////////////////////

class GCAddBurrowingCreatureHandler {

public :

	// execute packet's handler
	static void execute ( GCAddBurrowingCreature * pPacket , Player * pPlayer ) throw ( Error );

};

#endif
