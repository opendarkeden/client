//////////////////////////////////////////////////////////////////////
//
// Filename    : GCGQuestInventoryHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCGQuestInventory.h"
#include "Client.h"
#include "vs_ui.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCGQuestInventoryHandler::execute ( GCGQuestInventory * pGCGQuestInventory , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY 
//		__BEGIN_DEBUG_EX
		
#ifdef __GAME_CLIENT__

	//------------------------------------------------------
	// Zone이 아직 생성되지 않은 경우
	//------------------------------------------------------
	if (g_pZone==NULL)
	{
		// message
		DEBUG_ADD("[Error] Zone is Not Init.. yet.");			
	}
	//------------------------------------------------------
	// 정상.. 
	//------------------------------------------------------
	else
	{
		std::vector<MItem*> QuestItemList;

		std::vector<ItemType_t>	TempItemList = pGCGQuestInventory->getItemList();

		for(int i = 0; i<TempItemList.size(); i++)
		{
			ItemType_t TempType = TempItemList[i];

			MItem* pItem;
			pItem = MItem::NewItem( ITEM_CLASS_GQUEST_ITEM );
			pItem->SetItemType( TempType );
			pItem->SetCurrentDurability( 1 );	
			QuestItemList.push_back(pItem);
		}
		gC_vs_ui.UpdateQuestItemInfo(QuestItemList); 

	// 아이템 리스트 받아서 아이템을 생성한후 UI에 넘겨주자..
	// 나중에 퀘스트 아이템 추가 되믄 작업하자
	//gC_vs_ui.UpdateQuestItemInfo(아이템 리스트)



	}
//#elif __WINDOWS__

#endif

//	__END_DEBUG_EX
	__END_CATCH
}
