#include "Client_PCH.h"
#define __NPROTECT__
//#include "EXECryptor.h"
/* add by sonic 2006.9.14 start Ôö¼Ó¶ÔWPEÆÁ±Î*/
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
#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#else
#include "../../basic/Platform.h"
#endif
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
DWORD				g_CurrentTime		= 0;		// ½Ã°£
DWORD				g_CurrentFrame		= 0;		// frame¼ö

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
BOOL				g_bNeedUpdate			= FALSE; // updateÇØ¾ßµÇ³ª?

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
// [Futec¼öÁ¤]
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
	bool bMode;		// 0: ¼öµ¿Á¢¼Ó, 1:Á¤»óÁ¢¼Ó
	int WorldID;	// 0: ºê¶õ, 1: Æç·¹½º
	MString ID;		// id
	MString Key;	// Key
};
// 2004, 7, 14, sobeit add end

BYTE g_macAddress[6];
extern void CheckMacScreenMode();
extern BOOL GetMacAddressFromSock();
extern BOOL InitDebugInfo();
// add by Sonic 2006.9.26 ¼ì²â1024 * 768°æ±¾È«¾Ö±äÁ¿
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
// addFileÀ» ÀÐ¾î¼­ originalFileÀÇ ³¡¿¡ ºÙÀÎ´Ù.
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
//Ëæ»ú×Ö·û´®
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
// Get Futec Address [Futec¼öÁ¤]
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
	// ÇÁ·Î±×·¥ Áß´Ü..
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
const int g_numAppendFiles = 16;		// AppendPatch.infÀÇ fileµé °³¼ö

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
char*		g_pPatchLogBuffer = NULL;	// ¾ó¸¶³ª Å¬Áö ¸ô¶ó¼­¸® global¿¡ µ×´Ù.

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

	class ifstream file("PatchLog.txt", ios::nocreate);	// text fileÀÌ´Ù.

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

		// \r\nÀ» ºÙ¿©Áà¾ß ´ÙÀ½ÁÙÀÌ µÈ´Ù.		
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
	g_hPatchLogWnd = CreateWindow("PatchLog", "´ÙÅ©¿¡µ§ ÆÐÄ¡³»¿ë", 
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
		
		// ¹Ù·Î returnÇÏ¸é main window°¡ Á×´Â´Ù. - -;		
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
// DirectX VersionÀ» checkÇÑ´Ù.
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
	// DX 7.0 ÀÌ»ó
    if (dwVer==DXVER_DX_7)
	{			
		return true;
	}
	
	//InitFail("You need to install DirectX 7.0 or later version...");

	// ¾È ±ò·ÈÀ¸¸é ¾Æ¿¹ ±ò¾Æ¹ö¸®ÀÚ..
	char directory[_MAX_PATH];

	// ÇöÀç µð·ºÅä¸®¸¦ ¾ò¾î¼­ 
	GetCurrentDirectory( _MAX_PATH, directory );

	// ½ÇÇàÈ­ÀÏ ÀÌ¸§À» ºÙÀÎ´Ù.
	sprintf(directory, "%s\\%s", directory, PROGRAM_FILENAME);

	CDirectSetup::SetRestartProgram(directory);
	CDirectSetup::DirectXInstall(g_hWnd, g_hInstance, "DirectX7", true);
	
	// »õ·Î ±ò°í ³ª¼­ ¹Ù·Î ½ÇÇàÇØµµ µÇ´Â°É±î?
	// rebooting ÇØ¾ßÇÏÁö ¾ÊÀ»±î??

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
			// Mid°¡ ´Ù ¿¬ÁÖµÇ¾ú´Ù´Â ¸»ÀÌ´Ù.
			if (wParam==MCI_NOTIFY_SUCCESSFUL)
			{
				// Opening µ¿¿µ»óÀÌ ³¡³­ °æ¿ì
				if (g_pAvi!=NULL && (DWORD)lParam==g_pAvi->dwID) 
				{ 
					g_pAvi->bEndFlag=true; 
					g_pAvi->Close(); 

					// Login È­¸éÀ¸·Î...
					SetMode( MODE_MAINMENU );
				}				
				// À½¾Ç ¿¬ÁÖ°¡ ³¡³­ °æ¿ì
				else
				{
					/*
					// ¹Ýº¹ ¿¬ÁÖ ¾ÈÇÑ´Ù.
					if (g_pUserOption->PlayMusic)//g_Music.IsPause())	// °ú¿¬ ÇÊ¿äÇÒ±î.. - -;
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
						// ¹Ýº¹ÇØ¼­ ¿¬ÁÖÇÑ´Ù.
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

			// activeµÆ°í minimized°¡ ¾Æ´Ñ »óÅÂ°¡ ActiveGameÀÌ´ç..
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
				
				// ÀÔ·ÂÀ» ÃÊ±âÈ­ÇÑ´Ù.
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
				// OpeningÀ» ³¡³½´Ù.
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
				// Volume Á¶Àý
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
				//				Debug Mode ¿ë
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
					// ¿òÁ÷ÀÌ´Â ¹æ¹ý ¼³Á¤
					//-----------------------------------------------
					/*
					case VK_F2 :
					{					
						if (g_pPlayer->IsStop())
						{
							// ¿òÁ÷ÀÌ´Â ÇüÅÂ ¹Ù²Ù±â
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

							// ³²ÀÚ Slayer
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
							

							// ¹øÂ½~~
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
						
						// °ø°Ý ¸ðµå ÀüÈ¯
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
								//eidt by sonic 2006.7.27   ÐÞ¸Ä¼ÓËÙµØÖ·
								//g_UpdateDelay = (g_UpdateDelay==1)? DELAY_UPDATE_GAME : 1;
								g_UpdateDelay = (g_UpdateDelay==1)? (54 ^ 8) : 1;
							}
						#endif
						return 0L;

					//-----------------------------------------------
					// debug ¸Þ¼¼Áö
					//-----------------------------------------------
					case VK_F12 : 
						g_bPutMessage = !g_bPutMessage;
						return 0L;				
					
					//-----------------------------------------------
					// Debug Message Ã»¼Ò~ÇÏ±â
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
				class ofstream file("inputtest.txt", ios::app);
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
								// Á×Àº °æ¿ì..
								g_pPlayer->SetDead();						
							}
						}
					}
					return 0L;
					//*/

					// [ TEST CODE ]
					case '/' :
					{
						// »óÅÂ º¯°æ
						/*
						int newHP = g_pPlayer->GetHP() - 5;
						int newMP = g_pPlayer->GetMP() - 3;

						if (newHP < 0) newHP = 0;
						if (newMP < 0) newMP = 0;

						MStatus* pStatus = new MStatus;
						pStatus->SetStatus(MODIFY_HP, newHP);
						pStatus->SetStatus(MODIFY_MP, newMP);
						
						// UIº¯°æ					
						gC_vs_ui.SetHP(newHP, 100);
						gC_vs_ui.SetMP(newMP, 100);

						// °á°ú¿¡ HP°¡ º¯ÇÏµµ·Ï ¼³Á¤
						MActionResult *pResult = new MActionResult;					
						pResult->Add( new MActionResultNodeChangeStatus(g_pPlayer->GetID(), pStatus) );

						// °á°ú Action µî·Ï
						//g_pPlayer->PacketSpecialActionResult( SKILL_ATTACK_MELEE );
						g_pPlayer->PacketSpecialActionResult( RESULT_VAMPIRE_DIE );

						// °á°ú ³»¿ë µî·Ï(effectID°¡ ÀÖ¾î¾ß µÇ´Âµ¥..)
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

					// »ìÂ¦ ºÓ°Ô º¯ÇÏ±â
					case '6' :
					{
						g_pTopView->SetFadeStart(25, 31, 2, 31,0,0);					
					}
					break;

					// ¹ø°³
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

							// player¿¡ ÀÖ´Â actionÀÎ °æ¿ì..
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

							// player¿¡ ÀÖ´Â actionÀÎ °æ¿ì..
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
		// `ÇÑ±ÛÀÔ·Â½Ã IMEÇ¥½Ã°¡ ³ª¿ÀÁö ¸øÇÏµµ·Ï ÇÑ´Ù.
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
	//Éú³ÉËæ»úÀàÃû,´°¿Ú±êÌâ
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
// 2004, 8, 27, sobeit add start - mac address Ã¼Å© ¹×, mac screen mode Ã¼Å©
	memset( g_macAddress, 0, 6*sizeof(BYTE) );
	//if(GetMacAddressFromSock())
	if(GetMacAddressFromNetBIOS(g_macAddress))
	{
#if !defined(OUTPUT_DEBUG) && !defined(_DEBUG) && !defined(__DEBUG_OUTPUT__)
		CheckMacScreenMode();
#endif
	}

// 2004, 8, 27, sobeit add end - mac address Ã¼Å© ¹×, mac screen mode Ã¼Å©
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
	//Ôö¼ÓËæ»úÀàÃû´°¿ÚÃû±êÌâ
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
		//wsprintf(szTemp,"nProtect°¡ ½ÇÇàµÇÁö ¾Ê¾Ò½À´Ï´Ù.\n¸ÅÅ©·Î ÇÁ·Î±×·¥ÀÌ³ª ÇØÅ· ÇÁ·Î±×·¥À» »ç¿ëÇÑ °æ¿ì°¡ ¾Æ´Ï¶ó¸é \n bug@darkeden.com À¸·Î ¸ÞÀÏÀ» º¸³»ÁÖ½Ã±â ¹Ù¶ø´Ï´Ù.[ErrorCode:%d]",nProtectResult);
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

	// cursor¸¦ ¾ø¾ÖÁØ´Ù.
	SetCursor(NULL);
	ShowCursor( FALSE );

	// window¸¦ º¸¿©ÁØ´Ù.
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
	class ofstream fileAppendInfo2(FILE_INFO_APPENDPATCH, ios::binary);
	apt.SaveToFile( fileAppendInfo2 );
	fileAppendInfo2.close();
#endif
*/

	//-----------------------------------------------------------------------------
	// Load
	//-----------------------------------------------------------------------------
	class ifstream	fileAppendInfo;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_INFO_APPENDPATCH").c_str(), fileAppendInfo))
		return false;
	apt.LoadFromFile( fileAppendInfo );
	fileAppendInfo.close();

	

	//-----------------------------------------------------------------------------
	// Append ÆÐÄ¡¸¦ ÇÑ´Ù.
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
	// ½É°¢ÇÑ »óÈ²ÀÎ°¡? - -; 
	//-----------------------------------------------------------------------
	if (bCrash)
	{
		//char str[256];
		//add by sonic 2006.4.11
		//sprintf(str, "È­ÀÏÀÌ ¼Õ»óµÇ¾ú½À´Ï´Ù. ´ÙÅ©¿¡µ§ ¿î¿µÆÀÀ¸·Î ¿¬¶ôÁÖ¼¼¿ä [¿¡·¯ÄÚµå:%d]", errorCode);
		//MessageBox(NULL, str, PROGRAM_TITLE, MB_OK);
		//end 
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Check TerriblePatch
//-----------------------------------------------------------------------------
// auto-patchÀÇ ½Ç¼ö·Î ÀÎÇÏ¿©
// ¾îÂ¿ ¼ö ¾øÀÌ ½ÇÇàÈ­ÀÏ¿¡¼­ Ã¼Å©ÇØ¼­ ÆÐÄ¡ÇØ¾ßÇÏ´Â °æ¿ìÀÌ´Ù.
// ±×¸®ÇÏ¿©.. ÇÔ¼ö ÀÌ¸§ÀÌ ÀÌ·¸°Ô µÇ¾ú´Ù.  --;;
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
	// Data\\Image\\¿¡ New18ImageObjectSPK.spk°¡ ÀÖÀ¸¸é..
	// ImageObjectÀÇ °³¼ö´Â ÀüÃ¼°³¼ö¿Í °°Àºµ¥.. ¿ë·®ÀÌ ´Ù¸£´Ù¸é
	// "ÆÐÄ¡ ÀÛ¾÷À» ÇÏ´Âµ¥ ½Ã°£ÀÌ Á» °É¸³´Ï´Ù."¶ó°í ¸Þ½ÃÁö¸¦ ¶ç¿öÁØ´Ù.
	// ÀüÃ¼°³¼ö¸¸Å­ SPK Å©±â¸¦ Àâ°í 
	// ImageObjectSPK¸¦ SPK·Î LoadÇÑ´Ù.
	// New18ImageObjectSPK¸¦ SPK·Î LoadÇÑ´Ù.
	// SPK¸¦ ImageObjectSPK¿¡ writeÇÑ´Ù.
	// New18ImageObjectSPK.spk¸¦ Áö¿î´Ù.
	//
	// [22¹ø ÆÐÄ¡ÇÒ¶§ºÎÅÍ´Â..]
	// ±âÁ¸¿¡ Àß¸øµÈ ºÎºÐÀÌ ´Ù µ¤Çô¹ö¸±Á¤µµÀÇ ÆÐÄ¡È­ÀÏÀÌ ÁØºñµÇ¾î ÀÖÀ¸¹Ç·Î
	// 18ÆÐÄ¡È­ÀÏÀÌ ÀÖÀ¸¸é 18¹ø ÆÐÄ¡À§Ä¡ºÎÅÍ ¾²°í
	// 22ÆÐÄ¡È­ÀÏÀÌ ÀÖÀ¸¸é 22¹ø ÆÐÄ¡À§Ä¡ºÎÅÍ ¾²¸é... µÈ´Ù.
	const int numWrite = 2;
	char newSpkFilename[numWrite][80] = 
	{ 
		"Data\\Image\\New18ImageObject.spk", 
		"Data\\Image\\New22ImageObject.spk" 
	};
	const long writePosition[numWrite] = 
	{
		47564158,	// 18¹ø ÆÐÄ¡ Àû¿ëÇÒ file position
		49532618	// 22¹ø ÆÐÄ¡ Àû¿ëÈÄ file position
	};
	const WORD orgSpkSize[numWrite] = 
	{
		0x054C,		// 18¹ø ÆÐÄ¡ Àû¿ëÈÄ °³¼ö
		0x05D6		// 22¹ø ÆÐÄ¡ Àû¿ëÈÄ °³¼ö
	};
	const long orgFileSize[numWrite] = 
	{
		49532618,	// 18¹ø ÆÐÄ¡ Àû¿ëÈÄ È­ÀÏ»çÀÌÁî
		55239016	// 22¹ø ÆÐÄ¡ Àû¿ëÈÄ È­ÀÏ»çÀÌÁî
	};

	WORD spkSize;
	class ifstream imageObjectFile;//(FILE_ISPRITEINDEX_CREATURE, ios::binary);
	if (!FileOpenBinary(FILE_SPRITE_IMAGEOBJECT, imageObjectFile))
		return false;
	imageObjectFile.read((char*)&spkSize, 2);	// SpriteÀÇ °³¼ö
	imageObjectFile.close();
	
	//-----------------------------------------------------------------------
	// ¿ë·® ´Ù¸£¸é ÆÐÄ¡
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
	// ÆÐÄ¡¸¦ Àû¿ë½ÃÄÑ¾ß ÇÏ´Â °æ¿ì
	//-----------------------------------------------------------------------
	if (bImageObjectPatch)
	{
		//MessageBox(NULL, "´ÙÅ©¿¡µ§ µ¥ÀÌÅ¸¸¦ Á¤¸®ÇÏ°í ½ÇÇàÇÒ ¿¹Á¤ÀÔ´Ï´Ù.\n OK¸¦ ´©¸£°í ¹ÝÀÀÀÌ ¾ø´õ¶óµµ ÀÚµ¿À¸·Î ½ÇÇàµÇ´Ï Àá½Ã¸¸ ±â

´Ù·ÁÁÖ¼¼¿ä.", PROGRAM_TITLE, MB_OK);

		for (int i=0; i<numWrite; i++)
		{
			class ifstream newfile(newSpkFilename[i], ios::binary | ios::nocreate);				
			
			//-------------------------------------------------------------
			// ÆÐÄ¡ È­ÀÏÀÌ ¾ø´Â °æ¿ì.. ´ÙÀ½²¨ Ã¼Å©
			//-------------------------------------------------------------
			if (!newfile.is_open())
			{
				continue;
			}
			else
			{
				SetProgressBarText("ÆÐÄ¡ È­ÀÏÀ» Àû¿ë½ÃÅ°°í ÀÖ½À´Ï´Ù.");
				UpdateProgressBar();

				class ofstream imageObjectFile(FILE_SPRITE_IMAGEOBJECT, ios::binary | ios::ate);

				newfile.seekg( 2 );	// sizeºÎºÐ Á¦¿Ü
				imageObjectFile.seekp( writePosition[i] );

				WRITE_FROM_FILE( imageObjectFile, newfile );

				newfile.close();
				
				// °³¼ö º¯°æ
				spkSize = orgSpkSize[i];
				imageObjectFile.seekp( 0, ios::beg );
				imageObjectFile.write((const char*)&spkSize, 2);
				imageObjectFile.close();

				// ÆÐÄ¡È­ÀÏÀ» Áö¿î´Ù.
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
	// ¿ø·¡ÀÖ´øÈ­ÀÏ, Ãß°¡µÇ´ÂÈ­ÀÏ, ÇÕÇÑÈÄ°³¼ö
	// ÀÌ ¹æ½Ä¿¡¼­´Â spki´Â µû·Î µ¤¾î¾ß ÇÑ´Ù.
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

		// ¿©·¯¹ø appendÇØ¼­ total¿¡ µµ´ÞÇÏ°Ô µÇ´Â °æ¿ì°¡ ÀÖ´Ù.

		WORD orgNum, appNum;
			
		for (int i=0; i<numAppend; i++)
		{
			
			class fstream orgFile(orgFilename[i], ios::in | ios::out | ios::binary | ios::nocreate | ios::ate);
			class ifstream appFile(appFilename[i], ios::binary | ios::nocreate);		
		
			//-------------------------------------------------------------
			// ¿ø·¡ È­ÀÏÀÌ ¾ø´Â °æ¿ì - -;
			//-------------------------------------------------------------
			if (!orgFile.is_open())
			{
				bCrash = true;
				errorCode = 1;
				break;			
			}			
			
			orgFile.seekg( 0, ios::beg );
			orgFile.read((char*)&orgNum, 2);	// SpriteÀÇ °³¼ö
			
			if (orgNum>=total[i])
			{
				// ÀÌ¹Ì ÆÐÄ¡µÈ »óÈ²ÀÌ´Ù. ´ÙÀ½²¨ Ã¼Å©.
				orgFile.close();
				appFile.close();
				continue;
			}
				
			//-------------------------------------------------------------
			// appÇÒ²² ¾ø´Â °æ¿ì
			//-------------------------------------------------------------
			if (!appFile.is_open())
			{
				// °³¼ö´Â ´Ù¸¥µ¥ appÇÒ°ÍÀÌ ¾ø´Â °æ¿ì
				bCrash = true;
				errorCode = 2;
				break;
			}

			// total size¸¦ Ã¼Å©ÇØ¼­ appendÇÒ ÇÊ¿ä°¡ ÀÖ´ÂÁö Ã¼Å©ÇÑ´Ù.
			appFile.read((char*)&appNum, 2);	// SpriteÀÇ °³¼ö

			//-------------------------------------------------------------
			// µÎ°³ ÇÕÃÄ¼­ totalÀÌ µÈ´Ù¸é..		
			//-------------------------------------------------------------
			if (orgNum+appNum == total[i])
			{				
				SetProgressBarText("ÆÐÄ¡ È­ÀÏÀ» Àû¿ë½ÃÅ°°í ÀÖ½À´Ï´Ù.");
				UpdateProgressBar();

				// orgFileÀÇ ³¡À¸·Î..
				orgFile.seekp(0, ios::end);

				WRITE_FROM_FILE( orgFile, appFile );

				// ¿ø·¡ È­ÀÏÀÇ °³¼ö¸¦ ¹Ù²ãÁØ´Ù.
				orgFile.seekp(0, ios::beg);
				orgFile.write((const char*)&total[i], 2);

				orgFile.close();
				appFile.close();

				//---------------------------------------------------------------
				// AppÈ­ÀÏÀº Áö¿î´Ù.
				//---------------------------------------------------------------
				remove( appFilename[i] );
			}
			//-------------------------------------------------------------
			// size Äá°¡·ç.. - -;
			//-------------------------------------------------------------
			else
			{
				// µÎ °³ ÇÕÃÄµµ Á¦´ë·ÎµÈ ¼ýÀÚ°¡ ³ª¿ÀÁö ¾Ê´Â °æ¿ì
				// ½É°¢ÇÑ »óÈ²ÀÌ´Ù.
				bCrash = true;
				errorCode = 3;
				break;
			}
		}
	}
	
	//-----------------------------------------------------------------------
	// ½É°¢ÇÑ »óÈ²ÀÌ´Ù.
	//-----------------------------------------------------------------------
	if (bCrash)
	{
		char str[256];
		sprintf(str, "È­ÀÏÀÌ ¼Õ»óµÇ¾ú½À´Ï´Ù. ´ÙÅ©¿¡µ§ ¿î¿µÆÀÀ¸·Î ¿¬¶ôÁÖ¼¼¿ä [¿¡·¯ÄÚµå:%d]", errorCode);
		MessageBox(NULL, str, PROGRAM_TITLE, MB_OK);

		return false;
	}

	return true;
}
*/

//-----------------------------------------------------------------------------
// ConvertScreenEffect
//-----------------------------------------------------------------------------
// ±âÁ¸ÀÇ AlphaEffect¸¦ ScreenEffect¿¡ ¸Â°Ô ¹Ù²Û´Ù.
//-----------------------------------------------------------------------------
/*
bool
ConvertScreenEffect()
{
	//------------------------------------------------------------	
	// Screen SpritePack È®ÀÎ
	//------------------------------------------------------------	
	
//	class ifstream fileSPK2(FILE_SPRITE_SCREENEFFECT, ios::binary | ios::nocreate);
//	if (fileSPK2.is_open())
//	{
//		TYPE_SPRITEID num;
//		fileSPK2.read((char*)&num, SIZE_SPRITEID);
//
//		// Å©±â°¡ °°À¸¸é.. convertÇÒ ÇÊ¿ä ¾ø´Ù.
//		if (num >= 0x06BE)
//		{			
//			return false;
//		}
//
//		fileSPK2.close();
//	}
	
/*	class ifstream fileSPK2("Data\\Info\\EffectScreenConvert.inf", ios::binary | ios::nocreate);
	if (fileSPK2.is_open())
	{
		fileSPK2.close();
		return false;		
	}

	
	const int MAX_EST = 135;
	const int est[MAX_EST] =
	{
		SCR_EFFECTSPRITETYPE_ACID_BALL_1,		// ³¯¾Æ°¡±â
		SCR_EFFECTSPRITETYPE_ACID_BALL_2,		// ÅÍÁö±â
		SCR_EFFECTSPRITETYPE_ACID_BOLT_1,		// ³¯¾Æ°¡±â
		SCR_EFFECTSPRITETYPE_ACID_BOLT_2,		// ÅÍÁö±â	
		SCR_EFFECTSPRITETYPE_ACID_TOUCH,	
		SCR_EFFECTSPRITETYPE_AURA_PRISM_SHIELD,	// ¹æ¾î¸·
		SCR_EFFECTSPRITETYPE_AURA_BALL_1,	// Ä³½ºÆÃ
		SCR_EFFECTSPRITETYPE_AURA_BALL_2,	// ³¯¾Æ°¡±â
		SCR_EFFECTSPRITETYPE_AURA_BALL_3,	// ÅÍÁö±â
		SCR_EFFECTSPRITETYPE_AURA_PRISM_1,	// Ä³½ºÆÃ - ¹Ù´Ú¿¡ ºÙ´Â 
		SCR_EFFECTSPRITETYPE_AURA_PRISM_2,	// Ä³½ºÆÃ - ¹Ù´Ú¿¡¼­ À§·Î ¿Ã¶ó°¡±â
		SCR_EFFECTSPRITETYPE_AURA_PRISM_3,	// ºÙ¾î¼­ ½ÃÀÛ
		SCR_EFFECTSPRITETYPE_AURA_PRISM_4,	// ºÙ¾î¼­ ¹Ýº¹
		SCR_EFFECTSPRITETYPE_AURA_PRISM_5,	// ºÙ¾î¼­ ³¡
		SCR_EFFECTSPRITETYPE_AURA_SHIELD_1,	// Ä³½ºÆÃ
		SCR_EFFECTSPRITETYPE_AURA_SHIELD_2,	// Áö¼Ó
		SCR_EFFECTSPRITETYPE_AURA_SHIELD_3,	// ¸ÂÀ» ¶§ ¹øÂ½~
		SCR_EFFECTSPRITETYPE_AURA_RING,
		SCR_EFFECTSPRITETYPE_BLESS_GROUND_1,
		SCR_EFFECTSPRITETYPE_BLESS_GROUND_2,
		SCR_EFFECTSPRITETYPE_BLESS_GROUND_3,
		SCR_EFFECTSPRITETYPE_BLESS_GROUND_4,
		SCR_EFFECTSPRITETYPE_BLESS_ING,		// ¹Ýº¹ 
		SCR_EFFECTSPRITETYPE_BLESS,			// ÇÏ³ª ¹Ýº¹À¸·Î ¹Ù²ñ //½ÃÀÛ	
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
		SCR_EFFECTSPRITETYPE_HOLY_WATER_1,	// ±úÁö´Â°Å
		SCR_EFFECTSPRITETYPE_HOLY_WATER_2,	// ½ÇÆÐ? - -;	
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
		SCR_EFFECTSPRITETYPE_SACRIFICE_2,			// Áö¼Ó
		SCR_EFFECTSPRITETYPE_SNAKE_COMBO,
		SCR_EFFECTSPRITETYPE_SWORD_WAVE_1,			// frameÀº µû·Î ¾ø´Ù.
		SCR_EFFECTSPRITETYPE_SWORD_WAVE_2,			// frameÀº µû·Î ¾ø´Ù.
		SCR_EFFECTSPRITETYPE_SWORD_WAVE_3,			// frameÀº µû·Î ¾ø´Ù.
		SCR_EFFECTSPRITETYPE_TORNADO_SEVER_1,		// ¹Ù´Ú¿¡ ÂïÈ÷´Â°Å (5ÇÁ·¹ÀÓ ÈÄ¿¡ Ãâ·Â)
		SCR_EFFECTSPRITETYPE_TORNADO_SEVER_2,		// ¸ö¿¡ ºÙ´Â°Å

		SCR_EFFECTSPRITETYPE_EXPLOSION,

		// 2001.9.3
		SCR_EFFECTSPRITETYPE_LIGHTNING_HANDS_1,
		SCR_EFFECTSPRITETYPE_LIGHTNING_HANDS_2,		// ¹Ýº¹
		SCR_EFFECTSPRITETYPE_LIGHTNING_HANDS_3,	
		
		// 2001.9.6
		SCR_EFFECTSPRITETYPE_PROTECTION_FROM_ACID_1,		// ½ÃÀÛ
		SCR_EFFECTSPRITETYPE_PROTECTION_FROM_ACID_2,		// Áö¼Ó
		
		// 2001.10.8
		SCR_EFFECTSPRITETYPE_EXPLOSION_2,
		SCR_EFFECTSPRITETYPE_EXPLOSION_3,
		SCR_EFFECTSPRITETYPE_CURE_ALL_1,				// ½ÃÀÛ
		SCR_EFFECTSPRITETYPE_CURE_ALL_2,				// bless »Ñ·ÁÁÖ±â
		SCR_EFFECTSPRITETYPE_ENCHANT_1x1,
		SCR_EFFECTSPRITETYPE_ENCHANT_1x3,
		SCR_EFFECTSPRITETYPE_ENCHANT_2x2,
		SCR_EFFECTSPRITETYPE_ENCHANT_2x3		
	};

	//------------------------------------------------------------	
	// convertÇÒ¶§.. progress.. ³È..
	//------------------------------------------------------------	
	SetProgressBarText("µ¥ÀÌÅ¸ È­ÀÏÀ» Ã¼Å©ÁßÀÔ´Ï´Ù.");
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
	

	// 16 bit ¸ðµå·Î ¹Ù²ã¾ß ÇÑ´Ù.
	//DDSURFACEDESC2 ddsd;
	//CDirectDraw::GetDD()->GetDisplayMode( &ddsd );
	//CDirectDraw::GetDD()->SetDisplayMode(800, 600, 16, 0, 0);
	
	
	//------------------------------------------------------------	
	// ÇÊ¿äÇÑ Frame »Ì±â
	//------------------------------------------------------------	
	COrderedList<int> intList;

	class ifstream efpkFile(FILE_EFRAME_ALPHAEFFECT, ios::binary);
	CEffectFramePack	EFPK;
	EFPK.LoadFromFile( efpkFile );
	efpkFile.close();

	CEffectFramePack	NewEFPK;
	NewEFPK.Init( MAX_EST );
	
	//------------------------------------------------------------	
	// ÇÊ¿äÇÑ EFPK¸¦ »ý¼ºÇÏ¸é¼­ »ç¿ëµÈ SpriteIDµµ ±¸ÇÑ´Ù.
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
	// Alpha --> Normal ÁØºñ..
	//------------------------------------------------------------	
	CSpriteSurface	surface;
	surface.InitOffsurface( 640, 480, DDSCAPS_SYSTEMMEMORY );	

	POINT point0 = { 0, 0 };
	
	 
	CAlphaSprite565		ASPR;

	class ifstream aspkiFile(FILE_ASPRITEINDEX_ALPHAEFFECT, ios::binary);
	class ifstream aspkFile(FILE_ASPRITE_ALPHAEFFECT, ios::binary);	

	TYPE_SPRITEID totalNum;
	int spriteNum = intList.GetSize();

	CSpritePack* pSPK = new CSpritePack;
	pSPK->Init( spriteNum, CDirectDraw::Is565() );

	aspkFile.read((char*)&totalNum, 2);

	int* pNewID = new int [totalNum];

	COrderedList<int>::DATA_LIST::const_iterator iID = intList.GetIterator();
	WORD *lpSurface, lPitch;

	//------------------------------------------------------------	
	// convertÇÒ¶§.. progress.. ³È..
	//------------------------------------------------------------	
	SetProgressBarText("µ¥ÀÌÅ¸ È­ÀÏÀ» º¯°æÁßÀÔ´Ï´Ù.");
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
		// LoadÇÒ FilePointer¸¦ ÀÐ¾î¿Â´Ù.
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
		// Å©±â °è»ê.. °Á - -
		//--------------------------------------------------------
		int width = ASPR.GetWidth();
		int height = ASPR.GetHeight();
		
		//--------------------------------------------------------
		// AlphaSprite¸¦ Ãâ·ÂÇÑ ÈÄ¿¡ 
		// Sprite¸¦ »ý¼ºÇÑ´Ù.
		//--------------------------------------------------------
		surface.FillSurface( 0 );

		surface.LockW(lpSurface, lPitch);	
		surface.BltAlphaSprite( &point0, &ASPR );			
		((*pSPK)[sid]).SetPixel(lpSurface, lPitch, width, height);

		surface.Unlock();
	
		// ¿ø·¡´Â ID°¡ spriteID¿´´ø°Ô sid·Î ¹Ù²î°Ô µÈ´Ù.
		pNewID[ spriteID ] = sid;

		if ((sid & 0x00000007)==0x00000007)
		{
			UpdateProgressBar();	
		}

		iID++;
	}

	aspkiFile.close();
	aspkFile.close();

	// ¿ø·¡´ë·Î
	//CDirectDraw::GetDD()->RestoreDisplayMode();

	//------------------------------------------------------------	
	// SpriteID¸¦ ¼öÁ¤ÇÑ´Ù.
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
	// FramePackÀúÀå
	//------------------------------------------------------------	
	class ofstream fileFPK(FILE_EFRAME_SCREENEFFECT, ios::binary);
	class ofstream fileFPKI(FILE_EFRAMEINDEX_SCREENEFFECT, ios::binary);
	NewEFPK.SaveToFile( fileFPK, fileFPKI );
	fileFPK.close();
	fileFPKI.close();
	
	//------------------------------------------------------------	
	// SpritePack ÀúÀå
	//------------------------------------------------------------	
	class ofstream fileSPK(FILE_SPRITE_SCREENEFFECT, ios::binary);
	class ofstream fileSPKI(FILE_SPRITEINDEX_SCREENEFFECT, ios::binary);
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
	// º¯È¯ Ã¼Å©
	//------------------------------------------------------------	
	class ofstream fileCheck("Data\\Info\\EffectScreenConvert.inf", ios::binary);	
	int a = 1;
	fileCheck.write((const char*)&a, 4);
	fileCheck.close();

	return true;
}
*/
//------------------------------------------------------------------------
// ApplyPatch 
//------------------------------------------------------------------------
// °©ÀÚ±â ¾ÐÃàÀ» ÇÏ°Ô µÇ´Â ¹Ù¶÷¿¡... - -;
//------------------------------------------------------------------------
bool
ApplyPatch()
{	
	//-----------------------------------------------------------------
	// ÇöÀç version
	//-----------------------------------------------------------------
//	class ifstream versionFile(FILE_INFO_VERSION, ios::binary);
//	int version;
//	versionFile.read((char*)&version, 4);
//	versionFile.close();

	//-----------------------------------------------------------------
	// ÆÐÄ¡¾ÐÃàÈ­ÀÏÀÌ ÀÖ³ª È®ÀÎ
	//-----------------------------------------------------------------
//	char packFilename[256];
//	char infoFilename[256];

//	struct _finddata_t	FileData;
//	long				hFile;

	//-----------------------------------------------------------------
	// *.mpk fileÀ» Ã£´Â´Ù.
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
		// ÆÐÄ¡¾ÐÃàÈ­ÀÏÀÇ ¾ÐÃà ÇØÁ¦ 
		//-----------------------------------------------------------------
//		_mkdir( "Update" );
//
//		SetProgressBarText("ÆÐÄ¡ È­ÀÏÀÇ ¾ÐÃàÀ» ÇØÁ¦ÇÏ°í ÀÖ½À´Ï´Ù.");
//
//		MZLib mzlib;
//
//		// ¾ÐÃàÇÒ¶§ Updater°æ·Î¸¦ °°ÀÌ ÀúÀåÇßÀ¸¹Ç·Î °°ÀÌ Ç®¸°´Ù.
//		mzlib.Uncompress( packFilename );	
//
//		//-----------------------------------------------------------------
//		// infofileÀÌ¸§ °áÁ¤ - packFilename¿¡ µû¶ó¼­..
//		//-----------------------------------------------------------------
//		int len = strlen(packFilename);
//		char str[256];
//		strncpy(str, packFilename, len-4);	// °Á .±îÁö »©ÁØ´Ù.
//		str[len-4] = '\0';
//		sprintf(infoFilename, "Update\\%s.inf", str);
//
//	}
	//-----------------------------------------------------------------
	// mpkÈ­ÀÏÀÌ ¾ø´Â °æ¿ì´Â ¼öµ¿ÆÐÄ¡ÀÎ °æ¿ì¸¦ Ã¼Å©ÇØºÁ¾ßÇÑ´Ù.
	// packFilenameÀÌ ¸î¹ø version¿¡ Àû¿ëµÇ´Â°ÇÁö ¾Ë¾Æ³½´Ù.
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
//		// ÇöÀç ¹öÀü¿¡ ¸Â´Â ÆÐÄ¡È­ÀÏÀ» Ã£´Â´Ù.
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
//			// ¼öµ¿ÆÐÄ¡µµ ¾ø´Â °æ¿ì == ÇöÀç ¹öÀü
//			return false;
//		}
//	}
//
//	//-----------------------------------------------------------------
//	// info ÀÐ±â
//	//-----------------------------------------------------------------	
//	CreateProgressBar("ÆÐÄ¡ Á¤º¸¸¦ Ã¼Å© ÁßÀÔ´Ï´Ù.");
//	
//	UpdateManager	UM;
//	UM.load( infoFilename );
//
//	SetProgressBarCount( UM.getNum()+g_numAppendFiles );	
//
//	SetProgressBarText("ÆÐÄ¡ È­ÀÏÀ» Àû¿ë½ÃÅ°°í ÀÖ½À´Ï´Ù.");
//	
//	//-----------------------------------------------------------------
//	// ÆÐÄ¡È­ÀÏ Àû¿ë
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
//		// version upÀÎ°¡?
//		if (pUpdate->getVersion() > newVersion)
//		{
//			newVersion = pUpdate->getVersion();
//		}
//
//		UpdateProgressBar();
//	}
//
//	//-----------------------------------------------------------------
//	// »õ versionÀúÀå
//	//-----------------------------------------------------------------
//	class ofstream versionFile2(FILE_INFO_VERSION, ios::binary);
//	versionFile2.write((const char*)&newVersion, 4);
//	versionFile2.close();
//	
//	//-----------------------------------------------------------------
//	// ÆÐÄ¡¾ÐÃàÈ­ÀÏ Á¦°Å
//	//-----------------------------------------------------------------
//	remove( packFilename );
//
//	//-----------------------------------------------------------------
//	// ÆÐÄ¡È­ÀÏ Á¦°Å - Updater µð·ºÅä¸® Áö¿ì¸é¼­ µÇ¹Ç·Î .. ½Å°æ ¾È ½áµµ µÈ´Ù.
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
//		// *.spk fileÀ» Ã£´Â´Ù.
//		//-----------------------------------------------------------------
//		if ( (hFile = _findfirst( "Data\\UI\\txt\\Log*.txt", &FileData )) != -1L )
//		{
//			do
//			{
//				sprintf(filename, "Data\\UI\\txt\\%s", FileData.name);
//				
//				class ifstream file( filename, ios::binary );
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
//					sprintf(buffer, "\\\\¾¦°«\\Log\\%s-%s", computerName, FileData.name);
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
		// *.spk fileÀ» Ã£´Â´Ù.
		//-----------------------------------------------------------------
		if ( (hFile = _findfirst( "Log\\Log*.txt", &FileData )) != -1L )
		{
			do
			{
				sprintf(filename, "Log\\%s", FileData.name);
				
				class ifstream file( filename, ios::binary );
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
//						sprintf(buffer, "\\\\¾¦°«\\Log\\%s-%s", computerName, FileData.name);
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

				// 3ºÐ ÈÄ
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
//// ¾ËÆ®Å°°¡ ´­¸°°ÍÀÎÁö....
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
//// Control + ESCµµ ¸·Àð
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
	class ifstream ispkFile("f:\\vcpp\\clientinfo\\Data\\AppendPatch\\New18Creature.ispk", ios::binary);
	ISPK.LoadFromFile(ispkFile);
	ispkFile.close();

	class ofstream ispkFile2("f:\\vcpp\\clientinfo\\Data\\AppendPatch\\New18Creature.ispk", ios::binary);
	class ofstream ispkFile3("f:\\vcpp\\clientinfo\\Data\\AppendPatch\\New18Creature.ispki", ios::binary);
	ISPK.SaveToFile(ispkFile2, ispkFile3);
	ispkFile2.close();
	ispkFile3.close();
	*/
		
	
	#ifdef OUTPUT_DEBUG
		InitializeCriticalSection(&g_Lock);
	#endif	

	
	//----------------------------------------------------------
	// ÇÏ³ªÀÇ application¸¸ ½ÇÇà½ÃÅ²´Ù.
	//----------------------------------------------------------
	// °ª ´ëÃæ ³Ö±â.. --;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = FALSE;


#ifndef OUTPUT_DEBUG
	// 2006.11.07 edit Coffee  ÐÞÕýÎª³ÌÐò¿ÉÒÔË«¿ª
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
	
		// ÇöÀç directory¸¦ ÀúÀåÇØµÐ´Ù.
//	strcpy(g_CWD, __argv[0]);
 	GetModuleFileName(NULL, g_CWD, _MAX_PATH);
	char *tempCut = strrchr(g_CWD, '\\');
	if(tempCut == NULL)
		return FALSE;

	*tempCut = '\0';

//	GetCurrentDirectory( _MAX_PATH, g_CWD );

	
	SetCurrentDirectory(g_CWD);
	
// 	//	// »õ·Î¿î updater½ÇÇàÈ­ÀÏÀÌ Á¸ÀçÇÏ¸é..
//	if (_access(UPDATER_NEW_FILENAME, 0) == 0/* && _access(UPDATER_FILENAME, 0)*/)//updaterNewFile)
//	{	
//		// ¿¹Àü Patcher¸¦ Áö¿ì°í..
//		if (remove( UPDATER_FILENAME )==0)
//		{
//			// »õ°ÍÀ» ¿¹Àü°É·Î rename
//			if (rename( UPDATER_NEW_FILENAME, UPDATER_FILENAME )==0)
//			{
//				DEBUG_ADD("Update Updater.exe OK"); 
//			}
//		}
////		// ¿¹Àü updater¸¦ Áö¿ì°í..
////		//remove( UPDATER_FILENAME );
////		// »õ°ÍÀ» ¿¹Àü°É·Î rename
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

	// ÇÑ Client´Â µ¿½Ã¿¡ ´Ù¸¥ ÇÑ Client¿Í Åë½ÅÇÑ´Ù.
	// ¿©·¯±ºµ¥ÀÇ resource(±æµå¸¶Å©, profile...)°¡ ÇÊ¿äÇÑ °æ¿ì 
	//		ResourceQueue¿¡ ÀúÀåÇß´Ù°¡ Æ´Æ´È÷ ¹Þµµ·Ï ÇÑ´Ù.
	// while(1)ºÎºÐÀº ´Ù¸¥ thread·Î »©¾ßµÈ´Ù.
	// RequestManager Á¢±ÙÇÒ¶§´Â mutex¾²µçÁö ÇØ¾ßµÈ´Ù.
	// RequestManagerÀÇ Update()´Â main thread¿¡¼­ ÇÑ´Ù.
	//
	ServerSocket* pServerSocket = new ServerSocket( 9650 );

	char strClient[256];
	uint port;

	while (1)
	{
		Socket* pSocket = pServerSocket->accept();

		// request¿¡ µî·Ï
		RequestServerPlayer* pRequestServerPlayer = new RequestServerPlayer( pSocket );
		g_pRequestManager->AddPlayer( pRequestServerPlayer );

		// g_pRequestManager´Â µî·ÏµÈ RequestServerPlayer¿¡ ´ëÇØ¼­ processInput/Command/Output Ã³¸®

		strcpy(strClient, pSocket->getHost().c_str());
		port = pSocket->getPort();
	}
	*/

	/*
	class ifstream file("ImageObject.spk", ios::binary);
	class ofstream file2("ImageObject-2.spk", ios::binary);
	class ofstream file2index("ImageObject-2.spki", ios::binary);
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
	class ifstream file("f:\\vcpp\\clientexpo\\data\\image\\creatureshadow.cfpk", ios::binary);
	CCreatureFramePack CFPK;
	CFPK.LoadFromFile(file);
	file.close();

	CCreatureFramePack CFPK2;
	CFPK2.Init( CFPK.GetSize()-3 );

	for (int i=0; i<CFPK2.GetSize(); i++)
	{
		CFPK2[i] = CFPK[i];
	}

	class ofstream file2("f:\\CreatureShadow.cfpk", ios::binary);
	class ofstream fileindex2("f:\\CreatureShadow.cfpki", ios::binary);
	CFPK2.SaveToFile(file2, fileindex2);
	file2.close();
	fileindex2.close();
	*/
	
	//-----------------------------------------------------------------------
	// Client ½ÃÀÛ..
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
	class ifstream	fileSPK;//(FILE_ISPRITE_ADDON, ios::binary);
	if (!FileOpenBinary(FILE_SPRITE_TILE, fileSPK))
		return false;
	tileSPK.LoadFromFile(fileSPK);
	fileSPK.close();
	*/

	g_pFileDef = new Properties;
	g_pFileDef->load(FILE_INFO_FILEDEF);			
	
	// ³Ý¸¶ºí¿ë
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
	// Server IP°¡ ¼³Á¤µÇ¾î ÀÖÁö ¾ÊÀº °æ¿ì --> Launcher½ÇÇà
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
			//::MessageBox(0,"¶ÁÈ¡ÏµÍ³DLL³ö´í£¬´íÎóID£º8001,Çë¼°Ê±ÏòÎÒÃÇÌá½»´íÎó¡£","´íÎó",MB_OK);
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
	// Server IP°¡ ¼³Á¤µÇ¾î ÀÖ´Â °æ¿ì
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
			//::MessageBox(0,"¶ÁÈ¡ÏµÍ³DLL³ö´í£¬´íÎóID£º8002,Çë¼°Ê±ÏòÎÒÃÇÌá½»´íÎó¡£","´íÎó",MB_OK);
			_spawnl(_P_OVERLAY, UPDATER_FILENAME, UPDATER_FILENAME, lpCmdLine, NULL);
//			ShellExecute(g_hWnd, NULL, UPDATER_FILENAME, lpCmdLine, NULL, SW_SHOW);

			return FALSE;
		}

		bool bCheck = true;
		bool bRunUpdater = false;
		//char checkStr[9] = "NEWSTART";
		//char checkStr[9] = "START";

		//add by sonic 2006.4.9  ÉèÖÃ¼ÓÃÜº¯Êý
		DWORD GetCommand;
		//GetCommand=EXECryptor_GetHardwareID();
		GetCommand = 0x00000000;  //ÓÃÓÚµ÷ÊÔ
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
			//´«Èë²ÎÊý´íÎó///
			return false;
			bRunUpdater = true;
			//break;
		}
		//////////////////////////////////////////////////////////////////////////
		/* Delete by Sonic 2006-6-21
		for (int i=0; i<strlen(checkStr); i++)
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
	//Ôö¼Ó¹ÜµÀ¼ì²â
		/*
	if (GetSystem())
	{
		HANDLE hClient = CreateFile("\\\\.\\pipe\\¡¡¡¡¡¡\\",GENERIC_WRITE |	GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hClient == INVALID_HANDLE_VALUE)
		{
			//MessageBox(0,"Error","Error",MB_OK);
			bRunUpdater = true;
			return false;
		}
	}
	*/
		#ifdef OUTPUT_DEBUG
			// Ã¢¸ðµå ½ÃÀÛ..
			g_bTestMode = true;
			char checkTestMode[] = "TestMode";
			for (int i=0; i<strlen(checkTestMode); i++)
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
			//´«Èë²ÎÊý´íÎó///
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

			// ¿©±â
			// [Futec¼öÁ¤]
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
			//::MessageBox(0,"¶ÁÈ¡ÏµÍ³DLL³ö´í£¬´íÎóID£º8003,Çë¼°Ê±ÏòÎÒÃÇÌá½»´íÎó¡£","´íÎó",MB_OK);
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

	// [Futec¼öÁ¤]
	GetFutecAddress(lpCmdLine);



	//------------------------------------------------------------------------
	// Á¤»óÀûÀÎ Patch 
	// °©ÀÚ±â ¾ÐÃàÀ» ÇÏ°Ô µÇ´Â ¹Ù¶÷¿¡... - -;
	//------------------------------------------------------------------------
//	bool bPatched = ApplyPatch();

	//------------------------------------------------------------------------
	// ScreenEffect »ý¼º
	//------------------------------------------------------------------------
//	ConvertScreenEffect();

	//------------------------------------------------------------------------
	// ÀÌÁ¦ºÎÅÍ EffectScreen¿¡ AppendµÇ´Â SPK´Â 
	// ÀÌ ½ÃÁ¡¿¡¼­ AppendÇØ¾ß ÇÑ´Ù.
	// CheckTerriblePatch¿¡ °°ÀÌ ³Ö¾îµµ µÈ´Ù. - -;;
	//------------------------------------------------------------------------
	//AppendScreenEffect();
//add by sonic 2006.4.10 ÅÐ¶Ï³ÌÐòÆô¶¯Ä£Ê½
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
	// Updater.exe¸¦ update ½ÃÅ²´Ù.
	//----------------------------------------------------------------
//	class ifstream updaterNewFile(UPDATER_NEW_FILENAME, ios::binary | ios::nocreate);

	// »õ·Î¿î updater½ÇÇàÈ­ÀÏÀÌ Á¸ÀçÇÏ¸é..
	if (!_access(UPDATER_NEW_FILENAME, 0))//updaterNewFile)
	{	
//		updaterNewFile.close();

		// ¿¹Àü updater¸¦ Áö¿ì°í..
		if (remove( UPDATER_FILENAME )==0)
		{
			// »õ°ÍÀ» ¿¹Àü°É·Î rename
			if (rename( UPDATER_NEW_FILENAME, UPDATER_FILENAME )==0)
			{
				//MessageBox(0,"Error:[rename( UPDATER_NEW_FILENAME, UPDATER_FILENAME )==0]","Error",MB_OK);
				DEBUG_ADD("Update Updater.exe OK"); 
			}
		}
	}	

	//strcpy(g_ServerIP, SERVER_IP);
	//---------------------------------------------------
	// Update µð·ºÅä¸®¸¦ Áö¿öÁØ´Ù.
	//---------------------------------------------------
	// updater¿¡¼­ Áö¿ö¾ß ÇÏ´Âµ¥..»©¸Ô´Â ¹Ù¶÷¿¡.. T_T;;
	//---------------------------------------------------
	char CWD[_MAX_PATH];

	// ÇöÀç pathÀÐ±â
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
			// PATH°¡ Àß¸øµÈ °æ¿ì
			if (errno==ENOENT)
			{				
			}
			// ¹º°¡ ÀÖ¾î¼­ ¾È Áö¿öÁø °æ¿ì..		
			else //if (errno==ENOTEMPTY)
			{
				// ÇöÀç directory¸¦ ±â¾ïÇØµÐ´Ù.				
				if (_chdir( UpdateDir ) == 0)
				{
					//---------------------------------------------------
					// fileÇÏ³ªÇÏ³ª¸¦ Áö¿öÁØ´Ù. T_T;
					//---------------------------------------------------
					struct _finddata_t	FileData;
					long				hFile;

					// ¸ðµç È­ÀÏÀ» ÀÐ¾î¿Â´Ù.
					if( (hFile = _findfirst( "*.*", &FileData )) != -1L )					
					{
						while (_findnext( hFile, &FileData ) == 0)
						{
							// .À¸·Î ½ÃÀÛÇÏ´Â °Ç Áö¿ï ÇÊ¿ä ¾ø´ç..
							if (FileData.name[0] != '.')
							{
								remove( FileData.name );
							}
						}

						// ³¡
						_findclose( hFile );			
					}
					
					_chdir( CWD );

					if (_rmdir( UpdateDir )==0)
					{
						// Àß Áö¿öÁ³´Ù.
					}
				}				
			}
		}
	}

	
	RemoveProgressBar();

	//-----------------------------------------------------------------
	// ÆÐÄ¡ ³»¿ë Âï¾îÁØ´Ù.
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
				//MessageBox(NULL, "³Ý¸¶ºí¿ë ´ÙÅ©¿¡µ§Àº ³Ý¸¶ºí È¨ÆäÀÌÁö¸¦ ÅëÇØ¼­¸¸ ½ÇÇàÀÌ °¡´ÉÇÕ´Ï´Ù.", PROGRAM_TITLE, MB_OK);
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
//			MessageBox(NULL, "À¥ ÆäÀÌÁö ¿¡¼­ ´Ù½Ã ½ÇÇàÇØÁÖ½Ã±â ¹Ù¶ø´Ï´Ù.", PROGRAM_TITLE, MB_OK);
//			ShellExecute(NULL, NULL, "www.darkeden.com", NULL, NULL, SW_SHOW);
//			return -1;
//
//#endif
		}
	}
	//Ôö¼Ó¼ì²â
/* Add by sonic 2006.9.14 Ôö¼Ó¶ÔWPE ¼ì²â */
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
		
		// ³Ý¸¶ºí¿ë
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
		// ÃÊ±âÈ­ µÇÁö ¾Ê¾ÒÀ¸¸é ÃÊ±âÈ­ÇÑ´Ù.
		//------------------------------
		if (!g_pTopView->IsInit())
		{
			g_pTopView->Init();			
		
		}

		g_bActiveApp = TRUE;
		//g_bActiveGame = TRUE;
		DEBUG_CMD(MIN_CLRSCR, "½ÃÀÛ");
		DEBUG_CMD(MIN_SHOWWND, "------------------------------");
		

		while (TRUE)
		{
//			Sleep(1);	//add by viva
			/* add by sonic Ôö¼Ó¶Ôsock¼ì²â */
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
						// CDirectDraw°¡ ÀÛµ¿ÁßÀÌÁö ¾ÊÀ» °æ¿ì¿¡´Â return
						//if (g_bActiveApp)// && CDirectDraw::IsActive())
						if (g_pUpdate!=NULL)
						{
							// ³ëÆÄ½É.. À¸Èì.. --;;
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

							// 15 fps ÀÌ»ó
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
	// Å¸ÀÌ¸Ó ÀÌ»óÀ¸·Î Á¾·áÇÏ´Â °æ¿ì
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
				g_pUIDialog->PopupFreeMessageDlg( "¸ÅÅ©·Î³ª ¹ÙÀÌ·¯½º°¡ ¹ß°ßµÇ¾î ´ÙÅ©¿¡µ§ÀÌ Á¾·áµË´Ï´Ù. ¹ÙÀÌ·¯½º °Ë»ç¸¦ ÇØº¸½Ã°í, Á¤»óÀûÀ¸·Î °ÔÀÓ ÁøÇàÀ» ÇÏ´Âµ¥ ÀÌ·¯ÇÑ ¹®Á¦°¡ ¶Ç ¹ß»ýµÈ´Ù¸é bug@darkeden.comÀ¸·Î ¸ÞÀÏÀ» º¸³»ÁÖ¼¼¿ä.", -1, -1, 0 );
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
					wsprintf(szTemp,"nProtectÀÇ ¾Ë ¼ö ¾ø´Â ¿À·ù ¿¡·¯ÄÚµå[%d][%d]ÀÔ´Ï´Ù. ´ÙÅ©¿¡µ§À» Á¾·áÇÕ´Ï´Ù.",g_nProtectErrorMessage, g_nProtectErrorMessage2);
					g_pUIDialog->PopupFreeMessageDlg( szTemp );
				}
			}
#endif
				
//			if(g_bForceExitBynProtect2)
//			{
//				g_pUIDialog->PopupFreeMessageDlg( "nProtect °ü·Ã ÆÄÀÏ ÃÊ±âÈ­ ¿¡·¯°¡ ¹ß»ýÇß½À´Ï´Ù. °ü¸®ÀÚ °èÁ¤À¸·Î ·Î±×ÀÎÇÏ¿© °ÔÀÓÀ» ½ÇÇàÇÏ½Ã±â ¹Ù¶ø´Ï´Ù.", -1, -1, 0 );
//			}else
//			{
//				if(g_bForceExitBynProtect)
//					g_pUIDialog->PopupFreeMessageDlg( "¹ÙÀÌ·¯½º ¹× ÇØÅ· Â÷´Ü±â¿¡ ÀÇÇØ ´ÙÅ©¿¡µ§ÀÌ Á¾·áµË´Ï´Ù. ¹ÙÀÌ·¯½º °Ë»ç¸¦ ÇØº¸½Ã°í, Á¤»óÀûÀ¸·Î °ÔÀÓ ÁøÇàÀ» ÇÏ´Âµ¥ ÀÌ·¯ÇÑ ¹®Á¦°¡ ¶Ç ¹ß»ýµÈ´Ù¸é, bug@darkeden.com À¸·Î ¸ÞÀÏÀ» º¸³»ÁÖ¼¼¿ä.", -1, -1, 0 );
//				else
//					g_pUIDialog->PopupFreeMessageDlg( "¸ÅÅ©·Î³ª ¹ÙÀÌ·¯½º°¡ ¹ß°ßµÇ¾î ´ÙÅ©¿¡µ§ÀÌ Á¾·áµË´Ï´Ù. ¹ÙÀÌ·¯½º °Ë»ç¸¦ ÇØº¸½Ã°í, Á¤»óÀûÀ¸·Î °ÔÀÓ ÁøÇàÀ» ÇÏ´Âµ¥ ÀÌ·¯ÇÑ ¹®Á¦°¡ ¶Ç ¹ß»ýµÈ´Ù¸é bug@darkeden.comÀ¸·Î ¸ÞÀÏÀ» º¸³»ÁÖ¼¼¿ä.", -1, -1, 0 );
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
	// ¸ðµç objectµé Á¦°Å
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
	// Updater¸¦ ½ÇÇà½ÃÅ²´Ù.
	//----------------------------------------------------------------
	if (g_bNeedUpdate)
	{
		//_spawnl(_P_NOWAIT, UPDATER_FILENAME, UPDATER_FILENAME, "UPDATE", NULL);
		char szTemp[512];
		sprintf(szTemp, "UPDATE %s", lpCmdLine);
		_chdir( g_CWD );
		//::MessageBox(0,"¶ÁÈ¡ÏµÍ³DLL³ö´í£¬´íÎóID£º8004,Çë¼°Ê±ÏòÎÒÃÇÌá½»´íÎó¡£","´íÎó",MB_OK);
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
				//MessageBox(0,"²Ù×÷ÏµÍ³:WIN95","ÐÅÏ¢",MB_OK);
				//strcpy(sys.chSystem,"²Ù×÷ÏµÍ³:WIN95");
				return FALSE;
			else if(OsInfo.dwMajorVersion == 4)
				//MessageBox(0,"²Ù×÷ÏµÍ³:WIN98","ÐÅÏ¢",MB_OK);
				return FALSE;
				//strcpy(sys.chSystem,"²Ù×÷ÏµÍ³:WIN98");
			break;
		case VER_PLATFORM_WIN32_NT:
			if(OsInfo.dwMajorVersion == 5)
				//MessageBox(0,"²Ù×÷ÏµÍ³:WIN2000","ÐÅÏ¢",MB_OK);
				return TRUE;
				//strcpy(sys.chSystem,"²Ù×÷ÏµÍ³:WIN2000");
			else
				return TRUE;
				//MessageBox(0,"²Ù×÷ÏµÍ³:WINNT","ÐÅÏ¢",MB_OK);
				//strcpy(sys.chSystem,"²Ù×÷ÏµÍ³:WINNT");
			break;
		default:
			return FALSE;
			//strcpy(sys.chSystem,"Î´ÖªÏµÍ³!!");
			break;
		}
		// version and language
	}
	return FALSE;
}
