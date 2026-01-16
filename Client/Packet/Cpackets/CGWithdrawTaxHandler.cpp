#include "Client_PCH.h"
#ifndef __GAME_CLIENT__

//////////////////////////////////////////////////////////////////////////////
// Filename    : CGWithdrawTaxHandler.cpp
// Written By  :
// Description :
//////////////////////////////////////////////////////////////////////////////

#include "CGWithdrawTax.h"

#ifdef __GAME_SERVER__
	#include "GamePlayer.h"
	#include "PlayerCreature.h"
	#include "GuildManager.h"
	#include "CastleInfoManager.h"
	#include "Gpackets/GCNPCResponse.h"
	#include "Gpackets/GCModifyInformation.h"
#endif	// __GAME_SERVER__

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CGWithdrawTaxHandler::execute (CGWithdrawTax* pPacket , Player* pPlayer)
	 throw (Error)
{
#ifdef __GAME_SERVER__

	__BEGIN_TRY __BEGIN_DEBUG_EX
		

	Assert(pPacket != NULL);
	Assert(pPlayer != NULL);

	GamePlayer* pGamePlayer = dynamic_cast<GamePlayer*>(pPlayer);
	Assert(pGamePlayer != NULL);

	PlayerCreature* pPC = dynamic_cast<PlayerCreature*>(pGamePlayer->getCreature());
	Assert( pPC != NULL );

	GuildID_t guildID = pPC->getGuildID();
	Gold_t gold = pPacket->getGold();

	CastleInfo* pCastleInfo = g_pCastleInfoManager->getGuildCastleInfo( guildID );
	if ( !g_pGuildManager->isGuildMaster( guildID, pPC )	// 길드 마스터가 아니다.
		|| pCastleInfo == NULL 								// 길드가 점령한 성이 없다.
		|| pCastleInfo->getZoneID() != pPC->getZoneID()		// 이 플레이어의 길드가 점령한 성이 아니다.
		|| pCastleInfo->getTaxBalance() < gold				// 돈이 모자란다.
	)
	{
		GCNPCResponse fail;
		fail.setCode( NPC_RESPONSE_WITHDRAW_TAX_FAIL );

		pGamePlayer->sendPacket( &fail );
		return;
	}

	// 모든 조건이 충족되었다. 이제 돈을 찾아서 플레이어에게 넣어준다.
	Gold_t remainBalance = pCastleInfo->decreaseTaxBalanceEx( gold );
	pPC->increaseGoldEx( gold );

	// 사용자의 돈이 늘어났다는 정보를 보내준다.
	GCModifyInformation gcMI;
	gcMI.addLongData( MODIFY_GOLD, pPC->getGold() );

	pGamePlayer->sendPacket( &gcMI );

	// 돈을 찾는 데 성공했다고 알린다.
	GCNPCResponse success;
	success.setCode( NPC_RESPONSE_WITHDRAW_TAX_OK );
	success.setParameter( remainBalance );

	pGamePlayer->sendPacket( &success );

		
	__END_DEBUG_EX __END_CATCH
#endif	// __GAME_SERVER__
}

#endif