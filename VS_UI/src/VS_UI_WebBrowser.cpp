


#include "client_PCH.h"

#pragma warning(disable:4786)
#include "VS_UI_GameCommon.h"
#include "VS_UI.h"
#include "VS_UI_filepath.h"
#include "VS_UI_mouse_pointer.h"
#include "VS_UI_WebBrowser.h"
#include "Mshtml.h"
#include "CWebPage.h"

unsigned char WindowCount = 0;

// The class name of our Window to host the browser. It can be anything of your choosing.
static const TCHAR	ClassName[] = "Web Explorer";

// Where we store the pointers to CWebPage.dll's functions
EmbedBrowserObjectPtr		*lpEmbedBrowserObject;
UnEmbedBrowserObjectPtr		*lpUnEmbedBrowserObject;
DisplayHTMLPagePtr			*lpDisplayHTMLPage;
DisplayHTMLStrPtr			*lpDisplayHTMLStr;


LRESULT CALLBACK WindowProcChild(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)	
	{
	case WM_CREATE	:
		{
			if ((*lpEmbedBrowserObject)(hwnd)) return(-1);
			++WindowCount;
			return(0);
		}
		break ; 
	
	case WM_DESTROY	:	
		{
			(*lpUnEmbedBrowserObject)(hwnd);
			--WindowCount;
			return(TRUE);
		}
		break ;
	}
	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

//-----------------------------------------------------------------------------
// C_VS_UI_WEBBROWSER
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_WEBBROWSER::C_VS_UI_WEBBROWSER()
{
	AttrTopmost(false);
	AttrPin(true);
	g_RegisterWindow(this);	
	
	m_pC_button_group	= NULL;
	m_pC_WebBrowser_spk = NULL;

	m_pC_WebBrowser_spk = new C_SPRITE_PACK(SPK_WEBBROWSER);

	m_HIWebDll			= NULL;
	m_hWnd_Explorer		= NULL;
	m_IsMouseInWebBrowser = false;  
	Set(0, 0, 800 , 600);

	m_pC_button_group  = new ButtonGroup(this);

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(779 - m_pC_WebBrowser_spk->GetWidth(WEBBROWSER_CLOSE_BUTTON) , 5 , m_pC_WebBrowser_spk->GetWidth(WEBBROWSER_CLOSE_BUTTON), m_pC_WebBrowser_spk->GetHeight(WEBBROWSER_CLOSE_BUTTON), WEBBROWSER_CLOSE, this, WEBBROWSER_CLOSE_BUTTON));
}

C_VS_UI_WEBBROWSER::~C_VS_UI_WEBBROWSER()
{
	DestroyWindow(m_hWnd_Explorer)	; 
	FreeLibrary(m_HIWebDll);
	m_IsMouseInWebBrowser = false;

	g_UnregisterWindow(this);
	DeleteNew(m_pC_button_group);	
	gpC_base->SendMessage(UI_MARKET_ACCOUNT ,  0 ,  1) ;
}

bool C_VS_UI_WEBBROWSER::Start(HWND hwndApp,  char* pURL, HINSTANCE hInst) //   void* pWebOjbect )
{
	PI_Processor::Start();
	m_pC_button_group->Init();
	gpC_window_manager->AppearWindow(this);


	//HRESULT hr;

	WNDCLASSEX		wc	;


	if ((m_HIWebDll = LoadLibrary("DarkedenWeb.dll")))
	{
		lpEmbedBrowserObject = (EmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)m_HIWebDll, "EmbedBrowserObject");

		lpUnEmbedBrowserObject = (UnEmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)m_HIWebDll, "UnEmbedBrowserObject");

		lpDisplayHTMLPage = (DisplayHTMLPagePtr *)GetProcAddress((HINSTANCE)m_HIWebDll, "DisplayHTMLPage");
		
		lpDisplayHTMLStr = (DisplayHTMLStrPtr *)GetProcAddress((HINSTANCE)m_HIWebDll, "DisplayHTMLStr");

		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hInstance = hInst;

		wc.lpfnWndProc = WindowProcChild;
		wc.lpszClassName = &ClassName[0];
		RegisterClassEx(&wc);


		if ((m_hWnd_Explorer = CreateWindowEx(0, ClassName, "Microsoft's web site", WS_CHILD |WS_VISIBLE ,
						25, 25, 754, 553,
						hwndApp, NULL, hInst, 0)))
		{

			(*lpDisplayHTMLPage)(m_hWnd_Explorer , pURL) ; 

		}
	}
	else	return false ;
	return true;
}

//-----------------------------------------------------------------------------
// GetMouseCursorInfo
//	return value == 0 : 변경할 필요 없음
// 	return value == 1 : ShowCursor(TRUE)
//	return value == -1 : ShowCursor(FALSE)
//-----------------------------------------------------------------------------
int C_VS_UI_WEBBROWSER::GetMouseCursorInfo()
{
	bool IsInRect = IsInRectPoint(gpC_mouse_pointer->GetX(), gpC_mouse_pointer->GetY());
			
	if(IsInRect && m_IsMouseInWebBrowser == false)
	{
		m_IsMouseInWebBrowser = true;
		return 1;

	}
	else if(!IsInRect && m_IsMouseInWebBrowser == true)
	{
		m_IsMouseInWebBrowser = false;
		return -1;
	}

	return 0;
}

bool C_VS_UI_WEBBROWSER::IsInRectPoint(int X, int Y)
{
	RECT rc;
	if( 0 != GetWindowRect( m_hWnd_Explorer, &rc))
	{
		bool IsInRect = rc.left < X &&
						rc.right > X &&
						rc.top < Y &&
						rc.bottom > Y;
		return IsInRect;
	}

	return false;
}

void	C_VS_UI_WEBBROWSER::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetFocusState())
	{
		if(p_button->GetPressState())
			m_pC_WebBrowser_spk->BltLocked(x + p_button->x, y + p_button->y, p_button->m_image_index+1);
		else
			m_pC_WebBrowser_spk->BltLocked(x + p_button->x, y + p_button->y, p_button->m_image_index+2);
	}
	else
	{
		if(p_button->GetPressState())
			m_pC_WebBrowser_spk->BltLocked(x + p_button->x, y + p_button->y, p_button->m_image_index + 1);
		else
			m_pC_WebBrowser_spk->BltLocked(x + p_button->x, y + p_button->y, p_button->m_image_index);
	}
}

void	C_VS_UI_WEBBROWSER::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
}

void	C_VS_UI_WEBBROWSER::WindowEventReceiver(id_t event)
{
}

void	C_VS_UI_WEBBROWSER::Run(id_t id)
{
		switch (id)
	{
	
	case WEBBROWSER_CLOSE:	// 결제 웹 페이지 구동 
		gC_vs_ui.CloseWebBrowser();
		 //gpC_base->SendMessage(UI_MARKET_ACCOUNT ,  0) ;
		  break ;
		}
}

void	C_VS_UI_WEBBROWSER::Process()
{
	ProcessHide();
	m_pC_button_group->Process();
}

void	C_VS_UI_WEBBROWSER::Show()
{
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_WebBrowser_spk->BltLocked(x, y, WEBBROWSER_MAIN);
		m_pC_button_group->Show()  ; 
	}
	gpC_base->m_p_DDSurface_back->Unlock();
}

bool	C_VS_UI_WEBBROWSER::IsPixel(int _x, int _y)
{
	if(Moving()) return true;
	return IsInRect(_x, _y);
}

bool	C_VS_UI_WEBBROWSER::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	m_pC_button_group->MouseControl(message, _x, _y);
	return	true ; 
}


//extern IWebBrowser2*			g_pWebBrowser = NULL; 
//-----------------------------------------------------------------------------
// C_VS_UI_WEBBROWSER
//
// 
//-----------------------------------------------------------------------------
//C_VS_UI_WEBBROWSER::C_VS_UI_WEBBROWSER()
//{
//	m_HIWebDll	  = NULL;		
//	m_pWebBrowser = NULL;
//	m_IsMouseInWebBrowser = false;
//}
/*
//-----------------------------------------------------------------------------
// ~C_VS_UI_WEBBROWSER
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_WEBBROWSER::~C_VS_UI_WEBBROWSER()
{
	Finish();
	//if(m_pWebBrowser)
	//{
//		m_pWebBrowser->Release();
	//	m_pWebBrowser = NULL;
	//}
	
	//SendMessage(m_hWnd_Explorer, WM_DESTROY, 0, 0); 
	DestroyWindow(m_hWnd_Explorer)	; 
	FreeLibrary(m_HIWebDll);
	//FreeLibrary(m_HIWebDll);
	//m_pWebBrowser = NULL;
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
/*bool C_VS_UI_WEBBROWSER::Start(HWND hwndApp,  char* pURL, HINSTANCE hInst) //   void* pWebOjbect )
{
	//HRESULT hr;

	WNDCLASSEX		wc	;


	if ((m_HIWebDll = LoadLibrary("DarkedenWeb.dll")))
	{
		lpEmbedBrowserObject = (EmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)m_HIWebDll, "EmbedBrowserObject");

		lpUnEmbedBrowserObject = (UnEmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)m_HIWebDll, "UnEmbedBrowserObject");

		lpDisplayHTMLPage = (DisplayHTMLPagePtr *)GetProcAddress((HINSTANCE)m_HIWebDll, "DisplayHTMLPage");
		
		lpDisplayHTMLStr = (DisplayHTMLStrPtr *)GetProcAddress((HINSTANCE)m_HIWebDll, "DisplayHTMLStr");

		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hInstance = hInst;

		wc.lpfnWndProc = WindowProcChild;
		wc.lpszClassName = &ClassName[0];
		RegisterClassEx(&wc);


		if ((m_hWnd_Explorer = CreateWindowEx(0, ClassName, "Microsoft's web site", WS_CHILD |WS_VISIBLE ,
						25, 25, 750, 550,
						hwndApp, NULL, hInst, 0)))
		{

			(*lpDisplayHTMLPage)(m_hWnd_Explorer , pURL) ; 
		}
	}



	//Finish();

	//m_pWebBrowser = (IWebBrowser2*)pWebOjbect;
	/*if (hr = FAILED(CoCreateInstance(CLSID_InternetExplorer,NULL,
			CLSCTX_LOCAL_SERVER, IID_IWebBrowser2,(LPVOID*)&m_pWebBrowser))) 
	{
		//MessageBox(NULL, "IWebBrowser2 Create failed", "ERROR", MB_OK);
		return false;
	}

	m_pWebBrowser->get_HWND((LONG*)&m_hWnd_Explorer);

	//g_pWebBrowser->get_HWND((LONG*)&hExplorer);
	SetParent((HWND)m_hWnd_Explorer, (HWND)hwndApp);

	if(NULL == m_hWnd_Explorer)	return false;

	DWORD dwStyle;
	dwStyle = GetWebBrowserStyle();
	dwStyle &= ~WS_SYSMENU;
	dwStyle &= ~WS_CAPTION;
	dwStyle |= WS_CHILD;

	SetWebBrowserStyle(dwStyle);
	
	
	m_pWebBrowser->put_Width(750);                  // 가로 폭
	m_pWebBrowser->put_Height(550);                //  세로 폭
	m_pWebBrowser->put_Left(25);
	m_pWebBrowser->put_Top(25);
	m_pWebBrowser->put_AddressBar(FALSE);  // 주소창 없앰
	m_pWebBrowser->put_MenuBar(FALSE);
	m_pWebBrowser->put_StatusBar(FALSE);
	m_pWebBrowser->put_ToolBar(FALSE);
	m_pWebBrowser->put_Silent(TRUE);
	m_pWebBrowser->put_Resizable(FALSE);
	m_pWebBrowser->put_FullScreen(FALSE);

	
//_BLANK	//_PARENT	//_SELF	//_TOP

	CComBSTR url(pURL);
	CComVariant vtEmpty;
	hr = m_pWebBrowser->Navigate(url, &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty);
	
	//SetParent((HWND)m_hWnd_Explorer, (HWND)hwndApp);
	SetFocus(hwndApp);
	m_pWebBrowser->put_Visible( TRUE );  // Works fine...
	
	//SetParent(m_hWnd_Explorer, hwndApp);
	//SetActiveWindow(m_hWnd_Explorer) ; 
	//SetForegroundWindow(hwndApp) ;*/
//	return true;
/*}

//-----------------------------------------------------------------------------
// SetWebBrowserSize
//
// 
//-----------------------------------------------------------------------------
//void C_VS_UI_WEBBROWSER::SetWebBrowserSize(int StartX, int StartY, int Width, int Height)
//{
	//if(		StartX < gpC_mouse_pointer->GetX() &&
	//		StartX + Width > gpC_mouse_pointer->GetX() &&
	//		StartY < gpC_mouse_pointer->GetY() &&
	//		StartY + Height > gpC_mouse_pointer->GetY())
	//	m_IsMouseInWebBrowser = true;

	//MoveWindow(m_hWnd_Explorer, StartX, StartY, Width, Height, true);
//}

//-----------------------------------------------------------------------------
// SetWebBrowserStyle
//
// 
//-----------------------------------------------------------------------------
//void C_VS_UI_WEBBROWSER::SetWebBrowserStyle(DWORD dwstyle)
//{
	//SetWindowLong( m_hWnd_Explorer, GWL_STYLE, dwstyle ); 
//}
//-----------------------------------------------------------------------------
// GetWebBrowserStyle
//
// 
//-----------------------------------------------------------------------------
//DWORD C_VS_UI_WEBBROWSER::GetWebBrowserStyle()
//{
//	return 0 ; // GetWindowLong( m_hWnd_Explorer, GWL_STYLE );	
//}

//-----------------------------------------------------------------------------
// GetMouseCursorInfo
//	return value == 0 : 변경할 필요 없음
// 	return value == 1 : ShowCursor(TRUE)
//	return value == -1 : ShowCursor(FALSE)
//-----------------------------------------------------------------------------
int C_VS_UI_WEBBROWSER::GetMouseCursorInfo()
{
	bool IsInRect = IsInRectPoint(gpC_mouse_pointer->GetX(), gpC_mouse_pointer->GetY());
			
	if(IsInRect && m_IsMouseInWebBrowser == false)
	{
		m_IsMouseInWebBrowser = true;
		return 1;

	}
	else if(!IsInRect && m_IsMouseInWebBrowser == true)
	{
		m_IsMouseInWebBrowser = false;
		return -1;
	}

	return 0;
}

bool C_VS_UI_WEBBROWSER::IsInRectPoint(int X, int Y)
{
	RECT rc;
	if( 0 != GetWindowRect( m_hWnd_Explorer, &rc))
	{
		bool IsInRect = rc.left < X &&
						rc.right > X &&
						rc.top < Y &&
						rc.bottom > Y;
		return IsInRect;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_WEBBROWSER::Finish()
{
	//if (m_hWnd_Explorer)
	//{
	//	if(NULL != m_pWebBrowser)
	//	{
	//		m_pWebBrowser->Quit();
	//		m_pWebBrowser->Release();
	//		DeleteObject(m_pWebBrowser);
	//		m_pWebBrowser = NULL;
	//	}
		//m_hWnd_Explorer = NULL;
	//}



}



*/