#include <windows.h>
#include "CDirectInput.h"

#define MSB		0x80

CDirectInput*	g_pDXInput = NULL;

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

        if (hr == DIERR_INPUTLOST) 
        {
            m_pMouse->Acquire();
            break;
        }

        // Unable to read data or no data available
        if ( FAILED(hr) || dwElements == 0) 
        {
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
void CDirectInput::Init(HWND hWnd, HINSTANCE hInst, E_EXCLUSIVE ex)
{
	if (InitDI(hWnd, hInst, ex) != S_OK)
		MessageBox(NULL, 
						"Error Initializing DirectInput", 
						"DX Library", 
						MB_ICONERROR | MB_OK);
}