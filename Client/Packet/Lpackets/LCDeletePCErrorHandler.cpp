//----------------------------------------------------------------------
//
// Filename    : LCDeletePCErrorHandler.cpp
// Written By  : Reiot
// Description : 
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "LCDeletePCError.h"
#include "ClientDef.h"

extern void		PopupErrorMessage(ErrorID errorID);

//----------------------------------------------------------------------
// 캐릭터의 삭제가 실패했다는 뜻이다.
// 가만히 생각해보니, 삭제 실패는 크래킹 시도가 아닌가?? - -;
//----------------------------------------------------------------------
void LCDeletePCErrorHandler::execute ( LCDeletePCError * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__
	//#ifdef __DEBUG_OUTPUT__
	//cout << "Fail to delete PC... (" << pPacket->toString() << ")" << endl;
	//throw ProtocolException("DeletePC Failed!");
	
	//UI_DeleteCharacterFailed();

	PopupErrorMessage( (ErrorID)pPacket->getErrorID() );

	g_ModeNext = MODE_WAIT_SELECTPC;

#endif
		
	__END_CATCH
}
