//////////////////////////////////////////////////////////////////////////////
// Filename    : CGExchangeList.cpp
// Written By  : Exchange System
// Description : Client requests listing list from server
//////////////////////////////////////////////////////////////////////////////

#include "CGExchangeList.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

#include "Player.h"

CGExchangeList::CGExchangeList()
{
	m_Page = 1;
	m_PageSize = 20;
	m_ItemClass = 0xFF;  // All classes
	m_ItemType = 0xFFFF;  // All types
	m_MinPrice = 0;
	m_MaxPrice = 0;
}

CGExchangeList::~CGExchangeList()
{
}

void CGExchangeList::read(SocketInputStream& iStream)
{
	__BEGIN_TRY

	iStream.read(m_Page);
	iStream.read(m_PageSize);
	iStream.read(m_ItemClass);
	iStream.read(m_ItemType);
	iStream.read(m_MinPrice);
	iStream.read(m_MaxPrice);

	__END_CATCH
}

void CGExchangeList::write(SocketOutputStream& oStream) const
{
	__BEGIN_TRY

	oStream.write(m_Page);
	oStream.write(m_PageSize);
	oStream.write(m_ItemClass);
	oStream.write(m_ItemType);
	oStream.write(m_MinPrice);
	oStream.write(m_MaxPrice);

	__END_CATCH
}

string CGExchangeList::toString() const
{
	StringStream msg;
	msg << "CGExchangeList("
		<< "Page:" << (int)m_Page
		<< ", PageSize:" << (int)m_PageSize
		<< ", ItemClass:" << (int)m_ItemClass
		<< ", ItemType:" << (int)m_ItemType
		<< ", MinPrice:" << (int)m_MinPrice
		<< ", MaxPrice:" << (int)m_MaxPrice
		<< ")";
	return msg.toString();
}

void CGExchangeList::execute(Player* pPlayer)
{
	__BEGIN_TRY

	// This is handled by the server's packet handler
	// The client doesn't execute this packet

	__END_CATCH
}
