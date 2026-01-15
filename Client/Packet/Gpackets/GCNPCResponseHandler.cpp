//////////////////////////////////////////////////////////////////////
//
// Filename    : GCNPCResponseHandler.cpp
// Written By  : 김성민
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files

#include "Client_PCH.h"
#include "GCNPCResponse.h"
#include "ClientDef.h"
#include "MStorage.h"
#include "MInventory.h"
#include "MSlayerGear.h"
#include "MVampireGear.h"
#include "MOustersGear.h"
#include "MMoneyManager.h"
#include "MGameStringTable.h"
#include "TempInformation.h"
#include "MSkillManager.h"
#include "UIDialog.h"
#include "SoundDef.h"
#include "MItemFinder.h"
#include "UserInformation.h"
#include "MSkillManager.h"
#include "MEventManager.h"
#include "UIFunction.h"

#ifdef __GAME_CLIENT__
	#include "ClientPlayer.h"
	#include "Cpackets/CGConnect.h"
	#include "Cpackets/CGStashList.h"
	#include "VS_UI.h"
	#include "VS_UI_filepath.h"
#endif


void GCNPCResponseHandler::execute ( GCNPCResponse * pPacket , Player * pPlayer )
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

#ifdef __GAME_CLIENT__

	char sz_temp[512];
	//------------------------------------------------------
	// 검증
	//------------------------------------------------------
	if ( g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_NPC_ASK )
	{
		g_pPlayer->SetWaitVerifyNULL();

		DEBUG_ADD("[Verified] NPC Ask Answer OK");
	}

	int value = pPacket->getParameter();

	switch (pPacket->getCode())
	{		
		//------------------------------------------------------
		//
		//				Quit Dialog
		//
		//------------------------------------------------------
		case NPC_RESPONSE_QUIT_DIALOGUE :
			//if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_NPC_ASK)
			{
				g_pUIDialog->UnSetLockInputPCTalk();

				g_pPlayer->SetWaitVerifyNULL();

				// 대화중이 아니라는 의미이다.
				g_pUIDialog->ClosePCTalkDlg();
			}
		break;

		//------------------------------------------------------
		//
		//					수리 띄우기
		//
		//------------------------------------------------------			
		case NPC_RESPONSE_INTERFACE_REPAIR :
			UI_OpenInventoryToRepair();
		break;


		//------------------------------------------------------
		//
		//					수리 띄우기
		//
		//------------------------------------------------------			
		case NPC_RESPONSE_INTERFACE_HELICOPTER :
			UI_RunSelectWayPoint();
		break;		

		//------------------------------------------------------			
		//
		//					보관함 띄우기
		//
		//------------------------------------------------------			
		case NPC_RESPONSE_INTERFACE_STASHOPEN :
			//----------------------------------------------------------------------
			// 보관함 정보가 없는 경우엔 std::list를 요청한다.
			//----------------------------------------------------------------------
			if (g_pStorage==NULL)
			{	
				ClientPlayer * pClientPlayer = dynamic_cast<ClientPlayer*>(pPlayer);

				CGStashList	_CGStashList;

				_CGStashList.setObjectID( g_pPlayer->GetID() );
				
				pClientPlayer->sendPacket( &_CGStashList );
			}
			//----------------------------------------------------------------------
			// 이미 정보가 있으면 바로 띄운다.
			//----------------------------------------------------------------------
			else
			{
				UI_RunStorage();
				UI_SetStorage(g_pStorage);
			}
		break;
	
		
		//--------------------------------------------------------------
		//
		//  Repair OK
		//
		//--------------------------------------------------------------
		case NPC_RESPONSE_REPAIR_OK :
			//--------------------------------------------------------------
			// Item 파는 packet을 수리하는게 맞나?
			//--------------------------------------------------------------
			if (g_pTempInformation->GetMode() == TempInformation::MODE_SHOP_REPAIR)
			{
				//MItem* pCheckItem = (MItem*)g_pTempInformation->pValue;		
				
				MItem* pModifyItem = (MItem*)g_pTempInformation->pValue;		//NULL;

				BOOL	bInInventory	= g_pTempInformation->Value1;
				BOOL	bInGear			= g_pTempInformation->Value2;
				
				//--------------------------------------------------------------
				// inventory의 item수리
				//--------------------------------------------------------------		
				/*
				if (bInInventory)
				{
					pModifyItem = g_pInventory->GetItemToModify( itemID );
				}
				//--------------------------------------------------------------
				// gear의 item수리
				//--------------------------------------------------------------		
				else if (bInGear)
				{
					//--------------------------------------------------------------
					// gear의 item 수리
					//--------------------------------------------------------------
					if (pModifyItem==NULL)
					{
						if (g_pPlayer->IsSlayer())
						{
							pModifyItem = g_pSlayerGear->GetItemToModify( itemID );
						}
						else
						{
							pModifyItem = g_pVampireGear->GetItemToModify( itemID );
						}
					}
				}
				*/

				//--------------------------------------------------------------
				// 다~ 수리하는 경우
				//--------------------------------------------------------------
				if (pModifyItem==NULL && !bInInventory && bInGear)
				{
					MPlayerGear* pGear = g_pPlayer->GetGear();

					// 다 수리하기
					pGear->SetBegin();

					MItem* pLastItem = NULL;

					while (pGear->IsNotEnd())
					{
						MItem* pItem = pGear->Get();

						if (pItem!=NULL)
						{
							pItem->SetCurrentDurability( pItem->GetMaxDurability() );

							pLastItem = pItem;
						}

						pGear->Next();
					}

					// 상태 체크
					pGear->CheckItemStatusAll();

					//--------------------------------------------------------------
					// 수리했다고 소리 출력
					//--------------------------------------------------------------
					if (pLastItem!=NULL)
					{	
						PlaySound( pLastItem->GetInventorySoundID() );				
					}


					//--------------------------------------------------------------
					// 돈을 바꿔준다.
					//--------------------------------------------------------------
					if (!g_pMoneyManager->SetMoney( value ))
					{
						DEBUG_ADD_FORMAT("[Error] Can't Set Money=%d, Price=%d", g_pMoneyManager->GetMoney(), value);
					}

					DEBUG_ADD("RepairAll OK");
				}
				//--------------------------------------------------------------
				// 하나만 수리하는 경우
				//--------------------------------------------------------------
				else if (//pCheckItem!=NULL && 
						pModifyItem!=NULL)
				{
					// ID 검증을 한다.
					if (1)//pCheckItem->GetID()==itemID && pModifyItem->GetID()==itemID)
					{
						//--------------------------------------------------------------
						// durability를 max로..
						//--------------------------------------------------------------
						if (pModifyItem->IsChargeItem())
						{
							pModifyItem->SetNumber( pModifyItem->GetMaxNumber() );

							if (g_pSkillAvailable!=NULL)
							{
								g_pSkillAvailable->SetAvailableSkills();		
							}	
						}
						else
						{
							pModifyItem->SetCurrentDurability( pModifyItem->GetMaxDurability() );
						}

						//--------------------------------------------------------------
						// Gear에 있는 경우는 
						//--------------------------------------------------------------
						if (bInGear)
						{
							MPlayerGear *pGear = g_pPlayer->GetGear();
							
							pGear->CheckItemStatusAll();				
						}

						//--------------------------------------------------------------
						// 수리했다고 소리 출력
						//--------------------------------------------------------------
						PlaySound( pModifyItem->GetInventorySoundID() );				

						//--------------------------------------------------------------
						// 돈을 바꿔준다.
						//--------------------------------------------------------------
						if (!g_pMoneyManager->SetMoney( value ))
						{
							DEBUG_ADD_FORMAT("[Error] Can't Set Money=%d, Price=%d", g_pMoneyManager->GetMoney(), value);
						}
					}
					//else
					//{
					//	DEBUG_ADD_FORMAT("[Error] Different ID. Packet(%d)!=ClientTemp(%d)", itemID, pModifyItem->GetID());
					//}
				}
				else
				{
					DEBUG_ADD_FORMAT("[Error] Item is NULL");
				}

				// mode를 없앤다.
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				// 거래를 다시 활성화한다.
				UI_UnlockItemTrade();
			}
			else
			{
				DEBUG_ADD("[Error] Repair? -_-; No Temp Information!");
			}
		break;

		//------------------------------------------------------			
		//
		//					수리 불가
		//
		//------------------------------------------------------			
		case NPC_RESPONSE_REPAIR_FAIL_ITEM_NOT_EXIST :
		case NPC_RESPONSE_REPAIR_FAIL_ITEM_TYPE :
		case NPC_RESPONSE_REPAIR_FAIL_MONEY :
			// mode를 없앤다.
			g_pTempInformation->SetMode(TempInformation::MODE_NULL);

			// 거래를 다시 활성화한다.
			UI_UnlockItemTrade();

			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_REPAIR].GetString() );

		break;

		//------------------------------------------------------			
		//
		//					보관함 사기
		//
		//------------------------------------------------------			
		case NPC_RESPONSE_STASH_SELL_OK :
			if (g_pTempInformation->GetMode() == TempInformation::MODE_STORAGE_BUY_WAIT)
			{
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				// 다시 뭔가를?선택할 수 있게 한다.
				g_pUIDialog->ShowPCTalkDlg();

				// 이미 보관함이 있다면 제거시킨다.
				// 그러면.. 다음에 보관함 선택할때.. 하나 추가돼서 생성된다.
				if (g_pStorage!=NULL)
				{
					delete g_pStorage;
					g_pStorage = NULL;
				}

				// 보관함 구입 비용을 빼준다.
				g_pMoneyManager->UseMoney( g_pTempInformation->Value1 );

				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_STORAGE_BUY].GetString());
			}
		break;

		//------------------------------------------------------			
		//
		//					보관함 사기 실패
		//
		//------------------------------------------------------			
		case NPC_RESPONSE_STASH_SELL_FAIL_MAX :
		case NPC_RESPONSE_STASH_SELL_FAIL_MONEY :
			if (g_pTempInformation->GetMode() == TempInformation::MODE_STORAGE_BUY_WAIT)
			{
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				// 다시 뭔가를?선택할 수 있게 한다.
				g_pUIDialog->ShowPCTalkDlg();

				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_CANNOT_BUY_MORE].GetString());
			}
		break;

		//------------------------------------------------------
		//
		//					Silver 코팅 성공
		//
		//------------------------------------------------------
		case NPC_RESPONSE_SILVER_COATING_OK :
			//--------------------------------------------------------------
			// silvering이 맞나?
			//--------------------------------------------------------------
			if (g_pTempInformation->GetMode() == TempInformation::MODE_SHOP_SILVERING)
			{
				//MItem* pCheckItem = (MItem*)g_pTempInformation->pValue;		
				
				MItem* pModifyItem = (MItem*)g_pTempInformation->pValue;		//NULL;

				BOOL	bInInventory	= g_pTempInformation->Value1;
				BOOL	bInGear			= g_pTempInformation->Value2;	

				if (//pCheckItem!=NULL && 
					pModifyItem!=NULL)
				{
					// ID 검증을 한다.
					if (1)//pCheckItem->GetID()==itemID && pModifyItem->GetID()==itemID)
					{
						//--------------------------------------------------------------
						// durability를 max로..
						//--------------------------------------------------------------
						pModifyItem->SetSilver( pModifyItem->GetSilverMax() );
						const MItem* pWeapon = g_pSlayerGear->GetItem( MSlayerGear::GEAR_SLAYER_RIGHTHAND );
						if(pModifyItem->IsAffectStatus() && pModifyItem == pWeapon)
						{
							UI_SetCharInfoSilverDamage(pWeapon->GetMinDamage() / 10, pWeapon->GetMaxDamage() / 10);
						}

						//--------------------------------------------------------------
						// 수리했다고 소리 출력
						//--------------------------------------------------------------
						PlaySound( pModifyItem->GetInventorySoundID() );				

						//--------------------------------------------------------------
						// 돈을 바꿔준다.
						//--------------------------------------------------------------
						if (!g_pMoneyManager->SetMoney( value ))
						{
							DEBUG_ADD_FORMAT("[Error] Can't Set Money=%d, Price=%d", g_pMoneyManager->GetMoney(), value);
						}
					}
					//else
					//{
					//	DEBUG_ADD_FORMAT("[Error] Different ID. Packet(%d)!=ClientTemp(%d)", itemID, pModifyItem->GetID());
					//}
				}
				else
				{
					DEBUG_ADD_FORMAT("[Error] Item is NULL");
				}

				// mode를 없앤다.
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

				// 거래를 다시 활성화한다.
				UI_UnlockItemTrade();
			}
			else
			{
				DEBUG_ADD("[Error] Silvering? -_-; No Temp Information!");
			}
		break;

		//------------------------------------------------------
		//
		//					Silver 코팅 실패
		//
		//------------------------------------------------------
		case NPC_RESPONSE_SILVER_COATING_FAIL_ITEM_NOT_EXIST :
		case NPC_RESPONSE_SILVER_COATING_FAIL_ITEM_TYPE :
		case NPC_RESPONSE_SILVER_COATING_FAIL_MONEY :
			// mode를 없앤다.
			g_pTempInformation->SetMode(TempInformation::MODE_NULL);

			// 거래를 다시 활성화한다.
			UI_UnlockItemTrade();

			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_SILVERING].GetString() );
		break;

		
		//------------------------------------------------------
		//
		//				Silver 코팅 UI 띄우기
		//
		//------------------------------------------------------
		case NPC_RESPONSE_INTERFACE_SILVER_COATING :
			UI_OpenInventoryToSilvering();
		break;

		//------------------------------------------------------
		//
		//				길드 생성
		//
		//------------------------------------------------------
		case NPC_RESPONSE_INTERFACE_CREATE_GUILD :
		break;

		//------------------------------------------------------
		//
		//				길드 삭제
		//
		//------------------------------------------------------
		case NPC_RESPONSE_INTERFACE_DESTROY_GUILD :
		break;
	
		//------------------------------------------------------
		//
		//				Newbie Item
		//
		//-----------------------------------------------------
		case NPC_RESPONSE_INTERFACE_NEWBIE_ITEM :
			UI_RunTutorialExit();
		break;

		//------------------------------------------------------
		//
		//				Tutorial Computer
		//
		//------------------------------------------------------
		case NPC_RESPONSE_INTERFACE_TUTORIAL_COMPUTER :
			UI_RunComputer();
		break;

		//------------------------------------------------------
		//
		//				Tutorial Briefing
		//
		//------------------------------------------------------
		case NPC_RESPONSE_INTERFACE_TUTORIAL_BRIEFING :
			UI_RunBriefing();
		break;

		//------------------------------------------------------
		//
		//				Tutorial Bookcase
		//
		//------------------------------------------------------
		case NPC_RESPONSE_INTERFACE_TUTORIAL_BOOKCASE :
			UI_RunBookcase();
		break;

		//------------------------------------------------------
		//
		//				치료 받을 때
		//
		//------------------------------------------------------
		case NPC_RESPONSE_HEAL :
			if( g_pPlayer->IsSlayer() )
				PlaySound( SOUND_ITEM_USE_HPOTION );
			else if (g_pPlayer->IsOusters() )
				PlaySound( SOUND_OUSTERS_PUPA );
		break;

		//------------------------------------------------------
		//
		//				기부 했을 때
		//
		//------------------------------------------------------
		case NPC_RESPONSE_DONATION_OK :
			//--------------------------------------------------------------
			// 돈을 바꿔준다.
			//--------------------------------------------------------------
			if (!g_pMoneyManager->SetMoney( value ))
			{
				DEBUG_ADD_FORMAT("[Error] Can't Set Money=%d, Price=%d", g_pMoneyManager->GetMoney(), value);
			}

			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_DONATION_OK].GetString() );
		break;

		//------------------------------------------------------
		//
		//				기부 실패
		//
		//------------------------------------------------------
		case NPC_RESPONSE_DONATION_FAIL_MONEY :
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_DONATION_FAIL].GetString() );
		break;

		/////////////////////////////////////////////////////////
		// 축구공으로 물건사기
		/////////////////////////////////////////////////////////
		case NPC_RESPONSE_DECREASE_BALL:
		// 적절한 개수만큼을 inventory에서 지워준다.
		{
			int remainNum = pPacket->getParameter();
				
			// 몇개나 있는지 찾아본다.
			MItemClassTypeFinder ballFinder(ITEM_CLASS_EVENT_STAR, 6);	// ball은 6번 :)
				
			// 개수만큼 inventory에서 제거한다.
			while (remainNum > 0)
			{
				MItem* pBallItem = g_pInventory->FindItemGridOrder( ballFinder );
				
				// 별이 없는 경우 - -;
				if (pBallItem==NULL)
				{
					DEBUG_ADD("[Error] Not Enough Ball -_-");
					break;
				}
				
				int itemNum = pBallItem->GetNumber();
				
				if (itemNum > remainNum)
				{
					pBallItem->SetNumber( itemNum - remainNum );
					remainNum = 0;
				}
				else
				{
					// 같거나 적은 경우
					remainNum -= itemNum;
					
					MItem* pRemovedItem = g_pInventory->RemoveItem( 
						pBallItem->GetGridX(), 
						pBallItem->GetGridY() );
					
					if (pRemovedItem!=NULL)
					{
						delete pRemovedItem;
					}
					else
					{
						DEBUG_ADD_FORMAT("[Error] Can't remove ball(%d, %d)", pBallItem->GetGridX(), pBallItem->GetGridY());
					}
				}
			}
		}
		break;

		case NPC_RESPONSE_TEAM_REGIST_FAIL_ALREADY_JOIN:			// 어디 한번 볼까? 자네는 이미 <team_name> 팀 소속이라고 나와 있군
			sprintf(sz_temp, (*g_pGameStringTable)[STRING_MESSAGE_TEAM_REGIST_FAIL_ALREADY_JOIN].GetString(), g_pUserInformation->GuildName.GetString());
			g_pUIDialog->PopupFreeMessageDlg( sz_temp );
		break;

		case NPC_RESPONSE_TEAM_REGIST_FAIL_QUIT_TIMEOUT:			// 자네는 다른 팀을 탈퇴한지 얼마 되지 않았군. 조금 더 신중하게 생각하고 행동하게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TEAM_REGIST_FAIL_QUIT_TIMEOUT].GetString() );
		break;

		case NPC_RESPONSE_TEAM_REGIST_FAIL_CANCEL_TIMEOUT:		// 자네는 팀을 취소 당한 지 얼마 되지 않았군. 등록 조건에 부합하도록 자신을 좀 더 기르고 오게. 좀 더 신중하게 기회를 엿보도록 하게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TEAM_REGIST_FAIL_CANCEL_TIMEOUT].GetString() );
		break;

		case NPC_RESPONSE_TEAM_REGIST_FAIL_LEVEL:				// 훌륭하지만 리더로써의 자질은 좀 부족한 것 같군. 실력을 닦고 오게나
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TEAM_REGIST_FAIL_LEVEL].GetString() );
		break;

		case NPC_RESPONSE_TEAM_REGIST_FAIL_MONEY:				// 팀을 만들려면 많은 돈이 필요하다네. 자네는 돈이 없어 보이는군...
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TEAM_REGIST_FAIL_MONEY].GetString() );
		break;

		case NPC_RESPONSE_TEAM_REGIST_FAIL_FAME:					// <player_name>이라.. 난 아직 그런 이름은 들어보지도 못했다네. 그건 자네가 풋내기라는 것을 의미하지. 실력을 닦고 다시 찾아오게나
			sprintf(sz_temp, (*g_pGameStringTable)[STRING_MESSAGE_TEAM_REGIST_FAIL_FAME].GetString(), g_pUserInformation->CharacterID.GetString());
			g_pUIDialog->PopupFreeMessageDlg( sz_temp );
		break;

		case NPC_RESPONSE_TEAM_REGIST_FAIL_NAME:					// 팀 이름이 이미 쓰이고 있군, 다른 이름을 생각해 보게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TEAM_REGIST_FAIL_NAME].GetString() );
		break;

		case NPC_RESPONSE_TEAM_REGIST_FAIL_DENY:				// <team name>팀에서 자네를 거부하지 않았는가. 다른 팀을 찾아보게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TEAM_REGIST_FAIL_DENY].GetString() );
		break;


		case NPC_RESPONSE_TEAM_STARTING_FAIL_ALREADY_JOIN:		// 당신은 이미 다른 팀에 가입되어 있습니다.
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TEAM_STARTING_FAIL_ALREADY_JOIN].GetString() );
		break;

		case NPC_RESPONSE_TEAM_STARTING_FAIL_QUIT_TIMEOUT:		// 자네는 다른 팀을 탈퇴한지 얼마 되지 않았군. 조금 더 신중하게 생각하고 행동하게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TEAM_STARTING_FAIL_QUIT_TIMEOUT].GetString() );
		break;

		case NPC_RESPONSE_TEAM_STARTING_FAIL_CANCEL_TIMEOUT:		// 자네는 팀을 취소 당한 지 얼마 되지 않았군. 조건에 부합하도록 자신을 좀 더 기르고 오게. 좀 더 신중하게 기회를 엿보도록 하게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TEAM_STARTING_FAIL_CANCEL_TIMEOUT].GetString() );
		break;

		case NPC_RESPONSE_TEAM_STARTING_FAIL_LEVEL:				// 자네는 아직 부족한 점이 많아 보이는구만. 조금 더 실력을 닦고 찾게나.
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TEAM_STARTING_FAIL_LEVEL].GetString() );
		break;

		case NPC_RESPONSE_TEAM_STARTING_FAIL_MONEY:				// <player_name>, 팀 등록을 위해서는 돈이 더 필요하다네
			sprintf(sz_temp, (*g_pGameStringTable)[STRING_MESSAGE_TEAM_STARTING_FAIL_MONEY].GetString(), g_pUserInformation->CharacterID.GetString());
			g_pUIDialog->PopupFreeMessageDlg( sz_temp );
		break;

		case NPC_RESPONSE_TEAM_STARTING_FAIL_FAME:				// <player_name>이라.. 난 아직 그런 이름은 들어보지도 못했다네. 그건 자네가 풋내기라는 것을 의미하지. 실력을 닦고 다시 찾아오게나
			sprintf(sz_temp, (*g_pGameStringTable)[STRING_MESSAGE_TEAM_STARTING_FAIL_FAME].GetString(), g_pUserInformation->CharacterID.GetString());
			g_pUIDialog->PopupFreeMessageDlg( sz_temp );
		break;

		case NPC_RESPONSE_TEAM_STARTING_FAIL_DENY:				// <team name>팀에서 자네를 거부하지 않았는가. 다른 팀을 찾아보게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TEAM_STARTING_FAIL_DENY].GetString() );
		break;

		case NPC_RESPONSE_CLAN_REGIST_FAIL_ALREADY_JOIN:			// 어디 한번 볼까? 자네는 이미 <clan_name> 클랜과 서약을 맺은 상태라고 나와 있군
			sprintf(sz_temp, (*g_pGameStringTable)[STRING_MESSAGE_CLAN_REGIST_FAIL_ALREADY_JOIN].GetString(), g_pUserInformation->GuildName.GetString());
			g_pUIDialog->PopupFreeMessageDlg( sz_temp );
		break;

		case NPC_RESPONSE_CLAN_REGIST_FAIL_QUIT_TIMEOUT:			// 자네는 다른 클랜을 탈퇴한지 얼마되지 않았군. 너무 이리저리 옮겨 다니는 것은 좋지 않지. 신중을 기하게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CLAN_REGIST_FAIL_QUIT_TIMEOUT].GetString() );
		break;

		case NPC_RESPONSE_CLAN_REGIST_FAIL_CANCEL_TIMEOUT:		// 자네는 클랜을 취소 당한 지 얼마 되지 않았군. 좀 더 신중하게 기회를 엿보도록 하게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CLAN_REGIST_FAIL_CANCEL_TIMEOUT].GetString() );
		break;

		case NPC_RESPONSE_CLAN_REGIST_FAIL_LEVEL:				// 훌륭하지만 우두머리로써의 자질은 좀 부족한 것 같군. 실력을 닦고 오게나.
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CLAN_REGIST_FAIL_LEVEL].GetString() );
		break;

		case NPC_RESPONSE_CLAN_REGIST_FAIL_MONEY:				// 클랜을 등록하려면 많은 돈이 필요하다네. 자네는 돈이 없어보이는군...
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CLAN_REGIST_FAIL_MONEY].GetString() );
		break;

		case NPC_RESPONSE_CLAN_REGIST_FAIL_FAME:					// <player_name>이라.. 아직 어린 뱀파이어인가보군. 더 많은 피를 마시고 다시 찾아오게나.
			sprintf(sz_temp, (*g_pGameStringTable)[STRING_MESSAGE_CLAN_REGIST_FAIL_FAME].GetString(), g_pUserInformation->CharacterID.GetString());
			g_pUIDialog->PopupFreeMessageDlg( sz_temp );
		break;

		case NPC_RESPONSE_CLAN_REGIST_FAIL_NAME:					// 클랜 이름이 이미 쓰이고 있군, 다른 이름을 생각해 보게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CLAN_REGIST_FAIL_NAME].GetString() );
		break;

		case NPC_RESPONSE_CLAN_REGIST_FAIL_DENY:				// <team name>클랜에서 자네를 거부하지 않았는가. 다른 클랜을 찾아보게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CLAN_REGIST_FAIL_DENY].GetString() );
		break;


		case NPC_RESPONSE_CLAN_STARTING_FAIL_ALREADY_JOIN:		// 당신은 이미 다른 클랜에 가입되어 있습니다.
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CLAN_STARTING_FAIL_ALREADY_JOIN].GetString() );
		break;

		case NPC_RESPONSE_CLAN_STARTING_FAIL_QUIT_TIMEOUT:		// 자네는 다른 클랜을 탈퇴한지 얼마되지 않았군. 너무 이리저리 옮겨 다니는 것은 좋지 않지. 신중을 기하게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CLAN_STARTING_FAIL_QUIT_TIMEOUT].GetString() );
		break;

		case NPC_RESPONSE_CLAN_STARTING_FAIL_CANCEL_TIMEOUT:		// 자네는 클랜을 취소 당한 지 얼마 되지 않았군. 좀 더 신중하게 기회를 엿보도록 하게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CLAN_STARTING_FAIL_CANCEL_TIMEOUT].GetString() );
		break;

		case NPC_RESPONSE_CLAN_STARTING_FAIL_LEVEL:				// 훌륭한 조력자가 되기에는 자질이 좀 부족한 것 같군. 가서 좀더 수련을 하고 오게나
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CLAN_STARTING_FAIL_LEVEL].GetString() );
		break;

		case NPC_RESPONSE_CLAN_STARTING_FAIL_MONEY:				// 기술이 월등해도 클랜을 유지할 수 있는 돈이 부족하면 그 클랜은 무너지기 쉽상이지
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CLAN_STARTING_FAIL_MONEY].GetString() );
		break;

		case NPC_RESPONSE_CLAN_STARTING_FAIL_FAME:				// <player_name>이라.. 난 아직 그런 이름은 들어보지도 못했다네. 그건 자네가 풋내기라는 것을 의미하지. 실력을 닦고 다시 찾아오게나
			sprintf(sz_temp, (*g_pGameStringTable)[STRING_MESSAGE_CLAN_STARTING_FAIL_FAME].GetString(), g_pUserInformation->CharacterID.GetString());
			g_pUIDialog->PopupFreeMessageDlg( sz_temp );
		break;

		case NPC_RESPONSE_CLAN_STARTING_FAIL_DENY:				// <team name>클랜에서 자네를 거부하지 않았는가. 다른 클랜을 찾아보게
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CLAN_STARTING_FAIL_DENY].GetString() );
		break;


		case NPC_RESPONSE_GUILD_SHOW_REGIST:             // 길드 등록 창 띄우기
			UI_ShowGuildRegist(pPacket->getParameter());
		break;
	
		case NPC_RESPONSE_GUILD_SHOW_STARTING_JOIN:      // 길드 스타팅 멤버 가입 창 띄우기
//			UI_ShowGuildStartJoin(pPacket->getParameter());
		break;

		case NPC_RESPONSE_GUILD_SHOW_JOIN:               // 길드 가입 창 띄우기
//			UI_ShowGuildJoin(pPacket->getParameter());
		break;

		case NPC_RESPONSE_GUILD_SHOW_QUIT:               // 길드 탈퇴 창 띄우기
//			UI_ShowGuildQuit();
		break;

		case NPC_RESPONSE_GUILD_ERROR:                   // 길드 오류
		break;

		case NPC_RESPONSE_TRADE_GIFT_BOX_OK:             // 선물 상자 교환 성공
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TRADE_GIFT_BOX_OK].GetString() );
		break;

		case NPC_RESPONSE_TRADE_GIFT_BOX_NO_ITEM:        // 선물 상자 없다
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TRADE_GIFT_BOX_NO_ITEM].GetString() );
		break;

		case NPC_RESPONSE_TRADE_GIFT_BOX_ALREADY_TRADE:  // 선물 상자를 이미 한번 교환했다.
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_TRADE_GIFT_BOX_ALREADY_TRADE].GetString() );
		break;

		case NPC_RESPONSE_TRADE_GIFT_BOX_ERROR:          // 선물 상자 교환에 따른 기타 오류
		break;

		case NPC_RESPONSE_REWARD_OK:					// 보상을 받았습니다.
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_REWARD_OK].GetString() );
		break;

		case NPC_RESPONSE_REWARD_FAIL:					// 보상을 받을 수 없습니다.
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_REWARD_FAIL].GetString() );
		break;

		case NPC_RESPONSE_NO_EMPTY_SLOT:				// 빈 자리가 없습니다.
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_NO_EMPTY_SLOT].GetString() );
		break;

		case NPC_RESPONSE_SHOW_TAX_BALANCE :			// 성에 쌓여있는 세금의 잔액을 보여준다.
			UI_OpenBringFee(pPacket->getParameter());
			break;

		case NPC_RESPONSE_WITHDRAW_TAX_OK :				// 길드 마스터가 세금을 찾는 데에 성공했다.
			UI_SetTotalFee(pPacket->getParameter());
			if(g_pTempInformation->GetMode() == TempInformation::MODE_WAIT_BRING_FEE)
			{
				// 서버에서 날려준다니.. 무효-_-
				//g_pMoneyManager->AddMoney(g_pTempInformation->Value1);
				// 검증 완료
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);
			}
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_SUCCESS_BRING_FEE].GetString());
			break;

		case NPC_RESPONSE_WITHDRAW_TAX_FAIL :			// 길드 마스터가 세금을 찾는 데에 실패했다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_FAIL_BRING_FEE].GetString());
			
			// 검증-_- 완료 
			if(g_pTempInformation->GetMode() == TempInformation::MODE_WAIT_BRING_FEE)
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);

			break;			
		
		case NPC_RESPONSE_NO_GUILD :					// 팀(클랜)에 소속되어 있지 않습니다.
			if(g_pPlayer->IsSlayer())			
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_NO_TEAM].GetString());
			else
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_NO_CLAN].GetString());
			break;
		
		case NPC_RESPONSE_NOT_GUILD_MASTER:				// 팀(클랜) 마스터가 아닙니다.
			if(g_pPlayer->IsSlayer())			
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_NOT_TEAM_MASTER].GetString());
			else
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_NOT_CLAN_MASTER].GetString());
			break;
		
		case NPC_RESPONSE_HAS_NO_CASTLE:			    // 팀(클랜)이 가진 성이 없습니다.
			if(g_pPlayer->IsSlayer())			
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_TEAM_HAS_NO_CASTLE].GetString());
			else
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_CLAN_HAS_NO_CASTLE].GetString());
			break;
		
		case NPC_RESPONSE_NOT_YOUR_CASTLE:		        // 팀(클랜)이 소유한 성이 아닙니다.
			if(g_pPlayer->IsSlayer())			
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_TEAM_NOT_YOUR_CASTLE].GetString());
			else
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_CLAN_NOT_YOUR_CASTLE].GetString());
			break;
		
		case NPC_RESPONSE_NOT_ENOUGH_MONEY :			// 돈이 부족합니다.(전쟁신청금 부족이지만 범용적으로 쓸려고 -_-;)
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_WAR_NOT_ENOUGH_MONEY].GetString());
			break;
		
		case NPC_RESPONSE_WAR_SCHEDULE_FULL :			// 전쟁 스케쥴이 꽉 찼습니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_WAR_SCHEDULE_FULL].GetString());
			break;
		
		case NPC_RESPONSE_WAR_ALREADY_REGISTERED :        // 이미 전쟁을 신청했었습니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_WAR_ALREADY_REGISTERED].GetString());
			break;
		
		case NPC_RESPONSE_WAR_REGISTRATION_OK :			// 전쟁 스케쥴에 등록 되었습니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_WAR_REGISTRATION_OK].GetString());
			break;
		
		case NPC_RESPONSE_ALREADY_HAS_CASTLE :            // 이미 성을 가지고 있습니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_ALREADY_HAS_CASTLE].GetString());			
			break;
		
		case NPC_RESPONSE_WAR_UNAVAILABLE :               // 지금은 전쟁 신청이 되지 않습니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_WAR_UNAVAILABLE].GetString());
			break;

		case NPC_RESPONSE_RACE_WAR_JOIN_FAILED :               // <사용자> 님 레벨대의 종족 전쟁 신청인원이 꽉 찼습니다.
			sprintf(sz_temp, (*g_pGameStringTable)[STRING_MESSAGE_RACE_WAR_JOIN_FAILED].GetString(), g_pUserInformation->CharacterID.GetString());
			g_pUIDialog->PopupFreeMessageDlg( sz_temp );
			break;

		case NPC_RESPONSE_RACE_WAR_JOIN_OK :               // 종족 전쟁 참가 신청을 했습니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_RACE_WAR_JOIN_OK].GetString());
			break;

		case NPC_RESPONSE_RACE_WAR_GO_FIRST_SERVER :               // 종족 전쟁은 각 월드의 첫번째 서버에서만 신청 및 참가가 가능합니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_RACE_WAR_GO_FIRST_SERVER].GetString());
			break;

		case NPC_RESPONSE_GIVE_EVENT_ITEM_FAIL_NOW:      // 지금은 이벤트 아이템을 받을 수 없습니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_GIVE_EVENT_ITEM_FAIL_NOW].GetString());
		break;
    		
		case NPC_RESPONSE_GIVE_EVENT_ITEM_FAIL:          // 이벤트 아이템을 받을 수 없습니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_GIVE_EVENT_ITEM_FAIL].GetString());
		break;

		case NPC_RESPONSE_GIVE_EVENT_ITEM_OK:            // 이벤트에 따른 아이템을 받았습니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_GIVE_EVENT_ITEM_OK].GetString());
		break;

		case NPC_RESPONSE_GIVE_PREMIUM_USER_ONLY:        // 프리미엄서비스 사용자만 받을 수 있습니다.
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_GIVE_PREMIUM_USER_ONLY].GetString());
		break;

		// 커플신청 관련
		case NPC_RESPONSE_WAIT_FOR_MEET_COUPLE:			// 커플 신청 할 상대의 이름을 입력하세요
			UI_RunInputNameWindow(0);
		break;

		case NPC_RESPONSE_COUPLE_MEET_SUCCESS:			// 커플이 성사되었습니다.
			UI_CloseInputNameWindow();
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_COUPLE_MEET_SUCCESS].GetString());
			
		break;

		case NPC_RESPONSE_COUPLE_CANNOT_MEET:			// 커플이 성사될 수 없습니다.CoupleMessage Enum 사용
			UI_CloseInputNameWindow();
			if(pPacket->getParameter() != 0 && pPacket->getParameter() < COUPLE_MESSAGE_MAX)
				// enum이 1부터 시작하므로 -1해준다. MGameStringTabe의 순서와 맞출것!!!
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_COUPLE_NOT_EVENT_TERM+pPacket->getParameter()-1].GetString());
			else
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_COUPLE_CANNOT_MEET].GetString());
			
		break;

		case NPC_RESPONSE_MEET_WAIT_TIME_EXPIRED:		// 시간이 지나서 신청이 취소되었습니다.
			UI_CloseInputNameWindow();
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_MEET_WAIT_TIME_EXPIRED].GetString());			
		break;

		case NPC_RESPONSE_WAIT_FOR_APART_COUPLE:		// 헤어질 상대의 이름을 입력하세요
			UI_RunInputNameWindow(1);
		break;

		case NPC_RESPONSE_COUPLE_APART_SUCCESS:			// 헤어졌습니다.
			UI_CloseInputNameWindow();
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_COUPLE_APART_SUCCESS].GetString());
			g_pSkillAvailable->SetAvailableSkills();			
		break;

		case NPC_RESPONSE_NOT_COUPLE:					// 커플이 아니라서 헤어질 수 없습니다.CoupleMessage Enum 사용
			UI_CloseInputNameWindow();
			if(pPacket->getParameter() != 0 && pPacket->getParameter() < COUPLE_MESSAGE_MAX)
				// enum이 1부터 시작하므로 -1해준다. MGameStringTabe의 순서와 맞출것!!!
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_COUPLE_NOT_EVENT_TERM+pPacket->getParameter()-1].GetString());
			else
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_NOT_COUPLE].GetString());
		break;

		case NPC_RESPONSE_APART_WAIT_TIME_EXPIRED:		// 시간이 지나서 신청이 취소되었습니다.
			UI_CloseInputNameWindow();
			g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_MESSAGE_MEET_WAIT_TIME_EXPIRED].GetString());
		break;

		case NPC_RESPONSE_APART_COUPLE_FORCE :            // 일방적으로 헤어질 상대의 이름을 입력하세요.
			UI_RunInputNameWindow(2);
			break;
		case NPC_RESPONSE_QUEST :
			switch( (QuestMessage) pPacket->getParameter() )
			{
			case START_SUCCESS :
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_START_MONSTER_KILL_QUEST].GetString() );
				break;
			case START_FAIL_QUEST_NUM_EXCEEDED :				
			case START_FAIL_DUPLICATED_QUEST_ID :
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_FAIL_ALREADY_START_MONSTER_KILL_QUEST].GetString() );
				break;
			case START_FAIL_PC :
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_FAIL_MONSTER_KILL_QUEST_BY_STATUS].GetString() );
				break;
			case COMPLETE_SUCCESS :
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_SUCCESS_MONSTER_KILL_QUEST].GetString() );
				//g_pEventManager->RemoveEvent( EVENTID_MONSTER_KILL_QUEST );
				UI_InitQuestStatus();				
				break;
			case COMPLETE_FAIL_NOT_COMPLETE :
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_YET_COMPLETE_MONSTER_KILL_QUEST].GetString() );
				break;
			case COMPLETE_FAIL_NO_INVENTORY_SPACE :
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_INVENTORY_FULL_MONSTER_KILL_QUEST].GetString() );
				break;
			case COMPLETE_FAIL_NOT_IN_QUEST :
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_IN_QUEST].GetString() );
				break;
			case COMPLETE_FAIL_TIME_EXPIRED :
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_FAIL_QUEST_EXPIRED_TIME].GetString() );
				break;
			case START_FAIL_CANNOT_APPLY_QUEST :
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_APPLY_QUEST].GetString() );
				break;
			case CANCEL_SUCCESS :
				//				g_pEventManager->RemoveEvent( EVENTID_MONSTER_KILL_QUEST );				
				UI_InitQuestStatus();
				// NPC Script 를 찍어줘야 하므로, 다시 락을 걸어준다.
				g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_NPC_ASK );
				break;
			case CANCEL_NOT_IN_QUEST :
				g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_NPC_ASK );
				break;
			case COMPLETE_FAIL_INVALID_NPC :
				// 해당 NPC 에게 보상을 받을 수 없습니다.
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_FAIL_INVALID_NPC].GetString() );
				break;
			case FAIL_BUG :
				g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[UI_STRING_MESSAGE_FAIL_BUG].GetString() );
				break;
			}			
			break;
		case NPC_RESPONSE_LOTTERY :
			{
				int step = -1;
				if( pPacket->getParameter() >= 0 && pPacket->getParameter() <= 4 )
					step = pPacket->getParameter() + 1;
				
				if(step != -1 )
				{
					UI_RunLottery( step );
				}
			}
			break;
		case NPC_RESPONSE_CANNOT_BUY :			
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_TAKE_OUT_ITEM_FROM_SHOP].GetString() );
			break;
		case NPC_RESPONSE_CLEAR_RANK_BONUS_OK :           // 선택하신 계급의 스킬이 삭제 되었습니다.
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CLEAR_RANK_BONUS_OK].GetString() );
			break;
		case NPC_RESPONSE_NO_RANK_BONUS :                 // 해당 자격이 없습니다.
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_NO_RANK_BONUS].GetString() );
			break;
		case NPC_RESPONSE_ALREADY_CLEAR_RANK_BONUS :      // 선택하신 계급 스킬을 삭제한 적이 있습니다.
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_ALREADY_CLEAR_RANK_BONUS].GetString() );
			break;
		case NPC_RESPONSE_GNOME_CONTRACT_OK :
			g_pUIDialog->ClosePCTalkDlg();
			UI_PopupMessage( STRING_MESSAGE_CONTRACT_GNOMES_HORN_OK );
			break;
		case NPC_RESPONSE_DOWN_SKILL :
			UI_DownSkill();
			break;
			
		case NPC_RESPONSE_GUILD_REGIST_FAIL_ALREADY_JOIN :		// 어디 한번 볼까? 자네는 이미 <guild_name> 길드 소속이라고 나와 있군
			wsprintf(sz_temp, (*g_pGameStringTable)[STRING_MESSAGE_GUILD_REGIST_FAIL_ALREADY_JOIN].GetString(), g_pUserInformation->GuildName.GetString());
			g_pUIDialog->PopupFreeMessageDlg( sz_temp );
			break;
		case NPC_RESPONSE_GUILD_REGIST_FAIL_QUIT_TIMEOUT :		// 자네는 다른 길드를 탈퇴한지 얼마 되지 않았군. 조금 더 신중하게 생각하고 행동하게
			UI_PopupMessage( STRING_MESSAGE_GUILD_REGIST_FAIL_QUIT_TIMEOUT );
			break;
		case NPC_RESPONSE_GUILD_REGIST_FAIL_CANCEL_TIMEOUT :		// 자네는 길드를 취소 당한 지 얼마 되지 않았군. 등록 조건에 부합하도록 자신을 좀 더 기르고 오게. 좀 더 신중하게 기회를 엿보도록 하게
			UI_PopupMessage( STRING_MESSAGE_GUILD_REGIST_FAIL_CANCEL_TIMEOUT );
			break;
		case NPC_RESPONSE_GUILD_REGIST_FAIL_LEVEL :				// 훌륭하지만 리더로써의 자질은 좀 부족한 것 같군. 실력을 닦고 오게나
			UI_PopupMessage( STRING_MESSAGE_GUILD_REGIST_FAIL_LEVEL );
			break;
		case NPC_RESPONSE_GUILD_REGIST_FAIL_MONEY :				// 길드를 만들려면 많은 돈이 필요하다네. 자네는 돈이 없어 보이는군...
			UI_PopupMessage( STRING_MESSAGE_GUILD_REGIST_FAIL_MONEY );
			break;
		case NPC_RESPONSE_GUILD_REGIST_FAIL_FAME :				// <player_name>이라.. 난 아직 그런 이름은 들어보지도 못했다네. 그건 자네가 풋내기라는 것을 의미하지. 실력을 닦고 다시 찾아오게나
			wsprintf(sz_temp, (*g_pGameStringTable)[STRING_MESSAGE_GUILD_REGIST_FAIL_FAME].GetString(), g_pUserInformation->CharacterID.GetString() );
			g_pUIDialog->PopupFreeMessageDlg( sz_temp );
			break;
		case NPC_RESPONSE_GUILD_REGIST_FAIL_NAME :				// 길드 이름이 이미 쓰이고 있군, 다른 이름을 생각해 보게
			UI_PopupMessage( STRING_MESSAGE_GUILD_REGIST_FAIL_NAME );
			break;
		case NPC_RESPONSE_GUILD_REGIST_FAIL_DENY :				// 거부되었습니다.
			UI_PopupMessage( STRING_MESSAGE_GUILD_REGIST_FAIL_DENY );
			break;			
		case NPC_RESPONSE_GUILD_STARTING_FAIL_ALREADY_JOIN :		// 당신은 이미 다른 길드에 가입되어 있습니다.
			UI_PopupMessage( STRING_MESSAGE_GUILD_STARTING_FAIL_ALREADY_JOIN );
			break;
		case NPC_RESPONSE_GUILD_STARTING_FAIL_QUIT_TIMEOUT :		// 자네는 다른 길드를 탈퇴한지 얼마 되지 않았군. 조금 더 신중하게 생각하고 행동하게
			UI_PopupMessage( STRING_MESSAGE_GUILD_STARTING_FAIL_QUIT_TIMEOUT );
			break;
		case NPC_RESPONSE_GUILD_STARTING_FAIL_CANCEL_TIMEOUT :	// 자네는 길드를 취소 당한 지 얼마 되지 않았군. 조건에 부합하도록 자신을 좀 더 기르고 오게. 좀 더 신중하게 기회를 엿보도록 하게
			UI_PopupMessage( STRING_MESSAGE_GUILD_STARTING_FAIL_CANCEL_TIMEOUT );
			break;
		case NPC_RESPONSE_GUILD_STARTING_FAIL_LEVEL :				// 자네는 아직 부족한 점이 많아 보이는구만. 조금 더 실력을 닦고 찾게나.
			UI_PopupMessage( STRING_MESSAGE_GUILD_STARTING_FAIL_LEVEL );
			break;
		case NPC_RESPONSE_GUILD_STARTING_FAIL_MONEY :				// <player_name>, 길드 등록을 위해서는 돈이 더 필요하다네
			wsprintf( sz_temp, (*g_pGameStringTable)[STRING_MESSAGE_GUILD_STARTING_FAIL_MONEY].GetString(), g_pUserInformation->CharacterID.GetString() );
			g_pUIDialog->PopupFreeMessageDlg( sz_temp );
			break;
		case NPC_RESPONSE_GUILD_STARTING_FAIL_FAME :				// <player_name>이라.. 난 아직 그런 이름은 들어보지도 못했다네. 그건 자네가 풋내기라는 것을 의미하지. 실력을 닦고 다시 찾아오게나
			wsprintf( sz_temp, (*g_pGameStringTable)[STRING_MESSAGE_GUILD_STARTING_FAIL_FAME].GetString(), g_pUserInformation->CharacterID.GetString() );
			g_pUIDialog->PopupFreeMessageDlg( sz_temp );
			break;
		case NPC_RESPONSE_GUILD_STARTING_FAIL_DENY :			// 거부되었습니다.
			UI_PopupMessage( STRING_MESSAGE_GUILD_REGIST_FAIL_DENY );
			break;
		case NPC_RESPONSE_TOO_MANY_GUILD_REGISTERED:             // 공성 신청한 길드가 너무 많아서 신청할 수 없습니다.   // 120
			UI_PopupMessage( STRING_MESSAGE_TOO_MANY_GUILD_REGISTERED );
			break;
		case NPC_RESPONSE_REINFORCE_DENYED:                      // 수비측 참전 신청이 이미 거부당해서 재신청 할 수 없습니다.    // 121
			UI_PopupMessage( STRING_MESSAGE_REINFORCE_DENYED );
			break;
		case NPC_RESPONSE_ALREADY_REINFORCE_ACCEPTED:            // 수비측 참전 신청이 허가된 길드가 이미 있어서 신청할 수 없습니다. // 122
			UI_PopupMessage( STRING_MESSAGE_ALREADY_REINFORCE_ACCEPTED );
			break;
		case NPC_RESPONSE_NO_WAR_REGISTERED:                     // 공성 신청한 길드가 없어서 수비측 참전 신청을 할 수 없습니다. 
			UI_PopupMessage( STRING_MESSAGE_NO_WAR_REGISTERED );
			break;
		case NPC_RESPONSE_CANNOT_ACCEPT:
			UI_PopupMessage( STRING_MESSAGE_CANNOT_ACCEPT );//] = "수비측 참전 신청을 수락할 수 없습니다";
			break;
		case NPC_RESPONSE_ACCEPT_OK:
			UI_PopupMessage( STRING_MESSAGE_ACCEPT_OK );//] = "참전 신청을 수락하였습니다.";
			break;
		case NPC_RESPONSE_CANNOT_DENY:
			UI_PopupMessage( STRING_MESSAGE_CANNOT_DENY );//] = "수비측 참전 신청을 거부할 수 없습니다.
			break;
		case NPC_RESPONSE_DENY_OK:
			UI_PopupMessage( STRING_MESSAGE_DENY_OK );//] = "참전 신청을 거부하였습니다.;
			break;
			
		case NPC_RESPONSE_SHOW_TAX_RATIO:						// 유저가 살때의 세율 조절창 뜨게 한다.
			UI_RunModifyTax();
			break;
		case NPC_RESPONSE_MODIFY_TAX_RATIO_OK:					// 세율 변경 오케이~
			UI_PopupMessage( UI_STRING_MESSAGE_MODIFY_TAX_OK );
			break;
		case NPC_RESPONSE_MODIFY_TAX_RATIO_FAIL:				// 세율 변경 실패
			UI_PopupMessage( UI_STRING_MESSAGE_MODIFY_TAX_FAIL );
			break;
		case NPC_RESPONSE_SWAP_ADVANCEMENT_ITEM:
			UI_Show_Swap_Advancement_Item();
			break;
		case NPC_RESPONSE_NOT_ADVANCED:
			UI_PopupMessage( UI_STRING_MESSAGE_SWAP_ERROR );
			break;
		case NPC_RESPONSE_SHOW_DONATION_DAILOG:                  // 기부창을 띄운다.
		//	value
			UI_Run_Campaign_Help_Unfortunate_Neighbors(value);
			break;
		case NPC_RESPONSE_SHOW_DONATION_COMPLETE_DAILOG:         // 기부 성공 창을 띄운다.
			UI_PopupMessage( UI_STRING_MESSAGE_CAMPAIGN_HELP_THANKS );
			break;
			
		case NPC_RESPONSE_SHOW_CONFIRM_GET_EVENT_ITEM_DIALOG:    // 이벤트 아이템 받기 확인 다이얼로그를 띄운다.
			UI_Run_Confirm_GetEventItem(value);
			break;

		case NPC_RESPONSE_SHOW_COMMON_MESSAGE_DIALOG:           // 각종 메시지 다이얼로그를 띄운다.
			switch(value)
			{
			case YOU_CAN_GET_EVENT_200412_COMBACK_ITEM:
				gC_vs_ui.RunPopupMessage(TXT_TUTORIAL_COMEBACKEVENT_NOTICE, C_VS_UI_POPUP_MESSAGE::POPUP_WELCOME );
				//UI_PopupMessage( UI_STRING_MESSAGE_GET_EVENT_ITEM_COMEBACK_1 );
				break;
			case YOU_CAN_GET_EVENT_200412_COMBACK_PREMIUM_ITEM:
				gC_vs_ui.RunPopupMessage(TXT_TUTORIAL_COMEBACKEVENT_PAY, C_VS_UI_POPUP_MESSAGE::POPUP_WELCOME );
				//UI_PopupMessage( UI_STRING_MESSAGE_GET_EVENT_ITEM_COMEBACK_2 );
				break;
			case YOU_CAN_GET_EVENT_200412_COMBACK_RECOMMEND_ITEM:
				gC_vs_ui.RunPopupMessage(TXT_TUTORIAL_COMEBACKEVENT_RECOMMEND, C_VS_UI_POPUP_MESSAGE::POPUP_WELCOME );
				//UI_PopupMessage( UI_STRING_MESSAGE_GET_EVENT_ITEM_COMEBACK_3 );
				break;
			case YOU_GET_EVENT_ITEM:
				UI_PopupMessage( UI_STRING_MESSAGE_GET_EVENT_ITEM_RECEIVE_OK );
				break;
			case NOT_ENOUGH_INVENTORY_SPACE:
				UI_PopupMessage( UI_STRING_MESSAGE_INVENTORY_FULL_MONSTER_KILL_QUEST );
				break;
			case ALEADY_GET_EVENT_ITEM:
				UI_PopupMessage( UI_STRING_MESSAGE_GET_EVENT_ITEM_RECEIVE_ALREADY );
				break;
			case FAIL_GET_EVENT_ITEM:
				UI_PopupMessage( UI_STRING_MESSAGE_GET_EVENT_ITEM_RECEIVE_FAIL );
				break;
			case YOU_ARE_NOT_EVENT_USER:
				UI_PopupMessage( UI_STRING_MESSAGE_GET_EVENT_ITEM_NOT_EVENT_USER );
				break;

			}
			break;

	}
#endif

	__END_CATCH

}




























