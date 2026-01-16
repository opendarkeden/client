//////////////////////////////////////////////////////////////////////////////
// Filename    : CGAuthKey.cpp 
// Written By  : excel96
// Description :
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGAuthKey.h"

void CGAuthKey::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	iStream.read( m_Key );

	__END_CATCH
}
		    
void CGAuthKey::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	oStream.write( m_Key );

	__END_CATCH
}

void CGAuthKey::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifndef __GAME_CLIENT__
	CGAuthKeyHandler::execute( this , pPlayer );
#endif
	__END_CATCH
}
#ifdef __DEBUG_OUTPUT__
string CGAuthKey::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "CGAuthKey("
		<< "Key:" << m_Key 
		<< ")" ;
	return msg.toString();
		
	__END_CATCH
}


#endif