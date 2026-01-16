//////////////////////////////////////////////////////////////////////////////
// Filename    : CGGQuestCancel.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGGQuestCancel.h"

void CGGQuestCancel::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	iStream.read(m_QuestID);

	__END_CATCH
}
		    
void CGGQuestCancel::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	oStream.write(m_QuestID);

	__END_CATCH
}

void CGGQuestCancel::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
#ifndef __GAME_CLIENT__
	__BEGIN_TRY
		
	CGGQuestCancelHandler::execute(this , pPlayer);

	__END_CATCH
#endif
}
#ifdef __DEBUG_OUTPUT__
string CGGQuestCancel::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "CGGQuestCancel("
	    << "QuestID:"  << (int)m_QuestID << ","
	    << ")";
	
	return msg.toString();

	__END_CATCH
}
#endif