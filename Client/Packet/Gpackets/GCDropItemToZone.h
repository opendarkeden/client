//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCDropItemToZone.h 
// Written By  : elca
// Description : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __GC_DROP_ITEM_TO_ZONE_H__
#define __GC_DROP_ITEM_TO_ZONE_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"
#include "SubItemInfo.h"
#include "GCAddItemToZone.h"


//////////////////////////////////////////////////////////////////////
//
// class GCDropItemToZone;
//
////////////////////////////////////////////////////////////////////

class GCDropItemToZone : public GCAddItemToZone {

public :

	GCDropItemToZone() throw();
	~GCDropItemToZone() throw();
	
	PacketSize_t getPacketSize() const throw() { return GCAddItemToZone::getPacketSize() + szObjectID; }

    void read(SocketInputStream & iStream) throw(ProtocolException, Error);
    void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);

	// execute packet's handler
	void execute(Player* pPlayer) throw(ProtocolException, Error);

	// get packet id
	PacketID_t getPacketID() const throw() { return PACKET_GC_DROP_ITEM_TO_ZONE; }
	
#ifdef __DEBUG_OUTPUT__
	// get packet's name
	std::string getPacketName() const throw() { return "GCDropItemToZone"; }

	// get packet's debug string
	std::string toString() const throw();
#endif

public:
	ObjectID_t getDropPetOID() const { return m_DropPetOID; }
	void setDropPetOID( ObjectID_t PetOID ) { m_DropPetOID = PetOID; }

private:
	ObjectID_t	m_DropPetOID;
};


//////////////////////////////////////////////////////////////////////
//
// class GCDropItemToZoneFactory;
//
// Factory for GCDropItemToZone
//
//////////////////////////////////////////////////////////////////////

class GCDropItemToZoneFactory : public PacketFactory {

public :
	
	// create packet
	Packet* createPacket() throw() { return new GCDropItemToZone(); }

#ifdef __DEBUG_OUTPUT__
	// get packet name
	std::string getPacketName() const throw() { return "GCDropItemToZone"; }
#endif
	
	// get packet id
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_DROP_ITEM_TO_ZONE; }

	// get packet's body size
	// *OPTIMIZATION HINT*
	// const static GCDropItemToZonePacketSize 를 정의, 리턴하라.
	PacketSize_t getPacketMaxSize() const throw() { return szObjectID + szCoord + szCoord + szBYTE + szItemType + szBYTE + 255 + szDurability + szItemNum + szBYTE +(szObjectID + szBYTE + szItemType + szItemNum + szSlotID)* 12 + szObjectID; }

};


//////////////////////////////////////////////////////////////////////
//
// class GCDropItemToZoneHandler;
//
//////////////////////////////////////////////////////////////////////

class GCDropItemToZoneHandler {

public :

	// execute packet's handler
	static void execute(GCDropItemToZone* pPacket, Player* pPlayer) throw ( ProtocolException , Error );

};

#endif
