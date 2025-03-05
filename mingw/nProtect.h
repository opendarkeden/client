#ifndef __nProtect_HEADER__
#define __nProtect_HEADER__

#include "NPGameLib.h"
#include <map>
#include <string>

enum NPROTECT_STATUS
{
	NPROTECT_NORMAL = 0,
	NPROTECT_EXIT_TWO,
	NPROTECT_FORCE_EXIT,
	NPROTECT_SPEEDHACK,
	NPROTECT_SOFTICE,
	NPROTECT_SH_ERROR,

	NPROTECT_CHECK_ERROR,

	NPROTECT_MAX,
};

extern NPROTECT_STATUS g_nProtectSTATUS;

#define WM_NPROTECT_EXIT_TWO				(WM_USER + 10001)
#define WM_NPROTECT_FORCEEXIT				(WM_USER + 10010)
#define WM_NPROTECT_SPEEDHACK				(WM_USER + 10011)
#define WM_NPROTECT_SH_ERROR				(WM_USER + 10012)
#define WM_NPROTECT_SOFTICE					(WM_USER + 10111)

#define FILE_NPROTECT_DLL				"NPCHK.DLL"

#define NPROTECT_ERROR								0
#define NPROTECT_OK									1
#define NPROTECT_CANNOT_FIND_DLL					2
#define NPROTECT_DLL_ERROR							3
#define NPROTECT_ERROR_CRC_CHECK					4
#define NPROTECT_EXECUTE_ERROR						5
#define NPROTECT_CANNOT_UPDATE						6
#define NPROTECT_EXECUTE_ERROR_EXCEPTION			7
#define NPROTECT_EXECUTE_ERROR_CANNOT_FIND_DLL		8
#define NPROTECT_EXECUTE_ERROR_CANNOT_DOWNLOAD		9
#define NPROTECT_EXECUTE_ERROR_DID_NOT_FIX			10

#define NPROTECT_ERROR_GET_WINDOWS_DIRECTORY		11
#define NPROTECT_ERROR_CANNOT_GET_WINDOWS_VERSION	12
#define NPROTECT_ERROR_LOAD_DLL_GAMEHACK			13

#define NPROTECT_ERROR_NOT_EXIST_CHECK_NPMON		14


extern bool			g_bIsNT,g_bIsStartGameHack;
extern HINSTANCE	g_nProtectInstance;
extern HINSTANCE	g_NPFGMInstance;
extern HINSTANCE	g_nPSICEInstance;
extern std::map<DWORD,std::string> g_nProtectMessage;
extern DWORD			g_nProtectErrorMessage;
extern DWORD			g_nProtectErrorMessage2;
extern bool				g_bForceExitBynProtect;
extern bool				g_bForceExitBynProtect2;

int		CheckNPROTECT(int CurTime = 0);
int		RunNPROTECT();
void	CloseNPROTECT();
void	ReleaseGameHack();
void	EnterGameHack();
bool	LoadLibraryNPFGM();
bool	TimerCheckNPMON(int CurTime = 0);					// 일정 시간이 지난 후부터 체크한다.
void	Set_Auth(DWORD dwVal);

extern	DWORD		g_TimerNPMON;
typedef BOOL (*CheckCRC)(char* FilePath);
typedef BOOL (MYCHECKNPMON)();

extern CNPGameLib npgl;
#endif 
