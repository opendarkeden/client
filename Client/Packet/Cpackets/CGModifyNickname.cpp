//////////////////////////////////////////////////////////////////////////////
// Filename    : CGModifyNickname.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGModifyNickname.h"

CGModifyNickname::CGModifyNickname () 
     throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

CGModifyNickname::~CGModifyNickname () 
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

void CGModifyNickname::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

	iStream.read( m_NicknameID );

	BYTE szSTR;
	iStream.read( szSTR );
	iStream.read( m_Nickname, szSTR );

	__END_CATCH
}

void CGModifyNickname::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	oStream.write( m_NicknameID );

	BYTE szSTR;
	szSTR = m_Nickname.size();
	oStream.write( szSTR );
	oStream.write( m_Nickname );

	__END_CATCH
}

void CGModifyNickname::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
#ifndef __GAME_CLIENT__
	__BEGIN_TRY

	CGModifyNicknameHandler::execute (this , pPlayer);
		
	__END_CATCH
#endif
}

#ifdef __DEBUG_OUTPUT__
string CGModifyNickname::toString () 
	const throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "CGModifyNickname("
		<< ")";
	return msg.toString();

	__END_CATCH
}
#endif