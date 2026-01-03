//////////////////////////////////////////////////////////////////////
//
// Filename    : GCBloodBibleStatusHandler.cc
// Written By  : elca
//
//////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
// include files
#include "GCBloodBibleStatus.h"
#include "ClientDef.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////
//
// 클라이언트에서 서버로부터 메시지를 받았을때 실행되는 메쏘드이다.
//
//////////////////////////////////////////////////////////////////////
void GCBloodBibleStatusHandler::execute ( GCBloodBibleStatus * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__
	//cout << pPacket->toString() << endl;
	UI_SetBloodBibleStatus(pPacket);
#endif

	__END_CATCH
}
