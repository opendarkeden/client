//////////////////////////////////////////////////////////////////////
//
// Filename    : GCMyStoreInfoHandler.cc
// Written By  : reiot@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCMyStoreInfo.h"
#include "MStorage.h"
#include "MPlayer.h"
#include "PCItemInfo.h"
#include "DebugInfo.h"
#include "UIDialog.h"
#include "MGameStringTable.h"
#include "VS_UI.h"
#include "UserOption.h"
#include "ClientConfig.h"
//////////////////////////////////////////////////////////////////////
//
// 클라이언트에서 서버로부터 메시지를 받았을때 실행되는 메쏘드이다.
//
//////////////////////////////////////////////////////////////////////
void GCMyStoreInfoHandler::execute ( GCMyStoreInfo * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	
		__BEGIN_TRY 

		if(g_pPlayer->IsTraceCreature() == TRUE)
			g_pPlayer->TraceNULL();

		if(g_pPlayer->IsNPC() == false)
			g_pPlayer->SetPersnalShop(pPacket->getStoreInfo()->isOpen());

		if(pPacket->getStoreInfo()->isOpen()) // 상점이 열러 있을떄만 
		{
			if(!pPacket->getStoreInfo()->getSign().empty()) 
				g_pPlayer->SetPersnalString((char*)pPacket->getStoreInfo()->getSign().c_str(),g_pUserOption->ChattingColor);
			else
				g_pPlayer->SetPersnalString((*g_pGameStringTable)[UI_STRING_MESSAGE_PERSNAL_DEFAULT_MESSGE].GetString(),g_pUserOption->ChattingColor);
		}
 
		if(gC_vs_ui.GetPersnalShoptoUI() == 3) //상점 닫기
		{
			g_pPlayer->ClearChatString();
			gC_vs_ui.SetPersnalShoptoUI(0);
			gC_vs_ui.ClosePersnalShop(); 

			if (g_pStorage2!=NULL)
			{
				delete g_pStorage2;
				g_pStorage2 = NULL;
			}
			gC_vs_ui.SetPersnalShop( NULL );
			
			return;
		}
		if(gC_vs_ui.GetPersnalShoptoUI() == 2) //상점 열기일경우에 리턴한다.
		{ 
			gC_vs_ui.SetPersnalShoptoUI(0);
			gC_vs_ui.ClosePersnalShop(); 
			return;
		}

 		if(g_pStorage2!=NULL) 
		{
			delete g_pStorage2;
			g_pStorage2 = NULL;
		}
	
		g_pStorage2 = new MStorage;
		g_pStorage2->Init( 1 ); //STASH_RACK_MAX );	// 쩝.. 3개일까?? 
		g_pStorage2->SetCurrent( 0 );

		if(g_pPlayer->GetID() != NULL)
			g_pStorage2->SetCuropenid(g_pPlayer->GetID());


			
		MItem * p_olditem = NULL;
		MItem * p_Newitem = NULL;
		  
		if(strlen(pPacket->getStoreInfo()->getSign().c_str())>0)
			g_pPlayer->SetPersnalShopMessage(pPacket->getStoreInfo()->getSign().c_str());
//			set->SetPersnalShopMessage(pPacket->getStoreInfo()->getSign().c_str());
		for (int rack=0; rack<1; rack++)
		{	 
			//------------------------------------------------------------
			// 접근하는 Storage를 지정한다.
			//------------------------------------------------------------
			//int numitem = pPacket->getStoreInfo().getItems().size();
			int numitem = pPacket->getStoreInfo()->getItems().size();
			for (int index=0; index<numitem; index++)
			{
// 
				StoreItemInfo&	item = pPacket->getStoreInfo()->getStoreItemInfo(index);
				//if(item.isItemExist() == 45)
				if(item.isItemExist() != 0)
				{
					 
					//------------------------------------------------------------
					// item을 생성한다.
					//------------------------------------------------------------
					MItem* pItem = MItem::NewItem( (ITEM_CLASS)item.getItemClass() );

					pItem->SetID( item.getObjectID() );

					pItem->SetItemType( item.getItemType() );
					pItem->SetItemOptionList( item.getOptionType() );
					pItem->SetCurrentDurability( item.getDurability() );
					pItem->SetPersnalPrice(item.getPrice());
					pItem->SetNumber(pPacket->getStoreInfo()->getStoreItemInfo(index).getItemNum());
					
					g_pInventory->SetBegin();
					while (g_pInventory->IsNotEnd())
					{
						MItem * p_item = g_pInventory->Get();
						MItem * p_newitem = g_pInventory->Get();
						
						if(p_item != NULL)
						{
							if(item.getObjectID() == p_item->GetID())
							{
								p_olditem = g_pInventory->GetItem(p_item->GetGridX(),p_item->GetGridY());
								p_Newitem = p_olditem;
								p_Newitem->SetPersnal(true);
								g_pInventory->ReplaceItem(p_Newitem,p_item->GetGridX(),p_item->GetGridY(),p_olditem);
								break;
							}
						}
						g_pInventory->Next();
					}
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
						pItem->SetSilver( item.getSilver() );
					}

					pItem->SetGrade( item.getGrade() );
					pItem->SetEnchantLevel( item.getEnchantLevel() );

					//------------------------------------------------------------
					// Sub Item이 있으면 생성한다.
					//------------------------------------------------------------

					int subNum =item.getListNum();
	
					if (subNum!=0)
					{
						//------------------------------------------------------------
						// Belt인 경우
						//------------------------------------------------------------
						if (pItem->GetItemClass()==ITEM_CLASS_BELT)
						{
							MBelt* pBelt = (MBelt*)pItem;
							std::list<SubItemInfo*>& listSubItem = item.getSubItems();
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
						else if (pItem->GetItemClass()==ITEM_CLASS_OUSTERS_ARMSBAND)
						{
							MOustersArmsBand* pBelt = (MOustersArmsBand*)pItem;

							std::list<SubItemInfo*>& listSubItem = item.getSubItems();

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
					if (!g_pStorage2->SetItem( index, pItem ))
					{
						// 뭐지.. 
						delete pItem;
						
						DEBUG_ADD_FORMAT("[Error] Can't Add Item to Storage. rack=%d, slot=%d", rack, index);
					}

			}
			}
		}
		 
		if(gC_vs_ui.GetPersnalShoptoUI() == 1)
		{
			gC_vs_ui.RunPersnalShop();
			gC_vs_ui.SetPersnalShop( g_pStorage2 ); 		
			return; 
		}
		if(gC_vs_ui.IsRunningPersnalShop()== false && g_pPlayer->CurPernalShop() == false && pPacket->getOpenUI()!=0)
		{
			gC_vs_ui.RunPersnalShop();
			gC_vs_ui.SetPersnalShop( g_pStorage2 ); 		
		}
	__END_CATCH
}

  