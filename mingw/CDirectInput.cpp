#include "DirectXlib_PCH.h"
#include <windows.h>
#include "CDirectInput.h"

#define MSB		0x80

CDirectInput*	g_pDXInput = NULL;

const char*		CDirectInput::s_KeyName[256] = 
{ 	
	"NULL",		// 0x00
	"ESCAPE",	//0x01e
	"1",	//0x02
	"2",	//0x03
	"3",	//0x04
	"4",	//0x05
	"5",	//0x06
	"6",	//0x07
	"7",	//0x08
	"8",	//0x09
	"9",	//0x0A
	"0",	//0x0B
	"MINUS",	//0x0C//onmainkeyboard
	"EQUALS",	//0x0D
	"BACK",		//0x0E//backspace
	"TAB",	//0x0F
	"Q",	//0x10
	"W",	//0x11
	"E",	//0x12
	"R",	//0x13
	"T",	//0x14
	"Y",	//0x15
	"U",	//0x16
	"I",	//0x17
	"O",	//0x18
	"P",	//0x19
	"LBRACKET",		//0x1A
	"RBRACKET",		//0x1B
	"RETURN",	//0x1C//Enteronmainkeyboard
	"LCONTROL",		//0x1D
	"A",	//0x1E
	"S",	//0x1F
	"D",	//0x20
	"F",	//0x21
	"G",	//0x22
	"H",	//0x23
	"J",	//0x24
	"K",	//0x25
	"L",	//0x26
	"SEMICOLON",	//0x27
	"APOSTROPHE",	//0x28
	"GRAVE",	//0x29//accentgrave
	"LSHIFT",	//0x2A
	"BACKSLASH",	//0x2B
	"Z",	//0x2C
	"X",	//0x2D
	"C",	//0x2E
	"V",	//0x2F
	"B",	//0x30
	"N",	//0x31
	"M",	//0x32
	"COMMA",	//0x33
	"PERIOD",	//0x34//onmainkeyboard
	"SLASH",	//0x35//onmainkeyboard
	"RSHIFT",	//0x36
	"MULTIPLY",		//0x37//onnumerickeypad
	"LMENU",	//0x38//leftAlt
	"SPACE",	//0x39
	"CAPITAL",	//0x3A
	"F1",	//0x3B
	"F2",	//0x3C
	"F3",	//0x3D
	"F4",	//0x3E
	"F5",	//0x3F
	"F6",	//0x40
	"F7",	//0x41
	"F8",	//0x42
	"F9",	//0x43
	"F10",	//0x44
	"NUMLOCK",	//0x45
	"SCROLL",	//0x46//ScrollLock
	"NUMPAD7",	//0x47
	"NUMPAD8",	//0x48
	"NUMPAD9",	//0x49
	"SUBTRACT",		//0x4A//onnumerickeypad
	"NUMPAD4",	//0x4B
	"NUMPAD5",	//0x4C
	"NUMPAD6",	//0x4D
	"ADD",	//0x4E//onnumerickeypad
	"NUMPAD1",	//0x4F
	"NUMPAD2",	//0x50
	"NUMPAD3",	//0x51
	"NUMPAD0",	//0x52
	"DECIMAL",	//0x53//onnumerickeypad
NULL, // 0x54
NULL, // 0x55
NULL, // 0x56
	"F11",	//0x57
	"F12",	//0x58
NULL, // 0x59
NULL, // 0x5a
NULL, // 0x5b
NULL, // 0x5c
NULL, // 0x5d
NULL, // 0x5e
NULL, // 0x5f
NULL, // 0x60
NULL, // 0x61
NULL, // 0x62
NULL, // 0x63
	"F13",	//0x64//(NECPC98)
	"F14",	//0x65//(NECPC98)
	"F15",	//0x66//(NECPC98)
NULL, // 0x67
NULL, // 0x68
NULL, // 0x69
NULL, // 0x6a
NULL, // 0x6b
NULL, // 0x6c
NULL, // 0x6d
NULL, // 0x6e
NULL, // 0x6f
	"KANA",		//0x70//(Japanesekeyboard)
NULL, // 0x71
NULL, // 0x72
NULL, // 0x73
NULL, // 0x74
NULL, // 0x75
NULL, // 0x76
NULL, // 0x77
NULL, // 0x78
	"CONVERT",	//0x79//(Japanesekeyboard)
NULL, // 0x7a
	"NOCONVERT",	//0x7B//(Japanesekeyboard)
NULL, // 0x7c
	"YEN",	//0x7D//(Japanesekeyboard)
NULL, // 0x7e
NULL, // 0x7f
NULL, // 0x80
NULL, // 0x81
NULL, // 0x82
NULL, // 0x83
NULL, // 0x84
NULL, // 0x85
NULL, // 0x86
NULL, // 0x87
NULL, // 0x88
NULL, // 0x89
NULL, // 0x8a
NULL, // 0x8b
NULL, // 0x8c
	"NUMPADEQUALS",		//0x8D//=onnumerickeypad(NECPC98)
NULL, // 0x8e
NULL, // 0x8f
	"CIRCUMFLEX",	//0x90//(Japanesekeyboard)
	"AT",	//0x91//(NECPC98)
	"COLON",	//0x92//(NECPC98)
	"UNDERLINE",	//0x93//(NECPC98)
	"KANJI",	//0x94//(Japanesekeyboard)
	"STOP",		//0x95//(NECPC98)
	"AX",	//0x96//(JapanAX)
	"UNLABELED",	//0x97//(J3100)
NULL, // 0x98
NULL, // 0x99
NULL, // 0x9a
NULL, // 0x9b
	"NUMPADENTER",	//0x9C//Enteronnumerickeypad
	"RCONTROL",		//0x9D
NULL, // 0x9e
NULL, // 0x9f
NULL, // 0xa0
NULL, // 0xa1
NULL, // 0xa2
NULL, // 0xa3
NULL, // 0xa4
NULL, // 0xa5
NULL, // 0xa6
NULL, // 0xa7
NULL, // 0xa8
NULL, // 0xa9
NULL, // 0xaa
NULL, // 0xab
NULL, // 0xac
NULL, // 0xad
NULL, // 0xae
NULL, // 0xaf
NULL, // 0xb0
NULL, // 0xb1
NULL, // 0xb2
	"NUMPADCOMMA",	//0xB3//,onnumerickeypad(NECPC98)
NULL, // 0xb4
	"DIVIDE",	//0xB5///onnumerickeypad
NULL, // 0xb6
	"SYSRQ",	//0xB7
	"RMENU",	//0xB8//rightAlt
NULL, // 0xb9
NULL, // 0xba
NULL, // 0xbb
NULL, // 0xbc
NULL, // 0xbd
NULL, // 0xbe
NULL, // 0xbf
NULL, // 0xc0
NULL, // 0xc1
NULL, // 0xc2
NULL, // 0xc3
NULL, // 0xc4
NULL, // 0xc5
NULL, // 0xc6
	"HOME",		//0xC7//Homeonarrowkeypad
	"UP",		//0xC8//UpArrowonarrowkeypad
	"PRIOR",	//0xC9//PgUponarrowkeypad
NULL, // 0xca
	"LEFT",		//0xCB//LeftArrowonarrowkeypad
	"RIGHT",	//0xCD//RightArrowonarrowkeypad
NULL, // 0xce
	"END",		//0xCF//Endonarrowkeypad
	"DOWN",		//0xD0//DownArrowonarrowkeypad
	"NEXT",		//0xD1//PgDnonarrowkeypad
	"INSERT",	//0xD2//Insertonarrowkeypad
	"DELETE",	//0xD3//Deleteonarrowkeypad
NULL, // 0xd4
NULL, // 0xd5
NULL, // 0xd6
NULL, // 0xd7
NULL, // 0xd8
NULL, // 0xd9
NULL, // 0xda
	"LWIN",		//0xDB//LeftWindowskey
	"RWIN",		//0xDC//RightWindowskey
	"APPS",		//0xDD//AppMenukey
};

/*-----------------------------------------------------------------------------
- GetMouseAcceleration
- SystemParametersInfo()로 얻은 Mouse 정보를 가지고 Mouse 가속값을 얻는다.
-----------------------------------------------------------------------------*/
int CDirectInput::GetMouseAcceleration(int value)
{
	//
	// 제어판에서 변경할 수 있는 Mouse 속도 설정은 7단계이다.
	//
	// level: (1st threshold, 2nd threshold, acceleration level) -> int m_mouse_info[3]
	//
	// 0: (0,  0, 0)
	// 1: (10, 0, 1)
	// 2: (7,  0, 1)
	// 3: (4,  0, 1)
	// 4: (4, 12, 2)
	// 5: (4,  9, 2)
	// 6: (4,  6, 2)
	//
	// accel level이 0이 아닐 때, value가 첫번째 한계값보다 크면 value를 2배 한다.
	// accel level이 2일 때는 두번째 한계값보다 크면 value를 2배한 값을 또 2배 한다.
	//

	int result = value;
	value = abs(value);

	if (m_mouse_info[ACCEL_LEVEL] > 0)
		if (m_mouse_info[FIRST_THRESHOLD] < value)
			result *= 2;

	if (m_mouse_info[ACCEL_LEVEL] > 1)
		if (m_mouse_info[SECOND_THRESHOLD] < value)
			result *= 2;

	return result;
}

/*-----------------------------------------------------------------------------
- SetMouseSpeed
- 제어판에 설정된 Mouse 속도를 반영한다.
-----------------------------------------------------------------------------*/
void CDirectInput::SetMouseSpeed()
{
	//
	// m_mouse_info는 three integer array이다.
	//
	// m_mouse_info[0] = first test value
	// m_mouse_info[1] = second test value
	// m_mouse_info[2] = mouse acceleration level
	//
	BOOL ret = SystemParametersInfo(SPI_GETMOUSE, 0, &m_mouse_info, 0);

	//DWORD error = GetLastError();
}

/*-----------------------------------------------------------------------------
- SetMouseMoveLimit
- Mouse 이동 입력제한 값 설정.

  `일반적으로 이 값은 screen size이다.
-----------------------------------------------------------------------------*/
void CDirectInput::SetMouseMoveLimit(int x, int y)
{
	// init
	m_mouse_x = 0;
	m_mouse_y = 0;
	m_mouse_z = 0;

	m_limit_x = x;
	m_limit_y = y;
}

/*-----------------------------------------------------------------------------
Mouse 위치를 강제로 설정
-----------------------------------------------------------------------------*/
void		
CDirectInput::SetMousePosition(int x, int y)
{
	m_mouse_x = x;
	m_mouse_y = y;
}

/*-----------------------------------------------------------------------------
- UpdateInput
- Device polling.
-----------------------------------------------------------------------------*/
void CDirectInput::UpdateInput()
{
	OnMouseInput();
	OnKeyboardInput();
}

/*-----------------------------------------------------------------------------
- SetMouseEventReceiver
-
-----------------------------------------------------------------------------*/
void CDirectInput::SetMouseEventReceiver(void (*fp_receiver)(E_MOUSE_EVENT, int, int, int))
{
	m_fp_mouse_event_receiver = fp_receiver;
}

/*-----------------------------------------------------------------------------
- SetMouseEventReceiver
-
-----------------------------------------------------------------------------*/
void CDirectInput::SetKeyboardEventReceiver(void (*fp_receiver)(E_KEYBOARD_EVENT, DWORD))
{
	m_fp_keyboard_event_receiver = fp_receiver;
}

/*-----------------------------------------------------------------------------
- OnKeyboardInput
-
-----------------------------------------------------------------------------*/
void CDirectInput::OnKeyboardInput()
{
    if (m_pKeyboard)
    {
        DIDEVICEOBJECTDATA didod[BUFFER_SIZE];  // Receives buffered data 
        DWORD dwElements;
        HRESULT hr;
        hr = DIERR_INPUTLOST;

        while ( DI_OK != hr )
        {

            dwElements = BUFFER_SIZE;
            hr = m_pKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
                                            didod, 
                                            &dwElements, 
                                            0 );

            if (hr != DI_OK) 
            {
                 // We got an error or we got DI_BUFFEROVERFLOW.
                 //
                 // Either way, it means that continuous contact with the
                 // device has been lost, either due to an external
                 // interruption, or because the buffer overflowed
                 // and some events were lost.
                 //
                 // Consequently, if a button was pressed at the time
                 // the buffer overflowed or the connection was broken,
                 // the corresponding "up" message might have been lost.
                 //
                 // But since our simple sample doesn't actually have
                 // any state associated with button up or down events,
                 // there is no state to reset.  (In a real game, ignoring
                 // the buffer overflow would result in the game thinking
                 // a key was held down when in fact it isn't; it's just
                 // that the "up" event got lost because the buffer
                 // overflowed.)
                 //
                 // If we want to be cleverer, we could do a
                 // GetDeviceState() and compare the current state
                 // against the state we think the device is in,
                 // and process all the states that are currently
                 // different from our private state.
                hr = m_pKeyboard->Acquire();
                if ( FAILED(hr) )  
                    return;
            }
        }

        if ( FAILED(hr) )
            return;

		for (DWORD i = 0; i < dwElements; i++) 
		{
			DWORD key = didod[i].dwOfs & 0xFF;
				
			if (didod[i].dwData & MSB)
			{					
				m_key[key] = TRUE;

				if (m_fp_keyboard_event_receiver)
				{
					m_fp_keyboard_event_receiver(CDirectInput::KEYDOWN, key);
				}
			}
			else
			{
				m_key[key] = FALSE;
				if (m_fp_keyboard_event_receiver)
				{
					m_fp_keyboard_event_receiver(CDirectInput::KEYUP, key);
				}
			}
		}
    }
}

/*-----------------------------------------------------------------------------
- OnMouseInput
-
-----------------------------------------------------------------------------*/
void CDirectInput::OnMouseInput()
{
    BOOL                bDone;
    DIDEVICEOBJECTDATA  od;
    DWORD               dwElements;
    HRESULT             hr;

	 if (!m_pMouse)
		 return;

	// 값 초기화
	m_lb_down	= FALSE;	// left button - down
	m_rb_down	= FALSE;	// right button - down
	m_cb_down	= FALSE;	// center button - down
	m_lb_up		= FALSE;	// left button - up
	m_rb_up		= FALSE;	// right button - up
	m_cb_up		= FALSE;	// center button - up

    // Attempt to read one data element.  Continue as long as
    // device data is available.
    bDone = FALSE;

    while ( !bDone ) 
    {
        dwElements = 1;
        hr = m_pMouse->GetDeviceData( 
                            sizeof(DIDEVICEOBJECTDATA), 
                            &od,
                            &dwElements, 
                            0 );

        //if (hr==DIERR_INPUTLOST	|| hr==DIERR_NOTACQUIRED) 
		if (hr != DI_OK) 
        {
            m_pMouse->Acquire();
            break;
        }

        // Unable to read data or no data available
        if ( FAILED(hr) || dwElements == 0) 
        {
			/*
			int a;
			switch (hr)
			{
				case DI_BUFFEROVERFLOW :
					a = 0;
				break;
				case DI_DOWNLOADSKIPPED :
					a = 0;
				break;
				case DI_EFFECTRESTARTED :
					a = 0;
				break;
				case DI_OK :
					a = 0;
				break;
				case DI_POLLEDDEVICE :
					a = 0;
				break;
				case DI_TRUNCATED :
					a = 0;
				break;
				case DI_TRUNCATEDANDRESTARTED :
					a = 0;
				break;
				case DIERR_ACQUIRED :
					a = 0;
				break;
				case DIERR_ALREADYINITIALIZED :
					a = 0;
				break;
				case DIERR_BADDRIVERVER :
					a = 0;
				break;
				case DIERR_BETADIRECTINPUTVERSION :
					a = 0;
				break;
				case DIERR_DEVICEFULL :
					a = 0;
				break;
				case DIERR_DEVICENOTREG :
					a = 0;
				break;
				case DIERR_EFFECTPLAYING :
					a = 0;
				break;
				case DIERR_HASEFFECTS :
					a = 0;
				break;
				case DIERR_GENERIC :
					a = 0;
				break;
				case DIERR_HANDLEEXISTS :
					a = 0;
				break;
				case DIERR_INCOMPLETEEFFECT :
					a = 0;
				break;
				case DIERR_INPUTLOST :
					a = 0;
				break;
				case DIERR_INVALIDPARAM :
					a = 0;
				break;
				case DIERR_MOREDATA :
					a = 0;
				break;
				case DIERR_NOAGGREGATION :
					a = 0;
				break;
				case DIERR_NOINTERFACE :
					a = 0;
				break;
				case DIERR_NOTACQUIRED :
					a = 0;
				break;
				case DIERR_NOTBUFFERED :
					a = 0;
				break;
				case DIERR_NOTDOWNLOADED :
					a = 0;
				break;
				case DIERR_NOTEXCLUSIVEACQUIRED :
					a = 0;
				break;
				case DIERR_NOTFOUND :
					a = 0;
				break;
				case DIERR_NOTINITIALIZED :
					a = 0;
				break;
				case DIERR_OLDDIRECTINPUTVERSION :
					a = 0;
				break;
				case DIERR_OUTOFMEMORY :
					a = 0;
				break;
				case DIERR_REPORTFULL :
					a = 0;
				break;
				case DIERR_UNPLUGGED :
					a = 0;
				break;
				case DIERR_UNSUPPORTED :
					a = 0;
				break;
			}
			*/

			break;
        }

        // look at the element to see what happened
        switch (od.dwOfs) 
        {     
            case DIMOFS_X:  // Mouse horizontal motion 
					m_mouse_x += GetMouseAcceleration(od.dwData);

					// limit
					if (m_mouse_x < 0)
						m_mouse_x = 0;
					if (m_mouse_x > m_limit_x)
						m_mouse_x = m_limit_x;

					if (m_fp_mouse_event_receiver)
						m_fp_mouse_event_receiver(CDirectInput::MOVE, m_mouse_x, m_mouse_y, m_mouse_z);
                break;

            case DIMOFS_Y:  // Mouse vertical motion 
					m_mouse_y += GetMouseAcceleration(od.dwData);

					// limit
					if (m_mouse_y < 0)
						m_mouse_y = 0;
					if (m_mouse_y > m_limit_y)
						m_mouse_y = m_limit_y;

					if (m_fp_mouse_event_receiver)
						m_fp_mouse_event_receiver(CDirectInput::MOVE, m_mouse_x, m_mouse_y, m_mouse_z);
                break;

            case DIMOFS_Z:       // Mouse vertical motion 
					m_mouse_z += od.dwData; // 일반적으로 wheel의 입도는 120이다.
					if (m_fp_mouse_event_receiver)
					{
						// m_mouse_z을 변환해서 넘겨줄까?
						if ((int)od.dwData < 0)
							m_fp_mouse_event_receiver(CDirectInput::WHEELDOWN, m_mouse_x, m_mouse_y, m_mouse_z);
						else
							m_fp_mouse_event_receiver(CDirectInput::WHEELUP, m_mouse_x, m_mouse_y, m_mouse_z);
					}
                break;

            case DIMOFS_BUTTON0: // Right button pressed or released 
            case DIMOFS_BUTTON1: // Left button pressed or released 
                // is the right or a swapped left button down?
                if ( ( m_bSwapMouseButtons  && DIMOFS_BUTTON1 == od.dwOfs ) ||
                     ( !m_bSwapMouseButtons && DIMOFS_BUTTON0 == od.dwOfs ) )
                {
						 if ( od.dwData & MSB ) 
						{ 
								m_lb_down = TRUE;
								if (m_fp_mouse_event_receiver)
									m_fp_mouse_event_receiver(CDirectInput::LEFTDOWN, m_mouse_x, m_mouse_y, m_mouse_z);
						}
						  else
						  {
								//m_lb_down = FALSE;
								m_lb_up = TRUE;
								if (m_fp_mouse_event_receiver)
									m_fp_mouse_event_receiver(CDirectInput::LEFTUP, m_mouse_x, m_mouse_y, m_mouse_z);
						  }
                }

                // is the left or a swapped right button down?
                if ( ( m_bSwapMouseButtons  && DIMOFS_BUTTON0 == od.dwOfs ) ||
                     ( !m_bSwapMouseButtons && DIMOFS_BUTTON1 == od.dwOfs ) )
                {
                    if ( od.dwData & MSB ) 
                    {  
							m_rb_down = TRUE;
							if (m_fp_mouse_event_receiver)
								m_fp_mouse_event_receiver(CDirectInput::RIGHTDOWN, m_mouse_x, m_mouse_y, m_mouse_z);
					}
					  else
					  {
							//m_rb_down = FALSE;
							m_rb_up = TRUE;
							if (m_fp_mouse_event_receiver)
								m_fp_mouse_event_receiver(CDirectInput::RIGHTUP, m_mouse_x, m_mouse_y, m_mouse_z);
					  }
                }

					 bDone = TRUE; 
                break;

				case DIMOFS_BUTTON2:
					  if ( od.dwData & MSB ) 
					  {  
							m_cb_down = TRUE;
								if (m_fp_mouse_event_receiver)
									m_fp_mouse_event_receiver(CDirectInput::CENTERDOWN, m_mouse_x, m_mouse_y, m_mouse_z);
					  }
					  else
					  {
						  //m_cb_down = FALSE;
						  m_cb_up = TRUE;
								if (m_fp_mouse_event_receiver)
									m_fp_mouse_event_receiver(CDirectInput::CENTERUP, m_mouse_x, m_mouse_y, m_mouse_z);
					  }

					  bDone = TRUE;
					break;
        }
    }
}

/*-----------------------------------------------------------------------------
- FreeDirectInput
-
-----------------------------------------------------------------------------*/
void CDirectInput::FreeDirectInput()
{
    // Unacquire and release any DirectInputDevice objects.
    if ( m_pMouse ) 
    {
        // Unacquire the device one last time just in case 
        // the app tried to exit while the device is still acquired.
        m_pMouse->Unacquire();

        m_pMouse->Release();
        m_pMouse = NULL;
    }

    if ( m_pKeyboard ) 
    {
        // Unacquire the device one last time just in case 
        // the app tried to exit while the device is still acquired.
        m_pKeyboard->Unacquire();

        m_pKeyboard->Release();
        m_pKeyboard = NULL;
    }

    // Release any DirectInput objects.
    if ( m_pDI ) 
    {
        m_pDI->Release();
        m_pDI = NULL;
    }
}

/*-----------------------------------------------------------------------------
- CDirectInput
-
-----------------------------------------------------------------------------*/
CDirectInput::CDirectInput()
{
	m_pDI				= NULL;
	m_pMouse			= NULL;
	m_pKeyboard		= NULL;
	m_mouse_x		= 0;
	m_mouse_y		= 0;
	m_mouse_z		= 0;	
	m_limit_x		= 0;
	m_limit_y		= 0;
	m_mouse_info[0]=0;
	m_mouse_info[1]=0;
	m_mouse_info[2]=0;

	m_fp_mouse_event_receiver = NULL;
	m_fp_keyboard_event_receiver = NULL;
	
	Clear();
}

/*-----------------------------------------------------------------------------
- ~CDirectInput
-
-----------------------------------------------------------------------------*/
CDirectInput::~CDirectInput()
{
	FreeDirectInput();
}

/*-----------------------------------------------------------------------------
- SetAcquire
- Set DirectInput acquire.
-----------------------------------------------------------------------------*/
HRESULT CDirectInput::SetAcquire(bool active_app)
{
   // nothing to do if m_pMouse is NULL 
	if (m_pMouse)
	 {
		 if (active_app)
		 {
			  // acquire the input device 
			  m_pMouse->Acquire();
		 } 
		 else 
		 {
			  // unacquire the input device 
			  m_pMouse->Unacquire();
		 }

		 //
		 // 제어판에서 Mouse Speed를 조절할 때 Acquire를 다시 하므로 지금 해준다.
		 //
		 SetMouseSpeed();
	 }

    // nothing to do if m_pKeyboard is NULL
    if (m_pKeyboard)
	 {
		 if (active_app)
		 {
			  // acquire the input device 
			  m_pKeyboard->Acquire();
		 } 
		 else 
		 {
			  // unacquire the input device 
			  m_pKeyboard->Unacquire();
		 }
	 }

	 if (!m_pMouse || !m_pKeyboard)
		 return S_FALSE;

    return S_OK;
}

//------------------------------------------------------------------------
// Clear
//------------------------------------------------------------------------
// Input값을 초기화 시킨다.
//------------------------------------------------------------------------
void
CDirectInput::Clear()
{
	for (int i=0; i<256; i++)
	{
		m_key[i] = FALSE;
	}
	
	m_lb_down = FALSE;	// left button - down
	m_rb_down = FALSE;	// right button - down
	m_cb_down = FALSE;	// center button - down
	m_lb_up = FALSE;	// left button - up
	m_rb_up = FALSE;	// right button - up
	m_cb_up = FALSE;
}

/*-----------------------------------------------------------------------------
- InitDI
- Direct Input을 초기화한다.

  `DirectDraw를 사용하지 않고 DirectInput만 사용할 수 있으므로 Window handle을 
   인자로 받는다. 곧, hWnd에 독점 DirectInput를 사용한다.
-----------------------------------------------------------------------------*/
HRESULT CDirectInput::InitDI(HWND hWnd, HINSTANCE hInst, E_EXCLUSIVE ex)
{
    HRESULT hr;

    // Register with the DirectInput subsystem and get a pointer
    // to a IDirectInput interface we can use.
    hr = DirectInputCreate( hInst, DIRECTINPUT_VERSION, &m_pDI, NULL );
    if ( FAILED(hr) ) 
        return hr;

    // Obtain an interface to the system mouse device.
    hr = m_pDI->CreateDevice( GUID_SysMouse, &m_pMouse, NULL );
    if ( FAILED(hr) ) 
        return hr;

	 // Obtain an interface to the system keyboard device.
    hr = m_pDI->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, NULL );
    if ( FAILED(hr) ) 
        return hr;

    // Set the data format to "mouse format" - a predefined data format 
    //
    // A data format specifies which controls on a device we
    // are interested in, and how they should be reported.
    //
    // This tells DirectInput that we will be passing a
    // DIMOUSESTATE structure to IDirectInputDevice::GetDeviceState.
    hr = m_pMouse->SetDataFormat( &c_dfDIMouse );
    if ( FAILED(hr) ) 
        return hr;

    // Set the data format to "keyboard format" - a predefined data format 
    //
    // A data format specifies which controls on a device we
    // are interested in, and how they should be reported.
    //
    // This tells DirectInput that we will be passing an array
    // of 256 bytes to IDirectInputDevice::GetDeviceState.
    hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard );
    if ( FAILED(hr) ) 
        return hr;

    // Set the cooperativity level to let DirectInput know how
    // this device should interact with the system and with other
    // DirectInput applications.
	 DWORD mouse_flag;
	 if (ex == EXCLUSIVE)
		 mouse_flag = DISCL_EXCLUSIVE | DISCL_FOREGROUND;
	 else
		 mouse_flag = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND;

	 hr = m_pMouse->SetCooperativeLevel( hWnd, mouse_flag);
    if ( FAILED(hr) ) 
        return hr;

    // Set the cooperativity level to let DirectInput know how
    // this device should interact with the system and with other
    // DirectInput applications.
	 //
	 // Keyboard에 대해서는 Windows가 항상 Exclusive로 사용하므로 Application에서는
	 // None-Exclusive로 밖에 할 수 없다. 이것은 언제든지 alt+tab, ctrl+alt+del과 
	 // 같은 키입력을 유효하게 하기 위함이다.
	 //
    hr = m_pKeyboard->SetCooperativeLevel( hWnd, 
                                        DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    if ( FAILED(hr) ) 
        return hr;

    // setup the buffer size for the mouse data
    DIPROPDWORD dipdw;

    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = BUFFER_SIZE;

    hr = m_pMouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
    if ( FAILED(hr) ) 
        return hr;

    // not necessary, but nice for left handed users that have
    // their swapped mouse buttons
    m_bSwapMouseButtons = GetSystemMetrics( SM_SWAPBUTTON );

    // IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
    //
    // DirectInput uses unbuffered I/O (buffer size = 0) by default.
    // If you want to read buffered data, you need to set a nonzero
    // buffer size.
    //
    // Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
    //
    // The buffer size is a DWORD property associated with the device.
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = BUFFER_SIZE;

    hr = m_pKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
    if ( FAILED(hr) ) 
        return hr;

	 // change to acquire mode
	 SetAcquire(true);

    return S_OK;
}

/*-----------------------------------------------------------------------------
- Init
-
-----------------------------------------------------------------------------*/
BOOL CDirectInput::Init(HWND hWnd, HINSTANCE hInst, E_EXCLUSIVE ex)
{
	if (InitDI(hWnd, hInst, ex) == S_OK)
	{
		return TRUE;
	}
	
	return FALSE;
		//MessageBox(NULL, 
		//				"Error Initializing DirectInput", 
		//				"DX Library", 
		//				MB_ICONERROR | MB_OK);
}
