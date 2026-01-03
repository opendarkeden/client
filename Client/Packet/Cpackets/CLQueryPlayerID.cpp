//////////////////////////////////////////////////////////////////////
// 
// Filename    : CLQueryPlayerID.cpp 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "CLQueryPlayerID.h"


//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void CLQueryPlayerID::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	//--------------------------------------------------
	// read player id
	//--------------------------------------------------
	BYTE szPlayerID;

	iStream.read( szPlayerID );

	if ( szPlayerID == 0 )
		throw InvalidProtocolException("szPlayerID == 0");

	if ( szPlayerID > 20 )
		throw InvalidProtocolException("too long PlayerID length");

	iStream.read( m_PlayerID , szPlayerID );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void CLQueryPlayerID::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	//--------------------------------------------------
	// write player id
	//--------------------------------------------------
	BYTE szPlayerID = m_PlayerID.size();

	if ( szPlayerID == 0 )
		throw InvalidProtocolException("empty PlayerID");

	if ( szPlayerID > 20 )
		throw InvalidProtocolException("too long PlayerID length");

	oStream.write( szPlayerID );

	oStream.write( m_PlayerID );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void CLQueryPlayerID::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	#ifndef __GAME_CLIENT__
		CLQueryPlayerIDHandler::execute ( this , pPlayer );
	#endif
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// get debug std::string
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string CLQueryPlayerID::toString () const
		throw ()
	{
		__BEGIN_TRY
			
		StringStream msg;
		msg << "CLQueryPlayerID("
			<< "PlayerID:" << m_PlayerID 
			<< ")";
		return msg.toString();
			
		__END_CATCH
	}

#endif
