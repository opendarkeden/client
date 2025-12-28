//////////////////////////////////////////////////////////////////////
// 
// Filename    : CRRequest.cpp 
// Written By  : elca@ewestsoft.com
// Description : 다른 클라이언트에게 뭔가를 요청하는거다.
// 
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CRRequest.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CRRequest::CRRequest () 
     throw ()
{
	__BEGIN_TRY

	m_Code = CR_REQUEST_NULL;

	__END_CATCH
}

	
//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CRRequest::~CRRequest () 
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void CRRequest::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	BYTE code;
	iStream.read( code);
	m_Code = (CR_REQUEST_CODE)code;

	BYTE num;
	iStream.read( num );

	if (num > 20)
		throw InvalidProtocolException("szRequestName>20");
	
	if (num > 0)
	{
		iStream.read( m_RequestName, num );
	}
		
	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void CRRequest::write ( SocketOutputStream & oStream ) 
     const throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	BYTE code = (BYTE)m_Code;
	oStream.write( code);

	
	BYTE num = m_RequestName.size();
	
	if (num > 20)
		throw InvalidProtocolException("szRequestName>20");

	oStream.write( num );

	if (num > 0)
	{
		oStream.write( m_RequestName );	
	}

	
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////
//
// execute packet's handler
//
//////////////////////////////////////////////////////////////////////
void CRRequest::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	CRRequestHandler::execute( this , pPlayer );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// get packet's debug std::string
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string CRRequest::toString () 
		const throw ()
	{
		__BEGIN_TRY

		StringStream msg;

		msg << "CRRequest( "
			<< "code: " << (int)m_Code
			<< "RequestName: " << m_RequestName
			<< ")";

		return msg.toString();

		__END_CATCH
	}

#endif