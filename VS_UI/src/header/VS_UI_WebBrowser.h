/*-----------------------------------------------------------------------------

	WEBBROWSER.h

	webbrowser interface.

	2005,1,31, sobeit

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_WEBBROWSER_H__
#define __VS_UI_WEBBROWSER_H__

#ifdef PLATFORM_WINDOWS
//#include <ddraw.h>
#include "EXDISP.H"
#include "ATLBASE.H"

//-----------------------------------------------------------------------------
// MWEBBROWSER
//
//-----------------------------------------------------------------------------
class C_VS_UI_WEBBROWSER
{
public:
	
private:
	IWebBrowser2*			m_pWebBrowser;
	
	HWND				m_hWnd_Explorer;

	bool			m_IsMouseInWebBrowser;
public:
	C_VS_UI_WEBBROWSER();
	~C_VS_UI_WEBBROWSER();

	bool	Start(HWND hwndApp, char* pURL, void* pWebOjbect);
	void	Finish();

	void	SetWebBrowserSize(int StartX, int StartY, int Width, int Height);
	void	SetWebBrowserStyle(DWORD dwstyle);
	DWORD	GetWebBrowserStyle();
	int		GetMouseCursorInfo();
	bool	IsInRectPoint(int X, int Y);

//	IWebBrowser2* GetIWebBrowser2() {return m_pWebBrowser;}
};

#else
// WebBrowser not available on non-Windows platforms
class C_VS_UI_WEBBROWSER
{
public:
    C_VS_UI_WEBBROWSER() {}
    ~C_VS_UI_WEBBROWSER() {}

    // Stub implementation - web browser features not available on non-Windows platforms
    bool Start(void* hwndApp, char* pURL, void* pWebOjbect) { return false; }
    void Finish() {}
    void SetWebBrowserSize(int StartX, int StartY, int Width, int Height) {}
    void SetWebBrowserStyle(unsigned long dwstyle) {}
    unsigned long GetWebBrowserStyle() { return 0; }
    int GetMouseCursorInfo() { return 0; }
    bool IsInRectPoint(int X, int Y) { return false; }

private:
    bool m_IsMouseInWebBrowser;
};

#endif // PLATFORM_WINDOWS

#endif