/*-----------------------------------------------------------------------------

	u_window.h

	Window property base class.

	2000.8.14. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __U_WINDOW_H__
#define __U_WINDOW_H__

#pragma warning(disable:4786)

#include "Basics.h"
#include "PI.h"
#include <list>

#include "u_edit.h"

#define SCR2WIN_X(screen_x)			((screen_x)-x)
#define SCR2WIN_Y(screen_y)			((screen_y)-y)
#define WIN2SCR_X(win_x)				((win_x)+x)
#define WIN2SCR_Y(win_y)				((win_y)+y)

enum MOUSE_STATE
{
	MOUSE_NOSTATE,
	MOUSE_MOVE,
	MOUSE_LEFTDOWN,
	MOUSE_LEFTUP,
	MOUSE_RIGHTDOWN,
	MOUSE_RIGHTUP,
};

//-----------------------------------------------------------------------------
// Window
//
// 정의:
//
//		Window는 독립된 하나의 작업공간이다. 하나의 Multitasking을 구현할 때 물론
//		background에서 작업하는 process도 있지만 사용자에게 자신을 보이며 작업하는
//		Process도 있다. 이 때 그것은 다른 Process와 구별될 수 있어야 하는데 Window는
//		이것을 하기에 적절하다. Window는 객체로써 뚜렷하게 보이기 때문이다.
//		Window라고 꼭 사각일 필요는 없다. 하나의 작업공간으로써 여겨지면 그만이다.
//		그리고 또한 이것은 사용자에게 하나의 완벽한 작업공간으로써 기능을 다해야
//		한다.
//
//
// 기능:
//
//		Window는 자기 자신을 위한 Process를 갖는다. 그리고 자신을 보여야하며(Show)
//		사용자로부터 제어를 받아야한다(Control). 이 세가지가 핵심기능이다.
//
//		Process
//
//		자신의 작업을 실질적으로 행한다. 이것은 보이는 것과 입력받아야 하는 것에
//		완전히 분리되어 있다. 오직 순수하게 작업을 위해서 실행된다.
//
//		Show
//
//		Process 결과를 보이는 것 또는 현재상태를 보이는 것 그리고 Window 자체를 보이는
//		것을 수행한다. '보이는 것'이 임무이다.
//
//		Control
//
//		사용자는 컴퓨터 자체에 입력장치로 입력을 하듯, Window에 입력을 가한다. 
//		물론 이것은 자신의 Window에게만(현재 작업중인) 전달된다.
//
//-----------------------------------------------------------------------------
//
// < Rect를 상속받은 것에 대해 >
//
// Rect를 base class로 한다. Window라는 속성은 그 형태가 원이는 삼각형이든 사각형에
// 넣을 수 있다. 사각형태가 아니면 사실 Rect.w과 Rect.h는 별 의미가 없을수도 있지만,
// 범위를 확실히 정해주는 의미에서는 필요하다.
//
// < Window 속성 >
//
// Window에 속성을 설정할 수 있다. 아무것도 설정하지 않으면 기본값이 사용된다. 
// Window 이동하기도 하나의 속성이다. Window 속성은 각각의 Window가 지닐 수 있는 
// 것이다. "이 Window는 언제나 최우선순위를 갖는다"는 속성은 전체 Window 중 하나에
// 대한 속성이므로 이것은 WindowManager에서 다룬다.
// Window 각각에 설정하는 속성을 Local attributes라고 하고 WindowManager가 해주는
// 속성을 Global attributes라 하자.
// 속성의 지정은 아무때나 해도되며, 설정한 이후부터 효력이 나타난다.
//
// < Window 이동하기 >
//
// 형태가 자유로우므로 Window를 이동하기 위해서 선택하는 point는 외부에서 결정해야
// 한다. 그것은 IsPixel()를 test해 true를 받을경우여야 한다. 그리고 MoveReady()을 
// 수행한 후 SetOrigin()으로 point를 지정한다. 그 후의 처리는 Window::MouseControl이 
// 처리한다.
//
// < Event 통지 >
//
// Window가 이동하거나 resize되거나 할 경우는 그 event가 발생된 다음 바로 통지를
// 한다. 통지가 갈 필요가 있는 두 부분이 있다. 하나는 상속받은 객체에게로 그리고 
// 다른 하나는 App에게. App가 그 사실을 알 필요가 있을까? App가 Window를 다루는
// 하나의 WindowManager System이라면 그럴수도 있겠다. 상속받은 객체에게는 그 사실을
// 알 필요가 당연히 있다. 그래서 Event 발생후 곧바로 통지한다. 이건 간단히 자기
// 객체의 method를 실행함으로써 가능하다. App에게 알릴 경우는 WindowManager를
// 통하도록 한다.
//
// WindowManager에게 event를 알리는 방법은 어떻게 실현할까? WindowManager와 App는
// event receiver로 연결을 할 수 있다. Window는 독립적 객체이고 자신밖에 모르므로
// WindowManager가 적극적으로 그 사실을 알려고 해야한다. Window의 event를 WindowManager가
// 검색해야 한다는 것이다. 검색에 성공하면 자신도 그 event를 알고 App에게 전달 할
// 수 있다.
//
// < 전체적인 사용 방법 >
//
// Window를 상속받은 class는 pure virtual에 대한 구현을 해야한다. 그리고 MouseControl에서
// Window::MouseControl을 실행해야 한다.
// Process, Show, MouseControl, KeyboardControl은 전부 WindowManager에 의해 실행된다.
// WindowManager는 App와 Window 간의 중계역활을 수행한다. IsPixel은 현 Window 위치와
// 그리고 Window 형태를 참조하여 인자의 (x, y)가 그 Window의 pixel이 존재하는지 test한다.
// 이것이 pure virtual로써 개성적인 형태에 대한 Window focus를 만족시킨다.
//
// Window는 독립적인 객체로써 존재하며 오직 자신만 알뿐이고, 단지 외부의 접촉을 
// 공개된 방법에 의해 허용할 뿐이다. 이것이 바로 Window이다.
//
//
// * 주의사항
//
//		Rect를 base class로 하므로 인자중 (x, y, w, h)는 Rect의 이름과	같지 않아야 
//		한다. 이 Window class를 base로 하는 경우도 또한 마찬가지이다.
//
//
// [2000.8.21]
//
//	Size 조절에 대한 것은 구현되지 않음. 기초코드만 있을 뿐임.
//
//
// [2000.10.13]
//
// < Window 위에 다른 Widget 출력하기 >
//
// Window외의 Widget은 Window 위에서 출력된다. 그래서 그들은 생성되서 어느 Window에
// 놓일 것인가를 결정해야만 한다. 그것은 Window의 << 연산자에 의해서 구현된다.
//
//-----------------------------------------------------------------------------
class Window : public Rect, public PI_Visual
{
private:
	//
	// m_bl_window_move_ready
	//
	// 일반적으로 외부에서 Window의 형태를 파악하고 Window를 이동하기 전에
	// 설정하는 값이다. focus ok, push ok로 구분할 수 있지만, 이 모든 것에 대한
	// 것을 이 하나로 표현한다. 이것이 true로 될 때 Move를 실행할 수 있다.
	//
	bool							m_bl_window_move_ready;

	Point							m_origin;

	//
	// m_u_mouse_state
	//
	// Window에 Mouse event가 발생하면 그 상태 flag가 설정되어 WindowManager가 이 값을 
	// 참조할 수 있도록 한다. WindowManager가 이 값을 참조하면, MOUSE_NOSTATE로 된다.
	//
	// WindowManager Process는 UpdateInput 다음이나 이전에 있을 것이기 때문에 
	// 겹치는 경우(동시 누르기 등)는 생각할 필요가 없다.
	//
	MOUSE_STATE					m_u_mouse_state;

public:
	//
	// Local attributes flags
	//
	enum ATTRIBUTES_HIDE
	{
		ATTRIBUTES_HIDE_NOT,
		ATTRIBUTES_HIDE_WIDTH,
		ATTRIBUTES_HIDE_HEIGHT,
	};

	struct ATTRIBUTES
	{
		bool				window_move;		// Window의 movement enable/disable.
		bool				pin;					// pin이 꽂히면 안꽂힌 Window보다 우선순위가 빠르다.
		bool				topmost;				// topmost Window이다. pin 속성은 disable된다.
		bool				keyboard_control;	// Window가 activation되면 keyboard control을 받는다.
		bool				alpha;
		bool				statch;
		ATTRIBUTES_HIDE		autohide;
	};

private:
	ATTRIBUTES					m_attributes;

	virtual void	Init();

public:
	//
	// Event 통지 
	//
	virtual void WindowEventReceiver(id_t event) {}

	enum // event list
	{
		//EVENT_WINDOW_CREATE,
		//EVENT_WINDOW_DESTROY,
		EVENT_WINDOW_MOVE,
	};

public:
	Window(int _x, int _y, int _w, int _h);
	Window();
	virtual ~Window();

	virtual bool MouseControl(UINT message, int _x, int _y);
	virtual void KeyboardControl(UINT message, UINT key, long extra);
	virtual bool IsPixel(int _x, int _y) = 0;

	virtual void AcquireMouseFocus() {}		// mouse focus가 설정될 때.
	virtual void UnacquireMouseFocus() {}	// mouse focus가 해제될 때.

	// 최우선 Window로 되었을 때. topmost Window가 있고 Window가 topmost Window가 아니면 
	// 실행되지 않는다. (topmost Window가 최우선 Window이므로)
	virtual void AcquireFirstSequence() {} 

	// mouse로 click할 때.
	// AcquireFirstSequence는 pinned Window가 있을 경우는 그것으로만 가기 때문에 
	// click하여 입력받기 등을 할 경우 이것이 필요하다.
	virtual void AcquireMouseClick() {}		

	virtual void CancelPushState() {}		// mouse 눌림을 해제할 때.

	virtual void AcquireDisappear() {}		// DisappearWindow가 호출될 떄.

	MOUSE_STATE	GetMouseInputState();

	//
	// Local attributes 
	//
	void	InitAttributes();
	void	AttrTopmost(bool state);
	void	AttrKeyboardControl(bool state);
	void	AttrWindowMove(bool new_state);
	void	AttrPin(bool new_state);
	void	AttrAlpha(bool new_state);
	void	AttrAutoHide(ATTRIBUTES_HIDE new_state);
	void	AttrStatch(bool new_state);
	void	ProcessHide(int gap = 0, bool alpha_window = false);
	
	const Window::ATTRIBUTES *	GetAttributes() const { return &m_attributes; }

	//
	// Window moving
	//
	bool	Moving() const { return m_bl_window_move_ready; }
	void	MoveReady();
	void	SetOrigin(int ox, int oy);
	void	GetOrigin(Point &orgin) const { orgin = m_origin; }

private:
	bool	Move(int new_ox, int new_oy); // 오직 내부에 정의된 InputControl에 의해서 실행된다.
	void	MoveOk();

public:
	void	ClearInputState();

	//
	// Place other Widget.
	//
private:
	SimpleDataList<LineEditorVisual *>		m_sdl_lev;

public:
	void	ShowWidget() const;
	void	Attach(LineEditorVisual * p_lev);
};

//-----------------------------------------------------------------------------
// WindowManager
//
// 하나의 Window가 생성되면 그 Window는 WindowManager에 등록되어 관리된다.
// 관리에는 Window 정렬, Window 우선순위 등이 있다.
//
//
// < 속성 >
//
// 전체 Window 중에서 어떤 Window에 대한 속성을 설정할 수 있다.
//
//------------------------------------------------------------------------------
// < Focused Window >
//
// Focused Window는 두 가지로 분류된다. 하나는 Mouse가 Window 위로 이동하였 때
// 결정되는 Mouse focused Window, 다른 하나는 키보드입력을 비롯하여 입력을 받을
// 수 있는 Acquired Window이다.
//
// Focused Window:
//
// 최우선 Window가 아니더라도 Mouse pointer를 이동하여 어떠한 Window라도 scanning할 
// 수 있다. 이것은 실질적으로 입력작용이 일어나서 Window에 대한 어떠한 처리를 일어나지
// 않게 하는 범위에서 가능한 것이다. 이것을 했다고 Window는 변하지 않는다.
// 반면 scanning과 동시에 Window 닫기버튼을 클릭하므로써 Window의 상태를 변하게하는
// 것은 그 전에 Window가 Acquired된다는 전제하에 이루어지는 것이다.
// 그러므로 focused Window는 Mouse move만을 연속적으로 받는것이다. 다른 것은 없다.
//
//    - Mouse focus가 acquire/unacquire(set/unset)되면 Window에 정의된 아래의 두 method를
//		  실행한다.
//
//		  o AcquireMouseFocus()
//		  o UnacquireMouseFocus()
//
//      그러나 Mouse pointer의 이동 외에 다음과 같이 Mouse focus의 set/unset을 변경해야
//      할 때가 있다.
//
//      o AppearWindow가 실행될 때 - focused Window 위에 새 Window가 출력될 수 있음.
//		  o DisappearWindow가 실행될 때 - focused Window가 disappear 됨.
//		  o Unregister가 실행될 때 - focused Window가 unregister 됨.
//		  * Mouse pointer Move
//
//
// Acquired Window:
//
// Acquired Window는 단 하나의 Window로써 키보드입력과 같은 특수한 작용을 허용하는
// 것이다. Acquired되었다는 것이 그것이 최우선임을 단정지을 수는 없다. 일단은
// 특수한 Window로 정의하자. 왜냐하면 키보드입력을 받는 Window를 지정할 수 있기
// 때문이다. 이것은 Acuqired Window가 아니여도 키보드입력을 받을 수 있음을 의미한다.
//
// 
//-----------------------------------------------------------------------------
typedef std::list<Window *> List;

class WindowManager : public SimpleDataList<Window *>
{
private:
	//
	// handler
	//
	// 공통적인 처리행위를 handler로 정의한다.
	//
	void							(*m_fp_handler_acquire_mousefocus)(void);
	void							(*m_fp_handler_unacquire_mousefocus)(void);
	void							(*m_fp_handler_acquire_disappear)(void);

	//
	// (m_current_mouse_x, m_current_mouse_y)
	//
	// Window가 사라지거나 나타날 때, mouse move focus Window는 바뀔 수 있다.
	// 그 때 새로운 Window를 찾기 위해 필요하다.
	//
	int							m_current_mouse_x, m_current_mouse_y;

	//
	// m_pC_mouse_focused_window
	// 
	// 우선순위의 변경은 사용자의 option(Mouse move로 선택되든 click되서 선택되든)에
	// 맡겨야겠지만, 내부적으로는 사용자가 알든말든 Mouse move focus를 검색한다.
	//
	// 이것은 사용자는 다른 Window를 click하지 않고도 Mouse pointer를 이동해볼 수 
	// 있기 때문이다. 이는 그 Window를 검사하는 의미로 받아들일 수 있다.
	// 그런걸 수행하기 위해서이기도하지만, 정말로 Mouse move focus를 Window 전환으로
	// 원할 수도 있고 이 걸로 다양한 option을 만들수도 있기 때문이다.
	//
	// m_pC_mouse_focused_window는 현재 move focus된 Window의 pointer이다. 이것은 다른
	// 것으로 바뀔 때만 reset된다. 이것이 NULL이면 move focus된 Window가 없다는 것이다.
	//
	Window *						m_pC_mouse_focused_window;
	Window *						m_pC_keydown_window;

	//
	// m_pC_pushed_window
	//
	Window *						m_pC_pushed_window;

	//
	// m_pC_keyboard_control_window
	//
	// m_pC_keyboard_control_window는 출력되고 있는 Window이다.
	//
	// topmost Window가 아닌 Window 들 중에서 Keyboard control을 받는 Window이다.
	// topmost Window를 제외한 모든 Window의 keyboard 입력방법은 이것이다.
	// Window 속성으로 keyboard_control이 설정되어 있으면 AcquireFirstSequence나 AcquireMouseClick가
	// 수행될 때 그 Window를 m_pC_keyboard_control_window로 설정한다.
	//
	Window *						m_pC_keyboard_control_window;

	//
	// m_show_list
	//
	// 보이는 Window의 우선순위를 관리하기 위해서 list를 사용한다.
	//
	List							m_show_list;

	//
	// m_show_list_pinned_window
	//
	// Window의 pin 속성이 설정되면 m_show_list에 저장되지 않고 이 list에 저장된다.
	// 즉, 이 list는 pin 속성이 설정된 Window만 저장된다. m_show_list는 pin 속성이
	// 설정되지 않은 Window만이 저장된다. 이렇게 함으로써 pinned Window와 no pinned Window를
	// 구분할 수 있다. pinned Window가 no pinned Window의 위에 출력된다.
	// pinned Window의 우선순위는 pinned Window끼리 결정하게 된다.
	//
	List							m_show_list_pinned_window;

	//
	// m_pC_topmost_window
	//
	// topmost Window란 출력된 Window들 중 가장 위에 뜨며 이 Window외에는 마우스및 키보드의
	// 모든 입력을 자신만이 독점하는 Window이다.
	//---------------------------------------------------------------------------------------
	// m_pC_topmost_window는 출력되고 있는 Window이다.
	//
	// topmost Window는 여러개 존재할 수 있다. 하나의 topmost Window가 떠 있는 데 또다른
	// topmost Window가 뜰 수 있다는 것이다. 이렇게 되었을 때 제어권은 최우선순위부터 순서대로이다.
	// m_pC_topmost_window는 그들 중 현재의 제어권을 가지고 있는 Window이다. 그러므로 제어권이
	// 바뀔 때는 m_pC_topmost_window를 바꿔줘야 하는 것이다.
	// 복수개의 Window의 topmost 설정은 Window 개개에 save된다. 그들을 참조하여 제어가 가능한
	// topmost Window로써 m_pC_topmost_window를 설정한다.
	//
	Window *						m_pC_topmost_window;

	//
	// m_pC_mouse_click_window
	//
	// topmost Window가 있을 경우를 제외한 어떤 Window를 click하였을 때 설정된다.
	// 같은 Window이면 재설정하지 않는다.
	//
	Window *						m_pC_mouse_click_window;

	int							m_old_origin_x;
	int							m_old_origin_y;
	int							m_res_x, m_res_y; // resolution

	enum DIR
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
		DIRECTION,
	};

	void	SetNextTopmostWindow();
	void	SetNextKeyboardControlWindow();
	void	SetKeyboardControlWindow(Window * p_window);

	void	RunAcquireMouseFocusHandler() { if (m_fp_handler_acquire_mousefocus) m_fp_handler_acquire_mousefocus(); }
	void	RunAcquireDisappearHandler() { if (m_fp_handler_acquire_disappear) m_fp_handler_acquire_disappear(); }
	void	RunUnacquireMouseFocusHandler() { if (m_fp_handler_unacquire_mousefocus) m_fp_handler_unacquire_mousefocus(); }

public:
	WindowManager();
	~WindowManager();

	//
	// Window register/Unregister
	//
	Window*	GetTopmostWindow()	{ return m_pC_topmost_window; }
	void	Register(Window * p_window);
	void	Unregister(Window * p_window);

	bool	AlreadyRegistered(Window * p_window) const;

	void	Process();
	void	Show();

	//
	// Window input control
	// Window의 입력은 WindowManager를 통해서 이루어진다.
	//
	bool	MouseControl(UINT message, int x, int y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	SetResolution(int res_x, int res_y) { m_res_x = res_x; m_res_y = res_y; }

public:
	//
	// set handler
	//
	void	SetAcquireMouseFocusHandler(void (*fp)(void)) { m_fp_handler_acquire_mousefocus = fp; }
	void	SetUnacquireMouseFocusHandler(void (*fp)(void)) { m_fp_handler_unacquire_mousefocus = fp; }
	void	SetAcquireDisappearHandler(void (*fp)(void)) { m_fp_handler_acquire_disappear = fp; }

	Window *GetFocusedWindow(int x, int y);
	//
	// Window priority
	//
private: 

	void	SendMouseMoveMessageToMouseFocusedWindow();

public:
	void	SetMouseMoveFocusedWindow();	// 위험위험-_- 원래 private였는데-ㅅ-

	//
	// Window interface
	//
	void	FirstPriority(Window * p_this_window);
	void	AppearWindow(Window * p_this_window);
	bool	DisappearWindow(Window * p_this_window);
	int	GetSequence(Window * p_this_window);
	Window * GetMouseFocusedWindow() const { return m_pC_mouse_focused_window; }
	Window * GetFirstPriorityWindow() const;
	Window * GetMovingWindow() const;
	bool	CancelPushStateOfCurrentPushedWindow();

	int	ShowedWindowSize() const;
	int	ShowedPinnedWindowSize() const { return m_show_list_pinned_window.size(); }
	//int	RegisteredWindowSize() const { return m_window_buf_size; } // -> use 'Size()'
	bool	GetShowState(Window * p_window) const;
};

extern WindowManager *	gpC_window_manager;

void	g_RegisterWindow(Window * p_window);
void	g_UnregisterWindow(Window * p_window);

#endif
