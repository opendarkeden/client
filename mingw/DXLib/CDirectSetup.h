//-----------------------------------------------------------------------------
// CDirectSetup.h
//-----------------------------------------------------------------------------

#ifndef	__CDIRECTSETUP_H__
#define	__CDIRECTSETUP_H__

#include <Windows.h>


//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------

#define DSETUP_REINSTALL	0x00000080

#define SHOW_ALL		1
#define SHOW_UPGRADES	2
#define SHOW_PROBLEMS	3
#define SHOW_NONE		4

//-----------------------------------------------------------------------------
// DirectX Version
//-----------------------------------------------------------------------------
// Platform
#define	DXVER_PLATFORM_WINDOWS		VER_PLATFORM_WIN32_WINDOWS
#define	DXVER_PLATFORM_NT			VER_PLATFORM_WIN32_NT
        
// DirectX
#define	DXVER_DX_NO					0x000
#define	DXVER_DX_1					0x100
#define	DXVER_DX_2					0x200
#define	DXVER_DX_3					0x300
#define	DXVER_DX_5					0x500
#define	DXVER_DX_6					0x600
#define	DXVER_DX_6_1				0x601
#define	DXVER_DX_7					0x700



class CDirectSetup {
	public :
		CDirectSetup();
		~CDirectSetup();
		
		// DirectX를 설치한다.
		//static BOOL			DirectXInstall(HWND hWnd, HINSTANCE hInstance, const char* subDir, bool bInstall=true);

		// version을 얻어온다.
		static void			GetVersion( DWORD* pdwDXVersion, DWORD* pdwDXPlatform );		

		// 재부팅 한다면, 그 때 실행할 프로그램
		//static void			SetRestartProgram(const char* filename);

	protected :
		/*
		static int			GetReply(DWORD dwMsgType);
		static void			ShowButton(HWND hDlg, int Id, int strid);
		static void			SetButtons(HWND hDlg, DWORD wMsgType);
		static DLGPROC		DlgProc(HWND hDlg, WORD message, WPARAM wParam, LPARAM lParam);
		static DWORD WINAPI	DirectXSetupCallbackFunction(DWORD dwReason, DWORD dwMsgType, LPSTR szMessage, LPSTR szName, void *pInfo);
		static void			RegisterRestartProgram();


	protected :
		static DWORD			s_fStatus;
		static HWND				s_hDlg;          // window handle to dialog proc
		static HINSTANCE		s_hInstance;
		static char				s_szAppTitle[256];      // application title

		static int				s_wReply;          // global value for dialog return		

		static char				s_FilenameRunOnce[256];
		*/
};


#endif