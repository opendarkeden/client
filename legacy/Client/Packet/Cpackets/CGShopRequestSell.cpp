////////////////////////////////////////////////////////////////////////////////
// Filename    : CGShopRequestSell.cpp 
// Written By  : 김성민
// Description : 
// 플레이어에게 자신이 가지고 있는 아이템을 상점 NPC에게
// 팔기를 요청할 때 쓰이는 패킷이다.
////////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGShopRequestSell.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

////////////////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
////////////////////////////////////////////////////////////////////////////////
void CGShopRequestSell::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	iStream.read( m_ObjectID );
	iStream.read( m_ItemObjectID );
	iStream.read( m_OpCode );

	__END_CATCH
}

		    
////////////////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
////////////////////////////////////////////////////////////////////////////////
void CGShopRequestSell::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write( m_ObjectID );
	oStream.write( m_ItemObjectID );
	oStream.write( m_OpCode );

	__END_CATCH
}


////////////////////////////////////////////////////////////////////////////////
// execute packet's handler
////////////////////////////////////////////////////////////////////////////////
void CGShopRequestSell::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	#ifndef __GAME_CLIENT__
		CGShopRequestSellHandler::execute ( this , pPlayer );
	#endif
		
	__END_CATCH
}


////////////////////////////////////////////////////////////////////////////////
// get debug std::string
////////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
std::string CGShopRequestSell::toString () const 
	throw ()
{
	__BEGIN_TRY

	StringStream msg;
	msg << "CGShopRequestSell( ObjectID : " << (int)m_ObjectID << ")";
	return msg.toString();

	__END_CATCH
}
#endif