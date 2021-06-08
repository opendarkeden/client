


#include "client_PCH.h"

#pragma warning(disable:4786)

#include "VS_UI_mouse_pointer.h"
#include "VS_UI_WebBrowser.h"
#include "Mshtml.h"

//extern IWebBrowser2*			g_pWebBrowser = NULL; 
//-----------------------------------------------------------------------------
// C_VS_UI_WEBBROWSER
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_WEBBROWSER::C_VS_UI_WEBBROWSER()
{
	m_pWebBrowser = NULL;
	m_IsMouseInWebBrowser = false;
}

//-----------------------------------------------------------------------------
// ~C_VS_UI_WEBBROWSER
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_WEBBROWSER::~C_VS_UI_WEBBROWSER()
{
	Finish();
	if(m_pWebBrowser)
	{
		m_pWebBrowser->Release();
	//	m_pWebBrowser = NULL;
	}
	//m_pWebBrowser = NULL;
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_WEBBROWSER::Start(HWND hwndApp,  char* pURL, void* pWebOjbect)
{
	HRESULT hr;

//	if(NULL == pURL || NULL == pWebOjbect)
//		return false;
	Finish();

	//m_pWebBrowser = (IWebBrowser2*)pWebOjbect;
	if (hr = FAILED(CoCreateInstance(CLSID_InternetExplorer,NULL,
			CLSCTX_LOCAL_SERVER, IID_IWebBrowser2,(LPVOID*)&m_pWebBrowser))) 
		{
			//MessageBox(NULL, "IWebBrowser2 Create failed", "ERROR", MB_OK);
			return false;
		}

	m_pWebBrowser->get_HWND((LONG*)&m_hWnd_Explorer);

	if(NULL == m_hWnd_Explorer)
		return false;

	DWORD dwStyle;
	dwStyle = GetWebBrowserStyle();
	dwStyle &= ~WS_SYSMENU;
	dwStyle &= ~WS_CAPTION;
	dwStyle |= WS_CHILD;

	SetWebBrowserStyle(dwStyle);
	
	
//	m_pWebBrowser->put_Width(100);                  // 가로 폭
//	m_pWebBrowser->put_Height(100);                //  세로 폭
	m_pWebBrowser->put_Left(0);
	m_pWebBrowser->put_Top(0);
	m_pWebBrowser->put_AddressBar(FALSE);  // 주소창 없앰
	m_pWebBrowser->put_MenuBar(FALSE);
	m_pWebBrowser->put_StatusBar(FALSE);
	m_pWebBrowser->put_ToolBar(FALSE);
	m_pWebBrowser->put_Silent(TRUE);
	m_pWebBrowser->put_Resizable(FALSE);
	//m_pWebBrowser->put_FullScreen(TRUE);

	
//_BLANK
//_PARENT
//_SELF
//_TOP

	CComBSTR url(pURL);

	CComVariant vtEmpty;
//
//    CComVariant vtTarget;
//    vtTarget.vt = VT_BSTR;
//    vtTarget.bstrVal = BSTR("_SELF");  //타겟 브라우져 이름 입력

	hr = m_pWebBrowser->Navigate(url, &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty);

//
//	LPDISPATCH lpDispatch;
//	IHTMLDocument2 *m_pHTMLDocument2; 
//	if( !FAILED (m_pWebBrowser->get_Document( &lpDispatch ) ) ) 
//	{
//		if (lpDispatch)
//		{
//			HRESULT hr = lpDispatch->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&m_pHTMLDocument2);
//			lpDispatch->Release();
//			if( SUCCEEDED( hr) )
//			{
//				m_pHTMLDocument2->put_onmousedown(FALSE);
//			}
//		}   
//	}
    


	m_pWebBrowser->put_Visible( TRUE );  // Works fine...

	//SetWebBrowserSize(0, 50, 800, 550);
	SetWebBrowserSize(0, 50, 600, 400);

	SetParent(m_hWnd_Explorer, hwndApp);

	SetFocus(hwndApp);
	
	return true;
}

//-----------------------------------------------------------------------------
// SetWebBrowserSize
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_WEBBROWSER::SetWebBrowserSize(int StartX, int StartY, int Width, int Height)
{
	if(		StartX < gpC_mouse_pointer->GetX() &&
			StartX + Width > gpC_mouse_pointer->GetX() &&
			StartY < gpC_mouse_pointer->GetY() &&
			StartY + Height > gpC_mouse_pointer->GetY())
		m_IsMouseInWebBrowser = true;

	MoveWindow(m_hWnd_Explorer, StartX, StartY, Width, Height, true);
}
//-----------------------------------------------------------------------------
// SetWebBrowserStyle
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_WEBBROWSER::SetWebBrowserStyle(DWORD dwstyle)
{
	SetWindowLong( m_hWnd_Explorer, GWL_STYLE, dwstyle ); 
}
//-----------------------------------------------------------------------------
// GetWebBrowserStyle
//
// 
//-----------------------------------------------------------------------------
DWORD C_VS_UI_WEBBROWSER::GetWebBrowserStyle()
{
	return GetWindowLong( m_hWnd_Explorer, GWL_STYLE );	
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

//-----------------------------------------------------------------------------
// Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_WEBBROWSER::Finish()
{
	if (m_hWnd_Explorer)
	{
		if(NULL != m_pWebBrowser)
		{
			m_pWebBrowser->Quit();
			m_pWebBrowser->Release();
			DeleteObject(m_pWebBrowser);
			m_pWebBrowser = NULL;
		}
		m_hWnd_Explorer = NULL;
	}



}



