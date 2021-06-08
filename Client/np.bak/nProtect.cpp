#include "Client_PCH.h"
#ifndef OUTPUT_DEBUG

//#define __OLD_VERSION__
//#define __RUN_GAMEHACK__

//#define __TEST_SERVER__
//#define __REAL_SERVER__

#pragma warning(disable:4786)

#include <windows.h>
#include <process.h>
#include "nProtect.h"
#include "DebugInfo.h"
#include "npfgmsdk.h"	

bool		g_bIsNT = false, g_bIsStartGameHack = false;
HINSTANCE	g_nProtectInstance = NULL;
HINSTANCE	g_NPFGMInstance = NULL;
HINSTANCE	g_nPSICEInstance = NULL;
DWORD		g_OsVersion[2];
MYCHECKNPMON *pCheckNPMON = NULL;

//#ifdef __REAL_SERVER__
	CNPGameLib npgl("DarkEden");
	//CNPGameLib npgl("DarkEdenChina");
//#elif defined(__TEST_SERVER__)
//	CNPGameLib npgl("DarkEdenTest");
//#endif

DWORD	g_delayTime_npmon = 60000;			// 20초 후부터 체크 시작	

extern	HWND				g_hWnd;
extern void Send_nProtect_Auth(DWORD dwVal);
bool	CheckWindowVersion()
{
	OSVERSIONINFO VersionInfo;

	VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
	if( GetVersionEx(&VersionInfo) == FALSE )
	{
		MessageBox(NULL, "지원되지 않는 윈도우 버전입니다.","Execute Error", MB_OK);
		return false;
	}

	if(VersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT )
		g_bIsNT = true;
	else
		g_bIsNT = false;

	g_OsVersion[0] = VersionInfo.dwPlatformId;
	g_OsVersion[1] = VersionInfo.dwMajorVersion;
	
	return true;
}
//
//bool	ChecknProtectCRC(HINSTANCE dll, char* FilePath)
//{
//	CheckCRC		MyCheckCRC;
//		
//	DEBUG_ADD_FORMAT("[nProtect] CheckCRC %s",FilePath);
//	MyCheckCRC = (CheckCRC)GetProcAddress( dll, "CheckCRC" );
//	return MyCheckCRC( (LPTSTR)(LPCTSTR) FilePath ) != FALSE;
//}

int		CheckNPROTECT(int CurTime)
{
#ifdef __OLD_VERSION__
	static DWORD time = 0;
	HWND hWnd;
	
	time++;
	if(time >= 512)
		time = 0;
	
	if(time == 4)
	{
		hWnd = FindWindow("TfrmNPMON", "WWW.NPROTECT.COM");	
		
		if( hWnd != 0 )
			return NPROTECT_OK;
		
		DEBUG_ADD("[nProtect] Cannot Find nProtect");
		return NPROTECT_ERROR;
	} //else
#endif
	
//	if(time == 15)
//	if( TimerCheckNPMON() )
//	{
//		BOOL Result = pCheckNPMON();
//
//		if(!Result)
//			return NPROTECT_ERROR;
//
//		return NPROTECT_OK;
//		
//#ifdef __RUN_GAMEHACK__
//		if ( !pnPFindSICE())
//			return NPROTECT_OK;
//		
//		DEBUG_ADD("[nProtect] Find SoftICE");
//		return NPROTECT_ERROR;
//#endif
//	}
	return NPROTECT_OK;
}

int		RunNPROTECT()
{	
#ifdef __OLD_VERSION__
	int		ExecuteResult;


	g_nProtectMessage[NPROTECT_NORMAL]= "알 수 없는 오류 입니다. 다크에덴을 종료합니다.";
	g_nProtectMessage[NPROTECT_EXIT_TWO] = "nProtect 에 의해 다크에덴이 종료됩니다.(1)";
	g_nProtectMessage[NPROTECT_FORCE_EXIT] = "nProtect 에 의해 다크에덴이 종료됩니다.(2)";
	g_nProtectMessage[NPROTECT_SPEEDHACK] = "스피드핵이 발견되어 다크에덴이 종료됩니다.";
	g_nProtectMessage[NPROTECT_SOFTICE] = "게임핵이 발견되어 다크에덴이 종료됩니다.";
	g_nProtectMessage[NPROTECT_SH_ERROR] ="nProtect 에 의해 다크에덴이 종료됩니다(SHERROR)";
	g_nProtectMessage[NPROTECT_CHECK_ERROR] = "nProtect Check 에러에 의해 다크에덴이 종료됩니다.";	
	
	DEBUG_ADD("[nProtect] Execute nProtect");
	ExecuteResult = spawnl(P_WAIT, "findhack.exe","findhack.exe",NULL);
	
	switch(ExecuteResult)
	{
	case 1024 :				// 메모리상에 해킹툴이 존재하지 않을 경우
		DEBUG_ADD("[nProtect] Cannot Find Hacking program");
		break;
	case 1025 :				// 메모리상에 해킹툴이 존재하나 정상적으로 치료를 했을 경우
		DEBUG_ADD("[nProtect] Find Hacking program. - fixed it.");
		break;
	case 1026 :				// 메모리상의 해킹툴을 감지했으나 사용자가 치료를 선택하지 않거나 프로그램에서 정상적으로 치료를 하지 못했을 경우
		DEBUG_ADD("[nProtect] did not Fix Hacking program.");		
		return NPROTECT_EXECUTE_ERROR_DID_NOT_FIX;		
	case 1027 :				// 해킹툴 진단 프로그램이 정상적으로 다운로드 되지 않았을 경우(URL이 잘못되었거나 서버가 정상적으로 동작하지 않을 경우)
		DEBUG_ADD("[nProtect] Can not download check program");
		return NPROTECT_EXECUTE_ERROR_CANNOT_DOWNLOAD;		
	case 1028 :				// NPX.DLL 등록 에러 및 nProtect 구동에 필요한 파일이 없을 경우
		DEBUG_ADD("[nProtect] Cannot Find DLL File");
		return NPROTECT_EXECUTE_ERROR_CANNOT_FIND_DLL;
	case 1029 :				// 프로그램내에서 예외사항이 발생했을 경우
		DEBUG_ADD("[nProtect] Occured Exception in nProtect");
		return NPROTECT_EXECUTE_ERROR_EXCEPTION;
	case 1030 :				// 사용자가 종료 버튼을 클랙했을 경우의 처리값
		DEBUG_ADD("[nProtect] Quit nProtect by user");
		return NPROTECT_EXECUTE_ERROR;
	case 1031 :				// 업데이트 서버 접속을 실패한 경우
		DEBUG_ADD("[nProtect] Cannot connect updateserver");
		return NPROTECT_CANNOT_UPDATE;
	}

	DEBUG_ADD("[nProtect] Ok");
	
	if( ( g_nProtectInstance = LoadLibrary(FILE_NPROTECT_DLL)) == NULL )
		return NPROTECT_CANNOT_FIND_DLL;	
/*
	char szBuffer[256] = {0,};
	if( GetSystemDirectory( szBuffer, 256) == 0)
		return NPROTECT_ERROR_GET_WINDOWS_DIRECTORY;
	strcat( szBuffer, "\\NPMON.EXE");	
	bool	Result;
	Result = ChecknProtectCRC(g_nProtectInstance, "FindHack.exe");
	Result |=ChecknProtectCRC(g_nProtectInstance, szBuffer );
	
	if(!Result)
	{
		FreeLibrary( g_nProtectInstance );
		g_nProtectInstance = NULL;
		return NPROTECT_ERROR_CRC_CHECK;	
	}
*/
	if( (pCheckNPMON = (MYCHECKNPMON *)GetProcAddress(g_nProtectInstance, "CheckNPMON")) == NULL)
		return NPROTECT_ERROR_NOT_EXIST_CHECK_NPMON;	
	
#ifdef __RUN_GAMEHACK__
	if( LoadLibraryNPFGM() == false )
		return NPROTECT_ERROR_LOAD_DLL_GAMEHACK;

	EnterGameHack();
#endif


#endif // __OLD_VERSION__

#ifndef __OLD_VERSION__
//	g_nProtectMessage[NPGAMEMON_COMM_ERROR] = "[NPGAMEMON_COMM_ERROR] nProtect와의 통신에 에러가 발생하여 다크에덴을 종료합니다.";
//	g_nProtectMessage[NPGAMEMON_COMM_CLOSE] = "[NPGAMEMON_COMM_CLOSE] nProtect와의 통신이 닫혀 다크에덴을 종료합니다.";
//	g_nProtectMessage[NPGAMEMON_INIT_ERROR] = "[NPGAMEMON_INIT_ERROR] 초기화 에러입니다. 다크에덴을 종료합니다.";
//	g_nProtectMessage[NPGAMEMON_SPEEDHACK] = "[NPGAMEMON_SPEEDHACK] 스피드핵이 발견되어 다크에덴을 종료합니다.";
//	g_nProtectMessage[NPGAMEMON_GAMEHACK_KILLED] = "[NPGAMEMON_GAMEHACK_KILLED] 게임핵이 발견되었습니다. 다크에덴을 종료합니다.";
//	g_nProtectMessage[NPGAMEMON_GAMEHACK_DETECT] = "[NPGAMEMON_GAMEHACK_DETECT] 게임핵이 감지되어 다크에덴을 종료합니다.";
//	g_nProtectMessage[NPGAMEMON_UNDEFINED] = "[NPGAMEMON_UNDEFINED] nProtect 관련 문제가 발생하여 다크에덴이 종료됩니다.";

	g_nProtectMessage[NPGAMEMON_COMM_ERROR] = "[NPGAMEMON_COMM_ERROR] ";
	g_nProtectMessage[NPGAMEMON_COMM_CLOSE] = "[NPGAMEMON_COMM_CLOSE] ";
	g_nProtectMessage[NPGAMEMON_INIT_ERROR] = "[NPGAMEMON_INIT_ERROR] ";
	g_nProtectMessage[NPGAMEMON_SPEEDHACK] = "[NPGAMEMON_SPEEDHACK] ";
	g_nProtectMessage[NPGAMEMON_GAMEHACK_KILLED] = "[NPGAMEMON_GAMEHACK_KILLED] ";
	g_nProtectMessage[NPGAMEMON_GAMEHACK_DETECT] = "[NPGAMEMON_GAMEHACK_DETECT] ";
	g_nProtectMessage[NPGAMEMON_UNDEFINED] = "[NPGAMEMON_UNDEFINED] nProtect ";


//#ifdef __REAL_SERVER__
//	DWORD dwResult = npgl.Init(g_hWnd,"DarkEden");
//#elif defined(__TEST_SERVER__)
//#endif
	DWORD dwResult = npgl.Init(g_hWnd);
	
	if (dwResult != NPGAMEMON_SUCCESS)
    {
		// 적절한 종료 메시지 출력
		TCHAR msg[512];
		
		switch( dwResult )
		{
/*		case 110 :
			wsprintf(msg, "[110] 이미 nProtect가 실행되어 있습니다.\r\n다시 DarkEden을 실행해 주세요.");
			break;		
		case 112 :
			wsprintf(msg, "[112] 바이러스 및 해킹 툴 검사 모듈 로딩에 실패했습니다.\r\n바이러스에 감염되어 있을 수도 있으니 최신 백신으로 검사해보시기 바랍니다.");
			break;
		case 122 :
			wsprintf(msg, "[122] nProtect 의 GameMon 파일이 정상적이지 않습니다.");
			break;
		case 140 :
			wsprintf(msg, "[140] nProtect를 Update 할 수 없습니다.\r\n현재 인터넷 연결이 올바른지 확인하여 주시고\r\n그래도 문제가 없으면 운영팀에 문의해주세요.");
			break;		
		case 120 :
		case 150 :
		case 180 :
			{
				char szBuffer[256];
				GetCurrentDirectory(255, szBuffer );
				wsprintf(msg, "ErrorCode[%d] 에러 입니다.\r\nnProtect를 실행하는데 필요한 파일을 찾을수 없습니다.\r\n다크에덴홈페이지(http://www.darkeden.com)에서 직접 다운로드받아\r\n%s 폴더에 압축을 풀면 해결될 수 있습니다.",dwResult,szBuffer);
			}
			break;
		case 151 :
			wsprintf(msg, "[151] 게임가드 업데이트 모듈이 없거나 변조되었습니다. 바이러스 검사를 해보신 후\r\n게임가드 셋업파일을 다운받아 게임 가드 폴더에 다시 설치 해주세요.");
			break;
		case 210 :			
		case 220 :
		case 230 :
			CheckWindowVersion();
			wsprintf(msg, "[%d][%d/%d] nProtect GameGuard 초기화 에러 입니다.\r\n게임을 실행할 수 없습니다.\r\nDarkEden폴더의 GameGuard폴더에 있는 파일들을 압축하여\r\nnprotect_help@inca.co.kr로 보내주세요.", dwResult,g_OsVersion[0],g_OsVersion[1]);
			break;
		case 340 :
			wsprintf(msg, "[340] nProtect 의 다운로드에 실패하였습니다. \r\n네트워크 상태가 좋지 않거나, 인터넷 설정에 문제가 있을 수 있습니다.");
			break;
		case 360 :
			wsprintf(msg, "[360] 바이러스가 있어서 게임을 실행할 수 없습니다.\r\n최신 백신으로 바이러스를 치료하신후에 게임을\r\n실행해주세요.");
			break;
		case NPGMUP_ERROR_DECRYPT :
			wsprintf(msg, "[370] nProtect Decrypt Error");			
			break;
		case 380 :
			wsprintf(msg, "[380] 게임가드 업데이트 서버 접속에 실패하였습니다.\r\n잠시 후 다시 시도해주세요.");			
			break;
		case 390 :
			wsprintf(msg, "[390] INI 파일에 문제가 있습니다.");
			break;
		default :
			wsprintf(msg, "nProtect GameGuard 실행 에러 : %lu", dwResult);
			break;
		case 110 :
			wsprintf(msg, "[110] 이미 nProtect가 실행되어 있습니다.\r\n다시 DarkEden을 실행해 주세요.");
			break;		
		case 112 :
			wsprintf(msg, "[112] 바이러스 및 해킹 툴 검사 모듈 로딩에 실패했습니다.\r\n바이러스에 감염되어 있을 수도 있으니 최신 백신으로 검사해보시기 바랍니다.");
			break;
		case 122 :
			wsprintf(msg, "[122] nProtect 의 GameMon 파일이 정상적이지 않습니다.");
			break;
		case 140 :
			wsprintf(msg, "[140] nProtect를 Update 할 수 없습니다.\r\n현재 인터넷 연결이 올바른지 확인하여 주시고\r\n그래도 문제가 없으면 운영팀에 문의해주세요.");
			break;		
		case 120 :
		case 150 :
		case 180 :
			{
				char szBuffer[256];
				GetCurrentDirectory(255, szBuffer );
				wsprintf(msg, "ErrorCode[%d] 에러 입니다.\r\nnProtect를 실행하는데 필요한 파일을 찾을수 없습니다.\r\n다크에덴홈페이지(http://www.darkeden.com)에서 직접 다운로드받아\r\n%s 폴더에 압축을 풀면 해결될 수 있습니다.",dwResult,szBuffer);
			}
			break;
		case 151 :
			wsprintf(msg, "[151] 게임가드 업데이트 모듈이 없거나 변조되었습니다. 바이러스 검사를 해보신 후\r\n게임가드 셋업파일을 다운받아 게임 가드 폴더에 다시 설치 해주세요.");
			break;
		case 210 :			
		case 220 :
		case 230 :
			CheckWindowVersion();
			wsprintf(msg, "[%d][%d/%d] nProtect GameGuard 초기화 에러 입니다.\r\n게임을 실행할 수 없습니다.\r\nDarkEden폴더의 GameGuard폴더에 있는 파일들을 압축하여\r\nnprotect_help@inca.co.kr로 보내주세요.", dwResult,g_OsVersion[0],g_OsVersion[1]);
			break;
		case 340 :
			wsprintf(msg, "[340] nProtect 의 다운로드에 실패하였습니다. \r\n네트워크 상태가 좋지 않거나, 인터넷 설정에 문제가 있을 수 있습니다.");
			break;
		case 360 :
			wsprintf(msg, "[360] 바이러스가 있어서 게임을 실행할 수 없습니다.\r\n최신 백신으로 바이러스를 치료하신후에 게임을\r\n실행해주세요.");
			break;
		case NPGMUP_ERROR_DECRYPT :
			wsprintf(msg, "[370] nProtect Decrypt Error");			
			break;
		case 380 :
			wsprintf(msg, "[380] 게임가드 업데이트 서버 접속에 실패하였습니다.\r\n잠시 후 다시 시도해주세요.");			
			break;
		case 390 :
			wsprintf(msg, "[390] INI 파일에 문제가 있습니다.");
			break;
		default :
			wsprintf(msg, "nProtect GameGuard 실행 에러 : %lu", dwResult);
			break;
*/
		default :
			wsprintf(msg, "nProtect GameGuard Error : %lu", dwResult);
			break;
		} 
		MessageBox(g_hWnd, msg, "Error", MB_OK|MB_ICONERROR);
		return NPROTECT_ERROR;
    }	
#endif
	
	return NPROTECT_OK;
}

void	CloseNPROTECT()
{
#ifdef __OLD_VERSION__
	HWND nProtecthWnd;

	nProtecthWnd = FindWindow( NULL, "WWW.NPROTECT.COM" );

#ifdef __RUN_GAMEHACK__
	ReleaseGameHack();
#endif
	
	if(nProtecthWnd != 0)
	{
		DEBUG_ADD("[nProtect] Close nProtect");
		SendMessage( nProtecthWnd, WM_NPROTECT_FORCEEXIT, NULL, NULL );
	}
	if(g_nProtectInstance != NULL)
	{
		FreeLibrary( g_nProtectInstance );
		g_nProtectInstance = NULL;
	}
	if(g_NPFGMInstance != NULL)
	{
		FreeLibrary( g_NPFGMInstance );
		g_NPFGMInstance = NULL;
	}
#ifdef __RUN_GAMEHACK__
	if(g_nPSICEInstance != NULL)
	{
		FreeLibrary( g_nPSICEInstance );
		g_nPSICEInstance = NULL;
	}
#endif

#endif // __OLD_VERSION__
//	npgl.Close();
}

//bool	LoadLibraryNPFGM()
//{
//	if( CheckWindowVersion() == false)
//		return false;
//
//#ifdef __RUN_GAMEHACK__
//
//	if(g_bIsNT)
//		g_NPFGMInstance = LoadLibrary( NPFGM_NT );
//	else
//		g_NPFGMInstance = LoadLibrary( NPFGM_9X );
//
//	g_nPSICEInstance = LoadLibrary( NPFGM_NT );
//#endif
//	
//	if(g_NPFGMInstance == NULL 
//#ifdef __RUN_GAMEHACK__
//		|| g_nPSICEInstance == NULL
//#endif
//		)
//		return false;
//	
//	if( ((pEnterProtect = (EnterProtect) GetProcAddress(g_NPFGMInstance, "EnterProtect")) == NULL) ||
//		((pReleaseProtect = (ReleaseProtect) GetProcAddress( g_NPFGMInstance, "ReleaseProtect")) == NULL)
//#ifdef __RUN_GAMEHACK__
//		||((pnPFindSICE = (nPFindSICE) GetProcAddress( g_nPSICEInstance, "nPFindSICE")) == NULL)
//#endif
//		)
//		return false;
//	else
//		return true;	
//}
//
//void	EnterGameHack()
//{
//#ifdef __RUN_GAMEHACK__
//	if(g_bIsStartGameHack == true)
//		return;
//
//	pEnterProtect();
//	g_bIsStartGameHack = true;
//#endif
//
//}
//
//void	ReleaseGameHack()
//{
//#ifdef __RUN_GAMEHACK__
//	if(g_bIsStartGameHack == false)
//		return;
//
//	pReleaseProtect();
//	g_bIsStartGameHack = false;
//#endif
//}

bool	TimerCheckNPMON(int CurTime)
{
	DWORD		nCurrentTime;
	if(CurTime)
		nCurrentTime = CurTime;
	else
		nCurrentTime = timeGetTime();
	
	if( (nCurrentTime - g_TimerNPMON) > g_delayTime_npmon )
	{
		g_TimerNPMON = nCurrentTime;
		g_delayTime_npmon = 30000;
		return true;
	}
	return false;
}



bool CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
//	TCHAR msg[128];

	g_nProtectErrorMessage = dwMsg;	
	g_nProtectErrorMessage2 = dwArg;
	switch (dwMsg)
    {		
	case NPGAMEMON_COMM_ERROR:
	case NPGAMEMON_COMM_CLOSE:		
		g_bForceExitBynProtect = true;
		return false; // 반드시 종료
		
	case NPGAMEMON_INIT_ERROR:
//		wsprintf(msg, "nProtect GameGuard 초기화 에러 : %lu", dwArg);
//		MessageBox(hWnd, msg, "Error", MB_OK);
		g_bForceExitBynProtect = true;
		return false; // 반드시 종료
		
	case NPGAMEMON_SPEEDHACK:
//		MessageBox(hWnd, "스피드핵이 감지되었습니다.", "Warning", MB_OK);
		g_bForceExitBynProtect = true;
		return false; // 종료
		
	case NPGAMEMON_GAMEHACK_KILLED:
//		MessageBox(hWnd, "게임핵이 발견되었습니다.", "Warning", MB_OK);
		return true; // 계속 진행
		
	case NPGAMEMON_GAMEHACK_DETECT:
//		MessageBox(hWnd, "게임핵이 감지되었습니다.", "Warning", MB_OK);
		g_bForceExitBynProtect = true;
		return false; // 종료
	case NPGAMEMON_CHECK_CSAUTH:
		//MessageBox(g_hWnd, "NPGAMEMON_CHECK_CSAUTH받음", "Warning", MB_OK);
		Send_nProtect_Auth(dwArg);
		return true;
	}	
	return true;
}
void Set_Auth(DWORD dwVal)
{
	npgl.Auth(dwVal);
}

#endif