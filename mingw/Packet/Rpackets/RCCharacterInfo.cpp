//////////////////////////////////////////////////////////////////////
// 
// Filename    : RCCharacterInfo.cpp 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "RCCharacterInfo.h"


RCCharacterInfo::RCCharacterInfo()
{
	m_GuildID = 0;
}

//////////////////////////////////////////////////////////////////////
// Datagram 객체로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void RCCharacterInfo::read ( Datagram & iDatagram ) 
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
	iDatagram.read( m_GuildID );	

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// Datagram 객체로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void RCCharacterInfo::write ( Datagram & oDatagram ) const 
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

	oDatagram.write( m_GuildID );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void RCCharacterInfo::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	RCCharacterInfoHandler::execute( this );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string RCCharacterInfo::toString () const
		throw ()
	{
		StringStream msg;
		
		msg << "RCCharacterInfo( "
			<< ",GuildID" << (int)m_GuildID
			<< ")";


		return msg.toString();
	}
#endif

