//////////////////////////////////////////////////////////////////////////////
// Filename    : CGSelectRankBonus.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGSelectRankBonus.h"

void CGSelectRankBonus::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	iStream.read(m_RankBonusType);

	__END_CATCH
}
		    
void CGSelectRankBonus::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	oStream.write(m_RankBonusType);

	__END_CATCH
}

void CGSelectRankBonus::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

#ifndef __GAME_CLIENT__
	CGSelectRankBonusHandler::execute(this , pPlayer);
#endif

	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
std::string CGSelectRankBonus::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;

	msg << "CGSelectRankBonus("
	    << "RankBonusType:"  << (int)m_RankBonusType
	    << ")";
	
	return msg.toString();

	__END_CATCH
}
#endif