//////////////////////////////////////////////////////////////////////
// 
// Filename    : CLLogin.cpp 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "CLLogin.h"
#include "UserInformation.h"


//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void CLLogin::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	BYTE szID;

	iStream.read( szID );

	if ( szID == 0 )
		throw InvalidProtocolException("szID == 0");

	if ( szID > 30 )
		throw InvalidProtocolException("too large ID length");

	iStream.read( m_ID , szID );

	BYTE szPassword;

	iStream.read( szPassword );

	if ( szPassword == 0 )
		throw InvalidProtocolException("szPassword == 0");

	if ( szPassword > 20 )
		throw InvalidProtocolException("too large password length");

	iStream.read( m_Password , szPassword );

	iStream.read( (char*)m_MacAddress , 6 );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void CLLogin::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	if( g_pUserInformation == NULL || !g_pUserInformation->IsNetmarble )
	{
		BYTE szID = m_ID.size();

		if ( szID == 0 )
			throw InvalidProtocolException("empty ID");

		if ( szID > 30 )
			throw InvalidProtocolException("too large ID length");

		oStream.write( szID );

		oStream.write( m_ID );

		BYTE szPassword = m_Password.size();

		if ( szPassword == 0 )
			throw InvalidProtocolException("szPassword == 0");
		if ( szPassword > 20 )
			throw InvalidProtocolException("too large password length");

		oStream.write( szPassword );

		oStream.write( m_Password );

		oStream.write( (char*)m_MacAddress, 6*sizeof(BYTE) );
		
		BYTE loginMode = 0;
		// add by Coffee 2006.11.7 藤속貢籃되쩍친駕
		oStream.write( loginMode );
		// end by Coffee
		//oStream.write( m_LoginMode );
	}
	else
	{
		int	szID = m_ID.size();

		if ( szID == 0 )
			throw InvalidProtocolException("empty ID");
		if ( szID > 2048 )
			throw InvalidProtocolException("too large ID length");

		oStream.write( szID );
		oStream.write( m_ID );
	}

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void CLLogin::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	#ifndef __GAME_CLIENT__
		CLLoginHandler::execute ( this , pPlayer );
	#endif
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// get debug std::string
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string CLLogin::toString () const
		throw ()
	{
		char szTemp[10];

		StringStream msg;

		msg << "CLLogin("
			<< "ID:" << m_ID 
			<< ",Password:" << m_Password 
			<< ",MacAddress:"
			<< itoa( m_MacAddress[0], szTemp, 16 )
			<< itoa( m_MacAddress[1], szTemp, 16 )
			<< itoa( m_MacAddress[2], szTemp, 16 )
			<< itoa( m_MacAddress[3], szTemp, 16 )
			<< itoa( m_MacAddress[4], szTemp, 16 )
			<< itoa( m_MacAddress[5], szTemp, 16 )
			<< ")";

		return msg.toString();
	}

#endif

PacketSize_t CLLogin::getPacketSize () const throw ()
{
	if( g_pUserInformation == NULL || !g_pUserInformation->IsNetmarble )
	{
		return szBYTE + m_ID.size() + szBYTE + m_Password.size() + 6 + szBYTE;
	}
	
	return szint + m_ID.size();
}