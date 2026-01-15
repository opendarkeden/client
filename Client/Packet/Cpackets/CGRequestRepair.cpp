//--------------------------------------------------------------------------------
// 
// Filename    : CGRequestRepair.cpp 
// Written By  : 김성민
// Description : 
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "CGRequestRepair.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"


//--------------------------------------------------------------------------------
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//--------------------------------------------------------------------------------
void CGRequestRepair::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	iStream.read( m_ObjectID );

	__END_CATCH
}

		    
//--------------------------------------------------------------------------------
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//--------------------------------------------------------------------------------
void CGRequestRepair::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write( m_ObjectID );

	__END_CATCH
}


//--------------------------------------------------------------------------------
// execute packet's handler
//--------------------------------------------------------------------------------
void CGRequestRepair::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	#ifndef __GAME_CLIENT__
		CGRequestRepairHandler::execute ( this , pPlayer );
	#endif
		
	__END_CATCH
}


//--------------------------------------------------------------------------------
// get debug std::string
//--------------------------------------------------------------------------------
#ifdef __DEBUG_OUTPUT__
	std::string CGRequestRepair::toString () 
		const throw ()
	{
		__BEGIN_TRY

		StringStream msg;
		msg << "CGRequestRepair( ObjectID : " << (int)m_ObjectID << ")";
		return msg.toString();

		__END_CATCH
	}
#endif