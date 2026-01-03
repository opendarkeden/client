//////////////////////////////////////////////////////////////////////////////
// Filename    : CGUseMessageItemFromInventory.h 
// Written By  : excel96
// Description : 
// 인벤토리 안의 아이템을 사용할 때, 클라이언트가 X, Y 및 ObjectID를
// 보내면 아이템 클래스에 따라서, 서버가 이에 맞는 코드를 처리한다.
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_USE_MESSAGE_ITEM_FROM_INVENTORY_H__
#define __CG_USE_MESSAGE_ITEM_FROM_INVENTORY_H__

#include "Packet.h"
#include "PacketFactory.h"
#include "CGUseItemFromInventory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGUseMessageItemFromInventory;
//////////////////////////////////////////////////////////////////////////////

class CGUseMessageItemFromInventory : public CGUseItemFromInventory 
{
public:
#ifdef __DEBUG_OUTPUT__
    void read(SocketInputStream & iStream) throw(ProtocolException, Error);
#endif

    void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_CG_USE_MESSAGE_ITEM_FROM_INVENTORY; }
	PacketSize_t getPacketSize() const throw() 
	{ 
		return CGUseItemFromInventory::getPacketSize() 
				+ szBYTE + m_Message.size(); 
	}

#ifdef __DEBUG_OUTPUT__
	std::string getPacketName() const throw() { return "CGUseMessageItemFromInventory"; }
	std::string toString() const throw();
#endif
	
public:
	const std::string& getMessage() const throw() { return m_Message; }
	void setMessage(const std::string& msg) throw() { m_Message = msg; }

private:
	std::string m_Message;
};


//////////////////////////////////////////////////////////////////////////////
// class CGUseMessageItemFromInventoryFactory;
//////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGUseMessageItemFromInventoryFactory : public CGUseItemFromInventoryFactory 
{
public:
	Packet* createPacket() throw() { return new CGUseMessageItemFromInventory(); }
	std::string getPacketName() const throw() { return "CGUseMessageItemFromInventory"; }
	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_USE_MESSAGE_ITEM_FROM_INVENTORY; }
	PacketSize_t getPacketMaxSize() const throw() 
	{ 
		return CGUseItemFromInventoryFactory::getPacketMaxSize() 
				+ szBYTE + 128; 
	}
};
#endif

//////////////////////////////////////////////////////////////////////////////
// class CGUseMessageItemFromInventoryHandler;
//////////////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
class CGUseMessageItemFromInventoryHandler 
{
public:
	static void execute(CGUseMessageItemFromInventory* pPacket, Player* player) throw(ProtocolException, Error);

protected:
	static void executeEventTree(CGUseMessageItemFromInventory* pPacket, Player* player) throw(ProtocolException, Error);
};
#endif
#endif
