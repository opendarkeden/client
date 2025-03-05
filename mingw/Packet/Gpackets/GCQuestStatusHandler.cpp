//////////////////////////////////////////////////////////////////////
//
// Filename    : GCMonsterKillQuestStatusHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCQuestStatus.h"
#include "MGameStringTable.h"
#include "MEventManager.h"
#include "DebugInfo.h"
#include "ClientDef.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCQuestStatusHandler::execute ( GCQuestStatus * pGCMonsterKillQuestStatus , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY 
		
#ifdef __GAME_CLIENT__

	DEBUG_ADD("[GCQuestStatusHandler] execute");

	UI_RunQuestStatus( pGCMonsterKillQuestStatus );

//	MEvent event;
//
//	// 현재 몬스터 정보를 담고있는 매니저를 만들면 이럴 필요는 없지만...-_-; 가지고 있어봐야 
//	// 잠깐 표시해주는게 다이므로, 그냥 이렇게 쓰자.
//
//	const MEvent *pEvent = g_pEventManager->GetEvent( EVENTID_MONSTER_KILL_QUEST );
//
//	if( pEvent != NULL)
//	{
//		// 기존꺼가 있으면		
//		
//		event.eventID = EVENTID_MONSTER_KILL_QUEST;
//		event.eventDelay = pEvent->eventDelay; //pGCMonsterKillQuestStatus->getRemainTime() * 1000 + 3000;
//		event.eventFlag = EVENTFLAG_QUEST_INFO;
//		event.eventType = EVENTTYPE_ZONE;
////		event.parameter1 = 0;
//		event.parameter2 = pGCMonsterKillQuestStatus->getQuestID();
//		
//		if( pEvent->parameter3 < pGCMonsterKillQuestStatus->getCurrentNum() )
//			event.parameter1 = timeGetTime() + 4000;
//
//		event.parameter3 = pGCMonsterKillQuestStatus->getCurrentNum();
//		event.parameter4 = pEvent->parameter4; //pGCMonsterKillQuestStatus->getRemainTime() + (timeGetTime() / 1000);
//		g_pEventManager->AddEvent( event );
//	} else
//	{
//		// 기존에 이벤트가 없으면 
//		event.eventID = EVENTID_MONSTER_KILL_QUEST;
//		event.eventDelay = pGCMonsterKillQuestStatus->getRemainTime() * 1000 + 3000;
//		event.eventFlag = EVENTFLAG_QUEST_INFO;
//		event.eventType = EVENTTYPE_ZONE;
//		event.parameter1 = timeGetTime();// + 8000;
//		event.parameter2 = pGCMonsterKillQuestStatus->getQuestID();
//		event.parameter3 = pGCMonsterKillQuestStatus->getCurrentNum();
//		event.parameter4 = pGCMonsterKillQuestStatus->getRemainTime() + (timeGetTime() / 1000);
//		g_pEventManager->AddEvent( event );
//	}

#endif

	__END_CATCH
}
