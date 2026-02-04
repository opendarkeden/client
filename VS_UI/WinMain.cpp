  /*-----------------------------------------------------------------------------

	WinMain.cpp

	MS Windows Application basic routines.
	`VS UI Tester

	2000.1.5. KJTINC

-----------------------------------------------------------------------------*/
#include "client_PCH.h"
#include <assert.h>
#include <windows.h>
#include "DXLib.h"

#include "VS_UI.h" // include VS UI master header file.
#include <math.h>

#include <crtdbg.h>
#include <process.h>
// include by sigi
#include "MItemOptionTable.h"
#include "MItemTable.h"
#include "ExperienceTable.h"
#include "MPriceManager.h"
#include "MSortedItemManager.h"
#include "UserOption.h"
#include "MCreatureTable.h"
#include "ClientConfig.h"
#include "VS_UI_Mouse_pointer.h"
#include "MTradeManager.h"
#include "VS_UI_filepath.h"
#include "MNPCTable.h"
#include "MSoundTable.h"
#include "CSoundPartManager.h"
#include "MParty.h"
#include "AcceleratorManager.h"
#include "KeyAccelerator.h"
#include "AcceleratorDef.h"
#include "MGuildMarkManager.h"
#include "MGuildInfoMapper.h"
#include "UserInformation.h"
#include "UtilityFunction.h"
#include "MGameStringTable.H"
#include "MTimeItemManager.h"
#include "MMonsterKillQuestInfo.h"

#include "MStringArray.h"
#include "MZoneTable.h"
#include "RankBonusTable.h"
#include "FameInfo.h"
#include "MQuestInfo.h"
#include "MMonsterKillQuestInfo.h"
#include "../client/packet/Properties.h"
#include "SystemAvailabilities.h"
#include "ShrineInfoManager.h"
#include "RankBonusDef.h"
#include "VS_UI_GameCommon2.h"
#include "SXml.h"

bool g_bFamily = false;
RECT g_GameRect={799,599,800,600};
BOOL g_MyFull=FALSE;
void InitSound();

#define CLASSNAME				"DX"
#define TITLEBARNAME			"VS UI tester~"

#define _DEBUGGING			1

//#define _FULLSCREEN
/*-----------------------------------------------------------------------------
  PROTOTYPE
-----------------------------------------------------------------------------*/
void MouseEventReceiver(CSDLInput::E_MOUSE_EVENT event, int x, int y, int z);
void KeyboardEventReceiver(CSDLInput::E_KEYBOARD_EVENT event, DWORD scan_code);
void UI_ResultReceiver(DWORD message, int dw_left, int dw_right, void *void_ptr);
void ProgramLoop();

/*-----------------------------------------------------------------------------
  GLOBALS
-----------------------------------------------------------------------------*/
HWND						g_hWnd;
int							g_Dimension = 0;

bool						gbl_ui_input_state;
int		g_LeftPremiumDays =10;

extern EventButton *g_EventButton;

bool						gbl_active; // Is application active?
CDirectDraw				gC_DD;
CSpriteSurface			gC_DDSurface;
extern CDirectInput*			g_pSDLInput;// = new CDirectInput;

static DWORD			g_double_click_time;
int						g_mouse_x, g_mouse_y;

bool						gbl_info_show = false;//true;
bool						gbl_game_back = false;
bool						gbl_show_item = true;

C_VS_UI_NPC_DIALOG		*m_pC_dialog = NULL;

extern CSoundPartManager*		g_pSoundManager;

DWORD	g_CurrentFrame = 0;
						
void kk(C_VS_UI_DIALOG *b, unsigned long a)
{
	int i = 0;
}

std::map<int, bool> g_mapPremiumZone;

IWebBrowser2*			g_pWebBrowser = NULL;
/*-----------------------------------------------------------------------------
- KeyboardEventReceiver
-
-----------------------------------------------------------------------------*/
void KeyboardEventReceiver(CSDLInput::E_KEYBOARD_EVENT event, DWORD scan_code)
{
	static hp;
	static WORD progress;
	static bool bl_ctrl;
	static int time = 0;
	static int x = 0, y = 0;
	static step = 2;
	static count = 2;

	switch (event)
	{
		case CSDLInput::KEYDOWN:
			gC_vs_ui.DIKeyboardControl(event, scan_code);
			switch (scan_code)
			{
				case DIK_LCONTROL:
				case DIK_RCONTROL:
					bl_ctrl = true;
					break;

				case DIK_SPACE:
					//if (bl_ctrl == true)
					//	gC_vs_ui.HotKey_WindowToggle();
					//	gC_vs_ui.HotKey_SummonPet();
					break;

				case DIK_TAB:
					//if (bl_ctrl)
					//	gC_vs_ui.HotKey_Gear();
					//else
					//	gC_vs_ui.HotKey_Inventory();
					break;

				case DIK_ESCAPE:
					//gC_vs_ui.HotKey_ESC();
					break;

				case DIK_LEFT:
					if(g_EventButton)
					{
						g_EventButton->x--;
					}
					else
					if(bl_ctrl)
					{
						time = (time - 1 + 24)%24;
						char temp[10];
						wsprintf(temp, "%d:56:20", time);
						gC_vs_ui.SetTime(temp);
					}
					else
					{
						progress-=10;

//						gC_vs_ui.SetHP(progress, 300);
//						gC_vs_ui.SetMP(progress, 300);
						g_char_slot_ingame.HP -= 10;
						g_char_slot_ingame.MP -= 10;

						gC_vs_ui.SetProgress(progress, 100);
					}
					break;

				case DIK_RIGHT:
					if(g_EventButton)
					{
						g_EventButton->x++;
					}
					else
					if(bl_ctrl)
					{
						time = (time + 1)%24;
						char temp[10];
						wsprintf(temp, "%d:56:20", time);
						gC_vs_ui.SetTime(temp);
					}
					else
					{
						progress+=10;

//						gC_vs_ui.SetHP(progress, 300, true, progress);
//						gC_vs_ui.SetMP(progress, 300);
						g_char_slot_ingame.SILVER_HP += 10;
						g_char_slot_ingame.HP += 10;
						g_char_slot_ingame.MP += 10;

						gC_vs_ui.SetProgress(progress, 100);
					}
					break;

				case DIK_HOME:
					gbl_game_back = !gbl_game_back;
					break;

				extern bool gbl_show_item;
				case DIK_PGUP:
					gbl_show_item = !gbl_show_item;
					break;

				case DIK_INSERT:
					gbl_info_show = !gbl_info_show;
					break;

				case DIK_UP:
					if(g_EventButton)
					{
						g_EventButton->y--;
					}
					else
//					{
//						char buf[50];
//						static DWORD counter;
//
//						sprintf(buf, "@%d", counter++);

//					}
					hp++;
					//gC_vs_ui.SetHP(hp, 100);
					//gC_vs_ui.SetMP(hp, 100);
					break;

				case DIK_DOWN:
					if(g_EventButton)
					{
						g_EventButton->y++;
					}
					else
					hp--;
					//gC_vs_ui.SetHP(hp, 100);
					//gC_vs_ui.SetMP(hp, 100);
					break;

					//
					// hot-key
					//
				case DIK_F1:	
					{
					//	gC_vs_ui.RunNaming();
						//gpC_base->SendMessage(UI_MESSAGE_BOX, STRING_MESSAGE_CANNOT_REPAIR, 0, 	NULL);
//						gC_vs_ui.AddHelpMail(0,false);
//						gC_vs_ui.AddHelpMail(1,false);
//						gC_vs_ui.AddHelpMail(2,false);
//						gC_vs_ui.AddHelpMail(3,false);
//						gC_vs_ui.AddHelpMail(4,false);
//						gC_vs_ui.AddHelpMail(5,false);
//						gC_vs_ui.AddHelpMail(6,false);
//						gC_vs_ui.AddHelpMail(7,false);
//						gC_vs_ui.AddHelpMail(8,false);
//						gC_vs_ui.AddHelpMail(9,false);
//						gC_vs_ui.AddHelpMail(10,false);
//						gC_vs_ui.AddHelpMail(11,false);
//						gC_vs_ui.AddHelpMail(12,false);
//						gC_vs_ui.AddHelpMail(13,false);
//						gC_vs_ui.AddHelpMail(14,false);
//						gC_vs_ui.AddHelpMail(15,false);
//						gC_vs_ui.AddHelpMail(16,false);
//						gC_vs_ui.AddHelpMail(17,false);
//						gC_vs_ui.AddHelpMail(18,false);
//						gC_vs_ui.AddHelpMail(19,false);
//						gC_vs_ui.AddHelpMail(20,false);
//						gC_vs_ui.AddHelpMail(21,false);
//						

//						C_VS_UI_TEAM_INFO::READY_TEAM_INFO info;


//			//			info.REGISTERD_DATE = "2001-12-31 06:30";
//			//			info.MEMBERS = 30;
//						info.REG_FEE = 100000;
//						info.EXPIRE_DATE = "2002-01-05 06:40";





//						info.MEMBERS_MAX = 5;

//						gC_vs_ui.RunTeamInfo(true, &info);	




					}
					
					{
//						static MPetItem *tempItem;
//						tempItem = (MPetItem *)(MItem::NewItem(ITEM_CLASS_PET_ITEM));
//						tempItem->SetItemType(/*g_eRaceInterface+3*/3);
//						tempItem->SetCurrentDurability(20*24*60+20*60);
//						tempItem->SetEnchantLevel(ITEMOPTION_TABLE::PART_LUCK);
//						tempItem->SetSilver(4);
//						tempItem->SetNumber(50);
//						tempItem->SetPetFoodType(0);
//						tempItem->SetPetExpRemain(10000);
//						tempItem->SetPetGamble(true);
//						std::list<TYPE_ITEM_OPTION> optionList;
//						optionList.push_back(162);
//						tempItem->SetItemOptionList(optionList);
//						MPetItem *pPetItem = tempItem;
//						PETINFO petInfo;
//						petInfo.HP = pPetItem->GetCurrentDurability();
//						petInfo.ATTR = pPetItem->GetEnchantLevel();
//						petInfo.ATTR_VALUE = pPetItem->GetSilver();
//						petInfo.LEVEL = pPetItem->GetNumber();
//						petInfo.FOODTYPE = pPetItem->GetPetFoodType();
//						petInfo.EXP_REMAIN = pPetItem->GetPetExpRemain();
//						petInfo.CUT_HEAD = pPetItem->IsCanCutHead();
//						petInfo.GAMBLE = pPetItem->IsCanGamble();
//						if(pPetItem->GetItemOptionListCount() == 0)
//							petInfo.OPTION = -1;
//						else
//							petInfo.OPTION = pPetItem->GetItemOptionList().front();
//						petInfo.ITEM_TYPE = pPetItem->GetItemType();


//						gC_vs_ui.SetPetInfo( &petInfo );
////						gC_vs_ui.RefreshPet();
//						gC_vs_ui.RunPetInfo(&petInfo);
//						gC_vs_ui.RunOtherInfo();
//						gC_vs_ui.SetOtherPetInfo(tempItem);
					}
//					{
//						std::string rankbonusName[RANK_BONUS_MAX] =
//						{
//							"RANK_BONUS_DEADLY_SPEAR.bmp",
//							"RANK_BONUS_BEHEMOTH_ARMOR.bmp",
//							"RANK_BONUS_DARGON_EYE.bmp",
//							"RANK_BONUS_IMMORTAL_HEART.bmp",
//							"RANK_BONUS_RELIANCE_BRAIN.bmp",
//							"RANK_BONUS_SLAYING_KNIFE.bmp",
//							"RANK_BONUS_HAWK_WING.bmp",
//							"RANK_BONUS_HOLY_SMASHING.bmp",
//							"RANK_BONUS_SOUL_SMASHING.bmp",
//							"RANK_BONUS_SAPPHIRE_BLESS.bmp",
//							"RANK_BONUS_RUBY_BLESS.bmp",
//							"RANK_BONUS_DIAMOND_BLESS.bmp",
//							"RANK_BONUS_EMERALD_BLESS.bmp",
//							"RANK_BONUS_MAGIC_BRAIN.bmp",
//							"RANK_BONUS_WIGHT_HAND.bmp",
//							"RANK_BONUS_SEIREN_HAND.bmp",
//							"RANK_BONUS_FORTUNE_HAND.bmp",
//							
//							"RANK_BONUS_IMMORTAL_BLOOD.bmp",
//							"RANK_BONUS_BEHEMOTH_SKIN.bmp",
//							"RANK_BONUS_SAFE_ROBE.bmp",
//							"RANK_BONUS_CROW_WING.bmp",
//							"RANK_BONUS_WISDOM_OF_BLOOD.bmp",
//							"RANK_BONUS_TIGER_NAIL.bmp",
//							"RANK_BONUS_URANUS_BLESS.bmp",
//							"RANK_BONUS_DISRUPTION_STORM.bmp",
//							"RANK_BONUS_WIDE_STORM.bmp",
//							"RANK_BONUS_KNOWLEDGE_OF_POISON.bmp",
//							"RANK_BONUS_KNOWLEDGE_OF_ACID.bmp",
//							"RANK_BONUS_KNOWLEDGE_OF_CURSE.bmp",
//							"RANK_BONUS_KNOWLEDGE_OF_BLOOD.bmp",
//							"RANK_BONUS_KNOWLEDGE_OF_INNATE.bmp",
//							"RANK_BONUS_KNOWLEDGE_OF_SUMMON.bmp",
//							"RANK_BONUS_WISDOM_OF_SWAMP.bmp",
//							"RANK_BONUS_WISDOM_OF_SILENCE.bmp",
//							"RANK_BONUS_WISDOM_OF_DARKNESS.bmp",
//							"RANK_BONUS_WIDE_DARKNESS.bmp",
//							
//							"RANK_BONUS_WOOD_SKIN.bmp",
//							"RANK_BONUS_WIND_SENSE.bmp",
//							"RANK_BONUS_HOMING_EYE.bmp",
//							"RANK_BONUS_LIFE_ENERGY.bmp",
//							"RANK_BONUS_SOUL_ENERGY.bmp",
//							"RANK_BONUS_STONE_MAUL.bmp",
//							"RANK_BONUS_SWIFT_ARM.bmp",
//							"RANK_BONUS_FIRE_ENDOW.bmp",
//							"RANK_BONUS_WATER_ENDOW.bmp",
//							"RANK_BONUS_EARTH_ENDOW.bmp",
//							"RANK_BONUS_ANTI_ACID_SKIN.bmp",
//							"RANK_BONUS_ANTI_BLOODY_SKIN.bmp",
//							"RANK_BONUS_ANTI_CURSE_SKIN.bmp",
//							"RANK_BONUS_ANTI_POISON_SKIN.bmp",
//							"RANK_BONUS_ANTI_SILVER_DAMAGE_SKIN.bmp",
//							"RANK_BONUS_BLESS_OF_NATURE.bmp",
//							"RANK_BONUS_LIFE_ABSORB.bmp",
//							"RANK_BONUS_SOUL_ABSORB.bmp",
//							"RANK_BONUS_MYSTIC_RULE.bmp"
//						};
//						for(int i = 0; i < RANK_BONUS_MAX; i++)
//						{
//							int spriteID = (*g_pRankBonusTable)[i].GetSkillIconID();
//							CSprite *sprite = &C_VS_UI_SKILL::m_C_spk[spriteID];
//							CSpriteSurface surface;
//							int w = sprite->GetWidth(), h = sprite->GetHeight();
//							surface.InitOffsurface(w, h, DDSCAPS_SYSTEMMEMORY);
//							POINT p = {0, 0};
//							if(surface.Lock())
//							{
//								surface.BltSprite(&p, sprite);
//								surface.Unlock();
//
//								surface.SaveToBMP(rankbonusName[i].c_str());
//							}
//						}
//
//					}
//					{
////						C_VS_UI_POPUP_MESSAGE::PopupWindowStyle style;
//
//						std::string filename = "";
//						switch( g_eRaceInterface )
//						{
//						case RACE_SLAYER :
//							filename = "BeginnerZoneSlayer.txt";
//							break;
//						case RACE_OUSTERS :
//							filename = "BeginnerZoneOusters.txt";
//							break;
//						case RACE_VAMPIRE :
//							filename = "BeginnerZoneVampire.txt";
//							break;
//						}
//						
//						style.WindowX = 420;
//						style.WindowY = 450;
//						style.Type = C_VS_UI_POPUP_MESSAGE::POPUP_NORMAL;
//						
//						CRarFile PackFile;
//						PackFile.SetRAR(RPK_TUTORIAL_ETC, RPK_PASSWORD);
//						
//						std::string sender, title, contents;
//						SIZE windowSize;
//
//						if( PackFile.IsSet() )
//						{
//							char temp[4096];
//							PackFile.Open( filename.c_str() );
//							ZeroMemory( temp, 4096 );
//							PackFile.GetString(temp, 4096);
//							sscanf(temp, "%d %d", &windowSize.cx, &windowSize.cy);
//							ZeroMemory( temp, 4096 );
//							PackFile.GetString(temp, 4096);
//							sender = temp;
//							ZeroMemory( temp, 4096 );
//							PackFile.GetString(temp, 4096);
//							title = temp;
//							ZeroMemory( temp, 4096 );
//							PackFile.GetString(temp, 4096);
//							contents = temp;
//							
//							PackFile.Release();
//						} else
//							return;
//						
//						gC_vs_ui.AddMail(0, 3, windowSize, sender.c_str(), title.c_str(), 3102200, false);
//						gC_vs_ui.AddMailContents(0, 3, contents.c_str());
////						gC_vs_ui.RunMailBox(); 
////						gC_vs_ui.RunPopupMessage( style );
//					}
					//gC_vs_ui.RunHorn( 1006 );
//					gC_vs_ui.RunArrowTile();
//					gC_vs_ui.RunCTFStatusWindow();
//					{
//						DWORD endtime = timeGetTime() + 500;
//						int a[3] = {255,255,255};
//						
//						gC_vs_ui.SetCTFStatus( endtime, a[0], a[1], a[2] );
//					}
//					{
// 						gC_vs_ui.RunTeamRegist();
//						int step = 2;
//						gC_vs_ui.RunLotteryCard( step );}
//					gC_vs_ui.RunPopupMessage("ASKGOBEGINNERZONE.txt",C_VS_UI_POPUP_MESSAGE::POPUP_ASK_BEGINNER_ZONE);

//					gC_vs_ui.RequestShrineMinimap( 10000 );

//					gC_vs_ui.RunTeamList(true);
//					gC_vs_ui.RunWarListWindow();
//					{
//						srand(GetTickCount());
//							
//						for(int i=0;i<10;i++)
//						{
//							C_VS_UI_WAR_LIST::WarInfo warlist;
//							warlist.reinforceGuildID = rand()%1000;

//							for(int j = 0; j<5; j++)
//							{
//								warlist.challengerGuildID[j] = rand()%1000;

//							}
//							warlist.year = 2003;
//							warlist.month = 12;
//							warlist.day = i;
//							warlist.warType = rand() & 1;
//							gC_vs_ui.AddWarList(warlist);
//						}
//					}
//					gC_vs_ui.RunBringFeeWindow(100000,0);
//					gC_vs_ui.RunRangerChat();
					//gC_vs_ui.RunInputNameWindow();
					//gC_vs_ui.RunTestWindow()();   
					//gC_vs_ui.RunDepositLimit();
//					gC_vs_ui.RunXmasCardWindow((const MItem *)1); 
//					gC_vs_ui.Run_Campaign_Help_Unfortunate_Neighbors(); 
//					g_char_slot_ingame.m_AdvancementLevel = 2;
					if(gC_vs_ui.IsRunningWebBrowser())
						gC_vs_ui.CloseWebBrowser();
					else
#ifdef	__YHDK2__
						gC_vs_ui.RunWebBrowser(g_hWnd, "http://bbs.yhdk2.cn", (void*)g_pWebBrowser);
#else
						gC_vs_ui.RunWebBrowser(g_hWnd, "http://bbs.ttdk2.com", (void*)g_pWebBrowser);
#endif
					//gC_vs_ui.RunTeamInfo(false,);
				
//					{
//						C_VS_UI_OTHER_INFO::PLAYER_INFO info;
//						
//						info.ALIGNMENT=2;
//						
//						info.PLAYER_NAME="SONEE12345";
//						info.STR_PURE=g_char_slot_ingame.STR_PURE;
//						info.DEX_PURE=g_char_slot_ingame.DEX_PURE;
//						info.INT_PURE=g_char_slot_ingame.INT_PURE;
//						info.STR_CUR=g_char_slot_ingame.STR_CUR;
//						info.DEX_CUR=g_char_slot_ingame.DEX_CUR;
//						info.INT_CUR=g_char_slot_ingame.INT_CUR;
//						info.FAME=9992222;
//						info.guild_id=3067;
//						info.GRADE=35;
//						info.DOMAINLEVEL[0]=5;//g_char_slot_ingame.DOMAIN_BLADE;
//						info.DOMAINLEVEL[1]=100;//g_char_slot_ingame.DOMAIN_SWORD;
//						info.DOMAINLEVEL[2]=47;//g_char_slot_ingame.DOMAIN_GUN;
//						info.DOMAINLEVEL[3]=78;//g_char_slot_ingame.DOMAIN_HEAL;
//						info.DOMAINLEVEL[4]=99;//g_char_slot_ingame.DOMAIN_ENCHANT;
//						//nfo.bMale=TRUE;14
//						info.bl_female=false;
//						info.face_type=M_FACE2;
//						srand(GetTickCount());
//						
////						if(rand()&1)

////						else
////							info.TEAM_NAME = "TETETS";
//						gC_vs_ui.RunOtherInfo();
//						gC_vs_ui.SetOtherInfo(info);
//					}
//				
					//gC_vs_ui.RunShop();
					//gC_vs_ui.RunEnchant();
					//gC_vs_ui.RunExchange();
//					//gC_vs_ui.RunSlayerPortal();
//					gC_vs_ui.RunFileDialog(C_VS_UI_FILE_DIALOG::MODE_PROFILE_SELECT, ".bmp;.jpg");
//					gC_vs_ui.RunSMSMessage();					
					
//					gC_vs_ui.RunTe


//					gC_vs_ui.OpenInventoryToRepair();
//					gC_vs_ui.RequestDie();
//					gC_vs_ui.SetAccelMode(ACCEL_CHARINFO);


//					gC_vs_ui.OpenInventoryToSell();
/*					{
						gC_vs_ui.StartProgress();
						gC_vs_ui.SetCurrentServerName("파라파라월드", "샤마인", C_VS_UI_SERVER_SELECT::STATUS_VERY_GOOD);
						for (int i=0; i<=200; i++)
						{
							gC_vs_ui.SetProgress(i, 200);
							gC_vs_ui.Show();
							gC_DD.Flip();
						}
						
						gC_vs_ui.EndProgress();
					}*/
//					//gC_vs_ui.HotKey_F1();
//					gC_vs_ui.UnselectSkill();

					//					gC_vs_ui.RunEnchantSkillTree();
					//gC_vs_ui.ServerDisconnectMessage();
					//gC_vs_ui.LevelUp();


//					gC_vs_ui.RunTutorialExit();
					break;

				case DIK_F2:

			
//					gC_vs_ui.RunBookcase();
//					if((*g_pSkillInfoTable)[SKILL_BACK_STAB].IsEnable())
//					{
//						(*g_pSkillInfoTable)[SKILL_BACK_STAB].SetEnable(false);
//						(*g_pSkillInfoTable)[SKILL_INSTALL_TRAP].SetEnable(false);
//					}
//					else
//					{
//						(*g_pSkillInfoTable)[SKILL_BACK_STAB].SetEnable(true);
//						(*g_pSkillInfoTable)[SKILL_INSTALL_TRAP].SetEnable(true);
//					}
					//gC_vs_ui.RunComputer();
//					{
//						C_VS_UI_NicknameInfo nik;

//						nik.setNicknameID(100);
//						nik.setNicknameIndex(10);
//						nik.setNicknameType(2);
//						gC_vs_ui.AddNickNameList((void*)&nik);
//					}

					if(gC_vs_ui.IsRunningWebBrowser())
						gC_vs_ui.CloseWebBrowser();
					else
#ifdef	__YHDK2__
						gC_vs_ui.RunWebBrowser(g_hWnd, "http://bbs.yhdk2.cn", (void*)g_pWebBrowser);
#else
						gC_vs_ui.RunWebBrowser(g_hWnd, "http://bbs.ttdk2.com", (void*)g_pWebBrowser);
#endif

//					if(gC_vs_ui.IsRunningPowerjjang())
//					{
//						gC_vs_ui.PowerjjangGambleResult(rand()%7);
//					}
//					else
//						gC_vs_ui.RunPowerjjang();
//					gC_vs_ui.RunTeamMemberList();
//					gC_vs_ui.RunUsePetFood();
//					gC_vs_ui.RequestResurrect(true,true,true);

//						C_VS_UI_QUEST_STATUS::QUEST_STATUS quest;
//						quest.Title = NULL;
//						if(NULL == quest.Title)
//						{
//							char *szBuf = new char[128];

//							quest.Title = szBuf;
//
//
//						}
//						quest.QuestID = 49;
//						quest.quest_time = timeGetTime();
//						quest.current_point = 20;
//						gC_vs_ui.SetQuestStatus( quest );
//						gC_vs_ui.RunQuestStatusWindow();

					//gC_vs_ui.RunBringFeeWindow(1000000000,0);
//					gpC_Imm->ForceAction(SOUND_ITEM_USE_C4);
//					gpC_Imm->ForceSound("kaka");

//					gpC_Imm->ForceAction(CImm::FORCE_ACTION_DAMAGED);
//					gC_vs_ui.RunTeamList(false);
//					{
//						for(int i=100;i<150;i++)
//						{
//							char name[100];
//							C_VS_UI_TEAM_LIST::REGIST_TEAM_LIST son;
//							son.guild_id=i;
//							son.MEMBERS=i*50;							
//							wsprintf(name,"%d",rand()%100000);
//							son.TEAM_NAME=name;
//							wsprintf(name,"%d",rand()%100000);
//							son.LEADER_NAME=name;
//							son.RANKING=i;
//							gC_vs_ui.AddRegistTeamInfo(son);					
//						}
//				 	}					

					//gC_vs_ui.HotKey_F2();

//					gC_vs_ui.RunHealSkillTree();



					//gC_vs_ui.RunBookcase();
//					gC_vs_ui.RunSlayerPortal();

					
					break;

				case DIK_F3:
			//		gC_vs_ui.OpenInventoryToRepair();
		//	gC_vs_ui.RunFileDialog(C_VS_UI_FILE_DIALOG::MODE_SENDFILE_SELECT,".bmp");
					//gC_vs_ui.RunBookcase();
					//gC_vs_ui.RunPetStorage();
					//if(!gC_vs_ui.IsRunningQuestManager())
					{
					#ifdef __TEST_SUB_INVENTORY__
						MItem* pItem = MItem::NewItem( ITEM_CLASS_SUB_INVENTORY );
						pItem->SetID( rand()%10000 );
						pItem->SetItemType( 0 );
						gC_vs_ui.RunSubInventory( pItem );
					#endif

					}
//						gC_vs_ui.RunQuestManager();
					//else
					//	gC_vs_ui.CloseQuestManager();

//					{
//						MItem* pItem = MItem::NewItem( (MItem::ITEM_CLASS)(rand()%40) );
//						
//						pItem->SetID( rand()%10000 );
//						pItem->SetItemType( rand()% (*g_pItemTable)[pItem->GetItemClass()].GetSize());
//						for(int i=0;i<2;i++)
//						{
//							if(rand()&0x1)
//								pItem->AddItemOption( rand() % 50 );
//						}
//						pItem->SetCurrentDurability( 2000 );
//
//						gC_vs_ui.AddItemToItemList(pItem);
//					}
					//
					//gC_vs_ui.HotKey_F3();
//					gC_vs_ui.RunBladeSkillTree();



					//gC_vs_ui.RunBriefing();
//					gC_vs_ui.RunElevator();

//					gC_vs_ui.RunPartyManager();
//					gC_vs_ui.RequestDie();
//					gC_vs_ui.ResetOustersQuickItemSize();
//					gC_vs_ui.RunQuestInventory();
//					gC_vs_ui.RunItemListWindow();
//					gC_vs_ui.SetDelayRequestResurrect( C_VS_UI_REQUEST_RESURRECT::RESURRECT_MODE_ETERNITY, 5000);
					//					gC_vs_ui.RunMixingForge( C_VS_UI_MIXING_FORGE::CLASS_ACCESSORY, C_VS_UI_MIXING_FORGE::TYPE_A );
					{
//						C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_STATUS status;
//						
//						status.zone_id = 71;
//						status.x = 100;
//						status.y = 100;
//						status.name = "SONEE";
//						status.shrine_race = RACE_OUSTERS;
//						
//						status.status = C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_HAS_OUSTERS;
//						int id=3;
//
//						gC_vs_ui.RunBloodBibleStatus();
//
//						gC_vs_ui.SetBloodBibleStatus(id, status);
					}
					break;

				case DIK_F4:
				
//					gC_vs_ui.RunElevator();
					//gC_vs_ui.ServerDisconnectMessage();
//			gC_vs_ui.OpenInventoryToSell();
				//	gC_vs_ui.RunItemListWindow();
					

//					gC_vs_ui.RunSwordSkillTree();
					//gC_vs_ui.HotKey_F4();
//					gC_vs_ui.RunVampireSkillTree();
				//	gC_vs_ui.RunComputer();

					//					gC_vs_ui.ServerDisconnectMessage();
					//					gC_vs_ui.StartTitle();

					//					gC_vs_ui.OpenInventoryToSell();

					{
					//	g_pSlayerGear->RemoveItem((MSlayerGear::GEAR_SLAYER)10);
					}
					break;
					
				case DIK_F5:
					{
						//gC_vs_ui.OpenInventoryToRepair();

						//	PARTY_INFO *temp_party = new PARTY_INFO;
						//	temp_party->bMale = false;
						//	temp_party->hairStyle = W_FACE1;

						//	g_pParty->AddMember(temp_party);
						//					gC_vs_ui.OpenInventoryToRepair();
						
						

						//gC_vs_ui.OpenInventoryToSell();
//						{
//							DWORD id = 101;
//							gC_vs_ui.RunImageNotice();

//							int step = 1;
////							gC_vs_ui.RunLotteryCard(step);
//						}
						//					gC_vs_ui.RunGunSkillTree();
						//gC_vs_ui.HotKey_F5();
						//					gC_vs_ui.RunSkillView();
						

						//					gC_vs_ui.RunStorageBuy(1000000);
						//gC_vs_ui.PleaseConnectPCS("KJTINC", 1234567, 1);
						
						/*

					const char* pString = gC_vs_ui.GetInputString();

					if (pString!=NULL && pString[0]=='!')
					{
						if (gbl_vampire_interface)
						{
							gC_vs_ui.SetInputStringColor( RGB(50, 50, 200) );
						}
						else
						{
							gC_vs_ui.SetInputStringColor( RGB(200, 50, 50) );
						}
					}
					else
					{
						gC_vs_ui.SetInputStringColor( gpC_base->m_chatting_pi.text_color );
					}
					*/
				}
				break;
				
				case DIK_F6:
					gC_vs_ui.ChangeToOustersInterface();
//					{
//						DWORD id = 11;

//					}
//					gC_vs_ui.OpenInventoryToSilvering();
//					{
//						gC_vs_ui.RunElevator();
//						if (m_pC_dialog!=NULL)
//						{
//							delete m_pC_dialog;
//							m_pC_dialog = NULL;
//						}
//						else
//						{
//							static num = 431;

//						
//							DIALOG_MENU d_menu[] = {




//								};
//							m_pC_dialog->SetMenu(d_menu, 4, false);
//							














//							};
//							
//							m_pC_dialog->SetMessage(pp_dmsg, sizeof(pp_dmsg)/sizeof(char *));//, SMO_NOFIT);
//
//							m_pC_dialog->Start();
//							num++;
//						}
					
						

					//gC_vs_ui.HotKey_F6();
					//gC_vs_ui.PleaseConnectPCS("toggi", 3355447, 2);

//					}
					break;

				case DIK_F7:
					//gC_vs_ui.HotKey_F7();
					gC_vs_ui.ChangeToSlayerInterface();
					break;

				case DIK_F8:
					//gC_vs_ui.HotKey_F8();
					gC_vs_ui.ChangeToVampireInterface();
					break;

				case DIK_F9:
//					gC_vs_ui.ChangeToVampireInterface();
					gC_vs_ui.RunShop();
					//gC_vs_ui.OpenInventoryToRepair();
					//gC_vs_ui.OpenInventoryToSilvering();
					{
						//
//						gC_vs_ui.RunStorage();

					}
					break;

				case DIK_F10:
					//gC_vs_ui.OpenInventoryToRepair();
					
					//


					//
					//gC_vs_ui.HotKey_F10();
//					gC_vs_ui.RunStorage();
					break;

				case DIK_F11:
//					gC_vs_ui.OpenInventoryToSell();
//					gC_vs_ui.RunStorage();
					gC_vs_ui.RunExchange();
//					gC_vs_ui.ServerDisconnectMessage();
					break;

				case DIK_F12:
					//gC_vs_ui.RunShop();
//					gC_vs_ui.HotKey_F12();
					gC_vs_ui.RequestDie();
					break;

				case DIK_NUMPAD4:
					if(bl_ctrl)
						x -= 10;
					else x--;
					gC_vs_ui.SetXY(x, y);
					break;

				case DIK_NUMPAD6:
					if(bl_ctrl)
						x += 10;
					else x++;
					gC_vs_ui.SetXY(x, y);
					break;

				case DIK_NUMPAD8:
					if(bl_ctrl)
						y -= 10;
					else y--;
					gC_vs_ui.SetXY(x, y);
					break;

				case DIK_NUMPAD2:
					if(bl_ctrl)
						y += 10;
					else y++;
					gC_vs_ui.SetXY(x, y);
					break;


				case DIK_1:
//					gC_vs_ui.SetInputStringColor(RGB_WHITE);
					if(bl_ctrl)
					gpC_mouse_pointer->SetCursorDefault();
					break;

				case DIK_2:
//					gC_vs_ui.SetInputStringColor(RGB_RED);
					if(bl_ctrl)
					gpC_mouse_pointer->SetCursorAttack();
					break;

				case DIK_3:
					if(bl_ctrl)
					gpC_mouse_pointer->SetCursorPickUp("스컬 네크리스", RGB_YELLOW);
					break;

				case DIK_5:
					if(bl_ctrl)
					gpC_mouse_pointer->SetCursorNotMove();
					break;

				case DIK_6:
					if(bl_ctrl)
					gpC_mouse_pointer->SetCursorPortal(11);
					break;

				case DIK_7:
					if(bl_ctrl)
					gpC_mouse_pointer->SetCursorPortal(11, 10, 10);
					break;

				case DIK_8:
					if(bl_ctrl)
					gpC_mouse_pointer->SetCursorPortal(11, 10, 10, "뱀파테스터");
					break;

			}
			break;

		case CSDLInput::KEYUP:
			switch (scan_code)
			{
				case DIK_LCONTROL:
				case DIK_RCONTROL:
					bl_ctrl = false;
					break;
			}
			break;
	}
}

BOOL g_bLButtonDown = false;

/*-----------------------------------------------------------------------------
- MouseEventReceiver
-
-----------------------------------------------------------------------------*/
void MouseEventReceiver(CSDLInput::E_MOUSE_EVENT event, int x, int y, int z)
{
	static DWORD	last_click_time;
	static int		double_click_x, double_click_y;



#ifndef _FULLSCREEN
	POINT point;
	GetCursorPos(&point);
	g_mouse_x = point.x;
	g_mouse_y = point.y;

	if (g_mouse_x < 0)
		g_mouse_x = 0;
	else if (g_mouse_x >= RESOLUTION_X)
		g_mouse_x = RESOLUTION_X-1;
	if (g_mouse_y < 0)
		g_mouse_y = 0;
	else if (g_mouse_y >= RESOLUTION_Y)
		g_mouse_y = RESOLUTION_Y-1;

	x = g_mouse_x;
	y = g_mouse_y;
#else
	g_mouse_x = x;
	g_mouse_y = y;
#endif

	switch (event)
	{
		case CSDLInput::MOVE:
			gbl_ui_input_state = gC_vs_ui.MouseControl(M_MOVING, x, y);
//			gC_vs_ui.MouseControlExtra(M_MOVING, x, y);
			break;

		case CSDLInput::LEFTDOWN:
			//  double-click interval?
			if ((DWORD)abs(GetTickCount() - last_click_time) <= g_double_click_time)
			{
				if (x>= double_click_x-1 && x <= double_click_x+1 &&
					 y>= double_click_y-1 && y <= double_click_y+1)
				{
					gbl_ui_input_state = gC_vs_ui.MouseControl(M_LB_DOUBLECLICK, x, y);					
					last_click_time = 0;
					return;
				}
			}				

			gbl_ui_input_state = gC_vs_ui.MouseControl(M_LEFTBUTTON_DOWN, x, y);
			last_click_time = GetTickCount();
			double_click_x = x;
			double_click_y = y;

//			gC_vs_ui.ChatMouseControlExtra(M_LEFTBUTTON_DOWN, x, y);
			g_bLButtonDown = TRUE;

			break;

		case CSDLInput::LEFTUP:
			gbl_ui_input_state = gC_vs_ui.MouseControl(M_LEFTBUTTON_UP, x, y);
			g_bLButtonDown = FALSE;
			break;

		case CSDLInput::RIGHTDOWN:
			gbl_ui_input_state = gC_vs_ui.MouseControl(M_RIGHTBUTTON_DOWN, x, y);
			break;

		case CSDLInput::RIGHTUP:
			gbl_ui_input_state = gC_vs_ui.MouseControl(M_RIGHTBUTTON_UP, x, y);
			if(gbl_ui_input_state == false)g_EventButton = NULL;
			break;

		case CSDLInput::CENTERDOWN:
			gbl_ui_input_state = gC_vs_ui.MouseControl(M_CENTERBUTTON_DOWN, x, y);
			break;

		case CSDLInput::CENTERUP:
			gbl_ui_input_state = gC_vs_ui.MouseControl(M_CENTERBUTTON_UP, x, y);
			break;

		case CSDLInput::WHEELDOWN:
			gbl_ui_input_state = gC_vs_ui.MouseControl(M_WHEEL_DOWN, x, y);
			break;

		case CSDLInput::WHEELUP:
			gbl_ui_input_state = gC_vs_ui.MouseControl(M_WHEEL_UP, x, y);
			break;
	}
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The Main Window Procedure
//-----------------------------------------------------------------------------
long FAR PASCAL 
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static char		scan_code;
	//DWORD				dwCommand, dwData;

    switch (message)
    {
		 /*
		case WM_IME_NOTIFY:
			dwCommand = (DWORD) wParam;
			dwData = (DWORD) lParam;

			if (dwCommand == IMN_SETOPENSTATUS)
			{
				int z = 0;
			}
			return 0;*/

	 case WM_KEYUP:
		case WM_KEYDOWN:
		case WM_CHAR:
		 case WM_IME_COMPOSITION:
		 case WM_IME_STARTCOMPOSITION:
		 case WM_IME_ENDCOMPOSITION:
			gC_vs_ui.KeyboardControl(message, wParam, lParam);

			if (message == WM_IME_STARTCOMPOSITION)
				return 0;

			break; // ! break!!!!

		 case WM_MOVE:
			if (gbl_active && !CSDLGraphics::IsFullscreen())
				CSDLGraphics::OnMove();
			return 0L;

        case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
            // Pause if minimized
            gbl_active = !((BOOL)HIWORD(wParam));
//				g_pSDLInput->SetAcquire(gbl_active);
				g_pSDLInput->SetAcquire(true);
				g_double_click_time = GetDoubleClickTime();

            return 0L;

		case WM_MOUSEACTIVATE:
			{
			int a = 0;
			}
			break;

        case WM_DESTROY:
            // Clean up and close the app
            gC_DD.ReleaseAll();
            PostQuitMessage(0);
            return 0L;
/*
        case WM_KEYDOWN:
            // Handle any non-accelerated key commands
            switch (wParam)
            {
                case VK_ESCAPE:
                case VK_F12:
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                    return 0L;
            }
            break;*/

        case WM_SETCURSOR:
            // Turn off the cursor since this is a full-screen app
            SetCursor(NULL);
            return 0L;

		  case WM_SIZE:
			  return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

//.......................................................................................

float dtan(float deg)    {
	if(deg>360 || deg<0)    return((float)tan(-1));
	return((float)((3.141592*deg)/180));
}
////////////////////////////////////////////////////////////////////


void    Lenz(int x,int y,int radius)    {
	static int sx,sy;
	static int xsize, ysize;
	static int a,b,c,d,e,f;
	static int j,i, daddr;
	static float g;
	static WORD *data;
	
	if ( x<radius )            x = radius+1;
	if ( y<radius )            y = radius+1;
	if ( x+radius>=640 )    x = 639-radius;
	if ( y+radius>=480 )    y = 479-radius;
	/*

	static int dir=0, x=300, y=200;
	switch( dir )    {
	case 0:            // move(right,down)
	x+=10;    y+=10;
	if( (640-radius)<x )    dir = 3;
	if( (400-radius)<y )    dir = 1;
	break;
	case 1:            // move(right,up )
	x+=10;    y-=10;
	if( (640-radius)<x )    dir = 2;
	if( y<radius )    dir = 0;
	break;
	case 2:            // move(left ,up )
	x-=10;    y-=10;
	if( x<radius     )    dir = 1;
	if( y<radius )    dir = 3;
	break;
	case 3:            // move(left ,down)
	x-=10;    y+=10;
	if( x<radius     )    dir = 0;
	if( (400-radius)<y )    dir = 2;
	break;
	}
	*/
	///////////////////////////////////////////////////////////////////////////
	
	sx = x-radius;        sy = y-radius;
	xsize = ysize = radius+radius;
	data = new WORD[xsize*xsize];
	
//	gC_DD->Lock();// NULL, &m_ddsd, DDLOCK_WAIT, NULL );
	gpC_base->m_p_DDSurface_back->Lock();
	WORD *m_pdest = (WORD *)gpC_base->m_p_DDSurface_back->GetSurfacePointer();//(WORD*)m_ddsd.lpSurface;
	const int pitch = gpC_base->m_p_DDSurface_back->GetSurfacePitch()/2;
	m_pdest += sy*pitch/*m_ddsd.lPitch>>1*/ + sx;
	daddr = 0;
	for( j=0; j<ysize; j++ )    {
		memcpy( &data[daddr], m_pdest, (xsize<<1) );
		m_pdest += pitch;//m_ddsd.lPitch>>1;
		daddr += xsize;
	}
	
	m_pdest = (WORD *)gpC_base->m_p_DDSurface_back->GetSurfacePointer();//(WORD*)m_ddsd.lpSurface;
	for(j=0;j<radius;j++)    {
		for(i=0;i<radius;i++)    {
			g=1;
			double sum=sqrt(i*i+j*j);
			if(sum<radius)        g=dtan((float)(sum/2.5+20));
			a=(radius+j)*xsize;
			b=(radius-j)*xsize;
			c=(int)(j*g);
			d=(int)(i*g);
			e=(int)( c+y)*pitch;
			f=(int)(-c+y)*pitch;
			data[a-i+radius] = m_pdest[e+(int)(-d+x)];
			data[a+i+radius] = m_pdest[e+(int)( d+x)];
			data[b-i+radius] = m_pdest[f+(int)(-d+x)];
			data[b+i+radius] = m_pdest[f+(int)( d+x)];
		}
	}
	m_pdest += sy*pitch + sx;
	daddr = 0;
	for( j=0; j<ysize; j++ )    {
		memcpy( m_pdest, &data[daddr], (xsize<<1) );
		m_pdest += pitch;
		daddr += xsize;
	}
	gpC_base->m_p_DDSurface_back->Unlock();//m_DDSBack->Unlock(NULL);
	delete data;
}                                              




void Water( int xx, int yy, char Density, char Radius )    
{
	gpC_base->m_p_DDSurface_back->Lock();//m_DDSBack->Unlock(NULL);
	WORD *m_pBackSpace = (WORD *)gpC_base->m_p_DDSurface_back->GetSurfacePointer();//(WORD*)m_ddsd.lpSurface;
    static int Height[2][307200], hkind = 0;
	static WORD WaveScr[307200];
    static WORD Color2;
    static int cx,cy, cnt;
    static int addr1,addr2;
    static HiCurr, HiNext;
    static int xhdif, yhdif, Chdif;
    static int XPos, YPos, OPos;
    static int WcntX1,WcntY1, WcntX2,WcntY2;
    static bool bStarted = false;
	
    if( bStarted )    {

		HiNext = Height[0][1];
		for( cnt = 1; cnt<306560; cnt++) {
			HiCurr = HiNext;
            xhdif = HiCurr - HiNext;
            yhdif = HiCurr - Height[0][cnt+640];
			HiNext = Height[0][cnt+1];
			Chdif = 128 - yhdif;
			xhdif >>= 3;
			yhdif >>= 3;
			WcntY1 = cnt>>10;
			WcntX1 = cnt - (WcntY1<<10);
			WcntX2 = yhdif+WcntX1;
			WcntY2 = xhdif+WcntY1;
			OPos = (WcntY2<<10) + WcntX2;
			OPos = OPos>=0? OPos:0;
			OPos = OPos<307200? OPos:307199;
            addr1 = OPos/640;
            addr2 = OPos - addr1*640;
			WORD color = m_pBackSpace[addr1*800+addr2];
			WORD rr = (((color&0xf800)>>8) * Chdif) >> 7;
			WORD gg = (((color&0x7e0)>>3) * Chdif) >> 7;
			WORD bb = (((color&0x1f)<<3) * Chdif) >> 7;
			if( rr<0 || bb<0 || gg<0 ) rr = gg = bb = 0;
			if( rr>255 || bb>255 || gg>255 ) rr = gg = bb = 255;
			WaveScr[cnt] =((rr&0xf8)<<8)|((gg&0xfc)<<3)|((bb&0xf8)>>3);
		}
		


			switch( hkind )    {
        case 0:
            for( cnt= 642; cnt<306559; cnt++) {
                Height[0][cnt] = ((
                    Height[1][cnt-1] +
                    Height[1][cnt+1] +
                    Height[1][cnt-640] +
                    Height[1][cnt+640] +
                    Height[1][cnt-641] +
                    Height[1][cnt-639] +
                    Height[1][cnt+639] +
                    Height[1][cnt+641]
                    )>>2 ) - Height[0][cnt];
                Height[0][cnt] -= Height[0][cnt] >> Density;
            }
            hkind = 1;
            break;
        case 1:
            for( cnt= 642; cnt<306559; cnt++) {
                Height[1][cnt] = ((
                    Height[0][cnt-1] +
                    Height[0][cnt+1] +
                    Height[0][cnt-640] +
                    Height[0][cnt+640] +
                    Height[0][cnt-641] +
                    Height[0][cnt-639] +
                    Height[0][cnt+639] +
                    Height[0][cnt+641]
                    )>>2 ) - Height[1][cnt];
                Height[1][cnt] -= Height[1][cnt] >> Density;
            }
            hkind = 0;
            break;
        }
		

        addr1 = 0;        addr2 = 0;
        for( cy=0; cy<480; cy++ )    {
            memcpy( &m_pBackSpace[addr1], &WaveScr[addr2], 1280 );
            addr1 += 800;
            addr2 += 640;
        }
    }    else    {
        int left,top,right,bottom;
        int square, dist;
        int radsquare;
        double length;
		
        radsquare = Radius * Radius;
		length = (1024.0/(double)Radius)*(1024.0/(double)Radius);
        top = -Radius;        left = -Radius;
        bottom = Radius; right = Radius;
		
        // Perform edge clipping...
		if( xx-Radius < 1 )        left -= xx-Radius-1;
		if( yy-Radius < 1 ) top -= yy-Radius-1;
		if( xx+Radius > 639 ) right -= xx+Radius-640+1;
		if( yy+Radius > 479 ) bottom -= yy+Radius-480+1;
		
		for( cy=top; cy<bottom; cy++) {
            addr1 = (cy+yy)*640;
			for( cx=left; cx<right; cx++) {
				square = cy*cy + cx*cx;
				if( square<radsquare ) {
					dist = (int)sqrt(square*length);
					Height[0][addr1 + xx+cx] += (int)((cos(dist)+0xffff)*480) >> 19;
				}
			}
		}
        bStarted = true;
    }
	gpC_base->m_p_DDSurface_back->Unlock();//m_DDSBack->Unlock(NULL);
}                 

          
void ProgramLoop()
{

	if(gbl_game_back)
	{
		static C_SPRITE_PACK m_game_back(SPK_GAME_BACK);
		m_game_back.Blt(0, 0);
	}
	else	gC_DDSurface.FillSurface(15220); // clear the screen buffer 

	// update mouse input state
	g_pSDLInput->UpdateInput();

	gC_vs_ui.Process(); // at last
	gC_vs_ui.Show();
	gC_vs_ui.DrawMousePointer();


//	gC_vs_ui.SetTime("23:00:05");

	if (gbl_info_show == true)
	{
		gC_DDSurface.ShowFPS(0, 0, RGB(255, 255, 255), RGB(128, 0, 128));

		char buf[20];

		sprintf(buf, "%dx%dx%d", RESOLUTION_X, RESOLUTION_Y, gC_DD.Get_BPP());
		gC_DDSurface.GDI_Text(90, 0, buf, RGB(255, 255, 255), RGB(128, 0, 128), false);


		sprintf(buf, "(%d, %d)", g_mouse_x, g_mouse_y);
		gC_DDSurface.GDI_Text(500, 0, buf, RGB(255, 255, 255), RGB(128, 0, 128), false);

		if (gbl_ui_input_state)
			gC_DDSurface.GDI_Text(190, 0, "UI input mode", RGB(255, 255, 255), RGB(128, 0, 128), false);
		else
			gC_DDSurface.GDI_Text(190, 0, "Background mode", RGB(255, 255, 255), RGB(128, 0, 128), false);

	}

	assert(!gpC_base->m_p_DDSurface_back->IsLock());
//	Water(gpC_mouse_pointer->GetX(), gpC_mouse_pointer->GetY(), 3, 100);
	gC_DD.Flip();

}

/*-----------------------------------------------------------------------------
- UI_ResultReceiver
-

  dw_left와 dw_right는 LOWORD, HIWORD로 검색할 수 있다. WORD이하의 값은 전부
  그런식으로 들어온다.
  만약 DWORD의 값이라면 그것을 사용하지 않는다.
-----------------------------------------------------------------------------*/
void UI_ResultReceiver(DWORD message, int dw_left, int dw_right, void *void_ptr)
{
	switch (message)
	{
//	case UI_NEWCHARACTER_CHECK:
//		gC_vs_ui.UsedID(true);
//		break;

	case UI_CLOSE_POPUP_MESSAGE:
		gC_vs_ui.ClosePopupMessage();
		break;

	case UI_CLOSE_MAILBOX:
		gC_vs_ui.CloseMailBox();
		break;

	case UI_CLOSE_OUSTERS_SKILL_INFO:
		gC_vs_ui.CloseOustersSkillInfo();
		break;

	case UI_CLOSE_IMAGE_NOTICE :
		gC_vs_ui.CloseImageNotice();
		break;
	case UI_SEND_BRING_FEE :
		gC_vs_ui.CloseBringFeeWindow();
		break;
	case UI_CLOSE_BRING_FEE_WINDOW :
		gC_vs_ui.CloseBringFeeWindow();
		break;

	case UI_USE_XMAS_TREE:
		{
			const char *sz_ptr = (const char *)void_ptr;
		}
		break;

	case UI_CLOSE_XMAS_CARD_WINDOW:
		gC_vs_ui.CloseXmasCardWindow();
		break;

	case UI_RUN_CONNECT:
		gC_vs_ui.RunConnect();
		break;

	case UI_CLOSE_FILE_DIALOG:
		switch(dw_left)
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


						RECT bmpRect = { 0, 0, bmpSurface.GetWidth(), bmpSurface.GetHeight() };
						
						// BigSize
						surface.FillSurface( 0 );
						surface.Blt(&destBigRect, &bmpSurface, &bmpRect);
						
						char saveBmpName[512];
						strcpy(saveBmpName, DIR_PROFILE);
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
		break;

	case UI_CLOSE_TEAM_MEMBER_INFO:
		gC_vs_ui.CloseTeamMemberInfo();
		break;

	case UI_LOTTERY_CARD_STATUS :
		gC_vs_ui.SetResultLotteryCard( false);
		break;
	case UI_CLOSE_LOTTERY_CARD :
		gC_vs_ui.CloseLotteryCard();
		break;
		
	case UI_JOIN_READY_TEAM:
		gC_vs_ui.RunTeamRegist(true, 100000, 1,"2002-01-01", "쑥갓팀이다",100);
		break;

	case UI_JOIN_REGIST_TEAM:
		gC_vs_ui.RunTeamRegist(true, 100000, 2,"2002-01-01", "쑥갓팀이다",100);
		break;

	case UI_MESSAGE_BOX :
		switch(dw_left)
		{
		case STRING_MESSAGE_CANNOT_REPAIR :
			MessageBox(NULL,"에거... 이거 원래 클라이언트에서밖에 못띄우는 다이알로그라 이렇게 백업!\r\n아이템 리패어 못해염!","-_-; 게임상엔 제대로 나오는 다이알로그박스",MB_OK);
		break;
		}
		break;	

	case UI_REQUEST_GUILD_MEMBER_LIST:
		{
		C_VS_UI_TEAM_MEMBER_LIST::TEAM_MEMBER_LIST member_info;
		member_info.MEMBER_NAME = "가나다라마바";
		member_info.SERVER_NAME = "아바로스(pk금지)";
		member_info.member_grade = 1;
		member_info.bLogOn = 1;
		gC_vs_ui.AddTeamMemberInfo(member_info);
		member_info.MEMBER_NAME = "2";
		member_info.member_grade = 2;
		member_info.bLogOn = 1;
		gC_vs_ui.AddTeamMemberInfo(member_info);
		member_info.MEMBER_NAME = "3";
		member_info.SERVER_NAME = "3";
		member_info.member_grade = 0;
		member_info.bLogOn = 0;
		gC_vs_ui.AddTeamMemberInfo(member_info);
		member_info.MEMBER_NAME = "4";
		member_info.SERVER_NAME = "3";
		member_info.member_grade = 0;
		gC_vs_ui.AddTeamMemberInfo(member_info);
		member_info.MEMBER_NAME = "5";
		member_info.SERVER_NAME = "3";
		member_info.member_grade = 3;
		gC_vs_ui.AddTeamMemberInfo(member_info);
		member_info.MEMBER_NAME = "6";
		member_info.SERVER_NAME = "3";
		member_info.member_grade = 3;
		gC_vs_ui.AddTeamMemberInfo(member_info);
		member_info.MEMBER_NAME = "7";
		member_info.SERVER_NAME = "3";
		member_info.member_grade = 3;
		gC_vs_ui.AddTeamMemberInfo(member_info);
		member_info.MEMBER_NAME = "8";
		member_info.SERVER_NAME = "3";
		member_info.member_grade = 3;
		gC_vs_ui.AddTeamMemberInfo(member_info);
		member_info.MEMBER_NAME = "9";
		member_info.SERVER_NAME = "3";
		member_info.member_grade = 3;
		gC_vs_ui.AddTeamMemberInfo(member_info);
		member_info.MEMBER_NAME = "10";
		member_info.SERVER_NAME = "asgasfgas";
		member_info.member_grade = 3;
		gC_vs_ui.AddTeamMemberInfo(member_info);
		member_info.MEMBER_NAME = "11";
		member_info.SERVER_NAME = "111111";
		member_info.member_grade = 3;
		gC_vs_ui.AddTeamMemberInfo(member_info);
		member_info.MEMBER_NAME = "12";
		member_info.SERVER_NAME = "1";
		member_info.member_grade = 3;
		gC_vs_ui.AddTeamMemberInfo(member_info);
		member_info.MEMBER_NAME = "13";
		member_info.SERVER_NAME = "0";
		member_info.member_grade = 3;
		gC_vs_ui.AddTeamMemberInfo(member_info);
		}
		break;

	case UI_REGIST_GUILD_MEMBER:
	case UI_REGIST_GUILD_TEAM:
		gC_vs_ui.CloseTeamRegist();
		break;

	case UI_SELECT_TEAM_MEMBER_LIST:
		{
			C_VS_UI_TEAM_MEMBER_INFO::MEMBER_INFO info;
			info.NAME = "asdasfasdfa";
			info.GRADE = 0;
			info.INTRODUCTION = "ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ";
			info.guild_id=102;
			gC_vs_ui.RunTeamMemberInfo(&info);
		}
		break;
	case UI_MODIFY_GUILD_MEMBER_INTRO :
		MessageBox(NULL,(char*)void_ptr,"sd",MB_OK);
		break;
	case UI_SELECT_READY_TEAM_LIST:
		{
			C_VS_UI_TEAM_INFO::READY_TEAM_INFO info;
			info.TEAM_NAME = "쑥갓들이야";
			info.LEADER_NAME = "쑥갓";
//			info.REGISTERD_DATE = "2001-12-31 06:30";
//			info.MEMBERS = 30;
			info.REG_FEE = 100000;
			info.EXPIRE_DATE = "2002-01-05 06:40";
			info.MEMBERS_NAME.push_back("쑥갓");
			info.MEMBERS_NAME.push_back("치아푸푸");
			info.MEMBERS_NAME.push_back("푸리린");
			info.MEMBERS_NAME.push_back("로봇매니야");
			info.MEMBERS_NAME.push_back("깐따삐야");
			info.MEMBERS_MAX = 5;
			info.INTRODUCTION = "ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ";
			gC_vs_ui.RunTeamInfo(true, &info);
		}
		break;

	case UI_SELECT_REGIST_TEAM_LIST:
		{
			C_VS_UI_TEAM_INFO::REGIST_TEAM_INFO info;
			info.TEAM_NAME = "쑥갓들이야";
			info.LEADER_NAME = "쑥갓";
			info.REGISTERED_DATE = "2001-12-31 06:30";
			info.MEMBERS = 130;
			info.REG_FEE = 100000;
			info.RANKING = 1;
			info.guild_id= 101;		
			info.INTRODUCTION = "우리팀은여~ 절라 멋져염 ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ\n글구여 언능 와여~\n방가방가~";
			gC_vs_ui.RunTeamInfo(FALSE, &info);
		}
		break;
	case UI_MODIFY_TEAM_INFO :
		MessageBox(NULL,(char*)void_ptr,"sad",MB_OK);
		break;

		case UI_CLOSE_TEAM_REGIST:
		{
			gC_vs_ui.CloseTeamRegist();
		}
		break;

		case UI_CLOSE_TEAM_LIST:
		{
			gC_vs_ui.CloseTeamList();
		}
		break;

		case UI_CLOSE_TEAM_MEMBER_LIST:
		{
			gC_vs_ui.CloseTeamMemberList();
		}
		break;

		case UI_CLOSE_TEAM_INFO:
		{
			gC_vs_ui.CloseTeamInfo();
		}
		break;

		case UI_AWAY_PARTY:
		{
			int a = 0;
		}
		break;
		case UI_CLOSE_SLAYER_PORTAL :
			{
			gC_vs_ui.CloseSlayerPortal();
			}
		break;

		case UI_CLOSE_PARTY_MANAGER :
			{
			gC_vs_ui.ClosePartyManager();
			}
		break;

		case UI_SLAYER_PORTAL:
			{
				int a = 0;
			}
			break;
		case UI_ITEM_DROP_TO_CLIENT :
		{
			int a = 0;
		}
		break;

		case UI_CLOSE_DESC_DIALOG :
			gC_vs_ui.CloseDescDialog();
		break;

		case UI_SELECT_ELEVATOR:
		case UI_CLOSE_ELEVATOR :
			gC_vs_ui.CloseElevator();
		break;

		case UI_CLOSE_BOOKCASE :
			gC_vs_ui.CloseBookcase();
		break;

//		case UI_CLOSE_BRIEFING : // by larosel
//			gC_vs_ui.CloseBriefing();
//		break;

		case UI_CLOSE_COMPUTER : // by larosel
			gC_vs_ui.CloseComputer();
		break;

//		case UI_CLOSE_TUTORIAL_EXIT : // by larosel
//		{
//			gC_vs_ui.CloseTutorialExit();
//			

//		}
//		break;
		
		case UI_CLOSE_WAR_LIST_WINDOW :
			gC_vs_ui.CloseWarListWindow();
			break;

		case UI_CHANGE_OPTION :
		{
			int part = dw_left;
			int value = dw_right;
		}
		break;

		case UI_CLOSE_OPTION :
			gC_vs_ui.CloseOption();
		break;

		case UI_EXCHANGE_REQUEST_CANCEL :
		{
			int a = 0;
		}
		break;

		case UI_EXCHANGE_ACCEPT :
		{
			BOOL accept = (BOOL)dw_left;
		}
		break;

		case UI_STORAGE_BUY :
		{
			BOOL buy = (BOOL)dw_left;
		}
		break;
		//

		//
		// left = (BOOL)bGive
		// right = money
		//
		case UI_EXCHANGE_MONEY:
			{
				BOOL bGive= dw_left;
				int money = dw_right;

				if (bGive)
				{
					g_pMoneyManager->UseMoney( money );
					g_pTradeManager->GetMyMoneyManager()->AddMoney( money );
				}
				else
				{
					g_pMoneyManager->AddMoney( money );
					g_pTradeManager->GetMyMoneyManager()->UseMoney( money );
				}
			}
			break;

		case UI_ITEM_SELECT_EXCHANGE :
			{
				MItem* p_item = (MItem*)void_ptr;

				// trade mode toggle
				if (p_item->IsTrade())
				{
					p_item->UnSetTrade();

					g_pTradeManager->RefuseMyTrade();
				}
				else
				{
					p_item->SetTrade();

					g_pTradeManager->RefuseMyTrade();
				}	
			}
			break;

		case UI_OK_EXCHANGE:
			{
				g_pTradeManager->AcceptMyTrade();
			}
			break;

		case UI_CANCEL_EXCHANGE:
			{
				g_pTradeManager->RefuseMyTrade();
			}
			break;


		case UI_CLOSE_EXCHANGE :
			gC_vs_ui.CloseExchange();
			break;

		
		//

		//
		//	left = money
		//
		case UI_DEPOSIT_MONEY :
			{
				int a = 0;
			}
			break;

		//

		//
		//	left = money
		//
		case UI_WITHDRAW_MONEY :
			{
				int a = 0;
			}
			break;

		//

		//
		// left = tab (0, 1, 2)
		// right = slot
		//
		case UI_SELECT_STORAGE_SLOT:
			{
				int z = 0;
			}
			break;

		case UI_CLOSE_STORAGE:
			gC_vs_ui.CloseStorage();
			break;

		case UI_CLOSE_PETSTORAGE:
			gC_vs_ui.ClosePetStorage();
			break;

//		case UI_CLOSE_SKILL_VIEW:
//			gC_vs_ui.CloseSkillView();
//			break;

		//

		//
		case UI_RUNNING_GAMEMENU:
			break;


		case UI_FINISH_LEVELUP_BUTTON:
			gC_vs_ui.FinishLevelUp();
			break;

		case UI_FINISH_REQUEST_DIE_BUTTON:
			gC_vs_ui.FinishRequestDie();
			break;

		case UI_FINISH_REQUEST_PARTY_BUTTON:
			gC_vs_ui.FinishRequestParty();
			break;

		case UI_SELL_ITEM:
			//
			// void_ptr = item
			//
			break;

		case UI_SELL_ALL_ITEM:
			{
				int z = 0;
			}
			break;

		case UI_ITEM_SELL_FINISHED:
			gC_vs_ui.FinishItemSelling();
			break;

		case UI_REPAIR_ITEM:
			//
			// void_ptr = item
			//
			{
				int  z=0;
			}
			break;

		case UI_SILVERING_ITEM:
			//
			// void_ptr = item
			//
			{
				int  z=0;
			}
			break;

		case UI_ITEM_REPAIR_FINISHED:
 			gC_vs_ui.FinishItemRepairing();
			break;

		case UI_ITEM_SILVERING_FINISHED:
 			gC_vs_ui.FinishItemSilvering();
			break;

		case UI_CLOSE_OTHER_INFO :
			gC_vs_ui.CloseOtherInfo();
			break;

		case UI_CLICK_BONUS_POINT:
			//

			//

			//
			// left = { 0:STR, 1:DEX, 2:INT }
			//
			if (g_char_slot_ingame.bonus_point > 0)
			{
				g_char_slot_ingame.bonus_point--;
				switch (dw_left)
				{
					case 0:
						g_char_slot_ingame.STR_PURE++;
						g_char_slot_ingame.STR_CUR++;
						g_char_slot_ingame.STR_MAX++;
						break;

					case 1:
						g_char_slot_ingame.DEX_PURE++;
						g_char_slot_ingame.DEX_CUR++;
						g_char_slot_ingame.DEX_MAX++;
						break;

					case 2:
						g_char_slot_ingame.INT_PURE++;
						g_char_slot_ingame.INT_CUR++;
						g_char_slot_ingame.INT_MAXX++;
						break;
				}
			}
			break;

		case UI_INFO:
			//

			//
			if (gC_vs_ui.IsRunningCharInfo())
			{
				gC_vs_ui.CloseInfo();
			}
			else
			{
				// set slot


				g_char_slot_ingame.sz_name = "쑥갓";
				g_char_slot_ingame.sz_guild_name = "";
				g_char_slot_ingame.Race = g_eRaceInterface;
				g_char_slot_ingame.bl_drained = true;
				g_char_slot_ingame.MP_MAX = 100;
				g_char_slot_ingame.HP_MAX = 100;
				g_char_slot_ingame.HP = 100;
				g_char_slot_ingame.MP = 100;
				g_char_slot_ingame.FAME = 1234567890;
				g_char_slot_ingame.alignment = UI_GOOD_PLUS;
				g_char_slot_ingame.alignment_num = -10000;
				g_char_slot_ingame.TOHIT = 123;
				g_char_slot_ingame.DAM = 223;
				g_char_slot_ingame.DAM2 = 123;
				g_char_slot_ingame.SILVER_DAM = 3;
				g_char_slot_ingame.SILVER_DAM2 = 1;
				g_char_slot_ingame.STR_CUR = 6;
				g_char_slot_ingame.STR_PURE = 6;
				g_char_slot_ingame.STR_MAX = 6;
				g_char_slot_ingame.STR_EXP_REMAIN = 200500;
				g_char_slot_ingame.DEX_CUR = 6;
				g_char_slot_ingame.DEX_PURE = 6;
				g_char_slot_ingame.DEX_MAX = 6;
				g_char_slot_ingame.DEX_EXP_REMAIN = 300000;
				g_char_slot_ingame.INT_CUR = 6;
				g_char_slot_ingame.INT_PURE = 6;
				g_char_slot_ingame.INT_MAXX = 6;
				g_char_slot_ingame.INT_EXP_REMAIN = 350000;
				g_char_slot_ingame.level = 1;
				g_char_slot_ingame.EXP_REMAIN = 400000;
				g_char_slot_ingame.SILVER_HP = 0;
				g_char_slot_ingame.GUILD_ID=102;
				g_char_slot_ingame.STATUS.clear();
				g_char_slot_ingame.m_SMS_Charge = 10;
				

				S_SLOT::UI_EFFECTSTATUS_STRUCT efs;
				efs.actionInfo = SKILL_BLOOD_DRAIN;
				efs.delayFrame = 10;
				g_char_slot_ingame.STATUS.push_back(efs);
				efs.actionInfo = MAGIC_GREEN_POISON;
				efs.delayFrame = 10;
				g_char_slot_ingame.STATUS.push_back(efs);
				efs.actionInfo = MAGIC_YELLOW_POISON;
				efs.delayFrame = 10;
				g_char_slot_ingame.STATUS.push_back(efs);
				efs.actionInfo = MAGIC_DARKBLUE_POISON;
				efs.delayFrame = 10;
				g_char_slot_ingame.STATUS.push_back(efs);
				efs.actionInfo = MAGIC_GREEN_STALKER;
				efs.delayFrame = 10;
				g_char_slot_ingame.STATUS.push_back(efs);
				g_char_slot_ingame.GRADE=40;
				//g_char_slot_ingame.STATUS.push_back(6);
				//g_char_slot_ingame.STATUS.push_back(7);
				//g_char_slot_ingame.STATUS.push_back(8);
				//g_char_slot_ingame.STATUS.push_back(9);
				g_char_slot_ingame.bl_female = true;//false;
				g_char_slot_ingame.woman_info.helmet = W_OLD_HELMET;
				g_char_slot_ingame.woman_info.coat = W_OLD_COAT;
				g_char_slot_ingame.woman_info.trouser = W_OLD_TROUSER;
				g_char_slot_ingame.woman_info.hair = W_HAIR1;
				g_char_slot_ingame.woman_info.left = W_SHIELD;//M_TR;//M_SWORD;
				g_char_slot_ingame.woman_info.right = W_CROSS;//W_SWORD;//W_DRAGON_SHIELD;
				g_char_slot_ingame.skin_color = 0;
				g_char_slot_ingame.hair_color = 0;
				g_char_slot_ingame.left_color = 405;
				g_char_slot_ingame.right_color = 405;  
				g_char_slot_ingame.helmet_color = 0;
				g_char_slot_ingame.trouser_color = 0;
				g_char_slot_ingame.coat_color = 0;
				g_char_slot_ingame.bonus_point = 5;
				g_char_slot_ingame.skill_point = 50;

				gC_vs_ui.RunInfo();
			}
			break;

//		case UI_MINIMAP_TOGGLE:
//			break;

		case UI_DROP_MONEY:
			//

			//
			// left = money
			//
			break;

		case UI_CHECK_EXIST_ID:
			//
			// (char *)void_ptr = id string
			//
			DeleteNewArray(void_ptr);

			gC_vs_ui.NoAleadyExistIdMessage();
			break;
/*
		case UI_CHANGE_PCS_CONNECTED_SLOT:
			//

			//
			// left = pcs number
			// right = slot 
			//
			gC_vs_ui.GrantPCSWaitRoomToDuplex(dw_right);
			break;

		case UI_PLEASE_PCS_CONNECT_ME:
			//

			//
			// left = pcs number
			// right = slot 
			//


			gC_vs_ui.GrantPCSWaitRoomToDuplex(dw_right);
			break;

		//case UI_PCS_CONNECTOR_GRANTED:
			//

			//
			// left = pcs number
			// right = slot 
			//
		//	break;
*/
		case UI_CHAT_RETURN:

//			if (dw_right)
//			{

//			}
//			else
			{
				gC_vs_ui.AddChatToHistory((char *)void_ptr, "다크에덴짱", (CHAT_LINE_CONDITION)dw_left, dw_right);
			}

			DeleteNewArray(void_ptr);
			break;

		case UI_RUN_NEWUSER_REGISTRATION:
//			gC_vs_ui.RunNewUserRegistration();
			break;

		case UI_INFO_CLOSE:
			gC_vs_ui.CloseInfo();
			break;

		case UI_GAMEMENU_CONTINUE:
			gC_vs_ui.CloseGameMenu();
			break;
/*
		case UI_END_PCS:
			break;

		case UI_SEND_PCS_NUMBER:
			//
			// left = pcs number
			//

			//

			//
			gC_vs_ui.OnLinePCS(NULL, dw_left);


			break;

		case UI_PDS_CLOSED:
			//

			//
			gC_vs_ui.AcquirePDSClosedMessage();
			break;

		case UI_QUIT_PCS_ONLINE_MODE:
			//

			//
			gC_vs_ui.AcquireQuitPCSOnlineModeMessage();
			break;

		case UI_PLEASE_SET_SLAYER_VALUE:
			//C_VS_UI_SLAYER_PDS::m_str = 0;
			//C_VS_UI_SLAYER_PDS::m_dex;
			//C_VS_UI_SLAYER_PDS::m_int;
			//C_VS_UI_SLAYER_PDS::m_hp;
			//C_VS_UI_SLAYER_PDS::m_mp;
			//C_VS_UI_SLAYER_PDS::m_th;
			//C_VS_UI_SLAYER_PDS::m_ac;
			//C_VS_UI_SLAYER_PDS::m_cc;
			//C_VS_UI_SLAYER_PDS::m_str_max;
			//C_VS_UI_SLAYER_PDS::m_dex_max;
			//C_VS_UI_SLAYER_PDS::m_int_max;
			//C_VS_UI_SLAYER_PDS::m_hp_max;
			//C_VS_UI_SLAYER_PDS::m_mp_max;
			//C_VS_UI_SLAYER_PDS::m_th_max;
			//C_VS_UI_SLAYER_PDS::m_ac_max;
			//C_VS_UI_SLAYER_PDS::m_cc_max;
			break;
*/
//		case UI_CLOSE_SKILL_TREE:
//			gC_vs_ui.CloseSkillTree();
//			break;

//		case UI_LEARN_SLAYER_SKILL:
			//

			//
			// left = ACTIONINFO
			//
//			break;
			
		case UI_REMOVE_BACKGROUND_MOUSE_FOCUS:
			{ int z = 0; }
			break;

		case UI_BACKGROUND_MOUSE_FOCUS:
			//
			// (left, right) = mouse (x, y)
			//
			{ int z = 0; }
			break;

		case UI_BUY_ITEM:
			//
			// void_ptr = MShop
			// dw_left = slot
			// dw_right = item count
			//
			{
				int z =0;
			}
			break;

		case UI_CLOSE_SHOP:
			gC_vs_ui.CloseShop();
			break;

		case UI_CHARACTER_MANAGER_FINISHED:
			gC_vs_ui.StartServerSelect(false);
			{
				gC_vs_ui.StartServerSelect(false);
				LPSTR server_name[20] = {"윤일섭", "이창섭", "백일섭", "광섭", "종섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "빽섭"};
				int server_id[20] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
				
				int server_status[20] = {0, 1, 2, 3, 4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
				gC_vs_ui.SetServerList(server_name, server_id, server_status, 5, 0);
			}
			break;

		case UI_ITEM_INSERT_FROM_INVENTORY:
			//
			// dw_left	= gird_x
			// dw_right = grid_y


			//
			{ int z = 0; }
			break;

		case UI_ITEM_INSERT_FROM_GEAR:
			//
			// dw_left	= slot

			//
			{ int z = 0; }
			break;

		case UI_CLOSE_BBS:

			//gC_vs_ui.CloseBBS();
			break;
		case UI_CLOSE_INPUT_NAME :
			gC_vs_ui.CloseInputNameWindow();
			break;
		case UI_GO_BILING_PAGE :
			if(dw_left == TRUE)
			{							 	
    #ifdef PLATFORM_WINDOWS
				char str[256];
				
				GetWindowsDirectory(
					str,  // address of buffer for Windows directory
					255        // size of directory buffer
					);
				
				sprintf(str, "%s\\Explorer.exe", str);
				
				CSDLGraphics::GetDD()->RestoreDisplayMode();
				
				_spawnl(_P_NOWAIT, str, "Explorer.exe", "http://", NULL);
			}
			break;

		case UI_ITEM_DROP_TO_GEAR:
			{
				MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();
				
//				if (gC_vs_ui.IsRunningExchange())
//				{

//					g_pTradeManager->RefuseMyTrade();
    #else
    	// macOS/Linux: Use system() to open browser
    	system("xdg-open http:// 2>/dev/null || open http:// 2>/dev/null");
    #endif
//				}
				
				MItem* pOldItem = NULL;

				MPlayerGear *pGear = NULL;
//				switch(g_eRaceInterface)
//				{
//				case RACE_SLAYER:
//					pGear = g_pSlayerGear;
//					break;
//
//				case RACE_VAMPIRE:
//					pGear = g_pVampireGear;
//					break;
//
//				case RACE_OUSTERS:
//					pGear = g_pOustersGear;
//					break;
//				}
//
//				{



//					{

//						{				
//							gC_vs_ui.PickUpItem( pOldItem );
//						}
//						else
//						{
//							gC_vs_ui.DropItem();
//						}
//					}
//				}
				switch(g_eRaceInterface)
				{
				case RACE_SLAYER:
					if (g_pSlayerGear->ReplaceItem(pMouseItem,
						dw_left,
						pOldItem))
					{
						if (pOldItem != NULL)
						{				
							gC_vs_ui.PickUpItem( pOldItem );
						}
						else
						{
							gC_vs_ui.DropItem();
						}
					}
					break;

				case RACE_VAMPIRE:
					if (g_pVampireGear->ReplaceItem(pMouseItem,
						dw_left,
						pOldItem))
					{
						if (pOldItem != NULL)
						{				
							gC_vs_ui.PickUpItem( pOldItem );
						}
						else
						{
							gC_vs_ui.DropItem();
						}
					}			
					break;

				case RACE_OUSTERS:
					if (g_pOustersGear->ReplaceItem(pMouseItem,
						dw_left,
						pOldItem))
					{
						if (pOldItem != NULL)
						{				
							gC_vs_ui.PickUpItem( pOldItem );
						}
						else
						{
							gC_vs_ui.DropItem();
						}
					}		
					break;
				}

			}
			break;
			
		case UI_ITEM_DROP_TO_QUICKSLOT:
			{
				MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();
				
				MItem* pOldItem = NULL;
				if( g_eRaceInterface == RACE_OUSTERS )
				{
					MOustersArmsBand *pQuickSlot = NULL;
					int slot = dw_left;

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
					if( pQuickSlot->ReplaceItem( pMouseItem, slot, pOldItem ) )
					{
						if( pOldItem != NULL )
						{
							gC_vs_ui.PickUpItem( pOldItem );
						} else
						{
							gC_vs_ui.DropItem();
						}
					}
					

				} else				
				if (g_pQuickSlot->ReplaceItem(pMouseItem,
					dw_left, pOldItem))
				{
					if (pOldItem != NULL)
					{				
						gC_vs_ui.PickUpItem( pOldItem );
					}
					else
					{
						gC_vs_ui.DropItem();
					}
				}
			}
			break;
			
		case UI_ITEM_DROP_TO_INVENTORY:
			{
				MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();
				
				if (gC_vs_ui.IsRunningExchange())
				{

					g_pTradeManager->RefuseMyTrade();
				}
				
				MItem* pOldItem = NULL;
				if (g_pInventory->ReplaceItem(pMouseItem,
					dw_left, dw_right,
					pOldItem))
				{
					if (pOldItem != NULL)
					{				
						gC_vs_ui.PickUpItem( pOldItem );
					}
					else
					{
						gC_vs_ui.DropItem();
					}
				}
			}
			break;

		case UI_ITEM_TO_QUICKITEMSLOT:
			{
				int a = 0;
			}
			break;
			
		case UI_ITEM_PICKUP_FROM_INVENTORY:
			//


			//
			gC_vs_ui.PickUpItem((MItem *)void_ptr, HIWORD(dw_right), LOWORD(dw_right));
			
			if (gC_vs_ui.IsRunningExchange())
			{

				g_pTradeManager->RefuseMyTrade();
			}

			break;

		case UI_ITEM_PICKUP_FROM_QUICKSLOT:
			//


			//
			gC_vs_ui.PickUpItem((MItem *)void_ptr, HIWORD(dw_right), LOWORD(dw_right));
			break;

		case UI_ITEM_PICKUP_FROM_GEAR:
			//


			//
			gC_vs_ui.PickUpItem((MItem *)void_ptr, HIWORD(dw_right), LOWORD(dw_right));

			switch(g_eRaceInterface)
			{
			case RACE_SLAYER:
				g_pSlayerGear->RemoveItem((MSlayerGear::GEAR_SLAYER)dw_left);
				break;

			case RACE_VAMPIRE:
				g_pVampireGear->RemoveItem((MVampireGear::GEAR_VAMPIRE)dw_left);
				break;

			case RACE_OUSTERS:
				g_pOustersGear->RemoveItem((MOustersGear::GEAR_OUSTERS)dw_left);
				break;
			}

			break;

		case UI_DELETE_CHARACTER:
			//
			// ((DELETE_CHARACTER *)void_ptr) = sz_part1, sz_part2, slot
			//
			DeleteNewArray(((DELETE_CHARACTER *)void_ptr)->sz_part1);
			DeleteNewArray(((DELETE_CHARACTER *)void_ptr)->sz_part2);

			//gC_vs_ui.Invalid_SSN_Message();
			gC_vs_ui.DeleteCharacter(((DELETE_CHARACTER *)void_ptr)->slot);
			break;

		case UI_NEW_CHARACTER:
			//
			// ((C_VS_UI::NEW_CHARACTER *)void_ptr) = sz_name, bl_female, m_STR, m_DEX, m_INT
			//
			{
				gC_vs_ui.NewCharacterCreateOk();

				/*
				C_VS_UI_CHAR::S_SLOT slot;
				slot.sz_name = "KJTINC";
				slot.STR = 1;
				slot.DEX = 2;
				slot.INT = 3;
				slot.AC = 4;
				slot.DAM = 5;
				slot.TOHIT = 6;
				slot.HP = 20;
				slot.MP = 20;*/
			}
			break;

		case UI_TERMINATION:
			PostMessage(g_hWnd, WM_CLOSE, 0, 0);
			break;

		case UI_LOGIN:
			{
				gC_vs_ui.BackupPrevId(((LOGIN *)void_ptr)->sz_id);
				
				DeleteNewArray(((LOGIN *)void_ptr)->sz_id);
				DeleteNewArray(((LOGIN *)void_ptr)->sz_password);

				gC_vs_ui.StartServerSelect(true);
				LPSTR group_name[5] = {"그룹1", "그룹2", "그룹3", "그룹4", "그룹5"};
				int server_id[5] = {0,1,2,3,4};
				
				int server_status[5] = {0, 1, 1, 1, 1};
				gC_vs_ui.SetServerList(group_name, server_id, server_status, 5, 2);
			}
			break;

		case UI_CLOSE_SERVER_SELECT:
			if(dw_left)	//group
			{
				gC_vs_ui.StartTitle();
			}
			else		//server
			{
				gC_vs_ui.StartServerSelect(true);
				LPSTR group_name[5] = {"그룹1", "그룹2", "그룹3", "그룹4", "그룹5"};
				int server_id[5] = {0,1,2,3,4};
				
				int server_status[5] = {0, 1, 1, 1, 1};
				gC_vs_ui.SetServerList(group_name, server_id, server_status, 5, 2);
			}
			break;

		case UI_CONNECT_SERVER:
			if(dw_left)	//group
			{
				gC_vs_ui.StartServerSelect(false);
				LPSTR server_name[20] = {"윤일섭", "이창섭", "백일섭", "광섭", "종섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "백섭", "빽섭"};
				int server_id[20] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
				
				int server_status[20] = {0, 1, 2, 3, 4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
				gC_vs_ui.SetServerList(server_name, server_id, server_status, 5, 0);
			}
			else		//server
			//
			// ((C_VS_UI::LOGIN *)void_ptr) = id, password
			//

			//
			//{
			//char sz_str[200];
			//strcpy(sz_str, ((C_VS_UI::LOGIN *)void_ptr)->sz_id);
			//}
			//Sleep(5000);
			{
			gC_vs_ui.StartCharacterManager();
			gC_vs_ui.ClearAllCharacter();
			// set character
			S_SLOT slot;
			slot.sz_name = "쑥갓";
			slot.sz_guild_name = "드림마스터";
			slot.GUILD_ID=102;
			slot.Race = RACE_SLAYER;
			slot.m_AdvancementLevel = 1;
			slot.MP_MAX = 300;
			slot.HP_MAX = 300;
			slot.HP = 200;
			slot.MP = 100;
			slot.FAME = 1234567890;
			slot.alignment = UI_GOOD_PLUS;
			slot.TOHIT = 22;
			slot.DEFENSE = 33;
			slot.PROTECTION = 44;
			slot.DAM = 223;
			slot.DAM2 = 123;
			slot.STR_CUR = 80;
			slot.STR_PURE = 85;
			slot.STR_MAX = 100;
			slot.STR_EXP_REMAIN = 13796;
			slot.DEX_CUR = 50;
			slot.DEX_PURE = 85;
			slot.DEX_MAX = 100;
			slot.DEX_EXP_REMAIN = 70;
			slot.INT_CUR = 100;
			slot.INT_PURE = 85;
			slot.INT_MAXX = 70;
			slot.INT_EXP_REMAIN = 35365;
			slot.level = 123;
			slot.EXP_REMAIN = 1234567890;
			slot.bl_female = false;//false;
			slot.woman_info.helmet = W_NEW_HELMET;
			slot.woman_info.coat = W_NEW_COAT;
			slot.woman_info.trouser = W_NEW_TROUSER;
			slot.woman_info.face = W_FACE2;
			slot.woman_info.hair = W_HAIR1;
			slot.woman_info.left = W_DRAGON_SHIELD;//M_TR;//M_SWORD;
			slot.woman_info.right = W_MACE;//W_SWORD;//W_DRAGON_SHIELD;
			slot.skin_color = 0;
			slot.hair_color = 0;
			slot.left_color = 405;
			slot.right_color = 405;
			slot.helmet_color = 0xFFFF;
			slot.trouser_color = 0xFFFF;
			slot.coat_color = 0;
			slot.bonus_point = 0;
			slot.GRADE = 25;
			
			gC_vs_ui.SetCharacter(0, slot);
			
			slot.sz_name = "아우소니";
			slot.sz_guild_name = "소니길드";
			slot.GUILD_ID=102;
			slot.m_AdvancementLevel = 1;
			slot.Race = RACE_VAMPIRE;
			slot.MP_MAX = 300;
			slot.HP_MAX = 300;
			slot.HP = 200;
			slot.MP = 100;
			slot.FAME = 1234567890;
			slot.alignment = UI_GOOD_PLUS;
			slot.TOHIT = 22;
			slot.DEFENSE = 33;
			slot.PROTECTION = 44;
			slot.DAM = 223;
			slot.DAM2 = 123;
			slot.STR_CUR = 80;
			slot.STR_PURE = 85;
			slot.STR_MAX = 100;
			slot.STR_EXP_REMAIN = 13796;
			slot.DEX_CUR = 50;
			slot.DEX_PURE = 85;
			slot.DEX_MAX = 100;
			slot.DEX_EXP_REMAIN = 70;
			slot.INT_CUR = 100;
			slot.INT_PURE = 85;
			slot.INT_MAXX = 70;
			slot.INT_EXP_REMAIN = 35365;
			slot.level = 123;
			slot.EXP_REMAIN = 1234567890;
			slot.bl_female = false;
			slot.man_info.right = (CHAR_MAN)0;
			
//			slot.man_info.helmet = M_NEW_HELMET;
//			slot.man_info.coat = M_NEW_COAT;
//			slot.man_info.trouser = M_NEW_TROUSER;
//			slot.man_info.face = M_FACE2;
//			slot.man_info.hair = M_HAIR1;
//			slot.man_info.left = M_DRAGON_SHIELD;//M_TR;//M_SWORD;
//			slot.man_info.right = M_CROSS;//W_SWORD;//W_DRAGON_SHIELD;
			
			slot.woman_info.helmet = W_NEW_HELMET;
			slot.woman_info.coat = W_NEW_COAT;
			slot.woman_info.trouser = W_NEW_TROUSER;
			slot.woman_info.face = W_FACE2;
			slot.woman_info.hair = W_HAIR1;
			slot.woman_info.left = W_DRAGON_SHIELD;//M_TR;//M_SWORD;
			slot.woman_info.right = W_CROSS;//W_SWORD;//W_DRAGON_SHIELD;
			slot.skin_color = 0;
			slot.hair_color = 0;
			slot.left_color = 405;
			slot.right_color = 405;
			slot.helmet_color = 0xFFFF;
			slot.trouser_color = 0;
			slot.coat_color = 0;
			slot.bonus_point = 0;
			slot.GRADE = 40;
			
			gC_vs_ui.SetCharacter(1, slot);
			
			}
			break;

/*		case UI_NEW_USER_REGISTRATION:
			//
			// ((NEW_REGISTRATION *)void_ptr) = ...
			//

			//
			//struct NEW_REGISTRATION
			//{
			//	char *	sz_id;
			//	char *	sz_password;
			//	char *	sz_repassword;
			//	char *	sz_name;
			//	char *	sz_email;
			//	char *	sz_address;


			//	char *	sz_homepage;
			//	char *	sz_woo;
			//	char *	sz_phone;
			//	bool		bl_female;
			//	bool		bl_announce_my_info;
			//};
			//
			gC_vs_ui.BackupPrevId(((NEW_REGISTRATION *)void_ptr)->sz_id);

			DeleteNewArray(((NEW_REGISTRATION *)void_ptr)->sz_id);
			DeleteNewArray(((NEW_REGISTRATION *)void_ptr)->sz_password);
			DeleteNewArray(((NEW_REGISTRATION *)void_ptr)->sz_repassword);
			DeleteNewArray(((NEW_REGISTRATION *)void_ptr)->sz_name);
			DeleteNewArray(((NEW_REGISTRATION *)void_ptr)->sz_email);
			DeleteNewArray(((NEW_REGISTRATION *)void_ptr)->sz_address);
			DeleteNewArray(((NEW_REGISTRATION *)void_ptr)->sz_ssn_number_part1);
			DeleteNewArray(((NEW_REGISTRATION *)void_ptr)->sz_ssn_number_part2);
			DeleteNewArray(((NEW_REGISTRATION *)void_ptr)->sz_homepage);
			DeleteNewArray(((NEW_REGISTRATION *)void_ptr)->sz_woo);
			DeleteNewArray(((NEW_REGISTRATION *)void_ptr)->sz_phone);

			gC_vs_ui.CloseUserRegistrationWindow();
			gC_vs_ui.StartCharacterManager();
			gC_vs_ui.ClearAllCharacter();
			break;
*/
		case UI_CONNECT:
			gC_vs_ui.StartGame();

			//

			//
			//gC_vs_ui.AddSlayerSkill(SKILL_FLASH_SLASHER);
			break;

		case UI_LOGOUT:
			//

			//
			gC_vs_ui.StartTitle();
			break;

		case UI_SELECT_SKILL:
			//
			// dw_left = Skill ID
			//
			break;

		case UI_CANCEL_SELECT_SKILL:
			//

			//
			break;

		case UI_CLOSE_PET_INFO:
			gC_vs_ui.ClosePetInfo();
			break;

		case UI_CLOSE_SMS_MESSAGE:
			if(gC_vs_ui.IsRunningSMSList())
				gC_vs_ui.CloseSMSList();
			if(gC_vs_ui.IsRunningSMSRecord())
				gC_vs_ui.CloseSMSRecord();
			gC_vs_ui.CloseSMSMessage();
			break;
		case UI_SMS_OPEN_LIST:
			gC_vs_ui.RunSMSList();
			break;
		case UI_SMS_RECORD:
			gC_vs_ui.AddSMSAddressList(rand()%100, (char*)dw_left, (char*)dw_right, (char*)void_ptr);
			gC_vs_ui.CloseSMSRecord();
			break;
		case UI_SMS_DELETE:
			gC_vs_ui.DeleteSMSAddressList(dw_left);
			break;
		case UI_CLOSE_SMS_LIST:
			gC_vs_ui.CloseSMSList();
			break;
		case UI_CLOSE_SMS_RECORD:
			gC_vs_ui.CloseSMSRecord();
			break;
		case UI_SMS_ADD_SEND_LIST:
			gC_vs_ui.AddSMSSendList((char*)dw_left);
			break;

		case UI_CLOSE_NAMING:
			gC_vs_ui.CloseNaming();
			break;
		case UI_CHANGE_CUSTOM_NAMING:
		//	gC_vs_ui.Change_Custom_Naming(dw_right, (char*)dw_left);
			break;
		case UI_CLOSE_NAMING_CHANGE:
			gC_vs_ui.CloseNamingChange();
			break;
		case UI_RUN_NAMING_CHANGE:
			gC_vs_ui.RunNamingChange((MItem*)dw_left, "우헤헤^^");
			break;

		case UI_CLOSE_QUEST_MANAGER:
			gC_vs_ui.CloseQuestManager();
			break;
		case UI_CLOSE_QUEST_LIST:
			gC_vs_ui.CloseQuestList();
			break;
		case UI_CLOSE_QUEST_DETAIL:
			gC_vs_ui.CloseQuestDetail();
			break;
		case UI_CLOSE_QUEST_MISSION:
			gC_vs_ui.CloseQuestMission();
			break;
		case UI_CLOSE_QUEST_ITEM:
			gC_vs_ui.CloseQuestItem();
			break;
		case UI_GQUEST_ACCEPT:
			MessageBox(NULL, "수락 보냄", "^^", MB_OK);
			break;
		case UI_GQUEST_GIVEUP:
			MessageBox(NULL, "포기 보냄", "^^", MB_OK);
			break;
		case UI_CLOSE_QUEST_ICON:
			gC_vs_ui.CloseQuestIcon(dw_left);
			break;
		case UI_REQUEST_UNION:
			MessageBox(NULL, "연합길드 요청 메시지 보냅 ", "^^", MB_OK);
			break;
		case UI_REQUEST_UNION_ACCEPT:
			break;
		case UI_REQUEST_UNION_DENY:
			break;
		case UI_REQUEST_UNION_REQUEST_DENY:
			break;
		case UI_REQUEST_UNION_REQUEST_INFO:
			break;
		case UI_REQUEST_UNION_EXPERGUILD:
			MessageBox(NULL, "추방   요청 메시지 보냅 ", "^^", MB_OK);
			break;
		case UI_REQUEST_UNION_QUIT:
			MessageBox(NULL, "즉시 탈퇴  ", "^^", MB_OK);
			break;
		case UI_REQUEST_UNION_QUIT_ACCEPT:
			break;
		case UI_REQUEST_UNION_QUIT_DENY:
			break;
		case UI_CLOSE_POWER_JJANG:
			gC_vs_ui.ClosePowerjjang();
			break;

		case UI_CLOSE_INVENTORY_SUB:
			#ifdef __TEST_SUB_INVENTORY__
				gC_vs_ui.CloseSubInventory();
			#endif
			break;
	}
}

void
ReleaseAllObject()
{
	gC_vs_ui.Release();
	
	if ( g_pQuestInfoManager != NULL)
	{
		delete g_pQuestInfoManager;
		g_pQuestInfoManager = NULL;
	}

	if(g_pFameInfoTable )
	{
		delete g_pFameInfoTable;

		g_pFameInfoTable = NULL;
	}
	
	if(g_pRankBonusTable)
	{
		delete g_pRankBonusTable;
		g_pRankBonusTable = NULL;
	}
	if(g_pZoneTable)
	{
		delete g_pZoneTable;
		g_pZoneTable = NULL;
	}
	if(m_pC_dialog)
	{
		delete m_pC_dialog;
		m_pC_dialog = NULL;
	}
	if(g_pUserInformation)
	{
		delete g_pUserInformation;
		g_pUserInformation = NULL;
	}

	if(g_pSoundManager)
	{
		g_pSoundManager->Release();
		delete g_pSoundManager;
		g_pSoundManager = NULL;
	}

	if(g_pProfileManager)
	{
		g_pProfileManager->DeleteProfiles();
		delete g_pProfileManager;
		g_pProfileManager = NULL;
	}

	if (g_pParty != NULL)
	{
		delete g_pParty;
		g_pParty = NULL;
	}

	if (g_pSoundTable!=NULL)
	{
		delete g_pSoundTable;
		g_pSoundTable = NULL;
	}

	if (m_pC_dialog!=NULL)
	{
		delete m_pC_dialog;
		m_pC_dialog = NULL;
	}

	if (g_pClientConfig!=NULL)
	{
		delete g_pClientConfig;
		g_pClientConfig = NULL;
	}

	if (g_pUserOption!=NULL)
	{
		delete g_pUserOption;
		g_pUserOption = NULL;
	}

	if (g_pNPCTable!=NULL)
	{
		delete g_pNPCTable;
		g_pNPCTable = NULL;
	}

	if (g_pCreatureTable!=NULL)
	{
		delete g_pCreatureTable;
		g_pCreatureTable  = NULL;
	}

	if (g_pSDLInput)
	{
		delete g_pSDLInput;
		g_pSDLInput = NULL;
	}

	if (g_pInventory!=NULL)
	{
		delete g_pInventory;
		g_pInventory = NULL;
	}

	if (g_pSlayerGear!=NULL)
	{
		delete g_pSlayerGear;
		g_pSlayerGear = NULL;
	}

	if (g_pVampireGear!=NULL)
	{
		delete g_pVampireGear;
		g_pVampireGear = NULL;
	}

	if( g_pOustersGear != NULL)
	{
		delete g_pOustersGear;
		g_pOustersGear = NULL;
	}

	if (g_pGuildInfoMapper!=NULL)
	{
		g_pGuildInfoMapper->Release();
		delete g_pGuildInfoMapper;
		g_pGuildInfoMapper = NULL;
	}

	if (g_pGuildMarkManager!=NULL)
	{
		delete g_pGuildMarkManager;
		g_pGuildMarkManager = NULL;
	}

	if (g_pAcceleratorManager!=NULL)
	{
		delete g_pAcceleratorManager;
		g_pAcceleratorManager = NULL;
	}

	if (g_pKeyAccelerator!=NULL)
	{
		delete g_pKeyAccelerator;
		g_pKeyAccelerator = NULL;
	}

	if (g_pItemTable!=NULL)
	{
		delete g_pItemTable;
		g_pItemTable = NULL;
	}

	if (g_pItemOptionTable!=NULL)
	{
		delete g_pItemOptionTable;
		g_pItemOptionTable = NULL;
	}

	if (g_pMoneyManager!=NULL)
	{
		delete g_pMoneyManager;
		g_pMoneyManager = NULL;
	}

	if (g_pPriceManager!=NULL)
	{
		delete g_pPriceManager;
		g_pPriceManager = NULL;
	}

	if (g_pSkillInfoTable!=NULL)
	{
		delete g_pSkillInfoTable;
		g_pSkillInfoTable = NULL;
	}

	if (g_pSkillManager!=NULL)
	{
		delete g_pSkillManager;
		g_pSkillManager = NULL;
	}

	if (g_pExperienceTable!=NULL)
	{
		delete g_pExperienceTable;
		g_pExperienceTable = NULL;
	}

	if (g_pSkillAvailable!=NULL)
	{
		delete g_pSkillAvailable;
		g_pSkillAvailable = NULL;
	}

	if (  g_pTimeItemManager != NULL)
	{
		delete g_pTimeItemManager;
		g_pTimeItemManager = NULL;
	}

//	if( g_pGameStringTable != NULL )
//	{
//		delete g_pGameStringTable;
//		g_pGameStringTable = NULL;
//	}
	if(g_pNickNameStringTable != NULL)
	{
		g_pNickNameStringTable->Release();
		DeleteNew(g_pNickNameStringTable);
	}

	DeleteNew(g_pTradeManager);
	DeleteNew(g_pRegenTowerInfoManager);
	DeleteNew(g_pFileDef);
}

/*-----------------------------------------------------------------------------
- WinMain
- Windows program entry point
-----------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode)
{
// for Memory Leak Detecting

	int tmpDbgFlag;
	tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);

	HWND hwnd;
	MSG	msg;
	WNDCLASSEX wcl;

	g_pFileDef = new Properties;
	g_pFileDef->load(FILE_INFO_FILEDEF);
	//-------------------------------------

	//-------------------------------------	
	g_pClientConfig = new ClientConfig;
//		g_pClientConfig->Use3DHAL			= TRUE;
	
	
	g_pUserOption = new UserOption;
	g_pUserOption->Use3DHAL				= TRUE;
	g_pUserOption->UseSmoothCursor		= FALSE;//TRUE;
	g_pUserOption->DrawMinimap			= FALSE;
	g_pUserOption->DrawZoneName			= TRUE;
	g_pUserOption->DrawGameTime			= TRUE;
	g_pUserOption->DrawInterface			= FALSE;		// interface
	g_pUserOption->DrawFPS				= FALSE;		// FPS
	g_pUserOption->BlendingShadow			= FALSE;
	g_pUserOption->FilteringCurse		= TRUE;
	g_pUserOption->PlayMusic			= TRUE;
	g_pUserOption->PlaySound			= TRUE;
	g_pUserOption->VolumeMusic			= 10;
	g_pUserOption->VolumeSound			= 10;
	g_pUserOption->UseEnterChat			= true;
	
	g_pGameStringTable = new MStringArray;	
	InitGameStringTable();

	g_pRegenTowerInfoManager = new RegenTowerInfoManager;
	g_pRegenTowerInfoManager->LoadRegenTowerInfo();


	g_pInventory = new MInventory;
	g_pSlayerGear = new MSlayerGear;
	g_pVampireGear = new MVampireGear;
	g_pOustersGear = new MOustersGear;
	g_pTimeItemManager = new MTimeItemManager;

	g_pTimeItemManager->AddTimeItem( 0, 70 + (60*60));

	g_pGuildMarkManager = new MGuildMarkManager(SPK_GUILD_MARK, FILE_INFO_GUILD_MAPPER,SPK_GRADE_SLAYER,SPK_GRADE_VAMPIRE,SPK_GRADE_OUSTERS,SPK_LEVEL_MARK);
	g_pGuildInfoMapper = new MGuildInfoMapper;

	ifstream guildFile(FILE_INFO_GUILD_MAPPER, ios::binary);
	g_pGuildInfoMapper->LoadFromFile(guildFile);
	guildFile.close();

	g_pAcceleratorManager = new AcceleratorManager;
	g_pKeyAccelerator = new KeyAccelerator;
	g_pKeyAccelerator->Init( MAX_ACCELERATOR );
	SetDefaultAccelerator();

	g_pSystemAvailableManager = new SystemAvailabilitiesManager;

	g_pItemTable = new ITEMCLASS_TABLE;
	ifstream itemFile(FILE_INFO_ITEM_INFO , ios::binary);
	g_pItemTable->LoadFromFile( itemFile );
	itemFile.close();

	g_pNPCTable = new MNPCTable;
	ifstream npcFile(FILE_INFO_NPC_INFO , ios::binary);
	g_pNPCTable->LoadFromFile( npcFile );
	npcFile.close();

	g_pCreatureTable = new CREATURE_TABLE;
	ifstream creatureFile(FILE_INFO_CREATURE_INFO , ios::binary);
	g_pCreatureTable->LoadFromFile( creatureFile );
	creatureFile.close();

	g_pZoneTable = new CZoneTable;
	ifstream zoneFile(FILE_INFO_ZONE_TABLE , ios::binary);
	g_pZoneTable->LoadFromFile( zoneFile );
	zoneFile.close();

	g_pRankBonusTable = new RankBonusTable;
	ifstream rankFile(FILE_INFO_RANK_BONUS_TABLE , ios::binary);
	g_pRankBonusTable->LoadFromFile( rankFile );
	rankFile.close();

	g_pItemOptionTable = new ITEMOPTION_TABLE;
	g_pMoneyManager = new MMoneyManager;
	g_pPriceManager = new MPriceManager;


	g_pSDLInput = new CDirectInput;

	g_pParty = new MParty;

	g_pQuestInfoManager = new MQuestInfoManager;
	DWORD ver;

	class ifstream questinfo("data\\info\\questinfo.inf",ios::binary);
	questinfo.read((char*)&ver, sizeof( DWORD ) );
	g_pQuestInfoManager->LoadFromFile( questinfo );
	questinfo.close();

	//---------------------------------------------------------------------
	// nick name string table Loading
	//---------------------------------------------------------------------
	g_pNickNameStringTable = new MStringArray;
	class ifstream gameStringTableTable("data\\info\\Nickname.inf", ios::binary);
	if(false == (*g_pNickNameStringTable).LoadFromFile_NickNameString(gameStringTableTable))
		MessageBox(NULL, "File read Error - NickName.inf", "Error", MB_OK|MB_ICONERROR);
	gameStringTableTable.close();

	
	//---------------------------------------------------------------------
	//

	//
	//---------------------------------------------------------------------
	g_pSkillInfoTable = new MSkillInfoTable;
	g_pSkillInfoTable->Init();

	//------------------------------------------------

	//------------------------------------------------
	class ifstream serverSkillInfoFile(FILE_INFO_SKILL_INFO, ios::binary);
	g_pSkillInfoTable->LoadFromFileServerSkillInfo( serverSkillInfoFile );
	serverSkillInfoFile.close();

	//---------------------------------------------------------------------
	//

	//
	//---------------------------------------------------------------------

	g_pSkillManager = new MSkillManager;		
	g_pSkillManager->Init();

	//------------------------------------------------

	//------------------------------------------------
	class ifstream serverDomainInfoFile(FILE_INFO_SKILL_DOMAIN_EXP, ios::binary);
	g_pSkillManager->LoadFromFileServerDomainInfo( serverDomainInfoFile );
	serverDomainInfoFile.close();


	//---------------------------------------------------

	//---------------------------------------------------
	g_pFameInfoTable = new FameInfoTable;
	class ifstream FameLimit( "Data\\Info\\FameLimit.inf", ios::binary );
	g_pFameInfoTable->LoadFromFile( FameLimit );
	FameLimit.close();
	
	//---------------------------------------------------------------------
	//

	//
	//---------------------------------------------------------------------
	
	g_pExperienceTable = new ExperienceTable;

	//------------------------------------------------
	// Load
	//------------------------------------------------
	class ifstream strExpFile(FILE_INFO_STR_EXP, ios::binary);
	class ifstream dexExpFile(FILE_INFO_DEX_EXP, ios::binary);
	class ifstream intExpFile(FILE_INFO_INT_EXP, ios::binary);
	class ifstream vampireExpFile(FILE_INFO_VAMPIRE_EXP, ios::binary);
	class ifstream oustersExpFile(FILE_INFO_OUSTERS_EXP, ios::binary);
	class ifstream slayerRankExp(FILE_INFO_SLAYER_RANK_EXP, ios::binary);
	class ifstream vampireRankExp(FILE_INFO_VAMPIRE_RANK_EXP, ios::binary);
	class ifstream oustersRankExp(FILE_INFO_OUSTERS_RANK_EXP, ios::binary);
	class ifstream advancementExp(FILE_INFO_ADVANCEMENT_EXP, ios::binary);
	class ifstream petExpInfo("DATA\\INFO\\PETEXP.INF", ios::binary);

	g_pExperienceTable->LoadFromFileSTR(strExpFile);
	g_pExperienceTable->LoadFromFileDEX(dexExpFile);
	g_pExperienceTable->LoadFromFileINT(intExpFile);
	g_pExperienceTable->LoadFromFileVampire(vampireExpFile);
	g_pExperienceTable->LoadFromFileOusters(oustersExpFile);
	g_pExperienceTable->LoadFromFileSlayerRank(slayerRankExp);
	g_pExperienceTable->LoadFromFileVampireRank(vampireRankExp);
	g_pExperienceTable->LoadFromFileOustersRank(oustersRankExp);
	g_pExperienceTable->LoadFromFilePetExp( petExpInfo );
	g_pExperienceTable->LoadFromFileAdvanceMent( advancementExp);
	
	strExpFile.close();
	dexExpFile.close();
	intExpFile.close();
	vampireExpFile.close();
	oustersExpFile.close();
	slayerRankExp.close();
	vampireRankExp.close();
	oustersRankExp.close();
	petExpInfo.close();
	advancementExp.close();

	
//	g_pSkillInfoTable = new MSkillInfoTable;
//	g_pSkillManager = new SKILLDOMAIN_TABLE;
//	g_pSkillManager = new MSkillManager;
	g_pSkillAvailable = new MSkillSet;

	g_pTradeManager = new MTradeManager;
	g_pTradeManager->Init();
	g_pTradeManager->SetOtherName( "필살교환맨" );
	g_pTradeManager->GetOtherMoneyManager()->SetMoney( 42566 );
	g_pTradeManager->GetMyMoneyManager()->SetMoney( 13344 );
	

	CoInitialize(NULL);
	
	HRESULT hr;
	if (hr = FAILED(CoCreateInstance(CLSID_InternetExplorer,NULL,
		CLSCTX_LOCAL_SERVER, IID_IWebBrowser2,(LPVOID*)&g_pWebBrowser))) 
	{
		MessageBox(NULL, "IWebBrowser2 Create failed", "ERROR", MB_OK);
		return 0;
	}

	//


	//

	//
	wcl.hInstance = hInst;
	wcl.lpszClassName = CLASSNAME;
	wcl.lpfnWndProc = WindowProc;
	//
	// `wcl.style =	CS_HREDRAW | CS_VREDRAW;


	//
	wcl.style =	CS_HREDRAW;// | CS_VREDRAW;
	wcl.cbSize = sizeof(WNDCLASSEX);
	wcl.hIcon = LoadIcon(NULL, IDI_WINLOGO); 
	wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = NULL; //MAKEINTRESOURCE(KJTMENU);
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	RegisterClassEx(&wcl);

#ifndef _FULLSCREEN
//* // Window mode
    // Calculate the proper size for the window given a client of 640x480
    int cx = RESOLUTION_X+GetSystemMetrics(SM_CXSIZEFRAME)*2+2;
    int cy = RESOLUTION_Y+GetSystemMetrics(SM_CYSIZEFRAME)*2+GetSystemMetrics(SM_CYMENU)+2;

	hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
									CLASSNAME,
									TITLEBARNAME,
									WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW  & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX ,
									0, 
									0,
									cx,
									cy,
									HWND_DESKTOP,
									NULL,
									hInst,
									NULL);
//	*/
#else
///*	// Fullscreen mode
	hwnd = CreateWindowEx(WS_EX_TOPMOST ,
									CLASSNAME,
									TITLEBARNAME,
									WS_POPUP|WS_CLIPCHILDREN| WS_CLIPSIBLINGS,
									0, 
									0,
									GetSystemMetrics(SM_CXSCREEN),
									GetSystemMetrics(SM_CXSCREEN),
									HWND_DESKTOP,
									NULL,
									hInst,
									NULL);
//*/
#endif

	if (!hwnd) return 0;

	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);
   SetFocus(hwnd);

	g_hWnd = hwnd;

	InitSound();



#ifndef _FULLSCREEN
	//
	// Init DirectDraw
	//
	if (!gC_DD.Init(hwnd, RESOLUTION_X, RESOLUTION_Y, CSDLGraphics::WINDOWMODE, false, true))
		return 0;

	//gC_DD.Init( hwnd, RESOLUTION_X, RESOLUTION_Y, CSDLGraphics::WINDOWMODE);
#else
	if (!gC_DD.Init(hwnd, RESOLUTION_X, RESOLUTION_Y, CSDLGraphics::FULLSCREEN, false, true))
		return 0;
#endif

#if _DEBUGGING
	g_pSDLInput->Init(hwnd, hInst, CSDLInput::NONEXCLUSIVE);
#else
	g_pSDLInput->Init(hwnd, hInst, CSDLInput::EXCLUSIVE);
#endif

	//gC_ci = new CI_KOREAN;	
	gC_ci = new CI_CHINESE;	
	

	g_pSDLInput->SetMouseEventReceiver(MouseEventReceiver);
	g_pSDLInput->SetMouseMoveLimit(RESOLUTION_X-1, RESOLUTION_Y-1);
	g_pSDLInput->SetKeyboardEventReceiver(KeyboardEventReceiver);

	InitializeGL(CSDLGraphics::Get_BPP(), 
					 CSDLGraphics::Get_Count_Rbit(), 
					 CSDLGraphics::Get_Count_Gbit(), 
					 CSDLGraphics::Get_Count_Bbit());
//	gC_font.Initialize();

	gC_DDSurface.InitBacksurface();

	CIndexSprite::SetColorSet();

	CSpriteSurface::InitEffectTable();
	gC_vs_ui.Init(&gC_DDSurface, UI_ResultReceiver);

	
	g_pProfileManager = new ProfileManager;
	g_pProfileManager->InitProfiles();
	
	g_pUserInformation = new UserInformation;
	WAR_INFO s;
	s.left_time = 7200+(timeGetTime()/1000);
	s.war_type = 0;
	s.zone_id = 1204;
	s.zone_name = "쿠아르투스";
	g_pUserInformation->WarInfo.push_back(s);

	WAR_INFO ss;
	ss.left_time = 3600+212+(timeGetTime()/1000);
	ss.war_type = 0;
	ss.zone_id = 1201;
	ss.zone_name = "쿠아르투스a";
	g_pUserInformation->WarInfo.push_back(ss);

	g_char_slot_ingame.m_AdvancementLevel = 10;
	
	//	gC_vs_ui.StartProgress();
	//	gC_vs_ui.SetProgressStyle(SLAYER_PROGRESS_1); // default 'SLAYER_PROGRESS'
	//
	//	for (int i=0; i<=300; i++)
	//	{
	//		if (i==0)
	//		{
	//			CSpriteSurface* pSurface = new CSpriteSurface;
	//			
	//			pSurface->InitOffsurface(1056, 774);
	//
	//			pSurface->SetTransparency( 0 );
	//
	//			delete pSurface;
	//		}
	//
	//		gC_vs_ui.SetProgress(i, 300);
	//		gC_vs_ui.Show();
	//		gC_DD.Flip();
	//	}
	
	//	gC_vs_ui.EndProgress();

//	g_pUserInformation->IsNetmarble = false;
	

	g_char_slot_ingame.sz_name = "쑥갓";
	g_char_slot_ingame.sz_guild_name = "마스터라니까";
	g_char_slot_ingame.GUILD_ID=102;
	g_char_slot_ingame.Race = RACE_SLAYER;
	g_char_slot_ingame.bl_drained = true;
	g_char_slot_ingame.MP_MAX = 100;
	g_char_slot_ingame.HP_MAX = 100;
	g_char_slot_ingame.HP = 100;
	g_char_slot_ingame.MP = 230;
	g_char_slot_ingame.FAME = 1234567890;
	g_char_slot_ingame.alignment = UI_GOOD_PLUS;
	g_char_slot_ingame.alignment_num = -10000;
	g_char_slot_ingame.TOHIT = 123;
	g_char_slot_ingame.DAM = 223;
	g_char_slot_ingame.DAM2 = 123;
	g_char_slot_ingame.SILVER_DAM = 3;
	g_char_slot_ingame.SILVER_DAM2 = 1;
	g_char_slot_ingame.STR_CUR = 120;
	g_char_slot_ingame.STR_PURE = 100;
	g_char_slot_ingame.STR_MAX = 130;
	g_char_slot_ingame.STR_EXP_REMAIN = 500;
	g_char_slot_ingame.DEX_CUR = 6;
	g_char_slot_ingame.DEX_PURE = 6;
	g_char_slot_ingame.DEX_MAX = 6;
	g_char_slot_ingame.DEX_EXP_REMAIN = 300;
	g_char_slot_ingame.INT_CUR = 6;
	g_char_slot_ingame.INT_PURE = 6;
	g_char_slot_ingame.INT_MAXX = 6;
	g_char_slot_ingame.INT_EXP_REMAIN = 350;
	g_char_slot_ingame.level = 1;
	g_char_slot_ingame.EXP_REMAIN = 1000000;
	g_char_slot_ingame.SILVER_HP = 0;
	g_char_slot_ingame.GRADE=36;
	g_char_slot_ingame.WS = 0;
	g_char_slot_ingame.WeaponSpeed = 30;
	g_char_slot_ingame.STATUS.clear();
	g_char_slot_ingame.m_SMS_Charge = 10;
	S_SLOT::UI_EFFECTSTATUS_STRUCT efs;
	efs.actionInfo = SKILL_BLOOD_DRAIN;
	efs.delayFrame = timeGetTime()+360000;
	g_char_slot_ingame.STATUS.push_back(efs);
	efs.actionInfo = MAGIC_GREEN_POISON;
	efs.delayFrame = timeGetTime()+360000;
	g_char_slot_ingame.STATUS.push_back(efs);
	efs.actionInfo = MAGIC_YELLOW_POISON;
	efs.delayFrame = timeGetTime()+3700;
	g_char_slot_ingame.STATUS.push_back(efs);
	efs.actionInfo = MAGIC_DARKBLUE_POISON;
	efs.delayFrame = timeGetTime()+80;
	g_char_slot_ingame.STATUS.push_back(efs);
	efs.actionInfo = MAGIC_GREEN_STALKER;
	efs.delayFrame = timeGetTime()+600;
	g_char_slot_ingame.STATUS.push_back(efs);

	efs.actionInfo = SKILL_CLIENT_CAN_ENTER_GDR_LAIR;
	efs.delayFrame = timeGetTime()+8000;
	g_char_slot_ingame.STATUS.push_back(efs);
	//				g_char_slot_ingame.STATUS.push_back(MAGIC_GREEN_STALKER);
	//				g_char_slot_ingame.STATUS.push_back(MAGIC_GREEN_STALKER);
	//				g_char_slot_ingame.STATUS.push_back(MAGIC_GREEN_STALKER);
	//				g_char_slot_ingame.STATUS.push_back(MAGIC_GREEN_STALKER);
	//				g_char_slot_ingame.STATUS.push_back(MAGIC_GREEN_STALKER);
	//				g_char_slot_ingame.STATUS.push_back(MAGIC_GREEN_STALKER);
	//				g_char_slot_ingame.STATUS.push_back(MAGIC_GREEN_STALKER);
	//				g_char_slot_ingame.STATUS.push_back(MAGIC_GREEN_STALKER);
	//				g_char_slot_ingame.STATUS.push_back(SKILL_BLOOD_DRAIN);
	//g_char_slot_ingame.STATUS.push_back(6);
	//g_char_slot_ingame.STATUS.push_back(7);
	//g_char_slot_ingame.STATUS.push_back(8);
	//g_char_slot_ingame.STATUS.push_back(9);
	g_char_slot_ingame.bl_female = true;//false;
	g_char_slot_ingame.woman_info.helmet = W_OLD_HELMET;
	g_char_slot_ingame.woman_info.coat = W_OLD_COAT;
	g_char_slot_ingame.woman_info.trouser = W_OLD_TROUSER;
	g_char_slot_ingame.woman_info.hair = W_HAIR1;
	g_char_slot_ingame.woman_info.face = W_FACE1;
	g_char_slot_ingame.woman_info.left = W_SHIELD;//M_TR;//M_SWORD;
	g_char_slot_ingame.woman_info.right = W_CROSS;//W_SWORD;//W_DRAGON_SHIELD;
	g_char_slot_ingame.skin_color = 0;
	g_char_slot_ingame.hair_color = 0;
	g_char_slot_ingame.left_color = 405;
	g_char_slot_ingame.right_color = 405;
	g_char_slot_ingame.helmet_color = 0xFFFF;
	g_char_slot_ingame.trouser_color = 0XFFFF;
	g_char_slot_ingame.coat_color = 255;
	g_char_slot_ingame.bonus_point = 5;
	g_char_slot_ingame.skill_point = 50;
	

//	gC_vs_ui.StartTitle();
//	gC_vs_ui.StartCharacterManager();
	gC_vs_ui.StartGame();
//	gC_vs_ui.ChangeToOustersInterface();
	gC_vs_ui.ChangeToSlayerInterface();

	std::vector<C_VS_UI_NicknameInfo*>	TempNickNameList;
	C_VS_UI_NicknameInfo nik;
	nik.setNickname("바보0");
	nik.setNicknameID(0);
	nik.setNicknameIndex(0);
	nik.setNicknameType(0);
	TempNickNameList.push_back(&nik);
	//gC_vs_ui.AddNickNameList((void*)&nik);
	C_VS_UI_NicknameInfo nik1;
	nik1.setNickname("바보1");
	nik1.setNicknameID(1);
	nik1.setNicknameIndex(1);
	nik1.setNicknameType(1);
	//gC_vs_ui.AddNickNameList((void*)&nik);
	TempNickNameList.push_back(&nik1);
	C_VS_UI_NicknameInfo nik2;
	nik2.setNickname("바보2");
	nik2.setNicknameID(2);
	nik2.setNicknameIndex(2);
	nik2.setNicknameType(2);
	//gC_vs_ui.AddNickNameList((void*)&nik);
	TempNickNameList.push_back(&nik2);
	C_VS_UI_NicknameInfo nik3;
	nik3.setNickname("바보3");
	nik3.setNicknameID(3);
	nik3.setNicknameIndex(3);
	nik3.setNicknameType(3);
	//gC_vs_ui.AddNickNameList((void*)&nik);
	TempNickNameList.push_back(&nik3);
	C_VS_UI_NicknameInfo nik4;
	nik4.setNickname("바보4");
	nik4.setNicknameID(4);
	nik4.setNicknameIndex(4);
	nik4.setNicknameType(4);
	//gC_vs_ui.AddNickNameList((void*)&nik);
	TempNickNameList.push_back(&nik4);
	C_VS_UI_NicknameInfo nik5;
	nik5.setNickname("바보5");
	nik5.setNicknameID(5);
	nik5.setNicknameIndex(5);
	nik5.setNicknameType(5);
	TempNickNameList.push_back(&nik5);
	gC_vs_ui.SetNickNameList((void*)&TempNickNameList);

	g_char_slot_ingame.m_Powerjjang_Point = 0;
	//gC_vs_ui.AddNickNameList((void*)&nik);
	//	gC_vs_ui.RunQuickItemSlot();
	
	SIZE size = {256, 256};
	gC_vs_ui.SetSize(size);
	gC_vs_ui.SetZone(61);
	RECT rect = { 100, 100, 200, 200 };
	gC_vs_ui.SetSafetyZone(rect, 0);
	SetRect(&rect, 50, 50, 50, 50);
	gC_vs_ui.SetPortal(rect, 1001);
	SetRect(&rect, 50, 52, 60, 52);
	gC_vs_ui.SetPortal(rect, 2024);
	gC_vs_ui.SetNPC(50, 100, 21, "스토야노프");

	gC_vs_ui.SetNPC(100, 100, 670,"기잇발");
	
	
	
	

	//	gC_vs_ui.SetTime("23:00:05");
	
	//	gC_vs_ui.ServerDisconnectMessage();
	
	SetCursorPos(0, 0);
	
	//	gC_vs_ui.SetHP( 80, 100, TRUE );
	
	//	SIZE size = {256, 256};
	//	gC_vs_ui.SetZone(11);
	//	gC_vs_ui.SetSize(size);
	
	
	/*
	// color test
	int color = Convert24RGBto16(180, 240, 0);
	color = Convert24RGBto16(180, 240, 10);
	color = Convert24RGBto16(180, 240, 50);
	color = Convert24RGBto16(159, 151, 146);
	color = Convert24RGBto16(192, 192, 192);
	color = Convert24RGBto16(100, 100, 100);
	*/
	// Money Test add by Coffee 2007-8-2
// 	static C_VS_UI_MONEY_DIALOG * m_pC_dialog_withdraw_money;
// 				m_pC_dialog_withdraw_money = new C_VS_UI_MONEY_DIALOG(100, 
// 					100+gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY), 
// 					1, 0, NULL, DIALOG_OK|DIALOG_CANCEL, 10,
// 					C_VS_UI_MONEY_DIALOG::MONEY_WITHDRAW_FROM_EXCHANGE);
// 				m_pC_dialog_withdraw_money->Start();
	//
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) 
		{
			if (GetMessage(&msg, NULL, 0, 0)) 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				// for delete
				break;
				//return msg.wParam;
		}
		else 
      {
         if (gbl_active)
		   {
            ProgramLoop();
			g_CurrentFrame++;
		   }
   		else
	   		WaitMessage();
      }
	}

	delete g_pGameStringTable;
	ReleaseAllObject();
	gC_vs_ui.Release();

	if(NULL != g_pWebBrowser)
		g_pWebBrowser->Release();
	CoUninitialize();
//	DumpUnfreed();

	return 0;
}
