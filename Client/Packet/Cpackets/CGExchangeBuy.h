//////////////////////////////////////////////////////////////////////////////
// Filename    : CGExchangeBuy.h
// Written By  : Exchange System
// Description : Client buys an item from the exchange
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_EXCHANGE_BUY_H__
#define __CG_EXCHANGE_BUY_H__

#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGExchangeBuy;
//////////////////////////////////////////////////////////////////////////////

class CGExchangeBuy : public Packet
{
public:
	CGExchangeBuy();
	virtual ~CGExchangeBuy();

	void read(SocketInputStream& iStream);
	void write(SocketOutputStream& oStream) const;
	void execute(Player* pPlayer);

	PacketSize_t getPacketSize() const { return szint; }  // listingID
	PacketID_t getPacketID() const { return PACKET_CG_EXCHANGE_BUY; }
	string getPacketName() const { return "CGExchangeBuy"; }
	string toString() const;

	// Getters
	uint getListingID() const { return m_ListingID; }

	// Setters
	void setListingID(uint listingID) { m_ListingID = listingID; }

private:
	uint	m_ListingID;
};

//////////////////////////////////////////////////////////////////////////////
// class CGExchangeBuyFactory;
//////////////////////////////////////////////////////////////////////////////

class CGExchangeBuyFactory : public PacketFactory
{
public:
	Packet* createPacket() throw() { return new CGExchangeBuy(); }
	string getPacketName() const throw() { return "CGExchangeBuy"; }
	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_EXCHANGE_BUY; }
	PacketSize_t getPacketMaxSize() const throw() { return szint; }
};

#endif
