#include "Client_PCH.h"
#define __NPROTECT__
//#include "EXECryptor.h"
/* add by sonic 2006.9.14 start 藤속뚤WPE팁귁*/
#include "APICheck.h"
APICheck _APICheck;
/* ***************************************** */
//#undef __NPROTECT__
//#define NO_GAMEGUARD
//#define __NPROTECT_OLD_VERSION__



//-----------------------------------------------------------------------------
// Client.cpp
//-----------------------------------------------------------------------------

#pragma warning(disable:4786)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500 /* version 5.0 */
#endif /* !_WIN32_WINNT */

//#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#endif

#include "DebugInfo.h"
#ifndef _DEBUG
#include "CrashReport.h"
#endif

//add by zdj
#undef __NPROTECT__
#define NO_GAMEGUARD

#if defined(OUTPUT_DEBUG) || defined(_DEBUG) || defined(__DEBUG_OUTPUT__)
//#undef __NPROTECT__
//#define NO_GAMEGUARD
#endif

//
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <io.h>
#include <direct.h>
#include <time.h>
#include <crtdbg.h>
#include <string>
#include <process.h>
#include <sys/stat.h>
#include "Client.h"
#include "GameObject.h"
#include "VS_UI.h"
#include "MMusic.h"
#include "UIDialog.h"
#include "UpdateManager.h"
#include "MZLib.h"
#include <Commctrl.h>
#include "AppendPatchInfo.h"
#include "EffectSpriteTypeDef.h"
//#include "MFileDef.h"
#include "Properties.h"
#include "ServerInfo.h"
#include "MTestDef.h"
#include "Resource.h"
#include "UserInformation.h"
#include "MFileDef.h"
#include "SoundSetting.h"

#ifdef OUTPUT_DEBUG
#include "Packet/Gpackets/GCSystemMessage.h"
#endif

#ifdef __NPROTECT__
#include "nProtect.h"
#endif

//yckou
#include "DebugKit.h"

//#define	new			DEBUG_NEW
//#define	UPDATER_FILENAME		"Updater.exe"
#define DIRECTORY_UPDATE		"Update"

#ifndef OUTPUT_DEBUG
	#define	USE_UPDATER
#endif

#if defined(OUTPUT_DEBUG) && defined(__METROTECH_TEST__)
//	#define	__WM_OUTPUT_DEBUG__
#endif

#ifdef OUTPUT_DEBUG
	extern bool					g_bSlideScreenShot;
	extern CRITICAL_SECTION		g_Lock;
#endif

#ifdef OUTPUT_DEBUG
//	int CLogFile = -1;
#endif

#ifdef DEBUG_INFO
BOOL g_bMsgOutPutFlag=FALSE;
BOOL g_bMsgDetailFlag=FALSE;
BOOL g_bMsgContentFlag=FALSE;
BYTE g_nKeyMapSelect = 0;
int  g_nGameVersion = 0;
CMessageStringTable g_MessageStringTable;
#endif
//-----------------------------------------------------------------------------
// Global
//-----------------------------------------------------------------------------

#ifdef __NPROTECT__
NPROTECT_STATUS g_nProtectSTATUS = NPROTECT_NORMAL;
#endif
HWND				g_hWnd;
HINSTANCE			g_hInstance;
int					g_x=400;
int					g_y=300;
//bool				g_bHALAvailable = false;
DWORD				g_dwVideoMemory = 0;

char				g_CWD[_MAX_PATH];

int					g_Dimension = 0;
DWORD				g_TimerNPMON = 0;

// FPS
DWORD				g_CurrentTime		= 0;		// 시간
DWORD				g_CurrentFrame		= 0;		// frame수

int					g_FrameCount		= 0;
int					g_StartFrameCount	= 0;
DWORD				g_StartTime			= timeGetTime();
DWORD				g_EndTime			= g_StartTime;
int					g_FrameRate			= 0;
bool				g_bGoodFPS			= true;
const int			g_FrameGood			= 15;

// Execute Program --> bActiveApp
// minimize | anotherWnd click--> !ActiveGame
BOOL				g_bActiveApp			= FALSE; // Is application active?
BOOL				g_bActiveGame			= FALSE; // Is Game Active?
BOOL				g_bNeedUpdate			= FALSE; // update해야되나?

DWORD				g_double_click_time = 0;

bool				g_bTestMode = false;
//BOOL				g_WindowsNT = FALSE;
bool				g_bForceExitBynProtect = false;
bool				g_bForceExitBynProtect2 = false;
DWORD				g_nProtectErrorMessage = 0;
DWORD				g_nProtectErrorMessage2 = 0;
int					g_LeftPremiumDays = 0;
bool				g_bFamily = false;
bool				g_bSuperUser			= false;

DWORD g_dwSeqNumL = 0;
DWORD g_dwSeqNumG = 0;
//WORD g_wAuthKeyMap = 0x28AA;
//WORD g_wAuthKeyMap = 0x1455;
//edit by sonic 2006.4.12
//WORD g_wAuthKeyMap = 0xFFFFFFFF;
//end
WORD g_wAuthKeyMap = 0x5154;

//void SizeOfObjects();
//#define				__WEB_BROWSER__
IWebBrowser2*			g_pWebBrowser = NULL; 
// [Futec수정]
char g_FutecIP[20] = { 0, };
unsigned int g_FutecPort = 0;
BYTE g_AdvanceVampireActionMaxCount[ ACTION_ADVANCEMENT_MAX - ACTION_ADVANCEMENT_STOP ];
BYTE g_AdvanceSlayerActionMaxCount[ ACTION_ADVANCEMENT_SLAYER_MAX - ACTION_ADVANCEMENT_SLAYER_STOP_SWORD ];
BYTE g_AdvanceOustersActionMaxCount[ ACTION_ADVANCEMENT_OUSTERS_MAX - ACTION_ADVANCEMENT_OUSTERS_CHAKRAM_STOP ];

enum DARKEDEN_LANGUAGE
{
	DARKEDEN_KOREAN = 0,
	DARKEDEN_CHINESE,
	DARKEDEN_JAPANESE,
	DARKEDEN_ENGLISH,
	DARKEDEN_TAIWAN,
	
	DARKEDEN_LANGUAGE_MAX
};

struct NETMARBLE_INFO
{
	MString ID;
	MString Password;
	int WorldID;
	int ServerID;
	bool bGore;
};

// 2004, 7, 14, sobeit add start
struct REALSERVER_INFO
{
	REALSERVER_INFO() { bMode = false;WorldID = 0;}
	bool bMode;		// 0: 수동접속, 1:정상접속
	int WorldID;	// 0: 브란, 1: 펠레스
	MString ID;		// id
	MString Key;	// Key
};
// 2004, 7, 14, sobeit add end

BYTE g_macAddress[6];
extern void CheckMacScreenMode();
extern BOOL GetMacAddressFromSock();
extern BOOL InitDebugInfo();
// add by Sonic 2006.9.26 쇱꿎1024 * 768경굶홍애긴좆
BOOL g_MyFull=TRUE;
RECT g_GameRect={799,599,800,600};
LONG	g_SECTOR_WIDTH           =16 ;
LONG	g_SECTOR_HEIGHT          =25 ;
LONG	g_SECTOR_WIDTH_HALF      =9 ;
LONG	g_SECTOR_HEIGHT_HALF     =13 ;
LONG	g_SECTOR_SKIP_PLAYER_LEFT=-8 ;
LONG	g_SECTOR_SKIP_PLAYER_UP  =-12 ;

LONG	g_TILESURFACE_SECTOR_WIDTH         =22;
LONG	g_TILESURFACE_SECTOR_HEIGHT        =31;
LONG	g_TILESURFACE_SECTOR_OUTLINE_RIGHT =19;
LONG	g_TILESURFACE_SECTOR_OUTLINE_DOWN  =28;
LONG	g_TILESURFACE_WIDTH                =1056;
LONG	g_TILESURFACE_HEIGHT               =744;
LONG	g_TILESURFACE_OUTLINE_RIGHT        =912;
LONG	g_TILESURFACE_OUTLINE_DOWN         =672;
LONG		g_TILE_X_HALF=24;
LONG		g_TILE_Y_HALF=12;
// end
//-----------------------------------------------------------------------------
// define function
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// addFile을 읽어서 originalFile의 끝에 붙인다.
//-----------------------------------------------------------------------------			
#define WRITE_FROM_FILE(originalFile, patchFile)		\
		{												\
			char buffer[4096];							\
			int n;										\
														\
			while (1)									\
			{											\
				patchFile.read(buffer, 4096);			\
				n = patchFile.gcount();					\
														\
				if (n > 0)								\
				{										\
					originalFile.write(buffer, n);		\
				}										\
				else									\
				{										\
					break;								\
				}										\
			}											\
		}
//踞샙俚륜눔
void get_rand_str(char s[],int number);
void get_rand_str(char s[],int number)
{
        char str[64] = "00123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; 
        int i;
        char ss[2];
        //printf("%c %c\n",str[1],str[62]);
        srand((unsigned int)time((time_t *)NULL));
        for(i=1;i<=number;i++){
                sprintf(ss,"%c",str[(rand()%62)+1]);
                //printf(ss);
                strcat(s,ss);
        }
}
//-----------------------------------------------------------------------
// Get Futec Address [Futec수정]
//-----------------------------------------------------------------------
// DarkEden.exe Futec(IP:Port)
//              01234567890123
//-----------------------------------------------------------------------
bool
GetFutecAddress(const char* pStr)
{
	char * pFutecStr = strstr(pStr, "Futec");
	char * pLeftParen = strchr(pStr, '(');
	char * pSeperator = strchr(pStr, ':');
	char * pRightParen = strchr(pStr, ')');

	if (pFutecStr==NULL
		|| pLeftParen==NULL
		|| pSeperator==NULL
		|| pRightParen==NULL
		|| !(pFutecStr < pLeftParen 
			&& pLeftParen < pSeperator
			&& pSeperator < pRightParen))
		return false;

	char str[20];

	strncpy( g_FutecIP, pLeftParen+1, pSeperator-(pLeftParen+1) );
	strncpy( str, pSeperator+1, pRightParen-(pSeperator+1) );
	g_FutecPort = atoi(str);

	return true;
}

bool ParsingNetmarble(const char* pCommandLine, NETMARBLE_INFO &info);
//bool ParsingRealServer(const char* pCommandLine,  int Dimention, REALSERVER_INFO &info);

bool
ParsingRealServer(const char* pCommandLine, int Dimention, REALSERVER_INFO &info)
{
	if( pCommandLine == NULL )
		return false;
	
	char szTemp[4096];
	if(strlen(pCommandLine) < 16)
		return false;

	const char *pString = strchr(pCommandLine, ' ');
	if(pString == NULL)
	{
		return false;
	}
	
	strcpy(szTemp, pString+1);
	
	int argcnt = 0;
	char* token = NULL;
	char arg2[4][32];
	argcnt = 0;

	token = strtok(szTemp, "|");
	if(NULL == token)
		return false;
	while(token && argcnt < 4)
	{
		strcpy(arg2[argcnt], token);
		argcnt++;
		token = strtok(NULL, "|");
	//	if(NULL == token)
	//		return false;
	}
	// mode|demention|ID|key
	info.bMode = atoi(arg2[0])?1:0;
	info.WorldID = atoi(arg2[1]);
	info.ID = arg2[2];
	info.Key = arg2[3];
	
	return true;
} 
//-----------------------------------------------------------------------------
// Name: InitFail()
// Desc: This function is called if an initialization function fails
//-----------------------------------------------------------------------------
HRESULT InitFail(LPCTSTR szError,...)
{
	// 프로그램 중단..
	g_bActiveApp = FALSE;

	ShowCursor( TRUE );
	ShowWindow(g_hWnd, SW_HIDE);

    char		szBuff[1024];
    va_list		vl;

    va_start(vl, szError);
    vsprintf(szBuff, szError, vl);

    //ReleaseAllObjects();
	DEBUG_ADD( szBuff );
//add by sonic 2006.4.11
    //MessageBox(g_hWnd, szBuff, PROGRAM_TITLE, MB_OK);
    DestroyWindow(g_hWnd);
    va_end(vl);
//end 

    return 0;
}

//-----------------------------------------------------------------------------
// ProgressBar
//-----------------------------------------------------------------------------
bool g_bUseProgressBar = true;
HWND g_hWndProgress = NULL;
const int progressBarWidth = 300;
const int progressBarHeight = 40;
const int g_numAppendFiles = 16;		// AppendPatch.inf의 file들 개수

std::map<DWORD,std::string> g_nProtectMessage;

void
RemoveProgressBar()
{
	if (g_hWndProgress!=NULL)
	{
		DestroyWindow(g_hWndProgress); 
		g_hWndProgress = NULL;
	}
}

DARKEDEN_LANGUAGE CheckDarkEdenLanguage()
{
	if( g_pFileDef == NULL )
		return DARKEDEN_KOREAN;

	int num = 0;
	const char* fileName = g_pFileDef->getProperty("FILE_LANGUAGE_INFO").c_str();
	FILE *f = fopen(fileName, "r");
	if (f == NULL) goto exit;

	char szLine[512];

	while( fgets( szLine, 512, f) != NULL)
	{
		if( szLine[0] == ';' || strlen( szLine ) <= 0 )
			continue;

		if( strncmp( szLine, "LANGUAGE",8 ) == 0 )
		{
			sscanf(szLine+8,"%d",&num);
			break;
		}
	}

	fclose(f);

exit:
	return (DARKEDEN_LANGUAGE)(DARKEDEN_KOREAN + num );
}

void		PrecalculateAdvancementClassCreatureFrames()
{
	memset( g_AdvanceVampireActionMaxCount, 0, sizeof( g_AdvanceVampireActionMaxCount ) );
	memset( g_AdvanceSlayerActionMaxCount, 0, sizeof( g_AdvanceSlayerActionMaxCount ) );
	memset( g_AdvanceOustersActionMaxCount, 0, sizeof( g_AdvanceOustersActionMaxCount ) );

	int i;

	
	for( i = ACTION_ADVANCEMENT_SLAYER_STOP_SWORD; i < ACTION_ADVANCEMENT_SLAYER_MAX; ++i )
	{
		int actionindex = i - ADVANCEMENT_ACTION_START;
		
		char part = 0;

		if( i == ACTION_ADVANCEMENT_SLAYER_BIKE_MOVE )
		{
			g_AdvanceSlayerActionMaxCount[ actionindex ] = 4;
			continue;
		}
			// ||
			//i == ACTION_ADVANCEMENT_SLAYER_BIKE_STOP )
			//part = AC_BIKE_1;

//		try{
		g_AdvanceSlayerActionMaxCount[ actionindex ] = 
			g_pTopView->m_AdvancementSlayerManFPK[ part ][ actionindex ][2].GetSize();
//		}catch(...){} //add by viva
	}
	

	for( i = ACTION_ADVANCEMENT_STOP; i < ACTION_ADVANCEMENT_MAX; ++i )
	{
		int actionindex = i - ADVANCEMENT_ACTION_START;
//		try{
		g_AdvanceVampireActionMaxCount[ actionindex ] = 
			g_pTopView->m_AdvancementVampireManFPK[0][ actionindex ][2].GetSize();
//		}catch(...){} //add by viva
	}

	for( i = ACTION_ADVANCEMENT_OUSTERS_CHAKRAM_STOP; i < ACTION_ADVANCEMENT_OUSTERS_MAX; ++i )
	{
		int actionindex = i - ADVANCEMENT_ACTION_START;
//		try{
		g_AdvanceOustersActionMaxCount[ actionindex ] = 
			g_pTopView->m_AdvancementOustersFPK[1][ actionindex ][2].GetSize();
//		}catch(...){} //add by viva
	}
}

int			
GetPersnalShopUpdateTime()
{
	CRarFile rarfile;

	rarfile.SetRAR(g_pFileDef->getProperty("FILE_INFO_DATA").c_str(),"darkeden");
	rarfile.Open(g_pFileDef->getProperty("FILE_UPDATE_PERSNALSHOPTIME").c_str() );

	if( !rarfile.IsSet() )
		return 60000;

	char szLine[512];
	int num = 0;

	while( rarfile.GetString( szLine, 512 ) )
	{
		if( strncmp( szLine, "UPDATETIME",10 ) == 0 )
		{
			sscanf(szLine+10,"%d",&num);
			break;
		}
	}

	rarfile.Release();

	return num;
}
void
CreateProgressBar(const char* str)
{
	if (g_bUseProgressBar)
	{
		RemoveProgressBar();

		InitCommonControls();

		int cyVScroll = GetSystemMetrics(SM_CYVSCROLL); 

		g_hWndProgress = CreateWindowEx(WS_EX_TOPMOST, PROGRESS_CLASS, str, 
										WS_VISIBLE | PBS_SMOOTH, 
										GetSystemMetrics(SM_CXSCREEN)/2 - progressBarWidth/2, 
										GetSystemMetrics(SM_CYSCREEN)/2 - progressBarHeight/2, 
										progressBarWidth, progressBarHeight,
										NULL, (HMENU)0, g_hInstance, NULL); 
	}
}


void
SetProgressBarCount(int count)
{	
	if (!g_bUseProgressBar || g_hWndProgress==NULL)
	{
		return;
	}

	SendMessage(g_hWndProgress, PBM_SETRANGE, 0, MAKELPARAM(0, count)); 
    SendMessage(g_hWndProgress, PBM_SETSTEP, (WPARAM) 1, 0); 
	SendMessage(g_hWndProgress, PBM_SETPOS, (WPARAM) 0, 0); 	
}

void
SetProgressBarText(const char* str)
{
	if (g_bUseProgressBar)
	{
		if (g_hWndProgress==NULL)
		{
			CreateProgressBar(str);
			SetProgressBarCount( g_numAppendFiles );
		}
		else
		{
			SetWindowText( g_hWndProgress, str	);
		}
	}
}



void
UpdateProgressBar()
{
	if (!g_bUseProgressBar || g_hWndProgress==NULL)
	{
		return;
	}

	SendMessage(g_hWndProgress, PBM_STEPIT, 0, 0); 
}


//-----------------------------------------------------------------------------
// Patch Log Window
//-----------------------------------------------------------------------------
HWND		g_hPatchLogWnd = NULL;
HWND		g_hPatchLogEdit = NULL;
char*		g_pPatchLogBuffer = NULL;	// 얼마나 클지 몰라서리 global에 뒀다.

long FAR PASCAL 
PatchLogWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
    {
		case WM_DESTROY:
            PostQuitMessage(0);
        return 0L;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool
ReadPatchLogFromFile()
{
	char strBuffer[256];

	ifstream file("PatchLog.txt", ios::nocreate);	// text file이다.

	if (!file.is_open())
	{
		return false;
	}

	file.seekg( 0, ios::end );
	long fpEnd = file.tellg();

	if (g_pPatchLogBuffer!=NULL)
	{
		delete [] g_pPatchLogBuffer;
	}
	g_pPatchLogBuffer = new char [fpEnd*2];

	file.seekg( 0 );

	char* pBuffer = g_pPatchLogBuffer;

	while (!file.eof())
	{
		file.getline(strBuffer, 254);
		int n = file.gcount();		
		
		if (n==0)
		{
			break;
		}

		if (strBuffer[n-1]=='\0')
		{
			n -= 1;
		}

		// \r\n을 붙여줘야 다음줄이 된다.		
		strBuffer[n] = '\r';
		strBuffer[n+1] = '\n';
		
		strncpy(pBuffer, strBuffer, n+2);
		pBuffer += n+2;
	}
	*pBuffer = '\0';

	file.close();

	SetWindowText( g_hPatchLogEdit, g_pPatchLogBuffer );

	delete [] g_pPatchLogBuffer;
	g_pPatchLogBuffer = NULL;

	return true;
}

void OutPutClassSize()
{
//#ifdef __DEBUG_OUTPUT__
//	SizeOfObjects();
//#endif
}

void
ShowPatchLogWindow()
{
/*	int width = 600;
	int height = 350;

	WNDCLASS               wc;

	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= (WNDPROC)PatchLogWindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= g_hInstance;
	wc.hIcon			= LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_DARKEDEN));
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH )GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "PatchLog";
	RegisterClass(&wc);

	DWORD style = WS_OVERLAPPEDWINDOW;
	style &= ~WS_MINIMIZEBOX;
	style &= ~WS_MAXIMIZEBOX;
	style &= ~WS_THICKFRAME;
	g_hPatchLogWnd = CreateWindow("PatchLog", "다크에덴 패치내용", 
							style,
							GetSystemMetrics(SM_CXSCREEN)/2 - width/2, 
							GetSystemMetrics(SM_CYSCREEN)/2 - height/2, 
							width,	height, 
							NULL, NULL, g_hInstance, NULL);   
   
	if (!g_hPatchLogWnd)
	{
		return;
	}

	RECT rect;
	GetClientRect(g_hPatchLogWnd, &rect);
	g_hPatchLogEdit = CreateWindow(TEXT("edit"), NULL,
						   WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER | ES_LEFT | ES_MULTILINE  | 

ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_READONLY,
						   0,0, 
						   rect.right-rect.left, rect.bottom-rect.top,
						   g_hPatchLogWnd, NULL, g_hInstance, NULL);

	if (!ReadPatchLogFromFile())
	{
		DestroyWindow( g_hPatchLogWnd );		
		
		// 바로 return하면 main window가 죽는다. - -;		
	}
	else
	{
		ShowWindow(g_hPatchLogWnd, SW_SHOW);
		UpdateWindow(g_hPatchLogWnd);  
		SetFocus(g_hPatchLogWnd);
	}
	
	MSG		msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{	
			if (!GetMessage(&msg, NULL, 0, 0))
				break;
				//return msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			WaitMessage();
		}
	}
*/
}

//-----------------------------------------------------------------------------
// Check DX Version
//-----------------------------------------------------------------------------
// DirectX Version을 check한다.
//-----------------------------------------------------------------------------
/*
bool
CheckDXVersion()
{
	DWORD dwVer, dwPlatform;
	CDirectSetup::GetVersion( &dwVer, &dwPlatform );

	//------------------------------------------------------
	// Platform check
	//------------------------------------------------------
	//switch( dwPlatform )
    //{
      //  case DXVER_PLATFORM_WINDOWS:
            //strcpy( strBuff, "OS:\tWindows 9x\n" );
        //    break;
        //case DXVER_PLATFORM_NT:
            //strcpy( strBuff, "OS:\tWindows NT\n" );
          //  break;
        //default:
            //strcpy( strBuff, "Error!\n" );
          //  break;
    //}


	//------------------------------------------------------
	// DirectX Version check
	//------------------------------------------------------
	// DX 7.0 이상
    if (dwVer==DXVER_DX_7)
	{			
		return true;
	}
	
	//InitFail("You need to install DirectX 7.0 or later version...");

	// 안 깔렸으면 아예 깔아버리자..
	char directory[_MAX_PATH];

	// 현재 디렉토리를 얻어서 
	GetCurrentDirectory( _MAX_PATH, directory );

	// 실행화일 이름을 붙인다.
	sprintf(directory, "%s\\%s", directory, PROGRAM_FILENAME);

	CDirectSetup::SetRestartProgram(directory);
	CDirectSetup::DirectXInstall(g_hWnd, g_hInstance, "DirectX7", true);
	
	// 새로 깔고 나서 바로 실행해도 되는걸까?
	// rebooting 해야하지 않을까??

	return true;    
	//return false;    
}
*/
 
//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The Main Window Procedure
//-----------------------------------------------------------------------------
long FAR PASCAL 
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//MINMAXINFO      *pMinMax;

    switch (message)
    {
#ifdef __NPROTECT__
#ifdef __NPROTECT_OLD_VERSION__
	case WM_NPROTECT_SOFTICE :
		DEBUG_ADD("[nProtect] GameClose By nProtect (Window Message : WM_NPROTECT_SOFTICE)");
//		SetMode( MODE_QUIT );
		g_nProtectSTATUS = NPROTECT_SOFTICE;
		break;

	case WM_NPROTECT_EXIT_TWO :
		DEBUG_ADD("[nProtect] GameClose By nProtect (Window Message : WM_NPROTECT_EXIT_TWO)");
//		SetMode( MODE_QUIT );
		g_nProtectSTATUS = NPROTECT_EXIT_TWO;
		break;
	case WM_NPROTECT_FORCEEXIT :
		DEBUG_ADD("[nProtect] GameClose By nProtect (Window Message : WM_NPROTECT_FORCEEXIT)");		
//		SetMode( MODE_QUIT );
		g_nProtectSTATUS = NPROTECT_FORCE_EXIT;
		break;
	case WM_NPROTECT_SH_ERROR :
		DEBUG_ADD("[nProtect] GameClose By nProtect (Window Message : WM_NPROTECT_SH_ERROR)");
		g_nProtectSTATUS = NPROTECT_SH_ERROR;
		break;
	case WM_NPROTECT_SPEEDHACK :
		g_nProtectSTATUS = NPROTECT_SPEEDHACK;
		break;
#endif
#endif

		//---------------------------------------------------------------
		//
		//						MM_MCINOTIFY
		//
		//---------------------------------------------------------------
		case MM_MCINOTIFY :
		{
			// Mid가 다 연주되었다는 말이다.
			if (wParam==MCI_NOTIFY_SUCCESSFUL)
			{
				// Opening 동영상이 끝난 경우
				if (g_pAvi!=NULL && (DWORD)lParam==g_pAvi->dwID) 
				{ 
					g_pAvi->bEndFlag=true; 
					g_pAvi->Close(); 

					// Login 화면으로...
					SetMode( MODE_MAINMENU );
				}				
				// 음악 연주가 끝난 경우
				else
				{
					/*
					// 반복 연주 안한다.
					if (g_pUserOption->PlayMusic)//g_Music.IsPause())	// 과연 필요할까.. - -;
					{
						if (g_pUserOption->PlayWaveMusic)
						{
							g_pDXSoundStream->Play( FALSE );
						}
						else
						{
							g_Music.RePlay();						
						}
					}
					*/
					//else
					//{
						// 반복해서 연주한다.
					//	g_Music.RePlay();
					//}
//					if(g_pMP3->IsLoop())
//						g_pMP3->Play(true);
				}
			}
		}
		return 0L;

		//---------------------------------------------------------------
		//
		//	WM_SYSCOMMAND
		// 
		//---------------------------------------------------------------
		case WM_SYSCOMMAND:
            // Prevent moving/sizing and power loss in fullscreen mode
            switch( wParam )
            {
				case SC_HOTKEY:
				case SC_KEYMENU:
				case SC_TASKLIST:
#if !defined(OUTPUT_DEBUG) && !defined(__DEBUG_OUTPUT__)
				case SC_PREVWINDOW:
				case SC_NEXTWINDOW:
#endif
					return 1;

				case SC_CLOSE:
				case SC_MOVE:
                case SC_SIZE:
                case SC_MAXIMIZE:
                case SC_MONITORPOWER:
				case SC_SCREENSAVE :
                    if( CDirectDraw::IsFullscreen() )
                        return 1;
                    break;
            }
        break;//return 0L;


		//---------------------------------------------------------------
		//
		//							WM_ACTIVATE
		//
		//---------------------------------------------------------------
		/*
		case WM_ACTIVATE :		
		{
			#ifdef __WM_OUTPUT_DEBUG__
					DEBUG_ADD("[WM_ACTIVATE]");
			#endif

					/*
			//WORD fActive = LOWORD(wParam);           // activation flag 
			//BOOL fMinimized = (BOOL) HIWORD(wParam); // minimized flag 

			// active됐고 minimized가 아닌 상태가 ActiveGame이당..
			//BOOL bActive = (fActive==WA_ACTIVE) || (fActive==WA_CLICKACTIVE);
			//				//&& !fMinimized;

			//g_double_click_time = GetDoubleClickTime();

			//if (g_bActiveApp)
			{
			//	CheckActivate(bActive);
			
			//	g_bActiveGame = bActive;			
			}			
		
			BOOL bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);        
			
			if (g_pDXInput!=NULL)
			{
				// acquire
				g_pDXInput->SetAcquire(bActive);
				
				// 입력을 초기화한다.
				g_pDXInput->Clear();
			}
		}
		break;//return 0L;
		*/

		//---------------------------------------------------------------
		//
		//							WM_ACTIVATEAPP
		//
		//---------------------------------------------------------------
		///*
//		case WM_ACTIVATE :
		case WM_ACTIVATEAPP :
		{
			#ifdef __WM_OUTPUT_DEBUG__
					DEBUG_ADD("[WM_ACTIVATEAPP]");
			#endif

            // Pause if minimized or not the top window
			//WORD fActive = LOWORD(wParam);
			//WORD fMinimized = (BOOL) HIWORD(wParam); 
            //g_bActiveGame = (fActive == WA_ACTIVE) || (fActive == WA_CLICKACTIVE);

			g_double_click_time = GetDoubleClickTime();

			BOOL bActiveGame = (BOOL)wParam;

			//if (g_bActiveApp)
			{
				CheckActivate(bActiveGame);

				// 
				g_bActiveGame = bActiveGame;
			}
		}
        break;//return 0L;
		//*/

		/*
		case WM_SIZE :
		{
			BOOL bActive;

			// Check to see if we are losing our window...
            if( SIZE_MAXHIDE==wParam || SIZE_MINIMIZED==wParam )
                bActive = FALSE;
            else
                bActive = TRUE;

            g_double_click_time = GetDoubleClickTime();

			if (g_bActiveApp)
			{
				CheckActivate(bActive);
			
				g_bActiveGame = bActive;			
			}
		}
		break;
		*/
		
		//---------------------------------------------------------------
		//
		//							WM_SIZE
		//
		//---------------------------------------------------------------
		//case WM_SIZE:
            // Check to see if we are losing our window...
            //if (SIZE_MAXHIDE==wParam || SIZE_MINIMIZED==wParam)
              //  g_bActiveGame = FALSE;
            //else
              //  g_bActiveGame = TRUE;
        //return 0L;
		
		//---------------------------------------------------------------
		//
		//							WM_DESTROY
		//
		//---------------------------------------------------------------
        case WM_DESTROY:
            // Clean up and close the app            
            PostQuitMessage(0);
        return 0L;

		case WM_GETMINMAXINFO:
		{
            // Fix the size of the window to 640x480 (client size)
            MINMAXINFO* pMinMax = (MINMAXINFO *)lParam;
			// add by Sonic 2006.9.26
			//if(g_MyFull)
			//{
			//	pMinMax->ptMinTrackSize.x = 1024+GetSystemMetrics(SM_CXSIZEFRAME)*2;
			//	pMinMax->ptMinTrackSize.y = 768+GetSystemMetrics(SM_CYSIZEFRAME)*2
            //                               +GetSystemMetrics(SM_CYMENU);
			//}
			//else
			//{
			
				pMinMax->ptMinTrackSize.x = g_GameRect.right+GetSystemMetrics(SM_CXSIZEFRAME)*2;
				pMinMax->ptMinTrackSize.y = g_GameRect.bottom+GetSystemMetrics(SM_CYSIZEFRAME)*2
											   +GetSystemMetrics(SM_CYMENU);
			//}
			// end
            pMinMax->ptMaxTrackSize.x = pMinMax->ptMinTrackSize.x;
            pMinMax->ptMaxTrackSize.y = pMinMax->ptMinTrackSize.y;
		}
        break;
		
		/*
		case WM_PAINT:
			if (g_bActiveApp)
				CDirectDraw::OnPaint();
			return 0L;
		*/
	
		//---------------------------------------------------------------
		//
		//							WM_MOVE
		//
		//---------------------------------------------------------------
		case WM_MOVE :			
			if (g_bActiveApp && !CDirectDraw::IsFullscreen())
				CDirectDraw::OnMove();
		break;

		/*
		case WM_MOUSEMOVE :		
			//fwKeys = wParam;        // key flags 
			//g_x = LOWORD(lParam);  // horizontal position of cursor 
			//g_y = HIWORD(lParam);  // vertical position of cursor 
			
			return 0L;
		*/

		//---------------------------------------------------------------
		//
		//							WM_KEYDOWN
		//
		//---------------------------------------------------------------
        case WM_KEYDOWN:
			#ifdef __WM_OUTPUT_DEBUG__
				DEBUG_ADD("[WM_KEYDOWN]");
			#endif

			if (g_Mode==MODE_OPENING)
			{
				// Opening을 끝낸다.
				if (wParam==VK_ESCAPE || wParam==VK_RETURN || wParam==VK_SPACE)
				{	
					if (g_pAvi!=NULL)
					{
						g_pAvi->Close();
					}
					SetMode( MODE_MAINMENU );					
				}
				return 0L;
			}

			gC_vs_ui.KeyboardControl(message, wParam, lParam);
            // Handle any non-accelerated key commands

			switch (wParam)
			{
				//-----------------------------------------------
				// Screen Shot
				//-----------------------------------------------
				case VK_SCROLL :	
					#ifdef OUTPUT_DEBUG
						if (!g_bSlideScreenShot)
						{
							MakeScreenShot();
						}
					#else
						MakeScreenShot();
					#endif
					
				return 0L;
				//case VK_F11 : 
				//		g_UpdateDelay = (g_UpdateDelay==1)? DELAY_UPDATE_GAME : 1;
				//		return 0L;

				//-----------------------------------------------
				// Toggle Music
				//-----------------------------------------------
				/*
				case VK_F3 :
					//if (g_Music.IsPlay())
					if (g_DXMusic.IsPlay())
					{
						//g_Music.Pause();
						g_DXMusic.Pause();
					}
					else
					{						
						//g_Music.Resume();
						g_DXMusic.Resume();
					}										
				return 0L;
				*/

				//-----------------------------------------------
				// Volume 조절
				//-----------------------------------------------
				/*
				case VK_F5 :
					{
						g_pMasterVolume->SetCurrentVolume(
								g_pMasterVolume->GetCurrentVolume() - 0x1000
							);
					}
					//if (g_pPlayer->IsExistAddon(MCreatureWear::ADDON_COAT))
					//	g_pPlayer->SetAddonNULL(MCreatureWear::ADDON_COAT); 
					//else g_pPlayer->SetAddon(MCreatureWear::ADDON_COAT, 0);						

				
				return 0L;
	
				case VK_F6 :		
					{
						//g_DXMusic.AddVolume( 0xF000 );
						g_pMasterVolume->SetCurrentVolume(
								g_pMasterVolume->GetCurrentVolume() + 0x1000
							);
					}
					//if (g_pPlayer->IsExistAddon(MCreatureWear::ADDON_TROUSER))
					//	g_pPlayer->SetAddonNULL(MCreatureWear::ADDON_TROUSER); 
					//else g_pPlayer->SetAddon(MCreatureWear::ADDON_TROUSER, 1);
				return 0L;
				*/

				//-----------------------------------------------
				//
				//				Debug Mode 용
				//
				//-----------------------------------------------
				#if defined(OUTPUT_DEBUG)
				
					case VK_PAUSE :
						g_bActiveApp = !g_bActiveApp;					
					return 0L;

					//-----------------------------------------------
					// Toggle ActiveMode
					//-----------------------------------------------
					//case VK_F1 :
					//	g_bActiveApp = !g_bActiveApp;
					//	return 0L;	

					//-----------------------------------------------
					// 움직이는 방법 설정
					//-----------------------------------------------
					/*
					case VK_F2 :
					{					
						if (g_pPlayer->IsStop())
						{
							// 움직이는 형태 바꾸기
							//if (g_pPlayer->GetMoveDevice()==MCreature::MOVE_DEVICE_NULL)
							//{
							//	g_pPlayer->SetMoveDevice( MCreature::MOVE_DEVICE_MOTOR1 );
							//}/
							//else
							//{
								//g_pPlayer->SetMoveDevice( MCreature::MOVE_DEVICE_NULL );
							//}
						
							g_pPlayer->SetAction( ACTION_STAND );

							int type = g_pPlayer->GetCreatureType();

							type++;

							if (type==g_CreatureTable.GetSize())
							{							
								g_pPlayer->SetCreatureType( 0 );
							}
							else
							{
								g_pPlayer->SetCreatureType( type );
							}

							// 남자 Slayer
							if (g_pPlayer->GetCreatureType()==0)
							{
								g_pPlayer->RemoveAddon( ADDON_COAT );	
								g_pPlayer->RemoveAddon( ADDON_TROUSER );

								MCoat		coat1;	
								MTrouser	trouser1;	

								coat1.SetItemType( 0 ); coat1.SetItemOption( 0 );
								trouser1.SetItemType( 0 ); trouser1.SetItemOption( 0 );

								g_pPlayer->SetAddonHair(ADDONID_HAIR1_MALE, 0);		// type, color
								g_pPlayer->SetAddonItem( &coat1 );	
								g_pPlayer->SetAddonItem( &trouser1 );
							}
							else if (g_pPlayer->GetCreatureType()==1)
							{
								g_pPlayer->RemoveAddon( ADDON_COAT );	
								g_pPlayer->RemoveAddon( ADDON_TROUSER );

								MCoat		coat1;	
								MTrouser	trouser1;	

								coat1.SetItemType( 0 ); coat1.SetItemOption( 0 );
								trouser1.SetItemType( 0 ); trouser1.SetItemOption( 0 );

								g_pPlayer->SetAddonHair(ADDONID_HAIR1_FEMALE, 0);		// type, 

color
								g_pPlayer->SetAddonItem( &coat1 );	
								g_pPlayer->SetAddonItem( &trouser1 );
							}
							else
							{
								g_pPlayer->RemoveAddon( ADDON_COAT );	
								g_pPlayer->RemoveAddon( ADDON_TROUSER );
							}
							

							// 번쩍~~
							g_pTopView->SetFadeStart(1, 31, 10,  31,0,0);
						}							
					}		
					return 0L;
					*/		

					
					/*
					case VK_F7 :
						//{
						//	pWaveVolume->SetCurrentVolume(
						//			pWaveVolume->GetCurrentVolume() - 0x1000
						//		);
						//}
						//if (g_pPlayer->IsExistAddon(MCreatureWear::ADDON_SHOES))
							//g_pPlayer->SetAddonNULL(MCreatureWear::ADDON_SHOES); 
						//else g_pPlayer->SetAddon(MCreatureWear::ADDON_SHOES, 2);					
						
						// 공격 모드 전환
						if (g_pPlayer->IsAttackModeAggress())
						{
							g_pPlayer->SetAttackModePeace();
						}
						else if (g_pPlayer->IsAttackModePeace())
						{
							g_pPlayer->SetAttackModeNormal();
						}
						else if (g_pPlayer->IsAttackModeNormal())
						{
							g_pPlayer->SetAttackModeAggress();
						}

					return 0L;
					*/

					/*
					case VK_F8 :
					{
						//{
							////g_DXMusic.AddVolume( 0xF000 );
							//pWaveVolume->SetCurrentVolume(
									//pWaveVolume->GetCurrentVolume() + 0x1000
								//);
						//}
						//if (g_pPlayer->IsExistAddon(MCreatureWear::ADDON_HELM))
						//	g_pPlayer->SetAddonNULL(MCreatureWear::ADDON_HELM); 
						//else g_pPlayer->SetAddon(MCreatureWear::ADDON_HELM, 3);
						static int a = 0;

						if (a==0)
						{
							gC_vs_ui.ChangeToVampireInterface();
							a = 1;
						}
						else
						{
							gC_vs_ui.ChangeToSlayerInterface();
							a = 0;
						}
					}
					return 0L;					
					*/

					//-----------------------------------------------
					// Frame Fixing
					//-----------------------------------------------
					case VK_F11 : 
						#if defined(_DEBUG)
							if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
							{
								//eidt by sonic 2006.7.27   錦맣속醵뒈囹
								//g_UpdateDelay = (g_UpdateDelay==1)? DELAY_UPDATE_GAME : 1;
								g_UpdateDelay = (g_UpdateDelay==1)? (54 ^ 8) : 1;
							}
						#endif
						return 0L;

					//-----------------------------------------------
					// debug 메세지
					//-----------------------------------------------
					case VK_F12 : 
						g_bPutMessage = !g_bPutMessage;
						return 0L;				
					
					//-----------------------------------------------
					// Debug Message 청소~하기
					//-----------------------------------------------
					case VK_DELETE :
						{
							#ifdef	OUTPUT_DEBUG
								if (g_pDebugMessage!=NULL)
										g_pDebugMessage->Clear();
								GCSystemMessage _GCSystemMessage;
								_GCSystemMessage.setMessage("GM[Coffee]>123456789012345678901234567890123456789012345678901234567890");
								_GCSystemMessage.setType(SYSTEM_MESSAGE_PLAYER);
								_GCSystemMessage.execute(NULL);
								_GCSystemMessage.setMessage("GM[Coffee]>1234567890123456789012345678901234567890123456789012345678901");
								_GCSystemMessage.execute(NULL);
								_GCSystemMessage.setMessage("GM[Coffee]>12345678901234567890123456789012345678901234567890123456789012");
								_GCSystemMessage.execute(NULL);
							#endif
						}
						return 0L;						

				#endif
			}		
			
		return 0L; 
		case WM_TIMER :			
			//for (loop = 0; loop < STREAM_MAX; ++loop)
//					if (g_pOGG[loop] ) 
#ifndef __USE_MP3__
			if( g_DXSound.IsInit() && g_pOGG != NULL )
				g_pOGG->streamUpdate (g_pOGG);
#endif
			break;

		//---------------------------------------------------------------
		//
		//							WM_CHAR
		//
		//---------------------------------------------------------------
		case WM_CHAR :
		{
			#ifdef __WM_OUTPUT_DEBUG__
				DEBUG_ADD("[WM_CHAR]");
			#endif

			/*
			{
				ofstream file("inputtest.txt", ios::app);
				file << "input!" << (char)wParam << endl;
				file.close();
			}
			*/
			gC_vs_ui.KeyboardControl(message, wParam, lParam);

			#ifdef __WM_OUTPUT_DEBUG__
				DEBUG_ADD("UI KeyCtrl OK");
			#endif

			TCHAR ch = (TCHAR)wParam;    // character code 	
			
			//switch (ch)
			//{
			//	case '`' : g_bDrawMinimap = !g_bDrawMinimap;
			//	return 0L;
			//}

			#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
				if (g_Mode==MODE_GAME)
				switch (ch)
				{					
					///*
					case '5' :
					{
						/*
						for (int i=0; i<g_MaxNPC; i++)
						{
							MCreature* pCreature = g_pZone->GetCreature(1000+i);
							if (pCreature != NULL)
							{
								if (pCreature->IsDead())
								{
									pCreature->SetAlive();
								}
								else
								{
									pCreature->SetDead();
								}
							}
						}
						*/
						if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL) )
						{
							if (g_pPlayer->IsDead())
							{						
								g_pPlayer->SetAlive();
								g_pTopView->SetFadeEnd();
							}
							else
							{
								// 죽은 경우..
								g_pPlayer->SetDead();						
							}
						}
					}
					return 0L;
					//*/

					// [ TEST CODE ]
					case '/' :
					{
						// 상태 변경
						/*
						int newHP = g_pPlayer->GetHP() - 5;
						int newMP = g_pPlayer->GetMP() - 3;

						if (newHP < 0) newHP = 0;
						if (newMP < 0) newMP = 0;

						MStatus* pStatus = new MStatus;
						pStatus->SetStatus(MODIFY_HP, newHP);
						pStatus->SetStatus(MODIFY_MP, newMP);
						
						// UI변경					
						gC_vs_ui.SetHP(newHP, 100);
						gC_vs_ui.SetMP(newMP, 100);

						// 결과에 HP가 변하도록 설정
						MActionResult *pResult = new MActionResult;					
						pResult->Add( new MActionResultNodeChangeStatus(g_pPlayer->GetID(), pStatus) );

						// 결과 Action 등록
						//g_pPlayer->PacketSpecialActionResult( SKILL_ATTACK_MELEE );
						g_pPlayer->PacketSpecialActionResult( RESULT_VAMPIRE_DIE );

						// 결과 내용 등록(effectID가 있어야 되는데..)
						g_pPlayer->PacketAddActionResult(0, pResult);					
						
						
						//switch (rand()%6)
						//{
						//	case 0 : g_pPlayer->SetChatString("huk!"); break;
						//	case 1 : g_pPlayer->SetChatString("kek!"); break;
						//	case 2 : g_pPlayer->SetChatString("uk!"); break;
						//	case 3 : g_pPlayer->SetChatString("-_-!"); break;
						//	case 4 : g_pPlayer->SetChatString("!!!"); break;
						//	case 5 : g_pPlayer->SetChatString("*_*;"); break;
						//}	
						*/
					}
					return 0L;
					//*/

					// 살짝 붉게 변하기
					case '6' :
					{
						g_pTopView->SetFadeStart(25, 31, 2, 31,0,0);					
					}
					break;

					// 번개
					case '7' :
					{
						SetLightning(rand()%4*500+500);					
					}
					break;
					

					case '.' :
					{
						do {
							int newActionInfo;
							
							if (g_pPlayer->GetSpecialActionInfo()==ACTIONINFO_NULL)
							{
								newActionInfo = MAGIC_LIGHT;
							}
							else
							{
								newActionInfo = g_pPlayer->GetSpecialActionInfo()+1;
							}

							if (newActionInfo >= (*g_pActionInfoTable).GetMinResultActionInfo())
								newActionInfo = 0;

							int maxPlayerAction = g_pPlayer->GetActionMax();
							int newActionInfoAction = (*g_pActionInfoTable)[newActionInfo].GetAction();
							
							g_pPlayer->SetSpecialActionInfo( newActionInfo );

							// player에 있는 action인 경우..
							if (newActionInfoAction <= maxPlayerAction)
								break;
						} while (1);					
					}
					return 0L;

					case ',' :
					{
						do {
							int newActionInfo;
							
							if (g_pPlayer->GetSpecialActionInfo()==ACTIONINFO_NULL)
							{
								newActionInfo = MAGIC_LIGHT;
							}
							else
							{
								newActionInfo = g_pPlayer->GetSpecialActionInfo()-1;
							}

							if (newActionInfo < 0)
								newActionInfo = (*g_pActionInfoTable).GetMinResultActionInfo() - 1;

							int maxPlayerAction = g_pPlayer->GetActionMax();
							int newActionInfoAction = (*g_pActionInfoTable)[newActionInfo].GetAction();
							
							g_pPlayer->SetSpecialActionInfo( newActionInfo );

							// player에 있는 action인 경우..
							if (newActionInfoAction <= maxPlayerAction)
								break;
						} while (1);
					}
					return 0L;
					case 'L' :
						g_pPlayer->AddEffectStatus(EFFECTSTATUS_SHRINE_SHIELD,0xffff);
						return 0L;			
						
				}					
			#endif
		}
		return 0L;	
				

		/*
        case WM_SETCURSOR:
            // Turn off the cursor since this is a full-screen app
            //if (CDirectDraw::IsActive() )//&& CDirectDraw::IsFullscreen())
			{
				//SetCursor(LoadCursor(NULL, IDC_ARROW));
				//SetCursor(LoadCursorFromFile("Cursor2.cur"));
				//SetCursor(LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_MYCROSS)));			
				
				//SetCursor(LoadCursorFromFile("Mono.cur"));
				//SetCursor( LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_TEST)) );

				//SetCursor( LoadIcon(NULL, IDI_ERROR) );

				SetCursor(NULL);
				
				SetCapture(g_hWnd);
			}
            return TRUE;
		*/		
		// `한글입력시 IME표시가 나오지 못하도록 한다.
		//

		//---------------------------------------------------------------
		//
		//				WM_KEYUP
		//				WM_IME_COMPOSITION
		//				WM_IME_STARTCOMPOSITION
		//				WM_IME_ENDCOMPOSITION
		//
		//---------------------------------------------------------------
		case WM_KEYUP:
		 case WM_IME_COMPOSITION:
		 case WM_IME_STARTCOMPOSITION:
		 case WM_IME_ENDCOMPOSITION:
			 
			#ifdef __WM_OUTPUT_DEBUG__
				switch (message)
				{
					case WM_KEYUP:
						DEBUG_ADD("[WM_KEYUP]");
					break;

					 case WM_IME_COMPOSITION:
						 DEBUG_ADD("[WM_IME_COMPOSITION]");
					break;

					 case WM_IME_STARTCOMPOSITION:
						 DEBUG_ADD("[WM_IME_STARTCOMPOSITION]");
					break;

					 case WM_IME_ENDCOMPOSITION:
						 DEBUG_ADD("[WM_IME_ENDCOMPOSITION]");
					break;
				}	
			#endif

			gC_vs_ui.KeyboardControl(message, wParam, lParam);

			#ifdef __WM_OUTPUT_DEBUG__
				DEBUG_ADD("[WM_IME_....] OK");
			#endif

			if (message == WM_IME_STARTCOMPOSITION)
				return 0;

			break; // ! break!!!!

    }

	#ifdef __WM_OUTPUT_DEBUG__
		DEBUG_ADD("[WM_....]");
	#endif

    return DefWindowProc(hWnd, message, wParam, lParam);
}


//-----------------------------------------------------------------------------
// Name: InitApp()
// Desc: Do work required for every instance of the application:
//          Create the window, initialize data
//-----------------------------------------------------------------------------
BOOL
InitApp(int nCmdShow)
{
	WNDCLASS                    wc;
	//�邂�踞샙잚츰,눗왯깃痙
	//char rnd_PROGRAM_NAME[50];
	//char rnd_PROGRAM_TITLE[50];
	//get_rand_str(rnd_PROGRAM_NAME,5);
	//get_rand_str(rnd_PROGRAM_TITLE,5);
    // Set up and register window class
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_hInstance;
    wc.hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_DARKEDEN));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;//NAME;
	wc.lpszClassName = PROGRAM_NAME;
    //wc.lpszClassName = PROGRAM_NAME;
    RegisterClass(&wc);
	
	// Calculate the proper size for the window given a client of 640x480
    int cx,cy;	
	DWORD style = 0;
	DWORD exStyle = 0;

#ifdef __WEB_BROWSER__
	CoInitialize(NULL);
	if (HRESULT hr = FAILED(CoCreateInstance(CLSID_InternetExplorer,NULL,
			CLSCTX_LOCAL_SERVER, IID_IWebBrowser2,(LPVOID*)&g_pWebBrowser))) 
		{
			MessageBox(NULL, "IWebBrowser2 Create failed", "ERROR", MB_OK);
			return FALSE;
		}
#endif
// 2004, 8, 27, sobeit add start - mac address 체크 및, mac screen mode 체크
	memset( g_macAddress, 0, 6*sizeof(BYTE) );
	//if(GetMacAddressFromSock())
	if(GetMacAddressFromNetBIOS(g_macAddress))
	{
#if !defined(OUTPUT_DEBUG) && !defined(_DEBUG) && !defined(__DEBUG_OUTPUT__)
		CheckMacScreenMode();
#endif
	}

// 2004, 8, 27, sobeit add end - mac address 체크 및, mac screen mode 체크
	if (g_bFullScreen)
	{
		exStyle = WS_EX_TOPMOST;// | WS_EX_APPWINDOW;
		//style = WS_POPUP;
		style = WS_POPUP|WS_CLIPCHILDREN;
	//	style = WS_POPUP|WS_CLIPCHILDREN| WS_CLIPSIBLINGS;
		cx = GetSystemMetrics(SM_CXSCREEN);
		cy = GetSystemMetrics(SM_CYSCREEN);				  
	}
	else
	{
	//add by sonic 2006.4.10
	//DEVMODE  devmode;
	//	if(EnumDisplaySettings(0, 0, &devmode))
	//	{
	//		devmode.dmPelsWidth=1024;
	//		devmode.dmPelsHeight=768;
	//		devmode.dmBitsPerPel=16;
	//		devmode.dmDisplayFrequency=75;
			//ChangeDisplaySettings(&devmode,0);
	//	}
	//end
		exStyle = WS_EX_APPWINDOW;				//by viva : add WS_SYSMENU
		style = WS_CLIPCHILDREN | WS_OVERLAPPED | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU;//WS_OVERLAPPEDWINDOW;
//		style = WS_OVERLAPPEDWINDOW;
		// add by Sonic 2006.9.26
//		if(g_MyFull)
		//{
		//	cx = 1024 + GetSystemMetrics(SM_CXSIZEFRAME)*2;
		//	cy = 768 + GetSystemMetrics(SM_CYSIZEFRAME)*2+GetSystemMetrics(SM_CYMENU);	
		//}
		//else
		//{
			cx = g_GameRect.right + GetSystemMetrics(SM_CXSIZEFRAME)*2;
			cy = g_GameRect.bottom + GetSystemMetrics(SM_CYSIZEFRAME)*2+GetSystemMetrics(SM_CYMENU);
		//}
	}
	//藤속踞샙잚츰눗왯츰깃痙
	// Create a window
	/*
    g_hWnd = CreateWindowEx(exStyle,//0,
                          PROGRAM_NAME,
                          PROGRAM_TITLE,
	rnd_PROGRAM_TITLE*/

    g_hWnd = CreateWindowEx(exStyle,//0,
                          PROGRAM_NAME,
                          PROGRAM_TITLE,
						  style,
                          0,
                          0,
						  cx,
						  cy,
                          NULL,
                          NULL,
                          g_hInstance,
                          NULL);

    if (!g_hWnd)
	{
        return FALSE;
	}

#ifdef __NPROTECT__

#ifdef __NPROTECT_OLD_VERSION__

	int nProtectResult ;
	// ### nProtect ###
	if( (nProtectResult = RunNPROTECT()) != NPROTECT_OK )
	{
		char szTemp[256];
		//add by sonic 2006.4.11
		//wsprintf(szTemp,"nProtect가 실행되지 않았습니다.\n매크로 프로그램이나 해킹 프로그램을 사용한 경우가 아니라면 \n bug@darkeden.com 으로 메일을 보내주시기 바랍니다.[ErrorCode:%d]",nProtectResult);
		//MessageBox(NULL, szTemp,"nProtect Error", MB_OK);
		//end 
		CloseNPROTECT();
		return FALSE;
	}	
	
	if(g_bForceExitBynProtect2 )
	{
		CloseNPROTECT();
		return FALSE;
	}

#elif !defined(__NPROTECT_OLD_VERSION__)
	if( RunNPROTECT() != NPROTECT_OK )
	{
		char szTemp[256];
		//add by sonic 2006.4.11
		MessageBox(NULL, szTemp,"nProtect Error", MB_OK);
		//end
		CloseNPROTECT();
		return FALSE;
	}
	
	if( g_bForceExitBynProtect == true )
	{
		char szTemp[256];
		//add by sonic 2006.4.11
		//MessageBox(NULL,szTemp, "Error", MB_OK);
		//end
		return FALSE;
	}
#endif

#endif

	//
    //ShowWindow(g_hWnd, nCmdShow);
    //UpdateWindow(g_hWnd);
    //SetFocus(g_hWnd);

	//---------------------------------------------------
	// DirectX Version check
	//---------------------------------------------------
	//if (!CheckDXVersion())
	//{
	//	return 0;
	//}

	// cursor를 없애준다.
	SetCursor(NULL);
	ShowCursor( FALSE );

	// window를 보여준다.
	ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);
    SetFocus(g_hWnd);	



	//---------------------------------------------------
	// Init Game
	//---------------------------------------------------
	return InitGame();
}


//extern void OutputMemorySize();
//#include "MemoryCheck.cpp"
//#include "UIDialog.h"

//-----------------------------------------------------------------------------
// Check TerriblePatch
//-----------------------------------------------------------------------------
bool
CheckTerriblePatch()
{
	bool bCrash = false;
	int errorCode = 0;

	//-----------------------------------------------------------------------------
	// ImageObject Patch
	//-----------------------------------------------------------------------------
	AppendPatchTable apt;

/*
#ifdef _DEBUG
	apt.Init( 2 );	
	
	//-----------------------------------------------------------------------------
	// ImageObject Patch
	//-----------------------------------------------------------------------------	
	apt[0].SetOriginalInfo( FILE_SPRITE_IMAGEOBJECT, 47564158, 0x04CA );
	apt[0].AddAppendInfo( "Data\\Image\\New18ImageObject.spk", 1968462, 0x82 );
	apt[0].AddAppendInfo( "Data\\Image\\New22ImageObject.spk", 5706400, 0x8A );
	apt[0].SetFinalInfo( 55239016, 0x05D6 );

	//-----------------------------------------------------------------------------
	// Creature Patch
	//-----------------------------------------------------------------------------
	apt[1].SetOriginalInfo( FILE_ISPRITE_CREATURE, 47722828, 0x2A4E );
	apt[1].AddAppendInfo( "Data\\Image\\New18Creature.ispk", 4343846, 0x0418 );
	apt[1].AddAppendInfo( "Data\\Image\\New22Creature.ispk", 9813886, 0x0798 );
	apt[1].SetFinalInfo( 61880556, 0x35FE );

	
	//-----------------------------------------------------------------------------
	// Save
	//-----------------------------------------------------------------------------
	ofstream fileAppendInfo2(FILE_INFO_APPENDPATCH, ios::binary);
	apt.SaveToFile( fileAppendInfo2 );
	fileAppendInfo2.close();
#endif
*/

	//-----------------------------------------------------------------------------
	// Load
	//-----------------------------------------------------------------------------
	ifstream	fileAppendInfo;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_APPENDPATCH").c_str(), fileAppendInfo))
		return false;
	apt.LoadFromFile( fileAppendInfo );
	fileAppendInfo.close();

	

	//-----------------------------------------------------------------------------
	// Append 패치를 한다.
	//-----------------------------------------------------------------------------
	errorCode = 0;
	for (int i=0; i<apt.GetSize(); i++)
	{
		errorCode ++;
		if (!apt[i].ExecutePatch())
		{
			//bCrash = true;
			break;
		}

		errorCode ++;
		if (!apt[i].CheckFinalInfo())
		{
			#ifdef OUTPUT_DEBUG				
			#else
				//bCrash = true;
				break;
			#endif
		}
		
	}
	
	//-----------------------------------------------------------------------
	// 심각한 상황인가? - -; 
	//-----------------------------------------------------------------------
	if (bCrash)
	{
		//char str[256];
		//add by sonic 2006.4.11
		//sprintf(str, "화일이 손상되었습니다. 다크에덴 운영팀으로 연락주세요 [에러코드:%d]", errorCode);
		//MessageBox(NULL, str, PROGRAM_TITLE, MB_OK);
		//end 
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Check TerriblePatch
//-----------------------------------------------------------------------------
// auto-patch의 실수로 인하여
// 어쩔 수 없이 실행화일에서 체크해서 패치해야하는 경우이다.
// 그리하여.. 함수 이름이 이렇게 되었다.  --;;
//-----------------------------------------------------------------------------
/*
bool
CheckTerriblePatchOLD()
{
	bool bCrash = false;
	int errorCode = 0;
		
	//-----------------------------------------------------------------------
	//
	// Update SpritePack
	//
	//-----------------------------------------------------------------------
	// Data\\Image\\에 New18ImageObjectSPK.spk가 있으면..
	// ImageObject의 개수는 전체개수와 같은데.. 용량이 다르다면
	// "패치 작업을 하는데 시간이 좀 걸립니다."라고 메시지를 띄워준다.
	// 전체개수만큼 SPK 크기를 잡고 
	// ImageObjectSPK를 SPK로 Load한다.
	// New18ImageObjectSPK를 SPK로 Load한다.
	// SPK를 ImageObjectSPK에 write한다.
	// New18ImageObjectSPK.spk를 지운다.
	//
	// [22번 패치할때부터는..]
	// 기존에 잘못된 부분이 다 덮혀버릴정도의 패치화일이 준비되어 있으므로
	// 18패치화일이 있으면 18번 패치위치부터 쓰고
	// 22패치화일이 있으면 22번 패치위치부터 쓰면... 된다.
	const int numWrite = 2;
	char newSpkFilename[numWrite][80] = 
	{ 
		"Data\\Image\\New18ImageObject.spk", 
		"Data\\Image\\New22ImageObject.spk" 
	};
	const long writePosition[numWrite] = 
	{
		47564158,	// 18번 패치 적용할 file position
		49532618	// 22번 패치 적용후 file position
	};
	const WORD orgSpkSize[numWrite] = 
	{
		0x054C,		// 18번 패치 적용후 개수
		0x05D6		// 22번 패치 적용후 개수
	};
	const long orgFileSize[numWrite] = 
	{
		49532618,	// 18번 패치 적용후 화일사이즈
		55239016	// 22번 패치 적용후 화일사이즈
	};

	WORD spkSize;
	ifstream imageObjectFile;//(FILE_ISPRITEINDEX_CREATURE, ios::binary);
	if (!FileOpenBinary(FILE_SPRITE_IMAGEOBJECT, imageObjectFile))
		return false;
	imageObjectFile.read((char*)&spkSize, 2);	// Sprite의 개수
	imageObjectFile.close();
	
	//-----------------------------------------------------------------------
	// 용량 다르면 패치
	//-----------------------------------------------------------------------
	bool bImageObjectPatch;
	if (spkSize==orgSpkSize[numWrite-1])
	{
		 bImageObjectPatch = false;
	}
	else
	{
		bImageObjectPatch = true;		
	}

	//-----------------------------------------------------------------------
	// 패치를 적용시켜야 하는 경우
	//-----------------------------------------------------------------------
	if (bImageObjectPatch)
	{
		//MessageBox(NULL, "다크에덴 데이타를 정리하고 실행할 예정입니다.\n OK를 누르고 반응이 없더라도 자동으로 실행되니 잠시만 기

다려주세요.", PROGRAM_TITLE, MB_OK);

		for (int i=0; i<numWrite; i++)
		{
			ifstream newfile(newSpkFilename[i], ios::binary | ios::nocreate);				
			
			//-------------------------------------------------------------
			// 패치 화일이 없는 경우.. 다음꺼 체크
			//-------------------------------------------------------------
			if (!newfile.is_open())
			{
				continue;
			}
			else
			{
				SetProgressBarText("패치 화일을 적용시키고 있습니다.");
				UpdateProgressBar();

				ofstream imageObjectFile(FILE_SPRITE_IMAGEOBJECT, ios::binary | ios::ate);

				newfile.seekg( 2 );	// size부분 제외
				imageObjectFile.seekp( writePosition[i] );

				WRITE_FROM_FILE( imageObjectFile, newfile );

				newfile.close();
				
				// 개수 변경
				spkSize = orgSpkSize[i];
				imageObjectFile.seekp( 0, ios::beg );
				imageObjectFile.write((const char*)&spkSize, 2);
				imageObjectFile.close();

				// 패치화일을 지운다.
				remove( newSpkFilename[i] );
			}
		}
	}
	
	
	//-----------------------------------------------------------------------
	//
	// Append SpritePack
	//
	//-----------------------------------------------------------------------
	// 
	//-----------------------------------------------------------------------
	// Data\\Image\\New18Creature.ispk - 2001.9.26
	// 원래있던화일, 추가되는화일, 합한후개수
	// 이 방식에서는 spki는 따로 덮어야 한다.
	if (!bCrash)
	{
		const int numAppend = 2;
		char orgFilename[numAppend][80] = 
		{ 
			"Data\\Image\\Creature.ispk", 
			"Data\\Image\\Creature.ispk" 
		};
		char appFilename[numAppend][80] = 
		{ 
			"Data\\Image\\New18Creature.ispk", 
			"Data\\Image\\New22Creature.ispk" 
		};

		int total[numAppend] = 
		{ 
			0x2E66, 
			0x35FE 
		};

		// 여러번 append해서 total에 도달하게 되는 경우가 있다.

		WORD orgNum, appNum;
			
		for (int i=0; i<numAppend; i++)
		{
			
			class fstream orgFile(orgFilename[i], ios::in | ios::out | ios::binary | ios::nocreate | ios::ate);
			ifstream appFile(appFilename[i], ios::binary | ios::nocreate);		
		
			//-------------------------------------------------------------
			// 원래 화일이 없는 경우 - -;
			//-------------------------------------------------------------
			if (!orgFile.is_open())
			{
				bCrash = true;
				errorCode = 1;
				break;			
			}			
			
			orgFile.seekg( 0, ios::beg );
			orgFile.read((char*)&orgNum, 2);	// Sprite의 개수
			
			if (orgNum>=total[i])
			{
				// 이미 패치된 상황이다. 다음꺼 체크.
				orgFile.close();
				appFile.close();
				continue;
			}
				
			//-------------------------------------------------------------
			// app할께 없는 경우
			//-------------------------------------------------------------
			if (!appFile.is_open())
			{
				// 개수는 다른데 app할것이 없는 경우
				bCrash = true;
				errorCode = 2;
				break;
			}

			// total size를 체크해서 append할 필요가 있는지 체크한다.
			appFile.read((char*)&appNum, 2);	// Sprite의 개수

			//-------------------------------------------------------------
			// 두개 합쳐서 total이 된다면..		
			//-------------------------------------------------------------
			if (orgNum+appNum == total[i])
			{				
				SetProgressBarText("패치 화일을 적용시키고 있습니다.");
				UpdateProgressBar();

				// orgFile의 끝으로..
				orgFile.seekp(0, ios::end);

				WRITE_FROM_FILE( orgFile, appFile );

				// 원래 화일의 개수를 바꿔준다.
				orgFile.seekp(0, ios::beg);
				orgFile.write((const char*)&total[i], 2);

				orgFile.close();
				appFile.close();

				//---------------------------------------------------------------
				// App화일은 지운다.
				//---------------------------------------------------------------
				remove( appFilename[i] );
			}
			//-------------------------------------------------------------
			// size 콩가루.. - -;
			//-------------------------------------------------------------
			else
			{
				// 두 개 합쳐도 제대로된 숫자가 나오지 않는 경우
				// 심각한 상황이다.
				bCrash = true;
				errorCode = 3;
				break;
			}
		}
	}
	
	//-----------------------------------------------------------------------
	// 심각한 상황이다.
	//-----------------------------------------------------------------------
	if (bCrash)
	{
		char str[256];
		sprintf(str, "화일이 손상되었습니다. 다크에덴 운영팀으로 연락주세요 [에러코드:%d]", errorCode);
		MessageBox(NULL, str, PROGRAM_TITLE, MB_OK);

		return false;
	}

	return true;
}
*/

//-----------------------------------------------------------------------------
// ConvertScreenEffect
//-----------------------------------------------------------------------------
// 기존의 AlphaEffect를 ScreenEffect에 맞게 바꾼다.
//-----------------------------------------------------------------------------
/*
bool
ConvertScreenEffect()
{
	//------------------------------------------------------------	
	// Screen SpritePack 확인
	//------------------------------------------------------------	
	
//	ifstream fileSPK2(FILE_SPRITE_SCREENEFFECT, ios::binary | ios::nocreate);
//	if (fileSPK2.is_open())
//	{
//		TYPE_SPRITEID num;
//		fileSPK2.read((char*)&num, SIZE_SPRITEID);
//
//		// 크기가 같으면.. convert할 필요 없다.
//		if (num >= 0x06BE)
//		{			
//			return false;
//		}
//
//		fileSPK2.close();
//	}
	
/*	ifstream fileSPK2("Data\\Info\\EffectScreenConvert.inf", ios::binary | ios::nocreate);
	if (fileSPK2.is_open())
	{
		fileSPK2.close();
		return false;		
	}

	
	const int MAX_EST = 135;
	const int est[MAX_EST] =
	{
		SCR_EFFECTSPRITETYPE_ACID_BALL_1,		// 날아가기
		SCR_EFFECTSPRITETYPE_ACID_BALL_2,		// 터지기
		SCR_EFFECTSPRITETYPE_ACID_BOLT_1,		// 날아가기
		SCR_EFFECTSPRITETYPE_ACID_BOLT_2,		// 터지기	
		SCR_EFFECTSPRITETYPE_ACID_TOUCH,	
		SCR_EFFECTSPRITETYPE_AURA_PRISM_SHIELD,	// 방어막
		SCR_EFFECTSPRITETYPE_AURA_BALL_1,	// 캐스팅
		SCR_EFFECTSPRITETYPE_AURA_BALL_2,	// 날아가기
		SCR_EFFECTSPRITETYPE_AURA_BALL_3,	// 터지기
		SCR_EFFECTSPRITETYPE_AURA_PRISM_1,	// 캐스팅 - 바닥에 붙는 
		SCR_EFFECTSPRITETYPE_AURA_PRISM_2,	// 캐스팅 - 바닥에서 위로 올라가기
		SCR_EFFECTSPRITETYPE_AURA_PRISM_3,	// 붙어서 시작
		SCR_EFFECTSPRITETYPE_AURA_PRISM_4,	// 붙어서 반복
		SCR_EFFECTSPRITETYPE_AURA_PRISM_5,	// 붙어서 끝
		SCR_EFFECTSPRITETYPE_AURA_SHIELD_1,	// 캐스팅
		SCR_EFFECTSPRITETYPE_AURA_SHIELD_2,	// 지속
		SCR_EFFECTSPRITETYPE_AURA_SHIELD_3,	// 맞을 때 번쩍~
		SCR_EFFECTSPRITETYPE_AURA_RING,
		SCR_EFFECTSPRITETYPE_BLESS_GROUND_1,
		SCR_EFFECTSPRITETYPE_BLESS_GROUND_2,
		SCR_EFFECTSPRITETYPE_BLESS_GROUND_3,
		SCR_EFFECTSPRITETYPE_BLESS_GROUND_4,
		SCR_EFFECTSPRITETYPE_BLESS_ING,		// 반복 
		SCR_EFFECTSPRITETYPE_BLESS,			// 하나 반복으로 바뀜 //시작	
		SCR_EFFECTSPRITETYPE_CHAOS_COMBO,	
		SCR_EFFECTSPRITETYPE_CONTINUAL_LIGHT_1,
		SCR_EFFECTSPRITETYPE_CONTINUAL_LIGHT_2,
		SCR_EFFECTSPRITETYPE_CONTINUAL_LIGHT_3,
		SCR_EFFECTSPRITETYPE_CREATE_HOLY_WATER_1,
		SCR_EFFECTSPRITETYPE_CREATE_HOLY_WATER_2,
		SCR_EFFECTSPRITETYPE_CREATE_HOLY_WATER_3,
		SCR_EFFECTSPRITETYPE_CURE_PARALYSIS,		
		SCR_EFFECTSPRITETYPE_CURE_CRITICAL_WOUND_1,
		SCR_EFFECTSPRITETYPE_CURE_CRITICAL_WOUND_2,
		SCR_EFFECTSPRITETYPE_CURE_CRITICAL_WOUND_3,
		SCR_EFFECTSPRITETYPE_CURE_SERIOUS_WOUND_1,
		SCR_EFFECTSPRITETYPE_CURE_SERIOUS_WOUND_2,
		SCR_EFFECTSPRITETYPE_CURE_SERIOUS_WOUND_3,
		SCR_EFFECTSPRITETYPE_CURE_LIGHT_WOUND_1,
		SCR_EFFECTSPRITETYPE_CURE_LIGHT_WOUND_2,
		SCR_EFFECTSPRITETYPE_CURE_LIGHT_WOUND_3,
		SCR_EFFECTSPRITETYPE_CURE_POISON,
		SCR_EFFECTSPRITETYPE_DETECT_HIDDEN,
		SCR_EFFECTSPRITETYPE_DOUBLE_IMPACT,
		SCR_EFFECTSPRITETYPE_ENCHANT_CASTING1_1,
		SCR_EFFECTSPRITETYPE_ENCHANT_CASTING1_2,
		SCR_EFFECTSPRITETYPE_ENCHANT_CASTING1_3,
		SCR_EFFECTSPRITETYPE_ENCHANT_CASTING2_1,
		SCR_EFFECTSPRITETYPE_ENCHANT_CASTING2_2,
		SCR_EFFECTSPRITETYPE_ENCHANT_CASTING2_3,		
		SCR_EFFECTSPRITETYPE_EXPLOSION_OLD,
		SCR_EFFECTSPRITETYPE_HEALING_CASTING_1,
		SCR_EFFECTSPRITETYPE_HEALING_CASTING_2,
		SCR_EFFECTSPRITETYPE_HEALING_CASTING_3,
		SCR_EFFECTSPRITETYPE_HOLY_SHOOTING_AR_MALE,
		SCR_EFFECTSPRITETYPE_HOLY_SHOOTING_SMG_MALE,
		SCR_EFFECTSPRITETYPE_HOLY_SHOOTING_TR_MALE,
		SCR_EFFECTSPRITETYPE_HOLY_SHOOTING_SG_MALE,
		SCR_EFFECTSPRITETYPE_HOLY_SHOOTING_AR_FEMALE,
		SCR_EFFECTSPRITETYPE_HOLY_SHOOTING_SMG_FEMALE,		
		SCR_EFFECTSPRITETYPE_HOLY_SHOOTING_TR_FEMALE,
		SCR_EFFECTSPRITETYPE_HOLY_SHOOTING_SG_FEMALE,
		SCR_EFFECTSPRITETYPE_HOLY_SHOOTING_HIT,
		SCR_EFFECTSPRITETYPE_HOLY_WATER_1,	// 깨지는거
		SCR_EFFECTSPRITETYPE_HOLY_WATER_2,	// 실패? - -;	
		SCR_EFFECTSPRITETYPE_HURRICANE_COMBO,	
		SCR_EFFECTSPRITETYPE_LIGHT_1,
		SCR_EFFECTSPRITETYPE_LIGHT_2,
		SCR_EFFECTSPRITETYPE_LIGHT_3,
		SCR_EFFECTSPRITETYPE_POISONOUS_HAND,
		SCR_EFFECTSPRITETYPE_POTENTIAL,
		SCR_EFFECTSPRITETYPE_PROTECTION_FROM_CURSE,	
		SCR_EFFECTSPRITETYPE_PROTECTION_FROM_PARALYSIS,		
		SCR_EFFECTSPRITETYPE_PROTECTION_FROM_POISON,
		SCR_EFFECTSPRITETYPE_RAINBOW_SLASHER,
		SCR_EFFECTSPRITETYPE_REMOVE_CURSE,
		SCR_EFFECTSPRITETYPE_RESTORE_1,
		SCR_EFFECTSPRITETYPE_RESTORE_2,
		SCR_EFFECTSPRITETYPE_RESTORE_3,
		SCR_EFFECTSPRITETYPE_GUN_AR_MALE,
		SCR_EFFECTSPRITETYPE_GUN_SMG_MALE,
		SCR_EFFECTSPRITETYPE_GUN_AR_FEMALE,
		SCR_EFFECTSPRITETYPE_GUN_SMG_FEMALE,
		SCR_EFFECTSPRITETYPE_GUN_TR_MALE,
		SCR_EFFECTSPRITETYPE_GUN_SG_MALE,
		SCR_EFFECTSPRITETYPE_GUN_TR_FEMALE,
		SCR_EFFECTSPRITETYPE_GUN_SG_FEMALE,
		SCR_EFFECTSPRITETYPE_SCREW_SLASHER,
		SCR_EFFECTSPRITETYPE_SINGLE_BLOW,
		SCR_EFFECTSPRITETYPE_SMOKE,	
		SCR_EFFECTSPRITETYPE_SMOKE_1,	
		SCR_EFFECTSPRITETYPE_SMOKE_2,	
		SCR_EFFECTSPRITETYPE_SMOKE_3,	
		SCR_EFFECTSPRITETYPE_SMOKE_4,	
		SCR_EFFECTSPRITETYPE_SNAKE_COMBO_OLD,
		SCR_EFFECTSPRITETYPE_SPIRAL_SLAY,
		SCR_EFFECTSPRITETYPE_STRIKING_ING_1,
		SCR_EFFECTSPRITETYPE_STRIKING_ING_2,
		SCR_EFFECTSPRITETYPE_STRIKING_ING_3,
		SCR_EFFECTSPRITETYPE_STRIKING_1,
		SCR_EFFECTSPRITETYPE_STRIKING_2,
		SCR_EFFECTSPRITETYPE_STRIKING_3,	
		SCR_EFFECTSPRITETYPE_THUNDER_SPARK,
		SCR_EFFECTSPRITETYPE_TRIPLE_BREAK,
		SCR_EFFECTSPRITETYPE_TRIPLE_SLASHER,
		SCR_EFFECTSPRITETYPE_TWIN_BREAK,
		SCR_EFFECTSPRITETYPE_VAMPIRE_DEATH,
		SCR_EFFECTSPRITETYPE_WILD_SMASH,	
		SCR_EFFECTSPRITETYPE_DETECT_INVISIBILITY,
		SCR_EFFECTSPRITETYPE_IDENTIFY_1x1,
		SCR_EFFECTSPRITETYPE_IDENTIFY_1x3,
		SCR_EFFECTSPRITETYPE_IDENTIFY_2x2,
		SCR_EFFECTSPRITETYPE_IDENTIFY_2x3,
		SCR_EFFECTSPRITETYPE_SACRIFICE_1,			// casting
		SCR_EFFECTSPRITETYPE_SACRIFICE_2,			// 지속
		SCR_EFFECTSPRITETYPE_SNAKE_COMBO,
		SCR_EFFECTSPRITETYPE_SWORD_WAVE_1,			// frame은 따로 없다.
		SCR_EFFECTSPRITETYPE_SWORD_WAVE_2,			// frame은 따로 없다.
		SCR_EFFECTSPRITETYPE_SWORD_WAVE_3,			// frame은 따로 없다.
		SCR_EFFECTSPRITETYPE_TORNADO_SEVER_1,		// 바닥에 찍히는거 (5프레임 후에 출력)
		SCR_EFFECTSPRITETYPE_TORNADO_SEVER_2,		// 몸에 붙는거

		SCR_EFFECTSPRITETYPE_EXPLOSION,

		// 2001.9.3
		SCR_EFFECTSPRITETYPE_LIGHTNING_HANDS_1,
		SCR_EFFECTSPRITETYPE_LIGHTNING_HANDS_2,		// 반복
		SCR_EFFECTSPRITETYPE_LIGHTNING_HANDS_3,	
		
		// 2001.9.6
		SCR_EFFECTSPRITETYPE_PROTECTION_FROM_ACID_1,		// 시작
		SCR_EFFECTSPRITETYPE_PROTECTION_FROM_ACID_2,		// 지속
		
		// 2001.10.8
		SCR_EFFECTSPRITETYPE_EXPLOSION_2,
		SCR_EFFECTSPRITETYPE_EXPLOSION_3,
		SCR_EFFECTSPRITETYPE_CURE_ALL_1,				// 시작
		SCR_EFFECTSPRITETYPE_CURE_ALL_2,				// bless 뿌려주기
		SCR_EFFECTSPRITETYPE_ENCHANT_1x1,
		SCR_EFFECTSPRITETYPE_ENCHANT_1x3,
		SCR_EFFECTSPRITETYPE_ENCHANT_2x2,
		SCR_EFFECTSPRITETYPE_ENCHANT_2x3		
	};

	//------------------------------------------------------------	
	// convert할때.. progress.. 냠..
	//------------------------------------------------------------	
	SetProgressBarText("데이타 화일을 체크중입니다.");
	SetProgressBarCount( MAX_EST );	
	
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);	
	
	ShowWindow(g_hWndProgress, SW_HIDE);
	CDirectDraw::Init(g_hWndProgress, cx, cy, CDirectDraw::FULLSCREEN);	
	
	MoveWindow(g_hWndProgress, 
				(cx-progressBarWidth)/2, 
				(cy-progressBarHeight)/2, 
				progressBarWidth, 
				progressBarHeight, 
				TRUE);
	ShowWindow(g_hWndProgress, SW_SHOW);
	

	// 16 bit 모드로 바꿔야 한다.
	//DDSURFACEDESC2 ddsd;
	//CDirectDraw::GetDD()->GetDisplayMode( &ddsd );
	//CDirectDraw::GetDD()->SetDisplayMode(800, 600, 16, 0, 0);
	
	
	//------------------------------------------------------------	
	// 필요한 Frame 뽑기
	//------------------------------------------------------------	
	COrderedList<int> intList;

	ifstream efpkFile(FILE_EFRAME_ALPHAEFFECT, ios::binary);
	CEffectFramePack	EFPK;
	EFPK.LoadFromFile( efpkFile );
	efpkFile.close();

	CEffectFramePack	NewEFPK;
	NewEFPK.Init( MAX_EST );
	
	//------------------------------------------------------------	
	// 필요한 EFPK를 생성하면서 사용된 SpriteID도 구한다.
	//------------------------------------------------------------	
	for (int e=0; e<MAX_EST; e++)
	{
		int etype = est[e];

		DIRECTION_EFFECTFRAME_ARRAY& DEA = EFPK[etype];

		NewEFPK[e] = DEA;

		for (int d=0; d<8; d++)
		{
			EFFECTFRAME_ARRAY& EA = DEA[d];
			
			for (int f=0; f<EA.GetSize(); f++)
			{
				CEffectFrame& Frame = EA[f];

				int spriteID = Frame.GetSpriteID();

				intList.Add( spriteID );
			}
		}

		UpdateProgressBar();
	}	

	EFPK.Release();

	//------------------------------------------------------------	
	// Alpha --> Normal 준비..
	//------------------------------------------------------------	
	CSpriteSurface	surface;
	surface.InitOffsurface( 640, 480, DDSCAPS_SYSTEMMEMORY );	

	POINT point0 = { 0, 0 };
	
	 
	CAlphaSprite565		ASPR;

	ifstream aspkiFile(FILE_ASPRITEINDEX_ALPHAEFFECT, ios::binary);
	ifstream aspkFile(FILE_ASPRITE_ALPHAEFFECT, ios::binary);	

	TYPE_SPRITEID totalNum;
	int spriteNum = intList.GetSize();

	CSpritePack* pSPK = new CSpritePack;
	pSPK->Init( spriteNum, CDirectDraw::Is565() );

	aspkFile.read((char*)&totalNum, 2);

	int* pNewID = new int [totalNum];

	COrderedList<int>::DATA_LIST::const_iterator iID = intList.GetIterator();
	WORD *lpSurface, lPitch;

	//------------------------------------------------------------	
	// convert할때.. progress.. 냠..
	//------------------------------------------------------------	
	SetProgressBarText("데이타 화일을 변경중입니다.");
	SetProgressBarCount( spriteNum/8 );	
	

	//------------------------------------------------------------	
	// Alpha --> Normal
	//------------------------------------------------------------		
	for (int sid=0; sid<spriteNum; sid++)
	{
		int spriteID = *iID;
		
		aspkFile.seekg( 0 );
		aspkiFile.seekg( 0 );		
		
		//--------------------------------------------------------
		// Load할 FilePointer를 읽어온다.
		//--------------------------------------------------------
		long fp;	
		aspkiFile.seekg( 2 + spriteID*4 );		// 2(num) + spriteID * (4 bytes)
		aspkiFile.read((char*)&fp, 4);		
		aspkFile.seekg( fp );

		//--------------------------------------------------------
		// Load
		//--------------------------------------------------------
		ASPR.LoadFromFile( aspkFile );

		//--------------------------------------------------------
		// 크기 계산.. 걍 - -
		//--------------------------------------------------------
		int width = ASPR.GetWidth();
		int height = ASPR.GetHeight();
		
		//--------------------------------------------------------
		// AlphaSprite를 출력한 후에 
		// Sprite를 생성한다.
		//--------------------------------------------------------
		surface.FillSurface( 0 );

		surface.LockW(lpSurface, lPitch);	
		surface.BltAlphaSprite( &point0, &ASPR );			
		((*pSPK)[sid]).SetPixel(lpSurface, lPitch, width, height);

		surface.Unlock();
	
		// 원래는 ID가 spriteID였던게 sid로 바뀌게 된다.
		pNewID[ spriteID ] = sid;

		if ((sid & 0x00000007)==0x00000007)
		{
			UpdateProgressBar();	
		}

		iID++;
	}

	aspkiFile.close();
	aspkFile.close();

	// 원래대로
	//CDirectDraw::GetDD()->RestoreDisplayMode();

	//------------------------------------------------------------	
	// SpriteID를 수정한다.
	//------------------------------------------------------------	
	int numFPK = NewEFPK.GetSize();
	for (e=0; e<numFPK; e++)
	{
		DIRECTION_EFFECTFRAME_ARRAY& DEA = NewEFPK[e];

		for (int d=0; d<8; d++)
		{
			EFFECTFRAME_ARRAY& EA = DEA[d];
			
			for (int f=0; f<EA.GetSize(); f++)
			{
				CEffectFrame& Frame = EA[f];

				int cx = Frame.GetCX();
				int cy = Frame.GetCY();
				int light = Frame.GetLight();
				int spriteID = pNewID[ Frame.GetSpriteID() ];

				Frame.Set( spriteID, cx, cy, light );
			}
		}
	}

	delete [] pNewID;

	//------------------------------------------------------------	
	// FramePack저장
	//------------------------------------------------------------	
	ofstream fileFPK(FILE_EFRAME_SCREENEFFECT, ios::binary);
	ofstream fileFPKI(FILE_EFRAMEINDEX_SCREENEFFECT, ios::binary);
	NewEFPK.SaveToFile( fileFPK, fileFPKI );
	fileFPK.close();
	fileFPKI.close();
	
	//------------------------------------------------------------	
	// SpritePack 저장
	//------------------------------------------------------------	
	ofstream fileSPK(FILE_SPRITE_SCREENEFFECT, ios::binary);
	ofstream fileSPKI(FILE_SPRITEINDEX_SCREENEFFECT, ios::binary);
	pSPK->SaveToFile( fileSPK, fileSPKI );
	fileSPK.close();
	fileSPKI.close();

	delete pSPK;

	ShowWindow(g_hWndProgress, SW_HIDE);
	CDirectDraw::ReleaseAll();
	
	MoveWindow(g_hWndProgress, 
				(cx-progressBarWidth)/2, 
				(cy-progressBarHeight)/2, 
				progressBarWidth, 
				progressBarHeight, 
				TRUE);
	ShowWindow(g_hWndProgress, SW_SHOW);

	//------------------------------------------------------------	
	// 변환 체크
	//------------------------------------------------------------	
	ofstream fileCheck("Data\\Info\\EffectScreenConvert.inf", ios::binary);	
	int a = 1;
	fileCheck.write((const char*)&a, 4);
	fileCheck.close();

	return true;
}
*/
//------------------------------------------------------------------------
// ApplyPatch 
//------------------------------------------------------------------------
// 갑자기 압축을 하게 되는 바람에... - -;
//------------------------------------------------------------------------
bool
ApplyPatch()
{	
	//-----------------------------------------------------------------
	// 현재 version
	//-----------------------------------------------------------------
//	ifstream versionFile(FILE_INFO_VERSION, ios::binary);
//	int version;
//	versionFile.read((char*)&version, 4);
//	versionFile.close();

	//-----------------------------------------------------------------
	// 패치압축화일이 있나 확인
	//-----------------------------------------------------------------
//	char packFilename[256];
//	char infoFilename[256];

//	struct _finddata_t	FileData;
//	long				hFile;

	//-----------------------------------------------------------------
	// *.mpk file을 찾는다.
	//-----------------------------------------------------------------
//	if ( (hFile = _findfirst( "*.mpk", &FileData )) != -1L )
//	{		
//		int files = 1;
//		do
//		{
//			strcpy(packFilename, FileData.name);
//			files ++;
//		}
//		while (_findnext( hFile, &FileData ) == 0);
//
//		_findclose( hFile );			

		//-----------------------------------------------------------------
		// 패치압축화일의 압축 해제 
		//-----------------------------------------------------------------
//		_mkdir( "Update" );
//
//		SetProgressBarText("패치 화일의 압축을 해제하고 있습니다.");
//
//		MZLib mzlib;
//
//		// 압축할때 Updater경로를 같이 저장했으므로 같이 풀린다.
//		mzlib.Uncompress( packFilename );	
//
//		//-----------------------------------------------------------------
//		// infofile이름 결정 - packFilename에 따라서..
//		//-----------------------------------------------------------------
//		int len = strlen(packFilename);
//		char str[256];
//		strncpy(str, packFilename, len-4);	// 걍 .까지 빼준다.
//		str[len-4] = '\0';
//		sprintf(infoFilename, "Update\\%s.inf", str);
//
//	}
	//-----------------------------------------------------------------
	// mpk화일이 없는 경우는 수동패치인 경우를 체크해봐야한다.
	// packFilename이 몇번 version에 적용되는건지 알아낸다.
	//-----------------------------------------------------------------
//	else
//	{
//		/*
//		char* pUnder = strchr(packFilename+5, "_");
//		if (pUnder!=NULL)
//		{   
//			char strVersion[10];
//			strncpy(strVersion, packFilename+5, pUnder - packFilename+5);
//			int versionFrom = atoi(strVersion);
//		}
//		*/
//		
//		// 현재 버전에 맞는 패치화일을 찾는다.
//		sprintf(infoFilename, "Update\\Patch%d_*.inf", version);
//
//		if ( (hFile = _findfirst( infoFilename, &FileData )) != -1L )
//		{	
//			int files = 1;
//			do
//			{
//				sprintf(infoFilename, "Update\\%s", FileData.name);
//				files ++;
//			}
//			while (_findnext( hFile, &FileData ) == 0);
//			
//			_findclose( hFile );
//		}
//		else
//		{
//			// 수동패치도 없는 경우 == 현재 버전
//			return false;
//		}
//	}
//
//	//-----------------------------------------------------------------
//	// info 읽기
//	//-----------------------------------------------------------------	
//	CreateProgressBar("패치 정보를 체크 중입니다.");
//	
//	UpdateManager	UM;
//	UM.load( infoFilename );
//
//	SetProgressBarCount( UM.getNum()+g_numAppendFiles );	
//
//	SetProgressBarText("패치 화일을 적용시키고 있습니다.");
//	
//	//-----------------------------------------------------------------
//	// 패치화일 적용
//	//-----------------------------------------------------------------
//	int newVersion = 0;
//	while (!UM.empty())
//	{
//		Update* pUpdate = UM.front();
//		UM.pop_front();
//		
//		if (!pUpdate->execute())
//		{
//			// -_-;;
//		}	
//
//		// version up인가?
//		if (pUpdate->getVersion() > newVersion)
//		{
//			newVersion = pUpdate->getVersion();
//		}
//
//		UpdateProgressBar();
//	}
//
//	//-----------------------------------------------------------------
//	// 새 version저장
//	//-----------------------------------------------------------------
//	ofstream versionFile2(FILE_INFO_VERSION, ios::binary);
//	versionFile2.write((const char*)&newVersion, 4);
//	versionFile2.close();
//	
//	//-----------------------------------------------------------------
//	// 패치압축화일 제거
//	//-----------------------------------------------------------------
//	remove( packFilename );
//
//	//-----------------------------------------------------------------
//	// 패치화일 제거 - Updater 디렉토리 지우면서 되므로 .. 신경 안 써도 된다.
//	//-----------------------------------------------------------------
//
	return true;
}


#ifdef OUTPUT_DEBUG
	//-----------------------------------------------------------------------------
	// Check LogFile
	//-----------------------------------------------------------------------------
//	void
//	CheckCLogFile()
//	{
//		_chdir( g_CWD );
//
//		char filename[256] = { 0, };
//		char buffer[256] = { 0, };
//		char computerName[256] = { 0, };
//
//		struct _finddata_t	FileData;
//		long				hFile;
//
//		//-----------------------------------------------------------------
//		// *.spk file을 찾는다.
//		//-----------------------------------------------------------------
//		if ( (hFile = _findfirst( "Data\\UI\\txt\\Log*.txt", &FileData )) != -1L )
//		{
//			do
//			{
//				sprintf(filename, "Data\\UI\\txt\\%s", FileData.name);
//				
//				ifstream file( filename, ios::binary );
//				file.seekg( 0, ios::end );
//				
//				long fp = file.tellg();
//				
//				file.close();
//				if (fp == 0)
//				{
//					_chmod( filename, _S_IREAD | _S_IWRITE );
//					remove(filename);	
//				}
//				else
//				{
//					//copy
//					if (computerName[0]=='\0')
//					{
//						unsigned long numSize = 255;
//						GetComputerName( computerName,  &numSize );
//					}
//					
//					sprintf(buffer, "\\\\쑥갓\\Log\\%s-%s", computerName, FileData.name);
//					
//					_chmod( filename, _S_IREAD | _S_IWRITE );
//					rename(filename, buffer);						
//				}
//			}
//			
//			
//			while (_findnext( hFile, &FileData ) == 0);
//			
//			_findclose( hFile );			
//		}
//	}

	
	void
	CheckLogFile()
	{
		_chdir( g_CWD );

		char filename[256] = { 0, };
		char buffer[256] = { 0, };
		char computerName[256] = { 0, };

		struct _finddata_t	FileData;
		long				hFile;

		_mkdir( "Log" );
		
		//-----------------------------------------------------------------
		// *.spk file을 찾는다.
		//-----------------------------------------------------------------
		if ( (hFile = _findfirst( "Log\\Log*.txt", &FileData )) != -1L )
		{
			do
			{
				sprintf(filename, "Log\\%s", FileData.name);
				
				ifstream file( filename, ios::binary );
				file.seekg( 0, ios::end );
				
				long fp = file.tellg();
				
				if (fp < 39)
				{
					_chmod( filename, _S_IREAD | _S_IWRITE );
					remove(filename);	
				}
				else
				{
					file.seekg( -41, ios::end );
					file.read((char*)buffer, 24);
					buffer[24] = '\0';
					file.close();
					
					if (strcmp(buffer, "Delete DebugMessageArray")==0)
					{
						_chmod( filename, _S_IREAD | _S_IWRITE );
						remove(filename);
					}
					else
					{
						//copy
//						if (computerName[0]=='\0')
//						{
//							unsigned long numSize = 255;
//							GetComputerName( computerName,  &numSize );
//						}
//						
//						sprintf(buffer, "\\\\쑥갓\\Log\\%s-%s", computerName, FileData.name);
//						
//						_chmod( filename, _S_IREAD | _S_IWRITE );
//						rename(filename, buffer);						
					}
				}
			}
			
			
			while (_findnext( hFile, &FileData ) == 0);
			
			_findclose( hFile );			
		}
	}
	
	//-----------------------------------------------------------------------------
	// Check Flush LogFile
	//-----------------------------------------------------------------------------
	void
	CheckFlushLogFile()
	{
		//#ifdef OUTPUT_DEBUG_UPDATE_LOOP
		const DWORD flushDelay = 3*60*1000;
		static DWORD flushTime = g_CurrentTime + flushDelay;

			if (g_CurrentTime > flushTime)
			{
				if (g_pDebugMessage!=NULL)
				{
					char logFile[128];
					strcpy(logFile, g_pDebugMessage->GetFilename());
				
					if (g_pDebugMessage!=NULL)
					{
						DEBUG_ADD("--------------- Delete DebugMessageArray --------------");		
						delete g_pDebugMessage;	
						g_pDebugMessage = NULL;
					}
					_chmod( logFile, _S_IREAD | _S_IWRITE );
					remove( logFile );

					sprintf(logFile, "%s\\Log\\Log%d.txt", g_CWD, timeGetTime());
					g_pDebugMessage = new CMessageArray;
					g_pDebugMessage->Init(MAX_DEBUGMESSAGE, 256, logFile);

					DEBUG_ADD_FORMAT("[Time = %d]", g_CurrentTime);
				}

				// 3분 후
				flushTime = g_CurrentTime + flushDelay;
			}
		//#endif
	}
#endif

// HHOOK g_hHook = NULL;

//LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
//{
//    BOOL bSysKeyDown = FALSE;
//    BOOL bWinKeyDown = FALSE;
//    
//    if(nCode == HC_ACTION)
//    {
//        KBDLLHOOKSTRUCT* kbhook = (KBDLLHOOKSTRUCT*)lParam;
//
//// 알트키가 눌린것인지....
//        if(kbhook->flags & LLKHF_ALTDOWN)
//        {
//            switch(kbhook->vkCode)
//            {
//            case VK_TAB:
//            case VK_F4:
//			case VK_ESCAPE:
//                bSysKeyDown = TRUE;
//                break;
//            }
//        }
//// Control + ESC도 막쟈
//		else if(((GetAsyncKeyState( VK_CONTROL ) & 0x8000) || g_pDXInput != NULL && (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))) && kbhook->vkCode == VK_ESCAPE)
//		{
//			SHORT control = GetAsyncKeyState( VK_CONTROL );
//			bSysKeyDown = TRUE;
//		}
//        else
//        {
//            switch(kbhook->vkCode)
//            {
//            case VK_LWIN:
//            case VK_RWIN:
//                bWinKeyDown = TRUE;
//                break;
//            }
//        }
//        
//        if(bSysKeyDown || bWinKeyDown)
//            return 1;
//    }
//
//    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
//}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Initialization, message loop
//-----------------------------------------------------------------------------
int PASCAL
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{	
// 	char tttt[] = "0000000011";
// 	memcpy(lpCmdLine,tttt,strlen(tttt));
	//tttt += lpCmdLine;
#ifdef OUTPUT_DEBUG
	//lpCmdLine="";
	//MessageBox(0,"Winmain","",MB_OK);
	std::string tttt = "0000000031";
	tttt += lpCmdLine;
#endif

	//add by Soargon
	DEVMODE   DevMode; 
	EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &DevMode );
	if(DevMode.dmBitsPerPel != 16)
	{
		DEVMODE	tempDevMode = DevMode;
		tempDevMode.dmBitsPerPel = 16;
		ChangeDisplaySettings( &tempDevMode, CDS_RESET );
	}
	//end
	//add by zdj
	bool cmpFullScreen;
	if (lpCmdLine[strlen(lpCmdLine)-1] == '1')
	{
		g_MyFull=false;
		cmpFullScreen = false;
	}
	else if (lpCmdLine[strlen(lpCmdLine)-1] == '2')
	{
		g_MyFull=false;
		cmpFullScreen=true;
	}
	// add by Sonic 2006.9.26
	else if (lpCmdLine[strlen(lpCmdLine)-1] == '3')
	{
		g_MyFull=true;
		cmpFullScreen=false;
		g_GameRect.left=1023;
		g_GameRect.top=767;
		g_GameRect.right=1024;
		g_GameRect.bottom=768;
		g_x=512;
		g_y=384;
		g_SECTOR_WIDTH           = 21;
		g_SECTOR_HEIGHT          = 32;
		g_SECTOR_WIDTH_HALF      = 11;
		g_SECTOR_HEIGHT_HALF     = 17;
		g_SECTOR_SKIP_PLAYER_LEFT= -10;
		g_SECTOR_SKIP_PLAYER_UP  = -16;

		g_TILESURFACE_SECTOR_WIDTH         =27;
		g_TILESURFACE_SECTOR_HEIGHT        =37;
		g_TILESURFACE_SECTOR_OUTLINE_RIGHT =24;
		g_TILESURFACE_SECTOR_OUTLINE_DOWN  =34;
		g_TILESURFACE_WIDTH                =1296;
		g_TILESURFACE_HEIGHT               =888;
		g_TILESURFACE_OUTLINE_RIGHT        =1152;
		g_TILESURFACE_OUTLINE_DOWN         =816;
		g_TILE_X_HALF=24;
		g_TILE_Y_HALF=12;
	}
	else if (lpCmdLine[strlen(lpCmdLine)-1] == '4')
	{
		g_MyFull=true;
		cmpFullScreen=true;
		g_GameRect.left=1023;
		g_GameRect.top=767;
		g_GameRect.right=1024;
		g_GameRect.bottom=768;
		g_x=512;
		g_y=384;
		g_SECTOR_WIDTH           = 21;
		g_SECTOR_HEIGHT          = 32;
		g_SECTOR_WIDTH_HALF      = 11;
		g_SECTOR_HEIGHT_HALF     = 17;
		g_SECTOR_SKIP_PLAYER_LEFT= -10;
		g_SECTOR_SKIP_PLAYER_UP  = -16;

		g_TILESURFACE_SECTOR_WIDTH         =27;
		g_TILESURFACE_SECTOR_HEIGHT        =37;
		g_TILESURFACE_SECTOR_OUTLINE_RIGHT =24;
		g_TILESURFACE_SECTOR_OUTLINE_DOWN  =34;
		g_TILESURFACE_WIDTH                =1296;
		g_TILESURFACE_HEIGHT               =888;
		g_TILESURFACE_OUTLINE_RIGHT        =1152;
		g_TILESURFACE_OUTLINE_DOWN         =816;
		g_TILE_X_HALF=24;
		g_TILE_Y_HALF=12;
	}
	lpCmdLine[strlen(lpCmdLine)-1]=0x0;

	//std::string tttt = "NEWSTART0";
	//tttt = lpCmdLine + tttt;
	//lpCmdLine = (char *)tttt.c_str();

	/*
	CIndexSpritePack ISPK;
	ifstream ispkFile("f:\\vcpp\\clientinfo\\Data\\AppendPatch\\New18Creature.ispk", ios::binary);
	ISPK.LoadFromFile(ispkFile);
	ispkFile.close();

	ofstream ispkFile2("f:\\vcpp\\clientinfo\\Data\\AppendPatch\\New18Creature.ispk", ios::binary);
	ofstream ispkFile3("f:\\vcpp\\clientinfo\\Data\\AppendPatch\\New18Creature.ispki", ios::binary);
	ISPK.SaveToFile(ispkFile2, ispkFile3);
	ispkFile2.close();
	ispkFile3.close();
	*/
		
	
	#ifdef OUTPUT_DEBUG
		InitializeCriticalSection(&g_Lock);
	#endif	

	
	//----------------------------------------------------------
	// 하나의 application만 실행시킨다.
	//----------------------------------------------------------
	// 값 대충 넣기.. --;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = FALSE;


#ifndef OUTPUT_DEBUG
	// 2006.11.07 edit Coffee  錦攣槨넋埼옵鹿崗역
	HANDLE hMutex = CreateMutex(&sa, FALSE, "<<<DarkEden>>>");  
	/*
	HANDLE hMutex = CreateMutex(&sa, FALSE, "<<<DarkEden>>>");   
	

	if (GetLastError()==ERROR_ALREADY_EXISTS)
	{
		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage!=NULL)
			{
				DEBUG_ADD("DarkEden is Already Running..."); 
				DEBUG_ADD("--------------- Delete DebugMessageArray --------------");		
				delete g_pDebugMessage;	
				g_pDebugMessage = NULL;
			}
		#endif

		ReleaseMutex( hMutex );

		#ifdef OUTPUT_DEBUG
			DeleteCriticalSection(&g_Lock);
		#endif

		#ifndef _DEBUG
			return -1;
		#endif
	}
	
	
	if( FindWindow( "DK2", "DK2" ) != NULL )
	{
		return -1;
	}
	*/
	// end Coffee 2006.11.07  
	if( FindWindow( "PROCEXPL", "" ) != NULL )
	{
		return -1;
	}

/*	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	if((CurTime.wMonth == 1) || (CurTime.wMonth == 3) || (CurTime.wMonth == 5))
	{
		return -1;
	}*/

#endif
	
		// 현재 directory를 저장해둔다.
//	strcpy(g_CWD, __argv[0]);
 	GetModuleFileName(NULL, g_CWD, _MAX_PATH);
	char *tempCut = strrchr(g_CWD, '\\');
	if(tempCut == NULL)
		return FALSE;

	*tempCut = '\0';

//	GetCurrentDirectory( _MAX_PATH, g_CWD );

	
	SetCurrentDirectory(g_CWD);
	
// 	//	// 새로운 updater실행화일이 존재하면..
//	if (_access(UPDATER_NEW_FILENAME, 0) == 0/* && _access(UPDATER_FILENAME, 0)*/)//updaterNewFile)
//	{	
//		// 예전 Patcher를 지우고..
//		if (remove( UPDATER_FILENAME )==0)
//		{
//			// 새것을 예전걸로 rename
//			if (rename( UPDATER_NEW_FILENAME, UPDATER_FILENAME )==0)
//			{
//				DEBUG_ADD("Update Updater.exe OK"); 
//			}
//		}
////		// 예전 updater를 지우고..
////		//remove( UPDATER_FILENAME );
////		// 새것을 예전걸로 rename
////		if (rename( UPDATER_NEW_FILENAME, UPDATER_FILENAME )==0)
////		{
////			DEBUG_ADD("Update Updater.exe OK"); 
////		}
//	}	

	

#ifdef __METROTECH_TEST__
	CheckLogFile();
#endif

#ifdef DEBUG_INFO
	InitDebugInfo();
#endif
	
#ifdef DEBUG_INFO
	switch(g_nKeyMapSelect)
	{
	case 0:
		g_wAuthKeyMap = 0x5154;
		break;
	case 1:
		break;
	default:
		break;
	}
#else
//yckou:update the update.exe program
/*
	CFileFind finder;
	BOOL bWorking = finder.FindFile("Updater2.exe");
*/
	WIN32_FIND_DATA FileData1; 
	HANDLE hSearch1; 
	DWORD pid;
	HANDLE hProcess=NULL,hProcessToken=NULL;
	HWND hwndUpdate = FindWindow("#32770", "FsDarkedenLaunch");
		if(hwndUpdate != NULL)
		{
				pid =0;
				GetWindowThreadProcessId(hwndUpdate, &pid);
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
				TerminateProcess(hProcess, 0);
				int a = SendMessage(hwndUpdate, WM_CLOSE, 0 , 0);
		}
		Sleep(1000);
	hSearch1 = FindFirstFile("Updater2.exe", &FileData1); 
	if(hSearch1 != INVALID_HANDLE_VALUE)
	{
		DeleteFile("Updater.exe");
		CopyFile("Updater2.exe","Updater.exe",FALSE);
		DeleteFile("Updater2.exe");
	}
//yckou end
//add by sonic Check *.dll have Bug. 2006.4.13
//yckou begin: check invalid *.dll
	WIN32_FIND_DATA FileData; 
	HANDLE hSearch; 
	bool fFinished = false; 

	std::string InvalidDll;
	hSearch = FindFirstFile("*.dll", &FileData); 
	if (hSearch != INVALID_HANDLE_VALUE) 
	{
		while (!fFinished) 
		{
			int iLen = strlen(FileData.cFileName);
			for (int j=0;j<iLen;j++)
			{
				if(isupper(FileData.cFileName[j]) != 0)
					FileData.cFileName[j] = tolower(FileData.cFileName[j]);
			}
			InvalidDll = FileData.cFileName;

			if(InvalidDll != "timer.dll" &&
				InvalidDll != "msvcrtd.dll" &&
				InvalidDll != "msvcrt.dll" &&
				InvalidDll != "msvcp60.dll" &&
				InvalidDll != "msvcirt.dll" &&
				InvalidDll != "ifc22.dll" &&
				InvalidDll != "gl.dll" &&
				InvalidDll != "dsetup32.dll" &&
				InvalidDll != "dsetup.dll" &&
				InvalidDll != "deutil.dll" &&
				InvalidDll != "basics.dll" &&
				InvalidDll != "npx.dll" &&
				InvalidDll != "npupdate0.dll" &&
				InvalidDll != "nppsk.dll" &&
				InvalidDll != "npgmup.dll" &&
				InvalidDll != "npfgm9x.dll" &&
				InvalidDll != "npcipher.dll" &&
				InvalidDll != "mother.dll" &&
				InvalidDll != "fl.dll" &&
				InvalidDll != "dbghelp.dll" &&
				InvalidDll != "darkedenweb.dll" &&
				InvalidDll != "npchk.dll" &&
				InvalidDll != "xerces-c_2_4_0.dll")
				//MessageBox(0,(LPCTSTR)InvalidDll.c_str(),"ERROR",MB_OK);
				//MessageBox(0,
				return -1;
				
		
			if (!FindNextFile(hSearch, &FileData)) 
			{
				fFinished = true; 
			}
		} 
		// Close the search handle. 
		FindClose(hSearch);
	}
//yckou end
//end sonic

#endif

	/*
#include "packet\ServerSocket.h"

	// 한 Client는 동시에 다른 한 Client와 통신한다.
	// 여러군데의 resource(길드마크, profile...)가 필요한 경우 
	//		ResourceQueue에 저장했다가 틈틈히 받도록 한다.
	// while(1)부분은 다른 thread로 빼야된다.
	// RequestManager 접근할때는 mutex쓰든지 해야된다.
	// RequestManager의 Update()는 main thread에서 한다.
	//
	ServerSocket* pServerSocket = new ServerSocket( 9650 );

	char strClient[256];
	uint port;

	while (1)
	{
		Socket* pSocket = pServerSocket->accept();

		// request에 등록
		RequestServerPlayer* pRequestServerPlayer = new RequestServerPlayer( pSocket );
		g_pRequestManager->AddPlayer( pRequestServerPlayer );

		// g_pRequestManager는 등록된 RequestServerPlayer에 대해서 processInput/Command/Output 처리

		strcpy(strClient, pSocket->getHost().c_str());
		port = pSocket->getPort();
	}
	*/

	/*
	ifstream file("ImageObject.spk", ios::binary);
	ofstream file2("ImageObject-2.spk", ios::binary);
	ofstream file2index("ImageObject-2.spki", ios::binary);
	CSpritePack SPK;
	SPK.LoadFromFile(file);
	SPK.SaveToFile(file2, file2index);
	file.close();
	file2.close();
	file2index.close();

	return 0;
	*/

	/*
	OSVERSIONINFO        osVer;
    
    // First get the windows platform
    osVer.dwOSVersionInfoSize = sizeof(osVer);

    if( !GetVersionEx( &osVer ) )
    {
        return 0;
    }

    if (osVer.dwPlatformId == VER_PLATFORM_WIN32_NT )
    {
		g_WindowsNT = TRUE;
	}
	else 
	{
		g_WindowsNT = FALSE;
	}
	*/
	
	//OutputMemorySize();

//	return 0;

	/*
	ifstream file("f:\\vcpp\\clientexpo\\data\\image\\creatureshadow.cfpk", ios::binary);
	CCreatureFramePack CFPK;
	CFPK.LoadFromFile(file);
	file.close();

	CCreatureFramePack CFPK2;
	CFPK2.Init( CFPK.GetSize()-3 );

	for (int i=0; i<CFPK2.GetSize(); i++)
	{
		CFPK2[i] = CFPK[i];
	}

	ofstream file2("f:\\CreatureShadow.cfpk", ios::binary);
	ofstream fileindex2("f:\\CreatureShadow.cfpki", ios::binary);
	CFPK2.SaveToFile(file2, fileindex2);
	file2.close();
	fileindex2.close();
	*/
	
	//-----------------------------------------------------------------------
	// Client 시작..
	//-----------------------------------------------------------------------
	g_bNeedUpdate = FALSE;
//	OutPutClassSize();
//	return 0;

	#ifdef OUTPUT_DEBUG
	
	OutPutClassSize();
		if (g_pDebugMessage!=NULL)
		{
			delete g_pDebugMessage;		
		}
		g_pDebugMessage = new CMessageArray;

		//---------------------------------------------------
		// Debug Log File Init...
		//---------------------------------------------------
		if (g_pDebugMessage!=NULL)
		{
			char logFile[128];
			sprintf(logFile, "%s\\Log\\Log%d.txt", g_CWD, timeGetTime());			

//			#include <fcntl.h>
//			char clogFile[128];
//			sprintf(clogFile, "%s\\Data\\UI\\txt\\Logc%d.txt", g_CWD, timeGetTime());			
//			CLogFile = _open(clogFile, _O_WRONLY | _O_TEXT | _O_CREAT | _O_TRUNC);


			g_pDebugMessage->Init(MAX_DEBUGMESSAGE, 256, logFile);

			DEBUG_ADD_FORMAT("[ Compile Time : %s ]", __TIMESTAMP__);
		}
	#endif
	

	
	//return 0;

	//g_MemLogFile = _open("memlog.txt", _O_WRONLY | _O_TEXT | _O_CREAT | _O_TRUNC);

	#ifdef _DEBUG
		int tmpDbgFlag;
		tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
		tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
		tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
		//_CrtSetDbgFlag(tmpDbgFlag);
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | tmpDbgFlag);
		

		_CrtSetReportMode ( _CRT_WARN, _CRTDBG_MODE_DEBUG );
		_CrtSetReportMode ( _CRT_ERROR,	_CRTDBG_MODE_DEBUG );
		_CrtSetReportMode ( _CRT_ASSERT,_CRTDBG_MODE_DEBUG );
	#endif	
	

	g_hInstance = hInstance;
	// random
	srand(time(NULL));

	
	

	//---------------------------------------------------
	// SpritePack Loading Test
	//---------------------------------------------------
	/*
	CSpritePack tileSPK;
	ifstream	fileSPK;//(FILE_ISPRITE_ADDON, ios::binary);
	if (!FileOpenBinary(FILE_SPRITE_TILE, fileSPK))
		return false;
	tileSPK.LoadFromFile(fileSPK);
	fileSPK.close();
	*/

	g_pFileDef = new Properties;
	g_pFileDef->load(FILE_INFO_FILEDEF);			
	
	// 넷마블용
	bool bNetmarble = false;
	Properties NetmarbleConfig;
		
	NetmarbleConfig.load(g_pFileDef->getProperty("FILE_INFO_NETMARBLE").c_str());			
	bNetmarble = NetmarbleConfig.getPropertyInt("Netmarble") != 0;

	if(bNetmarble)
	{
		// -------------------------------------------------
		// read registry...
		// -------------------------------------------------
		HKEY newKey = 0;
		LONG ret = RegOpenKeyEx( HKEY_LOCAL_MACHINE, "SOFTWARE\\Netmarble\\NetmarbleDarkEden", NULL, KEY_ALL_ACCESS, &newKey);
		DWORD dwSize = MAX_PATH;
		char filepath[MAX_PATH];
		
		if( ret == ERROR_SUCCESS)
		{
			RegQueryValueEx( newKey, "RunFileName", NULL, NULL, (unsigned char*)filepath, &dwSize);

			char *pFilename = strstr(filepath, "DarkEden.exe");
			if(pFilename != NULL)
			{
				//MessageBox(0,"Error:[pFilename != NULL]","Error",MB_OK);
				strcpy(pFilename, "Updater.exe");
				RegSetValueEx( newKey, "RunFileName", NULL, REG_SZ, 
					(unsigned char*)&filepath, strlen(filepath));
			}
		}
		RegCloseKey( newKey);
	}
//	if( bNetmarble == false )
//		return -1;


	

	//MessageBox(g_hWnd, lpCmdLine, NULL, MB_OK);
		
	//------------------------------------------------------
	// Server IP가 설정되어 있지 않은 경우 --> Launcher실행
	//------------------------------------------------------
	if (strlen(lpCmdLine)==0)
	{
		#if defined(USE_UPDATER)
			
#ifndef OUTPUT_DEBUG
			if (!ReleaseMutex( hMutex ))
			{
				DEBUG_ADD_FORMAT("Failed to release mutex : %d", GetLastError()); 
			}
#endif

			#ifdef OUTPUT_DEBUG
				if (g_pDebugMessage!=NULL)
				{
					//MessageBox(0,"Error:[g_pDebugMessage!=NULL]","Error",MB_OK);
					DEBUG_ADD("No CommandLine --> Execute Updater.exe"); 
					DEBUG_ADD("--------------- Delete DebugMessageArray --------------");		
					delete g_pDebugMessage;	
					g_pDebugMessage = NULL;
				}
			#endif


			
			ReleaseAllObjects();			
			

			#ifdef OUTPUT_DEBUG
				DeleteCriticalSection(&g_Lock);
			#endif


			//_spawnl(_P_NOWAIT, "Updater.exe", "Updater.exe", NULL);
			_chdir( g_CWD );
			//::MessageBox(0,"뗍혤溝固DLL놔댄，댄轎ID：8001,헝섟珂蕨乖쳬瓊슥댄轎。","댄轎",MB_OK);
			_spawnl(_P_OVERLAY, UPDATER_FILENAME, UPDATER_FILENAME, lpCmdLine, NULL);

//			ShellExecute(g_hWnd, NULL, UPDATER_FILENAME, lpCmdLine, NULL, SW_SHOW);
			return FALSE;

		#else

			//strcpy(g_ServerIP, SERVER_IP);
			g_MaxNPC		= 0;//MAX_NPC;
			
			#ifdef _DEBUG
				g_bFullScreen	= false;
			#else
			#ifdef WINDOWS_SCREEN_DISPLAY
				g_bFullScreen	= false;
			#else
				g_bFullScreen	= true;
			#endif
			#endif
			g_bHAL			= true;
			g_bMusicSW		= true;		
			
		#endif
	}
	//------------------------------------------------------
	// Server IP가 설정되어 있는 경우
	//------------------------------------------------------
	else
	{
		char* str = lpCmdLine;
		if (strlen(str) < 8)
		{
			#ifdef OUTPUT_DEBUG
				if (g_pDebugMessage!=NULL)
				{
					//MessageBox(0,"Error [strlen(str) < 5]","Error",MBOK);
					DEBUG_ADD("Too short Commandline --> Run Updater.exe"); 
					DEBUG_ADD("--------------- Delete DebugMessageArray --------------");		
					delete g_pDebugMessage;	
					g_pDebugMessage = NULL;
				}
			#endif

#ifndef OUTPUT_DEBUG
			ReleaseMutex( hMutex );
#endif

			ReleaseAllObjects();

			
			#ifdef OUTPUT_DEBUG
				DeleteCriticalSection(&g_Lock);
			#endif

			//_spawnl(_P_NOWAIT, "Updater.exe", "Updater.exe", NULL);	
			_chdir( g_CWD );
			//::MessageBox(0,"뗍혤溝固DLL놔댄，댄轎ID：8002,헝섟珂蕨乖쳬瓊슥댄轎。","댄轎",MB_OK);
			_spawnl(_P_OVERLAY, UPDATER_FILENAME, UPDATER_FILENAME, lpCmdLine, NULL);
//			ShellExecute(g_hWnd, NULL, UPDATER_FILENAME, lpCmdLine, NULL, SW_SHOW);

			return FALSE;
		}

		bool bCheck = true;
		bool bRunUpdater = false;
		//char checkStr[9] = "NEWSTART";
		//char checkStr[9] = "START";

		//add by sonic 2006.4.9  �阮체痰甁�鑒
		DWORD GetCommand;
		//GetCommand=EXECryptor_GetHardwareID();
		GetCommand = 0x00000000;  //痰黨딧桿
		char checkStr[9];
		memset(checkStr,0,9);
		sprintf(checkStr, "%X" ,GetCommand);
		char T_checkStr[9];
		memset(T_checkStr,0,9);
		if (8 -strlen(checkStr) !=0)
		{
			memcpy(T_checkStr,checkStr,strlen(checkStr));
			int n = 8-strlen(checkStr);
			char* p=checkStr;
			for (int i =0;i<n;i++)
			{
				memcpy(p,"0",1);
				p++;
			}
			memcpy(p,T_checkStr,strlen(T_checkStr));
		}
		memcpy(checkStr,"00000000",8);
		memcpy(T_checkStr,str,8);
		//strcpy(checkStr,str);
		int i = 0;

#ifdef OUTPUT_DEBUG
		bRunUpdater = false;
#else
		////////////////////Add by Sonic 2006-6-21////////////////////////////////
		if (memcmp(checkStr,T_checkStr,strlen(checkStr)))
		{
			//MessageBox(0,checkStr,str,MB_OK);
			//눈흙꽝鑒댄轎///
			return false;
			bRunUpdater = true;
			//break;
		}
		//////////////////////////////////////////////////////////////////////////
		/* Delete by Sonic 2006-6-21
		for (i=0; i<strlen(checkStr); i++)
		{
			if (checkStr[i]!=str[i])
			{
				bRunUpdater = true;

				break;
			}
		}
		*/
		if(bRunUpdater == false && !bNetmarble && lpCmdLine[8] >= '0' && lpCmdLine[8] <= '9')
		{
			for( int i = 9; ; i++ )
			{
				//g_Dimension = lpCmdLine[8]-'0';
				if ( lpCmdLine[i] == NULL )
					break;
				
				if ( lpCmdLine[i] >= '0' && lpCmdLine[i] <= '9' )
				{
					continue;
				}
				else
					break;
			}
			if (memcmp(checkStr,str,strlen(checkStr)))
			{
				char szDimen[10];
				memset( szDimen, 0, 10 );
				memcpy( szDimen, "8", 1 );
				g_Dimension = atoi( szDimen );
			}
			else
			{
				char szDimen[10];
				memset( szDimen, 0, 10 );
				memcpy( szDimen, lpCmdLine+8, 1 );
				g_Dimension = atoi( szDimen );
			}
		//	g_Dimension = lpCmdLine[8]-'0';
		}
#endif
//add by Sonic 2006.7.26
	//藤속밗돛쇱꿎
		/*
	if (GetSystem())
	{
		HANDLE hClient = CreateFile("\\\\.\\pipe\\　　　\\",GENERIC_WRITE |	GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hClient == INVALID_HANDLE_VALUE)
		{
			//MessageBox(0,"Error","Error",MB_OK);
			bRunUpdater = true;
			return false;
		}
	}
	*/
		#ifdef OUTPUT_DEBUG
			// 창모드 시작..
			g_bTestMode = true;
			char checkTestMode[] = "TestMode";
			for (i=0; i<strlen(checkTestMode); i++)
			{
				if (checkTestMode[i]!=str[i])
				{
					g_bTestMode = false;

					break;
				}
			}

			if (g_bTestMode)
			{
				bRunUpdater = false;
			}
		#endif
			/*
		if (memcmp(checkStr,str,strlen(checkStr)))
		{
			//MessageBox(0,checkStr,str,MB_OK);
			//눈흙꽝鑒댄轎///
			return FALSE;
			bRunUpdater = true;
			//break;
		}
		*/
/////////////////////////////edit by sonic 2006.6.20///////////////////////
		if (bRunUpdater)
		{
			#ifdef OUTPUT_DEBUG
				if (g_pDebugMessage!=NULL)
				{
					//MessageBox(0,"Error [g_pDebugMessage!=NULL]","Error",MBOK);
					DEBUG_ADD("Wrong Commandline --> Run Updater.exe"); 
					DEBUG_ADD("--------------- Delete DebugMessageArray --------------");		
					delete g_pDebugMessage;	
					g_pDebugMessage = NULL;
				}
			#endif			
				
#ifndef OUTPUT_DEBUG
			ReleaseMutex( hMutex );
#endif

			ReleaseAllObjects();

			//bCheck = false;
			
			#ifdef OUTPUT_DEBUG
				DeleteCriticalSection(&g_Lock);
			#endif

			//_spawnl(_P_NOWAIT, "Updater.exe", "Updater.exe", NULL);	
			_chdir( g_CWD );

			// 여기
			// [Futec수정]
//			char szTemp[512];
//			sprintf(szTemp, "%s %s", UPDATER_FILENAME, lpCmdLine);
//			DWORD error = WinExec(szTemp, SW_SHOW);
//			switch(error)
//			{
//			case 0:
//				MessageBox(NULL, "The system is out of memory or resources. ", PROGRAM_TITLE, MB_OK);
//				break;
//
//			case ERROR_BAD_FORMAT:
//				MessageBox(NULL, "The .exe file is invalid (non-Win32 .exe or error in .exe image). ", PROGRAM_TITLE, MB_OK);
//				break;
//
//			case ERROR_FILE_NOT_FOUND:
//				MessageBox(NULL, "The specified file was not found. ", PROGRAM_TITLE, MB_OK);
//				break;
//
//			case ERROR_PATH_NOT_FOUND:
//				MessageBox(NULL, "The specified path was not found. ", PROGRAM_TITLE, MB_OK);
//				break;
//			}
			//::MessageBox(0,"뗍혤溝固DLL놔댄，댄轎ID：8003,헝섟珂蕨乖쳬瓊슥댄轎。","댄轎",MB_OK);
			_spawnl(_P_OVERLAY, UPDATER_FILENAME, UPDATER_FILENAME, lpCmdLine, NULL);
//			ShellExecute(g_hWnd, NULL, UPDATER_FILENAME, lpCmdLine, NULL, SW_SHOW);

			return FALSE;
		}
///////////////////////////////////end edit by sonic 2006.6.20///////////////////////
		#ifdef WINDOWS_SCREEN_DISPLAY
		g_bFullScreen	= false;//false;
		#else
		g_bFullScreen	= true;//false;  Z
		#endif
		
		//strcpy(g_ServerIP, SERVER_IP);
		g_MaxNPC		= 0;//MAX_NPC;

		g_bHAL			= true;
		g_bMusicSW		= true;

		#ifdef OUTPUT_DEBUG
			if (g_bTestMode)
			{
				g_bFullScreen	= false;
			}
		#endif
	}
	

	#ifdef OUTPUT_DEBUG
		//g_bFullScreen	= true;
	#endif

	// [Futec수정]
	GetFutecAddress(lpCmdLine);



	//------------------------------------------------------------------------
	// 정상적인 Patch 
	// 갑자기 압축을 하게 되는 바람에... - -;
	//------------------------------------------------------------------------
//	bool bPatched = ApplyPatch();

	//------------------------------------------------------------------------
	// ScreenEffect 생성
	//------------------------------------------------------------------------
//	ConvertScreenEffect();

	//------------------------------------------------------------------------
	// 이제부터 EffectScreen에 Append되는 SPK는 
	// 이 시점에서 Append해야 한다.
	// CheckTerriblePatch에 같이 넣어도 된다. - -;;
	//------------------------------------------------------------------------
	//AppendScreenEffect();
//add by sonic 2006.4.10 털뙤넋埼폘땡친駕
	if ( cmpFullScreen )
		{
			g_bFullScreen = true;
		}
		else
			g_bFullScreen = false;
	#ifdef _DEBUG
		g_bFullScreen = false;
	#endif
	
//end 
	if (!CheckTerriblePatch())
	{
		ReleaseAllObjects();

		#ifdef OUTPUT_DEBUG
			if (g_pDebugMessage!=NULL)
			{
				delete g_pDebugMessage;	
				g_pDebugMessage = NULL;
			}
		#endif
	
#ifndef OUTPUT_DEBUG
		ReleaseMutex( hMutex );
#endif

		#ifdef OUTPUT_DEBUG
			DeleteCriticalSection(&g_Lock);
		#endif

		return 0;
	}
	

	//----------------------------------------------------------------
	// Updater.exe를 update 시킨다.
	//----------------------------------------------------------------
//	ifstream updaterNewFile(UPDATER_NEW_FILENAME, ios::binary | ios::nocreate);

	// 새로운 updater실행화일이 존재하면..
	if (!_access(UPDATER_NEW_FILENAME, 0))//updaterNewFile)
	{	
//		updaterNewFile.close();

		// 예전 updater를 지우고..
		if (remove( UPDATER_FILENAME )==0)
		{
			// 새것을 예전걸로 rename
			if (rename( UPDATER_NEW_FILENAME, UPDATER_FILENAME )==0)
			{
				//MessageBox(0,"Error:[rename( UPDATER_NEW_FILENAME, UPDATER_FILENAME )==0]","Error",MB_OK);
				DEBUG_ADD("Update Updater.exe OK"); 
			}
		}
	}	

	//strcpy(g_ServerIP, SERVER_IP);
	//---------------------------------------------------
	// Update 디렉토리를 지워준다.
	//---------------------------------------------------
	// updater에서 지워야 하는데..빼먹는 바람에.. T_T;;
	//---------------------------------------------------
	char CWD[_MAX_PATH];

	// 현재 path읽기
	if (_getcwd( CWD, _MAX_PATH )!=NULL)
	{	
		char UpdateDir[_MAX_PATH];

		int lenCWD = strlen(CWD);
		if (lenCWD > 1)
		{
			if (CWD[lenCWD-1]=='\\')
			{
				sprintf(UpdateDir, "%s%s", CWD, DIRECTORY_UPDATE);
			}
			else
			{
				sprintf(UpdateDir, "%s\\%s", CWD, DIRECTORY_UPDATE);
			}
		}
		
		if (_rmdir( UpdateDir )!=0)
		{			
			// PATH가 잘못된 경우
			if (errno==ENOENT)
			{				
			}
			// 뭔가 있어서 안 지워진 경우..		
			else //if (errno==ENOTEMPTY)
			{
				// 현재 directory를 기억해둔다.				
				if (_chdir( UpdateDir ) == 0)
				{
					//---------------------------------------------------
					// file하나하나를 지워준다. T_T;
					//---------------------------------------------------
					struct _finddata_t	FileData;
					long				hFile;

					// 모든 화일을 읽어온다.
					if( (hFile = _findfirst( "*.*", &FileData )) != -1L )					
					{
						while (_findnext( hFile, &FileData ) == 0)
						{
							// .으로 시작하는 건 지울 필요 없당..
							if (FileData.name[0] != '.')
							{
								remove( FileData.name );
							}
						}

						// 끝
						_findclose( hFile );			
					}
					
					_chdir( CWD );

					if (_rmdir( UpdateDir )==0)
					{
						// 잘 지워졌다.
					}
				}				
			}
		}
	}

	
	RemoveProgressBar();

	//-----------------------------------------------------------------
	// 패치 내용 찍어준다.
	//-----------------------------------------------------------------
//	if (bPatched)
//	{
//		ShowPatchLogWindow();
//	}	


	BOOL bBadTimer = FALSE;

    MSG                         msg;

	NETMARBLE_INFO NetmarbleInfo;
	REALSERVER_INFO RealServerInfo;
	if(bNetmarble)
	{
		if(ParsingNetmarble(lpCmdLine, NetmarbleInfo) == false)
		{
			if(NetmarbleConfig.getPropertyInt("Netmarble") == 1)
			{
				//add by sonic 2006.4.11
				//MessageBox(NULL, "넷마블용 다크에덴은 넷마블 홈페이지를 통해서만 실행이 가능합니다.", PROGRAM_TITLE, MB_OK);
				//end

				return -1;
			}

			bNetmarble = false;
		}
	} else
	{
//		 //test  mode|demention|ID|key
//		char szBuf[1024]; 
//		sprintf(szBuf, "%s", lpCmdLine); 
//		MessageBox(NULL, szBuf, PROGRAM_TITLE, MB_OK);// ??
		if(ParsingRealServer(lpCmdLine, g_Dimension, RealServerInfo) == false)
		{
//#ifndef OUTPUT_DEBUG
//			MessageBox(NULL, "웹 페이지 에서 다시 실행해주시기 바랍니다.", PROGRAM_TITLE, MB_OK);
//			ShellExecute(NULL, NULL, "www.darkeden.com", NULL, NULL, SW_SHOW);
//			return -1;
//
//#endif
		}
	}
	//藤속쇱꿎
/* Add by sonic 2006.9.14 藤속뚤WPE 쇱꿎 */
_APICheck.init();
/* ************************************* */
//	FILE *fp = fopen("Data\\Info\\SuperUser.inf","rt");	
//	MessageBox(NULL,__TIME__,__TIME__,MB_OK);
//	if(fp != NULL)
//	{
//		char szLine[512];
//		fgets(szLine,512,fp);
//		char szCompare[512];
//		bool	superUser = true;
//		wsprintf(szCompare,"%d%s",__LINE__,__TIME__);
//		MessageBox(NULL,szCompare,szCompare,MB_OK);
//		if(strcmp(szLine,szCompare))
//			superUser = false;
//		fgets(szLine,512,fp);
//		fclose(fp);
//		
//		int Code=0;
//		char *tempCode = new char[strlen(szCompare)];
//		memset(tempCode, 0, strlen(szCompare) );
//		for(int i=0;i<strlen(szCompare);i++)
//		{
//			if( i != 0 )
//				tempCode[i] = szCompare[1] + szCompare[i];
//			else
//				tempCode[i] = szCompare[0] + szCompare[i];			
//		}
//		MessageBox(NULL, tempCode, tempCode, MB_OK);
//		if(strcmp(tempCode,szLine))
//			superUser = false;
//		delete []tempCode;	
//
//		std::string myIP;
//		
//		WORD wVersionRequested; 
//		WSADATA wsaData; 
//		char name[255]; 
//		PHOSTENT hostinfo; 
//		wVersionRequested = MAKEWORD( 2, 0 ); 
//		if ( WSAStartup( wVersionRequested, &wsaData ) == 0 ) 
//		{
//			if( gethostname ( name, sizeof(name)) == 0) 
//			{
//				if((hostinfo = gethostbyname(name)) != NULL) 
//					myIP= inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list); 
//				else
//					superUser = false;
//			} else superUser = false;
//			WSACleanup( ); 
//		} else
//			superUser = false;
//		if(strstr(myIP.c_str(),"220.117.154")==NULL)
//			superUser = false;
//
//		MessageBox(NULL, myIP.c_str(), myIP.c_str(), MB_OK);
//		if(superUser)
//		{
//			g_bSuperUser = true;			
//		} else
//			g_bSuperUser = false;
//	}

	DARKEDEN_LANGUAGE Language = CheckDarkEdenLanguage();
	
	switch( Language )
	{
	case DARKEDEN_CHINESE :
		gC_ci = new CI_CHINESE;
		break;
	default :
		gC_ci = new CI_KOREAN;
		break;
	}
	
#ifndef _DEBUG
	if( gC_ci->IsKorean() == true )
		InitCrashReport();
#endif
	if (InitApp(nCmdShow))
	{	

		
		DDSCAPS2 ddsCaps2;
		DWORD dwTotal;
		DWORD dwFree;
		ZeroMemory(&ddsCaps2, sizeof(ddsCaps2)); 
		ddsCaps2.dwCaps = DDSCAPS_VIDEOMEMORY;//DDSCAPS_TEXTURE; 
		HRESULT hr = CDirectDraw::GetDD()->GetAvailableVidMem(&ddsCaps2, &dwTotal, &dwFree);

		DDCAPS	driverCaps;
		ZeroMemory( &driverCaps, sizeof(driverCaps) );
		driverCaps.dwSize = sizeof(driverCaps);
		
		hr = CDirectDraw::GetDD()->GetCaps( &driverCaps, NULL );
		
		g_dwVideoMemory = driverCaps.dwVidMemTotal;
//		g_dwVideoMemory = driverCaps.dwVidMemFree; 
		
		// 넷마블용
		if(bNetmarble)
		{
			g_pUserInformation->NetmarbleID = NetmarbleInfo.ID;
			g_pUserInformation->NetmarblePassword = NetmarbleInfo.Password;
			g_pUserInformation->WorldID = NetmarbleInfo.WorldID;
			g_pUserInformation->ServerID = NetmarbleInfo.ServerID;
//			g_pUserInformation->GoreLevel = NetmarbleInfo.bGore;
			g_pUserInformation->bNetmarbleGoreLevel = NetmarbleInfo.bGore;
			g_pUserInformation->IsNetmarble = bNetmarble;
		}
		else
		{
			// mode|demention|ID|key
			g_pUserInformation->IsAutoLogIn			= RealServerInfo.bMode;
			if(true == g_pUserInformation->IsAutoLogIn)
			{
				g_pUserInformation->UserID				= RealServerInfo.ID;
				g_pUserInformation->AutoLogInKeyValue	= RealServerInfo.Key;
			
				g_Dimension = RealServerInfo.WorldID;

#ifdef OUTPUT_DEBUG
				char szBuf[1024];
				sprintf(szBuf, "mode = %d, id = %d,world = %s,key = %s", g_pUserInformation->IsAutoLogIn,
																		

	g_Dimension,
																		

	g_pUserInformation->UserID.GetString(),
																		

	g_pUserInformation->AutoLogInKeyValue.GetString());
				DEBUG_ADD(szBuf);
#endif
			}			
			
		
		}

		if( gC_ci->IsKorean() )
			g_pUserInformation->SetKorean();
		else if (gC_ci->IsChinese() )
			g_pUserInformation->SetChinese();
		else if (gC_ci->IsJapanese() )
			g_pUserInformation->SetJapanese();

//		#if !defined(OUTPUT_DEBUG) && !defined(__DEBUG_OUTPUT__)
//		g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)KeyboardHook, hInstance, 0);
//
//		SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, TRUE, NULL, NULL); 
//		#endif

		//static DWORD lastTime=1;
		//------------------------------
		// 초기화 되지 않았으면 초기화한다.
		//------------------------------
		if (!g_pTopView->IsInit())
		{
			g_pTopView->Init();			
		
		}

		g_bActiveApp = TRUE;
		//g_bActiveGame = TRUE;
		DEBUG_CMD(MIN_CLRSCR, "시작");
		DEBUG_CMD(MIN_SHOWWND, "------------------------------");
		

		while (TRUE)
		{
//			Sleep(1);	//add by viva
			/* add by sonic 藤속뚤sock쇱꿎 */
				_APICheck.CheckApi();
			/* *************************** */
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			//if (GetMessage(&msg, NULL, 0, 0))
			{	
				if (!GetMessage(&msg, NULL, 0, 0))
					break;
					//return msg.wParam;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (g_bActiveApp
#ifdef OUTPUT_DEBUG
				|| g_bTestMode
#endif
				)
			{
	
				{						

					g_CurrentTime = timeGetTime();

					//if (g_CurrentTime - lastTime > g_UpdateDelay)
					{
						// CDirectDraw가 작동중이지 않을 경우에는 return
						//if (g_bActiveApp)// && CDirectDraw::IsActive())
						if (g_pUpdate!=NULL)
						{
							// 노파심.. 으흠.. --;;
							CWinUpdate*	pCurrentUpdate = g_pUpdate;

							pCurrentUpdate->Update();

							#ifdef __METROTECH_TEST__
								CheckFlushLogFile();
							#endif

						}

						//lastTime = g_CurrentTime;
					}
#ifdef __NPROTECT__
					if (1)// g_bSuperUser == false)
					{
						if( CheckNPROTECT() != NPROTECT_OK )
						{
							//MessageBox(0,"#1","#1",MB_OK);
							bBadTimer =  TRUE;
							g_nProtectSTATUS = NPROTECT_CHECK_ERROR;
						}
					}
					if(g_nProtectSTATUS != NPROTECT_NORMAL)
					{
						//MessageBox(0,"#2","#2",MB_OK);
						bBadTimer =  TRUE;
						break;
					}
#endif
					
					if( g_bForceExitBynProtect )
					{
						//MessageBox(0,"#3","#3",MB_OK);
						bBadTimer = TRUE;
						
						break;
					}

					//#ifdef OUTPUT_DEBUG
						DWORD timeGap = g_CurrentTime - g_StartTime;
							
						if (timeGap > 1000)
						{
							//UpdateFrame();
					
							g_FrameRate = (g_FrameCount - g_StartFrameCount) * 1000 / timeGap;

							// 15 fps 이상
							g_bGoodFPS = (g_FrameRate >= g_FrameGood);
							
							g_StartTime = g_CurrentTime;
							g_StartFrameCount = g_FrameCount;
						}								
				}
			}
			else
			{
				// Make sure we go to sleep if we have nothing else to do
				WaitMessage();
			}
		}	

		#ifndef __OUTPUT_DEBUG__
		SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, FALSE, NULL, NULL); 
		#endif
	}
	else
	{		
	//	InitFail("Initialize Failed!");        
	}
	//add by Soargon
	ChangeDisplaySettings( &DevMode, CDS_RESET );
	//end
	//-----------------------------------------------------------------------------
	// 타이머 이상으로 종료하는 경우
	//-----------------------------------------------------------------------------
	if (bBadTimer)
	{
		if (g_pUIDialog!=NULL)
		{
#if defined(__NPROTECT_OLD_VERSION__)&&defined(__NPROTECT__)
			if( g_nProtectSTATUS != NPROTECT_NORMAL && g_nProtectSTATUS != NPROTECT_MAX )
			{
				g_pUIDialog->PopupFreeMessageDlg( g_nProtectMessage[g_nProtectSTATUS].c_str() );
			} else
				g_pUIDialog->PopupFreeMessageDlg( "매크로나 바이러스가 발견되어 다크에덴이 종료됩니다. 바이러스 검사를 해보시고, 정상적으로 게임 진행을 하는데 이러한 문제가 또 발생된다면 bug@darkeden.com으로 메일을 보내주세요.", -1, -1, 0 );
#endif

#if defined(__NPROTECT__)&&!defined(__NPROTECT_OLD_VERSION__)
			if( g_bForceExitBynProtect )
			{
				if( g_nProtectMessage.find( g_nProtectErrorMessage ) != g_nProtectMessage.end() )
				{
					char szTemp[256];
					wsprintf(szTemp,"%s [%d]", g_nProtectMessage[g_nProtectErrorMessage].c_str(), g_nProtectErrorMessage2 );
					g_pUIDialog->PopupFreeMessageDlg( szTemp );
				}
				else
				{
					char szTemp[256];
					wsprintf(szTemp,"nProtect의 알 수 없는 오류 에러코드[%d][%d]입니다. 다크에덴을 종료합니다.",g_nProtectErrorMessage, g_nProtectErrorMessage2);
					g_pUIDialog->PopupFreeMessageDlg( szTemp );
				}
			}
#endif
				
//			if(g_bForceExitBynProtect2)
//			{
//				g_pUIDialog->PopupFreeMessageDlg( "nProtect 관련 파일 초기화 에러가 발생했습니다. 관리자 계정으로 로그인하여 게임을 실행하시기 바랍니다.", -1, -1, 0 );
//			}else
//			{
//				if(g_bForceExitBynProtect)
//					g_pUIDialog->PopupFreeMessageDlg( "바이러스 및 해킹 차단기에 의해 다크에덴이 종료됩니다. 바이러스 검사를 해보시고, 정상적으로 게임 진행을 하는데 이러한 문제가 또 발생된다면, bug@darkeden.com 으로 메일을 보내주세요.", -1, -1, 0 );
//				else
//					g_pUIDialog->PopupFreeMessageDlg( "매크로나 바이러스가 발견되어 다크에덴이 종료됩니다. 바이러스 검사를 해보시고, 정상적으로 게임 진행을 하는데 이러한 문제가 또 발생된다면 bug@darkeden.com으로 메일을 보내주세요.", -1, -1, 0 );
//			}
//			//MessageBox(NULL, "Timer Error!", PROGRAM_TITLE, MB_OK | MB_TOPMOST);
			
			while (1)
			{
				UpdateInput();
				UpdateMouse();

//				if (CDirect3D::IsHAL())
//				{
//					gC_vs_ui.Show();
//				}
//				else
				{	
					gC_vs_ui.Show();

					POINT point = { 0, 0 };
					RECT rect = { 0, 0, g_GameRect.right, g_GameRect.bottom };
					
					g_pBack->BltNoColorkey( &point, g_pLast, &rect );	
				}
				
				CDirectDraw::Flip();

				if (g_pDXInput->KeyDown(DIK_ESCAPE)
					|| g_pDXInput->KeyDown(DIK_NUMPADENTER)
					|| g_pDXInput->KeyDown(DIK_RETURN)
					|| g_pDXInput->KeyDown(DIK_SPACE))
				{
					break;
				}
			}
		}
	}

	//-----------------------------------------------------------------------------
	// 모든 object들 제거
	//-----------------------------------------------------------------------------	
	ReleaseAllObjects();
#ifdef __WEB_BROWSER__
	if(NULL != g_pWebBrowser)
	{
		g_pWebBrowser->Quit();
		g_pWebBrowser->Release();
		DeleteObject(g_pWebBrowser);
		g_pWebBrowser= NULL;
		CoUninitialize();
	}
#endif
//#if defined(OUTPUT_DEBUG) && !defined(_DEBUG)
	delete gC_ci;
#ifdef OUTPUT_DEBUG
//	_close(CLogFile);
//	CheckCLogFile();
#endif

	#ifdef OUTPUT_DEBUG	
		if (g_bNeedUpdate)	
		{
			//MessageBox(0,"Error:[g_bNeedUpdate]","Error",MB_OK);
			DEBUG_ADD("Need to Update! Run Updater.exe"); 
		}

		DEBUG_ADD("---------------[  End ReleaseAllObjects  ]---------------");
	
		if (g_pDebugMessage!=NULL)
		{
			DEBUG_ADD("--------------- Delete DebugMessageArray --------------");		
			delete g_pDebugMessage;	
			g_pDebugMessage = NULL;
		}
	#endif
	DEBUG_CMD(MIN_HIDEWND, "------------------------------");


	if (g_pClientConfig!=NULL)
	{
		delete g_pClientConfig;
		g_pClientConfig = NULL;
	}

#ifdef __NPROTECT__
	CloseNPROTECT();
#endif


#ifndef OUTPUT_DEBUG
	ReleaseMutex( hMutex );
#endif

	#ifdef OUTPUT_DEBUG
		DeleteCriticalSection(&g_Lock);
	#endif
	
	//----------------------------------------------------------------
	// Updater를 실행시킨다.
	//----------------------------------------------------------------
	if (g_bNeedUpdate)
	{
		//_spawnl(_P_NOWAIT, UPDATER_FILENAME, UPDATER_FILENAME, "UPDATE", NULL);
		char szTemp[512];
		sprintf(szTemp, "UPDATE %s", lpCmdLine);
		_chdir( g_CWD );
		//::MessageBox(0,"뗍혤溝固DLL놔댄，댄轎ID：8004,헝섟珂蕨乖쳬瓊슥댄轎。","댄轎",MB_OK);
		_spawnl(_P_OVERLAY, UPDATER_FILENAME, UPDATER_FILENAME, szTemp, NULL);		
//		ShellExecute(g_hWnd, NULL, UPDATER_FILENAME, szTemp, NULL, SW_SHOW);
	}	

	return 0;
}
BOOL GetSystem()
{
	OSVERSIONINFO OsInfo;
	OsInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&OsInfo))
	{
		// platform
		switch (OsInfo.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_WINDOWS:
			if(OsInfo.dwMajorVersion == 3)
				//MessageBox(0,"꾸鱗溝固:WIN95","斤口",MB_OK);
				//strcpy(sys.chSystem,"꾸鱗溝固:WIN95");
				return FALSE;
			else if(OsInfo.dwMajorVersion == 4)
				//MessageBox(0,"꾸鱗溝固:WIN98","斤口",MB_OK);
				return FALSE;
				//strcpy(sys.chSystem,"꾸鱗溝固:WIN98");
			break;
		case VER_PLATFORM_WIN32_NT:
			if(OsInfo.dwMajorVersion == 5)
				//MessageBox(0,"꾸鱗溝固:WIN2000","斤口",MB_OK);
				return TRUE;
				//strcpy(sys.chSystem,"꾸鱗溝固:WIN2000");
			else
				return TRUE;
				//MessageBox(0,"꾸鱗溝固:WINNT","斤口",MB_OK);
				//strcpy(sys.chSystem,"꾸鱗溝固:WINNT");
			break;
		default:
			return FALSE;
			//strcpy(sys.chSystem,"灌列溝固!!");
			break;
		}
		// version and language
	}
	return FALSE;
}
