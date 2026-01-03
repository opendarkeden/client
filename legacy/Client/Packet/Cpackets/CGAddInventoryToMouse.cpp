//////////////////////////////////////////////////////////////////////
// 
// Filename    : CGAddInventoryToMouse.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "CGAddInventoryToMouse.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CGAddInventoryToMouse::CGAddInventoryToMouse () 
     throw ()
{
	__BEGIN_TRY

		m_InventoryItemObjectID = 0;
	
	__END_CATCH
}

	
//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CGAddInventoryToMouse::~CGAddInventoryToMouse () 
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void CGAddInventoryToMouse::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	iStream.read( m_ObjectID );
	//modify by viva for notice
	//iStream.read(m_InventoryItemObjectID);
	//end
	iStream.read( m_InvenX );
	iStream.read( m_InvenY );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void CGAddInventoryToMouse::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write( m_ObjectID );
	//modify by viva for notice
	//oStream.write(m_InventoryItemObjectID);
	//end
	oStream.write( m_InvenX  );
	oStream.write( m_InvenY  );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void CGAddInventoryToMouse::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	#ifndef __GAME_CLIENT__
		CGAddInventoryToMouseHandler::execute ( this , pPlayer );
	#endif
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string CGAddInventoryToMouse::toString () 
		const throw ()
	{
		__BEGIN_TRY
			
		StringStream msg;

		msg << "CGAddInventoryToMouse( ObjectID : " << (int)m_ObjectID 
			<< ", InventoryX : " << (int)m_InvenX
			<< ", InventoryY : " << (int)m_InvenY 
			<< ")";

		return msg.toString();

		__END_CATCH
	}

	// global variable definition
	//CGAddInventoryToMouseFactory g_CGAddInventoryToMouseFactory;
#endif
