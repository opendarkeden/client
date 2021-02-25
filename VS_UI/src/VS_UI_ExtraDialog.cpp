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

#include <algorithm>

Window* g_desc_dialog_window_id = NULL;
/*

char * C_VS_UI_EDIT_DIALOG::m_sz_question_msg[1] = {"이 아이템            개를 사시겠습니까?"};

// 주석처리 by sigi
//char * C_VS_UI_MONEY_DIALOG::m_sz_question_msg[1] = {"버릴 금액을 입력하세요."};
//char * C_VS_UI_MONEY_DIALOG::m_sz_question_msg_for_storage[1] = {"보관할 금액을 입력하세요."};

char * C_VS_UI_MONEY_DIALOG::m_sz_question_msg[MAX_MONEY_DIALOG_TYPE][1] = {	// by sigi
	{  "버릴 금액을 입력하세요." } ,
	{  "보관할 금액을 입력하세요." } ,
	{  "찾을 금액을 입력하세요." } ,
	{  "거래할 금액을 입력하세요." } ,
	{  "빼낼 금액을 입력하세요." } ,
};

char * C_VS_UI_ASK_DIALOG::m_sz_question_msg[MAX_ASK_DIALOG_TYPE][2] = {	// by sigi
	{  "보관함을 $%d에 사시겠습니까?"  },	
	{  "%s 님과 거래하겠습니까?"  },
	{  "%s 님의 응답을 기다립니다." ,  "취소하시려면 Cancel을 누르세요."   },
//	{  "선택하신 무기는 %s입니다.", "결정하시겠습니까?" },					// by larosel
	{  "%s 님이 당신의 %s에 가입을 요청하고 있습니다."  },
	{  "%s 님이 당신을 %s에 초청하셨습니다."  },
	{  "%s 님의 응답을 기다립니다.",   "취소하시려면 Cancel을 누르세요."  },
	{  "검색 결과를 찾을 수 없습니다."  },
	{  "이 아이템을 Enchant하시겠습니까?",   "취소하시려면 Cancel을 누르세요."  },
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
		// string이 없다.
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

	char sz_temp[10];	// by sigi (원래는 2였당 - -;)
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
		// string이 없다. backspace하고 했을 때.
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
// 바로 숫자 입력할 수 있게 한다.
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
		// 보관함에 넣기
		case MONEY_DEPOSIT :
			// 현재 가지고 있는 만큼만 넣을 수 있고 보관함에 돈의 limit이상은 넣을수 없다.
			_MAX_MONEY =  min(g_pMoneyManager->GetMoney(), g_pStorage->GetMoneyManager()->GetMaxAddMoney());
			break;

		case MONEY_EXCHANGE:
			_MAX_MONEY = min(g_pMoneyManager->GetMoney(), g_pTradeManager->GetOtherMoneyManager()->GetMaxAddMoney());
			break;

		// 버리기
		case MONEY_DROP : 
				_MAX_MONEY = g_pMoneyManager->GetMoney();	
			break;
		
		// 보관함에서 꺼내기
		case MONEY_WITHDRAW : 
			// 보관함에 있는 만큼만 찾을 수 있고 인벤토리에 돈의 limit이상은 넣을수 없다.
			_MAX_MONEY = min(g_pStorage->GetMoneyManager()->GetMoney(), g_pMoneyManager->GetMaxAddMoney());
			break;

		case MONEY_WITHDRAW_FROM_EXCHANGE:
			_MAX_MONEY = g_pTradeManager->GetMyMoneyManager()->GetMoney();
			break;
		case MONEY_BRING_FEE :			// 총 세금에서 가지고 있는 돈을 빼야 20억 안넘는다...
			_MAX_MONEY = min(gC_vs_ui.GetTotalFee(),g_pMoneyManager->GetMaxAddMoney());
			break;
		case MODIFY_TAX:
			_MAX_MONEY = 10; // 세율은 10 까지만
			break;
		case MONEY_SELL_ITEM:
			_MAX_MONEY = 2000000000;
			break;
		case MONEY_CAMPAIGN_HELP:
			_MAX_MONEY = g_pMoneyManager->GetMoney()/10000;
			break;
		case MONEY_WEDDING_CONTRIBUTION :			// 축의금 최대 100,000 을넘지 않는다.
			_MAX_MONEY = 10 ; 
			break;

	}

	if (GetValue() > _MAX_MONEY )
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

	if((m_type == MONEY_CAMPAIGN_HELP)|| (m_type == MONEY_WEDDING_CONTRIBUTION))
	{
		if(g_FL2_GetDC())
		{	
			int iHeight = 0 ; 
			if(m_type == MONEY_WEDDING_CONTRIBUTION)	iHeight = 14 ; 

			switch(g_eRaceInterface)
			{
			case RACE_SLAYER:
				g_PrintColorStr(x+185, y+64 + iHeight, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAMPAIGN_HELP_UNITS_SLAYER].GetString(), gpC_base->m_item_name_pi, RGB_WHITE);
				break;

			case RACE_VAMPIRE:
				g_PrintColorStr(x+185, y+64 + iHeight, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAMPAIGN_HELP_UNITS_VAMPIRE].GetString(), gpC_base->m_item_name_pi, RGB_WHITE);
				break;

			case RACE_OUSTERS:
				g_PrintColorStr(x+185, y+64 + iHeight, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAMPAIGN_HELP_UNITS_OUSTERS].GetString(), gpC_base->m_item_name_pi, RGB_WHITE);
				break;		
			}
			g_FL2_ReleaseDC();
		}


		int TempValue =  GetValue();
		if(TempValue > 0)
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
		// string이 없다.
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
C_VS_UI_MONEY_DIALOG::C_VS_UI_MONEY_DIALOG(int _x, int _y, int center_x, int center_y, void (*exec_fp)(C_VS_UI_DIALOG *, id_t), WORD dd_button, int digit_count, TYPE_MONEY_DIALOG type) :	// type 추가 by sigi
							C_VS_UI_DIALOG(_x, _y, center_x, center_y, exec_fp, dd_button)
{
	/*	// 주석처리 by sigi
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
	m_sz_question_msg[9][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_WEDDING_CONTRIBUTION_REQUEST].GetString();	 	

	m_type = type;										

	if(m_type != MONEY_WEDDING_CONTRIBUTION)
		SetMessage(m_sz_question_msg[m_type], 1 ) ;//, SMO_NOFIT);		// by sigi
	else
		SetMessage(m_sz_question_msg[m_type], 1 , SMO_FIT  , TRUE ,   105); //, SMO_NOFIT);		// by sigi
	
	if (digit_count < 0)
		digit_count = 0;

	m_lev_value.SetDigitOnlyMode(true);
	m_lev_value.SetByteLimit(digit_count);
	Attach(&m_lev_value);

	//	m_p_image_spk = new C_SPRITE_PACK(SPK_MONEY_DIALOG);

	m_money_dialog_pt.x = m_client_rect.x + 0;
	if(m_type == MONEY_WEDDING_CONTRIBUTION)
		m_money_dialog_pt.y = m_client_rect.y + 40;
	else
		m_money_dialog_pt.y = m_client_rect.y + 25;

	m_lev_value.SetPosition(m_money_dialog_pt.x, m_money_dialog_pt.y+3);
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
	/*	(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_BUY_STORAGE] = "보관함을 $%d에 사시겠습니까?";
	(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_TRADE_OTHER_PLAYER]= "%s 님과 거래하겠습니까?";
	(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_WAIT_OTHER_PLAYER]= "%s 님의 응답을 기다립니다.";
	(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_CANCEL]	="취소하시려면 Cancel을 누르세요.";
	(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_REQUEST_JOIN]= "%s 님이 당신의 %s에 가입을 요청하고 있습니다.";
	(*g_pGameStringTable)[UI_STRING_MESSAGE_ASK_DIALOG_INVITE]= "%s 님이 당신을 %s에 초청하셨습니다.";
	(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_FIND_RESULT]= "검색 결과를 찾을 수 없습니다." ;
	(*g_pGameStringTable)[UI_STRING_MESSAGE_ENCHANT_CONFIRM]= "이 아이템을 Enchant하시겠습니까?";

  {  "보관함을 $%d에 사시겠습니까?"  },	
	{  "%s 님과 거래하겠습니까?"  },
	{  "%s 님의 응답을 기다립니다." ,  "취소하시려면 Cancel을 누르세요."   },
	{  "%s 님이 당신의 %s에 가입을 요청하고 있습니다."  },
	{  "%s 님이 당신을 %s에 초청하셨습니다."  },
	{  "%s 님의 응답을 기다립니다.",   "취소하시려면 Cancel을 누르세요."  },
	{  "검색 결과를 찾을 수 없습니다."  },
	{  "이 아이템을 Enchant하시겠습니까?",   "취소하시려면 Cancel을 누르세요."  },
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
	m_sz_question_msg[7][5]=(*g_pGameStringTable)[UI_STRING_MESSAGE_USE_ITEM_DEL].GetString();
	m_sz_question_msg[7][6]=(*g_pGameStringTable)[UI_STRING_MESSAGE_PET_ATTRIBUTE_CLEANER_POTION].GetString();
	m_sz_question_msg[7][7]=(*g_pGameStringTable)[UI_STRING_MESSAGE_MASK_OF_MONSTER].GetString();
	m_sz_question_msg[7][8]=(*g_pGameStringTable)[UI_STRING_MESSAGE_MAGICAL_PET_CHANGER].GetString();
	m_sz_question_msg[7][9]=(*g_pGameStringTable)[UI_STRING_MESSAGE_ORNAMENTS_ITEM].GetString();
	m_sz_question_msg[7][10]=(*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_STRENGTHENING].GetString();

	
	
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

	m_sz_question_msg[18][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_USE_ITEM_DEL].GetString();
	m_sz_question_msg[18][1]="";

	m_sz_question_msg[18][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_USE_ITEM_DEL].GetString();
	m_sz_question_msg[18][1]="";


	m_sz_question_msg[19][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_USE_ITEM_DEL].GetString();
	m_sz_question_msg[19][1]="";
	
	m_sz_question_msg[20][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_USE_ITEM_DEL].GetString();
	m_sz_question_msg[20][1]="";

	m_sz_question_msg[21][0]=(*g_pGameStringTable)[UI_STRING_MESSAGE_USE_ITEM_DEL].GetString();
	m_sz_question_msg[21][1]="";
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
		// 숫자가 하나 들어가는 경우.. - -;
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
			case 0:		// 일반 아이템 인첸트
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0]);
				m_sz_question_msg_temp[1] = new char [strlen(m_sz_question_msg[type][1])+1];
				sprintf(m_sz_question_msg_temp[1], m_sz_question_msg[type][1]);
				
				SetMessage(m_sz_question_msg_temp, 2, SMO_NOFIT);	
				break;
				
			case 1:		// 펫 인첸트
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0]);
				
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;
				
			case 2:		// 펫 부활
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][2])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][2]);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;

			case 3:		// 펫 퓨리타스
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][3])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][3]);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;

			case 4:		// 펫 변신
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][4])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][4]);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;

			case 5:		// 펫 변신
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][5])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][5]);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;

			case 6:		// 펫 속성을 초기
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][6])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][6]);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;

			case 7:		// 무기를 강화
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][9])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][9]);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;
			case 8:		// 펫 개량
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][8])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][8]); 
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	

			case 9:		// 아이템 강화 
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][10])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][10]); 
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
		case ASK_USE_DYE_POTION : 
			if(value == 1)
			{
				m_sz_question_msg_temp[0] = new char [strlen( m_sz_question_msg[type][0])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0]);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT );
			}
			m_sz_question_msg[18][0] =	(*g_pGameStringTable)[UI_STRING_MESSAGE_GRADE_SKILL_CLEANER_POTION].GetString();
		
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
// 바로 숫자 입력할 수 있게 한다.
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
			253, 254, 255,//골레머*2, 레베카
			217,//바토리
			250,251,252,//헬기조종사
			296, //야마모토
			297, //데니스
			362, 363, 364, 365, 366, //라센성 4명+블라드테페즈
			302, 303, 311,//튜토리얼 추가
			368, //리스토어 NPC 브리콜라카스 클론
			370, //드로베타 옷팜 레이븐
			427, 428, 429, 430,	// 유료화 존 그리피스, 로빈벡스터, 머피, 카르멘버만
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
			48, 47, 49, //골레머*2, 레베카
			43,//바토리
			44, 44, 44,//헬기조종사
			45, //야마모토
			46, //데니스
			50, 51, 52, 53, 54,//라센성 4명+블라드테페즈
			45, 46, 45,
			39, //리스토어 NPC 브리콜라카스 클론
			35, //드로베타 옷팜 레이븐
			55, 56, 57, 58,	// 유료화 존 그리피스, 로빈벡스터, 머피, 카르멘버만
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
			m_p_NPCFace.Init( 2 );		// 임시로 1개만 loading.. - -;;

			bool re;
			re = m_p_NPCFace.LoadFromFileData( 0, 0, SPK_NPCFACE, SPKI_NPCFACE );
			assert(re);
			re = m_p_NPCFace.LoadFromFileData( 1, m_iSprite, SPK_NPCFACE, SPKI_NPCFACE );
			assert(re);
			
			m_client_rect.x += m_p_NPCFace[0].GetWidth();
			m_client_rect.w -= m_p_NPCFace[0].GetWidth();
		}
	}
	// 2005, 1, 14, sobeit add start - npc 얼굴 대신 ItemDescription 이미지를보여준다. - 퀘스트 땜시...
	else
	{
		if(sprite_num == 0)
		{
			m_iSprite = -1;
		}
		else
		{
			m_iSprite = sprite_num;

			m_p_NPCFace.Init( 1 );		// 임시로 1개만 loading.. - -;;

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
// 바로 숫자 입력할 수 있게 한다.
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
	Set(RESOLUTION_X/2-w/2, RESOLUTION_Y/2-h/2, w, h);

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
			if(p_item->GetItemOptionListCount() > 1)			// Rare Item ? 
			{
				title_color = g_pClientConfig->COLOR_NAME_ITEM_RARE_OPTION;
			} else
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
			// 이름이 너무 길면 줄여주자.	ReduceString2 는 뒤쪽에 "..." 찍어주기~ 	by sonee
			ReduceString2(sz_temp,55);			
			SetDescTitle(sz_temp);

			// weight
//			wsprintf(sz_temp, "\t무게 : %d", p_item->GetWeight());
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
			if (p_item->GetMaxDamage() != -1) // damage가 있는가?
			{
				wsprintf(sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DIALOG_DAMAGE].GetString(), p_item->GetMinDamage(), p_item->GetMaxDamage());
				temp_string = sz_temp;
				m_rep_string.push_back(temp_string);
			}
			
			// critical hit
			if (p_item->GetCriticalHit() != -1) // 크리티컬히트가 있는가?
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
//				wsprintf(sz_temp, "\t아이템 개수 : %d개", p_item->GetNumber());
//				temp_string = sz_temp;
//				m_rep_string.push_back(temp_string);
//			}
			
			// Add option
			/*if (p_item->GetItemOptionListCount() != 0)
			{
				int option = p_item->GetItemOptionListCount();
				ITEMOPTION_INFO& optionInfo = (*g_pItemOptionTable)[option];
				
				char pPartName[20];
				strcpy(pPartName, ITEMOPTION_INFO::ITEMOPTION_PARTNAME[optionInfo.Part]); // Option이 적용되는 수치의 이름
				if(p_item->IsVampireItem() && strstr(pPartName,"MP") != NULL)
					*strstr(pPartName,"MP") = 'H';

				BYTE PlusPoint	= optionInfo.PlusPoint; // 적용되는 수치
				
				if (pPartName)
				{
					if(!strcmp(pPartName, "Durability"))
					{
						sprintf(sz_temp, "\t옵션 : %s +%d", pPartName, PlusPoint-100);
						strcat(sz_temp, "%");
					}
					else
						sprintf(sz_temp, "\t옵션 : %s +%d", pPartName, PlusPoint);
					
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
					
					BYTE PlusPoint	= optionInfo.PlusPoint; // 적용되는 수치
					
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
					
					BYTE PlusPoint	= optionInfo.PlusPoint; // 적용되는 수치
					
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
			
				// 코어잽의 ItemDescription.spk 에서의 인덱스를 얻어오자
				std::string AddedFileName;
				std::string temp_string;
				AddedFileName += p_AddItem->GetEName();
				AddedFileName += ".txt";
		
				m_pack_file.Open(AddedFileName.c_str());
				m_pack_file.GetString(sz_buf1, 50);
				temp_string = sz_buf1;

				corezapID = atoi(temp_string.c_str()+1);
				// 코어잽의 ItemDescription.spk 에서의 인덱스를 얻어오자
		
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
			temp->Init( 1 );		// 임시로 1개만 loading.. - -;;

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
	_x-=x;_y-=y;//윈도우 이동을 위한
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
// 바로 숫자 입력할 수 있게 한다.
//-----------------------------------------------------------------------------
void	C_VS_UI_DESC_DIALOG::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN)
		switch (key)
		{
			case VK_RETURN: // ok로 간주. !cancel은 반드시 ok와 함께 있다.
			case VK_ESCAPE: // 아무것도 안함.
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
	x = RESOLUTION_X/2 - w/2;
	y = RESOLUTION_Y/2 - h/2;
	
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
	
//	Set(RESOLUTION_X/2-m_p_image_spk->GetWidth()/2, RESOLUTION_Y/2-m_p_image_spk->GetHeight()/2, m_p_image_spk->GetWidth(), m_p_image_spk->GetHeight());
	
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
// 여러개의 파일 검색 가능.	; 로 구분
// ex> C_VS_UI_FILE_DIALOG::Start(".bmp;.jpg;.pcx;.tga;.gif"); by sonee
//-----------------------------------------------------------------------------
void C_VS_UI_FILE_DIALOG::Start(char *type)
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	// type 에 몇가지의 파일 종류가 있는지 검색한다.
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


	// 파일 다이알로그에 들어가는 아이콘
	m_p_icon_spk=new C_SPRITE_PACK(SPK_FILE_DIALOG);


//	AttrPin(true);
	gpC_window_manager->AppearWindow(this);

	if(mp_open_current_directory == NULL)
	{
		// System에 부착된 모든 drive를 읽는다.
		DWORD drives = GetLogicalDrives();
		
		// 각 bit를 검사하여 존재하는 drive buffer를 만든다.
		DWORD index = 1;
		char	find_drive_name = 'a'; // a, b, c, d ... 소문자로...
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
		
		// Window가 만들어지면서 directory를 초기화 시킨다.
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
	_y-=y;//윈도우 이동을 위한
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
					if(m_vs_file_list_attr[m_select] & FILE_ATTRIBUTE_DIRECTORY) // 디렉토리일 경우 해당디렉토리로 옮김
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
					else if(m_mode==MODE_PROFILE_SELECT)				// MODE_PROFILE_SELECT 는 파일더블클릭을 지원한다. 
					{
						Run(OK_ID);
					}
					
					m_pC_scroll_bar->SetPosMax(m_vs_file_list.size()-12);				
				}
				if(m_select!=-1&&m_mode!=MODE_PROFILE_SELECT)	// 프로필 모드가 아니면
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
					else		// 그냥 클릭했을경우
					{
						if(!g_GetCtrlPushState())	
							m_select_file_num.clear();
					}
				}
				//m_stay_openfolder=GetTickCount();			// 긴이름등을 표시해줄때 쓰는 타이머
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
		// 드라이브 박스가 열렸을 경우 마우스를 갖다 놓았을때 1초이상 해당 드라이브에 위치해있으면
		// 이름이 압축된 경우 풀 네임을 출력하여 준다.	
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
// 바로 숫자 입력할 수 있게 한다.
//-----------------------------------------------------------------------------
void	C_VS_UI_FILE_DIALOG::KeyboardControl(UINT message, UINT key, long extra)
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_RETURN: // ok로 간주. !cancel은 반드시 ok와 함께 있다.
			Run(OK_ID);
			break;

		case VK_ESCAPE: // 아무것도 안함.
			Run(CANCEL_ID);
			break;		
		}
	}	
}

//-----------------------------------------------------------------------------
// C_VS_UI_FILE_DIALOG::Show
//
// 문자열 처리함수랑 함수명들이 좀 길어서 소스내용역시 좀 깁니다.
// 속도에는 별다른 지장을 주지 않습니다. 
//-----------------------------------------------------------------------------
void	C_VS_UI_FILE_DIALOG::Show()
{
	std::string title;
	char name[300],*p_name;
	DWORD dwNow;
	RECT rect;
	int i,j;
	// 600,133

	// 미리보기
	//SetRect(&rect, x+w,y,x+w+55,y+77);
	//DrawAlphaBox(&rect, 255, 255, 255, 20);
	

	gpC_global_resource->DrawDialog2(Rect(x, y, w, h), GetAttributes()->alpha);
	
	//----------------------------------------------------------------------------------------
	// 선택영역 표시
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
	// 아이콘 부분 출력		
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		for(i = 0; i < m_scroll_max && i+m_pC_scroll_bar->GetScrollPos() < m_vs_file_list.size(); i++){
			const char *get=m_vs_file_list[i+m_pC_scroll_bar->GetScrollPos()].c_str();
			if(get[0]=='\\') if(!strcmp(get,"\\.."))	m_p_icon_spk->BltLocked(x+m_string_x-19,y+m_string_y+i*m_string_gap,UPFOLDER_ID);
			else m_p_icon_spk->BltLocked(x+m_string_x-19,y+m_string_y+i*m_string_gap,FOLDER_ID);					
		}
		m_pC_button_group->Show();
		//----------------------------------------------------------------------------------------
		// 현재 오픈된 폴더명 출력
		m_p_icon_spk->BltLocked(x+m_string_x-19, y+37,OPENFOLDER_ID);
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	g_FL2_GetDC();	
	// 열린 폴더명의 길이가 길경우 잘라준다 
	// 38자 이상 되지 않도록 한다. 
	title+= mp_open_current_directory[mi_open_drive_index];
	title.erase(strlen(mp_open_current_directory[mi_open_drive_index])-2,2);
	for(i=0;i<m_filter.size();i++){title+=m_filter[i].c_str();title+=";";}
	title.erase(title.size()-1,1);
	strcpy(name,title.c_str());
	ReduceString(name,38);
	
	g_PrintColorStr(x+m_string_x, y+37, name, gpC_base->m_desc_menu_pi, RGB_WHITE);

	//----------------------------------------------------------------------------------------
	// 타이틀 출력
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
		// 이름들 출력
		// 파일이름이 너무 길면 적당히 자른다. 
		strcpy(name, m_vs_file_list[i+m_pC_scroll_bar->GetScrollPos()].c_str());
		ReduceString(name,38);
		if(name[0]=='\\')	p_name=&name[1];		else			p_name=&name[0];
		g_PrintColorStr(x+m_string_x, y+m_string_y+i*m_string_gap, p_name, gpC_base->m_desc_msg_pi, RGB_WHITE);
	}	
	m_pC_button_group->ShowDescription();
	
	g_FL2_ReleaseDC();	
	m_pC_scroll_bar->Show(x, y);
	
	//----------------------------------------------------------------------------------------
	// Drive List 출력
	//drive 선택 open 되어있으면
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
	// 긴이름 적어주기
	dwNow=GetTickCount();
	//if((dwNow-m_stay_openfolder)>=1000)			// 1초 이상 지체시
	if(Timer(false))
	{
		std::string title;
		if(STAY_FOLDER==0)
		{				// 현재 열린 폴더
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
		{		// 드라이브 리스트 중 			
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
		{				// 파일에서 가만히 있을경우
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
	// 미리보기
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
// 세미콜론으로 구분된 문자 뽑아내기
// GetFileNameInString 함수를 사용하고 나서 반환된 값은 반드시 delete 해주세요.
// usage>
// char *name=GetFileNameInString(str,2);
// MessageBox(NULL,name,name,MB_OK);
// delete name;

// 파일이름에서 파일의 총 갯수
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

// 해당 번호에 대한 파일 추출
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
		// SendMessage 로 파일이름을 보내도록 한다.
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
				// 파일이 여러개 선택된 경우에는 세미콜론으로 파일을 구분한다.
				std::string multiple_filename;
				
				for( i = 0; i < m_select_file_num.size(); i++ )
				{				
					multiple_filename += m_vs_file_list[m_select_file_num[i]];
					multiple_filename += ";";
				}
				multiple_filename.erase(multiple_filename.end()-1, multiple_filename.end());	
				gpC_base->SendMessage(UI_CLOSE_FILE_DIALOG, m_mode, 0, (void *) multiple_filename.c_str());
				
				// 디버깅하려고 넣은 코드입니다.
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

  `file 이 굉장히 많을 경우 계속 이 Method를 실행하면 엄청난 속도저하가 일어난다.
   따라서 갱신이 필요할 경우에만 이 Method를 실행한다.

  `GetCurrentDirectory()로 얻은 dir을 refresh한다.
-----------------------------------------------------------------------------*/
void C_VS_UI_FILE_DIALOG::RefreshFileList(char *sz_dirname)
{
	HANDLE				hFind;
	WIN32_FIND_DATA	fd;
	BOOL					next_ok = true; // FindNextFile(hFind, &fd)이 BOOL을 반환하므로...
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
		// '\.'은 생략한다.
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
		// `directory name의 추가가 가능한가? 그렇지 않다면 아무일도 하지 않는다.
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
- \.. 를 검색한다.
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



//-----------------------------------------------------------------------------
// C_VS_UI_NPC_DIALOG::C_VS_UI_NPC_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_SUMMER_COME_BACK::C_VS_UI_SUMMER_COME_BACK(int _x, int _y, int center_x, int center_y, void (*exec_fp)(C_VS_UI_DIALOG *, id_t), WORD dd_button, TYPE_SUMMER_COME_BACK_DIALOG type)  : 
							C_VS_UI_DIALOG(_x, _y, center_x, center_y, exec_fp, dd_button)
{
	
	g_RegisterWindow(this);

	AttrTopmost(true);
	AttrKeyboardControl(true);

	int w_w = 530	;
	int w_h = 400	;

	int ok_offset_x, ok_offset_y;

	ok_offset_x = w_w - 90;
	ok_offset_y = w_h-60;
	
	Set(RESOLUTION_X/2-w_w/2, RESOLUTION_Y/2-w_h/2, w_w, w_h);

	m_type = type ; 

	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+ok_offset_x, y+ok_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_OK), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_OK), BUTTON_OK, this, C_GLOBAL_RESOURCE::AB_BUTTON_OK));
	

}
C_VS_UI_SUMMER_COME_BACK::~C_VS_UI_SUMMER_COME_BACK()
{
		g_UnregisterWindow(this);
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SUMMER_COME_BACK::Run(id_t id)
{
	switch( id )
	{
	case BUTTON_OK :		
		Finish();
		break;
	}
}


void	C_VS_UI_SUMMER_COME_BACK::Start()
{
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);

	m_pC_button_group->Init();
}

void C_VS_UI_SUMMER_COME_BACK::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}


void	C_VS_UI_SUMMER_COME_BACK::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
			case VK_RETURN:
				Run(BUTTON_OK);
				return;
		}
	}	
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_SUMMER_COME_BACK::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	m_pC_button_group->MouseControl(message, _x, _y);

	return true;
}


//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SUMMER_COME_BACK::Process()
{
	m_pC_button_group->Process();
}


void	C_VS_UI_SUMMER_COME_BACK::Show()
{
	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);
	
	g_FL2_GetDC();
	std::string str ; 	
	if(m_type == COME_BACK_LOGIN_AFTER)
		str = (*g_pGameStringTable)[UI_STRING_MESSAGE_SUMMER_COMEBACK_LOGIN_AFTER].GetString();
	else if(m_type == COME_BACK_SETTLEMENT_AFTER)
		str = (*g_pGameStringTable)[UI_STRING_MESSAGE_SUMMER_COMEBACK_SETTLEMENT_AFTER].GetString();
	else if(m_type == COME_BACK_RECOMMENDER)
		str = (*g_pGameStringTable)[UI_STRING_MESSAGE_SUMMER_COMEBACK_RECOMMENDER].GetString();

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
	//g_PrintColorStr( 326 + 20, 356, (*g_pGameStringTable)[UI_STRING_MESSAGE_I_AGREE].GetString(),gpC_base->m_chatting_pi, RGB_WHITE);

	m_pC_button_group->ShowDescription();
	
	g_FL2_ReleaseDC();

	m_pC_button_group->Show();

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// C_VS_UI_NETMARBLE_AGREEMENT::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_SUMMER_COME_BACK::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{	
	
	if (p_button->GetFocusState())	
	{
		if(p_button->GetPressState())
			gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x, p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x, p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
	} else
		gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x, p_button->y, p_button->m_image_index);
	
}


/*
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
		// 숫자가 하나 들어가는 경우.. - -;
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
			case 0:		// 일반 아이템 인첸트
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0]);
				m_sz_question_msg_temp[1] = new char [strlen(m_sz_question_msg[type][1])+1];
				sprintf(m_sz_question_msg_temp[1], m_sz_question_msg[type][1]);
				
				SetMessage(m_sz_question_msg_temp, 2, SMO_NOFIT);	
				break;
				
			case 1:		// 펫 인첸트
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][0])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][0]);
				
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;
				
			case 2:		// 펫 부활
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][2])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][2]);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;

			case 3:		// 펫 퓨리타스
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][3])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][3]);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;

			case 4:		// 펫 변신
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][4])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][4]);
				SetMessage(m_sz_question_msg_temp, 1, SMO_NOFIT);	
				break;

			case 5:		// 펫 변신
				m_sz_question_msg_temp[0] = new char [strlen(m_sz_question_msg[type][5])+1];
				sprintf(m_sz_question_msg_temp[0], m_sz_question_msg[type][5]);
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
		default :
			SetMessage(m_sz_question_msg[type], 1, SMO_NOFIT);	
	}
}
*/




