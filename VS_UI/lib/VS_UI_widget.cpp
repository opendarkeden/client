#include "client_PCH.h"
#include <assert.h>
#include "VS_UI_widget.h"
#include "VS_UI_filepath.h"

#ifndef _LIB
Button *	gpC_press_button;
#endif

//bool ButtonGroup::press;

//----------------------------------------------------------------------------
// Operations
//----------------------------------------------------------------------------
/*
//-----------------------------------------------------------------------------
// SetPosition
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_BUTTON::SetPosition(int x, int y)
{
	m_pC_ani_button->SetPlayPosition(x, y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_BUTTON
//
// set_end는 ani button을 focus rect의 끝 정렬여부를 결정한다.
//-----------------------------------------------------------------------------
C_VS_UI_BUTTON::C_VS_UI_BUTTON(int _x, int _y, int _w, int _h, id_t id, Exec * pC_exec_handler, bool set_end, bool image_show) : 
						EventButton(_x, _y, _w, _h, id, pC_exec_handler)
{
	assert(gpC_global_resource);
	assert(gpC_global_resource->m_pC_ao_ani_button);

	m_pC_ani_button = new C_ANIMATION(gpC_global_resource->m_pC_ao_ani_button);

	m_pC_ani_button->SetSpeed(SPEED_MILLISEC);

	int px, py;
	if (set_end)
	{
		px = x+w-gpC_global_resource->m_pC_ao_ani_button->GetWidth(0);
		py = y;
	}
	else
	{
		px = x;
		py = y;
	}

	m_pC_ani_button->SetPlayPosition(px, py);
	m_bl_image_show = image_show;
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_BUTTON
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_BUTTON::~C_VS_UI_BUTTON()
{
	DeleteNew(m_pC_ani_button);
}

//-----------------------------------------------------------------------------
// Refresh
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_BUTTON::Refresh()
{
	InitState();
	m_pC_ani_button->Refresh();
}

//-----------------------------------------------------------------------------
// Show
//
// Button을 보인다.
//-----------------------------------------------------------------------------
void C_VS_UI_BUTTON::Show()
{
	if (m_bl_image_show)
		m_pC_ani_button->Show();
}

//-----------------------------------------------------------------------------
// EventFocusOn
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_BUTTON::EventFocusOn()
{
	// animation play
	switch (m_pC_ani_button->GetAnimationState())
	{
		case C_ANIMATION::PLAY_BACK:
			m_pC_ani_button->SetNextPlayOrder(C_ANIMATION::PLAY);
			break;

		case C_ANIMATION::STOP:
			m_pC_ani_button->Play();
			break;

		case C_ANIMATION::PLAY:
			m_pC_ani_button->SetNextPlayOrder(C_ANIMATION::STOP);
			break;
	}
}

//-----------------------------------------------------------------------------
// EventFocusOff
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_BUTTON::EventFocusOff()
{
	// animation play back
	switch (m_pC_ani_button->GetAnimationState())
	{
		case C_ANIMATION::PLAY:
			m_pC_ani_button->SetNextPlayOrder(C_ANIMATION::PLAY_BACK);
			break;

		case C_ANIMATION::STOP:
			m_pC_ani_button->PlayBack();
			break;

		case C_ANIMATION::PLAY_BACK:
			m_pC_ani_button->SetNextPlayOrder(C_ANIMATION::STOP);
			break;
	}
}

//-----------------------------------------------------------------------------
// EventPressDown
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_BUTTON::EventPressDown()
{
//	m_pC_ani_button->SetPlayPosition(m_x+2, m_y+2);
}

//-----------------------------------------------------------------------------
// UpAction
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_BUTTON::EventPressUp()
{
//	m_pC_ani_button->SetPlayPosition(m_x, m_y);
}*/