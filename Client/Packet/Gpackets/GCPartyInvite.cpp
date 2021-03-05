//////////////////////////////////////////////////////////////////////////////
// Filename    : GCPartyInvite.cpp 
// Written By  : ±è¼º¹Î
// Description :
//////////////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "GCPartyInvite.h"

//////////////////////////////////////////////////////////////////////////////
// class GCPartyInvite member methods
//////////////////////////////////////////////////////////////////////////////

void GCPartyInvite::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

	iStream.read(m_Code);
	iStream.read(m_TargetObjectID);
		
	__END_CATCH
}
		    
void GCPartyInvite::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY
	
	oStream.write(m_Code);
	oStream.write(m_TargetObjectID);

	__END_CATCH
}

void GCPartyInvite::execute (Player * pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	GCPartyInviteHandler::execute(this , pPlayer);

	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
std::string GCPartyInvite::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "GCPartyInvite("
		<< "CODE:" << (int)m_Code
		<< "TOID:" << m_TargetObjectID
		<< ")";
	return msg.toString();
		
	__END_CATCH
}
#endif

