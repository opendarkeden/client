//////////////////////////////////////////////////////////////////////
// 
// Filename    : BloodBibleSignInfo.cpp 
// Written By  : elca@ewestsoft.com
// Description : 자신에게 쓰는 기술의 성공을 알리기 위한 패킷 클래스의
//               멤버 정의.
// 
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "BloodBibleSignInfo.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "Assert.h"

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
BloodBibleSignInfo::BloodBibleSignInfo () 
     throw ()
{
	__BEGIN_TRY

	m_OpenNum = 0; 

	__END_CATCH
}

	
//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
BloodBibleSignInfo::~BloodBibleSignInfo () 
    throw ()
{
	__BEGIN_TRY

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void BloodBibleSignInfo::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	iStream.read(m_OpenNum);
	iStream.read(m_ListSize);
	for ( int i=0; i<m_ListSize; ++i )
	{
		ItemType_t type;
		iStream.read( type );
		m_SignList.push_back( type );
	}

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void BloodBibleSignInfo::write ( SocketOutputStream & oStream ) 
     const throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write(m_OpenNum);
	oStream.write(m_ListSize);
	for ( int i=0; i<m_ListSize; ++i )
	{
		oStream.write(m_SignList[i]);
	}
		
	__END_CATCH
}

