//////////////////////////////////////////////////////////////////////
//
// Filename    : CRRequestHandler.cc
// Written By  : crazydog
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "CRRequest2.h"
#include "ClientDef.h"
#include "UserInformation.h"
#include "ProfileManager.h"
#include "DebugInfo.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void CRRequest2Handler::execute ( CRRequest2 * pPacket )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__ 

	DEBUG_ADD("CRRequest2Handler");

	if ((g_Mode==MODE_GAME
			|| g_Mode==MODE_WAIT_UPDATEINFO			// 로딩 중이 아니거나..
			|| g_Mode==MODE_WAIT_SETPOSITION		// 좌표 기다리는 경우
			)
		&& g_pUserInformation!=NULL)
	{
		DEBUG_ADD("Not NULL");

		switch (pPacket->getCode())
		{
			case CR_REQUEST2_CHARACTER_INFO:
			{
				DEBUG_ADD("Request Character Info");
				
			}
			break;
		}
		
	}
	DEBUG_ADD("CRRequest2Handler End");

#endif

	__END_CATCH
}
