//////////////////////////////////////////////////////////////////////////////
// Filename    : CGWithdrawTax.cpp 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGWithdrawTax.h"

void CGWithdrawTax::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	iStream.read( m_Gold );

	__END_CATCH
}

void CGWithdrawTax::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	oStream.write( m_Gold );
	
	__END_CATCH
}

void CGWithdrawTax::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
#ifndef __GAME_CLIENT__		
	CGWithdrawTaxHandler::execute(this , pPlayer);
#endif

	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__

std::string CGWithdrawTax::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;

    msg << "CGWithdrawTax("
		<< "Gold : " << (int)m_Gold
		<< ")" ;
	return msg.toString();

	__END_CATCH
}
#endif

