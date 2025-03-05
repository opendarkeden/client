//////////////////////////////////////////////////////////////////////
//
// Filename    : GCChangeShapeHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCChangeShape.h"
#include "ClientDef.h"
#include "MItem.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCChangeShapeHandler::execute ( GCChangeShape * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


	//--------------------------------------------------------
	//
	// 임시로 Item을 생성한다.
	//
	//--------------------------------------------------------
	MItem* pItem = NULL;

	pItem = MItem::NewItem( (enum ITEM_CLASS)pPacket->getItemClass() );

	//--------------------------------------------------------
	// Item이 생성되지 않은 경우...
	//--------------------------------------------------------
	if (pItem==NULL)
	{
		DEBUG_ADD_FORMAT("[Error] Can't new Item ID=%d, Type=%d", pPacket->getItemClass(), pPacket->getItemType());
		
		return;
	}

	pItem->SetItemType( pPacket->getItemType() );
	pItem->AddItemOption( pPacket->getOptionType() );
	pItem->SetQuestFlag( (pPacket->getFlag() & SHAPE_FLAG_QUEST) ? true : false );


	//------------------------------------------------------
	//
	//  Creature에게 적절한 Addon을 착용시킨다.
	//
	//------------------------------------------------------
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
		MCreature* pCreature = g_pZone->GetCreature( pPacket->getObjectID() );

		//--------------------------------------------------
		// Creature가 없는 경우
		//--------------------------------------------------
		if (pCreature==NULL)
		{
			// message
			DEBUG_ADD_FORMAT("[Error] Not Exist Creature. ID=%d", pPacket->getObjectID());
		}
		//--------------------------------------------------
		// 존재하는 Creature인 경우
		// 복장이 있는 creature이면 --> Addon착용
		//--------------------------------------------------
		else if (pCreature->IsWear())
		{
			MCreatureWear* pCreatureWear = (MCreatureWear*)pCreature;

			// 캐릭터를 정지시킨다.
			pCreatureWear->SetStop();
			
			#ifdef	OUTPUT_DEBUG				
				if (pCreatureWear->SetAddonItem( pItem ))
				{
					DEBUG_ADD_FORMAT("[OK] Addon Item. ID=%d, Class=%d, Type=%d", pPacket->getObjectID(), pItem->GetItemClass(), pItem->GetItemType());
				}
				else
				{
					DEBUG_ADD_FORMAT("[Error] Can't Addon Item. ID=%d, Class=%d, Type=%d", pPacket->getObjectID(), pItem->GetItemClass(), pItem->GetItemType());
				}				
			#else
				pCreatureWear->SetAddonItem( pItem );
			#endif

			pCreatureWear->SetWeaponSpeed( pPacket->getAttackSpeed() );
		}
			
		//--------------------------------------------------
		// 옷을 입을 수 없는 Creature인 경우
		//--------------------------------------------------
		else 
		{
			DEBUG_ADD_FORMAT("[Error] Creature is Not CreatureWear. ID=%d", pPacket->getObjectID());
		}
	}	

	delete pItem;

#endif

	__END_CATCH
}
