/*-----------------------------------------------------------------------------

	Ci_macOS.cpp

	macOS stub implementations for Korean/Chinese Input Method classes.

	This file provides basic stub implementations for CI, CI_KOREAN, and CI_CHINESE
	classes on macOS since the original Windows IME implementation cannot be used.

-----------------------------------------------------------------------------*/

#include "Client_PCH.h"
#include "CI.h"
#include "Timer2.h"
#include "Vs_ui.h"  // For C_VS_UI class

CI *gC_ci = NULL;

static timer_id_t	g_tid_cursor_blink = INVALID_TID;
static bool	gbl_draw_cursor = false;

//----------------------------------------------------------------------------
// Timer callback
//----------------------------------------------------------------------------
void Timer_BlinkCursor()
{
	gbl_draw_cursor = !gbl_draw_cursor;
}

//----------------------------------------------------------------------------
// CI base class implementations
//----------------------------------------------------------------------------
void CI::RunCursorBlinker()
{
	g_tid_cursor_blink = gC_timer2.Add(700, Timer_BlinkCursor);
	gC_timer2.Continue(g_tid_cursor_blink);
}

CI::CI()
{
	Init();
}

CI::~CI()
{
}

void CI::Init()
{
	m_bl_insert_mode = true;
	m_bl_end_of_ime = false;
	m_bl_ime_running = false;
	m_composing_char[0] = '\0';
	m_composing_char[1] = '\0';
	m_composing_string[0] = '\0';
}

void CI::ClearCurrentIMEComposition()
{
	m_composing_char[0] = '\0';
	m_composing_char[1] = '\0';
	m_composing_string[0] = '\0';
}

bool CI::GetEndOfIME()
{
	return m_bl_end_of_ime;
}

bool CI::GetCursorBlink() const
{
	return gbl_draw_cursor;
}

void CI::ForceShowCursor() const
{
	gbl_draw_cursor = true;
}

void CI::FinishImeRunning()
{
	m_bl_ime_running = false;
}

bool CI::IsEngInput() const
{
	/* Stub: Assume English input on macOS */
	return true;
}

void CI::SetEngInput(bool bHangul)
{
	/* Stub: No-op on macOS */
	(void)bHangul;
}

//----------------------------------------------------------------------------
// CI_KOREAN implementations
//----------------------------------------------------------------------------
void CI_KOREAN::IME_MessageProcessor(UINT message, WPARAM wParam, LPARAM lParam)
{
	// On macOS/SDL2, bypass IME and directly send keyboard events to UI
	// The C_VS_UI::KeyboardControl will handle routing to LineEditor
	if (message == WM_CHAR || message == WM_KEYDOWN)
	{
		gC_vs_ui.KeyboardControl(message, wParam, lParam);
	}
}

void CI_KOREAN::IME_NextComposition()
{
	/* Stub: Korean IME not implemented on macOS */
}

void CI_KOREAN::IME_Composition()
{
	/* Stub: Korean IME not implemented on macOS */
}

//----------------------------------------------------------------------------
// CI_CHINESE implementations
//----------------------------------------------------------------------------
void CI_CHINESE::IME_MessageProcessor(UINT message, WPARAM wParam, LPARAM lParam)
{
	// On macOS/SDL2, bypass IME and directly send keyboard events to UI
	// The C_VS_UI::KeyboardControl will handle routing to LineEditor
	if (message == WM_CHAR || message == WM_KEYDOWN)
	{
		gC_vs_ui.KeyboardControl(message, wParam, lParam);
	}
}

void CI_CHINESE::IME_NextComposition()
{
	/* Stub: Chinese IME not implemented on macOS */
}

void CI_CHINESE::IME_Composition()
{
	/* Stub: Chinese IME not implemented on macOS */
}
