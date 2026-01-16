//////////////////////////////////////////////////////////////////////
//
// Filename    : GCDeleteandPickUpOKHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCDeleteandPickUpOK.h"
#include "ClientDef.h"
#include "MItem.h"
#include "MInventory.h"
#include "MMoneyManager.h"
#include "SkillDef.h"
#include "MSkillManager.h"
#include "TempInformation.h"
#include "MQuickSlot.h"
#include "UIFunction.h"

#include "ClientPlayer.h"
#include "packet/Cpackets/CGAddMouseToQuickSlot.h"

// MItem.cpp에 있다.
bool	IsBombMaterial(const MItem* pItem);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCDeleteandPickUpOKHandler::execute ( GCDeleteandPickUpOK * pPacket, Player * pPlayer )
	 throw ( ProtocolException,  Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


	//---------------------------------------------
	// ItemCheckBuffer의 item기억
	//---------------------------------------------
	MItem* pItem = g_pPlayer->GetItemCheckBuffer();

	//---------------------------------------------
	// buffer의 상태
	//---------------------------------------------
	MPlayer::ITEM_CHECK_BUFFER	status = g_pPlayer->GetItemCheckBufferStatus();

	//---------------------------------------------
	// item check buffer를 없애준다.
	//---------------------------------------------
	g_pPlayer->ClearItemCheckBuffer();

	//---------------------------------------------
	// 주울 item이 없다? 뭐지..
	//---------------------------------------------
	if (pItem==NULL)
	{
		DEBUG_ADD_FORMAT("[Error] Pickup Item : no item! [ID=%d]", pPacket->getObjectID());
		
		return;
	}

	ITEM_CLASS itemClass = pItem->GetItemClass();

	BOOL bRemoveZoneItem = FALSE;

	//------------------------------------------------------------------------
	// id가 같은 경우..
	//------------------------------------------------------------------------
	if (pItem->GetID() == pPacket->getObjectID())
	{		
		BOOL bSkillCheck = FALSE;

		// 우리편 성물인경우 바로 제거
//		if (pItem->GetItemClass()==ITEM_CLASS_RELIC// && 
//				(
//					(pItem->GetItemType() == 0 || pItem->GetItemType() == 1 ) && g_pPlayer->IsSlayer() ||
//					(pItem->GetItemType() == 2 || pItem->GetItemType() == 3 ) && g_pPlayer->IsVampire()
//				)
//			)
//		{
//			// sound
//			PlaySound( pItem->GetTileSoundID() );
//
//			// zone에서 제거
//			bRemoveZoneItem = TRUE;
//			//g_pZone->RemoveItem( pItem->GetID() );
//			
//		}

		//------------------------------------------------------------------------
		// 이벤트용 아이템인 경우... 제거한다.
		//------------------------------------------------------------------------
//		else 
		if (pItem->GetItemClass()==ITEM_CLASS_SKULL
			&& pItem->GetItemType()>=12 && pItem->GetItemType()<=16)
		{
			// sound
			PlaySound( pItem->GetInventorySoundID() );

			// zone에서 제거
			bRemoveZoneItem = TRUE;
			//g_pZone->RemoveItem( pItem->GetID() );
		}
		//------------------------------------------------------------------------
		// 돈을 줍는 경우
		//------------------------------------------------------------------------
		else if (status == MPlayer::ITEM_CHECK_BUFFER_PICKUP_MONEY)
		{
			int lowWord = pItem->GetCurrentDurability();
			int highWord = pItem->GetSilver();
			int money = (highWord << 16) | lowWord;

			// 돈제한 limit넘지 않게
			money = min(money, g_pMoneyManager->GetMaxAddMoney());
			
			// 돈 증가
			g_pMoneyManager->AddMoney( money );

			// sound
			PlaySound( pItem->GetTileSoundID() );

			// zone에서 제거
			bRemoveZoneItem = TRUE;
			//g_pZone->RemoveItem( pItem->GetID() );
		}		
		//------------------------------------------------------------------------
		// Item을 Inventory에 넣는 경우
		//------------------------------------------------------------------------
		else if (status == MPlayer::ITEM_CHECK_BUFFER_PICKUP_TO_INVENTORY)
		{
			const MItem* pOldItem = g_pInventory->GetItem( pItem->GetGridX(), pItem->GetGridY() );

			//------------------------------------------------------------------------
			// 빈 공간에 추가되는 경우
			//------------------------------------------------------------------------
			if (pOldItem==NULL)
			{
				//---------------------------------------------
				// item을 inventory에 넣는다.
				//---------------------------------------------
				if (g_pInventory->AddItem( pItem, pItem->GetGridX(), pItem->GetGridY() ))
				{		
					bSkillCheck = TRUE;

					// inventory에 추가됐을 경우만 zone에서 지워준다.
					if (g_pZone==NULL)
					{					
						DEBUG_ADD("[Error] Zone is not Init!");
					}
					else
					{
						// 줍는 소리를 낸다.
						//PlaySound( pItem->GetTileSoundID(),
						//			false,
						//			g_pPlayer->GetX(), g_pPlayer->GetY());

						g_pZone->PickupItem( pItem->GetID() );
					}
				}
				//---------------------------------------------
				// 아니면 뭐지?? item
				//---------------------------------------------
				else
				{
					// 들어갈 자리가 있었는데 없어진 경우..
					DEBUG_ADD_FORMAT("[Error] Pickup Item ID no fit position! ID=%d, xy=(%d, %d)", pItem->GetID(), pItem->GetGridX(), pItem->GetGridY());
				}
			}
			//------------------------------------------------------------------------
			// 기존에 있던 Item에 쌓이는 경우
			//------------------------------------------------------------------------
			else
			{
				//--------------------------------------------------------
				// 쌓일 수 있는 item인지 한번 더 검증해 준다.
				//--------------------------------------------------------
				if (pOldItem->IsPileItem() && pItem->IsPileItem()
					&& pOldItem->GetItemClass()==itemClass
					&& pOldItem->GetItemType()==pItem->GetItemType())
				{
					bSkillCheck = TRUE;

					//----------------------------------------------------
					// pItem을 pOldItem에 쌓는다.
					//----------------------------------------------------
					int total = pOldItem->GetNumber() + pItem->GetNumber();
					if ( total > pOldItem->GetMaxNumber() )
					{
						DEBUG_ADD_FORMAT("[Error] Exceed Item Pile Limit : %d/%d", total, pOldItem->GetMaxNumber());
						
						// max까지만 추가한다고 가정한다.
						total = pItem->GetMaxNumber();
					}
					
					//---------------------------------------------
					// OldItem의 개수를 바꿔서 다시 추가한다.
					//---------------------------------------------
					MItem* pNewItem = g_pInventory->RemoveItem( pOldItem->GetID() );
					pNewItem->SetNumber( total );
					g_pInventory->AddItem( pNewItem, pItem->GetGridX(), pItem->GetGridY() );

					// item을 완전히 제거한다.
					bRemoveZoneItem = TRUE;
					//g_pZone->RemoveItem( pItem->GetID() );
				}
				//--------------------------------------------------------
				// 쌓일 수 없는 item인 경우
				//--------------------------------------------------------
				else
				{
					DEBUG_ADD_FORMAT("[Error] Cannot Pile Items : inv_item_id=%d, zone_item_id=%d", pItem->GetID(), pOldItem->GetID());
				}
			}
		}
		//---------------------------------------------
		// Item을 Mouse에 붙이는 경우
		//---------------------------------------------
		else if (status == MPlayer::ITEM_CHECK_BUFFER_PICKUP_TO_MOUSE)
		{
			bSkillCheck = TRUE;

			UI_PickUpItem( pItem );

			if (g_pZone==NULL)
			{
				DEBUG_ADD("[Error] Zone is not Init!");
			}
			else
			{				
				// 줍는 소리를 낸다.
				//PlaySound( pItem->GetTileSoundID(),
				//			false,
				//			g_pPlayer->GetX(), g_pPlayer->GetY());


				g_pZone->PickupItem( pItem->GetID() );
			}
		}
		//------------------------------------------------------------------------
		// Item을 Quickslot에 넣는 경우
		//------------------------------------------------------------------------
		else if (status == MPlayer::ITEM_CHECK_BUFFER_PICKUP_TO_QUICKSLOT)
		{
			if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_PICKUP_TO_QUICKSLOT)
			{
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);
				int slot = g_pTempInformation->Value1;

				int itemID = pItem->GetID();

				// 냠..
				bSkillCheck = TRUE;	
				
				if (g_pZone==NULL)		// 걍 폼이다 - -;
				{
					DEBUG_ADD("[Error] Zone is not Init!");	// 콩가루~
				}
				else 
				{	
					UI_PickUpItem( pItem );

					// zone에서 줍는다.
					g_pZone->PickupItem( pItem->GetID() );

					//------------------------------------------
					// 다시 Quickslot에 넣는다.
					//------------------------------------------
					if (g_pQuickSlot!=NULL&&g_pPlayer->IsSlayer() || g_pPlayer->IsOusters() &&(
						g_pArmsBand1 != NULL || g_pArmsBand2 != NULL ))
					{
						MItem* pQuickItem = NULL ;//= g_pQuickSlot->GetItem( slot );
						MOustersArmsBand* pQuickSlot = NULL;
						
						if( slot >= 3 )
							pQuickSlot = g_pArmsBand2;
						else
							pQuickSlot = g_pArmsBand1;
						
						if( g_pPlayer->IsSlayer() )
							pQuickItem = g_pQuickSlot->GetItem( slot );
						else
							pQuickItem = pQuickSlot->GetItem( slot % 3 );


						bool bSendPacket = false;

						//--------------------------------------------------------
						// 아무것도 없다면 그냥 넣으면 된다.
						//--------------------------------------------------------
						if (pQuickItem==NULL)
						{
							// mouse --> quickslot
							UI_DropItem();
							
							if( pQuickSlot!= NULL)
							{
								if( !pQuickSlot->AddItem( pItem, slot %3) )
								{
									DEBUG_ADD_FORMAT("[Error] can't add to quickslot. ousters id=%d, slot=%d", pItem->GetID(), slot);
								}
							}else
							if (!g_pQuickSlot->AddItem( pItem, slot ))
							{
								DEBUG_ADD_FORMAT("[Error] can't add to quickslot. id=%d, slot=%d", pItem->GetID(), slot);
							}

							bSendPacket = true;
						}
						//--------------------------------------------------------
						// 뭔가 있는 경우면 쌓일 수 있는 경우일 것이다.
						// 다시 한번 검증해준다.
						//--------------------------------------------------------
						else
						{
							if (pQuickItem->GetItemClass()==pItem->GetItemClass()
								&& pQuickItem->GetItemType()==pItem->GetItemType())
							{
								//----------------------------------------------------
								// 더한 개수가 max를 넘지 않아야 한다.
								//----------------------------------------------------
								int addTotal = pQuickItem->GetNumber() + pItem->GetNumber();
								if ( addTotal <= pQuickItem->GetMaxNumber() )
								{
									UI_DropItem();

									delete pItem;	// 합쳐지므로 제거한다.

									pItem = pQuickItem;	// 아래에서 pItem을 참조하기 때문에..
									
									pQuickItem->SetNumber( addTotal );

									bSendPacket = true;
								}
							}
						}

						if (bSendPacket)
						{
							//------------------------------------------
							// 이거는 검증받지 않아도 된다.
							//------------------------------------------
							CGAddMouseToQuickSlot _CGAddMouseToQuickSlot;
							_CGAddMouseToQuickSlot.setObjectID( itemID );
							_CGAddMouseToQuickSlot.setSlotID( slot );
							
							ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

							pClientPlayer->sendPacket( &_CGAddMouseToQuickSlot );
						}
					}
				}
			}		 
		}
		//---------------------------------------------
		// 다른 경우?
		//---------------------------------------------		
		else
		{
			// 뭐지??
			DEBUG_ADD_FORMAT("[Error] ItemCheckBuffer is not Pickup Status [ID=%d]", pItem->GetID());
		}

		//------------------------------------------------------------------
		// 제대로 가졌을 경우의 처리..
		//------------------------------------------------------------------
		if (bSkillCheck)
		{
			g_pPlayer->CheckAffectStatus(pItem);

			//------------------------------------------------------------------------
			//
			//							Slayer인 경우
			//
			//------------------------------------------------------------------------
			if (g_pPlayer->IsSlayer())
			{
				switch (itemClass)
				{
					//----------------------------------------------------------
					// 성수
					//----------------------------------------------------------
					case ITEM_CLASS_HOLYWATER :
						g_pSkillAvailable->AddSkill( MAGIC_THROW_HOLY_WATER );
					break;

					//----------------------------------------------------------
					// 슬레이어 포탈
					//----------------------------------------------------------
					case ITEM_CLASS_SLAYER_PORTAL_ITEM :
						g_pSkillAvailable->AddSkill( SUMMON_HELICOPTER );
					break;

					//----------------------------------------------------------
					// 폭탄/지뢰 재료
					//----------------------------------------------------------
					case ITEM_CLASS_BOMB_MATERIAL :
					{
						// 0~4는 bomb
						// 5~9는 mine -_-;
						/*
						int itemType = pItem->GetItemType();
						if (IsBombMaterial(pItem))
							g_pSkillAvailable->AddSkill( SKILL_MAKE_BOMB );
						else 
							g_pSkillAvailable->AddSkill( SKILL_MAKE_MINE );
						*/
						// SKILL_INSTALL_MINE에 대한 체크를 해야한다. -_-;
						// g_pSkillAvailable에 각 기술에 대한 체크를 넣어두는게 좋겠다.
						g_pSkillAvailable->SetAvailableSkills();
					}
					break;

					//----------------------------------------------------------
					// 폭탄 / 지뢰
					//----------------------------------------------------------
					case ITEM_CLASS_BOMB :
						g_pSkillAvailable->SetAvailableSkills();
						g_pSkillAvailable->AddSkill( (ACTIONINFO)pItem->GetUseActionInfo(), 
													g_pSkillAvailable->IsEnableSkill(SKILL_THROW_BOMB));
					break;

					case ITEM_CLASS_MINE :
						g_pSkillAvailable->SetAvailableSkills();
						g_pSkillAvailable->AddSkill( (ACTIONINFO)pItem->GetUseActionInfo(),
													g_pSkillAvailable->IsEnableSkill(SKILL_INSTALL_MINE));
					break;
				}
			}
			//----------------------------------------------------------
			//
			//					Vampire인 경우
			//
			//----------------------------------------------------------
			else if (g_pPlayer->IsVampire())
			{
				switch (itemClass)
				{
					//----------------------------------------------------------
					// Vampire Portal Item
					//----------------------------------------------------------
					case ITEM_CLASS_VAMPIRE_PORTAL_ITEM :
						if (((MVampirePortalItem*)pItem)->IsMarked())
						{
							if (g_pSkillAvailable->find( MAGIC_BLOODY_TUNNEL ) != g_pSkillAvailable->end())
							{
								g_pSkillAvailable->AddSkill( MAGIC_BLOODY_TUNNEL );
							}
						}
						else
						{
							if (g_pSkillAvailable->find( MAGIC_BLOODY_MARK ) != g_pSkillAvailable->end())
							{
								g_pSkillAvailable->AddSkill( MAGIC_BLOODY_MARK );
							}
						}
					break;

					//----------------------------------------------------------
					// Vampire ETC (변신 아이템)
					//----------------------------------------------------------
					case ITEM_CLASS_VAMPIRE_ETC :
						if (pItem->GetItemType()==0)
						{
							if (g_pSkillAvailable->find( MAGIC_TRANSFORM_TO_WOLF ) != g_pSkillAvailable->end())
							{
								g_pSkillAvailable->AddSkill( MAGIC_TRANSFORM_TO_WOLF );
							}
						} 
						else if (pItem->GetItemType()==1)
						{
							if (g_pSkillAvailable->find( MAGIC_TRANSFORM_TO_BAT ) != g_pSkillAvailable->end())
							{
								g_pSkillAvailable->AddSkill( MAGIC_TRANSFORM_TO_BAT );
							}
						}
					break;
				}
			}
		}

		//---------------------------------------------
		// Zone에서 Item을 제거해야하는 경우
		//---------------------------------------------
		if (bRemoveZoneItem)
		{
			g_pZone->RemoveItem( pItem->GetID() );
		}
	}
	//---------------------------------------------
	// id가 다른 경우.. 뭐지?? - -;;
	//---------------------------------------------
	else
	{
		DEBUG_ADD_FORMAT("[Error] Pickup Item ID conflict (%d != %d)", pItem->GetID(), pPacket->getObjectID());
	}

//	__BEGIN_HELP_EVENT
		if(itemClass == ITEM_CLASS_BELT)
			ExecuteHelpEvent( HELP_EVENT_GET_BELT );
		else
			ExecuteHelpEvent( HELP_EVENT_GET_ITEM );
//	__END_HELP_EVENT

//	__BEGIN_HELP_EVENT
//		if (status == MPlayer::ITEM_CHECK_BUFFER_PICKUP_MONEY)
//		{
//			// [도움말] 아이템 주울 때
//			ExecuteHelpEvent( HE_ITEM_PICKUP_MONEY );	
//		}
//		else
//		{
//			// [도움말] 아이템 주울 때
//			ExecuteHelpEvent( HE_ITEM_PICKUP );	
//		}
//	__END_HELP_EVENT


#endif

	__END_CATCH
}
