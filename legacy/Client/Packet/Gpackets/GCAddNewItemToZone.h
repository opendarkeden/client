//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCAddNewItemToZone.h 
// Written By  : elca
// Description : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __GC_ADD_NEW_ITEM_TO_ZONE_H__
#define __GC_ADD_NEW_ITEM_TO_ZONE_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"
#include "SubItemInfo.h"
#include "GCAddItemToZone.h"


//////////////////////////////////////////////////////////////////////
//
// class GCAddNewItemToZone;
//
////////////////////////////////////////////////////////////////////

class GCAddNewItemToZone : public GCAddItemToZone {

public :

	GCAddNewItemToZone() throw();
	~GCAddNewItemToZone() throw();
	
	// execute packet's handler
	void execute(Player* pPlayer) throw(ProtocolException, Error);

	// get packet id
	PacketID_t getPacketID() const throw() { return PACKET_GC_ADD_NEW_ITEM_TO_ZONE; }
	
#ifdef __DEBUG_OUTPUT__	
	// get packet's name
	std::string getPacketName() const throw() { return "GCAddNewItemToZone"; }

	// get packet's debug std::string
	std::string toString() const throw();
#endif	
};


//////////////////////////////////////////////////////////////////////
//
// class GCAddNewItemToZoneFactory;
//
// Factory for GCAddNewItemToZone
//
//////////////////////////////////////////////////////////////////////

class GCAddNewItemToZoneFactory : public PacketFactory {

public :
	
	// create packet
	Packet* createPacket() throw() { return new GCAddNewItemToZone(); }

	// get packet name
	std::string getPacketName() const throw() { return "GCAddNewItemToZone"; }
	
	// get packet id
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_ADD_NEW_ITEM_TO_ZONE; }

	// get packet's body size
	// *OPTIMIZATION HINT*
	// const static GCAddNewItemToZonePacketSize 를 정의, 리턴하라.
	PacketSize_t getPacketMaxSize() const throw() { return szObjectID + szCoord + szCoord + szBYTE + szItemType + szBYTE + 255 + szDurability + szItemNum + szBYTE +(szObjectID + szBYTE + szItemType + szItemNum + szSlotID)* 12; }

};


//////////////////////////////////////////////////////////////////////
//
// class GCAddNewItemToZoneHandler;
//
//////////////////////////////////////////////////////////////////////

class GCAddNewItemToZoneHandler {

public :

	// execute packet's handler
	static void execute(GCAddNewItemToZone* pPacket, Player* pPlayer) throw(Error);

};

#endif
