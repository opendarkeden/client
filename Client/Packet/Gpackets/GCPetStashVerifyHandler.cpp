
//--------------------------------------------------------------------------------
//
// Filename    : GCPetStashVerifyHandler.cpp
// Written By  : elca, Reiot
// Description :
//
//--------------------------------------------------------------------------------

#include "Client_PCH.h"
// include files
#include "GCPetStashVerify.h"

#ifdef __GAME_CLIENT__
	#include "ClientPlayer.h"
	#include "ClientDef.h"
	#include "UIFunction.h"
	#include "TempInformation.h"
	#include "UIDialog.h"
	#include "MGameStringTable.h"
	#include "MStorage.h"
	#include "MInventory.h"
#endif
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCPetStashVerifyHandler::execute ( GCPetStashVerify * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY 
//		__BEGIN_DEBUG_EX
	__BEGIN_DEBUG
		
#ifdef __GAME_CLIENT__
	switch(pPacket->getCode())
	{ 
		case GCPetStashVerify::PET_STASH_OK:
			if(g_pTempInformation->GetMode() == TempInformation::MODE_PETITEM_MOVETO_INVENTORY)
			{// 보관함 -> 인벤
				MItem* pItem = g_pStorage->RemoveItem(g_pTempInformation->Value1) ;
				if(pItem)
				{
					MPetItem* pPetItem = (MPetItem*)g_pTempInformation->pValue;
					if(pPetItem)
					{
						pPetItem->SetPetKeepedDay(0);
						if(g_pInventory->AddItem( (MItem*)g_pTempInformation->pValue))
						{ // 잘 됐네
							DEBUG_ADD("@Stash - Get Keep PetItem Inventory AddItem Success");
						}
						else
						{
							DEBUG_ADD("@Stash - Get Keep PetItem Inventory AddItem Failed");
						}
					}
				}
				else
				{
					DEBUG_ADD("@Stash - Keep PetItem Storage RemoveItem Failed");
				}
			}
			else if(g_pTempInformation->GetMode() == TempInformation::MODE_PETITEM_MOVETO_PETSTORAGE)
			{// 인벤 -> 보관함
				MItem* pItem = g_pInventory->RemoveItem(g_pTempInformation->Value2, g_pTempInformation->Value3) ;
				if(pItem)
				{
					MPetItem* pPetItem = (MPetItem*)g_pTempInformation->pValue;
					if(pPetItem)
					{
						if(g_pStorage->SetItem(g_pTempInformation->Value1, (MItem*)g_pTempInformation->pValue))
						{
							DEBUG_ADD("@Stash - Keep PetItem Storage SetItem Success");
						}
						else
						{
							DEBUG_ADD("@Stash - Get Keep PetItem Storage SetItem Failed");
						}
					}
				}
				else
				{
					DEBUG_ADD("@Stash - Get Keep PetItem Inventory RemoveItem Failed");
				}
			}
			break;
		case GCPetStashVerify::PET_STASH_NOT_ENOUGH_MONEY:
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_WAR_NOT_ENOUGH_MONEY].GetString() );
			
			break;
		case GCPetStashVerify::PET_STASH_NO_INVENTORY_SPACE:
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_INVENTORY_FULL_MONSTER_KILL_QUEST].GetString() );
			
			break;
		case GCPetStashVerify::PET_STASH_RACK_IS_NOT_EMPTY:	// 해당 위치에 이미 다른 펫 아이템이 있습니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_EXIST_ITEM_ALREADY].GetString() );
			
			break;
		case GCPetStashVerify::PET_STASH_RACK_IS_EMPTY:// 찾으려고 한 위치에 아이템이 없습니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_CANNOT_BUY_NO_ITEM].GetString() );
			
			break;
	}
	g_pTempInformation->SetMode(TempInformation::MODE_NULL);
	UI_UnlockItem();
#endif

	__END_DEBUG
//	__END_DEBUG_EX 
	__END_CATCH
}
