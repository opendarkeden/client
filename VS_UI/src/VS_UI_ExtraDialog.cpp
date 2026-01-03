// VS_UI_ExtraDialog.cpp

#include "client_PCH.h"

#pragma warning(disable:4786)

#include "VS_UI_ExtraDialog.h"
#include "VS_UI_Filepath.h"
#include "VS_UI_Mouse_pointer.h"

#include "MMoneyManager.h"
#include "MItemOptionTable.h"
#include "VS_UI.h"
#include "MPriceManager.h"
#include "MCreatureTable.h"
#include "UtilityFunction.h"
#include "MGameStringTable.H"
#include "ClientConfig.H"
#include "MNpcTable.h"
extern RECT g_GameRect;
#include <algorithm>

Window* g_desc_dialog_window_id = NULL;
/*

char * C_VS_UI_EDIT_DIALOG::m_sz_question_msg[1] = {"捞 酒捞袍            俺甫 荤矫摆嚼聪鳖?"};

// 林籍贸府 by sigi
//char * C_VS_UI_MONEY_DIALOG::m_sz_question_msg[1] = {"滚副 陛咀阑 涝仿窍技夸."};
//char * C_VS_UI_MONEY_DIALOG::m_sz_question_msg_for_storage[1] = {"焊包且 陛咀阑 涝仿窍技夸."};

char * C_VS_UI_MONEY_DIALOG::m_sz_question_msg[MAX_MONEY_DIALOG_TYPE][1] = {	// by sigi
	{  "滚副 陛咀阑 涝仿窍技夸." } ,
	{  "焊包且 陛咀阑 涝仿窍技夸." } ,
	{  "茫阑 陛咀阑 涝仿窍技夸." } ,
	{  "芭贰且 陛咀阑 涝仿窍技夸." } ,
	{  "哗尘 陛咀阑 涝仿窍技夸." } ,
};

char * C_VS_UI_ASK_DIALOG::m_sz_question_msg[MAX_ASK_DIALOG_TYPE][2] = {	// by sigi
	{  "焊包窃阑 $%d俊 荤矫摆嚼聪鳖?"  },	
	{  "%s 丛苞 芭贰窍摆嚼聪鳖?"  },
	{  "%s 丛狼 览翠阑 扁促赋聪促." ,  "秒家窍矫妨搁 Cancel阑 穿福技夸."   },
//	{  "急琶窍脚 公扁绰 %s涝聪促.", "搬沥窍矫摆嚼聪鳖?" },					// by larosel
	{  "%s 丛捞 寸脚狼 %s俊 啊涝阑 夸没窍绊 乐嚼聪促."  },
	{  "%s 丛捞 寸脚阑 %s俊 檬没窍继嚼聪促."  },
	{  "%s 丛狼 览翠阑 扁促赋聪促.",   "秒家窍矫妨搁 Cancel阑 穿福技夸."  },
	{  "八祸 搬苞甫 茫阑 荐 绝嚼聪促."  },
	{  "捞 酒捞袍阑 Enchant窍矫摆嚼聪鳖?",   "秒家窍矫妨搁 Cancel阑 穿福技夸."  },
};
*/
//-----------------------------------------------------------------------------
// C_VS_UI_EDIT_DIALOG::C_VS_UI_EDIT_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_EDIT_DIALOG::C_VS_UI_EDIT_DIALOG(int _x, int _y, int center_x, int center_y, void (*exec_fp)(C_VS_UI_DIALOG *, id_t), WORD dd_button, int cur_val, int max_val) :
							C_VS_UI_DIALOG(_x, _y, center_x, center_y, exec_fp, dd_button)
{
	m_sz_question_msg[0] =(*g_pGameStringTable)[UI_STRING_MESSAGE_BUY_ITEM_NUM].GetString();
	SetMessage(m_sz_question_msg, 1, SMO_NOFIT);

//	if (digit_count < 0)
//		digit_count = 0;
	if(max_val < 0)
		max_val = 0;

	m_max_val = max_val;	
	m_lev_value.SetDigitOnlyMode(true);
	for(int digit_count = 0, number = max_val; number > 0; number/=10, digit_count++);
	m_lev_value.SetByteLimit(digit_count);
	Attach(&m_lev_value);
	m_default_val = cur_val;

	m_p_image_spk = new C_SPRITE_PACK(SPK_SLAYER_BUY_DIALOG);

	m_buy_dialog_pt.x = m_client_rect.x + 90;
	m_buy_dialog_pt.y = m_client_rect.y + 13;

	// set scroll button rect
	m_scroll_button_up_rt.Set(m_buy_dialog_pt.x+SCROLL_BUTTON_OFFSET_X, m_buy_dialog_pt.y+SCROLL_BUTTON1_OFFSET_Y, SCROLL_BUTTON_WIDTH, SCROLL_BUTTON_HEIGHT);
	m_scroll_button_down_rt.Set(m_buy_dialog_pt.x+SCROLL_BUTTON_OFFSET_X, m_buy_dialog_pt.y+SCROLL_BUTTON2_OFFSET_Y, SCROLL_BUTTON_WIDTH, SCROLL_BUTTON_HEIGHT);

	m_lev_value.SetPosition(m_buy_dialog_pt.x+20-7*(digit_count-1), m_buy_dialog_pt.y+5);
}

//-----------------------------------------------------------------------------
// C_VS_UI_EDIT_DIALOG::~C_VS_UI_EDIT_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_EDIT_DIALOG::~C_VS_UI_EDIT_DIALOG()
{
	m_lev_value.Unacquire();

	DeleteNew(m_p_image_spk);
}

//-----------------------------------------------------------------------------
// C_VS_UI_EDIT_DIALOG::GetValue
//
// 
//-----------------------------------------------------------------------------
int	C_VS_UI_EDIT_DIALOG::GetValue() const
{
	int cur_val;
	char * p_temp = NULL;
	g_Convert_DBCS_Ascii2SingleByte(m_lev_value.GetString(), m_lev_value.Size(), p_temp);
	if (p_temp == NULL)
	{
		// string捞 绝促.
		cur_val = m_default_val;
	}
	else
	{
		cur_val = atoi(p_temp);
	}

	DeleteNewArray(p_temp);

	return cur_val;
}

//-----------------------------------------------------------------------------
// C_VS_UI_EDIT_DIALOG::Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_EDIT_DIALOG::Start()
{
	C_VS_UI_DIALOG::Start();

	m_lev_value.Acquire();
	AttrKeyboardControl(true);

	m_bl_up_pushed = false;
	m_bl_down_pushed = false;
	m_bl_up_focused = false;
	m_bl_down_focused = false;

	char sz_temp[10];	// by sigi (盔贰绰 2看寸 - -;)
	wsprintf(sz_temp, "%d", m_default_val);
	m_lev_value.AddString(sz_temp);

//	m_lev_value.AddString("1");
}

//-----------------------------------------------------------------------------
// C_VS_UI_EDIT_DIALOG::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_EDIT_DIALOG::MouseControl(UINT message, int _x, int _y)
{
	C_VS_UI_DIALOG::MouseControl(message, _x, _y);

	switch (message)
	{
		case M_MOVING:
			m_bl_up_focused = false;
			m_bl_down_focused = false;
			if (m_scroll_button_up_rt.IsInRect(_x, _y))
				m_bl_up_focused = true;

			if (m_scroll_button_down_rt.IsInRect(_x, _y))
				m_bl_down_focused = true;
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			if (m_bl_up_focused)
			{
				m_bl_up_pushed = true;
				ScrollButton(true);
			}
			if (m_bl_down_focused)
			{
				m_bl_down_pushed = true;
				ScrollButton(false);
			}
			break;

		case M_LEFTBUTTON_UP:
			m_bl_up_pushed = false;
			m_bl_down_pushed = false;
			break;
	
	}

	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_EDIT_DIALOG::Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_EDIT_DIALOG::Show()
{
	C_VS_UI_DIALOG::Show();

	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_p_image_spk->BltLocked(m_buy_dialog_pt.x, m_buy_dialog_pt.y, BUY_DIALOG);
		
		int vampire_plus = 6;

		switch(g_eRaceInterface)
		{
		case RACE_SLAYER:
			vampire_plus = 0;
			break;
		}		
		
		if (m_bl_up_focused)
		{
			if(m_bl_up_pushed)
				m_p_image_spk->BltLocked(m_scroll_button_up_rt.x, m_scroll_button_up_rt.y, SLAYER_BUTTON_UP_HILIGHTED_PUSHED+vampire_plus);
			else
				m_p_image_spk->BltLocked(m_scroll_button_up_rt.x, m_scroll_button_up_rt.y, SLAYER_BUTTON_UP_HILIGHTED+vampire_plus);
		}
		else
			m_p_image_spk->BltLocked(m_scroll_button_up_rt.x, m_scroll_button_up_rt.y, SLAYER_BUTTON_UP+vampire_plus);
		
		if (m_bl_down_focused)
		{
			if(m_bl_down_pushed)
				m_p_image_spk->BltLocked(m_scroll_button_down_rt.x, m_scroll_button_down_rt.y, SLAYER_BUTTON_DOWN_HILIGHTED_PUSHED+vampire_plus);
			else
				m_p_image_spk->BltLocked(m_scroll_button_down_rt.x, m_scroll_button_down_rt.y, SLAYER_BUTTON_DOWN_HILIGHTED+vampire_plus);
		}
		else
			m_p_image_spk->BltLocked(m_scroll_button_down_rt.x, m_scroll_button_down_rt.y, SLAYER_BUTTON_DOWN+vampire_plus);
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	Window::ShowWidget();
/*
#ifndef _LIB
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO	surfaceinfo;
		SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());

		rectangle(&surfaceinfo, &m_scroll_button_up_rt, WHITE);
		rectangle(&surfaceinfo, &m_scroll_button_down_rt, BLUE);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
#endif*/
}

//-----------------------------------------------------------------------------
// C_VS_UI_EDIT_DIALOG::ScrollButton
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_EDIT_DIALOG::ScrollButton(bool up)
{
	const _MAX_NUM = m_max_val;

	int cur_val;
	char * p_temp = NULL;
	g_Convert_DBCS_Ascii2SingleByte(m_lev_value.GetString(), m_lev_value.Size(), p_temp);
	if (p_temp == NULL)
	{
		// string捞 绝促. backspace窍绊 沁阑 锭.
		cur_val = m_default_val;
	}
	else
	{
		cur_val = atoi(p_temp);
		m_lev_value.EraseAll();

		if (up)
		{
			if (cur_val < _MAX_NUM)
				cur_val++;
		}
		else
		{
			if (cur_val > 1)
				cur_val--;
		}
	}

	char sz_new_str[50];
	wsprintf(sz_new_str, "%d", cur_val);
	m_lev_value.AddString(sz_new_str);

	DeleteNewArray(p_temp);
}

//-----------------------------------------------------------------------------
// C_VS_UI_EDIT_DIALOG::KeyboardControl
//
// 官肺 箭磊 涝仿且 荐 乐霸 茄促.
//-----------------------------------------------------------------------------
void	C_VS_UI_EDIT_DIALOG::KeyboardControl(UINT message, UINT key, long extra)
{
	char num[100] = {0,};
//	if (message == WM_CHAR)
//		if ((char)key >= '1' && (char)key <= '9')
//		{
//			if(m_max_val < 10)m_lev_value.EraseAll();
//			num[0] = (char)key;
//			m_lev_value.AddString(num);
//		}
	for(int digit_count = 0, number = m_max_val; number > 0; number/=10, digit_count++);
	if (message == WM_CHAR)
		if ((char)key >= '0' && (char)key <= '9')
		{
			if(digit_count <= m_lev_value.Size())m_lev_value.EraseCharacterBegin();
//			num[0] = (char)key;
//			m_lev_value.AddString(num);
		}

	m_lev_value.KeyboardControl(message, key, extra);

	for(digit_count = 0, number = GetValue(); number > 0; number/=10, digit_count++);
	while(digit_count < m_lev_value.Size())m_lev_value.EraseCharacterBegin();
	

	if(GetValue() > m_max_val)
	{
		m_lev_value.EraseAll();
		wsprintf(num, "%d", m_max_val);
		m_lev_value.AddString(num);
	}
	if (message == WM_KEYDOWN)
		switch (key)
		{
			case VK_ESCAPE:
			case VK_RETURN:
				C_VS_UI_DIALOG::KeyboardControl(message, key, extra);
				break;
		}
}

//-----------------------------------------------------------------------------
// C_VS_UI_MONEY_DIALOG::Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_MONEY_DIALOG::Start()
{
	C_VS_UI_DIALOG::Start();

	m_lev_value.Acquire();
	AttrKeyboardControl(true);

	m_bPrintMessage = false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_MONEY_DIALOG::KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_MONEY_DIALOG::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_CHAR)
		if ((char)key == '0')
			if (m_lev_value.Size() == 0)
				return;

	C_VS_UI_DIALOG::KeyboardControl(message, key, extra);

	int _MAX_MONEY = 0;

	// by sigi
	switch (m_type)
	{
		// 焊包窃俊 持扁
		case MONEY_DEPOSIT :
			// 泅犁 啊瘤绊 乐绰 父怒父 持阑 荐 乐绊 焊包窃俊 捣狼 limit捞惑篮 持阑荐 绝促.
			_MAX_MONEY =  min(g_pMoneyManager->GetMoney(), g_pStorage->GetMoneyManager()->GetMaxAddMoney());
			break;

		case MONEY_EXCHANGE:
			_MAX_MONEY = min(g_pMoneyManager->GetMoney(), g_pTradeManager->GetOtherMoneyManager()->GetMaxAddMoney());
			break;

		// 滚府扁
		case MONEY_DROP : 
				_MAX_MONEY = g_pMoneyManager->GetMoney();	
			break;
		
		// 焊包窃俊辑 波郴扁
		case MONEY_WITHDRAW : 
			// 焊包窃俊 乐绰 父怒父 茫阑 荐 乐绊 牢亥配府俊 捣狼 limit捞惑篮 持阑荐 绝促.
			_MAX_MONEY = min(g_pStorage->GetMoneyManager()->GetMoney(), g_pMoneyManager->GetMaxAddMoney());
			break;

		case MONEY_WITHDRAW_FROM_EXCHANGE:
			_MAX_MONEY = g_pTradeManager->GetMyMoneyManager()->GetMoney();
			break;
		case MONEY_BRING_FEE :			// 醚 技陛俊辑 啊瘤绊 乐绰 捣阑 哗具 20撅 救逞绰促...
			_MAX_MONEY = min(gC_vs_ui.GetTotalFee(),g_pMoneyManager->GetMaxAddMoney());
			break;
		case MODIFY_TAX:
			_MAX_MONEY = 10; // 技啦篮 10 鳖瘤父
			break;
		case MONEY_SELL_ITEM:
			_MAX_MONEY = 2000000000;
			break;
		case MONEY_CAMPAIGN_HELP:
			_MAX_MONEY = g_pMoneyManager->GetMoney()/10000;
			break;
	}

	if (GetValue() > _MAX_MONEY)
	{
		char buf[30];
		sprintf(buf, "%d", _MAX_MONEY);
		m_lev_value.EraseAll();
		m_lev_value.AddString(buf);

		if(m_bPrintMessage == false)
		{
			m_bPrintMessage = true;
			switch(m_type)
			{
			case MONEY_DEPOSIT:
				gC_vs_ui.RunDepositLimit();
				break;

			case MONEY_WITHDRAW:
				gC_vs_ui.RunWithdrawLimit();
			case MONEY_BRING_FEE :
				gC_vs_ui.RunBringFeeLimit();
				break;
			}
			
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_MONEY_DIALOG::Show
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_MONEY_DIALOG::Show()
{
	C_VS_UI_DIALOG::Show();

	gpC_global_resource->m_pC_assemble_box_button_spk->Blt(m_money_dialog_pt.x, m_money_dialog_pt.y, C_GLOBAL_RESOURCE::AB_MONEY_BAR);
//	m_p_image_spk->Blt(m_money_dialog_pt.x, m_money_dialog_pt.y, MONEY_DIALOG);

	if(m_type == MONEY_CAMPAIGN_HELP)
	{
		if(g_FL2_GetDC())
		{		
			switch(g_eRaceInterface)
			{
			case RACE_SLAYER:
				g_PrintColorStr(x+185, y+64, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAMPAIGN_HELP_UNITS_SLAYER].GetString(), gpC_base->m_item_name_pi, RGB_WHITE);
				break;

			case RACE_VAMPIRE:
				g_PrintColorStr(x+185, y+64, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAMPAIGN_HELP_UNITS_VAMPIRE].GetString(), gpC_base->m_item_name_pi, RGB_WHITE);
				break;

			case RACE_OUSTERS:
				g_PrintColorStr(x+185, y+64, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAMPAIGN_HELP_UNITS_OUSTERS].GetString(), gpC_base->m_item_name_pi, RGB_WHITE);
				break;		
			}
			g_FL2_ReleaseDC();
		}
		int TempValue = GetValue();
		if(TempValue>0)
		{
			Point TempPoint = m_lev_value.GetPosition();
			char TempBuffer[64];
			sprintf(TempBuffer, "%d0000",TempValue);
			g_PrintColorStr(TempPoint.x, TempPoint.y, TempBuffer, gpC_base->m_item_name_pi, RGB_WHITE);
		}
		else
			Window::ShowWidget();
	}
	else
		Window::ShowWidget();
/*
#ifndef _LIB
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO	surfaceinfo;
		SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());

		rectangle(&surfaceinfo, &m_scroll_button_up_rt, WHITE);
		rectangle(&surfaceinfo, &m_scroll_button_down_rt, BLUE);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
#endif*/
}

//-----------------------------------------------------------------------------
// C_VS_UI_MONEY_DIALOG::GetValue
//
// 
//-----------------------------------------------------------------------------
int	C_VS_UI_MONEY_DIALOG::GetValue() const
{
	__int64 cur_val;
	char * p_temp = NULL;
	g_Convert_DBCS_Ascii2SingleByte(m_lev_value.GetString(), m_lev_value.Size(), p_temp);
	if (p_temp == NULL)
	{
		// string捞 绝促.
		cur_val = 0;
	}
	else
	{
		cur_val = _atoi64(p_temp);
	}

	if (cur_val > 0x7FFFFFFF)
	{
		cur_val = 0x7FFFFFFF;
	}

	DeleteNewArray(p_temp);

	return cur_val;	
}

//-----------------------------------------------------------------------------
// C_VS_UI_MONEY_DIALOG::C_VS_UI_MONEY_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_MONEY_DIALOG::C_VS_UI_MONEY_DIALOG(int _x, int _y, int center_x, int center_y, void (*exec_fp)(C_VS_UI_DIALOG *, id_t), WORD dd_button, int digit_count, TYPE_MONEY_DIALOG type) :	// type 眠啊 by sigi
							C_VS_UI_DIALOG(_x, _y, center_x, center_y, exec_fp, dd_button)
{
	/*	// 林籍贸府 by sigi
	if (!gC_vs_ui.IsRunningStorage())
		SetMessage(m_sz_question_msg, 1);//, SMO_NOFIT);
	else
		SetMessage(m_sz_question_msg_for_storage, 1);//, SMO_NOFIT);
	*/
	m_sz_question_msg[0][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_TRHOW_MONEY_IN_DIALOG].GetString();
	m_sz_question_msg[1][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_SAVE_MONEY_IN_DIALOG].GetString();
	m_sz_question_msg[2][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_BRING_MONEY_IN_DIALOG].GetString();
	m_sz_question_msg[3][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_TRADE_MONEY_IN_DIALOG].GetString();
	m_sz_question_msg[4][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_DIVIDE_MONEY_IN_DIALOG].GetString();	
	m_sz_question_msg[5][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_HOLY_LAND_INPUT_BRING_FEE].GetString();	
	m_sz_question_msg[6][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_MODIFY_TAX].GetString();	
	m_sz_question_msg[7][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_SELL_MONEY_IN_DIALOG].GetString();	
	m_sz_question_msg[8][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_CAMPAIGN_HELP_REQUEST].GetString();	
	

	m_type = type;										

	SetMessage(m_sz_question_msg[type], 1);//, SMO_NOFIT);		// by sigi

	if (digit_count < 0)
		digit_count = 0;

	m_lev_value.SetDigitOnlyMode(true);
	m_lev_value.SetByteLimit(digit_count);
	Attach(&m_lev_value);

	//	m_p_image_spk = new C_SPRITE_PACK(SPK_MONEY_DIALOG);

	m_money_dialog_pt.x = m_client_rect.x + 0;
	m_money_dialog_pt.y = m_client_rect.y + 25;

	m_lev_value.SetPosition(m_money_dialog_pt.x+5, m_money_dialog_pt.y+3);
}

//-----------------------------------------------------------------------------
// C_VS_UI_MONEY_DIALOG::~C_VS_UI_MONEY_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_MONEY_DIALOG::~C_VS_UI_MONEY_DIALOG()
{
//	DeleteNew(m_p_image_spk);
}


void	C_VS_UI_ASK_DIALOG::InitString()
{
	/*	(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_BUY_STORAGE] = "焊包窃阑 $%d俊 荤矫摆嚼聪鳖?";
	(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_TRADE_OTHER_PLAYER]= "%s 丛苞 芭贰窍摆嚼聪鳖?";
	(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_WAIT_OTHER_PLAYER]= "%s 丛狼 览翠阑 扁促赋聪促.";
	(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_CANCEL]	="秒家窍矫妨搁 Cancel阑 穿福技夸.";
	(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_REQUEST_JOIN]= "%s 丛捞 寸脚狼 %s俊 啊涝阑 夸没窍绊 乐嚼聪促.";
	(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_INVITE]= "%s 丛捞 寸脚阑 %s俊 檬没窍继嚼聪促.";
	(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_FIND_RESULT]= "八祸 搬苞甫 茫阑 荐 绝嚼聪促." ;
	(*g_pGameStringTable)[UI_STRING_MESSAGE_ENCHANT_CONFIRM]= "捞 酒捞袍阑 Enchant窍矫摆嚼聪鳖?";

  {  "焊包窃阑 $%d俊 荤矫摆嚼聪鳖?"  },	
	{  "%s 丛苞 芭贰窍摆嚼聪鳖?"  },
	{  "%s 丛狼 览翠阑 扁促赋聪促." ,  "秒家窍矫妨搁 Cancel阑 穿福技夸."   },
	{  "%s 丛捞 寸脚狼 %s俊 啊涝阑 夸没窍绊 乐嚼聪促."  },
	{  "%s 丛捞 寸脚阑 %s俊 檬没窍继嚼聪促."  },
	{  "%s 丛狼 览翠阑 扁促赋聪促.",   "秒家窍矫妨搁 Cancel阑 穿福技夸."  },
	{  "八祸 搬苞甫 茫阑 荐 绝嚼聪促."  },
	{  "捞 酒捞袍阑 Enchant窍矫摆嚼聪鳖?",   "秒家窍矫妨搁 Cancel阑 穿福技夸."  },
*/
	//m_sz_question_msg
	m_sz_question_msg[0][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_BUY_STORAGE].GetString();
	m_sz_question_msg[0][1]="";

	m_sz_question_msg[1][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_TRADE_OTHER_PLAYER].GetString();
	m_sz_question_msg[1][1]="";
	
	m_sz_question_msg[2][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_WAIT_OTHER_PLAYER].GetString();
	m_sz_question_msg[2][1]=(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_CANCEL].GetString();

	m_sz_question_msg[3][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_REQUEST_JOIN].GetString();
	m_sz_question_msg[3][1]="";

	m_sz_question_msg[4][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_INVITE].GetString();
	m_sz_question_msg[4][1]="";

	m_sz_question_msg[5][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_WAIT_OTHER_PLAYER].GetString();
	m_sz_question_msg[5][1]=(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_CANCEL].GetString();

	m_sz_question_msg[6][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_FIND_RESULT].GetString();
	m_sz_question_msg[6][1]="";

	m_sz_question_msg[7][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_ENCHANT_CONFIRM].GetString();
	m_sz_question_msg[7][1]=(*g_pGameStringTable)[UI_STRING_MESSAGE_ENCHANT_CONFIRM_2].GetString();
	m_sz_question_msg[7][2]=(*g_pGameStringTable)[UI_STRING_MESSAGE_PET_RESSURECT].GetString();
	m_sz_question_msg[7][3]=(*g_pGameStringTable)[UI_STRING_MESSAGE_REMOVE_PET_OPTION].GetString();
	m_sz_question_msg[7][4]=(*g_pGameStringTable)[UI_STRING_MESSAGE_PET_MUTANT].GetString();
	
	m_sz_question_msg[8][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_DEPOSIT_LIMIT].GetString();
	m_sz_question_msg[8][1]="";

	m_sz_question_msg[9][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_WITHDRAW_LIMIT].GetString();
	m_sz_question_msg[9][1]="";

	m_sz_question_msg[10][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_BRING_FEE_LIMIT].GetString();
	m_sz_question_msg[10][1]="";
	
	m_sz_question_msg[11][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_TRANS_ITEM].GetString();
	m_sz_question_msg[11][1]="";

	m_sz_question_msg[12][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_USE_PET_FOOD].GetString();
	m_sz_question_msg[12][1]=(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_DETACH].GetString();
	
	m_sz_question_msg[13][0]=(*g_pGameStringTable)[STRING_MESSAGE_KEEP_PETITEM].GetString();
	m_sz_question_msg[13][1]="";

	m_sz_question_msg[14][0]=(*g_pGameStringTable)[STRING_MESSAGE_GET_KEEP_PETITEM].GetString();
	m_sz_question_msg[14][1]="";

	m_sz_question_msg[15][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_USE_SMSITEM].GetString();
	m_sz_question_msg[15][1]="";

	m_sz_question_msg[16][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_USE_NAMINGITEM].GetString();
	m_sz_question_msg[16][1]="";

	m_sz_question_msg[17][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_REQUEST_GET_EVENT_ITEM].GetString();
	m_sz_question_msg[17][1]="";
//---------------------------------------------------friend string : add by viva-----------------------------------
	m_sz_question_msg[18][0]=(*g_pGameStringTable)[UI_STRING_ASK_FRIEND_REQUEST].GetString();
	m_sz_question_msg[18][1]="";

	m_sz_question_msg[19][0]=(*g_pGameStringTable)[UI_STRING_ASK_FRIEND_REFUSE].GetString();
	m_sz_question_msg[19][1]="";

	m_sz_question_msg[20][0]=(*g_pGameStringTable)[UI_STRING_ASK_FRIEND_WAIT].GetString();
	m_sz_question_msg[20][1]="";
	
	m_sz_question_msg[21][0]=(*g_pGameStringTable)[UI_STRING_ASK_FRIEND_EXSIT].GetString();
	m_sz_question_msg[21][1]="";

	m_sz_question_msg[22][0]=(*g_pGameStringTable)[UI_STRING_ASK_FRIEND_BLACK].GetString();
	m_sz_question_msg[22][1]="";

	m_sz_question_msg[23][0]=(*g_pGameStringTable)[UI_STRING_ASK_FRIEND_DELETE].GetString();
	m_sz_question_msg[23][1]="";
}
//-----------------------------------------------------------------------------
// C_VS_UI_ASK_DIALOG::C_VS_UI_ASK_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_ASK_DIALOG::C_VS_UI_ASK_DIALOG(int _x, int _y, int center_x, int center_y, void (*exec_fp)(C_VS_UI_DIALOG *, id_t), WORD dd_button, TYPE_ASK_DIALOG type, int value, void* pValue) :
							C_VS_UI_DIALOG(_x, _y, center_x, center_y, exec_fp, dd_button)
{
	m_sz_question_msg_temp[0] = NULL;	
	m_sz_question_msg_temp[1] = NULL;	

	m_type = type;										

	m_dwTemporayValue = value;
	m_pTemporayValue = pValue;

	InitString();
	char *PartyName = (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_PARTY_NAME].GetString();

	AttrKeyboardControl(true);

	switch (type)
	{	
		// 箭磊啊 窍唱 甸绢啊绰 版快.. - -;
		case ASK_STORAGE_BUY :
		{
			m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+20];
			sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0], value);
			SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
		}
		break;

		case ASK_EXCHANGE :
		{
			const char* pName = (const char*)m_pTemporayValue;

			m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+strlen(pName)+5];
			sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0], pName);
			SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
		}
		break;

		case ASK_EXCHANGE_CANCEL :
		{
			const char* pName = (const char*)m_pTemporayValue;
			m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+strlen(pName)+5];
			m_sz_question_msg_temp[1] = new char [strlen(m_sz_question_msg[type][1])+1];
			sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0], pName);
			sprintf(m_sz_question_msg_temp[1], m_sz_question_msg[type][1]);
			SetMessage(m_sz_question_msg_temp, 2, SMO_NOFIT);	
		}
		break;

//		case ASK_TUTORIAL_EXIT :	// by larosel
//		{
//			const char* pName = (const char*)m_pTemporayValue;
//
//			m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+12];
//			m_sz_question_msg_temp[1] = new char [strlen(m_sz_question_msg[type][1])+1];
//			sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0], pName);
//			sprintf(m_sz_question_msg_temp[1], m_sz_question_msg[type][1]);
//			SetMessage(m_sz_question_msg_temp, 2, SMO_NOFIT);	
//		}
//		break;

		case ASK_PARTY_REQUEST :
		{
			const char* pName = (const char*)m_pTemporayValue;

			m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+strlen(pName)+5];
			sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0], pName, PartyName);
			SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
		}
		break;

		case ASK_PARTY_INVITE :
		{
			const char* pName = (const char*)m_pTemporayValue;

			m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+strlen(pName)+5];
			sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0], pName, PartyName);
			SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
		}
		break;

		case ASK_PARTY_CANCEL :
		{
			const char* pName = (const char*)m_pTemporayValue;

			m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+strlen(pName)+5];
			m_sz_question_msg_temp[1] = new char [strlen(m_sz_question_msg[type][1])+1];
			sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0], pName);
			sprintf(m_sz_question_msg_temp[1], m_sz_question_msg[type][1]);
			SetMessage(m_sz_question_msg_temp, 2, SMO_NOFIT);	
		}
		break;

		case ASK_ENCHANT :
		{
			switch(value)
			{
			case 0:		// 老馆 酒捞袍 牢镁飘
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0]);
				m_sz_question_msg_temp[1] = new char [strlen(m_sz_question_msg[type][1])+1];
				sprintf(m_sz_question_msg_temp[1], m_sz_question_msg[type][1]);
				
				SetMessage(m_sz_question_msg_temp, 2, SMO_NOFIT);	
				break;
				
			case 1:		// 脐 牢镁飘
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0]);
				
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;
				
			case 2:		// 脐 何劝
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][2])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][2]);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;

			case 3:		// 脐 腔府鸥胶
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][3])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][3]);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;

			case 4:		// 脐 函脚
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][4])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][4]);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;


			}
		}
		break;

		case ASK_TRANS_ITEM :
		{
			m_sz_question_msg_temp[0] = new char [strlen( m_sz_question_msg[type][0])+1];
			sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0]);
			SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT );
		}
		break;		

		case ASK_USE_PET_FOOD :
			{
				m_sz_question_msg_temp[0] = new char [strlen( m_sz_question_msg[type][0])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0]);
				m_sz_question_msg_temp[1] = new char [strlen( m_sz_question_msg[type][1])+1];
				sprintf(m_sz_question_msg_temp[1], m_sz_question_msg[type][1]);
				
				SetMessage(m_sz_question_msg_temp, 2, SMO_NOFIT );
			}
			break;		

		case ASK_KEEP_PETITEM:
			m_sz_question_msg_temp[0] = new char [strlen( m_sz_question_msg[type][0])+1];
			sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0]);
			SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT );
			break;
			
		case ASK_GET_KEEP_PETITEM:
			m_sz_question_msg_temp[0] = new char [strlen( m_sz_question_msg[type][0])+1];
			sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0]);
			SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT );
			break;

		case ASK_USE_SMSITEM:
			m_sz_question_msg_temp[0] = new char [strlen( m_sz_question_msg[type][0])+1];
			sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0]);
			SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT );
			break;

			//add by viva--------------------------------------ASK_FRIEND_REQUEST-----------------------
		case ASK_FRIEND_REQUEST :
		case ASK_FRIEND_REFUSE:
		case ASK_FRIEND_WAIT:
		case ASK_FRIEND_EXIST:
		case ASK_FRIEND_BLACK:
		case ASK_FRIEND_DELETE:
			{
				const char* pName = (const char*)m_pTemporayValue;
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+strlen(pName)+5];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0], pName);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;
			}
		//case ASK_FRIEND_REFUSE:
		//	{
		//		const char* pName = (const char*)m_pTemporayValue;
		//		m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+strlen(pName)+5];
		//		sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0], pName);
		//		SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
		//		break;
		//	}
		//case ASK_FRIEND_WAIT:
		//	{
		//		const char* pName = (const char*)m_pTemporayValue;
		//		m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+strlen(pName)+5];
		//		sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0], pName);
		//		SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
		//		break;
		//	}
		//case ASK_FRIEND_EXIST:
		//	{
		//		const char* pName = (const char*)m_pTemporayValue;
		//		m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+strlen(pName)+5];
		//		sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0], pName);
		//		SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
		//		break;
		//	}
		//case ASK_FRIEND_BLACK:
		//	{
		//		const char* pName = (const char*)m_pTemporayValue;
		//		m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+strlen(pName)+5];
		//		sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0], pName);
		//		SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
		//		break;
		//	}
		default :
			SetMessage(m_sz_question_msg[type], 1, SMO_NOFIT);	
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_ASK_DIALOG::~C_VS_UI_ASK_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_ASK_DIALOG::~C_VS_UI_ASK_DIALOG()
{
	if (m_sz_question_msg_temp[0]!=NULL)
	{
		delete [] m_sz_question_msg_temp[0];
		m_sz_question_msg_temp[0] = NULL;
	}
	if (m_sz_question_msg_temp[1]!=NULL)
	{
		delete [] m_sz_question_msg_temp[1];
		m_sz_question_msg_temp[1] = NULL;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_ASK_DIALOG::Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_ASK_DIALOG::Start()
{
	C_VS_UI_DIALOG::Start();
}

//-----------------------------------------------------------------------------
// C_VS_UI_ASK_DIALOG::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_ASK_DIALOG::MouseControl(UINT message, int _x, int _y)
{
	C_VS_UI_DIALOG::MouseControl(message, _x, _y);
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_ASK_DIALOG::KeyboardControl
//
// 官肺 箭磊 涝仿且 荐 乐霸 茄促.
//-----------------------------------------------------------------------------
void	C_VS_UI_ASK_DIALOG::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN)
		switch (key)
		{
			case VK_ESCAPE:
			case VK_RETURN:
				C_VS_UI_DIALOG::KeyboardControl(message, key, extra);
				break;
		}
}

//-----------------------------------------------------------------------------
// C_VS_UI_NPC_DIALOG::C_VS_UI_NPC_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_NPC_DIALOG::C_VS_UI_NPC_DIALOG(void (*exec_fp)(C_VS_UI_DIALOG *, id_t), int sprite_num, const char* pname, int Width, int Height, WORD dd_button, bool IsShowItem):
							C_VS_UI_DIALOG(-1, -1, Width, Height, exec_fp, dd_button)
{
	InitNpcDlg(sprite_num, pname, IsShowItem);
}
//-----------------------------------------------------------------------------
// C_VS_UI_NPC_DIALOG::C_VS_UI_NPC_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_NPC_DIALOG::C_VS_UI_NPC_DIALOG(void (*exec_fp)(C_VS_UI_DIALOG *, id_t), int sprite_num, const char* pname, WORD dd_button):
							C_VS_UI_DIALOG(-1, -1, 6, 3, exec_fp, dd_button)
{
	InitNpcDlg(sprite_num, pname);
}

//-----------------------------------------------------------------------------
// C_VS_UI_NPC_DIALOG::~C_VS_UI_NPC_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_NPC_DIALOG::~C_VS_UI_NPC_DIALOG()
{
}
void C_VS_UI_NPC_DIALOG::InitNpcDlg(int sprite_num, const char* pname , bool IsShowItem)
{
	if(pname != NULL && strlen(pname) > 1)
		m_name = pname; 
	else
		m_name = "";
	if(false == IsShowItem)
	{
		const int g_npc_size = 65;
		const int g_npc_id[g_npc_size] = 
		{
			157, 17, 16, 18, 26, 
			21, 97, 100, 24, 14,
			110, 108, 101, 95, 109,
			103, 27, 98, 15, 20,
			96, 11, 102, 23, 158,
			99, 25, 163, 162, 164,
			12, 13, 22, 159, 160,
			161, 19,
			93, 92, 94, 219, 220,
			253, 254, 255,//榜饭赣*2, 饭海墨
			217,//官配府
			250,251,252,//秋扁炼辆荤
			296, //具付葛配
			297, //单聪胶
			362, 363, 364, 365, 366, //扼季己 4疙+喉扼靛抛其令
			302, 303, 311,//譬配府倔 眠啊
			368, //府胶配绢 NPC 宏府妮扼墨胶 努沸
			370, //靛肺海鸥 渴剖 饭捞红
			427, 428, 429, 430,	// 蜡丰拳 粮 弊府乔胶, 肺后氦胶磐, 赣乔, 墨福膏滚父
		};

		const int g_npc_num[g_npc_size] = 
		{
			1, 2, 3, 4, 5, 
			6, 7, 8, 9, 10,
			11, 12, 13, 14, 15,
			16, 17, 18, 19, 20,
			21, 22, 23, 24, 25,
			26, 27, 28, 29, 30,
			31, 32, 33, 34, 35,
			36, 37,
			38, 39, 40, 41, 42,
			48, 47, 49, //榜饭赣*2, 饭海墨
			43,//官配府
			44, 44, 44,//秋扁炼辆荤
			45, //具付葛配
			46, //单聪胶
			50, 51, 52, 53, 54,//扼季己 4疙+喉扼靛抛其令
			45, 46, 45,
			39, //府胶配绢 NPC 宏府妮扼墨胶 努沸
			35, //靛肺海鸥 渴剖 饭捞红
			55, 56, 57, 58,	// 蜡丰拳 粮 弊府乔胶, 肺后氦胶磐, 赣乔, 墨福膏滚父
		};

		m_id = sprite_num;
		for(int i = 0; i < g_npc_size && g_npc_id[i] != sprite_num; i++);

		NPC_INFO* ni=(*g_pNPCTable).GetData((unsigned int)sprite_num);
		
		if(ni == NULL)
			m_iSprite = -1;
		else
			m_iSprite = ni->SpriteID;
		
	//	if(i == g_npc_size)
	//		m_iSprite = -1;
	//	else
	//		m_iSprite = // g_npc_num[i];	// + GUARD
		

		if(m_iSprite != -1)
		{
			m_p_NPCFace.Init( 2 );		// 烙矫肺 1俺父 loading.. - -;;

			bool re;
			re = m_p_NPCFace.LoadFromFileData( 0, 0, SPK_NPCFACE, SPKI_NPCFACE );
			assert(re);
			re = m_p_NPCFace.LoadFromFileData( 1, m_iSprite, SPK_NPCFACE, SPKI_NPCFACE );
			assert(re);
			
			m_client_rect.x += m_p_NPCFace[0].GetWidth();
			m_client_rect.w -= m_p_NPCFace[0].GetWidth();
		}
	}
	// 2005, 1, 14, sobeit add start - npc 倔奔 措脚 ItemDescription 捞固瘤甫焊咯霖促. - 涅胶飘 东矫...
	else
	{
		if(sprite_num == 0)
		{
			m_iSprite = -1;
		}
		else
		{
			m_iSprite = sprite_num;

			m_p_NPCFace.Init( 1 );		// 烙矫肺 1俺父 loading.. - -;;

			bool re;
			re = m_p_NPCFace.LoadFromFileData( 0, m_iSprite, SPK_ITEM, SPKI_ITEM );
			assert(re);
//			re = m_p_NPCFace.LoadFromFileData( 1, 0, SPK_NPCFACE, SPKI_NPCFACE );
//			assert(re);

			m_client_rect.x += m_p_NPCFace[0].GetWidth();
			m_client_rect.w -= m_p_NPCFace[0].GetWidth();

		}
	}

	// 2005, 1, 14, sobeit add end
}
//-----------------------------------------------------------------------------
// C_VS_UI_NPC_DIALOG::Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_NPC_DIALOG::Start()
{
	C_VS_UI_DIALOG::Start();
	AttrTopmost(true);
}

//-----------------------------------------------------------------------------
// C_VS_UI_NPC_DIALOG::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_NPC_DIALOG::MouseControl(UINT message, int _x, int _y)
{
	C_VS_UI_DIALOG::MouseControl(message, _x, _y);
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_NPC_DIALOG::KeyboardControl
//
// 官肺 箭磊 涝仿且 荐 乐霸 茄促.
//-----------------------------------------------------------------------------
void	C_VS_UI_NPC_DIALOG::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
			case VK_ESCAPE:
			case VK_RETURN:
				if(key == VK_ESCAPE)
				{
					if(m_id == 638 || m_id == 639)
						return;
				}
				C_VS_UI_DIALOG::KeyboardControl(message, key, extra);
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_NPC_DIALOG::KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_NPC_DIALOG::Show()
{
	C_VS_UI_DIALOG::Show();
	
	if(m_iSprite != -1)
	{
		int _x = m_client_rect.x - m_p_NPCFace[0].GetWidth()-10, _y = m_client_rect.y;

		if (gpC_base->m_p_DDSurface_back->Lock())
		{
			POINT point = {_x, _y};
			gpC_base->m_p_DDSurface_back->BltSprite(&point, &m_p_NPCFace[0]);
			if(m_p_NPCFace.GetSize()>1)
			{
				point.x+=17; point.y+=18;
				gpC_base->m_p_DDSurface_back->BltSprite(&point, &m_p_NPCFace[1]);
			}			
			gpC_base->m_p_DDSurface_back->Unlock();
		}

		if(m_id < (*g_pCreatureTable).GetSize())
		{
			
			//char* str = (*g_pCreatureTable)[m_id].Name.GetString();

			char* str = m_name.GetString();
			
			if(str && strcmp(str, ""))g_PrintColorStr(_x + m_p_NPCFace[0].GetWidth()/2 - g_GetStringWidth(str, gpC_base->m_dialog_menu_pi.hfont)/2, _y + m_p_NPCFace[0].GetHeight()+10, str, gpC_base->m_info_pi, RGB_WHITE);
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_DESC_DIALOG::C_VS_UI_DESC_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_DESC_DIALOG::C_VS_UI_DESC_DIALOG(id_t type, void* void_ptr, void* void_ptr2)
					:C_VS_UI_DESC()
{
	g_RegisterWindow(this);
	g_desc_dialog_window_id = this;

	m_bl_main_help = false;
	

	AttrTopmost(true);

	int close_x_offset, close_y_offset;
	int help_x_offset, help_y_offset;

	PrintInfo pi, title_pi;
	COLORREF color, title_color;

	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		close_x_offset = 422;
		close_y_offset = 345;
		help_x_offset = 353;
		help_y_offset = 345;
		break;

	case RACE_VAMPIRE:
		close_x_offset = 431;
		close_y_offset = 350;
		help_x_offset = 363;
		help_y_offset = 350;
		break;

	case RACE_OUSTERS:
		close_x_offset = 431;
		close_y_offset = 350;
		help_x_offset = 363;
		help_y_offset = 350;
		break;
	}
	
	w = 540; h = 405;
	Set(g_GameRect.right/2-w/2, g_GameRect.bottom/2-h/2, w, h);

	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(510, 55, -1, 265));
	std::string filename;

	bool bl_title = false;
//	int sprID = -1;
	int corezapID = -1;
	switch(type)
	{
	case DID_HELP:
		{
			bl_title = true;
			title_pi = gpC_base->m_desc_menu_pi;
			title_color = RGB_YELLOW;
			pi = gpC_base->m_desc_msg_pi;
			color = RGB_WHITE;
			
			m_pack_file.SetRAR(RPK_HELP, RPK_PASSWORD);
			
			switch(int(void_ptr))
			{
			case MAIN:
				{

//					CSpritePack *temp;
					switch(g_eRaceInterface)
					{
					case RACE_SLAYER:
						filename = "MainSlayer.txt";
						break;

					case RACE_VAMPIRE:
						filename = "MainVampire.txt";
						break;

					case RACE_OUSTERS:
						filename = "MainOusters.txt";
						break;
					}

					m_bl_main_help = true;
				}
				break;

			case CHATTING:
				{
					filename = "Chatting.txt";
				}
				break;

			case SKILL:
				{
					filename = "Skill.txt";
				}
				break;
			
			case BATTLE:
				{
					filename = "Battle.txt";
				}
				break;

			case INVENTORY:
				{
					filename = "Inventory.txt";
				}
				break;

			case GEAR:
				{
					switch(g_eRaceInterface)
					{
					case RACE_SLAYER:
						filename = "GearSlayer.txt"; 
						break;

					case RACE_VAMPIRE:
						filename = "GearVampire.txt";
						break;

					case RACE_OUSTERS:
						filename = "GearOusters.txt";
						break;
					}					
				}
				break;

			case INFO:
				{
					switch(g_eRaceInterface)
					{
					case RACE_SLAYER:
						filename = "InfoSlayer.txt";
						break;

					case RACE_VAMPIRE:
						filename = "InfoVampire.txt";
						break;

					case RACE_OUSTERS:
						filename = "InfoOusters.txt";
						break;
					}					
				}
				break;

			case SHOP:
				filename = "Shop.txt";
				break;

			case STORAGE:
				filename = "Storage.txt";
				break;

			case EXCHANGE:
				filename = "Exchange.txt";
				break;

			case PARTY_MANAGER:
				{
					filename = "PartyManager.txt";
				}
				break;

			case GUILD:
				switch(g_eRaceInterface)
				{
				case RACE_SLAYER:
					filename = "Team.txt";
					break;

				case RACE_VAMPIRE:
					filename = "Clan.txt";
					break;

				case RACE_OUSTERS:
					filename = "Guild.txt";
					break;
				}
				break;

			case OTHER_INFO :
				switch(g_eRaceInterface)
				{
				case RACE_SLAYER:
					filename = "OtherInfoSlayer.txt";
					break;

				case RACE_VAMPIRE:
					filename = "OtherInfoVampire.txt";
					break;

				case RACE_OUSTERS:
					filename = "OtherInfoOusters.txt";
					break;
				}
				break;
				
			case TRACE_WINDOW :
				filename = "";				
				break;
			case MINI_GAME_ARROW_TILE:
				filename = "arrowtiles.txt";
				break;
			case MINI_GAME_CRAZY_MINE :
				filename = "crazymine.txt";
				break;
			case CTF_WINDOW:
				filename = "CTF.txt";
				break;
				
			case NICK_NAME_WINDOW:
				filename = "nickname.txt";
				break;
			}
		}
		break;

	case DID_ITEM:
		{
			m_pack_file.SetRAR(RPK_ITEM, RPK_PASSWORD);

			MItem *p_item = (MItem *)void_ptr;
			MItem *p_AddItem = (MItem *)void_ptr2;

			if(p_item == NULL)
				break;

			if(!p_item->IsIdentified())
			{
				Run(CLOSE_ID);
				return;
			}
			
			title_pi = gpC_base->m_item_name_pi;

			if(p_item->IsUniqueItem() || p_item->IsQuestItem())			// Unique Item?
			{
				WORD temp_color;
				if(p_item->IsQuestItem() )
					temp_color = CIndexSprite::ColorSet[g_pClientConfig->QuestItemColorSet+13][31];
				else
					temp_color = CIndexSprite::ColorSet[g_pClientConfig->UniqueItemColorSet+13][31];					
				title_color = RGB(CDirectDraw::Red(temp_color)<<3, CDirectDraw::Green(temp_color)<<3, CDirectDraw::Blue(temp_color)<<3);
			} else
			if(p_item->GetItemOptionListCount() == 2 )			// Rare Item ? 
			{
				title_color = g_pClientConfig->COLOR_NAME_ITEM_RARE_OPTION;
			} 
			// add by Sonic 2006.10.28 增加显示三属性装备为红色
			else if(p_item->GetItemOptionListCount() > 2)
			{
				title_color = g_pClientConfig->COLOR_NAME_VAMPIRE; //Red
			}
			// end by Sonic 2006.10.28 增加显示三属性装备为红色
			else
				title_color = RGB_YELLOW;						// Normal Item
			pi = gpC_base->m_desc_msg_pi;
			color = RGB_WHITE;
			

			std::string temp_string;
			std::string h_name,e_name;
			char sz_temp[200]="";

//			filename  = TXT_ITEMROOT;
			filename += p_item->GetEName();
			filename += ".txt";		
			e_name="( ";

			if(!p_item->IsEmptyItemOptionList()&&!p_item->IsUniqueItem() && !p_item->IsQuestItem())				
			{
				for(int i=0;i<min(2,p_item->GetItemOptionListCount());i++)
				{
					if(p_item->GetItemOptionName(i)!=NULL)
					{
						h_name+=p_item->GetItemOptionName(i);						
						h_name+=" ";						
					}
					if(p_item->GetItemOptionEName(i)!=NULL)
					{						
						e_name+=p_item->GetItemOptionEName(i);
						e_name+=" ";
					}					
				}				
			}
			h_name+=p_item->GetName();
			e_name+=p_item->GetEName();
			e_name+=" )";

			wsprintf(sz_temp, "%s %s",h_name.c_str(),e_name.c_str());			
			// 捞抚捞 呈公 辨搁 临咯林磊.	ReduceString2 绰 第率俊 "..." 嘛绢林扁~ 	by sonee
			ReduceString2(sz_temp,55);			
			SetDescTitle(sz_temp);

			// weight
//			wsprintf(sz_temp, "\t公霸 : %d", p_item->GetWeight());
//			temp_string = sz_temp;
//			m_rep_string.push_back(temp_string);
			
			// Durability
			if(p_item->IsGunItem())
			{
				switch(p_item->GetItemClass())
				{
				case ITEM_CLASS_SG:
					wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_ITEM_CLASS_SG].GetString());
					break;
					
				case ITEM_CLASS_SMG:
					wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_ITEM_CLASS_SMG].GetString());
					break;
					
				case ITEM_CLASS_AR:
					wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_ITEM_CLASS_AR].GetString());
					break;
					
				case ITEM_CLASS_SR:
					wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_ITEM_CLASS_SR].GetString());
					break;
				}
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}

			if (p_item->GetMaxDurability() != -1&&!p_item->IsUniqueItem()&& p_item->GetItemClass() != ITEM_CLASS_VAMPIRE_AMULET
				&& p_item->GetItemClass() != ITEM_CLASS_COUPLE_RING && p_item->GetItemClass() != ITEM_CLASS_VAMPIRE_COUPLE_RING
				&& !p_item->IsQuestItem())
			{
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_DURABILITY].GetString(), p_item->GetMaxDurability());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			// Silver
			if (p_item->GetSilverMax() != -1)
			{
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_SILVERING].GetString(), p_item->GetSilverMax());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			// Damage
			if (p_item->GetMaxDamage() != -1) // damage啊 乐绰啊?
			{
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_DAMAGE].GetString(), p_item->GetMinDamage(), p_item->GetMaxDamage());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			// critical hit
			if (p_item->GetCriticalHit() != -1) // 农府萍拿洒飘啊 乐绰啊?
			{
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_CRITICALHIT].GetString(), p_item->GetCriticalHit());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			// Defense
			if (p_item->GetDefenseValue() != -1)
			{
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_DEFENSE].GetString(), p_item->GetDefenseValue());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			// Protection
			if (p_item->GetProtectionValue() != -1)
			{
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_PROTECTION].GetString(), p_item->GetProtectionValue());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			// TOHIT
			if (p_item->GetToHit() != -1)
			{
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_ACCURACY].GetString(), p_item->GetToHit());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			// Heal point
			if (p_item->GetHealPoint() != -1)
			{
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_HP].GetString(), p_item->GetHealPoint());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			// Mana point
			if (p_item->GetManaPoint() != -1)
			{
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_MP].GetString(), p_item->GetManaPoint());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			// Attack range
			if (p_item->GetReach() != -1 && p_item->GetReach() != 1)
			{
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_RANGE].GetString(), p_item->GetReach());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			// Bullet
			if (p_item->GetMagazineSize() != -1)
			{
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_MAGAZINE_NUM].GetString(), p_item->GetMagazineSize());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			// Pocket size
			if (p_item->GetPocketNumber() != -1)
			{
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_POCKET_NUM].GetString(), p_item->GetPocketNumber());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			// Pile size
//			if (p_item->IsPileItem() == true)
//			{
//				wsprintf(sz_temp, "\t酒捞袍 俺荐 : %d俺", p_item->GetNumber());
//				temp_string = sz_temp;
//				m_rep_string.push_back(temp_string);
//			}
			
			// Add option
			/*if (p_item->GetItemOptionListCount() != 0)
			{
				int option = p_item->GetItemOptionListCount();
				ITEMOPTION_INFO& optionInfo = (*g_pItemOptionTable)[option];
				
				char pPartName[20];
				strcpy(pPartName, ITEMOPTION_INFO::ITEMOPTION_PARTNAME[optionInfo.Part]); // Option捞 利侩登绰 荐摹狼 捞抚
				if(p_item->IsVampireItem() && strstr(pPartName,"MP") != NULL)
					*strstr(pPartName,"MP") = 'H';

				BYTE PlusPoint	= optionInfo.PlusPoint; // 利侩登绰 荐摹
				
				if (pPartName)
				{
					if(!strcmp(pPartName, "Durability"))
					{
						sprintf(sz_temp, "\t可记 : %s +%d", pPartName, PlusPoint-100);
						strcat(sz_temp, "%");
					}
					else
						sprintf(sz_temp, "\t可记 : %s +%d", pPartName, PlusPoint);
					
					temp_string = sz_temp;
					m_rep_string.push_back(temp_string);
				}
			}*/
			BOOL fOptionCheck=FALSE;
			if(!p_item->IsEmptyItemOptionList())
			{				
				std::list<TYPE_ITEM_OPTION>::iterator itr;
				std::list<TYPE_ITEM_OPTION> optionList=p_item->GetItemOptionList();
				
				itr=optionList.begin();
				
				while(itr != optionList.end() && *itr < g_pItemOptionTable->GetSize())
				{
					ITEMOPTION_INFO& optionInfo=(*g_pItemOptionTable)[*itr];
					char pPartName[20];
					strcpy(pPartName,g_pItemOptionTable->ITEMOPTION_PARTNAME[optionInfo.Part]);
					if(p_item->IsVampireItem() && strstr(pPartName,"MP") != NULL)
						*strstr(pPartName,"MP") = 'H';
					
					BYTE PlusPoint	= optionInfo.PlusPoint; // 利侩登绰 荐摹
					
					if (pPartName)
					{
						if(!fOptionCheck)
						{
							if(optionInfo.Part == g_pItemOptionTable->PART_DURABILITY)
							{
								sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION].GetString(), pPartName, PlusPoint-100);
								strcat(sz_temp, "%");
							}
							else
							if(
								optionInfo.Part >= ITEMOPTION_TABLE::PART_STR_TO_DEX &&
								optionInfo.Part <= ITEMOPTION_TABLE::PART_INT_TO_DEX)
							{
								sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION].GetString(), pPartName, PlusPoint);
								strcat(sz_temp, "%");
							}
							else
								sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION].GetString(), pPartName, PlusPoint);
							fOptionCheck=TRUE;							
						} else
						{
							if(optionInfo.Part == g_pItemOptionTable->PART_DURABILITY)
							{
								sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION_EMPTY].GetString(), pPartName, PlusPoint-100);
								strcat(sz_temp, "%");
							}
							else
							if(
								optionInfo.Part >= ITEMOPTION_TABLE::PART_STR_TO_DEX &&
								optionInfo.Part <= ITEMOPTION_TABLE::PART_INT_TO_DEX)
							{
								sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION_EMPTY].GetString(), pPartName, PlusPoint);
								strcat(sz_temp, "%");
							}
							else
								sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION_EMPTY].GetString(), pPartName, PlusPoint);							
						}					
						temp_string=sz_temp;
						m_rep_string.push_back(temp_string);							
					}
					itr++;
				}
			}
			if(!(*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].DefaultOptionList.empty())
			{
				std::list<TYPE_ITEM_OPTION>::iterator itr;
				itr=(*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].DefaultOptionList.begin();
				while(itr != (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].DefaultOptionList.end())
				{
					ITEMOPTION_INFO& optionInfo=(*g_pItemOptionTable)[*itr];
					char pPartName[20];
					strcpy(pPartName,g_pItemOptionTable->ITEMOPTION_PARTNAME[optionInfo.Part]);
					if(p_item->IsVampireItem() && strstr(pPartName,"MP") != NULL)
						*strstr(pPartName,"MP") = 'H';
					
					BYTE PlusPoint	= optionInfo.PlusPoint; // 利侩登绰 荐摹
					
					if (pPartName)
					{
						if(!fOptionCheck)
						{
							if(optionInfo.Part == g_pItemOptionTable->PART_DURABILITY)
							{
								sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION].GetString(), pPartName, PlusPoint-100);
								strcat(sz_temp, "%");
							}
							else
							if(
								optionInfo.Part >= ITEMOPTION_TABLE::PART_STR_TO_DEX &&
								optionInfo.Part <= ITEMOPTION_TABLE::PART_INT_TO_DEX)
							{
								sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION].GetString(), pPartName, PlusPoint);
								strcat(sz_temp, "%");
							}
							else
								sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION].GetString(), pPartName, PlusPoint);
							fOptionCheck=TRUE;							
						} else
						{
							if(optionInfo.Part == g_pItemOptionTable->PART_DURABILITY)
							{
								sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION_EMPTY].GetString(), pPartName, PlusPoint-100);
								strcat(sz_temp, "%");
							}
							else
							if(
								optionInfo.Part >= ITEMOPTION_TABLE::PART_STR_TO_DEX &&
								optionInfo.Part <= ITEMOPTION_TABLE::PART_INT_TO_DEX)
							{
								sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION_EMPTY].GetString(), pPartName, PlusPoint);
								strcat(sz_temp, "%");
							}
							else
								sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION_EMPTY].GetString(), pPartName, PlusPoint);							
						}					
						temp_string=sz_temp;
						m_rep_string.push_back(temp_string);							
					}
					itr++;
				}				
			}
			if(void_ptr2!=NULL)
			{
				char sz_buf1 [50];
				MItem* p_AddItem = (MItem*)void_ptr2;
			
				// 内绢漓狼 ItemDescription.spk 俊辑狼 牢郸胶甫 掘绢坷磊
				std::string AddedFileName;
				std::string temp_string;
				AddedFileName += p_AddItem->GetEName();
				AddedFileName += ".txt";
		
				m_pack_file.Open(AddedFileName.c_str());
				m_pack_file.GetString(sz_buf1, 50);
				temp_string = sz_buf1;

				corezapID = atoi(temp_string.c_str()+1);
				// 内绢漓狼 ItemDescription.spk 俊辑狼 牢郸胶甫 掘绢坷磊
		
				//corezapID = 
				if(p_AddItem->GetItemClass() == ITEM_CLASS_CORE_ZAP && p_AddItem->GetItemType()>=0 && p_AddItem->GetItemType()<=3)
				{
					const std::list<TYPE_ITEM_OPTION> &optionList=p_item->GetItemOptionList();
					const std::list<TYPE_ITEM_OPTION> &DefaultOptionList = p_item->GetItemDefaultOptionList();
					if(optionList.size() ==0 && DefaultOptionList.size() == 0)
					{
						sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_CORE_ZAP_BLACK+p_AddItem->GetItemType()].GetString(), p_AddItem->GetGrade());
						sprintf(sz_buf1, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION2].GetString(), sz_temp);
						temp_string=sz_buf1;
						m_rep_string.push_back(temp_string);
						
						//vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OPTION].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);

					}
					else
					{
						sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_CORE_ZAP_BLACK+p_AddItem->GetItemType()].GetString(), p_AddItem->GetGrade());
						sprintf(sz_buf1, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION_EMPTY2].GetString(), sz_temp);
						temp_string=sz_buf1;
						m_rep_string.push_back(temp_string);
					}					
					DWORD TempColor = RGB_DARKGRAY;
					if(gC_vs_ui.IsHasAllCoreZap())
						TempColor = RGB_GREEN;
					sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_CORE_ZAP_REWARD_ALL_STAT].GetString(), 3);
					sprintf(sz_buf1, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION_EMPTY2].GetString(), sz_temp);
					temp_string=sz_buf1;
					m_rep_string.push_back(temp_string);
					sprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_CORE_ZAP_REWARD_ALL_REG].GetString(), 9);
					sprintf(sz_buf1, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_OPTION_EMPTY2].GetString(), sz_temp);
					temp_string=sz_buf1;
					m_rep_string.push_back(temp_string);
					
				}
			}
			
			bool bl_required = false;
			if(p_item->IsSlayerItem())
			{
				if (p_item->GetRequireSTR() > 0)
				{
					if(bl_required)
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_EMPTY].GetString());
					else
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE].GetString());
					bl_required = true;
					
					wsprintf(sz_temp + strlen(sz_temp), (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_STR].GetString(), p_item->GetRequireSTR());
					if (p_item->GetRequireDEX() > 0 ||
						p_item->GetRequireINT() > 0 ||
						//					p_item->GetRequireLevel() > 0 ||
						(p_item->GetRequireSUM() > 0 && p_item->GetRequireSUM() > p_item->GetRequireSTR()))
						strcat(sz_temp, ",");
					temp_string = sz_temp;
					m_rep_string.push_back(temp_string);
				}
				
				if (p_item->GetRequireDEX() > 0)
				{
					if(bl_required)
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_EMPTY].GetString());
					else
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE].GetString());
					bl_required = true;
					
					wsprintf(sz_temp + strlen(sz_temp), (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_DEX].GetString(), p_item->GetRequireDEX());
					if (p_item->GetRequireINT() > 0 ||
						//					p_item->GetRequireLevel() > 0 ||
						(p_item->GetRequireSUM() > 0 && p_item->GetRequireSUM() > p_item->GetRequireDEX()))
						strcat(sz_temp, ",");
					temp_string = sz_temp;
					m_rep_string.push_back(temp_string);
				}
				
				if (p_item->GetRequireINT() > 0)
				{
					if(bl_required)
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_EMPTY].GetString());
					else
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE].GetString());
					bl_required = true;
					
					wsprintf(sz_temp + strlen(sz_temp), (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_INT].GetString(), p_item->GetRequireINT());
					
					if (//p_item->GetRequireLevel() > 0 || 
						(p_item->GetRequireSUM() > 0 && p_item->GetRequireSUM() > p_item->GetRequireINT()))
						strcat(sz_temp, ",");
					temp_string = sz_temp;
					m_rep_string.push_back(temp_string);
				}
				
				if (p_item->GetRequireSUM() > 0 &&
				p_item->GetRequireSUM() > p_item->GetRequireSTR() &&
				p_item->GetRequireSUM() > p_item->GetRequireDEX() &&
				p_item->GetRequireSUM() > p_item->GetRequireINT() )
				{
					if(bl_required)
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_EMPTY].GetString());
					else
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE].GetString());
					bl_required = true;
					
					//				if (p_item->GetRequireLevel() > 0)
					//					strcat(sz_temp, " and");
	
					wsprintf(sz_temp + strlen(sz_temp),(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_ALL_STAT_SUM].GetString(), p_item->GetRequireSUM());
					temp_string = sz_temp;
					m_rep_string.push_back(temp_string);
				}
			}
			else if(p_item->IsVampireItem())
			{
				if (p_item->GetRequireLevel() > 0)
				{
					if(bl_required)
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_EMPTY].GetString());
					else
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE].GetString());
					bl_required = true;
					
					wsprintf(sz_temp + strlen(sz_temp), (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_LEVEL].GetString(), p_item->GetRequireLevel());
					temp_string = sz_temp;
					m_rep_string.push_back(temp_string);
				}
			}

			if(!p_item->IsSlayerItem() && g_eRaceInterface == RACE_SLAYER ||
				!p_item->IsVampireItem() && g_eRaceInterface == RACE_VAMPIRE ||
				!p_item->IsOustersItem() && g_eRaceInterface == RACE_OUSTERS
				)
			{
				if(bl_required)
					wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_EMPTY].GetString());
				else
					wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE].GetString());
				bl_required = true;
				
				strcat(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OTHER_TRIBE].GetString());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			if (p_item->IsGenderForMale())
			{
				if(bl_required)
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_EMPTY].GetString());
					else
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE].GetString());
				bl_required = true;

				strcat(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ONLY_MALE].GetString());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			else if (p_item->IsGenderForMale())
			{
				if(bl_required)
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE_EMPTY].GetString());
					else
						wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_REQUIRE].GetString());
				bl_required = true;

				strcat(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ONLY_FEMALE].GetString());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}

			// Price
//			if(//p_item->GetItemClass() != ITEM_CLASS_EVENT_GIFT_BOX
				// && p_item->GetItemClass() != ITEM_CLASS_EVENT_STAR)
//				p_item->GetItemClass() != ITEM_CLASS_EVENT_ETC
//				&& p_item->GetItemClass() != ITEM_CLASS_EVENT_TREE)
			{
				wsprintf(sz_temp, "%d", g_pPriceManager->GetItemPrice(p_item, MPriceManager::NPC_TO_PC));
				std::string sstr = sz_temp;
				for(int i = 3; i <= 13; i += 4)
					if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
					
				MString kkkstr = (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_PRICE];
				const char *pkkkstr = kkkstr.GetString();
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_PRICE].GetString(), sstr.c_str());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}

			// EVENT_STAR || EVENT_BOX
			if(p_item->GetItemClass() == ITEM_CLASS_EVENT_GIFT_BOX
				// || p_item->GetItemClass() == ITEM_CLASS_EVENT_STAR)
				|| p_item->GetItemClass() == ITEM_CLASS_EVENT_ETC
				|| p_item->GetItemClass() == ITEM_CLASS_EVENT_TREE)
				m_rep_string.push_back("\t");
			
		}
		break;
	
	case DID_SKILL:
		{
			m_pack_file.SetRAR(RPK_SKILL, RPK_PASSWORD);
			
			CSpritePack *temp;
			temp = new CSpritePack;
			temp->Init( 1 );		// 烙矫肺 1俺父 loading.. - -;;

			int id = (int)void_ptr;
			int sprID = (*g_pSkillInfoTable)[id].GetSpriteID();

			bool re = temp->LoadFromFileData( 0, sprID, SPK_SKILL_ICON, SPKI_SKILL_ICON );
			assert(re);

			m_pC_inpicture.push_back(temp);

			char sz_temp[10] = "%";
			wsprintf(sz_temp +1, "%d", 0);
			m_rep_string.push_back(sz_temp);

			title_pi = gpC_base->m_desc_menu_pi;
			title_color = RGB_YELLOW;
			pi = gpC_base->m_desc_msg_pi;
			color = RGB_WHITE;
			
			filename += (*g_pSkillInfoTable)[id].GetName();
			filename += ".txt";

			std::string title;
			title = (*g_pSkillInfoTable)[id].GetHName();
			title += " (";
			title += (*g_pSkillInfoTable)[id].GetName();
			title += ")";
			
			SetDescTitle(title.c_str());
		}
		break;

	}

//	if(sprID != -1)
//		SetSprite(0, sprID, 0);

	if(LoadDesc(filename.c_str(), 60, 17, bl_title,corezapID)==false)Run(CLOSE_ID);

//	if(m_ori_string.empty() && !m_rep_string.empty())
//	{
//		m_desc.insert(m_desc.begin(), m_rep_string.begin(), m_rep_string.end());
//	}

	m_ori_string.clear();
	m_rep_string.clear();


	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		SetDesc(50, 35, color, pi);
		SetDescTitle(50, 5, title_color, title_pi);
		break;
		
	case RACE_VAMPIRE:
		SetDesc(50, 35, color, pi);
		SetDescTitle(50, 5, title_color, title_pi);
		break;
		
	case RACE_OUSTERS:
		SetDesc(50, 35, color, pi);
		SetDescTitle(53, 8, title_color, title_pi);
		break;
	}

	m_pC_scroll_bar->SetPosMax(GetDescSize() - GetDescCol() +1);
	m_pC_button_group = new ButtonGroup(this);
//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(scroll_x_offset, scroll_up_y_offset, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_UP), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_UP), SCROLL_UP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_UP));
//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(scroll_x_offset, scroll_down_y_offset, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_DOWN), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_DOWN), SCROLL_DOWN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_DOWN));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x_offset, close_y_offset, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));
//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+help_x_offset, y+help_y_offset, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_HELP), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_HELP), HELP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_HELP));

}

//-----------------------------------------------------------------------------
// C_VS_UI_DESC_DIALOG::~C_VS_UI_DESC_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_DESC_DIALOG::~C_VS_UI_DESC_DIALOG()
{
	g_UnregisterWindow(this);
	g_desc_dialog_window_id = NULL;
	DeleteNew(m_pC_button_group);
	DeleteNew(m_pC_scroll_bar);
}

//-----------------------------------------------------------------------------
// C_VS_UI_DESC_DIALOG::Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_DESC_DIALOG::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_DESC_DIALOG::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_DESC_DIALOG::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x;_y-=y;//扩档快 捞悼阑 困茄
	bool re = m_pC_scroll_bar->MouseControl(message, _x, _y);
	re &= m_pC_button_group->MouseControl(message, _x, _y);

	g_descriptor_manager.Unset();

	switch(message)
	{
	case M_WHEEL_UP:
		m_pC_scroll_bar->ScrollUp();
		break;

	case M_WHEEL_DOWN:
		m_pC_scroll_bar->ScrollDown();
		break;

	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
//		if(_x > m_scroll_x && _x < m_scroll_x+gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth((gbl_vampire_interface?C_GLOBAL_RESOURCE::AB_VAMPIRE_SCROLL_TAG:C_GLOBAL_RESOURCE::AB_SLAYER_SCROLL_TAG)) &&
//			_y > m_scroll_y && _y < m_scroll_y+m_scroll_h+gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight((gbl_vampire_interface?C_GLOBAL_RESOURCE::AB_VAMPIRE_SCROLL_TAG:C_GLOBAL_RESOURCE::AB_SLAYER_SCROLL_TAG)))
//		{
//			if(_y > m_scroll_y+m_scroll_h)_y = m_scroll_y+m_scroll_h;
//			LB_SCROLL_DOWN = true;
//			SetDescScrollPos((GetDescSize()-GetDescCol()) * (_y - m_scroll_y) / (m_scroll_h));//+gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight((gbl_vampire_interface?C_GLOBAL_RESOURCE::AB_VAMPIRE_SCROLL_TAG:C_GLOBAL_RESOURCE::AB_SLAYER_SCROLL_TAG))));
//		}
//		else
		{
			if (gpC_mouse_pointer->GetPickUpItem() == false && re)
			{
				MoveReady();
				SetOrigin(_x, _y);
				break;
			}
		}
		break;

	case M_LEFTBUTTON_UP:
		break;

	case M_MOVING:
		break;
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_DESC_DIALOG::KeyboardControl
//
// 官肺 箭磊 涝仿且 荐 乐霸 茄促.
//-----------------------------------------------------------------------------
void	C_VS_UI_DESC_DIALOG::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN)
		switch (key)
		{
			case VK_RETURN: // ok肺 埃林. !cancel篮 馆靛矫 ok客 窃膊 乐促.
			case VK_ESCAPE: // 酒公巴档 救窃.
				Run(CLOSE_ID);
				break;
		}
}

//-----------------------------------------------------------------------------
// C_VS_UI_DESC_DIALOG::KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_DESC_DIALOG::Show()
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_global_resource->DrawDialogLocked2(x, y, w, h, GetAttributes()->alpha);
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	SetDescScrollPos(m_pC_scroll_bar->GetScrollPos());
	ShowDesc(x, y);

	m_pC_scroll_bar->Show(x, y);
	
	m_pC_button_group->ShowDescription();	
}

//-----------------------------------------------------------------------------
// C_VS_UI_DESC_DIALOG::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_DESC_DIALOG::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char *	m_help_string[1] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_DESC_WINDOW].GetString(),
	};
	
	if (p_button->GetID() == CLOSE_ID)
		g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}


//-----------------------------------------------------------------------------
// C_VS_UI_DESC_DIALOG::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_DESC_DIALOG::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);


	if(p_button->GetFocusState())
	{
		if(p_button->GetPressState())
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
	}
	else
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);

}

//-----------------------------------------------------------------------------
// C_VS_UI_DESC_DIALOG::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_DESC_DIALOG::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
//	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_DESC_DIALOG::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_DESC_DIALOG::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
//		if(m_bl_main_help)
//			gC_vs_ui.HotKey_Help();
//		else
			gpC_base->SendMessage(UI_CLOSE_DESC_DIALOG);
		break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_DESC_DIALOG::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_DESC_DIALOG::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_DESC_DIALOG::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_DESC_DIALOG::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_DESC_DIALOG::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_DESC_DIALOG::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}




//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::C_VS_UI_FILE_DIALOG
//
// Default Mode Value = MODE_PRO_FILE_SELECT
//-----------------------------------------------------------------------------
C_VS_UI_FILE_DIALOG::C_VS_UI_FILE_DIALOG(MODE Mode)
{
	STAY_FOLDER=-1;
	g_RegisterWindow(this);
	AttrTopmost(true);

	int close_x_offset, close_y_offset;
	int help_x_offset, help_y_offset;
	int scroll_x_offset, scroll_up_y_offset, scroll_down_y_offset;
	
	m_string_x = 50; m_string_y = 70; m_string_gap = 20;
	m_scroll_max = 13;
	m_select = -1;
	m_tempselect = -1;
	m_bl_open_drive = false;
	m_flag_preview_image=false;

	m_show_long_name = 1000;


	mp_open_current_directory = NULL;
	m_p_image_spk=NULL;
	m_p_icon_spk=NULL;


	m_pC_scroll_bar = NULL;
	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(1, Rect(370, 74, -1, 245));

	w = 400;
	h = 400;
	x = g_GameRect.right/2 - w/2;
	y = g_GameRect.bottom/2 - h/2;
	
	help_x_offset = w-150;
	help_y_offset = h-50;
	
	close_x_offset = w-80;
	close_y_offset = h-50;
	
	scroll_x_offset = 509;
	scroll_up_y_offset = 75;
	scroll_down_y_offset = 327;

	m_mode=Mode;

	m_temp_face.InitOffsurface(55,70,DDSCAPS_SYSTEMMEMORY);
	m_temp_face.FillSurface(0);
//	m_scroll_x = 510;
//	m_scroll_y = 95;
//	m_scroll_h = 212;
	
//	Set(g_GameRect.right/2-m_p_image_spk->GetWidth()/2, g_GameRect.bottom/2-m_p_image_spk->GetHeight()/2, m_p_image_spk->GetWidth(), m_p_image_spk->GetHeight());
	
	m_pC_button_group = new ButtonGroup(this);
//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(scroll_x_offset, scroll_up_y_offset, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_UP), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_UP), SCROLL_UP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_UP));
//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(scroll_x_offset, scroll_down_y_offset, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_DOWN), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_DOWN), SCROLL_DOWN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_DOWN));
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(help_x_offset, help_y_offset, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), OK_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_OK));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x_offset, close_y_offset, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_HELP), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_HELP), CANCEL_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL));

//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(drive_button_x, drive_button_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_DOWN), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_DOWN), DRIVE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_DOWN));

//	m_scroll = 0;

}



//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::~C_VS_UI_FILE_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_FILE_DIALOG::~C_VS_UI_FILE_DIALOG()
{
	g_UnregisterWindow(this);
	if(m_p_image_spk)
	{
		DeleteNew(m_p_image_spk);
	}
	if(m_p_icon_spk)
	{
		DeleteNew(m_p_icon_spk);
	}
	DeleteNew(m_pC_scroll_bar);
	DeleteNew(m_pC_button_group);

	if (mp_open_current_directory)
	{
		assert(mi_open_drive_count > 0);
		for (int i = 0; i < mi_open_drive_count; i++)
			delete []mp_open_current_directory[i];
		delete []mp_open_current_directory;
		mi_open_drive_count = 0;
	}
}


//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::Start
//
// 咯矾俺狼 颇老 八祸 啊瓷.	; 肺 备盒
// ex> C_VS_UI_FILE_DIALOG::Start(".bmp;.jpg;.pcx;.tga;.gif"); by sonee
//-----------------------------------------------------------------------------
void C_VS_UI_FILE_DIALOG::Start(char *type)
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	// type 俊 割啊瘤狼 颇老 辆幅啊 乐绰瘤 八祸茄促.
	int nType,i;

	for(i=0,nType=1;i<strlen(type);i++)
		if(type[i]==';') nType++;

	char *p_type;
	
	if(type == NULL)
		m_filter.clear();		
	else
	{
		m_filter.clear();
		p_type=type;
		m_filter.reserve(nType);
		for(i=0;i<nType;i++)
		{
			char *p_type_end;
			
			if(i==nType-1)
				p_type_end=&type[strlen(type)];
			else
				p_type_end=strstr(p_type,";");				
				
			char name[30]="";
			
			memcpy(name,p_type,p_type_end-p_type);
			name[p_type_end-p_type+1]='\0';
			m_filter.insert(&m_filter[i],name);
			p_type=p_type_end+1;
		}
		//m_filter = type;
	}
	
	m_filename = "";
	m_select_file_num.clear();


	// 颇老 促捞舅肺弊俊 甸绢啊绰 酒捞能
	m_p_icon_spk=new C_SPRITE_PACK(SPK_FILE_DIALOG);


//	AttrPin(true);
	gpC_window_manager->AppearWindow(this);

	if(mp_open_current_directory == NULL)
	{
		// System俊 何馒等 葛电 drive甫 佬绰促.
		DWORD drives = GetLogicalDrives();
		
		// 阿 bit甫 八荤窍咯 粮犁窍绰 drive buffer甫 父电促.
		DWORD index = 1;
		char	find_drive_name = 'a'; // a, b, c, d ... 家巩磊肺...
		char	find_drive_name_buf[sizeof(DWORD)*8];
		memset(find_drive_name_buf, 0, sizeof(DWORD)*8);
		
//		if (mp_open_current_directory)
//		{
//			assert(mi_open_drive_count > 0);
//			for (i = 0; i < mi_open_drive_count; i++)
//				delete []mp_open_current_directory[i];
//			delete []mp_open_current_directory;
//			mi_open_drive_count = 0;
//		}
		mi_open_drive_count = 0;
		assert(mi_open_drive_count == 0);
		
		for (i = 0; i < sizeof(DWORD)*8; i++)
		{
			if (drives & index)
			{
				find_drive_name_buf[mi_open_drive_count] = find_drive_name;
				mi_open_drive_count++;
			}
			
			index <<= 1;
			find_drive_name++;
		}

		mp_open_current_directory = new char *[mi_open_drive_count];
		CheckMemAlloc(mp_open_current_directory);
		
		for (i = 0; i < mi_open_drive_count; i++)
		{
			char *buf = new char[MAX_PATH];
			CheckMemAlloc(buf);
			memset(buf, 0, MAX_PATH);
			buf[0] = find_drive_name_buf[i];
			strcat(buf, ":\\*.*");
			mp_open_current_directory[i] = buf;
		}
		
		// Window啊 父甸绢瘤搁辑 directory甫 檬扁拳 矫挪促.
		char temp[MAX_PATH];
		DWORD char_count = GetCurrentDirectory(MAX_PATH, temp);
		assert(char_count > 0);
		
		for (i = 0; i < mi_open_drive_count; i++)
		{
			if (mp_open_current_directory[i][0] == tolower(temp[0]))
			{
				mi_open_drive_index = i;
				mp_open_current_directory[i][0] = '\0';
				strcat(mp_open_current_directory[i], temp);
				mp_open_current_directory[i][0] = tolower(mp_open_current_directory[i][0]);
			}
		}
	}
	RefreshFileList(mp_open_current_directory[mi_open_drive_index]);
	m_pC_scroll_bar->SetPosMax(m_vs_file_list.size()-12);
}

//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::MouseControl
//
// 
//-----------------------------------------------------------------------------

bool C_VS_UI_FILE_DIALOG::MouseControl(UINT message, int _x, int _y)
{
	static bool LB_SCROLL_DOWN = false;
	
	Window::MouseControl(message, _x, _y);
	_x-=x;
	_y-=y;//扩档快 捞悼阑 困茄
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	g_descriptor_manager.Unset();
	re&=m_pC_scroll_bar->MouseControl(message, _x, _y);	
	int temp_m_select;
//	m_scroll=m_pC_scroll_bar->GetScrollPos();
	
	
	switch(message)
	{
	case M_WHEEL_UP:
		m_flag_preview_image=false;
		m_pC_scroll_bar->ScrollUp();
		break;

	case M_WHEEL_DOWN:
		m_flag_preview_image=false;
		m_pC_scroll_bar->ScrollDown();
		break;

	case M_LEFTBUTTON_DOWN:		
	case M_LB_DOUBLECLICK :		
		if(m_bl_open_drive)		// Select From Drive Box
		{
			if(_x>51&&_y>54&&_x<350&&_y<54+m_string_gap*mi_open_drive_count) 
			{
				int temp=mi_open_drive_index;
				
				mi_open_drive_index = (_y - (37+m_string_gap))/m_string_gap;
				
				RefreshFileList(mp_open_current_directory[mi_open_drive_index]);
				m_pC_scroll_bar->SetPosMax(m_vs_file_list.size()-12);
				
				if(!m_vs_file_list.size()) 
					ChangeDir((char *)"\\..", mp_open_current_directory[temp]);
				
				m_bl_open_drive = false;
			} else
				m_bl_open_drive = false;
		} else
		if(_y>35&&_x>30&&_y<49&&_x<340)			// Open Drive Box
		{
			m_bl_open_drive=true;		
		}
				/*		else if(_x > m_scroll_x && _x < m_scroll_x+gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth((gbl_vampire_interface?C_GLOBAL_RESOURCE::AB_VAMPIRE_SCROLL_TAG:C_GLOBAL_RESOURCE::AB_SLAYER_SCROLL_TAG)) &&
				_y > m_scroll_y && _y < m_scroll_y+m_scroll_h)
				{
				LB_SCROLL_DOWN = true;
				m_scroll = (m_vs_file_list.size()-m_scroll_max) * (_y - m_scroll_y) / m_scroll_h;
				SetDescScrollPos(GetDescSize() * (_y - m_scroll_y) / (m_scroll_h+gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight((gbl_vampire_interface?C_GLOBAL_RESOURCE::AB_VAMPIRE_SCROLL_TAG:C_GLOBAL_RESOURCE::AB_SLAYER_SCROLL_TAG))));
	}*/
		else 
			if(_x > m_string_x-25 && _x < m_string_x+280 && 
				_y > m_string_y && _y < m_string_y + m_scroll_max*m_string_gap)		// File List
			{
				int m_old_select = m_select;
				m_select = (_y - m_string_y) / m_string_gap+m_pC_scroll_bar->GetScrollPos();
				
				if(m_select >= m_vs_file_list.size()) m_select = -1;
				
				//if(message == M_LB_DOUBLECLICK && m_select != -1)
				if( m_select == m_old_select && m_select != -1)			// Double Click Check
				{
					if(m_vs_file_list_attr[m_select] & FILE_ATTRIBUTE_DIRECTORY) // 叼泛配府老 版快 秦寸叼泛配府肺 颗辫
					{
						char name[200];
						strcpy(name, mp_open_current_directory[mi_open_drive_index]);					
						ChangeDir((char *)m_vs_file_list[m_select].c_str(), mp_open_current_directory[mi_open_drive_index]);					

						if(m_vs_file_list.empty() && strlen(name) > 3)
						{
							ChangeDir((char *)"\\..", mp_open_current_directory[mi_open_drive_index]);
						}
						m_filename = "";
						m_select_file_num.clear();
						m_select = -1;
						m_tempselect = -1;
					}
					else if(m_mode==MODE_PROFILE_SELECT)				// MODE_PROFILE_SELECT 绰 颇老歹喉努腐阑 瘤盔茄促. 
					{
						Run(OK_ID);
					}
					
					m_pC_scroll_bar->SetPosMax(m_vs_file_list.size()-12);				
				}
				if(m_select!=-1&&m_mode!=MODE_PROFILE_SELECT)	// 橇肺鞘 葛靛啊 酒聪搁
				{
					if(!(m_vs_file_list_attr[m_select] & FILE_ATTRIBUTE_DIRECTORY))	
					{
						if(g_GetCtrlPushState()) 
						{
							if(m_select_file_num.size()>0)
							{
								std::vector<int>::iterator i;
								i=std::find(m_select_file_num.begin(),m_select_file_num.end(),m_select);
								if(i!=m_select_file_num.end()) m_select_file_num.erase(i);
							} 							
						} 
						else
						{
							m_select_file_num.clear();
						}						
						m_select_file_num.insert(&m_select_file_num[m_select_file_num.size()], m_select);
					} 
					else		// 弊成 努腐沁阑版快
					{
						if(!g_GetCtrlPushState())	
							m_select_file_num.clear();
					}
				}
				//m_stay_openfolder=GetTickCount();			// 变捞抚殿阑 钎矫秦临锭 静绰 鸥捞赣
				Timer(true);
				STAY_FOLDER=0xfffffffe;
			}
			else
			{
				if (gpC_mouse_pointer->GetPickUpItem() == false && re)
				{
					MoveReady();
					SetOrigin(_x, _y);
					break;
				}
			}		
			break;

	case M_LEFTBUTTON_UP:
		LB_SCROLL_DOWN = false;
		break;

	case M_MOVING:		
		// 靛扼捞宏 冠胶啊 凯啡阑 版快 付快胶甫 爱促 初疽阑锭 1檬捞惑 秦寸 靛扼捞宏俊 困摹秦乐栏搁
		// 捞抚捞 拘绵等 版快 钱 匙烙阑 免仿窍咯 霖促.	
		m_flag_preview_image=true;
		if( _y > 35 && _x > 30 && _y < 49 && _x < 340 )
			STAY_FOLDER=0;

		if(m_bl_open_drive)
		{
			if( _x > 51 && _y > 54 && _x < 350 && _y < 54 +m_string_gap*mi_open_drive_count)
			{
				int mi_open_drive = (_y - (37+m_string_gap))/m_string_gap;
				mi_open_drive++;				
				if(STAY_FOLDER!=mi_open_drive) 
					Timer(true);
		//			m_stay_openfolder=GetTickCount();
				STAY_FOLDER=mi_open_drive;
			} 
			else
				STAY_FOLDER=-1;
		}
		temp_m_select = m_tempselect;				
		m_tempselect= (_y - m_string_y) / m_string_gap+m_pC_scroll_bar->GetScrollPos();
		
		if(m_tempselect >= m_vs_file_list.size() || m_tempselect < 0 || m_bl_open_drive)
			m_tempselect = -1;
		else
			STAY_FOLDER=-2;

//		m_stay_openfolder=GetTickCount();
		Timer(true);

		if(m_tempselect != -1)
		{
			if(temp_m_select != m_tempselect && !(m_vs_file_list_attr[m_tempselect] & FILE_ATTRIBUTE_DIRECTORY))
			{
				CDirectDrawSurface bmpSurface;
				const POINT faceSize={55,70};
				RECT destRect={0,0,55,70};
				std::string filename;
				
				filename+=mp_open_current_directory[mi_open_drive_index];
				filename.erase(filename.size()-3,3);
				filename+=m_vs_file_list[m_tempselect].c_str();							
				
				if(LoadImageToSurface(filename.c_str(), bmpSurface))
				{					
					RECT srcRect={0,0,bmpSurface.GetWidth(),bmpSurface.GetHeight()};					
					m_temp_face.FillSurface(0);
					m_temp_face.Blt(&destRect,&bmpSurface,&srcRect);
				} else
				{
					m_flag_preview_image=false;
					m_temp_face.FillSurface(0);
				}
			} 
		}
		
		break;

	default :
		break;
	}	
	return true;
}

bool	C_VS_UI_FILE_DIALOG::Timer(bool reset)
{
	static DWORD prev_time = GetTickCount();
	
	if(reset)
	{
		prev_time = GetTickCount();
	} else
	{
		if(prev_time + m_show_long_name < GetTickCount())
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::KeyboardControl
//
// 官肺 箭磊 涝仿且 荐 乐霸 茄促.
//-----------------------------------------------------------------------------
void	C_VS_UI_FILE_DIALOG::KeyboardControl(UINT message, UINT key, long extra)
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_RETURN: // ok肺 埃林. !cancel篮 馆靛矫 ok客 窃膊 乐促.
			Run(OK_ID);
			break;

		case VK_ESCAPE: // 酒公巴档 救窃.
			Run(CANCEL_ID);
			break;		
		}
	}	
}

//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::Show
//
// 巩磊凯 贸府窃荐尔 窃荐疙甸捞 粱 辨绢辑 家胶郴侩开矫 粱 遍聪促.
// 加档俊绰 喊促弗 瘤厘阑 林瘤 臼嚼聪促. 
//-----------------------------------------------------------------------------
void	C_VS_UI_FILE_DIALOG::Show()
{
	std::string title;
	char name[300],*p_name;
	DWORD dwNow;
	RECT rect;
	int i,j;
	// 600,133

	// 固府焊扁
	//SetRect(&rect, x+w,y,x+w+55,y+77);
	//DrawAlphaBox(&rect, 255, 255, 255, 20);
	

	gpC_global_resource->DrawDialog2(Rect(x, y, w, h), GetAttributes()->alpha);
	
	//----------------------------------------------------------------------------------------
	// 急琶康开 钎矫
	switch(m_mode)
	{
	case MODE_PROFILE_SELECT :
		if(m_select != -1 && m_select >= m_pC_scroll_bar->GetScrollPos() && m_select < m_pC_scroll_bar->GetScrollPos()+m_scroll_max)	
		{
			SetRect(&rect, x+m_string_x,y + m_string_y + (m_select - m_pC_scroll_bar->GetScrollPos()) * m_string_gap-3,x+m_string_x+275,y + m_string_y + (m_select - m_pC_scroll_bar->GetScrollPos()) * m_string_gap-3+m_string_gap);
			DrawAlphaBox(&rect, 0, 0, 20, 20);
		}
		break;

	case MODE_SENDFILE_SELECT :
		{
			for(int i=0;i<m_select_file_num.size();i++)	
			{
				if(m_select_file_num[i] >= m_pC_scroll_bar->GetScrollPos() && m_select_file_num[i] < m_pC_scroll_bar->GetScrollPos()+m_scroll_max)		{
					SetRect(&rect, x+m_string_x, y + m_string_y + (m_select_file_num[i] - m_pC_scroll_bar->GetScrollPos()) * m_string_gap-3,x+m_string_x + 275,rect.bottom = y + m_string_y + (m_select_file_num[i] - m_pC_scroll_bar->GetScrollPos()) * m_string_gap-3 + m_string_gap);
					DrawAlphaBox(&rect, 0, 0, 20, 20);	}
			}

			//if(m_select_file_num.size()<=0)
			if(m_select_file_num.empty())
			if(m_select != -1 && m_select >= m_pC_scroll_bar->GetScrollPos() && m_select < m_pC_scroll_bar->GetScrollPos()+m_scroll_max)	{
			SetRect(&rect, x+m_string_x,y + m_string_y + (m_select - m_pC_scroll_bar->GetScrollPos()) * m_string_gap-3,x+m_string_x+275,y + m_string_y + (m_select - m_pC_scroll_bar->GetScrollPos()) * m_string_gap-3+m_string_gap);
			DrawAlphaBox(&rect, 0, 0, 20, 20);}
		}
		break;
	}
	//----------------------------------------------------------------------------------------
	// 酒捞能 何盒 免仿		
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		for(i = 0; i < m_scroll_max && i+m_pC_scroll_bar->GetScrollPos() < m_vs_file_list.size(); i++){
			const char *get=m_vs_file_list[i+m_pC_scroll_bar->GetScrollPos()].c_str();
			if(get[0]=='\\') if(!strcmp(get,"\\.."))	m_p_icon_spk->BltLocked(x+m_string_x-19,y+m_string_y+i*m_string_gap,UPFOLDER_ID);
			else m_p_icon_spk->BltLocked(x+m_string_x-19,y+m_string_y+i*m_string_gap,FOLDER_ID);					
		}
		m_pC_button_group->Show();
		//----------------------------------------------------------------------------------------
		// 泅犁 坷锹等 弃歹疙 免仿
		m_p_icon_spk->BltLocked(x+m_string_x-19, y+37,OPENFOLDER_ID);
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	g_FL2_GetDC();	
	// 凯赴 弃歹疙狼 辨捞啊 辨版快 肋扼霖促 
	// 38磊 捞惑 登瘤 臼档废 茄促. 
	title+= mp_open_current_directory[mi_open_drive_index];
	title.erase(strlen(mp_open_current_directory[mi_open_drive_index])-2,2);
	for(i=0;i<m_filter.size();i++){title+=m_filter[i].c_str();title+=";";}
	title.erase(title.size()-1,1);
	strcpy(name,title.c_str());
	ReduceString(name,38);
	
	g_PrintColorStr(x+m_string_x, y+37, name, gpC_base->m_desc_menu_pi, RGB_WHITE);

	//----------------------------------------------------------------------------------------
	// 鸥捞撇 免仿
	if(m_mode==MODE_PROFILE_SELECT)	
		strcpy(name,(*g_pGameStringTable)[UI_STRING_MESSAGE_FILE_DIALOG_SELECT_PROFILE_PICTURE].GetString());
	else strcpy(name,(*g_pGameStringTable)[UI_STRING_MESSAGE_FILE_DIALOG_SELECT_FILE].GetString());
	
	int titleX = x+40, titleY = y+5;
	if(g_eRaceInterface == RACE_OUSTERS)
	{
		titleX += 3;
		titleY += 3;
	}
	g_PrintColorStr(titleX, titleY, name,gpC_base->m_desc_menu_pi, RGB_WHITE);

	for(i = 0; i < m_scroll_max && i+m_pC_scroll_bar->GetScrollPos() < m_vs_file_list.size(); i++) {		
		// 捞抚甸 免仿
		// 颇老捞抚捞 呈公 辨搁 利寸洒 磊弗促. 
		strcpy(name, m_vs_file_list[i+m_pC_scroll_bar->GetScrollPos()].c_str());
		ReduceString(name,38);
		if(name[0]=='\\')	p_name=&name[1];		else			p_name=&name[0];
		g_PrintColorStr(x+m_string_x, y+m_string_y+i*m_string_gap, p_name, gpC_base->m_desc_msg_pi, RGB_WHITE);
	}	
	m_pC_button_group->ShowDescription();
	
	g_FL2_ReleaseDC();	
	m_pC_scroll_bar->Show(x, y);
	
	//----------------------------------------------------------------------------------------
	// Drive List 免仿
	//drive 急琶 open 登绢乐栏搁
	if(m_bl_open_drive) {
		SetRect(&rect, x+m_string_x-5,y+37+m_string_gap-3, x+m_string_x-5 + 350, y+37+m_string_gap-3+m_string_gap*mi_open_drive_count);
		
		DrawAlphaBox(&rect, 100, 100,100, 20);
		gpC_global_resource->DrawOutBox(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top);		

		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			for(i = 0; i < mi_open_drive_count; i++)	{
				char ch=*mp_open_current_directory[i];
				if(ch=='a'||ch=='A'||i==mi_open_drive_count-1) 	m_p_icon_spk->BltLocked(x+m_string_x,y+37+(i+1)*m_string_gap,CDDRIVE_ID);
				else m_p_icon_spk->BltLocked(x+m_string_x,y+37+(i+1)*m_string_gap,HDDRIVE_ID);
			}
			gpC_base->m_p_DDSurface_back->Unlock();
		}
		g_FL2_GetDC();
		for(i = 0; i < mi_open_drive_count; i++)	{
			std::string filename;filename+=mp_open_current_directory[i];
			filename.erase(filename.size()-2,2);
			for(j=0;j<m_filter.size();j++){filename+=m_filter[j].c_str();filename+=";";}
			filename.erase(filename.size()-1,1);
			strcpy(name,filename.c_str());
			ReduceString(name,38);
			g_PrintColorStr(x+m_string_x+17, y+37+(i+1)*m_string_gap, name, gpC_base->m_desc_menu_pi, RGB_WHITE);
		}
		g_FL2_ReleaseDC();
	}
	
	//----------------------------------------------------------------------------------------
	// 变捞抚 利绢林扁
	dwNow=GetTickCount();
	//if((dwNow-m_stay_openfolder)>=1000)			// 1檬 捞惑 瘤眉矫
	if(Timer(false))
	{
		std::string title;
		if(STAY_FOLDER==0)
		{				// 泅犁 凯赴 弃歹
			title+=mp_open_current_directory[mi_open_drive_index];
			title.erase(title.size()-2,2);
			for(i=0;i<m_filter.size();i++) {title+=m_filter[i].c_str();	title+=';';}
			title.erase(title.size()-1,1);
			if(title.size()>38)
			{	
				SetRect(&rect, x+m_string_x+10,y+45,x+m_string_x+9*title.size(),y+68);
				DrawAlphaBox(&rect, 0, 0, 0, 20);		
				gpC_global_resource->DrawOutBox(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top);
				g_PrintColorStr(x+m_string_x+15, y+49, title.c_str(), gpC_base->m_desc_menu_pi, RGB_WHITE);
			}
		}
		else if(STAY_FOLDER!=-1&&m_bl_open_drive&&STAY_FOLDER!=0xfffffffe)	
		{		// 靛扼捞宏 府胶飘 吝 			
			title+=mp_open_current_directory[STAY_FOLDER-1];
			title.erase(title.size()-2,2);
			for(i=0;i<m_filter.size();i++){title+=m_filter[i].c_str();	title+=';';}
			title.erase(title.size()-1,1);			
			if(title.size()>38)
			{	
				SetRect(&rect, x+m_string_x+17,y+35+(STAY_FOLDER)*m_string_gap,x+m_string_x+17+9*title.size(),y+39+(STAY_FOLDER)*m_string_gap+18);
				DrawAlphaBox(&rect, 0, 0, 0, 20);		
				gpC_global_resource->DrawOutBox(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top);
				g_PrintColorStr(x+m_string_x+17, y+37+(STAY_FOLDER)*m_string_gap, title.c_str(), gpC_base->m_desc_menu_pi, RGB_WHITE);
			}			
		} 
		else
		{				// 颇老俊辑 啊父洒 乐阑版快
			if(m_tempselect != -1 && m_tempselect >= m_pC_scroll_bar->GetScrollPos() && m_tempselect < m_pC_scroll_bar->GetScrollPos()+m_scroll_max)
			if(m_vs_file_list[m_tempselect].size()>38)	
			{
				SetRect(&rect,  x+m_string_x, y + m_string_y + (m_tempselect - m_pC_scroll_bar->GetScrollPos()) * m_string_gap-3, x+m_string_x + 8*m_vs_file_list[m_tempselect].size(),y + m_string_y + (m_tempselect - m_pC_scroll_bar->GetScrollPos()) * m_string_gap-3 + m_string_gap);
				DrawAlphaBox(&rect, 0, 0, 0, 20);
				gpC_global_resource->DrawOutBox(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top);
				const char *temp=m_vs_file_list[m_tempselect].c_str();
				if(temp[0]=='\\')	
					g_PrintColorStr(rect.left+2,rect.top+2, (1+m_vs_file_list[m_tempselect].c_str()) ,gpC_base->m_desc_msg_pi, RGB_WHITE);
				else g_PrintColorStr(rect.left+2,rect.top+2, (m_vs_file_list[m_tempselect].c_str()) ,gpC_base->m_desc_msg_pi, RGB_WHITE);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------
	// 固府焊扁
	// 
	if(m_tempselect != -1 && m_tempselect >= m_pC_scroll_bar->GetScrollPos() && m_tempselect < m_pC_scroll_bar->GetScrollPos()+m_scroll_max && m_flag_preview_image)	
	{
		if(!(m_vs_file_list_attr[m_tempselect] & FILE_ATTRIBUTE_DIRECTORY)&&m_mode==MODE_PROFILE_SELECT)
		{
			POINT point = { x+m_string_x-m_temp_face.GetWidth()-2,  
				y + m_string_y + (m_tempselect - m_pC_scroll_bar->GetScrollPos()) * m_string_gap};
			SetRect(&rect,0,0,m_temp_face.GetWidth(), m_temp_face.GetHeight());
			gpC_base->m_p_DDSurface_back->BltNoColorkey(&point,&m_temp_face,&rect);
		}
	}		
}

//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_FILE_DIALOG::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	static char*	m_help_string[2] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_FILE_DIALOG_SELECT_FILE_OK].GetString(),			
		(*g_pGameStringTable)[UI_STRING_MESSAGE_FILE_DIALOG_CANCEL].GetString(),
	};	

	g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+p_button->y, (void *)m_help_string[p_button->GetID()],0,0);	
}

//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_FILE_DIALOG::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{			
	assert(p_button);

	
	if(p_button->GetFocusState())
	{
		if(p_button->GetPressState())
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
	}
	else
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);

}

//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_FILE_DIALOG::IsPixel(int _x, int _y)
{
//	return m_p_image_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
	return IsInRect(_x, _y);
}

//---------------------------------------------------------------------------
// 技固妮沸栏肺 备盒等 巩磊 惶酒郴扁
// GetFileNameInString 窃荐甫 荤侩窍绊 唱辑 馆券等 蔼篮 馆靛矫 delete 秦林技夸.
// usage>
// char *name=GetFileNameInString(str,2);
// MessageBox(NULL,name,name,MB_OK);
// delete name;

// 颇老捞抚俊辑 颇老狼 醚 肮荐
static int GetSizeFileInString(char *str)
{
	if(str==NULL||strlen(str)<=1)
		return 0;
	
	int nFile=1,i;
	
	for(i=0;i<strlen(str);i++)
	{
		if(str[i]==';') nFile++;
	}
	
	return nFile;
}

// 秦寸 锅龋俊 措茄 颇老 眠免
static const std::string GetFileNameInString(char *str,int n)
{
	if(str==NULL||strlen(str)<=1) return NULL;
	int len=strlen(str),i,PassSemi=0;
	char *start,*end;
	for(i=0;i<len;i++)
	{
		if(str[i]==';') PassSemi++;	
		else if(n==PassSemi)	
		{
			start=&str[i];
			break;
		}
	}
	if(GetSizeFileInString(str)<=n+1)	end=&str[len];
	else 
		end=strstr(start,";");	
	
	char name[512];
	memset(name,0,end-start+1);
	memcpy(name,start,end-start);name[end-start]='\0';

	return std::string(name);
}


//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_FILE_DIALOG::Run(id_t id)
{
	int i;
	switch (id)
	{
//	case SCROLL_UP_ID:
//		ScrollDescUp();
//		if(m_scroll > 0)
//			m_scroll--;
//		break;

//	case SCROLL_DOWN_ID:
//		if(m_scroll < m_vs_file_list.size()-m_scroll_max && m_vs_file_list.size() > m_scroll_max)
//			m_scroll++;
//		ScrollDescDown();
//		break;

	case CANCEL_ID:
//		if(m_bl_main_help)
//			gC_vs_ui.HotKey_Help();
//		else
		gpC_base->SendMessage(UI_CLOSE_FILE_DIALOG);
		break;
	//case DRIVE_ID:
	//	m_bl_open_drive = !m_bl_open_drive;
	//	break;
	case OK_ID :
		// SendMessage 肺 颇老捞抚阑 焊郴档废 茄促.
		//
		if( m_mode == MODE_PROFILE_SELECT )
		{
			if(m_select != -1 && m_select >= m_pC_scroll_bar->GetScrollPos() && m_select < m_pC_scroll_bar->GetScrollPos()+m_scroll_max)
			{
				m_filename = mp_open_current_directory[mi_open_drive_index];
				m_filename.erase(m_filename.end()-3, m_filename.end());
				m_filename += m_vs_file_list[m_select];
				gpC_base->SendMessage(UI_CLOSE_FILE_DIALOG, m_mode, 0,(void *) m_filename.c_str());
			} 
			else
				gpC_base->SendMessage(UI_CLOSE_FILE_DIALOG, m_mode, 0, NULL);
		} else
		{
			if( m_select_file_num.size() > 0 )
			{
				// 颇老捞 咯矾俺 急琶等 版快俊绰 技固妮沸栏肺 颇老阑 备盒茄促.
				std::string multiple_filename;
				
				for( i = 0; i < m_select_file_num.size(); i++ )
				{				
					multiple_filename += m_vs_file_list[m_select_file_num[i]];
					multiple_filename += ";";
				}
				multiple_filename.erase(multiple_filename.end()-1, multiple_filename.end());	
				gpC_base->SendMessage(UI_CLOSE_FILE_DIALOG, m_mode, 0, (void *) multiple_filename.c_str());
				
				// 叼滚彪窍妨绊 持篮 内靛涝聪促.
				//for(i=0;i<IsNumFileInString((char*)multiple_filename.c_str());i++)
				//{
				//	char *name=GetFileNameInString((char*)multiple_filename.c_str(),i);
				//	MessageBox(NULL,name,"Result",MB_OK);				
				//	delete name;
				//}
			} else
				gpC_base->SendMessage(UI_CLOSE_FILE_DIALOG, m_mode, 0, NULL);
		}
		break;

	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_FILE_DIALOG::Process()
{
	m_pC_button_group->Process();	
}

//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_FILE_DIALOG::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_FILE_DIALOG::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);

	if(m_p_image_spk)
	{
		DeleteNew(m_p_image_spk);
	}
	if(m_p_icon_spk)
		DeleteNew(m_p_icon_spk);	
}

/*-----------------------------------------------------------------------------
- RefreshFileList
-

  `file 捞 辈厘洒 腹阑 版快 拌加 捞 Method甫 角青窍搁 决没抄 加档历窍啊 老绢抄促.
   蝶扼辑 盎脚捞 鞘夸且 版快俊父 捞 Method甫 角青茄促.

  `GetCurrentDirectory()肺 掘篮 dir阑 refresh茄促.
-----------------------------------------------------------------------------*/
void C_VS_UI_FILE_DIALOG::RefreshFileList(char *sz_dirname)
{
	HANDLE				hFind;
	WIN32_FIND_DATA	fd;
	BOOL					next_ok = true; // FindNextFile(hFind, &fd)捞 BOOL阑 馆券窍骨肺...
	std::string			sz_filename;
	int					n;

	// exception
	if (sz_dirname[strlen(sz_dirname)-1] == '\\')
		sz_dirname[strlen(sz_dirname)-1] = 0;
	if (sz_dirname[strlen(sz_dirname)-1] == '*')
		sz_dirname[strlen(sz_dirname)-4] = 0;

	strcat(sz_dirname, "\\*.*");

	m_vs_file_list.clear();
	m_vs_file_list_attr.clear();
	
	hFind = FindFirstFile(sz_dirname, &fd);
	while (hFind != INVALID_HANDLE_VALUE && next_ok)
	{
		// '\.'篮 积帆茄促.
		//if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && fd.cFileName == ".")
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			if (fd.cFileName[0] == '.' && 
				 fd.cFileName[1] == '\0') goto _next_find_;

		n = strlen(fd.cFileName);
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			n += 1; // + '\' 
		}

		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			sz_filename = "\\";
			sz_filename += fd.cFileName;

			for(int i = 0; i < m_vs_file_list.size(); i++)
			{
				if(m_vs_file_list[i] > sz_filename || m_vs_file_list[i][0] != '\\')
				{
					m_vs_file_list.insert(&m_vs_file_list[i], sz_filename);
					m_vs_file_list_attr.insert(&m_vs_file_list_attr[i], fd.dwFileAttributes);
					break;
				}
			}
			if(i == m_vs_file_list.size())
			{
				m_vs_file_list.insert(&m_vs_file_list[i], sz_filename);
				m_vs_file_list_attr.insert(&m_vs_file_list_attr[i], fd.dwFileAttributes);
			}
		}
		else
		{
//			strcpy(str_buf, fd.cFileName);
			sz_filename = fd.cFileName;
			char szfile[20];
			int i,j;
			BOOL findflag=false,fAddFile=false;
			
			for(i=0;i<m_filter.size();i++)
			{
				strcpy(szfile,m_filter[i].c_str());
				
				findflag=false;
				for(j = 0; j < strlen(szfile); j++)
				{					
					if(Upperchar(sz_filename[sz_filename.size() - j-1]) 
						!= Upperchar(szfile[strlen(szfile) - j-1]))
						findflag=true;
				}
				if(!findflag) fAddFile=true;
			}
			if(!fAddFile) goto _next_find_;

//			m_vs_file_list.push_back(sz_filename);
//			m_vs_file_list_attr.push_back(fd.dwFileAttributes);

			for(i = 0; i < m_vs_file_list.size(); i++)
			{
				if(m_vs_file_list[i] > sz_filename && m_vs_file_list[i][0] != '\\')
				{
					m_vs_file_list.insert(&m_vs_file_list[i], sz_filename);
					m_vs_file_list_attr.insert(&m_vs_file_list_attr[i], fd.dwFileAttributes);
					break;
				}
			}
			if(i == m_vs_file_list.size())
			{
				m_vs_file_list.insert(&m_vs_file_list[i], sz_filename);
				m_vs_file_list_attr.insert(&m_vs_file_list_attr[i], fd.dwFileAttributes);
			}
		}

//		gC_ui.AddListUnit(dp, str_buf, fd.dwFileAttributes, true);
_next_find_:
		next_ok = FindNextFile(hFind, &fd);
	}

	FindClose(hFind);
	m_pC_scroll_bar->SetScrollPos(0);
	m_select = -1;

}

/*-----------------------------------------------------------------------------
- ChangeDir
-
-----------------------------------------------------------------------------*/
void C_VS_UI_FILE_DIALOG::ChangeDir(char *sz_cur_dirname, char *sz_pathname)
{
	assert(sz_cur_dirname);
	assert(sz_pathname);

	if (GetParentDir(sz_cur_dirname))
	{ 
		//
		// delete dir name
		//
		sz_pathname[strlen(sz_pathname)-4] = 0;

		int counter = 0;
		while (sz_pathname[strlen(sz_pathname)-(++counter)] != '\\');

		sz_pathname[strlen(sz_pathname)-counter] = 0;
		strcat(sz_pathname, "\\");
	}
	else
	{
		//
		// add dir name
		// `directory name狼 眠啊啊 啊瓷茄啊? 弊犯瘤 臼促搁 酒公老档 窍瘤 臼绰促.
		//
		if ((strlen(sz_pathname) + strlen(sz_cur_dirname) + 1 <= MAX_PATH) &&
			 sz_cur_dirname[1] != '.') // '.' = current dir -.-
		{
			sz_pathname[strlen(sz_pathname)-4] = 0;
			if (sz_pathname[strlen(sz_pathname)-1] == '\\')
				sz_pathname[strlen(sz_pathname)-1] = 0;

			strcat(sz_pathname, sz_cur_dirname);
		}
	}

//	int ret = SetCurrentDirectory(sz_pathname);
//	if (!ret)
//		SetCurrentDirectory("C:\\");
	RefreshFileList(mp_open_current_directory[mi_open_drive_index]);
	//if(m_vs_file_list.size()<=0)

	m_select = -1;
	m_tempselect = -1;

	EMPTY_MOVE;
}

/*-----------------------------------------------------------------------------
- 
- \.. 甫 八祸茄促.
-----------------------------------------------------------------------------*/
bool C_VS_UI_FILE_DIALOG::GetParentDir(char *sz_name)
{
	assert(sz_name);

	int n = strlen(sz_name);

	if (sz_name[0] != '\\') return false; // not dir!
	if (n != 3) return false;

	for (int i = 1; i < n; i++) // from 1
		if (sz_name[i] != '.')
			return false;

	return true;
}

int		C_VS_UI_FILE_DIALOG::GetMode()
{
	return m_mode;
}

