//////////////////////////////////////////////////////////////////////////////
// Filename    : CLSelectServer.cpp 
// Written By  : reiot@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CLSelectServer.h"

void CLSelectServer::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

	iStream.read(m_ServerGroupID);

	__END_CATCH
}

void CLSelectServer::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	oStream.write(m_ServerGroupID);
//	oStream.write((BYTE)4);

	__END_CATCH
}

void CLSelectServer::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	#ifndef __GAME_CLIENT__
		CLSelectServerHandler::execute (this , pPlayer);
	#endif
		
	__END_CATCH
}

