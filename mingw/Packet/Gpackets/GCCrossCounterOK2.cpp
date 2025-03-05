//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCCrossCounterOK2.cpp 
// Written By  : elca@ewestsoft.com
// Description : 자신에게 쓰는 기술의 성공을 알리기 위한 패킷 클래스의
//               멤버 정의.
// 
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "GCCrossCounterOK2.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
GCCrossCounterOK2::GCCrossCounterOK2 ()
     throw ()
{
	__BEGIN_TRY


	__END_CATCH
}

	
//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
GCCrossCounterOK2::~GCCrossCounterOK2 ()
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void GCCrossCounterOK2::read ( SocketInputStream & iStream )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	// 최적화 작업시 실제 크기를 명시하도록 한다.
	iStream.read( m_ObjectID );
	ModifyInfo::read( iStream);
	iStream.read( m_SkillType);

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void GCCrossCounterOK2::write ( SocketOutputStream & oStream )
     const throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	// 최적화 작업시 실제 크기를 명시하도록 한다.
	oStream.write( m_ObjectID );
	ModifyInfo::write( oStream);
	oStream.write( m_SkillType );

	__END_CATCH
}
//////////////////////////////////////////////////////////////////////
//
// execute packet's handler
//
//////////////////////////////////////////////////////////////////////
void GCCrossCounterOK2::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	GCCrossCounterOK2Handler::execute( this , pPlayer );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// get packet's debug std::string
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
std::string GCCrossCounterOK2::toString () 
	const throw ()
{
	__BEGIN_TRY

	StringStream msg;

	msg << "GCCrossCounterOK2(ObjectID: " << (int)m_ObjectID;
	msg << ModifyInfo::toString();
	msg << "SkillType : " << (int)m_SkillType;
	msg << ")";

	return msg.toString();

	__END_CATCH
}


#endif