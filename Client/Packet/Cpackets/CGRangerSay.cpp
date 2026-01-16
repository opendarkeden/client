//////////////////////////////////////////////////////////////////////////////
// Filename    : CGWhisper.cpp 
// Written By  : reiot@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGRangerSay.h"

void CGRangerSay::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	// 메세지 읽기
	BYTE szMessage;

	iStream.read(szMessage);

	if (szMessage == 0)
		throw InvalidProtocolException("szMessage == 0");

	if (szMessage > 128)
		throw InvalidProtocolException("too large message length");

	iStream.read(m_Message , szMessage);

	__END_CATCH
}
		    
void CGRangerSay::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	// 메세지 쓰기
	BYTE szMessage = m_Message.size();

	if (szMessage == 0)
		throw InvalidProtocolException("szMessage == 0");

	if (szMessage > 128)
		throw InvalidProtocolException("too large message length");

	oStream.write(szMessage);

	oStream.write(m_Message);

	__END_CATCH
}

void CGRangerSay::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
#ifndef __GAME_CLIENT__			
	CGWhisperHandler::execute(this , pPlayer);
#endif
	__END_CATCH
}
#ifdef __DEBUG_OUTPUT__
string CGRangerSay::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "( CGRangerSay : " << m_Message
		<< ")" ;
	return msg.toString();

	__END_CATCH
}
#endif