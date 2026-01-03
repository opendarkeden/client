// #include "client_PCH.h"

// VS_UI_Dialog.cpp
#pragma warning(disable:4786)

#include "VS_UI_Dialog.h"
#include "VS_UI_GlobalResource.h"
#include "VS_UI_description.h"
#include "ClientFunction.h"
#include "UserOption.h"

#include "mgamestringtable.h"
#include "UserInformation.h"
extern RECT g_GameRect;
//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_DIALOG::WindowEventReceiver(id_t event)
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
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_DIALOG::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// UnacquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_DIALOG::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_DIALOG::C_VS_UI_DIALOG(int _x, int _y, int width, int height, void (*exec_fp)(C_VS_UI_DIALOG *, id_t), WORD dd_button)
{
	if(width < 10)width = (width+2)*81;		//기존 코드와 호환
	if(height < 10 && height!=-1)height = (height+2)*81;	//기존 코드와 호환

	m_bOkOnly = false;
	m_TempValue1 = 0;
	
	g_RegisterWindow(this);

	AttrTopmost(true);
//	AttrKeyboardControl(true);

//	assert(center_x > -1);
//	assert(exec_fp != NULL);

//	m_center_x = center_x;
//	m_center_y = center_y;
	m_exec_fp = exec_fp;
	m_ddb = dd_button;

	m_pC_msg_scroll_bar = NULL;
	m_pC_menu_scroll_bar = NULL;
	m_p_menu = NULL;
	m_menu_count = 0;
//	m_pp_msg = NULL;
	m_line_count = 0;
	m_print_line_count = 0;
	m_nofit_mode_msg_y = 0;
//	m_remained_track = 0;
//	m_tag_up_limit = 0;
//	m_tag_down_limit = 0;
	m_flag_menu=FALSE;
	m_button_y_list=NULL;
	m_menu_y_size=0;

	w = width;
	h = height;
	// set Window pixel size
//	w = gpC_global_resource->m_pC_assemble_box_spk->GetWidth(C_GLOBAL_RESOURCE::AB_LEFT) + 
//		 gpC_global_resource->m_pC_assemble_box_spk->GetWidth(C_GLOBAL_RESOURCE::AB_CENTER)*m_center_x + 
//		 gpC_global_resource->m_pC_assemble_box_spk->GetWidth(C_GLOBAL_RESOURCE::AB_RIGHT);
//	if(m_center_y != -1)
//	{
//		h = gpC_global_resource->m_pC_assemble_box_spk->GetHeight(C_GLOBAL_RESOURCE::AB_UP) + 
//			gpC_global_resource->m_pC_assemble_box_spk->GetHeight(C_GLOBAL_RESOURCE::AB_CENTER)*m_center_y + 
//			gpC_global_resource->m_pC_assemble_box_spk->GetHeight(C_GLOBAL_RESOURCE::AB_DOWN);
//	}

	DIALOG_BUTTON_WIDTH = gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_OK);
	DIALOG_BUTTON_HEIGHT = gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_OK);

	// -1 = center
	if (_x == -1)
		x = g_GameRect.right/2-w/2;
	else
		x = _x;
	if (_y == -1 && h != -1)
		y = g_GameRect.bottom/2-h/2;
	else
		y = _y;

	// x좌표 보정
	// 화면밖으로 넘어가면 땡김...
	if (Right() >= g_GameRect.right)
		x = g_GameRect.right - w;
	if (x < 0)
		x = 0;
	if (Down() >= g_GameRect.bottom && h != -1)
		y = g_GameRect.bottom - h;
	if (y < 0 && h != -1)
		y = 0;

	// set Button count
	m_button_count = 0;
	if (m_ddb != DIALOG_NO_BUTTON)
	{
		if (m_ddb & DIALOG_OK)
			m_button_count++;
		if (m_ddb & DIALOG_CANCEL)
			m_button_count++;
		//add by viva : black button
		if (m_ddb & DIALOG_FRIEND_BLACK)
			m_button_count++;
	}

	// set Client rect
	// Window는 Move되지 않을 것임을 가정한다.
	const int _EXTRA = 30;//2;
	m_client_rect.x = x+DECORATE_GAP+_EXTRA;
	m_client_rect.w = w-DECORATE_GAP*2-_EXTRA*2;
	if(h != -1)
	{
		m_client_rect.y = y+DECORATE_GAP+_EXTRA/2;
		m_client_rect.h = h-DECORATE_GAP*2-_EXTRA;
	}

	// set button
	m_pC_button_group = new ButtonGroup(this);

	if (m_ddb & DIALOG_OK && h != -1)
	{
		m_pC_button_group->Add(	new C_VS_UI_EVENT_BUTTON(DIALOG_BUTTON_POS(BS_OK), DIALOG_BUTTON_Y, DIALOG_BUTTON_WIDTH, DIALOG_BUTTON_HEIGHT, DIALOG_EXECID_OK, this, C_GLOBAL_RESOURCE::AB_BUTTON_OK));
		//m_pC_button_group->Add(	new C_VS_UI_EVENT_BUTTON(150, DIALOG_BUTTON_Y, DIALOG_BUTTON_WIDTH, DIALOG_BUTTON_HEIGHT, DIALOG_EXECID_OK, this, C_GLOBAL_RESOURCE::AB_BUTTON_OK));
		// 두 버튼 다 있는 경우
		if (m_ddb & DIALOG_CANCEL)
		{
			m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(DIALOG_BUTTON_POS(BS_CANCEL), DIALOG_BUTTON_Y, DIALOG_BUTTON_WIDTH, DIALOG_BUTTON_HEIGHT, DIALOG_EXECID_CANCEL, this, C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL));
			//m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(200, DIALOG_BUTTON_Y, DIALOG_BUTTON_WIDTH, DIALOG_BUTTON_HEIGHT, DIALOG_EXECID_CANCEL, this, C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL));
		}
	}
	else
	{
		// cancel만 있는 경우
		if (m_ddb & DIALOG_CANCEL && h != -1)
		{
			m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(DIALOG_BUTTON_POS(BS_OK), DIALOG_BUTTON_Y, DIALOG_BUTTON_WIDTH, DIALOG_BUTTON_HEIGHT, DIALOG_EXECID_CANCEL, this, C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL));
			//m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(150, DIALOG_BUTTON_Y, DIALOG_BUTTON_WIDTH, DIALOG_BUTTON_HEIGHT, DIALOG_EXECID_CANCEL, this, C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL));
		}
	}
	if(m_ddb & DIALOG_FRIEND_BLACK && h != -1)		//add by viva
	{
		m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(DIALOG_BUTTON_POS(BS_OK)-gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_FRIEND_BLACK) -10, DIALOG_BUTTON_Y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_FRIEND_BLACK), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_FRIEND_BLACK), DIALOG_EXECID_FRIEND_BLACK, this, C_GLOBAL_RESOURCE::AB_BUTTON_FRIEND_BLACK));
		//m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(125, DIALOG_BUTTON_Y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_FRIEND_BLACK), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_FRIEND_BLACK), DIALOG_EXECID_FRIEND_BLACK, this, C_GLOBAL_RESOURCE::AB_BUTTON_FRIEND_BLACK));
	}
	
	
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_DIALOG
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_DIALOG::~C_VS_UI_DIALOG()
{
	g_UnregisterWindow(this);


	if(m_pC_menu_scroll_bar)
	{
		DeleteNew(m_pC_menu_scroll_bar);
	}
	if(m_pC_msg_scroll_bar)
	{
		DeleteNew(m_pC_msg_scroll_bar);
	}
//	for (int i=0; i < m_menu_count; i++)
//	{
//		DeleteNewArray(m_p_menu[i].sz_menu_str);
//	}

	DeleteNewArray(m_p_menu);
	DeleteNewArray(m_button_y_list);

	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_DIALOG::CancelPushState
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_DIALOG::CancelPushState()
{
//	m_tag_pushed = false;
	m_pC_button_group->CancelPushState();
}

//-----------------------------------------------------------------------------
// GetButtonGap
//
// button이 있냐없냐를 결정하여 button height gap을 반환한다.
//-----------------------------------------------------------------------------
int C_VS_UI_DIALOG::GetButtonGap() const
{
	if (m_button_count > 0)
		return DIALOG_BUTTON_HEIGHT+DIALOG_BUTTON_EXTRA_HGAP;
	else
		return DIALOG_BUTTON_EXTRA_HGAP;
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_DIALOG::Run(id_t id)
{
	if(m_exec_fp == NULL)return;

	if(id != DIALOG_EXECID_SCROLL_UP && id != DIALOG_EXECID_SCROLL_DOWN)
		m_exec_fp(this, id);

	switch (id)
	{
//		case DIALOG_EXECID_SCROLL_UP:
//			m_scrollbar.DecreaseAmountToScroll();
//			break;

//		case DIALOG_EXECID_SCROLL_DOWN:
//			m_scrollbar.IncreaseAmountToScroll();
//			break;

		case DIALOG_EXECID_OK:
#ifdef _LIB
			if(true == g_pUserInformation->IsAutoLogIn) // 웹 자동 로긴 일 경우 
				gpC_base->SendMessage(UI_TERMINATION, 0, 0); // 모드가 메인 메뉴 일 때
#endif
			Finish();
			break;

		case DIALOG_EXECID_CANCEL:
		case DIALOG_EXECID_EXIT:
		case DIALOG_EXECID_FRIEND_BLACK:
			Finish();
			break;
	}

	// scroll up/down common control
//	if (m_available_tag == true)
//	{
//		// change scroll tag position
//		double proportion = Proportion(m_scrollbar.GetPercentToScroll(), PERCENTAGE, m_remained_track);
//
//		// 소수 첫째자리 반올림.
//		proportion += 0.5;
//		m_tag_rect.y = m_tag_up_limit+(int)floor(proportion);
//	}
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_DIALOG::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_DIALOG::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
			case VK_RETURN: // ok로 간주. !cancel은 반드시 ok와 함께 있다.
				if (m_ddb & DIALOG_OK)
					Run(DIALOG_EXECID_OK);
				break;

			case VK_ESCAPE: // 아무것도 안함.
				if(true == m_bOkOnly)
					Run(DIALOG_EXECID_OK);
				else
					Run(DIALOG_EXECID_EXIT);
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_DIALOG::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	if(m_pC_menu_scroll_bar) 
	{
		BOOL result=m_pC_menu_scroll_bar->MouseControl(message,_x-m_menu_rect.x,_y-m_menu_rect.y);
		if(result==false) ProcessMenuScrollBar();		
		if(GetScrollPos()>0)
		{
			int skip;			
			skip=m_button_y_list[GetScrollPos()]-m_button_y_list[0];
			m_pC_button_group->MouseControl(message, _x, _y+skip+m_menu_y_size);
		}
		else
			m_pC_button_group->MouseControl(message, _x, _y+m_menu_y_size);		
	} else m_pC_button_group->MouseControl(message, _x, _y+m_menu_y_size);

	// control scroll bar tag
//	if (m_available_tag == true)
	if(m_pC_msg_scroll_bar)
	{
		m_pC_msg_scroll_bar->MouseControl(message, _x-m_msg_rect.x, _y-m_msg_rect.y);		
	}
	{
		switch (message)
		{
			case M_MOVING:
				if (_y >= m_msg_rect.y && _y < m_msg_rect.y+m_msg_rect.h)
				{
//					if (m_tag_pushed == true)
//					{
//						m_tag_rect.y += _y-m_prev_tag_y;
//						m_prev_tag_y = _y;
//
//						// check limit
//						if (m_tag_rect.y < m_tag_up_limit)
//							m_tag_rect.y = m_tag_up_limit;
//						if (m_tag_rect.y > m_tag_down_limit)
//							m_tag_rect.y = m_tag_down_limit;
//
//						m_scrollbar.SetAmountToScrollByPercent(Percentage(m_tag_rect.y-m_tag_up_limit, m_remained_track));
//					}
				}
				break;

			case M_LEFTBUTTON_DOWN:
			case M_LB_DOUBLECLICK:
//				if (m_tag_rect.IsInRect(_x, _y) == true)
//				{
//					m_tag_pushed = true;
//					m_prev_tag_y = _y;
//				}
				break;

			case M_LEFTBUTTON_UP:
				CancelPushState();
				break;

			case M_WHEEL_UP:
				if(_x>m_msg_rect.x&&_x<m_msg_rect.x+m_msg_rect.w&&_y>m_msg_rect.y&&_y<m_msg_rect.y+m_msg_rect.h)
				{
					if(m_pC_msg_scroll_bar)
						m_pC_msg_scroll_bar->ScrollUp();
				} else
				if(_x>m_menu_rect.x&&_x<m_menu_rect.x+m_menu_rect.w&&_y>m_menu_rect.y&&_y<m_menu_rect.y+m_menu_rect.h)
				{
					if(m_pC_menu_scroll_bar)
						m_pC_menu_scroll_bar->ScrollUp();
				}
				ProcessMenuScrollBar();
//				Run(DIALOG_EXECID_SCROLL_UP);
				break;

			case M_WHEEL_DOWN:
				if(_x>m_msg_rect.x&&_x<m_msg_rect.x+m_msg_rect.w&&_y>m_msg_rect.y&&_y<m_msg_rect.y+m_msg_rect.h)
				{				
					if(m_pC_msg_scroll_bar)
						m_pC_msg_scroll_bar->ScrollDown();
				} else
				if(_x>m_menu_rect.x&&_x<m_menu_rect.x+m_menu_rect.w&&_y>m_menu_rect.y&&_y<m_menu_rect.y+m_menu_rect.h)
				{
					if(m_pC_menu_scroll_bar)
						m_pC_menu_scroll_bar->ScrollDown();
				}
				ProcessMenuScrollBar();
//				Run(DIALOG_EXECID_SCROLL_DOWN);
				break;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
// ShowButtonDescription
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_DIALOG::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
}


//-----------------------------------------------------------------------------
// ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_DIALOG::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	int i;

	// Menu button과 그냥 Button을 다르게 처리. 
	if (p_button->GetID() == DIALOG_EXECID_OK ||
		 p_button->GetID() == DIALOG_EXECID_CANCEL ||
		 p_button->GetID() == DIALOG_EXECID_FRIEND_BLACK
		 )
	{
		if (p_button->GetFocusState())
		{
			if(p_button->GetPressState())
			{
				gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x, p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
			}
			else
			{
				gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x, p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
			}
		}
		else
		{
			gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x, p_button->y, p_button->m_image_index);
		}
	}
	else if (p_button->GetID() == DIALOG_EXECID_SCROLL_UP ||
				p_button->GetID() == DIALOG_EXECID_SCROLL_DOWN)
	{
//		gpC_global_resource->m_pC_assemble_box_etc_spk->Blt(p_button->x, p_button->y, p_button->m_image_index);

//		if (p_button->GetFocusState() && p_button->GetPressState())
//			gpC_global_resource->m_pC_assemble_box_etc_spk->Blt(p_button->x+1, p_button->y+1, p_button->m_image_index+2);
	}
	else
	{	
		if (m_menu_rect.w != 0 && m_menu_rect.h != 0)
		{
			PrintInfo * p_pi;
			
			// 메뉴 이동에 따른 버튼 좌표 보정 값
			int y_skip_line=0;
			if(m_pC_menu_scroll_bar!=NULL)
			{
				if(p_button->m_image_index<GetScrollPos()) return;
				if(p_button->m_image_index>0&&m_button_y_list!=NULL&&GetScrollPos()>0)
					y_skip_line=m_button_y_list[GetScrollPos()]-m_temp_menu_rect_y;
			}
			y_skip_line+=m_menu_y_size;			// m_menu_y_size 는 메뉴Rect 크기가 변경되었을때 보정할 값이다.


			if (p_button->GetFocusState())
			{
				gpC_base->m_dialog_menu_pi.text_color = RGB(180, 180, 255);

				p_pi = &gpC_base->m_dialog_menu_pi;

				// arrow^^
//				gpC_global_resource->m_pC_assemble_box_etc_spk->Blt(p_button->x-15, p_button->y+1, C_GLOBAL_RESOURCE::AB_RIGHT_ARROW);
//				gpC_global_resource->m_pC_assemble_box_etc_spk->Blt(p_button->Right()+9, p_button->y+1, C_GLOBAL_RESOURCE::AB_LEFT_ARROW);
//				gpC_global_resource->m_pC_assemble_box_etc_spk->Blt(p_button->x-25, p_button->y+1, C_GLOBAL_RESOURCE::AB_LEFT_ARROW);

			}
			else
			{
				gpC_base->m_dialog_menu_pi.text_color = RGB_WHITE;

				p_pi = &gpC_base->m_dialog_menu_pi;
			}
			
			if (p_button->GetFocusState() && p_button->GetPressState())
			{				
				if(p_button->y-y_skip_line<m_menu_rect.y+m_menu_rect.h)
					gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x - 20,p_button->y+2-y_skip_line, C_GLOBAL_RESOURCE::AB_DIALOG_TAG_SELECTED);

				if(!m_p_menu[p_button->m_image_index].sz_menu_str.empty())
				{
					g_FL2_GetDC();
					for(i=0; i < m_p_menu[p_button->m_image_index].sz_menu_str.size(); i++)
					{
						if(p_button->y-i*TEXT_EXTRA_HGAP-y_skip_line+m_menu_str_height*i<m_menu_rect.y+m_menu_rect.h)
							g_Print(p_button->x, p_button->y+6 +m_menu_str_height*i - TEXT_EXTRA_HGAP*i-y_skip_line, m_p_menu[p_button->m_image_index].sz_menu_str[i].c_str(), p_pi);
					}
					g_FL2_ReleaseDC();
				}
			}
			else
			{
				if(p_button->GetFocusState())
				{
					if(p_button->y-y_skip_line<m_menu_rect.y+m_menu_rect.h)
						gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x - 20, p_button->y-y_skip_line, C_GLOBAL_RESOURCE::AB_DIALOG_TAG_SELECTED);
				}
				else
				{
					if(p_button->y-y_skip_line<m_menu_rect.y+m_menu_rect.h)
						gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x - 20, p_button->y-y_skip_line, C_GLOBAL_RESOURCE::AB_DIALOG_TAG);
				}
				if(!m_p_menu[p_button->m_image_index].sz_menu_str.empty())
				{
					g_FL2_GetDC();
					for(i=0; i < m_p_menu[p_button->m_image_index].sz_menu_str.size(); i++)
					{
						// 윗줄을 컷해준다.
						if(p_button->y-i*TEXT_EXTRA_HGAP-y_skip_line+m_menu_str_height*i<m_menu_rect.y+m_menu_rect.h)
						{
							//m_p_menu[p_button->m_image_index].sz_menu_str[i].c_str()
							if(m_p_menu != NULL && p_button->m_image_index < m_menu_count && i <= m_p_menu[p_button->m_image_index].sz_menu_str.size() && m_p_menu[p_button->m_image_index].sz_menu_str[i].size() > 0)
								g_Print(p_button->x, p_button->y+4 +m_menu_str_height*i - TEXT_EXTRA_HGAP*i-y_skip_line, m_p_menu[p_button->m_image_index].sz_menu_str[i].c_str(), p_pi);
						}
					}
					g_FL2_ReleaseDC();
				}
			}
		}
	}

}

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_DIALOG::Show()
{
	
	int i;
//	int i, j;
//	int _x = x+81;//gpC_global_resource->m_pC_assemble_box_spk->GetWidth(C_GLOBAL_RESOURCE::AB_LEFTUP);
//	int _y = y+h-81;//gpC_global_resource->m_pC_assemble_box_spk->GetHeight(C_GLOBAL_RESOURCE::AB_LEFTDOWN);

	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);
//	Rect rect;
//	rect.Set(0, 0, w-2, h-2);
//
//	// alpha가 세팅되어 있으면 반투명창으로 출력 :)
//	if(GetAttributes()->alpha)
//	{
//		RECT alpha_rect;
//		if(gbl_vampire_interface)
//			SetRect(&alpha_rect, x+10, y+9, x+rect.w-7, y+rect.h-8);
//		else
//			SetRect(&alpha_rect, x+5, y+12, x+rect.w-4, y+rect.h-10);
//		DrawAlphaBox(&alpha_rect, 0, 0, 0, ALPHA_DEPTH);
//		gpC_global_resource->m_pC_assemble_box_spk->BltClip(x, y, rect, C_GLOBAL_RESOURCE::AB_BACK_ALPHA);
//	}
//	else
//		gpC_global_resource->m_pC_assemble_box_spk->BltClip(x, y, rect, C_GLOBAL_RESOURCE::AB_BACK);
//
//	rect.Set(0, 0, gpC_global_resource->m_pC_assemble_box_spk->GetWidth(C_GLOBAL_RESOURCE::AB_RIGHT), h);
//	gpC_global_resource->m_pC_assemble_box_spk->BltClip(x+w - gpC_global_resource->m_pC_assemble_box_spk->GetWidth(C_GLOBAL_RESOURCE::AB_RIGHT), y, rect, C_GLOBAL_RESOURCE::AB_RIGHT);
//	rect.Set(0, 0, w, gpC_global_resource->m_pC_assemble_box_spk->GetHeight(C_GLOBAL_RESOURCE::AB_DOWN));
//	gpC_global_resource->m_pC_assemble_box_spk->BltClip(x, y+h - gpC_global_resource->m_pC_assemble_box_spk->GetHeight(C_GLOBAL_RESOURCE::AB_DOWN), rect, C_GLOBAL_RESOURCE::AB_DOWN);
//	gpC_global_resource->m_pC_assemble_box_spk->Blt(x+w - gpC_global_resource->m_pC_assemble_box_spk->GetWidth(C_GLOBAL_RESOURCE::AB_RIGHTDOWN), y+h - gpC_global_resource->m_pC_assemble_box_spk->GetHeight(C_GLOBAL_RESOURCE::AB_RIGHTDOWN), C_GLOBAL_RESOURCE::AB_RIGHTDOWN);
//	if (gpC_base->m_p_DDSurface_back->Lock())
//	{
//	gpC_global_resource->m_pC_assemble_box_spk->BltLocked(x, y, C_GLOBAL_RESOURCE::AB_LEFTUP);
//	gpC_global_resource->m_pC_assemble_box_spk->BltLocked(x, _y, C_GLOBAL_RESOURCE::AB_LEFTDOWN);
//
//	for (i=0; i < m_center_x; i++)
//	{
//		gpC_global_resource->m_pC_assemble_box_spk->BltLocked(_x, y, C_GLOBAL_RESOURCE::AB_UP);
//		gpC_global_resource->m_pC_assemble_box_spk->BltLocked(_x, _y, C_GLOBAL_RESOURCE::AB_DOWN);
//		_x += gpC_global_resource->m_pC_assemble_box_spk->GetWidth(C_GLOBAL_RESOURCE::AB_UP);
//	}
//	gpC_global_resource->m_pC_assemble_box_spk->BltLocked(_x, y, C_GLOBAL_RESOURCE::AB_RIGHTUP);
//	gpC_global_resource->m_pC_assemble_box_spk->BltLocked(_x, _y, C_GLOBAL_RESOURCE::AB_RIGHTDOWN);
//
//	// center
//	// !각 block의 크기가 다를 수 있다.
//	_y = y+gpC_global_resource->m_pC_assemble_box_spk->GetHeight(C_GLOBAL_RESOURCE::AB_LEFTUP);
//	for (j=0; j < m_center_y; j++)
//	{
//		gpC_global_resource->m_pC_assemble_box_spk->BltLocked(x, 
//				_y+gpC_global_resource->m_pC_assemble_box_spk->GetHeight(C_GLOBAL_RESOURCE::AB_LEFT)*j,
//				C_GLOBAL_RESOURCE::AB_LEFT);
//
//		_x = x+gpC_global_resource->m_pC_assemble_box_spk->GetWidth(C_GLOBAL_RESOURCE::AB_LEFT);
//
//		for (i=0; i < m_center_x; i++)
//		{
//			gpC_global_resource->m_pC_assemble_box_spk->BltLocked(_x, 
//					_y+gpC_global_resource->m_pC_assemble_box_spk->GetHeight(C_GLOBAL_RESOURCE::AB_LEFT)*j,
//					C_GLOBAL_RESOURCE::AB_CENTER);
//
//			_x += gpC_global_resource->m_pC_assemble_box_spk->GetWidth(C_GLOBAL_RESOURCE::AB_UP);
//		}
//
//		gpC_global_resource->m_pC_assemble_box_spk->BltLocked(_x, 
//				_y+gpC_global_resource->m_pC_assemble_box_spk->GetHeight(C_GLOBAL_RESOURCE::AB_LEFT)*j,
//				C_GLOBAL_RESOURCE::AB_RIGHT);
//	}
//	}

	// message
	gpC_base->m_p_DDSurface_back->Lock();
	if (m_msg_rect.h != 0 && !m_vs_msg.empty())
	{
		if (m_message_mode == SMO_FIT)
		{
			int line;
			if (m_line_count > m_print_line_count)
			{
				line = m_print_line_count;

//				if (gpC_base->m_p_DDSurface_back->Lock()) // draw track
//				{
//				if(m_msg_rect.h > 0)
//				{
//					S_SURFACEINFO	surfaceinfo;
//					SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());
//
//					int track_x = m_msg_rect.x+m_msg_rect.w+m_tag_rect.w/2;
//					vertline(&surfaceinfo, track_x, m_msg_rect.y, track_x, m_msg_rect.Down(), BLACK);
//					vertline(&surfaceinfo, track_x+1, m_msg_rect.y, track_x+1, m_msg_rect.Down(), DARKGRAY);
//				}

//				}

//				if (m_available_tag == true)
//					gpC_global_resource->m_pC_assemble_box_etc_spk->BltLocked(m_tag_rect.x, m_tag_rect.y, C_GLOBAL_RESOURCE::AB_SCROLL_TAG);
			}
			else
				line = m_line_count;

//			int scrolled = m_scrollbar.GetAmountToScroll();
			int scrolled = 0;
			if(m_pC_msg_scroll_bar)
				scrolled = m_pC_msg_scroll_bar->GetScrollPos();

			gpC_base->m_p_DDSurface_back->Unlock();
			g_FL2_GetDC();
			for (i=0; i < line; i++)
				g_Print(m_msg_rect.x, 
						  m_msg_rect.y+(g_GetStringHeight(m_vs_msg[i+scrolled].c_str(), gpC_base->m_dialog_msg_pi.hfont)+MSG_EXTRA_HGAP)*i, 
						  m_vs_msg[i+scrolled].c_str(),
						  &gpC_base->m_dialog_msg_pi);
			g_FL2_ReleaseDC();
		}
		else
		{
			gpC_base->m_p_DDSurface_back->Unlock();
			m_line_count=m_vs_msg.size();
			if (m_line_count <= m_print_line_count)
			{
				g_FL2_GetDC();
				for (i=0; i < m_line_count; i++)
					g_Print(m_msg_rect.x+m_msg_rect.w/2-g_GetStringWidth(m_vs_msg[i].c_str(), gpC_base->m_dialog_msg_pi.hfont)/2,
							  m_nofit_mode_msg_y+(g_GetStringHeight(m_vs_msg[i].c_str(), gpC_base->m_dialog_msg_pi.hfont)+MSG_EXTRA_HGAP)*i, 
							  m_vs_msg[i].c_str(), 
							  &gpC_base->m_dialog_msg_pi);
				g_FL2_ReleaseDC();
			}
		}

		// draw boundary line
//		if (m_menu_count > 0)
//		{
//			if (gpC_base->m_p_DDSurface_back->Lock()) // draw track
//			{
//				S_SURFACEINFO	surfaceinfo;
//				SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());
//
//				int b_y = m_msg_rect.y+m_msg_rect.h+4;
//				horzline(&surfaceinfo, m_msg_rect.x, b_y, m_msg_rect.Right(), b_y, BLACK);
//				horzline(&surfaceinfo, m_msg_rect.x, b_y+1, m_msg_rect.Right(), b_y+1, DARKGRAY);
//
//				gpC_base->m_p_DDSurface_back->Unlock();
//			}
//		}
	}
	else
	{
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	m_pC_button_group->Show();
	if(m_pC_msg_scroll_bar)
		m_pC_msg_scroll_bar->Show(m_msg_rect.x, m_msg_rect.y);
	if(m_pC_menu_scroll_bar)
		m_pC_menu_scroll_bar->Show(m_menu_rect.x,m_menu_rect.y);

#ifndef _LIB
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO	surfaceinfo;
		SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());

		//rectangle(&surfaceinfo, this, WHITE);
		//FillRect(&surfaceinfo, &m_client_rect, WHITE);
		//FillRect(&surfaceinfo, &m_menu_rect, BLUE);
		//FillRect(&surfaceinfo, &m_msg_rect, BLUE);

		gpC_base->m_p_DDSurface_back->Unlock();
	}
#endif

	SHOW_WINDOW_ATTR;

}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_DIALOG::Start()
{
	AttrAlpha(g_pUserOption->DefaultAlpha != FALSE);

	PI_Processor::Start();
	m_pC_button_group->Init();

	gpC_window_manager->AppearWindow(this);
	
//	m_tag_pushed = false;
//	m_prev_tag_y = 0;
}

//-----------------------------------------------------------------------------
// StartByPinMode
//
// pin mode로 Start한다.
//-----------------------------------------------------------------------------
void C_VS_UI_DIALOG::StartByPinMode()
{
	AttrTopmost(false);
	AttrPin(true);

	Start();
}

//-----------------------------------------------------------------------------
// Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_DIALOG::Finish()
{	
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// SetMessage
//
// - Message rect = (Client rect - Menu rect - Button rect)
// - height를 구하지 못하면 출력되지 않을 것이다. => 아직구현안함.
//
// parameter format>
//							char * pp_msg[] = {"line1", "line2", ...} // global or local static
//							SetMessage(pp_msg, line_x);
//
// 2002년 7월 23일 추가사항 -by sonee
// 기존의 SetMessage 는 그대로 유지하되, m_flag_menu 를 두어, 메뉴가 있을시에만 스크롤 자동처리가 되도록
// 수정하였다.
// 반드시 SetMenu 가 이루어진후 SetMessage 를 해야 메뉴의 스크롤이 제대로 적용 될 수 있다.
// 만약 일반 다이알로그에서 문제가 생길시 주석처리된 ----[Fix] 부분을 주석처리를 해제해주도록 한다.
// 수정사항에서는 이부분만 변경되었다.
//-----------------------------------------------------------------------------
void C_VS_UI_DIALOG::SetMessage(char ** sz_msg, UINT line_count, SETMESSAGE_MODE_OPTION mode)
{
	assert(sz_msg != NULL);
	assert(line_count > 0);

	if (sz_msg == NULL || line_count == 0)
		return;

	m_message_mode = mode;
	
	
	for(int i = 0; i < line_count; i++)
	{
		char test[2] = " ";
		int font_w = g_GetStringWidth(test, gpC_base->m_dialog_msg_pi.hfont);
		int row = m_client_rect.w / font_w+3;

		char sz_temp[4048];
		char *cur = sz_temp;
		char char_temp;

		strcpy(sz_temp, sz_msg[i]);

		while(1)
		{
			int check;
			if(g_PossibleStringCut(cur, row))check = 0; else check = 1;

			char_temp = cur[row - check];
			cur[row - check] = '\0';
			m_vs_msg.push_back( cur );

			if(strlen(cur) < row-check) break;
			cur += row-check;
			*cur = char_temp;
			if(*cur == ' ')cur++;
		}
	}

	if(!m_vs_msg.empty()) 
		m_line_count = m_vs_msg.size();
	else
		m_line_count=0;

	int m_message_str_height = g_GetStringHeight(m_vs_msg[0].c_str(), gpC_base->m_dialog_msg_pi.hfont)+MSG_EXTRA_HGAP;

	const int _EXTRA = 30;//2;
	if(h == -1)
	{
		h = m_line_count*m_message_str_height;		// Rect 의 높이는 메시지창의 길이로 정한다. 
		if(m_menu_count > 0)
			h += 14 + m_menu_rect.h;

		m_client_rect.h = h-DECORATE_GAP*2-_EXTRA;

		if (y == -1)
			y = (g_GameRect.bottom-102*2)/2-h/2+90;

		m_client_rect.y = y+DECORATE_GAP+_EXTRA;
		m_menu_rect.y = (m_client_rect.y+m_client_rect.h-GetButtonGap()) - m_menu_rect.h;

		int plus = 0;
		int width, height;
		
		for (i=0; i < m_menu_count; i++, plus += (m_menu_str_height-TEXT_EXTRA_HGAP)*height+TEXT_EXTRA_HGAP)
		{
			if(!m_p_menu[i].sz_menu_str.empty())
			{
				height = m_p_menu[i].sz_menu_str.size();
				if(m_p_menu[i].sz_menu_str.size() > 1)width = m_menu_rect.w;
				else width = g_GetStringWidth(m_p_menu[i].sz_menu_str[0].c_str(), gpC_base->m_dialog_menu_pi.hfont);
			}
			else 
			{
				width = 0;
				height = 0;
			}
			
			m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_menu_rect.x,
				m_menu_rect.y+plus, 
				width,
				(m_menu_str_height-TEXT_EXTRA_HGAP)*height,
				m_p_menu[i].exec_id, 
				this, 
				i)); // m_p_menu의 string을 참조하기 위해서 index를 넣는다.
		}
	}

	m_msg_rect.x = m_client_rect.x;
	m_msg_rect.w = m_client_rect.w;
	m_msg_rect.y = m_client_rect.y;	
	
	// 2002년 7월 23일 수정 부분.
	// 메뉴가 있으면 현재 다이알로그의 높이의 반을 기준을 잡는다.
	// 그렇지 않은경우는 기존의 값 그대로 사용.
	
	if(m_flag_menu)
	{
		m_msg_rect.h = m_client_rect.h/2;
	}
	else
	{
		m_msg_rect.h = m_client_rect.h-m_menu_rect.h-GetButtonGap();
		if (m_menu_count > 0)
			m_msg_rect.h -= 14;
	}
	
	// set print line count
	m_print_line_count = (m_msg_rect.h)/m_message_str_height;//+MSG_EXTRA_HGAP
	
	if(m_pC_menu_scroll_bar==NULL&&m_flag_menu)		// 아래쪽 메뉴판에 스크롤바가 생기지 않았을경우
	{
		// 길이를 늘려준다.
		m_msg_rect.h=m_client_rect.h-m_menu_rect.h-28;
		m_print_line_count = (m_msg_rect.h)/m_message_str_height;
	}
	if (mode == SMO_NOFIT)
	{		
		m_nofit_mode_msg_y = m_msg_rect.y+m_msg_rect.h/2-(m_message_str_height*line_count)/2; // sort in center
	}
	else if (m_line_count > m_print_line_count)				// 출력범위가 좁으므로 ScrollBar 를 생성한다.
	{
		// 스크롤바때문에 다시 글씨를 자른다.
		m_vs_msg.clear();
		for(int i = 0; i < line_count; i++)
		{
			char test[2] = " ";
			int font_w = g_GetStringWidth(test, gpC_base->m_dialog_msg_pi.hfont);
			int row = m_client_rect.w / font_w;
			
			char sz_temp[4048];
			char *cur = sz_temp;
			char char_temp;
			
			strcpy(sz_temp, sz_msg[i]);
			
			while(1)
			{
				int check;
				if(g_PossibleStringCut(cur, row))check = 0; else check = 1;
				
				char_temp = cur[row - check];
				cur[row - check] = '\0';
				m_vs_msg.push_back( cur );
				
				if(strlen(cur) < row-check) break;
				cur += row-check;
				*cur = char_temp;
				if(*cur == ' ')cur++;
			}
		}
		m_line_count = m_vs_msg.size();
		
		if(m_pC_msg_scroll_bar)
		{
			delete m_pC_msg_scroll_bar;
			m_pC_msg_scroll_bar = NULL;
		}		
		m_pC_msg_scroll_bar = new C_VS_UI_SCROLL_BAR(m_line_count-m_print_line_count+1, 
			Rect(m_msg_rect.w, m_msg_rect.y-m_client_rect.y+10, -1, m_msg_rect.h-30));
	} else
	if(m_flag_menu)				// 메뉴가 있을 경우
	{
		// 위쪽이 좁기 때문에 스크롤바가 있을 경우 메뉴rect 를 위로 넓혀준다.
		if(m_pC_menu_scroll_bar!=NULL)
		{
			// linelen 은 경계라인 포함해서 메시지의 h 크기이다.
			// m_client.h/2-linelen 은 빈영역이며, 메뉴를 끌어 올릴 수 있는 길이이다.
			int linelen=(m_line_count+1)*m_message_str_height;
			
			m_msg_rect.y=m_client_rect.y;
			m_msg_rect.h=linelen;			

			m_menu_y_size=m_client_rect.h/2-linelen;
			m_menu_rect.y-=m_menu_y_size;
			m_menu_rect.h+=m_menu_y_size;

			delete m_pC_menu_scroll_bar;
			m_pC_menu_scroll_bar=NULL;
			
			int len_menu=m_button_y_list[m_menu_count]-m_temp_menu_rect_y;
			
			// 길이를 늘리고나서, 버튼들을 모두 출력할 수 있을때에는, 창에 맞게 재배치하고,
			// 스크롤을 생성하지 않는다. 
			if(len_menu<m_menu_rect.h)
			{
				len_menu=m_menu_rect.h-len_menu;
				m_menu_y_size-=len_menu;	
			} else
			{				
				m_pC_menu_scroll_bar=new C_VS_UI_SCROLL_BAR(len_menu-m_menu_rect.h, 
					Rect(m_menu_rect.w+20, 14, -1,m_menu_rect.h-20));
			}
		}
	}
}

//-----------------------------------------------------------------------------
// SetMenu
//
// - Client rect에서 Menu rect공간을 확보하지 못하면 출력되지 않을 것이다.
// - 두 번 이상 설정할 수 없다.
// - menu_only가 true이면 dialog center에 맞추고 false이면 message 공간을 위해
//   rect 하단에 위치시킨다.
// 
// 2002년 7월 23일 수정사항				-by sonee
// - Menu Rect 공간을 확보하지 못하였을경우 자동 스크롤바가 붙으며, 메시지 rect
//  와 비교하여 자동으로 공간을 확보한다.
//-----------------------------------------------------------------------------
void C_VS_UI_DIALOG::SetMenu(const DIALOG_MENU * p_dialog_menu, UINT menu_count, bool menu_only)
{
	BOOL fMakeScrollBar=FALSE;
	assert(p_dialog_menu != NULL);
	assert(menu_count > 0);

	int i;

	if (p_dialog_menu == NULL || menu_count == 0)
		return;
	if (m_p_menu != NULL)
		return;

	m_flag_menu=TRUE;
	// set Menu list
	m_menu_count = menu_count;

	if (m_p_menu!=NULL)
		delete [] m_p_menu;

	m_p_menu = new DIALOG_MENU_INNER[m_menu_count];

	// Menu 글자 넘어가는거 자르기.
	for (i=0; i < m_menu_count; i++)
	{
		m_p_menu[i].exec_id = p_dialog_menu[i].exec_id;

		char test[2] = " ";
		int font_w = g_GetStringWidth(test, gpC_base->m_dialog_menu_pi.hfont);

		if(0 == font_w) continue;
		assert(font_w!=0);
		
		int row = (m_client_rect.w / font_w)-2;

		assert(row > 0);

		char sz_temp[2048];
		char *cur = sz_temp;
		char char_temp;

		strcpy(sz_temp, p_dialog_menu[i].sz_menu_str);

		while (*cur!='\0')
		{
			int check;

			if (g_PossibleStringCut(cur, row))
				check = 0; 
			else 
				check = 1;

			int cutLen = row-check;
			assert( cutLen > 0 && cutLen < 2048 );

			char* pCut = cur+cutLen;
			
			char_temp = *pCut;
			*pCut = '\0';
			m_p_menu[i].sz_menu_str.push_back( cur );

			*pCut = char_temp;

			int size = strlen(cur)-1;
			if (strlen(cur) <= cutLen) break;
			cur += cutLen;
			
			while (*cur == ' ') cur++;
		}
	}

	//
	// ! SetMessage()에 의해서 Message가 먼저 설정될 수 있지만, 그것을 고려하지 않는다.
	// 그러니까 VS_UI_Dialog class의 올바른 사용은, SetMenu()를 먼저 해준 후 SetMessage()를
	// 해주는 것이다. Menu rect는 무조건 Client rect의 하단에서 모든 Menu를 배치할 수 있는
	// rect로 설정된다. 그래서 남은 Client rect가 Message 영역으로 되는 것이다.
	//
	//	m_menu_str_height;

	if(!m_p_menu[0].sz_menu_str.empty())
		m_menu_str_height = g_GetStringHeight(m_p_menu[0].sz_menu_str[0].c_str(), gpC_base->m_dialog_menu_pi.hfont)+2+TEXT_EXTRA_HGAP;
	
	m_menu_rect.x = m_client_rect.x+20;
	m_menu_rect.w = m_client_rect.w-20;
	m_menu_rect.h = 0;
	for(i = 0; i < m_menu_count; i++)
	{
		m_menu_rect.h += (m_menu_str_height-TEXT_EXTRA_HGAP)*m_p_menu[i].sz_menu_str.size()+TEXT_EXTRA_HGAP;
			//m_menu_str_height*m_p_menu[i].sz_menu_str.size();
				
	}

	if (menu_only)
		m_menu_rect.y = m_client_rect.y+(m_client_rect.h-GetButtonGap())/2 - m_menu_rect.h/2;
	else
	{
		if(h != -1)
			m_menu_rect.y = (m_client_rect.y+m_client_rect.h) - m_menu_rect.h;
	}
	
	if (m_menu_rect.y < m_client_rect.y + m_client_rect.h / 2 && h != -1)
	{
		m_menu_rect.y=m_client_rect.y + m_client_rect.h/2;
		m_menu_rect.h=m_client_rect.h/2;
		
		if(m_pC_menu_scroll_bar)
			delete m_pC_menu_scroll_bar;
		m_pC_menu_scroll_bar=NULL;		
		fMakeScrollBar=TRUE;
	}
	
	// Create Menu button
	if(h != -1)
	{
		int plus = 0;
		int width, height;		

		if(m_button_y_list!=NULL) delete [] m_button_y_list;
		m_button_y_list=new int[m_menu_count+1];
		
		for (i=0; i < m_menu_count; i++, plus += (m_menu_str_height-TEXT_EXTRA_HGAP)*height+TEXT_EXTRA_HGAP)
		{
			if(!m_p_menu[i].sz_menu_str.empty())
			{
				height = m_p_menu[i].sz_menu_str.size();
				if(m_p_menu[i].sz_menu_str.size() > 1)width = m_menu_rect.w;
				else width = g_GetStringWidth(m_p_menu[i].sz_menu_str[0].c_str(), gpC_base->m_dialog_menu_pi.hfont);
			}
			else 
			{
				width = 0;
				height = 0;
			}
			m_temp_menu_rect_y=m_menu_rect.y;
			m_button_y_list[i]=m_menu_rect.y+plus;
			m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_menu_rect.x,
				m_menu_rect.y+plus,
				width,
				(m_menu_str_height-TEXT_EXTRA_HGAP)*height,
				m_p_menu[i].exec_id, 
				this, 
				i)); // m_p_menu의 string을 참조하기 위해서 index를 넣는다. 이값은 imageindex 에 저장된다.			
		}
		m_button_y_list[i]=m_menu_rect.y+plus;		
	
		if (fMakeScrollBar)
		{
			int linelen=m_button_y_list[m_menu_count]-m_temp_menu_rect_y-m_menu_rect.h;
			m_pC_menu_scroll_bar=new C_VS_UI_SCROLL_BAR(linelen, Rect(m_menu_rect.w, 14, -1,m_menu_rect.h-20));
		}
	}
}


int C_VS_UI_DIALOG::GetScrollPos()
{
	int i,NowPos,now_len;

	if(m_pC_menu_scroll_bar)
		NowPos=m_pC_menu_scroll_bar->GetScrollPos();
	else 
		return 0;

	for(i=0;i<m_menu_count;i++)
	{
		now_len=m_button_y_list[i]-m_temp_menu_rect_y;
		// |-----+-----|
		//    +- 이경우 
		if(NowPos>=now_len&&NowPos<now_len+(m_button_y_list[i+1]-m_button_y_list[i])/2)
		{
			m_pC_menu_scroll_bar->SetScrollPos(m_button_y_list[i]-m_temp_menu_rect_y);
			return i;
		} else
		if(NowPos>=now_len+(m_button_y_list[i+1]-m_button_y_list[i])/2&&NowPos<m_button_y_list[i+1]-m_temp_menu_rect_y)
		{			
			m_pC_menu_scroll_bar->SetScrollPos(m_button_y_list[i+1]-m_temp_menu_rect_y);
			return i+1;
		}
	}
	
	return 0;
}

// --------------------------------------------------------------------------------------
//  C_VS_UI_DIALOG::ProcessMenuScrollBar();
//  
//  Pixel 별로 스크롤을 하기 때문에, 기존에 스크롤바를 사용하면서 수정을 가함.

void C_VS_UI_DIALOG::ProcessMenuScrollBar()
{
	int NowPos,i, MaxPos = 0,half;
	if(m_menu_count>0 && m_button_y_list!= NULL)
	{
		MaxPos=m_button_y_list[m_menu_count]-m_temp_menu_rect_y-m_menu_rect.h;
	}
	
//	BYTE Remain;
	
	if(m_pC_menu_scroll_bar==NULL) return;
	NowPos=m_pC_menu_scroll_bar->GetScrollPos();
	if(NowPos==0) return;
	
	
	for(i=0;i<=m_menu_count;i++)
	{
		if(i<m_menu_count)
		{
			half=(m_button_y_list[i+1]-m_button_y_list[i])>>1;

			// 스크롤을 아래로 하는경우
			if(NowPos>m_button_y_list[i]-m_temp_menu_rect_y&&NowPos<m_button_y_list[i+1]-m_temp_menu_rect_y-half)
			{				
				NowPos=m_button_y_list[i+1]-m_temp_menu_rect_y;

				// 한 화면에 출력이 되는경우 PosMax 를 다시 세팅해준다.
				if(!(m_button_y_list[m_menu_count]-m_temp_menu_rect_y<NowPos+m_menu_rect.h))
					m_pC_menu_scroll_bar->SetScrollPos(NowPos);
				else
					m_pC_menu_scroll_bar->SetPosMax(NowPos+1);
				m_pC_menu_scroll_bar->SetScrollPos(NowPos);
				break;
			}
		}
		if(i>0)
		{
			half=(m_button_y_list[i]-m_button_y_list[i-1])>>1;
			// 스크롤을 위로 하는 경우
			if(NowPos<m_button_y_list[i]-m_temp_menu_rect_y&&NowPos>m_button_y_list[i-1]-m_temp_menu_rect_y+half)
			{
				NowPos=m_button_y_list[i-1]-m_temp_menu_rect_y;
				
				if(!(m_button_y_list[m_menu_count]-m_temp_menu_rect_y<NowPos+m_menu_rect.h))
					m_pC_menu_scroll_bar->SetScrollPos(NowPos);
				
				break;
			}
		}
	}	
}
