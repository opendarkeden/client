//////////////////////////////////////////////////////////////////////////////
// Filename    : CGModifyTaxRatio.cpp 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGModifyTaxRatio.h"

void CGModifyTaxRatio::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	iStream.read( m_Ratio );

	__END_CATCH
}

void CGModifyTaxRatio::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	oStream.write( m_Ratio );
	
	__END_CATCH
}

void CGModifyTaxRatio::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
#ifndef __GAME_CLIENT__		
	CGModifyTaxRatioHandler::execute(this , pPlayer);
#endif
	__END_CATCH
}
#ifdef __DEBUG_OUTPUT__
std::string CGModifyTaxRatio::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
    msg << "CGModifyTaxRatio("
		<< "Ratio : " << (int)m_Ratio
		<< ")" ;
	return msg.toString();

	__END_CATCH
}
#endif