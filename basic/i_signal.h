/*-----------------------------------------------------------------------------

	i_signal.h

	input signal.

	19980806. KJTINC	

-----------------------------------------------------------------------------*/

#ifndef __I_SIGNAL_H__
#define __I_SIGNAL_H__

//
// keyboard scan code
// 2000.12.01.
//
#define SCANCODE_TILDE					41
#define SCANCODE_ESC					1
#define SCANCODE_SPACE					57
#define SCANCODE_TAB					15

#define SCANCODE_1						2
#define SCANCODE_2						3
#define SCANCODE_3						4
#define SCANCODE_4						5
#define SCANCODE_5						6
#define SCANCODE_6						7
#define SCANCODE_7						8
#define SCANCODE_8						9
#define SCANCODE_9						10
#define SCANCODE_0						11

#define SCANCODE_LBRACKET				0x1A
#define SCANCODE_RBRACKET				0x1B
#define SCANCODE_SEMICOLON				0x27
#define SCANCODE_APOSTROPHE				0x28
#define SCANCODE_GRAVE					0x29    /* accent grave */
#define SCANCODE_LSHIFT					0x2A
#define SCANCODE_BACKSLASH				0x2B
#define SCANCODE_COMMA					0x33
#define SCANCODE_PERIOD					0x34    /* . on main keyboard */
#define SCANCODE_SLASH					0x35    /* / on main keyboard */

#define SCANCODE_A						30
#define SCANCODE_B						48
#define SCANCODE_C						46
#define SCANCODE_D						32
#define SCANCODE_E						18
#define SCANCODE_F						33
#define SCANCODE_G						34
#define SCANCODE_H						35
#define SCANCODE_I						23
#define SCANCODE_J						36
#define SCANCODE_K						37
#define SCANCODE_L						38
#define SCANCODE_M						50
#define SCANCODE_N						49
#define SCANCODE_O						24
#define SCANCODE_P						25
#define SCANCODE_Q						16
#define SCANCODE_R						19
#define SCANCODE_S						31
#define SCANCODE_T						20
#define SCANCODE_U						22
#define SCANCODE_V						47
#define SCANCODE_W						17
#define SCANCODE_X						45
#define SCANCODE_Y						21
#define SCANCODE_Z						44

//#define BACKSPACE							0x08
//#define TAB									0x09

/*-----------------------------------------------------------------------------
  키보드 입력시 문자입력 or 일반 키 입력 정의

  `문자입력시에는 INPUT_CHAR를 지정한다.
-----------------------------------------------------------------------------*/
typedef enum
{
   INPUT_FUNCTION,
   INPUT_CHAR,
   INPUT_ETC,

} E_INPUT_TYPE;

/*-----------------------------------------------------------------------------
  KEYBOARD SIGNAL DEFINES
-----------------------------------------------------------------------------*/
typedef enum
{
   F1,
   F2,
   F3,
   F4,
   F5,
   F6,
   F7,
   F8,
   F9,
   F10,
   F11,
   F12,
	K_LEFT,
	K_RIGHT,
	K_UP,
	K_DOWN,
	K_ENTER,
	K_SPACEBAR,
	K_BACKSPACE,
	K_TAB,
	K_HOME,
	K_END,
	K_DELETE,
	K_INSERT,
	K_ESCAPE,
	K_ADD,
	K_SUBTRACT,
	K_DIVIDE,
	K_MULTIPLY,
	K_PAGEUP,
	K_PAGEDOWN,

} E_KEYBOARD;

/*-----------------------------------------------------------------------------
  MOUSE SIGNAL DEFINES
-----------------------------------------------------------------------------*/
typedef enum
{
	// wheel!
	M_WHEEL_UP,
	M_WHEEL_DOWN,

	M_MOVING,
	M_LEFTBUTTON_DOWN,
	M_LEFTBUTTON_UP,
	M_LB_DOUBLECLICK,
	M_RIGHTBUTTON_DOWN,
	M_RIGHTBUTTON_UP,
	M_CENTERBUTTON_DOWN,
	M_CENTERBUTTON_UP,

} E_MOUSE;

#endif