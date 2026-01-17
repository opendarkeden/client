//////////////////////////////////////////////////////////////////////////////
// Filename    : CGTryJoinGuild.cpp 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGTryJoinGuild.h"


void CGTryJoinGuild::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	iStream.read( m_GuildID );
	iStream.read( m_GuildMemberRank );

	__END_CATCH
}

void CGTryJoinGuild::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	oStream.write( m_GuildID );
	oStream.write( m_GuildMemberRank );

	__END_CATCH
}

void CGTryJoinGuild::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
#ifndef __GAME_CLIENT__		
	CGTryJoinGuildHandler::execute(this , pPlayer);
#endif
	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
std::string CGTryJoinGuild::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
    msg << "CGTryJoinGuild("
		<< "GuildID:" << m_GuildID
		<< "GuildMemberRank:" << m_GuildMemberRank
		<< ")" ;
	return msg.toString();

	__END_CATCH
}
#endif
