// u_edit.cpp

#include "client_PCH.h"

#include "debuginfo.h"
#include "u_edit.h"
#include <vector>


//
// gpC_focused_line_editor
//


//


//
static LineEditor *	gpC_focused_line_editor = NULL;
extern HWND g_hWnd;

void SetImePosition(int x,int y)
{	
	static int mx=0,my=0;
	
	if(mx==x&&my==y) return;

	mx=max(0,x);
	my=max(0,y);

	HIMC m_hIMC=NULL;
	m_hIMC = ImmGetContext(g_hWnd);

	POINT temps={mx,my};	
	const int ImeMaxY=800;
	while(temps.y>ImeMaxY)
		temps.y = ImeMaxY - (temps.y-ImeMaxY);	
	RECT temprect={mx,my-16,mx+60,my};	

	COMPOSITIONFORM comp;	
	comp.dwStyle=CFS_FORCE_POSITION;
	comp.ptCurrentPos=temps;
	comp.rcArea=temprect;

	ImmSetCompositionWindow(m_hIMC,&comp);
	ImmSetStatusWindowPos(m_hIMC,&temps);
	ImmReleaseContext(g_hWnd,m_hIMC);
}

//-----------------------------------------------------------------------------
// Operations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// LineEditor
//
//
//-----------------------------------------------------------------------------
LineEditor::LineEditor()
{
	Init();
}

LineEditor::LineEditor(int logical_size)
{
	Init();
//	SetLogicalSize(logical_size);
}

//-----------------------------------------------------------------------------
// ~LineEditor
//
// 
//-----------------------------------------------------------------------------
LineEditor::~LineEditor()
{
	if(IsAcquire())
		Unacquire();
}

//-----------------------------------------------------------------------------
// LineEditor::AddString
//

//-----------------------------------------------------------------------------
void	LineEditor::AddString(const char * sz_str)
{
	/*
	if (sz_str != NULL)
	{
		for (int i=0; i < strlen(sz_str); i++)
		{
			EditToCursorPosition((char_t)sz_str[i]);

			IncreaseCursor();
		}
	}*/

	if (sz_str != NULL)
	{
		char_t * p_new_buf = NULL;
		int dbcs_len = g_ConvertAscii2DBCS(sz_str, strlen(sz_str), p_new_buf);
		if (p_new_buf)
		{
			m_string.insert(Size(), p_new_buf);
			m_cursor += dbcs_len;
		}

		DeleteNewArray(p_new_buf);
	}
}

//-----------------------------------------------------------------------------
// CheckInputCharLimit
//

//-----------------------------------------------------------------------------
bool LineEditor::CheckInputCharLimit() const
{
	return (m_char_input_count < m_string.size()+1);
}

//-----------------------------------------------------------------------------
// SetByteLimit
//
// 
//-----------------------------------------------------------------------------
void LineEditor::SetByteLimit(int byte)
{
	m_byte_limit = byte;
}

//-----------------------------------------------------------------------------
// CheckInputLimit
//

//-----------------------------------------------------------------------------
bool LineEditor::CheckInputLimit(char_t will_input_char)
{
	//if (EndOfLogicalSize() == false) // logical size limit
//	if (ReachEndOfBox(will_input_char) == true)
	{
		// check byte limit
		if (m_byte_limit > -1)
		{
			char * str_buf = NULL;
			int len = g_Convert_DBCS_Ascii2SingleByte(m_string.c_str(), m_string.size(), str_buf);
			DeleteNewArray(str_buf);

			int add;
			if (will_input_char == 0) // DBCS
				add = 2;
			else
				add = 1;
			if (len+add > m_byte_limit)
				return true;
//			if (ReachEndOfBox(will_input_char) == true)
//				m_scroll++;

		}

		if (CheckInputCharLimit() == false)
		{
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// EraseCharacterFrontCursor
//

//-----------------------------------------------------------------------------
bool LineEditor::EraseCharacterFrontCursor()
{
	if (m_cursor > 0)
	{
		m_string.erase(m_cursor-1, 1);
		DecreaseCursor();
		if(m_scroll > 0 && !ReachEndOfBox(-1))m_scroll--;
		return true;
	}

	return false;
}

bool LineEditor::EraseCharacterBegin()
{
	if(m_string.size() > 0)
	{
		m_string.erase(0, 1);
		DecreaseCursor();
		if(m_scroll > 0 && !ReachEndOfBox(-1))m_scroll--;
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// EraseAll
//

//-----------------------------------------------------------------------------
void LineEditor::EraseAll()
{ 
	m_cursor = 0;
	m_scroll = 0;
	m_string.erase(); 
}

//-----------------------------------------------------------------------------
// InsertGap
//

//-----------------------------------------------------------------------------
bool LineEditor::InsertGap()
{
//	DEBUG_ADD("[LineEditor] InsertGap");
	char_t buf[2];
	buf[0] = 0x0020; // space
	buf[1] = 0;

	m_string.insert(m_cursor, buf);

	if(m_editor_height > 0 && GetLineCount() > m_editor_height)
	{
		m_string.erase(&m_string[m_cursor]);
//		DEBUG_ADD("[LineEditor] InsertGap FALSE");
		return false;
	}

//	DEBUG_ADD("[LineEditor] InsertGap OK");
	return true;
}

//-----------------------------------------------------------------------------
// IncreaseCursor
//
// 
//-----------------------------------------------------------------------------
int LineEditor::IncreaseCursor()
{ 
	if (m_cursor < m_string.size())
	{
		m_cursor++;
		if(m_gap == 0)
		{
			char *str_buf;
			if(ReachEndOfBox(0))
			{
				int len;
				do{
					len = g_Convert_DBCS_Ascii2SingleByte(m_string.c_str()+m_scroll, m_cursor-m_scroll, str_buf);
					DeleteNew(str_buf);
					if(len > m_reach_limit)m_scroll++;
					{
						len = g_Convert_DBCS_Ascii2SingleByte(m_string.c_str()+m_scroll, m_cursor-m_scroll, str_buf);
						DeleteNew(str_buf);
					}
				}while(len > m_reach_limit);
			}
		}
		if(m_string[m_cursor-1] & 0xff00)
			return 2;
		else
			return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// DecreaseCursor
//
// 
//-----------------------------------------------------------------------------
int LineEditor::DecreaseCursor()
{ 
	if (m_cursor > 0)
	{
		m_cursor--;
		if(m_cursor < m_scroll)m_scroll--;
		if(m_string[m_cursor] & 0xff00)
			return 2;
		else
			return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// HomeCursor
//
// 
//-----------------------------------------------------------------------------
bool LineEditor::HomeCursor()
{ 
	if (m_cursor != 0)
	{
		m_cursor = 0;
		m_scroll = 0;
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// EndCursor
//
// 
//-----------------------------------------------------------------------------
bool LineEditor::EndCursor()
{ 
//	if (m_cursor != m_string.size())
//	{
//		m_cursor = m_string.size(); 
//		return true;
//	}
	if(m_cursor != m_string.size())
	{
		while(IncreaseCursor());
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Init
//
// 
//-----------------------------------------------------------------------------
void LineEditor::Init()
{
	m_editor_height = 0;
	m_reach_limit = 100;
	m_byte_limit = -1;
	m_char_input_count = -1;
	m_scroll = 0;
	m_cursor = 0;
	//m_logical_size = 0;
	m_bl_digit_only = false;
	m_bl_password_mode = false;
	m_gap = 0;
}

//-----------------------------------------------------------------------------
// SetLogicalSize
//
// 
//-----------------------------------------------------------------------------
/*void LineEditor::SetLogicalSize(int logical_size)
{



	assert(logical_size > -1);

	if (logical_size < 0)
		_Error(FAILED_JOB);

	m_logical_size = logical_size;
}*/

//-----------------------------------------------------------------------------
// SetInputCharCount
//

//-----------------------------------------------------------------------------
void LineEditor::SetInputCharCount(int char_count)
{
	m_char_input_count = char_count;
}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void LineEditor::KeyboardControl(UINT message, UINT key, long extra)
{
	static int debug_count = 0;

	if (debug_count < 10) {
		printf("DEBUG LineEditor::KeyboardControl: message=%d, key=%d, extra=%ld\n",
			   message, key, extra);
		debug_count++;
	}

//	DEBUG_ADD("[LineEditor] KeyboardControl");
	switch (message)
	{
		case WM_KEYDOWN:
			switch (key)
			{
				case VK_LEFT:
					gC_ci->FinishImeRunning();
					if (DecreaseCursor())
						gC_ci->ForceShowCursor();
					break;

				case VK_RIGHT:
					gC_ci->FinishImeRunning();
					if (IncreaseCursor())
							gC_ci->ForceShowCursor();
					break;

				case VK_HOME:
					gC_ci->FinishImeRunning();
					if (HomeCursor() == true)
						gC_ci->ForceShowCursor();
					break;

				case VK_END:
					gC_ci->FinishImeRunning();
					if (EndCursor() == true)
						gC_ci->ForceShowCursor();
					break;

				case VK_INSERT:
					gC_ci->ToggleInsertMode();
					break;

				case VK_BACK:
					gC_ci->FinishImeRunning();
					if (EraseCharacterFrontCursor() == true)
						gC_ci->ForceShowCursor();
					break;

				case VK_RETURN:
					if(m_gap)
					{
						if(m_editor_height > 0 && GetLineCount()+1 > m_editor_height)
							return;

						if (gC_ci->GetInsertModeState() == true) // insert?
						{
							if (CheckInputLimit('\n') == true)
								return;
							
							//InsertToCursorPosition((char_t)key);
							if(InsertGap())
								EditToCursorPosition((char_t)'\n');							
						}
						else
						{
							if (m_cursor == m_string.size()) // at end?
								if (CheckInputLimit(key) == true)
									return;
								
								EditToCursorPosition((char_t)'\n');								
						}
						IncreaseCursor();
						gC_ci->ForceShowCursor();
					}
					break;

				case VK_UP:
					if(m_gap)
					{
						gC_ci->FinishImeRunning();
						int sum = 0;
						while(sum < m_reach_limit+1)
						{
							int num = DecreaseCursor();
							if(num == 0 || m_string[m_cursor] == '\n')break;
							sum += num;
						}
						if(sum > m_reach_limit+1)
							IncreaseCursor();

						gC_ci->ForceShowCursor();
					}
					break;

				case VK_DOWN:
					if(m_gap)
					{
						gC_ci->FinishImeRunning();
						int sum = 0;
						while(sum < m_reach_limit+1)
						{
							int num = IncreaseCursor();
							if(num == 0 || m_string[m_cursor-1] == '\n')break;
							sum += num;
						}
						if(sum > m_reach_limit+1)
							DecreaseCursor();

						gC_ci->ForceShowCursor();
					}
					break;

				case VK_DELETE:
					if (m_cursor < m_string.size())
					{
						IncreaseCursor();
						if (EraseCharacterFrontCursor() == true)
							gC_ci->ForceShowCursor();
					}
					break;
			}
			break;

		case WM_CHAR:
			//




			//

			//


			static int char_debug_count = 0;

			if (char_debug_count < 10) {
				printf("  DEBUG WM_CHAR: key=%d ('%c'), IsAcquire()=%d\n",
					   key, (char)key, IsAcquire());
				printf("    m_cursor=%zu, m_string.size()=%zu\n",
					   m_cursor, m_string.size());
				fflush(stdout);
			}

			if ((char)key >= 32 && (char)key <= 126)
			{
				if (m_bl_digit_only)
					if (!((char)key >= '0' && (char)key <= '9'))
						return;
											
				if (gC_ci->GetInsertModeState() == true) // insert?
				{
					if (CheckInputLimit(key) == true)
						return;

					//InsertToCursorPosition((char_t)key);
					if(InsertGap())
						EditToCursorPosition((char_t)key);
				}
				else
				{
					if (m_cursor == m_string.size()) // at end?
						if (CheckInputLimit(key) == true)
							return;

					EditToCursorPosition((char_t)key);
				}

				IncreaseCursor();
				gC_ci->ForceShowCursor();

				if (char_debug_count < 10) {
					printf("    After insert: m_cursor=%zu, m_string.size()=%zu\n",
						   m_cursor, m_string.size());
					printf("    m_string content: '%s'\n", m_string.c_str());
					fflush(stdout);
					char_debug_count++;
				}
			}
			break;
	}
//	DEBUG_ADD("[LineEditor] KeyboardControl OK");
}

//-----------------------------------------------------------------------------
// IsAcquire
//
//
//-----------------------------------------------------------------------------
bool LineEditor::IsAcquire() const
{

	return (gpC_focused_line_editor == (LineEditor *)this);
}

//-----------------------------------------------------------------------------
// Acquire
//

//-----------------------------------------------------------------------------
void LineEditor::Acquire()
{
	gpC_focused_line_editor = this;
	gC_ci->ForceShowCursor();
	gC_ci->ClearCurrentIMEComposition();
}

//-----------------------------------------------------------------------------
// Unacquire
//

//-----------------------------------------------------------------------------
void LineEditor::Unacquire()
{
	gpC_focused_line_editor = NULL;
}

//-----------------------------------------------------------------------------
// InsertToCursorPosition
//

//-----------------------------------------------------------------------------
//void LineEditor::InsertToCursorPosition(char_t a_char)
//{
//	if (m_cursor > m_string.size())
//		_Error(FAILED_JOB);
//
//	char_t buf[2];
//	buf[0] = a_char;
//	buf[1] = 0;
//
//	m_string.insert(m_cursor, buf);
//}

//-----------------------------------------------------------------------------
// InsertToCursorPositionForIME
//
// 
//-----------------------------------------------------------------------------
//void LineEditor::InsertToCursorPositionForIME(char_t a_char)
//{

//}

//-----------------------------------------------------------------------------
// EditToCursorPosition
//

//-----------------------------------------------------------------------------
void LineEditor::EditToCursorPosition(char_t a_char)
{
	if (m_cursor > m_string.size())
		_Error(FAILED_JOB);

	if (m_cursor == m_string.size()) // end of string?
	{
		//InsertToCursorPosition(a_char);
		if(InsertGap())
			m_string[m_cursor] = a_char;
		return;
	}
	//else
	m_string[m_cursor] = a_char;
}

//-----------------------------------------------------------------------------
// EditToCursorPositionForIME
//

//-----------------------------------------------------------------------------
void LineEditor::EditToCursorPositionForIME(char_t a_char)
{
	if (m_cursor == 0  || m_cursor > m_string.size())
		_Error(FAILED_JOB);

	m_string[m_cursor-1] = a_char;


	if (a_char == 0)
		EraseCharacterFrontCursor();
}

void LineEditor::InsertMark(char_t a_char)
{
	if (gC_ci->GetInsertModeState() == true) // insert?
	{
		if (CheckInputLimit(a_char) == true)
			return;
		
		//InsertToCursorPosition((char_t)key);
		if(InsertGap())
			EditToCursorPosition(a_char);
	}
	else
	{
		if (m_cursor == m_string.size()) // at end?
			if (CheckInputLimit(a_char) == true)
				return;
	
		EditToCursorPosition(a_char);
	}
				
	IncreaseCursor();
	gC_ci->ForceShowCursor();
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// IME_EndComposition - auto call
//

//-----------------------------------------------------------------------------
void IME_EndComposition()
{
}

//-----------------------------------------------------------------------------
// IME_Composition - auto call
//
// 
//-----------------------------------------------------------------------------
void CI_KOREAN::IME_Composition()
{
	if (gpC_focused_line_editor != NULL && gpC_focused_line_editor->GetDigitOnlyMode() == false)
	{
		if (gC_ci->ImeRunning() == true)
		{
			gpC_focused_line_editor->EditToCursorPositionForIME(gC_ci->GetComposingChar());
			gC_ci->ForceShowCursor();
		}
	}
}

//-----------------------------------------------------------------------------
// IME_StartComposition - auto call
//

//-----------------------------------------------------------------------------
void IME_StartComposition()
{
	if (gpC_focused_line_editor != NULL && gpC_focused_line_editor->GetDigitOnlyMode() == false)
	{
		if (gC_ci->GetInsertModeState() == true)
		{
			if (gpC_focused_line_editor->CheckInputLimit(0) == false)
			{
				if(gpC_focused_line_editor->InsertGap() == false)
				{
					gC_ci->FinishImeRunning();
					return;
				}
			}
			else
			{
				gC_ci->FinishImeRunning();
				return;
			}
		}
		else
		{
			if (gpC_focused_line_editor->GetCursor() == gpC_focused_line_editor->Size())
			{
				if (gpC_focused_line_editor->CheckInputLimit(0) == false)
				{
					if(gpC_focused_line_editor->InsertGap() == false)
					{
						gC_ci->FinishImeRunning();
						return;
					}
				}
				else
				{
					gC_ci->FinishImeRunning();
					return;
				}
			}
		}

		gpC_focused_line_editor->IncreaseCursor();
	}
}

//-----------------------------------------------------------------------------
// IME_NextComposition - auto call
//

//

//-----------------------------------------------------------------------------
void CI_KOREAN::IME_NextComposition()
{
	if (gpC_focused_line_editor != NULL && gpC_focused_line_editor->GetDigitOnlyMode() == false)
	{
		if (gC_ci->GetEndOfIME() == false)
		{
			//

			//


			//
			if (gC_ci->ImeRunning() == true)
				gpC_focused_line_editor->EditToCursorPositionForIME(gC_ci->GetComposingChar());

			if (gC_ci->GetInsertModeState() == true)
			{
				if (gpC_focused_line_editor->CheckInputLimit(0/*gC_ci->GetComposingChar()*/) == false)
				{
					if(gpC_focused_line_editor->InsertGap() == false)
					{
						gC_ci->FinishImeRunning();
						return;
					}
				}
				else
				{
					gC_ci->FinishImeRunning();
					return;
				}
			}
			else
			{
				if (gpC_focused_line_editor->GetCursor() == gpC_focused_line_editor->Size())
				{
					if (gpC_focused_line_editor->CheckInputLimit(0/*gC_ci->GetComposingChar()*/) == false)
					{
						if(gpC_focused_line_editor->InsertGap() == false)
						{
							gC_ci->FinishImeRunning();
							return;
						}
					}
					else
					{
						{
							gC_ci->FinishImeRunning();
							return;
						}
					}
				}
			}

			gpC_focused_line_editor->IncreaseCursor();
			gC_ci->ForceShowCursor();
		}
	}
}

//-----------------------------------------------------------------------------
// IME_Normal - auto call
//

//-----------------------------------------------------------------------------
void IME_Normal(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (gpC_focused_line_editor != NULL)
	{
		gpC_focused_line_editor->KeyboardControl(message, wParam, lParam);
	}
}

//-----------------------------------------------------------------------------
// LineEditorVisual
//
// 
//-----------------------------------------------------------------------------
LineEditorVisual::LineEditorVisual()
{
	m_print_info.bk_mode = TRANSPARENT;
	m_print_info.back_color = RGB(0, 0, 0);
	m_print_info.text_color = RGB(255, 255, 255);
	m_print_info.hfont = NULL;
	m_print_info.text_align = TA_LEFT;

//	m_reach_limit = 0;
	m_abs_width = -1;
	m_cursor_color = RGB(255, 255, 255);
}

//-----------------------------------------------------------------------------
// ~LineEditorVisual
//
// 
//-----------------------------------------------------------------------------
LineEditorVisual::~LineEditorVisual()
{
	
}

//-----------------------------------------------------------------------------
// SetPrintInfo
//

//-----------------------------------------------------------------------------
void LineEditorVisual::SetPrintInfo(PrintInfo &print_info)
{
	m_print_info = print_info;
}

//-----------------------------------------------------------------------------
// ReachEndOfBox
//

//-----------------------------------------------------------------------------
bool LineEditorVisual::ReachEndOfBox(char_t will_input_char) const
{
	char * str_buf = NULL;

	String temp_string;
	temp_string = m_string;
	if(will_input_char != 0)
	{
		if (will_input_char > 0) // ASCII?
			temp_string += (char_t)'a';
		else
			temp_string += *((char_t *)"김");
	}

	int len = g_Convert_DBCS_Ascii2SingleByte(temp_string.c_str()+m_scroll, temp_string.size(), str_buf);

	int str_width = g_GetStringWidth(str_buf, m_print_info.hfont);

	DeleteNewArray(str_buf);

	if (m_abs_width > -1 && m_abs_width < str_width)
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// ReachEndOfBox
//

//-----------------------------------------------------------------------------
int LineEditorVisual::ReachSizeOfBox() const
{
	int str_width = g_GetStringWidth("a", m_print_info.hfont);

	return m_abs_width/str_width;
}

//-----------------------------------------------------------------------------
// SetAbsWidth
//
// 
//-----------------------------------------------------------------------------
void LineEditorVisual::SetAbsWidth(int width)
{
	m_abs_width = width;
	m_reach_limit = ReachSizeOfBox();
}

//-----------------------------------------------------------------------------
// PasswordMode
//
// 
//-----------------------------------------------------------------------------
void LineEditorVisual::PasswordMode(bool enable)
{
	m_bl_password_mode = enable;
}

//-----------------------------------------------------------------------------
// EndOfLogicalSize
//

//-----------------------------------------------------------------------------
//bool LineEditorVisual::EndOfLogicalSize() const
//{
	/*
	int len = g_Convert_DBCS_Ascii2SingleByte(m_string.c_str(), m_string.size(), str_buf);

	SIZE size;
	GetTextExtentPoint32(hdc, str_buf, len, &size);*/

//	return true;
//}

//-----------------------------------------------------------------------------
// SetCursorColor
//
// 
//-----------------------------------------------------------------------------
void LineEditorVisual::SetCursorColor(COLORREF color)
{
	m_cursor_color = color;
}

//-----------------------------------------------------------------------------
// SetPosition
//

//-----------------------------------------------------------------------------
void LineEditorVisual::SetPosition(int x, int y)
{
	m_xy.Set(x, y);
}

//-----------------------------------------------------------------------------
// Show
//

//-----------------------------------------------------------------------------
void LineEditorVisual::Show() const
{
//	DEBUG_ADD("[LineEditorVisual] Show");

	if (gpC_fl2_surface == NULL)
	{
//		DEBUG_ADD("[LineEditorVisual] Show FALSE");
		return;
	}

	g_FL2_ReleaseDC();

	int text_width = g_GetStringWidth("a", m_print_info.hfont);

	HDC hdc;
	gpC_fl2_surface->GetDC(&hdc);
	if( m_print_info.hfont != NULL )
		SelectObject(hdc, m_print_info.hfont);

	//
	// set format
	//
	SetBkMode(hdc, m_print_info.bk_mode);
	SetTextColor(hdc, m_print_info.text_color);
	SetBkColor(hdc, m_print_info.back_color);
	
//	DEBUG_ADD("[LineEditorVisual] Show 1");

	String password_string;
	int len = 0;
	char * str_buf = NULL;

	int print_y = m_xy.y, next = 0;
	
	if (m_bl_password_mode == true)
	{
		for (int i=0; i < m_string.size(); i++)
			password_string += (char_t)'*';
		if(m_gap == 0)
			len = g_Convert_DBCS_Ascii2SingleByte(password_string.c_str()+m_scroll, password_string.size(), str_buf);
		else
			len = g_Convert_DBCS_Ascii2SingleByte(password_string.c_str(), password_string.size(), str_buf);
	}
	else
	{
		if(m_gap == 0)
			len = g_Convert_DBCS_Ascii2SingleByte(m_string.c_str()+m_scroll, m_string.size(), str_buf);
		else
			len = g_Convert_DBCS_Ascii2SingleByte(m_string.c_str(), m_string.size(), str_buf);
	}
	
//	DEBUG_ADD("[LineEditorVisual] Show 2");

	std::vector<int> v_cut;
	

	if(str_buf)
	{
		if(m_gap == 0)
		{
			TextOut(hdc, m_xy.x, print_y, str_buf, min(len, m_reach_limit+1));
		}
		else
		{
			do
			{
				if(str_buf+next == NULL)
					break;
				
				int cut = false;
				char *enter_pos = strchr(str_buf+next, '\n');
				if(enter_pos != NULL && enter_pos - (str_buf+next) < min(len-next, m_reach_limit+1))
				{
					TextOut(hdc, m_xy.x, print_y, str_buf+next, enter_pos - (str_buf+next));
					v_cut.push_back(m_reach_limit - (enter_pos - (str_buf+next)));
					next += enter_pos - (str_buf+next)+1;
				}
				else
				{
					TextOut(hdc, m_xy.x, print_y, str_buf+next, min(len-next, m_reach_limit+1));
					if(min(len-next, m_reach_limit+1) < strlen(str_buf))
						if(!g_PossibleStringCut(str_buf+next, min(len-next, m_reach_limit+1)))
						{
							next--;
							cut = 1;
						}
					v_cut.push_back(cut);
					next += min(len, m_reach_limit+1);
				}
				
				print_y += m_gap;

				if(m_editor_height != 0 && v_cut.size() >= m_editor_height)
					break;
			}while(next < strlen(str_buf));
//			if(m_editor_height != 0 && v_cut.size() == m_editor_height && next > 0 && str_buf[next-1] == '\n')
//				v_cut.push_back(0);
		}
	}
	//	char str_color[80];
	//	wsprintf(str_color, "%x", m_print_info.text_color);
	//	TextOut(hdc, m_xy.x, m_xy.y, str_color, strlen(str_color));
	
//	DEBUG_ADD("[LineEditorVisual] Show 3");


	//
	// cursor
	//
//	if(!(m_editor_height != 0 && v_cut.size() > m_editor_height))// && (strlen(str_buf+next) != 0 || next > 0 && str_buf[next-1] == '\n')))
	{
		if(str_buf != NULL)
			DeleteNewArray(str_buf);

//		DEBUG_ADD("[LineEditorVisual] Show 4");

		if (gC_ci->GetCursorBlink() == true && IsAcquire() == true)
		{
			if (m_bl_password_mode == true)
				len = g_Convert_DBCS_Ascii2SingleByte(password_string.c_str(), m_cursor, str_buf);
			else
			{
				if(m_gap == 0)
					len = g_Convert_DBCS_Ascii2SingleByte(m_string.c_str()+m_scroll, m_cursor-m_scroll, str_buf);// - g_Convert_DBCS_Ascii2SingleByte(m_string.c_str(), m_scroll, str_buf);
				else
					len = g_Convert_DBCS_Ascii2SingleByte(m_string.c_str(), m_cursor, str_buf);// - g_Convert_DBCS_Ascii2SingleByte(m_string.c_str(), m_scroll, str_buf);
			}
			
//			DEBUG_ADD("[LineEditorVisual] Show 4-1");

			SIZE size;
			GetTextExtentPoint32(hdc, str_buf, len, &size);
			
			int px, py;
			TEXTMETRIC tm;
			GetTextMetrics(hdc, &tm);
			
			//		SetROP2(hdc, R2_XORPEN);
			
//			DEBUG_ADD("[LineEditorVisual] Show 4-2");
			
			if (gC_ci->ImeRunning() == true && m_bl_password_mode == false)
			{
//				DEBUG_ADD("[LineEditorVisual] Show 4-3");

				HBRUSH hbrush = CreateSolidBrush(m_cursor_color);
				HBRUSH holdbrush = (HBRUSH)SelectObject(hdc, hbrush);
				
				// DBCS width

				SIZE dbcs_size;
				char str[] = "김";
				GetTextExtentPoint32(hdc, str, 2, &dbcs_size);
				
				px = m_xy.x+size.cx-dbcs_size.cx-1;
				py = m_xy.y-1;
				
//				DEBUG_ADD("[LineEditorVisual] Show 4-4");

				if(m_gap != 0)
				{
					std::vector<int>::iterator itr = v_cut.begin();
					
					while(px > m_xy.x + (m_reach_limit-1)*text_width || itr != NULL && *itr > 1 && px - (m_reach_limit-1 - *itr)*text_width > m_xy.x)
					{
						px -= (m_reach_limit+1)*text_width;
						py += m_gap;
						px += (*itr)*text_width;
						itr++;
					}
				}

//				DEBUG_ADD("[LineEditorVisual] Show 4-5");
				
				if(!(m_editor_height != 0 && (py - (m_xy.y-1))/m_gap >= m_editor_height))
				{
					Rectangle(hdc, px, py, px+dbcs_size.cx+2, py+tm.tmHeight+2);
					
					if(str_buf)
					{
						SetTextColor(hdc, m_cursor_color^0xffffff);
						TextOut(hdc, px+1, py+1, str_buf+strlen(str_buf)-2, 2);
					}
				}
				
//				DEBUG_ADD("[LineEditorVisual] Show 4-6");

				DeleteObject(hbrush);
				
				SelectObject(hdc, holdbrush);

//				DEBUG_ADD("[LineEditorVisual] Show 4-7");
			}
			else
			{
//				DEBUG_ADD("[LineEditorVisual] Show 4-8");

				HPEN hpen = CreatePen(PS_SOLID, 2, m_cursor_color);
				HPEN holdpen = (HPEN)SelectObject(hdc, hpen);
				
				px = m_xy.x+size.cx+1;
				py = m_xy.y;
				

				if(m_gap != 0)
				{
					std::vector<int>::iterator itr = v_cut.begin();
					while(itr != v_cut.end() && (px > m_xy.x + (m_reach_limit+1)*text_width || itr != NULL && *itr > 1 && px - (m_reach_limit+1 - *itr)*text_width > m_xy.x))
					{
						px -= (m_reach_limit+1)*text_width;
						py += m_gap;
						px += (*itr)*text_width;
						itr++;
					}
				}
				
//				DEBUG_ADD("[LineEditorVisual] Show 4-9");

				if(!(m_editor_height != 0 && (py - (m_xy.y-1))/m_gap >= m_editor_height))
				{
				MoveToEx(hdc, px, py, NULL);
				LineTo(hdc, px, py+tm.tmHeight-1);
				}
				
				SelectObject(hdc, holdpen);
				DeleteObject(hpen);			

//				DEBUG_ADD("[LineEditorVisual] Show 4-10");
			}
			if(gC_ci->IsChinese())
				SetImePosition(px,py);
		}
		
	}

//	DEBUG_ADD("[LineEditorVisual] Show 5");

	if(str_buf != NULL)
		DeleteNewArray(str_buf);
	
	gpC_fl2_surface->ReleaseDC(hdc);

//	DEBUG_ADD("[LineEditorVisual] Show OK");

}
/*
//-----------------------------------------------------------------------------
// EditManager
//
// 
//-----------------------------------------------------------------------------
EditManager::EditManager()
{

}

//-----------------------------------------------------------------------------
// ~EditManager
//
// 
//-----------------------------------------------------------------------------
EditManager::~EditManager()
{

}

//-----------------------------------------------------------------------------
// Register
//
// 
//-----------------------------------------------------------------------------
void EditManager::Register(DocumentEditor * p_de)
{
	if (p_de == NULL)
		_Error(NULL_REF);

	Add(p_de);
}

//-----------------------------------------------------------------------------
// Unregister
//
// 
//-----------------------------------------------------------------------------
void EditManager::Unregister(DocumentEditor * p_de)
{
	if (p_de == NULL)
		_Error(NULL_REF);

	Delete(p_de);
}

//-----------------------------------------------------------------------------
// EditManager::Show
//
// 
//-----------------------------------------------------------------------------
void EditManager::Show(HDC hdc) const
{

}*/

const int LineEditorVisual::GetLineCount() const
{
	char * str_buf = NULL;

	int count = 0, next = 0, len = 0;
	
	if(m_gap == 0)
		len = g_Convert_DBCS_Ascii2SingleByte(m_string.c_str()+m_scroll, m_string.size(), str_buf);
	else
		len = g_Convert_DBCS_Ascii2SingleByte(m_string.c_str(), m_string.size(), str_buf);

	if(str_buf == NULL)
		return 0;

	do
	{
		int cut = false;

		char *enter_pos = strchr(str_buf+next, '\n');
		if(enter_pos != NULL && enter_pos - (str_buf+next) < min(len-next, m_reach_limit+1))
		{
			count++;
			next += enter_pos - (str_buf+next)+1;
		}
		else
		{
			if(min(len-next, m_reach_limit+1) < strlen(str_buf))
			{
				if(!g_PossibleStringCut(str_buf+next, min(len-next, m_reach_limit+1)))
				{
					next--;
					cut = 1;
				}
			}
			count++;
			next += min(len, m_reach_limit+1);
		}
		
		if(m_editor_height != 0 && count > m_editor_height)
		{
// 			count++;
			break;
		}
	}while(next < strlen(str_buf));

	DeleteNewArray(str_buf);
	
	return count;
}


void CI_CHINESE::IME_Composition()
{
//	if (gpC_focused_line_editor != NULL && gpC_focused_line_editor->GetDigitOnlyMode() == false)
//	{
//		if (gC_ci->ImeRunning() == true)
//		{
//			gpC_focused_line_editor->EditToCursorPositionForIME(gC_ci->GetComposingChar());
//			gC_ci->ForceShowCursor();
//		}
//	}
}

//-----------------------------------------------------------------------------
// IME_NextComposition - auto call
//

//

//-----------------------------------------------------------------------------
void CI_CHINESE::IME_NextComposition()
{

//by viva
	bool LastErase=false;
	const char *temp_str=gC_ci->GetComposingStringPtr();
	char_t *temps;
	
	int len=strlen(temp_str);	

	if(len <= 0)
		return;

	IME_StartComposition();
	for(int j=0;j<min(98,len);)
	{
		bool bAscii=false;

		if(temp_str[j] >= 32 && temp_str[j] <= 126) bAscii=true;

		temps=(char_t*)&temp_str[j];
		char_t ascii_temp=(char_t)temp_str[j]&0xff;

		if(bAscii)
		{
			// Skip Ascii.
			gpC_focused_line_editor->EditToCursorPosition( ascii_temp );
		//	gpC_focused_line_editor->EditToCursorPositionForIME( ascii_temp );
			j++;
		//	gpC_focused_line_editor->IncreaseCursor();
		//	gC_ci->ForceShowCursor();
			continue;
		}

		if (gpC_focused_line_editor != NULL && gpC_focused_line_editor->GetDigitOnlyMode() == false)
		{
			if (gC_ci->GetEndOfIME() == false)
			{
				gpC_focused_line_editor->EditToCursorPositionForIME(*temps);					

				if (gC_ci->GetInsertModeState() == true)
				{
					if (gpC_focused_line_editor->CheckInputLimit(*temps) == false)
					{
						gpC_focused_line_editor->InsertGap();
					}
				}
				else
				{
					if (gpC_focused_line_editor->GetCursor() == gpC_focused_line_editor->Size())
					{
						if (gpC_focused_line_editor->CheckInputLimit(*temps) == false)
						{
							gpC_focused_line_editor->InsertGap();
							LastErase = true;
						}
					}
				}
				gpC_focused_line_editor->IncreaseCursor();
				gC_ci->ForceShowCursor();
			}
		}
		j+=2;
	}

	gC_ci->FinishImeRunning();

	if(gC_ci->GetInsertModeState())
	{
		gpC_focused_line_editor->EraseCharacterFrontCursor();
	}
	else
	{
		gpC_focused_line_editor->DecreaseCursor();			// decrease cursor			
	}
	gC_ci->FinishImeRunning();	
}