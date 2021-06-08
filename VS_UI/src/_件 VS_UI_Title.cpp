// VS_UI_Title.cpp

#include "client_PCH.h"
#include "VS_UI.h"

#include "VS_UI_filepath.h"
#include "VS_UI_Message.h"
#include "VS_UI_Mouse_pointer.h"
#include "VS_UI_GameCommon.h"
#include "UserOption.h"
#include "ClientConfig.h"

//#include "ExperienceTable.h"	//char_info지울때 지울것
#include "MGuildMarkManager.h"
#include "KeyAccelerator.h"
#include "UserInformation.h"
#include <time.h>

#include "MItemOptionTable.H"
#include "mgamestringtable.H"
#include "SkinManager.h"

//add by zdj 
#include <process.h>

#define LOGIN_ID_X 59 // 상대값
#define LOGIN_ID_Y 49
#define LOGIN_PASSWORD_X 59
#define LOGIN_PASSWORD_Y 89

#define	MAX_SOUND_VOLUME		15
#define	MAX_MUSIC_VOLUME		15
#define MIN_GAMMA_VALUE			50
#define	MAX_GAMMA_VALUE			100
#define	MAX_MOUSE_SPEED			100
#define MAX_ALPHA_DEPTH			31
//bool gbl_option_running = false;

#ifdef _LIB
	extern BOOL g_bEnable3DHAL;
#else
	BOOL g_bEnable3DHAL = TRUE;
#endif

extern bool		g_bFamily;
extern DWORD g_CurrentFrame;
extern int		g_LeftPremiumDays;
// add by sonic 2006.9.26
extern	BOOL	g_MyFull;
extern RECT g_GameRect;
/*
//----------------------------------------------------------------------------
// static
//----------------------------------------------------------------------------
C_VS_UI_OPTION::GAMEMENU_SPK_INDEX			
C_VS_UI_OPTION::m_sprite_id[C_VS_UI_OPTION::MENU_COUNT][4] =
// { normal, focused, checked, focused&checked }
{
	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //USE_3D_HAL,
//	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //DRAW_MINIMAP,
//	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //DRAW_ZONE_NAME,
//	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //DRAW_GAME_TIME,
//	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //DRAW_BLENDING_CHARACTER_SHADOW,
	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //PLAY_SOUND,
	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //PLAY_MUSIC,
	{ SPK_SCROLL_NORMAL_SELECT, SPK_SCROLL_FOCUSED_SELECT, SPK_SCROLL_NORMAL_SELECT, SPK_SCROLL_FOCUSED_SELECT }, //VOLUME_SOUND,
	{ SPK_SCROLL_NORMAL_SELECT, SPK_SCROLL_FOCUSED_SELECT, SPK_SCROLL_NORMAL_SELECT, SPK_SCROLL_FOCUSED_SELECT }, //VOLUME_MUSIC,
	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //PLAY_WAV,
	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //PLAY_MIDI,
	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //FILTERING_CURSE,
	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //VIEW_BLOOD,
	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //PARTY_HPBAR_ALPHA,
	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //IFEEL
	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //USE_GAMMA
	{ SPK_SCROLL_NORMAL_SELECT, SPK_SCROLL_FOCUSED_SELECT, SPK_SCROLL_NORMAL_SELECT, SPK_SCROLL_FOCUSED_SELECT }, //GAMMA_VALUE
	{ SPK_NORMAL_BOX, SPK_FOCUSED_BOX, SPK_CHECKED_BOX, SPK_FOCUSED_CHECKED_BOX }, //CHAT_BOX
};

// by sigi
void ExecF_TitleOption(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
		case DIALOG_EXECID_CANCEL:
		case DIALOG_EXECID_EXIT:
		case DIALOG_EXECID_OK:
			gpC_base->SendMessage( UI_CLOSE_TITLE_OPTION );
			gbl_title_option_running = false;
		break;
	}

	gC_vs_ui.AcquireChatting();
}
*/
//----------------------------------------------------------------------------
// Prototype
//----------------------------------------------------------------------------
void _Timer_CharUpdate();
void _Timer_CharUpdate2();
void _Timer_CharUpdate3();

//----------------------------------------------------------------------------
// Globals
//----------------------------------------------------------------------------
timer_id_t					g_char_update_tid = INVALID_TID;

int							g_char_index;

const int TITLE_X = 400, TITLE_Y = 50;
// heart rect
#define HEART_WIDTH			167
#define HEART_HEIGHT		271
#define HEART_Y				180
int g_heart_rect[] = {250, 430, 610};

namespace					// 2003.9.29		by sonee 딴데선 안쓴다 ㅎㅎ 
{
	int			g_vs_ui_title_only_premium_x = 380;			// 프리미엄 정보 찍히는 위치
};	int			g_vs_ui_title_only_premium_y = 127;			// 프리미엄 정보 찍히는 위치

int C_VS_UI_NEWCHAR::m_hair_color_array[COLOR_LIST_X][COLOR_LIST_Y] = {
	/*
	{0, 15, 30},
	{90, 165, 240},
	{225, 105, 60},
	{285, 210, 120},
	{180, 195, 150}, 
	{270, 75, 255},
	{45, 135, 300}, 
	{315, 330, 345},
	*/
	// 색깔 바꼈어요.. by sigi
	{ 57, 70, 86 },
	{ 101, 115, 130 },
	{ 145, 159, 174 },
	{ 193, 207, 222 },
	{ 237, 252, 267 },
	{ 283, 297, 312 },
	{ 327, 342, 359 },
	{ 369, 381, 400 },
};

int C_VS_UI_NEWCHAR::m_skin_color_array[COLOR_LIST_X][COLOR_LIST_Y] = {
/*	{405, 412, 419}, 
	{420, 428, 434}, 
	{435, 442, 449}, 
	{450, 457, 464}, 
	{465, 472, 479}, 
	{480, 487, 494}, 
	{389, 390, 374}, 
	{359, 375, 403},
	*/	
	{ 494, 487, 480 },
	{ 479, 471, 466 },
	{ 464, 455, 451 },
	{ 449, 440, 435 },
	{ 434, 426, 420 },
	{ 419, 412, 407 },
	{ 371, 364, 381 },
	{ 179, 170, 165 },
};

// Item blink color table
int ga_blink_color_table[INTERFACE_BLINK_VALUE_MAX] = {
	LIGHT_BLUE, LIGHT_BLUE, WHITE, WHITE
};

// ! m_item_search_sequence 순서가 같아야 한다.
/*
S_RECT C_VS_UI_NEWUSER::m_item_rect[ISS_COUNT] = {
	{237, 145, 141, 22}, // ISS_ID
	{237, 180, 141, 22}, // ISS_PASSWORD
	{488, 178, 141, 22}, // ISS_REPASSWORD
	{237, 212, 141, 22}, // ISS_NAME
	{484, 213, 141, 22}, // ISS_STATE
	{484, 245, 141, 22}, // ISS_SSN
	{237, 278, 388, 22}, // ISS_ADDRESS
	{237, 311, 141, 22}, // ISS_WOO
	{484, 311, 141, 22}, // ISS_PHONE
	{258, 342, 367, 22}, // ISS_HOMEPAGE
	{272, 374, 222, 22}, // ISS_EMAIL
};

C_VS_UI_NEWUSER::ITEM_SEARCH_SEQUENCE	C_VS_UI_NEWUSER::m_item_search_sequence[ISS_COUNT] = {
	ISS_ID,
	ISS_PASSWORD,
	ISS_REPASSWORD,
	ISS_NAME,
	ISS_STATE,
	ISS_SSN,
	ISS_ADDRESS,
	ISS_WOO, // 우편번호
	ISS_PHONE,
	ISS_HOMEPAGE,
	ISS_EMAIL,
};
*/
//----------------------------------------------------------------------------
// Operations
//----------------------------------------------------------------------------
/*
//-----------------------------------------------------------------------------
// C_VS_UI_CHAR_APPEARANCE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_CHAR_APPEARANCE::C_VS_UI_CHAR_APPEARANCE()
{
	m_pC_appearance_spk = new C_SPRITE_PACK(SPK_APPEARANCE);

	Set(APPEARANCE_X, APPEARANCE_Y, m_pC_appearance_spk->GetWidth(), m_pC_appearance_spk->GetHeight());

	m_pC_face_spk = new C_SPRITE_PACK(SPK_FACE);

	// appearance
	m_pC_button[APPERANCE_OK] = new C_VS_UI_BUTTON(410, 453, 80, 20, APPERANCE_OK, this);
	m_pC_button[APPERANCE_CANCEL] = new C_VS_UI_BUTTON(470, 453, 110, 20, APPERANCE_CANCEL, this);
	m_pC_button[FACE1] = new C_VS_UI_BUTTON(390, 250, 48, 46, FACE1, this, false, false);
	m_pC_button[FACE2] = new C_VS_UI_BUTTON(458, 250, 48, 46, FACE2, this, false, false);
	m_pC_button[FACE3] = new C_VS_UI_BUTTON(523, 250, 48, 46, FACE3, this, false, false);
	m_pC_button[COLORSET1] = new C_VS_UI_BUTTON(377, 317, 100, 20, COLORSET1, this, false, false);
	m_pC_button[COLORSET2] = new C_VS_UI_BUTTON(477, 317, 115, 20, COLORSET2, this, false, false);

	for (int j = 0; j < COLORSET_Y; j++)
		for (int i = 0; i < COLORSET_X; i++)
			m_b_colorset_array[j][i] = COLORSET_X*j+i;
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_CHAR_APPEARANCE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_CHAR_APPEARANCE::~C_VS_UI_CHAR_APPEARANCE()
{
	DeleteNew(m_pC_face_spk);
	DeleteNew(m_pC_appearance_spk);

	for (int i = 0; i < MENU_COUNT; i++)
		DeleteNew(m_pC_button[i]);
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_APPEARANCE::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_CHAR_APPEARANCE::IsPixel(int _x, int _y)
{
	return m_pC_appearance_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
}
*/
/*-----------------------------------------------------------------------------
- Show
-
  `(m_focused_x, m_focused_y)에 focus된 표시를 한다.
-----------------------------------------------------------------------------*/
/*
void C_VS_UI_CHAR_APPEARANCE::Show()
{
	int i, j;

	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO surface_info;
		SetSurfaceInfo(&surface_info, gpC_base->m_p_DDSurface_back->GetDDSD());

		for (j = 0; j < COLORSET_Y; j++)
			for (i = 0; i < COLORSET_X; i++)
			{
				S_RECT rect;
				SetRect(rect, x+COLORSET_OFFSET_X+COLOR_UNIT_W*i, 
								  y+COLORSET_OFFSET_Y+COLOR_UNIT_H*j, COLOR_UNIT_W, COLOR_UNIT_H);
				filledRect(&surface_info, &rect, CIndexSprite::ColorSet[j*COLORSET_X+i][15]);
			}

		// draw focus
		if (m_focused_x != NOT_SELECTED && m_focused_y != NOT_SELECTED)
		{
			int f_x = x+COLORSET_OFFSET_X+COLOR_UNIT_W*m_focused_x;
			int f_y = y+COLORSET_OFFSET_Y+COLOR_UNIT_W*m_focused_y;
			rectangle(&surface_info, f_x, f_y, f_x+COLOR_UNIT_W-1, f_y+COLOR_UNIT_H-1, WHITE);
		}

		gpC_base->m_p_DDSurface_back->Unlock();
	}

	// color 표 다음에 찍어야...-.-
	m_pC_appearance_spk->Blt(x, y);

	// show face
	if (m_p_slot->bl_female)
	{
		m_pC_face_spk->Blt(388+3, 245+3, W_GUNNER);
		m_pC_face_spk->Blt(455+3, 245+3, W_FIGHTER);
		m_pC_face_spk->Blt(517+3, 245+3, W_PRIEST);
	}
	else
	{
		m_pC_face_spk->Blt(388+3, 245+3, M_GUNNER);
		m_pC_face_spk->Blt(455+3, 245+3, M_FIGHTER);
		m_pC_face_spk->Blt(517+3, 245+3, M_PRIEST);
	}

	// check
	if (m_bl_colorset1)
		gpC_global_resource->m_pC_common_spk->Blt(388, 324, C_GLOBAL_RESOURCE::CHECK_MARK);
	else
		gpC_global_resource->m_pC_common_spk->Blt(489, 324, C_GLOBAL_RESOURCE::CHECK_MARK);

	// draw ani button
	for (i = 0; i < MENU_COUNT; i++)
		m_pC_button[i]->Show();
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_APPEARANCE::Run(id_t id)
{
	switch (id)
	{
		case FACE1:
			if (m_p_slot->bl_female)
				m_p_slot->woman_info.face = W_FACE1;
			else
				m_p_slot->man_info.face = M_FACE1;
			break;

		case FACE2:
			if (m_p_slot->bl_female)
				m_p_slot->woman_info.face = W_FACE2;
			else
				m_p_slot->man_info.face = M_FACE2;
			break;

		case FACE3:
			if (m_p_slot->bl_female)
				m_p_slot->woman_info.face = W_FACE3;
			else
				m_p_slot->man_info.face = M_FACE3;
			break;

		case COLORSET1:
			m_bl_colorset1 = true;
			break;

		case COLORSET2:
			m_bl_colorset1 = false;
			break;

		case APPERANCE_OK:
			Finish();
			break;

		case APPERANCE_CANCEL:
			Finish();
			break;
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_CHAR_APPEARANCE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	int i, j;

	for (i = 0; i < MENU_COUNT; i++)
		m_pC_button[i]->MouseControl(message, _x, _y);

	switch (message)
	{
		case M_MOVING:
			// color set
			if (m_bl_push_colorset)
			{
				for (j = 0; j < COLORSET_Y; j++)
					for (i = 0; i < COLORSET_X; i++)
					{
						S_RECT rect;
						SetRect(rect, x+COLORSET_OFFSET_X+COLOR_UNIT_W*i,
										  y+COLORSET_OFFSET_Y+COLOR_UNIT_H*j,
										  COLOR_UNIT_W, COLOR_UNIT_H);
						
						if (_x >= rect.x && _x < rect.x+rect.w &&
							 _y >= rect.y && _y < rect.y+rect.h)
						{
							m_focused_x = i;
							m_focused_y = j;
						}
					}

				BYTE colorset;
				if (m_focused_x != NOT_SELECTED && m_focused_y != NOT_SELECTED)
				{
					colorset = m_b_colorset_array[m_focused_y][m_focused_x];

					if (m_bl_colorset1)
						m_b_hair_colorset = colorset;
					else
						m_b_skin_colorset = colorset;
				}
			}
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			for (j = 0; j < COLORSET_Y; j++)
				for (i = 0; i < COLORSET_X; i++)
				{
					S_RECT rect;
					SetRect(rect, x+COLORSET_OFFSET_X+COLOR_UNIT_W*i,
									  y+COLORSET_OFFSET_Y+COLOR_UNIT_H*j,
									  COLOR_UNIT_W, COLOR_UNIT_H);
					
					if (_x >= rect.x && _x < rect.x+rect.w &&
						 _y >= rect.y && _y < rect.y+rect.h)
					{
						m_focused_x = i;
						m_focused_y = j;
						m_bl_push_colorset = true;
					}
				}

			BYTE colorset;
			if (m_focused_x != NOT_SELECTED && m_focused_y != NOT_SELECTED)
			{
				colorset = m_b_colorset_array[m_focused_y][m_focused_x];

				if (m_bl_colorset1)
					m_b_hair_colorset = colorset;
				else
					m_b_skin_colorset = colorset;
			}
			break;

		case M_LEFTBUTTON_UP:
			m_bl_push_colorset = false;
			break;
	}

	return true;
}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_APPEARANCE::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN && key == VK_ESCAPE) // cancel!
	{
		Finish();
	}
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_APPEARANCE::Start()
{
	PI_Processor::Start();

	m_bl_push_colorset= false;
	m_p_slot				= NULL;
	m_bl_colorset1		= true;
	m_b_hair_colorset = 0;
	m_b_skin_colorset = 0;
	m_focused_x			= NOT_SELECTED;
	m_focused_y			= NOT_SELECTED;

	for (int i = 0; i < MENU_COUNT; i++)
		m_pC_button[i]->Refresh();

	gpC_window_manager->AttrSetTopmostWindow(this);
}

void C_VS_UI_CHAR_APPEARANCE::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_APPEARANCE::Process()
{

}
*/
//-----------------------------------------------------------------------------
// SendCharacterDeleteToClient
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_DELETE::SendCharacterDeleteToClient()
{
	// static으로 하고 외부에서 string은 delete해준다.

	static DELETE_CHARACTER S_delete_char;

	// 넷마블용
	if(!g_pUserInformation->IsNetmarble)
	{
		g_Convert_DBCS_Ascii2SingleByte(m_lev_ssn_part1.GetString(), m_lev_ssn_part1.Size(), S_delete_char.sz_part1);
		g_Convert_DBCS_Ascii2SingleByte(m_lev_ssn_part2.GetString(), m_lev_ssn_part2.Size(), S_delete_char.sz_part2);
	}
	S_delete_char.slot = m_selected_slot;

	gpC_base->SendMessage(UI_DELETE_CHARACTER, 0, 0, &S_delete_char);
	
}

//-----------------------------------------------------------------------------
// C_VS_UI_CHAR_DELETE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_CHAR_DELETE::C_VS_UI_CHAR_DELETE()
{
	g_RegisterWindow(this);

	AttrTopmost(true);
	AttrKeyboardControl(true);

	// 넷마블용
	int w_h = 207;
	if(g_pUserInformation->IsNetmarble)
		w_h = 127;

	int cancel_offset_x, cancel_offset_y;
	int ok_offset_x, ok_offset_y;
	int ssn_part1_x, ssn_y, ssn_part2_x;

	{
//		m_pC_image_spk = new C_SPRITE_PACK(SPK_CHAR_DELETE);
		ok_offset_x = 189-26;
		ok_offset_y = w_h-60;
		cancel_offset_x = 253-24;
		cancel_offset_y = w_h-60;
		ssn_part1_x = 55;
		ssn_part2_x = 175;
		ssn_y = 108;
	}

	Set(g_GameRect.right/2-324/2, g_GameRect.bottom/2-w_h/2, 324, w_h);

	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+ok_offset_x, y+ok_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_OK), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_OK), DELETE_OK, this, C_GLOBAL_RESOURCE::AB_BUTTON_OK));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+cancel_offset_x, y+cancel_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_OK), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_OK), DELETE_CANCEL, this, C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL));

	if( gC_ci->IsChinese() )
	{
		m_lev_ssn_part1.SetPosition( x+ssn_part1_x+40, y+ssn_y );
		m_lev_ssn_part1.SetByteLimit( 3 );

		Attach( &m_lev_ssn_part1 );
	}
	else
	{
		m_lev_ssn_part1.SetPosition(x+ssn_part1_x, y+ssn_y);
		m_lev_ssn_part1.SetByteLimit(SSN_PART1_CHAR_COUNT);
		m_lev_ssn_part2.SetPosition(x+ssn_part2_x, y+ssn_y);
		m_lev_ssn_part2.SetByteLimit(SSN_PART2_CHAR_COUNT);
		m_lev_ssn_part2.PasswordMode(true);

		Attach(&m_lev_ssn_part1);
		Attach(&m_lev_ssn_part2);
	}
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_CHAR_DELETE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_CHAR_DELETE::~C_VS_UI_CHAR_DELETE()
{
	g_UnregisterWindow(this);

	if( gC_ci->IsChinese() )
		m_lev_ssn_part1.Unacquire();
	else
	{
		m_lev_ssn_part1.Unacquire();
		m_lev_ssn_part2.Unacquire();
	}

//	DeleteNew(m_pC_image_spk);
	DeleteNew(m_pC_button_group);
}

void	C_VS_UI_CHAR_DELETE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char * m_help_string[2] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_DELETE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_CANCEL].GetString()
	};

	g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}

//-----------------------------------------------------------------------------
// C_VS_UI_CHAR_DELETE::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_CHAR_DELETE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{	
	if (p_button->GetFocusState() && p_button->GetPressState())
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x, p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
	else
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x, p_button->y, p_button->m_image_index);
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_DELETE::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_CHAR_DELETE::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);//m_pC_image_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_DELETE::Start()
{
	PI_Processor::Start();

	if( gC_ci->IsChinese() )
	{
		m_lev_ssn_part1.EraseAll();
		m_lev_ssn_part1.Acquire();
	}
	else
	{
		m_lev_ssn_part1.EraseAll();
		m_lev_ssn_part2.EraseAll();
		m_lev_ssn_part1.Acquire();
	}

	m_bl_ssn_ip_part1 = true;
	m_selected_slot = 0;

	gpC_window_manager->AppearWindow(this);

	m_pC_button_group->Init();
}

void C_VS_UI_CHAR_DELETE::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_DELETE::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_DELETE::Show()
{
//	m_pC_image_spk->Blt(x, y, DELETE_WINDOW);
	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);
	
	g_FL2_GetDC();
	g_PrintColorStr(x+w/2-g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_DELETE_CONFIRM].GetString(), gpC_base->m_char_name_pi.hfont)/2, 
		y+30, (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_DELETE_CONFIRM].GetString(), gpC_base->m_char_name_pi, RGB_WHITE);
	// 넷마블용
	if(!g_pUserInformation->IsNetmarble)
	{
		g_PrintColorStr(x+w/2-g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_RE_INPUT_CORRECT_SSN].GetString(), gpC_base->m_char_name_pi.hfont)/2, 
			y+50,(*g_pGameStringTable)[UI_STRING_MESSAGE_RE_INPUT_CORRECT_SSN].GetString(), gpC_base->m_char_name_pi, RGB_WHITE);
	}
	m_pC_button_group->ShowDescription();
	g_FL2_ReleaseDC();

	const int chineseSSNBoxSizeX = 165;

	RECT chineseRect = {m_lev_ssn_part1.GetPosition().x-10, m_lev_ssn_part1.GetPosition().y-4,
			m_lev_ssn_part1.GetPosition().x-10+chineseSSNBoxSizeX,m_lev_ssn_part1.GetPosition().y-4+25};	

	if( gC_ci->IsChinese() )
	{
		gpC_base->m_p_DDSurface_back->FillRect(&chineseRect,0);
	}

	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		// 넷마블용
		if(!g_pUserInformation->IsNetmarble)
		{
			if( gC_ci->IsChinese() )
			{
//				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked( m_lev_ssn_chinese.GetPosition().x-10, m_lev_ssn_chinese.GetPosition().y-2, C_GLOBAL_RESOURCE::AB_MONEY_BAR );
				Rect rect(chineseRect.left,chineseRect.top,chineseSSNBoxSizeX,25);
				gpC_global_resource->DrawOutBoxLocked(rect);
			}
			else if( gC_ci->IsKorean() )
			{
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(m_lev_ssn_part1.GetPosition().x-10, m_lev_ssn_part1.GetPosition().y-2, C_GLOBAL_RESOURCE::AB_NAME_BAR);
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(m_lev_ssn_part2.GetPosition().x-10, m_lev_ssn_part2.GetPosition().y-2, C_GLOBAL_RESOURCE::AB_NAME_BAR);
			}
		}

		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	// 넷마블용
	if(!g_pUserInformation->IsNetmarble)
		Window::ShowWidget();

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_DELETE::Run(id_t id)
{
	switch (id)
	{
		case DELETE_OK:
			// 넷마블용
			{
				// 제대로 입력하였는가?
				if (
					 ( ( gC_ci->IsKorean()&& (	m_lev_ssn_part1.Size() == SSN_PART1_CHAR_COUNT &&
					 m_lev_ssn_part2.Size() == SSN_PART2_CHAR_COUNT ) ) ||
					 ( !gC_ci->IsKorean() && ( wcscmp( m_lev_ssn_part1.GetString(), _L("DeletePc")) == 0 ) )
					 )
					 || g_pUserInformation->IsNetmarble)
				{
					SendCharacterDeleteToClient();
				}
				else
				{
					// error message!
					g_msg_wrong_ssn->Start();
				}
			}
			break;

		case DELETE_CANCEL:
			Finish();
			break;
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_CHAR_DELETE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	m_pC_button_group->MouseControl(message, _x, _y);

	return true;
}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_DELETE::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN)
		switch (key)
		{
			case VK_ESCAPE:
				Run(DELETE_CANCEL);
				return;

			case VK_RETURN:
				Run(DELETE_OK);
				return;
		}

	// digit only
	if (message == WM_CHAR && (!gC_ci->IsKorean() || gC_ci->IsKorean() && (key >= '0' && key <= '9')))
	{
		Window::KeyboardControl(message, key, extra);

		if( gC_ci->IsKorean() )
		{
			if (m_bl_ssn_ip_part1)
			{
				if (m_lev_ssn_part1.Size() == SSN_PART1_CHAR_COUNT)
				{
					m_bl_ssn_ip_part1 = false;
					m_lev_ssn_part2.Acquire();
				}
			}
		}
	}

	if (message == WM_KEYDOWN)
		if (key == VK_BACK)
		{
			if( gC_ci->IsKorean() )
			{
				if (!m_bl_ssn_ip_part1)
				{
					if (m_lev_ssn_part2.Size() == 0)
					{
						m_bl_ssn_ip_part1 = true;
						m_lev_ssn_part1.Acquire();
					}
				}
			}

			Window::KeyboardControl(message, key, extra);
		}
}

//-----------------------------------------------------------------------------
// RollDice
//
// 주사위를 던져서 point를 정한다.
//-----------------------------------------------------------------------------
void C_VS_UI_NEWCHAR::RollDice(bool load)
{
	switch(m_p_slot->Race)
	{
	case RACE_SLAYER:
		if(load == false)
		{
			const int min = 5;
			const int max = 30;
			
			//
			// STR+DEX+INT = 30
			//
			m_p_slot->STR_PURE = min+(rand()%16); // 16 = 15(max:30 - min) + 1
			
			int r = max - m_p_slot->STR_PURE - min;
			m_p_slot->DEX_PURE = min+(rand()%(r-min+1));
			
			assert(r >= m_p_slot->DEX_PURE);
			
			m_p_slot->INT_PURE = max - (m_p_slot->STR_PURE+m_p_slot->DEX_PURE);
			
			assert(m_p_slot->STR_PURE+m_p_slot->DEX_PURE+m_p_slot->INT_PURE == max);
			
			int s[3] = {m_p_slot->STR_PURE, m_p_slot->DEX_PURE, m_p_slot->INT_PURE};
			
			for(int i = 0; i < rand()%100; i++)
			{
				int a, b, c;
				a = rand()%3;
				b = rand()%3;
				
				c = s[a]; s[a] = s[b]; s[b] = c;//스왑스왑-_-;
			}
			
			m_p_slot->STR_PURE = s[0];
			m_p_slot->DEX_PURE = s[1];
			m_p_slot->INT_PURE = s[2];
			
			if(m_iSave[0] == -1)
			{
				m_iSave[0] = s[0];
				m_iSave[1] = s[1];
				m_iSave[2] = s[2];
			}
		}
		else //load
		{
			m_p_slot->STR_PURE = m_iSave[0];
			m_p_slot->DEX_PURE = m_iSave[1];
			m_p_slot->INT_PURE = m_iSave[2];
		}
		break;

	case RACE_VAMPIRE:
		{
			m_p_slot->STR_PURE = 20;
			m_p_slot->DEX_PURE = 20;
			m_p_slot->INT_PURE = 20;
		}
		break;
		
	case RACE_OUSTERS:
		{
			m_p_slot->STR_PURE = 10;
			m_p_slot->DEX_PURE = 10;
			m_p_slot->INT_PURE = 10;
			m_p_slot->bonus_point = 15;
		}
		break;
	}

	m_p_slot->DAM			= 1;
	m_p_slot->DAM2			= max(1, m_p_slot->STR_PURE/10);
	m_p_slot->DEFENSE		= m_p_slot->DEX_PURE;
	m_p_slot->PROTECTION	= m_p_slot->STR_PURE/15;
	m_p_slot->TOHIT			= m_p_slot->DEX_PURE;

	m_p_slot->HP = m_p_slot->STR_PURE*2;
	m_p_slot->MP = m_p_slot->INT_PURE*2;

	m_p_slot->HP_MAX = 20*2; // 이걸 ... const int로 하니까 SetEnergy()에서 이 값이 변했다!
	m_p_slot->MP_MAX = 20*2;

//	SetEnergy(m_p_slot->HP, m_p_slot->HP_MAX, m_p_slot->hp_percent, m_p_slot->hp_cur_line);
//	SetEnergy(m_p_slot->MP, m_p_slot->MP_MAX, m_p_slot->mp_percent, m_p_slot->mp_cur_line);
}

//-----------------------------------------------------------------------------
// SendNewCharacterToClient
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_NEWCHAR::SendNewCharacterToClient()
{
	//
	// !외부에 sz_name을 delete하면 안된다.
	//
	static NEW_CHARACTER S_new_character; // 반드시 static으로..

//	DeleteNew(m_p_slot->sz_name);

	char *sz_temp;
	g_Convert_DBCS_Ascii2SingleByte(m_lev_name.GetString(), m_lev_name.Size(), sz_temp);

	m_p_slot->sz_name = sz_temp;
	DeleteNew(sz_temp);

	S_new_character.sz_name		= (char *)m_p_slot->sz_name.c_str();
	S_new_character.race		= m_p_slot->Race;
	S_new_character.bl_female	= m_p_slot->bl_female;
	S_new_character.STR			= m_p_slot->STR_PURE;
	S_new_character.DEX			= m_p_slot->DEX_PURE;
	S_new_character.INT			= m_p_slot->INT_PURE;
	S_new_character.slot		= m_selected_slot;
	S_new_character.skin_color = m_p_slot->skin_color;
	S_new_character.hair_color = m_p_slot->hair_color;
	S_new_character.coat_color = m_p_slot->coat_color;;
	S_new_character.trouser_color = m_p_slot->trouser_color;
	if (m_p_slot->bl_female)
		S_new_character.face	= m_p_slot->woman_info.hair-W_HAIR1;
	else
		S_new_character.face	= m_p_slot->man_info.hair-M_HAIR1;
	S_new_character.race		= m_p_slot->Race;
	

	gpC_base->SendMessage(UI_NEW_CHARACTER, 0, 0, &S_new_character);
}

//-----------------------------------------------------------------------------
// _Timer_CharUpdate
//
// 
//-----------------------------------------------------------------------------
void _Timer_CharUpdate() // globals
{
		g_char_index++;
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_NEWCHAR::Start()
{
	PI_Processor::Start();

	//m_pC_char_appearance->Finish();

	m_bl_pushed_table = false;
//	m_bl_skin_color_control = false;
//	m_bl_change_face	= false;
//	m_pC_board_spk		= NULL;
	m_p_slot				= NULL;
	m_selected_slot	= 0;

	// init face list
//	m_woman_face_list[0] = m_man_face_list[0] = 0;
//	m_woman_face_list[1] = m_man_face_list[1] = 1;
//	m_woman_face_list[2] = m_man_face_list[2] = 2;

	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
//	m_pC_button_group_face->Init();

	LoadDesc("create_window.txt", 30, 14, false);

	m_lev_name.Acquire();
	m_lev_name.EraseAll();

}

void C_VS_UI_NEWCHAR::Finish()
{
	//m_pC_char_appearance->Finish();

	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);

//	m_pC_ani_fin->Stop();
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_NEWCHAR::Process()
{
	m_pC_button_group->Process();
//	m_pC_button_group_face->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_NEWCHAR
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_NEWCHAR::C_VS_UI_NEWCHAR()
{
	g_RegisterWindow(this);

	AttrKeyboardControl(true);

	//m_pC_char_appearance = new C_VS_UI_CHAR_APPEARANCE;

//	m_pC_back.Open(SPK_NEWCHARACTER_BACK);
//	m_pC_etc.Open(SPK_NEWCHARACTER_ETC);
	// add by Sonic 2006.9.26
	if(g_MyFull)
	{
		m_common_spk.Open(SPK_COMMON_1024);
		m_image_spk.Open(SPK_CHAR_CREATE);
		m_face_spk.Open(SPK_FACE_MAKE);
		Set(0, 0, 1024, 768);
	}
	else
	{
		m_common_spk.Open(SPK_COMMON);
		m_image_spk.Open(SPK_CHAR_CREATE);
		m_face_spk.Open(SPK_FACE_MAKE);
		Set(0, 0, 800, 600);
	}
	// end by sonic


//	Set(0, 0, m_pC_back.GetWidth(), m_pC_back.GetHeight());
	

	// set button
	m_pC_button_group = new ButtonGroup(this);

	const InterfaceInformation *pSkin = &g_pSkinManager->Get( SkinManager::NEW_CHAR );

	//back, next
	int skinnum=0;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,	m_common_spk.GetWidth(BACK_BUTTON), m_common_spk.GetHeight(BACK_BUTTON), BACK_ID, this, BACK_BUTTON)); skinnum++;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,	pSkin->GetPoint( skinnum ).y,	m_common_spk.GetWidth(NEXT_BUTTON), m_common_spk.GetHeight(NEXT_BUTTON), NEXT_ID, this, NEXT_BUTTON));skinnum++;

	//face back, next
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(FACE_BACK_BUTTON), m_image_spk.GetHeight(FACE_BACK_BUTTON), FACE_BACK_ID, this, FACE_BACK_BUTTON));skinnum++;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(FACE_NEXT_BUTTON), m_image_spk.GetHeight(FACE_NEXT_BUTTON), FACE_NEXT_ID, this, FACE_NEXT_BUTTON));skinnum++;

	//slayer, vampire
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(SLAYER_BUTTON)+20, m_image_spk.GetHeight(SLAYER_BUTTON), SLAYER_ID, this, SLAYER_BUTTON));skinnum++;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(VAMPIRE_BUTTON)+20, m_image_spk.GetHeight(VAMPIRE_BUTTON), VAMPIRE_ID, this, VAMPIRE_BUTTON));skinnum++;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(OUSTERS_BUTTON)+20, m_image_spk.GetHeight(OUSTERS_BUTTON), OUSTERS_ID, this, OUSTERS_BUTTON));skinnum++;

	//male, female
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(MALE_BUTTON)+20, m_image_spk.GetHeight(MALE_BUTTON), MALE_ID, this, MALE_BUTTON));skinnum++;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(FEMALE_BUTTON)+20, m_image_spk.GetHeight(FEMALE_BUTTON), FEMALE_ID, this, FEMALE_BUTTON));skinnum++;

	//save,load,reroll
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(SAVE_BUTTON), m_image_spk.GetHeight(SAVE_BUTTON), SAVE_ID, this, SAVE_BUTTON));skinnum++;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(LOAD_BUTTON), m_image_spk.GetHeight(LOAD_BUTTON), LOAD_ID, this, LOAD_BUTTON));skinnum++;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(REROLL_BUTTON), m_image_spk.GetHeight(REROLL_BUTTON), REROLL_ID, this, REROLL_BUTTON));skinnum++;

	//check button
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(CHECK_BUTTON), m_image_spk.GetHeight(CHECK_BUTTON), CHECK_ID, this, CHECK_BUTTON));skinnum++;

	//아우스터즈 +, - 버튼
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(PLUS_BUTTON), m_image_spk.GetHeight(PLUS_BUTTON), STR_PLUS_ID, this, PLUS_BUTTON));skinnum++;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(MINUS_BUTTON), m_image_spk.GetHeight(MINUS_BUTTON), STR_MINUS_ID, this, MINUS_BUTTON));skinnum++;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(PLUS_BUTTON), m_image_spk.GetHeight(PLUS_BUTTON), DEX_PLUS_ID, this, PLUS_BUTTON));skinnum++;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(MINUS_BUTTON), m_image_spk.GetHeight(MINUS_BUTTON), DEX_MINUS_ID, this, MINUS_BUTTON));skinnum++;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(PLUS_BUTTON), m_image_spk.GetHeight(PLUS_BUTTON), INT_PLUS_ID, this, PLUS_BUTTON));skinnum++;
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint( skinnum ).x,pSkin->GetPoint( skinnum ).y,m_image_spk.GetWidth(MINUS_BUTTON), m_image_spk.GetHeight(MINUS_BUTTON), INT_MINUS_ID, this, MINUS_BUTTON));skinnum++;

	// LineEditorVisual setting...
	m_lev_name.SetPrintInfo(gpC_base->m_info_pi);
	m_lev_name.SetInputStringColor(RGB_WHITE);
	m_lev_name.SetPosition(x+NAME_BOARD_X, y+NAME_BOARD_Y);
	m_lev_name.SetByteLimit(10);
	Attach(&m_lev_name);

	// character ISPK & CFPK
//	int num;

//	m_slayer_ispk_file.open(ISPK_SLAYER, ios::binary|ios::nocreate);
//	if (!m_slayer_ispk_file)
//		_Error(FILE_OPEN);
//
//	m_slayer_ispk_file.read((char*)&num, SIZE_SPRITEID);
//	
//	m_pC_slayer_ispk.Init(num, CDirectDraw::Is565());
//
//	{
//		ifstream indexFile( ISPKI_SLAYER, ios::binary);
//		m_SlayerPackIndex.LoadFromFile( indexFile );
//		indexFile.close();
//	}
	m_slayer_ispk.LoadFromFileRunning(ISPK_SLAYER);

//	m_vampire_ispk_file.open(ISPK_VAMPIRE, ios::binary|ios::nocreate);
//	if (!m_vampire_ispk_file)
//		_Error(FILE_OPEN);
//
//	m_vampire_ispk_file.read((char*)&num, SIZE_SPRITEID);
//	
//	m_vampire_ispk.Init(num, CDirectDraw::Is565());
//
//	{
//		ifstream indexFile( ISPKI_VAMPIRE, ios::binary);
//		m_VampirePackIndex.LoadFromFile( indexFile );
//		indexFile.close();
//	}
	m_vampire_ispk.LoadFromFileRunning(ISPK_VAMPIRE);
	m_ousters_ispk.LoadFromFileRunning(ISPK_OUSTERS);

	//m_AdvancementSlayerIspk.LoadFromFileRunning( ISPK_ADVANCEMENT_SLAYER );
	m_AdvancementVampireManIspk.LoadFromFileRunning( ISPK_ADVANCEMENT_VAMPIRE_MAN );
	m_AdvancementVampireWomanIspk.LoadFromFileRunning( ISPK_ADVANCEMENT_VAMPIRE_WOMAN );
	m_AdvancementOustersIspk.LoadFromFileRunning( ISPK_ADVANCEMENT_OUSTERS );
	m_AdvancementSlayerManIspk.LoadFromFileRunning( ISPK_ADVANCEMENT_SLAYER_MAN );
	m_AdvancementSlayerWomanIspk.LoadFromFileRunning( ISPK_ADVANCEMENT_SLAYER_WOMAN );

	ifstream file_man(CFPK_SLAYER, ios::binary|ios::nocreate);
	if (!file_man)
		_Error(FILE_OPEN);
	m_slayer_cfpk.LoadFromFile(file_man);
	ifstream file_vampire(CFPK_VAMPIRE, ios::binary|ios::nocreate);
	file_man.close();

	if (!file_vampire)
		_Error(FILE_OPEN);
	m_vampire_cfpk.LoadFromFile(file_vampire);
	file_vampire.close();

	ifstream file_ousters(CFPK_OUSTERS, ios::binary|ios::nocreate);
	if (!file_ousters)
		_Error(FILE_OPEN);
	m_ousters_cfpk.LoadFromFile(file_ousters);
	file_ousters.close();
	
	ifstream file_ac_ousters( CFPK_ADVANCEMENT_OUSTERS, ios::binary|ios::nocreate );
	if( !file_ac_ousters )
		_Error( FILE_OPEN );
	m_AdvancementOustersCfpk.LoadFromFile( file_ac_ousters );
	file_ac_ousters.close();

	ifstream file_ac_vampire( CFPK_ADVANCEMENT_VAMPIRE_MAN, ios::binary|ios::nocreate );
	if( !file_ac_vampire )
		_Error( FILE_OPEN );
	m_AdvancementVampireManCfpk.LoadFromFile( file_ac_vampire );
	file_ac_vampire.close();

	ifstream file_ac_vampire2( CFPK_ADVANCEMENT_VAMPIRE_WOMAN, ios::binary|ios::nocreate );
	if( !file_ac_vampire2 )
		_Error( FILE_OPEN );
	m_AdvancementVampireWomanCfpk.LoadFromFile( file_ac_vampire2 );
	file_ac_vampire2.close();


	ifstream file_ac_slayerman( CFPK_ADVANCEMENT_SLAYER_MAN,ios::binary|ios::nocreate );
	if( !file_ac_slayerman )
		_Error( FILE_OPEN );
	m_AdvancementSlayerManCfpk.LoadFromFile( file_ac_slayerman );
	file_ac_slayerman.close();

	ifstream file_ac_slayerwoman( CFPK_ADVANCEMENT_SLAYER_WOMAN,ios::binary|ios::nocreate );
	if( !file_ac_slayerwoman )
		_Error( FILE_OPEN );
	m_AdvancementSlayerWomanCfpk.LoadFromFile( file_ac_slayerwoman );
	file_ac_slayerwoman.close();

	/*
	ifstream file_ac_slayer( CFPK_ADVANCEMENT_SLAYER, ios::binary | ios::nocreate );
	if( !file_ac_slayer )
		_Error( FILE_OPEN );
	m_AdvancementSlayerCfpk.LoadFromFile( file_ac_slayer );
	file_ac_slayer.close();
	
	ifstream file_ac_vampire( CFPK_ADVANCEMENT_VAMPIRE, ios::binary | ios::nocreate );
	if( !file_ac_vampire )
		_Error( FILE_OPEN );
	m_AdvancementVampireCfpk.LoadFromFile( file_ac_vampire );
	file_ac_vampire.close();
	*/

	// for HP/MP bar...
//	m_hp_width	= m_pC_etc.GetWidth(HP);
//	m_hp_height = m_pC_etc.GetHeight(MP);
//	m_mp_width	= m_pC_etc.GetWidth(MP);
//	m_mp_height = m_pC_etc.GetHeight(MP);
//
//	assert(m_hp_width > 0 && m_hp_height > 0);
//	assert(m_mp_width > 0 && m_mp_height > 0);
//
//	m_p_hp_buf = new WORD[m_hp_width*m_hp_height];
//	m_p_mp_buf = new WORD[m_mp_width*m_mp_height];

	// energy cylinder를 plain buffer에 blt한다.
//	m_pC_etc.BltOffscreen(0, 0, HP);

	// blt from offscreen
//	if (gpC_base->m_DDSurface_offscreen.Lock())
//	{
//		S_SURFACEINFO	src_surface, dest_surface;
//		S_RECT			src_rect, dest_rect;
//		SetSurfaceInfo(&src_surface, gpC_base->m_DDSurface_offscreen.GetDDSD());
//		SetSurfaceInfo(dest_surface, m_p_hp_buf, m_hp_width, m_hp_height);
//		SetRect(src_rect, 0, 0, m_hp_width, m_hp_height);
//		SetRect(dest_rect, 0, 0, m_hp_width, m_hp_height);
//
//		Bltz(&dest_surface, &src_surface, &dest_rect, &src_rect);
//
//		gpC_base->m_DDSurface_offscreen.Unlock();
//	}
//
//	// energy cylinder를 plain buffer에 blt한다.
//	m_pC_etc.BltOffscreen(0, 0, MP);
//
//	// blt from offscreen
//	if (gpC_base->m_DDSurface_offscreen.Lock())
//	{
//		S_SURFACEINFO	src_surface, dest_surface;
//		S_RECT			src_rect, dest_rect;
//		SetSurfaceInfo(&src_surface, gpC_base->m_DDSurface_offscreen.GetDDSD());
//		SetSurfaceInfo(dest_surface, m_p_mp_buf, m_mp_width, m_mp_height);
//		SetRect(src_rect, 0, 0, m_mp_width, m_mp_height);
//		SetRect(dest_rect, 0, 0, m_mp_width, m_mp_height);
//
//		Bltz(&dest_surface, &src_surface, &dest_rect, &src_rect);
//
//		gpC_base->m_DDSurface_offscreen.Unlock();
//	}

	srand((unsigned)time(NULL));

	m_iSave[0] = -1;
	m_focused_help = HELP_DEFAULT;

 	// fin ani
//	m_pC_ao_fin = new C_ANI_OBJECT(SPK_FIN, FRR_FIN);
//	m_pC_ani_fin = new C_ANIMATION(m_pC_ao_fin);
//	m_pC_ani_fin->SetPlayPosition(323, 368);

	m_pack_file.SetRAR(RPK_TITLE, RPK_PASSWORD);
//	LoadDesc("create.txt", 30, 14, false);
	SetDesc(29, 160, RGB(160, 160, 160), gpC_base->m_chatting_pi);
	m_desc_y_distance = 16;

}

//void	C_VS_UI_NEWCHAR::AssertSlayerSprite(int m_sprite_id)
//{
//	if(m_sprite_id >= m_pC_slayer_ispk.GetSize())return;
//
//	CIndexSprite *pSprite = &m_pC_slayer_ispk[ m_sprite_id];
//	if (pSprite->IsNotInit())
//	{
//		m_slayer_ispk_file.seekg( m_SlayerPackIndex[m_sprite_id], ios::beg );
//		pSprite->LoadFromFile( m_slayer_ispk_file );
//	}
//}
//
//void	C_VS_UI_NEWCHAR::AssertVampireSprite(int m_sprite_id)
//{
//	if(m_sprite_id >= m_pC_vampire_ispk.GetSize())return;
//
//	CIndexSprite *pSprite = &m_pC_vampire_ispk[ m_sprite_id];
//	if (pSprite->IsNotInit())
//	{
//		m_vampire_ispk_file.seekg( m_VampirePackIndex[m_sprite_id], ios::beg );
//		pSprite->LoadFromFile( m_vampire_ispk_file );
//	}
//}
//-----------------------------------------------------------------------------
// ~C_VS_UI_NEWCHAR
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_NEWCHAR::~C_VS_UI_NEWCHAR()
{
	g_UnregisterWindow(this);

//	m_slayer_ispk_file.close();
//	m_vampire_ispk_file.close();

//	DeleteNew(m_pC_ao_fin);
//	DeleteNew(m_pC_ani_fin);

	DeleteNew(m_pC_button_group);
//	DeleteNew(m_pC_button_group_face);

//	DeleteNewArray(m_p_hp_buf);
//	DeleteNewArray(m_p_mp_buf);

	//DeleteNew(m_pC_char_appearance);
}

void C_VS_UI_NEWCHAR::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// ChangeColor
//
// (x, y) 위치의 color table color로 color를 change한다.
// 바꿨으면 true를 반환한다.
//-----------------------------------------------------------------------------
bool C_VS_UI_NEWCHAR::ChangeColor(int _x, int _y)
{
	for (int j=0; j < COLOR_LIST_Y; j++)
		for (int i=0; i < COLOR_LIST_X; i++)
		{
			if (_x >= TABLE_X+i*COLOR_UNIT_X && _x < TABLE_X+i*COLOR_UNIT_X+COLOR_UNIT_X &&
				 _y >= TABLE_Y2+j*COLOR_UNIT_Y && _y < TABLE_Y2+j*COLOR_UNIT_Y+COLOR_UNIT_Y)
			{
				m_skin_point.Set(i, j);
				m_p_slot->skin_color = m_skin_color_array[i][j];
				return true;
			}
			if (_x >= TABLE_X+i*COLOR_UNIT_X && _x < TABLE_X+i*COLOR_UNIT_X+COLOR_UNIT_X &&
				 _y >= TABLE_Y+j*COLOR_UNIT_Y && _y < TABLE_Y+j*COLOR_UNIT_Y+COLOR_UNIT_Y)
			{
				m_hair_point.Set(i, j);
				m_p_slot->hair_color = m_hair_color_array[i][j];
				return true;
			}
		}

	return false;
}

//-----------------------------------------------------------------------------
// GetPoint
//
// color array에서 colorset의 point를 반환한다.
//-----------------------------------------------------------------------------
Point C_VS_UI_NEWCHAR::GetPoint(int colorset, bool bl_skin_color)
{
	int i, j;
	Point point;

	if (bl_skin_color)
	{
		for (j=0; j < COLOR_LIST_Y; j++)
			for (i=0; i < COLOR_LIST_X; i++)
				if (colorset == m_skin_color_array[i][j])
				{
					point.Set(i, j);
					return point;
				}
	}
	else
	{
		for (j=0; j < COLOR_LIST_Y; j++)
			for (i=0; i < COLOR_LIST_X; i++)
				if (colorset == m_hair_color_array[i][j])
				{
					point.Set(i, j);
					return point;
				}
	}

	assert(0);

	return point;
}

//-----------------------------------------------------------------------------
// GetColor
//
// color array에서 (x, y)의 color set으로 color를 구해서 반환한다.
//-----------------------------------------------------------------------------
int C_VS_UI_NEWCHAR::GetColor(int _x, int _y, bool bl_skin_color)
{
	int colorset;
	if (bl_skin_color)
		colorset = m_skin_color_array[_x][_y];
	else
		colorset = m_hair_color_array[_x][_y];

	const int center_value = 10;

	return CIndexSprite::ColorSet[colorset][center_value];
}

//-----------------------------------------------------------------------------
// ResetFaceList
//
// selected_face를 맨 앞으로 한다.
//-----------------------------------------------------------------------------
//void C_VS_UI_NEWCHAR::ResetFaceList(int * p_face_list, int selected_face)
//{
//	assert(selected_face >= 0 && selected_face < 3);
//
//	int temp[3];
//	temp[0] = p_face_list[0];
//	temp[1] = p_face_list[1];
//	temp[2] = p_face_list[2];
//
//	p_face_list[0] = selected_face;
//	for (int i=0, k=1; i < 3; i++)
//	{
//		if (selected_face != temp[i])
//			p_face_list[k++] = temp[i];
//	}
//}


void C_VS_UI_NEWCHAR::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{

}

//-----------------------------------------------------------------------------
// ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_NEWCHAR::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == MALE_ID || p_button->GetID() == FEMALE_ID 
		|| p_button->GetID() == SLAYER_ID || p_button->GetID() == VAMPIRE_ID || p_button->GetID() == OUSTERS_ID )
	{
		if(p_button->GetFocusState())
			m_image_spk.BltLocked(x+p_button->x+20, y+p_button->y, p_button->m_image_index);
	}
	else
	if(p_button->GetID() == BACK_ID || p_button->GetID() == NEXT_ID)
	{
		if(p_button->GetFocusState())
		{
			if (p_button->GetPressState()) // push state
				m_common_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				m_common_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
			if(p_button->GetID() == BACK_ID)
				m_focused_help = HELP_BACK;
			else
				m_focused_help = HELP_NEXT;
		}
		else
			m_common_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
	}
	else
	{
		if( 
			(p_button->GetID() >= STR_PLUS_ID && p_button->GetID() <= INT_MINUS_ID && m_p_slot->Race !=  RACE_OUSTERS ) ||
			(p_button->GetID() == REROLL_ID && m_p_slot->Race != RACE_SLAYER )
		  )
			return;

		if(p_button->GetFocusState())
		{
			if( m_p_slot->Race == RACE_SLAYER || !(p_button->GetID() == LOAD_ID || p_button->GetID() == SAVE_ID || p_button->GetID() == REROLL_ID || p_button->GetID() == FACE_BACK_ID || p_button->GetID() == FACE_NEXT_ID))
			{
				if (p_button->GetPressState()) // push state
					m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
				else
					m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
				
				switch(p_button->GetID())
				{
				case CHECK_ID:
					m_focused_help = HELP_CHECK;
					break;
					
				case LOAD_ID:
					m_focused_help = HELP_LOAD;
					break;
					
				case SAVE_ID:
					m_focused_help = HELP_SAVE;
					break;
					
				case REROLL_ID:
					m_focused_help = HELP_REROLL;
					break;
				}
			}
		}
		else
		{
			if( m_p_slot->Race == RACE_SLAYER && (p_button->GetID() == LOAD_ID || p_button->GetID() == SAVE_ID || p_button->GetID() == REROLL_ID) )
			{
				m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
				return;
			}

			if( 
				(
				m_p_slot->Race == RACE_SLAYER || 
				!(p_button->GetID() == LOAD_ID || 
				p_button->GetID() == SAVE_ID || 
				p_button->GetID() == REROLL_ID || 
				p_button->GetID() == FACE_BACK_ID || 
				p_button->GetID() == FACE_NEXT_ID)
				)

				&& (m_p_slot->Race == RACE_OUSTERS && p_button->GetID() >= STR_PLUS_ID && p_button->GetID() <= INT_MINUS_ID))
				m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}
	}
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_NEWCHAR::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_NEWCHAR::IsPixel(int _x, int _y)
{
	return true;
//	return m_pC_back.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), MAIN);
}

//-----------------------------------------------------------------------------
// SetCharacterToThisSlot
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_NEWCHAR::SetCharacterToThisSlot(int slot, S_SLOT * p_slot)
{ 
	assert(p_slot->bl_set == false);
	
	//assert(p_slot->sz_name == NULL); // 2000.12.15. ; 하지 말것. 중복된 ID일 경우에는 NULL이 아니다.

	m_p_slot = p_slot; 
	m_selected_slot = slot;

	// default
	m_p_slot->bl_female = false;
	m_p_slot->Race = RACE_SLAYER;
	srand(GetTickCount());

	//m_p_slot->man_info.coat = M_UP_BODY;
	m_p_slot->man_info.coat = M_OLD_COAT;
	switch(rand()%3)
	{
	case 0 :
		m_p_slot->man_info.face = M_FACE1;
		m_p_slot->man_info.hair = M_HAIR1;
		break;
	case 1:
		m_p_slot->man_info.face = M_FACE2;
		m_p_slot->man_info.hair = M_HAIR2;
		break;
	default :
		m_p_slot->man_info.face = M_FACE3;
		m_p_slot->man_info.hair = M_HAIR3;
		break;
	}
	
	m_p_slot->man_info.helmet = M_NO_WEAR;
	//m_p_slot->man_info.trouser = M_DOWN_BODY;
	m_p_slot->man_info.trouser = M_OLD_TROUSER;
	m_p_slot->man_info.right = M_NO_WEAR;//M_BLADE;
	m_p_slot->man_info.left = M_NO_WEAR;//M_DRAGON_SHIELD;

	//m_p_slot->woman_info.coat = W_UP_BODY;
	m_p_slot->woman_info.coat = W_OLD_COAT;
	switch(rand()%3)
	{
	case 0 :
		m_p_slot->woman_info.face = W_FACE1;
		m_p_slot->woman_info.hair = W_HAIR1;
		break;
	case 1:
		m_p_slot->woman_info.face = W_FACE2;
		m_p_slot->woman_info.hair = W_HAIR2;
		break;
	default :
		m_p_slot->woman_info.face = W_FACE3;
		m_p_slot->woman_info.hair = W_HAIR3;
		break;
	}	
	m_p_slot->woman_info.helmet = W_NO_WEAR;
	//m_p_slot->woman_info.trouser = W_DOWN_BODY;
	m_p_slot->woman_info.trouser = W_OLD_TROUSER;
	m_p_slot->woman_info.right = W_NO_WEAR;//M_BLADE;
	m_p_slot->woman_info.left = W_NO_WEAR;//M_DRAGON_SHIELD;

	// default color도 바꼈어요.. by sigi
	

	// 생성시 랜덤으로 선택되도록 수정.		2002.11  by sonee
//	m_p_slot->hair_color = m_hair_color_array[rand()%COLOR_LIST_X][rand()%COLOR_LIST_Y];
//	m_p_slot->skin_color = m_skin_color_array[rand()%COLOR_LIST_X][rand()%COLOR_LIST_Y];

//	m_p_slot->hair_color = 57;
//	m_p_slot->skin_color = 494;
	
	m_p_slot->left_color = 377;
	m_p_slot->right_color = 377;
	m_p_slot->helmet_color = 377;
	m_p_slot->trouser_color = 377;
	m_p_slot->coat_color = 377;
	if(m_p_slot->Race == RACE_OUSTERS)m_p_slot->skin_color = 377;

//	bool skin_color_control = m_bl_skin_color_control; // backup

//	m_bl_skin_color_control = false;
// 	m_hair_point = GetPoint(m_p_slot->hair_color, false);

//	m_bl_skin_color_control = true;
// 	m_skin_point = GetPoint(m_p_slot->skin_color, true);

	Run(SLAYER_ID+rand()%3);

//	m_bl_skin_color_control = skin_color_control;
}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_NEWCHAR::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN && key == VK_ESCAPE) // cancel!
	{
//		if (m_bl_change_face == true)
//		{
//			m_bl_change_face = false;
//			return;
//		}

//		Finish();
		Run(BACK_ID);
	}

	//
	// character Name
	//
	// 특수문자, 공백, 첫문자 숫자를 막는다.
	//
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

/*
#ifndef _LIB
	if (message == WM_KEYDOWN)
	{
		static bool bl_old;
		static int	position;

		switch (key)
		{
			case VK_LEFT:
				bl_old = true;
				break;

			case VK_RIGHT:
				bl_old = false;
				break;

			case VK_UP:
				if (position > 0)
					position--;
				break;

			case VK_DOWN:
				if (position < 3)
					position++;
				break;
		}

		// part
		switch (position)
		{
			case 0: // 투구 없음
				m_p_slot->man_info.helmet = M_NO_WEAR;
				m_p_slot->woman_info.helmet = W_NO_WEAR;
				break;

			case 1: // 투구
				if (bl_old)
				{
					m_p_slot->man_info.helmet = M_OLD_HELMET;
					m_p_slot->woman_info.helmet = W_OLD_HELMET;
				}
				else
				{
					m_p_slot->man_info.helmet = M_NEW_HELMET;
					m_p_slot->woman_info.helmet = W_NEW_HELMET;
				}
				break;

			case 2: // 상의 
				if (bl_old)
				{
					m_p_slot->man_info.coat = M_OLD_COAT;
					m_p_slot->woman_info.coat = W_OLD_COAT;
				}
				else
				{
					m_p_slot->man_info.coat = M_NEW_COAT;
					m_p_slot->woman_info.coat = W_NEW_COAT;
				}
				break;

			case 3: // 하의
				if (bl_old)
				{
					m_p_slot->man_info.trouser = M_OLD_TROUSER;
					m_p_slot->woman_info.trouser = W_OLD_TROUSER;
				}
				else
				{
					m_p_slot->man_info.trouser = M_NEW_TROUSER;
					m_p_slot->woman_info.trouser = W_NEW_TROUSER;
				}
				break;
		}
	}
#endif
*/
}

//-----------------------------------------------------------------------------
// ChangeFace
//
// 
//-----------------------------------------------------------------------------
//void C_VS_UI_NEWCHAR::ChangeFace()
//{
//	switch (m_man_face_list[0])
//	{
//		case 0:
//			// face와 hair는 한 쌍이다.
//			m_p_slot->man_info.face	= M_FACE1;
//			m_p_slot->man_info.hair = M_HAIR1;
//			break;
//
//		case 1:
//			m_p_slot->man_info.face	= M_FACE2;
//			m_p_slot->man_info.hair = M_HAIR2;
//			break;
//
//		case 2:
//			m_p_slot->man_info.face	= M_FACE3;
//			m_p_slot->man_info.hair = M_HAIR3;
//			break;
//	}
//
//	switch (m_woman_face_list[0])
//	{
//		case 0:
//			// face와 hair는 한 쌍이다.
//			m_p_slot->woman_info.face = W_FACE1;
//			m_p_slot->woman_info.hair = W_HAIR1;
//			break;
//
//		case 1:
//			m_p_slot->woman_info.face = W_FACE2;
//			m_p_slot->woman_info.hair = W_HAIR2;
//			break;
//
//		case 2:
//			m_p_slot->woman_info.face = W_FACE3;
//			m_p_slot->woman_info.hair = W_HAIR3;
//			break;
//	}
//}

//-----------------------------------------------------------------------------
//	Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_NEWCHAR::Run(id_t id)
{

	int i = 0;

	if( 
			(id >= STR_PLUS_ID && id <= INT_MINUS_ID && m_p_slot->Race !=  RACE_OUSTERS ) ||
			(id == REROLL_ID && m_p_slot->Race != RACE_SLAYER )
		  )
			return;

	switch (id)
	{
		case SLAYER_ID:
//			m_p_slot->bl_vampire = false;
			m_p_slot->Race = RACE_SLAYER;
//			m_p_slot->man_info.coat = M_UP_BODY;
//			m_p_slot->woman_info.coat = W_UP_BODY;
			m_p_slot->man_info.helmet = M_NO_WEAR;
			m_p_slot->woman_info.helmet = W_NO_WEAR;
			m_p_slot->man_info.right = M_NO_WEAR;
			m_p_slot->woman_info.right = W_NO_WEAR;
			m_p_slot->man_info.coat = M_OLD_COAT;
			m_p_slot->woman_info.coat = W_OLD_COAT;
			m_p_slot->man_info.trouser = M_OLD_TROUSER;
			m_p_slot->woman_info.trouser = W_OLD_TROUSER;

			Run(MALE_ID+rand()%2);
			for(i = 0; i < rand()%5; i++)
				Run(FACE_NEXT_ID);
			m_skin_point.x = rand()%COLOR_LIST_X;
			m_skin_point.y = rand()%COLOR_LIST_Y;
			m_hair_point.x = rand()%COLOR_LIST_X;
			m_hair_point.y = rand()%COLOR_LIST_Y;
			m_p_slot->hair_color = m_hair_color_array[m_hair_point.x][m_hair_point.y];
			m_p_slot->skin_color = m_skin_color_array[m_skin_point.x][m_skin_point.y];
			RollDice();
			break;

		case VAMPIRE_ID:
//			m_p_slot->bl_vampire = true;
			m_p_slot->Race = RACE_VAMPIRE;
			m_p_slot->man_info.coat = (CHAR_MAN)0;
			m_p_slot->woman_info.coat = (CHAR_WOMAN)1;
			Run(MALE_ID+rand()%2);
			m_skin_point.x = rand()%COLOR_LIST_X;
			m_skin_point.y = rand()%COLOR_LIST_Y;
			m_p_slot->skin_color = m_skin_color_array[m_skin_point.x][m_skin_point.y];
			RollDice();
			break;

		case OUSTERS_ID:
//			m_p_slot->bl_vampire = true;
			m_p_slot->Race = RACE_OUSTERS;
			m_p_slot->man_info.coat = (CHAR_MAN)1;
			m_p_slot->woman_info.coat = (CHAR_WOMAN)1;
			m_p_slot->man_info.right = (CHAR_MAN)0;
			m_p_slot->woman_info.right = (CHAR_WOMAN)0;
			m_hair_point.x = rand()%COLOR_LIST_X;
			m_hair_point.y = rand()%COLOR_LIST_Y;
			m_p_slot->hair_color = m_hair_color_array[m_hair_point.x][m_hair_point.y];
			RollDice();
			break;
			
		case MALE_ID:
			m_p_slot->bl_female = false;
			break;

		case FEMALE_ID:
			m_p_slot->bl_female = true;
			break;

		case NEXT_ID:
			//
			// character created
			//
			//if (gC_font.GetStringBuffer()->str.length() == 0)
			if (m_lev_name.Size() == 0)
			{
				g_msg_empty_name->Start();
			}
			else if(m_p_slot->Race == RACE_OUSTERS && m_p_slot->bonus_point != 0)
			{
				g_msg_left_bonus_point->Start();
			}
			else
			{
				SendNewCharacterToClient();
			}
			break;

		case BACK_ID:
			Finish();
			gC_vs_ui.StartCharacterManager(true);
			break;

		case FACE_BACK_ID:			
			if( m_p_slot->Race == RACE_SLAYER )
			{
				if(m_p_slot->bl_female)
				{
					switch(m_p_slot->woman_info.hair)
					{
					case W_HAIR1:
						m_p_slot->woman_info.hair = W_HAIR3;
						m_p_slot->woman_info.face = W_FACE3;
						break;
						
					case W_HAIR2:
						m_p_slot->woman_info.hair = W_HAIR1;
						m_p_slot->woman_info.face = W_FACE1;
						break;
						
					case W_HAIR3:
						m_p_slot->woman_info.hair = W_HAIR2;
						m_p_slot->woman_info.face = W_FACE2;
						break;
					}
				}
				else
				{
					switch(m_p_slot->man_info.hair)
					{
					case M_HAIR1:
						m_p_slot->man_info.hair = M_HAIR3;
						m_p_slot->man_info.face = M_FACE3;
						break;
						
					case M_HAIR2:
						m_p_slot->man_info.hair = M_HAIR1;
						m_p_slot->man_info.face = M_FACE1;
						break;
						
					case M_HAIR3:
						m_p_slot->man_info.hair = M_HAIR2;
						m_p_slot->man_info.face = M_FACE2;
						break;
					}
				}
			}
			break;

		case FACE_NEXT_ID:
			if( m_p_slot->Race == RACE_SLAYER )
			{
				if(m_p_slot->bl_female)
				{
					switch(m_p_slot->woman_info.hair)
					{
					case W_HAIR2:
						m_p_slot->woman_info.hair = W_HAIR3;
						m_p_slot->woman_info.face = W_FACE3;
						break;
						
					case W_HAIR3:
						m_p_slot->woman_info.hair = W_HAIR1;
						m_p_slot->woman_info.face = W_FACE1;
						break;
						
					case W_HAIR1:
						m_p_slot->woman_info.hair = W_HAIR2;
						m_p_slot->woman_info.face = W_FACE2;
						break;
					}
				}
				else
				{
					switch(m_p_slot->man_info.hair)
					{
					case M_HAIR2:
						m_p_slot->man_info.hair = M_HAIR3;
						m_p_slot->man_info.face = M_FACE3;
						break;
						
					case M_HAIR3:
						m_p_slot->man_info.hair = M_HAIR1;
						m_p_slot->man_info.face = M_FACE1;
						break;
						
					case M_HAIR1:
						m_p_slot->man_info.hair = M_HAIR2;
						m_p_slot->man_info.face = M_FACE2;
						break;
					}
				}
			}
			break;

		case SAVE_ID:
			if( m_p_slot->Race == RACE_SLAYER )
			{
				m_iSave[0] = m_p_slot->STR_PURE;
				m_iSave[1] = m_p_slot->DEX_PURE;
				m_iSave[2] = m_p_slot->INT_PURE;
			}
			break;

		case LOAD_ID:
			if( m_p_slot->Race == RACE_SLAYER )
				RollDice(true);
			break;

		case REROLL_ID:
			if( m_p_slot->Race == RACE_SLAYER )
				RollDice();
			break;

		case CHECK_ID:
			if (m_lev_name.Size() == 0)
			{
				g_msg_empty_name->Start();
			}
			else
			{
//				DeleteNew(m_p_slot->sz_name);
				char *sz_temp;
				g_Convert_DBCS_Ascii2SingleByte(m_lev_name.GetString(), m_lev_name.Size(), sz_temp);
				m_p_slot->sz_name = sz_temp;
				DeleteNew(sz_temp);
				gpC_base->SendMessage(UI_NEWCHARACTER_CHECK, 0, 0, (void *)m_p_slot->sz_name.c_str());
			}
			break;

		case STR_PLUS_ID:
			if(m_p_slot->bonus_point > 0)
			{
				m_p_slot->STR_PURE++;
				m_p_slot->bonus_point--;
			}
			break;

		case STR_MINUS_ID:
			if(m_p_slot->STR_PURE > 10)
			{
				m_p_slot->STR_PURE--;
				m_p_slot->bonus_point++;
			}
			break;

		case DEX_PLUS_ID:
			if(m_p_slot->bonus_point > 0)
			{
				m_p_slot->DEX_PURE++;
				m_p_slot->bonus_point--;
			}
			break;
			
		case DEX_MINUS_ID:
			if(m_p_slot->DEX_PURE > 10)
			{
				m_p_slot->DEX_PURE--;
				m_p_slot->bonus_point++;
			}
			break;
			
		case INT_PLUS_ID:
			if(m_p_slot->bonus_point > 0)
			{
				m_p_slot->INT_PURE++;
				m_p_slot->bonus_point--;
			}
			break;
			
		case INT_MINUS_ID:
			if(m_p_slot->INT_PURE > 10)
			{
				m_p_slot->INT_PURE--;
				m_p_slot->bonus_point++;
			}
			break;
			
	}
}

//void C_VS_UI_NEWCHAR::UsedID(bool used)
//{
//	if(used)
//		g_msg_character_used_id->Start();
//	else
//		g_msg_character_empty_id->Start();
//}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_NEWCHAR::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	m_pC_button_group->MouseControl(message, _x, _y);

//	if (m_bl_change_face)
//	{
//		m_pC_button_group_face->MouseControl(message, _x, _y);
//	}

	const RECT rect[HELP_MAX] =
	{
		{0, 0, 0, 0},			//default
		{0, 0, 0, 0},			//back
		{0, 0, 0, 0},			//next
		{300, 175, 496, 205},	//name
		{0, 0, 0, 0},			//check
		{300, 220, 540, 250},	//tribe
		{300, 250, 540, 290},	//gender
		{290, 345, 410, 400},	//haircolor
		{290, 400, 410, 460},	//skincolor
		{300, 290, 545, 485},	//face type
		{570, 245, 725, 270},		//str
		{570, 269, 725, 295},		//dex
		{570, 294, 725, 320},		//int
		{570, 319, 725, 345},		//hp
		{570, 344, 725, 370},		//mp
		{570, 369, 725, 395},		//defence
		{570, 394, 725, 420},		//protection
		{570, 419, 725, 445},		//tohit
		{570, 444, 725, 470},		//damage
		{0, 0, 0, 0},		//save
		{0, 0, 0, 0},		//load
		{0, 0, 0, 0},		//reroll
		{570, 469, 725, 495},		//bonus

	};

	// control color table
	switch (message)
	{
		case M_MOVING:
			m_focused_help = HELP_DEFAULT;
			{
				for(int i = 0; i < HELP_MAX; i++)
				{
					if(_x > rect[i].left && _x < rect[i].right && _y > rect[i].top && _y < rect[i].bottom)
					{
						m_focused_help = i;
						break;
					}
				}
			}
			if(m_focused_help == HELP_BONUS && m_p_slot->Race != RACE_OUSTERS)
				m_focused_help = HELP_DEFAULT;
			if (m_bl_pushed_table)
				ChangeColor(_x, _y);
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			if (ChangeColor(_x, _y) == true)
			{
				m_bl_pushed_table = true;
				return true;
			}
			break;

		case M_LEFTBUTTON_UP:
			m_bl_pushed_table = false;
			break;
	}

	return true;
}

//-----------------------------------------------------------------------------
// SetEnergy
//
// 
//-----------------------------------------------------------------------------
//void C_VS_UI_NEWCHAR::SetEnergy(int cur_val, int max_val, WORD &percent, WORD &line)
//{
//	if (max_val == 0 || cur_val > max_val)
//		return;
//
//	if (max_val > 0)
//	{
//		float f = cur_val / (float)max_val;
//
//		percent = f*100;
//
//		f = (percent / 100.) * TOTAL_ENERGY_LINE;
//
//		WORD n = f;
//		float t = n;
//		float r = f - t;
//		if (r > 0.)
//			n += 1; // 반올림.
//
//		if (percent < 100 && n >= TOTAL_ENERGY_LINE)
//			n = TOTAL_ENERGY_LINE - 1;
//
//		line = n;
//	}
//}

//-----------------------------------------------------------------------------
// DrawHP (Based on Vampire Energy interface)
//
//
//-----------------------------------------------------------------------------
//void C_VS_UI_NEWCHAR::DrawHP(int _x, int _y, int line)
//{
//	if (line < 1 || line > TOTAL_ENERGY_LINE)
//		return;
//
//	if (gpC_base->m_p_DDSurface_back->Lock())
//	{
//		S_SURFACEINFO	src_surface, dest_surface;
//		S_RECT			src_rect, dest_rect;
//		SetSurfaceInfo(&dest_surface, gpC_base->m_p_DDSurface_back->GetDDSD());
//		SetSurfaceInfo(src_surface, m_p_hp_buf, m_hp_width, m_hp_height);
//		SetRect(src_rect, 0, 0, line, m_hp_height);
//		SetRect(dest_rect, _x, _y, line, m_hp_height);
//
//		Bltz(&dest_surface, &src_surface, &dest_rect, &src_rect);
//
//		gpC_base->m_p_DDSurface_back->Unlock();
//	}
//}
//
//-----------------------------------------------------------------------------
// DrawMP
//
// 
//-----------------------------------------------------------------------------
//void C_VS_UI_NEWCHAR::DrawMP(int _x, int _y, int line)
//{
//	if (line < 1 || line > TOTAL_ENERGY_LINE)
//		return;
//
//	if (gpC_base->m_p_DDSurface_back->Lock())
//	{
//		S_SURFACEINFO	src_surface, dest_surface;
//		S_RECT			src_rect, dest_rect;
//		SetSurfaceInfo(&dest_surface, gpC_base->m_p_DDSurface_back->GetDDSD());
//		SetSurfaceInfo(src_surface, m_p_mp_buf, m_mp_width, m_mp_height);
//		SetRect(src_rect, 0, 0, line, m_mp_height);
//		SetRect(dest_rect, _x, _y, line, m_mp_height);
//
//		Bltz(&dest_surface, &src_surface, &dest_rect, &src_rect);
//
//		gpC_base->m_p_DDSurface_back->Unlock();
//	}
//}

//-----------------------------------------------------------------------------
// DrawEnergy
//
// 
//-----------------------------------------------------------------------------
//void C_VS_UI_NEWCHAR::DrawEnergy()
//{
//	assert(m_p_slot->hp_cur_line <= TOTAL_ENERGY_LINE);
//	assert(m_p_slot->mp_cur_line <= TOTAL_ENERGY_LINE);
//
//	DrawHP(ENERGY_X, ENERGY_HP_Y, m_p_slot->hp_cur_line);
//	DrawMP(ENERGY_X, ENERGY_MP_Y, m_p_slot->mp_cur_line);
//}


//-----------------------------------------------------------------------------
// ShowCharacter
//
// (x, y)에 남/여 캐릭터 하나를 출력한다. 따라서 어느 하나는 null로 되야 하다.
//
// 살부분 : i1
// 옷부분 : i2
//-----------------------------------------------------------------------------
void	C_VS_UI_NEWCHAR::ShowCharacter(int _x, int _y, S_SLOT * p_slot, int index, int enable, int dark)
{
	//
	// 출력순서
	//
	// 바지 -> 상의 -> 머리카락 -> 투구 -> 오른손장착 -> 왼손장착
	//
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		switch(p_slot->Race)
		{
		case RACE_OUSTERS:
			if( p_slot->m_AdvancementLevel > 0 )
				_ShowCharacterACOusters( _x, _y, p_slot, index, enable, dark );
			else
				_ShowCharacterOusters( _x, _y, p_slot, index, enable, dark );
			break;
			
		case RACE_VAMPIRE:
			if( p_slot->m_AdvancementLevel > 0 )
				_ShowCharacterACVampire( _x, _y, p_slot, index, enable, dark );
			else
				_ShowCharacterVampire( _x, _y, p_slot, index, enable, dark );
			break;
			
		case RACE_SLAYER:
			if( p_slot->m_AdvancementLevel > 0 )
				_ShowCharacterACSlayer( _x, _y, p_slot, index, enable, dark );
			else
				_ShowCharacterSlayer( _x, _y, p_slot, index, enable, dark );
			break;
		}
		gpC_base->m_p_DDSurface_back->Unlock();
	}
}

//-----------------------------------------------------------------------------
// DrawCover
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_NEWCHAR::DrawCover(int _x, int _y)
{
	//m_pC_new_char_spk->Blt(_x, _y, COVER);
}

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_NEWCHAR::Show()
{
	gpC_base->m_p_DDSurface_back->FillSurface(0);

	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_common_spk.BltLocked(0, 0);
		
		switch( m_p_slot->Race )
		{
		case RACE_SLAYER:
			if(m_p_slot->bl_female)
				m_face_spk.BltLocked(426, 312, m_p_slot->woman_info.hair-W_HAIR1+GENDER_OFFSET);
			else
				m_face_spk.BltLocked(426, 312, m_p_slot->man_info.hair-M_HAIR1);
			break;

		case RACE_VAMPIRE:
			if(m_p_slot->bl_female)
				m_face_spk.BltLocked(426, 312, 7);
			else
				m_face_spk.BltLocked(426, 312, 6);
			break;

		case RACE_OUSTERS:
			m_face_spk.BltLocked(426, 312, 8);
			break;			
		}		
		
		m_image_spk.BltLocked(250, 150);
		m_image_spk.BltLocked(TITLE_X - m_image_spk.GetWidth(TITLE)/2, TITLE_Y, TITLE);

		gpC_base->m_p_DDSurface_back->Unlock();
		RECT rect = {68, 355, 171, 475};
		gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
		if(m_p_slot->Race == RACE_OUSTERS)
		{
			SetRect(&rect, 667, 470, 723, 490);
			gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
		}
		gpC_base->m_p_DDSurface_back->Lock();
		
		if(m_p_slot->Race == RACE_OUSTERS)
		{
			m_image_spk.BltLocked(573, 470, BONUS_LINE);
		}
		
		m_image_spk.BltLocked(55, 350, CHARACTER_GUARD);		
		gpC_base->m_p_DDSurface_back->Unlock();
	}

//	DrawEnergy();

	// HP/MP cover
//	DrawCover(ENERGY_X, 427);
//	DrawCover(ENERGY_X, 482);

	//
	// color table
	//
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		int i, j;
		S_SURFACEINFO	surfaceinfo;
		SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());

		Rect color_unit_rect;

		color_unit_rect.WH(COLOR_UNIT_X, COLOR_UNIT_Y);

		// hair
		if(m_p_slot->Race != RACE_VAMPIRE)
		{
			for (j=0; j < COLOR_LIST_Y; j++)
				for (i=0; i < COLOR_LIST_X; i++)
				{
					color_unit_rect.XY(TABLE_X+COLOR_UNIT_X*i, TABLE_Y+COLOR_UNIT_Y*j);
					
					FillRect(&surfaceinfo, &color_unit_rect, GetColor(i, j, false));
				}
				
				// show select mark
				color_unit_rect.XY(TABLE_X+m_hair_point.x*COLOR_UNIT_X, TABLE_Y+m_hair_point.y*COLOR_UNIT_Y);
				rectangle(&surfaceinfo, &color_unit_rect, ga_blink_color_table[g_blink_value]);
		}
		else
		{
			m_p_slot->hair_color = 377;
		}

		if(m_p_slot->Race != RACE_OUSTERS)
		{
			// body
			for (j=0; j < COLOR_LIST_Y; j++)
				for (i=0; i < COLOR_LIST_X; i++)
				{
					color_unit_rect.XY(TABLE_X+COLOR_UNIT_X*i, TABLE_Y2+COLOR_UNIT_Y*j);

					FillRect(&surfaceinfo, &color_unit_rect, GetColor(i, j, true));
				}

			// show select mark
			color_unit_rect.XY(TABLE_X+m_skin_point.x*COLOR_UNIT_X, TABLE_Y2+m_skin_point.y*COLOR_UNIT_Y);
			rectangle(&surfaceinfo, &color_unit_rect, ga_blink_color_table[g_blink_value]);
		}
		else
		{
			m_p_slot->skin_color = 377;
		}

		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	ShowCharacter(95, 440, m_p_slot, g_char_index, false);

	switch(m_p_slot->Race)
	{
	case RACE_SLAYER:
		m_image_spk.Blt(411,229-6, MALE_CHECK);
		break;

	case RACE_VAMPIRE:
		m_image_spk.Blt(480,229-6, MALE_CHECK);
		break;

	case RACE_OUSTERS:
		m_image_spk.Blt(411,229-6+23, MALE_CHECK);
		break;	
	}	
	
	if(m_p_slot->Race != RACE_OUSTERS)
	{
		if(m_p_slot->bl_female)
			m_image_spk.Blt(473,268+13, MALE_CHECK);
		else
			m_image_spk.Blt(411,268+13, MALE_CHECK);
	}	

	// show point
	char str[10];
	g_FL2_GetDC();
	sprintf(str, "%d", m_p_slot->STR_PURE);
	g_Print(699+15, 249, str, &gpC_base->m_value_pi);
	sprintf(str, "%d", m_p_slot->DEX_PURE);
	g_Print(699+15, 249+25, str, &gpC_base->m_value_pi);
	sprintf(str, "%d", m_p_slot->INT_PURE);
	g_Print(699+15, 249+25*2, str, &gpC_base->m_value_pi);
	sprintf(str, "%d", m_p_slot->HP);
	g_Print(699+15, 250+25*3, str, &gpC_base->m_value_pi);
	sprintf(str, "%d", m_p_slot->MP);
	g_Print(699+15, 250+25*4, str, &gpC_base->m_value_pi);
	sprintf(str, "%d", m_p_slot->DEFENSE);
	g_Print(699+15, 250+25*5, str, &gpC_base->m_value_pi);
	sprintf(str, "%d", m_p_slot->PROTECTION);
	g_Print(699+15, 249+25*6, str, &gpC_base->m_value_pi);
	sprintf(str, "%d", m_p_slot->TOHIT);
	g_Print(699+15, 250+25*7, str, &gpC_base->m_value_pi);
	sprintf(str, "%d~%d", m_p_slot->DAM, m_p_slot->DAM2);
	g_Print(699+15, 250+25*8, str, &gpC_base->m_value_pi);
	if(m_p_slot->Race == RACE_OUSTERS)
	{
		sprintf(str, "%d", m_p_slot->bonus_point);
		g_Print(699+15, 250+25*9, str, &gpC_base->m_value_pi);
	}
	

/*
	// 문자열 출력 
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO	surfaceinfo;
		SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());

		gpC_base->SelectFont(FONT_SLAYER);

		gC_font.Update(&surfaceinfo, 445, 145, WHITE);

		// show point
		char str[3];
		sprintf(str, "%2d", m_p_slot->STR);
		gC_font.PrintString(&surfaceinfo, str, 433, 280, WHITE);
		sprintf(str, "%2d", m_p_slot->DEX);
		gC_font.PrintString(&surfaceinfo, str, 433, 280+46, WHITE);
		sprintf(str, "%2d", m_p_slot->INT);
		gC_font.PrintString(&surfaceinfo, str, 433, 280+92, WHITE);
		sprintf(str, "%2d", m_p_slot->DAM);
		gC_font.PrintString(&surfaceinfo, str, 723, 422, WHITE);
		sprintf(str, "%2d", m_p_slot->AC);
		gC_font.PrintString(&surfaceinfo, str, 723, 422+46, WHITE);
		sprintf(str, "%2d", m_p_slot->TOHIT);
		gC_font.PrintString(&surfaceinfo, str, 723, 422+92, WHITE);

		gpC_base->m_p_DDSurface_back->Unlock();
	}*/

	// draw Name board & name
//	m_pC_board_spk->Blt(NAME_BOARD_X, NAME_BOARD_Y, NAME_BOARD);
/*
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO	surfaceinfo;
		SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());

		int board_wh = m_pC_board_spk->GetWidth(NAME_BOARD)/2;

		int bx = NAME_BOARD_X+board_wh - gC_font.GetStringWidth(gC_font.GetStringBuffer()->str.c_str())/2;
		gC_font.PrintStringNoConvert(&surfaceinfo, gC_font.GetStringBuffer()->str.c_str(), bx, NAME_BOARD_Y+18, WHITE);

		gpC_base->m_p_DDSurface_back->Unlock();
	}*/

	
//	m_pC_button_group_face->Show();

//	m_pC_ani_fin->Show();

	Window::ShowWidget();

#ifndef _LIB
	extern bool gbl_info_show;
	if(gbl_info_show)
	{
	char debug_str[100];
	sprintf(debug_str, "hair color set = %d (%d)", m_p_slot->hair_color, GetColor(m_hair_point.x, m_hair_point.y, false));
	g_Print(120, 540, debug_str);

	sprintf(debug_str, "skin color set = %d (%d)", m_p_slot->skin_color, GetColor(m_skin_point.x, m_skin_point.y, true));
	g_Print(120, 560, debug_str);
	}
#endif
	char szBuffer[256];

	
	if (g_LeftPremiumDays == 0xFFFE )
		sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_PREMIUM_USER].GetString() );
	else if ( g_LeftPremiumDays > 0 )
	{
		if(g_bFamily)
			sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_FAMILY_DAYS].GetString(), g_LeftPremiumDays );
		else
			sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_PREMIUM_DAYS].GetString(), g_LeftPremiumDays );
	}
	else
	{
		if(g_bFamily)
			sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_EXPIRE_FAMILY_TODAY].GetString() );	
		else
			sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_EXPIRE_PREMIUM_SERVICE_TODAY].GetString() );	
	}

	if( g_LeftPremiumDays != 0xFFFF )
		g_PrintColorStr( g_vs_ui_title_only_premium_x, g_vs_ui_title_only_premium_y, szBuffer, gpC_base->m_desc_menu_pi, RGB_WHITE );
	
	g_FL2_ReleaseDC();

	std::string help_string[HELP_MAX] = 
	{
		"create_window.txt",
		"back.txt",
		"next.txt",
		"name.txt",
		"check.txt",
		"tribe.txt",
		"gender.txt",
		"haircolor.txt",
		"skincolor.txt",
		"facetype.txt",
		"str.txt",
		"dex.txt",
		"int.txt",
		"hp.txt",
		"mp.txt",
		"defence.txt",
		"protection.txt",
		"tohit.txt",
		"damage.txt",
		"save.txt",
		"load.txt",
		"reroll.txt",
		"bonus.txt",
	};

	static int old_focused_help = -1;
	if(old_focused_help != m_focused_help && m_focused_help < HELP_MAX)
	{
		LoadDesc(help_string[m_focused_help].c_str(), 30, 14, false);
		old_focused_help = m_focused_help;
	}

	ShowDesc();

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// NewCharacterCreateOk
//
// 이미 slot이 설정되어 있으므로, 마지막으로 bl_set = true만 해준다.
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_MANAGER::NewCharacterCreateOk()
{
	assert(m_slot[m_select_heart_temp].bl_set == false);

	m_slot[m_select_heart_temp].bl_set = true;
//	m_slot[m_select_heart_temp].bl_vampire = false;

	// init etc value
//	m_slot[m_select_heart_temp].CC = 0;
	m_slot[m_select_heart_temp].FAME = 1234567890;
//	m_slot[m_select_heart_temp].NOTERITY = 0;
	m_slot[m_select_heart_temp].level = 1;

	SelectSlot(m_select_heart_temp);

	Start(true);
//	m_pC_newchar->Finish();

}

//-----------------------------------------------------------------------------
// NewCharacterCreateFailed
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_MANAGER::NewCharacterCreateFailed(int error)
{
	g_msg_character_create_failed->Start();
}

//-----------------------------------------------------------------------------
// SetCharacter
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_MANAGER::SetCharacter(int slot, S_SLOT &S_slot)
{
	if (slot >= 0 && slot < SLOT)
	{
		if (S_slot.sz_name.c_str() == NULL)
			_ErrorStr("Null Character name entered.");

		//----------------------------------------------------
		// by sigi
		//----------------------------------------------------
//		if (m_slot[slot].sz_name.c_str()!=NULL)
//		{
//			delete [] m_slot[slot].sz_name;
//			m_slot[slot].sz_name = NULL;
//		}
//		if (m_slot[slot].sz_guild_name!=NULL)
//		{
//			delete [] m_slot[slot].sz_guild_name;
//			m_slot[slot].sz_guild_name = NULL;
//		}

		m_slot[slot] = S_slot;
		m_slot[slot].bl_set = true;
		//m_slot[slot].bl_vampire = S_slot.bl_vampire;


//		m_slot[slot].sz_name = new char[strlen(S_slot.sz_name)+1];
//		strcpy(m_slot[slot].sz_name, S_slot.sz_name);		
		m_slot[slot].sz_name = S_slot.sz_name;

		// by sigi
		if (S_slot.sz_guild_name.c_str()!=NULL)
		{
//			m_slot[slot].sz_guild_name = new char[strlen(S_slot.sz_guild_name)+1];
//			strcpy(m_slot[slot].sz_guild_name, S_slot.sz_guild_name);
			m_slot[slot].sz_guild_name = S_slot.sz_guild_name;
		}

		// HP/MP를 출력하기 위해 필요한 값들은 여기서 설정된다.
//		m_pC_newchar->SetEnergy(m_slot[slot].HP, m_slot[slot].HP_MAX, m_slot[slot].hp_percent, m_slot[slot].hp_cur_line);
//		m_pC_newchar->SetEnergy(m_slot[slot].MP, m_slot[slot].MP_MAX, m_slot[slot].mp_percent, m_slot[slot].mp_cur_line);

/*
#ifndef _LIB
		// default
		m_slot[slot].man_info.coat = M_UP_BODY;
		m_slot[slot].man_info.face = M_FACE1;
		m_slot[slot].man_info.hair = M_HAIR1;
		m_slot[slot].man_info.helmet = M_NO_WEAR;
		m_slot[slot].man_info.trouser = M_DOWN_BODY;
		m_slot[slot].woman_info.coat = W_UP_BODY;
		m_slot[slot].woman_info.face = W_FACE1;
		m_slot[slot].woman_info.hair = W_HAIR1;
		m_slot[slot].woman_info.helmet = W_NO_WEAR;
		m_slot[slot].woman_info.trouser = W_DOWN_BODY;

		m_slot[slot].hair_color = 0;
		m_slot[slot].skin_color = 405;
#endif*/

		// search new character for selection
		if(m_select_heart == NOT_SELECTED)
			for (int i=0; i < SLOT; i++)
				if (SelectSlot(i))
					break;
	}
}

//-----------------------------------------------------------------------------
// ClearAllCharacter
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_MANAGER::ClearAllCharacter()
{
	m_slot[SLOT_LEFT].bl_set = false;
	m_slot[SLOT_CENTER].bl_set = false;
	m_slot[SLOT_RIGHT].bl_set = false;
}

//-----------------------------------------------------------------------------
// DeleteCharacter
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_MANAGER::DeleteCharacter(int slot)
{
	assert(m_slot[slot].bl_set == true);
	assert(m_slot[slot].sz_name.c_str() != NULL);

	m_slot[slot].bl_set = false;
//	DeleteNewArray(m_slot[slot].sz_name);

	DeleteNew(m_pC_char_delete);

	// search new character for selection
	m_select_heart = NOT_SELECTED;
	for (int i=0; i < SLOT; i++)
		if (SelectSlot(i))
			break;

	g_msg_char_delete_completed->Start();
//	g_msg_char_delete_completed->AttrKeyboardControl(true);
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_MANAGER::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_MANAGER::Start(bool back)
{
	PI_Processor::Start();

	m_pC_newchar->Finish();

	if(!back)
	{
		m_slot[SLOT_LEFT].bl_set	= false;
		m_slot[SLOT_CENTER].bl_set = false;
		m_slot[SLOT_RIGHT].bl_set	= false;
//		DeleteNewArray(m_slot[SLOT_LEFT].sz_name);
//		DeleteNewArray(m_slot[SLOT_CENTER].sz_name);
//		DeleteNewArray(m_slot[SLOT_RIGHT].sz_name);
		
		m_select_heart	= NOT_SELECTED;
	}
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();

	gbl_wood_skin = false;
//	gbl_vampire_interface = false;

	g_eRaceInterface = RACE_SLAYER;

	// change skin data
	gpC_global_resource->FreeAssemble();
	gpC_global_resource->LoadAssemble();

	LoadDesc("character.txt", 30, 14, false);

	EMPTY_MOVE;
}

//-----------------------------------------------------------------------------
// Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_MANAGER::Finish()
{

	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

/*-----------------------------------------------------------------------------
- C_VS_UI_CHAR_MANAGER
-
-----------------------------------------------------------------------------*/
C_VS_UI_CHAR_MANAGER::C_VS_UI_CHAR_MANAGER()
{
	g_RegisterWindow(this);

	AttrKeyboardControl(true);

	m_pC_newchar = new C_VS_UI_NEWCHAR;
//	m_pC_char_info = NULL;
	m_pC_char_delete = NULL;
	m_pC_biling = NULL;

	// add by Sonic 2006.9.26
	if(g_MyFull)
		m_common_spk.Open(SPK_COMMON_1024);
	else
		m_common_spk.Open(SPK_COMMON);
	m_title1_spk.Open(SPK_TITLE_BACK);//뗍혤뒀暠
	// end by sonic
	m_image_spk.Open(SPK_CHAR_MANAGER);

	// 넷마블이 아닌경우
	if(g_pUserInformation!=NULL)
	{
		if(!g_pUserInformation->bChinese)
		{
			 m_pC_use_grade = new C_SPRITE_PACK(SPK_USE_GRADE);
		} else 
			m_pC_use_grade = NULL;
	} else
	{
		m_pC_use_grade = NULL;
	}
	// add by Sonic 2006.9.26
	if(g_MyFull)
	{
		Set(0, 0, 1024, 768);
	}
	else
	{
		Set(0, 0, 800, 600);
	}
	//end
//	Set(g_GameRect.right/2 - m_pC_back.GetWidth()/2, g_GameRect.bottom/2 - m_pC_back.GetHeight()/2, m_pC_back.GetWidth(), m_pC_back.GetHeight());
	

	m_pC_button_group = new ButtonGroup(this);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(28, 522, m_common_spk.GetWidth(BACK_BUTTON), m_common_spk.GetHeight(BACK_BUTTON), BACK_ID, this, BACK_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(687, 522, m_common_spk.GetWidth(NEXT_BUTTON), m_common_spk.GetHeight(NEXT_BUTTON), NEXT_ID, this, NEXT_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(555, 542, m_image_spk.GetWidth(CREATE_BUTTON), m_image_spk.GetHeight(CREATE_BUTTON), CREATE_ID, this, CREATE_BUTTON));

	for(int i = 0; i < 3; i++)
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+g_heart_rect[i]+119, y+HEART_Y+36, m_image_spk.GetWidth(DELETE_BUTTON), m_image_spk.GetHeight(DELETE_BUTTON), DELETE_1_ID+i, this, DELETE_BUTTON));

	//
	// character ani timer는 char manager object가 생성되면 계속 작동한다.
	// 각 character의 처음 frame index를 다름으로써 자연스러움을 구현한다.
	//
	g_char_update_tid = gC_timer2.Add(ANI_MILLISEC, _Timer_CharUpdate);
	gC_timer2.Continue(g_char_update_tid);

	// ANI_FRAME = 11
	g_char_index = 0;

//	m_slot[SLOT_LEFT].sz_name	= NULL;
//	m_slot[SLOT_LEFT].sz_guild_name	= NULL;
//	m_slot[SLOT_CENTER].sz_name = NULL;
//	m_slot[SLOT_CENTER].sz_guild_name = NULL;
//	m_slot[SLOT_RIGHT].sz_name	= NULL;
//	m_slot[SLOT_RIGHT].sz_guild_name	= NULL;

	m_pack_file.SetRAR(RPK_TITLE, RPK_PASSWORD);
	SetDesc(29, 160, RGB(160, 160, 160), gpC_base->m_chatting_pi);
	m_desc_y_distance = 16;

	m_focused_help = HELP_DEFAULT;

}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_CHAR_MANAGER
-
-----------------------------------------------------------------------------*/
C_VS_UI_CHAR_MANAGER::~C_VS_UI_CHAR_MANAGER()
{
	g_UnregisterWindow(this);

	// !Window를 먿저 delete해야 한다.
	DeleteNew(m_pC_biling);
	DeleteNew(m_pC_char_delete);
	DeleteNew(m_pC_newchar);
	DeleteNew(m_pC_use_grade);
//	DeleteNew(m_pC_char_info);

	DeleteNew(m_pC_button_group);

	gC_timer2.Delete(g_char_update_tid);

//	DeleteNewArray(m_slot[SLOT_LEFT].sz_name);
//	DeleteNewArray(m_slot[SLOT_CENTER].sz_name);
//	DeleteNewArray(m_slot[SLOT_RIGHT].sz_name);

//	DeleteNewArray(m_slot[SLOT_LEFT].sz_guild_name);
//	DeleteNewArray(m_slot[SLOT_CENTER].sz_guild_name);
//	DeleteNewArray(m_slot[SLOT_RIGHT].sz_guild_name);

}

//-----------------------------------------------------------------------------
// C_VS_UI_CHAR_MANAGER::CloseCharInfo
//
// 
//-----------------------------------------------------------------------------
//void C_VS_UI_CHAR_MANAGER::CloseCharInfo()
//{
//	if(m_pC_char_info != NULL)
//	{
//		DeleteNew(m_pC_char_info);
//		m_pC_char_info = NULL;
//	}
//
//	gbl_vampire_interface = false;
//
//	// change skin data
//	gpC_global_resource->FreeAssemble();
//	gpC_global_resource->LoadAssemble();
//}

//-----------------------------------------------------------------------------
// C_VS_UI_CHAR_MANAGER::SelectSlot
//
// select되었으면 true를 반환한다.
//-----------------------------------------------------------------------------
bool C_VS_UI_CHAR_MANAGER::SelectSlot(int n)
{
	if (n >= 0 && n < SLOT)
	{
		if (m_slot[n].bl_set == true)
		{
			m_select_heart = n;
			g_char_index = 0;
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// AcquireFirstSequence
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_MANAGER::AcquireFirstSequence()
{
}

//-----------------------------------------------------------------------------
// UnacquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_MANAGER::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_MANAGER::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_CHAR_MANAGER::IsPixel(int _x, int _y)
{
	return true;
//	return m_pC_back.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_MANAGER::Run(id_t id)
{
	int i=-1;

	switch (id)
	{
		case CREATE_ID:
			//
			// 자동으로 left부터 순서대로 비어있는 slot에 새캐릭을 만든다.
			//

			// search empty slot
			for (i=0; i < SLOT; i++)
			{
				if (m_slot[i].bl_set == false)
				{
					m_select_heart_temp = i;
					m_pC_newchar->Start();
					m_pC_newchar->SetCharacterToThisSlot(i, &m_slot[i]);
					m_pC_newchar->RollDice(); // Start()에 넣으면 안된다. - SetCharacterToThisSlot()를 그 뒤에 하기 때문.
					Finish();
					return;
				}
			}

			g_msg_all_slot_created->Start();
			break;

//		case CHARINFO:
//			if (m_slot[m_select_heart].bl_set == true)
//			{
//				//assert(m_pC_char_info == NULL); // 하지말것. WindowManager에서 cancel push state하면 두 번 실행됨.
//				if (m_pC_char_info != NULL)
//				{
//					DeleteNew(m_pC_char_info);
//					m_pC_char_info = NULL;
//				}
//
//				m_pC_char_info = new C_VS_UI_CHAR_INFO(&m_slot[m_select_heart], m_select_heart);
//				
//				m_pC_char_info->SetNewCharacterInterface(m_pC_newchar);
//				m_pC_char_info->Start();
//			}
//			break;
//
//		case TO_BOARD:
//#ifndef _LIB
//			OpenBBS();
//#else
//			g_msg_not_available_menu->Start();
//#endif
//			break;

		case NEXT_ID:
			// 모든 slot이 비어 있는가?
			if (m_slot[SLOT_LEFT].bl_set == false &&
				 m_slot[SLOT_CENTER].bl_set == false &&
				 m_slot[SLOT_RIGHT].bl_set == false)
			{
				g_msg_all_slot_not_char->Start();
				break;
			}

			if (m_slot[m_select_heart].bl_set)
				gpC_base->SendMessage(UI_CONNECT, m_select_heart, 0);
			else
				g_msg_not_select_char->Start();
			break;

		case BACK_ID:
			//Finish();
			gpC_base->SendMessage(UI_CHARACTER_MANAGER_FINISHED); // Finish()에 두지 말것.
			break;

		case DELETE_1_ID:
			i = 0;
		case DELETE_2_ID:
			if(i == -1)
				i = 1;
		case DELETE_3_ID:
			if(i == -1)
				i = 2;

			if (m_slot[i].bl_set == true)
			{
				DeleteNew(m_pC_char_delete);
				
				m_pC_char_delete = new C_VS_UI_CHAR_DELETE();
				m_pC_char_delete->Start();
				m_pC_char_delete->Slot(i);
			}
			break;
		case CANNOT_PLAY_ID :
			i=0;
		case CANNOT_PLAY_BY_ATTR_ID :
			if(i == -1)
				i=1;
			{
				DeleteNew(m_pC_biling);

				m_pC_biling = new C_VS_UI_GO_BILING_PAGE((C_VS_UI_GO_BILING_PAGE::BILING_MSG_LIST) i );
				m_pC_biling->Start();
			}
			break;
	}
}

/*-----------------------------------------------------------------------------
- MouseControl
-
-----------------------------------------------------------------------------*/
bool C_VS_UI_CHAR_MANAGER::MouseControl(UINT message, int _x, int _y)
{
	_x-=x;_y-=y;
	Window::MouseControl(message, _x, _y);

	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	int i;

	switch (message)
	{
		case M_MOVING:
			m_focused_help = HELP_DEFAULT;
			{
				for(int i = 0; i < 3; i++)
				{
					if(_x > x+g_heart_rect[i] && _x < x+g_heart_rect[i] + m_image_spk.GetWidth() &&
						_y > y+HEART_Y && _y < y+HEART_Y + m_image_spk.GetHeight())
					{
						m_focused_help = HELP_MAX+i;
						break;
					}
				}
			}
			break;

		case M_LB_DOUBLECLICK:
			if(re == true)
			if (m_select_heart != NOT_SELECTED)
			{
				if (_x >= g_heart_rect[m_select_heart] && _x < g_heart_rect[m_select_heart]+HEART_WIDTH &&
					 _y >= HEART_Y && _y < HEART_Y+HEART_HEIGHT)
					Run(NEXT_ID);
			}
			break;

		case M_LEFTBUTTON_DOWN:
			if(re == true)
			for (i=0; i < SLOT; i++)
				if (_x >= g_heart_rect[i] && _x < g_heart_rect[i]+HEART_WIDTH &&
					 _y >= HEART_Y && _y < HEART_Y+HEART_HEIGHT)
				{
					if (SelectSlot(i))
						break;
				}
			break;

		case M_LEFTBUTTON_UP:
			break;

		case M_RIGHTBUTTON_DOWN:
			for (i=0; i < SLOT; i++)
				if (_x >= g_heart_rect[i] && _x < g_heart_rect[i]+HEART_WIDTH &&
					 _y >= HEART_Y && _y < HEART_Y+HEART_HEIGHT)
				{
					if (SelectSlot(i))
						break;
				}
			break;

		case M_RIGHTBUTTON_UP:
//			Run(CHARINFO);
			break;

		case M_WHEEL_UP:
			break;

		case M_WHEEL_DOWN:
			break;

	}

	return true;
}

/*-----------------------------------------------------------------------------
- KeyboardControl
-
-----------------------------------------------------------------------------*/
void C_VS_UI_CHAR_MANAGER::KeyboardControl(UINT message, UINT key, long extra)
{
	switch(message)
	{
	case WM_KEYDOWN:
		switch(key)
		{
		case VK_ESCAPE:
			Run(BACK);
			break;

		case VK_RIGHT:
			if(!SelectSlot(m_select_heart+1))SelectSlot(m_select_heart+2);
			break;

		case VK_LEFT:
			if(!SelectSlot(m_select_heart-1))SelectSlot(m_select_heart-2);
			break;

		case VK_RETURN:
			Run(NEXT_ID);
			break;

		case VK_UP:
			break;

		case VK_DOWN:
			break;

		}
		break;
	}
}


void C_VS_UI_CHAR_MANAGER::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
}

//-----------------------------------------------------------------------------
// ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_CHAR_MANAGER::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
//	int index = p_button->m_image_index;

	if(p_button->GetID() == BACK_ID || p_button->GetID() == NEXT_ID)
	{
		if(p_button->GetFocusState())
		{
			if (p_button->GetPressState()) // push state
				m_common_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				m_common_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
			if(p_button->GetID() == BACK_ID)
				m_focused_help = HELP_BACK;
			else
				m_focused_help = HELP_NEXT;
		}
		else
			m_common_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
	}
	else
	{
		if(p_button->GetFocusState())
		{
			if (p_button->GetPressState()) // push state
				m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
			
			switch(p_button->GetID())
			{
			case DELETE_1_ID:
			case DELETE_2_ID:
			case DELETE_3_ID:
				m_focused_help = HELP_X;
				break;

			case CREATE_ID:
				m_focused_help = HELP_CREATE;
				break;
			}
		}
		else
			m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
	}

}

/*-----------------------------------------------------------------------------
- Show
-
-----------------------------------------------------------------------------*/
void C_VS_UI_CHAR_MANAGER::Show()
{
	gpC_base->m_p_DDSurface_back->FillSurface(0);
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_title1_spk.BltLocked(0,0);
		m_common_spk.BltLocked(x, y);
		//	m_image_spk.BltLocked(300, 150);
		m_image_spk.BltLocked(TITLE_X - m_image_spk.GetWidth(TITLE)/2, TITLE_Y, TITLE);
		
		if(m_pC_use_grade!=NULL && !g_pUserInformation->bChinese )
		{
			if(g_pUserInformation->GoreLevel)
				m_pC_use_grade->BltLocked(g_GameRect.right-m_pC_use_grade->GetWidth(ADULT)-30,50,ADULT);
			else
				m_pC_use_grade->BltLocked(g_GameRect.right-m_pC_use_grade->GetWidth(CHILD)-30,50,CHILD);
		}
		gpC_base->m_p_DDSurface_back->Unlock();
	}


	COLORREF disable_color = RGB(100, 100, 100);

	int i;

	// selector
	for (i = 0; i < SLOT; i++)
	{
		if(m_select_heart==i)
			m_image_spk.Blt(x+g_heart_rect[i], y+HEART_Y, WINDOW_HILIGHTED);
		else
			m_image_spk.Blt(x+g_heart_rect[i], y+HEART_Y, WINDOW);

		if (m_slot[i].bl_set)
		{
			int index;
			index = g_char_index;
			g_CurrentFrame = index;
			m_pC_newchar->ShowCharacter(x+g_heart_rect[i]+55, y+HEART_Y+170, &m_slot[i], index, (m_select_heart==i)?1:0, !(m_select_heart==i));
			g_PrintColorStr(x+g_heart_rect[i]+76 - g_GetStringWidth(m_slot[i].sz_name.c_str(), gpC_base->m_info_pi.hfont)/2, y+HEART_Y+42, m_slot[i].sz_name.c_str(), gpC_base->m_info_pi, RGB_WHITE);
		}
	}
	std::string help_string[HELP_MAX] = 
	{
		"character.txt",
		"back.txt",
		"next.txt",
		"X.txt",
		"create.txt",
	};

	static int old_focused_help = -1;
	if(old_focused_help != m_focused_help && m_focused_help < HELP_MAX)
	{
		LoadDesc(help_string[m_focused_help].c_str(), 30, 14, false);
		old_focused_help = m_focused_help;
	}

	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_button_group->Show();	
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	if(m_focused_help < HELP_MAX)
		ShowDesc();
	else
	{
		g_FL2_GetDC();
		
		if(m_slot[m_focused_help - HELP_MAX].bl_set)
		{
			S_SLOT *m_p_slot = &m_slot[m_focused_help - HELP_MAX];
			int px, py = 160;

			char str[10];
			px = g_PrintColorStr(29, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_NAME].GetString(), gpC_base->m_chatting_pi, RGB(160, 160, 160));
			g_PrintColorStr(px, py, m_p_slot->sz_name.c_str(), gpC_base->m_chatting_pi, RGB(160, 160, 160));
			py+=16;

			if(m_p_slot->Race != RACE_SLAYER)
			{
				px = g_PrintColorStr(29, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_LEVEL].GetString(), gpC_base->m_chatting_pi, RGB(160, 160, 160));
				wsprintf(str, "%d", m_p_slot->level);
				g_PrintColorStr(px, py, str, gpC_base->m_chatting_pi, RGB(160, 160, 160));
				py+=16;
			}

			px = g_PrintColorStr(29, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_STR_PURE].GetString(),gpC_base->m_chatting_pi, RGB(160, 160, 160));
			wsprintf(str, "%d", m_p_slot->STR_PURE);
			g_PrintColorStr(px, py, str, gpC_base->m_chatting_pi, RGB(160, 160, 160));
			py+=16;

			px = g_PrintColorStr(29, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_DEX_PURE].GetString(), gpC_base->m_chatting_pi, RGB(160, 160, 160));
			wsprintf(str, "%d", m_p_slot->DEX_PURE);
			g_PrintColorStr(px, py, str, gpC_base->m_chatting_pi, RGB(160, 160, 160));
			py+=16;

			px = g_PrintColorStr(29, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_INT_PURE].GetString(), gpC_base->m_chatting_pi, RGB(160, 160, 160));
			wsprintf(str, "%d", m_p_slot->INT_PURE);
			g_PrintColorStr(px, py, str, gpC_base->m_chatting_pi, RGB(160, 160, 160));
			py+=16;

			switch(m_p_slot->Race)
			{
			case RACE_SLAYER:
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
					
					px = g_PrintColorStr(29, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_SWORD_LEVEL].GetString(), gpC_base->m_chatting_pi, RGB(160, 160, 160));
					wsprintf(str, "%d", m_p_slot->DOMAIN_SWORD);
					g_PrintColorStr(px, py, str, gpC_base->m_chatting_pi, RGB(160, 160, 160));
					py+=16;
					
					px = g_PrintColorStr(29, py, 	(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_BLADE_LEVEL].GetString(), gpC_base->m_chatting_pi, RGB(160, 160, 160));
					wsprintf(str, "%d", m_p_slot->DOMAIN_BLADE);
					g_PrintColorStr(px, py, str, gpC_base->m_chatting_pi, RGB(160, 160, 160));
					py+=16;
					
					px = g_PrintColorStr(29, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_GUN_LEVEL].GetString(),gpC_base->m_chatting_pi, RGB(160, 160, 160));
					wsprintf(str, "%d", m_p_slot->DOMAIN_GUN);
					g_PrintColorStr(px, py, str, gpC_base->m_chatting_pi, RGB(160, 160, 160));
					py+=16;
					
					px = g_PrintColorStr(29, py, 	(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_HEAL_LEVEL].GetString(),gpC_base->m_chatting_pi, RGB(160, 160, 160));
					wsprintf(str, "%d", m_p_slot->DOMAIN_HEAL);
					g_PrintColorStr(px, py, str, gpC_base->m_chatting_pi, RGB(160, 160, 160));
					py+=16;
					
					px = g_PrintColorStr(29, py, 	(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_ENCHANT_LEVEL].GetString(),gpC_base->m_chatting_pi, RGB(160, 160, 160));
					wsprintf(str, "%d", m_p_slot->DOMAIN_ENCHANT);
					g_PrintColorStr(px, py, str, gpC_base->m_chatting_pi, RGB(160, 160, 160));
					py+=16;
					
					std::string sstr;
					
					// 숫자사이에 ,넣기
					wsprintf(str, "%d", m_p_slot->FAME);
					sstr = str;
					for(unsigned int i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
						
						px = g_PrintColorStr(29, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_FAME].GetString(), gpC_base->m_chatting_pi, RGB(160, 160, 160));
						g_PrintColorStr(px, py, sstr.c_str(), gpC_base->m_chatting_pi, RGB(160, 160, 160));
						py+=16;
						
						if(m_p_slot->GRADE>0&&m_p_slot->GRADE<=50)
						{
							px= g_PrintColorStr(29,py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_GRADE].GetString(),gpC_base->m_chatting_pi, RGB(160,160,160));
							char tempstr[100];
							wsprintf(tempstr,"%s [%d]",grade[(m_p_slot->GRADE-1)/5],m_p_slot->GRADE);
							//g_PrintColorStr(px,py,grade[(m_p_slot->GRADE-1)/5], gpC_base->m_chatting_pi, RGB(160,160,160));
							g_PrintColorStr(px,py,tempstr, gpC_base->m_chatting_pi, RGB(160,160,160));
							py+=16;
						}
						
						// 계급 경험치 
						//px = g_PrintColorStr(29,py,(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_GRADE_EXP].GetString(),gpC_base->m_chatting_pi, RGB(160,160,160));
						//char tempstr[100];
						//wsprintf(tempstr,"%d",m_p_slot->GRADE_EXP);
						//g_PrintColorStr(px,py,tempstr,gpC_base->m_chatting_pi, RGB(160,160,160));
						
				}
				break;

			case RACE_VAMPIRE:
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
					if(m_p_slot->GRADE>0&&m_p_slot->GRADE<=50)
					{
						px= g_PrintColorStr(29,py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_GRADE].GetString(),gpC_base->m_chatting_pi, RGB(160,160,160));
						char tempstr[100];
						wsprintf(tempstr,"%s [%d]",grade[(m_p_slot->GRADE-1)/5],m_p_slot->GRADE);
						g_PrintColorStr(px,py,tempstr, gpC_base->m_chatting_pi, RGB(160,160,160));
						py+=16;
					}			
					
					// 계급 경험치 
					//px = g_PrintColorStr(29,py,(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_GRADE_EXP].GetString(),gpC_base->m_chatting_pi, RGB(160,160,160));
					//char tempstr[100];
					//wsprintf(tempstr,"%d",m_p_slot->GRADE_EXP);
					//g_PrintColorStr(px,py,tempstr,gpC_base->m_chatting_pi, RGB(160,160,160));
					//py+=16;
					
					px = g_PrintColorStr( 29, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_FAME].GetString(), gpC_base->m_chatting_pi, RGB(160,160,160) );
					char tempstr[200];
					wsprintf(tempstr, "%d", m_p_slot->FAME );
					std::string sstr = tempstr;
					for(unsigned int i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
						
						g_PrintColorStr(px,py, sstr.c_str(), gpC_base->m_chatting_pi, RGB(160,160,160) );
						py+=16;
						
				}
				break;

			case RACE_OUSTERS:
				break;
			}
			//px= g_PrintColorStr(29,py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_GRADE].GetString(),gpC_base->m_chatting_pi, RGB(160,160,160));
			//sprintf(str,"%d",m_p_slot->GRADE_EXP);
			//g_PrintColorStr(px,py,grade[m_p_slot->GRADE/5], gpC_base->m_chatting_pi, RGB(160,160,160));
			//py+=16;
			
		}
		else
		{
			g_PrintColorStr(29, 160, (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_CREATEMSG1].GetString(), gpC_base->m_chatting_pi, RGB(160, 160, 160));
			g_PrintColorStr(29, 176, (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_CREATEMSG2].GetString(), gpC_base->m_chatting_pi, RGB(160, 160, 160));
		}
		g_FL2_ReleaseDC();
	}
	m_pC_button_group->ShowDescription();

	char szBuffer[256];
		
	if (g_LeftPremiumDays == 0xFFFE )
		sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_PREMIUM_USER].GetString() );
	else if ( g_LeftPremiumDays > 0 )
	{
		if(g_bFamily)
			sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_FAMILY_DAYS].GetString(), g_LeftPremiumDays );
		else
			sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_PREMIUM_DAYS].GetString(), g_LeftPremiumDays );
	}
	else
	{
		if(g_bFamily)
			sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_EXPIRE_FAMILY_TODAY].GetString() );	
		else
			sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_EXPIRE_PREMIUM_SERVICE_TODAY].GetString() );	
	}

	if( g_LeftPremiumDays != 0xFFFF )
		g_PrintColorStr( g_vs_ui_title_only_premium_x, g_vs_ui_title_only_premium_y, szBuffer, gpC_base->m_desc_menu_pi, RGB_WHITE );
	
	SHOW_WINDOW_ATTR;
}


/////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SERVER_SELECT::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SERVER_SELECT::Start(bool bGroup)
{
	m_bl_group = bGroup;

	PI_Processor::Start();

	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();

	// change skin data
	gpC_global_resource->FreeAssemble();
	gpC_global_resource->LoadAssemble();

	m_pack_file.SetRAR(RPK_TITLE, RPK_PASSWORD);

	SetDesc(29, 160, RGB(160, 160, 160), gpC_base->m_chatting_pi);
	m_desc_y_distance = 16;

	m_scroll = 0;
	m_bl_scrolling = false;

	m_focused_help = HELP_DEFAULT;
	m_focus_server = -1;

	if(m_bl_group)
		LoadDesc("world.txt", 30, 14, false);
	else
		LoadDesc("server.txt", 30, 14, false);

	EMPTY_MOVE;

}

//-----------------------------------------------------------------------------
// Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SERVER_SELECT::Finish()
{
	PI_Processor::Finish();

	m_server_name.clear();
	m_server_id.clear();;
	m_server_status.clear();;

	gpC_window_manager->DisappearWindow(this);
}

/*-----------------------------------------------------------------------------
- C_VS_UI_SERVER_SELECT
-
-----------------------------------------------------------------------------*/
C_VS_UI_SERVER_SELECT::C_VS_UI_SERVER_SELECT()
{
	m_focus_server = -1;
	m_server_select = -1;
	m_scroll = 0;

	//서버 리스트 출력위치
	m_server_x = 338;
	m_server_y = 202;

	g_RegisterWindow(this);

	AttrKeyboardControl(true);

	// add by Sonic 2006.9.26
	//m_title1_spk.BltLocked(0,0);
	if(g_MyFull)
	{
		m_title1_spk.Open(SPK_TITLE_BACK);//뗍혤뒀暠
		m_common_spk.Open(SPK_COMMON_1024);
		m_image_spk.Open(SPK_SERVER_SELECT);
		Set(0, 0, 1024, 768);
	}
	else
	{
		m_title1_spk.Open(SPK_TITLE_BACK);//뗍혤뒀暠
		m_common_spk.Open(SPK_COMMON);
		m_image_spk.Open(SPK_SERVER_SELECT);
		Set(0, 0, 800, 600);
	}
	// end by sonic
	

//	Set(g_GameRect.right/2 - m_common_spk.GetWidth()/2, g_GameRect.bottom/2 - m_common_spk.GetHeight()/2, m_common_spk.GetWidth(), m_common_spk.GetHeight());
	

	m_pC_button_group = new ButtonGroup(this);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(28, 522, m_common_spk.GetWidth(BACK_BUTTON), m_common_spk.GetHeight(BACK_BUTTON), BACK_ID, this, BACK_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(687, 522, m_common_spk.GetWidth(NEXT_BUTTON), m_common_spk.GetHeight(NEXT_BUTTON), NEXT_ID, this, NEXT_BUTTON));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(671, 200, m_image_spk.GetWidth(SCROLL_UP_BUTTON), m_image_spk.GetHeight(SCROLL_UP_BUTTON), SCROLL_UP_ID, this, SCROLL_UP_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(671, 420, m_image_spk.GetWidth(SCROLL_DOWN_BUTTON), m_image_spk.GetHeight(SCROLL_DOWN_BUTTON), SCROLL_DOWN_ID, this, SCROLL_DOWN_BUTTON));

	m_focused_help = HELP_DEFAULT;
}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_SERVER_SELECT
-
-----------------------------------------------------------------------------*/
C_VS_UI_SERVER_SELECT::~C_VS_UI_SERVER_SELECT()
{
	g_UnregisterWindow(this);

	// !Window를 먿저 delete해야 한다.
	DeleteNew(m_pC_button_group);
}


//-----------------------------------------------------------------------------
// AcquireFirstSequence
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SERVER_SELECT::AcquireFirstSequence()
{
}

//-----------------------------------------------------------------------------
// UnacquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SERVER_SELECT::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SERVER_SELECT::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_SERVER_SELECT::IsPixel(int _x, int _y)
{
//	return m_image_spk.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
	return true;
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SERVER_SELECT::Run(id_t id)
{
	switch (id)
	{
	case BACK_ID:
		gpC_base->SendMessage(UI_CLOSE_SERVER_SELECT, m_bl_group);
		break;

	case NEXT_ID:
	{
		bool validIndex = m_server_select > -1 && m_server_select < (int)m_server_name.size();
		bool validWorld = m_bl_group && m_server_status[m_server_select] != STATUS_CLOSED;
		bool validServer = !m_bl_group && m_server_status[m_server_select] != STATUS_DOWN;
			
		if( validIndex && (validServer || validWorld))
			gpC_base->SendMessage(UI_CONNECT_SERVER, m_bl_group, m_server_id[m_server_select]);
	}
	break;

	case SCROLL_UP_ID:
		if(m_scroll > 0)
			m_scroll--;
		EMPTY_MOVE;
		break;

	case SCROLL_DOWN_ID:
		if(m_server_name.size() > 12 && m_scroll < m_server_name.size()-12)
			m_scroll++;
		EMPTY_MOVE;
		break;
	}
}

/*-----------------------------------------------------------------------------
- MouseControl
-
-----------------------------------------------------------------------------*/
bool C_VS_UI_SERVER_SELECT::MouseControl(UINT message, int _x, int _y)
{
	_x-=x;_y-=y;
	Window::MouseControl(message, _x, _y);

	m_pC_button_group->MouseControl(message, _x, _y);

//	static char server_status_string[50];

	switch (message)
	{
		case M_MOVING:
			m_focused_help = HELP_DEFAULT;
			m_focus_server = -1;
			if(_x > m_server_x && _x < x+m_server_x+328 && _y > y+m_server_y-2 && _y < y+m_server_y+20*12)
			{
				m_focus_server = (_y - m_server_y)/20+m_scroll;
				if(m_focus_server < 0 || m_focus_server >= (int)m_server_name.size())
					m_focus_server = -1;
			}
			if(m_bl_scrolling)
			{
				if(m_server_name.size() > 12)
				{
					m_bl_scrolling = true;
					m_scroll = (m_server_name.size()-12)*(min(SCROLL_HEIGHT, max(0, (_y - SCROLL_Y))) + SCROLL_HEIGHT/(m_server_name.size()-12)/2)/SCROLL_HEIGHT;
				}
			}
			break;

		case M_LB_DOUBLECLICK:
			if(m_focus_server != -1)
			{
				m_server_select = m_focus_server;
				Run(NEXT_ID);
			}
			break;

		case M_LEFTBUTTON_DOWN:
			if(m_focus_server != -1)
				m_server_select = m_focus_server;
			if(_x >= SCROLL_X && _x <= SCROLL_X+m_image_spk.GetWidth(SCROLL_TAG) && _y >= SCROLL_Y && _y <= SCROLL_Y+SCROLL_HEIGHT+m_image_spk.GetHeight(SCROLL_TAG))
				if(m_server_name.size() > 12)
				{
					m_bl_scrolling = true;
					m_scroll = (m_server_name.size()-12)*(_y - SCROLL_Y + SCROLL_HEIGHT/(m_server_name.size()-12)/2)/SCROLL_HEIGHT;
				}
			break;

		case M_LEFTBUTTON_UP:
			m_bl_scrolling = false;
			break;

		case M_RIGHTBUTTON_DOWN:
			break;

		case M_RIGHTBUTTON_UP:
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

/*-----------------------------------------------------------------------------
- KeyboardControl
-
-----------------------------------------------------------------------------*/
void C_VS_UI_SERVER_SELECT::KeyboardControl(UINT message, UINT key, long extra)
{
	switch(message)
	{
	case WM_KEYDOWN:
		switch(key)
		{
		case VK_ESCAPE:
			Run(BACK_ID);
			break;

		case VK_RETURN:
			Run(NEXT_ID);
			break;

		case VK_UP:
			if(m_server_select > 0)
				m_server_select--;
			break;

		case VK_DOWN:
			if(m_server_select < (int)m_server_name.size()-1)
				m_server_select++;
			break;
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_SERVER_SELECT::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
//	int index = p_button->m_image_index;
	
	if(p_button->GetID() == BACK_ID || p_button->GetID() == NEXT_ID)
	{
		if(p_button->GetFocusState())
		{
			if (p_button->GetPressState()) // push state
				m_common_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				m_common_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
			if(p_button->GetID() == BACK_ID)
				m_focused_help = HELP_BACK;
			else
				m_focused_help = HELP_NEXT;
		}
		else
			m_common_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
	}
	else
		if (p_button->GetFocusState())
		{
			if(p_button->GetPressState()) // push state
				m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
		}
		else
			m_image_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
}

///////////////////////////////////////////////////////////////////////////////
// SetServerDefault
//
///////////////////////////////////////////////////////////////////////////////
void C_VS_UI_SERVER_SELECT::SetServerList(LPSTR *name, int *id, int *status, int size, int default_id)
{
	m_server_name.clear();
	m_server_id.clear();
	m_server_status.clear();

	for(int i = 0; i < size; i++)
	{
		m_server_name.push_back(name[i]);
		m_server_id.push_back(id[i]);
		m_server_status.push_back(status[i]);
		if(id[i] == default_id)
		{
			m_server_select = i;
			if(i >= 12)
				m_scroll = i - 12;
		}
	}

}

/*-----------------------------------------------------------------------------
- Show
-
-----------------------------------------------------------------------------*/
void C_VS_UI_SERVER_SELECT::Show()
{
	gpC_base->m_p_DDSurface_back->FillSurface(0);

	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_title1_spk.BltLocked(0,0);
		m_common_spk.BltLocked(x, y);
		m_image_spk.BltLocked(300, 150);
		
		m_pC_button_group->Show();
		
		if(m_bl_group)
		{
			m_image_spk.BltLocked(TITLE_X - m_image_spk.GetWidth(TITLE_WORLD)/2, TITLE_Y, TITLE_WORLD);
		}
		else
		{
			m_image_spk.BltLocked(TITLE_X - m_image_spk.GetWidth(TITLE_SERVER)/2, TITLE_Y, TITLE_SERVER);
		}
		//	int i;
		
		if(m_server_name.size() > 12)	// scroll tag 표시
		{
			if(m_bl_scrolling)
				m_image_spk.BltLocked(SCROLL_X, min(max(gpC_mouse_pointer->GetY(), SCROLL_Y+m_image_spk.GetHeight(SCROLL_TAG)/2), SCROLL_Y+SCROLL_HEIGHT+m_image_spk.GetHeight(SCROLL_TAG)/2)-m_image_spk.GetHeight(SCROLL_TAG)/2, SCROLL_TAG_HILIGHTED);
			else
				m_image_spk.BltLocked(SCROLL_X, SCROLL_Y+m_scroll*SCROLL_HEIGHT/(m_server_name.size()-12), SCROLL_TAG);
		}
		
		if(m_server_select >= m_scroll && m_server_select < m_scroll+12)
		{
			RECT rect = {x+m_server_x, y+m_server_y+(m_server_select-m_scroll)*20-2, x+m_server_x+327, y+m_server_y+(m_server_select-m_scroll)*20+20-3};
			DrawAlphaBox(&rect, 0, 0, 255, 20);
		}

		gpC_base->m_p_DDSurface_back->Unlock();
	}

	char server_status_string[30];
	//서버리스트 표시
	g_FL2_GetDC();
	for(int i = 0; i < min(m_server_name.size(), 12); i++)
	{
		COLORREF statusColor = RGB_WHITE;
		if(m_bl_group)
		{
			switch(m_server_status[i+m_scroll])
			{
			case STATUS_OPENED:
				strcpy(server_status_string,(*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_OPEN].GetString());
				break;

			case STATUS_CLOSED:
				statusColor = RGB_RED;
				strcpy(server_status_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_CLOSE].GetString());
				break;
			}
		}
		else
		{
			switch(m_server_status[i+m_scroll])
			{
			case STATUS_VERY_GOOD:
				strcpy(server_status_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_VERY_GOOD].GetString());
				break;
				
			case STATUS_GOOD:
				strcpy(server_status_string,(*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_GOOD].GetString());
				break;
				
			case STATUS_NORMAL:
				strcpy(server_status_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_NORMAL].GetString());
				break;
				
			case STATUS_BAD:
				strcpy(server_status_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_BAD].GetString());
				break;
				
			case STATUS_VERY_BAD:
				strcpy(server_status_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_VERY_BAD].GetString());
				break;

			case STATUS_DOWN:
				statusColor = RGB_RED;
				strcpy(server_status_string, (*g_pGameStringTable)[UI_STRING_MESSAGE_SERVER_STATUS_CLOSE].GetString());
				break;
			}
		}
		if(i+m_scroll == m_focus_server)
			g_PrintColorStr(x+m_server_x+5, y+m_server_y+i*20, m_server_name[i+m_scroll].c_str(), gpC_base->m_desc_menu_pi, RGB_YELLOW);
		else
			g_PrintColorStr(x+m_server_x+5, y+m_server_y+i*20, m_server_name[i+m_scroll].c_str(), gpC_base->m_desc_menu_pi, RGB_WHITE);

		g_PrintColorStr(x+m_server_x+150, y+m_server_y+i*20, server_status_string, gpC_base->m_desc_menu_pi, statusColor);
	}
	
	char szBuffer[256];

	
	if (g_LeftPremiumDays == 0xFFFE )
		sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_NOT_PREMIUM_USER].GetString() );
	else if ( g_LeftPremiumDays > 0 )
	{
		if(g_bFamily)
			sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_FAMILY_DAYS].GetString(), g_LeftPremiumDays );
		else
			sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_PREMIUM_DAYS].GetString(), g_LeftPremiumDays );
	}
	else
	{
		if(g_bFamily)
			sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_EXPIRE_FAMILY_TODAY].GetString() );	
		else
			sprintf( szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_EXPIRE_PREMIUM_SERVICE_TODAY].GetString() );	
	}

	if( g_LeftPremiumDays != 0xFFFF )
		g_PrintColorStr( g_vs_ui_title_only_premium_x, g_vs_ui_title_only_premium_y, szBuffer, gpC_base->m_desc_menu_pi, RGB_WHITE );
			
	g_FL2_ReleaseDC();
	std::string help_string[HELP_MAX] = 
	{
		"",
		"back.txt",
		"next.txt",
	};

	static bool old_bl_group = m_bl_group;
	static int old_focused_help = -1;
	if(old_focused_help != m_focused_help && m_focused_help < HELP_MAX || old_bl_group != m_bl_group)
	{
		if(m_focused_help == HELP_DEFAULT)
		{
			if(m_bl_group)
				LoadDesc("world.txt", 30, 14, false);
			else
				LoadDesc("server.txt", 30, 14, false);
		}
		else
			LoadDesc(help_string[m_focused_help].c_str(), 30, 14, false);
		old_focused_help = m_focused_help;
		old_bl_group = m_bl_group;
	}
	ShowDesc();

	SHOW_WINDOW_ATTR;
}

/*-----------------------------------------------------------------------------
- C_VS_UI_LOGIN
-
-----------------------------------------------------------------------------*/
C_VS_UI_LOGIN::C_VS_UI_LOGIN()
{
	g_RegisterWindow(this);

	AttrTopmost(true);
	AttrKeyboardControl(true);
	// add by Sonic 2006.9.26
	if(g_MyFull)
		m_pC_login_spk = new C_SPRITE_PACK(SPK_LOGIN_1024);
	else
		m_pC_login_spk = new C_SPRITE_PACK(SPK_LOGIN);

//	m_pC_login_menu_default.Open(SPK_LOGIN_MENU_DEFAULT);
	m_pC_login_menu.Open(SPK_LOGIN_MENU);
	if(g_MyFull)
	{
		Set(/*154, 180*/401, 293, m_pC_login_spk->GetWidth(), m_pC_login_spk->GetHeight());
	}
	else
	{
		Set(/*154, 180*/400 - m_pC_login_spk->GetWidth()/2, 300 - m_pC_login_spk->GetHeight()/2-57, m_pC_login_spk->GetWidth(), m_pC_login_spk->GetHeight());
	}

	m_pC_button_group = new ButtonGroup(this);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(118, 123, m_pC_login_menu.GetWidth(NEW_ID), m_pC_login_menu.GetHeight(NEW_ID), NEW_ID, this, PUSHED_NEW_ID, 1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(52, 123, m_pC_login_menu.GetWidth(OK), m_pC_login_menu.GetHeight(OK), OK, this, PUSHED_OK, 1));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(156, 28, m_pC_login_menu.GetWidth(CANCEL), m_pC_login_menu.GetHeight(CANCEL), CANCEL, this, PUSHED_CANCEL, 1));


	// LineEditorVisual setting...
	m_lev_id.SetPosition(x+LOGIN_ID_X, y+LOGIN_ID_Y);
	/******** Edit By Sonic 2006.9.25 錦맣쵱쯤渴흙낀똑槨13貫********/
	//m_lev_id.SetByteLimit(10);
	m_lev_id.SetByteLimit(13);
	/******** End By Sonic 2006.9.25 ********/
	m_lev_password.SetPosition(x+LOGIN_PASSWORD_X, y+LOGIN_PASSWORD_Y);
	m_lev_password.PasswordMode(true);
	m_lev_password.SetByteLimit(10);

	Attach(&m_lev_id);
	Attach(&m_lev_password);

//	m_bFirst = true;
}

//-----------------------------------------------------------------------------
// ReadySend
//
// 비어있지않고 입력이 다 되었으면 true를 반환하고 그렇지 않으면 false를 반환한다.
//-----------------------------------------------------------------------------
bool C_VS_UI_LOGIN::ReadySend()
{
	if (m_lev_id.Size() > 0 && m_lev_password.Size() > 0)
		return true;
	
	return false;

	/*if (gC_font.GetStringBuffer()->str.length() == 0)
		return false;

	if (m_e_ip == ID)
	{
		if (m_string_line_PASSWORD.length() == 0)
			return false;
	}
	else
	{
		if (m_string_line_ID.length() == 0)
			return false;
	}

	return true;*/
}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_LOGIN
-
-----------------------------------------------------------------------------*/
C_VS_UI_LOGIN::~C_VS_UI_LOGIN()
{
	g_UnregisterWindow(this);

	DeleteNew(m_pC_button_group);
	DeleteNew(m_pC_login_spk);
}

//-----------------------------------------------------------------------------
// AcquireFirstSequence
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_LOGIN::AcquireFirstSequence()
{
	
}

//-----------------------------------------------------------------------------
// ChangeFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_LOGIN::ChangeFocus()
{
	if (m_lev_id.IsAcquire())
	{
		if(m_lev_id.Size() == 0)
			m_lev_id.AddString(m_lev_id_backup.c_str());
		m_lev_password.Acquire();
	}
	else
	{
		char * p_temp = NULL;
		g_Convert_DBCS_Ascii2SingleByte(m_lev_id.GetString(), m_lev_id.Size(), p_temp);
		if(p_temp == NULL)
		{
			m_lev_id_backup = "";
		}
		else
		{
			m_lev_id_backup = p_temp;
			DeleteNewArray(p_temp);
		}
		m_lev_id.Acquire();
		m_lev_id.EraseAll();
	}
}

void C_VS_UI_LOGIN::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

void C_VS_UI_LOGIN::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
}

void C_VS_UI_LOGIN::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if (p_button->GetFocusState() && p_button->GetPressState())
	{
		m_pC_login_menu.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
	}
//	m_pC_login_menu_default.Blt(p_button->x, y, p_button->GetID());

	else if (p_button->m_alpha)
		m_pC_login_menu.BltLockedAlpha(x+p_button->x, y+p_button->y, p_button->GetID(), p_button->m_alpha);
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_LOGIN::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_LOGIN::IsPixel(int _x, int _y)
{
	return m_pC_login_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_LOGIN::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_LOGIN::Start()
{
	PI_Processor::Start();

	AttrKeyboardControl(true);

	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();

	m_lev_id.Acquire();
	m_lev_id.EraseAll();
	m_lev_password.EraseAll();

	if(strlen(g_pUserOption->BackupID) > 0)
	{
		m_lev_id.AddString(g_pUserOption->BackupID);
		ChangeFocus();
	}

	// prev-set
//	ifstream file(FILE_BACKUP_ID, ios::binary);
//	if (file)
//	{
//		file.seekg(0, ios::end); // to get file size
//		int size = file.tellg();
//		file.seekg(0);
//
//		if (size > 0 && size <= 10)
//		{
//			char sz_id[11];
//			memset(sz_id, 0, 11);	
//			file.read(sz_id, size);
//
//			m_lev_id.AddString(sz_id);
//			ChangeFocus();
//		}
//	}

//	file.close();

//	m_bFirst = true;

}

//-----------------------------------------------------------------------------
// Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_LOGIN::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_LOGIN::Run(id_t id)
{
	switch (id)
	{
		case OK:
			if (ReadySend() == true)
			{
				SendLoginToClient();
			}
			else
			{
				g_msg_wrong_id_password->Start();
			}
			break;

		case CANCEL:
			Finish();
			break;

		case NEW_ID:
			//g_msg_not_available_menu->Start();
			Finish();
			gpC_base->SendMessage(UI_RUN_NEWUSER_REGISTRATION);
			break;
	}
}

/*-----------------------------------------------------------------------------
- MouseControl
-
-----------------------------------------------------------------------------*/
bool C_VS_UI_LOGIN::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;

	m_pC_button_group->MouseControl(message, _x, _y);

	switch (message)
	{
		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			{
				Rect id_rt(LOGIN_ID_X, LOGIN_ID_Y, 130, 23);
				Rect pass_rt(LOGIN_PASSWORD_X, LOGIN_PASSWORD_Y, 130, 23);
				if (id_rt.IsInRect(_x, _y))
				{
					if (!m_lev_id.IsAcquire())
						ChangeFocus();
				}
				else if (pass_rt.IsInRect(_x, _y))
				{
					if (!m_lev_password.IsAcquire())
						ChangeFocus();
				}
			}
	}

	return true;
}

//-----------------------------------------------------------------------------
// NextFocus
//
// ID or Password 둘 중 어느 하나가 입력이 전혀 없었다면, 다음의 그곳으로 이동한다.
//
// `비어있는 곳이 없으면 true를 반환한다.
//-----------------------------------------------------------------------------
/*
bool C_VS_UI_LOGIN::NextFocus()
{
	// gC_font 내부에서 tab은 아무것도 아니다.
	gC_font.ForceShowCursor();

	if (m_e_ip == ID)
	{
		m_e_ip = PASSWORD;
		
		m_string_line_ID.erase();
		m_string_line_ID = gC_font.GetStringBuffer()->str;

		gC_font.SetInputLine(m_string_line_PASSWORD.c_str());
	}
	else
	{
		m_e_ip = ID;

		m_string_line_PASSWORD.erase();
		m_string_line_PASSWORD = gC_font.GetStringBuffer()->str;

		gC_font.SetInputLine(m_string_line_ID.c_str());
	}

	// no empty? - 마지막에 검사한다.
	if (m_string_line_ID.length() > 0 && m_string_line_PASSWORD.length() > 0)
		return true;

	return false;
}*/

//-----------------------------------------------------------------------------
// SendLoginToClient
//
// ID/Password를 Client로 보낸다.
//-----------------------------------------------------------------------------
void C_VS_UI_LOGIN::SendLoginToClient()
{
	// copy from input line.
/*	if (m_e_ip == ID)
	{
		m_string_line_ID.erase();
		m_string_line_ID += gC_font.GetStringBuffer()->str;
	}
	else
	{
		m_string_line_PASSWORD.erase();
		m_string_line_PASSWORD += gC_font.GetStringBuffer()->str;
	}
*/
	//
	// 반드시 static으로 하고 member를 login check가 끝나고 delete 해준다.
	//
	static LOGIN S_login; 
	//S_login.sz_id = (char *)m_string_line_ID.c_str();
	//S_login.sz_password = (char *)m_string_line_PASSWORD.c_str();
	g_Convert_DBCS_Ascii2SingleByte(m_lev_id.GetString(), m_lev_id.Size(), S_login.sz_id);
	g_Convert_DBCS_Ascii2SingleByte(m_lev_password.GetString(), m_lev_password.Size(), S_login.sz_password);
	strcpy(g_pUserOption->BackupID, S_login.sz_id);

	gpC_base->SendMessage(UI_LOGIN, 0, 0, &S_login);
}

/*-----------------------------------------------------------------------------
- KeyboardControl
- Log-In ID와 Password를 입력받는다.
-----------------------------------------------------------------------------*/
void C_VS_UI_LOGIN::KeyboardControl(UINT message, UINT key, long extra)
{

	//bool focus_end = false;
	//if (message == WM_KEYDOWN && key == VK_RETURN)
	//	focus_end = NextFocus();

	//gpC_base->SelectFont(FONT_SLAYER);

	//if (key != VK_RETURN)
	//	gC_font.KeyInput(message, key, extra);

//	if(message == WM_CHAR && m_bFirst)
//	{
//		m_bFirst = false;
//		return;
//	}


	Window::KeyboardControl(message, key, extra);

	switch (message)
	{
		case WM_KEYDOWN:
			// 
			// input position 변경.
			//
			if (key == VK_TAB)
			{
				//NextFocus();
				ChangeFocus();
			}
			else if (key == VK_ESCAPE) // cancel!
			{
				Finish();
			}
			else if (key == VK_RETURN) // ok
			{
				//if (focus_end) // 비어 있는 곳이 없으면 send!
				//{

				if (ReadySend() == true)
					SendLoginToClient();
				else
					//NextFocus();
					ChangeFocus();
				//}
			}
			break;
	}
}

extern HWND g_hWnd;
/*-----------------------------------------------------------------------------
- Show
-
-----------------------------------------------------------------------------*/
void C_VS_UI_LOGIN::Show()
{
//	if(m_lev_password.IsAcquire())
//	{
//		DWORD conversion, sentence;
//		ImmGetConversionStatus(ImmGetContext(g_hWnd), &conversion, &sentence);
//		if(conversion)
//		{
//			conversion = 0;
//			ImmSetConversionStatus(ImmGetContext(g_hWnd), conversion, sentence);
//		}
//	}

//	m_pC_login_spk->BltAlpha(x, y+20, SHADOW, 22);
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_login_spk->BltLocked(x, y);		
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
/*
	// login 문자열 출력.
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO	surfaceinfo;
		SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());

		gpC_base->SelectFont(FONT_SLAYER);

		// cursor는 현재 입력 line에만 있게 해야 한다.
		if (m_e_ip == ID)
			gC_font.Update(&surfaceinfo, x+LOGIN_ID_X, y+LOGIN_ID_Y, WHITE);
		else
			gC_font.PrintStringNoConvert(&surfaceinfo, m_string_line_ID.c_str(), x+LOGIN_ID_X, y+LOGIN_ID_Y, WHITE);

		gC_font.PasswordMode();
		if (m_e_ip == PASSWORD)
			gC_font.Update(&surfaceinfo, x+LOGIN_PASSWORD_X, y+LOGIN_PASSWORD_Y, WHITE);
		else
			gC_font.PrintStringNoConvert(&surfaceinfo, m_string_line_PASSWORD.c_str(), x+LOGIN_PASSWORD_X, y+LOGIN_PASSWORD_Y, WHITE);
		gC_font.NoPasswordMode();

		gpC_base->m_p_DDSurface_back->Unlock();
	}*/

	Window::ShowWidget();

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// NewCharacterCreateOk
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::NewCharacterCreateOk()
{
	m_pC_char_manager->NewCharacterCreateOk();
}

//-----------------------------------------------------------------------------
// StartCharacterManager
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::StartCharacterManager(bool back)
{
	Finish();
	m_pC_server_select->Finish();

	m_pC_char_manager->Start(back);
}

//-----------------------------------------------------------------------------
// StartCharacterManager
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::StartServerSelect(bool bGroup)
{
	Finish();

	m_pC_login->Finish();

	m_pC_char_manager->Finish();

	m_pC_server_select->Finish();

	m_pC_server_select->Start(bGroup);
}


//-----------------------------------------------------------------------------
// DeleteCharacter
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::DeleteCharacter(int slot)
{
	m_pC_char_manager->DeleteCharacter(slot);
}

//-----------------------------------------------------------------------------
// ClearAllCharacter
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::ClearAllCharacter()
{
	m_pC_char_manager->ClearAllCharacter();
}

void A(C_VS_UI_DIALOG * p, id_t id)
{
	int z = 0;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TITLE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_TITLE::C_VS_UI_TITLE()
{
	g_RegisterWindow(this);

#ifndef _LIB
/*	m_pC_dialog = new C_VS_UI_DIALOG(50, 20, 6, 2, A);//, DIALOG_OK);//|DIALOG_CANCEL);

	DIALOG_MENU d_menu[] = {
		{"사기 & asldkfsldjald", 0},
		{"팔기 / 26387638127638263", 1},
		{"끝내기", DIALOG_EXECID_EXIT},
	};
	m_pC_dialog->SetMenu(d_menu, 3);

	static char * pp_dmsg[] = {
		"안녕하세요? 저는 진태라고합니다.",
		"네? 아, 참 좋은 이름이군요^^헤..",
		"m_pC_newuser = new C_VS_UI_NEWUSER;",
		" ",
		"m_title_spk.Open(SPK_TITLE);",
		"m_title_menu_default.Open(SPK_TITLE_MENU_DEFAULT);",
		"m_title_menu_select.Open(SPK_TITLE_MENU_SELECT);",
		" ",
		"// set Window size",
		"Set(0, 0, m_title_spk.GetWidth(), m_title_spk.GetHeight());",
		" ",
		"// ani objects",
		"m_pC_ao_title = new C_ANI_OBJECT(SPK_ANI_TITLE, FRR_ANI_TITLE);",
		"m_pC_ao_symbol = new C_ANI_OBJECT(SPK_ANI_SYMBOL, FRR_ANI_SYMBOL);",
		"m_pC_ani_title = new C_ANIMATION(m_pC_ao_title);",
		"m_pC_ani_symbol = new C_ANIMATION(m_pC_ao_symbol);",
		" ",
		"m_pC_ani_title->SetPlayPosition(282, 100);",
		"m_pC_ani_title->SetSpeed(120);",
		"m_pC_ani_symbol->SetPlayPosition(282, 186);",
		"m_pC_ani_symbol->SetSpeed(70);",
	};
*/	//m_pC_dialog->SetMessage(pp_dmsg, sizeof(pp_dmsg)/sizeof(char *));//, SMO_NOFIT);
#endif

	AttrKeyboardControl(true);
//	gC_ci.SetEngInput();

	m_pC_char_manager = new C_VS_UI_CHAR_MANAGER;
	m_pC_server_select = new C_VS_UI_SERVER_SELECT;

	m_pC_login = new C_VS_UI_LOGIN;

//	m_pC_newuser = new C_VS_UI_NEWUSER;

//	gbl_option_running = false;	
//	m_pC_option = NULL;
// add by Sonic 2006.9.26털뙤角뤠槨1024친駕
	if(g_MyFull)
		m_title_spk.Open(SPK_TITLE_1024);
	else
		m_title_spk.Open(SPK_TITLE);
// end by sonic
	m_title_menu_default.Open(SPK_TITLE_MENU_DEFAULT);
	m_title_menu_select.Open(SPK_TITLE_MENU_SELECT);

	m_pC_title_ani.Open(SPK_ANI_TITLE);
//	m_pC_symbol_ani.Open(SPK_ANI_SYMBOL);
	// set Window size
	Set(0, 0, m_title_spk.GetWidth(), m_title_spk.GetHeight());

  	// ani objects
//	m_pC_ao_title = new C_ANI_OBJECT(SPK_ANI_TITLE, FRR_ANI_TITLE);
//	m_pC_ao_symbol = new C_ANI_OBJECT(SPK_ANI_SYMBOL, FRR_ANI_SYMBOL);
//	m_pC_ani_title = new C_ANIMATION(m_pC_ao_title);
//	m_pC_ani_symbol = new C_ANIMATION(m_pC_ao_symbol);

//	m_pC_ani_title->SetPlayPosition(263, 93);
//	m_pC_ani_title->SetSpeed(120);
//	m_pC_ani_symbol->SetPlayPosition(282, 186);
//	m_pC_ani_symbol->SetSpeed(70);
	
	m_pC_use_grade = NULL;
	
	const InterfaceInformation *pSkin = &g_pSkinManager->Get( SkinManager::TITLE );
	m_pC_button_group = new ButtonGroup(this);
	if(g_MyFull)
	{
		//되쩍썹충객큐鞫刻
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+888, y+544, m_title_menu_default.GetWidth(CONNECT_HILIGHT), m_title_menu_default.GetHeight(CONNECT_HILIGHT), CONNECT, this,CONNECT_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+888, y+592, m_title_menu_default.GetWidth(OPTION_HILIGHT), m_title_menu_default.GetHeight(OPTION_HILIGHT), OPTION, this,OPTION_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+888, y+640, m_title_menu_default.GetWidth(CREDIT_HILIGHT), m_title_menu_default.GetHeight(CREDIT_HILIGHT), CREDIT, this,CREDIT_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+888, y+688, m_title_menu_default.GetWidth(EXIT_HILIGHT), m_title_menu_default.GetHeight(EXIT_HILIGHT), EXIT, this,EXIT_HILIGHT));
	}
	else
	{
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+673, y+371, m_title_menu_default.GetWidth(CONNECT_HILIGHT), m_title_menu_default.GetHeight(CONNECT_HILIGHT), CONNECT, this,CONNECT_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+673, y+419, m_title_menu_default.GetWidth(OPTION_HILIGHT), m_title_menu_default.GetHeight(OPTION_HILIGHT), OPTION, this,OPTION_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+673, y+467, m_title_menu_default.GetWidth(CREDIT_HILIGHT), m_title_menu_default.GetHeight(CREDIT_HILIGHT), CREDIT, this,CREDIT_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+673, y+515, m_title_menu_default.GetWidth(EXIT_HILIGHT), m_title_menu_default.GetHeight(EXIT_HILIGHT), EXIT, this,EXIT_HILIGHT));
	}

	//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(324+17, 278, m_title_menu_default.GetWidth(TUTORIAL), m_title_menu_default.GetHeight(TUTORIAL), TUTORIAL, this));
//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(347+17, 319, m_title_menu_default.GetWidth(INTRO), m_title_menu_default.GetHeight(INTRO), INTRO, this));

	m_pC_credit = NULL;

	m_dw_prev_tickcount = GetTickCount();
	m_dw_millisec = 30;
	m_credit_scroll = 0;

}

//-----------------------------------------------------------------------------
// ~C_VS_UI_TITLE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_TITLE::~C_VS_UI_TITLE()
{
	g_UnregisterWindow(this);

#ifndef _LIB
//	DeleteNew(m_pC_dialog);
#endif

	//
	// !먼저 child Window들을 delete해야 한다.
	//
	// 그렇게 하지 않으면 child Window이 제거될 때는 이미 C_VS_UI_TITLE::IsPIxel()을
	// 할 수 없는 상태(m_title_spk가 delete된 이후)이기 때문이다.
	//
	DeleteNew(m_pC_login);
	DeleteNew(m_pC_char_manager);
	DeleteNew(m_pC_server_select);

//	DeleteNew(m_pC_newuser);
//	DeleteNew(m_pC_option);

	DeleteNew(m_pC_button_group);

	DeleteNew(m_pC_credit);
	DeleteNew(m_pC_use_grade);
}

//-----------------------------------------------------------------------------
// C_VS_UI_TITLE::CloseUserRegistrationWindow
//
// 
//-----------------------------------------------------------------------------
//void	C_VS_UI_TITLE::CloseUserRegistrationWindow()
//{
//	m_pC_newuser->Finish();
//}

//-----------------------------------------------------------------------------
// C_VS_UI_TITLE::RunNewUserRegistration
//
// 
//-----------------------------------------------------------------------------
//void	C_VS_UI_TITLE::RunNewUserRegistration()
//{
//	assert(m_pC_newuser != NULL);
//
//	m_pC_newuser->Start();
//}
/*
//-----------------------------------------------------------------------------
// RunTitleOption
//
// 교환할거냐고 묻는다.
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::RunOption()
{
	//gC_vs_ui.CloseTitle();

	if (m_pC_option!= NULL)
	{
		DeleteNew(m_pC_option);
	}

	// center
	m_pC_option = new C_VS_UI_OPTION;

	assert(m_pC_option != NULL);

//	gbl_title_option_running = true;

	m_pC_option->Start();	
}

//-----------------------------------------------------------------------------
// CloseTitleOption
//
// 이미 finish된 상태이면 아무것도 하지 않는다.
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::CloseOption()
{
	if (!m_pC_option)
		return;

	DeleteNew(m_pC_option);
	
	//assert(m_pC_tribe_interface != NULL);
	//m_pC_tribe_interface->DoCommonActionAfterEventOccured();	

	//gbl_title_option_running = false;
	//gC_vs_ui.AcquireChatting();
}
*/
//-----------------------------------------------------------------------------
// C_VS_UI_TITLE::CloseCharInfo
//
// 
//-----------------------------------------------------------------------------
//void C_VS_UI_TITLE::CloseCharInfo()
//{
//	if (m_pC_char_manager)
//		m_pC_char_manager->CloseCharInfo();
//}


//-----------------------------------------------------------------------------
// AcquireFirstSequence
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::AcquireFirstSequence()
{
}

//-----------------------------------------------------------------------------
// NewCharacterCreateFailed
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::NewCharacterCreateFailed(int error)
{
	if (m_pC_char_manager)
		m_pC_char_manager->NewCharacterCreateFailed(error);
}

void C_VS_UI_TITLE::SetCharacter(int slot, S_SLOT &S_slot)
{
	if (m_pC_char_manager)
		m_pC_char_manager->SetCharacter(slot, S_slot);
}

void C_VS_UI_TITLE::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

void C_VS_UI_TITLE::AcquireMouseFocus()
{

}

void C_VS_UI_TITLE::RunCannotPlayMessage()
{
	if(m_pC_char_manager)
		m_pC_char_manager->Run(C_VS_UI_CHAR_MANAGER::CANNOT_PLAY_ID);	
}

void C_VS_UI_TITLE::RunCannotPlayByAttrMessage()
{
	if(m_pC_char_manager)
		m_pC_char_manager->Run(C_VS_UI_CHAR_MANAGER::CANNOT_PLAY_BY_ATTR_ID);	
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_TITLE::IsPixel(int _x, int _y)
{
	return m_title_spk.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// Start
//
// Title을 재시작한다. 그래서 Title 하위메뉴를 모두 종료해야 한다.
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::Start()
{
	m_bl_credit = false;
	gbl_wood_skin = true;
//	gbl_vampire_interface = false;

	// change skin data
	gpC_global_resource->FreeAssemble();
	gpC_global_resource->LoadAssemble();

	PI_Processor::Start();

	m_pC_char_manager->Finish();
	m_pC_login->Finish();
	m_pC_server_select->Finish();
//	m_pC_newuser->Finish();

	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();

	g_descriptor_manager.Unset();

	g_eRaceInterface = RACE_SLAYER;

#ifndef _LIB
	//m_pC_dialog->Start();
#endif
}

//-----------------------------------------------------------------------------
// Finish
//
// Title/Login/Characters ... 모든 Window가 독립된 것이므로 어느 하나가 Finish되어서
// 다른 것도 Finish되면 안된다.
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetPressState())
		m_title_menu_default.BltLocked(p_button->x, p_button->y, (p_button->m_image_index)+1);
	else if (p_button->GetFocusState())
		m_title_menu_default.BltLocked(p_button->x, p_button->y, (p_button->m_image_index));
	
//	if (p_button->GetFocusState() && p_button->GetPressState())
//	{
//		m_title_menu_select.BltLocked(p_button->x, p_button->y, p_button->GetID());
//	}
//	else
//	{
//		m_title_menu_default.BltLocked(p_button->x, p_button->y, p_button->GetID());
//		if (p_button->m_alpha)
//			m_title_menu_select.BltLockedAlpha(p_button->x, p_button->y, p_button->GetID(), p_button->m_alpha);
//	}
//	

/*
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		S_SURFACEINFO	surfaceinfo;
		SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());

		//if (p_button->GetFocusState())
		//{
		//	rectangle(&surfaceinfo, p_button, BLUE);

		//	if (p_button->GetPressState())
		//		FillRect(&surfaceinfo, p_button, BLUE);
		//}
		//else
		//	rectangle(&surfaceinfo, p_button, WHITE);

		char buf[100];
		sprintf(buf, "alpha = %d", p_button->m_alpha);
		gpC_base->SelectFont(FONT_NORMAL);
		gC_font.PrintString(&surfaceinfo, buf,
										  10, 
										  10, WHITE);

		gpC_base->m_p_DDSurface_back->Unlock();
	}	*/
}

//-----------------------------------------------------------------------------
// Show
//
// [참고] 만약 descriptor를 Title에서 사용하려면, 그것의 Show()를 여기서 실행한다.
//			 이것은 Game class에도 있다. C_VS_UI::Show()에다 놓으면 안된다.
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::Show()
{
//	if (gpC_window_manager->GetShowState(m_pC_login))
//		m_title_spk.Blt(0, 0, 1);
//	else


	// message
//	if (C_VS_UI::m_sz_version)
//		gpC_base->Print(5, 5, C_VS_UI::m_sz_version, gpC_base->GREEN);


	Timer();
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		if (m_bl_credit)
		{
			assert(m_pC_credit != NULL);
			
			const int scroll_x = 100, scroll_y = 170;
			
			//gpC_base->m_p_DDSurface_back->FillSurface(0);
			m_pC_credit->BltLocked(g_GameRect.right/2-m_pC_credit->GetWidth()/2, g_GameRect.bottom/2-m_pC_credit->GetHeight()/2);
			int scroll = ( (m_credit_scroll < 0)? 0 : m_credit_scroll );
			int scroll2 = ( (m_credit_scroll < 0)? m_credit_scroll : m_credit_scroll );
			int scroll3 = ( (m_credit_scroll < 0)? 400+m_credit_scroll : 400 );
			
			Rect rect(0, scroll, m_pC_credit->GetWidth(1), min(scroll3, m_pC_credit->GetHeight(1) - scroll));
			m_pC_credit->BltLockedClip(scroll_x, scroll_y-scroll2, rect, 1);
			
			Rect rect2(0, 0, m_pC_credit->GetWidth(2), min(scroll3 - (m_pC_credit->GetHeight(1) - scroll) -50, m_pC_credit->GetHeight(2)));
			m_pC_credit->BltLockedClip(scroll_x +m_pC_credit->GetWidth(1)/2-m_pC_credit->GetWidth(2)/2, scroll_y + max(150, (m_pC_credit->GetHeight(1) - scroll + 50) ), rect2, 2);
			//return;

			gpC_base->m_p_DDSurface_back->Unlock();
		}
		else
		{
			m_title_spk.BltLocked();
			
			//if(!gC_vs_ui.IsRunningOption())
			//add by zdj				
				//DrawTitleEffect();

//			if(m_credit_scroll < 0)m_credit_scroll = 0;
//			const int title_x = 275, title_y = 30;
//			const int title_frame = (m_credit_scroll%60)/3;
//			if(title_frame < m_pC_title_ani.GetSize())
//				m_pC_title_ani.BltLocked(title_x, title_y, title_frame);
//			else
//				m_pC_title_ani.BltLocked(title_x, title_y, 19-title_frame);

//			m_pC_title_ani.BltLockedAlpha(title_x, title_y, 1, title_frame*31/100);
			
//			const int symbol_x = 323, symbol_y = 125;
//			const int symbol_frame = (m_credit_scroll%(14*5))/5;
//			if(symbol_frame < m_pC_symbol_ani.GetSize())
//				m_pC_symbol_ani.BltLocked(symbol_x, symbol_y, symbol_frame);
//			else if(14-symbol_frame >= 0)
//				m_pC_symbol_ani.BltLocked(symbol_x, symbol_y, 14-symbol_frame);
//			else m_pC_symbol_ani.BltLocked(symbol_x, symbol_y);
			m_pC_button_group->Show();

			if(m_pC_use_grade!=NULL && !g_pUserInformation->bChinese )
			{
				if(g_pUserInformation->bNetmarbleGoreLevel)
				{
					m_pC_use_grade->BltLocked(g_GameRect.right-m_pC_use_grade->GetWidth(ADULT)-30,30,ADULT);
				}
				else
				{
					m_pC_use_grade->BltLocked(g_GameRect.right-m_pC_use_grade->GetWidth(CHILD)-30,30,CHILD);
				}
			}
			
			gpC_base->m_p_DDSurface_back->Unlock();

			char sz_temp[50];
			if(g_pUserInformation->IsNetmarble)
				sprintf(sz_temp, "%s : %1.2f", (*g_pGameStringTable)[UI_STRING_MESSAGE_NETMARBLE_CLIENT_VERSION].GetString(),(float)g_pUserInformation->GameVersion/100+3);
			else
				//add by zdj
//				sprintf(sz_temp, "%s : %1.2f", (*g_pGameStringTable)[UI_STRING_MESSAGE_CLIENT_VERSION].GetString(),(float)g_pUserInformation->GameVersion/100+3);
				sprintf(sz_temp,"%s","  莖조崎《屆沂촬헙》 V2.20");
			//modify by viva for Notice
			//g_PrintColorStr(g_GameRect.right- 50 -g_GetStringWidth(sz_temp, gpC_base->m_info_pi.hfont), g_GameRect.bottom -30, sz_temp, gpC_base->m_info_pi, RGB_WHITE);
			//end
		}
	}

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// C_VS_UI_TITLE::Timer
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_TITLE::Timer()
{
	if(m_dw_prev_tickcount+m_dw_millisec <= GetTickCount())
	{
		m_dw_prev_tickcount = GetTickCount();
		m_credit_scroll++;
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_TITLE::MouseControl(UINT message, int _x, int _y)
{
	if (m_bl_credit)
	{
		if (message == M_LEFTBUTTON_UP)
		{
			m_bl_credit = false;
			DeleteNew(m_pC_credit);
			EMPTY_MOVE;
		}

		return true;
	}

	Window::MouseControl(message, _x, _y);

	m_pC_button_group->MouseControl(message, _x, _y);

	return true; // no game, then 'true'
}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::KeyboardControl(UINT message, UINT key, long extra)
{
	if (m_bl_credit)
	{
		if (message == WM_KEYDOWN)
			if (key == VK_ESCAPE)
			{
				m_bl_credit = false;
				DeleteNew(m_pC_credit);
				EMPTY_MOVE;
			}

		return;
	}

	// 만약 IME toggle되어 있을 경우를 대비하여 scan code로 한다.

	id_t id = INVALID_ID;
	if (message == WM_KEYDOWN) 
	{
		switch (SCAN_CODE(extra))
		{
			case SCANCODE_C: // 'C'onnect
				id = CONNECT;
				break;

//			case SCANCODE_T: // 'T'utorial
//				id = TUTORIAL;
//				break;

			case SCANCODE_O: // 'O'ption
				id = OPTION;
				break;

			case SCANCODE_R: // C'r'edit
				id = CREDIT;
				break;

			case SCANCODE_X: // E'x'it
				id = EXIT;
				break;

			default:
				id = INVALID_ID;
		}
	}

	Run(id);
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_TITLE::Run(id_t id)
{
	switch (id)
	{
		case CONNECT:
			// 넷마블용
//			m_pC_login->Start();
			gpC_base->SendMessage(UI_RUN_CONNECT);
			break;

//		case TUTORIAL:
//			g_msg_not_available_menu->Start();
//			break;

		case OPTION:
			//g_msg_not_available_menu->Start();
			gC_vs_ui.RunOption(true);
			break;

		case CREDIT:
			//g_msg_not_available_menu->Start();
			//add by zdj 6.8
			
			//m_pC_credit = new C_SPRITE_PACK(SPK_CREDIT);
			//m_bl_credit = true;

			//m_credit_scroll = -400;


			char str[256];

			GetWindowsDirectory(
				str,  // address of buffer for Windows directory
				255        // size of directory buffer
			);

			sprintf(str, "%s\\Explorer.exe", str);
			
			CDirectDraw::GetDD()->RestoreDisplayMode();

			_spawnl(_P_NOWAIT, str, "Explorer.exe", "http://www.ttdk2.com", NULL);

			//_spawnl(_P_NOWAIT, str, "Explorer.exe", g_pClientConfig->URL_HOMEPAGE_NEW_USER.GetString(), NULL);



			break;

		case EXIT:
			gpC_base->SendMessage(UI_TERMINATION, 0, 0);
			break;
	}
}


/*-----------------------------------------------------------------------------
- C_VS_UI_OPTION
-
-----------------------------------------------------------------------------*/
C_VS_UI_OPTION::C_VS_UI_OPTION(bool IsTitle)
{
	m_IsTitle = IsTitle;

	// CONTROL TAB
	if(g_pUserOption->UseEnterChat)
	{
		m_check[CHECK_NORMAL_CHAT] = CHECK_NOT;
		m_check[CHECK_ENTER_CHAT] = CHECK_CHECK;
	}
	else
	{
		m_check[CHECK_NORMAL_CHAT] = CHECK_CHECK;
		m_check[CHECK_ENTER_CHAT] = CHECK_NOT;
	}

//	m_check[CHECK_MOUSE_SPEED] = g_pUserOption->UseMouseSpeed?CHECK_CHECK:CHECK_NOT;
	m_value_mouse_speed = g_pUserOption->MouseSpeedValue;

	if(gpC_Imm && gpC_Imm->IsDevice())
	{
		m_check[CHECK_IFEEL] = g_pUserOption->UseForceFeel?CHECK_CHECK:CHECK_NOT;
	}
	else	m_check[CHECK_IFEEL] = CHECK_DISABLE;

	// GRAPHIC TAB
	m_check[CHECK_ALPHA_HPBAR] = g_pUserOption->DrawTransHPBar?CHECK_CHECK:CHECK_NOT;
	m_check[CHECK_BLOOD_DROP] = g_pUserOption->BloodDrop?CHECK_CHECK:CHECK_NOT;
	m_check[CHECK_ALPHA_DEPTH] = CHECK_DISABLE;
	m_check[CHECK_DEFAULT_ALPHA] = g_pUserOption->DefaultAlpha?CHECK_CHECK:CHECK_NOT;
	if(CDirectDraw::IsSupportGammaControl())
	{
		m_check[CHECK_GAMMA] = g_pUserOption->UseGammaControl?CHECK_CHECK:CHECK_NOT;
		m_value_gamma = g_pUserOption->GammaValue;
	}
	else	m_check[CHECK_GAMMA] = CHECK_DISABLE;

	m_check[CHECK_CHATBOX] = g_pUserOption->DrawChatBoxOutline?CHECK_CHECK:CHECK_NOT;
	m_check[CHECK_FPS] = g_pUserOption->DrawFPS?CHECK_CHECK:CHECK_NOT;
	m_check[CHECK_AUTOHIDE_SMOOTH] = g_pUserOption->AutoHideSmoothScroll?CHECK_CHECK:CHECK_NOT;

	// SOUND TAB
//	m_check[CHECK_YELL] = g_pUserOption->PlayYellSound?CHECK_CHECK:CHECK_NOT;
	m_check[CHECK_SOUND] = g_pUserOption->PlaySound?CHECK_CHECK:CHECK_NOT;
	m_check[CHECK_MUSIC] = g_pUserOption->PlayMusic?CHECK_CHECK:CHECK_NOT;
	m_volume_sound = g_pUserOption->VolumeSound;
	m_volume_music = g_pUserOption->VolumeMusic;

//	if(g_pUserOption->PlaySound)
//	{
//		m_check[CHECK_YELL] = g_pUserOption->PlayYellSound?CHECK_CHECK:CHECK_NOT;
//	}
//	else m_check[CHECK_YELL] = CHECK_DISABLE;

	if(g_pUserOption->PlayMusic)
	{
		if(g_pUserOption->PlayWaveMusic)
		{
			m_check[CHECK_WAV] = CHECK_CHECK;
			m_check[CHECK_MIDI] = CHECK_NOT;
		}
		else
		{
			m_check[CHECK_WAV] = CHECK_NOT;
			m_check[CHECK_MIDI] = CHECK_CHECK;
		}
	}
	else
	{
		m_check[CHECK_WAV] = CHECK_DISABLE;
		m_check[CHECK_MIDI] = CHECK_DISABLE;
	}

	// GAME TAB
	m_check[CHECK_HELP] = g_pUserOption->ShowChoboHelp?CHECK_CHECK:CHECK_NOT;
	m_check[CHECK_FILTERING] = g_pUserOption->FilteringCurse?CHECK_CHECK:CHECK_NOT;
//	m_check[CHECK_CHANGE] = g_pUserOption->TribeChange?CHECK_CHECK:CHECK_NOT;
//	m_check[CHECK_PARTY_INVITE] = g_pUserOption->DenyPartyInvite?CHECK_CHECK:CHECK_NOT;
//	m_check[CHECK_PARTY_REQUEST] = g_pUserOption->DenyPartyRequest?CHECK_CHECK:CHECK_NOT;
	m_check[CHECK_PRELOAD_MONSTER] = g_pUserOption->IsPreLoadMonster?CHECK_CHECK:CHECK_NOT;		
	m_check[CHECK_CHAT_WHITE] = g_pUserOption->ChatWhite?CHECK_CHECK:CHECK_NOT;		
	
	m_check[CHECK_TEEN_VERSION] = g_pUserOption->UseTeenVersion?CHECK_CHECK:CHECK_NOT;
	m_check[CHECK_POPUPCHAT_BY_WHISPER] = g_pUserOption->PopupChatByWhisper?CHECK_CHECK:CHECK_NOT;		
//	m_check[CHECK_NOT_SEND_MY_INFO] = g_pUserOption->NotSendMyInfo ? CHECK_CHECK:CHECK_NOT;
	m_check[CHECK_DO_NOT_SHOW_WAR_MSG] = g_pUserOption->DoNotShowWarMsg ? CHECK_CHECK:CHECK_NOT;
	m_check[CHECK_DO_NOT_SHOW_LAIR_MSG] = g_pUserOption->DoNotShowLairMsg ? CHECK_CHECK:CHECK_NOT;
	m_check[CHECK_DO_NOT_SHOW_HOLY_LAND_MSG] = g_pUserOption->DoNotShowHolyLandMsg ? CHECK_CHECK:CHECK_NOT;

	m_check[CHECK_SHOW_GAMEMONEY_WITH_HANGUL] = g_pUserOption->ShowGameMoneyWithHANGUL ? CHECK_CHECK:CHECK_NOT;
	m_check[CHECK_DO_NOT_SHOW_PERSNALSHOP_MSG] = g_pUserOption->DoNotShowPersnalShopMsg? CHECK_CHECK:CHECK_NOT;

	

	
	m_pC_main_spk = NULL;
	m_pC_etc_spk = NULL;
	m_pC_scroll_bar = NULL;
	m_i_selected_tab = TAB_CONTROL;

	if(false == m_IsTitle)
	{
		m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(3, Rect(370, 92, -1, 100));
		m_i_selected_tab = TAB_CONTROL;

		switch(g_eRaceInterface)
		{
		case RACE_SLAYER:
			m_vampire_plus_x = 0; m_vampire_plus_y = 0;
			m_pC_main_spk = new C_SPRITE_PACK(SPK_OPTION_SLAYER);
			break;

		case RACE_VAMPIRE:
			m_vampire_plus_x = 9; m_vampire_plus_y = 1;
			m_pC_main_spk = new C_SPRITE_PACK(SPK_OPTION_VAMPIRE);
			break;

		case RACE_OUSTERS:
			m_vampire_plus_x = 12; m_vampire_plus_y = 10;
			m_pC_main_spk = new C_SPRITE_PACK(SPK_OPTION_OUSTERS);
			break;
		}
		
		m_pC_etc_spk = new C_SPRITE_PACK(SPK_OPTION);
		Set(g_GameRect.right /2 - m_pC_main_spk->GetWidth()/2, g_GameRect.bottom /2 - m_pC_main_spk->GetHeight()/2, m_pC_main_spk->GetWidth(), m_pC_main_spk->GetHeight());
	}
	else
	{	
		m_vampire_plus_x = 0; m_vampire_plus_y = 0;
		if(g_MyFull)
		{
			m_pC_main_spk = new C_SPRITE_PACK(SPK_TITLE_OPTION_1024);
		}
		else
			m_pC_main_spk = new C_SPRITE_PACK(SPK_TITLE_OPTION);
		m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(378, 90, -1, 112), false, SPK_TITLE_OPTION_SCROLL, 2, 2, 2);
		if(g_MyFull)
			Set(330, 230, m_pC_main_spk->GetWidth(), m_pC_main_spk->GetHeight());
		else
			Set(g_GameRect.right /2 - m_pC_main_spk->GetWidth()/2, g_GameRect.bottom /2 - m_pC_main_spk->GetHeight()/2 - 59, m_pC_main_spk->GetWidth(), m_pC_main_spk->GetHeight());
	}
	AttrTopmost(true);



	const InterfaceInformation* pSkin = &g_pSkinManager->Get( SkinManager::OPTION );
	m_pC_button_group = new ButtonGroup(this);

	// 하단 디폴트등등 버튼
//	int default_button_x = 174, default_button_y = 282, load_button_x = 246, save_button_x = 303;
//	m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(default_button_x, default_button_y, m_pC_etc_spk->GetWidth(DEFAULT_BUTTON_HILIGHTED), m_pC_etc_spk->GetHeight(DEFAULT_BUTTON_HILIGHTED), DEFAULT_ID, this, DEFAULT_BUTTON_HILIGHTED) );
//	m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(load_button_x, default_button_y, m_pC_etc_spk->GetWidth(LOAD_BUTTON_HILIGHTED), m_pC_etc_spk->GetHeight(LOAD_BUTTON_HILIGHTED), LOAD_ID, this, LOAD_BUTTON_HILIGHTED) );
//	m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(save_button_x, default_button_y, m_pC_etc_spk->GetWidth(SAVE_BUTTON_HILIGHTED), m_pC_etc_spk->GetHeight(SAVE_BUTTON_HILIGHTED), SAVE_ID, this, SAVE_BUTTON_HILIGHTED) );

	// close버튼
	//int close_button_x = pSkin->GetPoint(0).x, close_button_y = pSkin->GetPoint(0).y;
	int i = 0;
	
	if(false == m_IsTitle)
	{
		Tab_X[0] = pSkin->GetRect(0).left;
		Tab_X[1] = pSkin->GetRect(0).top;
		Tab_X[2] = pSkin->GetRect(0).right;
		Tab_X[3] = pSkin->GetRect(0).bottom;
		Tab_Y = 20;

		m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint(0).x,pSkin->GetPoint(0).y, m_pC_etc_spk->GetWidth(BUTTON_CLOSE), m_pC_etc_spk->GetHeight(BUTTON_CLOSE), CLOSE_ID, this, BUTTON_CLOSE) );

		// tab버튼
		//int tab_control_x = 116, tab_y = 20, tab_graphic_x = 170, tab_sound_x = 224, tab_game_x = 278;
		m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint(1).x, pSkin->GetPoint(1).y, m_pC_main_spk->GetWidth(TAB_CONTROL), m_pC_main_spk->GetHeight(TAB_CONTROL), CONTROL_ID, this, TAB_CONTROL) );
		m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint(2).x, pSkin->GetPoint(2).y, m_pC_main_spk->GetWidth(TAB_GRAPHIC), m_pC_main_spk->GetHeight(TAB_GRAPHIC), GRAPHIC_ID, this, TAB_GRAPHIC) );
		m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint(3).x, pSkin->GetPoint(3).y, m_pC_main_spk->GetWidth(TAB_SOUND), m_pC_main_spk->GetHeight(TAB_SOUND), SOUND_ID, this, TAB_SOUND) );
		m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(pSkin->GetPoint(4).x, pSkin->GetPoint(4).y, m_pC_main_spk->GetWidth(TAB_GAME), m_pC_main_spk->GetHeight(TAB_GAME), GAME_ID, this, TAB_GAME) );

		m_check_x = pSkin->GetPoint(5).x, m_check_y = pSkin->GetPoint(5).y, m_check_gap = 20;

		
		// control_tab 버튼들
	//	int list_button_x = 341, list_button_y = 54;
		m_pC_control_button_group = new ButtonGroup(this);
		m_pC_control_button_group->Add( new C_VS_UI_EVENT_BUTTON(m_check_x+10, m_check_y, m_pC_etc_spk->GetWidth(RADIO_BACK_DISABLE), m_pC_etc_spk->GetHeight(RADIO_BACK_DISABLE), CHECK_NORMAL_CHAT, this, RADIO_BACK_DISABLE) );
		m_pC_control_button_group->Add( new C_VS_UI_EVENT_BUTTON(m_check_x+120, m_check_y, m_pC_etc_spk->GetWidth(RADIO_BACK_DISABLE), m_pC_etc_spk->GetHeight(RADIO_BACK_DISABLE), CHECK_ENTER_CHAT, this, RADIO_BACK_DISABLE) );

		for(i = 0; i < CHECK_CONTROL_MAX; i++)
			m_pC_control_button_group->Add( new C_VS_UI_EVENT_BUTTON(m_check_x, m_check_y+m_check_gap*(8+i), m_pC_etc_spk->GetWidth(CHECK_BACK_DISABLE), m_pC_etc_spk->GetHeight(CHECK_BACK_DISABLE), CHECK_CONTROL_TAB+i+2, this, CHECK_BACK_DISABLE) );

		// graphic_tab 버튼들
		m_pC_graphic_button_group = new ButtonGroup(this);
		for(i = 0; i < CHECK_GRAPHIC_MAX; i++)
			m_pC_graphic_button_group->Add( new C_VS_UI_EVENT_BUTTON(m_check_x, m_check_y+m_check_gap*i, m_pC_etc_spk->GetWidth(CHECK_BACK_DISABLE), m_pC_etc_spk->GetHeight(CHECK_BACK_DISABLE), CHECK_GRAPHIC_TAB+i, this, CHECK_BACK_DISABLE) );
		// sound_tab 버튼들
		m_pC_sound_button_group = new ButtonGroup(this);
		for(i = 0; i < CHECK_SOUND_MAX; i++)
			m_pC_sound_button_group->Add( new C_VS_UI_EVENT_BUTTON(m_check_x, m_check_y+m_check_gap*i, m_pC_etc_spk->GetWidth(CHECK_BACK_DISABLE), m_pC_etc_spk->GetHeight(CHECK_BACK_DISABLE), CHECK_SOUND_TAB+i, this, CHECK_BACK_DISABLE) );

		// game_tab 버튼들
		m_pC_game_button_group = new ButtonGroup(this);
		for(i = 0; i < CHECK_GAME_MAX; i++)
			m_pC_game_button_group->Add( new C_VS_UI_EVENT_BUTTON(m_check_x, m_check_y+m_check_gap*i, m_pC_etc_spk->GetWidth(CHECK_BACK_DISABLE), m_pC_etc_spk->GetHeight(CHECK_BACK_DISABLE), CHECK_GAME_TAB+i, this, CHECK_BACK_DISABLE) );

		m_rt_value[1].Set(m_check_x+120, m_check_y+m_check_gap*(CHECK_GAMMA-CHECK_GRAPHIC_TAB) , m_pC_etc_spk->GetWidth(VOLUME_BAR), 15);
		m_rt_value[2].Set(m_check_x+120, m_check_y+m_check_gap*(CHECK_SOUND-CHECK_SOUND_TAB), m_pC_etc_spk->GetWidth(VOLUME_BAR), 15);
		m_rt_value[3].Set(m_check_x+120, m_check_y+m_check_gap*(CHECK_MUSIC-CHECK_SOUND_TAB), m_pC_etc_spk->GetWidth(VOLUME_BAR), 15);
		m_rt_value[4].Set(m_check_x+120, m_check_y+m_check_gap*(CHECK_ALPHA_DEPTH-CHECK_GRAPHIC_TAB), m_pC_etc_spk->GetWidth(VOLUME_BAR), 15);

	}
	else
	{
		for(int i = 0; i<4; i++)
			Tab_X[i] = 46 + (i*69);
		Tab_Y = 29;
		if(g_MyFull)
			m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(305, 268, m_pC_main_spk->GetWidth(TITLE_BUTTON_EXIT_HILIGHT), m_pC_main_spk->GetHeight(TITLE_BUTTON_EXIT_HILIGHT), CLOSE_ID, this, TITLE_BUTTON_EXIT_HILIGHT) );
		else
			m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(312, 237, m_pC_main_spk->GetWidth(TITLE_BUTTON_EXIT_HILIGHT), m_pC_main_spk->GetHeight(TITLE_BUTTON_EXIT_HILIGHT), CLOSE_ID, this, TITLE_BUTTON_EXIT_HILIGHT) );
		m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(Tab_X[0], Tab_Y, m_pC_main_spk->GetWidth(TITLE_TAB_CONTROL_HILIGHT), m_pC_main_spk->GetHeight(TITLE_TAB_CONTROL_HILIGHT), CONTROL_ID, this, TITLE_TAB_CONTROL_HILIGHT) );
		m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(Tab_X[1], Tab_Y, m_pC_main_spk->GetWidth(TITLE_TAB_GRAPHIC_HILIGHT), m_pC_main_spk->GetHeight(TITLE_TAB_GRAPHIC_HILIGHT), GRAPHIC_ID, this, TITLE_TAB_GRAPHIC_HILIGHT) );
		m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(Tab_X[2], Tab_Y, m_pC_main_spk->GetWidth(TITLE_TAB_SOUND_HILIGHT), m_pC_main_spk->GetHeight(TITLE_TAB_SOUND_HILIGHT), SOUND_ID, this, TITLE_TAB_SOUND_HILIGHT) );
		m_pC_button_group->Add( new C_VS_UI_EVENT_BUTTON(Tab_X[3], Tab_Y, m_pC_main_spk->GetWidth(TITLE_TAB_GAME_HILIGHT), m_pC_main_spk->GetHeight(TITLE_TAB_GAME_HILIGHT), GAME_ID, this, TITLE_TAB_GAME_HILIGHT) );

		m_check_x = 50, m_check_y = 60 , m_check_gap = 20;

		
	// control_tab 버튼들
	//	int list_button_x = 341, list_button_y = 54;
		m_pC_control_button_group = new ButtonGroup(this);
		m_pC_control_button_group->Add( new C_VS_UI_EVENT_BUTTON(m_check_x+10, m_check_y, m_pC_main_spk->GetWidth(TITLE_RADIO_BACK), m_pC_main_spk->GetHeight(TITLE_RADIO_BACK), CHECK_NORMAL_CHAT, this, TITLE_RADIO_BACK) );
		m_pC_control_button_group->Add( new C_VS_UI_EVENT_BUTTON(m_check_x+120, m_check_y, m_pC_main_spk->GetWidth(TITLE_RADIO_BACK), m_pC_main_spk->GetHeight(TITLE_RADIO_BACK), CHECK_ENTER_CHAT, this, TITLE_RADIO_BACK) );

		for(i = 0; i < CHECK_CONTROL_MAX; i++)
			m_pC_control_button_group->Add( new C_VS_UI_EVENT_BUTTON(m_check_x, m_check_y+m_check_gap*(8+i), m_pC_main_spk->GetWidth(TITLE_CHECK_BACK), m_pC_main_spk->GetHeight(TITLE_CHECK_BACK), CHECK_CONTROL_TAB+i+2, this, TITLE_CHECK_BACK) );

		// graphic_tab 버튼들
		m_pC_graphic_button_group = new ButtonGroup(this);
		for(i = 0; i < CHECK_GRAPHIC_MAX; i++)
			m_pC_graphic_button_group->Add( new C_VS_UI_EVENT_BUTTON(m_check_x, m_check_y+m_check_gap*i, m_pC_main_spk->GetWidth(TITLE_CHECK_BACK), m_pC_main_spk->GetHeight(TITLE_CHECK_BACK), CHECK_GRAPHIC_TAB+i, this, TITLE_CHECK_BACK) );
		// sound_tab 버튼들
		m_pC_sound_button_group = new ButtonGroup(this);
		for(i = 0; i < CHECK_SOUND_MAX; i++)
			m_pC_sound_button_group->Add( new C_VS_UI_EVENT_BUTTON(m_check_x, m_check_y+m_check_gap*i, m_pC_main_spk->GetWidth(TITLE_CHECK_BACK), m_pC_main_spk->GetHeight(TITLE_CHECK_BACK), CHECK_SOUND_TAB+i, this, TITLE_CHECK_BACK) );
		// game_tab 버튼들
		m_pC_game_button_group = new ButtonGroup(this);
		for(i = 0; i < CHECK_GAME_MAX; i++)
			m_pC_game_button_group->Add( new C_VS_UI_EVENT_BUTTON(m_check_x, m_check_y+m_check_gap*i, m_pC_main_spk->GetWidth(TITLE_CHECK_BACK), m_pC_main_spk->GetHeight(TITLE_CHECK_BACK), CHECK_GAME_TAB+i, this, TITLE_CHECK_BACK) );

		m_rt_value[1].Set(m_check_x+120, m_check_y+m_check_gap*(CHECK_GAMMA-CHECK_GRAPHIC_TAB) , m_pC_main_spk->GetWidth(TITLE_VOLUME_BAR), 15);
		m_rt_value[2].Set(m_check_x+120, m_check_y+m_check_gap*(CHECK_SOUND-CHECK_SOUND_TAB), m_pC_main_spk->GetWidth(TITLE_VOLUME_BAR), 15);
		m_rt_value[3].Set(m_check_x+120, m_check_y+m_check_gap*(CHECK_MUSIC-CHECK_SOUND_TAB), m_pC_main_spk->GetWidth(TITLE_VOLUME_BAR), 15);
		m_rt_value[4].Set(m_check_x+120, m_check_y+m_check_gap*(CHECK_ALPHA_DEPTH-CHECK_GRAPHIC_TAB), m_pC_main_spk->GetWidth(TITLE_VOLUME_BAR), 15);

	}

//	m_rt_value[0].Set(m_check_x+120, m_check_y+m_check_gap*(6+CHECK_MOUSE_SPEED-CHECK_CONTROL_TAB), m_pC_etc_spk->GetWidth(VOLUME_BAR), 15);

	g_RegisterWindow(this);

//	if(false == m_IsTitle)
//	{
//		DeleteNew(m_pC_main_spk);
//		m_pC_main_spk = NULL;
//		DeleteNew(m_pC_etc_spk);
//		m_pC_etc_spk = NULL;
//	}
}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_OPTION
-
-----------------------------------------------------------------------------*/
C_VS_UI_OPTION::~C_VS_UI_OPTION()
{
	g_UnregisterWindow(this);

	DeleteNew(m_pC_scroll_bar);
	DeleteNew(m_pC_button_group);
	DeleteNew(m_pC_control_button_group);
	DeleteNew(m_pC_graphic_button_group);
	DeleteNew(m_pC_sound_button_group);
	DeleteNew(m_pC_game_button_group);
	if(m_pC_main_spk)
	{
		DeleteNew(m_pC_main_spk);
		m_pC_main_spk = NULL;
		DeleteNew(m_pC_etc_spk);
		m_pC_etc_spk = NULL;
	}

}

//-----------------------------------------------------------------------------
// C_VS_UI_OPTION::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_OPTION::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	
	//체크&라디오 버튼들
	if(false == m_IsTitle)
	{
		if(p_button->m_image_index == CHECK_BACK_DISABLE || p_button->m_image_index == RADIO_BACK_DISABLE)
		{
			if(m_check[p_button->GetID()] == CHECK_DISABLE || m_check[p_button->GetID()] == CHECK_CHECK_DISABLE)
			{
				m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index);
				if(m_check[p_button->GetID()] == CHECK_CHECK_DISABLE)
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+p_button->x+2, y+m_vampire_plus_y+p_button->y+2, p_button->m_image_index+2);
			}
			else
			{
				m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index+1);

				int radio_plus_x = 0, radio_plus_y = 0;
				if(p_button->m_image_index == RADIO_BACK_DISABLE)
				{
					radio_plus_x = 1; radio_plus_y = 1;
				}

				if(p_button->GetPressState())
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+radio_plus_x+p_button->x+2, y+m_vampire_plus_y+radio_plus_y+p_button->y+3, p_button->m_image_index+2);
				else if(m_check[p_button->GetID()] == CHECK_CHECK)
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+radio_plus_x+p_button->x+2, y+m_vampire_plus_y+radio_plus_y+p_button->y+2, p_button->m_image_index+2);
			}
		}
		//tab버튼들
		else if(p_button->GetID() == CONTROL_ID || p_button->GetID() == GRAPHIC_ID || p_button->GetID() == SOUND_ID || p_button->GetID() == GAME_ID)
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
				{
					m_pC_main_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index);
				}
				else
				{
					Rect rect(0, 0, p_button->w, p_button->h-1);
					m_pC_main_spk->BltLockedClip(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, rect, p_button->m_image_index);
				}
			}
		}
		//Close버튼
		else if(p_button->GetID() == CLOSE_ID)
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index+3);
				else
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index+2);
			}
			else
			{
				if(p_button->GetPressState())
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index+1);
				else
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index);
			}

		}
		//Hotkey List버튼
		else if(p_button->GetID() == LIST_ID)
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index+3);
				else
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index+2);
			}
			else
			{
				if(p_button->GetPressState())
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index+1);
				else
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index);
			}

		}
		else
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
			{
				m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index+1);
			}
			else
				m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index);
		}
	}
	else
	{
		if(p_button->m_image_index == TITLE_CHECK_BACK || p_button->m_image_index == TITLE_RADIO_BACK)
		{
			if(m_check[p_button->GetID()] == CHECK_DISABLE || m_check[p_button->GetID()] == CHECK_CHECK_DISABLE)
			{
				m_pC_main_spk->BltLockedDarkness(x + p_button->x, y + p_button->y, p_button->m_image_index, 1);
			}
			else
			{
				int radio_plus_x = 0, radio_plus_y = 0;
				if(p_button->m_image_index == TITLE_RADIO_BACK)
				{
					radio_plus_x = 3; radio_plus_y = 3;
					m_pC_main_spk->BltLocked(x + p_button->x, y + p_button->y, p_button->m_image_index);
					if(m_check[p_button->GetID()] == CHECK_CHECK)
					m_pC_main_spk->BltLocked(x + p_button->x + radio_plus_x, y + p_button->y + radio_plus_y, p_button->m_image_index+1);
				}
				else
				{
					if(m_check[p_button->GetID()] == CHECK_CHECK)
						m_pC_main_spk->BltLocked(x + p_button->x + radio_plus_x, y + p_button->y + radio_plus_y, p_button->m_image_index+1);
					else
						m_pC_main_spk->BltLocked(x + p_button->x + radio_plus_x, y + p_button->y + radio_plus_y, p_button->m_image_index);
				}
//				int radio_plus_x = 0, radio_plus_y = 0;
//				if(p_button->m_image_index == RADIO_BACK_DISABLE)
//				{
//					if(m_check[p_button->GetID()] == CHECK_CHECK)
//					radio_plus_x = 1; radio_plus_y = 1;
//				}
//
//				if(p_button->GetPressState())
//					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+radio_plus_x+p_button->x+2, y+m_vampire_plus_y+radio_plus_y+p_button->y+3, p_button->m_image_index+2);
//				else if(m_check[p_button->GetID()] == CHECK_CHECK)
//					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+radio_plus_x+p_button->x+2, y+m_vampire_plus_y+radio_plus_y+p_button->y+2, p_button->m_image_index+2);
			}
		}
		//tab버튼들
		else if(p_button->GetID() == CONTROL_ID || p_button->GetID() == GRAPHIC_ID || p_button->GetID() == SOUND_ID || p_button->GetID() == GAME_ID)
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
				{
					m_pC_main_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+4);
				}
				else
				{
					m_pC_main_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
				}
			}
		}
		//Close버튼
		else if(p_button->GetID() == CLOSE_ID)
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_pC_main_spk->BltLocked(x+p_button->x, y+p_button->y, TITLE_BUTTON_EXIT_PUSH);
				else
					m_pC_main_spk->BltLocked(x+p_button->x+1, y+p_button->y, TITLE_BUTTON_EXIT_HILIGHT);
			}
		}
		//Hotkey List버튼
		else if(p_button->GetID() == LIST_ID)
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_pC_main_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index+3);
				else
					m_pC_main_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index+2);
			}
			else
			{
				if(p_button->GetPressState())
					m_pC_main_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index+1);
				else
					m_pC_main_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index);
			}

		}
//		else
//		if(p_button->GetFocusState())
//		{
//			if(p_button->GetPressState())
//			{
//				m_pC_main_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index+1);
//			}
//			else
//				m_pC_main_spk->BltLocked(x+m_vampire_plus_x+p_button->x, y+m_vampire_plus_y+p_button->y, p_button->m_image_index);
//		}
	}
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OPTION::Process()
{
	m_pC_button_group->Process();
	switch(m_i_selected_tab)
	{
	case TAB_CONTROL:
		m_pC_control_button_group->Process();
		break;

	case TAB_GRAPHIC:
		m_pC_graphic_button_group->Process();
		break;

	case TAB_SOUND:
		m_pC_sound_button_group->Process();
		break;

	case TAB_GAME:
		m_pC_game_button_group->Process();
		break;
	}
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OPTION::Start()
{
	m_bLBPush = false;

	PI_Processor::Start();

	m_pC_button_group->Init();
	m_pC_control_button_group->Init();
	m_pC_graphic_button_group->Init();
	m_pC_sound_button_group->Init();
	m_pC_game_button_group->Init();

//	if(false == m_IsTitle)
//	{
//		if(m_pC_main_spk == NULL)
//		{
//			switch(g_eRaceInterface)
//			{
//			case RACE_SLAYER:
//				m_pC_main_spk = new C_SPRITE_PACK(SPK_OPTION_SLAYER);
//				break;
//
//			case RACE_VAMPIRE:
//				m_pC_main_spk = new C_SPRITE_PACK(SPK_OPTION_VAMPIRE);
//				break;
//
//			case RACE_OUSTERS:
//				m_pC_main_spk = new C_SPRITE_PACK(SPK_OPTION_OUSTERS);
//				break;
//			}
//			
//			m_pC_etc_spk = new C_SPRITE_PACK(SPK_OPTION);
//		}
//	}
	// 넷마블일경우에는 g_pUserInformation 의 bNetmarbleGoreLevel 를 사용하자.
	// false 일 경우가 teenversion.
	
	if(!gC_vs_ui.IsGameMode())
	{
		// 메뉴에서 넷마블 틴버젼이면 강제 설정후 Disable 그렇지 않으면 옵션에 따른다.
		if(!g_pUserInformation->bNetmarbleGoreLevel&&g_pUserInformation->IsNetmarble)
			m_check[CHECK_TEEN_VERSION] = CHECK_CHECK_DISABLE;			
		else
			m_check[CHECK_TEEN_VERSION] = g_pUserOption->UseTeenVersion?CHECK_CHECK:CHECK_NOT;
	} else		
	{
		// 넷마블 틴버젼이면 강제 설정. 그렇지 않으면 옵션에 따른다.
		if(!g_pUserInformation->bNetmarbleGoreLevel&&g_pUserInformation->IsNetmarble)
		{
			// 겜중이면
			m_check[CHECK_TEEN_VERSION] = g_pUserInformation->bNetmarbleGoreLevel ? CHECK_DISABLE : CHECK_CHECK_DISABLE ;

		} else
		{
			if(g_pUserInformation->GoreLevel)
				m_check[CHECK_TEEN_VERSION] = g_pUserOption->UseTeenVersion?CHECK_CHECK_DISABLE:CHECK_DISABLE;
			else
				m_check[CHECK_TEEN_VERSION] = CHECK_CHECK_DISABLE;		
		}
	}



	if(g_bEnable3DHAL && !gC_vs_ui.IsGameMode())
	{
		m_check[CHECK_3D] = g_pUserOption->Use3DHAL?CHECK_CHECK:CHECK_NOT;
	}
	else
		m_check[CHECK_3D] = g_pUserOption->Use3DHAL?CHECK_CHECK_DISABLE:CHECK_DISABLE;


	m_pC_scroll_bar->SetPosMax(MAX_ACCELERATOR-14-7+1);
	m_focus_hotkey = -1;

	gpC_window_manager->AppearWindow(this);
}

void C_VS_UI_OPTION::Finish()
{
	if(m_pC_main_spk)
	{
		DeleteNew(m_pC_main_spk);
		m_pC_main_spk = NULL;
		DeleteNew(m_pC_etc_spk);
		m_pC_etc_spk = NULL;
	}
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OPTION::Run(id_t id)
{
	switch (id)
	{
//	case DEFAULT_ID:
//		gC_vs_ui.StartGame(false);
//		break;

//	case SAVE_ID:
//		{
//			char sz_filename[512];
//			wsprintf(sz_filename, "UserSet\\%s.set", g_char_slot_ingame.sz_name);
//			
//			ofstream file(sz_filename, ios::binary);
//			
//			if(file)
//			{
//				gpC_vs_ui_window_manager->SaveToFile(file);
//				file.close();
//			}
//		}
//		break;
//
//	case LOAD_ID:
//		{
//			// window_set file loading
//			char sz_filename[512];
//			wsprintf(sz_filename, "UserSet\\%s.set", g_char_slot_ingame.sz_name);
//			
//			ifstream file(sz_filename, ios::binary | ios::nocreate);
//			
//			if(file)
//			{
//				gpC_vs_ui_window_manager->LoadFromFile(file);
//				file.close();
//			}
//			gC_vs_ui.StartGame(true, false);
//		}
//		break;

	case CLOSE_ID:
		gpC_base->SendMessage( UI_CLOSE_OPTION );
		break;

	case CONTROL_ID:
		m_i_selected_tab = TAB_CONTROL;
		break;

	case GRAPHIC_ID:
		m_i_selected_tab = TAB_GRAPHIC;
		break;

	case SOUND_ID:
		m_i_selected_tab = TAB_SOUND;
		break;

	case GAME_ID:
		m_i_selected_tab = TAB_GAME;
		break;

	// CONTROL TAB
	case CHECK_NORMAL_CHAT:
	case CHECK_ENTER_CHAT:
		{
			g_pUserOption->UseEnterChat = id == CHECK_ENTER_CHAT;
			m_check[CHECK_NORMAL_CHAT] = g_pUserOption->UseEnterChat?CHECK_NOT:CHECK_CHECK;
			m_check[CHECK_ENTER_CHAT] = g_pUserOption->UseEnterChat?CHECK_CHECK:CHECK_NOT;
		}
		break;

	case CHECK_IFEEL:
		if(m_check[CHECK_IFEEL] != CHECK_DISABLE)
		{
			m_check[CHECK_IFEEL] = (m_check[CHECK_IFEEL] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->UseForceFeel = m_check[CHECK_IFEEL] == CHECK_CHECK;
			if(gpC_Imm && g_pUserOption->UseForceFeel)gpC_Imm->Enable();
			else if(gpC_Imm)gpC_Imm->Disable();
		}
		break;

//	case CHECK_MOUSE_SPEED:
//		if(m_check[CHECK_MOUSE_SPEED] != CHECK_DISABLE)
//		{
//			m_check[CHECK_MOUSE_SPEED] = (m_check[CHECK_MOUSE_SPEED] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
//			g_pUserOption->UseMouseSpeed = m_check[CHECK_MOUSE_SPEED] == CHECK_CHECK;
//		}
		break;

	// GRAPHIC TAB
	case CHECK_3D:
		if(m_check[CHECK_3D] != CHECK_DISABLE && m_check[CHECK_3D] != CHECK_CHECK_DISABLE)
		{
			m_check[CHECK_3D] = (m_check[CHECK_3D] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->Use3DHAL = m_check[CHECK_3D] == CHECK_CHECK;
		}
		break;

	case CHECK_ALPHA_HPBAR:
		if(m_check[CHECK_ALPHA_HPBAR] != CHECK_DISABLE)
		{
			m_check[CHECK_ALPHA_HPBAR] = (m_check[CHECK_ALPHA_HPBAR] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->DrawTransHPBar = m_check[CHECK_ALPHA_HPBAR] == CHECK_CHECK;
		}
		break;

	case CHECK_BLOOD_DROP:
		if(m_check[CHECK_BLOOD_DROP] != CHECK_DISABLE)
		{
			m_check[CHECK_BLOOD_DROP] = (m_check[CHECK_BLOOD_DROP] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->BloodDrop = m_check[CHECK_BLOOD_DROP] == CHECK_CHECK;
		}
		break;

	case CHECK_AUTOHIDE_SMOOTH:
		if(m_check[CHECK_AUTOHIDE_SMOOTH] != CHECK_DISABLE)
		{
			m_check[CHECK_AUTOHIDE_SMOOTH] = (m_check[CHECK_AUTOHIDE_SMOOTH] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->AutoHideSmoothScroll = m_check[CHECK_AUTOHIDE_SMOOTH] == CHECK_CHECK;
		}
		break;

	case CHECK_GAMMA:
		if(m_check[CHECK_GAMMA] != CHECK_DISABLE)
		{
			m_check[CHECK_GAMMA] = (m_check[CHECK_GAMMA] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->UseGammaControl = m_check[CHECK_GAMMA] == CHECK_CHECK;
			if(g_pUserOption->UseGammaControl)CDirectDraw::SetGammaRamp(m_value_gamma);
			else if(CDirectDraw::IsSupportGammaControl())
				CDirectDraw::RestoreGammaRamp();
		}
		break;

	case CHECK_CHATBOX:
		if(m_check[CHECK_CHATBOX] != CHECK_DISABLE)
		{
			m_check[CHECK_CHATBOX] = (m_check[CHECK_CHATBOX] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->DrawChatBoxOutline = m_check[CHECK_CHATBOX] == CHECK_CHECK;
		}
		break;

	case CHECK_FPS:
		if(m_check[CHECK_FPS] != CHECK_DISABLE)
		{
			m_check[CHECK_FPS] = (m_check[CHECK_FPS] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->DrawFPS = m_check[CHECK_FPS] == CHECK_CHECK;
		}
		break;

	case CHECK_DEFAULT_ALPHA:
		if(m_check[CHECK_DEFAULT_ALPHA] != CHECK_DISABLE)
		{
			m_check[CHECK_DEFAULT_ALPHA] = (m_check[CHECK_DEFAULT_ALPHA] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->DefaultAlpha = m_check[CHECK_DEFAULT_ALPHA] == CHECK_CHECK;
		}
		break;

	// SOUND TAB
//	case CHECK_YELL:
//		if(m_check[CHECK_YELL] != CHECK_DISABLE)
//		{
//			m_check[CHECK_YELL] = (m_check[CHECK_YELL] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
//			g_pUserOption->PlayYellSound = m_check[CHECK_YELL] == CHECK_CHECK;
//		}
//		break;

	case CHECK_SOUND:
		if(m_check[CHECK_SOUND] != CHECK_DISABLE)
		{
			m_check[CHECK_SOUND] = (m_check[CHECK_SOUND] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->PlaySound = m_check[CHECK_SOUND] == CHECK_CHECK;

//			if(g_pUserOption->PlaySound)m_check[CHECK_YELL] = g_pUserOption->PlayYellSound?CHECK_CHECK:CHECK_NOT;
//			else m_check[CHECK_YELL] = CHECK_DISABLE;

			gpC_base->SendMessage( UI_CHANGE_OPTION, CHECK_SOUND, m_check[CHECK_SOUND] == CHECK_CHECK );
		}
		break;

	case CHECK_MUSIC:
		if(m_check[CHECK_MUSIC] != CHECK_DISABLE)
		{
			m_check[CHECK_MUSIC] = (m_check[CHECK_MUSIC] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->PlayMusic = m_check[CHECK_MUSIC] == CHECK_CHECK;
			gpC_base->SendMessage( UI_CHANGE_OPTION, CHECK_MUSIC, m_check[CHECK_MUSIC] == CHECK_CHECK );

			if(g_pUserOption->PlayMusic)
			{
				m_check[CHECK_WAV] = g_pUserOption->PlayWaveMusic?CHECK_CHECK:CHECK_NOT;
				m_check[CHECK_MIDI] = g_pUserOption->PlayWaveMusic?CHECK_NOT:CHECK_CHECK;
			}
			else
			{
				m_check[CHECK_WAV] = CHECK_DISABLE;
				m_check[CHECK_MIDI] = CHECK_DISABLE;
			}
		}
		break;

	case CHECK_WAV:
	case CHECK_MIDI:
		if(m_check[CHECK_WAV] != CHECK_DISABLE)
		{
			g_pUserOption->PlayWaveMusic = id == CHECK_WAV;
			m_check[CHECK_WAV] = g_pUserOption->PlayWaveMusic?CHECK_CHECK:CHECK_NOT;
			m_check[CHECK_MIDI] = g_pUserOption->PlayWaveMusic?CHECK_NOT:CHECK_CHECK;
			gpC_base->SendMessage( UI_CHANGE_OPTION, CHECK_WAV, m_check[CHECK_WAV] == CHECK_CHECK );
		}
		break;

	// GAME TAB
	case CHECK_HELP:
		if(m_check[CHECK_HELP] != CHECK_DISABLE)
		{
			m_check[CHECK_HELP] = (m_check[CHECK_HELP] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->ShowChoboHelp = m_check[CHECK_HELP] == CHECK_CHECK;
		}
		break;

	case CHECK_FILTERING:
		if(m_check[CHECK_FILTERING] != CHECK_DISABLE)
		{
			m_check[CHECK_FILTERING] = (m_check[CHECK_FILTERING] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->FilteringCurse = m_check[CHECK_FILTERING] == CHECK_CHECK;
		}
		break;

//	case CHECK_CHANGE:
//		if(m_check[CHECK_CHANGE] != CHECK_DISABLE)
//		{
//			m_check[CHECK_CHANGE] = (m_check[CHECK_CHANGE] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
//			g_pUserOption->TribeChange = m_check[CHECK_CHANGE] == CHECK_CHECK;
//		}
//		break;
//
//	case CHECK_PARTY_INVITE:
//		if(m_check[CHECK_PARTY_INVITE] != CHECK_DISABLE)
//		{
//			m_check[CHECK_PARTY_INVITE] = (m_check[CHECK_PARTY_INVITE] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
//			g_pUserOption->DenyPartyInvite = m_check[CHECK_PARTY_INVITE] == CHECK_CHECK;
//		}
//		break;
//
//	case CHECK_PARTY_REQUEST:
//		if(m_check[CHECK_PARTY_REQUEST] != CHECK_DISABLE)
//		{
//			m_check[CHECK_PARTY_REQUEST] = (m_check[CHECK_PARTY_REQUEST] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
//			g_pUserOption->DenyPartyRequest = m_check[CHECK_PARTY_REQUEST] == CHECK_CHECK;
//		}
//		break;

	case CHECK_PRELOAD_MONSTER:
		if(m_check[CHECK_PRELOAD_MONSTER] != CHECK_DISABLE)
		{
			m_check[CHECK_PRELOAD_MONSTER] = (m_check[CHECK_PRELOAD_MONSTER] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->IsPreLoadMonster = m_check[CHECK_PRELOAD_MONSTER] == CHECK_CHECK;
		}
		break;

	case CHECK_CHAT_WHITE:
		if(m_check[CHECK_CHAT_WHITE] != CHECK_DISABLE)
		{
			m_check[CHECK_CHAT_WHITE] = (m_check[CHECK_CHAT_WHITE] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->ChatWhite = m_check[CHECK_CHAT_WHITE] == CHECK_CHECK;
		}
		break;
	case CHECK_TEEN_VERSION :
		if(m_check[CHECK_TEEN_VERSION] != CHECK_DISABLE&& m_check[CHECK_TEEN_VERSION] != CHECK_CHECK_DISABLE)
		{
			m_check[CHECK_TEEN_VERSION] = (m_check[CHECK_TEEN_VERSION] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->UseTeenVersion = m_check[CHECK_TEEN_VERSION] == CHECK_CHECK;
		}
		break;
	case CHECK_POPUPCHAT_BY_WHISPER :
		if(m_check[CHECK_POPUPCHAT_BY_WHISPER] != CHECK_DISABLE)
		{
			m_check[CHECK_POPUPCHAT_BY_WHISPER] = (m_check[CHECK_POPUPCHAT_BY_WHISPER] == CHECK_CHECK)?CHECK_NOT:CHECK_CHECK;
			g_pUserOption->PopupChatByWhisper = m_check[CHECK_POPUPCHAT_BY_WHISPER] == CHECK_CHECK;
		}
		break;
	case CHECK_DO_NOT_SHOW_WAR_MSG :
		if(m_check[CHECK_DO_NOT_SHOW_WAR_MSG] != CHECK_DISABLE )
		{
			m_check[CHECK_DO_NOT_SHOW_WAR_MSG] = (m_check[CHECK_DO_NOT_SHOW_WAR_MSG] == CHECK_CHECK) ? CHECK_NOT:CHECK_CHECK;
			g_pUserOption->DoNotShowWarMsg = m_check[CHECK_DO_NOT_SHOW_WAR_MSG] == CHECK_CHECK;
		}
		break;
	case CHECK_DO_NOT_SHOW_LAIR_MSG :
		if(m_check[CHECK_DO_NOT_SHOW_LAIR_MSG] != CHECK_DISABLE )
		{
			m_check[CHECK_DO_NOT_SHOW_LAIR_MSG] = (m_check[CHECK_DO_NOT_SHOW_LAIR_MSG] == CHECK_CHECK) ? CHECK_NOT:CHECK_CHECK;
			g_pUserOption->DoNotShowLairMsg = m_check[CHECK_DO_NOT_SHOW_LAIR_MSG] == CHECK_CHECK;
		}
		break;
	case CHECK_DO_NOT_SHOW_HOLY_LAND_MSG :
		if(m_check[CHECK_DO_NOT_SHOW_HOLY_LAND_MSG] != CHECK_DISABLE )
		{
			m_check[CHECK_DO_NOT_SHOW_HOLY_LAND_MSG] = (m_check[CHECK_DO_NOT_SHOW_HOLY_LAND_MSG] == CHECK_CHECK) ? CHECK_NOT:CHECK_CHECK;
			g_pUserOption->DoNotShowHolyLandMsg = m_check[CHECK_DO_NOT_SHOW_HOLY_LAND_MSG] == CHECK_CHECK;
		}
		break;
	case CHECK_SHOW_GAMEMONEY_WITH_HANGUL:
		if(m_check[CHECK_SHOW_GAMEMONEY_WITH_HANGUL] != CHECK_DISABLE )
		{
			m_check[CHECK_SHOW_GAMEMONEY_WITH_HANGUL] = (m_check[CHECK_SHOW_GAMEMONEY_WITH_HANGUL] == CHECK_CHECK) ? CHECK_NOT:CHECK_CHECK;
			g_pUserOption->ShowGameMoneyWithHANGUL = m_check[CHECK_SHOW_GAMEMONEY_WITH_HANGUL] == CHECK_CHECK;
		}
		break;
	case CHECK_DO_NOT_SHOW_PERSNALSHOP_MSG:
		if(m_check[CHECK_DO_NOT_SHOW_PERSNALSHOP_MSG] != CHECK_DISABLE )
		{
			m_check[CHECK_DO_NOT_SHOW_PERSNALSHOP_MSG] = (m_check[CHECK_DO_NOT_SHOW_PERSNALSHOP_MSG] == CHECK_CHECK) ? CHECK_NOT:CHECK_CHECK;
			g_pUserOption->DoNotShowPersnalShopMsg = m_check[CHECK_DO_NOT_SHOW_PERSNALSHOP_MSG] == CHECK_CHECK;
		}
		break;

//	case CHECK_NOT_SEND_MY_INFO :
//		if(m_check[CHECK_NOT_SEND_MY_INFO] != CHECK_DISABLE)
//		{
//			m_check[CHECK_NOT_SEND_MY_INFO] = (m_check[CHECK_NOT_SEND_MY_INFO] == CHECK_CHECK)? CHECK_NOT : CHECK_CHECK ;
//			g_pUserOption->NotSendMyInfo = m_check[CHECK_NOT_SEND_MY_INFO] == CHECK_CHECK;
//		}
//		break;
	}

}


void	C_VS_UI_OPTION::SetValue(int _x, enum VALUE_RECT rect_value)
{
	if(rect_value >= RECT_MAX)return;

	_x = min(m_rt_value[rect_value].x+m_rt_value[rect_value].w, max(m_rt_value[rect_value].x, _x));
	_x -= m_rt_value[rect_value].x;

	switch(rect_value)
	{
	case RECT_MOUSE_SPEED:
		m_value_mouse_speed = _x*MAX_MOUSE_SPEED/m_rt_value[rect_value].w;
		break;

	case RECT_GAMMA:
		m_value_gamma = MIN_GAMMA_VALUE+_x*MAX_GAMMA_VALUE/m_rt_value[rect_value].w;
		break;

	case RECT_SOUND:
		m_volume_sound = _x*MAX_SOUND_VOLUME/m_rt_value[rect_value].w;
		break;

	case RECT_MUSIC:
		m_volume_music = _x*MAX_MUSIC_VOLUME/m_rt_value[rect_value].w;
		break;

	case RECT_ALPHA:
		g_pUserOption->ALPHA_DEPTH = _x*MAX_ALPHA_DEPTH/m_rt_value[rect_value].w;
		break;
	}
}

/*-----------------------------------------------------------------------------
- MouseControl
-
-----------------------------------------------------------------------------*/
bool C_VS_UI_OPTION::MouseControl(UINT message, int _x, int _y)
{
	static VALUE_RECT value = RECT_MOUSE_SPEED;

	if(false == m_IsTitle)
		_x -=m_vampire_plus_x; _y-=m_vampire_plus_y;
	
	//Window::MouseControl(message, _x, _y);
	_x -=x; _y-=y;
	
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	switch(m_i_selected_tab)
	{
	case TAB_CONTROL:
		re &= m_pC_control_button_group->MouseControl(message, _x, _y);
		re &= m_pC_scroll_bar->MouseControl(message, _x, _y);
		break;

	case TAB_GRAPHIC:
		re &= m_pC_graphic_button_group->MouseControl(message, _x, _y);
		break;

	case TAB_SOUND:
		re &= m_pC_sound_button_group->MouseControl(message, _x, _y);
		break;

	case TAB_GAME:
		re &= m_pC_game_button_group->MouseControl(message, _x, _y);
		break;
	}

	switch (message)
	{
		case M_MOVING :
			{
				m_focus_hotkey = -1;
				const int accel_gap = 15;
				int TitleOffset = 0;
				if(true == m_IsTitle)
					TitleOffset = -70;
				if(m_i_selected_tab == TAB_CONTROL && _x > 125 +TitleOffset && _x < 368+TitleOffset && _y > 100 && _y < 102+accel_gap*7)
				{
					m_focus_hotkey = (_y - 100)/accel_gap;
					if(m_focus_hotkey > 6)
					{
						m_focus_hotkey = -1;
						break;
					}
					m_focus_hotkey += m_pC_scroll_bar->GetScrollPos();
				}
			}
			if(m_bLBPush)
			{
				SetValue(_x, value);
			}
		break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
//			if(m_i_selected_tab == TAB_CONTROL && m_check[CHECK_MOUSE_SPEED] && m_rt_value[RECT_MOUSE_SPEED].IsInRect(_x, _y))
//			{
//				m_bLBPush = true;
//				value = RECT_MOUSE_SPEED;
//				SetValue(_x, value);
//			}
//			else 
			if(m_i_selected_tab == TAB_CONTROL && m_focus_hotkey != -1)
			{
				gC_vs_ui.SetAccelMode(ACCEL_NULL+14+m_focus_hotkey);
			}
			if(m_i_selected_tab == TAB_GRAPHIC && (m_check[CHECK_GAMMA] && m_rt_value[RECT_GAMMA].IsInRect(_x, _y) || m_check[CHECK_ALPHA_DEPTH] && m_rt_value[RECT_ALPHA].IsInRect(_x, _y)))
			{
				m_bLBPush = true;
				if(m_rt_value[RECT_GAMMA].IsInRect(_x, _y))
					value = RECT_GAMMA;
				else
					value = RECT_ALPHA;
				SetValue(_x, value);
			}
			else if(m_i_selected_tab == TAB_SOUND && (m_rt_value[RECT_SOUND].IsInRect(_x, _y) && m_check[CHECK_SOUND] || m_rt_value[RECT_MUSIC].IsInRect(_x, _y) && m_check[CHECK_MUSIC]))
			{
				m_bLBPush = true;
				if(m_rt_value[RECT_SOUND].IsInRect(_x, _y))
					value = RECT_SOUND;
				else
					value = RECT_MUSIC;
				SetValue(_x, value);
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
			if(m_bLBPush)
			{
				m_bLBPush = false;
				switch(value)
				{
				case RECT_MOUSE_SPEED:
					gpC_base->SendMessage( UI_CHANGE_OPTION, CHECK_VALUE_MOUSE_SPEED, m_value_mouse_speed );
					break;

				case RECT_GAMMA:
					if(CDirectDraw::IsSupportGammaControl() && m_check[CHECK_GAMMA])
						CDirectDraw::SetGammaRamp(m_value_gamma);
					g_pUserOption->GammaValue = m_value_gamma;
					break;

				case RECT_SOUND:
					gpC_base->SendMessage( UI_CHANGE_OPTION, CHECK_VALUE_SOUND_VOLUME, m_volume_sound );
					break;

				case RECT_MUSIC:
					gpC_base->SendMessage( UI_CHANGE_OPTION, CHECK_VALUE_MUSIC_VOLUME, m_volume_music );
					break;

				case RECT_ALPHA:
					break;
				}
			}
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

/*-----------------------------------------------------------------------------
- KeyboardControl
-
-----------------------------------------------------------------------------*/
void C_VS_UI_OPTION::KeyboardControl(UINT message, UINT key, long extra)
{
	// 만약 IME toggle되어 있을 경우를 대비하여 scan code로 한다.

	if (message == WM_KEYUP)
	{	
//		if(gC_vs_ui.IsAccelMode() && key==VK_ESCAPE)
//			gC_vs_ui.UnSetAccelMode();
//		else
		if (!gC_vs_ui.IsAccelMode() && !gC_vs_ui.IsInputHotkey() && key==VK_ESCAPE)
		{
			gpC_base->SendMessage( UI_CLOSE_OPTION );
		}
	}
}

/*-----------------------------------------------------------------------------
- Show
-
-----------------------------------------------------------------------------*/
void C_VS_UI_OPTION::Show()
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

	const InterfaceInformation* pSkin = &g_pSkinManager->Get( SkinManager::OPTION );

	int i = 0;
//	int tab_x[4] = {pSkin->GetRect(0).left, pSkin->GetRect(0).top, pSkin->GetRect(0).right, pSkin->GetRect(0).bottom}, tab_y = 20;
	
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_main_spk->BltLocked(x, y);
//		if(false == m_IsTitle)
//			m_pC_main_spk->BltLocked(x+tab_x[m_i_selected_tab-TAB_CONTROL]+m_vampire_plus_x, y+tab_y+m_vampire_plus_y, m_i_selected_tab);
//		else
			m_pC_main_spk->BltLocked(x+Tab_X[m_i_selected_tab-TAB_CONTROL]+m_vampire_plus_x, y+Tab_Y+m_vampire_plus_y, m_i_selected_tab);
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	switch(m_i_selected_tab)
	{
	case TAB_CONTROL:
		{
			int TitleOffset = 0;
			if(m_IsTitle)
				TitleOffset = -70;
			const std::string check_string[CHECK_CONTROL_MAX] =
			{
//				"Mouse Speed",
				"Logitech IFeel Mouse Force Feedback",
			};

			if(gpC_base->m_p_DDSurface_back->Lock())
			{
				m_pC_main_spk->BltLocked(x+m_vampire_plus_x+125+TitleOffset, y+m_vampire_plus_y+80, HOTKEY_WINDOW);
				m_pC_control_button_group->Show();
				gpC_base->m_p_DDSurface_back->Unlock();
			}
			
			g_FL2_GetDC();
//			g_PrintColorStr(x+m_vampire_plus_x+130, y+m_vampire_plus_y+56, "Input Style : ", gpC_base->m_user_id_pi, RGB_BLACK);
			g_PrintColorStr(x+m_vampire_plus_x+m_check_x+30, y+m_vampire_plus_y+m_check_y, 
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_NORMAL_CHATTING].GetString(), gpC_base->m_user_id_pi, RGB_BLACK);
			
			g_PrintColorStr(x+m_vampire_plus_x+m_check_x+140, y+m_vampire_plus_y+m_check_y, 
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_ENTER_CHATTING].GetString(), gpC_base->m_user_id_pi, RGB_BLACK);

//			m_pC_etc_spk->Blt(x+m_vampire_plus_x+230, y+m_vampire_plus_y+53, HOTKEY_BACK);
//			m_pC_etc_spk->Blt(x+m_vampire_plus_x+230+m_pC_etc_spk->GetWidth(HOTKEY_BACK), y+m_vampire_plus_y+53, HOTKEY_BACK_RIGHT);
			
			for(i = 0; i < CHECK_CONTROL_MAX; i++)
				g_PrintColorStr(x+m_vampire_plus_x+m_check_x+15, y+m_vampire_plus_y+m_check_y+m_check_gap*(8+i), check_string[i].c_str(), gpC_base->m_user_id_pi, RGB_BLACK);
			
//			m_pC_etc_spk->Blt(x+m_vampire_plus_x+m_rt_value[RECT_MOUSE_SPEED].x, y+m_vampire_plus_y+m_rt_value[RECT_MOUSE_SPEED].y+5, VOLUME_BAR);
//			if(m_check[CHECK_MOUSE_SPEED])
//				m_pC_etc_spk->Blt(x+m_vampire_plus_x+m_rt_value[RECT_MOUSE_SPEED].x-m_pC_etc_spk->GetWidth(VOLUME_TAG)/2+m_value_mouse_speed*m_rt_value[RECT_MOUSE_SPEED].w/MAX_MOUSE_SPEED, y+m_vampire_plus_y+m_rt_value[RECT_MOUSE_SPEED].y, VOLUME_TAG);
			
			// Accelator
			const int accel_count = MAX_ACCELERATOR;
			const int accel_gap = 15;
			for(i = 0; i < min(accel_count, 7); i++)
			{
				const BYTE accel = ACCEL_NULL+i+14+m_pC_scroll_bar->GetScrollPos();
				const char *pAccelName = g_pKeyAccelerator->GetName(accel);
				g_PrintColorStr(x+m_vampire_plus_x+127+TitleOffset, y+m_vampire_plus_y+80+22+accel_gap*i, pAccelName, gpC_base->m_chatting_pi, RGB_WHITE);
				WORD key = g_pKeyAccelerator->GetKey(accel);
				int px = 0;
				if(ACCEL_HAS_CONTROL(key))
					px = g_PrintColorStr(x+m_vampire_plus_x+127+100+px+TitleOffset, y+m_vampire_plus_y+80+22+accel_gap*i, "Ctrl", gpC_base->m_chatting_pi, RGB_WHITE);
				if(ACCEL_HAS_ALT(key))
				{
					if(px != 0)
					{
						px = g_PrintColorStr(px, y+m_vampire_plus_y+80+22+accel_gap*i, "+", gpC_base->m_chatting_pi, RGB_WHITE);
						px = g_PrintColorStr(px, y+m_vampire_plus_y+80+22+accel_gap*i, "Alt", gpC_base->m_chatting_pi, RGB_WHITE);
					}
					else
						px = g_PrintColorStr(x+m_vampire_plus_x+127+100+TitleOffset, y+m_vampire_plus_y+80+22+accel_gap*i, "Alt", gpC_base->m_chatting_pi, RGB_WHITE);
				}
				if(ACCEL_HAS_SHIFT(key))
				{
					if(px != 0)
					{
						px = g_PrintColorStr(px, y+m_vampire_plus_y+80+22+accel_gap*i, "+", gpC_base->m_chatting_pi, RGB_WHITE);
						px = g_PrintColorStr(px, y+m_vampire_plus_y+80+22+accel_gap*i, "Shift", gpC_base->m_chatting_pi, RGB_WHITE);
					}
					else
						px = g_PrintColorStr(x+m_vampire_plus_x+127+100+TitleOffset, y+m_vampire_plus_y+80+22+accel_gap*i, "Shift", gpC_base->m_chatting_pi, RGB_WHITE);
				}
				if(px != 0)
				{
					px = g_PrintColorStr(px, y+m_vampire_plus_y+80+22+accel_gap*i, "+", gpC_base->m_chatting_pi, RGB_WHITE);
					px = g_PrintColorStr(px, y+m_vampire_plus_y+80+22+accel_gap*i, scancode_name[ACCEL_GET_KEY(key)], gpC_base->m_chatting_pi, RGB_WHITE);
				}
				else
					px = g_PrintColorStr(x+m_vampire_plus_x+127+100+TitleOffset, y+m_vampire_plus_y+80+22+accel_gap*i, scancode_name[ACCEL_GET_KEY(key)], gpC_base->m_chatting_pi, RGB_WHITE);
							
				//				char sz_temp[20];
				//				wsprintf(sz_temp, "%d", m_focus_hotkey);
				//				g_PrintColorStr(x+m_vampire_plus_x+127+100, y+m_vampire_plus_y+80, sz_temp, gpC_base->m_chatting_pi, RGB_WHITE);
				
			}	
			g_FL2_ReleaseDC();
			if(false == m_IsTitle)
				m_pC_scroll_bar->Show(x+m_vampire_plus_x, y+m_vampire_plus_y);
			else
				m_pC_scroll_bar->Show3(x+m_vampire_plus_x+TitleOffset, y+m_vampire_plus_y);
		}
		break;

	case TAB_GRAPHIC:
		{
			const char* check_string[CHECK_GRAPHIC_MAX] =
			{
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_3D_ACCEL].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_ALPHA_HPBAR].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_SHED_BLOOD].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_HIDE_SOFT].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_GAME_BRIGHT].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_CHATTING_TALK].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_PUT_FPS].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_WINDOW_ALPHA].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_DENSITY_ALPHA].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_DO_NOT_SHOW_PERSNALSHOP_MSG].GetString(),				
				
			};
			
			g_FL2_GetDC();
			for(i = 0; i < CHECK_GRAPHIC_MAX; i++)
				g_PrintColorStr(x+m_vampire_plus_x+m_check_x+15, y+m_vampire_plus_y+m_check_y+m_check_gap*i, check_string[i], gpC_base->m_user_id_pi, RGB_BLACK);
			g_FL2_ReleaseDC();

			if(gpC_base->m_p_DDSurface_back->Lock())
			{
				if(false == m_IsTitle)
				{
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_GAMMA].x, y+m_vampire_plus_y+m_rt_value[RECT_GAMMA].y+5, VOLUME_BAR);
					if(m_check[CHECK_GAMMA])
						m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_GAMMA].x-m_pC_etc_spk->GetWidth(VOLUME_TAG)/2+(m_value_gamma-MIN_GAMMA_VALUE)*m_rt_value[RECT_GAMMA].w/MAX_GAMMA_VALUE, y+m_vampire_plus_y+m_rt_value[RECT_GAMMA].y, VOLUME_TAG);
					
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_ALPHA].x, y+m_vampire_plus_y+m_rt_value[RECT_ALPHA].y+5, VOLUME_BAR);
					if(m_check[CHECK_ALPHA_DEPTH])
						m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_ALPHA].x-m_pC_etc_spk->GetWidth(VOLUME_TAG)/2+(g_pUserOption->ALPHA_DEPTH)*m_rt_value[RECT_ALPHA].w/MAX_ALPHA_DEPTH, y+m_vampire_plus_y+m_rt_value[RECT_ALPHA].y, VOLUME_TAG);
				}
				else
				{
					m_pC_main_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_GAMMA].x, y+m_vampire_plus_y+m_rt_value[RECT_GAMMA].y+5, TITLE_VOLUME_BAR);
					if(m_check[CHECK_GAMMA])
						m_pC_main_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_GAMMA].x-m_pC_main_spk->GetWidth(TITLE_VOLUME_TAG)/2+(m_value_gamma-MIN_GAMMA_VALUE)*m_rt_value[RECT_GAMMA].w/MAX_GAMMA_VALUE, y+m_vampire_plus_y+m_rt_value[RECT_GAMMA].y, TITLE_VOLUME_TAG);
					
					m_pC_main_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_ALPHA].x, y+m_vampire_plus_y+m_rt_value[RECT_ALPHA].y+5, TITLE_VOLUME_BAR);
					if(m_check[CHECK_ALPHA_DEPTH])
						m_pC_main_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_ALPHA].x-m_pC_main_spk->GetWidth(TITLE_VOLUME_TAG)/2+(g_pUserOption->ALPHA_DEPTH)*m_rt_value[RECT_ALPHA].w/MAX_ALPHA_DEPTH, y+m_vampire_plus_y+m_rt_value[RECT_ALPHA].y, TITLE_VOLUME_TAG);
				}
				m_pC_graphic_button_group->Show();
				gpC_base->m_p_DDSurface_back->Unlock();
			}
		}
		break;

	case TAB_SOUND:
		{
			const char* check_string[CHECK_SOUND_MAX] =
			{
//				"기합 소리 듣기",
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_SOUND_VOLUME].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_MUSIC_VOLUME].GetString(),
			};

			g_FL2_GetDC();
			for(i = 0; i < CHECK_SOUND_MAX; i++)
				g_PrintColorStr(x+m_vampire_plus_x+m_check_x+15, y+m_vampire_plus_y+m_check_y+m_check_gap*i, check_string[i], gpC_base->m_user_id_pi, RGB_BLACK);
//			g_PrintColorStr(x+m_vampire_plus_x+m_check_x+80, y+m_vampire_plus_y+m_check_y+m_check_gap*i, "WAV", gpC_base->m_user_id_pi, RGB_BLACK);
//			g_PrintColorStr(x+m_vampire_plus_x+m_check_x+140, y+m_vampire_plus_y+m_check_y+m_check_gap*i, "MIDI", gpC_base->m_user_id_pi, RGB_BLACK);
			g_FL2_ReleaseDC();

			if(gpC_base->m_p_DDSurface_back->Lock())
			{
				if(false == m_IsTitle)
				{
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_SOUND].x, y+m_vampire_plus_y+m_rt_value[RECT_SOUND].y+5, VOLUME_BAR);
					m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_MUSIC].x, y+m_vampire_plus_y+m_rt_value[RECT_MUSIC].y+5, VOLUME_BAR);
					if(m_check[CHECK_SOUND])
						m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_SOUND].x-m_pC_etc_spk->GetWidth(VOLUME_TAG)/2+m_volume_sound*m_rt_value[RECT_SOUND].w/MAX_SOUND_VOLUME, y+m_vampire_plus_y+m_rt_value[RECT_SOUND].y, VOLUME_TAG);
					if(m_check[CHECK_MUSIC])
						m_pC_etc_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_MUSIC].x-m_pC_etc_spk->GetWidth(VOLUME_TAG)/2+m_volume_music*m_rt_value[RECT_MUSIC].w/MAX_MUSIC_VOLUME, y+m_vampire_plus_y+m_rt_value[RECT_MUSIC].y, VOLUME_TAG);
				}
				else
				{
					m_pC_main_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_SOUND].x, y+m_vampire_plus_y+m_rt_value[RECT_SOUND].y+5, TITLE_VOLUME_BAR);
					m_pC_main_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_MUSIC].x, y+m_vampire_plus_y+m_rt_value[RECT_MUSIC].y+5, TITLE_VOLUME_BAR);
					if(m_check[CHECK_SOUND])
						m_pC_main_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_SOUND].x-m_pC_main_spk->GetWidth(TITLE_VOLUME_TAG)/2+m_volume_sound*m_rt_value[RECT_SOUND].w/MAX_SOUND_VOLUME, y+m_vampire_plus_y+m_rt_value[RECT_SOUND].y, TITLE_VOLUME_TAG);
					if(m_check[CHECK_MUSIC])
						m_pC_main_spk->BltLocked(x+m_vampire_plus_x+m_rt_value[RECT_MUSIC].x-m_pC_main_spk->GetWidth(TITLE_VOLUME_TAG)/2+m_volume_music*m_rt_value[RECT_MUSIC].w/MAX_MUSIC_VOLUME, y+m_vampire_plus_y+m_rt_value[RECT_MUSIC].y, TITLE_VOLUME_TAG);
				}
				m_pC_sound_button_group->Show();
				gpC_base->m_p_DDSurface_back->Unlock();			
			}			
		}
		break;

	case TAB_GAME:
		{
			const char* check_string[CHECK_GAME_MAX] =
			{
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_SHOW_BASIC_HELP].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_NO_LISTEN_BAD_TALK].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_LOAD_ALL_IMAGE].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_CHATTING_COLOR_WHITE].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_RUN_TEEN_VERSION].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_OPEN_WINDOW_WHEN_WHISPER].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_DO_NOT_WAR_MSG].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_DO_NOT_LAIR_MSG].GetString(),
				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_DO_NOT_HOLY_LAND_MSG].GetString(),				
				(*g_pGameStringTable)[UI_STRING_MESSAGE_GAMEMONEY_WITH_HANGUL].GetString(),				
				
//				(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_NOT_SEND_MY_INFO].GetString(),
/*				"초보자용 도움말 보기",
				"나쁜말 듣지 않기",
//				"종족 변하게 하기",
//				"파티 초대 거부",
//				"파티 가입 신청 거부",
				"맵이동할 때 몬스터 이미지 불러오기",
				"채팅 모두 흰색으로 보기",
				"틴버젼으로 보기",
				"귓속말 왔을때 채팅창 띄우기",*/
			};
			
			g_FL2_GetDC();
			for(i = 0; i < CHECK_GAME_MAX; i++)
				g_PrintColorStr(x+m_vampire_plus_x+m_check_x+15, y+m_vampire_plus_y+m_check_y+m_check_gap*i, check_string[i], gpC_base->m_user_id_pi, RGB_BLACK);
			g_FL2_ReleaseDC();		
		}
		break;
	}
	
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_button_group->Show();
		if(m_i_selected_tab==TAB_GAME)
			m_pC_game_button_group->Show();

		//if(true == m_IsTitle)
			//add by zdj
//			DrawTitleEffect();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	

	if(gC_vs_ui.IsAccelMode())
	{
		Rect rect;
		rect.w = 330;
		rect.h = 110;
		rect.x = g_GameRect.right/2-rect.w/2;
		rect.y = g_GameRect.bottom/2-rect.h/2;
		gpC_global_resource->DrawDialog(rect, g_pUserOption->DefaultAlpha != FALSE);
		
		const BYTE accel = gC_vs_ui.GetAccelKey();
		const char *pAccelName = g_pKeyAccelerator->GetName(accel);
		int px = rect.x+30;
		int py = rect.y+30;
		
		g_FL2_GetDC();
		px = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_ACCEL_NAME].GetString(), gpC_base->m_chatting_pi, RGB_WHITE);
		g_PrintColorStr(px, py, pAccelName, gpC_base->m_chatting_pi, RGB_WHITE);
		WORD key = g_pKeyAccelerator->GetKey(accel);
		px = rect.x+30+150;
		px = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_ACCEL_KEY].GetString(), gpC_base->m_chatting_pi, RGB_WHITE);
		int old_px = px;
		if(ACCEL_HAS_CONTROL(key))
			px = g_PrintColorStr(px, py, "Ctrl", gpC_base->m_chatting_pi, RGB_WHITE);
		if(ACCEL_HAS_ALT(key))
		{
			if(px != old_px)
			{
				px = g_PrintColorStr(px, py, "+", gpC_base->m_chatting_pi, RGB_WHITE);
				px = g_PrintColorStr(px, py, "Alt", gpC_base->m_chatting_pi, RGB_WHITE);
			}
			else
				px = g_PrintColorStr(px, py, "Alt", gpC_base->m_chatting_pi, RGB_WHITE);
		}
		if(ACCEL_HAS_SHIFT(key))
		{
			if(px != old_px)
			{
				px = g_PrintColorStr(px, py, "+", gpC_base->m_chatting_pi, RGB_WHITE);
				px = g_PrintColorStr(px, py, "Shift", gpC_base->m_chatting_pi, RGB_WHITE);
			}
			else
				px = g_PrintColorStr(px, py, "Shift", gpC_base->m_chatting_pi, RGB_WHITE);
		}
		if(px != old_px)
		{
			px = g_PrintColorStr(px, py, "+", gpC_base->m_chatting_pi, RGB_WHITE);
			px = g_PrintColorStr(px, py, scancode_name[ACCEL_GET_KEY(key)], gpC_base->m_chatting_pi, RGB_WHITE);
		}
		else
			px = g_PrintColorStr(px, py, scancode_name[ACCEL_GET_KEY(key)], gpC_base->m_chatting_pi, RGB_WHITE);
	
		g_PrintColorStr(rect.x+30, rect.y+50, (*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_MSG1].GetString(), gpC_base->m_chatting_pi, RGB_WHITE);
		g_PrintColorStr(rect.x+30, rect.y+70,(*g_pGameStringTable)[UI_STRING_MESSAGE_OPTION_MENU_MSG2].GetString(), gpC_base->m_chatting_pi, RGB_WHITE);
		g_FL2_ReleaseDC();
	}
	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// AcquireFirstSequence
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OPTION::AcquireFirstSequence()
{
}

//-----------------------------------------------------------------------------
// UnacquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OPTION::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
	m_pC_control_button_group->UnacquireMouseFocus();
	m_pC_graphic_button_group->UnacquireMouseFocus();
	m_pC_sound_button_group->UnacquireMouseFocus();
	m_pC_game_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_OPTION::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_OPTION::IsPixel(int _x, int _y)
{
//	return m_image_spk.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
	if(m_pC_main_spk)
	{
		return m_pC_main_spk->IsPixel(_x-x, _y-y);
	}
	return false;
}


//-----------------------------------------------------------------------------
// C_VS_UI_GO_BILING_PAGE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_GO_BILING_PAGE::C_VS_UI_GO_BILING_PAGE(BILING_MSG_LIST msg)
{
	g_RegisterWindow(this);

	AttrTopmost(true);
	AttrKeyboardControl(true);
	m_biling_mode = msg;

	// 넷마블용
	int w_w = 350;
	int w_h = 200;

	int cancel_offset_x, cancel_offset_y;
	int ok_offset_x, ok_offset_y;

	ok_offset_x = w_w - 150;
	ok_offset_y = w_h-60;
	cancel_offset_x = w_w - 75;
	cancel_offset_y = w_h-60;

	
	Set(g_GameRect.right/2-w_w/2, g_GameRect.bottom/2-w_h/2, w_w, w_h);

	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+ok_offset_x, y+ok_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_OK), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_OK), BILING_OK, this, C_GLOBAL_RESOURCE::AB_BUTTON_OK));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(x+cancel_offset_x, y+cancel_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_OK), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_OK), BILING_CANCEL, this, C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL));

}

//-----------------------------------------------------------------------------
// ~C_VS_UI_GO_BILING_PAGE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_GO_BILING_PAGE::~C_VS_UI_GO_BILING_PAGE()
{
	g_UnregisterWindow(this);

	DeleteNew(m_pC_button_group);
}

void	C_VS_UI_GO_BILING_PAGE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char * m_help_string[2] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_GO_BILING_PAGE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_CANCEL].GetString()
	};

	g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);
}

//-----------------------------------------------------------------------------
// C_VS_UI_GO_BILING_PAGE::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_GO_BILING_PAGE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
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

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GO_BILING_PAGE::WindowEventReceiver(id_t event)
{
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_GO_BILING_PAGE::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);//m_pC_image_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GO_BILING_PAGE::Start()
{
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);

	m_pC_button_group->Init();
}

void C_VS_UI_GO_BILING_PAGE::Finish()
{
	PI_Processor::Finish();

	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GO_BILING_PAGE::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GO_BILING_PAGE::Show()
{
	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);
	
	g_FL2_GetDC();
	std::string str;

	switch(m_biling_mode)
	{
	case CANNOT_PLAY_MSG :
		str = (*g_pGameStringTable)[STRING_MESSAGE_SELECT_PC_CANNOT_PLAY].GetString();
		str += "\n";
		str += (*g_pGameStringTable)[UI_STRING_MESSAGE_WILL_YOU_GO_BILING_PAGE].GetString();
		break;
	case CANNOT_PLAY_BY_ATTR_MSG :
		str = (*g_pGameStringTable)[STRING_MESSAGE_SELECT_PC_CANNOT_PLAY_BY_ATTR].GetString();
		str += "\n";
		str += (*g_pGameStringTable)[UI_STRING_MESSAGE_WILL_YOU_GO_BILING_PAGE].GetString();
		break;
	default :
		str = "";
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
	m_pC_button_group->ShowDescription();
	
	g_FL2_ReleaseDC();

	m_pC_button_group->Show();

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GO_BILING_PAGE::Run(id_t id)
{
	switch (id)
	{
		case BILING_OK:			
			gpC_base->SendMessage(UI_GO_BILING_PAGE, TRUE);
			Finish();
			break;

		case BILING_CANCEL:
			Finish();
			break;
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_GO_BILING_PAGE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	m_pC_button_group->MouseControl(message, _x, _y);

	return true;
}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_GO_BILING_PAGE::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
			case VK_ESCAPE:
				Run(BILING_CANCEL);
				return;

			case VK_RETURN:
				Run(BILING_OK);
				return;
		}
	}
}


