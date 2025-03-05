//////////////////////////////////////////////////////////////////////
// 
// Filename    : RCConnectVerify.cpp 
// Written By  : elca@ewestsoft.com
// Description : 자신에게 쓰는 기술의 성공을 알리기 위한 패킷 클래스의
//               멤버 정의.
// 
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "RCConnectVerify.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
RCConnectVerify::RCConnectVerify () 
     throw ()
{
	__BEGIN_TRY
	
	m_Code = RC_CONNECT_NULL;

	__END_CATCH
}

	
//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
RCConnectVerify::~RCConnectVerify () 
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void RCConnectVerify::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	BYTE code;
	iStream.read( code);		

	m_Code = (RC_CONNECT_VERIFY_CODE)code;

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void RCConnectVerify::write ( SocketOutputStream & oStream ) 
     const throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	BYTE code = (BYTE)m_Code;
	oStream.write( code);
	
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////
//
// execute packet's handler
//
//////////////////////////////////////////////////////////////////////
void RCConnectVerify::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	RCConnectVerifyHandler::execute( this , pPlayer );
		
	__END_CATCH
}



//////////////////////////////////////////////////////////////////////
//
// get packet's debug string
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string RCConnectVerify::toString () 
		const throw ()
	{
		__BEGIN_TRY

		StringStream msg;

		msg << "RCConnectVerify( "
			<< "code: " << (int)m_Code 
			<< ")";

		return msg.toString();

		__END_CATCH
	}

#endif