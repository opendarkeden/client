//////////////////////////////////////////////////////////////////////
//
// Filename    : GCRemoveStoreItemHandler.cc
// Written By  : reiot@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCRemoveStoreItem.h"
#include "MStorage.h"
#include "MPlayer.h"
#include "VS_UI.h"


//////////////////////////////////////////////////////////////////////
//
// 클라이언트에서 서버로부터 메시지를 받았을때 실행되는 메쏘드이다.
//
//////////////////////////////////////////////////////////////////////
void GCRemoveStoreItemHandler::execute ( GCRemoveStoreItem * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY 
	//__BEGIN_DEBUG_EX
	
#ifdef __GAME_CLIENT__
 
	if(gC_vs_ui.inventory_mode == 1 || gC_vs_ui.inventory_mode == 2)
	{
		
		if(g_pStorage2 != NULL && g_pStorage2->GetCuropenid() !=NULL)
		{
			if(pPacket->getOwnerObjectID() == g_pStorage2->GetCuropenid())
			{
				const MItem* pStorageItem = g_pStorage2->GetItem( pPacket->getIndex() );
				
				if (pStorageItem!=NULL) 
				{
					g_pStorage2->SetCurrent(0);
					g_pStorage2->RemoveItem(pPacket->getIndex());
				}
			}	
		}
		
	}
	
#endif

	//__END_DEBUG_EX 
	__END_CATCH
}
