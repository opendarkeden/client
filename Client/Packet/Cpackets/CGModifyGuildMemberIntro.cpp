//////////////////////////////////////////////////////////////////////////////
// Filename    : CGModifyGuildMemberIntro.cpp 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGModifyGuildMemberIntro.h"


void CGModifyGuildMemberIntro::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

	BYTE szGuildMemberIntro;

	iStream.read( m_GuildID );
	iStream.read( szGuildMemberIntro );

	if ( szGuildMemberIntro > 255 )
		throw InvalidProtocolException( "too long szGuildMemberIntro length" );

	if ( szGuildMemberIntro > 0 )
		iStream.read( m_GuildMemberIntro, szGuildMemberIntro );
	else
		m_GuildMemberIntro = "";

	__END_CATCH
}

void CGModifyGuildMemberIntro::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	BYTE szGuildMemberIntro = m_GuildMemberIntro.size();

	if ( szGuildMemberIntro > 255 )
		throw InvalidProtocolException( "too long szGuildMemberIntro length" );

	oStream.write( m_GuildID );
	oStream.write( szGuildMemberIntro );

	if ( szGuildMemberIntro > 0 )
		oStream.write( m_GuildMemberIntro );

	__END_CATCH
}

void CGModifyGuildMemberIntro::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
#ifndef __GAME_CLIENT__
	CGModifyGuildMemberIntroHandler::execute(this , pPlayer);
#endif
	
	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
std::string CGModifyGuildMemberIntro::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
    msg << "CGModifyGuildMemberIntro("
		<< "GuildID:" << (int)m_GuildID
		<< "GuildMemberIntro:" << m_GuildMemberIntro
		<< ")";
	return msg.toString();

	__END_CATCH
}
#endif