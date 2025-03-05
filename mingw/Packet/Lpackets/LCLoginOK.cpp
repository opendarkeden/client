//////////////////////////////////////////////////////////////////////
// 
// Filename    : LCLoginOK.cpp
// Written By  : Reiot
// Description : 
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "LCLoginOK.h"
#include "UserInformation.h"

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void LCLoginOK::read ( SocketInputStream & iStream )
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	// 서버 그룹 이름을 읽는다.
	iStream.read( m_isAdult );
	iStream.read( m_bFamily );
	iStream.read( m_Stat );
	iStream.read( m_LastDays );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void LCLoginOK::write ( SocketOutputStream & oStream ) const
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	// 그룹 이름을 쓴다.
	oStream.write( m_isAdult );
	oStream.write( m_bFamily );
	oStream.write( m_Stat );
	
	oStream.write( m_LastDays );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void LCLoginOK::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	LCLoginOKHandler::execute( this , pPlayer );
		
	__END_CATCH
}


PacketSize_t LCLoginOK::getPacketSize() const throw() 
{ 
	return szBYTE + szBYTE + szBYTE + szWORD; 
}