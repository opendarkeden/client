//////////////////////////////////////////////////////////////////////////////
// Filename    : CGModifyGuildIntro.cpp 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGModifyGuildIntro.h"


void CGModifyGuildIntro::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

	BYTE szGuildIntro;

	iStream.read( m_GuildID );
	iStream.read( szGuildIntro );

	if ( szGuildIntro > 255 )
		throw InvalidProtocolException( "too long szGuildIntro length" );

	if ( szGuildIntro > 0 )
		iStream.read( m_GuildIntro, szGuildIntro );
	else
		m_GuildIntro = "";

	__END_CATCH
}

void CGModifyGuildIntro::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	BYTE szGuildIntro = m_GuildIntro.size();

	if ( szGuildIntro > 255 )
		throw InvalidProtocolException( "too long szGuildIntro length" );

	oStream.write( m_GuildID );
	oStream.write( szGuildIntro );

	if ( szGuildIntro > 0 )
		oStream.write( m_GuildIntro );

	__END_CATCH
}

void CGModifyGuildIntro::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
#ifndef __GAME_CLIENT__
	CGModifyGuildIntroHandler::execute(this , pPlayer);
#endif

	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
std::string CGModifyGuildIntro::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
    msg << "CGModifyGuildIntro("
		<< "GuildID:" << (int)m_GuildID
		<< "GuildIntro:" << m_GuildIntro
		<< ")";
	return msg.toString();

	__END_CATCH
}
#endif