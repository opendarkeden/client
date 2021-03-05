//////////////////////////////////////////////////////////////////////////////
// Filename    : CGUseItemFromInventory.h 
// Written By  : excel96
// Description : 
// 인벤토리 안의 아이템을 사용할 때, 클라이언트가 X, Y 및 ObjectID를
// 보내면 아이템 클래스에 따라서, 서버가 이에 맞는 코드를 처리한다.
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_USE_ITEM_FROM_INVENTORY_H__
#define __CG_USE_ITEM_FROM_INVENTORY_H__

#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGUseItemFromInventory;
//////////////////////////////////////////////////////////////////////////////

class CGUseItemFromInventory : public Packet 
{
public:
	CGUseItemFromInventory () throw ();
	~CGUseItemFromInventory () throw ();

    void read(SocketInputStream & iStream) throw(ProtocolException, Error);
    void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_CG_USE_ITEM_FROM_INVENTORY; }

	//modify by viva for notice
	//PacketSize_t getPacketSize() const throw() { return szObjectID + szObjectID + szCoordInven + szCoordInven; }
	PacketSize_t getPacketSize() const throw() { return szObjectID + szCoordInven + szCoordInven; }
	//end

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName() const throw() { return "CGUseItemFromInventory"; }
		std::string toString() const throw();
	#endif	
	
public:
	ObjectID_t getObjectID() const throw() { return m_ObjectID; }
	void setObjectID(ObjectID_t ObjectID) throw() { m_ObjectID = ObjectID; }

	ObjectID_t getInventoryItemObjectID() throw() { return m_InventoryItemObjectID; }
	void setInventoryItemObjectID(ObjectID_t InventoryItemObjectID) throw() { m_InventoryItemObjectID = InventoryItemObjectID; }
	
	CoordInven_t getX() const throw() { return m_InvenX; }
	void setX(CoordInven_t InvenX) throw() { m_InvenX = InvenX; }

	CoordInven_t getY() const throw() { return m_InvenY; }
	void setY(CoordInven_t InvenY) throw() { m_InvenY = InvenY; }

private:
	ObjectID_t   m_ObjectID; // 아이템의 object id 
	// 보조 인벤토리 아이템의 오브젝트 아이디. 0이면 메인 인벤토리에서 사용
	ObjectID_t	 m_InventoryItemObjectID;
	CoordInven_t m_InvenX;   // 아이템의 인벤토리 좌표 X
	CoordInven_t m_InvenY;   // 아이템의 인벤토리 좌표 Y
};


//////////////////////////////////////////////////////////////////////////////
// class CGUseItemFromInventoryFactory;
//////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGUseItemFromInventoryFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new CGUseItemFromInventory(); }
	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName() const throw() { return "CGUseItemFromInventory"; }
	#endif
	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_USE_ITEM_FROM_INVENTORY; }
	
	//modify by viva
	//PacketSize_t getPacketMaxSize() const throw() { return szObjectID + szObjectID + szCoordInven + szCoordInven; }
	PacketSize_t getPacketMaxSize() const throw() { return szObjectID + szCoordInven + szCoordInven; }
	//end
};
#endif

//////////////////////////////////////////////////////////////////////////////
// class CGUseItemFromInventoryHandler;
//////////////////////////////////////////////////////////////////////////////

class Inventory;
class Item;

#ifndef __GAME_CLIENT__
	class CGUseItemFromInventoryHandler 
	{
	public:
		static void execute(CGUseItemFromInventory* pPacket, Player* player) throw(ProtocolException, Error);

	protected:
		static void executePotion(CGUseItemFromInventory* pPacket, Player* player) throw(ProtocolException, Error);
		static void executeMagazine(CGUseItemFromInventory* pPacket, Player* player) throw(ProtocolException, Error);
		static void executeETC(CGUseItemFromInventory* pPacket, Player* player) throw(ProtocolException, Error);
		static void executeSerum(CGUseItemFromInventory* pPacket, Player* player) throw(ProtocolException, Error);
		static void executeVampireETC(CGUseItemFromInventory* pPacket, Player* player) throw(ProtocolException, Error);
	};
#endif

#endif
