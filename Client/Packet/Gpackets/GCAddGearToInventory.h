//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCAddGearToInventory.h 
// Written By  : crazydog
// Description : Skill ShieldStrike에 의해 발생. 
// 				shield를 장착창에서 inventory로 보낸다.
// 
//////////////////////////////////////////////////////////////////////

#ifndef __GC_ADD_GEAR_TO_INVENTORY_H__
#define __GC_ADD_GEAR_TO_INVENTORY_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"


//////////////////////////////////////////////////////////////////////
//
// class GCAddGearToInventory;
//
//////////////////////////////////////////////////////////////////////

class GCAddGearToInventory : public Packet {
public :

	// constructor
	GCAddGearToInventory() throw();

	// destructor
	~GCAddGearToInventory() throw();

public :
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_GC_ADD_GEAR_TO_INVENTORY; }
	
	// get packet's body size
	// *OPTIMIZATION HINT*
	// const static GCAddGearToInventoryPacketSize 를 정의해서 리턴하라.
	PacketSize_t getPacketSize () const throw () { return szSlotID + szCoordInven + szCoordInven; }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "GCAddGearToInventory"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif
	
public :

	// get / set ObjectID
	SlotID_t getSlotID() throw () { return m_SlotID; }
	void setSlotID( SlotID_t SlotID ) throw() { m_SlotID = SlotID; }

	// get / set Ivnentory X, Y Coordicate
	CoordInven_t getInvenX() const throw () { return m_InvenX; }
	void setInvenX( CoordInven_t InvenX ) throw () { m_InvenX = InvenX; }

	// get / set Inventory Y
	CoordInven_t getInvenY() const throw () { return m_InvenY; }
	void setInvenY( CoordInven_t InvenY ) { m_InvenY = InvenY; }

private :
	
	// wear part ID
	SlotID_t m_SlotID;

	// Inventory의 X, Y 좌표.
	CoordInven_t m_InvenX;
	CoordInven_t m_InvenY;

};


//////////////////////////////////////////////////////////////////////
//
// class GCAddGearToInventoryFactory;
//
// Factory for GCAddGearToInventory
//
//////////////////////////////////////////////////////////////////////

class GCAddGearToInventoryFactory : public PacketFactory {

public :
	
	// create packet
	Packet * createPacket () throw () { return new GCAddGearToInventory(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "GCAddGearToInventory"; }
	#endif	
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_GC_ADD_GEAR_TO_INVENTORY; }

	// get packet's max body size
	// *OPTIMIZATION HINT*
	// const static GCAddGearToInventoryPacketSize 를 정의해서 리턴하라.
	PacketSize_t getPacketMaxSize () const throw () { return szSlotID + szCoordInven + szCoordInven; }

};

//////////////////////////////////////////////////////////////////////
//
// class GCAddGearToInventoryHandler;
//
//////////////////////////////////////////////////////////////////////

class GCAddGearToInventoryHandler {
	
public :

	// execute packet's handler
	static void execute ( GCAddGearToInventory * pPacket , Player * player ) throw ( ProtocolException , Error );
};


#endif
