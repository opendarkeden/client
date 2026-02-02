//////////////////////////////////////////////////////////////////////////////
// Filename    : GCExchangeList.h
// Written By  : Exchange System
// Description : Server sends listing list to client
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_EXCHANGE_LIST_H__
#define __GC_EXCHANGE_LIST_H__

#include "Packet.h"
#include "PacketFactory.h"

// Forward declarations
struct ExchangeListing;

//-----------------------------------------------------------------------------
// GCExchangeList
//-----------------------------------------------------------------------------

class GCExchangeList : public Packet
{
public:
	GCExchangeList();
	virtual ~GCExchangeList();

	void read(SocketInputStream & iStream);
	void write(SocketOutputStream & oStream) const;
	void execute(Player* pPlayer);

	PacketSize_t getPacketSize() const throw();
	PacketID_t getPacketID() const throw() { return PACKET_GC_EXCHANGE_LIST; }
	string getPacketName() const throw() { return "GCExchangeList"; }

	// Getters
	int getPage() const { return m_Page; }
	int getPageSize() const { return m_PageSize; }
	int getTotal() const { return m_Total; }

	// For UI to access listing data
	void setListings(void* listings) { m_pListings = listings; }
	void* getListings() const { return m_pListings; }

private:
	int     m_Page;
	int     m_PageSize;
	int     m_Total;
	void*   m_pListings;  // Pointer to listings data
};

//-----------------------------------------------------------------------------
// GCExchangeListFactory
//-----------------------------------------------------------------------------

class GCExchangeListFactory : public PacketFactory
{
public:
	Packet* createPacket() throw() { return new GCExchangeList(); }
	string getPacketName() const throw() { return "GCExchangeList"; }
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_EXCHANGE_LIST; }
	PacketSize_t getPacketMaxSize() const throw() { return 2048; }
};

#endif // __GC_EXCHANGE_LIST_H__
