//////////////////////////////////////////////////////////////////////
//
// Filename    : GCGQuestStatusInfoHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCGQuestStatusInfo.h"
#include "ClientDef.h"
#include "Client.h"
#include "vs_ui.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void GCGQuestStatusInfoHandler::execute ( GCGQuestStatusInfo * pGCGQuestStatusInfo , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
//	__BEGIN_DEBUG_EX
		
#ifdef __GAME_CLIENT__
	if(gC_vs_ui.IsRunningQuestManager()) 
		gC_vs_ui.CloseQuestManager();
	gC_vs_ui.RunQuestManager(); // ui setting - load xml file

	std::list<QuestStatusInfo*>	Infos = pGCGQuestStatusInfo->getInfos();
	std::list<QuestStatusInfo*>::iterator itr = Infos.begin(); 

	int MaxSize = Infos.size();
//#ifdef __DEBUG_OUTPUT__
	DEBUG_ADD_FORMAT("[Receive GQuestStatusInfo] MaxSize = %d", MaxSize); 
//#endif

	//while(itr != Infos.end())
	for(int TempCount = 0; TempCount<MaxSize; TempCount++)
	{
		QuestStatusInfo* QuestInfo = (*itr);

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

		if(false == gC_vs_ui.SetQuestManagerInfo((void*)UI_Info))
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
		itr++;
	}

	gC_vs_ui.ReleaseQuestXML(); // Release xml file
#endif

	//__END_DEBUG_EX 
	__END_CATCH
}
