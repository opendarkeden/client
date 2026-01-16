//////////////////////////////////////////////////////////////////////
//
// Filename    : CRDisconnectHandler.cc
// Written By  : crazydog
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "CRDisconnect.h"
#include "RequestServerPlayer.h"
#include "RequestFunction.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void CRDisconnectHandler::execute ( CRDisconnect * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__

	RequestServerPlayer* pRequestServerPlayer = dynamic_cast<RequestServerPlayer*>( pPlayer );

	if (pRequestServerPlayer!=NULL)
	{
		// 접속된 사용자를 접속해제 시킨다.
		RequestDisconnect( pRequestServerPlayer->getName().c_str() );
	}

#endif

	__END_CATCH
}
