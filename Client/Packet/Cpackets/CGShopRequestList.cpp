//--------------------------------------------------------------------------------
// 
// Filename    : CGShopRequestList.cpp 
// Written By  : 김성민
// Description : 플레이어가 가지고 있는 상점 버전과 서버가 가지고 잇는 상점의 
//               버전이 다를 경우, 플레이어는 서버에게 상품의 리스트를 
//               요청하게 된다. 이 패킷은 그때 보내게 되는 패킷이다.
// 
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "CGShopRequestList.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"


//--------------------------------------------------------------------------------
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//--------------------------------------------------------------------------------
void CGShopRequestList::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	iStream.read( m_ObjectID );
	iStream.read( m_RackType );

	__END_CATCH
}

		    
//--------------------------------------------------------------------------------
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//--------------------------------------------------------------------------------
void CGShopRequestList::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write( m_ObjectID );
	oStream.write( m_RackType );

	__END_CATCH
}


//--------------------------------------------------------------------------------
// execute packet's handler
//--------------------------------------------------------------------------------
void CGShopRequestList::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	#ifndef __GAME_CLIENT__
		CGShopRequestListHandler::execute ( this , pPlayer );
	#endif
		
	__END_CATCH
}


//--------------------------------------------------------------------------------
// get debug std::string
//--------------------------------------------------------------------------------
#ifdef __DEBUG_OUTPUT__
	std::string CGShopRequestList::toString () 
		const throw ()
	{
		StringStream msg;
		
		msg << "CGShopRequestList(" 
			<< "ObjectID:" << (int)m_ObjectID 
			<< "RackType:" << (int)m_RackType
			<< ")";
			
		return msg.toString();
	}
#endif
