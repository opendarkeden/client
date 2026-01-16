//////////////////////////////////////////////////////////////////////////////
// Filename    : CGSubmitScore.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGSubmitScore.h"

CGSubmitScore::CGSubmitScore () 
     throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

CGSubmitScore::~CGSubmitScore () 
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

void CGSubmitScore::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	iStream.read(m_GameType);
	iStream.read(m_Level);
	iStream.read(m_Score);

	__END_CATCH
}
		    
void CGSubmitScore::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	oStream.write(m_GameType);
	oStream.write(m_Level);
	oStream.write(m_Score);

	__END_CATCH
}

void CGSubmitScore::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
#ifndef __GAME_CLIENT__
	CGSubmitScoreHandler::execute (this , pPlayer);
#endif
		
	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
std::string CGSubmitScore::toString () const
    throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "CGSubmitScore("
		<< ")";
	return msg.toString();

	__END_CATCH
}
#endif