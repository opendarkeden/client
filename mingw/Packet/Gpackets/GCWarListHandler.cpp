//////////////////////////////////////////////////////////////////////
//
// Filename    : GCWarListHandler.cpp
// Written By  : 
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCWarList.h"
#include "MWarManager.h"
#include "MZone.h"
#include "MEventManager.h"

#ifdef __GAME_CLIENT__

void PlayMusicCurrentZone();
extern bool g_bZoneSafe;

#endif

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void GCWarListHandler::execute ( GCWarList * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY 
	
#ifdef __GAME_CLIENT__
	if(g_pWarManager == NULL || g_pZone == NULL)
		return;

	while(1)
	{
		WarInfo *info = pPacket->popWarInfo();
		
		if(info == NULL)
			break;

		g_pWarManager->SetWar(info);
		
		
	}	
	
	// 현재 존도 있는지 검색해야 한다.		
	WarInfo *pInfo = g_pWarManager->GetWarInfo(g_pZone->GetID());
	if(pInfo != NULL && pInfo->getWarType() == WAR_RACE)
	{
		g_bZoneSafe = false;
	}
	
	PlayMusicCurrentZone();
	g_pEventManager->RemoveEvent( EVENTID_WAR_EFFECT );


#endif

	__END_CATCH
}
