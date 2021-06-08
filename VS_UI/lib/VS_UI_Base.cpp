// VS_UI_Base.cpp

#include "client_PCH.h"
#include "VS_UI_Base.h"
#include <windows.h>
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
// 
//-----------------------------------------------------------------------------
Base::~Base()
{
	DeleteObject(m_small_pi.hfont);
	DeleteObject(m_chatting_pi.hfont);
	DeleteObject(m_user_id_pi.hfont);
	DeleteObject(m_value_pi.hfont);
	DeleteObject(m_value2_pi.hfont);
	DeleteObject(m_item_name_pi.hfont);
	DeleteObject(m_item_desc_pi.hfont);
	DeleteObject(m_dialog_msg_pi.hfont);
	DeleteObject(m_dialog_menu_pi.hfont);
	DeleteObject(m_desc_msg_pi.hfont);
	DeleteObject(m_desc_menu_pi.hfont);
	DeleteObject(m_money_pi.hfont);
	DeleteObject(m_char_value_pi.hfont);

	if (m_chat_dialog_pi.hfont != NULL)
		DeleteObject(m_chat_dialog_pi.hfont);
	DeleteObject(m_info_pi.hfont);
	DeleteObject(m_item_pi.hfont);

	DeleteObject(m_char_name_pi.hfont);		//by larosel
	DeleteObject(m_char_chat_pi.hfont);
	DeleteObject(m_char_chat_large_pi.hfont);

	DeleteObject(m_party_name_pi.hfont);
	DeleteObject(m_xmas_pi.hfont);
	

	DeleteObject(m_money2_pi.hfont);
}

//-----------------------------------------------------------------------------
// SetFont
//
// 
//-----------------------------------------------------------------------------
void Base::SetFont(PrintInfo &pi, LOGFONT &lf, COLORREF textcolor, COLORREF backcolor, int bk_mode, int align)
{
	HFONT hfont = CreateFontIndirect(&lf);
	if (hfont == NULL)
	  _Error(FAILED_JOB);

	pi.hfont = hfont;
	pi.text_color = textcolor;
	pi.back_color = backcolor;
	pi.bk_mode = bk_mode;
	pi.text_align = align;
}

//-----------------------------------------------------------------------------
// SetDefaultLogfont
//
// 
//-----------------------------------------------------------------------------
void Base::SetDefaultLogfont(LOGFONT &lf) const
{
	lf.lfHeight = 0; // 0 = default
	lf.lfWidth = 0; // 0 = 높이에 기준하여 자동으로 설정됨.
	lf.lfEscapement = 0; // 방향설정 (900, 2700)
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL; // FW_BOLD
	lf.lfItalic = 0; // 0 아닌 값이면 italic이다.
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0;
	if(gC_ci->IsChinese())
		lf.lfCharSet = GB2312_CHARSET;//*/JOHAB_CHARSET;
	else
		lf.lfCharSet = HANGUL_CHARSET;//*/JOHAB_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
	strcpy(lf.lfFaceName, "굴림체");//"Times New Roman");
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
	// 괜찮은 style
	//
	// - 바탕, 13
	//
	// !Times New Romans로 하지 말것. OS간 위치 안맞는 문제 발생함.
	//
	const char szFontName[4][2][20] = {
		// Hangul Font      Chinese Font
		{ "굴림체",			"芥竟" },
		{ "MS Sans Serif",	"MS Sans Serif" },
		{ "돋움체",			"劤芥竟" },
		{ "굴림",			"芥竟" }
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
	//font used by Client by larosel 밖의 폰트보다 2씩 크게 해줘야함.
	//
	//SetDefaultLogfont(lf);
	//lf.lfHeight = 12;
	//strcpy(lf.lfFaceName, "굴림");
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

	bool ret = m_DDSurface_offscreen.InitOffsurface(g_GameRect.right, g_GameRect.bottom, DDSCAPS_SYSTEMMEMORY);
	if (!ret)
		_Error(FAILED_JOB);

	//
	// offscreen의 colorkey를 설정한다. 상황에 따라서 Image에서 안 쓰는 색을 
	// 결정해야 한다.
	//
	m_colorkey_red = RED;

	m_DDSurface_offscreen.SetTransparency(m_colorkey_red); // default colorkey = red

	// set Unicorn edit widget surface
	g_SetFL2Surface(m_p_DDSurface_back->GetSurface());
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