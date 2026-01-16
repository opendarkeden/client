//----------------------------------------------------------------------
//
// Filename    : GCRankBonusInfoHandler.cpp
// Written By  : elca
// Description : 
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "GCRankBonusInfo.h"
#include "RankBonusTable.h"
#include "MPlayer.h"

void GCRankBonusInfoHandler::execute ( GCRankBonusInfo * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	for(int i = 0; i < g_pRankBonusTable->GetSize(); i++)
		(*g_pRankBonusTable)[i].SetStatus(RankBonusInfo::STATUS_NULL);

	DWORD type = 0;

	while((type = pPacket->popFrontListElement()) != EndOfRankBonus)
	{
		if(type < g_pRankBonusTable->GetSize())
		{
			(*g_pRankBonusTable)[type].SetStatus(RankBonusInfo::STATUS_LEARNED);

			const int level = (*g_pRankBonusTable)[type].GetLevel();
			int i;

			for(i = type-1; i >= 0 && (*g_pRankBonusTable)[i].GetLevel() == level; i--)
				(*g_pRankBonusTable)[i].SetStatus(RankBonusInfo::STATUS_CANNOT_LEARN);

			for(i = type+1; i < g_pRankBonusTable->GetSize() && (*g_pRankBonusTable)[i].GetLevel() == level; i++)
				(*g_pRankBonusTable)[i].SetStatus(RankBonusInfo::STATUS_CANNOT_LEARN);
		}
	}

	g_pPlayer->CheckRegen();

	__END_CATCH
}
