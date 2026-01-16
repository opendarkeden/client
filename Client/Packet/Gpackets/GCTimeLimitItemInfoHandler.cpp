//////////////////////////////////////////////////////////////////////
//
// Filename    : GCTimeLimitItemInfoHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCTimeLimitItemInfo.h"
#include "MTimeItemManager.h"
#include "MSlayerGear.h"
#include "MVampireGear.h"
#include "MOustersGear.h"
#include "MPlayer.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCTimeLimitItemInfoHandler::execute ( GCTimeLimitItemInfo * pPacket , Player * pPlayer )
	 throw ( ProtocolException, Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__
		
	if( g_pTimeItemManager != NULL && !pPacket->m_TimeLimitItemInfos.empty())
	{
		GCTimeLimitItemInfo::ItemTimeLimitMap::const_iterator endItr = pPacket->m_TimeLimitItemInfos.end();
		GCTimeLimitItemInfo::ItemTimeLimitMap::const_iterator Itr = pPacket->m_TimeLimitItemInfos.begin();
		
		//MPlayerGear* pGear;
		
		MItemManager* pGear = g_pPlayer->GetGear();

		while( Itr != endItr )
		{
			g_pTimeItemManager->AddTimeItem( Itr->first, Itr->second );
			MItem *pItem = pGear->GetItem( Itr->first );
			if(pItem != NULL)
			{
				pItem->SetAffectStatus();			
				g_pPlayer->SetAddonItem( pItem );
			}
			
			Itr++;
		}

		pPacket->m_TimeLimitItemInfos.clear();
	}
		
#endif

	__END_CATCH
}
