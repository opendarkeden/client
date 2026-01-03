// VS_UI_GameVampire.cpp

#include "client_PCH.h"
#include "VS_UI_GameVampire.h"
#include "VS_UI_filepath.h"
#include "VS_UI_Mouse_pointer.h"
#include "VS_UI.h"
#include "UserInformation.h"
extern RECT g_GameRect;
int		C_VS_UI_VAMPIRE_GEAR::m_slot_image[SLOT_SIZE] = {
	NECKLACE, COAT, BRACELET, BRACELET, RING, RING, RING, RING, EARRING, EARRING, WEAPON, WEAPON, AMULET, AMULET, AMULET, AMULET
	,-1
	,-1
	,-1
	,-1
	,DERMIS,PERSONA,BLOODBIBLE,BLOODBIBLE,BLOODBIBLE,BLOODBIBLE,BLOODBIBLE,BLOODBIBLE
};


//
// help string
//
//
// help string
//
/*static S_DEFAULT_HELP_STRING	g_help_string[9] = {
	{"Inventory", "(아이템창)", "TAB"},
	{"PartyManager", "(파티창)", "Ctrl+P"},
	{"Character Info", "(캐릭터 정보)", "Ctrl+I"},
	{"Magic Info", "(마법 정보)", "Ctrl+K"},
	{"Help", "(도움말)", "Ctrl+H"},
	{"Gear", "(장착창)", "Ctrl+TAB"},
	{"Game Menu", "(게임 메뉴)", "ESC"},
	{"Mini-Map", "(미니맵)", "Ctrl+M"},
	{"Mark", "(특수문자)", "Ctrl+X"},
};

static S_DEFAULT_HELP_STRING	g_chat_help_string[5] = {
	{"Extend Window", "(채팅창 확장)", "Ctrl+E"},
	{"Chat", "(일반 채팅)", "Ctrl+C"},
	{"Clan/Party Chat", "(클랜/파티 채팅)", "Ctrl+G"},
	{"Whisper", "(귓속말)", "Ctrl+W"},
	{"ZoneChat", "(외치기)", "Ctrl+Z"},
};*/

//----------------------------------------------------------------------------
// Operations
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_VAMPIRE::C_VS_UI_VAMPIRE():C_VS_UI_TRIBE()
{
	m_pC_main_spk = new C_SPRITE_PACK(SPK_VAMPIRE_MAIN);

	Set(g_GameRect.right-m_pC_main_spk->GetWidth()+1, g_GameRect.bottom-m_pC_main_spk->GetHeight()+1, m_pC_main_spk->GetWidth()-1, m_pC_main_spk->GetHeight()-1);

	m_pC_sys_button_spk = new C_SPRITE_PACK(SPK_VAMPIRE_SYS_BUTTON);

	// common buttons
	int tab_x = 18, tab_y = 55;
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_MENU), m_pC_sys_button_spk->GetHeight(TAB_MENU), TAB_MENU_ID, this, TAB_MENU) );
	tab_x += m_pC_sys_button_spk->GetWidth(TAB_MENU);
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_GUILD), m_pC_sys_button_spk->GetHeight(TAB_GUILD), TAB_GUILD_ID, this, TAB_GUILD) );
	tab_x += m_pC_sys_button_spk->GetWidth(TAB_GUILD);
//	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_MSG), m_pC_sys_button_spk->GetHeight(TAB_MSG), TAB_MSG_ID, this, TAB_MSG) );
//	tab_x += m_pC_sys_button_spk->GetWidth(TAB_MSG);
	// 2004, 12, 2, sobeit add start - 기능
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_GUILD), m_pC_sys_button_spk->GetHeight(TAB_UTIL), TAB_UTIL_ID, this, TAB_UTIL) );
	tab_x += m_pC_sys_button_spk->GetWidth(TAB_UTIL);
	// 2004, 12, 2, sobeit add end
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(tab_x, tab_y, m_pC_sys_button_spk->GetWidth(TAB_HELP), m_pC_sys_button_spk->GetHeight(TAB_HELP), TAB_HELP_ID, this, TAB_HELP) );

	int system_x = w-m_pC_main_spk->GetWidth(BUTTON_SYSTEM)-5, system_y = h-m_pC_main_spk->GetHeight(BUTTON_SYSTEM);
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(system_x, system_y, m_pC_main_spk->GetWidth(BUTTON_SYSTEM), m_pC_main_spk->GetHeight(BUTTON_SYSTEM), SYSTEM_ID, this, BUTTON_SYSTEM) );
	m_pC_common_button_group->Add( new C_VS_UI_EVENT_BUTTON(24, system_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), PUSHPIN_ID, this,C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN ) );

	const int button_x = 24, button_y = 78, button_x_gap = 37, button_y_gap = 37;
	
	// menu buttons
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_INVENTORY), m_pC_sys_button_spk->GetHeight(BUTTON_INVENTORY), INVENTORY_ID, this, BUTTON_INVENTORY) );
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_GEAR), m_pC_sys_button_spk->GetHeight(BUTTON_GEAR), GEAR_ID, this, BUTTON_GEAR) );
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*2, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_INFO), m_pC_sys_button_spk->GetHeight(BUTTON_INFO), INFO_ID, this, BUTTON_INFO) );
	//add by viva : friend button
#ifdef __FRIEND_SYSTEM_VIVA__ 
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*3, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_FRIEND), m_pC_sys_button_spk->GetHeight(BUTTON_FRIEND), FRIEND_ID, this, BUTTON_FRIEND) );	
#endif
	//end
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_PARTY), m_pC_sys_button_spk->GetHeight(BUTTON_PARTY), PARTY_ID, this, BUTTON_PARTY) );
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_QUEST), m_pC_sys_button_spk->GetHeight(BUTTON_QUEST), QUEST_ID, this, BUTTON_QUEST) );
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*2, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_MAIL), m_pC_sys_button_spk->GetHeight(BUTTON_MAIL), MAIL_ID, this, BUTTON_MAIL) );

	// sms 버튼 
//add by zdj 2005.5.17
// add by Coffee 2006.11.26
//	if(false == g_pUserInformation->IsNetmarble)
		//m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*3, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_SMS), m_pC_sys_button_spk->GetHeight(BUTTON_SMS), SMS_ID, this, BUTTON_SMS) );
	
	m_pC_menu_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*3, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_NAMING), m_pC_sys_button_spk->GetHeight(BUTTON_NAMING), NAMING_ID, this, BUTTON_NAMING) );

	// guild buttons
	m_pC_guild_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y+button_y_gap*0, m_pC_sys_button_spk->GetWidth(BUTTON_TEAM_INFO), m_pC_sys_button_spk->GetHeight(BUTTON_TEAM_INFO), TEAM_INFO_ID, this, BUTTON_TEAM_INFO) );
	m_pC_guild_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y+button_y_gap*0, m_pC_sys_button_spk->GetWidth(BUTTON_TEAM_MEMBER_LIST), m_pC_sys_button_spk->GetHeight(BUTTON_TEAM_MEMBER_LIST), TEAM_MEMBER_LIST_ID, this, BUTTON_TEAM_MEMBER_LIST) );
	// 2004, 10, 12, sobeit add start - 길드 명령 아이콘
//	if(false == g_pUserInformation->IsNetmarble) // 2005, 1, 12, sobeit modify - 넷마블 길드 연합 들어감
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
		// edit by Coffee 2006.11.19  녜쉽溝固
		m_pC_util_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_POWER_JJANG), m_pC_sys_button_spk->GetHeight(BUTTON_POWER_JJANG), UTIL_POWER_JJANG_ID, this, BUTTON_POWER_JJANG) );
		// End by Coffee 2006.11.19
	}

	// 2004, 12, 2, sobeit add end

	// help buttons
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_HELP), m_pC_sys_button_spk->GetHeight(BUTTON_HELP), HELP_ID, this, BUTTON_HELP) );
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_CHAT_HELP), m_pC_sys_button_spk->GetHeight(BUTTON_CHAT_HELP), CHAT_HELP_ID, this, BUTTON_CHAT_HELP) );
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*2, button_y, m_pC_sys_button_spk->GetWidth(BUTTON_BATTLE_HELP), m_pC_sys_button_spk->GetHeight(BUTTON_BATTLE_HELP), BATTLE_HELP_ID, this, BUTTON_BATTLE_HELP) );
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*0, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_SKILL_HELP), m_pC_sys_button_spk->GetHeight(BUTTON_SKILL_HELP), SKILL_HELP_ID, this, BUTTON_SKILL_HELP) );
	m_pC_help_button_group->Add( new C_VS_UI_EVENT_BUTTON(button_x+button_x_gap*1, button_y+button_y_gap, m_pC_sys_button_spk->GetWidth(BUTTON_TEAM_MEMBER_LIST), m_pC_sys_button_spk->GetHeight(BUTTON_TEAM_MEMBER_LIST), GUILD_HELP_ID, this, BUTTON_TEAM_MEMBER_LIST) );

	// instance objects 
	m_pC_gear = new C_VS_UI_VAMPIRE_GEAR;
	
	m_time = "";
	m_date = "";

#ifndef _LIB
	SetTime("12:34:05");
	SetDate("2001/06/30");
#endif
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_VAMPIRE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_VAMPIRE::~C_VS_UI_VAMPIRE()
{
	DeleteNew(m_pC_gear);
}

//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE::SkillTreeRunning
//
// 
//-----------------------------------------------------------------------------
//bool	C_VS_UI_VAMPIRE::SkillTreeRunning() const
//{
//	return (m_pC_skilltree_vampire != NULL);
//}

/*
//-----------------------------------------------------------------------------
// DrawEnergy
//
// HP를 출력한다.
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::DrawEnergy()
{
	POINT point = {3,30};
	RECT rt = {0,0,m_p_time_surface->GetWidth(),m_p_time_surface->GetHeight()};
	gpC_base->m_p_DDSurface_back->Blt(&point, m_p_time_surface, &rt);

	// draw energy 'case'
	m_pC_etc_spk->Blt(ENERGY_CASE_X, ENERGY_CASE_Y, ENERGY_CASE);

	Rect rect(0, 0, m_hp_line, m_pC_etc_spk->GetHeight(ENERGY_HP));
	m_pC_etc_spk->BltClip(ENERGY_CASE_X+5, ENERGY_CASE_Y+22, rect, ENERGY_HP);

	if(m_silver)
	{
		Rect rect(m_silver_hp_line, 0, m_pC_etc_spk->GetWidth(ENERGY_HP)-m_silver_hp_line, m_pC_etc_spk->GetHeight(ENERGY_HP));
		m_pC_etc_spk->BltClip(ENERGY_CASE_X+5, ENERGY_CASE_Y+22, rect, ENERGY_HP_SILVER);
	}

	const int date_x = 14, date_y = ENERGY_CASE_Y+49;
	const int time_x = 14, time_y = ENERGY_CASE_Y+71;

	g_FL2_GetDC();

	g_PrintColorStr(date_x +68 - g_GetStringWidth(m_date.c_str(), gpC_base->m_info_pi.hfont)/2, date_y, m_date.c_str() ,gpC_base->m_info_pi, RGB_WHITE);	
	g_PrintColorStr(time_x + 40 - g_GetStringWidth(m_time.c_str(), gpC_base->m_info_pi.hfont)/2, time_y, m_time.c_str(), gpC_base->m_info_pi, RGB_WHITE);	

	g_FL2_ReleaseDC();

	
	// 이제 이거 출력안할때가 된거 같아서.. by sigi
//	char sz_info_buf[15];
//	sprintf(sz_info_buf, "(%d/%d)", m_hp_cur, m_hp_max);
//	g_Print(270, 9, sz_info_buf);
	
}
*/
/*
//-----------------------------------------------------------------------------
// DrawMinimap
//
// 미니맵, 좌표, 존이름을 표시한다
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::DrawMinimap()
{
	int _x, _y, map_x_x, map_x_y, map_y_x, map_y_y;

	if(!m_bl_minimap)
	{
		_x = g_GameRect.right - m_pC_etc_spk->GetWidth(MINIMAP_CASE);
		_y = 0;

		m_pC_etc_spk->Blt(_x, _y, MINIMAP_CASE);
		
		map_x_x = _x+71;
		map_x_y = _y + 24;
		map_y_x = map_x_x;
		map_y_y = _y + 45;
	}
	else
	{
		_x = g_GameRect.right - m_pC_etc_spk->GetWidth(MINIMAP_CASE_EXTEND);
		_y = 0;

		m_pC_etc_spk->Blt(_x, _y, MINIMAP_CASE_EXTEND);

		RECT rt = {0,0,m_p_minimap_surface->GetWidth(),m_p_minimap_surface->GetHeight()};
		gpC_base->m_p_DDSurface_back->Blt(&m_map_start_point, m_p_minimap_surface, &rt);
		
		map_x_x = _x+71;
		map_x_y = _y + 24+12;
		map_y_x = map_x_x;
		map_y_y = _y + 45+12;

		int map_w = 200, map_h = 100;
		if(m_map_w != m_map_h)
		{
			if(m_map_w > m_map_h)map_h = map_h * m_map_h / m_map_w;
			if(m_map_h > m_map_w)map_w = map_w * m_map_w / m_map_h;
		}

		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			WORD *mem = (WORD *)gpC_base->m_p_DDSurface_back->GetSurfacePointer();
			long pitch = gpC_base->m_p_DDSurface_back->GetSurfacePitch();

			int map_x = m_map_start_point.x + m_map_x*map_w/m_map_w + (200 - map_w)/2,
				map_y = m_map_start_point.y + m_map_y*map_h/m_map_h + (100 - map_h)/2;

			mem[(map_y)*pitch/2 + map_x] = 0xffff;
			mem[(map_y+1)*pitch/2 + map_x+1] = 0xffff;
			mem[(map_y-1)*pitch/2 + map_x-1] = 0xffff;
			mem[(map_y+1)*pitch/2 + map_x-1] = 0xffff;
			mem[(map_y-1)*pitch/2 + map_x+1] = 0xffff;

			static int color=21;
			static int color_direct = 1;
			if(Timer())
			{
				color += color_direct;
				if(color > 30 || color < 21)color_direct *= -1;
			}

			BYTE r = 31, g = 63, b = 31;
			WORD portal_color = CDirectDraw::Color(r, g, b);

			int i, _x, _y;
			WORD _color, _color2;

			r = color, g = color-10, b = color;
			_color = CDirectDraw::Color(r, g, b);
			r = color*2/3, g = (color-10)*2/3, b = color*2/3;
			_color2 = CDirectDraw::Color(r, g, b);
			for(i = 0; i < m_portal.size(); i++)
			{
				_x = m_map_start_point.x + (m_portal[i].left+m_portal[i].right)/2*map_w/m_map_w + (200 - map_w)/2;
				_y = m_map_start_point.y + (m_portal[i].top+m_portal[i].bottom)/2*map_h/m_map_h + (100 - map_h)/2;

				mem[(_y)*pitch/2 + _x] = _color;
				mem[(_y-1)*pitch/2 + _x-1] = _color2;
				mem[(_y-1)*pitch/2 + _x] = _color;
				mem[(_y-1)*pitch/2 + _x+1] = _color2;
				mem[(_y-2)*pitch/2 + _x-1] = _color;
				mem[(_y-2)*pitch/2 + _x] = _color;
				mem[(_y-2)*pitch/2 + _x+1] = _color;

			}

			r = color-10, g = color, b = color-10;
			_color = CDirectDraw::Color(r, g, b);
			for(i = 0; i < m_npc.size(); i++)
			{
				_x = m_map_start_point.x + m_npc[i].x*map_w/m_map_w + (200 - map_w)/2;
				_y = m_map_start_point.y + m_npc[i].y*map_h/m_map_h + (100 - map_h)/2;

				mem[(_y)*pitch/2 + _x] = _color;
				mem[(_y-1)*pitch/2 + _x] = _color;
				mem[(_y-2)*pitch/2 + _x-1] = _color;
				mem[(_y-2)*pitch/2 + _x] = _color;
				mem[(_y-2)*pitch/2 + _x+1] = _color;
				mem[(_y-3)*pitch/2 + _x] = _color;
			}
			gpC_base->m_p_DDSurface_back->Unlock();
		}

	}
	
	char sz_temp[100];


//	m_map_x = 125;
//	m_map_y = 208;

	g_FL2_GetDC();

	if(m_bl_minimap)
	{
		g_PrintColorStr(g_GameRect.right - 115 - g_GetStringWidth(m_zone_name.c_str(), gpC_base->m_info_pi.hfont)/2, _y+9, m_zone_name.c_str(), gpC_base->m_info_pi, RGB_WHITE);	
	}

	wsprintf(sz_temp, "%d", m_map_x);
	g_PrintColorStr(map_x_x - g_GetStringWidth(sz_temp, gpC_base->m_info_pi.hfont)/2, map_x_y, sz_temp, gpC_base->m_info_pi, RGB_WHITE);	
	wsprintf(sz_temp, "%d", m_map_y);
	g_PrintColorStr(map_y_x - g_GetStringWidth(sz_temp, gpC_base->m_info_pi.hfont)/2, map_y_y, sz_temp, gpC_base->m_info_pi, RGB_WHITE);	

	g_FL2_ReleaseDC();


}
*/
/*
//-----------------------------------------------------------------------------
// SetZone
//
// ZoneID를 받아서 미니맵을 불러온다 & 존이름 세팅
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::SetZone(int zone_id)
{
	const int id_size = 39;
	int id[id_size] = {11, 12, 13, 14, 21, 22, 23 ,24, 31, 32, 33, 34, 1001, 1002, 1003, 1004, 1005, 1006, 2000, 2001, 2002, 2003, 2004, 2010, 2011, 2012, 2013, 2014, 2020, 2021, 2022, 2023, 2024, 2101, 2102, 2103, 2104, 2105, 2106};
	char map_name[id_size][20] =
	{
		// 필드
		"에슬라니아 NE", "에슬라니아 NW", "에슬라니아 SE", "에슬라니아 SW",
		"림보성 NE", "림보성 NW", "림보성 SE", "림보성 SW",
		"드로베타 NE", "드로베타 NW", "드로베타 SE", "드로베타 SW",

		// 던젼 & 마을
		"지하 수련장", "에슬라니아 던전", "뱀파이어 마을", "림보성 던전", "이벤트 OX", "이벤트 경기장",

		// 건물
		"군인 길드 B1", "군인 길드 1F", "군인 길드 2F", "군인 길드 3F", "군인 길드 옥상",
		"성직자 길드 B1", "성직자 길드 1F", "성직자 길드 2F", "성직자 길드 3F", "성직자 길드 옥상",
		"무사 길드 B1", "무사 길드 1F", "무사 길드 2F", "무사 길드 3F", "무사 길드 옥상",

		// 튜토리얼
		"복도", "브리핑룸", "컴퓨터실", "무기점", "도서관", "헬기장",
	};

	int spk_id[id_size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
							12, 13, 14, 13, 15, 27, 
							26, 16, 17, 18, 24,
							26, 28, 29, 30, 24,
							26, 31, 32, 33, 24,
							19, 20, 21, 22, 23, 25};

	for(int i = 0; i < id_size; i++)
		if(zone_id == id[i])break;

	if(i == id_size)return;
	m_zone_name = map_name[i];

	CSpritePack		m_minimap_spk;
	
	ifstream	file(SPK_MINIMAP, ios::binary|ios::nocreate);
	if (!file)
		_Error(FILE_OPEN);
	m_minimap_spk.LoadFromFile(file);
	file.close();

	POINT point = {0, 0};

	m_minimap_surface.FillSurface(0x0000);
	m_minimap_surface.SetTransparency( 0xffff );

	m_minimap_surface.Lock();
	m_minimap_surface.BltSprite(&point, &m_minimap_spk[spk_id[i]]);
	m_minimap_surface.Unlock();

	m_minimap_spk.Release();
}

//-----------------------------------------------------------------------------
// SetSafetyZone
//
// 안전지대 설정
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::SetSafetyZone(RECT rect)
{
	m_minimap_surface.Lock();
	WORD *mem = (WORD *)m_minimap_surface.GetSurfacePointer();
	int pitch = m_minimap_surface.GetSurfacePitch();

	int map_w = 200, map_h = 100;
	if(m_map_w != m_map_h)
	{
		if(m_map_w > m_map_h)map_h = map_h * m_map_h / m_map_w;
		if(m_map_h > m_map_w)map_w = map_w * m_map_w / m_map_h;
	}
	
	for(int y = rect.top*map_h/m_map_h + (100 - map_h)/2; y <= rect.bottom*map_h/m_map_h + (100 - map_h)/2; y++)
	{
		for(int x = rect.left*map_w/m_map_w + (200 - map_w)/2; x <= rect.right*map_w/m_map_w + (200 - map_w)/2; x++)
		{
			mem[y*pitch/2+x] = mem[y*pitch/2+x] & CDirectDraw::Get_G_Bitmask();
		}
	}

	m_minimap_surface.Unlock();
}
*/

//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE::GetGearWindow
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_GEAR * C_VS_UI_VAMPIRE::GetGearWindow() const
{
	return m_pC_gear;
}

//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE::CloseGear
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_VAMPIRE::CloseGear()
{
	m_pC_gear->Finish();
}

//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE::OpenGear
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_VAMPIRE::OpenGear(bool bl_set_load)
{
	m_pC_gear->Start(bl_set_load);
}

//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE::CloseSkillTree
//
// 
//-----------------------------------------------------------------------------
//void	C_VS_UI_VAMPIRE::CloseSkillTree(SKILL_TREE_ARG arg)
//{
//	DeleteNew(m_pC_skilltree_vampire);
//}

//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE::RunSkillTree
//
// 
//-----------------------------------------------------------------------------
//void	C_VS_UI_VAMPIRE::RunSkillTree(SKILL_TREE_ARG arg)
//{
//	switch (arg)
//	{
//		case SKILLTREE_VAMPIRE:
//			if (m_pC_skilltree_vampire == NULL)
//			{
//				m_pC_skilltree_vampire = new C_VS_UI_SKILL_TREE_VAMPIRE;
//
//				m_pC_skilltree_vampire->Start();
//			}
//			break;
//	}
//}

//-----------------------------------------------------------------------------
// DoCommonActionBeforeEventOccured
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::DoCommonActionBeforeEventOccured()
{
	C_VS_UI_TRIBE::DoCommonActionBeforeEventOccured();
}

//-----------------------------------------------------------------------------
// DoCommonActionAfterEventOccured
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::DoCommonActionAfterEventOccured()
{
	C_VS_UI_TRIBE::DoCommonActionAfterEventOccured();
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::Start()
{
	C_VS_UI_TRIBE::Start();

	m_pC_skill->Start();
	m_pC_hpbar->Start();
	m_pC_effect_status->Start();
	m_pC_minimap->Start();

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
void C_VS_UI_VAMPIRE::Show()
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
		
		const int icon_x = 88, icon_y = 35;
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

		default:
			gpC_base->m_p_DDSurface_back->Unlock();
			break;
		}

	}	

	g_FL2_GetDC();				
	g_PrintColorStr(x + 128 - g_GetStringWidth(m_date.c_str(), gpC_base->m_chatting_pi.hfont)/2, y+16, m_date.c_str() ,gpC_base->m_chatting_pi, RGB_WHITE);	
	g_PrintColorStr(x + 6+128 - g_GetStringWidth(m_time.c_str(), gpC_base->m_chatting_pi.hfont)/2, y+37, m_time.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);	
	m_pC_common_button_group->ShowDescription();
	g_FL2_ReleaseDC();

	m_pC_skill->Show2();

	SHOW_WINDOW_ATTR;

}
/*
//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE::SetTime
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_VAMPIRE::SetTime(const char *str)
{
	if(str[0] != m_time[0] || str[1] != m_time[1])
	{
		CSpritePack		m_time_spk;
		
		ifstream	file(SPK_TIME, ios::binary|ios::nocreate);
		if (!file)
			_Error(FILE_OPEN);
		m_time_spk.LoadFromFile(file);
		file.close();
		
		POINT point = {0,0};
		RECT rect = {0,0,m_time_spk[0].GetWidth(),m_time_spk[0].GetHeight()};
		
		int time;

		time = str[0] - '0';
		if(str[1] != ':') time = time*10 + str[1]-'0';
		
		time = (time + 3)%24;

		int sprite = time % 6;
		int rotate = time/6;
		
		m_time_surface.FillSurface(0x001f);
		
		m_time_surface.Lock();
		
		WORD* surface = (WORD *)m_time_surface.GetSurfacePointer();
		int pitch = m_time_surface.GetSurfacePitch()/2;
		
		switch(rotate)
		{
		case 0:
			m_time_surface.BltSprite(&point, &m_time_spk[sprite]);
			break;
			
		case 1:
			{
				for(int i = 0, x = rect.right -1; i < rect.bottom; i++, x--)
					for(int j = 0, y = 0; j < rect.right; j++, y++)
					{
						if(m_time_spk[sprite].IsColorPixel(j, i))
						{
							surface[pitch * y + x] = m_time_spk[sprite].GetPixel(j, i);
						}
						
					}
			}
			break;

		case 2:
			{
				for(int i = 0, y = rect.bottom -1; i < rect.bottom; i++, y--)
					for(int j = 0, x = rect.right -1; j < rect.right; j++, x--)
					{
						if(m_time_spk[sprite].IsColorPixel(j, i))
						{
							surface[pitch * y + x] = m_time_spk[sprite].GetPixel(j, i);
						}
						
					}
			}
			break;

		case 3:
			{
				for(int i = 0, x = 0; i < rect.bottom; i++, x++)
					for(int j = 0, y = rect.bottom - 1; j < rect.right; j++, y--)
					{
						if(m_time_spk[sprite].IsColorPixel(j, i))
						{
							surface[pitch * y + x] = m_time_spk[sprite].GetPixel(j, i);
						}
						
					}
			}
			break;
		}

		m_time_surface.Unlock();

		m_time_spk.Release();
	}

	m_time = str;
}
*/
//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::Process()
{
	C_VS_UI_TRIBE::Process();
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
//bool C_VS_UI_VAMPIRE::MouseControl(UINT message, int _x, int _y)
//{
//
//	if (gpC_mouse_pointer->GetPickUpItem() == NULL) // Item을 들고 있으면 입력할 수 없다.
//	{
//		m_pC_chatting->MouseControl(message, _x, _y);
//		m_pC_button_group->MouseControl(message, _x, _y);
//	}
/*	
	switch (message)
	{
	case M_MOVING:
		{
			// 미니맵 위에 마우스가 있다.
			if(_x >= m_map_start_point.x && _y >= m_map_start_point.y && _x < m_map_start_point.x+200 && _y < m_map_start_point.y+100)
			{
				int map_w = 200, map_h = 100;
				if(m_map_w != m_map_h)
				{
					if(m_map_w > m_map_h)map_h = map_h * m_map_h / m_map_w;
					if(m_map_h > m_map_w)map_w = map_w * m_map_w / m_map_h;
				}
				
				for(int i = 0; i < m_portal.size(); i++)
				{
					int x = m_map_start_point.x + (m_portal[i].left+m_portal[i].right)/2*map_w/m_map_w + (200 - map_w)/2;
					int y = m_map_start_point.y + (m_portal[i].top+m_portal[i].bottom)/2*map_h/m_map_h + (100 - map_h)/2;
					if(_x > x-5 && _x < x+5 && _y > y-5 && _y < y+5)
					{
						int print_x = gpC_mouse_pointer->GetPointerX();
						int print_y = gpC_mouse_pointer->GetPointerY();
						
						g_descriptor_manager.Set(DID_INFO, print_x, print_y, (void *)GetZoneName(m_portal_zone_id[i]));
						break;
					}
				}
				
				for(i = 0; i < m_npc.size(); i++)
				{
					int x = m_map_start_point.x + m_npc[i].x*map_w/m_map_w + (200 - map_w)/2;
					int y = m_map_start_point.y + m_npc[i].y*map_h/m_map_h + (100 - map_h)/2;
					
					if(_x > x-5 && _x < x+5 && _y > y-5 && _y < y+5)
					{
						int print_x = gpC_mouse_pointer->GetPointerX();
						int print_y = gpC_mouse_pointer->GetPointerY();
						
						static char npc_name[50];
						if((*g_pCreatureTable)[m_npc[i].id].Name.GetString())
							strcpy(npc_name, (*g_pCreatureTable)[m_npc[i].id].Name.GetString());
						else strcpy(npc_name, "");
						
						g_descriptor_manager.Set(DID_INFO, print_x, print_y, (void *)npc_name);
						break;
					}
				}
//				C_VS_UI_EVENT_BUTTON * p_button;
//				p_button = m_pC_button_group->IsInRect(_x, _y);
//				if (p_button != NULL)
//				{
//					// help description
//					g_descriptor_manager.Set(DID_HELP, p_button->x, p_button->y, (void *)&g_help_string[p_button->GetID()-0xffff], 2);
//				}
			}
		}
		break;

	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
	case M_LEFTBUTTON_UP:
	case M_RIGHTBUTTON_DOWN:
	case M_RIGHTBUTTON_UP:
//		if(_y < 300)return false;
		break;
	}

*///	return true;
//}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::KeyboardControl(UINT message, UINT key, long extra)
{
	m_pC_chatting->KeyboardControl(message, key, extra);
}


//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE::HotKey_F5()
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::HotKey_F5()
{
	C_VS_UI_SKILL * p_skill = GetSkillWindow();

	int slot = p_skill->GetFocusedSlot();
	int id = p_skill->GetSkillID(slot);
	if (id != NOT_SELECTED)
	{
		p_skill->SetHotkey(C_VS_UI_SKILL::HOTKEY5, (ACTIONINFO)id);
	}
	else
	{
		// use
		p_skill->ChangeSkillByHotkey(C_VS_UI_SKILL::HOTKEY5);
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE::HotKey_F6()
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::HotKey_F6()
{
	C_VS_UI_SKILL * p_skill = GetSkillWindow();

	int slot = p_skill->GetFocusedSlot();
	int id = p_skill->GetSkillID(slot);
	if (id != NOT_SELECTED)
	{
		p_skill->SetHotkey(C_VS_UI_SKILL::HOTKEY6, (ACTIONINFO)id);
	}
	else
	{
		// use
		p_skill->ChangeSkillByHotkey(C_VS_UI_SKILL::HOTKEY6);
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE::HotKey_F7()
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::HotKey_F7()
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

//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE::HotKey_F8()
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::HotKey_F8()
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
// C_VS_UI_VAMPIRE::HotKey_F1()
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_VAMPIRE::HotKey_F1()
{
	//gpC_base->SendMessage(UI_USE_SERUM);

	const MItem * p_item = g_pInventory->FindItem( ITEM_CLASS_SERUM );
	if(p_item == NULL)
		p_item = g_pInventory->FindItem( ITEM_CLASS_EVENT_ETC, 14 ); // 노란사탕 - 3종족 모두 사용가능
	//add by sonic 2006.9.20
	/******** 藤속連넣 但庚쵯벎 괌뵐쵯벎 쟎輦쵯벎 꾸鱗********/
	if(p_item == NULL)
		p_item = g_pInventory->FindItem( ITEM_CLASS_EVENT_ETC, 15 ); // 노란사탕 - 3종족 모두 사용가능
	
	if(p_item == NULL)
		p_item = g_pInventory->FindItem( ITEM_CLASS_EVENT_ETC, 16 ); // 노란사탕 - 3종족 모두 사용가능
	
	if(p_item == NULL)
		p_item = g_pInventory->FindItem( ITEM_CLASS_EVENT_ETC, 17 ); // 노란사탕 - 3종족 모두 사용가능
	//end by sonic 2006.9.20

	if (p_item) // Item이 있다.
	{
		gpC_base->SendMessage(UI_ITEM_USE,
								  p_item->GetID(), 0,
								  (MItem *)p_item);
	}

		
}



//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE_GEAR::C_VS_UI_VAMPIRE_GEAR
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_VAMPIRE_GEAR::C_VS_UI_VAMPIRE_GEAR()
{
	assert(MVampireGear::MAX_GEAR_VAMPIRE == SLOT_SIZE);
	
//	if(g_char_slot_ingame.m_AdvancementLevel== 0)// 2차전직 bycsm 2004.12.31
//	{
		m_slot_rect[SN_NECKLACE].Set(108, 14, 30, 30);
		m_slot_rect[SN_COAT].Set(93, 105, 60, 90);
		m_slot_rect[SN_BRACELET1].Set(56, 167, 30, 30);
		m_slot_rect[SN_BRACELET2].Set(158, 167, 30, 30);
		m_slot_rect[SN_RING1].Set(56, 198, 30, 30);
		m_slot_rect[SN_RING2].Set(90, 198, 30, 30);
		m_slot_rect[SN_RING3].Set(124, 198, 30, 30);
		m_slot_rect[SN_RING4].Set(158, 198, 30, 30);
		m_slot_rect[SN_EARRING1].Set(57, 14, 30, 30);
		m_slot_rect[SN_EARRING2].Set(159, 14, 30, 30);
		m_slot_rect[SN_LEFTHAND].Set (26, 105, 60, 60);
		m_slot_rect[SN_RIGHTHAND].Set(159, 105, 60, 60);
		m_slot_rect[SN_AMULET1].Set(56, 230, 30, 30); 
		m_slot_rect[SN_AMULET2].Set(90, 230, 30, 30);
		m_slot_rect[SN_AMULET3].Set(125, 230, 30, 30);
		m_slot_rect[SN_AMULET4].Set(158, 230, 30, 30);
		m_slot_rect[SN_COREZAP1].Set(55, 198, 30, 30);
		m_slot_rect[SN_COREZAP2].Set(89, 198, 30, 30);
		m_slot_rect[SN_COREZAP3].Set(123, 198, 30, 30);
		m_slot_rect[SN_COREZAP4].Set(158, 198, 30, 30);

		m_slot_rect[SN_DERMIS].Set(159, 45, 60, 60);
		m_slot_rect[SN_PERSONA].Set(26, 45, 60, 60);

		m_slot_rect[SN_BLOODBIBLE1].Set(23, 263, 30, 30);
		m_slot_rect[SN_BLOODBIBLE2].Set(57, 263, 30, 30);
		m_slot_rect[SN_BLOODBIBLE3].Set(91, 263, 30, 30);
		m_slot_rect[SN_BLOODBIBLE4].Set(125, 263, 30, 30);
		m_slot_rect[SN_BLOODBIBLE5].Set(159, 263, 30, 30);
		m_slot_rect[SN_BLOODBIBLE6].Set(193, 263, 30, 30);
		m_slot_size = SLOT_SIZE;

		
//	}
//	else
//	{
//		m_slot_rect[SN_NECKLACE].Set(110, 14, 30, 30);
//		m_slot_rect[SN_COAT].Set(95, 49, 60, 90);
//		m_slot_rect[SN_BRACELET1].Set(56, 109, 30, 30);
//		m_slot_rect[SN_BRACELET2].Set(164, 108, 30, 31);
//		m_slot_rect[SN_RING1].Set(56, 144, 30, 30);
//		m_slot_rect[SN_RING2].Set(92, 144, 30, 30);
//		m_slot_rect[SN_RING3].Set(128, 144, 30, 30);
//		m_slot_rect[SN_RING4].Set(164, 144, 30, 30);
//		m_slot_rect[SN_EARRING1].Set(59, 14, 30, 30);
//		m_slot_rect[SN_EARRING2].Set(161, 14, 30, 30);
//		m_slot_rect[SN_LEFTHAND].Set(28, 47, 60, 60);
//		m_slot_rect[SN_RIGHTHAND].Set(161, 46, 60, 60);
//		m_slot_rect[SN_AMULET1].Set(56, 178, 30, 30); 
//		m_slot_rect[SN_AMULET2].Set(92, 178, 30, 30);
//		m_slot_rect[SN_AMULET3].Set(128, 178, 30, 30);
//		m_slot_rect[SN_AMULET4].Set(164, 178, 30, 30);
//		
//		m_slot_rect[SN_COREZAP1].Set(56, 144, 30, 30);
//		m_slot_rect[SN_COREZAP2].Set(93, 144, 30, 30);
//		m_slot_rect[SN_COREZAP3].Set(128, 144, 30, 30);
//		m_slot_rect[SN_COREZAP4].Set(164, 144, 30, 30);
//
//		m_slot_rect[SN_PERSONA].Set(159, 45, 60, 60);
//		m_slot_rect[SN_DERMIS].Set(26, 45, 60, 60);
//		
//
//		
//		m_slot_rect[SN_BLOODBIBLE1].Set(20, 212, 30, 30);
//		m_slot_rect[SN_BLOODBIBLE2].Set(56, 212, 30, 30);
//		m_slot_rect[SN_BLOODBIBLE3].Set(92, 212, 30, 30);
//		m_slot_rect[SN_BLOODBIBLE4].Set(128, 212, 30, 30);
//		m_slot_rect[SN_BLOODBIBLE5].Set(164, 212, 30, 30);
//		m_slot_rect[SN_BLOODBIBLE6].Set(200, 212, 30, 30);
//		m_slot_size = SLOT_SIZE;
//	}
//	
	
	m_p_slot_rect = m_slot_rect;
	m_p_slot_image = m_slot_image;
}

//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE_GEAR::~C_VS_UI_VAMPIRE_GEAR
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_VAMPIRE_GEAR::~C_VS_UI_VAMPIRE_GEAR()
{
}