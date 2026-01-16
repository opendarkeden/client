//////////////////////////////////////////////////////////////////////
//
// Filename    : GCLearnSkillOK1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCLearnSkillOK.h"
#include "ClientDef.h"
#include "MSkillManager.h"
#include "MGameStringTable.h"
#include "TempInformation.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCLearnSkillOKHandler::execute ( GCLearnSkillOK * pPacket , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
		
#if __GAME_CLIENT__
		
	// mode 제거
	// 제거안하면 계속 배울 수 있다.- -;
	//g_TempInformation.Mode = TempInformation::MODE_NULL;
	g_pTempInformation->SetMode(TempInformation::MODE_NULL);
						

	int domainType	= pPacket->getSkillDomainType();
	int skillType	= pPacket->getSkillType();


	// 배운다.
	MSkillDomain& swordDomain = (*g_pSkillManager)[domainType];

	swordDomain.SetNewSkill();
	bool bLearn = swordDomain.LearnSkill( (ACTIONINFO)skillType );
	
	if(bLearn == true || !g_pPlayer->IsOusters())
	{
		// 현재 사용가능한 skill에 추가
		g_pSkillAvailable->AddSkill( (ACTIONINFO)skillType );
		
		SKILLINFO_NODE& skillInfo = (*g_pSkillInfoTable)[skillType];
		
		skillInfo.SetExpLevel( 1 );
		skillInfo.SetSkillExp( 0 );
		
		skillInfo.SetDelayTime( skillInfo.maxDelay );
		skillInfo.SetEnable( true );
		
		skillInfo.SetNextAvailableTime();
	}
	else if( g_pPlayer->IsOusters() )
	{
		// 아우스터즈인경우 이미 배운 스킬이면
		SKILLINFO_NODE& skillInfo = (*g_pSkillInfoTable)[skillType];
		skillInfo.SetExpLevel( skillInfo.GetExpLevel()+1 );
		
	}
	
	// DEBUG_ADD_FORMAT( "[Error] TempInformation Mode is Not SKILL_LEARN : %d", (int)g_TempInformation.Mode);

	//------------------------------------------------------------
	// 사용할 수 있는 skill 다시 체크
	//------------------------------------------------------------
	g_pSkillAvailable->SetAvailableSkills();
	
	//------------------------------------------------------------
	// 기술 배웠다는 message 추가
	//------------------------------------------------------------
	g_pGameMessage->AddFormat((*g_pGameStringTable)[STRING_LEARN_SKILL].GetString(), (*g_pSkillInfoTable)[skillType].GetHName());

	// [도움말] Skill배웠을때
//	__BEGIN_HELP_EVENT
////		ExecuteHelpEvent( HE_SKILL_LEARNED );
//	__END_HELP_EVENT

#endif

	__END_CATCH
}
