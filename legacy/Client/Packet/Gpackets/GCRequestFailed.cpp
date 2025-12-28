//////////////////////////////////////////////////////////////////////////////
// Filename    : GCRequestFailed.cpp
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "GCRequestFailed.h"

//////////////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////////////
void GCRequestFailed::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	iStream.read(m_Code);

	BYTE num;
	iStream.read( num );

	if (num==0)
		throw InvalidProtocolException("szName==0");

	iStream.read(m_Name, num);

	__END_CATCH
}
		    
//////////////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////////////
void GCRequestFailed::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write(m_Code);

	BYTE num = m_Name.size();
	oStream.write( num );

	if (num==0)
		throw InvalidProtocolException("szName==0");

	oStream.write(m_Name);

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////////////
void GCRequestFailed::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	GCRequestFailedHandler::execute( this , pPlayer );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////////////
// get packet's debug std::string
//////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
std::string GCRequestFailed::toString () const
       throw ()
{
	__BEGIN_TRY

	StringStream msg;
	msg << "GCRequestFailed(" 
	    << "Code : " << (int)m_Code 
		<< "Name : " << m_Name
		<< ")";
	return msg.toString();

	__END_CATCH
}


#endif