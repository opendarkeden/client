//////////////////////////////////////////////////////////////////////
//
// Filename    : GCDropItemToZoneHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCDropItemToZone.h"
#include "ClientDef.h"
#include "MFakeCreature.h"
#include "UserInformation.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCDropItemToZoneHandler::execute ( GCDropItemToZone * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	#ifdef __GAME_CLIENT__

		DEBUG_ADD_FORMAT("[Drop Item] class=%d, type=%d, optSize=%d, id=%d, xy=(%d,%d)",
											(int)pPacket->getItemClass(), 
											(int)pPacket->getItemType(), 
											(int)pPacket->getOptionTypeSize(), 
											(int)pPacket->getObjectID(), 
											(int)pPacket->getX(), 
											(int)pPacket->getY());
		

		// dropping = TRUE
		AddItemToZone( (GCAddItemToZone*) pPacket, TRUE );		

		TYPE_OBJECTID id = pPacket->getDropPetOID();
		if(id != 0)	// 먼가가 떨궜다-_-
		{
			MCreature *pCreature = g_pZone->GetCreature(id);
			if(pCreature != NULL)
			{
				TYPE_OBJECTID petID = pCreature->GetPetID();

				if(petID != OBJECTID_NULL)
				{
					MFakeCreature *pPet = (MFakeCreature *)(g_pZone->GetFakeCreature(petID));

					if(pPet != NULL)
					{
						pPet->SetAction(ACTION_DRAINED);		// 펫은 흡혈당하기가 펫 짖기이다.

						MPetItem *pPetItem = pPet->GetPetItem();
						if(pPetItem != NULL)
						{
							if(pPetItem->GetItemType()<3) // 2차 펫이 아닐 때
							{
								if(pPetItem->GetNumber() >= 30)
								{
									PlaySound(SOUND_WORLD_DOG_GRRR, false, pPacket->getX(), pPacket->getY());
								}
								else
								{
									PlaySound(SOUND_WORLD_DOG_BARK, false, pPacket->getX(), pPacket->getY());
								}
							}
							else if(pPetItem->GetItemType() == 3) // 슬레이어 펫일 때
							{	// 슬래 펫(탱크)은 루팅할때 사운드가 있다.
								TYPE_SOUNDID soundID = (*g_pCreatureTable)[pPet->GetCreatureType()].GetActionSound( ACTION_DRAINED );
								if (soundID!=SOUNDID_NULL  )
								{
									PlaySound( soundID,
												false,
												pPet->GetX(), pPet->GetY());
								}

							}
						}
					}
				}
			}
		}

	#endif
	__END_CATCH
}
