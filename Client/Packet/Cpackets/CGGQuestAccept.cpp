//////////////////////////////////////////////////////////////////////////////
// Filename    : CGGQuestAccept.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGGQuestAccept.h"

void CGGQuestAccept::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	iStream.read(m_QuestID);

	__END_CATCH
}
		    
void CGGQuestAccept::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	oStream.write(m_QuestID);

	__END_CATCH
}

void CGGQuestAccept::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
#ifndef __GAME_CLIENT__		
	CGGQuestAcceptHandler::execute(this , pPlayer);
#endif
	__END_CATCH
}
#ifdef __DEBUG_OUTPUT__
string CGGQuestAccept::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "CGGQuestAccept("
	    << "QuestID:"  << (int)m_QuestID << ","
	    << ")";
	
	return msg.toString();

	__END_CATCH
}
#endif