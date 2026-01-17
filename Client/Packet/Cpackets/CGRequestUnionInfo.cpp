//////////////////////////////////////////////////////////////////////////////
// Filename    : CGRequestUnionInfo.cpp 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGRequestUnionInfo.h"


void CGRequestUnionInfo::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	__END_CATCH
}

void CGRequestUnionInfo::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	__END_CATCH
}

void CGRequestUnionInfo::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
#ifndef __GAME_CLIENT__			
	CGRequestUnionInfoHandler::execute(this , pPlayer);
#endif
	__END_CATCH
}
#ifdef __DEBUG_OUTPUT__
string CGRequestUnionInfo::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
    msg << "CGRequestUnionInfo("
		<< ")" ;
	return msg.toString();

	__END_CATCH
}
#endif