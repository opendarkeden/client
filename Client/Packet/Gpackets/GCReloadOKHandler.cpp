//////////////////////////////////////////////////////////////////////
//
// Filename    : GCReloadOKHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCReloadOK.h"
#include "ClientDef.h"
#include "MSlayerGear.h"
#include "MInventory.h"
#include "MQuickSlot.h"
#include "UIFunction.h"

#define SAFE_DELETE(x)		{ if(x!=NULL) delete x; x=NULL; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCReloadOKHandler::execute ( GCReloadOK * pPacket , Player * pPlayer )
	 throw ( ProtocolException, Error )
{
	__BEGIN_TRY
		
#if __GAME_CLIENT__
	// message

	//------------------------------------------------------------------
	// Slayer인 경우만 적용되는 packet이다.
	//------------------------------------------------------------------
	if (g_pPlayer->IsVampire())
	{
		DEBUG_ADD("[Error] Player is Vampire. No Effect!");

		return;
	}

	// 음.. cut & paste의 걸작.. - -;;

	//------------------------------------------------------------------
	//
	//				Item Check Buffer 확인
	//
	//------------------------------------------------------------------	
	MItem* pItem = g_pPlayer->GetItemCheckBuffer();

	//----------------------------------------------------
	// Check Buffer에 item이 있는 경우
	//----------------------------------------------------
	if (pItem!=NULL)
	{
		PlaySound( pItem->GetUseSoundID() );

		MPlayer::ITEM_CHECK_BUFFER status =	g_pPlayer->GetItemCheckBufferStatus();

		//----------------------------------------------------
		// Inventory에서 사용
		//----------------------------------------------------
		if (status==MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY)			
		{
			// Item Check Buffer를 지운다.
			g_pPlayer->ClearItemCheckBuffer();

			//----------------------------------------------------
			// MAGAZINE
			//----------------------------------------------------
			if (pItem->GetItemClass()==ITEM_CLASS_MAGAZINE)
			{
				MMagazine* pMagazineItem;

				//-------------------------------------------------
				// 개수 체크를 해야한다.
				//-------------------------------------------------
				if (pItem->GetNumber()>1)
				{
					// 아직 개수가 더 남아 있는 경우,
					// 개수를 하나 줄여준다.
					pItem->SetNumber( pItem->GetNumber() - 1 );

					//-------------------------------------------------
					// 새로운 탄창을 생성해서 총에 추가해야 한다.
					//-------------------------------------------------
					pMagazineItem = (MMagazine*)MItem::NewItem( ITEM_CLASS_MAGAZINE );

					pMagazineItem->SetID( 0 );	// 임의의 ID.. 사용하지 않을 듯.. 
					pMagazineItem->SetItemType( pItem->GetItemType() );
					pMagazineItem->SetItemOptionList( pItem->GetItemOptionList() );
				}
				//-------------------------------------------------
				// 다 사용한 경우 --> 제거한다.
				//-------------------------------------------------
				else
				{
					// inventory에서 제거
					g_pInventory->RemoveItem( pItem->GetID() );

					// memory에서 제거하면 안된다 --> 총에 들어가므로..
					//delete pItem;	
					
					pMagazineItem = (MMagazine*)pItem;
				}

				
				//----------------------------------------------------
				// 총알 개수를 다시 설정한다.
				//----------------------------------------------------
				// Magazine.GetNumber()
				//
				// 탄창으로 있을 때는 탄창 쌓인 개수
				// 총에 들어 있을 때 Number는 총알의 개수.
				//----------------------------------------------------
				pMagazineItem->SetNumber( pPacket->getBulletNum() );

				// Slayer인 경우만 가능하다.
				const MItem* pRightHandItem = g_pSlayerGear->GetItem( (MSlayerGear::GEAR_SLAYER)MSlayerGear::GEAR_SLAYER_RIGHTHAND );

				//----------------------------------------------------
				// 오른손에 총을 들고 있는 경우이면
				//----------------------------------------------------
				if (pRightHandItem!=NULL)
				{
					if (pRightHandItem->IsGunItem())
					{
						MGunItem* pGunItem = (MGunItem*)pRightHandItem;

						// 탄창 장착
						MMagazine* pOldMagazine = pGunItem->SetMagazine( pMagazineItem );

						// 현재의 탄창 설정
						g_pCurrentMagazine = pMagazineItem;

						// 기존에 있던 탄창이 있으면 제거한다.
						if (pOldMagazine!=NULL)
						{
							DEBUG_ADD("Delete Old Magazine");
							
							delete pOldMagazine;
						}
					}									
				}
				//----------------------------------------------------
				// 총이 아닌 경우 --> Error당..
				//----------------------------------------------------
				else
				{
					DEBUG_ADD_FORMAT("[Error] Player not Use GUN. the ItemClass=%d", (int)pRightHandItem->GetItemClass());
					
					// inventory에서 제거
					g_pInventory->RemoveItem( pItem->GetID() );

					// item정보 제거 - 의미잇을까 - -;
					UI_RemoveDescriptor( (void*)pItem );

					delete pItem;
				}
			}
			//----------------------------------------------------
			// 탄창이 아니면 에러다.
			//----------------------------------------------------
			else
			{
				DEBUG_ADD_FORMAT("[Error] Item is Not Magazine: the ItemClass=%d", (int)pItem->GetItemClass());
			}
		}
		//----------------------------------------------------
		// QuickSlot에서 사용
		//----------------------------------------------------
		else if (status==MPlayer::ITEM_CHECK_BUFFER_USE_FROM_QUICKSLOT)
		{
			// Item Check Buffer를 지운다.
			g_pPlayer->ClearItemCheckBuffer();

			//----------------------------------------------------
			// MAGAZINE
			//----------------------------------------------------
			if (pItem->GetItemClass()==ITEM_CLASS_MAGAZINE)
			{
				MMagazine* pMagazineItem;

				//-------------------------------------------------
				// 개수 체크를 해야한다.
				//-------------------------------------------------
				if (pItem->GetNumber()>1)
				{
					// 아직 개수가 더 남아 있는 경우,
					// 개수를 하나 줄여준다.
					pItem->SetNumber( pItem->GetNumber() - 1 );

					//-------------------------------------------------
					// 새로운 탄창을 생성해서 총에 추가해야 한다.
					//-------------------------------------------------
					pMagazineItem = (MMagazine*)MItem::NewItem( ITEM_CLASS_MAGAZINE );

					pMagazineItem->SetID( 0 );	// 임의의 ID.. 사용하지 않을 듯.. 
					pMagazineItem->SetItemType( pItem->GetItemType() );
					pMagazineItem->SetItemOptionList( pItem->GetItemOptionList() );					
				}
				//-------------------------------------------------
				// 다 사용한 경우 --> 제거한다.
				//-------------------------------------------------
				else
				{
					// QuickSlot에서 제거
					g_pQuickSlot->RemoveItem( pItem->GetID() );

					// memory에서 제거하면 안된다 --> 총에 들어가므로..
					//delete pItem;	
					
					pMagazineItem = (MMagazine*)pItem;
				}

				//----------------------------------------------------
				// 총알 개수를 다시 설정한다.
				//----------------------------------------------------
				pMagazineItem->SetNumber( pPacket->getBulletNum() );

				// Slayer인 경우만 가능하다.
				const MItem* pRightHandItem = g_pSlayerGear->GetItem( (MSlayerGear::GEAR_SLAYER)MSlayerGear::GEAR_SLAYER_RIGHTHAND );

				//----------------------------------------------------
				// 오른손에 총을 들고 있는 경우이면
				//----------------------------------------------------
				if (pRightHandItem!=NULL)
				{
					if (pRightHandItem->IsGunItem())
					{
						MGunItem* pGunItem = (MGunItem*)pRightHandItem;

						// 탄창 장착
						MMagazine* pOldMagazine = pGunItem->SetMagazine( pMagazineItem );

						// 현재의 탄창 설정
						g_pCurrentMagazine = pMagazineItem;

						// 기존에 있던 탄창이 있으면 제거한다.
						if (pOldMagazine!=NULL)
						{
							DEBUG_ADD("Delete Old Magazine");
							
							delete pOldMagazine;
						}
					}									
				}
				//----------------------------------------------------
				// 총이 아닌 경우 --> Error당..
				//----------------------------------------------------
				else
				{
					DEBUG_ADD_FORMAT("[Error] Player not Use GUN. the ItemClass=%d", (int)pRightHandItem->GetItemClass());
					
					// QuickSlot에서 제거
					g_pQuickSlot->RemoveItem( pItem->GetID() );

					SAFE_DELETE( pItem );					
				}
			}
			//----------------------------------------------------
			// 탄창이 아니면 에러다.
			//----------------------------------------------------
			else
			{
				DEBUG_ADD_FORMAT("[Error] Item is Not Magazine: the ItemClass=%d", (int)pItem->GetItemClass());
			}
		}
		//----------------------------------------------------
		// 다른 상태??
		//----------------------------------------------------
		else
		{
			DEBUG_ADD_FORMAT("[Error] ItemCheck Buffer is not Use Status: status=%d", (int)status);
			
			return;
		}

		// 은총알 때문에... 데미지 등등.. 다시 계산..
		g_pPlayer->CalculateStatus();

	}
	//----------------------------------------------------
	// item이 없는 경우.. - -;;
	//----------------------------------------------------
	else
	{
		DEBUG_ADD("[Error] No Item in CheckBuffer");
		
		return;
	}


#endif

	__END_CATCH
}
