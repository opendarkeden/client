//-----------------------------------------------------------------------------
// CWinMain.h
//-----------------------------------------------------------------------------
// window를 생성시키고 Loop를 수행해야 한다.
// 상속받아서 Update를 구현해야 한다.
//
// 상속받은 class에서 Init를 overloading하는 경우,
// 꼭(!) CWinMain::Init(...)를 호출해야 한다.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
//								[ Example ]
//
//-----------------------------------------------------------------------------
// 여기서는 CWinUpdate는 사용하지 않았다.
//-----------------------------------------------------------------------------
/*
//------------------------------------------------------------------------------
// AppTestApp.cpp
//------------------------------------------------------------------------------

#include "CWinMain.h"
//#include "CWinUpdate.h"

//------------------------------------------------------------------------------
// OnKeyDown - WM_KEYDOWN처리
//------------------------------------------------------------------------------
void
OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case VK_ESCAPE : case VK_RETURN :
			PostQuitMessage(0);
		break;
	}
}

//------------------------------------------------------------------------------
// WinMain
//------------------------------------------------------------------------------
int PASCAL
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
	CWinMain		TestApp;
	WINMAIN_INFO	wi;

	//-------------------------------------------------------------
	// WINMAIN_INFO 초기화
	//-------------------------------------------------------------
	wi.hInstance			= hInstance;
	strcpy(wi.ProgramName, "SiGi's Test Application");
	wi.SizeX				= 640;
	wi.SizeY				= 480;
	wi.Mode					= WINMAIN_WINDOW_STYPE_WINDOWMODE;
	
	//-------------------------------------------------------------
	// message 설정
	//-------------------------------------------------------------
	TestApp.SetWMFunction(WM_KEYDOWN, OnKeyDown);

	//-------------------------------------------------------------
	// Window 띄움
	//-------------------------------------------------------------
	TestApp.Init( &wi );
	TestApp.ShowWindow();

	//-------------------------------------------------------------
	// 처리 Loop
	//-------------------------------------------------------------
	TestApp.Loop();

	return 0;
}
*/
//-----------------------------------------------------------------------------

#ifndef	__CWINMAIN_H__
#define	__CWINMAIN_H__

#include <Windows.h>
#include "CWinMainInfo.h"
class CWinUpdate;


class CWinMain {
	public :
		typedef void	(*FUNCTION_MESSAGE)(WPARAM wParam, LPARAM lParam);


	public :
		CWinMain();
		virtual ~CWinMain();

		//-----------------------------------------------------------
		//
		//					Init / Release
		//
		//-----------------------------------------------------------
		virtual BOOL		Init(const WINMAIN_INFO* pInfo);
		virtual void		Release();

		//-----------------------------------------------------------
		// window를 보이게 한다.
		//-----------------------------------------------------------
		void				ShowWindow();


		//-----------------------------------------------------------
		//
		//					Set
		//
		//-----------------------------------------------------------
		// Update함수를 설정한다.
		//-----------------------------------------------------------
		void				SetUpdate(CWinUpdate* pWinUpdate)	{ m_pWinUpdate = pWinUpdate; }
		
		//-----------------------------------------------------------
		// Window Message를 처리할 함수를 설정한다.
		//-----------------------------------------------------------
		BOOL				SetWMFunction(UINT message, FUNCTION_MESSAGE function);		

		

		//-----------------------------------------------------------
		//
		//					Loop
		//
		//-----------------------------------------------------------
		void				Loop();	
		

		//-----------------------------------------------------------
		//
		//					Get
		//
		//-----------------------------------------------------------
		HWND				GetHWnd() const			{ return m_hWnd; }
		HINSTANCE			GetHInstance() const	{ return m_hInstance; }
		BOOL				IsActive() const		{ return m_bActiveApp; }



		//-----------------------------------------------------------
		// Error MessageBox를 띄우고 window종료한다.
		//-----------------------------------------------------------
		static void			Error(HWND hWnd, LPCSTR pProgramName, LPCTSTR szError,...);

	protected :
		virtual	void		Update();

		static long FAR PASCAL WMProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		//-----------------------------------------------------------
		// Default Window Message Functions
		//-----------------------------------------------------------
		static void			OnDestroy(WPARAM wParam, LPARAM lParam);


	protected :
		//-----------------------------------------------------------
		// app data
		//-----------------------------------------------------------
		HWND					m_hWnd;
		HINSTANCE				m_hInstance;

		BOOL					m_bActiveApp;		// Is application active?		

		char*					m_pProgramName;		// Program name

		CWinUpdate*				m_pWinUpdate;		// Update class			

	
		//-----------------------------------------------------------
		// message
		//-----------------------------------------------------------
		static FUNCTION_MESSAGE	m_WMFunctions[WM_USER];		// window message 처리		
};

#endif

