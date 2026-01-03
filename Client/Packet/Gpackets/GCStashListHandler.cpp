////////////////////////////////////////////////////////////////////////////////
// Filename    : GCStashListHandler.cpp
// Written By  : 김성민
// Description :
////////////////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCStashList.h"
#include "ClientDef.h"
#include "MStorage.h"
#include "UIDialog.h"
#include "MGameStringTable.h"
#include "UIFunction.h"

void GCStashListHandler::execute ( GCStashList * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

	int stashNum = pPacket->getStashNum();

	//------------------------------------------------------------
	// 보관함이 있는가?
	//------------------------------------------------------------
	if (stashNum>0)
	{
		//------------------------------------------------------------
		// 기존에걸 지우고 다시 생성한다.
		//------------------------------------------------------------
		if (g_pStorage!=NULL)
		{
			delete g_pStorage;
		}

		g_pStorage = new MStorage;

		
		g_pStorage->Init( stashNum ); //STASH_RACK_MAX );	// 쩝.. 3개일까??

		for (int rack=0; rack<stashNum; rack++)
		{
			//------------------------------------------------------------
			// 접근하는 Storage를 지정한다.
			//------------------------------------------------------------
			g_pStorage->SetCurrent( rack );

			for (int index=0; index<STASH_INDEX_MAX; index++)
			{
				if (pPacket->isExist(rack, index))
				{
					const STASHITEM&	item = pPacket->getStashItem(rack, index);

					//------------------------------------------------------------
					// item을 생성한다.
					//------------------------------------------------------------
					MItem* pItem = MItem::NewItem( (ITEM_CLASS)item.itemClass );

					pItem->SetID( item.objectID );

					pItem->SetItemType( item.itemType );
					pItem->SetItemOptionList( item.optionType );
					pItem->SetCurrentDurability( item.durability );

					//------------------------------------------
					// 개수
					//------------------------------------------
					// 총인 경우
					//------------------------------------------
					if (pItem->IsGunItem())
					{
						MMagazine* pMagazine = (MMagazine*)MItem::NewItem( (ITEM_CLASS)ITEM_CLASS_MAGAZINE );

						// 의미 없음 - -;
						pMagazine->SetID( 0 );

						// 이거는 총에 맞춰서 해줘야된다.
						for (int j=0; j<(*g_pItemTable)[ITEM_CLASS_MAGAZINE].GetSize(); j++)			
						{
							pMagazine->SetItemType(	j );

							if (pMagazine->IsInsertToItem( pItem ))
							{
								break;
							}
						}

						// 의미 없음
						pMagazine->ClearItemOption();
					
						// 탄창 개수
						pMagazine->SetNumber( item.num );

						//------------------------------------
						// 탄창 설정
						//------------------------------------
						MGunItem* pGunItem = (MGunItem*)pItem;
						pGunItem->SetMagazine( pMagazine );
					}		
					//------------------------------------------
					// 총이 아닌 경우
					//------------------------------------------
					else
					{
						pItem->SetNumber( item.num );
						pItem->SetSilver( item.silver );
					}

					pItem->SetGrade( item.grade );
					pItem->SetEnchantLevel( item.enchantLevel );

					//------------------------------------------------------------
					// Sub Item이 있으면 생성한다.
					//------------------------------------------------------------
					int subNum = pPacket->getSubItemCount(rack, index);
					if (subNum!=0)
					{
						//------------------------------------------------------------
						// Belt인 경우
						//------------------------------------------------------------
						if (pItem->GetItemClass()==ITEM_CLASS_BELT)
						{
							MBelt* pBelt = (MBelt*)pItem;

							std::list<SubItemInfo*>& listSubItem = pPacket->getSubItems(rack, index);

							std::list<SubItemInfo*>::const_iterator iItem = listSubItem.begin();

							//------------------------------------------------------------
							// 각각의 sub item을 설정한다.
							//------------------------------------------------------------
							while (iItem != listSubItem.end())
							{
								SubItemInfo* pItemInfo = *iItem;

								if (pItemInfo!=NULL)
								{
									//------------------------------------------------------------
									// sub item을 생성한다.
									//------------------------------------------------------------
									MItem* pSubItem = MItem::NewItem( (ITEM_CLASS)pItemInfo->getItemClass() );

									pSubItem->SetID( pItemInfo->getObjectID() );

									pSubItem->SetItemType( pItemInfo->getItemType() );
									//pSubItem->SetItemOption( 0 );
									//pSubItem->SetItemDurability( item.durability );
									pSubItem->SetNumber( pItemInfo->getItemNum() );

									if (!pBelt->AddItem( pSubItem, pItemInfo->getSlotID() ))
									{
										delete pSubItem;

										DEBUG_ADD_FORMAT("[Error] Can't Add Item to Belt. rack=%d, slot=%d, class=%d, belt-slot=%d", rack, index, (int)pSubItem->GetItemClass(), (int)pItemInfo->getSlotID());
									}
								}

								iItem++;
							}
						} else if (pItem->GetItemClass()==ITEM_CLASS_OUSTERS_ARMSBAND)
						{
							MOustersArmsBand* pBelt = (MOustersArmsBand*)pItem;

							std::list<SubItemInfo*>& listSubItem = pPacket->getSubItems(rack, index);

							std::list<SubItemInfo*>::const_iterator iItem = listSubItem.begin();

							//------------------------------------------------------------
							// 각각의 sub item을 설정한다.
							//------------------------------------------------------------
							while (iItem != listSubItem.end())
							{
								SubItemInfo* pItemInfo = *iItem;

								if (pItemInfo!=NULL)
								{
									//------------------------------------------------------------
									// sub item을 생성한다.
									//------------------------------------------------------------
									MItem* pSubItem = MItem::NewItem( (ITEM_CLASS)pItemInfo->getItemClass() );

									pSubItem->SetID( pItemInfo->getObjectID() );

									pSubItem->SetItemType( pItemInfo->getItemType() );
									//pSubItem->SetItemOption( 0 );
									//pSubItem->SetItemDurability( item.durability );
									pSubItem->SetNumber( pItemInfo->getItemNum() );

									if (!pBelt->AddItem( pSubItem, pItemInfo->getSlotID() ))
									{
										delete pSubItem;

										DEBUG_ADD_FORMAT("[Error] Can't Add Item to Belt. rack=%d, slot=%d, class=%d, belt-slot=%d", rack, index, (int)pSubItem->GetItemClass(), (int)pItemInfo->getSlotID());
									}
								}

								iItem++;
							}
						}
						else
						{
							DEBUG_ADD_FORMAT("[Error] This item can't have subitems. itemClass=%d", (int)pItem->GetItemClass());
						}
					}

					//------------------------------------------------------------
					// Storage에 item 설정
					//------------------------------------------------------------
					if (!g_pStorage->SetItem( index, pItem ))
					{
						// 뭐지..
						delete pItem;

						DEBUG_ADD_FORMAT("[Error] Can't Add Item to Storage. rack=%d, slot=%d", rack, index);
					}
				}
			}
		}
		
		//------------------------------------------------------------
		// 돈 설정
		//------------------------------------------------------------
		g_pStorage->GetMoneyManager()->SetMoney( pPacket->getStashGold() );

		//------------------------------------------------------------
		// 보관함을 띄운다.
		//------------------------------------------------------------
		UI_RunStorage();
		UI_SetStorage(g_pStorage);
	}
	//------------------------------------------------------------
	// 보관함이 없는 경우
	//------------------------------------------------------------
	else
	{
		g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_NO_STORAGE].GetString());
	}
		
#endif

	__END_CATCH
}
