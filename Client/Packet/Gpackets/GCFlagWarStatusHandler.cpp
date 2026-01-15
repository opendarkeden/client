//////////////////////////////////////////////////////////////////////
//
// Filename    : GCFlagWarStatusHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCFlagWarStatus.h"
#include "clientdef.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCFlagWarStatusHandler::execute ( GCFlagWarStatus * pGCFlagWarStatus , Player * pPlayer )
	 throw ( Error )
{
#ifdef __GAME_CLIENT__
	DWORD endTime = pGCFlagWarStatus->getTimeRemain() * 1000 + timeGetTime();
	int		flag_s = (int)pGCFlagWarStatus->getFlagCount( RACE_SLAYER );
	int		flag_v = (int)pGCFlagWarStatus->getFlagCount( RACE_VAMPIRE );
	int		flag_o = (int)pGCFlagWarStatus->getFlagCount( RACE_OUSTERS );
	
	// 시간이 3시간 이상이면 무조건 무시!
	if( ((endTime-timeGetTime())/1000)/60/60 > 3 )
		return;

	UI_SetCTFStatus( endTime, flag_s, flag_v, flag_o );
	
#endif
}
