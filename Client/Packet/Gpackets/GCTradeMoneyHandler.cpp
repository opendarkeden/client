//////////////////////////////////////////////////////////////////////
//
// Filename    : GCTradeMoneyHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCTradeMoney.h"
#include "ClientDef.h"
#include "MTradeManager.h"

void GCTradeMoneyHandler::execute ( GCTradeMoney * pPacket , Player * pPlayer )
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
	
	int money = pPacket->getAmount();

	bool bRefuseAccept = false;
	bool bNextAcceptTime = false;
	
	switch (pPacket->getCode())
	{
		//---------------------------------------------------------------
		// 상대방이 교환할 돈의 액수를 늘렸다.
		//---------------------------------------------------------------
		case GC_TRADE_MONEY_INCREASE :
			g_pTradeManager->GetOtherMoneyManager()->AddMoney( money );

			bRefuseAccept = true;
		break;

		//---------------------------------------------------------------
		// 상대방이 교환할 돈의 액수를 줄였다.
		//---------------------------------------------------------------
		case GC_TRADE_MONEY_DECREASE :
			g_pTradeManager->GetOtherMoneyManager()->UseMoney( money );

			bRefuseAccept = true;
			bNextAcceptTime = true;
		break;
		
		//---------------------------------------------------------------
		// [검증] 교환할 때 추가될 돈 
		//---------------------------------------------------------------
		case GC_TRADE_MONEY_INCREASE_RESULT :			
			g_pMoneyManager->UseMoney( money );
			g_pTradeManager->GetMyMoneyManager()->AddMoney( money );	
			
			bRefuseAccept = true;
		break;

		//---------------------------------------------------------------
		// [검증] 교환에서 빼낼 돈
		//---------------------------------------------------------------
		case GC_TRADE_MONEY_DECREASE_RESULT :			
			g_pTradeManager->GetMyMoneyManager()->UseMoney( money );
			g_pMoneyManager->AddMoney( money );	

			bRefuseAccept = true;
		break;
	}

	//-----------------------------------------------------------
	// 뭔가 바뀐다면... OK취소
	//-----------------------------------------------------------
	if (bRefuseAccept)
	{
		g_pTradeManager->RefuseOtherTrade();
		g_pTradeManager->RefuseMyTrade();

		if(bNextAcceptTime)
			g_pTradeManager->SetNextAcceptTime();
	}
	

#endif

	__END_CATCH
}
