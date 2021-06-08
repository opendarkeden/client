// VS_UI_GameOusters.cpp

#include "client_PCH.h"
#include "VS_UI_GameOusters.h"
#include "VS_UI_filepath.h"
#include "VS_UI_Mouse_pointer.h"
#include "VS_UI.h"
#include "MGameStringTable.h"
#include "UserOption.h"
#include "ExperienceTable.h"
#include "UserInformation.h"
extern RECT g_GameRect;
int		C_VS_UI_OUSTERS_GEAR::m_slot_image[SLOT_SIZE] = {
	CIRCLET, COAT, CHAKRAM, WRISTLET, BOOTS, ARMSBAND, ARMSBAND, RING, RING, PENDENT, PENDENT, PENDENT, STONE_1, STONE_2, STONE_3, STONE_4,
	-1,
	-1,
	-1,
	-1,
	FASCIA,MITTEN,BLOODBIBLE,BLOODBIBLE,BLOODBIBLE,BLOODBIBLE,BLOODBIBLE,BLOODBIBLE
};

//----------------------------------------------------------------------------
// Operations
//----------------------------------------------------------------------------

MItem*	C_VS_UI_OUSTERS_QUICKITEM::GetItem(int slot)
{
	int pocketsize=0;

	if( g_pArmsBand1 != NULL)
		pocketsize += g_pArmsBand1->GetPocketNumber();
	if( g_pArmsBand2 != NULL)
		pocketsize += g_pArmsBand2->GetPocketNumber();
	
	if( slot < 0 || slot >= pocketsize )
		return NULL;
	
	if( g_pArmsBand1 != NULL && slot < g_pArmsBand1->GetPocketNumber() )
		return g_pArmsBand1->GetItem( slot );
	else
	{
		if( g_pArmsBand1 != NULL )
			return g_pArmsBand2->GetItem( slot - g_pArmsBand1->GetPocketNumber() );
		else
			return g_pArmsBand2->GetItem( slot );
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// C_VS_UI_OUSTERS
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_OUSTERS::C_VS_UI_OUSTERS():C_VS_UI_TRIBE()
{
	m_pC_armsband = NULL; 
	m_pC_main_spk = new C_SPRITE_PACK(SPK_OUSTERS_MAIN);

	Set(g_GameRect.right-m_pC_main_spk->GetWidth()+1, g_GameRect.bottom-m_pC_main_spk->GetHeight()+1, m_pC_main_spk->GetWidth()-1, m_pC_main_spk->GetHeight()-1);

	m_pC_sys_button_spk = new C_SPRITE_PACK(SPK_OUSTERS_SYS_BUTTON);

	// common buttons
	int tab_x = 35, tab_y = 65;
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_MENU), m_pC_sys_button_spk->GetHeight(TAB_MENU), TAB_MENU_ID, this, TAB_MENU) );
	tab_x += m_pC_sys_button_spk->GetWidth(TAB_MENU);
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_EXP), m_pC_sys_button_spk->GetHeight(TAB_EXP), TAB_EXP_ID, this, TAB_EXP) );
	tab_x += m_pC_sys_button_spk->GetWidth(TAB_EXP);
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_GUILD), m_pC_sys_button_spk->GetHeight(TAB_GUILD), TAB_GUILD_ID, this, TAB_GUILD) );
	tab_x += m_pC_sys_button_spk->GetWidth(TAB_GUILD);
	// 2004, 12, 2, sobeit add start - 기능
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_GUILD), m_pC_sys_button_spk->GetHeight(TAB_UTIL), TAB_UTIL_ID, this, TAB_UTIL) );
	tab_x += m_pC_sys_button_spk->GetWidth(TAB_UTIL);
	// 2004, 12, 2, sobeit add end
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_HELP), m_pC_sys_button_spk->GetHeight(TAB_HELP), TAB_HELP_ID, this, TAB_HELP) );

	int system_x = w-m_pC_main_spk->GetWidth(BUTTON_SYSTEM)-5, system_y = h-m_pC_main_spk->GetHeight(BUTTON_SYSTEM);
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(system_x, system_y, m_pC_main_spk->GetWidth(BUTTON_SYSTEM), m_pC_main_spk->GetHeight(BUTTON_SYSTEM), SYSTEM_ID, this, BUTTON_SYSTEM) );
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(24, system_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), PUSHPIN_ID, this,C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN ) );

	const int button_x = 43, button_y = 89, button_x_gap = 37, button_y_gap = 37;
	
	// menu buttons
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_INVENTORY), m_pC_sys_button_spk->GetHeight(BUTTON_INVENTORY), INVENTORY_ID, this, BUTTON_INVENTORY) );
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_GEAR), m_pC_sys_button_spk->GetHeight(BUTTON_GEAR), GEAR_ID, this, BUTTON_GEAR) );
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*2, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_INFO), m_pC_sys_button_spk->GetHeight(BUTTON_INFO), INFO_ID, this, BUTTON_INFO) );
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_PARTY), m_pC_sys_button_spk->GetHeight(BUTTON_PARTY), PARTY_ID, this, BUTTON_PARTY) );
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_QUEST), m_pC_sys_button_spk->GetHeight(BUTTON_QUEST), QUEST_ID, this, BUTTON_QUEST) );
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*2, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_MAIL), m_pC_sys_button_spk->GetHeight(BUTTON_MAIL), MAIL_ID, this, BUTTON_MAIL) );

	// sms 버튼 

//	if(false == g_pUserInformation->IsNetmarble)
	//add by zdj 2005.5.17
	// add by Coffee 2006.11.26
		//m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*3, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_SMS), m_pC_sys_button_spk->GetHeight(BUTTON_SMS), SMS_ID, this, BUTTON_SMS) );

	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*3, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_NAMING), m_pC_sys_button_spk->GetHeight(BUTTON_NAMING), NAMING_ID, this, BUTTON_NAMING) );

	// guild buttons
	m_pC_guild_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y+button_y_gap*0, m_pC_sys_button_spk->GetWidth(BUTTON_TEAM_INFO), m_pC_sys_button_spk->GetHeight(BUTTON_TEAM_INFO), TEAM_INFO_ID, this, BUTTON_TEAM_INFO) );
	m_pC_guild_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y+button_y_gap*0, m_pC_sys_button_spk->GetWidth(BUTTON_TEAM_MEMBER_LIST), m_pC_sys_button_spk->GetHeight(BUTTON_TEAM_MEMBER_LIST), TEAM_MEMBER_LIST_ID, this, BUTTON_TEAM_MEMBER_LIST) );
	// 2004, 10, 12, sobeit add start - 길드 명령 아이콘
	//if(false == g_pUserInformation->IsNetmarble) // 2005, 1, 12, sobeit modify - 넷마블 길드 연합 들어감
	{
		m_pC_guild_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y+button_y_gap*1, m_pC_sys_button_spk->GetWidth(BUTTON_GUILD_LIST), m_pC_sys_button_spk->GetHeight(BUTTON_GUILD_LIST), TEAM_LIST_ID, this, BUTTON_GUILD_LIST) );
		m_pC_guild_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y+button_y_gap*1, m_pC_sys_button_spk->GetWidth(BUTTON_WAIT_GUILD_LIST), m_pC_sys_button_spk->GetHeight(BUTTON_WAIT_GUILD_LIST), TEAM_WAIT_LIST_ID, this, BUTTON_WAIT_GUILD_LIST) );
		m_pC_guild_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*2, button_y+button_y_gap*1, m_pC_sys_button_spk->GetWidth(BUTTON_UNION), m_pC_sys_button_spk->GetHeight(BUTTON_UNION), TEAM_UNION_ID, this, BUTTON_UNION) );
	}
	// 2004, 10, 12, sobeit add end - 길드 명령 아이콘

	// 2004, 12, 2, sobeit add start - 기능
	m_pC_util_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_STORE), m_pC_sys_button_spk->GetHeight(BUTTON_STORE), UTIL_STORE_ID, this, BUTTON_STORE) );
	if(false == g_pUserInformation->IsNetmarble && false == g_pUserInformation->IsTestServer) // 본섭만
	{
//add by zdj 2005.5.16
		// add by Coffee 2006.11.26
		m_pC_util_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_POWER_JJANG), m_pC_sys_button_spk->GetHeight(BUTTON_POWER_JJANG), UTIL_POWER_JJANG_ID, this, BUTTON_POWER_JJANG) );
	}

	// 2004, 12, 2, sobeit add end

	// help buttons
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_HELP), m_pC_sys_button_spk->GetHeight(BUTTON_HELP), HELP_ID, this, BUTTON_HELP) );
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_CHAT_HELP), m_pC_sys_button_spk->GetHeight(BUTTON_CHAT_HELP), CHAT_HELP_ID, this, BUTTON_CHAT_HELP) );
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*2, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_BATTLE_HELP), m_pC_sys_button_spk->GetHeight(BUTTON_BATTLE_HELP), BATTLE_HELP_ID, this, BUTTON_BATTLE_HELP) );
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_SKILL_HELP), m_pC_sys_button_spk->GetHeight(BUTTON_SKILL_HELP), SKILL_HELP_ID, this, BUTTON_SKILL_HELP) );
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_TEAM_MEMBER_LIST), m_pC_sys_button_spk->GetHeight(BUTTON_TEAM_MEMBER_LIST), GUILD_HELP_ID, this, BUTTON_TEAM_MEMBER_LIST) );

	// instance objects 
	m_pC_gear = new C_VS_UI_OUSTERS_GEAR;
	m_pC_armsband = new C_VS_UI_OUSTERS_QUICKITEM;
	
	m_time = "";
	m_date = "";

#ifndef _LIB
	SetTime("12:34:05");
	SetDate("2001/06/30");
#endif
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_OUSTERS
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_OUSTERS::~C_VS_UI_OUSTERS()
{
	DeleteNew(m_pC_gear);
	DeleteNew(m_pC_armsband);
}

//-----------------------------------------------------------------------------
// C_VS_UI_OUSTERS::GetGearWindow
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_GEAR * C_VS_UI_OUSTERS::GetGearWindow() const
{
	return m_pC_gear;
}

//-----------------------------------------------------------------------------
// C_VS_UI_OUSTERS::CloseGear
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_OUSTERS::CloseGear()
{
	m_pC_gear->Finish();
}

//-----------------------------------------------------------------------------
// C_VS_UI_OUSTERS::OpenGear
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_OUSTERS::OpenGear(bool bl_set_load)
{
	m_pC_gear->Start(bl_set_load);
}

//-----------------------------------------------------------------------------
// DoCommonActionBeforeEventOccured
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OUSTERS::DoCommonActionBeforeEventOccured()
{
	C_VS_UI_TRIBE::DoCommonActionBeforeEventOccured();
}

//-----------------------------------------------------------------------------
// DoCommonActionAfterEventOccured
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OUSTERS::DoCommonActionAfterEventOccured()
{
	C_VS_UI_TRIBE::DoCommonActionAfterEventOccured();
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OUSTERS::Start()
{
	C_VS_UI_TRIBE::Start();

	m_pC_skill->Start();
	m_pC_hpbar->Start();
	m_pC_effect_status->Start();
	m_pC_minimap->Start();
	m_pC_armsband->Start();

	// add by Coffee 2007-3-6 警속各썹뒈暠
//	m_pC_worldmap->Start();
	// end 

	WindowEventReceiver(EVENT_WINDOW_MOVE);


}

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OUSTERS::Show()
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_main_spk->BltLocked(x, y, MAIN_WINDOW);
		
		m_pC_common_button_group->Show();
		
		int sec = 0, min = 0, hour = 0;
		char sz_temp[20];
		strcpy(sz_temp, m_time.c_str());
		sec = atoi(sz_temp+strlen(sz_temp)-2);
		sz_temp[strlen(sz_temp)-3] = '\0';
		min = atoi(sz_temp+strlen(sz_temp)-2);
		sz_temp[strlen(sz_temp)-3] = '\0';
		hour = atoi(sz_temp);
		
		const int icon_x = 106, icon_y = 45;
		if(hour >= 8 && hour < 16)	// 낮이다
			m_pC_main_spk->BltLocked(x+icon_x, y+icon_y, ICON_SUN);
		else if(hour >= 20 || hour < 4)	// 밤이다
			m_pC_main_spk->BltLocked(x+icon_x+3, y+icon_y, ICON_MOON);
		else	// 낮밤 교체의 시기-_- 시기-_-?;;
		{
			if(hour >=4 && hour < 8)	// 해뜨는 시간
			{
				int time = (hour-4)*60+min;
				// 해랑 달이랑 사이즈가 달라서 귀찮네-_-;
				Rect rect(0, 0, m_pC_main_spk->GetWidth(ICON_MOON), m_pC_main_spk->GetHeight(ICON_MOON)*(240-time)/240);
				rect.y = m_pC_main_spk->GetHeight(ICON_MOON)-rect.h;
				m_pC_main_spk->BltLockedClip(x+icon_x+3, y+icon_y-rect.y, rect, ICON_MOON);
				rect.w = m_pC_main_spk->GetWidth(ICON_SUN);
				rect.h = m_pC_main_spk->GetHeight(ICON_SUN)*(time)/240;
				rect.y = 0;
				m_pC_main_spk->BltLockedClip(x+icon_x, y+icon_y+m_pC_main_spk->GetHeight(ICON_SUN)-rect.h, rect, ICON_SUN);
			}
			else						// 아니면 해지는 시간이겠지-_-
			{
				int time = (hour-16)*60+min;
				// 해랑 달이랑 사이즈가 달라서 귀찮네-_-;
				Rect rect(0, 0, m_pC_main_spk->GetWidth(ICON_SUN), m_pC_main_spk->GetHeight(ICON_SUN)*(240-time)/240);
				rect.y = m_pC_main_spk->GetHeight(ICON_SUN)-rect.h;
				m_pC_main_spk->BltLockedClip(x+icon_x, y+icon_y-rect.y, rect, ICON_SUN);
				rect.w = m_pC_main_spk->GetWidth(ICON_MOON);
				rect.h = m_pC_main_spk->GetHeight(ICON_MOON)*(time)/240;
				rect.y = 0;
				m_pC_main_spk->BltLockedClip(x+icon_x+3, y+icon_y+m_pC_main_spk->GetHeight(ICON_MOON)-rect.h, rect, ICON_MOON);
			}
		}
		
		switch(m_selected_tab)
		{
		case TAB_MENU_ID:
			m_pC_menu_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			m_pC_menu_button_group->ShowDescription();
			break;
			
		case TAB_GUILD_ID:
			m_pC_guild_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			m_pC_guild_button_group->ShowDescription();
			break;
			
		case TAB_MSG_ID:
			m_pC_msg_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			m_pC_msg_button_group->ShowDescription();
			break;
		case TAB_UTIL_ID:
			m_pC_util_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			m_pC_util_button_group->ShowDescription();
			break;

		case TAB_HELP_ID:
			m_pC_help_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			m_pC_help_button_group->ShowDescription();
			break;

		case TAB_EXP_ID:
		// EXP 는 버튼이 없다
		{
			const int bar_x = 110, bar_y = 98, str_x = 27, num_x = 46, bar_gap = 14;
			char sz_temp[100];
			Rect rect;


			int exp_remain = g_char_slot_ingame.EXP_REMAIN;
			__int64 goal_exp = g_pExperienceTable->GetOustersInfo( g_char_slot_ingame.level ).GoalExp;
			int exp_width = m_pC_main_spk->GetWidth(OUSTERS_EXP_BAR);
			int exp_height = m_pC_main_spk->GetHeight(OUSTERS_EXP_BAR);
			int exp_bar = /*int((float)exp_width * ((float)exp_remain / (float)goal_exp));*/
				exp_width * (goal_exp - exp_remain) / goal_exp;
			
			//exp bar
			rect.Set(0, 0, exp_bar, exp_height);
			
			//exp bar
			m_pC_main_spk->BltLocked(x+bar_x-3, y+bar_y-3, OUSTERS_EXP_BACK);
			m_pC_main_spk->BltLockedClip(x+bar_x, y+bar_y, rect, OUSTERS_EXP_BAR);

			const int elemental_max = 25;
			
			exp_bar = int((float)exp_width * ((float)g_char_slot_ingame.ElementalFire / (float)elemental_max));
			rect.Set(0, 0, exp_bar, exp_height);
			m_pC_main_spk->BltLocked(x+bar_x-3, y+bar_y-3+bar_gap*1+8, OUSTERS_EXP_BACK);
			m_pC_main_spk->BltLockedClip(x+bar_x, y+bar_y+bar_gap*1+8, rect, OUSTERS_EXP_BAR);

			exp_bar = int((float)exp_width * ((float)g_char_slot_ingame.ElementalWater / (float)elemental_max));
			rect.Set(0, 0, exp_bar, exp_height);
			m_pC_main_spk->BltLocked(x+bar_x-3, y+bar_y-3+bar_gap*2+8, OUSTERS_EXP_BACK);
			m_pC_main_spk->BltLockedClip(x+bar_x, y+bar_y+bar_gap*2+8, rect, OUSTERS_EXP_BAR);
			
			exp_bar = int((float)exp_width * ((float)g_char_slot_ingame.ElementalEarth / (float)elemental_max));
			rect.Set(0, 0, exp_bar, exp_height);
			m_pC_main_spk->BltLocked(x+bar_x-3, y+bar_y-3+bar_gap*3+8, OUSTERS_EXP_BACK);
			m_pC_main_spk->BltLockedClip(x+bar_x, y+bar_y+bar_gap*3+8, rect, OUSTERS_EXP_BAR);
			
			gpC_base->m_p_DDSurface_back->Unlock();

			g_FL2_GetDC();
			sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_LEVEL_DESCRIPTION].GetString(), g_char_slot_ingame.level);
			g_PrintColorStrShadow(x+num_x, y+bar_y-3, sz_temp, gpC_base->m_chatting_pi, RGB_GRAY, RGB_BLACK);
			
			sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_ELEMENTAL_FIRE_DESCRIPTION].GetString(), g_char_slot_ingame.ElementalFire);
			g_PrintColorStrShadow(x+num_x, y+bar_y-3+bar_gap*1+8, sz_temp, gpC_base->m_chatting_pi, g_ELEMENTAL_COLOR[ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE], RGB_BLACK);
			
			sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_ELEMENTAL_WATER_DESCRIPTION].GetString(), g_char_slot_ingame.ElementalWater);
			g_PrintColorStrShadow(x+num_x, y+bar_y-3+bar_gap*2+8, sz_temp, gpC_base->m_chatting_pi, g_ELEMENTAL_COLOR[ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER], RGB_BLACK);
			
			sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_ELEMENTAL_EARTH_DESCRIPTION].GetString(), g_char_slot_ingame.ElementalEarth);
			g_PrintColorStrShadow(x+num_x, y+bar_y-3+bar_gap*3+9, sz_temp, gpC_base->m_chatting_pi, g_ELEMENTAL_COLOR[ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH], RGB_BLACK);
			
			g_FL2_ReleaseDC();
		}
		break;
		

		default:
			gpC_base->m_p_DDSurface_back->Unlock();
			break;
		}

	}	

	g_FL2_GetDC();				
	g_PrintColorStr(x + 146 - g_GetStringWidth(m_date.c_str(), gpC_base->m_chatting_pi.hfont)/2, y+16+10, m_date.c_str() ,gpC_base->m_chatting_pi, RGB_WHITE);	
	g_PrintColorStr(x + 6+128+18 - g_GetStringWidth(m_time.c_str(), gpC_base->m_chatting_pi.hfont)/2, y+37+10, m_time.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);	
	m_pC_common_button_group->ShowDescription();
	g_FL2_ReleaseDC();

	m_pC_skill->Show2();

	SHOW_WINDOW_ATTR;

}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OUSTERS::Process()
{
	C_VS_UI_TRIBE::Process();
}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OUSTERS::KeyboardControl(UINT message, UINT key, long extra)
{
	m_pC_chatting->KeyboardControl(message, key, extra);
}

void C_VS_UI_OUSTERS::HotKey_F1()
{
	assert(m_pC_armsband != NULL);

	m_pC_armsband->Use(C_VS_UI_OUSTERS_QUICKITEM::SLOT_1);
}

void C_VS_UI_OUSTERS::HotKey_F2()
{
	assert(m_pC_armsband != NULL);

	m_pC_armsband->Use(C_VS_UI_OUSTERS_QUICKITEM::SLOT_2);
}

void C_VS_UI_OUSTERS::HotKey_F3()
{
	assert(m_pC_armsband != NULL);

	m_pC_armsband->Use(C_VS_UI_OUSTERS_QUICKITEM::SLOT_3);
}

void C_VS_UI_OUSTERS::HotKey_F4()
{
	assert(m_pC_armsband != NULL);

	m_pC_armsband->Use(C_VS_UI_OUSTERS_QUICKITEM::SLOT_4);
}

void C_VS_UI_OUSTERS::HotKey_F5()
{
	assert(m_pC_armsband != NULL);

	m_pC_armsband->Use(C_VS_UI_OUSTERS_QUICKITEM::SLOT_5);
}

void C_VS_UI_OUSTERS::HotKey_F6()
{
	assert(m_pC_armsband != NULL);

	m_pC_armsband->Use(C_VS_UI_OUSTERS_QUICKITEM::SLOT_6);
}

void C_VS_UI_OUSTERS::HotKey_F7()
{
	C_VS_UI_SKILL * p_skill = GetSkillWindow();

	int slot = p_skill->GetFocusedSlot();
	int id = p_skill->GetSkillID(slot);
	if (id != NOT_SELECTED)
	{
		p_skill->SetHotkey(C_VS_UI_SKILL::HOTKEY7, (ACTIONINFO)id);
	}
	else
	{
		// use
		p_skill->ChangeSkillByHotkey(C_VS_UI_SKILL::HOTKEY7);
	}
}

void C_VS_UI_OUSTERS::HotKey_F8()
{
	C_VS_UI_SKILL * p_skill = GetSkillWindow();

	int slot = p_skill->GetFocusedSlot();
	int id = p_skill->GetSkillID(slot);
	if (id != NOT_SELECTED)
	{
		p_skill->SetHotkey(C_VS_UI_SKILL::HOTKEY8, (ACTIONINFO)id);
	}
	else
	{
		// use
		p_skill->ChangeSkillByHotkey(C_VS_UI_SKILL::HOTKEY8);
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_OUSTERS_GEAR::C_VS_UI_OUSTERS_GEAR
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_OUSTERS_GEAR::C_VS_UI_OUSTERS_GEAR()
{
	assert(MOustersGear::MAX_GEAR_OUSTERS == SLOT_SIZE);
  
//	if(g_char_slot_ingame.m_AdvancementLevel > 0)// 2차전직 bycsm 2004.12.31 
//	{
	m_slot_rect[SN_CIRCLET].Set(114, 60, 60, 60);			// 서클릿
	m_slot_rect[SN_COAT].Set(116, 165, 60, 90);				// 옷
	m_slot_rect[SN_LEFTHAND].Set(201, 165, 60, 90);			// 왼손
	m_slot_rect[SN_RIGHTHAND].Set(27, 165, 60, 90);			// 오른손
	m_slot_rect[SN_BOOTS].Set(114, 261, 60, 90);				// 신발
	m_slot_rect[SN_ARMSBAND1].Set(25, 100, 60, 60);			// 암스밴드1
	m_slot_rect[SN_ARMSBAND2].Set(201, 100, 60, 60);			// 암스밴드2
	m_slot_rect[SN_RING1].Set(29, 269, 30, 30);				// 링1
	m_slot_rect[SN_RING2].Set(230, 269, 30, 30);				// 링2
	m_slot_rect[SN_PENDENT1].Set(94, 129, 30, 30);			// 목걸이1
	m_slot_rect[SN_PENDENT2].Set(130, 129, 30, 30);			// 목걸이2
	m_slot_rect[SN_PENDENT3].Set(166, 129, 30, 30);			// 목걸이3
	m_slot_rect[SN_STONE1].Set(65, 268, 30, 30);			// 정령석1
	m_slot_rect[SN_STONE2].Set(195, 268, 30, 30);			// 정령석2
	m_slot_rect[SN_STONE3].Set(65, 304, 30, 30);			// 정령석3
	m_slot_rect[SN_STONE4].Set(195, 304, 30, 30);			// 정령석4

	m_slot_rect[SN_COREZAP1].Set(62, 270, 30, 30);			// 코어잽1
	m_slot_rect[SN_COREZAP2].Set(194, 270, 30, 30);			// 코어잽2
	m_slot_rect[SN_COREZAP3].Set(62, 306, 30, 30);			// 코어잽3
	m_slot_rect[SN_COREZAP4].Set(194, 306, 30, 30);			// 코어잽4

	m_slot_rect[SN_FASCIA].Set(26, 37, 60, 60);	
	m_slot_rect[SN_MITTEN].Set(203, 37, 60, 60);

	m_slot_rect[SN_BLOODBIBLE1].Set(41, 351, 30, 30);
	m_slot_rect[SN_BLOODBIBLE2].Set(75, 351, 30, 30);
	m_slot_rect[SN_BLOODBIBLE3].Set(111, 351, 30, 30);
	m_slot_rect[SN_BLOODBIBLE4].Set(146, 351, 30, 30);
	m_slot_rect[SN_BLOODBIBLE5].Set(180, 351, 30, 30);
	m_slot_rect[SN_BLOODBIBLE6].Set(215, 351, 30, 30);
	
	m_slot_size = SLOT_SIZE;

//	}
//	else
//	{
//
//		m_slot_rect[SN_CIRCLET].Set(114, 25, 60, 60);			// 서클릿
//		m_slot_rect[SN_COAT].Set(114, 127, 60, 90);				// 옷
//		m_slot_rect[SN_LEFTHAND].Set(201, 127, 60, 90);			// 왼손
//		m_slot_rect[SN_RIGHTHAND].Set(27, 127, 60, 90);			// 오른손
//		m_slot_rect[SN_BOOTS].Set(114, 223, 60, 90);				// 신발
//		m_slot_rect[SN_ARMSBAND1].Set(27, 61, 60, 60);			// 암스밴드1
//		m_slot_rect[SN_ARMSBAND2].Set(201, 61, 60, 60);			// 암스밴드2
//		m_slot_rect[SN_RING1].Set(42, 247, 30, 30);				// 링1
//		m_slot_rect[SN_RING2].Set(218, 247, 30, 30);				// 링2
//		m_slot_rect[SN_PENDENT1].Set(93, 91, 30, 30);			// 목걸이1
//		m_slot_rect[SN_PENDENT2].Set(129, 91, 30, 30);			// 목걸이2
//		m_slot_rect[SN_PENDENT3].Set(165, 91, 30, 30);			// 목걸이3
//		m_slot_rect[SN_STONE1].Set(77, 246, 30, 30);			// 정령석1
//		m_slot_rect[SN_STONE2].Set(181, 246, 30, 30);			// 정령석2
//		m_slot_rect[SN_STONE3].Set(77, 282, 30, 30);			// 정령석3
//		m_slot_rect[SN_STONE4].Set(181, 282, 30, 30);			// 정령석4
//		
//		m_slot_rect[SN_COREZAP1].Set(77, 246, 30, 30);			// 코어잽1
//		m_slot_rect[SN_COREZAP2].Set(181, 246, 30, 30);			// 코어잽2
//		m_slot_rect[SN_COREZAP3].Set(77, 282, 30, 30);			// 코어잽3
//		m_slot_rect[SN_COREZAP4].Set(181, 282, 30, 30);			// 코어잽4
//		
//		m_slot_rect[SN_FASCIA].Set(26, 37, 60, 60);	
//		m_slot_rect[SN_MITTEN].Set(203, 37, 60, 60);
//
//
//		m_slot_rect[SN_BLOODBIBLE1].Set(37, 317, 30, 30);
//		m_slot_rect[SN_BLOODBIBLE2].Set(73, 317, 30, 30);
//		m_slot_rect[SN_BLOODBIBLE3].Set(109, 317, 30, 30);
//		m_slot_rect[SN_BLOODBIBLE4].Set(145, 317, 30, 30);
//		m_slot_rect[SN_BLOODBIBLE5].Set(181, 317, 30, 30);
//		m_slot_rect[SN_BLOODBIBLE6].Set(217, 317, 30, 30);
//		m_slot_size = SLOT_SIZE;
//				
//	}
	
	
	m_p_slot_rect = m_slot_rect;
	m_p_slot_image = m_slot_image;
}
//-----------------------------------------------------------------------------
// C_VS_UI_OUSTERS_GEAR::~C_VS_UI_OUSTERS_GEAR
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_OUSTERS_GEAR::~C_VS_UI_OUSTERS_GEAR()
{
//	DeleteNew(m_pC_gear);
}


/*-----------------------------------------------------------------------------
- C_VS_UI_OUSTERS_QUICKITEM
-
-----------------------------------------------------------------------------*/
C_VS_UI_OUSTERS_QUICKITEM::C_VS_UI_OUSTERS_QUICKITEM()
{
	g_RegisterWindow(this);

	AttrPin(true);

	m_image_spk.Open(SPK_OUSTERS_QUICKITEM);

	// 넓이는 처음 no belt일 경우이다. belt를 착용하면 바뀐다.
	// 높이는 일정하다.
	Set(0, 434, m_image_spk.GetWidth(END_EDGE), m_image_spk.GetHeight(END_EDGE));

	m_pC_button_group = new ButtonGroup(this);

	int alpha_button_offset_x = 14, alpha_button_offset_y = 7;
	int pushpin_button_offset_x = 14, pushpin_button_offset_y = 23;
	int rotate_button_offset_x = 5, rotate_button_offset_y = 8;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_button_offset_x, alpha_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pushpin_button_offset_x, pushpin_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), PUSHPIN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(rotate_button_offset_x, rotate_button_offset_y, m_image_spk.GetWidth(BUTTON_ROTATE_HILIGHTED), m_image_spk.GetHeight(BUTTON_ROTATE_HILIGHTED), ROTATE_ID, this, BUTTON_ROTATE_HILIGHTED));

	m_bl_width = true;

}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_OUSTERS_QUICKITEM
-
-----------------------------------------------------------------------------*/
C_VS_UI_OUSTERS_QUICKITEM::~C_VS_UI_OUSTERS_QUICKITEM()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::QUICKITEMSLOT, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::QUICKITEMSLOT, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::QUICKITEMSLOT, Rect(x, y, w, h));
	gpC_vs_ui_window_manager->SetQuickItemSlotHeight(!m_bl_width);

	g_UnregisterWindow(this);
	DeleteNew(m_pC_button_group);

}

//-----------------------------------------------------------------------------
// C_VS_UI_OUSTERS_QUICKITEM::ToggleBelt
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_OUSTERS_QUICKITEM::ToggleBelt()
{
//	m_bl_opened = !m_bl_opened;

	// reset Window width by open/close
	Run(PUSHPIN_ID);
	ResetSize();

	PlaySound(SOUND_OUSTERS_ARMSBAND);
}

void	C_VS_UI_OUSTERS_QUICKITEM::Open()
{
//	m_bl_opened = true;

	AttrAutoHide(Window::ATTRIBUTES_HIDE_NOT);
	// reset Window width by open/close
	ResetSize();

	PlaySound(SOUND_OUSTERS_ARMSBAND);
}

void	C_VS_UI_OUSTERS_QUICKITEM::Close()
{
//	m_bl_opened = false;
	AttrAutoHide(Window::ATTRIBUTES_HIDE_NOT);
	
	Run(PUSHPIN_ID);

	// reset Window width by open/close
	ResetSize();

	PlaySound(SOUND_OUSTERS_ARMSBAND);
}

//-----------------------------------------------------------------------------
// ResetSize
//
// belt를 교체하고나서 size를 재설정해야 한다.
//-----------------------------------------------------------------------------
void C_VS_UI_OUSTERS_QUICKITEM::ResetSize()
{
	// 으아아아 하드코딩!!
	int slot_size = m_image_spk.GetWidth(SLOT);
	if(m_bl_width)
	{
		h = m_image_spk.GetHeight(START_EDGE);
		w = m_image_spk.GetWidth(START_EDGE_EXTEND);
		w += m_image_spk.GetWidth(END_EDGE);
		w += m_image_spk.GetWidth(SLOT)*GetPocketCount();
		if(x <= 0)
		{
			w -= m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE);

			m_p_slot_x[SLOT_1] = m_image_spk.GetWidth(STATCH_EDGE)+3;
		}
		else if(x+w >= g_GameRect.right)
		{
			m_bl_resize = true;
			m_p_slot_x[SLOT_1] = m_image_spk.GetWidth(START_EDGE_EXTEND)+3;
		}
		else
		{
			m_p_slot_x[SLOT_1] = m_image_spk.GetWidth(START_EDGE)+3;
		}
	}
	else
	{
		w = m_image_spk.GetWidth(START_EDGE+ROTATED_OFFSET);
		h = m_image_spk.GetHeight(START_EDGE_EXTEND+ROTATED_OFFSET);
		h += m_image_spk.GetHeight(END_EDGE+ROTATED_OFFSET);
		h += m_image_spk.GetHeight(SLOT+ROTATED_OFFSET)*GetPocketCount();
		if(y <= 0)
		{
			h -= m_image_spk.GetHeight(START_EDGE+ROTATED_OFFSET) - m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET);
			m_p_slot_x[SLOT_1] = m_image_spk.GetWidth(STATCH_EDGE)+3;
		}
		else if(y+h >= g_GameRect.bottom)
		{
			m_bl_resize = true;
			m_p_slot_x[SLOT_1] = m_image_spk.GetWidth(START_EDGE_EXTEND)+3;
		}
		else
			m_p_slot_x[SLOT_1] = m_image_spk.GetWidth(START_EDGE_EXTEND)+3;

	}
	m_p_slot_x[SLOT_2] = m_p_slot_x[SLOT_1]+slot_size;
	m_p_slot_x[SLOT_3] = m_p_slot_x[SLOT_2]+slot_size;
	m_p_slot_x[SLOT_4] = m_p_slot_x[SLOT_3]+slot_size;
	m_p_slot_x[SLOT_5] = m_p_slot_x[SLOT_4]+slot_size;
	m_p_slot_x[SLOT_6] = m_p_slot_x[SLOT_5]+slot_size;
}

//-----------------------------------------------------------------------------
// GetPocketCount
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_OUSTERS_QUICKITEM::GetPocketCount() const
{
	int PocketSize = 0;
	
	if(g_pArmsBand1 != NULL)
		PocketSize += g_pArmsBand1->GetPocketNumber();

	if(g_pArmsBand2 != NULL)
		PocketSize += g_pArmsBand2->GetPocketNumber();

	return PocketSize;
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OUSTERS_QUICKITEM::WindowEventReceiver(id_t event)
{
	switch(event)
	{
	case EVENT_WINDOW_MOVE: 
		{
			ResetSize();
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_OUSTERS_QUICKITEM::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OUSTERS_QUICKITEM::Start()
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::QUICKITEMSLOT));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::QUICKITEMSLOT));
	if(m_bl_width == gpC_vs_ui_window_manager->IsQuickItemSlotHeight())
		Run(ROTATE_ID);
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::QUICKITEMSLOT);
	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}

	PI_Processor::Start();

	gpC_window_manager->AppearWindow(this);

	m_focus_slot = NOT_SELECTED;
	m_bl_resize = false;
	ResetSize();
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OUSTERS_QUICKITEM::Process()
{
	m_pC_button_group->Process();
	ProcessHide();
}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OUSTERS_QUICKITEM::KeyboardControl(UINT message, UINT key, long extra)
{

}

//-----------------------------------------------------------------------------
// UnacquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OUSTERS_QUICKITEM::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
	m_focus_slot = NOT_SELECTED;
}

/*-----------------------------------------------------------------------------
- MouseControl
-
-----------------------------------------------------------------------------*/
bool C_VS_UI_OUSTERS_QUICKITEM::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;

	// test open/close button
	bool ret = false;
	if(m_bl_width)
	{
		if(x+w >= g_GameRect.right)
			ret = !m_pC_button_group->MouseControl(message, _x, _y);
		else
			ret = !m_pC_button_group->MouseControl(message, w-_x, _y);
	}
	else
	{
		if(y <= 0)
			ret = !m_pC_button_group->MouseControl(message, h-_y, _x);
		else
			ret = !m_pC_button_group->MouseControl(message, _y, _x);
	}

	if (g_pArmsBand1 != NULL || g_pArmsBand2 != NULL)
	{
		int i;

		const MItem * p_selected_item;

		switch (message)
		{
		case M_MOVING:
			for (i=0; i < GetPocketCount(); i++)
			{
				int m;
				int loop;
				int px, py;
				
				if (gpC_mouse_pointer->GetPickUpItem())
					loop = ITEM_REF_POINT_COUNT;
				else
					loop = 1;
				
				for (m=0; m < loop; m++)
				{
					if(m_bl_width)
					{
						if (loop == ITEM_REF_POINT_COUNT)
						{
							px = g_item_ref_point[m].x+gpC_mouse_pointer->GetPointerX();
							py = g_item_ref_point[m].y+gpC_mouse_pointer->GetPointerY()-8;
						}
						else
						{
							px = _x+x;
							py = _y+y-8;
						}
					}
					else
					{
						if (loop == ITEM_REF_POINT_COUNT)
						{
							px = g_item_ref_point[m].y+gpC_mouse_pointer->GetPointerX()-8;
							py = g_item_ref_point[m].x+gpC_mouse_pointer->GetPointerY();
						}
						else
						{
							px = _x+x-8;
							py = _y+y;
						}
					}
					
					if (m_bl_width &&
						px >= x+m_p_slot_x[i] && px < x+m_p_slot_x[i]+QUICKSLOT_W &&
						py >= y && py < y+QUICKSLOT_H ||
						!m_bl_width &&
						px >= x && px < x+QUICKSLOT_W &&
						py >= y+m_p_slot_x[i] && py < y+m_p_slot_x[i]+QUICKSLOT_H
						)
					{
						if(m_focus_slot != i)
						{
							gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
							m_focus_slot = i;
						}

						if( g_pArmsBand1 != NULL && m_focus_slot < g_pArmsBand1->GetPocketNumber() )			// armsband 1부터 체크한다.	
							p_selected_item = g_pArmsBand1->GetItem(m_focus_slot);
						else
						{
							if( g_pArmsBand1 != NULL)
								p_selected_item = g_pArmsBand2->GetItem(m_focus_slot- g_pArmsBand1->GetPocketNumber() );
							else
								p_selected_item = g_pArmsBand2->GetItem(m_focus_slot);								
						}

						if (p_selected_item != NULL)
						{
							if(m_bl_width)
								g_descriptor_manager.Set(DID_ITEM, x+m_p_slot_x[m_focus_slot], y, (void *)p_selected_item);
							else
								g_descriptor_manager.Set(DID_ITEM, x, y+m_p_slot_x[m_focus_slot], (void *)p_selected_item);
						}
						
						return true;
					}
				}
			}
			m_focus_slot = NOT_SELECTED;
			break;

			case M_LEFTBUTTON_DOWN:
			case M_LB_DOUBLECLICK:
				//
				// Item을 집던가 놓는다.
				//
				{
					ret |= Click();

					if (ret)
						EMPTY_MOVE;
				}
				break;

			case M_RIGHTBUTTON_DOWN:
				//
				// Item을 사용한다.
				//
				Use(m_focus_slot);
				break;

			case M_RIGHTBUTTON_UP:
				break;
		}
	}
	switch (message)
	{
		case M_MOVING:
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			if (gpC_mouse_pointer->GetPickUpItem() == NULL && ret == false)
			{
				MoveReady();
				SetOrigin(_x, _y);
				break;
			}
			break;

		case M_LEFTBUTTON_UP:
			if(m_bl_resize)
			{
				m_bl_resize = false;
				if(m_bl_width)
				{
					if(x+w >= g_GameRect.right)
					{
						if(x+w == g_GameRect.right)
							x += (m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE));
						w -= (m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE));
					}
				}
				else
				{
					if(y+h >= g_GameRect.bottom)
					{
						if(y+h == g_GameRect.bottom)
							y += (m_image_spk.GetHeight(START_EDGE+ROTATED_OFFSET) - m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET));
						h -= (m_image_spk.GetHeight(START_EDGE+ROTATED_OFFSET) - m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET));
					}
				}
			}
			break;
	}

	return true;
}

void C_VS_UI_OUSTERS_QUICKITEM::CancelPushState()
{
	m_pC_button_group->CancelPushState();
}

void C_VS_UI_OUSTERS_QUICKITEM::Run(id_t id)
{
	switch (id)
	{
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;

	case PUSHPIN_ID:
		if(GetAttributes()->autohide)
			AttrAutoHide(ATTRIBUTES_HIDE_NOT);
		else
		{
			if(m_bl_width)
				AttrAutoHide(ATTRIBUTES_HIDE_HEIGHT);
			else
				AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
		}
		EMPTY_MOVE;
		break;

	case ROTATE_ID:
		if(m_bl_width)
		{
			if(GetAttributes()->autohide != ATTRIBUTES_HIDE_NOT)
				AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
			x += w-h;
		}
		else
		{
			if(GetAttributes()->autohide != ATTRIBUTES_HIDE_NOT)
				AttrAutoHide(ATTRIBUTES_HIDE_HEIGHT);
			x -= h-w;
		}
		m_bl_width = !m_bl_width;
		ResetSize();
		EMPTY_MOVE;
		break;
	}
}

void	C_VS_UI_OUSTERS_QUICKITEM::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char* m_quickitem_button_string[6] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_WIDTH].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HEIGHT].GetString(),
	};
	
	if(GetAttributes()->alpha && p_button->GetID() == ALPHA_ID)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_quickitem_button_string[3],0,0);
	if(GetAttributes()->autohide && p_button->GetID() == PUSHPIN_ID)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_quickitem_button_string[4],0,0);
	if(m_bl_width && p_button->GetID() == ROTATE_ID)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_quickitem_button_string[5],0,0);
	else
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_quickitem_button_string[p_button->GetID()],0,0);
}

void	C_VS_UI_OUSTERS_QUICKITEM::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{	
	// 가로용
	if(m_bl_width)
	{
		if(x+w >= g_GameRect.right)
		{
			if(p_button->GetID() == ALPHA_ID)
			{
				if(GetAttributes()->alpha)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
			}
			else if(p_button->GetID() == PUSHPIN_ID)
			{
				if(GetAttributes()->autohide)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
			}
			else
			{
				if(p_button->GetFocusState())
				{
					if(p_button->GetPressState())
						m_image_spk.BltLocked(p_button->x+x-1, p_button->y+y, BUTTON_ROTATE_HILIGHTED_PUSHED);
					else
						m_image_spk.BltLocked(p_button->x+x-1, p_button->y+y, BUTTON_ROTATE_HILIGHTED);
				}
			}
		}
		else
		{
			if(p_button->GetID() == ALPHA_ID)
			{
				if(GetAttributes()->alpha)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
			}
			else if(p_button->GetID() == PUSHPIN_ID)
			{
				if(GetAttributes()->autohide)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
			}
			else
			{
				if(p_button->GetFocusState())
				{
					if(p_button->GetPressState())
						m_image_spk.BltLocked(x+w-p_button->x-m_image_spk.GetWidth(BUTTON_ROTATE_HILIGHTED), p_button->y+y, BUTTON_ROTATE_HILIGHTED_PUSHED);
					else
						m_image_spk.BltLocked(x+w-p_button->x-m_image_spk.GetWidth(BUTTON_ROTATE_HILIGHTED), p_button->y+y, BUTTON_ROTATE_HILIGHTED);
				}
			}
		}
	}
	// 세로용
	else
	{
		if(y > 0)
		{
			if(p_button->GetID() == ALPHA_ID)
			{
				if(GetAttributes()->alpha)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->y+x, p_button->x+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->y+x, p_button->x+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
			}
			else if(p_button->GetID() == PUSHPIN_ID)
			{
				if(GetAttributes()->autohide)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->y+x, p_button->x+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->y+x, p_button->x+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
			}
			else
			{
				if(p_button->GetFocusState())
				{
					if(p_button->GetPressState())
						m_image_spk.BltLocked(p_button->y+x, p_button->x+y-1, BUTTON_ROTATE_HILIGHTED_PUSHED+ROTATED_OFFSET);
					else
						m_image_spk.BltLocked(p_button->y+x, p_button->x+y-1, BUTTON_ROTATE_HILIGHTED+ROTATED_OFFSET);
				}
			}
		}
		else
		{
			if(p_button->GetID() == ALPHA_ID)
			{
				if(GetAttributes()->alpha)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->y, y+h-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->y, y+h-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
			}
			else if(p_button->GetID() == PUSHPIN_ID)
			{
				if(GetAttributes()->autohide)
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->y, y+h-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->y, y+h-p_button->x-gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
			}
			else
			{
				if(p_button->GetFocusState())
				{
					if(p_button->GetPressState())
						m_image_spk.BltLocked(x+p_button->y, y+h-p_button->x-m_image_spk.GetHeight(BUTTON_ROTATE_HILIGHTED+ROTATED_OFFSET), BUTTON_ROTATE_HILIGHTED_PUSHED+ROTATED_OFFSET);
					else
						m_image_spk.BltLocked(x+p_button->y, y+h-p_button->x-m_image_spk.GetHeight(BUTTON_ROTATE_HILIGHTED+ROTATED_OFFSET), BUTTON_ROTATE_HILIGHTED+ROTATED_OFFSET);
				}
			}
		}
	}
	
}

/*-----------------------------------------------------------------------------
- Show
-
-----------------------------------------------------------------------------*/
void C_VS_UI_OUSTERS_QUICKITEM::Show()
{
	static int old_size = GetPocketCount();
	if(old_size != GetPocketCount())
	{
		ResetSize();
		old_size = GetPocketCount();
	}

	int i;
	int temp_x = x, temp_y = y;

	int len = 0;
	RECT rect[8];
	int num[8];

	// 아씽 졸라 하드코딩-_- 졸라 허덥해-_-;;
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		bool bl_statch = false;

		if(m_bl_width)
		{
			if(x <= 0)
			{
				m_image_spk.BltLocked(temp_x, temp_y+1, STATCH_EDGE);
				temp_x += m_image_spk.GetWidth(STATCH_EDGE);
			}
			else
			{
				if(x+w >= g_GameRect.right)
				{
					bl_statch = true;
					if(Moving() && m_bl_resize)	// 먼가 좋은 방법이 있을터인데-_-;; 이런 하드 코딩은 하면 안되는데-_-;;
						temp_x += (m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE));
					m_image_spk.BltLocked(temp_x, temp_y, START_EDGE_EXTEND);
					temp_x += m_image_spk.GetWidth(START_EDGE_EXTEND);
					if(Moving() && m_bl_resize)	// 먼가 좋은 방법이 있을터인데-_-;; 이런 하드 코딩은 하면 안되는데-_-;;
						x += (m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE));
					if(Moving() && m_bl_resize)	// 먼가 좋은 방법이 있을터인데-_-;; 이런 하드 코딩은 하면 안되는데-_-;;
						x -= (m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE));
				}
				else
				{
					
					m_image_spk.BltLocked(temp_x, temp_y, START_EDGE);
					temp_x += m_image_spk.GetWidth(START_EDGE);
				}
			}
		}
		else
		{
			if(y <= 0)
			{
				bl_statch = true;
				m_image_spk.BltLocked(temp_x+1, temp_y, STATCH_EDGE+ROTATED_OFFSET);
				temp_y += m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET);
			}
			else
			{
				if(Moving() && m_bl_resize)	// 먼가 좋은 방법이 있을터인데-_-;; 이런 하드 코딩은 하면 안되는데-_-;;
				{
					temp_y += (m_image_spk.GetHeight(START_EDGE+ROTATED_OFFSET) - m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET));
					m_image_spk.BltLocked(temp_x, temp_y, START_EDGE_EXTEND+ROTATED_OFFSET);
					temp_y += m_image_spk.GetHeight(START_EDGE_EXTEND+ROTATED_OFFSET);
					y += (m_image_spk.GetHeight(START_EDGE+ROTATED_OFFSET) - m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET));
					y -= (m_image_spk.GetHeight(START_EDGE+ROTATED_OFFSET) - m_image_spk.GetHeight(STATCH_EDGE+ROTATED_OFFSET));
				}
				else
				{
					m_image_spk.BltLocked(temp_x, temp_y, START_EDGE_EXTEND+ROTATED_OFFSET);
					temp_y += m_image_spk.GetHeight(START_EDGE_EXTEND+ROTATED_OFFSET);
				}
			}
		}
		
		MItem * p_selected_item = NULL;
		if(m_focus_slot != -1)
		{
			if( g_pArmsBand1 != NULL && m_focus_slot < g_pArmsBand1->GetPocketNumber() )			// armsband 1부터 체크한다.	
				p_selected_item = g_pArmsBand1->GetItem(m_focus_slot);
			else
			{
				if( g_pArmsBand1 != NULL)
					p_selected_item = g_pArmsBand2->GetItem(m_focus_slot - g_pArmsBand1->GetPocketNumber() );
				else
					p_selected_item = g_pArmsBand2->GetItem(m_focus_slot);
			}
//			p_selected_item = g_pQuickSlot->GetItem(m_focus_slot);
		}
		
		if(GetAttributes()->alpha)
		{
			if(m_bl_width)
			{
				RECT alpha_rect;
				alpha_rect.left = temp_x+3;
				alpha_rect.right = temp_x+GetPocketCount()*m_image_spk.GetWidth(SLOT_ALPHA)-4;
				alpha_rect.top = temp_y+9;
				alpha_rect.bottom = temp_y+37;

				DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
			}
			else
			{
				RECT alpha_rect;
				alpha_rect.left = temp_x+9;
				alpha_rect.right = temp_x+37;
				alpha_rect.top = temp_y+3;
				alpha_rect.bottom = temp_y+GetPocketCount()*m_image_spk.GetHeight(SLOT_ALPHA+ROTATED_OFFSET)-4;

				DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
			}
		}

		for (i=0; i < GetPocketCount(); i++)
		{
			if(GetAttributes()->alpha)
			{
				if(m_bl_width)
					m_image_spk.BltLocked(temp_x, temp_y+1, SLOT_ALPHA);
				else
					m_image_spk.BltLocked(temp_x+1, temp_y, SLOT_ALPHA+ROTATED_OFFSET);
			}
			else
			{
				if(m_bl_width)
					m_image_spk.BltLocked(temp_x, temp_y+1, SLOT);
				else
					m_image_spk.BltLocked(temp_x+1, temp_y, SLOT+ROTATED_OFFSET);
			}
			
			const MItem * p_item = NULL;
			
			if( g_pArmsBand1 != NULL && i < g_pArmsBand1->GetPocketNumber() )			// armsband 1부터 체크한다.	
				p_item = g_pArmsBand1->GetItem(i);
			else
			{
				if( g_pArmsBand1 != NULL )
					p_item = g_pArmsBand2->GetItem(i - g_pArmsBand1->GetPocketNumber() );
				else
					p_item = g_pArmsBand2->GetItem(i);
			}
			
			if (p_item != NULL)
			{
				// 아이템 출력
				TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
				
				int _x;
				int _y;
				if(m_bl_width)
				{
					_x = temp_x + QUICKSLOT_W/2-gpC_item->GetWidth(frame_id)/2+3;
					_y = temp_y + QUICKSLOT_H/2-gpC_item->GetHeight(frame_id)/2+8;
				}
				else
				{
					_x = temp_x + QUICKSLOT_W/2-gpC_item->GetWidth(frame_id)/2+8;
					_y = temp_y + QUICKSLOT_H/2-gpC_item->GetHeight(frame_id)/2+3;
				}

				if (p_selected_item && (p_selected_item->GetID() == p_item->GetID()))
				{
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);
					gpC_item->BltLockedOutline(_x, _y, WHITE, frame_id);
				}
				else
				{
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);
					gpC_item->BltLocked(_x, _y, frame_id);
				}
				
				// 개수표시 AlphaBox만 찍고, 숫자는 뒤에서 찍는다
				if(p_item->IsPileItem())
				{
					
					for(int depth = 0, number = p_item->GetNumber(); number > 0; number/=10, depth++);
					if(depth == 0) depth = 1;

					rect[len].right = temp_x + QUICKSLOT_W+2;
					rect[len].left = rect[len].right - 7*depth;
					rect[len].bottom = temp_y+QUICKSLOT_H+7;
					rect[len].top = rect[len].bottom - 12;
					if(!m_bl_width)
					{
						rect[len].right += 5;
						rect[len].left += 5;
						rect[len].top -= 5;
						rect[len].bottom -= 5;
					}
					
					DrawAlphaBox(&rect[len], 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
					num[len] = p_item->GetNumber();
					len++;
					
				}
				
			}
			else
				// 놓는 위치 미리 알 수 있도록 한다.
				if (gpC_mouse_pointer->GetPickUpItem() && 
					m_focus_slot == i)
				{
					MItem * p_item = gpC_mouse_pointer->GetPickUpItem();
					
					TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
					
					int _x;
					int _y;
					if(m_bl_width)
					{
						_x = temp_x + QUICKSLOT_W/2-gpC_item->GetWidth(frame_id)/2+3;
						_y = temp_y + QUICKSLOT_H/2-gpC_item->GetHeight(frame_id)/2+8;
					}
					else
					{
						_x = temp_x + QUICKSLOT_W/2-gpC_item->GetWidth(frame_id)/2+8;
						_y = temp_y + QUICKSLOT_H/2-gpC_item->GetHeight(frame_id)/2+3;
					}
					
					MItem * p_old_item;
					//if (g_pQuickSlot->CanReplaceItem(p_item, m_focus_slot, p_old_item))
					
					MOustersArmsBand *pQuickSlot;
					
					if( g_pArmsBand1 != NULL && g_pArmsBand1->GetPocketNumber() > m_focus_slot )
						pQuickSlot = g_pArmsBand1;
					else
						pQuickSlot = g_pArmsBand2;
					
					int skip_slot = 0;

					if( pQuickSlot == g_pArmsBand2 && g_pArmsBand1 != NULL )
						skip_slot = g_pArmsBand1->GetPocketNumber();

					if(pQuickSlot!= NULL && pQuickSlot->CanReplaceItem( p_item, m_focus_slot - skip_slot, p_old_item ) )
					{
						if (p_old_item)
							gpC_item->BltLockedOutlineOnly(_x, _y, ga_item_blink_color_table[g_blink_value], frame_id);
						else
							gpC_item->BltLockedOutlineOnly(_x, _y, POSSIBLE_COLOR, frame_id);
					}
				}
				
				
				if(m_bl_width)
					temp_x += m_image_spk.GetWidth(SLOT);
				else
					temp_y += m_image_spk.GetHeight(SLOT+ROTATED_OFFSET);
		}
		
		if(m_bl_width)
		{
			if(bl_statch)
				m_image_spk.BltLocked(temp_x, temp_y+1, STATCH_EDGE);
			else
			{
				m_image_spk.BltLocked(temp_x, temp_y, END_EDGE_EXTEND);
			}
		}
		else
		{
			if(bl_statch)
			{
				m_image_spk.BltLocked(temp_x, temp_y, END_EDGE_EXTEND+ROTATED_OFFSET);
			}
			else
			{
				if(y+h >= g_GameRect.bottom)
					m_image_spk.BltLocked(temp_x+1, temp_y, STATCH_EDGE+ROTATED_OFFSET);
				else
					m_image_spk.BltLocked(temp_x, temp_y, END_EDGE+ROTATED_OFFSET);
			}
		}
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	if(Moving() && m_bl_resize)
	{
		if(m_bl_width)
		{
			if(x+w == g_GameRect.right)
				x += m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE);
		}
		else 
		{
			if(y != 0)//+h == g_GameRect.bottom)
				y += m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE);
		}
	}	
	
	if(Moving() && m_bl_resize)
	{
		if(m_bl_width)
		{
			if(x+w == g_GameRect.right)
				x -= m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE);
		}
		else 
		{
			if(y != 0)//+h == g_GameRect.bottom)
				y -= m_image_spk.GetWidth(START_EDGE) - m_image_spk.GetWidth(STATCH_EDGE);
		}
	}

	COLORREF markColor = RGB(220, 220, 220);;//RGB(140, 140, 255);
	char sz_temp[10];
	g_FL2_GetDC();
	for(i = 0; i < len; i++)
	{
		wsprintf(sz_temp, "%d", num[i]);
		g_PrintColorStr(rect[i].left, rect[i].top, sz_temp, gpC_base->m_item_desc_pi, markColor);	
//		g_Print(rect.left, rect.top, sz_temp, &gpC_base->m_item_desc_pi);	
	}
	m_pC_button_group->ShowDescription();
	g_FL2_ReleaseDC();
	

	// show hot-key
	char * p_hotkey_mark[] = {"F1", "F2", "F3", "F4", "F5", "F6"};
	if (m_focus_slot != NOT_SELECTED)
	//for (i=0; i < GetPocketCount(); i++)
	{
		POINT point;
		if(m_bl_width)
		{
			point.x = x+m_p_slot_x[m_focus_slot]+1;
			point.y = y+7;
		}
		else
		{
			point.x = x+8;
			point.y = y+m_p_slot_x[m_focus_slot];
		}

		// 검은색 박스 출력..
//		if (gpC_base->m_p_DDSurface_back->Lock())
		{
			RECT rect;

			rect.left = point.x;
			rect.top = point.y+18;
			rect.right = rect.left + 14;
			rect.bottom = rect.top + 12;

			DrawAlphaBox(&rect, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);

//			gpC_base->m_p_DDSurface_back->Unlock();
		}

		COLORREF markColor = RGB(140, 140, 255);
		g_PrintColorStr(point.x, point.y+18, p_hotkey_mark[m_focus_slot], gpC_base->m_item_desc_pi, markColor);	
	}

}

bool C_VS_UI_OUSTERS_QUICKITEM::Click()
{
	if (m_focus_slot == NOT_SELECTED || gbl_item_lock || 
		(g_pArmsBand1 == NULL && g_pArmsBand2 == NULL) )
//		 g_pQuickSlot == NULL)
	{
		return false;
	}

	int item_x, item_y;

	if (gpC_mouse_pointer->GetPickUpItem()) // 들고 있는가?
	{
		MItem* p_cur_item = NULL;
		
		if( g_pArmsBand1 != NULL && g_pArmsBand1->GetPocketNumber() > m_focus_slot )
			p_cur_item = g_pArmsBand1->GetItem( m_focus_slot );
		else
		{
			if( g_pArmsBand1 != NULL )
				p_cur_item = g_pArmsBand2->GetItem( m_focus_slot - g_pArmsBand1->GetPocketNumber() );
			else
				p_cur_item = g_pArmsBand2->GetItem( m_focus_slot );
		}

		// 총에 탄창을 끼우는 것과 같은 것이 insert item이다.
		if (gpC_mouse_pointer->GetPickUpItem()->IsInsertToItem( p_cur_item ))
		{
			// 이전에 있던 item에 추가될 수 있는 경우

			// 대상 Item과 들고 있는(추가할 Item)을 Client에서 알아야 한다.
			// 들고 있는 Item은 Client에서 access할 수 있으므로 대상 Item을 보낸다.
			gpC_base->SendMessage(UI_ITEM_INSERT_FROM_QUICKSLOT,
																  m_focus_slot,
																  0,
																  (void *)p_cur_item); // 대상 Item
			return true;
		}
		else
		{
			MItem * p_old_item = NULL;

//			if (g_pQuickSlot->CanReplaceItem(gpC_mouse_pointer->GetPickUpItem(),
//												  m_focus_slot,
//												  p_old_item))
			MOustersArmsBand *pQuickSlot = NULL;
			if( g_pArmsBand1 != NULL && g_pArmsBand1->GetPocketNumber() > m_focus_slot )
				pQuickSlot = g_pArmsBand1;
			else
				pQuickSlot = g_pArmsBand2;

			int skip_slot = 0;

			if( pQuickSlot == g_pArmsBand2 && g_pArmsBand1 != NULL)
				skip_slot = g_pArmsBand1->GetPocketNumber();				

			if( pQuickSlot != NULL && pQuickSlot->CanReplaceItem( gpC_mouse_pointer->GetPickUpItem(),
				m_focus_slot - skip_slot, p_old_item ) )
			{
				gpC_base->SendMessage(UI_ITEM_DROP_TO_QUICKSLOT, m_focus_slot);
				return true;
			}
			else
			{
				// failed.
				return false;
			}
		}
	}
	else
	{
		// 집는다.
		//const MItem * p_item = g_pQuickSlot->GetItem(m_focus_slot);
		MOustersArmsBand * pQuickSlot = NULL;
		MItem *p_item = NULL;
		int slot = m_focus_slot;
		
		if( g_pArmsBand1 == NULL )
			pQuickSlot = g_pArmsBand2;
		else
		{
			if( g_pArmsBand1->GetPocketNumber() > slot )
			{
				pQuickSlot = g_pArmsBand1;
			} else
			{
				pQuickSlot = g_pArmsBand2;
				slot -= g_pArmsBand1->GetPocketNumber();
			}
		}
		
		if( pQuickSlot == NULL )
			return false;

		p_item = pQuickSlot->GetItem( slot );

		if (p_item != NULL) // Item이 있다.
		{
			if(m_bl_width)
			{
				item_x = x+m_p_slot_x[m_focus_slot]+QUICKSLOT_W/2-gpC_item->GetWidth(p_item->GetInventoryFrameID())/2;
				item_y = y+QUICKSLOT_H/2-gpC_item->GetHeight(p_item->GetInventoryFrameID())/2;
			}
			else
			{
				item_x = x+QUICKSLOT_W/2-gpC_item->GetWidth(p_item->GetInventoryFrameID())/2;
				item_y = y+m_p_slot_x[m_focus_slot]+QUICKSLOT_H/2-gpC_item->GetHeight(p_item->GetInventoryFrameID())/2;
			}

			//g_pQuickSlot->RemoveItem((BYTE)m_focus_slot);
			pQuickSlot->RemoveItem( (BYTE) slot );

			gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_QUICKSLOT,
																  m_focus_slot, 
																  MAKEDWORD(item_x, item_y), 
																  (MItem *)p_item);
			// SHIFT누르고 클릭했을때 아이템 자동 이동 처리 -> 인벤토리
			if(g_pDXInput->KeyDown(DIK_LSHIFT))
			{
				POINT point;
				if(p_item != NULL && g_pInventory->GetFitPosition((MItem *)p_item, point))
				{
					const MItem* p_cur_item = g_pInventory->GetItem(point.x, point.y);
					
					// 총에 탄창을 끼우는 것과 같은 것이 insert item이다.
					// 위치가 완전히 일치할경우에만 추가한다.
					if (p_item->IsInsertToItem( p_cur_item ) && p_cur_item->GetGridX() == point.x && p_cur_item->GetGridY() == point.y)
					{
						// 이전에 있던 item에 추가될 수 있는 경우
						
						// 대상 Item과 들고 있는(추가할 Item)을 Client에서 알아야 한다.
						// 들고 있는 Item은 Client에서 access할 수 있으므로 대상 Item을 보낸다.
						gpC_base->SendMessage(UI_ITEM_INSERT_FROM_INVENTORY,
							point.x, point.y,
							(void *)p_cur_item); // 대상 Item
					}
					else
					{	
						// 추가될 수 없는 경우
						MItem* p_old_item  = NULL;
						
						if (g_pInventory->CanReplaceItem((MItem *)p_item,		// 추가할 item
							point.x, point.y,	// 추가할 위치 
							p_old_item))								// 원래있던 item
						{
							
							gpC_base->SendMessage(UI_ITEM_DROP_TO_INVENTORY, 
								point.x, point.y,
								(void *)p_item);
							
						}
						else
						{
							// failed.
							return false;
						}
					}
					
				}
			}
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Use
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OUSTERS_QUICKITEM::Use(int slot, bool bFunctionKey)
{
	if (slot == NOT_SELECTED || gbl_item_lock || g_pArmsBand1 == NULL && g_pArmsBand2 == NULL)
		return;

	if (gpC_mouse_pointer->GetPickUpItem())
	{

	}
	else
	{
		const MItem * p_item = GetItem( slot );

		if (p_item) // Item이 있다.
		{
			gpC_base->SendMessage(UI_ITEM_USE_QUICKSLOT,
									  slot,
									  bFunctionKey, 
									  (MItem *)p_item);
		}
	}
}

void	C_VS_UI_OUSTERS::RunQuickItemSlot()
{
	if( m_pC_armsband != NULL )
	{
		m_pC_armsband->Open();
	}
}

void	C_VS_UI_OUSTERS::CloseQuickItemSlot()
{
	if( m_pC_armsband != NULL )
	{
		m_pC_armsband->Close();
	}
}

bool	C_VS_UI_OUSTERS::IsRunningQuickItemSlot()
{
	return true;
}

void	C_VS_UI_OUSTERS::ResetOustersQuickItemSize()
{
	m_pC_armsband->ResetSize();
}

void	C_VS_UI_OUSTERS::HotKey_QuickItemSlot()
{
	assert(m_pC_armsband != NULL);

	m_pC_armsband->ToggleBelt();
}