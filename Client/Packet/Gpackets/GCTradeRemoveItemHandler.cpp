//////////////////////////////////////////////////////////////////////
//
// Filename    : GCTradeRemoveItemHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCTradeRemoveItem.h"
#include "MTradeManager.h"
#include "ClientDef.h"

void GCTradeRemoveItemHandler::execute ( GCTradeRemoveItem * pPacket , Player * pPlayer )
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

	//ObjectID_t getTargetObjectID() const throw() { return m_TargetObjectID; }
	
	TYPE_OBJECTID itemID = pPacket->getItemObjectID();

	MItem* pItem = g_pTradeManager->GetOtherInventory()->RemoveItem( itemID );

	//------------------------------------------------------------------------
	// 아이템이 없다면..
	//------------------------------------------------------------------------
	if (pItem==NULL)
	{
		DEBUG_ADD_FORMAT( "[Error] There is no such item. id=%d", itemID );
	}
	//------------------------------------------------------------------------
	// 있으면 그냥 지우면 된다.
	//------------------------------------------------------------------------
	else
	{
		delete pItem;
	}
	
	//-----------------------------------------------------------
	// 뭔가 바뀐다면... OK취소
	//-----------------------------------------------------------
	g_pTradeManager->RefuseOtherTrade();
	g_pTradeManager->RefuseMyTrade();

	g_pTradeManager->SetNextAcceptTime();

#endif

	__END_CATCH
}
