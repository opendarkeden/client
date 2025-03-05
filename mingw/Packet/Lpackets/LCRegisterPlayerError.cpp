//////////////////////////////////////////////////////////////////////
// 
// Filename    : LCRegisterPlayerError.cpp 
// Written By  : Reiot
// Description : 로그인 실패시 로그인 서버에서 클라이언트에게 전송하는 패킷
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "LCRegisterPlayerError.h"

//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void LCRegisterPlayerError::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	iStream.read( m_ErrorID );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void LCRegisterPlayerError::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write( m_ErrorID );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void LCRegisterPlayerError::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	LCRegisterPlayerErrorHandler::execute( this , pPlayer );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// get packet's debug string
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string LCRegisterPlayerError::toString () const
		   throw ()
	{
		__BEGIN_TRY

		StringStream msg;
		msg << "LCRegisterPlayerError( " << (int)m_ErrorID << ")";
		return msg.toString();

		__END_CATCH
	}
#endif

