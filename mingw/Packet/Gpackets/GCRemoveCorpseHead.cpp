//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCRemoveCorpseHead.cpp 
// Written By  : Reiot
// Description : 
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCRemoveCorpseHead.h"

//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void GCRemoveCorpseHead::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	iStream.read( m_ObjectID );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void GCRemoveCorpseHead::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write( m_ObjectID );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void GCRemoveCorpseHead::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	GCRemoveCorpseHeadHandler::execute ( this , pPlayer );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// get debug std::string
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string GCRemoveCorpseHead::toString () const
		throw ()
	{
		StringStream msg;

		msg << "GCRemoveCorpseHead("
			<< "ObjectID:" << m_ObjectID
			<< ")";

		return msg.toString();
	}

#endif