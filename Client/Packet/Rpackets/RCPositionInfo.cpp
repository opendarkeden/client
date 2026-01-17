//////////////////////////////////////////////////////////////////////
// 
// Filename    : RCPositionInfo.cpp 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "RCPositionInfo.h"


RCPositionInfo::RCPositionInfo()
{
	m_ZoneID = 0;
	m_ZoneX = 0;
	m_ZoneY = 0;
}

//////////////////////////////////////////////////////////////////////
// Datagram 객체로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void RCPositionInfo::read ( Datagram & iDatagram ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	// Name
	BYTE szName;

	iDatagram.read( szName );

	//if ( szName == 0 )
	//	throw InvalidProtocolException("szName == 0");

	if ( szName > 20 )
		throw InvalidProtocolException("too long Name length");

	if (szName > 0)
	{
		iDatagram.read( m_Name , szName );
	}

	// 
	iDatagram.read( m_ZoneID );
	iDatagram.read( m_ZoneX );
	iDatagram.read( m_ZoneY );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// Datagram 객체로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void RCPositionInfo::write ( Datagram & oDatagram ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	// Name
	BYTE szName = m_Name.size();

	//if ( szName == 0 )
	//	throw InvalidProtocolException("szName == 0");

	if ( szName > 20 )
		throw InvalidProtocolException("too long Name length");

	oDatagram.write( szName );

	if (szName > 0)
	{
		oDatagram.write( m_Name );
	}

	oDatagram.write( m_ZoneID );
	oDatagram.write( m_ZoneX );
	oDatagram.write( m_ZoneY );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void RCPositionInfo::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	RCPositionInfoHandler::execute( this );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string RCPositionInfo::toString () const
		throw ()
	{
		StringStream msg;
		
		msg << "RCPositionInfo( "
			<< ",ZoneID: " << (int)m_ZoneID
			<< ",ZoneX: " << (int)m_ZoneX
			<< ",ZoneY: " << (int)m_ZoneY
			<< ")";


		return msg.toString();
	}
#endif

