/*-----------------------------------------------------------------------------

	u_edit.h

	Edit Widget.

	2000.6.7. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __U_EDIT_H__
#define __U_EDIT_H__

#include <string>
#include "FL2.h"
#include "SimpleDataList.h"

//-----------------------------------------------------------------------------
// LineEditor
//
// Single-line editor.
//
// * 문자를 입력할 수 있는 line은 직사각형 box 형태를 취한다. 이것을 edit box라 하자.
//
// < edit box >
//
// edit box는 입력하는 데 있어서 시각적으로 어떻게 표현되어야 할 것인지 기준을 
// 제공한다. 물론 이 class에서도 다른 Widget과 같이 출력에 대해서는 정의하지 않는다.
// 따라서 이것은 크기를 가늠할 수 있는 내부적인 기준이지만, 실제로 그렇게 표시되거나
// 되지않거나 하는 것은 이 class를 사용하는 App에 달려 있다. font 출력도 이에 
// 해당한다.
//
// < font size 변경 >
//
// LineEditor class는 '출력'에 대해 신경쓰지 않으므로 font size 변경에 관련된
// 내용도 정의되지 않는다. m_line_buf는 순수하게 'character code'만 저장하고 있다.
// 만약, m_line_buf의 일부의 문자열의 font size를 크게 바꾸고싶다면 그러한 정보를
// 저장하고 관리하는 code는 다른 곳에 두어야 한다.
//
// < cursor >
//
// cursor는 line에서의 '위치(position)'이다. cursor의 위치에서 삽입/삭제/copy/paste등이
// 이루어진다.
//
// < edit box의 절대적 크기와 입력제한 >
//
// edit box의 크기를 넘을 것이라고 해서 입력이 불가능한 것은 아니다. 이는 실제적인
// 입력제한은 edit box 내부에서의 논리적 크기에 의해서 제한됨을 의미한다.
// 그 논리적 크기가 edit box보다 클 경우 cursor의 위치에 의해 line은 스크롤된다.
// 입력할 수 있는 문자수가 많아도 절대로 논리적 크기는 넘지 못한다(제한된다).
// 이것은 font를 출력하는 곳에서 font의 width를 검사하여 결정할 문제다.
//-----------------------------------------------------------------------------
typedef std::basic_string<char_t> String;

class LineEditor
{
protected:
	String						m_string;
	int								m_gap;	// editor모드일때 출력할 y_gap
	int								m_editor_height;

	//
	// m_logical_size
	//
	// edit box의 논리적 크기다.
	//
	//int							m_logical_size;

	//
	// m_char_input_count
	//
	// 입력할 수 있는 문자의 개수를 지정한다.
	// 음수이면 입력제한 없다.
	//
	int							m_char_input_count;

	//
	// m_cursor
	//
	// cursor는 문자가 입력될 위치를 가리킨다. 곧, string의 index이다.
	// string에서의 cursor. cursor의 범위는 string의 맨 앞과 맨 끝 사이이다.
	//
	int							m_scroll;
	int							m_cursor;

	//
	// m_byte_limit
	//
	// byte 입력제한.
	// 이것은 DBCS이전의 산물로 영문 1byte 한글 2byte일 경우에 써먹는 것이다.
	// 음수이면 제한 없다.
	//
	int							m_byte_limit;
	int							m_reach_limit;

	bool							m_bl_digit_only;
	// m_bl_password_mode
	//
	// 하나의 문자를 '*'로 출력한다.
	//
	bool							m_bl_password_mode;

public:
	LineEditor();
	LineEditor(int logical_size);
	~LineEditor();

	void	Init();

	//----------------------
	// Acquire/Unacquire
	//----------------------
	void	Acquire();
	void	Unacquire();
	bool	IsAcquire() const;

	//----------------------
	// Input
	//----------------------
	void	KeyboardControl(UINT message, UINT key, long extra);
//	void	InsertToCursorPosition(char_t a_char);
//	void	InsertToCursorPositionForIME(char_t a_char);
	void	EditToCursorPosition(char_t a_char);
	void	EditToCursorPositionForIME(char_t a_char);
	void	InsertMark(char_t a_char);
	bool	InsertGap();

	virtual const int GetLineCount() const { return 0; }

	//----------------------
	// Check input limit
	//----------------------
public:
	//virtual bool EndOfLogicalSize() const { return false; } // 어떤 font인지 모름으로 LineEditor에서 결정할 수 없음.
	virtual bool	ReachEndOfBox(char_t will_input_char) const { return false; }
	bool	CheckInputLimit(char_t will_input_char);
	bool	CheckInputCharLimit() const;

	bool	IsPasswordMode() const { return m_bl_password_mode; }

public:

	//----------------------
	// Get
	//----------------------
	const char_t * GetString() const { return m_string.c_str(); }
	const char_t * GetStringScroll() const;
	int	Size() const { return m_string.size(); }
	int	GetCursor() const { return m_cursor; }

	//----------------------
	// Set
	//----------------------
	//void	SetLogicalSize(int logical_size);
	void	SetInputCharCount(int char_count);
	void	SetByteLimit(int byte);
	void	AddString(const char * sz_str);
	void	SetDigitOnlyMode(bool state) { m_bl_digit_only = state; }
	bool	GetDigitOnlyMode() const { return m_bl_digit_only; }

	//----------------------
	// Cursor movement
	//----------------------
	int		IncreaseCursor();
	int		DecreaseCursor();
	bool	HomeCursor();
	bool	EndCursor();

	//----------------------
	// Erase string
	//----------------------
//private:
	bool	EraseCharacterFrontCursor();

public:
	void	EraseAll();
	bool	EraseCharacterBegin();
};

//-----------------------------------------------------------------------------
// LineEditorVisual
//
// LineEditor를 시각화시킨다.
//-----------------------------------------------------------------------------
class LineEditorVisual : public LineEditor
{
private:
	Point							m_xy;
	int							m_abs_width; // edit box의 절대 넓이. 음수이면 무한대이다.
	COLORREF						m_cursor_color;

	//

	PrintInfo					m_print_info;
	
public:
	LineEditorVisual();
	~LineEditorVisual();

	void	SetAbsWidth(int width);
	void	SetCursorColor(COLORREF color);
	void	SetPosition(int x, int y);
	Point	GetPosition() { return m_xy; }
	void	Show() const;
	void	SetEditorMode(int gap, int height = 0) { m_gap = gap; m_editor_height = height; }

	bool		ReachEndOfBox(char_t will_input_char = 0) const;
	int		ReachSizeOfBox() const;

	//bool	EndOfLogicalSize() const;

	void	PasswordMode(bool enable);
	void	SetPrintInfo(PrintInfo &print_info);
	PrintInfo &GetPrintInfo()	{ return m_print_info; }	// by sigi
	void	SetInputStringColor(COLORREF rgb) { m_print_info.text_color = rgb; }

	const int GetLineCount() const;
	

};

//-----------------------------------------------------------------------------
// DocumentEditor
//
// Multi-line editor
//-----------------------------------------------------------------------------
class DocumentEditor
{
private:
	//LineEditor		m_line_editor[line];

public:
};

//-----------------------------------------------------------------------------
// EditManager
//
// DocumentEditor를 관리한다.
// Single line을 사용하는 edit line은 DocumentEditor로 구현된다. line 설정이 1로
// 되면 같아지기 때문이다. 그래서 EditManager는 DocumentEditor만을 관리할 필요가
// 있다. 특수한 목적으로 LineEditorVisual을 사용한다면 EditManager의 해택을
// 받을 수 없다. 그렇게 되면 문제가 생기겠지...
//
// Edit Widget은 한 번에 하나의 대상에 입력할 수 있으므로 그것의 처리를 해주며,
// 외부에서는 EditManager로 그들로 통하는 단일한 인터페이스를 구성한다.
//-----------------------------------------------------------------------------
/*
class EditManager : public SimpleDataList<DocumentEditor *>
{
private:

public:
	EditManager();
	~EditManager();

	//
	// Register/Unregister
	//
	void	Register(DocumentEditor * p_de);
	void	Unregister(DocumentEditor * p_de);

	void	Show(HDC hdc) const;
};*/

#endif