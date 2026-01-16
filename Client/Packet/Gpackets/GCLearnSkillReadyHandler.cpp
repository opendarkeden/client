//////////////////////////////////////////////////////////////////////
//
// Filename    : GCLearnSkillReady1Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCLearnSkillReady.h"
#include "ClientDef.h"
#include "MSkillManager.h"
#include "SkillDef.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCLearnSkillReadyHandler::execute ( GCLearnSkillReady * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
#ifdef __GAME_CLIENT__


	// 임시로 skill관련 정보를 저장한다.
	//g_TempInformation.Mode		=	TempInformation::MODE_SKILL_LEARN;
	//g_TempInformation.Value1	=	pPacket->getSkillDomainType();

	// 새로 배울 Skill이 있다고 표시한다.
	int domainType = pPacket->getSkillDomainType();

	(*g_pSkillManager)[domainType].SetNewSkill();
	
	g_pGameMessage->AddFormat( "%s 계열의 기술을 배울 수 있습니다.", SKILLDOMAIN_NAME[domainType]);

	// levelup했다고 뭔가 보여준다. 뭘까.... --;
	//UI_LevelUp();

	// [도움말] Skill배울 수 있을 때
//	__BEGIN_HELP_EVENT
//		switch ( domainType )
//		{
//			case SKILL_DOMAIN_BLADE :
////				ExecuteHelpEvent( HE_SKILL_CAN_LEARN_BLADE );
//			break;
//
//			case SKILL_DOMAIN_SWORD :
////				ExecuteHelpEvent( HE_SKILL_CAN_LEARN_SWORD );
//			break;
//
//			case SKILL_DOMAIN_GUN :
////				ExecuteHelpEvent( HE_SKILL_CAN_LEARN_GUN );
//			break;
//
//			case SKILL_DOMAIN_ENCHANT :
////				ExecuteHelpEvent( HE_SKILL_CAN_LEARN_ENCHANT );
//			break;
//
//			case SKILL_DOMAIN_HEAL :
////				ExecuteHelpEvent( HE_SKILL_CAN_LEARN_HEAL );
//			break;
//			
//			case SKILL_DOMAIN_VAMPIRE :
////				ExecuteHelpEvent( HE_SKILL_CAN_LEARN_VAMPIRE );
//			break;
//		}
//	__END_HELP_EVENT
	
#endif

	__END_CATCH
}
