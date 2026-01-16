//////////////////////////////////////////////////////////////////////////////
// Filename    : CGDenyUnion.cpp 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGDenyUnion.h"


void CGDenyUnion::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	iStream.read( m_GuildID );

	__END_CATCH
}

void CGDenyUnion::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	oStream.write( m_GuildID );

	__END_CATCH
}

void CGDenyUnion::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
#ifndef __GAME_CLIENT__			
	CGDenyUnionHandler::execute(this , pPlayer);
#endif
	__END_CATCH
}
#ifdef __DEBUG_OUTPUT__
string CGDenyUnion::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
    msg << "CGDenyUnion("
		<< "GuildID:" << m_GuildID
		<< ")" ;
	return msg.toString();

	__END_CATCH
}
#endif