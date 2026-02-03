// VS_UI_Base.cpp

#include "client_PCH.h"
#define assert(e) ((void)(e))
// Disabled assert for macOS
#include "VS_UI_Base.h"
// GDI removed (SDL2) - All platforms use TextSystem (SDL + freetype2)
#include "../../basic/Platform.h"
#include "TextSystem/FontHandleUtil.h"
extern RECT g_GameRect;
//----------------------------------------------------------------------------
// Globals
//----------------------------------------------------------------------------
Base * gpC_base;

//----------------------------------------------------------------------------
// Operations
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Base
//
// 
//-----------------------------------------------------------------------------
Base::Base()
{
	m_event_occured = false;
	m_p_DDSurface_back = NULL;

	m_chat_dialog_pi.hfont = NULL;

//	m_slayer_font	= C_FONT_MANAGER::NO_GID;
//	m_vampire_font = C_FONT_MANAGER::NO_GID;
//	m_bbs_font		= C_FONT_MANAGER::NO_GID;
//	m_item_font		= C_FONT_MANAGER::NO_GID;
//	m_normal_font	= C_FONT_MANAGER::NO_GID;
}

//-----------------------------------------------------------------------------
// ~Base
//
// GDI removed (SDL2) - TextSystem manages fonts automatically, no manual cleanup needed
//-----------------------------------------------------------------------------
Base::~Base()
{
	// TextSystem manages font lifetimes automatically
	// No need to delete font handles
}

//-----------------------------------------------------------------------------
// SetFont
//
// GDI removed (SDL2) - All platforms use TextSystem
//-----------------------------------------------------------------------------
void Base::SetFont(PrintInfo &pi, LOGFONT &lf, COLORREF textcolor, COLORREF backcolor, int bk_mode, int align)
{
	// GDI removed: CreateFontIndirect() was used on Windows
	// Now using TextSystem for all platforms
	pi.hfont = TextSystem::EncodeFontSizeHandle(lf.lfHeight);
	pi.text_color = textcolor;
	pi.back_color = backcolor;
	pi.bk_mode = bk_mode;
	pi.text_align = align;
}

//-----------------------------------------------------------------------------
// SetDefaultLogfont
//
// GDI removed (SDL2) - Simplified for TextSystem (all platforms)
//-----------------------------------------------------------------------------
void Base::SetDefaultLogfont(LOGFONT &lf) const
{
	// Set default LOGFONT values for TextSystem
	lf.lfHeight = 0;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = 0;
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
	strcpy(lf.lfFaceName, "Arial");
}

//-----------------------------------------------------------------------------
// SelectFont
//
// 
//-----------------------------------------------------------------------------
/*
void Base::SelectFont(font_id_t id)
{
	switch (id)
	{
		case FONT_SLAYER:
			gC_font.SelectFont(m_slayer_font);
			gC_font.NotUseNoPublicEng();
			break;

		case FONT_VAMPIRE:
			gC_font.SelectFont(m_vampire_font);
			gC_font.NotUseNoPublicEng();
			break;

		case FONT_BBS:
			gC_font.SelectFont(m_bbs_font);
			gC_font.NotUseNoPublicEng();
			break;

		case FONT_ITEM:
			gC_font.SelectFont(m_normal_font);
			gC_font.UseNoPublicEng(OLIVE);
			break;

		case FONT_ITEM_SMALL:
			gC_font.SelectFont(m_item_font);
			gC_font.NotUseNoPublicEng();
			break;

		case FONT_NORMAL:
			gC_font.SelectFont(m_normal_font);
			gC_font.NotUseNoPublicEng();
			break;
	}
}*/

//-----------------------------------------------------------------------------
// Init
//
// 
//-----------------------------------------------------------------------------
void Base::Init(CSpriteSurface *surface, void (*fp)(DWORD, int, int, void *))
{
	InitSurface(surface);
	InitFont();
	m_C_ui_result_receiver.SetResultReceiver(fp);
	gC_ci->RunCursorBlinker();
}

//-----------------------------------------------------------------------------
// InitFont
//
// 
//-----------------------------------------------------------------------------
void Base::InitFont()
{
	LOGFONT lf;

	// 
	// ������ style
	//
	// - ����, 13
	//
	// !Times New Romans�� ���� ����. OS�� ��ġ �ȸ´� ���� �߻���.
	//
	const char szFontName[4][2][20] = {
		// Hangul Font      Chinese Font
		{ "����ü",			"����" },
		{ "MS Sans Serif",	"MS Sans Serif" },
		{ "����ü",			"������" },
		{ "����",			"����" }
	};

	char Language;
	
	if(gC_ci->IsChinese())
		Language = 1;
	else 
		Language = 0;
	
	SetDefaultLogfont(lf); //by larosel
	lf.lfHeight = 10;
	strcpy(lf.lfFaceName, szFontName[0][Language]);
	SetFont(m_small_pi, lf, RGB(20, 70, 0));

	SetDefaultLogfont(lf); //by larosel
	lf.lfHeight = 12;
	strcpy(lf.lfFaceName, szFontName[0][Language]);
	SetFont(m_chatting_pi, lf, RGB(20, 70, 0));

	SetDefaultLogfont(lf); //by larosel
	lf.lfHeight = 12;
	lf.lfWeight = FW_BOLD;
	strcpy(lf.lfFaceName, szFontName[0][Language]);
	SetFont(m_user_id_pi, lf, RGB(20, 70, 0));

	// new style...
	SetDefaultLogfont(lf);
	lf.lfHeight = 14;
	lf.lfItalic = 1;
	lf.lfWeight = FW_BOLD;
	strcpy(lf.lfFaceName, szFontName[1][Language]);
	SetFont(m_value_pi, lf, RGB(255, 255, 255), 0, TRANSPARENT, TA_RIGHT);

	// new style...
	SetDefaultLogfont(lf);
	lf.lfHeight = 14;
	lf.lfItalic = 1;
	lf.lfWeight = FW_BOLD;
	strcpy(lf.lfFaceName, szFontName[1][Language]);
	SetFont(m_value2_pi, lf, RGB(20, 70, 0));

	// new style...
	SetDefaultLogfont(lf);
	lf.lfHeight = 14;
	lf.lfWeight = FW_BOLD;
	strcpy(lf.lfFaceName, szFontName[2][Language]);
	SetFont(m_item_name_pi, lf, RGB(255, 255, 255));

	// new style...
	SetDefaultLogfont(lf);
	lf.lfHeight = 12;
	strcpy(lf.lfFaceName, szFontName[0][Language]);
	SetFont(m_item_desc_pi, lf, RGB(192, 192, 255));

	// new style...
	SetDefaultLogfont(lf);
	lf.lfHeight = 14;
//	lf.lfWeight = FW_BOLD;
	strcpy(lf.lfFaceName, szFontName[0][Language]);
	SetFont(m_dialog_menu_pi, lf, RGB(255, 255, 255));

	// new style...
	SetDefaultLogfont(lf);
	strcpy(lf.lfFaceName, szFontName[0][Language]);
	lf.lfHeight = 13;
	SetFont(m_dialog_msg_pi, lf, RGB(255, 255, 255));

	// new style...
	SetDefaultLogfont(lf);
	lf.lfHeight = 14;
	lf.lfWeight = FW_BOLD;
	strcpy(lf.lfFaceName, szFontName[0][Language]);
	SetFont(m_desc_menu_pi, lf, RGB(255, 255, 255));

	// new style...
	SetDefaultLogfont(lf);
	strcpy(lf.lfFaceName, szFontName[0][Language]);
	lf.lfHeight = 14;
	SetFont(m_desc_msg_pi, lf, RGB(255, 255, 255));

	// new style...
	SetDefaultLogfont(lf);
	lf.lfHeight = 16;
	strcpy(lf.lfFaceName, szFontName[0][Language]);
	SetFont(m_money_pi, lf, RGB(255, 255, 255));

	// new style...
	SetDefaultLogfont(lf);
	lf.lfHeight = 16;
	lf.lfWeight = FW_BOLD;
	strcpy(lf.lfFaceName, szFontName[0][Language]);
	SetFont(m_char_value_pi, lf, RGB(255, 255, 255));

	//
	//font used by Client by larosel ���� ��Ʈ���� 2�� ũ�� �������.
	//
	//SetDefaultLogfont(lf);
	//lf.lfHeight = 12;
	//strcpy(lf.lfFaceName, "����");
	//SetFont(m_chat_dialog_pi, lf, RGB(255, 255, 255));

	SetDefaultLogfont(lf);
	lf.lfHeight = 14;
	lf.lfWeight = FW_BOLD;
	strcpy(lf.lfFaceName, szFontName[3][Language]);
	SetFont(m_info_pi, lf, RGB(255, 255, 255));

	SetDefaultLogfont(lf);
	lf.lfHeight = 12;
	strcpy(lf.lfFaceName, szFontName[3][Language]);
	SetFont(m_item_pi, lf, RGB(255, 255, 255));

	SetDefaultLogfont(lf);
	lf.lfHeight = 13;
	lf.lfWeight = FW_BOLD;
	strcpy(lf.lfFaceName, szFontName[3][Language]);
	SetFont(m_char_name_pi, lf, RGB(255, 255, 255));

	SetDefaultLogfont(lf);
	lf.lfHeight = 13;
	strcpy(lf.lfFaceName, szFontName[3][Language]);
	SetFont(m_char_chat_pi, lf, RGB(255, 255, 255));

	//party
	SetDefaultLogfont(lf); //by larosel
	lf.lfHeight = 12;
//	lf.lfWeight = FW_BOLD;
	strcpy(lf.lfFaceName, szFontName[3][Language]);
	SetFont(m_party_name_pi, lf, RGB(20, 70, 0));

	//xmas
	SetDefaultLogfont(lf); //by larosel
	lf.lfHeight = 10;
//	lf.lfWeight = FW_BOLD;
	lf.lfItalic = true;
	strcpy(lf.lfFaceName, szFontName[3][Language]);
	SetFont(m_xmas_pi, lf, RGB(20, 70, 0));

	SetDefaultLogfont(lf);
	lf.lfHeight = 16;
	lf.lfWeight = FW_BOLD;
	strcpy(lf.lfFaceName, szFontName[3][Language]);
	SetFont( m_char_chat_large_pi, lf, RGB(255,255,255));

	// new style...
	SetDefaultLogfont(lf);
	lf.lfHeight = 16;
	strcpy(lf.lfFaceName, szFontName[0][Language]);
	SetFont(m_money2_pi, lf, RGB(255, 255, 255), 0, TRANSPARENT, TA_RIGHT);

	
}

//-----------------------------------------------------------------------------
// InitSurface
//
// 
//-----------------------------------------------------------------------------
void Base::InitSurface(CSpriteSurface *surface)
{
	assert(surface);
	m_p_DDSurface_back = surface;

	bool ret = m_DDSurface_offscreen.InitOffsurface(g_GameRect.right, g_GameRect.bottom);
	if (!ret)
		_Error(FAILED_JOB);

	//
	// offscreen�� colorkey�� �����Ѵ�. ��Ȳ�� ���� Image���� �� ���� ����
	// �����ؾ� �Ѵ�.
	//
	m_colorkey_red = RED;

	m_DDSurface_offscreen.SetTransparency(m_colorkey_red); // default colorkey = red

	// GDI removed (SDL2) - All platforms use SDL backend: use sprite surface directly for text rendering
	g_SetFL2Surface(m_p_DDSurface_back);
}

//-----------------------------------------------------------------------------
// DispatchMessage
//
// 
//-----------------------------------------------------------------------------
void Base::DispatchMessage()
{
	m_C_ui_result_receiver._DispatchMessage();
}

//-----------------------------------------------------------------------------
// SendMessage
//
// 
//-----------------------------------------------------------------------------
void Base::SendMessage(DWORD message, int left, int right, void * void_ptr)
{
	m_C_ui_result_receiver._SendMessage(message, left, right, void_ptr);
}

//-----------------------------------------------------------------------------
// StartEvent
//
// 
//-----------------------------------------------------------------------------
void	Base::StartEvent() 
{ 
	//assert(m_event_occured == false);

	m_event_occured = true; 
}

//-----------------------------------------------------------------------------
// FinishEvent
//
// 
//-----------------------------------------------------------------------------
void	Base::FinishEvent() 
{ 
	//assert(m_event_occured == true);

	m_event_occured = false; 
}
