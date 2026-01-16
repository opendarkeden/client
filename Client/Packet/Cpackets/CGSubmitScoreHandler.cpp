//////////////////////////////////////////////////////////////////////////////
// Filename    : CGSubmitScoreHandler.cpp
// Written By  : crazydog
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "CGSubmitScore.h"

#ifdef __GAME_SERVER__
	#include "GamePlayer.h"
	#include "PlayerCreature.h"
	#include "DB.h"
#endif


#ifndef __GAME_CLIENT__
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CGSubmitScoreHandler::execute (CGSubmitScore* pPacket , Player* pPlayer)
	     throw (Error)
{
	__BEGIN_TRY __BEGIN_DEBUG_EX
	__BEGIN_DEBUG

#ifdef __GAME_SERVER__

	Assert(pPacket != NULL);
	Assert(pPlayer != NULL);

	try 
	{
		GamePlayer* pGamePlayer = dynamic_cast<GamePlayer*>(pPlayer);
		Assert(pGamePlayer != NULL);

		if (pGamePlayer->getPlayerStatus() == GPS_NORMAL) 
		{
			PlayerCreature* pPC = dynamic_cast<PlayerCreature*>(pGamePlayer->getCreature());
			Assert( pPC != NULL );

			Statement* pStmt = NULL;

			BEGIN_DB
			{
				pStmt = g_pDatabaseManager->getConnection("DARKEDEN")->createStatement();
				pStmt->executeQuery(
						"INSERT INTO MiniGameScores VALUES ('%s',%u,%u,now())", pPC->getName().c_str(),
						pPacket->getGameType(), pPacket->getScore() );

				SAFE_DELETE(pStmt);
			}
			END_DB(pStmt)


		}
	} 
	catch (Throwable & t) 
	{ 
		//cout << t.toString(); 
	}

#endif

	__END_DEBUG
    __END_DEBUG_EX __END_CATCH
}
#endif