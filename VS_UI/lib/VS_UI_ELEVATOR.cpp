// VS_UI_ELEVATOR.cpp: implementation of the C_VS_UI_ELEVATOR class.
//
//////////////////////////////////////////////////////////////////////

#include "client_PCH.h"

#pragma warning(disable:4786)

#include "VS_UI_ELEVATOR.h"
#include "VS_UI_filepath.h"
#include "VS_UI_description.h"
#include "VS_UI.h" // include VS UI master header file.

#include "MGameStringTable.H"
extern RECT g_GameRect;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C_VS_UI_ELEVATOR::C_VS_UI_ELEVATOR()
{
	g_RegisterWindow(this);

	AttrTopmost(true);

	m_image_spk.Open(SPK_ELEVATOR);

	int ok_button_x = 15, ok_button_y = 256;
	int close_button_x = 100, close_button_y = 256;
	int button_x = 41, button_y = 28, button_y_gap = 56;

	Set(g_GameRect.right/2-m_image_spk.GetWidth(ELEVATOR)/2, g_GameRect.bottom/2 - m_image_spk.GetHeight(ELEVATOR)/2, m_image_spk.GetWidth(ELEVATOR), m_image_spk.GetHeight(ELEVATOR));

	// set button
	m_pC_button_group = new ButtonGroup(this);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + ok_button_x, y + ok_button_y, m_image_spk.GetWidth(OK_BUTTON), m_image_spk.GetHeight(OK_BUTTON), OK_BUTTON, this, OK_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + close_button_x, y + close_button_y, m_image_spk.GetWidth(CLOSE_BUTTON), m_image_spk.GetHeight(CLOSE_BUTTON), CLOSE_BUTTON, this, CLOSE_BUTTON));
//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + button_x, y + button_y + button_y_gap*0, m_image_spk.GetWidth(LEVEL_4), m_image_spk.GetHeight(LEVEL_4), LEVEL_4, this, LEVEL_4));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + button_x, y + button_y + button_y_gap*0, m_image_spk.GetWidth(LEVEL_3), m_image_spk.GetHeight(LEVEL_3), LEVEL_3, this, LEVEL_3));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + button_x, y + button_y + button_y_gap*1, m_image_spk.GetWidth(LEVEL_2), m_image_spk.GetHeight(LEVEL_2), LEVEL_2, this, LEVEL_2));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + button_x, y + button_y + button_y_gap*2, m_image_spk.GetWidth(LEVEL_1), m_image_spk.GetHeight(LEVEL_1), LEVEL_1, this, LEVEL_1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x + button_x, y + button_y + button_y_gap*3, m_image_spk.GetWidth(LEVEL_B1), m_image_spk.GetHeight(LEVEL_B1), LEVEL_B1, this, LEVEL_B1));

	m_iLevel = -1;

}

C_VS_UI_ELEVATOR::~C_VS_UI_ELEVATOR()
{
	g_UnregisterWindow(this);

	DeleteNew( m_pC_button_group );
}

//-----------------------------------------------------------------------------
// C_VS_UI_ELEVATOR::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_ELEVATOR::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_ELEVATOR::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_ELEVATOR::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_ELEVATOR::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_ELEVATOR::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_ELEVATOR::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_ELEVATOR::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_ELEVATOR::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_ELEVATOR::Show()
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_image_spk.BltLocked(x, y, ELEVATOR);
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	m_pC_button_group->ShowDescription();

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// C_VS_UI_ELEVATOR::ShowButtonDescription
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_ELEVATOR::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char* m_help_string[7] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MOVE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CANCEL_MOVE].GetString(),		
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_MOVE_F4].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MOVE_F3].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MOVE_F2].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MOVE_F1].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MOVE_B1].GetString(),
	};
	
	g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()-1],0,0);
}

//-----------------------------------------------------------------------------
// C_VS_UI_ELEVATOR::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_ELEVATOR::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);
	
	if(p_button->GetFocusState() && p_button->GetPressState())m_image_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index);
	else if(m_iLevel == p_button->GetID())
		m_image_spk.BltLocked(p_button->x-1, p_button->y-1, p_button->m_image_index);
}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_ELEVATOR::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
			case VK_ESCAPE: 
				Run(CLOSE_BUTTON);
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_ELEVATOR::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_ELEVATOR::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	m_pC_button_group->MouseControl(message, _x, _y);

	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_ELEVATOR::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_ELEVATOR::IsPixel(int _x, int _y)
{
	return m_image_spk.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
}

//-----------------------------------------------------------------------------
// C_VS_UI_ELEVATOR::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_ELEVATOR::Run(id_t id)
{
	int zone_id = gC_vs_ui.GetZoneID()%10;
	
	switch (id)
	{
	case OK_BUTTON:
		if(m_iLevel != -1)
			gpC_base->SendMessage(UI_SELECT_ELEVATOR, m_iLevel - LEVEL_3 + 1);
		break;

	case CLOSE_BUTTON:
		gpC_base->SendMessage(UI_CLOSE_ELEVATOR);
		break;

//	case LEVEL_4:
//		if(zone_id != 4)
//		m_iLevel = LEVEL_4;
//		break;

	case LEVEL_3:
		if(zone_id != 3)
		m_iLevel = LEVEL_3;
		break;

	case LEVEL_2:
		if(zone_id != 2)
		m_iLevel = LEVEL_2;
		break;

	case LEVEL_1:
		if(zone_id != 1)
		m_iLevel = LEVEL_1;
		break;

	case LEVEL_B1:
		if(zone_id != 0)
		m_iLevel = LEVEL_B1;
		break;
	}
}
