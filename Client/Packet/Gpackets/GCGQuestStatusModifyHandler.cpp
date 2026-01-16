//////////////////////////////////////////////////////////////////////
//
// Filename    : GCGQuestStatusModifyHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCGQuestStatusModify.h"
#include "Client.h"
#include "vs_ui.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCGQuestStatusModifyHandler::execute ( GCGQuestStatusModify * pGCGQuestStatusModify , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY 
//	__BEGIN_DEBUG_EX
		
#ifdef __GAME_CLIENT__ 
	QuestStatusInfo* QuestInfo = pGCGQuestStatusModify->getInfo(); 

	if(NULL != QuestInfo)
	{
		UI_GQuestInfo *UI_Info = new UI_GQuestInfo; 

		UI_Info->dwQuestID	= QuestInfo->GetQuestID(); 
		UI_Info->bStatus	= QuestInfo->GetStatus();

		std::list<MissionInfo> TempMissionInfo = QuestInfo->GetMission();
		std::list<MissionInfo>::iterator subitr = TempMissionInfo.begin();

		while(subitr != TempMissionInfo.end())
		{
			MissionInfo TempMission = (*subitr);

			UI_GMissionInfo* UI_Mission = new UI_GMissionInfo;
			UI_Mission->bCondition = TempMission.m_Condition;
			UI_Mission->bStatus = TempMission.m_Status;
			UI_Mission->bIndex = TempMission.m_Index;
			UI_Mission->m_StrArg = TempMission.m_StrArg;
			UI_Mission->m_NumArg = TempMission.m_NumArg;
			UI_Mission->dwTimeLimit = 0;

			UI_Info->vMissionList.push_back(UI_Mission);

			subitr++;
		}
 
		if(false == gC_vs_ui.ModifyQuestManagerInfo((void*)UI_Info, pGCGQuestStatusModify->getType()-2))
		{ 
			std::vector<UI_GMissionInfo*>::iterator subitr1 = UI_Info->vMissionList.begin();

			while(subitr1 != UI_Info->vMissionList.end())
			{
				UI_GMissionInfo *TempMission1 = (*subitr1);
				UI_Info->vMissionList.erase(subitr1);
				SAFE_DELETE(TempMission1);
			}
			SAFE_DELETE(UI_Info);
		}
	}
#endif

//	__END_DEBUG_EX
	__END_CATCH
}
