//////////////////////////////////////////////////////////////////////
//
// Filename    : GCTradeVerifyHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCTradeVerify.h"
#include "ClientDef.h"
#include "TempInformation.h"
#include "MTradeManager.h"
#include "MMoneyManager.h"
#include "UIFunction.h"

#ifdef __GAME_CLIENT__

	#include "ClientPlayer.h"
	#include "Cpackets\CGTradeAddItem.h"
	#include "Cpackets\CGTradeFinish.h"
	#include "cpackets/CGTradeRemoveItem.h"

#endif

void GCTradeVerifyHandler::execute ( GCTradeVerify * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__

	//------------------------------------------------------------------------
	// TradeManager가 생성되지 않은 경우 --> -_-;;
	//------------------------------------------------------------------------
	if (g_pTradeManager==NULL)
	{
		DEBUG_ADD( "[Error] TradeManager is NULL");
		
		return;
	}

	ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

	switch (pPacket->getCode())
	{
		//----------------------------------------------------------------------
		//
		// GCTradeAddItem을 검증 받아야 하는 경우에.. OK
		//
		//----------------------------------------------------------------------
		case GC_TRADE_VERIFY_CODE_ADD_ITEM_OK :
			if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_ADD_ITEM)
			{
				// g_pPlayer->ItemCheckBuffer도 검증해야되는데..생략.. - -;
				g_pPlayer->ClearItemCheckBuffer();

				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				MItem* pItem	= (MItem*)g_pTempInformation->pValue;
				
				pItem->SetTrade();

				//-----------------------------------------------------------
				// 뭔가 바뀐다면... OK취소
				//-----------------------------------------------------------
				g_pTradeManager->RefuseOtherTrade();
				g_pTradeManager->RefuseMyTrade();
			}
		break;

		//----------------------------------------------------------------------
		//
		// GCTradeAddItem을 검증 받아야 하는 경우에.. Fail
		//
		//----------------------------------------------------------------------
		case GC_TRADE_VERIFY_CODE_ADD_ITEM_FAIL :
			if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_ADD_ITEM)
			{
				g_pPlayer->ClearItemCheckBuffer();

				g_pTempInformation->SetMode(TempInformation::MODE_NULL);
			}
		break;

		//----------------------------------------------------------------------
		//
		//		교환 대상으로 아이템을 추가하는 것을 인증
		//
		//----------------------------------------------------------------------
		case GC_TRADE_VERIFY_CODE_ADD_ITEM_WHEN_ACCEPT :
			
			if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_ADD_ITEM)
			{
				// g_pPlayer->ItemCheckBuffer도 검증해야되는데..생략.. - -;
				g_pPlayer->ClearItemCheckBuffer();

				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				MItem* pItem	= (MItem*)g_pTempInformation->pValue;
				
				//-----------------------------------------------------------
				// 추가 확인
				//-----------------------------------------------------------
				pItem->SetTrade();

				//-----------------------------------------------------------
				// 뭔가 바뀐다면... OK취소
				//-----------------------------------------------------------
				g_pTradeManager->RefuseOtherTrade();
				g_pTradeManager->RefuseMyTrade();				
			}
			else
			{
				DEBUG_ADD( "[Error] TempMode is Wrong..");
			}
		break;

		//----------------------------------------------------------------------
		//
		//			교환 대상에서 아이템을 제거하는 것을 인증
		//
		//----------------------------------------------------------------------
		case GC_TRADE_VERIFY_CODE_REMOVE_ITEM :
				
		//----------------------------------------------------------------------
		//
		//					mouse --> inventory OK
		//
		//----------------------------------------------------------------------
		case GC_TRADE_VERIFY_CODE_MOUSE_TO_INVENTORY_OK :	
		
		//----------------------------------------------------------------------
		//
		//					inventory --> mouse OK
		//
		//----------------------------------------------------------------------
		case GC_TRADE_VERIFY_CODE_INVENTORY_TO_MOUSE_OK :
		
			//----------------------------------------------------------------------
			// remove item from trade
			//----------------------------------------------------------------------
			if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_REMOVE_ITEM)
			{
				// g_pPlayer->ItemCheckBuffer도 검증해야되는데..생략.. - -;
				g_pPlayer->ClearItemCheckBuffer();

				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				MItem* pItem	= (MItem*)g_pTempInformation->pValue;
				
				//-----------------------------------------------------------
				// 선택되어 있던 아이템 --> 취소
				//-----------------------------------------------------------
				pItem->UnSetTrade();

				//-----------------------------------------------------------
				// 뭔가 바뀐다면... OK취소
				//-----------------------------------------------------------
				g_pTradeManager->RefuseOtherTrade();
				g_pTradeManager->RefuseMyTrade();				
			}
			//----------------------------------------------------------------------
			// inventory --> mouse 의 검증
			//----------------------------------------------------------------------
			else if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_PICKUP_FROM_INVENTORY)
			{
				// g_pPlayer->ItemCheckBuffer도 검증해야되는데..생략.. - -;
				g_pPlayer->ClearItemCheckBuffer();

				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				MItem* pItem	= (MItem*)g_pTempInformation->pValue;
				int gridX		= g_pTempInformation->Value2;
				int gridY		= g_pTempInformation->Value3;
				MItem* pMouseItem = UI_GetMouseItem();
			
				UI_PickUpItem( pItem );
				g_pTradeManager->GetMyInventory()->RemoveItem( gridX, gridY );

				//----------------------------------------------------------------
				// OK 취소
				//----------------------------------------------------------------				
				g_pTradeManager->RefuseMyTrade();
				g_pTradeManager->RefuseOtherTrade();

				//----------------------------------------------------------------
				// 교환에서 선택된 아이템이면 자동으로 OK가 취소되지만
				// 아닌 아이템은.. 수동으로 OK버튼을 취소하는 packet을 보내야 한다.
				//----------------------------------------------------------------
				if (!pItem->IsTrade())
				{
					CGTradeFinish _CGTradeFinish;
					_CGTradeFinish.setTargetObjectID( g_pTradeManager->GetOtherID() );
					_CGTradeFinish.setCode( CG_TRADE_FINISH_RECONSIDER );

					pClientPlayer->sendPacket( &_CGTradeFinish );

				}			
			}
			//----------------------------------------------------------------
			// 그냥 item 놓을 때
			//----------------------------------------------------------------
			else if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_DROP_TO_INVENTORY)
			{
				// g_pPlayer->ItemCheckBuffer도 검증해야되는데..생략.. - -;
				g_pPlayer->ClearItemCheckBuffer();

				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				// pItem은 원래 들고 있던 item
				MItem* pItem	= (MItem*)g_pTempInformation->pValue;
				int gridX		= g_pTempInformation->Value2;
				int gridY		= g_pTempInformation->Value3;
				MItem* pMouseItem = UI_GetMouseItem();

				// pItem == pMouseItem이다.
				//----------------------------------------------------------------
				// mouse에 item이 있을 때
				//----------------------------------------------------------------
				if (pMouseItem!=NULL)
				{			
					MItem* pOldItem = NULL;

					//----------------------------------------------------------------
					// inventory에 추가시킨다.
					//----------------------------------------------------------------
					if (g_pTradeManager->GetMyInventory()->ReplaceItem(
															pMouseItem,		// 추가할 item
															gridX, gridY,	// 추가할 위치 
															pOldItem))								// 원래있던 item
					{
						if (pOldItem != NULL) // replace 되었는가?
						{
							UI_PickUpItem( pOldItem );						
						}
						else
						{
							UI_DropItem();
						}
					}

					//---------------------------------------------------
					// 놓은 아이템이 교환할려고 선택된 것인 경우
					//---------------------------------------------------
					if (pMouseItem->IsTrade())
					{
						CGTradeAddItem _CGTradeAddItem;
						_CGTradeAddItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
						_CGTradeAddItem.setItemObjectID( pMouseItem->GetID() );

						pClientPlayer->sendPacket( &_CGTradeAddItem );

					}
				}
				else
				{
					// 콩가루
				}

				//----------------------------------------------------------------
				// OK 취소
				//----------------------------------------------------------------
				g_pTradeManager->RefuseMyTrade();
				g_pTradeManager->RefuseOtherTrade();

				//----------------------------------------------------------------
				// 교환에서 선택된 아이템이면 자동으로 OK가 취소되지만
				// 아닌 아이템은.. 수동으로 OK버튼을 취소하는 packet을 보내야 한다.
				//----------------------------------------------------------------
				if (pMouseItem!=NULL && !pMouseItem->IsTrade())
				{
					CGTradeFinish _CGTradeFinish;
					_CGTradeFinish.setTargetObjectID( g_pTradeManager->GetOtherID() );
					_CGTradeFinish.setCode( CG_TRADE_FINISH_RECONSIDER );

					pClientPlayer->sendPacket( &_CGTradeFinish );

				}
			}
			//----------------------------------------------------------------
			// item에 추가될때
			//----------------------------------------------------------------
			else if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_INSERT_TO_INVENTORY)				
			{
				// g_pPlayer->ItemCheckBuffer도 검증해야되는데..생략.. - -;
				g_pPlayer->ClearItemCheckBuffer();

				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				MItem* pItem	= (MItem*)g_pTempInformation->pValue;
				int gridX		= g_pTempInformation->Value2;
				int gridY		= g_pTempInformation->Value3;
				MItem* pMouseItem = UI_GetMouseItem();

				//----------------------------------------------------
				// pMouseItem을 pItem에 추가시킨다.
				//----------------------------------------------------
				int total = pMouseItem->GetNumber() + pItem->GetNumber();
				if ( total > pItem->GetMaxNumber() )
				{
					// 한계 수치를 넘어갈 경우
					pMouseItem->SetNumber( total - pItem->GetMaxNumber() );
					pItem->SetNumber( pItem->GetMaxNumber() );
				}
				else
				{
					// 모두 pItem에 추가될 수 있는 경우
					pItem->SetNumber( total );
					UI_DropItem();

					delete pMouseItem;
				}

				//----------------------------------------------------
				// 교환중에 선택된 아이템인 경우
				//----------------------------------------------------
				// Remove and Add - -;
				if (pItem->IsTrade())
				{
						//----------------------------------------------------
						// 개수가 바껴서 제거했다가..
						//----------------------------------------------------
						CGTradeRemoveItem _CGTradeRemoveItem;
						_CGTradeRemoveItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
						_CGTradeRemoveItem.setItemObjectID( pItem->GetID() );

						pClientPlayer->sendPacket( &_CGTradeRemoveItem );

						#ifdef __DEBUG_OUTPUT__
							DEBUG_ADD_FORMAT( "[Send] %s", _CGTradeRemoveItem.toString().c_str() );							
						#endif
						
						//----------------------------------------------------
						// 다시 추가
						//----------------------------------------------------
						CGTradeAddItem _CGTradeAddItem;
						_CGTradeAddItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
						_CGTradeAddItem.setItemObjectID( pItem->GetID() );

						pClientPlayer->sendPacket( &_CGTradeAddItem );
						
				}
				else
				{
					//----------------------------------------------------------------
					// 교환에서 선택된 아이템이면 자동으로 OK가 취소되지만
					// 아닌 아이템은.. 수동으로 OK버튼을 취소하는 packet을 보내야 한다.
					//----------------------------------------------------------------					
					CGTradeFinish _CGTradeFinish;
					_CGTradeFinish.setTargetObjectID( g_pTradeManager->GetOtherID() );
					_CGTradeFinish.setCode( CG_TRADE_FINISH_RECONSIDER );

					pClientPlayer->sendPacket( &_CGTradeFinish );

				}

				PlaySound( pItem->GetInventorySoundID() );
			}
			//----------------------------------------------------------------
			// 잘못된 mode
			//----------------------------------------------------------------
			else
			{
				DEBUG_ADD( "[Error] TempMode is Wrong..");
			}
		break;


		//----------------------------------------------------------------------
		//
		//			교환 대상에서 돈을 추가하는 것을 인증
		//
		//----------------------------------------------------------------------
		case GC_TRADE_VERIFY_CODE_MONEY_INCREASE :

			if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_ADD_MONEY)
			{
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				int money		= g_pTempInformation->Value1;
			
				//if (!g_pMoneyManager->GetOtherMoneyManager->CanAddMoney(money))
				//{
				//	money = g_pMoneyManager->GetOtherMoneyManager->GetMaxAddMoney();
				//}

				//g_pMoneyManager->UseMoney( money );
				//g_pTradeManager->GetMyMoneyManager()->AddMoney( money );

				//-----------------------------------------------------------
				// 뭔가 바뀐다면... OK취소
				//-----------------------------------------------------------
				g_pTradeManager->RefuseOtherTrade();
				g_pTradeManager->RefuseMyTrade();
			}
			else
			{
				DEBUG_ADD( "[Error] TempMode is Wrong..");
			}
		break;

		//----------------------------------------------------------------------
		//
		//			교환 대상에서 돈을 제거하는 것을 인증
		//
		//----------------------------------------------------------------------
		case GC_TRADE_VERIFY_CODE_MONEY_DECREASE:

			if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_REMOVE_MONEY)
			{
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				int money		= g_pTempInformation->Value1;
				
				//g_pTradeManager->GetMyMoneyManager()->UseMoney( money );
				//g_pMoneyManager->AddMoney( money );	

				//-----------------------------------------------------------
				// 뭔가 바뀐다면... OK취소
				//-----------------------------------------------------------
				g_pTradeManager->RefuseOtherTrade();
				g_pTradeManager->RefuseMyTrade();				
			}
			else
			{
				DEBUG_ADD( "[Error] TempMode is Wrong..");
			}
		break;

		//----------------------------------------------------------------------
		// 교환 성립을 확인하는 것을 인증
		//----------------------------------------------------------------------
		case GC_TRADE_VERIFY_CODE_FINISH_ACCEPT:
			// 검증이 필요없다.
		break;

		//----------------------------------------------------------------------
		//
		//				교환 성립을 취소하는 것을 인증
		//
		//----------------------------------------------------------------------
		case GC_TRADE_VERIFY_CODE_FINISH_REJECT:

			if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_CLOSE)
			{
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				//----------------------------------------------------------------
				// 교환창 닫기
				//----------------------------------------------------------------			
				if (g_pTradeManager!=NULL)
				{
					g_pTradeManager->CancelTrade();
				}

				UI_CloseExchange();
			}
		break;

		//----------------------------------------------------------------------
		//
		//				교환 성립을 재고려하는 것을 인증
		//
		//----------------------------------------------------------------------
		case GC_TRADE_VERIFY_CODE_FINISH_RECONSIDER:

			if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_CANCEL)
			{
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				//----------------------------------------------------------------
				// OK 취소
				//----------------------------------------------------------------			
				g_pTradeManager->RefuseMyTrade();
			}
		break;

		//----------------------------------------------------------------------
		//
		//					mouse --> inventory Fail
		//
		//----------------------------------------------------------------------
		case GC_TRADE_VERIFY_CODE_MOUSE_TO_INVENTORY_FAIL:
			if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_DROP_TO_INVENTORY)
			{
				// g_pPlayer->ItemCheckBuffer도 검증해야되는데..생략.. - -;
				g_pPlayer->ClearItemCheckBuffer();

				g_pTempInformation->SetMode(TempInformation::MODE_NULL);
			}
			else
			{
				DEBUG_ADD( "[Error] TempMode is Not MODE_TRADE_VERIFY_DROP_TO_INVENTORY");
			}
		break;


		//----------------------------------------------------------------------
		//
		//					inventory --> mouse Fail
		//
		//----------------------------------------------------------------------
		case GC_TRADE_VERIFY_CODE_INVENTORY_TO_MOUSE_FAIL:
			if (g_pTempInformation->GetMode()==TempInformation::MODE_TRADE_VERIFY_PICKUP_FROM_INVENTORY)
			{
				// g_pPlayer->ItemCheckBuffer도 검증해야되는데..생략.. - -;
				g_pPlayer->ClearItemCheckBuffer();

				g_pTempInformation->SetMode(TempInformation::MODE_NULL);
			}
			else
			{
				DEBUG_ADD( "[Error] TempMode is Not MODE_TRADE_VERIFY_PICKUP_FROM_INVENTORY");
			}
		break;
	}


#endif

	__END_CATCH
}
