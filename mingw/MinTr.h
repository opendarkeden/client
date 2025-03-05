// 마지막 업데이트 2003.3.5 19:00

#ifndef ___MIN_TRACE___
#define ___MIN_TRACE___

#include <TCHAR.H>
#include <WTYPES.H>
#include <stdio.h>
/* 
   이 값이 1이면 MinTrace가 적용되고
   0으로 하면 Visual C 의 기본 TRACE가 사용된다.
   MinTrace가 TRACE로 바뀐다는 말이다.
*/

#define MIN_TRACE_ON        1      

//-----------------------------
// 명령어 리스트 (Command List)
//-----------------------------

#define MIN_CLRSCR			1		// 트레이스화면 지우기

#define MIN_TRANSPARENTWND	2		// 트레이스창을 투명하게  CmdMinTrace(MIN_TRANSPARENTWND, "60"); 
									// "60" 부분이 투명도인데 20~100 사이의 값을 주면 된다. 100일경우 완전불투명

#define MIN_HIDEWND			3		// 트레이스창을 사라지게함.
#define MIN_SHOWWND			4		// 트레이스창을 나타나게함.

#define MIN_TIME			5		// TIME은	  "12:34:43"			시간을 출력할때..
#define MIN_TIMELN			6		// DATE는	  "2003/02/34"			날짜를 출력할때..
#define MIN_DATE			7		// DATETIME은 "2003/02/32 12:12:12" 날짜와시간을 모두 출력할때..
#define MIN_DATELN			8
#define MIN_DATETIME		9        
#define MIN_DATETIMELN		10      // 뒤에 LN을 붙이면 LineFeed가 적용된다.
									
									// 예를들면 CmdMinTrace( MIN_TIME, "시간보이냐?" ); 이렇게하면 
                                    // 12:12:12
									// 시간보이냐?              ' 이렇게 출력되고

									// CmdMinTrace( MIN_TIMELN, "시간보이냐?" ); 이렇게하면
									// 12:12:12 시간보이냐?     ' 이렇게 출력된다.

#define MIN_TOPMOST			11		// 트레이스창을 제일 꼭대기로 올린다.
#define MIN_NOTOPMOST		12		// 꼭대기로 올린거 해제한다.

#define MIN_STOP            13      // 트레이스를 중지한다.
#define MIN_RUN             14      // 트레이스를 시작한다.




static LPCTSTR g_pszMinTraceTitle     = _T("MinTrace 2003");
static LPCTSTR g_pszMinTraceClassName = _T("__MinTrace Window__");

#define MIN_UNICOD		0xFFFF0000
#define MIN_NOTUNI		0x00000000
#define MIN_COL         0x1100
#define MIN_CMD         0x2200
#define MIN_ERR			0x0033
#define MIN_WAR			0x0044

//-----------------------------------------------------------------------------
// 설명 : _MinTraceA 
//-----------------------------------------------------------------------------
inline void _MinTraceA(LPCSTR p)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
    if (hWnd)
    {  
        cd.dwData = MIN_NOTUNI;
        cd.cbData = (strlen(p)+1)*sizeof(char);
        cd.lpData = (void *)p; 
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
}

//-----------------------------------------------------------------------------
// 설명 : _MinTraceW (유니코드) 
//-----------------------------------------------------------------------------
inline void _MinTraceW(LPCWSTR p)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
    if (hWnd)
    {  
        cd.dwData = MIN_UNICOD;
        cd.cbData = (wcslen(p)+1)*sizeof(wchar_t);
        cd.lpData = (void *)p; 
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
}

//-----------------------------------------------------------------------------
// 설명 : 일반 트레이스
//-----------------------------------------------------------------------------
inline void _MinTrace(LPCTSTR pFormat, ...)
{
    va_list args;
	va_start(args, pFormat);
		
    _TCHAR buffer [1024*sizeof(_TCHAR)];

#ifdef UNICODE
	vswprintf(buffer, pFormat, args);
    _MinTraceW(buffer);
#else
	vsprintf(buffer, pFormat, args);
    _MinTraceA(buffer);
#endif
	
    va_end(args);
}

//-----------------------------------------------------------------------------
// 설명 : _MinTraceErrA 
//-----------------------------------------------------------------------------
inline void _MinTraceErrA(LPCSTR p)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
    if (hWnd)
    {  
        cd.dwData = MIN_NOTUNI | MIN_ERR;
        cd.cbData = (strlen(p)+1)*sizeof(char);
        cd.lpData = (void *)p; 
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
}

//-----------------------------------------------------------------------------
// 설명 : _MinTraceErrW (유니코드) 
//-----------------------------------------------------------------------------
inline void _MinTraceErrW(LPCWSTR p)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
    if (hWnd)
    {  
        cd.dwData = MIN_UNICOD | MIN_ERR;
        cd.cbData = (wcslen(p)+1)*sizeof(wchar_t);
        cd.lpData = (void *)p; 
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
}

//-----------------------------------------------------------------------------
// 설명 : 에러 트레이스
//-----------------------------------------------------------------------------
inline void _MinTraceErr(LPCTSTR pFormat, ...)
{
    va_list args;
	va_start(args, pFormat);
	
    _TCHAR buffer [1024*sizeof(_TCHAR)];
	
#ifdef UNICODE
	vswprintf(buffer, pFormat, args);
    _MinTraceErrW(buffer);
#else
	vsprintf(buffer, pFormat, args);
    _MinTraceErrA(buffer);
#endif
	
    va_end(args);
}

//-----------------------------------------------------------------------------
// 설명 : _MinTraceWarA 
//-----------------------------------------------------------------------------
inline void _MinTraceWarA(LPCSTR p)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
    if (hWnd)
    {  
        cd.dwData = MIN_NOTUNI | MIN_WAR;
        cd.cbData = (strlen(p)+1)*sizeof(char);
        cd.lpData = (void *)p; 
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
}

//-----------------------------------------------------------------------------
// 설명 : _MinTraceWarW (유니코드) 
//-----------------------------------------------------------------------------
inline void _MinTraceWarW(LPCWSTR p)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
    if (hWnd)
    {  
        cd.dwData = MIN_UNICOD | MIN_WAR;
        cd.cbData = (wcslen(p)+1)*sizeof(wchar_t);
        cd.lpData = (void *)p; 
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
}

//-----------------------------------------------------------------------------
// 설명 : 일반 트레이스
//-----------------------------------------------------------------------------
inline void _MinTraceWar(LPCTSTR pFormat, ...)
{
    va_list args;
	va_start(args, pFormat);
	
    _TCHAR buffer [1024*sizeof(_TCHAR)];
	
#ifdef UNICODE
	vswprintf(buffer, pFormat, args);
    _MinTraceWarW(buffer);
#else
	vsprintf(buffer, pFormat, args);
    _MinTraceWarA(buffer);
#endif
	
    va_end(args);
}

//-----------------------------------------------------------------------------
// 설명 : _ClMinTraceA
//-----------------------------------------------------------------------------
inline void _ClMinTraceA(COLORREF col, LPCSTR p)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
    if (hWnd)
    {  
		char buf[1024];
		sprintf(buf, "%8d,%s", col, p);
        cd.dwData = MIN_NOTUNI | MIN_COL;
        cd.cbData = (strlen(buf)+1)*sizeof(char);
        cd.lpData = (void *)buf;
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
}

//-----------------------------------------------------------------------------
// 설명 : _ClMinTraceW
//-----------------------------------------------------------------------------
inline void _ClMinTraceW(COLORREF col, LPCWSTR p)
{
#ifdef UNICODE
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
    if (hWnd)
    {  
		_TCHAR buf[1024*sizeof(_TCHAR)];
		swprintf(buf, _T("%8d,%s"), col, p);
        cd.dwData = MIN_UNICOD | MIN_COL;
        cd.cbData = (wcslen(buf)+1)*sizeof(_TCHAR);
        cd.lpData = (void *)buf;
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
#endif
}

//-----------------------------------------------------------------------------
// 설명 : 컬러 트레이스
//-----------------------------------------------------------------------------
inline void _ClMinTrace(COLORREF col, LPCTSTR pFormat, ...)
{
    va_list args;
	va_start(args, pFormat);
	
    _TCHAR buffer [1024*sizeof(_TCHAR)];
	
#ifdef UNICODE
	vswprintf(buffer, pFormat, args);
    _ClMinTraceW(col, buffer);
#else
	vsprintf(buffer, pFormat, args);
    _ClMinTraceA(col, buffer);
#endif
	
    va_end(args);
}

//-----------------------------------------------------------------------------
// 설명 : _CmdMinTraceA 
//-----------------------------------------------------------------------------
inline void _CmdMinTraceA(int nCmd, LPCSTR p)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
    if (hWnd)
    {  
		char buf[1024];
		sprintf(buf, "%8d,%s", nCmd, p);
        cd.dwData = MIN_NOTUNI | MIN_CMD;
        cd.cbData = (strlen(buf)+1)*sizeof(char);
        cd.lpData = (void *)&buf;
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
}

//-----------------------------------------------------------------------------
// 설명 : _CmdMinTraceW
//-----------------------------------------------------------------------------
inline void _CmdMinTraceW(int nCmd, LPCWSTR p)
{
#ifdef UNICODE
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_pszMinTraceClassName, g_pszMinTraceTitle); 
    if (hWnd)
    {  
		_TCHAR buf[1024];
		wsprintf(buf, _T("%8d,%s"), nCmd, p);
        cd.dwData = MIN_UNICOD | MIN_CMD;
        cd.cbData = (wcslen(buf)+1)*sizeof(_TCHAR);
        cd.lpData = (void *)&buf;
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
#endif
}

//-----------------------------------------------------------------------------
// 설명 : 명령 트레이스
//-----------------------------------------------------------------------------
inline void _CmdMinTrace(int nCmd, LPCTSTR pFormat, ...)
{
    va_list args;
	va_start(args, pFormat);
	
    _TCHAR buffer [1024*sizeof(_TCHAR)];
	
#ifdef UNICODE
	vswprintf(buffer, pFormat, args);
    _CmdMinTraceW(nCmd, buffer);
#else
	vsprintf(buffer, pFormat, args);
    _CmdMinTraceA(nCmd, buffer);
#endif
	
    va_end(args);
}

#ifdef _DEBUG
  #if MIN_TRACE_ON
    #define MinTrace ::_MinTrace
  #else
    #define MinTrace TRACE
  #endif
#else
  #define MinTrace ((void)0)
#endif

#ifdef _DEBUG
  #if MIN_TRACE_ON
    #define MinTraceErr ::_MinTraceErr
  #else
    #define MinTraceErr TRACE
  #endif
#else
  #define MinTraceErr ((void)0)
#endif

#ifdef _DEBUG
  #if MIN_TRACE_ON
    #define MinTraceWar ::_MinTraceWar
  #else
    #define MinTraceWar TRACE
  #endif
#else
  #define MinTraceWar ((void)0)
#endif

#ifdef _DEBUG
  #if MIN_TRACE_ON
    #define ClMinTrace ::_ClMinTrace
  #else
    #define ClMinTrace ((void)0)
  #endif
#else
  #define ClMinTrace ((void)0)
#endif

#ifdef _DEBUG
  #if MIN_TRACE_ON
    #define CmdMinTrace ::_CmdMinTrace
  #else
    #define CmdMinTrace ((void)0)
  #endif
#else
  #define CmdMinTrace ((void)0)
#endif

#define MinTraceRelease	  	::_MinTrace
#define MinTraceWarRelease	::_MinTraceWar
#define MinTraceErrRelease	::_MinTraceErr 
#define ClMinTraceRelease   ::_ClMinTrace
#define CmdMinTraceRelease  ::_CmdMinTrace

#endif

