//////////////////////////////////////////////////////////////////////////////
// Filename    : CGBuyStoreItemHandler.cpp
// Written By  : 김성민
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGBuyStoreItem.h"

#ifdef __GAME_SERVER__
	#include "GamePlayer.h"
	#include "Assert.h"
#endif

//////////////////////////////////////////////////////////////////////////////
// 플레이어가 원하는 물건을 파는 NPC와 그 아이템이 있는지
// 확인하고, 일반 아이템과 모터 사이클 처리부분으로 분기한다.
//////////////////////////////////////////////////////////////////////////////
void CGBuyStoreItemHandler::execute (CGBuyStoreItem* pPacket , Player* pPlayer)
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY 
	//__BEGIN_DEBUG_EX

#ifdef __GAME_SERVER__
	
	Assert(pPacket != NULL);
	Assert(pPlayer != NULL);

#endif

	//__END_DEBUG_EX 
	__END_CATCH
}


  