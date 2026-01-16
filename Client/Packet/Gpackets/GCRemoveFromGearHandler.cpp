//////////////////////////////////////////////////////////////////////
//
// Filename    : GCRemoveFromGearHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCRemoveFromGear.h"
#include "ClientDef.h"
#include "MSlayerGear.h"
#include "MVampireGear.h"
#include "MOustersGear.h"
#include "MGameStringTable.h"
#include "MItemOptionTable.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCRemoveFromGearHandler::execute ( GCRemoveFromGear * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__
	

	int slotID = pPacket->getSlotID();

	//----------------------------------------------------------------------
	// Slayer인 경우
	//----------------------------------------------------------------------
	switch(g_pPlayer->GetRace())
	{
	case RACE_SLAYER:
	{
		MItem* pRemovedItem = g_pSlayerGear->RemoveItem( (MSlayerGear::GEAR_SLAYER)slotID );

		
		if (pRemovedItem==NULL)
		{
			DEBUG_ADD_FORMAT("[Error] No Removed Item in Slot=%d", slotID);
		}
		else
		{
			//----------------------------------------------------------
			// game message 출력
			//----------------------------------------------------------

			if(pRemovedItem->GetItemClass() != ITEM_CLASS_COUPLE_RING && pRemovedItem->GetItemClass() != ITEM_CLASS_VAMPIRE_COUPLE_RING)
			{
				char str[128]; 
				
				if (pRemovedItem->IsEmptyItemOptionList() )
				{
					sprintf(str, "%s %s",					
						pRemovedItem->GetName(),
						(*g_pGameStringTable)[STRING_MESSAGE_ITEM_BROKEN].GetString());
				}
				else
				{
					std::string option_name;
					for(int i = 0; i < pRemovedItem->GetItemOptionListCount(); i++)
					{
						option_name += pRemovedItem->GetItemOptionName(i);
						if(i != pRemovedItem->GetItemOptionListCount())
							option_name += " ";
					}
					sprintf(str, "%s%s %s",					
						option_name.c_str(),	
						pRemovedItem->GetName(),
						(*g_pGameStringTable)[STRING_MESSAGE_ITEM_BROKEN].GetString());
				}
				
				g_pGameMessage->Add(str);
			}
			
			
			
			int addonSlot[] = 
			{
				ADDON_HELM,			//GEAR_SLAYER_HELM = 0,			// 모자
					ADDON_NULL,			//GEAR_SLAYER_NECKLACE,			// 목걸이
					ADDON_COAT,			//GEAR_SLAYER_COAT,				// 상의
					ADDON_LEFTHAND,		//GEAR_SLAYER_LEFTHAND,			// 왼손
					ADDON_RIGHTHAND,	//GEAR_SLAYER_RIGHTHAND,			// 오른손
					ADDON_NULL,			//GEAR_SLAYER_GLOVE,				// 장갑
					ADDON_NULL,			//GEAR_SLAYER_BELT,				// 혁대
					ADDON_TROUSER,		//GEAR_SLAYER_TROUSER,			// 하의			
					ADDON_NULL,			//GEAR_SLAYER_BRACELET1,			// 팔찌1
					ADDON_NULL,			//GEAR_SLAYER_BRACELET2,			// 팔찌2
					ADDON_NULL,			//GEAR_SLAYER_RING1,				// 반지1
					ADDON_NULL,			//GEAR_SLAYER_RING2,				// 반지2
					ADDON_NULL,			//GEAR_SLAYER_RING3,				// 반지3
					ADDON_NULL,			//GEAR_SLAYER_RING4,				// 반지4
					ADDON_NULL,			//GEAR_SLAYER_SHOES,				// 신발
			};
		
			//----------------------------------------------------------
			// 왼손인데.. 왼손이 비었고 오른손에 양손 무기이면..
			// 오른손을 없애야 한다.
			//----------------------------------------------------------
			if (slotID==MSlayerGear::GEAR_SLAYER_LEFTHAND)
			{
				const MItem* pLeftItem = g_pSlayerGear->GetItem( MSlayerGear::GEAR_SLAYER_LEFTHAND );
				const MItem* pRightItem = g_pSlayerGear->GetItem( MSlayerGear::GEAR_SLAYER_RIGHTHAND );

				if (pLeftItem==NULL 
					&& pRightItem!=NULL && pRightItem->IsGearSlotTwoHand())
				{
					slotID = MSlayerGear::GEAR_SLAYER_RIGHTHAND;
				}
			}

			//----------------------------------------------------------
			// 복장을 바꿔준다.
			//----------------------------------------------------------
			int addonSlotID = addonSlot[slotID];

			if (addonSlotID != ADDON_NULL)
			{
				g_pPlayer->SetStop();

				#ifdef	OUTPUT_DEBUG				
					if (g_pPlayer->RemoveAddon( addonSlotID ))
					{
						DEBUG_ADD_FORMAT("[OK] RemoveAddon. Slot=%d", addonSlotID);
					}
					else
					{
						const MCreatureWear::ADDON_INFO& addonInfo = g_pPlayer->GetAddonInfo( addonSlotID );
						DEBUG_ADD_FORMAT("[Error] RemoveAddon. Slot=%d, AddonFrameID=%d", addonSlotID, addonInfo.FrameID);						
					}				
				#else
					g_pPlayer->RemoveAddon( addonSlotID );
				#endif
			}

			// item정보 제거
			UI_RemoveDescriptor( (void*)pRemovedItem );

			//----------------------------------------------------------
			// item을 없앤다.
			//----------------------------------------------------------
			delete pRemovedItem;
		}
	}
	break;

	case RACE_VAMPIRE:
	//----------------------------------------------------------------------
	// Vampire인 경우
	//----------------------------------------------------------------------
	{
		MItem* pRemovedItem = g_pVampireGear->RemoveItem( (MVampireGear::GEAR_VAMPIRE)slotID );

		if (pRemovedItem==NULL)
		{
			DEBUG_ADD_FORMAT("[Error] No Removed Item in Slot=%d", slotID);
		}
		else
		{
			//----------------------------------------------------------
			// game message 출력
			//----------------------------------------------------------
			if(pRemovedItem->GetItemClass() != ITEM_CLASS_COUPLE_RING && pRemovedItem->GetItemClass() != ITEM_CLASS_VAMPIRE_COUPLE_RING)
			{
				char str[128]; 
				
				if (pRemovedItem->IsEmptyItemOptionList())
				{
					sprintf(str, "%s %s",					
						pRemovedItem->GetName(),
						(*g_pGameStringTable)[STRING_MESSAGE_ITEM_BROKEN].GetString());
				}
				else
				{
					std::string option_name;
					for(int i = 0; i < pRemovedItem->GetItemOptionListCount(); i++)
					{
						option_name += pRemovedItem->GetItemOptionName(i);
						if(i != pRemovedItem->GetItemOptionListCount())
							option_name += " ";
					}
					sprintf(str, "%s%s %s",					
						option_name.c_str(),	
						pRemovedItem->GetName(),
						(*g_pGameStringTable)[STRING_MESSAGE_ITEM_BROKEN].GetString());
				}
				
				g_pGameMessage->Add(str);
			}

			int addonSlot[] = 
			{
				ADDON_NULL,			//GEAR_VAMPIRE_NECKLACE,			// 목걸이
				ADDON_COAT,			//GEAR_VAMPIRE_COAT,				// 상의
				ADDON_NULL,			//GEAR_VAMPIRE_BRACELET1,			// 팔찌1
				ADDON_NULL,			//GEAR_VAMPIRE_BRACELET2,			// 팔찌2
				ADDON_NULL,			//GEAR_VAMPIRE_RING1,				// 반지1
				ADDON_NULL,			//GEAR_VAMPIRE_RING2,				// 반지2
				ADDON_NULL,			//GEAR_VAMPIRE_RING3,				// 반지3
				ADDON_NULL,			//GEAR_VAMPIRE_RING4,				// 반지4
				ADDON_NULL,			//GEAR_VAMPIRE_EARRING1,			// 귀걸이1
				ADDON_NULL,			//GEAR_VAMPIRE_EARRING2,			// 귀걸이2
				ADDON_NULL,			//GEAR_VAMPIRE_WEAPON1,				// 무기1
				ADDON_NULL,			//GEAR_VAMPIRE_WEAPON2,				// 무기2
				ADDON_NULL,			//GEAR_VAMPIRE_AMULET1,				// 아뮬렛1
				ADDON_NULL,			//GEAR_VAMPIRE_AMULET2,				// 아뮬렛2
				ADDON_NULL,			//GEAR_VAMPIRE_AMULET3,				// 아뮬렛3
				ADDON_NULL,			//GEAR_VAMPIRE_AMULET4,				// 아뮬렛4
			};

			//----------------------------------------------------------
			// 왼손인데.. 왼손이 비었고 오른손에 양손 무기이면..
			// 오른손을 없애야 한다.
			//----------------------------------------------------------
			if (slotID==MVampireGear::GEAR_VAMPIRE_LEFTHAND)
			{
				const MItem* pLeftItem = g_pVampireGear->GetItem( MVampireGear::GEAR_VAMPIRE_LEFTHAND );
				const MItem* pRightItem = g_pVampireGear->GetItem( MVampireGear::GEAR_VAMPIRE_RIGHTHAND );

				if (pLeftItem==NULL 
					&& pRightItem!=NULL && pRightItem->IsGearSlotTwoHand())
				{
					slotID = MVampireGear::GEAR_VAMPIRE_RIGHTHAND;
				}
			}

			//----------------------------------------------------------
			// 복장을 바꿔준다.
			//----------------------------------------------------------
			int addonSlotID = addonSlot[slotID];

			if (addonSlotID != ADDON_NULL)
			{
				g_pPlayer->SetStop();

				#ifdef	OUTPUT_DEBUG				
					if (g_pPlayer->RemoveAddon( addonSlotID ))
					{
						DEBUG_ADD_FORMAT("[OK] RemoveAddon. Slot=%d", addonSlotID);
					}
					else
					{
						const MCreatureWear::ADDON_INFO& addonInfo = g_pPlayer->GetAddonInfo( addonSlotID );
						DEBUG_ADD_FORMAT("[Error] RemoveAddon. Slot=%d, AddonFrameID=%d", addonSlotID, addonInfo.FrameID);						
					}				
				#else
					g_pPlayer->RemoveAddon( addonSlotID );
				#endif
			}

			// item정보 제거
			UI_RemoveDescriptor( (void*)pRemovedItem );

			//----------------------------------------------------------
			// item을 없앤다.
			//----------------------------------------------------------
			delete pRemovedItem;
		}
	}
	break;

	case RACE_OUSTERS:
	{
		MItem* pRemovedItem = g_pOustersGear->RemoveItem( (MOustersGear::GEAR_OUSTERS)slotID );

		if (pRemovedItem==NULL)
		{
			DEBUG_ADD_FORMAT("[Error] No Removed Item in Slot=%d", slotID);
		}
		else
		{
			//----------------------------------------------------------
			// game message 출력
			//----------------------------------------------------------
			if(1)
			{
				char str[128]; 
				
				if (pRemovedItem->IsEmptyItemOptionList())
				{
					sprintf(str, "%s %s",					
						pRemovedItem->GetName(),
						(*g_pGameStringTable)[STRING_MESSAGE_ITEM_BROKEN].GetString());
				}
				else
				{
					std::string option_name;
					for(int i = 0; i < pRemovedItem->GetItemOptionListCount(); i++)
					{
						option_name += pRemovedItem->GetItemOptionName(i);
						if(i != pRemovedItem->GetItemOptionListCount())
							option_name += " ";
					}
					sprintf(str, "%s%s %s",					
						option_name.c_str(),	
						pRemovedItem->GetName(),
						(*g_pGameStringTable)[STRING_MESSAGE_ITEM_BROKEN].GetString());
				}
				
				g_pGameMessage->Add(str);
			}

			int addonSlot[] = 
			{
				ADDON_NULL,			//GEAR_OUSTERS_CIRCLET,			// 서클릿
				ADDON_COAT,			//GEAR_OUSTERS_COAT,				// 옷
				ADDON_LEFTHAND,		//GEAR_OUSTERS_LEFTHAND,			// 왼손
				ADDON_RIGHTHAND,	//GEAR_OUSTERS_RIGHTHAND,			// 오른손
				ADDON_TROUSER,			//GEAR_OUSTERS_BOOTS,				// 신발
				ADDON_NULL,			//GEAR_OUSTERS_ARMSBAND1,			// 암스밴드1
				ADDON_NULL,			//GEAR_OUSTERS_ARMSBAND2,			// 암스밴드2
				ADDON_NULL,			//GEAR_OUSTERS_RING1,				// 링1
				ADDON_NULL,			//GEAR_OUSTERS_RING2,				// 링2
				ADDON_NULL,			//GEAR_OUSTERS_NECKLACE1,			// 목걸이1
				ADDON_NULL,			//GEAR_OUSTERS_NECKLACE2,			// 목걸이2
				ADDON_NULL,			//GEAR_OUSTERS_NECKLACE3,			// 목걸이3
				ADDON_NULL,			//GEAR_OUSTERS_STONE1,			// 정령석1
				ADDON_NULL,			//GEAR_OUSTERS_STONE2,			// 정령석2
				ADDON_NULL,			//GEAR_OUSTERS_STONE3,			// 정령석3
				ADDON_NULL,			//GEAR_OUSTERS_STONE4,			// 정령석4
			};

			//----------------------------------------------------------
			// 왼손인데.. 왼손이 비었고 오른손에 양손 무기이면..
			// 오른손을 없애야 한다.
			//----------------------------------------------------------
			if (slotID==MOustersGear::GEAR_OUSTERS_LEFTHAND)
			{
				const MItem* pLeftItem = g_pOustersGear->GetItem( MOustersGear::GEAR_OUSTERS_LEFTHAND );
				const MItem* pRightItem = g_pOustersGear->GetItem( MOustersGear::GEAR_OUSTERS_RIGHTHAND );

				if (pLeftItem==NULL 
					&& pRightItem!=NULL && pRightItem->IsGearSlotTwoHand())
				{
					slotID = MOustersGear::GEAR_OUSTERS_RIGHTHAND;
				}
			}

			//----------------------------------------------------------
			// 복장을 바꿔준다.
			//----------------------------------------------------------
			int addonSlotID = addonSlot[slotID];

			if (addonSlotID != ADDON_NULL)
			{
				g_pPlayer->SetStop();

				#ifdef	OUTPUT_DEBUG				
					if (g_pPlayer->RemoveAddon( addonSlotID ))
					{
						DEBUG_ADD_FORMAT("[OK] RemoveAddon. Slot=%d", addonSlotID);
					}
					else
					{
						const MCreatureWear::ADDON_INFO& addonInfo = g_pPlayer->GetAddonInfo( addonSlotID );
						DEBUG_ADD_FORMAT("[Error] RemoveAddon. Slot=%d, AddonFrameID=%d", addonSlotID, addonInfo.FrameID);						
					}				
				#else
					g_pPlayer->RemoveAddon( addonSlotID );
				#endif
			}

			// item정보 제거
			UI_RemoveDescriptor( (void*)pRemovedItem );

			//----------------------------------------------------------
			// item을 없앤다.
			//----------------------------------------------------------
			delete pRemovedItem;
		}
	}
	break;
	}

//	// [도움말] 아이템이 부서진 경우
//	__BEGIN_HELP_EVENT
////		ExecuteHelpEvent( HE_ITEM_BROKEN );
//	__END_HELP_EVENT

#endif

	__END_CATCH
}
