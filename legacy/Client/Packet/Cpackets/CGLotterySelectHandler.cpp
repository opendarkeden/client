//////////////////////////////////////////////////////////////////////////////
// Filename    : CGLotterySelectHandler.cpp
// Written By  :
// Description :
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGLotterySelect.h"

#ifdef __GAME_SERVER__

#include "GamePlayer.h"
#include "PlayerCreature.h"
#include "NPC.h"

#include "Gpackets/GCNoticeEvent.h"

#include <cstdio>

#endif	// __GAME_SERVER__

void CGLotterySelectHandler::execute (CGLotterySelect* pPacket , Player* pPlayer)
	 throw (Error)
{
	__BEGIN_TRY 
		
#ifdef __GAME_SERVER__

	GamePlayer* pGP = dynamic_cast<GamePlayer*>( pPlayer );
	Assert( pGP != NULL );
		
	PlayerCreature* pPC = dynamic_cast<PlayerCreature*>(pGP->getCreature());
	Assert( pPC != NULL );

	switch ( pPacket->getType() )
	{
		case TYPE_SELECT_LOTTERY:
			{
				GCNoticeEvent gcNE;
				gcNE.setCode( NOTICE_EVENT_RESULT_LOTTERY );
				gcNE.setParameter( (uint)rand()%2 );
				pGP->sendPacket( &gcNE );
			}
			break;
		case TYPE_FINISH_SCRATCH:
			{
				// 확인하고 디비에 저장하기
				// 패킷 만들어서 올월드에 브로드캐스팅해주기
				// 폭죽 이펙트 붙여주기..

			}
			break;
		case TYPE_OVER_ENDING:
			{
				// 죽인다.
				pPC->setHP(0);
			}
			break;
	}


#endif	// __GAME_SERVER__
		
	__END_CATCH
}

