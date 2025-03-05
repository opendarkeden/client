//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCAddBurrowingCreature.cc 
// Written By  : Reiot
// Description : 
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCAddBurrowingCreature.h"


//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void GCAddBurrowingCreature::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	// 최적화 작업시 실제 크기를 명시하도록 한다.
	iStream.read( m_ObjectID );

	BYTE szName;

	iStream.read( szName );

	if ( szName == 0 )
		throw InvalidProtocolException("szName == 0");

	if ( szName > 20 )
		throw InvalidProtocolException("too large name length");
		
	iStream.read( m_Name , szName );


	iStream.read( m_X );
	iStream.read( m_Y );
	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void GCAddBurrowingCreature::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	// 최적화 작업시 실제 크기를 명시하도록 한다.
	oStream.write( m_ObjectID );

	BYTE szName = m_Name.size();

	if ( szName == 0 )
		throw InvalidProtocolException("szName == 0");

	if ( szName > 20 )
		throw InvalidProtocolException("too large name length");

	oStream.write( szName );
	oStream.write( m_Name );

	oStream.write( m_X );
	oStream.write( m_Y );
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void GCAddBurrowingCreature::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	GCAddBurrowingCreatureHandler::execute( this , pPlayer );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// get packet's debug std::string
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string GCAddBurrowingCreature::toString () const
		   throw ()
	{
		__BEGIN_TRY

		StringStream msg;

		msg << "GCAddBurrowingCreature("
			<< "ObjectID:" << m_ObjectID 
			<< ",Name:" << m_Name 
			<< ",X:" << (int)m_X 
			<< ",Y:" << (int)m_Y 
			<< ")" ;

		return msg.toString();

		__END_CATCH
	}

#endif
