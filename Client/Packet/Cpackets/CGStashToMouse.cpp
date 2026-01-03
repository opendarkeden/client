////////////////////////////////////////////////////////////////////////////////
// 
// Filename    : CGStashToMouse.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
// 
////////////////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "CGStashToMouse.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

////////////////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
////////////////////////////////////////////////////////////////////////////////
void CGStashToMouse::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY 
	__BEGIN_DEBUG
		
	iStream.read( m_ObjectID );
	iStream.read( m_Rack );
	iStream.read( m_Index );

	__END_DEBUG 
	__END_CATCH
}

		    
////////////////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
////////////////////////////////////////////////////////////////////////////////
void CGStashToMouse::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	__BEGIN_DEBUG

	oStream.write( m_ObjectID );
	oStream.write( m_Rack );
	oStream.write( m_Index );

	__END_DEBUG
	__END_CATCH
}


////////////////////////////////////////////////////////////////////////////////
// execute packet's handler
////////////////////////////////////////////////////////////////////////////////
void CGStashToMouse::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	__BEGIN_DEBUG

	#ifndef __GAME_CLIENT__
		CGStashToMouseHandler::execute ( this , pPlayer );
	#endif
		
	__END_DEBUG
	__END_CATCH
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string CGStashToMouse::toString () 
		const throw ()
	{
		__BEGIN_TRY
		__BEGIN_DEBUG
			
		StringStream msg;
		msg << "CGStashToMouse("
			<< " ObjectID : " << (int)m_ObjectID
			<< " Rack : "     << (int)m_Rack
			<< " Index : "    << (int)m_Index
			<< ")";
		return msg.toString();

		__END_DEBUG
		__END_CATCH
	}
#endif