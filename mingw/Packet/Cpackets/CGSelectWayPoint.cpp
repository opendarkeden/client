//////////////////////////////////////////////////////////////////////////////
// Filename    : CGSelectWayPoint.cpp 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGSelectWayPoint.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CGSelectWayPoint::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	iStream.read( m_ZoneID );
	iStream.read( m_X );
	iStream.read( m_Y );

	__END_CATCH
}
		    
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CGSelectWayPoint::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	oStream.write( m_ZoneID );
	oStream.write( m_X );
	oStream.write( m_Y );

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CGSelectWayPoint::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifndef __GAME_CLIENT__
	CGSelectWayPointHandler::execute( this , pPlayer );
#endif

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string CGSelectWayPoint::toString () const
		   throw ()
	{
		__BEGIN_TRY
			
		StringStream msg;
		msg << "CGSelectWayPoint("
			<< "ZoneID:" << (int)m_ZoneID
			<< ",X:"      << (int)m_X
			<< ",Y:"      << (int)m_Y
			<< ")" ;
		return msg.toString();

		__END_CATCH
	}
#endif