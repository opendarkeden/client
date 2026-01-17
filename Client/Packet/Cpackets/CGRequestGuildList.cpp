//////////////////////////////////////////////////////////////////////////////
// Filename    : CGRequestGuildList.cpp 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGRequestGuildList.h"


void CGRequestGuildList::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
	
	iStream.read( m_GuildType );
		
	__END_CATCH
}

void CGRequestGuildList::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY
	
	oStream.write( m_GuildType );
		
	__END_CATCH
}

void CGRequestGuildList::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
#ifndef __GAME_CLIENT__		
	CGRequestGuildListHandler::execute(this , pPlayer);
#endif
	__END_CATCH
}
#ifdef __DEBUG_OUTPUT__
string CGRequestGuildList::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
    msg << "CGRequestGuildList("
		<< "GuildType : " << m_GuildType
		<< ")" ;
	return msg.toString();

	__END_CATCH
}
#endif