//////////////////////////////////////////////////////////////////////////////
// Filename    : CGExchangeBuy.cpp
// Written By  : Exchange System
// Description : Client buys an item from the exchange
//////////////////////////////////////////////////////////////////////////////

#include "CGExchangeBuy.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

#include "Player.h"

CGExchangeBuy::CGExchangeBuy()
{
	m_ListingID = 0;
}

CGExchangeBuy::~CGExchangeBuy()
{
}

void CGExchangeBuy::read(SocketInputStream& iStream)
{
	__BEGIN_TRY

	iStream.read(m_ListingID);

	__END_CATCH
}

void CGExchangeBuy::write(SocketOutputStream& oStream) const
{
	__BEGIN_TRY

	oStream.write(m_ListingID);

	__END_CATCH
}

string CGExchangeBuy::toString() const
{
	StringStream msg;
	msg << "CGExchangeBuy(ListingID:" << (int)m_ListingID << ")";
	return msg.toString();
}

void CGExchangeBuy::execute(Player* pPlayer)
{
	__BEGIN_TRY

	// This is handled by the server's packet handler
	// The client doesn't execute this packet

	__END_CATCH
}
