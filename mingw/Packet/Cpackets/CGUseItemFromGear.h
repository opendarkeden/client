//////////////////////////////////////////////////////////////////////////////
// Filename    : CGUseItemFromGear.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_USE_ITEM_FROM_GEAR_H__
#define __CG_USE_ITEM_FROM_GEAR_H__

#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGUseItemFromGear;
//////////////////////////////////////////////////////////////////////////////

class CGUseItemFromGear : public Packet 
{
public:
    void read(SocketInputStream & iStream) throw(ProtocolException, Error);
    void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_CG_USE_ITEM_FROM_GEAR; }
	PacketSize_t getPacketSize() const throw() { return szObjectID + szBYTE; }

#ifdef __DEBUG_OUTPUT__
	std::string getPacketName() const throw() { return "CGUseItemFromGear"; }
	std::string toString() const throw();
#endif
	
public:
	ObjectID_t getObjectID() const throw() { return m_ObjectID; }
	void setObjectID(ObjectID_t ObjectID) throw() { m_ObjectID = ObjectID; }

	BYTE getPart() const throw() { return m_Part; }
	void setPart( BYTE part ) throw() { m_Part = part; }

private:
	ObjectID_t   m_ObjectID; // 아이템의 object id 
	BYTE		 m_Part;	 // 아이템이 있는 slot 
};

#ifdef __DEBUG_OUTPUT__
//////////////////////////////////////////////////////////////////////////////
// class CGUseItemFromGearFactory;
//////////////////////////////////////////////////////////////////////////////

class CGUseItemFromGearFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new CGUseItemFromGear(); }
	std::string getPacketName() const throw() { return "CGUseItemFromGear"; }
	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_USE_ITEM_FROM_GEAR; }
	PacketSize_t getPacketMaxSize() const throw() { return szObjectID + szBYTE; }
};
#endif

#ifndef __GAME_CLIENT__
//////////////////////////////////////////////////////////////////////////////
// class CGUseItemFromGearHandler;
//////////////////////////////////////////////////////////////////////////////

class Item;

class CGUseItemFromGearHandler 
{
public:
	static void execute(CGUseItemFromGear* pPacket, Player* player) throw(ProtocolException, Error);

protected:
	static void executeCoupleRing(CGUseItemFromGear* pPacket, Player* player) throw(ProtocolException, Error);
};
#endif

#endif
