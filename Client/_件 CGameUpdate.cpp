//-----------------------------------------------------------------------------
// CGameUpdate.cpp
//-----------------------------------------------------------------------------
// 실제 게임을 진행하는 부분
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

// 할할...
extern void	SetWatchMode( bool );
extern bool g_bWatchMode;

extern void	CheckTime();
extern int g_MyCheckTime=0;
extern int g_CheckErrorTime=0;
extern BOOL g_MyFull;
extern RECT g_GameRect;
//add by sonic 2006.7.30
//쇱꿎溝固되쩍돨痰빵
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

// update loop에 대한 debug message를 출력할까 말까?
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

	// mouse event 처리
	g_pDXInput->SetMouseEventReceiver( DXMouseEvent );
	
	// keyboard event 처리
	g_pDXInput->SetKeyboardEventReceiver( DXKeyboardEvent );
}

//-----------------------------------------------------------------------------
// DXMouseEvent
//-----------------------------------------------------------------------------
void		
CGameUpdate::DXMouseEvent(CDirectInput::E_MOUSE_EVENT event, int x, int y, int z)
{
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
				// Minimap에 클릭하면 그쪽으로 이동한다.
				//---------------------------------------------------------
				/* // 2001.7.14 주석처리
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
				// Chatting창에 extra input
				//---------------------------------------------------------
				gC_vs_ui.ChatMouseControlExtra( M_LEFTBUTTON_DOWN, g_x, g_y );
				*/
				#endif
			break;

			case CDirectInput::RIGHTUP :
				// 지뢰 만드는 거 중지
				gC_vs_ui.EndInstallMineProgress();
				//gC_vs_ui.EndCreateMineProgress();
				//gC_vs_ui.EndCreateBombProgress();
			break;

			//case CDirectInput::RIGHTDOWN :
				//---------------------------------------------------------
				// Chatting창에 extra input
				//---------------------------------------------------------
				// 2001.7.14 주석처리
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
		if(key == 0xcc) return;		// 이건 머지-_-?;;
				
//		gC_vs_ui.DIKeyboardControl(event, key);
	
		switch (key)
		{	
			case DIK_SPACE :
				if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL) )
				{

				}
			break;
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
					(*g_pUserOption).DrawFPS = !(*g_pUserOption).DrawFPS;
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
							// 5초 동안 좌우 잔상
							g_pPlayer->AddEffectStatus( EFFECTSTATUS_FADE_OUT, 5*1000 );
						}
					}
				#endif
			break;							
		
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
					}
				break;

				#if defined(_DEBUG) || defined(OUTPUT_DEBUG)
					case DIK_F1 :
					{
						// info 다시 읽기 
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
					// 기술 사용 - packet 테스트를 위해서
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
									MZone::CREATURE_MAP::const_iterator iCreature2;

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

										gcSkillToTileOK2.execute( g_pSocket );
										
										DEBUG_ADD("TileOK2OKOK");
									}									
								}

								iCreature++;
							}
						}
					}
					break;

					case DIK_1 :
						if(g_pDXInput->KeyDown(DIK_LCONTROL))
						{ 							
							{
								UI_PopupMessage( UI_STRING_MESSAGE_CAMPAIGN_HELP_THANKS );
							}
						}
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
						{							
							#ifdef __METROTECH_TEST__
								g_iAutoHealPotion = (g_iAutoHealPotion+1)%2;
							#endif
						}
					break;

					case DIK_2 :
						if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
						{  
							if(UI_IsRunning_WebBrowser())
								UI_Close_WebBrowser();
							else
							//add by zdj
					 			UI_Run_WebBrowser(/*"http://market.darkeden.com/"*/"");
						}
						// ALT + 2  --> 공격 속도 normal
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
						{							
							#ifdef __METROTECH_TEST__
								g_bAutoManaPotion = !g_bAutoManaPotion;
							#endif
						}
					break;

					case DIK_3 :
						if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
						{
	#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
							MItem* pItem = MItem::NewItem( ITEM_CLASS_SUB_INVENTORY );
							pItem->SetID( rand()%10000 );
							pItem->SetItemType( 0 );
							gC_vs_ui.RunSubInventory( pItem );
	#endif						
						}
						// ALT + 3  --> 공격 속도 Fast
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
						{							
							#ifdef __METROTECH_TEST__
								g_iAutoReload = (g_iAutoReload+1)%5;
							#endif
						}
					break;

					case DIK_4 :
						if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
						{
							int TempCount = ShowCursor(FALSE);
						}
#ifdef __METROTECH_TEST__
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
							g_bLight = !g_bLight;
#endif
						break;
					case DIK_5 :
#ifdef __METROTECH_TEST__
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
						UI_RunHorn(g_pZone->GetID());
#endif						
						break;

					case DIK_6 :
#ifdef __METROTECH_TEST__
						if (g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU))
						UI_RunMinigame(0);
#endif
						break;
						
					case DIK_7 :
						break;

					case DIK_8 :
#ifdef __METROTECH_TEST__
						if (g_pDXInput->KeyDown(DIK_RCONTROL) && g_pDXInput->KeyDown(DIK_RSHIFT))// &&
						{
							g_iSpeed *= -1;
						}
#endif
						break;
						
					case DIK_9 :
#ifdef __METROTECH_TEST__
						if (g_pDXInput->KeyDown(DIK_RCONTROL) && g_pDXInput->KeyDown(DIK_RSHIFT))// &&

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
									// PC Talk Box의 정보 설정
									//---------------------------------------------------
									// SetContent라고 이름이 되어있지만.. Subject이다. - -;
									g_pPCTalkBox->SetContent( "하하하" );
									g_pPCTalkBox->SetNPCID( 297 );
									g_pPCTalkBox->SetCreatureType( 297 );
									g_pPCTalkBox->SetScriptID( scriptID );
									
									//---------------------------------------------------
									// 각 string 추가
									//---------------------------------------------------
									int contentSize = 2;//g_pNPCScriptTable->GetContentSize( scriptID );
									
									//for (int i=0; i<contentSize; i++)
									{
										// g_PCTalkBox에 추가
										g_pPCTalkBox->AddString( "우헤헤" );
										g_pPCTalkBox->AddString( "크하하" );
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
				//------------------------------------------------------------
				//
				// 음악 볼륨 조절
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
							// Player, NPC가 아닌 경우
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
					}
				}
				break;

				//------------------------------------------------------------
				//
				// 컴퓨터 띄우기
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
				// 캐릭터 추가 - packet 테스트를 위해서
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
							// Vampire 추가
							//------------------------------------------------------------
							if (rand()%2)
							{
								pCreature = new MCreatureWear;

								pCreature->SetZone( g_pZone );
								
								//--------------------------------------------------
								// CreatureType 설정
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

								// 임시로
								pCreature->SetGuildNumber( 2 );

								pCreature->SetGroundCreature();
								pCreature->SetID( objectID );
								//pCreature->SetAction(ACTION_MOVE);
								
								pCreature->SetPosition( x, y );
								pCreature->SetServerPosition( x, y );
								pCreature->SetDirection( dir );
								pCreature->SetCurrentDirection( dir );
								pCreature->SetAction( ACTION_STAND );

								// 색깔
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
							// Slayer 추가
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

								// 피부색
								pCreatureWear->SetBodyColor2( rand()%MAX_COLORSET );

								pCreatureWear->SetStatus( MODIFY_MAX_HP, 80 );
								pCreatureWear->SetStatus( MODIFY_CURRENT_HP, 100 );

								// 이름
								pCreatureWear->SetName( name );

								// 임시로 NPC복장을 입힌다.
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
				// 방향 바꾸기
				//-------------------------------------------------------------
				case DIK_LEFT :
				case DIK_RIGHT :
				{
					MZone::CREATURE_MAP::const_iterator iCreature = g_pZone->GetCreatureBegin();

					for (int i=0; i<g_pZone->GetCreatureNumber(); i++)
					{
						MCreature* pCreature = (*iCreature).second;

						//--------------------------------------------------
						// Player, NPC가 아닌 경우
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
				// HP 회복 test
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
				// zone 이동 자동..
				//
				//------------------------------------------------------------
				case DIK_LBRACKET : case DIK_RBRACKET :

				break;						

				case DIK_A :
					if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
					{
						//------------------------------------------------------
						// 사용자
						//------------------------------------------------------
						MCreature* pCreature = g_pZone->GetCreature( 1000 );

						if (pCreature!=NULL)
						{
							//------------------------------------------------------
							// TileOK2로 인한 결과 
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
							// TileOK2로 인한 결과 추가
							//------------------------------------------------------
							//Duration_t	m_Duration;
							pCreature->PacketSpecialActionToSector(
												MAGIC_LIGHT, 
												g_pPlayer->GetX(), g_pPlayer->GetY(),
												pResult						// 결과
							);

							// 방향을 바라보기
							pCreature->SetDirectionToPosition(g_pPlayer->GetX(), g_pPlayer->GetY());
					
						}
					}
				break;

			#endif			
		}
	}
	else
	{
	}
}



//-----------------------------------------------------------------------------
// ProcessInput RButton Down
//-----------------------------------------------------------------------------
// pObject와 관련..이 있을 수도 있고..
//-----------------------------------------------------------------------------
void
ProcessInputRButtonDown(MObject* pObject, bool bForceAttack = false)
{
	g_bRButtonDown = TRUE;
	
	// 선택된 sector가 없게 한다.
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
	// 오토바이를 타고 있는 경우 
	//
	//---------------------------------------------------------------
	// 오토바이에서 내린다.
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
	// Burrow 된 경우 땅으로 솟아나온다는 packet을 보낸다.
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
	// object/tile에 기술을 사용하는 경우 
	//
	//---------------------------------------------------------------
	else
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("TileSkill");
		#endif

		// sector 선택하기
		//g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);
		//g_pTopView->SetSelectedSector(g_SelectSector.x, g_SelectSector.y);

		//
		//MObject*	pObject = g_pTopView->GetSelectedObject(g_x, g_y);
	
		//--------------------------------------------------
		// 선택한 곳에 Object가 없으면 ..
		//--------------------------------------------------
		if (pObject == NULL)
		{		
			g_pPlayer->UnSetRepeatAction();
			//g_pPlayer->TraceNextNULL();

			//--------------------------------------------------
			// 자신한테 특수 기술 사용
			//--------------------------------------------------
			if (g_pPlayer->SelfSpecialAction())
			{
				g_pPlayer->SetRepeatAction();
			}
			//--------------------------------------------------
			// 자신한테 사용하는 특수 기술이 아닌 경우
			//--------------------------------------------------				
			else if (!g_pPlayer->IsSpecialActionInfoTargetSelf())
			{
				g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);

				//--------------------------------------------------				
				// Zone에 특수 기술을 사용한다.
				//--------------------------------------------------				
				if (g_pPlayer->TraceSectorToSpecialAction( g_SelectSector.x, g_SelectSector.y ))
				{
					// 선택된 Sector로 표시한다.
					g_pTopView->SetSelectedSector( g_SelectSector );
				
					g_pPlayer->SetRepeatAction();

					g_bPreviousMove = false;
				}
			}

			//----------------------------------
			// Tile이나 자신에게 하는 행동 반복 설정
			//----------------------------------						
			g_pTopView->SetSelectedSectorNULL();
			
		}		
		//--------------------------------------------------
		// 선택된 Object에 대해서 
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
					// 목표가 되는 Creature
					MCreature* pCreature = ((MCreature*)pObject);
					
					//--------------------------------------------------
					// 다른 애덜에게 사용하는건가?
					//--------------------------------------------------
					if (g_pPlayer->TraceCreatureToSpecialAction( pCreature->GetID(), bForceAttack ))
					{
						g_pPlayer->SetRepeatAction();								

						g_bPreviousMove = false;
					}
					else if (!g_pPlayer->IsSpecialActionInfoTargetOther())
					{
						//--------------------------------------------------
						// (!) 다른 Creature에 사용하는게 아닐 경우
						//--------------------------------------------------
						// 자신한테 특수 기술 사용
						if (g_pPlayer->SelfSpecialAction())
						{
							g_pPlayer->SetRepeatAction();
						}
						else
						{	
							//--------------------------------------------------
							// 자신에게 사용하는게 아닌 경우
							//--------------------------------------------------
							// 선택됐던 Creature의 좌표의 ..
							// Zone에 특수 기술을 사용할까?
							if (!g_pPlayer->IsSpecialActionInfoTargetSelf()
								&& g_pPlayer->TraceSectorToSpecialAction( pCreature->GetX(), pCreature->GetY()))
							{
								g_pPlayer->SetRepeatAction();

								g_bPreviousMove = false;
							}
						}									
					}
					
					//----------------------------------
					// 선택 sector표시 없애기
					//----------------------------------
					g_pTopView->SetSelectedSectorNULL();
				}
				break;

				//--------------------------------------------------
				// Item
				//--------------------------------------------------
				// Item일 경우는.. 자신한테 사용하거나 Zone에 사용한다.
				case MObject::TYPE_ITEM :		
				{
					//--------------------------------------------------
					// 자신한테 특수 기술 사용
					//--------------------------------------------------
					if (g_pPlayer->SelfSpecialAction())
					{
						g_pPlayer->SetRepeatAction();
					}
					else
					{	
						//--------------------------------------------------
						// 자신한테 사용하는게 아닌 경우..
						//--------------------------------------------------
						// 목표가 되는 Creature
						if (!g_pPlayer->IsSpecialActionInfoTargetSelf())
						{
							MItem* pItem = ((MItem*)pObject);

							if (pItem!=NULL)
							{
								// 와일드 울프를 시체한테 썼을때 예외 처리
								if(g_pPlayer->GetSpecialActionInfo() == SKILL_WILD_WOLF)
								{
									g_pPlayer->UseWildWolf_Corpse(pItem);
								}
								// 선택됐던 Creature의 좌표의 ..
								// Zone에 특수 기술을 사용한다.
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
	// packet 이동 테스트
	//-----------------------------------------------
	#ifdef OUTPUT_DEBUG
		if (g_pDXInput->KeyDown(DIK_M) 
			&& g_pDXInput->KeyDown(DIK_LMENU))
		{			
			PacketTest();
		}
	#endif
	

	//---------------------------------------------------	
	// LButton Up
	//---------------------------------------------------	
	if (g_bLButtonDown
		// L이 Up이거나.. R이 Down인 경우
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

		// 주위의 누군가를 계속 공격하는 mode를 해제한다.
		g_pPlayer->UnSetLockMode();
	
		if (g_pPlayer->IsRepeatAction())
		{
			// 버튼을 떼었으므로 행동 반복을 취소한다.
			g_pPlayer->UnSetRepeatAction();
			//g_pPlayer->TraceNextNULL();
		}
		else 
		{		
			//---------------------------------------------------	
			// item을 들고 있지 않은 경우
			//---------------------------------------------------	
			// 선택된 sector로 정한다.
			POINT point;
			g_pPlayer->GetNextDestination( point );		
			if (point.x==SECTORPOSITION_NULL || point.y==SECTORPOSITION_NULL)
			{
				// 현재 가고 있는 곳이 없으면
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
		// R이 Up이거나. L이 Down인 경우
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
				// 버튼을 떼었으므로 행동 반복을 취소한다.
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

	#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("UIMM");
	#endif
	gC_vs_ui.MouseControl(M_MOVING, cursorPoint.x, cursorPoint.y);

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

		if (gC_vs_ui.MouseControl(M_LEFTBUTTON_DOWN, g_x, g_y))
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
	
	#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("CINL");
	#endif

	//---------------------------------------------------	
	// ItemName List 출력?
	//---------------------------------------------------	
	if ((g_pDXInput->KeyDown(DIK_LMENU) || g_pDXInput->KeyDown(DIK_RMENU) || g_pDXInput->KeyDown(DIK_RCONTROL) || g_pDXInput->KeyDown(DIK_LCONTROL))
		&& !g_pDXInput->KeyDown(DIK_LSHIFT)
		&& g_pTradeManager==NULL && g_pPlayer->GetWaitVerify()!=MPlayer::WAIT_VERIFY_TRADE)
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("DINL");
		#endif

		g_pTopView->SetDrawItemNameList();

		if (!g_bWatchMode 
			// 뭔가 반복행동을 하고 있지 않은 경우
			&& !g_pPlayer->IsRepeatAction()
			// 박쥐나 늑대가 아닌 경우
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
				else if (g_pDXInput->KeyDown(DIK_RCONTROL))		//add by viva for test
				{
					g_pTopView->SetRequestMode( MTopView::REQUEST_INFO );
				}
				///add by viva
				else if(g_pDXInput->KeyDown(DIK_LCONTROL))
				{
					gpC_mouse_pointer->SetCursorAddFriend();
				//	g_pPlayer->SetRepeatAction();
				}

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
	//
	//		입력이 제한되는 경우..
	//
	//---------------------------------------------------	
	// - 게임 못하게 하는 dialog가 떠 있는 경우
	//
	// - Player가 server로부터 검증을 기다리는 상태
	//
	// - 반복 action중에는 딴 입력은 필요없다.(무시)
	//
	// - UI에서 입력을 받고 있는 경우
	//
	//---------------------------------------------------	
	if (g_pPlayer->IsWaitVerify() 
		|| gC_vs_ui.IsInstallMineProgress()	// 지뢰 만드는 중이면..
		//|| gC_vs_ui.IsCreateMineProgress()	// 지뢰 만드는 중이면..
		//|| gC_vs_ui.IsCreateBombProgress()	// 지뢰 만드는 중이면..
		|| g_pPlayer->IsFastMove()	// 빨리 움직이는 경우
		// 2004, 12, 3, 석민씨 추가
//		|| g_pPlayer->CurPernalShop() == 1  || gC_vs_ui.inventory_mode == 2
//		|| gC_vs_ui.IsRunningPersnalShop()
		// 2004, 12, 3, 석민씨 추가
		|| UI_IsRunning_WebBrowser()
		)	
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD_FORMAT("[ProcessInput] Is WaitVerify Return. %d %d", g_pPlayer->IsWaitVerify(), g_pPlayer->IsFastMove());
		#endif

		return;
	}

	// 현재 셀렉트중인 캐릭터에 대한 동작이 있으면 리턴
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
	// UI에서 mouse커서 입력을 잡고 있는 경우
	// elevator 작동중..
	//---------------------------------------------------	
	if ((g_pUIDialog->IsLockInput() || g_bUIInput)
		
		// 파티 떠있을때는 파티창의 캐릭을 선택할 수도 있다.
		&& !bRunningParty

		// 다른 동작 못하도록 ...
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
	// Player가 죽은 경우... 
	//
	//---------------------------------------------------		
	if (g_pPlayer->IsDead())
	{
		g_pTopView->SetSelectedNULL();
		g_pTopView->SetSelectedSectorNULL();

		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("[ProcessInput] Player Dead return.");
		#endif

		return;
	}

	#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("FA");
	#endif

	//---------------------------------------------------	
	//
	// 현재 mouse의 위치에 있는 object에 대해서 check
	//
	//---------------------------------------------------	
	// 강제공격이거나 SmallZone에 있거나
	// Hallucination에 걸렸으면 다~ 선택
	//---------------------------------------------------	
	

	// 강제 공격이 설정되는 경우
	bool bForceAttack = (g_pDXInput->KeyDown(DIK_LSHIFT) 
						|| g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION));
						
						

	if (bForceAttack
		//|| !g_bZonePlayerInLarge
		|| g_bZoneSafe
		|| g_pPlayer->IsInSafeSector()
		|| g_pPlayer->IsSlayer() && g_pPlayer->GetSpecialActionInfo() != ACTIONINFO_NULL && (*g_pActionInfoTable)[g_pPlayer->GetSpecialActionInfo()].GetUser() == FLAG_ACTIONINFO_USER_SLAYER
		|| g_pPlayer->IsVampire() && g_pPlayer->GetSpecialActionInfo() != ACTIONINFO_NULL && (*g_pActionInfoTable)[g_pPlayer->GetSpecialActionInfo()].GetUser() == FLAG_ACTIONINFO_USER_VAMPIRE
		|| g_pPlayer->IsOusters() && g_pPlayer->GetSpecialActionInfo() != ACTIONINFO_NULL && (*g_pActionInfoTable)[g_pPlayer->GetSpecialActionInfo()].GetUser() == FLAG_ACTIONINFO_USER_OUSTERS
		// 2004, 11, 26, sobeit add start - 슬레 140 인챈 스킬 - 슬레에겐 축복, 나머진 저주..ㅋㅋ 
		|| g_pPlayer->GetSpecialActionInfo() == SKILL_INTIMATE_GRAIL
		// 2004, 11, 26, sobeit add end
		)
	{
		g_pObjectSelector->SelectAll();				
	}
	//---------------------------------------------------	
	// L-Control누르면 우리편만 선택
	//---------------------------------------------------	
	else if (g_pDXInput->KeyDown(DIK_LCONTROL)
		|| g_pTopView->IsRequestMode())
	{
		g_pObjectSelector->SelectFriend();		
	}
	//---------------------------------------------------	
	// 아니면.. 적만 선택
	//---------------------------------------------------	
	else
	{
		// Notice Event 체크해서 ...
		// 성을 차지하기 위한 전쟁중이라면!!
		// 1. 기존에 수비하는 길드일 경우 자기 길드 아니면 다 적!
		// 2. 공격 신청을 한 길드일경우 우리 길드 아니면 다 적!
		// 3. 길드간 전쟁 신청 외에 다른사람일경우 그 두 길드원 빼고 다 같은 팀!
		
		g_pObjectSelector->SelectEnemy();		
	}
	//---------------------------------------------------	
	// Slayer인 경우는 종족에 따라서 선택
	//---------------------------------------------------	
	if (g_pPlayer->IsSlayer() || g_pPlayer->IsOusters() )
	{
		g_pObjectSelector->SelectByRace();
	}
	//---------------------------------------------------	
	// Vampire인 경우는 Guild에 따라서 선택
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
	// 파티UI를 선택하는 경우..
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

			pObject = g_pZone->GetCreature( g_pZone->GetCreatureID( pInfo->Name.GetString(), 1 ) );

			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("SPMk");
			#endif
		}
	}
	//---------------------------------------------------	
	// 존에서 선택..
	//---------------------------------------------------	
	else
	{
		#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("selZ");
		#endif

		//---------------------------------------------------
		// 아예 딴 UI에 focus 가 있으면 걍 return해야 한다.
		// inventory에서 아이템 사용했는데 기술 나갈 수가 있어서.
		//---------------------------------------------------
		if (partyMember==-2		// party창에 focus 된게 아니고
			&& g_bUIInput)		// UI에 입력이 있다면..
		{
			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("selZk1");
			#endif

			return;
		}

		//---------------------------------------------------	
		// ItemName을 선택하는 경우면..
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
			// ItemName이 선택 안됐으면 Object선택
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
		// 그냥 Object 선택
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
	// Lock Mode 체크
	//---------------------------------------------------	
	if (g_pDXInput->KeyDown(DIK_CAPITAL) 
		&& !g_pPlayer->IsInCasket()
		&& !g_pPlayer->IsUndergroundCreature())
	{
		g_pPlayer->SetLockMode();

		// 무조건 공격 커서로 바뀐다.
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

		// Lock Mode에서는 선택된 sector를 없앤다.
		g_pTopView->SetSelectedSectorNULL();
	}
	else
	{
		// 아무것도 누르지 않은 상태에서 
		// LockMode이면 LockMode 해제해야 한다.
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
	// Lock Mode가 아닌 경우만 
	// 일반적인 입력을 받아들여 행동을 취한다.
	//---------------------------------------------------	
	if (!g_pPlayer->IsLockMode())
	{	
#ifdef OUTPUT_DEBUG_PROCESS_INPUT
		DEBUG_ADD("notLock");
#endif
		
		//---------------------------------------------------	
		// Mouse 위치에 Object가 없으면..
		//---------------------------------------------------	
		if (pObject==NULL)
		{
#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD("noObj");
#endif
			
			// 선택된 것 없게 한다.
			g_pTopView->SetSelectedNULL();
			
			//---------------------------------------------------	
			// mouse pointer 설정
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
						// 갈 수 없는 곳이면
						//---------------------------------------------------
						if (g_pPlayer->IsGroundCreature() && sector.IsBlockGround()
							|| g_pPlayer->IsUndergroundCreature() && sector.IsBlockUnderground()
							|| g_pPlayer->IsFlyingCreature() && sector.IsBlockFlying())
							//g_pZone->CanMove(g_pPlayer->GetMoveType(), g_MouseSector.x, g_MouseSector.y))
						{	
							// 포탈이 아니고
							if (!g_bMouseInPortal
								// UI에 입력이 없는 경우 --> 게임에 커서가 있는 경우
								&& !g_bUIInput
								// 선택된 캐릭터가 없는 경우
								&& g_pTopView->GetSelectedCreature()==OBJECTID_NULL)
							{
								// 갈 수 없는 커서로 표시
								gpC_mouse_pointer->SetCursorNotMove();					
							}
						}
						//---------------------------------------------------
						// 갈 수 있는 곳이면..
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
		// Mouse 위치에 Object가 있는 경우
		//
		//---------------------------------------------------	
		else
		{
			BYTE objectType = pObject->GetObjectType();
			
#ifdef OUTPUT_DEBUG_PROCESS_INPUT
			DEBUG_ADD_FORMAT("obj(%d)", objectType);
#endif
			
			//------------------------------------------------
			// Creature 위에 mouse가 있는 경우
			//------------------------------------------------
			if (objectType==MObject::TYPE_CREATURE)////////add by viva for test
			{
				MCreature* pCreature = (MCreature*)pObject;

				g_pTopView->SetSelectedCreature( pCreature->GetID() );
					
				if (!g_bMouseInPortal 
						
					// 종족에 따라서 공격할 수 있거나
					// 강제 공격이거나..
					//&& (g_pPlayer->CanAttackTribe( pCreature ) || bForceAttack)
					&& g_pObjectSelector->CanAttack( pCreature ) )
					// 뱀파이어인 경우에는 Guild에 따라서 공격할 수 있거나..	
						
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
				if(g_pDXInput->KeyDown(DIK_LCONTROL) && g_pDXInput->m_lb_down)	//add by viva
				{
					gpC_base->SendMessage(UI_FRIEND_CHATTING_ADD_FRIEND, 0, 0, (void*)pCreature->GetName());
				}
			
					
			}
			
			//------------------------------------------------
			// Item 위에 mouse가 있는 경우
			//------------------------------------------------
			else if (pObject->GetObjectType()==MObject::TYPE_ITEM)
			{
				MItem* pItem = (MItem*)pObject;
				MCorpse* pCorpse = (MCorpse*) pItem;
				MCreature* pCreature = pCorpse->GetCreature();
				
				if(g_pZone != NULL && pItem != NULL )
				{
					// 슬레이어면 안되게
					// 뱀파이어면 creature 찾아서 성물인경우만

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
						// option에 따른 색깔
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
						// add by Sonic 2006.10.28 藤속鞫刻힛橄昑陋구槨븐��
						else if(pItem->GetItemOptionListCount() > 2)
						{
							color = g_pClientConfig->COLOR_NAME_VAMPIRE; //Red
						}
						// end by Sonic 2006.10.28 藤속鞫刻힛橄昑陋구槨븐��
						else
						{
							color = g_pClientConfig->COLOR_NAME_ITEM_OPTION;
						}
						
						//------------------------------------------------
						// item집는 커서
						//------------------------------------------------
						if (!g_bMouseInPortal&&!g_pPlayer->IsInDarkness())
						{						
							if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
							{
								gpC_mouse_pointer->SetCursorPickUp( "", color );
							}
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
			// Effect 위에 mouse가 있는 경우
			//------------------------------------------------
			else if (pObject->GetObjectType()==MObject::TYPE_EFFECT)
			{
				g_pTopView->SetSelectedEffect( pObject->GetID() );
			}
		}


		//---------------------------------------------------	
		//
		// player의 기술 사용 delay 시간이 지나야 공격 가능하다.
		//
		//---------------------------------------------------	
		if (g_pPlayer->IsNotDelay() && !g_pPlayer->HasEffectStatus( EFFECTSTATUS_ETERNITY_PAUSE ) )
		{
			#ifdef OUTPUT_DEBUG_PROCESS_INPUT
				DEBUG_ADD("noDel");
			#endif

			//-----------------------------------------------
			// Shift + L/R ButtonDown : 강제 Tile 공격
			//-----------------------------------------------
			if (g_pDXInput->KeyDown(DIK_LSHIFT))
			{
				#ifdef OUTPUT_DEBUG_PROCESS_INPUT
					DEBUG_ADD("shiAt");
				#endif
				//-----------------------------------------------
				// Shift + LButtonDown : 강제 공격
				//-----------------------------------------------
				if (g_pDXInput->m_lb_down
					// 숨어 있는 경우가 아닐때
					&& !g_pPlayer->IsInCasket()
					&& !g_pPlayer->IsUndergroundCreature()
					&& !g_pPlayer->HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
				{
					g_pPlayer->UnSetRequestMode();

					//-----------------------------------------------
					// 캐릭터를 강제 공격..
					//-----------------------------------------------
					if (pObject!=NULL)
					{
						if (pObject->GetObjectType()==MObject::TYPE_CREATURE)
						{
							// Object가 뭔가 선택됐을 경우에는 
							// LButton을 누르고 있는 것만으로 이동이 되지않게 한다.
							MCreature *pCreature = dynamic_cast<MCreature*>(pObject);
							g_bLButtonDown = TRUE;
							
							BOOL	ForceAttack = TRUE;

							if (g_pPlayer->TraceCreatureToBasicAction(
										pCreature->GetID(), 
										true))		// 강제 공격
							{
								g_pPlayer->SetRepeatAction();
								g_bPreviousMove = false;
							}
							// 목적지 표시를 없앤다.
							g_pTopView->SetSelectedSectorNULL();
						}
					}		

					#ifdef OUTPUT_DEBUG_PROCESS_INPUT
						DEBUG_ADD("shiKK");
					#endif
				}		
				//-----------------------------------------------
				// Shift + RButtonDown : 강제 Tile 기술 공격
				//-----------------------------------------------
				else if (g_pDXInput->m_rb_down)
				{
					g_pPlayer->UnSetRequestMode();

					ProcessInputRButtonDown(pObject, true);

					g_bPreviousMove = false;
					/*
					g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);

					// 선택된 Sector로 표시한다.
					g_pTopView->SetSelectedSector( g_SelectSector );

					if (g_pPlayer->TraceSectorToSpecialAction( g_SelectSector.x, g_SelectSector.y ))
					{
						//g_pPlayer->SetRepeatAction();
					}


					// 목적지 표시를 없앤다.
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
					// burrow 상태가 아니어야 한다.
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
					
					// 선택된 sector가 없게 한다.
					g_pTopView->SetSelectedSectorNULL();

					// sector 선택하기
					g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);
					//g_pTopView->SetSelectedSector(g_SelectSector.x, g_SelectSector.y);

					//
					//MObject*	pObject = g_pTopView->GetSelectedObject(g_x, g_y);

					// 인스톨 터렛일때.. 잡다한 lbuttton 처리 않는다.. lbutton = 무조건 터렛 공격
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
//								// 선택된 Sector로 표시한다.
//								g_pTopView->SetSelectedSector( g_SelectSector );
//							
//								g_pPlayer->SetRepeatAction();
//							}
//
//							g_bPreviousMove = false;
//								char szTemp[128];
//								sprintf(szTemp, "방향이 같다: %d,방향 ", TempDir);
//								g_pSystemMessage->Add(szTemp);
						}
						else
						{
							if(g_pPlayer->IsStop())
								g_pPlayer->SetCurrentDirection(TempDir);
						}
					}
					//--------------------------------------------------
					// 선택한 곳에 Object가 없으면 MOVE
					//--------------------------------------------------
					else if (pObject == NULL)
					{		
						// l-shift나 l-control이 눌려있지 않은 상태에서만 이동.
						if (//!g_pDXInput->KeyDown(DIK_LSHIFT) &&
							!g_pDXInput->KeyDown(DIK_LCONTROL))
						{		
							if (g_pPlayer->IsNotDelay() && !g_pPlayer->HasEffectStatus(EFFECTSTATUS_ETERNITY_PAUSE ) 
								// 2004, 9, 14, sobeit add start - 총슬 130 skill 관련
								//&&  !g_pPlayer->HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET ) 
								// 2004, 9, 14, sobeit add end - 총슬 130 skill 관련
								)
							{
								if (g_pPlayer->SetMovePosition(g_SelectSector.x, g_SelectSector.y))
								{
									//if (g_pPlayer->IsStop())
									{
										// 다음 목표위치로 설정한다
										g_pPlayer->TraceNULL();							
										
										g_pPlayer->SetNextActionToMove();
										
										g_bPreviousMove = true;
									}
								}
							}
						}
					}
					//--------------------------------------------------
					// 선택된 Object에 대해서 
					//--------------------------------------------------
					else 
					{	
						// Object가 뭔가 선택됐을 경우에는 
						// LButton을 누르고 있는 것만으로 이동이 되지않게 한다.
						//g_bLButtonDown = FALSE;

						switch (pObject->GetObjectType())
						{			
							case MObject::TYPE_CREATURE :	
								{
									// 2004, 12, 3, 석민씨 추가   //by viva for test
									MCreature* TempCreature = (MCreature*)pObject;
									if(TempCreature->CurPernalShop() == 1 )
//										|| g_pPlayer->IsFlyingCreature() || g_pPlayer->IsUndergroundCreature() 
//										|| g_pPlayer->IsGroundCreature() || g_pPlayer->IsFakeCreature() 
//										|| g_pPlayer->GetMoveDevice()== 1|| g_pPlayer->GetMoveDevice()== 2)
									{
										gpC_base->SendMessage(UI_REQUEST_STORE_INFO, pObject->GetID(),0);
										gC_vs_ui.SetOtherObjectID(pObject->GetID());
									}
									// 2004, 12, 3, 석민씨 추가

									if (g_pPlayer->TraceCreatureToBasicAction( pObject->GetID(), 
																				bForceAttack, true ))
									{
										//----------------------------------
										// 남에게 하는 기본 행동 반복 설정
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
				// L/R Button을 누르지는 않았지만,
				// 이전에 누른 LButton이 계속 눌려진 상태라면..
				// But(!), L-Shift가 눌러지지 않아야 한다.
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
					// 선택한 곳에 Object가 없고
					// 방금전에 이동하고 있던 중이면 MOVE
					//--------------------------------------------------
					if (pObject == NULL && g_bPreviousMove)
					{	
						if (g_pPlayer->IsNotDelay() && !g_pPlayer->IsRepeatAction())
						{
							g_SelectSector = g_pTopView->GetSelectedSector(g_x, g_y);

							// 다음 목표위치로 설정한다
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
			// 버튼을 떼었으므로 행동 반복을 취소한다.
			g_pPlayer->UnSetRepeatAction();
			//g_pPlayer->TraceNextNULL();

			g_bPreviousMove = false;
		}
		else
		{	
			// 선택된 sector로 정한다.
			POINT point;
			
			// 다음 갈 곳이 없으면
			g_pPlayer->GetNextDestination( point );		
			if (point.x==SECTORPOSITION_NULL || point.y==SECTORPOSITION_NULL)
			{
				// 현재 가고 있는 곳이 없으면
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
			// 버튼을 떼었으므로 행동 반복을 취소한다.
			g_pPlayer->UnSetRepeatAction();
			//g_pPlayer->TraceNextNULL();
		}
	}

	if(g_pDXInput->m_cb_up && g_bCButtonDown)
	{
		g_bCButtonDown = FALSE;
	}

	// 가만히 서 있는 상태이면 목표 위치를 없앤다.
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
	// Missile 종류 바꾸기
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
		// 비~~
		//---------------------------------------------------
		if (g_pDXInput->KeyDown(DIK_8))
		{
			SetWeather( WEATHER_RAINY, rand()%10+10);
		}

		//---------------------------------------------------
		// 눈~~
		//---------------------------------------------------
		if (g_pDXInput->KeyDown(DIK_9))
		{
			SetWeather( WEATHER_SNOWY, rand()%10+10);
		}

		//---------------------------------------------------
		// 날씨 멈춤
		//---------------------------------------------------
		if (g_pDXInput->KeyDown(DIK_0))
		{
			SetWeather( WEATHER_CLEAR, 0 );
		}



		//---------------------------------------------------
		// 시야  + / -
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
		// 밝기 감소
		//---------------------------------------------------
		if (g_pDXInput->KeyDown(DIK_F9))
		{
			BYTE DarkBits = g_pTopView->GetDarkBits()-1;
			g_pTopView->SetDarkBits(DarkBits);
		}
		
		//---------------------------------------------------
		// 밝기 증가
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
		// Zone이동 test
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

							BYTE effectSpriteType = rand()%(*g_pEffectSpriteTypeTable).GetSize();
										
							TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[effectSpriteType].FrameID;
							BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[effectSpriteType].BltType;
							
							// BLT_NORMAL
							int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);
							
							// Sound ... Test *_*;;
							//g_Sound.Play(g_sb1);



							MLinearEffect*	pEffect;
							//pEffect = new MParabolaEffect(BLT_EFFECT);
							pEffect = new MLinearEffect(BLT_EFFECT);

							pEffect->SetFrameID(frameID, maxFrame);		// 0번 Effect, Max 8 Frame

							// 발사 위치 Pixel좌표
							pEffect->SetPixelPosition(playerPoint.x, playerPoint.y, 0);	
							
							// 목표 위치 Pixel좌표
							pEffect->SetTarget(playerPoint.x + -i*300, 
												playerPoint.y + -j*350,
												0,
												20);	// step

							// 지속되는 Frame (목표가 있다면 별로 관계 없음 - -;)
							pEffect->SetCount(1000);							

							//pEffect->SetLight(1);							

							g_pZone->AddEffect( pEffect );
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
	

	char	str[128];



	//-----------------------------------------------------------------		
	// 마우스 좌표 다시 설정
	//-----------------------------------------------------------------
	GetCursorPos(&point);
		
	// ui에 mouse좌표 설정
	gC_vs_ui.MouseControl(M_MOVING, point.x, point.y);

	//-----------------------------------------------------------------
	// 탑뷰 Zone 출력
	//-----------------------------------------------------------------
	{
		//-----------------------------------------------------------------
		// Font 출력하는 Surface를 바꿔줘야 한다.
		//-----------------------------------------------------------------
		//g_SetFL2Surface( g_pLast->GetSurface() );

		//-----------------------------------------------------------------
		// Game 화면 출력
		//-----------------------------------------------------------------
		__BEGIN_PROFILE("GameDraw2D")


		const MEvent *event = g_pEventManager->GetEventByFlag(EVENTFLAG_SHAKE_SCREEN);
		if(event != NULL && event->IsShowTime() == true)
		{		
			static int count = 0;
			const int maxStep = 13;

			// 2004, 5, 21 sobeit modify start
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
			g_pTopView->Draw(0,0);			

		__END_PROFILE("GameDraw2D")

		//-----------------------------------------------------------------
		// UI 출력
		//-----------------------------------------------------------------		
		__BEGIN_PROFILE("UIDraw2D")

		gC_vs_ui.Show();

		__END_PROFILE("UIDraw2D")

		if (!g_pTopView->IsDrawRequest())
		{
			gC_vs_ui.DrawMousePointer();
		}

		
		__BEGIN_PROFILE("DrawFPS")
			if (outputInfo || g_pUserOption->DrawFPS)
			{
				//-----------------------------------------------------------------
				// FPS 찍기	
				//-----------------------------------------------------------------				
				sprintf(str, "%d FPS", g_FrameRate);	
				
				g_pLast->GDI_Text(11,11, str, RGB(20,20,20));
				g_pLast->GDI_Text(10,10, str, 0xFFFFFF);
			}
		__END_PROFILE("DrawFPS")

		//-----------------------------------------------------------------
		// Last를 Back으로 copy - 3D HAL이 아닌 경우만..
		//-----------------------------------------------------------------
		__BEGIN_PROFILE("LastToBack")

		point.x = 0;
		point.y = 0;
		rect.left = 0;
		rect.top = 0;
		rect.right = g_GameRect.right;
		rect.bottom = g_GameRect.bottom;
		g_pBack->BltNoColorkey( &point, g_pLast, &rect );		

		__END_PROFILE("LastToBack")
	}
	
	#ifdef OUTPUT_DEBUG_UPDATE_LOOP
			//DEBUG_ADD("ddk");//[Update-Draw] After Draw");
	#endif

	//-----------------------------------------------------------------
	// 죽은 상태..
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

			// 죽는 동작이 끝난 경우에 화면을 검게...
			g_pTopView->SetFadeStart(blackValue, 0, 0, 5,5,5);
		

			// 6 frame씩 보여준다.
			if ((a & 0x01)==0)
			{				
				// 6초가 지나고나서 부활버튼을 띄운다.
				if (second < 4
					&& g_pTempInformation->GetMode()==TempInformation::MODE_NULL)
				{
					// FixMe Please oh shit by ssuk
					if( (g_pWarManager->IsExist( g_pZone->GetID() ) || g_pZone->GetID() == 71 || g_pZone->GetID() == 72 || g_pZone->GetID() == 73) && gC_vs_ui.IsRunningBloodBibleStatus() )
						gC_vs_ui.RequestShrineMinimap( g_pClientConfig->DELAY_PLAYER_RESURRECT_TIMEOUT );

					gC_vs_ui.RequestDie( g_pClientConfig->DELAY_PLAYER_RESURRECT_TIMEOUT );

					g_pTempInformation->SetMode(TempInformation::MODE_WAIT_RESURRECT);
					bool bResurrect = false, bElixir= false, bEternity= false, IsSiegeAttacker= false ;

					// 2005, 1, 18, sobeit add start - 퀘스트 action으로 죽은 경우엔 아래를 체크 안한다.
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
									// 2004, 11, 11, sobeit add start - 레벨 검사도 한다.. 요거..g_char_slot_ingame.DOMAIN_HEAL는 뭐지? -_-; 버그 수정..ㅡㅡ;
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
	// Mouse Cursor 위치의 image를 기억시켜두기
	//-----------------------------------------------------------------
	//
	// FullScreen이고...
	if (g_pUserOption->UseSmoothCursor)
	{
		if (CDirectDraw::IsFullscreen() && g_FrameRate > LIMIT_FPS_FOR_CURSOR)
		{
			GetCursorPos(&point);	
			
			// ui에 mouse좌표 설정
			gC_vs_ui.MouseControl(M_MOVING, point.x, point.y);

			// 저장할 영역 설정
			MOUSEPOINTER_INFO mp_info;
			gC_vs_ui.GetCurrentMousePointerInfo(mp_info);

			g_pCursorSurface->Init(2, mp_info.width, mp_info.height);
			
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

	if (CDirect3D::IsHAL())
	{
		//-----------------------------------------------------------------
		// Mouse 그리기
		//-----------------------------------------------------------------		
		if (!g_pTopView->IsDrawRequest())
		{
			gC_vs_ui.DrawMousePointer();
		}		
	}

	//-----------------------------------------------------------------
	// Debug Information출력
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
		
			if (g_bPutMessage && g_pDebugMessage!=NULL)
			{
				int strY = 565;
				
				for (int c=g_pDebugMessage->GetSize()-1; c>=0; c--)
				{
					if ((*g_pDebugMessage)[c][0] != NULL)
					{						
						const COLORREF debugColor = RGB(28<<3, 28<<3, 28<<3);				
						pPrintInfo->text_color	= debugColor;
			
						g_Print(10, strY, (*g_pDebugMessage)[c], pPrintInfo);
						strY-=15;
					}
				}

				// Debug Log Filename 출력하기
				if (g_pDebugMessage->GetFilename()!=NULL)
				{
					sprintf(str, "LogFile : %s", g_pDebugMessage->GetFilename());
					const COLORREF txColor = RGB(31<<3, 11<<3, 11<<3);										
					pPrintInfo->text_color	= txColor;
			
					g_Print(10, 580, str, pPrintInfo);
				}				
			}	
			//*/
			
			if (g_pLoadingThread!=NULL && g_pLoadingThread->IsWorking())
			{		
				sprintf(str, "<<< Loading >>>");

				const COLORREF txColor = RGB(31<<3, 31<<3, 31<<3);				

				pPrintInfo->text_color	= txColor;
				
				g_Print(300, 5, str, pPrintInfo);
			}
			
			// Missile종류
			///*	 
			if (g_pPlayer->GetSpecialActionInfo() != ACTIONINFO_NULL)
			{
				sprintf(str, "[%d] %s", g_pPlayer->GetSpecialActionInfo(), (*g_pActionInfoTable)[g_pPlayer->GetSpecialActionInfo()].GetName());
				
				const COLORREF bkColor = RGB(0, 0, 0);
				const COLORREF txColor = RGB(16<<3, 31<<3, 16<<3);				

				pPrintInfo->text_color	= bkColor;
				g_Print(500, 10, str, pPrintInfo);

				pPrintInfo->text_color	= txColor;
				g_Print(501, 11, str, pPrintInfo);				
			}	
			// 가진 돈
			sprintf(str, "Money : %d", (*g_pMoneyManager).GetMoney());
						
			const COLORREF bkMoneyColor = RGB(0, 0, 0);
			const COLORREF txMoneyColor = RGB(16<<3, 31<<3, 16<<3);				

			pPrintInfo->text_color	= bkMoneyColor;
			g_Print(500, 30, str, pPrintInfo);

			pPrintInfo->text_color	= txMoneyColor;
			g_Print(501, 31, str, pPrintInfo);

			sprintf(str, "QuestColorset = %d", g_pClientConfig->QuestItemColorSet);
			pPrintInfo->text_color	= txMoneyColor;
			g_Print(501, 51, str, pPrintInfo);

			if (!CDirect3D::IsHAL())
			{
				g_SetFL2Surface( g_pLast->GetSurface() );
			}
		}
	#endif

	__END_PROFILE("DrawDebugInfo")

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

	// 5초마다 한번씩.. scroll
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
///鹿苟角쇱꿎溝固角뤠唐되쩍랗몸痰빵
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
// 게임 실행 중...
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
	// Logout 할 시간인치 체크한다.
	//------------------------------------------
	if (g_pUserInformation!=NULL
		&& g_pUserInformation->LogoutTime!=0
		&& g_CurrentTime > g_pUserInformation->LogoutTime
		&& g_pPlayer->GetWaitVerify() == MPlayer::WAIT_VERIFY_LOGOUT)
	{
		ExecuteLogout();

		g_pUserInformation->LogoutTime = 0;	// Logout시간 제거

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
	// Sound관련
	//---------------------------------------------
	// 매 loop 마다 안해줘도 되지 않을까..
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
		// 초당 play한 sound 수..
		g_SoundPerSecond = 0;

		// 1초 후
		nextSoundCheckTime = g_CurrentTime + 1000;
	}

	//---------------------------------------------------
	//
	// 일정 시간마다 한번씩 처리를 해준다.
	//
	//---------------------------------------------------
	// [예제]
	// g_UpdateDelay	: 100
	// lastTime			: 1000 
	// currentTime		: 1240 
	//
	// 1200, 1100의 두번을 처리하게 된다.
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
	// 무한루프 예방도 되고...
	// k값마다 한번씩은 Draw를 해주기 때문에
	// Frame Skipping을 적용시킨다.				
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
		// 2006.11.07  혼뇜넋埼崗역쇱꿎 Coffee 
		/*
		if(GetCurrentUserNumber()>1)
		{
			//char* szInfo="넋埼綠쇱꿎놔콱賈痰죄崗痰빵되쩍溝固，헝藁놔寧몸Windows痰빵疼되쩍踏狗。";
			//MessageBox(0,szInfo,"놔댄",MB_OK);
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
		// 변화된것이 있다고 check
		g_bFrameChanged = true;

		do
		{
			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
				//DEBUG_ADD("CGUP");
			#endif

			// 게임의 frame수 증가
			g_CurrentFrame++;
		
			//if (--k==0) break;

			//------------------------------------------
			//
			// Socket 입력 처리
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
			// Input 처리
			//
			//------------------------------------------
			if (g_bActiveGame)
			{
				// Input값을 읽어온다.
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
			// 1분마다 한번씩 보내는 packet
			//------------------------------------------
			CheckTime();
//			CheckInvalidProcess();
			//------------------------------------------
			// 접속을 유지해야하는 경우에는 접속 유지..
			//------------------------------------------			
			//KeepConnection();

			//------------------------------------------
			//
			// Socket 출력 처리
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
			//	날씨 처리
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
			// Zone의 object들을 update한다.
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
/* add by sonic 2006.9.12 */
//藤속珂쇌쇱꿎
		static DWORD nextTime = g_CurrentTime + 60000;
		
		//------------------------------------------------------------------
		// 1분 마다 한번씩 garbarge packet을 보낸다.
		//------------------------------------------------------------------
		int nextTimeValue =60000;
		g_MyCheckTime+=72;
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
		// k번 만큼 update했는데도..
		// 더 update해야할 것이 있으면.. 무시한다..
		// 무시할까??
		// 디아처럼.. 갑자기 달리기는 어떨까? -_-;
		//쇱꿎棍밈
		
		if(g_CheckErrorTime>=5)
		{
			g_CheckErrorTime=0;
			SetMode(MODE_QUIT);
			return;
		}
		
		static int OnetimeUpdateCount = 0;
		if (k==0)
		{
			//lastTime = g_CurrentTime;  // 무시하는 경우..
			OnetimeUpdateCount++;
	
			if (OnetimeUpdateCount > g_pClientConfig->MAX_UPDATE_ONETIME_COUNT)
			{
				// 더 이상은 update못하게 한다.
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
	// Update 회수 출력
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
			// 시간이 바꼈으면 그림자도 바꿔야 한다.
			//---------------------------------------------
			if (g_pTopView!=NULL)
			{
				g_pTopView->ClearShadowManager();
			}

			lastHour = g_pGameTime->GetHour();

			//---------------------------------------------
			// 음악도 바꾼다.
			//---------------------------------------------
			// 짝수 시간대에만 음악 연주..
			if ((currentHour & 0x01)==0)
			{
				PlayMusicCurrentZone();
			}

		}

		
		//---------------------------------------------
		// 시간
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
		// 시간
		//---------------------------------------------
		sprintf(str, (*g_pGameStringTable)[STRING_DRAW_GAME_DATE].GetString(),
					g_pGameTime->GetYear(),
					g_pGameTime->GetMonth(),
					g_pGameTime->GetDay()
				);

		gC_vs_ui.SetDate( str );
	}
	
	
	//---------------------------------------------
	// Zone의 환경 사운드 출력
	//---------------------------------------------
	if (g_pZoneSoundManager!=NULL)
	{
		g_pZoneSoundManager->UpdateSound();
	}

	//---------------------------------------------
	// UI에 player 좌표 설정
	//---------------------------------------------
	gC_vs_ui.SetXY( g_pPlayer->GetX(), g_pPlayer->GetY() );

	//---------------------------------------------
	//
	// Draw - 변화된게 있으면 그려준다.
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
		// Update됐으면.. 
		// 새로운 게임 화면을 그려준다.
		//------------------------------------------------------
		if (g_bFrameChanged || !(*g_pUserOption).UseSmoothCursor)
		{
			g_bNewDraw = true;		
			
			#ifdef OUTPUT_DEBUG_UPDATE_LOOP
					//DEBUG_ADD("d");//[Update] Before Draw");
			#endif
			
			/*
			//------------------------------------------------------
			// 외치기 시간 갱신..
			//------------------------------------------------------
			if (g_CurrentTime > g_pUserInformation->GlobalSayTime+g_pClientConfig->DELAY_GLOBAL_SAY
				//#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
				//	|| 1
				//#endif
				)			
			{
				// 정상적인 출력			
				gC_vs_ui.SetInputStringColor( gpC_base->m_chatting_pi.text_color );
			}
			//------------------------------------------------------
			// 외치기 불가능 상태
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
			// 화면 출력
			//------------------------------------------------------
			// 마우스 좌표나 ... frame이 바뀐 경우에 출력..
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
		// 부드럽게 움직이는 커서를 출력할까??
		//------------------------------------------------------
		else if (g_bSmoothCursor)
		{
			///*
			static int surface = 0;
			POINT point;

			POINT cursorPoint;
			GetCursorPos(&cursorPoint);

			// UI에 마우스 좌표 설정
			gC_vs_ui.MouseControl(M_MOVING, cursorPoint.x, cursorPoint.y);


			//------------------------------------------------------
			// 방금 게임 화면이 그려진 상태라면..
			//------------------------------------------------------
			// 다음부터는 Flip해서 마우스만 그리기 위해서..
			// Primary와 Back을 똑같이 만든다.
			//------------------------------------------------------
			if (g_bNewDraw)
			{
				// 다시 false로..
				g_bNewDraw = false;
				
				//------------------------------------------------------
				// PrimarySurface --> BackSurface로 copy
				//------------------------------------------------------
				// 하드웨어 가속이 되는 경우..
				// Primary --> Back으로..
				// 그런데(!) 문제가 좀!! 있다.. 
				// 그래서 두번 Blt해줬다. - -;;
				//------------------------------------------------------
				// (방금 그린 화면과 이전의 화면이 다르기 때문에.. 똑같이 해준다)
//				if (CDirect3D::IsHAL())
//				{
//					point.x = 0;
//					point.y = 0;
//					RECT rect = { 0, 0, 800, 600 };
//
//					g_pBack->BltPrimarySurface(&point, &rect);
//
//					// 기억했던 커서 부분을 지워준다. --> (0)
//					//g_pCursorSurface->Restore(0, g_pBack);
//					//g_FrameCount++;
//					CDirectDraw::Flip();
//
//					g_pBack->BltPrimarySurface(&point, &rect);
//
//					// 기억했던 커서 부분을 지워준다. --> (0)
//					g_pCursorSurface->Restore(0, g_pBack);
//					
//					// 게임 화면의 Interface부분을 Update시켜줘야 한다.
//					//
//					// [ TEST CODE ]					
//					//gC_vs_ui.VS_UI_Loop();					
//
//					//---------------------------------------
//					// 글자 출력
//					//---------------------------------------
//					//UpdateDrawText();
//				}
//				//------------------------------------------------------
//				// 하드웨어 가속이 안되는 경우..
//				// Last --> Back으로 copy
//				//------------------------------------------------------
//				else
				{
					// Last --> Back으로 copy
					RECT rect;
					point.x = 0;
					point.y = 0;
					rect.left = 0;
					rect.top = 0;
					rect.right = g_GameRect.right;
					rect.bottom = g_GameRect.bottom;
					g_pBack->BltNoColorkey( &point, g_pLast, &rect );

					// 게임 화면의 Interface부분을 Update시켜줘야 한다.
					//
					// [ TEST CODE ]					
					gC_vs_ui.Show();					

					//---------------------------------------
					// 글자 출력
					//---------------------------------------
					//UpdateDrawText();

				}					

				
				MOUSEPOINTER_INFO mp_info;
				gC_vs_ui.GetCurrentMousePointerInfo(mp_info);

				// 커서 출력 위치를 기억시킨다. --> (1)
				point.x = mp_info.x;
				point.y = mp_info.y;
				g_pCursorSurface->Store(1, g_pBack, &point);

				// 다음에 처리할 surface번호
				surface = 0;
			}
			//------------------------------------------------------
			// Flip해서 마우스만 Update하는 상태..
			//------------------------------------------------------
			else
			{	
				// 기억했던 부분을 출력해서 커서를 지워준다. --> (surface)
				g_pCursorSurface->Restore(surface, g_pBack);

				// 마우스 출력 영역 설정
				MOUSEPOINTER_INFO mp_info;
				gC_vs_ui.GetCurrentMousePointerInfo(mp_info);

				// 커서 출력 위치를 기억시킨다. --> (surface)
				point.x = mp_info.x;
				point.y = mp_info.y;
				g_pCursorSurface->Store(surface, g_pBack, &point);
				
				//g_pCursorSurface->Restore(surface, g_pBack);

				// 다음에 처리할 surface번호
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
//	if( g_bTestMusic )			// 배경음악 루프중이면
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
//			wsprintf(szbuffer,"CTRL+1~8을 누르시면 해당 곡만 반복해서 들을 수 있습니다.CTRL+0 은 다음곡으로 넘어갑니다.");
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