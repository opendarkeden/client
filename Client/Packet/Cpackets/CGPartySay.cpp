//////////////////////////////////////////////////////////////////////////////
// Filename    : CGPartySay.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGPartySay.h"

CGPartySay::CGPartySay () 
     throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

CGPartySay::~CGPartySay () 
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

void CGPartySay::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

	BYTE szMessage;
	iStream.read(m_Color);
	iStream.read(szMessage);
	iStream.read(m_Message, szMessage);
	
	__END_CATCH
}
		    
void CGPartySay::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	BYTE szMessage = m_Message.size();
	oStream.write(m_Color);
	oStream.write(szMessage);
	oStream.write(m_Message);

	__END_CATCH
}

void CGPartySay::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
#ifndef __GAME_CLIENT__
	CGPartySayHandler::execute (this , pPlayer);
#endif		
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
// get debug std::string
//////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
string CGPartySay::toString () const
    throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "CGPartySay("
		<< "Color :" << m_Color
		<< "Message :" << m_Message
		<< ")";
	return msg.toString();

	__END_CATCH
}
#endif