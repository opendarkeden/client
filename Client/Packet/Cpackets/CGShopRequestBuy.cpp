////////////////////////////////////////////////////////////////////////////////
// Filename    : CGShopRequestBuy.cpp 
// Written By  : 김성민
// Description : 
// 플레이어가 상점 NPC의 진열창을 보고, 물건을 구입하고자 할 때
// 보내는 패킷이다. 서버는 플레이어에게 충분한 돈과 인벤토리에
// 자리가 있는지 검증한 후 플레이어에게 아이템을 넘긴다.
////////////////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "CGShopRequestBuy.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"


////////////////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
////////////////////////////////////////////////////////////////////////////////
void CGShopRequestBuy::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	iStream.read( m_ObjectID );
	iStream.read( m_RackType );
	iStream.read( m_RackIndex );
	iStream.read( m_Num );
	iStream.read( m_X );
	iStream.read( m_Y );

	__END_CATCH
}

		    
////////////////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
////////////////////////////////////////////////////////////////////////////////
void CGShopRequestBuy::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write( m_ObjectID );
	oStream.write( m_RackType );
	oStream.write( m_RackIndex );
	oStream.write( m_Num );
	oStream.write( m_X );
	oStream.write( m_Y );

	__END_CATCH
}


////////////////////////////////////////////////////////////////////////////////
// execute packet's handler
////////////////////////////////////////////////////////////////////////////////
void CGShopRequestBuy::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifndef __GAME_CLIENT__
	CGShopRequestBuyHandler::execute ( this , pPlayer );
#endif
		
	__END_CATCH
}


////////////////////////////////////////////////////////////////////////////////
// get debug std::string
////////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
std::string CGShopRequestBuy::toString () 
	const throw ()
{
	__BEGIN_TRY

	StringStream msg;
	msg << "CGShopRequestBuy(" 
	 		<< "ObjectID:" << (int)m_ObjectID  << ","
			<< "RackType:" << (int)m_RackType << ","
			<< "RackIndex:" << (int)m_RackIndex << ","
			<< "NumberOfItems:" << (int)m_Num << ","
			<< "InvenX" << (int)m_X << ","
			<< "InvenY" << (int)m_Y << ","
			<< ")";
	return msg.toString();

	__END_CATCH
}
#endif