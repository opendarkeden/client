//////////////////////////////////////////////////////////////////////////////
// Filename    : GCExchangeList.cpp
// Written By  : Exchange System
// Description : Server sends listing list to client
//////////////////////////////////////////////////////////////////////////////

#include "GCExchangeList.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

#include "Player.h"

GCExchangeList::GCExchangeList()
{
	m_Page = 1;
	m_PageSize = 20;
	m_Total = 0;
	m_pListings = NULL;
}

GCExchangeList::~GCExchangeList()
{
}

void GCExchangeList::read(SocketInputStream & iStream)
{
	__BEGIN_TRY

	iStream.read(m_Page);
	iStream.read(m_PageSize);
	iStream.read(m_Total);

	// Note: We're not reading the actual listings data here yet
	// The UI will request detailed data separately if needed
	// For now, we just update the page/total counts

	__END_CATCH
}

void GCExchangeList::write(SocketOutputStream & oStream) const
{
	__BEGIN_TRY

	oStream.write(m_Page);
	oStream.write(m_PageSize);
	oStream.write(m_Total);

	// Write listing count (placeholder)
	uint16_t count = 0;
	oStream.write(count);

	__END_CATCH
}

PacketSize_t GCExchangeList::getPacketSize() const throw()
{
	return szBYTE + szBYTE + szBYTE + szWORD;  // Basic size for now
}

void GCExchangeList::execute(Player* pPlayer)
{
	__BEGIN_TRY

	// Client-side packet handling
	// This will be implemented to update the UI

	__END_CATCH
}
