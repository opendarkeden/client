// CI.cpp

#include "client_PCH.h"
#include "DXLib.h"
#include "CI.h"
#include "Timer2.h"

extern void IME_StartComposition();
extern void IME_EndComposition();
extern void IME_Composition();
extern void IME_NextComposition();
extern void IME_Normal(UINT message, WPARAM wParam, LPARAM lParam);

CI *gC_ci;

static timer_id_t	g_tid_cursor_blink = INVALID_TID;
static bool	gbl_draw_cursor = false;
extern bool	gbl_skip_escape;
extern HWND	g_hWnd;

//----------------------------------------------------------------------------
// Operations
//----------------------------------------------------------------------------

void Timer_BlinkCursor()
{
	gbl_draw_cursor = !gbl_draw_cursor;
}

//-----------------------------------------------------------------------------
// CI::RunCursorBlinker
//
// constructor에 두면 Timer instance가 생성되어있지 않을 수도 있기 때문에 
// 외부에서 실행해줘야 한다.
//-----------------------------------------------------------------------------
void CI::RunCursorBlinker()
{
	g_tid_cursor_blink = gC_timer2.Add(700, Timer_BlinkCursor);
	gC_timer2.Continue(g_tid_cursor_blink);
}

//-----------------------------------------------------------------------------
// CI
//
//
//-----------------------------------------------------------------------------
CI::CI()
{
	Init();
}

//-----------------------------------------------------------------------------
// ~CI
//
// 
//-----------------------------------------------------------------------------
CI::~CI()
{

}

//-----------------------------------------------------------------------------
// IsEngInput
//
// 현재 입력상태가 영문이면 true를 반환한다.
//-----------------------------------------------------------------------------
bool CI::IsEngInput() const
{
	return !ImmGetOpenStatus(ImmGetContext(g_hWnd));
}

void CI::SetEngInput(bool bHangul)
{
	FinishImeRunning();
	ImmSetConversionStatus(ImmGetContext(g_hWnd), bHangul, NULL);
}

//-----------------------------------------------------------------------------
// FinishImeRunning
//
// 
//-----------------------------------------------------------------------------
void CI::FinishImeRunning()
{ 
	ClearCurrentIMEComposition();
	m_bl_ime_running = false;

	//SendMessage(g_hWnd, WM_IME_ENDCOMPOSITION, 0, 0);
	//SendMessage(CDirectDraw::GetHwnd(), WM_KEYDOWN, VK_LEFT, 0);
}

//-----------------------------------------------------------------------------
// ForceShowCursor
//
// 강제로 cursor를 보이게 한다.
//-----------------------------------------------------------------------------
void CI::ForceShowCursor() const
{
	gbl_draw_cursor = true;
	gC_timer2.Refresh(g_tid_cursor_blink);
}

//-----------------------------------------------------------------------------
// GetCursorBlink
//
// 
//-----------------------------------------------------------------------------
bool CI::GetCursorBlink() const
{
	return gbl_draw_cursor;
}

//-----------------------------------------------------------------------------
// GetEndOfIME
//
// 
//-----------------------------------------------------------------------------
bool CI::GetEndOfIME()
{
	bool state = m_bl_end_of_ime;
	m_bl_end_of_ime = false;

	return state;
}

//-----------------------------------------------------------------------------
// Init
//
// 외부에서 실행 가능하다.
//-----------------------------------------------------------------------------
void CI::Init()
{
	m_composing_char[0] = 0;
	m_composing_char[1] = 0;
	m_bl_insert_mode = true;
	m_bl_end_of_ime = false;
	m_bl_ime_running = false;

	ClearCurrentIMEComposition();
}

//-----------------------------------------------------------------------------
// CI::ClearCurrentIMEComposition
//
// 
//-----------------------------------------------------------------------------
void	CI::ClearCurrentIMEComposition()
{
	ImmNotifyIME(ImmGetContext(g_hWnd), NI_COMPOSITIONSTR, CPS_CANCEL, 0);
}

//-----------------------------------------------------------------------------
// IME_MessageProcessor
//
// 
//-----------------------------------------------------------------------------
void CI_KOREAN::IME_MessageProcessor(UINT message, WPARAM wParam, LPARAM lParam)
{
	//pass 는 윸이란뒤에 아스키가 오면 0xc05d 식으로 뒷부분에 날라오는게 아스키 범위내의 코드가 날라와서 '윸D'이런식으로 된다.
	//그걸 방지하기위해 endcomposition이후에 오는 wm_char두개를 무시하게 했는데
	//그러고 나니 e5코드가 들어오지 않는다.
	static int pass = 0;
	
	//이건 웈기 식으로 end안거칠때의 처리
	if(message == WM_CHAR && m_bl_ime_running)
		return;

	//아래에서 e5코드가 있는경우는 미리 pass--를 해주므로 0xe5가 혹시나? 들어올 경우는 처리하지 않는다.
	//lParam이 1일경우가 ime에서 나오는 WM_CHAR메세지다
	if(pass > 0 && message == WM_CHAR && wParam != 0xe5 && lParam == 1)
	{
		pass--;
		return;
	}


	switch (message)
	{
		case WM_IME_STARTCOMPOSITION:
			m_bl_end_of_ime = false;
			m_bl_ime_running = true;
			IME_StartComposition();
			break;

		case WM_IME_ENDCOMPOSITION:
			m_bl_end_of_ime = true;
			m_bl_ime_running = false;
			IME_EndComposition();

			//만들던 글자가 있으면
			if(m_composing_char[0])
			{
				pass = 2;
				
				//위에서 말한 e5코드는 wm_char로 들어오지 않으므로 여기서 pass를 줄여준다
				if((m_composing_char[0] & 0xff00) == 0xe500)
					pass--;
			}
			break;

		case WM_IME_COMPOSITION:
			//
			// wParam의 상하위 byte 위치를 바꿔야 정상적으로 출력된다.
			//
			m_composing_char[0] = (char_t)(wParam<<8);
			m_composing_char[0] |= (char_t)((wParam>>8)&0x00FF);
			
			if ((lParam&GCS_COMPSTR) == 0)
				IME_NextComposition();
			else
				IME_Composition();
			break;
			
		default:
			IME_Normal(message, wParam, lParam);
			break;
	}
}

//-----------------------------------------------------------------------------
// IME_MessageProcessor
//
// 
//-----------------------------------------------------------------------------
void CI_CHINESE::IME_MessageProcessor(UINT message, WPARAM wParam, LPARAM lParam)
{
	int len;
	if(wParam == 0xe5) return;
	
	//이건 웈기 식으로 end안거칠때의 처리
	if(message == WM_CHAR && ImeRunning())
		return;
	
	HIMC m_hIMC=NULL;
	HWND ImehWnd=ImmGetDefaultIMEWnd(g_hWnd);
				
	switch (message)
	{
	case WM_IME_STARTCOMPOSITION:
		// When focus change, enter garbage value in input buffer.
		gbl_skip_escape = true;
		g_pDXInput->SetAcquire(false);
		ShowWindow( ImehWnd , SW_HIDE );
		ShowWindow( ImehWnd , SW_SHOWNORMAL );
		SetFocus(g_hWnd);
		g_pDXInput->SetAcquire(true);
		
		//			SetStartComposition();
		//			IME_StartComposition();
		//			SetGapEscape();
		break;
	case WM_IME_ENDCOMPOSITION:
		//			if(ImeRunning() || !m_bl_end_of_ime)
		//				SetGapEscape();
		//			SetEndComposition();
		IME_EndComposition();
		break;
	case WM_IME_COMPOSITION:			
		//			SetGapEscape();
		gbl_skip_escape = true;
		m_hIMC = ImmGetContext(g_hWnd);
		if(lParam & GCS_RESULTSTR)
		{
			if ((len = ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, NULL, 0)) > 0)
			{
				//					SetStartComposition();
				ImmGetCompositionString(m_hIMC, GCS_RESULTSTR,m_composing_string, len);
				m_composing_string[len] = NULL;
				IME_NextComposition();
				memset(m_composing_string,0,128);
				gbl_skip_escape = false;
			}				
		} 
		ImmReleaseContext(g_hWnd,m_hIMC);
		break;			
	default:
		IME_Normal(message, wParam, lParam);
		break;
	}	
}