//-----------------------------------------------------------------------------
// CWinMain.cpp
//-----------------------------------------------------------------------------

#include <stdio.h>
#include "CWinMain.h"
#include "CWinUpdate.h"

//-----------------------------------------------------------------------------
// static members
//-----------------------------------------------------------------------------
CWinMain::FUNCTION_MESSAGE	CWinMain::m_WMFunctions[WM_USER] = { NULL, };

//-----------------------------------------------------------------------------
//
// constructor/destructor
//
//-----------------------------------------------------------------------------
CWinMain::CWinMain()
{
	m_hWnd = NULL;
	m_hInstance = NULL;
	m_bActiveApp = FALSE;
	m_pProgramName = NULL;

	m_pWinUpdate = NULL;

	// Default Window Message Functions
	SetWMFunction(WM_DESTROY, OnDestroy);
}

CWinMain::~CWinMain()
{
	if (m_pProgramName!=NULL)
	{
		delete [] m_pProgramName;
		m_pProgramName = NULL;
	}

	Release();
}

//-----------------------------------------------------------------------------
//
// member functions
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Error
//-----------------------------------------------------------------------------
void 
CWinMain::Error(HWND hWnd, LPCSTR pProgramName, LPCTSTR szError,...)
{
    char		szBuff[128];
    va_list		vl;

    va_start(vl, szError);
    vsprintf(szBuff, szError, vl);

    //ReleaseAllObjects();

    MessageBox(hWnd, szBuff, pProgramName, MB_OK);
    DestroyWindow(hWnd);
    va_end(vl);
}

//-----------------------------------------------------------------------------
// Set WM Function
//-----------------------------------------------------------------------------
BOOL			
CWinMain::SetWMFunction(UINT message, FUNCTION_MESSAGE function)
{
	if (message < WM_USER)
	{
		m_WMFunctions[message] = function;

		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// WM Process
//-----------------------------------------------------------------------------
long FAR PASCAL 
CWinMain::WMProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//---------------------------------------------------------
	// 적절한 Window Message인 경우에..
	//---------------------------------------------------------
	if (message < WM_USER)
	{
		//---------------------------------------------------------
		// 그 message에 대한 handling code가 있는 경우
		//---------------------------------------------------------
		if (m_WMFunctions[message]!=NULL)
		{
			(*m_WMFunctions[message])(wParam, lParam);
		}

		//---------------------------------------------------------	
		// 없으면 기본~~을 부른다..
		//---------------------------------------------------------
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	
	//---------------------------------------------------------
	// 뭐지.. - -;
	// 혹시.. WM_USER를 넘는 Window Message도 있지 않을까? 흠냐.
	//---------------------------------------------------------
	//Error(hWnd, NULL, "Unknown Window Message : %d", message);

	return -1;	// 흠? - -;;		
}

//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
BOOL
CWinMain::Init(const WINMAIN_INFO* pInfo)
{
	//---------------------------------------------------------------
	// 기본 정보 설정
	//---------------------------------------------------------------
	m_hInstance = pInfo->hInstance;	

	// Program 이름 설정
	if (m_pProgramName!=NULL)
	{
		delete [] m_pProgramName;		
	}

	m_pProgramName = new char [strlen(pInfo->ProgramName) + 1];
	strcpy(m_pProgramName, pInfo->ProgramName);



	//---------------------------------------------------------------
	// WNDCLASS를 생성해서 등록
	//---------------------------------------------------------------
	WNDCLASS                    wc;

    // Set up and register window class
    wc.style			= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc		= WMProcess;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= m_hInstance;
    wc.hIcon			= NULL;
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH )GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName		= NULL;
    wc.lpszClassName	= m_pProgramName;
    RegisterClass(&wc);

	//---------------------------------------------------------------
	// Window Style에 맞게 값을 설정한다.
	//---------------------------------------------------------------
    int cx,cy;	
	DWORD style;
	DWORD exStyle;

	switch (pInfo->Mode)
	{
		case WINMAIN_WINDOW_STYPE_FULLSCREEN :
			exStyle = WS_EX_TOPMOST;// | WS_EX_APPWINDOW;
			style = WS_POPUP;
			cx = GetSystemMetrics(SM_CXSCREEN);
			cy = GetSystemMetrics(SM_CYSCREEN);				  
		break;
	
		case WINMAIN_WINDOW_STYPE_WINDOWMODE :
			exStyle = WS_EX_APPWINDOW;
			style = WS_OVERLAPPED | WS_THICKFRAME | WS_MINIMIZEBOX;
			cx = pInfo->SizeX + GetSystemMetrics(SM_CXSIZEFRAME)*2;
			cy = pInfo->SizeY + GetSystemMetrics(SM_CYSIZEFRAME)*2+GetSystemMetrics(SM_CYMENU);	
		break;

		default :
			Error(m_hWnd, m_pProgramName, "Wrong WinMain WindowStyle!");
		return false;
	}

	//---------------------------------------------------------------
	// Create a window
	//---------------------------------------------------------------
    m_hWnd = CreateWindowEx(exStyle,
							  m_pProgramName,
							  m_pProgramName,
							  style,
							  0,
							  0,
							  cx,
							  cy,
							  NULL,
							  NULL,
							  m_hInstance,
							  NULL);

    if (!m_hWnd)
	{
		Error(m_hWnd, m_pProgramName, "Can't Create Window!");
        return false;
	}

	// 제대로 초기화 됐다고 표시..
	m_bActiveApp = TRUE;

	return true;
}

//-----------------------------------------------------------------------------
// Release
//-----------------------------------------------------------------------------
void
CWinMain::Release()
{
}

//-----------------------------------------------------------------------------
// Window를 나타나게 한다.
//-----------------------------------------------------------------------------
void
CWinMain::ShowWindow()
{
	::ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
    SetFocus(m_hWnd);
}


//-----------------------------------------------------------------------------
// Loop
//-----------------------------------------------------------------------------
void
CWinMain::Loop()
{
	MSG		msg;

    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {	
            if (!GetMessage(&msg, NULL, 0, 0))
                return;// msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else if (m_bActiveApp)
        {
			Update();
	    }
        else
        {
			//------------------------------------------------
            // Make sure we go to sleep if we have nothing else to do
			//------------------------------------------------
            WaitMessage();
        }
    }
}

//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
void
CWinMain::Update()
{
	if (m_pWinUpdate!=NULL)
	{
		m_pWinUpdate->Update();
	}
}

//-----------------------------------------------------------------------------
// Default Message Functions
//-----------------------------------------------------------------------------
void
CWinMain::OnDestroy(WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
}