//////////////////////////////////////////////////////////////////////////////
// Filename    : CGQuitUnion.cpp 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGQuitUnion.h"


void CGQuitUnion::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

	iStream.read ( m_GuildID );
	iStream.read ( m_Method );
		
	__END_CATCH
}

void CGQuitUnion::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	oStream.write ( m_GuildID );
	oStream.write ( m_Method );
	
	__END_CATCH
}

void CGQuitUnion::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
#ifndef __GAME_CLIENT__		
	CGQuitUnionHandler::execute(this , pPlayer);
#endif
	__END_CATCH
}
#ifdef __DEBUG_OUTPUT__
string CGQuitUnion::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
    msg << "CGQuitUnion("
		<< "GuildID:" << m_GuildID
		<< "Method:" << m_Method
		<< ")" ;
	return msg.toString();

	__END_CATCH
}
#endif