 //-----------------------------------------------------------------------------
// GameUI.cpp`
//-----------------------------------------------------------------------------
// UI의 message를 처리하는 부분이다.
//-----------------------------------------------------------------------------
#include "Client_PCH.h"

#include <Winuser.h>

#include "Client.h"
#include "UIFunction.h"
#include "VS_UI.h" 
#include "VS_UI_Mouse_pointer.h"
#include "PacketDef.h"
#include "TalkBox.h"
#include "MShopShelf.h"
#include "MPriceManager.h"
#include "MMoneyManager.h"
#include "UIDialog.h"
#include "Packet\PCSlayerInfo.h"
#include "Packet\PCVampireInfo.h"
#include "Packet\PCOustersInfo.h"
#include "SkillDef.h"
#include "ClientFunction.h"
#include "MGameStringTable.h"
#include "MItemOptionTable.h"
#include "CToken.h"
#include "UserOption.h"
#include "UtilityFunction.h"
#include "MWeather.h"
#include "MStatusManager.h"
#include "MChatManager.h"
#include "UIMessageManager.h"
#include "ServerInformation.h"
#include "UserInformation.h"
#include "TempInformation.h"
#include "ClientConfig.h"
#include "MMusic.h"
//#include "MFileDef.h"
#include "Properties.h"
#include "ServerInfo.h"
#include "DebugInfo.h"
#include "GuildInfo.h"
#include "MGuildInfoMapper.h"
//#include "KeyAccelerator.h"
#include "Gpackets/GCWaitGuildList.h"
#include "Gpackets/GCActiveGuildList.h"
#include "Gpackets/GCGuildMemberList.h"
#include "Gpackets/GCShowWaitGuildInfo.h"
#include "Gpackets/GCShowGuildInfo.h"
#include "Gpackets/GCShowGuildMemberInfo.h"
#include "Gpackets/GCWarScheduleList.h"
#include "Gpackets/GCBloodBibleStatus.h"
#include "Gpackets/GCSelectQuestID.h"
#include "Gpackets/GCQuestStatus.h"
#include "Gpackets/GCGoodsList.h"
#include "Gpackets/GCShowUnionInfo.h"
#include "CMP3.h"
#include "AddonDef.h"
#include "VS_UI_filepath.h"
#include "PacketFunction.h"
#include "MFakeCreature.h"
#include "MMonsterKillQuestInfo.h"
#include "MZoneTable.h"
#include <string>

#include "SoundSetting.h"
#include "packet/GPackets/GCMiniGameScores.h"
#include "SystemAvailabilities.h"
#include "VS_UI_GameCommon2.h"
//add by viva : include friend file
#include "Gpackets/GCFriendChatting.h"
//#include "mintr.h"
#include "MGameTime.h"
#ifdef OUTPUT_DEBUG
	#include <io.h>
#endif

#include "packet/Cpackets/CLGetWorldList.h"

extern Race g_eRaceInterface;
extern BOOL g_MyFull;
extern RECT g_GameRect;
BOOL	g_bSetHotKey = FALSE;

BOOL g_bDrawProgress = FALSE;


extern IWebBrowser2*			g_pWebBrowser; 

///////////////////////////////////////////////////////////////////////////////
//UI friend system
//add by viva
///////////////////////////////////////////////////////////////////////////////
extern C_VS_UI_FRIEND_INFO* gpC_vs_ui_friend_info;
void
UI_FriendChatting_UpdateFriendList(GCFriendChatting* pPacket)
{
	C_VS_UI_FRIEND_INFO::FRIEND_LIST* pList = NULL;
	if(pPacket->getIsBlack() == 0)
	{
		pList = (C_VS_UI_FRIEND_INFO::FRIEND_LIST*)gpC_vs_ui_friend_info->GetFriendList(pPacket->getPlayerName());
		if(pList)
			pList->blIsOnline = pPacket->getIsOnLine();
		else
		{
			pList = new C_VS_UI_FRIEND_INFO::FRIEND_LIST;
			pList->Name = pPacket->getPlayerName();
			pList->blIsOnline = pPacket->getIsOnLine();
			gpC_vs_ui_friend_info->m_v_pFriendList.push_back(pList);
		}
	}
	else
	{
		pList = (C_VS_UI_FRIEND_INFO::FRIEND_LIST*)gpC_vs_ui_friend_info->GetBlackList(pPacket->getPlayerName());
		if(pList)
			pList->blIsOnline = pPacket->getIsOnLine();
		else
		{
			pList = new C_VS_UI_FRIEND_INFO::FRIEND_LIST;
			pList->Name = pPacket->getPlayerName();
			pList->blIsOnline = pPacket->getIsOnLine();
			gpC_vs_ui_friend_info->m_v_pBlackList.push_back(pList);
		}
	}
	gpC_vs_ui_friend_info->SortOrder();
}

void UI_FriendChatting_Message(GCFriendChatting* pPacket)
{
	C_VS_UI_FRIEND_INFO::FRIEND_LIST* pList = (C_VS_UI_FRIEND_INFO::FRIEND_LIST*)gpC_vs_ui_friend_info->GetFriendList(pPacket->getPlayerName());
	if(pList)
	{
		gpC_vs_ui_friend_info->AddChattingMessageToHistory(pList, pPacket->getPlayerName(), pPacket->getMessage());
		pList->blIsShow = 1;
		gpC_vs_ui_friend_info->SortOrder();
		if(!gpC_vs_ui_friend_info->Running())
			gpC_vs_ui_friend_info->Start(true);
	}
}
//--------------------------------------------------------------------------------
//UI_Friend_REQUEST
//
//add by viva
//--------------------------------------------------------------------------------
void UI_RunFriendRequest(GCFriendChatting* pPacket)
{
	char* pName = new char[pPacket->getPlayerName().size()+1];
	strcpy(pName, pPacket->getPlayerName().c_str());
	gC_vs_ui.RunFriendRequestAsk(pName);
}
//void UI_CanncelFriendRequest(C_VS_UI_ASK_DIALOG* pDialog)
//{
////	gC_vs_ui.CanncelFriendRequest(pDialog);
//}
//void UI_CloseFriendRequest(C_VS_UI_ASK_DIALOG* pDialog)
//{
////	gC_vs_ui.CloseFriendRequest(pDialog);
//}
void UI_RunFriendRefuse(GCFriendChatting* pPacket)
{
	char* pName = new char[pPacket->getPlayerName().size()+1];
	strcpy(pName, pPacket->getPlayerName().c_str());
	gC_vs_ui.RunFriendOK();
	gC_vs_ui.RunFriendRefuseAsk(pName);
}
void UI_RunFriendWait(GCFriendChatting* pPacket)
{
	char* pName = new char[pPacket->getPlayerName().size()+1];
	strcpy(pName, pPacket->getPlayerName().c_str());
	gC_vs_ui.RunFriendWaitAsk(pName);
}
void UI_RunFriendOK(GCFriendChatting* pPacket)
{
	gC_vs_ui.RunFriendOK();
	gpC_base->SendMessage(UI_FRIEND_CHATTING_UPDATE, 0, 0, NULL);
}
void UI_RunFriendExist(GCFriendChatting* pPacket)
{
	char* pName = new char[pPacket->getPlayerName().size()+1];
	strcpy(pName, pPacket->getPlayerName().c_str());
	gC_vs_ui.RunFriendExistAsk(pName);
}
void UI_RunFriendBlack(GCFriendChatting* pPacket)
{
	char* pName = new char[pPacket->getPlayerName().size()+1];
	strcpy(pName, pPacket->getPlayerName().c_str());
	gC_vs_ui.RunFriendBlackAsk(pName);
}
void UI_RunFriendDelete(GCFriendChatting* pPacket)
{
	//C_VS_UI_FRIEND_INFO::FRIEND_LIST* pList = (C_VS_UI_FRIEND_INFO::FRIEND_LIST*)gpC_vs_ui_friend_info->GetFriendList(pPacket->getPlayerName());
	//if(pList)
	//{
	//	std::vector<C_VS_UI_FRIEND_INFO::FRIEND_LIST*>::iterator it = find(gpC_vs_ui_friend_info->m_v_pFriendList.begin(), gpC_vs_ui_friend_info->m_v_pFriendList.end(), pList);
	//	if(it)
	//		gpC_vs_ui_friend_info->m_v_pFriendList.erase(it);
	//}
	//	//gpC_vs_ui_friend_info->m_v_pFriendList.remove(pList);
	std::vector<C_VS_UI_FRIEND_INFO::FRIEND_LIST*>::iterator it;
	bool Isfind = false;
	for(it = gpC_vs_ui_friend_info->m_v_pFriendList.begin(); it!=gpC_vs_ui_friend_info->m_v_pFriendList.end(); it++)
	{
		if((*it)->Name == pPacket->getPlayerName())
		{
			Isfind = true;
			break;
		}
	}
	if(Isfind)
		gpC_vs_ui_friend_info->m_v_pFriendList.erase(it);
	else
	{
		for(it = gpC_vs_ui_friend_info->m_v_pBlackList.begin(); it!=gpC_vs_ui_friend_info->m_v_pBlackList.end(); it++)
		{
			if((*it)->Name == pPacket->getPlayerName())
			{
				Isfind = true;
				break;
			}
		}
		if(Isfind)
			gpC_vs_ui_friend_info->m_v_pBlackList.erase(it);
	}

}










//-----------------------------------------------------------------------------
// Is Running SelectWayPoint
//-----------------------------------------------------------------------------
bool
UI_IsRunningSelectWayPoint()
{
	return gC_vs_ui.IsRunningSlayerPortal();
}

//-----------------------------------------------------------------------------
// Run SelectWayPoint
//-----------------------------------------------------------------------------
void
UI_RunSelectWayPoint()
{
	gC_vs_ui.RunSlayerPortal();
}

//-----------------------------------------------------------------------------
// Close SelectWayPoint
//-----------------------------------------------------------------------------
void
UI_CloseSelectWayPoint()
{
	gC_vs_ui.CloseSlayerPortal();
}

//-----------------------------------------------------------------------------
// Save UserOption
//-----------------------------------------------------------------------------
// UI의 UserOption정보를 화일로 저장한다.
//-----------------------------------------------------------------------------
void		
UI_SaveUserOption()
{			
	if (g_pUserOption!=NULL)
	{
//		g_pUserOption->OpenQuickSlot = gC_vs_ui.IsRunningQuickItemSlot();		
//		g_pUserOption->DrawMinimap = gC_vs_ui.IsRunningMinimap();
	
		g_pUserOption->SaveToFile( g_pFileDef->getProperty("FILE_INFO_USEROPTION").c_str());
	}	
}

//-----------------------------------------------------------------------------
// Add EffectStatus
//-----------------------------------------------------------------------------
void		
UI_AffectUserOption()
{
	if (g_pUserOption!=NULL)
	{
//		bool bOldMute = g_DXSound.IsMute();
//
//		g_DXSound.SetMute();

		//--------------------------------------------------------------
		// QuickSlot
		//--------------------------------------------------------------
		//if (g_pUserOption->OpenQuickSlot)
		{
//			gC_vs_ui.RunQuickItemSlot();	
		}
		//else
		{
		//	gC_vs_ui.CloseQuickItemSlot();
		}

		//--------------------------------------------------------------
		// Minimap
		//--------------------------------------------------------------
//		if (g_pUserOption->DrawMinimap)
//		{
//			gC_vs_ui.RunMinimap();	
//		}
//		else
//		{
//			gC_vs_ui.CloseMinimap();
//		}
//
//		if (!bOldMute)		
//		{
//			g_DXSound.UnSetMute();
//		}
//		if(g_pUserOption->UseEnterChat)
//			g_pKeyAccelerator->SetEnterChat();
//		else
//			g_pKeyAccelerator->UnsetEnterChat();
	}
}

//-----------------------------------------------------------------------------
// Add EffectStatus
//-----------------------------------------------------------------------------
void		
UI_AddEffectStatus(int es, DWORD delayFrame)
{
	if (es < g_pEffectStatusTable->GetSize())
	{
		TYPE_ACTIONINFO ai = (*g_pEffectStatusTable)[es].OriginalActionInfo;

		//---------------------------------------------------------------
		// EffectStatus와 관련된 ActionInfo가 있는 경우
		//---------------------------------------------------------------
		if (ai < g_pActionInfoTable->GetMinResultActionInfo())
		{
			S_SLOT::UI_EFFECTSTATUS_TYPE& status = g_char_slot_ingame.STATUS;

			//-----------------------------------------------------------------
			// 이미 있는지 검사.. 음.. vector라.. - -;
			//-----------------------------------------------------------------
			S_SLOT::UI_EFFECTSTATUS_TYPE::iterator itr = status.begin(); 
			S_SLOT::UI_EFFECTSTATUS_TYPE::iterator endItr = status.end();

			while (itr != status.end())
			{
				//-----------------------------------------------------------------
				// 이미 있으면 return이당..
				//-----------------------------------------------------------------
				if (itr->actionInfo == ai)
				{
					// 2004, 6, 11 sobeit add start - 옵저빙아이,블레스 등등, delay 버그때문에 delay만 갱신
					if(delayFrame)
						(itr->delayFrame = timeGetTime()+delayFrame*1000/16);
					// 2004, 6, 11 sobeit add end
					return;
				}

				itr++;
			}

			//-----------------------------------------------------------------
			// 없으면 추가한다.
			//-----------------------------------------------------------------
			S_SLOT::UI_EFFECTSTATUS_STRUCT efs;
			efs.actionInfo = ai;
			efs.delayFrame = timeGetTime()+delayFrame*1000/16;

			status.push_back( efs );
			if(ai == SKILL_BLOOD_DRAIN)
			{
				g_char_slot_ingame.bl_drained = true;
				// 2004, 5, 7, sobeit add start - 흡혈 당했을 때 도움말을 보여준다.
			//	ExecuteHelpEvent( HELP_EVENT_DRAIN_BLOOD ); 
				// 2004, 5, 6, sobeit add end
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Add EffectStatus
//-----------------------------------------------------------------------------
void		
UI_RemoveEffectStatus(int es)
{
	if (es < g_pEffectStatusTable->GetSize())
	{
		int ai = (*g_pEffectStatusTable)[es].OriginalActionInfo;

		//---------------------------------------------------------------
		// EffectStatus와 관련된 ActionInfo가 있는 경우
		//---------------------------------------------------------------
		if (ai < g_pActionInfoTable->GetMinResultActionInfo())
		{
			S_SLOT::UI_EFFECTSTATUS_TYPE& status = g_char_slot_ingame.STATUS;

			//-----------------------------------------------------------------
			// 이미 있는지 검사.. map으로 바꾸고 싶지만.. 걍.. - -;
			//-----------------------------------------------------------------
			S_SLOT::UI_EFFECTSTATUS_TYPE::iterator itr = status.begin();
			S_SLOT::UI_EFFECTSTATUS_TYPE::iterator endItr = status.end();

			while (itr != endItr)
			{
				//-----------------------------------------------------------------
				// 있는 거면..
				//-----------------------------------------------------------------
				if (itr->actionInfo == ai)
				{
					status.erase( itr );

				if(ai == SKILL_BLOOD_DRAIN)
					g_char_slot_ingame.bl_drained = false;

					// 더 체크할 필요없다.
					return;
				}

				itr++;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Set WorldList
//-----------------------------------------------------------------------------
void
UI_SetWorldList()
{	
	// 넷마블용
	if(g_pUserInformation->IsNetmarble)
	{
		gpC_base->SendMessage(UI_CONNECT_SERVER, true, g_pUserInformation->WorldID);
		return;
	}

	if (g_pServerInformation!=NULL)
	{
		// true면 group을 선택하는 거다.
		gC_vs_ui.StartServerSelect( true );

//		gC_vs_ui.CharManagerEnable();

		const int numGroup = g_pServerInformation->size();

		char** groupName = new char* [numGroup];
		int* groupID = new int [numGroup];
		int* groupStatus = new int [numGroup];

		ServerInformation::const_iterator iGroup = g_pServerInformation->begin();

		//-----------------------------------------------------
		// UI에 넘겨줄 server정보 생성
		//-----------------------------------------------------
		for (int i=0; i<numGroup; i++)
		{			
			const char* groupNameOrg = iGroup->second->GetGroupName();
			int status = iGroup->second->GetGroupStatus();
		
			groupName[i] = new char[strlen(groupNameOrg) + 1];

			groupID[i] = iGroup->first;			
			strcpy(groupName[i], groupNameOrg);
			groupStatus[i] = status;

			iGroup ++;
		}			

		//-----------------------------------------------------
		// UI에 server list를 보낸다.
		//-----------------------------------------------------		
		int defaultID = g_pServerInformation->GetServerGroupID();
		gC_vs_ui.SetServerList(groupName, groupID, groupStatus, numGroup, defaultID);

		//-----------------------------------------------------
		// 메모리에서 제거
		//-----------------------------------------------------
		for (i=0; i<numGroup; i++)
		{
			delete [] groupName[i];
		}

		delete [] groupName;
		delete [] groupID;
		delete [] groupStatus;

	}
	
}

//-----------------------------------------------------------------------------
// Set ServerList
//-----------------------------------------------------------------------------
void
UI_SetServerList()
{
	// 넷마블용
	if(g_pUserInformation->IsNetmarble)
	{
		gpC_base->SendMessage(UI_CONNECT_SERVER, false, g_pUserInformation->ServerID);
		return;
	}

	if (g_pServerInformation!=NULL)
	{
		// false면 server를 선택하는 거다.
		gC_vs_ui.StartServerSelect( false );

//		gC_vs_ui.CharManagerEnable();
	
		int groupID = g_pServerInformation->GetServerGroupID();
		ServerGroup* pServerGroup = g_pServerInformation->GetData( groupID );

		const int numServer = pServerGroup->size();

		char** serverName = new char* [numServer];
		int* serverID = new int [numServer];
		int* serverStatus = new int [numServer];

		ServerGroup::const_iterator iServer = pServerGroup->begin();

		//-----------------------------------------------------
		// UI에 넘겨줄 server정보 생성
		//-----------------------------------------------------		
		for (int i=0; i<numServer; i++)
		{			
			SERVER_INFO* pServerInfo = iServer->second;

			const char* serverNameOrg = pServerInfo->ServerName.GetString();
			int status = pServerInfo->ServerStatus;

			serverName[i] = new char[strlen(serverNameOrg) + 1];

			serverID[i] = iServer->first;			
			strcpy(serverName[i], serverNameOrg);
			serverStatus[i] = status;

			iServer ++;
		}			

		//-----------------------------------------------------
		// UI에 server list를 보낸다.
		//-----------------------------------------------------		
		int defaultID = g_pServerInformation->GetServerID();
		gC_vs_ui.SetServerList(serverName, serverID, serverStatus, numServer, defaultID);

		//-----------------------------------------------------
		// 메모리에서 제거
		//-----------------------------------------------------
		for (i=0; i<numServer; i++)
		{
			delete [] serverName[i];
		}

		delete [] serverName;
		delete [] serverID;
		delete [] serverStatus;

	}
	
}

//-----------------------------------------------------------------------------
// Set Current ServerName
//-----------------------------------------------------------------------------
void	
UI_SetCurrentServerName(const char *name, int status)
{
//	gC_vs_ui.SetCurrentServerName(name, (C_VS_UI_SERVER_SELECT::SERVER_STATUS)status);
}

//-----------------------------------------------------------------------------
// IsRunningSelectExplosive
//-----------------------------------------------------------------------------
BOOL		
UI_IsRunningSelectExplosive()
{
//	return gC_vs_ui.IsRunningSelectExplosive();
	return FALSE;
}

//-----------------------------------------------------------------------------
// UI_RunSelectExplosive
//-----------------------------------------------------------------------------
void		
UI_RunSelectExplosive()
{
//	gC_vs_ui.RunSelectExplosive();
}

//-----------------------------------------------------------------------------
// UI_RunSelectBomb
//-----------------------------------------------------------------------------
void		
UI_RunSelectBomb()
{
//	gC_vs_ui.RunSelectBomb();
}

//-----------------------------------------------------------------------------
// UI_RunSelectMine
//-----------------------------------------------------------------------------
void		
UI_RunSelectMine()
{
//	gC_vs_ui.RunSelectMine();
}

//-----------------------------------------------------------------------------
// UI_CloseSelectExplosive
//-----------------------------------------------------------------------------
void		
UI_CloseSelectExplosive()
{
//	gC_vs_ui.CloseSelectExplosive();
}

//-----------------------------------------------------------------------------
// RemoveDescriptor
//-----------------------------------------------------------------------------
void		
UI_RemoveDescriptor(void* pPtr)
{
	g_descriptor_manager.Unset( pPtr );
}

//-----------------------------------------------------------------------------
// Is Running Interface
//-----------------------------------------------------------------------------
BOOL
UI_IsRunningExchange()
{
	return gC_vs_ui.IsRunningExchange();
}

//-----------------------------------------------------------------------------
// Is Running Storage
//-----------------------------------------------------------------------------
BOOL
UI_IsRunningStorage()
{
	return gC_vs_ui.IsRunningStorage();
}

//-----------------------------------------------------------------------------
// Is Running Shop
//-----------------------------------------------------------------------------
BOOL
UI_IsRunningShop()
{
	return gC_vs_ui.IsRunningShop();
}

//-----------------------------------------------------------------------------
// Is Running Bookcase
//-----------------------------------------------------------------------------
BOOL
UI_IsRunningBookcase()
{
	return gC_vs_ui.IsRunningBookcase();
}

//-----------------------------------------------------------------------------
// Is Running Repair
//-----------------------------------------------------------------------------
BOOL 
UI_IsRunningRepair()
{
	return gC_vs_ui.IsRunningRepair();
}

//-----------------------------------------------------------------------------
// Is Running Silvering
//-----------------------------------------------------------------------------
BOOL 
UI_IsRunningSilvering()
{
	return gC_vs_ui.IsRunningSilvering();
}


//-----------------------------------------------------------------------------
// Is Running Sell
//-----------------------------------------------------------------------------
BOOL
UI_IsRunningSell()
{
	return gC_vs_ui.IsRunningSell();
}

//-----------------------------------------------------------------------------
// Is Running Tutorial Exit
//-----------------------------------------------------------------------------
BOOL		
UI_IsRunningTutorialExit()
{
	//return gC_vs_ui.IsRunningTutorialExit();
	return TRUE;
}

//-----------------------------------------------------------------------------
// Is Running Briefing
//-----------------------------------------------------------------------------
BOOL		
UI_IsRunningBriefing()
{
	//return gC_vs_ui.IsRunningBriefing();
	return TRUE;
}

//-----------------------------------------------------------------------------
// Is Running Computer
//-----------------------------------------------------------------------------
BOOL		
UI_IsRunningComputer()
{
	return gC_vs_ui.IsRunningComputer();
}

//-----------------------------------------------------------------------------
// Is Run Briefing
//-----------------------------------------------------------------------------
void		
UI_RunBriefing()
{
	if (g_pUIDialog)
	{
	//	g_pUIDialog->HidePCTalkDlg();
	}

	//gC_vs_ui.RunBriefing();
}

//-----------------------------------------------------------------------------
// Is Run Computer
//-----------------------------------------------------------------------------
void		
UI_RunComputer()
{
	DEBUG_ADD("UI_RunComputer()");

	if (g_pUIDialog)
	{
		DEBUG_ADD("HidePCTalkDlg");
		g_pUIDialog->HidePCTalkDlg();
	}

	DEBUG_ADD("gC_vs_ui.RunComputer()");
	gC_vs_ui.RunComputer();

	DEBUG_ADD("g_pUserInformation keep");

	// 접속 유지
	g_pUserInformation->KeepConnection = TRUE;

	DEBUG_ADD("UI_RunComputer OK");
}

//-----------------------------------------------------------------------------
// Is Run Tutorial Exit
//-----------------------------------------------------------------------------
void		
UI_RunTutorialExit()
{
	//gC_vs_ui.RunTutorialExit();
}

//-----------------------------------------------------------------------------
// Is Close Briefing
//-----------------------------------------------------------------------------
void		
UI_CloseBriefing()
{
	if (g_pUIDialog)
	{
		//g_pUIDialog->ShowPCTalkDlg();
	}

	//gC_vs_ui.CloseBriefing();
}

//-----------------------------------------------------------------------------
// Is Close Computer
//-----------------------------------------------------------------------------
void		
UI_CloseComputer()
{
	gC_vs_ui.CloseComputer();

	if (g_pUIDialog)
	{
		g_pUIDialog->ShowPCTalkDlg();
	}

	// 접속 해제
	g_pUserInformation->KeepConnection = FALSE;
}

//-----------------------------------------------------------------------------
// Is Close Tutorial Exit
//-----------------------------------------------------------------------------
void		
UI_CloseTutorialExit()
{
	//gC_vs_ui.CloseTutorialExit();
}

//--------------------------------------------------------------------------------
// UI_BackupID
//--------------------------------------------------------------------------------
void		
UI_RunExchangeAsk(TYPE_OBJECTID otherID)
{
	// by csm 12/4
	MCreature* pCreature = g_pZone->GetCreature( otherID );
	if(pCreature == NULL)
	{
		DEBUG_ADD_FORMAT( "[Error] UI_RunExchangeAsk - There is no such Creature. id=%d", otherID);
		return;
	}
	if(pCreature->CurPernalShop() != 1)
	{
		gC_vs_ui.RunExchangeAsk( pCreature->GetName() );
		
		// 교환중이라는 의미..
		g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_TRADE );
		
		g_pTempInformation->SetMode(TempInformation::MODE_TRADE_REQUEST);
		g_pTempInformation->Value1 = otherID;
		
		// [도움말] 교환신청 받을 때
		//		__BEGIN_HELP_EVENT
		//			ExecuteHelpEvent( HE_TRADE_REQUESTED );	
		//		__END_HELP_EVENT
	}
	else
	{
		DEBUG_ADD_FORMAT( "[Error] UI_RunExchangeAsk - Failed , pCreature is using persnal shop.");
	}
}

//-----------------------------------------------------------------------------
// Run ExchangeCancel
//-----------------------------------------------------------------------------
void
UI_RunExchangeCancel(const char* pName)
{
	gC_vs_ui.RunExchangeCancel(pName);

	// [도움말] 교환신청 하고 나서
//	__BEGIN_HELP_EVENT
//		ExecuteHelpEvent( HE_TRADE_REQUEST );	
//	__END_HELP_EVENT
}

//-----------------------------------------------------------------------------
// Run Exchange
//-----------------------------------------------------------------------------
void
UI_RunExchange(TYPE_OBJECTID otherID)
{	
	UI_CloseExchangeCancel();

	//-----------------------------------------------------------
	// 없는 사람이면 안된당..
	//-----------------------------------------------------------
	MCreature* pCreature = g_pZone->GetCreature( otherID );

	if (pCreature!=NULL)
	{
		if (g_pTradeManager!=NULL)
		{
			delete g_pTradeManager;			
		}
		
		g_pTradeManager = new MTradeManager;
		
		g_pTradeManager->Init();
		
		g_pTradeManager->SetOtherID( otherID );
		g_pTradeManager->SetOtherName( pCreature->GetName() );
		
		//-----------------------------------------------------------
		// mouse item을 선택하지 않은 상태로..
		//-----------------------------------------------------------
		MItem* pMouseItem = UI_GetMouseItem();

		if (pMouseItem!=NULL)
		{
			pMouseItem->UnSetTrade();
		}

		//-----------------------------------------------------------
		// inventory의 모든 아이템을 선택하지 않은 상태로 만든다.
		//-----------------------------------------------------------
		g_pInventory->SetBegin();

		while (g_pInventory->IsNotEnd())
		{
			MItem* pItem = g_pInventory->Get();

			pItem->UnSetTrade();
			
			g_pInventory->Next();
		}

		//-----------------------------------------------------------
		// gear의 모든 아이템을 선택하지 않은 상태로 만든다.
		//-----------------------------------------------------------
		MPlayerGear* pGear = g_pPlayer->GetGear();

		pGear->SetBegin();

		while (pGear->IsNotEnd())
		{
			MItem* pItem = pGear->Get();

			pItem->UnSetTrade();
			
			pGear->Next();
		}

	
		//-----------------------------------------------------------
		// 교환창 띄운다
		//-----------------------------------------------------------
		gC_vs_ui.RunExchange();

		// 교환중이라는 의미로 설정해둔다...
		g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_TRADE );
	}
	else
	{
		DEBUG_ADD_FORMAT( "[Error] There is no such Creature. id=%d", otherID);
	}
	
}

//-----------------------------------------------------------------------------
// Run Bookcase
//-----------------------------------------------------------------------------
void
UI_RunBookcase()
{
	gC_vs_ui.RunBookcase();
}

//-----------------------------------------------------------------------------
// Close Bookcase
//-----------------------------------------------------------------------------
void
UI_CloseBookcase()
{
	gC_vs_ui.CloseBookcase();
}

//-----------------------------------------------------------------------------
// Close Shop
//-----------------------------------------------------------------------------
void
UI_CloseShop()
{
	gC_vs_ui.CloseShop();
}

//-----------------------------------------------------------------------------
// Close Storage
//-----------------------------------------------------------------------------
void
UI_CloseStorage()
{
	gC_vs_ui.CloseStorage();

	// 보관함 중지
	if (g_pStorage!=NULL)
	{
		g_pStorage->UnSetActive();
	}

	// storage를 없앤다.
	gC_vs_ui.SetStorage( NULL );

	// 보관함에도 skill icon과 관계된 아이템이 들어갈 수 있다.
	// PacketFunction.cpp에 있다. compile 시간 관계상..
	if (g_pSkillAvailable!=NULL)
	{
		g_pSkillAvailable->SetAvailableSkills();
	}
}


//-----------------------------------------------------------------------------
// Close Exchange
//-----------------------------------------------------------------------------
void
UI_CloseExchange()
{
	if (g_pPlayer!=NULL
		&& g_pClientConfig!=NULL
		&& g_pTempInformation!=NULL
		&& g_pUserInformation!=NULL)
	{
		gC_vs_ui.CloseExchange();

		// 교환끝
		if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_TRADE)
		{
			g_pPlayer->SetWaitVerifyNULL();
		}

		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
		
		if (g_pTradeManager!=NULL)
		{
			delete g_pTradeManager;			
			g_pTradeManager = NULL;

			// trade 후 item떨어지기가 가능해지는 시간
			g_pUserInformation->ItemDropEnableTime = g_CurrentTime 
													+ g_pClientConfig->AFTER_TRADE_ITEM_DROP_DELAY;
		}
	}
}

//-----------------------------------------------------------------------------
// Close Exchange Ask
//-----------------------------------------------------------------------------
void
UI_CloseExchangeAsk()
{
	gC_vs_ui.CloseExchangeAsk();

	// 교환끝
	if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_TRADE)
	{
		g_pPlayer->SetWaitVerifyNULL();
	}

	if (g_pTempInformation->GetMode() == TempInformation::MODE_TRADE_REQUEST)
	{
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
	}

	if (g_pTradeManager!=NULL)
	{
		delete g_pTradeManager;			
		g_pTradeManager = NULL;
	}
}

//-----------------------------------------------------------------------------
// Close Exchange Cancel
//-----------------------------------------------------------------------------
void
UI_CloseExchangeCancel()
{	
	if (g_pTempInformation!=NULL)
	{
		if (g_pTempInformation->GetMode() == TempInformation::MODE_TRADE_REQUEST)
		{
			g_pTempInformation->SetMode(TempInformation::MODE_NULL);
		}

		gC_vs_ui.CloseExchangeCancel();
	}
}

//--------------------------------------------------------------------------------
// UI_BackupID
//--------------------------------------------------------------------------------
void		
UI_BackupLoginID(char *id)
{
	if( id != NULL && strlen( id ) < 20 )
		gC_vs_ui.BackupPrevId( id );
}

//--------------------------------------------------------------------------------
// Close All Dialog
//--------------------------------------------------------------------------------
void
UI_CloseAllDialog()
{
	if (g_pUIDialog!=NULL)
	{
		g_pUIDialog->ClosePCTalkDlg();
		g_pUIDialog->CloseMessageDlg();
//		g_pUIDialog->CloseHelpDlg();
	}

	gC_vs_ui.CloseAllDialog();

	// dialog닫는 함수로 이용되고 있는 중..
	//gC_vs_ui.ServerDisconnectMessage();
}

//--------------------------------------------------------------------------------
// 얻을 때
//--------------------------------------------------------------------------------
void
UI_SaveHotKeyToServer()
{
//	#ifdef	CONNECT_SERVER
//		if (g_pPlayer!=NULL)
//		{
//			//---------------------------------------------------------------
//			// Slayer인 경우
//			//---------------------------------------------------------------
//			if (g_pPlayer->IsSlayer())
//			{
//				CGSetSlayerHotKey _CGSetSlayerHotKey;
//					
//				for (int i=0; i < 4; i++)
//				{
//					int id = gC_vs_ui.GetHotkey((C_VS_UI_SKILL::HOTKEY)i);
//
//					//if (id != NOT_SELECTED)
//					//{
//						// save(i, id)
//						// 선택 안된 것이라도.. 그대로 저장시켰다가 받아오면 된다.
//						_CGSetSlayerHotKey.setHotKey( i, id );
//					//}
//				}
//
//				g_pSocket->sendPacket( &_CGSetSlayerHotKey );
//			}
//			//---------------------------------------------------------------
//			// Vampire인 경우
//			//---------------------------------------------------------------
//			else
//			{
//				CGSetVampireHotKey _CGSetVampireHotKey;
//					
//				for (int i=0; i < 8; i++)
//				{
//					int id = gC_vs_ui.GetHotkey((C_VS_UI_SKILL::HOTKEY)i);
//
//					//if (id != NOT_SELECTED)
//					//{
//						// save(i, id)
//						// 선택 안된 것이라도.. 그대로 저장시켰다가 받아오면 된다.
//						_CGSetVampireHotKey.setHotKey( i, id );
//					//}
//				}
//
//				g_pSocket->sendPacket( &_CGSetVampireHotKey );
//			}
//
//		}
//	#endif
}

//--------------------------------------------------------------------------------
// UI_SetHotkey - 스킬 단축키 지정할때,
//--------------------------------------------------------------------------------
void
UI_SetHotKey(int hotkey, int id)
{
//	if (id>0 && id < (*g_pActionInfoTable).GetMinResultActionInfo())
//	{
//		gC_vs_ui.SetHotkey((C_VS_UI_SKILL::HOTKEY)hotkey, (ACTIONINFO)id); 
//	}
}

//--------------------------------------------------------------------------------
// Get Hotkey
//--------------------------------------------------------------------------------
int		
UI_GetHotKey(int hotkey)
{
//	return gC_vs_ui.GetHotkey((C_VS_UI_SKILL::HOTKEY)hotkey);
	return -1;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoName
//--------------------------------------------------------------------------------
void
UI_LevelUp()
{
	if(g_pPlayer->IsDead() || g_pPlayer->HasEffectStatus(EFFECTSTATUS_COMA))
		return;

	gC_vs_ui.LevelUp();
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoName
//--------------------------------------------------------------------------------
void
UI_SetCharInfoName(char* pName)
{
	g_char_slot_ingame.sz_name = pName;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoCreatureType
//--------------------------------------------------------------------------------
void
UI_SetCharInfoCreatureType(Race eRace)
{
	g_char_slot_ingame.Race = eRace;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoSex
//--------------------------------------------------------------------------------
void
UI_SetCharInfoSex(bool female)
{
	g_char_slot_ingame.bl_female = female;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoLevel
//--------------------------------------------------------------------------------
void
UI_SetCharInfoLevel(int value)
{
	g_char_slot_ingame.level = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoSTR
//--------------------------------------------------------------------------------
void
UI_SetCharInfoSTR(int value)
{
	g_char_slot_ingame.STR_CUR = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoDEX
//--------------------------------------------------------------------------------
void
UI_SetCharInfoDEX(int value)
{
	g_char_slot_ingame.DEX_CUR = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoINT
//--------------------------------------------------------------------------------
void
UI_SetCharInfoINT(int value)
{
	g_char_slot_ingame.INT_CUR = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoSTR MAX
//--------------------------------------------------------------------------------
void
UI_SetCharInfoSTR_MAX(int value)
{
	g_char_slot_ingame.STR_MAX = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoDEX MAX
//--------------------------------------------------------------------------------
void
UI_SetCharInfoDEX_MAX(int value)
{
	g_char_slot_ingame.DEX_MAX = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoINT MAX
//--------------------------------------------------------------------------------
void
UI_SetCharInfoINT_MAX(int value)
{
	g_char_slot_ingame.INT_MAXX = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoSTR_PURE
//--------------------------------------------------------------------------------
void
UI_SetCharInfoSTR_PURE(int value)
{
	g_char_slot_ingame.STR_PURE = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoDEX_PURE
//--------------------------------------------------------------------------------
void
UI_SetCharInfoDEX_PURE(int value)
{
	g_char_slot_ingame.DEX_PURE = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoINT_PURE
//--------------------------------------------------------------------------------
void
UI_SetCharInfoINT_PURE(int value)
{
	g_char_slot_ingame.INT_PURE = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoDAM ( Max, Min )
//--------------------------------------------------------------------------------
void
UI_SetCharInfoDAM(int value, int value2)
{
	if (value2==-1)
	{
		value2 = value;
	}
	
	g_char_slot_ingame.DAM = value;
	g_char_slot_ingame.DAM2 = value2;	
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoDefense
//--------------------------------------------------------------------------------
void
UI_SetCharInfoDefense(int value)
{
	g_char_slot_ingame.DEFENSE = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoProtection
//--------------------------------------------------------------------------------
void
UI_SetCharInfoProtection(int value)
{
	g_char_slot_ingame.PROTECTION = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoTOHIT
//--------------------------------------------------------------------------------
void
UI_SetCharInfoTOHIT(int value)
{
	g_char_slot_ingame.TOHIT = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoSilverDamage
//--------------------------------------------------------------------------------
void
UI_SetCharInfoSilverDamage(int value)
{
	g_char_slot_ingame.SILVER_HP = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoSilverDamage
//--------------------------------------------------------------------------------
void
UI_SetCharInfoSilverDamage(int min_damage, int max_damage)
{
	g_char_slot_ingame.SILVER_DAM = max_damage;
	g_char_slot_ingame.SILVER_DAM2 = min_damage;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoHP
//--------------------------------------------------------------------------------
void
UI_SetCharInfoHP(int value)
{
	g_char_slot_ingame.HP = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoMP
//--------------------------------------------------------------------------------
void
UI_SetCharInfoMP(int value)
{
	g_char_slot_ingame.MP = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoHP_max
//--------------------------------------------------------------------------------
void
UI_SetCharInfoHP_max(int value)
{
	g_char_slot_ingame.HP_MAX = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoMP_max
//--------------------------------------------------------------------------------
void
UI_SetCharInfoMP_max(int value)
{
	g_char_slot_ingame.MP_MAX = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoCC
//--------------------------------------------------------------------------------
/*
void
UI_SetCharInfoCC(int value)
{
	g_char_slot_ingame.CC = value;
}
*/

//--------------------------------------------------------------------------------
// UI_SetCharInfoFAME
//--------------------------------------------------------------------------------
void
UI_SetCharInfoFAME(int value)
{
	g_char_slot_ingame.FAME = value;
}

//--------------------------------------------------------------------------------
// UI_SetCharInfoNOTERITY
//--------------------------------------------------------------------------------
/*
void
UI_SetCharInfoNOTERITY(int value)
{
	g_char_slot_ingame.NOTERITY = value;
}
*/


//-----------------------------------------------------------------------------
// UI_SetBonusPoint()
//-----------------------------------------------------------------------------
void
UI_SetBonusPoint(int bp)
{
	g_char_slot_ingame.bonus_point = bp;
}

//-----------------------------------------------------------------------------
// UI_SetSkillBonusPoint()
//-----------------------------------------------------------------------------
void
UI_SetSkillPoint(int sp)
{
	g_char_slot_ingame.skill_point = sp;
}


//-----------------------------------------------------------------------------
// Is Vampire Interface
//-----------------------------------------------------------------------------
Race
UI_GetInterfaceRace()
{
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		return RACE_SLAYER;
		break;

	case RACE_VAMPIRE:
		return RACE_VAMPIRE;
		break;

	case RACE_OUSTERS:
		return RACE_OUSTERS;
		break;
	}
	return RACE_SLAYER;
}
//int
//UI_IsVampireInterface()
//{
//	return gbl_vampire_interface;
//}

//-----------------------------------------------------------------------------
// Already Exist ID Message
//-----------------------------------------------------------------------------
void
UI_AlreadyExistIDMessage()
{
	gC_vs_ui.AleadyExistIdMessage(); // 사용불가		
}

//-----------------------------------------------------------------------------
// No Already Exist ID Message
//-----------------------------------------------------------------------------
void
UI_NoAlreadyExistIDMessage()
{
	gC_vs_ui.NoAleadyExistIdMessage(); // 사용가능
}

//-----------------------------------------------------------------------------
// Disconnect PCS
//-----------------------------------------------------------------------------
/*
void
UI_DisconnectPCS(int slot)
{
	gC_vs_ui.DisconnectPCS(slot);
}
*/

//-----------------------------------------------------------------------------
// PCS Connection Failed
//-----------------------------------------------------------------------------
/*
void
UI_PCSConnectionFailed()
{
	gC_vs_ui.ConnectionFailed();
}
*/

//-----------------------------------------------------------------------------
// CloseUserRegistrationWindow
//-----------------------------------------------------------------------------
void
UI_CloseUserRegistrationWindow()
{
//	gC_vs_ui.CloseUserRegistrationWindow();
}

//-----------------------------------------------------------------------------
// UI_AcquireQuitPCSOnlineModeMessage
//-----------------------------------------------------------------------------
/*
void	
UI_AcquireQuitPCSOnlineModeMessage()
{
	gC_vs_ui.AcquireQuitPCSOnlineModeMessage();
}
*/

//-----------------------------------------------------------------------------
// Please Connect PCS
//-----------------------------------------------------------------------------
/*
void	
UI_PleaseConnectPCS(char * sz_name, long pcs_number, int slot)
{
	gC_vs_ui.PleaseConnectPCS(sz_name, pcs_number, slot);
}
*/

//-----------------------------------------------------------------------------
// OnLine PCS
//-----------------------------------------------------------------------------
/*
void	
UI_OnLinePCS(char* pName, int pcsNumber)
{
	gC_vs_ui.OnLinePCS(pName, pcsNumber);
}
*/

//-----------------------------------------------------------------------------
// AcquirePDSClosedMessage
//-----------------------------------------------------------------------------
/*
void	
UI_AcquirePDSClosedMessage()
{
	gC_vs_ui.AcquirePDSClosedMessage();
}
*/

//-----------------------------------------------------------------------------
// Change To Vampire Interface
//-----------------------------------------------------------------------------
void		
UI_ChangeInterfaceRace(Race race)
{
	//UI_SaveUserOption();

	bool bLevelUp = gC_vs_ui.IsRunningLevelUp();

	switch(race)
	{
	case RACE_SLAYER:
		gC_vs_ui.ChangeToSlayerInterface();
		break;

	case RACE_VAMPIRE:
		gC_vs_ui.ChangeToVampireInterface();
		break;

	case RACE_OUSTERS:
		gC_vs_ui.ChangeToOustersInterface();
		break;
	}

	if(bLevelUp)
		gC_vs_ui.LevelUp();

	// UI에 정보 설정
	int zoneID = (g_bZonePlayerInLarge? g_nZoneLarge : g_nZoneSmall);

	if (g_pZone!=NULL)
	{
		SIZE zoneSize = { g_pZone->GetWidth(), g_pZone->GetHeight() };
		gC_vs_ui.SetZone( zoneID );
		gC_vs_ui.SetSize( zoneSize );

		// 안전지대 정보 등.. 다시 설정해준다.
		LoadZoneInfo(zoneID);
	}

	UI_AffectUserOption();
}

//-----------------------------------------------------------------------------
// Change To Slayer Interface
//-----------------------------------------------------------------------------
//void		
//UI_ChangeToSlayerInterface()
//{
//	//UI_SaveUserOption();
//
//	gC_vs_ui.ChangeToSlayerInterface();
//
//	// UI에 정보 설정
//	int zoneID = (g_bZonePlayerInLarge? g_nZoneLarge : g_nZoneSmall);
//
//	if (g_pZone!=NULL)
//	{
//		SIZE zoneSize = { g_pZone->GetWidth(), g_pZone->GetHeight() };
//		gC_vs_ui.SetZone( zoneID );
//		gC_vs_ui.SetSize( zoneSize );
//
//		// 안전지대 정보 등.. 다시 설정해준다.
//		LoadZoneInfo(zoneID);		
//	}
//	
//	UI_AffectUserOption();
//}

//-----------------------------------------------------------------------------
// UI_GetMouseItem
//-----------------------------------------------------------------------------
MItem*
UI_GetMouseItem()
{
	return gpC_mouse_pointer->GetPickUpItem();
}

//-----------------------------------------------------------------------------
// Get Inventory GridPosition
//-----------------------------------------------------------------------------
POINT	
UI_GetInventoryGridPosition(int x, int y)
{
	Point point = gC_vs_ui.GetInventoryGridPosition(x, y);
	
	POINT pt = { point.x, point.y };

	return pt;
}

//-----------------------------------------------------------------------------
// Get Inventory Position
//-----------------------------------------------------------------------------
POINT	
UI_GetInventoryPosition()
{
	Point point = gC_vs_ui.GetInventoryPosition();	

	POINT pt = { point.x, point.y };

	return pt;
}

//-----------------------------------------------------------------------------
// Lock Item Trade
//-----------------------------------------------------------------------------
void
UI_LockItemTrade()
{
	DEBUG_ADD("[UI] Lock ItemTrade");

	gC_vs_ui.LockItemTrade();
}

//-----------------------------------------------------------------------------
// UnLock Item Trade
//-----------------------------------------------------------------------------
void
UI_UnlockItemTrade()
{
	DEBUG_ADD("[UI] Unlock ItemTrade");
	
	gC_vs_ui.UnlockItemTrade();
}

//-----------------------------------------------------------------------------
//  Run Skill Tree ( domain )
//-----------------------------------------------------------------------------
void
UI_RunSkillTree(int domain, int maxLevel)
{
	/*
	BOOL bExistSkillTree = FALSE;
	// 적절한 SkillTree를 띄운다.
	SKILLDOMAIN sd = (enum SKILLDOMAIN)domain;

	switch (sd)
	{	
		//-----------------------------------------------------
		// 도
		//-----------------------------------------------------
		case SKILLDOMAIN_BLADE : gC_vs_ui.RunBladeSkillTree(); bExistSkillTree=TRUE; break;

		//-----------------------------------------------------
		// 검
		//-----------------------------------------------------
		case SKILLDOMAIN_SWORD : gC_vs_ui.RunSwordSkillTree(); bExistSkillTree=TRUE; break;

		//-----------------------------------------------------
		// 총
		//-----------------------------------------------------
		case SKILLDOMAIN_GUN : 
		//case SKILLDOMAIN_RIFLE : 
			gC_vs_ui.RunGunSkillTree(); bExistSkillTree=TRUE; break;
			
		//-----------------------------------------------------
		// 인챈트
		//-----------------------------------------------------
		case SKILLDOMAIN_ENCHANT : gC_vs_ui.RunEnchantSkillTree(); bExistSkillTree=TRUE; break;

		//-----------------------------------------------------
		// 힐
		//-----------------------------------------------------
		case SKILLDOMAIN_HEAL : gC_vs_ui.RunHealSkillTree(); bExistSkillTree=TRUE; break;

		//-----------------------------------------------------
		// 뱀파이어
		//-----------------------------------------------------
		case SKILLDOMAIN_VAMPIRE : gC_vs_ui.RunVampireSkillTree(); bExistSkillTree=TRUE; break;

		case SKILLDOMAIN_ETC : break;		
	}	
	*/

	//gC_vs_ui.RunSkillView();

	//----------------------------------------------------
	// NPC대화하는 dialog를 숨긴다 
	//----------------------------------------------------
	//if (bExistSkillTree)
	//{
	//	g_pUIDialog->HidePCTalkDlg();
	//}	

	//if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_NPC_ASK)
	{
		g_pPlayer->SetWaitVerifyNULL();

		g_pPCTalkBox->Release();		// 

		//---------------------------------------------------
		// normal
		//---------------------------------------------------
		g_pPCTalkBox->SetType( PCTalkBox::SKILL_LEARN );

		//---------------------------------------------------
		// PC Talk Box의 정보 설정
		//---------------------------------------------------
		char str[192];

		//g_pPCTalkBox->SetNPCID( pPacket->getObjectID() );
		//g_pPCTalkBox->SetCreatureType( pCreature->GetCreatureType() );	// 이미 설정되어 있다고 본다.
		//g_pPCTalkBox->SetScriptID( pPacket->getScriptID() );

		//---------------------------------------------------
		// Domain에서 배울 수 있는 기술들 추가
		//---------------------------------------------------
		MSkillDomain& domainInfo = (*g_pSkillManager)[domain];

		int domainLevel;

		if (g_pPlayer->IsSlayer())
		{
			domainLevel = domainInfo.GetDomainLevel();
		}
		else
		{
			domainLevel = g_pPlayer->GetLEVEL();
		}


		int availableSkills = 0;

		domainInfo.SetBegin();

		while (domainInfo.IsNotEnd())
		{
			int skillID = domainInfo.GetSkillID();

			MSkillDomain::SKILLSTATUS status = domainInfo.GetSkillStatus( (ACTIONINFO)skillID );
			int skillLevel = (*g_pSkillInfoTable)[skillID].GetLearnLevel();

			//---------------------------------------------------
			// 아직 안 배운 기술들 중에서..
			// 배울 수 있는 레벨이 된 기술들..
			//---------------------------------------------------
			if ((status==MSkillDomain::SKILLSTATUS_NEXT || status==MSkillDomain::SKILLSTATUS_OTHER)
				&& skillLevel>0 && skillLevel<150 && skillLevel <= domainLevel)
			{
				const char* pSkillName = (*g_pSkillInfoTable)[skillID].GetHName();

				sprintf(str, "%3d%s (%s)", skillID, pSkillName, (*g_pSkillInfoTable)[skillID].GetName() );
				
				if( g_pSystemAvailableManager->GetLimitLearnSkillLevel() >= skillLevel )
					g_pPCTalkBox->AddString( str );

				availableSkills++;
			}
			
			domainInfo.Next();
		}

		// 끝내기 추가
		std::string szMsg;
		szMsg += "999";
		szMsg += (*g_pGameStringTable)[UI_STRING_MESSAGE_CANCEL_LEARN_SKILL].GetString();
		g_pPCTalkBox->AddString( szMsg.c_str() );

		//---------------------------------------------------
		// 스킬 뭐 배울래?
		//---------------------------------------------------
		if (availableSkills==0)
		{
			strcpy(str, (*g_pGameStringTable)[STRING_MESSAGE_NO_SKILL_TO_LEARN].GetString());
		}
		else
		{
			sprintf(str, (*g_pGameStringTable)[STRING_MESSAGE_WHAT_SKILL_TO_LEARN].GetString(), 
							(*g_pGameStringTable)[SKILLDOMAIN_NAME[domain]].GetString());			
		}
		
		g_pPCTalkBox->SetContent( str );

		g_pUIDialog->PopupPCTalkDlg();
	}

}

//-----------------------------------------------------------------------------
// Open Inventory To Sell
//-----------------------------------------------------------------------------
void
UI_OpenInventoryToSell()
{
	// NPC대화하는 dialog를 숨긴다 
	g_pUIDialog->HidePCTalkDlg();

	gC_vs_ui.OpenInventoryToSell();
}

//-----------------------------------------------------------------------------
// Open Inventory To Repair
//-----------------------------------------------------------------------------
void
UI_OpenInventoryToRepair()
{
	// NPC대화하는 dialog를 숨긴다 
	g_pUIDialog->HidePCTalkDlg();

	gC_vs_ui.OpenInventoryToRepair();
}

//-----------------------------------------------------------------------------
// Open Inventory To Silvering
//-----------------------------------------------------------------------------
void
UI_OpenInventoryToSilvering()
{
	// NPC대화하는 dialog를 숨긴다 
	g_pUIDialog->HidePCTalkDlg();

	gC_vs_ui.OpenInventoryToSilvering();
}


//-----------------------------------------------------------------------------
// Close Repair
//-----------------------------------------------------------------------------
void
UI_CloseRepair()
{
	gC_vs_ui.FinishItemRepairing();
}

//-----------------------------------------------------------------------------
// Close Silvering
//-----------------------------------------------------------------------------
void
UI_CloseSilvering()
{
	gC_vs_ui.FinishItemSilvering();
}

//-----------------------------------------------------------------------------
// Close Sell
//-----------------------------------------------------------------------------
void
UI_CloseSell()
{
	gC_vs_ui.FinishItemSelling();
}

//-----------------------------------------------------------------------------
// Run Shop
//-----------------------------------------------------------------------------
void
UI_RunShop()
{
	// NPC대화하는 dialog를 숨긴다
	g_pUIDialog->HidePCTalkDlg();
	// 2004, 8, 17, sobeit add start - 상점창 여는 동시에 esc키 눌러서 치료대화를 이용하는 버그 때문에 추가
	g_pUIDialog->SetLockInputPCTalk();
	// 2004, 8, 17, sobeit add end
	gC_vs_ui.RunShop();
}

//-----------------------------------------------------------------------------
// Set Shop
//-----------------------------------------------------------------------------
void
UI_SetShop(MShop* pShop)
{
	gC_vs_ui.SetShop( pShop );
}

//-----------------------------------------------------------------------------
// Run Storage Buy
//-----------------------------------------------------------------------------
// 보관함 살래말래? 가격은 price
//-----------------------------------------------------------------------------
void		
UI_RunStorageBuy(int price)
{
	// NPC대화하는 dialog를 숨긴다
	g_pUIDialog->HidePCTalkDlg();

	gC_vs_ui.RunStorageBuy( price );
}

//-----------------------------------------------------------------------------
// Run Storage
//-----------------------------------------------------------------------------
void
UI_RunStorage()
{
	// NPC대화하는 dialog를 숨긴다
	g_pUIDialog->HidePCTalkDlg();

	gC_vs_ui.RunStorage();

	// 보관함 작동중이라고 설정한다.
	g_pStorage->SetActive();
}
//-----------------------------------------------------------------------------
// Run Pet Storage
//-----------------------------------------------------------------------------
void
UI_RunPetStorage()
{
//	// NPC대화하는 dialog를 숨긴다
//	g_pUIDialog->HidePCTalkDlg();

	gC_vs_ui.RunPetStorage();

	// 보관함 작동중이라고 설정한다.
	g_pStorage->SetActive();
}
//-----------------------------------------------------------------------------
// Set Storage
//-----------------------------------------------------------------------------
void
UI_SetStorage(MStorage* pStorage)
{
	gC_vs_ui.SetStorage( pStorage );
}
//-----------------------------------------------------------------------------
// Set Storage
//-----------------------------------------------------------------------------
void
UI_SetPetStorage(MStorage* pStorage)
{
	gC_vs_ui.SetPetStorage( pStorage );
}

//-----------------------------------------------------------------------------
// StartCharacterManager
//-----------------------------------------------------------------------------
void
UI_StartCharacterManager()
{
	gC_vs_ui.StartCharacterManager();
}

//-----------------------------------------------------------------------------
// New Character Create OK
//-----------------------------------------------------------------------------
// 새 캐릭터 생성 성공
//-----------------------------------------------------------------------------
void	
UI_NewCharacterCreateOk()
{
	gC_vs_ui.NewCharacterCreateOk();
}

//-----------------------------------------------------------------------------
// New Character Create Failed
//-----------------------------------------------------------------------------
// 새 캐릭터 생성이 실패한 경우
//-----------------------------------------------------------------------------
void	
UI_NewCharacterCreateFailed(int error)
{
	gC_vs_ui.NewCharacterCreateFailed( error );
}

//-----------------------------------------------------------------------------
// Delete Character OK
//-----------------------------------------------------------------------------
// 캐릭터 삭제 성공
//-----------------------------------------------------------------------------
void	
UI_DeleteCharacterOK()
{	
	// 캐릭터 삭제
	gC_vs_ui.DeleteCharacter( g_pUserInformation->Slot );	
}

//-----------------------------------------------------------------------------
// Delete Character Failed
//-----------------------------------------------------------------------------
// 캐릭터 삭제 실패 - 주민등록번호.. 문제. - -;
//-----------------------------------------------------------------------------
void	
UI_DeleteCharacterFailed()
{
	// 주민등록번호가 틀렸을 때의 message
	gC_vs_ui.Invalid_SSN_Message();	
}

//-----------------------------------------------------------------------------
// StartGame
//-----------------------------------------------------------------------------
void
UI_StartGame()
{
	DEBUG_ADD("STARTGAME2");
	gC_vs_ui.StartGame();
	DEBUG_ADD("STARTGAME");

	g_pQuickSlot = NULL;
	g_pArmsBand1 = NULL;
	g_pArmsBand2 = NULL;
	UI_AffectUserOption();
	DEBUG_ADD("AFFECT");

	// minimap은 무조건 켜둔다.
	gC_vs_ui.RunMinimap();	
	DEBUG_ADD("MINIMAP");
}


//-----------------------------------------------------------------------------
// Set NPC Info
//-----------------------------------------------------------------------------
// Zone에 존재하는 NPC에 대한 정보를 설정한다.
//-----------------------------------------------------------------------------
void
UI_SetNPCInfo( const char* pName, int npcID, int x, int y )
{
	DEBUG_ADD_FORMAT("[UI_SetNPCInfo] %s %d %d %d", pName, npcID, x, y);
	gC_vs_ui.SetNPC( x, y, npcID, pName );
}

//-----------------------------------------------------------------------------
// Set Character
//-----------------------------------------------------------------------------
void
UI_SetCharacter(int slotID, PCSlayerInfo * pInfo)
{
	// set character
	S_SLOT slot;
	ZeroMemory(&slot, sizeof(S_SLOT));

	slot.sz_name = g_pUserInformation->Character[slotID];
	slot.sz_guild_name = "";
	slot.Race = RACE_SLAYER;

	short alignment = pInfo->getAlignment();
	if (alignment > 10000)
	{	
		alignment = - (short)(~alignment + 1);
	}

	slot.alignment_num = alignment;
	slot.alignment = (ALIGNMENT)ConvertAlignment( pInfo->getAlignment() );

	slot.bl_female = pInfo->getSex()==FEMALE;	

	slot.HP_MAX = pInfo->getHP(ATTR_MAX);
	slot.HP		= pInfo->getHP(ATTR_MAX);
	slot.MP_MAX = pInfo->getMP(ATTR_MAX);	
	slot.MP		= pInfo->getMP(ATTR_MAX);

	slot.STR_PURE	= pInfo->getSTR();
	slot.DEX_PURE	= pInfo->getDEX();
	slot.INT_PURE	= pInfo->getINT();
	slot.STR_EXP_REMAIN = pInfo->getSTRExp();
	slot.DEX_EXP_REMAIN = pInfo->getDEXExp();
	slot.INT_EXP_REMAIN = pInfo->getINTExp();
	slot.STATUS.clear();

	SKILLDOMAIN weaponDomain[] =
	{
		MAX_SKILLDOMAIN,
		SKILLDOMAIN_SWORD,
		SKILLDOMAIN_BLADE,
		SKILLDOMAIN_GUN,
		SKILLDOMAIN_HEAL,
		SKILLDOMAIN_ENCHANT,
	};

	// 임시!!
	g_StatusManager.SetCurrentWeaponDomain( weaponDomain[pInfo->getWeaponType()], 1 );

	// status 값을 얻어낸다.
	g_StatusManager.Set(pInfo->getSTR(), pInfo->getDEX(), pInfo->getINT());

	//slot.CC		= g_StatusManager.GetCC();
	slot.DAM2	= g_StatusManager.GetMinDAM();
	slot.DAM	= g_StatusManager.GetMaxDAM();
	slot.DEFENSE	= g_StatusManager.GetDefense();
	slot.PROTECTION	= g_StatusManager.GetProtection();
	slot.TOHIT	= g_StatusManager.GetTOHIT();
	slot.m_AdvancementLevel = pInfo->getAdvancementLevel();

	slot.FAME	= pInfo->getFame();
	slot.GRADE	= pInfo->getRank();
	//slot.NOTERITY = pInfo->getNotoriety();

	// 가장 높은 domain level을 찾는다.
	int maxDomainLevel = 1;

	/*
	for (int sd=0; sd<SKILL_DOMAIN_MAX; sd++)
	{
		maxDomainLevel = max( maxDomainLevel, pInfo->getSkillDomainLevel( (SkillDomain)sd ) );
	}
	*/

	slot.level = maxDomainLevel;

	//------------------------------------------------------------
	// domain level 설정
	//------------------------------------------------------------
	slot.DOMAIN_SWORD	= pInfo->getSkillDomainLevel( SKILL_DOMAIN_SWORD );
	slot.DOMAIN_BLADE	= pInfo->getSkillDomainLevel( SKILL_DOMAIN_BLADE );
	slot.DOMAIN_GUN		= pInfo->getSkillDomainLevel( SKILL_DOMAIN_GUN );
	slot.DOMAIN_HEAL	= pInfo->getSkillDomainLevel( SKILL_DOMAIN_HEAL );
	slot.DOMAIN_ENCHANT = pInfo->getSkillDomainLevel( SKILL_DOMAIN_ENCHANT );
	

	//------------------------------------------------------------
	// 복장 정보 설정
	//------------------------------------------------------------
	// 여자인 경우
	//------------------------------------------------------------
	if (slot.bl_female)
	{
		const CHAR_WOMAN uiFaceFemale[] =
		{
			W_FACE1,
			W_FACE2,
			W_FACE3
		};

		const CHAR_WOMAN uiHairFemale[] =
		{
			W_HAIR1,
			W_HAIR2,
			W_HAIR3
		};

		const CHAR_WOMAN uiHelmFemale[] = 
		{
			W_NO_WEAR,
			W_OLD_HELMET,
			W_NEW_HELMET
		};

		const CHAR_WOMAN uiCoatFemale[] =
		{
			W_UP_BODY,
			W_OLD_COAT,
			W_NEW_COAT,
			W_NEW2_COAT,
		};	

		const CHAR_WOMAN uiTrouserFemale[] =
		{
			W_DOWN_BODY,
			W_OLD_TROUSER,
			W_NEW_TROUSER,
			W_NEW2_TROUSER,
		};

		const CHAR_WOMAN uiWeaponFemale[] =
		{
			W_NO_WEAR,
			W_SWORD,
			W_BLADE,
			W_SR,
			W_AR,
			W_SG,
			W_SMG,
			W_CROSS,
			W_MACE
		};

		const CHAR_WOMAN uiShieldFemale[] =
		{
			W_NO_WEAR,
			W_BRONZE_SHIELD,
			W_DRAGON_SHIELD,			
		};

		slot.woman_info.helmet = uiHelmFemale[pInfo->getHelmetType()];
		slot.woman_info.coat = uiCoatFemale[pInfo->getJacketType()];
		slot.woman_info.trouser = uiTrouserFemale[pInfo->getPantsType()];
		slot.woman_info.face = uiFaceFemale[pInfo->getHairStyle()];
		slot.woman_info.hair = uiHairFemale[pInfo->getHairStyle()];
		slot.woman_info.right = uiWeaponFemale[pInfo->getWeaponType()];
		slot.woman_info.left = uiShieldFemale[pInfo->getShieldType()];
	}
	//------------------------------------------------------------
	// 남자인 경우
	//------------------------------------------------------------
	else
	{
		const CHAR_MAN uiFaceMale[] =
		{			
			M_FACE1,
			M_FACE2,
			M_FACE3
		};

		const CHAR_MAN uiHairMale[] =
		{			
			M_HAIR1,
			M_HAIR2,
			M_HAIR3
		};

		const CHAR_MAN uiHelmMale[] = 
		{
			M_NO_WEAR,
			M_OLD_HELMET,
			M_NEW_HELMET
		};

		const CHAR_MAN uiCoatMale[] =
		{
			M_UP_BODY,
			M_OLD_COAT,
			M_NEW_COAT,
			M_NEW2_COAT,
		};

		const CHAR_MAN uiTrouserMale[] =
		{
			M_DOWN_BODY,
			M_OLD_TROUSER,
			M_NEW_TROUSER,
			M_NEW2_TROUSER,
		};

		const CHAR_MAN uiWeaponMale[] =
		{
			M_NO_WEAR,
			M_SWORD,
			M_BLADE,
			M_SR,
			M_AR,
			M_SG,
			M_SMG,
			M_CROSS,
			M_MACE,
		};

		const CHAR_MAN uiShieldMale[] =
		{
			M_NO_WEAR,
			M_BRONZE_SHIELD,
			M_DRAGON_SHIELD			
		};

		slot.man_info.helmet	= uiHelmMale[pInfo->getHelmetType()];
		slot.man_info.coat		= uiCoatMale[pInfo->getJacketType()];
		slot.man_info.trouser	= uiTrouserMale[pInfo->getPantsType()];
		slot.man_info.face = uiFaceMale[pInfo->getHairStyle()];
		slot.man_info.hair = uiHairMale[pInfo->getHairStyle()];
		slot.man_info.right = uiWeaponMale[pInfo->getWeaponType()];
		slot.man_info.left = uiShieldMale[pInfo->getShieldType()];
	}

	// 복장 정보
	/*
	pInfo->getHairStyle();	//	HAIR_STYLE1 , 	HAIR_STYLE2 , 	HAIR_STYLE3 
	pInfo->getHelmetType();	// HELMET_NONE , HELMET1 , HELMET2 ,
	pInfo->getJacketType();	// 	JACKET_BASIC , 	JACKET1 , 	JACKET2 ,
	pInfo->getPantsType(); 	// PANTS_BASIC , 	PANTS1 , 	PANTS2 ,
	*/

	//------------------------------------------------------------
	// 색깔 정보
	//------------------------------------------------------------
	slot.skin_color	= pInfo->getSkinColor();
	slot.hair_color	= pInfo->getHairColor();

	slot.helmet_color = 0;
	slot.left_color = 0;
	slot.right_color = 0;

	if (pInfo->getHelmetType()!=HELMET_NONE)
	{
		if(pInfo->getHelmetColor() == UNIQUE_ITEM_COLOR)
			slot.helmet_color = MItem::GetUniqueItemColorset();
		else
		if(pInfo->getHelmetColor() == QUEST_ITEM_COLOR)
			slot.helmet_color = QUEST_ITEM_COLOR;
		else
			slot.helmet_color = (*g_pItemOptionTable)[pInfo->getHelmetColor()].ColorSet;			
	}
	
	if(pInfo->getPantsColor() == UNIQUE_ITEM_COLOR )
		slot.trouser_color = MItem::GetUniqueItemColorset();
	else
	if(pInfo->getPantsColor() == QUEST_ITEM_COLOR)
		slot.trouser_color = QUEST_ITEM_COLOR;
	else
		slot.trouser_color = (*g_pItemOptionTable)[pInfo->getPantsColor()].ColorSet;	

	if(pInfo->getJacketColor() == UNIQUE_ITEM_COLOR)
		slot.coat_color = MItem::GetUniqueItemColorset( );
	else
	if(pInfo->getJacketColor() == QUEST_ITEM_COLOR)
		slot.coat_color = QUEST_ITEM_COLOR;
	else
		slot.coat_color = (*g_pItemOptionTable)[pInfo->getJacketColor()].ColorSet;
	
	if (pInfo->getShieldType()!=SHIELD_NONE)
	{		
		if(pInfo->getShieldColor() == UNIQUE_ITEM_COLOR)			
			slot.left_color = MItem::GetUniqueItemColorset( );
		else
		if(pInfo->getShieldColor() == QUEST_ITEM_COLOR)
			slot.left_color = QUEST_ITEM_COLOR;
		else
			slot.left_color = (*g_pItemOptionTable)[pInfo->getShieldColor()].ColorSet;
	}
	

	if (pInfo->getWeaponType()!=WEAPON_NONE)
	{
		if( pInfo->getWeaponColor() == UNIQUE_ITEM_COLOR)
			slot.right_color = MItem::GetUniqueItemColorset();
		else		
		if( pInfo->getWeaponColor() == QUEST_ITEM_COLOR)
			slot.right_color = QUEST_ITEM_COLOR;
		else
			slot.right_color = (*g_pItemOptionTable)[pInfo->getWeaponColor()].ColorSet;
	}

	// 색깔 확인
	if(slot.skin_color != UNIQUE_ITEM_COLOR && slot.skin_color != QUEST_ITEM_COLOR)
		slot.skin_color = max(0, min(slot.skin_color, MAX_COLORSET-1));
	if(slot.helmet_color != UNIQUE_ITEM_COLOR && slot.helmet_color != QUEST_ITEM_COLOR)
		slot.helmet_color = max(0, min(slot.helmet_color, MAX_COLORSET-1));
	if(slot.coat_color != UNIQUE_ITEM_COLOR && slot.coat_color != QUEST_ITEM_COLOR)
		slot.coat_color = max(0, min(slot.coat_color, MAX_COLORSET-1));
	if(slot.hair_color != UNIQUE_ITEM_COLOR && slot.hair_color != QUEST_ITEM_COLOR)
		slot.hair_color = max(0, min(slot.hair_color, MAX_COLORSET-1));
	if(slot.left_color != UNIQUE_ITEM_COLOR && slot.left_color != QUEST_ITEM_COLOR)
		slot.left_color = max(0, min(slot.left_color, MAX_COLORSET-1));
	if(slot.right_color != UNIQUE_ITEM_COLOR && slot.right_color != QUEST_ITEM_COLOR)
		slot.right_color = max(0, min(slot.right_color, MAX_COLORSET-1));	
	
	gC_vs_ui.SetCharacter(slotID, slot);
}

//-----------------------------------------------------------------------------
// Set Character
//-----------------------------------------------------------------------------
void
UI_SetCharacter(int slotID, PCVampireInfo * pInfo)
{
	S_SLOT slot;
	ZeroMemory(&slot, sizeof(S_SLOT));

	slot.sz_name = g_pUserInformation->Character[slotID];
	slot.sz_guild_name = "";
//	slot.bl_vampire = true;
	slot.Race = RACE_VAMPIRE;

	slot.bl_female = pInfo->getSex()==FEMALE;	
	
	short alignment = pInfo->getAlignment();
	if (alignment > 10000)
	{	
		alignment = - (short)(~alignment + 1);
	}

	slot.alignment_num = alignment;
	slot.alignment = (ALIGNMENT)ConvertAlignment( pInfo->getAlignment() );

	slot.HP_MAX = pInfo->getHP(ATTR_MAX);		
	slot.HP		= pInfo->getHP(ATTR_MAX);
	slot.MP_MAX = 1;
	slot.MP		= 1;
	
	slot.STR_PURE	= pInfo->getSTR();
	slot.DEX_PURE	= pInfo->getDEX();
	slot.INT_PURE	= pInfo->getINT();
	
	slot.STATUS.clear();

	// status 값을 얻어낸다.
	g_StatusManager.SetCurrentWeaponDomain( SKILLDOMAIN_VAMPIRE, pInfo->getExp() );

	g_StatusManager.Set(pInfo->getSTR(), pInfo->getDEX(), pInfo->getINT());

	//slot.CC		= g_StatusManager.GetCC();
	slot.DAM2	= g_StatusManager.GetMinDAM();
	slot.DAM	= g_StatusManager.GetMaxDAM();
	slot.DEFENSE		= g_StatusManager.GetDefense();
	slot.PROTECTION		= g_StatusManager.GetProtection();
	slot.TOHIT	= g_StatusManager.GetTOHIT();

	slot.level			= pInfo->getLevel();
	slot.bonus_point	= pInfo->getBonus();

	slot.EXP_REMAIN		= pInfo->getExp();
	
	slot.FAME	= pInfo->getFame();
	slot.GRADE	= pInfo->getRank();
//	slot.NOTERITY = pInfo->getNotoriety();

	// coat
	int coatType = pInfo->getCoatType();
	int coatColor = pInfo->getCoatColor();

	// color
	slot.skin_color	= pInfo->getSkinColor();
	slot.hair_color = pInfo->getBatColor();
	slot.m_AdvancementLevel = pInfo->getAdvancementLevel();

	slot.left_color = 0;
	slot.right_color = 0;
	slot.helmet_color = 0;
	slot.trouser_color = 0;

	// 뱀파 옷추가
	int creatureType = 0;
	if(slot.bl_female)
		creatureType = (*g_pItemTable)[ITEM_CLASS_VAMPIRE_COAT][coatType].AddonFemaleFrameID;
	else
		creatureType = (*g_pItemTable)[ITEM_CLASS_VAMPIRE_COAT][coatType].AddonMaleFrameID;

	int spriteType = (*g_pCreatureTable)[creatureType].SpriteTypes[0];
	int coat = (*g_pCreatureSpriteTable)[spriteType].FrameID;
	
	slot.woman_info.helmet = W_NO_WEAR;
	slot.woman_info.coat = (CHAR_WOMAN)coat;		// coatType에 따라서 바꿔야 한다.. 나중에~
	slot.woman_info.trouser = W_NO_WEAR;
	slot.woman_info.face = W_FACE1;	// default
	slot.woman_info.hair = W_NO_WEAR;
	slot.woman_info.right = W_NO_WEAR;
	slot.woman_info.left = W_NO_WEAR;

	slot.man_info.helmet = M_NO_WEAR;
	slot.man_info.coat = (CHAR_MAN)coat;		// coatType에 따라서 바꿔야 한다.. 나중에~
	slot.man_info.trouser = M_NO_WEAR;
	slot.man_info.face = M_FACE1;	// default
	slot.man_info.hair = M_NO_WEAR;
	slot.man_info.right = M_NO_WEAR;
	slot.man_info.left = M_NO_WEAR;

	//------------------------------------------------------------
	// 여자
	//------------------------------------------------------------
	slot.coat_color = coatColor;//(*g_pItemOptionTable)[coatOption].ColorSet;
	/*
	if (slot.bl_female)
	{
		// default .. 나중에 바꿔야 한다.
		slot.coat_color = 38;
	}
	//------------------------------------------------------------
	// 남자
	//------------------------------------------------------------
	else
	{
		// default 
		slot.coat_color = 91;
	}
	*/

	// 색깔 확인
	if(slot.skin_color != UNIQUE_ITEM_COLOR && slot.skin_color != QUEST_ITEM_COLOR)
		slot.skin_color = max(0, min(slot.skin_color, MAX_COLORSET-1));
	if(slot.helmet_color != UNIQUE_ITEM_COLOR && slot.helmet_color != QUEST_ITEM_COLOR)
		slot.helmet_color = max(0, min(slot.helmet_color, MAX_COLORSET-1));
	if(slot.coat_color != UNIQUE_ITEM_COLOR && slot.coat_color != QUEST_ITEM_COLOR)
		slot.coat_color = max(0, min(slot.coat_color, MAX_COLORSET-1));
	if(slot.hair_color != UNIQUE_ITEM_COLOR && slot.hair_color != QUEST_ITEM_COLOR)
		slot.hair_color = max(0, min(slot.hair_color, MAX_COLORSET-1));
	if(slot.left_color != UNIQUE_ITEM_COLOR && slot.left_color != QUEST_ITEM_COLOR)
		slot.left_color = max(0, min(slot.left_color, MAX_COLORSET-1));
	if(slot.right_color != UNIQUE_ITEM_COLOR && slot.right_color != QUEST_ITEM_COLOR)
		slot.right_color = max(0, min(slot.right_color, MAX_COLORSET-1));

	DEBUG_ADD_FORMAT("[UI_SETCHARACTER] %d Before", slotID);
	gC_vs_ui.SetCharacter(slotID, slot);
	DEBUG_ADD_FORMAT("[UI_SETCHARACTER] %d After", slotID);
}

//-----------------------------------------------------------------------------
// Set Character
//-----------------------------------------------------------------------------
void
UI_SetCharacter(int slotID, PCOustersInfo * pInfo)
{
	S_SLOT slot;
	ZeroMemory(&slot, sizeof(S_SLOT));

	slot.sz_name = g_pUserInformation->Character[slotID];
	slot.sz_guild_name = "";
	slot.Race = RACE_OUSTERS;

	short alignment = pInfo->getAlignment();
	if (alignment > 10000)
	{	
		alignment = - (short)(~alignment + 1);
	}

	slot.alignment_num = alignment;
	slot.alignment = (ALIGNMENT)ConvertAlignment( pInfo->getAlignment() );

	slot.HP_MAX = pInfo->getHP(ATTR_MAX);		
	slot.HP		= pInfo->getHP(ATTR_MAX);
	slot.MP_MAX = pInfo->getMP(ATTR_MAX);
	slot.MP		= pInfo->getMP(ATTR_MAX);
	
	slot.STR_PURE	= pInfo->getSTR();
	slot.DEX_PURE	= pInfo->getDEX();
	slot.INT_PURE	= pInfo->getINT();
	
	slot.STATUS.clear();

	// status 값을 얻어낸다.
	g_StatusManager.SetCurrentWeaponDomain( SKILLDOMAIN_OUSTERS, pInfo->getLevel() );

	g_StatusManager.Set(pInfo->getSTR(), pInfo->getDEX(), pInfo->getINT());

	//slot.CC		= g_StatusManager.GetCC();
	slot.DAM2	= g_StatusManager.GetMinDAM();
	slot.DAM	= g_StatusManager.GetMaxDAM();
	slot.DEFENSE		= g_StatusManager.GetDefense();
	slot.PROTECTION		= g_StatusManager.GetProtection();
	slot.TOHIT	= g_StatusManager.GetTOHIT();

	slot.level			= pInfo->getLevel();
	slot.bonus_point	= pInfo->getBonus();
	slot.skill_point	= pInfo->getSkillBonus();

	slot.EXP_REMAIN		= pInfo->getExp();
	
	slot.FAME	= pInfo->getFame();
	slot.GRADE	= pInfo->getRank();
	slot.m_AdvancementLevel = pInfo->getAdvancementLevel();
//	slot.NOTERITY = pInfo->getNotoriety();

	// coat
	int coatType = pInfo->getCoatType();
	int coatColor = pInfo->getCoatColor();
	int weaponType = pInfo->getArmType();
	int weaponColor = pInfo->getArmColor();
	int bootsColor = pInfo->getBootsColor();
	int armColor = pInfo->getArmColor();

	// color
//	slot.skin_color	= pInfo->getSkinColor();
	slot.hair_color = pInfo->getHairColor();

	slot.left_color = 0;
	slot.right_color = 0;
	slot.helmet_color = 0;
	slot.trouser_color = 0;

	// 뱀파 옷추가
//	int creatureType = 0;
//	if(slot.bl_female)
//		creatureType = (*g_pItemTable)[ITEM_CLASS_VAMPIRE_COAT][coatType].AddonFemaleFrameID;
//	else
//		creatureType = (*g_pItemTable)[ITEM_CLASS_VAMPIRE_COAT][coatType].AddonMaleFrameID;
//
//	int spriteType = (*g_pCreatureTable)[creatureType].SpriteType;
	int spriteType = (*g_pItemTable)[ITEM_CLASS_OUSTERS_COAT][g_pPacketItemOustersCoat[coatType]->GetItemType()].AddonMaleFrameID;
	
	slot.woman_info.helmet = W_NO_WEAR;
	slot.woman_info.coat = (CHAR_WOMAN)(spriteType);		// coatType에 따라서 바꿔야 한다.. 나중에~
	slot.woman_info.trouser = W_NO_WEAR;
	slot.woman_info.face = W_FACE1;	// default
	slot.woman_info.hair = W_NO_WEAR;
	slot.woman_info.right = (CHAR_WOMAN)weaponType;
	slot.woman_info.left = (CHAR_WOMAN)weaponType;

	slot.man_info.helmet = M_NO_WEAR;
	slot.man_info.coat = (CHAR_MAN)(spriteType);		// coatType에 따라서 바꿔야 한다.. 나중에~
	slot.man_info.trouser = M_NO_WEAR;
	slot.man_info.face = M_FACE1;	// default
	slot.man_info.hair = M_NO_WEAR;
	slot.man_info.right = (CHAR_MAN)weaponType;
	slot.man_info.left = (CHAR_MAN)weaponType;

	//------------------------------------------------------------
	// 여자
	//------------------------------------------------------------
	slot.coat_color = coatColor;//(*g_pItemOptionTable)[coatOption].ColorSet;
	slot.left_color = weaponColor;
	slot.right_color = weaponColor;
	slot.trouser_color = bootsColor;
	slot.skin_color = armColor;
	/*
	if (slot.bl_female)
	{
		// default .. 나중에 바꿔야 한다.
		slot.coat_color = 38;
	}
	//------------------------------------------------------------
	// 남자
	//------------------------------------------------------------
	else
	{
		// default 
		slot.coat_color = 91;
	}
	*/

	// 색깔 확인
	if(slot.skin_color != UNIQUE_ITEM_COLOR && slot.skin_color != QUEST_ITEM_COLOR)
		slot.skin_color = max(0, min(slot.skin_color, MAX_COLORSET-1));
	if(slot.helmet_color != UNIQUE_ITEM_COLOR && slot.helmet_color != QUEST_ITEM_COLOR)
		slot.helmet_color = max(0, min(slot.helmet_color, MAX_COLORSET-1));
	if(slot.coat_color != UNIQUE_ITEM_COLOR && slot.coat_color != QUEST_ITEM_COLOR)
		slot.coat_color = max(0, min(slot.coat_color, MAX_COLORSET-1));
	if(slot.trouser_color != UNIQUE_ITEM_COLOR && slot.trouser_color != QUEST_ITEM_COLOR)
		slot.trouser_color = max(0, min(slot.trouser_color, MAX_COLORSET-1));
	if(slot.hair_color != UNIQUE_ITEM_COLOR && slot.hair_color != QUEST_ITEM_COLOR)
		slot.hair_color = max(0, min(slot.hair_color, MAX_COLORSET-1));
	if(slot.left_color != UNIQUE_ITEM_COLOR && slot.left_color != QUEST_ITEM_COLOR)
		slot.left_color = max(0, min(slot.left_color, MAX_COLORSET-1));
	if(slot.right_color != UNIQUE_ITEM_COLOR && slot.right_color != QUEST_ITEM_COLOR)
		slot.right_color = max(0, min(slot.right_color, MAX_COLORSET-1));

	DEBUG_ADD_FORMAT("[UI_SETCHARACTER] %d Before", slotID);
	gC_vs_ui.SetCharacter(slotID, slot);
	DEBUG_ADD_FORMAT("[UI_SETCHARACTER] %d After", slotID);
}


//-----------------------------------------------------------------------------
// Start Progress
//-----------------------------------------------------------------------------
void
UI_StartProgress(int zoneID)
{
	DEBUG_ADD("[UI] Start Progress");
	
	//---------------------------------------
	// 음악 멈춘다.
	//---------------------------------------
	if (g_pUserOption!=NULL)
	{
		if (g_pUserOption->PlayWaveMusic)
		{
#ifdef __USE_MP3__
			if(g_pMP3 != NULL)
			{
				DEBUG_ADD("MP3 STOP6");
				g_pMP3->Stop();
				DEBUG_ADD("MP3 STOP6 OK");
			}
//			if (g_pDXSoundStream!=NULL)
//			{
//				g_pDXSoundStream->Stop();
//			}
#else
			if(g_pOGG != NULL )
			{
				if( g_DXSound.IsInit() )
					g_pOGG->streamClose();
			}
#endif
		}
		else
		{
			g_Music.Stop();
		}

		DEBUG_ADD("[UI] Start Progress : Stop Music OK");
	}

	g_bDrawProgress = TRUE;

	DEBUG_ADD("[UI] Start Progress : gC_vs_ui.StartProgress()");
	
	gC_vs_ui.StartProgress();

	DEBUG_ADD("[UI] Start Progress : gC_vs_ui.StartProgress() OK");

	if (g_pServerInformation!=NULL)
	{
		DEBUG_ADD_FORMAT("[UI] SetCurrentServerName : %x %s %s %d", g_pServerInformation, g_pServerInformation->GetServerGroupName(), g_pServerInformation->GetServerName(), g_pServerInformation->GetServerStatus());
		gC_vs_ui.SetCurrentServerName( g_pServerInformation->GetServerGroupName(),
										g_pServerInformation->GetServerName(),
										g_pServerInformation->GetServerStatus() );
		DEBUG_ADD("[UI] SetCurrentServerName OK");
#ifdef OUTPUT_DEBUG
//	extern int CLogFile;
//
//	_write( CLogFile, "<<<", 3);
//	_write( CLogFile, g_pServerInformation->GetServerGroupName(), strlen(g_pServerInformation->GetServerGroupName()) );
//	_write( CLogFile, " - ", 3);
//	_write( CLogFile, g_pServerInformation->GetServerName(), strlen(g_pServerInformation->GetServerName()) );
//	_write( CLogFile, ">>>", 3);
//	_write( CLogFile, "\n", 1 );
#endif
	}

	DEBUG_ADD("[UI] Start Progress : StartProgress OK");
	
	int oldZoneID = (g_bZonePlayerInLarge? g_nZoneLarge : g_nZoneSmall);

	/*
	PROGRESS_SPK_INDEX index;
			
	//----------------------------------------------------------------
	// tutorial 
	//----------------------------------------------------------------
	if (oldZoneID==2106 
		|| zoneID==2106)
	{
		index = TUTORIAL_HELIPORT;
	}
	//----------------------------------------------------------------
	// dungeon
	//----------------------------------------------------------------
	else if (oldZoneID==1002 || oldZoneID==1003 || oldZoneID==1004
			|| zoneID==1002 || zoneID==1003 || zoneID==1004)
	{
		index = DUNGEON_EXIT;
	}
	//----------------------------------------------------------------
	// Slayer random
	//----------------------------------------------------------------
	else if (g_pPlayer!=NULL && g_pPlayer->IsSlayer())
	{
		DEBUG_ADD("[UI] Start Progress : Slayer Random");
		
		const int numSlayerProgress = 2;
		PROGRESS_SPK_INDEX slayerProgress[numSlayerProgress] =
		{
			SLAYER_PROGRESS_1,
			SLAYER_PROGRESS_2
		};
	
		index = slayerProgress[ (PROGRESS_SPK_INDEX)(rand()%numSlayerProgress) ];

		DEBUG_ADD("[UI] Start Progress : Slayer Random OK");
	}
	//----------------------------------------------------------------
	// Vampire random
	//----------------------------------------------------------------
	else
	{
		DEBUG_ADD("[UI] Start Progress : Vampire Random");
		
		const int numVampireProgress = 3;
		PROGRESS_SPK_INDEX vampireProgress[numVampireProgress] =
		{
			VAMPIRE_PROGRESS_1,
			VAMPIRE_PROGRESS_2,
			VAMPIRE_PROGRESS_3
		};

		index = vampireProgress[ (PROGRESS_SPK_INDEX)(rand()%numVampireProgress) ];

		DEBUG_ADD("[UI] Start Progress : Vampire Random OK");
	}
	*/
	DEBUG_ADD("[UI] Start Progress : Set Progress Style");
	
	//gC_vs_ui.SetProgressStyle(index);	
	
	DEBUG_ADD("[UI] Start Progress OK");
}

//-----------------------------------------------------------------------------
// Draw Progess
//-----------------------------------------------------------------------------
void		
UI_DrawProgress(int percent)
{
	DEBUG_ADD_FORMAT("[UI] Draw Progress : %d", percent);
	
	if (g_bDrawProgress)
	{
		gC_vs_ui.SetProgress(percent, 100);
		g_pBack->FillSurface(CDirectDraw::Color(0,0,0));		

		//if (CDirect3D::GetDevice()->BeginScene()!=D3D_OK)
		//{
		//	return;
		//}

//		if (CDirect3D::IsHAL())
//		{
//			if (CDirect3D::GetDevice()->BeginScene()!=D3D_OK)
//			{
//				return;
//			}
//
//			DEBUG_ADD("3D-Before UI.Show()");
//			
//			gC_vs_ui.Show();
//
//			DEBUG_ADD("3D-After UI.Show()");
//			
//			CDirect3D::GetDevice()->EndScene();
//		}
//		else
		{
			DEBUG_ADD("2D-Before UI.Show()");
			gC_vs_ui.Show();
			DEBUG_ADD("2D-After UI.Show()");
			
			POINT point = { 0, 0 };
			RECT rect = { 0, 0, g_GameRect.right, g_GameRect.bottom };

			g_pBack->BltNoColorkey( &point, g_pLast, &rect );
		}

		//CDirect3D::GetDevice()->EndScene();

		CDirectDraw::Flip();
	}
}

//-----------------------------------------------------------------------------
// End Progess
//-----------------------------------------------------------------------------
void		
UI_EndProgress()
{
	DEBUG_ADD("[UI] End Progress");
	
	gC_vs_ui.EndProgress();

	g_bDrawProgress = FALSE;
}

//-----------------------------------------------------------------------------
// Set HP
//-----------------------------------------------------------------------------
void
UI_SetHP(int current, int max)
{
	if (current < 0)
	{
		current = 0;
	}

	if (max <= 0)
	{
		max = 1;
	}

	if (current > max)
	{
		current = max;
	}

	//---------------------------------------------------------------
	// HP 낮은 경우 체크
	//---------------------------------------------------------------
	__BEGIN_HELP_EVENT
		static int lastPercent = 100;
		int percent = ((max==0)?0 : current*100 / max);

		// 방금 전에는 30%이상이었는데.. 지금 이하가 된 경우
		if (percent <= 30 && lastPercent > 30)
		{
			// [도움말] HP 낮은 경우
			ExecuteHelpEvent( HELP_EVENT_USE_POTION );
		}
		lastPercent = percent;

	__END_HELP_EVENT

	DEBUG_ADD_FORMAT("[UI] Set HP  (%d / %d)", current, max);
	
	//---------------------------------------------------------------
	// 물렸을때 HP bar 변경
	//---------------------------------------------------------------
//	if (g_pPlayer->IsSlayer())
//	{
//		if (g_pPlayer->GetConversionDelayTime() > g_CurrentTime)
//		{
//			// 물렸당..
//			gC_vs_ui.SetHP(current, max, TRUE);
//		}
//		else
//		{
//			gC_vs_ui.SetHP(current, max, FALSE);
//		}
//	}
//	else
//	{
//		gC_vs_ui.SetHP(current, max, FALSE, g_pPlayer->GetSilverDamage());
//	}

	/*
	if (g_Mode==MODE_GAME)
	{
		float ratio = (max==0)? 1 : (float)current/max;

		// hp가 30%이하면..
		if (ratio < 0.3f)
		{
			//g_DXMusic.SetCurrentTempo( g_DXMusic.GetOriginalTempo() * (float)(1.3f - ratio) );
			g_DXMusic.SetCurrentTempo( g_DXMusic.GetOriginalTempo() * 1.3f );
		}	
		// 100이 아니면..
		else if (g_DXMusic.GetCurrentTempo()!=g_DXMusic.GetOriginalTempo())	
		{
			g_DXMusic.SetOriginalTempo();
		}
	}
	else
	{
		if (g_DXMusic.GetCurrentTempo()!=g_DXMusic.GetOriginalTempo())	
		{		
			g_DXMusic.SetOriginalTempo();
		}
	}
	*/
}

//-----------------------------------------------------------------------------
// Set MP
//-----------------------------------------------------------------------------
void
UI_SetMP(int current, int max)
{
	if (current < 0)
	{
		current = 0;
	}

	if (max < 0)
	{
		max = 0;
	}

	if (current > max)
	{
		current = max;
	}

	//---------------------------------------------------------------
	// MP 낮은 경우 체크
	//---------------------------------------------------------------
	__BEGIN_HELP_EVENT
		static int lastPercent = 100;
		int percent = ((max==0)?0 : current*100 / max);

		// 방금 전에는 30%이상이었는데.. 지금 이하가 된 경우
		if (percent <= 30 && lastPercent > 30)
		{
			// [도움말] MP 낮은 경우
			ExecuteHelpEvent( HELP_EVENT_USE_POTION );
			ExecuteHelpEvent( HELP_EVENT_ABSORB_SOUL );

			lastPercent = percent;
		}
	__END_HELP_EVENT

	DEBUG_ADD_FORMAT("[UI] Set MP  (%d / %d)", current, max);
	
//	gC_vs_ui.SetMP(current, max);
}

//-----------------------------------------------------------------------------
// Add Chat To History
//-----------------------------------------------------------------------------
void
UI_AddChatToHistory(char* str, char* sz_id, int cond, DWORD color) // const로 하면 안됨! (6/23, KJTINC)
{
	if(g_pUserOption->ChatWhite)
		color = gpC_base->m_chatting_pi.text_color;

	if (str==NULL || cond>=CLD_TOTAL)
	{
		return;
	}

	CHAT_LINE_CONDITION condition = (CHAT_LINE_CONDITION)cond;

	//-----------------------------------------------------------
	// 이름이 있다면.. 출력해도 되는 사람껀지 확인한다.
	//-----------------------------------------------------------
	/*
	if (sz_id!=NULL)
	{
		// 허용이 안되는 ID이면 출력하지 않는다.
		if (!g_pChatManager->IsAcceptID( sz_id ))
		{
			return;
		}
	}
	*/

	DEBUG_ADD_FORMAT("[UI_AddChatToHistory][%d:%s] %s, %x", cond, sz_id, str, color);
/*
#ifdef OUTPUT_DEBUG
const char g_sz_chat_id_divisor[CLD_TOTAL][3] = 
{
	">",
	">",
	"->",
	")",
	"}",
	":",
};

//	extern int CLogFile;
//
//	if(sz_id != NULL)
//	{
//	if(strcmp(sz_id, g_char_slot_ingame.sz_name.c_str()) == 0 || strstr(str, g_char_slot_ingame.sz_name.c_str()) != NULL)
//		_write( CLogFile, "]]]", 3);
//	_write( CLogFile, sz_id, strlen(sz_id) );
//	_write( CLogFile, g_sz_chat_id_divisor[cond], strlen(g_sz_chat_id_divisor[cond]) );
//	_write( CLogFile, str, strlen(str));
//	_write( CLogFile, "\n", 1 );
//	}
#endif
*/	

	gC_vs_ui.AddChatToHistory(str, sz_id, condition, color);
}

//-----------------------------------------------------------------------------
// Drop Item
//-----------------------------------------------------------------------------
void
UI_DropItem()
{
	gC_vs_ui.DropItem();
}

//-----------------------------------------------------------------------------
// Pickup Item
//-----------------------------------------------------------------------------
void
UI_PickUpItem(MItem* pItem)
{
	// 소리 낸다..
	PlaySound( pItem->GetInventorySoundID() );

	gC_vs_ui.PickUpItem( pItem );
}

//-----------------------------------------------------------------------------
// Lock Item
//-----------------------------------------------------------------------------
void
UI_LockItem()
{
	DEBUG_ADD("[UI] Lock Item");
	
	gC_vs_ui.LockItem();
}

//-----------------------------------------------------------------------------
// UnLock Item
//-----------------------------------------------------------------------------
void
UI_UnlockItem()
{
	DEBUG_ADD("[UI] Unlock Item");
	
	gC_vs_ui.UnlockItem();
}

//-----------------------------------------------------------------------------
// Lock Gear
//-----------------------------------------------------------------------------
void
UI_LockGear()
{
	DEBUG_ADD("[UI] Lock Gear");
	
	gC_vs_ui.LockGear();
}

//-----------------------------------------------------------------------------
// Unlock Gear
//-----------------------------------------------------------------------------
void
UI_UnlockGear()
{
	DEBUG_ADD("[UI] Unlock Gear");
	
	gC_vs_ui.UnlockGear();
}

//-----------------------------------------------------------------------------
// UI 메세지 처리
//-----------------------------------------------------------------------------
void UI_ResultReceiver(DWORD message, int left, int right, void *void_ptr)
{
//	DEBUG_ADD("[UI_ResultReceiver] Start");
	if (g_pUIMessageManager==NULL)
	{
		g_pUIMessageManager = new UIMessageManager;
	}
	
	g_pUIMessageManager->Execute(message, left, right, void_ptr);	
//	DEBUG_ADD("[UI_ResultReceiver] End");
}

//-----------------------------------------------------------------------------
// Run Party
//-----------------------------------------------------------------------------
void		
UI_RunParty()
{
	// by csm 12/4
	if (!UI_IsRunningParty())
	{	
		//gC_vs_ui.RunPartyManager();
		gC_vs_ui.HotKey_Party();
	}
}

//-----------------------------------------------------------------------------
// Request Party
//-----------------------------------------------------------------------------
void
UI_RunPartyRequest(TYPE_OBJECTID otherID)
{
	if (g_pZone==NULL)
	{
		return;
	}

	//-------------------------------------------
	// 파티 정보창 띄운다.
	//-------------------------------------------
	MCreature* pCreature = g_pZone->GetCreature( otherID );

	if (pCreature!=NULL)
	{
		gC_vs_ui.RequestParty( pCreature->GetName(), 30000 );

		// 파티 신청중이라는 의미..
		// 검증 없이 처리한다.
		//g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_PARTY );

		//g_pTempInformation->Mode = TempInformation::MODE_PARTY_REQUEST;
		//g_pTempInformation->Value1 = otherID;

		g_pTempInformation->PartyInviter = otherID;

		// [도움말] 교환신청 받을 때
		//__BEGIN_HELP_EVENT
		//	ExecuteHelpEvent( HE_TRADE_REQUESTED );	
		//__END_HELP_EVENT

	}
	else
	{
		DEBUG_ADD_FORMAT( "[Error] There is no such Creature. id=%d", otherID);
	}	
}


//-----------------------------------------------------------------------------
// Run PartyAsk
//-----------------------------------------------------------------------------
void		
UI_RunPartyAsk(TYPE_OBJECTID otherID)
{
	// by csm 12/4
	MCreature* TempCreature = g_pZone->GetCreature( otherID );
	if(TempCreature->CurPernalShop() != 1)
	{
		
		
		if (g_pZone==NULL || !g_pSystemAvailableManager->IsAvailablePartySystem())
		{
			return;
		}
		
		//-------------------------------------------
		// 파티 정보창 띄운다.
		//-------------------------------------------
		MCreature* pCreature = g_pZone->GetCreature( otherID ); 
		
		if (pCreature!=NULL)
		{
			// C_VS_UI_REQUEST_PARTY::REQUEST
			gC_vs_ui.RunPartyAsk( pCreature->GetName(), C_VS_UI_REQUEST_PARTY::INVITE );
			
			// 파티 검증안한다.
			//g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_PARTY );
			
			//g_pTempInformation->Mode = TempInformation::MODE_PARTY_REQUEST;
			g_pTempInformation->PartyInviter = otherID;
			
			// [도움말] 교환신청 받을 때
			//__BEGIN_HELP_EVENT
			//	ExecuteHelpEvent( HE_TRADE_REQUESTED );	
			//__END_HELP_EVENT
			
		}
		else
		{
			DEBUG_ADD_FORMAT( "[Error] There is no such Creature. id=%d", otherID);
		}
	}
}

//-----------------------------------------------------------------------------
// Run PartyCancel
//-----------------------------------------------------------------------------
void		
UI_RunPartyCancel(const char* pName)
{
	gC_vs_ui.RunPartyCancel(pName);

	// [도움말] 교환신청 하고 나서
	//__BEGIN_HELP_EVENT
	//	ExecuteHelpEvent( HE_TRADE_REQUEST );	
	//__END_HELP_EVENT
}

//-----------------------------------------------------------------------------
// Close PartyRequest
//-----------------------------------------------------------------------------
void		
UI_ClosePartyRequest()
{
	gC_vs_ui.FinishRequestParty();
}


//-----------------------------------------------------------------------------
// Close Party
//-----------------------------------------------------------------------------
void		
UI_CloseParty()
{
	gC_vs_ui.ClosePartyManager();
}

//-----------------------------------------------------------------------------
// Close PartyAsk
//-----------------------------------------------------------------------------
void		
UI_ClosePartyAsk()
{
	gC_vs_ui.ClosePartyAsk();

	// 교환끝
	if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_PARTY)
	{
		g_pPlayer->SetWaitVerifyNULL();
	}

	if (g_pTempInformation->GetMode() == TempInformation::MODE_PARTY_REQUEST)
	{
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
	}
}

//-----------------------------------------------------------------------------
// Close PartyCancel
//-----------------------------------------------------------------------------
void		
UI_ClosePartyCancel()
{
	if (g_pPlayer->IsWaitVerify()==MPlayer::WAIT_VERIFY_PARTY)
	{
		g_pPlayer->SetWaitVerifyNULL();		
	}

	if (g_pTempInformation->GetMode() == TempInformation::MODE_PARTY_REQUEST)
	{
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
	}

	gC_vs_ui.ClosePartyCancel();
}

//-----------------------------------------------------------------------------
// IsRunningPartyManager
//-----------------------------------------------------------------------------
bool
UI_IsRunningParty() 
{
	return gC_vs_ui.IsRunningPartyManager();
}

//-----------------------------------------------------------------------------
// IsRunningPartyManager
//-----------------------------------------------------------------------------
bool
UI_SetWaitGuildList() 
{
	return gC_vs_ui.IsRunningPartyManager();
}

//-----------------------------------------------------------------------------
// UI_SetWaitGuildList
//-----------------------------------------------------------------------------
void
UI_ShowWaitGuildList( GCWaitGuildList *pPacket)
{
	// NPC대화하는 dialog를 숨긴다 
	g_pUIDialog->HidePCTalkDlg();

	DEBUG_ADD_FORMAT( "[GCWaitGuildList] Start");
	gC_vs_ui.RunTeamList(true);
	DEBUG_ADD_FORMAT( "[GCWaitGuildList] Run gC_vs_ui.RunTeamList(true)");

	GuildInfo *pInfo = NULL;
	C_VS_UI_TEAM_LIST::READY_TEAM_LIST ready_team_info;

	while(pInfo = pPacket->popFrontGuildInfoList())
	{
		ready_team_info.guild_id = pInfo->getGuildID();
		ready_team_info.TEAM_NAME = pInfo->getGuildName();
		ready_team_info.LEADER_NAME = pInfo->getGuildMaster();
		ready_team_info.EXPIRE_DATE = pInfo->getGuildExpireDate();
		ready_team_info.MEMBERS = pInfo->getGuildMemberCount();
		ready_team_info.MEMBERS_MAX = 5;

		gC_vs_ui.AddReadyTeamInfo(ready_team_info);
	}

	DEBUG_ADD_FORMAT( "[GCWaitGuildList] Finish");

}

void		
UI_ShowActiveGuildList( GCActiveGuildList *pPacket)
{
	// NPC대화하는 dialog를 숨긴다 
	g_pUIDialog->HidePCTalkDlg();

	DEBUG_ADD_FORMAT( "[GCActiveGuildList] Start");
	gC_vs_ui.RunTeamList(false);
	DEBUG_ADD_FORMAT( "[GCActiveGuildList] Run gC_vs_ui.RunTeamList(false)");

	GuildInfo *pInfo = NULL;
	C_VS_UI_TEAM_LIST::REGIST_TEAM_LIST regist_team_info;

	while(pInfo = pPacket->popFrontGuildInfoList())
	{
		regist_team_info.guild_id = pInfo->getGuildID();
		regist_team_info.TEAM_NAME = pInfo->getGuildName();
		regist_team_info.LEADER_NAME = pInfo->getGuildMaster();
		// 랭킹 나중에 넣쟈ㅠ.ㅠ
		regist_team_info.RANKING = 0;
		regist_team_info.MEMBERS = pInfo->getGuildMemberCount();

		gC_vs_ui.AddRegistTeamInfo(regist_team_info);
	}

	DEBUG_ADD_FORMAT( "[GCActiveGuildList] Finish");
	
}

void		
UI_ShowGuildMemberList( GCGuildMemberList *pPacket)
{
	DEBUG_ADD_FORMAT( "[GCShowGuildMemberList] Start");

	if(false == gC_vs_ui.IsRunningTeamMemberList())
		gC_vs_ui.RunTeamMemberList();
	GuildMemberInfo *pInfo = NULL;

	C_VS_UI_TEAM_MEMBER_LIST::TEAM_MEMBER_LIST member_info;
	
	int groupID = g_pServerInformation->GetServerGroupID();
	ServerGroup* pServerGroup = g_pServerInformation->GetData( groupID );

	bool SetServerName;
	BYTE bAvailableRecall = pPacket->getType();
	while(pInfo = pPacket->popFrontGuildMemberInfoList())
	{
		SetServerName = false;
		member_info.MEMBER_NAME = pInfo->getName();
		member_info.member_grade = pInfo->getRank();
		member_info.bLogOn = pInfo->getLogOn();
		// 2004, 10, 8, sobeit add start 
//		member_info.server_type = pInfo->getServerID();  
		if(pServerGroup && member_info.bLogOn)
		{
			ServerGroup::const_iterator iServer = pServerGroup->find(pInfo->getServerID());
			if(iServer != pServerGroup->end())
			{
				SERVER_INFO* pServerInfo = iServer->second;
				if(NULL != pServerInfo)
				{
					member_info.SERVER_NAME = pServerInfo->ServerName.GetString(); 
					SetServerName = true;
				}
			}
		}
		if(false == SetServerName)
		{
			member_info.SERVER_NAME = (*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_LOGINED].GetString();
			member_info.bLogOn = false; // 서버네임 찾을수 없는거는 그냥 비 로그인으로 하자..ㅋㅋ
		}
		// 2004, 10, 8, sobeit add end
		gC_vs_ui.AddTeamMemberInfo(member_info, bAvailableRecall); 
	}

	DEBUG_ADD_FORMAT( "[GCShowGuildMemberList] Finish");	
}


void		
UI_ShowGuildRegist(DWORD reg_fee)
{
	// NPC대화하는 dialog를 숨긴다 
	g_pUIDialog->HidePCTalkDlg();
	
	gC_vs_ui.RunTeamRegist(false, reg_fee);
}

void		
UI_ShowWaitGuildInfo(GCShowWaitGuildInfo *pPacket)
{
	C_VS_UI_TEAM_INFO::READY_TEAM_INFO team_info;
	team_info.TEAM_NAME = pPacket->getGuildName();
	team_info.LEADER_NAME = pPacket->getGuildMaster();
	team_info.INTRODUCTION = pPacket->getGuildIntro();
	team_info.guild_id = pPacket->getGuildID();
	team_info.MEMBERS = pPacket->getMemberNum();
	team_info.MEMBERS_MAX = 5;
	for(int i = 0; i < team_info.MEMBERS; i++)
	{
		team_info.MEMBERS_NAME.push_back(pPacket->popMember());
	}
	team_info.REG_FEE = pPacket->getJoinFee();
	team_info.EXPIRE_DATE = "";
	gC_vs_ui.RunTeamInfo(true, (void *)&team_info);
}

void		
UI_ShowGuildStartJoin(DWORD join_fee)
{
	gC_vs_ui.RunTeamRegist(true, join_fee);
}

void		
UI_ShowGuildJoin(DWORD join_fee, int rank, const char *date, const char *team_name, int guild_id)
{
	gC_vs_ui.RunTeamRegist(true, join_fee, rank, (char *)date, (char *)team_name, guild_id);
}

void		
UI_ShowGuildInfo(GCShowGuildInfo *pPacket)
{
	C_VS_UI_TEAM_INFO::REGIST_TEAM_INFO team_info;
	team_info.TEAM_NAME = pPacket->getGuildName();
	team_info.LEADER_NAME = pPacket->getGuildMaster();
	team_info.INTRODUCTION = pPacket->getGuildIntro();
	team_info.guild_id = pPacket->getGuildID();
	team_info.MEMBERS = pPacket->getGuildMemberCount();
	team_info.REG_FEE = pPacket->getJoinFee();
	team_info.REGISTERED_DATE = "";
	team_info.RANKING = 0;

	// 2004, 10, 19, sobeit add start - 현재 떠 있는 길드 리스트가 연합리스트 인지 그냥 길드 리스트 인지..에휴..
	bool IsUnionTeamInfo = gC_vs_ui.IsRunningTeamList(true); //  
	// 2004, 10, 19, sobeit add end
	gC_vs_ui.RunTeamInfo(false, (void *)&team_info, IsUnionTeamInfo);
}

void		
UI_ShowGuildMemberInfo(GCShowGuildMemberInfo *pPacket)
{
	C_VS_UI_TEAM_MEMBER_INFO::MEMBER_INFO member_info;
	member_info.guild_id = pPacket->getGuildID();
	member_info.NAME = pPacket->getName();
	member_info.GRADE = pPacket->getGuildMemberRank();
	member_info.INTRODUCTION = pPacket->getGuildMemberIntro().c_str();
	gC_vs_ui.RunTeamMemberInfo(&member_info);
}

void		
UI_ShowUnionGuildMemberInfo(GCShowUnionInfo *pPacket)
{
//	C_VS_UI_TEAM_MEMBER_INFO::MEMBER_INFO member_info;
//	member_info.guild_id = pPacket->getGuildID();
//	member_info.NAME = pPacket->getName();
//	member_info.GRADE = pPacket->getGuildMemberRank();
//	member_info.INTRODUCTION = pPacket->getGuildMemberIntro().c_str();
//	gC_vs_ui.RunTeamMemberInfo(&member_info);

	DEBUG_ADD_FORMAT( "[GCShowUnionInfo] Start");
	gC_vs_ui.RunTeamList(false, true);
	DEBUG_ADD_FORMAT( "[GCShowUnionInfo] Run gC_vs_ui.RunTeamList(false)");
	C_VS_UI_TEAM_LIST::REGIST_TEAM_LIST regist_team_info;
	SingleGuildInfo&pmaster = pPacket->getMasterGuildInfo();
	
	regist_team_info.guild_id = pmaster.getGuildID();
	regist_team_info.TEAM_NAME = pmaster.getGuildName();
	regist_team_info.LEADER_NAME = pmaster.getGuildMaster();
	regist_team_info.RANKING = 0;
	regist_team_info.MEMBERS = pmaster.getGuildMemberCount();
	gC_vs_ui.AddRegistTeamInfo(regist_team_info);

	SingleGuildInfo *pInfo = NULL;
	while(pInfo = pPacket->popFrontUnionGuildInfoList())
	{
		regist_team_info.guild_id = pInfo->getGuildID();
		regist_team_info.TEAM_NAME = pInfo->getGuildName();
		regist_team_info.LEADER_NAME = pInfo->getGuildMaster();
		// 랭킹 나중에 넣쟈ㅠ.ㅠ
		regist_team_info.RANKING = 0;
		regist_team_info.MEMBERS = pInfo->getGuildMemberCount();
	
		gC_vs_ui.AddRegistTeamInfo(regist_team_info);
	}
	DEBUG_ADD_FORMAT( "[GCActiveGuildList] Finish");
}

void
UI_ShowGuildQuit()
{
}

void		UI_ResetQuickItemSlot()
{
	if( g_pPlayer->IsSlayer() )
		gC_vs_ui.ResetSlayerQuickItemSize();
	else if( g_pPlayer->IsOusters() )
		gC_vs_ui.ResetOustersQuickItemSize();
}

void		UI_SetWeaponSpeed(int speed, BYTE type)
{
	g_char_slot_ingame.WS = type;
	g_char_slot_ingame.WeaponSpeed = speed;
}

void		UI_CloseEnchant()
{
	gC_vs_ui.CloseEnchant();
}

void	UI_SetGuild(WORD guild_id, BYTE guild_rank, std::string guild_name)
{
	g_char_slot_ingame.GUILD_ID = guild_id;
	g_char_slot_ingame.GUILD_GRADE = guild_rank;
	g_char_slot_ingame.sz_guild_name = guild_name;
	gC_vs_ui.CloseTeamMemberList();
}

void	UI_RunOtherInfo(MCreature *pCreature)
{
	DEBUG_ADD("[UI_RunOtherInfo] START");

	if(pCreature == NULL)
	{
		DEBUG_ADD("[UI_RunOtherInfo] pCreature == NULL");
		return;
	}


	C_VS_UI_OTHER_INFO::PLAYER_INFO info;
	if(pCreature->GetName() != NULL)
		info.PLAYER_NAME = pCreature->GetName();
	else
		DEBUG_ADD("[UI_RunOtherInfo] Name == NULL");

//	GUILD_INFO *guildInfo = g_pGuildInfoMapper->Get(pCreature->GetGuildNumber());

//	if(guildInfo != NULL && guildInfo->GetGuildName() != NULL)
//		info.TEAM_NAME = guildInfo->GetGuildName();

	info.DOMAINLEVEL[SKILLDOMAIN_BLADE] = pCreature->GetBLADE_DOMAIN_LEVEL();
	info.DOMAINLEVEL[SKILLDOMAIN_SWORD] = pCreature->GetSWORD_DOMAIN_LEVEL();
	info.DOMAINLEVEL[SKILLDOMAIN_GUN] = pCreature->GetGUN_DOMAIN_LEVEL();
	info.DOMAINLEVEL[SKILLDOMAIN_HEAL] = pCreature->GetHEAL_DOMAIN_LEVEL();
	info.DOMAINLEVEL[SKILLDOMAIN_ENCHANT] = pCreature->GetENCHANT_DOMAIN_LEVEL();

	info.bl_female = !pCreature->IsMale();

	if(pCreature->IsSlayer())
	{
		MCreatureWear *pCreatureWear = (MCreatureWear *)pCreature;
		const CHAR_MAN uiFaceMale[] =
		{
			M_FACE1,
			M_FACE2,
			M_FACE3
		};
		const CHAR_WOMAN uiFaceFemale[] =
		{
			W_FACE1,
			W_FACE2,
			W_FACE3
		};

		int faceFrameID = pCreatureWear->GetHairFrameID();
		
		if(info.bl_female)
		{
			info.face_type = uiFaceFemale[faceFrameID - ADDONID_HAIR1_FEMALE];
		}
		else
		{
			info.face_type = uiFaceMale[faceFrameID - ADDONID_HAIR1_MALE];
		}
	}
	info.GRADE = pCreature->GetGrade();
	info.LEVEL = pCreature->GetLEVEL();
	info.FAME = pCreature->GetFAME();
	info.ALIGNMENT = pCreature->GetAlignment();
	info.STR_CUR = pCreature->GetSTR();
	info.DEX_CUR = pCreature->GetDEX();
	info.INT_CUR = pCreature->GetINT();
	info.STR_PURE = pCreature->GetBASIC_STR();
	info.DEX_PURE = pCreature->GetBASIC_DEX();
	info.INT_PURE = pCreature->GetBASIC_INT();
	info.guild_id = pCreature->GetGuildNumber();
	DEBUG_ADD_FORMAT("[UI_RunOtherInfo] Name : %s, guild_id : %d, BLADE : %d, SWORD : %d, GUN : %d, HEAL : %d, ENCHANT : %d, GRADE : %d, LEVEL : %d, FAME : %d, ALIGNMENT : %d, STR_PURE : %d, STR_CUR : %d, DEX_PURE : %d, DEX_CUR : %d, INT_PURE : %d, INT_CUR : %d, face_type : %d, bl_female : %d", 
		info.PLAYER_NAME.c_str(), info.guild_id, info.DOMAINLEVEL[0], info.DOMAINLEVEL[1], info.DOMAINLEVEL[2], info.DOMAINLEVEL[3], info.DOMAINLEVEL[4], info.GRADE, info.LEVEL, info.FAME, info.ALIGNMENT, info.STR_PURE, info.STR_CUR, info.DEX_PURE, info.DEX_CUR, info.INT_PURE, info.INT_CUR, info.face_type, info.bl_female);
	
	if(!gC_vs_ui.IsRunningOtherInfo())
		gC_vs_ui.RunOtherInfo();
	gC_vs_ui.SetOtherInfo(info);
	gC_vs_ui.SetOtherPetInfo(NULL);
	if(pCreature->GetPetID() != OBJECTID_NULL)
	{
		MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(pCreature->GetPetID());
		if(pFakeCreature != NULL)
		{
			MPetItem *pPetItem = pFakeCreature->GetPetItem();
			if(pPetItem != NULL)
			{
				PETINFO petInfo;
				petInfo.HP = pPetItem->GetCurrentDurability();
				petInfo.ATTR = pPetItem->GetEnchantLevel();
				petInfo.ATTR_VALUE = pPetItem->GetSilver();
				petInfo.LEVEL = pPetItem->GetNumber();
				petInfo.FOODTYPE = pPetItem->GetPetFoodType();
				petInfo.EXP_REMAIN = pPetItem->GetPetExpRemain();
				petInfo.CUT_HEAD = pPetItem->IsCanCutHead();
				petInfo.GAMBLE = pPetItem->IsCanGamble();
				if(pPetItem->GetItemOptionListCount() == 0)
					petInfo.OPTION = -1;
				else
					petInfo.OPTION = pPetItem->GetItemOptionList().front();
				petInfo.ITEM_TYPE = pPetItem->GetItemType();
				petInfo.NAME = pPetItem->GetPetName();
				petInfo.ITEM_ID = pPetItem->GetID();
				gC_vs_ui.SetOtherPetInfo(&petInfo);
			}
		}
	}

	DEBUG_ADD("[UI_RunOtherInfo] END");	
}

const char *UI_GetOtherInfoName()
{
	if(gC_vs_ui.IsRunningOtherInfo())
		return gC_vs_ui.GetCurrentOtherInfoName();

	return NULL;
}

extern std::string g_CpCookie;

// 넷마블용
void	UI_RunConnect()
{

	if(g_pUserInformation->IsNetmarble)
	{
		static LOGIN	login;
		// 방식 바뀜.   2003.12.12 by sonee
		//login.sz_id = new char [g_pUserInformation->NetmarbleID.GetLength()+1];
		//strcpy(login.sz_id, g_pUserInformation->NetmarbleID.GetString());
		//		login.sz_password = new char [g_pUserInformation->NetmarblePassword.GetLength()+1];
//		strcpy(login.sz_password, g_pUserInformation->NetmarblePassword.GetString());
		
		login.sz_id = new char[ g_CpCookie.length() + 1 ];
		login.sz_password = new char[ 10 ];
		
		strcpy( login.sz_id, g_CpCookie.c_str() );		
		strcpy( login.sz_password, "DarkEden" );
		
		gpC_base->SendMessage(UI_LOGIN, 0, 0, (void *)&login);
	}
	else
	{
		// 2004, 7, 15, sobeit modify start
		if(false == g_pUserInformation->IsAutoLogIn) // 수동 로그인
			gC_vs_ui.RunConnect();
		else // 웹 로그인
		{
			//edit by Coffee 2006.11.7  槨貢젬藤속菱땡되쩍쌈왯
/*
			if(NULL != g_pUserInformation->pLogInClientPlayer)
			{
				const int numGroup = g_pServerInformation->size();
				if(numGroup != 0)
				{
					UI_SetWorldList();
					SetMode( MODE_WAIT_SELECT_WORLD );
				}
				else
				{
					CLGetWorldList clGetWorldList;
					ClientPlayer * TempClient = (ClientPlayer *)(g_pUserInformation->pLogInClientPlayer);
					TempClient->sendPacket( &clGetWorldList );
		
					SetMode( MODE_WAIT_WORLD_LIST );
				}			
			}
*/
			static LOGIN	login;
			login.sz_id = new char[  g_pUserInformation->UserID.GetLength() + 1 ];
			login.sz_password = new char[ 20 ];
			
			strcpy( login.sz_id, g_pUserInformation->UserID.GetString() );		
			strcpy( login.sz_password, g_pUserInformation->AutoLogInKeyValue.GetString() );
			
			gpC_base->SendMessage(UI_LOGIN, 0, 0, (void *)&login);
		}
		// 2004, 7, 15, sobeit modify end
	}
}

void	UI_OpenBringFee(UINT totalfee)
{
	DEBUG_ADD("[UI_OpenBringFee] START");
	g_pUIDialog->HidePCTalkDlg();
	gC_vs_ui.RunBringFeeWindow(totalfee);
	DEBUG_ADD("[UI_OpenBringFee] END");
//	gC_vs_ui.OpenInventoryToSell();	
}

void	UI_SetTotalFee(UINT fee)
{
	// 현재 창에 나와있는 총 금액과 서버로부터 남은 돈이 날라오는걸 비교해서 그 차이만큼 내돈에 더해준다.		
	gC_vs_ui.SetTotalFee(fee);
	gC_vs_ui.SetBringFee(0);
}

void	UI_ClearNPC()
{
	DEBUG_ADD("[UI_CLEARNPC]");
	gC_vs_ui.ClearNPC();
}

void	UI_RunWarList(GCWarScheduleList* pPacket)
{
	DEBUG_ADD("[UI_RunWarList] Start");
	g_pUIDialog->UnSetLockInputPCTalk();
	g_pUIDialog->ClosePCTalkDlg();
	g_pPlayer->SetWaitVerifyNULL();

	DEBUG_ADD("[UI_RunWarList] OK close PCDialog");

	gC_vs_ui.RunWarListWindow();

	DEBUG_ADD("[UI_RunWarList] OK RunWarListWindow");

	while(1)
	{
		WarScheduleInfo *info = pPacket->popWarScheduleInfo();

		if(info == NULL)
			break;


		C_VS_UI_WAR_LIST::WarInfo wi;

		wi.warType	= info->warType;
		wi.year		= info->year;
		wi.month	= info->month;
		wi.day		= info->day;
		wi.hour		= info->hour;
		if(0 == info->warType) // 길드전일때 - 2004,10,2,sobeit modify
		{
			for(int i = 0; i<5; i++)
			{
				wi.challengerGuildID[i] = info->challengerGuildID[i];
				wi.challengerGuildName[i] = info->challengerGuildName[i];
			}
			wi.reinforceGuildID = info->reinforceGuildID;
			wi.reinforceGuildName = info->reinforceGuildName;
		}
		
		
		gC_vs_ui.AddWarList(wi);

		delete info;
	}
	DEBUG_ADD("[UI_RunWarList] END");
}

void	UI_CloseBloodBibleStatus()
{
	if(gC_vs_ui.IsRunningBloodBibleStatus())
	{
		gC_vs_ui.CloseBloodBibleStatus();
	}
}

void	UI_RunBloodBibleStatus()
{
	gC_vs_ui.RunBloodBibleStatus();
	gC_vs_ui.SetBloodBibleStatusTimer(0);
}

void	UI_SetBloodBibleStatus(GCBloodBibleStatus * pPacket)
{
	C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_STATUS status;
	
	status.zone_id = pPacket->getZoneID();
	status.x = pPacket->getX();
	status.y = pPacket->getY();
	status.name = pPacket->getOwnerName();
	status.shrine_race = pPacket->getShrineRace();
	
	switch(pPacket->getStorage())
	{
	case STORAGE_INVENTORY:
		if(pPacket->getRace() == RACE_SLAYER)	// Slayer
			status.status = C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_HAS_SLAYER;
		else if( pPacket->getRace() == RACE_VAMPIRE )
			status.status = C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_HAS_VAMPIRE;
		else if (pPacket->getRace() == RACE_OUSTERS )
			status.status = C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_HAS_OUSTERS;
		break;

	case STORAGE_ZONE:
		status.status = C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_DROP;
		break;

	case STORAGE_CORPSE:
		if(pPacket->getRace() == RACE_SLAYER)	// Slayer
			status.status = C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_SHRINE_SLAYER;
		else if(pPacket->getRace() == RACE_VAMPIRE )
			status.status = C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_SHRINE_VAMPIRE;
		else if (pPacket->getRace() == RACE_OUSTERS )
			status.status = C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_SHRINE_OUSTERS;
		break;
	}

	int itemType = pPacket->getItemType();
	gC_vs_ui.SetBloodBibleStatus(itemType, status);
}

void	UI_SetBloodBibleStatusTimer(DWORD sec)
{
	gC_vs_ui.SetBloodBibleStatusTimer(sec);
}

void	UI_RunInputNameWindow(int ID)
{
	if(gC_vs_ui.IsRunningInputNameWindow())
		gC_vs_ui.CloseInputNameWindow();

	g_pUIDialog->HidePCTalkDlg();
	gC_vs_ui.RunInputNameWindow((C_VS_UI_INPUT_NAME::INPUT_NAME_MODE_LIST) ID );
	gC_vs_ui.SetEnableInputName();
}

void	UI_CloseInputNameWindow()
{	
	gC_vs_ui.CloseInputNameWindow();	
	g_pUIDialog->UnSetLockInputPCTalk();
	g_pUIDialog->ClosePCTalkDlg();
	g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_NULL );	
	g_pTempInformation->SetMode ( TempInformation::MODE_NULL );
}

void		UI_RunAskGoBilingPage(int n)
{
	switch(n)
	{
	case 0 :
		gC_vs_ui.RunCannotPlayMessage();
		break;
	case 1 :
		gC_vs_ui.RunCannotPlayByAttrMessage();
		break;
	}
}

void		UI_RunQuestList(GCSelectQuestID *pPacket)
{		
	if( g_pQuestInfoManager == NULL || pPacket->empty())
		return;

	g_pPlayer->SetWaitVerifyNULL();
	g_pPCTalkBox->Release();	
	g_pPCTalkBox->SetType( PCTalkBox::SELECT_QUEST );

	if( g_pPCTalkBox->GetCreatureType() == 17 || g_pPCTalkBox->GetCreatureType() == 255 || g_pPCTalkBox->GetCreatureType() == 653)			// 카이저면, 레베카면
	{
		char tempstr[128] = {0,};
			
		while( !pPacket->empty() )
		{
			char str[256];
			QuestID_t	ID = pPacket->popQuestID();		
			QUEST_INFO* mkq = g_pQuestInfoManager->GetInfo( ID );		
			if(NULL != mkq )
			{
				DWORD time = mkq->GetTimeLimit();				
				DWORD minute = (time / 60) % 60;
				DWORD hour = (time / 60 / 60);
				bool bContinue = false;
				
				if(hour > 0 || bContinue)
				{
					bContinue = true;
					char temp[100];
					sprintf(temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_HOUR].GetString(), hour );
					strcat(tempstr, temp);
					strcat(tempstr, " ");
				}
				if(minute > 0 || bContinue)
				{
					bContinue = true;
					char temp[100];
					sprintf(temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_MINUTE].GetString(), minute );
					strcat(tempstr, temp);			
				}
				char selectType = 0;
				switch( mkq->GetType() )
				{
				case QUEST_INFO_MONSTER_KILL :
					if( g_pPlayer->IsVampire() )
					{
						wsprintf(str,(*g_pGameStringTable)[STRING_MESSAGE_MONSTER_KILL_QUEST_STRING_SET_VAMPIRE],mkq->GetName(),mkq->GetGoal(),tempstr);
					} else
					{
						wsprintf(str,(*g_pGameStringTable)[STRING_MESSAGE_MONSTER_KILL_QUEST_STRING_SET],mkq->GetName(),mkq->GetGoal(),tempstr);
					}						
					break;
				case QUEST_INFO_MEET_NPC :
					if( g_pPlayer->IsVampire() )
					{
						wsprintf(str, (*g_pGameStringTable)[STRING_MESSAGE_MEET_NPC_VAMPIRE].GetString() );
					} else
					{
						wsprintf(str, (*g_pGameStringTable)[STRING_MESSAGE_MEET_NPC_SLAYER].GetString() );				
					}
					break;
				case QUEST_INFO_GATHER_ITEM :
					break;
				case QUEST_INFO_MINI_GAME :
					wsprintf(str, (*g_pGameStringTable)[STRING_MESSAGE_SELECT_MINI_GAME].GetString() );
					selectType = 1;
					break;
				}		
				g_pPCTalkBox->SetContent( str );

				if( selectType == 0 )
				{
					wsprintf(tempstr,"%4d%s",ID,(*g_pGameStringTable)[STRING_MESSAGE_YES_I_SEE].GetString() );		
					g_pPCTalkBox->AddString( tempstr );				
				} else
				{
					wsprintf(tempstr,"%4d",ID);
					
					switch( mkq->GetGameType() )
					{
					case GAME_MINE :
						strcat(tempstr,(*g_pGameStringTable)[STRING_MESSAGE_SELECT_CRAZY_MINE].GetString() );
						g_pPCTalkBox->AddString( tempstr );
						break;
					case GAME_ARROW :
						strcat(tempstr,(*g_pGameStringTable)[STRING_MESSAGE_SELECT_ARROW_TILES].GetString() );
						g_pPCTalkBox->AddString( tempstr );
						break;
					}
				}
			}
		}
//		wsprintf(tempstr,"9999%s",g_pPlayer->IsSlayer() ? 
//					(*g_pGameStringTable)[STRING_MESSAGE_CANCEL_MONSTER_KILL_QUEST].GetString() :
//					(*g_pGameStringTable)[STRING_MESSAGE_CANCEL_QUEST_VAMPIRE].GetString() );		
//				g_pPCTalkBox->AddString( tempstr );	

		wsprintf(tempstr,"9999%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_CANCEL_SELECT_QUEST].GetString() );
		g_pPCTalkBox->AddString( tempstr );
	} else
	{
		if( g_pPlayer->IsVampire() )
		{
			g_pPCTalkBox->SetContent( (*g_pGameStringTable)[UI_STRING_MESSAGE_SELECT_QUEST_VAMPIRE].GetString() );
		} else if( g_pPlayer->IsSlayer() )
		{
			g_pPCTalkBox->SetContent( (*g_pGameStringTable)[UI_STRING_MESSAGE_SELECT_QUEST_SLAYER].GetString() );
		} else if( g_pPlayer->IsOusters())
		{
			g_pPCTalkBox->SetContent( (*g_pGameStringTable)[UI_STRING_MESSAGE_SELECT_QUEST_OUSTERS].GetString() );
		}
		else
			return;				// 아우스터즈용 ㅋㅋ
		
		char str[256];
		
		while ( ! pPacket->empty() )
		{
			char timestr[128] = {0,};
			QuestID_t	ID = pPacket->popQuestID();
			
			QUEST_INFO* mkq = g_pQuestInfoManager->GetInfo( ID );
			if(NULL != mkq) 
			{
				DWORD time = mkq->GetTimeLimit();				
				DWORD minute = (time / 60) % 60;
				DWORD hour = (time / 60 / 60);
				bool bContinue = false;
				
				if(hour > 0 || bContinue)
				{
					bContinue = true;
					char temp[100];
					sprintf(temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_HOUR].GetString(), hour );
					strcat(timestr, temp);
					strcat(timestr, " ");
				}
				if(minute > 0 || bContinue)
				{
					bContinue = true;
					char temp[100];
					sprintf(temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_MINUTE].GetString(), minute );
					strcat(timestr, temp);			
				}		
				switch(mkq->GetType() )
				{
				case QUEST_INFO_MONSTER_KILL :
					wsprintf(str,"%4d%s %d%s %s%s",ID,mkq->GetName(), mkq->GetGoal(), (*g_pGameStringTable)[UI_STRING_MESSAGE_NUMBER_OF_ANIMALS].GetString(),(*g_pGameStringTable)[UI_STRING_MESSAGE_TIME_LIMIT].GetString(),timestr);			
					break;			
				default :
					break;
				}		
				
				g_pPCTalkBox->AddString( str );
			}
		}
		
		std::string szMsg;
		
		szMsg += "9999";
		szMsg += (*g_pGameStringTable)[UI_STRING_MESSAGE_CANCEL_SELECT_QUEST].GetString();
		
		g_pPCTalkBox->AddString( szMsg.c_str() );
	}
	g_pUIDialog->PopupPCTalkDlg();
}

void		UI_RunWelcomeMessage()
{
	if(g_pPlayer == NULL)
		return;

	if(g_pPlayer->IsSlayer())
	{
		gC_vs_ui.RunPopupMessage(TXT_TUTORIAL_WELCOME_SLAYER, C_VS_UI_POPUP_MESSAGE::POPUP_WELCOME );
	} else
	if(g_pPlayer->IsVampire())
	{
		gC_vs_ui.RunPopupMessage(TXT_TUTORIAL_WELCOME_VAMPIRE, C_VS_UI_POPUP_MESSAGE::POPUP_WELCOME );
	} else
	if(g_pPlayer->IsOusters())
	{
		gC_vs_ui.RunPopupMessage(TXT_TUTORIAL_WELCOME_OUSTERS, C_VS_UI_POPUP_MESSAGE::POPUP_WELCOME );
	}
}

void		UI_RunQuestStatus( GCQuestStatus * pPacket )
{
	if( pPacket == NULL )
		return;
	
	C_VS_UI_QUEST_STATUS::QUEST_STATUS QuestStatus;
	QUEST_INFO* Quest_Info = g_pQuestInfoManager->GetInfo( pPacket->getQuestID() );
	if(NULL != Quest_Info)
	{
		if( Quest_Info->GetEventQuest() )
		{
			QuestStatus.Title = (*g_pGameStringTable)[UI_STRING_MESSAGE_EVENT_QUEST2_1+Quest_Info->GetQuestLevel()].GetString();
		} else
		{
			QuestStatus.Title			= (*g_pGameStringTable)[UI_STRING_MESSAGE_QUEST_MONSTER_KILL].GetString();
		}
		QuestStatus.QuestID			= pPacket->getQuestID();
		QuestStatus.quest_time		= pPacket->getRemainTime() + (timeGetTime() / 1000);
		QuestStatus.current_point	= pPacket->getCurrentNum();	

		gC_vs_ui.RunQuestStatusWindow();
		gC_vs_ui.SetQuestStatus( QuestStatus );
	}
}

void		UI_InitQuestStatus()
{
	gC_vs_ui.SetQuestStatusInit();
}

void		UI_CloseQuestStatus()
{
	if( gC_vs_ui.IsRunningQuestStatusWindow() )
		gC_vs_ui.CloseQuestStatusWindow();
}

void		UI_RunPopupMessage(int type)
{
}

void		UI_LotteryResult(bool bSuccess)
{
	gC_vs_ui.SetResultLotteryCard( bSuccess );	
}

void		UI_RunLottery(int step)
{
	gC_vs_ui.RunLotteryCard( step );
}

void		UI_RunImageNotice(const char* name, DWORD id )
{
	gC_vs_ui.AddNotice( name, id );
}

void		UI_REQUEST_DIE_TIMER_RESET()
{
	gC_vs_ui.RequestDieTimerReset();
}

void		UI_RunItemShop( GCGoodsList *pPacket )
{
	g_pUIDialog->HidePCTalkDlg();
	g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_NULL );	
	g_pTempInformation->SetMode( TempInformation::MODE_NULL );


	gC_vs_ui.RunItemListWindow();
	while( !pPacket->isEmpty() )
	{
		GoodsInfo *pInfo = pPacket->popGoodsInfo();
		
		if(pInfo == NULL)
			return;

		MItem *pItem = MItem::NewItem( (ITEM_CLASS)pInfo->itemClass );
		
		pItem->SetID( pInfo->objectID );
		pItem->SetItemType( pInfo->itemType );
		// 2004, 5, 18 sobeit add start
		pItem->SetGrade( pInfo->grade );
		// 2004, 5, 18 sobeit add end
		pItem->SetItemOptionList( pInfo->optionType );
		if( pItem->IsGunItem() )
		{
			MMagazine* pMagazine = (MMagazine*)MItem::NewItem( (ITEM_CLASS)ITEM_CLASS_MAGAZINE );
			pMagazine->SetID( 0 );
			for (int j=0; j<(*g_pItemTable)[ITEM_CLASS_MAGAZINE].GetSize(); j++)			
			{
				pMagazine->SetItemType(	j );
				
				if (pMagazine->IsInsertToItem( pItem ))
				{
					break;
				}
			}			
			// 의미 없음
			pMagazine->ClearItemOption();			
			// 탄창 개수
			pMagazine->SetNumber( pInfo->num );			
			//------------------------------------
			// 탄창 설정
			//------------------------------------
			MGunItem* pGunItem = (MGunItem*)pItem;
			pGunItem->SetMagazine( pMagazine );
		} else
		{
			pItem->SetNumber( pInfo->num );
		}
		
		if(pInfo->timeLimit > 0)
			pItem->SetQuestFlag();

		C_VS_UI_ITEM_LIST::ItemList Item;
		Item.pItem = pItem;
		Item.TimeLimit = pInfo->timeLimit;		
		gC_vs_ui.AddItemToItemList( Item );
		
		delete pInfo;
		pInfo = NULL;
	}	
}

void		UI_RemoveItemFromItemShop( DWORD id )
{
	if(gC_vs_ui.IsRunningItemListWindow() )
		gC_vs_ui.RemoveItemInItemList( id );
}

void		UI_CloseTransItem()
{
	gC_vs_ui.CloseTransItem();
}

void		UI_OkMixingForge(DWORD parameter, MItem* pItem, MItem* pItem2)
{
	WORD	FirstOption = (parameter & 0xffff0000) >> 16;		// paramter >> 16 하든지-_- ㅋㅋ 
	WORD	SecondOption = parameter & 0xffff;
	
	if( pItem == NULL || pItem2 == NULL || 
		pItem->IsEmptyItemOptionList() || 
		pItem2->IsEmptyItemOptionList()||
		pItem->GetItemOptionListCount() > 1 ||
		pItem2->GetItemOptionListCount() > 1)
	{
		g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_ETC_ERROR].GetString() );
		return;
	}

	const std::list<TYPE_ITEM_OPTION>& OptionList1  = pItem->GetItemOptionList();
	const std::list<TYPE_ITEM_OPTION>& OptionList2  = pItem2->GetItemOptionList();

	std::list<TYPE_ITEM_OPTION>::const_iterator itr1 = OptionList1.begin();
	std::list<TYPE_ITEM_OPTION>::const_iterator itr2 = OptionList2.begin();
	
	if( *itr1 != FirstOption || *itr2 != SecondOption )
	{
		g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_ETC_ERROR].GetString() );
		return;		
	}
	
	// 옵션 검증이 끝나면.
	
	MItem* pModifyItem = g_pInventory->GetItem( pItem->GetID() );
	if(pModifyItem != NULL)
	{
		pModifyItem->AddItemOption( SecondOption );
	}
	else
	{
		g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_ETC_ERROR].GetString() );
		return;
	}
	//2004,04,27 sobeit set grade add start
	int nMaxGrade = max(pItem->GetGrade(), pItem2->GetGrade());
	pModifyItem->SetGrade(nMaxGrade);
	//2004,04,27 sobeit set grade add end

	MItem* pRemoveItem = g_pInventory->RemoveItem( pItem2->GetID() );
	SAFE_DELETE( pRemoveItem );
	//delete pRemoveItem;	
		
	PlaySound(SOUND_EVENT_FANFARE);
	gC_vs_ui.CloseMixingForge();	
	g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[UI_STRING_MESSAGE_COMPLETE_MERGE_ITEM].GetString() );
	g_pInventory->CheckAffectStatus( pModifyItem );
}

void		UI_CloseMixingForge()
{
	gC_vs_ui.CloseMixingForge();
	
	g_pTempInformation->SetMode( TempInformation::MODE_NULL );
}

void		UI_OkRemoveOption( DWORD parameter, MItem *pItem)
{
	if( pItem == NULL )
	{
		g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_ERROR_ETC_ERROR].GetString() );
		return;
	}
	BYTE OptionType = (BYTE)parameter;
	
	pItem->ClearItemOption();	
	pItem->AddItemOption( OptionType );
	PlaySound(SOUND_XMAS_STAR);
	g_pInventory->CheckAffectStatus( pItem );
	
	g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_SUCCESS_REMOVE_OPTION].GetString());
}

void		UI_SetElementalValue(BYTE type, int value)
{
	switch(type)
	{
	case ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE:
		g_char_slot_ingame.ElementalFire = value;
		break;

	case ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER:
		g_char_slot_ingame.ElementalWater = value;
		break;
		
	case ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH:
		g_char_slot_ingame.ElementalEarth = value;
		break;
		
	case ITEMTABLE_INFO::ELEMENTAL_TYPE_WIND:
		g_char_slot_ingame.ElementalWind = value;
		break;		
	}
}

void	UI_RunHorn(int currentZoneID)
{
	if(g_pPlayer->IsOusters())
		gC_vs_ui.RunHorn(currentZoneID);
}

void		UI_MasterLairMessage(BYTE type, short value, TYPE_ZONEID ZoneID)
{
	if( g_pUserOption->DoNotShowLairMsg || g_pSystemMessage == NULL || g_pZoneTable == NULL || g_pGameStringTable == NULL )
		return;	

	MString msg;
	
	switch( type ) 
	{
	case 0 :			// 마스터레어가 열렸습니다.
		msg.Format((*g_pGameStringTable)[STRING_MESSAGE_OPEN_LAIR].GetString(), g_pZoneTable->Get( ZoneID )->Name.GetString() );
		break;
	case 1 :			// 마스터레어가 닫혔습니다.
		msg.Format((*g_pGameStringTable)[STRING_MESSAGE_CLOSED_LAIR].GetString(), g_pZoneTable->Get( ZoneID )->Name.GetString() );
		break;
	case 2 :			// 마스터레어의 출입가능 시간이 %d 분 남았습니다.
		msg.Format((*g_pGameStringTable)[STRING_MESSAGE_LEFT_TIME_LAIR].GetString(), g_pZoneTable->Get( ZoneID )->Name.GetString() , value );
		break;
	default :
		return;
	}
	
	g_pSystemMessage->Add( msg.GetString() );
}

void		UI_PopupMessage(int gamestringID)
{
	if( gamestringID < 0 || gamestringID >= MAX_GAME_STRING || g_pUIDialog == NULL || g_pGameStringTable == NULL)
		return;

	g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[gamestringID].GetString());
}

void		UI_DownSkill()
{
	g_pUIDialog->ClosePCTalkDlg();
	gC_vs_ui.CloseAllDialog();
	gC_vs_ui.HotKey_SkillInfo();
	gC_vs_ui.SetOustersSkillDown();
}

void		UI_RunMinigame(int game)
{

	switch(game)
	{
	case 0:
		gC_vs_ui.RunFindingMine();
		break;

	case 1:
//		gC_vs_ui.NemoNemo();
		break;
	
	case 2:
//		gC_vs_ui.PushPush();
		break;
	}
}

void		UI_MiniGameScores(GCMiniGameScores* pPacket)
{
	BYTE Type = pPacket->getGameType();
	BYTE Level = pPacket->getLevel();	
	int Size = pPacket->getSize();

	// Size 가 0이면 아무도 안한것.
	// 1이면 top score 만 있는것	
	
	switch(Type)
	{
	case GAME_MINE :
		if( Level == 0 )
			gC_vs_ui.RunCrazyMine();
		
		break;
	case GAME_ARROW :
		if( Level == 0 )
			gC_vs_ui.RunArrowTile();
		break;
	}

	std::string name = "";
	WORD score = 0;

	std::pair<std::string,WORD> scores ,score2;

	if( Size > 0 )
		scores = pPacket->popScore();
	switch( Size )
	{
	case 0 :
		gC_vs_ui.SetMiniGameScore( Type, name, score, score);
		break;
	case 1 :
		gC_vs_ui.SetMiniGameScore( Type, scores.first, scores.second, score );
		break;
	case 2 : 
		score2 = pPacket->popScore();
		gC_vs_ui.SetMiniGameScore( Type, scores.first, scores.second, score2.second);
		break;
	}	
}

void		UI_SetCTFStatus( DWORD endTime, int flag_s, int flag_v, int flag_o )
{
	gC_vs_ui.SetCTFStatus( endTime, flag_s, flag_v, flag_o );
}

void		UI_CloseCTFStatusWindow()
{
	gC_vs_ui.CloseCTFStatusWindow();
}

void		UI_SetDelayEternity()
{
	gC_vs_ui.SetDelayRequestResurrect( C_VS_UI_REQUEST_RESURRECT::RESURRECT_MODE_ETERNITY, 2000 );
}

void		UI_CloseRequestResurrectWindow()
{
	gC_vs_ui.FinishRequestResurrect();
}

void		UI_SetGuildName(WORD guildID, std::string& guildName)
{
	DEBUG_ADD_FORMAT("[SetGuildName] %d %s", guildID, guildName.c_str() );

	if( !guildName.empty() )
		g_pGuildInfoMapper->SetGuildName( guildID, guildName );
}

void		UI_RefreshInfoImage()
{
	gC_vs_ui.RefreshInfoImage();
}

int			GetMyLevelWarStair()
{
	int MyStat;
	if( g_pPlayer->IsSlayer() )
		MyStat = g_char_slot_ingame.STR_PURE + g_char_slot_ingame.DEX_PURE + g_char_slot_ingame.INT_PURE;
	else
		MyStat = g_pPlayer->GetLEVEL();

	switch( g_pPlayer->GetRace() )
	{
	case RACE_SLAYER :
		if( MyStat >= 0 && MyStat <= 150 )
			return 1;
		else if (MyStat <= 210 )
			return 2;
		else if (MyStat <= 260 )
			return 3;
		else if (MyStat <= 300 )
			return 4;	
		break;
	
	case RACE_VAMPIRE :
	case RACE_OUSTERS :
		if( MyStat >= 1 && MyStat <= 30 )
			return 1;
		else if ( MyStat <= 50 )
			return 2;
		else if (MyStat <= 70 )
			return 3;
		else if (MyStat <= 90 )
			return 4;
		break;
	}

	return 0;
}

void
UI_RunNotice(DWORD sendID, DWORD parameter)
{
	std::string FileName;

	SIZE windowSize;
	bool bOpen = false;
	char szDate[128];
	switch(sendID)
	{
	// 초보자 메세지
	case 1:
		FileName = "BeginnerZone";
//		bOpen = true;
		break;

	// START CTF
	case 2:
		FileName = "StartCTF";
		bOpen = true;
		break;

	// 레벨 전쟁 예고
	case 3:
		if(parameter/100000000 != GetMyLevelWarStair())
			return;
		parameter %= 100000000;
		FileName = "LevelWar";
		FileName += '0'+GetMyLevelWarStair();
		break;
	
	// 레벨 전쟁 시작
	case 4:
		if(parameter/100000000 != GetMyLevelWarStair())
			return;
		parameter %= 100000000;
		FileName = "StartLevelWar";
		FileName += '0'+GetMyLevelWarStair();
		bOpen = true;
		break;

	// 종족 전쟁 예고
	case 5:
		FileName = "RaceWar";
		break;

	// 종족 전쟁 시작
	case 6:
		FileName = "StartRaceWar";
		bOpen = true;
		break;

	// 2차 펫 퀘스트
	case 7:
		sprintf(szDate, "%02d%02d%02d",	(g_pGameTime->GetYear()+100)%100,
									g_pGameTime->GetMonth(),
									g_pGameTime->GetDay());
		parameter = (atoi(szDate))*100;
		FileName = "2ndPetQuest";
		break;
	case 8: 
		gC_vs_ui.RunSMSMessage();
		return;
	}

	if(sendID != 2)
	{
		switch( g_pPlayer->GetRace() )
		{
		case RACE_SLAYER :	
			FileName += "Slayer.txt" ;	
			break;

		case RACE_VAMPIRE :	
			FileName += "Vampire.txt";	
			break;

		case RACE_OUSTERS :	
			FileName += "Ousters.txt";	
			break;
		}
	}
	else
	{
		FileName += ".txt";
	}

//	style.Type = C_VS_UI_POPUP_MESSAGE::POPUP_NORMAL;	
//	style.SendID = 2;

	CRarFile PackFile;
	char szLine[4096];
		
	PackFile.SetRAR( RPK_TUTORIAL_ETC, RPK_PASSWORD );
	std::string sender, title, contents;
	if( PackFile.IsSet() )
	{
		PackFile.Open( FileName.c_str() );
		ZeroMemory( szLine, 4096 );
		PackFile.GetString(szLine, 4096);
		sscanf(szLine, "%d %d", &windowSize.cx, &windowSize.cy);
		ZeroMemory( szLine, 4096 );
		PackFile.GetString(szLine, 4096);
		sender = szLine;
		ZeroMemory( szLine, 4096 );
		PackFile.GetString(szLine, 4096);
		title = szLine;
		ZeroMemory( szLine, 4096 );
		PackFile.GetString(szLine, 4096);
		contents = szLine;
		
		PackFile.Release();
	}
	else return;
	

	gC_vs_ui.AddMail(0, sendID, windowSize, sender.c_str(), title.c_str(), parameter, bOpen);
	gC_vs_ui.AddMailContents(0, sendID, contents.c_str());
//	gC_vs_ui.AddMail(0, 2, sender, title, 030000, windowSize);
//	gC_vs_ui.AddMailContents(0, 2, contents, true);
//	gC_vs_ui.RunPopupMessage( style );
}

void
UI_DeleteNotice(DWORD sendID)
{
	gC_vs_ui.DeleteMail(0, sendID);
}

void
UI_RunPetInfo(struct PETINFO *pPetInfo)
{
	gC_vs_ui.RunPetInfo(pPetInfo);
}

void 
UI_SetSMSList(void* pVoid)
{
	gC_vs_ui.SetSMSAddressList(pVoid);

}

void 
UI_AddSMSList(DWORD element, char* name, char* id, char* num)
{
	gC_vs_ui.AddSMSAddressList(element,name, id, num);
}

void 
UI_DeleteSMSList(DWORD id)
{
	gC_vs_ui.DeleteSMSAddressList(id);

}
void 
UI_SMS_Set_Charge(int i)
{
	gC_vs_ui.SetSMSCharage(i);
}
void 
UI_SMS_ERROR(DWORD param)
{
	switch(param)
	{
	case 1:
		UI_PopupMessage(UI_STRING_MESSAGE_SMS_FAIL_MAX_NUM_EXCEEDED);
		break;
	case 2:
		UI_PopupMessage(UI_STRING_MESSAGE_SMS_FAIL_INVALID_DATA);
		break;
	case 3:
		UI_PopupMessage(UI_STRING_MESSAGE_SMS_FAIL_NO_SUCH_EID);
		break;
	case 4:
		UI_PopupMessage(UI_STRING_MESSAGE_SMS_FAIL_NOT_ENOUGH_CHARGE);
		break;

	default:
		UI_PopupMessage(STRING_ERROR_ETC_ERROR);
		break;
	}
}
void 
UI_RunModifyTax()
{
	// NPC대화하는 dialog를 숨긴다
	g_pUIDialog->HidePCTalkDlg();

	gC_vs_ui.RunModifyTax();
}


void 
UI_RunRangerChat()
{
	gC_vs_ui.RunRangerChat();
}
void 
UI_CloseRangerChat()
{
	gC_vs_ui.CloseRangerChat();
}

void		
UI_SetRangerChatString(char* str)
{
	gC_vs_ui.SetRangerChatString(str);
}

bool UI_IsMonsterKillQuest_Monster(DWORD nID)
{
	QUEST_INFO* Quest_Info = g_pQuestInfoManager->GetInfo( gC_vs_ui.GetQuestStatusID() );
	
	if(NULL != Quest_Info)
	{
		if( Quest_Info->GetType() == QUEST_INFO_MONSTER_KILL && Quest_Info->GetSpriteType() == nID)
		{
			return true;
		}
	}
	return false;
}

void UI_ShowTargetArrow(int x, int y)
{
	gC_vs_ui.DrawTargetArrow(x,y);
}

// 2005, 1, 3, sobeit add start - 승직 아이템 교환
void
UI_Show_Swap_Advancement_Item()
{
	// NPC대화하는 dialog를 숨긴다 
	g_pUIDialog->HidePCTalkDlg();

	gC_vs_ui.OpenInventoryToSwapAdvanceItem();
}
bool		UI_IsRunningSwapAdvancementItem()
{
	return gC_vs_ui.IsRunningSwapAdvancementItem();
}
// 2005, 1, 3, sobeit add end

// 2005, 1, 11, sobeit add start - 불우 이웃돕기 성금 창
void 
UI_Run_Campaign_Help_Unfortunate_Neighbors(int value)
{
	// NPC대화하는 dialog를 숨긴다
//	g_pUIDialog->HidePCTalkDlg();

	gC_vs_ui.Run_Campaign_Help_Unfortunate_Neighbors(value);
}
// 2005, 1, 11, sobeit add end

// 2005, 1, 24, sobeit add start - 이벤트 아이템 받기
void 
UI_Run_Confirm_GetEventItem(int value)
{
	// NPC대화하는 dialog를 숨긴다
//	g_pUIDialog->HidePCTalkDlg();

	gC_vs_ui.Run_Confirm_GetItemEvent(value);
}
// 2005, 1, 11, sobeit add end

void 
UI_Run_WebBrowser(char* szURL)
{
	if(UI_IsRunning_WebBrowser())
	{
		UI_Close_WebBrowser();
	
	}

	gC_vs_ui.RunWebBrowser(g_hWnd, szURL, (void*)g_pWebBrowser);
	//gC_vs_ui.RunWebBrowser(g_hWnd, szURL, (void*)g_x);
	if(gC_vs_ui.IsRunningWebBrowser())
	{
//		CDirectDraw::CreateClipper();
	//	SetCursor(LoadCursor(NULL, IDC_ARROW));

//	
//		CURSORINFO _CursorInfo;
//		GetCursorInfo(&_CursorInfo);
//		if(_CursorInfo.flags != CURSOR_SHOWING)
//		{
//		}
		if(gC_vs_ui.IsInRectPointWebBrowser(g_x, g_y))
		{
			UI_ShowWindowCursor();
		}
	}
}
bool
UI_IsRunning_WebBrowser()
{
	return gC_vs_ui.IsRunningWebBrowser();
}
int
UI_GetMouseCursorInfo_WebBrowser()
{
	return gC_vs_ui.GetMouseCursorInfo();
}
void 
UI_Close_WebBrowser()
{
	if(gC_vs_ui.IsRunningWebBrowser())
	{
		if(gC_vs_ui.IsInRectPointWebBrowser(g_x, g_y))
		{
			UI_HiddenWindowCursor();
		}

		gC_vs_ui.CloseWebBrowser();

		SetFocus(g_hWnd);
	}
}

void 
UI_ShowWindowCursor()
{
	// 정상적으로 처리 됐을 경우 TempCount 값이 0이 들어온다.
	int TempCount = ShowCursor( TRUE );
		
	// 혹시라도 있을지 모를 Cursor 관련 에러 방지. ShowCursor가 윈도 내부적으로 중복되기 땜시..
	if(TempCount != 0)
	{
		int MaxChange = 10;
		if(TempCount > 0)
		{
			while(TempCount != 0 && MaxChange-- > 0)
			{
				TempCount = ShowCursor( FALSE );
			}
		}
		else if(TempCount < 0)
		{
			while(TempCount != 0 && MaxChange-- > 0)
			{
				TempCount = ShowCursor( TRUE );
			}
		}
	}
}

void 
UI_HiddenWindowCursor()
{
	// 정상적으로 처리 됐을 경우 TempCount 값이 -1이 들어온다.
	int TempCount = ShowCursor( FALSE );

	// 혹시라도 있을지 모를 Cursor 관련 에러 방지. ShowCursor가 윈도 내부적으로 중복되기 땜시..
	if(TempCount != -1)
	{
		int MaxChange = 10;
		if(TempCount > -1)
		{
			while(TempCount != -1 && MaxChange-- > 0)
			{
				TempCount = ShowCursor( FALSE );
			}
		}
		else if(TempCount < -1)
		{
			while(TempCount != -1 && MaxChange-- > 0)
			{
				TempCount = ShowCursor( TRUE );
			}
		}
	}
}


#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관

	void UI_RunSubInventory(MItem* pItem)
	{
		if(gC_vs_ui.IsRunningSubInventory())
			gC_vs_ui.CloseSubInventory();
		gC_vs_ui.RunSubInventory(pItem);
	}

	void UI_CloseSubInventory()
	{
		gC_vs_ui.CloseSubInventory();
	}
	bool UI_AddItemToSubInventory(MItem *pItem, int X, int Y)
	{
		return gC_vs_ui.AddItemToSubInventory(pItem, X, Y);
	}

#endif
