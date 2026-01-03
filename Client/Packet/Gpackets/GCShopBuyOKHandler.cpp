//////////////////////////////////////////////////////////////////////
//
// Filename    : GCShopBuyOKHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCShopBuyOK.h"
#include "ClientDef.h"
#include "MShop.h"
#include "MShopShelf.h"
#include "MInventory.h"
#include "TempInformation.h"
#include "MMoneyManager.h"
#include "MPriceManager.h"
#include "MItemFinder.h"
#include "UIFunction.h"

#define SAFE_DELETE(x)		{ if(x!=NULL) delete x; x=NULL; }

// PacketFunction.cpp에 있다. compile 시간 관계상..
void	CheckItemForSkillIcon(const MItem* pItem);

void GCShopBuyOKHandler::execute ( GCShopBuyOK * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

	//--------------------------------------------------------------
	// Item 사는 packet을 받는게 맞나?
	//--------------------------------------------------------------
	if (g_pTempInformation->GetMode() == TempInformation::MODE_SHOP_BUY)
	{
		MShop* pShop = (MShop*)g_pTempInformation->pValue;		
		int ShelfType	= g_pTempInformation->Value1;
		int index		= g_pTempInformation->Value2;
		int x			= g_pTempInformation->Value3;
		int y			= g_pTempInformation->Value4;

		DEBUG_ADD_FORMAT("[BuyOK] TempInfo. ShelfType=%d, index=%d, xy=(%d, %d)", ShelfType, index, x, y);
		
		
		MShopShelf* pShopShelf = pShop->GetShelf( ShelfType );

		//--------------------------------------------------------------
		// 상점에서 item을 제거한다.			
		//--------------------------------------------------------------
		if (pShopShelf!=NULL)
		{
			//------------------------------------------------------
			// Normal shelf가 아닌 경우에만 delete한다.
			//------------------------------------------------------
			if (ShelfType==SHOP_RACK_SPECIAL)
			{
				pShopShelf->DeleteItem( index );
			}

			// 상점의 Version을 바꾼다.
			pShopShelf->SetVersion( pPacket->getShopVersion() );
		}
		else
		{
			DEBUG_ADD_FORMAT("[Error] No Shelf.. type=%d", ShelfType);
		}

		//--------------------------------------------------------------
		// 방금 구입한 item을 생성한다.
		//--------------------------------------------------------------		
		MItem* pItem = MItem::NewItem( (ITEM_CLASS)pPacket->getItemClass() );

		pItem->SetID( pPacket->getItemObjectID() );
		pItem->SetItemType( pPacket->getItemType() );
		pItem->SetItemOptionList( pPacket->getOptionType() );
		pItem->SetNumber( pPacket->getItemNum() );	// 이미 쌓여진 개수
		pItem->SetCurrentDurability( pPacket->getDurability() );
		pItem->SetSilver( pPacket->getSilver() );
		pItem->SetGrade( pPacket->getGrade() );
		pItem->SetEnchantLevel( pPacket->getEnchantLevel() );

		const MItem* pOldItem = g_pInventory->GetItem( x, y );

		//--------------------------------------------------------------		
		// 그 위치에 아무것도 없는 경우 --> 그냥 추가하면 된다.
		//--------------------------------------------------------------				
		if (pOldItem==NULL)
		{				
		}
		//--------------------------------------------------------------		
		// 쌓일 수 있는지 체크한다.
		//--------------------------------------------------------------				
		else
		{
			if (pItem->IsInsertToItem( pOldItem ))
			{
				int total = //pOldItem->GetNumber() + 
							pItem->GetNumber();
				
				//------------------------------------------------
				// 개수 초과					
				//------------------------------------------------
				if ( total > pItem->GetMaxNumber() )
				{
					DEBUG_ADD_FORMAT("[Error] Cannot Add. MaxNum exceed=%d", total);
				}
				//------------------------------------------------
				// 정상적으로 쌓여질 수 있는 경우					
				//------------------------------------------------
				else
				{
					//pItem->SetNumber( total );

					// 기존에 있던 item을 제거한다.
					MItem* pRemovedItem = g_pInventory->RemoveItem( x, y );
					
					SAFE_DELETE( pRemovedItem );

//					if (pRemovedItem!=NULL)
//					{
//						delete pRemovedItem;
//					}
				}
			}
			else
			{
				DEBUG_ADD("[Error] Cannot Add. No match item");
			}
		}

		//--------------------------------------------------------------
		// item을 inventory에 추가한다.
		//--------------------------------------------------------------
		if (g_pInventory->AddItem( pItem, x, y ))
		{
			// 제대로 추가된 경우 --> sound출력
			PlaySound( pItem->GetTileSoundID() );

			// skill icon 체크
			CheckItemForSkillIcon( pItem );
		}
		else
		{
			DEBUG_ADD_FORMAT("[Error] Cannot Add to Inventory(%d,%d)", x,y);
			
			// 추가가 안되는 경우 지워야 한다.
			delete pItem;
		}
	
		//--------------------------------------------------------------
		// 상점에 따라서 뭘로 샀는가?..
		//--------------------------------------------------------------
		switch (pShop->GetShopType())
		{
			//--------------------------------------------------------------
			// 돈
			//--------------------------------------------------------------
			case MShop::SHOP_NORMAL :
				//--------------------------------------------------------------
				// 돈을 바꿔준다.
				//--------------------------------------------------------------
				if (!g_pMoneyManager->SetMoney( pPacket->getPrice() ))
				{
					DEBUG_ADD_FORMAT("[Error] Can't Set Money=%d, Price=%d", g_pMoneyManager->GetMoney(), pPacket->getPrice());
				}
			break;

			//--------------------------------------------------------------
			// 별
			//--------------------------------------------------------------
			case MShop::SHOP_EVENT_STAR :
				// 적절한 개수만큼을 inventory에서 지워준다.
				if (pItem!=NULL)
				{
					STAR_ITEM_PRICE starPrice;

					g_pPriceManager->GetItemPrice( pItem, starPrice );

					int remainNum = starPrice.number;

					if (starPrice.type!=-1 && starPrice.number!=0)
					{
						// 몇개나 있는지 찾아본다.
						MItemClassTypeFinder starFinder(ITEM_CLASS_EVENT_STAR, starPrice.type);

						// 개수만큼 inventory에서 제거한다.
						while (remainNum > 0)
						{
							MItem* pStarItem = g_pInventory->FindItemGridOrder( starFinder );

							// 별이 없는 경우 - -;
							if (pStarItem==NULL)
							{
								DEBUG_ADD("[Error] Not Enough Star -_-");
								break;
							}

							int itemNum = pStarItem->GetNumber();

							if (itemNum > remainNum)
							{
								pStarItem->SetNumber( itemNum - remainNum );
								remainNum = 0;
							}
							else
							{
								// 같거나 적은 경우
								remainNum -= itemNum;

								MItem* pRemovedItem = g_pInventory->RemoveItem( 
																pStarItem->GetGridX(), 
																pStarItem->GetGridY() );

								if (pRemovedItem!=NULL)
								{
									delete pRemovedItem;
								}
								else
								{
									DEBUG_ADD_FORMAT("[Error] Can't remove star(%d, %d)", pStarItem->GetGridX(), pStarItem->GetGridY());
								}
							}
						}
					}
				}
			break;
		}

		
		// mode를 없앤다.
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);

		// 거래를 다시 활성화한다.
		UI_UnlockItemTrade();

		//--------------------------------------------------------------
		// 오토바이 (열쇠를) 산 경우
		//--------------------------------------------------------------
//		__BEGIN_HELP_EVENT
//			if (pItem->GetItemClass()==ITEM_CLASS_KEY 
//				&& pItem->GetItemType()==2)
//			{
//				// [도움말] 오토바이 산 경우
////				ExecuteHelpEvent( HE_ITEM_BUY_MOTORCYCLE );
//			}
//		__END_HELP_EVENT
	}
	else
	{
		DEBUG_ADD("[Error] Buy? -_-; No Temp Information!");
	}

#endif

	__END_CATCH
}
