//-----------------------------------------------------------------------------
// UIMessageManager.cpp
//-----------------------------------------------------------------------------
// QuickSlot¿¡ ¹º°¡ ÇÒ·Á´Â °æ¿ì¿¡.. QuickSlotÀÌ ¾ø¾îÁú ¼ö°¡ ÀÖ´Ù... 
// Áö±Ý UI¿¡¼­ Å¸ÀÌ¹Ö ¹®Á¦°¡ ÀÖ¾î¼­ ¸ø °íÄ¡°í ÀÖÀ½.
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
#include "packet/CPackets/CGMixItem.h"					// -_- ºñ¢O ¾î½Ã½ºÆ®¿¡¼­ ÀÌ·¸°Ô ÇÏ¸é ³ª¿Â´Ù°íÇÏ±æ·¡
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
#include "Packet\Cpackets\CGUseMessageItemFromInventory.h"

//add by viva
#include "Packet\Cpackets\CGConnectSetKey.h"
#include "Packet\Gpackets\GCFriendChatting.h"
//end

//-----------------------------------------------------------------------------
// Chat¿¡¼­ »ç¿ëÇÏ´Â Æ¯¼öÇÑ ¹®ÀÚ
//-----------------------------------------------------------------------------
#define	SYMBOL_GLOBALCHAT			'!'			// zone Ã¤ÆÃ
#define	SYMBOL_WHISPER				'/'			// ±Ó¼Ó¸»
#define	SYMBOL_COMMAND				'@'			// Æ¯¼ö ¸í·É¾î
#define SYMBOL_MASTER_COMMAND		'*'			// ¿î¿µÀÚ¿ë Æ¯¼ö ¸í·É¾î

#define	STRING_IGNORE				"°ÅºÎ"		// ´ëÈ­ °ÅºÎ
#define	STRING_IGNORE_ENG			"ignore"	// ´ëÈ­ °ÅºÎ
#define	STRING_ACCEPT				"Çã¿ë"		// ´ëÈ­ Çã¿ë
#define	STRING_ACCEPT_ENG			"accept"	// ´ëÈ­ Çã¿ë
#define	STRING_ACCEPT_CURSE			"¹¹¶ó°í?"	// ³ª»Û¸» º¸±â
#define	STRING_ACCEPT_CURSE_ENG		"what?"		// ³ª»Û¸» º¸±â
#define	STRING_FILTER_CURSE			"¹Ù¸¥¸»"	// ¹Ù¸¥¸»¸¸ º¸±â
#define	STRING_FILTER_CURSE_ENG		"filter"	// ¹Ù¸¥¸»¸¸ º¸±â

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
extern BOOL g_MyFull;
extern RECT g_GameRect;
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
extern DWORD g_dwSeqNumL;
extern DWORD g_dwSeqNumG;

//-----------------------------------------------------------------------------
// Execute Logout
//-----------------------------------------------------------------------------
void
ExecuteLogout()
{
	//--------------------------------------------------
	// »ì¾ÆÀÖ´Â °æ¿ì
	//--------------------------------------------------
	if (g_Mode==MODE_GAME)
	{
		DEBUG_ADD("ExecuteLogout");

		UI_SaveUserOption();

		// ³¯¾¾ ¸ØÃã
		SetWeather(WEATHER_CLEAR, 0);

		// Á×¾úÀ¸¸é ºÎÈ° ÆÐÅ¶À» º¸³»°í LogoutÇÑ´Ù.
		// ±Ùµ¥.. ½ÇÁ¦ Ã³¸®µÉ¶§ º° ÀÇ¹Ì°¡ ¾ø³×.. - -;
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
			// À½¾Ç ¸ØÃá´Ù.
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
			// Thread Loading Á¾·á..
			//--------------------------------------------------
			StopLoadingThread();

			//
			// Client, Server¿¡ logout packet º¸³¿.
			//
			//--------------------------------------------------
			// °ÔÀÓ ¼­¹ö·Î CGLogout ÆÐÅ¶À» º¸³½´Ù.
			//--------------------------------------------------
				UI_SaveHotKeyToServer();

				CGLogout cgLogout;
			
				g_pSocket->sendPacket( &cgLogout );
				//g_pSocket->setPlayerStatus( CPS_END_SESSION );

				g_pSocket->processOutput();

				g_pSocket->setPlayerStatus( CPS_WAITING_FOR_GC_RECONNECT_LOGIN );	

				
				//g_pSocket->disconnect();

				// 2001.6.12 : releaseÇÏÁö ¾Ê°í ÀçÁ¢ÇÑ´Ù.
				//ReleaseSocket();

			/*
			if (g_pZone != NULL)
			{
				g_pZone->RemovePlayer();
				g_pPlayer->SetStop();
				g_pPlayer->SetAction( ACTION_STAND );
				//g_pZone->UnSetLight(g_pPlayer->GetX(), g_pPlayer->GetY(), g_pPlayer->GetLightSight());	// ½Ã¾ß Á¦°Å
			}
			*/

			// Ä³¸¯ÅÍ ¼±ÅÃÃ¢À¸·Î °£´Ù´Â message
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

			// ³¯¾¾ ¸ØÃã..
			//SetWeather(WEATHER_CLEAR, 0);
			ReleaseGameObject();

			// 2001.6.12 : ÀçÁ¢¼Ó~
			//SetMode( MODE_MAINMENU );	
			SetMode( MODE_WAIT_RECONNECT_LOGIN );

			//------------------------------------------------------
			// [ TEST CODE ]
			// ¿©±â¼­ sound¸¦ ÃÊ±âÈ­ÇØµµ µÇ³²??
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
			// ¸ðµÎ ´ëÈ­ Çã¿ë
			//------------------------------------------------------
			g_pChatManager->ClearID();
			g_pChatManager->SetAcceptMode();

//			gC_vs_ui.ServerDisconnectMessage();
//			gC_vs_ui.CloseAllDialog();

			// TitleÈ­¸é UI½ÃÀÛ
			//gC_vs_ui.EndTitle();
			gC_vs_ui.StartTitle();	
		
			gC_vs_ui.DropItem();

			g_bUIInput = FALSE;

			// ±Ó¼Ó¸» ´ë»óÀ» Áö¿öÁØ´Ù.
			g_pUserInformation->WhisperID.Release();

			//----------------------------------------------
			// message Á¦°Å
			//----------------------------------------------
			g_pSystemMessage->Clear();
			g_pGameMessage->Clear();
			g_pNoticeMessage->Clear();

			//------------------------------------------------------
			// À½¾Ç ½ÃÀÛ
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
	// WAV Ãâ·Â
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
	// MID Ãâ·Â
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
	// WAV Ãâ·Â
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
	// MID Ãâ·Â
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
	// ´ÜÁö Å×½ºÆ®¿ë.. - -;
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
//	m_UIMessageFunction[UI_CHANGE_PCS_CONNECTED_SLOT] = Execute_UI_CHANGE_PCS_CONNECTED_SLOT;		// ÀÌ¹Ì ¿¬°áµÈ ´Ù¸¥ »ç¶÷À» clickÇÏ¿´´Ù.
//	m_UIMessageFunction[UI_PLEASE_PCS_CONNECT_ME] = Execute_UI_PLEASE_PCS_CONNECT_ME;			// »ç¶÷ÀÌ ÀÖ´Â wait roomÀ» clickÇÏ¿´´Ù.
	//m_UIMessageFunction[UI_PCS_CONNECTOR_GRANTED] = Execute_UI_PCS_CONNECTOR_GRANTED;			// wait room¿¡ ÀÖ´Â connector¿Í ¿¬°áÀ» ½Â³«ÇÏ¿´´Ù.
//	m_UIMessageFunction[UI_QUIT_PCS_ONLINE_MODE] = Execute_UI_QUIT_PCS_ONLINE_MODE;			// PCS online mode¿¡¼­ quit ¹öÆ°À» ´­·¶´Ù.
//	m_UIMessageFunction[UI_END_PCS] = Execute_UI_END_PCS;								// sending ÇÏ°í ÀÖÀ» ¶§, end¸¦ ´­·¶´Ù.
//	m_UIMessageFunction[UI_SEND_PCS_NUMBER] = Execute_UI_SEND_PCS_NUMBER;
//	m_UIMessageFunction[UI_PDS_CLOSED] = Execute_UI_PDS_CLOSED;							// slayer pds°¡ ´ÝÇû´Ù. !ÇöÀç ¿¬°áÀº À¯ÁöÇÑ´Ù.

//	m_UIMessageFunction[UI_PLEASE_SET_SLAYER_VALUE] = Execute_UI_PLEASE_SET_SLAYER_VALUE;		// slayer pds¿¡¼­ gage menu¸¦ ½ÇÇàÇÏ¿´´Ù.

//	m_UIMessageFunction[UI_LEARN_SLAYER_SKILL] = Execute_UI_LEARN_SLAYER_SKILL;				// skill tree¿¡¼­ ¹è¿ï ¼ö ÀÖ´Â skillÀ» ¼±ÅÃÇÏ¿´´Ù.
//	m_UIMessageFunction[UI_CLOSE_SKILL_VIEW] = Execute_UI_CLOSE_SKILL_VIEW;

	m_UIMessageFunction[UI_BACKGROUND_MOUSE_FOCUS] = Execute_UI_BACKGROUND_MOUSE_FOCUS;			// Mouse pointer°¡ UI Window¿¡¼­ ÀÌµ¿¾øÀÌ ÀÏÀ» ¶§, ±× Window°¡ »ç¶óÁú °æ¿ì Client¿¡ Àü´Þ.
	m_UIMessageFunction[UI_REMOVE_BACKGROUND_MOUSE_FOCUS] = Execute_UI_REMOVE_BACKGROUND_MOUSE_FOCUS;// Game Menu¿Í °°Àº topmost Window°¡ Ãâ·ÂµÇ¾úÀ» ¶§´Â background focus¸¦ Á¦°ÅÇÏ´Â °ÍÀÌ ÁÁ´Ù.

	//
	// »óÁ¡ Message
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

	m_UIMessageFunction[UI_CHARACTER_MANAGER_FINISHED] = Execute_UI_CHARACTER_MANAGER_FINISHED;	// Characters Window¿¡¼­ ÃÊ±âÈ­¸éÀ¸·Î °¥ ¶§ ¾Ë·ÁÁØ´Ù.
	m_UIMessageFunction[UI_TERMINATION] = Execute_UI_TERMINATION;						// Á¾·á
	m_UIMessageFunction[UI_LOGIN] = Execute_UI_LOGIN;								// »ç¿ëÀÚ login ¿äÃ»
	m_UIMessageFunction[UI_LOGOUT] = Execute_UI_LOGOUT;								
	//m_UIMessageFunction[UI_NEW_USER_REGISTRATION] = Execute_UI_NEW_USER_REGISTRATION;			// »õ »ç¿ëÀÚ µî·Ï ¿äÃ»
	m_UIMessageFunction[UI_CONNECT] = Execute_UI_CONNECT;							   // Game connection.
	m_UIMessageFunction[UI_CHAT_RETURN] = Execute_UI_CHAT_RETURN;						// Ã¤ÆÃÃ¢ ÀÔ·Â.
//	m_UIMessageFunction[UI_CHAT_SELECT_NAME] = Execute_UI_CHAT_SELECT_NAME;						// Ã¤ÆÃÃ¢ ÀÌ¸§ ¼±ÅÃ.
	m_UIMessageFunction[UI_SELECT_SKILL] = Execute_UI_SELECT_SKILL;						// Skill selection.
	m_UIMessageFunction[UI_CANCEL_SELECT_SKILL] = Execute_UI_CANCEL_SELECT_SKILL;				// Skill ¼±ÅÃÀÌ Ãë¼ÒµÇ¾ú´Ù.

	//
	// Item message
	//

	// drop
	m_UIMessageFunction[UI_ITEM_DROP_TO_CLIENT] = Execute_UI_ITEM_DROP_TO_CLIENT;				// ItemÀ» ¹Ù´Ú¿¡ ¶³¾î¶ß·È´Ù.
	m_UIMessageFunction[UI_ITEM_DROP_TO_INVENTORY] = Execute_UI_ITEM_DROP_TO_INVENTORY; 		// Inventory¿¡ ³õ´Â´Ù.
	//m_UIMessageFunction[UI_ITEM_DROP_TO_TRADEGRID] = Execute_UI_ITEM_DROP_TO_TRADEGRID;			// ±³È¯Ã¢ ±×¸®µå¿¡ ³õ´Â´Ù.
	m_UIMessageFunction[UI_ITEM_DROP_TO_GEAR] = Execute_UI_ITEM_DROP_TO_GEAR;				// Gear¿¡ ³õ´Â´Ù.
	m_UIMessageFunction[UI_ITEM_DROP_TO_QUICKSLOT] = Execute_UI_ITEM_DROP_TO_QUICKSLOT;			// Quick Slot¿¡ ³õ´Â´Ù.

	// pick up
	// Client¿¡¼­ ÁýÀ» ¶§´Â Client -> UI ÀÌ´Ù.
	//m_UIMessageFunction[UI_ITEM_PICKUP_FROM_TRADEGRID] = Execute_UI_ITEM_PICKUP_FROM_TRADEGRID;
	m_UIMessageFunction[UI_ITEM_PICKUP_FROM_INVENTORY] = Execute_UI_ITEM_PICKUP_FROM_INVENTORY;
	m_UIMessageFunction[UI_ITEM_PICKUP_FROM_GEAR] = Execute_UI_ITEM_PICKUP_FROM_GEAR;
	m_UIMessageFunction[UI_ITEM_PICKUP_FROM_QUICKSLOT] = Execute_UI_ITEM_PICKUP_FROM_QUICKSLOT;

	// insert to other Item
	//m_UIMessageFunction[UI_ITEM_INSERT_FROM_TRADEGRID] = Execute_UI_ITEM_INSERT_FROM_TRADEGRID;
	m_UIMessageFunction[UI_ITEM_INSERT_FROM_INVENTORY] = Execute_UI_ITEM_INSERT_FROM_INVENTORY;
	m_UIMessageFunction[UI_ITEM_INSERT_FROM_GEAR] = Execute_UI_ITEM_INSERT_FROM_GEAR;
	m_UIMessageFunction[UI_ITEM_INSERT_FROM_QUICKSLOT] = Execute_UI_ITEM_INSERT_FROM_QUICKSLOT;  // ½×ÀÌ´Â °æ¿ì - ¾ÆÁ÷ Áö¿ø¾ÈÇÔ -

	// use
	m_UIMessageFunction[UI_ITEM_USE] = Execute_UI_ITEM_USE;							// Inventory¿¡¼­ »ç¿ë.
	m_UIMessageFunction[UI_ITEM_USE_QUICKSLOT] = Execute_UI_ITEM_USE_QUICKSLOT;				// Quick Item Slot¿¡¼­ »ç¿ë.

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
//add by viva
	m_UIMessageFunction[UI_CLOSE_FRIEND_CHATTING_INFO] = Execute_UI_CLOSE_FRIEND_CHATTING_INFO;
	m_UIMessageFunction[UI_OPEN_FRIEND_CHATTING_INFO] = Execute_UI_OPEN_FRIEND_CHATTING_INFO;
	//friend message
	m_UIMessageFunction[UI_FRIEND_CHATTING_SEND_MESSAGE] = Execute_UI_FRIEND_CHATTING_SEND_MESSAGE;
	m_UIMessageFunction[UI_FRIEND_CHATTING_UPDATE] = Execute_UI_FRIEND_CHATTING_UPDATE;
	m_UIMessageFunction[UI_FRIEND_CHATTING_ADD_FRIEND] = Execute_UI_FRIEND_CHATTING_ADD_FRIEND;
	//ask_friend_request
	m_UIMessageFunction[UI_FRIEND_REQUEST_ACCEPT] = Execute_UI_FRIEND_REQUEST_ACCEPT;
	//ask_friend_ask_close
	m_UIMessageFunction[UI_FRIEND_ASK_CLOSE] = Execute_UI_FRIEND_ASK_CLOSE;
	//ask_friend_delete_request
	m_UIMessageFunction[UI_FRIEND_DELETE_ASK] = Execute_UI_FRIEND_DELETE_ASK;
	m_UIMessageFunction[UI_FRIEND_DELETE_ACCEPT] = Execute_UI_FRIEND_DELETE_ACCEPT;
//end
	m_UIMessageFunction[UI_JOIN_READY_TEAM] = Execute_UI_JOIN_READY_TEAM;				// void_ptr = TEAM_NAME
	m_UIMessageFunction[UI_JOIN_REGIST_TEAM] = Execute_UI_JOIN_REGIST_TEAM;			// void_ptr = TEAM_NAME

	m_UIMessageFunction[UI_REGIST_GUILD_MEMBER] = Execute_UI_REGIST_GUILD_MEMBER;			// void_ptr = introduction max:150byte Ã¢ ´Ý¾ÆÁÙ°Í!
	m_UIMessageFunction[UI_REGIST_GUILD_TEAM] = Execute_UI_REGIST_GUILD_TEAM;			// left = TEAM_NAME, void_ptr = introduction max:150byte Ã¢ ´Ý¾ÆÁÙ°Í!

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

	// 2004, 5, 11 sobeit add start - Æê º¸°ü¼Ò
	m_UIMessageFunction[UI_CLOSE_PETSTORAGE] = Execute_UI_CLOSE_PETSTORAGE;
	m_UIMessageFunction[UI_CLOSE_KEEP_PETITEM] = Execute_UI_CLOSE_KEEP_PETITEM;
	m_UIMessageFunction[UI_CLOSE_GET_KEEP_PETITEM] = Execute_UI_CLOSE_GET_KEEP_PETITEM;
	m_UIMessageFunction[UI_KEEP_PETITEM] = Execute_UI_KEEP_PETITEM;
	m_UIMessageFunction[UI_GET_KEEP_PETITEM] = Execute_UI_GET_KEEP_PETITEM;
	// 2004, 5, 11 sobeit add end - Æê º¸°ü¼Ò

	// 2004, 5, 28 sobeit add start - sms °ü·Ã
	m_UIMessageFunction[UI_CLOSE_SMS_MESSAGE] = Execute_UI_CLOSE_SMS_MESSAGE;
	m_UIMessageFunction[UI_SEND_SMS_MESSAGE] = Execute_UI_SEND_SMS_MESSAGE;
	m_UIMessageFunction[UI_SMS_OPEN_LIST] = Execute_UI_SMS_OPEN_LIST;
	m_UIMessageFunction[UI_SMS_RECORD] = Execute_UI_SMS_RECORD;
	m_UIMessageFunction[UI_SMS_DELETE] = Execute_UI_SMS_DELETE;
	m_UIMessageFunction[UI_CLOSE_SMS_LIST] = Execute_UI_CLOSE_SMS_LIST;
	m_UIMessageFunction[UI_CLOSE_SMS_RECORD] = Execute_UI_CLOSE_SMS_RECORD;
	m_UIMessageFunction[UI_SMS_ADD_SEND_LIST] = Execute_UI_SMS_ADD_SEND_LIST;
	// 2004, 5, 28 sobeit add end

	// 2004, 6, 15 sobeit add start - nameing °ü·Ã
	m_UIMessageFunction[UI_CLOSE_NAMING] = Execute_UI_CLOSE_NAMING;
	m_UIMessageFunction[UI_CHANGE_CUSTOM_NAMING] = Execute_UI_CHANGE_CUSTOM_NAMING;
	m_UIMessageFunction[UI_SELECT_NAMING] = Execute_UI_SELECT_NAMING;
	m_UIMessageFunction[UI_CLOSE_NAMING_CHANGE] = Execute_UI_CLOSE_NAMING_CHANGE;
	m_UIMessageFunction[UI_RUN_NAMING_CHANGE] = Execute_UI_RUN_NAMING_CHANGE;
	// 2004, 6, 15 sobeit add end

	// 2004, 7, 12 sobeit add start - quest °ü·Ã
	m_UIMessageFunction[UI_CLOSE_QUEST_MANAGER] = Execute_UI_CLOSE_QUEST_MANAGER;
	m_UIMessageFunction[UI_CLOSE_QUEST_LIST] = Execute_UI_CLOSE_QUEST_LIST;
	m_UIMessageFunction[UI_CLOSE_QUEST_DETAIL] = Execute_UI_CLOSE_QUEST_DETAIL;
	m_UIMessageFunction[UI_CLOSE_QUEST_MISSION] = Execute_UI_CLOSE_QUEST_MISSION;
	m_UIMessageFunction[UI_CLOSE_QUEST_ITEM] = Execute_UI_CLOSE_QUEST_ITEM;
	m_UIMessageFunction[UI_CLOSE_QUEST_ICON] = Execute_UI_CLOSE_QUEST_ICON;
	m_UIMessageFunction[UI_GQUEST_ACCEPT] = Execute_UI_GQUEST_ACCEPT;
	m_UIMessageFunction[UI_GQUEST_GIVEUP] = Execute_UI_GQUEST_GIVEUP;

	// 2004, 7, 12 sobeit add end - quest °ü·Ã

	m_UIMessageFunction[UI_ITEM_USE_GQUEST_ITEM] = Execute_UI_ITEM_USE_GQUEST_ITEM;

	// 2004,10.9 csm guild °ü·Ã 
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

	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 Ôö¼Ó°üÖÐ°ü
		m_UIMessageFunction[UI_CLOSE_INVENTORY_SUB]			=	 Execute_UI_CLOSE_INVENTORY_SUB;
		m_UIMessageFunction[UI_ITEM_DROP_TO_INVENTORY_SUB]		=	 Execute_UI_ITEM_DROP_TO_INVENTORY_SUB;
		m_UIMessageFunction[UI_ITEM_PICKUP_FROM_INVENTORY_SUB]	=	 Execute_UI_ITEM_PICKUP_FROM_INVENTORY_SUB;

		m_UIMessageFunction[UI_ITEM_USE_SUBINVENTORY]	=	 Execute_UI_ITEM_USE_SUBINVENTORY;
	#endif
}

//-----------------------------------------------------------------------------
// Execute
//-----------------------------------------------------------------------------
// message¿Í ¿¬°áµÇ¾î ÀÖ´Â ÀûÀýÇÑ Ã³¸® ÇÔ¼ö¸¦ È£ÃâÇÑ´Ù.
//-----------------------------------------------------------------------------
void			
UIMessageManager::Execute(DWORD message, int left, int right, void* void_ptr)
{
	// ±âº»ÀûÀ¸·Î MODE_GAMEÀÌ¸é
	// NULLÀÌ ¾Æ´Ï¾î¾ß ÇÏ´Â °ªµéÀÌ ÀÖ´Ù..
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
// »õ Ä³¸¯ÅÍ ¸¸µé±â
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
	// ÀÌ¸§ ±æÀÌ Ã¼Å©
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
			// Àß¸øµÈ IDÀÎ °æ¿ì
			//---------------------------------------------						
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
			bAllOK = FALSE;
		}
		else
		{
			char strName[80];
			strcpy(strName, pChar->sz_name);

			// ¾È ÁÁÀº ¸»ÀÌ µé¾îÀÖ´Â °æ¿ì´Â Çã¿ëÀÌ ¾ÈµÈ´Ù
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
	// ¸ðµç data°¡ Á¤»óÀûÀÌ¸é...
	//---------------------------------------------
	if (bAllOK)
	{

			//char wansungName[16];

			//---------------------------------------------
			// Á¶ÇÕÇü --> ¿Ï¼ºÇü
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

			// »ö±ò È®ÀÎ
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
		// CLIENT - »õ »ç¿ëÀÚ µî·Ï °¡´É
		//-----------------------------------------------------------
		case ClientConfig::NUR_CLIENT :
		{
//			gC_vs_ui.RunNewUserRegistration();
		}
//		break;

		//-----------------------------------------------------------
		// HOMEPAGE - È¨ÆäÀÌÁö¸¦ ¶ç¿öÁÖ¸é¼­ µî·Ï
		//-----------------------------------------------------------
		case ClientConfig::NUR_HOMEPAGE :		
		{
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_HOMEPAGE].GetString() );			

			// Á¾·á..
			SetMode( MODE_QUIT );

			char str[256];

			GetWindowsDirectory(
				str,  // address of buffer for Windows directory
				255        // size of directory buffer
			);

			sprintf(str, "%s\\Explorer.exe", str);
			
			CDirectDraw::GetDD()->RestoreDisplayMode();
#ifdef __YHDK2__
			_spawnl(_P_NOWAIT, str, "Explorer.exe", "http://www.yhdk2.cn", NULL);
#else
			_spawnl(_P_NOWAIT, str, "Explorer.exe", "http://www.ttdk2.com", NULL);
#endif

			//_spawnl(_P_NOWAIT, str, "Explorer.exe", "http://www.ttdk2.cn", NULL);
		}
		break;

		//-----------------------------------------------------------
		// MESSAGE_HOMEPAGE - È¨ÆäÀÌÁö¿¡¼­ µî·ÏÇÏ¶ó´Â message
		//-----------------------------------------------------------
		case ClientConfig::NUR_MESSAGE_HOMEPAGE :	
		{
			g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_HOMEPAGE].GetString() );			
		}
		break;
		
		//-----------------------------------------------------------
		// DENY - »õ »ç¿ëÀÚ µî·Ï ºÒ°¡		
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
	// ID Ã¼Å©
	//--------------------------------------------------
	if (IsValidID(pName, NULL))
	{
		int len = strlen(pName);	
		
		if (len<PlayerInfo::minIDLength || len>PlayerInfo::maxIDLength)
		{
			char strTemp[128];
			sprintf(strTemp, "ID´Â %d~%dÀÚÀÔ´Ï´Ù", PlayerInfo::minIDLength, PlayerInfo::maxIDLength);
			g_pUIDialog->PopupFreeMessageDlg( strTemp );						
		}
		else
		{
			//--------------------------------------------------
			// socket ÃÊ±âÈ­
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


			//gC_vs_ui.AleadyExistIdMessage(); // »ç¿ëºÒ°¡
			//gC_vs_ui.NoAleadyExistIdMessage(); // »ç¿ë°¡´É						
		}
	}
	else
	{
		g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
	}

	// Áö¿öµµ µÇ´Â°Ô ¸Â´ÂÁö.. Èì *_*;
	DeleteNewArray( void_ptr );
}


//-----------------------------------------------------------------------------
//
//		Ä³¸¯ÅÍ »èÁ¦
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
	// ÁÖ¹Îµî·Ï¹øÈ£ Ã¼Å©
	//--------------------------------------------------
	// ³Ý¸¶ºí¿ë
	if (g_pUserInformation->IsNetmarble || g_pUserInformation->bChinese || IsValidSSN( pChar->sz_part1, pChar->sz_part2 ))
	{
		char strTemp[20];
		
		if( g_pUserInformation->bChinese )
		{
			sprintf(strTemp,"%s",pChar->sz_part1);
		} else
		if(!g_pUserInformation->IsNetmarble)
		{
			// xxxxxx-xxxxxxx Çü½ÄÀ¸·Î ¸¸µç´Ù.
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
			

		// ÁÖ¹Îµî·Ï¹øÈ£
		//pChar->sz_part1
		//pChar->sz_part2					

		// ÁÖ¹Îµî·Ï¹øÈ£°¡ Æ²·ÈÀ» ¶§ÀÇ message
		//gC_vs_ui.Invalid_SSN_Message();

		// Ä³¸¯ÅÍ »èÁ¦
		//gC_vs_ui.DeleteCharacter(slot);

		//
		// ((DELETE_CHARACTER *)void_ptr) = sz_part1, sz_part2, slot
		//

		// ³Ý¸¶ºí¿ë
		if(!g_pUserInformation->IsNetmarble)
		{
			DeleteNewArray( pChar->sz_part1 );
			DeleteNewArray( pChar->sz_part2 );
		}

		//gC_vs_ui.Invalid_SSN_Message();
		//gC_vs_ui.DeleteCharacter( pChar->slot );

		//--------------------------------------------
		// »èÁ¦ÇÒ·Á´Â slotÀúÀå
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
//		ÇÁ·Î±×·¥ Á¾·á
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
// »õ »ç¿ëÀÚ µî·Ï
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
	// // !string ptrÀ» ÀúÀåÇÏ¸é ¾ÈµÈ´Ù.
	//
	//struct NEW_REGISTRATION
	//{
	//	char *	sz_id;
	//	char *	sz_password;
	//	char *	sz_repassword;
	//	char *	sz_name;
	//	char *	sz_email;
	//	char *	sz_address;
	//	char *	sz_ssn_number_part1; // ÁÖ¹Î¹øÈ£
	//	char *	sz_ssn_number_part2; // ÁÖ¹Î¹øÈ£
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
	// string ±æÀÌ Ã¼Å©
	//--------------------------------------------------
	if (pReg->sz_id==NULL 
		|| pReg->sz_password==NULL
		|| pReg->sz_name==NULL
		|| pReg->sz_ssn_number_part1==NULL
		|| pReg->sz_ssn_number_part2==NULL
		|| pReg->sz_email==NULL)
	{
		// ÇÊ¼öÇ×¸ñÀÌ ÀÔ·Â ¾ÈµÈ °æ¿ì
		AllOK = FALSE;

		g_pUIDialog->PopupFreeMessageDlg((*g_pGameStringTable)[STRING_USER_REGISTER_EMPTY_FIELD].GetString());
	}
	else
	{
		int len;
		char strTemp[128];
			
		
		//--------------------------------------------------
		// ID ±æÀÌ Ã¼Å©
		//--------------------------------------------------
		len = strlen(pReg->sz_id);	
		
		if (len<PlayerInfo::minIDLength || len>PlayerInfo::maxIDLength)
		{
			sprintf(strTemp, (*g_pGameStringTable)[STRING_USER_REGISTER_ID_LENGTH].GetString(), PlayerInfo::minIDLength, PlayerInfo::maxIDLength);
			g_pUIDialog->PopupFreeMessageDlg( strTemp );
			AllOK = FALSE;
		}

		//---------------------------------------------
		// Àß¸øµÈ IDÀÎÁö Ã¼Å©ÇÑ´Ù.
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

				// ¾È ÁÁÀº ¸»ÀÌ µé¾îÀÖ´Â °æ¿ì´Â Çã¿ëÀÌ ¾ÈµÈ´Ù.
				if (g_pChatManager->RemoveCurse(strName))
				{
					g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
					AllOK = FALSE;
				}								
			}
		}
		
		//--------------------------------------------------
		// Password ±æÀÌ Ã¼Å©
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
			//else if (!IsValidID(pReg->sz_id))	// ¿ø·¡´Â ÀÌ°Å ½á¾ßµÇ´Âµ¥
			// ÀÌ¹Ì ¸¸µé¾îÁø ¾ÆÀÌµð Áß¿¡..  ID»ý¼º·ê¿¡ ÀûÇÕÇÏÁö ¾ÊÀº °Íµµ ÀÖ¾î¼­..
			{
				// ID¿¡ Æ¯¼ö¹®ÀÚ°¡ µé¾î°£ °æ¿ì
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_ID_SPECIAL].GetString() );
				AllOK = FALSE;
			}			
			else if (!IsValidPassword(pReg->sz_password))
			{
				// ÆÐ½º¿öµå°¡ Àß¸øµÈ °æ¿ì
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_PASSWORD_SPECIAL].GetString() );
				AllOK = FALSE;
			}
			else
			{
				//--------------------------------------------------
				// ¼ýÀÚ¸¸ »ç¿ëÇÏ¸é ¾ÈµÈ´Ù.
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

				if (AllNumber)	// ÀüºÎ ¼ýÀÚÀÎ °æ¿ì..
				{
					g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_PASSWORD_NUMBER].GetString() );
					AllOK = FALSE;
				}
			}

		}

		//--------------------------------------------------
		// ÀÌ¸§
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
		// ÀÌ¸§
		//--------------------------------------------------
		if (AllOK)
		{
			// Á¦´ë·Î ÀÔ·ÂµÈ °æ¿ì
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
				// ÁÖ¹Îµî·Ï¹øÈ£ Ã¼Å©
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
	// ¸ðµÎ Á¤»óÀÌ¸é..
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
					class ifstream versionFile;//(FILE_INFO_ACTION, ios::binary);
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
			// ÇÊ¼öÇ×¸ñ
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
			// ÇÊ¼öÇ×¸ñÀÌ ¾Æ´Ñ °Íµé
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
			// Áö±Ý ¾ø´Ù.. 
			//--------------------------------------------------
			_CLRegisterPlayer.setProfile( "profile" );

			g_pSocket->setPlayerStatus( CPS_AFTER_SENDING_CL_REGISTER_PLAYER );

			g_pSocket->sendPacket( &_CLRegisterPlayer );
			
		#endif	

		// id±â¾ï
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

	g_dwSeqNumL = rand()%1024;
	g_dwSeqNumG = 0;

	UI_SaveUserOption();

	//
	// ((C_VS_UI::LOGIN *)void_ptr) = id, password
	//
	// !string ptrÀ» ÀúÀåÇÏ¸é ¾ÈµÈ´Ù.
	//
	LOGIN*	login = (LOGIN*)void_ptr;

	if (g_Mode!=MODE_MAINMENU)
	{
		DeleteNewArray(login->sz_id);
		DeleteNewArray(login->sz_password);

		DEBUG_ADD("Not Mode MODE_MAINMENU");
		return;
	}	

	// ½½·¹ÀÌ¾î ±×¸² ÀÏºÎ ·Îµù - 2001.8.20
	LoadingAddonSPK( false );

	if (login->sz_id!=NULL && login->sz_password!=NULL)
	{
		//if (IsValidID(login->sz_id, NULL))
		{
			// ID±â¾ï
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
					// Á¶ÇÕÇü --> ¿Ï¼ºÇü
					//---------------------------------------------
					//UI_JohapToWansung( login->sz_id, wansungID );
	
					//--------------------------------------------------
					// ·Î±×ÀÎ ¼­¹ö¿¡ ¿¬°áÇÑ ÈÄ 
					// °¡Àå ¸ÕÀú º¸³»¾ß ÇÏ´Â ÆÐÅ¶Àº CLLogin ÆÐÅ¶ÀÌ´Ù.
					// ±Ùµ¥,
					// VersionÃ¼Å© packetÀÌ ¸ÕÀú´Ù. - -;
					//--------------------------------------------------
					//--------------------------------------------------
					// CLVersionCheck
					//--------------------------------------------------
					#if !defined(_DEBUG) && !defined(OUTPUT_DEBUG)
						int version = g_pUserInformation->GameVersion;
//						class ifstream versionFile;//(FILE_INFO_ACTION, ios::binary);
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
					//add by viva
					CGConnectSetKey cgConnectSetKey;
					cgConnectSetKey.setEncryptKey(rand());
					cgConnectSetKey.setHashKey(rand());
					g_pSocket->sendPacket(&cgConnectSetKey);
					UpdateSocketOutput();
					cgConnectSetKey.execute(g_pSocket);
					Sleep(500);
					//--------------------------------------------------
					// CLLogin
					//--------------------------------------------------
					CLLogin clLogin;

					//clLogin.setID("Reiot");
					//clLogin.setPassword("fpdldhxm");

					// ³Ý¸¶ºí¿ë
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
					// UserInformation¿¡ ÀúÀåÇÑ´Ù.
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
				// ÆÐ½º¿öµå°¡ Àß¸øµÈ °æ¿ì
				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_PASSWORD_SPECIAL].GetString() );
			}
		}
		//---------------------------------------------
		// Àß¸øµÈ IDÀÎ °æ¿ì
		//---------------------------------------------
		//else
		//{
			//g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );							
		//}
	}
	

	// (!) ¿ø·¡´Â Ä³¸¯ÅÍ ¼±ÅÃ Ã¢À» ¶ç¿ö¾ß ÇÑ´Ù.

	//gC_vs_ui.StartGame();
	//gC_vs_ui.StartCharacterManager();

	DeleteNewArray(login->sz_id);
	DeleteNewArray(login->sz_password);
	DEBUG_ADD("[Execute_UI_LOGIN] OK");
}

//-----------------------------------------------------------------------------
//
// Ä³¸¯ÅÍ ¼±ÅÃ Ã¢¿¡¼­ main menu·Î µ¹¾Æ°¥¶§
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

	// ³Ý¸¶ºí¿ë
	if(g_pUserInformation->IsNetmarble)
	{
		SetMode( MODE_MAINMENU );
		return;
	}

	//
	// Client, Server¿¡ logout packet º¸³¿.
	//
	//--------------------------------------------------
	// Login ¼­¹ö·Î CLLogout ÆÐÅ¶À» º¸³½´Ù.
	//--------------------------------------------------
	/*
	#ifdef	CONNECT_SERVER		
		
		// hot key saveÇÑ´Ù.
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
// UI_CONNECT - Ä³¸¯ÅÍ ¼±ÅÃ ÇßÀ» ¶§
//
//-----------------------------------------------------------------------------
// °ÔÀÓ ½ÃÀÛÇÏ·Á°í ÇÒ¶§...
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
	// À½¾Ç ÁßÁö - 2001.8.20
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
	// Á¢¼ÓÇØ¾ßµÈ´Ù°í Ãâ·ÂÇØÁØ´Ù.
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
			// Last¸¦ BackÀ¸·Î copy - 3D HALÀÌ ¾Æ´Ñ °æ¿ì¸¸..
			//-----------------------------------------------------------------		
			point.x = 0;
			point.y = 0;
			RECT rect = { 0, 0, g_GameRect.right, g_GameRect.bottom };
			g_pBack->BltNoColorkey( &point, g_pLast, &rect );	
		}
		CDirectDraw::Flip();

		// È­¸é¿¡ ±×·ÁÁÖ°í Áö¿ì¸é µÈ´ç.. ¤»¤»
		DEBUG_ADD("close msg dlg");
		g_pUIDialog->CloseMessageDlg();
	}

	DEBUG_ADD("LoadingAddonSPK");

	//------------------------------------------------------------
	// Test 2001.8.20
	//------------------------------------------------------------
	// Ä³¸¯ ¼±ÅÃ ÈÄ¿¡ ¹Ù·Î
	// ½½·¹ÀÌ¾î ±×¸² ÀÏºÎ ·Îµù - 2001.8.20
	LoadingAddonSPK( false );
	

	DEBUG_ADD_FORMAT("CLSelectPC(%d)", left);

	CLSelectPC clSelectPC;
	// CGConnect ¶§ »ç¿ëÇÏ·Á¸é ¿©±â¼­ ÀúÀåÇØµÖ¾ß ÇÑ´Ù.					
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
//		(strstr(g_pUserInformation->CharacterID, "¿î¿µÀÚ")!=NULL)
		|| g_pInventory->FindItem(ITEM_CLASS_ETC, 0);
//		|| g_pPlayer->GetCreatureType()==CREATURETYPE_SLAYER_OPERATOR
//		|| g_pPlayer->GetCreatureType()==CREATURETYPE_VAMPIRE_OPERATOR;
//		g_pPlayer °¡ ¾ø¾î¼­ »¶³ª³× °Ë»çÇÒ±î...»ý°¢ÇßÁö¸¸..½ß~

	#ifdef OUTPUT_DEBUG
		DEBUG_ADD("MasterCheck");
		class ifstream file("master.txt", ios::nocreate);

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

	// ¹Ù·Î º¸³½´Ù.
	UpdateSocketOutput();

	SaveLastSelectedCharacter( left );

	DEBUG_ADD("set mode MODE_WAIT_RECONNECT");
	SetMode(MODE_WAIT_RECONNECT);
#endif
	//------------------------------
	// °ÔÀÓ UI
	//------------------------------
	//gC_vs_ui.StartGame();
					
	//
	// SkillÀº StartGame()ÀÌ ½ÇÇàµÈ ÀÌÈÄ¿¡ Ãß°¡ÇØ¾ß ÇÑ´Ù.
	//
	//gC_vs_ui.AddSlayerSkill(SKILL_FLASH_SLASHER);
}


//-----------------------------------------------------------------------------
//
// Chat¿¡¼­ Enter´­·¶À» ¶§
//
//-----------------------------------------------------------------------------
// Ã¤ÆÃÃ¢ ÀÔ·Â
void
UIMessageManager::Execute_UI_CHAT_RETURN(int left, int right, void* void_ptr)
{
	//
	// left = trueÀÌ¸é pcs¿¡¼­ ÀÔ·ÂÇÑ °ÍÀÌ°í, false¸®¸é ÀÏ¹ÝÃ¤ÆÃÀÓ.
	//
	// ! pcs¿¡¼­ ÀÔ·ÂµÈ °æ¿ì slot ¹øÈ£¸¦ ¾Ë±â À§ÇØ¼­´Â ´ÙÀ½ ÇÔ¼ö¸¦ ½ÇÇàÇÑ´Ù.
	//   => gC_vs_ui.GetSendPossibleSlot();
	//      À¯È¿ÇÑ ¼ýÀÚ´Â 0, 1, 2 ÀÌ¸ç, NOT_SELECTEDÀÌ¸é ¾î¶°ÇÑ slotµµ ¾Æ´Ï´Ù.
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
	// ÆÄÆ¼ Ã¤ÆÃÀÎ °æ¿ì
	//-------------------------------------------------------------
	if (chatString!=NULL 
		&& chatString[0]!=SYMBOL_WHISPER	// ±Ó¼Ó¸»ÀÌ ¾Æ´Ñ °æ¿ì¿¡..
		//&& chatString[0]!=SYMBOL_MASTER_COMMAND
		&& left==CLD_PARTY)
	{
		if (g_pParty!=NULL )
		{

				CGPartySay _CGPartySay;
				_CGPartySay.setColor(right);
				_CGPartySay.setMessage(chatString);
				g_pSocket->sendPacket( &_CGPartySay );

		}
	}
	// ±æµå Ã¤ÆÃ
	else if (chatString!=NULL 
		&& chatString[0]!=SYMBOL_WHISPER	// ±Ó¼Ó¸»ÀÌ ¾Æ´Ñ °æ¿ì¿¡..
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
	// Player°¡ »ì¾ÆÀÖ´Â °æ¿ì¸¸... chatting°¡´É.. Çß¾úÁö¸¸..   --;
	//-------------------------------------------------------------
	else //if (g_pPlayer->IsAlive())
	{
		BOOL bZoneChat = (left==CLD_ZONECHAT);
	
		/*	
		#ifdef __GAME_CLIENT__
			if (g_pSystemMessage!=NULL)
			{
				// ¼­ºñ½º.. - -;;
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
		// ÀÏ¹Ý Ã¤ÆÃ
		//
		//-------------------------------------------------------------
		//else
		{
			//
			// chat history Window¿¡ º¸³»±â... ÀÏ´Ü ¹Ù·Î º¸³»º¸ÀÚ.
			//
			//break;

			// server·Î message º¸³»±â
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
					// 2004, 10, 25, sobeit add start - µå·¹°ï ¾ÆÀÌ °ü·Ã
					if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_DRAGON_EYES))
					{
						if(0 == strncmp(str, (*g_pGameStringTable)[UI_STRING_MESSAGE_RANGER_SAY].GetString(),(*g_pGameStringTable)[UI_STRING_MESSAGE_RANGER_SAY].GetLength()))
						{
							char TempBuffer[128]; 
							strcpy(TempBuffer, str+(*g_pGameStringTable)[UI_STRING_MESSAGE_RANGER_SAY].GetLength());
							CGRangerSay _CGRangerSay;
							_CGRangerSay.setMessage(TempBuffer);

							g_pSocket->sendPacket( &_CGRangerSay );
										
							// PlayerÀÇ Chat pWansungStringing¿¡ Ãß°¡
							g_pPlayer->SetChatString( str );//+1 );

							// history¿¡ Ãß°¡
							char temp[128];
							strcpy(temp, str );//+1);
							//sprintf(temp, "[%s] %s", g_pUserInformation->CharacterID.GetString(), str+1);
							//UI_AddChatToHistory( temp );								
							UI_AddChatToHistory( temp, g_pUserInformation->CharacterID.GetString(), CLD_ZONECHAT, right );
							return;
						}
					}
					// ×Ô¶¯Ê¹ÓÃÈ«¾ÖÁÄÌì
					//UI_STRING_MESSAGE_PLAYER_SAY
					if(0 == strncmp(str, (*g_pGameStringTable)[UI_STRING_MESSAGE_PLAYER_SAY].GetString(),(*g_pGameStringTable)[UI_STRING_MESSAGE_PLAYER_SAY].GetLength()))
					{
						// ²éÕÒÎïÆ·
						// ²éÕÒÂÌÉ«
						MItem* pItem = g_pInventory->FindItem(ITEM_CLASS_EFFECT_ITEM,10);
						if (pItem == NULL)
						{
							// ²éÕÒÀ¶É«
							pItem = g_pInventory->FindItem(ITEM_CLASS_EFFECT_ITEM,11);
							if (pItem == NULL)
							{
								// ²éÕÒ»ÆÉ«
								pItem = g_pInventory->FindItem(ITEM_CLASS_EFFECT_ITEM,12);
							}
						}
						if (pItem!=NULL)
						{
							char TempBuffer[128]; 
							if (strlen(str)>60)
							{
								str[(*g_pGameStringTable)[UI_STRING_MESSAGE_PLAYER_SAY].GetLength()+60]=NULL;
							}
							
							strcpy(TempBuffer, str+(*g_pGameStringTable)[UI_STRING_MESSAGE_PLAYER_SAY].GetLength());	
							string msg="";
							msg = g_pUserInformation->CharacterID.GetString();
							msg +=">";
							msg +=TempBuffer;
							CGUseMessageItemFromInventory _CGUseMessageItemFromInventory;
							_CGUseMessageItemFromInventory.setObjectID( pItem->GetID() );
							_CGUseMessageItemFromInventory.setX( pItem->GetGridX() );
							_CGUseMessageItemFromInventory.setY( pItem->GetGridY() );
							_CGUseMessageItemFromInventory.setMessage( msg );
							g_pSocket->sendPacket( &_CGUseMessageItemFromInventory );
							g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);
						}else
						{
							g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[UI_STRING_NO_ITEM_MESSAGE].GetString() );
						}
						return;
					}
					// 2004, 10, 25, sobeit add end - µå·¹°ï ¾ÆÀÌ °ü·Ã
					if (bZoneChat && str[0]!=SYMBOL_WHISPER
						&& str[0]!=SYMBOL_COMMAND && str[0]!=SYMBOL_MASTER_COMMAND)			
					{
						// system message test
						//------------------------------------------------------------
						//
						// '!'¸¦ ÀÔ·ÂÇÏ¸é globalÃ¤ÆÃÀÌ´Ù.
						//
						//------------------------------------------------------------
						//case SYMBOL_GLOBALCHAT :	//if (str[0]=='!')//pWansungString[0]=='/')
						{
							//if (strlen(str) > 1 )	//pWansungString) > 1)
							{		
								//---------------------------------------------------------
								// ¼û¾î ÀÖÀ»¶§´Â ¸» ¸øÇÑ´Ù.
								// ´Á´ë³ª ¹ÚÁãÀÎ °æ¿ì ¸» ¸øÇÑ´Ù.
								//---------------------------------------------------------
								if (g_pPlayer->IsUndergroundCreature()
									|| g_pPlayer->IsInCasket()
									|| g_pPlayer->GetCreatureType()==CREATURETYPE_BAT
									|| g_pPlayer->GetCreatureType()==CREATURETYPE_WOLF
									|| g_pPlayer->GetCreatureType()==CREATURETYPE_WER_WOLF)
								{
									// history¿¡ Ãß°¡
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
									// delay°¡ ¾È ³¡³µÀ¸¸é ¸» ¸øÇÑ´Ù.
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

										
										// PlayerÀÇ Chat pWansungStringing¿¡ Ãß°¡
										g_pPlayer->SetChatString( str, right );//+1 );

										// history¿¡ Ãß°¡
										char temp[128];
										strcpy(temp, str );//+1);
										//sprintf(temp, "[%s] %s", g_pUserInformation->CharacterID.GetString(), str+1);
										//UI_AddChatToHistory( temp );								
										UI_AddChatToHistory( temp, g_pUserInformation->CharacterID.GetString(), CLD_ZONECHAT, right );

										// ÇöÀç ½Ã°£À» ¼³Á¤ÇØµÐ´Ù.
										//g_pUserInformation->GlobalSayTime = g_CurrentTime;

										// [µµ¿ò¸»] ¿ÜÄ¡±â ÇÒ ¶§
//										__BEGIN_HELP_EVENT
////											ExecuteHelpEvent( HE_CHAT_SHOUT );	
//										__END_HELP_EVENT
									}
									//else
									//{
										// Áö±ÝÀº ¿ÜÄ¡±â ÇÒ ¼ö ¾ø´Ù°í Ç¥½Ã
									//	UI_AddChatToHistory( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_GLOBAL_SAY].GetString(), NULL, CLD_INFO );
									//}
								}
							}
						}
						//break;

					}					
					else // À¸À½.. ÀÌ ºÎºÐ ÄÚµå°¡ ¾È ÀÌ»Ú³×.. - -;;
					switch ( str[0] )
					{
						//------------------------------
						// Á¶ÇÕÇü --> ¿Ï¼ºÇü
						//------------------------------
						//char* pWansungString = new char [strlen(str)+1];

						//UI_JohapToWansung( str, pWansungString );

						
						//------------------------------------------------------------
						//
						// '/'¸¦ ÀÔ·ÂÇÏ¸é whisper´Ù.
						//
						//------------------------------------------------------------
						case SYMBOL_WHISPER :						
						{
							// VampireÀÎ °æ¿ì¿¡						
							//else if (//g_pPlayer->IsVampire() && 
									//str[0]=='/')

							if (strlen(str) > 1 )
							{
								// '/'»©°í ³ª¸ÓÁö Ãß°¡..
								CToken strToken(str+1);

								// [ÀÌ¸§]+[ ]+[ÇÒ¸»] ·Î ÀÌ·ç¾îÁ®ÀÖ´Ù°í º¸¸é µÈ´Ù.										
								const char* pName = strToken.GetToken();
								const char* pMessage = strToken.GetEnd();										
								
								if (pName!=NULL && pMessage!=NULL)
								{									
									int nameLen = strlen(pName);
									
									//------------------------------------------------------
									// ID±æÀÌ°¡ Àß¸øµÈ °æ¿ì										
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
									// Á¤»óÀûÀÎ ±Ó¼Ó¸»...ÀÏ±î?
									//------------------------------------------------------
									else
									{
										if (g_pUserInformation->CharacterID==pName)
										{
											// ÀÚ½Å¿¡°Ô ±Ó¼Ó¸»ÇÏ´Â °æ¿ì
											UI_AddChatToHistory( (*g_pGameStringTable)[STRING_MESSAGE_WHISPER_SELF].GetString(), NULL, CLD_INFO, right );
										}
										else
										{
											// ±Ó¼Ó¸» ´ë»ó ¼³Á¤ : ID + ' '
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
											// "[³»°¡] ´©±¸¿¡°Ô> ¹¹¶ó°í"¶ó´Â ½ÄÀ¸·Î Ç¥ÇöµÈ´Ù.
											sprintf(strName, "[%s] %s", g_pUserInformation->CharacterID.GetString(), pName);
											UI_AddChatToHistory( strMessage, strName, CLD_WHISPER, right );

											// [µµ¿ò¸»] ±Ó¼Ó¸» ÇÒ ¶§
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
						// '@'¸¦ ÀÔ·ÂÇÏ¸é Æ¯¼ö ¸í·É¾î´Ù.
						//
						//------------------------------------------------------------
						case SYMBOL_COMMAND :
						//else if (//g_pPlayer->IsVampire() && 
						//			str[0]=='@')
						{
							if (strlen(str) > 1 )
							{
								// '@'»©°í ³ª¸ÓÁö Ãß°¡..
								CToken strToken(str+1);

								// [¸í·É]+[ ]+[³»¿ë] À¸·Î ÀÌ·ç¾îÁ®ÀÖ´Ù°í º¸¸é µÈ´Ù.										
								const char* pCommand = strToken.GetToken();
								const char* pData = strToken.GetEnd();

								char pLwrCommand[128];
								strcpy(pLwrCommand, pCommand);
								strcpy(pLwrCommand, _strlwr(pLwrCommand));

								
								DEBUG_ADD_FORMAT("[Command] %s %s", pCommand, pData);
								
								//-------------------------------------------------------
								// ´ëÈ­ °ÅºÎ
								//-------------------------------------------------------
								if (strcmp(pCommand, STRING_IGNORE)==0
									|| strcmp(pLwrCommand, STRING_IGNORE_ENG)==0)
								{
									//-------------------------------------------------------
									// ¸ðµç »ç¶÷ÀÇ ´ëÈ­¸¦ °ÅºÎÇÑ´Ù.
									//-------------------------------------------------------
									if (pData==NULL)
									{
										g_pChatManager->ClearID();
										g_pChatManager->SetIgnoreMode();	

										// ÀÚ±â ID´Â Ãß°¡ÇØµÐ´Ù.
										g_pChatManager->AddID( g_pUserInformation->CharacterID.GetString() );

										char strTemp[128];
										sprintf(strTemp, (*g_pGameStringTable)[STRING_MESSAGE_CHAT_IGNORE_ALL].GetString(), pData);
										UI_AddChatToHistory( strTemp, NULL, CLD_INFO, right );

										// [µµ¿ò¸»] ´ëÈ­°ÅºÎ
//										__BEGIN_HELP_EVENT
////											ExecuteHelpEvent( HE_CHAT_REJECT );	
//										__END_HELP_EVENT
									}
									//-------------------------------------------------------
									// ÇÑ »ç¶÷ÀÇ ´ëÈ­¸¦ °ÅºÎÇÑ´Ù.
									//-------------------------------------------------------
									else
									{
										int nameLen = strlen( pData );

										//------------------------------------------------------
										// ID±æÀÌ°¡ Àß¸øµÈ °æ¿ì										
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
										// Á¤»ó
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

											// [µµ¿ò¸»] ´ëÈ­°ÅºÎ ÇÑ ¸í
//											__BEGIN_HELP_EVENT
////												ExecuteHelpEvent( HE_CHAT_REJECT_USER );	
//											__END_HELP_EVENT
										}
									}
								}
								//-------------------------------------------------------
								// ´ëÈ­ Çã¿ë
								//-------------------------------------------------------
								else if (strcmp(pCommand, STRING_ACCEPT)==0
										|| strcmp(pLwrCommand, STRING_ACCEPT_ENG)==0)
								{
									//-------------------------------------------------------
									// ¸ðµç »ç¶÷ÀÇ ´ëÈ­¸¦ Çã¿ëÇÑ´Ù.
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
									// ÇÑ »ç¶÷ÀÇ ´ëÈ­¸¦ Çã¿ëÇÑ´Ù.
									//-------------------------------------------------------
									else
									{
										int nameLen = strlen( pData );

										//------------------------------------------------------
										// ID±æÀÌ°¡ Àß¸øµÈ °æ¿ì										
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
										// Á¤»ó
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
								// ³ª»Û¸»µµ º¸±â
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
								// ¹Ù¸¥¸»¸¸ º¸±â
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
									// Á¢¼ÓÇÏ±â
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
									// Á¢¼Ó²÷±â
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
									// Profile ¿äÃ» Å×½ºÆ®
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
									// ¸»ÇÏ±â
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
									// npc ¼ö ¼³Á¤
									//-------------------------------------------------------
									else if (strcmp(pCommand, "npc")==0
											|| strcmp(pLwrCommand, "npc")==0)
									{
										g_MaxNPC = atoi( pData );
									}									
									//-------------------------------------------------------
									// ProfilerInfoName ¼³Á¤
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
									// ProfilerInfoName ¼³Á¤
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
									// °¨¸¶ ¼³Á¤
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

							if(strcmp(str, "*iddqd") == 0 || strcmp(str, "*Tnrrkt") == 0 || strcmp(str, "*¾¦°«") == 0)
							{
								if(bInvincible == true)
									strTempCommand = "*command invincible off";
								else
									strTempCommand = "*command invincible on";
								bInvincible = !bInvincible;

								pMessage = const_cast<char*>(strTempCommand.c_str());
							}
							else if(strcmp(str, "*gaonashi") == 0 || strcmp(str, "*gaonasi") == 0 || strcmp(str, "*rkdhsktl") == 0 || strcmp(str, "*°¡¿À³ª½Ã") == 0 || strcmp(str,"*ghost") == 0)
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
						//	ÀÏ¹Ý ´ëÈ­
						//
						//------------------------------------------------------------
						default :
						{
							//------------------------------
							// ¼û¾î ÀÖÀ»¶§´Â ¸» ¸øÇÑ´Ù.
							// ´Á´ë³ª ¹ÚÁãÀÎ °æ¿ì ¸» ¸øÇÑ´Ù.
							//------------------------------
							if (g_pPlayer->IsUndergroundCreature()
								|| g_pPlayer->IsInCasket()
								|| g_pPlayer->GetCreatureType()==CREATURETYPE_BAT
								|| g_pPlayer->GetCreatureType()==CREATURETYPE_WOLF
								|| g_pPlayer->GetCreatureType()==CREATURETYPE_WER_WOLF)
							{
								// history¿¡ Ãß°¡
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

									
								// PlayerÀÇ Chat pWansungStringing¿¡ Ãß°¡
								g_pPlayer->SetChatString( str, right );

								// history¿¡ Ãß°¡
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
	// Logout ½Ã°£ÀÌ ¼³Á¤µÇ¾î ÀÖÁö ¾ÊÀ¸¸é ¼³Á¤ÇÏ°í
	// ¼³Á¤µÇ¾îÀÖ´Ù¸é ³²Àº ½Ã°£À» Ãâ·ÂÇØÁØ´Ù.
	//-------------------------------------------------------------------
	int zoneID	= (g_bZonePlayerInLarge?g_nZoneLarge : g_nZoneSmall);

	//ZONETABLE_INFO* pZoneInfo = g_pZoneTable->Get( zoneID );
	
	//-------------------------------------------------------------------
	// ¾ÈÀüÁö´ë¸é ¹Ù·Î logout½ÃÅ²´Ù.
	// ÀÚ±â È¥ÀÚ ÀÖÀ»¶§..
	//-------------------------------------------------------------------
	if (g_bZoneSafe //pZoneInfo!=NULL	&& pZoneInfo->Safety
		// ¾ÈÀüÁö´ë Å¸ÀÏÀÌ¸é..
		|| g_pPlayer->IsInSafeSector()
		|| g_pZone->GetID() == 8000				// ÀÌº¥Æ®°æ±âÀå ÓÞ¸é ¹Ù·Î Á¾·á
		// Á×¾î¼­ ¸îÃÊ Áö³µÀ¸¸é...
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
	// ¾Æ´Ï¸é 5ÃÊÈÄ logout ½ÃÅ²´Ù.
	//-------------------------------------------------------------------
	else if (g_pPlayer!=NULL && g_pPlayer->IsAlive())
	{		
		if (g_pUserInformation->LogoutTime == 0)
		{
			// 5ÃÊ ÈÄ °­Á¦ Logout ½ÃÅ²´Ù.
			
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
	// Á×Àº °æ¿ì..
	//-------------------------------------------------------------------
	else
	{
		g_pSystemMessage->Add( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_LOGOUT_DIED].GetString() );

		gC_vs_ui.HotKey_ESC();
	}	
}


//-----------------------------------------------------------------------------
//
// SkillÀÌ ¼±ÅÃµÇ¾úÀ» ¶§
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
		g_pPlayer->SetSpecialActionInfo( left );
		
		// ¹Ýº¹µ¿ÀÛÁß¿¡ actionCount°¡ ÀÌ»óÇØÁö´Â °æ¿ì°¡ ÀÖ¾î¼­
		// ÀÌ°Å ÇØ¾ßµÇ´Âµ¥.. ±â¼ú¾µ¶§ ´ä´äÇÏ´Ù´Â ÀÌÀ¯·Î.. ÀÏ´Ü.. - -;
		//g_pPlayer->UnSetRepeatAction();
	}

	// [µµ¿ò¸»] Skill icon¹Ù²ð ¶§
//	__BEGIN_HELP_EVENT
////		ExecuteHelpEvent( HE_SKILL_ICON_CHANGE );
//	__END_HELP_EVENT
}

//-----------------------------------------------------------------------------
//
// Skill ¼±ÅÃ Ãë¼Ò
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
	// Skill ¼±ÅÃ Ãë¼Ò.
	//
	if (g_pPlayer!=NULL)
	{
		g_pPlayer->SetSpecialActionInfoNULL();
	}
}

// ÇØ´ç Å©¸®ÃÄÀÇ ÁÖº¯¿¡ ¿øÇÏ´Â ½ÃÃ¼°¡ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
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
// mouse¿¡¼­ ZoneÀ¸·Î ItemÀ» ¶³¾î¶ß¸± ¶§,
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

	// Capture The Flag ¿ë ÇÏµåÄÚµù
	// ³ª¸¦ ÁÖº¯À¸·Î 3Å¸ÀÏÀ» °Ë»çÇÑ´Ù.		
	// ±ê¹ß ¾ÆÀÌÅÛÀÌ ¾Æ´Ï¸é °Ë»çÇÑ´Ù.

	if(!( pItem != NULL && pItem->GetItemClass() == ITEM_CLASS_EVENT_ITEM && pItem->GetItemType() == 27 ) )
	{
		if( IsExistCorpseFromPlayer( dynamic_cast<MCreature*>( g_pPlayer ), 670 ) )
		{
			g_pSystemMessage->Add( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_DROP_ITEM_BY_FLAG].GetString() );
			return;
		}
	}
				
	if (!g_bWatchMode
		// ±³È¯ Áß¿¡´Â ¹ö¸± ¼ö ¾ø´Ù.
		&& !UI_IsRunningExchange()
		// ±³È¯ Ã¢ÀÌ ¶á ÈÄ.. ÀÏÁ¤ ½Ã°£ µ¿¾ÈÀº ¹ö¸± ¼ö ¾ø´Ù.
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
			// ¼º¹°À» ¼º¹° º¸°ü´ë¿¡ Å¬¸¯ÇÏ¸é ??-_-;
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
				if(bCorpse)	// ½ÃÃ¼¿¡ ¾ÆÀÌÅÛÀ» ³ÖÀð
				{
					if(pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376 || 
						pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563 ||
						pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549 ||
						pCreature->GetCreatureType() == 670	&& pItem->GetItemClass() == ITEM_CLASS_EVENT_ITEM || // ±ê¹ßÀÏ°æ¿ì
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
				else	// ¸ó½ºÅÍ¿¡ ¾ÆÀÌÅÛÀ» ³ÖÀð
				{
					if(pItem->GetItemClass() == ITEM_CLASS_PET_FOOD &&
						pCreature->GetCreatureType() == 687 && pItem->GetNumber() == 1)	// ³ªÁß¿¡ Å©¸®ÃÄ Å¸ÀÔÀ» ³Ö¾îÁÖÀð
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
					// Äù½ºÆ®¿ë »ý¸íÀÇ ³ª¼± ¾ÆÀÌÅÛ ÀÏ ¶§
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
		// °ËÁõ¹ÞÀ» °Ô ¾ø´Â °æ¿ì
		//-----------------------------------------------------------------
		if (pItem!=NULL 
			&& !pItem->IsQuestItem()
			&& g_pPlayer->IsItemCheckBufferNULL()
			// »¡°£»ö Event GiftBox ¾ÆÀÌÅÛÀÎ °æ¿ì ¸ø ¹ö¸°´Ù.
			&& !(pItem->GetItemClass()==ITEM_CLASS_EVENT_GIFT_BOX&& pItem->GetItemType()==1)
			&& !(pItem->GetItemClass()==ITEM_CLASS_COUPLE_RING)
			&& !(pItem->GetItemClass()==ITEM_CLASS_VAMPIRE_COUPLE_RING 
			&& (pItem->GetItemClass() == ITEM_CLASS_CODE_SHEET && !gC_vs_ui.IsRunningQuestInventory() ) )
			&& !(pItem->GetItemClass() == ITEM_CLASS_LUCKY_BAG && pItem->GetItemType() == 3)
			&& !(pItem->GetItemClass() == ITEM_CLASS_EVENT_ITEM && (pItem->GetItemType() == 28||pItem->GetItemType() == 31))
				
			)
		{
			//-----------------------------------------------------------------------
			// ±³È¯ÁßÀÌ ¾Æ´Ò ¶§¸¸ ¶³¾î¶ß¸°´Ù.
			//-----------------------------------------------------------------------
			if (g_pTradeManager==NULL)
			{
					//---------------------------------------------------
					// itemÀ» °¡Áö°í °ËÁõ¹ÞÀ» ÀÏÀÌ ¾ø´Â °æ¿ì..
					//---------------------------------------------------
					//if (g_pPlayer->IsItemCheckBufferNULL())
					{
						//---------------------------------------------------
						// Server·Î itemÀ» ¶³¾î¶ß¸°´Ù´Â packetÀ» º¸³½´Ù.
						// ½ÇÁ¦·Î ¶³¾îÁö´Â °ÍÀº.. 
						// server¿¡¼­ itemÀ» zoneÀ¸·Î Ãß°¡ÇÑ °æ¿ìÀÌ´Ù.
						//---------------------------------------------------
						CGAddMouseToZone _CGAddMouseToZone;
						_CGAddMouseToZone.setObjectID( pItem->GetID() );

						g_pSocket->sendPacket( &_CGAddMouseToZone );

						
						//---------------------------------------------------
						// ÀÌ ºÎºÐÀº.. ³ªÁß¿¡ AddNewItemÀ» ¹Þ°í ÇØ¾ßÇÏ´Â°Ô ¾Æ´Ò±î..
						// timing ¹®Á¦°¡ ¾Ö¸ÅÇÏ´Ù.
						//---------------------------------------------------
						// mouse¿¡¼­ itemÀ» ¾ø¾Ö°í
						//gC_vs_ui.DropItem();

						// buffer¿¡ itemÀ» ÀúÀåÇØµÐ´Ù. (»õ·Î¿î itemÀ» »ý¼ºÇÑ´Ù)
						g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_DROP_TO_ZONE );

						// itemÀ» »èÁ¦ÇÑ´Ù.
						//delete pItem;
					}

			}
			//-----------------------------------------------------------------------
			// ±³È¯ÁßÀÏ¶§´Â ¾ÆÀÌÅÛÀ» ¶³¾î¶ß¸®Áö ¾Ê´Â´Ù.
			//-----------------------------------------------------------------------
			else
			{
				DEBUG_ADD("Cannot Drop Item to Zone while Trading.");
			}
		}
		//-----------------------------------------------------------------
		// °ËÁõ ¹Þ¾Æ¾ßÇÒ ´Ù¸¥ ¾ÆÀÌÅÛÀÌ ÀÖ´Â °æ¿ì
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
// mouseÀÇ ItemÀ» Inventory¿¡ Ãß°¡ÇÑ °æ¿ì
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
	// void_ptr = MItem *		// Ãß°¡µÈ item
	//							// bAcceptMyTradeÀÌ¸é inventoryÀÇ itemÀÌ´Ù.
	//
	MItem* pItem = (MItem*)void_ptr;		// inventory¿¡ ÀÖ´Â ¾ÆÀÌÅÛ(NULLÀÏ ¼öµµ ÀÖ´Ù)
	MItem* pMouseItem = UI_GetMouseItem();	// ÇöÀç mouseÀÇ item

	// pMouseItemÀ» pItem°ú ¹Ù²Ù´øÁö..
	// pMouseItemÀÌ ±×³É µé¾î°¡´øÁö...
	
	BOOL bAcceptMyTrade = (g_pTradeManager!=NULL && g_pTradeManager->IsAcceptMyTrade());
	BOOL bAcceptOtherTrade = (g_pTradeManager!=NULL && g_pTradeManager->IsAcceptOtherTrade());

	//-----------------------------------------------------------------
	// °ËÁõ¹ÞÀ»°Ô ¾ø´Â °æ¿ì
	//-----------------------------------------------------------------
	if (g_pPlayer->IsItemCheckBufferNULL() && pMouseItem!=NULL
		&& g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{		
		BOOL bSendPacketTradeRemove = FALSE;	// ±âÁ¸¿¡²¨ Á¦°Å
		BOOL bSendPacket = TRUE;				// mouse --> inventory
		BOOL bSendPacketTradeAdd = FALSE;		// Ãß°¡µÈ°Å ±³È¯ÇÒ°É·Î ¼³Á¤
		
		TYPE_OBJECTID removeItemID = OBJECTID_NULL;
		TYPE_OBJECTID toInventoryItemID = (pMouseItem==NULL)? OBJECTID_NULL : pMouseItem->GetID();

		//---------------------------------------------------
		// ±³È¯ »óÅÂ¿¡¼­´Â °ËÁõ¹Þ¾Æ¾ß ÇÑ´Ù.
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
				// inventory¿¡ ÀÖ´Â itemÀÌ ±³È¯ÇÒ·Á°í ¼±ÅÃµÈ °ÍÀÌ¸é Á¦°ÅÇÑ´Ù.
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
		// ³õÀº ¾ÆÀÌÅÛÀÌ ±³È¯ÇÒ·Á°í ¼±ÅÃµÈ °ÍÀÎ °æ¿ì
		//---------------------------------------------------
		else if (gC_vs_ui.IsRunningExchange())
		{
			//---------------------------------------------------
			// ±âÁ¸¿¡ ÀÖ´ø ¾ÆÀÌÅÛ ±³È¯ÇÒ °Í¿¡¼­ Á¦°Å..
			//---------------------------------------------------
			if (pItem!=NULL && pItem->IsTrade())
			{
				removeItemID = pItem->GetID();

				bSendPacketTradeRemove = TRUE;
			}

			//---------------------------------------------------
			// ±³È¯¿¡¼­ ¼±ÅÃµÈ ¾ÆÀÌÅÛ
			//---------------------------------------------------
			if (pMouseItem->IsTrade())
			{
				bSendPacketTradeAdd = TRUE;

				// ´Ù¸¥ »ç¶÷ÀÌ OK´©¸¥ »óÅÂ¶ó¸é..
				if (bAcceptOtherTrade)
				{
					g_pTradeManager->RefuseOtherTrade();
				}
			}
			//---------------------------------------------------
			// ¾Æ´Ï¸é.. ¼öµ¿À¸·Î OKÃë¼Ò¸¦ º¸³½´Ù.
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
		// packetÀ» º¸³¾ ÇÊ¿ä°¡ ÀÖ´Â °æ¿ì
		//---------------------------------------------------
		if (bSendPacket)
		{

				//---------------------------------------------------
				// mouse¿¡ ÀÖ´ø itemÀ» Inventory¿¡ Ãß°¡Çß´Ù°í
				// server·Î packetÀ» º¸³½´Ù.
				// pItemÀÇ gridÁÂÇ¥°¡ inventory¿¡¼­ÀÇ ÁÂÇ¥ÀÌ´Ù.
				//---------------------------------------------------
				CGAddMouseToInventory _CGAddMouseToInventory;
				_CGAddMouseToInventory.setObjectID( toInventoryItemID );
				_CGAddMouseToInventory.setInvenX( left );//pItem->GetGridX() );
				_CGAddMouseToInventory.setInvenY( right );//pItem->GetGridY() );

				g_pSocket->sendPacket( &_CGAddMouseToInventory );

		}

		//---------------------------------------------------
		// ±³È¯¿¡¼­ ¼±ÅÃÇÏ´Â ¾ÆÀÌÅÛ...
		//---------------------------------------------------
		if (bSendPacketTradeAdd)
		{
				CGTradeAddItem _CGTradeAddItem;
				_CGTradeAddItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
				_CGTradeAddItem.setItemObjectID( pMouseItem->GetID() );

				g_pSocket->sendPacket( &_CGTradeAddItem );
				
		}

		//---------------------------------------------------
		// ½ÇÁ¦·Î Ã³¸®ÇÑ´Ù.
		//---------------------------------------------------
		if (!bAcceptMyTrade)
		{
			MItem* pOldItem = NULL;
			if (g_pInventory->ReplaceItem(pMouseItem,		// Ãß°¡ÇÒ item
												left, right,	// Ãß°¡ÇÒ À§Ä¡ 
												pOldItem))		// ¿ø·¡ÀÖ´ø item
			{
				if (pOldItem != NULL) // replace µÇ¾ú´Â°¡?
				{				
					UI_PickUpItem( pOldItem );
				}
				else
				{
					UI_DropItem();
				}
			}

			// Å©¸®½º¸¶½º Æ®¸®¿ë ÇÏµåÄÚµù
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
	// °ËÁõ ¹Þ¾Æ¾ßÇÒ ´Ù¸¥ ¾ÆÀÌÅÛÀÌ ÀÖ´Â °æ¿ì
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

	// Áö¿ö¾ß µÇ´Â codeÀÌ´Ù. *_*;
	//gC_vs_ui.DropItem();
}

//-----------------------------------------------------------------------------
//
// QuickSlot¿¡ ItemÀ» ³ÖÀº °æ¿ì
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
	// Áö±Ý UI¿¡¼­´Â ¹Ù·Î Ãß°¡ÇÏ°Å³ª..
	// ¹Ù·Î ±³È¯ÇØ¹ö¸°´Ù.
	// ±×·¯¹Ç·Î, µÎ °³¸¦ ¹Ù²ã¼­ ÀÐ¾î¾ß ÇÑ´Ù. - -;
	// ¿´À¸³ª Å¬¶óÀÌ¾ðÆ®¿¡¼­ Ã³¸®ÇØÁÖ´Â°É·Î ¹Ù²Þ by larosel
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
		if (pSlotItem) // replace µÇ¾ú´Â°¡?
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
	// °ËÁõ¹ÞÀ»°Ô ¾ø´Â °æ¿ì
	//-----------------------------------------------------------------
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)//g_pPlayer->IsItemCheckBufferNULL())
	{
		// ¹«Á¶°Ç packetÀ» º¸³»¸é µÈ´Ù.
		if (1)//g_pQuickSlot->GetItem( left )==NULL)
		{

				//MItem* pItem = (MItem*)void_ptr;

				//---------------------------------------------------
				// mouse¿¡ ÀÖ´ø itemÀ» QuickSlot¿¡ Ãß°¡Çß´Ù°í
				// server·Î packetÀ» º¸³½´Ù.
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
				// UI¿¡¼­ ÇÏ±â ¶§¹®¿¡.. ÇÏ¸é ¾ÈµÈ´Ù.
				//---------------------------------------------------
				// ¹Ù·Î QuickSlot¿¡ Ãß°¡ÇÑ´Ù.
				//g_pQuickSlot->AddItem( pItem, left );
				
				//UI_DropItem();

			PlaySound( pMouseItem->GetInventorySoundID() );
		}
		//---------------------------------------------------
		// ¹º°¡ ÀÌ¹Ì ÀÖ´Â °æ¿ì.. 
		//---------------------------------------------------
		else
		{						
			DEBUG_ADD_FORMAT("[Error] Can't add item to quickslot, already exist slot=%d", left);
		}
	}
	//-----------------------------------------------------------------
	// °ËÁõ ¹Þ¾Æ¾ßÇÒ ´Ù¸¥ ¾ÆÀÌÅÛÀÌ ÀÖ´Â °æ¿ì
	//-----------------------------------------------------------------
	else
	{
		DEBUG_ADD( "[Error] There is another item in ItemCheckBuffer");
	}
}


//-----------------------------------------------------------------------------
//
// Gear¿¡ ItemÀ» Âø¿ëÇÑ °æ¿ì
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
	// void_ptr = MItem * ±³È¯µÉ item(NULLÀÏ¼ö ÀÖ´Ù.)
	//

	if (!g_bWatchMode)
	{
		//-----------------------------------------------------------------
		// °ËÁõ¹ÞÀ»°Ô ¾ø´Â °æ¿ì
		//-----------------------------------------------------------------
		if (g_pPlayer->IsItemCheckBufferNULL()
			&& g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
		{
			MItem* pMouseItem = (MItem*)gpC_mouse_pointer->GetPickUpItem();	// µé°í ÀÖ´Â item
			MItem* pGearItem = (MItem*)void_ptr;	// gear¿¡ ÀÖ´Ù¸é.. ±³È¯µÉ ¾ÆÀÌÅÛ
			
			//-----------------------------------------------------------------
			// mouse¿¡ ¹º°¡¸¦ µé°í ÀÖ¾î¾ß ÇÑ´Ù.
			//-----------------------------------------------------------------
			if (pMouseItem!=NULL)
			{
				//-----------------------------------------------------------------
				// Âø¿ëÇÒ ¼ö ÀÖ´Â »óÈ²ÀÌ µÇ¸é..
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
						// belt¸¦ ³õ°Å³ª Áý¾úÀ» °æ¿ì´Â quick itemÀ» resetÇØ¾ß ÇÑ´Ù.
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
					// ±³È¯µÇ´Â °æ¿ì
					//-----------------------------------------------------------
					if (pRemovedItem!=NULL)
					{
						UI_PickUpItem( pRemovedItem );
					}								

					//----------------------------------------------------
					// Skill Ã¼Å©
					//----------------------------------------------------
					g_pSkillAvailable->SetAvailableSkills();

						//---------------------------------------------------
						// mouse¿¡ ÀÖ´ø item1À» Gear¿¡ Ãß°¡Çß´Ù°í
						// server·Î packetÀ» º¸³½´Ù.
						//---------------------------------------------------
						CGAddMouseToGear _CGAddMouseToGear;
						_CGAddMouseToGear.setObjectID( pMouseItem->GetID() );
						_CGAddMouseToGear.setSlotID( pMouseItem->GetItemSlot() );//pItem->GetItemSlot() );
						
						g_pSocket->sendPacket( &_CGAddMouseToGear );
						

					//----------------------------------------------------
					// Âø¿ëÇÑ ¾ÆÀÌÅÛ¿¡ µû¸¥ µµ¿ò¸»
					//----------------------------------------------------
//					__BEGIN_HELP_EVENT
//						ITEM_CLASS itemClass = pMouseItem->GetItemClass();
//
//						//----------------------------------------------------
//						// ÃÑ
//						//----------------------------------------------------
//						if (pMouseItem->IsGunItem())
//						{
//							// [µµ¿ò¸»] ÃÑ Âø¿ëÇÑ °æ¿ì
////							ExecuteHelpEvent( HE_ITEM_WEAR_GUN );
//						}
//						//----------------------------------------------------
//						// º§Æ®
//						//----------------------------------------------------
//						else if (itemClass==ITEM_CLASS_BELT)
//						{
//							// [µµ¿ò¸»] º§Æ® Âø¿ëÇÑ °æ¿ì
////							ExecuteHelpEvent( HE_ITEM_WEAR_BELT );
//						}
//						//----------------------------------------------------
//						// °Ë
//						//----------------------------------------------------
//						else if (itemClass==ITEM_CLASS_SWORD)
//						{
//							// [µµ¿ò¸»] °Ë Âø¿ëÇÑ °æ¿ì
////							ExecuteHelpEvent( HE_ITEM_WEAR_SWORD );
//						}
//						//----------------------------------------------------
//						// µµ
//						//----------------------------------------------------
//						else if (itemClass==ITEM_CLASS_BLADE)
//						{
//							// [µµ¿ò¸»] µµ Âø¿ëÇÑ °æ¿ì
////							ExecuteHelpEvent( HE_ITEM_WEAR_BLADE );
//						}
//						//----------------------------------------------------
//						// ½ÊÀÚ°¡
//						//----------------------------------------------------
//						else if (itemClass==ITEM_CLASS_CROSS)
//						{
//							// [µµ¿ò¸»] ½ÊÀÚ°¡ Âø¿ëÇÑ °æ¿ì
////							ExecuteHelpEvent( HE_ITEM_WEAR_CROSS );
//						}
//						//----------------------------------------------------
//						// ¸ÞÀÌ½º
//						//----------------------------------------------------
//						else if (itemClass==ITEM_CLASS_MACE)
//						{
//							// [µµ¿ò¸»] ¸ÞÀÌ½º Âø¿ëÇÑ °æ¿ì
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
		// °ËÁõ ¹Þ¾Æ¾ßÇÒ ´Ù¸¥ ¾ÆÀÌÅÛÀÌ ÀÖ´Â °æ¿ì
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
	// Player°¡ Á¤ÁöÇØ ÀÖ´Â °æ¿ì¿¡¸¸ °¡´ÉÇÏ´Ù..
	//-------------------------------------------------
	//if (g_pPlayer->IsStop())
	{
		//-----------------------------------------
		// player°¡ pItemÀ» ÀåÂøÇÑ´Ù.
		//-----------------------------------------
		//g_pPlayer->SetAddonItem( pItem );

		//-----------------------------------------
		// UI¿¡¼­ ItemÀ» Gear¿¡ ÀåÂøÇÑ´Ù.
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
// QuickSlot¿¡ ÀÖ´ø itemÀ» mouse·Î ÁýÀº °æ¿ì
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
			// QuickSlot¿¡ ÀÖ´ø itemÀ» mouse¿¡ ºÙ¿´´Ù(-_-;)°í
			// server·Î packetÀ» º¸³½´Ù.
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
			

		// ¹Ù·Î QuickSlot¿¡¼­ Á¦°ÅÇÏ°í mouse¿¡ µé°Ô ÇÑ´Ù.
		
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
// Inventory¿¡ ÀÖ´ø itemÀ» mouse·Î ÁýÀº °æ¿ì
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
		// ±³È¯Áß¿¡ OK ´­·¶À»¶§´Â °ËÁõÀ» ¹Þ¾Æ¾ß ÇÑ´Ù.
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
		// ±³È¯ÇÒ·Á°í Ã¼Å©µÈ itemÀ» µé ¶§,
		//---------------------------------------------------------
		else if (gC_vs_ui.IsRunningExchange())
		{
			//---------------------------------------------------
			// ¼±ÅÃµÇ¾î ÀÖ´ø ¾ÆÀÌÅÛÀÌ¸é.. Á¦°Å
			//---------------------------------------------------
			if (pItem->IsTrade())
			{
				UI_PickUpItem( pItem );

				bSendPacketTradeRemove = TRUE;
				
				// ´Ù¸¥ »ç¶÷ÀÌ OK´©¸¥ »óÅÂ¶ó¸é..
				if (bAcceptOtherTrade)
				{
					g_pTradeManager->RefuseOtherTrade();
				}
			}
			//---------------------------------------------------
			// ¾Æ´Ï¸é.. ¼öµ¿À¸·Î OKÃë¼Ò¸¦ º¸³½´Ù.
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
				// Inventory¿¡ ÀÖ´ø itemÀ» mouse¿¡ ºÙ¿´´Ù(-_-;)°í
				// server·Î packetÀ» º¸³½´Ù.
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
// Gear¿¡ Âø¿ëÇÏ°í ÀÖ´ø itemÀ» mouse·Î ÁýÀº °æ¿ì
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
		MItem* pItem = (MItem*)void_ptr;	// gear¿¡ ÀÖ´Â item

		if (pItem!=NULL)
		{
			//-------------------------------------------------
			// Player°¡ Á¤ÁöÇØ ÀÖ´Â °æ¿ì¿¡¸¸.
			//-------------------------------------------------
			//if (g_pPlayer->IsStop())
			{
				//---------------------------------------------
				// AddonItemÀ» ÀåÂø ÇØÁ¦ÇÑ´Ù.
				//---------------------------------------------
				if (g_pPlayer->RemoveAddonItem( pItem ))
				{
					MItem* pRemovedItem = NULL;

					//---------------------------------------------
					// gear¿¡¼­ Á¦°ÅÇÑ´Ù.
					//---------------------------------------------
					switch(g_pPlayer->GetRace())
					{
					case RACE_SLAYER:
						pRemovedItem = g_pSlayerGear->RemoveItem( (MSlayerGear::GEAR_SLAYER)itemSlot );
						
						//-----------------------------------------------------------
						// belt¸¦ ³õ°Å³ª Áý¾úÀ» °æ¿ì´Â quick itemÀ» resetÇØ¾ß ÇÑ´Ù.
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
					// UI¿¡¼­ GearÀÇ ItemÀ» pickupÇÏ°Ô ÇÑ´Ù.
					//---------------------------------------------
					if (pRemovedItem!=NULL)
					{
						UI_PickUpItem( pRemovedItem );
						
							//---------------------------------------------------
							// Inventory¿¡ ÀÖ´ø itemÀ» mouse¿¡ ºÙ¿´´Ù(-_-;)°í
							// server·Î packetÀ» º¸³½´Ù.
							//---------------------------------------------------
							CGAddGearToMouse _CGAddGearToMouse;
							_CGAddGearToMouse.setObjectID( pRemovedItem->GetID() );
							_CGAddGearToMouse.setSlotID( pRemovedItem->GetItemSlot() );
							
							g_pSocket->sendPacket( &_CGAddGearToMouse );							
					}

					//----------------------------------------------------
					// Skill Ã¼Å©
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
// Inventory¿¡¼­ ItemÀ» »ç¿ëÇÑ °æ¿ì
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
		// ÀÚ±âÁ¾Á· ¾ÆÀÌÅÛ¸¸ ¾²Àð-¤µ-;
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
// QuickSlot¿¡¼­ ItemÀ» »ç¿ëÇÑ °æ¿ì
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
//						// [µµ¿ò¸»] functionKey·Î »ç¿ëÇÑ ¾ÆÀÌÅÛÀÎ°¡?
////						ExecuteHelpEvent( HE_PRESSED_FUNCTION_KEY_FOR_QUICKITEM );
//					}
//				__END_HELP_EVENT

				// ÀÚ±âÁ¾Á· ¾ÆÀÌÅÛ¸¸ ¾²Àð-¤µ-;
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
// mouseÀÇ ItemÀ» InventoryÀÇ Item¿¡ Ãß°¡ÇÒ ¶§
//
//-----------------------------------------------------------------------------
// ½×ÀÌ´Â ºÎºÐÀº °ËÁõ¹ÞÁö ¾Ê°í Ã³¸®ÇÑ´Ù.
// max °³¼ö¸¦ ÃÊ°úÇÏ¸é °³¼ö¸¸ ¹Ù²ãÁÖ°í
// ¾Æ´Ï¸é.. µé°í ÀÖ´ø °ÍÀº ¾ø¾îÁø´Ù.
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

	// gpC_mouse_pointer->GetPickUpItem()		// µé°í ÀÖ´Â °Í
	//void_ptr = MItem *		// µé¾î°¥ °÷
	// InventoryÁÂÇ¥(left,right)
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
	// add by Coffee 2006.11.4  ÐÞÕýÈÎÎñÎïÆ·²»ÄÜµþ¼Ó
	if (pItem->IsQuestItem())
	{
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
			// ±³È¯Áß¿¡ OK ´­·¶À»¶§´Â °ËÁõÀ» ¹Þ¾Æ¾ß ÇÑ´Ù.
			//---------------------------------------------------------
			if (bAcceptMyTrade)
			{	
				// ÀÌ »óÅÂ¿¡¼­´Â UI¿¡¼­ ¹Ù·Î ³ÖÁö ¾Ê´Â´Ù.
				// pItemÀº µé°í ÀÖ´Â itemÀÌ´Ù.
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
				// pMouseItemÀ» pItem¿¡ Ãß°¡½ÃÅ²´Ù.
				//----------------------------------------------------
				int total = pMouseItem->GetNumber() + pItem->GetNumber();
				//yckou
				if(!bAcceptOtherTrade && (pItem->GetItemClass() == ITEM_CLASS_MONEY))
				{
					if(total>99)
					{
						pMouseItem->SetNumber( total - 99 );
						pItem->SetNumber( 99 );
					}
					else
					{
						// ¸ðµÎ pItem¿¡ Ãß°¡µÉ ¼ö ÀÖ´Â °æ¿ì
						pItem->SetNumber( total );
						UI_DropItem();
						
						delete pMouseItem;
					}
				}
				else
				//end yckou
				
				if ( total > pItem->GetMaxNumber())
				{
					// ÇÑ°è ¼öÄ¡¸¦ ³Ñ¾î°¥ °æ¿ì
					pMouseItem->SetNumber( total - pItem->GetMaxNumber() );
					pItem->SetNumber( pItem->GetMaxNumber() );
				}
				else
				{
					// ¸ðµÎ pItem¿¡ Ãß°¡µÉ ¼ö ÀÖ´Â °æ¿ì
					pItem->SetNumber( total );
					UI_DropItem();

					delete pMouseItem;
				}

				//----------------------------------------------------
				// ±³È¯Áß¿¡ ¼±ÅÃµÈ ¾ÆÀÌÅÛÀÎ °æ¿ì
				//----------------------------------------------------
				// Remove and Add - -;
				if (gC_vs_ui.IsRunningExchange() && pItem->IsTrade())
				{
						//----------------------------------------------------
						// ³õ¿© ÀÖ´ø°Å Á¦°Å
						//----------------------------------------------------
						CGTradeRemoveItem _CGTradeRemoveItem;
						_CGTradeRemoveItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
						_CGTradeRemoveItem.setItemObjectID( pItem->GetID() );

						g_pSocket->sendPacket( &_CGTradeRemoveItem );

						g_pTradeManager->SetNextAcceptTime();

						
						//----------------------------------------------------
						// ¾ÆÀÌÅÛ Ãß°¡ --> °³¼ö Áõ°¡
						//----------------------------------------------------
						CGAddMouseToInventory _CGAddMouseToInventory;
						_CGAddMouseToInventory.setObjectID( mouseItemID );
						_CGAddMouseToInventory.setInvenX( pItem->GetGridX() );
						_CGAddMouseToInventory.setInvenY( pItem->GetGridY() );								

						g_pSocket->sendPacket( &_CGAddMouseToInventory );

						bSendPacket = FALSE;

						
						//----------------------------------------------------
						// ´Ù½Ã ±³È¯Ã¢¿¡ ¾ÆÀÌÅÛ Ãß°¡
						//----------------------------------------------------
						CGTradeAddItem _CGTradeAddItem;
						_CGTradeAddItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
						_CGTradeAddItem.setItemObjectID( pItem->GetID() );

						g_pSocket->sendPacket( &_CGTradeAddItem );
						

					// ´Ù¸¥ »ç¶÷ÀÌ OK´©¸¥ »óÅÂ¶ó¸é..
					if (bAcceptOtherTrade)
					{
						g_pTradeManager->RefuseOtherTrade();
					}
				}

				PlaySound( pItem->GetInventorySoundID() );
			}

			//----------------------------------------------------
			// Server¿¡ Á¢¼ÓÇÑ °æ¿ì
			//----------------------------------------------------
				if (bSendPacket)
				{
					CGAddMouseToInventory _CGAddMouseToInventory;
					_CGAddMouseToInventory.setObjectID( mouseItemID );
					_CGAddMouseToInventory.setInvenX( pItem->GetGridX() );
					_CGAddMouseToInventory.setInvenY( pItem->GetGridY() );								

					g_pSocket->sendPacket( &_CGAddMouseToInventory );

					
					//----------------------------------------------------
					// °¢ Item class¿¡ µû¸¥ Ã³¸®
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
// mouseÀÇ ItemÀ» GearÀÇ Item¿¡ Ãß°¡ÇÒ ¶§
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

	// gpC_mouse_pointer->GetPickUpItem()		// µé°í ÀÖ´Â °Í
	//void_ptr = MItem *		// µé¾î°¥ °÷
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
			// Server¿¡ Á¢¼ÓÇÑ °æ¿ì
			//----------------------------------------------------
				//---------------------------------------------------
				// mouse¿¡ ÀÖ´ø itemÀ» Gear¿¡ Ãß°¡Çß´Ù°í
				// server·Î packetÀ» º¸³½´Ù.
				//---------------------------------------------------
				CGAddMouseToGear _CGAddMouseToGear;
				_CGAddMouseToGear.setObjectID( pMouseItem->GetID() );
				_CGAddMouseToGear.setSlotID( pItem->GetItemSlot() );
				
				g_pSocket->sendPacket( &_CGAddMouseToGear );

				
				// °ËÁõ¹Þ±â¸¦ ±â´Ù·Á¾ßÇÑ´Ù.
				g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_INSERT_FROM_GEAR );

				// °ËÁõ¹ÞÀ¸¸é m_p_current_pickup_itemÀ» pItemÀ» Ãß°¡ÇØ¾ßÇÑ´Ù.
		}
		//----------------------------------------------------
		// ¾Æ´Ñ °æ¿ì.
		//----------------------------------------------------
		else
		{
			// »ç¿ë¾ÈÇÔ.. - -;
		}
	}
	else
	{
		DEBUG_ADD( "[Watch Mode]");
	}
}


//-----------------------------------------------------------------------------
//
// mouseÀÇ ItemÀ» QuickSlotÀÇ Item¿¡ Ãß°¡ÇÒ ¶§
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
		// gpC_mouse_pointer->GetPickUpItem()		// µé°í ÀÖ´Â °Í
		//void_ptr = MItem *		// µé¾î°¥ °÷
		// QuickSlotÁÂÇ¥(left)
		MItem* pItem = (MItem*)void_ptr;
		MItem* pMouseItem = (MItem*)gpC_mouse_pointer->GetPickUpItem();

		if (pItem->GetItemClass()==pMouseItem->GetItemClass()
			&& pItem->GetItemType()==pMouseItem->GetItemType())
		{
			//----------------------------------------------------
			// Server¿¡ Á¢¼ÓÇÑ °æ¿ì
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

				
				// °ËÁõ¹Þ±â À§ÇØ¼­ ±â´Ù·Á¾ß ÇÑ´Ù.
				// °ËÁõ¹ÞÀ¸¸é m_p_current_pickup_itemÀ» pItemÀ» Ãß°¡ÇØ¾ßÇÑ´Ù.
				//g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_INSERT_FROM_INVENTORY );
				//MItem* pMouseItem = (MItem*)gpC_mouse_pointer->GetPickUpItem();

				//----------------------------------------------------
				// pMouseItemÀ» pItem¿¡ Ãß°¡½ÃÅ²´Ù.
				//----------------------------------------------------
				int total = pMouseItem->GetNumber() + pItem->GetNumber();
				if ( total > pItem->GetMaxNumber() )
				{
					// ÇÑ°è ¼öÄ¡¸¦ ³Ñ¾î°¥ °æ¿ì
					pMouseItem->SetNumber( total - pItem->GetMaxNumber() );
					pItem->SetNumber( pItem->GetMaxNumber() );
				}
				else
				{
					// ¸ðµÎ pItem¿¡ Ãß°¡µÉ ¼ö ÀÖ´Â °æ¿ì
					pItem->SetNumber( total );
					UI_DropItem();

					delete pMouseItem;
				}

				PlaySound( pItem->GetInventorySoundID() );

				//----------------------------------------------------
				// °¢ Item class¿¡ µû¸¥ Ã³¸®
				//----------------------------------------------------
				//if (pItem->GetItemClass()==ITEM_CLASS_MONEY)
				//{
				//}
				
		}
	}
}


//-----------------------------------------------------------------------------
//
// »óÁ¡ ´ÝÀ» ¶§,
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

		// shopÀ» ¾ø¾Ø´Ù.
		gC_vs_ui.SetShop( NULL );


		// ´Ù½Ã ¹º°¡¸¦?¼±ÅÃÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
		g_pUIDialog->ShowPCTalkDlg();
	}
}

//-----------------------------------------------------------------------------
//
// »óÁ¡¿¡¼­ »ì·Á´Â ¹°°ÇÀ» ¼±ÅÃÇßÀ» ¶§,
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

	// °ËÁõÇÒ°Ô ¾ø´Â °æ¿ì
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		int index = left;					// »ì·Á´Â ¾ÆÀÌÅÛÀÇ À§Ä¡
		int number = right;					// »ì·Á´Â °³¼ö
		MShop* pShop = (MShop*)void_ptr;	// »óÁ¡
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
					// »ì ¼ö ÀÖ´ÂÁö Ã¼Å©ÇÑ´Ù.
					//-------------------------------------------------							
					bool bBuyPossible = false;
					//GAME_STRINGID buyImpossibleMessage = STRING_MESSAGE_CANNOT_BUY;
					int buyImpossibleMessage = STRING_MESSAGE_CANNOT_BUY;

					switch (pShop->GetShopType())
					{
						//-------------------------------------------------
						// º¸Åë »óÁ¡
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
						// ÀÌº¥Æ®º° »óÁ¡
						//-------------------------------------------------
						case MShop::SHOP_EVENT_STAR :
						{
							STAR_ITEM_PRICE starPrice;

							g_pPriceManager->GetItemPrice( pItem, starPrice );

							if (starPrice.type!=-1 && starPrice.number!=0)
							{
								// ¸î°³³ª ÀÖ´ÂÁö Ã£¾Æº»´Ù.
								MItemClassTypeNumberFinder starFinder(ITEM_CLASS_EVENT_STAR, 
																		starPrice.type);


								((MItemManager*)g_pInventory)->FindItem( starFinder );

								// °¡Áö°í ÀÖ´Â°Ô ´õ ¸¹¾Æ¾ß ÇÑ´Ù.
								bBuyPossible = (starFinder.GetTotalNumber() >= starPrice.number);

								buyImpossibleMessage = STRING_MESSAGE_CANNOT_BUY_NO_STAR;
							}							
							
						}
						break;
					}

					//-------------------------------------------------
					// µ·ÀÌ ÃæºÐÇÑ °æ¿ì
					//-------------------------------------------------
					if (bBuyPossible)
					{								
						//-------------------------------------------------
						// inventory¿¡ ³ÖÀ» À§Ä¡¸¦ Ã£´Â´Ù.
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
								// °ËÁõÀ» À§ÇÑ Temp Information¼³Á¤
								//-------------------------------------------------
								(*g_pTempInformation).Mode	= TempInformation::MODE_SHOP_BUY;
								(*g_pTempInformation).Value1 = pShop->GetCurrent();
								(*g_pTempInformation).Value2 = index;
								(*g_pTempInformation).Value3 = point.x;
								(*g_pTempInformation).Value4 = point.y;
								(*g_pTempInformation).pValue = (void*)pShop;

								//-------------------------------------------------
								// ´Ù¸¥ ¾ÆÀÌÅÛ¿¡ Á¢±Ù ¸øÇÏµµ·Ï..
								//-------------------------------------------------
								UI_LockItemTrade();
						}
						else
						{
							// inventory°¡ ²ËÂ÷¼­ ¸ø »ê´Ù!
							g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_BUY_NO_SPACE ].GetString());
							
							// 2004, 5, 7 , sobeit add start - ÀÎº¥¿¡ ÀÚ¸®°¡ ¾øÀ¸¸é º¸°üÇÔ »ç¶ó°í µµ¿ò¸» º¸¿©ÁÜ
							ExecuteHelpEvent( HELP_EVENT_STORAGE_BUY );
							// 2004, 5, 6, sobeit add end
						}

						pItem->SetNumber( oldNumber );

					}
					//-------------------------------------------------
					// µ· È¤Àº º°ÀÌ.. ºÎÁ·ÇÑ °æ¿ì
					//-------------------------------------------------					
					else
					{
						g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[buyImpossibleMessage].GetString());
					}
				}
				else
				{
					// ¼±¹ÝÀÇ index À§Ä¡¿¡ ¾ÆÀÌÅÛÀÌ ¾ø´Â °æ¿ì
					DEBUG_ADD_FORMAT("[Error] There is NO Item in index=%d", index);
				}
			}
			else
			{
				// ¼±¹ÝÀÌ ¼³Á¤ ¾È µÈ °æ¿ì
				DEBUG_ADD_FORMAT("[Error] There is NO Shelf type=%d", (int)pShop->GetCurrent());
			}
		}
		else
		{
			// shopÀÌ ¼³Á¤ ¾È µÈ °æ¿ì
			DEBUG_ADD_FORMAT("[Error] There is NO Shop. npc id=%d", npcID);
		}
	}
}

//-----------------------------------------------------------------------------
//
// ¹°°Ç ÆÈ±â dialog ´ÝÀ» ¶§
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

		// ´Ù½Ã ¹º°¡¸¦?¼±ÅÃÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
		g_pUIDialog->ShowPCTalkDlg();
	}
}

//-----------------------------------------------------------------------------
//
// ÆÈ·Á´Â ¹°°ÇÀ» ¼±ÅÃÇßÀ» ¶§,
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
	// °ËÁõÇÒ°Ô ¾ø´Â °æ¿ì
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		const MItem* pItem = g_pInventory->GetItem( left, right );

		if (pItem!=NULL && g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
		{
			// Event GiftBox ¾ÆÀÌÅÛÀÎ °æ¿ì ¸ø ³õ´Â´Ù.
			
			if (//pItem->GetItemClass()!=ITEM_CLASS_EVENT_GIFT_BOX &&
				!pItem->IsUniqueItem() || (g_pTimeItemManager->IsExist( pItem->GetID())))
			{
				//-----------------------------------------------------
				// ¾ÆÀÌÅÛÀ» ÆÈ±â À§ÇÑ packetÀ» º¸³½´Ù.
				//-----------------------------------------------------
					CGShopRequestSell	_CGShopRequestSell;

					_CGShopRequestSell.setObjectID( (*g_pPCTalkBox).GetNPCID() );		// NPC ID	
					_CGShopRequestSell.setOpCode( SHOP_REQUEST_SELL_NORMAL );
					_CGShopRequestSell.setItemObjectID( pItem->GetID() );	// Item Object ID

					g_pSocket->sendPacket( &_CGShopRequestSell );
			
						
					//-------------------------------------------------
					// °ËÁõÀ» À§ÇÑ Temp Information¼³Á¤
					//-------------------------------------------------
					(*g_pTempInformation).Mode	= TempInformation::MODE_SHOP_SELL;
					(*g_pTempInformation).pValue = (void*)pItem;

					//-------------------------------------------------
					// ´Ù¸¥ ¾ÆÀÌÅÛ¿¡ Á¢±Ù ¸øÇÏµµ·Ï..
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
// ÆÈ·Á´Â ¹°°ÇÀ» ¼±ÅÃÇßÀ» ¶§,
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
	// °ËÁõÇÒ°Ô ¾ø´Â °æ¿ì
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		//-----------------------------------------------------
		// ¾ÆÀÌÅÛÀ» ÆÈ±â À§ÇÑ packetÀ» º¸³½´Ù.
		//-----------------------------------------------------
			CGShopRequestSell	_CGShopRequestSell;

			_CGShopRequestSell.setObjectID( (*g_pPCTalkBox).GetNPCID() );		// NPC ID	
			_CGShopRequestSell.setOpCode( SHOP_REQUEST_SELL_ALL_SKULL );
			_CGShopRequestSell.setItemObjectID( 0 );	// Item Object ID

			g_pSocket->sendPacket( &_CGShopRequestSell );
	
				
			//-------------------------------------------------
			// °ËÁõÀ» À§ÇÑ Temp Information¼³Á¤
			//-------------------------------------------------
			(*g_pTempInformation).Mode	= TempInformation::MODE_SHOP_SELL_ALL_SKULL;
			
			//-------------------------------------------------
			// ´Ù¸¥ ¾ÆÀÌÅÛ¿¡ Á¢±Ù ¸øÇÏµµ·Ï..
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
// Skill View¸¦ ´ÝÀ» ¶§,
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

	// ´Ù½Ã ¹º°¡¸¦?¼±ÅÃÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
	//g_pUIDialog->ShowPCTalkDlg();

	// mode Á¦°Å
	//(*g_pTempInformation).Mode = TempInformation::MODE_NULL;
}


//-----------------------------------------------------------------------------
//
//	SkillÀ» ¹è¿î´Ù°í ÇßÀ» ¶§,
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
// PDS¸¦ ´ÝÀ» ¶§,
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
// Slayer Á¤º¸ º¸±â Àü¿¡ °ª ¼³Á¤ÇØÁÖ±â..
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
// PCS ¹øÈ£¸¦ ¹ÞÀ» ¶§ --> ´Ù¸¥ »ç¶÷°ú ÅëÈ­ÇÒ·Á°í..
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
	// Server¿¡ Á¢¼ÓÇØ ÀÖÀ» ¶§,
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

	//gC_vs_ui.OnLinePCS(); // PCS°¡ ¿¬°áµÇ¾úÀ» ¶§ ÇØÁà¾ß ÇÑ´Ù!
}
 */

//-----------------------------------------------------------------------------
//
// Á¢¼Ó µÆÀ» ¶§
//
//-----------------------------------------------------------------------------
/*
case UI_PCS_CONNECTOR_GRANTED:
//
// ´ë±â¹æ¿¡ ÀÖ´Â »ç¶÷°ú ¿¬°áÇÔ.
//
// left = pcs number
//
DEBUG_ADD("[UI] UI_PCS_CONNECTOR_GRANTED");

UI_OnLinePCS();
break;
*/

//-----------------------------------------------------------------------------
//
// PCS Á¢¼ÓÁß¿¡ ³¡³¾¶§..
//
//-----------------------------------------------------------------------------
// PCS Ã¢¿¡¼­ PCS number¸¦ sendÇÏ°í connecting Áß¿¡ END¸¦ ´­·¶´Ù.
/*
void
UIMessageManager::Execute_UI_END_PCS(int left, int right, void* void_ptr) 
{
	DEBUG_ADD("[UI] UI_END_PCS");
	
	//------------------------------------------------
	// ¸ðµç Á¢¼ÓÀ» ´Ù ²÷´Â´Ù.
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
// Online »óÅÂ¿¡¼­ PCS³¡³¾¶§
//
//-----------------------------------------------------------------------------
/*
void
UIMessageManager::Execute_UI_QUIT_PCS_ONLINE_MODE(int left, int right, void* void_ptr)
{
	//
	// PCS online mode¿¡¼­ quit ¹öÆ°À» ´­·¶´Ù.
	//
	DEBUG_ADD("[UI] UI_QUIT_PCS_ONLINE_MODE");
	
	//------------------------------------------------
	// ¸ðµç Á¢¼ÓÀ» ´Ù ²÷´Â´Ù.
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
// ´Ù¸¥ »ç¶÷À¸·Î ¹Ù²Þ.
//
//-----------------------------------------------------------------------------
/*
void
UIMessageManager::Execute_UI_CHANGE_PCS_CONNECTED_SLOT(int left, int right, void* void_ptr)
{
	//
	// ¶Ç ´Ù¸¥ ¿¬°áµÈ »ç¶÷À¸·Î ¹Ù²Þ.
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
// Online »óÅÂ¿¡¼­ PCS³¡³¾¶§
//
//-----------------------------------------------------------------------------
// ´ë±â¹æ click! (ÀÌ¹Ì ¿¬°á-duplex-µÇÁö ¾Ê¾ÒÀ½)
/*
void
UIMessageManager::Execute_UI_PLEASE_PCS_CONNECT_ME(int left, int right, void* void_ptr)
{
	//
	// »ç¶÷ÀÌ ÀÖ´Â ´ë±â¹æÀ» Å¬¸¯ÇÏ¿´´Ù. (¸Þ½ÃÁö¸¦ ¼Û½Å ÇÏ±â À§ÇØ¼­)
	//
	// left = pcs number
	// right = slot 
	//
	DEBUG_ADD("[UI] UI_PLEASE_PCS_CONNECT_ME");
	
	int slot = right;

	// Å¬¸¯µÈ slot°ú ¿¬°áÇÔ.
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
	// ÀÎº¥Åä¸®¿¡¼­ µ·À» dropÇÑ´Ù.
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
	// µ·ÀÌ 0ÀÌ¸é ¾ÈµÈ´Ù.
	//-----------------------------------------------------------------------------
	if (left > 0)
	{
		if (g_pMoneyManager!=NULL)
		{
				CGDropMoney _CGDropMoney;
				_CGDropMoney.setAmount( left );

				g_pSocket->sendPacket( &_CGDropMoney );

				

			//-----------------------------------------------
			// µ· ÁÙ¿©¹ö¸°´Ù.
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
// Click Bonus Point - º¸³Ê½º Æ÷ÀÎÆ® ¿Ã¸±¶§,
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_CLICK_BONUS_POINT(int left, int right, void* void_ptr)
{
	//
	// vampire bonus point¸¦ ¿Ã¸®±â À§ÇØ ¹öÆ°À» ´­·¶´Ù.
	//
	// g_char_slot_ingameÀÇ bonus point¸¦ °Ë»öÇÏ¸é ¾ó¸¶³ª ³²¾Ò´ÂÁö ¾Ë ¼ö ÀÖ´Ù.
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

	// ±â¾ïÇØµÎ±â À§ÇØ¼­...
	if ((*g_pTempInformation).Mode==TempInformation::MODE_NULL)
	{
		int bonusPoint = g_pPlayer->GetBonusPoint();

		//----------------------------------------------
		// vampireÀÌ°í bonuspoint°¡ ÀÖÀ» ¶§,
		//----------------------------------------------
		// 2005, 1, 18, sobeit modify start - ½ÂÁ÷ ½½·¹´Â º¸³Ê½º Æ÷ÀÎÆ®°¡ ÀÖ´Ù.
//		if (!g_pPlayer->IsSlayer()// || g_pPlayer->IsOusters()
//			&& bonusPoint > 0)
		if(bonusPoint > 0)
		// 2005, 1, 18, sobeit modify end - ½ÂÁ÷ ½½·¹´Â º¸³Ê½º Æ÷ÀÎÆ®°¡ ÀÖ´Ù.
		{
			
				CGUseBonusPoint _CGUseBonusPoint;

				_CGUseBonusPoint.setWhich( bonusPart[left] );

				g_pSocket->sendPacket( &_CGUseBonusPoint );

				
				//----------------------------------------------
				// ¼±ÅÃÇÑ ºÎºÐ ±â¾ï
				//----------------------------------------------
				(*g_pTempInformation).Mode = TempInformation::MODE_BONUSPOINT_USE;
				(*g_pTempInformation).Value1 = bonusPart[left];				
		
				
		}
	}
}


//-----------------------------------------------------------------------------
//
// CharInfo ¹öÆ°À» ´­·¶À» ¶§,
//
//-----------------------------------------------------------------------------
void
UIMessageManager::Execute_UI_INFO(int left, int right, void* void_ptr)
{
	//
	// character info buttonÀ» ´­·¶´Ù. ÀÚ½ÅÀÇ Á¤º¸¸¦ º¸¿©Áà¾ß ÇÑ´Ù!
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
//	// ¿­·ÁÀÖÀ¸¸é ´Ý´Â´Ù.
//	//----------------------------------------------
//	if (gC_vs_ui.IsRunningCharInfo())
//	{
//		gC_vs_ui.CloseCharInfo();
//	}
//	//----------------------------------------------
//	// ¶ç¿î´Ù.
//	//----------------------------------------------
//	else
//	{
//		// set slot
//
//		// g_char_slot_ingame °ÔÀÓ½ÇÇà Áß °Ô¼Ó ¼³Á¤ÇØÁà¾ß ÇÑ´Ù.
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
// ¼ö¸®ÇÒ·Á´Â ¹°°ÇÀ» ¼±ÅÃÇßÀ» ¶§,
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

	
	// °ËÁõÇÒ°Ô ¾ø´Â °æ¿ì
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		MItem* pItem = NULL;// = (MItem*)void_ptr;
		int itemID = left;

		//-------------------------------------------------------------
		// itemID°¡ 0ÀÎ °æ¿ì´Â ÀüÃ¼ ´Ù ¼ö¸®
		//-------------------------------------------------------------
		if (itemID==0)
		{
			//-----------------------------------------------------
			// ¾ÆÀÌÅÛÀ» ¼ö¸®ÇÏ±â À§ÇÑ packetÀ» º¸³½´Ù.
			//-----------------------------------------------------
				CGRequestRepair	_CGRequestRepair;

				_CGRequestRepair.setObjectID( 0 );
				
				g_pSocket->sendPacket( &_CGRequestRepair );
			
				//-------------------------------------------------
				// °ËÁõÀ» À§ÇÑ Temp Information¼³Á¤
				//-------------------------------------------------
				(*g_pTempInformation).Mode		= TempInformation::MODE_SHOP_REPAIR;
				(*g_pTempInformation).Value1	= false;	// inventory¿¡ ÀÖ´Â itemÀÎ°¡?
				(*g_pTempInformation).Value2	= true;		// gear¿¡ ÀÖ´Â itemÀÎ°¡?
				(*g_pTempInformation).pValue	= NULL;

				//-------------------------------------------------
				// ´Ù¸¥ ¾ÆÀÌÅÛ¿¡ Á¢±Ù ¸øÇÏµµ·Ï..
				//-------------------------------------------------
				UI_LockItemTrade();
		}
		//-------------------------------------------------------------
		// item ÇÏ³ª¸¸ ¼ö¸®
		//-------------------------------------------------------------
		else
		{
			BOOL bInInventory = FALSE;
			BOOL bInGear = FALSE;

			//------------------------------------------------------------------------------
			// inventory¿¡¼­ Ã£´Â´Ù.
			//------------------------------------------------------------------------------
			pItem = g_pInventory->GetItemToModify( itemID );

			if (pItem==NULL)
			{
				//------------------------------------------------------------------------------
				// gear¿¡¼­ Ã£´Â´Ù.
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
			// inventory³ª gear¿¡ ÀÖ´Ù¸é ¼ö¸®ÇÑ´Ù.
			//-------------------------------------------------
			if (bInInventory || bInGear)
			{
				//-------------------------------------------------
				// ¼ö¸®ÇÒ ¼ö ÀÖ´Â itemÀÎÁö Ã¼Å©ÇÑ´Ù.
				// ¿­¼è´Â ¿ÀÅä¹ÙÀÌ ¼ö¸®¸¦ ÀÇ¹ÌÇÑ´Ù.
				//-------------------------------------------------
				if (pItem->GetItemClass() != ITEM_CLASS_VAMPIRE_AMULET
					|| !pItem->IsUniqueItem()
					|| !pItem->IsQuestItem()
					|| pItem->GetMaxDurability() != -1
					|| pItem->GetItemClass()==ITEM_CLASS_KEY
					|| pItem->IsChargeItem())
				{
					//-------------------------------------------------
					// µ·ÀÌ ÃæºÐÇÑÁö Ã¼Å©ÇÑ´Ù.
					//-------------------------------------------------
					int price = g_pPriceManager->GetItemPrice(pItem, MPriceManager::REPAIR);
					int money = g_pMoneyManager->GetMoney();

					//-------------------------------------------------					
					// °¡°ÝÀÌ 0ÀÌ¸é ¼ö¸®ÇÒ ÇÊ¿ä°¡ ¾ø´Ù.
					// ´Ü, key´Â ¿ÀÅä¹ÙÀÌ ¼ö¸®¿¡ ÀÌ¿ëµÈ´Ù.
					//-------------------------------------------------					
					if (price!=0 || pItem->GetItemClass()==ITEM_CLASS_KEY)
					{
						//-------------------------------------------------
						// µ·ÀÌ ÃæºÐÇÑ °æ¿ì
						//-------------------------------------------------
						if (money!=0 && price <= money)
						{														
							//-----------------------------------------------------
							// ¾ÆÀÌÅÛÀ» ¼ö¸®ÇÏ±â À§ÇÑ packetÀ» º¸³½´Ù.
							//-----------------------------------------------------
								CGRequestRepair	_CGRequestRepair;

								_CGRequestRepair.setObjectID( pItem->GetID() );
								
								g_pSocket->sendPacket( &_CGRequestRepair );
							
								//-------------------------------------------------
								// °ËÁõÀ» À§ÇÑ Temp Information¼³Á¤
								//-------------------------------------------------
								(*g_pTempInformation).Mode		= TempInformation::MODE_SHOP_REPAIR;
								(*g_pTempInformation).Value1	= bInInventory;	// inventory¿¡ ÀÖ´Â itemÀÎ°¡?
								(*g_pTempInformation).Value2	= bInGear;		// gear¿¡ ÀÖ´Â itemÀÎ°¡?
								(*g_pTempInformation).pValue	= (void*)pItem;

								//-------------------------------------------------
								// ´Ù¸¥ ¾ÆÀÌÅÛ¿¡ Á¢±Ù ¸øÇÏµµ·Ï..
								//-------------------------------------------------
								UI_LockItemTrade();
						}
						//-------------------------------------------------
						// µ·ÀÌ ºÎÁ·ÇÑ °æ¿ì
						//-------------------------------------------------					
						else
						{
							g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_BUY_NO_MONEY ].GetString());
						}
					}
					//-------------------------------------------------
					// ¼ö¸®ÇÒ ÇÊ¿ä°¡ ¾ø´Â °æ¿ì
					//-------------------------------------------------
					else
					{
						g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_REPAIR].GetString());
					}
				}
				//-------------------------------------------------
				// ¼ö¸®ÇÒ ÇÊ¿ä°¡ ¾ø´Â °æ¿ì
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
// Silvering ¹°°ÇÀ» ¼±ÅÃÇßÀ» ¶§,
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

	
	// °ËÁõÇÒ°Ô ¾ø´Â °æ¿ì
	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
	{
		MItem* pItem = NULL;// = (MItem*)void_ptr;
		int itemID = left;

		BOOL bInInventory = FALSE;
		BOOL bInGear = FALSE;

		//------------------------------------------------------------------------------
		// inventory¿¡¼­ Ã£´Â´Ù.
		//------------------------------------------------------------------------------
		pItem = g_pInventory->GetItemToModify( itemID );

		if (pItem==NULL)
		{
			//------------------------------------------------------------------------------
			// gear¿¡¼­ Ã£´Â´Ù.
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
		// inventory³ª gear¿¡ ÀÖ´Ù¸é silveringÇÑ´Ù..
		//-------------------------------------------------
		if (bInInventory || bInGear)
		{
			//-------------------------------------------------
			// Silvering ÇÒ ¼ö ÀÖ´Â itemÀÎÁö Ã¼Å©ÇÑ´Ù.
			//-------------------------------------------------
			if (pItem->GetSilverMax() > 0)
			{
				//-------------------------------------------------
				// µ·ÀÌ ÃæºÐÇÑÁö Ã¼Å©ÇÑ´Ù.
				//-------------------------------------------------
				int price = (*g_pPriceManager).GetItemPrice(pItem, MPriceManager::SILVERING);
				int money = (*g_pMoneyManager).GetMoney();

				//-------------------------------------------------					
				// °¡°ÝÀÌ 0ÀÌ¸é silvering ÇÒ ÇÊ¿ä°¡ ¾ø´Ù.
				//-------------------------------------------------					
				if (price!=0)
				{
					//-------------------------------------------------
					// µ·ÀÌ ÃæºÐÇÑ °æ¿ì
					//-------------------------------------------------
					if (money!=0 && price <= money)
					{														
						//-----------------------------------------------------
						// ¾ÆÀÌÅÛÀ» silverigÇÏ±â À§ÇÑ packetÀ» º¸³½´Ù.
						//-----------------------------------------------------
							CGSilverCoating	_CGSilverCoating;

							_CGSilverCoating.setObjectID( pItem->GetID() );
							
							g_pSocket->sendPacket( &_CGSilverCoating );
						
							//-------------------------------------------------
							// °ËÁõÀ» À§ÇÑ Temp Information¼³Á¤
							//-------------------------------------------------
							(*g_pTempInformation).Mode		= TempInformation::MODE_SHOP_SILVERING;
							(*g_pTempInformation).Value1	= bInInventory;	// inventory¿¡ ÀÖ´Â itemÀÎ°¡?
							(*g_pTempInformation).Value2	= bInGear;		// gear¿¡ ÀÖ´Â itemÀÎ°¡?
							(*g_pTempInformation).pValue	= (void*)pItem;

							//-------------------------------------------------
							// ´Ù¸¥ ¾ÆÀÌÅÛ¿¡ Á¢±Ù ¸øÇÏµµ·Ï..
							//-------------------------------------------------
							UI_LockItemTrade();
					}
					//-------------------------------------------------
					// µ·ÀÌ ºÎÁ·ÇÑ °æ¿ì
					//-------------------------------------------------					
					else
					{
						g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_BUY_NO_MONEY ].GetString());
					}
				}
				//-------------------------------------------------
				// SilveringÇÒ ÇÊ¿ä°¡ ¾ø´Â °æ¿ì
				//-------------------------------------------------
				else
				{
					g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_SILVERING].GetString());
				}
			}
			//-------------------------------------------------
			// ¼ö¸®ÇÒ ÇÊ¿ä°¡ ¾ø´Â °æ¿ì
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
// ¼ö¸® ±×¸¸µÎ±â
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

		// ´Ù½Ã ¹º°¡¸¦?¼±ÅÃÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
		g_pUIDialog->ShowPCTalkDlg();
	}
}

//-----------------------------------------------------------------------------
//
// silvering ±×¸¸µÎ±â
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

		// ´Ù½Ã ¹º°¡¸¦?¼±ÅÃÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
		g_pUIDialog->ShowPCTalkDlg();
	}
}

//-----------------------------------------------------------------------------
//
// ´É·ÂÄ¡
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
// °ÔÀÓ¸Þ´º ¶ç¿ï¶§, ´Ù¸¥ °Å ´Ù ´Ý´Â´Ù.
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
// º¸°üÇÔ »ç±â
//
//-----------------------------------------------------------------------------
// left = »ê´Ù(TRUE), ¾È»ê´Ù(FALSE)
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
	// º¸°üÇÔÀ» »ì·Á´Â »óÅÂ..
	//-----------------------------------------------------------------------------
	if (buy)
	{
		if (g_pTempInformation->GetMode()==TempInformation::MODE_STORAGE_BUY)
		{		
			//--------------------------------------------------------
			// µ· Ã¼Å©
			//--------------------------------------------------------
			if (g_pMoneyManager->GetMoney() >= g_pTempInformation->Value1)
			{
					CGStashRequestBuy _CGStashRequestBuy;
					
					g_pSocket->sendPacket( &_CGStashRequestBuy );					

				// mode ¼³Á¤
				g_pTempInformation->SetMode(TempInformation::MODE_STORAGE_BUY_WAIT);

			}
			//--------------------------------------------------------
			// µ·ÀÌ ºÎÁ·ÇÑ °æ¿ì
			//--------------------------------------------------------
			else
			{
				// ´Ù½Ã ¹º°¡¸¦?¼±ÅÃÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
				g_pUIDialog->ShowPCTalkDlg();

				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_BUY_NO_MONEY ].GetString());
				
				g_pTempInformation->SetMode(TempInformation::MODE_NULL);
			}
		}
		else
		{
			DEBUG_ADD("[Error] Mode is Not MODE_STORAGE_BUY");
		}

		// »ç´Â °æ¿ì´Â.. °á°ú packetÀ» ¹ÞÀ»¶§±îÁö
		// NPC´ëÈ­ dialog¸¦ ¾È ¶ç¿î´Ù.
	}
	//-----------------------------------------------------------------------------
	// º¸°üÇÔ ¾È »ê´Ù.
	//-----------------------------------------------------------------------------
	else
	{
		// ´Ù½Ã ¹º°¡¸¦?¼±ÅÃÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
		g_pUIDialog->ShowPCTalkDlg();

		g_pTempInformation->SetMode(TempInformation::MODE_NULL);
	}					
}

//-----------------------------------------------------------------------------
//
// º¸°üÇÔÀÇ slotÀ» ¼±ÅÃÇÏ¿´´Ù.
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

	g_pStorage->SetCurrent( left );	// È®ÀÎ¿ë

	int slot = right;

	MItem* pMouseItem = (MItem*)gpC_mouse_pointer->GetPickUpItem();
	const MItem* pStorageItem = g_pStorage->GetItem( slot );

	//-----------------------------------------------------------------------------
	// º¸°üÇÔ¿¡¼­ ¹º°¡¸¦ ÁýÀ»·Á´Â °æ¿ì
	//-----------------------------------------------------------------------------
	if (pMouseItem==NULL)
	{
		//---------------------------------------------------
		// ¼±ÅÃÇÑ slot¿¡ itemÀÌ ÀÖÀ¸¸é Áý´Â´Ù.
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
	// µé°í ÀÖ´Â itemÀ» º¸°üÇÔ¿¡ ³õÀ»·Á´Â °æ¿ì
	//-----------------------------------------------------------------------------
	else 
	{
		BOOL bSendPacket = TRUE;

		TYPE_OBJECTID mouseItemID = pMouseItem->GetID();		
	
		// Event GiftBox ¾ÆÀÌÅÛÀÎ °æ¿ì ¸ø ³õ´Â´Ù.
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
			)
		{
			//---------------------------------------------------
			// ±× ÀÚ¸®¿¡ ¾Æ¹«°Íµµ ¾øÀ¸¸é item ³Ö´Â´Ù.
			//---------------------------------------------------
			if (pStorageItem==NULL)
			{
				UI_DropItem();	// mouse¿¡¼­ item¶¾´Ù.

				g_pStorage->SetItem( slot, pMouseItem );							
			}
			//---------------------------------------------------
			// ¹º°¡ ÀÖ´Ù¸é...
			//---------------------------------------------------
			else
			{
				//---------------------------------------------------
				// ½×ÀÏ ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÎ °æ¿ì
				//---------------------------------------------------
				if (pStorageItem->IsPileItem()
					&& pStorageItem->GetItemClass()==pMouseItem->GetItemClass()
					&& pStorageItem->GetItemType()==pMouseItem->GetItemType())
				{
					//---------------------------------------------------
					// ´õ ½×ÀÏ ¼ö ÀÖ´Ù¸é
					//---------------------------------------------------
					if (pStorageItem->GetNumber() < pStorageItem->GetMaxNumber())
					{
						PlaySound( pMouseItem->GetInventorySoundID() );
						
						MItem* pModifyStorageItem = g_pStorage->RemoveItem( slot );

						//----------------------------------------------------
						// pMouseItemÀ» pStorageItem¿¡ Ãß°¡½ÃÅ²´Ù.
						//----------------------------------------------------
						int total = pMouseItem->GetNumber() + pStorageItem->GetNumber();
						if ( total > pStorageItem->GetMaxNumber() )
						{
							// ÇÑ°è ¼öÄ¡¸¦ ³Ñ¾î°¥ °æ¿ì
							pMouseItem->SetNumber( total - pStorageItem->GetMaxNumber() );
							pModifyStorageItem->SetNumber( pStorageItem->GetMaxNumber() );
						}
						else
						{
							// ¸ðµÎ pItem¿¡ Ãß°¡µÉ ¼ö ÀÖ´Â °æ¿ì
							pModifyStorageItem->SetNumber( total );
							UI_DropItem();

							delete pMouseItem;
						}	

						//----------------------------------------------------
						// ¹Ù²Û°Å¸¦ ´Ù½Ã ¼³Á¤ÇÑ´Ù.
						//----------------------------------------------------
						g_pStorage->SetItem( slot, pModifyStorageItem );					
						
					}
					//---------------------------------------------------
					// ´õ ½×ÀÏ ¼ö ¾ø´Ù¸é.. ±â³É µÐ´Ù.
					//---------------------------------------------------				
					else
					{
						bSendPacket = FALSE;
					}
				}
				//---------------------------------------------------				
				// ½×ÀÏ ¼ö ¾ø´Â ¾ÆÀÌÅÛÀÎ °æ¿ì --> ¹Ù²Û´Ù.
				//---------------------------------------------------				
				else
				{				
					MItem* pTempItem = pMouseItem;
					
					UI_PickUpItem( (MItem*)pStorageItem );

					g_pStorage->RemoveItem( slot );	// ÀÌ°Ô ½ÇÆÐÇÏ¸é.. - -;

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
// º¸°üÇÔ¿¡ µ· ÀúÀåÇÒ¶§,
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
	// µ·ÀÌ 0ÀÌ¸é ¾ÈµÈ´Ù.
	//-----------------------------------------------------------------------------
	if (left > 0)
	{
		if (g_pMoneyManager!=NULL && g_pStorage!=NULL)
		{
				CGStashDeposit _CGStashDeposit;
				_CGStashDeposit.setAmount( left );

				g_pSocket->sendPacket( &_CGStashDeposit );				

			//-----------------------------------------------
			// µ·À» º¸°üÇÔÀ¸·Î ¿Å±ä´Ù.
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
// º¸°üÇÔÀÇ µ·À» Ã£À»¶§,
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
	// µ·ÀÌ 0ÀÌ¸é ¾ÈµÈ´Ù.
	//-----------------------------------------------------------------------------
	if (left > 0)
	{
		if (g_pMoneyManager!=NULL && g_pStorage!=NULL)
		{
				CGStashWithdraw _CGStashWithdraw;
				_CGStashWithdraw.setAmount( left );

				g_pSocket->sendPacket( &_CGStashWithdraw );				

			//-----------------------------------------------
			// º¸°üÇÔÀÇ µ·À» player¿¡°Ô ¿Å±ä´Ù.
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
//	º¸°üÇÔ ´ÝÀ» ¶§
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

	// ´Ù½Ã ¹º°¡¸¦?¼±ÅÃÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
	g_pUIDialog->ShowPCTalkDlg();
}

//-----------------------------------------------------------------------------
//
// ±³È¯Ã¢¿¡ µ·À» ³Ö´Â´Ù.
//
//-----------------------------------------------------------------------------
// left = bGive - ³²ÇÑÅ× ÁÖ´Â µ·ÀÎ°¡?
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
		// ±³È¯Áß¿¡ OK ´­·¶À»¶§´Â °ËÁõÀ» ¹Þ¾Æ¾ß ÇÑ´Ù.
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
		// OK ¾È ´­·ÁÀÖ´Â º¸Åë ¶§..
		//---------------------------------------------------------
		else
		{
			/*
			//-----------------------------------------------------------
			// ¹º°¡ ¹Ù²ï´Ù¸é... OKÃë¼Ò
			//-----------------------------------------------------------
			g_pTradeManager->RefuseOtherTrade();
			g_pTradeManager->RefuseMyTrade();
		
			//-----------------------------------------------------------
			// client µ· ÀÌµ¿
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
		// client µ· ÀÌµ¿
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
		// server¿¡ ¾Ë¸²
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
// ±³È¯Ã¢ÀÇ ¾ÆÀÌÅÛÀ» ¼±ÅÃÇÒ¶§ 
//
//-----------------------------------------------------------------------------
// gridXY = (left,right)
// µé·Á´Â item = (MItem*)void_ptr
/*
void
UIMessageManager::Execute_UI_ITEM_PICKUP_FROM_TRADEGRID(int left, int right, void* void_ptr)
{
	//
	// hi/low dw_left´Â grid (x, y)°ªÀÌ´Ù.
	//        dw_right´Â itemÀÇ screen ÁÂÇ¥ÀÌ´Ù.
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
// ±³È¯Ã¢ÀÇ ¾ÆÀÌÅÛ¿¡ Ãß°¡µÉ¶§,
//
//-----------------------------------------------------------------------------
// gridXY = (left, right)
// ³õ¿©ÀÖ´ø item = (MItem*)void_ptr
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
// ±³È¯Ã¢¿¡ ¾ÆÀÌÅÛ ³õÀ» ¶§,
//
//-----------------------------------------------------------------------------
// gridXY = (left, right)
// ³õ±â ¹Ù·Î Àü¿¡ µé°í ÀÖ´ø item = (MItem*)void_ptr
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
// ±³È¯Ã¢¿¡ ¾ÆÀÌÅÛ ³õÀ» ¶§,
//
//-----------------------------------------------------------------------------
// gridXY = (left, right)
// ³õ±â ¹Ù·Î Àü¿¡ µé°í ÀÖ´ø item = (MItem*)void_ptr
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

		// ¼±¹° »óÀÚ ÇÏµå ÄÚµù-¤µ-;
		if(pItem->GetItemClass() == ITEM_CLASS_EVENT_GIFT_BOX && pItem->GetItemType() > 1 && pItem->IsTrade() == false)	//
		{
			MItem* pFindItem = ((MItemManager*)g_pInventory)->FindItem( MEventGiftBoxItemFinder() );
			if(pFindItem != NULL)
			{
				// ÀÌ¹Ì ±³È¯¿¡ ¿Ã¶ó°£°Ô ÀÖ´Ù
				return;
			}
		}
	

		//---------------------------------------------------------
		// ±³È¯Áß¿¡ OK ´­·¶À»¶§´Â °ËÁõÀ» ¹Þ¾Æ¾ß ÇÑ´Ù.
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
				// ¼±ÅÃµÇ¾î ÀÖ´ø ¾ÆÀÌÅÛ --> Ãë¼Ò
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

					// [µµ¿ò¸»] ±³È¯ Áß - ¾ÆÀÌÅÛ Ãë¼Ò
//					__BEGIN_HELP_EVENT
////						ExecuteHelpEvent( HE_TRADE_ITEM_REMOVE );	
//					__END_HELP_EVENT
				}
				//-----------------------------------------------------------
				// ¾ÆÀÌÅÛ ¼±ÅÃ
				//-----------------------------------------------------------
				else
				{
					g_pTempInformation->SetMode(TempInformation::MODE_TRADE_VERIFY_ADD_ITEM);

					g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_TRADE_ADD );

						CGTradeAddItem _CGTradeAddItem;
						_CGTradeAddItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
						_CGTradeAddItem.setItemObjectID( pItem->GetID() );

						g_pSocket->sendPacket( &_CGTradeAddItem );
						

					// [µµ¿ò¸»] ±³È¯ Áß - ¾ÆÀÌÅÛ ¼±ÅÃ
//					__BEGIN_HELP_EVENT
////						ExecuteHelpEvent( HE_TRADE_ITEM_ADD );	
//					__END_HELP_EVENT
				}				
			}			
		}
		//---------------------------------------------------------
		// OK ¾È ´­·ÁÀÖ´Â º¸Åë ¶§..
		//---------------------------------------------------------
		else
		{		
			//-----------------------------------------------------------
			// ¼±ÅÃµÇ¾î ÀÖ´ø ¾ÆÀÌÅÛ --> Ãë¼Ò
			//-----------------------------------------------------------
			if (pItem->IsTrade())
			{
				//-----------------------------------------------------------
				// ¹º°¡ ¹Ù²ï´Ù¸é... OKÃë¼Ò
				//-----------------------------------------------------------
				g_pTradeManager->RefuseOtherTrade();
				g_pTradeManager->RefuseMyTrade();

				pItem->UnSetTrade();		

					CGTradeRemoveItem _CGTradeRemoveItem;
					_CGTradeRemoveItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
					_CGTradeRemoveItem.setItemObjectID( pItem->GetID() );

					g_pSocket->sendPacket( &_CGTradeRemoveItem );

					g_pTradeManager->SetNextAcceptTime();

				// [µµ¿ò¸»] ±³È¯ Áß - ¾ÆÀÌÅÛ Ãë¼Ò
//				__BEGIN_HELP_EVENT
////					ExecuteHelpEvent( HE_TRADE_ITEM_REMOVE );	
//				__END_HELP_EVENT
			}
			//-----------------------------------------------------------
			// ¼±ÅÃµÇ¾î ÀÖÁö ¾Ê´ø ¾ÆÀÌÅÛ --> ¼±ÅÃ
			//-----------------------------------------------------------
			else
			{
				// »¡°£»ö ¼±¹°»óÀÚ´Â ±³È¯µÇÁö ¾Ê´Â´Ù.
				if (pItem->GetItemClass()==ITEM_CLASS_EVENT_GIFT_BOX
					&& pItem->GetItemType()==1)
				{
				}
				else
				{
					// ¼±¹°»óÀÚÀÎ °æ¿ì´Â °ËÁõÀÌ ÇÊ¿äÇÏ´Ù.
					if (pItem->GetItemClass()==ITEM_CLASS_EVENT_GIFT_BOX)// && pItem->GetItemClass() < 2)
					{
						g_pTempInformation->SetMode(TempInformation::MODE_TRADE_VERIFY_ADD_ITEM);
						g_pTempInformation->pValue	= pItem;
						
						g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_TRADE_ADD );
					}
					// ±× ¿Ü¿¡´Â ±×³É ³ÖÀ¸¸é µÈ´Ù.
					else
					{
						//-----------------------------------------------------------
						// ¹º°¡ ¹Ù²ï´Ù¸é... OKÃë¼Ò
						//-----------------------------------------------------------
						g_pTradeManager->RefuseOtherTrade();
						g_pTradeManager->RefuseMyTrade();

						pItem->SetTrade();		
					}

						CGTradeAddItem _CGTradeAddItem;
						_CGTradeAddItem.setTargetObjectID( g_pTradeManager->GetOtherID() );
						_CGTradeAddItem.setItemObjectID( pItem->GetID() );

						g_pSocket->sendPacket( &_CGTradeAddItem );
						

					// [µµ¿ò¸»] ±³È¯ Áß - ¾ÆÀÌÅÛ ¼±ÅÃ
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
// ±³È¯ È®ÀÎ
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
		// ±³È¯ÀÌ °¡´ÉÇÑ °æ¿ì
		// ¼±¹°»óÀÚ¸¦ µé°í ÀÖÀ¸¸é ±³È¯ÀÌ ¾ÈµÈ´Ù.
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
		// ±³È¯ÀÌ ºÒ°¡´ÉÇÑ °æ¿ì
		//--------------------------------------------------------
		else
		{
			// ±³È¯ÇÒ ¼ö ¾ø´Ù¸é..
			// ÀÏ´ÜÀº.. °ø°£ ºÎÁ·ÀÌ¶ó°í º»´Ù.
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
// ±³È¯ Ãë¼Ò
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
		// OK ´©¸¥ »óÅÂ¿¡¼­´Â °ËÁõÀ» ¹Þ¾Æ¾ß ÇÑ´Ù.
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
		// ±×³É °ÅºÎ (ÀÖÀ» ¼ö ¾ø´Â °æ¿ìÀÝ¾Æ - -;;)
		//---------------------------------------------------------------
		else
		{		
			//---------------------------------------------------------------
			// ±³È¯ °ÅºÎ´ç..
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
	// ³» OK¸¦ Ãë¼ÒÇÏ´Â packet
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
// ±³È¯Ã¢ ´Ý±â
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
	// OK ´©¸¥ »óÅÂ¿¡¼­´Â °ËÁõÀ» ¹Þ¾Æ¾ß ÇÑ´Ù.
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
	// ±×³É Ãë¼Ò..
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
		// ±³È¯ ¾È ÇØ~~ ³¡~ÀÌ´ç
		//---------------------------------------------------------------
		UI_CloseExchange();	
	}
	
	//---------------------------------------------------------------
	// ±³È¯À» ³¡³»´Â packet
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
// Ã¤ÆÃÃ¢¿¡¼­ ÀÌ¸§ ¼±ÅÃ
//
//-----------------------------------------------------------------------------
// left = ÀÔ·ÂÀÇ Á¾·ù
// void_ptr = Ä³¸¯ÅÍÀÌ¸§(!=NULL)
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
// ±³È¯ÇÒ·¡? Y/N¿¡ ´ëÇÑ ÀÀ´ä
//
//-----------------------------------------------------------------------------
// left = (BOOL)±³È¯¿©ºÎ
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
	// ±³È¯ÇÒ·¡? Y/N¿¡ ´ëÇÑ ÀÀ´ä
	//---------------------------------------------------------
	if (g_pTempInformation->GetMode() == TempInformation::MODE_TRADE_REQUEST)
	{
		//---------------------------------------------------------
		// Çã¿ë
		//---------------------------------------------------------
		if (accept)
		{
			code = CG_TRADE_PREPARE_CODE_ACCEPT;

			//---------------------------------------------------------
			// ±³È¯Ã¢À» ¶ç¿î´Ù.
			//---------------------------------------------------------
			UI_RunExchange( g_pTempInformation->Value1 );	// otherID ¼³Á¤
		}
		//---------------------------------------------------------
		// °ÅºÎ
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
// ±³È¯Ãë¼ÒÇÒ·¡?
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
	// ±³È¯ÇÒ·¡? Y/N¿¡ ´ëÇÑ ÀÀ´ä
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
		
		// ±³È¯ Ãë¼ÒÇÒ·¡?¸¦ Á¦°ÅÇÑ´Ù.
		UI_CloseExchangeCancel();
	}	
}

/*
//-----------------------------------------------------------------------------
//
// Game Option ¹Ù²Ù±â
//
//-----------------------------------------------------------------------------
// left = ¹Ù²î´Â option
// right = °ª
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

					// ¿ø·¡ÀÇ max volumeÀ¸·Î µ¹¸°´Ù.
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
		// Ä³¸¯ÅÍ HP¹Ù Åõ¸í/ºÒÅõ¸í 
		//-------------------------------------------------------------------
		case C_VS_UI_GAMEMENU_OPTION::PARTY_HPBAR_ALPHA :
			g_pUserOption->DrawTransHPBar = (BOOL)value;
		break;
		
	}
}


//-----------------------------------------------------------------------------
//
// GameOption ´Ý±â
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
	// ±×³É ÀúÀåÇØ¹ö¸°´Ù. - -;
	//---------------------------------------------------------------
	g_pUserOption->SaveToFile( FILE_INFO_USEROPTION );
}
*/

//-----------------------------------------------------------------------------
//
// Title Option ¹Ù²Ù±â
//
//-----------------------------------------------------------------------------
// left = ¹Ù²î´Â option
// right = °ª
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

					// ¿ø·¡ÀÇ max volumeÀ¸·Î µ¹¸°´Ù.
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
		// Ä³¸¯ÅÍ HP¹Ù Åõ¸í/ºÒÅõ¸í 
		//-------------------------------------------------------------------
//		case C_VS_UI_OPTION::PARTY_HPBAR_ALPHA :
//			g_pUserOption->DrawTransHPBar = (BOOL)value;
//		break;
	}
}


//-----------------------------------------------------------------------------
//
// Option ´Ý±â
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
	// ±×³É ÀúÀåÇØ¹ö¸°´Ù. - -;
	//---------------------------------------------------------------
	g_pClientConfig->SaveToFile( g_pFileDef->getProperty("FILE_INFO_CLIENTCONFIG").c_str());
	g_pUserOption->SaveToFile( g_pFileDef->getProperty("FILE_INFO_USEROPTION").c_str());

	if (g_Mode==MODE_MAINMENU)
	{
		if (CDirect3D::IsHAL())
		{
			// 3D°¡¼Ó ÁßÀÎµ¥.. °¡¼Ó ²ô´Â °æ¿ì
			if (!g_pUserOption->Use3DHAL)
			{
				g_Mode = MODE_CHANGE_OPTION;
			}
		}
		else
		{
			// 3D°¡¼Ó ¾Æ´Ñµ¥.. °¡¼Ó ÇÏ´Â °æ¿ì
			if (g_bEnable3DHAL && g_pUserOption->Use3DHAL)
			{
				g_Mode = MODE_CHANGE_OPTION;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
// Bookcase ´Ý±â
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
// Briefing ´Ý±â
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
// Computer ´Ý±â
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
// Tutorial¿¡¼­ ºüÁ®³ª°¥¶§, ¹«±â ÇÏ³ª ¼±ÅÃ
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

	int arms = left;	// 0: µµ 1: °Ë 2: ¸ÞÀÌ½º 3: ½ÊÀÚ°¡ 4: AR 5: TR 6: SMG 7:SG

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
// Desc Dialog ´ÝÀ» ¶§
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
// Elevator´ÝÀ» ¶§
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
// Elevator ¼±ÅÃÇÒ ¶§
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

	
	// left 0 : 4Ãþ ....
	//		1 : 3
	//		2 : 2
	//		3 : 1
	//		4 : ÁöÇÏ 1Ãþ
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

			// zoneID·Î ÀÌµ¿ÇÑ´Ù.
			//-----------------------------------------------------
			// Packet º¸³»±â
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
// Server ¼±ÅÃÇÒ ¶§
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
	// ServerÁ¤º¸ °»½Å
	//-----------------------------------------------------
	if (g_pServerInformation!=NULL)
	{
		//-----------------------------------------------------
		// ¼­¹ö ÀÌ¸§ ÀÐ¾î¿À±â
		//-----------------------------------------------------
		const ServerGroup* pGroup = g_pServerInformation->GetData( selectedGroup );

		if (pGroup!=NULL)
		{
			const char* pGroupName = pGroup->GetGroupName();

			//-----------------------------------------------------
			// ÇöÀç ¼±ÅÃµÇ¾î ÀÖ´Â server¶û ´Ù¸£¸é...
			//-----------------------------------------------------
			if (pGroupName != g_pServerInformation->GetServerGroupName())
			{
				char str[80];
				strcpy(str, pGroupName);	
				
				// UI¿¡ ¼³Á¤
				//gC_vs_ui.SetServerDefault( str, selectedGroup );

			
				//-----------------------------------------------------
				// Packet º¸³»±â
				//-----------------------------------------------------
				#ifdef CONNECT_SERVER
					CLChangeServer _CLChangeServer;

					_CLChangeServer.setServerGroupID( selectedGroup );

					g_pSocket->sendPacket( &_CLChangeServer );

					gC_vs_ui.CharManagerDisable();
				#endif

				// ServerÁ¤º¸¿¡ ¼³Á¤
				g_pServerInformation->SetServerGroupName( pGroupName );			
			}
		}
	}
}
*/

//-----------------------------------------------------------------------------
//
// Server ¼±ÅÃÇÒ ¶§
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
//	// °ËÁõ¹ÞÀ»°Ô ¾ø´Â °æ¿ì
//	//-----------------------------------------------------------------
//	if (g_pTempInformation->GetMode()==TempInformation::MODE_NULL
//		&& g_pPlayer->IsItemCheckBufferNULL())
//	{
//		if (gC_vs_ui.IsRunningExchange())
//		{
//			// ±³È¯ Áß¿¡´Â ±×³É Mouse¿¡ ºÙÀÎ´Ù.
//			Execute_UI_ITEM_PICKUP_FROM_INVENTORY(left, right, void_ptr);
//		}
//		else
//		{
//			MItem* pItem = (MItem*)void_ptr;
//
//			//---------------------------------------------------------
//			// ½×ÀÏ ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÌ°í..
//			// QuickSlotÀÌ ÀÖ´ÂÁö È®ÀÎ
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
//				// QuickSlotÀÇ ¾îµð¿¡ µé¾î°¥ ¼ö ÀÖÀ»±î?
//				//---------------------------------------------------------
//				for (int i=0; i<num; i++)
//				{
//					MItem* pQuickItem = g_pQuickSlot->GetItem( i );
//
//					//---------------------------------------------------------
//					// ¾Æ¹«°Íµµ ¾ø´Â °÷ÀÌ¸é ±×³É ³ÖÀ¸¸é µÈ´Ù.
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
//					// ¹º°¡ ÀÖÀ¸¸é.. ±×°÷¿¡ ½×ÀÏ ¼ö ÀÖ´ÂÁö ¾Ë¾Æº»´Ù.
//					//---------------------------------------------------------
//					else
//					{
//						//--------------------------------------------------------
//						// ½×ÀÏ ¼ö ÀÖ´Â itemÀÎÁö °ËÁõÇØ ÁØ´Ù.
//						//--------------------------------------------------------
//						if (pQuickItem->GetItemClass()==pItem->GetItemClass()
//							&& pQuickItem->GetItemType()==pItem->GetItemType())
//						{
//							//----------------------------------------------------
//							// ´õÇÑ °³¼ö°¡ max¸¦ ³ÑÁö ¾Ê¾Æ¾ß ÇÑ´Ù.
//							//----------------------------------------------------
//							addTotal = pQuickItem->GetNumber() + pItem->GetNumber();
//							if ( addTotal <= pQuickItem->GetMaxNumber() )
//							{
//								// i¹øÂ°¿¡ Ãß°¡ °¡´ÉÇÏ´Ù°í ÆÇ´ÜÇÑ´Ù.								
//								addIndex = i;
//
//								//---------------------------------------------------
//								// InventoryÀÇ ¾ÆÀÌÅÛÀº Á¦°ÅÇÑ´Ù.
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
//					// ½ÇÁ¦·Î´Â Inventory --> Mouse --> QuickItemÀÌ´Ù.
//					//---------------------------------------------------------
//						//---------------------------------------------------
//						// Inventory¿¡ ÀÖ´ø itemÀ» mouse¿¡ ºÙ¿´´Ù(-_-;)°í
//						// server·Î packetÀ» º¸³½´Ù.
//						//---------------------------------------------------
//						CGAddInventoryToMouse _CGAddInventoryToMouse;
//						_CGAddInventoryToMouse.setObjectID( itemID );
//						_CGAddInventoryToMouse.setX( itemX );
//						_CGAddInventoryToMouse.setY( itemY );
//						
//						g_pSocket->sendPacket( &_CGAddInventoryToMouse );
//
//						//---------------------------------------------------
//						// mouse¿¡ ÀÖ´ø itemÀ» QuickSlot¿¡ Ãß°¡Çß´Ù°í
//						// server·Î packetÀ» º¸³½´Ù.
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
//			// Quick slot¿¡ ¾È µé¾î°¡´Â °Å¸é..
//			// Gear·Î ³Ö¾î¹ö¸®ÀÚ. -_-;
//			//---------------------------------------------------------
//			else
//			{
//				//---------------------------------------------------------
//				// ÀÏ´Ü..  mouse·Î µé°í..
//				//---------------------------------------------------------
//				g_pInventory->RemoveItem( pItem->GetID() );
//				Execute_UI_ITEM_PICKUP_FROM_INVENTORY(left, right, void_ptr);
//
//				int maxSlot;
//
//				//---------------------------------------------------------
//				// gear¿¡ µé¾î°¥ ¼ö ÀÖ´ÂÁö Ã¼Å©
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
//						// ºó °÷ÀÌ¸é ¹Ù·Î ³Ö¾î¹ö¸®¸é µÈ´Ù.
//						// ºó °÷ÀÌ ¾Æ´Ï¶ó¸é.. ´ÙÀ½°É Ã£´Â´Ù.
//						if (pGearItem==NULL)
//						{
//							break;
//						}							
//					} 
//				}
//				
//				//---------------------------------------------------------
//				// gear¿¡ ³ÖÀÚ..
//				//---------------------------------------------------------
//				if (addSlot != -1)
//				{
//					Execute_UI_ITEM_DROP_TO_GEAR(addSlot, 0, pChangeItem);
//				}
//			}
//		}		
//	}
//	//-----------------------------------------------------------------
//	// °ËÁõ ¹Þ¾Æ¾ßÇÒ ´Ù¸¥ ¾ÆÀÌÅÛÀÌ ÀÖ´Â °æ¿ì
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
// ÆÄÆ¼ ¿äÃ»¹ÞÀº°Å ´ÝÀ»¶§
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
			// ÆÄÆ¼ ÃÊ´ë¸¦ °ÅºÎÇÑ´Ù.
			CGPartyInvite _CGPartyInvite;
			_CGPartyInvite.setTargetObjectID( g_pTempInformation->PartyInviter );
			_CGPartyInvite.setCode( CG_PARTY_INVITE_REJECT );	

			g_pSocket->sendPacket( &_CGPartyInvite );		
	}
}

//-----------------------------------------------------------------------------
//
// ÆÄÆ¼ ¿äÃ»¹ÞÀº°Å Ãë¼Ò
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
	// ÆÄÆ¼ÇÒ·¡? Y/N¿¡ ´ëÇÑ ÀÀ´ä
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
		
		// ÆÄÆ¼ Ãë¼ÒÇÒ·¡?¸¦ Á¦°ÅÇÑ´Ù.
		UI_ClosePartyCancel();
	}	
}

//-----------------------------------------------------------------------------
//
// ÆÄÆ¼ ¿äÃ» ÀÀ´ä
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
	// ±³È¯ÇÒ·¡? Y/N¿¡ ´ëÇÑ ÀÀ´ä
	//---------------------------------------------------------
	//if (g_pTempInformation->Mode == TempInformation::MODE_PARTY_REQUEST)
	{
		//---------------------------------------------------------
		// Çã¿ë
		//---------------------------------------------------------
		if (accept)
		{
			code = CG_PARTY_INVITE_ACCEPT;
		}
		//---------------------------------------------------------
		// °ÅºÎ
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
// ÆÄÆ¼ Å»Åð/Ãß¹æ
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


	// ÆÄÆ¼ Å»Åð Ãß¹æ 
	// left :: -1 == Å»Åð, 0~4 == Ãß¹æ
	BOOL bLeftParty = (left==-1);

	if (g_pPlayer->IsWaitVerifyNULL())
	{
		//-----------------------------------------------------------------
		// ³»°¡ ÆÄÆ¼¸¦ ¶°³ª´Â °æ¿ì
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
		// ³»°¡ ´Ù¸¥ ´©±¸¸¦ Ãß¹æÇÏ´Â °æ¿ì
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
// ÆÄÆ¼ Å»Åð/Ãß¹æ
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


	// ´ÙÀÌ-_-¿äÃ» ´Ý±â left == TRUE : timeout
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
			// ÀÌ°Å¸»°í Verify¸¦ ÇÏ³ª ¸¸µé¾î¾ß µÇ´Âµ¥
			// ÄÄÆÄÀÏ ÇÏ±â ½È¾î¼­ ÀÏ´Ü... - -;
		g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_RESURRECT );

		gC_vs_ui.FinishRequestDie();
		gC_vs_ui.FinishRequestResurrect();
		gC_vs_ui.CloseRequestShrineMinimap();
	}
}

//-----------------------------------------------------------------------------
//
// ¼­¹ö ¼±ÅÃÇÒ ¶§
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
	// World¸¦ ¼±ÅÃÇÑ °æ¿ì
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
	// Server¸¦ ¼±ÅÃÇÑ °æ¿ì
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
// server ¼±ÅÃ ´ÝÀ» ¶§
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
	// group¼±ÅÃ¿¡¼­ backÇÏ¸é ÃÊ±âÈ­¸éÀ¸·Î
	//------------------------------------------------------------
	if (bSelectGroup)
	{
		SetMode( MODE_MAINMENU );
	}
	//------------------------------------------------------------
	// server¼±ÅÃ¿¡¼­ backÇÏ¸é group¼±ÅÃÀ¸·Î..
	//------------------------------------------------------------
	else
	{
		UI_SetWorldList();

		SetMode( MODE_WAIT_SELECT_WORLD );
	}	
}

//-----------------------------------------------------------------------------
//
// Ä³¸¯ÅÍ ÀÌ¸§ È®ÀÎÇÒ¶§
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
			// ÀÌ¸§ ±æÀÌ Ã¼Å©
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
					// Àß¸øµÈ IDÀÎ °æ¿ì
					//---------------------------------------------						
					g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
					bAllOK = FALSE;
				}
				else
				{
					char strName[80];
					strcpy(strName, pName);

					// ¾È ÁÁÀº ¸»ÀÌ µé¾îÀÖ´Â °æ¿ì´Â Çã¿ëÀÌ ¾ÈµÈ´Ù
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

					// ÀÓ½Ã·Î.. -_-;;
					g_pUserInformation->WhisperID = pName;
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
// Áö·Ú/ÆøÅº ¸¸µé²¨ ¼±ÅÃÇÒ¶§
//
//-----------------------------------------------------------------------------
/*
void
UIMessageManager::Execute_UI_SELECT_EXPLOSIVE(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_SELECT_EXPLOSIVE");
	
	int type = left;		// Áö·Ú(2) / ÆøÅº(3)
	int itemType = right;	// Á¾·ù(Áö·Ú:0~3, ÆøÅº:0~5)


	//--------------------------------------------------------
	// °ËÁõ¹ÞÀ»°Ô ¾ø´Â °æ¿ì
	//--------------------------------------------------------
	if (g_pPlayer->IsItemCheckBufferNULL()
		&& g_pTempInformation->Mode==TempInformation::MODE_NULL)
	{
		//--------------------------------------------------------
		// Á¤»óÀûÀÎ °æ¿ì
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

			// item ±â¾ï
			g_pTempInformation->Mode = TempInformation::MODE_SKILL_MAKE_ITEM;
			g_pTempInformation->Value1 = itemClass;
			g_pTempInformation->Value2 = itemType;

			// ¸ø ¿òÁ÷ÀÌ°Ô ¸·±â
			UI_LockItem();
			
			// ¼±ÅÃÃ¢À» ´Ý´Â´Ù.
			UI_CloseSelectExplosive();
		}
		//--------------------------------------------------------
		// ¹º°¡ Àß¸øµÈ °æ¿ì
		//--------------------------------------------------------
		else
		{
			// ¸¸µé²¨¸®¸¦ Á¦´ë·Î ¼±ÅÃÇÏ½Ã¶ó~~
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
// Áö·Ú/ÆøÅº ¼±ÅÃÇÏ´Â°Å ´ÝÀ» ¶§
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
		// ´Ù½Ã ¹º°¡¸¦?¼±ÅÃÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
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
		// ´Ù½Ã ¹º°¡¸¦?¼±ÅÃÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
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
//		// ´Ù½Ã ¹º°¡¸¦?¼±ÅÃÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
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

//add by viva
void
UIMessageManager::Execute_UI_CLOSE_FRIEND_CHATTING_INFO(int left, int right, void* void_ptr)
{
	C_VS_UI_FRIEND_CHATTING_INFO* pInfo = (C_VS_UI_FRIEND_CHATTING_INFO*)void_ptr;
	gC_vs_ui.CloseFriendChattingInfo(pInfo);
}

void
UIMessageManager::Execute_UI_OPEN_FRIEND_CHATTING_INFO(int left, int right, void* void_ptr)
{
	C_VS_UI_FRIEND_INFO::FRIEND_LIST* pList = (C_VS_UI_FRIEND_INFO::FRIEND_LIST*)void_ptr;
	gC_vs_ui.OpenFriendChattingInfo(pList);
}

////////////////////////friend message////////////////////////////////////////////////////////
void UIMessageManager::Execute_UI_FRIEND_CHATTING_SEND_MESSAGE(int left, int right, void* void_ptr)
{
	C_VS_UI_FRIEND_CHATTING_INFO::FRIEND_SEND_MESSAGE* pMessage = (C_VS_UI_FRIEND_CHATTING_INFO::FRIEND_SEND_MESSAGE*)void_ptr;
	GCFriendChatting gcFriend;
	gcFriend.setCommand(CG_MESSAGE);
	gcFriend.setPlayerName(pMessage->Name);
	gcFriend.setMessage(pMessage->Message);
	g_pSocket->sendPacket( &gcFriend );
	delete pMessage;
}
void UIMessageManager::Execute_UI_FRIEND_CHATTING_UPDATE(int left, int right, void* void_ptr)
{
	GCFriendChatting gcFriend;
	gcFriend.setCommand(CG_UPDATE);
	g_pSocket->sendPacket( &gcFriend  );
}
void UIMessageManager::Execute_UI_FRIEND_CHATTING_ADD_FRIEND(int left, int right, void* void_ptr)
{
	if(!gC_vs_ui.IsFriendWaitAskRuning())
	{
		char* pName = (char*)void_ptr;
		GCFriendChatting gcFriend;
		gcFriend.setCommand(CG_ADD_FRIEND);
		gcFriend.setPlayerName(string(pName));
		g_pSocket->sendPacket( &gcFriend );
//		Delete(pName);
	}
//	gC_vs_ui.SendAddFriend(pName);
}

//////////////////////////ask_friend_request///////////////////////////////////////////////////
void UIMessageManager::Execute_UI_FRIEND_REQUEST_ACCEPT(int left, int right, void* void_ptr)
{
	C_VS_UI_ASK_DIALOG* pDialog = (C_VS_UI_ASK_DIALOG*)void_ptr;
	char* pName = (char*)pDialog->GetpTemporayValue();
	bool IsAgree = (bool)left;
	GCFriendChatting gcFriend;

	if(left == 0)
		gcFriend.setCommand(CG_ADD_FRIEND_AGREE);
	else if(left == 1)
		gcFriend.setCommand(CG_ADD_FRIEND_REFUSE);
	else if(left == 2)
		gcFriend.setCommand(CG_ADD_FRIEND_BLACK);

	gcFriend.setPlayerName(string(pName));
	g_pSocket->sendPacket(&gcFriend);
	
	DeleteNew(pDialog);
	DeleteNew(pName);
}
////////////////////////////////////////////ask_friend_close///////////////////////////////////
void UIMessageManager::Execute_UI_FRIEND_ASK_CLOSE(int left, int right, void* void_ptr)
{
	C_VS_UI_ASK_DIALOG* pDialog = (C_VS_UI_ASK_DIALOG*)void_ptr;
	char* pName = (char*)pDialog->GetpTemporayValue();
	if(pDialog->GetAskType() == C_VS_UI_ASK_DIALOG::ASK_FRIEND_WAIT)
		gC_vs_ui.setFriendWaitAskNull();

	DeleteNew(pDialog);
	DeleteNew(pName);
}
//////////////////////////////////////////////ask_friend_delete_ask//////////////////////////////
void UIMessageManager::Execute_UI_FRIEND_DELETE_ASK(int left, int right, void* void_ptr)
{
	C_VS_UI_FRIEND_INFO::FRIEND_LIST* pList = (C_VS_UI_FRIEND_INFO::FRIEND_LIST*)void_ptr;
	char* pName = new char[pList->Name.size()+1];
	strcpy(pName, pList->Name.c_str());
	gC_vs_ui.RunFriendDeleteAsk(pName);
}
////////////////////////////////////////////////ask_friend_delete_accept/////////////////////////
void UIMessageManager::Execute_UI_FRIEND_DELETE_ACCEPT(int left, int right, void* void_ptr)
{
	C_VS_UI_ASK_DIALOG* pDialog = (C_VS_UI_ASK_DIALOG*)void_ptr;
	char* pName = (char*)pDialog->GetpTemporayValue();
	GCFriendChatting gcFriend;
	gcFriend.setCommand(CG_FRIEND_DELETE);
	gcFriend.setPlayerName(string(pName));
	g_pSocket->sendPacket(&gcFriend);

	DeleteNew(pDialog);
	DeleteNew(pName);
}
//end

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
UIMessageManager::Execute_UI_REGIST_GUILD_MEMBER(int left, int right, void* void_ptr)			// void_ptr = introduction max:150byte Ã¢ ´Ý¾ÆÁÙ°Í!
{

	if(void_ptr == NULL)
	{
		// ¿¡·¯¸Þ¼¼Áö Ãâ·Â
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
UIMessageManager::Execute_UI_REGIST_GUILD_TEAM(int left, int right, void* void_ptr)			// left = TEAM_NAME, void_ptr = introduction max:150byte Ã¢ ´Ý¾ÆÁÙ°Í!
{

	if(void_ptr == NULL || (char *)left == NULL)
	{
		// ¿¡·¯¸Þ¼¼Áö Ãâ·Â
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
					// surfaceÀÇ Å©±â°¡ default ProfileÅ©±â¿Í ´Ù¸£´Ù¸é
					// size¸¦ º¯°æ½ÃÄÑÁà¾ß ÇÑ´Ù..	
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
// mouseÀÇ Item(EVENT_STAR)À¸·Î InventoryÀÇ ItemÀ» EnchantÇÒ ¶§
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

	// gpC_mouse_pointer->GetPickUpItem()		// µé°í ÀÖ´Â °Í
	//void_ptr = MItem *		// µé¾î°¥ °÷
	// InventoryÁÂÇ¥(left,right)
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

	// ±³È¯ÁßÀÏ¶§´Â EnchantÇÒ ¼ö ¾ø´Ù!
	if (g_pTradeManager!=NULL)
		return;
	
	//g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_INSERT_FROM_INVENTORY );
	
	//----------------------------------------------------
	// Server¿¡ Á¢¼ÓÇÑ °æ¿ì
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
	// ÀÌ¹Ì ÃßÀû¹öÆ°À» ´©¸¥»óÅÂÀÏ°æ¿ì
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
//	// Player°¡ ±â´Ù¸®´ø skillÀÇ ¼º°øÀ¯¹«¸¦ °ËÁõ¹Þ¾Ò´Ù.
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
//	// Item LockÀ» Ç¬´Ù.
//	//------------------------------------------------------------------
//	if (g_pPlayer->GetItemCheckBufferStatus()==MPlayer::ITEM_CHECK_BUFFER_SKILL_TO_INVENTORY)
//	{
//		g_pPlayer->ClearItemCheckBuffer();
//	}

	gC_vs_ui.CloseTraceWindow();
}

// ³Ý¸¶ºí¿ë ¼öÁ¤
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
		// ºóÇ×¸ñÀÌ ÀÖ½À´Ï´Ù.
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
	// °ËÁõÇÒ°Ô ÀÖÀ¸¸é ¸®ÅÏ.
	if(g_pTempInformation->GetMode() != TempInformation::MODE_NULL)
	{
		DEBUG_ADD("Excute_UI_SEND_BRING_FEE TempInformation is not NULL");
		return;
	}
	// ¼¼±Ý °¡Á®¿À±â
	DEBUG_ADD("Excute_UI_SEND_BRING_FEE");
	// °¡Á®¿À±â ÇßÀ»¶© °ËÁõ ÆÐÅ¶ ³¯¶ó¿À±âÀü±îÁø ³ÀµÎÀÚ.

	g_pTempInformation->SetMode(TempInformation::MODE_WAIT_BRING_FEE);
	g_pTempInformation->Value1 = left;

	CGWithdrawTax _CGWithdrawTax;

	_CGWithdrawTax.setGold((DWORD)left);
	g_pSocket->sendPacket( &_CGWithdrawTax );

	DEBUG_ADD("Excute_UI_SEND_BRING_FEE OK");
}

void UIMessageManager::Excute_UI_CLOSE_BRING_FEE_WINDOW(int left,int right, void* void_ptr)
{
	// °ËÁõÇÒ°Ô ¾øÀ»°æ¿ì
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
// Gear¿¡¼­ ItemÀ» »ç¿ëÇÑ °æ¿ì
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
				// ÀÚ±âÁ¾Á· ¾ÆÀÌÅÛ¸¸ ¾²Àð-¤µ-;
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
		// Á¾·á..
		SetMode( MODE_QUIT );
		
		char str[256];
		
		GetWindowsDirectory(
			str,  // address of buffer for Windows directory
			255        // size of directory buffer
			);
		
		sprintf(str, "%s\\Explorer.exe", str);
		
		CDirectDraw::GetDD()->RestoreDisplayMode();
		
	//	_spawnl(_P_NOWAIT, str, "Explorer.exe", g_pClientConfig->URL_HOMEPAGE_BILING.GetString(), NULL);
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
	// left = step, right = »óÇ° ID
	
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
	// gpC_mouse_pointer->GetPickUpItem()		// µé°í ÀÖ´Â °Í
	//void_ptr = MItem *		// µé¾î°¥ °÷
	// InventoryÁÂÇ¥(left,right)
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

	// ±³È¯ÁßÀÏ¶§´Â EnchantÇÒ ¼ö ¾ø´Ù!
	if (g_pTradeManager!=NULL)
		return;
	
	//g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_INSERT_FROM_INVENTORY );
	
	//----------------------------------------------------
	// Server¿¡ Á¢¼ÓÇÑ °æ¿ì
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

	// 2004, 11, 8, sobeit modify start - ¾Æ¿ì½ºÅÍÁî ½ºÅ³¿¡ µû¶ó 0·¹º§·Îµµ ¸¸µé¼ö ÀÖ´Ù.
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
	case 0 :				// ´ÙÚ´Ù!!!!
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
	// ³ª¸¦ Á×¿©ÁÒ
	gC_vs_ui.CloseAllDialog();
	
	switch(left)
	{
	case 10 :						// ArrowTile ÇÏ´Ù°¡ Á×¾úÀ»¶§
		break;
	case 11 :						// CrazyMine ÇÏ´Ù°¡ Á×¾úÀ»¶§			
		break;
	}
	
	// ¼­¹ö¿¡ ³ª Á×¿©ÁÒ ÆÐÅ¶À» º¸³½´Ù.
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
	// ÃÊº¸Á¸
	case 1:
		zoneID = 1122;
		break;

	case 2:
		zoneID = 1122;
		break;
		
	// ·¹º§ ÀüÀï ½ÃÀÛ
	case 4:
		zoneID = 1131;
		break;

	// Á¾Á· ÀüÀï ½ÃÀÛ
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
	// Random-_- À¸·Î º¸³»ÀÚ. °­Á¦·Î ´ÝÀº °æ¿ìÀÌ´Ù. 

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
	// °×ºíÇÏ´Âµ¿¾È ¾ÆÀÌÅÛ ¸ø»©°Ô
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
	
	// º¸°üÇÔ ÁßÁö
	if (g_pStorage!=NULL)
	{
		delete g_pStorage;
		g_pStorage = NULL;
	}

	// storage¸¦ ¾ø¾Ø´Ù.
	gC_vs_ui.SetPetStorage( NULL );

//	// ´Ù½Ã ¹º°¡¸¦?¼±ÅÃÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
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
		// sms ÁÖ¼Ò ¿äÃ» ÆÐÅ¶ º¸³¿
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

	// µî·Ï ¿äÃ» ÆÐÅ¶ º¸³¿
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
	// ¼­¹ö ¼¼ÆÃ ³¡³ª°í Å×½ºÆ® ÈÄ return Ç®ÀÚ..
//	return;
	// left : name, right : item ID(0=¿ï¹ö¸°) void_ptr:penitem pointer
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
//			// ¾È ÁÁÀº ¸»ÀÌ µé¾îÀÖ´Â °æ¿ì´Â Çã¿ëÀÌ ¾ÈµÈ´Ù
//			if (g_pChatManager->RemoveCurse(strName))
//			{
//				g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[STRING_USER_REGISTER_INVALID_ID].GetString() );
//				bAllOK = FALSE;
//			}
//		}
		g_pChatManager->RemoveCurse(szTemp );

		// ÀÌÂë¿¡¼­ effect status¸¦ °Ë»ö ÇØ¼­ ¾ÆÀÌÅÛ »ç¿ë ÆÐÅ¶À» º¸³»´Â°Ô ÁÁÀ»µí..
		MItem* pItem = NULL;	
		
		if(right != 0)
			pItem = g_pInventory->GetItemToModify( right );//(MItem*)void_ptr;
		
		if(pItem!= NULL || right == 0)
		{
			CGModifyNickname	_CGModifyNickname; 
			_CGModifyNickname.setNickname(std::string((char *)szTemp));

			if(right != 0)
			{ 
				// ¸ÕÀú ¾ÆÀÌÅÛ ½è´Ù°í ÆÐÅ¶ º¸³»°í - ¾Èº¸³»±â·ç ¼³Á¤
			//	Execute_UI_ITEM_USE(pItem->GetID(), 0, (void*)pItem);
				// ´Ð³×ÀÓ Ã¼ÀÎÁö ÆÐÅ¶À» º¸³»Àå..
				_CGModifyNickname.setNicknameID(pItem->GetID());
				g_pSocket->sendPacket( &_CGModifyNickname );

				g_pTempInformation->SetMode(TempInformation::MODE_NICKNAME_CHANGE_CUSTOM);
				g_pTempInformation->Value1 = left;					// ¹Ù²ï ´Ð³×ÀÓ ¾ÆÀÌµð
				g_pTempInformation->Value2 = pItem->GetItemType();  // ¾ÆÀÌÅÛ Å¸ÀÔ

				if(25 != g_pTempInformation->Value2)  // ½Ã°£Á¦ ¤¿¤·ÀÌÅÛ
					g_pPlayer->SetItemCheckBuffer( pItem, MPlayer::ITEM_CHECK_BUFFER_USE_FROM_INVENTORY);
			}
			else
			{
				// ¿ï¹ö¸°ÀÏ¶© ¾ÆÀÌÅÛ ¾øÀÌ ¹Ù²Û´ç..
				_CGModifyNickname.setNicknameID(0);
				g_pSocket->sendPacket( &_CGModifyNickname );
				g_pTempInformation->SetMode(TempInformation::MODE_NICKNAME_CHANGE_CUSTOM);
				g_pTempInformation->Value1 = left;					// ¹Ù²ï ´Ð³×ÀÓ ¾ÆÀÌµð
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
		// pItem ÀÌ Ä³¸¯ÅÍ ´Ð³×ÀÓ ¹Ù²Ù´Â ¾ÆÀÌÅÛ ÀÌ¸é
			// 1È¸¿ë (2°¡Áö) - ±âÁ¸´Ð³×ÀÓ ¼öÁ¤, »õ·Î¿î ´Ð³×ÀÓ Ãß°¡
			// ½Ã°£Á¦ (1°¡Áö) - Æ¯Á¤ ½Ã°£µ¿¾È ±âÁ¸ ´Ð³×ÀÓ ¹«Á¦ÇÑ ¼öÁ¤°¡´É
		// pItemÀÌ Æê ´Ð³×ÀÓ ¹Ù²Ù´Â ¾ÆÀÌÅÛÀÌ¸é ÆêÀÌ ¼ÒÈ¯ »óÅÂ¿©¾ß¸¸ »ç¿ë °¡´É
			// ¿ï¹ö¸°ÀÇ °æ¿ì ¾ÆÀÌÅÛÀÌ ÇÊ¿ä ¾øÀ½
		
		if(pItem->GetItemType() == 22 || pItem->GetItemType() == 25) // À¯Àú ´Ð³×ÀÓ ¼öÁ¤
			gC_vs_ui.RunNamingChange(pItem, (char*)g_pPlayer->GetNickName().c_str());
//		// ´Ð³×ÀÓ Ãß°¡
		else if(pItem->GetItemType() == 24) // extra naming pen // ³×ÀÌ¹Ö Ãß°¡ ¾ÆÀÌÅÛ
			gC_vs_ui.RunNamingChange(pItem, (char*)(*g_pGameStringTable)[UI_STRING_MESSAGE_ADD_PLAYER_NICKNAME].GetString());
//		// ½Ã°£Á¦ ¾ÆÀÌÅÛ
//		else if(pItem->GetItemType() == 25) // limited naming pen // ³×ÀÌ¹Ö º¯°æ ½Ã°£ ¾ÆÀÌÅÛ
//			gC_vs_ui.RunNamingChange(pItem, (char*)g_pPlayer->GetNickName().c_str());
		else if(pItem->GetItemType() == 23) // Æê ´Ð³×ÀÓ ¼öÁ¤
		{
			MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(g_pPlayer->GetPetID());
			if(NULL != pFakeCreature)
			{
				MPetItem *pPetItem = pFakeCreature->GetPetItem();
				if(pPetItem != NULL)
				{
					if(2 == pPetItem->GetItemType()) // ¿ï¹ö¸° ÀÏ¶© Ææ ¾ÆÀÌÅÛ ÇÊ¿ä ¾øÀ½
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
// GQuest Inventory¿¡¼­ ItemÀ» »ç¿ëÇÑ °æ¿ì
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
	
	if(g_pUserInformation->GuildGrade != 1) // ±æµå¸¶½ºÅÍ°¡ ¾Æ´Ï¶ó¸é
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

	if(g_pUserInformation->GuildGrade != 1) // ±æµå¸¶½ºÅÍ°¡ ¾Æ´Ï¶ó¸é
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
	if(g_pUserInformation->GuildGrade != 1) // ±æµå¸¶½ºÅÍ°¡ ¾Æ´Ï¶ó¸é
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
	g_pStorage2->SetCurrent( 0 );	// È®ÀÎ¿ë
 
 	int slot = right;

	
	const MItem* pStorageItem = g_pStorage2->GetItem( slot );
	//TYPE_OBJECTID mouseItemID = pMouseItem->GetID();		
	
	// Event GiftBox ¾ÆÀÌÅÛÀÎ °æ¿ì ¸ø ³õ´Â´Ù.
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
				
				// ½½¶ùÀÌ ºñ¾úÀ¸¸é °Á~ ³Ö´Â´Ù
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

	 DEBUG_ADD("[UI] Execute_UI_MY_STORE_INFO"); // ¾ÆÀÌÅÛ Á¤º¸ ¸®½ºÆ® 
	 
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
	MItem* pItem = (MItem*)void_ptr;		// inventory¿¡ ÀÖ´Â ¾ÆÀÌÅÛ(NULLÀÏ ¼öµµ ÀÖ´Ù)
	
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
			// Event GiftBox ¾ÆÀÌÅÛÀÎ °æ¿ì ¸ø ³õ´Â´Ù.
			
			if (!pItem->IsUniqueItem() && !pItem->IsQuestItem())
			{
				//-----------------------------------------------------
				// ¾ÆÀÌÅÛÀ» ÆÈ±â À§ÇÑ packetÀ» º¸³½´Ù.
				//-----------------------------------------------------
					CGShopRequestSell	_CGShopRequestSell;

					_CGShopRequestSell.setObjectID( (*g_pPCTalkBox).GetNPCID() );		// NPC ID	
					_CGShopRequestSell.setOpCode( SHOP_REQUEST_SWAP_ADVANCEMENT_ITEM );
					_CGShopRequestSell.setItemObjectID( pItem->GetID() );	// Item Object ID

					g_pSocket->sendPacket( &_CGShopRequestSell );
			
						
					//-------------------------------------------------
					// °ËÁõÀ» À§ÇÑ Temp Information¼³Á¤
					//-------------------------------------------------
					(*g_pTempInformation).Mode	= TempInformation::MODE_SHOP_SELL;
					(*g_pTempInformation).pValue = (void*)pItem;

					//-------------------------------------------------
					// ´Ù¸¥ ¾ÆÀÌÅÛ¿¡ Á¢±Ù ¸øÇÏµµ·Ï..
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

// 2005, 1, 11, sobeit add start - ºÒ¿ìÀÌ¿ô µ½±â ¼º±Ý °ü·Ã
void
UIMessageManager::Execute_UI_CAMPAIGN_HELP(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_CAMPAIGN_HELP");
	
	// ¼º±Ý °ü·Ã ÆÐÅ¶ º¸³»±â - ±Ý¾× * 10000

	if(left>0)
	{
		CGDonationMoney _CGDonationMoney;
		_CGDonationMoney.setGold(left*10000);
		_CGDonationMoney.setDonationType(right);
		g_pSocket->sendPacket( &_CGDonationMoney );
	}
// 2005, 1, 11, sobeit add end - ºÒ¿ìÀÌ¿ô µ½±â ¼º±Ý °ü·Ã
}

// 2005, 1, 20, sobeit add start - Äù½ºÆ® °ü·Ã 
void
UIMessageManager::Execute_UI_RUN_NEXT_GQUEST_EXCUTE_ELEMENT(int left, int right, void* void_ptr)
{
	// quest element List¿¡ ÀúÀåµÈ ´ÙÀ½ element¸¦ ½ÇÇà ÇÑ´Ù.
	DEBUG_ADD("[UI] Execute_UI_RUN_NEXT_GQUEST_EXCUTE_ELEMENT");
	gC_vs_ui.RunNextGQuestExcuteElement();
}

void
UIMessageManager::Execute_UI_GQUEST_SET_ACTION(int left, int right, void* void_ptr)
{
	DEBUG_ADD("[UI] Execute_UI_GQUEST_SET_ACTION");
	// Æ¯Á¤ element ½ÇÇà ½Ã Æ¯Á¤ µ¿ÀÛÀ» ÇÑ´Ù.
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
	// Æ¯Á¤ Äù½ºÆ® ¿Ï·á½Ã °ü·Ã ÀÌº¥Æ®¸¦ ÁøÇà ÇÑ´Ù.
	MEvent event;
	event.eventID = EVENTID_ADVANCEMENT_QUEST_ENDING;
	event.eventType = EVENTTYPE_ZONE;
	event.eventFlag = EVENTFLAG_ONLY_EVENT_BACKGROUND | EVENTFLAG_NOT_DRAW_UI | EVENTFLAG_NOT_DRAW_CREATURE | EVENTFLAG_NOT_DRAW_INFORMATION | EVENTFLAG_NOT_DRAW_CREATURE_SHADOW | EVENTFLAG_NOT_DRAW_ITEM | EVENTFLAG_NOT_DRAW_MOUSE_POINTER | EVENTFLAG_NOT_DRAW_EFFECT | EVENTFLAG_DENY_INPUT | EVENTFLAG_NOT_FADE_SCREEN | EVENTFLAG_NOT_PLAY_SOUND;
	switch(left)
	{
	case 1: // ½½·¹ÀÌ¾î ½ÂÁ÷ Äù½ºÆ® ¿£µù ÀÌº¥Æ®
		event.parameter1 = 0;
		event.parameter4 = 1;
		g_pEventManager->AddEvent(event);
		break;
	case 2: // ¹ìÆÄÀÌ¾î ½ÂÁ÷ Äù½ºÆ® ¿£µù ÀÌº¥Æ®
		event.parameter1 = 0;
		event.parameter4 = 3;
		g_pEventManager->AddEvent(event);
		break;
	case 3: // ¾Æ¿ì½ºÅÍÁî ½ÂÁ÷ Äù½ºÆ® ¿£µù ÀÌº¥Æ®
		event.parameter1 = 0;
		event.parameter4 = 5;
		g_pEventManager->AddEvent(event);
		break;
	}
	
}
// 2005, 1, 20, sobeit add end - Äù½ºÆ® °ü·Ã 

// 2005, 1, 24, sobeit add start - ¾ÆÀÌÅÛ ¹Þ±â ÀÌº¥Æ® °ü·Ã
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
// 2005, 1, 24, sobeit add end - ¾ÆÀÌÅÛ ¹Þ±â ÀÌº¥Æ® °ü·Ã
#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 Ôö¼Ó°üÖÐ°ü
	// 2005, 2, 25, sobeit add start - ¼­ºê ÀÎº¥Åä¸®¸¦ ´Ý´Â´Ù.
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

		MItem* pMouseItem = UI_GetMouseItem();	// ÇöÀç mouseÀÇ item


		//-----------------------------------------------------------------
		// °ËÁõ¹ÞÀ»°Ô ¾ø´Â °æ¿ì
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
			if (gC_vs_ui.ReplaceSubInventoryItem(pMouseItem,		// Ãß°¡ÇÒ item
												left, right,	// Ãß°¡ÇÒ À§Ä¡ 
												pOldItem))		// ¿ø·¡ÀÖ´ø item
			{
				if (pOldItem != NULL) // replace µÇ¾ú´Â°¡?
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
		// °ËÁõ ¹Þ¾Æ¾ßÇÒ ´Ù¸¥ ¾ÆÀÌÅÛÀÌ ÀÖ´Â °æ¿ì
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
		// Inventory¿¡ ÀÖ´ø itemÀ» mouse¿¡ ºÙ¿´´Ù(-_-;)°í
		// server·Î packetÀ» º¸³½´Ù.
		//---------------------------------------------------
		CGAddInventoryToMouse _CGAddInventoryToMouse;
		_CGAddInventoryToMouse.setObjectID( pItem->GetID() );
		_CGAddInventoryToMouse.setX( pItem->GetGridX() );
		_CGAddInventoryToMouse.setY( pItem->GetGridY() );
		_CGAddInventoryToMouse.setInventoryItemObjectID(MultiPackItemID);
		
		g_pSocket->sendPacket( &_CGAddInventoryToMouse );				
	}

	// 2005, 2, 25, sobeit add end - ¼­ºê ÀÎº¥Åä¸®¸¦ ´Ý´Â´Ù.


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
			// ÀÚ±âÁ¾Á· ¾ÆÀÌÅÛ¸¸ ¾²Àð-¤µ-;
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
#endif