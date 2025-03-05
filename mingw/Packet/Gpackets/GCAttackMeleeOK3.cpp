//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCAttackMeleeOK3.cpp 
// Written By  : elca@ewestsoft.com
// Description : 자신에게 쓰는 기술의 성공을 알리기 위한 패킷 클래스의
//               멤버 정의.
// 
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "GCAttackMeleeOK3.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
GCAttackMeleeOK3::GCAttackMeleeOK3 ()
     throw ()
{
	__BEGIN_TRY

	__END_CATCH
}

	
//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
GCAttackMeleeOK3::~GCAttackMeleeOK3 ()
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void GCAttackMeleeOK3::read ( SocketInputStream & iStream )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	// 최적화 작업시 실제 크기를 명시하도록 한다.
	iStream.read( m_ObjectID );
	iStream.read( m_TargetObjectID );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void GCAttackMeleeOK3::write ( SocketOutputStream & oStream )
     const throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	// 최적화 작업시 실제 크기를 명시하도록 한다.
	oStream.write( m_ObjectID );
	oStream.write( m_TargetObjectID );

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////
//
// execute packet's handler
//
//////////////////////////////////////////////////////////////////////
void GCAttackMeleeOK3::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	GCAttackMeleeOK3Handler::execute( this , pPlayer );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// get packet's debug std::string
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string GCAttackMeleeOK3::toString () 
		const throw ()
	{
		__BEGIN_TRY

		StringStream msg;

		msg << "GCAttackMeleeOK3(ObjectID: " << (int)m_ObjectID 
			<< ",TargetObjectID: " << (int)m_TargetObjectID
			<< ")";

		return msg.toString();

		__END_CATCH
	}
#endif
