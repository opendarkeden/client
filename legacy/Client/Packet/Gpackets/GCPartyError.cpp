//////////////////////////////////////////////////////////////////////////////
// Filename    : GCPartyError.cpp 
// Written By  : ±è¼º¹Î
// Description :
//////////////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "GCPartyError.h"

//////////////////////////////////////////////////////////////////////////////
// class GCPartyError member methods
//////////////////////////////////////////////////////////////////////////////

void GCPartyError::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

	iStream.read(m_Code);
	iStream.read(m_TargetObjectID);
		
	__END_CATCH
}
		    
void GCPartyError::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY
	
	oStream.write(m_Code);
	oStream.write(m_TargetObjectID);

	__END_CATCH
}

void GCPartyError::execute (Player * pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	GCPartyErrorHandler::execute(this , pPlayer);

	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
std::string GCPartyError::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "GCPartyError("
		<< "CODE:" << (int)m_Code
		<< "TOID:" << m_TargetObjectID
		<< ")";
	return msg.toString();
		
	__END_CATCH
}
#endif