//--------------------------------------------------------------------------------
// 
// Filename    : CGNPCTalk.cpp 
// Written By  : Reiot
// 
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "CGNPCTalk.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"


//--------------------------------------------------------------------------------
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//--------------------------------------------------------------------------------
void CGNPCTalk::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	iStream.read( m_ObjectID );

	__END_CATCH
}

		    
//--------------------------------------------------------------------------------
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//--------------------------------------------------------------------------------
void CGNPCTalk::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write( m_ObjectID );

	__END_CATCH
}


//--------------------------------------------------------------------------------
// execute packet's handler
//--------------------------------------------------------------------------------
void CGNPCTalk::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	#ifndef __GAME_CLIENT__
		CGNPCTalkHandler::execute ( this , pPlayer );
	#endif
		
	__END_CATCH
}


//--------------------------------------------------------------------------------
// get debug std::string
//--------------------------------------------------------------------------------
#ifdef __DEBUG_OUTPUT__
	std::string CGNPCTalk::toString () 
		const throw ()
	{
		StringStream msg;
		msg << "CGNPCTalk( ObjectID : " << (int)m_ObjectID << ")";
		return msg.toString();
	}
#endif
