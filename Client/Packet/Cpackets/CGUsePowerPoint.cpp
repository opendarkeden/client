//////////////////////////////////////////////////////////////////////////////
// Filename    : CGUsePowerPoint.cpp 
// Written By  :
// Description :
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGUsePowerPoint.h"

//////////////////////////////////////////////////////////////////////////////
// class CGUsePowerPoint member methods
//////////////////////////////////////////////////////////////////////////////

CGUsePowerPoint::CGUsePowerPoint () 
     throw ()
{
	__BEGIN_TRY
	__END_CATCH
}
	
CGUsePowerPoint::~CGUsePowerPoint () 
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

void CGUsePowerPoint::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	__END_CATCH
}
		    
void CGUsePowerPoint::write ( SocketOutputStream & oStream ) 
     const throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	__END_CATCH
}

void CGUsePowerPoint::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
#ifndef __GAME_CLIENT__		
	CGUsePowerPointHandler::execute( this , pPlayer );
#endif		
	__END_CATCH
}
#ifdef __DEBUG_OUTPUT__
string CGUsePowerPoint::toString () 
	const throw ()
{
	__BEGIN_TRY

	StringStream msg;
	msg << "CGUsePowerPoint()";
	return msg.toString();

	__END_CATCH
}
#endif