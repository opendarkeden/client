//-----------------------------------------------------------------------------
// CGameUpdate.cpp
//-----------------------------------------------------------------------------
// ½ÇÁ¦ °ÔÀÓÀ» ÁøÇàÇÏ´Â ºÎºÐ
//-----------------------------------------------------------------------------
#include "Client_PCH.h"

#pragma warning(disable:4786)


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <MMSystem.h>
#include <string>
#include "Client.h"
#include "GameObject.h"
#include "UserInformation.h"
#include "ServerInfo.h"
#include "PacketDef.h"
#include "VS_UI.h"
#include "VS_UI_Mouse_pointer.h"
#include "UIDialog.h"
#include "DebugInfo.h"
#include "CGameUpdate.h"
#include "MMoneyManager.h"
#include "MGameStringTable.h"
#include "MObjectSelector.h"
#include "ClientFunction.h"
#include "MMusic.h"
#include "MZoneSoundManager.h"
#include "TempInformation.h"
#include "MFakeCreature.h"
#include "MParty.h"
#include "Profiler.h"
#include "MTestDef.h"
#include "MEventManager.h"
#include "UIFunction.h"
#include "cmp3.h"
#include "COGGSTREAM.H"
#include "SystemAvailabilities.h"
#include "MWarManager.H"
//#include "EXECryptor.h"
//#include "ThemidaSDK.h"
#include "packet\Cpackets\CGVerifyTime.h"

#ifdef OUTPUT_DEBUG
	#include "packet\Gpackets\GCSkillFailed2.h"
	#include "packet\Gpackets\GCSkillToObjectOK5.h"
	#include "packet\Gpackets\GCSkillToSelfOK2.h"
	#include "packet\Gpackets\GCSkillToTileOK2.h"
	#include "packet\Cpackets\CGTypeStringList.h"
#endif
	
//add by viva
#include "packet\Gpackets\GCFriendChatting.h"

#ifdef OUTPUT_DEBUG
	bool g_bSlideScreenShot = false;
	bool g_bSaveSlideScreenShot = false;
	bool g_bSlideRectSelect = false;
	RECT g_rectScreenShot = { 0, 0, 0, 0 };

	int gtx_op = 4;
	int gtx_src = 2;
	int gtx_dest = 2;
#endif

// Global
CGameUpdate*		g_pCGameUpdate = NULL;

bool g_bPreviousMove = false;
	
extern DWORD g_double_click_time;

COGGSTREAM*					g_pOGG = NULL;
CDirectSoundBuffer*			g_pSoundBufferForOGG = NULL;
FILE	*					g_oggfile = NULL;

extern int g_SoundPerSecond;

extern int	g_previousSoundID;
extern bool g_bMouseInPortal;
		
extern POINT g_MouseSector;	
extern bool	g_bTestMode;

extern bool g_bZoneSafe;

// ÇÒÇÒ...
extern void	SetWatchMode( bool );
extern bool g_bWatchMode;

extern void	CheckTime();
extern int g_MyCheckTime=0;
extern int g_CheckErrorTime=0;
extern BOOL g_MyFull;
extern RECT g_GameRect;
//add by sonic 2006.7.30
//¼ì²âÏµÍ³µÇÂ½µÄÓÃ»§
#include <tlhelp32.h>
#pragma comment(lib, "th32")
extern int GetCurrentUserNumber();
extern int g_CheckTimeNum=0;
//end
//extern bool CheckInvalidProcess();
extern void	KeepConnection();
extern BOOL	InitInfomation();
extern void	ExecuteLogout();

extern char	g_CWD[_MAX_PATH];

#define	MOUSE_POINTER_SKIP_LEFT		-16
#define	MOUSE_POINTER_SKIP_UP		-16

//#define __FIX_GUNFRAME__

#ifdef __FIX_GUNFRAME__
	extern int g_tempX;
	extern int g_tempY;
#endif

// update loop¿¡ ´ëÇÑ debug message¸¦ Ãâ·ÂÇÒ±î ¸»±î?
#if defined(OUTPUT_DEBUG) //&& defined(_DEBUG)
	#ifdef __METROTECH_TEST__
//		#define	OUTPUT_DEBUG_UPDATE_LOOP	
//		#define OUTPUT_DEBUG_PROCESS_INPUT

		extern int  g_iAutoHealPotion;
		extern bool g_bAutoManaPotion;
		extern int  g_iAutoReload;
		extern int	g_iSpeed;
		extern bool g_bLight;
	#endif
#endif

#ifdef OUTPUT_DEBUG
	extern int g_HISTORY_LINE;
#endif

bool	g_bFrameChanged	 = false;
bool	g_bTestMusic = false;

#ifdef OUTPUT_DEBUG
std::string g_musicfilename[8]=
{
	"data\\music\\blood.mp3",
	"data\\music\\chaos.mp3",
	"data\\music\\dominator of darkness.mp3",
	"data\\music\\oblivion.mp3",
	"data\\music\\rest.mp3",
	"data\\music\\ruin.mp3",
	"data\\music\\silence of battlefield.mp3",
	"data\\music\\underworld.mp3"
};

int			g_CurrentMusicNum = 0;
#endif

extern MCreature* AddClientCreature();

void	PacketTest();
bool IsExistCorpseFromPlayer(MCreature* OriginCreature, int creature_type);
bool			HasEffectStatusSummonSylph( MCreature* pCreature );

extern void	SetFlagTo( bool bTae );

//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
void
CGameUpdate::Init()
{
	g_bPreviousMove = false;

	// mouse event Ã³¸®
	g_pDXInput->SetMouseEventReceiver( DXMouseEvent );
	
	// keyboard event Ã³¸®
	g_pDXInput->SetKeyboardEventReceiver( DXKeyboardEvent );
}

//-----------------------------------------------------------------------------
// DXMouseEvent
//-----------------------------------------------------------------------------
void		
CGameUpdate::DXMouseEvent(CDirectInput::E_MOUSE_EVENT event, int x, int y, int z)
{
	POINT point;

	if(g_pEventManager->GetEventByFlag(EVENTFLAG_DENY_INPUT_MOUSE))
	{
//		g_pEventManager->RemoveAllEvent();
		return;
	}

	static DWORD	last_click_time;
	static int		double_click_x, double_click_y;
	
	switch (event)
	{
			case CDirectInput::LEFTDOWN :
//				if ((DWORD)abs(GetTickCount() - last_click_time) <= g_double_click_time)
//				{
//					if (g_x>= double_click_x-1 && g_x <= double_click_x+1 &&
//						g_y>= double_click_y-1 && g_y <= double_click_y+1)
//					{
//						#ifdef OUTPUT_DEBUG_UPDATE_LOOP
//						DEBUG_ADD("MLD");
//						#endif
//						
//						gC_vs_ui.MouseControl(M_LB_DOUBLECLICK, g_x, g_y);
//						
//						g_pDXInput->m_lb_down = false;
//						g_pDXInput->m_lb_up = false;
//						
//						#ifdef OUTPUT_DEBUG_UPDATE_LOOP
//						DEBUG_ADD("MLD2");
//						#endif
//						
//						last_click_time = 0;
//						return;
//					}
//				}				
//				
				//gC_vs_ui.MouseControl(M_LEFTBUTTON_DOWN, g_x, g_y);
				last_click_time = GetTickCount();
				double_click_x = g_x;
				double_click_y = g_y;
				
				#ifdef OUTPUT_DEBUG			
				if (g_pDXInput->KeyDown(DIK_LSHIFT) && g_bSlideScreenShot)
				{
					g_rectScreenShot.left = g_x;
					g_rectScreenShot.right = g_x;
					g_rectScreenShot.top = g_y;
					g_rectScreenShot.bottom = g_y;
				}

				//---------------------------------------------------------
				// Minimap¿¡ Å¬¸¯ÇÏ¸é ±×ÂÊÀ¸·Î ÀÌµ¿ÇÑ´Ù.
				//---------------------------------------------------------
				/* // 2001.7.14 ÁÖ¼®Ã³¸®
				if (g_pDXInput->KeyDown(DIK_RCONTROL) && g_pUserOption->DrawMinimap)
				{
					int x = g_x - (800 - 256) * g_pZone->GetWidth() / 256;
					int y = g_y * g_pZone->GetWidth() / 128;

					if (x>=0 && x<g_pZone->GetWidth()
						&& y>=0 && y<g_pZone->GetWidth())
					{
						char str[80];

						int zoneID = (g_bZonePlayerInLarge? g_nZoneLarge : g_nZoneSmall);

						sprintf(str, "*warp %d %d %d", zoneID, x, y);

						#ifdef CONNECT_SERVER
							CGSay _CGSay;
							_CGSay.setMessage( str );	//pWansungString );
							g_pSocket->sendPacket( &_CGSay );

							#if defined(OUTPUT_DEBUG) && defined(__DEBUG_OUTPUT__)
										DEBUG_ADD_FORMAT("[Send] %s", _CGSay.toString().c_str() );
							#endif	
						#endif
					}
				}

				//---------------------------------------------------------
				// ChattingÃ¢¿¡ extra input
				//---------------------------------------------------------
				gC_vs_ui.ChatMouseControlExtra( M_LEFTBUTTON_DOWN, g_x, g_y );
				*/
				#endif
			break;

			case CDirectInput::RIGHTUP :
				// Áö·Ú ¸¸µå´Â °Å ÁßÁö
				gC_vs_ui.EndInstallMineProgress();
				//gC_vs_ui.EndCreateMineProgress();
				//gC_vs_ui.EndCreateBombProgress();
			break;

			//case CDirectInput::RIGHTDOWN :
				//---------------------------------------------------------
				// ChattingÃ¢¿¡ extra input
				//---------------------------------------------------------
				// 2001.7.14 ÁÖ¼®Ã³¸®
				//gC_vs_ui.ChatMouseControlExtra( M_RIGHTBUTTON_DOWN, g_x, g_y );
			//break;


		case CDirectInput::WHEELDOWN:
			gC_vs_ui.MouseControl(M_WHEEL_DOWN, g_x, g_y);
			///gC_vs_ui.ChatMouseControlExtra( M_WHEEL_DOWN, g_x, g_y );
			break;

		case CDirectInput::WHEELUP:
			gC_vs_ui.MouseControl(M_WHEEL_UP, g_x, g_y);
//			gC_vs_ui.ChatMouseControlExtra( M_WHEEL_UP, g_x, g_y );
			break;
	}
}

//#include "PacketDef.h"
				
//-----------------------------------------------------------------------------
// DXKeyboardEvent
//-----------------------------------------------------------------------------
void	
CGameUpdate::DXKeyboardEvent(CDirectInput::E_KEYBOARD_EVENT event, DWORD key)
{
	if(g_pEventManager->GetEventByFlag(EVENTFLAG_DENY_INPUT_KEYBOARD))
	{
		return;
	}
	
	if (event==CDirectInput::KEYDOWN)
	{
		if(key == 0xcc) return;		// ÀÌ°Ç ¸ÓÁö-_-?;;
				
		gC_vs_ui.DIKeyboardControl(event, key);
	
		switch (key)
		{	
//			__BEGIN_HELP_EVENT
//				case DIK_LMENU :
//					// [µµ¿ò¸»] alt´©¸¦¶§
//				
//					ExecuteHelpEvent( HE_PRESSED_ALT );					
//				break;
//			__END_HELP_EVENT				

// ¸ÓÁö ÀÌ°Ç-_-?;;; È¤½Ã ? ¾Æ´Ñµ¥-_-;
//			case DIK_F10 :
//				gC_vs_ui.HotKey_F10();
//			break;

			case DIK_SPACE :
				if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL) )
				{
				//	gC_vs_ui.HotKey_SummonPet(); 
//					gC_vs_ui.HotKey_WindowToggle();
				}
			break;

			//------------------------------------
			// L-CONTROL + S
			//------------------------------------
			//case DIK_S :				
			//	if (g_pDXInput->KeyDown(DIK_LCONTROL))
			//	{	
					/*
					switch (rand()%3)
					{
						case 0 : (*g_pUIDialog).PopupMessageDlg(UIDialog::MESSAGE_CANNOT_BUY_NO_SPACE); break;
						case 1 : (*g_pUIDialog).PopupMessageDlg(UIDialog::MESSAGE_CANNOT_BUY_NO_MONEY); break;
						case 2 : (*g_pUIDialog).PopupMessageDlg(UIDialog::MESSAGE_CANNOT_BUY_NO_ITEM); break;
					}
					*/
					/*
					// ÀÓ½Ã...
					g_TempInformation.Value2 = SKILL_DOUBLE_IMPACT;

					if (g_TempInformation.Mode==TempInformation::MODE_SKILL_LEARN)
					{
						#ifdef CONNECT_SERVER
							CGLearnSkill _CGLearnSkill;

							// temp information¿¡ ÀúÀåµÈ Á¤º¸ ÀÐ±â
							_CGLearnSkill.setSkillDomainType( g_TempInformation.Value1 );
							_CGLearnSkill.setSkillType( g_TempInformation.Value2 );
							
							
							#if defined(OUTPUT_DEBUG) && defined(__DEBUG_OUTPUT__)
									DEBUG_ADD_FORMAT( "[ Send ] %s", _CGLearnSkill.toString().c_str() );
							#endif

							g_pSocket->sendPacket( &_CGLearnSkill );
						#endif
					}
					*/


					// test
					/*
					ExecuteActionInfoFromMainNode(
							MAGIC_LIGHT,
						
							g_pPlayer->GetX(), g_pPlayer->GetY(), 0,
							g_pPlayer->GetDirection(),
							
							g_pPlayer->GetID(),
							g_pPlayer->GetX(), g_pPlayer->GetY(), 0,
							
							64,
							
							NULL
					);
					*/

					//---------------------------------------------------
					//
					// PC Talk Box Test
					//
					//---------------------------------------------------
					//---------------------------------------------------
					// UI¿¡ ¾Ë¸²...
					//---------------------------------------------------
					//
					// [ TEST CODE ]
					//
					// UI¿¡¼­ ¼±ÅÃµÈ ´ë´ä
					/*
					g_PCTalkBox.SetAnswerID( rand()%g_PCTalkBox.size() );

					//---------------------------------------------------
					// server¿¡ ¾Ë¸²
					//---------------------------------------------------
					#ifdef CONNECT_SERVER
						CGNPCAskAnswer _CGNPCAskAnswer;
						_CGNPCAskAnswer.setObjectID( g_PCTalkBox.GetNPCID() );
						_CGNPCAskAnswer.setScriptID( g_PCTalkBox.GetScriptID() );
						_CGNPCAskAnswer.setAnswerID( g_PCTalkBox.GetAnswerID() );

						#if defined(OUTPUT_DEBUG) && defined(__DEBUG_OUTPUT__)
								DEBUG_ADD_FORMAT("[Send] %s", _CGNPCAskAnswer.toString().c_str());
						#endif

						g_pSocket->sendPacket( &_CGNPCAskAnswer );
					#endif
					*/
			//	}
			//break;

			//------------------------------------
			// FPS Toggle
			//------------------------------------
			case DIK_F : 
				if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
				{
					if(g_pEventManager->IsEvent(EVENTID_OUSTERS_FIN))
					{
						g_pEventManager->RemoveEvent(EVENTID_OUSTERS_FIN);
					}
//					else
//					{
//						MEvent event;
//						event.eventID = EVENTID_OUSTERS_FIN;
//						event.eventType = EVENTTYPE_ZONE;
//						event.eventFlag = EVENTFLAG_ONLY_EVENT_BACKGROUND | EVENTFLAG_NOT_DRAW_UI | EVENTFLAG_NOT_DRAW_CREATURE | EVENTFLAG_NOT_DRAW_INFORMATION | EVENTFLAG_NOT_DRAW_CREATURE_SHADOW | EVENTFLAG_NOT_DRAW_ITEM | EVENTFLAG_NOT_DRAW_MOUSE_POINTER | EVENTFLAG_NOT_DRAW_EFFECT | EVENTFLAG_DENY_INPUT | EVENTFLAG_NOT_FADE_SCREEN | EVENTFLAG_NOT_PLAY_SOUND;
//						event.parameter1 = 0;
//						event.parameter4 = EVENTBACKGROUNDID_CLOUD;
//						g_pEventManager->AddEvent(event);
//					}
					(*g_pUserOption).DrawFPS = !(*g_pUserOption).DrawFPS;
//							g_abHolyLandBonusSkills[4] = !g_abHolyLandBonusSkills[4];
//							g_pSkillAvailable->SetAvailableSkills();
				}

				#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
					else if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
					{
						if (g_pPlayer->HasEffectStatus( EFFECTSTATUS_FADE_OUT ))
						{
							g_pPlayer->RemoveEffectStatus( EFFECTSTATUS_FADE_OUT );
						}
						else
						{
							// 5ÃÊ µ¿¾È ÁÂ¿ì ÀÜ»ó
							g_pPlayer->AddEffectStatus( EFFECTSTATUS_FADE_OUT, 5*1000 );
						}
					}
				#endif
			break;				

			//------------------------------------
			// Minimap Toggle
			//------------------------------------
			/*
			case DIK_M : 
				if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
				{
					(*g_pUserOption).DrawMinimap = !(*g_pUserOption).DrawMinimap;
				}
			break;

			//------------------------------------
			// DrawZoneName Toggle
			//------------------------------------
			case DIK_Z :
				if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
				{
					(*g_pUserOption).DrawZoneName = !(*g_pUserOption).DrawZoneName;
				}
			break;

			//------------------------------------
			// DrawGameTime Toggle
			//------------------------------------
			case DIK_T :
				if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
				{
					(*g_pUserOption).DrawGameTime = !(*g_pUserOption).DrawGameTime;
				}
			break;
			*/

			//------------------------------------
			// Blending Shadow
			//------------------------------------
			/*
			case DIK_V : 
				if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
				{
					(*g_pUserOption).BlendingShadow = !(*g_pUserOption).BlendingShadow;
					if (g_pTopView!=NULL)
					{
						g_pTopView->ClearShadowManager();
					}
				}
			break;
			*/

			//------------------------------------
			// È¿°úÀ½ on/off
			//------------------------------------
			/*
			case DIK_E : 
				if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
				{
					//------------------------------------
					// ¿¬ÁÖÁßÀÌ¸é.. Áß´Ü..
					//------------------------------------
					if (g_pUserOption->PlaySound)
					{
						g_pUserOption->PlaySound = FALSE;

						StopSound( g_previousSoundID );
						g_previousSoundID = SOUNDID_NULL;						
					}
					else
					{
						g_pUserOption->PlaySound = TRUE;
					}
				}
				//---------------------------------------------------------
				// Watch Mode
				//---------------------------------------------------------
				else if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
				{
					SetWatchMode( !g_bWatchMode );
				}
			break;
			*/

			//------------------------------------
			// ¹è°æ À½¾Ç on/off
			//------------------------------------
			/*
			case DIK_B : 
				#ifdef OUTPUT_DEBUG
					//------------------------------------
					// ¹ÚÁã º¯½Å test
					//------------------------------------
					if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
					{
						MActionResult* pResult = new MActionResult;

						if (g_pPlayer->GetCreatureType()!=185)
						{									
							pResult->Add( new MActionResultNodeChangeCreatureType( g_pPlayer->GetID(), 185 ) );
							//g_pPlayer->SetCreatureType( 185 );
							//g_pPlayer->SetFlyingCreature();
						}
						else
						{
							pResult->Add( new MActionResultNodeChangeCreatureType( g_pPlayer->GetID(), CREATURETYPE_VAMPIRE_MALE ) );
							//g_pPlayer->SetCreatureType( CREATURETYPE_VAMPIRE_MALE );
							//g_pPlayer->SetGroundCreature();
						}

						//--------------------------------------------------
						// ¹ÚÁã º¯½Å test
						//--------------------------------------------------								
						ExecuteActionInfoFromMainNode(
							RESULT_MAGIC_TRANSFORM_TO_BAT,										// »ç¿ë ±â¼ú ¹øÈ£
						
							g_pPlayer->GetX(), g_pPlayer->GetY(), 0,
							g_pPlayer->GetDirection(),														// »ç¿ë ¹æÇâ
							
							OBJECTID_NULL,												// ¸ñÇ¥¿¡ ´ëÇÑ Á¤º¸
							g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 
							
							0,													// ±â¼úÀÇ (³²Àº) Áö¼Ó ½Ã°£		
							
							pResult, //NULL,
							
							false);			// ±â¼ú Ã·ºÎÅÍ ½ÃÀÛÇÑ´Ù.

						g_pPlayer->SetDelay( 1000 );
					}
					
				#endif
				if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
				{
					//if (g_DXMusic.IsPlay())
					//{
					//	g_DXMusic.Pause();
					//}
					//else
					//{						
					//	g_DXMusic.Resume();
					//}
					
					//------------------------------------
					// ¿¬ÁÖÁßÀÌ¸é.. Áß´Ü..
					//------------------------------------
					if (g_pUserOption->PlayWaveMusic)
					{
						if (g_pUserOption->PlayMusic)
						{
							g_pUserOption->PlayMusic = FALSE;

							if (g_pDXSoundStream->IsPlay())
							{
								g_pDXSoundStream->Stop();
							}
						}
						//------------------------------------
						// ¾Æ´Ï¸é.. ´Ù½Ã play
						//------------------------------------
						else 
						{
							g_pUserOption->PlayMusic = TRUE;
										
							PlayMusicCurrentZone();
						}
					}
					else
					{
						if (g_pUserOption->PlayMusic)
						{
							g_pUserOption->PlayMusic = FALSE;

							if (g_Music.IsPlay())
							{
								g_Music.Pause();
							}
						}
						//------------------------------------
						// ¾Æ´Ï¸é.. ´Ù½Ã play
						//------------------------------------
						else 
						{
							g_pUserOption->PlayMusic = TRUE;

							if (g_Music.IsPlay())
							{
								g_Music.Resume();
							}
							else
							{													
								PlayMusicCurrentZone();
							}
						}
					}
				}
			break;
			*/

			//------------------------------------
			// DrawInterface Toggle
			//------------------------------------
			/*
			case DIK_H :
				if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
				{
					//(*g_pUserOption).DrawInterface = !(*g_pUserOption).DrawInterface;

					// µµ¿ò¸» ¶ç¿î´Ù.
					//(*g_pUIDialog).PopupHelpDlg();
				}
			break;
			*/
			
		
			#ifdef OUTPUT_DEBUG

		
				case DIK_SCROLL :
					if (g_bSlideScreenShot)
					{
						g_bSaveSlideScreenShot = !g_bSaveSlideScreenShot;
						g_bSlideRectSelect = false;
					}
				break;

				case DIK_S :
					if (g_pDXInput->KeyDown(DIK_LMENU))
					{
						g_bSlideScreenShot = !g_bSlideScreenShot;
						g_bSlideRectSelect = false;
					}
				break;

				case DIK_P :
					if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU) )
					{
						bool en = true;
						if(g_abHolyLandBonusSkills[0] == true)
							en = false;
						else
							en = true;

						for(int i = 0; i < HOLYLAND_BONUS_MAX; i++)
						{
							g_abHolyLandBonusSkills[i] = en;
						}

						g_pSkillAvailable->SetAvailableSkills();

//						if (g_pProfiler!=NULL)
//						{
//							g_pProfiler->WriteToFile("profiler.txt", true);
//						}
					}
				break;

				#if defined(_DEBUG) || defined(OUTPUT_DEBUG)
					case DIK_F1 :
					{
						// info ´Ù½Ã ÀÐ±â 
						if (g_pDXInput->KeyDown(DIK_LMENU) 
							|| g_pDXInput->KeyDown(DIK_RMENU))
						{
							InitInfomation();
							InitSound();
						}					
					}
					break;

					//------------------------------------------------------------
					//
					// ±â¼ú »ç¿ë - packet Å×½ºÆ®¸¦ À§ÇØ¼­
					//
					//------------------------------------------------------------
					case DIK_L : 
					{
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
						{
							MZone::CREATURE_MAP::const_iterator iCreature = g_pZone->GetCreatureBegin();					

							for (int i=0; i<g_pZone->GetCreatureNumber(); i++)
							{
								MCreature* pCreature = (*iCreature).second;

								if (pCreature!=g_pPlayer)
								{
									// ±â¼ú »ç¿ë
									// ±â¼ú ½ÇÆÐ
									//int targetID;						
										
									//int nth;
									
									MZone::CREATURE_MAP::const_iterator iCreature2;

									// ±â¼ú ¼º°ø
									if (rand()%2==0)
									{
										GCSkillToTileOK2 gcSkillToTileOK2;

										gcSkillToTileOK2.setObjectID( pCreature->GetID() );
										gcSkillToTileOK2.setSkillType( ((rand()%2)? 34 : 37) );
										gcSkillToTileOK2.setX( pCreature->GetX() );
										gcSkillToTileOK2.setY( pCreature->GetY() );
										gcSkillToTileOK2.setRange( rand()%8 );
										gcSkillToTileOK2.setDuration( 0 );
										gcSkillToTileOK2.addCListElement( g_pPlayer->GetID() );
										gcSkillToTileOK2.addShortData( (ModifyType)12, 97 );
										gcSkillToTileOK2.addLongData( (ModifyType)48, 68002 );										

//										DEBUG_ADD_FORMAT("[Execute] %s", gcSkillToTileOK2.toString().c_str());

										gcSkillToTileOK2.execute( g_pSocket );
										
										DEBUG_ADD("TileOK2OKOK");
									}
									/*
									else
									if (rand()%2==0)
									{
										nth = rand()%g_pZone->GetCreatureNumber();

										iCreature2 = g_pZone->GetCreatureBegin();

										for (int i=0; i<nth; i++)
										{
											targetID = iCreature->second->GetID();
											iCreature2 ++;
										}

										GCSkillToObjectOK5 gcSkillToObjectOK5;

										gcSkillToObjectOK5.setObjectID( pCreature->GetID() );
										gcSkillToObjectOK5.setTargetObjectID( targetID );
										gcSkillToObjectOK5.setSkillType( rand()%60+10 );
										gcSkillToObjectOK5.setDuration( 50 );

										DEBUG_ADD_FORMAT("[Execute] %s", gcSkillToObjectOK5.toString().c_str());

										#ifdef CONNECT_SERVER
											gcSkillToObjectOK5.execute( g_pSocket );
										#else
											gcSkillToObjectOK5.execute( NULL );
										#endif	
									}
									else if (rand()%2==0)
									{										
										int skillType = rand()%60+10;

										if (skillType != MAGIC_INVISIBILITY
											&& skillType != SKILL_SNIPPING)
										{
											GCSkillToSelfOK2 gcSkillToSelfOK2;

											gcSkillToSelfOK2.setObjectID( pCreature->GetID() );
											gcSkillToSelfOK2.setSkillType( skillType );
											gcSkillToSelfOK2.setDuration( 50 );

											DEBUG_ADD_FORMAT("[Execute] %s", gcSkillToSelfOK2.toString().c_str());

											#ifdef CONNECT_SERVER
												gcSkillToSelfOK2.execute( g_pSocket );
											#else
												gcSkillToSelfOK2.execute( NULL );
											#endif	
										}
									}
									else
									{
										// ±â¼ú ½ÇÆÐ
										nth = rand()%g_pZone->GetCreatureNumber();

										iCreature2 = g_pZone->GetCreatureBegin();

										for (int i=0; i<nth; i++)
										{
											targetID = iCreature->second->GetID();
											iCreature2 ++;
										}						

										GCSkillFailed2 gcSkillFailed2;
										gcSkillFailed2.setObjectID( pCreature->GetID() );
										gcSkillFailed2.setTargetObjectID( targetID );
										gcSkillFailed2.setSkillType( (rand()%2? 5:65) );

										DEBUG_ADD_FORMAT("[Execute] %s", gcSkillFailed2.toString().c_str());
										
										#ifdef CONNECT_SERVER
											gcSkillFailed2.execute( g_pSocket );
										#else
											gcSkillFailed2.execute( NULL );
										#endif
									}
									*/
								}

								iCreature++;
							}
						}
					}
					break;

					case DIK_1 :
//						if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
//						{
//							if( g_pSoundBufferForOGG == NULL )
//								g_pSoundBufferForOGG = new CDirectSoundBuffer(g_hWnd, SOUND_STEREO, SOUND_44K, SOUND_16BIT);
//
//							if( g_pOGG == NULL )
//								g_pOGG = new COGGSTREAM(g_hWnd, g_pSoundBufferForOGG, 44100, 11025, 8800,1);
//							
//							if( g_oggfile != NULL)
//								fclose(g_oggfile );
//							g_oggfile = fopen("C:\\ogggggg\\3\\blood.ogg","rb");
//							g_pOGG->streamLoad(g_oggfile, NULL);
//							g_pOGG->streamPlay(SOUND_PLAY_REPEAT);
//							
////							gC_vs_ui.SetOustersSkillDown();
////							g_pMP3->Stop();
////							g_pMP3->Open("data\\music\\blood.mp3");
////							g_pMP3->Play( true );
////							g_bTestMusic = false;
////							g_pSystemMessage->Add("Blood.mp3 ¸¦ ¿¬ÁÖÇÕ´Ï´Ù");							
//						}
//						// ALT + 1  --> °ø°Ý ¼Óµµ Slow
						if(g_pDXInput->KeyDown(DIK_LCONTROL))
						{ 
							
							//g_pPlayer->SetSpecialActionInfo(TEMP_SKILL_INFINITY_THUNDERBLOT);
//							if( g_pEventManager->GetEvent( EVENTID_WAR_EFFECT ) == NULL )
//							{
//								MEvent event;
//								event.eventID = EVENTID_WAR_EFFECT;
//								event.eventType = EVENTTYPE_NULL;
//								event.eventFlag = EVENTFLAG_SHAKE_SCREEN | EVENTFLAG_FADE_SCREEN;
//								event.parameter2 = RGB( -30, -30, 0 );
//								event.parameter3 = 1;
//								event.eventDelay = -1;
//								event.showTime = 2000;
//								event.totalTime = 30000;
//								g_pEventManager->AddEvent( event );
//							}
//							else
//							{
//								g_pEventManager->RemoveEvent( EVENTID_WAR_EFFECT );
//							}
//							ExecuteActionInfoFromMainNode(SKILL_DESTRUCTION_SPEAR_MASTERY,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX()+5, g_pPlayer->GetY(), 0, 100, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_SUMMON_GDR,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 5000, NULL, false);			
//							MEvent event;
//
//							event.eventID = EVENTID_GDR_PRESENT;
//							event.eventType = EVENTTYPE_ZONE;
//							event.eventFlag = EVENTFLAG_SHAKE_SCREEN;
//							event.parameter3 = 2;
//							event.eventDelay =5000;
//							g_pEventManager->AddEvent(event);
//							//SetFadeStart(31, 0, 1, 0,0,0, 6);
							//gC_vs_ui.RunNameing();
							//g_pPlayer->SetAction(ACTION_DRAINED);
//							g_pPlayer->m_ActionCount = 0;
							//g_pPlayer->SetCauseCriticalWounds(100);
							//g_pPlayer->AddEffectStatus(EFFECTSTATUS_GLACIER, 100);
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_BLINDNESS,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 30, NULL, false);
						//	g_pPlayer->AddEffectStatus(EFFECTSTATUS_GRAND_MASTER_SLAYER_130, 0xffff);
							//g_pPlayer->AddEffectStatus(EFFECTSTATUS_BLINDNESS, 10);
//							g_pPlayer->AddEffectStatus(EFFECTSTATUS_CAN_ENTER_GDR_LAIR, 10);
//							g_pPlayer->AddEffectStatus(EFFECTSTATUS_ICE_STICK_SE, 10);
//							g_pPlayer->AddEffectStatus(EFFECTSTATUS_GDR_GLACIER_SLAYER, 100);
//							g_pPlayer->AddEffectStatus(EFFECTSTATUS_GRAND_MASTER_OUSTERS_130, 0xffff);
//							g_pPlayer->AddEffectStatus(EFFECTSTATUS_GRAND_MASTER_OUSTERS_150, 0xffff);
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_SLAYER_MASTER_130,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 5000, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_SLAYER_MASTER_150,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 5000, NULL, false);			
////
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_VAMPIRE_MASTER_130,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 5000, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_VAMPIRE_MASTER_150,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 5000, NULL, false);			
//
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_OUSTERS_MASTER_130,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 5000, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TEST_VIOLENT_PHANTOM,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 20, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TEST_SUMMON_GORE_GRAND_GROUND,g_pPlayer->GetX()+1, g_pPlayer->GetY()-3, 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX()+1, g_pPlayer->GetY()-3, 0, 50, NULL, false);
//							ExecuteActionInfoFromMainNode(RESULT_CLIENT_TEST_SWORD_OF_THOR,g_pPlayer->GetX(), g_pPlayer->GetY()+3, 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX(), g_pPlayer->GetY()+3, 0, 100, NULL, false);
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TEST_BURNING_SOL_1,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX()-5, g_pPlayer->GetY(), 0, 100, NULL, false);
//							g_pPlayer->SetAction(ACTION_SLAYER_BLADE);
//							ExecuteActionInfoFromMainNode(SKILL_PLEASURE_EXPLOSION,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 0, NULL, false);			
							
//							if(g_pEventManager->IsEvent(EVENTID_BG_CLOUD))
//							{
//								g_pEventManager->RemoveEvent(EVENTID_BG_CLOUD);
//							}
//							else
//							{
//								MEvent event;
//								event.eventID = EVENTID_BG_CLOUD;
//								event.eventType = EVENTTYPE_ZONE;
//								event.eventFlag = EVENTFLAG_CLOUD_BACKGROUND;
//								event.parameter3 = 2;
//								event.eventDelay =60000; // 5 ÃÊ
//								g_pEventManager->AddEvent(event);
//							}
							
//							ExecuteActionInfoFromMainNode(SKILL_PLEASURE_EXPLOSION,g_pPlayer->GetX(), g_pPlayer->GetY()+3, 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX(), g_pPlayer->GetY()+3, 0, 100, NULL, false);
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_LAND_MINE_LIGHT,g_pPlayer->GetX()+5, g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX()+5, g_pPlayer->GetY(), 0, 100, NULL, false);
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_PLEASUER_STATION_LIGHT,g_pPlayer->GetX()+3, g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX()+3, g_pPlayer->GetY(), 0, 100, NULL, false);

							//g_pPlayer->AddEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_1, 1000);
//
//							MActionResult* pResult = new MActionResult;
////
//							if (g_pPlayer->GetCreatureType()!=742)
//							{									
//								g_pPlayer->SetCreatureType( 742 );
//
//								g_pPlayer->SetGroundCreature();
//					
//								g_pPlayer->SetDirection( 2 );
//								g_pPlayer->SetCurrentDirection( 2 );
//								g_pPlayer->SetAction( ACTION_STAND );			
//
//								//--------------------------------------------------
//								// ´Á´ë·Î º¯½ÅÇÏ´Â °á°ú
//								//--------------------------------------------------
//								MActionResult* pResult = new MActionResult;
//
//								pResult->Add( new MActionResultNodeChangeCreatureType( g_pPlayer->GetID(), 742 ) );
//
//								//--------------------------------------------------
//								// ´Á´ë º¯½Å 
//								//--------------------------------------------------								
//								ExecuteActionInfoFromMainNode(
//									RESULT_SKILL_INSTALL_TURRET,										// »ç¿ë ±â¼ú ¹øÈ£
//								
//									g_pPlayer->GetX(), g_pPlayer->GetY(), 0,
//									g_pPlayer->GetDirection(),														// »ç¿ë ¹æÇâ
//									
//									OBJECTID_NULL,												// ¸ñÇ¥¿¡ ´ëÇÑ Á¤º¸
//									g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 
//									
//									0,													// ±â¼úÀÇ (³²Àº) Áö¼Ó ½Ã°£		
//									
//									NULL, //NULL,
//									
//									false);			// ±â¼ú Ã·ºÎÅÍ ½ÃÀÛÇÑ´Ù.
//
//							}
//							else
//							{
//								pResult->Add( new MActionResultNodeChangeCreatureType( g_pPlayer->GetID(), CREATURETYPE_SLAYER_FEMALE ) );
//								//g_pPlayer->SetCreatureType( CREATURETYPE_VAMPIRE_MALE );
//								//g_pPlayer->SetGroundCreature();
//							}
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TEST_BURNING_SOL_4,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 20, pResult, false);	
//
//							g_pPlayer->SetDelay( 1000 );

							
//							UI_RunPetStorage();SKILL_CLIENT_TEST_SUMMON_GORE_GRAND_GROUND
//							UI_SetPetStorage(g_pStorage);
							// start Æê ¼ÒÈ¯ ´ÜÃàÅ° °ü·Ã
//							for(int i = 0; i< 10; i++)
//								for(int j = 0; j < 6; j++)
//								{
//									const MItem * p_item = g_pInventory->GetItem(i, j);
//				
//									if (p_item) // ItemÀÌ ÀÖ´Ù.
//									{
//										if(p_item->GetItemClass() == ITEM_CLASS_PET_ITEM)
//										{
//											gpC_base->SendMessage(UI_ITEM_USE, p_item->GetID(), 0, (MItem *)p_item);
//											return;
//										}
//									}
//								}
							// end Æê ¼ÒÈ¯ ´ÜÃàÅ° °ü·Ã

							// start npc ´ëÈ­Ã¢ °ü·Ã
//							g_pPCTalkBox->Release();
//							//---------------------------------------------------
//							// normal
//							//---------------------------------------------------
//							g_pPCTalkBox->SetType( PCTalkBox::SELECT_QUEST );
//						
//							int scriptID = 101;
//							
//							//---------------------------------------------------
//							// PC Talk BoxÀÇ Á¤º¸ ¼³Á¤
//							//---------------------------------------------------
//							// SetContent¶ó°í ÀÌ¸§ÀÌ µÇ¾îÀÖÁö¸¸.. SubjectÀÌ´Ù. - -;
//							g_pPCTalkBox->SetContent( "ÇÏÇÏÇÏ" );
//							g_pPCTalkBox->SetNPCID( 297 );
//							g_pPCTalkBox->SetCreatureType( 297 );
//							g_pPCTalkBox->SetScriptID( scriptID );
//							
//							//---------------------------------------------------
//							// °¢ string Ãß°¡
//							//---------------------------------------------------
//							int contentSize = 2;//g_pNPCScriptTable->GetContentSize( scriptID );
//							
//							//for (int i=0; i<contentSize; i++)
//							{
//								// g_PCTalkBox¿¡ Ãß°¡
//								g_pPCTalkBox->AddString( "¿ìÇìÇì" );
//								g_pPCTalkBox->AddString( "Å©ÇÏÇÏ" );
//							}
//							
//							g_pUIDialog->PopupPCTalkDlg();
							// start npc ´ëÈ­Ã¢ °ü·Ã

//							gC_vs_ui.AddHelpMail(6); 
//							gC_vs_ui.AddHelpMail(0); 
//							gC_vs_ui.AddHelpMail(1); 
//							gC_vs_ui.AddHelpMail(2); 
//							gC_vs_ui.AddHelpMail(3); 
//							gC_vs_ui.AddHelpMail(4); 
//							gC_vs_ui.AddHelpMail(5); 
						
//							gC_vs_ui.RunSMSMessage();
//							extern void Add_GDR_Effect(int nEffect, bool bAppearBossMonster);
//							Add_GDR_Effect(1, false);
//							Add_GDR_Effect(2, false);
//
//							ExecuteActionInfoFromMainNode(TEMP_SKILL_SPIT_STREAM,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 0xffff, NULL, false);			
							//g_pPlayer->SetAction(ACTION_OUSTERS_CHAKRAM_SLOW);
	// create ghost
							{

//								extern void SetDragonTorando(int Type, DWORD ObjectID, int TileX, int TileY);
//								SetDragonTorando(EFFECTSTATUS_DRAGON_TORNADO, 11000, g_pPlayer->GetX()+4, g_pPlayer->GetY());

								UI_PopupMessage( UI_STRING_MESSAGE_CAMPAIGN_HELP_THANKS );
//
//								ExecuteActionInfoFromMainNode(SKILL_CLIENT_BIKE_CRASH, g_pPlayer->GetX(), g_pPlayer->GetY(), 0, g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//									g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 5000, NULL, false);	
//								MFakeCreature *pFakeCreature = NewFakeCreature(CREATURETYPE_ROCKET_LUNCHER, g_pPlayer->GetX()+5, g_pPlayer->GetY(), rand()%8);
//
//								if (!g_pZone->AddFakeCreature( pFakeCreature ))
//								{
//									delete pFakeCreature;
//								}
//								else
//								{
//									pFakeCreature->SetZone(g_pZone);
//									pFakeCreature->SetFakeCreatureType(MFakeCreature::FAKE_CREATURE_ROCKET);
//									pFakeCreature->SetMoveType(MCreature::CREATURE_FAKE_FLYING);
//									pFakeCreature->AddEffectStatus( (EFFECTSTATUS)(EFFECTSTATUS_ROCKET_LUNCHER), 0xFFFF );
//									pFakeCreature->SetTraceID(g_pPlayer->GetID());
//									pFakeCreature->SetTime(timeGetTime()+3000); 
//								//	pFakeCreature->SetAction( ACTION_MOVE );
//								}
							}
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICICLE_DROP_SIGN,g_pPlayer->GetX()+5, g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX()+5, g_pPlayer->GetY(), 0, 10, NULL, false);			
//							
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICICLE_LARGE_AUGER,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 5000, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICICLE_LARGE_AUGER_SIGN,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 10, NULL, false);			
//							
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICICLE_AUGER,g_pPlayer->GetX(), g_pPlayer->GetY()-5, 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX(), g_pPlayer->GetY()-5, 0, 5000, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICICLE_AUGER_SIGN,g_pPlayer->GetX(), g_pPlayer->GetY()-5, 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX(), g_pPlayer->GetY()-5, 0, 10, NULL, false);	
//							
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICE_STICK_SW,g_pPlayer->GetX(), g_pPlayer->GetY()+5, 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX(), g_pPlayer->GetY()+5, 0, 0, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICE_STICK_SW_SIGN,g_pPlayer->GetX(), g_pPlayer->GetY()+5, 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX(), g_pPlayer->GetY()+5, 0, 0, NULL, false);			
//
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICE_STICK_SE,g_pPlayer->GetX()-5, g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX()-5, g_pPlayer->GetY(), 0, 5000, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICE_STICK_SE_SIGN,g_pPlayer->GetX()-5, g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX()-5, g_pPlayer->GetY(), 0, 10, NULL, false);	
//
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICE_STICK_NE,g_pPlayer->GetX()-5, g_pPlayer->GetY()-5, 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX()-5, g_pPlayer->GetY()-5, 0, 5000, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICE_STICK_NE_SIGN,g_pPlayer->GetX()-5, g_pPlayer->GetY()-5, 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX()-5, g_pPlayer->GetY()-5, 0, 10, NULL, false);
//
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICE_STICK_NW,g_pPlayer->GetX()+5, g_pPlayer->GetY()+5, 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX()+5, g_pPlayer->GetY()+5, 0, 5000, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICE_STICK_NW_SIGN,g_pPlayer->GetX()+5, g_pPlayer->GetY()+5, 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX()+5, g_pPlayer->GetY()+5, 0, 10, NULL, false);
//							
//								for(int i = 0; i< 100 ; i++)
//								{
//									int MapX = rand()% g_pZone->GetWidth();
//									int MapY = rand()% g_pZone->GetHeight();
//									//MFakeCreature *pFakeCreature = NewFakeCreature(716, g_pPlayer->GetX(), g_pPlayer->GetY(), 2);
//									MFakeCreature *pFakeCreature = NewFakeCreature(CREATURETYPE_GHOST, MapX, MapY, rand()%8);
//
//									if (!g_pZone->AddFakeCreature( pFakeCreature ))
//									{
//										delete pFakeCreature;
//									}
//									pFakeCreature->SetZone(g_pZone);
//									pFakeCreature->SetFakeCreatureType(MFakeCreature::FAKE_CREATURE_GHOST);
//									pFakeCreature->SetMoveType(MCreature::CREATURE_FAKE_UNDERGROUND);
//									pFakeCreature->AddEffectStatus( (EFFECTSTATUS)(EFFECTSTATUS_GHOST_1+rand()%2), 0xFFFF );
//								}

								
//							if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_FIRE_ELEMENTAL))
//								g_pPlayer->RemoveEffectStatus( EFFECTSTATUS_FIRE_ELEMENTAL );
//							else
//								g_pPlayer->AddEffectStatus( EFFECTSTATUS_FIRE_ELEMENTAL, 0xFFFF );
//							g_pPlayer->SetDelay(2000);
//							TYPE_OBJECTID petID = g_pPlayer->GetPetID();
//							if(petID != OBJECTID_NULL)
//							{
//								MFakeCreature *pCreature = (MFakeCreature *)g_pZone->GetFakeCreature(petID);
//								pCreature->SetAction( ACTION_ATTACK );
//								pCreature->SyncTurretDirection();
//
//
//								ExecuteActionInfoFromMainNode(
//									SKILL_CLIENT_TANK_ATTACK_3,										// »ç¿ë ±â¼ú ¹øÈ£
//									
//									0, 0, 0,
//									pCreature->GetDirection(), // »ç¿ë ¹æÇâ
//									
//									petID,												// ¸ñÇ¥¿¡ ´ëÇÑ Á¤º¸
//									0, 0, 0, 
//									
//									0,													// ±â¼úÀÇ (³²Àº) Áö¼Ó ½Ã°£		
//									
//									NULL,
//									
//									false);	
//								pCreature->AddEffectStatus(EFFECTSTATUS_COMA, 0xFFFF);
//								pCreature->SetDead();
//							SetFlagTo( true );
//						}
//							g_pPlayer->AddEffectStatus(EFFECTSTATUS_HAS_PET, 0);
						}
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
						{
							//g_pPlayer->SetWeaponSpeed( MCreature::WEAPON_SPEED_SLOW );

							// 1 ~ 24
							/*
							if (++gtx_op > 24)
							{
								gtx_op = 1;
							}
							*/
							#ifdef __METROTECH_TEST__
								g_iAutoHealPotion = (g_iAutoHealPotion+1)%2;
							#endif
						}
					break;

					case DIK_2 :
						if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
						{  
//							extern void SetDragonTorando(int Type, DWORD ObjectID, int TileX, int TileY);
//							SetDragonTorando(EFFECTSTATUS_DRAGON_TORNADO, 11000, g_pPlayer->GetX()+3, g_pPlayer->GetY());
////
//							UI_RunModifyTax();  
//							if(g_pEventManager->IsEvent(EVENTID_ADVANCEMENT_QUEST_ENDING))
//							{
//								g_pEventManager->RemoveEvent(EVENTID_ADVANCEMENT_QUEST_ENDING);
//							}
//							else
//							{
//								MEvent event;
//								event.eventID = EVENTID_ADVANCEMENT_QUEST_ENDING;
//								event.eventType = EVENTTYPE_ZONE;
//								event.eventFlag = EVENTFLAG_ONLY_EVENT_BACKGROUND | EVENTFLAG_NOT_DRAW_UI | EVENTFLAG_NOT_DRAW_CREATURE | EVENTFLAG_NOT_DRAW_INFORMATION | EVENTFLAG_NOT_DRAW_CREATURE_SHADOW | EVENTFLAG_NOT_DRAW_ITEM | EVENTFLAG_NOT_DRAW_MOUSE_POINTER | EVENTFLAG_NOT_DRAW_EFFECT | EVENTFLAG_DENY_INPUT | EVENTFLAG_NOT_FADE_SCREEN | EVENTFLAG_NOT_PLAY_SOUND;
//								event.parameter1 = 0;
//								event.parameter4 = 3;
//								g_pEventManager->AddEvent(event);
//
////								MEvent event;
////								event.eventID = EVENTID_WAR_EFFECT;
////								event.eventType = EVENTTYPE_NULL;
////								event.eventFlag = EVENTFLAG_SHAKE_SCREEN | EVENTFLAG_FADE_SCREEN;
////								event.parameter2 = RGB( -30, -30, 0 );
////								event.parameter3 = 1;
////								event.eventDelay = -1;
////								event.showTime = 2000;
////								event.totalTime = 30000;
////								g_pEventManager->AddEvent( event );
//							}
//
							if(UI_IsRunning_WebBrowser())
								UI_Close_WebBrowser();
							else
							//add by zdj
					 			UI_Run_WebBrowser(/*"http://market.darkeden.com/"*/"");
//							if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_DIVINE_GUIDANCE))
//								g_pPlayer->RemoveEffectStatus(EFFECTSTATUS_DIVINE_GUIDANCE); 
//							else
//								g_pPlayer->AddEffectStatus(EFFECTSTATUS_DIVINE_GUIDANCE, 0xffff);
							//g_pPlayer->SetCreatureType(770); 
//							if(g_pPlayer->GetCreatureType() == CREATURETYPE_SLAYER_OPERATOR)
//								g_pPlayer->SetCreatureType(358);
//							else
//								g_pPlayer->SetCreatureType(CREATURETYPE_SLAYER_OPERATOR);
//							
						//	g_pPlayer->RemoveEffectStatus(EFFECTSTATUS_GLACIER);
						//	g_pPlayer->RemoveEffectStatus(EFFECTSTATUS_BLESS_FIRE);

//							ExecuteActionInfoFromMainNode(TEMP_SKILL_NOOSE_OF_WRAITH,g_SelectSector.x, g_SelectSector.y, 0,g_pPlayer->GetDirection(), g_pPlayer->GetID()	,	
//								g_SelectSector.x, g_SelectSector.y, 0, 0xffff, NULL, false);
//							g_pPlayer->SetAction(ACTION_DRAINED);
//							ExecuteActionInfoFromMainNode(RESULT_MAGIC_CAUSE_CRITICAL_WOUNDS,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//									g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 0xffff, NULL, false);

//							if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1))
//								g_pPlayer->AddEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2, 32);
//							else if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2))
//								g_pPlayer->AddEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3, 32);
//							else if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3))
//								g_pPlayer->AddEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_4, 32);
//							else if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_4))
//							{
//								g_pPlayer->RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_4);
//								g_pPlayer->StopBurningSol();
//								ExecuteActionInfoFromMainNode(SKILL_CLIENT_BURNING_SOL_4,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//									g_pPlayer->GetX()-4, g_pPlayer->GetY(), 0, 100, NULL, false);	
//								ExecuteActionInfoFromMainNode(RESULT_CLIENT_BURNING_SOL_4,g_pPlayer->GetX()-4, g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//									g_pPlayer->GetX()-4, g_pPlayer->GetY(), 0, 100, NULL, false);	
//							}
//							else 
//								g_pPlayer->AddEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1, 32);
//
//							ExecuteActionInfoFromMainNode(RESULT_SKILL_SWORD_OF_THOR,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	g_pPlayer->GetID(),	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 20, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_SWORD_OF_THOR,g_pPlayer->GetX()-4, g_pPlayer->GetY()-4, 0,g_pPlayer->GetDirection(),0	,	
//								g_pPlayer->GetX()-4, g_pPlayer->GetY()-4, 0, 50, NULL, false);
//
//							ExecuteActionInfoFromMainNode(RESULT_SKILL_WHITSUNTIDE,g_pPlayer->GetX()+4, g_pPlayer->GetY()+4, 0,g_pPlayer->GetDirection(),0	,	
//								g_pPlayer->GetX()+4, g_pPlayer->GetY()+4, 0, 50, NULL, false);
//
//							ExecuteActionInfoFromMainNode(SKILL_SWEEP_VICE_1,g_pPlayer->GetX(), g_pPlayer->GetY()+4, 0,g_pPlayer->GetDirection(),0	,	
//								g_pPlayer->GetX(), g_pPlayer->GetY()+4, 0, 1000, NULL, false);
//							ExecuteActionInfoFromMainNode(SKILL_SWEEP_VICE_5,g_pPlayer->GetX(), g_pPlayer->GetY()-4, 0,g_pPlayer->GetDirection(),0	,	
//								g_pPlayer->GetX(), g_pPlayer->GetY()-4, 0, 50, NULL, false);
//							g_pPlayer->SetAction(ACTION_MAGIC);

							
	//						EVENTFLAG_NOT_FADE_SCREEN
						//	g_pPlayer->AddEffectStatus(EFFECTSTATUS_ICICLE_LARGE_AUGER, 100);
						//	ExecuteActionInfoFromMainNode(SKILL_WIDE_ICE_FIELD,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,g_pPlayer->GetDirection(),	0,	
						//				g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 100, NULL, false);	
							
						//	ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICICLE_LARGE_AUGER,g_pPlayer->GetX()-5, g_pPlayer->GetY()+5, 0,g_pPlayer->GetDirection(),	0,	
						//		g_pPlayer->GetX()-5, g_pPlayer->GetY()+5, 0, 5, NULL, false);			
							//ExecuteActionInfoFromMainNode(SKILL_ICE_HAIL,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,0,	0,	
							//	g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 10000, NULL, false);			
						//	ExecuteActionInfoFromMainNode(SKILL_ICE_HAIL,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,0,	0,	
						//		g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 10000, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_ICE_WAVE,g_pPlayer->GetX()+5, g_pPlayer->GetY()+5, 0,g_pPlayer->GetDirection(),	0,	
//								g_pPlayer->GetX()+5, g_pPlayer->GetY()+5, 0, 100, NULL, false);			
							//ExecuteActionInfoFromMainNode(SKILL_CLIENT_TRAP_ICE_STICK_SW,g_pPlayer->GetX(), g_pPlayer->GetY()+5, 0,g_pPlayer->GetDirection(),	0,	
							//	g_pPlayer->GetX(), g_pPlayer->GetY()+5, 0, 0, NULL, false);			
						//	UI_RunNotice(7, 100000000);
						//	extern void Add_GDR_Effect(int nEffect, bool bAppearBossMonster);
//						//	Add_GDR_Effect(1,true);	
						//	Add_GDR_Effect(2,true);	
							
//							SetFlagTo( false );
//							TYPE_OBJECTID petID = g_pPlayer->GetPetID();
//							if(petID != OBJECTID_NULL)
//							{
//								MCreature *pCreature = g_pZone->GetFakeCreature(petID);
//								
//								g_pPlayer->SetAction( ACTION_DAMAGED );
//								ExecuteActionInfoFromMainNode(
//									SKILL_CLIENT_TANK_ATTACKED,										// »ç¿ë ±â¼ú ¹øÈ£
//									
//									0, 0, 0,
//									DIRECTION_DOWN, // »ç¿ë ¹æÇâ
//									
//									g_pPlayer->GetID(),												// ¸ñÇ¥¿¡ ´ëÇÑ Á¤º¸
//									0, 0, 0, 
//									
//									0,													// ±â¼úÀÇ (³²Àº) Áö¼Ó ½Ã°£		
//									
//									NULL,
//									
//									false);	
//							}
//							g_pPlayer->AddEffectStatus( EFFECTSTATUS_FIRE_ELEMENTAL_ATTACK, 21 );
//							MEvent event;
//							event.eventID = EVENTID_OUSTERS_FIN;
//							event.eventType = EVENTTYPE_ZONE;
//							event.eventFlag = EVENTFLAG_ONLY_EVENT_BACKGROUND | EVENTFLAG_NOT_DRAW_UI | EVENTFLAG_NOT_DRAW_CREATURE | 
//								EVENTFLAG_NOT_DRAW_INFORMATION | EVENTFLAG_NOT_DRAW_CREATURE_SHADOW | EVENTFLAG_NOT_DRAW_ITEM | 
//								EVENTFLAG_NOT_DRAW_MOUSE_POINTER | EVENTFLAG_NOT_DRAW_EFFECT | EVENTFLAG_DENY_INPUT | 
//								EVENTFLAG_NOT_FADE_SCREEN | EVENTFLAG_NOT_PLAY_SOUND;
//							event.parameter1 = 0;
//							event.parameter4 = EVENTBACKGROUNDID_QUEST_2;
//							g_pEventManager->AddEvent(event);		
						}
						// ALT + 2  --> °ø°Ý ¼Óµµ normal
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
						{
							//g_pPlayer->SetWeaponSpeed( MCreature::WEAPON_SPEED_NORMAL);

							// 1 ~ 13
							/*
							if (++gtx_src > 13)
							{
								gtx_src = 1;
							}
							*/
							#ifdef __METROTECH_TEST__
								g_bAutoManaPotion = !g_bAutoManaPotion;
							#endif
						}
					break;

					case DIK_3 :
						if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
						{
//							g_pZone->RemoveTileEffect( g_pPlayer->GetX()+3, g_pPlayer->GetY(), EFFECTSTATUS_DRAGON_TORNADO, 11000 );

	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 Ôö¼Ó°üÖÐ°ü
							MItem* pItem = MItem::NewItem( ITEM_CLASS_SUB_INVENTORY );
							pItem->SetID( rand()%10000 );
							pItem->SetItemType( 0 );
							gC_vs_ui.RunSubInventory( pItem );
	#endif


							//g_pPlayer->SetAction(ACTION_ADVANCEMENT_SLAYER_MAGIC_ATTACK);
							//gC_vs_ui.RunPopupMessage("ComeBackEventNotice.txt", C_VS_UI_POPUP_MESSAGE::POPUP_WELCOME );
//							UI_PopupMessage(UI_STRING_MESSAGE_GET_EVENT_ITEM_NOTICE);
							//g_pUIDialog->PopupFreeMessageDlg( (*g_pGameStringTable)[UI_STRING_MESSAGE_GET_EVENT_ITEM_NOTICE].GetString() );
							//g_pPlayer->SetInvisibleSoon();
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_QUEST_MAGIC_ZONE,16, 16, 0,0,	0,	
//								16, 16, 0, 0xffff, NULL, false);			
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_QUEST_MAGIC_ZONE_POTAL,16, 16, 0,0,	0,	
//								16, 16, 0, 0xffff, NULL, false);			
//							if(gC_vs_ui.IsRunningPowerjjang())
//							{
//								gC_vs_ui.PowerjjangGambleResult(rand()%7);
//							}
//							else
//							gC_vs_ui.RunPowerjjang();
//							if(!g_pPlayer->HasEffectStatus(EFFECTSTATUS_VIEW_HP))
//								g_pPlayer->AddEffectStatus(EFFECTSTATUS_VIEW_HP,0xffffffff);
//							else
//							{
////								ExecuteActionInfoFromMainNode(SKILL_TURRET_FIRE,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,2,	g_pPlayer->GetID(),	
////									g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 200, NULL, true);	
//
//								g_pPlayer->RemoveEffectStatus(EFFECTSTATUS_VIEW_HP);
//							}
//							ExecuteActionInfoFromMainNode(SKILL_VIOLENT_PHANTOM,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,DIRECTION_DOWN,	g_pPlayer->GetID(),	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 200, NULL, true);	
//							g_pPlayer->SetAction(ACTION_DAMAGED);
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TURRET_LASER_ATTACK,g_pPlayer->GetX(), g_pPlayer->GetY()-4, 0,g_pPlayer->GetDirection(),0	,	
//								g_pPlayer->GetX(), g_pPlayer->GetY()-4, 0, 1000, NULL, false);
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TURRET_LASER_ATTACK,g_pPlayer->GetX()+1, g_pPlayer->GetY()-3, 0,g_pPlayer->GetDirection(),0	,	
//								g_pPlayer->GetX()+1, g_pPlayer->GetY()-3, 0, 1000, NULL, false);
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TURRET_LASER_ATTACK,g_pPlayer->GetX()+2, g_pPlayer->GetY()-2, 0,g_pPlayer->GetDirection(),0	,	
//								g_pPlayer->GetX()+2, g_pPlayer->GetY()-2, 0, 1000, NULL, false);
//							ExecuteActionInfoFromMainNode(SKILL_CLIENT_TURRET_LASER_ATTACK,g_pPlayer->GetX()+3, g_pPlayer->GetY()-1, 0,g_pPlayer->GetDirection(),0	,	
//								g_pPlayer->GetX()+3, g_pPlayer->GetY()-1, 0, 1000, NULL, false);
//”î      ”î   ”î ”î
//”î      ”î   ”î ”î
//”î”î”î”î”î   ”î ”î 
//”î      ”î   ”î ”î
//”î”î”î”î”î   ”î ”î
//             ”î ”î
//”î”î”î”î”î   ”î ”î
//    ”î   ”î”î”î ”î
//    ”î       ”î ”î
//”î”î”î”î”î”î”î”î”î
//        ”î      ”î
//”î”î”î”î”î      ”î
//”î              ”î
//”î”î”î”î”î      ”î



							//g_pPlayer->AddEffectStatus( EFFECTSTATUS_SIEGE_DEFENDERL, 100 );
//							ExecuteActionInfoFromMainNode(SKILL_WIDE_ICE_HAIL,g_pPlayer->GetX(), g_pPlayer->GetY(), 0,0,	0,	
//								g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 10000, NULL, false);	
//							g_pPlayer->RemoveEffectStatus(EFFECTSTATUS_BLINDNESS);
//							g_pZone->RemoveTileEffect(137, 80,EFFECTSTATUS_ELECTRIC_POST);
//							g_pZone->RemoveTileEffect(143, 86,EFFECTSTATUS_ELECTRIC_POST);
//							g_pZone->RemoveTileEffect(136, 93,EFFECTSTATUS_ELECTRIC_POST);
//							g_pZone->RemoveTileEffect(130, 87,EFFECTSTATUS_ELECTRIC_POST);

//							if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_WATER_ELEMENTAL))
//								g_pPlayer->RemoveEffectStatus( EFFECTSTATUS_WATER_ELEMENTAL );
//							else
//								g_pPlayer->AddEffectStatus( EFFECTSTATUS_WATER_ELEMENTAL, 0xFFFF );

//							g_pMP3->Stop();
//							g_pMP3->Open("data\\music\\dominator of darkness.mp3");
//							g_pMP3->Play( true );
//							g_pSystemMessage->Add("dominator of darkness.mp3 ¸¦ ¿¬ÁÖÇÕ´Ï´Ù");
//							g_bTestMusic = false;
						}
						// ALT + 3  --> °ø°Ý ¼Óµµ Fast
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
						{
							//g_pPlayer->SetWeaponSpeed( MCreature::WEAPON_SPEED_FAST );

							// 1 ~ 13
							/*
							if (++gtx_dest > 13)
							{
								gtx_dest = 1;
							}
							*/
							#ifdef __METROTECH_TEST__
								g_iAutoReload = (g_iAutoReload+1)%5;
							#endif
						}
					break;

					case DIK_4 :
						if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
						{
							int TempCount = ShowCursor(FALSE);
//							g_pUserInformation->IsNonPK = !g_pUserInformation->IsNonPK;
//							if(g_pUserInformation->IsNonPK)
//								g_pSystemMessage->Add("PK Not Available");
//							else
//								g_pSystemMessage->Add("PK Available");
							//g_pPlayer->AddEffectStatus( EFFECTSTATUS_WATER_ELEMENTAL_HEAL, 21 );
							//g_pPlayer->AddEffectStatus(EFFECTSTATUS_BLINDNESS, 300);
//							g_pMP3->Stop();
//							g_pMP3->Open("data\\music\\oblivion.mp3");
//							g_pMP3->Play( true );
//							g_pSystemMessage->Add("oblivion.mp3 ¸¦ ¿¬ÁÖÇÕ´Ï´Ù");
//							g_bTestMusic = false;
						}
#ifdef __METROTECH_TEST__
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
							g_bLight = !g_bLight;
//							g_bHolyWater = !g_bHolyWater;
#endif
						break;
					case DIK_5 :
#ifdef __METROTECH_TEST__
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
						UI_RunHorn(g_pZone->GetID());
#endif
//						if (g_pDXInput->KeyDown(DIK_RCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
//						{
//							g_pMP3->Stop();
//							g_pMP3->Open("data\\music\\rest.mp3");
//							g_pMP3->Play( true );
//							g_pSystemMessage->Add("rest.mp3 ¸¦ ¿¬ÁÖÇÕ´Ï´Ù");
//							g_bTestMusic = false;
//						}
						
						//						{
						//							g_pClientConfig->QuestItemColorSet += 15;
						//							
						//							if( g_pClientConfig->QuestItemColorSet >= 495 )
						//								g_pClientConfig->QuestItemColorSet = 0;
						//						} else
						//						{							
						//							g_pClientConfig->QuestItemColorSet += 15;
						//							
						//							if( g_pClientConfig->QuestItemColorSet >= 495 )
						//								g_pClientConfig->QuestItemColorSet = 0;
						//						}
						
						break;

					case DIK_6 :
//						{
//							if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
//							{
//								g_pMP3->Stop();
//								g_pMP3->Open("data\\music\\ruin.mp3");
//								g_pMP3->Play( true );
//								g_pSystemMessage->Add("ruin.mp3 ¸¦ ¿¬ÁÖÇÕ´Ï´Ù");
//								g_bTestMusic = false;
//								
//								(*g_pUserOption).BlendingShadow = !(*g_pUserOption).BlendingShadow;
//								if (g_pTopView!=NULL)
//								{
//									g_pTopView->ClearShadowManager();
//								}
//							}				
//						}
#ifdef __METROTECH_TEST__
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
						UI_RunMinigame(0);
#endif
						break;
						
					case DIK_7 :
//						{
//							if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
//							{
//								g_pMP3->Stop();
//								g_pMP3->Open("data\\music\\silence of battlefield.mp3");
//								g_pMP3->Play( true );
//								g_pSystemMessage->Add("silence of battlefield.mp3 ¸¦ ¿¬ÁÖÇÕ´Ï´Ù");
//								g_bTestMusic = false;
//							}				
//						}
						break;

					case DIK_8 :
#ifdef __METROTECH_TEST__
						if (g_pDXInput->KeyDown(DIK_RCONTROL) && g_pDXInput->KeyDown(DIK_RSHIFT))// &&
							//							g_pDXInput->KeyDown(DIK_LCONTROL) && g_pDXInput->KeyDown(DIK_RCONTROL))
						{
							g_iSpeed *= -1;
						}
#endif
//						if (g_pDXInput->KeyDown(DIK_RCONTROL) || g_pDXInput->KeyDown(DIK_LCONTROL))// &&
//							//							g_pDXInput->KeyDown(DIK_LCONTROL) && g_pDXInput->KeyDown(DIK_RCONTROL))
//						{
////							g_iSpeed *= -1;
//							g_pMP3->Stop();
//							g_pMP3->Open("data\\music\\underworld.mp3");
//							g_pMP3->Play( true );
//							g_pSystemMessage->Add("underworld.mp3 ¸¦ ¿¬ÁÖÇÕ´Ï´Ù");
//							g_bTestMusic = false;
//						}
						break;
						
					case DIK_9 :
//						if( g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
//						{
//							g_bTestMusic = !g_bTestMusic;
//
//							if(g_bTestMusic)
//							{
//								if (g_CurrentMusicNum >= 8 ) g_CurrentMusicNum = 0;
//								g_pMP3->Stop();
//								g_pMP3->Open(g_musicfilename[g_CurrentMusicNum].c_str());
//								g_pMP3->Play( false );
//							} else
//							{
//								g_pMP3->Stop();								
//							}
//						}

#ifdef __METROTECH_TEST__
						if (g_pDXInput->KeyDown(DIK_RCONTROL) && g_pDXInput->KeyDown(DIK_RSHIFT))// &&
//							g_pDXInput->KeyDown(DIK_LCONTROL) && g_pDXInput->KeyDown(DIK_RCONTROL))
						{
							int mi = 1;
							if(g_iSpeed < 0)
							{
								mi = -1;
								g_iSpeed *= -1;
							}
							g_iSpeed = (g_iSpeed%4)+1;
							g_iSpeed *= mi;
						}
#endif
						break;
					case DIK_0 :
						{
							if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
							{
								if( g_bTestMusic )
								{
									g_pMP3->Stop();									
								}
							}				
						}

					case DIK_Z :
						if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
						{
							CDirectDraw::RestoreAllSurfaces();
							CDirect3D::Restore();					
								
							if (g_pTopView!=NULL)
							{
								g_pTopView->RestoreSurface();
							}
						}
					break;
					
					case DIK_N :
						{
							if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
							{
								g_pUserInformation->IsNetmarble = !g_pUserInformation->IsNetmarble;
								if(g_pUserInformation->IsNetmarble)
									g_pSystemMessage->Add("Netmarble Set");
								else
									g_pSystemMessage->Add("Netmarble Unset");
							}
							else
								if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
								{
									g_pPCTalkBox->Release();
									
									//---------------------------------------------------
									// normal
									//---------------------------------------------------
									g_pPCTalkBox->SetType( PCTalkBox::NORMAL );
									
									
									int scriptID = 101;
									
									//---------------------------------------------------
									// PC Talk BoxÀÇ Á¤º¸ ¼³Á¤
									//---------------------------------------------------
									// SetContent¶ó°í ÀÌ¸§ÀÌ µÇ¾îÀÖÁö¸¸.. SubjectÀÌ´Ù. - -;
									g_pPCTalkBox->SetContent( "ÇÏÇÏÇÏ" );
									g_pPCTalkBox->SetNPCID( 297 );
									g_pPCTalkBox->SetCreatureType( 297 );
									g_pPCTalkBox->SetScriptID( scriptID );
									
									//---------------------------------------------------
									// °¢ string Ãß°¡
									//---------------------------------------------------
									int contentSize = 2;//g_pNPCScriptTable->GetContentSize( scriptID );
									
									//for (int i=0; i<contentSize; i++)
									{
										// g_PCTalkBox¿¡ Ãß°¡
										g_pPCTalkBox->AddString( "¿ìÇìÇì" );
										g_pPCTalkBox->AddString( "Å©ÇÏÇÏ" );
									}
									
									g_pUIDialog->PopupPCTalkDlg();
								}
						}
						break;
						
					case DIK_M :
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
						{
							if(g_pEventManager->IsEvent(EVENTID_METEOR))
							{
								g_pEventManager->RemoveEvent(EVENTID_METEOR);
							}
							else
							{
								MEvent event;
								event.eventID = EVENTID_METEOR;
								event.eventType = EVENTTYPE_ZONE;
								event.eventFlag = EVENTFLAG_FADE_SCREEN;
								event.parameter2 = 30 << 16;
//								event.parameter4 = EVENTBACKGROUNDID_COSMOS;
								g_pEventManager->AddEvent(event);
							}
						}
						break;

					case DIK_G:
						if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
						{
//							MFakeCreature* pFakeCreature = NewFakeCreature( g_pPlayer, g_pPlayer->GetX(), g_pPlayer->GetY() );
//							
//							pFakeCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_FAST_MOVE_ACTION );
//							
//							pFakeCreature->SetFakeCreatureFastMoveAction(g_pPlayer->GetX()+10, g_pPlayer->GetY()+10, 0, 0);
//							
//							//------------------------------------------------------
//							// Fake Creature¸¦ Zone¿¡ Ãß°¡
//							//------------------------------------------------------
//							if (!g_pZone->AddFakeCreature( pFakeCreature ))
//							{
//								delete pFakeCreature;
//							}
						}
						break;

					case DIK_H :
					{
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
						{
							if (g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION))
							{
								g_pPlayer->RemoveEffectStatus(EFFECTSTATUS_HALLUCINATION);
							}
							else
							{
								g_pPlayer->AddEffectStatus(EFFECTSTATUS_HALLUCINATION, 16*15);
							}
						}
					}
					break;
				#endif
				
			#endif


			#if defined(OUTPUT_DEBUG) && defined(_DEBUG)	//!defined(CONNECT_SERVER)
				/*
				case DIK_L :
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						if (g_pPlayer->IsVampire())
						{
							#ifdef OUTPUT_DEBUG
								#ifdef CONNECT_SERVER
									int value = g_pPlayer->GetBonusPoint();

									if (value > 0)
									{
										int strValue = value/4;
										int intValue = value - strValue;

										// NPC Ã³¸® packetÀ» º¸³½´Ù.
										CGUseBonusPoint _CGUseBonusPoint;

										// 1/4Àº STRÀ» ¿Ã¸°´Ù.
										_CGUseBonusPoint.setWhich( INC_STR );

										for (int i=0; i<strValue; i++)
										{
											g_pSocket->sendPacket( &_CGUseBonusPoint );

											#if defined(OUTPUT_DEBUG) && defined(__DEBUG_OUTPUT__)
													DEBUG_ADD_FORMAT( "[Send] %s", _CGUseBonusPoint.toString().c_str() );							
											#endif
										}
										
										// 3/4´Â INT¸¦ ¿Ã¸°´Ù.
										_CGUseBonusPoint.setWhich( INC_INT );

										for (i=0; i<strValue; i++)
										{
											g_pSocket->sendPacket( &_CGUseBonusPoint );

											#if defined(OUTPUT_DEBUG) && defined(__DEBUG_OUTPUT__)
													DEBUG_ADD_FORMAT( "[Send] %s", _CGUseBonusPoint.toString().c_str() );							
											#endif
										}	
									}

									
								#endif			
							#endif
						}
					}
				break;
				*/				

				//------------------------------------------------------------
				//
				// À½¾Ç º¼·ý Á¶Àý
				//
				//------------------------------------------------------------				
				case DIK_SUBTRACT :
				{
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						int volume = g_Music.GetVolume();

						DEBUG_ADD_FORMAT("[MIDI] Volume = %x", volume);

						volume -= 0x1000;

						if (volume < 0)
						{
							volume = 0;
						}

						g_Music.Pause();
						g_Music.SetVolume( volume );
						g_Music.Resume();
						
					}
					
				}
				break;
				
				case DIK_ADD :
				{
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						int volume = g_Music.GetVolume();

						DEBUG_ADD_FORMAT("[MIDI] Volume = %x", volume);

						volume += 0x1000;

						if (volume > 0xFFFF)
						{
							volume = 0xFFFF;
						}

						g_Music.Pause();
						g_Music.SetVolume( volume );
						g_Music.Resume();
						
					}
					
				}
				break;
				
				//------------------------------------------------------------
				//
				// wide chatting
				//
				//------------------------------------------------------------
				/*
				case DIK_E : 
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						if (g_HISTORY_LINE > 4)//HISTORY_LINE) ¤»¤»
						{
							g_HISTORY_LINE = 4;
						}
						else
						{
							g_HISTORY_LINE = 16;
						}
					}
					//---------------------------------------------------------
					// Wolf º¯½Å test
					//---------------------------------------------------------
					else if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
					{
						MActionResult* pResult = new MActionResult;

						if (g_pPlayer->GetCreatureType()!=186)
						{									
							pResult->Add( new MActionResultNodeChangeCreatureType( g_pPlayer->GetID(), 186 ) );
							//g_pPlayer->SetCreatureType( 185 );
							//g_pPlayer->SetFlyingCreature();
						}
						else
						{
							pResult->Add( new MActionResultNodeChangeCreatureType( g_pPlayer->GetID(), CREATURETYPE_VAMPIRE_MALE ) );
							//g_pPlayer->SetCreatureType( CREATURETYPE_VAMPIRE_MALE );
							//g_pPlayer->SetGroundCreature();
						}

						//--------------------------------------------------
						// ´Á´ë º¯½Å test
						//--------------------------------------------------								
						ExecuteActionInfoFromMainNode(
							RESULT_MAGIC_TRANSFORM_TO_WOLF,										// »ç¿ë ±â¼ú ¹øÈ£
						
							g_pPlayer->GetX(), g_pPlayer->GetY(), 0,
							g_pPlayer->GetDirection(),														// »ç¿ë ¹æÇâ
							
							OBJECTID_NULL,												// ¸ñÇ¥¿¡ ´ëÇÑ Á¤º¸
							g_pPlayer->GetX(), g_pPlayer->GetY(), 0, 
							
							0,													// ±â¼úÀÇ (³²Àº) Áö¼Ó ½Ã°£		
							
							pResult, //NULL,
							
							false);			// ±â¼ú Ã·ºÎÅÍ ½ÃÀÛÇÑ´Ù.

						g_pPlayer->SetDelay( 1000 );
					}

				break;
				*/
				//------------------------------------------------------------
				// Çï±â
				//------------------------------------------------------------
				/*
				case DIK_H :
				{
					// helicopter Å×½ºÆ®
					if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
					{
						TYPE_OBJECTID creatureID = g_pPlayer->GetID();

						if (g_pZone->GetHelicopter( creatureID )==NULL)
						{
							g_pZone->AddHelicopter( creatureID, 0, 0 );
						}
						else
						{
							g_pZone->RemoveHelicopter( creatureID );
						}
					}
				}
				break;
				*/

				//------------------------------------------------------------
				// Warp
				//------------------------------------------------------------
				case DIK_W :
				{
					if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
					{
						int x, y;

						while (1)
						{
							x = rand()%g_pZone->GetWidth();
							y = rand()%g_pZone->GetHeight();

							if (!g_pZone->GetSector(x,y).IsBlockGround())
							{
								break;
							}
						}

						g_pPlayer->SetStop();						
						g_pPlayer->MovePosition( x, y );
					}
				}
				break;

				//------------------------------------------------------------
				//
				//			Portal
				// 
				//------------------------------------------------------------
				/*
				case DIK_P : 
				{					
					//------------------------------------------------------------
					// ¿ÞÂÊ ´©¸£¸é µé¾î°¡±â
					//------------------------------------------------------------
					if (g_pDXInput->KeyDown(DIK_LMENU))
					{
						MZone::CREATURE_MAP::const_iterator iCreature = g_pZone->GetCreatureBegin();

						for (int i=0; i<g_pZone->GetCreatureNumber(); i++)
						{
							MCreature* pCreature = (*iCreature).second;

							//--------------------------------------------------
							// Player, NPC°¡ ¾Æ´Ñ °æ¿ì
							//--------------------------------------------------					
							if (pCreature->GetID()!=g_pPlayer->GetID()
								&& pCreature->IsVampire()
								&& !pCreature->IsNPC())
							{					
								AddVampirePortal(
										0,								// ÀÌÆåÆ®ÀÇ OID
										pCreature->GetName(),								// Æ÷Å» ÁÖÀÎ
										pCreature->GetX(), pCreature->GetY(),		// Æ÷Å»ÀÇ ÁÂÇ¥
										16*10,							// Æ÷Å»ÀÇ Áö¼Ó ½Ã°£
										
										// ¸ñÇ¥ Á¤º¸
										rand()%4+11,							// Æ÷Å»ÀÇ ¸ñÇ¥ Á¸ ID
										rand()%255,			// Æ÷Å»ÀÇ ¸ñÇ¥ ÁÂÇ¥ x
										rand()%255,			// Æ÷Å»ÀÇ ¸ñÇ¥ ÁÂÇ¥ y

										(rand()%2? true : false) );

								MoveIntoPortal( pCreature );

								g_pZone->RemoveCreature( pCreature->GetID() );
								
								break;
							}

							iCreature++;
						}
					}
					//------------------------------------------------------------
					// ¿À¸¥ÂÊ ´©¸£¸é ³ª¿À±â
					//------------------------------------------------------------
					else if (g_pDXInput->KeyDown(DIK_RMENU))
					{
						MCreature* pCreature = AddClientCreature();

						if (pCreature!=NULL
							&& pCreature->IsVampire()
							&& !pCreature->IsNPC())
						{
							AddVampirePortal(
										0,								// ÀÌÆåÆ®ÀÇ OID
										pCreature->GetName(),								// Æ÷Å» ÁÖÀÎ
										pCreature->GetX(), pCreature->GetY(),		// Æ÷Å»ÀÇ ÁÂÇ¥
										16*10,							// Æ÷Å»ÀÇ Áö¼Ó ½Ã°£
										
										// ¸ñÇ¥ Á¤º¸
										rand()%4+11,							// Æ÷Å»ÀÇ ¸ñÇ¥ Á¸ ID
										rand()%255,			// Æ÷Å»ÀÇ ¸ñÇ¥ ÁÂÇ¥ x
										rand()%255,			// Æ÷Å»ÀÇ ¸ñÇ¥ ÁÂÇ¥ y

										(rand()%2? true : false) );

							ComeFromPortal( pCreature );
						}
					}					
				}
				break;				
				*/

				//------------------------------------------------------------
				//
				// casket
				//
				//------------------------------------------------------------
				case DIK_V : 
				{
					if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
					{
						MZone::CREATURE_MAP::const_iterator iCreature = g_pZone->GetCreatureBegin();

						for (int i=0; i<g_pZone->GetCreatureNumber(); i++)
						{
							MCreature* pCreature = (*iCreature).second;

							//--------------------------------------------------
							// Player, NPC°¡ ¾Æ´Ñ °æ¿ì
							//--------------------------------------------------					
							if (pCreature->GetID()!=g_pPlayer->GetID()
								&& pCreature->IsVampire()
								&& !pCreature->IsNPC())
							{							
								if (pCreature->IsInCasket())
								{
									pCreature->RemoveCasket();
								}
								else
								{
									pCreature->AddCasket( rand()%3 );
								}							
							}

							iCreature++;
						}
					}
				}
				break;

				case DIK_C : 
				{
					if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
					{
						if (g_pPlayer->IsInCasket())
						{
							g_pPlayer->RemoveCasket();
						}
						else
						{
							g_pPlayer->AddCasket( rand()%3 );
						}

						/*
						#ifdef OUTPUT_DEBUG
							if (g_pDebugMessage!=NULL)
							{
								char logFile[128];
								strcpy(logFile, g_pDebugMessage->GetFilename());
								
								g_pDebugMessage->Init(MAX_DEBUGMESSAGE, 256, logFile);
							}
						#endif
						*/
						/*
						int x = g_pPlayer->GetX();
						int y = g_pPlayer->GetY();
						int dir = g_pPlayer->GetDirection();

						MFakeCreature* pCreature = new MFakeCreature;

						pCreature->SetID( 0 );
						pCreature->SetSameBody( g_pPlayer );
						pCreature->SetSameWear( g_pPlayer );
						
						pCreature->SetZone( g_pZone );

						pCreature->SetPosition( x, y );
						pCreature->SetServerPosition( x, y );
						pCreature->SetDirection( dir );
						pCreature->SetCurrentDirection( dir );
						pCreature->SetAction( ACTION_ATTACK );
						pCreature->SetWeaponSpeed( g_pPlayer->GetWeaponSpeed() );

						pCreature->SetFakeCreatureType( MFakeCreature::FAKE_CREATURE_FADE_ACTION );						

						if (!g_pZone->AddFakeCreature( pCreature ))
						{
							delete pCreature;
						}
						*/
					}
				}
				break;

				//------------------------------------------------------------
				//
				// ÄÄÇ»ÅÍ ¶ç¿ì±â
				//
				//------------------------------------------------------------
				case DIK_D : 
					if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
					{
						UI_RunComputer();
					}					
				break;

				//------------------------------------------------------------
				//
				// Ä³¸¯ÅÍ Ãß°¡ - packet Å×½ºÆ®¸¦ À§ÇØ¼­
				//
				//------------------------------------------------------------
				case DIK_K : 
				{
					if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
					{
						int objectID = MFakeCreature::GetFakeID();
						char name[80];
						sprintf(name, "Fake%d", objectID);
						int x = g_pPlayer->GetX() + rand()%30-15;
						int y = g_pPlayer->GetY() + rand()%30-15;
						int dir = rand()%8;

						if (x >=0 && x < g_pZone->GetWidth()
							&& y >= 0 && y < g_pZone->GetHeight())
						{
							MCreature* pCreature = g_pZone->GetCreature( objectID );

							if (pCreature!=NULL)
							{
								break;
							}


							//------------------------------------------------------------
							// Vampire Ãß°¡
							//------------------------------------------------------------
							if (rand()%2)
							{
								pCreature = new MCreatureWear;

								pCreature->SetZone( g_pZone );
								
								//--------------------------------------------------
								// CreatureType ¼³Á¤
								//--------------------------------------------------
								if (rand()%3)
								{
									pCreature->SetCreatureType( CREATURETYPE_BAT );
									pCreature->SetFlyingCreature();
								}
								else
								{
									pCreature->SetCreatureType( (rand()%2? CREATURETYPE_VAMPIRE_MALE1 : CREATURETYPE_VAMPIRE_FEMALE1) );
								}

								pCreature->SetMale( pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_MALE1 );						
								

								pCreature->SetName( name );

								// ÀÓ½Ã·Î
								pCreature->SetGuildNumber( 2 );

								pCreature->SetGroundCreature();
								pCreature->SetID( objectID );
								//pCreature->SetAction(ACTION_MOVE);
								
								pCreature->SetPosition( x, y );
								pCreature->SetServerPosition( x, y );
								pCreature->SetDirection( dir );
								pCreature->SetCurrentDirection( dir );
								pCreature->SetAction( ACTION_STAND );

								// »ö±ò
								pCreature->SetBodyColor1( rand()%MAX_COLORSET );
								pCreature->SetBodyColor2( rand()%MAX_COLORSET );
								
								pCreature->SetStatus( MODIFY_MAX_HP, 80 );
								pCreature->SetStatus( MODIFY_CURRENT_HP, 100 );
								pCreature->SetStatus( MODIFY_ALIGNMENT, 7500 );

								pCreature->SetWeaponSpeed( 70 );							

								if (!g_pZone->AddCreature( pCreature ))
								{
									delete pCreature;
								}
							}
							//------------------------------------------------------------
							// Slayer Ãß°¡
							//------------------------------------------------------------
							else
							{
								MCreatureWear* pCreatureWear = new MCreatureWear;

								pCreatureWear->SetZone( g_pZone );

								pCreatureWear->SetCreatureType( (rand()%2)? CREATURETYPE_SLAYER_MALE : CREATURETYPE_SLAYER_FEMALE );
								pCreatureWear->SetGroundCreature();

								
								
								pCreatureWear->SetID( objectID );
								//pCreatureWear->SetAction(ACTION_MOVE);
								pCreatureWear->SetPosition( x, y );
								pCreatureWear->SetServerPosition( x, y );
								pCreatureWear->SetDirection( dir );
								pCreatureWear->SetCurrentDirection( dir );
								pCreatureWear->SetAction( ACTION_STAND );

								// ÇÇºÎ»ö
								pCreatureWear->SetBodyColor2( rand()%MAX_COLORSET );

								pCreatureWear->SetStatus( MODIFY_MAX_HP, 80 );
								pCreatureWear->SetStatus( MODIFY_CURRENT_HP, 100 );

								// ÀÌ¸§
								pCreatureWear->SetName( name );

								// ÀÓ½Ã·Î NPCº¹ÀåÀ» ÀÔÈù´Ù.
								SetAddonToSlayer( pCreatureWear, 250 );

								
								
								if (!g_pZone->AddCreature( pCreatureWear ))
								{
									delete pCreatureWear;
								}
							}
						}
					}
				}
				break;
				

				//-------------------------------------------------------------
				// ¹æÇâ ¹Ù²Ù±â
				//-------------------------------------------------------------
				case DIK_LEFT :
				case DIK_RIGHT :
				{
					MZone::CREATURE_MAP::const_iterator iCreature = g_pZone->GetCreatureBegin();

					for (int i=0; i<g_pZone->GetCreatureNumber(); i++)
					{
						MCreature* pCreature = (*iCreature).second;

						//--------------------------------------------------
						// Player, NPC°¡ ¾Æ´Ñ °æ¿ì
						//--------------------------------------------------					
						if (pCreature->GetID()!=g_pPlayer->GetID()
							&& !pCreature->IsNPC())
						{							
							int dir = pCreature->GetDirection();

							if (key==DIK_LEFT)
							{
								dir = (dir + 7) % 8;
							}
							else
							{
								dir = (dir + 1) % 8;
							}

							pCreature->SetDirection( dir );									
						}

						iCreature++;
					}
				}
				break;

				//------------------------------------
				// HP È¸º¹ test
				//------------------------------------
				case DIK_R : 
					if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
					{
						g_pPlayer->SetStatus( MODIFY_CURRENT_HP, 0 );
						g_pPlayer->SetStatus( MODIFY_MAX_HP, 100 );
						g_pPlayer->SetRecoveryHP( 1, 100, 200 );
					}
				break;


				//------------------------------------------------------------
				//
				// zone ÀÌµ¿ ÀÚµ¿..
				//
				//------------------------------------------------------------
				case DIK_LBRACKET : case DIK_RBRACKET :
					/*
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						char str[80];

						if (key==DIK_LBRACKET)
						{
							// ½½·¹ÀÌ¾î ±æµå
							strcpy(str, "*warp 3 229 52");
						}
						else
						{
							// ¹ìÆÄÀÌ¾î ±æµå
							sprintf(str, "*warp 9 55 78");
						}

						#ifdef CONNECT_SERVER
							CGSay _CGSay;
							_CGSay.setMessage( str );	//pWansungString );
							g_pSocket->sendPacket( &_CGSay );

							#if defined(OUTPUT_DEBUG) && defined(__DEBUG_OUTPUT__)
									DEBUG_ADD_FORMAT("[Send] %s", _CGSay.toString().c_str() );
							#endif	
						#endif
					}
					*/
				break;						

				/*
				case DIK_GRAVE :
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						(*g_pUIDialog).PopupDirectionSelectDlg(0, 0);
					}					
				break;
				*/

				

//				case DIK_5 :
//				{	
//					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
//					{
//						g_pPlayer->SetMoveDevice( MCreature::MOVE_DEVICE_WALK );
//						g_pPlayer->SetBasicActionInfo( SKILL_ATTACK_MELEE );
//						g_pPlayer->SetStop();
//								
//						if (g_pPlayer->IsSlayer())
//						{
//							if (g_pPlayer->IsMale())
//							{
//								g_pPlayer->SetCreatureType( CREATURETYPE_SLAYER_FEMALE );
//							}
//							else
//							{
//								g_pPlayer->SetCreatureType( CREATURETYPE_SLAYER_MALE );
//							}
//						}
//						else
//						{
//							if (g_pPlayer->IsMale())
//							{
//								g_pPlayer->SetCreatureType( CREATURETYPE_VAMPIRE_FEMALE1 );
//							}
//							else
//							{
//								g_pPlayer->SetCreatureType( CREATURETYPE_VAMPIRE_MALE1 );
//							}
//						}
//						/*
//						if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
//							for (int i=0; i<8; i++)
//							{
//								const MItem* pItem = g_Inventory.GetItem(i, 0);
//
//								if (pItem!=NULL)
//								{
//									AddNewInventoryEffect( pItem->GetID(),
//														RESULT_MAGIC_CREATE_HOLY_WATER,
//														80	// 5ÃÊ
//													);
//								}
//							}
//							*/
//					}
//				}
//				break;

				

//				case DIK_8 :
//				{
//					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
//					{
//						if (g_pPlayer->HasEffectStatus(EFFECTSTATUS_SNIPPING_MODE))
//						{
//							g_pPlayer->RemoveEffectStatus(EFFECTSTATUS_SNIPPING_MODE);
//						}
//						else
//						{
//							g_pPlayer->AddEffectStatus(EFFECTSTATUS_SNIPPING_MODE, 16*10);
//							g_pPlayer->SetInvisible();
//						}
//					}				
//				}
//				break;

//				case DIK_9 :
//				{
//					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
//					{
//						if (g_pPlayer->HasEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS))
//						{
//							g_pPlayer->RemoveEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS);
//						}
//						else
//						{
//							g_pPlayer->AddEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS, 16*10);
//						}
////						if (g_pPlayer->HasEffectStatus(EFFECTSTATUS_SHUT_DOWN))
////						{
////							g_pPlayer->RemoveEffectStatus(EFFECTSTATUS_SHUT_DOWN);
////						}
////						else
////						{
////							g_pPlayer->AddEffectStatus(EFFECTSTATUS_SHUT_DOWN, 16*10);
////						}
//					}				
//				}
//				break;

				

				/*
				case DIK_1 :
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						MItem*	pBlade = new MBlade;	pBlade->SetItemType( 0 ); pBlade->SetItemOption( 0 );
						g_pPlayer->RemoveAddon( ADDON_RIGHTHAND );
						g_pPlayer->SetAddonItem( pBlade );				
					}
				break;

				case DIK_2 :
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						MItem*	pSword = new MSword;	pSword->SetItemType( 0 );	pSword->SetItemOption( 0 );
						g_pPlayer->RemoveAddon( ADDON_RIGHTHAND );
						g_pPlayer->SetAddonItem( pSword );
					}				
				break;

				case DIK_7 :
				case DIK_8 :
				case DIK_9 :
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						static int itemType[] = { 0, 1, 5 };
						MItem*	pShield = new MShield;	pShield->SetItemType( itemType[key-DIK_7] ); pShield->SetItemOption( 0 );
						g_pPlayer->RemoveAddon( ADDON_LEFTHAND );
						g_pPlayer->SetAddonItem( pShield );				
					}
				break;

				case DIK_3 :			
				case DIK_4 :
				case DIK_5 :			
				case DIK_6 :
				{
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						MGunItem* pGunItem;

						#ifdef __FIX_GUNFRAME__
							int ac;
						#endif
						
						switch (key)
						{
							case DIK_3 :
								pGunItem = new MGunTR;
								#ifdef __FIX_GUNFRAME__
									ac = SKILL_ATTACK_GUN_TR;						
								#endif
							break;

							case DIK_4 :
								pGunItem = new MGunSMG;
								#ifdef __FIX_GUNFRAME__
									ac = SKILL_ATTACK_GUN_SMG;						
								#endif
							break;

							case DIK_5 :
								pGunItem = new MGunAR;
								#ifdef __FIX_GUNFRAME__
									ac = SKILL_ATTACK_GUN_AR;						
								#endif
							break;

							case DIK_6 :
								pGunItem = new MGunSG;
								#ifdef __FIX_GUNFRAME__
									ac = SKILL_ATTACK_GUN_SG;						
								#endif
							break;

							
						}

						#ifdef __FIX_GUNFRAME__
							// ÀÓ½Ã ÃÑ ºÒ²É
							g_pPlayer->ClearAttachEffect();
							MAttachEffect* pEffect = g_pPlayer->CreateAttachEffect( 
																(*g_pActionInfoTable)[ac].GetActionEffectSpriteType(), 
																//(*g_pActionInfoTable)[m_nUsedActionInfo].GetDelay()
																0xFFFF
																);	// Áö¼Ó ½Ã°£

							if (pEffect != NULL)
							{
								pEffect->SetDirection( g_pPlayer->GetDirection() );

								pEffect->SetLink( ac, NULL );

								// ºÙ¾î¾ß ÇÏ´Â Ä³¸¯ÅÍ
								pEffect->SetAttachCreatureID( 1 );			
							}
						#endif

						pGunItem->SetItemType( 0 ); pGunItem->SetItemOption( 0 );				
					
						MMagazine* pMagazine = (MMagazine*)MItem::NewItem( (ITEM_CLASS)ITEM_CLASS_MAGAZINE );

						// ÀÇ¹Ì ¾øÀ½ - -;
						pMagazine->SetID( 0 );

						// ÀÌ°Å´Â ÃÑ¿¡ ¸ÂÃç¼­ ÇØÁà¾ßµÈ´Ù.
						for (int j=0; j<g_ItemTable[ITEM_CLASS_MAGAZINE].GetSize(); j++)			
						{
							pMagazine->SetItemType(	j );

							if (pMagazine->IsInsertToItem( pGunItem ))
							{
								break;
							}
						}

						// ÀÇ¹Ì ¾øÀ½
						pMagazine->SetItemOption( 0 );

						// ÅºÃ¢ °³¼ö
						pMagazine->SetNumber( 0xFFFF );

						//------------------------------------
						// ÅºÃ¢ ¼³Á¤
						//------------------------------------
						pGunItem->SetMagazine( pMagazine );

						g_pPlayer->RemoveAddon( ADDON_RIGHTHAND );
						g_pPlayer->RemoveAddon( ADDON_LEFTHAND );
						g_pPlayer->SetAddonItem( pGunItem );
					}
				}
				break;
				*/

				case DIK_A :
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						//------------------------------------------------------
						// »ç¿ëÀÚ
						//------------------------------------------------------
						MCreature* pCreature = g_pZone->GetCreature( 1000 );

						if (pCreature!=NULL)
						{
							//------------------------------------------------------
							// TileOK2·Î ÀÎÇÑ °á°ú 
							//------------------------------------------------------
							MActionResult* pResult = new MActionResult;
	
							pResult->Add( new MActionResultNodeActionInfo( 
														MAGIC_LIGHT,
														pCreature->GetID(),
														g_pPlayer->GetID(), 
														g_pPlayer->GetX(),
														g_pPlayer->GetY()
														) 
										);
								
							//------------------------------------------------------
							// TileOK2·Î ÀÎÇÑ °á°ú Ãß°¡
							//------------------------------------------------------
							//Duration_t	m_Duration;
							pCreature->PacketSpecialActionToSector(
												MAGIC_LIGHT, 
												g_pPlayer->GetX(), g_pPlayer->GetY(),
												pResult						// °á°ú
							);

							// ¹æÇâÀ» ¹Ù¶óº¸±â
							pCreature->SetDirectionToPosition(g_pPlayer->GetX(), g_pPlayer->GetY());
					
						}
					}
				break;


				/*		
				case DIK_Q :								
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						if (g_pPlayer->IsDead())
						{						
							g_pPlayer->SetAlive();
							if (g_pTopView!=NULL)
							{
								g_pTopView->SetFadeEnd();
							}
						}
						else
						{
							// Á×Àº °æ¿ì..
							g_pPlayer->SetDead();						
						}
					}
				break;

				case DIK_W :								
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						for (int i=0; i<g_MaxNPC; i++)
						{
							MCreature* pCreature = g_pZone->GetCreature(1000+i);

							if (pCreature!=NULL)
							{
								if (pCreature->IsDead())
								{
									pCreature->SetAlive();
								}
								else
								{
									pCreature->SetDead();
								}
							}
						}
					}
				break;
				*/
			#endif			
		}
	}
	else // if (event==CDirectInput::KEYUP)
	{
	}
}



//-----------------------------------------------------------------------------
// ProcessInput RButton Down
//-----------------------------------------------------------------------------
// pObject¿Í °ü·Ã..ÀÌ ÀÖÀ» ¼öµµ ÀÖ°í..
//-----------------------------------------------------------------------------
void
ProcessInputRButtonDown(MObject* pObject, bool bForceAttack = false)
{
	g_bRButtonDown = TRUE;
	
	// ¼±ÅÃµÈ sector°¡ ¾ø°Ô ÇÑ´Ù.
	if (g_pTopView!=NULL)
	{
		g_pTopView->SetSelectedSectorNULL();
	}
	// add by Sonic
		//START_START
		//CRYPT_START
		if(g_CheckErrorTime>=5)
		{
			g_CheckErrorTime=0;
			SetMode(MODE_QUIT);
			return;
		}
		//VM_END
		//CRYPT_END
	// end 
//	UI_RunRaceWarNotice(0xFFFFFFFF);
	
	//bool bFreePKZone = g_pZoneTable->Get( g_pZone->GetID() )->FreePK;
	if(NULL != pObject)
		bForceAttack |= (g_pObjectSelector->IsWarEnemy( (MCreature*) pObject ) == TRUE);
	//bForceAttack |= ((g_pZone->GetID() == 1303)||(g_pZone->GetID() == 1020));
//	bForceAttack |= g_pZone->IsFreePKZone();

	if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_DRAGON_EYES))
		return;
	if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
	{
		if(!g_pPlayer->IsStop()) return;
		if(g_pPlayer->GetSpecialActionInfo() == MAGIC_UN_TRANSFORM)
		{
		//	if(!g_pPlayer->IsStop()) return;
			CGSkillToSelf _CGSkillToSelf;
			_CGSkillToSelf.setSkillType( MAGIC_UN_TRANSFORM );
			g_pSocket->sendPacket( &_CGSkillToSelf );
		//	g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_SYLPH_SUMMON_GETOFF );
			return ;
		}
		else if(g_pPlayer->GetSpecialActionInfo() == SKILL_TURRET_FIRE)
			PlaySound( SOUND_SLAYER_TURRET_FIRE_START );
	}
	if( HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(g_pPlayer) )
//		&& //g_pPlayer->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH ) && 
//		!g_bZoneSafe && !g_pPlayer->IsInSafeSector() 
		) 
	{ 
		if(!g_pPlayer->IsStop()) return;
		
		if(!g_bZoneSafe && !g_pPlayer->IsInSafeSector())
		{
			CGSkillToSelf _CGSkillToSelf;
			_CGSkillToSelf.setSkillType( MAGIC_UN_TRANSFORM );
	//		_CGSkillToSelf.setCEffectID( m_pEffectTarget->GetEffectID() );
			g_pSocket->sendPacket( &_CGSkillToSelf );
			g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_SYLPH_SUMMON_GETOFF );
			(*g_pSkillInfoTable)[SKILL_SUMMON_SYLPH].SetAvailableTime( 4000 );
	//		gC_vs_ui.UnselectSkill();
		}
		else 
			return;
	} else
	//---------------------------------------------------------------
	// 
	// ¿ÀÅä¹ÙÀÌ¸¦ Å¸°í ÀÖ´Â °æ¿ì 
	//
	//---------------------------------------------------------------
	// ¿ÀÅä¹ÙÀÌ¿¡¼­ ³»¸°´Ù.
	//---------------------------------------------------------------
	if (g_pPlayer->GetMoveDevice()==MCreature::MOVE_DEVICE_RIDE
		&& g_pPlayer->IsStop()
		&& !g_pPlayer->HasEffectStatus(EFFECTSTATUS_CURSE_PARALYSIS)
		&& !g_pPlayer->HasEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS)
		&& !g_pPlayer->HasEffectStatus(EFFECTSTATUS_EXPLOSION_WATER)
		)
	{
#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("MotorOff");
#endif
	
		if( ! IsExistCorpseFromPlayer(dynamic_cast<MCreature*>(g_pPlayer), 670) )
		{		
			CGGetOffMotorCycle _CGGetOffMotorCycle;
			
			_CGGetOffMotorCycle.setObjectID( 0 );
			
			g_pSocket->sendPacket(&_CGGetOffMotorCycle);
			
#ifdef __DEBUG_OUTPUT__
			DEBUG_ADD_FORMAT("[Send] %s", _CGGetOffMotorCycle.toString().c_str() );
#endif
			
			g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_MOTORCYCLE_GETOFF );
		} else
		{
			g_pSystemMessage->Add((*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_ACTION_MOTORCYCLE_FLAG].GetString() );
		}
	}
	//---------------------------------------------------------------
	//
	// Burrow µÈ °æ¿ì ¶¥À¸·Î ¼Ú¾Æ³ª¿Â´Ù´Â packetÀ» º¸³½´Ù.
	//
	//---------------------------------------------------------------
	else if (g_pPlayer->IsUndergroundCreature())
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("Unburrow");
		#endif

		CGUnburrow _CGUnburrow;

		_CGUnburrow.setX( g_pPlayer->GetX() );
		_CGUnburrow.setY( g_pPlayer->GetY() );
		_CGUnburrow.setDir( g_pPlayer->GetDirection() );

		g_pSocket->sendPacket(&_CGUnburrow);

		#ifdef __DEBUG_OUTPUT__
			DEBUG_ADD_FORMAT("[Send] %s", _CGUnburrow.toString().c_str() );
		#endif

		g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_SKILL_SUCCESS );
	}
	//---------------------------------------------------------------
	//
	// object/tile¿¡ ±â¼úÀ» »ç¿ëÇÏ´Â °æ¿ì 
	//
	//---------------------------------------------------------------
	else
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("TileSkill");
		#endif

		// sector ¼±ÅÃÇÏ±â
		//g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);
		//g_pTopView->SetSelectedSector(g_SelectSector.x, g_SelectSector.y);

		//
		//MObject*	pObject = g_pTopView->GetSelectedObject(g_x, g_y);
	
		//--------------------------------------------------
		// ¼±ÅÃÇÑ °÷¿¡ Object°¡ ¾øÀ¸¸é ..
		//--------------------------------------------------
		if (pObject == NULL)
		{		
			g_pPlayer->UnSetRepeatAction();
			//g_pPlayer->TraceNextNULL();

			//--------------------------------------------------
			// ÀÚ½ÅÇÑÅ× Æ¯¼ö ±â¼ú »ç¿ë
			//--------------------------------------------------
			if (g_pPlayer->SelfSpecialAction())
			{
				g_pPlayer->SetRepeatAction();
			}
			//--------------------------------------------------
			// ÀÚ½ÅÇÑÅ× »ç¿ëÇÏ´Â Æ¯¼ö ±â¼úÀÌ ¾Æ´Ñ °æ¿ì
			//--------------------------------------------------				
			else if (!g_pPlayer->IsSpecialActionInfoTargetSelf())
			{
				g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);

				//--------------------------------------------------				
				// Zone¿¡ Æ¯¼ö ±â¼úÀ» »ç¿ëÇÑ´Ù.
				//--------------------------------------------------				
				if (g_pPlayer->TraceSectorToSpecialAction( g_SelectSector.x, g_SelectSector.y ))
				{
					// ¼±ÅÃµÈ Sector·Î Ç¥½ÃÇÑ´Ù.
					g_pTopView->SetSelectedSector( g_SelectSector );
				
					g_pPlayer->SetRepeatAction();

					g_bPreviousMove = false;
				}
			}

			//----------------------------------
			// TileÀÌ³ª ÀÚ½Å¿¡°Ô ÇÏ´Â Çàµ¿ ¹Ýº¹ ¼³Á¤
			//----------------------------------						
			g_pTopView->SetSelectedSectorNULL();
			
		}		
		//--------------------------------------------------
		// ¼±ÅÃµÈ Object¿¡ ´ëÇØ¼­ 
		//--------------------------------------------------
		else 
		{				
			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("Obj");
			#endif

			switch (pObject->GetObjectType())
			{	
				//--------------------------------------------------
				// Creature
				//--------------------------------------------------
				case MObject::TYPE_CREATURE :		
				{
					// ¸ñÇ¥°¡ µÇ´Â Creature
					MCreature* pCreature = ((MCreature*)pObject);
					
					//--------------------------------------------------
					// ´Ù¸¥ ¾Ö´ú¿¡°Ô »ç¿ëÇÏ´Â°Ç°¡?
					//--------------------------------------------------
					if (g_pPlayer->TraceCreatureToSpecialAction( pCreature->GetID(), bForceAttack ))
					{
						g_pPlayer->SetRepeatAction();								

						g_bPreviousMove = false;
					}
					else if (!g_pPlayer->IsSpecialActionInfoTargetOther())
					{
						//--------------------------------------------------
						// (!) ´Ù¸¥ Creature¿¡ »ç¿ëÇÏ´Â°Ô ¾Æ´Ò °æ¿ì
						//--------------------------------------------------
						// ÀÚ½ÅÇÑÅ× Æ¯¼ö ±â¼ú »ç¿ë
						if (g_pPlayer->SelfSpecialAction())
						{
							g_pPlayer->SetRepeatAction();
						}
						else
						{	
							//--------------------------------------------------
							// ÀÚ½Å¿¡°Ô »ç¿ëÇÏ´Â°Ô ¾Æ´Ñ °æ¿ì
							//--------------------------------------------------
							// ¼±ÅÃµÆ´ø CreatureÀÇ ÁÂÇ¥ÀÇ ..
							// Zone¿¡ Æ¯¼ö ±â¼úÀ» »ç¿ëÇÒ±î?
							if (!g_pPlayer->IsSpecialActionInfoTargetSelf()
								&& g_pPlayer->TraceSectorToSpecialAction( pCreature->GetX(), pCreature->GetY()))
							{
								g_pPlayer->SetRepeatAction();

								g_bPreviousMove = false;
							}
						}									
					}
					
					//----------------------------------
					// ¼±ÅÃ sectorÇ¥½Ã ¾ø¾Ö±â
					//----------------------------------
					g_pTopView->SetSelectedSectorNULL();
				}
				break;

				//--------------------------------------------------
				// Item
				//--------------------------------------------------
				// ItemÀÏ °æ¿ì´Â.. ÀÚ½ÅÇÑÅ× »ç¿ëÇÏ°Å³ª Zone¿¡ »ç¿ëÇÑ´Ù.
				case MObject::TYPE_ITEM :		
				{
					//--------------------------------------------------
					// ÀÚ½ÅÇÑÅ× Æ¯¼ö ±â¼ú »ç¿ë
					//--------------------------------------------------
					if (g_pPlayer->SelfSpecialAction())
					{
						g_pPlayer->SetRepeatAction();
					}
					else
					{	
						//--------------------------------------------------
						// ÀÚ½ÅÇÑÅ× »ç¿ëÇÏ´Â°Ô ¾Æ´Ñ °æ¿ì..
						//--------------------------------------------------
						// ¸ñÇ¥°¡ µÇ´Â Creature
						if (!g_pPlayer->IsSpecialActionInfoTargetSelf())
						{
							MItem* pItem = ((MItem*)pObject);

							if (pItem!=NULL)
							{
								// ¿ÍÀÏµå ¿ïÇÁ¸¦ ½ÃÃ¼ÇÑÅ× ½èÀ»¶§ ¿¹¿Ü Ã³¸®
								if(g_pPlayer->GetSpecialActionInfo() == SKILL_WILD_WOLF)
								{
									g_pPlayer->UseWildWolf_Corpse(pItem);
								}
								// ¼±ÅÃµÆ´ø CreatureÀÇ ÁÂÇ¥ÀÇ ..
								// Zone¿¡ Æ¯¼ö ±â¼úÀ» »ç¿ëÇÑ´Ù.
								else if (g_pPlayer->TraceSectorToSpecialAction( pItem->GetX(), pItem->GetY()))
								{
									g_pPlayer->SetRepeatAction();
									
									g_bPreviousMove = false;
								}
							}
						}
					}
				}
				break;

			}
		}		
	}

	#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("RbOK");
	#endif
}

//---------------------------------------------------------------------------
// Process Game Input
//---------------------------------------------------------------------------
void
CGameUpdate::ProcessInput()
{
	if(g_pEventManager->GetEventByFlag(EVENTFLAG_DENY_INPUT))
		return;

	//-----------------------------------------------
	// Quit
	//-----------------------------------------------
	///*
	#ifdef OUTPUT_DEBUG
		if (g_pDXInput->KeyDown(DIK_LMENU) && g_pDXInput->KeyDown(DIK_X))
		{	
			//TerminateThread(g_hFileThread, 0);
			SetMode( MODE_QUIT );
			
			return;
		}	
	#endif
	//*/	


	//-----------------------------------------------
	// packet ÀÌµ¿ Å×½ºÆ®
	//-----------------------------------------------
	#ifdef OUTPUT_DEBUG
		if (g_pDXInput->KeyDown(DIK_M) 
			&& g_pDXInput->KeyDown(DIK_LMENU))
		{			
			PacketTest();
		}
	#endif
	
	/*
	// [ TEST CODE ] DEBUG MESSAGE
	if (g_pDXInput->m_lb_down || g_pDXInput->m_lb_up)
			DEBUG_ADD("----------");

	if (g_pDXInput->m_lb_down && g_pDXInput->m_lb_up)
	{
			DEBUG_ADD("LButtonDown & Up");
	}
	else if (g_pDXInput->m_lb_down)
	{
			DEBUG_ADD("LButtonDown");
	}
	else if (g_pDXInput->m_lb_up)
	{
			DEBUG_ADD("LButtonUp");
	}
	*/

	
	//g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);
	

	//---------------------------------------------------	
	// LButton Up
	//---------------------------------------------------	
	if (g_bLButtonDown
		// LÀÌ UpÀÌ°Å³ª.. RÀÌ DownÀÎ °æ¿ì
		&& (g_pDXInput->m_lb_up	|| g_pDXInput->m_rb_down)
#ifdef __METROTECH_TEST__
		&& !g_bCButtonDown
#endif
		)
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("LBUP");
		#endif

		g_bLButtonDown = FALSE;

		// ÁÖÀ§ÀÇ ´©±º°¡¸¦ °è¼Ó °ø°ÝÇÏ´Â mode¸¦ ÇØÁ¦ÇÑ´Ù.
		g_pPlayer->UnSetLockMode();
	
		if (g_pPlayer->IsRepeatAction())
		{
			// ¹öÆ°À» ¶¼¾úÀ¸¹Ç·Î Çàµ¿ ¹Ýº¹À» Ãë¼ÒÇÑ´Ù.
			g_pPlayer->UnSetRepeatAction();
			//g_pPlayer->TraceNextNULL();

			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
					//DEBUG_ADD("plu");//[ProcessInput] Lbutton UP");
			#endif
		}
		else 
		{		
			//---------------------------------------------------	
			// itemÀ» µé°í ÀÖÁö ¾ÊÀº °æ¿ì
			//---------------------------------------------------	
			// ¼±ÅÃµÈ sector·Î Á¤ÇÑ´Ù.
			POINT point;
			g_pPlayer->GetNextDestination( point );		
			if (point.x==SECTORPOSITION_NULL || point.y==SECTORPOSITION_NULL)
			{
				// ÇöÀç °¡°í ÀÖ´Â °÷ÀÌ ¾øÀ¸¸é
				g_pPlayer->GetDestination( point );
				if (point.x==SECTORPOSITION_NULL || point.y==SECTORPOSITION_NULL)
				{
					//-_-;;
				}
				else
				{
					g_pTopView->SetSelectedSector( point );
				}
			}
			else
			{
				g_pTopView->SetSelectedSector( point );
			}
		}
	}

	//---------------------------------------------------	
	// RButton Up
	//---------------------------------------------------	
	if (g_bRButtonDown
		// RÀÌ UpÀÌ°Å³ª. LÀÌ DownÀÎ °æ¿ì
		&& (g_pDXInput->m_rb_up || g_pDXInput->m_lb_down)
#ifdef __METROTECH_TEST__
		&& !g_bCButtonDown
#endif
		)
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("RBUP");
		#endif

		g_bRButtonDown = FALSE;

		g_pPlayer->UnSetLockMode();

		// 2004, 8, 30, sobeit add start
		g_pPlayer->CheckRbuttonUpSkill();
		// 2004, 8, 30, sobeit add end
		{
			if (g_pPlayer->IsRepeatAction())
			{
				// ¹öÆ°À» ¶¼¾úÀ¸¹Ç·Î Çàµ¿ ¹Ýº¹À» Ãë¼ÒÇÑ´Ù.
				g_pPlayer->UnSetRepeatAction();
				//g_pPlayer->TraceNextNULL();

				#ifdef OUTPUT_DEBUG_UPDATE_LOOP
						//DEBUG_ADD("pru");//[ProcessInput] Rbutton UP");
				#endif
			}
		}
	}

	if(g_bCButtonDown && g_pDXInput->m_cb_up)
	{
		g_bCButtonDown = FALSE;
	}
	
	//---------------------------------------------------	
	// UI Input
	//---------------------------------------------------	
	POINT cursorPoint;
	GetCursorPos(&cursorPoint);

	ScreenToClient(g_hWnd, &cursorPoint);//by viva

	#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("UIMM");
	#endif
	gC_vs_ui.MouseControl(M_MOVING, cursorPoint.x, cursorPoint.y);
	//gC_vs_ui.MouseControl(M_MOVING, g_x, g_y);
	//if (gC_vs_ui.MouseControl(M_MOVING, g_x, g_y))
	//	return;

	// Àü½ÃÈ¸¿ë Client´Â interface¸¦ ¾È ±×·ÁÁÙ¶§°¡ ÀÖ´Ù.
	//#ifdef __EXPO_CLIENT__	
	//	if ((*g_pUserOption).DrawInterface)
	//	{
	//#endif

	#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("IRP");
	#endif

	
	bool bRunningParty = UI_IsRunningParty();

	if (g_pDXInput->m_lb_down)
	{
		#ifdef OUTPUT_DEBUG
			if (g_pDXInput->KeyDown(DIK_LSHIFT) && g_bSlideScreenShot)
			{
				g_bSlideRectSelect = true;
			}
		#endif
		
		if (gC_vs_ui.MouseControl(M_LEFTBUTTON_DOWN, g_x, g_y) )
		{
			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
					DEBUG_ADD("[ProcessInput] UI LButton Down return");
			#endif

			return;				
		}
	}

	if (g_pDXInput->m_lb_up)
	{
		#ifdef OUTPUT_DEBUG
			g_bSlideRectSelect = false;			
		#endif

		if (gC_vs_ui.MouseControl(M_LEFTBUTTON_UP, g_x, g_y))
		{
			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
					DEBUG_ADD("[ProcessInput] UI LButton Up return");
			#endif

			return;
		}
	}
	if (g_pDXInput->m_rb_down)
	{
		if (gC_vs_ui.MouseControl(M_RIGHTBUTTON_DOWN, g_x, g_y)
			&& !bRunningParty)
		{
			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
					DEBUG_ADD("[ProcessInput] UI RButton Down return");
			#endif

			return;
		}
	}

	if (g_pDXInput->m_rb_up)
	{
		if (gC_vs_ui.MouseControl(M_RIGHTBUTTON_UP, g_x, g_y)
			&& !bRunningParty)
		{
			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
					DEBUG_ADD("[ProcessInput] UI RButton Up return");
			#endif

			return;
		}
	}	

	if( g_pDXInput->m_cb_up)
	{
		if( gC_vs_ui.MouseControl(M_CENTERBUTTON_UP, g_x,g_y)
			&& !bRunningParty)
		{
			return;
		}
	}

	if( g_pDXInput->m_cb_down)
	{
		if( gC_vs_ui.MouseControl( M_CENTERBUTTON_DOWN, g_x,g_y)
			&& !bRunningParty)
		{
			return;
		}
	}
	
	//#ifdef OUTPUT_DEBUG
	// Ã¤ÆÃ
//		if (gC_vs_ui.ChatMouseControlExtra( M_MOVING, g_x, g_y ))
//			return;
	//#endif

	// 2000.7.8. KJTINC
	/*
	static bool bl_ctrl;
	if (g_pDXInput->KeyDown(DIK_LCONTROL) ||
		g_pDXInput->KeyDown(DIK_RCONTROL))
	{
		bl_ctrl = true;
	}
	//if (g_pDXInput->KeyUp(DIK_LCONTROL) &&
	//	 g_pDXInput->KeyUp(DIK_RCONTROL))
	//{
	//	bl_ctrl = false;
	//}
	if (g_pDXInput->KeyDown(DIK_SPACE) && bl_ctrl == true)
	{
		gC_vs_ui.PressCtrlSpace();
	}*/


	#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("CINL");
	#endif

	//---------------------------------------------------	
	// ItemName List Ãâ·Â?
	//---------------------------------------------------	
	if ((g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU) || g_pDXInput->KeyDown(DIK_RCONTROL)
#ifdef __FRIEND_SYSTEM_VIVA__	// add by viva  g_pDXInput->KeyDown(DIK_LCONTROL
		|| g_pDXInput->KeyDown(DIK_LCONTROL)
#endif
		)  
		&& !g_pDXInput->KeyDown(DIK_LSHIFT)
		&& g_pTradeManager==NULL && g_pPlayer->GetWaitVerify()!=MPlayer::WAIT_VERIFY_TRADE)
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("DINL");
		#endif

		g_pTopView->SetDrawItemNameList();

		if (!g_bWatchMode 
			// ¹º°¡ ¹Ýº¹Çàµ¿À» ÇÏ°í ÀÖÁö ¾ÊÀº °æ¿ì
			&& !g_pPlayer->IsRepeatAction()
			// ¹ÚÁã³ª ´Á´ë°¡ ¾Æ´Ñ °æ¿ì
			&& g_pPlayer->GetCreatureType()!=CREATURETYPE_BAT
			&& g_pPlayer->GetCreatureType()!=CREATURETYPE_WOLF
			)
		{
			g_pTopView->UnSetRequestMode();
			g_pTopView->SetSelectedSectorNULL();					

			if (g_pTempInformation->GetMode() == TempInformation::MODE_NULL)
			{					
				if (g_pDXInput->KeyDown(DIK_LMENU))
				{
					g_pTopView->SetRequestMode( MTopView::REQUEST_TRADE );
				}
				else if (g_pDXInput->KeyDown(DIK_RMENU))
				{
					if (!gC_vs_ui.IsRunningRequestParty()
						&& g_pSystemAvailableManager->IsAvailablePartySystem() )
					{
						g_pTopView->SetRequestMode( MTopView::REQUEST_PARTY );
					}
				}
				else if (g_pDXInput->KeyDown(DIK_RCONTROL))
				{
					g_pTopView->SetRequestMode( MTopView::REQUEST_INFO );
				}
#ifdef __FRIEND_SYSTEM_VIVA__
				///add by viva
				else if(g_pDXInput->KeyDown(DIK_LCONTROL))
				{
					gpC_mouse_pointer->SetCursorAddFriend();
				//	g_pPlayer->SetRepeatAction();
				}
#endif
			}
		}
	}
	else
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("NDINL");
		#endif

		g_pTopView->UnSetDrawItemNameList();
		g_pTopView->UnSetRequestMode();		
	}

	#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("PMC");
	#endif

		
	//---------------------------------------------------	
	// ÆÄÆ¼ ¾ø´Âµ¥ ÆÄÆ¼Ã¢ÀÌ ¶° ÀÖ´Â °æ¿ì..
	// ÆÄÆ¼ ½ÅÃ» ¸ðµå.
	//---------------------------------------------------	
//	if (g_pParty!=NULL 
//		&& g_pParty->GetSize()==0
//		&& UI_IsRunningParty()
//		&& !gC_vs_ui.IsRunningRequestParty()
//		&& g_pTempInformation->Mode==TempInformation::MODE_NULL
//		&& !g_bWatchMode 
//		// ¹º°¡ ¹Ýº¹Çàµ¿À» ÇÏ°í ÀÖÁö ¾ÊÀº °æ¿ì
//		&& !g_pPlayer->IsRepeatAction()
//		// ¹ÚÁã³ª ´Á´ë°¡ ¾Æ´Ñ °æ¿ì
//		&& g_pPlayer->GetCreatureType()!=CREATURETYPE_BAT
//		&& g_pPlayer->GetCreatureType()!=CREATURETYPE_WOLF)
//	{
//		g_pTopView->SetSelectedSectorNULL();
//		g_pTopView->SetRequestMode( MTopView::REQUEST_PARTY );
//	}

	//---------------------------------------------------	
	//
	//		ÀÔ·ÂÀÌ Á¦ÇÑµÇ´Â °æ¿ì..
	//
	//---------------------------------------------------	
	// - °ÔÀÓ ¸øÇÏ°Ô ÇÏ´Â dialog°¡ ¶° ÀÖ´Â °æ¿ì
	//
	// - Player°¡ server·ÎºÎÅÍ °ËÁõÀ» ±â´Ù¸®´Â »óÅÂ
	//
	// - ¹Ýº¹ actionÁß¿¡´Â µý ÀÔ·ÂÀº ÇÊ¿ä¾ø´Ù.(¹«½Ã)
	//
	// - UI¿¡¼­ ÀÔ·ÂÀ» ¹Þ°í ÀÖ´Â °æ¿ì
	//
	//---------------------------------------------------	
	if (g_pPlayer->IsWaitVerify() 
		|| gC_vs_ui.IsInstallMineProgress()	// Áö·Ú ¸¸µå´Â ÁßÀÌ¸é..
		//|| gC_vs_ui.IsCreateMineProgress()	// Áö·Ú ¸¸µå´Â ÁßÀÌ¸é..
		//|| gC_vs_ui.IsCreateBombProgress()	// Áö·Ú ¸¸µå´Â ÁßÀÌ¸é..
		|| g_pPlayer->IsFastMove()	// »¡¸® ¿òÁ÷ÀÌ´Â °æ¿ì
		// 2004, 12, 3, ¼®¹Î¾¾ Ãß°¡
//		|| g_pPlayer->CurPernalShop() == 1  || gC_vs_ui.inventory_mode == 2
//		|| gC_vs_ui.IsRunningPersnalShop()
		// 2004, 12, 3, ¼®¹Î¾¾ Ãß°¡
		|| UI_IsRunning_WebBrowser()
		)	
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD_FORMAT("[ProcessInput] Is WaitVerify Return. %d %d", g_pPlayer->IsWaitVerify(), g_pPlayer->IsFastMove());
		#endif

		return;
	}

	// ÇöÀç ¼¿·ºÆ®ÁßÀÎ Ä³¸¯ÅÍ¿¡ ´ëÇÑ µ¿ÀÛÀÌ ÀÖÀ¸¸é ¸®ÅÏ
	if (g_pPlayer->IsRepeatAction())
	{
		g_pTopView->SetSelectedSectorNULL();

		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("[ProcessInput] Is RepeatAction return");
		#endif

		return;
	}

	#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("CUILock");
	#endif

	//---------------------------------------------------	
	// UI¿¡¼­ mouseÄ¿¼­ ÀÔ·ÂÀ» Àâ°í ÀÖ´Â °æ¿ì
	// elevator ÀÛµ¿Áß..
	//---------------------------------------------------	
	if ((g_pUIDialog->IsLockInput() || g_bUIInput)
		
		// ÆÄÆ¼ ¶°ÀÖÀ»¶§´Â ÆÄÆ¼Ã¢ÀÇ Ä³¸¯À» ¼±ÅÃÇÒ ¼öµµ ÀÖ´Ù.
		&& !bRunningParty

		// ´Ù¸¥ µ¿ÀÛ ¸øÇÏµµ·Ï ...
		|| gC_vs_ui.IsRunningElevator()
		|| gC_vs_ui.IsRunningXmasCardWindow()
		|| UI_IsRunningSelectWayPoint())
	{
		g_pTopView->SetSelectedNULL();
			
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD_FORMAT("[ProcessInput] UI return. %d %d %d", g_pUIDialog->IsLockInput(), g_bUIInput, gC_vs_ui.IsRunningElevator());
		#endif

		return;
	}

	
	
	//---------------------------------------------------	
	//
	// Player°¡ Á×Àº °æ¿ì... 
	//
	//---------------------------------------------------		
	if (g_pPlayer->IsDead())
	{
		g_pTopView->SetSelectedNULL();
		g_pTopView->SetSelectedSectorNULL();

		//---------------------------------------------------	
		// delay½Ã°£ÀÌ Áö³ª°í ³ª¼­
		// ¹º°¡¸¦ ´©¸£¸é ´Ù½Ã »ì¾Æ³ª¾ß ÇÑ´Ù.
		//---------------------------------------------------	
		//if (g_pPlayer->IsNotDeadDelay() && g_pDXInput->KeyDown( DIK_SPACE ))
		//{
		//	g_pPlayer->SetAlive();
		//	g_pTopView->SetFadeEnd();
		//}
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("[ProcessInput] Player Dead return.");
		#endif

		return;
	}
 


	// Mouse¸¦ ÇâÇØ¼­ ¹Ù¶óº¸´Â player...
	//POINT temp = g_pTopView->ScreenToPixel(g_x, g_y);
	//temp = MTopView::PixelToMap( temp.x, temp.y );
	//g_pPlayer->SetDirectionToPosition(temp.x, temp.y);

	#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("FA");
	#endif

	//---------------------------------------------------	
	//
	// ÇöÀç mouseÀÇ À§Ä¡¿¡ ÀÖ´Â object¿¡ ´ëÇØ¼­ check
	//
	//---------------------------------------------------	
	// °­Á¦°ø°ÝÀÌ°Å³ª SmallZone¿¡ ÀÖ°Å³ª
	// Hallucination¿¡ °É·ÈÀ¸¸é ´Ù~ ¼±ÅÃ
	//---------------------------------------------------	
	

	// °­Á¦ °ø°ÝÀÌ ¼³Á¤µÇ´Â °æ¿ì
	bool bForceAttack = (g_pDXInput->KeyDown(DIK_LSHIFT) 
						|| g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION));
						
						

	if (bForceAttack
		//|| !g_bZonePlayerInLarge
		|| g_bZoneSafe
		|| g_pPlayer->IsInSafeSector()
		|| g_pPlayer->IsSlayer() && g_pPlayer->GetSpecialActionInfo() != ACTIONINFO_NULL && (*g_pActionInfoTable)[g_pPlayer->GetSpecialActionInfo()].GetUser() == FLAG_ACTIONINFO_USER_SLAYER
		|| g_pPlayer->IsVampire() && g_pPlayer->GetSpecialActionInfo() != ACTIONINFO_NULL && (*g_pActionInfoTable)[g_pPlayer->GetSpecialActionInfo()].GetUser() == FLAG_ACTIONINFO_USER_VAMPIRE
		|| g_pPlayer->IsOusters() && g_pPlayer->GetSpecialActionInfo() != ACTIONINFO_NULL && (*g_pActionInfoTable)[g_pPlayer->GetSpecialActionInfo()].GetUser() == FLAG_ACTIONINFO_USER_OUSTERS
		// 2004, 11, 26, sobeit add start - ½½·¹ 140 ÀÎÃ¦ ½ºÅ³ - ½½·¹¿¡°Õ Ãàº¹, ³ª¸ÓÁø ÀúÁÖ..¤»¤» 
		|| g_pPlayer->GetSpecialActionInfo() == SKILL_INTIMATE_GRAIL
		// 2004, 11, 26, sobeit add end
		)
	{
		g_pObjectSelector->SelectAll();				
	}
	//---------------------------------------------------	
	// L-Control´©¸£¸é ¿ì¸®Æí¸¸ ¼±ÅÃ
	//---------------------------------------------------	
	else if (g_pDXInput->KeyDown(DIK_LCONTROL)
		|| g_pTopView->IsRequestMode())
	{
		g_pObjectSelector->SelectFriend();		
	}
	//---------------------------------------------------	
	// ¾Æ´Ï¸é.. Àû¸¸ ¼±ÅÃ
	//---------------------------------------------------	
	else
	{
		// Notice Event Ã¼Å©ÇØ¼­ ...
		// ¼ºÀ» Â÷ÁöÇÏ±â À§ÇÑ ÀüÀïÁßÀÌ¶ó¸é!!
		// 1. ±âÁ¸¿¡ ¼öºñÇÏ´Â ±æµåÀÏ °æ¿ì ÀÚ±â ±æµå ¾Æ´Ï¸é ´Ù Àû!
		// 2. °ø°Ý ½ÅÃ»À» ÇÑ ±æµåÀÏ°æ¿ì ¿ì¸® ±æµå ¾Æ´Ï¸é ´Ù Àû!
		// 3. ±æµå°£ ÀüÀï ½ÅÃ» ¿Ü¿¡ ´Ù¸¥»ç¶÷ÀÏ°æ¿ì ±× µÎ ±æµå¿ø »©°í ´Ù °°Àº ÆÀ!
		
		g_pObjectSelector->SelectEnemy();		
	}

	//---------------------------------------------------	
	// ÀÌ°Í ¿Ü¿¡µµ..
	//
	// g_pObjectSelector->SelectByRace()ÀÎÁö
	// g_pObjectSelector->SelectByGuild()ÀÎÁö¸¦ ¼±ÅÃÇØ¾ß ÇÑ´Ù.
	//
	// È­¸é¿¡ ´Ã IconÀ» ¶ç¿ö³õ´Â°Ô ÁÁÀ» µí.. Race/Guild
	//
	// ¶Ç, °ø°Ý¸ðµå¿¡ µû¶ó¼­ Peace/Attack/Normal Iconµµ
	// ÀÖ¾î¾ß ÇÑ´Ù.
	//---------------------------------------------------	
	// SlayerÀÎ °æ¿ì´Â Á¾Á·¿¡ µû¶ó¼­ ¼±ÅÃ
	//---------------------------------------------------	
	if (g_pPlayer->IsSlayer() || g_pPlayer->IsOusters() )
	{
		g_pObjectSelector->SelectByRace();
	}
	//---------------------------------------------------	
	// VampireÀÎ °æ¿ì´Â Guild¿¡ µû¶ó¼­ ¼±ÅÃ
	//---------------------------------------------------	
	else
	{
		g_pObjectSelector->SelectByGuild();
	}


	//MObject*	pObject = g_pTopView->GetSelectedObjectSprite(g_x, g_y);
	MObject*	pObject = NULL;
	
	
	#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("PMF");
	#endif

	int partyMember = (bRunningParty? gC_vs_ui.GetPartyManagerFocused() : -1) - 1;

	//---------------------------------------------------	
	// ÆÄÆ¼UI¸¦ ¼±ÅÃÇÏ´Â °æ¿ì..
	//---------------------------------------------------	
	if (partyMember >= 0
		&& partyMember < g_pParty->GetSize())
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("SelP");
		#endif

		PARTY_INFO* pInfo = g_pParty->GetMemberInfo( partyMember );

		if (pInfo!=NULL)
		{
			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("SPM");
			#endif

			// ¿ì¿í.. terrible.. - -;
			// MZone¿¡ NameÀ¸·Î id°Ë»öÇÒ ¼ö ÀÖ°Ô mapÀ» Ãß°¡ÇØ¾ßÇÑ´Ù.
			// ±×¸®°í.. ¹Ù·Î Name --> MCreature* ¸¦ ¾Ë ¼ö ÀÖ°Ô ÇØ¾ßÇÑ´Ù.. ¾ðÁ¦? - -;
			pObject = g_pZone->GetCreature( g_pZone->GetCreatureID( pInfo->Name.GetString(), 1 ) );

			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("SPMk");
			#endif
		}
	}
	//---------------------------------------------------	
	// Á¸¿¡¼­ ¼±ÅÃ..
	//---------------------------------------------------	
	else
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("selZ");
		#endif

		//---------------------------------------------------
		// ¾Æ¿¹ µý UI¿¡ focus °¡ ÀÖÀ¸¸é °Á returnÇØ¾ß ÇÑ´Ù.
		// inventory¿¡¼­ ¾ÆÀÌÅÛ »ç¿ëÇß´Âµ¥ ±â¼ú ³ª°¥ ¼ö°¡ ÀÖ¾î¼­.
		//---------------------------------------------------
		if (partyMember==-2		// partyÃ¢¿¡ focus µÈ°Ô ¾Æ´Ï°í
			&& g_bUIInput)		// UI¿¡ ÀÔ·ÂÀÌ ÀÖ´Ù¸é..
		{
			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("selZk1");
			#endif

			return;
		}

		//---------------------------------------------------	
		// ItemNameÀ» ¼±ÅÃÇÏ´Â °æ¿ì¸é..
		//---------------------------------------------------	
		if (g_pTopView->IsDrawItemNameList())
		{
			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("drawItemNL");
			#endif

			MItem* pItem = g_pTopView->SelectItemName(g_x, g_y);

			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("selIN");
			#endif

			if (pItem==NULL)
			{
				pObject = NULL;
			}
			else
			{
				pObject = (MObject*)pItem;
			}

			//---------------------------------------------------	
			// ItemNameÀÌ ¼±ÅÃ ¾ÈµÆÀ¸¸é Object¼±ÅÃ
			//---------------------------------------------------	
			if (pObject==NULL)
			{
				#ifdef OUTPUT_DEBUG_PROCESS_INPUT
					DEBUG_ADD("selObj");
				#endif

				pObject = g_pTopView->GetSelectedObject(g_x, g_y);

				#ifdef OUTPUT_DEBUG_PROCESS_INPUT
					DEBUG_ADD("selObjK");
				#endif
			}
		}
		//---------------------------------------------------	
		// ±×³É Object ¼±ÅÃ
		//---------------------------------------------------	
		else
		{
			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("selObj2");
			#endif

			pObject = g_pTopView->GetSelectedObject(g_x, g_y);

			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("selObj2K");
			#endif
		}

		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("selZk");
		#endif
	}

//	if (g_pPlayer->IsRepeatAction())
//	{
//		g_pTopView->SetSelectedSectorNULL();
//		
//#ifdef OUTPUT_DEBUG_PROCESS_INPUT
//		DEBUG_ADD("[ProcessInput] Is RepeatAction return");
//#endif
//		
//		return;
//	}
	
	//---------------------------------------------------		
	// Watch mode
	//---------------------------------------------------		
	if (g_bWatchMode)
	{
		int cx = 0, cy = 0;

		if (g_x < 10)		cx = -1;
		else if (g_x > 790)	cx = 1;
		if (g_y < 10)		cy = -1;
		else if (g_y > 590)	cy = 1;

		int x = g_pPlayer->GetX();
		int y = g_pPlayer->GetY();

		x += cx;
		y += cy;

		if (x < 0) x = 0;
		else if (x >= g_pZone->GetWidth()) x = g_pZone->GetWidth()-1;
		if (y < 0) y = 0;
		else if (y >= g_pZone->GetHeight()) y = g_pZone->GetHeight()-1;

		g_pPlayer->SetPosition( x, y );

		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("[ProcessInput] Watch Mode");
		#endif

		return;
	}

	#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("ckL");
	#endif

	//---------------------------------------------------	
	// Lock Mode Ã¼Å©
	//---------------------------------------------------	
	if (g_pDXInput->KeyDown(DIK_CAPITAL) 
		&& !g_pPlayer->IsInCasket()
		&& !g_pPlayer->IsUndergroundCreature())
	{
		g_pPlayer->SetLockMode();

		// ¹«Á¶°Ç °ø°Ý Ä¿¼­·Î ¹Ù²ï´Ù.
		gpC_mouse_pointer->SetCursorAttack();

		if (g_pDXInput->m_lb_down)
		{		
			g_bLButtonDown = TRUE;			
		}
		else if (g_pDXInput->m_rb_down)
		{
			g_bRButtonDown = TRUE;
		}
		else if (g_pDXInput->m_cb_down)
		{
			g_bCButtonDown = TRUE;
		}

		// Lock Mode¿¡¼­´Â ¼±ÅÃµÈ sector¸¦ ¾ø¾Ø´Ù.
		g_pTopView->SetSelectedSectorNULL();
	}
	else
	{
		// ¾Æ¹«°Íµµ ´©¸£Áö ¾ÊÀº »óÅÂ¿¡¼­ 
		// LockModeÀÌ¸é LockMode ÇØÁ¦ÇØ¾ß ÇÑ´Ù.
		if (!g_bLButtonDown && !g_bRButtonDown
#ifdef __METROTECH_TEST__
			&& !g_bCButtonDown
#endif
			)
		{
			g_pPlayer->UnSetLockMode();
		}
	}

	//---------------------------------------------------	
	// Lock Mode°¡ ¾Æ´Ñ °æ¿ì¸¸ 
	// ÀÏ¹ÝÀûÀÎ ÀÔ·ÂÀ» ¹Þ¾Æµé¿© Çàµ¿À» ÃëÇÑ´Ù.
	//---------------------------------------------------	
	if (!g_pPlayer->IsLockMode())
	{	
#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("notLock");
#endif
		
		//---------------------------------------------------	
		// Mouse À§Ä¡¿¡ Object°¡ ¾øÀ¸¸é..
		//---------------------------------------------------	
		if (pObject==NULL)
		{
#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("noObj");
#endif
			
			// ¼±ÅÃµÈ °Í ¾ø°Ô ÇÑ´Ù.
			g_pTopView->SetSelectedNULL();
			
			//---------------------------------------------------	
			// mouse pointer ¼³Á¤
			//---------------------------------------------------	
			//if (!g_bMouseInPortal)
			{
				if (g_pPlayer->OnAttacking() || g_pDXInput->KeyDown(DIK_LSHIFT))
				{
					gpC_mouse_pointer->SetCursorAttack();
				}
				else
				{
					if (g_MouseSector.x >= 0 
						&& g_MouseSector.y >= 0
						&& g_MouseSector.x < g_pZone->GetWidth() 
						&& g_MouseSector.y < g_pZone->GetHeight())
					{
#ifdef OUTPUT_DEBUG_PROCESS_INPUT
						DEBUG_ADD_FORMAT("getSec(%d, %d)", g_MouseSector.x, g_MouseSector.y);
#endif
						
						const MSector& sector = g_pZone->GetSector(g_MouseSector.x, g_MouseSector.y);
						
						//---------------------------------------------------
						// °¥ ¼ö ¾ø´Â °÷ÀÌ¸é
						//---------------------------------------------------
						if (g_pPlayer->IsGroundCreature() && sector.IsBlockGround()
							|| g_pPlayer->IsUndergroundCreature() && sector.IsBlockUnderground()
							|| g_pPlayer->IsFlyingCreature() && sector.IsBlockFlying())
							//g_pZone->CanMove(g_pPlayer->GetMoveType(), g_MouseSector.x, g_MouseSector.y))
						{	
							// Æ÷Å»ÀÌ ¾Æ´Ï°í
							if (!g_bMouseInPortal
								// UI¿¡ ÀÔ·ÂÀÌ ¾ø´Â °æ¿ì --> °ÔÀÓ¿¡ Ä¿¼­°¡ ÀÖ´Â °æ¿ì
								&& !g_bUIInput
								// ¼±ÅÃµÈ Ä³¸¯ÅÍ°¡ ¾ø´Â °æ¿ì
								&& g_pTopView->GetSelectedCreature()==OBJECTID_NULL)
							{
								// °¥ ¼ö ¾ø´Â Ä¿¼­·Î Ç¥½Ã
								gpC_mouse_pointer->SetCursorNotMove();					
							}
						}
						//---------------------------------------------------
						// °¥ ¼ö ÀÖ´Â °÷ÀÌ¸é..
						//---------------------------------------------------
						else
						{
							if (!g_bMouseInPortal)
							{
								gpC_mouse_pointer->SetCursorDefault();
							}
						}
					}
					else
					{
						if (!g_bMouseInPortal)
						{
							gpC_mouse_pointer->SetCursorDefault();
						}
					}
				}
			}
			
#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("noObjk");
#endif
		}
		//---------------------------------------------------	
		//
		// Mouse À§Ä¡¿¡ Object°¡ ÀÖ´Â °æ¿ì
		//
		//---------------------------------------------------	
		else
		{
			BYTE objectType = pObject->GetObjectType();
			
#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD_FORMAT("obj(%d)", objectType);
#endif
			
			//------------------------------------------------
			// Creature À§¿¡ mouse°¡ ÀÖ´Â °æ¿ì
			//------------------------------------------------
			if (objectType==MObject::TYPE_CREATURE)
			{
				MCreature* pCreature = (MCreature*)pObject;

				g_pTopView->SetSelectedCreature( pCreature->GetID() );
					
				if (!g_bMouseInPortal 
						
					// Á¾Á·¿¡ µû¶ó¼­ °ø°ÝÇÒ ¼ö ÀÖ°Å³ª
					// °­Á¦ °ø°ÝÀÌ°Å³ª..
					//&& (g_pPlayer->CanAttackTribe( pCreature ) || bForceAttack)
					&& g_pObjectSelector->CanAttack( pCreature ) )
					// ¹ìÆÄÀÌ¾îÀÎ °æ¿ì¿¡´Â Guild¿¡ µû¶ó¼­ °ø°ÝÇÒ ¼ö ÀÖ°Å³ª..	
						
				{
					if (pCreature->IsNPC())
					{
						gpC_mouse_pointer->SetCursorDefault();
					}
					else
					{
						gpC_mouse_pointer->SetCursorAttack();
					}
				}
#ifdef __FRIEND_SYSTEM_VIVA__//add by viva
				if(g_pDXInput->KeyDown(DIK_LCONTROL) && g_pDXInput->m_lb_down)	
				{
					gpC_base->SendMessage(UI_FRIEND_CHATTING_ADD_FRIEND, 0, 0, (void*)pCreature->GetName());
				}
#endif
					
			}
			
			//------------------------------------------------
			// Item À§¿¡ mouse°¡ ÀÖ´Â °æ¿ì
			//------------------------------------------------
			else if (pObject->GetObjectType()==MObject::TYPE_ITEM)
			{
				MItem* pItem = (MItem*)pObject;
				MCorpse* pCorpse = (MCorpse*) pItem;
				MCreature* pCreature = pCorpse->GetCreature();
				
				if(g_pZone != NULL && pItem != NULL )
				{
					// ½½·¹ÀÌ¾î¸é ¾ÈµÇ°Ô
					// ¹ìÆÄÀÌ¾î¸é creature Ã£¾Æ¼­ ¼º¹°ÀÎ°æ¿ì¸¸

					if(!g_pZone->GetSector(pItem->GetX(), pItem->GetY()).HasDarkness() || 
						g_pZone->GetSector(pItem->GetX(), pItem->GetY()).HasDarkness() && g_pPlayer->IsVampire() && g_pZone->GetID() != 3001||
						g_pZone->GetSector(pItem->GetX(), pItem->GetY()).HasDarkness() && !g_pPlayer->IsVampire() && 
						g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) && g_pZone->GetID() != 3001						
						|| g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
						|| g_bLight
#endif
						)// &&
						//pCreature != NULL && pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549)
					{
						g_pTopView->SetSelectedItem( pItem->GetID() );			
						
						COLORREF color;
						//------------------------------------------------
						// option¿¡ µû¸¥ »ö±ò
						//------------------------------------------------
						if (pItem->IsSpecialColorItem() )
						{
							WORD temp_color = CIndexSprite::ColorSet[pItem->GetSpecialColorItemColorset()][16];
							color = RGB(CDirectDraw::Red(temp_color)<<3, CDirectDraw::Green(temp_color)<<3, CDirectDraw::Blue(temp_color)<<3);
						}
						else
						if (pItem->IsEmptyItemOptionList())
						{
							color = g_pClientConfig->COLOR_NAME_ITEM;	
						}
						else if(pItem->GetItemOptionListCount() == 2)
						{
							color = g_pClientConfig->COLOR_NAME_ITEM_RARE_OPTION;
						}
						// add by Sonic 2006.10.28 Ôö¼ÓÏÔÊ¾ÈýÊôÐÔ×°±¸ÎªºìÉ«
						else if(pItem->GetItemOptionListCount() > 2)
						{
							color = g_pClientConfig->COLOR_NAME_VAMPIRE; //Red
						}
						// end by Sonic 2006.10.28 Ôö¼ÓÏÔÊ¾ÈýÊôÐÔ×°±¸ÎªºìÉ«
						else
						{
							color = g_pClientConfig->COLOR_NAME_ITEM_OPTION;
						}
						
						//------------------------------------------------
						// itemÁý´Â Ä¿¼­
						//------------------------------------------------
						if (!g_bMouseInPortal&&!g_pPlayer->IsInDarkness())
						{						
							// ½ÃÃ¼ÀÎ °æ¿ì´Â '½ÃÃ¼'¶ó°í Ç¥½Ã ¾ÈÇÑ´Ù.
							if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
							{
								gpC_mouse_pointer->SetCursorPickUp( "", color );
							}
							// º¸Åë ¾ÆÀÌÅÛÀº ÀÌ¸§ Ç¥½ÃÇÑ´Ù.
							else
							{
								char str[80];
								strcpy(str, pItem->GetName());
								
								gpC_mouse_pointer->SetCursorPickUp( str, color );
							}
						}
					} 
				}
			}

			//------------------------------------------------
			// Effect À§¿¡ mouse°¡ ÀÖ´Â °æ¿ì
			//------------------------------------------------
			else if (pObject->GetObjectType()==MObject::TYPE_EFFECT)
			{
				g_pTopView->SetSelectedEffect( pObject->GetID() );
			}

			//------------------------------------------------
			// InteractionObject À§¿¡ mouse°¡ ÀÖ´Â °æ¿ì
			//------------------------------------------------
//			else if (pObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//			{
//				g_pTopView->SetSelectedInteractionObject( pObject->GetID() );
//			}
		}


		//---------------------------------------------------	
		//
		// playerÀÇ ±â¼ú »ç¿ë delay ½Ã°£ÀÌ Áö³ª¾ß °ø°Ý °¡´ÉÇÏ´Ù.
		//
		//---------------------------------------------------	
		if (g_pPlayer->IsNotDelay() && !g_pPlayer->HasEffectStatus( EFFECTSTATUS_ETERNITY_PAUSE ) )
		{
			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("noDel");
			#endif

			//-----------------------------------------------
			// Shift + L/R ButtonDown : °­Á¦ Tile °ø°Ý
			//-----------------------------------------------
			if (g_pDXInput->KeyDown(DIK_LSHIFT))
			{
				#ifdef OUTPUT_DEBUG_PROCESS_INPUT
					DEBUG_ADD("shiAt");
				#endif
				//-----------------------------------------------
				// Shift + LButtonDown : °­Á¦ °ø°Ý
				//-----------------------------------------------
				if (g_pDXInput->m_lb_down
					// ¼û¾î ÀÖ´Â °æ¿ì°¡ ¾Æ´Ò¶§
					&& !g_pPlayer->IsInCasket()
					&& !g_pPlayer->IsUndergroundCreature()
					&& !g_pPlayer->HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
				{
					g_pPlayer->UnSetRequestMode();

					//-----------------------------------------------
					// Ä³¸¯ÅÍ¸¦ °­Á¦ °ø°Ý..
					//-----------------------------------------------
					if (pObject!=NULL)
					{
						if (pObject->GetObjectType()==MObject::TYPE_CREATURE)
						{
							// Object°¡ ¹º°¡ ¼±ÅÃµÆÀ» °æ¿ì¿¡´Â 
							// LButtonÀ» ´©¸£°í ÀÖ´Â °Í¸¸À¸·Î ÀÌµ¿ÀÌ µÇÁö¾Ê°Ô ÇÑ´Ù.
							MCreature *pCreature = dynamic_cast<MCreature*>(pObject);
							g_bLButtonDown = TRUE;
							
							BOOL	ForceAttack = TRUE;

							if (g_pPlayer->TraceCreatureToBasicAction( 
										pCreature->GetID(), 
										true))		// °­Á¦ °ø°Ý
							{
								//----------------------------------
								// ³²¿¡°Ô ÇÏ´Â ±âº» Çàµ¿ ¹Ýº¹ ¼³Á¤
								//----------------------------------
								g_pPlayer->SetRepeatAction();
								g_bPreviousMove = false;
							}
							// ¸ñÀûÁö Ç¥½Ã¸¦ ¾ø¾Ø´Ù.
							g_pTopView->SetSelectedSectorNULL();
						}
					}
					//-----------------------------------------------
					// °­Á¦ Tile °ø°Ý
					//-----------------------------------------------
					//g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);
					
					// ¼±ÅÃµÈ Sector·Î Ç¥½ÃÇÑ´Ù.
					//g_pTopView->SetSelectedSector( g_SelectSector );

					//g_pPlayer->TraceSectorToBasicAction( g_SelectSector.x, g_SelectSector.y );			

					#ifdef OUTPUT_DEBUG_PROCESS_INPUT
						DEBUG_ADD("shiKK");
					#endif
				}		
				//-----------------------------------------------
				// Shift + RButtonDown : °­Á¦ Tile ±â¼ú °ø°Ý
				//-----------------------------------------------
				else if (g_pDXInput->m_rb_down)
				{
					g_pPlayer->UnSetRequestMode();

					ProcessInputRButtonDown(pObject, true);

					g_bPreviousMove = false;
					/*
					g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);

					// ¼±ÅÃµÈ Sector·Î Ç¥½ÃÇÑ´Ù.
					g_pTopView->SetSelectedSector( g_SelectSector );

					if (g_pPlayer->TraceSectorToSpecialAction( g_SelectSector.x, g_SelectSector.y ))
					{
						//g_pPlayer->SetRepeatAction();
					}


					// ¸ñÀûÁö Ç¥½Ã¸¦ ¾ø¾Ø´Ù.
					g_pTopView->SetSelectedSectorNULL();
					*/

					#ifdef OUTPUT_DEBUG_PROCESS_INPUT
						DEBUG_ADD("shiRK");
					#endif
				}
				
			}

			//---------------------------------------------------------------
			// L/R ButtonDown only
			//---------------------------------------------------------------
			else
			{
				
				#ifdef OUTPUT_DEBUG_PROCESS_INPUT
					DEBUG_ADD("LRonly");
				#endif
				//---------------------------------------------------------------
				//
				//				LButtonDown
				//
				//---------------------------------------------------------------
				if (g_pDXInput->m_lb_down 
					// burrow »óÅÂ°¡ ¾Æ´Ï¾î¾ß ÇÑ´Ù.
					&& !g_pPlayer->IsInCasket()
					&& !g_pPlayer->IsUndergroundCreature()
					&& g_pPlayer->CurPernalShop() != 2
					&& g_pPlayer->CurPernalShop() != 1
					&& gC_vs_ui.IsRunningPersnalShop() == false
					)
				{		
					#ifdef OUTPUT_DEBUG_PROCESS_INPUT
						DEBUG_ADD("Lon");
					#endif

					g_pPlayer->UnSetRequestMode();

					g_bLButtonDown = TRUE;
					
					// ¼±ÅÃµÈ sector°¡ ¾ø°Ô ÇÑ´Ù.
					g_pTopView->SetSelectedSectorNULL();

					// sector ¼±ÅÃÇÏ±â
					g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);
					//g_pTopView->SetSelectedSector(g_SelectSector.x, g_SelectSector.y);

					//
					//MObject*	pObject = g_pTopView->GetSelectedObject(g_x, g_y);

					// ÀÎ½ºÅç ÅÍ·¿ÀÏ¶§.. Àâ´ÙÇÑ lbuttton Ã³¸® ¾Ê´Â´Ù.. lbutton = ¹«Á¶°Ç ÅÍ·¿ °ø°Ý
					if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
					{		
						int TempDir = MTopView::GetDirectionToPosition(g_pPlayer->GetX(), g_pPlayer->GetY(),g_SelectSector.x, g_SelectSector.y);
						
//							if(g_pPlayer->GetDirection() != TempDir)
//							{
//								CGMove _CGMove;
//								_CGMove.setX( g_pPlayer->GetX() );
//								_CGMove.setY( g_pPlayer->GetY() );
//								_CGMove.setDir( TempDir );
//								g_pSocket->sendPacket( &_CGMove );
//							}
						if(g_pPlayer->GetDirection() == TempDir)
						{
//							g_pPlayer->UnSetRequestMode();
//							g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);
//							g_pPlayer->SetSpecialActionInfo(SKILL_TURRET_FIRE);
//							if (g_pPlayer->TraceSectorToSpecialAction( g_SelectSector.x, g_SelectSector.y ))
//							{
//								// ¼±ÅÃµÈ Sector·Î Ç¥½ÃÇÑ´Ù.
//								g_pTopView->SetSelectedSector( g_SelectSector );
//							
//								g_pPlayer->SetRepeatAction();
//							}
//
//							g_bPreviousMove = false;
//								char szTemp[128];
//								sprintf(szTemp, "¹æÇâÀÌ °°´Ù: %d,¹æÇâ ", TempDir);
//								g_pSystemMessage->Add(szTemp);
						}
						else
						{
//							CGMove _CGMove;
//							_CGMove.setX( g_pPlayer->GetX() );
//							_CGMove.setY( g_pPlayer->GetY() );
//							_CGMove.setDir( TempDir );
//							g_pSocket->sendPacket( &_CGMove );
							if(g_pPlayer->IsStop())
								g_pPlayer->SetCurrentDirection(TempDir);
						}
					}
					//--------------------------------------------------
					// ¼±ÅÃÇÑ °÷¿¡ Object°¡ ¾øÀ¸¸é MOVE
					//--------------------------------------------------
					else if (pObject == NULL)
					{		
						// l-shift³ª l-controlÀÌ ´­·ÁÀÖÁö ¾ÊÀº »óÅÂ¿¡¼­¸¸ ÀÌµ¿.
						if (//!g_pDXInput->KeyDown(DIK_LSHIFT) &&
							!g_pDXInput->KeyDown(DIK_LCONTROL))
						{		
							if (g_pPlayer->IsNotDelay() && !g_pPlayer->HasEffectStatus(EFFECTSTATUS_ETERNITY_PAUSE ) 
								// 2004, 9, 14, sobeit add start - ÃÑ½½ 130 skill °ü·Ã
								//&&  !g_pPlayer->HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET ) 
								// 2004, 9, 14, sobeit add end - ÃÑ½½ 130 skill °ü·Ã
								)
							{
								if (g_pPlayer->SetMovePosition(g_SelectSector.x, g_SelectSector.y))
								{
									//if (g_pPlayer->IsStop())
									{
										// ´ÙÀ½ ¸ñÇ¥À§Ä¡·Î ¼³Á¤ÇÑ´Ù
										g_pPlayer->TraceNULL();							
										
										g_pPlayer->SetNextActionToMove();
										
										g_bPreviousMove = true;
									}
								}
							}
						}

						/*
						if (g_pPlayer->IsStop())
						{
							// ´ÙÀ½ ¸ñÇ¥À§Ä¡·Î ¼³Á¤ÇÑ´Ù
							g_pPlayer->TraceNULL();
							
							if (g_pPlayer->SetMovePosition(g_SelectSector.x, g_SelectSector.y))
							{
								g_pPlayer->SetNextAction( ACTION_MOVE );
							}
						}
						*/
					}
					//--------------------------------------------------
					// ¼±ÅÃµÈ Object¿¡ ´ëÇØ¼­ 
					//--------------------------------------------------
					else 
					{	
						// Object°¡ ¹º°¡ ¼±ÅÃµÆÀ» °æ¿ì¿¡´Â 
						// LButtonÀ» ´©¸£°í ÀÖ´Â °Í¸¸À¸·Î ÀÌµ¿ÀÌ µÇÁö¾Ê°Ô ÇÑ´Ù.
						//g_bLButtonDown = FALSE;

						switch (pObject->GetObjectType())
						{			
							case MObject::TYPE_CREATURE :	
								{
									// 2004, 12, 3, ¼®¹Î¾¾ Ãß°¡
									MCreature* TempCreature = (MCreature*)pObject;
									if(TempCreature->CurPernalShop() == 1 )
//										|| g_pPlayer->IsFlyingCreature() || g_pPlayer->IsUndergroundCreature() 
//										|| g_pPlayer->IsGroundCreature() || g_pPlayer->IsFakeCreature() 
//										|| g_pPlayer->GetMoveDevice()== 1|| g_pPlayer->GetMoveDevice()== 2)
									{
										gpC_base->SendMessage(UI_REQUEST_STORE_INFO, pObject->GetID(),0);
										gC_vs_ui.SetOtherObjectID(pObject->GetID());
									}
									// 2004, 12, 3, ¼®¹Î¾¾ Ãß°¡

									if (g_pPlayer->TraceCreatureToBasicAction( pObject->GetID(), 
																				bForceAttack, true ))
									{
										//----------------------------------
										// ³²¿¡°Ô ÇÏ´Â ±âº» Çàµ¿ ¹Ýº¹ ¼³Á¤
										//----------------------------------
										g_pPlayer->SetRepeatAction();
										g_bPreviousMove = false;
									}
									
									g_pTopView->SetSelectedSectorNULL();
								}
							break;

							case MObject::TYPE_ITEM :
								g_pPlayer->TraceItem( pObject->GetID() );

								g_bLButtonDown = FALSE;

								g_bPreviousMove = false;
							break;

							case MObject::TYPE_EFFECT :
								g_pPlayer->TraceEffect( pObject->GetID() );

								g_bLButtonDown = FALSE;

								g_bPreviousMove = false;								
							break;
							
							case MObject::TYPE_INTERACTIONOBJECT :
								g_pPlayer->TraceInteractionObject( pObject->GetID(), ((MInteractionObject*)pObject)->GetAction() );

								g_bLButtonDown = FALSE;

								g_bPreviousMove = false;
								//-------------------------
								// TEST CODE
								//-------------------------
								//((MInteractionObject*)pObject)->SetAction( !((MInteractionObject*)pObject)->GetAction() );
							break;
						}
					}
					#ifdef OUTPUT_DEBUG_PROCESS_INPUT
						DEBUG_ADD("LK");
					#endif
				}

				//---------------------------------------------------------------
				//
				//		 RButtonDown
				//
				//---------------------------------------------------------------
				else if (g_pDXInput->m_rb_down)
				{
					#ifdef OUTPUT_DEBUG_PROCESS_INPUT
						DEBUG_ADD("Ro");
					#endif

					g_pPlayer->UnSetRequestMode();

					ProcessInputRButtonDown(pObject);

					g_bPreviousMove = false;

					#ifdef OUTPUT_DEBUG_PROCESS_INPUT
						DEBUG_ADD("RK");
					#endif
				}

				//---------------------------------------------------------------
				// L/R ButtonÀ» ´©¸£Áö´Â ¾Ê¾ÒÁö¸¸,
				// ÀÌÀü¿¡ ´©¸¥ LButtonÀÌ °è¼Ó ´­·ÁÁø »óÅÂ¶ó¸é..
				// But(!), L-Shift°¡ ´­·¯ÁöÁö ¾Ê¾Æ¾ß ÇÑ´Ù.
				//---------------------------------------------------------------
				else if (g_bLButtonDown 				
						&& !g_pDXInput->KeyDown(DIK_LSHIFT)
#ifdef __METROTECH_TEST__
						&& !g_bCButtonDown
#endif
						)
				{
					#ifdef OUTPUT_DEBUG_PROCESS_INPUT
						DEBUG_ADD("ConM");
					#endif

					//--------------------------------------------------
					// ¼±ÅÃÇÑ °÷¿¡ Object°¡ ¾ø°í
					// ¹æ±ÝÀü¿¡ ÀÌµ¿ÇÏ°í ÀÖ´ø ÁßÀÌ¸é MOVE
					//--------------------------------------------------
					if (pObject == NULL && g_bPreviousMove)
					{	
						if (g_pPlayer->IsNotDelay() && !g_pPlayer->IsRepeatAction())
						{
							g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);

							// ´ÙÀ½ ¸ñÇ¥À§Ä¡·Î ¼³Á¤ÇÑ´Ù
							g_pPlayer->TraceNULL();
							
							if (g_pPlayer->SetMovePosition(g_SelectSector.x, g_SelectSector.y))
							{
								g_pPlayer->SetNextActionToMove();
							}
						}
					}

					#ifdef OUTPUT_DEBUG_PROCESS_INPUT
						DEBUG_ADD("ConMK");
					#endif
				}
			}
		}
	}	// lock mode

	//---------------------------------------------------	
	// LButton Up
	//---------------------------------------------------	
	if (g_pDXInput->m_lb_up && g_bLButtonDown
#ifdef __METROTECH_TEST__
		&& !g_bCButtonDown
#endif
		)
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("LUP2");
		#endif

		g_bLButtonDown = FALSE;
	
		if (g_pPlayer->IsRepeatAction())
		{
			// ¹öÆ°À» ¶¼¾úÀ¸¹Ç·Î Çàµ¿ ¹Ýº¹À» Ãë¼ÒÇÑ´Ù.
			g_pPlayer->UnSetRepeatAction();
			//g_pPlayer->TraceNextNULL();

			g_bPreviousMove = false;
		}
		else
		{	
			// ¼±ÅÃµÈ sector·Î Á¤ÇÑ´Ù.
			POINT point;
			
			// ´ÙÀ½ °¥ °÷ÀÌ ¾øÀ¸¸é
			g_pPlayer->GetNextDestination( point );		
			if (point.x==SECTORPOSITION_NULL || point.y==SECTORPOSITION_NULL)
			{
				// ÇöÀç °¡°í ÀÖ´Â °÷ÀÌ ¾øÀ¸¸é
				g_pPlayer->GetDestination( point );
				if (point.x==SECTORPOSITION_NULL || point.y==SECTORPOSITION_NULL)
				{
					//-_-;;
				}
				else
				{
					g_pTopView->SetSelectedSector( point );
				}
			}
			else
			{
				g_pTopView->SetSelectedSector( point );
			}		
		}
	}

	//---------------------------------------------------	
	// RButton Up
	//---------------------------------------------------	
	if (g_pDXInput->m_rb_up && g_bRButtonDown
#ifdef __METROTECH_TEST__
		&& !g_bCButtonDown
#endif
		)
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("RUP2");
		#endif

		g_bRButtonDown = FALSE;
	
		if (g_pPlayer->IsRepeatAction())
		{
			// ¹öÆ°À» ¶¼¾úÀ¸¹Ç·Î Çàµ¿ ¹Ýº¹À» Ãë¼ÒÇÑ´Ù.
			g_pPlayer->UnSetRepeatAction();
			//g_pPlayer->TraceNextNULL();
		}
	}

	if(g_pDXInput->m_cb_up && g_bCButtonDown)
	{
		g_bCButtonDown = FALSE;
	}

	// °¡¸¸È÷ ¼­ ÀÖ´Â »óÅÂÀÌ¸é ¸ñÇ¥ À§Ä¡¸¦ ¾ø¾Ø´Ù.
	if (g_pPlayer->GetAction()==ACTION_STAND)
	{
		g_pTopView->SetSelectedSectorNULL();
	}

	#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("IAEnd");
	#endif


	//---------------------------------------------------
	//
	// keyboard
	//
	//---------------------------------------------------
	/*
	// Missile Á¾·ù ¹Ù²Ù±â
	if (g_pDXInput->KeyDown(DIK_1))
		g_pPlayer->SetActionInfo( ACTIONINFO_BOMB_TO_CREATURE );

	else if (g_pDXInput->KeyDown(DIK_2))
			g_pPlayer->SetActionInfo( ACTIONINFO_FIRE_TO_CREATURE );

	else if (g_pDXInput->KeyDown(DIK_3))
			g_pPlayer->SetActionInfo( ACTIONINFO_LIGHTNING_TO_CREATURE );

	else if (g_pDXInput->KeyDown(DIK_4))
			g_pPlayer->SetActionInfo( ACTIONINFO_ATTACH_FIRE_TO_CREATURE );

	else if (g_pDXInput->KeyDown(DIK_5))
			g_pPlayer->SetActionInfo( ACTIONINFO_ATTACH_AURA_TO_SELF );
	*/

	#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
		//---------------------------------------------------
		// ºñ~~
		//---------------------------------------------------
		if (g_pDXInput->KeyDown(DIK_8))
		{
			SetWeather( WEATHER_RAINY, rand()%10+10);
		}

		//---------------------------------------------------
		// ´«~~
		//---------------------------------------------------
		if (g_pDXInput->KeyDown(DIK_9))
		{
			SetWeather( WEATHER_SNOWY, rand()%10+10);
		}

		//---------------------------------------------------
		// ³¯¾¾ ¸ØÃã
		//---------------------------------------------------
		if (g_pDXInput->KeyDown(DIK_0))
		{
			SetWeather( WEATHER_CLEAR, 0 );
		}



		//---------------------------------------------------
		// ½Ã¾ß  + / -
		//---------------------------------------------------

		if (g_pDXInput->KeyDown(DIK_SUBTRACT) 
			)//&& g_pPlayer->GetLightSight()!=0)
		{
//			g_pPlayer->SetLightSight( g_pPlayer->GetLightSight() - 1 );		
						g_pClientConfig->QuestItemColorSet -= 15;

					if( g_pClientConfig->QuestItemColorSet <= 0 )
						g_pClientConfig->QuestItemColorSet = 495-15;
		}
		
		if (g_pDXInput->KeyDown(DIK_ADD) )// && g_pPlayer->GetLightSight()!=13)
		{
//			g_pPlayer->SetLightSight( g_pPlayer->GetLightSight() + 1 );		
			
			
		}

		//-----------------------------------------------
		// play Sound
		//-----------------------------------------------
		if (g_pDXInput->KeyDown(DIK_F4))
		{
			//g_sb2 = g_Sound.LoadWav((*g_pSoundTable)[rand()%g_pSoundTable->GetSize()].Filename);
			//g_sb2 = g_Sound.LoadWav("Data\\Sound\\Sound1.wav");
			// [ TEST CODE ]
			/*
			static int pan = -10000;
			static int p = 200;
			pan += p;

			if (pan > 10000) 
			{
				pan = 10000;
				p = -p;
			}

			if (pan < -10000) 
			{
				pan = -10000;
				p = -p;
			}
			*/

			//g_Sound.ChangePan((*g_pSoundTable)[rand()%g_pSoundTable->GetSize()].pDSBuffer, pan);
			//g_Sound.Play((*g_pSoundTable)[rand()%g_pSoundTable->GetSize()].pDSBuffer);

			//PlaySound( rand()%g_pSoundTable->GetSize(), 
			//			false,
			//			g_pPlayer->GetX(), g_pPlayer->GetY() );
			PlaySound( SOUND_S_DIE, 
						false,
						g_pPlayer->GetX() + rand()%20-10, g_pPlayer->GetY() + rand()%20-10 );			

		}

		//---------------------------------------------------
		// ¹à±â °¨¼Ò
		//---------------------------------------------------
		if (g_pDXInput->KeyDown(DIK_F9))
		{
			BYTE DarkBits = g_pTopView->GetDarkBits()-1;
			g_pTopView->SetDarkBits(DarkBits);
		}
		
		//---------------------------------------------------
		// ¹à±â Áõ°¡
		//---------------------------------------------------
		if (g_pDXInput->KeyDown(DIK_F10))
		{
			BYTE DarkBits = g_pTopView->GetDarkBits()+1;
			g_pTopView->SetDarkBits(DarkBits);
		}	
		
		// Gamma Test
		/*
		static BYTE gammaStep = 255;
		if (CDirectDraw::IsSupportGammaControl())
		{
			if (g_pDXInput->KeyDown(DIK_LBRACKET))
			{
				gammaStep -= 10;
				CDirectDraw::SetGammaRamp( gammaStep );
			}
			if (g_pDXInput->KeyDown(DIK_RBRACKET))
			{		
				gammaStep += 10;		
				CDirectDraw::SetGammaRamp( gammaStep );
			}
		}
		*/
	#endif

	//-----------------------------------------------
	// File Loading Test with Thread
	//-----------------------------------------------		
	/*
	if (g_pDXInput->KeyDown(DIK_Z))
	{	
		// test for state of event nonsignaled?
		if (WaitForSingleObject(g_hFileEvent, 0) != WAIT_OBJECT_0)
		{
			g_ThreadJob = THREADJOB_LOAD_CREATURE;
			SetEvent(g_hFileEvent);			
		}		
	} 

	if (g_pDXInput->KeyDown(DIK_X) && WaitForSingleObject(g_hFileEvent, 0) != WAIT_OBJECT_0)
	{	
		g_pTopView->ReleaseCreatureSPK( 0 );
		g_pTopView->ReleaseCreatureSPK( 1 );
	}
	*/

	//-----------------------------------------------
	// Thread Priority Test
	//-----------------------------------------------	
	/*
	if (g_pDXInput->KeyDown(DIK_9))
	{
		SetThreadPriority(g_hFileThread, 
							THREAD_PRIORITY_BELOW_NORMAL);
	}

	if (g_pDXInput->KeyDown(DIK_0))
	{
		SetThreadPriority(g_hFileThread, 
							THREAD_PRIORITY_HIGHEST);
	}
	*/
		
	#ifdef OUTPUT_DEBUG
		// ZoneÀÌµ¿ test
		/*
		if (g_pDXInput->KeyDown(DIK_1))
		{	
			MoveZone( 1 );				
		}
		else if (g_pDXInput->KeyDown(DIK_2))
		{				
			MoveZone( 2 );				
		}	
		else if (g_pDXInput->KeyDown(DIK_3))
		{							
			MoveZone( 3 );			
		}
		else if (g_pDXInput->KeyDown(DIK_4))
		{			
			MoveZone( 4 );				
		}
		*/
		

		//-----------------------------------------------
		// Test for Effect
		//-----------------------------------------------
		#ifdef _DEBUG
			static DWORD start=0, current = 0;

			

			current++;

			if (current-start > 10)
			{
				if (g_pDXInput->KeyDown(DIK_LCONTROL) && g_pDXInput->KeyDown(DIK_RCONTROL))
				{					
					
					POINT playerPoint = MTopView::MapToPixel(g_pPlayer->GetX(), g_pPlayer->GetY());

					for (int i=-1; i<=1; i++)
					{
						for (int j=-1; j<=1; j++)
						{
							if (i==0 && j==0)
								continue;
							/*
							if (i==0 && j==0)
							{
								MEffect*	pEffect;
								pEffect = new MEffect;

								pEffect->SetFrameID(0, 8);		// 0¹ø Effect, Max 8 Frame
								pEffect->SetPosition(g_pPlayer->GetX()+j, g_pPlayer->GetY()+i);	// Sector ÁÂÇ¥						
								pEffect->SetCount(125+rand()%8);			// Áö¼ÓµÇ´Â Frame

								g_pZone->AddEffect( pEffect );
							}
							*/

							BYTE effectSpriteType = rand()%(*g_pEffectSpriteTypeTable).GetSize();
			
							//if (g_pDXInput->KeyDown(DIK_1))	effectSpriteType = 0;
							//else if (g_pDXInput->KeyDown(DIK_2))	effectSpriteType = 1;
							//else if (g_pDXInput->KeyDown(DIK_3))	effectSpriteType = 2;
							//else if (g_pDXInput->KeyDown(DIK_4))	effectSpriteType = 3;

							
							TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[effectSpriteType].FrameID;
							BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[effectSpriteType].BltType;
							
							// BLT_NORMAL
							int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);
							
							// Sound ... Test *_*;;
							//g_Sound.Play(g_sb1);



							MLinearEffect*	pEffect;
							//pEffect = new MParabolaEffect(BLT_EFFECT);
							pEffect = new MLinearEffect(BLT_EFFECT);

							pEffect->SetFrameID(frameID, maxFrame);		// 0¹ø Effect, Max 8 Frame

							// ¹ß»ç À§Ä¡ PixelÁÂÇ¥
							pEffect->SetPixelPosition(playerPoint.x, playerPoint.y, 0);	
							
							// ¸ñÇ¥ À§Ä¡ PixelÁÂÇ¥
							pEffect->SetTarget(playerPoint.x + -i*300, 
												playerPoint.y + -j*350,
												0,
												20);	// step

							// Áö¼ÓµÇ´Â Frame (¸ñÇ¥°¡ ÀÖ´Ù¸é º°·Î °ü°è ¾øÀ½ - -;)
							pEffect->SetCount(1000);							

							//pEffect->SetLight(1);							

							g_pZone->AddEffect( pEffect );
							

							//*/	
							/*
							MLinearEffect*	pEffect;
							pEffect = new MLinearEffect;

							pEffect->SetFrameID(0, 8);		// 0¹ø Effect, Max 8 Frame

							// ¹ß»ç À§Ä¡ PixelÁÂÇ¥
							int xx=rand()%800-300;
							int yy=rand()%400-400;
							int last=rand()%500;
							pEffect->SetPixelPosition(playerPoint.x+xx, playerPoint.y+yy, 0);
							
							// ¸ñÇ¥ À§Ä¡ PixelÁÂÇ¥
							pEffect->SetTarget(playerPoint.x+xx-rand()%50 , 
												playerPoint.y+yy+last,
												0,
												20);

							// Áö¼ÓµÇ´Â Frame (¸ñÇ¥°¡ ÀÖ´Ù¸é º°·Î °ü°è ¾øÀ½ - -;)
							pEffect->SetCount(25);						

							g_pZone->AddEffect( pEffect );
							*/
						}					
					}
				}

				start = current;
			}
		#endif
	#endif
}

//---------------------------------------------------------------------------
// UpdateGame Draw
//---------------------------------------------------------------------------
void 
CGameUpdate::UpdateDraw()
{
	//--------------------------------------------------------
	/*
	RECT rectClip = 
	{ 
		50,
		50, 
		750, 
		550 
	};
	g_pBack->SetClip(&rectClip);
	*/
	static bool outputInfo = false;		
	

	POINT	point;
	RECT	rect;
	
	//#ifdef	OUTPUT_DEBUG
		char	str[128];
	//#endif

	// buffer : InitSurface¿¡¼­ SYSTEMMEMORY·Î ÇØÁÖ°í ½á¾ßµÈ´Ù.
	//
	//g_pLast->FillSurface(CDirectDraw::Color(20,20,20));
	
	// È­¸é ¹Ø¿¡ InterfaceºÎºÐ Áö¿öÁÖ±â...
	// [ TEST CODE ]
	/*
	rect.left = 0;
	rect.right = CLIPSURFACE_WIDTH;
	rect.top = CLIPSURFACE_HEIGHT;
	rect.bottom = SURFACE_HEIGHT;
	g_pBack->FillRect(&rect, CDirectDraw::Color(5,7,7));
	*/

	//-----------------------------------------------------------------		
	// ¸¶¿ì½º ÁÂÇ¥ ´Ù½Ã ¼³Á¤
	//-----------------------------------------------------------------
	GetCursorPos(&point);

	ScreenToClient(g_hWnd, &point);//by viva
	
		
	// ui¿¡ mouseÁÂÇ¥ ¼³Á¤
	gC_vs_ui.MouseControl(M_MOVING, point.x, point.y);

	//-----------------------------------------------------------------
	// Å¾ºä Zone Ãâ·Â
	//-----------------------------------------------------------------
	#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			//DEBUG_ADD("dd");//[Update-Draw] Before Draw");
	#endif

//	if (CDirect3D::IsHAL())
//	{
//		//-----------------------------------------------------------------
//		// Game È­¸é Ãâ·Â
//		//-----------------------------------------------------------------
//		__BEGIN_PROFILE("GameDraw3D")
//			
//		const MEvent *event = g_pEventManager->GetEventByFlag(EVENTFLAG_SHAKE_SCREEN);
//		if(event != NULL)
//		{		
//			static int count = 0;
//			const int maxStep = 13;
//			const int step[maxStep] = { 0, 2, 3, 6, 5, 3, 0, -4, -6, -5, -3, -1, 0 };
//			
//			g_pTopView->Draw( step[count]*2, ((g_pEventManager->IsEvent(EVENTID_METEOR_SHAKE) == true)?((count%2)*20-10):(count%3-1)*3) );
//			
//			if(g_bFrameChanged)
//			{
//				count ++;
//				count %= maxStep;			
//			}
//		}
//		else
//			g_pTopView->Draw(0,0);
//
//		__END_PROFILE("GameDraw3D")
//
//		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
//				DEBUG_ADD("us");//[Update-Draw] Before UI Show");
//		#endif
//
//		//-----------------------------------------------------------------
//		// UI Ãâ·Â
//		//-----------------------------------------------------------------		
//		__BEGIN_PROFILE("UIDraw3D")
//
//		gC_vs_ui.Show();		
//
//		__END_PROFILE("UIDraw3D")
//
//
//		__BEGIN_PROFILE("DrawFPS")
//			if (outputInfo || g_pUserOption->DrawFPS)
//			{
//				//-----------------------------------------------------------------
//				// FPS Âï±â	
//				//-----------------------------------------------------------------
//				sprintf(str, "%d FPS(HAL)", g_FrameRate);	
//			
//				g_pBack->GDI_Text(11,11, str, RGB(20,20,20));
//				g_pBack->GDI_Text(10,10, str, 0xFFFFFF);
//			}
//		__END_PROFILE("DrawFPS")
//		
//
//		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
//				DEBUG_ADD("usk");//[Update-Draw] After UI Show");
//		#endif
//
//		__BEGIN_HELP_EVENT
//			UpdateDrawHelp();
//		__END_HELP_EVENT
//	}
//	else
	{
		//-----------------------------------------------------------------
		// Font Ãâ·ÂÇÏ´Â Surface¸¦ ¹Ù²ãÁà¾ß ÇÑ´Ù.
		//-----------------------------------------------------------------
		//g_SetFL2Surface( g_pLast->GetSurface() );

		//-----------------------------------------------------------------
		// Game È­¸é Ãâ·Â
		//-----------------------------------------------------------------
		__BEGIN_PROFILE("GameDraw2D")

//		#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
//			if (g_pDXInput->KeyDown(DIK_SPACE))
			const MEvent *event = g_pEventManager->GetEventByFlag(EVENTFLAG_SHAKE_SCREEN);
			if(event != NULL && event->IsShowTime() == true)
			{		
				static int count = 0;
				const int maxStep = 13;

				// 2004, 5, 21 sobeit modify start
				//const int step[maxStep] = { 0, 2, 3, 6, 5, 3, 0, -4, -6, -5, -3, -1, 0 };
				//g_pTopView->Draw( step[count]*event->parameter3, ((g_pEventManager->IsEvent(EVENTID_METEOR_SHAKE) == true)?((count%2)*3-3):(count%3-1)*3)*event->parameter3 );
				const int step[maxStep] = { 0, 1, 2, 2, 2, 1, 0, -2, -2, -2, -1, -1, 0 };
				g_pTopView->Draw( step[count]*event->parameter3, ((g_pEventManager->IsEvent(EVENTID_METEOR_SHAKE) == true)?((count%2)*3-2):(count%3-1)*3)*event->parameter3 );
				// 2004, 5, 21 sobeit modify end

				if(g_bFrameChanged)
				{
					count ++;
					count %= maxStep;			
				}
			}
			else
//		#endif
			g_pTopView->Draw(0,0);			

		__END_PROFILE("GameDraw2D")

		//g_SetFL2Surface( g_pBack->GetSurface() );

		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				//DEBUG_ADD("us");//[Update-Draw] Before UI Show");
		#endif

		//-----------------------------------------------------------------
		// UI Ãâ·Â
		//-----------------------------------------------------------------		
		__BEGIN_PROFILE("UIDraw2D")

		gC_vs_ui.Show();

		__END_PROFILE("UIDraw2D")

		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				//DEBUG_ADD("usk");//[Update-Draw] After UI Show");
		#endif

//		__BEGIN_HELP_EVENT
//			UpdateDrawHelp();
//		__END_HELP_EVENT

		if (!g_pTopView->IsDrawRequest())
		{
			gC_vs_ui.DrawMousePointer();
		}

		
		__BEGIN_PROFILE("DrawFPS")
			if (outputInfo || g_pUserOption->DrawFPS)
			{
				//-----------------------------------------------------------------
				// FPS Âï±â	
				//-----------------------------------------------------------------				
				sprintf(str, "%d FPS", g_FrameRate);	
				
				g_pLast->GDI_Text(11,11, str, RGB(20,20,20));
				g_pLast->GDI_Text(10,10, str, 0xFFFFFF);
			}
		__END_PROFILE("DrawFPS")

		//-----------------------------------------------------------------
		// Last¸¦ BackÀ¸·Î copy - 3D HALÀÌ ¾Æ´Ñ °æ¿ì¸¸..
		//-----------------------------------------------------------------
		__BEGIN_PROFILE("LastToBack")

//		if(CDirect3D::IsHAL())
//		{
//			point.x = 0;
//			point.y = 0;
//			rect.left = 0;
//			rect.top = 0;
//			rect.right = SURFACE_WIDTH;
//			rect.bottom = SURFACE_HEIGHT;
//			g_pLast->SetTransparency(0x001F);
//			g_pBack->Blt( &point, g_pLast, &rect );		
//		}
//		else
//		{
			point.x = 0;
			point.y = 0;
			rect.left = 0;
			rect.top = 0;
			rect.right = g_GameRect.right;
			rect.bottom = g_GameRect.bottom;
			g_pBack->BltNoColorkey( &point, g_pLast, &rect );		
//		}

		__END_PROFILE("LastToBack")
	}
	
	#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			//DEBUG_ADD("ddk");//[Update-Draw] After Draw");
	#endif


	//-------------------------------------------------------------------
	// Mouse·Î ¼±ÅÃÇÑ ÁÂÇ¥¿¡ ´ëÇÑ debug¿ë code
	//-------------------------------------------------------------------
	/*
	WORD*	lpSurface;
	long	lPitch;
	g_pLast->LockW(NULL, lpSurface, lPitch);

	for (int y=g_y-30; y<g_y+30; y++)
		for (int x=g_x-60; x<g_x+60; x++)
		{
			point = g_pTopView->GetSelectedSector(x,y);

			if (point.x==g_SelectSector.x && point.y==g_SelectSector.y)
			{
				*(lpSurface + y*lPitch + x) = 0xCCCC;
			}
		}

	g_pLast->Unlock();

	sprintf(str, "Selected Sector = (%d, %d)", g_SelectSector.x, g_SelectSector.y);
	g_pLast->GDI_Text(10,200, str, RGB(220,220,220));
	*/


	//-----------------------------------------------------------------
	// tab´©¸£¸é MiniMapÀÌ º¸¿©Áø´Ù.
	//-----------------------------------------------------------------
	//if (g_bDrawMinimap)
	//{	
	//	g_pTopView->DrawMinimap(
	//			800-g_pZone->GetWidth()*2,	//10
	//			0, //-g_pZone->GetHeight(),	// 465
	//			1);
	//}

	//-----------------------------------------------------------------
	// ³×Æ®¿÷ »óÅÂ°¡ ÁÁÀº°¡?
	//-----------------------------------------------------------------
	/*
	if (!g_bNetStatusGood)
	{
		DEBUG_ADD("Network Condition is Bad.");

		g_pBack->GDI_Text(351,11, (*g_pGameStringTable)[STRING_NETWORK_CONDITION_BAD].GetString(), RGB(10,10,10));
		g_pBack->GDI_Text(350,10, (*g_pGameStringTable)[STRING_NETWORK_CONDITION_BAD].GetString(), RGB(230,50,50));		
	}
	*/

	//-----------------------------------------------------------------
	// Á×Àº »óÅÂ..
	//-----------------------------------------------------------------
	if (g_pPlayer->IsDead())
	{
		if (g_pPlayer->GetActionCount()==g_pPlayer->GetActionCountMax())		
		{	
			static BYTE a = 0;
			static BYTE count = 0;

			int second = g_pPlayer->GetDeadDelayLast();
			//char str[128];

			// 30~20
			int blackValue = 20;
			if (second >= 3)
			{
				//blackValue = max(0, (g_pClientConfig->DELAY_PLAYER_DEAD/1000 - (second-5)*2)) * 2;
				blackValue = min(31, second+20);
			}

			// Á×´Â µ¿ÀÛÀÌ ³¡³­ °æ¿ì¿¡ È­¸éÀ» °Ë°Ô...
			g_pTopView->SetFadeStart(blackValue, 0, 0, 5,5,5);
		

			// 6 frame¾¿ º¸¿©ÁØ´Ù.
			if ((a & 0x01)==0)
			{
				//if (second==0)
				{
					//sprintf(str, "[SPACE]¸¦ ´©¸£¸é µÇ»ì¾Æ³¯ ¼ö ÀÖ½À´Ï´Ù.", second);
					//g_pBack->GDI_Text(270,400, str, RGB(230,230,230));
				}
				//else
				{
				//	sprintf(str, (*g_pGameStringTable)[STRING_RESURRECTION_AFTER_SECONDS].GetString(), second);
					//strcpy(str, "... Please Wait for Your Resurrection ...");
				//	g_pBack->GDI_Text(301,401, str, RGB(0,0,0)); 
				//	g_pBack->GDI_Text(300,400, str, RGB(230,230,230));
				}
				
				// 6ÃÊ°¡ Áö³ª°í³ª¼­ ºÎÈ°¹öÆ°À» ¶ç¿î´Ù.
				if (second < 4
					&& g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
				{
					// FixMe Please oh shit by ssuk
					if( (g_pWarManager->IsExist( g_pZone->GetID() ) || g_pZone->GetID() == 71 || g_pZone->GetID() == 72 || g_pZone->GetID() == 73) && gC_vs_ui.IsRunningBloodBibleStatus() )
						gC_vs_ui.RequestShrineMinimap( g_pClientConfig->DELAY_PLAYER_RESURRECT_TIMEOUT );

					gC_vs_ui.RequestDie( g_pClientConfig->DELAY_PLAYER_RESURRECT_TIMEOUT );

					g_pTempInformation->SetMode(TempInformation::MODE_WAIT_RESURRECT);
					bool bResurrect = false, bElixir= false, bEternity= false, IsSiegeAttacker= false ;

					// 2005, 1, 18, sobeit add start - Äù½ºÆ® actionÀ¸·Î Á×Àº °æ¿ì¿£ ¾Æ·¡¸¦ Ã¼Å© ¾ÈÇÑ´Ù.
					if(0 == g_pPlayer->GetResurrectZoneID())
					// 2005, 1, 18, sobeit add end
					{
						if( g_pInventory->FindItem( ITEM_CLASS_RESURRECT_ITEM, 0 ) != NULL )
							bResurrect = true;

						if(g_pInventory->FindItem( ITEM_CLASS_RESURRECT_ITEM, 1 ) != NULL )
							bElixir = true;

						//bEternity = (*g_pSkillInfoTable)[SKILL_ETERNITY].IsEnable();						
						bEternity = g_pSkillAvailable->IsEnableSkill( SKILL_ETERNITY ) &&
									(*g_pSkillInfoTable)[SKILL_ETERNITY].IsAvailableTime() 
									// 2004, 11, 11, sobeit add start - ·¹º§ °Ë»çµµ ÇÑ´Ù.. ¿ä°Å..g_char_slot_ingame.DOMAIN_HEAL´Â ¹¹Áö? -_-; ¹ö±× ¼öÁ¤..¤Ñ¤Ñ;
									&& (*g_pSkillManager)[SKILLDOMAIN_HEAL].GetDomainLevel() >= (*g_pSkillInfoTable)[SKILL_ETERNITY].GetLearnLevel();
									// 2004, 11, 11, sobeit add end
						IsSiegeAttacker = g_pZone->GetPKType() == PK_TYPE_SIEGE &&
										 ( g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_1) ||
										 g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_2) ||
										 g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_3) ||
										 g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_4) ||
										 g_pPlayer->HasEffectStatus(EFFECTSTATUS_SIEGE_ATTACKER_5) );
					}
					gC_vs_ui.RequestResurrect( bResurrect, bElixir, bEternity , IsSiegeAttacker);
				}
			}

			static DWORD lastTime = g_CurrentTime;
			if (g_CurrentTime - lastTime >= g_UpdateDelay)
			{
				if (++count==6) { count = 0; a++; }
				lastTime = g_CurrentTime;
			}			
		}
	}

	//-----------------------------------------------------------------
	// Mouse Cursor À§Ä¡ÀÇ image¸¦ ±â¾ï½ÃÄÑµÎ±â
	//-----------------------------------------------------------------
	//
	// FullScreenÀÌ°í...
	// ¹Ù·Î ÀüÀÇ FPS°¡ Ãâ·ÂFPS ÇÑ°è¸¦ ³ÑÀ» °æ¿ì....
	// Ä¿¼­ Ãâ·Â À§Ä¡¸¦ ±â¾ïÇÑ´Ù.
	// 	
	if (g_pUserOption->UseSmoothCursor)
	{
		if (CDirectDraw::IsFullscreen() && g_FrameRate > LIMIT_FPS_FOR_CURSOR)
		{
			GetCursorPos(&point);
			
			ScreenToClient(g_hWnd, &point);//by viva
			
			// ui¿¡ mouseÁÂÇ¥ ¼³Á¤
			gC_vs_ui.MouseControl(M_MOVING, point.x, point.y);

			// ÀúÀåÇÒ ¿µ¿ª ¼³Á¤
			MOUSEPOINTER_INFO mp_info;
			gC_vs_ui.GetCurrentMousePointerInfo(mp_info);

			g_pCursorSurface->Init(2, mp_info.width, mp_info.height);
			//DEBUG_ADD_FORMAT("[MouseCursor] %d,%d, %d,%d", mp_info.width, mp_info.height, mp_info.rx, mp_info.ry);
			
			POINT tempPoint;
			tempPoint.x = mp_info.x;
			tempPoint.y = mp_info.y;
			g_pCursorSurface->Store(0, g_pBack, &tempPoint);
			
			g_bSmoothCursor = true;
		}
		else
		{
			g_bSmoothCursor = false;
		}	
	}
	else
	{
		//GetCursorPos(&point);	
			
		// ui¿¡ mouseÁÂÇ¥ ¼³Á¤
		//gC_vs_ui.MouseControl(M_MOVING, point.x, point.y);
	}

	if (CDirect3D::IsHAL())
	{
		//-----------------------------------------------------------------
		// Mouse ±×¸®±â
		//-----------------------------------------------------------------		
		if (!g_pTopView->IsDrawRequest())
		{
			gC_vs_ui.DrawMousePointer();
		}		
	}
	else
	{
		// Ã¢¸ðµå¿¡¼­ 3D°¡¼Ó ¾ÈÇÑ °æ¿ì¿¡..
		// ¿Ö ÀÌ°Å ÇÏ´Ï±î »¡¶óÁöÁö? - -;
		//HDC hdc;
		//g_pBack->GetSurface()->GetDC(&hdc);
		//g_pBack->GetSurface()->ReleaseDC(hdc);
	}

	//-----------------------------------------------------------------
	// Debug InformationÃâ·Â
	//-----------------------------------------------------------------
	__BEGIN_PROFILE("DrawDebugInfo")

	PrintInfo* pPrintInfo = &gpC_base->m_chatting_pi;

	#ifdef	OUTPUT_DEBUG
		static DWORD lastCount = 0;	
		
		if (g_bSlideScreenShot)
		{
			if (g_bSlideRectSelect)
			{
				g_rectScreenShot.right = g_x;
				g_rectScreenShot.bottom = g_y;
			}

			RECT rect = g_rectScreenShot;

			g_pBack->DrawRect( &rect, CDirectDraw::Color(3, 30, 10) );
		}
		
		if (g_pDXInput->KeyDown(DIK_LSHIFT) && g_pDXInput->KeyDown(DIK_RSHIFT) 
			&& g_CurrentFrame-lastCount > 7)
		{
			outputInfo = !outputInfo;
			lastCount = g_CurrentFrame;
		}

		//----------------------------------------------------------------
		// UDP packet test
		//----------------------------------------------------------------
		#ifdef __METROTECH_TEST__

			if (g_pPlayer!=NULL)
				
			{
				if (g_iAutoHealPotion && !g_pPlayer->IsVampire())
				{
					RECT rect = 
					{ 
						2,
						448,
						10,
						455
					};

					if(g_iAutoHealPotion == 1)
						g_pBack->FillRect(&rect, CDirectDraw::Color(30, 10, 10));
					else if(g_iAutoHealPotion == 2)
						g_pBack->FillRect(&rect, CDirectDraw::Color(30, 20, 10));
					else if(g_iAutoHealPotion == 3)
						g_pBack->FillRect(&rect, CDirectDraw::Color(30, 10, 20));
				}

				if (g_bAutoManaPotion && !g_pPlayer->IsVampire())
				{
					RECT rect = 
					{ 
						12,
						448,
						20,
						455
					};

					g_pBack->FillRect(&rect, CDirectDraw::Color(10, 10, 30));
				}

				if (g_iAutoReload && g_pPlayer->IsSlayer())
				{
//					char sz_temp[3];
//					wsprintf(sz_temp, "%d", g_iAutoReload-1);
//					g_PrintColorStr(22, 448, sz_temp, gpC_base->m_chatting_pi, RGB_GREEN);

					RECT rect = 
					{ 
						22,
						448,
						30,
						455
					};

					g_pBack->FillRect(&rect, CDirectDraw::Color(10, 30, 10));

					rect.left++;
					rect.right--;
					rect.bottom++;

					for(int i = 0; i < g_iAutoReload-1; i++)
					{
						rect.bottom -= 2;
						rect.top = rect.bottom -1;
						g_pBack->FillRect(&rect, CDirectDraw::Color(0, 0, 0));
					}
				}

				if (g_bLight)
				{
					RECT rect = 
					{ 
						32,
						448,
						40,
						455
					};
					
					g_pBack->FillRect(&rect, CDirectDraw::Color(10, 30, 30));
				}

				if (g_iSpeed)
				{
					RECT rect = 
					{ 
						42,
						448,
						50,
						455
					};

					if(g_iSpeed < 0)
						g_pBack->FillRect(&rect, CDirectDraw::Color(30, 0, 0));
					else
						g_pBack->FillRect(&rect, CDirectDraw::Color(30, 30, 30));
					
					rect.left++;
					rect.right--;
					rect.bottom++;
					
					for(int i = 0; i < abs(g_iSpeed)-1; i++)
					{
						rect.bottom -= 2;
						rect.top = rect.bottom -1;
						g_pBack->FillRect(&rect, CDirectDraw::Color(0, 0, 0));
					}
				}
			}

			if (outputInfo)
			{
				const int numValue = 5;
				int value[numValue] = 
				{
					g_UDPTest.UDPPacketAvailable,
					g_UDPTest.UDPPacketReceive,
					g_UDPTest.UDPPacketRead,
					g_UDPTest.UDPPacketExecute,
					g_UDPTest.UDPPacketSend
				};

				WORD color[numValue] =
				{
					CDirectDraw::Color(10, 20, 20),
					CDirectDraw::Color(20, 20, 20),
					CDirectDraw::Color(30, 20, 20),
					CDirectDraw::Color(20, 30, 20),
					CDirectDraw::Color(20, 20, 30)
				};

				for (int v=0; v<numValue; v++)
				{
					RECT rect = 
					{ 
						210,
						455 + v*8,
						210 + (value[v]%100)*4,
						455 + v*8 + 8
					};

					g_pBack->DrawRect(&rect, color[v]);
				}

				if (outputInfo)
				{
					if (!CDirect3D::IsHAL())
					{
						g_SetFL2Surface( g_pBack->GetSurface() );
					}

					const COLORREF color = RGB(28<<3, 28<<3, 28<<3);				
					pPrintInfo->text_color	= color;
		
					char str[10];
					for (int i=0; i<numValue; i++)
					{
						sprintf(str, "%d", value[i]);
						g_Print(200 + i*40, 455 + i*8, str, pPrintInfo);
					}				

					if (!CDirect3D::IsHAL())
					{
						g_SetFL2Surface( g_pLast->GetSurface() );
					}
				}
			}
		#endif


		if (outputInfo)//!g_pDXInput->KeyDown(DIK_RSHIFT))
		{
			g_pTopView->DrawDebugInfo( g_pBack );

			if (!CDirect3D::IsHAL())
			{
				g_SetFL2Surface( g_pBack->GetSurface() );
			}

			///*
			//-----------------------------------------------------------------
			//							Lock
			//-----------------------------------------------------------------
			//if (!g_pBack->Lock()) return;

			// SurfaceÀÇ Á¤º¸¸¦ ÀúÀåÇØµÐ´Ù.
			//S_SURFACEINFO		SurfaceInfo;
			//SetSurfaceInfo(&SurfaceInfo, g_pBack->GetDDSD());
		
			if (g_bPutMessage && g_pDebugMessage!=NULL)
			{
				int strY = 565;
				
				for (int c=g_pDebugMessage->GetSize()-1; c>=0; c--)
				{
					if ((*g_pDebugMessage)[c][0] != NULL)
					{
						//g_pBack->GDI_Text(10,strY, g_DebugMessage[c], RGB(230,230,230));
						//gC_font.PrintStringNoConvert(&SurfaceInfo, (*g_pDebugMessage)[c], 10,strY, CDirectDraw::Color(28,28,28));

						const COLORREF debugColor = RGB(28<<3, 28<<3, 28<<3);				
						pPrintInfo->text_color	= debugColor;
			
						g_Print(10, strY, (*g_pDebugMessage)[c], pPrintInfo);
						strY-=15;
					}
				}

				// Debug Log Filename Ãâ·ÂÇÏ±â
				if (g_pDebugMessage->GetFilename()!=NULL)
				{
					sprintf(str, "LogFile : %s", g_pDebugMessage->GetFilename());
					//gC_font.PrintStringNoConvert(&SurfaceInfo, str, 10,580, CDirectDraw::Color(31,11,11));

					const COLORREF txColor = RGB(31<<3, 11<<3, 11<<3);										
					pPrintInfo->text_color	= txColor;
			
					g_Print(10, 580, str, pPrintInfo);
				}				
			}	
			//*/
			
			if (g_pLoadingThread!=NULL && g_pLoadingThread->IsWorking())
			{		
				sprintf(str, "<<< Loading >>>");
				//g_pBack->GDI_Text(700,580, str, 0xFFFFFF);
				//gC_font.PrintStringNoConvert(&SurfaceInfo, str, 300,5, CDirectDraw::Color(31,31,31));
				const COLORREF txColor = RGB(31<<3, 31<<3, 31<<3);				

				pPrintInfo->text_color	= txColor;
				
				g_Print(300, 5, str, pPrintInfo);
			}
			
			// MissileÁ¾·ù
			///*	 
			if (g_pPlayer->GetSpecialActionInfo() != ACTIONINFO_NULL)
			{
				sprintf(str, "[%d] %s", g_pPlayer->GetSpecialActionInfo(), (*g_pActionInfoTable)[g_pPlayer->GetSpecialActionInfo()].GetName());
				
				//g_pBack->GDI_Text(550,10, str, RGB(20,20,20));
				//g_pBack->GDI_Text(551,11, str, RGB(120,255,120));
				//gC_font.PrintStringNoConvert(&SurfaceInfo, str, 500,10, CDirectDraw::Color(2,2,2));
				//gC_font.PrintStringNoConvert(&SurfaceInfo, str, 501,11, CDirectDraw::Color(16,31,16));


				const COLORREF bkColor = RGB(0, 0, 0);
				const COLORREF txColor = RGB(16<<3, 31<<3, 16<<3);				

				pPrintInfo->text_color	= bkColor;
				g_Print(500, 10, str, pPrintInfo);

				pPrintInfo->text_color	= txColor;
				g_Print(501, 11, str, pPrintInfo);				
			}	
			//*/

			// °¡Áø µ·
			sprintf(str, "Money : %d", (*g_pMoneyManager).GetMoney());
			
			//g_pBack->GDI_Text(550,10, str, RGB(20,20,20));
			//g_pBack->GDI_Text(551,11, str, RGB(120,255,120));
			//gC_font.PrintStringNoConvert(&SurfaceInfo, str, 500,10, CDirectDraw::Color(2,2,2));
			//gC_font.PrintStringNoConvert(&SurfaceInfo, str, 501,11, CDirectDraw::Color(16,31,16));
			
			const COLORREF bkMoneyColor = RGB(0, 0, 0);
			const COLORREF txMoneyColor = RGB(16<<3, 31<<3, 16<<3);				

			pPrintInfo->text_color	= bkMoneyColor;
			g_Print(500, 30, str, pPrintInfo);

			pPrintInfo->text_color	= txMoneyColor;
			g_Print(501, 31, str, pPrintInfo);

			sprintf(str, "QuestColorset = %d", g_pClientConfig->QuestItemColorSet);
			pPrintInfo->text_color	= txMoneyColor;
			g_Print(501, 51, str, pPrintInfo);

			//-----------------------------------------------------------------
			//							UnLock
			//-----------------------------------------------------------------
			//g_pBack->Unlock();
		

			/*
			// ÀÓ½Ã·Î item °³¼ö º¸¿©ÁÖ±â
			if (gpC_mouse_pointer->GetPickUpItem() != NULL)
			{
				sprintf(str, "%d", gpC_mouse_pointer->GetPickUpItem()->GetNumber());
				g_pBack->GDI_Text(g_x+1,g_y+1, str, RGB(20,20,20));
				g_pBack->GDI_Text(g_x,g_y, str, 0xFFFFFF);
			}
			*/

			if (!CDirect3D::IsHAL())
			{
				g_SetFL2Surface( g_pLast->GetSurface() );
			}
		}
	#endif

	/*
	if (outputInfo || (*g_pUserOption).DrawFPS)
	{
		//-----------------------------------------------------------------
		// FPS Âï±â	
		//-----------------------------------------------------------------
		if (CDirect3D::IsHAL())
		{
			sprintf(str, "%d FPS(HAL)", g_FrameRate);	
		}
		else
		{
			sprintf(str, "%d FPS", g_FrameRate);	
		}
		g_pBack->GDI_Text(11,11, str, RGB(20,20,20));
		g_pBack->GDI_Text(10,10, str, 0xFFFFFF);
	}
	*/

	__END_PROFILE("DrawDebugInfo")

	//---------------------------------------------------------------------
	//
	// [ TEST CODE ] - Sword DomainÀÇ SkillµéÀ» °¡Áö°í ÀÛ¾÷ÇÑ´Ù.
	//
	//---------------------------------------------------------------------
	/*
	MSkillDomain& swordDomain = g_SkillManager[SKILLDOMAIN_SWORD];

	//---------------------------------------------------------------------
	// ¸î°¡Áö skillÀ» ¹è¿ü´Ù°í Ç¥½ÃÇÑ´Ù.
	//---------------------------------------------------------------------
	//swordDomain.LearnSkill( SKILL_DOUBLE_IMPACT );
	//swordDomain.LearnSkill( SKILL_TRIPLE_SLASHER );
	//swordDomain.LearnSkill( SKILL_RAINBOW_SLASHER );
	//swordDomain.LearnSkill( SKILL_HURRICANE_COMBO );

	//swordDomain.UnLearnSkill( SKILL_HURRICANE_COMBO );

	//---------------------------------------------------------------------
	// Sword DomainÀÇ ¸ðµç ±â¼úµéÀ» Ãâ·ÂÇÑ´Ù.
	//---------------------------------------------------------------------
	swordDomain.SetBegin();
	
	while (swordDomain.IsNotEnd())
	{
		// skillÀÇ id¿Í status
		ACTIONINFO					id		= swordDomain.GetSkillID();
		MSkillDomain::SKILLSTATUS	status	= swordDomain.GetSkillStatus();

		//---------------------------------------
		// status´Â ´ÙÀ½°ú °°´Ù. 
		//---------------------------------------
		//	MSkillDomain::SKILLSTATUS_LEARNED		// ¹è¿ü´Ù.
		//	MSkillDomain::SKILLSTATUS_NEXT			// ´ÙÀ½¿¡ ¹è¿ï ¼ö ÀÖ´Ù.
		//	MSkillDomain::SKILLSTATUS_OTHER			// ¾ÆÁ÷Àº ¹è¿ï ¼ö ¾ø´Ù.	
		//---------------------------------------
		
		//---------------------------------------
		// id¸¦ ¾Ë¸é g_SkillInfoTable¿¡¼­ 
		// ±× idÀÇ skill¿¡ ´ëÇÑ Á¤º¸¸¦ ¾òÀ» ¼ö ÀÖ´Ù.
		//---------------------------------------
		COLORREF color;
		switch (status)
		{
			case MSkillDomain::SKILLSTATUS_LEARNED :	
				color = RGB(250, 250, 250);
			break;

			case MSkillDomain::SKILLSTATUS_NEXT :	
				color = RGB(10, 10, 250);
			break;

			case MSkillDomain::SKILLSTATUS_OTHER :	
				color = RGB(120, 120, 120);
			break;
		}

		SKILLINFO_NODE& skillInfo = g_SkillInfoTable[id];
		
		int x = skillInfo.GetX()*2/3 - strlen(skillInfo.GetName())*8 + 100;
		int y = skillInfo.GetY();
		g_pBack->GDI_Text(x+1, y+1, skillInfo.GetName(), 0);
		g_pBack->GDI_Text(x, y, skillInfo.GetName(), color);

		// ´ÙÀ½
		swordDomain.Next();
	}
	//*/

	// Ä¿¼­ Ãâ·Â
	//WORD color = 0xFFFF;//(rand()%2)?CDirectDraw::Color(20,20,20):CDirectDraw::Color(230,230,230);
	//g_pBack->HLine(point.x-7, point.y, 7, color);
	//g_pBack->HLine(point.x+1, point.y, 7, color);
	//g_pBack->VLine(point.x, point.y-7, 7, color);
	//g_pBack->VLine(point.x, point.y+1, 7, color);	
	//*/
	

	//-----------------------------------------------------------------
	// flip
	//-----------------------------------------------------------------
	CDirectDraw::Flip();
}

//-----------------------------------------------------------------------------
// Update Draw Help
//-----------------------------------------------------------------------------
void
CGameUpdate::UpdateDrawHelp()
{
	int strX = 350;
	int strY = (g_pPlayer->IsSlayer()? 430 : 460);

	PrintInfo* pPrintInfo = &gpC_base->m_info_pi;

	for (int c=g_pHelpMessage->GetSize()-1; c>=0; c--)
	{
		if ((*g_pHelpMessage)[c][0] != NULL)
		{
			const COLORREF color = RGB(28<<3, 28<<3, 8<<3);	//CDirectDraw::Color(29,8,12);

			pPrintInfo->text_color = 0;
			g_Print(strX+1, strY+1, (*g_pHelpMessage)[c], pPrintInfo);

			pPrintInfo->text_color = color;
			g_Print(strX, strY, (*g_pHelpMessage)[c], pPrintInfo);

			strY-=20;
		}		
	}

	// 5ÃÊ¸¶´Ù ÇÑ¹ø¾¿.. scroll
	static DWORD HelplastTime = g_CurrentTime;
	if (g_CurrentTime - HelplastTime >= g_pClientConfig->DELAY_GAMEMESSAGE)
	{
		g_pHelpMessage->Add("\0");
		HelplastTime = g_CurrentTime;
	}
}

//-----------------------------------------------------------------------------
// PacketTest
//-----------------------------------------------------------------------------
void
PacketTest()
{
	MZone::CREATURE_MAP::const_iterator iCreature = g_pZone->GetCreatureBegin();

	for (int i=0; i<g_pZone->GetCreatureNumber(); i++)
	{
		MCreature* pCreature = (*iCreature).second;

		if (pCreature->GetID()!=g_pPlayer->GetID()
			&& !pCreature->IsNPC()
			&& (rand()%8==0))
		{	
			int x = pCreature->GetX();
			int y = pCreature->GetY();
			int dir = rand()%8;
			pCreature->PacketMove(x, y, dir);
		}

		iCreature++;
	}
}

//-----------------------------------------------------------------------------
// AttackMelee
//-----------------------------------------------------------------------------
/*
#include "packet\Gpackets\GCAttackMeleeOK2.h"
#include "packet\Gpackets\GCAttackMeleeOK3.h"
void
PacketAttackMelee(int user, int target)
{
	Packet* pPacket;
	
	if (user==g_pPlayer->GetID())
	{
		pPacket = new GCAttackMeleeOK1;
	}
	GCAttackMeleeOK3 packet;
	packet.setObjectID( user );
	packet.setTargetObjectID( target );

	packet.execute( g_pSocket );
}
*/
///ÒÔÏÂÊÇ¼ì²âÏµÍ³ÊÇ·ñÓÐµÇÂ½¶þ¸öÓÃ»§
int GetCurrentUserNumber()
{
	HANDLE			hSnapShot;
    PROCESSENTRY32	ProcessInfo;
	char*			pstrExeName;
    DWORD			ret = 0;
	DWORD			dwwinlogon = 0;
	DWORD			dwexplorer = 0;
	DWORD			dwcsrss = 0;

	ProcessInfo.dwSize = sizeof(ProcessInfo);
    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hSnapShot == INVALID_HANDLE_VALUE) return 0;
	if(!Process32First(hSnapShot,&ProcessInfo)) return 0;
	do
	{
		pstrExeName = strrchr(ProcessInfo.szExeFile,'\\');
        if(!pstrExeName)
            pstrExeName = ProcessInfo.szExeFile;
        else
            pstrExeName++; 
		if(!stricmp(pstrExeName, "winlogon.exe"))
		{
			dwwinlogon++;
		}
		if(!stricmp(pstrExeName, "explorer.exe"))
		{
			dwexplorer++;
		}
		/*
		if(!stricmp(pstrExeName, "csrss.exe"))
		{
			dwcsrss++;
		}
		*/
		if (dwwinlogon>1 && dwexplorer >1)
		{
			ret =2;
		}
	}while(Process32Next(hSnapShot,&ProcessInfo));
	if(hSnapShot)
		CloseHandle(hSnapShot);	
	
	return ret;
}
//-----------------------------------------------------------------------------
// Update Game
//-----------------------------------------------------------------------------
// °ÔÀÓ ½ÇÇà Áß...
//-----------------------------------------------------------------------------
void 
CGameUpdate::Update(void)
{
	#ifdef OUTPUT_DEBUG_UPDATE_LOOP
		//DEBUG_ADD("UB");
	#endif

	__BEGIN_PROFILE("GameUpdate")

	static DWORD lastTime = g_CurrentTime;
	//bool bChanged = false;

	if (g_Mode!=MODE_GAME || g_pClientConfig==NULL)
	{
		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			//DEBUG_ADD("UE1");
		#endif
		return;
	}
	
	//------------------------------------------
	// Logout ÇÒ ½Ã°£ÀÎÄ¡ Ã¼Å©ÇÑ´Ù.
	//------------------------------------------
	if (g_pUserInformation!=NULL
		&& g_pUserInformation->LogoutTime!=0
		&& g_CurrentTime > g_pUserInformation->LogoutTime
		&& g_pPlayer->GetWaitVerify() == MPlayer::WAIT_VERIFY_LOGOUT)
	{
		ExecuteLogout();

		g_pUserInformation->LogoutTime = 0;	// Logout½Ã°£ Á¦°Å

		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			//DEBUG_ADD("UE2");
		#endif

		return;
	}

	//------------------------------------------
	// Sound Stream
	//------------------------------------------
//	if (g_pDXSoundStream!=NULL)
//	{
//		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
//			DEBUG_ADD("DSU");
//		#endif
//
//		g_pDXSoundStream->Update();
//
//		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
//			DEBUG_ADD("DSUk");
//		#endif
//	}


	//---------------------------------------------
	// Sound°ü·Ã
	//---------------------------------------------
	// ¸Å loop ¸¶´Ù ¾ÈÇØÁàµµ µÇÁö ¾ÊÀ»±î..
	#ifdef OUTPUT_DEBUG_UPDATE_LOOP
		//DEBUG_ADD("DXRTDB");
	#endif

	g_DXSound.ReleaseTerminatedDuplicateBuffer();

	#ifdef OUTPUT_DEBUG_UPDATE_LOOP
		//DEBUG_ADD("DXRTDBk");
	#endif
	
	static DWORD nextSoundCheckTime = g_CurrentTime;

	if (g_CurrentTime > nextSoundCheckTime)
	{
		// ÃÊ´ç playÇÑ sound ¼ö..
		g_SoundPerSecond = 0;

		// 1ÃÊ ÈÄ
		nextSoundCheckTime = g_CurrentTime + 1000;
	}

	//---------------------------------------------------
	//
	// ÀÏÁ¤ ½Ã°£¸¶´Ù ÇÑ¹ø¾¿ Ã³¸®¸¦ ÇØÁØ´Ù.
	//
	//---------------------------------------------------
	// [¿¹Á¦]
	// g_UpdateDelay	: 100
	// lastTime			: 1000 
	// currentTime		: 1240 
	//
	// 1200, 1100ÀÇ µÎ¹øÀ» Ã³¸®ÇÏ°Ô µÈ´Ù.
	//---------------------------------------------------

	int k = g_pClientConfig->MAX_UPDATE_ONETIME;		// 12 frame * 1 Tile.. 

	#ifdef OUTPUT_DEBUG
		if (g_bSlideScreenShot)
		{
			k = 1;
		}
	#endif

	UpdateMouse();

	//------------------------------------------
	// ¹«ÇÑ·çÇÁ ¿¹¹æµµ µÇ°í...
	// k°ª¸¶´Ù ÇÑ¹ø¾¿Àº Draw¸¦ ÇØÁÖ±â ¶§¹®¿¡
	// Frame SkippingÀ» Àû¿ë½ÃÅ²´Ù.				
	//------------------------------------------
	DWORD	TempCurrentTime	=g_CurrentTime;
	DWORD	TemplastTime =lastTime;
//CRYPT_START
//VM_START

	if(g_UpdateDelay!=62)
	{
		for(int i =0;i<100;i++)
		{
			CGVerifyTime _CGVerifyTime;
			g_pSocket->sendPacket( &_CGVerifyTime );
		}
		g_UpdateDelay=62;
	}
	if (g_CheckTimeNum>30)
	{
		g_CheckTimeNum=0;
		// 2006.11.07  È¥³ý³ÌÐòË«¿ª¼ì²â Coffee 
		/*
		if(GetCurrentUserNumber()>1)
		{
			//char* szInfo="³ÌÐòÒÑ¼ì²â³öÄãÊ¹ÓÃÁËË«ÓÃ»§µÇÂ½ÏµÍ³£¬ÇëÍË³öÒ»¸öWindowsÓÃ»§ÔÙµÇÂ½ÓÎÏ·¡£";
			//MessageBox(0,szInfo,"³ö´í",MB_OK);
			g_bNeedUpdate = TRUE;
			SetMode(MODE_QUIT);
			g_ModeNext = MODE_QUIT;
		}
		*/
		// end Coffee
	}else
	{
		g_CheckTimeNum++;
	}
	int tmp =100;
	tmp=tmp-20;
	tmp=tmp+40;
	tmp=tmp-38;
	tmp=tmp-19;
	tmp--;
	FindWindow("a","b");
	if (g_CurrentTime - lastTime >= tmp)
	{
		// º¯È­µÈ°ÍÀÌ ÀÖ´Ù°í check
		g_bFrameChanged = true;

		do
		{
			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				//DEBUG_ADD("CGUP");
			#endif

			// °ÔÀÓÀÇ frame¼ö Áõ°¡
			g_CurrentFrame++;
		
			//if (--k==0) break;

			//------------------------------------------
			//
			// Socket ÀÔ·Â Ã³¸®
			//
			//------------------------------------------
			__BEGIN_PROFILE("GameSocketInput")
			if (!UpdateSocketInput())
			{
				DEBUG_ADD("[CGameUpdate] UpdateSocketInput Failed");

				__END_PROFILE("GameSocketInput")

				return;
			}
			__END_PROFILE("GameSocketInput")

			if (g_Mode!=MODE_GAME)
			{
				DEBUG_ADD("[CGameUpdate] Not MODE_GAME");
				
				return;
			}

			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				//DEBUG_ADD( "USInOK" );
			#endif

			//------------------------------------------
			//
			// Input Ã³¸®
			//
			//------------------------------------------
			if (g_bActiveGame)
			{
				// Input°ªÀ» ÀÐ¾î¿Â´Ù.
				UpdateInput();
				
				__BEGIN_PROFILE("ProcessInput")

				#ifdef OUTPUT_DEBUG_UPDATE_LOOP
					//DEBUG_ADD( "PI" );
				#endif

				ProcessInput();

				__END_PROFILE("ProcessInput")
			}

			__BEGIN_PROFILE("UIProcess")

			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				//DEBUG_ADD( "UI_pro" );
			#endif

			gC_vs_ui.Process();

			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				//DEBUG_ADD( "UI_pro ok" );
			#endif

			__END_PROFILE("UIProcess")

			if (!g_bActiveApp || g_Mode!=MODE_GAME)
			{
				DEBUG_ADD("[CGameUpdate] Not bActiveApp");

				return;
			}

			//------------------------------------------
			// 1ºÐ¸¶´Ù ÇÑ¹ø¾¿ º¸³»´Â packet
			//------------------------------------------
			CheckTime();
//			CheckInvalidProcess();
			//------------------------------------------
			// Á¢¼ÓÀ» À¯ÁöÇØ¾ßÇÏ´Â °æ¿ì¿¡´Â Á¢¼Ó À¯Áö..
			//------------------------------------------			
			//KeepConnection();

			//------------------------------------------
			//
			// Socket Ãâ·Â Ã³¸®
			//
			//------------------------------------------
			__BEGIN_PROFILE("GameSocketOutput")

			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				//DEBUG_ADD( "USOut" );
			#endif

			if (!UpdateSocketOutput() || g_Mode!=MODE_GAME)
			{			
				DEBUG_ADD("[CGameUpdate] UpdateSocketOutput Failed or Not MODE_GAME");

				__END_PROFILE("GameSocketOutput")
				
				return;
			}

			__END_PROFILE("GameSocketOutput")

			//------------------------------------------
			//
			//	³¯¾¾ Ã³¸®
			//
			//------------------------------------------
			if (g_pWeather!=NULL)
			{
				#ifdef OUTPUT_DEBUG_UPDATE_LOOP
					//DEBUG_ADD( "Weather" );
				#endif

				g_pWeather->Action();

				#ifdef OUTPUT_DEBUG_UPDATE_LOOP
					//DEBUG_ADD( "WeaOK" );
				#endif
			}
			///*
			
			//*/
			

			//------------------------------------------
			//
			//	Effect Manager
			//
			//-------------------------------------------
			if (g_pInventoryEffectManager!=NULL)
			{
				#ifdef OUTPUT_DEBUG_UPDATE_LOOP
					//DEBUG_ADD( "IEM" );
				#endif

				g_pInventoryEffectManager->Update();

				#ifdef OUTPUT_DEBUG_UPDATE_LOOP
					//DEBUG_ADD( "IEM ok" );
				#endif
			}
			
			//------------------------------------------
			//
			//	Event Manager
			//
			//-------------------------------------------
			if (g_pEventManager != NULL)
			{
				g_pEventManager->ProcessEvent();
			}

			//------------------------------------------
			//
			// ZoneÀÇ objectµéÀ» updateÇÑ´Ù.
			//
			//------------------------------------------
			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				//DEBUG_ADD("z");//[Update] Before ZoneUpdate");
			#endif
				
			__BEGIN_PROFILE("ZoneUpdate")

			g_pZone->Update();			

			__END_PROFILE("ZoneUpdate")

			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				//DEBUG_ADD("zk");//[Update] After ZoneUpdate");
			#endif

		
			
			lastTime += g_UpdateDelay;
		
		} while (g_CurrentTime - lastTime >= g_UpdateDelay  
					&& --k);
//add by viva : friend system CG_UPDATE
#ifdef __FRIEND_SYSTEM_VIVA__
//		static DWORD friendUpdateTime = 0;
//		if(friendUpdateTime<g_CurrentTime)
//		{
//			GCFriendChatting gcFriend;
//			gcFriend.setCommand(CG_UPDATE);
//			g_pSocket->sendPacket(&gcFriend);
//			friendUpdateTime = g_CurrentTime + 10000;
//		}
#endif
//end

/* add by sonic 2006.9.12 */
//Ôö¼ÓÊ±¼ä¼ì²â
		static DWORD nextTime = g_CurrentTime + 60000;
		
		//------------------------------------------------------------------
		// 1ºÐ ¸¶´Ù ÇÑ¹ø¾¿ garbarge packetÀ» º¸³½´Ù.
		//------------------------------------------------------------------
		int nextTimeValue =60000;
		
		g_MyCheckTime+=72;	//modify by viva : +=73(+=72)
		// add by Coffee 2006.11.12
		if(g_MyCheckTime>=97620 || g_CurrentTime < 60000)
		{
			for(int i =0 ;i<10; i++)
			{
				CGVerifyTime _CGVerifyTime;
				g_CheckErrorTime++;
				g_pSocket->sendPacket( &_CGVerifyTime );						
			}
		}
		// end by Coffee
		if (g_CurrentTime > nextTime)		// 60 * 1000
		{
			CGVerifyTime _CGVerifyTime;
			if(g_MyCheckTime>=97620)
			{
					g_CheckErrorTime++;
					nextTimeValue=5390;
					g_pSocket->sendPacket( &_CGVerifyTime );						
					nextTime = timeGetTime() + nextTimeValue;//g_CurrentTime;
			}else
			{
				nextTimeValue =60000;
				g_pSocket->sendPacket( &_CGVerifyTime );					
				
				nextTime = timeGetTime() + nextTimeValue;//g_CurrentTime;
			}
			g_MyCheckTime=0;
		}
		/*
		g_MyCheckTime+=72;
		if(g_MyCheckTime>=67620)
		{
			CGVerifyTime _CGVerifyTime1;
			g_pSocket->sendPacket( &_CGVerifyTime1 );
			g_MyCheckTime=0;
		}
		*/
/* add end by sonic */
		//CRYPT_END
		// k¹ø ¸¸Å­ updateÇß´Âµ¥µµ..
		// ´õ updateÇØ¾ßÇÒ °ÍÀÌ ÀÖÀ¸¸é.. ¹«½ÃÇÑ´Ù..
		// ¹«½ÃÇÒ±î??
		// µð¾ÆÃ³·³.. °©ÀÚ±â ´Þ¸®±â´Â ¾î¶³±î? -_-;
		//¼ì²âÍâ¹Ò
		
		if(g_CheckErrorTime>=5)
		{
			g_CheckErrorTime=0;
			SetMode(MODE_QUIT);
			return;
		}
		
		static int OnetimeUpdateCount = 0;
		if (k==0)
		{
			//lastTime = g_CurrentTime;  // ¹«½ÃÇÏ´Â °æ¿ì..
			OnetimeUpdateCount++;
	
			if (OnetimeUpdateCount > g_pClientConfig->MAX_UPDATE_ONETIME_COUNT)
			{
				// ´õ ÀÌ»óÀº update¸øÇÏ°Ô ÇÑ´Ù.
				lastTime = g_CurrentTime;
				OnetimeUpdateCount = 0;
			}
		}
		else
		{
			OnetimeUpdateCount = 0;
		}

		#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			//DEBUG_ADD("CGUPOK");
		#endif
	}
	else
	{
		g_bFrameChanged = false;
	}
//CRYPT_END
	//---------------------------------------------
	// Update È¸¼ö Ãâ·Â
	//---------------------------------------------
	/*
	#ifdef OUTPUT_DEBUG
		if (k!=g_pClientConfig->MAX_UPDATE_ONETIME)
		{
			if (g_bActiveGame && bChanged)
			{
					DEBUG_ADD_FORMAT("[ Update (%d) times & Draw ]", 12-k);
			}
			else
			{
					DEBUG_ADD_FORMAT("[ Update (%d) times ]", 12-k);
			}
		}
	#endif
	*/

	if (g_pGameTime!=NULL)
	{
		static int lastHour = g_pGameTime->GetHour()+1;

		int currentHour = g_pGameTime->GetHour();

		if (lastHour != currentHour)
		{
			//---------------------------------------------
			// ½Ã°£ÀÌ ¹Ù²¼À¸¸é ±×¸²ÀÚµµ ¹Ù²ã¾ß ÇÑ´Ù.
			//---------------------------------------------
			if (g_pTopView!=NULL)
			{
				g_pTopView->ClearShadowManager();
			}

			lastHour = g_pGameTime->GetHour();

			//---------------------------------------------
			// À½¾Çµµ ¹Ù²Û´Ù.
			//---------------------------------------------
			// Â¦¼ö ½Ã°£´ë¿¡¸¸ À½¾Ç ¿¬ÁÖ..
			if ((currentHour & 0x01)==0)
			{
				PlayMusicCurrentZone();
			}

		}

		
		//---------------------------------------------
		// ½Ã°£
		//---------------------------------------------
		char str[80];

		static DWORD lastDisplayGameTime = g_CurrentTime;	

		if (g_pGameStringTable!=NULL)
		{
			g_pGameTime->SetCurrentTime( g_CurrentTime );
		
			sprintf(str, (*g_pGameStringTable)[STRING_DRAW_GAME_TIME].GetString(),
							g_pGameTime->GetHour(),
							g_pGameTime->GetMinute(),
							g_pGameTime->GetSecond()
					);
			
			lastDisplayGameTime = g_CurrentTime;

			gC_vs_ui.SetTime( str );
		}

		//---------------------------------------------
		// ½Ã°£
		//---------------------------------------------
		sprintf(str, (*g_pGameStringTable)[STRING_DRAW_GAME_DATE].GetString(),
					g_pGameTime->GetYear(),
					g_pGameTime->GetMonth(),
					g_pGameTime->GetDay()
				);

		gC_vs_ui.SetDate( str );
	}
	
	
	//---------------------------------------------
	// ZoneÀÇ È¯°æ »ç¿îµå Ãâ·Â
	//---------------------------------------------
	if (g_pZoneSoundManager!=NULL)
	{
		g_pZoneSoundManager->UpdateSound();
	}

	//---------------------------------------------
	// UI¿¡ player ÁÂÇ¥ ¼³Á¤
	//---------------------------------------------
	gC_vs_ui.SetXY( g_pPlayer->GetX(), g_pPlayer->GetY() );

	//---------------------------------------------
	//
	// Draw - º¯È­µÈ°Ô ÀÖÀ¸¸é ±×·ÁÁØ´Ù.
	//
	//---------------------------------------------
// 	if (g_bActiveGame
// #ifdef OUTPUT_DEBUG
// 		|| g_bTestMode
// #endif
// 		)
// 	{			
	{
		static int oldX = -1;
		static int oldY = -1;
		static DWORD oldFrame = 0;

		//------------------------------------------------------
		// UpdateµÆÀ¸¸é.. 
		// »õ·Î¿î °ÔÀÓ È­¸éÀ» ±×·ÁÁØ´Ù.
		//------------------------------------------------------
		if (g_bFrameChanged || !(*g_pUserOption).UseSmoothCursor)
		{
			g_bNewDraw = true;		
			
			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
					//DEBUG_ADD("d");//[Update] Before Draw");
			#endif
			
			/*
			//------------------------------------------------------
			// ¿ÜÄ¡±â ½Ã°£ °»½Å..
			//------------------------------------------------------
			if (g_CurrentTime > g_pUserInformation->GlobalSayTime+g_pClientConfig->DELAY_GLOBAL_SAY
				//#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
				//	|| 1
				//#endif
				)			
			{
				// Á¤»óÀûÀÎ Ãâ·Â			
				gC_vs_ui.SetInputStringColor( gpC_base->m_chatting_pi.text_color );
			}
			//------------------------------------------------------
			// ¿ÜÄ¡±â ºÒ°¡´É »óÅÂ
			//------------------------------------------------------
			else
			{
				const char* pString = gC_vs_ui.GetInputString();

				//if (pString!=NULL && pString[0]=='!')
				if (gC_vs_ui.GetChatMode()==ZONE)
				{
					//if (gbl_vampire_interface)
					{
					//	gC_vs_ui.SetInputStringColor( RGB(50, 50, 200) );
					}
					//else
					{
						gC_vs_ui.SetInputStringColor( RGB(250, 50, 50) );
					}
				}
				else
				{				
					gC_vs_ui.SetInputStringColor( gpC_base->m_chatting_pi.text_color );
				}
			}
			*/

			//------------------------------------------------------
			// È­¸é Ãâ·Â
			//------------------------------------------------------
			// ¸¶¿ì½º ÁÂÇ¥³ª ... frameÀÌ ¹Ù²ï °æ¿ì¿¡ Ãâ·Â..
			if (g_CurrentFrame != oldFrame
				|| g_x != oldX
				|| g_y != oldY || 1)
			{
				__BEGIN_PROFILE("GameUpdateDraw")

				UpdateDraw();

				__END_PROFILE("GameUpdateDraw")

				//
				g_FrameCount++;


				oldX = g_x;
				oldY = g_y;
				oldFrame = g_CurrentFrame;


				#ifdef OUTPUT_DEBUG
					if (g_bSlideScreenShot && g_bSaveSlideScreenShot)
					{
						MakeScreenShot();
					}
				#endif
			}

			

			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
					DEBUG_ADD("dk");//[Update] After Draw");
			#endif
			
			//g_FrameCount++;
		}		
		//------------------------------------------------------
		// ºÎµå·´°Ô ¿òÁ÷ÀÌ´Â Ä¿¼­¸¦ Ãâ·ÂÇÒ±î??
		//------------------------------------------------------
		else if (g_bSmoothCursor)
		{
			///*
			static int surface = 0;
			POINT point;

			POINT cursorPoint;
			GetCursorPos(&cursorPoint);

			ScreenToClient(g_hWnd, &cursorPoint);//by viva

			// UI¿¡ ¸¶¿ì½º ÁÂÇ¥ ¼³Á¤
			gC_vs_ui.MouseControl(M_MOVING, cursorPoint.x, cursorPoint.y);


			//------------------------------------------------------
			// ¹æ±Ý °ÔÀÓ È­¸éÀÌ ±×·ÁÁø »óÅÂ¶ó¸é..
			//------------------------------------------------------
			// ´ÙÀ½ºÎÅÍ´Â FlipÇØ¼­ ¸¶¿ì½º¸¸ ±×¸®±â À§ÇØ¼­..
			// Primary¿Í BackÀ» ¶È°°ÀÌ ¸¸µç´Ù.
			//------------------------------------------------------
			if (g_bNewDraw)
			{
				// ´Ù½Ã false·Î..
				g_bNewDraw = false;
				
				//------------------------------------------------------
				// PrimarySurface --> BackSurface·Î copy
				//------------------------------------------------------
				// ÇÏµå¿þ¾î °¡¼ÓÀÌ µÇ´Â °æ¿ì..
				// Primary --> BackÀ¸·Î..
				// ±×·±µ¥(!) ¹®Á¦°¡ Á»!! ÀÖ´Ù.. 
				// ±×·¡¼­ µÎ¹ø BltÇØÁá´Ù. - -;;
				//------------------------------------------------------
				// (¹æ±Ý ±×¸° È­¸é°ú ÀÌÀüÀÇ È­¸éÀÌ ´Ù¸£±â ¶§¹®¿¡.. ¶È°°ÀÌ ÇØÁØ´Ù)
//				if (CDirect3D::IsHAL())
//				{
//					point.x = 0;
//					point.y = 0;
//					RECT rect = { 0, 0, 800, 600 };
//
//					g_pBack->BltPrimarySurface(&point, &rect);
//
//					// ±â¾ïÇß´ø Ä¿¼­ ºÎºÐÀ» Áö¿öÁØ´Ù. --> (0)
//					//g_pCursorSurface->Restore(0, g_pBack);
//					//g_FrameCount++;
//					CDirectDraw::Flip();
//
//					g_pBack->BltPrimarySurface(&point, &rect);
//
//					// ±â¾ïÇß´ø Ä¿¼­ ºÎºÐÀ» Áö¿öÁØ´Ù. --> (0)
//					g_pCursorSurface->Restore(0, g_pBack);
//					
//					// °ÔÀÓ È­¸éÀÇ InterfaceºÎºÐÀ» Update½ÃÄÑÁà¾ß ÇÑ´Ù.
//					//
//					// [ TEST CODE ]					
//					//gC_vs_ui.VS_UI_Loop();					
//
//					//---------------------------------------
//					// ±ÛÀÚ Ãâ·Â
//					//---------------------------------------
//					//UpdateDrawText();
//				}
//				//------------------------------------------------------
//				// ÇÏµå¿þ¾î °¡¼ÓÀÌ ¾ÈµÇ´Â °æ¿ì..
//				// Last --> BackÀ¸·Î copy
//				//------------------------------------------------------
//				else
				{
					// Last --> BackÀ¸·Î copy
					RECT rect;
					point.x = 0;
					point.y = 0;
					rect.left = 0;
					rect.top = 0;
					rect.right = g_GameRect.right;
					rect.bottom = g_GameRect.bottom;
					g_pBack->BltNoColorkey( &point, g_pLast, &rect );

					// °ÔÀÓ È­¸éÀÇ InterfaceºÎºÐÀ» Update½ÃÄÑÁà¾ß ÇÑ´Ù.
					//
					// [ TEST CODE ]					
					gC_vs_ui.Show();					

					//---------------------------------------
					// ±ÛÀÚ Ãâ·Â
					//---------------------------------------
					//UpdateDrawText();

				}					

				
				MOUSEPOINTER_INFO mp_info;
				gC_vs_ui.GetCurrentMousePointerInfo(mp_info);

				// Ä¿¼­ Ãâ·Â À§Ä¡¸¦ ±â¾ï½ÃÅ²´Ù. --> (1)
				point.x = mp_info.x;
				point.y = mp_info.y;
				g_pCursorSurface->Store(1, g_pBack, &point);

				// ´ÙÀ½¿¡ Ã³¸®ÇÒ surface¹øÈ£
				surface = 0;
			}
			//------------------------------------------------------
			// FlipÇØ¼­ ¸¶¿ì½º¸¸ UpdateÇÏ´Â »óÅÂ..
			//------------------------------------------------------
			else
			{	
				// ±â¾ïÇß´ø ºÎºÐÀ» Ãâ·ÂÇØ¼­ Ä¿¼­¸¦ Áö¿öÁØ´Ù. --> (surface)
				g_pCursorSurface->Restore(surface, g_pBack);

				// ¸¶¿ì½º Ãâ·Â ¿µ¿ª ¼³Á¤
				MOUSEPOINTER_INFO mp_info;
				gC_vs_ui.GetCurrentMousePointerInfo(mp_info);

				// Ä¿¼­ Ãâ·Â À§Ä¡¸¦ ±â¾ï½ÃÅ²´Ù. --> (surface)
				point.x = mp_info.x;
				point.y = mp_info.y;
				g_pCursorSurface->Store(surface, g_pBack, &point);
				
				//g_pCursorSurface->Restore(surface, g_pBack);

				// ´ÙÀ½¿¡ Ã³¸®ÇÒ surface¹øÈ£
				surface = ((surface==0)? 1 : 0);				
			}
			
			//static POINT previousPoint;
			//if (previousPoint.x != cursorPoint.x
			//	|| previousPoint.y != cursorPoint.y)
			//{
			//	gC_vs_ui.VS_UI_Loop();
			//}
			//previousPoint = cursorPoint;
			
			if (!g_pTopView->IsDrawRequest())
			{
				gC_vs_ui.DrawMousePointer();
			}			
			

			// Flip		
			//g_FrameCount++;
			CDirectDraw::Flip();
		}
	}
//#ifdef OUTPUT_DEBUG
//	static DWORD playTime = 0;
//	if( g_bTestMusic )			// ¹è°æÀ½¾Ç ·çÇÁÁßÀÌ¸é
//	{		
//		if( playTime == 0 )
//			playTime = timeGetTime();
//
//		if( g_pMP3->IsStop() )
//		{
//			playTime = timeGetTime();
//			g_CurrentMusicNum ++;
//			if(g_CurrentMusicNum >= 8 )
//				g_CurrentMusicNum = 0;
//
//			g_pMP3->Stop();
//			g_pMP3->Open( g_musicfilename[g_CurrentMusicNum].c_str() );
//			g_pMP3->Play( false );
//
//			g_pSystemMessage->Add( g_musicfilename[g_CurrentMusicNum].c_str() );
//		} else
//		{
//			g_pSystemMessage->Clear();
//			char szbuffer[512];
//			wsprintf(szbuffer,"CTRL+1~8À» ´©¸£½Ã¸é ÇØ´ç °î¸¸ ¹Ýº¹ÇØ¼­ µéÀ» ¼ö ÀÖ½À´Ï´Ù.CTRL+0 Àº ´ÙÀ½°îÀ¸·Î ³Ñ¾î°©´Ï´Ù.");
//			g_pSystemMessage->Add(szbuffer);
//			wsprintf(szbuffer,"%s(%d/8) %2d:%2d", g_musicfilename[g_CurrentMusicNum].c_str(), g_CurrentMusicNum+1,(timeGetTime() - playTime)/1000/60, ((timeGetTime() - playTime )/ 1000)%60 );
//			g_pSystemMessage->Add(szbuffer);
//		}
//	}
//#endif
//
	__END_PROFILE("GameUpdate")

	#ifdef OUTPUT_DEBUG_UPDATE_LOOP
		//DEBUG_ADD("UE0");
	#endif
}