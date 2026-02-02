//////////////////////////////////////////////////////////////////////////////
// Filename    : CGExchangeList.h
// Written By  : Exchange System
// Description : Client requests listing list from server
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_EXCHANGE_LIST_H__
#define __CG_EXCHANGE_LIST_H__

#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGExchangeList;
//////////////////////////////////////////////////////////////////////////////

class CGExchangeList : public Packet
{
public:
	CGExchangeList();
	virtual ~CGExchangeList();

	void read(SocketInputStream& iStream);
	void write(SocketOutputStream& oStream) const;
	void execute(Player* pPlayer);

	PacketSize_t getPacketSize() const { return szint * 4 + sizeof(BYTE) + sizeof(ushort); }
	PacketID_t getPacketID() const { return PACKET_CG_EXCHANGE_LIST; }
	string getPacketName() const { return "CGExchangeList"; }
	string toString() const;

	// Getters
	int getPage() const { return m_Page; }
	int getPageSize() const { return m_PageSize; }
	BYTE getItemClass() const { return m_ItemClass; }
	ushort getItemType() const { return m_ItemType; }
	int getMinPrice() const { return m_MinPrice; }
	int getMaxPrice() const { return m_MaxPrice; }

	// Setters
	void setPage(int page) { m_Page = page; }
	void setPageSize(int size) { m_PageSize = size; }
	void setItemClass(BYTE itemClass) { m_ItemClass = itemClass; }
	void setItemType(ushort itemType) { m_ItemType = itemType; }
	void setMinPrice(int minPrice) { m_MinPrice = minPrice; }
	void setMaxPrice(int maxPrice) { m_MaxPrice = maxPrice; }

private:
	int		m_Page;
	int		m_PageSize;
	BYTE	m_ItemClass;	// 0xFF = all
	ushort	m_ItemType;		// 0xFFFF = all
	int		m_MinPrice;
	int		m_MaxPrice;
};

//////////////////////////////////////////////////////////////////////////////
// class CGExchangeListFactory;
//////////////////////////////////////////////////////////////////////////////

class CGExchangeListFactory : public PacketFactory
{
public:
	Packet* createPacket() throw() { return new CGExchangeList(); }
	string getPacketName() const throw() { return "CGExchangeList"; }
	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_EXCHANGE_LIST; }
	PacketSize_t getPacketMaxSize() const throw() { return 4 + 4 + 1 + 2 + 4 + 4; }
};

#endif
