//--------------------------------------------------------------------------------
//
// Filename    : GCAddItemToItemVerifyHandler.cpp
// Written By  : elca, Reiot
// Description :
//
//--------------------------------------------------------------------------------
#include "Client_PCH.h"
// include files
#include "GCAddItemToItemVerify.h"
#include "ClientDef.h"
#include "TempInformation.h"
#include "MInventory.h"
#include "UIDialog.h"
#include "MGameStringTable.h"
#include "ClientFunction.h"
#include "SkillDef.h"
#include "MinTR.H"
#include "UIFunction.h"

#ifdef __GAME_CLIENT__
	#include "ClientPlayer.h"
#endif

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCAddItemToItemVerifyHandler::execute ( GCAddItemToItemVerify * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY 
	__BEGIN_DEBUG
		
#ifdef __GAME_CLIENT__
	MItem *pInventoryItem = NULL;

	MItem *pItem[2] = { NULL, NULL };

	if(g_pTempInformation->GetMode() == TempInformation::MODE_ITEM_TO_ITEM)
	{
		pInventoryItem = g_pInventory->GetItem(g_pTempInformation->Value1, g_pTempInformation->Value2);
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
	} else if( g_pTempInformation->GetMode() == TempInformation::MODE_ITEM_MIXING )
	{
		pItem[0] = g_pInventory->GetItem( g_pTempInformation->Value1, g_pTempInformation->Value2 );
		pItem[1] = g_pInventory->GetItem( g_pTempInformation->Value3, g_pTempInformation->Value4 );

		g_pTempInformation->SetMode( TempInformation::MODE_NULL);
	}		
	else
		DEBUG_ADD("[ERROR] TempInformationMode");


	switch(pPacket->getCode())
	{
		case ADD_ITEM_TO_ITEM_VERIFY_ERROR:
			// ui error dialog
			UI_PopupMessage( STRING_ERROR_ETC_ERROR );
			UI_UnlockItem();
			g_pPlayer->ClearItemCheckBuffer();
			break;

		case ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE:
			// ui error dialog
			UI_PopupMessage( STRING_MESSAGE_ITEM_TO_ITEM_IMPOSIBLE );
			UI_UnlockItem();
			g_pPlayer->ClearItemCheckBuffer();
			break;

		case ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL_CRASH:
			//----------------------------------------------------
			// 扁贱 荤侩 矫档 悼累
			//----------------------------------------------------
			AddNewInventoryEffect( pInventoryItem->GetID(),
				MAGIC_ENCHANT_REMOVE_ITEM,
				0
				);
			// 荤款靛??
			PlaySound(SOUND_XMAS_STAR);
			UI_DropItem();
			break;

		case ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL_DECREASE:
			{
				DWORD OptionType = pPacket->getParameter();
				
				//----------------------------------------------------
				// 扁贱 荤侩 矫档 悼累
				//----------------------------------------------------
				AddNewInventoryEffect( pInventoryItem->GetID(),
					MAGIC_ENCHANT_OPTION_NULL,
					0, OptionType
					);
				
				// 荤款靛??
				PlaySound(SOUND_XMAS_STAR);
				UI_DropItem();
			}
			break;

		case ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK:
			{
				DWORD OptionType = pPacket->getParameter();
				//----------------------------------------------------
				// 扁贱 荤侩 矫档 悼累
				//----------------------------------------------------
				AddNewInventoryEffect( pInventoryItem->GetID(),
					MAGIC_ENCHANT_OPTION_PLUS,
					0, OptionType
					);


//				pInventoryItem->SetItemOption(OptionType);
				UI_DropItem();

			}
			break;
			// add  by sonic 2006.11.3  三属性
		case ADD_ITEM_TO_ITEM_VERIFY_THREE_ENCHANT_OK:
			{
				DWORD OptionType = pPacket->getParameter();
				DWORD OptionType2= pPacket->getParameter2();
				//----------------------------------------------------
				// 扁贱 荤侩 矫档 悼累
				//----------------------------------------------------
				TYPE_ITEM_OPTION Option1;
				TYPE_ITEM_OPTION Option2;
				TYPE_ITEM_OPTION Option3;
				TYPE_ITEM_OPTION oldOption1;
				TYPE_ITEM_OPTION oldOption2;
				TYPE_ITEM_OPTION oldOption3;
				BYTE bTmp[4];
				memcpy(&bTmp[0],&OptionType,4);
				memcpy(&Option1,&bTmp[0],1);
				memcpy(&oldOption1,&bTmp[1],1);
				memcpy(&Option2,&bTmp[2],1);
				memcpy(&oldOption2,&bTmp[3],1);
				memcpy(&bTmp[0],&OptionType2,4);
				memcpy(&Option3,&bTmp[0],1);
				memcpy(&oldOption3,&bTmp[1],1);
				if (Option1!=0)
					pInventoryItem->ChangeItemOption(oldOption1,Option1);
				if (Option2!=0)
					pInventoryItem->ChangeItemOption(oldOption2,Option2);
				if (Option3!=0)
					pInventoryItem->ChangeItemOption(oldOption3,Option3);
				AddNewInventoryEffect( pInventoryItem->GetID(),
					MAGIC_ENCHANT_OPTION_PLUS,
					0, OptionType
					);


//				pInventoryItem->SetItemOption(OptionType);
				UI_DropItem();

			}
			break;
		case ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE_NO_PREMIUM:
			if(g_pPlayer->IsSlayer())
				UI_PopupMessage( STRING_MESSAGE_ITEM_TO_ITEM_FAIL_NO_PREMIUM_SLAYER );
			else
				UI_PopupMessage( STRING_MESSAGE_ITEM_TO_ITEM_FAIL_NO_PREMIUM_VAMPIRE );
			UI_UnlockItem();
			g_pPlayer->ClearItemCheckBuffer();
			break;
		case ADD_ITEM_TO_ITEM_VERIFY_TRANS_OK :				// 己傈券 己傍
			{
				AddNewInventoryEffect( pInventoryItem->GetID() ,
					MAGIC_TRANS_ITEM_OK,
					0, 0 );
				UI_DropItem();
			}
			break;
		case ADD_ITEM_TO_ITEM_VERIFY_TRANS_IMPOSSIBLE :		// 己傈券 角菩
			{
			}
			break;
		case ADD_ITEM_TO_ITEM_VERIFY_MIXING_OK :
			{
				if( pItem[0] == NULL || pItem[1] == NULL )
					return;
								
				UI_OkMixingForge( pPacket->getParameter(), pItem[0], pItem[1] );
				
				g_pTempInformation->SetMode( TempInformation::MODE_NULL );
				g_pTempInformation->Value1 = 0;
				g_pTempInformation->Value2 = 0;
				g_pTempInformation->Value3 = 0;
				g_pTempInformation->Value4 = 0;
				g_pTempInformation->pValue = NULL;
			}
			break;
		case ADD_ITEM_TO_ITEM_VERIFY_DETACHING_OK :	
			if( pInventoryItem == NULL )
				return;

			UI_OkRemoveOption( pPacket->getParameter(), pInventoryItem );
			UI_DropItem();
			g_pPlayer->ClearItemCheckBuffer();
			break;
		case ADD_ITEM_TO_ITEM_VERIFY_MIXING_FAILED_SAME_OPTION_GROUP :
			UI_PopupMessage( STRING_MESSAGE_MIXING_FORGE_FAILED_SAME_OPTION_GROUP );
			break;
			
		case ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL:
			{
				AddNewInventoryEffect( pInventoryItem->GetID() ,
					MAGIC_UNKOWN1,
					0, 0 );
				UI_DropItem();
				UI_UnlockItem();
				g_pPlayer->ClearItemCheckBuffer();
			}
			break;

		case ADD_ITEM_TO_ITEM_VERIFY_REVIVAL_OK:
			{
				if( pInventoryItem == NULL )
				return;

				DWORD OptionType = pPacket->getParameter();
				AddNewInventoryEffect( pInventoryItem->GetID(),
					MAGIC_ENCHANT_OPTION_PLUS,
					0, OptionType
					);
				
	//			pInventoryItem->SetCurrentDurability(pPacket->getParameter());
				UI_DropItem();
	//			g_pPlayer->ClearItemCheckBuffer();
			}
			break;
		case ADD_ITEM_TO_ITEM_VERIFY_CLEAR_OPTION_OK:
			{
				DWORD OptionType = pPacket->getParameter();
				AddNewInventoryEffect( pInventoryItem->GetID(),
				MAGIC_ENCHANT_OPTION_NULL,
				0, OptionType
				);
				PlaySound(SOUND_XMAS_STAR);
//				pInventoryItem->RemoveItemOption(g_pTempInformation->Value3);

				UI_DropItem();
				UI_PopupMessage( STRING_MESSAGE_SUCCESS_CHANGE );
			}
			break;

		// add by svi 2009-06-24 增加升级成功确认
		
		case ADD_ITEM_TO_ITEM_VERIFY_UP_GRADE_OK:
			{
				DWORD OptionType = pPacket->getParameter();
				// add by svi 2009-06-28
				DWORD OptionType2= pPacket->getParameter2();
				pInventoryItem->SetItemType(OptionType2);
				// end
				
				AddNewInventoryEffect( pInventoryItem->GetID(),
				MAGIC_ENCHANT_OPTION_PLUS,
				0, OptionType
				);
				PlaySound(SOUND_XMAS_STAR);
//				pInventoryItem->RemoveItemOption(g_pTempInformation->Value3);

				UI_DropItem();
				UI_PopupMessage( STRING_MESSAGE_SUCCESS_CHANGE );
			}
			break;

		// add by svi 2009-07-15 增加删除属性确认
		
		case ADD_ITEM_TO_ITEM_REMOVE_OPTION_OK:
			{
				DWORD OptionType = pPacket->getParameter();

				pInventoryItem->RemoveItemOption( OptionType );
				
				AddNewInventoryEffect( pInventoryItem->GetID(),
				MAGIC_ENCHANT_OPTION_PLUS,
				0, 0
				);
				PlaySound(SOUND_XMAS_STAR);
				UI_DropItem();
				UI_PopupMessage( STRING_MESSAGE_SUCCESS_CHANGE );
			}
			break;

	}

#endif

	__END_DEBUG
	__END_CATCH
}
