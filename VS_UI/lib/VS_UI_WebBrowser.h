/*-----------------------------------------------------------------------------

	WEBBROWSER.h

	webbrowser interface.

	2005,1,31, sobeit

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_WEBBROWSER_H__
#define __VS_UI_WEBBROWSER_H__

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


#endif