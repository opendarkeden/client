// VS_UI_GameCommon.cpp

#include "VS_UI_GameCommon.h"
#include "VS_UI_GlobalResource.h"
#include "VS_UI_filepath.h"
#include "VS_UI_Mouse_pointer.h"
//#include "VS_UI_Item.h"
#include "VS_UI.h"

#include "MPriceManager.h"
#include "MCreatureTable.h"
#include "CDirectInput.h"
#include "MNPCTable.h"
#include "MParty.h"
#include "VS_UI_Title.h"
#include "MGuildMarkManager.h"
#include "MGuildInfoMapper.h"
#include "ExperienceTable.h"
#include "UserOption.h"
#include "UserInformation.h"
#include "MItemOptionTable.h"
#include "MGameStringTable.H"
//#include "ex\DebugInfo.h"
#include "RankBonusDef.H"
#include "RankBonusTable.H"
#include "KeyAccelerator.h"

#include "MZoneTable.h"
#include "FameInfo.h"

#include <algorithm>
#include <time.h>

#define WHISPER_MAX 10	// by larosel

#define MAX_SLAYER_ATTR_OLD				200
#define MAX_SLAYER_ATTR_SUM_OLD			300
#define MAX_SLAYER_DOMAIN_SUM_OLD		100
#define MAX_VAMPIRE_LEVEL_OLD			100

#define	MAX_SLAYER_ATTR					290
#define	MAX_SLAYER_ATTR_SUM				435
#define MAX_VAMPIRE_LEVEL				150


bool					gbl_mine_progress = false;
DWORD					gi_mine_progress;

extern CDirectInput*			g_pDXInput;

extern BOOL g_bActiveGame;

CSpritePack	C_VS_UI_SKILL::m_C_spk; // Skill icon Sprite list
CSpritePack	C_VS_UI_SKILL::m_C_spk_mini; // Skill icon Sprite list

int C_VS_UI_INFO::m_selected_grade_skill = -1;

//C_SPRITE_PACK	* C_VS_UI_INFO::m_pC_info_spk=NULL;


void	g_StartSellConfirmDialog(int _x, int _y);
void	g_StartSellAllConfirmDialog(int _x, int _y, int price);
void	g_StartSellFinishDialog(int _x, int _y);

void	g_StartRepairConfirmDialog(int _x, int _y, bool bChargeItem);
void	g_StartRepairAllConfirmDialog(int _x, int _y, int price);
void	g_StartRepairFinishDialog(int _x, int _y);

void	g_StartSilveringConfirmDialog(int _x, int _y);
void	g_StartSilveringFinishDialog(int _x, int _y);

static C_VS_UI_DIALOG *	gpC_dialog_sellall_confirm = NULL;
static C_VS_UI_DIALOG *	gpC_dialog_sell_confirm = NULL;
static C_VS_UI_DIALOG *	gpC_dialog_sell_finish = NULL;

static C_VS_UI_DIALOG *	gpC_dialog_repairall_confirm = NULL;
static C_VS_UI_DIALOG *	gpC_dialog_repair_confirm = NULL;
static C_VS_UI_DIALOG *	gpC_dialog_repair_finish = NULL;

static C_VS_UI_DIALOG *	gpC_dialog_silvering_confirm = NULL;
static C_VS_UI_DIALOG *	gpC_dialog_silvering_finish = NULL;

extern	BOOL g_MyFull;
//const int PCS_CHAT_LINE_START_X =	40;
//const int CHAT_LINE_START_X =			20;
//const int CHAT_LINE_START_Y =			RESOLUTION_Y-20;
//const int CHAT_HISTORY_START_Y =		RESOLUTION_Y-40;

const char g_sz_chat_id_divisor[CLD_TOTAL][3] = 
{
	">",
	">",
	"->",
	")",
	"}",
	":",
};

int g_HISTORY_LINE = 4;

#define FONT_GAP							17

char g_mark[MARK_MAX][9][3] =
{
	{"＃","＆","＊","＠","§","※","☆","★","○"},
	{"●","◎","◇","◆","□","■","△","▲","▽"},
	{"▼","→","←","↑","↓","↔","〓","◁","◀"},
	{"▷","▶","♤","♠","♡","♥","♧","♣","⊙"},
	{"◈","▣","◐","◑","▒","▤","▥","▨","▧"},
	{"▦","▩","♨","☏","☎","☜","☞","¶","†"},
};

COLORREF g_color[4][4] = 
{
	RGB(0, 0, 0),		RGB(132, 0, 0),	RGB(0, 130, 0),	RGB(132, 130, 0),
	RGB(0, 0, 132),		RGB(132, 0, 132),	RGB(0, 130, 132),	RGB(132, 130, 132),
	RGB(198, 195, 198),	RGB(255, 0, 0),	RGB(0, 255, 0),	RGB(255, 255, 0),
	RGB(0, 0, 255),		RGB(255, 0, 255),	RGB(0, 255, 255),	RGB(255, 255, 255),
};

//
// g_char_slot_ingame
//
// 게임중에 계속 update시키는 char-slot이다.
// 이것이 실시간으로 update되어야 info Window에 옳바로 표시된다.
//
S_SLOT	g_char_slot_ingame; // any character info slot in game.

static MItem *	m_p_sell_item;
static MItem *	m_p_repair_item;
static MItem *	m_p_silvering_item;
static C_VS_UI_MONEY_DIALOG *	m_pC_dialog_drop_money;
static C_VS_UI_MONEY_DIALOG *	m_pC_dialog_bring_fee;


// Item blink color table
int ga_item_blink_color_table[INTERFACE_BLINK_VALUE_MAX] = {
	LIGHT_BLUE, SKY_BLUE, WHITE, BLUE
};

//
// Inventory/Gear/QuickItem ... 에서 item을 특정위치에 놓을 때 참조하는 값.
// [global effect. slayer/vampire Inventory, Gear 등에서 참조한다. 즉, 
//  Game class의 object가 참조한다.
//
//           +-------+
//           | 1   2 |
//           |       |
//           | 3   4 |
//           +-------+
//					(a item)
//
// item에 대한 '참조점' 4개를 설정한다. 이것은 item 크기에따라 적절하게 위치
// 하여야 한다. 이것은 원점으로부터의 거리이다.
// Inventory나 Gear에서는 이 점을 순서대로 검색하여 item이 놓일 위치를 결정한다.
//
Point g_item_ref_point[ITEM_REF_POINT_COUNT];

//-----------------------------------------------------------------------------
// g_SetItemRefPoint
//
// 참조점을 설정한다.
//-----------------------------------------------------------------------------
#define REFPOINT_DISTANCE_DEFAULT	5
void g_SetItemRefPoint(int item_rect_w, int item_rect_h)
{
	g_item_ref_point[0].x = REFPOINT_DISTANCE_DEFAULT;
	g_item_ref_point[0].y = REFPOINT_DISTANCE_DEFAULT;
	g_item_ref_point[1].x = item_rect_w - REFPOINT_DISTANCE_DEFAULT;
	g_item_ref_point[1].y = REFPOINT_DISTANCE_DEFAULT;
	g_item_ref_point[2].x = REFPOINT_DISTANCE_DEFAULT;
	g_item_ref_point[2].y = item_rect_h - REFPOINT_DISTANCE_DEFAULT;
	g_item_ref_point[3].x = item_rect_w - REFPOINT_DISTANCE_DEFAULT;
	g_item_ref_point[3].y = item_rect_h - REFPOINT_DISTANCE_DEFAULT;
}

//-----------------------------------------------------------------------------
// Exec functions
//-----------------------------------------------------------------------------

void ExecF_LearnGradeSkillConfirm(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch(id)
	{
	case DIALOG_EXECID_OK :
		if(C_VS_UI_INFO::m_selected_grade_skill != -1)
			gpC_base->SendMessage(UI_SELECT_GRADE_SKILL,C_VS_UI_INFO::m_selected_grade_skill,0,NULL);

		C_VS_UI_INFO::m_selected_grade_skill = -1;
		break;
	}
}
void ExecF_SellFinish(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
	case DIALOG_EXECID_OK:
		gpC_base->SendMessage(UI_ITEM_SELL_FINISHED);
		break;
	}
}

void ExecF_SellConfirm(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
	case DIALOG_EXECID_OK:
		assert(m_p_sell_item != NULL);
		
		gpC_base->SendMessage(UI_SELL_ITEM, m_p_sell_item->GetGridX(), m_p_sell_item->GetGridY(), m_p_sell_item);
		break;
	}
}

void ExecF_SellAllConfirm(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
	case DIALOG_EXECID_OK:
		gpC_base->SendMessage(UI_SELL_ALL_ITEM);
		break;
	}
}

void ExecF_RepairFinish(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
	case DIALOG_EXECID_OK:
		gpC_base->SendMessage(UI_ITEM_REPAIR_FINISHED);
		break;
	}
}

void ExecF_RepairConfirm(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
	case DIALOG_EXECID_OK:
		assert(m_p_repair_item != NULL);
		
		gpC_base->SendMessage(UI_REPAIR_ITEM, m_p_repair_item->GetID(), 0, m_p_repair_item);
		break;
	}
}

void ExecF_RepairAllConfirm(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
	case DIALOG_EXECID_OK:
		gpC_base->SendMessage(UI_REPAIR_ITEM, 0, 0, NULL);
		break;
	}
}

void ExecF_SilveringFinish(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
	case DIALOG_EXECID_OK:
		gpC_base->SendMessage(UI_ITEM_SILVERING_FINISHED);
		break;
	}
}

void ExecF_SilveringConfirm(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
	case DIALOG_EXECID_OK:
		assert(m_p_silvering_item != NULL);
		
		gpC_base->SendMessage(UI_SILVERING_ITEM, m_p_silvering_item->GetID(), 0, m_p_silvering_item);
		break;
	}
}

void ExecF_DropMoney(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
	case DIALOG_EXECID_OK:
		if (gC_vs_ui.IsRunningStorage())
		{
			gpC_base->SendMessage(UI_DEPOSIT_MONEY, m_pC_dialog_drop_money->GetValue());
		}
		//else if (gC_vs_ui.IsRunningExchange())
		//{
		//gpC_base->SendMessage(UI_EXCHANGE_MONEY, m_pC_dialog_drop_money->GetValue());
		//}
		else
		{
			// by sigi
			gpC_base->SendMessage(UI_DROP_MONEY, m_pC_dialog_drop_money->GetValue());
		}
		break;
	}
	gC_vs_ui.AcquireChatting();
}

void Execf_BringFee(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch(id)
	{
	case DIALOG_EXECID_OK :
		gC_vs_ui.SetBringFee((UINT)m_pC_dialog_bring_fee->GetValue());
		break;
	}
	gC_vs_ui.AcquireChatting();
}

//-----------------------------------------------------------------------------
// g_StartSellConfirmDialog
//
// 
//-----------------------------------------------------------------------------
void g_StartSellConfirmDialog(int _x, int _y)
{
	DeleteNew(gpC_dialog_sell_confirm);
	
	gpC_dialog_sell_confirm = new C_VS_UI_DIALOG(_x, _y, 2, 0, ExecF_SellConfirm, DIALOG_OK|DIALOG_CANCEL);
	char * pp_dmsg_sell_confirm[1] = {(*g_pGameStringTable)[UI_STRING_MESSAGE_SELL_CONFIRM].GetString()};
	gpC_dialog_sell_confirm->SetMessage(pp_dmsg_sell_confirm, 1, SMO_NOFIT);
	
	gpC_dialog_sell_confirm->Start();
}

//-----------------------------------------------------------------------------
// g_StartSellAllConfirmDialog
//
// 
//-----------------------------------------------------------------------------
void g_StartSellAllConfirmDialog(int _x, int _y, int price)
{
	// 비상용으로 땜빵.
	DeleteNew(gpC_dialog_sellall_confirm);
	
	gpC_dialog_sellall_confirm = new C_VS_UI_DIALOG(_x, _y, 2, 0, ExecF_SellAllConfirm, DIALOG_OK|DIALOG_CANCEL);
	
	char sz_buf[512];
	static char temp[100];
	wsprintf(sz_buf, "%d", price);
	std::string sstr = sz_buf;
	for(int i = 3; i <= 13; i += 4)
		if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
	sprintf(sz_buf, "%s", sstr.c_str());	
	
	wsprintf(temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_ALL_PRICE].GetString(), sz_buf);
	
	static char *pp_dmsg_sellall_confirm[2] = {
		temp, 
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ALL_SELL_CONFIRM].GetString(),
	};
	gpC_dialog_sellall_confirm->SetMessage(pp_dmsg_sellall_confirm, 2, SMO_NOFIT);		
	gpC_dialog_sellall_confirm->Start();
}

//-----------------------------------------------------------------------------
// g_StartRepairAllConfirmDialog
//
// 
//-----------------------------------------------------------------------------
void g_StartRepairAllConfirmDialog(int _x, int _y, int price)
{
	//비상용 땜빵 
	DeleteNew(gpC_dialog_repairall_confirm);
	
	gpC_dialog_repairall_confirm = new C_VS_UI_DIALOG(_x, _y, 2, 0, ExecF_RepairAllConfirm, DIALOG_OK|DIALOG_CANCEL);
	
	static char temp[100];	
	char sz_buf[512];
	wsprintf(sz_buf, "%d", price);
	std::string sstr = sz_buf;
	for(int i = 3; i <= 13; i += 4)
		if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
	sprintf(sz_buf, "%s", sstr.c_str());	

	wsprintf(temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_ALL_PRICE].GetString(), sz_buf);
	
	static char *pp_dmsg_repairall_confirm[2] = {		
		temp,
		(*g_pGameStringTable)[UI_STRING_MESSAGE_REPAIR_CONFIRM].GetString(),
	};

	gpC_dialog_repairall_confirm->SetMessage(pp_dmsg_repairall_confirm, 2, SMO_NOFIT);
	gpC_dialog_repairall_confirm->Start();
}

//-----------------------------------------------------------------------------
// g_StartSellFinishDialog
//
// 
//-----------------------------------------------------------------------------
void g_StartSellFinishDialog(int _x, int _y)
{
	DeleteNew(gpC_dialog_sell_finish);
	
	gpC_dialog_sell_finish = new C_VS_UI_DIALOG(_x, _y, 2, 1, ExecF_SellFinish, DIALOG_OK);
	static char * pp_dmsg_sell_finish[4] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLICK_TRADE_ITEM].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLICK_OK_BUTTON_TO_END].GetString(),
		"",
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TIP_SELL_ALL_VAMPIRE_HEAD].GetString(),
	};
	gpC_dialog_sell_finish->SetMessage(pp_dmsg_sell_finish, 4, SMO_NOFIT);
	
	gpC_dialog_sell_finish->StartByPinMode();
}


//-----------------------------------------------------------------------------
// g_StartRepairConfirmDialog
//
// Gear/Inventory에 해당하므로 global로 함.
//-----------------------------------------------------------------------------
void g_StartRepairConfirmDialog(int _x, int _y, bool bChargeItem)
{
	DeleteNew(gpC_dialog_repair_confirm);
	
	gpC_dialog_repair_confirm = new C_VS_UI_DIALOG(_x, _y, 2, 0, ExecF_RepairConfirm, DIALOG_OK|DIALOG_CANCEL);
	
	char * pp_dmsg_repair_confirm[1] = {(*g_pGameStringTable)[UI_STRING_MESSAGE_THIS_ITEM_REPAIR_CONFIRM].GetString()};
	char * pp_dmsg_charge_confirm[1] = {(*g_pGameStringTable)[UI_STRING_MESSAGE_THIS_ITEM_CHARGE_CONFIRM].GetString()};
	
	if(bChargeItem)
		gpC_dialog_repair_confirm->SetMessage(pp_dmsg_charge_confirm, 1, SMO_NOFIT);
	else
		gpC_dialog_repair_confirm->SetMessage(pp_dmsg_repair_confirm, 1, SMO_NOFIT);
	
	gpC_dialog_repair_confirm->Start();
}

//-----------------------------------------------------------------------------
// g_StartRepairFinishDialog
//
// 
//-----------------------------------------------------------------------------
void g_StartRepairFinishDialog(int _x, int _y)
{
	DeleteNew(gpC_dialog_repair_finish);
	
	gpC_dialog_repair_finish = new C_VS_UI_DIALOG(_x, _y, 4, 180, ExecF_RepairFinish, DIALOG_OK);
	static char * pp_dmsg_repair_finish[] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLICK_REPAIR_ITEM].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLICK_OK_BUTTON_TO_END].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TIP_REPAIR_ALL_ITEM].GetString(),
	};
	gpC_dialog_repair_finish->SetMessage(pp_dmsg_repair_finish, 3, SMO_NOFIT);
	
	gpC_dialog_repair_finish->StartByPinMode();
}

// g_StartSilveringConfirmDialog
//
// Gear/Inventory에 해당하므로 global로 함.
//-----------------------------------------------------------------------------
void g_StartSilveringConfirmDialog(int _x, int _y)
{
	DeleteNew(gpC_dialog_silvering_confirm);
	
	gpC_dialog_silvering_confirm = new C_VS_UI_DIALOG(_x, _y, 2, 0, ExecF_SilveringConfirm, DIALOG_OK|DIALOG_CANCEL);
	static char * pp_dmsg_silvering_confirm[] = {(*g_pGameStringTable)[UI_STRING_MESSAGE_THIS_ITEM_SILVERING_CONFIRM].GetString()};
	gpC_dialog_silvering_confirm->SetMessage(pp_dmsg_silvering_confirm, 1, SMO_NOFIT);
	
	gpC_dialog_silvering_confirm->Start();
}

//-----------------------------------------------------------------------------
// g_StartSilveringFinishDialog
//
// 
//-----------------------------------------------------------------------------
void g_StartSilveringFinishDialog(int _x, int _y)
{
	DeleteNew(gpC_dialog_silvering_finish);
	
	gpC_dialog_silvering_finish = new C_VS_UI_DIALOG(_x, _y, 3, 0, ExecF_SilveringFinish, DIALOG_OK);
	
	static char * pp_dmsg_silvering_finish[2] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLICK_SILVERING_ITEM].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLICK_OK_BUTTON_TO_END].GetString()
	};
	
	gpC_dialog_silvering_finish->SetMessage(pp_dmsg_silvering_finish, 2, SMO_NOFIT);
	
	gpC_dialog_silvering_finish->StartByPinMode();
}


//----------------------------------------------------------------------------
// Operations
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::C_VS_UI_TRIBE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_TRIBE::C_VS_UI_TRIBE()
{
	g_RegisterWindow(this);
	
	AttrKeyboardControl(true);
	
	AttrPin(true);
	m_pC_inventory = new C_VS_UI_INVENTORY;
	
	m_pC_hpbar = new C_VS_UI_HPBAR;
	m_pC_effect_status = new C_VS_UI_EFFECT_STATUS;
	m_pC_minimap = new C_VS_UI_MINIMAP;
	m_pC_chatting = NULL;
	m_pC_skill = new C_VS_UI_SKILL;
	m_pC_common_button_group = new ButtonGroup(this);
	m_pC_menu_button_group = new ButtonGroup(this);
	m_pC_guild_button_group = new ButtonGroup(this);
	m_pC_msg_button_group = new ButtonGroup(this);
	m_pC_help_button_group = new ButtonGroup(this);
	m_pC_level_up = NULL;
	
	// TIMER
	m_dw_prev_tickcount = GetTickCount();
	m_dw_millisec = 100;
	
	m_selected_tab = TAB_MENU_ID;
	
	//	m_bl_help = false;
	//	m_bl_party = false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::~C_VS_UI_TRIBE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_TRIBE::~C_VS_UI_TRIBE()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::MAINMENU, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::MAINMENU, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetMainTab(GetSelectedTab());

	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_hpbar);
	DeleteNew(m_pC_effect_status);
	DeleteNew(m_pC_minimap);
	// add by Coffee 2007-3-6 警속各썹뒈暠
	DeleteNew(m_pC_worldmap);
	// end 
	DeleteNew(m_pC_inventory);
	DeleteNew(m_pC_skill);
	
	DeleteNew(m_pC_main_spk);
	DeleteNew(m_pC_sys_button_spk);
	
	DeleteNew(m_pC_common_button_group);
	DeleteNew(m_pC_menu_button_group);
	DeleteNew(m_pC_guild_button_group);
	DeleteNew(m_pC_msg_button_group);
	DeleteNew(m_pC_help_button_group);
	DeleteNew(m_pC_level_up);
	
	FinishItemSelling();
	FinishItemRepairing(); // to release dialog mem
	FinishItemSilvering(); // to release dialog mem
}

void	C_VS_UI_TRIBE::Process()
{
	if(!m_pC_skill->IsPixel(gpC_mouse_pointer->GetX(), gpC_mouse_pointer->GetY()))
	{
		if(gbl_vampire_interface)
			ProcessHide(47);
		else
			ProcessHide(41);
	}
	m_pC_common_button_group->Process();
	m_pC_menu_button_group->Process();
	m_pC_guild_button_group->Process();
	m_pC_msg_button_group->Process();
	m_pC_help_button_group->Process();
}

void	C_VS_UI_TRIBE::WindowEventReceiver(id_t event)
{
	switch (event)
	{
	case EVENT_WINDOW_MOVE:
		if(gbl_vampire_interface)
			m_pC_skill->SetStartPoint(x+35, y+8);
		else
			m_pC_skill->SetStartPoint(x+36, y+3);
		EMPTY_MOVE;
		break;
	}
}
void	C_VS_UI_TRIBE::SetSelectTab(int tab)
{
	if(gbl_vampire_interface)
	{
		if(tab == TAB_MENU_ID || tab == TAB_GUILD_ID || tab == TAB_HELP_ID )			
			m_selected_tab = tab; 
	} else
	{
		if(tab == TAB_MENU_ID || tab == TAB_EXP_ID || tab == TAB_GUILD_ID || tab == TAB_HELP_ID)
			m_selected_tab = tab;
	}
}



//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRIBE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char* slayer_string[EXEC_MAX+1] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GAME_MENU].GetString(),
			
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MENU].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EXP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MESSAGE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_HELP].GetString(),
			
		// Menu TAB
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INVENTORY].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GEAR_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INFO_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_PARTY_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_QUEST_WINDOW].GetString(),
			
		// Guild Tab
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_MEMBER_LIST].GetString(),
			
		// HELP TAB
		(*g_pGameStringTable)[UI_STRING_MESSAGE_BASIC_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CHATTING_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_WAR_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SKILL_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_HELP].GetString(),
			
		// MAX+
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
	};
	
	const static char* vampire_string[EXEC_MAX+1] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GAME_MENU].GetString(),
			
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MENU].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EXP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLAN].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MESSAGE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_HELP].GetString(),
		
		// Menu TAB
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INVENTORY].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GEAR_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INFO_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_PARTY_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_QUEST_WINDOW].GetString(),
			
		// Guild Tab
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLAN_INFO].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLAN_MEMBER_LIST].GetString(),
			
		// HELP TAB
		(*g_pGameStringTable)[UI_STRING_MESSAGE_BASIC_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CHATTING_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_WAR_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MAGIC_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLAN_HELP].GetString(),
			
		// MAX+
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
	};
	
	if(gbl_vampire_interface)
	{
		if(p_button->GetID() == PUSHPIN_ID && GetAttributes()->autohide)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)vampire_string[EXEC_MAX],0,0);		else
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)vampire_string[p_button->GetID()],0,0);
	}
	else
	{
		if(p_button->GetID() == PUSHPIN_ID && GetAttributes()->autohide)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)slayer_string[EXEC_MAX],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)slayer_string[p_button->GetID()],0,0);
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRIBE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	//	ShowButtonDescription(p_button);
	if(p_button->GetID() == PUSHPIN_ID)
	{
		if(GetAttributes()->autohide)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
	}
	else if(p_button->GetID() == SYSTEM_ID)
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_pC_main_spk->BltLocked(x+p_button->x, y+p_button->y+1, p_button->m_image_index+1);
			else
				m_pC_main_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
		}
		else
			m_pC_main_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
	}
	else if(p_button->GetID() >= TAB_MENU_ID && p_button->GetID() <= TAB_HELP_ID)
	{
		if(p_button->GetID() == m_selected_tab)
			m_pC_sys_button_spk->BltLocked(x+p_button->x, y+p_button->y-2, p_button->m_image_index+2);
		else
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
				{
					Rect rect(0, 0, m_pC_sys_button_spk->GetWidth(p_button->m_image_index+2), m_pC_sys_button_spk->GetHeight(p_button->m_image_index+2)-3);
					m_pC_sys_button_spk->BltLockedClip(x+p_button->x, y+p_button->y+1, rect, p_button->m_image_index+2);
				}
				else
				{
					Rect rect(0, 0, m_pC_sys_button_spk->GetWidth(p_button->m_image_index+2), m_pC_sys_button_spk->GetHeight(p_button->m_image_index+2)-2);
					m_pC_sys_button_spk->BltLockedClip(x+p_button->x, y+p_button->y, rect, p_button->m_image_index+2);
				}
			}
			else
				m_pC_sys_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}
	}
	else
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
			{
				m_pC_sys_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
			}
			else
			{
				m_pC_sys_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			}
		}
		else
			m_pC_sys_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
	}
	
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::Run
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRIBE::Run(id_t id)
{
	switch(id)
	{
	case PUSHPIN_ID:
		if(GetAttributes()->autohide)
			AttrAutoHide(ATTRIBUTES_HIDE_NOT);
		else
			AttrAutoHide(ATTRIBUTES_HIDE_HEIGHT);
		EMPTY_MOVE;
		break;
		
	case SYSTEM_ID:
		gC_vs_ui.ToggleESC4UI();
		EMPTY_MOVE;
		break;
		
	case TAB_MENU_ID:
	case TAB_EXP_ID:
	case TAB_GUILD_ID:
	case TAB_MSG_ID:
	case TAB_HELP_ID:
		m_selected_tab = id;
		break;
		
		// MENU TAB
	case INVENTORY_ID:
		HotKey_Inventory();
		break;
		
	case GEAR_ID:
		HotKey_Gear();
		break;
		
	case INFO_ID:
		HotKey_CharInfo();
		break;
		
	case PARTY_ID:
		HotKey_Party();
		break;
		
	case QUEST_ID:
		break;
		
		// TEAM TAB
	case TEAM_INFO_ID:
		if(g_pUserInformation->GuildGrade != 4)
			gpC_base->SendMessage(UI_REQUEST_GUILD_INFO, 0, 0, NULL);
		break;
		
	case TEAM_MEMBER_LIST_ID:
		if(g_pUserInformation->GuildGrade != 4)
			gC_vs_ui.RunTeamMemberList();
		break;
		
		// HELP TAB
	case HELP_ID:
		HotKey_Help();
		break;
		
	case CHAT_HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::CHATTING);
		break;
		
	case BATTLE_HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::BATTLE);
		break;
		
	case SKILL_HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::SKILL);
		break;
		
	case GUILD_HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::GUILD);
		break;
	}
	
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_TRIBE::IsPixel(int _x, int _y)
{
	bool re;
	re = m_pC_main_spk->IsPixel(_x-x, _y-y);
	//	re |= m_pC_skill->IsPixel(_x, _y);
	return re;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::OpenInventoryToSell
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRIBE::OpenInventoryToSell()
{
	if (gbl_vampire_interface)
		InventoryXY(380, 150);
	else
		InventoryXY(380, 150);
	
	g_StartSellFinishDialog(30, 150);
	
	OpenInventory(false);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::OpenInventoryToRepair
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRIBE::OpenInventoryToRepair()
{
	if (gbl_vampire_interface)
	{
		InventoryXY(30, 30);
		GearXY(500, 60);
	}
	else
	{
		InventoryXY(30, 30);
		GearXY(500, 30);
	}
	
	g_StartRepairFinishDialog(0, 300);
	
	OpenInventory(false);
	OpenGear(false);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::OpenInventoryToSilvering
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRIBE::OpenInventoryToSilvering()
{
	if (gbl_vampire_interface)
	{
		InventoryXY(30, 30);
		GearXY(500, 60);
	}
	else
	{
		InventoryXY(30, 30);
		GearXY(500, 30);
	}
	
	g_StartSilveringFinishDialog(0, 300);
	
	OpenInventory(false);
	OpenGear(false);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::FinishItemSelling
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TRIBE::FinishItemSelling()
{
	DeleteNew(gpC_dialog_sell_finish);
	DeleteNew(gpC_dialog_sell_confirm);
	DeleteNew(gpC_dialog_sellall_confirm);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::FinishItemRepairing
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TRIBE::FinishItemRepairing()
{
	DeleteNew(gpC_dialog_repair_finish);
	DeleteNew(gpC_dialog_repair_confirm);
	DeleteNew(gpC_dialog_repairall_confirm);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::FinishItemSilvering
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TRIBE::FinishItemSilvering()
{
	DeleteNew(gpC_dialog_silvering_finish);
	DeleteNew(gpC_dialog_silvering_confirm);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::SetChattingInterface
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TRIBE::SetChattingInterface(C_VS_UI_CHATTING * p_chatting)
{
	m_pC_chatting = p_chatting;
}

//-----------------------------------------------------------------------------
// Shop RunningAnnounced
//-----------------------------------------------------------------------------
void	
C_VS_UI_TRIBE::ShopRunningAnnounced() // Shop을 위한 필요한 Window를 띄운다.
{
	if(gbl_vampire_interface)
		InventoryXY(380, 80);
	else
		InventoryXY(380, 85);
	OpenInventory(false);
}

//-----------------------------------------------------------------------------
// Storage RunningAnnounced
//-----------------------------------------------------------------------------
void	
C_VS_UI_TRIBE::StorageRunningAnnounced()
{
	if(gbl_vampire_interface)
		InventoryXY(380, 80);
	else
		InventoryXY(380, 85);
	OpenInventory(false);
}

//-----------------------------------------------------------------------------
// Exchange RunningAnnounced
//-----------------------------------------------------------------------------
void	
C_VS_UI_TRIBE::ExchangeRunningAnnounced()
{
	//OpenInventory();
	//InventoryXY(410, 455);
	
	if(gbl_vampire_interface)
		GearXY(426, 150);
	else
		GearXY(426,80);
	
	OpenGear(false);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::CloseInventoryGearWindow
//
// event lock걸려있으면 닫지 못한다.
//-----------------------------------------------------------------------------
bool C_VS_UI_TRIBE::CloseInventoryGearWindow()
{
	if (gpC_base->EventOccured())
		return false;
	
	bool inventory, gear;
	
	inventory = gpC_window_manager->GetShowState(GetInventoryWindow());
	gear = gpC_window_manager->GetShowState(GetGearWindow());
	
	if (inventory)
		HotKey_Inventory();
	if (gear)
		HotKey_Gear();
	
	return (inventory || gear);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::GetInventoryOpenState
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_TRIBE::GetInventoryOpenState() const
{
	return m_pC_inventory->Running();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::GetGearOpenState
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_TRIBE::GetGearOpenState() const
{
	return GetGearWindow()->Running();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::HotKey_Inventory
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TRIBE::HotKey_Inventory()
{
	if (gpC_base->EventOccured() == true)
		return;
	
	if (!GetInventoryOpenState())
		OpenInventory();
	else
		CloseInventory();
	
	if(gbl_vampire_interface)
		PlaySound(SOUND_VAMPIRE_BUTTON);
	else
		PlaySound(SOUND_SLAYER_BUTTON);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::HotKey_Gear
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TRIBE::HotKey_Gear()
{
	if (gpC_base->EventOccured() == true)
		return;
	
	if (!GetGearOpenState())
		OpenGear();
	else
		CloseGear();
	
	if(gbl_vampire_interface)
		PlaySound(SOUND_VAMPIRE_BUTTON);
	else
		PlaySound(SOUND_SLAYER_BUTTON);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRIBE::HotKey_Skill
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRIBE::HotKey_Mark()
{
	if (gpC_base->EventOccured() == true)
		return;
	
	m_pC_chatting->Run(C_VS_UI_CHATTING::MARK_ID);
}

void	C_VS_UI_TRIBE::HotKey_Skill()
{
	if (gpC_base->EventOccured() == true)
		return;
	
	GetSkillWindow()->ToggleWindow();
}

void	C_VS_UI_TRIBE::HotKey_CharInfo()
{
	if (gpC_base->EventOccured() == true)
		return;
	
	if(gbl_vampire_interface)
		PlaySound(SOUND_VAMPIRE_BUTTON);
	else
		PlaySound(SOUND_SLAYER_BUTTON);
	
	if(gC_vs_ui.IsRunningCharInfo())
		gC_vs_ui.CloseInfo();
	else
		gC_vs_ui.RunInfo(C_VS_UI_INFO::CHARACTER_INFO_MODE);
}

void	C_VS_UI_TRIBE::HotKey_SkillInfo()
{
	if (gpC_base->EventOccured() == true)
		return;
	
	if(gbl_vampire_interface)
		PlaySound(SOUND_VAMPIRE_BUTTON);
	else
		PlaySound(SOUND_SLAYER_BUTTON);
	
	if(gC_vs_ui.IsRunningSkillInfo())
		gC_vs_ui.CloseInfo();
	else
		gC_vs_ui.RunInfo(C_VS_UI_INFO::SKILL_INFO_MODE);
}

void	C_VS_UI_TRIBE::HotKey_Grade1Info()
{
	if (gpC_base->EventOccured() == true)
		return;
	
	if(gbl_vampire_interface)
		PlaySound(SOUND_VAMPIRE_BUTTON);
	else
		PlaySound(SOUND_SLAYER_BUTTON);
	
	if(gC_vs_ui.IsRunningGrade1Info())
		gC_vs_ui.CloseInfo();
	else
		gC_vs_ui.RunInfo(C_VS_UI_INFO::GRADE1_INFO_MODE);
}

void	C_VS_UI_TRIBE::HotKey_Grade2Info()
{
	if (gpC_base->EventOccured() == true)
		return;
	
	if(gbl_vampire_interface)
		PlaySound(SOUND_VAMPIRE_BUTTON);
	else
		PlaySound(SOUND_SLAYER_BUTTON);
	
	if(gC_vs_ui.IsRunningGrade2Info())
		gC_vs_ui.CloseInfo();
	else
		gC_vs_ui.RunInfo(C_VS_UI_INFO::GRADE2_INFO_MODE);
}

void	C_VS_UI_TRIBE::HotKey_Help()
{
	if(gC_vs_ui.IsRunningMainHelp())
	{
		gC_vs_ui.CloseDescDialog();
		gC_vs_ui.RunDescDialog(DID_HELP, NULL);
	}
	else
	{
		if(/*m_bl_help && */gC_vs_ui.IsRunningDescDialog())
		{
			gC_vs_ui.CloseDescDialog();
			//			m_bl_help = false;
		}
		else
		{
			//			if(gC_vs_ui.IsRunningDescDialog())return;
			gC_vs_ui.RunDescDialog(DID_HELP, NULL);
			//			m_bl_help = true;
		}
	}
	
	if(gbl_vampire_interface)
		PlaySound(SOUND_VAMPIRE_BUTTON);
	else
		PlaySound(SOUND_SLAYER_BUTTON);
	
}

void	C_VS_UI_TRIBE::HotKey_Party()
{
	if(/*m_bl_party && */gC_vs_ui.IsRunningPartyManager())
	{
		gC_vs_ui.ClosePartyManager();
		//		m_bl_party = false;
	}
	else
	{
		//		if(gC_vs_ui.IsRunningPartyManager())return;
		gC_vs_ui.RunPartyManager();
		//		m_bl_party = true;
	}
	
	if(gbl_vampire_interface)
		PlaySound(SOUND_VAMPIRE_BUTTON);
	else
		PlaySound(SOUND_SLAYER_BUTTON);
	
}

void	C_VS_UI_TRIBE::HotKey_Minimap()
{
	ToggleMinimap();
	EMPTY_MOVE;
}

void C_VS_UI_TRIBE::HotKey_ExtendChat()		// ctrl + e
{
	m_pC_chatting->Extend();
}

void C_VS_UI_TRIBE::HotKey_Chat()			// ctrl + c
{
	m_pC_chatting->Run(C_VS_UI_CHATTING::CHAT_NORMAL_ID);
}

void C_VS_UI_TRIBE::HotKey_ZoneChat()		// ctrl + z
{
	m_pC_chatting->Run(C_VS_UI_CHATTING::CHAT_ZONE_ID);
}

void C_VS_UI_TRIBE::HotKey_GuildChat()		// ctrl + g
{
	m_pC_chatting->Run(C_VS_UI_CHATTING::CHAT_GUILD_ID);
}

void C_VS_UI_TRIBE::HotKey_Whisper()		// ctrl + w
{
	m_pC_chatting->Run(C_VS_UI_CHATTING::CHAT_WHISPER_ID);
}


void C_VS_UI_TRIBE::HotKey_F9()
{
	C_VS_UI_SKILL * p_skill = GetSkillWindow();
	
	int slot = p_skill->GetFocusedSlot();
	int id = p_skill->GetSkillID(slot);
	if (id != NOT_SELECTED)
	{
		p_skill->SetHotkey(C_VS_UI_SKILL::HOTKEY1, (ACTIONINFO)id);
	}
	else
	{
		// use
		p_skill->ChangeSkillByHotkey(C_VS_UI_SKILL::HOTKEY1);
	}
}

void C_VS_UI_TRIBE::HotKey_F10()			
{
	C_VS_UI_SKILL * p_skill = GetSkillWindow();
	
	int slot = p_skill->GetFocusedSlot();
	int id = p_skill->GetSkillID(slot);
	if (id != NOT_SELECTED)
	{
		p_skill->SetHotkey(C_VS_UI_SKILL::HOTKEY2, (ACTIONINFO)id);
	}
	else
	{
		// use
		p_skill->ChangeSkillByHotkey(C_VS_UI_SKILL::HOTKEY2);
	}
}

void C_VS_UI_TRIBE::HotKey_F11()			
{
	C_VS_UI_SKILL * p_skill = GetSkillWindow();
	
	int slot = p_skill->GetFocusedSlot();
	int id = p_skill->GetSkillID(slot);
	if (id != NOT_SELECTED)
	{
		p_skill->SetHotkey(C_VS_UI_SKILL::HOTKEY3, (ACTIONINFO)id);
	}
	else
	{
		// use
		p_skill->ChangeSkillByHotkey(C_VS_UI_SKILL::HOTKEY3);
	}
}

void C_VS_UI_TRIBE::HotKey_F12()
{
	C_VS_UI_SKILL * p_skill = GetSkillWindow();
	
	int slot = p_skill->GetFocusedSlot();
	int id = p_skill->GetSkillID(slot);
	if (id != NOT_SELECTED)
	{
		p_skill->SetHotkey(C_VS_UI_SKILL::HOTKEY4, (ACTIONINFO)id);
	}
	else
	{
		// use
		p_skill->ChangeSkillByHotkey(C_VS_UI_SKILL::HOTKEY4);
	}
}

void	C_VS_UI_TRIBE::DoCommonActionBeforeEventOccured()
{
	//
	// 불필요한 Window들을 닫는다.
	//
	gC_vs_ui.CloseInfo();
	CloseInventory();
	CloseGear();
	GetSkillWindow()->CloseInterface();
	
	// stop all Window move
	GetInventoryWindow()->AttrWindowMove(false);
	GetGearWindow()->AttrWindowMove(false);
	
	// backup xy
	m_backup_inventory_xy.x = GetInventoryWindow()->x;
	m_backup_inventory_xy.y = GetInventoryWindow()->y;
	m_backup_gear_xy.x = GetGearWindow()->x;
	m_backup_gear_xy.y = GetGearWindow()->y;
	
	gpC_base->StartEvent();
}

//-----------------------------------------------------------------------------
// Timer
//
//-----------------------------------------------------------------------------
bool C_VS_UI_TRIBE::Timer()
{
	if(m_dw_prev_tickcount+m_dw_millisec <= GetTickCount())
	{
		m_dw_prev_tickcount = GetTickCount();
		return true;
	}
	
	return false;
}

bool C_VS_UI_TRIBE::MouseControl(UINT message, int _x, int _y)
{
	_x -= x; _y -= y;
	m_pC_common_button_group->MouseControl(message, _x, _y);
	switch(m_selected_tab)
	{
	case TAB_MENU_ID:
		m_pC_menu_button_group->MouseControl(message, _x, _y);
		break;
		
	case TAB_EXP_ID:
		// EXP 는 버튼이 없다
		if(!gbl_vampire_interface)
		{
			char* exp_bar_string[3] = 
			{
				(*g_pGameStringTable)[UI_STRING_MESSAGE_CURRENT_EXP].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_NEXT_LEVEL].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_EXP].GetString(),
			};
			static char temp_str[3][50];
			static LPSTR str[3] = { temp_str[0], temp_str[1], temp_str[2] };
			str[0] = temp_str[0];
			str[1] = temp_str[1];
			str[2] = temp_str[2];
			
			const int bar_x = 98, bar_y = 74, str_x = 27, num_x = 76, bar_gap = 12;
			int num[3] = {-1, -1, -1};
			
			
			if(_x > str_x && _x < 165 && _y > bar_y && _y < bar_y + bar_gap*6)
			{
				int domain[3];
				domain[0] = -1;
				domain[1] = -1;
				domain[2] = -1;

				bool bMax = false, bFame = false;			
				
				int TotalAttr = g_char_slot_ingame.STR_PURE + g_char_slot_ingame.DEX_PURE + g_char_slot_ingame.INT_PURE;
				int DomainLevelMax = -1;
				int fame=-1;
								
				
				if((_y-bar_y)/bar_gap > 2)
				{
					for(int i = 0; i < 3; i++)
					{
						int high_level = -1;
						
						for(int d = SKILLDOMAIN_BLADE ; d <= SKILLDOMAIN_ENCHANT; d++)
						{
							const int domain_level = (*g_pSkillManager)[d].GetDomainLevel();
							if(d != domain[0] && d != domain[1] && d != domain[2] && domain_level > high_level)
							{
								domain[i] = d;
								high_level = domain_level;
							}

							DomainLevelMax = max(DomainLevelMax, (*g_pSkillManager)[d].GetDomainLevel() ) ;
						}
					}
				}
				

				switch((_y-bar_y)/bar_gap)
				{
				case 0:	// str
					num[0] = g_char_slot_ingame.STR_EXP_CUR;
					num[1] = g_pExperienceTable->GetSTRInfo(g_char_slot_ingame.STR_PURE).AccumExp;
					num[2] = g_pExperienceTable->GetSTRInfo(g_char_slot_ingame.STR_PURE).AccumExp - g_char_slot_ingame.STR_EXP_CUR;
					
					// 도메인 레벨이 100이하이면서 능력치 총합이 300이하이면..
					// 하나의 능력치는 200을 넘을 수 없다.
					if( DomainLevelMax <= MAX_SLAYER_DOMAIN_SUM_OLD && TotalAttr <= MAX_SLAYER_ATTR_SUM_OLD )
					{
	//					DEBUG_ADD_FORMAT(
						//DEBUG_ADD("SD");
						
						if( g_char_slot_ingame.STR_PURE >= MAX_SLAYER_ATTR_OLD )
							bMax = true;
					}			
					
					break;
					
				case 1:	// dex
					num[0] = g_char_slot_ingame.DEX_EXP_CUR;
					num[1] = g_pExperienceTable->GetDEXInfo(g_char_slot_ingame.DEX_PURE).AccumExp;
					num[2] = g_pExperienceTable->GetDEXInfo(g_char_slot_ingame.DEX_PURE).AccumExp - g_char_slot_ingame.DEX_EXP_CUR;

					// 도메인 레벨이 100이하이면서 능력치 총합이 300이하이면..
					if( DomainLevelMax <= MAX_SLAYER_DOMAIN_SUM_OLD && TotalAttr <= MAX_SLAYER_ATTR_SUM_OLD )
					{
						if( g_char_slot_ingame.DEX_PURE >= MAX_SLAYER_ATTR_OLD )
							bMax = true;
					}			
					
					break;
					
				case 2:	// int
					num[0] = g_char_slot_ingame.INT_EXP_CUR;
					num[1] = g_pExperienceTable->GetINTInfo(g_char_slot_ingame.INT_PURE).AccumExp;
					num[2] = g_pExperienceTable->GetINTInfo(g_char_slot_ingame.INT_PURE).AccumExp - g_char_slot_ingame.INT_EXP_CUR;
					// 도메인 레벨이 100이하이면서 능력치 총합이 300이하이면..
					if( DomainLevelMax <= MAX_SLAYER_DOMAIN_SUM_OLD && TotalAttr <= MAX_SLAYER_ATTR_SUM_OLD )
					{
						if( g_char_slot_ingame.INT_PURE >= MAX_SLAYER_ATTR_OLD )
							bMax = true;
					}			
					
					break;
					
				case 3:	// skill1
					if(g_pSkillManager)
					{
						int level = (*g_pSkillManager)[domain[0]].GetDomainLevel();
						int exp = (*g_pSkillManager)[domain[0]].GetDomainExp();
						
						if(level >= 0 && exp >= 0)
						{
							int next_exp = (*g_pSkillManager)[domain[0]].GetExpInfo(level).AccumExp;
							
							num[0] = exp;
							num[1] = next_exp;
							num[2] = next_exp - exp;
						}
						
						fame = g_pFameInfoTable->GetFameForLevel( (SKILLDOMAIN) domain[0], level );
							
						if(g_char_slot_ingame.FAME < fame )
							bFame = true;							
					}
					break;
					
				case 4:	// skill2
					if(g_pSkillManager)
					{
						int level = (*g_pSkillManager)[domain[1]].GetDomainLevel();
						int exp = (*g_pSkillManager)[domain[1]].GetDomainExp();
						
						if(level >= 0 && exp >= 0)
						{
							int next_exp = (*g_pSkillManager)[domain[1]].GetExpInfo(level).AccumExp;
							
							num[0] = exp;
							num[1] = next_exp;
							num[2] = next_exp - exp;
						}
						fame = g_pFameInfoTable->GetFameForLevel( (SKILLDOMAIN) domain[1], level );
							
						if(g_char_slot_ingame.FAME < fame )
							bFame = true;													
					}
					break;
					
				case 5:	// skill3
					if(g_pSkillManager)
					{
						int level = (*g_pSkillManager)[domain[2]].GetDomainLevel();
						int exp = (*g_pSkillManager)[domain[2]].GetDomainExp();
						
						if(level >= 0 && exp >= 0)
						{
							int next_exp = (*g_pSkillManager)[domain[2]].GetExpInfo(level).AccumExp;
							
							num[0] = exp;
							num[1] = next_exp;
							num[2] = next_exp - exp;
						}

						
						fame = g_pFameInfoTable->GetFameForLevel( (SKILLDOMAIN) domain[2], level );
							
						if(g_char_slot_ingame.FAME < fame )
							bFame = true;													
					}
					break;
				}
				
				
				// 숫자사이에 ,넣기
				char sz_temp[100];
				for(int j = 0; j < 3; j++)
				{
					strcpy(temp_str[j], exp_bar_string[j]);
					wsprintf(sz_temp, "%d", num[j]);
					std::string sstr = sz_temp;
					for(int i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
						
					strcat(temp_str[j], sstr.c_str());
				}
				
				if(num[2] < 0)
					bMax = true;	// 레벨업맥스
								
				if(bMax)
				{
					strcpy(temp_str[1], (*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_UP_STAT].GetString());
					str[2] = NULL;
					g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str, 2);
				}
				else
				if(bFame)
				{
					wsprintf(temp_str[1],"%s(%s:%d)",(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_UP_LEVEL_BY_FAME].GetString(), 
						(*g_pGameStringTable)[UI_STRING_MESSAGE_NEED_FAME].GetString(),
						fame - g_char_slot_ingame.FAME );					
					str[2] = NULL;
					g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str, 2);					
				} else
					g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str, 3);
				
			}
		}
		break;
		
	case TAB_GUILD_ID:
		m_pC_guild_button_group->MouseControl(message, _x, _y);
		break;
		
	case TAB_MSG_ID:
		m_pC_msg_button_group->MouseControl(message, _x, _y);
		break;
		
	case TAB_HELP_ID:
		m_pC_help_button_group->MouseControl(message, _x, _y);
		break;
	}
	
	return true;
	
}

void C_VS_UI_TRIBE::Start()
{
	PI_Processor::Start();
	
	gpC_window_manager->AppearWindow(this);
	
	m_pC_chatting->Start();
	m_pC_common_button_group->Init();
	m_pC_menu_button_group->Init();
	m_pC_guild_button_group->Init();
	m_pC_msg_button_group->Init();
	m_pC_help_button_group->Init();
	
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::MAINMENU));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::MAINMENU));
	SetSelectTab(gpC_vs_ui_window_manager->GetMainTab());
}
/*-----------------------------------------------------------------------------
- C_VS_UI_GEAR
-
-----------------------------------------------------------------------------*/
C_VS_UI_GEAR::C_VS_UI_GEAR()
{
	g_RegisterWindow(this);
	
	m_focus_slot = NOT_SELECTED;
	
	m_pC_button_group = new ButtonGroup(this);
	
	int close_button_offset_x, close_button_offset_y, help_button_offset_x, help_button_offset_y;
	int alpha_button_offset_x, alpha_button_offset_y;
	
	if(gbl_vampire_interface)
	{
		m_pC_gear_spk = new C_SPRITE_PACK(SPK_VAMPIRE_GEAR);
		m_pC_gear_slot_spk = new C_SPRITE_PACK(SPK_VAMPIRE_GEAR_SLOT);
		
		Set(526, 128, m_pC_gear_spk->GetWidth(0), m_pC_gear_spk->GetHeight(0));
		
		close_button_offset_x = 162;
		close_button_offset_y = 215;
		help_button_offset_x = 138;
		help_button_offset_y = 215;
		alpha_button_offset_x = 44;
		alpha_button_offset_y = 215;
		
	}
	else
	{
		m_pC_gear_spk = new C_SPRITE_PACK(SPK_SLAYER_GEAR);
		m_pC_gear_slot_spk = new C_SPRITE_PACK(SPK_SLAYER_GEAR_SLOT);
		
		Set(426, 128, m_pC_gear_spk->GetWidth(0), m_pC_gear_spk->GetHeight(0));
		
		close_button_offset_x = 178;
		close_button_offset_y = 314;
		help_button_offset_x = 152;
		help_button_offset_y = 314;
		alpha_button_offset_x = 19;
		alpha_button_offset_y = 314;
		
		//		int first_button_offset_x = 14, first_button_offset_y = 59;
		//		int second_button_offset_x = 45, second_button_offset_y = 59;
		//		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(first_button_offset_x, first_button_offset_y, m_pC_gear_spk->GetWidth(BUTTON_FIRST), m_pC_gear_spk->GetHeight(BUTTON_FIRST), FIRST_ID, this, BUTTON_FIRST));
		//		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(second_button_offset_x, second_button_offset_y, m_pC_gear_spk->GetWidth(BUTTON_SECOND), m_pC_gear_spk->GetHeight(BUTTON_SECOND), SECOND_ID, this, BUTTON_SECOND));
		
	}
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(help_button_offset_x, help_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION), HELP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_offset_x, close_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_X), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_X), CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_X));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_button_offset_x, alpha_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));
	
	m_bl_set_load = false;
}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_GEAR
-
-----------------------------------------------------------------------------*/
C_VS_UI_GEAR::~C_VS_UI_GEAR()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::GEAR, GetAttributes()->alpha);
	if(m_bl_set_load)
	{
		gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::GEAR, Rect(x, y, w, h));
	}
	
	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_gear_spk);
	DeleteNew(m_pC_gear_slot_spk);
	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::CancelPushState
//
// 
//-----------------------------------------------------------------------------
//void	C_VS_UI_GEAR::CancelPushState()
//{
//	m_bl_close_button_pushed = false;
//	m_bl_help_button_pushed = false;
//}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_GEAR::MouseControl(UINT message, int _x, int _y)
{
	const static char* m_gear_button_string[2] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_GEAR_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP_GEAR_WINDOW].GetString(),
	};
	
	const static char* m_slayer_help_string[15] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_HELM].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_NECKLACE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_ARMOR].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_SHIELD].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_WEAPON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_GLOVE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_BELT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_TROUSER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_BRACELET].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_BRACELET].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_RING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_RING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_RING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_RING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_SHOES].GetString(),
	};
	
	const static char* m_vampire_help_string[16] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_NECKLACE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_VAMPIRE_COAT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_BRACELET].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_BRACELET].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_RING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_RING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_RING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_RING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_EARRING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_EARRING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_WEAPON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_WEAPON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_AMULET].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_AMULET].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_AMULET].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EQUIP_AMULET].GetString(),
	};
	
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	
	int i;
	
	const MItem * p_selected_item;
	
	switch (message)
	{
	case M_MOVING:
		for (i = 0 ; i < m_slot_size; i++)
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
				if (loop == ITEM_REF_POINT_COUNT)
				{
					px = g_item_ref_point[m].x+gpC_mouse_pointer->GetPointerX();
					py = g_item_ref_point[m].y+gpC_mouse_pointer->GetPointerY();
				}
				else
				{
					px = _x+x;
					py = _y+y;
				}
				
				if (m_p_slot_rect[i].IsInRect(SCR2WIN_X(px), SCR2WIN_Y(py)))
				{
					if(m_focus_slot != i)
					{
						m_focus_slot = i;
						gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
					}
					
					p_selected_item = gC_vs_ui.GetGearItem(m_focus_slot);
					
					if(!gpC_mouse_pointer->GetPickUpItem())
					{
						if (p_selected_item == NULL)
							g_descriptor_manager.Set(DID_INFO, x+m_p_slot_rect[m_focus_slot].x-2, y+m_p_slot_rect[m_focus_slot].y -2, (void*)(gbl_vampire_interface?m_vampire_help_string:m_slayer_help_string)[i]);
						
						//						if (loop == ITEM_REF_POINT_COUNT)
						//						{
						//							// slot에 들어갈 수 있는 Item인가?
						//							MItem * p_old_item;
						//							if (gC_vs_ui.CanReplaceItemInGear(gpC_mouse_pointer->GetPickUpItem(), i, p_old_item) == false)
						//								continue;
						//						}
						
						//						if (p_selected_item != NULL)
						else
							g_descriptor_manager.Set(DID_ITEM, x+m_p_slot_rect[m_focus_slot].x, y+m_p_slot_rect[m_focus_slot].y, (void *)p_selected_item);
					}
					//						m_focus_slot = NOT_SELECTED;
					return true;
				}
			}
		}
		if(m_focus_slot != NOT_SELECTED)
		{
			m_focus_slot = NOT_SELECTED;
			gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
		}
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if (gpC_mouse_pointer->GetPickUpItem() == false && re && gC_vs_ui.GetGearItem(m_focus_slot) == false)
			//				 TestSlotRect(_x, _y) == false && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
			break;
		}
		
		//
		// Item을 집던가 놓는다.
		//
		{
			bool ret = Click(x, y, m_p_slot_rect);
			
			// belt를 놓거나 집었을 경우는 quick item을 reset해야 한다.
			//if ((MSlayerGear::GEAR_SLAYER)m_focus_slot == MSlayerGear::GEAR_SLAYER_BELT)
			//{
			//	gC_vs_ui.ResetSlayerQuickItemSize();
			//}
			
			if (ret)
				EMPTY_MOVE;
		}
		break;
		
	case M_LEFTBUTTON_UP:
		break;
		
	case M_RIGHTBUTTON_DOWN:
		if (gbl_repair_running == true && m_focus_slot != -1)
		{
			if (gbl_item_trade_lock == false)
			{
				MItem * p_item;
				
				if(gC_vs_ui.GetGearItem(m_focus_slot) == NULL)break;
				
				int sum = 0;
				for(int i = 0; i < gC_vs_ui.GetGearSize(); i++)
				{
					p_item = (MItem *)gC_vs_ui.GetGearItem(i);
					if(p_item != NULL)
					{
						if(i == MSlayerGear::GEAR_SLAYER_LEFTHAND && !p_item->IsGearSlotLeftHand())
							continue;
						sum += g_pPriceManager->GetItemPrice(p_item, MPriceManager::REPAIR);
					}
				}
				if(sum > 0)
					g_StartRepairAllConfirmDialog(x+m_p_slot_rect[m_focus_slot].x, y+m_p_slot_rect[m_focus_slot].Down(), sum);
			}
		}
		else 
			Use();
		break;
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_GEAR::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char *m_inventory_button_string[6] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP_GEAR_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_GEAR_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_FIRST_GEAR_SET].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SECOND_GEAR_SET].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
	};	
	
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
		{
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_inventory_button_string[5],0,0);
		}
		else
		{
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_inventory_button_string[p_button->GetID()],0,0);
		}
	} else if(p_button->GetID() == FIRST_ID || p_button->GetID() == SECOND_ID)
	{
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_inventory_button_string[p_button->GetID()],0,0);	
	} else
	{
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_inventory_button_string[p_button->GetID()],0,0);
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_GEAR::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
		{
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		}
		else
		{
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
		}
	}
	else if(p_button->GetID() == FIRST_ID || p_button->GetID() == SECOND_ID)
	{
		if(p_button->GetFocusState())
		{			
			if(p_button->GetPressState())
				m_pC_gear_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+2);
			else
				m_pC_gear_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+1);
		}
		else
			m_pC_gear_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index);
	}
	else if(p_button->GetFocusState())
	{		
		if(p_button->GetPressState())
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
	}
	else
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index);
	
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GEAR::Run(id_t id)
{
	switch (id)
	{
	case HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::GEAR);
		break;
		
	case CLOSE_ID:
		if (gbl_repair_running)
		{
			gpC_base->SendMessage(UI_ITEM_REPAIR_FINISHED);
		}
		else if (gbl_silvering_running)
		{
			gpC_base->SendMessage(UI_ITEM_SILVERING_FINISHED);
		}
		else if (gC_vs_ui.IsRunningExchange())
		{
			gpC_base->SendMessage(UI_CLOSE_EXCHANGE);
		}
		else
		{
			gC_vs_ui.HotKey_Gear();
		}
		break;
		
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GEAR::Show()
{
	const static char *m_help_string[2] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_EQUIP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_EQUIP].GetString(),
	};
	
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		if(GetAttributes()->alpha)
		{
			if(!gbl_vampire_interface)
			{
				RECT alpha_rect = { x+5, y+11, x+215, y+341 };
				DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
			}
			m_pC_gear_spk->BltLocked(x, y, GEAR_WINDOW_ALPHA);
		}
		else
			m_pC_gear_spk->BltLocked(x, y, GEAR_WINDOW);
		
		const MItem * p_selected_item = gC_vs_ui.GetGearItem(m_focus_slot);
		
		for (int i=0; i<m_slot_size; i++)
		{
			if(GetAttributes()->alpha && gbl_vampire_interface)
			{
				RECT alpha_rect;
				alpha_rect.left = m_p_slot_rect[i].x+x;
				alpha_rect.top = m_p_slot_rect[i].y+y;
				alpha_rect.right = alpha_rect.left+m_p_slot_rect[i].w;
				alpha_rect.bottom = alpha_rect.top+m_p_slot_rect[i].h;
				
				DrawAlphaBox(&alpha_rect, 2, 0, 0, g_pUserOption->ALPHA_DEPTH);
			}
			
			const MItem* p_item = gC_vs_ui.GetGearItem(i);
			
			if (p_item==NULL)
			{
#ifndef _LIB
				if (m_pC_gear_slot_spk != NULL)
#endif
					// 비어 있는 slot에는 slot image를...
					m_pC_gear_slot_spk->BltLocked(m_p_slot_rect[i].x+x,
					m_p_slot_rect[i].y+y,
					m_p_slot_image[i]);
			}
			else
			{
				// Item이 있는 slot
				
				TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
				
				int item_x = x+m_p_slot_rect[i].x+m_p_slot_rect[i].w/2-gpC_item->GetWidth(frame_id)/2;
				int item_y = y+m_p_slot_rect[i].y+m_p_slot_rect[i].h/2-gpC_item->GetHeight(frame_id)/2;
				
				if (p_selected_item && (p_selected_item->GetID() == p_item->GetID()))
				{
//					if(p_item->IsQuestItem())
//						CIndexSprite::SetUsingColorSet(p_item->GetQuestItemColorset(), 0);
//					else
//					if(p_item->IsUniqueItem())
//						CIndexSprite::SetUsingColorSet(p_item->GetUniqueItemColorset(), 0);
					if(p_item->IsSpecialColorItem())
						CIndexSprite::SetUsingColorSet(p_item->GetSpecialColorItemColorset(), 0);
					else			
						CIndexSprite::SetUsingColorSet(p_item->GetItemOptionColorSet(), 0);
					gpC_item->BltLockedOutline(item_x, item_y, WHITE, frame_id);
				}
				else
				{
//					if(p_item->IsQuestItem())
//						CIndexSprite::SetUsingColorSet(p_item->GetQuestItemColorset(), 0);
//					else
//					if(p_item->IsUniqueItem())
//						CIndexSprite::SetUsingColorSet(p_item->GetUniqueItemColorset(), 0);
										
					if(p_item->IsSpecialColorItem() )
						CIndexSprite::SetUsingColorSet(p_item->GetSpecialColorItemColorset(), 0);
					else			
						CIndexSprite::SetUsingColorSet(p_item->GetItemOptionColorSet(), 0);

					if (p_item->IsAffectStatus() || p_item->IsQuestItem() )
					{
						// frame id -> sprite id
						gpC_item->BltLocked(item_x, item_y, frame_id);
					}
					else
					{
						gpC_item->BltLockedColorSet(item_x, item_y, frame_id, ITEM_DISABLE_COLOR_SET);
					}
				}
			}
		}
		
		// 놓는 위치 미리 알 수 있도록 한다.
		if (gpC_mouse_pointer->GetPickUpItem() && 
			m_focus_slot != NOT_SELECTED)
		{
			MItem * p_item = gpC_mouse_pointer->GetPickUpItem();
			
			TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
			
			int item_x = x+m_p_slot_rect[m_focus_slot].x+m_p_slot_rect[m_focus_slot].w/2-gpC_item->GetWidth(frame_id)/2;
			int item_y = y+m_p_slot_rect[m_focus_slot].y+m_p_slot_rect[m_focus_slot].h/2-gpC_item->GetHeight(frame_id)/2;
			
			MItem * p_old_item;
			if (gC_vs_ui.CanReplaceItemInGear(p_item, m_focus_slot, p_old_item))
			{
				
				if (p_old_item)
					gpC_item->BltLockedOutlineOnly(item_x, item_y, ga_item_blink_color_table[g_blink_value], frame_id);
				else
					gpC_item->BltLockedOutlineOnly(item_x, item_y, POSSIBLE_COLOR, frame_id);
				
				gpC_base->m_p_DDSurface_back->Unlock();
				g_descriptor_manager.Set(DID_INFO, x+m_p_slot_rect[m_focus_slot].x-2, y+m_p_slot_rect[m_focus_slot].y -2, (void*)m_help_string[0], RGB_GREEN);
				
				gpC_base->m_p_DDSurface_back->Lock();
			}
			else
			{
				gpC_base->m_p_DDSurface_back->Unlock();
				g_descriptor_manager.Set(DID_INFO, x+m_p_slot_rect[m_focus_slot].x-2, y+m_p_slot_rect[m_focus_slot].y -2, (void*)m_help_string[1], RGB_RED);
				gpC_base->m_p_DDSurface_back->Lock();
			}			
		}		
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	m_pC_button_group->ShowDescription();
	
	SHOW_WINDOW_ATTR;
	/*
	#ifndef _LIB
	//
	// focus slot rect -- Test
	//
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
	S_SURFACEINFO surface_info;
	SetSurfaceInfo(&surface_info, gpC_base->m_p_DDSurface_back->GetDDSD());
	
	  if (m_focus_slot != NOT_SELECTED)
	  {
	  S_RECT rect;
	  SetRect(rect, m_p_slot_rect[m_focus_slot].x+x, m_p_slot_rect[m_focus_slot].y+y, 
	  m_p_slot_rect[m_focus_slot].w, m_p_slot_rect[m_focus_slot].h);
	  filledRect(&surface_info, &rect, 0xFF);
	  }
	  
		//for (int i=0; i < ITEM_REF_POINT_COUNT; i++)
		//{
		//	putPixel(&surface_info, 
		//		      g_item_ref_point[i].x+gpC_mouse_pointer->GetPointerX(),
		//				g_item_ref_point[i].y+gpC_mouse_pointer->GetPointerY(),
		//				RED);
		//}
		}
		
		  gpC_base->m_p_DDSurface_back->Unlock();
		  char str[100];
		  sprintf(str, "Gear focus slot = %d", m_focus_slot);
		  g_Print(10, 420, str);
		  #endif
	*/
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GEAR::Start(bool bl_set_load)
{
	m_bl_set_load = bl_set_load;
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::GEAR));
	if(bl_set_load == true)
	{
		Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::GEAR);
		if(rect.w != -1)
		{
			x = rect.x;
			y = rect.y;
		}
	}
	
	PI_Processor::Start();
	
	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
	
	//	m_bl_close_button_focused = false;
	//	m_bl_close_button_pushed = false;
	//	m_bl_help_button_focused = false;
	//	m_bl_help_button_pushed = false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GEAR::Finish()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::GEAR, GetAttributes()->alpha);
	if(m_bl_set_load)
	{
		gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::GEAR, Rect(x, y, w, h));
	}
	
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// UnacquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GEAR::UnacquireMouseFocus()
{
	m_focus_slot = NOT_SELECTED;
	m_pC_button_group->UnacquireMouseFocus();
	//	m_bl_close_button_focused = false;
	//	m_bl_help_button_focused = false;
}

//-----------------------------------------------------------------------------
// Click
//
// 현재 Item을 들고 있으면 놓던가(놓을 수 있으면) 교체하고, 들고 있지 않으면
// Gear에 있는 것을 집는다.
//
// 뭔가 했으면 true를, 그렇지않으면 false를 반환한다.
//-----------------------------------------------------------------------------
bool C_VS_UI_GEAR::Click(int window_x, int window_y, Rect * slot_rect)
{
	if (m_focus_slot == NOT_SELECTED || 
		gbl_item_lock == true || 
		gbl_gear_lock == true)
	{
		return false;
	}
	
	int item_x, item_y;
	
	if (gpC_mouse_pointer->GetPickUpItem()) // 들고 있는가?
	{
		const MItem* p_cur_item = gC_vs_ui.GetGearItem(m_focus_slot);
		
		if (gpC_mouse_pointer->GetPickUpItem()->IsInsertToItem( p_cur_item ))
		{
			// 이전에 있던 item에 추가될 수 있는 경우
			
			// 대상 Item과 들고 있는(추가할 Item)을 Client에서 알아야 한다.
			// 들고 있는 Item은 Client에서 access할 수 있으므로 대상 Item을 보낸다.
			gpC_base->SendMessage(UI_ITEM_INSERT_FROM_GEAR,
				m_focus_slot, 
				0, 
				(void *)p_cur_item); // 대상 Item
		}
		else
		{
			MItem * p_old_item = NULL;
			
			if (gC_vs_ui.CanReplaceItemInGear(gpC_mouse_pointer->GetPickUpItem(), m_focus_slot, p_old_item))
			{
				if (p_old_item != NULL) // replace 되는가?
				{
					item_x = window_x+slot_rect[m_focus_slot].x+slot_rect[m_focus_slot].w/2-gpC_item->GetWidth(p_old_item->GetInventoryFrameID());
					item_y = window_y+slot_rect[m_focus_slot].y+slot_rect[m_focus_slot].h/2-gpC_item->GetHeight(p_old_item->GetInventoryFrameID());
					
					gpC_base->SendMessage(UI_ITEM_DROP_TO_GEAR,
						m_focus_slot, 
						MAKEDWORD(item_x, item_y), 
						p_old_item); // 교환될 거
					
					// UI에서 바꿔야 한다.
					//gpC_mouse_pointer->PickUpItem((MItem *)p_old_item);
				}
				else
				{
					// not replace, just drop at empty grid.
					
					gpC_base->SendMessage(UI_ITEM_DROP_TO_GEAR, 
						m_focus_slot, 
						0, 
						NULL);
					
					// 100% 놓을 수 있으니까 UI에서 drop시킨다.
					//gpC_mouse_pointer->DropItem();
				}
				
				// 기어에 넣은 아이템이 벨트라면 벨트를 연다.
				if(gpC_mouse_pointer->GetPickUpItem()->GetItemClass() == MItem::ITEM_CLASS_BELT)
					gC_vs_ui.RunQuickItemSlot();
				
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
		const MItem * p_item = gC_vs_ui.GetGearItem(m_focus_slot);
		
		if (p_item != NULL) // Item이 있다.
		{
			if(gpC_mouse_pointer->IsCursorDescription())
			{
				gC_vs_ui.RunDescDialog(DID_ITEM, (void *)p_item);
			}
			else if (gbl_repair_running == true)
			{
				if (gbl_item_trade_lock == false && 
					p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_GIFT_BOX && 
					p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_STAR &&
					p_item->GetItemClass() != MItem::ITEM_CLASS_VAMPIRE_AMULET &&
					p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_TREE &&
					p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_ETC &&
					!p_item->IsUniqueItem() &&
					!p_item->IsQuestItem())
				{
					// repair 가격이 0보다 클 때... by sigi
					if (g_pPriceManager->GetItemPrice((MItem*)p_item, MPriceManager::REPAIR) > 0)
					{
						m_p_repair_item = (MItem *)p_item;
						g_StartRepairConfirmDialog(window_x+slot_rect[m_focus_slot].x, window_y+slot_rect[m_focus_slot].Down(), p_item->IsChargeItem());
					}
				} else
				{
					gpC_base->SendMessage(UI_MESSAGE_BOX, 
						STRING_MESSAGE_CANNOT_REPAIR, 
						0, 
						NULL);					
				}
			}
			else if (gbl_silvering_running == true)
			{
				if (gbl_item_trade_lock == false)
				{
					// silvering 가격이 0보다 클 때... by larosel
					if (g_pPriceManager->GetItemPrice((MItem*)p_item, MPriceManager::SILVERING) > 0)
					{
						m_p_silvering_item = (MItem *)p_item;
						g_StartSilveringConfirmDialog(window_x+slot_rect[m_focus_slot].x, window_y+slot_rect[m_focus_slot].Down());
					}
				}
			}
			else
			{
				item_x = window_x+slot_rect[m_focus_slot].x+slot_rect[m_focus_slot].w/2-gpC_item->GetWidth(p_item->GetInventoryFrameID())/2;
				item_y = window_y+slot_rect[m_focus_slot].y+slot_rect[m_focus_slot].h/2-gpC_item->GetHeight(p_item->GetInventoryFrameID())/2;
				//gC_vs_ui.RemoveItemInGear(m_focus_slot);
				
				gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_GEAR,
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
			}
		}
	}	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::Use
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GEAR::Use()
{
	if (m_focus_slot == NOT_SELECTED ||
		gbl_item_lock)
		return;
	
	if (gpC_mouse_pointer->GetPickUpItem())
	{
		
	}
	else
	{
		MPlayerGear *pGear = g_pSlayerGear;
		
		if(gbl_vampire_interface)
			pGear = g_pVampireGear;

		const MItem * p_item = pGear->GetItem(m_focus_slot);
		
		if (p_item) // Item이 있다.
		{
			if(IsPlayerInSafePosition() && (p_item->GetItemClass() == MItem::ITEM_CLASS_COUPLE_RING || p_item->GetItemClass() == MItem::ITEM_CLASS_VAMPIRE_COUPLE_RING))
				return;
			
			gpC_base->SendMessage(UI_ITEM_USE_GEAR, m_focus_slot, 0, (MItem *)p_item);
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GEAR::WindowEventReceiver(id_t event)
{
	switch (event)
	{
	case EVENT_WINDOW_MOVE:
		break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::AcquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GEAR::AcquireMouseFocus()
{
	gpC_base->SendMessage(UI_REMOVE_BACKGROUND_MOUSE_FOCUS);
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::AcquireDisappear
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GEAR::AcquireDisappear()
{
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_GEAR::IsPixel(int _x, int _y)
{
	if(Moving()) return true;
	return AnyMatchWindowPixel(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::AnyMatchWindowPixel
//
// Window image의 pixel이 (x, y) 한 점이라도 일치하면 true를 아니면 false를 반환한다.
//
// Item을 들고 있을 때에는 'item 참조포인트'에 따라 진행한다.
//-----------------------------------------------------------------------------
bool C_VS_UI_GEAR::AnyMatchWindowPixel(int _x, int _y) const
{
	if (gpC_mouse_pointer->GetPickUpItem())
	{
		for (int i=0; i < ITEM_REF_POINT_COUNT; i++)
		{
			int px = g_item_ref_point[i].x+gpC_mouse_pointer->GetPointerX();
			int py = g_item_ref_point[i].y+gpC_mouse_pointer->GetPointerY();
			if (m_pC_gear_spk->IsPixel(px-x, py-y))
				return true;
			
		}		
		return false;
	}
	else
	{
		return m_pC_gear_spk->IsPixel(_x-x, _y-y);
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::AllMatchWindowPixel
//
//
//-----------------------------------------------------------------------------
bool C_VS_UI_GEAR::AllMatchWindowPixel(int _x, int _y) const
{
	if (gpC_mouse_pointer->GetPickUpItem())
	{
		for (int i=0; i < ITEM_REF_POINT_COUNT; i++)
		{
			int px = g_item_ref_point[i].x+gpC_mouse_pointer->GetPointerX();
			int py = g_item_ref_point[i].y+gpC_mouse_pointer->GetPointerY();
			if (m_pC_gear_spk->IsPixel(px-x, py-y) == false)
				return false;
			
		}
		
		return true;
	}
	else
	{
		return m_pC_gear_spk->IsPixel(_x-x, _y-y);
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_GEAR::TestSlotRect
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_GEAR::TestSlotRect(int _x, int _y) const
{
	for (int i = 0 ; i < m_slot_size; i++)
	{
		if (_x >= m_p_slot_rect[i].x && _x < m_p_slot_rect[i].x+m_p_slot_rect[i].w &&
			_y >= m_p_slot_rect[i].y && _y < m_p_slot_rect[i].y+m_p_slot_rect[i].h)
		{
			return true;
		}
	}
	return false;
}

/*-----------------------------------------------------------------------------
- ResetScroll
- 
-----------------------------------------------------------------------------*/
void C_VS_UI_CHATTING::ResetScroll()
{
	
	m_pC_scroll_bar->SetSize(Rect(w-23, 35, -1, h-79));
	
	int i=0, line = 0;
	C_VS_UI_CHAT_LINE * p_line;	
	while(p_line = m_pC_history_list.GetLine(i))
	{
		i++;
		const int _ID_GAP = 2;
		
		if(m_chat_filter[p_line->GetCondition()] == false)
			continue;
		
		int vx = CHAT_LINE_START_X;
		
		int bl_backup = 0;
		char *p_temp = NULL;
		if (p_temp = (char *)p_line->GetMsgString())
		{
			int cut_index = CHAT_WINDOW_WIDTH/g_GetStringWidth("F", gpC_base->m_chatting_pi.hfont) - (p_line->GetIdString()==NULL?0:(strlen(p_line->GetIdString())+2));
			
			if(cut_index < strlen(p_temp))
			{
				line++;
			}
			
			line++;
		}
	}
	m_pC_scroll_bar->SetPosMax(line-g_HISTORY_LINE+1);
	m_pC_scroll_bar->SetScrollPos(0);
	
}

//-----------------------------------------------------------------------------
// RestoreHistoryTemp
//
// History temp에 있는 것들을 History에 입력한다.
//-----------------------------------------------------------------------------
void C_VS_UI_CHATTING::RestoreHistoryTemp()
{
	// 옛날 것부터 입력한다.
	
	//	m_bl_whisper_stacked = false;
	//	switch(CHAT)
	//	{
	//		case CHAT:
	//		case ZONE:
	//			m_bl_chat_stacked = false;
	//			m_bl_zone_chat_stacked = false;
	//			break;
	//	
	//		case GUILD:
	//			m_bl_guild_chat_stacked = false;
	//			break;
	//
	//		case PARTY:
	//			m_bl_party_chat_stacked = false;
	//			break;
	//	}
	
	while (m_pC_history_temp_list.Size() > 0)	
	{
		C_VS_UI_CHAT_LINE * p_line = m_pC_history_temp_list.PopBack();
		assert(p_line != NULL);
		
		AddToChatHistory(p_line->GetMsgString(), p_line->GetIdString(), p_line->GetCondition(), p_line->GetColor() );
		delete p_line;
	}
	
	//	while (m_pC_history_guild_temp_list.Size() > 0)
	//	{
	//		C_VS_UI_CHAT_LINE * p_line = m_pC_history_guild_temp_list.PopBack();
	//		assert(p_line != NULL);
	//
	//		AddToChatHistory(p_line->GetMsgString(), p_line->GetIdString(), p_line->GetCondition());
	//		delete p_line;
	//	}
	//
	//	while (m_pC_history_party_temp_list.Size() > 0)
	//	{
	//		C_VS_UI_CHAT_LINE * p_line = m_pC_history_party_temp_list.PopBack();
	//		assert(p_line != NULL);
	//
	//		AddToChatHistory(p_line->GetMsgString(), p_line->GetIdString(), p_line->GetCondition());
	//		delete p_line;
	//	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_CHATTING::SetGuildChat
// 길드채팅 <-> 일반채팅 변환
//-----------------------------------------------------------------------------
void C_VS_UI_CHATTING::SetGuildChat(bool guild)
{
	RestoreHistoryTemp();
	//	m_bl_guild = guild;
	ResetScroll();
}

/*-----------------------------------------------------------------------------
- AddChatToHistory
- history buffer에 추가한다.

  //`buffer size를 넘으면 다시 buf의 맨 앞에 추가하는 데, m_insert_index는 
  //바로 1순위 chat string이 있는 index이다. m_insert_index가 n이면, n 이전 것은
  //2순위 chat string이고, n 다음 것은 가장 옛날 chat string이다.
-----------------------------------------------------------------------------*/
void C_VS_UI_CHATTING::AddChatToHistory(const char * str, const char * sz_id, enum CHAT_LINE_CONDITION condition, DWORD color)
{
	assert(str);
	
	// history를 보고 있을 경우에는 입력하지 않는다.
#ifndef _LIB
	if (m_pC_scroll_bar->GetScrollPos() > 0)
#else
		if (m_pC_scroll_bar->GetScrollPos() > 0 || g_bActiveGame == FALSE)
#endif
		{
			switch(condition)
			{
			case CLD_NORMAL:
				//			m_bl_chat_stacked = true;
				break;
				
			case CLD_ZONECHAT:
				//			m_bl_zone_chat_stacked = true;
				break;
				
			case CLD_GUILD:
				//			m_bl_guild_chat_stacked = true;
				break;
				
			case CLD_PARTY:
				//			m_bl_party_chat_stacked = true;
				break;
				
			case CLD_WHISPER:
				//			m_bl_whisper_stacked = true;
				break;
			}
			
			
			// by larosel
			//		if ( (condition == CLD_PARTY) && m_pC_history_temp_list.Size() == HISTORY_SIZE_TEMP)
			//			delete m_pC_history_guild_temp_list.PopBack();
			//		if ( (condition == CLD_GUILD) && m_pC_history_temp_list.Size() == HISTORY_SIZE_TEMP)
			//			delete m_pC_history_guild_temp_list.PopBack();
			if ( /*(condition == CLD_NOMAL || condition == CLD_ZONECHAT) && */m_pC_history_temp_list.Size() == HISTORY_SIZE_TEMP)
				delete m_pC_history_temp_list.PopBack();
			
			//		if(condition == CLD_PARTY)m_pC_history_guild_temp_list.Add(sz_id, str, condition);
			//		if(condition == CLD_GUILD)m_pC_history_guild_temp_list.Add(sz_id, str, condition);
			//		if((condition == CLD_NOMAL || condition == CLD_ZONECHAT))
			m_pC_history_temp_list.Add(sz_id, str, condition, color );
			
			return;
		}
		
		switch(condition)
		{
		case CLD_NORMAL:
			//		if(chat_mode != CHAT && chat_mode != ZONE)m_bl_chat_stacked = true;
			break;
			
		case CLD_ZONECHAT:
			//		if(chat_mode != CHAT && chat_mode != ZONE)m_bl_zone_chat_stacked = true;
			break;
			
		case CLD_GUILD:
			///		if(chat_mode != GUILD)m_bl_guild_chat_stacked = true;
			break;
			
		case CLD_PARTY:
			//		if(chat_mode != PARTY)m_bl_party_chat_stacked = true;
			break;
			
		case CLD_WHISPER:
			//		// 스크롤이 안되있는 상태에서 WHISPER가 스택될수 없다
			
			break;
		}
		
		AddToChatHistory(str, sz_id, condition, color);
		if(condition == CLD_WHISPER)
		{
			//		AddToChatHistory(str, sz_id, condition);
			AddWhisperID(sz_id);
			if(g_pUserOption->PopupChatByWhisper&&m_chat_filter[FILTER_WHISPER_ID-FILTER_NORMAL_ID]) 
				TimerHide(true);
		}	
}

//-----------------------------------------------------------------------------
// C_VS_UI_CHATTING::AddToChatHistory
//
// //chat history에 sz_str을 넣고 스크롤바를 초기화한다.
//-----------------------------------------------------------------------------
void	C_VS_UI_CHATTING::AddToChatHistory(const char * sz_str, const char * sz_id, CHAT_LINE_CONDITION condition, DWORD color)
{
	if(m_pC_history_list.Size() >= HISTORY_SIZE)
		delete m_pC_history_list.PopBack();
	
	
	//
	// string이 chatting Window의 넓이를 넘으면 자른다.
	//
	// !실제로 출력하는 format과 같게 한 후 넓이를 테스트해야 한다.
	//
	//	char * p_temp = NULL;
	//	int size = 0;
	
	//	HFONT hfont;
	//	if (condition == CLD_ZONECHAT)
	//		hfont = gpC_base->m_user_id_pi.hfont;
	//	else
	//		hfont = gpC_base->m_chatting_pi.hfont;
	
	//	if (sz_id)
	//		size = strlen(sz_id)+1; // + '>'
	//	if (sz_str)
	//		size += strlen(sz_str);
	//
	//	if (size > 0)
	//	{
	//		p_temp = new char[size+1];
	//		memset(p_temp, 0, size+1);
	//
	//		if (sz_id)
	//		{
	//			strcat(p_temp, sz_id);
	//			strcat(p_temp, g_sz_chat_id_divisor);
	//		}
	//		if (sz_str)
	//			strcat(p_temp, sz_str);
	//
	m_pC_history_list.Add(sz_id, sz_str, condition, color);	// by larosel
	//	}
	
	//	DeleteNewArray(p_temp);
	if(m_pC_scroll_bar->GetScrollPos() == 0)
		ResetScroll();
}

/*-----------------------------------------------------------------------------
- MouseControl
- Slayer/Vampire main interface Mouse input.
-----------------------------------------------------------------------------*/
bool C_VS_UI_CHATTING::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	int scroll = m_pC_scroll_bar->GetScrollPos();
	
	bool re = true;// = m_pC_button_group->MouseControl(message, _x, _y);
	if(m_sub_window != 0 && m_sub_rect.IsInRect(_x, _y))
	{
		gpC_mouse_pointer->SetCursorDefault();
		// sub window 안에 있으면 sub scroll
		if(m_sub_window != 2)
			re &= m_pC_sub_scroll_bar->MouseControl(message, _x-m_sub_rect.x, _y-m_sub_rect.y);
		switch(message)
		{
		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			{
				POINT old_selected = m_sub_selected;
				if(m_sub_window == 1)	// mark
				{
					m_sub_selected.x = (_x - m_sub_rect.x-MARK_X)/14;
					m_sub_selected.y = (_y - m_sub_rect.y-MARK_Y)/14+m_pC_sub_scroll_bar->GetScrollPos();
					if(m_sub_selected.x < 0 || m_sub_selected.x > 8 || m_sub_selected.y < 0 || m_sub_selected.y >= MARK_MAX)
						m_sub_selected.x = -1;
					if(m_sub_selected.x != -1 && !(g_pUserOption->UseEnterChat && m_bl_input_mode == false))
					{
						char_t ch;
						memcpy(&ch, g_mark[m_sub_selected.y][m_sub_selected.x], 2);
						InsertMark(ch);
					}
				}
				else if(m_sub_window == 2)	// color
				{
					
					m_sub_selected.x = (_x - m_sub_rect.x-COLOR_X)/30;
					m_sub_selected.y = (_y - m_sub_rect.y-COLOR_Y)/17;
					if(m_sub_selected.x < 0 || m_sub_selected.x > 3 || m_sub_selected.y < 0 || m_sub_selected.y > 3)
						m_sub_selected.x = -1;
					if(m_sub_selected.x != -1)
					{
						if(g_pUserOption->ChatWhite)
							g_pUserOption->ChattingColor = gpC_base->m_chatting_pi.text_color;
						else
							g_pUserOption->ChattingColor = g_color[m_sub_selected.y][m_sub_selected.x];
						
						m_lev_chatting.SetInputStringColor(g_pUserOption->ChattingColor);
					}
					if(old_selected.x == m_sub_selected.x && old_selected.y == m_sub_selected.y)
						m_sub_window = 0;
				}
				
				//if(message == M_LB_DOUBLECLICK)
				//	m_sub_window = 0;
			}
			break;
			
		case M_WHEEL_UP:
			m_pC_sub_scroll_bar->ScrollUp();
			break;
			
		case M_WHEEL_DOWN:
			m_pC_sub_scroll_bar->ScrollDown();
			break;
		}
		return true;
	}
	
	_x -= x; _y -= y;
	re &= m_pC_button_group->MouseControl(message, _x, h-_y);
	re &= m_pC_scroll_bar->MouseControl(message, _x, _y);
	
	if(!g_pUserOption->UseEnterChat || m_bl_input_mode == true)
	{
		re &= m_pC_input_button_group->MouseControl(message, _x, h-_y);
		re &= m_pC_input_right_button_group->MouseControl(message, w-_x, h-_y);
	}
	
	
	switch (message)
	{
	case M_MOVING:
		if(!Moving())
		{
			m_resize = RESIZE_NOT;
			if(_x >= 0 && _x <= 5)
				m_resize = RESIZE_LEFT;
			if(_x >= w-5 && _x < w)
				m_resize = RESIZE_RIGHT;
			if(_y >= 0 && _y <= 5)
			{
				if(m_resize == RESIZE_LEFT)
					m_resize = RESIZE_LEFTTOP;
				else if(m_resize == RESIZE_RIGHT)
					m_resize = RESIZE_RIGHTTOP;
				else m_resize = RESIZE_TOP;
			}
			if(_y >= h-5 && _y < h)
			{
				if(m_resize == RESIZE_LEFT)
					m_resize = RESIZE_LEFTBOTTOM;
				else if(m_resize == RESIZE_RIGHT)
					m_resize = RESIZE_RIGHTBOTTOM;
				else m_resize = RESIZE_BOTTOM;
			}
			if(m_resize == RESIZE_NOT)
				gpC_mouse_pointer->SetCursorDefault();
			else
				gpC_mouse_pointer->SetCursorResize(this);
		}
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		
		if(m_bl_whisper_mode && re)
		{
			if(IsSpreadID())
			{
				RECT rect = { CHAT_LINE_START_X+15-x, CHAT_LINE_START_Y-y - GetWhisperSize()*FONT_GAP -3, CHAT_LINE_START_X+85-x, CHAT_LINE_START_Y -3-y };			
				if(_x > rect.left && _x < rect.right && _y > rect.top && _y < rect.bottom)
				{
					int select = (_y - rect.top)/FONT_GAP;
					if(GetWhisperFocus())
					{
						m_lev_chatting.EraseAll();
						m_lev_chatting.AddString( GetWhisperID(select).c_str() );
					}
					else
					{
						m_sz_whisper_backup = GetWhisperID(select);
					}
					m_bl_spreadID = false;
					break;
				}
			}
			
			
			if(_y > 100 && _y < 120)
			{
				if(_x > 51 && _x < 129 && !m_bl_focus_whisper)
					ChangeWhisperFocus();
				else if(_x > 129 && _x < w-50 && m_bl_focus_whisper)
					ChangeWhisperFocus();
				break;
			}
		}
		if (gpC_mouse_pointer->GetPickUpItem() == false && re)
		{
			if(m_resize != RESIZE_NOT)
			{
				m_backup_window_point.x = x;
				m_backup_window_point.y = y;
				//					AttrStatch(false);
			}
			m_backup_rect.Set(x, y, w, h);
			MoveReady();
			SetOrigin(_x, _y);
			break;
		}
		break;
		
	case M_LEFTBUTTON_UP:
		CancelPushState();
		//			AttrStatch(true);
		break;
		
	case M_WHEEL_UP:
		m_pC_scroll_bar->ScrollUp();
		break;
		
	case M_WHEEL_DOWN:
		m_pC_scroll_bar->ScrollDown();
		break;
	}
	
	if(scroll > 0 && m_pC_scroll_bar->GetScrollPos() == 0)
		RestoreHistoryTemp();
		/*
		// Mouse Control Extra
		RECT rect;
		if(!m_bl_spreadID)
		{
		if (g_HISTORY_LINE > HISTORY_LINE)
		{
		int pixelY = (g_HISTORY_LINE - HISTORY_LINE) * FONT_GAP;
		
		  rect.left = 18;
		  rect.top = 506 - pixelY;
		  rect.right = 560;
		  rect.bottom = 574;
		  }
		  else
		  {
		  rect.left = 18;
		  rect.top = 506;
		  rect.right = 560;
		  rect.bottom = 574;
		  }
		  
			//------------------------------------------------------------
			// 일단, 전체 영역에 포함되는지 본다.
			//------------------------------------------------------------
			if (x > rect.left && x < rect.right
			&& y > rect.top && y < rect.bottom)
			{
			switch (message)
			{	
			case M_LEFTBUTTON_DOWN:
			case M_RIGHTBUTTON_DOWN:
			case M_LB_DOUBLECLICK:	
			{
			if (x > rect.left && x < rect.left + 120)
			{
			//------------------------------------------------------------
			// 채팅창에서 이름 선택하기 - by sigi
			//------------------------------------------------------------				
			// FONT_GAP으로 몇번째 줄인지 판단한다.
			int yLine = -((y - (CHAT_HISTORY_START_Y+FONT_GAP)) / FONT_GAP);				
			
			  //	by larosel
			  C_VS_UI_CHAT_LINE * p_line = NULL;	
			  switch(CHAT)//gC_vs_ui.GetChatMode())
			  {
			  case ZONE:
			  case CHAT:
			  p_line = m_pC_history_list.GetLine(yLine + m_chat_scrollbar.GetAmountToScroll());
			  break;
			  
				case GUILD:
				p_line = m_pC_history_guild_list.GetLine(yLine + m_chat_scrollbar.GetAmountToScroll());
				break;
				
				  case PARTY:
				  p_line = m_pC_history_party_list.GetLine(yLine + m_chat_scrollbar.GetAmountToScroll());
				  break;
				  }
				  
					if (p_line!=NULL)
					{
					const char* pID = p_line->GetIdString();
					
					  if (pID!=NULL)
					  {
					  //							gpC_base->SendMessage(UI_CHAT_SELECT_NAME, message, 0, (void*)pID);												
					  if(m_lev_chatting.GetString()[0] == '*')	// 운영자 명령어인경우 채팅창에 넣어준다
					  {
					  m_lev_chatting.AddString(" ");
					  m_lev_chatting.AddString(pID);
					  }
					  else
					  {
					  //									AddWhisperID(pID);
					  // 내가 귓속말 보낼때 '[쑥갓] 초변태>어쩌구' 를 클릭하면 '[쑥갓] 초변태'가 귓말 아이디에 들어가는거 방지
					  if(strchr(pID, ' ') != NULL)
					  SetWhisperID(strchr(pID,' ')+1);
					  else
					  SetWhisperID((char *)pID);
					  }
					  }
					  }
					  }
					  }
					  break;
					  
						case M_WHEEL_UP:
						Scroll(UP);
						break;
						
						  case M_WHEEL_DOWN:
						  Scroll(DOWN);
						  break;
						  
							}
							
							  return true;
							  }
							  }
							  else
							  {
							  SetRect(&rect, CHAT_LINE_START_X, CHAT_LINE_START_Y - GetWhisperSize()*FONT_GAP -3, CHAT_LINE_START_X+70, CHAT_LINE_START_Y -3);
							  
								if (x > rect.left && x < rect.right
								&& y > rect.top && y < rect.bottom)
								{
								switch (message)
								{	
								case M_LEFTBUTTON_DOWN:
								case M_RIGHTBUTTON_DOWN:
								case M_LB_DOUBLECLICK:	
								{
								//------------------------------------------------------------
								// 스프레드된 아이디 창에서 아이디 선택하기 by larosel
								//------------------------------------------------------------				
								// FONT_GAP으로 몇번째 줄인지 판단한다.
								int yLine = (y - rect.top)/FONT_GAP;
								SetWhisperID((char *)GetWhisperID(yLine).c_str());
								
								  m_bl_spreadID = false;
								  
									}
									break;
									}
									
									  return true;
									  }
									  
										}
										
*/
return true;
}

void C_VS_UI_CHATTING::SetWhisperID(char *id)
{
	if(!GetWhisperFocus())
	{
		m_sz_whisper_backup = id;
	}
	else
	{
		m_lev_chatting.EraseAll();
		m_lev_chatting.AddString(id);
		ChangeWhisperFocus();
	}
}
/*-----------------------------------------------------------------------------
- MouseControl Extra - by sigi
- Slayer/Vampire main interface Mouse input.

  UI구조상.. 영역 밖의 mouse입력이 어려워서..
-----------------------------------------------------------------------------*/
/*
bool C_VS_UI_CHATTING::MouseControlExtra(UINT message, int x, int y)
{
//------------------------------------------------------------
// 채팅창에서 이름 선택하기 - by sigi
//------------------------------------------------------------
RECT rect;
if(!m_bl_spreadID)
{
if (g_HISTORY_LINE > HISTORY_LINE)
{
int pixelY = (g_HISTORY_LINE - HISTORY_LINE) * FONT_GAP;

  rect.left = 18;
  rect.top = 506 - pixelY;
  rect.right = 560;
  rect.bottom = 574;
		}
		else
		{
		rect.left = 18;
		rect.top = 506;
		rect.right = 560;
		rect.bottom = 574;
		}
		
		  //------------------------------------------------------------
		  // 일단, 전체 영역에 포함되는지 본다.
		  //------------------------------------------------------------
		  if (x > rect.left && x < rect.right
		  && y > rect.top && y < rect.bottom)
		  {
		  switch (message)
		  {	
		  case M_LEFTBUTTON_DOWN:
		  case M_RIGHTBUTTON_DOWN:
		  case M_LB_DOUBLECLICK:	
		  {
		  if (x > rect.left && x < rect.left + 120)
		  {
		  //------------------------------------------------------------
		  // 채팅창에서 이름 선택하기 - by sigi
		  //------------------------------------------------------------				
		  // FONT_GAP으로 몇번째 줄인지 판단한다.
		  int yLine = -((y - (CHAT_HISTORY_START_Y+FONT_GAP)) / FONT_GAP);				
		  
			//	by larosel
			C_VS_UI_CHAT_LINE * p_line;	
			if(m_bl_guild)p_line = m_pC_history_guild_list.GetLine(yLine + m_chat_scrollbar.GetAmountToScroll());
			else p_line = m_pC_history_list.GetLine(yLine + m_chat_scrollbar.GetAmountToScroll());
			
			  if (p_line!=NULL)
			  {
			  const char* pID = p_line->GetIdString();
			  
				if (pID!=NULL)
				{
				//							gpC_base->SendMessage(UI_CHAT_SELECT_NAME, message, 0, (void*)pID);												
				AddWhisperID(pID);	// by larosel
				}
				}
				}
				}
				break;
				
				  case M_WHEEL_UP:
				  Scroll(UP);
				  break;
				  
					case M_WHEEL_DOWN:
					Scroll(DOWN);
					break;
					
					  }
					  
						return true;
						}
						}
						else
						{
						SetRect(&rect, CHAT_LINE_START_X, CHAT_LINE_START_Y - GetWhisperSize()*FONT_GAP -3, CHAT_LINE_START_X+70, CHAT_LINE_START_Y -3);
						
						  if (x > rect.left && x < rect.right
						  && y > rect.top && y < rect.bottom)
						  {
						  switch (message)
						  {	
						  case M_LEFTBUTTON_DOWN:
						  case M_RIGHTBUTTON_DOWN:
						  case M_LB_DOUBLECLICK:	
						  {
						  //------------------------------------------------------------
						  // 스프레드된 아이디 창에서 아이디 선택하기 by larosel
						  //------------------------------------------------------------				
						  // FONT_GAP으로 몇번째 줄인지 판단한다.
						  int yLine = (y - rect.top)/FONT_GAP;
						  if(!GetWhisperFocus())
						  {
						  m_sz_whisper_backup = GetWhisperID(yLine);
						  }
						  else
						  {
						  m_lev_chatting.EraseAll();
						  m_lev_chatting.AddString(GetWhisperID(yLine).c_str());
						  ChangeWhisperFocus();
						  }
						  
							m_bl_spreadID = false;
							
							  }
							  break;
							  }
							  
								return true;
								}
								
								  }
								  
									return false;
									}
*/

/*-----------------------------------------------------------------------------
- KeyboardControl
- Slayer/Vampire main interface Keyboard input.
-----------------------------------------------------------------------------*/
void C_VS_UI_CHATTING::KeyboardControl(UINT message, UINT key, long extra)
{
	if(g_pUserOption->UseEnterChat && m_bl_input_mode == false)
	{
		if(message == WM_KEYDOWN && key == VK_RETURN)
		{
			m_bl_input_mode = true;
		}
		return;
	}
	
	// Hiding 중에는 키보드 입력 안받음
	if((x < 0 || x+w > RESOLUTION_X || y < 0 || y+h > RESOLUTION_Y) && g_pUserOption->UseEnterChat)
		return;
	
	// 스크롤중에 엔터를 안받는건 왜냐-ㅅ- 입력되면 스크롤 돌리구 입력해야지-_-;
	if (message == WM_KEYDOWN && key == VK_RETURN && m_pC_scroll_bar->GetScrollPos() > 0)
	{
		RestoreHistoryTemp();
	}
	
	int old_cursor = m_lev_chatting.GetCursor();
	
	gC_ci.IME_MessageProcessor(message, key, extra);
	
	/*	if (message == WM_CHAR)
	if (key == '/')
	if (m_lev_chatting.Size() == 1)
	{
				std::string ID = GetWhisperID();
				if(ID.size())
				{
				m_lev_chatting.AddString( ID.c_str() );
				m_lev_chatting.AddString( " " );
				}
				}
	*/
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_RETURN:
			if(Timer())
			{
				break;
			}
			
			if(GetWhisperFocus())
			{
				ChangeWhisperFocus();
				break;
			}
			// 입력된 것이 없으면 무효이다.
			if (m_lev_chatting.Size() > 0)
			{
				//
				// sz_chat_str
				//
				// DBCS를 ASCII로 바꿀 때 사용하는 buf.
				// 실제로 UI_CHAT_RETURN을 받을 때 chatting string이므로 메시지를 처리하고나서 반드시 DeleteNewArray를
				// 해줘야 한다.
				//
				// UI에서 이 ptr을 관리하면 동기화문제인지몰라도 글자가 깨지는 경우가 있다. UI_CHAT_RETURN이
				// 처리되지 않은 상태에서 g_Convert_DBCS_Ascii2SingleByte()가 실행되서 그런 것 같다.
				//
				
				char * sz_chat_str = NULL;
				g_Convert_DBCS_Ascii2SingleByte(m_lev_chatting.GetString(), m_lev_chatting.Size(), sz_chat_str);
				
				// 타이핑 했던 문장 기억하기
				if(m_history.size() == m_history_line)
				{
					if(m_history.size() == 20)
					{
						m_history.erase(&m_history[0]);
					}
					
					PAPERING_HISTORY temp_history;
					
					temp_history.m_string = sz_chat_str;
					temp_history.m_timer.push_back(GetTickCount());
					m_history.push_back(temp_history);
					m_history_line = m_history.size();
				}
				else
				{
					PAPERING_HISTORY temp_history = m_history[m_history_line];
					
					if(temp_history.m_timer.size() == 5)
					{
						if(temp_history.m_timer[0] + 20000 > GetTickCount() && sz_chat_str[0] != '*' && strstr(g_char_slot_ingame.sz_name.c_str(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString()) == NULL)
						{
							Timer(true);
							m_timer = TIMER_PAPERING;
							delete [] sz_chat_str;
							break;
						}
						else
						{
							temp_history.m_timer.erase(&temp_history.m_timer[0]);
						}
					}
					
					m_history.erase(&m_history[m_history_line]);
					temp_history.m_string = sz_chat_str;
					temp_history.m_timer.push_back(GetTickCount());
					m_history.push_back(temp_history);
					m_history_line = m_history.size();
				}
				
				// 2초동안 5문장 말하면 막기
				if(m_dw_rep_tickcount.size()==5 && m_dw_rep_tickcount[0] + 2000 > GetTickCount() && sz_chat_str[0] != '*' && strstr(g_char_slot_ingame.sz_name.c_str(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString()) == NULL)
				{
					Timer(true);
					m_timer = TIMER_REP;
					delete [] sz_chat_str;
					break;
				}
				
				if(m_dw_rep_tickcount.size() == 5)
				{
					m_dw_rep_tickcount.erase(&m_dw_rep_tickcount[0]);
				}
				m_dw_rep_tickcount.push_back(GetTickCount());
				
				char *pTempstr;
				
				int condition = CLD_NORMAL;
				
				if(m_bl_whisper_mode)
				{
					condition = CLD_WHISPER;
					pTempstr = new char [strlen(sz_chat_str) + strlen(m_sz_whisper_backup.c_str()) +3];
					strcpy(pTempstr, "/");
					strcat(pTempstr, m_sz_whisper_backup.c_str());
					strcat(pTempstr, " ");
					strcat(pTempstr, sz_chat_str);
					delete [] sz_chat_str;
				}else
				{
					condition = m_chat_mode;
					pTempstr = sz_chat_str;
					if(condition == CLD_ZONECHAT)
					{
						if(strstr(g_char_slot_ingame.sz_name.c_str(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString()) == NULL)
						{
							Timer(true);
							m_timer = TIMER_ZONECHAT;
						}
					}
				}
				
				
				if(g_pUserOption->ChatWhite)
					gpC_base->SendMessage(UI_CHAT_RETURN, condition, gpC_base->m_chatting_pi.text_color, pTempstr);	// by larosel
				else
					gpC_base->SendMessage(UI_CHAT_RETURN, condition, g_pUserOption->ChattingColor, pTempstr);	// by larosel
				
				if(pTempstr[0] == '/')AddWhisperID(pTempstr + 1);
				
				m_lev_chatting.EraseAll();
				if(m_bl_whisper_mode)m_whisper_index = GetWhisperSize() -1;
				else m_whisper_index = -1;
				
				//					m_bl_spreadID = false;
				
				// message를 보내고 곧바로 확인한다. 왜냐하면 lack에 의해 늦어질 수 있기 때문이다.
				// 이것은 바로 반응해야 하는 것이다.
				RestoreHistoryTemp();
				ResetScroll();	// by larosel
				
				if(!g_pUserOption->UseEnterChat)
					TimerHide(true);
				}
				// 입력된 것이 없을때
				else
				{
					if(g_pUserOption->UseEnterChat && m_bl_input_mode)
						m_bl_input_mode = false;
				}
				break;
				
			case VK_UP:
				if(m_bl_whisper_mode && GetWhisperFocus())
				{
					if(m_whisper_index > 0)m_whisper_index--;
					else if(m_whisper_index == -1)m_whisper_index = GetWhisperSize() -1;
					
					std::string ID = GetWhisperID();
					if(ID.size())
					{
						//						if(m_bl_whisper_mode)m_sz_whisper_backup = ID;
						//						else
						{
							m_lev_chatting.EraseAll();
							m_lev_chatting.AddString( ID.c_str() );
						}
					}
				}
				else
				{
					if(m_history_line > 0)
					{
						m_lev_chatting.EraseAll();
						m_lev_chatting.AddString(m_history[--m_history_line].m_string.c_str());
					}
				}
				
				//				if (m_lev_chatting.Size() == 0)
				//				if(GetWhisperFocus())ChangeWhisperFocus();
				//				else
				//				{
				//					if(m_whisper_index > 0)m_whisper_index--;
				//					else if(m_whisper_index == -1)m_whisper_index = GetWhisperSize() -1;
				//
				//					std::string ID = GetWhisperID();
				//					if(ID.size())
				//					{
				//						if(m_bl_whisper_mode)m_sz_whisper_backup = ID;
				//						else
				//						{
				//							m_lev_chatting.EraseAll();
				//							m_lev_chatting.AddString( "/" );
				//							m_lev_chatting.AddString( ID.c_str() );
				//							m_lev_chatting.AddString( " " );
				//						}
				//					}
				//				}
				break;
				
			case VK_DOWN:
				if(m_bl_whisper_mode && GetWhisperFocus())
				{
					if(m_whisper_index < GetWhisperSize() -1 && m_whisper_index != -1)m_whisper_index++;
					else if(m_whisper_index == GetWhisperSize() -1 && !m_bl_whisper_mode || m_whisper_index == -1)
					{
						m_whisper_index = -1;
						m_lev_chatting.EraseAll();
						break;
					}
					
					std::string ID = GetWhisperID();
					if(ID.size())
					{
						//						if(m_bl_whisper_mode)m_sz_whisper_backup = ID;
						//						else
						{
							m_lev_chatting.EraseAll();
							m_lev_chatting.AddString( ID.c_str() );
							//							m_lev_chatting.AddString( " " );
						}
					}
				}
				else
				{
					if(m_history_line+1 < m_history.size())
					{
						m_lev_chatting.EraseAll();
						m_lev_chatting.AddString(m_history[++m_history_line].m_string.c_str());
					}
					else if(m_history_line+1 == m_history.size())
					{
						m_lev_chatting.EraseAll();
						m_history_line++;
					}
				}
				
				//				if(GetWhisperFocus())ChangeWhisperFocus();
				//				else
				//				if (m_lev_chatting.Size() == 0)
				//				{
				//					if(m_whisper_index < GetWhisperSize() -1 && m_whisper_index != -1)m_whisper_index++;
				//					else if(m_whisper_index == GetWhisperSize() -1 && !m_bl_whisper_mode || m_whisper_index == -1)
				//					{
				//						m_whisper_index = -1;
				//						m_lev_chatting.EraseAll();
				//						break;
				//					}
				//
				//					std::string ID = GetWhisperID();
				//					if(ID.size())
				//					{
				//						if(m_bl_whisper_mode)m_sz_whisper_backup = ID;
				//						else
				//						{
				//							m_lev_chatting.EraseAll();
				//							m_lev_chatting.AddString( "/" );
				//							m_lev_chatting.AddString( ID.c_str() );
				//							m_lev_chatting.AddString( " " );
				//						}
				//					}
				//				}
				break;
				
			case VK_LEFT:
				if(m_bl_whisper_mode && !m_bl_focus_whisper)
				{
					if(old_cursor == 0)ChangeWhisperFocus();
				}
				break;
				
			case VK_RIGHT:
				if(m_bl_whisper_mode && m_bl_focus_whisper)
				{
					if(old_cursor == m_lev_chatting.Size())ChangeWhisperFocus();
				}
				break;
				
		}
	}
}

//-----------------------------------------------------------------------------
// 입력중인 string을 바로 설정한다. by sigi
//-----------------------------------------------------------------------------
void	
C_VS_UI_CHATTING::SetInputString(const char* pString)
{
	m_lev_chatting.EraseAll();		
	
	if (pString!=NULL)
	{
		m_lev_chatting.AddString( pString );
	}
}

//-----------------------------------------------------------------------------
// 입력중인 string을 바로 설정한다. by sigi
//-----------------------------------------------------------------------------
void	
C_VS_UI_CHATTING::AddInputString(const char* pString)
{
	if (pString!=NULL)
	{
		m_lev_chatting.AddString( pString );
	}
}


/*-----------------------------------------------------------------------------
- Show
-
-----------------------------------------------------------------------------*/
void C_VS_UI_CHATTING::Show()
{
	int gap = 0;
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		if(!g_pUserOption->UseEnterChat || m_bl_input_mode == true) gap = m_pC_chatting_spk->GetHeight(INPUT_RIGHT);
		Rect rect(0, 0, w-10, h-gap);
		if(GetAttributes()->alpha)
		{
			if(gbl_vampire_interface)
			{
				RECT alpha_rect = { x+30, y+10, x+w-20, y+h-5 };
				DrawAlphaBox(&alpha_rect, 2, 0, 0, g_pUserOption->ALPHA_DEPTH);
			}
			else
			{
				RECT alpha_rect = { x+35, y+5, x+w-20, y+h-5 };
				DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
			}
			m_pC_chatting_spk->BltLockedClip(x, y, rect, MAIN_ALPHA);
		}
		else
			m_pC_chatting_spk->BltLockedClip(x, y, rect, MAIN);
		rect.w = m_pC_chatting_spk->GetWidth(MAIN_RIGHT);
		m_pC_chatting_spk->BltLockedClip(x+w-rect.w, y, rect, MAIN_RIGHT);
		
		if(gap)
		{
			if(!gbl_vampire_interface)
			{
				rect.w = w;
				rect.h = m_pC_chatting_spk->GetHeight(MAIN_BOTTOM)-5;
				m_pC_chatting_spk->BltLockedClip(x, y+h-gap-rect.h, rect, MAIN_BOTTOM);
			}
			rect.w = w-10;
			//			if(!gbl_vampire_interface)
			//				gap+=4;
			rect.h = gap;
			if(GetAttributes()->alpha)
				m_pC_chatting_spk->BltLockedClip(x, y+h-gap, rect, INPUT_ALPHA);
			else
				m_pC_chatting_spk->BltLockedClip(x, y+h-gap, rect, INPUT);
			m_pC_chatting_spk->BltLocked(x+w-m_pC_chatting_spk->GetWidth(INPUT_RIGHT), y+h-gap, INPUT_RIGHT);
			
			m_pC_input_button_group->Show();
			m_pC_input_right_button_group->Show();
		}
		else
		{
			rect.w = w;
			rect.h = m_pC_chatting_spk->GetHeight(MAIN_BOTTOM);
			m_pC_chatting_spk->BltLockedClip(x, y+h-gap-rect.h, rect, MAIN_BOTTOM);
		}
		m_pC_button_group->Show();
		
		
		if(m_sub_window != 0)
		{
			m_sub_rect.x = x+w-m_sub_rect.w;
			if(y > m_sub_rect.h)
				m_sub_rect.y = y-m_sub_rect.h;
			else
				m_sub_rect.y = y+h;
			gpC_global_resource->DrawDialogLocked(m_sub_rect, GetAttributes()->alpha);
			
			if(m_sub_window == 1)	// mark
			{
				if(m_sub_selected.x != -1)
				{
//					RECT rect = {m_sub_rect.x+MARK_X+m_sub_selected.x*14-1, m_sub_rect.y+MARK_Y-2+(m_sub_selected.y+m_pC_sub_scroll_bar->GetScrollPos())*14, 0, 0};			// hyungony
					RECT rect = {m_sub_rect.x+MARK_X+m_sub_selected.x*14-1, m_sub_rect.y+MARK_Y-2+(m_sub_selected.y-m_pC_sub_scroll_bar->GetScrollPos())*14, 0, 0};
					rect.right = rect.left +15;
					rect.bottom = rect.top +16;
					
					if(m_sub_selected.y >= m_pC_sub_scroll_bar->GetScrollPos() && m_sub_selected.y < m_pC_sub_scroll_bar->GetScrollPos()+5)
						DrawAlphaBox(&rect, 0, 0, 255, g_pUserOption->ALPHA_DEPTH);
				}
				//				
				gpC_base->m_p_DDSurface_back->Unlock();
				
				
				g_FL2_GetDC();
				for(int i = 0; i < 5; i++)
				{
					for(int j = 0; j < 9; j++)
					{
						g_PrintColorStr(m_sub_rect.x + j*14 +MARK_X, m_sub_rect.y + i*14 +MARK_Y, g_mark[i+m_pC_sub_scroll_bar->GetScrollPos()][j], gpC_base->m_chatting_pi, RGB_WHITE);
					}
				}
				g_FL2_ReleaseDC();
				m_pC_sub_scroll_bar->Show(m_sub_rect.x, m_sub_rect.y);
				
				//				if(MARK_MAX > 5)
				//					gpC_global_resource->m_pC_assemble_box_button_spk->Blt(SLAYER_SCROLLTAG_X, 439+18*m_mark_scroll/(MARK_MAX-5), C_GLOBAL_RESOURCE::AB_SLAYER_SCROLL_TAG);
			}
			else if(m_sub_window == 2)	// color
			{
				gpC_base->m_p_DDSurface_back->Unlock();
				RECT rect;
				for(int i = 0; i < 4; i++)
				{
					for(int j = 0; j < 4; j++)
					{
						rect.left = m_sub_rect.x + j*30+COLOR_X ;
						rect.top = m_sub_rect.y + i*17+COLOR_Y ;
						rect.right = rect.left +28;
						rect.bottom = rect.top +15;
						COLORREF color = g_color[i][j];
						BYTE b = (color&0xff0000)>>19;
						BYTE g = (color&0xff00)>>11;
						BYTE r = (color&0xff)>>3;
						WORD c = CDirectDraw::Color(r, g, b);
						gpC_base->m_p_DDSurface_back->FillRect(&rect, c);
					}
				}
				if(m_sub_selected.x != -1)
				{
					gpC_global_resource->m_pC_scroll_bar_spk->Blt(m_sub_rect.x+COLOR_X+m_sub_selected.x*30, m_sub_rect.y+COLOR_Y+m_sub_selected.y*17, C_GLOBAL_RESOURCE::SB_BUTTON);
				}
			}			
		}
		else
			gpC_base->m_p_DDSurface_back->Unlock();
	}
	m_pC_scroll_bar->Show(x, y);
	
	// 이곳이 도움말 파일 불러오게 했던 부분. process 로 옮김 by  sonee
	
	// 타이머로 입력 글씨를 GRAY,RED 설정했던부분. process 로 옮김 by sonee
	
	// 타이머에 맞게 헬프 메시지 추가해주는 부분. process 로 옮김  by sonee
	
	
	if(!g_pUserOption->UseEnterChat || m_bl_input_mode)
	{
		if(m_timer == TIMER_REP || m_timer == TIMER_PAPERING)
		{
			if(m_bl_whisper_mode)
				g_PrintColorStr(CHAT_LINE_START_X+115, CHAT_LINE_START_Y, (*g_pGameStringTable)[UI_STRING_MESSAGE_LIMIT_STRING_COUNT].GetString(), gpC_base->m_chatting_pi, RGB_RED);
			else
				g_PrintColorStr(CHAT_LINE_START_X+15, CHAT_LINE_START_Y, (*g_pGameStringTable)[UI_STRING_MESSAGE_LIMIT_STRING_COUNT].GetString(), gpC_base->m_chatting_pi, RGB_RED);
		}
		else
		{
			if(g_pUserOption->ChatWhite)
				m_lev_chatting.SetInputStringColor(gpC_base->m_chatting_pi.text_color);
			else
				if(!Timer())				// Chating Color Setting. Timer....(도배 및 존 채팅시 제외)
					m_lev_chatting.SetInputStringColor(g_pUserOption->ChattingColor);
			m_lev_chatting.Show();
		}
	}	
	g_FL2_GetDC();	
	for (int i=0, line = 0, scroll = 0; line < g_HISTORY_LINE; line++, i++)
	{
		// by larosel
		C_VS_UI_CHAT_LINE * p_line;	
		p_line = m_pC_history_list.GetLine(i);
		
		const int _ID_GAP = 2;
		
		if (p_line)
		{
			if(m_chat_filter[p_line->GetCondition()] == false)
			{
				line--;
				continue;
			}
			int vx = CHAT_LINE_START_X;
			
			int bl_backup = 0;
			if (p_line->GetMsgString())
			{
				char *p_temp = (char *)p_line->GetMsgString();
				int cut_index = 0;
				
				if(p_line->GetCondition() == CLD_ZONECHAT)
				{
					//으아악~~!!! 하드코딩이다아아아!!!!
					// BOLD를 먹여보리면 한글이랑 영문이랑 글씨크기가 제멋대로자나-ㅅ-					
					char sz_temp[130];
					int smart_size = CHAT_WINDOW_WIDTH  - g_GetStringWidth(p_line->GetIdString(), gpC_base->m_user_id_pi.hfont) -g_GetStringWidth(g_sz_chat_id_divisor[p_line->GetCondition()], gpC_base->m_chatting_pi.hfont) -_ID_GAP;
					cut_index = strlen(p_temp);
					if(g_GetStringWidth(p_temp, gpC_base->m_user_id_pi.hfont) > smart_size)
					{
						strcpy(sz_temp, p_temp);
						while(g_GetStringWidth(sz_temp, gpC_base->m_user_id_pi.hfont) > smart_size)
						{
							sz_temp[cut_index--] = '\0';
						}
					}
				}
				else
					cut_index = (CHAT_WINDOW_WIDTH  - g_GetStringWidth(p_line->GetIdString(), gpC_base->m_user_id_pi.hfont) -g_GetStringWidth(g_sz_chat_id_divisor[p_line->GetCondition()], gpC_base->m_chatting_pi.hfont) -_ID_GAP)/(g_GetStringWidth("F", gpC_base->m_chatting_pi.hfont));
				
				char backup_char;
				
				if(cut_index < strlen(p_temp))
				{
					if (!g_PossibleStringCut(p_temp, cut_index))
						cut_index--;
					
					bl_backup = 1;
					
					assert(cut_index > 0);
				}
				
				// 스크롤 체크.. 왜 여기서 하지-.-
				if(scroll < m_pC_scroll_bar->GetScrollPos())
				{
					if(bl_backup)
					{
						scroll++;
					}
					scroll++;
					line--;
					if(scroll <= m_pC_scroll_bar->GetScrollPos())
					{
						continue;
					}
				}
				
				vx += g_GetStringWidth(p_line->GetIdString(), gpC_base->m_user_id_pi.hfont);
				vx += g_GetStringWidth(g_sz_chat_id_divisor[p_line->GetCondition()], gpC_base->m_chatting_pi.hfont);
				vx += _ID_GAP;
				
				if(bl_backup)
				{
					if(line < g_HISTORY_LINE && line >= 0)
					{
						if (p_line->GetCondition() == CLD_ZONECHAT)
						{
							g_PrintColorStr(vx, CHAT_HISTORY_START_Y-(FONT_GAP*line), p_temp+cut_index, gpC_base->m_user_id_pi, p_line->GetColor());
						}
						else
						{
							g_PrintColorStr(vx, CHAT_HISTORY_START_Y-(FONT_GAP*line), p_temp+cut_index, gpC_base->m_chatting_pi, p_line->GetColor());
						}
					}
					line++;
					
					backup_char = p_temp[cut_index];
					p_temp[cut_index] = '\0';
				}
				
				if(line < g_HISTORY_LINE && line >= 0)
				{
					if (p_line->GetIdString())
					{
						int tabvalue=0;
						if(strstr(p_line->GetIdString(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString()) != NULL)
							tabvalue=CLD_MASTER;
						else
							tabvalue=p_line->GetCondition();
						
						vx = g_PrintColorStr(CHAT_LINE_START_X, CHAT_HISTORY_START_Y-(FONT_GAP*line), p_line->GetIdString(), gpC_base->m_user_id_pi, m_color_tab[tabvalue]);
						vx = g_PrintColorStr(vx, CHAT_HISTORY_START_Y-(FONT_GAP*line), g_sz_chat_id_divisor[p_line->GetCondition()], gpC_base->m_chatting_pi, m_color_tab[tabvalue]);
						vx += _ID_GAP;
					}
					
					if (p_line->GetCondition() == CLD_ZONECHAT)
					{
						g_PrintColorStr(vx, CHAT_HISTORY_START_Y-(FONT_GAP*line), p_temp, gpC_base->m_user_id_pi, p_line->GetColor());
					}
					else
					{
						g_PrintColorStr(vx, CHAT_HISTORY_START_Y-(FONT_GAP*line), p_temp, gpC_base->m_chatting_pi, p_line->GetColor());
					}
				}
				if(bl_backup)
				{
					p_temp[cut_index] = backup_char;
				}
			}
		}
	}
	if(gap)
	{
		m_pC_input_button_group->ShowDescription();
		m_pC_input_right_button_group->ShowDescription();
	}
	m_pC_button_group->ShowDescription();
	
	g_FL2_ReleaseDC();
	
	if(m_bl_whisper_mode && (!g_pUserOption->UseEnterChat || m_bl_input_mode))
	{
		if(m_bl_spreadID)
		{
			
			RECT rect = { CHAT_LINE_START_X+15, CHAT_LINE_START_Y - GetWhisperSize()*FONT_GAP -3, CHAT_LINE_START_X+85, CHAT_LINE_START_Y -3 };			
			DrawAlphaBox(&rect, 50>>3, 50>>3, 150>>3, 25);
			g_FL2_GetDC();
			for(int i = 0; i<GetWhisperSize(); i++)
			{
				g_PrintColorStr(CHAT_LINE_START_X+15, CHAT_LINE_START_Y - GetWhisperSize()*FONT_GAP + i*FONT_GAP, GetWhisperID(i).c_str(), gpC_base->m_user_id_pi, gpC_base->m_user_id_pi.text_color);
			}
			g_FL2_ReleaseDC();
			
			//			g_PrintColorStr(CHAT_LINE_START_X +90, CHAT_LINE_START_Y, "v", gpC_base->m_user_id_pi, gpC_base->m_user_id_pi.text_color);
		}// else g_PrintColorStr(CHAT_LINE_START_X +90, CHAT_LINE_START_Y, "^", gpC_base->m_user_id_pi, gpC_base->m_user_id_pi.text_color);
		
		if(m_bl_focus_whisper)
		{
			//if(m_sz_whisper_backup.size() > 0)     ###@@@
			if(!m_sz_whisper_backup.empty())
			{
				
				int len = m_sz_whisper_backup.size();
				int len2 = m_lev_chatting.ReachSizeOfBox()+1;
				
				HDC hdc;
				gpC_fl2_surface->GetDC(&hdc);
				DeleteObject( SelectObject(hdc, gpC_base->m_chatting_pi.hfont) );
				
				
				//
				// set format
				//
				SetBkMode(hdc, gpC_base->m_chatting_pi.bk_mode);
				SetTextColor(hdc, m_color_tab[CLD_NORMAL]);
				SetBkColor(hdc, gpC_base->m_chatting_pi.back_color);
				
				TextOut(hdc, CHAT_LINE_START_X +115, CHAT_LINE_START_Y, m_sz_whisper_backup.c_str(), min(len, len2));
				
				gpC_fl2_surface->ReleaseDC(hdc);
				
				//				g_PrintColorStr(CHAT_LINE_START_X +100, CHAT_LINE_START_Y, m_sz_whisper_backup.c_str(), gpC_base->m_chatting_pi, m_color_tab[CLD_NORMAL]);
			}
		}
		else
		{
			//if(m_sz_whisper_backup.size() > 0) ###@@@
			if(!m_sz_whisper_backup.empty())
				g_PrintColorStr(CHAT_LINE_START_X+15, CHAT_LINE_START_Y, m_sz_whisper_backup.c_str(), gpC_base->m_user_id_pi, gpC_base->m_user_id_pi.text_color);
		}
	}	
	
	//*
#ifndef _LIB
	//	if (gpC_base->m_p_DDSurface_back->Lock())
	//	{
	//		S_SURFACEINFO	surfaceinfo;
	//		SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());
	
	//		Rect rect;
	
	//rect.Set(CHAT_LINE_START_X, CHAT_LINE_START_Y, 100, 15);
	//rectangle(&surfaceinfo, &rect, WHITE);
	
	// -- TEST
	// show scroll button rect.
	//
	//rectangle(&surfaceinfo, SCROLL_UP_X, SCROLL_UP_Y, SCROLL_UP_X+SCROLL_W-1, SCROLL_UP_Y+SCROLL_H-1, 0xFFFF);
	//rectangle(&surfaceinfo, SCROLL_DOWN_X, SCROLL_DOWN_Y, SCROLL_DOWN_X+SCROLL_W-1, SCROLL_DOWN_Y+SCROLL_H-1, 0xFFFF);
	
	//rectangle(&surfaceinfo, SCROLLTAG_X, SCROLLTAG_Y, SCROLLTAG_X+SCROLLTAG_W-1, SCROLLTAG_Y+SCROLLTAG_H-1, 0xFFFF);
	
	//rectangle(&surfaceinfo, SCROLLTRACK_X, SCROLLTRACK_Y, SCROLLTRACK_X+SCROLLTRACK_W-1, SCROLLTRACK_Y+SCROLLTRACK_H-1, 0xFFFF);
	
	//		gpC_base->m_p_DDSurface_back->Unlock();
	//	}
	extern bool gbl_info_show;
	
	if(gbl_info_show)
	{
		char sz_buf[100];
		
		sprintf(sz_buf, "m_pC_history_list = %d", m_pC_history_list.Size());
		g_Print(5, TEXT_LINE(16), sz_buf);
		sprintf(sz_buf, "m_pC_history_temp_list = %d", m_pC_history_temp_list.Size());
		g_Print(5, TEXT_LINE(17), sz_buf);
		
		sprintf(sz_buf, "GetWhisperSize() = %d", GetWhisperSize());
		g_Print(5, TEXT_LINE(20), sz_buf);
		sprintf(sz_buf, "m_whisper_index = %d", m_whisper_index);
		g_Print(5, TEXT_LINE(21), sz_buf);
		sprintf(sz_buf, "g_HISTORYLINE = %d", g_HISTORY_LINE);
		g_Print(5, TEXT_LINE(22), sz_buf);
		
		
	}
#endif
	//*/
}

//-----------------------------------------------------------------------------
// C_VS_UI_CHATTING::ShowButtonDescription
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_CHATTING::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char* m_chatting_button_string[23] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SPECIAL_CHARACTER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SET_LETTER_COLOR].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CHANGE_INPUT_LANGUAGE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_WHISPER_ID].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NORMAL_CHATING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ZONE_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_WHISPER_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_PARTY_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NORMAL_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ZONE_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_WHISPER_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_PARTY_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_TEAM_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_SHOW_NORMAL_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_SHOW_ZONE_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_SHOW_WHISPER_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_SHOW_PARTY_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_SHOW_TEAM_CHATTING].GetString(),
	};
	
	const static char* m_vampire_chatting_button_string[23] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SPECIAL_CHARACTER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SET_LETTER_COLOR].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CHANGE_INPUT_LANGUAGE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_WHISPER_ID].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NORMAL_CHATING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ZONE_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_WHISPER_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_PARTY_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLAN_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NORMAL_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ZONE_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_WHISPER_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_PARTY_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_CLAN_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_SHOW_NORMAL_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_SHOW_ZONE_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_SHOW_WHISPER_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_SHOW_PARTY_CHATTING].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_SHOW_CLAN_CHATTING].GetString(),
	};


	// 귓속말 아이디 대상 버튼.
	if(p_button->GetID() == 5)
	{
		if(!m_bl_whisper_mode) return;
	}
	
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			g_descriptor_manager.Set(DID_INFO, x+w-p_button->x-p_button->w, y+h-p_button->y-p_button->h, (void *)m_chatting_button_string[16],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, x+w-p_button->x-p_button->w, y+h-p_button->y-p_button->h, (void *)m_chatting_button_string[p_button->GetID()],0,0);
	}
	else if(p_button->GetID() == PUSHPIN_ID)
	{
		if(GetAttributes()->autohide)
			g_descriptor_manager.Set(DID_INFO, x+w-p_button->x-p_button->w, y+h-p_button->y-p_button->h, (void *)m_chatting_button_string[17],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, x+w-p_button->x-p_button->w, y+h-p_button->y-p_button->h, (void *)m_chatting_button_string[p_button->GetID()],0,0);
	}
	else if(p_button->GetID() >= FILTER_NORMAL_ID && p_button->GetID() <= FILTER_GUILD_ID && m_chat_filter[p_button->GetID() -FILTER_NORMAL_ID] == true)
	{
		if(gbl_vampire_interface)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, y+h-p_button->y-p_button->h, (void *)m_vampire_chatting_button_string[p_button->GetID()+7],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, y+h-p_button->y-p_button->h, (void *)m_chatting_button_string[p_button->GetID()+7],0,0);
	}
	else
	{
		if(gbl_vampire_interface)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, y+h-p_button->y-p_button->h, (void *)m_vampire_chatting_button_string[p_button->GetID()],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, y+h-p_button->y-p_button->h, (void *)m_chatting_button_string[p_button->GetID()],0,0);
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_CHATTING::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_CHATTING::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-p_button->w, y+h-p_button->y-p_button->h, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-p_button->w, y+h-p_button->y-p_button->h, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	}
	else if(p_button->GetID() == PUSHPIN_ID)
	{
		if(GetAttributes()->autohide)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-p_button->w, y+h-p_button->y-p_button->h, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-p_button->w, y+h-p_button->y-p_button->h, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
	}
	else if(p_button->GetID() == LANGUAGE_ID)
	{
		int image;
		if (gC_ci.IsEngInput())
			image = BUTTON_ENG;
		else
			image = BUTTON_HAN;
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, image+2);
			else
				m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, image+1);
		}
		else
			m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, image);
	}
	else if(p_button->GetID() == MARK_ID || p_button->GetID() == COLOR_ID || p_button->GetID() == SPREAD_ID)
	{
		if(p_button->GetID() == SPREAD_ID && !m_bl_whisper_mode)
			return;
		
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index+2);
			else
				m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index+1);
		}
		else
			m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index);
	}
	else if(p_button->GetID() >= CHAT_NORMAL_ID && p_button->GetID() <= CHAT_GUILD_ID)
	{
		if(p_button->GetID()-CHAT_NORMAL_ID == m_chat_mode || p_button->GetID() == CHAT_WHISPER_ID && m_bl_whisper_mode)
		{
			if(p_button->GetPressState())
				m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index+3);
			else
				m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index+3);
		}
		else
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index+2);
				else
					m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index+1);
			}
			else
				m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index);
	}
	else if(p_button->GetID() >= FILTER_NORMAL_ID && p_button->GetID() <= FILTER_GUILD_ID)
	{
		if(m_chat_filter[p_button->GetID()-FILTER_NORMAL_ID])
		{
			if(p_button->GetPressState())
				m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h+1, p_button->m_image_index+2);
			else
				m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index+2);
		}
		else
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h+1, p_button->m_image_index+1);
				else
					m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index+1);
			}
			else
				m_pC_chatting_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index);
	}		
}

void C_VS_UI_CHATTING::Extend()
{
	Rect temp_rect(x, y, w, h);
	Set(m_backup_rect.x, m_backup_rect.y, m_backup_rect.w, m_backup_rect.h);
	m_backup_rect = temp_rect;
	WindowEventReceiver(EVENT_WINDOW_MOVE);
	if(gbl_vampire_interface)
		PlaySound(SOUND_VAMPIRE_BUTTON);
	else
		PlaySound(SOUND_SLAYER_BUTTON);
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHATTING::WindowEventReceiver(id_t event)
{
	int old_w = w, old_h = h;
	int w_max, h_max;
	if(gbl_vampire_interface)
	{
		w_max = 576;
		h_max = 125;
	}
	else
	{
		w_max = 576;
		h_max = 119;
	}
	
	switch(event)
	{
	case EVENT_WINDOW_MOVE: 
		if(Moving()) //&& (!m_bl_input_mode && m_lev_chatting.Size() == 0) && !GetAttributes()->autohide)
		{
			switch(m_resize)
			{
			case RESIZE_TOP:
				y = max(0, y);
				h += m_backup_window_point.y-y;
				h = max(h_max, min(h, RESOLUTION_Y));
				if(h != old_h)
					m_backup_window_point.y += old_h-h;
				y = m_backup_window_point.y;
				x = m_backup_window_point.x;
				break;
				
			case RESIZE_BOTTOM:
				h += y - m_backup_window_point.y;
				x = m_backup_window_point.x;
				y = m_backup_window_point.y;
				h = max(h_max, min(h, RESOLUTION_Y));
				if(old_h != h)
					SetOrigin(gpC_mouse_pointer->GetX()-x, gpC_mouse_pointer->GetY()-y);
				break;
				
			case RESIZE_LEFT:
				x = max(0, x);
				w += m_backup_window_point.x - x;
				w = max(w_max, min(w, RESOLUTION_X));
				if(w != old_w)
					m_backup_window_point.x += old_w-w;
				y = m_backup_window_point.y;
				x = m_backup_window_point.x;
				break;
				
			case RESIZE_RIGHT:
				w += x - m_backup_window_point.x;
				x = m_backup_window_point.x;
				y = m_backup_window_point.y;
				w = max(w_max, min(w, RESOLUTION_X));
				if(old_w != w)
					SetOrigin(gpC_mouse_pointer->GetX()-x, gpC_mouse_pointer->GetY()-y);
				break;
				
			case RESIZE_LEFTTOP:
				x = max(0, x);
				y = max(0, y);
				h += m_backup_window_point.y - y;
				h = max(h_max, min(h, RESOLUTION_Y));
				if(h != old_h)
					m_backup_window_point.y += old_h-h;
				w += m_backup_window_point.x - x;
				w = max(w_max, min(w, RESOLUTION_X));
				if(w != old_w)
					m_backup_window_point.x += old_w-w;
				y = m_backup_window_point.y;
				x = m_backup_window_point.x;
				break;
				
			case RESIZE_RIGHTTOP:
				w += x - m_backup_window_point.x;
				x = m_backup_window_point.x;
				w = max(w_max, min(w, RESOLUTION_X));
				if(old_w != w)
					SetOrigin(gpC_mouse_pointer->GetX()-x, gpC_mouse_pointer->GetY()-y);
				y = max(0, y);
				h += m_backup_window_point.y - y;
				h = max(h_max, min(h, RESOLUTION_Y));
				if(h != old_h)
					m_backup_window_point.y += old_h-h;
				y = m_backup_window_point.y;
				break;
				
			case RESIZE_LEFTBOTTOM:
				h += y - m_backup_window_point.y;
				y = m_backup_window_point.y;
				h = max(h_max, min(h, RESOLUTION_Y));
				if(old_h != h)
					SetOrigin(gpC_mouse_pointer->GetX()-x, gpC_mouse_pointer->GetY()-y);
				x = max(0, x);
				w += m_backup_window_point.x - x;
				w = max(w_max, min(w, RESOLUTION_X));
				if(w != old_w)
					m_backup_window_point.x += old_w-w;
				x = m_backup_window_point.x;
				break;
				
			case RESIZE_RIGHTBOTTOM:
				y = max(0, y);
				x = max(0, x);
				w += m_backup_window_point.x - x;
				w = max(w_max, min(w, RESOLUTION_X));
				if(w != old_w)
					m_backup_window_point.x += old_w-w;
				h += m_backup_window_point.y - y;
				h = max(h_max, min(h, RESOLUTION_Y));
				if(h != old_h)
					m_backup_window_point.y += old_h-h;
				y = m_backup_window_point.y;
				x = m_backup_window_point.x;
				break;
			}
		}
		CHAT_LINE_START_X = x+40;
		CHAT_LINE_START_Y = y+h-FONT_GAP-3;
		CHAT_WINDOW_WIDTH = w-70;
		CHAT_INPUT_WIDTH = w-110;
		CHAT_HISTORY_START_Y = y+h-FONT_GAP-4-20;

		if(gbl_vampire_interface)
			g_HISTORY_LINE = (h-20-20)/FONT_GAP;
		else
			g_HISTORY_LINE = (h-14-20)/FONT_GAP;
		
		if(m_bl_whisper_mode)
		{
			if(m_bl_focus_whisper)
			{
				m_lev_chatting.SetPosition(CHAT_LINE_START_X+15, CHAT_LINE_START_Y);
				m_lev_chatting.SetByteLimit(10);
			}
			else
			{
				m_lev_chatting.SetPosition(CHAT_LINE_START_X +115, CHAT_LINE_START_Y);
				m_lev_chatting.SetAbsWidth(CHAT_INPUT_WIDTH-100);
			}
		}
		else
		{
			m_lev_chatting.SetPosition(CHAT_LINE_START_X +15, CHAT_LINE_START_Y);
			m_lev_chatting.SetAbsWidth(CHAT_INPUT_WIDTH);
		}
		ResetScroll();
		//			SlayerWhisperMode(m_bl_whisper_mode);
		break;
	}
}


//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHATTING::Run(id_t id)
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
			AttrAutoHide(ATTRIBUTES_HIDE_HEIGHT);
		EMPTY_MOVE;
		break;
		
	case SPREAD_ID:
		m_bl_spreadID = !m_bl_spreadID;
		break;
		
	case CHAT_WHISPER_ID:
		SlayerWhisperMode(!m_bl_whisper_mode);
		break;
		
	case CHAT_NORMAL_ID:
	case CHAT_ZONE_ID:
	case CHAT_PARTY_ID:
	case CHAT_GUILD_ID:
		m_chat_mode = (CHAT_LINE_CONDITION)(id-CHAT_NORMAL_ID);
		SlayerWhisperMode(false);
		break;
		
	case FILTER_NORMAL_ID:
	case FILTER_WHISPER_ID:
	case FILTER_ZONE_ID:
	case FILTER_PARTY_ID:
	case FILTER_GUILD_ID:
		m_chat_filter[id-FILTER_NORMAL_ID] = !m_chat_filter[id-FILTER_NORMAL_ID];
		ResetScroll();
		break;
		
	case MARK_ID:
		if(m_sub_window == 0)
			m_sub_window = 1;
		else
			m_sub_window = 0;
		break;
		
	case COLOR_ID:
		if(m_sub_window == 0)
			m_sub_window = 2;
		else
			m_sub_window = 0;
		break;
		
	case LANGUAGE_ID:
//		gC_ci.SetEngInput(gC_ci.IsEngInput());
		gC_ci.SetEngInput();
		break;
	}
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHATTING::Process()
{
	if(m_v_help_check.empty())
	{
		// 도움말용 파일 읽기
#define dSTRING_LEN 2048 
		
		char szLine[dSTRING_LEN]; 
		
		CRarFile pack_file;
		pack_file.SetRAR(RPK_HELP, RPK_PASSWORD);
		
		bool re = pack_file.Open("commoningame.txt");
		//		assert(re);
		
		while(pack_file.GetString(szLine, dSTRING_LEN))
		{
			if(strlen(szLine) == 0)
				continue;
			
			m_v_help_string.push_back(szLine);
			m_v_help_check.push_back(false);
			//			ZeroMemory(szLine, dSTRING_LEN);
		}
		
		if(gbl_vampire_interface)
			re = pack_file.Open("vampireingame.txt");
		else
			re = pack_file.Open("slayeringame.txt");
		
		while(pack_file.GetString(szLine, dSTRING_LEN))
		{
			if(strlen(szLine) == 0)
				continue;
			
			m_v_help_string.push_back(szLine);
			m_v_help_check.push_back(false);
			//			ZeroMemory(szLine, dSTRING_LEN);
		}		
		pack_file.Release();		
		srand(time(0));
	}
	
	if(Timer())
	{
		SetInputStringColor(RGB_RED);
	}
	else
	{
		SetInputStringColor(RGB_GRAY);
	}
	
	
	m_pC_button_group->Process();
	m_pC_input_button_group->Process();
	m_pC_input_right_button_group->Process();
	
	m_backup_window_point.x = x;
	m_backup_window_point.y = y;
	
	bool bl_hide = false;
	if(!(!m_bl_input_mode && m_lev_chatting.Size() == 0 && TimerHide() == false) && GetAttributes()->autohide)
	{
		AttrAutoHide(ATTRIBUTES_HIDE_NOT);
		bl_hide = true;
	}
	
	bool bl_alpha = GetAttributes()->alpha;
	if(bl_alpha)
	{
		AttrAlpha(false);
	}
	ProcessHide(0, bl_alpha);
	if(bl_alpha)
	{
		AttrAlpha(true);
	}
	
	if(bl_hide)
		AttrAutoHide(ATTRIBUTES_HIDE_HEIGHT);
	
	if(TimerHelp() && !m_v_help_check.empty() && g_pUserOption->ShowChoboHelp)// && (gbl_vampire_interface && g_char_slot_ingame.level < 10 || !gbl_vampire_interface && g_char_slot_ingame.STR_CUR+g_char_slot_ingame.DEX_CUR+g_char_slot_ingame.INT_CUR < 45))
	{
		
		static int CheckNum = 0;
		if(CheckNum == 0)
		{
			for(int i = 0; i < m_v_help_check.size(); i++)
				m_v_help_check[i] = false;
			CheckNum = m_v_help_check.size();
		}
		int num = 0;
		if(CheckNum > 0) num = rand()%CheckNum;
		for(int i = 0; i <= num; i++)
		{
			if(m_v_help_check[i] == true)num++;//break;
		}
		//		m_v_help_check[i] = true;
		if(num < m_v_help_check.size())
		{
			
			m_v_help_check[num] = true;
			CheckNum--;
			
			AddChatToHistory(m_v_help_string[num].c_str(), (*g_pGameStringTable)[UI_STRING_MESSAGE_HELP_MESSAGE].GetString(), CLD_INFO, m_color_tab[CLD_INFO]);
		}
	}
}

/*-----------------------------------------------------------------------------
- C_VS_UI_CHATTING
- Constructor.

  chatting object start.
-----------------------------------------------------------------------------*/
C_VS_UI_CHATTING::C_VS_UI_CHATTING()
{
	
	AttrPin(true);
	
	g_RegisterWindow(this);
	
	//	if(gbl_vampire_interface)
	//	{
	//		m_pC_chatting_spk = new C_SPRITE_PACK(SPK_CHATTING_VAMPIRE);
	//	}
	//	else
	//	{
	//		m_pC_chatting_spk = new C_SPRITE_PACK(SPK_CHATTING_SLAYER);
	//	}
	//	Set(0, RESOLUTION_Y - 119, 623, 119);
	m_pC_chatting_spk = NULL;
	
	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR;
	m_pC_scroll_bar->SetReverse(true);
	TribeChanged();
	
	m_pC_sub_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(135, 35, -1, 30));
	m_pC_sub_scroll_bar->SetPosMax(MARK_MAX - 5+1);
	m_pC_sub_scroll_bar->SetScrollPos(0);
	
	m_pC_button_group = new ButtonGroup(this);
	m_pC_input_button_group = new ButtonGroup(this);
	m_pC_input_right_button_group = new ButtonGroup(this);
	
	m_sub_window = 0;
	m_sub_rect.Set(0, 0, 160, 100);
	m_sub_selected.x = -1;
	
	// alpha & pushpin button
	int alpha_button_offset_x, alpha_button_offset_y;
	int pushpin_button_offset_x, pushpin_button_offset_y;
	
	if(gbl_vampire_interface)
	{
		alpha_button_offset_x = 8; alpha_button_offset_y = 6;
		pushpin_button_offset_x = 24; pushpin_button_offset_y = 6;
	}
	else
	{
		alpha_button_offset_x = 8; alpha_button_offset_y = 7;
		pushpin_button_offset_x = 26; pushpin_button_offset_y = 7;
	}
	
	m_pC_input_right_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_button_offset_x, alpha_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));
	m_pC_input_right_button_group->Add(new C_VS_UI_EVENT_BUTTON(pushpin_button_offset_x, pushpin_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), PUSHPIN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN));
	
	// chat buttons
	int chat_button_offset_x = 7, chat_button_offset_y = 35, chat_button_gap = 12;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(chat_button_offset_x, chat_button_offset_y, m_pC_chatting_spk->GetWidth(BUTTON_GUILD), m_pC_chatting_spk->GetHeight(BUTTON_GUILD), CHAT_GUILD_ID, this, BUTTON_GUILD));
	chat_button_offset_y += chat_button_gap;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(chat_button_offset_x, chat_button_offset_y, m_pC_chatting_spk->GetWidth(BUTTON_PARTY), m_pC_chatting_spk->GetHeight(BUTTON_PARTY), CHAT_PARTY_ID, this, BUTTON_PARTY));
	chat_button_offset_y += chat_button_gap;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(chat_button_offset_x, chat_button_offset_y, m_pC_chatting_spk->GetWidth(BUTTON_WHISPER), m_pC_chatting_spk->GetHeight(BUTTON_WHISPER), CHAT_WHISPER_ID, this, BUTTON_WHISPER));
	chat_button_offset_y += chat_button_gap;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(chat_button_offset_x, chat_button_offset_y, m_pC_chatting_spk->GetWidth(BUTTON_ZONE), m_pC_chatting_spk->GetHeight(BUTTON_ZONE), CHAT_ZONE_ID, this, BUTTON_ZONE));
	chat_button_offset_y += chat_button_gap;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(chat_button_offset_x, chat_button_offset_y, m_pC_chatting_spk->GetWidth(BUTTON_NORMAL), m_pC_chatting_spk->GetHeight(BUTTON_NORMAL), CHAT_NORMAL_ID, this, BUTTON_NORMAL));
	
	// filter buttons
	chat_button_offset_x += 13; chat_button_offset_y = 36;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(chat_button_offset_x, chat_button_offset_y, m_pC_chatting_spk->GetWidth(BUTTON_FILTER), m_pC_chatting_spk->GetHeight(BUTTON_FILTER), FILTER_GUILD_ID, this, BUTTON_FILTER));
	chat_button_offset_y += chat_button_gap;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(chat_button_offset_x, chat_button_offset_y, m_pC_chatting_spk->GetWidth(BUTTON_FILTER), m_pC_chatting_spk->GetHeight(BUTTON_FILTER), FILTER_PARTY_ID, this, BUTTON_FILTER));
	chat_button_offset_y += chat_button_gap;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(chat_button_offset_x, chat_button_offset_y, m_pC_chatting_spk->GetWidth(BUTTON_FILTER), m_pC_chatting_spk->GetHeight(BUTTON_FILTER), FILTER_WHISPER_ID, this, BUTTON_FILTER));
	chat_button_offset_y += chat_button_gap;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(chat_button_offset_x, chat_button_offset_y, m_pC_chatting_spk->GetWidth(BUTTON_FILTER), m_pC_chatting_spk->GetHeight(BUTTON_FILTER), FILTER_ZONE_ID, this, BUTTON_FILTER));
	chat_button_offset_y += chat_button_gap;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(chat_button_offset_x, chat_button_offset_y, m_pC_chatting_spk->GetWidth(BUTTON_FILTER), m_pC_chatting_spk->GetHeight(BUTTON_FILTER), FILTER_NORMAL_ID, this, BUTTON_FILTER));
	
	// input buttons
	int input_button_offset_x = 4, input_button_offset_y = 7, input_button_gap = 15;
	m_pC_input_button_group->Add(new C_VS_UI_EVENT_BUTTON(input_button_offset_x, input_button_offset_y, m_pC_chatting_spk->GetWidth(BUTTON_MARK), m_pC_chatting_spk->GetHeight(BUTTON_MARK), MARK_ID, this, BUTTON_MARK));
	input_button_offset_x += input_button_gap;
	m_pC_input_button_group->Add(new C_VS_UI_EVENT_BUTTON(input_button_offset_x, input_button_offset_y, m_pC_chatting_spk->GetWidth(BUTTON_COLOR), m_pC_chatting_spk->GetHeight(BUTTON_COLOR), COLOR_ID, this, BUTTON_COLOR));
	input_button_offset_x += input_button_gap;
	m_pC_input_button_group->Add(new C_VS_UI_EVENT_BUTTON(input_button_offset_x, input_button_offset_y, m_pC_chatting_spk->GetWidth(BUTTON_ENG), m_pC_chatting_spk->GetHeight(BUTTON_ENG), LANGUAGE_ID, this, BUTTON_HAN));
	// Spread button
	m_pC_input_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+131, 4, m_pC_chatting_spk->GetWidth(BUTTON_SPREAD), m_pC_chatting_spk->GetHeight(BUTTON_SPREAD), SPREAD_ID, this, BUTTON_SPREAD));
	
	CancelPushState();
	UnacquireMouseFocus();
	
	scroll_up_x = scroll_up_y = scroll_down_x = scroll_down_y = scrolltag_x = scrolltag_y = scrolltrack_x = scrolltrack_y = 0;
	
	//m_lev_chatting.SetInputCharCount(CHAT_INPUT_LIMIT);
	m_lev_chatting.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_chatting.SetCursorColor(RGB_WHITE);//RGB(128, 128, 128));
	m_lev_chatting.SetAbsWidth(563);
	m_lev_chatting.SetByteLimit(100);
	
	if(g_pUserOption->ChatWhite)
		m_lev_chatting.SetInputStringColor(gpC_base->m_chatting_pi.text_color);
	else
		m_lev_chatting.SetInputStringColor(g_pUserOption->ChattingColor);
	
	m_bl_focus_whisper = false;	// by larosel
	m_bl_whisper_mode = false;	// by larosel
	m_whisper_index = -1;		// by larosel
	
	//	m_bl_guild = false;		// 길드 채팅 by larosel
	m_bl_spreadID = false;
	
	m_history_line = 0;
	
	//timer
	m_dw_zonechat_timer = 15000;
	m_dw_rep_timer = 20000;
	m_dw_papering_timer = 20000;
	m_dw_help_timer = 60000;
	m_dw_hide_timer = 5000;
	
	m_timer = TIMER_NONE;
	
	m_dw_help_prev_tickcount = GetTickCount()-m_dw_help_timer;
	
	m_resize = RESIZE_NOT;
	
	m_bl_input_mode = false;
	
	for(int i = 0; i < CLD_TOTAL; i++)
		m_chat_filter[i] = true;
	
	m_chat_mode = CLD_NORMAL;	
}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_CHATTING
- Destructor.

  chatting object end.
-----------------------------------------------------------------------------*/
C_VS_UI_CHATTING::~C_VS_UI_CHATTING()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::CHATTING, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::CHATTING, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::CHATTING, Rect(x, y, w, h));
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::CHATTING_OLD, m_backup_rect);
	
	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_scroll_bar);
	DeleteNew(m_pC_sub_scroll_bar);
	DeleteNew(m_pC_chatting_spk);
	DeleteNew(m_pC_button_group);
	DeleteNew(m_pC_input_button_group);
	DeleteNew(m_pC_input_right_button_group);
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_CHATTING::IsPixel(int _x, int _y)
{
	if(Moving()) return true;
	
	bool re = false;
	if(IsSpreadID())
	{
		if(_x > CHAT_LINE_START_X && _y > CHAT_LINE_START_Y - GetWhisperSize()*FONT_GAP -3 && _x < CHAT_LINE_START_X+70 && _y < CHAT_LINE_START_Y -3 )
			re = true;
	}
	if(re == false && GetAttributes()->alpha)
	{
		RECT alpha_rect;
		int gap = 0;
		if(!g_pUserOption->UseEnterChat || m_bl_input_mode == true)
			gap = m_pC_chatting_spk->GetHeight(INPUT_RIGHT);
		
		if(gbl_vampire_interface)
			SetRect( &alpha_rect, x+30, y+10, x+w-33, y+h-5-gap );
		else
			SetRect( &alpha_rect, x+35, y+5, x+w-26, y+h-5-gap );
		
		if( _x >= alpha_rect.left && _x <= alpha_rect.right &&
			_y >= alpha_rect.top && _y <= alpha_rect.bottom )
			return false;
	}
	
	if(re == false)
		re = IsInRect(_x, _y);
	if(re == false && m_sub_window != 0)
		re = m_sub_rect.IsInRect(_x, _y);
	
	return re;
}


//-----------------------------------------------------------------------------
// C_VS_UI_CHATTING::TribeChanged
//
// 종족이 바뀌었을 때 실행할것.
//-----------------------------------------------------------------------------
void	C_VS_UI_CHATTING::TribeChanged()
{
	if(m_pC_chatting_spk != NULL)
	{
		DeleteNew(m_pC_chatting_spk);
		m_pC_chatting_spk = NULL;
	}
	
	m_v_help_check.clear();
	m_v_help_string.clear();
	
	if (gbl_vampire_interface)
	{
		m_pC_chatting_spk = new C_SPRITE_PACK(SPK_CHATTING_VAMPIRE);
		Set(0, RESOLUTION_Y - 125, 624, 125);
		m_backup_rect.Set(0, RESOLUTION_Y - 125-100, 624, 125+100);
		gpC_base->m_user_id_pi.text_color = VAMPIRE_FONT_COLOR;
		gpC_base->m_chatting_pi.text_color = VAMPIRE_FONT_COLOR;
		m_color_tab[CLD_NORMAL] = VAMPIRE_FONT_COLOR;
		m_color_tab[CLD_GUILD] = RGB(180, 230, 230);
		m_color_tab[CLD_PARTY] = RGB(230, 180, 230);
		m_color_tab[CLD_ZONECHAT] = VAMPIRE_FONT_COLOR;
		m_color_tab[CLD_WHISPER] = RGB(65, 206, 81);//RGB_GREEN;
		m_color_tab[CLD_INFO] = RGB(130, 230, 230);
		m_color_tab[CLD_MASTER] = RGB(255,180, 180);
	}
	else
	{
		// add by sonic 2006.9.26
		if (g_MyFull)
			m_pC_chatting_spk = new C_SPRITE_PACK(SPK_CHATTING_SLAYER_1024);
		else
			m_pC_chatting_spk = new C_SPRITE_PACK(SPK_CHATTING_SLAYER);
		// End by Snic
		Set(0, RESOLUTION_Y - 119, 623, 119);
		m_backup_rect.Set(0, RESOLUTION_Y - 119-100, 623, 119+100);
		//gpC_base->m_chatting_pi.bk_mode = OPAQUE;
		//gpC_base->m_chatting_pi.back_color = RGB_WHITE;
		
		gpC_base->m_user_id_pi.text_color = SLAYER_FONT_COLOR;
		gpC_base->m_chatting_pi.text_color = SLAYER_FONT_COLOR;
		m_color_tab[CLD_NORMAL] = SLAYER_FONT_COLOR;
		m_color_tab[CLD_GUILD] = RGB(180, 230, 180);
		m_color_tab[CLD_PARTY] = RGB(230, 180, 230);
		m_color_tab[CLD_ZONECHAT] = SLAYER_FONT_COLOR;
		m_color_tab[CLD_WHISPER] = RGB(65, 206, 81);//RGB_BLUE;	// by larosel
		m_color_tab[CLD_INFO] = RGB(130, 230, 230);
		m_color_tab[CLD_MASTER] = RGB(255,180, 180);
	}
	
	m_lev_chatting.SetPrintInfo(gpC_base->m_chatting_pi);
}

//-----------------------------------------------------------------------------
// C_VS_UI_CHATTING::SlayerChatMode
//
// chatting mode를 바꾼다.
//-----------------------------------------------------------------------------
bool	C_VS_UI_CHATTING::SlayerWhisperMode(bool mode)
{
	
	m_bl_whisper_mode = mode;
	m_bl_focus_whisper = false;
	
	
	if(mode)
	{
		m_lev_chatting.SetPosition(CHAT_LINE_START_X +115, CHAT_LINE_START_Y);
		m_lev_chatting.EraseAll();
		m_lev_chatting.SetByteLimit(100);
		m_lev_chatting.SetAbsWidth(CHAT_INPUT_WIDTH-100);
		
		if(GetWhisperSize())
		{
			m_sz_whisper_backup = m_sz_whisper_id.back();
			m_whisper_index = GetWhisperSize() -1;
		}
		else ChangeWhisperFocus();
	}
	else
	{
		m_lev_chatting.SetPosition(CHAT_LINE_START_X+15, CHAT_LINE_START_Y);
		m_lev_chatting.EraseAll();
		m_lev_chatting.SetByteLimit(100);
		m_lev_chatting.SetAbsWidth(CHAT_INPUT_WIDTH);
		m_whisper_index = -1;
	}
	
	m_lev_chatting.Acquire();
	
	return mode;
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHATTING::Start()
{
	PI_Processor::Start();
	
	gpC_window_manager->AppearWindow(this);
	
	m_pC_button_group->Init();
	m_pC_input_button_group->Init();
	m_pC_input_right_button_group->Init();
	
	SlayerWhisperMode(false);
	
	m_bl_spreadID = false;
	m_dw_hide_prev_tickcount = GetTickCount() - m_dw_hide_timer;
	
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::CHATTING));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::CHATTING));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::CHATTING);

	if(rect.w != -1)	
		Set(rect.x, rect.y, rect.w, rect.h);
	
	Rect &rect2 = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::CHATTING_OLD);
	if(rect2.w != -1)
		m_backup_rect.Set(rect2.x, rect2.y, rect2.w, rect2.h);
	
	m_lev_chatting.Acquire();
	WindowEventReceiver(EVENT_WINDOW_MOVE);
	
}

void C_VS_UI_CHATTING::Finish()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::CHATTING, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::CHATTING, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::CHATTING, Rect(x, y, w, h));
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::CHATTING_OLD, m_backup_rect);
	
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
	
}


//-----------------------------------------------------------------------------
// GetWhisperID
// 귓속말 아이디를 리턴한다.
// 디폴트는 가장 최근의 아이디
//-----------------------------------------------------------------------------
std::string C_VS_UI_CHATTING::GetWhisperID(int num)
{
	if(!m_sz_whisper_id.size())
	{
		std::string temp="";
		return temp;
	}
	
	if(num == -1)
	{
		if(m_whisper_index == -1)return m_sz_whisper_id.back();
		return m_sz_whisper_id[m_whisper_index];
	}
	
	return m_sz_whisper_id[num];
	
}

//-----------------------------------------------------------------------------
// ChangeWhisperFocus
// 귓말에서 아이디<->내용 사이의 포커스 이동
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHATTING::ChangeWhisperFocus()
{
	char * sz_chat_str = NULL;
	
	std::string temp;
	
	//	strcpy(temp, m_sz_whisper_backup);
	g_Convert_DBCS_Ascii2SingleByte(m_lev_chatting.GetString(), m_lev_chatting.Size(), sz_chat_str);
	if(sz_chat_str)
	{
		temp = sz_chat_str;
		delete [] sz_chat_str;
	}
	else temp = "";
	
	m_lev_chatting.EraseAll();
	temp.swap(m_sz_whisper_backup);
	m_lev_chatting.AddString(temp.c_str());
	
	m_bl_focus_whisper = !m_bl_focus_whisper;
	
	if(m_bl_focus_whisper)
	{
		m_lev_chatting.SetPosition(CHAT_LINE_START_X+15, CHAT_LINE_START_Y);
		m_lev_chatting.SetByteLimit(10);
		m_lev_chatting.EraseAll();
	}
	else
	{
		//		if(m_sz_whisper_backup == "")m_sz_whisper_backup = GetWhisperID();
		m_lev_chatting.SetPosition(CHAT_LINE_START_X +115, CHAT_LINE_START_Y);
		m_lev_chatting.SetByteLimit(100);
		m_lev_chatting.HomeCursor();
		m_lev_chatting.EndCursor();
	}
	
	
	m_lev_chatting.Acquire();
	
}

//-----------------------------------------------------------------------------
// AddWhisperID
// 귓속말 아이디를 등록한다.
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_CHATTING::AddWhisperID(const char *sz_ID)
{
	int i;
	
	if(sz_ID[0] == ' ' || sz_ID[0] == '\0' || sz_ID[0] == '[')return false;
	
	if(GetWhisperSize() == WHISPER_MAX)
		m_sz_whisper_id.erase(m_sz_whisper_id.begin());
	
	char szTemp[512];
	
	strcpy(szTemp, sz_ID);
	
	for(i = 0; i < 11 && szTemp[i] != ' ' && szTemp[i] != '\0'; i++);
	szTemp[i] = '\0';
	
	
	std::string temp = szTemp;
	
	for(i = 0; i < GetWhisperSize(); i++)
	{
		if(m_sz_whisper_id[i] == temp)
		{
			m_sz_whisper_id.erase(&m_sz_whisper_id[i]);
			break;
		}
	}
		
	m_sz_whisper_id.push_back(temp);
		
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_CHATTING::Timer
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_CHATTING::Timer(bool reset)
{
	if(reset)
	{
		m_dw_prev_tickcount = GetTickCount();
	}
	else 
	if(m_dw_prev_tickcount+m_dw_zonechat_timer >= GetTickCount() && m_timer == TIMER_ZONECHAT && m_chat_mode == CLD_ZONECHAT && !m_bl_whisper_mode ||
		m_dw_prev_tickcount+m_dw_rep_timer >= GetTickCount() && m_timer == TIMER_REP  ||
		m_dw_prev_tickcount+m_dw_papering_timer >= GetTickCount() && m_timer == TIMER_PAPERING)
	{
		return true;
	}
	
	if(!(m_dw_prev_tickcount+m_dw_zonechat_timer >= GetTickCount() && m_timer == TIMER_ZONECHAT))
		m_timer = TIMER_NONE;
	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_CHATTING::TimerHelp
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_CHATTING::TimerHelp()
{
	if(m_dw_help_prev_tickcount+m_dw_help_timer < GetTickCount())
	{
		m_dw_help_prev_tickcount = GetTickCount();
		return true;
	}
	
	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_CHATTING::TimerHide
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_CHATTING::TimerHide(bool reset)
{
	if(reset)
	{
		m_dw_hide_prev_tickcount = GetTickCount();
	}
	else
	if(m_dw_hide_prev_tickcount+m_dw_hide_timer < GetTickCount())
	{
		//		m_dw_hide_prev_tickcount = GetTickCount();
		return false;
	}		
	return true;
}



int	C_VS_UI_INVENTORY::m_mine_grid_x = -1, C_VS_UI_INVENTORY::m_mine_grid_y = -1;
C_SPRITE_PACK *	C_VS_UI_INVENTORY::m_pC_mine_progress_spk = NULL;
DWORD	C_VS_UI_INVENTORY::m_dw_millisec;

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::C_VS_UI_INVENTORY
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_INVENTORY::C_VS_UI_INVENTORY()
{
	g_RegisterWindow(this);
	
	int	desc_button_offset_x, desc_button_offset_y;
	int	close_button_offset_x, close_button_offset_y;
	int	help_button_offset_x, help_button_offset_y;
	int alpha_button_offset_x, alpha_button_offset_y;
	
	m_pC_dialog_drop_money = NULL;
	
	m_bl_shift = false;
	m_bl_set_load = false;
	
	m_pC_button_group = new ButtonGroup(this);
	m_pC_mine_progress_spk = NULL;
	
	if(gbl_vampire_interface)
	{
		m_pC_inventory_spk = new C_SPRITE_PACK(SPK_VAMPIRE_INVENTORY);
		
		m_grid_start_offset_x = 17;
		m_grid_start_offset_y = 19;
		m_money_button_offset_x = 90;
		m_money_button_offset_y = 204;
		desc_button_offset_x = 65;
		desc_button_offset_y = 204;
		close_button_offset_x = 294;
		close_button_offset_y = 204;
		help_button_offset_x = 40;
		help_button_offset_y = 204;
		alpha_button_offset_x = 20;
		alpha_button_offset_y = 204;
		
		Set(10, 128, m_pC_inventory_spk->GetWidth(INVENTORY_WINDOW), m_pC_inventory_spk->GetHeight(INVENTORY_WINDOW)+m_pC_inventory_spk->GetHeight(INVENTORY_WINDOW_BOTTOM)-25);
	}
	else
	{
		m_pC_inventory_spk = new C_SPRITE_PACK(SPK_SLAYER_INVENTORY);
		m_pC_mine_progress_spk = new C_SPRITE_PACK(SPK_MINE_PROGRESS);
		
		m_grid_start_offset_x = 13;
		m_grid_start_offset_y = 25;
		m_money_button_offset_x = 90;
		m_money_button_offset_y = 204+10;
		desc_button_offset_x = 65;
		desc_button_offset_y = 204+10;
		close_button_offset_x = 294;
		close_button_offset_y = 204+10;
		help_button_offset_x = 40;
		help_button_offset_y = 204+10;
		alpha_button_offset_x = 20;
		alpha_button_offset_y = 204+10;
		
		Set(10, 128, m_pC_inventory_spk->GetWidth(INVENTORY_WINDOW), m_pC_inventory_spk->GetHeight(INVENTORY_WINDOW));
	}
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_money_button_offset_x, m_money_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY), MONEY_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_MONEY));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(desc_button_offset_x, desc_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_DESC), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_DESC), DESC_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_DESC));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(help_button_offset_x, help_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION), HELP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_offset_x, close_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_X), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_X), CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_X));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_button_offset_x, alpha_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));
	
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::~C_VS_UI_INVENTORY
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_INVENTORY::~C_VS_UI_INVENTORY()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::INVENTORY, GetAttributes()->alpha);
	
	if(m_bl_set_load)
	{
		gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::INVENTORY, Rect(x, y, w, h));
	}
	
	g_UnregisterWindow(this);
	
	if(m_pC_mine_progress_spk != NULL)
		DeleteNew(m_pC_mine_progress_spk);
	if(m_pC_button_group != NULL)
		DeleteNew(m_pC_button_group);
	if(m_pC_inventory_spk != NULL)
		DeleteNew(m_pC_inventory_spk);
	//	DeleteNew(m_pC_backcolor_spk);
	//	DeleteNew(m_pC_etc_button_spk);
	
	if(m_pC_dialog_drop_money != NULL)
		DeleteNew(m_pC_dialog_drop_money);
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY::WindowEventReceiver(id_t event)
{
	switch (event)
	{
	case EVENT_WINDOW_MOVE:
		ResetRect();
		break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY::Start(bool bl_set_load)
{
	m_bl_set_load = bl_set_load;
	
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::INVENTORY));
	
	if(bl_set_load)	
	{
		Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::INVENTORY);
		if(rect.w != -1)
		{
			x = rect.x;
			y = rect.y;
		}
	}
	
	PI_Processor::Start();
	
	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
	
	// init
	ResetRect();
	m_focus_grid_x = NOT_SELECTED;
	m_focus_grid_y = NOT_SELECTED;
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY::Finish()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::INVENTORY, GetAttributes()->alpha);
	
	if(m_bl_set_load)
	{
		gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::INVENTORY, Rect(x, y, w, h));
	}
	
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
	if(gpC_mouse_pointer->IsCursorDescription())gpC_mouse_pointer->CursorDescriptionToggle();
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_INVENTORY::IsPixel(int _x, int _y)
{
	if(Moving()) return true;
	
	return AnyMatchWindowPixel(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_INVENTORY::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char* m_inventory_button_string[7] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_THROW_MONEY].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ITEM_DESCRIPTION].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP_INVENTORY_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_INVENTORY_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DEPOSIT_MONEY].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),		
	};
	if(gC_vs_ui.IsRunningStorage() && p_button->GetID() == MONEY_ID)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_inventory_button_string[5],0,0);
	else if(GetAttributes()->alpha && p_button->GetID() == ALPHA_ID)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_inventory_button_string[6],0,0);
	else
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_inventory_button_string[p_button->GetID()],0,0);
}


//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_INVENTORY::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{		
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	}
	else
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
		}
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index);
	}
	
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY::Run(id_t id)
{
	switch (id)
	{
	case DESC_ID:
		gpC_mouse_pointer->CursorDescriptionToggle();
		break;
		
	case MONEY_ID:
		// money button이 눌렸다.
		// 돈을 버린다.
		if(m_pC_dialog_drop_money)
			DeleteNew(m_pC_dialog_drop_money);
		
		if (g_pMoneyManager->GetMoney() > 0)
		{
			if (gC_vs_ui.IsRunningStorage())
			{
				m_pC_dialog_drop_money = new C_VS_UI_MONEY_DIALOG(x+m_money_button_offset_x, y+m_money_button_offset_y+gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY), 2, 0, ExecF_DropMoney, DIALOG_OK|DIALOG_CANCEL, 10, 
					C_VS_UI_MONEY_DIALOG::MONEY_DEPOSIT);	// by sigi
			}
			//else if (gC_vs_ui.IsRunningExchange())
			//{
			//	m_pC_dialog_drop_money = new C_VS_UI_MONEY_DIALOG(m_money_button_rect.x, m_money_button_rect.Down(), 1, 0, ExecF_DropMoney, DIALOG_OK|DIALOG_CANCEL, 10, 
			//																	C_VS_UI_MONEY_DIALOG::MONEY_EXCHANGE);
			//}
			else
			{
				m_pC_dialog_drop_money = new C_VS_UI_MONEY_DIALOG(x+m_money_button_offset_x, y+m_money_button_offset_y+gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY), 1, 0, ExecF_DropMoney, DIALOG_OK|DIALOG_CANCEL, 10, 
					C_VS_UI_MONEY_DIALOG::MONEY_DROP);	// by sigi
			}
			
			m_pC_dialog_drop_money->Start();
		}
		break;
		
	case CLOSE_ID:
		// close button이 눌렸다.
		
		if (gbl_sell_running)
		{
			gpC_base->SendMessage(UI_ITEM_SELL_FINISHED);
		}
		else if (gbl_buy_running)
		{
			gpC_base->SendMessage(UI_CLOSE_SHOP);
		}
		else if (gbl_repair_running)
		{
			gpC_base->SendMessage(UI_ITEM_REPAIR_FINISHED);
		}
		else if (gbl_silvering_running)
		{
			gpC_base->SendMessage(UI_ITEM_SILVERING_FINISHED);
		}
		else if (gC_vs_ui.IsRunningStorage())
		{
			gpC_base->SendMessage(UI_CLOSE_STORAGE);
		}
		//else if (gC_vs_ui.IsRunningExchange())
		//{
		//	gpC_base->SendMessage(UI_CLOSE_EXCHANGE);
		//}
		else
		{
			gC_vs_ui.HotKey_Inventory();
		}
		break;
		
	case HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::INVENTORY);
		break;
		
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::AnyMatchWindowPixel
//
// Window image의 pixel이 (x, y) 한 점이라도 일치하면 true를 아니면 false를 반환한다.
//
// Item을 들고 있을 때에는 'item 참조포인트'에 따라 진행한다.
//-----------------------------------------------------------------------------
bool C_VS_UI_INVENTORY::AnyMatchWindowPixel(int _x, int _y) const
{
	if (gpC_mouse_pointer->GetPickUpItem())
	{
		for (int i=0; i < ITEM_REF_POINT_COUNT; i++)
		{
			int px = g_item_ref_point[i].x+gpC_mouse_pointer->GetPointerX();
			int py = g_item_ref_point[i].y+gpC_mouse_pointer->GetPointerY();
			if (m_pC_inventory_spk->IsPixel(px-x, py-y))
				return true;
		}
		
		return false;
	}
	else
	{
		return m_pC_inventory_spk->IsPixel(_x-x, _y-y);
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::AllMatchWindowPixel
//
//
//-----------------------------------------------------------------------------
bool C_VS_UI_INVENTORY::AllMatchWindowPixel(int _x, int _y) const
{
	if (gpC_mouse_pointer->GetPickUpItem())
	{
		for (int i=0; i < ITEM_REF_POINT_COUNT; i++)
		{
			int px = g_item_ref_point[i].x+gpC_mouse_pointer->GetPointerX();
			int py = g_item_ref_point[i].y+gpC_mouse_pointer->GetPointerY();
			if (m_pC_inventory_spk->IsPixel(px-x, py-y) == false)
				return false;
		}
		
		return true;
	}
	else
	{
		return m_pC_inventory_spk->IsPixel(_x-x, _y-y);
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::TestGridRect
//
// Grid rect 위에 있으면 true, 아니면 false를 반환한다.
//-----------------------------------------------------------------------------
bool C_VS_UI_INVENTORY::TestGridRect(int _x, int _y) const
{
	return (m_grid_rect.IsInRect(_x, _y)// || 
		//			  m_money_button_rect.IsInRect(_x, _y) ||
		//			  m_desc_button_rect.IsInRect(_x, _y) ||
		//			  m_bike_button_rect.IsInRect(_x, _y) ||
		//			  m_close_button_rect.IsInRect(_x, _y) ||
		//			  m_help_button_rect.IsInRect(_x, _y)
		);
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::CancelPushState
//
// 
//-----------------------------------------------------------------------------
//void	C_VS_UI_INVENTORY::CancelPushState()
//{
//	m_bl_money_button_pushed = false;
//	m_bl_desc_button_pushed = false;
//	m_bl_bike_button_pushed = false;
//	m_bl_close_button_pushed = false;
//	m_bl_help_button_pushed = false;
//}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::AcquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_INVENTORY::AcquireMouseFocus()
{
	gpC_base->SendMessage(UI_REMOVE_BACKGROUND_MOUSE_FOCUS);
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::AcquireDisappear
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY::AcquireDisappear()
{
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::UnacquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
	m_focus_grid_x = NOT_SELECTED;
	m_focus_grid_y = NOT_SELECTED;
	
	//	m_bl_money_button_focused = false;
	//	m_bl_desc_button_focused = false;
	//	m_bl_bike_button_focused = false;
	//	m_bl_close_button_focused = false;
	//	m_bl_help_button_focused = false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::GetFocusedItemGridX
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_INVENTORY::GetFocusedItemGridX(const MItem * p_item)
{
	assert(p_item);
	
	if (p_item == NULL)
		return 0;
	
	return m_grid_rect.x+GRID_UNIT_PIXEL_X*p_item->GetGridX();
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::GetFocusedItemGridY
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_INVENTORY::GetFocusedItemGridY(const MItem * p_item)
{
	assert(p_item);
	
	if (p_item == NULL)
		return 0;
	
	return m_grid_rect.y+GRID_UNIT_PIXEL_Y*p_item->GetGridY();
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::GetFocusedItemGridH
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_INVENTORY::GetFocusedItemGridH(const MItem * p_item)
{
	assert(p_item);
	
	if (p_item == NULL)
		return 0;
	
	return GRID_UNIT_PIXEL_Y*p_item->GetGridHeight();
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY::Show()
{
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		//		if(gbl_vampire_interface)
		{
			Rect rect;
			rect.Set(0, 0, w, h-m_pC_inventory_spk->GetHeight(INVENTORY_WINDOW_BOTTOM));
			
			if(GetAttributes()->alpha)
			{
				//슬레이어 인터페이스 알파는 창 구석부분 알파처리-_-;
				if(gbl_vampire_interface == false)
				{
					RECT alpha_rect;
					
					// 위
					SetRect(&alpha_rect, x+10, y+12, x+315, y+21);
					DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
					
					// 아래
					SetRect(&alpha_rect, x+5, y+209, x+321, y+245);
					DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
					
					// 왼쪽
					SetRect(&alpha_rect, x+5, y+24, x+9, y+245);
					DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
					
					// 오른쪽
					SetRect(&alpha_rect, x+317, y+24, x+321, y+245);
					DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
					m_pC_inventory_spk->BltLocked(x, y, INVENTORY_WINDOW_ALPHA);
					
				}
				else
					m_pC_inventory_spk->BltLockedClip(x, y, rect, INVENTORY_WINDOW_ALPHA);
			}
			else
			{
				if(gbl_vampire_interface)
					m_pC_inventory_spk->BltLockedClip(x, y, rect, INVENTORY_WINDOW);
				else
					m_pC_inventory_spk->BltLocked(x, y, INVENTORY_WINDOW);
			}
			
			if(gbl_vampire_interface)
				// 뱀파이어 인터페이스는 하단 합체
				m_pC_inventory_spk->BltLocked(x, y+rect.h, INVENTORY_WINDOW_BOTTOM);
		}
		//		else
		//		{
		//			const int grid_back_x = -4, grid_back_y = -4;
		//			if(GetAttributes()->alpha)
		//			{
		//				m_pC_inventory_spk->BltLocked(x+m_grid_start_offset_x+grid_back_x, y+m_grid_start_offset_y+grid_back_y, INVENTORY_WINDOW_ALPHA);
		//			}
		//			else
		//				m_pC_inventory_spk->BltLocked(x+m_grid_start_offset_x+grid_back_x, y+m_grid_start_offset_y+grid_back_y, INVENTORY_WINDOW);
		//		}
		
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+m_money_button_offset_x+25, y+m_money_button_offset_y, C_GLOBAL_RESOURCE::AB_MONEY_BAR);
		
		// show Item
		g_pInventory->SetBegin();
		
		const MItem * p_selected_item = g_pInventory->GetItem(m_focus_grid_x, m_focus_grid_y);
		
		bool bl_alpha[10][6];	// 커헉-_- 하드코딩 아이템 놓인곳은 다르게 표현해야 하니까.. 남은부분은 따로 처리
		ZeroMemory(bl_alpha, 10*6*sizeof(bool));
		
		while (g_pInventory->IsNotEnd())
		{
			const MItem * p_item = g_pInventory->Get();
			
			// p_item은 NULL이 반드시 아니다. 왜냐하면 존재하는 것만 Get()하기 때문이다.
			assert(p_item);
			
			// frame id -> sprite id
			if(p_item)
			{
				TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
				
				int item_x = x+GetFocusedItemGridX(p_item);
				int item_y = y+GetFocusedItemGridY(p_item);
				
				int alpha_r, alpha_g, alpha_b;
				if(gbl_vampire_interface)
				{
					alpha_r = 7;
					alpha_g = 5;
					alpha_b = 5;
				}
				else
				{
					alpha_r = 5;
					alpha_g = 7;
					alpha_b = 7;
				}
				
				// Item이 놓여있는 영역 표시
				for (int j = 0; j < p_item->GetGridHeight(); j++)
				{
					for (int i = 0; i < p_item->GetGridWidth(); i++)
					{
						if(GetAttributes()->alpha)
						{
							RECT alpha_rect;
							alpha_rect.left = item_x+GRID_UNIT_PIXEL_X*i+1;
							alpha_rect.top = item_y+GRID_UNIT_PIXEL_Y*j+1;
							alpha_rect.right = alpha_rect.left+28;
							alpha_rect.bottom = alpha_rect.top+28;
							//							gpC_base->m_p_DDSurface_back->Unlock();
							DrawAlphaBox(&alpha_rect, alpha_r, alpha_g, alpha_b, g_pUserOption->ALPHA_DEPTH);
							//							gpC_base->m_p_DDSurface_back->Lock();
							
							bl_alpha[p_item->GetGridX()+i][p_item->GetGridY()+j] = true;
						}
						else
						{
							m_pC_inventory_spk->BltLocked(item_x+GRID_UNIT_PIXEL_X*i, 
								item_y+GRID_UNIT_PIXEL_Y*j,
								INVENTORY_ITEMBACK);
						}
					}
				}
					
				item_x += (p_item->GetGridWidth()*GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(frame_id)/2;
				item_y += (p_item->GetGridHeight()*GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(frame_id)/2;
				
				// 크리스마스 트리용 하드코딩
				if(p_item->GetItemClass() == MItem::ITEM_CLASS_EVENT_TREE && p_item->GetItemType() != 12)
				{
					bool bTreeItem = false;
					
					MItem *pTreeItem = NULL;
				
					// 위쪽
					pTreeItem = g_pInventory->GetItem(p_item->GetGridX(), p_item->GetGridY()-1);
					if(pTreeItem != NULL &&
						pTreeItem->GetItemClass() == MItem::ITEM_CLASS_EVENT_TREE &&
						pTreeItem->GetItemType() == p_item->GetItemType()-3)
						bTreeItem = true;
					
					// 아래쪽
					pTreeItem = g_pInventory->GetItem(p_item->GetGridX(), p_item->GetGridY()+1);
					if(pTreeItem != NULL &&
						pTreeItem->GetItemClass() == MItem::ITEM_CLASS_EVENT_TREE &&
						pTreeItem->GetItemType() == p_item->GetItemType()+3)
						bTreeItem = true;
					
					// 왼쪽
					pTreeItem = g_pInventory->GetItem(p_item->GetGridX()-1, p_item->GetGridY());
					if(pTreeItem != NULL &&
						pTreeItem->GetItemClass() == MItem::ITEM_CLASS_EVENT_TREE &&
						pTreeItem->GetItemType() == p_item->GetItemType()-1 &&
						p_item->GetItemType()%3 != 0)
						bTreeItem = true;
					
					// 오른쪽
					pTreeItem = g_pInventory->GetItem(p_item->GetGridX()+1, p_item->GetGridY());
					if(pTreeItem != NULL &&
						pTreeItem->GetItemClass() == MItem::ITEM_CLASS_EVENT_TREE &&
						pTreeItem->GetItemType() == p_item->GetItemType()+1 &&
						p_item->GetItemType()%3 != 2)
						bTreeItem = true;

					if (p_selected_item && (p_selected_item->GetID() == p_item->GetID()))
					{
						gpC_item->BltLockedOutlineOnly(item_x, item_y, WHITE, frame_id);
					}
					
					if(bTreeItem)
					{
						gpC_item->BltLocked(item_x, item_y, frame_id);
					}
					else
					{
						gpC_item->BltLockedDarkness(item_x, item_y, frame_id, 1);
					}
					
				}
				else
				if (p_selected_item && (p_selected_item->GetID() == p_item->GetID()))
				{
//					if(p_item->IsQuestItem())
//						CIndexSprite::SetUsingColorSet(p_item->GetQuestItemColorset(), 0);
//					else
//					if(p_item->IsUniqueItem())
//						CIndexSprite::SetUsingColorSet(p_item->GetUniqueItemColorset(), 0);
					if(p_item->IsSpecialColorItem() )
						CIndexSprite::SetUsingColorSet(p_item->GetSpecialColorItemColorset(), 0);
					else			
						CIndexSprite::SetUsingColorSet(p_item->GetItemOptionColorSet(), 0);
					gpC_item->BltLockedOutline(item_x, item_y, WHITE, frame_id);
				}
				else
				{
//					if(p_item->IsQuestItem())
//						CIndexSprite::SetUsingColorSet(p_item->GetQuestItemColorset(), 0);
//					else
//					if(p_item->IsUniqueItem())
//						CIndexSprite::SetUsingColorSet(p_item->GetUniqueItemColorset(), 0);
					if(p_item->IsSpecialColorItem() )
						CIndexSprite::SetUsingColorSet(p_item->GetSpecialColorItemColorset(), 0);
					else			
						CIndexSprite::SetUsingColorSet(p_item->GetItemOptionColorSet(), 0);
					
					if (p_item->IsAffectStatus() || p_item->IsQuestItem() )
					{
						// frame id -> sprite id
						gpC_item->BltLocked(item_x, item_y, frame_id);
					}
					else
					{
						gpC_item->BltLockedColorSet(item_x, item_y, frame_id, ITEM_DISABLE_COLOR_SET);
					}
				}
			}
			g_pInventory->Next();
		}
		
		if(GetAttributes()->alpha)
		{
			int alpha_r, alpha_g, alpha_b;
			if(gbl_vampire_interface)
			{
				alpha_r = 2;
				alpha_g = 0;
				alpha_b = 0;
			}
			else
			{
				alpha_r = 0;
				alpha_g = 2;
				alpha_b = 2;
			}
			for(int alpha_x = 0; alpha_x < 10; alpha_x++)
			{
				for(int alpha_y = 0; alpha_y < 6; alpha_y++)
				{
					if(bl_alpha[alpha_x][alpha_y] == false)
					{
						RECT alpha_rect;
						alpha_rect.left = x+m_grid_rect.x+alpha_x*GRID_UNIT_PIXEL_X+1;
						alpha_rect.top = y+m_grid_rect.y+alpha_y*GRID_UNIT_PIXEL_Y+1;
						alpha_rect.right = alpha_rect.left+28;
						alpha_rect.bottom = alpha_rect.top+28;
						
						DrawAlphaBox(&alpha_rect, alpha_r, alpha_g, alpha_b, g_pUserOption->ALPHA_DEPTH);
					}
				}
			}
		}
		
		//
		// Item을 들고 있으면 grid 위치를 미리 알 수 있도록 한다.
		//
		if (gpC_mouse_pointer->GetPickUpItem() && 
			m_focus_grid_x != NOT_SELECTED && 
			m_focus_grid_y != NOT_SELECTED)
		{
			MItem * p_item = gpC_mouse_pointer->GetPickUpItem();
			
			TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
			int item_x = x+m_grid_rect.x+GRID_UNIT_PIXEL_X*m_focus_grid_x+(p_item->GetGridWidth()*GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(frame_id)/2;
			int item_y = y+m_grid_rect.y+GRID_UNIT_PIXEL_Y*m_focus_grid_y+(p_item->GetGridHeight()*GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(frame_id)/2;
			
			MItem * p_old_item;
			if (g_pInventory->CanReplaceItem(p_item, m_focus_grid_x, m_focus_grid_y, p_old_item))
			{
				if (p_old_item)
					gpC_item->BltLockedOutlineOnly(item_x, item_y, ga_item_blink_color_table[g_blink_value], frame_id);
				else
					
					gpC_item->BltLockedOutlineOnly(item_x, item_y, POSSIBLE_COLOR, frame_id);
			}
			else
			{
				gpC_item->BltLockedOutlineOnly(item_x, item_y, IMPOSSIBLE_COLOR, frame_id);
			}
		}
		
		//지뢰 설치 Progress Bar
		if(gbl_mine_progress)
		{
			const MItem * p_item = g_pInventory->GetItem(m_mine_grid_x, m_mine_grid_y);
			
			if (p_item && (p_item->GetItemClass() == MItem::ITEM_CLASS_MINE || p_item->GetItemClass() == MItem::ITEM_CLASS_BOMB_MATERIAL)) // Item이 있다.
			{
				if(Timer())
				{
					gpC_base->SendMessage(UI_ITEM_USE, p_item->GetID(), 0, (MItem *)p_item);
					
					EndInstallMineProgress();
				}
				else
				{
					TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
					int item_x = x+GetFocusedItemGridX(p_item)+(p_item->GetGridWidth()*GRID_UNIT_PIXEL_X)/2-m_pC_mine_progress_spk->GetWidth(INVENTORY_BAR_BACK)/2;
					int item_y = y+GetFocusedItemGridY(p_item)+1;
					
					m_pC_mine_progress_spk->BltLocked(item_x, item_y, INVENTORY_BAR_BACK);
					
					Rect rect;
					rect.Set(0, 0, m_pC_mine_progress_spk->GetWidth(INVENTORY_BAR)*(GetTickCount() - gi_mine_progress)/m_dw_millisec, m_pC_mine_progress_spk->GetHeight(INVENTORY_BAR));
					m_pC_mine_progress_spk->BltLockedClip(item_x, item_y, rect, INVENTORY_BAR);
				}
			}
		}
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	//
	// 아이템 개수 표시
	//
	int len = 0;
	RECT rect[60];
	int num[60];
	g_pInventory->SetBegin();
	while (g_pInventory->IsNotEnd())
	{
		const MItem * p_item = g_pInventory->Get();
		
		// p_item은 NULL이 반드시 아니다. 왜냐하면 존재하는 것만 Get()하기 때문이다.
		assert(p_item);
		
		// 아이템 개수표시
		if(p_item->IsPileItem() || p_item->IsChargeItem())
		{
			RECT rt;
			rt.right = x+GetFocusedItemGridX(p_item) + p_item->GetGridWidth()*GRID_UNIT_PIXEL_X-1;
			for(int depth = 0, number = p_item->GetNumber(); number > 0; number/=10, depth++);
			if(depth == 0) depth = 1;
			rt.left = rt.right - 7*depth;
			rt.bottom = y+GetFocusedItemGridY(p_item) + p_item->GetGridHeight()*GRID_UNIT_PIXEL_Y;
			rt.top = rt.bottom - 12;
			
			rect[len] = rt;
			rt.left = max(0, rt.left);
			rt.top = max(0, rt.top);
			
			if(rt.left < rt.right && rt.top < rt.bottom)DrawAlphaBox(&rt, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
			num[len] = p_item->GetNumber();
			len++;
		}
		g_pInventory->Next();
	}
	
	char sz_temp[512];
	g_FL2_GetDC();	
	COLORREF markColor = RGB(220, 220, 220);//RGB(140, 140, 255);
	
	for(int i = 0; i < len; i++)
	{
		wsprintf(sz_temp, "%d", num[i]);
		g_PrintColorStr(rect[i].left, rect[i].top, sz_temp, gpC_base->m_item_desc_pi, markColor);	
	}
	// show money
	char money_buf[512];
	wsprintf(money_buf, "%d", g_pMoneyManager->GetMoney());
	std::string sstr = money_buf;
	
	for(i = 3; i <= 13; i += 4)
		if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
	
	sprintf(money_buf, "%s", sstr.c_str());
	g_Print(x+m_money_button_offset_x+30, y+m_money_button_offset_y+4, money_buf, &gpC_base->m_money_pi);
		
	m_pC_button_group->ShowDescription();
	g_FL2_ReleaseDC();
		
	DrawInventoryEffect();
		
	SHOW_WINDOW_ATTR;
		
		//
		// -- TEST
		//
#ifndef _LIB
		/*
		if (gpC_base->m_p_DDSurface_back->Lock())
		{
		S_SURFACEINFO	surface_info;
		S_RECT			rect;
		SetSurfaceInfo(&surface_info, gpC_base->m_p_DDSurface_back->GetDDSD());
		
		  // Grid 전체영역 표시 
		  rectangle(&surface_info, &m_grid_rect, GREEN);
		  
			//rectangle(&surface_info, &m_money_button_rect, WHITE);
			//rectangle(&surface_info, &m_close_button_rect, WHITE);
			
			  // 마우스 focus된 Grid 한 칸 표시
			  if (m_focus_grid_x != NOT_SELECTED && m_focus_grid_y != NOT_SELECTED)
			  {
			  SetRect(rect, m_grid_rect.x+(GRID_UNIT_PIXEL_X)*m_focus_grid_x,
			  m_grid_rect.y+(GRID_UNIT_PIXEL_Y)*m_focus_grid_y,
			  GRID_UNIT_PIXEL_X,
			  GRID_UNIT_PIXEL_Y);
			  
				filledRect(&surface_info, &rect, BLUE);
				}
				
				  for (int i=0; i < ITEM_REF_POINT_COUNT; i++)
				  {
				  //	putPixel(&surface_info, 
				  //		      g_item_ref_point[i].x+gpC_mouse_pointer->GetPointerX(),
				  //				g_item_ref_point[i].y+gpC_mouse_pointer->GetPointerY(),
				  //				RED);
				  }
				  
					gpC_base->m_p_DDSurface_back->Unlock();
					
					  char str[100];
					  
						sprintf(str, "focus grid (x, y) = %d, %d", m_focus_grid_x, m_focus_grid_y);
						g_Print(10, 420, str);
	}*/
#endif
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_INVENTORY::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	
	const MItem * p_selected_item;
	
	switch (message)
	{
	case M_MOVING:
		//			UnacquireMouseFocus();
		
		//
		// case:
		//			no item pickup state
		//			item pickup state
		//
		int i;
		int loop;
		int px, py;
		
		if (gpC_mouse_pointer->GetPickUpItem())
			loop = ITEM_REF_POINT_COUNT;
		else
		{
			loop = 1;
		}
		
		for (i=0; i < loop; i++)
		{
			if (loop == ITEM_REF_POINT_COUNT)
			{
				px = g_item_ref_point[i].x+gpC_mouse_pointer->GetPointerX()-x;
				py = g_item_ref_point[i].y+gpC_mouse_pointer->GetPointerY()-y;
			}
			else
			{
				px = _x;
				py = _y;
			}
			
			// search grid
			int distance_x = px - m_grid_rect.x;
			int distance_y = py - m_grid_rect.y;
			
			if (distance_x >= 0 && distance_x < m_grid_rect.w && 
				distance_y >= 0 && distance_y < m_grid_rect.h)
			{
				if(gpC_Imm != NULL && (m_focus_grid_x != distance_x/GRID_UNIT_PIXEL_X || m_focus_grid_y != distance_y/GRID_UNIT_PIXEL_Y))
					gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
				m_focus_grid_x = distance_x/GRID_UNIT_PIXEL_X;
				m_focus_grid_y = distance_y/GRID_UNIT_PIXEL_Y;
				
				if (loop == ITEM_REF_POINT_COUNT)
				{
					// item이 grid 영역에 어느정도 들어오면 안으로 위치시킨다.
					const MItem * p_pickup_item = gpC_mouse_pointer->GetPickUpItem();
					int a, b;
					switch (i)
					{
					case 0: // left up - first!
						a = m_focus_grid_x+p_pickup_item->GetGridWidth()-GRID_X;
						b = m_focus_grid_y+p_pickup_item->GetGridHeight()-GRID_Y;
						if (a > 0)
							m_focus_grid_x -= a;
						if (b > 0)
							m_focus_grid_y -= b;
						break;
						
					case 1: // right up
						m_focus_grid_x = 0;
						b = m_focus_grid_y+p_pickup_item->GetGridHeight()-GRID_Y;
						if (b > 0)
							m_focus_grid_y -= b;
						break;
						
					case 2: // left down
						m_focus_grid_y = 0;
						a = m_focus_grid_x+p_pickup_item->GetGridWidth()-GRID_X;
						if (a > 0)
							m_focus_grid_x -= a;
						break;
						
					case 3: // right down
						m_focus_grid_y = 0;
						if (m_focus_grid_x+1 <= p_pickup_item->GetGridHeight())
							m_focus_grid_x = 0;
					}
				}
				
				p_selected_item = g_pInventory->GetItem(m_focus_grid_x, m_focus_grid_y);
				if (p_selected_item != NULL)
				{
					MItem *pMouseItem = gpC_mouse_pointer->GetPickUpItem();
					g_descriptor_manager.Set(DID_ITEM, x+GetFocusedItemGridX(p_selected_item), y+GetFocusedItemGridY(p_selected_item), (void *)p_selected_item);
				}
				
				return true;
				//					break; // escape 'for'
			}
		}
		if(m_focus_grid_x != NOT_SELECTED || m_focus_grid_y != NOT_SELECTED)
			gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
		m_focus_grid_x = NOT_SELECTED;
		m_focus_grid_y = NOT_SELECTED;
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if (gpC_mouse_pointer->GetPickUpItem() == NULL && re && g_pInventory->GetItem(m_focus_grid_x, m_focus_grid_y) == NULL)
			//TestGridRect(_x, _y) == false && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
			break;
		}
		
		//
		// Item을 집던가 놓는다.
		//
		{
			bool ret = Click(m_grid_rect.x, m_grid_rect.y);
			
			if (ret)
				EMPTY_MOVE;
		}
		
		//
		// money button / close button
		//
		/*			if (m_bl_money_button_focused)
		m_bl_money_button_pushed = true;
		else if (m_bl_desc_button_focused)
		m_bl_desc_button_pushed = true;
		else if (m_bl_bike_button_focused)
		m_bl_bike_button_pushed = true;
		else if (m_bl_close_button_focused)
		m_bl_close_button_pushed = true;
		else if (m_bl_help_button_focused)
		m_bl_help_button_pushed = true;
		*/			break;
		
	case M_LEFTBUTTON_UP:
		CancelPushState();
		break;
		
	case M_RIGHTBUTTON_DOWN:
		//
		// Item을 사용한다.
		//
		if (gbl_sell_running == true)
		{
			int mx=x,my=y;
			if (gbl_item_trade_lock == false)
			{
				MItem * p_item = g_pInventory->GetItem(m_focus_grid_x, m_focus_grid_y);
				if(p_item && p_item->GetItemClass() == MItem::ITEM_CLASS_SKULL)
				{
					int price = 0;
					for(int y = 0; y < GRID_Y; y++)
					{
						for(int x = 0; x < GRID_X; x++)
						{
							MItem * p_sell_item = g_pInventory->GetItem(x, y);
							if(p_sell_item && p_sell_item->GetItemClass() == MItem::ITEM_CLASS_SKULL)
								price += g_pPriceManager->GetItemPrice(p_sell_item, MPriceManager::PC_TO_NPC)*p_sell_item->GetNumber();
						}
					}
					g_StartSellAllConfirmDialog(
						mx+p_item->GetGridX()*GRID_UNIT_PIXEL_X, 
						my+(1+p_item->GetGridY()+p_item->GetGridWidth())*GRID_UNIT_PIXEL_Y, price);
				}
			}
		}
		else Use();
		break;
		
	case M_RIGHTBUTTON_UP:
		EndInstallMineProgress();
		break;
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::ResetRect
//
// 전체 Grid rect를 설정한다. 이것은 Inventory의 이동이 있을 때마다 해줘야 한다.
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY::ResetRect()
{
	m_grid_rect.Set(m_grid_start_offset_x, 
								m_grid_start_offset_y, 
								GRID_UNIT_PIXEL_X*GRID_X, GRID_UNIT_PIXEL_Y*GRID_Y);
	
								/*	m_money_button_rect.Set(x+m_money_button_offset_x, y+m_money_button_offset_y,
								m_pC_etc_button_spk->GetWidth(MONEY_BUTTON_PUSHED),
								m_pC_etc_button_spk->GetHeight(MONEY_BUTTON_PUSHED));
								
								  m_desc_button_rect.Set(x+m_desc_button_offset_x, y+m_desc_button_offset_y,
								  m_pC_etc_button_spk->GetWidth(DESC_BUTTON_PUSHED),
								  m_pC_etc_button_spk->GetHeight(DESC_BUTTON_PUSHED));
								  
									m_bike_button_rect.Set(x+m_bike_button_offset_x, y+m_bike_button_offset_y,
									m_pC_etc_button_spk->GetWidth(BIKE_BUTTON_PUSHED),
									m_pC_etc_button_spk->GetHeight(BIKE_BUTTON_PUSHED));
									
									  m_close_button_rect.Set(x+m_close_button_offset_x, y+m_close_button_offset_y,
									  gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE),
									  gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));
									  
										m_help_button_rect.Set(x+m_help_button_offset_x, y+m_help_button_offset_y,
										gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_HELP),
										gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_HELP));
	*/
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::Use
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY::Use()
{
	if (m_focus_grid_x == NOT_SELECTED || m_focus_grid_y == NOT_SELECTED ||
		gbl_item_lock)
		return;
	
	if (gpC_mouse_pointer->GetPickUpItem())
	{
		
	}
	else
	{
		const MItem * p_item = g_pInventory->GetItem(m_focus_grid_x, m_focus_grid_y);
		
		if (p_item) // Item이 있다.
		{
			if(IsPlayerInSafePosition() && (p_item->GetItemClass() == MItem::ITEM_CLASS_BOMB_MATERIAL || p_item->GetItemClass() == MItem::ITEM_CLASS_MINE))
				return;
			
			if(StartInstallMineProgress(m_focus_grid_x, m_focus_grid_y) == false &&
				StartCreateMineProgress(m_focus_grid_x, m_focus_grid_y) == false &&
				StartCreateBombProgress(m_focus_grid_x, m_focus_grid_y) == false
				)
				gpC_base->SendMessage(UI_ITEM_USE, p_item->GetID(), 0, (MItem *)p_item);
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::StartInstallMineProgress
//-----------------------------------------------------------------------------
bool C_VS_UI_INVENTORY::StartInstallMineProgress(int focus_grid_x, int focus_grid_y)
{
	if(IsPlayerInSafePosition())
		return false;
	
	const MItem * p_item = g_pInventory->GetItem(focus_grid_x, focus_grid_y);
	if(p_item != NULL && gbl_vampire_interface == false && p_item->GetItemClass() == MItem::ITEM_CLASS_MINE &&
		g_pSkillAvailable->IsEnableSkill( SKILL_INSTALL_MINE )
		&& (*g_pSkillInfoTable)[SKILL_INSTALL_MINE].IsEnable()
		&& (*g_pSkillInfoTable)[SKILL_INSTALL_MINE].IsAvailableTime()
		)	// 지뢰인경우 지뢰 progress바 보여줌
	{
		int mine_level = (*g_pSkillInfoTable)[SKILL_INSTALL_MINE].GetExpLevel();
		m_dw_millisec = min(30, max(20, 30-mine_level/10))*100;
		Timer(true);
		gbl_mine_progress = true;
		m_mine_grid_x = focus_grid_x;
		m_mine_grid_y = focus_grid_y;
	}
	else
		gbl_mine_progress = false;
	
	return gbl_mine_progress;
}

// 0~4 : BOMB MATERIAL 5~9 : MINE MATERIAL
//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::StartCreateMineProgress
//-----------------------------------------------------------------------------
bool C_VS_UI_INVENTORY::StartCreateMineProgress(int focus_grid_x, int focus_grid_y)
{
	if(IsPlayerInSafePosition())
		return false;
	
	const MItem * p_item = g_pInventory->GetItem(focus_grid_x, focus_grid_y);
	if(p_item != NULL && gbl_vampire_interface == false && p_item->GetItemClass() == MItem::ITEM_CLASS_BOMB_MATERIAL && p_item->GetItemType() > 4 &&
		g_pSkillAvailable->IsEnableSkill( SKILL_MAKE_MINE )
		&& (*g_pSkillInfoTable)[SKILL_MAKE_MINE].IsEnable()
		&& (*g_pSkillInfoTable)[SKILL_MAKE_MINE].IsAvailableTime()
		)	// 지뢰인경우 지뢰 progress바 보여줌
	{
		int mine_level = (*g_pSkillInfoTable)[SKILL_MAKE_MINE].GetExpLevel();
		m_dw_millisec = min(30, max(20, 30-mine_level/10))*100;
		Timer(true);
		gbl_mine_progress = true;
		m_mine_grid_x = focus_grid_x;
		m_mine_grid_y = focus_grid_y;
	}
	else
		gbl_mine_progress = false;
	
	return gbl_mine_progress;
}

// 0~4 : BOMB MATERIAL 5~9 : MINE MATERIAL
//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::StartCreateBombProgress
//-----------------------------------------------------------------------------
bool C_VS_UI_INVENTORY::StartCreateBombProgress(int focus_grid_x, int focus_grid_y)
{
	if(IsPlayerInSafePosition())
		return false;
	
	const MItem * p_item = g_pInventory->GetItem(focus_grid_x, focus_grid_y);
	if(p_item != NULL && gbl_vampire_interface == false && p_item->GetItemClass() == MItem::ITEM_CLASS_BOMB_MATERIAL && p_item->GetItemType() < 5 &&
		g_pSkillAvailable->IsEnableSkill( SKILL_MAKE_BOMB )
		&& (*g_pSkillInfoTable)[SKILL_MAKE_BOMB].IsEnable()
		&& (*g_pSkillInfoTable)[SKILL_MAKE_BOMB].IsAvailableTime()
		)	// 지뢰인경우 지뢰 progress바 보여줌
	{
		int mine_level = (*g_pSkillInfoTable)[SKILL_MAKE_BOMB].GetExpLevel();
		m_dw_millisec = min(30, max(20, 30-mine_level/10))*100;
		Timer(true);
		gbl_mine_progress = true;
		m_mine_grid_x = focus_grid_x;
		m_mine_grid_y = focus_grid_y;
	}
	else
		gbl_mine_progress = false;
	
	return gbl_mine_progress;
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::Click
//
// 현재 Item을 들고 있으면 놓던가 교체하고, 들고 있지 않으면 Inventory에 있는
// 것을 집는다.
//
// 뭔가 했다면 true를, 그렇지않으면 false를 반환한다.
//
// grid_start_x, grid_start_y는 inventory grid 시작점이다. 이것을 참조하여 item
// (x, y)를 구한다.
//-----------------------------------------------------------------------------
bool C_VS_UI_INVENTORY::Click(int grid_start_x, int grid_start_y)
{
	if (m_focus_grid_x == NOT_SELECTED || 
		m_focus_grid_y == NOT_SELECTED ||
		gbl_item_lock == true)
	{
		return false;
	}
	
	//	int item_x, item_y;
	
	const MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();
	
	if (pMouseItem) // 들고 있는가?
	{
		const MItem* p_cur_item = g_pInventory->GetItem(m_focus_grid_x, m_focus_grid_y);
		
		// Enchant
		if(pMouseItem->GetItemClass() == MItem::ITEM_CLASS_EVENT_STAR && 
			pMouseItem->GetNumber() == 1 && p_cur_item != NULL && 
			p_cur_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_STAR && p_cur_item->IsGearItem() 
			&& !p_cur_item->IsUniqueItem() && !p_cur_item->IsQuestItem() &&
			p_cur_item->GetItemClass() != MItem::ITEM_CLASS_BLOOD_BIBLE &&
			p_cur_item->GetItemClass() != MItem::ITEM_CLASS_COUPLE_RING &&
			p_cur_item->GetItemClass() != MItem::ITEM_CLASS_VAMPIRE_COUPLE_RING
			)//p_cur_item->GetItemOptionListCount() <= 1)		// 레어 아이템일경우
		{	

			bool bCanUpgrade = false;
			
			// 송편일때
			if(pMouseItem->GetItemType() > 7)
			{
				if(p_cur_item->GetItemOptionListCount() == 0)
					bCanUpgrade = true;
			}
			else	// 송편이 아닌 일반 별일때
			{
				const std::list<TYPE_ITEM_OPTION> &optionList=p_cur_item->GetItemOptionList();
				std::list<TYPE_ITEM_OPTION>::const_iterator itr=optionList.begin();	
				
				while(itr!=optionList.end())
				{
					if((*g_pItemOptionTable)[*itr].UpgradeOptionType != 0)
					{
						bCanUpgrade=true;	
						break;
					}
					itr++;
				}	
			}
			
			if(bCanUpgrade)
			{
				// gpC_base->SendMessage(UI_ITEM_TO_ITEM, m_focus_grid_x, m_focus_grid_y, (void *)p_cur_item);
				m_mine_grid_x = m_focus_grid_x;
				m_mine_grid_y = m_focus_grid_y;
				gC_vs_ui.RunEnchant();
			}
		}
		// 총에 탄창을 끼우는 것과 같은 것이 insert item이다.
		// 위치가 완전히 일치할경우에만 추가한다.
		else
		if (pMouseItem->IsInsertToItem( p_cur_item ) && p_cur_item->GetGridX() == m_focus_grid_x && p_cur_item->GetGridY() == m_focus_grid_y)
		{
			// 이전에 있던 item에 추가될 수 있는 경우
				
			// 대상 Item과 들고 있는(추가할 Item)을 Client에서 알아야 한다.
			// 들고 있는 Item은 Client에서 access할 수 있으므로 대상 Item을 보낸다.
			gpC_base->SendMessage(UI_ITEM_INSERT_FROM_INVENTORY,
				m_focus_grid_x, m_focus_grid_y,
				(void *)p_cur_item); // 대상 Item
		}
		else
		{	
			// 추가될 수 없는 경우
			MItem* p_old_item  = NULL;
				
			if (g_pInventory->CanReplaceItem(gpC_mouse_pointer->GetPickUpItem(),		// 추가할 item
				m_focus_grid_x, m_focus_grid_y,	// 추가할 위치 
				p_old_item))								// 원래있던 item
			{
				
				gpC_base->SendMessage(UI_ITEM_DROP_TO_INVENTORY, 
					m_focus_grid_x, m_focus_grid_y,
					(void *)pMouseItem);
				
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
		MItem * p_item = g_pInventory->GetItem(m_focus_grid_x, m_focus_grid_y);
		
		if (p_item != NULL) // Item이 있다.
		{
			int number = p_item->GetNumber();
			
			if(gpC_mouse_pointer->IsCursorDescription())
				gC_vs_ui.RunDescDialog(DID_ITEM, (void *)p_item);
			else if (gbl_sell_running == true)
			{
				if (gbl_item_trade_lock == false)// && p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_GIFT_BOX)// && p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_STAR)
				{
					m_p_sell_item = (MItem *)p_item;
					g_StartSellConfirmDialog(x+grid_start_x+p_item->GetGridX()*GRID_UNIT_PIXEL_X, 
						y+grid_start_y+(p_item->GetGridY()+p_item->GetGridWidth())*GRID_UNIT_PIXEL_Y);
				}
			}
			else if (gbl_repair_running == true)
			{
				if (gbl_item_trade_lock == false && 
					p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_GIFT_BOX && 
					p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_TREE && 
					p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_ETC && 
					p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_STAR &&
					p_item->GetItemClass() != MItem::ITEM_CLASS_VAMPIRE_AMULET &&
					p_item->GetItemClass() != MItem::ITEM_CLASS_BLOOD_BIBLE &&					
					!p_item->IsUniqueItem() &&
					!p_item->IsQuestItem() )
				{					
					// repair 가격이 0보다 클 때... by sigi
					if (g_pPriceManager->GetItemPrice((MItem*)p_item, MPriceManager::REPAIR) > 0)
					{
						m_p_repair_item = (MItem *)p_item;
						g_StartRepairConfirmDialog(x+grid_start_x+p_item->GetGridX()*GRID_UNIT_PIXEL_X, y+grid_start_y+(p_item->GetGridY()+p_item->GetGridWidth())*GRID_UNIT_PIXEL_Y, p_item->IsChargeItem());
					}
				} else
				{
					gpC_base->SendMessage(UI_MESSAGE_BOX, 
						STRING_MESSAGE_CANNOT_REPAIR, 
						0, 
						NULL);
				}
			}
			else if (gbl_silvering_running == true)
			{
				if (gbl_item_trade_lock == false
					&& p_item->GetItemClass() != MItem::ITEM_CLASS_BLOOD_BIBLE
					&& p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_TREE
					&& p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_ETC
					&& p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_GIFT_BOX
					&& p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_STAR)
				{
					// silvering 가격이 0보다 클 때... by larosel
					if (g_pPriceManager->GetItemPrice((MItem*)p_item, MPriceManager::SILVERING) > 0)
					{
						m_p_silvering_item = (MItem *)p_item;
						g_StartSilveringConfirmDialog(x+grid_start_x+p_item->GetGridX()*GRID_UNIT_PIXEL_X, y+grid_start_y+(p_item->GetGridY()+p_item->GetGridWidth())*GRID_UNIT_PIXEL_Y);
					}
				}
			}
			else
			{
				//#ifdef _LIB
				// SHIFT누르고 클릭했을때 아이템 자동 이동 처리 -> 퀵아이템 -> 기어창 -> 보관함
				if(g_pDXInput->KeyDown(DIK_LSHIFT))
				{
					if(gC_vs_ui.IsRunningStorage() && g_pStorage)
					{
						int current_storage = g_pStorage->GetCurrent();
						const int storage_size = g_pStorage->GetSize();
						const int storage_max = STORAGE_SLOT;
						
						int i, j;
						
						for(j = 0; j < storage_size; j++)
						{
						//	g_pStorage->SetCurrent(j);
							g_pStorage->SetCurrent(current_storage);
							
							if(p_item->IsPileItem())
							{
								for(i = 0; i < storage_max; i++)
								{
									const MItem *p_slot_item = g_pStorage->GetItem(i);
									
									// 슬롯에 머가 있는경우 아이템 종류가 같은지 보고, 쌓을수 있는가 보고 쌓는다.
									if(p_item->IsInsertToItem( p_slot_item ))
									{
										int total_number = p_slot_item->GetNumber()+p_item->GetNumber();
										int max_number = p_item->GetMaxNumber();
										if(total_number <= max_number)
										{
											g_pInventory->RemoveItem(m_focus_grid_x, m_focus_grid_y);
											
											// 이전에 있던 item에 추가될 수 있는 경우
											
											// 대상 Item과 들고 있는(추가할 Item)을 Client에서 알아야 한다.
											// 들고 있는 Item은 Client에서 access할 수 있으므로 대상 Item을 보낸다.
											// 클라이언트에서 픽업된 아이템을 참조하므로 먼저 픽업
											// 픽업할때의 인자는 아무거나 보내도 상관없다 바로 떨어뜨릴꺼니까
#ifdef _LIB
											gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
												m_focus_grid_x, m_focus_grid_y,
												(MItem *)p_item);
#else
											int item_x = x+grid_start_x+p_item->GetGridX()*GRID_UNIT_PIXEL_X+(p_item->GetGridWidth()*GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(p_item->GetInventoryFrameID())/2;
											int item_y = y+grid_start_y+p_item->GetGridY()*GRID_UNIT_PIXEL_Y+(p_item->GetGridHeight()*GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(p_item->GetInventoryFrameID())/2;
											
											
											gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
												MAKEDWORD(m_focus_grid_x, m_focus_grid_y),
												MAKEDWORD(item_x, item_y),
												(MItem *)p_item);
#endif
											gpC_base->SendMessage(UI_SELECT_STORAGE_SLOT, current_storage, i);
											
											return true;
										}
									}	
								}
							}
							current_storage ++;
							if(current_storage >= storage_size)
								current_storage = 0;
						}
						for(j = 0; j < storage_size; j++)
						{
							//g_pStorage->SetCurrent(j);
							g_pStorage->SetCurrent(current_storage);
							
							for(i = 0; i < storage_max; i++)
							{
								const MItem *p_slot_item = g_pStorage->GetItem(i);
								
								// 슬랏이 비었으면 걍~ 넣는다
								if(p_slot_item == NULL)
								{
									g_pInventory->RemoveItem(m_focus_grid_x, m_focus_grid_y);
									
#ifdef _LIB
									gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
										m_focus_grid_x, m_focus_grid_y,
										(MItem *)p_item);
#else
									int item_x = x+grid_start_x+p_item->GetGridX()*GRID_UNIT_PIXEL_X+(p_item->GetGridWidth()*GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(p_item->GetInventoryFrameID())/2;
									int item_y = y+grid_start_y+p_item->GetGridY()*GRID_UNIT_PIXEL_Y+(p_item->GetGridHeight()*GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(p_item->GetInventoryFrameID())/2;
									
									
									gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
										MAKEDWORD(m_focus_grid_x, m_focus_grid_y),
										MAKEDWORD(item_x, item_y),
										(MItem *)p_item);
#endif
									
									gpC_base->SendMessage(UI_SELECT_STORAGE_SLOT, current_storage, i);
									
									return true;
								}
							}
							current_storage ++;
							if(current_storage >= storage_size)
								current_storage = 0;
						}
						// Storage에 아이템을 넣을 공간이 없다면 여기까지 오게 되고, 그렇다면 원래의 스토리지를 선택
						g_pStorage->SetCurrent(current_storage);
					}
					else
					// 퀵슬랏에 들어갈 수 있는 아이템인가 보고 퀵슬랏이 있는가 본다.
					if(p_item->IsQuickItem() && g_pQuickSlot != NULL)
					{
						// 큇슬랏 개수만큼 넣을수 있는가 검사한다
						// 처음에 쌓을수 있는가를 알아보고 쌓을수 있는곳이 없으면 빈곳에 넣는다
						const int quick_slot_max = g_pQuickSlot->GetSize();
						if(p_item->IsPileItem())
							for(int i = 0; i < quick_slot_max; i++)
							{
								MItem *p_slot_item = g_pQuickSlot->GetItem(i);
								
								// 슬롯에 머가 있는경우 아이템 종류가 같은지 보고, 쌓을수 있는가 보고 쌓는다.
								if(p_item->IsInsertToItem( p_slot_item ))
								{
									int total_number = p_slot_item->GetNumber()+p_item->GetNumber();
									int max_number = p_item->GetMaxNumber();
									if(total_number <= max_number)
									{
										g_pInventory->RemoveItem(m_focus_grid_x, m_focus_grid_y);
										
										// 이전에 있던 item에 추가될 수 있는 경우
										
										// 대상 Item과 들고 있는(추가할 Item)을 Client에서 알아야 한다.
										// 들고 있는 Item은 Client에서 access할 수 있으므로 대상 Item을 보낸다.
										// 클라이언트에서 픽업된 아이템을 참조하므로 먼저 픽업
										// 픽업할때의 인자는 아무거나 보내도 상관없다 바로 떨어뜨릴꺼니까
#ifdef _LIB
										gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
											m_focus_grid_x, m_focus_grid_y,
											(MItem *)p_item);
#else
										int item_x = x+grid_start_x+p_item->GetGridX()*GRID_UNIT_PIXEL_X+(p_item->GetGridWidth()*GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(p_item->GetInventoryFrameID())/2;
										int item_y = y+grid_start_y+p_item->GetGridY()*GRID_UNIT_PIXEL_Y+(p_item->GetGridHeight()*GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(p_item->GetInventoryFrameID())/2;
										
										
										gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
											MAKEDWORD(m_focus_grid_x, m_focus_grid_y),
											MAKEDWORD(item_x, item_y),
											(MItem *)p_item);
#endif
										gpC_base->SendMessage(UI_ITEM_INSERT_FROM_QUICKSLOT,
											i, 0, (void *)p_slot_item); // 대상 Item
										
										return true;
									}
								}	
							}
							for(int i = 0; i < quick_slot_max; i++)
							{
								MItem *p_slot_item = g_pQuickSlot->GetItem(i);
								
								// 슬랏이 비었으면 걍~ 넣는다
								if(p_slot_item == NULL)
								{
									g_pInventory->RemoveItem(m_focus_grid_x, m_focus_grid_y);
									
#ifdef _LIB
									gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
										m_focus_grid_x, m_focus_grid_y,
										(MItem *)p_item);
#else
									int item_x = x+grid_start_x+p_item->GetGridX()*GRID_UNIT_PIXEL_X+(p_item->GetGridWidth()*GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(p_item->GetInventoryFrameID())/2;
									int item_y = y+grid_start_y+p_item->GetGridY()*GRID_UNIT_PIXEL_Y+(p_item->GetGridHeight()*GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(p_item->GetInventoryFrameID())/2;
									
									
									gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
										MAKEDWORD(m_focus_grid_x, m_focus_grid_y),
										MAKEDWORD(item_x, item_y),
										(MItem *)p_item);
#endif
									
									if (g_pQuickSlot->CanReplaceItem(p_item, i, p_slot_item))
									{
										gpC_base->SendMessage(UI_ITEM_DROP_TO_QUICKSLOT,
											i); // 들고 있던 것을 보낸다.
										
										return true;
									}
									else
									{
										return false;
									}
								}
							}
					}
					// 기어창에 들어갈 수 있는 아이템인가 보고 넣는다
					else if(p_item->IsGearItem() && gbl_gear_lock == false)
					{
						int slot_max = 0;
						
						// 슬롯개수 만큼 돌면서 아이템이 들어가나 본다.
						// 뱀파이어와 슬레이어의 슬롯 개수는 틀리므로 따로따로
						if(gbl_vampire_interface)
							slot_max = MVampireGear::MAX_GEAR_VAMPIRE;
						else
							slot_max = MSlayerGear::MAX_GEAR_SLAYER;
						
						
						int add_slot = -1;
						MItem *pChangeItem = NULL;
						for(int i = 0; i < slot_max; i++)
						{
							MItem * p_slot_item = NULL;
							
							if (gC_vs_ui.CanReplaceItemInGear(p_item, i, p_slot_item))
							{
								add_slot = i;
								pChangeItem = p_slot_item;
								// 빈 곳이면 바로 넣어버리면 된다.
								// 빈 곳이 아니라면.. 다음걸 찾는다.
								if(p_slot_item == NULL)
								{
									break;
								}
							}
						}
						
						if(add_slot != -1)
						{
							// 딱걸렸다
							// 클라이언트에서 픽업된 아이템을 참조하므로 먼저 픽업
							// 픽업할때의 인자는 아무거나 보내도 상관없다 바로 떨어뜨릴꺼니까
							g_pInventory->RemoveItem(m_focus_grid_x, m_focus_grid_y);
#ifdef _LIB
							gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
								m_focus_grid_x, m_focus_grid_y,
								(MItem *)p_item);
#else
							int item_x = x+grid_start_x+p_item->GetGridX()*GRID_UNIT_PIXEL_X+(p_item->GetGridWidth()*GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(p_item->GetInventoryFrameID())/2;
							int item_y = y+grid_start_y+p_item->GetGridY()*GRID_UNIT_PIXEL_Y+(p_item->GetGridHeight()*GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(p_item->GetInventoryFrameID())/2;
							
							
							gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
								MAKEDWORD(m_focus_grid_x, m_focus_grid_y),
								MAKEDWORD(item_x, item_y),
								(MItem *)p_item);
#endif
							
							// 기어에 넣는다
							gpC_base->SendMessage(UI_ITEM_DROP_TO_GEAR, 
								add_slot, 
								0, 
								NULL);
							
							// 기어에 넣은 아이템이 벨트라면 벨트를 연다.
							if(p_item->GetItemClass() == MItem::ITEM_CLASS_BELT)
								gC_vs_ui.RunQuickItemSlot();
							
							return true;
						}
					}
					
				}
				//					gpC_base->SendMessage(UI_ITEM_TO_QUICKITEMSLOT, 
				//																	m_focus_grid_x, m_focus_grid_y,
				//																	(MItem *)p_item);
				else
				{
					//#endif
					
					g_pInventory->RemoveItem(m_focus_grid_x, m_focus_grid_y);
					
#ifdef _LIB
					gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
						m_focus_grid_x, m_focus_grid_y,
						(MItem *)p_item);
#else
					int item_x = x+grid_start_x+p_item->GetGridX()*GRID_UNIT_PIXEL_X+(p_item->GetGridWidth()*GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(p_item->GetInventoryFrameID())/2;
					int item_y = y+grid_start_y+p_item->GetGridY()*GRID_UNIT_PIXEL_Y+(p_item->GetGridHeight()*GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(p_item->GetInventoryFrameID())/2;
					
					
					gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY,
						MAKEDWORD(m_focus_grid_x, m_focus_grid_y),
						MAKEDWORD(item_x, item_y),
						(MItem *)p_item);
#endif
				}
			}
		}
	}
	
	return true;
}

void C_VS_UI_INVENTORY::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);
	
	//	if (message == WM_KEYDOWN)
	//	{
	//		switch (key)
	//		{
	//		case VK_SHIFT:
	//			m_bl_shift = true;
	//			break;
	//		}
	//	}
	//	else if(message == WM_KEYUP)
	//	{
	//		switch(key)
	//		{
	//		case VK_SHIFT:
	//			m_bl_shift = false;
	//			break;
	//		}
	//	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY::Timer
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_INVENTORY::Timer(bool reset)
{
	if(reset)
	{
		gi_mine_progress = GetTickCount();
	}
	else if(gi_mine_progress+m_dw_millisec <= GetTickCount())
	{
		gi_mine_progress = GetTickCount();
		return true;
	}
	
	return false;
}


/*-----------------------------------------------------------------------------
- C_VS_UI_SKILL
-
-----------------------------------------------------------------------------*/
C_VS_UI_SKILL::C_VS_UI_SKILL()
{
	if(gbl_vampire_interface)
	{
		m_etc_spk.Open(SPK_VAMPIRE_SKILL_ETC);
		m_skill_guard_x = 42;
		m_skill_guard_y = 42;
		m_skill_start_x = RESOLUTION_X-m_skill_guard_x-10;
		m_skill_start_y = 455+14;
		m_skill_guard_extra_offset = 3;
		m_extra_icon_offset_x = 1;
		m_extra_icon_offset_y = 2;
		
		Set(m_skill_start_x, m_skill_start_y, m_skill_guard_x, m_skill_guard_y);
	}
	else
	{
		m_etc_spk.Open(SPK_SLAYER_SKILL_ETC);
		m_skill_guard_x = 42;
		m_skill_guard_y = 42;
		m_skill_start_x = RESOLUTION_X-m_skill_guard_x-10;
		m_skill_start_y = 455+14;
		m_skill_guard_extra_offset = 3;
		m_extra_icon_offset_x = 1;
		m_extra_icon_offset_y = 2;
		
		Set(m_skill_start_x, m_skill_start_y, m_skill_guard_x, m_skill_guard_y);
	}
	
	if (m_C_spk.GetSize()==0)
	{
		ifstream	file(SPK_SKILL_ICON, ios::binary|ios::nocreate);
		if (!file)
			_Error(FILE_OPEN);
		
		m_C_spk.LoadFromFile(file);
		file.close();
		
		ifstream	file2(SPK_SKILL_MINI, ios::binary|ios::nocreate);
		if (!file)
			_Error(FILE_OPEN);
		
		m_C_spk_mini.LoadFromFile(file2);
		file2.close();
	}
	
	g_RegisterWindow(this);
	
	AttrPin(true);
	
	m_current_skill_set = -1;
	m_current_skill_grade = -1;
	m_selected_skillid = NOT_SELECTED;
	
	for (int i=0; i < HOTKEY_MAX; i++)
	{
		for(int j=0; j < GRADE_MAX; j++)
		{
			m_skill_hotkey_buf[i][j] = NOT_SELECTED;
		}
	}
	
	// TIMER
	m_dw_prev_tickcount = GetTickCount();
	m_dw_millisec = 2000;
	
	
#ifndef _LIB
	//	g_pSkillAvailable->AddSkill(SKILL_BLOOD_DRAIN);
	//	g_pSkillAvailable->AddSkill(MAGIC_PROTECTION_FROM_PARALYSIS);
	//	g_pSkillAvailable->AddSkill(MAGIC_RESTORE);
	//	g_pSkillAvailable->AddSkill(MAGIC_DETECT_HIDDEN);
	g_pSkillAvailable->AddSkill(MAGIC_CONTINUAL_LIGHT);
	(*g_pSkillInfoTable)[MAGIC_CONTINUAL_LIGHT].SetPassive();
	
	g_pSkillAvailable->AddSkill(MAGIC_HIDE);
	//	g_pSkillAvailable->DisableSkill(MAGIC_HIDE);
	//	g_pSkillAvailable->AddSkill(MAGIC_DARKNESS);
	//	g_pSkillAvailable->AddSkill(MAGIC_POISONOUS_HANDS);
	//	g_pSkillAvailable->AddSkill(MAGIC_ACID_TOUCH);
	//	g_pSkillAvailable->AddSkill(MAGIC_GREEN_POISON);
	//	g_pSkillAvailable->AddSkill(MAGIC_YELLOW_POISON);
	//	g_pSkillAvailable->AddSkill(MAGIC_PURPLE_WALL);
	//	g_pSkillAvailable->AddSkill(MAGIC_ACID_BOLT);
	//	g_pSkillAvailable->AddSkill(MAGIC_GREEN_STALKER);
	//	g_pSkillAvailable->AddSkill(MAGIC_INVISIBILITY);
	//	g_pSkillAvailable->AddSkill(MAGIC_DARKBLUE_POISON);
	//	g_pSkillAvailable->AddSkill(MAGIC_ACID_BALL);
	//	g_pSkillAvailable->AddSkill(MAGIC_ACID_SWAMP);
	//	g_pSkillAvailable->AddSkill(MAGIC_BLOODY_NAIL);
	//	g_pSkillAvailable->AddSkill(MAGIC_BLOODY_KNIFE);
	//	g_pSkillAvailable->AddSkill(MAGIC_BLOODY_BALL);
	//	g_pSkillAvailable->AddSkill(MAGIC_BLOODY_WALL);
	//	g_pSkillAvailable->AddSkill(MAGIC_BLOODY_SPEAR);
	//	g_pSkillAvailable->AddSkill(MAGIC_SUMMON);
#endif
}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_SKILL
-
-----------------------------------------------------------------------------*/
C_VS_UI_SKILL::~C_VS_UI_SKILL()
{
	gpC_vs_ui_window_manager->SetHotKey((int **)m_skill_hotkey_buf);
	m_C_spk.Release();
	m_C_spk_mini.Release();
	
	g_UnregisterWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL::ChangeSkillByHotkey
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL::ChangeSkillByHotkey(HOTKEY hotkey)
{
	if(m_current_skill_set != hotkey)
	{
		m_current_skill_set = hotkey;
		m_current_skill_grade = 0;
	}
	else
	{
		for(int i = 1; i < GRADE_MAX+1; i++)
		{
			if(m_skill_hotkey_buf[m_current_skill_set][(m_current_skill_grade+i)%GRADE_MAX] != NOT_SELECTED)
			{
				m_current_skill_grade = (m_current_skill_grade+i)%GRADE_MAX;
				break;
			}
		}
	}
	int id = m_skill_hotkey_buf[m_current_skill_set][m_current_skill_grade];
	
	if (id != NOT_SELECTED)
	{
		MSkillSet::SKILLID_MAP::iterator iNode = g_pSkillAvailable->begin();
		while (iNode != g_pSkillAvailable->end())
		{
			if(((*iNode).second).SkillID == id)break;
			iNode++;
		}
		
		if(iNode != g_pSkillAvailable->end())
		{
			m_selected_skillid = id;
			gpC_base->SendMessage(UI_SELECT_SKILL, m_selected_skillid);
		}
		
		EMPTY_MOVE;
		
		Timer(true);
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL::ChangeSkillWheel
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL::ChangeSkillWheel(bool direct)
{
	int id = m_selected_skillid;
	
	if(g_pSkillAvailable->empty())return;
	
	MSkillSet::SKILLID_MAP::iterator iNode = g_pSkillAvailable->begin();
	
	//	if (id != NOT_SELECTED)
	{
		while (iNode != g_pSkillAvailable->end())
		{
			if(((*iNode).second).SkillID == id)break;
			iNode++;
		}
		
		if(iNode != g_pSkillAvailable->end())
		{
			if(direct)
			{
				do
				{
					iNode++;
					if(g_pSkillAvailable->end() == iNode)
						iNode = g_pSkillAvailable->begin();
				}while((*g_pSkillInfoTable)[(ACTIONINFO)((*iNode).second).SkillID].IsPassive() && id != ((*iNode).second).SkillID);
			}
			else 
			{
				do
				{
					if(g_pSkillAvailable->begin() == iNode)
						iNode = g_pSkillAvailable->end();
					iNode--;
				}while((*g_pSkillInfoTable)[(ACTIONINFO)((*iNode).second).SkillID].IsPassive() && id != ((*iNode).second).SkillID);
			}
		}
		else
		{
			if(direct)
			{
				iNode = g_pSkillAvailable->begin();
				while((*g_pSkillInfoTable)[(ACTIONINFO)((*iNode).second).SkillID].IsPassive())
				{
					iNode++;
					if(g_pSkillAvailable->end() == iNode)
						break;
				}
			}
			else 
			{
				do
				{
					if(g_pSkillAvailable->begin() == iNode)
					{
						iNode = g_pSkillAvailable->end();
						break;
					}
					iNode--;
				}while((*g_pSkillInfoTable)[(ACTIONINFO)((*iNode).second).SkillID].IsPassive());
			}
		}
		
		Timer(true);
	}
	
	if(iNode != g_pSkillAvailable->end())
		m_selected_skillid = ((*iNode).second).SkillID;
	else
		m_selected_skillid = NOT_SELECTED;
	
	gpC_base->SendMessage(UI_SELECT_SKILL, m_selected_skillid);
	
	EMPTY_MOVE;
	
	
}

void	C_VS_UI_SKILL::SelectSkill(int id)
{
	m_selected_skillid = id;
	gpC_base->SendMessage(UI_SELECT_SKILL, m_selected_skillid);
	EMPTY_MOVE;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL::UnselectSkill
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL::UnselectSkill()
{
	m_selected_skillid = NOT_SELECTED;
	gpC_base->SendMessage(UI_SELECT_SKILL, NOT_SELECTED);
	EMPTY_MOVE;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL::SetHotkey
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL::SetHotkey(HOTKEY hotkey, ACTIONINFO id)
{
	int i, j;
	
	for (i=0; i < HOTKEY_MAX; i++)
	{
		for(j=0; j < GRADE_MAX; j++)
		{
			if (m_skill_hotkey_buf[i][j] == id)
			{
				//				m_skill_hotkey_buf[i][j] = NOT_SELECTED;
				for(int k = j; k < GRADE_MAX-1; k++)
					m_skill_hotkey_buf[i][k] = m_skill_hotkey_buf[i][k+1];
				m_skill_hotkey_buf[i][k] = NOT_SELECTED;
				return;
			}
		}
	}
	
	for(j = 0; j < GRADE_MAX; j++)
	{
		if(m_skill_hotkey_buf[hotkey][j] == NOT_SELECTED)
		{
			m_skill_hotkey_buf[hotkey][j] = id;
			return;
		}
	}
	if(j == GRADE_MAX)
	{
		for(i = 0; i < GRADE_MAX-1; i++)
		{
			m_skill_hotkey_buf[hotkey][i] = m_skill_hotkey_buf[hotkey][i+1];
		}
		m_skill_hotkey_buf[hotkey][i] = id;
		return;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL::ToggleWindow
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL::ToggleWindow()
{
	ResetHotkey();
	if (m_bl_open == false)
	{
		// skill interface가 열린다.
		
		g_pSkillAvailable->SetAvailableSkills(); // 열릴 때 한 번만 해준다.
		
		if(gbl_vampire_interface)
			PlaySound(SOUND_VAMPIRE_ICON);
		else
			PlaySound(SOUND_SLAYER_SKILL);
	}
	
	m_bl_open = !m_bl_open;
	
	if(g_pSkillAvailable->size() == 0) m_bl_open = false;
	
	ResetSize();
	
	// re-acquire
	EMPTY_MOVE;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL::NoPassive
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_SKILL::NoPassive(int n) const
{
	if (n != NOT_SELECTED)
	{
		int id = GetSkillID(n);
		if (id != NOT_SELECTED && (*g_pSkillInfoTable)[id].IsPassive())
			return false;
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// CloseInterface
//
// 펼쳐져있다면.. 닫는다. 외부에서 닫기를 요청할 때 실행된다.
//-----------------------------------------------------------------------------
void	C_VS_UI_SKILL::CloseInterface()
{
	m_bl_pushed = false;
	m_bl_open = false;
	
	ResetSize();
}

//-----------------------------------------------------------------------------
// AbleToUse
//
// 사용가능한 마법인가?
//-----------------------------------------------------------------------------
bool C_VS_UI_SKILL::AbleToUse(int id) const
{
	return g_pSkillAvailable->IsEnableSkill((ACTIONINFO)id);
	//	{
	//		return (*g_pSkillInfoTable)[id].IsAvailableTime();
	//	}
	//	return false;
	
}

//-----------------------------------------------------------------------------
// IsEnableSkill
//
// 사용불가된 기술이 아닌가?(도메인등등의 이유)
//-----------------------------------------------------------------------------
bool C_VS_UI_SKILL::IsEnableSkill(int id) const
{
	return (*g_pSkillInfoTable)[id].IsEnable();
}

//-----------------------------------------------------------------------------
// GetDelay
//
// Delay가 얼마나 남았나 %
//-----------------------------------------------------------------------------
//int C_VS_UI_SKILL::GetDelay(int id) const
//{
//	if((*g_pSkillInfoTable)[id].GetDelayTime() == 0)return 0;	
//	
//	return (*g_pSkillInfoTable)[id].GetAvailableTimeLeft()*100/(*g_pSkillInfoTable)[id].GetDelayTime();
//}
int C_VS_UI_SKILL::GetDelay(int id) const
{
	if((*g_pSkillInfoTable)[id].GetDelayTime() == 0)return 0;	
	
	int DelayTime = (*g_pSkillInfoTable)[id].GetDelayTime();

	
	if( id == SKILL_WILL_OF_LIFE )
		DelayTime = (3+ (g_char_slot_ingame.level/10))*2*1000;
		

	return (*g_pSkillInfoTable)[id].GetAvailableTimeLeft()*100/DelayTime;
}

//-----------------------------------------------------------------------------
// GetSkillID
//
// 실패하면 NOT_SELECTED을 반환하고 성공하면 n번째 skill id를 반환한다.
//-----------------------------------------------------------------------------
int C_VS_UI_SKILL::GetSkillID(int n) const
{
	if (n > 0)
	{
		int i = 0;
		MSkillSet::SKILLID_MAP::iterator iNode = g_pSkillAvailable->begin();
		while (iNode != g_pSkillAvailable->end())
		{
			// skill의 id와 status
			ACTIONINFO id = ((*iNode).second).SkillID;
			
			// 다음
			iNode++;
			i++;
			
			if (n == i)
				return id;
		}
	}
	
	return NOT_SELECTED;
}

//-----------------------------------------------------------------------------
// GetFocusSlot
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_SKILL::GetFocusSlot(int _x, int _y) const
{
	if (m_bl_open == true)
	{
		for (int i=0; i < g_pSkillAvailable->size()+1; i++)
		{
			int px = m_skill_start_x-(m_skill_guard_x*(i%SPREAD_X_MAX));
			int py = m_skill_start_y-(m_skill_guard_y*(i/SPREAD_X_MAX));
			
			if (_x >= px && _x < px+m_skill_guard_x &&
				_y >= py && _y < py+m_skill_guard_y)
			{
				return i;
			}
		}
	}
	else
	{
		if (IsInRect(_x, _y) == true)
			return 0;
	}
	
	return NOT_SELECTED;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL::GetIconPoint
//
// n번째 icon의 point를 반환한다.
//-----------------------------------------------------------------------------
Point	C_VS_UI_SKILL::GetIconPoint(int n) const
{
	Point point;
	
	point.x = m_skill_start_x-(m_skill_guard_x*(n%SPREAD_X_MAX));
	point.y = m_skill_start_y-(m_skill_guard_y*(n/SPREAD_X_MAX));
	
	return point;
}

//-----------------------------------------------------------------------------
// ResetSize
//
// !가능한 skill이 하나두 없을 때는 open되지 않는다.
//-----------------------------------------------------------------------------
void C_VS_UI_SKILL::ResetSize()
{
	if (m_bl_open == true)
	{
		if (g_pSkillAvailable->size() > (SPREAD_X_MAX-1))
		{
			x = m_skill_start_x-m_skill_guard_x*(SPREAD_X_MAX-1);
			w = m_skill_guard_x*SPREAD_X_MAX;
		}
		else
		{
			x = m_skill_start_x-m_skill_guard_x*g_pSkillAvailable->size();
			w = m_skill_guard_x*(g_pSkillAvailable->size()+1);
		}
		
		y = m_skill_start_y-m_skill_guard_y*(g_pSkillAvailable->size()/SPREAD_X_MAX);
		h = m_skill_guard_y+m_skill_guard_y*(g_pSkillAvailable->size()/SPREAD_X_MAX);
	}
	else
	{
		x = m_skill_start_x;
		y = m_skill_start_y;
		w = m_skill_guard_x;
		h = m_skill_guard_y;
	}
}

//-----------------------------------------------------------------------------
// UnacquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SKILL::UnacquireMouseFocus()
{
	m_focused_slot = NOT_SELECTED;
}

//-----------------------------------------------------------------------------
// CancelPushState
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SKILL::CancelPushState()
{
	m_bl_pushed = false;
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SKILL::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_SKILL::IsPixel(int _x, int _y)
{
	return (GetFocusSlot(_x, _y) != NOT_SELECTED);
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SKILL::Start()
{
	PI_Processor::Start();
	
	memcpy(m_skill_hotkey_buf, gpC_vs_ui_window_manager->GetHotKey(), sizeof(int)*C_VS_UI_SKILL::HOTKEY_MAX*C_VS_UI_SKILL::GRADE_MAX);
	gpC_window_manager->AppearWindow(this);
	
	m_focused_slot = NOT_SELECTED;
	m_bl_open = false;
	m_bl_pushed = false;
	
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SKILL::Process()
{
	
}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SKILL::KeyboardControl(UINT message, UINT key, long extra)
{
}

/*-----------------------------------------------------------------------------
- MouseControl
- Skill Mouse Control.

  `Skill은 화면 어디에나 나올 수 있으므로 Skill 하나하나의 좌표를 검사해서
  UI 입력 여부를 결정해야 한다.
-----------------------------------------------------------------------------*/
bool C_VS_UI_SKILL::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	
	switch (message)
	{
	case M_MOVING:
		//
		// 사용불가능한 것은 0번 slot을 빼고 선택되지 않는다.
		// 여기서 결정해야 한다.
		//
		{
			int focused_slot = GetFocusSlot(_x, _y); // m_focused_slot은 중요하니까...
			
			int id;
			if (focused_slot == 0 && m_selected_skillid != NOT_SELECTED)
				id = m_selected_skillid;
			else
				id = GetSkillID(focused_slot);
			
			if (id != NOT_SELECTED)
			{
				Point point = GetIconPoint(focused_slot);
				g_descriptor_manager.Set(DID_SKILL, point.x, point.y, NULL, id, m_skill_guard_y);
			}
			
			if (!NoPassive(focused_slot))
				focused_slot = NOT_SELECTED;
			
			if(m_focused_slot != focused_slot)
			{
				gpC_Imm->ForceUI(CImm::FORCE_UI_WINDOW);
				m_focused_slot = focused_slot;
			}
			
			//				if(m_focused_slot != focused_slot)
			//				{
			//					if(gpC_Imm)
			//						gpC_Imm->ForceUI(CImm::FORCE_UI_WINDOW);
			//					m_focused_slot = focused_slot;
			//				}
			//				
			//				m_focused_slot = GetFocusSlot(_x, _y);
			//
			//			if (m_focused_slot != 0 && AbleToUse(GetSkillID(m_focused_slot)) == false)
			//				m_focused_slot = NOT_SELECTED;
			//
			//			if (!NoPassive(m_focused_slot))
			//				m_focused_slot = NOT_SELECTED;
			//
			//			// set description
			//
			//				int id;
			//				if (focused_slot == 0 && m_selected_skillid != NOT_SELECTED)
			//					id = m_selected_skillid;
			//				else
			//					id = GetSkillID(focused_slot);
			//
			//				if (id != NOT_SELECTED)
			//				{
			//					Point point = GetIconPoint(focused_slot);
			//					g_descriptor_manager.Set(DID_SKILL, point.x, point.y, NULL, id, m_skill_guard_y);
			//				}
		}
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if (gpC_base->EventOccured() == true)
			return true;
		
		if (m_focused_slot != NOT_SELECTED)
		{
			if (m_focused_slot == 0 || /*(AbleToUse(GetSkillID(m_focused_slot))*/(IsEnableSkill(GetSkillID(m_focused_slot)) && NoPassive(m_focused_slot)))
				m_bl_pushed = true;
		}
		break;
		
	case M_LEFTBUTTON_UP:
		if (m_bl_pushed == true && m_focused_slot != NOT_SELECTED)
		{
			// select skill
			if (m_bl_open == true && m_focused_slot != 0)
			{
				m_selected_skillid = GetSkillID(m_focused_slot);
				gpC_base->SendMessage(UI_SELECT_SKILL, m_selected_skillid);
			}
			
			ToggleWindow();
		}
		if(m_bl_pushed)
		{
			m_bl_pushed = false;
			m_focused_slot = GetFocusSlot(_x, _y);
		}
		EMPTY_MOVE;
		break;
		
	case M_RIGHTBUTTON_DOWN:
		break;
		
	case M_RIGHTBUTTON_UP:
		g_descriptor_manager.Unset();
		m_selected_skillid = NOT_SELECTED;
		gpC_base->SendMessage(UI_CANCEL_SELECT_SKILL);
		break;
		
	case M_WHEEL_UP:
		ChangeSkillWheel(true);
		break;
		
	case M_WHEEL_DOWN:
		ChangeSkillWheel(false);
		break;
		
	}
	
	return true;
}

/*-----------------------------------------------------------------------------
- Show
- Skill Show.
-----------------------------------------------------------------------------*/
void C_VS_UI_SKILL::Show()
{
	//
	// Show Skill icon
	//
	// skill guard 위에 push/unpush button이 놓인다.
	// skill을 선택하면 그것이 button으로 대체된다.
	//
	// 선택된 skill이 제일 처음 출력된다. 그 line은 그것을 포함해서 SPREAD_X_MAX이다.
	//
	int i = 0;
	CSprite * p_sprite;
	data_id_t spr_id;
	
	// 선택된 skill을 출력한다.
	if (m_selected_skillid != NOT_SELECTED && g_pSkillAvailable->find( (ACTIONINFO) m_selected_skillid ) != g_pSkillAvailable->end())
	{
		spr_id = (*g_pSkillInfoTable)[(ACTIONINFO)m_selected_skillid].GetSpriteID();
		p_sprite = &m_C_spk[spr_id];
		
		if (p_sprite != NULL)
		{
			POINT point;
			point.x = m_skill_start_x+2; // 고정된 위치이다.
			point.y = m_skill_start_y+1;
			
			if (m_bl_pushed == true && m_focused_slot == 0)
			{
				point.x += 1;
				point.y += 1;
			}
			
			point.x += m_extra_icon_offset_x;
			point.y += m_extra_icon_offset_y;
			
			if (gpC_base->m_p_DDSurface_back->Lock())
			{
				// 사용할 수 없으면 빨간색으로..
				if(IsEnableSkill(m_selected_skillid) == false)
				{
					CSpriteSurface::SetEffect(CSpriteSurface::EFFECT_GRAY_SCALE);
					gpC_base->m_p_DDSurface_back->BltSpriteEffect(&point, p_sprite);
				}
				else if (AbleToUse(m_selected_skillid) == false)
					gpC_base->m_p_DDSurface_back->BltSpriteColor(&point, p_sprite, rgb_RED);
				else
				{
					gpC_base->m_p_DDSurface_back->BltSprite(&point, p_sprite);
					int percent = GetDelay(m_selected_skillid);
					if(percent)
					{
						Rect rect;
						RECT rt;
						S_SURFACEINFO surface_info;
						SetSurfaceInfo(&surface_info, gpC_base->m_p_DDSurface_back->GetDDSD());
						
						SetRect((RECT *)&rect, 0, 0, p_sprite->GetWidth(), p_sprite->GetHeight()*(percent)/100);
						rt.left = rect.x;
						rt.top = rect.y;
						rt.right = rect.x+rect.w;
						rt.bottom = rect.y+rect.h;
						
						if(rt.left < rt.right && rt.top < rt.bottom)
						{
							WORD * p_dest = (WORD *)surface_info.p_surface+point.x+rt.left;
							p_dest = (WORD *)((BYTE *)p_dest+(point.y+rt.top)*surface_info.pitch);
							
							p_sprite->BltColorClipHeight(p_dest, surface_info.pitch, &rt, rgb_RED);
						}
					}
				}
				
				gpC_base->m_p_DDSurface_back->Unlock();
			}
		}
	}
	else
	{
		if (m_bl_pushed == true && m_focused_slot == 0)
			m_etc_spk.Blt(m_skill_start_x+m_skill_guard_extra_offset, m_skill_start_y+m_skill_guard_extra_offset, SKILL_PUSHED_BUTTON);
		else
			m_etc_spk.Blt(m_skill_start_x+m_skill_guard_extra_offset, m_skill_start_y+m_skill_guard_extra_offset, SKILL_EMPTY_BUTTON);
	}
	
	//	if (m_selected_skillid != NOT_SELECTED && m_bl_pushed == true && m_focused_slot == 0)
	//		m_etc_spk.BltColor(m_skill_start_x+1, m_skill_start_y+1, SKILL_GUARD, rgb_GREEN);
	//	else
	//		m_etc_spk.Blt(m_skill_start_x, m_skill_start_y, SKILL_GUARD);
	
	if (m_bl_open == true)
	{
		// 선택된 skill을 제외하고 가능한 skill을 모두 출력한다.
		MSkillSet::SKILLID_MAP::iterator iNode = g_pSkillAvailable->begin();
		while (iNode != g_pSkillAvailable->end())
		{
			// skill의 id와 status
			SKILLID_NODE node = ((*iNode).second);
			ACTIONINFO id		= node.SkillID;
			
			spr_id = (*g_pSkillInfoTable)[id].GetSpriteID();
			
			//---------------------------------------
			// id를 알면 (*g_pSkillInfoTable)에서 
			// 그 id의 skill에 대한 정보를 얻을 수 있다.
			//---------------------------------------
			//logFile << "[" << id << "] " << (*g_pSkillInfoTable)[id].GetName()
			//		<< " = " << (int)status << endl;
			
			p_sprite = &m_C_spk[spr_id];
			
			if (p_sprite != NULL)
			{
				POINT point;
				point.x = m_skill_start_x-(m_skill_guard_x*((i+1)%SPREAD_X_MAX))+2;
				point.y = m_skill_start_y-(m_skill_guard_y*((i+1)/SPREAD_X_MAX))+1;
				
				if (m_bl_pushed == true && m_focused_slot == (i+1))
				{
					point.x += 1;
					point.y += 1;
				}
				
				POINT backup_pt = point;
				point.x += m_extra_icon_offset_x;
				point.y += m_extra_icon_offset_y;
				
				if (gpC_base->m_p_DDSurface_back->Lock())
				{
					// 사용할 수 없으면 빨간색으로..
					if(IsEnableSkill(id) == false)
					{
						CSpriteSurface::SetEffect(CSpriteSurface::EFFECT_GRAY_SCALE);
						gpC_base->m_p_DDSurface_back->BltSpriteEffect(&point, p_sprite);
					}
					else if ((*g_pSkillInfoTable)[id].IsPassive())
					{
						gpC_base->m_p_DDSurface_back->BltSpriteColorSet(&point, p_sprite, 315);
					}
					else if (AbleToUse(id) == false)
						gpC_base->m_p_DDSurface_back->BltSpriteColor(&point, p_sprite, rgb_RED);
					else
					{
						gpC_base->m_p_DDSurface_back->BltSprite(&point, p_sprite);
						int percent = GetDelay(id);
						if(percent)
						{
							Rect rect;
							RECT rt;
							S_SURFACEINFO surface_info;
							SetSurfaceInfo(&surface_info, gpC_base->m_p_DDSurface_back->GetDDSD());
							
							SetRect((RECT *)&rect, 0, 0, p_sprite->GetWidth(), p_sprite->GetHeight()*(percent)/100);
							rt.left = rect.x;
							rt.top = rect.y;
							rt.right = rect.x+rect.w;
							rt.bottom = rect.y+rect.h;
							
							if(rt.left < rt.right && rt.top < rt.bottom)
							{
								WORD * p_dest = (WORD *)surface_info.p_surface+point.x+rt.left;
								p_dest = (WORD *)((BYTE *)p_dest+(point.y+rt.top)*surface_info.pitch);
								
								p_sprite->BltColorClipHeight(p_dest, surface_info.pitch, &rt, rgb_RED);
							}
						}
					}
					
					gpC_base->m_p_DDSurface_back->Unlock();
				}
				
				point = backup_pt;
				
				if (m_bl_pushed == true && m_focused_slot == (i+1))
					m_etc_spk.BltColor(point.x-2, point.y-1, SKILL_GUARD, rgb_GREEN);
				//else if (m_selected_skillid != NOT_SELECTED && GetSkillID(i) == m_selected_skillid) // 그들 중 현재 선택된 거...
				//	m_etc_spk.BltColor(point.x-2, point.y-1, SKILL_GUARD, rgb_BLUE);
				else
					m_etc_spk.Blt(point.x-2, point.y-1, SKILL_GUARD);
				
				// print hotkey mark
				// 글자색이 많이 튄다는 이유로 고치랍니다.. by sigi
				const char hotkey_mark[8][4][10] = 
				{
					{"F9:1", "F9:2", "F9:3", "F9:4"},
					{"F10:1", "F10:2", "F10:3", "F10:4"}, 
					{"F11:1", "F11:2", "F11:3", "F11:4"}, 
					{"F12:1", "F12:2", "F12:3", "F12:4"},
					{"F5:1", "F5:2", "F5:3", "F5:4"},
					{"F6:1", "F6:2", "F6:3", "F6:4"},
					{"F7:1", "F7:2", "F7:3", "F7:4"},
					{"F8:1", "F8:2", "F8:3", "F8:4"},
				};
				
				int widthMark[8][4] =
				{
					26,
					26,
					26,
					26,
					32,
					32,
					32,
					32,
					32,
					32,
					32,
					32,
					32,
					32,
					32,
					32,
					26,
					26,
					26,
					26,
					26,
					26,
					26,
					26,
					26,
					26,
					26,
					26,
					26,
					26,
					26,
					26,
				};
				
				const int heightMark = 14;//g_GetStringHeight(hotkey_mark[0], gpC_base->m_item_desc_pi.hfont);
				
				// 검은색 박스 출력..
				if (gpC_base->m_p_DDSurface_back->Lock())
				{
					RECT rect;
					
					for (int hk=0; hk < HOTKEY_MAX; hk++)
					{
						for (int hl=0; hl < GRADE_MAX; hl++)
						{
							if (m_skill_hotkey_buf[hk][hl] == id)
							{
								rect.left = point.x;
								rect.top = point.y+24;
								rect.right = rect.left + widthMark[hk][hl];
								rect.bottom = rect.top+heightMark;
								
								DrawAlphaBox(&rect, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
							}
						}
					}
					
					gpC_base->m_p_DDSurface_back->Unlock();
				}
				
				g_FL2_GetDC();
				for (int hk=0; hk < HOTKEY_MAX; hk++)
				{
					for(int hl=0; hl < GRADE_MAX; hl++)
					{
						if (m_skill_hotkey_buf[hk][hl] == id)
						{
							//g_Print(point.x, point.y+24, hotkey_mark[hk]);
							COLORREF markColor = RGB(140, 140, 255);
							
							g_PrintColorStr(point.x+1, point.y+25, hotkey_mark[hk][hl], gpC_base->m_item_desc_pi, markColor);	
							
						}
					}
				}
				g_FL2_ReleaseDC();
			}
			
			// 다음
			iNode++;
			i++;
		}
	}
	/*
	if (focused_id != NOT_SELECTED && m_focus_slot != NOT_SELECTED)
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
	S_SURFACEINFO	surface_info;
	S_RECT			rect;
	SetSurfaceInfo(&surface_info, gpC_base->m_p_DDSurface_back->GetDDSD());
	
	  gpC_base->SelectFont(FONT_ITEM);
	  
		SetRect(rect, gpC_mouse_pointer->GetPointerX(), gpC_mouse_pointer->GetPointerY()-30, gC_font.GetStringWidth((*g_pSkillInfoTable)[focused_id].GetName())+10, 30);
		
		  filledRect(&surface_info, &rect, 0);
		  rectangle(&surface_info, rect.x, rect.y, rect.x+rect.w-1, rect.y+rect.h-1, LIGHT_MAGENTA);
		  
			gC_font.PrintString(&surface_info, (*g_pSkillInfoTable)[focused_id].GetName(), 
			rect.x+5, 
			rect.y+5, WHITE);
			
			  gpC_base->m_p_DDSurface_back->Unlock();
			  }
			  */
			  if(Timer() && GetSelectedSkillID() != NOT_SELECTED)
			  {
				  Point point = GetIconPoint(0);
				  g_descriptor_manager.Set(DID_SKILL, point.x, point.y, NULL, GetSelectedSkillID(), m_skill_guard_y);
			  }
			  //	else
			  //	{
			  //		g_descriptor_manager.Unset();
			  //	}
			  
			  //지뢰 설치 Progress Bar
			  if(gbl_mine_progress)
			  {
				  const MItem * p_item = g_pInventory->GetItem(C_VS_UI_INVENTORY::m_mine_grid_x, C_VS_UI_INVENTORY::m_mine_grid_y);
				  
				  if (p_item && 
					  (p_item->GetItemClass() == MItem::ITEM_CLASS_MINE && (GetSelectedSkillID() == SKILL_INSTALL_MINE || GetSelectedSkillID() == MINE_ANKLE_KILLER || GetSelectedSkillID() == MINE_POMZ || GetSelectedSkillID() == MINE_AP_C1 || GetSelectedSkillID() == MINE_DIAMONDBACK || GetSelectedSkillID() == MINE_SWIFT_EX || GetSelectedSkillID() == MINE_SIDEWINDER || GetSelectedSkillID() == MINE_COBRA)) ||
					  (p_item->GetItemClass() == MItem::ITEM_CLASS_BOMB_MATERIAL && p_item->GetItemType() > 4 && GetSelectedSkillID() == SKILL_MAKE_MINE) ||
					  (p_item->GetItemClass() == MItem::ITEM_CLASS_BOMB_MATERIAL && p_item->GetItemType() < 5 && GetSelectedSkillID() == SKILL_MAKE_BOMB)
					  ) // Item이 있다.
				  {
					  if(!gpC_mouse_pointer->RightMousePushed())
						  gC_vs_ui.EndInstallMineProgress();
					  
					  if(C_VS_UI_INVENTORY::Timer())
					  {
						  gpC_base->SendMessage(UI_ITEM_USE, p_item->GetID(), 0, (MItem *)p_item);
						  
						  gC_vs_ui.EndInstallMineProgress();
					  }
					  else
					  {
						  TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
						  
						  int item_x = m_skill_start_x+4;
						  int item_y = m_skill_start_y+36-1;
						  
						  if(gpC_base->m_p_DDSurface_back->Lock())
						  {					
							  C_VS_UI_INVENTORY::m_pC_mine_progress_spk->BltLocked(item_x, item_y, C_VS_UI_INVENTORY::INVENTORY_BAR_BACK);
							  
							  Rect rect;
							  rect.Set(0, 0, C_VS_UI_INVENTORY::m_pC_mine_progress_spk->GetWidth(C_VS_UI_INVENTORY::INVENTORY_BAR)*(GetTickCount() - gi_mine_progress)/C_VS_UI_INVENTORY::m_dw_millisec, C_VS_UI_INVENTORY::m_pC_mine_progress_spk->GetHeight(C_VS_UI_INVENTORY::INVENTORY_BAR));
							  C_VS_UI_INVENTORY::m_pC_mine_progress_spk->BltLockedClip(item_x, item_y, rect, C_VS_UI_INVENTORY::INVENTORY_BAR);
							  gpC_base->m_p_DDSurface_back->Unlock();
						  }
					  }
				  }
				  //		else
				  //			gC_vs_ui.EndInstallMineProgress();				  
			  }			  
#ifndef _LIB
			  extern bool gbl_info_show;
			  
			  if(gbl_info_show)
			  {
				  char skill_size_buf[100];
				  sprintf(skill_size_buf, "available skill count = %d", g_pSkillAvailable->size());
				  g_Print(5, TEXT_LINE(3), skill_size_buf);
				  
				  sprintf(skill_size_buf, "skill focused slot = %d", m_focused_slot);
				  g_Print(5, TEXT_LINE(4), skill_size_buf);
				  
				  sprintf(skill_size_buf, "selected skill = %d", m_selected_skillid);
				  g_Print(5, TEXT_LINE(5), skill_size_buf);
				  
				  if (gpC_base->m_p_DDSurface_back->Lock())
				  {
					  S_SURFACEINFO	surface_info;
					  SetSurfaceInfo(&surface_info, gpC_base->m_p_DDSurface_back->GetDDSD());
					  
					  //		rectangle(&surface_info, x, y, x+w-1, y+h-1, GREEN);
					  
					  gpC_base->m_p_DDSurface_back->Unlock();
				  }
			  }
#endif
			  
}

//-----------------------------------------------------------------------------
// C_VS_UI_SKILL::Timer
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_SKILL::Timer(bool reset)
{
	if(reset)
	{
		//		m_timer = false;
		m_dw_prev_tickcount = GetTickCount();
	}
	else if(m_dw_prev_tickcount+m_dw_millisec >= GetTickCount())
	{
		return true;
		//		m_timer = true;
		//		m_dw_prev_tickcount = GetTickCount();
	}
	//	else m_timer = false;
	
	return false;
}

bool findSkillAvailable(ACTIONINFO id)
{
	MSkillSet::SKILLID_MAP::iterator iNode = g_pSkillAvailable->begin();	
	
	while( iNode != g_pSkillAvailable->end() )
	{
		if( ((*iNode).second).SkillID == id)
			return true;
		iNode++;
	}
	return false;
}

void	C_VS_UI_SKILL::ResetSkillSet()
{
	// 아아.. 여기서 자꾸 버그가 난다..
	// 결국... Love Chain 만 막을까낭-_-;
	ACTIONINFO id = (ACTIONINFO) m_selected_skillid;

	switch( id )
	{
	case SKILL_LOVE_CHAIN :
		if( !findSkillAvailable ( id ) )
		{
			m_selected_skillid = NOT_SELECTED;
		}
		break;
	}

//	if(! findSkillAvailable( (ACTIONINFO) GetSkillID(m_focused_slot) ))
//		m_focused_slot = 0;
//
//	if(! findSkillAvailable( (ACTIONINFO) m_selected_skillid ) && m_selected_skillid != NOT_SELECTED )
//		m_selected_skillid = NOT_SELECTED;
}

void	C_VS_UI_SKILL::ResetHotkey()
{
	// 5개 이하는 단축키 리셋 안한다.
//	if( g_pSkillAvailable->size() < 5 )
		return;

	for (int hk=0; hk < HOTKEY_MAX; hk++)
	{
		for(int hl=0; hl < GRADE_MAX; hl++)
		{
			ACTIONINFO id = (ACTIONINFO)m_skill_hotkey_buf[hk][hl];
			if(! findSkillAvailable ( id ) )
			{
				// disable 된 skill 이면 단축키를 지워준다.
				if( m_selected_skillid == m_skill_hotkey_buf[hk][hl])
					m_selected_skillid  = NOT_SELECTED;			
				
				m_skill_hotkey_buf[hk][hl] = NOT_SELECTED;
			}			
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_LEVELUP::C_VS_UI_LEVELUP
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_LEVELUP::C_VS_UI_LEVELUP()
{
	g_RegisterWindow(this);
	
	AttrPin(true);
	
	int _y;
	
	m_image_spk.Open(SPK_LEVELUP);
	_y = 425;
	
	Set(2, _y, m_image_spk.GetWidth(LVU_GUARD_SLAYER), m_image_spk.GetHeight(LVU_GUARD_SLAYER));
}

//-----------------------------------------------------------------------------
// C_VS_UI_LEVELUP::~C_VS_UI_LEVELUP
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_LEVELUP::~C_VS_UI_LEVELUP()
{
	g_UnregisterWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_LEVELUP::Show
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_LEVELUP::Show()
{
	RECT rect = { x+9, y+9, x+9+36, y+9+36 };
	gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		if (m_bl_pushed && m_bl_focused)
		{
			m_image_spk.BltLocked(x+10, y+10, LVU_BUTTON);
		}
		else
		{
			m_image_spk.BltLocked(x+9, y+9, LVU_BUTTON);
		}
		
		m_image_spk.BltLocked(x, y, LVU_GUARD_VAMPIRE);
		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_LEVELUP::IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_LEVELUP::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_LEVELUP::Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_LEVELUP::Start()
{
	PI_Processor::Start();
	
	gpC_window_manager->AppearWindow(this);
	
	m_bl_pushed = false;
	m_bl_focused = false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_LEVELUP::Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_LEVELUP::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_LEVELUP::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_LEVELUP::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	
	switch (message)
	{
	case M_MOVING:
		m_bl_focused = true;
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if (m_bl_focused)
			m_bl_pushed = true;
		break;
		
	case M_LEFTBUTTON_UP:
		if (m_bl_pushed)
		{
			// run
			if(!gC_vs_ui.IsRunningCharInfo())
				gC_vs_ui.RunInfo(C_VS_UI_INFO::CHARACTER_INFO_MODE);
			
			if (!gbl_vampire_interface)
				gpC_base->SendMessage(UI_FINISH_LEVELUP_BUTTON);				
		}
		
		m_bl_pushed = false;
		break;
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_PARTY::C_VS_UI_REQUEST_PARTY
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_REQUEST_PARTY::C_VS_UI_REQUEST_PARTY(const char *name, DWORD timer)
{
	g_RegisterWindow(this);
	
	AttrPin(true);
	
	int _y;
	if (gbl_vampire_interface)
	{
		m_image_spk.Open(SPK_LEVELUP);
		_y = 425;
	}
	else
	{
		m_image_spk.Open(SPK_LEVELUP);
		_y = 382;
	}
	
	Set(2, _y, m_image_spk.GetWidth(REQUEST_GUARD_SLAYER), m_image_spk.GetHeight(REQUEST_GUARD_SLAYER));
	
	m_name = name;

	if(g_pParty)
		m_type = (g_pParty->GetSize()>0)?REQUEST:INVITE;
	else
		m_type = REQUEST;
	
	//timer
	m_dw_timer_tickcount = timer;
	Timer(true);
	
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_PARTY::~C_VS_UI_REQUEST_PARTY
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_REQUEST_PARTY::~C_VS_UI_REQUEST_PARTY()
{
	g_UnregisterWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_PARTY::Show
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_REQUEST_PARTY::Show()
{
	if(!Timer())
		gpC_base->SendMessage(UI_FINISH_REQUEST_PARTY_BUTTON, TRUE);
	
	RECT rect = { x+9, y+9, x+9+36, y+9+36 };
	gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		if (m_bl_pushed && m_bl_focused)
		{
			m_image_spk.BltLocked(x+10, y+10, PARTY_REQUEST+m_type);
		}
		else
		{
			m_image_spk.BltLocked(x+9, y+9, PARTY_REQUEST+m_type);
		}
		
		if(gbl_vampire_interface)
			m_image_spk.BltLocked(x, y, REQUEST_GUARD_VAMPIRE);
		else
			m_image_spk.BltLocked(x, y, REQUEST_GUARD_SLAYER);
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_PARTY::IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_REQUEST_PARTY::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_PARTY::Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_REQUEST_PARTY::Start()
{
	PI_Processor::Start();
	
	gpC_window_manager->AppearWindow(this);
	
	m_bl_pushed = false;
	m_bl_focused = false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_PARTY::Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_REQUEST_PARTY::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_PARTY::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_REQUEST_PARTY::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	
	switch (message)
	{
	case M_MOVING:
		if(_x > x+16 && _y > y+15 && _x < x+16+m_image_spk.GetWidth(PARTY_REQUEST) && _y < y+15+m_image_spk.GetHeight(PARTY_REQUEST))
		{
			m_bl_focused = true;
			if(g_pParty->GetSize() > 0)
				g_descriptor_manager.Set(DID_INFO, x, y, (*g_pGameStringTable)[UI_STRING_MESSAGE_REQUEST_PARTY].GetString(),0,0);
			else
				g_descriptor_manager.Set(DID_INFO, x, y, (*g_pGameStringTable)[UI_STRING_MESSAGE_INVITE_PARTY].GetString(),0,0);
		}
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if (m_bl_focused)
			m_bl_pushed = true;
		break;
		
	case M_LEFTBUTTON_UP:
		if (m_bl_pushed)
		{
			// run
			//				gC_vs_ui.RunCharacterInfo();
			
			//				if (!gbl_vampire_interface)
			gC_vs_ui.RunPartyAsk(m_name.c_str(), m_type);
			gpC_base->SendMessage(UI_FINISH_REQUEST_PARTY_BUTTON, FALSE);
		}
		
		m_bl_pushed = false;
		break;
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_PARTY::Timer
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_REQUEST_PARTY::Timer(bool reset)
{
	if(reset)
	{
		m_dw_prev_tickcount = GetTickCount();
	}
	else if(m_dw_prev_tickcount+m_dw_timer_tickcount >= GetTickCount())
	{
		return true;
	}
	
	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_DIE::C_VS_UI_REQUEST_DIE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_REQUEST_DIE::C_VS_UI_REQUEST_DIE(DWORD timer)
{
	g_RegisterWindow(this);
	
	AttrPin(true);
	
	int _y;
	if (gbl_vampire_interface)
	{
		m_image_spk.Open(SPK_LEVELUP);
		_y = 425;
	}
	else
	{
		m_image_spk.Open(SPK_LEVELUP);
		_y = 382;
	}
	
	Set(2, _y, m_image_spk.GetWidth(RESURRECT_GUARD_SLAYER), m_image_spk.GetHeight(RESURRECT_GUARD_SLAYER));
	
	//timer
	m_dw_timer_tickcount = timer;
	Timer(true);
	
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_DIE::~C_VS_UI_REQUEST_DIE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_REQUEST_DIE::~C_VS_UI_REQUEST_DIE()
{
	g_UnregisterWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_DIE::Show
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_REQUEST_DIE::Show()
{
	if(!Timer())
		gpC_base->SendMessage(UI_FINISH_REQUEST_DIE_BUTTON, TRUE);
	
	RECT rect = { x+9, y+9, x+9+36, y+9+36 };
	gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
	
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		if (m_bl_pushed && m_bl_focused)
		{
			if(gbl_vampire_interface)
				m_image_spk.BltLocked(x+10, y+10, RESURRECT_VAMPIRE);
			else
				m_image_spk.BltLocked(x+10, y+10, RESURRECT_SLAYER);
		}
		else
		{
			if(gbl_vampire_interface)
				m_image_spk.BltLocked(x+9, y+9, RESURRECT_VAMPIRE);
			else
				m_image_spk.BltLocked(x+9, y+9, RESURRECT_SLAYER);
		}
		
		if(gbl_vampire_interface)
			m_image_spk.BltLocked(x, y, RESURRECT_GUARD_VAMPIRE);
		else
			m_image_spk.BltLocked(x, y, RESURRECT_GUARD_SLAYER);
		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_DIE::IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_REQUEST_DIE::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_DIE::Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_REQUEST_DIE::Start()
{
	PI_Processor::Start();
	
	gpC_window_manager->AppearWindow(this);
	
	m_bl_pushed = false;
	m_bl_focused = false;

	EMPTY_MOVE;
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_DIE::Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_REQUEST_DIE::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_DIE::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_REQUEST_DIE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	
	switch (message)
	{
	case M_MOVING:
		if(_x > x+16 && _y > y+15 && _x < x+16+m_image_spk.GetWidth(PARTY_REQUEST) && _y < y+15+m_image_spk.GetHeight(PARTY_REQUEST))
		{
			m_bl_focused = true;
			if(gbl_vampire_interface)
				g_descriptor_manager.Set(DID_INFO, x, y, (*g_pGameStringTable)[UI_STRING_MESSAGE_RESURRECTION].GetString(),0,0);
			else
				g_descriptor_manager.Set(DID_INFO, x, y, (*g_pGameStringTable)[UI_STRING_MESSAGE_EVACUATION].GetString(),0,0);
		}
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if (m_bl_focused)
			m_bl_pushed = true;
		break;
		
	case M_LEFTBUTTON_UP:
		if (m_bl_pushed)
		{
			// run
			//				gC_vs_ui.RunCharacterInfo();
			
			//				if (!gbl_vampire_interface)
			gpC_base->SendMessage(UI_FINISH_REQUEST_DIE_BUTTON, FALSE);
		}
		m_bl_pushed = false;
		break;
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_DIE::Timer
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_REQUEST_DIE::Timer(bool reset)
{
	if(reset)
	{
		m_dw_prev_tickcount = GetTickCount();
	}
	else if(m_dw_prev_tickcount+m_dw_timer_tickcount >= GetTickCount())
	{
		return true;
	}
	
	return false;
}



//-----------------------------------------------------------------------------
// C_VS_UI_PARTY_MANAGER::C_VS_UI_PARTY_MANAGER
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_PARTY_MANAGER::C_VS_UI_PARTY_MANAGER()
{
	g_RegisterWindow(this);
	AttrPin(true);
	
	m_pC_button_group = new ButtonGroup(this);
	m_pC_bottom_button_group = new ButtonGroup(this);
	
	int deny_button_x = 12, help_button_x = 48, close_button_x = 82, button_y = 4;
	
	int away_x = 77, away_y = 100, away_gap = 79;
	
	m_p_image_spk = new C_SPRITE_PACK;

	if (gbl_vampire_interface)
	{
		m_p_image_spk->Open(SPK_VAMPIRE_PARTY);
	}
	else
	{
		m_p_image_spk->Open(SPK_SLAYER_PARTY);
	}

	m_p_face_spk = NULL;
	m_p_large_face_spk = NULL;
	
	Set(RESOLUTION_X-m_p_image_spk->GetWidth(PARTY_WINDOW), 130, m_p_image_spk->GetWidth(PARTY_WINDOW), m_p_image_spk->GetHeight(PARTY_WINDOW));
	
	m_pC_bottom_button_group->Add(new C_VS_UI_EVENT_BUTTON(deny_button_x, button_y, m_p_image_spk->GetWidth(DENY_BUTTON), m_p_image_spk->GetHeight(DENY_BUTTON), DENY_ID, this, DENY_BUTTON));
	m_pC_bottom_button_group->Add(new C_VS_UI_EVENT_BUTTON(help_button_x, button_y, m_p_image_spk->GetWidth(HELP_BUTTON), m_p_image_spk->GetHeight(HELP_BUTTON), HELP_ID, this, HELP_BUTTON));
	m_pC_bottom_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_x, button_y, m_p_image_spk->GetWidth(CLOSE_BUTTON), m_p_image_spk->GetHeight(CLOSE_BUTTON), CLOSE_ID, this, CLOSE_BUTTON));
	
	int alpha_button_offset_x, alpha_button_offset_y;
	if(gbl_vampire_interface)
	{
		alpha_button_offset_x = 98; alpha_button_offset_y = 2;
	}
	else
	{
		alpha_button_offset_x = 98; alpha_button_offset_y = 2;
	}
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_button_offset_x, alpha_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));
	
	m_bl_show_face_large = false;
	m_away_pushed = false;
	m_away_button_focused;
	m_away_focused = -1;
	m_old_away_focused = -1;
	m_dw_show_face_large_time = 3000;
	
#ifndef _LIB
	if(g_pParty != NULL)
		delete g_pParty;
	
	g_pParty = new MParty;
	PARTY_INFO *temp_party;
	
	temp_party = new PARTY_INFO;
	temp_party->bMale = false;
	temp_party->hairStyle = W_HAIR1;
	temp_party->HP = 0;
	temp_party->Name = "TestName";
	temp_party->zoneID = 2024;
	temp_party->zoneX = 150;
	temp_party->zoneY = 150;
	temp_party->guildID = 101;
	g_pParty->AddMember(temp_party);
#endif	
}

//-----------------------------------------------------------------------------
// C_VS_UI_PARTY_MANAGER::~C_VS_UI_PARTY_MANAGER
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_PARTY_MANAGER::~C_VS_UI_PARTY_MANAGER()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::PARTY, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::PARTY, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::PARTY, Rect(x, y, w, h));
	
	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_button_group);
	DeleteNew(m_pC_bottom_button_group);
	
	DeleteNew(m_p_image_spk);
	DeleteNew(m_p_face_spk);
	DeleteNew(m_p_large_face_spk);
	
	for(int i = 0; i < m_vp_face.size(); i++)
	{
		if(m_vp_face[i] != NULL)
			DeleteNew(m_vp_face[i]);
	}

	m_vp_face.clear();
	m_v_face_name.clear();
	
}

//-----------------------------------------------------------------------------
// C_VS_UI_PARTY_MANAGER::Timer
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_PARTY_MANAGER::Timer(bool reset)
{
	static DWORD prev_time = GetTickCount();

	if(reset)
	{
		prev_time = GetTickCount();
	}
	else
	{
		if(prev_time + m_dw_show_face_large_time < GetTickCount())
			return true;
	}
	return false;
	
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PARTY_MANAGER::Process()
{
	m_pC_button_group->Process();
}


void	C_VS_UI_PARTY_MANAGER::RefreshFaceImage()
{
	for(int i = 0; i < m_vp_face.size(); i++)
	{
		if(m_vp_face[i] != NULL)
			DeleteNew(m_vp_face[i]);
	}
	m_vp_face.clear();
	m_v_face_name.clear();
}

//-----------------------------------------------------------------------------
// C_VS_UI_PARTY_MANAGER::Show
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_PARTY_MANAGER::Show()
{
	Rect window_rect;
	h = window_default_height + window_gap*(g_pParty->GetSize()+1);
	SetRect((RECT *)&window_rect, 0, 0, m_p_image_spk->GetWidth(), h+window_bottom_gap);
	h += m_p_image_spk->GetHeight(PARTY_WINDOW_BOTTOM);
	
	int name_x = 75, name_y = window_default_height + 27;
	
	COLORREF name_color;
	if(gbl_vampire_interface)
		name_color = RGB(255, 255, 255);
	else
		name_color = RGB(255, 255, 255);
	
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		if(GetAttributes()->alpha)
		{
			RECT alpha_rect = {x+4, y+8, x+111, y+window_rect.h};
			
			if(gbl_vampire_interface)
				DrawAlphaBox(&alpha_rect, 0, 0, 2, g_pUserOption->ALPHA_DEPTH);
			else
				DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
			
			m_p_image_spk->BltLockedClip(x, y, window_rect, PARTY_WINDOW_ALPHA);
		}
		else
			m_p_image_spk->BltLockedClip(x, y, window_rect, PARTY_WINDOW);
		
		m_p_image_spk->BltLocked(x, y+window_rect.h - window_bottom_gap, PARTY_WINDOW_BOTTOM);
		
		POINT point;
		std::string name;
		bool bl_female;
		int spriteID = 0;
		
		
		for(int i = 0; i < 1+g_pParty->GetSize(); i++)
		{
			CSprite *p_guild_mark = NULL;
			
			if(i == 0)
			{
				name = g_char_slot_ingame.sz_name;
				bl_female = g_char_slot_ingame.bl_female;
				if(gbl_vampire_interface)
				{
					if (bl_female)
						spriteID = 6+1;
					else
						spriteID = 6+0;
				}
				else
				{
					if (bl_female)
						spriteID = g_char_slot_ingame.woman_info.face-W_FACE1+3; 
					else
						spriteID = g_char_slot_ingame.man_info.face-M_FACE1;
				}
				
				//길드 마크 로딩
				// 작은거
				p_guild_mark = g_pGuildMarkManager->GetGuildMarkSmall(g_char_slot_ingame.GUILD_ID);
				
				if (p_guild_mark==NULL)
				{		
					//-------------------------------------------------
					// file에 있는지 본다.
					//-------------------------------------------------
					g_pGuildMarkManager->LoadGuildMark(g_char_slot_ingame.GUILD_ID);
					
					//-------------------------------------------------
					// file에서 load되었는지 다시 체크
					//-------------------------------------------------
					p_guild_mark = g_pGuildMarkManager->GetGuildMarkSmall(g_char_slot_ingame.GUILD_ID);
				}
			}
			else if(g_pParty->GetMemberInfo(i-1) != NULL)
			{
				name = g_pParty->GetMemberInfo(i-1)->Name;
				bl_female = !g_pParty->GetMemberInfo(i-1)->bMale;
				if(gbl_vampire_interface)
				{
					if (bl_female)
						spriteID = 6+1;
					else
						spriteID = 6+0;
				}
				else
				{
					if (bl_female)
						spriteID = g_pParty->GetMemberInfo(i-1)->hairStyle-W_FACE1+3; 
					else
						spriteID = g_pParty->GetMemberInfo(i-1)->hairStyle-M_FACE1;
				}
				//길드 마크 로딩
				// 작은거
				p_guild_mark = g_pGuildMarkManager->GetGuildMarkSmall(g_pParty->GetMemberInfo(i-1)->guildID);
				
				if (p_guild_mark==NULL)
				{		
					//-------------------------------------------------
					// file에 있는지 본다.
					//-------------------------------------------------
					g_pGuildMarkManager->LoadGuildMark(g_pParty->GetMemberInfo(i-1)->guildID);
					
					//-------------------------------------------------
					// file에서 load되었는지 다시 체크
					//-------------------------------------------------
					p_guild_mark = g_pGuildMarkManager->GetGuildMarkSmall(g_pParty->GetMemberInfo(i-1)->guildID);
				}
			}
			
			
			for(int find = 0; find < m_v_face_name.size(); find++)
			{
				if(strcmp(m_v_face_name[find].c_str(), name.c_str()) == 0)break;
			}
			
			if(find == m_v_face_name.size())	// 이름 자체가 등록이 안되있으면 search&load
			{
				C_SPRITE_PACK *temp_spk = NULL;
				if(g_pProfileManager->HasProfile(name.c_str()))
				{
					const char *filename = g_pProfileManager->GetFilename(name.c_str());
					temp_spk = new C_SPRITE_PACK(filename);
					
					m_vp_face.push_back(temp_spk);
					m_v_face_name.push_back(name);
				}
				
				if(g_pProfileManager->HasProfileNULL(name.c_str()))
				{
					if(temp_spk)
					{
						DeleteNew(temp_spk);
						temp_spk = NULL;
					}
					m_vp_face.push_back(temp_spk);
					m_v_face_name.push_back(name);
				}
			}
			
			point.x = x + 9;
			point.y = y + window_default_height + i*window_gap +1;
			
			m_p_image_spk->BltLocked(point.x -1, point.y -1, FACE_GRID);
			m_p_image_spk->BltLocked(point.x +33, point.y +24, NAME_GRID);
			if(i == 0)
			{
				Rect rect;
				rect.Set(0, 0, (m_p_image_spk->GetWidth(HPBAR))*g_char_slot_ingame.HP/g_char_slot_ingame.HP_MAX, m_p_image_spk->GetHeight(HPBAR));
				m_p_image_spk->BltLockedClip(point.x +34, point.y +25, rect, HPBAR);
			}
			else
			{
				PARTY_INFO *info = g_pParty->GetMemberInfo(i-1);
				if(info)
				{
					Rect rect;
					rect.Set(0, 0, (m_p_image_spk->GetWidth(HPBAR))*info->HP/info->MaxHP, m_p_image_spk->GetHeight(HPBAR));
					m_p_image_spk->BltLockedClip(point.x +34, point.y +25, rect, HPBAR);
				}
			}
			
			if(g_pParty->GetSize() > 0 && (i == 0 || i != 0 && g_pParty->IsKickAvailableTime()))
			{
				if(m_away_focused == i && m_away_button_focused)
				{
					if(m_away_pushed)
						m_p_image_spk->BltLocked(x +away_x, y +away_y +window_default_height + i*window_gap, AWAY_BUTTON_HILIGHTED_PUSHED);
					else
						m_p_image_spk->BltLocked(x +away_x, y +away_y +window_default_height + i*window_gap, AWAY_BUTTON_HILIGHTED);
				}
				else
					m_p_image_spk->BltLocked(x +away_x, y +away_y +window_default_height + i*window_gap, AWAY_BUTTON);
			}
			
			if(m_v_face_name.empty() || find == m_v_face_name.size() || (find < m_v_face_name.size() && m_vp_face[find] == NULL))
			{	// default face
				if(spriteID < 0 || spriteID >= m_p_face_spk->GetSize())spriteID = 0;
				
				if(i == 0 && g_char_slot_ingame.HP == 0 || i != 0 && g_pParty->GetMemberInfo(i-1)->HP == 0)
					m_p_face_spk->BltLockedColor(point.x, point.y, spriteID, rgb_RED);
				else
					m_p_face_spk->BltLocked(point.x, point.y, spriteID);
				
			}
			else
			{
				//user face
				if(i == 0 && g_char_slot_ingame.HP == 0 || i != 0 && g_pParty->GetMemberInfo(i-1)->HP == 0)
				{
					m_vp_face[find]->BltLockedColor(point.x, point.y, 0, rgb_RED);
				}
				else
					m_vp_face[find]->BltLocked(point.x, point.y, 0);
			}
			
			//길드 마크 찍기
			if (p_guild_mark!=NULL)
			{			
				POINT guild_point = { point.x+33, point.y+2 };
				gpC_base->m_p_DDSurface_back->BltSprite(&guild_point, p_guild_mark);
			}
			
			if(Timer() && i == m_away_focused)
			{
				if(m_v_face_name.empty() || find == m_v_face_name.size() || (find < m_v_face_name.size() && m_vp_face[find] == NULL))
				{	// default face
					if(spriteID < 0 || spriteID >= m_p_face_spk->GetSize())spriteID = 0;
					
					m_p_large_face_spk->BltLocked(point.x-65, point.y, spriteID);
					
				}
				else
				{
					//user face
					m_vp_face[find]->BltLocked(point.x-65, point.y, 1);
				}
				
			}
		}
		m_pC_button_group->Show();	
		m_pC_bottom_button_group->Show();
		
		gpC_base->m_p_DDSurface_back->Unlock();
		if(Timer() && m_away_focused != -1)
		{
			RECT rect;
			point.x = x + 9-65;
			point.y = y + window_default_height + m_away_focused*window_gap +1;
			SetRect(&rect, point.x-2, point.y-2, point.x+55+1, point.y+70+1);
			gpC_base->m_p_DDSurface_back->DrawRect(&rect, 0);
			gpC_base->m_p_DDSurface_back->HLine(point.x-1, point.y-1, 57, GRAY);
			gpC_base->m_p_DDSurface_back->VLine(point.x-1, point.y-1, 72, GRAY);
			gpC_base->m_p_DDSurface_back->HLine(point.x-1, point.y+70, 57, DARKGRAY);
			gpC_base->m_p_DDSurface_back->VLine(point.x+55, point.y-1, 72, DARKGRAY);
			//			gpC_base->m_p_DDSurface_back->HLine(point.x-1, point.y+139, 1, GRAY);
			//			gpC_base->m_p_DDSurface_back->VLine(point.x+110, point.y-1, 1, GRAY);
		}
	}
	
	g_FL2_GetDC();
	g_PrintColorStr(x+name_x-g_GetStringWidth(g_char_slot_ingame.sz_name.c_str(), gpC_base->m_party_name_pi.hfont)/2, y+name_y, g_char_slot_ingame.sz_name.c_str(), gpC_base->m_party_name_pi, name_color);
	
	for(int i = 0; i < g_pParty->GetSize(); i++)
	{
		if(g_pParty->GetMemberInfo(i) != NULL)
			g_PrintColorStr(x+name_x-g_GetStringWidth(g_pParty->GetMemberInfo(i)->Name.GetString(), gpC_base->m_party_name_pi.hfont)/2, y+name_y+window_gap*(i+1), g_pParty->GetMemberInfo(i)->Name.GetString(), gpC_base->m_party_name_pi, name_color);
	}
	m_pC_button_group->ShowDescription();
	m_pC_bottom_button_group->ShowDescription();
	g_FL2_ReleaseDC();
}

//-----------------------------------------------------------------------------
// C_VS_UI_PARTY_MANAGER::IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_PARTY_MANAGER::IsPixel(int _x, int _y)
{
	if(m_p_image_spk == NULL)return false;
	int size;
	if(g_pParty == NULL)
		size = window_gap;
	else
		size = (g_pParty->GetSize() +1)*window_gap;
	
	bool re = (_x >= x && _x < x +m_p_image_spk->GetWidth() && _y >= y && _y < y + window_default_height + size + m_p_image_spk->GetHeight(PARTY_WINDOW_BOTTOM));
	if(re == false)
		m_away_focused = -1;
	
	return re;
}

//-----------------------------------------------------------------------------
// C_VS_UI_PARTY_MANAGER::Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_PARTY_MANAGER::Start()
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::PARTY));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::PARTY);
	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}
	
	PI_Processor::Start();
	
	gpC_window_manager->AppearWindow(this);
	
	for(int i = 0; i < m_vp_face.size(); i++)
	{
		if(m_vp_face[i] != NULL)
			DeleteNew(m_vp_face[i]);
	}
	m_vp_face.clear();
	m_v_face_name.clear();
	
	DeleteNew(m_p_image_spk);
	m_p_image_spk = new C_SPRITE_PACK;
	if (gbl_vampire_interface)
	{
		m_p_image_spk->Open(SPK_VAMPIRE_PARTY);
	}
	else
	{
		m_p_image_spk->Open(SPK_SLAYER_PARTY);
	}
	
	DeleteNew(m_p_face_spk);
	DeleteNew(m_p_large_face_spk);
	m_p_face_spk = new C_SPRITE_PACK(SPK_PARTY_FACE);
	m_p_large_face_spk = new C_SPRITE_PACK(SPK_FACE);
	
	//	std::string profile_filename;
	//	profile_filename = SPK_PROFILE;
	//	profile_filename += g_char_slot_ingame.sz_name;
	//	profile_filename += ".bmp";
	
	//	CSpriteSurface temp_surface;
	
	//	if(temp_surface.InitFromBMP(profile_filename.c_str()) == true)
	//	{
	//		CDirectDrawSurface *p_temp_surface = new CDirectDrawSurface;
	//		p_temp_surface->InitOffsurface(73, 89);
	//		RECT dest_rect = {0, 0, p_temp_surface->GetWidth(), p_temp_surface->GetHeight()};
	//		RECT src_rect = {0, 0, temp_surface.GetWidth(), temp_surface.GetHeight()};
	//		p_temp_surface->Blt(&dest_rect, &temp_surface, &src_rect);
	//		m_vp_face_surface.push_back(p_temp_surface);
	//		m_v_face_name.push_back(g_char_slot_ingame.sz_name);
	//	}
	
	//	temp_surface.ReleaseSurface();
	//	CSpritePack temp_spk;
	//
	//	if(gbl_vampire_interface)
	//	{
	//		temp_spk.Init(2, CDirectDraw::Is565());
	//		for(int k = 0; k < 2; k++)
	//		{
	//			temp_spk.LoadFromFileSprite(k, 6+k, SPK_FACE, SPKI_FACE);
	//		}
	//	}
	//	else
	//	{
	//		temp_spk.Init(6, CDirectDraw::Is565());
	//		for(int k = 0; k < 6; k++)
	//		{
	//			temp_spk.LoadFromFileSprite(k, k, SPK_FACE, SPKI_FACE);
	//		}
	//	}
	
	EMPTY_MOVE;
}

//-----------------------------------------------------------------------------
// C_VS_UI_PARTY_MANAGER::Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PARTY_MANAGER::Finish()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::PARTY, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::PARTY, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::PARTY, Rect(x, y, w, h));
	
	DeleteNew(m_p_image_spk);
	DeleteNew(m_p_face_spk);
	DeleteNew(m_p_large_face_spk);

	for(int i = 0; i < m_vp_face.size(); i++)
	{
		if(m_vp_face[i] != NULL)
			DeleteNew(m_vp_face[i]);
	}
	m_vp_face.clear();
	m_v_face_name.clear();
	
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_PARTY_MANAGER::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_PARTY_MANAGER::MouseControl(UINT message, int _x, int _y)
{
	const static char*	m_buttoninfo_string[2] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_EXPEL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_SECEDE].GetString(),
	};
	
	Window::MouseControl(message, _x, _y);
	
	_x-=x;_y-=y;//창이동 가능하게 하기 위한
	
	bool re = true;
	
	if(g_pParty)
		re = m_pC_bottom_button_group->MouseControl(message, _x, _y-(window_default_height + window_gap*(g_pParty->GetSize()+1)));
	re &= m_pC_button_group->MouseControl(message, _x, _y);
	
	if(g_pParty != NULL)// && g_pParty->GetSize() > 0)
		switch (message)
	{
		case M_MOVING:
			m_away_focused = (_y-window_default_height)/window_gap;
			if(m_away_focused > g_pParty->GetSize() || m_away_focused < 0)
				m_away_focused = -1;
			
			if(m_away_focused != m_old_away_focused)
				Timer(true);
			if(!m_away_pushed)
				m_old_away_focused = m_away_focused;
			if(m_away_focused != -1 && m_p_image_spk && _x >= away_x && _x <= away_x + m_p_image_spk->GetWidth(AWAY_BUTTON) && (_y-window_default_height)%window_gap >= away_y && (_y-window_default_height)%window_gap <= away_y +m_p_image_spk->GetHeight(AWAY_BUTTON))
				m_away_button_focused = true;
			else
				m_away_button_focused = false;
			
			// 캐릭터 얼굴 디스크립션 (zoneid, x, y)
			if(m_p_face_spk && _x >= 9 && _x <= 9 +m_p_face_spk->GetWidth() && (_y-window_default_height)%window_gap >= 1 && (_y-window_default_height)%window_gap <= 1 +m_p_face_spk->GetHeight())
			{
				if(m_away_focused > 0 && m_away_focused < g_pParty->GetSize()+1)
				{
					PARTY_INFO *info = g_pParty->GetMemberInfo(m_away_focused-1);
					if(info != NULL)
					{
						static char zonename[20];
						static char zonexy[10];
						static S_DEFAULT_HELP_STRING party_string;
						party_string.sz_main_str = zonename;
						party_string.sz_sub_str = zonexy;
						wsprintf(zonename, "%s", g_pZoneTable->Get(info->zoneID)->Name.GetString());
						wsprintf(zonexy, (*g_pGameStringTable)[UI_STRING_MESSAGE_ZONEINFO_XY].GetString(), info->zoneX, info->zoneY);
						g_descriptor_manager.Set(DID_HELP, x+9, y+1+window_gap*m_away_focused+window_default_height, (void *)&party_string,0,0);
					}
				}
			}
			// 탈퇴 추방 디스크립션
			else if(m_p_image_spk && _x >= away_x && _x <= away_x + m_p_image_spk->GetWidth(AWAY_BUTTON) && (_y-window_default_height)%window_gap >= away_y && (_y-window_default_height)%window_gap <= away_y +m_p_image_spk->GetHeight(AWAY_BUTTON))
			{
				if(!g_pParty->IsKickAvailableTime() && m_away_focused != 0 || g_pParty->GetSize() == 0)
					m_away_focused = -1;
				//				if(m_away_focused != -1 && m_away_focused == m_old_away_focused)m_away_pushed = true;
				if(m_away_focused != -1)
				{
					g_descriptor_manager.Set(DID_INFO, x+away_x, y+away_y+window_gap*m_away_focused+window_default_height, (void *)m_buttoninfo_string[(m_away_focused==0)?0:1],0,0);
				}
			}
			// 길드 마크 디스크립션
			else if(_x >= 42 && _x <= 42+20 && (_y-window_default_height)%window_gap >= 2 && (_y-window_default_height)%window_gap <= 2+20)
			{
				if(m_away_focused > 0 && m_away_focused < g_pParty->GetSize()+1)
				{
					PARTY_INFO *info = g_pParty->GetMemberInfo(m_away_focused-1);
					if(info != NULL)
					{
						if(info->guildID > 0)
						{
							GUILD_INFO *p_guild_info = g_pGuildInfoMapper->Get(info->guildID);
							if(p_guild_info != NULL)
							{
								const char *guild_name = p_guild_info->GetGuildName();
								if(guild_name != NULL)
									g_descriptor_manager.Set(DID_INFO, x+33, y+2+window_gap*m_away_focused+window_default_height, (void *)guild_name,0,0);
								guild_name = NULL;
							}
							p_guild_info = NULL;
						}
					}
				}
			}
			else
			{
				//				m_away_focused = -1;
				m_away_pushed = false;
			}
			break;
			
		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			if(m_away_focused != -1 && m_p_image_spk && _x >= away_x && _x <= away_x + m_p_image_spk->GetWidth(AWAY_BUTTON) && (_y-window_default_height)%window_gap >= away_y && (_y-window_default_height)%window_gap <= away_y +m_p_image_spk->GetHeight(AWAY_BUTTON))
			{
				m_old_away_focused = m_away_focused;
				m_away_pushed = true;
			}
			else
			if (gpC_mouse_pointer->GetPickUpItem() == false && re)
			{
				MoveReady();
				SetOrigin(_x, _y);
				break;
			}
			break;
			
		case M_LEFTBUTTON_UP:
			if(m_away_focused != -1 && m_away_pushed == true && m_p_image_spk && _x >= away_x && _x <= away_x + m_p_image_spk->GetWidth(AWAY_BUTTON) && (_y-window_default_height)%window_gap >= away_y && (_y-window_default_height)%window_gap <= away_y +m_p_image_spk->GetHeight(AWAY_BUTTON))
				Run(AWAY_I+m_away_focused);
			m_old_away_focused = -1;
			m_away_pushed = false;
			break;
	}
	
	if(message == M_RIGHTBUTTON_DOWN || message == M_RIGHTBUTTON_UP)
		return false;
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_PARTY_MANAGER::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_PARTY_MANAGER::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char*	m_buttoninfo_string[8] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SET_INVITE_DENY_MODE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CANCEL_INVITE_DENY_MODE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SET_JOIN_DENY_MODE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CANCEL_JOIN_DENY_MODE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP_PARTY_MANAGER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_PARTY_MANAGER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
	};
	
	int desc_select = 0;
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			desc_select = 6;
		else
			desc_select = 7;
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_buttoninfo_string[desc_select],0,0);
	}
	else if (m_p_image_spk->IsPixel(gpC_mouse_pointer->GetX()-x-p_button->x, gpC_mouse_pointer->GetY()-y-p_button->y-(window_default_height + window_gap*(g_pParty->GetSize()+1)), p_button->m_image_index))
	{
		if(p_button->GetID() == DENY_ID)
		{
			if(g_pParty->GetSize() > 0)
				desc_select += 2;
			if(g_pParty->IsAcceptInvite() == false)
				desc_select++;
		}
		else
			desc_select = 4+p_button->GetID()-HELP_ID;
		
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y+(window_default_height + window_gap*(g_pParty->GetSize()+1)), (void *)m_buttoninfo_string[desc_select],0,0);
	}	
}

//-----------------------------------------------------------------------------
// C_VS_UI_PARTY_MANAGER::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_PARTY_MANAGER::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(m_p_image_spk == NULL)return;
	
	
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	}
	else if(p_button->GetFocusState() && p_button->GetPressState() && m_p_image_spk->IsPixel(gpC_mouse_pointer->GetX()-x-p_button->x, gpC_mouse_pointer->GetY()-y-p_button->y-(window_default_height + window_gap*(g_pParty->GetSize()+1)), p_button->m_image_index))
	{
		if(p_button->GetID() == DENY_ID && g_pParty->IsAcceptInvite() == false)
			m_p_image_spk->BltLocked(p_button->x+x, p_button->y+y+(window_default_height + window_gap*(g_pParty->GetSize()+1)), DENY_BUTTON_HILIGHTED_PUSHED);
		else
			m_p_image_spk->BltLocked(p_button->x+x, p_button->y+y+(window_default_height + window_gap*(g_pParty->GetSize()+1)), p_button->m_image_index+1);
	}
	else
	{
		if(p_button->GetID() == DENY_ID && g_pParty->IsAcceptInvite() == false)
			m_p_image_spk->BltLocked(p_button->x+x, p_button->y+y+(window_default_height + window_gap*(g_pParty->GetSize()+1)), DENY_BUTTON_HILIGHTED);
		else
			m_p_image_spk->BltLocked(p_button->x+x, p_button->y+y+(window_default_height + window_gap*(g_pParty->GetSize()+1)), p_button->m_image_index);
	}
	
	/*
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
	S_SURFACEINFO	surfaceinfo;
	SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());
	
	  rectangle(&surfaceinfo, p_button, WHITE);
	  
		gpC_base->m_p_DDSurface_back->Unlock();
}*/
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PARTY_MANAGER::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_PARTY_MANAGER::Run(id_t id)
{
	switch (id)
	{
	case DENY_ID:
		if(g_pParty->IsAcceptInvite())
			g_pParty->RefuseInvite();
		else
			g_pParty->AcceptInvite();
		EMPTY_MOVE;
		break;
		break;
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_PARTY_MANAGER);
		break;
		
	case HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::PARTY_MANAGER);
		m_pC_bottom_button_group->UnacquireMouseFocus();
		m_pC_button_group->UnacquireMouseFocus();
		break;
		
	case AWAY_I:
		gpC_base->SendMessage(UI_AWAY_PARTY, -1);
		break;
		
	case AWAY_0:
	case AWAY_1:
	case AWAY_2:
	case AWAY_3:
	case AWAY_4:
		if(id-AWAY_0 < g_pParty->GetSize())
			gpC_base->SendMessage(UI_AWAY_PARTY, id-AWAY_0);
		break;
		
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;
		
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_INFO
//
//-----------------------------------------------------------------------------
C_VS_UI_INFO::C_VS_UI_INFO()
{
	m_pC_learn_grade_skill_confirm = NULL;
#ifndef _LIB
	(*g_pSkillManager)[SKILLDOMAIN_SWORD].SetDomainLevel(0);
	(*g_pSkillManager)[SKILLDOMAIN_SWORD].SetDomainExp(990);
	(*g_pSkillManager)[SKILLDOMAIN_BLADE].SetDomainLevel(30);
	(*g_pSkillManager)[SKILLDOMAIN_BLADE].SetDomainExp(400000);
	(*g_pSkillManager)[SKILLDOMAIN_GUN].SetDomainLevel(60);
	(*g_pSkillManager)[SKILLDOMAIN_HEAL].SetDomainLevel(100);
	(*g_pSkillManager)[SKILLDOMAIN_HEAL].SetDomainExp(3129);
	(*g_pSkillManager)[SKILLDOMAIN_ENCHANT].SetDomainLevel(20);
	(*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].SetDomainLevel(21);
	
	MSkillDomain& swordDomain = (*g_pSkillManager)[SKILLDOMAIN_SWORD];
	
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_DOUBLE_IMPACT );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_TRIPLE_SLASHER );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_THUNDER_SPARK );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_RAINBOW_SLASHER );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_SNAKE_COMBO );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_CROSS_COUNTER );
	swordDomain.SetNewSkill();
	swordDomain.LearnSkill( SKILL_SOUL_CHAIN );
	swordDomain.SetNewSkill();

	MSkillDomain& etcDomain = (*g_pSkillManager)[SKILLDOMAIN_ETC];

	etcDomain.SetNewSkill();
	etcDomain.LearnSkill( SKILL_SOUL_CHAIN );
	
	//swordDomain.LearnSkill ( SKILL_

	(*g_pSkillInfoTable)[SKILL_DOUBLE_IMPACT].SetExpLevel( 00 );
	(*g_pSkillInfoTable)[SKILL_TRIPLE_SLASHER].SetExpLevel( 20 );
	(*g_pSkillInfoTable)[SKILL_THUNDER_SPARK].SetExpLevel( 40 );
	(*g_pSkillInfoTable)[SKILL_RAINBOW_SLASHER].SetExpLevel( 60 );
	(*g_pSkillInfoTable)[SKILL_SNAKE_COMBO].SetExpLevel( 80 );
	(*g_pSkillInfoTable)[SKILL_CROSS_COUNTER].SetExpLevel( 100 );
	(*g_pSkillInfoTable)[SKILL_SOUL_CHAIN].SetExpLevel( 100 );
	
	(*g_pSkillInfoTable)[SKILL_DOUBLE_IMPACT].SetSkillExp( 149 );
	(*g_pSkillInfoTable)[SKILL_TRIPLE_SLASHER].SetSkillExp( 19999 );
	(*g_pSkillInfoTable)[SKILL_THUNDER_SPARK].SetSkillExp( 40 );
	(*g_pSkillInfoTable)[SKILL_RAINBOW_SLASHER].SetSkillExp( 60 );
	(*g_pSkillInfoTable)[SKILL_SNAKE_COMBO].SetSkillExp( 80 );
	(*g_pSkillInfoTable)[SKILL_CROSS_COUNTER].SetSkillExp( 100 );
	(*g_pSkillInfoTable)[SKILL_SOUL_CHAIN].SetSkillExp( 1 );
	
	(*g_pSkillManager)[SKILLDOMAIN_GUN].SetNewSkill();
	(*g_pSkillManager)[SKILLDOMAIN_GUN].LearnSkill(SKILL_FAST_RELOAD);
	
	(*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].SetNewSkill();
	(*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].LearnSkill(MAGIC_ACID_TOUCH);
#endif
	m_focused_skill_id = -1;
	
	if(gbl_vampire_interface)
	{
		m_rt_char_box.Set(20, 15, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::CHAR_BOX), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::CHAR_BOX));
		m_rt_tab.x = 37;
		m_rt_tab.y = -8;
		m_rt_tab.w = gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::TAB_CHAR);
		m_rt_tab.h = gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::TAB_CHAR);
		SetRect(&m_rcSkillDesciption, 43, 82+4, 297, 234+4);
	}
	else
	{
		m_rt_char_box.Set(20, 22, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::CHAR_BOX), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::CHAR_BOX));
		m_rt_tab.x = 37;
		m_rt_tab.y = -6;
		m_rt_tab.w = gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::TAB_CHAR);
		m_rt_tab.h = gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::TAB_CHAR);
		SetRect(&m_rcSkillDesciption, 35, 127+4, 289, 279+4);
	}
	
	g_RegisterWindow(this);
	
	if(gbl_vampire_interface)
		Set(RESOLUTION_X/2 - 336/2, RESOLUTION_Y/2 - 335/2, 336, 335);
	else
		Set(RESOLUTION_X/2 - 321/2, RESOLUTION_Y/2 - 330/2, 321, 330);	
	
	AttrTopmost(false);
	AttrPin(true);
	
	g_RegisterWindow(this);
	
	//스크롤바
	m_pC_char_scroll_bar = NULL;
	m_pC_skill_scroll_bar = NULL;
	m_pC_grade1_scroll_bar = NULL;
	if(gbl_vampire_interface)
	{
		m_pC_char_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(300, 140, -1, 137));
		m_pC_skill_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(298, 80+4, -1, 141));
		//m_pC_grade1_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(291, 125+4, -1, 141));
	}
	else
	{
		m_pC_char_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(295, 145, -1, 145));
		m_pC_skill_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(291, 125+4, -1, 141));
		//m_pC_grade1_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(282, 125+3, -1, 144));
	}
	
	//close버튼 좌표 세팅
	int close_x = w-24, close_y = h-19;
	int help_x = w-24-20, help_y = h-19;
	int alpha_x = 6, alpha_y = h-21;
	
	if(gbl_vampire_interface)
	{
		close_x -= 5;
		close_y -= 7;
		help_x -=5;
		help_y -= 7;
		alpha_x += 5;
		alpha_y -= 5;
	}	
	
	//공통버튼
	m_pC_common_button_group = new ButtonGroup(this);
	m_pC_common_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BUTTON_CLOSE), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BUTTON_CLOSE), CLOSE_ID, this,C_GLOBAL_RESOURCE:: BUTTON_CLOSE));
	m_pC_common_button_group->Add(new C_VS_UI_EVENT_BUTTON(help_x, help_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BUTTON_HELP), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BUTTON_HELP), HELP_ID, this, C_GLOBAL_RESOURCE::BUTTON_HELP));
	m_pC_common_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_x, alpha_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));
	
	//charinfo 버튼
	int plus_x = 60, plus_y = 104, plus_gap = 20;
	m_pC_char_button_group = new ButtonGroup(this);
	m_pC_char_button_group->Add(new C_VS_UI_EVENT_BUTTON(plus_x, plus_y+plus_gap*1, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BONUS_BUTTON), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BONUS_BUTTON), STR_PLUS_ID, this, C_GLOBAL_RESOURCE::BONUS_BUTTON));
	m_pC_char_button_group->Add(new C_VS_UI_EVENT_BUTTON(plus_x, plus_y+plus_gap*2, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BONUS_BUTTON), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BONUS_BUTTON), DEX_PLUS_ID, this, C_GLOBAL_RESOURCE::BONUS_BUTTON));
	m_pC_char_button_group->Add(new C_VS_UI_EVENT_BUTTON(plus_x, plus_y+plus_gap*3, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BONUS_BUTTON), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BONUS_BUTTON), INT_PLUS_ID, this, C_GLOBAL_RESOURCE::BONUS_BUTTON));
	
	//skillinfo 버튼
	m_pC_skill_button_group = new ButtonGroup(this);
	
	if(gbl_vampire_interface)
	{
		const int domain_x1 = 20, domain_x2 = 69, domain_x3 = 117, domain_x4 = 164, domain_x5 = 217, domain_x6 = 264, domain_y = 22;
		m_pC_skill_button_group->Add(new C_VS_UI_EVENT_BUTTON(domain_x1, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DM_POISON), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DM_POISON), POISON_ID, this, C_GLOBAL_RESOURCE::DM_POISON));
		m_pC_skill_button_group->Add(new C_VS_UI_EVENT_BUTTON(domain_x2, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DM_ACID), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DM_ACID), ACID_ID, this, C_GLOBAL_RESOURCE::DM_ACID));
		m_pC_skill_button_group->Add(new C_VS_UI_EVENT_BUTTON(domain_x3, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DM_CURSE), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DM_CURSE), CURSE_ID, this, C_GLOBAL_RESOURCE::DM_CURSE));
		m_pC_skill_button_group->Add(new C_VS_UI_EVENT_BUTTON(domain_x4, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DM_SUMMON), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DM_SUMMON), SUMMON_ID, this, C_GLOBAL_RESOURCE::DM_SUMMON));
		m_pC_skill_button_group->Add(new C_VS_UI_EVENT_BUTTON(domain_x5, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DM_BLOOD), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DM_BLOOD), BLOOD_ID, this, C_GLOBAL_RESOURCE::DM_BLOOD));
		m_pC_skill_button_group->Add(new C_VS_UI_EVENT_BUTTON(domain_x6, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DM_INNATE), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DM_INNATE), INNATE_ID, this, C_GLOBAL_RESOURCE::DM_INNATE));
	}
	else
	{
		const int domain_x1 = 12, domain_x2 = 61, domain_x3 = 109, domain_x4 = 156, domain_x5 = 203, domain_x6 = 256, domain_y = 26;
		m_pC_skill_button_group->Add(new C_VS_UI_EVENT_BUTTON(domain_x1, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DM_BLADE), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DM_BLADE), BLADE_ID, this, C_GLOBAL_RESOURCE::DM_BLADE));
		m_pC_skill_button_group->Add(new C_VS_UI_EVENT_BUTTON(domain_x2, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DM_SWORD), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DM_SWORD), SWORD_ID, this, C_GLOBAL_RESOURCE::DM_SWORD));
		m_pC_skill_button_group->Add(new C_VS_UI_EVENT_BUTTON(domain_x3, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DM_GUN), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DM_GUN), GUN_ID, this, C_GLOBAL_RESOURCE::DM_GUN));
		m_pC_skill_button_group->Add(new C_VS_UI_EVENT_BUTTON(domain_x4, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DM_HEAL), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DM_HEAL), HEAL_ID, this, C_GLOBAL_RESOURCE::DM_HEAL));
		m_pC_skill_button_group->Add(new C_VS_UI_EVENT_BUTTON(domain_x5, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DM_ENCHANT), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DM_ENCHANT), ENCHANT_ID, this, C_GLOBAL_RESOURCE::DM_ENCHANT));
		m_pC_skill_button_group->Add(new C_VS_UI_EVENT_BUTTON(domain_x6, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DM_TOTAL), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DM_TOTAL), TOTAL_ID, this, C_GLOBAL_RESOURCE::DM_TOTAL));
	}

	// gradeskillinfo 버튼
	m_pC_grade1_button_group = new ButtonGroup(this);
	
	if(gbl_vampire_interface)
	{
		int domain_y=98;
		int startX=53;
		//m_pC_grade1_button_group->Add(new C_VS_UI_EVENT_BUTTON(startX, domain_y, m_pC_info_spk->GetWidth(GRADE_RITTER), m_pC_info_spk->GetHeight(GRADE_RITTER), RITTER_ID, this, GRADE_RITTER));
		//startX+=m_pC_info_spk->GetWidth(GRADE_RITTER);
		m_pC_grade1_button_group->Add(new C_VS_UI_EVENT_BUTTON(startX, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_REICHSRITTER), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::GRADE_REICHSRITTER), REICHSRITTER_ID, this, C_GLOBAL_RESOURCE::GRADE_REICHSRITTER));
		startX+=gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_REICHSRITTER);
		m_pC_grade1_button_group->Add(new C_VS_UI_EVENT_BUTTON(startX, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_BARONET), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::GRADE_BARONET), BARONET_ID, this, C_GLOBAL_RESOURCE::GRADE_BARONET));
		startX+=gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_BARONET);
		m_pC_grade1_button_group->Add(new C_VS_UI_EVENT_BUTTON(startX, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_PREYHER), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::GRADE_PREYHER), PREYHER_ID, this, C_GLOBAL_RESOURCE::GRADE_PREYHER));
		startX+=gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_PREYHER);
		m_pC_grade1_button_group->Add(new C_VS_UI_EVENT_BUTTON(startX, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_GRAF), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::GRADE_GRAF), GRAF_ID, this, C_GLOBAL_RESOURCE::GRADE_GRAF));

		int button_x=43,button_y=151,gap_x=34,gap_y=50;
		
		int Button_ID = (int)GRADE1_BUTTON1_ID;

		if(C_VS_UI_SKILL::m_C_spk[0].IsInit())
		{
			for(int i=0;i<8;i++)
			{
				m_pC_grade1_button_group->Add(new C_VS_UI_EVENT_BUTTON(
					button_x+(C_VS_UI_SKILL::m_C_spk[0].GetWidth()*(i&0x3))+gap_x*(i&0x3),
					button_y+(C_VS_UI_SKILL::m_C_spk[0].GetHeight()*(i/4))+gap_y*(i/4),
					C_VS_UI_SKILL::m_C_spk[0].GetWidth(),C_VS_UI_SKILL::m_C_spk[0].GetWidth(), 
					Button_ID+i, this, i));
			}
		}
	}
	else
	{
		int domain_y=99;
		int startX=47;
		//m_pC_grade1_button_group->Add(new C_VS_UI_EVENT_BUTTON(startX, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_PIVATE), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::GRADE_PIVATE), PIVATE_ID, this, GRADE_PIVATE));
		//startX+=gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_PIVATE);
		m_pC_grade1_button_group->Add(new C_VS_UI_EVENT_BUTTON(startX, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_SERENT), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::GRADE_SERENT), SERENT_ID, this, C_GLOBAL_RESOURCE::GRADE_SERENT));
		startX+=gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_SERENT);
		m_pC_grade1_button_group->Add(new C_VS_UI_EVENT_BUTTON(startX, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_FEACEL), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::GRADE_FEACEL), FEACEL_ID, this, C_GLOBAL_RESOURCE::GRADE_FEACEL));
		startX+=gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_FEACEL);
		m_pC_grade1_button_group->Add(new C_VS_UI_EVENT_BUTTON(startX, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_LITENA), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::GRADE_LITENA), LITENA_ID, this, C_GLOBAL_RESOURCE::GRADE_LITENA));
		startX+=gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_LITENA);
		m_pC_grade1_button_group->Add(new C_VS_UI_EVENT_BUTTON(startX, domain_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_KAINEL), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::GRADE_KAINEL), KAINEL_ID, this, C_GLOBAL_RESOURCE::GRADE_KAINEL));

		int button_x=39,button_y=146,gap_x=33,gap_y=48;
		int Button_ID = GRADE1_BUTTON1_ID;


		if(C_VS_UI_SKILL::m_C_spk[0].IsInit())
		{
			for(int i=0;i<8;i++)
			{
				m_pC_grade1_button_group->Add(new C_VS_UI_EVENT_BUTTON(
					button_x+(C_VS_UI_SKILL::m_C_spk[0].GetWidth()*(i&0x3))+gap_x*(i&0x3),
					button_y+(C_VS_UI_SKILL::m_C_spk[0].GetHeight()*(i/4))+gap_y*(i/4),
					C_VS_UI_SKILL::m_C_spk[0].GetWidth(),C_VS_UI_SKILL::m_C_spk[0].GetWidth(), 
					Button_ID+i, this, i));			
			}
		}
	}
	
	if(gbl_vampire_interface)
	{
		m_skill_domain = SKILLDOMAIN_VAMPIRE;
		m_iDomain = POISON_ID;
		m_pC_skill_scroll_bar->SetPosMax(((*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetSkillStepList((SKILL_STEP)(m_iDomain-POISON_ID+SKILL_STEP_VAMPIRE_POISON)))->size()-7);

		//m_iGrade = RITTER_ID;
		m_iGrade = ((g_char_slot_ingame.GRADE-1)/5)+RITTER_ID;
		m_iGrade=max(REICHSRITTER_ID,m_iGrade);
		m_iGrade=min(m_iGrade,GRAF_ID);
	}
	else
	{
		int level = -1;
		for(int i = 0; i < 5; i++)
		{
			int level_temp = (*g_pSkillManager)[(SKILLDOMAIN)i].GetDomainLevel();
			if(level_temp > level)
			{
				level = level_temp;
				m_skill_domain = (SKILLDOMAIN)i;
				m_iDomain = BLADE_ID+i;
				m_pC_skill_scroll_bar->SetPosMax(((*g_pSkillManager)[m_skill_domain].GetSkillStepList((SKILL_STEP)(SKILL_STEP_APPRENTICE)))->size()-7);
				//				m_pC_skill_scroll_bar->SetPosMax((*g_pSkillManager)[(SKILLDOMAIN)i].GetSize()-7);
			}
		}
		//m_iGrade = PIVATE_ID;
		m_iGrade = (g_char_slot_ingame.GRADE-1)/5+PIVATE_ID;
		m_iGrade=max(SERENT_ID,m_iGrade);
		m_iGrade=min(m_iGrade,KAINEL_ID);
	}
	
	Run(m_iGrade);	
	// profile
	m_p_face = NULL;
	
	RefreshImage();
}

//-----------------------------------------------------------------------------
// C_VS_UI_INFO
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_INFO::~C_VS_UI_INFO()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::INFO, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::INFO, Rect(x, y, w, h));
	
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
	
	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_grade1_button_group);
	DeleteNew(m_pC_char_scroll_bar);
	DeleteNew(m_pC_skill_scroll_bar);	
	DeleteNew(m_pC_common_button_group);
	DeleteNew(m_pC_char_button_group);
	DeleteNew(m_pC_skill_button_group);
	DeleteNew(m_pC_learn_grade_skill_confirm);
}

//-----------------------------------------------------------------------------
// C_VS_UI_INFO::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_INFO::CheckGradeSkillIDList()
{
	m_grade_skill_id.clear();
	int total=g_pRankBonusTable->GetSize();
	
	int gradeID=gbl_vampire_interface ? m_iGrade - RITTER_ID : m_iGrade - PIVATE_ID;
	
	for(int i=0;i<total;i++)
	{
		if(gbl_vampire_interface)
		{
			if((*g_pRankBonusTable)[i].IsVampireSkill())
			{
				int skillLevel=(*g_pRankBonusTable)[i].GetLevel();
				if(skillLevel >= gradeID*5 && skillLevel < (gradeID+1)*5)
					m_grade_skill_id.push_back(i);
			}
		} else
		{
			if((*g_pRankBonusTable)[i].IsSlayerSkill())
			{
				int skillLevel=(*g_pRankBonusTable)[i].GetLevel();
				if(skillLevel >= gradeID*5 && skillLevel < (gradeID+1)*5)
					m_grade_skill_id.push_back(i);
			}
		}
	}
	m_draw_grade_skill_mark = m_grade_skill_id.size();
}

//-----------------------------------------------------------------------------
// C_VS_UI_INFO::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------

void	C_VS_UI_INFO::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char* m_info_button_string[23] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_CHARACTER_INFO_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP_CHARACTER_INFO_WINDOW].GetString(),
			
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_BLADE_INFO].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_SWORD_INFO].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_GUN_INFO].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HEAL_INFO].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ENCHANT_INFO].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALL_INFO].GetString(),
			
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_POISON_INFO].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ACID_INFO].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_CURSE_INFO].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_SUMMON_INFO].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_BLOOD_INFO].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ESSENCE_INFO].GetString(),
			
		(*g_pGameStringTable)[UI_STRING_MESSAGE_UP_STR].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_UP_DEX].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_UP_INT].GetString(),
			
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_MAGIC_INFO_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_SKILL_INFO_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP_MAGIC_INFO_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP_SKILL_INFO_WINDOW].GetString(),
	};

	char* m_grade_skill_description_string[RANK_BONUS_MAX] =
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_CRITICAL_10].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_DEFENSE_5].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_TOHIT_5].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_HP_10].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_MP_15].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_DAMAGE_3].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_ATTACKSPEED_15].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_ENCHANT_DAMAGE_10_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_HEAL_DAMAGE_10_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_RESISTANCE_ACID_15_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_RESISTANCE_BLOODY_15_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_RESISTANCE_CURSE_15_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_RESISTANCE_POISON_15_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DECREASE_MP_EXHAUSTION_10_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_HP_STEAL_2_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_MP_STEAL_2_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_LUCKY_2].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_HP_20].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_DEFENSE_10].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_PROTECTION_10].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_ATTACKSPEED_15].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DECREASE_HP_EXHAUSTION_10_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_NEAR_ATTACK_DAMAGE_10_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_HP_RESTORE_SPEED_15_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_DAMAGE_STORM_20_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_RANGE_STORM_5_BY_5].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_SUCCESS_RATIO_POISON_10_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_SUCCESS_RATIO_ACID_10_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_SUCCESS_RATIO_CURSE_10_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_SUCCESS_RATIO_BLOODY_10_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_SUCCESS_RATIO_INNATE_10_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_SUCCESS_RATIO_SUMMON_10_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_KEEP_TIME_ACID_SWAMP_20_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_KEEP_TIME_PARALYZE_20_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_KEEP_TIME_DARKNESS_30_PERCENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INCREASE_RANGE_DARKNESS_5_BY_5].GetString(),
	};
	
	// 툴팁
	if(GetAttributes()->alpha && p_button->GetID() == ALPHA_ID)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_info_button_string[18],0,0);
	if(p_button->GetID() == CLOSE_ID && m_info_mode == SKILL_INFO_MODE)//m_skill_info_mode)
	{
		if(gbl_vampire_interface)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_info_button_string[19],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_info_button_string[20],0,0);
	}
	else if(p_button->GetID() == HELP_ID && m_info_mode == SKILL_INFO_MODE) //m_skill_info_mode)
	{
		if(gbl_vampire_interface)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_info_button_string[21],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_info_button_string[22],0,0);
	}
	else if(p_button->GetID() >= PIVATE_ID && p_button->GetID() <= GRADE1_BUTTON8_ID)
	{
		const static char *grade_desc[3] = 
		{
			NULL,
			NULL,
			NULL,
		};
		grade_desc[1]=NULL;
		int id = p_button->GetID() - GRADE1_BUTTON1_ID;
		int MyGrade = gbl_vampire_interface ? m_iGrade - RITTER_ID : m_iGrade - PIVATE_ID;
		if( id < 0 || id >= m_draw_grade_skill_mark || g_pRankBonusTable == NULL || 
			g_pRankBonusTable->GetSize() <= m_grade_skill_id[id] ) 
			return;

		grade_desc[1] = m_grade_skill_description_string[(*g_pRankBonusTable)[m_grade_skill_id[id]].GetType()];
		if(grade_desc[1] != NULL)
		{
			grade_desc[0] = (*g_pRankBonusTable)[m_grade_skill_id[id]].GetName();			
			switch((*g_pRankBonusTable)[m_grade_skill_id[id]].GetStatus())
			{
			case RankBonusInfo::STATUS_NULL :
				if((g_char_slot_ingame.GRADE-1)/5 >= MyGrade)
				{
					// 지금 배울 수 있습니다.
					grade_desc[2] = (*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_LEARN_SKILL_NOW].GetString();
				} else
				{
					// 아직 배울 수 없습니다.
					grade_desc[2] = (*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_LEARN_SKILL_YET].GetString();
				}
				break;
			case RankBonusInfo::STATUS_CANNOT_LEARN :
				grade_desc[2] = (*g_pGameStringTable)[UI_STRING_MESSAGE_NEVER_CANNOT_LEARN_SKILL].GetString();
				//      배울 수 없습니다.
				break;
			case RankBonusInfo::STATUS_LEARNED :
				//     이미 배운 스킬입니다.
				grade_desc[2] = (*g_pGameStringTable)[UI_STRING_MESSAGE_ALREADY_LEARNED_SKILL].GetString();
				break;
			}
			g_descriptor_manager.Set(DID_HELP, p_button->x+x, p_button->y+y, (void *)grade_desc,0,0);			
		}			
	} else
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_info_button_string[p_button->GetID()],0,0);
}


//-----------------------------------------------------------------------------
// C_VS_UI_INFO::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_INFO::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	CSpriteSurface::SetEffect(CSpriteSurface::EFFECT_GRAY_SCALE);
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	}
	//Close버튼
	else if(p_button->GetID() == CLOSE_ID || p_button->GetID() == HELP_ID)
	{
		gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-5, y+p_button->y-5, C_GLOBAL_RESOURCE::BUTTON_CLOSE_BACK);
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
		}
		else
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}
		
	}
	else if((p_button->GetID() >= BLADE_ID && p_button->GetID() <= TOTAL_ID && m_iDomain != p_button->GetID()) || 
		(p_button->GetID() >= POISON_ID && p_button->GetID() <= INNATE_ID && m_iDomain != p_button->GetID()) ||
		(p_button->GetID() >= RITTER_ID && p_button->GetID() <= GRAF_ID && m_iGrade != p_button->GetID()) ||
		(p_button->GetID() >= PIVATE_ID && p_button->GetID() <= KAINEL_ID && m_iGrade != p_button->GetID()))
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y-1, p_button->m_image_index+1);
			else
			{
				Rect rect;
				rect.Set(0, 0, gpC_global_resource->m_pC_info_spk->GetWidth(p_button->m_image_index+1), 
					gpC_global_resource->m_pC_info_spk->GetHeight(p_button->m_image_index+1)-2);
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+p_button->x, y+p_button->y+1, rect,p_button->m_image_index+1);
			}
		}
		else
		{
			gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y+1, p_button->m_image_index);
		}
	}
	else if(p_button->GetID() == STR_PLUS_ID || p_button->GetID() == DEX_PLUS_ID ||	p_button->GetID() == INT_PLUS_ID)
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
		}
		else
			gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
	} else if(p_button->GetID() >= GRADE1_BUTTON1_ID && p_button->GetID() <= GRADE1_BUTTON8_ID)
	{
		// 계급 스킬 버튼들
		int now_id=p_button->GetID() - GRADE1_BUTTON1_ID;

		if(now_id >= 0 && now_id<m_draw_grade_skill_mark)
		{
			POINT pt={p_button->x+x,p_button->y+y};
			const RankBonusInfo& RankBonus = (*g_pRankBonusTable)[m_grade_skill_id[now_id]];
			int MyGrade = gbl_vampire_interface ? m_iGrade - RITTER_ID : m_iGrade - PIVATE_ID;

			if(p_button->GetFocusState())
			{	
				if(gbl_vampire_interface)
					gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-15,y+p_button->y-15,C_GLOBAL_RESOURCE::VAMPIRE_GRADE_MARK_EDGE);
				else
					gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-15,y+p_button->y-15,C_GLOBAL_RESOURCE::SLAYER_GRADE_MARK_EDGE);

				// 클릭 하였을때					
				switch(RankBonus.GetStatus())
				{
				case RankBonusInfo::STATUS_NULL :
					if((g_char_slot_ingame.GRADE-1)/5 >= MyGrade)		// 초록색 						
					{
						if(p_button->GetPressState())
						{
							pt.y++;
							pt.x++;	
						}
						gpC_base->m_p_DDSurface_back->BltSpriteColor(&pt,&C_VS_UI_SKILL::m_C_spk[RankBonus.GetSkillIconID()],rgb_GREEN);
					}
					else
						gpC_base->m_p_DDSurface_back->BltSpriteEffect(&pt,&C_VS_UI_SKILL::m_C_spk[RankBonus.GetSkillIconID()]);
					break;
				case RankBonusInfo::STATUS_CANNOT_LEARN :
					gpC_base->m_p_DDSurface_back->BltSpriteDarkness(&pt,&C_VS_UI_SKILL::m_C_spk[RankBonus.GetSkillIconID()],1);
					break;
				case RankBonusInfo::STATUS_LEARNED :					
					gpC_base->m_p_DDSurface_back->BltSpriteColorSet(&pt,&C_VS_UI_SKILL::m_C_spk[RankBonus.GetSkillIconID()],315);
					break;
				default :
					gpC_base->m_p_DDSurface_back->BltSpriteDarkness(&pt,&C_VS_UI_SKILL::m_C_spk[RankBonus.GetSkillIconID()],1);
					break;
				}

			}
			else
			{
				// 일반 출력
				if(gbl_vampire_interface)
					gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-15,y+p_button->y-15,C_GLOBAL_RESOURCE::VAMPIRE_GRADE_MARK_EDGE);
				else
					gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-15,y+p_button->y-15,C_GLOBAL_RESOURCE::SLAYER_GRADE_MARK_EDGE);

				switch(RankBonus.GetStatus())
				{
				case RankBonusInfo::STATUS_NULL :
					if((g_char_slot_ingame.GRADE-1)/5 >= MyGrade)		// 초록색 						
						gpC_base->m_p_DDSurface_back->BltSpriteColor(&pt,&C_VS_UI_SKILL::m_C_spk[RankBonus.GetSkillIconID()],rgb_GREEN);
					else
						gpC_base->m_p_DDSurface_back->BltSpriteEffect(&pt,&C_VS_UI_SKILL::m_C_spk[RankBonus.GetSkillIconID()]);
					break;
				case RankBonusInfo::STATUS_CANNOT_LEARN :
					gpC_base->m_p_DDSurface_back->BltSpriteDarkness(&pt,&C_VS_UI_SKILL::m_C_spk[RankBonus.GetSkillIconID()],1);
					break;
				case RankBonusInfo::STATUS_LEARNED :					
					gpC_base->m_p_DDSurface_back->BltSpriteColorSet(&pt,&C_VS_UI_SKILL::m_C_spk[RankBonus.GetSkillIconID()],315);
					break;
				default :
					gpC_base->m_p_DDSurface_back->BltSpriteDarkness(&pt,&C_VS_UI_SKILL::m_C_spk[RankBonus.GetSkillIconID()],1);
					break;
				}				
			}			
		}
	}
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INFO::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_INFO::IsPixel(int _x, int _y)
{
	//	bool re = C_VS_UI_DIALOG::IsPixel(_x, _y);
	bool re = IsInRect(_x, _y);
	if(re == false)
	{
		re = gpC_global_resource->m_pC_info_spk->IsPixel(_x-x-m_rt_tab.x, _y-y-m_rt_tab.y);
	}
	
	return re;
	//	return m_pC_charinfo->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), CHARINFO_WINDOW);
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INFO::Run(id_t id)
{
	// Process Grade Skill Button
	if(id>=GRADE1_BUTTON1_ID && id<=GRADE1_BUTTON8_ID)
	{
		int nowID=id-GRADE1_BUTTON1_ID;

		if(m_draw_grade_skill_mark > nowID)
		{
			RankBonusInfo& RankBonus = (*g_pRankBonusTable)[m_grade_skill_id[nowID]];
			int MyGrade = gbl_vampire_interface ? m_iGrade - RITTER_ID : m_iGrade - PIVATE_ID;
			
			// 스킬 상태가 NULL 이어야 하고, 현재 선택한 계급이 자기 계급과 같거나 낮아야 한다.
			if(RankBonusInfo::STATUS_NULL==RankBonus.GetStatus() && ((g_char_slot_ingame.GRADE-1)/5)>=MyGrade)
			{
			//	gpC_base->SendMessage(UI_SELECT_GRADE_SKILL,(int)RankBonus.GetType(),0,NULL);
				int dx = m_pC_grade1_button_group->GetButton(id)->x+x;
				int dy = m_pC_grade1_button_group->GetButton(id)->y+y+40;

				m_selected_grade_skill = (int)RankBonus.GetType();
				DeleteNew(m_pC_learn_grade_skill_confirm);
				m_pC_learn_grade_skill_confirm = new C_VS_UI_DIALOG(dx,dy,3,0,ExecF_LearnGradeSkillConfirm,DIALOG_OK|DIALOG_CANCEL);
				
				static const char *ppmsg[] = {
					(*g_pGameStringTable)[UI_STRING_MESSAGE_LEARN_GRADE_SKILL_CONFIRM].GetString(),
				};
				m_pC_learn_grade_skill_confirm->SetMessage((char**)ppmsg,1,SMO_NOFIT);
				m_pC_learn_grade_skill_confirm->Start();				
			} else
				m_selected_grade_skill = -1;						
		} else
			m_selected_grade_skill = -1;
		return;
	}
	
	switch (id)
	{
	case CLOSE_ID:
		gpC_base->SendMessage(UI_INFO_CLOSE);
		break;
		
	case HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::INFO);
		EMPTY_MOVE;
		break;
		
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;
		
		//domain
	case BLADE_ID:
		m_skill_domain = SKILLDOMAIN_BLADE;
		m_iDomain = BLADE_ID;
		//			m_pC_skill_scroll_bar->SetPosMax((*g_pSkillManager)[SKILLDOMAIN_BLADE].GetSize()-7);
		m_pC_skill_scroll_bar->SetPosMax(((*g_pSkillManager)[SKILLDOMAIN_BLADE].GetSkillStepList((SKILL_STEP)(SKILL_STEP_APPRENTICE)))->size()-7);
		break;
		
	case SWORD_ID:
		m_skill_domain = SKILLDOMAIN_SWORD;
		m_iDomain = SWORD_ID;
		//			m_pC_skill_scroll_bar->SetPosMax((*g_pSkillManager)[SKILLDOMAIN_SWORD].GetSize()-7);
		m_pC_skill_scroll_bar->SetPosMax(((*g_pSkillManager)[m_skill_domain].GetSkillStepList((SKILL_STEP)(SKILL_STEP_APPRENTICE)))->size()-7);
		break;
		
	case GUN_ID:
		m_skill_domain = SKILLDOMAIN_GUN;
		m_iDomain = GUN_ID;
		//			m_pC_skill_scroll_bar->SetPosMax((*g_pSkillManager)[SKILLDOMAIN_GUN].GetSize()-7);
		m_pC_skill_scroll_bar->SetPosMax(((*g_pSkillManager)[m_skill_domain].GetSkillStepList((SKILL_STEP)(SKILL_STEP_APPRENTICE)))->size()-7);
		break;
		
	case HEAL_ID:
		m_skill_domain = SKILLDOMAIN_HEAL;
		m_iDomain = HEAL_ID;
		//			m_pC_skill_scroll_bar->SetPosMax((*g_pSkillManager)[SKILLDOMAIN_HEAL].GetSize()-7);
		m_pC_skill_scroll_bar->SetPosMax(((*g_pSkillManager)[m_skill_domain].GetSkillStepList((SKILL_STEP)(SKILL_STEP_APPRENTICE)))->size()-7);
		break;
		
	case ENCHANT_ID:
		m_skill_domain = SKILLDOMAIN_ENCHANT;
		m_iDomain = ENCHANT_ID;
		//			m_pC_skill_scroll_bar->SetPosMax((*g_pSkillManager)[SKILLDOMAIN_ENCHANT].GetSize()-7);
		m_pC_skill_scroll_bar->SetPosMax(((*g_pSkillManager)[m_skill_domain].GetSkillStepList((SKILL_STEP)(SKILL_STEP_APPRENTICE)))->size()-7);
		break;
		
	case TOTAL_ID:
		m_iDomain = TOTAL_ID;
		break;
		
	case POISON_ID:
		m_skill_domain = SKILLDOMAIN_VAMPIRE;
		m_iDomain = POISON_ID;
		m_pC_skill_scroll_bar->SetPosMax(((*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetSkillStepList((SKILL_STEP)(m_iDomain-POISON_ID+SKILL_STEP_VAMPIRE_POISON)))->size()-7);
		break;
		
	case ACID_ID:
		m_skill_domain = SKILLDOMAIN_VAMPIRE;
		m_iDomain = ACID_ID;
		m_pC_skill_scroll_bar->SetPosMax(((*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetSkillStepList((SKILL_STEP)(m_iDomain-POISON_ID+SKILL_STEP_VAMPIRE_POISON)))->size()-7);
		break;
		
	case CURSE_ID:
		m_skill_domain = SKILLDOMAIN_VAMPIRE;
		m_iDomain = CURSE_ID;
		m_pC_skill_scroll_bar->SetPosMax(((*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetSkillStepList((SKILL_STEP)(m_iDomain-POISON_ID+SKILL_STEP_VAMPIRE_POISON)))->size()-7);
		break;
		
	case SUMMON_ID:
		m_skill_domain = SKILLDOMAIN_VAMPIRE;
		m_iDomain = SUMMON_ID;
		m_pC_skill_scroll_bar->SetPosMax(((*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetSkillStepList((SKILL_STEP)(m_iDomain-POISON_ID+SKILL_STEP_VAMPIRE_POISON)))->size()-7);
		break;
		
	case BLOOD_ID:
		m_skill_domain = SKILLDOMAIN_VAMPIRE;
		m_iDomain = BLOOD_ID;
		m_pC_skill_scroll_bar->SetPosMax(((*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetSkillStepList((SKILL_STEP)(m_iDomain-POISON_ID+SKILL_STEP_VAMPIRE_POISON)))->size()-7);
		break;
		
	case INNATE_ID:
		m_skill_domain = SKILLDOMAIN_VAMPIRE;
		m_iDomain = INNATE_ID;
		// etc 땜빵
		m_pC_skill_scroll_bar->SetPosMax(((*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetSkillStepList((SKILL_STEP)(m_iDomain-POISON_ID+SKILL_STEP_VAMPIRE_POISON)))->size()-7);
		break;
		
		// Vampire Bonus Buttons
	case STR_PLUS_ID:
		gpC_base->SendMessage(UI_CLICK_BONUS_POINT, 0);
		break;
		
	case DEX_PLUS_ID:
		gpC_base->SendMessage(UI_CLICK_BONUS_POINT, 1);
		break;
		
	case INT_PLUS_ID:
		gpC_base->SendMessage(UI_CLICK_BONUS_POINT, 2);
		break;

		//Slayer GradeSkill
	case PIVATE_ID :
		//m_iGrade = PIVATE_ID;
		CheckGradeSkillIDList();
		break;
	case SERENT_ID :
		m_iGrade = SERENT_ID;
		CheckGradeSkillIDList();
		break;
	case FEACEL_ID :
		m_iGrade = FEACEL_ID;
		CheckGradeSkillIDList();
		break;
	case LITENA_ID :
		m_iGrade = LITENA_ID;
		CheckGradeSkillIDList();
		break;
	case KAINEL_ID :
		m_iGrade = KAINEL_ID;
		CheckGradeSkillIDList();
		break;
		
		//Vampire GradeSkill
	case RITTER_ID :
		//m_iGrade = RITTER_ID;
		CheckGradeSkillIDList();
		break;
	case REICHSRITTER_ID :
		m_iGrade = REICHSRITTER_ID;
		CheckGradeSkillIDList();
		break;
	case BARONET_ID :
		m_iGrade = BARONET_ID;
		CheckGradeSkillIDList();
		break;
	case PREYHER_ID :
		m_iGrade = PREYHER_ID;
		CheckGradeSkillIDList();
		break;
	case GRAF_ID :
		m_iGrade = GRAF_ID;
		CheckGradeSkillIDList();
		break;
	}
	EMPTY_MOVE;
}

bool	C_VS_UI_INFO::CharacterInfoMouseControl(UINT message, int _x, int _y)
{
	static char temp_str[3][100];
	static LPSTR str[3] = { 
		temp_str[0],
		temp_str[1],
		temp_str[2],
	};
	str[0] = temp_str[0];
	str[1] = temp_str[1];
	str[2] = temp_str[2];

	const static char* info_vampire_title_string[8] =
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_STR].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DEX].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_INT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_HP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ACCURACY].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DAMAGE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AVOID].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DEFENCE].GetString(),
	};
	
	const static char* info_slayer_title_string[8] =
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_HP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ACCURACY].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DAMAGE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AVOID].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DEFENCE].GetString(),
	};


	
				// GRADE
	const static char* slayer_grade[10] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PIVATE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_SERENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FEACEL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LITENA].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_KAINEL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_GENEAL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FORE_GENEAL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MAJORIS_GENEAL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_CLOEL_GENEAL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MARSHAL].GetString(),
	};
	
	
	const static char* vampire_grade[10] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_RITTER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_REICHSRITTER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_BARONET].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PREYHER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_GRAF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MARKGRAF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PFALZGRAF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FURST].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_HERZOG].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LANDESHER].GetString(),
	};


	const static char* exp_bar_string[3]= 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CURRENT_EXP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NEXT_LEVEL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_EXP].GetString(),
	};

	
	if(gbl_vampire_interface)
	{
		if(m_rt_char_box.x < _x && m_rt_char_box.x+55 > _x &&
			m_rt_char_box.y < _y && m_rt_char_box.y+70 > _y)
		{
			wsprintf(temp_str[0],"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_TIP_CHANGE_PICTURE_CLICK_HERE].GetString());
			str[1]=NULL;
			str[2]=NULL;
			g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void *)str,1);						 
		} 
		else if(_x > 74 && _x < 181 && _y > 15 && _y < 55)	// alignment
		{
			int num=(_y-20)/20;

			switch(num)
			{
			case 0 :			//name
				wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_NAME].GetString(),g_char_slot_ingame.sz_name.c_str());
				str[1]=NULL;
				str[2]=NULL;
				break;
			case 1 :			//ali
				wsprintf(temp_str[0], "%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGNMENT].GetString());
				if(g_char_slot_ingame.alignment_num > 0)
				{
					wsprintf(temp_str[2], "(+%d)", g_char_slot_ingame.alignment_num);
				}
				else
				{
					wsprintf(temp_str[2], "(%d)", g_char_slot_ingame.alignment_num);
				}
				str[1] = NULL;
				str[2]=temp_str[2];
				break;
			}						
			g_descriptor_manager.Set(DID_HELP, x+_x, y+_y, (void *)str,0,0);
		}
		else if(_x > 187 && _x < 321 && _y > 36 && _y < 58)	// exp
		{
			int LeftExp = g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level).AccumExp - g_char_slot_ingame.EXP_CUR;
			
			if(LeftExp < 0)
			{
				wsprintf(temp_str[0], "%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_UP_LEVEL].GetString());
			}
			else
			{
				char szTemp[256] = {0,};
				std::string temp[3];
				wsprintf(szTemp,"%d",g_char_slot_ingame.EXP_CUR);
				temp[0] = szTemp;
				for(int i = 3; i <= 13; i += 4)
					if(temp[0].size() > i)temp[0].insert(temp[0].size()-i, ",");
				wsprintf(szTemp,"%d",g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level).AccumExp);
				temp[1] = szTemp;
				for(i = 3; i <= 13; i += 4)
					if(temp[1].size() > i)temp[1].insert(temp[1].size()-i, ",");
				wsprintf(szTemp,"%d",LeftExp);
				temp[2] = szTemp;
				for(i = 3; i <= 13; i += 4)
					if(temp[2].size() > i)temp[2].insert(temp[2].size()-i, ",");
				
				wsprintf(temp_str[0], "%s/%s (%s%s)", temp[0].c_str(), temp[1].c_str(), 
					(*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_EXP].GetString(), temp[2].c_str());
//				wsprintf(temp_str[0], "%d/%d (%s%d)", 
//					g_char_slot_ingame.EXP_CUR, g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level).AccumExp,
//					(*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_EXP].GetString(), LeftExp);
				int fame = g_pFameInfoTable->GetFameForLevel( SKILLDOMAIN_VAMPIRE, g_char_slot_ingame.level );

				if(g_char_slot_ingame.FAME < fame )
					wsprintf(temp_str[0], "%s(%s:%d)",(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_UP_LEVEL_BY_FAME].GetString(), 
					(*g_pGameStringTable)[UI_STRING_MESSAGE_NEED_FAME].GetString(), 
					fame - g_char_slot_ingame.FAME );
			}
			g_descriptor_manager.Set(DID_INFO, x+_x, y+_y, (void *)temp_str[0],0,0);
		}
		else if(_x > 187 && _x < 264 && _y > 15 && _y < 35)	// level
		{
			wsprintf(temp_str[0],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_LEVEL].GetString(),g_char_slot_ingame.level);
			g_descriptor_manager.Set(DID_INFO, x+_x, y+_y, (void *)temp_str[0],0,0);
		}
		else if(_x > 79 && _y > 58 && _x < 249 && _y < 118)
		{
			int num=(_y-63)/20;
			str[2]=NULL;
			str[1]=NULL;
			if(!(g_char_slot_ingame.sz_guild_name.size()>0))
				num++;
			
			switch(num)
			{
			case 0 ://팀
				if(g_char_slot_ingame.sz_guild_name.size()>0)
					wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_CLAN_NAME].GetString(),g_char_slot_ingame.sz_guild_name.c_str());
				else
					wsprintf(temp_str[0],"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_JOIN_ANY_CLAN].GetString());
				break;
			case 1 ://계급
				if(g_char_slot_ingame.GRADE > 0 &&g_char_slot_ingame.GRADE <= GRADE_MARK_MAX)
				{								
					wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_NAME].GetString(),vampire_grade[(g_char_slot_ingame.GRADE-1)/5]);
					wsprintf(temp_str[1],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LEVEL].GetString(),g_char_slot_ingame.GRADE);	
					wsprintf(temp_str[2],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_GRADE_EXP].GetString(),g_char_slot_ingame.GRADE_EXP);
					str[1]=temp_str[1];									
					str[2]=temp_str[2];
				}
				else
					str[0]=NULL;
				break;
			case 2 : // 명성
				wsprintf(temp_str[0],"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_FAME].GetString());
				wsprintf(temp_str[1],"%d",g_char_slot_ingame.FAME);
				{
					std::string temp = temp_str[1];
					for(int i = 3; i <= 13; i += 4)
						if(temp.size() > i)temp.insert(temp.size()-i, ",");
					wsprintf(temp_str[1],"%s",temp.c_str() );
				}
				
				str[0] = temp_str[0];
				str[1] = NULL;
				str[2] = temp_str[1];
				break;
				
			default :
				str[0]=NULL;
				break;
			}
			if(str[0]!=NULL)
			{
				if(str[1]!=NULL)
				{
					if(str[2]!=NULL)
						g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str,3);
					else
						g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str,2);
				}
				else
				{
					if(str[2]!=NULL)
						g_descriptor_manager.Set(DID_HELP, x+_x, y+_y, (void *)str,0,0);
					else
						g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str,1);
				}
			}
		}
		if(_x > m_rt_char_box.x && _x < m_rt_char_box.x+125 && _y > 125 && _y < 125+20*8)
		{
			if(!((_y-125)/20 < 3 && _x > m_rt_char_box.x+35 && g_char_slot_ingame.bonus_point > 0))
			{
				int num=(_y-125)/20;
				wsprintf(temp_str[0],info_vampire_title_string[num]);
				switch(num)
				{
				case 0 :
					wsprintf(temp_str[2],"%d/%d",g_char_slot_ingame.STR_CUR,g_char_slot_ingame.STR_MAX);
					break;
				case 1 :
					wsprintf(temp_str[2],"%d/%d",g_char_slot_ingame.DEX_CUR,g_char_slot_ingame.DEX_MAX);
					break;
				case 2 :
					wsprintf(temp_str[2],"%d/%d",g_char_slot_ingame.INT_CUR,g_char_slot_ingame.INT_MAXX);
					break;
				case 3 :
					wsprintf(temp_str[2],"%d/%d",g_char_slot_ingame.HP,g_char_slot_ingame.HP_MAX);
					break;
				case 4 :
					wsprintf(temp_str[2],"%d",g_char_slot_ingame.TOHIT);
					break;
				case 5 :
					{
						//damage
						int dam1, dam2;
						dam1 = g_char_slot_ingame.DAM + g_char_slot_ingame.SILVER_DAM;
						dam2 = g_char_slot_ingame.DAM2 + g_char_slot_ingame.SILVER_DAM2;
						if (dam1 == dam2)
							sprintf(temp_str[2], "%d", dam1);
						else
							sprintf(temp_str[2], "%d~%d", dam2, dam1);									
					}
					break;
				case 6 :
					wsprintf(temp_str[2],"%d",g_char_slot_ingame.DEFENSE);
					break;
				case 7 :
					wsprintf(temp_str[2],"%d",g_char_slot_ingame.PROTECTION);
					break;
				}				
				str[0]=temp_str[0];
				str[1]=NULL;
				str[2]=temp_str[2];
				g_descriptor_manager.Set(DID_HELP, x+_x, y+_y, (void *)str,0);
			}
		}
	} else 
	{
		if(m_rt_char_box.x < _x && m_rt_char_box.x+55 > _x &&
			m_rt_char_box.y < _y && m_rt_char_box.y+70 > _y)
		{
			wsprintf(temp_str[0],"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_TIP_CHANGE_PICTURE_CLICK_HERE].GetString());
			str[1]=NULL;
			str[2]=NULL;
			g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void *)str,1);						 
		}
		else if(_x > m_rt_char_box.x && _x < m_rt_char_box.x+125 && _y > 128+20*0 && _y < 128+20*3)	// exp
		{
			bool bMax = false;
			int select = (_y-128)/20;
			
			bool bExp=(128+20*(select+1)-6)<_y;
			
			if(!bExp)
			{
				switch(select)
				{
				case 0 :
					wsprintf(temp_str[0],"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_STR].GetString());
					wsprintf(temp_str[1],"%d/%d",g_char_slot_ingame.STR_CUR,g_char_slot_ingame.STR_MAX);
					break;
				case 1 :
					wsprintf(temp_str[0],"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_DEX].GetString());
					wsprintf(temp_str[1],"%d/%d",g_char_slot_ingame.DEX_CUR,g_char_slot_ingame.DEX_MAX);
					break;
				case 2 :
					wsprintf(temp_str[0],"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_INT].GetString());
					wsprintf(temp_str[1],"%d/%d",g_char_slot_ingame.INT_CUR,g_char_slot_ingame.INT_MAXX);
					break;
				}
				
				str[0]=temp_str[0];
				str[1]=NULL;
				str[2]=temp_str[1];
				
				g_descriptor_manager.Set(DID_HELP,x+_x,y+_y,(void*)str,0,0);
			} else
			{
				for(int j = 0; j < 3; j++)
				{
					strcpy(temp_str[j], exp_bar_string[j]);
					
					char temp[30];
					int num;
					switch(j)
					{
					case 0:
						num = *(&g_char_slot_ingame.STR_EXP_CUR + select);
						break;
						
					case 1:
						{
							switch(select)
							{
							case 0:	//STR
								num = g_pExperienceTable->GetSTRInfo(g_char_slot_ingame.STR_PURE).AccumExp;
								break;
							case 1:	//DEX
								num = g_pExperienceTable->GetDEXInfo(g_char_slot_ingame.DEX_PURE).AccumExp;
								break;
							case 2:	//INT
								num = g_pExperienceTable->GetINTInfo(g_char_slot_ingame.INT_PURE).AccumExp;
								break;
							}
						}
						break;
						
					case 2:
						switch(select)
						{
						case 0:	//STR
							num = g_pExperienceTable->GetSTRInfo(g_char_slot_ingame.STR_PURE).AccumExp - g_char_slot_ingame.STR_EXP_CUR;
							break;
						case 1:	//DEX
							num = g_pExperienceTable->GetDEXInfo(g_char_slot_ingame.DEX_PURE).AccumExp - g_char_slot_ingame.DEX_EXP_CUR;
							break;
						case 2:	//INT
							num = g_pExperienceTable->GetINTInfo(g_char_slot_ingame.INT_PURE).AccumExp - g_char_slot_ingame.INT_EXP_CUR;
							break;
						}
						break;
					}
					
					// 숫자사이에 ,넣기
					wsprintf(temp, "%d", num);
					std::string sstr = temp;
					for(int i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
						
						strcat(temp_str[j], sstr.c_str());
						
						if(j == 2 && num < 0)bMax = true;	// 레벨업맥스
				}
				
				if(bMax)
				{
					strcpy(temp_str[1], (*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_UP_STAT].GetString());
					g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str, 2);
				}
				else
					g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str, 3);
			}
			
		}
		else if(_x > m_rt_char_box.x && _x < m_rt_char_box.x+120 && _y > 125+20*3 && _y < 125+20*9)
		{
			int num=(_y-125)/20-3;
			
			switch(num)
			{
			case 0 : // 생명력
				wsprintf(temp_str[0],"%s",info_slayer_title_string[num]);
				wsprintf(temp_str[2],"%d/%d",g_char_slot_ingame.HP,g_char_slot_ingame.HP_MAX);
				break;
			case 1 : // 마법력
				wsprintf(temp_str[0],"%s",info_slayer_title_string[num]);
				wsprintf(temp_str[2],"%d/%d",g_char_slot_ingame.MP,g_char_slot_ingame.MP_MAX);
				break;
			case 2 : // 명중률
				wsprintf(temp_str[0],"%s",info_slayer_title_string[num]);
				wsprintf(temp_str[2],"%d",g_char_slot_ingame.TOHIT);
				break;
			case 3 : // 데미지
				{
					int dam1, dam2;
					dam1 = g_char_slot_ingame.DAM + g_char_slot_ingame.SILVER_DAM;
					dam2 = g_char_slot_ingame.DAM2 + g_char_slot_ingame.SILVER_DAM2;								
					wsprintf(temp_str[0],"%s",info_slayer_title_string[num]);								
					
					if(dam1==dam2)
						wsprintf(temp_str[2],"%d",dam1);
					else
						wsprintf(temp_str[2],"%d~%d",dam2,dam1);								
				}
				break;
			case 4 : //회피율
				wsprintf(temp_str[0],"%s",info_slayer_title_string[num]);
				wsprintf(temp_str[2],"%d",g_char_slot_ingame.DEFENSE);
				break;
			case 5 : // 방어율 
				wsprintf(temp_str[0],"%s",info_slayer_title_string[num]);
				wsprintf(temp_str[2],"%d",g_char_slot_ingame.PROTECTION);
				break;
			}
			str[0]=temp_str[0];
			str[1]=NULL;
			
			g_descriptor_manager.Set(DID_HELP,x+_x, y+_y, (void *)str,0);
		}
		else if(_y>23&&_y<83&&_x>80&&_x<253)
		{
			int num = (_y-23)/20;
			
			str[0]=temp_str[0];
			str[1]=NULL;
			str[2]=temp_str[2];
			
			switch(num)
			{
			case 0 :
				sprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_NAME].GetString(),g_char_slot_ingame.sz_name.c_str());
				str[2]=NULL;
				break;
			case 1 :
				{
					sprintf(temp_str[2],"%d",g_char_slot_ingame.FAME);
					std::string sstr;
					sstr=temp_str[2];
					for(int i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
						strcpy(temp_str[0],(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_FAME].GetString());
						strcpy(temp_str[2],sstr.c_str());								
				}									
				break;
			case 2 :
				wsprintf(temp_str[0], "%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGNMENT].GetString());
				if(g_char_slot_ingame.alignment_num > 0)
				{
					wsprintf(temp_str[2], "(+%d)", g_char_slot_ingame.alignment_num);
				}
				else
				{
					wsprintf(temp_str[2], "(%d)", g_char_slot_ingame.alignment_num);
				}
				break;
			}
			if(str[0]!=NULL)
			{
				if(str[1]!=NULL)
					g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void*)str,2);
				else
				{
					if(str[2]!=NULL)
						g_descriptor_manager.Set(DID_HELP,x+_x,y+_y,(void*)str,0.0);
					else
						g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void*)str,1);
				}
			}
		} else if(_y>83&&_y<123&&_x>80&&_x<278)
		{
			int num=(_y-83)/20;
			
			str[0]=temp_str[0];
			str[1]=NULL;
			str[2]=temp_str[2];
			
			
			switch(num)
			{
			case 0 ://team
				if(g_char_slot_ingame.sz_guild_name.c_str() != NULL&&
					g_char_slot_ingame.sz_guild_name.size()>0)
				{
					wsprintf(temp_str[0],"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_TEAM_NAME].GetString());
					strcat(temp_str[0],g_char_slot_ingame.sz_guild_name.c_str());
				} else
				{
					goto grade;									// goto 는 안쓰는게 좋긴하지만 뭐-_-
				}
				str[2]=NULL;								
				break;
			case 1 ://grade
				if(!(g_char_slot_ingame.sz_guild_name.size()>0))
				{
					str[0]=NULL;
				}
grade :			str[2]=NULL;
				if(g_char_slot_ingame.GRADE > 0 &&g_char_slot_ingame.GRADE <= GRADE_MARK_MAX)
				{
					// 계급 Description                   계급 이름, 계급 레벨 
					wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_NAME].GetString(),slayer_grade[(g_char_slot_ingame.GRADE-1)/5]);
					wsprintf(temp_str[1],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LEVEL].GetString(),g_char_slot_ingame.GRADE);											
					wsprintf(temp_str[2],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_GRADE_EXP].GetString(),g_char_slot_ingame.GRADE_EXP);
					str[1]=temp_str[1];											
					str[2]=temp_str[2];
				}
				else
					str[0]=NULL;
				break;
			}
			if(str[0]!=NULL)
			{
				if(str[1]!=NULL)
				{
					if(str[2]!=NULL)
						g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void*)str,3);
					else
						g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void*)str,2);
				}
				else
				{
					if(str[2]!=NULL)
						g_descriptor_manager.Set(DID_HELP,x+_x,y+_y,(void*)str,0.0);
					else
						g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void*)str,1);
				}
			}
		}									
	}
	return true;
}


bool	C_VS_UI_INFO::SkillInfoMouseControl(UINT message, int _x, int _y)
{	
	static char temp_str[3][100];
	static LPSTR str[3] = { temp_str[0], temp_str[1], temp_str[2] };
	str[0] = temp_str[0];
	str[1] = temp_str[1];
	str[2] = temp_str[2];
	
	if(!((gbl_vampire_interface && m_iDomain >=POISON_ID && m_iDomain <= INNATE_ID && m_pC_skill_scroll_bar->GetScrollPos() >= 0 && m_skill_domain == SKILLDOMAIN_VAMPIRE) || 
		(!gbl_vampire_interface && m_iDomain >=BLADE_ID && m_iDomain <= ENCHANT_ID && m_pC_skill_scroll_bar->GetScrollPos() >= 0 && (m_skill_domain == SKILLDOMAIN_SWORD || m_skill_domain == SKILLDOMAIN_BLADE || m_skill_domain == SKILLDOMAIN_GUN || m_skill_domain == SKILLDOMAIN_HEAL || m_skill_domain == SKILLDOMAIN_ENCHANT))))
		return false;
				
	if(gbl_vampire_interface)	// vampire skill info mode
	{
		const SKILL_STEP step = (SKILL_STEP)(m_iDomain-POISON_ID+SKILL_STEP_VAMPIRE_POISON);
		
		if(g_pSkillManager && (*g_pSkillManager)[m_skill_domain].IsExistSkillStep(step))
		{
			if(_x >= m_rcSkillDesciption.left-19 && _y >= m_rcSkillDesciption.top && _x < m_rcSkillDesciption.right && _y < m_rcSkillDesciption.bottom)
			{
				const int num = (_y - m_rcSkillDesciption.top)/19+m_pC_skill_scroll_bar->GetScrollPos();
				
				if(num >= (*((*g_pSkillManager)[m_skill_domain].GetSkillStepList(step))).size())
					return false;
				
				const int id = ( *( (*g_pSkillManager)[m_skill_domain].GetSkillStepList( step ) ) )[num];
				
				g_descriptor_manager.Set(DID_SKILLTREE, x+_x, y+_y, (void *)&m_skill_domain, id, 0);
				m_focused_skill_id = id;
			}
		}
	}
	else	// slayer skill info mode
	{
		if(_x > 24 && _x < 292 && _y > 72 && _y < 84)			//Domain Level
		{
			static char temp_string[100];
			static char* leveldesc[1]={ temp_string};
			
			switch(m_iDomain)
			{
			case BLADE_ID :
				wsprintf(temp_string,"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_BLADE_LEVEL].GetString(),(*g_pSkillManager)[m_iDomain-BLADE_ID].GetDomainLevel());
				break;
			case SWORD_ID :
				wsprintf(temp_string,"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_SWORD_LEVEL].GetString(),(*g_pSkillManager)[m_iDomain-BLADE_ID].GetDomainLevel());
				break;
			case ENCHANT_ID :
				wsprintf(temp_string,"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_ENCHANT_LEVEL].GetString(),(*g_pSkillManager)[m_iDomain-BLADE_ID].GetDomainLevel());
				break;
			case HEAL_ID :
				wsprintf(temp_string,"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_HEAL_LEVEL].GetString(),(*g_pSkillManager)[m_iDomain-BLADE_ID].GetDomainLevel());
				break;
			case GUN_ID :
				wsprintf(temp_string,"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_GUN_LEVEL].GetString(),(*g_pSkillManager)[m_iDomain-BLADE_ID].GetDomainLevel());
				break;
			}						
			g_descriptor_manager.Set(DID_STRINGS, x+_x+24, y+_y+86, (void *)leveldesc, 1);
		}
		if(_x > 24 && _x < 292 && _y > 90 && _y < 104)	// domain exp
		{
			if(g_pSkillManager)
			{
				int level = (*g_pSkillManager)[m_skill_domain].GetDomainLevel();
				int exp = (*g_pSkillManager)[m_skill_domain].GetDomainExp();
				
				if(level >= 0 && exp >= 0)
				{
					char sz_temp[100];
					std::string sstr;
					int i;
					
					int next_exp = (*g_pSkillManager)[m_skill_domain].GetExpInfo(level).AccumExp;
					
					// 숫자사이에 ,넣기
					wsprintf(sz_temp, "%d", exp);
					sstr = sz_temp;
					for(i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
					wsprintf(temp_str[0], "%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_CURRENT_EXP].GetString(), sstr.c_str());
						
					// 숫자사이에 ,넣기
					wsprintf(sz_temp, "%d", next_exp);
					sstr = sz_temp;
					for(i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
					wsprintf(temp_str[1], "%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_NEXT_LEVEL].GetString(), sstr.c_str());
							
					// 숫자사이에 ,넣기
					wsprintf(sz_temp, "%d", next_exp - exp);
					sstr = sz_temp;
					for(i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
					wsprintf(temp_str[2], "%s%s", (*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_EXP].GetString(),sstr.c_str());

					int fame = g_pFameInfoTable->GetFameForLevel( (SKILLDOMAIN) m_skill_domain, level );

					if( g_char_slot_ingame.FAME < fame )
					{
						wsprintf(temp_str[2], "%s(%s:%d)",(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_UP_LEVEL_BY_FAME].GetString(),
							(*g_pGameStringTable)[UI_STRING_MESSAGE_NEED_FAME].GetString(), fame - g_char_slot_ingame.FAME );
					}
					
					g_descriptor_manager.Set(DID_STRINGS, x+_x+24, y+_y+86, (void *)str, 3);//3);
				}
			}
		}
		// skill desc
		else if(_x >= m_rcSkillDesciption.left-19 && _y >= m_rcSkillDesciption.top && _x < m_rcSkillDesciption.right && _y < m_rcSkillDesciption.bottom)
		{
			const SKILL_STEP step = (SKILL_STEP)(SKILL_STEP_APPRENTICE);
			
			if(g_pSkillManager && (*g_pSkillManager)[m_skill_domain].IsExistSkillStep(step))
			{
				const int num = (_y - m_rcSkillDesciption.top)/19+m_pC_skill_scroll_bar->GetScrollPos();
				
				if(num >= (*((*g_pSkillManager)[m_skill_domain].GetSkillStepList(step))).size())return false;
				
				const int id = ( *( (*g_pSkillManager)[m_skill_domain].GetSkillStepList( step ) ) )[num];
				
				//							(*g_pSkillManager)[m_skill_domain].SetBegin();
				//							for(int skip = 0; skip < num && !(*g_pSkillManager)[m_skill_domain].IsEnd(); skip++)
				//							{
				//								(*g_pSkillManager)[m_skill_domain].Next();
				//							}
				//							if(!(*g_pSkillManager)[m_skill_domain].IsEnd())
				//							{
				//								ACTIONINFO SkillID = (*g_pSkillManager)[m_skill_domain].GetSkillID();
				g_descriptor_manager.Set(DID_SKILLTREE, x+_x, y+_y, (void *)&m_skill_domain, id, 0);
				m_focused_skill_id = id;
				//							}
			}
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_INFO::MouseControl(UINT message, int _x, int _y)
{
	static const char scancode_name[256][20] = 
	{
		"",						// 0x00
		"ESCAPE",				// 0x01
		"1",					// 0x02
		"2",					// 0x03
		"3",					// 0x04
		"4",					// 0x05
		"5",					// 0x06
		"6",					// 0x07
		"7",					// 0x08
		"8",					// 0x09
		"9",					// 0x0A
		"0",					// 0x0B
		"MINUS",				// 0x0C
		"EQUALS",				// 0x0D
		"BACK",					// 0x0E
		"TAB",					// 0x0F
		"Q",					// 0x10
		"W",					// 0x11
		"E",					// 0x12
		"R",					// 0x13
		"T",					// 0x14
		"Y",					// 0x15
		"U",					// 0x16
		"I",					// 0x17
		"O",					// 0x18
		"P",					// 0x19
		"LBRACKET",				// 0x1A
		"RBRACKET",				// 0x1B
		"RETURN",				// 0x1C
		"LCONTROL",				// 0x1D
		"A",					// 0x1E
		"S",					// 0x1F
		"D",					// 0x20
		"F",					// 0x21
		"G",					// 0x22
		"H",					// 0x23
		"J",					// 0x24
		"K",					// 0x25
		"L",					// 0x26
		":",					// 0x27
		"'",					// 0x28
		"~",					// 0x29
		"LSHIFT",				// 0x2A
		"BACKSLASH",			// 0x2B
		"Z",					// 0x2C
		"X",					// 0x2D
		"C",					// 0x2E
		"V",					// 0x2F
		"B",					// 0x30
		"N",					// 0x31
		"M",					// 0x32
		"COMMA",				// 0x33
		"PERIOD",				// 0x34
		"SLASH",				// 0x35
		"RSHIFT",				// 0x36
		"MULTIPLY",				// 0x37
		"LMENU",				// 0x38
		"SPACE",				// 0x39
		"CAPITAL",				// 0x3A
		"F1",					// 0x3B
		"F2",					// 0x3C
		"F3",					// 0x3D
		"F4",					// 0x3E
		"F5",					// 0x3F
		"F6",					// 0x40
		"F7",					// 0x41
		"F8",					// 0x42
		"F9",					// 0x43
		"F10",					// 0x44
		"NUMLOCK",				// 0x45
		"SCROLL",				// 0x46
		"NUMPAD7",				// 0x47
		"NUMPAD8",				// 0x48
		"NUMPAD9",				// 0x49
		"SUBTRACT",				// 0x4A
		"NUMPAD4",				// 0x4B
		"NUMPAD5",				// 0x4C
		"NUMPAD6",				// 0x4D
		"ADD",					// 0x4E
		"NUMPAD1",				// 0x4F
		"NUMPAD2",				// 0x50
		"NUMPAD3",				// 0x51
		"NUMPAD0",				// 0x52
		"DECIMAL",				// 0x53
		"",						// 0x54
		"",						// 0x55
		"",						// 0x56
		"F11",					// 0x57
		"F12",					// 0x58
		"",						// 0x59
		"",						// 0x5A
		"",						// 0x5B
		"",						// 0x5C
		"",						// 0x5D
		"",						// 0x5E
		"",						// 0x5F
		"",						// 0x60
		"",						// 0x61
		"",						// 0x62
		"",						// 0x63
		"F13",					// 0x64
		"F14",					// 0x65
		"F15",					// 0x66
		"",						// 0x67
		"",						// 0x68
		"",						// 0x69
		"",						// 0x6A
		"",						// 0x6B
		"",						// 0x6C
		"",						// 0x6D
		"",						// 0x6E
		"",						// 0x6F
		"KANA",					// 0x70
		"",						// 0x71
		"",						// 0x72
		"",						// 0x73
		"",						// 0x74
		"",						// 0x75
		"",						// 0x76
		"",						// 0x77
		"",						// 0x78
		"CONVERT",				// 0x79
		"",						// 0x7A
		"NOCONVERT",			// 0x7B
		"",						// 0x7C
		"YEN",					// 0x7D
		"",						// 0x7E
		"",						// 0x7F
		"",						// 0x80
		"",						// 0x81
		"",						// 0x82
		"",						// 0x83
		"",						// 0x84
		"",						// 0x85
		"",						// 0x86
		"",						// 0x87
		"",						// 0x88
		"",						// 0x89
		"",						// 0x8A
		"",						// 0x8B
		"",						// 0x8C
		"NUMPADEQUALS",			// 0x8D
		"",						// 0x8E
		"",						// 0x8F
		"CIRCUMFLEX",			// 0x90
		"AT",					// 0x91
		"COLON",				// 0x92
		"UNDERLINE",			// 0x93
		"KANJI",				// 0x94
		"STOP",					// 0x95
		"AX",					// 0x96
		"UNLABELED",			// 0x97
		"",						// 0x98
		"",						// 0x99
		"",						// 0x9A
		"",						// 0x9B
		"NUMPADENTER",			// 0x9C
		"RCONTROL",				// 0x9D
		"",						// 0x9E
		"",						// 0x9F
		"",						// 0xA0
		"",						// 0xA1
		"",						// 0xA2
		"",						// 0xA3
		"",						// 0xA4
		"",						// 0xA5
		"",						// 0xA6
		"",						// 0xA7
		"",						// 0xA8
		"",						// 0xA9
		"",						// 0xAA
		"",						// 0xAB
		"",						// 0xAC
		"",						// 0xAD
		"",						// 0xAE
		"",						// 0xAF
		"",						// 0xB0
		"",						// 0xB1
		"",						// 0xB2
		"NUMPADCOMMA",			// 0xB3
		"",						// 0xB4
		"DIVIDE",				// 0xB5
		"",						// 0xB6
		"SYSRQ",				// 0xB7
		"RMENU",				// 0xB8
		"",						// 0xB9
		"",						// 0xBA
		"",						// 0xBB
		"",						// 0xBC
		"",						// 0xBD
		"",						// 0xBE
		"",						// 0xBF
		"",						// 0xC0
		"",						// 0xC1
		"",						// 0xC2
		"",						// 0xC3
		"",						// 0xC4
		"PAUSE",				// 0xC5
		"",						// 0xC6
		"HOME",					// 0xC7
		"UP",					// 0xC8
		"PRIOR",				// 0xC9
		"",						// 0xCA
		"LEFT",					// 0xCB
		"",						// 0xCC
		"RIGHT",				// 0xCD
		"",						// 0xCE
		"END",					// 0xCF
		"DOWN",					// 0xD0
		"NEXT",					// 0xD1
		"INSERT",				// 0xD2
		"DELETE",				// 0xD3
		"",						// 0xD4
		"",						// 0xD5
		"",						// 0xD6
		"",						// 0xD7
		"",						// 0xD8
		"",						// 0xD9
		"",						// 0xDA
		"LWIN",					// 0xDB
		"RWIN",					// 0xDC
		"APPS",					// 0xDD
		"POWER",				// 0xDE
		"SLEEP",				// 0xDF
		"",						// 0xE0
		"",						// 0xE1
		"",						// 0xE2
		"",						// 0xE3
		"",						// 0xE4
		"",						// 0xE5
		"",						// 0xE6
		"",						// 0xE7
		"",						// 0xE8
		"",						// 0xE9
		"",						// 0xEA
		"",						// 0xEB
		"",						// 0xEC
		"",						// 0xED
		"",						// 0xEE
		"",						// 0xEF
		"",						// 0xF0
		"",						// 0xF1
		"",						// 0xF2
		"",						// 0xF3
		"",						// 0xF4
		"",						// 0xF5
		"",						// 0xF6
		"",						// 0xF7
		"",						// 0xF8
		"",						// 0xF9
		"",						// 0xFA
		"",						// 0xFB
		"",						// 0xFC
		"",						// 0xFD
		"",						// 0xFE
		"",						// 0xFF
	};
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;


	const char* m_info_string[5] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_CHARACTER_INFO_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_SKILL_INFO_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_MAGIC_INFO_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_GRADE1_INFO_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_SUPPORT_MENU].GetString(),
	};
				// GRADE
	const static char* slayer_grade[10] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PIVATE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_SERENT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FEACEL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LITENA].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_KAINEL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_GENEAL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FORE_GENEAL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MAJORIS_GENEAL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_CLOEL_GENEAL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MARSHAL].GetString(),
	};
	
	
	const static char* vampire_grade[10] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_RITTER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_REICHSRITTER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_BARONET].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PREYHER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_GRAF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MARKGRAF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PFALZGRAF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FURST].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_HERZOG].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LANDESHER].GetString(),
	};
	bool re = m_pC_common_button_group->MouseControl(message, _x, _y);
/*	if(m_skill_info_mode)
	{
		re &= m_pC_skill_button_group->MouseControl(message, _x, _y);
		re &= m_pC_skill_scroll_bar->MouseControl(message, _x, _y);
	}
	else	// char info mode
	{
		if(gbl_vampire_interface && g_char_slot_ingame.bonus_point > 0)
			re &= m_pC_char_button_group->MouseControl(message, _x, _y);
		re &= m_pC_char_scroll_bar->MouseControl(message, _x, _y);
	}*/
	switch(m_info_mode)
	{
	case CHARACTER_INFO_MODE :
		if(gbl_vampire_interface && g_char_slot_ingame.bonus_point > 0)
			re &= m_pC_char_button_group->MouseControl(message, _x, _y);
		re &= m_pC_char_scroll_bar->MouseControl(message, _x, _y);
		break;
	case SKILL_INFO_MODE :
		re &= m_pC_skill_button_group->MouseControl(message, _x, _y);
		re &= m_pC_skill_scroll_bar->MouseControl(message, _x, _y);
		break;
	case GRADE1_INFO_MODE :
		{
			// 화면에 출력되어야 할 버튼 수 			 
		
			C_VS_UI_EVENT_BUTTON *pButton=m_pC_grade1_button_group->IsInRect(_x,_y);
			if(pButton == NULL)
				re &= m_pC_grade1_button_group->MouseControl(message, _x, _y);
			else
			{
				if(m_pC_grade1_button_group->GetButton(GRADE1_BUTTON1_ID)->GetID()+m_draw_grade_skill_mark > pButton->GetID())
					re &= m_pC_grade1_button_group->MouseControl(message, _x, _y);		
			}
			//re &= m_pC_grade1_scroll_bar->MouseControl(message, _x, _y);
		}
		break;
	}
	
	
	switch(message)
	{
	case M_MOVING:
		m_focused_skill_id = -1;
		if(_x > m_rt_tab.x && _x < m_rt_tab.x+m_rt_tab.w && _y > m_rt_tab.y && _y < m_rt_tab.y + m_rt_tab.h)
		{
			//if(_x > m_rt_tab.x+m_rt_tab.w/2)
			int temp_mode = (_x - m_rt_tab.x) / (m_rt_tab.w/4);
			static char name[200];
			switch(temp_mode)
			{
			case CHARACTER_INFO_MODE :
				wsprintf(name,"%s (Ctrl+%s)",m_info_string[0],scancode_name[ACCEL_GET_KEY(g_pKeyAccelerator->GetKey(ACCEL_CHARINFO))]);
				g_descriptor_manager.Set(DID_INFO, x+m_rt_tab.x+m_rt_tab.w/2, y+m_rt_tab.y, (void *)name,0,0);
				break;
			case SKILL_INFO_MODE :				
				if(gbl_vampire_interface)
				{
					wsprintf(name,"%s (Ctrl+%s)",m_info_string[2],scancode_name[ACCEL_GET_KEY(g_pKeyAccelerator->GetKey(ACCEL_SKILLINFO))]);
					g_descriptor_manager.Set(DID_INFO, x+m_rt_tab.x+m_rt_tab.w/2, y+m_rt_tab.y, (void *)name,0,0);
				}
				else
				{
					wsprintf(name,"%s (Ctrl+%s)",m_info_string[1],scancode_name[ACCEL_GET_KEY(g_pKeyAccelerator->GetKey(ACCEL_SKILLINFO))]);
					g_descriptor_manager.Set(DID_INFO, x+m_rt_tab.x+m_rt_tab.w/2, y+m_rt_tab.y, (void *)name,0,0);
				}
				break;
			case GRADE1_INFO_MODE :
				wsprintf(name,"%s (Ctrl+%s)",m_info_string[3],scancode_name[ACCEL_GET_KEY(g_pKeyAccelerator->GetKey(ACCEL_GRADE1INFO))]);
				g_descriptor_manager.Set(DID_INFO, x+m_rt_tab.x+m_rt_tab.w/2, y+m_rt_tab.y, (void *)name,0,0);
				break;
			case GRADE2_INFO_MODE :
				wsprintf(name,"%s",m_info_string[4]);
				g_descriptor_manager.Set(DID_INFO, x+m_rt_tab.x+m_rt_tab.w/2, y+m_rt_tab.y, (void *)name,0,0);
				break;				
			}			
		}		
		{
			static char temp_str[3][100];
			static LPSTR str[3] = { temp_str[0], temp_str[1], temp_str[2] };
			str[0] = temp_str[0];
			str[1] = temp_str[1];
			str[2] = temp_str[2];
			
			bool Result;
			switch(m_info_mode)
			{
			case SKILL_INFO_MODE :
				Result = SkillInfoMouseControl(message,_x,_y);				
				break;
			case CHARACTER_INFO_MODE :
				Result = CharacterInfoMouseControl(message,_x,_y);
				break;
			case GRADE1_INFO_MODE :
				if(g_char_slot_ingame.GRADE>0 && g_char_slot_ingame.GRADE <=GRADE_MARK_MAX)
				{
					if(_x>37&&_y>39&&_x<287&&_y<51)
					{
						// Grade Level Bar Description						
						int GradeID = (g_char_slot_ingame.GRADE-1)/5;
						
						wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_NAME].GetString(),gbl_vampire_interface ? vampire_grade[GradeID] : slayer_grade[GradeID]);
						wsprintf(temp_str[1],"%s%d / %d",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LEVEL].GetString(),g_char_slot_ingame.GRADE,GRADE_MARK_MAX);
						if(g_char_slot_ingame.GRADE == GRADE_MARK_MAX)
						{
							wsprintf(temp_str[2],"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_UP_GRADE].GetString());
							g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void*)str,3);
						} else
						{
							g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void*)str,2);														
						}
					} else if(_x>37&& _y>59 &&_x<287&&_y<71)
					{
						// Grade Exp Bar Description
						
						if(g_char_slot_ingame.GRADE == GRADE_MARK_MAX)
						{
							wsprintf(temp_str[0],"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_UP_GRADE].GetString());
							g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void*)str,1);
						} else
						{
							const ExpInfo& expinf = gbl_vampire_interface ? g_pExperienceTable->GetVampireRankInfo(g_char_slot_ingame.GRADE) : g_pExperienceTable->GetSlayerRankInfo(g_char_slot_ingame.GRADE);
							int NextLevelExp = expinf.AccumExp;
							
							wsprintf(temp_str[0],"%s%d / %d",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_GRADE_EXP].GetString(),g_char_slot_ingame.GRADE_EXP,NextLevelExp);
							wsprintf(temp_str[1],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_EXP].GetString(),NextLevelExp - g_char_slot_ingame.GRADE_EXP);
						
							g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void*)str,2);														
						}
					}
				}
				break;
			case GRADE2_INFO_MODE :
				break;
			}
			if(Result == false) 
				break;			
		}
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if(m_info_mode == CHARACTER_INFO_MODE && m_rt_char_box.x < _x && m_rt_char_box.x+55 > _x &&
			m_rt_char_box.y < _y && m_rt_char_box.y+70 > _y)
		{
			gC_vs_ui.RunFileDialog(C_VS_UI_FILE_DIALOG::MODE_PROFILE_SELECT, ".bmp;.jpg");
			break;
		}
		
		if(_x > m_rt_tab.x && _x < m_rt_tab.x+m_rt_tab.w && _y > m_rt_tab.y && _y < m_rt_tab.y + m_rt_tab.h)
		{
			/*if(_x > m_rt_tab.x+m_rt_tab.w/2)
				m_skill_info_mode = true;
			else
				m_skill_info_mode = false;*/
			int temp_mode = (_x - m_rt_tab.x) / (m_rt_tab.w/4);
			
			if(temp_mode >= CHARACTER_INFO_MODE && temp_mode <GRADE2_INFO_MODE)
			{
				m_info_mode =(INFO_MODE)temp_mode;
			}
		}
		else if(m_focused_skill_id != -1)
			gC_vs_ui.RunDescDialog(DID_SKILL, (void *)m_focused_skill_id);
		else if (gpC_mouse_pointer->GetPickUpItem() == NULL && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
			break;
		}
		break;
		
	case M_WHEEL_UP:
//		if(m_skill_info_mode)
		if(m_info_mode == SKILL_INFO_MODE)
			m_pC_skill_scroll_bar->ScrollUp();
		EMPTY_MOVE;
		break;
		
	case M_WHEEL_DOWN:
		//if(m_skill_info_mode)
		if(m_info_mode == SKILL_INFO_MODE)
			m_pC_skill_scroll_bar->ScrollDown();
		EMPTY_MOVE;
		break;
	}
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_INFO
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INFO::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN && key == VK_ESCAPE)
	{
		//		Run(CLOSE);
	}
}

void C_VS_UI_INFO::RefreshImage()
{
	if(m_p_face)
		DeleteNew(m_p_face);
	
	m_p_face = new CSpritePack;
	m_p_face->Init(1, CDirectDraw::Is565());
	
	if(g_pProfileManager->HasProfile(g_char_slot_ingame.sz_name.c_str()))
	{
		const char *filename = g_pProfileManager->GetFilename(g_char_slot_ingame.sz_name.c_str());
		char filenamei[256];
		
		strcpy(filenamei, filename);
		strcat(filenamei, "i");
		
		if(!m_p_face->LoadFromFileSprite(0, 1, filename, filenamei ))
		{
			delete m_p_face;
			m_p_face = NULL;
		}
	}
	else
	{
		// face spk
		// portrait
		const int _VAMPIRE_OFFSET = 6;
		const int _WOMAN_OFFSET = 3;
		int spriteID;
		if (g_char_slot_ingame.bl_female)
			spriteID = g_char_slot_ingame.woman_info.face-W_FACE1+_WOMAN_OFFSET; 
		else
			spriteID = g_char_slot_ingame.man_info.face-M_FACE1;
		
		if (g_char_slot_ingame.bl_vampire)
		{
			if(g_char_slot_ingame.bl_female)
				spriteID = _VAMPIRE_OFFSET+1;
			else
				spriteID = _VAMPIRE_OFFSET;
		}
		
		if(!m_p_face->LoadFromFileSprite(0, spriteID, SPK_FACE, SPKI_FACE))
		{
			delete m_p_face;
			m_p_face = NULL;
		}
	}
}

//-----------------------------------------------------------------------------------
// Show 가 길어져서 분리했다.			by sonee
// _Show1()
//   >> Skill Info
//-----------------------------------------------------------------------------------
void	C_VS_UI_INFO::_Show1()
{
	CSpriteSurface::SetEffect(CSpriteSurface::EFFECT_GRAY_SCALE);
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if(gbl_vampire_interface)	// vampire skill info
	{
		const int skip_y=4;
		if(gpC_base->m_p_DDSurface_back->Lock())	//그림 출력할게 많으므로 lock건 상태에서 한다.
		{
			
			if(GetAttributes()->alpha)
			{
				RECT alpha_rect;
				
				// Top
				SetRect(&alpha_rect, x+10, y+9, x+w-9, y+m_rcSkillDesciption.top);
				DrawAlphaBox(&alpha_rect, 2, 0, 0, g_pUserOption->ALPHA_DEPTH);
				
				// Bottom
				SetRect(&alpha_rect, x+10, y+m_rcSkillDesciption.bottom, x+w-9, y+h-10);
				DrawAlphaBox(&alpha_rect, 2, 0, 0, g_pUserOption->ALPHA_DEPTH);
				
				// Left
				SetRect(&alpha_rect, x+10, y+m_rcSkillDesciption.top, x+m_rcSkillDesciption.left, y+m_rcSkillDesciption.bottom);
				DrawAlphaBox(&alpha_rect, 2, 0, 0, g_pUserOption->ALPHA_DEPTH);
				
				// Right
				SetRect(&alpha_rect, x+m_rcSkillDesciption.right, y+m_rcSkillDesciption.top, x+w-9, y+m_rcSkillDesciption.bottom);
				DrawAlphaBox(&alpha_rect, 2, 0, 0, g_pUserOption->ALPHA_DEPTH);
				
				gpC_global_resource->DrawDialogOnlyLocked(x, y, w, h);
			}
			else
				gpC_global_resource->DrawDialogLocked(x, y, w, h, GetAttributes()->alpha);
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x+m_rt_tab.x, y+m_rt_tab.y, C_GLOBAL_RESOURCE::TAB_SKILL);
			//아웃라인
			gpC_global_resource->DrawOutBoxLocked(x+16, y+40+skip_y, 303, 236);
			//스킬선택 탭
			gpC_global_resource->m_pC_info_spk->BltLocked(x+m_pC_skill_button_group->GetButton(m_iDomain)->x, 
				y+m_pC_skill_button_group->GetButton(m_iDomain)->y-1, m_pC_skill_button_group->GetButton(m_iDomain)->m_image_index+1);
			
			//스킬 리스트
			gpC_global_resource->m_pC_info_spk->BltLocked(x+23, y+63+skip_y, C_GLOBAL_RESOURCE::SKILL_TITLE);
			gpC_global_resource->m_pC_info_spk->BltLocked(x+23, y+63+skip_y, C_GLOBAL_RESOURCE::SKILL_BOX);
			
			if(m_iDomain >= POISON_ID && m_iDomain <= INNATE_ID && m_skill_domain == SKILLDOMAIN_VAMPIRE)
			{
				const int domain_level = (*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetDomainLevel();
				SKILL_STEP step = (SKILL_STEP)(m_iDomain-POISON_ID+SKILL_STEP_VAMPIRE_POISON);
				if((*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].IsExistSkillStep(step))
				{
					MSkillDomain::SKILL_STEP_LIST list = *((*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetSkillStepList(step));
					MSkillDomain::SKILL_STEP_LIST::iterator ss = list.begin()+m_pC_skill_scroll_bar->GetScrollPos();
					for(int i = 0; i < min( 8, list.size() - m_pC_skill_scroll_bar->GetScrollPos()); i++)
					{
						const ACTIONINFO SkillID = (ACTIONINFO)*ss;
						MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetSkillStatus(SkillID);
						const int sprID = (*g_pSkillInfoTable)[SkillID].GetSpriteID();
						
						
						//---------------------------------------
						// status는 다음과 같다. 
						//---------------------------------------
						//	MSkillDomain::SKILLSTATUS_LEARNED		// 배웠다.
						//	MSkillDomain::SKILLSTATUS_NEXT			// 다음에 배울 수 있다.
						//	MSkillDomain::SKILLSTATUS_OTHER			// 아직은 배울 수 없다.	
						//---------------------------------------
						POINT p = {x+24, y+83+i*19+skip_y};
						
						switch(status)
						{
						case MSkillDomain::SKILLSTATUS_LEARNED:
							{
								if(!GetAttributes()->alpha)
									gpC_global_resource->m_pC_info_spk->BltLocked(p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED);
								else
								{
									RECT alpha_rect = { p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK)+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED), p.y+gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED)-1 };
									DrawAlphaBox(&alpha_rect, 7, 5, 5, g_pUserOption->ALPHA_DEPTH);
									gpC_global_resource->m_pC_info_spk->BltLocked(p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, C_GLOBAL_RESOURCE::SKILL_BACK_ALPHA);
								}
								
								if((*g_pSkillInfoTable)[SkillID].IsEnable() == false)
								{
									if(sprID < C_VS_UI_SKILL::m_C_spk.GetSize() && C_VS_UI_SKILL::m_C_spk_mini[sprID].IsInit())
										gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &C_VS_UI_SKILL::m_C_spk_mini[sprID]);
								}
								if((*g_pSkillInfoTable)[SkillID].IsPassive())
								{
									if(sprID < C_VS_UI_SKILL::m_C_spk.GetSize() && C_VS_UI_SKILL::m_C_spk_mini[sprID].IsInit())
										gpC_base->m_p_DDSurface_back->BltSpriteColorSet(&p, &C_VS_UI_SKILL::m_C_spk_mini[sprID], 315);
								}
								else
								{
									if(sprID < C_VS_UI_SKILL::m_C_spk.GetSize() && C_VS_UI_SKILL::m_C_spk_mini[sprID].IsInit())
										gpC_base->m_p_DDSurface_back->BltSprite(&p, &C_VS_UI_SKILL::m_C_spk_mini[sprID]);
								}
							}
							break;
							
						default:
							if(!GetAttributes()->alpha)
								gpC_global_resource->m_pC_info_spk->BltLocked(p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, C_GLOBAL_RESOURCE::SKILL_BACK);
							else
							{
								RECT alpha_rect = { p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK)+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED),p.y+gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED)-1 };
								DrawAlphaBox(&alpha_rect, 2, 0, 0, g_pUserOption->ALPHA_DEPTH);
								gpC_global_resource->m_pC_info_spk->BltLocked(p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, C_GLOBAL_RESOURCE::SKILL_BACK_ALPHA);
							}
							
							if((*g_pSkillInfoTable)[SkillID].GetLearnLevel() <= domain_level && (*g_pSkillInfoTable)[SkillID].GetLearnLevel() >= 0)
							{
								if(sprID < C_VS_UI_SKILL::m_C_spk_mini.GetSize() && C_VS_UI_SKILL::m_C_spk_mini[sprID].IsInit())
									gpC_base->m_p_DDSurface_back->BltSpriteColor(&p, &C_VS_UI_SKILL::m_C_spk_mini[sprID], rgb_GREEN);
							}
							else
							{
								if(sprID < C_VS_UI_SKILL::m_C_spk_mini.GetSize() && C_VS_UI_SKILL::m_C_spk_mini[sprID].IsInit())
									gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &C_VS_UI_SKILL::m_C_spk_mini[sprID]);
							}
							break;
						}
						//Skill icon back
						gpC_global_resource->m_pC_info_spk->BltLocked(p.x, p.y, C_GLOBAL_RESOURCE::SKILL_ICON_BACK);
						ss++;
					}
						
					//스킬이 꽉찬상태가 아닐때의 여분처리
					for(; i < 8; i++)
					{
						POINT p = {x+24, y+83+i*19+skip_y};
						gpC_global_resource->m_pC_info_spk->BltLocked(p.x, p.y, C_GLOBAL_RESOURCE::SKILL_ICON_BACK);
						
						if(!GetAttributes()->alpha)
							gpC_global_resource->m_pC_info_spk->BltLocked(p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, C_GLOBAL_RESOURCE::SKILL_BACK);
						else
						{
							RECT alpha_rect = { p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK)+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED), p.y+gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED)-1 };
							DrawAlphaBox(&alpha_rect, 2, 0, 0, g_pUserOption->ALPHA_DEPTH);
							gpC_global_resource->m_pC_info_spk->BltLocked(p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, C_GLOBAL_RESOURCE::SKILL_BACK_ALPHA);
						}
					}
				}
			}
			m_pC_skill_button_group->Show();
			m_pC_common_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			m_pC_skill_scroll_bar->Show(x, y);
		}
			
		//글씨 찍기
		g_FL2_GetDC();
		
		char sz_temp[80];
		
		const int level_plus = 127;
		
		
		if(m_iDomain >= POISON_ID && m_iDomain <= INNATE_ID && m_skill_domain == SKILLDOMAIN_VAMPIRE)
		{
			const int domain_level = (*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetDomainLevel();
			SKILL_STEP step = (SKILL_STEP)(m_iDomain-POISON_ID+SKILL_STEP_VAMPIRE_POISON);
			if((*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].IsExistSkillStep(step))
			{
				MSkillDomain::SKILL_STEP_LIST list = *((*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetSkillStepList(step));
				
				MSkillDomain::SKILL_STEP_LIST::iterator ss = list.begin()+m_pC_skill_scroll_bar->GetScrollPos();
				for(int i = 0; i < min( 8, list.size() - m_pC_skill_scroll_bar->GetScrollPos()); i++)
				{
					const ACTIONINFO SkillID = (ACTIONINFO)*ss;
					MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetSkillStatus(SkillID);
					
					//---------------------------------------
					// status는 다음과 같다. 
					//---------------------------------------
					//	MSkillDomain::SKILLSTATUS_LEARNED		// 배웠다.
					//	MSkillDomain::SKILLSTATUS_NEXT			// 다음에 배울 수 있다.
					//	MSkillDomain::SKILLSTATUS_OTHER			// 아직은 배울 수 없다.	
					//---------------------------------------
					POINT p = {x+145, y+87+i*19+skip_y};
					
					const int learn_level = (*g_pSkillInfoTable)[SkillID].GetLearnLevel();
					const char *skill_name = (*g_pSkillInfoTable)[SkillID].GetName();

					
					DWORD shadow_color = RGB_BLACK;
					//						if(GetAttributes()->alpha)
					//							shadow_color = 0xFFFFFFFF;
					
					wsprintf(sz_temp, "%d", learn_level);
					g_PrintColorStrShadow(p.x +level_plus- g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, p.y, sz_temp, gpC_base->m_chatting_pi, RGB(70, 255, 70), shadow_color);
					
					switch(status)
					{
					case MSkillDomain::SKILLSTATUS_LEARNED:
						{
							if((*g_pSkillInfoTable)[SkillID].IsEnable() == false)
							{
								if(skill_name)
								{
									g_PrintColorStrShadow(p.x - g_GetStringWidth(skill_name, gpC_base->m_chatting_pi.hfont)/2, p.y, skill_name, gpC_base->m_chatting_pi, RGB_GRAY, shadow_color);
								}
							}
							else
								if((*g_pSkillInfoTable)[SkillID].IsPassive())
								{
									if(skill_name)
									{
										g_PrintColorStrShadow(p.x - g_GetStringWidth(skill_name, gpC_base->m_chatting_pi.hfont)/2, p.y, skill_name, gpC_base->m_chatting_pi, RGB(150, 200, 255), shadow_color);
									}
								}
								else
								{
									if(skill_name)
									{
										g_PrintColorStrShadow(p.x - g_GetStringWidth(skill_name, gpC_base->m_chatting_pi.hfont)/2, p.y, skill_name, gpC_base->m_chatting_pi, RGB_WHITE, shadow_color);
									}
								}
						}
						break;
						
					default:
						if((*g_pSkillInfoTable)[SkillID].GetLearnLevel() <= domain_level && (*g_pSkillInfoTable)[SkillID].GetLearnLevel() >= 0)
						{
							if(skill_name)
							{
								g_PrintColorStrShadow(p.x - g_GetStringWidth(skill_name, gpC_base->m_chatting_pi.hfont)/2, p.y, skill_name, gpC_base->m_chatting_pi, RGB(70, 255, 70), shadow_color);
							}
						}
						else
						{
							if(skill_name)
							{
								g_PrintColorStrShadow(p.x - g_GetStringWidth(skill_name, gpC_base->m_chatting_pi.hfont)/2, p.y, skill_name, gpC_base->m_chatting_pi, RGB_GRAY, shadow_color);
							}
						}
						break;
					}
					
					ss++;
				}
			}
		}
		m_pC_skill_button_group->ShowDescription();
		m_pC_common_button_group->ShowDescription();
		g_FL2_ReleaseDC();
		
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	else	// slayer skill info
	{
		const int domain_bar_x = 124, domain_bar_y = 69;
		const int domain_level = (*g_pSkillManager)[m_skill_domain].GetDomainLevel();
		const int exp = (*g_pSkillManager)[m_skill_domain].GetDomainExp();
		const int skip_y=4;
		int prev_exp;
		
		if(domain_level == 0)
		{
			prev_exp = 0;
		}
		else
		{
			prev_exp = (*g_pSkillManager)[m_skill_domain].GetExpInfo(domain_level-1).AccumExp;
		}
		const int next_exp = (*g_pSkillManager)[m_skill_domain].GetExpInfo(domain_level).AccumExp;
		
		if(gpC_base->m_p_DDSurface_back->Lock())	//그림 출력할게 많으므로 lock건 상태에서 한다.
		{
			if(GetAttributes()->alpha && m_iDomain != TOTAL_ID)
			{
				RECT alpha_rect;
				
				// Top
				SetRect(&alpha_rect, x+5, y+12, x+w-6, y+m_rcSkillDesciption.top);
				DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
				
				// Bottom
				SetRect(&alpha_rect, x+5, y+m_rcSkillDesciption.bottom, x+w-6, y+h-12);
				DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
				
				// Left
				SetRect(&alpha_rect, x+5, y+m_rcSkillDesciption.top, x+m_rcSkillDesciption.left, y+m_rcSkillDesciption.bottom);
				DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
				
				// Right
				SetRect(&alpha_rect, x+m_rcSkillDesciption.right, y+m_rcSkillDesciption.top, x+w-6, y+m_rcSkillDesciption.bottom);
				DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
				
				gpC_global_resource->DrawDialogOnlyLocked(x, y, w, h);
			}
			else
				gpC_global_resource->DrawDialogLocked(x, y, w, h, GetAttributes()->alpha);
			
			if(m_iDomain != TOTAL_ID && !GetAttributes()->alpha)
			{
				gpC_base->m_p_DDSurface_back->Unlock();
				//도메인바 뒤에 검은색 칠하기
				RECT rect = {x+124, y+69+skip_y, x+124+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DOMAIN_BAR), y+69+gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DOMAIN_BAR)+skip_y };
				gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
				rect.top += 20;
				rect.bottom += 20;
				gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
				gpC_base->m_p_DDSurface_back->Lock();
			}
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x+m_rt_tab.x, y+m_rt_tab.y, C_GLOBAL_RESOURCE::TAB_SKILL);
			//아웃라인
			gpC_global_resource->DrawOutBoxLocked(x+8, y+44+skip_y, 304, 249);
			//스킬선택 탭
			gpC_global_resource->m_pC_info_spk->BltLocked(x+m_pC_skill_button_group->GetButton(m_iDomain)->x, y+m_pC_skill_button_group->GetButton(m_iDomain)->y-1, m_pC_skill_button_group->GetButton(m_iDomain)->m_image_index+1);
			
			if(m_iDomain != TOTAL_ID)
			{
				//도메인 바
				gpC_global_resource->m_pC_info_spk->BltLocked(x+25, y+68+skip_y, C_GLOBAL_RESOURCE::TITLE_DOMAINLEVEL);
				gpC_global_resource->m_pC_info_spk->BltLocked(x+25, y+88+skip_y, C_GLOBAL_RESOURCE::TITLE_DOMAINEXP);
				
				gpC_global_resource->m_pC_info_spk->BltLocked(x+110, y+66+skip_y, C_GLOBAL_RESOURCE::DOMAIN_BACK);
				gpC_global_resource->m_pC_info_spk->BltLocked(x+110, y+86+skip_y, C_GLOBAL_RESOURCE::DOMAIN_BACK);
				
				//도메인 레벨 바
				Rect rect;
				rect.Set(0, 0, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DOMAIN_BAR)*domain_level/MAX_LEVEL, gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DOMAIN_BAR));
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+domain_bar_x, y+domain_bar_y+skip_y, rect, C_GLOBAL_RESOURCE::DOMAIN_BAR);
				
				//도메인 경험치 바
				rect.Set(0, 0, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DOMAIN_BAR)*(exp - prev_exp)/max(1, (next_exp - prev_exp)), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DOMAIN_BAR));
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+domain_bar_x, y+domain_bar_y+20+skip_y, rect, C_GLOBAL_RESOURCE::DOMAIN_BAR);
				
				//스킬 리스트
				gpC_global_resource->m_pC_info_spk->BltLocked(x+15, y+108+skip_y, C_GLOBAL_RESOURCE::SKILL_TITLE);
				gpC_global_resource->m_pC_info_spk->BltLocked(x+15, y+108+skip_y, C_GLOBAL_RESOURCE::SKILL_BOX);
				
				// SKILL STEP이 있으면 스킬들을 출력한다
				if(m_iDomain >= BLADE_ID && m_iDomain <= ENCHANT_ID)
				{
					int i = 0;
					
					SKILL_STEP step = (SKILL_STEP)(SKILL_STEP_APPRENTICE);
					if((*g_pSkillManager)[m_skill_domain].IsExistSkillStep(step))
					{
						MSkillDomain::SKILL_STEP_LIST list = *((*g_pSkillManager)[m_skill_domain].GetSkillStepList(step));
						
						MSkillDomain::SKILL_STEP_LIST::iterator ss = list.begin()+m_pC_skill_scroll_bar->GetScrollPos();
						
						const int level_plus = 127;
						DWORD shadow_color = RGB_BLACK;
						for(i = 0; i < min( 8, list.size() - m_pC_skill_scroll_bar->GetScrollPos()); i++)
						{
							const ACTIONINFO SkillID = (ACTIONINFO)*ss;
							MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[m_skill_domain].GetSkillStatus(SkillID);
							
							int sprID = (*g_pSkillInfoTable)[SkillID].GetSpriteID();
							
							//---------------------------------------
							// status는 다음과 같다. 
							//---------------------------------------
							//	MSkillDomain::SKILLSTATUS_LEARNED		// 배웠다.
							//	MSkillDomain::SKILLSTATUS_NEXT			// 다음에 배울 수 있다.
							//	MSkillDomain::SKILLSTATUS_OTHER			// 아직은 배울 수 없다.	
							//---------------------------------------
							
							POINT p = {x+16, y+128+i*19+skip_y};
							
							switch(status)
							{
							case MSkillDomain::SKILLSTATUS_LEARNED:
								{
									if(!GetAttributes()->alpha)
										gpC_global_resource->m_pC_info_spk->BltLocked(p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED);
									else
									{
										RECT alpha_rect = { p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK)+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED)-1, p.y+gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED)-1 };
										DrawAlphaBox(&alpha_rect, 5, 7, 7, g_pUserOption->ALPHA_DEPTH);
										gpC_global_resource->m_pC_info_spk->BltLocked(p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, C_GLOBAL_RESOURCE::SKILL_BACK_ALPHA);
									}
									
									if((*g_pSkillInfoTable)[SkillID].IsEnable() == false)
									{
										if(sprID < C_VS_UI_SKILL::m_C_spk.GetSize() && C_VS_UI_SKILL::m_C_spk_mini[sprID].IsInit())
											gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &C_VS_UI_SKILL::m_C_spk_mini[sprID]);
									}
									if((*g_pSkillInfoTable)[SkillID].IsPassive())
									{
										if(sprID < C_VS_UI_SKILL::m_C_spk.GetSize() && C_VS_UI_SKILL::m_C_spk_mini[sprID].IsInit())
											gpC_base->m_p_DDSurface_back->BltSpriteColorSet(&p, &C_VS_UI_SKILL::m_C_spk_mini[sprID], 315);
									}
									else
									{
										if(sprID < C_VS_UI_SKILL::m_C_spk.GetSize() && C_VS_UI_SKILL::m_C_spk_mini[sprID].IsInit())
											gpC_base->m_p_DDSurface_back->BltSprite(&p, &C_VS_UI_SKILL::m_C_spk_mini[sprID]);
									}
								}
								break;
								
							default:
								if(!GetAttributes()->alpha)
									gpC_global_resource->m_pC_info_spk->BltLocked(p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, C_GLOBAL_RESOURCE::SKILL_BACK);
								else
								{
									RECT alpha_rect = { p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK)+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED)-1, p.y+gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED)-1 };
									DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
									gpC_global_resource->m_pC_info_spk->BltLocked(p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, C_GLOBAL_RESOURCE::SKILL_BACK_ALPHA);
								}
								if((*g_pSkillInfoTable)[SkillID].GetLearnLevel() <= domain_level && (*g_pSkillInfoTable)[SkillID].GetLearnLevel() >= 0)
								{
									if(sprID < C_VS_UI_SKILL::m_C_spk_mini.GetSize() && C_VS_UI_SKILL::m_C_spk_mini[sprID].IsInit())
										gpC_base->m_p_DDSurface_back->BltSpriteColor(&p, &C_VS_UI_SKILL::m_C_spk_mini[sprID], rgb_GREEN);
								}
								else
								{
									if(sprID < C_VS_UI_SKILL::m_C_spk_mini.GetSize() && C_VS_UI_SKILL::m_C_spk_mini[sprID].IsInit())
										gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &C_VS_UI_SKILL::m_C_spk_mini[sprID]);
								}
								break;
							}
							
							ss++;
							
							//Skill icon back
							gpC_global_resource->m_pC_info_spk->BltLocked(p.x, p.y, C_GLOBAL_RESOURCE::SKILL_ICON_BACK);
							
						}
					}			
					//스킬개수가 8개가 안될경우 나머지부분 출력
					for(; i < 8; i++)
					{
						POINT p = { x+16, y+128+i*19+skip_y };
						gpC_global_resource->m_pC_info_spk->BltLocked(p.x, p.y, C_GLOBAL_RESOURCE::SKILL_ICON_BACK);
						if(!GetAttributes()->alpha)
							gpC_global_resource->m_pC_info_spk->BltLocked(p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, C_GLOBAL_RESOURCE::SKILL_BACK);
						else
						{
							RECT alpha_rect = { p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK)+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED)-1, p.y+gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::SKILL_BACK_HILIGHTED)-1 };
							DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
							gpC_global_resource->m_pC_info_spk->BltLocked(p.x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SKILL_ICON_BACK), p.y, C_GLOBAL_RESOURCE::SKILL_BACK_ALPHA);
						}
					}
				}
			}
			else	// TOTAL_ID 인경우
			{
				if(!GetAttributes()->alpha)
				{
					gpC_base->m_p_DDSurface_back->Unlock();
					for(int i = 0; i < 6; i++)
					{
						//도메인바 뒤에 검은색 칠하기
						RECT rect = {x+104, y+69+i*30+skip_y, x+104+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DOMAIN_BAR), y+69+i*30+gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DOMAIN_BAR)+skip_y };
						gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
					}
					gpC_base->m_p_DDSurface_back->Lock();
				}
				
				int dm_level = 0;
				int dm_sum = 0;
				for(int i = 0; i < 6; i++)
				{
					Rect rect;
					rect.Set(2, 2, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DM_BLADE+i*2)-4, 
						gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DM_BLADE+i*2)-2);
					gpC_global_resource->m_pC_info_spk->BltLockedClip(x+25, y+61+i*30+skip_y, rect, C_GLOBAL_RESOURCE::DM_BLADE+i*2);
					gpC_global_resource->m_pC_info_spk->BltLocked(x+90, y+66+i*30+skip_y, C_GLOBAL_RESOURCE::DOMAIN_BACK);
					
					//도메인 레벨 바
					if(i == 5)	// total인 경우는 따로 처리
						dm_level = dm_sum;
					else
					{
						dm_level = (*g_pSkillManager)[(SKILLDOMAIN)i].GetDomainLevel();
						dm_sum += dm_level;
					}
					rect.Set(0, 0, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DOMAIN_BAR)*dm_level/MAX_LEVEL, gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DOMAIN_BAR));
					gpC_global_resource->m_pC_info_spk->BltLockedClip(x+domain_bar_x-20, y+domain_bar_y+i*30+skip_y, rect, C_GLOBAL_RESOURCE::DOMAIN_BAR);
				}
				
			}
			m_pC_skill_button_group->Show();
			m_pC_common_button_group->Show();
			
			gpC_base->m_p_DDSurface_back->Unlock();
			
			if(m_iDomain != TOTAL_ID)
				m_pC_skill_scroll_bar->Show(x, y);
		}
		//글씨 찍기
		if(m_iDomain != TOTAL_ID)
		{
			g_FL2_GetDC();
			char sz_temp[80];
			wsprintf(sz_temp, "%d/%d",(*g_pSkillManager)[m_iDomain-BLADE_ID].GetDomainLevel(), MAX_LEVEL);
			g_PrintColorStrOut(x+domain_bar_x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DOMAIN_BAR)/2-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, y+domain_bar_y+skip_y, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
			wsprintf(sz_temp, "%d%%/100%%", (exp - prev_exp)*100/max(1, (next_exp - prev_exp)));
			g_PrintColorStrOut(x+domain_bar_x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DOMAIN_BAR)/2-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, y+domain_bar_y+20+skip_y, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
			
			
			// SKILL STEP이 있으면 스킬이름들을 출력한다
			if(m_iDomain >= BLADE_ID && m_iDomain <= ENCHANT_ID)
			{
				
				SKILL_STEP step = (SKILL_STEP)(SKILL_STEP_APPRENTICE);
				if((*g_pSkillManager)[m_skill_domain].IsExistSkillStep(step))
				{
					MSkillDomain::SKILL_STEP_LIST list = *((*g_pSkillManager)[m_skill_domain].GetSkillStepList(step));

					
					MSkillDomain::SKILL_STEP_LIST::iterator ss = list.begin()+m_pC_skill_scroll_bar->GetScrollPos();
					
					//					int i = 0;
					//					(*g_pSkillManager)[m_skill_domain].SetBegin();
					//					for(int skip = 0; skip < m_pC_skill_scroll_bar->GetScrollPos(); skip++)
					//					{
					//						(*g_pSkillManager)[m_skill_domain].Next();
					//					}
					
					const int level_plus = 127;
					DWORD shadow_color = RGB_BLACK;
					//					if(GetAttributes()->alpha)
					//						shadow_color = 0xFFFFFFFF;
					for(int i = 0; i < min( 8, list.size() - m_pC_skill_scroll_bar->GetScrollPos()); i++)
					{
						const ACTIONINFO SkillID = (ACTIONINFO)*ss;
						
						MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[m_skill_domain].GetSkillStatus(SkillID);
						//					do
						//					{
						//						ACTIONINFO SkillID = (*g_pSkillManager)[m_skill_domain].GetSkillID();
						//						MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[m_skill_domain].GetSkillStatus(SkillID);
						const char *skill_name = (*g_pSkillInfoTable)[SkillID].GetName();
						const int exp_level = (*g_pSkillInfoTable)[SkillID].GetExpLevel();
						const int learn_level = (*g_pSkillInfoTable)[SkillID].GetLearnLevel();
						char sz_temp[10];
						
						POINT p = {x+137, y+132+i*19+skip_y};
						
						//---------------------------------------
						// status는 다음과 같다. 
						//---------------------------------------
						//	MSkillDomain::SKILLSTATUS_LEARNED		// 배웠다.
						//	MSkillDomain::SKILLSTATUS_NEXT			// 다음에 배울 수 있다.
						//	MSkillDomain::SKILLSTATUS_OTHER			// 아직은 배울 수 없다.	
						//---------------------------------------
						
						switch(status)
						{
						case MSkillDomain::SKILLSTATUS_LEARNED:
							{
								if((*g_pSkillInfoTable)[SkillID].IsPassive())
								{
									g_PrintColorStrShadow(p.x +level_plus- 
										g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_PASSIVE].GetString(), gpC_base->m_chatting_pi.hfont)/2, p.y, (*g_pGameStringTable)[UI_STRING_MESSAGE_PASSIVE].GetString(), gpC_base->m_chatting_pi, RGB(150, 200, 255), shadow_color);
								}
								else
								{
									wsprintf(sz_temp, "%d", exp_level);
									g_PrintColorStrShadow(p.x +level_plus- g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, p.y, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, shadow_color);
								}
								
								if((*g_pSkillInfoTable)[SkillID].IsEnable() == false)
								{
									if(skill_name)
									{
										g_PrintColorStrShadow(p.x - g_GetStringWidth(skill_name, gpC_base->m_chatting_pi.hfont)/2, p.y, skill_name, gpC_base->m_chatting_pi, RGB_GRAY, shadow_color);
									}
								}
								else
									if((*g_pSkillInfoTable)[SkillID].IsPassive())
									{
										if(skill_name)
										{
											g_PrintColorStrShadow(p.x - g_GetStringWidth(skill_name, gpC_base->m_chatting_pi.hfont)/2, p.y, skill_name, gpC_base->m_chatting_pi, RGB(150, 200, 255), shadow_color);
										}
									}
									else
									{
										if(skill_name)
										{
											g_PrintColorStrShadow(p.x - g_GetStringWidth(skill_name, gpC_base->m_chatting_pi.hfont)/2, p.y, skill_name, gpC_base->m_chatting_pi, RGB_WHITE, shadow_color);
										}
									}
							}
							break;
							
						default:
							wsprintf(sz_temp, "%d", learn_level);
							g_PrintColorStrShadow(p.x +level_plus- g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, p.y, sz_temp, gpC_base->m_chatting_pi, RGB(70, 255, 70), shadow_color);
							
							if((*g_pSkillInfoTable)[SkillID].GetLearnLevel() <= domain_level && (*g_pSkillInfoTable)[SkillID].GetLearnLevel() >= 0)
							{
								if(skill_name)
								{
									g_PrintColorStrShadow(p.x - g_GetStringWidth(skill_name, gpC_base->m_chatting_pi.hfont)/2, p.y, skill_name, gpC_base->m_chatting_pi, RGB(70, 255, 70), shadow_color);
								}
							}
							else
							{
								if(skill_name)
								{
									g_PrintColorStrShadow(p.x - g_GetStringWidth(skill_name, gpC_base->m_chatting_pi.hfont)/2, p.y, skill_name, gpC_base->m_chatting_pi, RGB_GRAY, shadow_color);
								}
							}
							break;
						}
						
						ss++;
						//						(*g_pSkillManager)[m_skill_domain].Next();
						//						i++;
						
						//					}while(!(*g_pSkillManager)[m_skill_domain].IsEnd() && i < 8);
					}
				}
			}
			m_pC_skill_button_group->ShowDescription();
			m_pC_common_button_group->ShowDescription();
			g_FL2_ReleaseDC();
		}
		else	// TOTAL_ID 인경우
		{
			g_FL2_GetDC();
			
			int dm_level = 0, dm_sum = 0;
			char sz_temp[80];
			
			for(int i = 0; i < 6; i++)
			{
				//도메인 레벨 바
				if(i == 5)	// total인 경우는 따로 처리
					dm_level = dm_sum;
				else
				{
					dm_level = (*g_pSkillManager)[(SKILLDOMAIN)i].GetDomainLevel();
					dm_sum += dm_level;
				}
				wsprintf(sz_temp, "%d/%d", dm_level,MAX_LEVEL);
				g_PrintColorStrOut(x+domain_bar_x-20+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DOMAIN_BAR)/2-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, 
					y+domain_bar_y+i*30+skip_y, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
			}
			m_pC_skill_button_group->ShowDescription();
			m_pC_common_button_group->ShowDescription();
			g_FL2_ReleaseDC();
		}
	}	
}

//-------------------------------------------------------------------------------
//	Show 에서 분리			by sonee
//  _Show2()
//		>> Character Info
//-------------------------------------------------------------------------------
void	C_VS_UI_INFO::_Show2()
{
	if(gbl_vampire_interface)	//vampire character info
	{
		const int desc_box_x = 153,desc_box_y = 127-5;
		const int field1_x = 77, field1_x2 = 115, field1_x3 = 190, field1_x4 = 225, field1_y = m_rt_char_box.y, field1_gap = 20;
		const int field2_x = m_rt_char_box.x, field2_x2 = field2_x+55, field2_y = desc_box_y, field2_gap = 20;
		
		if(!GetAttributes()->alpha)
		{
			RECT rect;
			rect.left = x+desc_box_x;
			rect.right = x+desc_box_x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DESC_BOX);
			rect.top = y+desc_box_y;
			rect.bottom = y+desc_box_y+gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DESC_BOX);
			gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
			
			int i = 0;
			int bar_width, bar_height;
			
			bar_width = gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR);
			bar_height = gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR);
			
			rect.left = x +field1_x2;
			rect.right = rect.left + 65;
			for(i = 0; i < 2; i++)
			{
				rect.top = y+field1_y+field1_gap*i;
				rect.bottom = rect.top+bar_height;
				gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
			}
			
			if(g_char_slot_ingame.sz_guild_name.c_str() != NULL && strlen(g_char_slot_ingame.sz_guild_name.c_str()) > 0)
			{
				rect.top = y+field1_y+field1_gap*i+5;
				rect.bottom = rect.top+bar_height;
				rect.right = rect.left + 113;
				gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
				i++;
			}
			
			rect.top = y+field1_y+field1_gap*i+5;
			rect.bottom = rect.top+bar_height;
			rect.right = rect.left + 113;
			gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);

			i++;
			rect.top = y+field1_y+field1_gap*i+5;
			rect.bottom = rect.top+bar_height;
			rect.right = rect.left + 113;
			gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
			
			rect.left = x +field1_x4;
			rect.top = y+field1_y+field1_gap*0;
			rect.right = rect.left +40;
			rect.bottom = rect.top+bar_height;
			gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
			
			rect.right = rect.left + gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SMALL_BAR);
			rect.top = y+field1_y+field1_gap*1;
			rect.bottom = rect.top+bar_height+3;
			gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
			
			if(g_char_slot_ingame.bonus_point > 0)	// 보너스 포인트가 없을땐 출력하지 않는다.
			{
				rect.left = x +field1_x4+53;
				rect.top = y+field1_y+field1_gap*3+3;
				rect.right = rect.left +40;
				rect.bottom = rect.top+bar_height;
				gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
			}
			
			bar_width = gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SMALL_BAR2);
			bar_height = gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::SMALL_BAR2);
			rect.left = x +field2_x2;
			rect.right = rect.left + bar_width;
			
			for(i = 7; i >= 0; i--)
			{
				rect.top = y+field2_y+field2_gap*i;
				rect.bottom = rect.top+bar_height;
				gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
			}
		}
		
		if(gpC_base->m_p_DDSurface_back->Lock())	// 그림 출력할게 많으므로 lock건 상태에서 한다.
		{
			gpC_global_resource->m_pC_info_spk->BltLocked(x+m_rt_tab.x, y+m_rt_tab.y, C_GLOBAL_RESOURCE::TAB_CHAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x+m_rt_char_box.x, y+m_rt_char_box.y, C_GLOBAL_RESOURCE::CHAR_BOX);
			gpC_global_resource->m_pC_info_spk->BltLocked(x+desc_box_x, y+desc_box_y, C_GLOBAL_RESOURCE::DESC_BOX);
			
			//얼굴 찍기
			POINT point = {x+m_rt_char_box.x+1, y+m_rt_char_box.y+1};
			if(m_p_face)
			{
				gpC_base->m_p_DDSurface_back->BltSprite(&point, &(*m_p_face)[0]);
			}
			
			Rect rect;
			//name, align, clan출력
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x+4, y+field1_y+field1_gap*0+3, C_GLOBAL_RESOURCE::TITLE_NAME);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x+4, y+field1_y+field1_gap*1+3, C_GLOBAL_RESOURCE::TITLE_ALIGN);
			
			int gap_line=2;
			//길드마크가 있으면 길드 마크를 출력하고, 없으면 GUILD라는 글자를 출력, 길드 자체가 없으면 아예 만다-_-;
			if(g_char_slot_ingame.sz_guild_name.c_str() != NULL && strlen(g_char_slot_ingame.sz_guild_name.c_str()) > 0)
			{
				if(m_p_guild_mark)
				{
					POINT point = {x+232, y+field1_y+field1_gap*2+6};
					gpC_base->m_p_DDSurface_back->BltSprite(&point, m_p_guild_mark);
					gpC_global_resource->m_pC_info_spk->BltLocked(point.x-1, point.y-1, C_GLOBAL_RESOURCE::GUILD_BACK);
				}					
				gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x+4, y+field1_y+field1_gap*2+3+5, C_GLOBAL_RESOURCE::TITLE_CLAN);
				rect.Set(0, 0, 113, gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR));
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x +field1_x2, y+field1_y+field1_gap*2+5, rect, C_GLOBAL_RESOURCE::LARGE_BAR);
				gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2+113, y+field1_y+field1_gap*2+5, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
				gap_line++;
			}
			// Grade 출력
			if(g_char_slot_ingame.GRADE > 0 &&g_char_slot_ingame.GRADE <= GRADE_MARK_MAX)				
			{
				POINT GradePoint = { x + 232, y + field1_y + field1_gap * gap_line + 6};
				CSprite* GradeMark=g_pGuildMarkManager->GetGradeMarkSmall(g_char_slot_ingame.GRADE-1,gbl_vampire_interface);
				if(GradeMark!=NULL)
					gpC_base->m_p_DDSurface_back->BltSprite(&GradePoint,GradeMark);
			}
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x + field1_x+4, y + field1_y + field1_gap * gap_line+10,C_GLOBAL_RESOURCE::TITLE_GRADE2);				
			rect.Set(0, 0, 65, gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR));
			gpC_global_resource->m_pC_info_spk->BltLockedClip(x +field1_x2, y+field1_y+field1_gap*0, rect, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2+65, y+field1_y+field1_gap*0, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			
			gpC_global_resource->m_pC_info_spk->BltLockedClip(x +field1_x2, y+field1_y+field1_gap*1, rect, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2+65, y+field1_y+field1_gap*1, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			
			rect.Set(0, 0, 113, gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR));
			gpC_global_resource->m_pC_info_spk->BltLockedClip(x +field1_x2, y+field1_y+field1_gap*gap_line+5, rect, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2+113, y+field1_y+field1_gap*gap_line+5, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			gap_line ++;
			
			// 뱀파이어 명성
			gpC_global_resource->m_pC_info_spk->BltLocked( x+ field1_x+4, y+ field1_y + field1_gap * gap_line + 10, C_GLOBAL_RESOURCE::VAMPIRE_FAME);
			rect.Set(0, 0, 113, gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR));
			gpC_global_resource->m_pC_info_spk->BltLockedClip(x +field1_x2, y+field1_y+field1_gap*gap_line+5, rect, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2+113, y+field1_y+field1_gap*gap_line+5, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			
			// level, exp, bonus출력
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x3, y+field1_y+field1_gap*0+3, C_GLOBAL_RESOURCE::TITLE_LEVEL);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x3, y+field1_y+field1_gap*1+3, C_GLOBAL_RESOURCE::TITLE_EXP);
			if(g_char_slot_ingame.bonus_point > 0)	// 보너스 포인트가 없을땐 출력하지 않는다.
				gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x4+54, y+field1_y+field1_gap*2+6+5, C_GLOBAL_RESOURCE::TITLE_BONUS);
			
			rect.Set(0, 0, 40, gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR));
			gpC_global_resource->m_pC_info_spk->BltLockedClip(x +field1_x4, y+field1_y+field1_gap*0, rect, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x4+40, y+field1_y+field1_gap*0, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x4, y+field1_y+field1_gap*1, C_GLOBAL_RESOURCE::SMALL_BAR);
			
			if(g_char_slot_ingame.bonus_point > 0)	// 보너스 포인트가 없을땐 출력하지 않는다.
			{
				rect.Set(0, 0, 40, gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR));
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x +field1_x4+53, y+field1_y+field1_gap*3+3, rect, C_GLOBAL_RESOURCE::LARGE_BAR);
				gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x4+93, y+field1_y+field1_gap*3+3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			}
			
			// str, dex, int, hp, mp, tohit, damage, defence, protection출력
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*0+3, C_GLOBAL_RESOURCE::TITLE_STR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*1+3, C_GLOBAL_RESOURCE::TITLE_DEX);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*2+3, C_GLOBAL_RESOURCE::TITLE_INT);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*3+3, C_GLOBAL_RESOURCE::TITLE_HP);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*4+3, C_GLOBAL_RESOURCE::TITLE_TOHIT);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*5+3, C_GLOBAL_RESOURCE::TITLE_DAMAGE);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*6+3, C_GLOBAL_RESOURCE::TITLE_DEFENCE);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*7+3, C_GLOBAL_RESOURCE::TITLE_PROTECTION);
			
			
			//				if(!GetAttributes()->alpha)
			//				{
			//					RECT rect;
			//					rect.left = x+desc_box_x;
			//					rect.right = x+desc_box_x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DESC_BOX);
			//					rect.top = y+desc_box_y;
			//					rect.bottom = y+desc_box_y+gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DESC_BOX);
			//					gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
			//				}
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*7, C_GLOBAL_RESOURCE::SMALL_BAR2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*6, C_GLOBAL_RESOURCE::SMALL_BAR2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*5, C_GLOBAL_RESOURCE::SMALL_BAR2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*4, C_GLOBAL_RESOURCE::SMALL_BAR2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*3, C_GLOBAL_RESOURCE::SMALL_BAR2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*2, C_GLOBAL_RESOURCE::SMALL_BAR2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*1, C_GLOBAL_RESOURCE::SMALL_BAR2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*0, C_GLOBAL_RESOURCE::SMALL_BAR2);
			
			//EXP BAR
			if(g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level).GoalExp)
			{
				int cur_level = 0;
				if(g_char_slot_ingame.level == 1)cur_level = 0;
				else cur_level = g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level -1).AccumExp;
				
				int goal_exp = g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level).GoalExp;
				
				rect.Set(0, 0, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::EXP_BAR)*(g_char_slot_ingame.EXP_CUR-cur_level)/(g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level).GoalExp), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::EXP_BAR));
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+field1_x4+3, y+field1_y+field1_gap*2, rect, C_GLOBAL_RESOURCE::EXP_BAR);
			}
			if(gbl_vampire_interface && g_char_slot_ingame.bonus_point > 0)
				m_pC_char_button_group->Show();
			
			m_pC_common_button_group->Show();
			
			gpC_base->m_p_DDSurface_back->Unlock();
		}
			
		//글자 출력할게 많으므로 따로 모아서 한다.
		g_FL2_GetDC();
		char sz_temp[50];
		std::string sstr;
		
		////////////////field1/////////////////////
		//name
		g_PrintColorStr(x+field1_x2+5, y+field1_y+field1_gap*0+5, g_char_slot_ingame.sz_name.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);
		//align
		const static char *align_temp[5] = 
		{
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_VERY_BAD].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_BAD].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_NORMAL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_GOOD].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_VERY_GOOD].GetString(),
		};
		g_PrintColorStr(x+field1_x2+5, y+field1_y+field1_gap*1+5, align_temp[g_char_slot_ingame.alignment], gpC_base->m_chatting_pi, RGB_WHITE);
		//guild
		int gap_line=2;
		
		if(g_char_slot_ingame.sz_guild_name.c_str() != NULL&&
			!g_char_slot_ingame.sz_guild_name.empty())
			//g_char_slot_ingame.sz_guild_name.size()>0)		###@@@
		{
			g_PrintColorStr(x+field1_x2+5, y+field1_y+field1_gap*2+5+5, g_char_slot_ingame.sz_guild_name.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);
			gap_line++;
		}
		
		const static char* grade[10] = 
		{
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_RITTER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_REICHSRITTER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_BARONET].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PREYHER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_GRAF].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MARKGRAF].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PFALZGRAF].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FURST].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_HERZOG].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LANDESHER].GetString(),
		};
		
		if(g_char_slot_ingame.GRADE > 0 &&g_char_slot_ingame.GRADE <= GRADE_MARK_MAX)			
			g_PrintColorStr(x+field1_x2+5,y+field1_y+field1_gap*gap_line+5+5,grade[(g_char_slot_ingame.GRADE-1)/5],gpC_base->m_chatting_pi,RGB_WHITE);

		wsprintf(sz_temp,"%d", g_char_slot_ingame.FAME);
		sstr = sz_temp;
		for(int i = 3; i <= 13; i += 4)
			if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");

		gap_line++;
		g_PrintColorStr(x + field1_x2+5, y+field1_y+field1_gap*gap_line+5+5, sstr.c_str(), gpC_base->m_chatting_pi,RGB_WHITE);
		
		//level
		wsprintf(sz_temp, "%d", g_char_slot_ingame.level);
		g_PrintColorStr(x+field1_x4+5, y+field1_y+field1_gap*0+5, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		//exp
		// 숫자사이에 ,넣기
		wsprintf(sz_temp, "%d", g_char_slot_ingame.EXP_CUR);
		sstr = sz_temp;
		for(i = 3; i <= 13; i += 4)
			if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");

		g_PrintColorStr(x+field1_x4+5, y+field1_y+field1_gap*1+5, sstr.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);
		//bonus
		if(g_char_slot_ingame.bonus_point > 0)//보너스 포인트가 없을땐 출력하지 않는다.
		{
			wsprintf(sz_temp, "%d", g_char_slot_ingame.bonus_point);
			g_PrintColorStr(x+field1_x4+53+5, y+field1_y+field1_gap*3+5+3, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		}
		
		////////////////field2/////////////////////
		//str
		wsprintf(sz_temp, "%d/%d", g_char_slot_ingame.STR_CUR, g_char_slot_ingame.STR_MAX);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*0+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		//dex
		wsprintf(sz_temp, "%d/%d", g_char_slot_ingame.DEX_CUR, g_char_slot_ingame.DEX_MAX);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*1+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		//int
		wsprintf(sz_temp, "%d/%d", g_char_slot_ingame.INT_CUR, g_char_slot_ingame.INT_MAXX);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*2+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		//hp
		wsprintf(sz_temp, "%d/%d", g_char_slot_ingame.HP, g_char_slot_ingame.HP_MAX);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*3+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		//tohit
		wsprintf(sz_temp, "%d", g_char_slot_ingame.TOHIT);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*4+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		//damage
		int dam1, dam2;
		dam1 = g_char_slot_ingame.DAM + g_char_slot_ingame.SILVER_DAM;
		dam2 = g_char_slot_ingame.DAM2 + g_char_slot_ingame.SILVER_DAM2;
		
		if (dam1 == dam2)
			sprintf(sz_temp, "%d", dam1);
		else
			sprintf(sz_temp, "%d~%d", dam2, dam1);
		
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*5+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		//defence
		wsprintf(sz_temp, "%d", g_char_slot_ingame.DEFENSE);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*6+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		
		//protection
		wsprintf(sz_temp, "%d", g_char_slot_ingame.PROTECTION);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*7+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		
		// desc box
		int px = x+desc_box_x+5, py = y+desc_box_y+5;
				
		wsprintf(sz_temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_STR_PURE].GetString(), g_char_slot_ingame.STR_PURE);
		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		py += 16;
		
		wsprintf(sz_temp, 	(*g_pGameStringTable)[UI_STRING_MESSAGE_DEX_PURE].GetString(), g_char_slot_ingame.DEX_PURE);
		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		py += 16;
		
		wsprintf(sz_temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_INT_PURE].GetString(), g_char_slot_ingame.INT_PURE);
		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		py += 16;
		
		{
			char szBuffer[128];
			ZeroMemory(szBuffer, 128 );
			wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_STATUS_SUM_PURE].GetString(), g_char_slot_ingame.STR_PURE+g_char_slot_ingame.DEX_PURE+g_char_slot_ingame.INT_PURE);			
			memcpy(szBuffer, sz_temp+19, strlen(sz_temp) - 19 );			
			sz_temp[19] = '\0';			
			g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
			py += 12;
			g_PrintColorStr(px, py, szBuffer, gpC_base->m_chatting_pi, RGB_WHITE);
			py += 16;
			
			ZeroMemory(szBuffer, 128 ); //14
			wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_STATUS_SUM].GetString(), g_char_slot_ingame.STR_CUR+g_char_slot_ingame.DEX_CUR+g_char_slot_ingame.INT_CUR);
			memcpy(szBuffer, sz_temp + 14, strlen(sz_temp) - 14);
			sz_temp[14] = '\0';
			g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
			py += 12;		
			g_PrintColorStr(px, py, szBuffer, gpC_base->m_chatting_pi, RGB_WHITE);
			py += 16;		
		}
		
		const static char *weapon_speed_string[3] = 
		{
			(*g_pGameStringTable)[UI_STRING_MESSAGE_SPEED_SLOW].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_SPEED_NORMAL].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_SPEED_FAST].GetString(),
		};
		
		{
			char szBuffer[128];
			const int cut_pos = 9;
			ZeroMemory( szBuffer, 128 );			
			wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_ATTACK_SPEED].GetString(), weapon_speed_string[g_char_slot_ingame.WS], g_char_slot_ingame.WeaponSpeed);
			memcpy(szBuffer, sz_temp + cut_pos, strlen(sz_temp) - cut_pos );
			sz_temp[cut_pos] = '\0';
			g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
			py += 12;
			g_PrintColorStr(px, py, szBuffer, gpC_base->m_chatting_pi, RGB_WHITE);
			py += 16;
		}

//		wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_RESISTANCE_ACID].GetString(), weapon_speed_string[g_char_slot_ingame.WS], g_char_slot_ingame.WeaponSpeed);
//		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
//		py += 16;
//
//		wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_RESISTANCE_POISON].GetString(), weapon_speed_string[g_char_slot_ingame.WS], g_char_slot_ingame.WeaponSpeed);
//		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
//		py += 16;
//		
//		wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_RESISTANCE_BLOODY].GetString(), weapon_speed_string[g_char_slot_ingame.WS], g_char_slot_ingame.WeaponSpeed);
//		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
//		py += 16;		
//
//		wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_RESISTANCE_CURSE].GetString(), weapon_speed_string[g_char_slot_ingame.WS], g_char_slot_ingame.WeaponSpeed);
//		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
//		py += 16;
//		
		if(gbl_vampire_interface && g_char_slot_ingame.bonus_point > 0)
			m_pC_char_button_group->ShowDescription();
		
		m_pC_common_button_group->ShowDescription();
		
		g_FL2_ReleaseDC();
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	else	//slayer character info
	{
		const int desc_box_x = 148,desc_box_y = 127;
		const int field1_x = 82, field1_x2 = 120, field1_y = m_rt_char_box.y, field1_gap = 20;
		const int field2_x = m_rt_char_box.x, field2_x2 = field2_x+55, field2_y = desc_box_y, field2_gap = 20;
		
		
		if(!GetAttributes()->alpha)
		{
			RECT rect;
			rect.left = x+desc_box_x;
			rect.right = x+desc_box_x+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DESC_BOX);
			rect.top = y+desc_box_y;
			rect.bottom = y+desc_box_y+gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DESC_BOX);
			gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
			
			int i = 0;
			int bar_width, bar_height;
			
			bar_width = gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR);
			bar_height = gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR);
			
			rect.left = x +field1_x2;
			rect.right = rect.left + bar_width;
			i = 3;
			if(g_char_slot_ingame.sz_guild_name.c_str() != NULL && strlen(g_char_slot_ingame.sz_guild_name.c_str()) > 0)
				i++;
			
			for(; i >= 0; i--)
			{
				rect.top = y+field1_y+field1_gap*i;
				rect.bottom = rect.top+bar_height;
				gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
			}
			
			bar_width = gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SMALL_BAR2);
			bar_height = gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::SMALL_BAR2);
			rect.left = x +field2_x2;
			rect.right = rect.left + bar_width;
			
			for(i = 8; i > 2; i--)
			{
				rect.top = y+field2_y+field2_gap*i;
				rect.bottom = rect.top+bar_height;
				gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
			}
			
			bar_height = gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::SMALL_BAR);
			for(i = 2; i >= 0; i--)
			{
				rect.top = y+field2_y+field2_gap*i;
				rect.bottom = rect.top+bar_height;
				gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
			}
		}
		
		if(gpC_base->m_p_DDSurface_back->Lock())	//그림 출력할게 많으므로 lock건 상태에서 한다.
		{
			gpC_global_resource->m_pC_info_spk->BltLocked(x+m_rt_tab.x, y+m_rt_tab.y, C_GLOBAL_RESOURCE::TAB_CHAR);
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x+m_rt_char_box.x, y+m_rt_char_box.y, C_GLOBAL_RESOURCE::CHAR_BOX);
			
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x+desc_box_x, y+desc_box_y, C_GLOBAL_RESOURCE::DESC_BOX);
			
			//얼굴 찍기
			POINT point = {x+m_rt_char_box.x+1, y+m_rt_char_box.y+1};
			
			if(m_p_face)
			{
				gpC_base->m_p_DDSurface_back->BltSprite(&point, &(*m_p_face)[0]);
			}
			
			//name, fame, align, guild출력
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x, y+field1_y+field1_gap*0+3, C_GLOBAL_RESOURCE::TITLE_NAME);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x+2, y+field1_y+field1_gap*1+3, C_GLOBAL_RESOURCE::TITLE_FAME);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x+2, y+field1_y+field1_gap*2+3, C_GLOBAL_RESOURCE::TITLE_ALIGN);
			
			//길드마크가 있으면 길드 마크를 출력하고, 없으면 Clan라는 글자를 출력, 길드 자체가 없으면 아예 출력 안함
			int gap_line=3;
			if(g_char_slot_ingame.sz_guild_name.c_str() != NULL && strlen(g_char_slot_ingame.sz_guild_name.c_str()) > 0)
			{
				if(m_p_guild_mark)
				{
					POINT point = {x+258, y+field1_y+field1_gap*3};
					gpC_base->m_p_DDSurface_back->BltSprite(&point, m_p_guild_mark);
					gpC_global_resource->m_pC_info_spk->BltLocked(point.x-1, point.y-1, C_GLOBAL_RESOURCE::GUILD_BACK);
				}
				gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x, y+field1_y+field1_gap*3+3, C_GLOBAL_RESOURCE::TITLE_TEAM);					
				gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2, y+field1_y+field1_gap*3, C_GLOBAL_RESOURCE::LARGE_BAR);
				gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR), y+field1_y+field1_gap*3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
				gap_line++;
			}			
			// 계급 이미지				
			if(g_char_slot_ingame.GRADE > 0 &&g_char_slot_ingame.GRADE <= GRADE_MARK_MAX)				
			{
				CSprite* GradeMark=g_pGuildMarkManager->GetGradeMarkSmall(g_char_slot_ingame.GRADE-1,gbl_vampire_interface);
				if(GradeMark!=NULL)
				{
					POINT GradePoint = { x+258, y+field1_y+field1_gap*gap_line};
					gpC_base->m_p_DDSurface_back->BltSprite(&GradePoint,GradeMark);
				}
			}				
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x + field1_x,y+field1_y+field1_gap*gap_line+3,C_GLOBAL_RESOURCE::TITLE_GRADE);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2,y+field1_y+field1_gap*gap_line, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR), y+field1_y+field1_gap*gap_line, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2, y+field1_y+field1_gap*0, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2, y+field1_y+field1_gap*1, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2, y+field1_y+field1_gap*2, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR), y+field1_y+field1_gap*0, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR), y+field1_y+field1_gap*1, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field1_x2+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR), y+field1_y+field1_gap*2, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			
			//str, dx, int, hp, mp, tohit, damage, defence, protection출력
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*0+3, C_GLOBAL_RESOURCE::TITLE_STR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*1+3, C_GLOBAL_RESOURCE::TITLE_DEX);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*2+3, C_GLOBAL_RESOURCE::TITLE_INT);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*3+3, C_GLOBAL_RESOURCE::TITLE_HP);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*4+3, C_GLOBAL_RESOURCE::TITLE_MP);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*5+3, C_GLOBAL_RESOURCE::TITLE_TOHIT);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*6+3, C_GLOBAL_RESOURCE::TITLE_DAMAGE);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*7+3, C_GLOBAL_RESOURCE::TITLE_DEFENCE);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x, y+field2_y+field2_gap*8+3, C_GLOBAL_RESOURCE::TITLE_PROTECTION);
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*8, C_GLOBAL_RESOURCE::SMALL_BAR2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*7, C_GLOBAL_RESOURCE::SMALL_BAR2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*6, C_GLOBAL_RESOURCE::SMALL_BAR2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*5, C_GLOBAL_RESOURCE::SMALL_BAR2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*4, C_GLOBAL_RESOURCE::SMALL_BAR2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*3, C_GLOBAL_RESOURCE::SMALL_BAR2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*2, C_GLOBAL_RESOURCE::SMALL_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*1, C_GLOBAL_RESOURCE::SMALL_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field2_x2, y+field2_y+field2_gap*0, C_GLOBAL_RESOURCE::SMALL_BAR);
			
			//EXP BAR
			int cur_value = 0;
			Rect rect;
			if(g_pExperienceTable->GetSTRInfo(g_char_slot_ingame.STR_PURE).GoalExp)
			{
				if(g_char_slot_ingame.STR_PURE == 1)cur_value = 0;
				else cur_value = g_pExperienceTable->GetSTRInfo(g_char_slot_ingame.STR_PURE -1).AccumExp;
				
				rect.Set(0, 0, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::EXP_BAR)*(g_char_slot_ingame.STR_EXP_CUR-cur_value)/(g_pExperienceTable->GetSTRInfo(g_char_slot_ingame.STR_PURE).GoalExp), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::EXP_BAR));
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+field2_x2+3, y+field2_y+field2_gap*0+18, rect, C_GLOBAL_RESOURCE::EXP_BAR);
			}
			
			if(g_pExperienceTable->GetDEXInfo(g_char_slot_ingame.DEX_PURE).GoalExp)
			{
				if(g_char_slot_ingame.DEX_PURE == 1)cur_value = 0;
				else cur_value = g_pExperienceTable->GetDEXInfo(g_char_slot_ingame.DEX_PURE -1).AccumExp;
				
				rect.Set(0, 0, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::EXP_BAR)*(g_char_slot_ingame.DEX_EXP_CUR-cur_value)/(g_pExperienceTable->GetDEXInfo(g_char_slot_ingame.DEX_PURE).GoalExp), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::EXP_BAR));
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+field2_x2+3, y+field2_y+field2_gap*1+18, rect, C_GLOBAL_RESOURCE::EXP_BAR);
			}
			
			if(g_pExperienceTable->GetINTInfo(g_char_slot_ingame.INT_PURE).GoalExp)
			{
				if(g_char_slot_ingame.INT_PURE == 1)cur_value = 0;
				else cur_value = g_pExperienceTable->GetINTInfo(g_char_slot_ingame.INT_PURE -1).AccumExp;
				
				rect.Set(0, 0, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::EXP_BAR)*(g_char_slot_ingame.INT_EXP_CUR-cur_value)/(g_pExperienceTable->GetINTInfo(g_char_slot_ingame.INT_PURE).GoalExp), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::EXP_BAR));
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+field2_x2+3, y+field2_y+field2_gap*2+18, rect, C_GLOBAL_RESOURCE::EXP_BAR);
			}
			if(gbl_vampire_interface && g_char_slot_ingame.bonus_point > 0)
				m_pC_char_button_group->Show();
			
			m_pC_common_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
		}
		
		//글자 출력할게 많으므로 따로 모아서 한다.
		g_FL2_GetDC();
		char sz_temp[50];
		std::string sstr;
		
		////////////////field1/////////////////////
		//name
		g_PrintColorStr(x+field1_x2+5, y+field1_y+field1_gap*0+5, g_char_slot_ingame.sz_name.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);
		//fame
		// 숫자사이에 ,넣기
		wsprintf(sz_temp, "%d", g_char_slot_ingame.FAME);
		sstr = sz_temp;
		
		for(int i = 3; i <= 13; i += 4)
			if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
			
		g_PrintColorStr(x+field1_x2+5, y+field1_y+field1_gap*1+5, sstr.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);
		//align
		const static char* align_temp[5] = 
		{
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_VERY_BAD].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_BAD].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_NORMAL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_GOOD].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_VERY_GOOD].GetString(),
		};
		g_PrintColorStr(x+field1_x2+5, y+field1_y+field1_gap*2+5, align_temp[g_char_slot_ingame.alignment], gpC_base->m_chatting_pi, RGB_WHITE);
		//guild
		
		int gap_line=3;
		if(g_char_slot_ingame.sz_guild_name.c_str() != NULL&&
			!g_char_slot_ingame.sz_guild_name.empty())
			//g_char_slot_ingame.sz_guild_name.size()>0)###@@@
		{
			g_PrintColorStr(x+field1_x2+5, y+field1_y+field1_gap*3+5, g_char_slot_ingame.sz_guild_name.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);
			gap_line++;
		}
		// GRADE
		const static char* grade[10] = 
		{
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PIVATE].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_SERENT].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FEACEL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LITENA].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_KAINEL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_GENEAL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FORE_GENEAL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MAJORIS_GENEAL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_CLOEL_GENEAL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MARSHAL].GetString(),
		};
		
		
		if(g_char_slot_ingame.GRADE > 0 &&g_char_slot_ingame.GRADE <= GRADE_MARK_MAX)
			g_PrintColorStr(x+field1_x2+5,y+field1_y+field1_gap*gap_line+5,grade[(g_char_slot_ingame.GRADE-1)/5],gpC_base->m_chatting_pi,RGB_WHITE);
		
		
		////////////////field2/////////////////////
		
		//str
 		wsprintf(sz_temp, "%d/%d", g_char_slot_ingame.STR_CUR, g_char_slot_ingame.STR_MAX);//, g_char_slot_ingame.STR_PURE);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*0+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		
		//dex
		wsprintf(sz_temp, "%d/%d", g_char_slot_ingame.DEX_CUR, g_char_slot_ingame.DEX_MAX);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*1+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		
		//int
		wsprintf(sz_temp, "%d/%d", g_char_slot_ingame.INT_CUR, g_char_slot_ingame.INT_MAXX);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*2+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		
		//hp
		wsprintf(sz_temp, "%d/%d", g_char_slot_ingame.HP, g_char_slot_ingame.HP_MAX);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*3+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		
		//mp
		wsprintf(sz_temp, "%d/%d", g_char_slot_ingame.MP, g_char_slot_ingame.MP_MAX);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*4+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		
		//tohit
		wsprintf(sz_temp, "%d", g_char_slot_ingame.TOHIT);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*5+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		
		//damage
		int dam1, dam2;
		dam1 = g_char_slot_ingame.DAM + g_char_slot_ingame.SILVER_DAM;
		dam2 = g_char_slot_ingame.DAM2 + g_char_slot_ingame.SILVER_DAM2;
		
		if (dam1 == dam2)
			sprintf(sz_temp, "%d", dam1);
		else
			sprintf(sz_temp, "%d~%d", dam2, dam1);
		
		if(g_char_slot_ingame.SILVER_DAM > 0)
			g_PrintColorStrOut(x+field2_x2+5, y+field2_y+field2_gap*6+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLUE);
		else
			g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*6+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		
		//defence
		wsprintf(sz_temp, "%d", g_char_slot_ingame.DEFENSE);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*7+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		
		//protection
		wsprintf(sz_temp, "%d", g_char_slot_ingame.PROTECTION);
		g_PrintColorStr(x+field2_x2+5, y+field2_y+field2_gap*8+4, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		
		// desc box
		int px = x+desc_box_x+5, py = y+desc_box_y+5;
		
		wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_STR_PURE].GetString(), g_char_slot_ingame.STR_PURE);
		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		py += 16;
		
		wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DEX_PURE].GetString(), g_char_slot_ingame.DEX_PURE);
		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		py += 16;
		
		wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_INT_PURE].GetString(), g_char_slot_ingame.INT_PURE);
		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		py += 16;
		
//		wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_STATUS_SUM_PURE].GetString(), g_char_slot_ingame.STR_PURE+g_char_slot_ingame.DEX_PURE+g_char_slot_ingame.INT_PURE);
//		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
//		py += 16;
		{
			char szBuffer[128];
			ZeroMemory(szBuffer, 128 );
			wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_STATUS_SUM_PURE].GetString(), g_char_slot_ingame.STR_PURE+g_char_slot_ingame.DEX_PURE+g_char_slot_ingame.INT_PURE);			
			memcpy(szBuffer, sz_temp+19, strlen(sz_temp) - 19 );			
			sz_temp[19] = '\0';			
			g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
			py += 12;
			g_PrintColorStr(px, py, szBuffer, gpC_base->m_chatting_pi, RGB_WHITE);
			py += 16;
			
			ZeroMemory(szBuffer, 128 ); //14
			wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_STATUS_SUM].GetString(), g_char_slot_ingame.STR_CUR+g_char_slot_ingame.DEX_CUR+g_char_slot_ingame.INT_CUR);
			memcpy(szBuffer, sz_temp + 14, strlen(sz_temp) - 14);
			sz_temp[14] = '\0';
			g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
			py += 12;		
			g_PrintColorStr(px, py, szBuffer, gpC_base->m_chatting_pi, RGB_WHITE);
			py += 16;		
		}
		
		const static char *weapon_speed_string[3] = 
		{
			(*g_pGameStringTable)[UI_STRING_MESSAGE_SPEED_SLOW].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_SPEED_NORMAL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_SPEED_FAST].GetString(),
		};
		
		{
			char szBuffer[128];
			const int cut_pos = 9;
			ZeroMemory( szBuffer, 128 );			
			wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_ATTACK_SPEED].GetString(), weapon_speed_string[g_char_slot_ingame.WS], g_char_slot_ingame.WeaponSpeed);
			memcpy(szBuffer, sz_temp + cut_pos, strlen(sz_temp) - cut_pos );
			sz_temp[cut_pos] = '\0';
			g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
			py += 12;
			g_PrintColorStr(px, py, szBuffer, gpC_base->m_chatting_pi, RGB_WHITE);
			py += 16;
		}
//		wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_RESISTANCE_ACID].GetString(), weapon_speed_string[g_char_slot_ingame.WS], g_char_slot_ingame.WeaponSpeed);
//		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
//		py += 16;
//
//		wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_RESISTANCE_POISON].GetString(), weapon_speed_string[g_char_slot_ingame.WS], g_char_slot_ingame.WeaponSpeed);
//		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
//		py += 16;
//		
//		wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_RESISTANCE_BLOODY].GetString(), weapon_speed_string[g_char_slot_ingame.WS], g_char_slot_ingame.WeaponSpeed);
//		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
//		py += 16;		
//
//		wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_RESISTANCE_CURSE].GetString(), weapon_speed_string[g_char_slot_ingame.WS], g_char_slot_ingame.WeaponSpeed);
//		g_PrintColorStr(px, py, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
//		py += 16;
//		
		if(gbl_vampire_interface && g_char_slot_ingame.bonus_point > 0)
			m_pC_char_button_group->ShowDescription();
		
		m_pC_common_button_group->ShowDescription();
		g_FL2_ReleaseDC();
	}	
}

//------------------------------------------------------------------------------
//	Show 에서 분리			by sonee
//	_Show3()
//		>> Grade1
//------------------------------------------------------------------------------
void	C_VS_UI_INFO::_Show3()
{
	int grade_text_x=37,grade_text_y=40;
	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);

	
	if(gbl_vampire_interface)
	{
		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			bool drawGrade = false;

			if(g_char_slot_ingame.GRADE > 0 && g_char_slot_ingame.GRADE <= GRADE_MARK_MAX )
				drawGrade = true;

			gpC_global_resource->m_pC_info_spk->BltLocked(x+m_rt_tab.x, y+m_rt_tab.y, C_GLOBAL_RESOURCE::TAB_GRADE1);
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x+m_pC_grade1_button_group->GetButton(m_iGrade)->x,
				y+m_pC_grade1_button_group->GetButton(m_iGrade)->y-1, m_pC_grade1_button_group->GetButton(m_iGrade)->m_image_index+1);
			gpC_global_resource->m_pC_info_spk->BltLocked(x+grade_text_x,y+grade_text_y,C_GLOBAL_RESOURCE::GRADE_LEVEL_HILIGHTED);
			gpC_global_resource->m_pC_info_spk->BltLocked(x+grade_text_x,y+grade_text_y+20,C_GLOBAL_RESOURCE::GRADE_EXP_HILIGHTED);
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x+grade_text_x+73,y+grade_text_y-2,C_GLOBAL_RESOURCE::GRADE_BAR_BACK);
			gpC_global_resource->m_pC_info_spk->BltLocked(x+grade_text_x+73,y+grade_text_y+18,C_GLOBAL_RESOURCE::GRADE_BAR_BACK);
			
			const int barWidth = gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::GRADE_BAR);
			const int barHeight = gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::GRADE_BAR);

			int NextLevelExp,PrevLevelExp;
			if(drawGrade)
			{
				NextLevelExp = g_pExperienceTable->GetVampireRankInfo(g_char_slot_ingame.GRADE).AccumExp;
				PrevLevelExp = (g_char_slot_ingame.GRADE == 1) ? 0 : g_pExperienceTable->GetVampireRankInfo(g_char_slot_ingame.GRADE-1).AccumExp;
			}
			if(NextLevelExp == 0 ) NextLevelExp = 1;

			RECT box_rect = {x+grade_text_x+92,y+grade_text_y+1,x+grade_text_x+92+barWidth,y+grade_text_y+1+barHeight};
			
			gpC_base->m_p_DDSurface_back->Unlock();
			gpC_base->m_p_DDSurface_back->FillRect(&box_rect, 0);
			box_rect.top+=20;
			box_rect.bottom+=20;
			gpC_base->m_p_DDSurface_back->FillRect(&box_rect, 0);
			gpC_base->m_p_DDSurface_back->Lock();
			
//			// BAR
//			if(drawGrade)
//			{
//				Rect rect(0,0,(barWidth*(min(GRADE_MARK_MAX,g_char_slot_ingame.GRADE))/GRADE_MARK_MAX),barHeight);
//				
//				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+grade_text_x+92,y+grade_text_y+1,rect,C_GLOBAL_RESOURCE::GRADE_BAR);				
//				rect.Set(0,0,
//					max(0,min(barWidth,(barWidth*(g_char_slot_ingame.GRADE_EXP-PrevLevelExp))/max(1,(NextLevelExp-PrevLevelExp)))),
//					barHeight);
//				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+grade_text_x+92,y+grade_text_y+21,rect,C_GLOBAL_RESOURCE::GRADE_BAR);
//			}
			int	exp = g_char_slot_ingame.GRADE_EXP-PrevLevelExp;
			int	goal_exp = max(1,(NextLevelExp-PrevLevelExp));
			float	rate = (float)exp / (float)goal_exp;

			if(drawGrade)
			{
				Rect rect(0,0,(barWidth*(min(GRADE_MARK_MAX,g_char_slot_ingame.GRADE))/GRADE_MARK_MAX),barHeight);
				
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+grade_text_x+92,y+grade_text_y+1,rect,C_GLOBAL_RESOURCE::GRADE_BAR);

				rect.Set(0, 0, max(0,min(barWidth,int((float)barWidth * rate))), barHeight);  // hyungony
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+grade_text_x+92,y+grade_text_y+21,rect,C_GLOBAL_RESOURCE::GRADE_BAR);
			}

			m_pC_grade1_button_group->Show();
			gpC_global_resource->DrawOutBoxLocked(x+18, y+122, 298, 180);
			m_pC_common_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			g_FL2_GetDC();
			char sz_temp[80];
			if(drawGrade)
			{
				wsprintf(sz_temp, "%d/50", g_char_slot_ingame.GRADE);
				g_PrintColorStrOut(x+grade_text_x+87+barWidth/2-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, y+grade_text_y+1, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
				wsprintf(sz_temp, "%d%%/100%%", int(100 * rate)); // hyungony
				g_PrintColorStrOut(x+grade_text_x+87+barWidth/2-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, y+grade_text_y+21, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
			}

//			if(drawGrade)
//			{
//				wsprintf(sz_temp, "%d/50", g_char_slot_ingame.GRADE);
//				g_PrintColorStrOut(x+grade_text_x+87+barWidth/2-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, y+grade_text_y+1, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
//				wsprintf(sz_temp, "%d%%/100%%", 
//					(100*(g_char_slot_ingame.GRADE_EXP-PrevLevelExp))/max(1,(NextLevelExp-PrevLevelExp)));
//				g_PrintColorStrOut(x+grade_text_x+87+barWidth/2-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, y+grade_text_y+21, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
//			}
			g_FL2_ReleaseDC();			
		}
	} else
	{
		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			bool drawGrade = false;
			if(g_char_slot_ingame.GRADE > 0 && g_char_slot_ingame.GRADE <= GRADE_MARK_MAX )
				drawGrade=true;
			gpC_global_resource->m_pC_info_spk->BltLocked(x+m_rt_tab.x, y+m_rt_tab.y, C_GLOBAL_RESOURCE::TAB_GRADE1);
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x+m_pC_grade1_button_group->GetButton(m_iGrade)->x,
				y+m_pC_grade1_button_group->GetButton(m_iGrade)->y-1, m_pC_grade1_button_group->GetButton(m_iGrade)->m_image_index+1);
			gpC_global_resource->m_pC_info_spk->BltLocked(x+grade_text_x,y+grade_text_y,C_GLOBAL_RESOURCE::GRADE_LEVEL_HILIGHTED);
			gpC_global_resource->m_pC_info_spk->BltLocked(x+grade_text_x,y+grade_text_y+20,C_GLOBAL_RESOURCE::GRADE_EXP_HILIGHTED);
			gpC_global_resource->m_pC_info_spk->BltLocked(x+grade_text_x+73,y+grade_text_y-2,C_GLOBAL_RESOURCE::DOMAIN_BACK);
			gpC_global_resource->m_pC_info_spk->BltLocked(x+grade_text_x+73,y+grade_text_y+18,C_GLOBAL_RESOURCE::DOMAIN_BACK);

			const int barWidth = gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DOMAIN_BAR);
			const int barHeight = gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::DOMAIN_BAR);

			
			int NextLevelExp, PrevLevelExp;
			if(drawGrade)
			{
				NextLevelExp = g_pExperienceTable->GetSlayerRankInfo(g_char_slot_ingame.GRADE).AccumExp;				
				PrevLevelExp = (g_char_slot_ingame.GRADE == 1) ? 0 : g_pExperienceTable->GetSlayerRankInfo(g_char_slot_ingame.GRADE - 1).AccumExp;
			}

			if(NextLevelExp == 0) NextLevelExp = 1;
			RECT box_rect = {x+grade_text_x+87,y+grade_text_y+1,x+grade_text_x+87+barWidth,y+grade_text_y+1+barHeight};
			gpC_base->m_p_DDSurface_back->Unlock();
			gpC_base->m_p_DDSurface_back->FillRect(&box_rect, 0);
			box_rect.top+=20;
			box_rect.bottom+=20;
			gpC_base->m_p_DDSurface_back->FillRect(&box_rect, 0);
			gpC_base->m_p_DDSurface_back->Lock();
//			// BAR
//			if(drawGrade)
//			{
//				Rect rect(0,0,(barWidth*min(GRADE_MARK_MAX,g_char_slot_ingame.GRADE)/GRADE_MARK_MAX),barHeight);				
//				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+grade_text_x+87,y+grade_text_y+1,rect,C_GLOBAL_RESOURCE::DOMAIN_BAR);
//				rect.Set(0,0,
//					max(0,min(barWidth,(barWidth*(g_char_slot_ingame.GRADE_EXP-PrevLevelExp))/max(1,(NextLevelExp-PrevLevelExp)))),
//					barHeight);
//				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+grade_text_x+87,y+grade_text_y+21,rect,C_GLOBAL_RESOURCE::DOMAIN_BAR);
//			}			
			//BAR

			int	exp = g_char_slot_ingame.GRADE_EXP-PrevLevelExp;
			int	goal_exp = max(1,(NextLevelExp-PrevLevelExp));
			float	rate = (float)exp / (float)goal_exp;

			if(drawGrade)
			{
				Rect rect(0,0,(barWidth*min(GRADE_MARK_MAX,g_char_slot_ingame.GRADE)/GRADE_MARK_MAX),barHeight);				
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+grade_text_x+87,y+grade_text_y+1,rect,C_GLOBAL_RESOURCE::DOMAIN_BAR);

				rect.Set(0, 0, max(0,min(barWidth,  int((float)barWidth * rate))), barHeight); // hyungony
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x+grade_text_x+87,y+grade_text_y+21,rect,C_GLOBAL_RESOURCE::DOMAIN_BAR);
			}
			m_pC_grade1_button_group->Show();
			gpC_global_resource->DrawOutBoxLocked(x+15, y+120, 292, 176);
			m_pC_common_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			g_FL2_GetDC();
			char sz_temp[80];
//			if(drawGrade)
//			{
//				wsprintf(sz_temp, "%d/50", g_char_slot_ingame.GRADE);
//				g_PrintColorStrOut(x+grade_text_x+87+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DOMAIN_BAR)/2-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, y+grade_text_y+1, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
//				wsprintf(sz_temp, "%d%%/100%%", 
//					(100*(g_char_slot_ingame.GRADE_EXP-PrevLevelExp))/max(1,(NextLevelExp-PrevLevelExp)));
//				//wsprintf(sz_temp, "%d%%/100%%", (exp - prev_exp)*100/max(1, (next_exp - prev_exp)));
//				g_PrintColorStrOut(x+grade_text_x+87+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DOMAIN_BAR)/2-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, y+grade_text_y+21, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
//			}
			if(drawGrade)
			{
				wsprintf(sz_temp, "%d/50", g_char_slot_ingame.GRADE);
				g_PrintColorStrOut(x+grade_text_x+87+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DOMAIN_BAR)/2-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, y+grade_text_y+1, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
			
				wsprintf(sz_temp, "%d%%/100%%", int(100 * rate)); // hyungony		
				g_PrintColorStrOut(x+grade_text_x+87+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::DOMAIN_BAR)/2-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, y+grade_text_y+21, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);
			}
			g_FL2_ReleaseDC();
		}
	}		
	m_pC_common_button_group->ShowDescription();
	m_pC_grade1_button_group->ShowDescription();
}

//------------------------------------------------------------------------------
//  Show 에서 분리			by sonee
//	_Show4()
//		>> Grade2
//------------------------------------------------------------------------------
void	C_VS_UI_INFO::_Show4()
{
	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_global_resource->m_pC_info_spk->BltLocked(x+m_rt_tab.x, y+m_rt_tab.y, C_GLOBAL_RESOURCE::TAB_GRADE2);		
		m_pC_common_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	m_pC_common_button_group->ShowDescription();
}

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INFO::Show()
{
	/*
	if(m_skill_info_mode)	//Skill Info Mode
	{
		_Show1();
	}
	else	// Character Info Mode
	{
		gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);
		_Show2();
		m_pC_char_scroll_bar->Show(x, y);
	}*/
	///*
	switch(m_info_mode)
	{
	case CHARACTER_INFO_MODE :
		gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);
		_Show2();
		m_pC_char_scroll_bar->Show(x, y);
		break;
	case SKILL_INFO_MODE :
		_Show1();
		break;
	case GRADE1_INFO_MODE :		
		_Show3();
		break;
	case GRADE2_INFO_MODE :		
		_Show4();
		break;
	}//*/	
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INFO::Start(INFO_MODE	Info_Mode)
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::INFO));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::INFO);
	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}
	
	//길드마크 로딩
	m_p_guild_mark = g_pGuildMarkManager->GetGuildMarkSmall(g_char_slot_ingame.GUILD_ID);
	
	if (m_p_guild_mark==NULL)
	{
		//-------------------------------------------------
		// file에 있는지 본다.
		//-------------------------------------------------
		g_pGuildMarkManager->LoadGuildMark(g_char_slot_ingame.GUILD_ID);
		
		//-------------------------------------------------
		// file에서 load되었는지 다시 체크
		//-------------------------------------------------
		m_p_guild_mark = g_pGuildMarkManager->GetGuildMarkSmall(g_char_slot_ingame.GUILD_ID);
	}
	
	PI_Processor::Start();
	
	//	m_pC_button_group->Init();
	m_pC_common_button_group->Init();
	m_pC_char_button_group->Init();
	m_pC_skill_button_group->Init();
	
	m_info_mode = Info_Mode;
	
	gpC_window_manager->AppearWindow(this);
	
	EMPTY_MOVE;
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INFO::Start(bool skill_mode)
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::INFO));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::INFO);
	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}
	
	//길드마크 로딩
	m_p_guild_mark = g_pGuildMarkManager->GetGuildMarkSmall(g_char_slot_ingame.GUILD_ID);
	
	if (m_p_guild_mark==NULL)
	{
		//-------------------------------------------------
		// file에 있는지 본다.
		//-------------------------------------------------
		g_pGuildMarkManager->LoadGuildMark(g_char_slot_ingame.GUILD_ID);
		
		//-------------------------------------------------
		// file에서 load되었는지 다시 체크
		//-------------------------------------------------
		m_p_guild_mark = g_pGuildMarkManager->GetGuildMarkSmall(g_char_slot_ingame.GUILD_ID);
	}
	
	PI_Processor::Start();
	
	//	m_pC_button_group->Init();
	m_pC_common_button_group->Init();
	m_pC_char_button_group->Init();
	m_pC_skill_button_group->Init();
	
	//m_skill_info_mode = skill_mode;
	if(skill_mode)
		m_info_mode = SKILL_INFO_MODE;
	else
		m_info_mode = CHARACTER_INFO_MODE;
	
	gpC_window_manager->AppearWindow(this);
	
	EMPTY_MOVE;
}

void C_VS_UI_INFO::Finish()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::INFO, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::INFO, Rect(x, y, w, h));
	
	//	if(gpC_global_resource->m_pC_info_spk)
	//	{
	//		DeleteNew(m_pC_info_spk);
	//		m_pC_info_spk = NULL;
	//	}
	
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
	
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INFO::Process()
{
	m_pC_common_button_group->Process();

	switch(m_info_mode)
	{
	case CHARACTER_INFO_MODE :
		m_pC_char_button_group->Process();
		if(g_char_slot_ingame.bl_vampire && g_char_slot_ingame.bonus_point > 0)
			gC_vs_ui.LevelUp();
		else
			gC_vs_ui.FinishLevelUp();
		break;
	case SKILL_INFO_MODE :
		m_pC_skill_button_group->Process();
		break;
	case GRADE1_INFO_MODE :
		m_pC_grade1_button_group->Process();
		break;
	}
	/*if(m_skill_info_mode)
		m_pC_skill_button_group->Process();
	else
	{
		m_pC_char_button_group->Process();
		if(g_char_slot_ingame.bl_vampire && g_char_slot_ingame.bonus_point > 0)
			gC_vs_ui.LevelUp();
		else
			gC_vs_ui.FinishLevelUp();
	}*/
}


//-----------------------------------------------------------------------------
// C_VS_UI_HPBAR
//
//-----------------------------------------------------------------------------
C_VS_UI_HPBAR::C_VS_UI_HPBAR()
{
	m_pC_hpbar_spk = NULL;
	
	AttrTopmost(false);
	AttrPin(true);
	
	m_width_mode = true;//false;
	m_small_mode = false;
	m_small_offset = 0;	
	
	g_RegisterWindow(this);
	
	if(gbl_vampire_interface)
	{
		m_pC_hpbar_spk = new C_SPRITE_PACK(SPK_HPBAR_VAMPIRE);
	}
	else
	{
		m_pC_hpbar_spk = new C_SPRITE_PACK(SPK_HPBAR_SLAYER);
	}
	Set(0, 0, m_pC_hpbar_spk->GetWidth(), m_pC_hpbar_spk->GetHeight());
	
	//skillinfo 버튼
	m_pC_width_button_group = new ButtonGroup(this);
	m_pC_height_button_group = new ButtonGroup(this);
	m_pC_small_width_button_group = new ButtonGroup(this);
	m_pC_small_height_button_group = new ButtonGroup(this);
	
	if(gbl_vampire_interface)
	{
		m_pC_small_width_button_group->Add(new C_VS_UI_EVENT_BUTTON(133, 3, m_pC_hpbar_spk->GetWidth(CHANGE_BUTTON+VAMPIRE_MAX), m_pC_hpbar_spk->GetHeight(CHANGE_BUTTON+VAMPIRE_MAX), CHANGE_ID, this, CHANGE_BUTTON+VAMPIRE_MAX));
		m_pC_small_width_button_group->Add(new C_VS_UI_EVENT_BUTTON(4, 14, m_pC_hpbar_spk->GetWidth(SMALL_BUTTON+VAMPIRE_MAX), m_pC_hpbar_spk->GetHeight(SMALL_BUTTON+VAMPIRE_MAX), RESIZE_ID, this, SMALL_BUTTON+VAMPIRE_MAX));
		m_pC_small_height_button_group->Add(new C_VS_UI_EVENT_BUTTON(2, 4, m_pC_hpbar_spk->GetWidth(CHANGE_BUTTON+VAMPIRE_MAX), m_pC_hpbar_spk->GetHeight(CHANGE_BUTTON+VAMPIRE_MAX), CHANGE_ID, this, CHANGE_BUTTON+VAMPIRE_MAX));
		m_pC_small_height_button_group->Add(new C_VS_UI_EVENT_BUTTON(14, 136, m_pC_hpbar_spk->GetWidth(SMALL_BUTTON+VAMPIRE_MAX), m_pC_hpbar_spk->GetHeight(SMALL_BUTTON+VAMPIRE_MAX), RESIZE_ID, this, SMALL_BUTTON+VAMPIRE_MAX));
		m_pC_width_button_group->Add(new C_VS_UI_EVENT_BUTTON(200, 3, m_pC_hpbar_spk->GetWidth(CHANGE_BUTTON), m_pC_hpbar_spk->GetHeight(CHANGE_BUTTON), CHANGE_ID, this, CHANGE_BUTTON));
		m_pC_width_button_group->Add(new C_VS_UI_EVENT_BUTTON(7, 21, m_pC_hpbar_spk->GetWidth(SMALL_BUTTON), m_pC_hpbar_spk->GetHeight(SMALL_BUTTON), RESIZE_ID, this, SMALL_BUTTON));
		m_pC_height_button_group->Add(new C_VS_UI_EVENT_BUTTON(3, 6, m_pC_hpbar_spk->GetWidth(CHANGE_BUTTON), m_pC_hpbar_spk->GetHeight(CHANGE_BUTTON), CHANGE_ID, this, CHANGE_BUTTON));
		m_pC_height_button_group->Add(new C_VS_UI_EVENT_BUTTON(21, 204, m_pC_hpbar_spk->GetWidth(SMALL_BUTTON), m_pC_hpbar_spk->GetHeight(SMALL_BUTTON), RESIZE_ID, this, SMALL_BUTTON));
	}
	else
	{
		m_pC_small_width_button_group->Add(new C_VS_UI_EVENT_BUTTON(133, 4, m_pC_hpbar_spk->GetWidth(CHANGE_BUTTON_WIDTH+SLAYER_MAX), m_pC_hpbar_spk->GetHeight(CHANGE_BUTTON_WIDTH+SLAYER_MAX), CHANGE_ID, this, CHANGE_BUTTON_WIDTH+SLAYER_MAX));
		m_pC_small_width_button_group->Add(new C_VS_UI_EVENT_BUTTON(2, 4, m_pC_hpbar_spk->GetWidth(CHANGE_BUTTON_WIDTH+SLAYER_MAX), m_pC_hpbar_spk->GetHeight(CHANGE_BUTTON_WIDTH+SLAYER_MAX), RESIZE_ID, this, CHANGE_BUTTON_WIDTH+SLAYER_MAX));
		m_pC_small_height_button_group->Add(new C_VS_UI_EVENT_BUTTON(4, 4, m_pC_hpbar_spk->GetWidth(CHANGE_BUTTON_HEIGHT+SLAYER_MAX), m_pC_hpbar_spk->GetHeight(CHANGE_BUTTON_HEIGHT+SLAYER_MAX), CHANGE_ID, this, CHANGE_BUTTON_HEIGHT+SLAYER_MAX));
		m_pC_small_height_button_group->Add(new C_VS_UI_EVENT_BUTTON(4, 135, m_pC_hpbar_spk->GetWidth(CHANGE_BUTTON_HEIGHT+SLAYER_MAX), m_pC_hpbar_spk->GetHeight(CHANGE_BUTTON_HEIGHT+SLAYER_MAX), RESIZE_ID, this, CHANGE_BUTTON_HEIGHT+SLAYER_MAX));
		m_pC_width_button_group->Add(new C_VS_UI_EVENT_BUTTON(199, 5, m_pC_hpbar_spk->GetWidth(CHANGE_BUTTON_WIDTH), m_pC_hpbar_spk->GetHeight(CHANGE_BUTTON_WIDTH), CHANGE_ID, this, CHANGE_BUTTON_WIDTH));
		m_pC_width_button_group->Add(new C_VS_UI_EVENT_BUTTON(2, 5, m_pC_hpbar_spk->GetWidth(CHANGE_BUTTON_WIDTH), m_pC_hpbar_spk->GetHeight(CHANGE_BUTTON_WIDTH), RESIZE_ID, this, CHANGE_BUTTON_WIDTH));
		m_pC_height_button_group->Add(new C_VS_UI_EVENT_BUTTON(5, 6, m_pC_hpbar_spk->GetWidth(CHANGE_BUTTON_HEIGHT), m_pC_hpbar_spk->GetHeight(CHANGE_BUTTON_HEIGHT), CHANGE_ID, this, CHANGE_BUTTON_HEIGHT));
		m_pC_height_button_group->Add(new C_VS_UI_EVENT_BUTTON(5, 203, m_pC_hpbar_spk->GetWidth(CHANGE_BUTTON_HEIGHT), m_pC_hpbar_spk->GetHeight(CHANGE_BUTTON_HEIGHT), RESIZE_ID, this, CHANGE_BUTTON_HEIGHT));
	}
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_HPBAR
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_HPBAR::~C_VS_UI_HPBAR()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::HPBAR, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::HPBAR, Rect(x, y, w, h));
	gpC_vs_ui_window_manager->SetHPBarHeight(!m_width_mode);
	gpC_vs_ui_window_manager->SetHPBarSmall(m_small_mode);
	
	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_hpbar_spk);
	DeleteNew(m_pC_width_button_group);
	DeleteNew(m_pC_height_button_group);
	DeleteNew(m_pC_small_width_button_group);
	DeleteNew(m_pC_small_height_button_group);
}


void	C_VS_UI_HPBAR::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char* hpbar_string[4] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_SET_LARGE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_SET_SMALL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_SET_WIDTH].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_SET_HEIGHT].GetString(),		
	};
	
	if(p_button->GetID() == CHANGE_ID && m_width_mode == false)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)hpbar_string[2],0,0);
	else if(p_button->GetID() == RESIZE_ID && m_small_mode)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)hpbar_string[3],0,0);
	else
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)hpbar_string[p_button->GetID()],0,0);
}
//-----------------------------------------------------------------------------
// C_VS_UI_HPBAR::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_HPBAR::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(gbl_vampire_interface)
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_pC_hpbar_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
			else
				m_pC_hpbar_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
		}
		else
		{
			if(p_button->GetPressState())
				m_pC_hpbar_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				m_pC_hpbar_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}
	}
	else
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_pC_hpbar_spk->BltLocked(x+p_button->x, y+p_button->y+1, p_button->m_image_index+1);
			else
				m_pC_hpbar_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
		}
		else
		{
			if(p_button->GetPressState())
				m_pC_hpbar_spk->BltLocked(x+p_button->x, y+p_button->y+1, p_button->m_image_index);
			else
				m_pC_hpbar_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}
	}
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_HPBAR::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_HPBAR::IsPixel(int _x, int _y)
{
	if(Moving()) return true;
	return IsInRect(_x, _y);
	//	if(m_width_mode)
	//		return m_pC_hpbar_spk->IsPixel(_x-x, _y-y, MAIN_WIDTH);
	//	else
	//		return m_pC_hpbar_spk->IsPixel(_x-x, _y-y, MAIN_HEIGHT);
	
	//	return false;
	//	return m_pC_charinfo->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), CHARINFO_WINDOW);
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_HPBAR::Run(id_t id)
{
	switch (id)
	{
	case CHANGE_ID:
		m_width_mode = !m_width_mode;
		w^=h; h^=w; w^=h; //swap
		x -= w-h;
		EMPTY_MOVE;
		break;
		
	case RESIZE_ID:
		m_small_mode = !m_small_mode;
		if(m_small_mode)
		{
			if(gbl_vampire_interface)
				m_small_offset = VAMPIRE_MAX;
			else
				m_small_offset = SLAYER_MAX;
		}
		else m_small_offset = 0;
		if(m_width_mode)
		{
			w = m_pC_hpbar_spk->GetWidth(MAIN_WIDTH+m_small_offset);
			h = m_pC_hpbar_spk->GetHeight(MAIN_WIDTH+m_small_offset);
		}
		else
		{
			w = m_pC_hpbar_spk->GetWidth(MAIN_HEIGHT+m_small_offset);
			h = m_pC_hpbar_spk->GetHeight(MAIN_HEIGHT+m_small_offset);
		}
		EMPTY_MOVE;
		break;
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_HPBAR::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	
	bool re;
	int descType = 0;
	if(gbl_vampire_interface)
	{
		if(m_small_mode)
		{
			if(m_width_mode)
			{
				re = m_pC_small_width_button_group->MouseControl(message, _x, _y);
				if(_x > 14 && _x < 115 && _y > 3 && _y < 15)
					descType = 1; // hp desc
				if(_x > 14 && _x < 115 && _y > 15 && _y < 22)
					descType = 2; // mp desc
			}
			else
			{
				re = m_pC_small_height_button_group->MouseControl(message, _x, _y);
				if(_y < h-14 && _y > h-115 && _x > 3 && _x < 15)
					descType = 1; // hp desc
				if(_y < h-14 && _y > h-115 && _x > 15 && _x < 22)
					descType = 2; // mp desc
			}
		}
		else
		{
			if(m_width_mode)
			{
				re = m_pC_width_button_group->MouseControl(message, _x, _y);
				if(_x > 25 && _x < 172 && _y > 3 && _y < 23)
					descType = 1; // hp desc
				if(_x > 25 && _x < 172 && _y > 23 && _y < 34)
					descType = 2; // mp desc
			}
			else
			{
				re = m_pC_height_button_group->MouseControl(message, _x, _y);
				if(_y < h-25 && _y > h-172 && _x > 3 && _x < 23)
					descType = 1; // hp desc
				if(_y < h-25 && _y > h-172 && _x > 23 && _x < 34)
					descType = 2; // mp desc
			}
		}
	}
	else
	{
		if(m_small_mode)
		{
			if(m_width_mode)
			{
				re = m_pC_small_width_button_group->MouseControl(message, _x, _y);
				if(_x > 14 && _x < 120 && _y > 3 && _y < 15)
					descType = 1; // hp desc
				if(_x > 14 && _x < 120 && _y > 15 && _y < 22)
					descType = 2; // mp desc
			}
			else
			{
				re = m_pC_small_height_button_group->MouseControl(message, _x, _y);
				if(_y < h-14 && _y > h-120 && _x > 3 && _x < 15)
					descType = 1; // hp desc
				if(_y < h-14 && _y > h-120 && _x > 15 && _x < 22)
					descType = 2; // mp desc
			}
		}
		else
		{
			if(m_width_mode)
			{
				re = m_pC_width_button_group->MouseControl(message, _x, _y);
				if(_x > 20 && _x < 182 && _y > 3 && _y < 23)
					descType = 1; // hp desc
				if(_x > 20 && _x < 182 && _y > 23 && _y < 34)
					descType = 2; // mp desc
			}
			else
			{
				re = m_pC_height_button_group->MouseControl(message, _x, _y);
				if(_y < h-20 && _y > h-182 && _x > 3 && _x < 23)
					descType = 1; // hp desc
				if(_y < h-20 && _y > h-182 && _x > 23 && _x < 34)
					descType = 2; // mp desc
			}
		}
	}
	if(descType != 0)
	{
		static std::string hpbar_string[2];
		const static char *help_string[3] = {NULL, NULL, NULL};
		char temp_string[512];
		
		help_string[1] = NULL;
		help_string[2] = NULL;
		
		
		
		int print_x = gpC_mouse_pointer->GetPointerX();
		int print_y = gpC_mouse_pointer->GetPointerY();
		int color = 0;
		
		if(descType == 2)	// MP
		{
			if(gbl_vampire_interface)	// exp
			{
				int LeftExp = g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level).AccumExp - g_char_slot_ingame.EXP_CUR;
				wsprintf(temp_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_LEVEL_DESCRIPTION].GetString(), g_char_slot_ingame.level);
				hpbar_string[0]=temp_string;
				
				int fame = g_pFameInfoTable->GetFameForLevel( SKILLDOMAIN_VAMPIRE, g_char_slot_ingame.level );
				
				if(LeftExp < 0)
				{
					hpbar_string[1]=(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_UP_LEVEL].GetString();					
				}
				else
				if(g_char_slot_ingame.FAME < fame)
				{
					wsprintf(temp_string, "%s(%s:%d)",(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_UP_LEVEL_BY_FAME].GetString(), 
						(*g_pGameStringTable)[UI_STRING_MESSAGE_NEED_FAME].GetString(), 
						fame- g_char_slot_ingame.FAME);
					hpbar_string[1] = temp_string;
				}
				else
				{
					std::string temp[3];
					char szTemp[256]={0,};
					wsprintf(szTemp,"%d",g_char_slot_ingame.EXP_CUR);
					temp[0] = szTemp;
					for(int i = 3; i <= 13; i += 4)
						if(temp[0].size() > i) temp[0].insert(temp[0].size()-i, ",");
					wsprintf(szTemp,"%d",g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level).AccumExp);
					temp[1] = szTemp;
					for(i = 3; i <= 13; i += 4)
						if(temp[1].size() > i)temp[1].insert(temp[1].size()-i, ",");
					wsprintf(szTemp,"%d",LeftExp);
					temp[2] = szTemp;
					for(i = 3; i <= 13; i += 4)
						if(temp[2].size() > i)temp[2].insert(temp[2].size()-i, ",");
					
					wsprintf(temp_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_EXP_DESCRIPTION].GetString(), 
						temp[0].c_str(),temp[1].c_str(),temp[2].c_str());
//						g_char_slot_ingame.EXP_CUR, 
//						g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level).AccumExp, 	LeftExp);
					hpbar_string[1]=temp_string;
				}
				help_string[2] = hpbar_string[1].c_str();				
			}
			else	// mp
			{
				wsprintf(temp_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_MP_DESCRIPTION].GetString(), g_char_slot_ingame.MP, g_char_slot_ingame.MP_MAX);
				hpbar_string[0]=temp_string;
			}
		}
		else	// HP
		{
#ifndef _LIB
			g_char_slot_ingame.SILVER_HP = 10;
#endif
			if(gbl_vampire_interface && g_char_slot_ingame.SILVER_HP > 0)				
				wsprintf(temp_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_HP_DESCRIPTION_WITH_SILVERING].GetString(), g_char_slot_ingame.HP, g_char_slot_ingame.HP_MAX, g_char_slot_ingame.SILVER_HP);
			else
				wsprintf(temp_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_HP_DESCRIPTION].GetString(), g_char_slot_ingame.HP, g_char_slot_ingame.HP_MAX);
			
			hpbar_string[0]=temp_string;
			if(!gbl_vampire_interface && g_char_slot_ingame.bl_drained)
			{
#ifndef _LIB
				g_char_slot_ingame.CHANGE_VAMPIRE = 29;
#endif
				if(g_char_slot_ingame.CHANGE_VAMPIRE > 60)
				{
					if(g_char_slot_ingame.CHANGE_VAMPIRE > 60*24)
						wsprintf(temp_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_CHANGE_VAMPIRE_DAY].GetString(), g_char_slot_ingame.CHANGE_VAMPIRE/60/24, (g_char_slot_ingame.CHANGE_VAMPIRE/60)%24, g_char_slot_ingame.CHANGE_VAMPIRE%60);
					else
					{
						wsprintf(temp_string,(*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_CHANGE_VAMPIRE_HOUR].GetString(), g_char_slot_ingame.CHANGE_VAMPIRE/60, g_char_slot_ingame.CHANGE_VAMPIRE%60);
						color = RGB_YELLOW;
					}
					hpbar_string[1] = temp_string;
				}
				else
				{
					if(g_char_slot_ingame.CHANGE_VAMPIRE == 0)
						hpbar_string[1]=(*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_CHANGE_VAMPIRE_SOON].GetString();
					else
					{
						wsprintf(temp_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_CHANGE_VAMPIRE_MINUTE].GetString(),
							g_char_slot_ingame.CHANGE_VAMPIRE);
						hpbar_string[1]=temp_string;
					}					
					color = RGB_RED;
				}
				help_string[2] = hpbar_string[1].c_str();
			}			
		}		
		help_string[0] = hpbar_string[0].c_str();
		g_descriptor_manager.Set(DID_HELP, print_x+30, print_y, (void *)help_string, 0, color);
	}
	
	switch(message)
	{
	case M_MOVING:
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if (gpC_mouse_pointer->GetPickUpItem() == false && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
			break;
		}
		break;
		
	}
	
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_HPBAR
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_HPBAR::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN && key == VK_ESCAPE)
	{
		//		Run(CLOSE);
	}
}

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_HPBAR::Show()
{
	int hp = min(max(0, g_char_slot_ingame.HP), g_char_slot_ingame.HP_MAX);
	
	if(gbl_vampire_interface)	//vampire hpbar
	{
		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			if(m_width_mode)
			{
				m_pC_hpbar_spk->BltLocked(x, y, MAIN_WIDTH+m_small_offset);
				if(m_small_mode)
					m_pC_small_width_button_group->Show();
				else
					m_pC_width_button_group->Show();
				
				int hpbar_x = 19, hpbar_y = 4, expbar_x = 19, expbar_y = 17;
				if(!m_small_mode)
				{
					hpbar_x = 28; hpbar_y = 6; expbar_x = 28; expbar_y = 25;
				}
				
				Rect rect;
				
				// hpbar
				rect.Set(0, 0, m_pC_hpbar_spk->GetWidth(HPBAR_WIDTH+m_small_offset)*hp/g_char_slot_ingame.HP_MAX, m_pC_hpbar_spk->GetHeight(HPBAR_WIDTH+m_small_offset));
				m_pC_hpbar_spk->BltLockedClip(x+hpbar_x, y+hpbar_y, rect, HPBAR_WIDTH+m_small_offset);
				
				// slivered hpbar
				rect.Set(m_pC_hpbar_spk->GetWidth(HPBAR_SILVER_WIDTH+m_small_offset)-m_pC_hpbar_spk->GetWidth(HPBAR_SILVER_WIDTH+m_small_offset)*g_char_slot_ingame.SILVER_HP/g_char_slot_ingame.HP_MAX, 0, m_pC_hpbar_spk->GetWidth(HPBAR_SILVER_WIDTH+m_small_offset)*g_char_slot_ingame.SILVER_HP/g_char_slot_ingame.HP_MAX, m_pC_hpbar_spk->GetHeight(HPBAR_SILVER_WIDTH+m_small_offset));
				m_pC_hpbar_spk->BltLockedClip(x+hpbar_x, y+hpbar_y, rect, HPBAR_SILVER_WIDTH+m_small_offset);
				
				int cur_level = 0;
				if(g_char_slot_ingame.level == 1)cur_level = 0;
				else cur_level = g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level -1).AccumExp;
				
				//exp bar
				int exp = g_char_slot_ingame.EXP_CUR-cur_level;
				int goal_exp = g_pExperienceTable->GetVampireInfo( g_char_slot_ingame.level ).GoalExp;
				int exp_width = m_pC_hpbar_spk->GetWidth(EXPBAR_WIDTH+m_small_offset);
				int exp_height = m_pC_hpbar_spk->GetHeight(EXPBAR_WIDTH+m_small_offset);
				int exp_bar = int((float)exp_width * ((float)exp / (float)goal_exp));
				
				//exp bar
				rect.Set(0, 0, exp_bar, exp_height );
//				rect.Set(0, 0, m_pC_hpbar_spk->GetWidth(EXPBAR_WIDTH+m_small_offset)*(g_char_slot_ingame.EXP_CUR-cur_level)/(g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level).GoalExp), m_pC_hpbar_spk->GetHeight(EXPBAR_WIDTH+m_small_offset));
				m_pC_hpbar_spk->BltLockedClip(x+expbar_x, y+expbar_y, rect, EXPBAR_WIDTH+m_small_offset);
			}
			else
			{
				m_pC_hpbar_spk->BltLocked(x, y, MAIN_HEIGHT+m_small_offset);
				if(m_small_mode)
					m_pC_small_height_button_group->Show();
				else
					m_pC_height_button_group->Show();
				
				int hpbar_x = 4, hpbar_y = 31, expbar_x = 17, expbar_y = 31;
				if(!m_small_mode)
				{
					hpbar_x = 6; hpbar_y = 47; expbar_x = 25; expbar_y = 47;
				}
				
				Rect rect;
				
				// hpbar
				rect.Set(0, m_pC_hpbar_spk->GetHeight(HPBAR_HEIGHT+m_small_offset)-m_pC_hpbar_spk->GetHeight(HPBAR_HEIGHT+m_small_offset)*hp/g_char_slot_ingame.HP_MAX, m_pC_hpbar_spk->GetWidth(HPBAR_HEIGHT+m_small_offset), m_pC_hpbar_spk->GetHeight(HPBAR_HEIGHT+m_small_offset)*hp/g_char_slot_ingame.HP_MAX);
				m_pC_hpbar_spk->BltLockedClip(x+hpbar_x, y+hpbar_y, rect, HPBAR_HEIGHT+m_small_offset);
				
				// silver hpbar
				rect.Set(0, 0, m_pC_hpbar_spk->GetWidth(HPBAR_SILVER_HEIGHT+m_small_offset), m_pC_hpbar_spk->GetHeight(HPBAR_SILVER_HEIGHT+m_small_offset)*g_char_slot_ingame.SILVER_HP/g_char_slot_ingame.HP_MAX);
				m_pC_hpbar_spk->BltLockedClip(x+hpbar_x, y+hpbar_y, rect, HPBAR_SILVER_HEIGHT+m_small_offset);
				
				int cur_level = 0;
				if(g_char_slot_ingame.level == 1)cur_level = 0;
				else cur_level = g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level -1).AccumExp;
				
				const int goal_exp = g_pExperienceTable->GetVampireInfo(g_char_slot_ingame.level).GoalExp;
				int exp = max(0, min(goal_exp, g_char_slot_ingame.EXP_CUR-cur_level));
		
				int exp_width = m_pC_hpbar_spk->GetWidth(EXPBAR_HEIGHT+m_small_offset);
				int exp_height = m_pC_hpbar_spk->GetHeight(EXPBAR_HEIGHT+m_small_offset);
				int exp_bar = int((float)exp_height * ((float)exp / (float)goal_exp));
				
				//exp bar
				rect.Set(0, exp_height - exp_bar, exp_width, exp_bar);

				//exp bar
		//		rect.Set(0, m_pC_hpbar_spk->GetHeight(EXPBAR_HEIGHT+m_small_offset)-m_pC_hpbar_spk->GetHeight(EXPBAR_HEIGHT+m_small_offset)*exp/goal_exp, m_pC_hpbar_spk->GetWidth(EXPBAR_HEIGHT+m_small_offset), m_pC_hpbar_spk->GetHeight(EXPBAR_HEIGHT+m_small_offset)*exp/goal_exp);
				m_pC_hpbar_spk->BltLockedClip(x+expbar_x, y+expbar_y, rect, EXPBAR_HEIGHT+m_small_offset);
			}
			gpC_base->m_p_DDSurface_back->Unlock();
		}
		if(m_width_mode)
		{
			if(m_small_mode)
				m_pC_small_width_button_group->ShowDescription();
			else
				m_pC_width_button_group->ShowDescription();
			
		} else
		{
			if(m_small_mode)
				m_pC_small_height_button_group->ShowDescription();
			else
				m_pC_height_button_group->ShowDescription();
		}
	}
	else	// slayer hpbar
	{
		int mp = min(max(0, g_char_slot_ingame.MP), g_char_slot_ingame.MP_MAX);
		
		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			if(m_width_mode)
			{
				m_pC_hpbar_spk->BltLocked(x, y, MAIN_WIDTH+m_small_offset);
				if(m_small_mode)
					m_pC_small_width_button_group->Show();
				else
					m_pC_width_button_group->Show();
				
				int hpbar_x = 19, hpbar_y = 3, mpbar_x = 19, mpbar_y = 16;
				if(!m_small_mode)
				{
					hpbar_x = 28; hpbar_y = 4; mpbar_x = 28; mpbar_y = 22;
				}
				
				
				Rect rect;
				//hpbar
				rect.Set(0, 0, m_pC_hpbar_spk->GetWidth(HPBAR_WIDTH+m_small_offset)*hp/g_char_slot_ingame.HP_MAX, m_pC_hpbar_spk->GetHeight(HPBAR_WIDTH+m_small_offset));
				if(g_char_slot_ingame.bl_drained)
					m_pC_hpbar_spk->BltLockedClip(x+hpbar_x, y+hpbar_y, rect, HPBAR_BLOOD_WIDTH+m_small_offset);
				else
					m_pC_hpbar_spk->BltLockedClip(x+hpbar_x, y+hpbar_y, rect, HPBAR_WIDTH+m_small_offset);
				
				//mpbar
				rect.Set(0, 0, m_pC_hpbar_spk->GetWidth(MPBAR_WIDTH+m_small_offset)*mp/g_char_slot_ingame.MP_MAX, m_pC_hpbar_spk->GetHeight(MPBAR_WIDTH+m_small_offset));
				m_pC_hpbar_spk->BltLockedClip(x+mpbar_x, y+mpbar_y, rect, MPBAR_WIDTH+m_small_offset);
			}
			else
			{
				m_pC_hpbar_spk->BltLocked(x, y, MAIN_HEIGHT+m_small_offset);
				if(m_small_mode)
					m_pC_small_height_button_group->Show();
				else
					m_pC_height_button_group->Show();
				
				int hpbar_x = 3, hpbar_y = 28, mpbar_x = 16, mpbar_y = 28;
				if(!m_small_mode)
				{
					hpbar_x = 4; hpbar_y = 43; mpbar_x = 23; mpbar_y = 43;
				}
				
				Rect rect;
				//hpbar
				rect.Set(0, m_pC_hpbar_spk->GetHeight(HPBAR_HEIGHT+m_small_offset)-m_pC_hpbar_spk->GetHeight(HPBAR_HEIGHT+m_small_offset)*hp/g_char_slot_ingame.HP_MAX, m_pC_hpbar_spk->GetWidth(HPBAR_HEIGHT+m_small_offset), m_pC_hpbar_spk->GetHeight(HPBAR_HEIGHT+m_small_offset)*hp/g_char_slot_ingame.HP_MAX);
				if(g_char_slot_ingame.bl_drained)
					m_pC_hpbar_spk->BltLockedClip(x+hpbar_x, y+hpbar_y, rect, HPBAR_BLOOD_HEIGHT+m_small_offset);
				else
					m_pC_hpbar_spk->BltLockedClip(x+hpbar_x, y+hpbar_y, rect, HPBAR_HEIGHT+m_small_offset);
				
				//mpbar
				rect.Set(0, m_pC_hpbar_spk->GetHeight(MPBAR_HEIGHT+m_small_offset)-m_pC_hpbar_spk->GetHeight(MPBAR_HEIGHT+m_small_offset)*mp/g_char_slot_ingame.MP_MAX, m_pC_hpbar_spk->GetWidth(MPBAR_HEIGHT+m_small_offset), m_pC_hpbar_spk->GetHeight(MPBAR_HEIGHT+m_small_offset)*mp/g_char_slot_ingame.MP_MAX);
				m_pC_hpbar_spk->BltLockedClip(x+mpbar_x, y+mpbar_y, rect, MPBAR_HEIGHT+m_small_offset);
			}
			gpC_base->m_p_DDSurface_back->Unlock();
		}
		if(m_width_mode)
		{
			if(m_small_mode)
				m_pC_small_width_button_group->ShowDescription();
			else
				m_pC_width_button_group->ShowDescription();
			
		} else
		{
			if(m_small_mode)
				m_pC_small_height_button_group->ShowDescription();
			else
				m_pC_height_button_group->ShowDescription();
			
		}
	}
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_HPBAR::Start()
{
	PI_Processor::Start();
	
	m_pC_small_width_button_group->Init();
	m_pC_small_height_button_group->Init();
	m_pC_width_button_group->Init();
	m_pC_height_button_group->Init();
	
	gpC_window_manager->AppearWindow(this);
	
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::HPBAR));
	if(m_width_mode == gpC_vs_ui_window_manager->IsHPBarHeight())Run(CHANGE_ID);
	if(gpC_vs_ui_window_manager->IsHPBarSmall() != m_small_mode)Run(RESIZE_ID);

	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::HPBAR);
	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}
	
}

void C_VS_UI_HPBAR::Finish()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::HPBAR, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::HPBAR, Rect(x, y, w, h));
	gpC_vs_ui_window_manager->SetHPBarHeight(!m_width_mode);
	gpC_vs_ui_window_manager->SetHPBarSmall(m_small_mode);
	
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
	
}

//-----------------------------------------------------------------------------
// Process
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_HPBAR::Process()
{
	ProcessHide();
	if(m_small_mode)
	{
		if(m_width_mode)
			m_pC_small_width_button_group->Process();
		else
			m_pC_small_height_button_group->Process();
	}
	else
	{
		if(m_width_mode)
			m_pC_width_button_group->Process();
		else
			m_pC_height_button_group->Process();
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_EFFECT_STATUS
//
//-----------------------------------------------------------------------------
C_VS_UI_EFFECT_STATUS::C_VS_UI_EFFECT_STATUS()
{
	m_scroll = 0;
	m_bl_effect_size_null = true;
	
	m_pC_effect_status_spk = NULL;
	
	AttrTopmost(false);
	AttrPin(true);
	
	m_width_mode = true;//false;
	
	g_RegisterWindow(this);
	
	if(gbl_vampire_interface)
	{
		m_pC_effect_status_spk = new C_SPRITE_PACK(SPK_EFFECT_STATUS_VAMPIRE);
	}
	else
	{
		m_pC_effect_status_spk = new C_SPRITE_PACK(SPK_EFFECT_STATUS_SLAYER);
	}
	Set(240, 0, m_pC_effect_status_spk->GetWidth(), m_pC_effect_status_spk->GetHeight());
	
	m_pC_width_button_group = new ButtonGroup(this);
	m_pC_height_button_group = new ButtonGroup(this);
	
	const int width_pushpin_x = 5, width_pushpin_y = 8, width_change_x = 20, width_change_y = 7, width_left_x = 54, width_right_x = 37;
	m_pC_width_button_group->Add(new C_VS_UI_EVENT_BUTTON(width_pushpin_x, width_pushpin_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), PUSHPIN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN));
	m_pC_width_button_group->Add(new C_VS_UI_EVENT_BUTTON(width_change_x, width_change_y, m_pC_effect_status_spk->GetWidth(BUTTON_CHANGE), m_pC_effect_status_spk->GetHeight(BUTTON_CHANGE), CHANGE_ID, this, BUTTON_CHANGE));
	m_pC_width_button_group->Add(new C_VS_UI_EVENT_BUTTON(width_left_x, width_change_y, m_pC_effect_status_spk->GetWidth(BUTTON_LEFT), m_pC_effect_status_spk->GetHeight(BUTTON_LEFT), UP_ID, this, BUTTON_LEFT));
	m_pC_width_button_group->Add(new C_VS_UI_EVENT_BUTTON(width_right_x, width_change_y, m_pC_effect_status_spk->GetWidth(BUTTON_RIGHT), m_pC_effect_status_spk->GetHeight(BUTTON_RIGHT), DOWN_ID, this, BUTTON_RIGHT));
	
	const int height_pushpin_y = 5, height_pushpin_x = 7, height_change_y = 20, height_change_x = 6, height_left_y = 54, height_right_y = 37;
	m_pC_height_button_group->Add(new C_VS_UI_EVENT_BUTTON(height_pushpin_x, height_pushpin_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), PUSHPIN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN));
	m_pC_height_button_group->Add(new C_VS_UI_EVENT_BUTTON(height_change_x, height_change_y, m_pC_effect_status_spk->GetWidth(BUTTON_CHANGE), m_pC_effect_status_spk->GetHeight(BUTTON_CHANGE), CHANGE_ID, this, BUTTON_CHANGE));
	m_pC_height_button_group->Add(new C_VS_UI_EVENT_BUTTON(height_change_x, height_left_y, m_pC_effect_status_spk->GetWidth(BUTTON_UP), m_pC_effect_status_spk->GetHeight(BUTTON_UP), UP_ID, this, BUTTON_UP));
	m_pC_height_button_group->Add(new C_VS_UI_EVENT_BUTTON(height_change_x, height_right_y, m_pC_effect_status_spk->GetWidth(BUTTON_DOWN), m_pC_effect_status_spk->GetHeight(BUTTON_DOWN), DOWN_ID, this, BUTTON_DOWN));
	
	
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_EFFECT_STATUS
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_EFFECT_STATUS::~C_VS_UI_EFFECT_STATUS()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::EFFECT_STATUS, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::EFFECT_STATUS, Rect(x, y, w, h));
	gpC_vs_ui_window_manager->SetEffectStatusHeight(!m_width_mode);
	
	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_effect_status_spk);
	DeleteNew(m_pC_width_button_group);
	DeleteNew(m_pC_height_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_EFFECT_STATUS::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_EFFECT_STATUS::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char* m_effect_button_string[4] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HEIGHT].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_WIDTH].GetString(),
	};
	if(m_width_mode)
	{
		if(p_button->GetID()==PUSHPIN_ID)
		{
			if(GetAttributes()->autohide)
				g_descriptor_manager.Set(DID_INFO, x+w-p_button->x-p_button->w, y+p_button->y, (void *)m_effect_button_string[1],0,0);
			else
				g_descriptor_manager.Set(DID_INFO, x+w-p_button->x-p_button->w, y+p_button->y, (void *)m_effect_button_string[0],0,0);
		}
		if(p_button->GetID()==CHANGE_ID)
		{
			g_descriptor_manager.Set(DID_INFO, x+w-p_button->x-p_button->w, y+p_button->y, (void *)m_effect_button_string[2],0,0);
		}
	} else
	{
		if(p_button->GetID()==PUSHPIN_ID)
		{
			if(GetAttributes()->autohide)
				g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+h-p_button->y-p_button->h, (void *)m_effect_button_string[1],0,0);
			else
				g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+h-p_button->y-p_button->h, (void *)m_effect_button_string[0],0,0);
		}
		if(p_button->GetID()==CHANGE_ID)
		{
			g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+h-p_button->y-p_button->h, (void *)m_effect_button_string[3],0,0);
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_EFFECT_STATUS::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_EFFECT_STATUS::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(m_width_mode)	// width mode
	{
		if(p_button->GetID() == PUSHPIN_ID)
		{
			if(GetAttributes()->autohide)
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-p_button->w, y+p_button->y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+w-p_button->x-p_button->w, y+p_button->y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
		}
		else
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_pC_effect_status_spk->BltLocked(x+w-p_button->x-p_button->w, y+p_button->y, p_button->m_image_index+2);
				else
					m_pC_effect_status_spk->BltLocked(x+w-p_button->x-p_button->w, y+p_button->y, p_button->m_image_index+1);
			}
			else
				m_pC_effect_status_spk->BltLocked(x+w-p_button->x-p_button->w, y+p_button->y, p_button->m_image_index);
		}
	}
	else	// height mode
	{
		if(p_button->GetID() == PUSHPIN_ID)
		{
			if(GetAttributes()->autohide)				
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
			else				
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);			
		}
		else
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_pC_effect_status_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index+2);
				else
					m_pC_effect_status_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index+1);
			}
			else
				m_pC_effect_status_spk->BltLocked(x+p_button->x, y+h-p_button->y-p_button->h, p_button->m_image_index);
		}
	}
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_EFFECT_STATUS::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_EFFECT_STATUS::IsPixel(int _x, int _y)
{
	if(Moving()) return true;
	return IsInRect(_x, _y);
	//	if(m_width_mode)
	//		return m_pC_EFFECT_STATUS_spk->IsPixel(_x-x, _y-y, MAIN_WIDTH);
	//	else
	//		return m_pC_EFFECT_STATUS_spk->IsPixel(_x-x, _y-y, MAIN_HEIGHT);
	
	//	return false;
	//	return m_pC_charinfo->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), CHARINFO_WINDOW);
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_EFFECT_STATUS::Run(id_t id)
{
	switch (id)
	{
	case PUSHPIN_ID:
		if(GetAttributes()->autohide)
			AttrAutoHide(ATTRIBUTES_HIDE_NOT);
		else
		{
			if(m_width_mode)
				AttrAutoHide(ATTRIBUTES_HIDE_HEIGHT);
			else
				AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
		}
		EMPTY_MOVE;
		break;
		
	case CHANGE_ID:
		if(m_width_mode)
		{
			if(GetAttributes()->autohide != ATTRIBUTES_HIDE_NOT)
				AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
		}
		else
		{
			if(GetAttributes()->autohide != ATTRIBUTES_HIDE_NOT)
				AttrAutoHide(ATTRIBUTES_HIDE_HEIGHT);
		}
		m_width_mode = !m_width_mode;
		w^=h; h^=w; w^=h; //swap
		x -= w-h;
		EMPTY_MOVE;
		break;
		
	case UP_ID:
		if(m_scroll > 0)m_scroll--;
		break;
		
	case DOWN_ID:
		if(m_scroll+10 < g_char_slot_ingame.STATUS.size()+g_pUserInformation->WarInfo.size())
			m_scroll++;
		break;
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_EFFECT_STATUS::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	
	static int skill_id = -1;
	bool re;
	if(m_width_mode)
		re = m_pC_width_button_group->MouseControl(message, w-_x, _y);
	else
		re = m_pC_height_button_group->MouseControl(message, _x, h-_y);
	
	switch(message)
	{
	case M_MOVING:
		skill_id = -1;
		if(!g_pUserInformation->WarInfo.empty())
		{
			int select = -1;
			
			if(m_width_mode)
			{
				if(_x >= 5 && _x < 5+10*20 && _y >= 5 && _y < 5+20)
				{
					select = (_x-5)/20+m_scroll;
				}
			}
			else
			{
				if(_y >= 5 && _y < 5+10*20 && _x >= 5 && _x < 5+20)
				{
					select = (_y-5)/20+m_scroll;
				}
			}

			if(select != -1 && select < g_char_slot_ingame.STATUS.size() + g_pUserInformation->WarInfo.size() )
			{
				if(select >= g_char_slot_ingame.STATUS.size() && select <= g_char_slot_ingame.STATUS.size() + g_pUserInformation->WarInfo.size())		// 상태가 아니라 전쟁 관련이면
				{
					int count = select - g_char_slot_ingame.STATUS.size();
					const WAR_INFO& ifo = g_pUserInformation->WarInfo[count];					
					char temp2[512]="";
					
					
					static std::string str[2];

					str[0] = ifo.zone_name;
					DWORD	current_time=timeGetTime()/1000;
					char temp_string[512]="";

					if(current_time > ifo.left_time)
							current_time = ifo.left_time;

					DWORD c_time = ifo.left_time - current_time;
						
					DWORD hours = c_time / 3600;
					DWORD min = (c_time - (hours * 3600) )/60;

					if(ifo.war_type == 0)
					{
						sprintf(temp2,(*g_pGameStringTable)[UI_STRING_MESSAGE_STATUS_TIME_FORMAT].GetString(),hours,min);
						sprintf(temp_string,"%s, %s : %s, %s : %s, %s : %s",
							(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_WAR].GetString(),
							(*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_ATTACK_GUILD].GetString(),
							ifo.attack_guild_name.c_str(),
							(*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_DEFENSE_GUILD].GetString(),
							ifo.defense_guild_name.c_str(),
							(*g_pGameStringTable)[UI_STRING_MESSAGE_STATUS_LEFT_TIME].GetString(),
							temp2);
					
						static const char *change_vampire_string[3] ;
						
						long color = RGB_YELLOW;
						str[1] = temp_string;
						change_vampire_string[0]=str[0].c_str();					
						change_vampire_string[1]=NULL;
						change_vampire_string[2]=str[1].c_str();
						g_descriptor_manager.Set(DID_HELP, x+_x, y+_y, (void *)change_vampire_string, 0, color);
					} else
					{
						sprintf(temp2,(*g_pGameStringTable)[UI_STRING_MESSAGE_STATUS_TIME_FORMAT].GetString(),hours,min);
						sprintf(temp_string,"%s, %s : %s",
							(*g_pGameStringTable)[UI_STRING_MESSAGE_RACE_WAR].GetString(),
							(*g_pGameStringTable)[UI_STRING_MESSAGE_STATUS_LEFT_TIME].GetString(),
							temp2);
					
						static const char *change_vampire_string[3] ;
						
						
						long color = RGB_YELLOW;
						str[1] = temp_string;
						change_vampire_string[0]=str[0].c_str();					
						change_vampire_string[1]=NULL;
						change_vampire_string[2]=str[1].c_str();
						g_descriptor_manager.Set(DID_HELP, x+_x, y+_y, (void *)change_vampire_string, 0, color);
					}
				}
			}
		}
		if(!g_char_slot_ingame.STATUS.empty())
		{
			int select = -1;
			if(m_width_mode)
			{
				if(_x >= 5 && _x < 5+10*20 && _y >= 5 && _y < 5+20)
				{
					select = (_x-5)/20+m_scroll;
				}
			}
			else
			{
				if(_y >= 5 && _y < 5+10*20 && _x >= 5 && _x < 5+20)
				{
					select = (_y-5)/20+m_scroll;
				}
			}
			if(select != -1 && select < g_char_slot_ingame.STATUS.size())
				skill_id = g_char_slot_ingame.STATUS[select];
			if(skill_id != -1 && skill_id != ACTIONINFO_NULL && skill_id >= 0 && skill_id < g_pSkillInfoTable->GetSize())
			{
				if(skill_id != SKILL_BLOOD_DRAIN)
					g_descriptor_manager.Set(DID_INFO, x+_x, y+_y, (void *)((*g_pSkillInfoTable)[skill_id].GetName()), 0, 0);
				else
				{
					char temp_string[512]="";
					static std::string vampire_string[2]={
						(*g_pSkillInfoTable)[skill_id].GetName(),
							""
					};

					static const char *change_vampire_string[3] ;//{vampire_string[0], NULL, vampire_string[1]};					
					//strcpy(vampire_string[0], (*g_pSkillInfoTable)[skill_id].GetName());
					
#ifndef _LIB
					g_char_slot_ingame.CHANGE_VAMPIRE = 17290;
#endif
					int color = 0;
					
					if(g_char_slot_ingame.CHANGE_VAMPIRE > 60)
					{
						if(g_char_slot_ingame.CHANGE_VAMPIRE > 60*24)
							wsprintf(temp_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_CHANGE_VAMPIRE_DAY].GetString(),g_char_slot_ingame.CHANGE_VAMPIRE/60/24, (g_char_slot_ingame.CHANGE_VAMPIRE/60)%24, g_char_slot_ingame.CHANGE_VAMPIRE%60);
						else
						{
							wsprintf(temp_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_CHANGE_VAMPIRE_HOUR].GetString(), g_char_slot_ingame.CHANGE_VAMPIRE/60, g_char_slot_ingame.CHANGE_VAMPIRE%60);
							color = RGB_YELLOW;
						}
					}
					else
					{
						if(g_char_slot_ingame.CHANGE_VAMPIRE == 0)
							wsprintf(temp_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_CHANGE_VAMPIRE_SOON].GetString(),g_char_slot_ingame.CHANGE_VAMPIRE, 0, RGB_RED);
						else
							wsprintf(temp_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_CHANGE_VAMPIRE_MINUTE].GetString(),g_char_slot_ingame.CHANGE_VAMPIRE, 0, RGB_RED);
						
						color = RGB_RED;
					}
					vampire_string[1]=temp_string;
					change_vampire_string[0]=vampire_string[0].c_str();					
					change_vampire_string[1]=NULL;
					change_vampire_string[2]=vampire_string[1].c_str();
					g_descriptor_manager.Set(DID_HELP, x+_x, y+_y, (void *)change_vampire_string, 0, color);
				}
			}
		}
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if(skill_id != -1)
			gC_vs_ui.RunDescDialog(DID_SKILL, (void *)skill_id);
		if (gpC_mouse_pointer->GetPickUpItem() == false && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
			break;
		}
		break;
		
	case M_WHEEL_UP:
		Run(UP_ID);
		break;
		
	case M_WHEEL_DOWN:
		Run(DOWN_ID);
		break;
	}
	
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_EFFECT_STATUS
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_EFFECT_STATUS::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN && key == VK_ESCAPE)
	{
		//		Run(CLOSE);
	}
}

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_EFFECT_STATUS::Show()
{
	int size = g_char_slot_ingame.STATUS.size() + g_pUserInformation->WarInfo.size();
	if(size < m_scroll+10)
		m_scroll = max(0, size-10);
	bool bDrained = false;
	if(m_width_mode)
		// 가로 모드
	{
		/*
		상징물 석상: 성 문 앞에서 출입을 통제하는 역할을 합니다. 각 성의 상징물에 맞게 4종류의 석상을 각각 성 앞에 4개씩 배치하면 됩니다.
옥타부스(nw): 박쥐날개를 달고 있는 흑표범.
셉티무스(sw): 직립보행의 긴 뿔의 붉은 소
테르티우스(ne): 용
쿠아르투스(se): 불사조(Phoenix)
*/
		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			Rect rect(0, 0, w-4, h);
			int i=0;
			m_pC_effect_status_spk->BltLockedClip(x, y, rect, MAIN_WIDTH);
			m_pC_effect_status_spk->BltLocked(x+w-m_pC_effect_status_spk->GetWidth(MAIN_WIDTH_RIGHT), y, MAIN_WIDTH_RIGHT);
			
			if(!g_char_slot_ingame.STATUS.empty())
			{
				for(i = 0; i < min(10, g_char_slot_ingame.STATUS.size()); i++)
				{
					const int skill_id = g_char_slot_ingame.STATUS[i+m_scroll];
					if(skill_id == ACTIONINFO_NULL || skill_id < 0 || skill_id >= g_pSkillInfoTable->GetSize())
						break;
					
					if(skill_id == SKILL_BLOOD_DRAIN)bDrained = true;
					
					POINT point = {x+5+i*20, y+5};
					const int sprite_id = (*g_pSkillInfoTable)[skill_id].GetSpriteID();
					
					if(C_VS_UI_SKILL::m_C_spk_mini.GetSize() > sprite_id)
					{
						gpC_base->m_p_DDSurface_back->BltSprite(&point, &C_VS_UI_SKILL::m_C_spk_mini[sprite_id]);
					}
				}				
			}
			if(!g_pUserInformation->WarInfo.empty())
			{
				for(int a=0;a<g_pUserInformation->WarInfo.size();i++,a++)
				{					
					if(i>=10)
						break;
					POINT point = {x+5+i*20, y+5};
					int sprite_id = 12;

					int color_set = 315;

					switch(g_pUserInformation->WarInfo[a].zone_id)
					{
					case 1201 :
						sprite_id = 226;
						if(g_pUserInformation->WarInfo[a].war_type == 0)	// 길드전인경우
							color_set = 155;
						break;

					case 1202 :
						sprite_id = 227;
						if(g_pUserInformation->WarInfo[a].war_type == 0)	// 길드전인경우
							color_set = 25;
						break;
					
					case 1203 :
						sprite_id = 228;
						if(g_pUserInformation->WarInfo[a].war_type == 0)	// 길드전인경우
							color_set = 85;
						break;
					
					case 1204 :
						sprite_id = 229;
						if(g_pUserInformation->WarInfo[a].war_type == 0)	// 길드전인경우
							color_set = 40;
						break;
					}					

					if(C_VS_UI_SKILL::m_C_spk_mini.GetSize() > sprite_id)
					{
						//gpC_base->m_p_DDSurface_back->BltSprite(&point, &C_VS_UI_SKILL::m_C_spk_mini[sprite_id]);

						gpC_base->m_p_DDSurface_back->BltSpriteColorSet(&point,&C_VS_UI_SKILL::m_C_spk_mini[sprite_id], color_set);
					}
				}
			}
			
			m_pC_width_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			m_pC_width_button_group->ShowDescription();	
		}		
	}
	else
		// 세로 모드
	{
		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			Rect rect(0, 0, w, h-4);
			int i=0;
			m_pC_effect_status_spk->BltLockedClip(x, y, rect, MAIN_HEIGHT);
			m_pC_effect_status_spk->BltLocked(x, y+h-m_pC_effect_status_spk->GetHeight(MAIN_HEIGHT_BOTTOM), MAIN_HEIGHT_BOTTOM);
			
			if(!g_char_slot_ingame.STATUS.empty())
			{
				for(int i = 0; i < min(10, g_char_slot_ingame.STATUS.size()); i++)
				{
					const int skill_id = g_char_slot_ingame.STATUS[i+m_scroll];
					if(skill_id == ACTIONINFO_NULL || skill_id < 0 || skill_id >= g_pSkillInfoTable->GetSize())
						break;
					
					if(skill_id == SKILL_BLOOD_DRAIN)bDrained = true;
					
					POINT point = {x+5, y+5+i*20};
					const int sprite_id = (*g_pSkillInfoTable)[skill_id].GetSpriteID();
					
					if(C_VS_UI_SKILL::m_C_spk_mini.GetSize() > sprite_id)
					{
						gpC_base->m_p_DDSurface_back->BltSprite(&point, &C_VS_UI_SKILL::m_C_spk_mini[sprite_id]);
					}
				}
			}if(!g_pUserInformation->WarInfo.empty())
			{
				for(int a=0;a<g_pUserInformation->WarInfo.size();i++,a++)
				{					
					if(i>=10)
						break;
					POINT point = {x+5, y+5+i*20};
					int sprite_id = 12;

					int color_set = 315;

					switch(g_pUserInformation->WarInfo[a].zone_id)
					{
					case 1201 :
						sprite_id = 226;
						if(g_pUserInformation->WarInfo[a].war_type == 0)	// 길드전인경우
							color_set = 155;
						break;

					case 1202 :
						sprite_id = 227;
						if(g_pUserInformation->WarInfo[a].war_type == 0)	// 길드전인경우
							color_set = 25;
						break;
					
					case 1203 :
						sprite_id = 228;
						if(g_pUserInformation->WarInfo[a].war_type == 0)	// 길드전인경우
							color_set = 85;
						break;
					
					case 1204 :
						sprite_id = 229;
						if(g_pUserInformation->WarInfo[a].war_type == 0)	// 길드전인경우
							color_set = 40;
						break;
					}					

					if(C_VS_UI_SKILL::m_C_spk_mini.GetSize() > sprite_id)
					{
						//gpC_base->m_p_DDSurface_back->BltSprite(&point, &C_VS_UI_SKILL::m_C_spk_mini[sprite_id]);

						gpC_base->m_p_DDSurface_back->BltSpriteColorSet(&point,&C_VS_UI_SKILL::m_C_spk_mini[sprite_id], color_set);
					}
				}
//				for(int a=0;a<g_pUserInformation->WarInfo.size();i++,a++)
//				{					
//					if(i>=10)
//						break;
//					POINT point = {x+5, y+5+i*20};
//					
//					int sprite_id = 12;
//
//					switch(g_pUserInformation->WarInfo[a].zone_id)
//					{
//					case 1201 :
//						sprite_id = 226;
//						break;
//					case 1202 :
//						sprite_id = 227;
//						break;
//					case 1203 :
//						sprite_id = 228;
//						break;
//					case 1204 :
//						sprite_id = 229;
//						break;
//					}					
//
//					if(C_VS_UI_SKILL::m_C_spk_mini.GetSize() > sprite_id)
//					{
//						gpC_base->m_p_DDSurface_back->BltSprite(&point, &C_VS_UI_SKILL::m_C_spk_mini[sprite_id]);
//					}
//				}
			}
			m_pC_height_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
			m_pC_height_button_group->ShowDescription();	
		}		
	}	
	g_char_slot_ingame.bl_drained = bDrained;
}

//-----------------------------------------------------------------------------
// ResetSize
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_EFFECT_STATUS::ResetSize()
{
	//13개가 넘으면 스크롤 버튼 나온다하!
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_EFFECT_STATUS::Start()
{
	PI_Processor::Start();
	
	m_pC_width_button_group->Init();
	m_pC_height_button_group->Init();
	
	gpC_window_manager->AppearWindow(this);
	
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::EFFECT_STATUS));
	if(m_width_mode == gpC_vs_ui_window_manager->IsEffectStatusHeight())
		Run(CHANGE_ID);
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::EFFECT_STATUS);
	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}	
}

void C_VS_UI_EFFECT_STATUS::Finish()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::EFFECT_STATUS, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::EFFECT_STATUS, Rect(x, y, w, h));
	gpC_vs_ui_window_manager->SetEffectStatusHeight(!m_width_mode);
	
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_EFFECT_STATUS::Process()
{
	if((g_pUserInformation->WarInfo.empty() && g_char_slot_ingame.STATUS.empty()) && m_bl_effect_size_null == false )
	{
		m_bl_effect_size_null = true;
		if(m_width_mode)
			AttrAutoHide(ATTRIBUTES_HIDE_HEIGHT);
		else
			AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
	}
	else if(!(g_pUserInformation->WarInfo.empty() && g_char_slot_ingame.STATUS.empty()) && m_bl_effect_size_null == true)
	{
		m_bl_effect_size_null = false;
		AttrAutoHide(ATTRIBUTES_HIDE_NOT);
	}
	
	if(m_width_mode)
		m_pC_width_button_group->Process();
	else
		m_pC_height_button_group->Process();
	ProcessHide();
	
	/*std::vector<WAR_INFO>::iterator itr = g_pUserInformation->WarInfo.begin();
	
	while(itr != g_pUserInformation->WarInfo.end() )
	{
		//WAR_INFO info =(*itr);
		//if(info.left_time <= timeGetTime()/1000)

		if ( itr->left_time <= timeGetTime() / 1000 )

			
		itr++;
	}*/
}

const int zone_id_size = 73;

//-----------------------------------------------------------------------------
// GetZoneNumber
//
// ZoneID를 받아서 컨버팅
//-----------------------------------------------------------------------------
//int C_VS_UI_MINIMAP::GetZoneNumber(int zone_id)
//{
//	int id[zone_id_size] = {
//		11, 12, 13, 14, 21, 22, 23 ,24, 31, 32, 33, 34, 41, 42, 43, 44, 51, 52, 53, 54, 61, 62, 63, 64, // 필드
//		1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010, // 던젼&마을
//		1100, 1101, 1102, 1103, 1104, 1105, // 바토리 던전
//		1111, 1112, 1113, 1114, // 라센성
//		1121, 1122, 1123, 1124, // 성지
//		2000, 2001, 2002, 2003, 2004, 2010, 2011, 2012, 2013, 2014, 2020, 2021, 2022, 2023, 2024,//건물
//		//			2101, 2102, 2103, 2104, 2105, 2106,//튜토리얼
//		2101, 2102,
//		10001, 20001, 
//		1011, 1012,	// 로스트 타이얀
//		1013, // 아실리온
//		
//		60000, 60001, 60002};//클라이언트용 가짜 아이디
//		
//	for(int i = 0; i < zone_id_size; i++)
//		if(zone_id == id[i])break;
//		
//	if(i == zone_id_size)return -1;
//		return i;
//}

//-----------------------------------------------------------------------------
// GetZoneName
//
// ZoneID를 받아서 존이름을 반환한다
//-----------------------------------------------------------------------------
//char * C_VS_UI_MINIMAP::GetZoneName(int zone_id)
//{
//	char *map_name[zone_id_size] =
//	{
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_ESLANIA_NE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_ESLANIA_NW].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_ESLANIA_SE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_ESLANIA_SW].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_LIMBOLAIR_NE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_LIMBOLAIR_NW].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_LIMBOLAIR_SE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_LIMBOLAIR_SW].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_DROBETA_NE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_DROBETA_NW].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_DROBETA_SE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_DROBETA_SW].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_RODIN_NE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_RODIN_NW].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_RODIN_SE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_RODIN_SW].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_TIMORE_NE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_TIMORE_NW].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_TIMORE_SE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_TIMORE_SW].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_PERONA_NE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_PERONA_NW].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_PERONA_SE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_PERONA_SW].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_UNDERGROUND_WATERWAY].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_ESLANIA_DUNGEON].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_IN_LIMBOLAIR].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_LIMBOLAIR_DEONGEON].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_EVENTOX].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_EVENT_STADIUM].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_IN_RASENCASTLE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_KALI_CAVE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_RASEN_TRAINING].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_PEIAC_TUNNEL].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_BATHORY_B1].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_BATHORY_B2].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_BATHORY_B3].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_BATHORY_B4].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_BATHORY_LAIR].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GHORGOBA_TUNNEL].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_RASEN_YARD].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_CASTLE_RASEN_1F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_CASTLE_RASEN_2F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_TEPES_LAIR].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_SLAYER_BATTLEZONE1].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_SLAYER_BATTLEZONE2].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_BATHORY_BATTLEZONE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_RASEN_BATTLEZONE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_ARMY_B1].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_ARMY_1F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_ARMY_2F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_ARMY_3F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_ARMY_4F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_CELRIC_B1].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_CELRIC_1F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_CELRIC_2F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_CELRIC_3F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_CELRIC_4F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_KNIGHT_B1].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_KNIGHT_1F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_KNIGHT_2F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_KNIGHT_3F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_KNIGHT_4F].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_TUTORIAL_N].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_TUTORIAL_S].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_GUILD_HOUSE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLAN_HOUSE].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_LOST_TAIYAN_B1].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_LOST_TAIYAN_B2].GetString(),			
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_ASYLION_DUNGEON].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_TAKE_HELICOPTER].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_TAKE_ELIVATOR].GetString(),
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_UNKNOW].GetString(),
//	};
//	
//	int id = GetZoneNumber(zone_id);
//	if(id == -1)return NULL;
//	else return map_name[id];
//}
//
//-----------------------------------------------------------------------------
// C_VS_UI_MINIMAP
//
//-----------------------------------------------------------------------------
C_VS_UI_MINIMAP::C_VS_UI_MINIMAP()
{
	m_pC_minimap_spk = NULL;
	
	AttrTopmost(false);
	AttrPin(true);
	
	g_RegisterWindow(this);
	
	if(gbl_vampire_interface)
	{
		m_pC_minimap_spk = new C_SPRITE_PACK(SPK_MINIMAP_VAMPIRE);
	}
	else
	{
		m_pC_minimap_spk = new C_SPRITE_PACK(SPK_MINIMAP_SLAYER);
	}
	Set(RESOLUTION_X-m_pC_minimap_spk->GetWidth(MINIMAP_MAIN)-m_pC_minimap_spk->GetWidth(MINIMAP_RIGHT), 0, m_pC_minimap_spk->GetWidth(MINIMAP_MAIN)+m_pC_minimap_spk->GetWidth(MINIMAP_RIGHT), m_pC_minimap_spk->GetHeight(MINIMAP_MAIN));
	
	//skillinfo 버튼
	m_pC_button_group = new ButtonGroup(this);
	
	int alpha_button_offset_x, alpha_button_offset_y;
	int pushpin_button_offset_x, pushpin_button_offset_y;
	alpha_button_offset_x = 7; alpha_button_offset_y = 36;
	pushpin_button_offset_x = 23; pushpin_button_offset_y = 36;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_button_offset_x, alpha_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pushpin_button_offset_x, pushpin_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), PUSHPIN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN));
	
	
	m_zone_id = -1;
	m_map_x = 0; m_map_y = 0;
	m_map_w = 0; m_map_h = 0;
	m_map_start_point.x = 4;
	m_map_start_point.y = 29;
	m_bl_refresh = false;
	
	// TIMER
	m_dw_minimap_prev_tickcount = GetTickCount();
	m_dw_minimap_millisec = 100;
	
	m_p_minimap_surface = new CSpriteSurface;
	
	m_p_minimap_surface->InitOffsurface(200, 100, DDSCAPS_SYSTEMMEMORY);
	m_p_minimap_surface->SetTransparency(0xffff);
	
	m_surface_w = 200;
	m_surface_w = 100;
	
#ifndef _LIB
	m_map_w = 256;
	m_map_h = 256;
	SetZone(11);
	RECT rect = {110,110,140,150};
	SetSafetyZone(rect, true);
	SetRect(&rect, 10, 10, 30, 30);
	SetPortal(rect, 1001);
	MINIMAP_NPC npc;//{20, 50, 157};
	npc.x=20;
	npc.y=50;
	npc.id=157;
	npc.name = "sonee";
	SetNPC(npc);
#endif
	
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_MINIMAP
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_MINIMAP::~C_VS_UI_MINIMAP()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::MINIMAP, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::MINIMAP, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::MINIMAP, Rect(x, y, w, h));
	
	g_UnregisterWindow(this);
	
	if(m_p_minimap_surface != NULL)
	{
		DeleteNew(m_p_minimap_surface);
		m_p_minimap_surface = NULL;
	}
	DeleteNew(m_pC_minimap_spk);
	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_MINIMAP::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_MINIMAP::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char* m_minimap_button_string[4] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
	};
	
	if(GetAttributes()->alpha && p_button->GetID() == ALPHA_ID)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_minimap_button_string[2],0,0);
	if(GetAttributes()->autohide && p_button->GetID() == PUSHPIN_ID)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_minimap_button_string[3],0,0);
	else
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_minimap_button_string[p_button->GetID()],0,0);
}

//-----------------------------------------------------------------------------
// C_VS_UI_MINIMAP::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_MINIMAP::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{	
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x+m_board_x, p_button->y+y+m_board_y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x+m_board_x, p_button->y+y+m_board_y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	}
	else if(p_button->GetID() == PUSHPIN_ID)
	{
		if(GetAttributes()->autohide)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x+m_board_x, p_button->y+y+m_board_y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x+m_board_x, p_button->y+y+m_board_y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
	}
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_MINIMAP::WindowEventReceiver(id_t event)
{
	switch(event)
	{
	case EVENT_WINDOW_MOVE: 
		{
			if(x+w/2 < RESOLUTION_X/2)
				m_board_x = w-4;
			else
				m_board_x = -m_pC_minimap_spk->GetWidth(MINIMAP_BOARD)+4;
			if(y+h/2 < RESOLUTION_Y/2)
				m_board_y = 0;
			else
				m_board_y = h - m_pC_minimap_spk->GetHeight(MINIMAP_BOARD);
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_MINIMAP::IsPixel(int _x, int _y)
{
	if(Moving()) return true;
	if(GetAttributes()->alpha &&
		_x >= x+m_map_start_point.x && _x <= x+m_map_start_point.x+m_surface_w &&
		_y >= y+m_map_start_point.y && _y <= y+m_map_start_point.y+m_surface_h)
	{
		return false;
	}
	
	
	bool re = IsInRect(_x, _y);
	if(re == false)
	{
		re = m_pC_minimap_spk->IsPixel(_x-x-m_board_x, _y-y-m_board_y, MINIMAP_BOARD);
	}
	return re;
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_MINIMAP::Run(id_t id)
{
	switch (id)
	{
		//	case CHANGE_ID:
		//		m_width_mode = !m_width_mode;
		//		w^=h; h^=w; w^=h; //swap
		//		x -= w-h;
		//		EMPTY_MOVE;
		//		break;
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;
		
	case PUSHPIN_ID:
		if(GetAttributes()->autohide)
			AttrAutoHide(ATTRIBUTES_HIDE_NOT);
		else
			AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
		EMPTY_MOVE;
		break;
		
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_MINIMAP::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	
	bool re;
	re = m_pC_button_group->MouseControl(message, _x-m_board_x, _y-m_board_y);
	
	switch(message)
	{
	case M_MOVING:
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		// 반투명 상태에서 맵 클릭하면 이동되게
		if (gpC_mouse_pointer->GetPickUpItem() == false && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
			break;
		}
		break;
		
	}
	return true;
}

//-----------------------------------------------------------------------------
// MouseControlExtra
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_MINIMAP::MouseControlExtra(UINT message, int _x, int _y)
{
	_x-=x; _y-=y;
	
	switch(message)
	{
	case M_MOVING:
		// 미니맵 위에 마우스가 있다.
		{
			int map_w = m_surface_w, map_h = m_surface_h;
			//			if(m_map_w != m_map_h)
			//			{
			//				if(m_map_w > m_map_h)map_h = map_h * m_map_h / m_map_w;
			//				if(m_map_h > m_map_w)map_w = map_w * m_map_w / m_map_h;
			//			}
			
			for(int i = 0; i < m_portal.size(); i++)
			{
				int x = m_map_start_point.x + (m_portal[i].left+m_portal[i].right)/2*map_w/m_map_w;
				int y = m_map_start_point.y + (m_portal[i].top+m_portal[i].bottom)/2*map_h/m_map_h;
				if(_x > x-5 && _x < x+5 && _y > y-5 && _y < y+5)
				{
					g_descriptor_manager.Set(DID_INFO, gpC_mouse_pointer->GetPointerX(), gpC_mouse_pointer->GetPointerY(), g_pZoneTable->Get(m_portal_zone_id[i])->Name.GetString());
					break;
				}
			}
			
			for(i = 0; i < m_npc.size(); i++)
			{
				int x = m_map_start_point.x + m_npc[i].x*map_w/m_map_w;
				int y = m_map_start_point.y + m_npc[i].y*map_h/m_map_h;
				
				if(_x > x-5 && _x < x+5 && _y > y-5 && _y < y+5)
				{
			//		static char npc_name[50];
			//		if((*g_pCreatureTable)[m_npc[i].id].Name.GetString())
			//			strcpy(npc_name, (*g_pCreatureTable)[m_npc[i].id].Name.GetString());
			//		else strcpy(npc_name, "");
					
					static S_DEFAULT_HELP_STRING npc_string;
			//		npc_string.sz_main_str = npc_name;
					if(m_npc[i].name.GetLength() >= 1)
						npc_string.sz_main_str = m_npc[i].name.GetString();
					else
						npc_string.sz_main_str = NULL;
					
					//					static char npc_description[50];
					NPC_INFO *npc_info = g_pNPCTable->GetData(m_npc[i].id);
					if(npc_info)
						npc_string.sz_sub_str = npc_info->Description;
					
					g_descriptor_manager.Set(DID_HELP, gpC_mouse_pointer->GetPointerX(), gpC_mouse_pointer->GetPointerY(), (void *)&npc_string);
					break;
				}
			}
			
			for(i = 0; i < m_shrine.size(); i++)
			{
				int x = m_map_start_point.x + m_shrine[i].x*map_w/m_map_w;
				int y = m_map_start_point.y + m_shrine[i].y*map_h/m_map_h;
				
				if(_x > x-5 && _x < x+5 && _y > y-5 && _y < y+5)
				{
					g_descriptor_manager.Set(DID_INFO, gpC_mouse_pointer->GetPointerX(), gpC_mouse_pointer->GetPointerY(), m_shrine[i].name.GetString());
					break;
				}
			}

			for(i = 0; i < g_pParty->GetSize(); i++)
			{
				PARTY_INFO *info = g_pParty->GetMemberInfo(i);
				if(info != NULL && info->zoneID == GetZoneID())
				{
					int x = m_map_start_point.x + info->zoneX*map_w/m_map_w;
					int y = m_map_start_point.y + info->zoneY*map_h/m_map_h;
					
					if(_x > x-5 && _x < x+5 && _y > y-5 && _y < y+5)
					{
						static char party_name[20];
						static char party_desc[50];
						if(info->Name.GetString())
							strcpy(party_name, info->Name.GetString());
						else strcpy(party_name, "");
						
						static S_DEFAULT_HELP_STRING party_string;
						party_string.sz_main_str = party_name;
						
						wsprintf(party_desc, "(%d,%d)", info->zoneX, info->zoneY);
						party_string.sz_sub_str = party_desc;
						
						g_descriptor_manager.Set(DID_HELP, gpC_mouse_pointer->GetPointerX(), gpC_mouse_pointer->GetPointerY(), (void *)&party_string);
						break;
					}
				}
			}
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_MINIMAP
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_MINIMAP::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN && key == VK_ESCAPE)
	{
		//		Run(CLOSE);
	}
}

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_MINIMAP::Show()
{
	// 맵이 바뀌었으면 텍스쳐 세팅을 다시 해줘야 한다.
	if(m_bl_refresh == true)
	{
		InitMinimap(m_p_minimap_surface);
		m_bl_refresh = false;
	}
	
	// 맵출력
	if(GetAttributes()->alpha)
	{
		DrawMinimapAlpha(x+m_map_start_point.x, y+m_map_start_point.y, m_p_minimap_surface);
	}
	else
	{
		RECT rt = {0, 0, m_surface_w, m_surface_h};
		POINT map_point = m_map_start_point;
		map_point.x+=x;
		map_point.y+=y;
		
		gpC_base->m_p_DDSurface_back->Blt(&map_point, m_p_minimap_surface, &rt);
	}
	
	// 미니맵위에 기타등등 출력
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_minimap_spk->BltLocked(x+m_board_x, y+m_board_y, MINIMAP_BOARD);
		
		// 맵테두리
		Rect rect(0, 0, w-m_pC_minimap_spk->GetWidth(MINIMAP_RIGHT), h);
		if(GetAttributes()->alpha)
		{
			RECT alpha_rect = {x+4, y+4, x+w-4, y+25};
			if(gbl_vampire_interface)
				DrawAlphaBox(&alpha_rect, 2, 0, 0, g_pUserOption->ALPHA_DEPTH);
			else
				DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
			m_pC_minimap_spk->BltLockedClip(x, y, rect, MINIMAP_MAIN_ALPHA);
		}
		else
			m_pC_minimap_spk->BltLockedClip(x, y, rect, MINIMAP_MAIN);
		
		rect.Set(0, 0, m_pC_minimap_spk->GetWidth(MINIMAP_RIGHT), h);
		m_pC_minimap_spk->BltLockedClip(x+w-m_pC_minimap_spk->GetWidth(MINIMAP_RIGHT), y, rect, MINIMAP_RIGHT);
		
		// use only peiac
		if(m_surface_h != 100)
		{
			rect.Set(0, m_pC_minimap_spk->GetHeight(MINIMAP_MAIN)-4, w-m_pC_minimap_spk->GetWidth(MINIMAP_RIGHT), 4);
			m_pC_minimap_spk->BltLockedClip(x, y-(100-m_surface_h), rect, MINIMAP_MAIN);
			rect.Set(0, m_pC_minimap_spk->GetHeight(MINIMAP_MAIN)-4, m_pC_minimap_spk->GetWidth(MINIMAP_RIGHT), 4);
			m_pC_minimap_spk->BltLockedClip(x+w-m_pC_minimap_spk->GetWidth(MINIMAP_RIGHT), y-(100-m_surface_h), rect, MINIMAP_RIGHT);
		}
		
		// hiding중에는 기타등등 생략
		//		if(GetAttributes()->autohide && x < 0 || x+w > RESOLUTION_X || y < 0 || y+h > RESOLUTION_Y)
		//		{
		//			gpC_base->m_p_DDSurface_back->Unlock();
		//			return;
		//		}
		
		int map_w = m_surface_w, map_h = m_surface_h;
		//		if(m_map_w != m_map_h)
		//		{
		//			if(m_map_w > m_map_h)map_h = map_h * m_map_h / m_map_w;
		//			if(m_map_h > m_map_w)map_w = map_w * m_map_w / m_map_h;
		//		}
		
		
		
		WORD *mem = (WORD *)gpC_base->m_p_DDSurface_back->GetSurfacePointer();
		long pitch = gpC_base->m_p_DDSurface_back->GetSurfacePitch();
		long pitch_div_2 = pitch>>1;	// by sigi
		
		//		RECT rect = {m_map_start_point.x, m_map_start_point.y, m_map_start_point.x+m_surface_w, m_map_start_point.y+m_surface_h};
		
		static int color=21;
		static int color_direct = 1;
		if(TimerMinimap())
		{
			color += color_direct;
			if(color > 30 || color < 21)color_direct *= -1;
		}
		
		BYTE r = 31, g = 63, b = 31;
		WORD portal_color = CDirectDraw::Color(r, g, b);
		
		int i, _x, _y;
		WORD _color, _color2;
		
		// 포탈 위치 표시 ▽
		r = color, g = color-10, b = color;
		_color = CDirectDraw::Color(r, g, b);
		r = color*2/3, g = (color-10)*2/3, b = color*2/3;
		_color2 = CDirectDraw::Color(r, g, b);
		for(i = 0; i < m_portal.size(); i++)
		{
			_x = x+m_map_start_point.x + (m_portal[i].left+m_portal[i].right)/2*map_w/m_map_w;
			_y = y+m_map_start_point.y + (m_portal[i].top+m_portal[i].bottom)/2*map_h/m_map_h;
			
			if(_x > 0 && _x < RESOLUTION_X-1 && _y > 1 && _y < RESOLUTION_Y)
			{
				mem[(_y)*pitch_div_2 + _x] = _color;
				mem[(_y-1)*pitch_div_2 + _x-1] = _color2;
				mem[(_y-1)*pitch_div_2 + _x] = _color;
				mem[(_y-1)*pitch_div_2 + _x+1] = _color2;
				mem[(_y-2)*pitch_div_2 + _x-1] = _color;
				mem[(_y-2)*pitch_div_2 + _x] = _color;
				mem[(_y-2)*pitch_div_2 + _x+1] = _color;
				
				mem[(_y-3)*pitch_div_2 + _x-1] = 0;
				mem[(_y-3)*pitch_div_2 + _x] = 0;
				mem[(_y-3)*pitch_div_2 + _x+1] = 0;
				mem[(_y-2)*pitch_div_2 + _x-2] = 0;
				mem[(_y-2)*pitch_div_2 + _x+2] = 0;
				mem[(_y-1)*pitch_div_2 + _x-2] = 0;
				mem[(_y-1)*pitch_div_2 + _x+2] = 0;
				mem[(_y)*pitch_div_2 + _x-1] = 0;
				mem[(_y)*pitch_div_2 + _x+1] = 0;
				mem[(_y+1)*pitch_div_2 + _x] = 0;
			}
			
		}
		
		// npc위치 표시 †
		r = color-10, g = color, b = color-10;
		_color = CDirectDraw::Color(r, g, b);
		for(i = 0; i < m_npc.size(); i++)
		{
			_x = x+m_map_start_point.x + m_npc[i].x*map_w/m_map_w;
			_y = y+m_map_start_point.y + m_npc[i].y*map_h/m_map_h;
			
			if(_x > 0 && _x < RESOLUTION_X-1 && _y > 2 && _y < RESOLUTION_Y)
			{
				mem[(_y)*pitch_div_2 + _x] = _color;
				mem[(_y-1)*pitch_div_2 + _x] = _color;
				mem[(_y-2)*pitch_div_2 + _x-1] = _color;
				mem[(_y-2)*pitch_div_2 + _x] = _color;
				mem[(_y-2)*pitch_div_2 + _x+1] = _color;
				mem[(_y-3)*pitch_div_2 + _x] = _color;
				
				mem[(_y-4)*pitch_div_2 + _x] = 0;
				mem[(_y-3)*pitch_div_2 + _x-1] = 0;
				mem[(_y-3)*pitch_div_2 + _x+1] = 0;
				mem[(_y-2)*pitch_div_2 + _x-2] = 0;
				mem[(_y-2)*pitch_div_2 + _x+2] = 0;
				mem[(_y-1)*pitch_div_2 + _x-1] = 0;
				mem[(_y-1)*pitch_div_2 + _x+1] = 0;
				mem[(_y)*pitch_div_2 + _x-1] = 0;
				mem[(_y)*pitch_div_2 + _x+1] = 0;
				mem[(_y+1)*pitch_div_2 + _x] = 0;
			}
		}

		// shrine위치 표시 †
		r = color-10, g = color-10, b = color;
		_color = CDirectDraw::Color(r, g, b);
		r = (color-10)*2/3, g = (color-10)*2/3, b = color*2/3;
		_color2 = CDirectDraw::Color(r, g, b);
		for(i = 0; i < m_shrine.size(); i++)
		{
			_x = x+m_map_start_point.x + m_shrine[i].x*map_w/m_map_w;
			_y = y+m_map_start_point.y + m_shrine[i].y*map_h/m_map_h;
			
			if(_x > 0 && _x < RESOLUTION_X-1 && _y > 2 && _y < RESOLUTION_Y)
			{
				mem[(_y)*pitch_div_2 + _x-1] = _color;
				mem[(_y)*pitch_div_2 + _x+1] = _color;
				mem[(_y-1)*pitch_div_2 + _x] = _color;
				mem[(_y-1)*pitch_div_2 + _x-1] = _color2;
				mem[(_y-1)*pitch_div_2 + _x+1] = _color2;
				mem[(_y-2)*pitch_div_2 + _x] = _color;
				mem[(_y+1)*pitch_div_2 + _x] = _color;
				mem[(_y+1)*pitch_div_2 + _x-1] = _color2;
				mem[(_y+1)*pitch_div_2 + _x+1] = _color2;
				mem[(_y+2)*pitch_div_2 + _x] = _color;
				
				mem[(_y)*pitch_div_2 + _x] = 0;
				mem[(_y)*pitch_div_2 + _x-2] = 0;
				mem[(_y)*pitch_div_2 + _x+2] = 0;
				mem[(_y-1)*pitch_div_2 + _x-2] = 0;
				mem[(_y-1)*pitch_div_2 + _x+2] = 0;
				mem[(_y-2)*pitch_div_2 + _x-1] = 0;
				mem[(_y-2)*pitch_div_2 + _x+1] = 0;
				mem[(_y-3)*pitch_div_2 + _x] = 0;
				mem[(_y+1)*pitch_div_2 + _x-2] = 0;
				mem[(_y+1)*pitch_div_2 + _x+2] = 0;
				mem[(_y+2)*pitch_div_2 + _x-1] = 0;
				mem[(_y+2)*pitch_div_2 + _x+1] = 0;
				mem[(_y+3)*pitch_div_2 + _x] = 0;
			}
		}

		/*		
		// 파티원위치 표시 x
		if(g_pParty->GetSize())
		{
		r = color, g = color-5, b = color-5;
		_color = CDirectDraw::Color(r, g, b);
		for(i = 0; i < g_pParty->GetSize(); i++)
		{
		if(g_pParty->GetMemberInfo(i) != NULL && g_pParty->GetMemberInfo(i)->zoneID == m_zone_id)
		{
		_x = x+m_map_start_point.x + g_pParty->GetMemberInfo(i)->zoneX*map_w/m_map_w;
		_y = y+m_map_start_point.y + g_pParty->GetMemberInfo(i)->zoneY*map_h/m_map_h;
		
		  if(_x > 0 && _x < RESOLUTION_X-1 && _y > 0 && _y < RESOLUTION_Y-1)
		  {
		  mem[(_y)*pitch_div_2 + _x] = _color;
		  mem[(_y-1)*pitch_div_2 + _x-1] = _color;
		  mem[(_y-1)*pitch_div_2 + _x+1] = _color;
		  mem[(_y+1)*pitch_div_2 + _x-1] = _color;
		  mem[(_y+1)*pitch_div_2 + _x+1] = _color;
		  
			mem[(_y-2)*pitch_div_2 + _x-1] = 0;
			mem[(_y-2)*pitch_div_2 + _x+1] = 0;
			mem[(_y-1)*pitch_div_2 + _x-2] = 0;
			mem[(_y-1)*pitch_div_2 + _x] = 0;
			mem[(_y-1)*pitch_div_2 + _x+2] = 0;
			mem[(_y)*pitch_div_2 + _x-1] = 0;
			mem[(_y)*pitch_div_2 + _x+1] = 0;
			mem[(_y+1)*pitch_div_2 + _x-2] = 0;
			mem[(_y+1)*pitch_div_2 + _x] = 0;
			mem[(_y+1)*pitch_div_2 + _x+2] = 0;
			mem[(_y+2)*pitch_div_2 + _x-1] = 0;
			mem[(_y+2)*pitch_div_2 + _x+1] = 0;
			}
			}
			}
			}
			
			  //자기 위치 표시 x
			  _x = x+m_map_start_point.x + m_map_x*map_w/m_map_w;
			  _y = y+m_map_start_point.y + m_map_y*map_h/m_map_h;
			  
				if(_x > 0 && _x < RESOLUTION_X-1 && _y > 0 && _y < RESOLUTION_Y-1)
				{
				mem[(_y)*pitch_div_2 + _x] = 0xffff;
				mem[(_y+1)*pitch_div_2 + _x+1] = 0xffff;
				mem[(_y-1)*pitch_div_2 + _x-1] = 0xffff;
				mem[(_y+1)*pitch_div_2 + _x-1] = 0xffff;
				mem[(_y-1)*pitch_div_2 + _x+1] = 0xffff;
				
				  mem[(_y-2)*pitch_div_2 + _x-1] = 0;
				  mem[(_y-2)*pitch_div_2 + _x+1] = 0;
				  mem[(_y-1)*pitch_div_2 + _x-2] = 0;
				  mem[(_y-1)*pitch_div_2 + _x] = 0;
				  mem[(_y-1)*pitch_div_2 + _x+2] = 0;
				  mem[(_y)*pitch_div_2 + _x-1] = 0;
				  mem[(_y)*pitch_div_2 + _x+1] = 0;
				  mem[(_y+1)*pitch_div_2 + _x-2] = 0;
				  mem[(_y+1)*pitch_div_2 + _x] = 0;
				  mem[(_y+1)*pitch_div_2 + _x+2] = 0;
				  mem[(_y+2)*pitch_div_2 + _x-1] = 0;
				  mem[(_y+2)*pitch_div_2 + _x+1] = 0;
				  }
		*/
		// 파티 위치 표시
		for(i = 0; i < g_pParty->GetSize(); i++)
		{
			if(g_pParty->GetMemberInfo(i) != NULL && g_pParty->GetMemberInfo(i)->zoneID == m_zone_id)
			{
				_x = x+m_map_start_point.x + g_pParty->GetMemberInfo(i)->zoneX*map_w/m_map_w;
				_y = y+m_map_start_point.y + g_pParty->GetMemberInfo(i)->zoneY*map_h/m_map_h;
				if(_x > 0 && _x < RESOLUTION_X-1 && _y > 0 && _y < RESOLUTION_Y-1)
					m_pC_minimap_spk->BltLocked(_x-m_pC_minimap_spk->GetWidth(MINIMAP_ICON_PARTY)/2, _y-m_pC_minimap_spk->GetHeight(MINIMAP_ICON_PARTY)/2, MINIMAP_ICON_PARTY);
			}
		}
		
		// 자기 위치 표시
		_x = x+m_map_start_point.x + m_map_x*map_w/m_map_w;
		_y = y+m_map_start_point.y + m_map_y*map_h/m_map_h;
		
		if(_x > 0 && _x < RESOLUTION_X-1 && _y > 0 && _y < RESOLUTION_Y-1)
			m_pC_minimap_spk->BltLocked(_x-m_pC_minimap_spk->GetWidth(MINIMAP_ICON_SELF)/2, _y-m_pC_minimap_spk->GetHeight(MINIMAP_ICON_SELF)/2, MINIMAP_ICON_SELF);
		
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	g_FL2_GetDC();
	g_PrintColorStr(x+w/2-g_GetStringWidth(m_zone_name.c_str(), gpC_base->m_info_pi.hfont)/2, y+8, m_zone_name.c_str(), gpC_base->m_info_pi, RGB_WHITE);
	char sz_temp[5];
	wsprintf(sz_temp, "%d", m_map_x);
	g_PrintColorStr(x+m_board_x+19, y+m_board_y+6, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
	wsprintf(sz_temp, "%d", m_map_y);
	g_PrintColorStr(x+m_board_x+19, y+m_board_y+21, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
	m_pC_button_group->ShowDescription();
	g_FL2_ReleaseDC();	
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_MINIMAP::Start()
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::MINIMAP));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::MINIMAP));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::MINIMAP);
	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}
	
	PI_Processor::Start();
	
	m_pC_button_group->Init();
	
	WindowEventReceiver(EVENT_WINDOW_MOVE);
	
	gpC_window_manager->AppearWindow(this);
}

void C_VS_UI_MINIMAP::Finish()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::MINIMAP, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::MINIMAP, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::MINIMAP, Rect(x, y, w, h));
	
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);	
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_MINIMAP::Process()
{
	m_pC_button_group->Process();
	
	bool bl_alpha = GetAttributes()->alpha;
	if(bl_alpha)
	{
		AttrAlpha(false);
	}
	if(x <= 0 || x+w >= RESOLUTION_X)
		ProcessHide(-4, bl_alpha);
	else
		ProcessHide(0, bl_alpha);
	if(bl_alpha)
	{
		AttrAlpha(true);
	}
	
}

void C_VS_UI_MINIMAP::SetZone(int zone_id)
{
	m_bl_refresh = true;
	
	//	int spk_id[zone_id_size] = 
	//	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,//필드
	//	12, 13, 14, 13, 15, 21, 40, 45, //던젼&마을
	//	39, 28, 29, 30, 31, 32,//바토리던젼
	//	41, 42, 43, 44, //라센성
	//	20, 16, 17, 18, 19,//군인길드
	//	20, 22, 23, 24, 19,//성직자길드
	//	20, 25, 26, 27, 19,//무사길드
	//	19, 20, 21, 22, 23, 25,//튜토리얼
	//	33, 34,					//튜토리얼
	//	-1, -1, -1};//클라이언트용
	
	//	bool bl_statch = (x+w >= RESOLUTION_X-1);
	//	int statch_w = w;
	
	m_zone_id = zone_id;
	m_zone_name = g_pZoneTable->Get(zone_id)->Name.GetString();
	
	//-------------------------------------------------------------------
	//
	// 필요한 minimap sprite를 하나 읽어오기 위해서.
	//
	//-------------------------------------------------------------------
	// by sigi 2001.9.5
	CSpritePack	minimapSPK;
	minimapSPK.Init( 1, CDirectDraw::Is565() );		// 임시로 1개만 loading.. - -;;
	
	bool re = minimapSPK.LoadFromFileSprite( 0, g_pZoneTable->Get(zone_id)->MinimapSpriteID, SPK_MINIMAP, SPKI_MINIMAP );
	assert(re && "Minimap Not Load");
	
	//-------------------------------------------------------------------
	// 화면에 출력
	//-------------------------------------------------------------------
	if(re)
	{
		m_surface_w = minimapSPK[0].GetWidth();
		m_surface_h = minimapSPK[0].GetHeight();
		
		POINT point = {0, 0};
		DeleteNew(m_p_minimap_surface);
		m_p_minimap_surface = new CSpriteSurface;
		m_p_minimap_surface->InitOffsurface(m_surface_w, m_surface_h, DDSCAPS_SYSTEMMEMORY);
		m_p_minimap_surface->SetTransparency( 0xffff );
		m_p_minimap_surface->FillSurface(0x0000);
		
		m_p_minimap_surface->Lock();
		m_p_minimap_surface->BltSprite(&point, &minimapSPK[0]);
		m_p_minimap_surface->Unlock();
		
		int map_minus_w = 200-m_surface_w;
		if(x+w == RESOLUTION_X)
			x -= m_pC_minimap_spk->GetWidth(MINIMAP_MAIN)+m_pC_minimap_spk->GetWidth(MINIMAP_RIGHT)-map_minus_w-w;
		w = m_pC_minimap_spk->GetWidth(MINIMAP_MAIN)+m_pC_minimap_spk->GetWidth(MINIMAP_RIGHT)-map_minus_w;
		
		int map_minus_h = 100-m_surface_h;
		if(x+h == RESOLUTION_Y)
			y -= m_pC_minimap_spk->GetHeight(MINIMAP_MAIN)-map_minus_h-h;
		h = m_pC_minimap_spk->GetHeight(MINIMAP_MAIN)-map_minus_h;
	}
	
	m_portal.clear();
	m_portal_zone_id.clear();
	m_npc.clear();
	m_shrine.clear();
}

//-----------------------------------------------------------------------------
// SetSafetyZone
//
// 안전지대 설정
//-----------------------------------------------------------------------------
void C_VS_UI_MINIMAP::SetSafetyZone(RECT rect, bool my_zone)
{
	m_bl_refresh = true;
	
	m_p_minimap_surface->Lock();
	WORD *mem = (WORD *)m_p_minimap_surface->GetSurfacePointer();
	int pitch = m_p_minimap_surface->GetSurfacePitch();
	
	//int map_w = m_surface_w, map_h = m_surface_h;
	//	if(m_map_w != m_map_h)
	//	{
	//		if(m_map_w > m_map_h)map_h = map_h * m_map_h / m_map_w;
	//		if(m_map_h > m_map_w)map_w = map_w * m_map_w / m_map_h;
	//	}
	
	int remainy = rect.bottom % m_map_h;
	int remainx = rect.right % m_map_w;

	if(remainy)
		remainy = 1;
	else remainy = 0;

	if(remainx)
		remainx = 1;
	else remainx = 0;

	if(m_map_w && m_map_h)
	{
		for(int y = rect.top*m_surface_h/m_map_h ; 
		y <= min((remainy+rect.bottom)*m_surface_h/m_map_h ,m_surface_h-1); y++)
		{
			for(int x = rect.left*m_surface_w/m_map_w; 
			x <= min((remainx+rect.right)*m_surface_w/m_map_w ,m_surface_w-1); x++)
			{
				if(my_zone)
					mem[y*pitch/2+x] = mem[y*pitch/2+x] & CDirectDraw::Get_G_Bitmask();
				else
					mem[y*pitch/2+x] = mem[y*pitch/2+x] & CDirectDraw::Get_R_Bitmask();
			}
		}
	}
	
	/*if(m_map_w && m_map_h)
	{
		for(int y = rect.top*map_h/m_map_h + (m_surface_h - map_h)/2; 
		y <= min(remainy+rect.bottom*map_h/m_map_h + (m_surface_h - map_h)/2,m_surface_h-1); y++)
		{
			for(int x = rect.left*map_w/m_map_w + (m_surface_w - map_w)/2; 
			x <= min(remainx+rect.right*map_w/m_map_w + (m_surface_w - map_w)/2,m_surface_w-1); x++)
			{
				if(my_zone)
					mem[y*pitch/2+x] = mem[y*pitch/2+x] & CDirectDraw::Get_G_Bitmask();
				else
					mem[y*pitch/2+x] = mem[y*pitch/2+x] & CDirectDraw::Get_R_Bitmask();
			}
		}
	}*/
	
	m_p_minimap_surface->Unlock();
}

//-----------------------------------------------------------------------------
// SetNPC
//
// NPC 설정
//-----------------------------------------------------------------------------
void C_VS_UI_MINIMAP::SetNPC(MINIMAP_NPC npc)
{
	if(npc.id >= 560 && npc.id <= 563)	// 성 상징물 받침대
	{
		for(int i = 0; i < m_shrine.size(); i++)
		{
			if(npc.id == m_shrine[i].id)break;
		}
		
		if(i == m_shrine.size())
		{
			MINIMAP_SHRINE shrine;
			shrine.x = npc.x;
			shrine.y = npc.y;
			shrine.id = npc.id;
			shrine.name = npc.name;
			shrine.type = 0;
			m_shrine.push_back(shrine);
		}
	}
	else if(npc.id >= 526 && npc.id <= 537)	// 수호성단
	{
		for(int i = 0; i < m_shrine.size(); i++)
		{
			if(npc.id == m_shrine[i].id)break;
		}
		
		if(i == m_shrine.size())
		{
			MINIMAP_SHRINE shrine;
			shrine.x = npc.x;
			shrine.y = npc.y;
			shrine.id = npc.id;
			shrine.name = npc.name;
			shrine.type = 1;
			m_shrine.push_back(shrine);
		}
	}
	else if(npc.id >= 538 && npc.id <= 549)	// 성지 성단
	{
		for(int i = 0; i < m_shrine.size(); i++)
		{
			if(npc.id == m_shrine[i].id)break;
		}
		
		if(i == m_shrine.size())
		{
			MINIMAP_SHRINE shrine;
			shrine.x = npc.x;
			shrine.y = npc.y;
			shrine.id = npc.id;
			shrine.name = npc.name;
			shrine.type = 2;
			m_shrine.push_back(shrine);
		}
	}
	else
	{
		for(int i = 0; i < m_npc.size(); i++)
		{
			if(npc.id == m_npc[i].id)break;
		}
		
		if(i == m_npc.size())
		{
			m_npc.push_back(npc);
		}
	}
}

//-----------------------------------------------------------------------------
// SetPortal
//
// 포탈 설정
//-----------------------------------------------------------------------------
void C_VS_UI_MINIMAP::SetPortal(RECT rect, int id)
{
	extern std::map<int, bool> g_mapPremiumZone;
	if(g_pUserInformation->IsNetmarble && g_mapPremiumZone.find(id) != g_mapPremiumZone.end())
		return;
	
	for(int i = 0; i < m_portal.size(); i++)
	{
		if(m_portal_zone_id[i] == id &&
			m_portal[i].top <= rect.bottom +5 &&
			m_portal[i].bottom +5>= rect.top &&
			m_portal[i].left <= rect.right +5 &&
			m_portal[i].right +5 >= rect.left
			)
		{
			m_portal[i].left = min(m_portal[i].left, rect.left);
			m_portal[i].right = max(m_portal[i].right, rect.right);
			m_portal[i].top = min(m_portal[i].top, rect.top);
			m_portal[i].bottom = max(m_portal[i].bottom, rect.bottom);
			break;
		}
	}
	
	if(i == m_portal.size())
	{
		//		if(rect.top > 4 && rect.bottom < m_map_h -1 && rect.left > 0 && rect.right < m_map_w)
		{
			m_portal.push_back(rect);
			m_portal_zone_id.push_back(id);
		}
		
	}
}


//-----------------------------------------------------------------------------
// Timer
//
//-----------------------------------------------------------------------------
bool C_VS_UI_MINIMAP::TimerMinimap()
{
	if(m_dw_minimap_prev_tickcount+m_dw_minimap_millisec <= GetTickCount())
	{
		m_dw_minimap_prev_tickcount = GetTickCount();
		return true;
	}
	
	return false;
}

C_VS_UI_WINDOW_MANAGER *gpC_vs_ui_window_manager;

C_VS_UI_WINDOW_MANAGER::C_VS_UI_WINDOW_MANAGER()
{
	SetDefault();
}

C_VS_UI_WINDOW_MANAGER::~C_VS_UI_WINDOW_MANAGER()
{
}

void C_VS_UI_WINDOW_MANAGER::SetDefault()
{
	int i = 0, j = 0;
	
	for (i=0; i < C_VS_UI_SKILL::HOTKEY_MAX; i++)
		for(j=0; j < C_VS_UI_SKILL::GRADE_MAX; j++)
			m_skill_hotkey_buf[i][j] = NOT_SELECTED;
		
	for(i = 0; i < CLD_TOTAL; i++)
		SetFilter((CHAT_LINE_CONDITION)i, true);
		
	for( i = 0; i < WINDOW_TOTAL; i++)
	{
		SetAlpha((WINDOW_ID)i, false);
		SetAutoHide((WINDOW_ID)i, Window::ATTRIBUTES_HIDE_NOT);
		SetRect((WINDOW_ID)i, Rect(-1, -1, -1, -1));
	}
		
	m_bl_hpbar_small = false;
	m_bl_hpbar_height = false;
	m_bl_quickitemslot_height = false;
	m_bl_effectstatus_height = false;

	m_i_main_tab = 0;

	// effectstatus는 숨겨논다
	SetAutoHide(EFFECT_STATUS, Window::ATTRIBUTES_HIDE_HEIGHT);		
}

void C_VS_UI_WINDOW_MANAGER::SetFilter(CHAT_LINE_CONDITION filter, bool set)
{
	m_filter[filter] = set;
}

void C_VS_UI_WINDOW_MANAGER::SetAlpha(WINDOW_ID id, bool bl_alpha)
{
	m_alpha[id] = bl_alpha;
}

void C_VS_UI_WINDOW_MANAGER::SetAutoHide(WINDOW_ID id, Window::ATTRIBUTES_HIDE hide)
{
	m_autohide[id] = hide;
}

void C_VS_UI_WINDOW_MANAGER::SetRect(WINDOW_ID id, Rect rect)
{
	m_rect[id] = rect;
}

void C_VS_UI_WINDOW_MANAGER::SetHotKey(int **hotkey)
{
	if(hotkey)
	{
		memcpy(m_skill_hotkey_buf, hotkey, C_VS_UI_SKILL::HOTKEY_MAX*C_VS_UI_SKILL::GRADE_MAX*sizeof(int));
	}
}

void C_VS_UI_WINDOW_MANAGER::SaveToFile(ofstream &file)
{
	int i = 0, j = 0;
	
	DWORD flag = 0;
	file.write((const char *)&flag, sizeof(DWORD));
	
	BYTE hotkey_num = C_VS_UI_SKILL::HOTKEY_MAX, grade_num = C_VS_UI_SKILL::GRADE_MAX;
	file.write((const char *)&hotkey_num, sizeof(BYTE));
	file.write((const char *)&grade_num, sizeof(BYTE));
	for(i = 0; i < hotkey_num; i++)
	{
		for(j = 0; j < grade_num; j++)
		{
			file.write((const char *)&m_skill_hotkey_buf[i][j], sizeof(int));
		}
	}
	
	BYTE filter_num = CLD_TOTAL;
	file.write((const char *)&filter_num, sizeof(BYTE));
	for(i = 0; i < filter_num; i++)
	{
		file.write((const char *)&m_filter[i], sizeof(bool));
	}
	
	int window_num = WINDOW_TOTAL;
	file.write((const char *)&window_num, sizeof(int));
	for(i = 0; i < window_num; i++)
	{
		file.write((const char *)&m_alpha[i], sizeof(bool));
		file.write((const char *)&m_autohide[i], sizeof(Window::ATTRIBUTES_HIDE));
		file.write((const char *)&m_rect[i], sizeof(Rect));
	}
	
	file.write((const char *)&m_bl_hpbar_height, sizeof(bool));
	file.write((const char *)&m_bl_hpbar_small, sizeof(bool));
	file.write((const char *)&m_bl_quickitemslot_height, sizeof(bool));
	file.write((const char *)&m_bl_effectstatus_height, sizeof(bool));
	file.write((const char *)&m_i_main_tab,sizeof(int));
}

void C_VS_UI_WINDOW_MANAGER::LoadFromFile(ifstream &file)
{
	// 각각 개수가 늘거나 줄었을때에 대한 처리는.... 귀찮으니까 나중에 하쟈-.-;
	int i = 0, j = 0;
	
	DWORD flag = 0;
	file.read((char *)&flag, sizeof(DWORD));
	
	BYTE hotkey_num = C_VS_UI_SKILL::HOTKEY_MAX, grade_num = C_VS_UI_SKILL::GRADE_MAX;
	file.read((char *)&hotkey_num, sizeof(BYTE));
	file.read((char *)&grade_num, sizeof(BYTE));
	for(i = 0; i < hotkey_num; i++)
	{
		for(j = 0; j < grade_num; j++)
		{
			file.read((char *)&m_skill_hotkey_buf[i][j], sizeof(int));
		}
	}
	
	BYTE filter_num = CLD_TOTAL;
	file.read((char *)&filter_num, sizeof(BYTE));
	for(i = 0; i < filter_num; i++)
	{
		file.read((char *)&m_filter[i], sizeof(bool));
	}
	
	int window_num = WINDOW_TOTAL;
	file.read((char *)&window_num, sizeof(int));
	for(i = 0; i < window_num; i++)
	{
		file.read((char *)&m_alpha[i], sizeof(bool));
		file.read((char *)&m_autohide[i], sizeof(Window::ATTRIBUTES_HIDE));
		file.read((char *)&m_rect[i], sizeof(Rect));
	}
	
	file.read((char *)&m_bl_hpbar_height, sizeof(bool));
	file.read((char *)&m_bl_hpbar_small, sizeof(bool));
	file.read((char *)&m_bl_quickitemslot_height, sizeof(bool));
	file.read((char *)&m_bl_effectstatus_height, sizeof(bool));
	file.read((char *)&m_i_main_tab,sizeof(int));
//	gC_vs_ui.SetSelectMainTab(tab);
}


C_VS_UI_TEAM_LIST::C_VS_UI_TEAM_LIST(bool ready)
{
	m_bl_ready = ready;
	
	g_RegisterWindow(this);
	
	AttrTopmost(true);
	AttrKeyboardControl(true);

	if(gbl_vampire_interface)
		m_guild_spk.Open(SPK_VAMPIRE_TEAM);
	else
		m_guild_spk.Open(SPK_SLAYER_TEAM);
	
	Set(RESOLUTION_X/2-m_guild_spk.GetWidth(0)/2, RESOLUTION_Y/2 - m_guild_spk.GetHeight(0)/2, m_guild_spk.GetWidth(0), m_guild_spk.GetHeight(0));
	
	
	// set button
	m_pC_button_group = new ButtonGroup(this);
	
	if(gbl_vampire_interface)
	{
		m_vampire_point.x = 17;
		m_vampire_point.y = 0;
	}
	else
	{
		m_vampire_point.x = 0;
		m_vampire_point.y = 0;
	}
	
	int help_x = 353, x_x = 367, button_y = 2;
	const int scroll_x = 365+m_vampire_point.x, scroll_up_y = 96+m_vampire_point.y, scroll_down_y = 312+m_vampire_point.y;
	
	//	int print_x[4], print_y = y+123, print_gap = 23;
	int scroll_tag_x = 365+m_vampire_point.x, scroll_tag_y = y+108+m_vampire_point.y, scroll_tag_height = 198;
	
	if(gbl_vampire_interface)
	{
		button_y = 20;
		help_x+=20; x_x+=20;
	}
	//	if(m_bl_ready)
	{
		m_print_x[0] = x+95+m_vampire_point.x;	//team
		m_print_x[1] = x+180+m_vampire_point.x;	//leader
		m_print_x[2] = x+256+m_vampire_point.x;	//members
		m_print_x[3] = x+325+m_vampire_point.x;	//ranking
		m_print_y = y+123+m_vampire_point.y;
		m_print_gap = 23;
	}
	
	
	//help, x버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + help_x, y + button_y, m_guild_spk.GetWidth(HELP_BUTTON), m_guild_spk.GetHeight(HELP_BUTTON), HELP_ID, this, HELP_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + x_x, y + button_y, m_guild_spk.GetWidth(X_BUTTON), m_guild_spk.GetHeight(X_BUTTON), X_ID, this, X_BUTTON));
	
	//scroll up, down 버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + scroll_x, y + scroll_up_y, m_guild_spk.GetWidth(SCROLL_UP_BUTTON), m_guild_spk.GetHeight(SCROLL_UP_BUTTON), SCROLL_UP_ID, this, SCROLL_UP_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + scroll_x, y + scroll_down_y, m_guild_spk.GetWidth(SCROLL_DOWN_BUTTON), m_guild_spk.GetHeight(SCROLL_DOWN_BUTTON), SCROLL_DOWN_ID, this, SCROLL_DOWN_BUTTON));
	
	//team_list 버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + w/2-m_guild_spk.GetWidth(TITLE_TEAM_LIST)/2, y + 50, m_guild_spk.GetWidth(TITLE_TEAM_LIST), m_guild_spk.GetHeight(TITLE_TEAM_LIST), TEAM_LIST_ID, this, TITLE_TEAM_LIST));
	
	//index버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_print_x[0] - m_guild_spk.GetWidth(INDEX_TEAM)/2-10, m_print_y+m_print_gap*-1, m_guild_spk.GetWidth(INDEX_TEAM), m_guild_spk.GetHeight(INDEX_TEAM), INDEX_TEAM_ID, this, INDEX_TEAM));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_print_x[1] - m_guild_spk.GetWidth(INDEX_LEADER)/2, m_print_y+m_print_gap*-1, m_guild_spk.GetWidth(INDEX_LEADER), m_guild_spk.GetHeight(INDEX_LEADER), INDEX_LEADER_ID, this, INDEX_LEADER));
	if(m_bl_ready)
	{
		//		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_print_x[2] - m_guild_spk.GetWidth(INDEX_EXPIREDATE)/2, m_print_y+m_print_gap*-1, m_guild_spk.GetWidth(INDEX_EXPIREDATE), m_guild_spk.GetHeight(INDEX_EXPIREDATE), INDEX_EXPIREDATE_ID, this, INDEX_EXPIREDATE));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_print_x[3] - m_guild_spk.GetWidth(INDEX_MEMBERS)/2, m_print_y+m_print_gap*-1, m_guild_spk.GetWidth(INDEX_MEMBERS), m_guild_spk.GetHeight(INDEX_MEMBERS), INDEX_MEMBERS_ID, this, INDEX_MEMBERS));
	}
	else
	{
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_print_x[2] - m_guild_spk.GetWidth(INDEX_MEMBERS)/2, m_print_y+m_print_gap*-1, m_guild_spk.GetWidth(INDEX_MEMBERS), m_guild_spk.GetHeight(INDEX_MEMBERS), INDEX_MEMBERS_ID, this, INDEX_MEMBERS));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_print_x[3] - m_guild_spk.GetWidth(INDEX_RANKING)/2, m_print_y+m_print_gap*-1, m_guild_spk.GetWidth(INDEX_RANKING), m_guild_spk.GetHeight(INDEX_RANKING), INDEX_RANKING_ID, this, INDEX_RANKING));
	}
	
	//search버튼
	if(gbl_vampire_interface)
	{
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+54+19, y+h-79, m_guild_spk.GetWidth(SEARCH_NAME), m_guild_spk.GetHeight(SEARCH_NAME), SEARCH_TYPE_ID, this, SEARCH_NAME));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+268+18, y+h-79, m_guild_spk.GetWidth(SEARCH_SEARCH), m_guild_spk.GetHeight(SEARCH_SEARCH), SEARCH_ID, this, SEARCH_SEARCH));
	}
	else
	{
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+54, y+h-49, m_guild_spk.GetWidth(SEARCH_NAME), m_guild_spk.GetHeight(SEARCH_NAME), SEARCH_TYPE_ID, this, SEARCH_NAME));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+268, y+h-49, m_guild_spk.GetWidth(SEARCH_SEARCH), m_guild_spk.GetHeight(SEARCH_SEARCH), SEARCH_ID, this, SEARCH_SEARCH));
	}
	
	//CLOSE버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+w-14, y+h-49, m_guild_spk.GetWidth(CLOSE_BUTTON), m_guild_spk.GetHeight(CLOSE_BUTTON), CLOSE_ID, this, CLOSE_BUTTON));
	
	m_scroll = 0;
	m_bl_scrolling = false;
	
	m_iFocus = -1;
	m_last_sort_type = 0;
	
	m_search_type = SEARCH_NAME;
	
	if(gbl_vampire_interface)
		m_lev_search.SetPosition(x+w/2-m_guild_spk.GetWidth(BAR_SEARCH)/2+2, y+h-80+4);
	else
		m_lev_search.SetPosition(x+w/2-m_guild_spk.GetWidth(BAR_SEARCH)/2+2, y+h-50+4);
	m_lev_search.SetByteLimit(16);
	
	Attach(&m_lev_search);
	
}

C_VS_UI_TEAM_LIST::~C_VS_UI_TEAM_LIST()
{
	g_UnregisterWindow(this);
	
	m_lev_search.Unacquire();
	
	gC_vs_ui.AcquireChatting();
	
	DeleteNew( m_pC_button_group );
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_LIST::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_LIST::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_LIST::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_LIST::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_LIST::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_LIST::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();
	
	AttrTopmost(true);
	AttrKeyboardControl(true);
	gpC_window_manager->AppearWindow(this);
	
	m_lev_search.Acquire();
	m_lev_search.EraseAll();
	
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_LIST::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_LIST::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_LIST::Show 
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_TEAM_LIST::Show()
{
	int scroll_tag_x = x+365+m_vampire_point.x, scroll_tag_y = y+108+m_vampire_point.y, scroll_tag_height = 198;
	
	// Lock 부분 모으기~
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_guild_spk.BltLocked(x, y);

		if(gbl_vampire_interface)
			m_guild_spk.BltLocked(x+w/2-m_guild_spk.GetWidth(BAR_SEARCH)/2, y+h-80, BAR_SEARCH);
		else
			m_guild_spk.BltLocked(x+w/2-m_guild_spk.GetWidth(BAR_SEARCH)/2, y+h-50, BAR_SEARCH);

		m_guild_spk.BltLocked(scroll_tag_x-2, m_print_y-6+m_print_gap*-1, SCROLL_BAR);
		m_guild_spk.BltLocked(m_print_x[0]-71, m_print_y-5+m_print_gap*-1,BAR_DARKEN);	
		if(m_iFocus != -1)
			m_guild_spk.BltLocked(m_print_x[0]-71, m_print_y-5+m_print_gap*m_iFocus, BAR_HILIGHTED);
		//스크롤TAG찍기
		int list_size_search=m_bl_ready ? m_v_ready_team_search_list.size() : m_v_regist_team_search_list.size() ;
		int list_size=m_bl_ready ? m_v_ready_team_list.size() : m_v_regist_team_list.size() ;
		if(list_size_search)
		{
			if(list_size_search > 9)	// 스크롤이 생기는 경우에만
			{
				if(m_bl_scrolling)
					m_guild_spk.BltLocked(scroll_tag_x, min(max(gpC_mouse_pointer->GetY(), scroll_tag_y), scroll_tag_y+scroll_tag_height), SCROLL_TAG);
				else
					m_guild_spk.BltLocked(scroll_tag_x, scroll_tag_y+scroll_tag_height*m_scroll/(list_size_search-9), SCROLL_TAG);
			}
		}
		else
		if(list_size > 9)	// 스크롤이 생기는 경우에만
		{
			if(m_bl_scrolling)
				m_guild_spk.BltLocked(scroll_tag_x, min(max(gpC_mouse_pointer->GetY(), scroll_tag_y), scroll_tag_y+scroll_tag_height), SCROLL_TAG);
			else
				m_guild_spk.BltLocked(scroll_tag_x, scroll_tag_y+scroll_tag_height*m_scroll/(list_size-9), SCROLL_TAG);
		}
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	m_lev_search.Show();	
	
	gpC_base->m_p_DDSurface_back->HLine(m_print_x[0]-72, m_print_y-6+m_print_gap*-1, 334, 0);
	gpC_base->m_p_DDSurface_back->HLine(m_print_x[0]-72, m_print_y-6+m_print_gap*0, 334, 0);	
	
	gpC_base->m_p_DDSurface_back->VLine(m_print_x[0]-72, m_print_y-6+m_print_gap*-1, m_print_gap*10, 0);
	gpC_base->m_p_DDSurface_back->VLine(m_print_x[0]-49, m_print_y-6, m_print_gap*9, 0);
	gpC_base->m_p_DDSurface_back->VLine(m_print_x[0]+48, m_print_y-6+m_print_gap*-1, m_print_gap*10, 0);
	gpC_base->m_p_DDSurface_back->VLine(m_print_x[1]+37, m_print_y-6+m_print_gap*-1, m_print_gap*10, 0);
	gpC_base->m_p_DDSurface_back->VLine(m_print_x[2]+35, m_print_y-6+m_print_gap*-1, m_print_gap*10, 0);
	gpC_base->m_p_DDSurface_back->VLine(m_print_x[3]+32, m_print_y-6+m_print_gap*-1, m_print_gap*10+1, 0);	
	
	if(m_bl_ready)	// 대기 list
	{
		for(int i = 0; i < 9; i++)
		{
			gpC_base->m_p_DDSurface_back->HLine(m_print_x[0]-72, m_print_y+(i+1)*m_print_gap-6, 334, 0);
			
			if(m_v_ready_team_search_list.size() && m_scroll+i >= m_v_ready_team_search_list.size() || m_scroll+i >= m_v_ready_team_list.size())
				continue;
			
			const char *string;
			
			READY_TEAM_LIST *info;
			if(m_v_ready_team_search_list.size())
				info = &m_v_ready_team_search_list[i+m_scroll];
			else
				info = &m_v_ready_team_list[i+m_scroll];
			
			string = info->TEAM_NAME.c_str();
			g_PrintColorStr(m_print_x[0]-g_GetStringWidth(string, gpC_base->m_chatting_pi.hfont)/2, m_print_y+i*m_print_gap, string, gpC_base->m_chatting_pi, RGB_BLACK);
			
			// 대기 리스트인경우 길드 마크 대신 빈 박스를 출력할라다 걍 출력-ㅅ-
			RECT rect;
			rect.left = m_print_x[0]-70;
			rect.right = rect.left +20;
			rect.top = m_print_y+i*m_print_gap-4;
			rect.bottom = rect.top+20;
			
			// 길드 마크 출력
			CSprite *p_guild_mark;
			p_guild_mark=g_pGuildMarkManager->GetGuildMarkSmall(info->guild_id);
			if(p_guild_mark==NULL)
			{
				g_pGuildMarkManager->LoadGuildMark(info->guild_id);
				p_guild_mark = g_pGuildMarkManager->GetGuildMarkSmall(info->guild_id);			
			}
			POINT guild_point = { rect.left,rect.top };
			if(p_guild_mark != NULL)	// 악!! CSprite직접 쓸때는 Lock할것!! by 쑥갓
			{
				if(gpC_base->m_p_DDSurface_back->Lock())
				{
					gpC_base->m_p_DDSurface_back->BltSprite(&guild_point, p_guild_mark);
					gpC_base->m_p_DDSurface_back->Unlock();
				}
			}
			else
			{
				RECT rt={rect.left,rect.top,rect.left+20,rect.top+20};
				DrawAlphaBox(&rt, 255, 255, 255, 15);
			}
			
			g_FL2_GetDC();
			string = info->LEADER_NAME.c_str();
			g_PrintColorStr(m_print_x[1]-g_GetStringWidth(string, gpC_base->m_chatting_pi.hfont)/2, m_print_y+i*m_print_gap, string, gpC_base->m_chatting_pi, RGB_BLACK);
			//			string = info->EXPIRE_DATE.c_str();
			//			g_PrintColorStr(m_print_x[2]-g_GetStringWidth(string, gpC_base->m_small_pi.hfont)/2, m_print_y+i*m_print_gap, string, gpC_base->m_small_pi, RGB_BLACK);
			
			char sz_temp[100];
			wsprintf(sz_temp, "%d/%d", info->MEMBERS, info->MEMBERS_MAX);
			g_PrintColorStr(m_print_x[3]-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, m_print_y+i*m_print_gap, sz_temp, gpC_base->m_chatting_pi, RGB_BLACK);
			g_FL2_ReleaseDC();
		}
	}
	else	//등록된 list
	{
		for(int i = 0; i < 9; i++)
		{
			gpC_base->m_p_DDSurface_back->HLine(m_print_x[0]-72, m_print_y+(i+1)*m_print_gap-6, 334, 0);
			
			if(m_v_regist_team_search_list.size() && m_scroll+i >= m_v_regist_team_search_list.size() || m_scroll+i >= m_v_regist_team_list.size())
				continue;
			
			const char *string;
			
			REGIST_TEAM_LIST *info;
			if(m_v_regist_team_search_list.size())
				info = &m_v_regist_team_search_list[i+m_scroll];
			else
				info = &m_v_regist_team_list[i+m_scroll];
			
			string = info->TEAM_NAME.c_str();
			g_PrintColorStr(m_print_x[0]-g_GetStringWidth(string, gpC_base->m_chatting_pi.hfont)/2, m_print_y+i*m_print_gap, string, gpC_base->m_chatting_pi, RGB_BLACK);
			RECT rect;
			rect.left = m_print_x[0]-70;
			//			rect.right = rect.left +20;
			rect.top = m_print_y+i*m_print_gap-4;
			//			rect.bottom = rect.top+20;
			
			// 길드 마크 출력
			CSprite *p_guild_mark;
			p_guild_mark=g_pGuildMarkManager->GetGuildMarkSmall(info->guild_id);
			if(p_guild_mark==NULL)
			{
				g_pGuildMarkManager->LoadGuildMark(info->guild_id);
				p_guild_mark = g_pGuildMarkManager->GetGuildMarkSmall(info->guild_id);			
			}
			POINT guild_point = { rect.left,rect.top };
			if(p_guild_mark != NULL)	// 악!! CSprite직접 쓸때는 Lock할것!! by 쑥갓
			{
				if(gpC_base->m_p_DDSurface_back->Lock())
				{
					gpC_base->m_p_DDSurface_back->BltSprite(&guild_point, p_guild_mark);
					gpC_base->m_p_DDSurface_back->Unlock();
				}
			}
			else
			{
				RECT rt={rect.left,rect.top,rect.left+20,rect.top+20};
				DrawAlphaBox(&rt, 255, 255, 255, 15);
			}
			
			g_FL2_GetDC();
			string = info->LEADER_NAME.c_str();
			g_PrintColorStr(m_print_x[1]-g_GetStringWidth(string, gpC_base->m_chatting_pi.hfont)/2, m_print_y+i*m_print_gap, string, gpC_base->m_chatting_pi, RGB_BLACK);
			
			char sz_temp[100];
			wsprintf(sz_temp, "%d", info->MEMBERS);
			g_PrintColorStr(m_print_x[2]-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, m_print_y+i*m_print_gap, sz_temp, gpC_base->m_chatting_pi, RGB_BLACK);
			
			wsprintf(sz_temp, "%d", info->RANKING);
			g_PrintColorStr(m_print_x[3]-g_GetStringWidth(sz_temp, gpC_base->m_chatting_pi.hfont)/2, m_print_y+i*m_print_gap, sz_temp, gpC_base->m_chatting_pi, RGB_BLACK);
			g_FL2_ReleaseDC();
		}
	}	
	m_pC_button_group->ShowDescription();
	
	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_LIST::ShowButtonDescription
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_LIST::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char*	m_help_string[EXEC_MAX] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_REGIST].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_LIST_UP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_LIST_DOWN].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_REFRESH_LIST].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SORT_TEAM_NAME].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SORT_LEADER_NAME].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SORT_EXPIRE_DATE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SORT_NUMBER_MEMBER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SORT_RANKING].GetString(),
			
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CHANGE_FIND_MODE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_FIND].GetString(),
	};
	
	if (!m_bl_scrolling)
		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_LIST::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_LIST::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);	
	switch(p_button->GetID())
	{
	case X_ID:
	case HELP_ID:
	case SCROLL_UP_ID:
	case SCROLL_DOWN_ID:
		if(p_button->GetFocusState() && p_button->GetPressState())
			m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index+1);
		else
			m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index);
		break;
		
	case TEAM_LIST_ID:
	case INDEX_TEAM_ID:
	case INDEX_LEADER_ID:
		//	case INDEX_EXPIREDATE_ID:
	case INDEX_MEMBERS_ID:
	case INDEX_RANKING_ID:
	case CLOSE_ID:
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_guild_spk.BltLocked(p_button->x, p_button->y+2, p_button->m_image_index+1);
			else
				m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index+1);
		}
		else
			m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index);
		break;
		
	case SEARCH_TYPE_ID:
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_guild_spk.BltLocked(p_button->x, p_button->y, m_search_type+2);
			else
				m_guild_spk.BltLocked(p_button->x, p_button->y, m_search_type+1);
		}
		else
			m_guild_spk.BltLocked(p_button->x, p_button->y, m_search_type);
		break;
		
	case SEARCH_ID:
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index+2);
			else
				m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index+1);
		}
		else
			m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index);
		break;
		
	}
	
}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_LIST::KeyboardControl(UINT message, UINT key, long extra)
{
	
	Window::KeyboardControl(message, key, extra);
	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_RETURN:
			Run(SEARCH_ID);
			break;
			
		case VK_ESCAPE: 
			gpC_base->SendMessage(UI_CLOSE_TEAM_LIST);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_LIST::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_TEAM_LIST::MouseControl(UINT message, int _x, int _y)
{
	if(!m_lev_search.IsAcquire())
		m_lev_search.Acquire();
	
	int scroll_tag_x = x+365+m_vampire_point.x, scroll_tag_y = y+108+m_vampire_point.y, scroll_tag_height = 198;
	
	Window::MouseControl(message, _x, _y);
	
	switch(message)
	{
	case M_MOVING:
		if(m_bl_scrolling)
		{
			if(m_bl_ready)
			{
				if(m_v_ready_team_search_list.size())
					m_scroll = min(m_v_ready_team_search_list.size()-9, (m_v_ready_team_search_list.size()-9)*(max(_y, scroll_tag_y) - scroll_tag_y + scroll_tag_height/(m_v_ready_team_search_list.size()-9)/2)/scroll_tag_height);
				else
					m_scroll = min(m_v_ready_team_list.size()-9, (m_v_ready_team_list.size()-9)*(max(_y, scroll_tag_y) - scroll_tag_y + scroll_tag_height/(m_v_ready_team_list.size()-9)/2)/scroll_tag_height);
			}
			else
			{
				if(m_v_regist_team_search_list.size())
					m_scroll = min(m_v_regist_team_search_list.size()-9, (m_v_regist_team_search_list.size()-9)*(max(_y, scroll_tag_y) - scroll_tag_y + scroll_tag_height/(m_v_regist_team_search_list.size()-9)/2)/scroll_tag_height);
				else
					m_scroll = min(m_v_regist_team_list.size()-9, (m_v_regist_team_list.size()-9)*(max(_y, scroll_tag_y) - scroll_tag_y + scroll_tag_height/(m_v_regist_team_list.size()-9)/2)/scroll_tag_height);
			}
		}
		else
		{
			if(_x > m_print_x[0]-72 && _x < m_print_x[0]-72+334 && _y > m_print_y-5 && _y < m_print_y-7+m_print_gap*9)
			{
				if(m_iFocus != (_y - (m_print_y-6))/m_print_gap)
				{
					if(gpC_Imm)
						gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
					m_iFocus = (_y - (m_print_y-6))/m_print_gap;
				}
			}
			else
			{
				if(m_iFocus != -1)
				{
					if(gpC_Imm)
						gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
					m_iFocus = -1;
				}
			}
		}
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if(_x >= scroll_tag_x && _x <= scroll_tag_x+m_guild_spk.GetWidth(SCROLL_BAR) && _y >= scroll_tag_y && _y <= scroll_tag_y+scroll_tag_height)
		{
			if(m_bl_ready)
			{
				if(m_v_ready_team_search_list.size() > 9)
				{
					m_bl_scrolling = true;
					m_scroll = (m_v_ready_team_search_list.size()-9)*(_y - scroll_tag_y + scroll_tag_height/(m_v_ready_team_search_list.size()-9)/2)/scroll_tag_height;
				}
				else
					if(m_v_ready_team_list.size() > 9)	// 스크롤이 생기는 경우에만
					{
						m_bl_scrolling = true;
						m_scroll = (m_v_ready_team_list.size()-9)*(_y - scroll_tag_y + scroll_tag_height/(m_v_ready_team_list.size()-9)/2)/scroll_tag_height;
					}
			}
			else
			{
				if(m_v_regist_team_search_list.size() > 9)	// 스크롤이 생기는 경우에만
				{
					m_bl_scrolling = true;
					m_scroll = (m_v_regist_team_search_list.size()-9)*(_y - scroll_tag_y + scroll_tag_height/(m_v_regist_team_search_list.size()-9)/2)/scroll_tag_height;
				}
				else
					if(m_v_regist_team_list.size() > 9)	// 스크롤이 생기는 경우에만
					{
						m_bl_scrolling = true;
						m_scroll = (m_v_regist_team_list.size()-9)*(_y - scroll_tag_y + scroll_tag_height/(m_v_regist_team_list.size()-9)/2)/scroll_tag_height;
					}
			}
		}
		else if(m_iFocus != -1)
		{
			if(m_bl_ready)
			{
				if(m_v_ready_team_search_list.size())
				{
					if(m_v_ready_team_search_list.size() > m_iFocus + m_scroll)
						gpC_base->SendMessage(UI_SELECT_READY_TEAM_LIST, m_v_ready_team_search_list[m_iFocus + m_scroll].guild_id, 0, (void *)m_v_ready_team_search_list[m_iFocus + m_scroll].TEAM_NAME.c_str());
				}
				else
					if(m_v_ready_team_list.size())
					{
						if(m_v_ready_team_list.size() > m_iFocus + m_scroll)
							gpC_base->SendMessage(UI_SELECT_READY_TEAM_LIST, m_v_ready_team_list[m_iFocus + m_scroll].guild_id, 0, (void *)m_v_ready_team_list[m_iFocus + m_scroll].TEAM_NAME.c_str());
					}
			}
			else
			{
				if(m_v_regist_team_search_list.size())
				{
					if(m_v_regist_team_search_list.size() > m_iFocus + m_scroll)
						gpC_base->SendMessage(UI_SELECT_REGIST_TEAM_LIST, m_v_regist_team_search_list[m_iFocus + m_scroll].guild_id, 0, (void *)m_v_regist_team_search_list[m_iFocus + m_scroll].TEAM_NAME.c_str());
				}
				else
				if(m_v_regist_team_list.size())
				{
					if(m_v_regist_team_list.size() > m_iFocus + m_scroll)
						gpC_base->SendMessage(UI_SELECT_REGIST_TEAM_LIST, m_v_regist_team_list[m_iFocus + m_scroll].guild_id, 0, (void *)m_v_regist_team_list[m_iFocus + m_scroll].TEAM_NAME.c_str());
				}
			}
		}
		break;
		
	case M_LEFTBUTTON_UP:
		if(m_bl_scrolling)
		{
			m_bl_scrolling = false;
		}
		break;
		
	case M_WHEEL_UP:
		Run(SCROLL_UP_ID);
		break;
		
	case M_WHEEL_DOWN:
		Run(SCROLL_DOWN_ID);
		break;
	}
	
	m_pC_button_group->MouseControl(message, _x, _y);
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_LIST::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_TEAM_LIST::IsPixel(int _x, int _y)
{
	bool re = m_guild_spk.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
	
	if(re == false)
	{
		if(m_guild_spk.IsPixel(_x - (x+w-14), _y - (y+h-49), CLOSE_BUTTON))
			re = true;
	}
	
	return re;
}


//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_LIST::C_VS_UI_TEAM_MEMBER_LIST
//
//
//-----------------------------------------------------------------------------
C_VS_UI_TEAM_MEMBER_LIST::C_VS_UI_TEAM_MEMBER_LIST()
{
#ifndef _LIB
	g_pUserInformation->GuildGrade = 1;
#endif
	g_RegisterWindow(this);
	
	AttrTopmost(true);
	AttrKeyboardControl(true);
	
	if(gbl_vampire_interface)
		m_guild_member_spk.Open(SPK_SLAYER_TEAM_MEMBER);
	else
		m_guild_member_spk.Open(SPK_VAMPIRE_TEAM_MEMBER);
	
	Set(400-300/2, 100, 300, 330);
	
	
	
	// set button
	m_pC_button_group = new ButtonGroup(this);
	
	const int help_x = 353, x_x = 367, button_y = 2;
	
	m_print_x[0] = 30;	//team
	m_print_x[1] = 180;	//leader
	m_print_y = 31;
	m_print_gap = 23;
	
	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(w-25, m_print_y+15, -1, m_print_gap*11-30));
	
	//CLOSE버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(196, h-35, m_guild_member_spk.GetWidth(BUTTON_CLOSE), m_guild_member_spk.GetHeight(BUTTON_CLOSE), CLOSE_ID, this, BUTTON_CLOSE));
	m_focus = -1;
	
}

C_VS_UI_TEAM_MEMBER_LIST::~C_VS_UI_TEAM_MEMBER_LIST()
{
	g_UnregisterWindow(this);
	
	DeleteNew( m_pC_button_group );
	DeleteNew( m_pC_scroll_bar);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_LIST::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_LIST::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_LIST::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_LIST::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_LIST::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_LIST::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();
	
	AttrTopmost(true);
	AttrKeyboardControl(true);
	gpC_window_manager->AppearWindow(this);
	
	gpC_base->SendMessage(UI_REQUEST_GUILD_MEMBER_LIST);
	
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_LIST::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_LIST::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_LIST::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_TEAM_MEMBER_LIST::Show()
{
	const static char* grade_string[3] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_GRADE_MASTER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_GRADE_SUB_MASTER].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_GRADE_WAIT].GetString(),		
	};
	
	gpC_global_resource->DrawDialog2(x, y, w, h, GetAttributes()->alpha);
	gpC_global_resource->DrawOutBox(x+10, y+m_print_y-2, w-40, m_print_gap*11+5);
	
	
	int i = 0;
	
	
	// 선긋기
	for(i = 1; i < 11; i++)
	{
		gpC_base->m_p_DDSurface_back->HLine(x+10, y+m_print_y+m_print_gap*i, w-40, 0);
	}	
	
	g_FL2_GetDC();
	g_PrintColorStr(x+w/2-g_GetStringWidth(g_char_slot_ingame.sz_guild_name.c_str(), gpC_base->m_info_pi.hfont), y+6, g_char_slot_ingame.sz_guild_name.c_str(), gpC_base->m_info_pi, RGB_WHITE);
	for(i = 0; i < min(m_v_member_list.size()-m_pC_scroll_bar->GetScrollPos(), 11); i++)
	{
		// 정렬 1 2 0 3
		// 0 : normal
		// 1 : master
		// 2 : sub master
		// 3 : wait
		// 4 : deny
		
		if(m_focus == i+m_pC_scroll_bar->GetScrollPos())
		{
			// 길드원 로그온인가?
		//	if(m_v_member_list[i+m_pC_scroll_bar->GetScrollPos()].bLogOn)
		//		g_PrintColorStr(x+m_print_x[0]-10, y+m_print_y+m_print_gap*i+8, "*", gpC_base->m_chatting_pi, RGB_YELLOW);
			
			g_PrintColorStr(x+m_print_x[0], y+m_print_y+m_print_gap*i+8, m_v_member_list[i+m_pC_scroll_bar->GetScrollPos()].MEMBER_NAME.c_str(), gpC_base->m_chatting_pi, RGB_YELLOW);
		}
		else
		{
		//	if(m_v_member_list[i+m_pC_scroll_bar->GetScrollPos()].bLogOn)
		//		g_PrintColorStr(x+m_print_x[0]-10, y+m_print_y+m_print_gap*i+8, "*", gpC_base->m_chatting_pi, RGB_WHITE);
			
			g_PrintColorStr(x+m_print_x[0], y+m_print_y+m_print_gap*i+8, m_v_member_list[i+m_pC_scroll_bar->GetScrollPos()].MEMBER_NAME.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);
		}
		if(m_v_member_list[i+m_pC_scroll_bar->GetScrollPos()].member_grade != 0 && m_v_member_list[i+m_pC_scroll_bar->GetScrollPos()].member_grade < 4)
			g_PrintColorStr(x+m_print_x[1], y+m_print_y+m_print_gap*i+8, grade_string[m_v_member_list[i+m_pC_scroll_bar->GetScrollPos()].member_grade-1], gpC_base->m_chatting_pi, RGB_WHITE);
	}
	g_FL2_ReleaseDC();
	
	// 길드마크 출력 부분
	CSprite *p_guild_mark;
	p_guild_mark=g_pGuildMarkManager->GetGuildMarkSmall(g_char_slot_ingame.GUILD_ID);
	if(p_guild_mark==NULL)
	{
		g_pGuildMarkManager->LoadGuildMark(g_char_slot_ingame.GUILD_ID);
		p_guild_mark=g_pGuildMarkManager->GetGuildMarkSmall(g_char_slot_ingame.GUILD_ID);			
	}
	POINT guild_mark_position={x+2,y+2};
	if(p_guild_mark!=NULL)
		gpC_base->m_p_DDSurface_back->BltSprite(&guild_mark_position,p_guild_mark);
	else
	{
		RECT rect={x+2,y+2,x+22,y+22};
		DrawAlphaBox(&rect,255,255,255,15);
	}
	
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}	
	
	//	char sz_temp[100];
	//	wsprintf(sz_temp, "%d", m_focus);
	//	g_PrintColorStr(x, y, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
	
	m_pC_scroll_bar->Show(x, y);
	
	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_LIST::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_LIST::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetFocusState())
	{
		if(p_button->GetPressState())
			m_guild_member_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
		else
			m_guild_member_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
	}
	else
		m_guild_member_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
}


//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_LIST::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_LIST::Run(id_t id)
{
	switch(id)
	{
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_TEAM_MEMBER_LIST, 0, 0, NULL);
		break;
		
	}
}


//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_LIST::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);
	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_RETURN:
			break;
			
		case VK_ESCAPE: 
			gpC_base->SendMessage(UI_CLOSE_TEAM_MEMBER_LIST);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_LIST::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_TEAM_MEMBER_LIST::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	
	_x-=x; _y-=y;
	
	m_pC_button_group->MouseControl(message, _x, _y);
	m_pC_scroll_bar->MouseControl(message, _x, _y);
	
	switch(message)
	{
	case M_MOVING:
		m_focus = -1;
		if(_x > 10 && _x < w-40&&_y>30)
		{
			m_focus = (_y - m_print_y)/m_print_gap;
			if(m_focus < 0 || m_focus > 10)
			{
				m_focus = -1;
				break;
			}
			
			m_focus += m_pC_scroll_bar->GetScrollPos();			
		}
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if(m_focus >= 0 && m_focus < m_v_member_list.size())// && (g_pUserInformation->GuildGrade == 1 || g_pUserInformation->GuildGrade == 2))
			gpC_base->SendMessage(UI_SELECT_TEAM_MEMBER_LIST, 0, 0, 
			(void *)m_v_member_list[m_focus].MEMBER_NAME.c_str());
		break;
		
	case M_LEFTBUTTON_UP:
		break;
		
	case M_WHEEL_UP:
		m_pC_scroll_bar->ScrollUp();
		EMPTY_MOVE;
		break;
		
	case M_WHEEL_DOWN:
		m_pC_scroll_bar->ScrollDown();
		EMPTY_MOVE;
		break;
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_LIST::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_TEAM_MEMBER_LIST::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_LIST::IsPixel
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_LIST::AddMemberList(const TEAM_MEMBER_LIST &member_list)
{
	int convert_table[5] = { 2, 0, 1, 3, 4 }; // 우선순위
	
	if(m_v_member_list.empty())
		m_v_member_list.push_back(member_list);
	else
	{
		for(int i = 0; i < m_v_member_list.size(); i++)
		{
			if(convert_table[member_list.member_grade] < convert_table[m_v_member_list[i].member_grade])
			{
				m_v_member_list.insert(&m_v_member_list[i], member_list);
				break;
			}
		}
		if(i == m_v_member_list.size())
			m_v_member_list.push_back(member_list);
	}
	
	// 스크롤바 세팅해주기
	m_pC_scroll_bar->SetPosMax(max(0, m_v_member_list.size() - 11 +1));
	m_pC_scroll_bar->SetScrollPos(0);
	
	///////////////////////////////////////////////////////////////
	//
	// 마스터, 서브마스터, 회원, 대기자 순으로 정렬하는 테이블 생성
	// 좀 무식하지만-_-;; 
	// 1 2 0 3	
	// focus 참조할때나 순서 대로 출력할때 참조
	//
}



class CTeamTeamLess
{
public:
	bool operator()(const C_VS_UI_TEAM_LIST::TEAM_LIST_BASE& p, const C_VS_UI_TEAM_LIST::TEAM_LIST_BASE& q) const
	{
		return strcmp(p.TEAM_NAME.c_str(), q.TEAM_NAME.c_str())>0;
	}
};

class CTeamTeamGreater
{
public:
	bool operator()(const C_VS_UI_TEAM_LIST::TEAM_LIST_BASE& p, const C_VS_UI_TEAM_LIST::TEAM_LIST_BASE& q) const
	{
		int re = strcmp(p.TEAM_NAME.c_str(), q.TEAM_NAME.c_str());
		return strcmp(p.TEAM_NAME.c_str(), q.TEAM_NAME.c_str())<0;
	}
};

class CTeamLeaderLess
{
public:
	bool operator()(const C_VS_UI_TEAM_LIST::TEAM_LIST_BASE& p, const C_VS_UI_TEAM_LIST::TEAM_LIST_BASE& q) const
	{
		return strcmp(p.LEADER_NAME.c_str(), q.LEADER_NAME.c_str())>0;
	}
};

class CTeamLeaderGreater
{
public:
	bool operator()(const C_VS_UI_TEAM_LIST::TEAM_LIST_BASE& p, const C_VS_UI_TEAM_LIST::TEAM_LIST_BASE& q) const
	{
		return strcmp(p.LEADER_NAME.c_str(), q.LEADER_NAME.c_str())<0;
	}
};

class CReadyTeamExpireLess
{
public:
	bool operator()(const C_VS_UI_TEAM_LIST::READY_TEAM_LIST& p, const C_VS_UI_TEAM_LIST::READY_TEAM_LIST& q) const
	{
		return strcmp(p.EXPIRE_DATE.c_str(), q.EXPIRE_DATE.c_str())>0;
	}
};

class CReadyTeamExpireGreater
{
public:
	bool operator()(const C_VS_UI_TEAM_LIST::READY_TEAM_LIST& p, const C_VS_UI_TEAM_LIST::READY_TEAM_LIST& q) const
	{
		return strcmp(p.EXPIRE_DATE.c_str(), q.EXPIRE_DATE.c_str())<0;
	}
};

class CTeamMembersLess
{
public:
	bool operator()(const C_VS_UI_TEAM_LIST::TEAM_LIST_BASE& p, const C_VS_UI_TEAM_LIST::TEAM_LIST_BASE& q) const
	{
		return p.MEMBERS > q.MEMBERS;
	}
};

class CTeamMembersGreater
{
public:
	bool operator()(const C_VS_UI_TEAM_LIST::TEAM_LIST_BASE& p, const C_VS_UI_TEAM_LIST::TEAM_LIST_BASE& q) const
	{
		return p.MEMBERS < q.MEMBERS;
	}
};

class CTeamRankingLess
{
public:
	bool operator()(const C_VS_UI_TEAM_LIST::REGIST_TEAM_LIST& p, const C_VS_UI_TEAM_LIST::REGIST_TEAM_LIST& q) const
	{
		return p.RANKING > q.RANKING;
	}
};

class CTeamRankingGreater
{
public:
	bool operator()(const C_VS_UI_TEAM_LIST::REGIST_TEAM_LIST& p, const C_VS_UI_TEAM_LIST::REGIST_TEAM_LIST& q) const
	{
		return p.RANKING < q.RANKING;
	}
};

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_LIST::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_LIST::Run(id_t id)
{
	switch (id)
	{
	case X_ID:
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_TEAM_LIST);
		break;
		
	case SCROLL_UP_ID:
		if(m_scroll > 0)
			m_scroll--;
		break;
		
	case SCROLL_DOWN_ID:
		if(m_bl_ready)
		{
			if(m_v_ready_team_search_list.size())
			{
				if(m_scroll+9 < m_v_ready_team_search_list.size())
					m_scroll++;
			}
			else
				if(m_scroll+9 < m_v_ready_team_list.size())
					m_scroll++;
		}
		else
		{
			if(m_v_regist_team_search_list.size())
			{
				if(m_scroll+9 < m_v_regist_team_search_list.size())
					m_scroll++;
			}
			else
				if(m_scroll+9 < m_v_regist_team_list.size())
					m_scroll++;
		}
		break;
		
	case TEAM_LIST_ID:
		m_scroll = 0;
		if(m_bl_ready)
		{
			m_v_ready_team_search_list.clear();
		}
		else
		{
			m_v_regist_team_search_list.clear();
		}
		break;
		
	case INDEX_TEAM_ID:
		m_scroll = 0;
		if(m_bl_ready)
		{
			if(m_v_ready_team_search_list.empty())
			{
				if(m_last_sort_type == INDEX_TEAM_ID)
				{
					std::sort(m_v_ready_team_list.begin(), m_v_ready_team_list.end(), CTeamTeamLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_ready_team_list.begin(), m_v_ready_team_list.end(), CTeamTeamGreater());
					m_last_sort_type = INDEX_TEAM_ID;
				}
			}
			else
			{
				if(m_last_sort_type == INDEX_TEAM_ID)
				{
					std::sort(m_v_ready_team_search_list.begin(), m_v_ready_team_search_list.end(), CTeamTeamLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_ready_team_search_list.begin(), m_v_ready_team_search_list.end(), CTeamTeamGreater());
					m_last_sort_type = INDEX_TEAM_ID;
				}
			}
		}
		else
		{
			if(m_v_regist_team_search_list.empty())
			{
				if(m_last_sort_type == INDEX_TEAM_ID)
				{
					std::sort(m_v_regist_team_list.begin(), m_v_regist_team_list.end(), CTeamTeamLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_regist_team_list.begin(), m_v_regist_team_list.end(), CTeamTeamGreater());
					m_last_sort_type = INDEX_TEAM_ID;
				}
			}
			else
			{
				if(m_last_sort_type == INDEX_TEAM_ID)
				{
					std::sort(m_v_regist_team_search_list.begin(), m_v_regist_team_search_list.end(), CTeamTeamLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_regist_team_search_list.begin(), m_v_regist_team_search_list.end(), CTeamTeamGreater());
					m_last_sort_type = INDEX_TEAM_ID;
				}
			}
		}
		break;
		
	case INDEX_LEADER_ID:
		m_scroll = 0;
		if(m_bl_ready)
		{
			if(m_v_ready_team_search_list.empty())
			{
				if(m_last_sort_type == INDEX_LEADER_ID)
				{
					std::sort(m_v_ready_team_list.begin(), m_v_ready_team_list.end(), CTeamLeaderLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_ready_team_list.begin(), m_v_ready_team_list.end(), CTeamLeaderGreater());
					m_last_sort_type = INDEX_LEADER_ID;
				}
			}
			else
			{
				if(m_last_sort_type == INDEX_LEADER_ID)
				{
					std::sort(m_v_ready_team_search_list.begin(), m_v_ready_team_search_list.end(), CTeamLeaderLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_ready_team_search_list.begin(), m_v_ready_team_search_list.end(), CTeamLeaderGreater());
					m_last_sort_type = INDEX_LEADER_ID;
				}
			}
		}
		else
		{
			if(m_v_regist_team_search_list.empty())
			{
				if(m_last_sort_type == INDEX_LEADER_ID)
				{
					std::sort(m_v_regist_team_list.begin(), m_v_regist_team_list.end(), CTeamLeaderLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_regist_team_list.begin(), m_v_regist_team_list.end(), CTeamLeaderGreater());
					m_last_sort_type = INDEX_LEADER_ID;
				}
			}
			else
			{
				if(m_last_sort_type == INDEX_LEADER_ID)
				{
					std::sort(m_v_regist_team_search_list.begin(), m_v_regist_team_search_list.end(), CTeamLeaderLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_regist_team_search_list.begin(), m_v_regist_team_search_list.end(), CTeamLeaderGreater());
					m_last_sort_type = INDEX_LEADER_ID;
				}
			}
		}
		break;
		
		//	case INDEX_EXPIREDATE_ID:
		//		m_scroll = 0;
		//		if(m_bl_ready)
		//		{
		//			if(m_v_ready_team_search_list.empty())
		//			{
		//				if(m_last_sort_type == INDEX_EXPIREDATE_ID)
		//				{
		//					std::sort(m_v_ready_team_list.begin(), m_v_ready_team_list.end(), CReadyTeamExpireLess());
		//					m_last_sort_type = 0;
		//				}
		//				else
		//				{
		//					std::sort(m_v_ready_team_list.begin(), m_v_ready_team_list.end(), CReadyTeamExpireGreater());
		//					m_last_sort_type = INDEX_EXPIREDATE_ID;
		//				}
		//			}
		//			else
		//			{
		//				if(m_last_sort_type == INDEX_EXPIREDATE_ID)
		//				{
		//					std::sort(m_v_ready_team_search_list.begin(), m_v_ready_team_search_list.end(), CReadyTeamExpireLess());
		//					m_last_sort_type = 0;
		//				}
		//				else
		//				{
		//					std::sort(m_v_ready_team_search_list.begin(), m_v_ready_team_search_list.end(), CReadyTeamExpireGreater());
		//					m_last_sort_type = INDEX_EXPIREDATE_ID;
		//				}
		//			}
		//		}
		//		break;
		
	case INDEX_MEMBERS_ID:
		m_scroll = 0;
		if(m_bl_ready)
		{
			if(m_v_ready_team_search_list.empty())
			{
				if(m_last_sort_type == INDEX_MEMBERS_ID)
				{
					std::sort(m_v_ready_team_list.begin(), m_v_ready_team_list.end(), CTeamMembersLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_ready_team_list.begin(), m_v_ready_team_list.end(), CTeamMembersGreater());
					m_last_sort_type = INDEX_MEMBERS_ID;
				}
			}
			else
			{
				if(m_last_sort_type == INDEX_MEMBERS_ID)
				{
					std::sort(m_v_ready_team_search_list.begin(), m_v_ready_team_search_list.end(), CTeamMembersLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_ready_team_search_list.begin(), m_v_ready_team_search_list.end(), CTeamMembersGreater());
					m_last_sort_type = INDEX_MEMBERS_ID;
				}
			}
		}
		else
		{
			if(m_v_regist_team_search_list.empty())
			{
				if(m_last_sort_type == INDEX_MEMBERS_ID)
				{
					std::sort(m_v_regist_team_list.begin(), m_v_regist_team_list.end(), CTeamMembersLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_regist_team_list.begin(), m_v_regist_team_list.end(), CTeamMembersGreater());
					m_last_sort_type = INDEX_MEMBERS_ID;
				}
			}
			else
			{
				if(m_last_sort_type == INDEX_MEMBERS_ID)
				{
					std::sort(m_v_regist_team_search_list.begin(), m_v_regist_team_search_list.end(), CTeamMembersLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_regist_team_search_list.begin(), m_v_regist_team_search_list.end(), CTeamMembersGreater());
					m_last_sort_type = INDEX_MEMBERS_ID;
				}
			}
		}
		break;
		
	case INDEX_RANKING_ID:
		m_scroll = 0;
		if(m_bl_ready)
		{
		}
		else
		{
			if(m_v_regist_team_search_list.empty())
			{
				if(m_last_sort_type == INDEX_RANKING_ID)
				{
					std::sort(m_v_regist_team_list.begin(), m_v_regist_team_list.end(), CTeamRankingLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_regist_team_list.begin(), m_v_regist_team_list.end(), CTeamRankingGreater());
					m_last_sort_type = INDEX_RANKING_ID;
				}
			}
			else
			{
				if(m_last_sort_type == INDEX_RANKING_ID)
				{
					std::sort(m_v_regist_team_search_list.begin(), m_v_regist_team_search_list.end(), CTeamRankingLess());
					m_last_sort_type = 0;
				}
				else
				{
					std::sort(m_v_regist_team_search_list.begin(), m_v_regist_team_search_list.end(), CTeamRankingGreater());
					m_last_sort_type = INDEX_RANKING_ID;
				}
			}
		}
		break;
		
	case SEARCH_TYPE_ID:
		if(m_search_type >= SEARCH_ALL)
			m_search_type = SEARCH_NAME;
		else
			m_search_type += 3;
		break;
		
	case SEARCH_ID:
		{
			//어쩌구 저쩌구
			m_scroll = 0;
			char * p_temp = NULL;
			g_Convert_DBCS_Ascii2SingleByte(m_lev_search.GetString(), m_lev_search.Size(), p_temp);
			
			if(p_temp == NULL)
			{
				Run(TEAM_LIST_ID);
				return;
			}
			
			if(m_bl_ready)
			{
				m_v_ready_team_search_list.clear();
				std::vector<READY_TEAM_LIST>::iterator itr = m_v_ready_team_list.begin();
				
				while(itr != m_v_ready_team_list.end())
				{
					switch(m_search_type)
					{
					case SEARCH_NAME:
						if(strstr(itr->TEAM_NAME.c_str(), p_temp))
							m_v_ready_team_search_list.push_back(*itr);
						break;
						
					case SEARCH_LEADER:
						if(strstr(itr->LEADER_NAME.c_str(), p_temp))
							m_v_ready_team_search_list.push_back(*itr);
						break;
						
					case SEARCH_ALL:
						// 대기중인 팀 리스트는 Members, Ranking 이 없다.
						if(strstr(itr->TEAM_NAME.c_str(), p_temp) || strstr(itr->LEADER_NAME.c_str(), p_temp))
							m_v_ready_team_search_list.push_back(*itr);
						
						break;
					}
					itr++;
				}
			}
			else
			{
				m_v_regist_team_search_list.clear();
				std::vector<REGIST_TEAM_LIST>::iterator itr = m_v_regist_team_list.begin();
				int numTemp;
				numTemp=atoi(p_temp);
				
				while(itr != m_v_regist_team_list.end())
				{
					switch(m_search_type)
					{
					case SEARCH_NAME:
						if(strstr(itr->TEAM_NAME.c_str(), p_temp))
							m_v_regist_team_search_list.push_back(*itr);
						break;
						
					case SEARCH_LEADER:
						if(strstr(itr->LEADER_NAME.c_str(), p_temp))
							m_v_regist_team_search_list.push_back(*itr);
						break;
						
					case SEARCH_ALL:					
						if(strstr(itr->TEAM_NAME.c_str(), p_temp) || strstr(itr->LEADER_NAME.c_str(), p_temp))
							m_v_regist_team_search_list.push_back(*itr);
						else 
							if(numTemp)
							{
								if(itr->MEMBERS == numTemp || itr->RANKING == numTemp)
									m_v_regist_team_search_list.push_back(*itr);
							}
							break;
					}
					itr++;
				}
			}
			
			DeleteNewArray(p_temp);
			
			if(m_v_ready_team_search_list.empty() && m_v_regist_team_search_list.empty())
			{
				gC_vs_ui.RunNoSearchResult();
			}
			
			m_lev_search.EraseAll();
		}
		break;
		
	case HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::GUILD);
		break;
		
	}
}

void	C_VS_UI_TEAM_LIST::AddReadyTeamList(const C_VS_UI_TEAM_LIST::READY_TEAM_LIST &team_info)
{
	m_v_ready_team_list.push_back(team_info);
}

void	C_VS_UI_TEAM_LIST::AddRegistTeamList(const C_VS_UI_TEAM_LIST::REGIST_TEAM_LIST &team_info)
{
	m_v_regist_team_list.push_back(team_info);
}

///----------------------------------------------------------------------------
C_VS_UI_TEAM_INFO::C_VS_UI_TEAM_INFO(bool ready, void *info)
{
	m_bl_ready = ready;
	m_master = false;
	
	g_RegisterWindow(this);
	
	AttrTopmost(true);
	AttrKeyboardControl(true);
	
	if(gbl_vampire_interface)
		m_guild_spk.Open(SPK_VAMPIRE_TEAM);
	else
		m_guild_spk.Open(SPK_SLAYER_TEAM);
	
	Set(RESOLUTION_X/2-m_guild_spk.GetWidth(INFO_WINDOW)/2, RESOLUTION_Y/2 - m_guild_spk.GetHeight(INFO_WINDOW)/2, m_guild_spk.GetWidth(INFO_WINDOW), m_guild_spk.GetHeight(INFO_WINDOW));
	
	
	// set button
	m_pC_button_group = new ButtonGroup(this);
	
	const int help_x = 257, x_x = 257+14, button_y = 6;
	const int scroll_x = w-28, scroll_up_y = 96-20, scroll_down_y = 312-20;
	
	const int scroll_tag_x = x+w-28, scroll_tag_y = y+108-20, scroll_tag_height = 198;
	
	
	
	//help, x버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + help_x, y + button_y, m_guild_spk.GetWidth(HELP_BUTTON), m_guild_spk.GetHeight(HELP_BUTTON), HELP_ID, this, HELP_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + x_x, y + button_y, m_guild_spk.GetWidth(X_BUTTON), m_guild_spk.GetHeight(X_BUTTON), X_ID, this, X_BUTTON));
	
	//scroll up, down 버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + scroll_x, y + scroll_up_y, m_guild_spk.GetWidth(SCROLL_UP_BUTTON), m_guild_spk.GetHeight(SCROLL_UP_BUTTON), SCROLL_UP_ID, this, SCROLL_UP_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + scroll_x, y + scroll_down_y, m_guild_spk.GetWidth(SCROLL_DOWN_BUTTON), m_guild_spk.GetHeight(SCROLL_DOWN_BUTTON), SCROLL_DOWN_ID, this, SCROLL_DOWN_BUTTON));
	
	
	m_scroll = 0;
	m_scroll_max = 5;
	m_bl_scrolling = false;

	if(m_bl_ready)
	{
		m_ready_info = *(READY_TEAM_INFO *)info;
		
	}
	else
	{
		m_regist_info = *(REGIST_TEAM_INFO *)info;
	}
	
	if(g_pUserInformation->GuildName.GetLength() > 0)		// 팀에 등록 되어 있으면
	{
		const char *team_name = m_bl_ready ? m_ready_info.TEAM_NAME.c_str() : m_regist_info.TEAM_NAME.c_str();
		const char *leader_name = m_bl_ready ? m_ready_info.LEADER_NAME.c_str() : m_regist_info.LEADER_NAME.c_str();

		if(team_name !=NULL )
		{
			if(!strcmp(team_name,g_pUserInformation->GuildName.GetString()))
			{
				if(g_pUserInformation->GuildGrade == 1)// && !strcmp(leader_name,g_pUserInformation->)
					// 일단 등록된 팀으로 한정해두자.
					if(!m_bl_ready)
					{
						m_master = true;
						// Modify 버튼. 길드 이름이 같으면서 등급이 0이면 
					}
			}
		}
		if(m_master)
			m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+w-2,y+h-49-30,m_guild_spk.GetWidth(MODIFY_BUTTON), m_guild_spk.GetHeight(MODIFY_BUTTON),MODIFY_ID, this, MODIFY_BUTTON));
	} 
	else		// 어떠한 팀에도 등록되어 있지 않으면.
		if(g_pUserInformation->GuildName.GetLength() == 0)
			m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+w-2, y+h-49-30, m_guild_spk.GetWidth(JOIN_BUTTON), m_guild_spk.GetHeight(JOIN_BUTTON), JOIN_ID, this, JOIN_BUTTON));

	
	//JOIN, CLOSE버튼	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+w-2, y+h-49, m_guild_spk.GetWidth(CLOSE_BUTTON), m_guild_spk.GetHeight(CLOSE_BUTTON), CLOSE_ID, this, CLOSE_BUTTON));
	
	if(!m_bl_ready)
	{
		int m_print_x = x+30;
		int m_print_y = y+60;
		int m_print_gap = 20;
		
		m_lev_intro.SetPosition(m_print_x, m_print_y+m_print_gap*5);
		//m_lev_intro.SetEditorMode(m_print_gap, 9);
		m_lev_intro.SetEditorMode(20, 10);
		m_lev_intro.SetPrintInfo(gpC_base->m_chatting_pi);
		m_lev_intro.SetByteLimit(30*10);
		m_lev_intro.SetAbsWidth(w - (m_print_x - x)-30);
		m_lev_intro.SetInputStringColor(RGB_BLACK);
		m_lev_intro.SetCursorColor(RGB_BLACK);
		m_lev_intro.AddString(m_regist_info.INTRODUCTION.c_str());
		
		Attach(&m_lev_intro);
	}	
}

C_VS_UI_TEAM_INFO::~C_VS_UI_TEAM_INFO()
{
	g_UnregisterWindow(this);

	m_lev_intro.Unacquire();
	gC_vs_ui.AcquireChatting();
	
	DeleteNew( m_pC_button_group );
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_INFO::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_INFO::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_INFO::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_INFO::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_INFO::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_INFO::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();
	
	if(m_master)
		m_lev_intro.Acquire();

	AttrTopmost(true);

	gpC_window_manager->AppearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_INFO::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_INFO::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_INFO::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_TEAM_INFO::Show()
{
	POINT guild_point = { x+29, y+15 };
	
	const int scroll_tag_x = x+w-28, scroll_tag_y = y+108-20, scroll_tag_height = 198;
	const int print_x = x+30, print_y = y+60, print_gap = 20, line_gap = 12;
	
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_guild_spk.BltLocked(x, y, INFO_WINDOW);		
		m_guild_spk.BltLocked(x+w/2-m_guild_spk.GetWidth(TITLE_TEAM_INFO)/2, y+20, TITLE_TEAM_INFO);		
		m_guild_spk.BltLocked(x+w-30, print_y-6+print_gap*1, SCROLL_BAR);
		if(m_bl_scrolling)
			m_guild_spk.BltLocked(scroll_tag_x, min(max(gpC_mouse_pointer->GetY(), scroll_tag_y), scroll_tag_y+scroll_tag_height), SCROLL_TAG);
		else
			m_guild_spk.BltLocked(scroll_tag_x, scroll_tag_y+scroll_tag_height*m_scroll/(m_scroll_max), SCROLL_TAG);
		
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	char sz_string[512];
	int py = print_y, vx, i;
	
	if(m_bl_ready)
	{	
		//		if(m_scroll < 3)
		//		{
		//			wsprintf(sz_string, "Registered : %s", m_ready_info.REGISTERD_DATE.c_str());
		//			g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		//			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
		//			py += print_gap;
		//		}
		
		//		if(m_scroll < 3)
		//		{
		//			wsprintf(sz_string, "Expired : %s", m_ready_info.EXPIRE_DATE.c_str());
		//			g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		//			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
		//			py += print_gap;
		//		}
		
		if(m_scroll < 1)
		{
			wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_NAME].GetString(), m_ready_info.TEAM_NAME.c_str());
			g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
			py += print_gap;
		}
		
		if(m_scroll < 2)
		{
			wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_LEADER].GetString(), m_ready_info.LEADER_NAME.c_str());
			g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
			py += print_gap;
		}
		
		if(m_scroll < 3)
		{
			// 숫자사이에 ,넣기
			wsprintf(sz_string, "%d", m_ready_info.REG_FEE);
			std::string sstr = sz_string;
			for(i = 3; i <= 13; i += 4)
				if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
				
				wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_REG_FEE].GetString(), sstr.c_str());
				g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
				gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
				py += print_gap;
		}
		
		wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_MEMBERS].GetString());
		vx = g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		const int old_vx = vx;
		bool b_member_num = false;
		
		gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
		for(i = 0; i < m_ready_info.MEMBERS_NAME.size(); i++)
		{
			if(g_GetStringWidth(m_ready_info.MEMBERS_NAME[i].c_str(), gpC_base->m_chatting_pi.hfont)+vx > x+w-30)
			{
				vx = old_vx;
				py += print_gap;
				gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
				
				wsprintf(sz_string, "(%d/%d)", m_ready_info.MEMBERS, m_ready_info.MEMBERS_MAX);
				g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
				b_member_num = true;
			}
			vx = g_PrintColorStr(vx, py, m_ready_info.MEMBERS_NAME[i].c_str(), gpC_base->m_chatting_pi, RGB_BLACK);
			if(i != m_ready_info.MEMBERS_NAME.size()-1)
				vx = g_PrintColorStr(vx, py, ", ", gpC_base->m_chatting_pi, RGB_BLACK);
		}
		
		if(b_member_num == false)
		{
			wsprintf(sz_string, "(%d/%d)", m_ready_info.MEMBERS, m_ready_info.MEMBERS_MAX);
			if(g_GetStringWidth(sz_string, gpC_base->m_chatting_pi.hfont)+vx > x+w-30)
			{
				py+=print_gap;
				gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
				g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
			}
			else
			{
				g_PrintColorStr(vx, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
			}
		}
		py += print_gap;
		
		int next = 0;
		
		wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_INTRODUCTION].GetString());//, m_ready_info.INTRODUCTION.c_str());
		vx = g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
		
		const int char_width = g_GetStringWidth("a", gpC_base->m_chatting_pi.hfont);
		
		while(m_ready_info.INTRODUCTION.size()>next && py < y+h-print_gap*2)
		{
			strcpy(sz_string, m_ready_info.INTRODUCTION.c_str()+next);
			
			char *sz_string2 = sz_string;
			
			while(*sz_string2 == ' ')		// 앞의 공백제거
			{
				sz_string2++;
				next++;
			}
			
			int cut_pos = (x+w-30 -vx)/char_width;
			if(!g_PossibleStringCut(sz_string2, cut_pos))
				cut_pos--;
			sz_string2[cut_pos] = NULL;
			
			char *return_char = NULL;
			if((return_char = strchr(sz_string2, '\n')) != NULL)	// return 처리
			{
				cut_pos = return_char - sz_string2+1;
				sz_string2[cut_pos-1] = NULL;
			}
			
			g_PrintColorStr(vx, py, sz_string2, gpC_base->m_chatting_pi, RGB_BLACK);
			next += cut_pos;
			vx = print_x;
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
			py += print_gap;
		}
		
		if(!strlen(m_ready_info.INTRODUCTION.c_str()))
		{
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
			py += print_gap;
		}
		
		while(py < y+h-print_gap*2)
		{
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
			py += print_gap;
		}
		
		// 대기팀은 길드마크 안찍음
		//		RECT rect={guild_point.x,guild_point.y,guild_point.x+40,guild_point.y+40};
		//		SetRect(&rect, guild_point.x-2,guild_point.y-2,guild_point.x+42,guild_point.y+42);
		//		DrawAlphaBox(&rect, 255, 255, 255, 15);
		//		gpC_global_resource->DrawOutBox(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top);
	}
	else	// regist team
	{				
		if(m_scroll < 1)
		{
			wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_NAME].GetString(), m_regist_info.TEAM_NAME.c_str());
			g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
			py += print_gap;
		}
		
		if(m_scroll < 2)
		{
			wsprintf(sz_string,(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_LEADER].GetString(), m_regist_info.LEADER_NAME.c_str());
			g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
			py += print_gap;
		}
		
		/*		if(m_scroll < 3)
		{
		wsprintf(sz_string, "Registered : %s", m_regist_info.REGISTERED_DATE.c_str());
		g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
		py += print_gap;
		}
		*/		
		//		if(m_scroll < 3)
		//		{
		//			// 숫자사이에 ,넣기
		//			wsprintf(sz_string, "%d", m_regist_info.REG_FEE);
		//			std::string sstr = sz_string;
		//			for(int i = 3; i <= 13; i += 4)
		//				if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
		//				
		//			wsprintf(sz_string, "Reg. Fee : $%s", sstr.c_str());
		//			g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		//			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
		//			py += print_gap;
		//		}
		if(m_scroll < 3)
		{
			wsprintf(sz_string, "%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_MEMBERS].GetString(), m_regist_info.MEMBERS);
			g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
			py += print_gap;
		}
		
		if(m_scroll < 4)
		{
			wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_RANKING].GetString(), m_regist_info.RANKING);
			g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
			py += print_gap;
		}
		
		int next = 0;
		
		if(m_scroll < 5)
		{
			wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_INTRODUCTION].GetString());//, m_ready_info.INTRODUCTION.c_str());
			vx = g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
		}
			
		const int char_width = g_GetStringWidth("a", gpC_base->m_chatting_pi.hfont);

		/*char *sz_intro;		
		g_Convert_DBCS_Ascii2SingleByte(m_lev_intro.GetString(), m_lev_intro.Size(), sz_intro);
		m_regist_info.INTRODUCTION = sz_intro;
		DeleteNew(sz_intro);
		while(m_regist_info.INTRODUCTION.size()>next && py < y+h-print_gap*2)
		{
			strcpy(sz_string, m_regist_info.INTRODUCTION.c_str()+next);
			
			char *sz_string2 = sz_string;
			
			while(*sz_string2 == ' ')		// 앞의 공백제거
			{
				sz_string2++;
				next++;
			}
			
			int cut_pos = (x+w-30 -vx)/char_width;
			if(!g_PossibleStringCut(sz_string2, cut_pos))
				cut_pos--;			
			sz_string2[cut_pos] = NULL;
			
			char *return_char = NULL;
			if((return_char = strchr(sz_string2, '\n')) != NULL)	// return 처리
			{
				cut_pos = return_char - sz_string2+1;
				sz_string2[cut_pos-1] = NULL;
			}
			
			g_PrintColorStr(vx, py, sz_string2, gpC_base->m_chatting_pi, RGB_BLACK);
			next += cut_pos;
			vx = print_x;
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
			py += print_gap;
		}*/
		m_lev_intro.SetPosition(x+30, y+60+20*(5-m_scroll));
		m_lev_intro.SetEditorMode(20,8+m_scroll);
		m_lev_intro.Show();
		
		
		if(!strlen(m_regist_info.INTRODUCTION.c_str()))
		{
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
			py += print_gap;
		}
		
		while(py < y+h-print_gap*2)
		{
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
			py += print_gap;
		}

		if(gbl_vampire_interface)
			gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
		
		CSprite *p_guild_mark;
		p_guild_mark=g_pGuildMarkManager->GetGuildMark(m_regist_info.guild_id);
		if(p_guild_mark==NULL)
		{
			g_pGuildMarkManager->LoadGuildMark(m_regist_info.guild_id);
			p_guild_mark=g_pGuildMarkManager->GetGuildMark(m_regist_info.guild_id);			
		}		
		
		if(p_guild_mark!=NULL)
		{
			RECT rect;
			SetRect(&rect, guild_point.x-2,guild_point.y-2,guild_point.x+42,guild_point.y+42);
			//			DrawAlphaBox(&rect, 0, 0, 0, 20);
			gpC_global_resource->DrawOutBox(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top);
			gpC_base->m_p_DDSurface_back->BltSprite(&guild_point, p_guild_mark);
		}
	}
	m_pC_button_group->ShowDescription();
	
	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_INFO::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_INFO::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char*	m_help_string[EXEC_MAX] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_JOIN].GetString(),
			
		(*g_pGameStringTable)[UI_STRING_MESSAGE_UP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DOWN].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MODIFY_INFO].GetString(),
	};
	
	if (!m_bl_scrolling)
		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}


//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_INFO::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_INFO::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);
	
	//	ShowButtonDescription(p_button);
	
	switch(p_button->GetID())
	{
	case X_ID:
	case HELP_ID:
	case SCROLL_UP_ID:
	case SCROLL_DOWN_ID:
		if(p_button->GetFocusState() && p_button->GetPressState())
			m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index+1);
		else
			m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index);
		break;
		
	case MODIFY_ID :
	case JOIN_ID:
	case CLOSE_ID:
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_guild_spk.BltLocked(p_button->x, p_button->y+2, p_button->m_image_index+1);
			else
				m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index+1);
		}
		else
			m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index);
		break;
	}
	
}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_INFO::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);
	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE: 
			gpC_base->SendMessage(UI_CLOSE_TEAM_INFO);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_INFO::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_TEAM_INFO::MouseControl(UINT message, int _x, int _y)
{
	if(!m_lev_intro.IsAcquire()&&m_master)
		m_lev_intro.Acquire();

	const int scroll_tag_x = x+w-28, scroll_tag_y = y+108-20, scroll_tag_height = 198;
	
	Window::MouseControl(message, _x, _y);
	
	switch(message)
	{
	case M_MOVING:
		if(m_bl_scrolling)
		{
			m_scroll = min(m_scroll_max, (m_scroll_max)*(max(_y, scroll_tag_y) - scroll_tag_y + scroll_tag_height/(m_scroll_max)/2)/scroll_tag_height);
		}
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if(_x >= scroll_tag_x && _x <= scroll_tag_x+m_guild_spk.GetWidth(SCROLL_BAR) && _y >= scroll_tag_y && _y <= scroll_tag_y+scroll_tag_height)
		{
			//			if(m_v_ready_team_list.size() > 9)	// 스크롤이 생기는 경우에만
			{
				m_bl_scrolling = true;
				m_scroll = (m_scroll_max)*(_y - scroll_tag_y + scroll_tag_height/(m_scroll_max)/2)/scroll_tag_height;
			}
		}
		break;
		
	case M_LEFTBUTTON_UP:
		if(m_bl_scrolling)
		{
			m_bl_scrolling = false;
		}
		break;
		
	case M_WHEEL_UP:
		Run(SCROLL_UP_ID);
		break;
		
	case M_WHEEL_DOWN:
		Run(SCROLL_DOWN_ID);
		break;
	}
	
	m_pC_button_group->MouseControl(message, _x, _y);
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_INFO::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_TEAM_INFO::IsPixel(int _x, int _y)
{
	bool re = m_guild_spk.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), INFO_WINDOW);
	
	if(re == false)
	{
		if(m_guild_spk.IsPixel(_x - (x+w-2), _y - (y+h-49), CLOSE_BUTTON) ||
			m_guild_spk.IsPixel(_x - (x+w-2), _y - (y+h-49-30), JOIN_BUTTON))
			re = true;
	}
	
	return re;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_INFO::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_INFO::Run(id_t id)
{
	switch (id)
	{
	case X_ID:
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_TEAM_INFO);
		break;
		
	case SCROLL_UP_ID:
		if(m_scroll > 0)
			m_scroll--;
		break;
		
	case SCROLL_DOWN_ID:
		if(m_scroll < m_scroll_max)
			m_scroll++;
		break;
		
	case JOIN_ID:
		if(m_bl_ready)
		{
			gpC_base->SendMessage(UI_JOIN_READY_TEAM, m_ready_info.guild_id, 0, (void *)m_ready_info.TEAM_NAME.c_str());
		}
		else
		{
			gpC_base->SendMessage(UI_JOIN_REGIST_TEAM, m_regist_info.guild_id, 0, (void *)m_regist_info.TEAM_NAME.c_str());
		}
		break;
		
	case HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::GUILD);
		break;
	
	case MODIFY_ID :		
		{
			char *sz_intro = NULL;

			g_Convert_DBCS_Ascii2SingleByte(m_lev_intro.GetString(), m_lev_intro.Size(), sz_intro);

			if(sz_intro != NULL)
			{
				m_regist_info.INTRODUCTION = sz_intro;
				
				gpC_base->SendMessage(UI_MODIFY_TEAM_INFO, m_regist_info.guild_id, 0, (void *)m_regist_info.INTRODUCTION.c_str());								
				DeleteNew(sz_intro);
			}			
		}
		break;
	}
}

//----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_INFO::C_VS_UI_TEAM_MEMBER_INFO
// 
//
//----------------------------------------------------------------------------
C_VS_UI_TEAM_MEMBER_INFO::C_VS_UI_TEAM_MEMBER_INFO(MEMBER_INFO *info)
{
	m_own_info = false;
	g_RegisterWindow(this);
	
	AttrTopmost(true);
	AttrKeyboardControl(true);
	
	if(gbl_vampire_interface)
		m_guild_member_spk.Open(SPK_VAMPIRE_TEAM);
	else
		m_guild_member_spk.Open(SPK_SLAYER_TEAM);
	
	Set(RESOLUTION_X/2-m_guild_member_spk.GetWidth(INFO_WINDOW)/2, RESOLUTION_Y/2 - m_guild_member_spk.GetHeight(INFO_WINDOW)/2, m_guild_member_spk.GetWidth(INFO_WINDOW), m_guild_member_spk.GetHeight(INFO_WINDOW));
	
	
	// set button
	m_pC_button_group = new ButtonGroup(this);
	
	const int help_x = 257, x_x = 257+14, button_y = 6;
	const int scroll_x = w-28, scroll_up_y = 96-20, scroll_down_y = 312-20;
	
	const int scroll_tag_x = x+w-28, scroll_tag_y = y+108-20, scroll_tag_height = 198;
	
	
	
	//help, x버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + help_x, y + button_y, m_guild_member_spk.GetWidth(HELP_BUTTON), m_guild_member_spk.GetHeight(HELP_BUTTON), HELP_ID, this, HELP_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + x_x, y + button_y, m_guild_member_spk.GetWidth(X_BUTTON), m_guild_member_spk.GetHeight(X_BUTTON), X_ID, this, X_BUTTON));
	
	//scroll up, down 버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + scroll_x, y + scroll_up_y, m_guild_member_spk.GetWidth(SCROLL_UP_BUTTON), m_guild_member_spk.GetHeight(SCROLL_UP_BUTTON), SCROLL_UP_ID, this, SCROLL_UP_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + scroll_x, y + scroll_down_y, m_guild_member_spk.GetWidth(SCROLL_DOWN_BUTTON), m_guild_member_spk.GetHeight(SCROLL_DOWN_BUTTON), SCROLL_DOWN_ID, this, SCROLL_DOWN_BUTTON));
	
	//JOIN, CLOSE버튼
	//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+w-2, y+h-49-30, m_guild_member_spk.GetWidth(JOIN_BUTTON), m_guild_member_spk.GetHeight(JOIN_BUTTON), JOIN_ID, this, JOIN_BUTTON));
	// 내가 길마이면
	if(g_pUserInformation->GuildGrade == 1 || g_pUserInformation->GuildGrade == 2)
	{
		if(info->GRADE == 3)	// 대기자인경우
		{
			m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+w-2, y+h-49-60, m_guild_member_spk.GetWidth(ACCEPT_BUTTON), m_guild_member_spk.GetHeight(ACCEPT_BUTTON), ACCEPT_ID, this, ACCEPT_BUTTON));
			
			if(g_pUserInformation->GuildGrade == 1)	// deny도 마스터만 된다
				m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+w-2, y+h-49-30, m_guild_member_spk.GetWidth(DENY_BUTTON), m_guild_member_spk.GetHeight(DENY_BUTTON), DENY_ID, this, DENY_BUTTON));
		}
		else if(info->GRADE != 1 && g_pUserInformation->GuildGrade == 1)	// 추방은 마스터만 된다
		{
			m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+w-2, y+h-49-30, m_guild_member_spk.GetWidth(EXPEL_BUTTON), m_guild_member_spk.GetHeight(EXPEL_BUTTON), EXPEL_ID, this, EXPEL_BUTTON));
		}
	}
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+w-2, y+h-49, m_guild_member_spk.GetWidth(CLOSE_BUTTON), m_guild_member_spk.GetHeight(CLOSE_BUTTON), CLOSE_ID, this, CLOSE_BUTTON));	
	
	m_scroll = 0;
	m_scroll_max = 5;
	m_bl_scrolling = false;
	
	m_member_info = *info;
	
	// 캐릭터 이름과 선택한 정보의 이름이 같으면 자기 자신 정보이다.
	if(!strcmp(m_member_info.NAME.c_str(),g_char_slot_ingame.sz_name.c_str()))
		m_own_info = true;

	if(m_own_info)
	{
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+w-2, y+h-49-30, m_guild_member_spk.GetWidth(MODIFY_BUTTON), m_guild_member_spk.GetHeight(MODIFY_BUTTON), MODIFY_ID, this, MODIFY_BUTTON));		
	}
	
	int m_print_x = x+30;
	int m_print_y = y+60;
	int m_print_gap = 20;
	
	m_lev_intro.SetPosition(m_print_x, m_print_y+m_print_gap*3);
	m_lev_intro.SetEditorMode(20, 10);
	m_lev_intro.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_intro.SetByteLimit(30*10);
	m_lev_intro.SetAbsWidth(w - (m_print_x - x)-30);
	m_lev_intro.SetInputStringColor(RGB_BLACK);
	m_lev_intro.SetCursorColor(RGB_BLACK);
	m_lev_intro.AddString(m_member_info.INTRODUCTION.c_str());
	
	Attach(&m_lev_intro);
}

C_VS_UI_TEAM_MEMBER_INFO::~C_VS_UI_TEAM_MEMBER_INFO()
{
	g_UnregisterWindow(this);
	m_lev_intro.Unacquire();	
	gC_vs_ui.AcquireChatting();
	DeleteNew( m_pC_button_group );
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_INFO::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_INFO::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_INFO::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_INFO::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_INFO::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_INFO::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();
	
	AttrTopmost(true);
	gpC_window_manager->AppearWindow(this);

	if(m_own_info)
		m_lev_intro.Acquire();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_INFO::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_INFO::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_INFO::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_TEAM_MEMBER_INFO::Show()
{
	const int scroll_tag_x = x+w-28, scroll_tag_y = y+108-20, scroll_tag_height = 198;
	const int print_x = x+30, print_y = y+60, print_gap = 20, line_gap = 12;
	
	
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_guild_member_spk.BltLocked(x, y, INFO_WINDOW);
		m_guild_member_spk.BltLocked(x+w/2-m_guild_member_spk.GetWidth(TITLE_TEAM_MEMBER_INFO)/2, y+20, TITLE_TEAM_MEMBER_INFO);
		m_guild_member_spk.BltLocked(x+w-30, print_y-6+print_gap*1, SCROLL_BAR);
		if(m_bl_scrolling)
			m_guild_member_spk.BltLocked(scroll_tag_x, min(max(gpC_mouse_pointer->GetY(), scroll_tag_y), scroll_tag_y+scroll_tag_height), SCROLL_TAG);
		else
			m_guild_member_spk.BltLocked(scroll_tag_x, scroll_tag_y+scroll_tag_height*m_scroll/(m_scroll_max), SCROLL_TAG);
		
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	char sz_string[512];
	int py = print_y, vx;
	
	if(m_scroll < 1)
	{
		wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_NAME].GetString(), m_member_info.NAME.c_str());
		g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
		py += print_gap;
	}
	
	if(m_scroll < 2)
	{
		const static char* grade_string[4] = 
		{
			(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_GRADE_MEMBER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_GRADE_MASTER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_GRADE_SUB_MASTER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_GRADE_WAIT].GetString(),
		};
		wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_GRADE].GetString(), grade_string[m_member_info.GRADE]);
		g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
		py += print_gap;
	}
	
	//	py += print_gap;
	
	int next = 0;
	
	if(m_scroll < 3)
	{
		wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_INTRODUCTION].GetString());//, m_member_info.INTRODUCTION.c_str());
		vx = g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
	}
	gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
	
	const int char_width = g_GetStringWidth("a", gpC_base->m_chatting_pi.hfont);
	
	/*
	while(m_member_info.INTRODUCTION.size()>next && py < y+h-print_gap*2)
	{
		strcpy(sz_string, m_member_info.INTRODUCTION.c_str()+next);
		
		char *sz_string2 = sz_string;
		
		while(*sz_string2 == ' ')		// 앞의 공백제거
		{
			sz_string2++;
			next++;
		}
		
		int cut_pos = (x+w-30 -vx)/char_width;

		if(!g_PossibleStringCut(sz_string2, cut_pos))
			cut_pos--;
		sz_string2[cut_pos] = NULL;
		
		char *return_char = NULL;
		if((return_char = strchr(sz_string2, '\n')) != NULL)	// return 처리
		{
			cut_pos = return_char - sz_string2+1;
			sz_string2[cut_pos-1] = NULL;
		}
		
		g_PrintColorStr(vx, py, sz_string2, gpC_base->m_chatting_pi, RGB_BLACK);
		next += cut_pos;
		vx = print_x;
		gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
		py += print_gap;
	}*/
	m_lev_intro.SetPosition(x+30, y+60+20*max(0,(3-m_scroll)));
	m_lev_intro.SetEditorMode(20,10+m_scroll);

	m_lev_intro.Show();
	
	if(!strlen(m_member_info.INTRODUCTION.c_str()))
	{
		gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
		py += print_gap;
	}
	
	while(py < y+h-print_gap*2)
	{
		gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, w - (print_x - x) -30, 0);
		py += print_gap;
	}
	m_pC_button_group->ShowDescription();
	/*					 길드 멤버 정보에 길드마크 x
	// 길드 마크 출력
	CSprite *p_guild_mark;
	p_guild_mark=g_pGuildMarkManager->GetGuildMark(m_member_info.guild_id);
	if(p_guild_mark==NULL)
	{
	g_pGuildMarkManager->LoadGuildMark(m_member_info.guild_id);
	p_guild_mark=g_pGuildMarkManager->GetGuildMark(m_member_info.guild_id);			
	}
	RECT rect={x,y,0,0};
	POINT guild_point = { rect.left,rect.top };
	if(p_guild_mark!=NULL)
	gpC_base->m_p_DDSurface_back->BltSprite(&guild_point, p_guild_mark);	
	*/
	
	SHOW_WINDOW_ATTR;
}

void	C_VS_UI_TEAM_MEMBER_INFO::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char*	m_help_string[EXEC_MAX] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
			
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_EXPEL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_JOIN_ACCEPT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_JOIN_DENY].GetString(),
			
		(*g_pGameStringTable)[UI_STRING_MESSAGE_UP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_DOWN].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MODIFY_INFO].GetString(),
	};
	
	if (!m_bl_scrolling)
		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}
//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_INFO::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_INFO::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);
	
	
	
	switch(p_button->GetID())
	{
	case X_ID:
	case HELP_ID:
	case SCROLL_UP_ID:
	case SCROLL_DOWN_ID:
		if(p_button->GetFocusState() && p_button->GetPressState())
			m_guild_member_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index+1);
		else
			m_guild_member_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index);
		break;
		
	default:
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_guild_member_spk.BltLocked(p_button->x, p_button->y+2, p_button->m_image_index+1);
			else
				m_guild_member_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index+1);
		}
		else
			m_guild_member_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index);
		break;
		
		
	}
	
}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_INFO::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);
	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE: 
			gpC_base->SendMessage(UI_CLOSE_TEAM_MEMBER_INFO);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_INFO::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_TEAM_MEMBER_INFO::MouseControl(UINT message, int _x, int _y)
{
	if(!m_lev_intro.IsAcquire()&&m_own_info)
		m_lev_intro.Acquire();

	const int scroll_tag_x = x+w-28, scroll_tag_y = y+108-20, scroll_tag_height = 198;
	
	Window::MouseControl(message, _x, _y);
	
	switch(message)
	{
	case M_MOVING:
		if(m_bl_scrolling)
		{
			m_scroll = min(m_scroll_max, (m_scroll_max)*(max(_y, scroll_tag_y) - scroll_tag_y + scroll_tag_height/(m_scroll_max)/2)/scroll_tag_height);
		}
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if(_x >= scroll_tag_x && _x <= scroll_tag_x+m_guild_member_spk.GetWidth(SCROLL_BAR) && _y >= scroll_tag_y && _y <= scroll_tag_y+scroll_tag_height)
		{
			m_bl_scrolling = true;
			m_scroll = (m_scroll_max)*(_y - scroll_tag_y + scroll_tag_height/(m_scroll_max)/2)/scroll_tag_height;
		}
		break;
		
	case M_LEFTBUTTON_UP:
		if(m_bl_scrolling)
		{
			m_bl_scrolling = false;
		}
		break;
		
	case M_WHEEL_UP:
		Run(SCROLL_UP_ID);
		break;
		
	case M_WHEEL_DOWN:
		Run(SCROLL_DOWN_ID);
		break;
	}
	
	m_pC_button_group->MouseControl(message, _x, _y);
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_INFO::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_TEAM_MEMBER_INFO::IsPixel(int _x, int _y)
{
	bool re = m_guild_member_spk.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), INFO_WINDOW);
	
	if(re == false)
	{
		if(m_guild_member_spk.IsPixel(_x - (x+w-2), _y - (y+h-49), CLOSE_BUTTON))// ||
			re = true;
	}
	if(re == false && g_pUserInformation->GuildGrade == 1 || g_pUserInformation->GuildGrade == 2)
	{
		if(m_member_info.GRADE == 3)	// 대기자일때
		{
			if(m_guild_member_spk.IsPixel(_x - (x+w-2), _y - (y+h-49-60), ACCEPT_BUTTON) ||
				m_guild_member_spk.IsPixel(_x - (x+w-2), _y - (y+h-49-30), DENY_BUTTON))
			re = true;
		}
		else if(m_member_info.GRADE != 1)
		{
			if(m_guild_member_spk.IsPixel(_x - (x+w-2), _y - (y+h-49-30), EXPEL_BUTTON))// ||
				re = true;
		}
		
	}
	if(m_own_info)
	{
		if(m_guild_member_spk.IsPixel( _x - (x+w-2),_y-(y+h-49-30), MODIFY_BUTTON))
			re = true;
	}
	
	return re;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_MEMBER_INFO::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_MEMBER_INFO::Run(id_t id)
{
	switch (id)
	{
	case MODIFY_ID :
		{
			char *sz_intro = NULL;
			g_Convert_DBCS_Ascii2SingleByte(m_lev_intro.GetString(),m_lev_intro.Size(),sz_intro);
			if(sz_intro != NULL)
			{
				m_member_info.INTRODUCTION = sz_intro;
				DeleteNew(sz_intro);
				gpC_base->SendMessage(UI_MODIFY_GUILD_MEMBER_INTRO,0,0,(void *)m_member_info.INTRODUCTION.c_str());
			}
		}
		break;

	case X_ID:
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_TEAM_MEMBER_INFO);
		break;
		
	case ACCEPT_ID:
		gpC_base->SendMessage(UI_MODIFY_GUILD_MEMBER, m_member_info.guild_id, 0, (void *)m_member_info.NAME.c_str());
		break;
		
	case DENY_ID:
	case EXPEL_ID:
		gpC_base->SendMessage(UI_MODIFY_GUILD_MEMBER, m_member_info.guild_id, 4, (void *)m_member_info.NAME.c_str());
		break;
		
	case SCROLL_UP_ID:
		if(m_scroll > 0)
			m_scroll--;
		break;
		
	case SCROLL_DOWN_ID:
		if(m_scroll < m_scroll_max)
			m_scroll++;
		break;
		
	case HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::GUILD);
		break;
		
	}
}


//----------------------------------------------------------------------------
// C_VS_UI_TEAM_REGIST::C_VS_UI_TEAM_REGIST
//
//
//----------------------------------------------------------------------------
C_VS_UI_TEAM_REGIST::C_VS_UI_TEAM_REGIST(bool member, int reg_fee, int rank, char *date, char *team_name, int guild_id)
{
	m_guild_id = guild_id;
	m_bl_member = member;
	
	if(team_name)
		m_team_name = team_name;
	
	if(date)
		m_date = date;

	m_rank = rank;
	m_reg_fee = reg_fee;
	
	g_RegisterWindow(this);
	
	AttrTopmost(true);
	AttrKeyboardControl(true);
	
	if(gbl_vampire_interface)
		m_guild_spk.Open(SPK_VAMPIRE_TEAM);
	else
		m_guild_spk.Open(SPK_SLAYER_TEAM);
	
	if(gbl_vampire_interface && member == false)
	{
		m_vampire_point.x = 17;
		m_vampire_point.y = 0;
	}
	else
	{
		m_vampire_point.x = 0;
		m_vampire_point.y = 0;
	}
	
	m_guild_mark = NULL;
	
	int help_x, x_x, button_y;
	int button_x, regist_y, close_y;
	
	if(m_bl_member)
	{
		Set(RESOLUTION_X/2-m_guild_spk.GetWidth(INFO_WINDOW)/2, RESOLUTION_Y/2 - m_guild_spk.GetHeight(INFO_WINDOW)/2, m_guild_spk.GetWidth(INFO_WINDOW), m_guild_spk.GetHeight(INFO_WINDOW));
		m_print_x = x+30;
		m_print_y = y+60;
		m_print_gap = 20;
		help_x = 257;
		x_x = 257+14;
		button_y = 6;
		button_x = x+w-2;
	}
	else
	{
		Set(RESOLUTION_X/2-m_guild_spk.GetWidth()/2, RESOLUTION_Y/2 - m_guild_spk.GetHeight()/2, m_guild_spk.GetWidth(), m_guild_spk.GetHeight());
		m_print_x = x+30+m_vampire_point.x;
		m_print_y = y+90+m_vampire_point.y;
		m_print_gap = 20;
		help_x = 353;
		x_x = 367;
		if(gbl_vampire_interface)
		{
			help_x+=20; x_x+=20;
		}
		button_y = 2;
		button_x = x+w-14;
		m_mark_x = 40+m_vampire_point.x;
		m_mark_y = 50+m_vampire_point.y+7;
	}
	
	regist_y = y+h-49-30;
	close_y = y+h-49;
	
	// set button
	m_pC_button_group = new ButtonGroup(this);
	
	
	//help, x버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + help_x, y + button_y, m_guild_spk.GetWidth(HELP_BUTTON), m_guild_spk.GetHeight(HELP_BUTTON), HELP_ID, this, HELP_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + x_x, y + button_y, m_guild_spk.GetWidth(X_BUTTON), m_guild_spk.GetHeight(X_BUTTON), X_ID, this, X_BUTTON));
	
	//REGIST, CLOSE버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(button_x, regist_y, m_guild_spk.GetWidth(REGISTER_BUTTON), m_guild_spk.GetHeight(REGISTER_BUTTON), REGIST_ID, this, REGISTER_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(button_x, close_y, m_guild_spk.GetWidth(CLOSE_BUTTON), m_guild_spk.GetHeight(CLOSE_BUTTON), CLOSE_ID, this, CLOSE_BUTTON));
	
	m_scroll = 0;
	m_bl_scrolling = false;
	
	if(!m_bl_member)
	{
		m_lev_team_name.SetPosition(m_print_x+72, m_print_y+m_print_gap*2);
		m_lev_team_name.SetPrintInfo(gpC_base->m_chatting_pi);
		m_lev_team_name.SetByteLimit(16);
		m_lev_team_name.SetInputStringColor(RGB_BLACK);
		m_lev_team_name.SetCursorColor(RGB_BLACK);
	}
	
	m_lev_intro.SetEditorMode(m_print_gap, 10);
	if(m_bl_member)
	{
		if(m_reg_fee == 0)
			m_lev_intro.SetPosition(m_print_x, m_print_y+m_print_gap*3);
		else
		{
			m_lev_intro.SetPosition(m_print_x, m_print_y+m_print_gap*4);
			m_lev_intro.SetEditorMode(m_print_gap, 9);
		}
	}
	else
		m_lev_intro.SetPosition(m_print_x, m_print_y+m_print_gap*4);
	m_lev_intro.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_intro.SetByteLimit(30*10);
	m_lev_intro.SetAbsWidth(w - (m_print_x - x)-30);
	m_lev_intro.SetInputStringColor(RGB_BLACK);
	m_lev_intro.SetCursorColor(RGB_BLACK);		
	Attach(&m_lev_intro);
}

C_VS_UI_TEAM_REGIST::~C_VS_UI_TEAM_REGIST()
{
	g_UnregisterWindow(this);
	
	m_lev_intro.Unacquire();
	if(!m_bl_member)
		gC_vs_ui.AcquireChatting();
	
	if(m_guild_mark)
		DeleteNew(m_guild_mark);
	DeleteNew( m_pC_button_group );
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_REGIST::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_REGIST::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_REGIST::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_REGIST::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_REGIST::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_REGIST::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();
	
	if(m_bl_member)
	{
		m_lev_intro.Acquire();
	}
	else
	{
		m_lev_team_name.Acquire();
	}
	
	AttrTopmost(true);
	gpC_window_manager->AppearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_REGIST::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_REGIST::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_REGIST::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_TEAM_REGIST::Show()
{
	const int scroll_tag_x = x+365+m_vampire_point.x, scroll_tag_y = y+108+m_vampire_point.y, scroll_tag_height = 198;
	const int line_gap = 12;
	
	
	char sz_string[256];
	int py = m_print_y;
	
	if(m_bl_member)	//member regist
	{
		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			m_guild_spk.BltLocked(x, y, INFO_WINDOW);
			m_guild_spk.BltLocked(x+w/2-m_guild_spk.GetWidth(TITLE_MEMBER_REGISTRATION)/2, y+20, TITLE_MEMBER_REGISTRATION);
			m_pC_button_group->Show();			
			gpC_base->m_p_DDSurface_back->Unlock();
		}
		
		if(gbl_vampire_interface)
			wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_CLAN_NAME].GetString(), m_team_name.c_str());
		else
			wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_TEAM_NAME].GetString(), m_team_name.c_str());

		g_PrintColorStr(m_print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		gpC_base->m_p_DDSurface_back->HLine(m_print_x, py+line_gap, w - (m_print_x - x) -30, 0);
		py += m_print_gap;
		
		wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_NAME].GetString(), g_char_slot_ingame.sz_name.c_str());
		g_PrintColorStr(m_print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		gpC_base->m_p_DDSurface_back->HLine(m_print_x, py+line_gap, w - (m_print_x - x) -30, 0);
		py += m_print_gap;
		
		// 숫자사이에 ,넣기
		if(m_reg_fee != 0)
		{
			wsprintf(sz_string, "%d", m_reg_fee);
			std::string sstr = sz_string;
			for(int i = 3; i <= 13; i += 4)
				if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
				
			wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_REGISTRATION_FEE].GetString(), sstr.c_str());
			g_PrintColorStr(m_print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
			gpC_base->m_p_DDSurface_back->HLine(m_print_x, py+line_gap, w - (m_print_x - x) -30, 0);
			py += m_print_gap;
		}
		
		wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_SELF_INTRODUCTION].GetString());
		g_PrintColorStr(m_print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		m_pC_button_group->ShowDescription();
		
		gpC_base->m_p_DDSurface_back->HLine(m_print_x, py+line_gap, w - (m_print_x - x) -30, 0);
		py += m_print_gap;
		
		while(py <= y+h-m_print_gap*2)
		{
			gpC_base->m_p_DDSurface_back->HLine(m_print_x, py+line_gap, w - (m_print_x - x) -30, 0);
			py += m_print_gap;
		}
		
		//		wsprintf(sz_string, "date : %s", m_date.c_str());
		//		int print_x = x+w - 30 - g_GetStringWidth(sz_string, gpC_base->m_chatting_pi.hfont);
		
		//		g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		//		gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, x+w - print_x -30, 0);
		//		py += m_print_gap;
	}
	else	// team regist
	{
		//파일 받아오기
		//		if(gC_vs_ui.GetFileName())
		//		{
		//			m_guild_mark = new CDirectDrawSurface;
		//
		//			m_guild_mark->InitFromBMP(gC_vs_ui.GetFileName());
		//			if(m_guild_mark->GetWidth() != 20 || m_guild_mark->GetHeight() != 20)
		//			{
		//				m_guild_mark->ReleaseSurface();
		//				DeleteNew(m_guild_mark);
		//				m_guild_mark = NULL;
		//			}
		//			gC_vs_ui.CloseFileDialog();
		//		}
		//
		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			m_guild_spk.BltLocked(x, y);
			m_guild_spk.BltLocked(x+w/2-m_guild_spk.GetWidth(TITLE_TEAM_REGISTRATION)/2, y+50, TITLE_TEAM_REGISTRATION);
			
			m_pC_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();
		}
		
		// 길드 마크 찍기
		//		if(m_guild_mark == NULL)
		//		{
		//			RECT rect;
		//			rect.left = x+m_mark_x;
		//			rect.right = rect.left + 20;
		//			rect.top = y+m_mark_y;
		//			rect.bottom = rect.top + 20;
		//			DrawAlphaBox(&rect, 255, 255, 255, 20);
		//		}
		//		else
		//		{
		//			RECT rect = {0, 0, 20, 20};
		//			POINT point = {x+m_mark_x, y+m_mark_y};
		//			gpC_base->m_p_DDSurface_back->BltNoColorkey(&point, m_guild_mark, &rect);
		//		}
		
		int vampire_width = 0;
		if(gbl_vampire_interface)
		{
			vampire_width = -20;
		}
		wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_NAME].GetString(), g_char_slot_ingame.sz_name.c_str());
		g_PrintColorStr(m_print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		gpC_base->m_p_DDSurface_back->HLine(m_print_x, py+line_gap, w - (m_print_x - x) -30+vampire_width, 0);
		py += m_print_gap;
		
		// 숫자사이에 ,넣기
		wsprintf(sz_string, "%d", m_reg_fee);
		std::string sstr = sz_string;
		for(int i = 3; i <= 13; i += 4)
			if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
			
		wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_REGISTRATION_FEE].GetString(), sstr.c_str());
		g_PrintColorStr(m_print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		gpC_base->m_p_DDSurface_back->HLine(m_print_x, py+line_gap, w - (m_print_x - x) -30+vampire_width, 0);
		py += m_print_gap;
		
		if(gbl_vampire_interface)
			wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_CLAN_NAME].GetString(), "");
		else
			wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_TEAM_NAME].GetString(), "");
	
		g_PrintColorStr(m_print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		gpC_base->m_p_DDSurface_back->HLine(m_print_x, py+line_gap, w - (m_print_x - x) -30+vampire_width, 0);
		py += m_print_gap;
		
		if(gbl_vampire_interface)
			wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_CLAN_INTRODUCTION].GetString());
		else
			wsprintf(sz_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_TEAM_INTRODUCTION].GetString());
		
		g_PrintColorStr(m_print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		gpC_base->m_p_DDSurface_back->HLine(m_print_x, py+line_gap, w - (m_print_x - x) -30+vampire_width, 0);
		py += m_print_gap;
		m_pC_button_group->ShowDescription();
		
		while(py < y+h-m_print_gap*3)
		{
			gpC_base->m_p_DDSurface_back->HLine(m_print_x, py+line_gap, w - (m_print_x - x) -30+vampire_width, 0);
			py += m_print_gap;
		}
		// 슬레이어가 한줄 모자르더라 .-_-
		if(!gbl_vampire_interface)
		{
			gpC_base->m_p_DDSurface_back->HLine(m_print_x, py+line_gap, w - (m_print_x - x) -30+vampire_width, 0);
			py += m_print_gap;
		}
		//		wsprintf(sz_string, "date : %s", m_date.c_str());
		//		int print_x = x+w - 30+vampire_width - g_GetStringWidth(sz_string, gpC_base->m_chatting_pi.hfont);
		
		//		g_PrintColorStr(print_x, py, sz_string, gpC_base->m_chatting_pi, RGB_BLACK);
		//		gpC_base->m_p_DDSurface_back->HLine(print_x, py+line_gap, x+w - print_x -30+vampire_width, 0);
		//		py += m_print_gap;
	}
	
	//	m_guild_spk.Blt(x+w-30, m_print_y-6+m_print_gap*-1, SCROLL_BAR);
	m_lev_intro.Show();
	m_lev_team_name.Show();
	
	
	SHOW_WINDOW_ATTR;
}

void	C_VS_UI_TEAM_REGIST::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char*	m_help_string[EXEC_MAX] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_REGIST].GetString(),
	};
	
	if (!m_bl_scrolling)
		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);	
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_REGIST::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_REGIST::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);
	
	
	switch(p_button->GetID())
	{
	case X_ID:
	case HELP_ID:
		if(p_button->GetFocusState() && p_button->GetPressState())
			m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index+1);
		else
			m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index);
		break;
		
	case REGIST_ID:
	case CLOSE_ID:
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_guild_spk.BltLocked(p_button->x, p_button->y+2, p_button->m_image_index+1);
			else
				m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index+1);
		}
		else
			m_guild_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index);
		break;
	}
}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_REGIST::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);
	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_TAB:
			if(!m_bl_member)
			{
				if(m_lev_team_name.IsAcquire())
					m_lev_intro.Acquire();
				else
					m_lev_team_name.Acquire();
			}
			break;
			
		case VK_RETURN:
			if(!m_bl_member)
			{
				if(m_lev_team_name.IsAcquire())
					m_lev_intro.Acquire();
			}
			break;
			
		case VK_ESCAPE: 
			gpC_base->SendMessage(UI_CLOSE_TEAM_REGIST);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_REGIST::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_TEAM_REGIST::MouseControl(UINT message, int _x, int _y)
{
	int scroll_tag_x = x+365, scroll_tag_y = y+108, scroll_tag_height = 198;
	
	Window::MouseControl(message, _x, _y);
	
	switch(message)
	{
	case M_MOVING:
		//		if(m_bl_scrolling)
		//		{
		//			if(m_bl_ready)
		//			{
		//				if(m_v_ready_team_search_list.size())
		//					m_scroll = min(m_v_ready_team_search_list.size()-9, (m_v_ready_team_search_list.size()-9)*(max(_y, scroll_tag_y) - scroll_tag_y + scroll_tag_height/(m_v_ready_team_search_list.size()-9)/2)/scroll_tag_height);
		//				else
		//					m_scroll = min(m_v_ready_team_list.size()-9, (m_v_ready_team_list.size()-9)*(max(_y, scroll_tag_y) - scroll_tag_y + scroll_tag_height/(m_v_ready_team_list.size()-9)/2)/scroll_tag_height);
		//			}
		//		}
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		//		if(_x >= scroll_tag_x && _x <= scroll_tag_x+m_guild_spk.GetWidth(SCROLL_BAR) && _y >= scroll_tag_y && _y <= scroll_tag_y+scroll_tag_height)
		//		{
		//			if(m_bl_ready)
		//			{
		//				if(m_v_ready_team_list.size() > 9)	// 스크롤이 생기는 경우에만
		//				{
		//					m_bl_scrolling = true;
		//					m_scroll = (m_v_ready_team_list.size()-9)*(_y - scroll_tag_y + scroll_tag_height/(m_v_ready_team_list.size()-9)/2)/scroll_tag_height;
		//				}
		//			}
		//		}
		//		if(_x > x+m_mark_x && _x < x+m_mark_x+20 && _y > y+m_mark_y && _y < y+m_mark_y+20)
		//		{
		//			gC_vs_ui.RunFileDialog(".bmp");
		//		}
		break;
		
	case M_LEFTBUTTON_UP:
		if(m_bl_scrolling)
		{
			m_bl_scrolling = false;
		}
		break;
	}
	
	m_pC_button_group->MouseControl(message, _x, _y);
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_REGIST::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_TEAM_REGIST::IsPixel(int _x, int _y)
{
	bool re;
	if(m_bl_member)
	{
		re = m_guild_spk.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), INFO_WINDOW);
		if(re == false)
		{
			if(m_guild_spk.IsPixel(_x - (x+w-2), _y - (y+h-49), CLOSE_BUTTON) ||
				m_guild_spk.IsPixel(_x - (x+w-2), _y - (y+h-49-30), JOIN_BUTTON))
				re = true;
		}
	}
	else
	{
		re = m_guild_spk.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
		if(re == false)
		{
			if(m_guild_spk.IsPixel(_x - (x+w-14), _y - (y+h-49), CLOSE_BUTTON) ||
				m_guild_spk.IsPixel(_x - (x+w-14), _y - (y+h-49-30), JOIN_BUTTON))
				re = true;
		}
	}
	return re;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TEAM_REGIST::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TEAM_REGIST::Run(id_t id)
{
	switch (id)
	{
	case X_ID:
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_TEAM_REGIST);
		break;
		
	case REGIST_ID:
		if(m_bl_member)
		{			
			char *sz_intro = NULL;
			g_Convert_DBCS_Ascii2SingleByte(m_lev_intro.GetString(), m_lev_intro.Size(), sz_intro);

			if(sz_intro!=NULL)
			{
				m_introduction = sz_intro;
				gpC_base->SendMessage(UI_REGIST_GUILD_MEMBER, m_guild_id, m_rank, (void *)m_introduction.c_str());
				
				DeleteNew(sz_intro);
			}
		}
		else
		{
			char *sz_intro = NULL, *sz_team_name = NULL;			
			
			g_Convert_DBCS_Ascii2SingleByte(m_lev_team_name.GetString(), m_lev_team_name.Size(), sz_team_name);
			g_Convert_DBCS_Ascii2SingleByte(m_lev_intro.GetString(), m_lev_intro.Size(), sz_intro);

			if(sz_intro != NULL && sz_team_name != NULL)
			{
				m_introduction = sz_intro;
				m_team_name = sz_team_name;
				
				gpC_base->SendMessage(UI_REGIST_GUILD_TEAM, (int)m_team_name.c_str(), 0, (void *)m_introduction.c_str());
				
				DeleteNew(sz_intro);
				DeleteNew(sz_team_name);
			}
		}
		break;
		
	case HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::GUILD);
		break;
	}
}


//-----------------------------------------------------------------------------
// C_VS_UI_OTHER_INFO::C_VS_UI_OTHER_INFO
//
//
//-----------------------------------------------------------------------------
C_VS_UI_OTHER_INFO::C_VS_UI_OTHER_INFO()
{
	g_RegisterWindow(this);
	
	if(gbl_vampire_interface)
	{
		Set(RESOLUTION_X/2 - 210/2, RESOLUTION_Y/2 - 242/2, 210, 242);
	}
	else
	{
		Set(RESOLUTION_X/2 - 210/2, RESOLUTION_Y/2 - 255/2, 210, 255);
	}
	//m_pC_info_spk=C_VS_UI_INFO::m_pC_info_spk;
	
	AttrPin(true);
	
	AttrTopmost(false);
	
	g_RegisterWindow(this);
	//close버튼 좌표 세팅
	int close_x = w-24, close_y = h-19;
	int help_x = w-24-20, help_y = h-19;
	if(gbl_vampire_interface)
	{
		close_x -= 5;
		close_y -= 7;
		help_x -=5;
		help_y -= 7;
	}
	int alpha_x = 6, alpha_y = h-21;
	if(gbl_vampire_interface)
	{
		alpha_x += 5;
		alpha_y -= 5;
	}
	
	//공통버튼
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BUTTON_CLOSE), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BUTTON_CLOSE), CLOSE_ID, this, C_GLOBAL_RESOURCE::BUTTON_CLOSE));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(help_x, help_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BUTTON_HELP), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BUTTON_HELP), HELP_ID, this, C_GLOBAL_RESOURCE::BUTTON_HELP));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_x, alpha_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));
	m_p_face = NULL;
	
	ZeroMemory(&m_player_info,sizeof(PLAYER_INFO));
	m_p_face_spk=NULL;
	m_p_face_spk = new C_SPRITE_PACK(SPK_FACE);
	
//	IsRunning=false;
	m_p_guild_mark = NULL;
	
//	RefreshImage();

}

//-----------------------------------------------------------------------------
// C_VS_UI_OTHER_INFO::~C_VS_UI_OTHER_INFO
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_OTHER_INFO::~C_VS_UI_OTHER_INFO()
{
	m_p_guild_mark = NULL;
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::OTHER_INFO, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::OTHER_INFO, Rect(x, y, w, h));
	
	g_UnregisterWindow(this);
	//C_VS_UI_INFO::m_pC_info_spk
	//m_pC_info_spk=NULL;
	DeleteNew(m_p_face_spk);
	DeleteNew(m_p_face);
	DeleteNew(m_pC_button_group);	
}

//-----------------------------------------------------------------------------
// C_VS_UI_OTHER_INFO::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_OTHER_INFO::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char* m_other_info_button_string[4]=
	{		
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),		
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP].GetString(),		
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
	};
	switch(p_button->GetID())
	{
	case CLOSE_ID :
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[2],0,0);
		break;
	case HELP_ID :
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[1],0,0);
		break;
	case ALPHA_ID :
		if(GetAttributes()->alpha)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[3],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[0],0,0);
		break;
	}	
}


//-----------------------------------------------------------------------------
// C_VS_UI_OTHER_INFO::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_OTHER_INFO::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	}
	//Close버튼
	else if(p_button->GetID() == CLOSE_ID || p_button->GetID() == HELP_ID)
	{
		gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-5, y+p_button->y-5, C_GLOBAL_RESOURCE::BUTTON_CLOSE_BACK);
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
		}
		else
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}		
	}
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_OTHER_INFO::WindowEventReceiver(id_t event)
{
}


//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_OTHER_INFO::Run(id_t id)
{
//	DEBUG_ADD("[ OtherInfo ] Run");
	switch (id)
	{
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_OTHER_INFO);
		break;
		
	case HELP_ID:		
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::OTHER_INFO);
		EMPTY_MOVE;
		break;
		
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;
		
	}
//	DEBUG_ADD("[ OtherInfo ] Run End");
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_OTHER_INFO::MouseControl(UINT message, int _x, int _y)
{
	RECT grade_rect={124+9,30,40+124+9,30+40};
	RECT guildmark_rect={22+55+9,30,22+55+9+40,30+40};

	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	
	switch(message)
	{
	case M_MOVING:
		{
			static char temp_str[2][100];
			const static char* str_g[2] = {
				temp_str[0],
				NULL,
			};			
			str_g[1]=NULL;					
			if(gbl_vampire_interface)
			{
				// 뱀파이어 계급
				const static char* grade[10] = 
				{
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_RITTER].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_REICHSRITTER].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_BARONET].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PREYHER].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_GRAF].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MARKGRAF].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PFALZGRAF].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FURST].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_HERZOG].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LANDESHER].GetString(),
				};
				// 계급, 길드, 레벨부분 Description
				if(_x>guildmark_rect.left && _x < guildmark_rect.right && _y > guildmark_rect.top && _y < guildmark_rect.bottom)
				{
					// 팀 이름이 비지 않았으면.
					//if(m_player_info.TEAM_NAME.size()>0)###@@@
					if(!m_player_info.TEAM_NAME.empty())
					{
						wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_CLAN_NAME].GetString(),m_player_info.TEAM_NAME.c_str());
						g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void *)str_g,1);
					}

				} else				
				if(_x > 22 && _y > 97 && _x < 184 && _y < 157)
				{					
					int num=(_y-97)/20;
					switch(num)
					{
					case 0 : // grade
//						DEBUG_ADD("[ OtherInfo ] MouseControl - Grade");
						if(m_player_info.GRADE > 0 && m_player_info.GRADE <= GRADE_MARK_MAX )
						{
							wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_NAME].GetString(),grade[(m_player_info.GRADE-1)/5]);
							wsprintf(temp_str[1],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LEVEL].GetString(),m_player_info.GRADE);
							str_g[1]=temp_str[1];							
						} else
						{
							//str[0] = NULL;
						}
						break;
					case 1 : // team
						{
//							DEBUG_ADD("[ OtherInfo ] MouseControl - Team");
							//if(m_player_info.TEAM_NAME.size()>0)###@@@
							if(!m_player_info.TEAM_NAME.empty())
								wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_CLAN_NAME].GetString(),m_player_info.TEAM_NAME.c_str());								
							else
								wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_CLAN_NAME].GetString(),(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_JOIN_ANY_CLAN].GetString());
						}
						break;
						
					case 2 : // level
						{
//							DEBUG_ADD("[ OtherInfo ] MouseControl - Level");
							// -_- 삑싸리. 툴팁 옆부부분 나가길래 임시로 막아놓음... 아래 str 쪽으로 빼자.
							if(_x > 101)
								return true;
							wsprintf(temp_str[0],"%s: %d",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEVEL].GetString(),m_player_info.LEVEL);
						}
						break;
					default :
						str_g[0] = NULL;
					}
					// 두번째줄이 NULL 이 아니면 두줄을 출력하도록 한다.
					if(str_g[0] != NULL)
					{
						if(str_g[1]!=NULL)
							g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str_g,2);
						else
							g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str_g,1);
					}

				} else			// 캐릭터 이름.
				if(_x>86 && _y>79 && _x < 184 && _y < 94)
				{
					// 캐릭터 이름 
					static char name[30];
//					DEBUG_ADD("[ OtherInfo ] MouseControl - CharacterName");
					wsprintf(name,"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_NAME].GetString(),m_player_info.PLAYER_NAME.c_str());
					const static char* str[1] = {
						name,
					};
					g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str,1);
				} else				// 계급 마크
				if(_x>grade_rect.left && _x < grade_rect.right && _y > grade_rect.top && _y < grade_rect.bottom && 
					m_player_info.GRADE > 0 && m_player_info.GRADE <= GRADE_MARK_MAX )
					
				{
					static char temp_str[2][100];
//					DEBUG_ADD("[ OtherInfo ] MouseControl - GradeMarkRect");
					
					wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_NAME].GetString(),grade[(m_player_info.GRADE-1)/5]);
					wsprintf(temp_str[1],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LEVEL].GetString(),m_player_info.GRADE);

					const static char* str_s[2] = {
						temp_str[0],					
						temp_str[1],
					};
					g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str_s,2);
				} /*else				// 성향
				if(_x>120&&_y>145&&_x<186&&_y<175)
				{
					static char temp_str[20];					
					// 음수 여부 판단해서 + 를 출력하지 않게 해준다.
					if(m_player_info.ALIGNMENT_NUM>0)
						wsprintf(temp_str, "(+%d)",m_player_info.ALIGNMENT_NUM);
					else
						wsprintf(temp_str, "(%d)",m_player_info.ALIGNMENT_NUM);
					const static char* str[3]={
						(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGNMENT].GetString(),
							NULL,
							temp_str
					};
					g_descriptor_manager.Set(DID_HELP, x+_x, y+_y, (void *)str,0,0);
				}*/ else			// str dex int
				if(_x>20&&_x<102&&_y>157&&_y<217)
				{					
					static char temp_str[2][50];
					int num=(_y-157)/20;
					
					switch(num)
					{
					case 0 :			// str
						wsprintf(temp_str[0],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_STR_PURE].GetString(),m_player_info.STR_PURE);
						wsprintf(temp_str[1],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_STR_CUR].GetString(),m_player_info.STR_CUR);
						break;
					case 1 :			// dex
						wsprintf(temp_str[0],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_DEX_PURE].GetString(),m_player_info.DEX_PURE);
						wsprintf(temp_str[1],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_DEX_CUR].GetString(),m_player_info.DEX_CUR);
						break;
					case 2 :				// int
						wsprintf(temp_str[0],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_INT_PURE].GetString(),m_player_info.INT_PURE);
						wsprintf(temp_str[1],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_INT_CUR].GetString(),m_player_info.INT_CUR);
						break;
					}
					const static char* str_s[2] = {
						temp_str[0],
						temp_str[1],						
					};					
					g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str_s,2);
				} 
			} else
			{
				const static char* grade[10] = 
				{
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PIVATE].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_SERENT].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FEACEL].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LITENA].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_KAINEL].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_GENEAL].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FORE_GENEAL].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MAJORIS_GENEAL].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_CLOEL_GENEAL].GetString(),
					(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MARSHAL].GetString(),
				};

				if(_x>guildmark_rect.left && _x < guildmark_rect.right && _y > guildmark_rect.top && _y < guildmark_rect.bottom)
				{
					//if(m_player_info.TEAM_NAME.size()>0)
					if(!m_player_info.TEAM_NAME.empty())
					{
						wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_TEAM_NAME].GetString(),m_player_info.TEAM_NAME.c_str());
						g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void *)str_g,1);
					}
				} else				
				if(_x > 22 && _y > 97 && _x < 184 && _y < 157)
				{
					static char temp_str[2][100];
					const static char* str[2] = {
						temp_str[0],	
						NULL,
					};

					str[0]=temp_str[0];				
					str[1]=NULL;
					
					int num=(_y-97)/20;
					switch(num)
					{
					case 0 : // grade
//						DEBUG_ADD("[ OtherInfo ] MouseControl - Grade [S]");
						if(m_player_info.GRADE > 0 && m_player_info.GRADE <= GRADE_MARK_MAX )						
						{
							wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_NAME].GetString(),grade[(m_player_info.GRADE-1)/5]);
							wsprintf(temp_str[1],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LEVEL].GetString(),m_player_info.GRADE);
							str[1]=temp_str[1];							
						} else
						{
							str[0] =NULL;
						}
						break;
					case 1 : // team
						{
							//if(m_player_info.TEAM_NAME.size()>0)
//							DEBUG_ADD("[ OtherInfo ] MouseControl - Team [S]");
							if(!m_player_info.TEAM_NAME.empty())
								wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_TEAM_NAME].GetString(),m_player_info.TEAM_NAME.c_str());
							else
								wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_TEAM_NAME].GetString(),(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_JOIN_ANY_TEAM].GetString());
						}
						break;
						
					case 2 : // level
						{
//							DEBUG_ADD("[ OtherInfo ] MouseControl - Level [S]");
							const static char *SlayerJob[5] = {
								(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_BLADE2].GetString(),
								(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_SWORD2].GetString(),
								(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_GUN2].GetString(),
								(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_HEAL2].GetString(),
								(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_ENCHANT2].GetString(),
							};
					
							if(TopDomain>=0&&TopDomain<5)
								wsprintf(temp_str[0],"%s: %s %s %d",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEVEL].GetString(),SlayerJob[TopDomain],
								(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CLASS2].GetString(),
								m_player_info.DOMAINLEVEL[TopDomain]);
							else
								wsprintf(temp_str[0],"%s: ",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEVEL].GetString());
						}
						break;
					default :
						str[0]= NULL;
						break;
					}
					if(str[0]!=NULL)
					{
						if(str[1]!=NULL)
							g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str,2);
						else
							g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str,1);
					}
				} else
				if(_x>86 && _y>79 && _x < 184 && _y < 94)
				{
					// 캐릭터 이름
//					DEBUG_ADD("[ OtherInfo ] MouseControl - Character Name [S]");
					static char name[50];
					wsprintf(name,"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_NAME].GetString(),m_player_info.PLAYER_NAME.c_str());
					const static char* str[1] = {
						name,
					};
					g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str,1);
				} else
				if(_x>grade_rect.left && _x < grade_rect.right && _y > grade_rect.top && _y < grade_rect.bottom && 
					m_player_info.GRADE > 0 && m_player_info.GRADE <= GRADE_MARK_MAX )					
				{			
					
//					DEBUG_ADD("[ OtherInfo ] MouseControl - GradeMarkRect [S]");
					static char temp_str[2][100];
					wsprintf(temp_str[0],"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_NAME].GetString(),grade[(m_player_info.GRADE-1)/5]);
					wsprintf(temp_str[1],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LEVEL].GetString(),m_player_info.GRADE);
					const static char* str[2] = {
						temp_str[0],						
						temp_str[1],
					};
					g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str,2);
					
				} else
				/*if(_x>120&&_y>175&&_x<186&&_y<215)
				{
					static char temp_str[20];
					// 음수 여부 판단해서 + 를 출력하지 않게 해준다.
					if(m_player_info.ALIGNMENT_NUM > 0 )
						wsprintf(temp_str, "(+%d)",m_player_info.ALIGNMENT_NUM);
					else
						wsprintf(temp_str, "(%d)",m_player_info.ALIGNMENT_NUM);
					
					const static char* str[3]={
						(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGNMENT].GetString(),
						NULL,
						temp_str,
					};
					g_descriptor_manager.Set(DID_HELP, x+_x, y+_y, (void *)str,0,0);
				}else*/
				if(_x>20&&_x<102&&_y>178&&_y<238)
				{	
//					DEBUG_ADD("[ OtherInfo ] MouseControl - STRDEXINT [S]");
					static char temp_str[2][50];
					int num=(_y-178)/20;
					
					switch(num)
					{
					case 0 :			// str
						wsprintf(temp_str[0],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_STR_PURE].GetString(),m_player_info.STR_PURE);
						wsprintf(temp_str[1],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_STR_CUR].GetString(),m_player_info.STR_CUR);
						break;
					case 1 :			// dex
						wsprintf(temp_str[0],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_DEX_PURE].GetString(),m_player_info.DEX_PURE);
						wsprintf(temp_str[1],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_DEX_CUR].GetString(),m_player_info.DEX_CUR);
						break;
					case 2 :				// int
						wsprintf(temp_str[0],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_INT_PURE].GetString(),m_player_info.INT_PURE);
						wsprintf(temp_str[1],"%s%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_INFO_INT_CUR].GetString(),m_player_info.INT_CUR);
						break;
					//default :
					//	temp_str[0]=NULL;
					}

					const static char* str[2] = {
						temp_str[0],
						temp_str[1],						
					};

					if(temp_str[0]!=NULL)
						g_descriptor_manager.Set(DID_STRINGS, x+_x, y+_y, (void *)str,2);
				} else
				if(_x>54&&_x<183&&_y>160&&_y<174)
				{
//					DEBUG_ADD("[ OtherInfo ] MouseControl - Fame [S]");
					static char tempstring[100];
					char sz_temp[50];
					const static char *str[1] = {tempstring};
					
					wsprintf(sz_temp, "%d", m_player_info.FAME);
					
					std::string sstr = sz_temp;
					for(int i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
						
					sprintf(tempstring,"%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_FAME].GetString(),sstr.c_str());
						
					g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void*)str,1);
				}
			}
		}
		break;
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if (gpC_mouse_pointer->GetPickUpItem()==FALSE && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
		}
		break;
	}
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_OTHER_INFO
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_OTHER_INFO::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN && key == VK_ESCAPE)
	{
		//		Run(CLOSE_ID);
	}
}

void	C_VS_UI_OTHER_INFO::RefreshImage()
{
//	DEBUG_ADD("[ OtherInfo ] Refresh Image");
	if(m_p_face)
		DeleteNew(m_p_face);
	
	m_p_face = new CSpritePack;
	m_p_face->Init(1, CDirectDraw::Is565());
	
	if(g_pProfileManager->HasProfile(m_player_info.PLAYER_NAME.c_str()))
	{
		const char *filename = g_pProfileManager->GetFilename(m_player_info.PLAYER_NAME.c_str());
		char filenamei[256];
		
		strcpy(filenamei, filename);
		strcat(filenamei, "i");
		
		if(!m_p_face->LoadFromFileSprite(0, 1, filename, filenamei ))
		{
			delete m_p_face;
			m_p_face = NULL;
		}
	}
	else
	{
		if(m_p_face)
		{
			delete m_p_face;
			m_p_face = NULL;
		}

		// face spk
		// portrait
		const int _VAMPIRE_OFFSET = 6;
		const int _WOMAN_OFFSET = 3;
		int spriteID;

		if (m_player_info.bl_female)
			spriteID = m_player_info.face_type-W_FACE1+_WOMAN_OFFSET; 
		else
			spriteID = m_player_info.face_type-M_FACE1;
		
		if (g_char_slot_ingame.bl_vampire)
		{
			if(g_char_slot_ingame.bl_female)
				spriteID = _VAMPIRE_OFFSET+1;
			else
				spriteID = _VAMPIRE_OFFSET;
		}
		
		if(!m_p_face->LoadFromFileSprite(0, spriteID, SPK_FACE, SPKI_FACE))
		{
			delete m_p_face;
			m_p_face = NULL;
		}
	}		
}

//-----------------------------------------------------------------------------
// Show
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_OTHER_INFO::Show()
{
	//m_pC_info_spk=C_VS_UI_INFO::m_pC_info_spk;
	
//	assert(m_pC_info_spk);
	const int field_x=22,field_gap=20;

	RECT face_rect={x+field_x,y+25,x+field_x+55,y+25+70};
	RECT grade_rect={x+124+9,y+30,x+40+124+9,y+30+40};
	RECT guildmark_rect={x+field_x+55+9,y+30,x+field_x+55+9+40,y+30+40};
	Rect grade_rect_t(grade_rect.left-2,grade_rect.top-2,44,44);
	RECT field_rect;

	char sz_temp[100];
	int i;

	const static char* align_temp[5] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_VERY_BAD].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_BAD].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_NORMAL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_GOOD].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ALIGN_VERY_GOOD].GetString(),
	};	

	
	
	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);
	// grade mark
	if(gbl_vampire_interface)
	{	
		const static char* grade[10] = 
		{
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_RITTER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_REICHSRITTER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_BARONET].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PREYHER].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_GRAF].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MARKGRAF].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PFALZGRAF].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FURST].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_HERZOG].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LANDESHER].GetString(),
		};
		
		int field_y = 100;
		int bar_height = gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR);	
		int bar_width = gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR),x_gap_from_image=30;
		// 이름 GRADE,TEAM,LEVEL,FAME부분
		
		field_rect.left=x+field_x+x_gap_from_image;
		field_rect.right=field_rect.left+bar_width;
		field_rect.top=y+field_y-3;
		
		for(i=0;i<2;i++)
		{			
			field_rect.bottom=field_rect.top+bar_height;
			gpC_base->m_p_DDSurface_back->FillRect(&field_rect, 0);			
			field_rect.top+=field_gap;
		}
		// STR,DEX,INT 부분
		bar_height = gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::SMALL_BAR2);	
		bar_width = gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SMALL_BAR2);
		field_rect.right=field_rect.left+bar_width-19;		
		for(i=0;i<4;i++)
		{	
			field_rect.bottom=field_rect.top+bar_height;
			gpC_base->m_p_DDSurface_back->FillRect(&field_rect, 0);			
			field_rect.top+=field_gap;
		}

		// align
		field_rect.left=x+92+x_gap_from_image;
		field_rect.right=field_rect.left+bar_width-8;
		field_rect.top-=field_gap*3;
		field_rect.bottom=field_rect.top+bar_height;
		gpC_base->m_p_DDSurface_back->FillRect(&field_rect, 0);
		
		// name
		field_rect.left=x+85;
		field_rect.right=field_rect.left+102;
		field_rect.top=y+field_y-field_gap;
		field_rect.bottom=field_rect.top+gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR);
		gpC_base->m_p_DDSurface_back->FillRect(&field_rect, 0);
		
		if(gpC_base->m_p_DDSurface_back->Lock())
		{				
			POINT guild_point={guildmark_rect.left,guildmark_rect.top};
			RECT guild_rect={guildmark_rect.left,guildmark_rect.top,guildmark_rect.left+40,guildmark_rect.top+40};

//			DEBUG_ADD("[ OtherInfo ] Show Vampire Face");
			// 얼굴 찍기
//			if(m_p_face)
			{
				Rect Face_rect(face_rect.left-2,face_rect.top-2,59,74);
				gpC_global_resource->m_pC_info_spk->BltLocked(face_rect.left-1,face_rect.top-1, C_GLOBAL_RESOURCE::CHAR_BOX);
//				C_SPRITE_PACK *temp_spk = NULL;
//				if(g_pProfileManager->HasProfile(m_player_info.PLAYER_NAME.c_str()))
//				{
//					const char *filename = g_pProfileManager->GetFilename(m_player_info.PLAYER_NAME.c_str());
//					temp_spk = new C_SPRITE_PACK(filename);
//				}
//				
//				if(g_pProfileManager->HasProfileNULL(m_player_info.PLAYER_NAME.c_str()))
//				{
//					if(temp_spk)
//					{
//						DeleteNew(temp_spk);
//						temp_spk = NULL;
//					}
//				}
//				if(temp_spk!=NULL)
//					temp_spk->BltLocked(face_rect.left,face_rect.top,0);
//				else
				{
					// 기본 얼굴창으로 출력을 할때!
					const int _VAMPIRE_OFFSET = 6;
					const int _WOMAN_OFFSET = 3;
					
					int spriteID;
					if(m_player_info.bl_female)
						spriteID = _VAMPIRE_OFFSET+1;
					else
						spriteID = _VAMPIRE_OFFSET;

					if(spriteID >= 0 && spriteID <= 7)
						m_p_face_spk->BltLocked(face_rect.left,face_rect.top,spriteID);			
				}
			}
//			DEBUG_ADD("[ OtherInfo ] Show Vampire Grade");
			POINT grade_point={grade_rect.left,grade_rect.top};
			CSprite* GradeMark=NULL;
			// Grade mark
			if(m_player_info.GRADE > 0 && m_player_info.GRADE <= GRADE_MARK_MAX )		
			{
				GradeMark=g_pGuildMarkManager->GetGradeMark(m_player_info.GRADE-1,gbl_vampire_interface);			
				
				if(GradeMark!=NULL)
				{						
			/*		gpC_base->m_p_DDSurface_back->Unlock();
					gpC_base->m_p_DDSurface_back->HLine(grade_point.x-1,grade_point.y-1,42,GRAY);
					gpC_base->m_p_DDSurface_back->HLine(grade_point.x,grade_point.y+40,41,DARKGRAY);
					gpC_base->m_p_DDSurface_back->VLine(grade_point.x-1,grade_point.y-1,42,GRAY);
					gpC_base->m_p_DDSurface_back->VLine(grade_point.x+40,grade_point.y,40,DARKGRAY);
					
					gpC_base->m_p_DDSurface_back->Lock();*/
					gpC_base->m_p_DDSurface_back->BltSprite(&grade_point, GradeMark);
				}		
			}
			
			DrawAlphaBox(&guild_rect,0,0,2,16);
		/*	gpC_base->m_p_DDSurface_back->Unlock();
			gpC_base->m_p_DDSurface_back->HLine(guild_point.x-1,guild_point.y-1,42,GRAY);
			gpC_base->m_p_DDSurface_back->HLine(guild_point.x,guild_point.y+40,41,DARKGRAY);
			gpC_base->m_p_DDSurface_back->VLine(guild_point.x-1,guild_point.y-1,42,GRAY);
			gpC_base->m_p_DDSurface_back->VLine(guild_point.x+40,guild_point.y,40,DARKGRAY);
			gpC_base->m_p_DDSurface_back->Lock();*/
			// 길드 마크 찍기. 길드 마크는 SetOtherInfo 에서 미리 읽어 온다.
//			DEBUG_ADD("[ OtherInfo ] Show Vampire Guild");
			if(m_p_guild_mark != NULL)
				gpC_base->m_p_DDSurface_back->BltSprite(&guild_point, m_p_guild_mark);

			Rect clip_name(0,0,102,gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR));
			
			field_y-=field_gap;
			gpC_global_resource->m_pC_info_spk->BltLockedClip(x +85, y+field_y-3,clip_name, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR), y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);			
			field_y+=field_gap;
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x-2, y+field_y+2, C_GLOBAL_RESOURCE::TITLE_GRADE2);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image, y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR), y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			field_y+=field_gap;
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x, y+field_y+2, C_GLOBAL_RESOURCE::TITLE_CLAN);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image, y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR), y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			field_y+=field_gap;
			clip_name.Set(0,0,bar_width-22,bar_height);
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x, y+field_y+2, C_GLOBAL_RESOURCE::TITLE_LEVEL);				
			gpC_global_resource->m_pC_info_spk->BltLockedClip(x +field_x+x_gap_from_image, y+field_y-3, clip_name,C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image+bar_width-22, y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			field_y+=field_gap;
			
			for(i=0;i<3;i++)
			{
				gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x, y+field_y+2, C_GLOBAL_RESOURCE::TITLE_STR+i);				
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x +field_x+x_gap_from_image, y+field_y-3, clip_name,C_GLOBAL_RESOURCE::LARGE_BAR);
				gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image+bar_width-22, y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
				field_y+=field_gap;			
			}
			
			field_y-=field_gap*4;
			clip_name.Set(0,0,bar_width-8,bar_height);
			gpC_global_resource->m_pC_info_spk->BltLocked(x+92+x_gap_from_image,y+field_y+2,C_GLOBAL_RESOURCE::TITLE_ALIGN);
			field_y+=field_gap;
			gpC_global_resource->m_pC_info_spk->BltLockedClip(x +92+x_gap_from_image, y+field_y-3, clip_name ,C_GLOBAL_RESOURCE::LARGE_BAR);				
			gpC_global_resource->m_pC_info_spk->BltLocked(x +92+x_gap_from_image+bar_width-8, y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			m_pC_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();

			// Lock 해도 상관없나?-_- 일단 Unlock 상태에서 해줘야 하기 때문에 뒤로 빼놓음.
			// 길드마크 테두리
			if(m_p_guild_mark != NULL)
			{
				gpC_base->m_p_DDSurface_back->HLine(guild_point.x-1,guild_point.y-1,42,GRAY);
				gpC_base->m_p_DDSurface_back->HLine(guild_point.x,guild_point.y+40,41,DARKGRAY);
				gpC_base->m_p_DDSurface_back->VLine(guild_point.x-1,guild_point.y-1,42,GRAY);
				gpC_base->m_p_DDSurface_back->VLine(guild_point.x+40,guild_point.y,40,DARKGRAY);
			}
			// 계급마크 테두리			
			if(GradeMark != NULL)
			{
				gpC_base->m_p_DDSurface_back->HLine(grade_point.x-1,grade_point.y-1,42,GRAY);
				gpC_base->m_p_DDSurface_back->HLine(grade_point.x,grade_point.y+40,41,DARKGRAY);
				gpC_base->m_p_DDSurface_back->VLine(grade_point.x-1,grade_point.y-1,42,GRAY);
				gpC_base->m_p_DDSurface_back->VLine(grade_point.x+40,grade_point.y,40,DARKGRAY);
			}
		}		
		
		g_FL2_GetDC();
		// Text
		// align
		if(m_player_info.ALIGNMENT>=0&&m_player_info.ALIGNMENT<5)
			g_PrintColorStr(x+92+x_gap_from_image+4, y+field_y+1,align_temp[m_player_info.ALIGNMENT], gpC_base->m_chatting_pi, RGB_WHITE);
		
		field_y=80;			
		
		//if(m_player_info.PLAYER_NAME.size()>0)
		if(!m_player_info.PLAYER_NAME.empty())
			g_PrintColorStr(x+85+3,y+field_y+1,m_player_info.PLAYER_NAME.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);		
		field_y+=field_gap;		
		
		// Grade 는 0부터 49까지 들어오는게 아니라 1 부터 50까지 들어오므로 계산시에는 1을 빼서 계산을 해주도록!
		if(m_player_info.GRADE > 0 && m_player_info.GRADE <= GRADE_MARK_MAX )
			g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,grade[((m_player_info.GRADE-1)/5)], gpC_base->m_chatting_pi, RGB_WHITE);
		
		field_y+=field_gap;		
		// 팀이름이 없으면 가입된 길드가 없고, 있으면, 팀 이름 찍어줌
		//if(m_player_info.TEAM_NAME.size()>0)
		if(!m_player_info.TEAM_NAME.empty())
			g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,m_player_info.TEAM_NAME.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);
		else
			g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_JOIN_ANY_CLAN].GetString(), gpC_base->m_chatting_pi, RGB_WHITE);
		field_y+=field_gap;
		
		sprintf(sz_temp,"%d",m_player_info.LEVEL);
		g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		field_y+=field_gap;
		
		sprintf(sz_temp,"%d/%d",m_player_info.STR_PURE,m_player_info.STR_CUR);
		g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		field_y+=field_gap;
		
		sprintf(sz_temp,"%d/%d",m_player_info.DEX_PURE,m_player_info.DEX_CUR);
		g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		field_y+=field_gap;
		
		sprintf(sz_temp,"%d/%d",m_player_info.INT_PURE,m_player_info.INT_CUR);
		g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		field_y+=field_gap;
		
		m_pC_button_group->ShowDescription();
		g_FL2_ReleaseDC();
	} else
	{
		const static char* grade[10] = 
		{
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_PIVATE].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_SERENT].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FEACEL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_LITENA].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_KAINEL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_GENEAL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_FORE_GENEAL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MAJORIS_GENEAL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_CLOEL_GENEAL].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_MARSHAL].GetString(),
		};
		
		int field_y = 100;
		int bar_height = gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR);	
		int bar_width = gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR),x_gap_from_image=30;
		// 이름 GRADE,TEAM,LEVEL,FAME부분
		
		field_rect.left=x+field_x+x_gap_from_image;
		field_rect.right=field_rect.left+bar_width;
		field_rect.top=y+field_y-3;		
		
		for(i=0; i < 4; i++)
		{			
			field_rect.bottom=field_rect.top+bar_height;
			gpC_base->m_p_DDSurface_back->FillRect(&field_rect, 0);			
			field_rect.top+=field_gap;
		}

		// STR,DEX,INT 부분
		bar_height = gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::SMALL_BAR2);	
		bar_width = gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::SMALL_BAR2);
		field_rect.right=field_rect.left+bar_width-19;
		
		for(i=0; i < 3; i++)
		{	
			field_rect.bottom=field_rect.top+bar_height;
			gpC_base->m_p_DDSurface_back->FillRect(&field_rect, 0);			
			field_rect.top+=field_gap;
		}
		// align
		field_rect.left=x+92+x_gap_from_image;
		field_rect.right=field_rect.left+bar_width-8;
		field_rect.top-=field_gap*2;
		field_rect.bottom=field_rect.top+bar_height;
		gpC_base->m_p_DDSurface_back->FillRect(&field_rect, 0);
		
		// name
		field_rect.left=x+85;
		field_rect.right=field_rect.left+102;
		field_rect.top=y+field_y-field_gap;
		field_rect.bottom=field_rect.top+gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR);
		gpC_base->m_p_DDSurface_back->FillRect(&field_rect, 0);

		// 모아서
		if(gpC_base->m_p_DDSurface_back->Lock())
		{	
//			DEBUG_ADD("[ OtherInfo ] Show Slayer Face");
			// 얼굴 
//			if(m_p_face)
			{
				Rect Face_rect(face_rect.left-2,face_rect.top-2,59,74);
				gpC_global_resource->m_pC_info_spk->BltLocked(face_rect.left-1,face_rect.top-1, C_GLOBAL_RESOURCE::CHAR_BOX);
//				C_SPRITE_PACK *temp_spk = NULL;
//				if(g_pProfileManager->HasProfile(m_player_info.PLAYER_NAME.c_str()))
//				{
//					const char *filename = g_pProfileManager->GetFilename(m_player_info.PLAYER_NAME.c_str());
//					temp_spk = new C_SPRITE_PACK(filename);
//				}
//				
//				if(g_pProfileManager->HasProfileNULL(m_player_info.PLAYER_NAME.c_str()))
//				{
//					if(temp_spk)
//					{
//						DeleteNew(temp_spk);
//						temp_spk = NULL;
//					}
//				}
//				if(temp_spk!=NULL)
//					temp_spk->BltLocked(face_rect.left,face_rect.top,0);
//				else
				{
					// 기본 얼굴창으로 출력을 할때!
					const int _VAMPIRE_OFFSET = 6;
					const int _WOMAN_OFFSET = 3;
					
					int spriteID;
					
					if (m_player_info.bl_female) // 3 4 5
						spriteID = max(3,min(m_player_info.face_type-W_FACE1+_WOMAN_OFFSET,5));
					else
						spriteID = max(0,min(m_player_info.face_type-M_FACE1,2));
					
					if(spriteID >= 0)
						m_p_face_spk->BltLocked(face_rect.left,face_rect.top,spriteID);			
				}
			}

			POINT grade_point={grade_rect.left,grade_rect.top};
			CSprite* GradeMark=NULL;

			// Grade mark
			if(m_player_info.GRADE > 0 && m_player_info.GRADE <= GRADE_MARK_MAX )		
			{
				GradeMark=g_pGuildMarkManager->GetGradeMark(m_player_info.GRADE-1,gbl_vampire_interface);				
				if(GradeMark!=NULL)
				{
/*					gpC_base->m_p_DDSurface_back->Unlock();
					gpC_base->m_p_DDSurface_back->HLine(grade_point.x-1,grade_point.y-1,42,GRAY);
					gpC_base->m_p_DDSurface_back->HLine(grade_point.x,grade_point.y+40,41,DARKGRAY);
					gpC_base->m_p_DDSurface_back->VLine(grade_point.x-1,grade_point.y-1,42,GRAY);
					gpC_base->m_p_DDSurface_back->VLine(grade_point.x+40,grade_point.y,40,DARKGRAY);
					gpC_base->m_p_DDSurface_back->Lock();*/
					gpC_base->m_p_DDSurface_back->BltSprite(&grade_point, GradeMark);
				}		
			}

			POINT guild_point={guildmark_rect.left,guildmark_rect.top};
			RECT guild_rect={guildmark_rect.left,guildmark_rect.top,guildmark_rect.left+40,guildmark_rect.top+40};
			
			DrawAlphaBox(&guild_rect,0,0,2,16);
			
			/*gpC_base->m_p_DDSurface_back->Unlock();
			gpC_base->m_p_DDSurface_back->HLine(guild_point.x-1,guild_point.y-1,42,GRAY);
			gpC_base->m_p_DDSurface_back->HLine(guild_point.x,guild_point.y+40,41,DARKGRAY);
			gpC_base->m_p_DDSurface_back->VLine(guild_point.x-1,guild_point.y-1,42,GRAY);
			gpC_base->m_p_DDSurface_back->VLine(guild_point.x+40,guild_point.y,40,DARKGRAY);
			gpC_base->m_p_DDSurface_back->Lock();
*/
			// 길드 마크 출력. 길드 마크는 SetOtherInfo 에서 미리 읽어옴.
			if(m_p_guild_mark!=NULL)
				gpC_base->m_p_DDSurface_back->BltSprite(&guild_point, m_p_guild_mark);

			Rect clip_name(0,0,102,gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::LARGE_BAR));
			
			field_y-=field_gap;			
			gpC_global_resource->m_pC_info_spk->BltLockedClip(x +85, y+field_y-3,clip_name, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR), y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			field_y+=field_gap;
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x-2, y+field_y+2, C_GLOBAL_RESOURCE::TITLE_GRADE);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image, y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR), y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			field_y+=field_gap;
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x-4, y+field_y+2, C_GLOBAL_RESOURCE::TITLE_TEAM);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image, y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR), y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			field_y+=field_gap;
			
			// LEVEL 은 DomainLevel 에서 Cut 해서 찍는다. 흐흐-.-
			clip_name.Set(44,0,gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::TITLE_DOMAINLEVEL)-44,gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::TITLE_DOMAINLEVEL));
			gpC_global_resource->m_pC_info_spk->BltLockedClip(x +field_x-44, y+field_y+2,clip_name, C_GLOBAL_RESOURCE::TITLE_DOMAINLEVEL);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image, y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR), y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			field_y+=field_gap;
			
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x, y+field_y+2, C_GLOBAL_RESOURCE::TITLE_FAME);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image, y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR);
			gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image+gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::LARGE_BAR), y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			field_y+=field_gap;
			
			clip_name.Set(0,0,bar_width-22,bar_height);
			
			for(i=0; i < 3; i++)
			{
				gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x, y+field_y+2, C_GLOBAL_RESOURCE::TITLE_STR+i);				
				gpC_global_resource->m_pC_info_spk->BltLockedClip(x +field_x+x_gap_from_image, y+field_y-3, clip_name,C_GLOBAL_RESOURCE::LARGE_BAR);				
				gpC_global_resource->m_pC_info_spk->BltLocked(x +field_x+x_gap_from_image+bar_width-22, y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
				field_y+=field_gap;			
			}
			
			field_y-=field_gap*3;
			clip_name.Set(0,0,bar_width-8,bar_height);
			gpC_global_resource->m_pC_info_spk->BltLocked(x+92+x_gap_from_image+3,y+field_y+2,C_GLOBAL_RESOURCE::TITLE_ALIGN);
			field_y+=field_gap;
			gpC_global_resource->m_pC_info_spk->BltLockedClip(x +92+x_gap_from_image, y+field_y-3, clip_name,C_GLOBAL_RESOURCE::LARGE_BAR);				
			gpC_global_resource->m_pC_info_spk->BltLocked(x +92+x_gap_from_image+bar_width-8, y+field_y-3, C_GLOBAL_RESOURCE::LARGE_BAR_RIGHT);
			
			m_pC_button_group->Show();
			gpC_base->m_p_DDSurface_back->Unlock();

			// Lock 해도 상관없나?-_- 일단 Unlock 상태에서 해줘야 하기 때문에 뒤로 빼놓음.
			// 길드마크 테두리

			if(m_p_guild_mark!=NULL)
			{
				gpC_base->m_p_DDSurface_back->HLine(guild_point.x-1,guild_point.y-1,42,GRAY);
				gpC_base->m_p_DDSurface_back->HLine(guild_point.x,guild_point.y+40,41,DARKGRAY);
				gpC_base->m_p_DDSurface_back->VLine(guild_point.x-1,guild_point.y-1,42,GRAY);
				gpC_base->m_p_DDSurface_back->VLine(guild_point.x+40,guild_point.y,40,DARKGRAY);
			}

			// 계급 마크 테두리
			if(GradeMark!=NULL)
			{
				gpC_base->m_p_DDSurface_back->HLine(grade_point.x-1,grade_point.y-1,42,GRAY);
				gpC_base->m_p_DDSurface_back->HLine(grade_point.x,grade_point.y+40,41,DARKGRAY);
				gpC_base->m_p_DDSurface_back->VLine(grade_point.x-1,grade_point.y-1,42,GRAY);
				gpC_base->m_p_DDSurface_back->VLine(grade_point.x+40,grade_point.y,40,DARKGRAY);
			}			
		}
		g_FL2_GetDC();
		// Text
		// Align

		if(m_player_info.ALIGNMENT>=0 && m_player_info.ALIGNMENT < 5)
			g_PrintColorStr(x+92+x_gap_from_image+4, y+field_y+1,align_temp[m_player_info.ALIGNMENT], gpC_base->m_chatting_pi, RGB_WHITE);	

		field_y=80;			
		
		// 이름
		//if(m_player_info.PLAYER_NAME.size()>0)###@@@
		if(!m_player_info.PLAYER_NAME.empty())
			g_PrintColorStr(x+85+3,y+field_y+1,m_player_info.PLAYER_NAME.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);
		
		field_y+=field_gap;
		
		// 계급은 1부터 50까지의 값이 들어오므로 계산시 -1해주기.
		// Grade
		if(m_player_info.GRADE > 0 && m_player_info.GRADE <= GRADE_MARK_MAX )		
			g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,grade[(m_player_info.GRADE-1)/5], gpC_base->m_chatting_pi, RGB_WHITE);
		field_y+=field_gap;
		
		// Team Name
		//if(m_player_info.TEAM_NAME.size() > 0)
		if(!m_player_info.TEAM_NAME.empty())
			g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,m_player_info.TEAM_NAME.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);
		else
			g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,(*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_JOIN_ANY_TEAM].GetString(), gpC_base->m_chatting_pi, RGB_WHITE);
		
		field_y+=field_gap;
		
		// SkillDef.h 에 정의되어있는 enum 순서대로.
		// Level
		const static char *SlayerJob[5] = {
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_BLADE2].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_SWORD2].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_GUN2].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_HEAL2].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DOMAIN_ENCHANT2].GetString(),
		};
		
		// 제일 높은 도메인. skilldef.h 에 정의되어 있는 순서대로임

		if(TopDomain>=0 && TopDomain<5)
		{
			sprintf(sz_temp,"%s %s %d",SlayerJob[TopDomain],
				(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CLASS2].GetString(),
				m_player_info.DOMAINLEVEL[TopDomain]);
			g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		}
		field_y+=field_gap;
		
		// 성향
		sprintf(sz_temp,"%d",m_player_info.FAME);
		
		std::string sstr = sz_temp;
		for(int i = 3; i <= 13; i += 4)
			if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
			
		g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,sstr.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);
		field_y+=field_gap;
		
		sprintf(sz_temp,"%d/%d",m_player_info.STR_PURE,m_player_info.STR_CUR);
		g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		field_y+=field_gap;
		
		sprintf(sz_temp,"%d/%d",m_player_info.DEX_PURE,m_player_info.DEX_CUR);
		g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		field_y+=field_gap;
		
		sprintf(sz_temp,"%d/%d",m_player_info.INT_PURE,m_player_info.INT_CUR);
		g_PrintColorStr(x+field_x+x_gap_from_image+3,y+field_y+1,sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
		field_y+=field_gap;
		
		m_pC_button_group->ShowDescription();
		g_FL2_ReleaseDC();
	}		
}

const char*		C_VS_UI_OTHER_INFO::GetCurrentOtherInfoName()
{
	//if(m_player_info.PLAYER_NAME.size()>0)
	if(!m_player_info.PLAYER_NAME.empty())
		return m_player_info.PLAYER_NAME.c_str();
	
	return NULL;
}
//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_OTHER_INFO::Start()
{
	//IsRunning=true;
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::OTHER_INFO));
	
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::OTHER_INFO);
	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}
	
	PI_Processor::Start();
	m_pC_button_group->Init();	
	gpC_window_manager->AppearWindow(this);
	
	ZeroMemory(&m_player_info,sizeof(PLAYER_INFO));
	m_player_info.TEAM_NAME="";
	m_player_info.PLAYER_NAME="";
	
	TopDomain=0;
	m_p_guild_mark = NULL;
	EMPTY_MOVE;
}

void	C_VS_UI_OTHER_INFO::Finish()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::OTHER_INFO, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::OTHER_INFO, Rect(x, y, w, h));
	
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);	
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_OTHER_INFO::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_OTHER_INFO::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);	
}

void	C_VS_UI_OTHER_INFO::SetOtherInfo(PLAYER_INFO &info)
{	
	m_player_info = info;
	TopDomain = 0;

	m_p_guild_mark = NULL;

	// 제일 높은 도메인을 찾는다.
	for( int i = 0 ; i < 5; i++ )
	{
		if(m_player_info.DOMAINLEVEL[TopDomain] < m_player_info.DOMAINLEVEL[i])
			TopDomain = i;
	}
	m_p_guild_mark = g_pGuildMarkManager->GetGuildMark(m_player_info.guild_id);
	
	if(m_p_guild_mark == NULL)
	{
		g_pGuildMarkManager->LoadGuildMark( m_player_info.guild_id );
		m_p_guild_mark = g_pGuildMarkManager->GetGuildMark( m_player_info.guild_id );
	}	
}


//-----------------------------------------------------------------------------
// C_VS_UI_TRACE::C_VS_UI_TRACE
//
//
//-----------------------------------------------------------------------------
C_VS_UI_TRACE::C_VS_UI_TRACE()
{
	status = false;
	m_pC_trace_spk = NULL;
	m_pC_trace_spk = new C_SPRITE_PACK(SPK_SOUL_CHAIN);
	
	Set(RESOLUTION_X/2 - 210/2, RESOLUTION_Y/2 - 242/2, m_pC_trace_spk->GetWidth(MAIN_WINDOW), m_pC_trace_spk->GetHeight(MAIN_WINDOW));
	
	g_RegisterWindow(this);

	AttrTopmost(true);
	AttrKeyboardControl(true);
	
	//공통버튼
	m_pC_button_group = new ButtonGroup(this);
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(17, 227, m_pC_trace_spk->GetWidth(TRACE_START), 
		m_pC_trace_spk->GetHeight(TRACE_START), OK_ID, this, TRACE_START));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(128, 227, m_pC_trace_spk->GetWidth(TRACE_CANCEL),
		m_pC_trace_spk->GetHeight(TRACE_CANCEL), CLOSE_ID, this, TRACE_CANCEL));
//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(180,249, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION),	
//		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION),HELP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION));
	//m_lev_name.SetEditorMode(20);
	m_lev_name.SetPosition(x+30,y+30);
	m_lev_name.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_name.SetByteLimit(10);
	m_lev_name.SetAbsWidth(90);
	m_lev_name.SetInputStringColor(RGB_WHITE);
	m_lev_name.SetCursorColor(RGB_WHITE);
	Attach(&m_lev_name);	
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRACE::~C_VS_UI_TRACE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_TRACE::~C_VS_UI_TRACE()
{
	g_UnregisterWindow(this);
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW, Rect(x, y, w, h));	
	
	if(m_lev_name.IsAcquire())
		m_lev_name.Unacquire();
	gC_vs_ui.AcquireChatting();

	DeleteNew(m_pC_trace_spk);
	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRACE::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRACE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char* m_trace_window_button_string[2] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_START_TRACE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CANCEL_TRACE].GetString(),
	};

	switch(p_button->GetID())
	{
	case CLOSE_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,(void *)m_trace_window_button_string[1],0,0);
		break;
	case OK_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,(void *)m_trace_window_button_string[0],0,0);
		break;		
	}
	
}


//-----------------------------------------------------------------------------
// C_VS_UI_TRACE_WINDOW::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRACE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	switch(p_button->GetID())
	{
	case OK_ID :
		if(status == false)
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_pC_trace_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index+2);
				else
					m_pC_trace_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index+1);
			} else
				m_pC_trace_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index);
		} else
		{
			// 활성화 상태가 아님
			POINT dark = {x+p_button->x,y+p_button->y};
			m_pC_trace_spk->BltDarkness(dark,p_button->m_image_index,1);
		}
		break;
	case CLOSE_ID :
		if(status == false)
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_pC_trace_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index+2);
				else
					m_pC_trace_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index+1);
			} else
				m_pC_trace_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index);
		} else
		{
			POINT dark = {x+p_button->x,y+p_button->y};
			m_pC_trace_spk->BltDarkness(dark,p_button->m_image_index,1);
		}
		break;
	case HELP_ID :
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
			{
				Rect rect(0,1,gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET),
					gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET)-1);
				gpC_global_resource->m_pC_assemble_box_button_spk->BltClip(p_button->x+x, p_button->y+y, rect,p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
			}
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x+x, p_button->y+y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
		} else
			gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x+x, p_button->y+y, p_button->m_image_index);
		break;
	}
	
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRACE::WindowEventReceiver(id_t event)
{
}


//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRACE::Run(id_t id)
{
	// 버튼이 비활성상태이면 리턴!
	if(status == true) return;

	switch (id)
	{
	case OK_ID:		
		{
			char *str=NULL;
			g_Convert_DBCS_Ascii2SingleByte(m_lev_name.GetString(),m_lev_name.Size(),str);
			if(str!=NULL&&status==false)
			{
				m_lev_name.Unacquire();
				m_lev_name.SetInputStringColor(RGB_GRAY);
				status = true;
				name = str;
				char temp[100];
				wsprintf(temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_TRACE].GetString(),name.c_str());
				m_status_msg = temp;
				gpC_base->SendMessage(UI_SEND_NAME_FOR_SOUL_CHAIN,0,0,(void*)name.c_str());				
			}
			DeleteNew(str);
		}
		break;
		
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_TRACE_WINDOW);
		EMPTY_MOVE;
		break;
	case HELP_ID :
		//gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::OTHER_INFO);		
		break;	
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_TRACE::MouseControl(UINT message, int _x, int _y)
{
	if(!m_lev_name.IsAcquire()&&status==false)
		m_lev_name.Acquire();

	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	
	switch(message)
	{
	case M_MOVING:
		break;
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if (gpC_mouse_pointer->GetPickUpItem()==FALSE && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
		}
		break;
	}
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TRACE
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRACE::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_CHAR)
	{
		if (isdigit(key) && m_lev_name.Size() == 0)
			return;
		
		char ignore_char[] = {'~', '`', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_',
			'+', '=', '\\', '|', '[', ']', '{', '}', ';', ':', '\"', '\'', ',', '<', '.', '>',
			'/', '?', ' '};
		
		for (int i=0; i<sizeof(ignore_char); i++)
			if ((char)key == ignore_char[i])
				return;
	}

	Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE:
			Run(CLOSE_ID);
			break;

		case VK_RETURN :
			Run(OK_ID);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Show
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_TRACE::Show()
{
//	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);
	m_pC_trace_spk->Blt(x,y,MAIN_WINDOW);
	m_lev_name.SetPosition(x+93,y+208);
	m_lev_name.Show();
	m_pC_button_group->Show();
	if(!m_status_msg.empty())
	{
		g_PrintColorStr(x+17,y+255,
			m_status_msg.c_str(), gpC_base->m_chatting_pi, RGB_WHITE);
	}
//	m_pC_button_group->ShowDescription();
	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRACE::Start()
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW));	
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW);
	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}	
	PI_Processor::Start();

	m_pC_button_group->Init();
	AttrTopmost(true);
	gpC_window_manager->AppearWindow(this);	
	
	m_lev_name.Acquire();
	m_lev_name.EraseAll();
}

void	C_VS_UI_TRACE::Finish()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW, Rect(x, y, w, h));
	
	PI_Processor::Finish();	
	gpC_window_manager->DisappearWindow(this);
	m_lev_name.Unacquire();
	gC_vs_ui.AcquireChatting();
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_TRACE::Process()
{
	m_pC_button_group->Process();	
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_TRACE::IsPixel(int _x, int _y)
{
	//m_pC_monitor_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), MONITOR_WINDOW);
	return m_pC_trace_spk->IsPixel(SCR2WIN_X(_x),SCR2WIN_Y(_y),MAIN_WINDOW);	
}

bool	C_VS_UI_TRACE::IsStartTrace()
{
	return status;
}

void	C_VS_UI_TRACE::SetCannotTrace()
{
	status = false;
	char temp[100];
	wsprintf(temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_TRACE].GetString(),name.c_str());
	m_status_msg = temp;
	m_lev_name.Acquire();
	m_lev_name.SetInputStringColor(RGB_WHITE);
}



//-----------------------------------------------------------------------------
// C_VS_UI_XMAS_CARD::C_VS_UI_XMAS_CARD
//
//
//-----------------------------------------------------------------------------
C_VS_UI_XMAS_CARD::C_VS_UI_XMAS_CARD(const MItem *pItem)
{
	if(pItem == NULL)
		return;

	m_pItem = (MItem *)pItem;
	
	status = false;
	m_pC_card_spk = NULL;
	m_pC_card_spk = new C_SPRITE_PACK(SPK_XMAS_CARD);
	
	Set(RESOLUTION_X/2 - m_pC_card_spk->GetWidth(MAIN_WINDOW)/2, RESOLUTION_Y/2 - m_pC_card_spk->GetHeight(MAIN_WINDOW)/2, m_pC_card_spk->GetWidth(MAIN_WINDOW), m_pC_card_spk->GetHeight(MAIN_WINDOW));
	
	g_RegisterWindow(this);

	AttrTopmost(true);
	AttrKeyboardControl(true);
	
	//공통버튼
	m_pC_button_group = new ButtonGroup(this);
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(130, 146, m_pC_card_spk->GetWidth(BUTTON_OK), 
		m_pC_card_spk->GetHeight(BUTTON_OK), OK_ID, this, BUTTON_OK));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(165, 146, m_pC_card_spk->GetWidth(BUTTON_CANCEL),
		m_pC_card_spk->GetHeight(BUTTON_CANCEL), CANCEL_ID, this, BUTTON_CANCEL));
	
	m_lev_to.SetByteLimit(14);
	m_lev_to.SetPosition(x+85,y+58);
	m_lev_to.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_to.SetInputStringColor(RGB(0, 150, 0));
	m_lev_to.SetCursorColor(RGB_BLACK);
//	Attach(&m_lev_to);	

	m_lev_message.SetEditorMode(18, 2);
	m_lev_message.SetPosition(x+55,y+80);
	m_lev_message.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_message.SetByteLimit(40);
	m_lev_message.SetAbsWidth(115);
	m_lev_message.SetInputStringColor(RGB(0, 0, 150));
	m_lev_message.SetCursorColor(RGB_BLACK);
//	Attach(&m_lev_message);	

	m_lev_from.SetByteLimit(14);
	m_lev_from.SetPosition(x+115,y+123);
	m_lev_from.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_from.SetCursorColor(RGB_BLACK);
	m_lev_from.SetInputStringColor(RGB(0, 150, 0));
//	Attach(&m_lev_from);	
}

//-----------------------------------------------------------------------------
// C_VS_UI_XMAS_CARD::~C_VS_UI_XMAS_CARD
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_XMAS_CARD::~C_VS_UI_XMAS_CARD()
{
	g_UnregisterWindow(this);
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW, Rect(x, y, w, h));	
	
	m_lev_to.Unacquire();
	m_lev_message.Unacquire();
	m_lev_from.Unacquire();
	gC_vs_ui.AcquireChatting();

	DeleteNew(m_pC_card_spk);
	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_XMAS_CARD::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_XMAS_CARD::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char* m_trace_window_button_string[2] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TREE_OK].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_TREE_CANCEL].GetString(),
	};

	switch(p_button->GetID())
	{
	case CANCEL_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,(void *)m_trace_window_button_string[1],0,0);
		break;
	case OK_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,(void *)m_trace_window_button_string[0],0,0);
		break;		
	}
	
}


//-----------------------------------------------------------------------------
// C_VS_UI_XMAS_CARD_WINDOW::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_XMAS_CARD::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetFocusState())
	{
		if(p_button->GetPressState())
			m_pC_card_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index+2);
		else
			m_pC_card_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index+1);
	} else
		m_pC_card_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index);
	
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_XMAS_CARD::WindowEventReceiver(id_t event)
{
}


//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_XMAS_CARD::Run(id_t id)
{
	// 버튼이 비활성상태이면 리턴!
	if(status == true) return;

	switch (id)
	{
	case OK_ID:		
		{
			char *psz_to, *psz_msg, *psz_from = NULL;

			g_Convert_DBCS_Ascii2SingleByte(m_lev_to.GetString(), m_lev_to.Size(), psz_to);
			g_Convert_DBCS_Ascii2SingleByte(m_lev_message.GetString(), m_lev_message.Size(), psz_msg);
			g_Convert_DBCS_Ascii2SingleByte(m_lev_from.GetString(), m_lev_from.Size(), psz_from);

			if(psz_to != NULL && psz_msg != NULL && psz_from != NULL)
			{
				m_szTreeMessage = "";
				
				m_szTreeMessage += psz_to;
				m_szTreeMessage += "%";

				char *find;
				do{
					find = strrchr(psz_msg, '\n');
					if(find != NULL && *(find+1) == '\0')
						*find = '\0';
				}while(find != NULL && *find == '\0');

				m_szTreeMessage += psz_msg;

				m_szTreeMessage += "%";
				m_szTreeMessage += psz_from;
				
				gpC_base->SendMessage(UI_USE_XMAS_TREE, (int)m_pItem, 0, (void*)m_szTreeMessage.c_str());

				DeleteNew(psz_to);
				DeleteNew(psz_msg);
				DeleteNew(psz_from);
			}
			else
			{
				// 빈항목이 있습니다.
				gpC_base->SendMessage(UI_USE_XMAS_TREE, (int)m_pItem, 0, NULL);
			}
		}
		break;
		
	case CANCEL_ID:
		gpC_base->SendMessage(UI_CLOSE_XMAS_CARD_WINDOW);
		EMPTY_MOVE;
		break;

	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_XMAS_CARD::MouseControl(UINT message, int _x, int _y)
{
//	if(!m_lev_message.IsAcquire())
//		m_lev_message.Acquire();

	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	
	switch(message)
	{
	case M_MOVING:
		break;
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if(_x >= 57 && _x <= 172 && _y >= 58 && _y <= 70)
		{
			m_lev_message.Unacquire();
			m_lev_from.Unacquire();
			m_lev_to.Acquire();
		}
		else if(_x >= 35 && _x <= 200 && _y >= 73 && _y <= 120)
		{
			m_lev_to.Unacquire();
			m_lev_from.Unacquire();
			m_lev_message.Acquire();
		}
		else if(_x >= 77 && _x <= 200 && _y >= 121 && _y <= 140)
		{
			m_lev_to.Unacquire();
			m_lev_message.Unacquire();
			m_lev_from.Acquire();
		}
		else if (gpC_mouse_pointer->GetPickUpItem()==FALSE && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
		}
		break;
	}
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_XMAS_CARD
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_XMAS_CARD::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_CHAR)
	{
//		if (isdigit(key) && m_lev_name.Size() == 0)
//			return;
		
			char ignore_char[] = {'%'};//'~', '`', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_',
//			'+', '=', '\\', '|', '[', ']', '{', '}', ';', ':', '\"', '\'', ',', '<', '.', '>',
//			'/', '?', ' '};
//		
//		if(m_lev_message.GetLineCount() > 2)
//			return;
		
		for (int i=0; i<sizeof(ignore_char); i++)
			if ((char)key == ignore_char[i])
				return;
	}

	Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE:
			Run(CANCEL_ID);
			break;

		case VK_RETURN :
			if(m_lev_to.IsAcquire())
			{
				m_lev_to.Unacquire();
				m_lev_message.Acquire();
			}
			else if(m_lev_message.IsAcquire() && m_lev_message.GetLineCount() > 1)
			{
				m_lev_message.Unacquire();
				m_lev_from.Acquire();
//				m_lev_message.EraseCharacterFrontCursor();
			}
			break;

		case VK_BACK:
			if(m_lev_from.IsAcquire() && m_lev_from.Size() == 0)
			{
				m_lev_from.Unacquire();
				m_lev_message.Acquire();
			}
			else if(m_lev_message.IsAcquire() && m_lev_message.Size() == 0)
			{
				m_lev_message.Unacquire();
				m_lev_to.Acquire();
			}
			break;

		case VK_UP:
			if(m_lev_from.IsAcquire())
			{
				m_lev_from.Unacquire();
				m_lev_message.Acquire();
				m_lev_message.EndCursor();
			}
			else if(m_lev_message.IsAcquire())
			{
				m_lev_message.Unacquire();
				m_lev_to.Acquire();
			}
			break;

		case VK_DOWN:
		case VK_TAB:
			if(m_lev_to.IsAcquire())
			{
				m_lev_to.Unacquire();
				m_lev_message.Acquire();
				m_lev_message.HomeCursor();
			}
			else if(m_lev_message.IsAcquire())
			{
				m_lev_message.Unacquire();
				m_lev_from.Acquire();
			}
			else if(m_lev_from.IsAcquire())
			{
				m_lev_from.Unacquire();
				m_lev_to.Acquire();
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Show
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_XMAS_CARD::Show()
{
//	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);
	m_pC_card_spk->Blt(x,y,MAIN_WINDOW);

	g_FL2_GetDC();

	g_PrintColorStr(x+57, y+58, "Dear.", gpC_base->m_chatting_pi, RGB(0, 150, 0));
	g_PrintColorStr(x+80, y+123, "From.", gpC_base->m_chatting_pi, RGB(0, 150, 0));

	g_FL2_ReleaseDC();

	gpC_base->m_p_DDSurface_back->HLine(x+57, y+58+13, 115, CDirectDraw::Color(100>>3, 200>>3, 100>>3));
	gpC_base->m_p_DDSurface_back->HLine(x+80, y+123+13, 115, CDirectDraw::Color(100>>3, 200>>3, 100>>3));

	gpC_base->m_p_DDSurface_back->HLine(x+50, y+80+13, 121, CDirectDraw::Color(100>>3, 100>>3, 200>>3));
	gpC_base->m_p_DDSurface_back->HLine(x+50, y+80+13+18, 121, CDirectDraw::Color(100>>3, 100>>3, 200>>3));

	m_lev_to.SetPosition(x+85,y+58);
	m_lev_to.Show();
	m_lev_message.SetPosition(x+50,y+80);
	m_lev_message.Show();
	m_lev_from.SetPosition(x+110,y+123);
	m_lev_from.Show();

	m_pC_button_group->Show();

	m_pC_button_group->ShowDescription();
	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_XMAS_CARD::Start()
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW));	
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW);
	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}	
	PI_Processor::Start();

	m_pC_button_group->Init();
	AttrTopmost(true);
	gpC_window_manager->AppearWindow(this);	
	
	m_lev_to.Acquire();
	m_lev_to.EraseAll();
	m_lev_message.EraseAll();
	m_lev_from.EraseAll();
}

void	C_VS_UI_XMAS_CARD::Finish()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW, Rect(x, y, w, h));
	
	PI_Processor::Finish();	
	gpC_window_manager->DisappearWindow(this);
 	m_lev_to.Unacquire();
 	m_lev_message.Unacquire();
 	m_lev_from.Unacquire();
	gC_vs_ui.AcquireChatting();
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_XMAS_CARD::Process()
{
	m_pC_button_group->Process();	
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_XMAS_CARD::IsPixel(int _x, int _y)
{
	//m_pC_monitor_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), MONITOR_WINDOW);
	return m_pC_card_spk->IsPixel(SCR2WIN_X(_x),SCR2WIN_Y(_y),MAIN_WINDOW)
		|| m_pC_card_spk->IsPixel(SCR2WIN_X(_x)-130, SCR2WIN_Y(_y)-146, BUTTON_OK)
		|| m_pC_card_spk->IsPixel(SCR2WIN_X(_x)-165, SCR2WIN_Y(_y)-146, BUTTON_CANCEL);	
}

//------------------------------------------------------------------------------
// C_VS_UI_BRING_FEE
//
// 성지에서 세금을 가져온다.
//------------------------------------------------------------------------------
C_VS_UI_BRING_FEE::C_VS_UI_BRING_FEE(UINT totalfee, UINT bringfee)
{	
	SetTotalFee(totalfee);
	SetBringFee(bringfee);
	
	
	Set(RESOLUTION_X/2 - 340/2, RESOLUTION_Y/2 - 200/2, 340, 200);

	int ok_x = 160,ok_y = h-44;
	int cancel_x=236,cancel_y=h-44;
	m_money_x=135;
	m_money_y=h-75;


	m_pC_button_group = new ButtonGroup(this);
	

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(ok_x,ok_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_OK),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_OK),
		OK_ID,this,C_GLOBAL_RESOURCE::AB_BUTTON_OK));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(cancel_x,cancel_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL),
		CANCEL_ID,this,C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL));
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_money_x, m_money_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_MONEY), 
		MONEY_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_MONEY));

	g_RegisterWindow(this);
	AttrTopmost(true);

	m_pC_dialog_bring_fee = NULL;
}

C_VS_UI_BRING_FEE::~C_VS_UI_BRING_FEE()
{
	DeleteNew(m_pC_dialog_bring_fee);
	DeleteNew(m_pC_button_group);
}

void	C_VS_UI_BRING_FEE::Start()
{
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
	AttrTopmost(true);
}

void	C_VS_UI_BRING_FEE::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);
}

void	C_VS_UI_BRING_FEE::Show()
{
	int total_fee_x = x+35,total_fee_y = y+m_money_y+3-30;
	int bring_fee_x = x+35,bring_fee_y = y+m_money_y+3;
	
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_global_resource->DrawDialogLocked(x,y,w,h,false);
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(total_fee_x+130,total_fee_y-3,C_GLOBAL_RESOURCE::AB_MONEY_BAR);
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(total_fee_x+130,total_fee_y-3 + 30,C_GLOBAL_RESOURCE::AB_MONEY_BAR);
	
		m_pC_button_group->Show();	
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}
/*	RECT rect = {total_fee_x+100,total_fee_y - 4,total_fee_x+160+100,total_fee_y+20 - 4};		
	gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
	gpC_global_resource->DrawOutBox(total_fee_x+100 - 1,total_fee_y -4 - 1,160 + 2,20 + 2);

	rect.top += 30;
	rect.bottom += 30;
	gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
	gpC_global_resource->DrawOutBox(total_fee_x+100 - 1,rect.top ,160 + 2,20 + 2);
*/	
	if(g_FL2_GetDC())
	{		
		// show money
		char money_buf[100];
		wsprintf(money_buf, "%d", m_TotalFee);
		std::string sstr = money_buf;
		
		for(int i = 3; i <= 13; i += 4)
			if(sstr.size() > i)
				sstr.insert(sstr.size()-i, ",");
			
		sprintf(money_buf, "$%s", sstr.c_str());
		//g_Print(x+m_money_button_offset_x+30, y+m_money_button_offset_y+4, money_buf, &gpC_base->m_money_pi);

		int len = max(0,160 - g_GetStringWidth(money_buf,gpC_base->m_money_pi.hfont));

		g_PrintColorStrShadow(total_fee_x,total_fee_y,(*g_pGameStringTable)[UI_STRING_MESSAGE_HOLY_LAND_TOTAL_FEE].GetString(),
			gpC_base->m_dialog_msg_pi,RGB(255,255,255),RGB(20,20,20));

		g_Print(total_fee_x+88 + len,total_fee_y,money_buf,&gpC_base->m_money_pi);

		g_PrintColorStrShadow(bring_fee_x,bring_fee_y,(*g_pGameStringTable)[UI_STRING_MESSAGE_HOLY_LAND_CAN_BRING_FEE].GetString(),
			gpC_base->m_dialog_msg_pi,RGB(255,255,255),RGB(20,20,20));

		money_buf[100];
		wsprintf(money_buf, "%d", m_BringFee);
		sstr = money_buf;
		
		for(i = 3; i <= 13; i += 4)
			if(sstr.size() > i)
				sstr.insert(sstr.size()-i, ",");
			
		sprintf(money_buf, "$%s", sstr.c_str());

		len = max(0,160 - g_GetStringWidth(money_buf,gpC_base->m_money_pi.hfont));

		g_Print(bring_fee_x+88 + len,bring_fee_y,money_buf,&gpC_base->m_money_pi);

		std::string str;

		str = (*g_pGameStringTable)[UI_STRING_MESSAGE_BRING_FEE_MSG].GetString();

		int next=0;
		char sz_string[512];

		int print_x=30+x,vx;
		int py = 40+y;
		const int print_gap = 20;
		const int char_width = g_GetStringWidth("a", gpC_base->m_chatting_pi.hfont);

		vx = print_x;

		while(str.size() > next)
		{
			strcpy(sz_string, str.c_str()+next);
			
			char *sz_string2 = sz_string;
			
			while(*sz_string2 == ' ')		// 앞의 공백제거
			{
				sz_string2++;
				next++;
			}
			
			int cut_pos = (x+w-30 -vx)/char_width;
			
			if(!g_PossibleStringCut(sz_string2, cut_pos))
				cut_pos--;
			sz_string2[cut_pos] = NULL;
			
			char *return_char = NULL;
			if((return_char = strchr(sz_string2, '\n')) != NULL)	// return 처리
			{
				cut_pos = return_char - sz_string2+1;
				sz_string2[cut_pos-1] = NULL;
			}
			
			g_PrintColorStr(vx, py, sz_string2, gpC_base->m_chatting_pi, RGB_WHITE);
			next += cut_pos;
			vx = print_x;
			py += print_gap;
		}

		g_FL2_ReleaseDC();
	}

	m_pC_button_group->ShowDescription();
}

bool	C_VS_UI_BRING_FEE::MouseControl(UINT message, int _x, int _y)
{
	/*char *m_bring_fee_msg[2] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_HOLY_LAND_TOTAL_FEE_DESC].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_HOLY_LAND_CLICK_INPUT_FEE].GetString(),
	};*/
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	return true;
}

void	C_VS_UI_BRING_FEE::KeyboardControl(UINT message, UINT key, long extra)	

{
	Window::KeyboardControl(message, key, extra);
	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_BRING_FEE_WINDOW);
			break;		
		}
	}
}

bool	C_VS_UI_BRING_FEE::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_BRING_FEE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetFocusState())
	{
		if(p_button->GetPressState())
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+20);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+10);
	} else
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index);
}

void	C_VS_UI_BRING_FEE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	char* m_bring_fee_msg[3] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_HOLY_LAND_OK].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CANCEL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_HOLY_LAND_CLICK_INPUT_FEE].GetString(),
	};
	
	switch(p_button->GetID())
	{
	case OK_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,
			(void *)m_bring_fee_msg[0],0,0);		
		break;
	case CANCEL_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,
			(void *)m_bring_fee_msg[1],0,0);		
		break;
	case MONEY_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,
			(void *)m_bring_fee_msg[2],0,0);		
		break;
	}
}

void	C_VS_UI_BRING_FEE::WindowEventReceiver(id_t event)
{
}

void	C_VS_UI_BRING_FEE::Run(id_t id)
{
	switch(id)
	{
	case OK_ID :
		if(m_BringFee <= m_TotalFee && m_BringFee >0)
			gpC_base->SendMessage(UI_SEND_BRING_FEE,(int)m_BringFee);
		break;
	case CANCEL_ID :
		gpC_base->SendMessage(UI_CLOSE_BRING_FEE_WINDOW);
		break;		
	case MONEY_ID :
		m_pC_dialog_bring_fee = new C_VS_UI_MONEY_DIALOG(x+m_money_x, 
			y+m_money_y+gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CHANGE_MONEY), 
			2, 0, Execf_BringFee, DIALOG_OK|DIALOG_CANCEL, 10, 
			C_VS_UI_MONEY_DIALOG::MONEY_BRING_FEE);																						
		m_pC_dialog_bring_fee->Start();
		break;
	}
}

void	C_VS_UI_BRING_FEE::Process()
{
	m_pC_button_group->Process();
}

C_VS_UI_WAR_LIST::C_VS_UI_WAR_LIST()
{
	g_RegisterWindow(this);
	
	AttrTopmost(true);
	AttrKeyboardControl(true);

	if(gbl_vampire_interface)
		m_guild_spk.Open(SPK_VAMPIRE_TEAM);
	else
		m_guild_spk.Open(SPK_SLAYER_TEAM);
	
	Set(RESOLUTION_X/2-m_guild_spk.GetWidth(0)/2, RESOLUTION_Y/2 - m_guild_spk.GetHeight(0)/2, m_guild_spk.GetWidth(0), m_guild_spk.GetHeight(0));
	
	m_vampire_point.x= 0;
	m_vampire_point.y= 0;

	// set button
	m_pC_button_group = new ButtonGroup(this);
	
	
	int help_x = 353, x_x = 367, button_y = 2;
	
	if(gbl_vampire_interface)
	{
		button_y = 20;
		help_x+=20; x_x+=20;
		m_vampire_point.x = 17;
		m_vampire_point.y = 6;
	}	

	m_scroll = 0;
	m_bl_scrolling = false;

	int scroll_x,scroll_up_y,scroll_down_y;

	scroll_x= 20+240+93+m_vampire_point.x+7;
	scroll_up_y = 94+m_vampire_point.y+2;
	scroll_down_y = 94+m_vampire_point.y + 218;
	
	
	//help, x버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + help_x, y + button_y, m_guild_spk.GetWidth(HELP_BUTTON), 
		m_guild_spk.GetHeight(HELP_BUTTON), HELP_ID, this, HELP_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + x_x, y + button_y, m_guild_spk.GetWidth(X_BUTTON), m_guild_spk.GetHeight(X_BUTTON), X_ID, this, X_BUTTON));
			
	//CLOSE버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+w-14, y+h-49, m_guild_spk.GetWidth(CLOSE_BUTTON), m_guild_spk.GetHeight(CLOSE_BUTTON), CLOSE_ID, this, CLOSE_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+129+m_vampire_point.x,y+49+m_vampire_point.y,m_guild_spk.GetWidth(TITLE_WAR_LIST),m_guild_spk.GetHeight(TITLE_WAR_LIST),
		WAR_LIST_ID, this, TITLE_WAR_LIST));

	//scroll up, down 버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + scroll_x, 23+ y + scroll_up_y, m_guild_spk.GetWidth(SCROLL_UP_BUTTON), m_guild_spk.GetHeight(SCROLL_UP_BUTTON), SCROLL_UP_ID, this, SCROLL_UP_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + scroll_x, 23+ y + scroll_down_y, m_guild_spk.GetWidth(SCROLL_DOWN_BUTTON), m_guild_spk.GetHeight(SCROLL_DOWN_BUTTON), SCROLL_DOWN_ID, this, SCROLL_DOWN_BUTTON));
}

C_VS_UI_WAR_LIST::~C_VS_UI_WAR_LIST()
{
	g_UnregisterWindow(this);
	
	DeleteNew( m_pC_button_group );
	
}

//-----------------------------------------------------------------------------
// C_VS_UI_WAR_LIST::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_WAR_LIST::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_WAR_LIST::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_WAR_LIST::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_WAR_LIST::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_WAR_LIST::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();
	
	AttrTopmost(true);
	gpC_window_manager->AppearWindow(this);	
}

//-----------------------------------------------------------------------------
// C_VS_UI_WAR_LIST::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_WAR_LIST::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_WAR_LIST::Show 
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_WAR_LIST::Show()
{
	
	const int print_list = 10;
	int tab_y[3];
	int tab_x[3];
	
	int temp_x = 8;

	if(m_v_war_list.size() > 10)
		temp_x = 0;
	
	tab_x[0] = 20 + m_vampire_point.x + temp_x;
	tab_x[1] = tab_x[0]+220;
	tab_x[2] = tab_x[1]+113;

	int tab_x_len = tab_x[2] - tab_x[0]+1;

	tab_y[0] = 94 + m_vampire_point.y;
	tab_y[1] = tab_y[0] + 23;
	tab_y[2] = tab_y[1] + 207+23;
	// Lock 부분 모으기~

	int scroll_tag_x = x+tab_x[2]+5, scroll_tag_y = y+tab_y[0]+14, scroll_tag_height = 198;
	int gap;



	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_guild_spk.BltLocked(x, y);
		m_guild_spk.BltLocked(x+tab_x[0], y+tab_y[0]+1,BAR_DARKEN);
		m_guild_spk.BltLocked(x+tab_x[0]+(tab_x[1] - tab_x[0])/2-m_guild_spk.GetWidth(INDEX_WAR_CHALLENGER)/2,y+tab_y[0]+6,INDEX_WAR_CHALLENGER);
		m_guild_spk.BltLocked(x+tab_x[1]+(tab_x[2] - tab_x[1])/2-m_guild_spk.GetWidth(INDEX_WAR_DATE)/2,y+tab_y[0]+6,INDEX_WAR_DATE);		
				
		if(m_v_war_list.size()>print_list)
		{			
			m_guild_spk.BltLocked(x+tab_x[2] + 5, y+tab_y[0] + 23 , SCROLL_BAR);
			m_guild_spk.BltLocked(scroll_tag_x+2,scroll_tag_y + 23 +
				max(0, min(scroll_tag_height,m_scroll)),SCROLL_TAG);
		}
		
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	if(!m_v_war_list.empty())
	{
		int list = (m_v_war_list.size()-print_list) * m_scroll / scroll_tag_height;
		
		gap = 0;
		std::vector<WarInfo>::const_iterator		itr;

		itr = m_v_war_list.begin();
		
		while(itr != m_v_war_list.end() && list > -(print_list))
		{
			list--;
			
			if(list >= 0) {
				itr ++;
				continue;
			}
			if(itr->warType == 0) // 동족간				
			{
				// 길드 마크 출력
				CSprite *p_guild_mark;
				p_guild_mark=g_pGuildMarkManager->GetGuildMarkSmall(itr->challengerGuildID);
				if(p_guild_mark==NULL)
				{
					g_pGuildMarkManager->LoadGuildMark(itr->challengerGuildID);
					p_guild_mark = g_pGuildMarkManager->GetGuildMarkSmall(itr->challengerGuildID);
				}
				POINT guild_point = { x+tab_x[0]+2, y+tab_y[1]+gap+2 };
				if(p_guild_mark != NULL)	// 악!! CSprite직접 쓸때는 Lock할것!! by 쑥갓
				{
					if(gpC_base->m_p_DDSurface_back->Lock())
					{
						gpC_base->m_p_DDSurface_back->BltSprite(&guild_point, p_guild_mark);
						gpC_base->m_p_DDSurface_back->Unlock();
					}
				}
				else
				{
					RECT rt={guild_point.x,guild_point.y,guild_point.x+20,guild_point.y+20};
					DrawAlphaBox(&rt, 255, 255, 255, 15);
				}

				g_FL2_GetDC();
				g_PrintColorStr(x+ tab_x[0] + 21 + (tab_x[1] - 21 - tab_x[0])/2 - g_GetStringWidth(itr->challengerGuildName.c_str())/2, y+tab_y[1] + gap + 6, itr->challengerGuildName.c_str(),gpC_base->m_chatting_pi,RGB_BLACK);
				char date[100];
				sprintf(date,"%d/%d/%d %d:00",itr->year,itr->month,itr->day,itr->hour);
				g_PrintColorStr(x+ tab_x[1] + (tab_x[2] - tab_x[1])/2 - g_GetStringWidth(date)/2, y+tab_y[1] + gap + 6, date,gpC_base->m_chatting_pi,RGB_BLACK);				
				g_FL2_ReleaseDC();
			} else
			{
				// 종족간 BAR_HILIGHTED		

				std::string challenger = (*g_pGameStringTable)[UI_STRING_MESSAGE_RACE_WAR].GetString();
				m_guild_spk.BltAlpha(x + tab_x[0] , y + tab_y[1] + gap + 1, BAR_HILIGHTED,12);

				g_FL2_GetDC();
				g_PrintColorStr(x+ tab_x[0] + 21 + (tab_x[1] - 21 - tab_x[0])/2 - g_GetStringWidth(challenger.c_str())/2, y+tab_y[1] + gap + 6, challenger.c_str(),gpC_base->m_chatting_pi,RGB_BLACK);
				char date[100];
				sprintf(date,"%d/%d/%d %d:00",itr->year,itr->month,itr->day,itr->hour);
				g_PrintColorStr(x+ tab_x[1] + (tab_x[2] - tab_x[1])/2 - g_GetStringWidth(date)/2, y+tab_y[1] + gap + 6, date,gpC_base->m_chatting_pi,RGB_BLACK);
				g_FL2_ReleaseDC();				
			}			
			gap+=23;
			itr++;			
		}
	}

	// 
	gpC_base->m_p_DDSurface_back->HLine(x+tab_x[0], y+tab_y[0],tab_x_len,0);
	gpC_base->m_p_DDSurface_back->HLine(x+tab_x[0], y+tab_y[1],tab_x_len,0);
	gpC_base->m_p_DDSurface_back->HLine(x+tab_x[0], y+tab_y[2],tab_x_len,0);

	gpC_base->m_p_DDSurface_back->VLine(x+tab_x[0], y+tab_y[0],tab_y[2] - tab_y[0],0);
	gpC_base->m_p_DDSurface_back->VLine(x+tab_x[1], y+tab_y[0],tab_y[2] - tab_y[0],0);
	gpC_base->m_p_DDSurface_back->VLine(x+tab_x[2], y+tab_y[0],tab_y[2] - tab_y[0],0);
	gpC_base->m_p_DDSurface_back->VLine(x+tab_x[0] + 23, y+tab_y[0] + tab_y[1] - tab_y[0],tab_y[2] - tab_y[0] - (tab_y[1] - tab_y[0]),0);

	gap=23;
	for(int i=0;i<print_list - 1;i++)
	{
		gpC_base->m_p_DDSurface_back->HLine(x+tab_x[0], y+tab_y[1]+gap,tab_x_len,0);		

		gap+=23;
	}

	m_pC_button_group->ShowDescription();
	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// C_VS_UI_WAR_LIST::ShowButtonDescription
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_WAR_LIST::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	//	g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}

//-----------------------------------------------------------------------------
// C_VS_UI_WAR_LIST::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_WAR_LIST::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);	
	
	switch(p_button->GetID())
	{		
	case SCROLL_DOWN_ID :
	case SCROLL_UP_ID :		
		if(m_v_war_list.size() > 10)
		{
			if(p_button->GetPressState())
				m_guild_spk.BltLocked(p_button->x,p_button->y,p_button->m_image_index+1);
			else
				m_guild_spk.BltLocked(p_button->x,p_button->y,p_button->m_image_index);
		}
		break;
	case HELP_ID :
	case X_ID :
		if(p_button->GetPressState())
			m_guild_spk.BltLocked(p_button->x,p_button->y,p_button->m_image_index+1);
		else
			m_guild_spk.BltLocked(p_button->x,p_button->y,p_button->m_image_index);
		break;
	case WAR_LIST_ID :
		m_guild_spk.BltLocked(p_button->x,p_button->y,p_button->m_image_index+1);
		break;
	default :
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_guild_spk.BltLocked(p_button->x,p_button->y+1,p_button->m_image_index+1);
			else
				m_guild_spk.BltLocked(p_button->x,p_button->y,p_button->m_image_index+1);
		} else
			m_guild_spk.BltLocked(p_button->x,p_button->y,p_button->m_image_index);
		break;
	}
}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_WAR_LIST::KeyboardControl(UINT message, UINT key, long extra)
{	
	Window::KeyboardControl(message, key, extra);
	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE: 
			gpC_base->SendMessage(UI_CLOSE_WAR_LIST_WINDOW);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_WAR_LIST::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_WAR_LIST::MouseControl(UINT message, int _x, int _y)
{
	m_pC_button_group->MouseControl(message, _x, _y);

	const int print_list = 9;
	int tab_y[3];
	int tab_x[3];
	
	tab_x[0] = 20 + m_vampire_point.x;
	tab_x[1] = tab_x[0]+240;
	tab_x[2] = tab_x[1]+93;

	int tab_x_len = tab_x[2] - tab_x[0]+1;
	

	tab_y[0] = 94 + m_vampire_point.y;
	tab_y[1] = tab_y[0] + 23;
	tab_y[2] = tab_y[1] + 207;
	// Lock 부분 모으기~

			
	int scroll_tag_x = x+tab_x[2]+5, scroll_tag_y = y+tab_y[0]+14+23, scroll_tag_height = 198;


	switch(message)
	{
	case M_MOVING :
		if(m_bl_scrolling == true)
		{
			int pos = _y - scroll_tag_y;
			
			//m_scroll = (m_v_war_list.size()-9) * pos / scroll_tag_height;
			m_scroll = max(0,min(scroll_tag_height, pos ));
		}		
		break;
	case M_LEFTBUTTON_UP :
		m_bl_scrolling = false;
		break;
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if( _x > scroll_tag_x && _x < scroll_tag_x+m_guild_spk.GetWidth(SCROLL_BAR) &&
			_y > scroll_tag_y && _y < scroll_tag_y+scroll_tag_height && m_v_war_list.size() > 10)
		{
			int pos = _y - scroll_tag_y;
			
			//m_scroll = (m_v_war_list.size()-9) * pos / scroll_tag_height;
			m_scroll = max(0,min(scroll_tag_height, pos ));
			m_bl_scrolling = true;
		}
		break;
	case M_WHEEL_UP:
		Run(SCROLL_UP_ID);
		break;
		
	case M_WHEEL_DOWN:
		Run(SCROLL_DOWN_ID);
		break;
	}

	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_WAR_LIST::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_WAR_LIST::IsPixel(int _x, int _y)
{
	bool re = m_guild_spk.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
	if(re == false)
	{
		if(m_guild_spk.IsPixel(_x - (x+w-14), _y - (y+h-49), CLOSE_BUTTON))
			re = true;
	}	
	return re;
}


void	C_VS_UI_WAR_LIST::AddWarList(const WarInfo	&war_info)
{
	m_v_war_list.push_back(war_info);
}

void	C_VS_UI_WAR_LIST::ClearWarList()
{
	m_v_war_list.clear();
}

void	C_VS_UI_WAR_LIST::Run(id_t id)
{
	int scroll_tag_height = 198;

	switch(id)
	{
	case SCROLL_UP_ID :
		if(m_v_war_list.size()>9)
		{
			int list = (m_v_war_list.size() - 9) * m_scroll / scroll_tag_height;

			if(list > 0)
				m_scroll = scroll_tag_height * (list-1) / (m_v_war_list.size()-9) + 1;			
		}
		break;
	case SCROLL_DOWN_ID :
		if(m_v_war_list.size()>9)
		{
			int list = (m_v_war_list.size() - 9) * m_scroll / scroll_tag_height;

			m_scroll = (scroll_tag_height * min((list+1),m_v_war_list.size()-9)) / (m_v_war_list.size()-9) + 1;
		}
		break;
	case CLOSE_ID :
	case X_ID :
		gpC_base->SendMessage(UI_CLOSE_WAR_LIST_WINDOW);
		break;
	case HELP_ID :		
		break;	
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_BLOOD_BIBLE_STATUS::C_VS_UI_BLOOD_BIBLE_STATUS
//
//
//-----------------------------------------------------------------------------
C_VS_UI_BLOOD_BIBLE_STATUS::C_VS_UI_BLOOD_BIBLE_STATUS()
{
	m_sec = 0;
	m_tickCount = 0;

	g_RegisterWindow(this);	
	AttrPin(true);	
	AttrTopmost(false);	
	g_RegisterWindow(this);

	int window_w = 100;
	int window_h = 290;

	Set(RESOLUTION_X/2-window_w/2, RESOLUTION_Y/2 - window_h/2, window_w, window_h);
	
	//close버튼 좌표 세팅
	int close_x = w-24, close_y = h-19;
	int help_x = w-24-20, help_y = h-19;
	if(gbl_vampire_interface)
	{
		close_x -= 5;
		close_y -= 7;
		help_x -=5;
		help_y -= 7;
	}
	int alpha_x = 6, alpha_y = h-21;
	if(gbl_vampire_interface)
	{
		alpha_x += 5;
		alpha_y -= 5;
	}
	
	//공통버튼
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		PUSHPIN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_x, alpha_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), 
		ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));	

//	memset(&m_BloodBibleStatus,-1,sizeof(BLOOD_BIBLE_STATUS) * 12 );

	for(int i=0;i<12;i++)
	{
		m_BloodBibleStatus[i].name="";
		m_BloodBibleStatus[i].status = BLOOD_BIBLE_DROP;
		m_BloodBibleStatus[i].x = -1;
		m_BloodBibleStatus[i].y = -1;
		m_BloodBibleStatus[i].zone_id = -1;
	}	
}

//-----------------------------------------------------------------------------
// C_VS_UI_BLOOD_BIBLE_STATUS::~C_VS_UI_BLOOD_BIBLE_STATUS
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_BLOOD_BIBLE_STATUS::~C_VS_UI_BLOOD_BIBLE_STATUS()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::BLOOD_BIBLE_WINDOW, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::BLOOD_BIBLE_WINDOW, Rect(x, y, w, h));
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::BLOOD_BIBLE_WINDOW, GetAttributes()->autohide);
	
	
	g_UnregisterWindow(this);
	//C_VS_UI_INFO::m_pC_info_spk
	//m_pC_info_spk=NULL;
	DeleteNew(m_pC_button_group);	
}

//-----------------------------------------------------------------------------
// C_VS_UI_BLOOD_BIBLE_STATUS::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BLOOD_BIBLE_STATUS::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char* m_other_info_button_string[6]=
	{		
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),		
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP].GetString(),		
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),		
	};
	switch(p_button->GetID())
	{
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[4],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[5],0,0);
		break;
	case CLOSE_ID :
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[2],0,0);
		break;
	case HELP_ID :
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[1],0,0);
		break;
	case ALPHA_ID :
		if(GetAttributes()->alpha)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[3],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[0],0,0);
		break;
	}	
}


//-----------------------------------------------------------------------------
// C_VS_UI_BLOOD_BIBLE_STATUS::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BLOOD_BIBLE_STATUS::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	} else
	if(p_button->GetID() == CLOSE_ID)
	{
		gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-5, y+p_button->y-5, C_GLOBAL_RESOURCE::BUTTON_CLOSE_BACK);
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+1);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+2);
		} else
			gpC_global_resource->m_pC_info_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index);

	} else
	if(p_button->GetID() == PUSHPIN_ID)
	{
		if(GetAttributes()->autohide)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
		
	}
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BLOOD_BIBLE_STATUS::WindowEventReceiver(id_t event)
{
}


//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BLOOD_BIBLE_STATUS::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
		break;
		
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;
		
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			AttrAutoHide(ATTRIBUTES_HIDE_NOT);
		else
		{
			AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
		}		
		break;		
		
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_BLOOD_BIBLE_STATUS::MouseControl(UINT message, int _x, int _y)
{
	const int skill_id = 230;
	const int start_x = 13, start_y=20;


	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	
	switch(message)
	{
	case M_MOVING:
		if(_x > start_x && _x < start_x + 70 && _y > start_y && _y < start_y + 12*20)
		{
			COLORREF color;
			int count = (_y - start_y) / 20;

			static char temp_str[2][100];
			static LPSTR str[5] = { 
				temp_str[0],
					temp_str[1],
					NULL,
					NULL,
			};
			
			sprintf(temp_str[0],"%s (%s)",(*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_ARMEGA+count].GetString(),(*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_ARMEGA_ENG+count].GetString());
			if(m_BloodBibleStatus[count].zone_id == -1)
			{
				sprintf(temp_str[1],"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_NONE].GetString());
				g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void*)str,2);
				break;
			}
			sprintf(temp_str[1],"%s (%d,%d)",g_pZoneTable->Get(m_BloodBibleStatus[count].zone_id)->Name.GetString(),m_BloodBibleStatus[count].x,m_BloodBibleStatus[count].y);
			//sprintf(temp_str[2],"%s",(*g_pGameStringTable)[STRING_MESSAGE_BLOOD_BIBLE_BONUS_ARMEGA+count].GetString());
			
			str[2] = (*g_pGameStringTable)[STRING_MESSAGE_BLOOD_BIBLE_BONUS_ARMEGA+count].GetString();

			switch(m_BloodBibleStatus[count].status)
			{
			case BLOOD_BIBLE_NONE :
				color = RGB_WHITE;
				str[3] = (*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_NONE].GetString();
				break;
			case BLOOD_BIBLE_DROP :
				str[3] = (*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_DROP].GetString();
				color = RGB_WHITE;
				break;
			case BLOOD_BIBLE_HAS_SLAYER :
				str[3] = (*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_HAS_SLAYER].GetString();
				color = RGB(173,207,255);
				break;
			case BLOOD_BIBLE_HAS_VAMPIRE :
				str[3] = (*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_HAS_VAMPIRE].GetString();
				color = RGB(255, 150, 150);
				break;
			case BLOOD_BIBLE_SHRINE_SLAYER :
				str[3] = (*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_SLAYER].GetString();
				color = RGB(23,107,255);
				break;
			case BLOOD_BIBLE_SHRINE_VAMPIRE :
				str[3] = (*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_VAMPIRE].GetString();
				color = RGB_RED;
				break;
			default :
				color = RGB_WHITE;
				str[3] = (*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_STATUS_NONE].GetString();
				break;			
			}

			if(m_BloodBibleStatus[count].shrine_race == 0)
				str[4] = (*g_pGameStringTable)[STRING_MESSAGE_SLAYER].GetString();
			else
				str[4] = (*g_pGameStringTable)[STRING_MESSAGE_VAMPIRE].GetString();
			
			static S_BLOOD_BIBLE_DESCRIPTION sbl;

			sbl.sz_name = str[0];
			sbl.sz_place = str[1];
			if(m_BloodBibleStatus[count].name.empty())
				sbl.sz_player = NULL;
			else
				sbl.sz_player = (char*)m_BloodBibleStatus[count].name.c_str();
			sbl.sz_option = str[2];
			sbl.sz_status = str[3];
			sbl.sz_shrine = str[4];
			
			g_descriptor_manager.Set(DID_BLOOD_BIBLE_STATUS, _x+x,_y+y, (void *)&sbl,4,(long)color);
		}
		break;
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if (gpC_mouse_pointer->GetPickUpItem() == false && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
			break;
		}
		break;
	}
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_BLOOD_BIBLE_STATUS
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BLOOD_BIBLE_STATUS::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN && key == VK_ESCAPE)
	{
		//		Run(CLOSE_ID);
	}
}
//-----------------------------------------------------------------------------
// Show
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BLOOD_BIBLE_STATUS::Show()
{
	if(m_sec != 0)
	{
		if(m_sec < (timeGetTime() - m_tickCount)/1000)
		{
			gpC_base->SendMessage(UI_CLOSE_BLOOD_BIBLE_STATUS);
			m_sec = 0;
		}
	}

	const int skill_id = 230;
	const int start_x = x+13, start_y=y+20;
	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);
	
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_button_group->Show();

		int i;
		for(i = 0; i < 12; i++)
		{			
			POINT point = {start_x,start_y + i*20 };
			if(C_VS_UI_SKILL::m_C_spk_mini.GetSize() > skill_id + i)
			{
				gpC_base->m_p_DDSurface_back->BltSprite(&point, &C_VS_UI_SKILL::m_C_spk_mini[skill_id + i]);				
			}
		}
		gpC_base->m_p_DDSurface_back->Unlock();

		for(i = 0; i < 12; i++)
		{			
			POINT point = {start_x,start_y + i*20 };

			BYTE r = 0, g = 0, b = 0;
			WORD color = 0;

			if(m_BloodBibleStatus[i].shrine_race == 0)	// slayer
				b = 255;
			else
				r = 255;

			color = CDirectDraw::Color(r, g, b);

			RECT rect = {point.x, point.y, point.x +17, point.y +17};

			gpC_base->m_p_DDSurface_back->DrawRect(&rect, color);
		}

	}
	
//	for(int i=0;i<12;i++)
//	{
//	}

	g_FL2_GetDC();
	for(int i = 0; i < 12; i++)
	{
		if(m_BloodBibleStatus[i].zone_id != -1)
		{
			COLORREF color = RGB_WHITE;

			switch(m_BloodBibleStatus[i].status)
			{
			case BLOOD_BIBLE_NONE :
			case BLOOD_BIBLE_DROP :
				color = RGB_WHITE;
				break;
			case BLOOD_BIBLE_HAS_SLAYER :
				color = RGB(173,207,255);
				break;

			case BLOOD_BIBLE_SHRINE_SLAYER :
				color = RGB(23,107,255);
				break;

			case BLOOD_BIBLE_HAS_VAMPIRE :
				color = RGB(255, 150, 150);
				break;

			case BLOOD_BIBLE_SHRINE_VAMPIRE :
				color = RGB_RED;
				break;
			}
			g_PrintColorStrShadow( start_x + 22, start_y+4 + i*20,(*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_ARMEGA+i].GetString(),
				gpC_base->m_chatting_pi,color, RGB_BLACK);
		}
		else
			g_PrintColorStrShadow( start_x + 22, start_y + 4 +i*20, (*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_ARMEGA+i].GetString(),
				gpC_base->m_chatting_pi,RGB_GRAY, RGB_BLACK);
			
	}
	m_pC_button_group->ShowDescription();
	g_FL2_ReleaseDC();
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BLOOD_BIBLE_STATUS::Start()
{
	//IsRunning=true;
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::BLOOD_BIBLE_WINDOW));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::BLOOD_BIBLE_WINDOW));
	
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::BLOOD_BIBLE_WINDOW);
	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}
	
	PI_Processor::Start();
	m_pC_button_group->Init();	
	gpC_window_manager->AppearWindow(this);
	
	EMPTY_MOVE;
}

void	C_VS_UI_BLOOD_BIBLE_STATUS::Finish()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::BLOOD_BIBLE_WINDOW, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::BLOOD_BIBLE_WINDOW, Rect(x, y, w, h));
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::BLOOD_BIBLE_WINDOW, GetAttributes()->autohide);
	
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);	
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BLOOD_BIBLE_STATUS::Process()
{
	m_pC_button_group->Process();
	ProcessHide();
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_BLOOD_BIBLE_STATUS::IsPixel(int _x, int _y)
{
//	bool re = m_pC_button_group->IsInRect(_x - x,_y - y) != NULL;

//	if(GetAttributes()->alpha)
//		return re;

	return IsInRect(_x, _y);	
}

void	C_VS_UI_BLOOD_BIBLE_STATUS::SetBloodBible(int blood_bible_id, BLOOD_BIBLE_STATUS &blood_bible_status)
{
	if(blood_bible_id < 0 || blood_bible_id >= 12)				// 피의 성서가 아닐 경우
		return;

	m_BloodBibleStatus[blood_bible_id] = blood_bible_status;
}

void	C_VS_UI_BLOOD_BIBLE_STATUS::UnSetBloodBible(int blood_bible_id)
{
	if(blood_bible_id < 0 || blood_bible_id >= 12)
		return;
	m_BloodBibleStatus[blood_bible_id].zone_id = -1;
}

//------------------------------------------------------------------------------
// C_VS_UI_INPUT_NAME
//
// 성지에서 세금을 가져온다.
//------------------------------------------------------------------------------
C_VS_UI_INPUT_NAME::C_VS_UI_INPUT_NAME(INPUT_NAME_MODE_LIST mode)
{	
	m_Mode = mode;
	Set(RESOLUTION_X/2 - 340/2, RESOLUTION_Y/2 - 267/2, 340, 267);

	int ok_x = 160,ok_y = h-44;
	int cancel_x=236,cancel_y=h-44;
	m_name_x=135;
	m_name_y=h-75;


	m_pC_button_group = new ButtonGroup(this);
	

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(ok_x,ok_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_OK),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_OK),
		OK_ID,this,C_GLOBAL_RESOURCE::AB_BUTTON_OK));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(cancel_x,cancel_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL),
		CANCEL_ID,this,C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL));

	g_RegisterWindow(this);
	AttrTopmost(true);
	AttrKeyboardControl(true);
	


	m_lev_name.SetPosition(x+170,y+193);
	m_lev_name.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_name.SetByteLimit(10);
	m_lev_name.SetAbsWidth(90);
	m_lev_name.SetInputStringColor(RGB_WHITE);
	m_lev_name.SetCursorColor(RGB_WHITE);
	Attach(&m_lev_name);	

	
	m_name = "";
	m_status = "";

	m_Status= INPUT_STATUS_NORMAL;
}

C_VS_UI_INPUT_NAME::~C_VS_UI_INPUT_NAME()
{
	DeleteNew(m_pC_button_group);
}

void	C_VS_UI_INPUT_NAME::Start()
{
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
	AttrTopmost(true);
	m_lev_name.Acquire();
	m_lev_name.EraseAll();
}

void	C_VS_UI_INPUT_NAME::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);

	if(m_lev_name.IsAcquire())			
		m_lev_name.Unacquire();
	gC_vs_ui.AcquireChatting();
}

void	C_VS_UI_INPUT_NAME::Show()
{
	int box_x = 160+x, box_y = 189+y;
	int name_x =190+x, name_y = 193+y;
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_global_resource->DrawDialogLocked(x,y,w,h,g_pUserOption->DefaultAlpha == TRUE);
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(box_x, box_y,C_GLOBAL_RESOURCE::AB_MONEY_BAR);		
	
		m_pC_button_group->Show();	
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	if(g_FL2_GetDC())
	{		
		g_PrintColorStr(x+30, name_y, (*g_pGameStringTable)[UI_STRING_MESSAGE_OTHER_PLAYER_NAME].GetString(), gpC_base->m_chatting_pi, RGB_WHITE);
		
		std::string str;

		switch(m_Mode)
		{
		case INPUT_NAME_BREAK_UP :
			str = (*g_pGameStringTable)[UI_STRING_MESSAGE_BREAK_UP_COUPLE_MSG].GetString();			
			break;
		case INPUT_NAME_HOPE_COUPLE :
			str = (*g_pGameStringTable)[UI_STRING_MESSAGE_HOPE_COUPLE_MSG].GetString();
			break;
		case INPUT_NAME_FORCE_BREAK_UP :
			str = (*g_pGameStringTable)[UI_STRING_MESSAGE_FORCE_BREAK_UP_COUPLE].GetString();
			break;
		default :
			str = "";
			break;
		}		

		int next=0;
		char sz_string[512];

		int print_x=30+x,vx;
		int py = 40+y;
		const int print_gap = 20;
		const int char_width = g_GetStringWidth("a", gpC_base->m_chatting_pi.hfont);

		vx = print_x;

		while(str.size() > next)
		{
			strcpy(sz_string, str.c_str()+next);
			
			char *sz_string2 = sz_string;
			
			while(*sz_string2 == ' ')		// 앞의 공백제거
			{
				sz_string2++;
				next++;
			}
			
			int cut_pos = (x+w-30 -vx)/char_width;
			
			if(!g_PossibleStringCut(sz_string2, cut_pos))
				cut_pos--;
			sz_string2[cut_pos] = NULL;
			
			char *return_char = NULL;
			if((return_char = strchr(sz_string2, '\n')) != NULL)	// return 처리
			{
				cut_pos = return_char - sz_string2+1;
				sz_string2[cut_pos-1] = NULL;
			}
						
			g_PrintColorStr(vx, py, sz_string2, gpC_base->m_chatting_pi, RGB_WHITE);
			next += cut_pos;
			vx = print_x;
			py += print_gap;
		}
		g_FL2_ReleaseDC();
	}

	m_lev_name.SetPosition(name_x,name_y);
	if(m_Status == INPUT_STATUS_NORMAL)
		m_lev_name.SetInputStringColor(RGB_WHITE);
	else
		m_lev_name.SetInputStringColor(RGB_GRAY);
	m_lev_name.Show();


	m_pC_button_group->ShowDescription();

	SHOW_WINDOW_ATTR;
}

bool	C_VS_UI_INPUT_NAME::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	if(m_Status == INPUT_STATUS_NORMAL)
	{
		if(!m_lev_name.IsAcquire())
			m_lev_name.Acquire();
	}		
	else
		m_lev_name.Unacquire();

	return true;
}

void	C_VS_UI_INPUT_NAME::KeyboardControl(UINT message, UINT key, long extra)	
{
	if (message == WM_CHAR)
	{
		if (isdigit(key) && m_lev_name.Size() == 0)
			return;
		
		char ignore_char[] = {'~', '`', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_',
			'+', '=', '\\', '|', '[', ']', '{', '}', ';', ':', '\"', '\'', ',', '<', '.', '>',
			'/', '?', ' '};
		
		for (int i=0; i<sizeof(ignore_char); i++)
			if ((char)key == ignore_char[i])
				return;
	}

	Window::KeyboardControl(message, key, extra);
	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_RETURN :
			{			
				char *str=NULL;
				g_Convert_DBCS_Ascii2SingleByte(m_lev_name.GetString(),m_lev_name.Size(),str);
				if(str != NULL && m_Status == INPUT_STATUS_NORMAL)
				{
					m_name = str;
					gpC_base->SendMessage(UI_SEND_NAME_FOR_COUPLE,(int) m_Mode,0,(void*)m_name.c_str());
				}
				DeleteNew(str);
			}
			break;
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_INPUT_NAME);
			break;		
		}
	}
}

bool	C_VS_UI_INPUT_NAME::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_INPUT_NAME::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == OK_ID || p_button->GetID() == CANCEL_ID)
	{
		if(m_Status == INPUT_STATUS_NORMAL)
		{
			if(p_button->GetFocusState())
			{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+20);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+10);
			} else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index);
		} else
		{
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLockedDarkness(x+p_button->x,y+p_button->y,p_button->m_image_index,1);
		}
	} else
	if(p_button->GetFocusState())
	{
		if(p_button->GetPressState())
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+20);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+10);
	} else
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index);
}

void	C_VS_UI_INPUT_NAME::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	char* m_bring_fee_msg[2] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_COMPLETE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CANCEL].GetString(),
	};
	
	switch(p_button->GetID())
	{
	case OK_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,
			(void *)m_bring_fee_msg[0],0,0);		
		break;
	case CANCEL_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,
			(void *)m_bring_fee_msg[1],0,0);		
		break;
	}
}

void	C_VS_UI_INPUT_NAME::WindowEventReceiver(id_t event)
{
}

void	C_VS_UI_INPUT_NAME::Run(id_t id)
{
	switch(id)
	{
	case OK_ID :
		{			
			char *str=NULL;
			g_Convert_DBCS_Ascii2SingleByte(m_lev_name.GetString(),m_lev_name.Size(),str);
			if(str != NULL && m_Status == INPUT_STATUS_NORMAL)
			{
				m_name = str;
				gpC_base->SendMessage(UI_SEND_NAME_FOR_COUPLE,(int) m_Mode,0,(void*)m_name.c_str());
			}
			DeleteNew(str);
		}
		break;
	case CANCEL_ID :
		gpC_base->SendMessage(UI_CLOSE_INPUT_NAME);
		break;		
	}
}

void	C_VS_UI_INPUT_NAME::Process()
{
	m_pC_button_group->Process();
	
	if(m_Status != INPUT_STATUS_NORMAL )
		m_lev_name.Unacquire();
	else
	{
		if(!m_lev_name.IsAcquire())
			m_lev_name.Acquire();
	}

}


void	C_VS_UI_INPUT_NAME::SetName(const char* name)
{
	if(name != NULL)
	{
		m_name = name;
		m_lev_name.AddString(m_name.c_str());
	}
}

const char* C_VS_UI_INPUT_NAME::GetCurrentName()
{
	char *str=NULL;
	g_Convert_DBCS_Ascii2SingleByte(m_lev_name.GetString(),m_lev_name.Size(),str);
	if(str != NULL)
		m_name = str;
	else
		return NULL;
	 
	DeleteNew(str);

	return m_name.c_str();
}

//------------------------------------------------------------------------------
// C_VS_UI_POPUP_MESSAGE
//
// 팝업창을 띄운다.
//------------------------------------------------------------------------------
C_VS_UI_POPUP_MESSAGE::C_VS_UI_POPUP_MESSAGE(const char *str, POPUP_TYPE type)
{	
	m_Type = type;
	int window_x,window_y;

	if(m_Type == POPUP_WELCOME)
	{
		CRarFile				PackFile;

		PackFile.SetRAR(RPK_TUTORIAL_ETC, RPK_PASSWORD);
		if(PackFile.IsSet())
		{
			char temp[4096];
			ZeroMemory(temp,4096);
			PackFile.Open(str);
			PackFile.GetString(temp,4096);
			m_Str = temp;
			
			PackFile.Release();
		} else
		{
			m_Str = "";
		}
		window_x = 500;
		window_y = 400;
	} 
	else
	{
		m_Str=str;
		window_x = 400;
		window_y = 300;
	}
	
	
	Set(RESOLUTION_X/2 - window_x/2, RESOLUTION_Y/2 - window_y/2, window_x, window_y);

	int ok_x = w-150,ok_y = h-44;
	int cancel_x=w-100,cancel_y=h-44;
	
	m_pC_button_group = new ButtonGroup(this);
	
 
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(cancel_x,cancel_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_OK),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_OK),
		OK_ID,this,C_GLOBAL_RESOURCE::AB_BUTTON_OK));
//
//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(cancel_x,cancel_y,
//		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL),
//		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL),
//		CANCEL_ID,this,C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL));
//
	g_RegisterWindow(this);
	AttrTopmost(true);
}

C_VS_UI_POPUP_MESSAGE::~C_VS_UI_POPUP_MESSAGE()
{
	DeleteNew(m_pC_button_group);
}

void	C_VS_UI_POPUP_MESSAGE::Start()
{
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
	AttrTopmost(true);
}

void	C_VS_UI_POPUP_MESSAGE::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);

}

void	C_VS_UI_POPUP_MESSAGE::Show()
{
	int box_x = 160+x, box_y = 189+y;
	int name_x =190+x, name_y = 193+y;

	const int char_width = g_GetStringWidth("a", gpC_base->m_dialog_msg_pi.hfont);
	

	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_global_resource->DrawDialogLocked(x,y,w,h,g_pUserOption->DefaultAlpha == TRUE);
	
		m_pC_button_group->Show();	
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	if(g_FL2_GetDC())
	{	
		int next=0;
		char sz_string[2048];
		ZeroMemory(sz_string, 2048);

		int print_x=30+x,vx;
		int py = 40+y;
		const int print_gap = 20;
		

		vx = print_x;

		while(m_Str.size() > next)
		{
			if(m_Str.size() - next > 2048)
			{
				memcpy(sz_string,m_Str.c_str()+next,2047);
				sz_string[2047] = '\0';
			}
			else
				strcpy(sz_string, m_Str.c_str()+next);
			
			char *sz_string2 = sz_string;
			
			while(*sz_string2 == ' ')		// 앞의 공백제거
			{
				sz_string2++;
				next++;
			}
			
			int cut_pos = (x+w-30 -vx)/char_width;			
			
			if(!g_PossibleStringCut(sz_string2, cut_pos))
				cut_pos--;
			sz_string2[cut_pos] = NULL;
			
			char *return_char = NULL;
			if((return_char = strstr(sz_string2, "\\n")) != NULL)	// return 처리
			{
				cut_pos = return_char - sz_string2+2;
				sz_string2[cut_pos-2] = NULL;
			}
						
			g_PrintColorStr(vx, py, sz_string2, gpC_base->m_dialog_msg_pi, RGB_WHITE);
			next += cut_pos;
			vx = print_x;
			py += print_gap;
		}
		g_FL2_ReleaseDC();
	}
	
	m_pC_button_group->ShowDescription();

	SHOW_WINDOW_ATTR;
}

bool	C_VS_UI_POPUP_MESSAGE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	
	return true;
}

void	C_VS_UI_POPUP_MESSAGE::KeyboardControl(UINT message, UINT key, long extra)	
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_RETURN :
			gpC_base->SendMessage(UI_CLOSE_POPUP_MESSAGE);
			break;
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_POPUP_MESSAGE);
			break;		
		}
	}
}

bool	C_VS_UI_POPUP_MESSAGE::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_POPUP_MESSAGE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == OK_ID || p_button->GetID() == CANCEL_ID)
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+20);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+10);
		} else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index);
	}
}

void	C_VS_UI_POPUP_MESSAGE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	char* m_bring_fee_msg[2] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
	};
	
	switch(p_button->GetID())
	{
	case OK_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,
			(void *)m_bring_fee_msg[0],0,0);		
		break;
	case CANCEL_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,
			(void *)m_bring_fee_msg[1],0,0);		
		break;
	}
}

void	C_VS_UI_POPUP_MESSAGE::WindowEventReceiver(id_t event)
{
}

void	C_VS_UI_POPUP_MESSAGE::Run(id_t id)
{
	switch(id)
	{
	case OK_ID :
		gpC_base->SendMessage(UI_CLOSE_POPUP_MESSAGE);		
		break;
	case CANCEL_ID :
		gpC_base->SendMessage(UI_CLOSE_POPUP_MESSAGE);
		break;		
	}
}

void	C_VS_UI_POPUP_MESSAGE::Process()
{
	m_pC_button_group->Process();
}
