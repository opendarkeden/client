//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCDeleteEffectFromTile.cpp 
// Written By  : crazydog
// Description : 
//               
// 
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "GCDeleteEffectFromTile.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
GCDeleteEffectFromTile::GCDeleteEffectFromTile () 
     throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

	
//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
GCDeleteEffectFromTile::~GCDeleteEffectFromTile () 
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void GCDeleteEffectFromTile::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	// 최적화 작업시 실제 크기를 명시하도록 한다.
	iStream.read( m_ObjectID);
	iStream.read( m_X);
	iStream.read( m_Y);
	iStream.read( m_EffectID);

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void GCDeleteEffectFromTile::write ( SocketOutputStream & oStream ) 
     const throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	// 최적화 작업시 실제 크기를 명시하도록 한다.
	oStream.write( m_ObjectID);
	oStream.write( m_X);
	oStream.write( m_Y);
	oStream.write( m_EffectID);
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// execute packet's handler
//
//////////////////////////////////////////////////////////////////////
void GCDeleteEffectFromTile::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	GCDeleteEffectFromTileHandler::execute( this , pPlayer );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// get packet's debug std::string
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string GCDeleteEffectFromTile::toString () 
		const throw ()
	{
		__BEGIN_TRY

		StringStream msg;

		msg << "GCDeleteEffectFromTile( "
			<< ",ObjectID: " << (int)m_ObjectID
			<< ",X, Y: " << (int)m_X<<","<<(int)m_Y
			<< ",EffectID: " << (int)m_EffectID;


		msg << ")";

		return msg.toString();

		__END_CATCH
	}
#endif
