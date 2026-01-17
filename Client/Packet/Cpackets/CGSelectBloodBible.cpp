//////////////////////////////////////////////////////////////////////////////
// Filename    : CGSelectBloodBible.cpp 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGSelectBloodBible.h"


void CGSelectBloodBible::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	iStream.read( m_BloodBibleID );

	__END_CATCH
}

void CGSelectBloodBible::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	oStream.write( m_BloodBibleID );

	__END_CATCH
}

void CGSelectBloodBible::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
#ifndef __GAME_CLIENT__		
	CGSelectBloodBibleHandler::execute(this , pPlayer);
#endif
	__END_CATCH
}
#ifdef __DEBUG_OUTPUT__
std::string CGSelectBloodBible::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
    msg << "CGSelectBloodBible("
		<< "BloodBibleID:" << m_BloodBibleID
		<< ")" ;
	return msg.toString();

	__END_CATCH
}
#endif