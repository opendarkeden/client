/*-----------------------------------------------------------------------------

	VS_UI_widget.h

	VS UI 전용 Widget.

	2000.6.7. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_WIDGET_H__
#define __VS_UI_WIDGET_H__

#include "VS_UI_Base.h"
#include "VS_UI_GlobalResource.h"
#include "CImm.h"
#include "VS_UI_MOUSE_POINTER.h"
/*
//----------------------------------------------------------------------------
// Button Class
//----------------------------------------------------------------------------
class C_VS_UI_BUTTON : public EventButton
{
private:
	enum
	{
		SPEED_MILLISEC = 75,
	};

	bool						m_bl_image_show;

	C_ANIMATION *			m_pC_ani_button;

public:
	C_VS_UI_BUTTON(int _x, int _y, int _w, int _h, id_t id, Exec * pC_exec_handler, bool set_end = false, bool image_show = true);
	~C_VS_UI_BUTTON();

	void Show();
	void EventFocusOn();
	void EventFocusOff();
	void EventPressDown();
	void EventPressUp();
	void SetPosition(int x, int y);
	void Refresh();
};*/

class C_VS_UI_EVENT_BUTTON : public EventButton
{
private:
	DWORD						m_dw_prev_tickcount;
	DWORD						m_dw_millisec;
	bool						m_bl_start;

public:
	int						m_alpha;
	int						m_image_index;

	enum { MAX_ALPHA = 32 };

public:
	C_VS_UI_EVENT_BUTTON(int _x, int _y, int _w, int _h, id_t id, Exec * pC_exec_handler, int image_index=-1, int millisec = 10) : 
									EventButton(_x, _y, _w, _h, id, pC_exec_handler)
	{
		Init();
		m_dw_millisec = millisec;

		if (m_image_index == -1) // default
			m_image_index = id;
		else
			m_image_index = image_index;
	}

	void	Init()
	{
		m_bl_start = false;
		m_alpha = 0;
	}

	void	Timer()
	{
		if (m_bl_start)
		{
			if (m_dw_prev_tickcount+m_dw_millisec <= GetTickCount())
			{
				// next frame!

				if (GetFocusState())
				{
					m_alpha++;
				}
				else
				{
					m_alpha--;
				}

				// !m_alpha = 0인 상태에서 또 m_alpha--가 될 수 있다. 이것은 시간차에 의해서
				// EventFocuxX가 두번이상 실행되기 때문이다.
				if (m_alpha <= 0)
				{
					m_alpha = 0;
					m_bl_start = false;
				}
				
				if (m_alpha >= MAX_ALPHA)
				{
					m_alpha = MAX_ALPHA;
					m_bl_start = false;
				}

				m_dw_prev_tickcount = GetTickCount();
			}
		}
	}

	void	EventFocusOn()
	{
		m_dw_prev_tickcount = GetTickCount();
		m_bl_start = true;
		if(gpC_Imm)
			gpC_Imm->ForceUI(CImm::FORCE_UI_BUTTON);
	}

	void	EventFocusOff()
	{
		m_dw_prev_tickcount = GetTickCount();
		m_bl_start = true;
		m_bl_prev_focus = false;
	}
};

class ButtonVisual
{
public:
	//
	// 하나의 button을 구별하기 위한 id와 button의 상태 flag를 인자로 한다.
	//
	virtual void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button) = 0;

	// Desciption 이 있는부분에만 호출된다.
	// ButtonGroup::ShowDescription(); 으로 호출이 되며, 필요하지 않을경우 사용하지 않아도 상
	virtual void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button){}

};

#ifndef _LIB
extern Button *	gpC_press_button;
#endif

//-----------------------------------------------------------------------------
// ButtonGroup
//
// C_VS_UI_BUTTON2 object를 관리한다.
//-----------------------------------------------------------------------------
class ButtonGroup : public SimpleDataList<C_VS_UI_EVENT_BUTTON *>
{
private:
	ButtonVisual *				m_p_button_visual;

public:
	ButtonGroup(ButtonVisual * p_visual) { m_p_button_visual = p_visual; }
	~ButtonGroup()
	{
		C_VS_UI_EVENT_BUTTON * data;
		for (int i=0; i < Size(); i++)
			if (Data(i, data))
				delete data;
	}

	C_VS_UI_EVENT_BUTTON * GetButton(int id)
	{
		C_VS_UI_EVENT_BUTTON * data;
		for (int i=0; i < Size(); i++)
			if (Data(i, data))
				if ((int)(data->GetID()) == id)
					return data;

		return NULL;
	}

	C_VS_UI_EVENT_BUTTON * GetFocusedButton()
	{
		C_VS_UI_EVENT_BUTTON * data;
		for (int i=0; i < Size(); i++)
			if (Data(i, data))
				if (data->GetFocusState())
					return data;

		return NULL;
	}

	
	C_VS_UI_EVENT_BUTTON * IsInRect(int _x, int _y)
	{
		C_VS_UI_EVENT_BUTTON * data;
		for (int i=0; i < Size(); i++)
			if (Data(i, data))
				if (data->IsInRect(_x, _y))
					return data;

		return NULL;
	}

	void	UnacquireMouseFocus()
	{
		C_VS_UI_EVENT_BUTTON * data;
		for (int i=0; i < Size(); i++)
			if (Data(i, data))
			{
				data->FocusOff();
				data->EventFocusOff();
			}
	}

	void	CancelPushState()
	{
		C_VS_UI_EVENT_BUTTON * data;
		for (int i=0; i < Size(); i++)
			if (Data(i, data))
			{
				data->PressUp();
			}
	}

	void	Init()
	{
		C_VS_UI_EVENT_BUTTON * data;
		for (int i=0; i < Size(); i++)
			if (Data(i, data))
			{
				data->Init();
				CancelPushState();
				UnacquireMouseFocus();
			}
	}

	void	ShowDescription()
	{
		C_VS_UI_EVENT_BUTTON *data;
		for(int i=0;i< Size();i++)
			if(Data(i,data))
				if(data->GetFocusState())
				{
					m_p_button_visual->ShowButtonDescription(data);
					return;
				}
	}

	void	Show()
	{ 
		C_VS_UI_EVENT_BUTTON * data;
		for (int i=0; i < Size(); i++)
			if (Data(i, data))
				m_p_button_visual->ShowButtonWidget(data);
	}

	void	Process()
	{ 
		C_VS_UI_EVENT_BUTTON * data;
		for (int i=0; i < Size(); i++)
			if (Data(i, data))
				data->Timer();
	}

	bool	MouseControl(UINT message, int _x, int _y)
	{
		bool re = true;
		C_VS_UI_EVENT_BUTTON * data;
		for (int i=0; i < Size(); i++)
			if (Data(i, data))
			{
#ifndef _LIB
				if (!g_GetCtrlPushState())
#endif
					re = data->MouseControl(message, _x, _y) && re;

#ifndef _LIB
				static bool press;
				static int gapx, gapy;

				switch (message)
				{
					case M_LEFTBUTTON_DOWN:
					case M_LB_DOUBLECLICK:
						if (data->IsInRect(_x, _y) && g_GetCtrlPushState())
						{
							gapx = _x-data->x;
							gapy = _y-data->y;
							press = true;
							gpC_press_button = data;
						}
						break;

					case M_LEFTBUTTON_UP:
						press = false;
						gpC_press_button = NULL;
						break;

					case M_MOVING:
						if (press && gpC_press_button)
						{
							gpC_press_button->x = _x - gapx;
							gpC_press_button->y = _y - gapy;
						}
						break;
				}
#endif
			}
		return re;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////
// 스크롤바 클래스이다
// 내부적으로 Show를 가지고 있으며 기본 스크롤바 spk를 출력한다.
// 다른 spk를 사용하도록 설정할수 있으나, 스프라이트의 순서는 기본 spk와 같아야 한다.
////////////////////////////////////////////////////////////////////////////////////////////

class C_VS_UI_SCROLL_BAR : public Rect
{
private:
	C_SPRITE_PACK	*m_spk;
	int				m_pos_max;
	int				m_bar_plus, m_tag_plus, m_button_plus;
	int				m_pos;
	bool			m_tag_pushed, m_up_button_focused, m_down_button_focused, m_up_button_pushed, m_down_button_pushed;
	int				m_tag_height, m_button_height, m_button_width;
	bool			m_bl_reverse, m_bHeight;


public:
	C_VS_UI_SCROLL_BAR(int pos_max, const Rect scroll_rect, bool bl_reverse = false, const char *scroll_bar_spk_name = NULL, int bar_plus = 6, int tag_plus = 6, int button_plus = 0, bool bHeight = true)
	{
		m_spk = NULL;
		if(scroll_bar_spk_name)
			m_spk = new C_SPRITE_PACK(scroll_bar_spk_name);

		C_SPRITE_PACK *spk = m_spk;
		if(spk == NULL)
			spk = gpC_global_resource->m_pC_scroll_bar_spk;

		Set(scroll_rect.x, scroll_rect.y, scroll_rect.w, scroll_rect.h);
		if(bHeight)
		{
			if(scroll_rect.w == -1)
			{
				w = spk->GetWidth(C_GLOBAL_RESOURCE::SB_BUTTON);
			}
		}
		else
		{
			if(scroll_rect.h == -1)
			{
				h = spk->GetHeight(C_GLOBAL_RESOURCE::SB_BUTTON);
			}
		}

		m_pos_max = pos_max;
		m_bar_plus = bar_plus;
		m_tag_plus = tag_plus;
		m_button_plus = button_plus;

		m_up_button_focused = false;
		m_down_button_focused = false;
		m_up_button_pushed = false;
		m_down_button_pushed = false;
		m_tag_pushed = false;
		m_bl_reverse = bl_reverse;
		m_bHeight = bHeight;

		m_button_width = spk->GetWidth(C_GLOBAL_RESOURCE::SB_BUTTON);
		m_button_height = spk->GetHeight(C_GLOBAL_RESOURCE::SB_BUTTON);
		m_tag_height = spk->GetHeight(C_GLOBAL_RESOURCE::SB_TAG);

		m_pos = 0;
	}

	C_VS_UI_SCROLL_BAR()
	{
		m_spk = NULL;

		Set(-1, -1, -1, -1);

		m_pos_max = -1;
		m_bar_plus = 6;
		m_tag_plus = 6;
		m_button_plus = 0;

		m_up_button_focused = false;
		m_down_button_focused = false;
		m_up_button_pushed = false;
		m_down_button_pushed = false;
		m_tag_pushed = false;
		m_bl_reverse = false;
		m_bHeight = true;

		m_button_height = gpC_global_resource->m_pC_scroll_bar_spk->GetHeight(C_GLOBAL_RESOURCE::SB_BUTTON);
		m_tag_height = gpC_global_resource->m_pC_scroll_bar_spk->GetHeight(C_GLOBAL_RESOURCE::SB_TAG);

		m_pos = 0;
	}

	~C_VS_UI_SCROLL_BAR()
	{
		if(m_spk)
		{
			delete m_spk;
			m_spk = NULL;
		}
	}

	void	SetSize(Rect scroll_rect)
	{
		C_SPRITE_PACK *spk = m_spk;
		if(spk == NULL)
			spk = gpC_global_resource->m_pC_scroll_bar_spk;

		Set(scroll_rect.x, scroll_rect.y, scroll_rect.w, scroll_rect.h);
		if(m_bHeight)
		{
			if(scroll_rect.w == -1)
			{
				w = spk->GetWidth(C_GLOBAL_RESOURCE::SB_BUTTON);
			}
		}
		else
		{
			if(scroll_rect.h == -1)
			{
				h = spk->GetHeight(C_GLOBAL_RESOURCE::SB_BUTTON);
			}
		}
	}

	void	Show(int _x, int _y)
	{
		C_SPRITE_PACK *spk = m_spk;
		if(spk == NULL)
			spk = gpC_global_resource->m_pC_scroll_bar_spk;

		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			if(spk)
			{
				Rect rect;
				if(m_bHeight)
				{
					rect.Set(0, 0, spk->GetWidth(C_GLOBAL_RESOURCE::SB_BAR), h);
					spk->BltLockedClip(_x+x+m_bar_plus, _y+y, rect, C_GLOBAL_RESOURCE::SB_BAR);

					if(m_up_button_focused)
					{
						if(m_up_button_pushed)
							spk->BltLocked(_x+x+m_button_plus, _y+y-m_button_height, C_GLOBAL_RESOURCE::SB_BUTTON_HILIGHTED_PUSHED);
						else
							spk->BltLocked(_x+x+m_button_plus, _y+y-m_button_height, C_GLOBAL_RESOURCE::SB_BUTTON_HILIGHTED);
					}
					else
					{
						if(m_up_button_pushed)
							spk->BltLocked(_x+x+m_button_plus, _y+y-m_button_height, C_GLOBAL_RESOURCE::SB_BUTTON_PUSHED);
						else
							spk->BltLocked(_x+x+m_button_plus, _y+y-m_button_height, C_GLOBAL_RESOURCE::SB_BUTTON);
					}
					if(m_down_button_focused)
					{
						if(m_down_button_pushed)
							spk->BltLocked(_x+x+m_button_plus, _y+y+h, C_GLOBAL_RESOURCE::SB_BUTTON_HILIGHTED_PUSHED);
						else
							spk->BltLocked(_x+x+m_button_plus, _y+y+h, C_GLOBAL_RESOURCE::SB_BUTTON_HILIGHTED);
					}
					else
					{
						if(m_down_button_pushed)
							spk->BltLocked(_x+x+m_button_plus, _y+y+h, C_GLOBAL_RESOURCE::SB_BUTTON_PUSHED);
						else
							spk->BltLocked(_x+x+m_button_plus, _y+y+h, C_GLOBAL_RESOURCE::SB_BUTTON);
					}
					if(m_pos_max > 1)
					{
						if(m_bl_reverse)
							m_pos = (m_pos_max-1) - m_pos;
						
						if(m_tag_pushed)
						{
							spk->BltLocked(_x+x+m_tag_plus, min(_y+y+h-m_tag_height, max(_y+y, gpC_mouse_pointer->GetY()-m_tag_height/2)), C_GLOBAL_RESOURCE::SB_TAG);
						}
						else
							spk->BltLocked(_x+x+m_tag_plus, _y+y+m_pos*(h-m_tag_height)/(m_pos_max-1), C_GLOBAL_RESOURCE::SB_TAG);
						
						if(m_bl_reverse)
							m_pos = (m_pos_max-1) - m_pos;
					}
				}
				else
				{
					rect.Set(0, 0, w, spk->GetHeight(C_GLOBAL_RESOURCE::SB_BAR_WIDTH));
					spk->BltLockedClip(_x+x, _y+y+m_bar_plus, rect, C_GLOBAL_RESOURCE::SB_BAR_WIDTH);

					if(m_up_button_focused)
					{
						if(m_up_button_pushed)
							spk->BltLocked(_x+x-m_button_width, _y+y+m_button_plus, C_GLOBAL_RESOURCE::SB_BUTTON_HILIGHTED_PUSHED);
						else
							spk->BltLocked(_x+x-m_button_width, _y+y+m_button_plus, C_GLOBAL_RESOURCE::SB_BUTTON_HILIGHTED);
					}
					else
					{
						if(m_up_button_pushed)
							spk->BltLocked(_x+x-m_button_width, _y+y+m_button_plus, C_GLOBAL_RESOURCE::SB_BUTTON_PUSHED);
						else
							spk->BltLocked(_x+x-m_button_width, _y+y+m_button_plus, C_GLOBAL_RESOURCE::SB_BUTTON);
					}
					if(m_down_button_focused)
					{
						if(m_down_button_pushed)
							spk->BltLocked(_x+x+w, _y+y+m_button_plus, C_GLOBAL_RESOURCE::SB_BUTTON_HILIGHTED_PUSHED);
						else
							spk->BltLocked(_x+x+w, _y+y+m_button_plus, C_GLOBAL_RESOURCE::SB_BUTTON_HILIGHTED);
					}
					else
					{
						if(m_down_button_pushed)
							spk->BltLocked(_x+x+w, _y+y+m_button_plus, C_GLOBAL_RESOURCE::SB_BUTTON_PUSHED);
						else
							spk->BltLocked(_x+x+w, _y+y+m_button_plus, C_GLOBAL_RESOURCE::SB_BUTTON);
					}
					if(m_pos_max > 1)
					{
						if(m_bl_reverse)
							m_pos = (m_pos_max-1) - m_pos;
						
						if(m_tag_pushed)
						{
							spk->BltLocked(min(_x+x+w-m_tag_height, max(_x+x, gpC_mouse_pointer->GetX()-m_tag_height/2)), _y+y+m_tag_plus, C_GLOBAL_RESOURCE::SB_TAG_WIDTH);
						}
						else
							spk->BltLocked(_x+x+m_pos*(w-m_tag_height)/(m_pos_max-1), _y+y+m_tag_plus, C_GLOBAL_RESOURCE::SB_TAG_WIDTH);
						
						if(m_bl_reverse)
							m_pos = (m_pos_max-1) - m_pos;
					}
				}

			}
			gpC_base->m_p_DDSurface_back->Unlock();
		}
	}
	void	Show2(int _x, int _y)
	{
		C_SPRITE_PACK *spk = m_spk;
		if(spk == NULL)
			spk = gpC_global_resource->m_pC_scroll_bar_spk;

		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			if(spk)
			{
				Rect rect;
				rect.Set(0, 0, spk->GetWidth(C_GLOBAL_RESOURCE::SB_BAR), h);
				spk->BltLockedClip(_x+x, _y+y, rect, C_GLOBAL_RESOURCE::SB_BAR);

				if(m_up_button_focused)
				{
					if(m_up_button_pushed)
						spk->BltLocked(_x+x+m_button_plus, _y+y-m_button_height, C_GLOBAL_RESOURCE::SB_BUTTON_PUSHED);
					else
						spk->BltLocked(_x+x+m_button_plus, _y+y-m_button_height, C_GLOBAL_RESOURCE::SB_BUTTON);
				}
				if(m_down_button_focused)
				{
					if(m_down_button_pushed)
						spk->BltLocked(_x+x+m_button_plus, _y+y+h, C_GLOBAL_RESOURCE::SB_BUTTON_HILIGHTED_PUSHED);
					else
						spk->BltLocked(_x+x+m_button_plus, _y+y+h, C_GLOBAL_RESOURCE::SB_BUTTON_HILIGHTED);
				}
				if(m_pos_max > 1)
				{
					if(m_bl_reverse)
						m_pos = (m_pos_max-1) - m_pos;
					spk->BltLocked(_x+x, _y+y+m_pos*(h-m_tag_height)/(m_pos_max-1), C_GLOBAL_RESOURCE::SB_TAG);
					
					if(m_bl_reverse)
						m_pos = (m_pos_max-1) - m_pos;
				}
			}
			gpC_base->m_p_DDSurface_back->Unlock();
		}
	}
	void	Show3(int _x, int _y)
	{
		C_SPRITE_PACK *spk = m_spk;
		if(spk == NULL)
			spk = gpC_global_resource->m_pC_scroll_bar_spk;

		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			if(spk)
			{
				Rect rect;
				rect.Set(0, 0, spk->GetWidth(C_GLOBAL_RESOURCE::SB_BAR), h);
				spk->BltLockedClip(_x+x, _y+y, rect, C_GLOBAL_RESOURCE::SB_BAR);

				if(m_up_button_focused)
				{
					if(m_up_button_pushed)
						spk->BltLocked(_x+x+m_button_plus, _y+y-m_button_height, C_GLOBAL_RESOURCE::SB3_BUTTON_UP_PUSHED);
					else
						spk->BltLocked(_x+x+m_button_plus, _y+y-m_button_height, C_GLOBAL_RESOURCE::SB3_BUTTON_UP_HILIGHTED);
				}
				else
					spk->BltLocked(_x+x+m_button_plus, _y+y-m_button_height, C_GLOBAL_RESOURCE::SB3_BUTTON_UP);
				if(m_down_button_focused)
				{
					if(m_up_button_pushed)
						spk->BltLocked(_x+x+m_button_plus, _y+y+h, C_GLOBAL_RESOURCE::SB3_BUTTON_DOWN_PUSHED);
					else
						spk->BltLocked(_x+x+m_button_plus, _y+y+h, C_GLOBAL_RESOURCE::SB3_BUTTON_DOWN_HILIGHTED);
				}
				else
					spk->BltLocked(_x+x+m_button_plus, _y+y+h, C_GLOBAL_RESOURCE::SB3_BUTTON_DOWN);
				
				if(m_pos_max > 1)
				{
					if(m_bl_reverse)
						m_pos = (m_pos_max-1) - m_pos;
					spk->BltLocked(_x+x+m_tag_plus, _y+y+m_pos*(h-m_tag_height)/(m_pos_max-1), C_GLOBAL_RESOURCE::SB3_TAG);
					
					if(m_bl_reverse)
						m_pos = (m_pos_max-1) - m_pos;
				}
			}
			gpC_base->m_p_DDSurface_back->Unlock();
		}
	}
	bool	MouseControl(UINT message, int _x, int _y)
	{
		switch(message)
		{
		case M_MOVING:
			if(m_bHeight)
			{
				if(m_tag_pushed)
				{
					SetScrollPixel(_y);
				}
				else if(_x > x && _x < x+w && _y < y && _y > y-m_button_height)
				{
					if(m_up_button_focused == false)
					{
						m_up_button_focused = true;
						if(gpC_Imm)
							gpC_Imm->ForceUI(CImm::FORCE_UI_BUTTON);
					}
					m_down_button_focused = false;
				}
				else if(_x > x && _x < x+w && _y > y+h && _y < y+h+m_button_height)
				{
					if(m_down_button_focused == false)
					{
						m_down_button_focused = true;
						if(gpC_Imm)
							gpC_Imm->ForceUI(CImm::FORCE_UI_BUTTON);
					}
					m_up_button_focused = false;
				}
				else
				{
					m_up_button_focused = false;
					m_down_button_focused = false;
				}
			}
			else
			{
				if(m_tag_pushed)
				{
					SetScrollPixel(_x);
				}
				else if(_y > y && _y < y+h && _x < x && _x > x-m_button_width)
				{
					if(m_up_button_focused == false)
					{
						m_up_button_focused = true;
						if(gpC_Imm)
							gpC_Imm->ForceUI(CImm::FORCE_UI_BUTTON);
					}
					m_down_button_focused = false;
				}
				else if(_y > y && _y < y+h && _x > x+w && _x < x+w+m_button_width)
				{
					if(m_down_button_focused == false)
					{
						m_down_button_focused = true;
						if(gpC_Imm)
							gpC_Imm->ForceUI(CImm::FORCE_UI_BUTTON);
					}
					m_up_button_focused = false;
				}
				else
				{
					m_up_button_focused = false;
					m_down_button_focused = false;
				}
			}
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			if(m_up_button_focused)
			{
				m_up_button_pushed = true;
				return false;
			}
			else if(m_down_button_focused)
			{
				m_down_button_pushed = true;
				return false;
			}
			else
			{
				if(m_bHeight)
				{
					if(_x > x && _x < x+w && _y > y && _y < y+h)
					{
						SetScrollPixel(_y);
						m_tag_pushed = true;
						return false;
					}
				}
				else
				{
					if(_y > y && _y < y+h && _x > x && _x < x+w)
					{
						SetScrollPixel(_x);
						m_tag_pushed = true;
						return false;
					}
				}
			}
			break;

		case M_LEFTBUTTON_UP:
			m_tag_pushed = false;
			if(m_up_button_pushed)
			{
				m_up_button_pushed = false;
				ScrollUp();
				return false;
			}
			else if(m_down_button_pushed)
			{
				m_down_button_pushed = false;
				ScrollDown();
				return false;
			}
			break;
		}
		return true;
	}

	void	CancelPushState()
	{
		m_up_button_pushed = false;
		m_down_button_pushed = false;
		m_tag_pushed = false;
	}

	void	ScrollUp(int pos = 1)
	{
		if(m_bl_reverse)
		{
			m_pos = min(m_pos_max-1, m_pos+pos);
		}
		else
		{
			m_pos = max(0, m_pos-pos);
		}
	}

	void	ScrollDown(int pos = 1)
	{
		if(m_bl_reverse)
		{
			m_pos = max(0, m_pos-pos);
		}
		else
		{
			m_pos = min(m_pos_max-1, m_pos+pos);
		}
	}

	void	SetScrollPos(int pos)
	{
		m_pos = max(0, min(m_pos_max-1, pos));
	}

	void	SetScrollPixel(int _pixel)
	{
		if(m_bHeight)
			SetScrollPos((_pixel-y-m_tag_height/2)*m_pos_max/(h-m_tag_height));
		else
			SetScrollPos((_pixel-x-m_tag_height/2)*m_pos_max/(w-m_tag_height));
	}

	int		GetScrollPos()
	{
		return m_pos;
	}

	void	SetPosMax(int max)	//pos_max는 스크롤될 항목의 개수이다. 만약 한 화면에 5개의 항목이 나오고, 총 10개의 항목이 있다면 스크롤값은 0~5 까지 가지므로 pos_max == 6 이다. 
	{
		m_pos = 0;
		m_pos_max = max;
	}

	void	SetReverse(bool bl_reverse)
	{
		m_bl_reverse = bl_reverse;
	}
};
#endif