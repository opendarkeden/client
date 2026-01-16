//////////////////////////////////////////////////////////////////////
//
// Filename    : GCHolyLandBonusInfoHandler.cpp
// Written By  : 
//
//////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
// include files
#include "GCHolyLandBonusInfo.h"
#include "ClientDef.h"
#include "MSkillManager.h"

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void GCHolyLandBonusInfoHandler::execute ( GCHolyLandBonusInfo * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	
#ifdef __GAME_CLIENT__
	if(g_pPlayer == NULL || g_pSkillAvailable == NULL)
		return;

	BloodBibleBonusInfo *pInfo = pPacket->popFrontBloodBibleBonusInfoList();
	
	int i = 0;
	while(pInfo != NULL)
	{
		if(g_pPlayer->GetRace() == pInfo->getRace())
//		if(g_pPlayer->IsSlayer() && pInfo->getRace() == RACE_SLAYER
//			|| g_pPlayer->IsVampire() && pInfo->getRace() == RACE_VAMPIRE
//			|| g_pPlayer->IsOusters() && pInfo->getRace() == RACE_OUSTERS
//			)
		{
			g_abHolyLandBonusSkills[i] = true;
		}
		else
		{
			// 스킬 리스트에서 빼준다?		순서 틀리면 바보 된다
			g_abHolyLandBonusSkills[i] = false;
		}
		
		delete pInfo;

		pInfo = pPacket->popFrontBloodBibleBonusInfoList();
		i++;
	}

	//--------------------------------------------------
	// 현재 사용 가능한 skill들을 다시 체크한다.
	//--------------------------------------------------
	g_pSkillAvailable->SetAvailableSkills();

#endif

	__END_CATCH
}
