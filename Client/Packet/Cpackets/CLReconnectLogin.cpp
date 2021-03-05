//----------------------------------------------------------------------
// 
// Filename    : CLReconnectLogin.cpp 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "CLReconnectLogin.h"

//----------------------------------------------------------------------
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//----------------------------------------------------------------------
void CLReconnectLogin::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	//--------------------------------------------------
	// read authentication key
	//--------------------------------------------------
	iStream.read( m_Key );
	iStream.read( m_LoginMode );

	__END_CATCH
}

		    
//----------------------------------------------------------------------
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//----------------------------------------------------------------------
void CLReconnectLogin::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	//--------------------------------------------------
	// write authentication key
	//--------------------------------------------------
	oStream.write( m_Key );
	oStream.write( m_LoginMode );

	__END_CATCH
}


//----------------------------------------------------------------------
// execute packet's handler
//----------------------------------------------------------------------
void CLReconnectLogin::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	#ifndef __GAME_CLIENT__
		CLReconnectLoginHandler::execute( this , pPlayer );
	#endif

	__END_CATCH
}


//----------------------------------------------------------------------
// get packet's debug std::string
//----------------------------------------------------------------------
#ifdef __DEBUG_OUTPUT__
	std::string CLReconnectLogin::toString () const
		   throw ()
	{
		StringStream msg;
		
		msg << "CLReconnectLogin("
			<< "KEY:" << m_Key 
			<< ")" ;
		
		return msg.toString();
	}
#endif
