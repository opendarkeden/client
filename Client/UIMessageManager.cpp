//-----------------------------------------------------------------------------
// UIMessageManager.cpp
//-----------------------------------------------------------------------------
// QuickSlot에 뭔가 할려는 경우에.. QuickSlot이 없어질 수가 있다... 
// 지금 UI에서 타이밍 문제가 있어서 못 고치고 있음.
//-----------------------------------------------------------------------------
#include "Client_PCH.h"
#include <io.h>
#include <process.h>
#include <stdio.h>
#include "Client.h"
#include "UIMessageManager.h"
#include "UIFunction.h"
#include "VS_UI.h" 
#include "VS_UI_Mouse_pointer.h"
#include "TalkBox.h"
#include "MShopShelf.h"
#include "MPriceManager.h"
#include "MMoneyManager.h"
#include "UIDialog.h"
#include "SkillDef.h"
#include "ClientFunction.h"
#include "MGameStringTable.h"
#include "MItemOptionTable.h"
#include "CToken.h"
#include "UserOption.h"
#include "MWeather.h"
#include "MStatusManager.h"
#include "MChatManager.h"
#include "PacketDef.h"
#include "UtilityFunction.h"
#include "ClientConfig.h"
#include "MMusic.h"
#include "MMusicTable.h"
#include "SoundDef.h"
#include "ServerInformation.h"
#include "MParty.h"
#include "MItemFinder.h"
#include "ServerInfo.h"
#include "MZoneTable.h"
//#include "MFileDef.h"
#include "Properties.h"
#include "UserInformation.h"
#include "TempInformation.h"
#include "ProfileManager.h"
#include "CGameUpdate.h"
#include "MTestDef.h"

#include "Packet\CPackets\CGRequestNewbieItem.h"
#include "Packet\CPackets\CGSilverCoating.h"
#include "Packet\CPackets\CLChangeServer.h"
#include "Packet\CPackets\CLGetServerList.h"
#include "Packet\CPackets\CGSelectPortal.h"
#include "Packet\CPackets\CGSelectWayPoint.h"
#include "Packet\CPackets\CGPartyInvite.h"
#include "Packet\CPackets\CGPartyLeave.h"
#include "packet\Cpackets\CGResurrect.h"
#include "packet\Cpackets\CLSelectWorld.h"
#include "packet\Cpackets\CLSelectServer.h"
#include "packet\Cpackets\CLQueryCharacterName.h"
#include "packet\CPackets\CGRelicToObject.h"
#include "packet\CPackets\CGSelectGuild.h"
#include "packet\CPackets\CGSelectGuildMember.h"
#include "packet\CPackets\CGRequestGuildMemberList.h"
#include "packet\CPackets\CGTryJoinGuild.h"
#include "packet\CPackets\CGJoinGuild.h"
#include "packet\CPackets\CGRegistGuild.h"
#include "packet\CPackets\CGModifyGuildMember.h"
#include "packet\CPackets\CGAddItemToItem.h"
#include "packet\CPackets\CGModifyGuildIntro.h"
#include "packet\CPackets\CGModifyGuildMemberIntro.h"
#include "packet\CPackets\CGSkillToNamed.H"
#include "packet\CPackets\CGSelectRankBonus.H"
#include "packet\CPackets\CGWithdrawTax.H"
#include "packet\CPackets\CGTypeStringList.h"
#include "packet\CPackets\CGLotterySelect.h"
#include "packet\CPackets\CGTakeOutGood.h"
#include "packet/CPackets/CGMixItem.h"					// -_- 비줠 어시스트에서 이렇게 하면 나온다고하길래
#include "packet/CPackets/CGDownSkill.h"
#include "packet/GPackets/GCMiniGameScores.h"
#include "packet/CPackets/CGSubmitScore.h"
#include "packet/CPackets/CGAddItemToCodeSheet.h"
#include "packet/CPackets/CGFailQuest.h"
#include "packet/CPackets/CGSelectRegenZone.h"
#include "packet/CPackets/CGTameMonster.h"
#include "packet/CPackets/CGPetGamble.h"
#include "packet\CPackets\CGUseMessageItemFromInventory.h"
#include "packet\Cpackets\CGPartySay.h"
#include "packet\Cpackets\CGDepositPet.h"
#include "packet\Cpackets\CGWithdrawPet.h"
#include "packet\Cpackets\CGSMSSend.h"
#include "packet\Cpackets\CGSMSAddressList.h"
#include "packet\Cpackets\CGDeleteSMSAddress.h"
#include "packet\Cpackets\CGAddSMSAddress.h"
#include "packet\Cpackets\CGSelectNickname.h"
#include "packet\Cpackets\CGModifyNickname.h"
#include "packet\Cpackets\CGGQuestAccept.h"
#include "packet\Cpackets\CGGQuestCancel.h"
#include "packet\Cpackets\CGUseItemFromGQuestInventory.h"
#include "packet\Cpackets\CLAgreement.h"

#include "RequestFunction.h"
#include "RequestServerPlayerManager.h"
#include "RequestUserManager.h"
#include "ClientCommunicationManager.h"
#include "WhisperManager.h"
#include "packet\Rpackets\RCSay.h"
#include "packet\Cpackets\CGGuildChat.h"
#include "CMP3.h"
#include "RankBonusTable.h"
#include "Profiler.h"

#include "DebugInfo.h"
#include "MEventManager.h"
#include "MOustersGear.h"
#include "SoundSetting.h"
#include "ShrineInfoManager.h"

#include "MTimeItemManager.h"
#include "MFakeCreature.h"

#include "packet\Cpackets\CGRequestUnion.h"
#include "packet\Cpackets\CGQuitUnion.h"
#include "packet\Cpackets\CGExpelGuild.h"
#include "Packet\Cpackets\CGRequestGuildList.h"
#include "Packet\Cpackets\CGRequestUnionInfo.h"
#include "Packet\Cpackets\CGAcceptUnion.h"
#include "Packet\Cpackets\CGDenyUnion.h"
#include "Packet\Cpackets\CGQuitUnionAccept.h"
#include "Packet\Cpackets\CGQuitUnionDeny.h"
#include "Packet\Cpackets\CGRangerSay.h"
#include "Packet\Cpackets\CGModifyTaxRatio.h"
#include "Packet\Cpackets\CGAppointSubmaster.h"
//
#include "packet\Cpackets\CGRequestUnion.h"
#include "packet\Cpackets\CGQuitUnion.h"
#include "packet\Cpackets\CGExpelGuild.h"
#include "Packet\Cpackets\CGRequestGuildList.h"
#include "Packet\Cpackets\CGRequestUnionInfo.h"
#include "Packet\Cpackets\CGAcceptUnion.h"
#include "Packet\Cpackets\CGDenyUnion.h"
#include "Packet\Cpackets\CGQuitUnionAccept.h"
#include "Packet\Cpackets\CGQuitUnionDeny.h"
#include "Packet\Cpackets\CGRangerSay.h"
#include "Packet\Cpackets\CGModifyTaxRatio.h"
#include "Packet\Cpackets\CGAppointSubmaster.h"

#include "Packet/Cpackets/CGDisplayItem.h"
#include "Packet/Cpackets/CGUndisplayItem.h"
#include "Packet/Cpackets/CGStoreOpen.h"
#include "Packet/Cpackets/CGStoreClose.h"
#include "Packet/Cpackets/CGStoreSign.h"
#include "Packet/Cpackets/CGRequestStoreInfo.h"
#include "Packet/Gpackets/GCMyStoreInfo.h"
#include "Packet/Gpackets/GCOtherStoreInfo.h"
#include "Packet/Cpackets/CGBuyStoreItem.h"
#include "Packet/Gpackets/GCRemoveStoreItem.h"
#include "Packet/Gpackets/GCAddStoreItem.h"
//

#include "Packet\Cpackets\CGUsePowerPoint.h"
#include "Packet\Cpackets\CGRequestPowerPoint.h"
#include "Packet\Cpackets\CGDonationMoney.h"
#include "Packet\Cpackets\CGGetEventItem.h"

#include "Packet\Cpackets\CGRequestWebMarket.h"

//-----------------------------------------------------------------------------
// Chat에서 사용하는 특수한 문자
//-----------------------------------------------------------------------------
#define	SYMBOL_GLOBALCHAT			'!'			// zone 채팅
#define	SYMBOL_WHISPER				'/'			// 귓속말
#define	SYMBOL_COMMAND				'@'			// 특수 명령어
#define SYMBOL_MASTER_COMMAND		'*'			// 운영자용 특수 명령어

#define	STRING_IGNORE				"거부"		// 대화 거부
#define	STRING_IGNORE_ENG			"ignore"	// 대화 거부
#define	STRING_ACCEPT				"허용"		// 대화 허용
#define	STRING_ACCEPT_ENG			"accept"	// 대화 허용
#define	STRING_ACCEPT_CURSE			"뭐라고?"	// 나쁜말 보기
#define	STRING_ACCEPT_CURSE_ENG		"what?"		// 나쁜말 보기
#define	STRING_FILTER_CURSE			"바른말"	// 바른말만 보기
#define	STRING_FILTER_CURSE_ENG		"filter"	// 바른말만 보기

int g_C2G = 0;

//-----------------------------------------------------------------------------
// extern
//-----------------------------------------------------------------------------
#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
	extern int g_ShowImageObjectID;
#endif

#ifdef OUTPUT_DEBUG
	extern MString g_ProfileInfoName;
#endif

bool							g_bEventQuestEnding = false;
extern bool		LoadingAddonSPK(bool bLoading);
extern CGameUpdate*		g_pCGameUpdate;

//-----------------------------------------------------------------------------
// Global
//-----------------------------------------------------------------------------
UIMessageManager*	g_pUIMessageManager = NULL;

extern BOOL g_bEnable3DHAL;
extern BOOL g_bSetHotKey;
extern bool g_bWatchMode;
extern bool	g_bTestMode;
extern bool g_bHolyLand;
extern bool g_bZoneSafe;

//-----------------------------------------------------------------------------
// Execute Logout
//-----------------------------------------------------------------------------
void
ExecuteLogout()
{
	//--------------------------------------------------
	// 살아있는 경우
	//--------------------------------------------------
	if (g_Mode==MODE_GAME)
	{
		DEBUG_ADD("ExecuteLogout");

		UI_SaveUserOption();

		// 날씨 멈춤
		SetWeather(WEATHER_CLEAR, 0);

		// 죽었으면 부활 패킷을 보내고 Logout한다.
		// 근데.. 실제 처리될때 별 의미가 없네.. - -;
		if (!g_pPlayer->IsAlive())
		{
				CGResurrect _CGResurrect;
				
				g_pSocket->sendPacket( &_CGResurrect );				

			gC_vs_ui.FinishRequestDie();
			gC_vs_ui.FinishRequestResurrect();
			gC_vs_ui.CloseRequestShrineMinimap();
		}

		if (1)//g_pPlayer->IsAlive())
		{
			//--------------------------------------------------
			// 음악 멈춘다.
			//--------------------------------------------------
			if (g_pUserOption->PlayWaveMusic)
			{
//				g_pDXSoundStream->Stop();
				DEBUG_ADD("MP3 STOP7");
#ifdef __USE_MP3__
				g_pMP3->Stop();
#else
				if( g_DXSound.IsInit() )
					g_pOGG->streamClose();
#endif
				DEBUG_ADD("MP3 STOP7 OK");
			}
			else
			{
				g_Music.Stop();
			}

			//--------------------------------------------------
			// Thread Loading 종료..
			//--------------------------------------------------
			StopLoadingThread();

			//
			// Client, Server에 logout packet 보냄.
			//
			//--------------------------------------------------
			// 게임 서버로 CGLogout 패킷을 보낸다.
			//--------------------------------------------------
				UI_SaveHotKeyToServer();

				CGLogout cgLogout;
			
				g_pSocket->sendPacket( &cgLogout );
				//g_pSocket->setPlayerStatus( CPS_END_SESSION );

				g_pSocket->processOutput();

				g_pSocket->setPlayerStatus( CPS_WAITING_FOR_GC_RECONNECT_LOGIN );	

				
				//g_pSocket->disconnect();

				// 2001.6.12 : release하지 않고 재접한다.
				//ReleaseSocket();

			/*
			if (g_pZone != NULL)
			{
				g_pZone->RemovePlayer();
				g_pPlayer->SetStop();
				g_pPlayer->SetAction( ACTION_STAND );
				//g_pZone->UnSetLight(g_pPlayer->GetX(), g_pPlayer->GetY(), g_pPlayer->GetLightSight());	// 시야 제거
			}
			*/

			// 캐릭터 선택창으로 간다는 message
			/*
			if (g_pCGameUpdate!=NULL)
			{
				gC_vs_ui.CloseGameMenu();

				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_WAIT_FOR_CHARACTER_SELECT_MODE].GetString(), -1, -1, 0 );			
				
				g_pCGameUpdate->UpdateDraw();				
				CDirectDraw::Flip();

				g_pCGameUpdate->UpdateDraw();				
				CDirectDraw::Flip();
			}
			*/

			// 날씨 멈춤..
			//SetWeather(WEATHER_CLEAR, 0);
			ReleaseGameObject();

			// 2001.6.12 : 재접속~
			//SetMode( MODE_MAINMENU );	
			SetMode( MODE_WAIT_RECONNECT_LOGIN );

			//------------------------------------------------------
			// [ TEST CODE ]
			// 여기서 sound를 초기화해도 되남??
			//------------------------------------------------------
			//InitSound();
			g_CurrentFrame		= 0;

			g_bSetHotKey	= FALSE;

			//------------------------------------------------------
			// ui dialog
			//------------------------------------------------------
			g_pUIDialog->HidePCTalkDlg();
			g_pUIDialog->CloseMessageDlg();


			//------------------------------------------------------
			// 모두 대화 허용
			//------------------------------------------------------
			g_pChatManager->ClearID();
			g_pChatManager->SetAcceptMode();

//			gC_vs_ui.ServerDisconnectMessage();
//			gC_vs_ui.CloseAllDialog();

			// Title화면 UI시작
			//gC_vs_ui.EndTitle();
			gC_vs_ui.StartTitle();	
		
			gC_vs_ui.DropItem();

			g_bUIInput = FALSE;

			// 귓속말 대상을 지워준다.
			g_pUserInformation->WhisperID.Release();

			//----------------------------------------------
			// message 제거
			//----------------------------------------------
			g_pSystemMessage->Clear();
			g_pGameMessage->Clear();
			g_pNoticeMessage->Clear();

			//------------------------------------------------------
			// 음악 시작
			//------------------------------------------------------
			if (g_pUserOption->PlayWaveMusic)
			{
//				g_pDXSoundStream->Stop();			
				DEBUG_ADD("MP3 STOP8");
#ifdef __USE_MP3__
				g_pMP3->Stop();
#else
				if( g_DXSound.IsInit() )
					g_pOGG->streamClose();
#endif
				DEBUG_ADD("MP3 STOP8 OK");

				if (g_pUserOption->PlayMusic)
				{
					int musicID = g_pClientConfig->MUSIC_THEME;

					if (musicID!=MUSICID_NULL)
					{
#ifdef __USE_MP3__					
//						g_pDXSoundStream->Load( (*g_pMusicTable)[ musicID ].FilenameWav );
//						g_pDXSoundStream->Play( FALSE );
						DEBUG_ADD("MP3 OPEN2");
						g_pMP3->Open( (*g_pMusicTable)[ musicID ].FilenameWav );
						DEBUG_ADD("MP3 OPEN2 OK");
						DEBUG_ADD("MP3 PLAY4");
						g_pMP3->Play( false );
						DEBUG_ADD("MP3 PLAY4 OK");
#else
						if( g_oggfile != NULL )
							fclose(g_oggfile);
						
						g_oggfile = NULL;

						if( g_DXSound.IsInit() )
						{
							g_oggfile = fopen( (*g_pMusicTable)[ musicID ].FilenameWav ,"rb");
							if( g_oggfile != NULL )
							{
								g_pOGG->streamLoad( g_oggfile, NULL );
								g_pOGG->streamPlay( SOUND_PLAY_ONCE );
								int volume = (g_pUserOption->VolumeMusic - 15) * 250;
								g_pOGG->streamVolume( max( -10000, min( -1, volume ) ) );
							}
						}
#endif
					}
				}
			}
			else
			{
				g_Music.Stop();			

				if (g_pUserOption->PlayMusic)
				{
					int musicID = g_pClientConfig->MUSIC_THEME;

					if (musicID!=MUSICID_NULL)
					{
						g_Music.Play( (*g_pMusicTable)[ musicID ].Filename );
					}
				}
			}
		}
		else
		{
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_LOGOUT_DIED].GetString() );
		}
	}
	else
	{
		DEBUG_ADD("Logout Not ModeGame");
	}
}

//-----------------------------------------------------------------------------
// Play TitleMusic
//-----------------------------------------------------------------------------
void
PlayTitleMusic()
{
	//----------------------------------------------------------------
	// WAV 출력
	//----------------------------------------------------------------
	if (g_pUserOption->PlayWaveMusic)
	{
		g_Music.Stop();

		if (g_pUserOption->PlayMusic)
		{
			int musicID = g_pClientConfig->MUSIC_THEME;

			if (musicID!=MUSICID_NULL)
			{
#ifdef __USE_MP3__
				LONG volume = g_pUserOption->VolumeMusic*16*257;//*SOUND_DEGREE + SOUND_MIN;
//				g_pDXSoundStream->SetVolumeLimit( volume );
				DEBUG_ADD("MP3 SetVolume2");
				g_pMP3->SetVolume( volume );
				DEBUG_ADD("MP3 SetVolume2 OK");

				DEBUG_ADD("MP3 OPEN3");
				g_pMP3->Open( (*g_pMusicTable)[ musicID ].FilenameWav );
				DEBUG_ADD("MP3 OPEN3 OK");
				DEBUG_ADD("MP3 PLAY5");
				g_pMP3->Play( false );				
				DEBUG_ADD("MP3 PLAY5 OK");
//				g_pDXSoundStream->Load( (*g_pMusicTable)[ musicID ].FilenameWav );
//				g_pDXSoundStream->Play( FALSE );				
#else
				if( g_oggfile != NULL )
					fclose(g_oggfile);

				g_oggfile = NULL;

				if( g_DXSound.IsInit() )
				{
					g_oggfile = fopen( (*g_pMusicTable)[ musicID ].FilenameWav, "rb") ;
					if( g_oggfile != NULL )
					{					
						int volume = (g_pUserOption->VolumeMusic - 15) * 250;

						g_pOGG->streamVolume( max( -10000, min( -1, volume ) ) );
						g_pOGG->streamLoad( g_oggfile,NULL );
						g_pOGG->streamPlay( SOUND_PLAY_ONCE );					
					}
				}
#endif
			}									
		}
		else
		{
//			g_pDXSoundStream->Stop();
			DEBUG_ADD("MP3 STOP9");
#ifdef __USE_MP3__
			g_pMP3->Stop();
#else
			if( g_DXSound.IsInit() )
				g_pOGG->streamClose();
#endif
			DEBUG_ADD("MP3 STOP9 OK");
		}
	}
	//----------------------------------------------------------------
	// MID 출력
	//----------------------------------------------------------------
	else
	{
//		g_pDXSoundStream->Stop();
		DEBUG_ADD("MP3 STOP10");
#ifdef __USE_MP3__
		g_pMP3->Stop();
#else
		if( g_DXSound.IsInit() )
			g_pOGG->streamClose();
#endif
		DEBUG_ADD("MP3 STOP10 OK");

		if (g_pUserOption->PlayMusic)
		{
			if (g_Music.IsPlay())
			{
				g_Music.Resume();
			}
			else //if (g_Mode==MODE_GAME)
			{	
				WORD volume = g_pUserOption->VolumeMusic*0x1000 + 0x0FFF;

				if (g_Music.IsPause())
				{
					g_Music.SetVolume( volume );					
					g_Music.Resume();
				}
				else
				{
					g_Music.Pause();
					g_Music.SetVolume( volume );
					g_Music.Resume();					

					int musicID = g_pClientConfig->MUSIC_THEME;

					if (musicID!=MUSICID_NULL)
					{
						g_Music.Play( (*g_pMusicTable)[ musicID ].Filename );
					}
				}					
			}				
		}
		else
		{
			if (g_Music.IsPlay())
			{
				g_Music.Pause();
			}
			else
			{
				g_Music.Stop();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// PlayGameMusic
//-----------------------------------------------------------------------------
void
PlayGameMusic()
{
	//----------------------------------------------------------------
	// WAV 출력
	//----------------------------------------------------------------
	if (g_pUserOption->PlayWaveMusic)
	{
		g_Music.Stop();

		if (g_pUserOption->PlayMusic)
		{
#ifdef __USE_MP3__
			LONG volume = g_pUserOption->VolumeMusic*16*257;//*SOUND_DEGREE + SOUND_MIN;
//			g_pDXSoundStream->SetVolumeLimit( volume );
			DEBUG_ADD("MP3 SetVolume3");
			g_pMP3->SetVolume( volume );
			DEBUG_ADD("MP3 SetVolume3 OK");
#else
			if( g_DXSound.IsInit() )
			{
				int volume = (g_pUserOption->VolumeMusic - 15) * 250;

				g_pOGG->streamVolume( max( -10000, min( -1, volume ) ) );
			}
#endif

			PlayMusicCurrentZone();				
		}
		else
		{					
//			g_pDXSoundStream->Stop();				
			DEBUG_ADD("MP3 STOP11");
#ifdef __USE_MP3__
			g_pMP3->Stop();
#else
			if( g_DXSound.IsInit() )
				g_pOGG->streamClose();
#endif
			DEBUG_ADD("MP3 STOP11 OK");
		}
	}
	//----------------------------------------------------------------
	// MID 출력
	//----------------------------------------------------------------
	else
	{
//		g_pDXSoundStream->Stop();
		DEBUG_ADD("MP3 STOP12");
#ifdef __USE_MP3__
		g_pMP3->Stop();
#else
		if( g_DXSound.IsInit() )
			g_pOGG->streamClose();
#endif
		
		DEBUG_ADD("MP3 STOP12 OK");

		if (g_pUserOption->PlayMusic)
		{
			DEBUG_ADD("MP3 STOP12 OK2");
			if (g_Music.IsPlay())
			{
				DEBUG_ADD("MP3 STOP12 RESUME");
				g_Music.Resume();
				DEBUG_ADD("MP3 STOP12 RESUME OK");
			}
			else //if (g_Mode==MODE_GAME)
			{				
				DEBUG_ADD("MP3 STOP12 pp");
				WORD volume = g_pUserOption->VolumeMusic*0x1000 + 0x0FFF;
				DEBUG_ADD("MP3 STOP12 pp1");
				g_Music.SetVolume( volume );
				DEBUG_ADD("MP3 STOP12 pp2");
				
				PlayMusicCurrentZone();
				DEBUG_ADD("MP3 STOP12 PlayMusicCurrentZone OK");
			}				
		}
		else
		{
			if (g_Music.IsPlay())
			{
				g_Music.Pause();
			}
			else
			{
				g_Music.Stop();
			}
		}
	}
	DEBUG_ADD("PlayGameMusic OK");
}

//-----------------------------------------------------------------------------
//
// constructor / destructor
//
//-----------------------------------------------------------------------------
UIMessageManager::UIMessageManager()
{
	for (int i=0; i<MAX_UI_MESSAGE; i++)
	{
		m_UIMessageFunction[i] = NULL;
	}
}

UIMessageManager::~UIMessageManager()
{
	// 단지 테스트용.. - -;
	for (int i=0; i<MAX_UI_MESSAGE; i++)
	{
		m_UIMessageFunction[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
//
// member functions
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
void
UIMessageManager::Init()
{	
	m_UIMessageFunction[UI_STORAGE_BUY] = Execute_UI_STORAGE_BUY;
	m_UIMessageFunction[UI_SELECT_STORAGE_SLOT] = Execute_UI_SELECT_STORAGE_SLOT;
	m_UIMessageFunction[UI_RUNNING_GAMEMENU] = Execute_UI_RUNNING_GAMEMENU;
	m_UIMessageFunction[UI_FINISH_LEVELUP_BUTTON] = Execute_UI_FINISH_LEVELUP_BUTTON;
	m_UIMessageFunction[UI_CLICK_BONUS_POINT] = Execute_UI_CLICK_BONUS_POINT;
	m_UIMessageFunction[UI_INFO] = Execute_UI_INFO;
//	m_UIMessageFunction[UI_MINIMAP_TOGGLE] = Execute_UI_MINIMAP_TOGGLE;
	m_UIMessageFunction[UI_DROP_MONEY] = Execute_UI_DROP_MONEY;
	m_UIMessageFunction[UI_EXCHANGE_REQUEST_CANCEL] = Execute_UI_EXCHANGE_REQUEST_CANCEL;
	m_UIMessageFunction[UI_EXCHANGE_ACCEPT] = Execute_UI_EXCHANGE_ACCEPT;
	m_UIMessageFunction[UI_EXCHANGE_MONEY] = Execute_UI_EXCHANGE_MONEY;
	m_UIMessageFunction[UI_WITHDRAW_MONEY] = Execute_UI_WITHDRAW_MONEY;
	m_UIMessageFunction[UI_ITEM_SELECT_EXCHANGE] = Execute_UI_ITEM_SELECT_EXCHANGE;
	m_UIMessageFunction[UI_DEPOSIT_MONEY] = Execute_UI_DEPOSIT_MONEY;
	m_UIMessageFunction[UI_CHECK_EXIST_ID] = Execute_UI_CHECK_EXIST_ID;
	m_UIMessageFunction[UI_RUN_NEWUSER_REGISTRATION] = Execute_UI_RUN_NEWUSER_REGISTRATION;
	m_UIMessageFunction[UI_INFO_CLOSE] = Execute_UI_INFO_CLOSE;
	m_UIMessageFunction[UI_GAMEMENU_CONTINUE] = Execute_UI_GAMEMENU_CONTINUE;

	//
	// PDS
	//
//	m_UIMessageFunction[UI_CHANGE_PCS_CONNECTED_SLOT] = Execute_UI_CHANGE_PCS_CONNECTED_SLOT;		// 이미 연결된 다른 사람을 click하였다.
//	m_UIMessageFunction[UI_PLEASE_PCS_CONNECT_ME] = Execute_UI_PLEASE_PCS_CONNECT_ME;			// 사람이 있는 wait room을 click하였다.
	//m_UIMessageFunction[UI_PCS_CONNECTOR_GRANTED] = Execute_UI_PCS_CONNECTOR_GRANTED;			// wait room에 있는 connector와 연결을 승낙하였다.
//	m_UIMessageFunction[UI_QUIT_PCS_ONLINE_MODE] = Execute_UI_QUIT_PCS_ONLINE_MODE;			// PCS online mode에서 quit 버튼을 눌렀다.
//	m_UIMessageFunction[UI_END_PCS] = Execute_UI_END_PCS;								// sending 하고 있을 때, end를 눌렀다.
//	m_UIMessageFunction[UI_SEND_PCS_NUMBER] = Execute_UI_SEND_PCS_NUMBER;
//	m_UIMessageFunction[UI_PDS_CLOSED] = Execute_UI_PDS_CLOSED;							// slayer pds가 닫혔다. !현재 연결은 유지한다.

//	m_UIMessageFunction[UI_PLEASE_SET_SLAYER_VALUE] = Execute_UI_PLEASE_SET_SLAYER_VALUE;		// slayer pds에서 gage menu를 실행하였다.

//	m_UIMessageFunction[UI_LEARN_SLAYER_SKILL] = Execute_UI_LEARN_SLAYER_SKILL;				// skill tree에서 배울 수 있는 skill을 선택하였다.
//	m_UIMessageFunction[UI_CLOSE_SKILL_VIEW] = Execute_UI_CLOSE_SKILL_VIEW;

	m_UIMessageFunction[UI_BACKGROUND_MOUSE_FOCUS] = Execute_UI_BACKGROUND_MOUSE_FOCUS;			// Mouse pointer가 UI Window에서 이동없이 일을 때, 그 Window가 사라질 경우 Client에 전달.
	m_UIMessageFunction[UI_REMOVE_BACKGROUND_MOUSE_FOCUS] = Execute_UI_REMOVE_BACKGROUND_MOUSE_FOCUS;// Game Menu와 같은 topmost Window가 출력되었을 때는 background focus를 제거하는 것이 좋다.

	//
	// 상점 Message
	//
	m_UIMessageFunction[UI_BUY_ITEM] = Execute_UI_BUY_ITEM;

	m_UIMessageFunction[UI_SELL_ITEM] = Execute_UI_SELL_ITEM;
	m_UIMessageFunction[UI_SELL_ALL_ITEM] = Execute_UI_SELL_ALL_ITEM;
	m_UIMessageFunction[UI_REPAIR_ITEM] = Execute_UI_REPAIR_ITEM;
	m_UIMessageFunction[UI_SILVERING_ITEM] = Execute_UI_SILVERING_ITEM;
	m_UIMessageFunction[UI_ITEM_SELL_FINISHED] = Execute_UI_ITEM_SELL_FINISHED;
	m_UIMessageFunction[UI_ITEM_REPAIR_FINISHED] = Execute_UI_ITEM_REPAIR_FINISHED;
	m_UIMessageFunction[UI_ITEM_SILVERING_FINISHED] = Execute_UI_ITEM_SILVERING_FINISHED;

	m_UIMessageFunction[UI_CLOSE_SHOP] = Execute_UI_CLOSE_SHOP;
	m_UIMessageFunction[UI_CLOSE_STORAGE] = Execute_UI_CLOSE_STORAGE;
	m_UIMessageFunction[UI_CLOSE_EXCHANGE] = Execute_UI_CLOSE_EXCHANGE;
	m_UIMessageFunction[UI_CANCEL_EXCHANGE] = Execute_UI_CANCEL_EXCHANGE;
	m_UIMessageFunction[UI_OK_EXCHANGE] = Execute_UI_OK_EXCHANGE;

	m_UIMessageFunction[UI_CHARACTER_MANAGER_FINISHED] = Execute_UI_CHARACTER_MANAGER_FINISHED;	// Characters Window에서 초기화면으로 갈 때 알려준다.
	m_UIMessageFunction[UI_TERMINATION] = Execute_UI_TERMINATION;						// 종료
	m_UIMessageFunction[UI_LOGIN] = Execute_UI_LOGIN;								// 사용자 login 요청
	m_UIMessageFunction[UI_LOGOUT] = Execute_UI_LOGOUT;								
	//m_UIMessageFunction[UI_NEW_USER_REGISTRATION] = Execute_UI_NEW_USER_REGISTRATION;			// 새 사용자 등록 요청
	m_UIMessageFunction[UI_CONNECT] = Execute_UI_CONNECT;							   // Game connection.
	m_UIMessageFunction[UI_CHAT_RETURN] = Execute_UI_CHAT_RETURN;						// 채팅창 입력.
//	m_UIMessageFunction[UI_CHAT_SELECT_NAME] = Execute_UI_CHAT_SELECT_NAME;						// 채팅창 이름 선택.
	m_UIMessageFunction[UI_SELECT_SKILL] = Execute_UI_SELECT_SKILL;						// Skill selection.
	m_UIMessageFunction[UI_CANCEL_SELECT_SKILL] = Execute_UI_CANCEL_SELECT_SKILL;				// Skill 선택이 취소되었다.

	//
	// Item message
	//

	// drop
	m_UIMessageFunction[UI_ITEM_DROP_TO_CLIENT] = Execute_UI_ITEM_DROP_TO_CLIENT;				// Item을 바닥에 떨어뜨렸다.
	m_UIMessageFunction[UI_ITEM_DROP_TO_INVENTORY] = Execute_UI_ITEM_DROP_TO_INVENTORY; 		// Inventory에 놓는다.
	//m_UIMessageFunction[UI_ITEM_DROP_TO_TRADEGRID] = Execute_UI_ITEM_DROP_TO_TRADEGRID;			// 교환창 그리드에 놓는다.
	m_UIMessageFunction[UI_ITEM_DROP_TO_GEAR] = Execute_UI_ITEM_DROP_TO_GEAR;				// Gear에 놓는다.
	m_UIMessageFunction[UI_ITEM_DROP_TO_QUICKSLOT] = Execute_UI_ITEM_DROP_TO_QUICKSLOT;			// Quick Slot에 놓는다.

	// pick up
	// Client에서 집을 때는 Client -> UI 이다.
	//m_UIMessageFunction[UI_ITEM_PICKUP_FROM_TRADEGRID] = Execute_UI_ITEM_PICKUP_FROM_TRADEGRID;
	m_UIMessageFunction[UI_ITEM_PICKUP_FROM_INVENTORY] = Execute_UI_ITEM_PICKUP_FROM_INVENTORY;
	m_UIMessageFunction[UI_ITEM_PICKUP_FROM_GEAR] = Execute_UI_ITEM_PICKUP_FROM_GEAR;
	m_UIMessageFunction[UI_ITEM_PICKUP_FROM_QUICKSLOT] = Execute_UI_ITEM_PICKUP_FROM_QUICKSLOT;

	// insert to other Item
	//m_UIMessageFunction[UI_ITEM_INSERT_FROM_TRADEGRID] = Execute_UI_ITEM_INSERT_FROM_TRADEGRID;
	m_UIMessageFunction[UI_ITEM_INSERT_FROM_INVENTORY] = Execute_UI_ITEM_INSERT_FROM_INVENTORY;
	m_UIMessageFunction[UI_ITEM_INSERT_FROM_GEAR] = Execute_UI_ITEM_INSERT_FROM_GEAR;
	m_UIMessageFunction[UI_ITEM_INSERT_FROM_QUICKSLOT] = Execute_UI_ITEM_INSERT_FROM_QUICKSLOT;  // 쌓이는 경우 - 아직 지원안함 -

	// use
	m_UIMessageFunction[UI_ITEM_USE] = Execute_UI_ITEM_USE;							// Inventory에서 사용.
	m_UIMessageFunction[UI_ITEM_USE_QUICKSLOT] = Execute_UI_ITEM_USE_QUICKSLOT;				// Quick Item Slot에서 사용.

	//
	// Character management
	//
	m_UIMessageFunction[UI_NEW_CHARACTER] = Execute_UI_NEW_CHARACTER;
	m_UIMessageFunction[UI_DELETE_CHARACTER] = Execute_UI_DELETE_CHARACTER;

	//m_UIMessageFunction[UI_CLOSE_BBS] = Execute_UI_CLOSE_BBS;

//	m_UIMessageFunction[UI_CHANGE_GAME_OPTION] = Execute_UI_CHANGE_GAME_OPTION;
//	m_UIMessageFunction[UI_CLOSE_GAME_OPTION] = Execute_UI_CLOSE_GAME_OPTION;
	m_UIMessageFunction[UI_CHANGE_OPTION] = Execute_UI_CHANGE_OPTION;
	m_UIMessageFunction[UI_CLOSE_OPTION] = Execute_UI_CLOSE_OPTION;
	m_UIMessageFunction[UI_CLOSE_BOOKCASE] = Execute_UI_CLOSE_BOOKCASE;
	m_UIMessageFunction[UI_CLOSE_BRIEFING] = Execute_UI_CLOSE_BRIEFING;
	m_UIMessageFunction[UI_CLOSE_COMPUTER] = Execute_UI_CLOSE_COMPUTER;
	m_UIMessageFunction[UI_CLOSE_TUTORIAL_EXIT] = Execute_UI_CLOSE_TUTORIAL_EXIT;

	m_UIMessageFunction[UI_CLOSE_DESC_DIALOG] = Execute_UI_CLOSE_DESC_DIALOG;
	m_UIMessageFunction[UI_CLOSE_ELEVATOR] = Execute_UI_CLOSE_ELEVATOR;
	m_UIMessageFunction[UI_SELECT_ELEVATOR] = Execute_UI_SELECT_ELEVATOR;

	//m_UIMessageFunction[UI_SELECT_SERVER] = Execute_UI_SELECT_SERVER;

	//m_UIMessageFunction[UI_REQUEST_SERVER_LIST] = Execute_UI_REQUEST_SERVER_LIST;

	m_UIMessageFunction[UI_ITEM_TO_QUICKITEMSLOT] = Execute_UI_ITEM_TO_QUICKITEMSLOT;

	m_UIMessageFunction[UI_CLOSE_SLAYER_PORTAL] = Execute_UI_CLOSE_SLAYER_PORTAL;
	m_UIMessageFunction[UI_SLAYER_PORTAL] = Execute_UI_SLAYER_PORTAL;

	m_UIMessageFunction[UI_FINISH_REQUEST_PARTY_BUTTON] = Execute_UI_FINISH_REQUEST_PARTY_BUTTON;
	m_UIMessageFunction[UI_PARTY_REQUEST_CANCEL] = Execute_UI_PARTY_REQUEST_CANCEL;
	m_UIMessageFunction[UI_PARTY_ACCEPT] = Execute_UI_PARTY_ACCEPT;

	m_UIMessageFunction[UI_CLOSE_PARTY_MANAGER] = Execute_UI_CLOSE_PARTY_MANAGER;
	m_UIMessageFunction[UI_AWAY_PARTY] = Execute_UI_AWAY_PARTY;

	m_UIMessageFunction[UI_FINISH_REQUEST_DIE_BUTTON] = Execute_UI_FINISH_REQUEST_DIE_BUTTON;

	m_UIMessageFunction[UI_CLOSE_TEAM_LIST] = Execute_UI_CLOSE_TEAM_LIST;
	m_UIMessageFunction[UI_CLOSE_TEAM_INFO] = Execute_UI_CLOSE_TEAM_INFO;
	m_UIMessageFunction[UI_CLOSE_TEAM_MEMBER_INFO] = Execute_UI_CLOSE_TEAM_MEMBER_INFO;
	m_UIMessageFunction[UI_CLOSE_TEAM_REGIST] = Execute_UI_CLOSE_TEAM_REGIST;
	m_UIMessageFunction[UI_CLOSE_TEAM_MEMBER_LIST] = Execute_UI_CLOSE_TEAM_MEMBER_LIST;
	
	m_UIMessageFunction[UI_REQUEST_GUILD_INFO] = Execute_UI_REQUEST_GUILD_INFO;
	m_UIMessageFunction[UI_REQUEST_GUILD_MEMBER_LIST] = Execute_UI_REQUEST_GUILD_MEMBER_LIST;
	m_UIMessageFunction[UI_MODIFY_GUILD_MEMBER] = Execute_UI_MODIFY_GUILD_MEMBER;

	m_UIMessageFunction[UI_SELECT_TEAM_MEMBER_LIST] = Execute_UI_SELECT_TEAM_MEMBER_LIST;		// void_ptr = TEAM_NAME
	m_UIMessageFunction[UI_SELECT_READY_TEAM_LIST] = Execute_UI_SELECT_READY_TEAM_LIST;		// void_ptr = TEAM_NAME
	m_UIMessageFunction[UI_SELECT_REGIST_TEAM_LIST] = Execute_UI_SELECT_REGIST_TEAM_LIST;		// void_ptr = TEAM_NAME

	m_UIMessageFunction[UI_JOIN_READY_TEAM] = Execute_UI_JOIN_READY_TEAM;				// void_ptr = TEAM_NAME
	m_UIMessageFunction[UI_JOIN_REGIST_TEAM] = Execute_UI_JOIN_REGIST_TEAM;			// void_ptr = TEAM_NAME

	m_UIMessageFunction[UI_REGIST_GUILD_MEMBER] = Execute_UI_REGIST_GUILD_MEMBER;			// void_ptr = introduction max:150byte 창 닫아줄것!
	m_UIMessageFunction[UI_REGIST_GUILD_TEAM] = Execute_UI_REGIST_GUILD_TEAM;			// left = TEAM_NAME, void_ptr = introduction max:150byte 창 닫아줄것!

	m_UIMessageFunction[UI_CLOSE_FILE_DIALOG] = Execute_UI_CLOSE_FILE_DIALOG;


	m_UIMessageFunction[UI_CONNECT_SERVER] = Execute_UI_CONNECT_SERVER;
	m_UIMessageFunction[UI_CLOSE_SERVER_SELECT] = Execute_UI_CLOSE_SERVER_SELECT;

	m_UIMessageFunction[UI_NEWCHARACTER_CHECK] = Execute_UI_NEWCHARACTER_CHECK;
	
//	m_UIMessageFunction[UI_SELECT_EXPLOSIVE] = Execute_UI_SELECT_EXPLOSIVE;
//	m_UIMessageFunction[UI_CLOSE_SELECT_EXPLOSIVE] = Execute_UI_CLOSE_SELECT_EXPLOSIVE;

	m_UIMessageFunction[UI_ENCHANT_ACCEPT] = Execute_UI_ENCHANT_ACCEPT;
	m_UIMessageFunction[UI_ENCHANT_CANCEL] = Execute_UI_ENCHANT_CANCEL;

	m_UIMessageFunction[UI_MESSAGE_BOX] = Execute_UI_MESSAGE_BOX;

	m_UIMessageFunction[UI_CLOSE_OTHER_INFO] = Execute_UI_CLOSE_OTHER_INFO;

	m_UIMessageFunction[UI_MODIFY_TEAM_INFO] = Execute_UI_MODIFY_TEAM_INFO;
	m_UIMessageFunction[UI_MODIFY_GUILD_MEMBER_INTRO] = Execute_UI_MODIFY_GUILD_MEMBER_INTRO;

	m_UIMessageFunction[UI_SEND_NAME_FOR_SOUL_CHAIN] = Execute_UI_SEND_NAME_FOR_SOUL_CHAIN;
	m_UIMessageFunction[UI_CLOSE_TRACE_WINDOW] = Execute_UI_CLOSE_TRACE_WINDOW;

	m_UIMessageFunction[UI_RUN_CONNECT] = Execute_UI_RUN_CONNECT;

	m_UIMessageFunction[UI_SELECT_GRADE_SKILL] = Excute_UI_SELECT_GRADE_SKILL;

	m_UIMessageFunction[UI_USE_XMAS_TREE] = Excute_UI_USE_XMAS_TREE;
	m_UIMessageFunction[UI_CLOSE_XMAS_CARD_WINDOW] = Excute_UI_CLOSE_XMAS_CARD_WINDOW;

	m_UIMessageFunction[UI_SEND_BRING_FEE] = Excute_UI_SEND_BRING_FEE;
	m_UIMessageFunction[UI_CLOSE_BRING_FEE_WINDOW] = Excute_UI_CLOSE_BRING_FEE_WINDOW;

	m_UIMessageFunction[UI_CLOSE_WAR_LIST_WINDOW] = Excute_UI_CLOSE_WAR_LIST;

	m_UIMessageFunction[UI_CLOSE_BLOOD_BIBLE_STATUS] = Execute_UI_CLOSE_BLOOD_BIBLE_STATUS;
	m_UIMessageFunction[UI_SEND_NAME_FOR_COUPLE] = Execute_UI_SEND_NAME_FOR_COUPLE;
	m_UIMessageFunction[UI_CLOSE_INPUT_NAME] = Execute_UI_CLOSE_INPUT_NAME_WINDOW;

	m_UIMessageFunction[UI_ITEM_USE_GEAR] = Execute_UI_ITEM_USE_GEAR;
	m_UIMessageFunction[UI_GO_BILING_PAGE] = Execute_GO_BILING_PAGE;
	m_UIMessageFunction[UI_CLOSE_POPUP_MESSAGE] = Execute_UI_CLOSE_POPUP_MESSAGE;
	m_UIMessageFunction[UI_CLOSE_QUEST_STATUS] = Execute_UI_CLOSE_QUEST_STATUS;

	m_UIMessageFunction[UI_CLOSE_LOTTERY_CARD] = Execute_UI_CLOSE_LOTTERY_CARD;
	m_UIMessageFunction[UI_LOTTERY_CARD_STATUS] = Execute_UI_LOTTERY_CARD_STATUS;
	m_UIMessageFunction[UI_FINISH_SCRATCH_LOTTERY]	= Execute_UI_FINISH_SCRATCH_LOTTERY;
	m_UIMessageFunction[UI_CLOSE_IMAGE_NOTICE] = Execute_UI_CLOSE_IMAGE_NOTICE;

	m_UIMessageFunction[UI_CLOSE_ITEM_LIST]	= Execute_UI_CLOSE_ITEM_LIST;
	m_UIMessageFunction[UI_SELECT_ITEM_FROM_SHOP] = Execute_UI_SELECT_ITEM_FROM_SHOP;
	m_UIMessageFunction[UI_CLOSE_BULLETIN_BOARD_WINDOW] = Execute_UI_CLOSE_BULLETIN_BOARD;

	m_UIMessageFunction[UI_TRANS_ITEM_CANCEL] = Execute_UI_TRANS_ITEM_CANCEL;
	m_UIMessageFunction[UI_TRANS_ITEM_ACCEPT] = Execute_UI_TRANS_ITEM_ACCEPT;
	m_UIMessageFunction[UI_FINISH_REQUEST_RESURRECT_BUTTON] = Execute_UI_REQUEST_RESURRECT;

	m_UIMessageFunction[UI_CLOSE_MIXING_FORGE] = Execute_UI_CLOSE_MIXING_FORGE;
	m_UIMessageFunction[UI_USE_MIXING_FORGE] = Execute_UI_MIXING_FORGE;

	m_UIMessageFunction[UI_CLOSE_REMOVE_OPTION] = Execute_UI_CLOSE_REMOVE_OPTION;
	m_UIMessageFunction[UI_SEND_REMOVE_OPTION] = Execute_UI_SEND_REMOVE_OPTION;

	m_UIMessageFunction[UI_CLOSE_OUSTERS_SKILL_INFO] = Execute_UI_CLOSE_OUSTERS_SKILL_INFO;
	m_UIMessageFunction[UI_OUSTERS_LEARN_SKILL] = Execute_UI_LEARN_OUSTERS_SKILL;

	m_UIMessageFunction[UI_RUN_LEVELUP] = Execute_UI_RUN_LEVELUP;
	m_UIMessageFunction[UI_CLOSE_HORN] = Execute_UI_CLOSE_HORN;
	m_UIMessageFunction[UI_OUSTERS_DOWN_SKILL] = Execute_UI_OUSTERS_DOWN_SKILL;	
	
	m_UIMessageFunction[UI_CLEAR_ALL_STAGE] = Execute_UI_CLEAR_ALL_STAGE;
	m_UIMessageFunction[UI_CLOSE_FINDING_MINE] = Execute_UI_CLOSE_FINDING_MINE;
	m_UIMessageFunction[UI_CLOSE_NEMONEMO] = Execute_UI_CLOSE_NEMONEMO;
	m_UIMessageFunction[UI_CLOSE_PUSHPUSH] = Execute_UI_CLOSE_PUSHPUSH;
	m_UIMessageFunction[UI_CLOSE_CRAZY_MINE] = Execute_UI_CLOSE_CRAZY_MINE;
	m_UIMessageFunction[UI_CLOSE_ARROW_TILE] = Execute_UI_CLOSE_ARROW_TILE;
	m_UIMessageFunction[UI_CLEAR_STAGE] = Execute_UI_CLEAR_STAGE;
	m_UIMessageFunction[UI_REQUEST_FORCE_DIE] = Execute_UI_FORCE_DIE;
	m_UIMessageFunction[UI_ADD_ITEM_TO_CODE_SHEET] = Execute_UI_ADD_ITEM_TO_CODE_SHEET;
	m_UIMessageFunction[UI_SEND_BUG_REPORT] = Execute_UI_SEND_BUG_REPORT;
	m_UIMessageFunction[UI_GO_TO_BEGINNER_ZONE] = Execute_UI_GO_BEGINNER_ZONE;
	m_UIMessageFunction[UI_POPUP_MESSAGE_OK] = Execute_UI_POPUP_MESSAGE_OK;

	m_UIMessageFunction[UI_CLOSE_SHRINE_MINIMAP] = Execute_UI_CLOSE_SHRINE_MINIMAP;
	m_UIMessageFunction[UI_WARP_REGEN_TOWER] = Execute_UI_WARP_TO_REGEN_TOWER;
	m_UIMessageFunction[UI_CLOSE_MAILBOX] = Execute_UI_CLOSE_MAILBOX;
	m_UIMessageFunction[UI_CLOSE_PET_INFO] = Execute_UI_CLOSE_PET_INFO;
	m_UIMessageFunction[UI_PET_GAMBLE] = Execute_UI_PET_GAMBLE;
	m_UIMessageFunction[UI_CLOSE_USE_PET_FOOD] = Execute_UI_CLOSE_USE_PET_FOOD;

	// 2004, 5, 11 sobeit add start - 펫 보관소
	m_UIMessageFunction[UI_CLOSE_PETSTORAGE] = Execute_UI_CLOSE_PETSTORAGE;
	m_UIMessageFunction[UI_CLOSE_KEEP_PETITEM] = Execute_UI_CLOSE_KEEP_PETITEM;
	m_UIMessageFunction[UI_CLOSE_GET_KEEP_PETITEM] = Execute_UI_CLOSE_GET_KEEP_PETITEM;
	m_UIMessageFunction[UI_KEEP_PETITEM] = Execute_UI_KEEP_PETITEM;
	m_UIMessageFunction[UI_GET_KEEP_PETITEM] = Execute_UI_GET_KEEP_PETITEM;
	// 2004, 5, 11 sobeit add end - 펫 보관소

	// 2004, 5, 28 sobeit add start - sms 관련
	m_UIMessageFunction[UI_CLOSE_SMS_MESSAGE] = Execute_UI_CLOSE_SMS_MESSAGE;
	m_UIMessageFunction[UI_SEND_SMS_MESSAGE] = Execute_UI_SEND_SMS_MESSAGE;
	m_UIMessageFunction[UI_SMS_OPEN_LIST] = Execute_UI_SMS_OPEN_LIST;
	m_UIMessageFunction[UI_SMS_RECORD] = Execute_UI_SMS_RECORD;
	m_UIMessageFunction[UI_SMS_DELETE] = Execute_UI_SMS_DELETE;
	m_UIMessageFunction[UI_CLOSE_SMS_LIST] = Execute_UI_CLOSE_SMS_LIST;
	m_UIMessageFunction[UI_CLOSE_SMS_RECORD] = Execute_UI_CLOSE_SMS_RECORD;
	m_UIMessageFunction[UI_SMS_ADD_SEND_LIST] = Execute_UI_SMS_ADD_SEND_LIST;
	// 2004, 5, 28 sobeit add end

	// 2004, 6, 15 sobeit add start - nameing 관련
	m_UIMessageFunction[UI_CLOSE_NAMING] = Execute_UI_CLOSE_NAMING;
	m_UIMessageFunction[UI_CHANGE_CUSTOM_NAMING] = Execute_UI_CHANGE_CUSTOM_NAMING;
	m_UIMessageFunction[UI_SELECT_NAMING] = Execute_UI_SELECT_NAMING;
	m_UIMessageFunction[UI_CLOSE_NAMING_CHANGE] = Execute_UI_CLOSE_NAMING_CHANGE;
	m_UIMessageFunction[UI_RUN_NAMING_CHANGE] = Execute_UI_RUN_NAMING_CHANGE;
	// 2004, 6, 15 sobeit add end

	// 2004, 7, 12 sobeit add start - quest 관련
	m_UIMessageFunction[UI_CLOSE_QUEST_MANAGER] = Execute_UI_CLOSE_QUEST_MANAGER;
	m_UIMessageFunction[UI_CLOSE_QUEST_LIST] = Execute_UI_CLOSE_QUEST_LIST;
	m_UIMessageFunction[UI_CLOSE_QUEST_DETAIL] = Execute_UI_CLOSE_QUEST_DETAIL;
	m_UIMessageFunction[UI_CLOSE_QUEST_MISSION] = Execute_UI_CLOSE_QUEST_MISSION;
	m_UIMessageFunction[UI_CLOSE_QUEST_ITEM] = Execute_UI_CLOSE_QUEST_ITEM;
	m_UIMessageFunction[UI_CLOSE_QUEST_ICON] = Execute_UI_CLOSE_QUEST_ICON;
	m_UIMessageFunction[UI_GQUEST_ACCEPT] = Execute_UI_GQUEST_ACCEPT;
	m_UIMessageFunction[UI_GQUEST_GIVEUP] = Execute_UI_GQUEST_GIVEUP;

	// 2004, 7, 12 sobeit add end - quest 관련

	m_UIMessageFunction[UI_ITEM_USE_GQUEST_ITEM] = Execute_UI_ITEM_USE_GQUEST_ITEM;

	// 2004,10.9 csm guild 관련 
	m_UIMessageFunction[UI_REQUEST_UNION] =	 Execute_UI_ITEM_USE_REQUEST_UNION;
	m_UIMessageFunction[UI_REQUEST_UNION_QUIT] =	 Execute_UI_ITEM_USE_QUIT;
	m_UIMessageFunction[UI_REQUEST_UNION_EXPERGUILD] =	 Execute_UI_ITEM_USE_EXPER;
	m_UIMessageFunction[UI_REQUEST_UNION_REQUEST_INFO] =   Execute_UI_ITEM_USE_UNION_INFO;
	m_UIMessageFunction[UI_REQUEST_UNION_REQUEST_GUILD_LIST] =		Execute_UI_ITEM_USE_REQUEST_GUILD_LIST;
	m_UIMessageFunction[UI_REQUEST_UNION_ACCEPT] =		Execute_UI_ITEM_USE_UNION_ACCEPT;
	m_UIMessageFunction[UI_REQUEST_UNION_DENY] =	Execute_UI_ITEM_USE_UNION_DENY;
	m_UIMessageFunction[UI_REQUEST_UNION_QUIT_ACCEPT] =		Execute_UI_ITEM_USE_UNION_QUIT_ACCEPT;
	m_UIMessageFunction[UI_REQUEST_UNION_QUIT_DENY] =	Execute_UI_ITEM_USE_UNION_QUIT_DENY;

	m_UIMessageFunction[UI_RECALL_BY_NAME] =	 Execute_UI_RECALL_BY_NAME;

	m_UIMessageFunction[UI_MODIFY_TAX] =	 Execute_UI_UI_MODIFY_TAX;

	m_UIMessageFunction[UI_APPOINT_SUBMASTER] =	 Execute_UI_APPOINT_SUBMASTER;


	m_UIMessageFunction[UI_DISPLAY_ITEM] =	             Execute_UI_DISPLAY_ITEM;        
    m_UIMessageFunction[UI_UNDISPLAY_ITEM] =	         Execute_UI_UNDISPLAY_ITEM;
    m_UIMessageFunction[UI_STORE_SIGN] =	             Execute_UI_STORE_SIGN;
    m_UIMessageFunction[UI_STORE_OPEN] =				 Execute_UI_STORE_OPEN;
    m_UIMessageFunction[UI_STORE_CLOSE] =				 Execute_UI_STORE_CLOSE;
	m_UIMessageFunction[UI_REQUEST_STORE_INFO] =		 Execute_UI_REQUEST_STORE_INFO;
	m_UIMessageFunction[UI_MY_STORE_INFO] =				 Execute_UI_MY_STORE_INFO;
    m_UIMessageFunction[UI_OTHER_STORE_INFO] =			Execute_UI_OTHER_STORE_INFO;
    m_UIMessageFunction[UI_BUY_STORE_ITEM] =			Execute_UI_BUY_STORE_ITEM;
    m_UIMessageFunction[UI_REMOVE_STORE_ITEM] =			Execute_UI_REMOVE_STORE_ITEM;
    m_UIMessageFunction[UI_ADD_STORE_ITEM] =			Execute_UI_ADD_STORE_ITEM;
	m_UIMessageFunction[UI_CLOSE_PERSNALSHOP] =			Execute_UI_CLOSE_PERSNALSHOP;
	
	m_UIMessageFunction[UI_CLOSE_POWER_JJANG]			=	 Execute_UI_CLOSE_POWER_JJANG;
	m_UIMessageFunction[UI_REQUEST_POWER_JJANG_POINT]	=	 Execute_UI_REQUEST_POWER_JJANG_POINT;
	m_UIMessageFunction[UI_POWER_JJANG_GAMBLE]			=	 Execute_UI_POWER_JJANG_GAMBLE;

	m_UIMessageFunction[UI_CLOSE_SWAPADVANCEMENTITEM]			=	 Execute_UI_CLOSE_SWAPADVANCEMENTITEM;
	m_UIMessageFunction[UI_SWAPADVANCEMENTITEM]			=	 Execute_UI_SWAPADVANCEMENTITEM;

	m_UIMessageFunction[UI_LEARN_ADVANCE_SKILL]			=	 Execute_UI_LEARN_ADVANCE_SKILL;
	
	
	m_UIMessageFunction[UI_CAMPAIGN_HELP] =	 Execute_UI_CAMPAIGN_HELP;

	m_UIMessageFunction[UI_RUN_NEXT_GQUEST_EXCUTE_ELEMENT] =	 Execute_UI_RUN_NEXT_GQUEST_EXCUTE_ELEMENT;

	m_UIMessageFunction[UI_GQUEST_SET_ACTION] =	 Execute_UI_GQUEST_SET_ACTION;
	m_UIMessageFunction[UI_GQUEST_ENDING_EVENT] =	 Execute_UI_GQUEST_ENDING_EVENT;

	m_UIMessageFunction[UI_REQUEST_EVENT_ITEM] =	 Execute_UI_REQUEST_EVENT_ITEM;

	m_UIMessageFunction[UI_CLOSE_INVENTORY_SUB]			=	 Execute_UI_CLOSE_INVENTORY_SUB;
	m_UIMessageFunction[UI_ITEM_DROP_TO_INVENTORY_SUB]		=	 Execute_UI_ITEM_DROP_TO_INVENTORY_SUB;
	m_UIMessageFunction[UI_ITEM_PICKUP_FROM_INVENTORY_SUB]	=	 Execute_UI_ITEM_PICKUP_FROM_INVENTORY_SUB;

	m_UIMessageFunction[UI_ITEM_USE_SUBINVENTORY]	=	 Execute_UI_ITEM_USE_SUBINVENTORY;
	
	m_UIMessageFunction[UI_WEDDING_CONTRIBUTION]	=	 Execute_UI_WEDDING_CONTRIBUTION;


	m_UIMessageFunction[UI_NETMARBLE_AGREEMENT] = Execute_UI_NETMARBLE_AGREEMENT;

	m_UIMessageFunction[UI_BLOOD_BURST] = Execute_UI_BLOOD_BURST;
	m_UIMessageFunction[UI_CLIENT_REMOVE_EFFECT_STATUS] = Execute_UI_CLIENT_REMOVE_EFFECT_STATUS;
	m_UIMessageFunction[UI_MARKET_ACCOUNT] = Execute_UI_MARKET_ACCOUNT;
}

//-----------------------------------------------------------------------------
// Execute
//-----------------------------------------------------------------------------
// message와 연결되어 있는 적절한 처리 함수를 호출한다.
//-----------------------------------------------------------------------------
void			
UIMessageManager::Execute(DWORD message, int left, int right, void* void_ptr)
{
	// 기본적으로 MODE_GAME이면
	// NULL이 아니어야 하는 값들이 있다..
	if (g_Mode==MODE_GAME
		&& (g_pZone==NULL
			|| g_pPlayer==NULL
			|| g_pInventory==NULL
			|| g_pPlayer->IsSlayer() && g_pSlayerGear==NULL
			|| g_pPlayer->IsVampire() && g_pVampireGear==NULL
			|| g_pPlayer->IsOusters() && g_pOustersGear==NULL
			|| g_pTempInformation==NULL
			// 2004, 12, 17, sobeit add start
			|| message >= MAX_UI_MESSAGE
			// 2004, 12, 17, sobeit add end
			)
		)
	{
		DEBUG_ADD("[Error] UI message. game object pointer NULL");

		return;
	}
		
	if (m_UIMessageFunction[message]==NULL)
	{
		DEBUG_ADD_FORMAT("[Error] NO UI_MessageFunction. msg=%d", message);
	}
	else
	{
		(*m_UIMessageFunction[message])(left, right, void_ptr);

		DEBUG_ADD("[UI] Execute OK");
	}
}


//-----------------------------------------------------------------------------
//
// 새 캐릭터 만들기
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_NEW_CHARACTER(int left, int right, void* void_ptr)
{
	DEBUG_ADD("UI_NEW_CHARACTER");
	if (g_Mode!=MODE_WAIT_SELECTPC)
	{
		DEBUG_ADD("Not Mode MODE_WAIT_SELECTPC");
		return;
	}

	//
	// ((C_VS_UI::NEW_CHARACTER *)void_ptr) = 
	//	char *	sz_name;
	//	bool		bl_female;
	//	int		STR;
	//	int		DEX;
	//	int		INT;
	//	int		slot;
	//	
	//	int		face;
	//	int		skin_color;
	//	int		hair_color;
	//	int		coat_color;
	//	int		trouser_color;
	//
	NEW_CHARACTER* pChar = (NEW_CHARACTER *)void_ptr;
	
	BOOL bAllOK = TRUE;

	//---------------------------------------------
	// 이름 길이 체크
	//---------------------------------------------
	int len = strlen(pChar->sz_name);	
	
	if (len<PlayerInfo::minIDLength || len>PlayerInfo::maxIDLength)
	{
		char strTemp[128];
		sprintf(strTemp, (*g_pGameStringTable)[STRING_USER_REGISTER_ID_LENGTH].GetString(), PlayerInfo::minIDLength, PlayerInfo::maxIDLength);
		g_pUIDialog->PopupFreeMessageDlg( strTemp );
		bAllOK = FALSE;
	}

	if (bAllOK)
	{
		if (!IsValidID(pChar->sz_name, "-_"))
		{
			//---------------------------------------------
			// 잘못된 ID인 경우
			//---------------------------------------------						
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
			bAllOK = FALSE;
		}
		else
		{
			char strName[80];
			strcpy(strName, pChar->sz_name);

			// 안 좋은 말이 들어있는 경우는 허용이 안된다
			if (g_pChatManager->RemoveCurse(strName))
			{
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
				bAllOK = FALSE;
			}
			else
			{
				MChatManager badNameManager;
				badNameManager.LoadFromFile( g_pFileDef->getProperty("FILE_INFO_CHAT_2").c_str());

				if (badNameManager.RemoveCurse(strName))
				{
					g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
					bAllOK = FALSE;
				}
			}
		}
	}

	//---------------------------------------------
	// 모든 data가 정상적이면...
	//---------------------------------------------
	if (bAllOK)
	{

			//char wansungName[16];

			//---------------------------------------------
			// 조합형 --> 완성형
			//---------------------------------------------
			//UI_JohapToWansung( pChar->sz_name, wansungName );

			CLCreatePC	_CLCreatePC;
			_CLCreatePC.setName ( pChar->sz_name );//wansungName );
			
			g_pUserInformation->Character[pChar->slot] = pChar->sz_name;
			//g_pUserInformation->CharacterW[pChar->slot] = wansungName;

			HairStyle hairStyle[3] =
			{
				HAIR_STYLE1,
				HAIR_STYLE2,
				HAIR_STYLE3
			};

			// 색깔 확인
			pChar->hair_color = max(0, min(pChar->hair_color, MAX_COLORSET-1));
			pChar->skin_color = max(0, min(pChar->skin_color, MAX_COLORSET-1));
			
			_CLCreatePC.setSlot ( pChar->slot==0? SLOT1 : pChar->slot==1? SLOT2 : SLOT3 );
			_CLCreatePC.setRace ( pChar->race );
			_CLCreatePC.setSex ( pChar->bl_female? FEMALE : MALE );
			_CLCreatePC.setHairStyle ( hairStyle[pChar->face] );
			_CLCreatePC.setHairColor ( pChar->hair_color );					
			_CLCreatePC.setSkinColor ( pChar->skin_color );
			_CLCreatePC.setShirtColor ( pChar->coat_color, MAIN_COLOR );
			_CLCreatePC.setShirtColor ( pChar->coat_color, SUB_COLOR );
			_CLCreatePC.setJeansColor ( pChar->trouser_color, MAIN_COLOR );
			_CLCreatePC.setJeansColor ( pChar->trouser_color, SUB_COLOR );
			_CLCreatePC.setSTR( pChar->STR );
			_CLCreatePC.setDEX( pChar->DEX );
			_CLCreatePC.setINT( pChar->INT );

			g_pSocket->sendPacket( &_CLCreatePC );

			g_pSocket->setPlayerStatus( CPS_AFTER_SENDING_CL_CREATE_PC );	

			

		SetMode(MODE_WAIT_CREATEPCOK);
	}
}


//-----------------------------------------------------------------------------
//
// Run NewUser Registration
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_RUN_NEWUSER_REGISTRATION(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_RUN_NEWUSER_REGISTRATION");
	
	switch (g_pClientConfig->NEW_USER_REGISTERATION_MODE)
	{
		//-----------------------------------------------------------
		// CLIENT - 새 사용자 등록 가능
		//-----------------------------------------------------------
		case ClientConfig::NUR_CLIENT :
		{
//			gC_vs_ui.RunNewUserRegistration();
		}
//		break;

		//-----------------------------------------------------------
		// HOMEPAGE - 홈페이지를 띄워주면서 등록
		//-----------------------------------------------------------
		case ClientConfig::NUR_HOMEPAGE :		
		{
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_HOMEPAGE].GetString() );			

			// 종료..
			SetMode( MODE_QUIT );

			char str[256];

			GetWindowsDirectory(
				str,  // address of buffer for Windows directory
				255        // size of directory buffer
			);

			sprintf(str, "%s\\Explorer.exe", str);
			
			CDirectDraw::GetDD()->RestoreDisplayMode();

			_spawnl(_P_NOWAIT, str, "Explorer.exe", g_pClientConfig->URL_HOMEPAGE_NEW_USER.GetString(), NULL);
		}
		break;

		//-----------------------------------------------------------
		// MESSAGE_HOMEPAGE - 홈페이지에서 등록하라는 message
		//-----------------------------------------------------------
		case ClientConfig::NUR_MESSAGE_HOMEPAGE :	
		{
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_HOMEPAGE].GetString() );			
		}
		break;
		
		//-----------------------------------------------------------
		// DENY - 새 사용자 등록 불가		
		//-----------------------------------------------------------
		case ClientConfig::NUR_DENY :				
		{
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_DENY].GetString() );						
		}
		break;	
	}

}

//-----------------------------------------------------------------------------
//
// Char Info Close
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_INFO_CLOSE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_INFO_CLOSE");
	
	if (g_Mode!=MODE_WAIT_SELECTPC
		&& g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_WAIT_SELECTPC");
		return;
	}
	
	gC_vs_ui.CloseInfo();
}

//-----------------------------------------------------------------------------
//
// Check Exist ID
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CHECK_EXIST_ID(int left, int right, void* void_ptr)
{
	//
	// left = id
	//
	DEBUG_ADD("[UI] UI_CHECK_EXIST_ID");
	
	const char* pName = (const char*)void_ptr;

	//--------------------------------------------------
	// ID 체크
	//--------------------------------------------------
	if (IsValidID(pName, NULL))
	{
		int len = strlen(pName);	
		
		if (len<PlayerInfo::minIDLength || len>PlayerInfo::maxIDLength)
		{
			char strTemp[128];
			sprintf(strTemp, "ID는 %d~%d자입니다", PlayerInfo::minIDLength, PlayerInfo::maxIDLength);
			g_pUIDialog->PopupFreeMessageDlg( strTemp );						
		}
		else
		{
			//--------------------------------------------------
			// socket 초기화
			//--------------------------------------------------
			if (!InitSocket())
			{						
				//InitFail("[Error] Can't init Socket");
				return;
			}

				CLQueryPlayerID _CLQueryPlayerID;
				//_CLDeletePC.setName( g_pUserInformation->CharacterW[pChar->slot].GetString() );
				_CLQueryPlayerID.setPlayerID( pName );

				g_pSocket->sendPacket( &_CLQueryPlayerID );

				g_pSocket->setPlayerStatus( CPS_AFTER_SENDING_CL_QUERY_PLAYER_ID );


			//gC_vs_ui.AleadyExistIdMessage(); // 사용불가
			//gC_vs_ui.NoAleadyExistIdMessage(); // 사용가능						
		}
	}
	else
	{
		g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
	}

	// 지워도 되는게 맞는지.. 흠 *_*;
	DeleteNewArray( void_ptr );
}


//-----------------------------------------------------------------------------
//
//		캐릭터 삭제
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_DELETE_CHARACTER(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_DELETE_CHARACTER");
	
	if (g_Mode!=MODE_WAIT_SELECTPC)
	{
		DEBUG_ADD("Not Mode MODE_WAIT_SELECTPC");
		return;
	}
	
	DELETE_CHARACTER *pChar = (DELETE_CHARACTER*)void_ptr;

	//--------------------------------------------------
	// 주민등록번호 체크
	//--------------------------------------------------
	// 넷마블용
	if (g_pUserInformation->IsNetmarble || g_pUserInformation->bChinese || IsValidSSN( pChar->sz_part1, pChar->sz_part2 ))
	{
		char strTemp[20];
		
		if( g_pUserInformation->bChinese )
		{
			sprintf(strTemp,"%s",pChar->sz_part1);
		} else
		if(!g_pUserInformation->IsNetmarble)
		{
			// xxxxxx-xxxxxxx 형식으로 만든다.
			sprintf(strTemp, "%s-%s", pChar->sz_part1, pChar->sz_part2);
		}
		else
			sprintf(strTemp, "1");

			CLDeletePC _CLDeletePC;
			//_CLDeletePC.setName( g_pUserInformation->CharacterW[pChar->slot].GetString() );
			_CLDeletePC.setName( g_pUserInformation->Character[pChar->slot].GetString() );
			_CLDeletePC.setSlot( pChar->slot==0? SLOT1 : pChar->slot==1? SLOT2 : SLOT3 );
			
			_CLDeletePC.setSSN( strTemp );

			g_pSocket->sendPacket( &_CLDeletePC );

			g_pSocket->setPlayerStatus( CPS_AFTER_SENDING_CL_DELETE_PC );
			

		// 주민등록번호
		//pChar->sz_part1
		//pChar->sz_part2					

		// 주민등록번호가 틀렸을 때의 message
		//gC_vs_ui.Invalid_SSN_Message();

		// 캐릭터 삭제
		//gC_vs_ui.DeleteCharacter(slot);

		//
		// ((DELETE_CHARACTER *)void_ptr) = sz_part1, sz_part2, slot
		//

		// 넷마블용
		if(!g_pUserInformation->IsNetmarble)
		{
			DeleteNewArray( pChar->sz_part1 );
			DeleteNewArray( pChar->sz_part2 );
		}

		//gC_vs_ui.Invalid_SSN_Message();
		//gC_vs_ui.DeleteCharacter( pChar->slot );

		//--------------------------------------------
		// 삭제할려는 slot저장
		//--------------------------------------------
		g_pUserInformation->Slot = pChar->slot;

		SetMode( MODE_WAIT_DELETEPCOK );
	}
	else
	{
		g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_SSN].GetString() );
	}
}

	
//-----------------------------------------------------------------------------
//
//		프로그램 종료
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_TERMINATION(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_TERMINATION");
	
	if (g_Mode!=MODE_MAINMENU)		
	{
		DEBUG_ADD("Not Mode MODE_MAINMENU");
		return;
	}

	SetMode( MODE_QUIT );
}

//-----------------------------------------------------------------------------
//
// 새 사용자 등록
//
//-----------------------------------------------------------------------------
/*
void
UIMessageManager::Execute_UI_NEW_USER_REGISTRATION(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_NEW_USER_REGISTRATION");
	
	//
	// ((NEW_REGISTRATION *)void_ptr) = ...
	//
	// // !string ptr을 저장하면 안된다.
	//
	//struct NEW_REGISTRATION
	//{
	//	char *	sz_id;
	//	char *	sz_password;
	//	char *	sz_repassword;
	//	char *	sz_name;
	//	char *	sz_email;
	//	char *	sz_address;
	//	char *	sz_ssn_number_part1; // 주민번호
	//	char *	sz_ssn_number_part2; // 주민번호
	//	char *	sz_homepage;
	//	char *	sz_woo;
	//	char *	sz_phone;
	//	bool		bl_female;
	//	bool		bl_announce_my_info;
	//};
	//			

	NEW_REGISTRATION* pReg = (NEW_REGISTRATION *)void_ptr;

	BOOL AllOK = TRUE;

	//--------------------------------------------------
	// string 길이 체크
	//--------------------------------------------------
	if (pReg->sz_id==NULL 
		|| pReg->sz_password==NULL
		|| pReg->sz_name==NULL
		|| pReg->sz_ssn_number_part1==NULL
		|| pReg->sz_ssn_number_part2==NULL
		|| pReg->sz_email==NULL)
	{
		// 필수항목이 입력 안된 경우
		AllOK = FALSE;

		g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_USER_REGISTER_EMPTY_FIELD].GetString());
	}
	else
	{
		int len;
		char strTemp[128];
			
		
		//--------------------------------------------------
		// ID 길이 체크
		//--------------------------------------------------
		len = strlen(pReg->sz_id);	
		
		if (len<PlayerInfo::minIDLength || len>PlayerInfo::maxIDLength)
		{
			sprintf(strTemp, (*g_pGameStringTable)[STRING_USER_REGISTER_ID_LENGTH].GetString(), PlayerInfo::minIDLength, PlayerInfo::maxIDLength);
			g_pUIDialog->PopupFreeMessageDlg( strTemp );
			AllOK = FALSE;
		}

		//---------------------------------------------
		// 잘못된 ID인지 체크한다.
		//---------------------------------------------						
		if (AllOK)
		{
			if (!IsValidID(pReg->sz_id, NULL))
			{
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
				AllOK = FALSE;
			}
			else
			{
				char strName[80];
				strcpy(strName, pReg->sz_id);

				// 안 좋은 말이 들어있는 경우는 허용이 안된다.
				if (g_pChatManager->RemoveCurse(strName))
				{
					g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
					AllOK = FALSE;
				}								
			}
		}
		
		//--------------------------------------------------
		// Password 길이 체크
		//--------------------------------------------------
		if (AllOK)
		{
			len = strlen(pReg->sz_password);	
			
			if (len<PlayerInfo::minPasswordLength || len>PlayerInfo::maxPasswordLength)
			{
				sprintf(strTemp, (*g_pGameStringTable)[STRING_USER_REGISTER_PASSWORD_LENGTH].GetString(), PlayerInfo::minPasswordLength, PlayerInfo::maxPasswordLength);
				g_pUIDialog->PopupFreeMessageDlg( strTemp );
				AllOK = FALSE;
			}
			else if (!IsValidPassword(pReg->sz_id))
			//else if (!IsValidID(pReg->sz_id))	// 원래는 이거 써야되는데
			// 이미 만들어진 아이디 중에..  ID생성룰에 적합하지 않은 것도 있어서..
			{
				// ID에 특수문자가 들어간 경우
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_ID_SPECIAL].GetString() );
				AllOK = FALSE;
			}			
			else if (!IsValidPassword(pReg->sz_password))
			{
				// 패스워드가 잘못된 경우
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_PASSWORD_SPECIAL].GetString() );
				AllOK = FALSE;
			}
			else
			{
				//--------------------------------------------------
				// 숫자만 사용하면 안된다.
				//--------------------------------------------------
				char* str = pReg->sz_password;
				
				char ch;

				bool AllNumber = TRUE;

				while (ch=*str++, ch)
				{
					if (ch<'0' || ch>'9')
					{
						AllNumber = FALSE;
						break;
					}
				}

				if (AllNumber)	// 전부 숫자인 경우..
				{
					g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_PASSWORD_NUMBER].GetString() );
					AllOK = FALSE;
				}
			}

		}

		//--------------------------------------------------
		// 이름
		//--------------------------------------------------
		if (AllOK)
		{
			len = strlen(pReg->sz_name);	
			
			if (len>PlayerInfo::maxNameLength)
			{
				sprintf(strTemp, (*g_pGameStringTable)[STRING_USER_REGISTER_NAME_LENGTH].GetString(), PlayerInfo::maxNameLength);
				g_pUIDialog->PopupFreeMessageDlg( strTemp );
				AllOK = FALSE;
			}		
		}

		//--------------------------------------------------
		// 이름
		//--------------------------------------------------
		if (AllOK)
		{
			// 제대로 입력된 경우
			if (//strlen(pReg->sz_ssn_number) == 6+1+7
				//&& pReg->sz_ssn_number[6]=='-')
				1)
			{
				char ssn1[7];
				char ssn2[8];

				
				//pReg->sz_ssn_number[6] = '\0';
				//strcpy(ssn1, pReg->sz_ssn_number);

				//pReg->sz_ssn_number[6] = '-';
				//strcpy(ssn2, pReg->sz_ssn_number + 7);
				
				strcpy(ssn1, pReg->sz_ssn_number_part1);
				strcpy(ssn2, pReg->sz_ssn_number_part2);

				//--------------------------------------------------
				// 주민등록번호 체크
				//--------------------------------------------------
				if (!IsValidSSN( ssn1, ssn2 ))
				{
					sprintf(strTemp, (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_SSN].GetString());
					g_pUIDialog->PopupFreeMessageDlg( strTemp );
					AllOK = FALSE;
				}
				
			}		
			//else
			//{
			//	sprintf(strTemp, (*g_pGameStringTable)[STRING_USER_REGISTER_SSN_FORMAT].GetString());
			//	g_pUIDialog->PopupFreeMessageDlg( strTemp );
			//	AllOK = FALSE;
			//}
		}
	}

	//--------------------------------------------------
	// 모두 정상이면..
	//--------------------------------------------------
	if (AllOK)
	{
		if (!InitSocket())
		{						
			//InitFail("[Error] Can't init Socket");
			return;
		}

		#ifdef	CONNECT_SERVER

			//--------------------------------------------------
			// CLVersionCheck
			//--------------------------------------------------
			#ifndef _DEBUG

				if (!g_bTestMode)
				{
					int version;
					ifstream versionFile;//(FILE_INFO_ACTION, std::ios::binary);
					if (!FileOpenBinary(FILE_INFO_VERSION, versionFile))
						return;
					versionFile.read((char*)&version, 4);
					versionFile.close();

					CLVersionCheck _CLVersionCheck;
					_CLVersionCheck.setVersion( version );

					g_pSocket->sendPacket( &_CLVersionCheck );
				}
				
			#endif
			
			char ssnAll[20];

			sprintf(ssnAll, "%s-%s", pReg->sz_ssn_number_part1, pReg->sz_ssn_number_part2);							
			
			//--------------------------------------------------
			// CLRegisterPlayer
			//--------------------------------------------------
			CLRegisterPlayer	_CLRegisterPlayer;
			
			//--------------------------------------------------
			// 필수항목
			//--------------------------------------------------
			_CLRegisterPlayer.setID( pReg->sz_id );
			_CLRegisterPlayer.setPassword( pReg->sz_password );
			_CLRegisterPlayer.setName( pReg->sz_name );
			_CLRegisterPlayer.setSSN( ssnAll );
			_CLRegisterPlayer.setEmail( pReg->sz_email );						
			_CLRegisterPlayer.setSex( pReg->bl_female? FEMALE:MALE );
			
			//--------------------------------------------------
			// default
			//--------------------------------------------------
			_CLRegisterPlayer.setNation( KOREA );


			//--------------------------------------------------
			// 필수항목이 아닌 것들
			//--------------------------------------------------
			if (pReg->sz_address!=NULL)
			{
				_CLRegisterPlayer.setAddress( pReg->sz_address );
			}
			else
			{
				_CLRegisterPlayer.setAddress( "NULL" );
			}

			if (pReg->sz_phone!=NULL)
			{
				_CLRegisterPlayer.setCellular( pReg->sz_phone );
			}
			else
			{
				_CLRegisterPlayer.setCellular( "NULL" );
			}

			if (pReg->sz_homepage!=NULL)
			{
				_CLRegisterPlayer.setHomepage( pReg->sz_homepage );
			}
			else
			{
				_CLRegisterPlayer.setHomepage( "NULL" );
			}
			
			if (pReg->sz_phone!=NULL)
			{
				_CLRegisterPlayer.setTelephone( pReg->sz_phone );
			}
			else
			{
				_CLRegisterPlayer.setTelephone( "NULL" );
			}

			if (pReg->sz_woo!=NULL)
			{
				_CLRegisterPlayer.setZipCode( pReg->sz_woo );
			}
			else
			{
				_CLRegisterPlayer.setZipCode( "NULL" );
			}

			//--------------------------------------------------
			// 지금 없다.. 
			//--------------------------------------------------
			_CLRegisterPlayer.setProfile( "profile" );

			g_pSocket->setPlayerStatus( CPS_AFTER_SENDING_CL_REGISTER_PLAYER );

			g_pSocket->sendPacket( &_CLRegisterPlayer );
			
		#endif	

		// id기억
		g_pUserInformation->UserID = pReg->sz_id;

		SetMode( MODE_WAIT_REGISTERPLAYEROK );

		//gC_vs_ui.StartCharacterManager();
		DeleteNewArray(pReg->sz_id);
		DeleteNewArray(pReg->sz_password);
		DeleteNewArray(pReg->sz_repassword);
		DeleteNewArray(pReg->sz_name);
		DeleteNewArray(pReg->sz_email);
		DeleteNewArray(pReg->sz_address);
		DeleteNewArray(pReg->sz_ssn_number_part1);
		DeleteNewArray(pReg->sz_ssn_number_part2);
		DeleteNewArray(pReg->sz_homepage);
		DeleteNewArray(pReg->sz_woo);
		DeleteNewArray(pReg->sz_phone);

		//gC_vs_ui.CloseUserRegistrationWindow();
		//gC_vs_ui.StartCharacterManager();
		//gC_vs_ui.ClearAllCharacter();
	}
	
}
*/

//-----------------------------------------------------------------------------
//
// Login
//
//-----------------------------------------------------------------------------
extern BYTE g_macAddress[6];

void
UIMessageManager::Execute_UI_LOGIN(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_LOGIN");

	UI_SaveUserOption();

	//
	// ((C_VS_UI::LOGIN *)void_ptr) = id, password
	//
	// !string ptr을 저장하면 안된다.
	//
	LOGIN*	login = (LOGIN*)void_ptr;

	if (g_Mode!=MODE_MAINMENU)
	{
		DeleteNewArray(login->sz_id);
		DeleteNewArray(login->sz_password);

		DEBUG_ADD("Not Mode MODE_MAINMENU");
		return;
	}	

	// 슬레이어 그림 일부 로딩 - 2001.8.20
	LoadingAddonSPK( false );

	if (login->sz_id!=NULL && login->sz_password!=NULL)
	{
		//if (IsValidID(login->sz_id, NULL))
		{
			// ID기억
			//UI_BackupLoginID(((LOGIN *)void_ptr)->sz_id);
			
			if (IsValidPassword(login->sz_password))
			{
				{
					DEBUG_ADD("[Execute_UI_LOGIN]");
					if (!InitSocket())
					{
						//InitFail("[Error] Can't init Socket");
						DEBUG_ADD("[Execute_UI_LOGIN] Can't init Socket");
						DeleteNewArray(login->sz_id);
						DeleteNewArray(login->sz_password);
						DEBUG_ADD("[Execute_UI_LOGIN] Can't init Socket2");
						return;
					} 

					//char wansungID[16];

					//---------------------------------------------
					// 조합형 --> 완성형
					//---------------------------------------------
					//UI_JohapToWansung( login->sz_id, wansungID );
	
					//--------------------------------------------------
					// 로그인 서버에 연결한 후 
					// 가장 먼저 보내야 하는 패킷은 CLLogin 패킷이다.
					// 근데,
					// Version체크 packet이 먼저다. - -;
					//--------------------------------------------------
					//--------------------------------------------------
					// CLVersionCheck
					//--------------------------------------------------
					#if !defined(_DEBUG) && !defined(OUTPUT_DEBUG)
						int version = g_pUserInformation->GameVersion;
//						ifstream versionFile;//(FILE_INFO_ACTION, std::ios::binary);
//						if (!FileOpenBinary(FILE_INFO_VERSION, versionFile))
//						{
//							DeleteNewArray(login->sz_id);
//							DeleteNewArray(login->sz_password);
//							return;
//						}
//						versionFile.read((char*)&version, 4);
//						versionFile.close();

						CLVersionCheck _CLVersionCheck;
						_CLVersionCheck.setVersion( version );

						g_pSocket->sendPacket( &_CLVersionCheck );

						
					#endif
					

					//--------------------------------------------------
					// CLLogin
					//--------------------------------------------------
					CLLogin clLogin;

					//clLogin.setID("Reiot");
					//clLogin.setPassword("fpdldhxm");

					// 넷마블용
					std::string temp_id;
					DEBUG_ADD("[Execute_UI_LOGIN] #");
					#ifdef __METROTECH_TEST__
						temp_id = "#";
					#endif
					
					//if(g_pUserInformation->IsNetmarble)
					//	temp_id += "@";
					
					temp_id += login->sz_id;
										
					clLogin.setID( temp_id );//wansungID);
					clLogin.setPassword(login->sz_password);			

					// 2004,03,16 sobeit add start - mac address
//					BYTE MacAddress[6];
//					memset(MacAddress, 0,sizeof(BYTE)*6);
//					memcpy(MacAddress, g_macAddress, sizeof(BYTE)*6);
//					GetMacAddressFromNetBIOS(MacAddress);
					clLogin.setMacAddress(g_macAddress);
					// 2004,03,16 sobeit add end - mac address
					clLogin.SetLoginMode(g_pUserInformation->IsAutoLogIn);

					if (g_pSocket)
					{
						g_pSocket->sendPacket( &clLogin );
						g_pSocket->setPlayerStatus( CPS_AFTER_SENDING_CL_LOGIN );
					}
					DEBUG_ADD("[Execute_UI_LOGIN] SendPacket OK");

					//----------------------------------------------------
					// UserInformation에 저장한다.
					//----------------------------------------------------
					g_pUserInformation->UserID = login->sz_id;
					//g_pUserInformation->UserIDW = wansungID;
				}
				
				DEBUG_ADD("[Execute_UI_LOGIN] ClearAllCharacter");
				gC_vs_ui.ClearAllCharacter();
				DEBUG_ADD("[Execute_UI_LOGIN] ClearAllCharacter OK");
				if(false == g_pUserInformation->IsAutoLogIn) 
					SetMode( MODE_WAIT_LOGINOK );
				DEBUG_ADD("[Execute_UI_LOGIN] SetMode OK");
			}
			else
			{
				// 패스워드가 잘못된 경우
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_PASSWORD_SPECIAL].GetString() );
			}
		}
		//---------------------------------------------
		// 잘못된 ID인 경우
		//---------------------------------------------
		//else
		//{
			//g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );							
		//}
	}
	

	// (!) 원래는 캐릭터 선택 창을 띄워야 한다.

	//gC_vs_ui.StartGame();
	//gC_vs_ui.StartCharacterManager();

	DeleteNewArray(login->sz_id);
	DeleteNewArray(login->sz_password);
	DEBUG_ADD("[Execute_UI_LOGIN] OK");
}

//-----------------------------------------------------------------------------
//
// 캐릭터 선택 창에서 main menu로 돌아갈때
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CHARACTER_MANAGER_FINISHED(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CHARACTER_MANAGER_FINISHED");

	if (g_Mode!=MODE_WAIT_SELECTPC)
	{
		DEBUG_ADD("g_Mode not MODE_WAIT_SELECTPC");

		return;
	}

	// 넷마블용
	if(g_pUserInformation->IsNetmarble)
	{
		SetMode( MODE_MAINMENU );
		return;
	}

	//
	// Client, Server에 logout packet 보냄.
	//
	//--------------------------------------------------
	// Login 서버로 CLLogout 패킷을 보낸다.
	//--------------------------------------------------
	/*
	#ifdef	CONNECT_SERVER		
		
		// hot key save한다.
		//UI_SaveHotKeyToServer();

		CLLogout clLogout;
	
		g_pSocket->sendPacket( &clLogout );
		//g_pSocket->setPlayerStatus( CPS_END_SESSION );
		//g_pSocket->disconnect();

		ReleaseSocket();
	#endif					


	SetMode( MODE_MAINMENU );
	*/

	UI_SetServerList();

	SetMode( MODE_WAIT_SELECT_SERVER );
}

//-----------------------------------------------------------------------------
//
// UI_CONNECT - 캐릭터 선택 했을 때
//
//-----------------------------------------------------------------------------
// 게임 시작하려고 할때...
void
UIMessageManager::Execute_UI_CONNECT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CONNECT");
	
	if (g_Mode!=MODE_WAIT_SELECTPC)
	{
		DEBUG_ADD("Not MODE_WAIT_SELECTPC");
		return;
	}

	//------------------------------------------------------------
	// 음악 중지 - 2001.8.20
	//------------------------------------------------------------
	if (g_pUserOption!=NULL)
	{
		if (g_pUserOption->PlayWaveMusic)
		{
//			if (g_pDXSoundStream!=NULL)
//			{
//				g_pDXSoundStream->Stop();
//			}
#ifdef __USE_MP3__
			if (g_pMP3 != NULL)
			{
				DEBUG_ADD("MP3 STOP13");
				g_pMP3->Stop();
				DEBUG_ADD("MP3 STOP13 OK");
			}
#else
			if (g_pOGG != NULL)
			{
				DEBUG_ADD("MP3 STOP13");
				if( g_DXSound.IsInit() )
					g_pOGG->streamClose();
				DEBUG_ADD("MP3 STOP13 OK");
			}
		}
		else
		{
			g_Music.Stop();
		}
	}

	//------------------------------------------------------------
	// 접속해야된다고 출력해준다.
	//------------------------------------------------------------
	if (g_pUIDialog!=NULL)
	{
		//g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_LOGOUT_DIED].GetString() );
		g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CONNECTING_SERVER].GetString(), -1, -1, 0 );

//		if (CDirect3D::IsHAL())
//		{
//			DEBUG_ADD("UI show1");
//			gC_vs_ui.Show();
//			//gC_vs_ui.DrawMousePointer();		
//			DEBUG_ADD("UI show1 OK");
//		}
//		else
		{
			POINT point;
			
			DEBUG_ADD("UI show2");
			gC_vs_ui.Show();
			DEBUG_ADD("UI show2 OK");

			//gC_vs_ui.DrawMousePointer();

			//-----------------------------------------------------------------
			// Last를 Back으로 copy - 3D HAL이 아닌 경우만..
			//-----------------------------------------------------------------		
			point.x = 0;
			point.y = 0;
			RECT rect = { 0, 0, SURFACE_WIDTH, SURFACE_HEIGHT };
			g_pBack->BltNoColorkey( &point, g_pLast, &rect );	
		}
		CDirectDraw::Flip();

		// 화면에 그려주고 지우면 된당.. ㅋㅋ
		DEBUG_ADD("close msg dlg");
		g_pUIDialog->CloseMessageDlg();
	}

	DEBUG_ADD("LoadingAddonSPK");

	//------------------------------------------------------------
	// Test 2001.8.20
	//------------------------------------------------------------
	// 캐릭 선택 후에 바로
	// 슬레이어 그림 일부 로딩 - 2001.8.20
	LoadingAddonSPK( false );
	

	DEBUG_ADD_FORMAT("CLSelectPC(%d)", left);

	CLSelectPC clSelectPC;
	// CGConnect 때 사용하려면 여기서 저장해둬야 한다.					
	//clSelectPC.setPCName( (const char *)g_pUserInformation->CharacterW[left] );//"sigi");
	clSelectPC.setPCName( (const char *)g_pUserInformation->Character[left] );//"sigi");
	clSelectPC.setPCType( (PCType)(g_pUserInformation->Race[left]));

	DEBUG_ADD_FORMAT("socket Set PC type");

	g_pSocket->setPCType(clSelectPC.getPCType());
	g_pSocket->setPCName(clSelectPC.getPCName());

	DEBUG_ADD_FORMAT("user Information");

	g_pUserInformation->CharacterID = g_pUserInformation->Character[left];
	g_pUserInformation->FaceStyle = g_pUserInformation->FaceStyleSlot[left];	
	g_pUserInformation->IsMaster = 
		strncmp( g_pUserInformation->CharacterID, (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetLength() ) == 0 
//		(strstr(g_pUserInformation->CharacterID, "운영자")!=NULL)
		|| g_pInventory->FindItem(ITEM_CLASS_ETC, 0);
//		|| g_pPlayer->GetCreatureType()==CREATURETYPE_SLAYER_OPERATOR
//		|| g_pPlayer->GetCreatureType()==CREATURETYPE_VAMPIRE_OPERATOR;
//		g_pPlayer 가 없어서 뻑나네 검사할까...생각했지만..쌩~

	#ifdef OUTPUT_DEBUG
		DEBUG_ADD("MasterCheck");
		ifstream file("master.txt", std::ios::in);

		char str[256];
		while (!file.eof())
		{
			file.getline(str, 256);
			
			int n = file.gcount();

			if (n <= 0)
				break;
			
			str[n] = '\0';

			g_pUserInformation->IsMaster |= (strstr(g_pUserInformation->CharacterID, str)!=NULL);
		}
		
		file.close();
		DEBUG_ADD("MasterCheck OK");
	#endif
	
	//g_pUserInformation->CharacterIDW = g_pUserInformation->CharacterW[left];
	DEBUG_ADD("send CLSelectPC");
	g_pSocket->sendPacket( &clSelectPC );
	g_pSocket->setPlayerStatus( CPS_AFTER_SENDING_CL_SELECT_PC );

	// 바로 보낸다.
	UpdateSocketOutput();

	SaveLastSelectedCharacter( left );

	DEBUG_ADD("set mode MODE_WAIT_RECONNECT");
	SetMode(MODE_WAIT_RECONNECT);
#endif
	//------------------------------
	// 게임 UI
	//------------------------------
	//gC_vs_ui.StartGame();
					
	//
	// Skill은 StartGame()이 실행된 이후에 추가해야 한다.
	//
	//gC_vs_ui.AddSlayerSkill(SKILL_FLASH_SLASHER);
}


//-----------------------------------------------------------------------------
//
// Chat에서 Enter눌렀을 때
//
//-----------------------------------------------------------------------------
// 채팅창 입력
void
UIMessageManager::Execute_UI_CHAT_RETURN(int left, int right, void* void_ptr)
{
	//
	// left = true이면 pcs에서 입력한 것이고, false리면 일반채팅임.
	//
	// ! pcs에서 입력된 경우 slot 번호를 알기 위해서는 다음 함수를 실행한다.
	//   => gC_vs_ui.GetSendPossibleSlot();
	//      유효한 숫자는 0, 1, 2 이며, NOT_SELECTED이면 어떠한 slot도 아니다.
	//
	// void_ptr = input string
	//	right = color
	//

	DEBUG_ADD("[UI] UI_CHAT_RETURN");

	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}
	
	DEBUG_ADD_FORMAT("[Original] %s", (char*)void_ptr );
	
	char* chatString = (char*)void_ptr; 
//	g_pChatManager->RemoveCurse(chatString);
	//-------------------------------------------------------------
	// 파티 채팅인 경우
	//-------------------------------------------------------------
	if (chatString!=NULL 
		&& chatString[0]!=SYMBOL_WHISPER	// 귓속말이 아닌 경우에..
		//&& chatString[0]!=SYMBOL_MASTER_COMMAND
		&& left==CLD_PARTY)
	{
		if (g_pParty!=NULL )
		{
			if(g_pUserInformation->bKorean == true && g_pClientCommunicationManager!=NULL)
			{
				RCSay _RCSay;
				_RCSay.setName( g_pUserInformation->CharacterID.GetString() );
				_RCSay.setMessage( chatString );
				_RCSay.setColor( right );

				if (g_pParty->GetSize()==0)
				{
					DEBUG_ADD("PartySize==0");
				}
			
				for (int i=0; i<g_pParty->GetSize(); i++)
				{
					PARTY_INFO* pInfo = g_pParty->GetMemberInfo( i );
					
					if (pInfo!=NULL)										
					{
						int port = g_pClientConfig->CLIENT_COMMUNICATION_UDP_PORT;

						RequestUserInfo* pUserInfo = g_pRequestUserManager->GetUserInfo( pInfo->Name.GetString() );

						if (pUserInfo!=NULL)
						{
							if (pUserInfo->UDPPort!=0)
								port = pUserInfo->UDPPort;
						}
						
						g_pClientCommunicationManager->sendPacket( 
								pInfo->IP, 
								port,
								&_RCSay );
					}
					else
					{
						DEBUG_ADD_FORMAT("pInfo[%d]==NULL", i);
					}
				}			
				UI_AddChatToHistory( chatString, g_pUserInformation->CharacterID.GetString(), CLD_PARTY, right );
			}
			else	// 한국어가 아닐때
			{
				CGPartySay _CGPartySay;
				_CGPartySay.setColor(right);
				_CGPartySay.setMessage(chatString);
				g_pSocket->sendPacket( &_CGPartySay );
			}
		}
	}
	// 길드 채팅
	else if (chatString!=NULL 
		&& chatString[0]!=SYMBOL_WHISPER	// 귓속말이 아닌 경우에..
		&& chatString[0]!=SYMBOL_MASTER_COMMAND
		&& (left==CLD_GUILD || left == CLD_UNION))
	{
		if(g_pUserInformation->GuildName.GetLength() > 0)
		{
			CGGuildChat _CGGuildChat;
			// 2004, 11, 11, sobeit add start
			if(left==CLD_GUILD)
				_CGGuildChat.SetType(0);
			else
				_CGGuildChat.SetType(1);
			// 2004, 11, 11, sobeit add end
			_CGGuildChat.setMessage( std::string(chatString) );
			_CGGuildChat.setColor( right );
			
			g_pSocket->sendPacket( &_CGGuildChat );
		}
	}
	//-------------------------------------------------------------
	// Player가 살아있는 경우만... chatting가능.. 했었지만..   --;
	//-------------------------------------------------------------
	else //if (g_pPlayer->IsAlive())
	{
		BOOL bZoneChat = (left==CLD_ZONECHAT);
	
		/*	
		#ifdef __GAME_CLIENT__
			if (g_pSystemMessage!=NULL)
			{
				// 서비스.. - -;;
				g_pSystemMessage->Add((*g_pGameStringTable)[STRING_MESSAGE_CHAT_BE_GOOD].GetString());
			}
		#endif
		*/

		//int pcsChat = left;

		//-------------------------------------------------------------
		//
		// PCS chat
		//
		//-------------------------------------------------------------
		//if (pcsChat)
		//{
			/*
			int slot = gC_vs_ui.GetSendPossibleSlot();

			if (slot!=NOT_SELECTED)
			{
				char* strUI = (char*)chatString;

				#ifdef	CONNECT_SERVER
					CGPhoneSay _CGPhoneSay;
					_CGPhoneSay.setSlotID( slot );
					_CGPhoneSay.setMessage( strUI );

					g_pSocket->sendPacket( &_CGPhoneSay );					
				#endif

				char temp[128];
				sprintf(temp, "[%s] %s", g_pUserInformation->CharacterID.GetString(), strUI);
				UI_AddChatToHistory( temp );								
			}
			*/
		//}
		//-------------------------------------------------------------
		//
		// 일반 채팅
		//
		//-------------------------------------------------------------
		//else
		{
			//
			// chat history Window에 보내기... 일단 바로 보내보자.
			//
			//break;

			// server로 message 보내기
			//g_Socket.Send(g_String);
			char* strUI = chatString;
			char* strOrg = new char[128];
			char* str = strOrg;

			strcpy( strOrg, strUI );

			if (str!=NULL && str[0]!=NULL)
			{
				RemoveStringSpace( str );							

				DEBUG_ADD_FORMAT("[After RemoveSpace] %s", str );
					
				if (str!=NULL && str[0]!=NULL)
				{
					// 2004, 10, 25, sobeit add start - 드레곤 아이 관련
					if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_DRAGON_EYES))
					{
						if(0 == strncmp(str, (*g_pGameStringTable)[UI_STRING_MESSAGE_RANGER_SAY].GetString(),(*g_pGameStringTable)[UI_STRING_MESSAGE_RANGER_SAY].GetLength()))
						{
							char TempBuffer[128]; 
							strcpy(TempBuffer, str+(*g_pGameStringTable)[UI_STRING_MESSAGE_RANGER_SAY].GetLength());
							CGRangerSay _CGRangerSay;
							_CGRangerSay.setMessage(TempBuffer);

							g_pSocket->sendPacket( &_CGRangerSay );
										
							// Player의 Chat pWansungStringing에 추가
							g_pPlayer->SetChatString( str );//+1 );

							// history에 추가
							char temp[128];
							strcpy(temp, str );//+1);
							//sprintf(temp, "[%s] %s", g_pUserInformation->CharacterID.GetString(), str+1);
							//UI_AddChatToHistory( temp );								
							UI_AddChatToHistory( temp, g_pUserInformation->CharacterID.GetString(), CLD_ZONECHAT, right );
							return;
						}
					}
					// 2004, 10, 25, sobeit add end - 드레곤 아이 관련
					if (bZoneChat && str[0]!=SYMBOL_WHISPER
						&& str[0]!=SYMBOL_COMMAND && str[0]!=SYMBOL_MASTER_COMMAND)			
					{
						// system message test
						//------------------------------------------------------------
						//
						// '!'를 입력하면 global채팅이다.
						//
						//------------------------------------------------------------
						//case SYMBOL_GLOBALCHAT :	//if (str[0]=='!')//pWansungString[0]=='/')
						{
							//if (strlen(str) > 1 )	//pWansungString) > 1)
							{		
								//---------------------------------------------------------
								// 숨어 있을때는 말 못한다.
								// 늑대나 박쥐인 경우 말 못한다.
								//---------------------------------------------------------
								if (g_pPlayer->IsUndergroundCreature()
									|| g_pPlayer->IsInCasket()
									|| g_pPlayer->GetCreatureType()==CREATURETYPE_BAT
									|| g_pPlayer->GetCreatureType()==CREATURETYPE_WOLF
									|| g_pPlayer->GetCreatureType()==CREATURETYPE_WER_WOLF)
								{
									// history에 추가
									//char temp[128];
									//sprintf(temp, "[%s] .....", g_pUserInformation->CharacterID.GetString());
									//UI_AddChatToHistory( temp );								
									UI_AddChatToHistory( ".....", g_pUserInformation->CharacterID.GetString(), CLD_NORMAL, right );
								}
								//---------------------------------------------------------
								// global say
								//---------------------------------------------------------
								else
								{
									//---------------------------------------------------------
									// delay가 안 끝났으면 말 못한다.
									//---------------------------------------------------------
									if (1)//g_CurrentTime > g_pUserInformation->GlobalSayTime+g_pClientConfig->DELAY_GLOBAL_SAY
										//#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
										//	|| 1
										//#endif
										//)
									{
										CGGlobalChat _CGGlobalChat;
										_CGGlobalChat.setMessage( str );//+ 1 );	//pWansungString+1 );
										_CGGlobalChat.setColor( right );
										g_pSocket->sendPacket( &_CGGlobalChat );

										
										// Player의 Chat pWansungStringing에 추가
										g_pPlayer->SetChatString( str, right );//+1 );

										// history에 추가
										char temp[128];
										strcpy(temp, str );//+1);
										//sprintf(temp, "[%s] %s", g_pUserInformation->CharacterID.GetString(), str+1);
										//UI_AddChatToHistory( temp );								
										UI_AddChatToHistory( temp, g_pUserInformation->CharacterID.GetString(), CLD_ZONECHAT, right );

										// 현재 시간을 설정해둔다.
										//g_pUserInformation->GlobalSayTime = g_CurrentTime;

										// [도움말] 외치기 할 때
//										__BEGIN_HELP_EVENT
////											ExecuteHelpEvent( HE_CHAT_SHOUT );	
//										__END_HELP_EVENT
									}
									//else
									//{
										// 지금은 외치기 할 수 없다고 표시
									//	UI_AddChatToHistory( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_GLOBAL_SAY].GetString(), NULL, CLD_INFO );
									//}
								}
							}
						}
						//break;

					}					
					else // 으음.. 이 부분 코드가 안 이쁘네.. - -;;
					switch ( str[0] )
					{
						//------------------------------
						// 조합형 --> 완성형
						//------------------------------
						//char* pWansungString = new char [strlen(str)+1];

						//UI_JohapToWansung( str, pWansungString );

						
						//------------------------------------------------------------
						//
						// '/'를 입력하면 whisper다.
						//
						//------------------------------------------------------------
						case SYMBOL_WHISPER :						
						{
							// Vampire인 경우에						
							//else if (//g_pPlayer->IsVampire() && 
									//str[0]=='/')

							if (strlen(str) > 1 )
							{
								// '/'빼고 나머지 추가..
								CToken strToken(str+1);

								// [이름]+[ ]+[할말] 로 이루어져있다고 보면 된다.										
								const char* pName = strToken.GetToken();
								const char* pMessage = strToken.GetEnd();										
								
								if (pName!=NULL && pMessage!=NULL)
								{									
									int nameLen = strlen(pName);
									
									//------------------------------------------------------
									// ID길이가 잘못된 경우										
									//------------------------------------------------------
									if (nameLen<PlayerInfo::minIDLength || nameLen>PlayerInfo::maxIDLength)
									{
										char strTemp[256];
										sprintf(strTemp, (*g_pGameStringTable)[STRING_USER_REGISTER_ID_LENGTH].GetString(), 
													PlayerInfo::minIDLength,
													PlayerInfo::maxIDLength);

										UI_AddChatToHistory( strTemp, NULL, CLD_INFO, right );
									}
									//------------------------------------------------------
									// 정상적인 귓속말...일까?
									//------------------------------------------------------
									else
									{
										if (g_pUserInformation->CharacterID==pName)
										{
											// 자신에게 귓속말하는 경우
											UI_AddChatToHistory( (*g_pGameStringTable)[STRING_MESSAGE_WHISPER_SELF].GetString(), NULL, CLD_INFO, right );
										}
										else
										{
											// 귓속말 대상 설정 : ID + ' '
											char strWhisperID[128];
											sprintf(strWhisperID, "%s ", pName);
											g_pUserInformation->WhisperID = strWhisperID;

											/*
											CGWhisper _CGWhisper;
											_CGWhisper.setName( pName );
											_CGWhisper.setMessage( pMessage );

											g_pSocket->sendPacket( &_CGWhisper );
											*/
											g_pWhisperManager->SendWhisperMessage( pName, pMessage, right );

											
											char strMessage[128];
											char strName[128];
											//sprintf(temp, "[%s] <%s> %s", g_pUserInformation->CharacterID.GetString(), pName, pMessage);
											//UI_AddChatToHistory( temp );
											strcpy(strMessage, pMessage);
											// "[내가] 누구에게> 뭐라고"라는 식으로 표현된다.
											sprintf(strName, "[%s] %s", g_pUserInformation->CharacterID.GetString(), pName);
											UI_AddChatToHistory( strMessage, strName, CLD_WHISPER, right );

											// [도움말] 귓속말 할 때
//											__BEGIN_HELP_EVENT
////												ExecuteHelpEvent( HE_CHAT_WHISPER );	
//											__END_HELP_EVENT
										}
									}
								}
							}
						}
						break;

						//------------------------------------------------------------
						//
						// '@'를 입력하면 특수 명령어다.
						//
						//------------------------------------------------------------
						case SYMBOL_COMMAND :
						//else if (//g_pPlayer->IsVampire() && 
						//			str[0]=='@')
						{
							if (strlen(str) > 1 )
							{
								// '@'빼고 나머지 추가..
								CToken strToken(str+1);

								// [명령]+[ ]+[내용] 으로 이루어져있다고 보면 된다.										
								const char* pCommand = strToken.GetToken();
								const char* pData = strToken.GetEnd();

								char pLwrCommand[128];
								strcpy(pLwrCommand, pCommand);
								strcpy(pLwrCommand, _strlwr(pLwrCommand));

								
								DEBUG_ADD_FORMAT("[Command] %s %s", pCommand, pData);
								
								//-------------------------------------------------------
								// 대화 거부
								//-------------------------------------------------------
								if (strcmp(pCommand, STRING_IGNORE)==0
									|| strcmp(pLwrCommand, STRING_IGNORE_ENG)==0)
								{
									//-------------------------------------------------------
									// 모든 사람의 대화를 거부한다.
									//-------------------------------------------------------
									if (pData==NULL)
									{
										g_pChatManager->ClearID();
										g_pChatManager->SetIgnoreMode();	

										// 자기 ID는 추가해둔다.
										g_pChatManager->AddID( g_pUserInformation->CharacterID.GetString() );

										char strTemp[128];
										sprintf(strTemp, (*g_pGameStringTable)[STRING_MESSAGE_CHAT_IGNORE_ALL].GetString(), pData);
										UI_AddChatToHistory( strTemp, NULL, CLD_INFO, right );

										// [도움말] 대화거부
//										__BEGIN_HELP_EVENT
////											ExecuteHelpEvent( HE_CHAT_REJECT );	
//										__END_HELP_EVENT
									}
									//-------------------------------------------------------
									// 한 사람의 대화를 거부한다.
									//-------------------------------------------------------
									else
									{
										int nameLen = strlen( pData );

										//------------------------------------------------------
										// ID길이가 잘못된 경우										
										//------------------------------------------------------
										if (nameLen<PlayerInfo::minIDLength || nameLen>PlayerInfo::maxIDLength)
										{
											char strTemp[256];
											sprintf(strTemp, (*g_pGameStringTable)[STRING_USER_REGISTER_ID_LENGTH].GetString(), 
													PlayerInfo::minIDLength,
													nameLen>PlayerInfo::maxIDLength);

											UI_AddChatToHistory( strTemp, NULL, CLD_INFO, right );
										}
										//------------------------------------------------------
										// 정상
										//------------------------------------------------------
										else
										{
											if (g_pChatManager->IsAcceptMode())
											{
												g_pChatManager->AddID( pData );
											}
											else
											{
												g_pChatManager->RemoveID( pData );
											}

											char strTemp[128];
											sprintf(strTemp, (*g_pGameStringTable)[STRING_MESSAGE_CHAT_IGNORE].GetString(), pData);
											UI_AddChatToHistory( strTemp, NULL, CLD_INFO, right );

											// [도움말] 대화거부 한 명
//											__BEGIN_HELP_EVENT
////												ExecuteHelpEvent( HE_CHAT_REJECT_USER );	
//											__END_HELP_EVENT
										}
									}
								}
								//-------------------------------------------------------
								// 대화 허용
								//-------------------------------------------------------
								else if (strcmp(pCommand, STRING_ACCEPT)==0
										|| strcmp(pLwrCommand, STRING_ACCEPT_ENG)==0)
								{
									//-------------------------------------------------------
									// 모든 사람의 대화를 허용한다.
									//-------------------------------------------------------
									if (pData==NULL)
									{
										g_pChatManager->ClearID();
										g_pChatManager->SetAcceptMode();	

										char strTemp[128];
										sprintf(strTemp, (*g_pGameStringTable)[STRING_MESSAGE_CHAT_ACCEPT_ALL].GetString(), pData);
										UI_AddChatToHistory( strTemp, NULL, CLD_INFO, right );
									}
									//-------------------------------------------------------
									// 한 사람의 대화를 허용한다.
									//-------------------------------------------------------
									else
									{
										int nameLen = strlen( pData );

										//------------------------------------------------------
										// ID길이가 잘못된 경우										
										//------------------------------------------------------
										if (nameLen<PlayerInfo::minIDLength || nameLen>PlayerInfo::maxIDLength)
										{
											char strTemp[256];
											sprintf(strTemp, (*g_pGameStringTable)[STRING_USER_REGISTER_ID_LENGTH].GetString(), 
													PlayerInfo::minIDLength,
													nameLen>PlayerInfo::maxIDLength);

											UI_AddChatToHistory( strTemp, NULL, CLD_INFO, right );
										}
										//------------------------------------------------------
										// 정상
										//------------------------------------------------------
										else
										{
											if (g_pChatManager->IsAcceptMode())
											{
												g_pChatManager->RemoveID( pData );
											}
											else
											{	
												g_pChatManager->AddID( pData );
											}

											char strTemp[128];
											sprintf(strTemp, (*g_pGameStringTable)[STRING_MESSAGE_CHAT_ACCEPT].GetString(), pData);
											UI_AddChatToHistory( strTemp, NULL, CLD_INFO, right );
										}
									}
								}
								//-------------------------------------------------------
								// 나쁜말도 보기
								//-------------------------------------------------------
								else if (strcmp(pCommand, STRING_ACCEPT_CURSE)==0
										|| strcmp(pLwrCommand, STRING_ACCEPT_CURSE_ENG)==0)
								{
									g_pUserOption->FilteringCurse = FALSE;

									char strTemp[128];
									sprintf(strTemp, (*g_pGameStringTable)[STRING_MESSAGE_CHAT_ACCEPT_CURSE].GetString(), pData);
									UI_AddChatToHistory( strTemp, NULL, CLD_INFO, right );
								}
								//-------------------------------------------------------
								// 바른말만 보기
								//-------------------------------------------------------
								else if (strcmp(pCommand, STRING_FILTER_CURSE)==0
										|| strcmp(pLwrCommand, STRING_FILTER_CURSE_ENG)==0)
								{
									g_pUserOption->FilteringCurse = TRUE;

									char strTemp[128];
									sprintf(strTemp, (*g_pGameStringTable)[STRING_MESSAGE_CHAT_FILTER_CURSE].GetString(), pData);
									UI_AddChatToHistory( strTemp, NULL, CLD_INFO, right );
								}

								#if defined(OUTPUT_DEBUG) //&& defined(_DEBUG)
									//-------------------------------------------------------
									// 접속하기
									//-------------------------------------------------------
									else if (strcmp(pCommand, "connect")==0
											|| strcmp(pLwrCommand, "connect")==0)
									{
										CToken strToken(pData);

										// [IP]+[]+[Name]
										const char* pIP = strToken.GetToken();
										const char* pName = strToken.GetEnd();

										//RequestConnect( pIP, pName );
										if (g_pParty!=NULL)
										{
											PARTY_INFO* pInfo = g_pParty->GetMemberInfo( pName );

											if (pInfo!=NULL)
											{
												pInfo->IP = pIP;
											}
										}
									}
									//-------------------------------------------------------
									// 접속끊기
									//-------------------------------------------------------
									else if (strcmp(pCommand, "disconnect")==0
											|| strcmp(pLwrCommand, "disconnect")==0)
									{
										const char* pName = pData;
										//RequestDisconnect( pData );
										if (g_pParty!=NULL)
										{
											PARTY_INFO* pInfo = g_pParty->GetMemberInfo( pName );

											if (pInfo!=NULL)
											{
												pInfo->IP = "0.0.0.0";
											}
										}
									}
									//-------------------------------------------------------
									// Profile 요청 테스트
									//-------------------------------------------------------
									else if (strcmp(pCommand, "profile")==0
											|| strcmp(pLwrCommand, "profile")==0)
									{
										const char* pName = pData;

										if (!g_pProfileManager->HasProfile(pName)
											&& !g_pProfileManager->HasProfileNULL(pName))
										{
											g_pProfileManager->RequestProfile(pName);
										}
									}
									//-------------------------------------------------------
									// 말하기
									//-------------------------------------------------------
									else if (strcmp(pCommand, "say")==0
											|| strcmp(pLwrCommand, "say")==0)
									{
										if (g_pParty!=NULL
											&& g_pClientCommunicationManager!=NULL)
										{
											RCSay _RCSay;
											_RCSay.setName( g_pUserInformation->CharacterID.GetString() );
											_RCSay.setMessage( pData );
											_RCSay.setColor( right );
										
											for (int i=0; i<g_pParty->GetSize(); i++)
											{
												PARTY_INFO* pInfo = g_pParty->GetMemberInfo( i );
												
												if (pInfo!=NULL)										
												{
													int port = g_pClientConfig->CLIENT_COMMUNICATION_UDP_PORT;

													RequestUserInfo* pUserInfo = g_pRequestUserManager->GetUserInfo( pInfo->Name.GetString() );

													if (pUserInfo!=NULL)
													{
														if (pUserInfo->UDPPort!=0)
															port = pUserInfo->UDPPort;
													}

													g_pClientCommunicationManager->sendPacket( 
															pInfo->IP, 
															port, 
															&_RCSay );
												}
											}	
										}
									}
									//-------------------------------------------------------
									// npc 수 설정
									//-------------------------------------------------------
									else if (strcmp(pCommand, "npc")==0
											|| strcmp(pLwrCommand, "npc")==0)
									{
										g_MaxNPC = atoi( pData );
									}									
									//-------------------------------------------------------
									// ProfilerInfoName 설정
									//-------------------------------------------------------
									else if (strcmp(pCommand, "profileName")==0
											|| strcmp(pLwrCommand, "profilename")==0)
									{
										if (g_pProfiler!=NULL)
										{
											g_ProfileInfoName = pData;
										}
									}
									//-------------------------------------------------------
									// ProfilerInfoName 설정
									//-------------------------------------------------------
									else if (strcmp(pCommand, "profileClear")==0
											|| strcmp(pLwrCommand, "profileclear")==0)
									{
										if (g_pProfiler!=NULL)
										{
											g_pProfiler->Release();
										}
									}
									//-------------------------------------------------------
									// 감마 설정
									//-------------------------------------------------------
									else if (strcmp(pCommand, "gamma")==0
											|| strcmp(pLwrCommand, "gamma")==0)
									{
										const char* pValue = pData;

										int value = atoi( pValue );

										if (value==999)
										{
											CDirectDraw::RestoreGammaRamp();
										}
										else
										{
											CDirectDraw::SetGammaRamp( value );
										}
									}
									
								#endif
							}
						}
						break;

						case SYMBOL_MASTER_COMMAND:
						{
							static std::vector<std::string> vMasterCommand;
							static std::string	strTempCommand;

							char *pMessage = str;
							static bool bInvincible = false;

							if(strcmp(str, "*iddqd") == 0 || strcmp(str, "*Tnrrkt") == 0 || strcmp(str, "*쑥갓") == 0)
							{
								if(bInvincible == true)
									strTempCommand = "*command invincible off";
								else
									strTempCommand = "*command invincible on";
								bInvincible = !bInvincible;

								pMessage = const_cast<char*>(strTempCommand.c_str());
							}
							else if(strcmp(str, "*gaonashi") == 0 || strcmp(str, "*gaonasi") == 0 || strcmp(str, "*rkdhsktl") == 0 || strcmp(str, "*가오나시") == 0 || strcmp(str,"*ghost") == 0)
							{
								if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_GHOST))
									strTempCommand = "*command ghost off";
								else
									strTempCommand = "*command ghost on";
								pMessage = const_cast<char*>(strTempCommand.c_str());
							}
							else if(strcmp(str, "*goes") == 0 )
							{
								strTempCommand = "*warp 8000 75 77";
								pMessage = const_cast<char*>(strTempCommand.c_str());
							}
							else if(strncmp(str, "*mc", 3) == 0 && isdigit(str[4]))
							{
//								if(vMasterCommand.empty())
								{
									std::string filename = "MasterCommand";
									filename += str[4];
									filename += ".txt";
									if(!_access(filename.c_str(), 0))
									{
										FILE *fp;

										fp = fopen(filename.c_str(), "rt");

										if(fp != NULL)
										{
											char szTemp[512];
											while(fgets(szTemp, 512, fp))
											{
												if( strlen(szTemp) > 0 )
												{
													if(szTemp[strlen(szTemp)-1] == '\n')
													szTemp[strlen(szTemp)-1] = '\0';
													char *pszTemp = new char[strlen(szTemp)+1];
													strcpy( pszTemp, szTemp );
													
													Execute_UI_CHAT_RETURN(left, right, (void *)pszTemp);
												}

//												CGSay _CGSay;
//												_CGSay.setMessage( szTemp );	//pWansungString );
//												_CGSay.setColor( right );
//												g_pSocket->sendPacket( &_CGSay );
											}
										}
										else
										{
											DEBUG_ADD("[Master Command] File pointer is NULL.");
										}
									}
									else
									{
										DEBUG_ADD("[Master Command] File access is Denied.");
									}
								}
							}
							else if(strncmp(str, "*C2G", 4) == 0 )
							{
								if( strcmp(str+5, "on") == 0 )
								{
									g_C2G = 1;
								}
								else
								{
									g_C2G--;
								}
//								for( int grid_y = 0; grid_y < g_pInventory->GetHeight(); grid_y++ )
//								for( int grid_x = 0; grid_x < g_pInventory->GetWidth(); grid_x++ )
//								{
//									C_VS_UI_INVENTORY::AutoMove( grid_x, grid_y );
//									gpC_base->DispatchMessage();
//								}
							}
							else if( g_C2G > 0 && strncmp(str, "*create", 7 ) == 0 )
							{
								g_C2G++;
							}

//							{
//								std::string aaaa[4];
//								aaaa[0] = "*set war_active off";
//								aaaa[1] = "*command removeRaceWar";
//								aaaa[2] = "*set war_active on";
//								aaaa[3] = "*set war_active off";
//
//								CGSay _CGSay;
//
//								_CGSay.setColor( right );
//
//								for(int i=0;i<4;i++)
//								{
//									_CGSay.setMessage( aaaa[i].c_str() );									
//									g_pSocket->sendPacket( &_CGSay );
//								}
//							}

							bool bSay = true;
#ifndef __METROTECH_TEST
							if(g_pUserInformation->bCompetence == true && g_pUserInformation->bCompetenceShape != true)
							{
								if(g_pZoneTable->Get( g_pZone->GetID() )->CompetenceZone == true)
								{
								}
								else
								{
									if(strstr(pMessage, "*command ghost off") != NULL)
										bSay = false;
								}
							}
#endif

							if(bSay == true)
							{
								CGSay _CGSay;
								_CGSay.setMessage( pMessage );	//pWansungString );
								_CGSay.setColor( right );
								g_pSocket->sendPacket( &_CGSay );
								
//								UI_AddChatToHistory( pMessage, g_pUserInformation->CharacterID.GetString(), CLD_NORMAL, right );
							}
							break;
						}

						//------------------------------------------------------------
						//
						//	일반 대화
						//
						//------------------------------------------------------------
						default :
						{
							//------------------------------
							// 숨어 있을때는 말 못한다.
							// 늑대나 박쥐인 경우 말 못한다.
							//------------------------------
							if (g_pPlayer->IsUndergroundCreature()
								|| g_pPlayer->IsInCasket()
								|| g_pPlayer->GetCreatureType()==CREATURETYPE_BAT
								|| g_pPlayer->GetCreatureType()==CREATURETYPE_WOLF
								|| g_pPlayer->GetCreatureType()==CREATURETYPE_WER_WOLF)
							{
								// history에 추가
								//char temp[128];
								//sprintf(temp, "[%s] .....", g_pUserInformation->CharacterID.GetString());
								//UI_AddChatToHistory( temp );
								UI_AddChatToHistory( ".....", g_pUserInformation->CharacterID.GetString(), CLD_NORMAL, right );
							}
							else
							{
								CGSay _CGSay;
								_CGSay.setMessage( str );	//pWansungString );
								_CGSay.setColor( right );
								g_pSocket->sendPacket( &_CGSay );

									
								// Player의 Chat pWansungStringing에 추가
								g_pPlayer->SetChatString( str, right );

								// history에 추가
								//char temp[256];
								//sprintf(temp, "%s> %s", g_pUserInformation->CharacterID.GetString(), str);
								//UI_AddChatToHistory( temp );
								UI_AddChatToHistory( str, g_pUserInformation->CharacterID.GetString(), CLD_NORMAL, right );
							}
						}	
						
						//delete [] str;
						//delete [] pWansungString;
					}
				}
			}

			delete [] strOrg;
		}					
	}

	DeleteNewArray(void_ptr);

//	__BEGIN_HELP_EVENT
		ExecuteHelpEvent( HELP_EVENT_CHAT );
//	__END_HELP_EVENT

}


//-----------------------------------------------------------------------------
//
// Logout
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_LOGOUT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_LOGOUT");

	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}
	
	if (g_pPlayer->IsWaitVerify())
	{
		DEBUG_ADD_FORMAT("WaitVerify %d", g_pPlayer->GetWaitVerify());
		return;
	}
	
	//-------------------------------------------------------------------
	// Logout 시간이 설정되어 있지 않으면 설정하고
	// 설정되어있다면 남은 시간을 출력해준다.
	//-------------------------------------------------------------------
	int zoneID	= (g_bZonePlayerInLarge?g_nZoneLarge : g_nZoneSmall);

	//ZONETABLE_INFO* pZoneInfo = g_pZoneTable->Get( zoneID );
	
	//-------------------------------------------------------------------
	// 안전지대면 바로 logout시킨다.
	// 자기 혼자 있을때..
	//-------------------------------------------------------------------
	if (g_bZoneSafe //pZoneInfo!=NULL	&& pZoneInfo->Safety
		// 안전지대 타일이면..
		|| g_pPlayer->IsInSafeSector()
		|| g_pZone->GetID() == 8000				// 이벤트경기장 大면 바로 종료
		// 죽어서 몇초 지났으면...
//		|| g_pPlayer->IsDead() && g_pPlayer->GetDeadDelayLast() < 4)
		//|| g_pZone->GetCreatureNumber()==1)
#ifdef __METROTECH_TEST__
		|| 1
#endif
		)
	{
		ExecuteLogout();
	}
	//-------------------------------------------------------------------
	// 아니면 5초후 logout 시킨다.
	//-------------------------------------------------------------------
	else if (g_pPlayer!=NULL && g_pPlayer->IsAlive())
	{		
		if (g_pUserInformation->LogoutTime == 0)
		{
			// 5초 후 강제 Logout 시킨다.
			
			#ifdef _DEBUG
				g_pUserInformation->LogoutTime = g_CurrentTime + 2000;
			#else
				g_pUserInformation->LogoutTime = g_CurrentTime + 5000;
			#endif

			MEvent event;
			event.eventID = EVENTID_LOGOUT;
			event.eventDelay = 5000;
			event.eventFlag = EVENTFLAG_SHOW_DELAY_STRING;
			event.eventType = EVENTTYPE_ZONE;
			event.m_StringsID.push_back(STRING_MESSAGE_LOGOUT_AFTER_SECOND);

			g_pEventManager->AddEvent(event);

			g_pPlayer->SetWaitVerify(MPlayer::WAIT_VERIFY_LOGOUT);
//			g_pSystemMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_LOGOUT_AFTER_SECOND].GetString(), 5);
		}
		else if (g_pUserInformation->LogoutTime > g_CurrentTime)
		{
			DWORD sec = (g_pUserInformation->LogoutTime - g_CurrentTime) / 1000;

			if (sec > 0)
			{
				MEvent event;
				event.eventID = EVENTID_LOGOUT;
				event.eventDelay = sec*1000;
				event.eventFlag = EVENTFLAG_SHOW_DELAY_STRING;
				event.eventType = EVENTTYPE_ZONE;
				event.m_StringsID.push_back(STRING_MESSAGE_LOGOUT_AFTER_SECOND);
				
				g_pEventManager->AddEvent(event);
				g_pPlayer->SetWaitVerify(MPlayer::WAIT_VERIFY_LOGOUT);

//				g_pSystemMessage->AddFormat((*g_pGameStringTable)[STRING_MESSAGE_LOGOUT_AFTER_SECOND].GetString(), sec);
			}
		}

		gC_vs_ui.HotKey_ESC();
	}
	//-------------------------------------------------------------------
	// 죽은 경우..
	//-------------------------------------------------------------------
	else
	{
		g_pSystemMessage->Add( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_LOGOUT_DIED].GetString() );

		gC_vs_ui.HotKey_ESC();
	}	
}


//-----------------------------------------------------------------------------
//
// Skill이 선택되었을 때
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_SELECT_SKILL(int left, int right, void* void_ptr)
{
	DEBUG_ADD_FORMAT("[UI] UI_SELECT_SKILL(%d)", left);

	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	//
	// dw_left = Skill ID
	//
	if (g_pPlayer!=NULL)
	{
		//left = SKILL_OUSTERS_COMBO ; 
		g_pPlayer->SetSpecialActionInfo( left );
		
		// 반복동작중에 actionCount가 이상해지는 경우가 있어서
		// 이거 해야되는데.. 기술쓸때 답답하다는 이유로.. 일단.. - -;
		//g_pPlayer->UnSetRepeatAction();
	}

	// [도움말] Skill icon바뀔 때
//	__BEGIN_HELP_EVENT
////		ExecuteHelpEvent( HE_SKILL_ICON_CHANGE );
//	__END_HELP_EVENT
}

//-----------------------------------------------------------------------------
//
// Skill 선택 취소
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CANCEL_SELECT_SKILL(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CANCEL_SELECT_SKILL");

	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	//
	// Skill 선택 취소.
	//
	if (g_pPlayer!=NULL)
	{
		g_pPlayer->SetSpecialActionInfoNULL();
	}
}

// 해당 크리쳐의 주변에 원하는 시체가 있는지 검사한다.
bool IsExistCorpseFromPlayer(MCreature* OriginCreature, int creature_type)
{
	int sx, sy;

	sx = OriginCreature->GetX();
	sy = OriginCreature->GetY();

	for(int i=-1;i<=1;i++)
	{
		for(int j=-1;j<=1;j++)
		{
			if( sx+i < 0 || sy + j < 0 || sx+i >= g_pZone->GetWidth() || sy+j >= g_pZone->GetHeight() )
				continue;

			const MSector& sector = g_pZone->GetSector( sx+i, sy+j );
			MItem* pItem = sector.GetItem();
			
			if( pItem != NULL && pItem->GetItemClass() == ITEM_CLASS_CORPSE)
			{
				MCreature* pCreature = (dynamic_cast<MCorpse*>(pItem))->GetCreature();
				if( pCreature != NULL && pCreature->GetCreatureType() == creature_type )
					return true;
			}
		}
	}
	return false;
}


//-----------------------------------------------------------------------------
//
// mouse에서 Zone으로 Item을 떨어뜨릴 때,
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_DROP_TO_CLIENT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_DROP_TO_CLIENT");

	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}

	MItem* pItem = UI_GetMouseItem();//(MItem*)void_ptr;		

	// Capture The Flag 용 하드코딩
	// 나를 주변으로 3타일을 검사한다.		
	// 깃발 아이템이 아니면 검사한다.

	if(!( pItem != NULL && pItem->GetItemClass() == ITEM_CLASS_EVENT_ITEM && pItem->GetItemType() == 27 ) )
	{
		if( IsExistCorpseFromPlayer( dynamic_cast<MCreature*>( g_pPlayer ), 670 ) )
		{
			g_pSystemMessage->Add( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_DROP_ITEM_BY_FLAG].GetString() );
			return;
		}
	}
				
	if (!g_bWatchMode
		// 교환 중에는 버릴 수 없다.
		&& !UI_IsRunningExchange()
		// 교환 창이 뜬 후.. 일정 시간 동안은 버릴 수 없다.
		&& g_pUserInformation->ItemDropEnableTime < g_CurrentTime)
	{
		//void_ptr = MItem *

		if( pItem != NULL && 
				(
					gC_vs_ui.IsRunningRemoveOptionFromRareItem() && pItem->GetItemClass() == ITEM_CLASS_MIXING_ITEM 
				)
			)
//			pItem != NULL && pItem->GetItemClass() == ITEM_CLASS_QUEST_ITEM && pItem->GetItemType() == 4 )
			return;
				
		if (pItem != NULL && (pItem->GetItemClass() == ITEM_CLASS_RELIC ||
			pItem->GetItemClass() == ITEM_CLASS_BLOOD_BIBLE || 
			pItem->GetItemClass() == ITEM_CLASS_CASTLE_SYMBOL ||
			pItem->GetItemClass() == ITEM_CLASS_EVENT_ITEM && (pItem->GetItemType() == 27 || pItem->GetItemType() == 31) ||
			pItem->GetItemClass() == ITEM_CLASS_SWEEPER || pItem->GetItemClass() == ITEM_CLASS_PET_FOOD
			))
		{
			// 성물을 성물 보관대에 클릭하면 ??-_-;
			int CreatureID = g_pTopView->GetSelectedCreature();

			MCreature *pCreature = NULL;
			bool		bCorpse = false;

			if(CreatureID == -1)
			{
				CreatureID = g_pTopView->GetSelectedItemID();
				MItem *selectedItem = g_pZone->GetItem(CreatureID);
				if(selectedItem != NULL)
				{
					if(selectedItem->GetItemClass() == ITEM_CLASS_CORPSE)
					{
						MCorpse *pCorpse = (MCorpse *)selectedItem;
						pCreature = pCorpse->GetCreature();
						bCorpse = true;
					}
				}
			}
			else
			{
//				((g_pPlayer->IsSlayer() && pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 373)
//				|| (g_pPlayer->IsVampire() && pCreature->GetCreatureType() >= 374 && pCreature->GetCreatureType() <= 376))
				pCreature = g_pZone->GetCreature(CreatureID);
			}

			if(g_pPlayer->IsItemCheckBufferNULL() && pCreature != NULL)
			{
				if(bCorpse)	// 시체에 아이템을 넣쟈
				{
					if(pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376 || 
						pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563 ||
						pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549 ||
						pCreature->GetCreatureType() == 670	&& pItem->GetItemClass() == ITEM_CLASS_EVENT_ITEM || // 깃발일경우
						pCreature->GetCreatureType() == 672 && pItem->GetItemClass() == ITEM_CLASS_SWEEPER
					)
					{
						if(abs(g_pPlayer->GetX()-pCreature->GetX()) < 3 && abs(g_pPlayer->GetY()-pCreature->GetY()) < 3)
						{
							CGRelicToObject _CGRelicToObject;
							_CGRelicToObject.setItemObjectID(pItem->GetID());
							_CGRelicToObject.setObjectID(CreatureID);
							g_pSocket->sendPacket( &_CGRelicToObject );
							g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_DROP_TO_RELICTABLE );
						}
						else
						{
							g_pSystemMessage->Add((*g_pGameStringTable)[UI_STRING_MESSAGE_TOO_FAR].GetString());
						}
						return;
					}
				}
				else	// 몬스터에 아이템을 넣쟈
				{
					if(pItem->GetItemClass() == ITEM_CLASS_PET_FOOD &&
						pCreature->GetCreatureType() == 687 && pItem->GetNumber() == 1)	// 나중에 크리쳐 타입을 넣어주쟈
					{
						if(abs(g_pPlayer->GetX()-pCreature->GetX()) < 3 && abs(g_pPlayer->GetY()-pCreature->GetY()) < 3)
						{
							CGTameMonster _CGTameMonster;
							_CGTameMonster.setObjectID(CreatureID);
							g_pSocket->sendPacket( &_CGTameMonster );
							g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_DROP_TO_CREATURE );
						}
						else
						{
							g_pSystemMessage->Add((*g_pGameStringTable)[UI_STRING_MESSAGE_TOO_FAR].GetString());
						}
						return;
					}
					// 퀘스트용 생명의 나선 아이템 일 때
					else if(pItem->GetItemClass() == ITEM_CLASS_EVENT_ITEM && pItem->GetItemType() == 31 &&
						(pCreature->GetCreatureType() == 793 || pCreature->GetCreatureType() == 794 || pCreature->GetCreatureType() == 795))
					{
						CGRelicToObject _CGRelicToObject;
						_CGRelicToObject.setItemObjectID(pItem->GetID());
						_CGRelicToObject.setObjectID(CreatureID);
						g_pSocket->sendPacket( &_CGRelicToObject );
						g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_DROP_TO_RELICTABLE );
						pCreature->RemoveCauseCriticalWoundsEffect();

					}
				}
			}

		}

		//-----------------------------------------------------------------
		// 검증받을 게 없는 경우
		//-----------------------------------------------------------------
		if (pItem!=NULL 
			&& !pItem->IsQuestItem()
			&& g_pPlayer->IsItemCheckBufferNULL()
			// 빨간색 Event GiftBox 아이템인 경우 못 버린다.
			&& !(pItem->GetItemClass()==ITEM_CLASS_EVENT_GIFT_BOX&& pItem->GetItemType()==1)
			&& !(pItem->GetItemClass()==ITEM_CLASS_COUPLE_RING)
			&& !(pItem->GetItemClass()==ITEM_CLASS_VAMPIRE_COUPLE_RING 
			&& (pItem->GetItemClass() == ITEM_CLASS_CODE_SHEET && !gC_vs_ui.IsRunningQuestInventory() ) )
			&& !(pItem->GetItemClass() == ITEM_CLASS_LUCKY_BAG && pItem->GetItemType() == 3)
			&& !(pItem->GetItemClass() == ITEM_CLASS_EVENT_ITEM && (pItem->GetItemType() == 28||pItem->GetItemType() == 31))
			

			// sjheon 풍선 머리띠  이벤트 아이템인 경우 못 버린다. 2005.05.02 Add
			//&& (pItem->GetItemClass() == ITEM_CLASS_EVENT_ETC && (pItem->GetItemType() == 18))		// 패밀리 코인 
			&& !(pItem->GetItemClass() == ITEM_CLASS_EVENT_ITEM && (pItem->GetItemType() >= 32 && pItem->GetItemType() <= 36))		// 풍선 머리띠 
			&& !(pItem->GetItemClass() == ITEM_CLASS_SLAYER_PORTAL_ITEM) 		// 무전기 
			// sjheon 봄나들이 이벤트 아이템인 경우 못 버린다. 2005.05.02 End 
			)
		{
			//-----------------------------------------------------------------------
			// 교환중이 아닐 때만 떨어뜨린다.
			//-----------------------------------------------------------------------
			if (g_pTradeManager==NULL)
			{
					//---------------------------------------------------
					// item을 가지고 검증받을 일이 없는 경우..
					//---------------------------------------------------
					//if (g_pPlayer->IsItemCheckBufferNULL())
					{
						//---------------------------------------------------
						// Server로 item을 떨어뜨린다는 packet을 보낸다.
						// 실제로 떨어지는 것은.. 
						// server에서 item을 zone으로 추가한 경우이다.
						//---------------------------------------------------
						CGAddMouseToZone _CGAddMouseToZone;
						_CGAddMouseToZone.setObjectID( pItem->GetID() );

						g_pSocket->sendPacket( &_CGAddMouseToZone );

						
						//---------------------------------------------------
						// 이 부분은.. 나중에 AddNewItem을 받고 해야하는게 아닐까..
						// timing 문제가 애매하다.
						//---------------------------------------------------
						// mouse에서 item을 없애고
						//gC_vs_ui.DropItem();

						// buffer에 item을 저장해둔다. (새로운 item을 생성한다)
						g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_DROP_TO_ZONE );

						// item을 삭제한다.
						//delete pItem;
					}

			}
			//-----------------------------------------------------------------------
			// 교환중일때는 아이템을 떨어뜨리지 않는다.
			//-----------------------------------------------------------------------
			else
			{
				DEBUG_ADD("Cannot Drop Item to Zone while Trading.");
			}
		}
		//-----------------------------------------------------------------
		// 검증 받아야할 다른 아이템이 있는 경우
		//-----------------------------------------------------------------
		else
		{
			DEBUG_ADD( "[Error] There is another item in ItemCheckBuffer or Mouse NULL");
		}
	}
	else
	{
		DEBUG_ADD( "Can't Drop");
	}
}


//-----------------------------------------------------------------------------
//
// mouse의 Item을 Inventory에 추가한 경우
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_DROP_TO_INVENTORY(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_DROP_TO_INVENTORY");
	
	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}
	//
	// left = grid_x, right = grid_y
	// void_ptr = MItem *		// 추가된 item
	//							// bAcceptMyTrade이면 inventory의 item이다.
	//
	MItem* pItem = (MItem*)void_ptr;		// inventory에 있는 아이템(NULL일 수도 있다)
	MItem* pMouseItem = UI_GetMouseItem();	// 현재 mouse의 item

	// pMouseItem을 pItem과 바꾸던지..
	// pMouseItem이 그냥 들어가던지...
	
	BOOL bAcceptMyTrade = (g_pTradeManager!=NULL && g_pTradeManager->IsAcceptMyTrade());
	BOOL bAcceptOtherTrade = (g_pTradeManager!=NULL && g_pTradeManager->IsAcceptOtherTrade());

	//-----------------------------------------------------------------
	// 검증받을게 없는 경우
	//-----------------------------------------------------------------
	if (g_pPlayer->IsItemCheckBufferNULL() && pMouseItem!=NULL
		&& g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{		
		BOOL bSendPacketTradeRemove = FALSE;	// 기존에꺼 제거
		BOOL bSendPacket = TRUE;				// mouse --> inventory
		BOOL bSendPacketTradeAdd = FALSE;		// 추가된거 교환할걸로 설정
		
		TYPE_OBJECTID removeItemID = OBJECTID_NULL;
		TYPE_OBJECTID toInventoryItemID = (pMouseItem==NULL)? OBJECTID_NULL : pMouseItem->GetID();

		//---------------------------------------------------
		// 교환 상태에서는 검증받아야 한다.
		//---------------------------------------------------
		if (bAcceptMyTrade)
		{
			if (g_pTempInformation->GetMode() == TempInformation::MODE_NULL
				&& g_pPlayer->IsItemCheckBufferNULL())
			{	
				g_pTempInformation->SetMode(TempInformation::MODE_TRADE_VERIFY_DROP_TO_INVENTORY);
				g_pTempInformation->Value1 = pMouseItem->GetID();
				g_pTempInformation->Value2 = left;
				g_pTempInformation->Value3 = right;
				g_pTempInformation->pValue = pMouseItem;

				g_pPlayer->SetItemCheckBuffer( pMouseItem, MPlayer::ITEM_CHECK_BUFFER_DROP_TO_INVENTORY );

				//toInventoryItemID = pMouseItem->GetID();

				//-------------------------------------------------------------
				// inventory에 있는 item이 교환할려고 선택된 것이면 제거한다.
				//-------------------------------------------------------------
				if (pItem!=NULL && pItem->IsTrade())
				{
					removeItemID = pItem->GetID();				
					
					bSendPacketTradeRemove = TRUE;
				}

				//bSendPacketTradeAdd = TRUE;
			}
			else
			{
				bSendPacket = FALSE;
			}
		}	
		//---------------------------------------------------
		// 놓은 아이템이 교환할려고 선택된 것인 경우
		//---------------------------------------------------
		else if (gC_vs_ui.IsRunningExchange())
		{
			//---------------------------------------------------
			// 기존에 있던 아이템 교환할 것에서 제거..
			//---------------------------------------------------
			if (pItem!=NULL && pItem->IsTrade())
			{
				removeItemID = pItem->GetID();

				bSendPacketTradeRemove = TRUE;
			}

			//---------------------------------------------------
			// 교환에서 선택된 아이템
			//---------------------------------------------------
			if (pMouseItem->IsTrade())
			{
				bSendPacketTradeAdd = TRUE;

				// 다른 사람이 OK누른 상태라면..
				if (bAcceptOtherTrade)
				{
					g_pTradeManager->RefuseOtherTrade();
				}
			}
			//---------------------------------------------------
			// 아니면.. 수동으로 OK취소를 보낸다.
			//---------------------------------------------------
			else
			{
				/*
				if (bAcceptMyTrade)
				#ifdef CONNECT_SERVER
					CGTradeFinish _CGTradeFinish;
					_CGTradeFinish.setTargetObjectID( g_pTradeManager->GetOtherID() );
					_CGTradeFinish.setCode( CG_TRADE_FINISH_RECONSIDER );

					g_pSocket->sendPacket( &_CGTradeFinish );
				#endif
				*/
			}
		}

		//---------------------------------------------------------
		// SendPacket - Trade Remove Item
		//---------------------------------------------------------
		if (bSendPacketTradeRemove)
		{
				CGTradeRemoveItem _CGTradeRemoveItem;
				_CGTradeRemoveItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
				_CGTradeRemoveItem.setItemObjectID( removeItemID );

				g_pSocket->sendPacket( &_CGTradeRemoveItem );

				g_pTradeManager->SetNextAcceptTime();
		}

		//---------------------------------------------------
		// packet을 보낼 필요가 있는 경우
		//---------------------------------------------------
		if (bSendPacket)
		{

				//---------------------------------------------------
				// mouse에 있던 item을 Inventory에 추가했다고
				// server로 packet을 보낸다.
				// pItem의 grid좌표가 inventory에서의 좌표이다.
				//---------------------------------------------------
				CGAddMouseToInventory _CGAddMouseToInventory;
				_CGAddMouseToInventory.setObjectID( toInventoryItemID );
				_CGAddMouseToInventory.setInvenX( left );//pItem->GetGridX() );
				_CGAddMouseToInventory.setInvenY( right );//pItem->GetGridY() );

				g_pSocket->sendPacket( &_CGAddMouseToInventory );

		}

		//---------------------------------------------------
		// 교환에서 선택하는 아이템...
		//---------------------------------------------------
		if (bSendPacketTradeAdd)
		{
				CGTradeAddItem _CGTradeAddItem;
				_CGTradeAddItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
				_CGTradeAddItem.setItemObjectID( pMouseItem->GetID() );

				g_pSocket->sendPacket( &_CGTradeAddItem );
				
		}

		//---------------------------------------------------
		// 실제로 처리한다.
		//---------------------------------------------------
		if (!bAcceptMyTrade)
		{
			MItem* pOldItem = NULL;
			if (g_pInventory->ReplaceItem(pMouseItem,		// 추가할 item
												left, right,	// 추가할 위치 
												pOldItem))		// 원래있던 item
			{
				if (pOldItem != NULL) // replace 되었는가?
				{				
					UI_PickUpItem( pOldItem );
				}
				else
				{
					UI_DropItem();
				}
			}

			// 크리스마스 트리용 하드코딩
			if(pMouseItem->GetItemClass() == ITEM_CLASS_EVENT_TREE 
				&& pMouseItem->GetItemType() != 12
				&& pMouseItem->GetItemType() != 25
				&& pMouseItem->GetItemType() != 26
				&& pMouseItem->GetItemType() != 27
				&& pMouseItem->GetItemType() != 28
				&& pMouseItem->GetItemType() != 41
				)
			{
				int mx,my;
				
				
				if(pMouseItem->GetItemType() < 12)
				{
					my = pMouseItem->GetItemType()/3;
					mx = pMouseItem->GetItemType()%3;
				}
				else if( pMouseItem->GetItemType() < 25 )
				{
					my = (pMouseItem->GetItemType()-13)/3;
					mx = (pMouseItem->GetItemType()-13)%3;
				} 
				else if( pMouseItem->GetItemType() < 41 )
				{
					my = (pMouseItem->GetItemType()-29)/3;
					mx = (pMouseItem->GetItemType()-29)%3;
				}
				const int pointX = pMouseItem->GetGridX() - mx;
				const int pointY = pMouseItem->GetGridY() - my;
				
				bool bTree = true;

				for(int y = 0; y < 4; y++)
				{
					for(int x = 0; x < 3; x++)
					{
						MItem *pPartOfTree = g_pInventory->GetItem(x+pointX, y+pointY);
						if(pPartOfTree != NULL && 
							pPartOfTree->GetItemClass() == ITEM_CLASS_EVENT_TREE &&
							(pPartOfTree->GetItemType() == y*3+x ||
							pPartOfTree->GetItemType() == y*3+x + 13 ||
							pPartOfTree->GetItemType() == y*3+x + 29) )
						{
						}
						else
						{
							bTree = false;
						}
					}
				}

				if(bTree)
					g_pPlayer->SetItemCheckBuffer(NULL, MPlayer::ITEM_CHECK_BUFFER_TREE_MERGE);
			}
		}
	}
	//-----------------------------------------------------------------
	// 검증 받아야할 다른 아이템이 있는 경우
	//-----------------------------------------------------------------
	else
	{
		DEBUG_ADD( "[Error] There is another item in ItemCheckBuffer or Mouse NULL");		

		if(!g_pPlayer->IsItemCheckBufferNULL())
			DEBUG_ADD_FORMAT("[ITEMDROP] !g_pPlayer->IsItemCheckBufferNULL() %d", g_pPlayer->GetItemCheckBufferStatus());
		if(pMouseItem == NULL)
			DEBUG_ADD("[ITEMDROP] pMouseItem == NULL)");
		if(g_pTempInformation->GetMode()!=TempInformation::MODE_NULL)
			DEBUG_ADD_FORMAT("[ITEMPDROP] g_pTempInformation->Mode!=TempInformation::MODE_NULL %d", g_pTempInformation->Mode);
	}

	// 지워야 되는 code이다. *_*;
	//gC_vs_ui.DropItem();
}

//-----------------------------------------------------------------------------
//
// QuickSlot에 Item을 넣은 경우
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_DROP_TO_QUICKSLOT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_DROP_TO_QUICKSLOT");

	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}
	
	MOustersArmsBand* pQuickSlot = NULL;

	int slot = left;
	
	if( g_pPlayer->IsOusters() )
	{
		if( g_pArmsBand1 == NULL )
			pQuickSlot = g_pArmsBand2;
		else
		{
			if( g_pArmsBand1->GetPocketNumber() > slot )
				pQuickSlot = g_pArmsBand1;
			else				
			{
				pQuickSlot = g_pArmsBand2;
				slot -= g_pArmsBand1->GetPocketNumber();
			}
		}

		if( pQuickSlot == NULL )
			return;
	}

	//
	// left = grid_x
	// void_ptr = MItem *
	//
	//---------------------------------------------------
	// 지금 UI에서는 바로 추가하거나..
	// 바로 교환해버린다.
	// 그러므로, 두 개를 바꿔서 읽어야 한다. - -;
	// 였으나 클라이언트에서 처리해주는걸로 바꿈 by larosel
	//---------------------------------------------------
	MItem* pSlotItem = NULL ;// = (MItem*)g_pQuickSlot->GetItem( left );;//gpC_mouse_pointer->GetPickUpItem();
	if( g_pPlayer->IsSlayer() )
		pSlotItem = (MItem*)g_pQuickSlot->GetItem( left );
	else
		pSlotItem = (MItem*)pQuickSlot->GetItem( slot );
	
	MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();//g_pQuickSlot->GetItem( left );
	
	bool	Replace = FALSE;
	
	if( g_pPlayer->IsSlayer() )
		Replace = g_pQuickSlot->ReplaceItem( pMouseItem, left, pSlotItem );
	else
		Replace = pQuickSlot->ReplaceItem( pMouseItem, slot, pSlotItem );

	if (Replace)
	{
		if (pSlotItem) // replace 되었는가?
		{
			gpC_mouse_pointer->PickUpItem((MItem *)pSlotItem);
		}
		else
		{
			// not replace, just drop at empty grid.
			gpC_mouse_pointer->DropItem();
		}
	}
	
	//-----------------------------------------------------------------
	// 검증받을게 없는 경우
	//-----------------------------------------------------------------
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)//g_pPlayer->IsItemCheckBufferNULL())
	{
		// 무조건 packet을 보내면 된다.
		if (1)//g_pQuickSlot->GetItem( left )==NULL)
		{

				//MItem* pItem = (MItem*)void_ptr;

				//---------------------------------------------------
				// mouse에 있던 item을 QuickSlot에 추가했다고
				// server로 packet을 보낸다.
				//---------------------------------------------------
				CGAddMouseToQuickSlot _CGAddMouseToQuickSlot;
				_CGAddMouseToQuickSlot.setObjectID( pMouseItem->GetID() );
				
				if( g_pPlayer->IsSlayer() )
					_CGAddMouseToQuickSlot.setSlotID( left );//pItem->GetItemSlot() );
				else
				{
					if( pQuickSlot == g_pArmsBand2 )
						_CGAddMouseToQuickSlot.setSlotID( slot + 3 );
					else
						_CGAddMouseToQuickSlot.setSlotID( slot );
				}
				
				g_pSocket->sendPacket( &_CGAddMouseToQuickSlot );
		
				//---------------------------------------------------
				// UI에서 하기 때문에.. 하면 안된다.
				//---------------------------------------------------
				// 바로 QuickSlot에 추가한다.
				//g_pQuickSlot->AddItem( pItem, left );
				
				//UI_DropItem();

			PlaySound( pMouseItem->GetInventorySoundID() );
		}
		//---------------------------------------------------
		// 뭔가 이미 있는 경우.. 
		//---------------------------------------------------
		else
		{						
			DEBUG_ADD_FORMAT("[Error] Can't add item to quickslot, already exist slot=%d", left);
		}
	}
	//-----------------------------------------------------------------
	// 검증 받아야할 다른 아이템이 있는 경우
	//-----------------------------------------------------------------
	else
	{
		DEBUG_ADD( "[Error] There is another item in ItemCheckBuffer");
	}
}


//-----------------------------------------------------------------------------
//
// Gear에 Item을 착용한 경우
//
//-----------------------------------------------------------------------------				
void
UIMessageManager::Execute_UI_ITEM_DROP_TO_GEAR(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ADD_ITEM_TO_GEAR");
	
	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}
	//
	// left = slot enum
	// void_ptr = MItem * 교환될 item(NULL일수 있다.)
	//

	if (!g_bWatchMode)
	{
		//-----------------------------------------------------------------
		// 검증받을게 없는 경우
		//-----------------------------------------------------------------
		if (g_pPlayer->IsItemCheckBufferNULL()
			&& g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
		{
			MItem* pMouseItem = (MItem*)gpC_mouse_pointer->GetPickUpItem();	// 들고 있는 item
			MItem* pGearItem = (MItem*)void_ptr;	// gear에 있다면.. 교환될 아이템
			
			//-----------------------------------------------------------------
			// mouse에 뭔가를 들고 있어야 한다.
			//-----------------------------------------------------------------
			if (pMouseItem!=NULL)
			{
				//-----------------------------------------------------------------
				// 착용할 수 있는 상황이 되면..
				//-----------------------------------------------------------------
				if (g_pPlayer->SetAddonItem( pMouseItem ))
				{				
					MItem* pRemovedItem = NULL;

					// mouse --> gear
					switch(g_pPlayer->GetRace())
					{
					case RACE_SLAYER:
						g_pSlayerGear->ReplaceItem( pMouseItem, (MSlayerGear::GEAR_SLAYER)left, pRemovedItem );
						
						//-----------------------------------------------------------
						// belt를 놓거나 집었을 경우는 quick item을 reset해야 한다.
						//-----------------------------------------------------------
						if ((MSlayerGear::GEAR_SLAYER)left == MSlayerGear::GEAR_SLAYER_BELT)
						{
							gC_vs_ui.ResetSlayerQuickItemSize();
						}
						break;

					case RACE_VAMPIRE:
						g_pVampireGear->ReplaceItem( pMouseItem, (MVampireGear::GEAR_VAMPIRE)left, pRemovedItem );
						break;

					case RACE_OUSTERS:
						g_pOustersGear->ReplaceItem( pMouseItem, (MVampireGear::GEAR_VAMPIRE)left, pRemovedItem );
						break;
					}

					UI_DropItem();

					//-----------------------------------------------------------
					// 교환되는 경우
					//-----------------------------------------------------------
					if (pRemovedItem!=NULL)
					{
						UI_PickUpItem( pRemovedItem );
					}								

					//----------------------------------------------------
					// Skill 체크
					//----------------------------------------------------
					g_pSkillAvailable->SetAvailableSkills();

						//---------------------------------------------------
						// mouse에 있던 item1을 Gear에 추가했다고
						// server로 packet을 보낸다.
						//---------------------------------------------------
						CGAddMouseToGear _CGAddMouseToGear;
						_CGAddMouseToGear.setObjectID( pMouseItem->GetID() );
						_CGAddMouseToGear.setSlotID( pMouseItem->GetItemSlot() );//pItem->GetItemSlot() );
						
						g_pSocket->sendPacket( &_CGAddMouseToGear );
						

					//----------------------------------------------------
					// 착용한 아이템에 따른 도움말
					//----------------------------------------------------
//					__BEGIN_HELP_EVENT
//						ITEM_CLASS itemClass = pMouseItem->GetItemClass();
//
//						//----------------------------------------------------
//						// 총
//						//----------------------------------------------------
//						if (pMouseItem->IsGunItem())
//						{
//							// [도움말] 총 착용한 경우
////							ExecuteHelpEvent( HE_ITEM_WEAR_GUN );
//						}
//						//----------------------------------------------------
//						// 벨트
//						//----------------------------------------------------
//						else if (itemClass==ITEM_CLASS_BELT)
//						{
//							// [도움말] 벨트 착용한 경우
////							ExecuteHelpEvent( HE_ITEM_WEAR_BELT );
//						}
//						//----------------------------------------------------
//						// 검
//						//----------------------------------------------------
//						else if (itemClass==ITEM_CLASS_SWORD)
//						{
//							// [도움말] 검 착용한 경우
////							ExecuteHelpEvent( HE_ITEM_WEAR_SWORD );
//						}
//						//----------------------------------------------------
//						// 도
//						//----------------------------------------------------
//						else if (itemClass==ITEM_CLASS_BLADE)
//						{
//							// [도움말] 도 착용한 경우
////							ExecuteHelpEvent( HE_ITEM_WEAR_BLADE );
//						}
//						//----------------------------------------------------
//						// 십자가
//						//----------------------------------------------------
//						else if (itemClass==ITEM_CLASS_CROSS)
//						{
//							// [도움말] 십자가 착용한 경우
////							ExecuteHelpEvent( HE_ITEM_WEAR_CROSS );
//						}
//						//----------------------------------------------------
//						// 메이스
//						//----------------------------------------------------
//						else if (itemClass==ITEM_CLASS_MACE)
//						{
//							// [도움말] 메이스 착용한 경우
////							ExecuteHelpEvent( HE_ITEM_WEAR_MACE );
//						}				
//					__END_HELP_EVENT
				}		
			}
			else
			{
				DEBUG_ADD( "[Error] Mouse Item is NULL" );
			}
		}
		//-----------------------------------------------------------------
		// 검증 받아야할 다른 아이템이 있는 경우
		//-----------------------------------------------------------------
		else
		{
			DEBUG_ADD( "[Error] There is another item in ItemCheckBuffer");
		}
	}
	else
	{
		DEBUG_ADD( "[Watch Mode]");
	}

	//-------------------------------------------------
	// Player가 정지해 있는 경우에만 가능하다..
	//-------------------------------------------------
	//if (g_pPlayer->IsStop())
	{
		//-----------------------------------------
		// player가 pItem을 장착한다.
		//-----------------------------------------
		//g_pPlayer->SetAddonItem( pItem );

		//-----------------------------------------
		// UI에서 Item을 Gear에 장착한다.
		//-----------------------------------------
		//gC_vs_ui.DropItem();
	}
	//else
	{
	//	(*g_pSlayerGear).AddItem( pItem, (enum MSlayerGear::GEAR_SLAYER)left );
	}
}


//-----------------------------------------------------------------------------
//
// QuickSlot에 있던 item을 mouse로 집은 경우
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_PICKUP_FROM_QUICKSLOT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_PICKUP_FROM_QUICKSLOT");
	
	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}

	//
	// left = slot enum
	// void_ptr = MItem *
	//
	MOustersArmsBand* pQuickSlot = NULL;
		
	if( g_pPlayer->IsOusters() )
	{
		if( g_pArmsBand1 == NULL )
			pQuickSlot = g_pArmsBand2;
		else
		{
			if( g_pArmsBand1->GetPocketNumber() > left )
				pQuickSlot = g_pArmsBand1;
			else
				pQuickSlot = g_pArmsBand2;
		}
		
		if( pQuickSlot == NULL )
			return;
	}
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		MItem* pItem = (MItem*)void_ptr;

			//---------------------------------------------------
			// QuickSlot에 있던 item을 mouse에 붙였다(-_-;)고
			// server로 packet을 보낸다.
			//---------------------------------------------------
			CGAddQuickSlotToMouse _CGAddQuickSlotToMouse;
			_CGAddQuickSlotToMouse.setObjectID( pItem->GetID() );
			if( pQuickSlot != NULL)
			{
				if( pQuickSlot == g_pArmsBand2 )
					_CGAddQuickSlotToMouse.setSlotID( pItem->GetItemSlot() + 3 );
				else
					_CGAddQuickSlotToMouse.setSlotID( pItem->GetItemSlot() );
			} else
				_CGAddQuickSlotToMouse.setSlotID( pItem->GetItemSlot() );
			
			g_pSocket->sendPacket( &_CGAddQuickSlotToMouse );
			

		// 바로 QuickSlot에서 제거하고 mouse에 들게 한다.
		
		if( g_pPlayer->IsSlayer() )
			g_pQuickSlot->RemoveItem( pItem->GetID() );
		else
		{
			if( pQuickSlot != NULL )
				pQuickSlot->RemoveItem( pItem->GetID() );
		}

		UI_PickUpItem(pItem);
	}
}


//-----------------------------------------------------------------------------
//
// Inventory에 있던 item을 mouse로 집은 경우
//
//-----------------------------------------------------------------------------				
void
UIMessageManager::Execute_UI_ITEM_PICKUP_FROM_INVENTORY(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_PICKUP_FROM_INVENTORY"); 
	
	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}
	//
	// left = grid x
	// right = grid y
	// void_ptr = MItem *
	//
	//if (g_pTempInformation->Mode==TempInformation::MODE_NULL)
	{
		MItem* pItem = (MItem*)void_ptr;

		BOOL bSendPacket = TRUE;
		
		BOOL bSendPacketTradeRemove = FALSE;

		BOOL bAcceptMyTrade = (g_pTradeManager!=NULL && g_pTradeManager->IsAcceptMyTrade());
		BOOL bAcceptOtherTrade = (g_pTradeManager!=NULL && g_pTradeManager->IsAcceptOtherTrade());

		//---------------------------------------------------------
		// 교환중에 OK 눌렀을때는 검증을 받아야 한다.
		//---------------------------------------------------------
		if (bAcceptMyTrade)
		{
			if (g_pTempInformation->GetMode() == TempInformation::MODE_NULL
				&& g_pPlayer->IsItemCheckBufferNULL())
			{	
				g_pTempInformation->SetMode(TempInformation::MODE_TRADE_VERIFY_PICKUP_FROM_INVENTORY);
				g_pTempInformation->Value1 = pItem->GetID();
				g_pTempInformation->Value2 = left;
				g_pTempInformation->Value3 = right;
				g_pTempInformation->pValue = pItem;

				g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_PICKUP_FROM_INVENTORY );

				if (pItem->IsTrade())
				{
					bSendPacketTradeRemove = TRUE;
				}
			}
			else
			{
				bSendPacket = FALSE;
			}
		}
		//---------------------------------------------------------
		// 교환할려고 체크된 item을 들 때,
		//---------------------------------------------------------
		else if (gC_vs_ui.IsRunningExchange())
		{
			//---------------------------------------------------
			// 선택되어 있던 아이템이면.. 제거
			//---------------------------------------------------
			if (pItem->IsTrade())
			{
				UI_PickUpItem( pItem );

				bSendPacketTradeRemove = TRUE;
				
				// 다른 사람이 OK누른 상태라면..
				if (bAcceptOtherTrade)
				{
					g_pTradeManager->RefuseOtherTrade();
				}
			}
			//---------------------------------------------------
			// 아니면.. 수동으로 OK취소를 보낸다.
			//---------------------------------------------------
			else
			{
				UI_PickUpItem( pItem );

				/*
				if (bAcceptMyTrade)
				{
					#ifdef CONNECT_SERVER
						CGTradeFinish _CGTradeFinish;
						_CGTradeFinish.setTargetObjectID( g_pTradeManager->GetOtherID() );
						_CGTradeFinish.setCode( CG_TRADE_FINISH_RECONSIDER );

						g_pSocket->sendPacket( &_CGTradeFinish );					
					#endif
				}
				*/
			}
		}
		else
		{
			UI_PickUpItem( pItem );
		}

		//---------------------------------------------------------
		// SendPacket - Trade Remove Item
		//---------------------------------------------------------
		if (bSendPacketTradeRemove)
		{
				CGTradeRemoveItem _CGTradeRemoveItem;
				_CGTradeRemoveItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
				_CGTradeRemoveItem.setItemObjectID( pItem->GetID() );

				g_pSocket->sendPacket( &_CGTradeRemoveItem );

				g_pTradeManager->SetNextAcceptTime();
		}

		//---------------------------------------------------------
		// SendPacket - Inventory to Mouse
		//---------------------------------------------------------
		if (bSendPacket)
		{
				//---------------------------------------------------
				// Inventory에 있던 item을 mouse에 붙였다(-_-;)고
				// server로 packet을 보낸다.
				//---------------------------------------------------
				CGAddInventoryToMouse _CGAddInventoryToMouse;
				_CGAddInventoryToMouse.setObjectID( pItem->GetID() );
				_CGAddInventoryToMouse.setX( pItem->GetGridX() );
				_CGAddInventoryToMouse.setY( pItem->GetGridY() );
				
				g_pSocket->sendPacket( &_CGAddInventoryToMouse );				
		}
	}
}


//-----------------------------------------------------------------------------
//
// Gear에 착용하고 있던 item을 mouse로 집은 경우
//
//-----------------------------------------------------------------------------				
void
UIMessageManager::Execute_UI_ITEM_PICKUP_FROM_GEAR(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_REMOVE_ITEM_FROM_GEAR");
	
	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}
	//
	// left = slot enum
	// void_ptr = MItem *
	//

	if (!g_bWatchMode)
	{
		int itemSlot = left;
		MItem* pItem = (MItem*)void_ptr;	// gear에 있는 item

		if (pItem!=NULL)
		{
			//-------------------------------------------------
			// Player가 정지해 있는 경우에만.
			//-------------------------------------------------
			//if (g_pPlayer->IsStop())
			{
				//---------------------------------------------
				// AddonItem을 장착 해제한다.
				//---------------------------------------------
				if (g_pPlayer->RemoveAddonItem( pItem ))
				{
					MItem* pRemovedItem = NULL;

					//---------------------------------------------
					// gear에서 제거한다.
					//---------------------------------------------
					switch(g_pPlayer->GetRace())
					{
					case RACE_SLAYER:
						pRemovedItem = g_pSlayerGear->RemoveItem( (MSlayerGear::GEAR_SLAYER)itemSlot );
						
						//-----------------------------------------------------------
						// belt를 놓거나 집었을 경우는 quick item을 reset해야 한다.
						//-----------------------------------------------------------
						if (pRemovedItem!=NULL 
							&& pRemovedItem->GetItemSlot() == MSlayerGear::GEAR_SLAYER_BELT)
						{
							gC_vs_ui.ResetSlayerQuickItemSize();
						}					
						break;

					case RACE_VAMPIRE:
						pRemovedItem = g_pVampireGear->RemoveItem( (MVampireGear::GEAR_VAMPIRE)itemSlot );
						break;

					case RACE_OUSTERS:
						pRemovedItem = g_pOustersGear->RemoveItem( (MOustersGear::GEAR_OUSTERS)itemSlot );
						break;
					}


					//---------------------------------------------
					// UI에서 Gear의 Item을 pickup하게 한다.
					//---------------------------------------------
					if (pRemovedItem!=NULL)
					{
						UI_PickUpItem( pRemovedItem );
						
							//---------------------------------------------------
							// Inventory에 있던 item을 mouse에 붙였다(-_-;)고
							// server로 packet을 보낸다.
							//---------------------------------------------------
							CGAddGearToMouse _CGAddGearToMouse;
							_CGAddGearToMouse.setObjectID( pRemovedItem->GetID() );
							_CGAddGearToMouse.setSlotID( pRemovedItem->GetItemSlot() );
							
							g_pSocket->sendPacket( &_CGAddGearToMouse );							
					}

					//----------------------------------------------------
					// Skill 체크
					//----------------------------------------------------
					g_pSkillAvailable->SetAvailableSkills();
				}			
			}
			//else
			{
			//	(*g_pSlayerGear).AddItem( pItem, (enum MSlayerGear::GEAR_SLAYER)left );
			}
		}
	}
	else
	{
		DEBUG_ADD( "[Watch Mode]");		
	}
}

//-----------------------------------------------------------------------------
//
// Inventory에서 Item을 사용한 경우
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_USE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_USE");
	
	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}
	//
	// void_ptr = MItem *
	//
	MItem* pItem = g_pInventory->GetItemToModify( left );//(MItem*)void_ptr;

	if (pItem!=NULL && g_pPlayer->IsItemCheckBufferNULL())
	{
		// 자기종족 아이템만 쓰쟈-ㅅ-;
		if(g_pPlayer->IsSlayer() && pItem->IsSlayerItem() ||
			g_pPlayer->IsVampire() && pItem->IsVampireItem() ||
			g_pPlayer->IsOusters() && pItem->IsOustersItem())
		{
			pItem->UseInventory();
		}
	}
	//----------------------------------------------------
	// item check buffer not null
	//----------------------------------------------------
	else
	{
		#ifdef OUTPUT_DEBUG
			if (pItem==NULL)
			{
				DEBUG_ADD("[Wait] Item is NULL");
			}
			else 
			{
				DEBUG_ADD("[Wait] Item Check Buffer is Not NULL");
			}
		#endif
	}
}

//-----------------------------------------------------------------------------
//
// QuickSlot에서 Item을 사용한 경우
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_USE_QUICKSLOT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_USE_QUICKSLOT");

	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}
	//
	// left = slot
	// right = by FunctionKey
	//
	//MItem* pItem = (MItem*)void_ptr;
	if (g_pPlayer!=NULL && (g_pQuickSlot!=NULL && g_pPlayer->IsSlayer() || 
		g_pPlayer->IsOusters() && (g_pArmsBand1 != NULL || g_pArmsBand2 != NULL) ) )
	{
		int bByFunctionKey = right;	// by FunctionKey
		MSlotItemManager *pQuickSlot = g_pQuickSlot;

		int slot = left;
//		const MItem* pSlotItem = g_pQuickSlot->GetItem( left );
		MItem * pSlotItem = NULL;
		
		if( g_pPlayer->IsOusters() )
		{
			if( g_pArmsBand1 == NULL )
				pQuickSlot = g_pArmsBand2;
			else
			{
				if( g_pArmsBand1->GetPocketNumber() > slot )
					pQuickSlot = g_pArmsBand1;
				else
				{
					pQuickSlot = g_pArmsBand2;
					slot -= g_pArmsBand1->GetPocketNumber();
				}
			}
		}

		if( pQuickSlot == NULL )
			return;

		pSlotItem = pQuickSlot->GetItem( slot );

		if (pSlotItem!=NULL)
		{
			MItem *pItem = NULL;
			
			pItem = pQuickSlot->GetItemToModify( pSlotItem->GetID() );

			if (g_pPlayer->IsItemCheckBufferNULL())
			{
//				__BEGIN_HELP_EVENT
//					if (bByFunctionKey)
//					{
//						// [도움말] functionKey로 사용한 아이템인가?
////						ExecuteHelpEvent( HE_PRESSED_FUNCTION_KEY_FOR_QUICKITEM );
//					}
//				__END_HELP_EVENT

				// 자기종족 아이템만 쓰쟈-ㅅ-;
				if(g_pPlayer->IsSlayer() && pItem->IsSlayerItem() ||
					g_pPlayer->IsVampire() && pItem->IsVampireItem() ||
					g_pPlayer->IsOusters() && pItem->IsOustersItem())
				{
					pItem->UseQuickItem();
				}
			}
			//----------------------------------------------------
			// item check buffer not null
			//----------------------------------------------------
			else
			{
				DEBUG_ADD("[Wait] Item Check Buffer is Not NULL");
			}
		}
		else
		{
			DEBUG_ADD_FORMAT("[Error] There is no item in slot=%d", left);
		}
	}
	else
	{
		DEBUG_ADD("[Error] There is no QuickSlot");
	}

}

//-----------------------------------------------------------------------------
//
// mouse의 Item을 Inventory의 Item에 추가할 때
//
//-----------------------------------------------------------------------------
// 쌓이는 부분은 검증받지 않고 처리한다.
// max 개수를 초과하면 개수만 바꿔주고
// 아니면.. 들고 있던 것은 없어진다.
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_INSERT_FROM_INVENTORY(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_INSERT_FROM_INVENTORY");

	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}

	// gpC_mouse_pointer->GetPickUpItem()		// 들고 있는 것
	//void_ptr = MItem *		// 들어갈 곳
	// Inventory좌표(left,right)
	MItem* pItem = (MItem*)void_ptr;
	MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();

	if (pMouseItem==NULL)
	{
		DEBUG_ADD("[Error] Mouse Item is NULL");
		return;
	}

	if (pItem==NULL)
	{
		DEBUG_ADD("[Error] InvenItem is NULL");
		return;
	}

	TYPE_OBJECTID mouseItemID = ((pMouseItem==NULL)?OBJECTID_NULL : pMouseItem->GetID());

	if (pItem->IsPileItem())
	{
		if (pItem->GetItemClass()==pMouseItem->GetItemClass()
			&& pItem->GetItemType()==pMouseItem->GetItemType()
			&& pItem->GetNumber() < pItem->GetMaxNumber())
		{
			BOOL bSendPacket = TRUE;

			BOOL bAcceptMyTrade = (g_pTradeManager!=NULL && g_pTradeManager->IsAcceptMyTrade());
			BOOL bAcceptOtherTrade = (g_pTradeManager!=NULL && g_pTradeManager->IsAcceptOtherTrade());

			//---------------------------------------------------------
			// 교환중에 OK 눌렀을때는 검증을 받아야 한다.
			//---------------------------------------------------------
			if (bAcceptMyTrade)
			{	
				// 이 상태에서는 UI에서 바로 넣지 않는다.
				// pItem은 들고 있는 item이다.
				if (g_pTempInformation->GetMode() == TempInformation::MODE_NULL
					&& g_pPlayer->IsItemCheckBufferNULL())
				{	
					g_pTempInformation->SetMode(TempInformation::MODE_TRADE_VERIFY_INSERT_TO_INVENTORY);
					g_pTempInformation->Value1 = pItem->GetID();
					g_pTempInformation->Value2 = left;
					g_pTempInformation->Value3 = right;
					g_pTempInformation->pValue = pItem;

					g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_INSERT_FROM_INVENTORY );
				}
				else
				{
					bSendPacket = FALSE;
				}
			}
			else
			{
				//g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_INSERT_FROM_INVENTORY );
				MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();

				//----------------------------------------------------
				// pMouseItem을 pItem에 추가시킨다.
				//----------------------------------------------------
				int total = pMouseItem->GetNumber() + pItem->GetNumber();
				if ( total > pItem->GetMaxNumber() )
				{
					// 한계 수치를 넘어갈 경우
					pMouseItem->SetNumber( total - pItem->GetMaxNumber() );
					pItem->SetNumber( pItem->GetMaxNumber() );
				}
				else
				{
					// 모두 pItem에 추가될 수 있는 경우
					pItem->SetNumber( total );
					UI_DropItem();

					delete pMouseItem;
				}

				//----------------------------------------------------
				// 교환중에 선택된 아이템인 경우
				//----------------------------------------------------
				// Remove and Add - -;
				if (gC_vs_ui.IsRunningExchange() && pItem->IsTrade())
				{
						//----------------------------------------------------
						// 놓여 있던거 제거
						//----------------------------------------------------
						CGTradeRemoveItem _CGTradeRemoveItem;
						_CGTradeRemoveItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
						_CGTradeRemoveItem.setItemObjectID( pItem->GetID() );

						g_pSocket->sendPacket( &_CGTradeRemoveItem );

						g_pTradeManager->SetNextAcceptTime();

						
						//----------------------------------------------------
						// 아이템 추가 --> 개수 증가
						//----------------------------------------------------
						CGAddMouseToInventory _CGAddMouseToInventory;
						_CGAddMouseToInventory.setObjectID( mouseItemID );
						_CGAddMouseToInventory.setInvenX( pItem->GetGridX() );
						_CGAddMouseToInventory.setInvenY( pItem->GetGridY() );								

						g_pSocket->sendPacket( &_CGAddMouseToInventory );

						bSendPacket = FALSE;

						
						//----------------------------------------------------
						// 다시 교환창에 아이템 추가
						//----------------------------------------------------
						CGTradeAddItem _CGTradeAddItem;
						_CGTradeAddItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
						_CGTradeAddItem.setItemObjectID( pItem->GetID() );

						g_pSocket->sendPacket( &_CGTradeAddItem );
						

					// 다른 사람이 OK누른 상태라면..
					if (bAcceptOtherTrade)
					{
						g_pTradeManager->RefuseOtherTrade();
					}
				}

				PlaySound( pItem->GetInventorySoundID() );
			}

			//----------------------------------------------------
			// Server에 접속한 경우
			//----------------------------------------------------
				if (bSendPacket)
				{
					CGAddMouseToInventory _CGAddMouseToInventory;
					_CGAddMouseToInventory.setObjectID( mouseItemID );
					_CGAddMouseToInventory.setInvenX( pItem->GetGridX() );
					_CGAddMouseToInventory.setInvenY( pItem->GetGridY() );								

					g_pSocket->sendPacket( &_CGAddMouseToInventory );

					
					//----------------------------------------------------
					// 각 Item class에 따른 처리
					//----------------------------------------------------
					//if (pItem->GetItemClass()==ITEM_CLASS_MONEY)
					//{
					//}
				}
				
		}
	}
}

//-----------------------------------------------------------------------------
//
// mouse의 Item을 Gear의 Item에 추가할 때
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_INSERT_FROM_GEAR(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_INSERT_FROM_GEAR");

	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}

	// gpC_mouse_pointer->GetPickUpItem()		// 들고 있는 것
	//void_ptr = MItem *		// 들어갈 곳
	// Gear Slot(left)

	if (!g_bWatchMode
		&& g_pPlayer->IsItemCheckBufferNULL())
	{
		MItem* pItem = (MItem*)void_ptr;
		MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();

		//----------------------------------------------------
		// Magazine
		//----------------------------------------------------
		if (pItem->GetItemClass()==ITEM_CLASS_MAGAZINE)
		{
			//----------------------------------------------------
			// Server에 접속한 경우
			//----------------------------------------------------
				//---------------------------------------------------
				// mouse에 있던 item을 Gear에 추가했다고
				// server로 packet을 보낸다.
				//---------------------------------------------------
				CGAddMouseToGear _CGAddMouseToGear;
				_CGAddMouseToGear.setObjectID( pMouseItem->GetID() );
				_CGAddMouseToGear.setSlotID( pItem->GetItemSlot() );
				
				g_pSocket->sendPacket( &_CGAddMouseToGear );

				
				// 검증받기를 기다려야한다.
				g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_INSERT_FROM_GEAR );

				// 검증받으면 m_p_current_pickup_item을 pItem을 추가해야한다.
		}
		//----------------------------------------------------
		// 아닌 경우.
		//----------------------------------------------------
		else
		{
			// 사용안함.. - -;
		}
	}
	else
	{
		DEBUG_ADD( "[Watch Mode]");
	}
}


//-----------------------------------------------------------------------------
//
// mouse의 Item을 QuickSlot의 Item에 추가할 때
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_INSERT_FROM_QUICKSLOT(int left, int right, void* void_ptr)
{				
	DEBUG_ADD("[UI] UI_ITEM_INSERT_FROM_QUICKSLOT");
	
	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}

	
	if (g_pPlayer->IsItemCheckBufferNULL())
	{
		// gpC_mouse_pointer->GetPickUpItem()		// 들고 있는 것
		//void_ptr = MItem *		// 들어갈 곳
		// QuickSlot좌표(left)
		MItem* pItem = (MItem*)void_ptr;
		MItem* pMouseItem = (MItem*)gpC_mouse_pointer->GetPickUpItem();

		if (pItem->GetItemClass()==pMouseItem->GetItemClass()
			&& pItem->GetItemType()==pMouseItem->GetItemType())
		{
			//----------------------------------------------------
			// Server에 접속한 경우
			//----------------------------------------------------
				CGAddMouseToQuickSlot _CGAddMouseToQuickSlot;
				_CGAddMouseToQuickSlot.setObjectID( pMouseItem->GetID() );
				if ( g_pPlayer->IsOusters() )
				{
					int slotID = left;
					
					if( g_pArmsBand1 != NULL &&g_pArmsBand1->GetPocketNumber() > slotID )
						_CGAddMouseToQuickSlot.setSlotID( slotID );
					else
					{
						_CGAddMouseToQuickSlot.setSlotID( pItem->GetItemSlot() + 3);
					}
				}
				else
					_CGAddMouseToQuickSlot.setSlotID( pItem->GetItemSlot() );

				g_pSocket->sendPacket( &_CGAddMouseToQuickSlot );

				
				// 검증받기 위해서 기다려야 한다.
				// 검증받으면 m_p_current_pickup_item을 pItem을 추가해야한다.
				//g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_INSERT_FROM_INVENTORY );
				//MItem* pMouseItem = (MItem*)gpC_mouse_pointer->GetPickUpItem();

				//----------------------------------------------------
				// pMouseItem을 pItem에 추가시킨다.
				//----------------------------------------------------
				int total = pMouseItem->GetNumber() + pItem->GetNumber();
				if ( total > pItem->GetMaxNumber() )
				{
					// 한계 수치를 넘어갈 경우
					pMouseItem->SetNumber( total - pItem->GetMaxNumber() );
					pItem->SetNumber( pItem->GetMaxNumber() );
				}
				else
				{
					// 모두 pItem에 추가될 수 있는 경우
					pItem->SetNumber( total );
					UI_DropItem();

					delete pMouseItem;
				}

				PlaySound( pItem->GetInventorySoundID() );

				//----------------------------------------------------
				// 각 Item class에 따른 처리
				//----------------------------------------------------
				//if (pItem->GetItemClass()==ITEM_CLASS_MONEY)
				//{
				//}
				
		}
	}
}


//-----------------------------------------------------------------------------
//
// 상점 닫을 때,
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CLOSE_SHOP(int left, int right, void* void_ptr) 
{
	DEBUG_ADD("[UI] UI_CLOSE_SHOP");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.CloseShop();

		// shop을 없앤다.
		gC_vs_ui.SetShop( NULL );


		// 다시 뭔가를?선택할 수 있게 한다.
		g_pUIDialog->ShowPCTalkDlg();
	}
}

//-----------------------------------------------------------------------------
//
// 상점에서 살려는 물건을 선택했을 때,
//
//-----------------------------------------------------------------------------
//
// void_ptr = MShop
// left = slot
// right = item count
//
void
UIMessageManager::Execute_UI_BUY_ITEM(int left, int right, void* void_ptr)
{				
	DEBUG_ADD("[UI] UI_BUY_ITEM");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	// 검증할게 없는 경우
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		int index = left;					// 살려는 아이템의 위치
		int number = right;					// 살려는 개수
		MShop* pShop = (MShop*)void_ptr;	// 상점
		int npcID = (*g_pPCTalkBox).GetNPCID();					

		if (pShop!=NULL)
		{					
			MShopShelf* pShopShelf = pShop->GetCurrentShelf();
			
			if (pShopShelf!=NULL)
			{
				MItem* pItem = pShopShelf->GetItem( index );
				
				if (pItem!=NULL)
				{
					POINT point;
					
					int oldNumber = pItem->GetNumber();					

					//-------------------------------------------------
					// 살 수 있는지 체크한다.
					//-------------------------------------------------							
					bool bBuyPossible = false;
					//GAME_STRINGID buyImpossibleMessage = STRING_MESSAGE_CANNOT_BUY;
					int buyImpossibleMessage = STRING_MESSAGE_CANNOT_BUY;

					switch (pShop->GetShopType())
					{
						//-------------------------------------------------
						// 보통 상점
						//-------------------------------------------------
						case MShop::SHOP_NORMAL :
						{
							int price = number * (*g_pPriceManager).GetItemPrice(pItem, MPriceManager::NPC_TO_PC, pShopShelf->GetShelfType()==MShopShelf::SHELF_UNKNOWN);
							int money = (*g_pMoneyManager).GetMoney();

							bBuyPossible = (price <= money);

							buyImpossibleMessage = STRING_MESSAGE_CANNOT_BUY_NO_MONEY;
						}
						break;

						//-------------------------------------------------
						// 이벤트별 상점
						//-------------------------------------------------
						case MShop::SHOP_EVENT_STAR :
						{
							STAR_ITEM_PRICE starPrice;

							g_pPriceManager->GetItemPrice( pItem, starPrice );

							if (starPrice.type!=-1 && starPrice.number!=0)
							{
								// 몇개나 있는지 찾아본다.
								MItemClassTypeNumberFinder starFinder(ITEM_CLASS_EVENT_STAR, 
																		starPrice.type);


								((MItemManager*)g_pInventory)->FindItem( starFinder );

								// 가지고 있는게 더 많아야 한다.
								bBuyPossible = (starFinder.GetTotalNumber() >= starPrice.number);

								buyImpossibleMessage = STRING_MESSAGE_CANNOT_BUY_NO_STAR;
							}							
							
						}
						break;
					}

					//-------------------------------------------------
					// 돈이 충분한 경우
					//-------------------------------------------------
					if (bBuyPossible)
					{								
						//-------------------------------------------------
						// inventory에 넣을 위치를 찾는다.
						//-------------------------------------------------
						pItem->SetNumber( number );

						if (g_pInventory->GetFitPosition(pItem, point))
						{								
								CGShopRequestBuy _CGShopRequestBuy;
								_CGShopRequestBuy.setObjectID( npcID );

								_CGShopRequestBuy.setShopType( pShop->GetCurrent() );
								_CGShopRequestBuy.setShopIndex( index );

								_CGShopRequestBuy.setX(point.x);
								_CGShopRequestBuy.setY(point.y);

								_CGShopRequestBuy.setItemNum( number );

								g_pSocket->sendPacket( &_CGShopRequestBuy );

								
								//-------------------------------------------------
								// 검증을 위한 Temp Information설정
								//-------------------------------------------------
								(*g_pTempInformation).Mode	= TempInformation::MODE_SHOP_BUY;
								(*g_pTempInformation).Value1 = pShop->GetCurrent();
								(*g_pTempInformation).Value2 = index;
								(*g_pTempInformation).Value3 = point.x;
								(*g_pTempInformation).Value4 = point.y;
								(*g_pTempInformation).pValue = (void*)pShop;

								//-------------------------------------------------
								// 다른 아이템에 접근 못하도록..
								//-------------------------------------------------
								UI_LockItemTrade();
						}
						else
						{
							// inventory가 꽉차서 못 산다!
							g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_BUY_NO_SPACE ].GetString());
							
							// 2004, 5, 7 , sobeit add start - 인벤에 자리가 없으면 보관함 사라고 도움말 보여줌
							ExecuteHelpEvent( HELP_EVENT_STORAGE_BUY );
							// 2004, 5, 6, sobeit add end
						}

						pItem->SetNumber( oldNumber );

					}
					//-------------------------------------------------
					// 돈 혹은 별이.. 부족한 경우
					//-------------------------------------------------					
					else
					{
						g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[buyImpossibleMessage].GetString());
					}
				}
				else
				{
					// 선반의 index 위치에 아이템이 없는 경우
					DEBUG_ADD_FORMAT("[Error] There is NO Item in index=%d", index);
				}
			}
			else
			{
				// 선반이 설정 안 된 경우
				DEBUG_ADD_FORMAT("[Error] There is NO Shelf type=%d", (int)pShop->GetCurrent());
			}
		}
		else
		{
			// shop이 설정 안 된 경우
			DEBUG_ADD_FORMAT("[Error] There is NO Shop. npc id=%d", npcID);
		}
	}
}

//-----------------------------------------------------------------------------
//
// 물건 팔기 dialog 닫을 때
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_SELL_FINISHED(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_SELL_FINISHED");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	if (g_pTempInformation!=NULL
		&& g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.FinishItemSelling();

		// 다시 뭔가를?선택할 수 있게 한다.
		g_pUIDialog->ShowPCTalkDlg();
	}
}

//-----------------------------------------------------------------------------
//
// 팔려는 물건을 선택했을 때,
//
//-----------------------------------------------------------------------------
//
// gridX   //void_ptr = Mitem*
// gridY
//
void
UIMessageManager::Execute_UI_SELL_ITEM(int left, int right, void* void_ptr)
{				
	DEBUG_ADD("[UI] UI_SELL_ITEM");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	//MItem* pItem = (MItem*)void_ptr;
	// 검증할게 없는 경우
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		const MItem* pItem = g_pInventory->GetItem( left, right );

		if (pItem!=NULL && g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
		{
			// Event GiftBox 아이템인 경우 못 놓는다.
			
			if (//pItem->GetItemClass()!=ITEM_CLASS_EVENT_GIFT_BOX &&
				!pItem->IsUniqueItem() || (g_pTimeItemManager->IsExist( pItem->GetID())))
			{
				//-----------------------------------------------------
				// 아이템을 팔기 위한 packet을 보낸다.
				//-----------------------------------------------------
					CGShopRequestSell	_CGShopRequestSell;

					_CGShopRequestSell.setObjectID( (*g_pPCTalkBox).GetNPCID() );		// NPC ID	
					_CGShopRequestSell.setOpCode( SHOP_REQUEST_SELL_NORMAL );
					_CGShopRequestSell.setItemObjectID( pItem->GetID() );	// Item Object ID

					g_pSocket->sendPacket( &_CGShopRequestSell );
			
						
					//-------------------------------------------------
					// 검증을 위한 Temp Information설정
					//-------------------------------------------------
					(*g_pTempInformation).Mode	= TempInformation::MODE_SHOP_SELL;
					(*g_pTempInformation).pValue = (void*)pItem;

					//-------------------------------------------------
					// 다른 아이템에 접근 못하도록..
					//-------------------------------------------------
					UI_LockItemTrade();
			}
			else
			{
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_SELL ].GetString());
			}
		}
		else
		{
			DEBUG_ADD("[Error] Item Is NULL");
		}
	}
}


//-----------------------------------------------------------------------------
//
// 팔려는 물건을 선택했을 때,
//
//-----------------------------------------------------------------------------
//
// gridX   //void_ptr = Mitem*
// gridY
//
void
UIMessageManager::Execute_UI_SELL_ALL_ITEM(int left, int right, void* void_ptr)
{				
	DEBUG_ADD("[UI] UI_SELL_ALL_ITEM");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	//MItem* pItem = (MItem*)void_ptr;
	// 검증할게 없는 경우
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		//-----------------------------------------------------
		// 아이템을 팔기 위한 packet을 보낸다.
		//-----------------------------------------------------
			CGShopRequestSell	_CGShopRequestSell;

			_CGShopRequestSell.setObjectID( (*g_pPCTalkBox).GetNPCID() );		// NPC ID	
			_CGShopRequestSell.setOpCode( SHOP_REQUEST_SELL_ALL_SKULL );
			_CGShopRequestSell.setItemObjectID( 0 );	// Item Object ID

			g_pSocket->sendPacket( &_CGShopRequestSell );
	
				
			//-------------------------------------------------
			// 검증을 위한 Temp Information설정
			//-------------------------------------------------
			(*g_pTempInformation).Mode	= TempInformation::MODE_SHOP_SELL_ALL_SKULL;
			
			//-------------------------------------------------
			// 다른 아이템에 접근 못하도록..
			//-------------------------------------------------
			UI_LockItemTrade();
	}
}

//-----------------------------------------------------------------------------
//
//	Mouse Focus
// 
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_REMOVE_BACKGROUND_MOUSE_FOCUS(int left, int right, void* void_ptr)
{	
	if (g_Mode!=MODE_GAME)
	{
		return;
	}

	g_bUIInput = TRUE;
}

void
UIMessageManager::Execute_UI_BACKGROUND_MOUSE_FOCUS(int left, int right, void* void_ptr)
{	
	if (g_Mode!=MODE_GAME)
	{
		return;
	}

	//
	// (left, right) = mouse (x, y)
	//
	g_bUIInput = FALSE;
}
/*
//-----------------------------------------------------------------------------
//
// Skill View를 닫을 때,
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CLOSE_SKILL_VIEW(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CLOSE_SKILL_VIEW");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	gC_vs_ui.CloseSkillView();

	// 다시 뭔가를?선택할 수 있게 한다.
	//g_pUIDialog->ShowPCTalkDlg();

	// mode 제거
	//(*g_pTempInformation).Mode = TempInformation::MODE_NULL;
}


//-----------------------------------------------------------------------------
//
//	Skill을 배운다고 했을 때,
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_LEARN_SLAYER_SKILL(int left, int right, void* void_ptr)
{
	//
	// left = ACTIONINFO
	//
	DEBUG_ADD("[UI] UI_LEARN_SLAYER_SKILL");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	if ((*g_pTempInformation).Mode == TempInformation::MODE_SKILL_LEARN)
	{
		(*g_pTempInformation).Value3 = left;

		#ifdef	CONNECT_SERVER
			CGLearnSkill _CGLearnSkill;
			
			//_CGLearnSkill.setObjectID( (*g_pPCTalkBox).GetNPCID() );
			_CGLearnSkill.setSkillType( left );
			_CGLearnSkill.setSkillDomainType( (*g_pTempInformation).Value1 );

			g_pSocket->sendPacket( &_CGLearnSkill );

			
		#endif
	}
}
*/
//-----------------------------------------------------------------------------
//
// PDS를 닫을 때,
//
//-----------------------------------------------------------------------------
/*
void
UIMessageManager::Execute_UI_PDS_CLOSED(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_PDS_CLOSED");
	
	gC_vs_ui.AcquirePDSClosedMessage();
}
*/

//-----------------------------------------------------------------------------
//
// Slayer 정보 보기 전에 값 설정해주기..
//
//-----------------------------------------------------------------------------
/*
void
UIMessageManager::Execute_UI_PLEASE_SET_SLAYER_VALUE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_PLEASE_SET_SLAYER_VALUE");
	
	//C_VS_UI_SLAYER_PDS::m_str		= g_pPlayer->GetSTR();
	//C_VS_UI_SLAYER_PDS::m_dex		= g_pPlayer->GetDEX();
	//C_VS_UI_SLAYER_PDS::m_int		= g_pPlayer->GetINT();
	//C_VS_UI_SLAYER_PDS::m_hp		= g_pPlayer->GetHP();
	//C_VS_UI_SLAYER_PDS::m_mp		= g_pPlayer->GetMP();
	//C_VS_UI_SLAYER_PDS::m_th		= g_pPlayer->GetTOHIT();
	//C_VS_UI_SLAYER_PDS::m_ac		= g_pPlayer->GetAC();
	//C_VS_UI_SLAYER_PDS::m_cc		= g_pPlayer->GetCARRYWEIGHT();
	//C_VS_UI_SLAYER_PDS::m_str_max	= g_pPlayer->GetSTR(); // (!)
	//C_VS_UI_SLAYER_PDS::m_dex_max	= g_pPlayer->GetDEX(); // (!)
	//C_VS_UI_SLAYER_PDS::m_int_max	= g_pPlayer->GetINT();
	//C_VS_UI_SLAYER_PDS::m_HP_MAX	= g_pPlayer->GetMAX_HP();
	//C_VS_UI_SLAYER_PDS::m_MP_MAX	= g_pPlayer->GetMAX_MP();
	//C_VS_UI_SLAYER_PDS::m_th_max	= g_pPlayer->GetTOHIT(); // (!)
	//C_VS_UI_SLAYER_PDS::m_ac_max	= g_pPlayer->GetAC();	 // (!)
	//C_VS_UI_SLAYER_PDS::m_cc_max	= g_pPlayer->GetCARRYWEIGHT(); // (!)	
}
*/

//-----------------------------------------------------------------------------
//
// PCS 번호를 받을 때 --> 다른 사람과 통화할려고..
//
//-----------------------------------------------------------------------------
/*
void
UIMessageManager::Execute_UI_SEND_PCS_NUMBER(int left, int right, void* void_ptr)
{
	//
	// left = pcs number
	//
	DEBUG_ADD("[UI] UI_SEND_PCS_NUMBER");
	
	int pcsNumber = left;


	//-------------------------------------------
	// Server에 접속해 있을 때,
	//-------------------------------------------
	#ifdef CONNECT_SERVER
		CGDialUp _CGDialUp;
		_CGDialUp.setPhoneNumber( pcsNumber );

		g_pSocket->sendPacket( &_CGDialUp );

		
	//-------------------------------------------
	// Client Only
	//-------------------------------------------
	#else
		char str[128];
		strcpy(str, "Noname");
		UI_OnLinePCS(str, left);
	#endif

	//gC_vs_ui.OnLinePCS(); // PCS가 연결되었을 때 해줘야 한다!
}
 */

//-----------------------------------------------------------------------------
//
// 접속 됐을 때
//
//-----------------------------------------------------------------------------
/*
case UI_PCS_CONNECTOR_GRANTED:
//
// 대기방에 있는 사람과 연결함.
//
// left = pcs number
//
DEBUG_ADD("[UI] UI_PCS_CONNECTOR_GRANTED");

UI_OnLinePCS();
break;
*/

//-----------------------------------------------------------------------------
//
// PCS 접속중에 끝낼때..
//
//-----------------------------------------------------------------------------
// PCS 창에서 PCS number를 send하고 connecting 중에 END를 눌렀다.
/*
void
UIMessageManager::Execute_UI_END_PCS(int left, int right, void* void_ptr) 
{
	DEBUG_ADD("[UI] UI_END_PCS");
	
	//------------------------------------------------
	// 모든 접속을 다 끊는다.
	//------------------------------------------------
	#ifdef	CONNECT_SERVER			
		CGPhoneDisconnect _CGPhoneDisconnect;
			
		for (int i=0; i<3; i++)
		{
			if (g_pUserInformation->OtherPCSNumber[i] != 0)
			{
				_CGPhoneDisconnect.setSlotID( i );

				g_pSocket->sendPacket( &_CGPhoneDisconnect );

			}
		}
	#endif
}
*/

//-----------------------------------------------------------------------------
//
// Online 상태에서 PCS끝낼때
//
//-----------------------------------------------------------------------------
/*
void
UIMessageManager::Execute_UI_QUIT_PCS_ONLINE_MODE(int left, int right, void* void_ptr)
{
	//
	// PCS online mode에서 quit 버튼을 눌렀다.
	//
	DEBUG_ADD("[UI] UI_QUIT_PCS_ONLINE_MODE");
	
	//------------------------------------------------
	// 모든 접속을 다 끊는다.
	//------------------------------------------------
	#ifdef	CONNECT_SERVER			
		CGPhoneDisconnect _CGPhoneDisconnect;
			
		for (int i=0; i<3; i++)
		{
			if (g_pUserInformation->OtherPCSNumber[i] != 0)
			{
				_CGPhoneDisconnect.setSlotID( i );

				g_pSocket->sendPacket( &_CGPhoneDisconnect );

			}
		}
	#endif

	UI_AcquireQuitPCSOnlineModeMessage();
}
*/

//-----------------------------------------------------------------------------
//
// 다른 사람으로 바꿈.
//
//-----------------------------------------------------------------------------
/*
void
UIMessageManager::Execute_UI_CHANGE_PCS_CONNECTED_SLOT(int left, int right, void* void_ptr)
{
	//
	// 또 다른 연결된 사람으로 바꿈.
	//
	// left = pcs number
	// right = slot 
	//
	DEBUG_ADD("[UI] UI_CHANGE_PCS_CONNECTED_SLOT");
	
	int slot = right;

	gC_vs_ui.GrantPCSWaitRoomToDuplex( slot );
}
*/

//-----------------------------------------------------------------------------
//
// Online 상태에서 PCS끝낼때
//
//-----------------------------------------------------------------------------
// 대기방 click! (이미 연결-duplex-되지 않았음)
/*
void
UIMessageManager::Execute_UI_PLEASE_PCS_CONNECT_ME(int left, int right, void* void_ptr)
{
	//
	// 사람이 있는 대기방을 클릭하였다. (메시지를 송신 하기 위해서)
	//
	// left = pcs number
	// right = slot 
	//
	DEBUG_ADD("[UI] UI_PLEASE_PCS_CONNECT_ME");
	
	int slot = right;

	// 클릭된 slot과 연결함.
	gC_vs_ui.GrantPCSWaitRoomToDuplex( slot );
}
*/

//-----------------------------------------------------------------------------
//
// GameMenu Continue
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_GAMEMENU_CONTINUE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_GAMEMENU_CONTINUE");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	gC_vs_ui.CloseGameMenu();
}

//-----------------------------------------------------------------------------
//
// Minimap Toggle
//
//-----------------------------------------------------------------------------
/*
void
UIMessageManager::Execute_UI_MINIMAP_TOGGLE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_MINIMAP_TOGGLE");
	
	g_pUserOption->DrawMinimap = !g_pUserOption->DrawMinimap;
}
*/

//-----------------------------------------------------------------------------
//
// Drop Money
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_DROP_MONEY(int left, int right, void* void_ptr)
{
	return;
	//
	// 인벤토리에서 돈을 drop한다.
	//
	// left = money
	//
	DEBUG_ADD("[UI] UI_DROP_MONEY");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	//-----------------------------------------------------------------------------
	// 돈이 0이면 안된다.
	//-----------------------------------------------------------------------------
	if (left > 0)
	{
		if (g_pMoneyManager!=NULL)
		{
				CGDropMoney _CGDropMoney;
				_CGDropMoney.setAmount( left );

				g_pSocket->sendPacket( &_CGDropMoney );

				

			//-----------------------------------------------
			// 돈 줄여버린다.
			//-----------------------------------------------
			g_pMoneyManager->UseMoney( left );
		}
		else
		{
			DEBUG_ADD("[Error] MoneyManager is NULL");
		}
	}
}

//-----------------------------------------------------------------------------
//
// Click Bonus Point - 보너스 포인트 올릴때,
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CLICK_BONUS_POINT(int left, int right, void* void_ptr)
{
	//
	// vampire bonus point를 올리기 위해 버튼을 눌렀다.
	//
	// g_char_slot_ingame의 bonus point를 검색하면 얼마나 남았는지 알 수 있다.
	//
	// left = { 0:STR, 1:DEX, 2:INT }
	//
	DEBUG_ADD("[UI] UI_CLICK_BONUS_POINT");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	const int bonusPart[] =
	{
		INC_STR,
		INC_DEX,
		INC_INT
	};

	// 기억해두기 위해서...
	if ((*g_pTempInformation).Mode==TempInformation::MODE_NULL)
	{
		int bonusPoint = g_pPlayer->GetBonusPoint();

		//----------------------------------------------
		// vampire이고 bonuspoint가 있을 때,
		//----------------------------------------------
		// 2005, 1, 18, sobeit modify start - 승직 슬레는 보너스 포인트가 있다.
//		if (!g_pPlayer->IsSlayer()// || g_pPlayer->IsOusters()
//			&& bonusPoint > 0)
		if(bonusPoint > 0)
		// 2005, 1, 18, sobeit modify end - 승직 슬레는 보너스 포인트가 있다.
		{
			
				CGUseBonusPoint _CGUseBonusPoint;

				_CGUseBonusPoint.setWhich( bonusPart[left] );

				g_pSocket->sendPacket( &_CGUseBonusPoint );

				
				//----------------------------------------------
				// 선택한 부분 기억
				//----------------------------------------------
				(*g_pTempInformation).Mode = TempInformation::MODE_BONUSPOINT_USE;
				(*g_pTempInformation).Value1 = bonusPart[left];				
		
				
		}
	}
}


//-----------------------------------------------------------------------------
//
// CharInfo 버튼을 눌렀을 때,
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_INFO(int left, int right, void* void_ptr)
{
	//
	// character info button을 눌렀다. 자신의 정보를 보여줘야 한다!
	//
//	DEBUG_ADD("[UI] UI_CHARINFO");
//	
//	
//	if (g_Mode!=MODE_GAME)
//	{
//		DEBUG_ADD("Not Mode MODE_GAME");
//		return;
//	}
//
//
//	if (gC_vs_ui.IsRunningElevator())
//	{
//		DEBUG_ADD("[Execute_UI_CHARINFO] before RunCharInfo-Elevator Running");
//	}
//
//	//----------------------------------------------
//	// 열려있으면 닫는다.
//	//----------------------------------------------
//	if (gC_vs_ui.IsRunningCharInfo())
//	{
//		gC_vs_ui.CloseCharInfo();
//	}
//	//----------------------------------------------
//	// 띄운다.
//	//----------------------------------------------
//	else
//	{
//		// set slot
//
//		// g_char_slot_ingame 게임실행 중 게속 설정해줘야 한다.
//		/*
//		g_char_slot_ingame.sz_name = "yaho";
//		g_char_slot_ingame.bl_vampire = true;//false;
//		g_char_slot_ingame.MP_MAX = 100;
//		g_char_slot_ingame.HP_MAX = 100;
//		g_char_slot_ingame.HP = 100;
//		g_char_slot_ingame.MP = 100;
//		g_char_slot_ingame.bl_female = false;
//
//		g_char_slot_ingame.bonus_point = 5;
//		*/
//		g_char_slot_ingame.sz_name = g_pUserInformation->CharacterID.GetString();
//		g_char_slot_ingame.bl_vampire = g_pPlayer->IsVampire();
//		g_char_slot_ingame.bl_female = !g_pPlayer->IsMale();
//		
//		
//		if (g_pPlayer->IsSlayer())
//		{
//			g_char_slot_ingame.level = 1;//g_pPlayer->GetLEVEL();
//
//			//g_char_slot_ingame.skin_color = g_pPlayer->GetAddonInfo(ADDON_COAT).ColorSet1;
//			//g_char_slot_ingame.hair_color = g_pPlayer->GetAddonInfo(ADDON_HAIR).ColorSet1;
//			g_char_slot_ingame.hair_color = g_pUserInformation->HairColor;
//			g_char_slot_ingame.skin_color = g_pUserInformation->SkinColor;
//		}
//		else
//		{
//			g_char_slot_ingame.level = g_pPlayer->GetVampExp();
//
//			//g_char_slot_ingame.skin_color = g_pPlayer->GetBodyColor1();//pCreatureWear->GetAddonInfo(ADDON_COAT).ColorSet1;
//			//g_char_slot_ingame.hair_color = pCreatureWear->GetAddonInfo(ADDON_HAIR).ColorSet1;
//			g_char_slot_ingame.hair_color = g_pUserInformation->HairColor;
//			g_char_slot_ingame.skin_color = g_pUserInformation->SkinColor;
//		}
//		
//		if (g_pPlayer->IsMale())
//		{
//			switch (g_pUserInformation->FaceStyle)
//			{
//				case HAIR_STYLE1 :
//					g_char_slot_ingame.man_info.face = M_FACE1;
//				break;
//				case HAIR_STYLE2 :
//					g_char_slot_ingame.man_info.face = M_FACE2;
//				break;
//				case HAIR_STYLE3 :
//					g_char_slot_ingame.man_info.face = M_FACE3;
//				break;
//			}
//		}
//		else
//		{
//			switch (g_pUserInformation->FaceStyle)
//			{
//				case HAIR_STYLE1 :
//					g_char_slot_ingame.woman_info.face = W_FACE1;
//				break;
//				case HAIR_STYLE2 :
//					g_char_slot_ingame.woman_info.face = W_FACE2;
//				break;
//				case HAIR_STYLE3 :
//					g_char_slot_ingame.woman_info.face = W_FACE3;
//				break;
//			}
//		}
//
//		g_char_slot_ingame.level = g_pPlayer->GetLEVEL();
//		g_char_slot_ingame.alignment = (ALIGNMENT)ConvertAlignment( g_pPlayer->GetAlignment() );
//		g_char_slot_ingame.EXP_CUR	= g_pPlayer->GetVampExp();
//
//		g_char_slot_ingame.STR_CUR =g_pPlayer->GetSTR();
//		g_char_slot_ingame.DEX_CUR =g_pPlayer->GetDEX();
//		g_char_slot_ingame.INT_CUR =g_pPlayer->GetINT();
//		g_char_slot_ingame.STR_MAX =g_pPlayer->GetMAX_STR();
//		g_char_slot_ingame.DEX_MAX =g_pPlayer->GetMAX_DEX();
//		g_char_slot_ingame.INT_MAXX =g_pPlayer->GetMAX_INT();
//		g_char_slot_ingame.STR_EXP_CUR =g_pPlayer->GetSTR_EXP();
//		g_char_slot_ingame.DEX_EXP_CUR =g_pPlayer->GetDEX_EXP();
//		g_char_slot_ingame.INT_EXP_CUR =g_pPlayer->GetINT_EXP();

		// test code
//		g_char_slot_ingame.STR_EXP_NEXT_LEVEL	= 10000;
//		g_char_slot_ingame.DEX_EXP_NEXT_LEVEL	= 10000;
		//g_char_slot_ingame.INT_EXP_NEXT_LEVEL	= 10000;
//		g_char_slot_ingame.STR_EXP_CUR_LEVEL	= 5000;
//		g_char_slot_ingame.DEX_EXP_CUR_LEVEL	= 5000;
//		g_char_slot_ingame.INT_EXP_CUR_LEVEL	= 5000;


		//g_char_slot_ingame.DAM =g_pPlayer->GetDAM();
		//g_char_slot_ingame.AC =g_pPlayer->GetAC();
		//g_char_slot_ingame.TOHIT =g_pPlayer->GetTOHIT();
//		g_char_slot_ingame.HP =g_pPlayer->GetHP();
//		g_char_slot_ingame.MP =g_pPlayer->GetMP();
//		g_char_slot_ingame.HP_MAX =g_pPlayer->GetMAX_HP();
//		g_char_slot_ingame.MP_MAX =g_pPlayer->GetMAX_MP();
//		g_char_slot_ingame.SILVER_HP = g_pPlayer->GetSilverDamage();
//		//g_char_slot_ingame.CC =g_pPlayer->GetCARRYWEIGHT();
//		g_char_slot_ingame.FAME =g_pPlayer->GetFAME();
//		g_char_slot_ingame.NOTERITY =g_pPlayer->GetNotoriety();
//
//		DEBUG_ADD("[UI] CalculateStatus");
//		
//		g_pPlayer->CalculateStatus();
//
//		DEBUG_ADD("[UI] RunCharInfo");
//		
//
//		gC_vs_ui.RunInfo();
//	}
//
//	if (gC_vs_ui.IsRunningElevator())
//	{
//		DEBUG_ADD("[Execute_UI_CHARINFO] after RunCharInfo-Elevator Running");
//	}
//
//	DEBUG_ADD("[UI] UI_CHARINFO OK");
}

//-----------------------------------------------------------------------------
//
// 수리할려는 물건을 선택했을 때,
//
//-----------------------------------------------------------------------------
//
// void_ptr = Mitem*
//
void
UIMessageManager::Execute_UI_REPAIR_ITEM(int left, int right, void* void_ptr)
{				
	DEBUG_ADD("[UI] UI_REPAIR_ITEM");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	// 검증할게 없는 경우
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		MItem* pItem = NULL;// = (MItem*)void_ptr;
		int itemID = left;

		//-------------------------------------------------------------
		// itemID가 0인 경우는 전체 다 수리
		//-------------------------------------------------------------
		if (itemID==0)
		{
			//-----------------------------------------------------
			// 아이템을 수리하기 위한 packet을 보낸다.
			//-----------------------------------------------------
				CGRequestRepair	_CGRequestRepair;

				_CGRequestRepair.setObjectID( 0 );
				
				g_pSocket->sendPacket( &_CGRequestRepair );
			
				//-------------------------------------------------
				// 검증을 위한 Temp Information설정
				//-------------------------------------------------
				(*g_pTempInformation).Mode		= TempInformation::MODE_SHOP_REPAIR;
				(*g_pTempInformation).Value1	= false;	// inventory에 있는 item인가?
				(*g_pTempInformation).Value2	= true;		// gear에 있는 item인가?
				(*g_pTempInformation).pValue	= NULL;

				//-------------------------------------------------
				// 다른 아이템에 접근 못하도록..
				//-------------------------------------------------
				UI_LockItemTrade();
		}
		//-------------------------------------------------------------
		// item 하나만 수리
		//-------------------------------------------------------------
		else
		{
			BOOL bInInventory = FALSE;
			BOOL bInGear = FALSE;

			//------------------------------------------------------------------------------
			// inventory에서 찾는다.
			//------------------------------------------------------------------------------
			pItem = g_pInventory->GetItemToModify( itemID );

			if (pItem==NULL)
			{
				//------------------------------------------------------------------------------
				// gear에서 찾는다.
				//------------------------------------------------------------------------------
				switch(g_pPlayer->GetRace())
				{
				case RACE_SLAYER:
					pItem = g_pSlayerGear->GetItemToModify( itemID );
					
					if (pItem!=NULL)
					{
						bInGear = TRUE;
					}
					break;

				case RACE_VAMPIRE:
					pItem = g_pVampireGear->GetItemToModify( itemID );
					
					if (pItem!=NULL)
					{
						bInGear = TRUE;
					}
					break;

				case RACE_OUSTERS:
					pItem = g_pOustersGear->GetItemToModify( itemID );
					
					if (pItem!=NULL)
					{
						bInGear = TRUE;
					}
					break;
				}
			}
			else
			{
				bInInventory = TRUE;
			}

			//-------------------------------------------------
			// inventory나 gear에 있다면 수리한다.
			//-------------------------------------------------
			if (bInInventory || bInGear)
			{
				//-------------------------------------------------
				// 수리할 수 있는 item인지 체크한다.
				// 열쇠는 오토바이 수리를 의미한다.
				//-------------------------------------------------
				if (pItem->GetItemClass() != ITEM_CLASS_VAMPIRE_AMULET
					|| !pItem->IsUniqueItem()
					|| !pItem->IsQuestItem()
					|| pItem->GetMaxDurability() != -1
					|| pItem->GetItemClass()==ITEM_CLASS_KEY
					|| pItem->IsChargeItem())
				{
					//-------------------------------------------------
					// 돈이 충분한지 체크한다.
					//-------------------------------------------------
					int price = g_pPriceManager->GetItemPrice(pItem, MPriceManager::REPAIR);
					int money = g_pMoneyManager->GetMoney();

					//-------------------------------------------------					
					// 가격이 0이면 수리할 필요가 없다.
					// 단, key는 오토바이 수리에 이용된다.
					//-------------------------------------------------					
					if (price!=0 || pItem->GetItemClass()==ITEM_CLASS_KEY)
					{
						//-------------------------------------------------
						// 돈이 충분한 경우
						//-------------------------------------------------
						if (money!=0 && price <= money)
						{														
							//-----------------------------------------------------
							// 아이템을 수리하기 위한 packet을 보낸다.
							//-----------------------------------------------------
								CGRequestRepair	_CGRequestRepair;

								_CGRequestRepair.setObjectID( pItem->GetID() );
								
								g_pSocket->sendPacket( &_CGRequestRepair );
							
								//-------------------------------------------------
								// 검증을 위한 Temp Information설정
								//-------------------------------------------------
								(*g_pTempInformation).Mode		= TempInformation::MODE_SHOP_REPAIR;
								(*g_pTempInformation).Value1	= bInInventory;	// inventory에 있는 item인가?
								(*g_pTempInformation).Value2	= bInGear;		// gear에 있는 item인가?
								(*g_pTempInformation).pValue	= (void*)pItem;

								//-------------------------------------------------
								// 다른 아이템에 접근 못하도록..
								//-------------------------------------------------
								UI_LockItemTrade();
						}
						//-------------------------------------------------
						// 돈이 부족한 경우
						//-------------------------------------------------					
						else
						{
							g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_BUY_NO_MONEY ].GetString());
						}
					}
					//-------------------------------------------------
					// 수리할 필요가 없는 경우
					//-------------------------------------------------
					else
					{
						g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_REPAIR].GetString());
					}
				}
				//-------------------------------------------------
				// 수리할 필요가 없는 경우
				//-------------------------------------------------
				else
				{
					g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_REPAIR].GetString());
				}
			}		
		}
	}
}

//-----------------------------------------------------------------------------
//
// Silvering 물건을 선택했을 때,
//
//-----------------------------------------------------------------------------
//
// void_ptr = Mitem*
//
void
UIMessageManager::Execute_UI_SILVERING_ITEM(int left, int right, void* void_ptr)
{				
	DEBUG_ADD("[UI] UI_SILVERING_ITEM");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	// 검증할게 없는 경우
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		MItem* pItem = NULL;// = (MItem*)void_ptr;
		int itemID = left;

		BOOL bInInventory = FALSE;
		BOOL bInGear = FALSE;

		//------------------------------------------------------------------------------
		// inventory에서 찾는다.
		//------------------------------------------------------------------------------
		pItem = g_pInventory->GetItemToModify( itemID );

		if (pItem==NULL)
		{
			//------------------------------------------------------------------------------
			// gear에서 찾는다.
			//------------------------------------------------------------------------------
			switch(g_pPlayer->GetRace())
			{
			case RACE_SLAYER:
				pItem = g_pSlayerGear->GetItemToModify( itemID );
				
				if (pItem!=NULL)
				{
					bInGear = TRUE;
				}
				break;

			case RACE_VAMPIRE:
				pItem = g_pVampireGear->GetItemToModify( itemID );
				
				if (pItem!=NULL)
				{
					bInGear = TRUE;
				}
				break;

			case RACE_OUSTERS:
				pItem = g_pOustersGear->GetItemToModify( itemID );
				
				if (pItem!=NULL)
				{
					bInGear = TRUE;
				}
				break;
			}
		}
		else
		{
			bInInventory = TRUE;
		}

		//-------------------------------------------------
		// inventory나 gear에 있다면 silvering한다..
		//-------------------------------------------------
		if (bInInventory || bInGear)
		{
			//-------------------------------------------------
			// Silvering 할 수 있는 item인지 체크한다.
			//-------------------------------------------------
			if (pItem->GetSilverMax() > 0)
			{
				//-------------------------------------------------
				// 돈이 충분한지 체크한다.
				//-------------------------------------------------
				int price = (*g_pPriceManager).GetItemPrice(pItem, MPriceManager::SILVERING);
				int money = (*g_pMoneyManager).GetMoney();

				//-------------------------------------------------					
				// 가격이 0이면 silvering 할 필요가 없다.
				//-------------------------------------------------					
				if (price!=0)
				{
					//-------------------------------------------------
					// 돈이 충분한 경우
					//-------------------------------------------------
					if (money!=0 && price <= money)
					{														
						//-----------------------------------------------------
						// 아이템을 silverig하기 위한 packet을 보낸다.
						//-----------------------------------------------------
							CGSilverCoating	_CGSilverCoating;

							_CGSilverCoating.setObjectID( pItem->GetID() );
							
							g_pSocket->sendPacket( &_CGSilverCoating );
						
							//-------------------------------------------------
							// 검증을 위한 Temp Information설정
							//-------------------------------------------------
							(*g_pTempInformation).Mode		= TempInformation::MODE_SHOP_SILVERING;
							(*g_pTempInformation).Value1	= bInInventory;	// inventory에 있는 item인가?
							(*g_pTempInformation).Value2	= bInGear;		// gear에 있는 item인가?
							(*g_pTempInformation).pValue	= (void*)pItem;

							//-------------------------------------------------
							// 다른 아이템에 접근 못하도록..
							//-------------------------------------------------
							UI_LockItemTrade();
					}
					//-------------------------------------------------
					// 돈이 부족한 경우
					//-------------------------------------------------					
					else
					{
						g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_BUY_NO_MONEY ].GetString());
					}
				}
				//-------------------------------------------------
				// Silvering할 필요가 없는 경우
				//-------------------------------------------------
				else
				{
					g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_SILVERING].GetString());
				}
			}
			//-------------------------------------------------
			// 수리할 필요가 없는 경우
			//-------------------------------------------------
			else
			{
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_SILVERING].GetString());
			}
		}		
	}
}

//-----------------------------------------------------------------------------
//
// 수리 그만두기
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_REPAIR_FINISHED(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_REPAIR_FINISHED");
	
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation!=NULL
		&& g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.FinishItemRepairing();

		// 다시 뭔가를?선택할 수 있게 한다.
		g_pUIDialog->ShowPCTalkDlg();
	}
}

//-----------------------------------------------------------------------------
//
// silvering 그만두기
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_SILVERING_FINISHED(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_SILVERING_FINISHED");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}


	if (g_pTempInformation!=NULL
		&& g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.FinishItemSilvering();

		// 다시 뭔가를?선택할 수 있게 한다.
		g_pUIDialog->ShowPCTalkDlg();
	}
}

//-----------------------------------------------------------------------------
//
// 능력치
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_FINISH_LEVELUP_BUTTON(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_FINISH_LEVELUP_BUTTON");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	gC_vs_ui.FinishLevelUp();
}

//-----------------------------------------------------------------------------
//
// 게임메뉴 띄울때, 다른 거 다 닫는다.
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_RUNNING_GAMEMENU(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_RUNNING_GAMEMENU");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
//	g_pUIDialog->CloseHelpDlg();
	g_pUIDialog->CloseMessageDlg();
	g_pUIDialog->ClosePCTalkDlg();
}

//-----------------------------------------------------------------------------
//
// 보관함 사기
//
//-----------------------------------------------------------------------------
// left = 산다(TRUE), 안산다(FALSE)
void
UIMessageManager::Execute_UI_STORAGE_BUY(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_STORAGE_BUY");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	BOOL buy = (BOOL)left;

	//-----------------------------------------------------------------------------
	// 보관함을 살려는 상태..
	//-----------------------------------------------------------------------------
	if (buy)
	{
		if (g_pTempInformation->GetMode()==TempInformation::MODE_STORAGE_BUY)
		{		
			//--------------------------------------------------------
			// 돈 체크
			//--------------------------------------------------------
			if (g_pMoneyManager->GetMoney() >= g_pTempInformation->Value1)
			{
					CGStashRequestBuy _CGStashRequestBuy;
					
					g_pSocket->sendPacket( &_CGStashRequestBuy );					

				// mode 설정
				g_pTempInformation->SetMode(TempInformation::MODE_STORAGE_BUY_WAIT);

			}
			//--------------------------------------------------------
			// 돈이 부족한 경우
			//--------------------------------------------------------
			else
			{
				// 다시 뭔가를?선택할 수 있게 한다.
				g_pUIDialog->ShowPCTalkDlg();

				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_BUY_NO_MONEY ].GetString());
				
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);
			}
		}
		else
		{
			DEBUG_ADD("[Error] Mode is Not MODE_STORAGE_BUY");
		}

		// 사는 경우는.. 결과 packet을 받을때까지
		// NPC대화 dialog를 안 띄운다.
	}
	//-----------------------------------------------------------------------------
	// 보관함 안 산다.
	//-----------------------------------------------------------------------------
	else
	{
		// 다시 뭔가를?선택할 수 있게 한다.
		g_pUIDialog->ShowPCTalkDlg();

		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
	}					
}

//-----------------------------------------------------------------------------
//
// 보관함의 slot을 선택하였다.
//
//-----------------------------------------------------------------------------
// left = tab (0, 1, 2)
// right = slot
//
void
UIMessageManager::Execute_UI_SELECT_STORAGE_SLOT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_SELECT_STORAGE_SLOT");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if(g_pStorage == NULL)
		return;

	g_pStorage->SetCurrent( left );	// 확인용

	int slot = right;

	MItem* pMouseItem = (MItem*)gpC_mouse_pointer->GetPickUpItem();
	const MItem* pStorageItem = g_pStorage->GetItem( slot );

	//-----------------------------------------------------------------------------
	// 보관함에서 뭔가를 집을려는 경우
	//-----------------------------------------------------------------------------
	if (pMouseItem==NULL)
	{
		//---------------------------------------------------
		// 선택한 slot에 item이 있으면 집는다.
		//---------------------------------------------------
		if (pStorageItem!=NULL)
		{
			MItem* pRemovedItem = g_pStorage->RemoveItem( slot );

			if (pRemovedItem!=NULL)
			{
				 UI_PickUpItem( (MItem*)pStorageItem );

					CGStashToMouse _CGStashToMouse;
					_CGStashToMouse.setObjectID( pStorageItem->GetID() );
					_CGStashToMouse.setRack( g_pStorage->GetCurrent() );
					_CGStashToMouse.setIndex( slot );									

					g_pSocket->sendPacket( &_CGStashToMouse );					
			}
		}
	}
	//-----------------------------------------------------------------------------
	// 들고 있는 item을 보관함에 놓을려는 경우
	//-----------------------------------------------------------------------------
	else 
	{
		BOOL bSendPacket = TRUE;

		TYPE_OBJECTID mouseItemID = pMouseItem->GetID();		

		if(pMouseItem->GetItemClass() == ITEM_CLASS_SUB_INVENTORY && pMouseItem->GetEnchantLevel()>0) 
		{
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_STORAGE].GetString());
			return ;
		}
	
		// Event GiftBox 아이템인 경우 못 놓는다.
		if (pMouseItem->GetItemClass()!=ITEM_CLASS_EVENT_GIFT_BOX
			&& pMouseItem->GetItemClass()!=ITEM_CLASS_VAMPIRE_AMULET
			&& pMouseItem->GetItemClass()!=ITEM_CLASS_COUPLE_RING
			&& pMouseItem->GetItemClass()!=ITEM_CLASS_VAMPIRE_COUPLE_RING
			&& !pMouseItem->IsQuestItem()
			&& pMouseItem->IsNormalItem()
			&& !((pMouseItem->GetItemClass() == ITEM_CLASS_EVENT_TREE) && pMouseItem->GetItemType() >= 13  )
			&& pMouseItem->GetItemClass() != ITEM_CLASS_EVENT_ITEM
			&& pMouseItem->GetItemClass() != ITEM_CLASS_CODE_SHEET
			&& !(pMouseItem->GetItemClass() == ITEM_CLASS_LUCKY_BAG && pMouseItem->GetItemType() == 3)
			// sjheo 2005.05.02 풍선 아이템 일때 보관함 처리  Add 
			&& !(pMouseItem->GetItemClass() == ITEM_CLASS_EVENT_ETC && (pMouseItem->GetItemType() == 18))
			&& !(pMouseItem->GetItemClass() == ITEM_CLASS_EVENT_ITEM && (pMouseItem->GetItemType() >= 32 && pMouseItem->GetItemType() <= 36))
			// sjheo 2005.05.02 풍선 아이템 일때 보관함 처리 End
			)
		{
			//---------------------------------------------------
			// 그 자리에 아무것도 없으면 item 넣는다.
			//---------------------------------------------------
			if (pStorageItem==NULL)
			{
				UI_DropItem();	// mouse에서 item뗀다.

				g_pStorage->SetItem( slot, pMouseItem );							
			}
			//---------------------------------------------------
			// 뭔가 있다면...
			//---------------------------------------------------
			else
			{
				//---------------------------------------------------
				// 쌓일 수 있는 아이템인 경우
				//---------------------------------------------------
				if (pStorageItem->IsPileItem()
					&& pStorageItem->GetItemClass()==pMouseItem->GetItemClass()
					&& pStorageItem->GetItemType()==pMouseItem->GetItemType())
				{
					//---------------------------------------------------
					// 더 쌓일 수 있다면
					//---------------------------------------------------
					if (pStorageItem->GetNumber() < pStorageItem->GetMaxNumber())
					{
						PlaySound( pMouseItem->GetInventorySoundID() );
						
						MItem* pModifyStorageItem = g_pStorage->RemoveItem( slot );

						//----------------------------------------------------
						// pMouseItem을 pStorageItem에 추가시킨다.
						//----------------------------------------------------
						int total = pMouseItem->GetNumber() + pStorageItem->GetNumber();
						if ( total > pStorageItem->GetMaxNumber() )
						{
							// 한계 수치를 넘어갈 경우
							pMouseItem->SetNumber( total - pStorageItem->GetMaxNumber() );
							pModifyStorageItem->SetNumber( pStorageItem->GetMaxNumber() );
						}
						else
						{
							// 모두 pItem에 추가될 수 있는 경우
							pModifyStorageItem->SetNumber( total );
							UI_DropItem();

							delete pMouseItem;
						}	

						//----------------------------------------------------
						// 바꾼거를 다시 설정한다.
						//----------------------------------------------------
						g_pStorage->SetItem( slot, pModifyStorageItem );					
						
					}
					//---------------------------------------------------
					// 더 쌓일 수 없다면.. 기냥 둔다.
					//---------------------------------------------------				
					else
					{
						bSendPacket = FALSE;
					}
				}
				//---------------------------------------------------				
				// 쌓일 수 없는 아이템인 경우 --> 바꾼다.
				//---------------------------------------------------				
				else
				{				
					MItem* pTempItem = pMouseItem;
					
					UI_PickUpItem( (MItem*)pStorageItem );

					g_pStorage->RemoveItem( slot );	// 이게 실패하면.. - -;

					g_pStorage->SetItem( slot, pTempItem );
				}
			}

				if (bSendPacket)
				{		
					CGMouseToStash _CGMouseToStash;
					_CGMouseToStash.setObjectID( mouseItemID );
					_CGMouseToStash.setRack( g_pStorage->GetCurrent() );
					_CGMouseToStash.setIndex( slot );									

					g_pSocket->sendPacket( &_CGMouseToStash );				
				}
		}
		else
		{
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_STORAGE].GetString());
		}


	}
}

//-----------------------------------------------------------------------------
//
// 보관함에 돈 저장할때,
//
//-----------------------------------------------------------------------------
// left = money
//
void
UIMessageManager::Execute_UI_DEPOSIT_MONEY(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_DEPOSIT_MONEY");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	//-----------------------------------------------------------------------------
	// 돈이 0이면 안된다.
	//-----------------------------------------------------------------------------
	if (left > 0)
	{
		if (g_pMoneyManager!=NULL && g_pStorage!=NULL)
		{
				CGStashDeposit _CGStashDeposit;
				_CGStashDeposit.setAmount( left );

				g_pSocket->sendPacket( &_CGStashDeposit );				

			//-----------------------------------------------
			// 돈을 보관함으로 옮긴다.
			//-----------------------------------------------
			g_pMoneyManager->UseMoney( left );
			g_pStorage->GetMoneyManager()->AddMoney( left );
		}
		else
		{
			DEBUG_ADD("[Error] MoneyManager or Storage is NULL");
		}
	}
}

//-----------------------------------------------------------------------------
//
// 보관함의 돈을 찾을때,
//
//-----------------------------------------------------------------------------
// left = money
//
void
UIMessageManager::Execute_UI_WITHDRAW_MONEY(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_WITHDRAW_MONEY");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	//-----------------------------------------------------------------------------
	// 돈이 0이면 안된다.
	//-----------------------------------------------------------------------------
	if (left > 0)
	{
		if (g_pMoneyManager!=NULL && g_pStorage!=NULL)
		{
				CGStashWithdraw _CGStashWithdraw;
				_CGStashWithdraw.setAmount( left );

				g_pSocket->sendPacket( &_CGStashWithdraw );				

			//-----------------------------------------------
			// 보관함의 돈을 player에게 옮긴다.
			//-----------------------------------------------
			g_pStorage->GetMoneyManager()->UseMoney( left );
			g_pMoneyManager->AddMoney( left );						
		}
		else
		{
			DEBUG_ADD("[Error] MoneyManager or Storage is NULL");
		}
	}
}

//-----------------------------------------------------------------------------
//
//	보관함 닫을 때
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CLOSE_STORAGE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CLOSE_STORAGE");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	UI_CloseStorage();		

	// 다시 뭔가를?선택할 수 있게 한다.
	g_pUIDialog->ShowPCTalkDlg();
}

//-----------------------------------------------------------------------------
//
// 교환창에 돈을 넣는다.
//
//-----------------------------------------------------------------------------
// left = bGive - 남한테 주는 돈인가?
// right = money
//
void
UIMessageManager::Execute_UI_EXCHANGE_MONEY(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_EXCHANGE_MONEY");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	if (g_pTradeManager!=NULL)
	{
		BOOL bGive = left;
		int money = right;
			
		BOOL bAcceptMyTrade = g_pTradeManager->IsAcceptMyTrade();

		int code;		

		//---------------------------------------------------------
		// 교환중에 OK 눌렀을때는 검증을 받아야 한다.
		//---------------------------------------------------------
		if (bAcceptMyTrade)
		{	
			if (g_pTempInformation->GetMode() == TempInformation::MODE_NULL)
			{
				if (bGive)
				{
					g_pTempInformation->SetMode(TempInformation::MODE_TRADE_VERIFY_ADD_MONEY);
				}
				else
				{
					g_pTempInformation->SetMode(TempInformation::MODE_TRADE_VERIFY_REMOVE_MONEY);

					g_pTradeManager->SetNextAcceptTime();
				}

				g_pTempInformation->Value1	= money;
			}
		}
		//---------------------------------------------------------
		// OK 안 눌려있는 보통 때..
		//---------------------------------------------------------
		else
		{
			/*
			//-----------------------------------------------------------
			// 뭔가 바뀐다면... OK취소
			//-----------------------------------------------------------
			g_pTradeManager->RefuseOtherTrade();
			g_pTradeManager->RefuseMyTrade();
		
			//-----------------------------------------------------------
			// client 돈 이동
			//-----------------------------------------------------------
			if (bGive)
			{
				//if (!g_pMoneyManager->GetOtherMoneyManager->CanAddMoney(money))
				//{
				//	money = g_pMoneyManager->GetOtherMoneyManager->GetMaxAddMoney();
				//}

				g_pMoneyManager->UseMoney( money );
				g_pTradeManager->GetMyMoneyManager()->AddMoney( money );
			}
			else
			{
				g_pTradeManager->GetMyMoneyManager()->UseMoney( money );
				g_pMoneyManager->AddMoney( money );	
			}
			*/

			if (!bGive)
			{
				g_pTradeManager->SetNextAcceptTime();
			}
		}	

		//-----------------------------------------------------------
		// client 돈 이동
		//-----------------------------------------------------------
		if (bGive)
		{
			code = CG_TRADE_MONEY_INCREASE;
		}
		else
		{
			code = CG_TRADE_MONEY_DECREASE;
		}
		
		//-----------------------------------------------------------
		// server에 알림
		//-----------------------------------------------------------
			CGTradeMoney _CGTradeMoney;
			_CGTradeMoney.setTargetObjectID( g_pTradeManager->GetOtherID() );
			_CGTradeMoney.setAmount( money );
			_CGTradeMoney.setCode( code );				

			g_pSocket->sendPacket( &_CGTradeMoney );			

//			g_pTradeManager->SetNextAcceptTime();
	}
	else
	{
		DEBUG_ADD( "[Error] TradeManager is NULL");
	}


}

//-----------------------------------------------------------------------------
//
// 교환창의 아이템을 선택할때 
//
//-----------------------------------------------------------------------------
// gridXY = (left,right)
// 들려는 item = (MItem*)void_ptr
/*
void
UIMessageManager::Execute_UI_ITEM_PICKUP_FROM_TRADEGRID(int left, int right, void* void_ptr)
{
	//
	// hi/low dw_left는 grid (x, y)값이다.
	//        dw_right는 item의 screen 좌표이다.
	//
	DEBUG_ADD("[UI] UI_ITEM_PICKUP_FROM_TRADEGRID");
	
	int		gridX = left;
	int		gridY = right;
	MItem*	pItem = (MItem *)void_ptr;

	UI_PickUpItem( pItem );
}
*/

//-----------------------------------------------------------------------------
//
// 교환창의 아이템에 추가될때,
//
//-----------------------------------------------------------------------------
// gridXY = (left, right)
// 놓여있던 item = (MItem*)void_ptr
/*
void
UIMessageManager::Execute_UI_ITEM_INSERT_FROM_TRADEGRID(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_INSERT_FROM_TRADEGRID");
	
	int		gridX = left;
	int		gridY = right;
	MItem*	pItem = (MItem *)void_ptr;
	MItem*	pMouseItem = (MItem*)gpC_mouse_pointer->GetPickUpItem();
}
*/

//-----------------------------------------------------------------------------
//
// 교환창에 아이템 놓을 때,
//
//-----------------------------------------------------------------------------
// gridXY = (left, right)
// 놓기 바로 전에 들고 있던 item = (MItem*)void_ptr
/*
void
UIMessageManager::Execute_UI_ITEM_DROP_TO_TRADEGRID(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_DROP_TO_TRADEGRID");
	
	int		gridX = left;
	int		gridY = right;
	MItem*	pItem = (MItem *)void_ptr;
}
*/

//-----------------------------------------------------------------------------
//
// 교환창에 아이템 놓을 때,
//
//-----------------------------------------------------------------------------
// gridXY = (left, right)
// 놓기 바로 전에 들고 있던 item = (MItem*)void_ptr
void
UIMessageManager::Execute_UI_ITEM_SELECT_EXCHANGE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_SELECT_EXCHANGE");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTradeManager!=NULL)
	{
		int		gridX = left;
		int		gridY = right;
		MItem*	pItem = (MItem *)void_ptr;

		BOOL bAcceptMyTrade = g_pTradeManager->IsAcceptMyTrade();		

		// 선물 상자 하드 코딩-ㅅ-;
		if(pItem->GetItemClass() == ITEM_CLASS_EVENT_GIFT_BOX && pItem->GetItemType() > 1 && pItem->IsTrade() == false)	//
		{
			MItem* pFindItem = ((MItemManager*)g_pInventory)->FindItem( MEventGiftBoxItemFinder() );
			if(pFindItem != NULL)
			{
				// 이미 교환에 올라간게 있다
				return;
			}
		}
		else if((pItem->GetItemClass() == ITEM_CLASS_SUB_INVENTORY && pItem->GetEnchantLevel()>0) 
			   || (pItem->GetItemClass() == ITEM_CLASS_EVENT_ITEM && (pItem->GetItemType() >= 32 && pItem->GetItemType() <= 36))
			   || (pItem->GetItemClass() == ITEM_CLASS_SLAYER_PORTAL_ITEM)
			   )
		{
			UI_PopupMessage( STRING_MESSAGE_CANNOT_TRADE );
			return;
		}

		//---------------------------------------------------------
		// 교환중에 OK 눌렀을때는 검증을 받아야 한다.
		//---------------------------------------------------------
		if (bAcceptMyTrade)
		{	
			if (g_pTempInformation->GetMode() == TempInformation::MODE_NULL
				&& g_pPlayer->IsItemCheckBufferNULL())
			{
				g_pTempInformation->Value1 = pItem->GetID();
				g_pTempInformation->Value2 = gridX;
				g_pTempInformation->Value3 = gridY;
				g_pTempInformation->pValue = pItem;

				//-----------------------------------------------------------
				// 선택되어 있던 아이템 --> 취소
				//-----------------------------------------------------------
				if (pItem->IsTrade())
				{
					g_pTempInformation->SetMode(TempInformation::MODE_TRADE_VERIFY_REMOVE_ITEM);

					g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_TRADE_REMOVE );

						CGTradeRemoveItem _CGTradeRemoveItem;
						_CGTradeRemoveItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
						_CGTradeRemoveItem.setItemObjectID( pItem->GetID() );

						g_pSocket->sendPacket( &_CGTradeRemoveItem );

						g_pTradeManager->SetNextAcceptTime();

					// [도움말] 교환 중 - 아이템 취소
//					__BEGIN_HELP_EVENT
////						ExecuteHelpEvent( HE_TRADE_ITEM_REMOVE );	
//					__END_HELP_EVENT
				}
				//-----------------------------------------------------------
				// 아이템 선택
				//-----------------------------------------------------------
				else
				{
					g_pTempInformation->SetMode(TempInformation::MODE_TRADE_VERIFY_ADD_ITEM);

					g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_TRADE_ADD );

						CGTradeAddItem _CGTradeAddItem;
						_CGTradeAddItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
						_CGTradeAddItem.setItemObjectID( pItem->GetID() );

						g_pSocket->sendPacket( &_CGTradeAddItem );
						

					// [도움말] 교환 중 - 아이템 선택
//					__BEGIN_HELP_EVENT
////						ExecuteHelpEvent( HE_TRADE_ITEM_ADD );	
//					__END_HELP_EVENT
				}				
			}			
		}
		//---------------------------------------------------------
		// OK 안 눌려있는 보통 때..
		//---------------------------------------------------------
		else
		{		
			//-----------------------------------------------------------
			// 선택되어 있던 아이템 --> 취소
			//-----------------------------------------------------------
			if (pItem->IsTrade())
			{
				//-----------------------------------------------------------
				// 뭔가 바뀐다면... OK취소
				//-----------------------------------------------------------
				g_pTradeManager->RefuseOtherTrade();
				g_pTradeManager->RefuseMyTrade();

				pItem->UnSetTrade();		

					CGTradeRemoveItem _CGTradeRemoveItem;
					_CGTradeRemoveItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
					_CGTradeRemoveItem.setItemObjectID( pItem->GetID() );

					g_pSocket->sendPacket( &_CGTradeRemoveItem );

					g_pTradeManager->SetNextAcceptTime();

				// [도움말] 교환 중 - 아이템 취소
//				__BEGIN_HELP_EVENT
////					ExecuteHelpEvent( HE_TRADE_ITEM_REMOVE );	
//				__END_HELP_EVENT
			}
			//-----------------------------------------------------------
			// 선택되어 있지 않던 아이템 --> 선택
			//-----------------------------------------------------------
			else
			{
				// 빨간색 선물상자는 교환되지 않는다.
				if (pItem->GetItemClass()==ITEM_CLASS_EVENT_GIFT_BOX
					&& pItem->GetItemType()==1)
				{
				}
				else
				{
					// 선물상자인 경우는 검증이 필요하다.
					if (pItem->GetItemClass()==ITEM_CLASS_EVENT_GIFT_BOX)// && pItem->GetItemClass() < 2)
					{
						g_pTempInformation->SetMode(TempInformation::MODE_TRADE_VERIFY_ADD_ITEM);
						g_pTempInformation->pValue	= pItem;
						
						g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_TRADE_ADD );
					}
					// 그 외에는 그냥 넣으면 된다.
					else
					{
						//-----------------------------------------------------------
						// 뭔가 바뀐다면... OK취소
						//-----------------------------------------------------------
						g_pTradeManager->RefuseOtherTrade();
						g_pTradeManager->RefuseMyTrade();

						pItem->SetTrade();		
					}

						CGTradeAddItem _CGTradeAddItem;
						_CGTradeAddItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
						_CGTradeAddItem.setItemObjectID( pItem->GetID() );

						g_pSocket->sendPacket( &_CGTradeAddItem );
						

					// [도움말] 교환 중 - 아이템 선택
//					__BEGIN_HELP_EVENT
////						ExecuteHelpEvent( HE_TRADE_ITEM_ADD );	
//					__END_HELP_EVENT
				}
			}	
		}
	}
	else
	{
		DEBUG_ADD( "[Error] TradeManager is NULL");
	}
}


//-----------------------------------------------------------------------------
//
// 교환 확인
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_OK_EXCHANGE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_OK_EXCHANGE");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	if (g_pTradeManager!=NULL
		&& g_pTradeManager->IsAcceptTime())
	{
		MItem* pMouseItem = UI_GetMouseItem();

		//--------------------------------------------------------
		// 교환이 가능한 경우
		// 선물상자를 들고 있으면 교환이 안된다.
		//--------------------------------------------------------
		if ((pMouseItem==NULL || pMouseItem->GetItemClass()!=ITEM_CLASS_EVENT_GIFT_BOX)
			&& g_pTradeManager->CanTrade())
		{
			g_pTradeManager->AcceptMyTrade();
			
				CGTradeFinish _CGTradeFinish;
				_CGTradeFinish.setTargetObjectID( g_pTradeManager->GetOtherID() );
				_CGTradeFinish.setCode( CG_TRADE_FINISH_ACCEPT );

				g_pSocket->sendPacket( &_CGTradeFinish );
				
		}
		//--------------------------------------------------------
		// 교환이 불가능한 경우
		//--------------------------------------------------------
		else
		{
			// 교환할 수 없다면..
			// 일단은.. 공간 부족이라고 본다.
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_TRADE_NO_SPACE].GetString() );
		}
	}
	else
	{
		DEBUG_ADD( "[Error] TradeManager is NULL or NotAcceptTime");
	}
}

//-----------------------------------------------------------------------------
//
// 교환 취소
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CANCEL_EXCHANGE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CANCEL_EXCHANGE");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	BOOL bSendPacket = FALSE;

	if (g_pTradeManager!=NULL)
	{	
		BOOL bAcceptMyTrade = g_pTradeManager->IsAcceptMyTrade();

		//---------------------------------------------------------------
		// OK 누른 상태에서는 검증을 받아야 한다.
		//---------------------------------------------------------------
		if (bAcceptMyTrade)
		{
			if (g_pTempInformation->GetMode() == TempInformation::MODE_NULL)
			{
				g_pTempInformation->SetMode(g_pTempInformation->MODE_TRADE_VERIFY_CANCEL);

				bSendPacket = TRUE;
			}
			else
			{
				DEBUG_ADD( "[Error] Mode is not MODE_NULL");
			}
		}
		//---------------------------------------------------------------
		// 그냥 거부 (있을 수 없는 경우잖아 - -;;)
		//---------------------------------------------------------------
		else
		{		
			//---------------------------------------------------------------
			// 교환 거부당..
			//---------------------------------------------------------------
			g_pTradeManager->RefuseMyTrade();
			
			bSendPacket = TRUE;
		}
	}	
	else
	{
		DEBUG_ADD( "[Error] TradeManager is NULL");
	}

	//---------------------------------------------------------------
	// 내 OK를 취소하는 packet
	//---------------------------------------------------------------
	if (bSendPacket)
	{
			CGTradeFinish _CGTradeFinish;
			_CGTradeFinish.setTargetObjectID( g_pTradeManager->GetOtherID() );
			_CGTradeFinish.setCode( CG_TRADE_FINISH_RECONSIDER );

			g_pSocket->sendPacket( &_CGTradeFinish );
			
	}
}

//-----------------------------------------------------------------------------
//
// 교환창 닫기
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CLOSE_EXCHANGE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CLOSE_EXCHANGE");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	BOOL bAcceptMyTrade = (g_pTradeManager!=NULL && g_pTradeManager->IsAcceptMyTrade());

	BOOL bSendPacket = FALSE;

	int otherID;

	//---------------------------------------------------------------
	// OK 누른 상태에서는 검증을 받아야 한다.
	//---------------------------------------------------------------
	if (bAcceptMyTrade)
	{
		if (g_pTempInformation->GetMode() == TempInformation::MODE_NULL)
		{
			g_pTempInformation->SetMode(g_pTempInformation->MODE_TRADE_VERIFY_CLOSE);
			
			otherID = g_pTradeManager->GetOtherID();

			bSendPacket = TRUE;
		}
		else
		{
			DEBUG_ADD( "[Error] Mode is not MODE_NULL");
		}
	}
	//---------------------------------------------------------------
	// 그냥 취소..
	//---------------------------------------------------------------
	else
	{		
		if (g_pTradeManager!=NULL)
		{			
			otherID = g_pTradeManager->GetOtherID();

			g_pTradeManager->CancelTrade();

			bSendPacket = TRUE;
		}

		//---------------------------------------------------------------
		// 교환 안 해~~ 끝~이당
		//---------------------------------------------------------------
		UI_CloseExchange();	
	}
	
	//---------------------------------------------------------------
	// 교환을 끝내는 packet
	//---------------------------------------------------------------
	if (bSendPacket)
	{
			CGTradeFinish _CGTradeFinish;
			_CGTradeFinish.setTargetObjectID( otherID );
			_CGTradeFinish.setCode( CG_TRADE_FINISH_REJECT );

			g_pSocket->sendPacket( &_CGTradeFinish );
	}
}

//-----------------------------------------------------------------------------
//
// 채팅창에서 이름 선택
//
//-----------------------------------------------------------------------------
// left = 입력의 종류
// void_ptr = 캐릭터이름(!=NULL)
/*
void
UIMessageManager::Execute_UI_CHAT_SELECT_NAME(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CHAT_SELECT_NAME");
	
	char str[80];

	const char* pID = (const char*)void_ptr;
		
	switch (left)
	{
		case M_LEFTBUTTON_DOWN :
			sprintf(str, "/%s ", pID);
			gC_vs_ui.SetInputString( str );
		break;

		case M_RIGHTBUTTON_DOWN :
			#ifdef OUTPUT_DEBUG
				sprintf(str, "*trace %s", pID);
				gC_vs_ui.SetInputString( str );
			#else
				gC_vs_ui.AddInputString( pID );
			#endif
		break;
	}
}
*/

//-----------------------------------------------------------------------------
//
// 교환할래? Y/N에 대한 응답
//
//-----------------------------------------------------------------------------
// left = (BOOL)교환여부
//
void
UIMessageManager::Execute_UI_EXCHANGE_ACCEPT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_EXCHANGE_ACCEPT");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	BOOL accept = (BOOL)left;

	int code;

	//---------------------------------------------------------
	// 교환할래? Y/N에 대한 응답
	//---------------------------------------------------------
	if (g_pTempInformation->GetMode() == TempInformation::MODE_TRADE_REQUEST)
	{
		//---------------------------------------------------------
		// 허용
		//---------------------------------------------------------
		if (accept)
		{
			code = CG_TRADE_PREPARE_CODE_ACCEPT;

			//---------------------------------------------------------
			// 교환창을 띄운다.
			//---------------------------------------------------------
			UI_RunExchange( g_pTempInformation->Value1 );	// otherID 설정
		}
		//---------------------------------------------------------
		// 거부
		//---------------------------------------------------------
		else
		{	
			if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_TRADE)
			{
				g_pPlayer->SetWaitVerifyNULL();
			}

			code = CG_TRADE_PREPARE_CODE_REJECT;
		}

			CGTradePrepare _CGTradePrepare;
			_CGTradePrepare.setTargetObjectID( g_pTempInformation->Value1 );
			_CGTradePrepare.setCode( code );				

			g_pSocket->sendPacket( &_CGTradePrepare );			
	
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);		
	}	
}

//-----------------------------------------------------------------------------
//
// 교환취소할래?
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_EXCHANGE_REQUEST_CANCEL(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_EXCHANGE_REQUEST_CANCEL");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	//---------------------------------------------------------
	// 교환할래? Y/N에 대한 응답
	//---------------------------------------------------------
	if (g_pTempInformation->GetMode() == TempInformation::MODE_TRADE_REQUEST)
	{	
		if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_TRADE)
		{
			g_pPlayer->SetWaitVerifyNULL();
		}

			CGTradePrepare _CGTradePrepare;
			_CGTradePrepare.setTargetObjectID( g_pTempInformation->Value1 );
			_CGTradePrepare.setCode( CG_TRADE_PREPARE_CODE_CANCEL );				

			g_pSocket->sendPacket( &_CGTradePrepare );			
	
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
		
		// 교환 취소할래?를 제거한다.
		UI_CloseExchangeCancel();
	}	
}

/*
//-----------------------------------------------------------------------------
//
// Game Option 바꾸기
//
//-----------------------------------------------------------------------------
// left = 바뀌는 option
// right = 값
void
UIMessageManager::Execute_UI_CHANGE_GAME_OPTION(int left, int right, void* void_ptr)
{
	DEBUG_ADD_FORMAT("[UI] Execute_UI_CHANGE_GAME_OPTION[%d] = %d", left, right);
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	int value = right;
	
	switch (left)
	{
		//-------------------------------------------------------------------
		// DRAW_MINIMAP
		//-------------------------------------------------------------------		
		//case C_VS_UI_GAMEMENU_OPTION::DRAW_MINIMAP :
		//	g_pUserOption->DrawMinimap = (BOOL)value;
		//break;

		//-------------------------------------------------------------------
		// DRAW_ZONE_NAME
		//-------------------------------------------------------------------
		//case C_VS_UI_GAMEMENU_OPTION::DRAW_ZONE_NAME :
		//	g_pUserOption->DrawZoneName = (BOOL)value;
		//break;

		//-------------------------------------------------------------------
		// DRAW_GAME_TIME
		//-------------------------------------------------------------------
		//case C_VS_UI_GAMEMENU_OPTION::DRAW_GAME_TIME :
		//	g_pUserOption->DrawGameTime = (BOOL)value;
		//break;

		//-------------------------------------------------------------------
		// DRAW_BLENDING_CHARACTER_SHADOW
		//-------------------------------------------------------------------
		//case C_VS_UI_GAMEMENU_OPTION::DRAW_BLENDING_CHARACTER_SHADOW :
		//	///g_pUserOption->BlendingShadow = (BOOL)value;
		//break;

		//-------------------------------------------------------------------
		// PLAY_WAV
		//-------------------------------------------------------------------
		case C_VS_UI_GAMEMENU_OPTION::PLAY_WAV :
			g_pUserOption->PlayWaveMusic = (BOOL)value;

			PlayGameMusic();			
		break;

		//-------------------------------------------------------------------
		// BLOOD_DROP
		//-------------------------------------------------------------------
		case C_VS_UI_GAMEMENU_OPTION::BLOOD_DROP :
			g_pUserOption->BloodDrop = (BOOL)value;
		break;

		//-------------------------------------------------------------------
		// PLAY_SOUND
		//-------------------------------------------------------------------
		case C_VS_UI_GAMEMENU_OPTION::PLAY_SOUND :
			g_pUserOption->PlaySound = (BOOL)value;

			if (g_pUserOption->PlaySound)
			{
				g_DXSound.UnSetMute();
			}
			else
			{
				if (g_pSoundManager != NULL)
				{
					g_pSoundManager->Stop();
				}
				
				g_DXSound.SetMute();
			}
		break;

		//-------------------------------------------------------------------
		// PLAY_MUSIC
		//-------------------------------------------------------------------
		case C_VS_UI_GAMEMENU_OPTION::PLAY_MUSIC :
			g_pUserOption->PlayMusic = (BOOL)value;

			PlayGameMusic();
		break;

		//-------------------------------------------------------------------
		// VOLUME_SOUND
		//-------------------------------------------------------------------
		case C_VS_UI_GAMEMENU_OPTION::VOLUME_SOUND :
			if (value>=0 && value<16)
			{
				g_pUserOption->VolumeSound = value;	

				LONG volume = value*SOUND_DEGREE + SOUND_MIN;

				g_DXSound.SetVolumeLimit( volume );			
			}
		break;

		//-------------------------------------------------------------------
		// VOLUME_MUSIC 
		//-------------------------------------------------------------------
		case C_VS_UI_GAMEMENU_OPTION::VOLUME_MUSIC :

			if (value>=0 && value<16)
			{
				g_pUserOption->VolumeMusic = value;
				
				if (g_pUserOption->PlayWaveMusic)
				{
					LONG volume = value*SOUND_DEGREE + SOUND_MIN;

					DEBUG_ADD_FORMAT("Change VOLUME_MUSIC = %ld", volume);
					
					//LONG maxVolume = g_DXSound.GetVolumeLimit();

					//g_DXSound.SetVolumeLimit( volume );
					//g_DXSound.AddVolume( g_pDXSoundStream->GetBuffer(), 0 );
					g_pDXSoundStream->SetVolumeLimit( volume );

					// 원래의 max volume으로 돌린다.
					//g_DXSound.SetVolumeLimit( maxVolume );
				}
				else
				{
					WORD volume = value*0x1000 + 0x0FFF;
				
					if (!g_pUserOption->PlayMusic || g_Music.IsPause())
					{
						g_Music.SetVolume( volume );
					}
					else
					{
						g_Music.Pause();
						g_Music.SetVolume( volume );
						g_Music.Resume();
					}
				}
			}
		break;

		//-------------------------------------------------------------------
		// 캐릭터 HP바 투명/불투명 
		//-------------------------------------------------------------------
		case C_VS_UI_GAMEMENU_OPTION::PARTY_HPBAR_ALPHA :
			g_pUserOption->DrawTransHPBar = (BOOL)value;
		break;
		
	}
}


//-----------------------------------------------------------------------------
//
// GameOption 닫기
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CLOSE_GAME_OPTION(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CLOSE_GAME_OPTION");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	gC_vs_ui.CloseGameMenuOption();

	//---------------------------------------------------------------
	// 그냥 저장해버린다. - -;
	//---------------------------------------------------------------
	g_pUserOption->SaveToFile( FILE_INFO_USEROPTION );
}
*/

//-----------------------------------------------------------------------------
//
// Title Option 바꾸기
//
//-----------------------------------------------------------------------------
// left = 바뀌는 option
// right = 값
void
UIMessageManager::Execute_UI_CHANGE_OPTION(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CHANGE_TITLE_OPTION");
	
//	if (g_Mode!=MODE_MAINMENU)
//	{
//		DEBUG_ADD("Not Mode MODE_MAINMENU");
//		return;
//	}

	
	int value = right;

	switch (left)
	{
		//-------------------------------------------------------------------
		// USE 3D HAL
		//-------------------------------------------------------------------
//		case C_VS_UI_OPTION::CHECK_3D :
//			g_pUserOption->Use3DHAL = (BOOL)value;
//		break;

		//-------------------------------------------------------------------
		// DRAW_MINIMAP
		//-------------------------------------------------------------------
		//case C_VS_UI_OPTION::DRAW_MINIMAP :
		//	g_pUserOption->DrawMinimap = (BOOL)value;
		//break;

		//-------------------------------------------------------------------
		// DRAW_ZONE_NAME
		//-------------------------------------------------------------------
		//case C_VS_UI_OPTION::DRAW_ZONE_NAME :
		//	g_pUserOption->DrawZoneName = (BOOL)value;
		//break;

		//-------------------------------------------------------------------
		// DRAW_GAME_TIME
		//-------------------------------------------------------------------
		//case C_VS_UI_OPTION::DRAW_GAME_TIME :
		//	g_pUserOption->DrawGameTime = (BOOL)value;
		//break;

		//-------------------------------------------------------------------
		// DRAW_BLENDING_CHARACTER_SHADOW
		//-------------------------------------------------------------------
		//case C_VS_UI_OPTION::DRAW_BLENDING_CHARACTER_SHADOW :
	//		g_pUserOption->BlendingShadow = (BOOL)value;
		//break;

		//-------------------------------------------------------------------
		// PLAY_WAV
		//-------------------------------------------------------------------
		case C_VS_UI_OPTION::CHECK_WAV :
			g_pUserOption->PlayWaveMusic = (BOOL)value;

			if(g_Mode == MODE_MAINMENU)
				PlayTitleMusic();			
			else 
				PlayGameMusic();			
		break;

		//-------------------------------------------------------------------
		// BLOOD_DROP
		//-------------------------------------------------------------------
//		case C_VS_UI_OPTION::BLOOD_DROP :
//			g_pUserOption->BloodDrop = (BOOL)value;
//		break;

		//-------------------------------------------------------------------
		// PLAY_SOUND
		//-------------------------------------------------------------------
		case C_VS_UI_OPTION::CHECK_SOUND :
			g_pUserOption->PlaySound = (BOOL)value;

			if (g_pUserOption->PlaySound)
			{
				g_DXSound.UnSetMute();
			}
			else
			{
				if (g_pSoundManager != NULL)
				{
					g_pSoundManager->Stop();
				}
				
				g_DXSound.SetMute();
			}
		break;

		//-------------------------------------------------------------------
		// PLAY_MUSIC
		//-------------------------------------------------------------------
		case C_VS_UI_OPTION::CHECK_MUSIC :
			g_pUserOption->PlayMusic = (BOOL)value;

			PlayTitleMusic();			
		break;

		//-------------------------------------------------------------------
		// VOLUME_SOUND
		//-------------------------------------------------------------------
		case C_VS_UI_OPTION::CHECK_VALUE_SOUND_VOLUME :
			if (value>=0 && value<16)
			{
				g_pUserOption->VolumeSound = value;	

				LONG volume = value*SOUND_DEGREE + SOUND_MIN;

				g_DXSound.SetVolumeLimit( volume );	
				
				PlaySound(SOUND_SLAYER_BUTTON);
			}
		break;

		//-------------------------------------------------------------------
		// VOLUME_MUSIC 
		//-------------------------------------------------------------------
		case C_VS_UI_OPTION::CHECK_VALUE_MUSIC_VOLUME :

			if (value>=0 && value<16)
			{
				g_pUserOption->VolumeMusic = value;
				
				if (g_pUserOption->PlayWaveMusic)
				{
					/*
					LONG volume = value*SOUND_DEGREE + SOUND_MIN;

					LONG maxVolume = g_DXSound.GetVolumeLimit();

					g_DXSound.SetVolumeLimit( volume );
					g_DXSound.AddVolume( g_pDXSoundStream->GetBuffer(), 0 );

					// 원래의 max volume으로 돌린다.
					g_DXSound.SetVolumeLimit( maxVolume );
					*/
//					LONG volume = value*16*257;//*SOUND_DEGREE + SOUND_MIN;

//					DEBUG_ADD_FORMAT("Change VOLUME_MUSIC = %ld", volume);
					
//					g_pDXSoundStream->SetVolumeLimit( volume );
					DEBUG_ADD("MP3 SetVolume4");
#ifdef __USE_MP3__
					g_pMP3->SetVolume( volume );
#else
					if( g_DXSound.IsInit() && g_pOGG != NULL )
					{
						//int step = (DSBVOLUME_MIN) / 16;
						int volume = (value - 15) * 250;

						g_pOGG->streamVolume( max( -10000, min( -1, volume ) ) );
					}
#endif
					DEBUG_ADD("MP3 SetVolume4 OK");
				}
				else
				{
					WORD volume = (value)*0x1000 + 0x0FFF;

					if (!g_pUserOption->PlayMusic || g_Music.IsPause())
					{
						g_Music.SetVolume( volume );
					}
					else
					{
						g_Music.Pause();
						g_Music.SetVolume( volume );
						g_Music.Resume();
					}				
				}
			}
		break;

		//-------------------------------------------------------------------
		// 캐릭터 HP바 투명/불투명 
		//-------------------------------------------------------------------
//		case C_VS_UI_OPTION::PARTY_HPBAR_ALPHA :
//			g_pUserOption->DrawTransHPBar = (BOOL)value;
//		break;
	}
}


//-----------------------------------------------------------------------------
//
// Option 닫기
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CLOSE_OPTION(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CLOSE_TITLE_OPTION");

//	if (g_Mode!=MODE_MAINMENU)
//	{
//		DEBUG_ADD("Not Mode MODE_MAINMENU");
//		return;
//	}

	
	gC_vs_ui.CloseOption();

	//---------------------------------------------------------------
	// 그냥 저장해버린다. - -;
	//---------------------------------------------------------------
	g_pClientConfig->SaveToFile( g_pFileDef->getProperty("FILE_INFO_CLIENTCONFIG").c_str());
	g_pUserOption->SaveToFile( g_pFileDef->getProperty("FILE_INFO_USEROPTION").c_str());

	if (g_Mode==MODE_MAINMENU)
	{
		if (CDirect3D::IsHAL())
		{
			// 3D가속 중인데.. 가속 끄는 경우
			if (!g_pUserOption->Use3DHAL)
			{
				g_Mode = MODE_CHANGE_OPTION;
			}
		}
		else
		{
			// 3D가속 아닌데.. 가속 하는 경우
			if (g_bEnable3DHAL && g_pUserOption->Use3DHAL)
			{
				g_Mode = MODE_CHANGE_OPTION;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
// Bookcase 닫기
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CLOSE_BOOKCASE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CLOSE_BOOKCASE");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	UI_CloseBookcase();
}

//-----------------------------------------------------------------------------
//
// Briefing 닫기
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CLOSE_BRIEFING(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CLOSE_BRIEFING");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	UI_CloseBriefing();
}

//-----------------------------------------------------------------------------
//
// Computer 닫기
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CLOSE_COMPUTER(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CLOSE_COMPUTER");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	UI_CloseComputer();
}

//-----------------------------------------------------------------------------
//
// Tutorial에서 빠져나갈때, 무기 하나 선택
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CLOSE_TUTORIAL_EXIT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CLOSE_TUTORIAL_EXIT");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return; 
	}

	
	UI_CloseTutorialExit();

	int arms = left;	// 0: 도 1: 검 2: 메이스 3: 십자가 4: AR 5: TR 6: SMG 7:SG

	if (arms >= 0 && arms <=7)
	{
		ITEM_CLASS itemClass[] =
		{
			ITEM_CLASS_BLADE,
			ITEM_CLASS_SWORD,
			ITEM_CLASS_MACE,
			ITEM_CLASS_CROSS,
			ITEM_CLASS_AR,
			ITEM_CLASS_SR,
			ITEM_CLASS_SMG,
			ITEM_CLASS_SG
		};
		

			CGRequestNewbieItem _CGRequestNewbieItem;
		
			_CGRequestNewbieItem.setItemClass( itemClass[arms] );
			
			g_pSocket->sendPacket( &_CGRequestNewbieItem );
			
	}
}


//-----------------------------------------------------------------------------
//
// Desc Dialog 닫을 때
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_CLOSE_DESC_DIALOG(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CLOSE_DESC_DIALOG");

	
	//if (g_Mode!=MODE_GAME)
	{
	//	DEBUG_ADD("Not Mode MODE_GAME");
	//	return;
	}
	
	// Close	
	gC_vs_ui.CloseDescDialog();
}

//-----------------------------------------------------------------------------
//
// Elevator닫을 때
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_CLOSE_ELEVATOR(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CLOSE_ELEVATOR");
	
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	// Close
	gC_vs_ui.CloseElevator();
}

//-----------------------------------------------------------------------------
//
// Elevator 선택할 때
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_SELECT_ELEVATOR(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_SELECT_ELEVATOR");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	// left 0 : 4층 ....
	//		1 : 3
	//		2 : 2
	//		3 : 1
	//		4 : 지하 1층
	int selectPortal = 4-left;

	if (selectPortal <= 4)
	{
		const MSector& sector = g_pZone->GetSector(g_pPlayer->GetX(), g_pPlayer->GetY());

		if (sector.IsPortal())
		{
			int numPortal = sector.GetPortalSize();		
			
			selectPortal = min(selectPortal, numPortal);

			PORTAL_LIST::const_iterator	iPortal = sector.GetPortalBegin();

			for (int i=0; i<selectPortal; i++)
			{
				iPortal++;		
			}		

			PORTAL_INFO portalInfo = *iPortal;

			int zoneID = portalInfo.ZoneID;

			// zoneID로 이동한다.
			//-----------------------------------------------------
			// Packet 보내기
			//-----------------------------------------------------
				CGSelectPortal _CGSelectPortal;

				_CGSelectPortal.setZoneID( zoneID );

				g_pSocket->sendPacket( &_CGSelectPortal );
		}
	}

	gC_vs_ui.CloseElevator();
}

//-----------------------------------------------------------------------------
//
// Server 선택할 때
//
//-----------------------------------------------------------------------------
/*
void	
UIMessageManager::Execute_UI_SELECT_SERVER(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_SELECT_SERVER");

	
	if (g_Mode!=MODE_WAIT_SELECTPC)
	{
		DEBUG_ADD("Not Mode MODE_WAIT_SELECTPC");
		return;
	}

	
	// left : serverGroupID
	int selectedGroup = left;


	//-----------------------------------------------------
	// Server정보 갱신
	//-----------------------------------------------------
	if (g_pServerInformation!=NULL)
	{
		//-----------------------------------------------------
		// 서버 이름 읽어오기
		//-----------------------------------------------------
		const ServerGroup* pGroup = g_pServerInformation->GetData( selectedGroup );

		if (pGroup!=NULL)
		{
			const char* pGroupName = pGroup->GetGroupName();

			//-----------------------------------------------------
			// 현재 선택되어 있는 server랑 다르면...
			//-----------------------------------------------------
			if (pGroupName != g_pServerInformation->GetServerGroupName())
			{
				char str[80];
				strcpy(str, pGroupName);	
				
				// UI에 설정
				//gC_vs_ui.SetServerDefault( str, selectedGroup );

			
				//-----------------------------------------------------
				// Packet 보내기
				//-----------------------------------------------------
				#ifdef CONNECT_SERVER
					CLChangeServer _CLChangeServer;

					_CLChangeServer.setServerGroupID( selectedGroup );

					g_pSocket->sendPacket( &_CLChangeServer );

					gC_vs_ui.CharManagerDisable();
				#endif

				// Server정보에 설정
				g_pServerInformation->SetServerGroupName( pGroupName );			
			}
		}
	}
}
*/

//-----------------------------------------------------------------------------
//
// Server 선택할 때
//
//-----------------------------------------------------------------------------
/*
void	
UIMessageManager::Execute_UI_REQUEST_SERVER_LIST(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_REQUEST_SERVER_LIST");

	
	if (g_Mode!=MODE_WAIT_SELECTPC)
	{
		DEBUG_ADD("Not Mode MODE_WAIT_SELECTPC");
		return;
	}

	
	#ifdef CONNECT_SERVER
		if (gC_vs_ui.IsCharManagerEnable())
		{
			CLGetServerList _CLGetServerList;					
			
			g_pSocket->sendPacket( &_CLGetServerList );

			gC_vs_ui.CharManagerDisable();
		}
	#endif
}
*/

//-----------------------------------------------------------------------------
//
// Inventory --> QuickSlot
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_ITEM_TO_QUICKITEMSLOT(int left, int right, void* void_ptr)
{
//	DEBUG_ADD("[UI] UI_ITEM_TO_QUICKITEMSLOT");
//	// focus_grid_x, focus_grid_y, (MItem *)p_item
//
//	
//	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
//	{
//		DEBUG_ADD("Not Mode MODE_GAME or Dead");
//		return;
//	}
//				
//	//-----------------------------------------------------------------
//	// 검증받을게 없는 경우
//	//-----------------------------------------------------------------
//	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL
//		&& g_pPlayer->IsItemCheckBufferNULL())
//	{
//		if (gC_vs_ui.IsRunningExchange())
//		{
//			// 교환 중에는 그냥 Mouse에 붙인다.
//			Execute_UI_ITEM_PICKUP_FROM_INVENTORY(left, right, void_ptr);
//		}
//		else
//		{
//			MItem* pItem = (MItem*)void_ptr;
//
//			//---------------------------------------------------------
//			// 쌓일 수 있는 아이템이고..
//			// QuickSlot이 있는지 확인
//			//---------------------------------------------------------
//			if (pItem->IsQuickItem() && g_pQuickSlot!=NULL)
//			{		
//				int itemID = pItem->GetID();
//				int itemX = pItem->GetGridX();
//				int itemY = pItem->GetGridY();
//
//				int num = g_pQuickSlot->GetSize();
//				int addTotal = -1;
//
//				int addIndex = -1;
//
//				//---------------------------------------------------------
//				// QuickSlot의 어디에 들어갈 수 있을까?
//				//---------------------------------------------------------
//				for (int i=0; i<num; i++)
//				{
//					MItem* pQuickItem = g_pQuickSlot->GetItem( i );
//
//					//---------------------------------------------------------
//					// 아무것도 없는 곳이면 그냥 넣으면 된다.
//					//---------------------------------------------------------
//					if (pQuickItem==NULL)
//					{						
//						addIndex = i;
//
//						MItem* pRemoveItem = g_pInventory->RemoveItem( itemX, itemY );
//
//						g_pQuickSlot->AddItem( pRemoveItem, addIndex );
//
//						PlaySound( pRemoveItem->GetInventorySoundID() );
//
//						break;
//					}
//					//---------------------------------------------------------
//					// 뭔가 있으면.. 그곳에 쌓일 수 있는지 알아본다.
//					//---------------------------------------------------------
//					else
//					{
//						//--------------------------------------------------------
//						// 쌓일 수 있는 item인지 검증해 준다.
//						//--------------------------------------------------------
//						if (pQuickItem->GetItemClass()==pItem->GetItemClass()
//							&& pQuickItem->GetItemType()==pItem->GetItemType())
//						{
//							//----------------------------------------------------
//							// 더한 개수가 max를 넘지 않아야 한다.
//							//----------------------------------------------------
//							addTotal = pQuickItem->GetNumber() + pItem->GetNumber();
//							if ( addTotal <= pQuickItem->GetMaxNumber() )
//							{
//								// i번째에 추가 가능하다고 판단한다.								
//								addIndex = i;
//
//								//---------------------------------------------------
//								// Inventory의 아이템은 제거한다.
//								//---------------------------------------------------
//								MItem* pRemoveItem = g_pInventory->RemoveItem( itemX, itemY );
//								if (pRemoveItem!=NULL)
//								{
//									// assert( pRemoveItem==pItem );
//									delete pRemoveItem;
//								}
//
//								pQuickItem->SetNumber( addTotal );
//
//								PlaySound( pQuickItem->GetInventorySoundID() );
//								
//								break;
//							}
//						}
//					}
//				}
//
//				if (addIndex != -1)
//				{
//					//---------------------------------------------------------
//					// SendPacket - Inventory to QuickItem
//					// 실제로는 Inventory --> Mouse --> QuickItem이다.
//					//---------------------------------------------------------
//						//---------------------------------------------------
//						// Inventory에 있던 item을 mouse에 붙였다(-_-;)고
//						// server로 packet을 보낸다.
//						//---------------------------------------------------
//						CGAddInventoryToMouse _CGAddInventoryToMouse;
//						_CGAddInventoryToMouse.setObjectID( itemID );
//						_CGAddInventoryToMouse.setX( itemX );
//						_CGAddInventoryToMouse.setY( itemY );
//						
//						g_pSocket->sendPacket( &_CGAddInventoryToMouse );
//
//						//---------------------------------------------------
//						// mouse에 있던 item을 QuickSlot에 추가했다고
//						// server로 packet을 보낸다.
//						//---------------------------------------------------
//						CGAddMouseToQuickSlot _CGAddMouseToQuickSlot;
//						_CGAddMouseToQuickSlot.setObjectID( itemID );
//						_CGAddMouseToQuickSlot.setSlotID( addIndex );
//						
//						g_pSocket->sendPacket( &_CGAddMouseToQuickSlot );
//						
//				}
//
//			}
//			//---------------------------------------------------------
//			// Quick slot에 안 들어가는 거면..
//			// Gear로 넣어버리자. -_-;
//			//---------------------------------------------------------
//			else
//			{
//				//---------------------------------------------------------
//				// 일단..  mouse로 들고..
//				//---------------------------------------------------------
//				g_pInventory->RemoveItem( pItem->GetID() );
//				Execute_UI_ITEM_PICKUP_FROM_INVENTORY(left, right, void_ptr);
//
//				int maxSlot;
//
//				//---------------------------------------------------------
//				// gear에 들어갈 수 있는지 체크
//				//---------------------------------------------------------
//				if (g_pPlayer->IsSlayer())
//				{
//					maxSlot = MSlayerGear::MAX_GEAR_SLAYER;
//				}
//				else
//				{
//					maxSlot = MVampireGear::MAX_GEAR_VAMPIRE;
//				}
//
//				MItem* pGearItem = NULL;
//				MItem* pChangeItem = NULL;
//				int addSlot = -1;
//
//				for (int slot=0; slot<maxSlot; slot++)
//				{
//					MItem* pGearItem = NULL;
//
//					if (g_pPlayer->IsSlayer() && g_pSlayerGear->CanReplaceItem(pItem, (MSlayerGear::GEAR_SLAYER)slot, pGearItem)
//						|| g_pPlayer->IsVampire() && g_pVampireGear->CanReplaceItem(pItem, (MVampireGear::GEAR_VAMPIRE)slot, pGearItem)
//						|| g_pPlayer->IsOusters() && g_pOustersGear->CanReplaceItem(pItem, (MOustersGear::GEAR_OUSTERS)slot, pGearItem))
//					{
//						addSlot = slot;
//						pChangeItem = pGearItem;
//
//						// 빈 곳이면 바로 넣어버리면 된다.
//						// 빈 곳이 아니라면.. 다음걸 찾는다.
//						if (pGearItem==NULL)
//						{
//							break;
//						}							
//					} 
//				}
//				
//				//---------------------------------------------------------
//				// gear에 넣자..
//				//---------------------------------------------------------
//				if (addSlot != -1)
//				{
//					Execute_UI_ITEM_DROP_TO_GEAR(addSlot, 0, pChangeItem);
//				}
//			}
//		}		
//	}
//	//-----------------------------------------------------------------
//	// 검증 받아야할 다른 아이템이 있는 경우
//	//-----------------------------------------------------------------
//	else
//	{
//		DEBUG_ADD( "[Error] There is another item in ItemCheckBuffer");
//	}
}

//-----------------------------------------------------------------------------
//
// Close Slayer Portal
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_CLOSE_SLAYER_PORTAL(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CLOSE_SLAYER_PORTAL");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}


		CGSelectWayPoint _CGSelectWayPoint;
		
		_CGSelectWayPoint.setZoneID( 0 );
		_CGSelectWayPoint.setX( 0 );
		_CGSelectWayPoint.setY( 0 );

		g_pSocket->sendPacket( &_CGSelectWayPoint );

	g_pZone->RemoveHelicopter( g_pPlayer->GetID() );

	UI_CloseSelectWayPoint();
}

//-----------------------------------------------------------------------------
//
// Slayer Portal
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_SLAYER_PORTAL(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_SLAYER_PORTAL");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}


	int zoneID = left;
	int zoneX = HIWORD(right);
	int zoneY = LOWORD(right);
	
	int cost = int( pow(g_pPlayer->GetLEVEL(), 1.3) * 100 );

//	if( g_pMoneyManager->GetMoney() < cost && g_pPlayer->IsOusters() )
//	{
//		UI_PopupMessage( STRING_MESSAGE_WAR_NOT_ENOUGH_MONEY );
//		return;
//	}
	
		CGSelectWayPoint _CGSelectWayPoint;
		
		_CGSelectWayPoint.setZoneID( zoneID );
		_CGSelectWayPoint.setX( zoneX );
		_CGSelectWayPoint.setY( zoneY );

		g_pSocket->sendPacket( &_CGSelectWayPoint );


	if(g_pPlayer->IsSlayer())
	{
		g_pZone->RemoveHelicopter( g_pPlayer->GetID() );
		UI_CloseSelectWayPoint();
	}
	else if(g_pPlayer->IsOusters())
		gC_vs_ui.CloseHorn();
		
}

//-----------------------------------------------------------------------------
//
// 파티 요청받은거 닫을때
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_FINISH_REQUEST_PARTY_BUTTON(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_FINISH_REQUEST_PARTY_BUTTON");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}


	BOOL bTimeout = left;

	UI_ClosePartyRequest();

	if (bTimeout)
	{
			// 파티 초대를 거부한다.
			CGPartyInvite _CGPartyInvite;
			_CGPartyInvite.setTargetObjectID( g_pTempInformation->PartyInviter );
			_CGPartyInvite.setCode( CG_PARTY_INVITE_REJECT );	

			g_pSocket->sendPacket( &_CGPartyInvite );		
	}
}

//-----------------------------------------------------------------------------
//
// 파티 요청받은거 취소
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_PARTY_REQUEST_CANCEL(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_PARTY_REQUEST_CANCEL");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}


	
	//---------------------------------------------------------
	// 파티할래? Y/N에 대한 응답
	//---------------------------------------------------------
	if (g_pTempInformation->GetMode() == TempInformation::MODE_PARTY_REQUEST)
	{	
		if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_PARTY)
		{
			g_pPlayer->SetWaitVerifyNULL();
		}

			CGPartyInvite _CGPartyInvite;
			_CGPartyInvite.setTargetObjectID( g_pTempInformation->Value1 );
			_CGPartyInvite.setCode( CG_PARTY_INVITE_CANCEL );				

			g_pSocket->sendPacket( &_CGPartyInvite );			
	
		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
		
		// 파티 취소할래?를 제거한다.
		UI_ClosePartyCancel();
	}	
}

//-----------------------------------------------------------------------------
//
// 파티 요청 응답
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_PARTY_ACCEPT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_PARTY_ACCEPT");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}


	BOOL accept = (BOOL)left;

	int code;

	//---------------------------------------------------------
	// 교환할래? Y/N에 대한 응답
	//---------------------------------------------------------
	//if (g_pTempInformation->Mode == TempInformation::MODE_PARTY_REQUEST)
	{
		//---------------------------------------------------------
		// 허용
		//---------------------------------------------------------
		if (accept)
		{
			code = CG_PARTY_INVITE_ACCEPT;
		}
		//---------------------------------------------------------
		// 거부
		//---------------------------------------------------------
		else
		{	
			//if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_TRADE)
			{
			//	g_pPlayer->SetWaitVerifyNULL();
			}

			code = CG_PARTY_INVITE_REJECT;
		}

			CGPartyInvite _CGPartyInvite;
			_CGPartyInvite.setTargetObjectID( g_pTempInformation->PartyInviter );
			_CGPartyInvite.setCode( code );				

			g_pSocket->sendPacket( &_CGPartyInvite );			
	
		//g_pTempInformation->Mode = TempInformation::MODE_NULL;		
	}
}

//-----------------------------------------------------------------------------
//
// Close Party Manager
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_CLOSE_PARTY_MANAGER(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CLOSE_PARTY_MANAGER");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}


	UI_CloseParty();
}

//-----------------------------------------------------------------------------
//
// 파티 탈퇴/추방
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_AWAY_PARTY(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_AWAY_PARTY");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}


	// 파티 탈퇴 추방 
	// left :: -1 == 탈퇴, 0~4 == 추방
	BOOL bLeftParty = (left==-1);

	if (g_pPlayer->IsWaitVerifyNULL())
	{
		//-----------------------------------------------------------------
		// 내가 파티를 떠나는 경우
		//-----------------------------------------------------------------
		if (bLeftParty)
		{
			if (g_pParty->GetSize() > 0)
			{
					CGPartyLeave _CGPartyLeave;
				
					//_CGPartyLeave.setTargetName( g_pUserInformation->CharacterID.GetString() );
					
					g_pSocket->sendPacket( &_CGPartyLeave );
					
					g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_PARTY_LEAVE );
			}
		}
		//-----------------------------------------------------------------
		// 내가 다른 누구를 추방하는 경우
		//-----------------------------------------------------------------
		else
		{
			if (g_pParty->IsKickAvailableTime())
			{
				int kickIndex = left;

				PARTY_INFO*	pInfo = g_pParty->GetMemberInfo( kickIndex );

				if (pInfo!=NULL)
				{
						CGPartyLeave _CGPartyLeave;
					
						_CGPartyLeave.setTargetName( pInfo->Name.GetString() );
						
						g_pSocket->sendPacket( &_CGPartyLeave );			

						g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_PARTY_LEAVE );
				}
			}
			else
			{
				g_pGameMessage->Add( (*g_pGameStringTable)[STRING_MESSAGE_NO_AUTHORITY].GetString() );
			}
		}		
	}
}


//-----------------------------------------------------------------------------
//
// 파티 탈퇴/추방
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_FINISH_REQUEST_DIE_BUTTON(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_FINISH_REQUEST_DIE_BUTTON");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}


	// 다이-_-요청 닫기 left == TRUE : timeout
	BOOL bTimeout = left;

	if (g_pPlayer->IsDead() && !g_pPlayer->IsWaitVerify())
	{
		int TempZoneID = g_pPlayer->GetResurrectZoneID();
		if(TempZoneID)
		{
			CGSelectWayPoint _CGSelectWayPoint;

			_CGSelectWayPoint.setZoneID( TempZoneID );
			_CGSelectWayPoint.setX( 0 );
			_CGSelectWayPoint.setY( 0 );

			g_pSocket->sendPacket( &_CGSelectWayPoint );

			g_pPlayer->SetResurrectZoneID(0);
		}
		else
		{
			CGResurrect _CGResurrect;
			
			g_pSocket->sendPacket( &_CGResurrect );
		}	
			// 이거말고 Verify를 하나 만들어야 되는데
			// 컴파일 하기 싫어서 일단... - -;
		g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_RESURRECT );

		gC_vs_ui.FinishRequestDie();
		gC_vs_ui.FinishRequestResurrect();
		gC_vs_ui.CloseRequestShrineMinimap();
	}
}

//-----------------------------------------------------------------------------
//
// 서버 선택할 때
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_CONNECT_SERVER(int left, int right, void* void_ptr)
{
//	DEBUG_ADD_FORMAT("[UI] UI_CONNECT_SERVER. %s(%d)", (left? "group" : "server"), right);

	// left == true : group
	//         false : server
    //  right : id

	if(NULL == g_pSocket)
		return;
	
	BOOL bSelectGroup = left;
	int	selectID = right;

	//-------------------------------------------------------
	// World를 선택한 경우
	//-------------------------------------------------------
	if (bSelectGroup)
	{
		if (g_Mode==MODE_WAIT_SELECT_WORLD)
		{
			// CLSelectWorld
				CLSelectWorld clSelectWorld;
			
				clSelectWorld.setWorldID( selectID );				
				g_pUserInformation->WorldID = selectID;

				g_pSocket->sendPacket( &clSelectWorld );				

			g_pServerInformation->SetServerGroupID( selectID );

			SetMode( MODE_WAIT_SERVER_LIST );
		}
		else
		{
			DEBUG_ADD("Not MODE_WAIT_SELECT_WORLD");
		}
	}
	//-------------------------------------------------------
	// Server를 선택한 경우
	//-------------------------------------------------------
	else
	{
		if (g_Mode==MODE_WAIT_SELECT_SERVER)
		{
			// CLSelectServer
				CLSelectServer clSelectServer;
			
				clSelectServer.setServerGroupID( selectID );				
				g_pUserInformation->ServerID = selectID;

				g_pSocket->sendPacket( &clSelectServer );				

				g_pSocket->setPlayerStatus( CPS_AFTER_SENDING_CL_GET_PC_LIST );

			g_pServerInformation->SetServerID( selectID );			

			SetMode( MODE_WAIT_PCLIST );
		}
		else
		{
			DEBUG_ADD("Not MODE_WAIT_SELECT_SERVER");
		}
	}
		
}

//-----------------------------------------------------------------------------
//
// server 선택 닫을 때
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_CLOSE_SERVER_SELECT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CLOSE_SERVER_SELECT");

	// left == true : group 
	//         false : server
	BOOL bSelectGroup = left;

	//------------------------------------------------------------
	// group선택에서 back하면 초기화면으로
	//------------------------------------------------------------
	if (bSelectGroup)
	{
		SetMode( MODE_MAINMENU );
	}
	//------------------------------------------------------------
	// server선택에서 back하면 group선택으로..
	//------------------------------------------------------------
	else
	{
		UI_SetWorldList();

		SetMode( MODE_WAIT_SELECT_WORLD );
	}	
}

//-----------------------------------------------------------------------------
//
// 캐릭터 이름 확인할때
//
//-----------------------------------------------------------------------------
void	
UIMessageManager::Execute_UI_NEWCHARACTER_CHECK(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_NEWCHARACTER_CHECK");

	char* pName = (char*)void_ptr;

	if (pName!=NULL)
	{
		if (g_pUserInformation->WhisperID.GetLength()==0)
		{
			//---------------------------------------------
			// 이름 길이 체크
			//---------------------------------------------
			BOOL bAllOK = TRUE;

			int len = strlen(pName);	
			
			if (len<PlayerInfo::minIDLength || len>PlayerInfo::maxIDLength)
			{
				char strTemp[128];
				sprintf(strTemp, (*g_pGameStringTable)[STRING_USER_REGISTER_ID_LENGTH].GetString(), PlayerInfo::minIDLength, PlayerInfo::maxIDLength);
				g_pUIDialog->PopupFreeMessageDlg( strTemp );
				bAllOK = FALSE;
			}

			if (bAllOK)
			{
				if (!IsValidID(pName, "-_"))
				{
					//---------------------------------------------
					// 잘못된 ID인 경우
					//---------------------------------------------						
					g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
					bAllOK = FALSE;
				}
				else
				{
					char strName[80];
					strcpy(strName, pName);

					// 안 좋은 말이 들어있는 경우는 허용이 안된다
					if (g_pChatManager->RemoveCurse(strName))
					{
						g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
						bAllOK = FALSE;
					}
					else
					{
						MChatManager badNameManager;
						badNameManager.LoadFromFile( g_pFileDef->getProperty("FILE_INFO_CHAT_2").c_str());

						if (badNameManager.RemoveCurse(strName))
						{
							g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
							bAllOK = FALSE;
						}
					}
				}
			}

			if (bAllOK)
			{
					CLQueryCharacterName clQueryCharacterName;

					clQueryCharacterName.setCharacterName( pName );

					g_pSocket->sendPacket( &clQueryCharacterName );

					// 임시로.. -_-;;
					g_pUserInformation->WhisperID = pName;
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
// 지뢰/폭탄 만들꺼 선택할때
//
//-----------------------------------------------------------------------------
/*
void
UIMessageManager::Execute_UI_SELECT_EXPLOSIVE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_SELECT_EXPLOSIVE");
	
	int type = left;		// 지뢰(2) / 폭탄(3)
	int itemType = right;	// 종류(지뢰:0~3, 폭탄:0~5)


	//--------------------------------------------------------
	// 검증받을게 없는 경우
	//--------------------------------------------------------
	if (g_pPlayer->IsItemCheckBufferNULL()
		&& g_pTempInformation->Mode==TempInformation::MODE_NULL)
	{
		//--------------------------------------------------------
		// 정상적인 경우
		//--------------------------------------------------------
		if (type==2 && itemType>=0 && itemType<6
			|| type==3 && itemType>=0 && itemType<4)
		{
			int itemClass = ((type==2)? ITEM_CLASS_MINE : ITEM_CLASS_BOMB);
				
			#ifdef CONNECT_SERVER
				CGMakeItem _CGMakeItem;
			
				_CGMakeItem.setItemClass( itemClass );
				_CGMakeItem.setItemType( itemType );

				g_pSocket->sendPacket( &_CGMakeItem );
				
			#endif	

			// item 기억
			g_pTempInformation->Mode = TempInformation::MODE_SKILL_MAKE_ITEM;
			g_pTempInformation->Value1 = itemClass;
			g_pTempInformation->Value2 = itemType;

			// 못 움직이게 막기
			UI_LockItem();
			
			// 선택창을 닫는다.
			UI_CloseSelectExplosive();
		}
		//--------------------------------------------------------
		// 뭔가 잘못된 경우
		//--------------------------------------------------------
		else
		{
			// 만들꺼리를 제대로 선택하시라~~
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_ERROR_ETC_ERROR].GetString() );
		}
	}
	else
	{
		DEBUG_ADD("ItemCheckBuffer is not NULL. wait~~" );
	}
}
*/

//-----------------------------------------------------------------------------
//
// 지뢰/폭탄 선택하는거 닫을 때
//
//-----------------------------------------------------------------------------
/*
void
UIMessageManager::Execute_UI_CLOSE_SELECT_EXPLOSIVE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CLOSE_SELECT_EXPLOSIVE");
	
	UI_CloseSelectExplosive();
}
*/

void
UIMessageManager::Execute_UI_CLOSE_TEAM_LIST(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CLOSE_TEAM_LIST");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.CloseTeamList();

		g_pUIDialog->UnSetLockInputPCTalk();
//		g_pUIDialog->ClosePCTalkDlg();
//		g_pPlayer->SetWaitVerifyNULL();
		// 다시 뭔가를?선택할 수 있게 한다.
		//g_pUIDialog->ShowPCTalkDlg();
	}
}

void
UIMessageManager::Execute_UI_CLOSE_TEAM_INFO(int left, int right, void* void_ptr)
{
	gC_vs_ui.CloseTeamInfo(); 
}

void
UIMessageManager::Execute_UI_CLOSE_TEAM_MEMBER_INFO(int left, int right, void* void_ptr)
{
	gC_vs_ui.CloseTeamMemberInfo();
}

void
UIMessageManager::Execute_UI_CLOSE_TEAM_REGIST(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CLOSE_TEAM_REGIST");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		// 다시 뭔가를?선택할 수 있게 한다.
		if(!gC_vs_ui.ReturnTeamRegistMember())
			g_pUIDialog->ShowPCTalkDlg();

		gC_vs_ui.CloseTeamRegist();
	}
}

void
UIMessageManager::Execute_UI_CLOSE_TEAM_MEMBER_LIST(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_CLOSE_TEAM_MEMBER_LIST");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	
	gC_vs_ui.CloseTeamMemberList();
//	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
//	{
//
//		// 다시 뭔가를?선택할 수 있게 한다.
//		g_pUIDialog->ShowPCTalkDlg();
//	}
}

void
UIMessageManager::Execute_UI_REQUEST_GUILD_INFO(int left, int right, void* void_ptr)
{
		CGSelectGuild  _CGSelectGuild ;
			
		_CGSelectGuild.setGuildID(g_pPlayer->GetGuildNumber());
		g_pSocket->sendPacket( &_CGSelectGuild  );				

}

	
void
UIMessageManager::Execute_UI_REQUEST_GUILD_MEMBER_LIST(int left, int right, void* void_ptr)
{
		CGRequestGuildMemberList  _CGRequestGuildMemberList ;
			
		g_pSocket->sendPacket( &_CGRequestGuildMemberList  );				

}

void
UIMessageManager::Execute_UI_MODIFY_GUILD_MEMBER(int left, int right, void* void_ptr)
{
		CGModifyGuildMember _CGModifyGuildMember;

		_CGModifyGuildMember.setGuildID(left);
		_CGModifyGuildMember.setGuildMemberRank(right);
		_CGModifyGuildMember.setName(std::string((char *)void_ptr));

		g_pSocket->sendPacket( &_CGModifyGuildMember );				

		gC_vs_ui.CloseTeamMemberInfo();
}

void
UIMessageManager::Execute_UI_SELECT_TEAM_MEMBER_LIST(int left, int right, void* void_ptr)
{
	char *pName = (char *)void_ptr;

	if(pName != NULL)
	{
			CGSelectGuildMember  _CGSelectGuildMember ;
			
			_CGSelectGuildMember.setName(std::string(pName));
			g_pSocket->sendPacket( &_CGSelectGuildMember  );				
	}
}


void
UIMessageManager::Execute_UI_SELECT_READY_TEAM_LIST(int left, int right, void* void_ptr)		// void_ptr = TEAM_NAME
{
	char *pName = (char *)void_ptr;

	if(pName != NULL)
	{
			CGSelectGuild  _CGSelectGuild ;
			
			_CGSelectGuild.setGuildID(left);
			g_pSocket->sendPacket( &_CGSelectGuild  );				
	}
}

void
UIMessageManager::Execute_UI_SELECT_REGIST_TEAM_LIST(int left, int right, void* void_ptr)		// void_ptr = TEAM_NAME
{
	char *pName = (char *)void_ptr;

	if(pName != NULL)
	{
			CGSelectGuild  _CGSelectGuild ;
			
			_CGSelectGuild.setGuildID(left);
			g_pSocket->sendPacket( &_CGSelectGuild  );				
	}
}

	
void
UIMessageManager::Execute_UI_JOIN_READY_TEAM(int left, int right, void* void_ptr)				// void_ptr = TEAM_NAME
{
	CGTryJoinGuild _CGTryJoinGuild;
	_CGTryJoinGuild.setGuildID(left);
	_CGTryJoinGuild.setGuildMemberRank(2);
	g_pSocket->sendPacket( &_CGTryJoinGuild );
}

void
UIMessageManager::Execute_UI_JOIN_REGIST_TEAM(int left, int right, void* void_ptr)			// void_ptr = TEAM_NAME
{
	CGTryJoinGuild _CGTryJoinGuild;
	_CGTryJoinGuild.setGuildID(left);
	_CGTryJoinGuild.setGuildMemberRank(3);
	g_pSocket->sendPacket( &_CGTryJoinGuild );
}

	
void
UIMessageManager::Execute_UI_REGIST_GUILD_MEMBER(int left, int right, void* void_ptr)			// void_ptr = introduction max:150byte 창 닫아줄것!
{

	if(void_ptr == NULL)
	{
		// 에러메세지 출력
		g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_GUILD_REGIST_FAIL_INTRO].GetString() );
		return;
	}

	CGJoinGuild _CGJoinGuild;
	_CGJoinGuild.setGuildID(left);
	_CGJoinGuild.setGuildMemberRank(right);
	_CGJoinGuild.setGuildMemberIntro(std::string((char *)void_ptr));
	g_pSocket->sendPacket( &_CGJoinGuild );

	gC_vs_ui.CloseTeamRegist();
	gC_vs_ui.CloseTeamInfo();
}

void
UIMessageManager::Execute_UI_REGIST_GUILD_TEAM(int left, int right, void* void_ptr)			// left = TEAM_NAME, void_ptr = introduction max:150byte 창 닫아줄것!
{

	if(void_ptr == NULL || (char *)left == NULL)
	{
		// 에러메세지 출력
		g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_GUILD_REGIST_FAIL_INTRO].GetString() );
		return;
	}

	CGRegistGuild _CGRegistGuild;
	_CGRegistGuild.setGuildName(std::string((char *)left));
	_CGRegistGuild.setGuildIntro(std::string((char *)void_ptr));
	g_pSocket->sendPacket( &_CGRegistGuild );

	gC_vs_ui.CloseTeamRegist();
}

	
void
UIMessageManager::Execute_UI_CLOSE_FILE_DIALOG(int left, int right, void* void_ptr)
{
	switch(left)
	{
	case C_VS_UI_FILE_DIALOG::MODE_PROFILE_SELECT:
		if(void_ptr != NULL)
		{
			char *p_str = (char *)void_ptr;
			const int str_size = strlen(p_str);

			if(str_size > 4)
			{
				const POINT bigSize = { 55, 70 };
				
				CDirectDrawSurface surface;

				surface.InitOffsurface( bigSize.x, bigSize.y, DDSCAPS_SYSTEMMEMORY );
				
				RECT destBigRect = { 0, 0, bigSize.x, bigSize.y };

				CDirectDrawSurface bmpSurface;
				
				if (LoadImageToSurface(p_str, bmpSurface))
				{
					// surface의 크기가 default Profile크기와 다르다면
					// size를 변경시켜줘야 한다..	
					RECT bmpRect = { 0, 0, bmpSurface.GetWidth(), bmpSurface.GetHeight() };

					// BigSize
					surface.FillSurface( 0 );
					surface.Blt(&destBigRect, &bmpSurface, &bmpRect);

					char saveBmpName[512];
					strcpy(saveBmpName, g_pFileDef->getProperty("DIR_PROFILE").c_str());
					strcat(saveBmpName, "\\");
					strcat(saveBmpName, g_char_slot_ingame.sz_name.c_str());
					strcat(saveBmpName, ".bmp");
					surface.SaveToBMP(saveBmpName);

					g_pProfileManager->InitProfiles();
					gC_vs_ui.RefreshInfoImage();

				}

			}
		}
		break;
	}
	gC_vs_ui.CloseFileDialog();
}

//-----------------------------------------------------------------------------
//
// mouse의 Item(EVENT_STAR)으로 Inventory의 Item을 Enchant할 때
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ENCHANT_ACCEPT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ENCHANT_ACCEPT");

	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}

	// gpC_mouse_pointer->GetPickUpItem()		// 들고 있는 것
	//void_ptr = MItem *		// 들어갈 곳
	// Inventory좌표(left,right)
	MItem* pItem = (MItem*)void_ptr;
	MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();

	if (pMouseItem==NULL)
	{
		DEBUG_ADD("[Error] Mouse Item is NULL");
		return;
	}

	if (pItem==NULL)
	{
		DEBUG_ADD("[Error] InvenItem is NULL");
		return;
	}

	TYPE_OBJECTID mouseItemID = ((pMouseItem==NULL)?OBJECTID_NULL : pMouseItem->GetID());

	// 교환중일때는 Enchant할 수 없다!
	if (g_pTradeManager!=NULL)
		return;
	
	//g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_INSERT_FROM_INVENTORY );
	
	//----------------------------------------------------
	// Server에 접속한 경우
	//----------------------------------------------------
	if(g_pTempInformation->GetMode() == TempInformation::MODE_NULL && g_pPlayer->IsItemCheckBufferNULL())
	{
		CGAddItemToItem _CGAddItemToItem; 
		_CGAddItemToItem.setObjectID( mouseItemID );
		_CGAddItemToItem.setX( pItem->GetGridX() );
		_CGAddItemToItem.setY( pItem->GetGridY() );								
		
		g_pSocket->sendPacket( &_CGAddItemToItem );
		
		g_pPlayer->SetItemCheckBuffer( pMouseItem, MPlayer::ITEM_CHECK_BUFFER_ITEM_TO_ITEM );
		g_pTempInformation->SetMode(TempInformation::MODE_ITEM_TO_ITEM);
		g_pTempInformation->Value1 = pItem->GetGridX();
		g_pTempInformation->Value2 = pItem->GetGridY();

//		UI_LockItem();
	}
}

void
UIMessageManager::Execute_UI_ENCHANT_CANCEL(int left, int right, void* void_ptr)
{
	UI_CloseEnchant();
}

void 
UIMessageManager::Execute_UI_MESSAGE_BOX(int left, int right, void* void_ptr)
{
	if(left < MAX_GAME_STRING)
		g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[left].GetString() );
}


void 
UIMessageManager::Execute_UI_CLOSE_OTHER_INFO(int left, int right, void* void_ptr)
{
	gC_vs_ui.CloseOtherInfo();
}

void UIMessageManager::Execute_UI_MODIFY_TEAM_INFO(int left, int right, void* void_ptr)
{
	if(void_ptr != NULL)
	{
		CGModifyGuildIntro _CGModifyGuildIntro;
		_CGModifyGuildIntro.setGuildID(left);
		_CGModifyGuildIntro.setGuildIntro(std::string((char *)void_ptr));
		g_pSocket->sendPacket( &_CGModifyGuildIntro );
	}
}

void UIMessageManager::Execute_UI_MODIFY_GUILD_MEMBER_INTRO(int left, int right, void* void_ptr)
{
	if(void_ptr != NULL)
	{
		CGModifyGuildMemberIntro _CGModifyGuildMemberIntro;
		_CGModifyGuildMemberIntro.setGuildID(left);
		_CGModifyGuildMemberIntro.setGuildMemberIntro(std::string((char *)void_ptr));
		g_pSocket->sendPacket( &_CGModifyGuildMemberIntro );
	}
}

void UIMessageManager::Execute_UI_SEND_NAME_FOR_SOUL_CHAIN(int left, int right, void* void_ptr)
{
	// 이미 추적버튼을 누른상태일경우
	if(g_pPlayer->IsWaitVerify())
		return;

	if(void_ptr != NULL)
	{
		CGSkillToNamed _CGSkillToNamed;
		_CGSkillToNamed.setSkillType(SKILL_SOUL_CHAIN);
		_CGSkillToNamed.setTargetName(std::string((char *)void_ptr));
		_CGSkillToNamed.setCEffectID(0);
		g_pSocket->sendPacket( &_CGSkillToNamed );

		g_pPlayer->SetWaitVerify(MPlayer::WAIT_VERIFY_SKILL_SUCCESS);

//		(*g_pSkillInfoTable)[SKILL_SOUL_CHAIN].SetNextAvailableTime();
	}
}

void UIMessageManager::Execute_UI_CLOSE_TRACE_WINDOW(int left, int right, void* void_ptr)
{
//	//------------------------------------------------------------------
//	// Player가 기다리던 skill의 성공유무를 검증받았다.
//	//------------------------------------------------------------------	
//	if (g_pPlayer->GetWaitVerify()==MPlayer::WAIT_VERIFY_SKILL_SUCCESS)
//	{		
//		g_pPlayer->SetWaitVerifyNULL();
//	}
//	else
//	{
//		DEBUG_ADD("[Error] Player is not WaitVerifySkillSuccess");
//	}
//
//	//------------------------------------------------------------------
//	// Item Lock을 푼다.
//	//------------------------------------------------------------------
//	if (g_pPlayer->GetItemCheckBufferStatus()==MPlayer::ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY)
//	{
//		g_pPlayer->ClearItemCheckBuffer();
//	}

	gC_vs_ui.CloseTraceWindow();
}

// 넷마블용 수정
void UIMessageManager::Execute_UI_RUN_CONNECT(int left, int right, void* void_ptr)
{
	UI_RunConnect();
}

void UIMessageManager::Excute_UI_SELECT_GRADE_SKILL(int left, int right, void* void_ptr)
{
	if(left >= g_pRankBonusTable->GetSize() || (*g_pRankBonusTable)[left].GetStatus() == RankBonusInfo::STATUS_CANNOT_LEARN || (*g_pRankBonusTable)[left].GetStatus() == RankBonusInfo::STATUS_LEARNED
		|| !g_pPlayer->IsWaitVerifyNULL() || g_pTempInformation->GetMode() != TempInformation::MODE_NULL)
		return;

	CGSelectRankBonus _CGSelectRankBonus;
	_CGSelectRankBonus.setRankBonusType(left);
	g_pSocket->sendPacket( &_CGSelectRankBonus );
	g_pTempInformation->SetMode(TempInformation::MODE_SKILL_LEARN);

}

void UIMessageManager::Excute_UI_USE_XMAS_TREE(int left, int right, void* void_ptr)				// left = to, right = from, void_ptr = message
{
	DEBUG_ADD("Excute_UI_USE_XMAS_TREE");
	MItem *pItem = (MItem *)left;

	if(pItem == NULL  || !g_pPlayer->IsItemCheckBufferNULL())
		return;

	char *sz_ptr = (char *)void_ptr;

	if(sz_ptr == NULL)
	{
		// 빈항목이 있습니다.
		g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_XMAS_CARD_CANNOT_USE].GetString() );
		return;
	}

	CGUseMessageItemFromInventory _CGUseMessageItemFromInventory;
	_CGUseMessageItemFromInventory.setObjectID( pItem->GetID() );
	_CGUseMessageItemFromInventory.setX( pItem->GetGridX() );
	_CGUseMessageItemFromInventory.setY( pItem->GetGridY() );
	_CGUseMessageItemFromInventory.setMessage(sz_ptr);
					
	g_pSocket->sendPacket( &_CGUseMessageItemFromInventory );
					
	g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);

	if(right == 1)
		gC_vs_ui.CloseBulletinBoardWindow();
	else
		gC_vs_ui.CloseXmasCardWindow();
	
	DEBUG_ADD("Excute_UI_USE_XMAS_TREE OK");
}

void UIMessageManager::Excute_UI_CLOSE_XMAS_CARD_WINDOW(int left, int right, void* void_ptr)
{
	gC_vs_ui.CloseXmasCardWindow();
}


void UIMessageManager::Excute_UI_SEND_BRING_FEE(int left,int right, void* void_ptr)
{
	// 검증할게 있으면 리턴.
	if(g_pTempInformation->GetMode() != TempInformation::MODE_NULL)
	{
		DEBUG_ADD("Excute_UI_SEND_BRING_FEE TempInformation is not NULL");
		return;
	}
	// 세금 가져오기
	DEBUG_ADD("Excute_UI_SEND_BRING_FEE");
	// 가져오기 했을땐 검증 패킷 날라오기전까진 냅두자.

	g_pTempInformation->SetMode(TempInformation::MODE_WAIT_BRING_FEE);
	g_pTempInformation->Value1 = left;

	CGWithdrawTax _CGWithdrawTax;

	_CGWithdrawTax.setGold((DWORD)left);
	g_pSocket->sendPacket( &_CGWithdrawTax );

	DEBUG_ADD("Excute_UI_SEND_BRING_FEE OK");
}

void UIMessageManager::Excute_UI_CLOSE_BRING_FEE_WINDOW(int left,int right, void* void_ptr)
{
	// 검증할게 없을경우
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.CloseBringFeeWindow();
		g_pUIDialog->ShowPCTalkDlg();
	}
}

void UIMessageManager::Excute_UI_CLOSE_WAR_LIST(int left, int right, void *void_ptr)
{
	gC_vs_ui.CloseWarListWindow();	
}

void UIMessageManager::Execute_UI_CLOSE_BLOOD_BIBLE_STATUS(int left, int right, void *void_ptr)
{
	gC_vs_ui.CloseBloodBibleStatus();	
}

void UIMessageManager::Execute_UI_SEND_NAME_FOR_COUPLE(int left, int right, void *void_ptr)
{	
	if( g_pPlayer->GetWaitVerify() != MPlayer::WAIT_VERIFY_NULL 
		&& g_pTempInformation->GetMode()!=TempInformation::MODE_NULL)
	{
		g_pUIDialog->ClosePCTalkDlg();
		return;
	}

	int npcID = (*g_pPCTalkBox).GetNPCID();
	
	CGTypeStringList _CGTypeStringList;

	_CGTypeStringList.setType((CGTypeStringList::StringType) left);
	_CGTypeStringList.addString( std::string((char*) void_ptr) );	
	_CGTypeStringList.setParam( npcID );
	
	g_pSocket->sendPacket( &_CGTypeStringList );

//	gC_vs_ui.CloseInputNameWindow();
	gC_vs_ui.SetDisableInputName();

	g_pPlayer->SetWaitVerify(MPlayer::WAIT_VERIFY_NPC_ASK);
	g_pTempInformation->SetMode(TempInformation::MODE_INPUT_NAME);
	g_pTempInformation->Value1 = (int)timeGetTime();
}

void UIMessageManager::Execute_UI_CLOSE_INPUT_NAME_WINDOW(int left, int right, void *void_ptr)
{
	if( g_pPlayer->GetWaitVerify() != MPlayer::WAIT_VERIFY_NPC_ASK	)
	{
		g_pUIDialog->ClosePCTalkDlg();
		gC_vs_ui.CloseInputNameWindow();
		g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_NULL );
	}  else
	{
		if(g_pTempInformation->GetMode() == TempInformation::MODE_INPUT_NAME)
		{
			DWORD dwTime = (DWORD) g_pTempInformation->Value1;
			
			if( timeGetTime() - dwTime >= 1000*60 )
			{
				g_pUIDialog->ClosePCTalkDlg();
				gC_vs_ui.CloseInputNameWindow();
				g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_NULL );
				g_pTempInformation->SetMode( TempInformation::MODE_NULL );
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
// Gear에서 Item을 사용한 경우
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_USE_GEAR(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_USE_GEAR");

	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}
	//
	// left = slot
	// right = by FunctionKey
	//
	//MItem* pItem = (MItem*)void_ptr;
	if (g_pPlayer!=NULL && (g_pPlayer->IsSlayer() && g_pSlayerGear!=NULL || g_pPlayer->IsVampire() && g_pVampireGear!= NULL || g_pPlayer->IsOusters() && g_pOustersGear!= NULL))
	{
		MItem* pSlotItem = static_cast<MItem*>(void_ptr);

		if (pSlotItem!=NULL && pSlotItem->IsAffectStatus())
		{

			if (g_pPlayer->IsItemCheckBufferNULL())
			{
				// 자기종족 아이템만 쓰쟈-ㅅ-;
				if(g_pPlayer->IsSlayer() && pSlotItem->IsSlayerItem() ||
					g_pPlayer->IsVampire() && pSlotItem->IsVampireItem() ||
					g_pPlayer->IsOusters() && pSlotItem->IsOustersItem())
				{
					pSlotItem->UseQuickItem();
				}
			}
			//----------------------------------------------------
			// item check buffer not null
			//----------------------------------------------------
			else
			{
				DEBUG_ADD("[Wait] Item Check Buffer is Not NULL");
			} 
		}
		else
		{
			DEBUG_ADD_FORMAT("[Error] There is no item in slot=%d", left);
		}
	}
	else
	{
		DEBUG_ADD("[Error] There is no Gear");
	}

}

void
UIMessageManager::Execute_GO_BILING_PAGE(int left, int right, void* void_ptr)
{
	if(left == TRUE)
	{
		// 종료..
		SetMode( MODE_QUIT );
		
		char str[256];
		
		GetWindowsDirectory(
			str,  // address of buffer for Windows directory
			255        // size of directory buffer
			);
		
		sprintf(str, "%s\\Explorer.exe", str);
		
		CDirectDraw::GetDD()->RestoreDisplayMode();
		
		_spawnl(_P_NOWAIT, str, "Explorer.exe", g_pClientConfig->URL_HOMEPAGE_BILING.GetString(), NULL);
	}	
}

void 
UIMessageManager::Execute_UI_CLOSE_POPUP_MESSAGE(int left, int right, void *void_ptr)
{
	gC_vs_ui.ClosePopupMessage();
}


void 
UIMessageManager::Execute_UI_CLOSE_QUEST_STATUS(int left, int right, void *void_ptr)
{
	gC_vs_ui.CloseQuestStatusWindow();
}

void 
UIMessageManager::Execute_UI_CLOSE_LOTTERY_CARD(int left, int right, void *void_ptr)
{
	gC_vs_ui.CloseLotteryCard();
	gC_vs_ui.SetQuestStatusInit();

	if(g_bEventQuestEnding && right == 5)
	{
		UI_CloseAllDialog();
		MEvent event;
		event.eventID = EVENTID_OUSTERS_FIN;
		event.eventType = EVENTTYPE_ZONE;
		event.eventFlag = EVENTFLAG_ONLY_EVENT_BACKGROUND | EVENTFLAG_NOT_DRAW_UI | EVENTFLAG_NOT_DRAW_CREATURE | 
			EVENTFLAG_NOT_DRAW_INFORMATION | EVENTFLAG_NOT_DRAW_CREATURE_SHADOW | EVENTFLAG_NOT_DRAW_ITEM | 
			EVENTFLAG_NOT_DRAW_MOUSE_POINTER | EVENTFLAG_NOT_DRAW_EFFECT | EVENTFLAG_DENY_INPUT | 
			EVENTFLAG_NOT_FADE_SCREEN | EVENTFLAG_NOT_PLAY_SOUND;
		event.parameter1 = 0;
		event.parameter4 = EVENTBACKGROUNDID_QUEST_2;
		g_pEventManager->AddEvent(event);			
	}
	g_bEventQuestEnding = false;	
}

void 
UIMessageManager::Execute_UI_LOTTERY_CARD_STATUS(int left, int right, void *void_ptr)
{
	// left = step, right = 상품 ID
	
	CGLotterySelect _CGLotterySelect;
	
	_CGLotterySelect.setType( TYPE_SELECT_LOTTERY );
	_CGLotterySelect.setQuestLevel ( (DWORD) left-1 );
	_CGLotterySelect.setGiftID ( (DWORD) right );

	g_pSocket->sendPacket( &_CGLotterySelect );
}

void 
UIMessageManager::Execute_UI_FINISH_SCRATCH_LOTTERY(int left, int right, void *void_ptr)
{
	CGLotterySelect _CGLotterySelect;
	
	_CGLotterySelect.setType( TYPE_FINISH_SCRATCH );
	_CGLotterySelect.setQuestLevel ( 0 );
	_CGLotterySelect.setGiftID ( 0 );

	g_pSocket->sendPacket( &_CGLotterySelect );
	
}

void 
UIMessageManager::Execute_UI_CLOSE_IMAGE_NOTICE(int left,int right, void *void_ptr)
{
	gC_vs_ui.CloseImageNotice();
}

void 
UIMessageManager::Execute_UI_SELECT_ITEM_FROM_SHOP(int left, int right, void *void_ptr)
{
	CGTakeOutGood _CGTakeOutGood;

	_CGTakeOutGood.setObjectID( (DWORD) left );

	g_pSocket->sendPacket( &_CGTakeOutGood );
}

void 
UIMessageManager::Execute_UI_CLOSE_ITEM_LIST( int left, int right, void *void_ptr)
{
	g_pUIDialog->ClosePCTalkDlg();	
	gC_vs_ui.CloseItemListWindow();
}

void 
UIMessageManager::Execute_UI_CLOSE_BULLETIN_BOARD(int left, int right, void *void_ptr)
{
	gC_vs_ui.CloseBulletinBoardWindow();
}

void 
UIMessageManager::Execute_UI_TRANS_ITEM_CANCEL(int left, int right, void *void_ptr)
{
	UI_CloseTransItem();
}

void 
UIMessageManager::Execute_UI_TRANS_ITEM_ACCEPT(int left, int right, void *void_ptr)
{	
	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}
	// gpC_mouse_pointer->GetPickUpItem()		// 들고 있는 것
	//void_ptr = MItem *		// 들어갈 곳
	// Inventory좌표(left,right)
	MItem* pItem = (MItem*)void_ptr;
	MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();

	if (pMouseItem==NULL)
	{
		DEBUG_ADD("[Error] Mouse Item is NULL");
		return;
	}
	if (pItem==NULL)
	{
		DEBUG_ADD("[Error] InvenItem is NULL");
		return;
	}
	TYPE_OBJECTID mouseItemID = ((pMouseItem==NULL)?OBJECTID_NULL : pMouseItem->GetID());

	// 교환중일때는 Enchant할 수 없다!
	if (g_pTradeManager!=NULL)
		return;
	
	//g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_INSERT_FROM_INVENTORY );
	
	//----------------------------------------------------
	// Server에 접속한 경우
	//----------------------------------------------------
	if(g_pTempInformation->GetMode() == TempInformation::MODE_NULL && g_pPlayer->IsItemCheckBufferNULL())
	{
		CGAddItemToItem _CGAddItemToItem;
		_CGAddItemToItem.setObjectID( mouseItemID );
		_CGAddItemToItem.setX( pItem->GetGridX() );
		_CGAddItemToItem.setY( pItem->GetGridY() );								
		
		g_pSocket->sendPacket( &_CGAddItemToItem );
		
		g_pPlayer->SetItemCheckBuffer( pMouseItem, MPlayer::ITEM_CHECK_BUFFER_ITEM_TO_ITEM );
		g_pTempInformation->SetMode(TempInformation::MODE_ITEM_TO_ITEM);
		g_pTempInformation->Value1 = pItem->GetGridX();
		g_pTempInformation->Value2 = pItem->GetGridY();

//		UI_LockItem();
	}
}

void 
UIMessageManager::Execute_UI_REQUEST_RESURRECT(int left, int right, void *void_ptr)
{
	if( left == 0 || left == 1)
	{
		// Resurrect Scroll
		MItem* pItem = g_pInventory->FindItem( ITEM_CLASS_RESURRECT_ITEM, left );
		if( pItem == NULL )
		{
			return;
		}

		CGUseItemFromInventory _CGUseItemFromInventory;
		_CGUseItemFromInventory.setObjectID( pItem->GetID() );
		_CGUseItemFromInventory.setX( pItem->GetGridX() );
		_CGUseItemFromInventory.setY( pItem->GetGridY() );
		
		g_pSocket->sendPacket( &_CGUseItemFromInventory );				
		g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);
	}
	else
	if( left == 2 )
	{
		// Eternity
		CGSkillToSelf			_CGSkillToSelf;

		_CGSkillToSelf.setSkillType( SKILL_ETERNITY );
		g_pSocket->sendPacket( &_CGSkillToSelf );
	}
	else if(left == 3)
	{
		CGSelectRegenZone _CGSelectRegenZone;
		_CGSelectRegenZone.setRegenZoneID(14);
		g_pSocket->sendPacket( &_CGSelectRegenZone );
	}
}

void 
UIMessageManager::Execute_UI_CLOSE_MIXING_FORGE(int left, int right, void *void_ptr)
{
	if(left == 0 )
	{
		gC_vs_ui.CloseMixingForge();
		g_pPlayer->ClearItemCheckBuffer();
		g_pTempInformation->SetMode( TempInformation::MODE_NULL );
	} else
	{
		g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[left].GetString() );		
	}
}

void 
UIMessageManager::Execute_UI_MIXING_FORGE(int left, int right, void* void_ptr)
{
	if( g_pPlayer->GetItemCheckBufferStatus() != MPlayer::ITEM_CHECK_BUFFER_USE_MIXING_ITEM ||
		g_pTempInformation->GetMode() != TempInformation::MODE_NULL )
		return;

	MItem *pFirstItem = g_pInventory->GetItemToModify( left );
	MItem *pSecondItem = g_pInventory->GetItemToModify( right );

	if( pFirstItem == NULL || pSecondItem == NULL )
	{
		return;
	}

	if( pFirstItem->GetItemClass() == pSecondItem->GetItemClass() &&
		pFirstItem->GetItemType() == pSecondItem->GetItemType() &&
		pFirstItem->GetItemOptionListCount() == 1 &&
		pSecondItem->GetItemOptionListCount() == 1 &&
		!pFirstItem->IsUniqueItem() && ! pSecondItem->IsUniqueItem() &&
		!pFirstItem->IsQuestItem() && ! pSecondItem->IsQuestItem() )
	{		
		MItem* pMixingItem = g_pPlayer->GetItemCheckBuffer();

		CGMixItem _CGMixItem;

		_CGMixItem.setObjectID( pMixingItem->GetID() );
		_CGMixItem.setX( pMixingItem->GetGridX() );
		_CGMixItem.setY( pMixingItem->GetGridY() );
		_CGMixItem.setTargetObjectID( 0, pFirstItem->GetID() );
		_CGMixItem.setTargetObjectID( 1, pSecondItem->GetID() );
		
		g_pSocket->sendPacket( &_CGMixItem );
		g_pPlayer->SetItemCheckBuffer( pMixingItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY );
		
		g_pTempInformation->SetMode( TempInformation::MODE_ITEM_MIXING );
		g_pTempInformation->Value1 = pFirstItem->GetGridX();
		g_pTempInformation->Value2 = pFirstItem->GetGridY();

		g_pTempInformation->Value3 = pSecondItem->GetGridX();
		g_pTempInformation->Value4 = pSecondItem->GetGridY();
		
		g_pTempInformation->pValue = (void*) pMixingItem;
	}		
}

void 
UIMessageManager::Execute_UI_CLOSE_REMOVE_OPTION(int left, int right, void *void_ptr)
{
	gC_vs_ui.CloseRemoveOptionFromRareItem();
}

void 
UIMessageManager::Execute_UI_SEND_REMOVE_OPTION(int left, int right, void *void_ptr)
{
	MItem *pMouseItem = (MItem*) void_ptr;
	MItem *pCurItem = g_pInventory->GetItemToModify( right );


	if( left < 0 || left > 1 || pMouseItem == NULL || pCurItem == NULL)
	{
		g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_ERROR_ETC_ERROR].GetString());
		return;
	}

	CGMixItem _CGMixItem;
	_CGMixItem.setObjectID( pMouseItem->GetID() );
	_CGMixItem.setX( pCurItem->GetGridX() );
	_CGMixItem.setY( pCurItem->GetGridY() );
	_CGMixItem.setTargetObjectID( 0, pCurItem->GetID() );
	_CGMixItem.setTargetObjectID( 1, left );
	g_pSocket->sendPacket( &_CGMixItem );
	gC_vs_ui.CloseRemoveOptionFromRareItem();

	g_pPlayer->SetItemCheckBuffer( pMouseItem, MPlayer::ITEM_CHECK_BUFFER_ITEM_TO_ITEM );
	
	g_pTempInformation->SetMode(TempInformation::MODE_ITEM_TO_ITEM);
	g_pTempInformation->Value1 = pCurItem->GetGridX();
	g_pTempInformation->Value2 = pCurItem->GetGridY();
	g_pTempInformation->Value3 = left;
}

void 
UIMessageManager::Execute_UI_CLOSE_OUSTERS_SKILL_INFO(int left, int right, void *void_ptr)
{
	gC_vs_ui.CloseOustersSkillInfo();
}

void 
UIMessageManager::Execute_UI_LEARN_OUSTERS_SKILL(int left, int right, void *void_ptr)
{
	if( g_pTempInformation->GetMode() != TempInformation::MODE_NULL )
		return;		
		
	DEBUG_ADD("[UI] UI_LEARN_OUSTERS_SKILL");
	
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}
	
	
	g_pTempInformation->SetMode(TempInformation::MODE_SKILL_LEARN);
	{
		(*g_pTempInformation).Value3 = left;
		
		CGLearnSkill _CGLearnSkill;
		
		//_CGLearnSkill.setObjectID( (*g_pPCTalkBox).GetNPCID() );
		_CGLearnSkill.setSkillType( left );
		_CGLearnSkill.setSkillDomainType( SKILLDOMAIN_OUSTERS );
		
		g_pSocket->sendPacket( &_CGLearnSkill );
		
	}
}

void 
UIMessageManager::Execute_UI_RUN_LEVELUP(int left, int right, void *void_ptr)
{
	UI_LevelUp();
}

void 
UIMessageManager::Execute_UI_CLOSE_HORN(int left, int right, void *void_ptr)
{
	gC_vs_ui.CloseHorn();
}


void 
UIMessageManager::Execute_UI_OUSTERS_DOWN_SKILL(int left, int right, void *void_ptr)
{
	if( left < 0 || left >= MAX_ACTIONINFO)
		return;

	int curLevel = (*g_pSkillInfoTable)[left].GetExpLevel();

	// 2004, 11, 8, sobeit modify start - 아우스터즈 스킬에 따라 0레벨로도 만들수 있다.
	if( curLevel > 30 || (curLevel == 1 && (*g_pSkillInfoTable)[left].CanDelete == 0) )
		return;	
	// 2004, 11, 8, sobeit modify end
	
	CGDownSkill	_CGDownSkill;

	_CGDownSkill.setSkillType( (SkillType_t) left );

	g_pSocket->sendPacket( &_CGDownSkill );

	DEBUG_ADD("[UI] Execute_UI_OUSTERS_DOWN_SKILL"); 
}

void 
UIMessageManager::Execute_UI_CLEAR_ALL_STAGE(int left, int right, void *void_ptr)
{	
	// LEFT = GAMETYPE
	switch( left )
	{
	case 0 :				// 다꺴다!!!!
		break;
	case 1 :
		break;
	}
	gC_vs_ui.CloseArrowTile();
	gC_vs_ui.CloseCrazyMine();
}

void 
UIMessageManager::Execute_UI_CLOSE_FINDING_MINE(int left, int right, void *void_ptr)
{
	gC_vs_ui.CloseFindingMine();
}

void 
UIMessageManager::Execute_UI_CLOSE_NEMONEMO(int left, int right, void *void_ptr)
{
	//gC_vs_ui.CloseNemo
}

void 
UIMessageManager::Execute_UI_CLOSE_PUSHPUSH(int left, int right, void *void_ptr)
{
}

void 
UIMessageManager::Execute_UI_CLOSE_CRAZY_MINE(int left, int right, void *void_ptr)
{
	gC_vs_ui.CloseCrazyMine();

	CGFailQuest _CGFailQuest;

	_CGFailQuest.setFail( false );

	g_pSocket->sendPacket( &_CGFailQuest );
	gC_vs_ui.SetQuestStatusInit();
}

void 
UIMessageManager::Execute_UI_CLOSE_ARROW_TILE(int left, int right, void *void_ptr)
{
	gC_vs_ui.CloseArrowTile();
	CGFailQuest _CGFailQuest;

	_CGFailQuest.setFail( false );

	g_pSocket->sendPacket( &_CGFailQuest );
	gC_vs_ui.SetQuestStatusInit();
}

void 
UIMessageManager::Execute_UI_CLEAR_STAGE(int left, int right, void *void_ptr)
{
	WORD	GameType = HIWORD(left);
	WORD	Stage = LOWORD(left);
	DWORD	Time = (DWORD)right;

	Time/=10;

	Time = min(65535,Time);
	DEBUG_ADD_FORMAT("[Execute_UI_CLEAR_STAGE] GameType:%d Stage:%d Time:%d", GameType, Stage,Time);	
	
	CGSubmitScore _CGSubmitScore;

	_CGSubmitScore.setGameType( GameType == 0 ? GAME_ARROW : GAME_MINE );
	_CGSubmitScore.setLevel( Stage );
	_CGSubmitScore.setScore( WORD(Time) );
	g_pSocket->sendPacket( &_CGSubmitScore );
}

void
UIMessageManager::Execute_UI_FORCE_DIE(int left, int right, void *void_ptr)
{
	// 나를 죽여죠
	gC_vs_ui.CloseAllDialog();
	
	switch(left)
	{
	case 10 :						// ArrowTile 하다가 죽었을때
		break;
	case 11 :						// CrazyMine 하다가 죽었을때			
		break;
	}
	
	// 서버에 나 죽여죠 패킷을 보낸다.
	CGFailQuest		_CGFailQuest;
	
	_CGFailQuest.setFail( true );
	g_pSocket->sendPacket( &_CGFailQuest );
	gC_vs_ui.SetQuestStatusInit();
}

void 
UIMessageManager::Execute_UI_ADD_ITEM_TO_CODE_SHEET(int left, int right, void *void_ptr)
{
	MItem* pItem = (MItem*)void_ptr;

	if( pItem == NULL || right < 0 || right >= 5)
		return;

	CGAddItemToCodeSheet _CGAddItemToCodeSheet;

	_CGAddItemToCodeSheet.setX( HIWORD( left ) );
	_CGAddItemToCodeSheet.setY( LOWORD( left ) );
	_CGAddItemToCodeSheet.setObjectID( pItem->GetID() );
	
	g_pSocket->sendPacket( &_CGAddItemToCodeSheet );
	g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_ITEM_TO_ITEM );	

	g_pTempInformation->SetMode(TempInformation::MODE_ITEM_CODE_SHEET);
	g_pTempInformation->Value1 = right;	
	g_pTempInformation->Value2 = left;
	g_pTempInformation->pValue = (void*)gpC_mouse_pointer->GetPickUpItem();
}


void 
UIMessageManager::Execute_UI_SEND_BUG_REPORT(int left,int right, void *void_ptr)
{
	SendBugReport("[UI_BUG] %d, %s", left, reinterpret_cast<char*>( void_ptr ) );
}

void 
UIMessageManager::Execute_UI_GO_BEGINNER_ZONE(int left, int right, void* void_ptr)
{
//	Execute_UI_CLOSE_POPUP_MESSAGE(left,right,void_ptr);
//
//	CGSelectWayPoint _CGSelectWayPoint;
//
//	_CGSelectWayPoint.setZoneID( 1122 );
//	_CGSelectWayPoint.setX( 0 );
//	_CGSelectWayPoint.setY( 0 );
//
//	g_pSocket->sendPacket( &_CGSelectWayPoint );
}

void
UIMessageManager::Execute_UI_POPUP_MESSAGE_OK(int left, int right, void* void_ptr)
{
	gC_vs_ui.CloseMailBox();
	
	Execute_UI_CLOSE_POPUP_MESSAGE(left,right,void_ptr);

	DWORD recvID = (DWORD) left;
	
	int zoneID = 0;

	switch(recvID)
	{
	// 초보존
	case 1:
		zoneID = 1122;
		break;

	case 2:
		zoneID = 1122;
		break;
		
	// 레벨 전쟁 시작
	case 4:
		zoneID = 1131;
		break;

	// 종족 전쟁 시작
	case 6:
		zoneID = 72;
		break;

	}

	CGSelectWayPoint _CGSelectWayPoint;
	
	_CGSelectWayPoint.setZoneID( zoneID );
	_CGSelectWayPoint.setX( 0 );
	_CGSelectWayPoint.setY( 0 );
	
	g_pSocket->sendPacket( &_CGSelectWayPoint );
}

void 
UIMessageManager::Execute_UI_CLOSE_SHRINE_MINIMAP(int left, int right, void* void_ptr)
{
	// Random-_- 으로 보내자. 강제로 닫은 경우이다. 

	bool bSended = false;

	for(int i = 0; i < g_pRegenTowerInfoManager->GetSize(); i++ )
	{
		const RegenTowerInfo* pInfo = &g_pRegenTowerInfoManager->Get(i);
		
		if( pInfo->owner == (int)g_pPlayer->GetRace() )
		{
			CGSelectRegenZone _CGSelectRegenZone;
			_CGSelectRegenZone.setRegenZoneID( i );
			g_pSocket->sendPacket( &_CGSelectRegenZone );
			bSended = true;
			break;
		}
	}
	gC_vs_ui.CloseRequestShrineMinimap();

	if( bSended == false )
	{
		Execute_UI_FINISH_REQUEST_DIE_BUTTON(left, right, void_ptr);
	}
	gC_vs_ui.FinishRequestDie();
	gC_vs_ui.FinishRequestResurrect();	
}

void 
UIMessageManager::Execute_UI_WARP_TO_REGEN_TOWER(int left, int right, void* void_ptr)
{
	if( g_pRegenTowerInfoManager == NULL || left < 0 || 
		left >= g_pRegenTowerInfoManager->GetSize() || 
		g_pRegenTowerInfoManager->Get(left).owner != g_pPlayer->GetRace() )
		return;

	CGSelectRegenZone _CGSelectRegenZone;
	
	_CGSelectRegenZone.setRegenZoneID( left );

	g_pSocket->sendPacket( &_CGSelectRegenZone );
	gC_vs_ui.CloseRequestShrineMinimap();
	gC_vs_ui.FinishRequestDie();
	gC_vs_ui.FinishRequestResurrect();	
}

void 
UIMessageManager::Execute_UI_CLOSE_MAILBOX(int left, int right, void *void_ptr)
{
	gC_vs_ui.CloseMailBox();
}

void
UIMessageManager::Execute_UI_CLOSE_PET_INFO(int left, int right, void* void_ptr)
{
	gC_vs_ui.ClosePetInfo();
}

void	
UIMessageManager::Execute_UI_PET_GAMBLE(int left, int right, void* void_ptr)
{
	CGPetGamble _CGPetGamble;
	g_pSocket->sendPacket( &_CGPetGamble );
	// 겜블하는동안 아이템 못빼게
	UI_LockItem();
}

void
UIMessageManager::Execute_UI_CLOSE_USE_PET_FOOD(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}
	gC_vs_ui.CloseUsePetFood();
}

void	 
UIMessageManager::Execute_UI_CLOSE_PETSTORAGE(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	gC_vs_ui.ClosePetStorage();
	
	// 보관함 중지
	if (g_pStorage!=NULL)
	{
		delete g_pStorage;
		g_pStorage = NULL;
	}

	// storage를 없앤다.
	gC_vs_ui.SetPetStorage( NULL );

//	// 다시 뭔가를?선택할 수 있게 한다.
//	g_pUIDialog->ShowPCTalkDlg();

	g_pTempInformation->SetMode(TempInformation::MODE_NULL);

}
void	 
UIMessageManager::Execute_UI_CLOSE_KEEP_PETITEM(int left, int right, void* void_ptr)
{
	gC_vs_ui.CloseKeepPetItemDialog();
}
void	 
UIMessageManager::Execute_UI_CLOSE_GET_KEEP_PETITEM(int left, int right, void* void_ptr)
{
	gC_vs_ui.CloseGetKeepPetItemDialog();
}

void	 
UIMessageManager::Execute_UI_KEEP_PETITEM(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_KEEP_PETITEM");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}
	if (g_pTempInformation->GetMode() != TempInformation::MODE_NULL)
		return;
	MItem* pItem = (MItem*)void_ptr;

	if(NULL == pItem)
		return;

	CGDepositPet _CGDepositPet;
	_CGDepositPet.setObjectID(pItem->GetID());
	if(-1 != right)
		_CGDepositPet.setIndex(right);
	else
		return;
	g_pSocket->sendPacket( &_CGDepositPet );	

	g_pTempInformation->SetMode(TempInformation::MODE_PETITEM_MOVETO_PETSTORAGE);
	g_pTempInformation->Value1 = right;
	g_pTempInformation->Value2 = pItem->GetGridX();
	g_pTempInformation->Value3 = pItem->GetGridY();
	g_pTempInformation->pValue = pItem;
	UI_LockItem(); 
}
void	 
UIMessageManager::Execute_UI_GET_KEEP_PETITEM(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_GET_KEEP_PETITEM");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}
	if (g_pTempInformation->GetMode() != TempInformation::MODE_NULL)
		return;
	MItem* pItem = (MItem*)void_ptr;

	if(NULL == pItem)
		return;

	CGWithdrawPet _CGWithdrawPet;
	_CGWithdrawPet.setObjectID(pItem->GetID());
	if(right<20 &&right>-1)
		_CGWithdrawPet.setIndex(right);
	else
		return;
	g_pSocket->sendPacket( &_CGWithdrawPet );	
	g_pTempInformation->SetMode(TempInformation::MODE_PETITEM_MOVETO_INVENTORY);
	g_pTempInformation->Value1 = right;
	g_pTempInformation->Value2 = pItem->GetGridX();
	g_pTempInformation->Value3 = pItem->GetGridY();
	g_pTempInformation->pValue = pItem;
	UI_LockItem();
}


void	 
UIMessageManager::Execute_UI_CLOSE_SMS_MESSAGE(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		if(gC_vs_ui.IsRunningSMSRecord())
			gC_vs_ui.CloseSMSRecord();
		if(gC_vs_ui.IsRunningSMSList())
			gC_vs_ui.CloseSMSList();
		gC_vs_ui.CloseSMSMessage(); 
		
	}

}
void
UIMessageManager::Execute_UI_SEND_SMS_MESSAGE(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}
	if (g_pTempInformation->GetMode() != TempInformation::MODE_NULL)
		return;

	std::list<std::string> *TempStr = (std::list<std::string>*)(void_ptr);

	if(NULL == TempStr || TempStr->size()>5)
		return ;
	
	CGSMSSend _CGSMSSend;
	_CGSMSSend.setCallerNumber(std::string((char *)left));
	_CGSMSSend.setMessage(std::string((char *)right));
	_CGSMSSend.clearString(); 
	std::list<std::string>::iterator itr = TempStr->begin();
	while(itr != TempStr->end())
	{
		string str = (*itr);
		_CGSMSSend.addString(str);
		itr++;
	}
	g_pSocket->sendPacket( &_CGSMSSend );
	
	gC_vs_ui.CloseSMSMessage();
	if(gC_vs_ui.IsRunningSMSList())
		gC_vs_ui.CloseSMSList();
	if(gC_vs_ui.IsRunningSMSRecord())
		gC_vs_ui.CloseSMSRecord();
}


void	 
UIMessageManager::Execute_UI_SMS_OPEN_LIST(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.RunSMSList();
		// sms 주소 요청 패킷 보냄
		CGSMSAddressList _CGSMSAddressList;
		g_pSocket->sendPacket( &_CGSMSAddressList );
	}
}

void	 
UIMessageManager::Execute_UI_SMS_RECORD(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		CGAddSMSAddress	_CGAddSMSAddress;
		_CGAddSMSAddress.setCharacterName(std::string((char *)left));
		_CGAddSMSAddress.setCustomName((std::string((char *)right)));
		_CGAddSMSAddress.setNumber((std::string((char *)void_ptr)));
		g_pSocket->sendPacket( &_CGAddSMSAddress );

		g_pTempInformation->SetMode(TempInformation::MODE_SMS_ADD_ADDRESS);
		g_pTempInformation->Value1 = (int)left;
		g_pTempInformation->Value2 = (int)right;
		g_pTempInformation->Value3 = (int)void_ptr;

	}

}


void	 
UIMessageManager::Execute_UI_SMS_DELETE(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	// 등록 요청 패킷 보냄
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		CGDeleteSMSAddress	_CGDeleteSMSAddress;
		_CGDeleteSMSAddress.setElementID((DWORD)left);
		g_pSocket->sendPacket( &_CGDeleteSMSAddress );

		g_pTempInformation->SetMode(TempInformation::MODE_SMS_DELETE_ADDRESS);
		g_pTempInformation->Value1 = left;
	}
}

void	 
UIMessageManager::Execute_UI_CLOSE_SMS_LIST(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.CloseSMSList();
	}
}

void	 
UIMessageManager::Execute_UI_CLOSE_SMS_RECORD(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.CloseSMSRecord();
	}
}
void	 
UIMessageManager::Execute_UI_SMS_ADD_SEND_LIST(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.AddSMSSendList((char*)left);
	}
}


void	 
UIMessageManager::Execute_UI_CLOSE_NAMING(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.CloseNaming();
	}
}

void	 
UIMessageManager::Execute_UI_SELECT_NAMING(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		CGSelectNickname	_CGSelectNickName;
		_CGSelectNickName.setNicknameID((WORD)left);
		g_pSocket->sendPacket( &_CGSelectNickName );

//		g_pTempInformation->SetMode(TempInformation::MODE_NICKNAME_CHANGE_CUSTOM);
//		g_pTempInformation->Value1 = left;
	}
}


void	 
UIMessageManager::Execute_UI_CLOSE_NAMING_CHANGE(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.CloseNamingChange();
	}
}


void	 
UIMessageManager::Execute_UI_CHANGE_CUSTOM_NAMING(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}
	// 서버 세팅 끝나고 테스트 후 return 풀자..
//	return;
	// left : name, right : item ID(0=울버린) void_ptr:penitem pointer
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		char szTemp[22];
		strcpy(szTemp, (char*) left);
		if(strlen(szTemp) == 0)
			return;
//		if (!IsValidID(szTemp, "-_"))
//		{
//			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
//			bAllOK = FALSE;
//		}
//		else
//		{
//			char strName[80];
//			strcpy(strName, pChar->sz_name);
//
//			// 안 좋은 말이 들어있는 경우는 허용이 안된다
//			if (g_pChatManager->RemoveCurse(strName))
//			{
//				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
//				bAllOK = FALSE;
//			}
//		}
		g_pChatManager->RemoveCurse(szTemp );

		// 이쯤에서 effect status를 검색 해서 아이템 사용 패킷을 보내는게 좋을듯..
		MItem* pItem = NULL;	
		
		if(right != 0)
			pItem = g_pInventory->GetItemToModify( right );//(MItem*)void_ptr;
		
		if(pItem!= NULL || right == 0)
		{
			CGModifyNickname	_CGModifyNickname; 
			_CGModifyNickname.setNickname(std::string((char *)szTemp));

			if(right != 0)
			{ 
				// 먼저 아이템 썼다고 패킷 보내고 - 안보내기루 설정
			//	Execute_UI_ITEM_USE(pItem->GetID(), 0, (void*)pItem);
				// 닉네임 체인지 패킷을 보내장..
				_CGModifyNickname.setNicknameID(pItem->GetID());
				g_pSocket->sendPacket( &_CGModifyNickname );

				g_pTempInformation->SetMode(TempInformation::MODE_NICKNAME_CHANGE_CUSTOM);
				g_pTempInformation->Value1 = left;					// 바뀐 닉네임 아이디
				g_pTempInformation->Value2 = pItem->GetItemType();  // 아이템 타입

				if(25 != g_pTempInformation->Value2)  // 시간제 ㅏㅇ이템
					g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);
			}
			else
			{
				// 울버린일땐 아이템 없이 바꾼당..
				_CGModifyNickname.setNicknameID(0);
				g_pSocket->sendPacket( &_CGModifyNickname );
				g_pTempInformation->SetMode(TempInformation::MODE_NICKNAME_CHANGE_CUSTOM);
				g_pTempInformation->Value1 = left;					// 바뀐 닉네임 아이디
				g_pTempInformation->Value2 = 0;  // 
			}
		}
		else
			UI_PopupMessage (UI_STRING_MESSAGE_FAIL_SEARCH_ITEM);
	}
}

void	 
UIMessageManager::Execute_UI_RUN_NAMING_CHANGE(int left, int right, void* void_ptr)
{ 
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		MItem* pItem = (MItem*)left;
		if(pItem->GetItemClass() != ITEM_CLASS_EVENT_GIFT_BOX)
			return;
		// pItem 이 캐릭터 닉네임 바꾸는 아이템 이면
			// 1회용 (2가지) - 기존닉네임 수정, 새로운 닉네임 추가
			// 시간제 (1가지) - 특정 시간동안 기존 닉네임 무제한 수정가능
		// pItem이 펫 닉네임 바꾸는 아이템이면 펫이 소환 상태여야만 사용 가능
			// 울버린의 경우 아이템이 필요 없음
		
		if(pItem->GetItemType() == 22 || pItem->GetItemType() == 25) // 유저 닉네임 수정
			gC_vs_ui.RunNamingChange(pItem, (char*)g_pPlayer->GetNickName().c_str());
//		// 닉네임 추가
		else if(pItem->GetItemType() == 24) // extra naming pen // 네이밍 추가 아이템
			gC_vs_ui.RunNamingChange(pItem, (char*)(*g_pGameStringTable)[UI_STRING_MESSAGE_ADD_PLAYER_NICKNAME].GetString());
//		// 시간제 아이템
//		else if(pItem->GetItemType() == 25) // limited naming pen // 네이밍 변경 시간 아이템
//			gC_vs_ui.RunNamingChange(pItem, (char*)g_pPlayer->GetNickName().c_str());
		else if(pItem->GetItemType() == 23) // 펫 닉네임 수정
		{
			MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(g_pPlayer->GetPetID());
			if(NULL != pFakeCreature)
			{
				MPetItem *pPetItem = pFakeCreature->GetPetItem();
				if(pPetItem != NULL)
				{
					if(2 == pPetItem->GetItemType()) // 울버린 일땐 펜 아이템 필요 없음
						UI_PopupMessage( UI_STRING_MESSAGE_PET_NAMING_WOLVERINE );
					else
						gC_vs_ui.RunNamingChange(pItem, (char*)pFakeCreature->GetNickName().c_str());
				}
			}
			else
				UI_PopupMessage( UI_STRING_MESSAGE_PET_NAMING_SUMMON );
		}
		//gC_vs_ui.RunNamingChange(pItem, char* szCustom);

	}
}

void	 
UIMessageManager::Execute_UI_CLOSE_QUEST_MANAGER(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.CloseQuestManager();
	}
}
void	 
UIMessageManager::Execute_UI_CLOSE_QUEST_LIST(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.CloseQuestList();
	}
}
void	 
UIMessageManager::Execute_UI_CLOSE_QUEST_DETAIL(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.CloseQuestDetail();
	}
}
void	 
UIMessageManager::Execute_UI_CLOSE_QUEST_MISSION(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.CloseQuestMission();
	}
}
void	 
UIMessageManager::Execute_UI_CLOSE_QUEST_ITEM(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.CloseQuestItem();
	}
}
void	 
UIMessageManager::Execute_UI_CLOSE_QUEST_ICON(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		gC_vs_ui.CloseQuestIcon(left);
	}
}
void	 
UIMessageManager::Execute_UI_GQUEST_ACCEPT(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		CGGQuestAccept	_CGGQuestAccept;
		_CGGQuestAccept.setQuestID((DWORD)left);
		g_pSocket->sendPacket( &_CGGQuestAccept );
	}
}
void	 
UIMessageManager::Execute_UI_GQUEST_GIVEUP(int left, int right, void* void_ptr)
{
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}

	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		CGGQuestCancel	_CGGQuestCancel;
		_CGGQuestCancel.setQuestID((DWORD)left);
		g_pSocket->sendPacket( &_CGGQuestCancel );
	}
}
//-----------------------------------------------------------------------------
//
// GQuest Inventory에서 Item을 사용한 경우
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_ITEM_USE_GQUEST_ITEM(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_ITEM_USE_GQUEST_ITEM");
	
	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}
	MItem* pItem =  (MItem*)void_ptr ;//(MItem*)void_ptr; 

	if(NULL != pItem)
	{
		CGUseItemFromGQuestInventory _CGUseItemFromGQuestInventory;
		_CGUseItemFromGQuestInventory.setIndex( left );
		
		g_pSocket->sendPacket( &_CGUseItemFromGQuestInventory );
		
		g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_GQUEST_INVENTORY);
	}
}


void
UIMessageManager::Execute_UI_ITEM_USE_REQUEST_GUILD_LIST(int left, int right, void* void_ptr)
{

	DEBUG_ADD("[UI] Execute_UI_ITEM_USE_REQUEST_GUILD_LIST");
	CGRequestGuildList _CGRequestGuildlist;	
	_CGRequestGuildlist.setGuildType(left);
	g_pSocket->sendPacket(&_CGRequestGuildlist); 

}


void
UIMessageManager::Execute_UI_ITEM_USE_UNION_INFO(int left, int right, void* void_ptr)
{

	DEBUG_ADD("[UI] Execute_UI_ITEM_USE_UNION_INFO");
	CGRequestUnionInfo _CGRequestUnioninfo;	
	g_pSocket->sendPacket(&_CGRequestUnioninfo); 

}


void
UIMessageManager::Execute_UI_ITEM_USE_REQUEST_UNION(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_ITEM_USE_REQUEST_UNION");
	
	if(g_pUserInformation->GuildGrade != 1) // 길드마스터가 아니라면
	{
		UI_PopupMessage(UI_STRING_MESSAGE_REQUEST_UNION_ERROR_1);
		return;
	}
	else if(g_pUserInformation->dwUnionID != 0)
	{
		UI_PopupMessage(UI_STRING_MESSAGE_REQUEST_UNION_ERROR_2);
		return;
	}
	CGRequestUnion _CGRequestUnion;	
	_CGRequestUnion.setGuildID(left);
	g_pSocket->sendPacket(&_CGRequestUnion); 

	gC_vs_ui.CloseTeamInfoDialogBox();
}
void
UIMessageManager::Execute_UI_ITEM_USE_QUIT(int left, int right, void* void_ptr)
{	
	DEBUG_ADD("[UI] Execute_UI_ITEM_USE_REQUEST_DENY");

	if(g_pUserInformation->GuildGrade != 1) // 길드마스터가 아니라면
	{
		UI_PopupMessage(UI_STRING_MESSAGE_REQUEST_UNION_ERROR_1);
		return;
	}

	CGQuitUnion   _CGQuitUnion;
	_CGQuitUnion.setGuildID(left);
	_CGQuitUnion.setQuitMethod(right);
	g_pSocket->sendPacket(&_CGQuitUnion); 

	gC_vs_ui.ClosePopupMessage();
	gC_vs_ui.CloseTeamInfoDialogBox();
}
void
UIMessageManager::Execute_UI_ITEM_USE_EXPER(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_ITEM_USE_EXPER"); 
	if(g_pUserInformation->GuildGrade != 1) // 길드마스터가 아니라면
	{
		UI_PopupMessage(UI_STRING_MESSAGE_REQUEST_UNION_ERROR_1);
		return;
	}
	else if(g_pUserInformation->bUnionGrade != 0)
	{
		UI_PopupMessage(UI_STRING_MESSAGE_REQUEST_UNION_ERROR_3);
		return;
	}
	CGExpelGuild  _CGExpelGuild;
	_CGExpelGuild.setGuildID(left);
	g_pSocket->sendPacket(&_CGExpelGuild); 

	gC_vs_ui.CloseTeamInfoDialogBox();
	gC_vs_ui.CloseTeamInfo();
	gC_vs_ui.CloseTeamList();
}

void
UIMessageManager::Execute_UI_ITEM_USE_UNION_ACCEPT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_ITEM_USE_UNION_ACCEPT");
	CGAcceptUnion  _CGAcceptUnion;
	_CGAcceptUnion.setGuildID(left);
	g_pSocket->sendPacket(&_CGAcceptUnion); 

	gC_vs_ui.CloseMailBoxDialogBox();
}

void
UIMessageManager::Execute_UI_ITEM_USE_UNION_DENY(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_ITEM_USE_UNION_DENY");
	CGDenyUnion  _CGDenyUnion;
	_CGDenyUnion.setGuildID(left);
	g_pSocket->sendPacket(&_CGDenyUnion); 

	gC_vs_ui.CloseMailBoxDialogBox();

}


void
UIMessageManager::Execute_UI_ITEM_USE_UNION_QUIT_ACCEPT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_ITEM_USE_UNION_QUIT_ACCEPT");
	CGQuitUnionAccept _CGQuitUnionAccept;
	_CGQuitUnionAccept.setGuildID(left);
	g_pSocket->sendPacket(&_CGQuitUnionAccept); 

	gC_vs_ui.CloseMailBoxDialogBox();
}

void
UIMessageManager::Execute_UI_ITEM_USE_UNION_QUIT_DENY(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_ITEM_USE_UNION_QUIT_DENY");
	CGQuitUnionDeny _CGQuitUnionDeny;
	_CGQuitUnionDeny.setGuildID(left);
	g_pSocket->sendPacket(&_CGQuitUnionDeny); 

	gC_vs_ui.CloseMailBoxDialogBox();
}




void
UIMessageManager::Execute_UI_RECALL_BY_NAME(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_RECALL_BY_NAME");
	char *szSelectedID = gC_vs_ui.GetTeamMember_SelectedID();
	if(szSelectedID != NULL && strlen(szSelectedID)>0)
	{
		char szBuf[128];
		sprintf(szBuf, "*recall %s", szSelectedID );
		CGSay _CGSay;
		_CGSay.setMessage( szBuf );	//pWansungString );
		_CGSay.setColor( 0 );
		g_pSocket->sendPacket( &_CGSay );
	}
}

void
UIMessageManager::Execute_UI_UI_MODIFY_TAX(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_UI_MODIFY_TAX");

	CGModifyTaxRatio _CGModifyTaxRatio;
	_CGModifyTaxRatio.setRatio(left);
	g_pSocket->sendPacket( &_CGModifyTaxRatio );

}


void
UIMessageManager::Execute_UI_APPOINT_SUBMASTER(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_APPOINT_SUBMASTER");

	CGAppointSubmaster _CGAppointSubmaster;
	_CGAppointSubmaster.setGuildID(left);
	_CGAppointSubmaster.setName(std::string((char *)void_ptr));
	g_pSocket->sendPacket( &_CGAppointSubmaster );

}

//2004.11.27
 void
UIMessageManager::Execute_UI_DISPLAY_ITEM(int left, int right, void* void_ptr)
 {
	  	DEBUG_ADD("[UI] UI_SELECT_PERSNALSHOP_SLOT");

	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}
	if(g_pStorage2 == NULL)
		return;	


	MItem* pItem = (MItem*)void_ptr;
	g_pStorage2->SetCurrent( 0 );	// 확인용
 
 	int slot = right;

	
	const MItem* pStorageItem = g_pStorage2->GetItem( slot );
	//TYPE_OBJECTID mouseItemID = pMouseItem->GetID();		
	
	// Event GiftBox 아이템인 경우 못 놓는다.
	if (pItem->GetItemClass()!=ITEM_CLASS_EVENT_GIFT_BOX
		//&& pItem->GetItemClass()!=ITEM_CLASS_VAMPIRE_AMULET
//		&& pItem->GetItemClass()!=ITEM_CLASS_COUPLE_RING
//		&& pItem->GetItemClass()!=ITEM_CLASS_VAMPIRE_COUPLE_RING
//		&& !pItem->IsQuestItem()
//		&& pItem->IsNormalItem()
//		&& !((pItem->GetItemClass() == ITEM_CLASS_EVENT_TREE) && pItem->GetItemType() >= 13  )
//		&& pItem->GetItemClass() != ITEM_CLASS_EVENT_ITEM
//		&& pItem->GetItemClass() != ITEM_CLASS_CODE_SHEET
//		&& !(pItem->GetItemClass() == ITEM_CLASS_LUCKY_BAG && pItem->GetItemType() == 3)
		)
	{
		int current_storage = g_pStorage2->GetCurrent();
		const int storage_size = g_pStorage2->GetSize();
		const int storage_max = STORAGE_SLOT;
		
			
			for(int i = 0; i < storage_max; i++)
			{
				const MItem *p_slot_item = g_pStorage2->GetItem(i);
				
				// 슬랏이 비었으면 걍~ 넣는다
				if(p_slot_item == NULL)
				{
					CGDisplayItem _CGDisplayItem;
					_CGDisplayItem.setIndex(i);
					_CGDisplayItem.setItemObjectID(pItem->GetID());
					_CGDisplayItem.setPrice(pItem->GetPersnalPrice());
					//_CGDisplayItem.setXY(pMouseItem->GetGridX(),pMouseItem->GetGridY());
					_CGDisplayItem.setXY(pItem->GetGridX(),pItem->GetGridY());
					g_pSocket->sendPacket( &_CGDisplayItem ); 
					break;
				}
			}
			current_storage ++;
			if(current_storage >= storage_size)
				current_storage = 0;

		}
		else
		{
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_STORAGE].GetString());
		}
	}

 void
UIMessageManager::Execute_UI_UNDISPLAY_ITEM(int left, int right, void* void_ptr)
 {

	MItem* pMouseItem = (MItem*)void_ptr;
	const MItem* pStorageItem = g_pStorage2->GetItem( right );

	if (pMouseItem!=NULL)
	{
//		if (pStorageItem!=NULL)
//		{
			MItem* pRemovedItem = g_pStorage2->RemoveItem( left );

			if (pRemovedItem!=NULL)
			{
				 CGUndisplayItem _CGUndisplayItem;
				 _CGUndisplayItem.setIndex(left);
				 _CGUndisplayItem.setItemObjectID(pRemovedItem->GetID());
				 //_CGUndisplayItem.setXY(left,right);
				 _CGUndisplayItem.setXY(pMouseItem->GetGridX(),pMouseItem->GetGridY());
				 
				 g_pSocket->sendPacket( &_CGUndisplayItem );
			}
//		}
	}
	DEBUG_ADD("[UI] Execute_UI_UNDISPLAY_ITEM");
 }

 void
UIMessageManager::Execute_UI_STORE_SIGN(int left, int right, void* void_ptr)
 {
	 DEBUG_ADD("[UI] Execute_UI_STORE_SIGN");

	DEBUG_ADD_FORMAT("[Original] %s", (char*)void_ptr );

	char * pernalshop_message = (char*)void_ptr;
    CGStoreSign _CGStoreSign;
	
	char str[250];
	memset(str,0,250);
	strcpy(str, (char*)void_ptr);
	
	g_pChatManager->RemoveCurse( str );
	_CGStoreSign.setSign(str);
	g_pSocket->sendPacket( &_CGStoreSign );

	gC_vs_ui.ClosePersnalShopMessage();
 }

 void
 UIMessageManager::Execute_UI_STORE_OPEN(int left, int right, void* void_ptr)
 {
	 	DEBUG_ADD("[UI] Execute_UI_STORE_OPEN");
		
		CGStoreOpen _CGCGStoreOpen;
		gC_vs_ui.SetPersnalShoptoUI(2);
		g_pSocket->sendPacket( &_CGCGStoreOpen );
				
 }
 
 void
UIMessageManager::Execute_UI_STORE_CLOSE(int left, int right, void* void_ptr)
 {
	 DEBUG_ADD("[UI] Execute_UI_STORE_CLOSE");
	 
	 CGStoreClose _CGStoreClose;
	 gC_vs_ui.SetPersnalShoptoUI(3);
	 
	 g_pSocket->sendPacket( &_CGStoreClose );


 }

 void
UIMessageManager::Execute_UI_REQUEST_STORE_INFO(int left, int right, void* void_ptr)
 {
	 DEBUG_ADD("[UI] Execute_UI_REQUEST_STORE_INFO");
	 
	 if(g_pPlayer->IsFlyingCreature() 
		 || g_pPlayer->IsUndergroundCreature() 
		 //|| g_pPlayer->IsGroundCreature() 
		 || g_pPlayer->IsFakeCreature() 
		 || g_pPlayer->GetMoveDevice()== 1
		 || g_pPlayer->GetMoveDevice()== 2
		 || gC_vs_ui.IsRunningSlayerPortal()
		 || gC_vs_ui.IsRunningHorn()
		)
	 {
		 DEBUG_ADD("[UI] Execute_DON'T OPEN PERSNAL SHOP Cuz your's type can't open PersnalShop./......");
	 }
	 else
	 {
		 if(right == 1)
			 gC_vs_ui.SetPersnalShoptoUI(1); 
	 

		 CGRequestStoreInfo _CGRequestStoreInfo;
		 _CGRequestStoreInfo.setOwnerObjectID(left);
		 g_pSocket->sendPacket( &_CGRequestStoreInfo );
	 }

 }

 void
UIMessageManager::Execute_UI_MY_STORE_INFO(int left, int right, void* void_ptr)
 {

	 DEBUG_ADD("[UI] Execute_UI_MY_STORE_INFO"); // 아이템 정보 리스트 
	 
	 GCMyStoreInfo _GCMyStoreInfo;
	 _GCMyStoreInfo.getStoreInfo();

//	 if (g_pStorage2!=NULL)
//		 delete g_pStorage2;
//	 
//	 g_pStorage2 = new MStorage;
//	 
//	 g_pStorage2->Init( left ); 
//	 gC_vs_ui.RunPersnalShop();
//	 gC_vs_ui.SetPersnalShop( g_pStorage2 );
//	 
	 
	 g_pSocket->sendPacket( &_GCMyStoreInfo );

 }

void
UIMessageManager::Execute_UI_OTHER_STORE_INFO(int left, int right, void* void_ptr)
{
	// not sure
		 DEBUG_ADD("[UI] Execute_UI_OTHER_STORE_INFO");
	 
	  GCOtherStoreInfo _GCOtherStoreInfo;
	 _GCOtherStoreInfo.setObjectID(g_pTopView->GetSelectedCreature());
	  g_pSocket->sendPacket( &_GCOtherStoreInfo );
}

void
UIMessageManager::Execute_UI_BUY_STORE_ITEM(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_BUY_STORE_ITEM");
	
	if(g_pTopView->GetSelectedCreature() == NULL)
		return;
	MItem* pItem = (MItem*)void_ptr;		// inventory에 있는 아이템(NULL일 수도 있다)
	
	if(pItem != NULL)		
	{
		CGBuyStoreItem _CGBuyStoreItem;
		_CGBuyStoreItem.setIndex(left);
		_CGBuyStoreItem.setItemObjectID(pItem->GetID());
		_CGBuyStoreItem.setOwnerObjectID(gC_vs_ui.GetOtherObjectID());
		
		g_pSocket->sendPacket( &_CGBuyStoreItem );
	}

}

void
UIMessageManager::Execute_UI_REMOVE_STORE_ITEM(int left, int right, void* void_ptr)
{
	 DEBUG_ADD("[UI] Execute_UI_REMOVE_STORE_ITEM");
	 
	 GCRemoveStoreItem _GCRemoveStoreItem;
	 _GCRemoveStoreItem.setIndex(0);
	 _GCRemoveStoreItem.setOwnerObjectID(g_pPlayer->GetID());
	 g_pSocket->sendPacket( &_GCRemoveStoreItem );
}

void
UIMessageManager::Execute_UI_ADD_STORE_ITEM(int left, int right, void* void_ptr)
{

	DEBUG_ADD("[UI] Execute_UI_ADD_STORE_ITEM");
	 
	 GCAddStoreItem _GCAddStoreItem;
	 _GCAddStoreItem.setIndex(0);
	 _GCAddStoreItem.setOwnerObjectID(g_pPlayer->GetID());
	 g_pSocket->sendPacket( &_GCAddStoreItem );
}


void
UIMessageManager::Execute_UI_CLOSE_PERSNALSHOP(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CLOSE_PERSNALSHOP");
	gC_vs_ui.ClosePersnalShop();	
	
	if (g_pStorage2!=NULL)
	{
		delete g_pStorage2;
		g_pStorage2 = NULL;
	}
	 
	gC_vs_ui.SetPersnalShop( NULL );
}

void
UIMessageManager::Execute_UI_CLOSE_POWER_JJANG(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CLOSE_POWER_JJANG");
	gC_vs_ui.ClosePowerjjang();
}
void
UIMessageManager::Execute_UI_REQUEST_POWER_JJANG_POINT(int left, int right, void* void_ptr)
{ 
	DEBUG_ADD("[UI] Execute_UI_REQUEST_POWER_JJANG_POINT");

	CGRequestPowerPoint _CGRequestPowerPoint;
	_CGRequestPowerPoint.setCellNum(std::string((char *)void_ptr));
	g_pSocket->sendPacket( &_CGRequestPowerPoint );
}
void
UIMessageManager::Execute_UI_POWER_JJANG_GAMBLE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_POWER_JJANG_GAMBLE");

	CGUsePowerPoint _CGUsePowerPoint;
	g_pSocket->sendPacket( &_CGUsePowerPoint );
}


void
UIMessageManager::Execute_UI_CLOSE_SWAPADVANCEMENTITEM(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CLOSE_SWAPADVANCEMENTITEM");

	gC_vs_ui.FinishSwapAdvancementItem();
}

void
UIMessageManager::Execute_UI_SWAPADVANCEMENTITEM(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_SWAPADVANCEMENTITEM");
	
	if (g_Mode!=MODE_GAME)
	{
		DEBUG_ADD("Not Mode MODE_GAME");
		return;
	}
	
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		const MItem* pItem = g_pInventory->GetItem( left, right );

		if (pItem!=NULL && g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
		{
			// Event GiftBox 아이템인 경우 못 놓는다.
			
			if (!pItem->IsUniqueItem() && !pItem->IsQuestItem())
			{
				//-----------------------------------------------------
				// 아이템을 팔기 위한 packet을 보낸다.
				//-----------------------------------------------------
					CGShopRequestSell	_CGShopRequestSell;

					_CGShopRequestSell.setObjectID( (*g_pPCTalkBox).GetNPCID() );		// NPC ID	
					_CGShopRequestSell.setOpCode( SHOP_REQUEST_SWAP_ADVANCEMENT_ITEM );
					_CGShopRequestSell.setItemObjectID( pItem->GetID() );	// Item Object ID

					g_pSocket->sendPacket( &_CGShopRequestSell );
			
						
					//-------------------------------------------------
					// 검증을 위한 Temp Information설정
					//-------------------------------------------------
					(*g_pTempInformation).Mode	= TempInformation::MODE_SHOP_SELL;
					(*g_pTempInformation).pValue = (void*)pItem;

					//-------------------------------------------------
					// 다른 아이템에 접근 못하도록..
					//-------------------------------------------------
					UI_LockItemTrade();
			}
			else
			{
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[UI_STRING_MESSAGE_SWAP_ADVANCEMENT_ITEM_ERROR ].GetString());
			}
		}
		else
		{
			DEBUG_ADD("[Error] Item Is NULL");
		}
	}
}

void
UIMessageManager::Execute_UI_LEARN_ADVANCE_SKILL(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_LEARN_ADVANCE_SKILL");
	CGLearnSkill _CGLearnSkill;
	
	_CGLearnSkill.setSkillType( left );
	_CGLearnSkill.setSkillDomainType( right );
	
	g_pSocket->sendPacket( &_CGLearnSkill );
}

// 2005, 1, 11, sobeit add start - 불우이웃 돕기 성금 관련
void
UIMessageManager::Execute_UI_CAMPAIGN_HELP(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CAMPAIGN_HELP");
	
	// 성금 관련 패킷 보내기 - 금액 * 10000

	if(left>0)
	{
		CGDonationMoney _CGDonationMoney;
		_CGDonationMoney.setGold(left*10000);
		_CGDonationMoney.setDonationType(right);
		g_pSocket->sendPacket( &_CGDonationMoney );
	}
// 2005, 1, 11, sobeit add end - 불우이웃 돕기 성금 관련
}

// 2005, 1, 20, sobeit add start - 퀘스트 관련 
void
UIMessageManager::Execute_UI_RUN_NEXT_GQUEST_EXCUTE_ELEMENT(int left, int right, void* void_ptr)
{
	// quest element List에 저장된 다음 element를 실행 한다.
	DEBUG_ADD("[UI] Execute_UI_RUN_NEXT_GQUEST_EXCUTE_ELEMENT");
	gC_vs_ui.RunNextGQuestExcuteElement();
}

void
UIMessageManager::Execute_UI_GQUEST_SET_ACTION(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_GQUEST_SET_ACTION");
	// 특정 element 실행 시 특정 동작을 한다.
	if(left == 1) // dead
	{
		g_pPlayer->SetDead();
		g_pPlayer->SetResurrectZoneID(right);
	}
}

void
UIMessageManager::Execute_UI_GQUEST_ENDING_EVENT(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_GQUEST_ENDING_EVENT");
	// 특정 퀘스트 완료시 관련 이벤트를 진행 한다.
	MEvent event;
	event.eventID = EVENTID_ADVANCEMENT_QUEST_ENDING;
	event.eventType = EVENTTYPE_ZONE;
	event.eventFlag = EVENTFLAG_ONLY_EVENT_BACKGROUND | EVENTFLAG_NOT_DRAW_UI | EVENTFLAG_NOT_DRAW_CREATURE | EVENTFLAG_NOT_DRAW_INFORMATION | EVENTFLAG_NOT_DRAW_CREATURE_SHADOW | EVENTFLAG_NOT_DRAW_ITEM | EVENTFLAG_NOT_DRAW_MOUSE_POINTER | EVENTFLAG_NOT_DRAW_EFFECT | EVENTFLAG_DENY_INPUT | EVENTFLAG_NOT_FADE_SCREEN | EVENTFLAG_NOT_PLAY_SOUND;
	switch(left)
	{
	case 1: // 슬레이어 승직 퀘스트 엔딩 이벤트
		event.parameter1 = 0;
		event.parameter4 = 1;
		g_pEventManager->AddEvent(event);
		break;
	case 2: // 뱀파이어 승직 퀘스트 엔딩 이벤트
		event.parameter1 = 0;
		event.parameter4 = 3;
		g_pEventManager->AddEvent(event);
		break;
	case 3: // 아우스터즈 승직 퀘스트 엔딩 이벤트
		event.parameter1 = 0;
		event.parameter4 = 5;
		g_pEventManager->AddEvent(event);
		break;
	}
	
}
// 2005, 1, 20, sobeit add end - 퀘스트 관련 

// 2005, 1, 24, sobeit add start - 아이템 받기 이벤트 관련
void
UIMessageManager::Execute_UI_REQUEST_EVENT_ITEM(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_REQUEST_EVENT_ITEM");
	{
		CGGetEventItem _CGGetEventItem;
		_CGGetEventItem.setEventType(left);
		g_pSocket->sendPacket( &_CGGetEventItem );
	}
}
// 2005, 1, 24, sobeit add end - 아이템 받기 이벤트 관련

// 2005, 2, 25, sobeit add start - 서브 인벤토리를 닫는다.
void
UIMessageManager::Execute_UI_CLOSE_INVENTORY_SUB(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CLOSE_INVENTORY_SUB");
	{
		gC_vs_ui.CloseSubInventory();
	}
}

void
UIMessageManager::Execute_UI_ITEM_DROP_TO_INVENTORY_SUB(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_ITEM_DROP_TO_INVENTORY_SUB");
	
	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead() || !gC_vs_ui.IsRunningSubInventory())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}

	MItem* pMultiPackItem = gC_vs_ui.GetSubInventoryItem();

	if(NULL == pMultiPackItem)
		return;

	MItem* pMouseItem = UI_GetMouseItem();	// 현재 mouse의 item


	//-----------------------------------------------------------------
	// 검증받을게 없는 경우
	//-----------------------------------------------------------------
	if (g_pPlayer->IsItemCheckBufferNULL() && pMouseItem!=NULL
		&& g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{		
		if(pMouseItem->GetItemClass() == ITEM_CLASS_SUB_INVENTORY)
			return;
		TYPE_OBJECTID toInventoryItemID = (pMouseItem==NULL)? OBJECTID_NULL : pMouseItem->GetID();
		
		CGAddMouseToInventory _CGAddMouseToInventory;
		_CGAddMouseToInventory.setObjectID( toInventoryItemID );
		_CGAddMouseToInventory.setInvenX( left );//pItem->GetGridX() );
		_CGAddMouseToInventory.setInvenY( right );//pItem->GetGridY() );
		_CGAddMouseToInventory.setInventoryItemObjectID(pMultiPackItem->GetID());

		g_pSocket->sendPacket( &_CGAddMouseToInventory );

		MItem* pOldItem = NULL;
		if (gC_vs_ui.ReplaceSubInventoryItem(pMouseItem,		// 추가할 item
											left, right,	// 추가할 위치 
											pOldItem))		// 원래있던 item
		{
			if (pOldItem != NULL) // replace 되었는가?
			{				
				UI_PickUpItem( pOldItem );
			}
			else
			{
				UI_DropItem();
			}
		}
	}
	//-----------------------------------------------------------------
	// 검증 받아야할 다른 아이템이 있는 경우
	//-----------------------------------------------------------------
	else
	{
		DEBUG_ADD( "[Error] There is another item in ItemCheckBuffer or Mouse NULL");		

		if(!g_pPlayer->IsItemCheckBufferNULL())
			DEBUG_ADD_FORMAT("[ITEMDROP] !g_pPlayer->IsItemCheckBufferNULL() %d", g_pPlayer->GetItemCheckBufferStatus());
		if(pMouseItem == NULL)
			DEBUG_ADD("[ITEMDROP] pMouseItem == NULL)");
		if(g_pTempInformation->GetMode()!=TempInformation::MODE_NULL)
			DEBUG_ADD_FORMAT("[ITEMPDROP] g_pTempInformation->Mode!=TempInformation::MODE_NULL %d", g_pTempInformation->Mode);
	}
}


void
UIMessageManager::Execute_UI_ITEM_PICKUP_FROM_INVENTORY_SUB(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] UI_ITEM_PICKUP_FROM_INVENTORY"); 
	
	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead() || !gC_vs_ui.IsRunningSubInventory())
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}
	MItem* pItem = (MItem*)void_ptr;

	MItem* pMultiPackItem = gC_vs_ui.GetSubInventoryItem();

	TYPE_OBJECTID MultiPackItemID = (pMultiPackItem==NULL)? OBJECTID_NULL : pMultiPackItem->GetID();

	UI_PickUpItem( pItem );
	//---------------------------------------------------
	// Inventory에 있던 item을 mouse에 붙였다(-_-;)고
	// server로 packet을 보낸다.
	//---------------------------------------------------
	CGAddInventoryToMouse _CGAddInventoryToMouse;
	_CGAddInventoryToMouse.setObjectID( pItem->GetID() );
	_CGAddInventoryToMouse.setX( pItem->GetGridX() );
	_CGAddInventoryToMouse.setY( pItem->GetGridY() );
	_CGAddInventoryToMouse.setInventoryItemObjectID(MultiPackItemID);
	
	g_pSocket->sendPacket( &_CGAddInventoryToMouse );				
}

// 2005, 2, 25, sobeit add end - 서브 인벤토리를 닫는다.


// 2005, 3, 2, sobeit add start
void
UIMessageManager::Execute_UI_ITEM_USE_SUBINVENTORY(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_ITEM_USE_SUBINVENTORY"); 
	
	if (g_Mode!=MODE_GAME || g_pPlayer->IsDead() )
	{
		DEBUG_ADD("Not Mode MODE_GAME or Dead");
		return;
	}
	MItem* pSubInventory = g_pInventory->GetItem(right);

	if(NULL == pSubInventory)
		return;

	MItem* pItem = ((MSubInventory*)pSubInventory)->GetItemToModify( left );//(MItem*)void_ptr;

	if (pItem!=NULL && g_pPlayer->IsItemCheckBufferNULL())
	{
		// 자기종족 아이템만 쓰쟈-ㅅ-;
		if(g_pPlayer->IsSlayer() && pItem->IsSlayerItem() ||
			g_pPlayer->IsVampire() && pItem->IsVampireItem() ||
			g_pPlayer->IsOusters() && pItem->IsOustersItem())
		{
			pItem->UseInventory(pSubInventory->GetID());
		}
	}
	//----------------------------------------------------
	// item check buffer not null
	//----------------------------------------------------
	else
	{
		#ifdef OUTPUT_DEBUG
			if (pItem==NULL)
			{
				DEBUG_ADD("[Wait] Item is NULL");
			}
			else 
			{
				DEBUG_ADD("[Wait] Item Check Buffer is Not NULL");
			}
		#endif
	}
}

// 2005, 3, 2, sobeit add end



// 2005, 4, 21, sjheon add start -결혼축의금  관련
void
UIMessageManager::Execute_UI_WEDDING_CONTRIBUTION(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_WEDDING_CONTRIBUTION");
	
	// 결혼  축의금 

	if(left>0)
	{
		CGDonationMoney _CGDonationMoney;
		_CGDonationMoney.setGold(left*10000);
		_CGDonationMoney.setDonationType(right);
		g_pSocket->sendPacket( &_CGDonationMoney );
	}
// 2005, 4, 21, sjheon add end - 결혼축의금  관련
}

void 
UIMessageManager::Execute_UI_NETMARBLE_AGREEMENT( int left, int right, void* void_ptr )
{
	CLAgreement _CLAgreement;	
	_CLAgreement.setAgree( left == 1 );

	g_pSocket->sendPacket( &_CLAgreement);
}

void 
UIMessageManager::Execute_UI_BLOOD_BURST( int left, int right, void* void_ptr )
{
	int SkillType = 0; 

	if(left == 1)
	{
		if(g_pPlayer->IsSlayer())
		{
			int domain_level_Sword = (*g_pSkillManager)[SKILLDOMAIN_SWORD].GetDomainLevel();
			int domain_level_Blade = (*g_pSkillManager)[SKILLDOMAIN_BLADE].GetDomainLevel();
			int domain_level_Gun = (*g_pSkillManager)[SKILLDOMAIN_GUN].GetDomainLevel();
			int domain_level_Heal = (*g_pSkillManager)[SKILLDOMAIN_HEAL].GetDomainLevel();
			int domain_level_Enchant = (*g_pSkillManager)[SKILLDOMAIN_ENCHANT].GetDomainLevel();

			int domainLevel = 0 ;			
			domainLevel = max(domain_level_Sword , domain_level_Blade)  ;
			domainLevel = max(domainLevel , domain_level_Gun)  ;
			domainLevel = max(domainLevel , domain_level_Heal)  ;
			domainLevel = max(domainLevel , domain_level_Enchant)  ; 
		
			if(domainLevel == domain_level_Sword)
				SkillType = SKILL_HOLY_SWORD ; 
			else if(domainLevel == domain_level_Blade)
				SkillType = SKILL_HOLY_BLADE ; 
			else if(domainLevel == domain_level_Gun)
				SkillType = SKILL_BURST_GUN ; 
			else if(domainLevel == domain_level_Enchant)
				SkillType = SKILL_HOLY_MACE ; 
			else if(domainLevel == domain_level_Heal)
				SkillType = SKILL_HOLY_CROSS ; 
			
			//SkillType = SKILL_HOLY_CROSS ; 
		}
		else if(g_pPlayer->IsVampire())
			SkillType = SKILL_DARK_FORCE ; 
		
		else if(g_pPlayer->IsOusters())
		{
		
			MCreatureWear *pCreatureWear = (MCreatureWear *)g_pPlayer;
			const MCreatureWear::ADDON_INFO& addonInfoChakram = g_pPlayer->GetAddonInfo(ADDON_RIGHTHAND);
			bool bChakram = addonInfoChakram.bAddon && addonInfoChakram.ItemClass == ITEM_CLASS_OUSTERS_CHAKRAM;

			bool bWristlet = addonInfoChakram.bAddon && addonInfoChakram.ItemClass == ITEM_CLASS_OUSTERS_WRISTLET;
			
			
			if(bChakram)
				SkillType = SKILL_NATURAL_FORCE ; 
			else if(bWristlet)
			{
				int ItemClass = addonInfoChakram.ItemClass	; 
				int ItemType  = addonInfoChakram.ItemType	;
				ITEMTABLE_INFO iInfo = (*g_pItemTable)[ItemClass][ItemType];
				
				if(iInfo.ElementalType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE)
					SkillType = SKILL_FIRE_FORCE ; 
				else if(iInfo.ElementalType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER)
					SkillType = SKILL_WATER_FORCE ; 
				else if(iInfo.ElementalType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH)
					SkillType = SKILL_EARTH_FORCE ; 

			}
			else return ;
		}
	}
	else if(left ==  2)
	{
		if(g_pPlayer->IsSlayer())
			SkillType = SKILL_LOSE_SIGHT ; 
		else if(g_pPlayer->IsVampire())
			SkillType = SKILL_WIDE_BLOOD_DRAIN ; 
		else if(g_pPlayer->IsOusters())
			SkillType = SKILL_NATURAL_PEACE ; 
	}
	else if(left ==  3)
	{
		SkillType = SKILL_PARTY_AURA ; 
	}

	CGSkillToSelf _CGSkillToSelf; 
	_CGSkillToSelf.setSkillType( SkillType );
	_CGSkillToSelf.setCEffectID( 0 );
	g_pSocket->sendPacket( &_CGSkillToSelf );


	// 2001.8.20 주석처리
	g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_SKILL_SUCCESS, SkillType );
}

void 
UIMessageManager::Execute_UI_CLIENT_REMOVE_EFFECT_STATUS( int left, int right, void* void_ptr )
{
	int iEffectState = 0 ; 
	if(left == SKILL_CLIENT_MAGICAL_PET_CHANGER)
	{	
		UI_RemoveEffectStatus(EFFECTSTATUS_CLIENT_MAGICAL_PET_CHANGER) ; 
	}
	
}


void 
UIMessageManager::Execute_UI_MARKET_ACCOUNT( int left, int right, void* void_ptr )
{
	if(left == 0 && right == 0)
	{
		CGRequestWebMarket	_CGRequestWebMarket	;
		_CGRequestWebMarket.setCode(CGRequestWebMarket::REQUEST_WEB_MARKET)	; 
		g_pSocket->sendPacket( &_CGRequestWebMarket );	
	}	
	
	else if(left == 0 && right == 1)
	{
		CGRequestWebMarket	_CGRequestWebMarket	;
		_CGRequestWebMarket.setCode(CGRequestWebMarket::REQUEST_BUY_COMPLETE)	; 
		g_pSocket->sendPacket( &_CGRequestWebMarket );	
	}	

		//if(gC_vs_ui.IsRunningWebBrowser())
	//		gC_vs_ui.CloseWebBrowser(); 
	//	else if(left == 0 && !gC_vs_ui.IsRunningWebBrowser())
	//		gC_vs_ui.RunWebBrowser(g_hWnd,"http://gmk.darkeden.com/?userid=tezar" , g_hInstance) ;
	//	else if(left == 2 && !gC_vs_ui.IsRunningWebBrowser())
	//		
	
	
	
	//gC_vs_ui.RunWebBrowser(g_hWnd,"http://gmk.darkeden.com/?userid=sjheon" , g_hInstance) ;


	//}
	else if(left == 1 && right == 0 )
	{
		CGRequestWebMarket	_CGRequestWebMarket	; 
		_CGRequestWebMarket.setCode(CGRequestWebMarket::REQUEST_GOODS_LIST)	; 
		g_pSocket->sendPacket( &_CGRequestWebMarket );	
	}

}

