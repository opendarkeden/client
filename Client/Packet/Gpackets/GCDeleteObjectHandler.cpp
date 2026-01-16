//--------------------------------------------------------------------------------
//
// Filename    : GCDeleteObjectHandler.cpp
// Written By  : Reiot
// Description :
//
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCDeleteObject.h"
#include "ClientDef.h"
#include "UIFunction.h"


#ifdef __GAME_CLIENT__
#endif

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void GCDeleteObjectHandler::execute ( GCDeleteObject * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	//cout << "Object[" << pPacket->getObjectID() << "] deleted from zone." << endl;

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
	// 어떤 Object를 제거하는데..
	// --> 제거해보고 확인해야 한다.
	//------------------------------------------------------
	else
	{	
		TYPE_OBJECTID objectID = pPacket->getObjectID();

		if (g_pPlayer->IsItemCheckBufferDropToRelicTable())
		{
			//---------------------------------------------
			// 떨어뜨릴려는 item
			//---------------------------------------------
			MItem* pItem = g_pPlayer->GetItemCheckBuffer();

			if(pItem->GetID() == objectID)UI_DropItem();
			g_pPlayer->ClearItemCheckBuffer();
		}

		//------------------------------------------------------
		// Creature인 경우??
		//------------------------------------------------------
		if ( g_pZone->RemoveCreature( objectID ) )
		{
			DEBUG_ADD_FORMAT("Remove Creature OK. ID=%d", objectID);
		}
		//------------------------------------------------------
		// Item인 경우??
		//------------------------------------------------------
		else if ( g_pZone->RemoveItem( objectID ) )
		{			
			DEBUG_ADD_FORMAT("Remove Item OK. ID=%d", objectID);
		}
		//------------------------------------------------------
		// 뭐지???
		//------------------------------------------------------
		else
		{
			DEBUG_ADD_FORMAT("[Error] Can't Remove Object. ID=%d", objectID);
		}
	}

#endif

	__END_CATCH
}
