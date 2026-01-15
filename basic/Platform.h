/*-----------------------------------------------------------------------------

	Platform.h

	Cross-platform abstraction layer for Dark Eden client.
	Provides unified API for Windows, Linux, and macOS.

	Original Windows API dependencies are abstracted here.

	2025.01.14

-----------------------------------------------------------------------------*/

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Platform Detection
 * ============================================================================ */

/* Detect platform */
#if defined(_WIN32) || defined(_WIN64)
	#define PLATFORM_WINDOWS
#elif defined(__linux__)
	#define PLATFORM_LINUX
#elif defined(__APPLE__)
	#include <TargetConditionals.h>
	#if TARGET_OS_MAC
		#define PLATFORM_MACOS
	#endif
#else
	#define PLATFORM_UNKNOWN
#endif

/* ============================================================================
 * Platform Selection
 * ============================================================================ */

/* Force SDL backend on non-Windows platforms */
#ifndef PLATFORM_WINDOWS
	#ifndef PLATFORM_USE_SDL
		#define PLATFORM_USE_SDL
	#endif
#endif

/* Allow explicit SDL selection on Windows */
#ifdef PLATFORM_USE_SDL
	#include <SDL.h>
#endif

/* ============================================================================
 * Basic Type Definitions (from Typedef.h)
 * ============================================================================ */

#ifndef NULL
	#define NULL 0
#endif

#define NOT_SELECTED						-1

/* Type definitions (same as original Typedef.h) */
typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned int	UINT;
typedef unsigned long   DWORD;

/* Color type definitions */
typedef DWORD			COLORREF;
#define RGB(r,g,b)		((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

/* id_t conflicts with POSIX on macOS/Linux, only define on Windows */
#ifdef PLATFORM_WINDOWS
typedef DWORD			id_t;
#endif

typedef WORD			char_t;

/* ============================================================================
 * Common Windows Type Definitions (for cross-platform compatibility)
 * ============================================================================ */

#ifndef PLATFORM_WINDOWS
	/* Windows-compatible type definitions for non-Windows platforms */
	typedef int				BOOL;
	#ifndef TRUE
		#define TRUE	1
	#endif
	#ifndef FALSE
		#define FALSE	0
	#endif

	typedef long			HRESULT;
	#ifndef S_OK
		#define S_OK		0
	#endif
	#ifndef S_FALSE
		#define S_FALSE		1
	#endif

	/* HRESULT macros */
	#ifndef SUCCEEDED
		#define SUCCEEDED(hr)	(((HRESULT)(hr)) >= 0)
	#endif
	#ifndef FAILED
		#define FAILED(hr)		(((HRESULT)(hr)) < 0)
	#endif

	/* VOID type */
	#ifndef VOID
		typedef void		VOID;
	#endif

	/* Additional Windows types */
	typedef long			LONG;
	typedef void*			LPVOID;
	typedef void*			HWND;
	typedef void*			HINSTANCE;
	typedef void*			HANDLE;
	typedef DWORD			LPDWORD;
	typedef const char*		LPCSTR;
	typedef char*			LPSTR;
	typedef const char*		LPCTSTR;
	typedef char*			LPTSTR;
	typedef const wchar_t*	LPCWSTR;
	typedef wchar_t*		LPWSTR;
	typedef unsigned char*	LPBYTE;
	typedef intptr_t		LPARAM;
	typedef intptr_t		WPARAM;
	typedef unsigned int		UINT;

	/* MessageBox constants */
	#define MB_OK			0x00000000L

	/* Character type macros */
	#ifndef _T
		#define _T(x)		x
	#endif
	#ifndef TEXT
		#define TEXT(x)	x
	#endif

	/* TCHAR and related types */
	#ifndef UNICODE
		typedef char			TCHAR;
		#define _tcscat		strcat
		#define _tcscpy		strcpy
		#define _tcslen		strlen
		#define _tcschr		strchr
		#define _tcsrchr		strrchr
		#define _stprintf	sprintf
		#define _tprintf		printf
		#define _tmain		main
	#else
		typedef wchar_t		TCHAR;
		#define _tcscat		wcscat
		#define _tcscpy		wcscpy
		#define _tcslen		wcslen
		#define _tcschr		wcschr
		#define _tcsrchr		wcsrchr
		#define _stprintf	swprintf
		#define _tprintf		wprintf
		#define _tmain		wmain
	#endif

	typedef TCHAR*			LPTSTR;
	typedef const TCHAR*	LPCTSTR;

	/* _TCHAR alias for compatibility with older code */
	#ifndef _TCHAR
		#define _TCHAR	TCHAR
	#endif

	/* Stub for MessageBox - just prints to stderr */
	static inline int MessageBox(void* hWnd, const char* lpText, const char* lpCaption, unsigned int uType) {
		(void)hWnd; (void)uType;
		fprintf(stderr, "[%s] %s\n", lpCaption, lpText);
		return 1;
	}

	/* SystemParametersInfo constants */
	#define SPI_GETMOUSE			0x0003
	#define SPI_SETMOUSE			0x0004

	/* Stub for SystemParametersInfo */
	static inline BOOL SystemParametersInfo(UINT uiAction, UINT uiParam, void* pvParam, UINT fWinIni) {
		(void)uiAction; (void)uiParam; (void)pvParam; (void)fWinIni;
		// Non-Windows platforms don't have mouse acceleration settings in the same way
		return FALSE;
	}

	/* Windows timing functions */
	#define GetTickCount()		platform_get_ticks()
	#define timeGetTime()		platform_get_ticks()
#endif

/* ============================================================================
 * Platform-Specific Types
 * ============================================================================ */

#ifdef PLATFORM_WINDOWS
	/* Windows types */
	#ifndef _WINDOWS_
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
	#endif

	typedef HANDLE	platform_thread_t;
	typedef HANDLE	platform_mutex_t;
	typedef HANDLE	platform_event_t;
	typedef HMODULE	platform_lib_t;

	#define PLATFORM_INVALID_THREAD	NULL
	#define PLATFORM_INVALID_MUTEX	NULL
	#define PLATFORM_INVALID_EVENT	NULL
	#define PLATFORM_INVALID_LIB		NULL

#else
	/* SDL/POSIX types */
	typedef SDL_Thread*	platform_thread_t;
	typedef SDL_mutex*	platform_mutex_t;

	/* Forward declaration for event structure */
	typedef struct platform_event_s* platform_event_t;

	typedef void*	platform_lib_t;

	#define PLATFORM_INVALID_THREAD	NULL
	#define PLATFORM_INVALID_MUTEX	NULL
	#define PLATFORM_INVALID_EVENT	NULL
	#define PLATFORM_INVALID_LIB		NULL

#endif /* PLATFORM_WINDOWS */

/* ============================================================================
 * Time Functions
 * ============================================================================ */

/**
 * Get current time in milliseconds (like timeGetTime/GetTickCount)
 * @return Time in milliseconds
 */
DWORD platform_get_ticks(void);

/**
 * Get high-performance counter value (like QueryPerformanceCounter)
 * @return Counter value
 */
uint64_t platform_get_performance_counter(void);

/**
 * Get high-performance counter frequency (like QueryPerformanceFrequency)
 * @return Counter frequency (counts per second)
 */
uint64_t platform_get_performance_frequency(void);

/**
 * Sleep for specified milliseconds (like Sleep)
 * @param ms Milliseconds to sleep
 */
void platform_sleep(DWORD ms);

/* ============================================================================
 * Thread Functions
 * ============================================================================ */

/**
 * Thread entry point type
 */
typedef DWORD (*platform_thread_func_t)(void* param);

/**
 * Create a new thread (like CreateThread)
 * @param func Thread function
 * @param param Parameter to pass to thread function
 * @return Thread handle or PLATFORM_INVALID_THREAD on failure
 */
platform_thread_t platform_thread_create(platform_thread_func_t func, void* param);

/**
 * Wait for thread to finish (like WaitForSingleObject on thread)
 * @param thread Thread handle
 * @return Wait result (0 = success, non-zero = failure)
 */
int platform_thread_wait(platform_thread_t thread);

/**
 * Close thread handle (like CloseHandle on thread)
 * @param thread Thread handle
 */
void platform_thread_close(platform_thread_t thread);

/* ============================================================================
 * Mutex Functions
 * ============================================================================ */

/**
 * Create a mutex (like CreateMutex)
 * @param initial_locked Whether mutex starts locked
 * @return Mutex handle or PLATFORM_INVALID_MUTEX on failure
 */
platform_mutex_t platform_mutex_create(int initial_locked);

/**
 * Lock a mutex (like WaitForSingleObject on mutex)
 * @param mutex Mutex handle
 * @return Lock result (0 = success, non-zero = failure)
 */
int platform_mutex_lock(platform_mutex_t mutex);

/**
 * Unlock a mutex (like ReleaseMutex)
 * @param mutex Mutex handle
 * @return Unlock result (0 = success, non-zero = failure)
 */
int platform_mutex_unlock(platform_mutex_t mutex);

/**
 * Close mutex handle (like CloseHandle on mutex)
 * @param mutex Mutex handle
 */
void platform_mutex_close(platform_mutex_t mutex);

/* ============================================================================
 * Event Functions
 * ============================================================================ */

/**
 * Create an event object (like CreateEvent)
 * @param manual_reset Whether event requires manual reset
 * @param initial_state Initial state (TRUE = signaled, FALSE = non-signaled)
 * @return Event handle or PLATFORM_INVALID_EVENT on failure
 */
platform_event_t platform_event_create(int manual_reset, int initial_state);

/**
 * Wait for event to be signaled (like WaitForSingleObject on event)
 * @param event Event handle
 * @param timeout Timeout in milliseconds (or PLATFORM_INFINITE for infinite wait)
 * @return Wait result (0 = success, non-zero = timeout/failure)
 */
int platform_event_wait(platform_event_t event, DWORD timeout);

/**
 * Signal an event (like SetEvent)
 * @param event Event handle
 * @return Signal result (0 = success, non-zero = failure)
 */
int platform_event_signal(platform_event_t event);

/**
 * Reset an event to non-signaled (like ResetEvent)
 * @param event Event handle
 * @return Reset result (0 = success, non-zero = failure)
 */
int platform_event_reset(platform_event_t event);

/**
 * Close event handle (like CloseHandle on event)
 * @param event Event handle
 */
void platform_event_close(platform_event_t event);

#define PLATFORM_INFINITE	((DWORD)-1)

/* ============================================================================
 * Dynamic Library Functions
 * ============================================================================ */

/**
 * Load a dynamic library (like LoadLibrary)
 * @param filename Library file name/path
 * @return Library handle or PLATFORM_INVALID_LIB on failure
 */
platform_lib_t platform_lib_load(const char* filename);

/**
 * Get function address from library (like GetProcAddress)
 * @param lib Library handle
 * @param symbol Function symbol name
 * @return Function pointer or NULL on failure
 */
void* platform_lib_get_symbol(platform_lib_t lib, const char* symbol);

/**
 * Unload a dynamic library (like FreeLibrary)
 * @param lib Library handle
 */
void platform_lib_free(platform_lib_t lib);

/* ============================================================================
 * File/Path Functions
 * ============================================================================ */

/**
 * Get path separator for current platform
 * @return Path separator character ('\\' on Windows, '/' on POSIX)
 */
char platform_get_path_separator(void);

/**
 * Check if file exists
 * @param filename File path to check
 * @return 1 if exists, 0 otherwise
 */
int platform_file_exists(const char* filename);

/**
 * Get current executable directory
 * @param buffer Buffer to store path
 * @param size Buffer size
 * @return 0 on success, non-zero on failure
 */
int platform_get_executable_dir(char* buffer, size_t size);

/**
 * Create directory if it doesn't exist
 * @param path Directory path
 * @return 0 on success, non-zero on failure
 */
int platform_create_directory(const char* path);

/* ============================================================================
 * Keyboard Functions (from PlatformUtil.h)
 * ============================================================================ */

/**
 * Check if Control key is currently pressed
 * @return 1 if pressed, 0 otherwise
 */
int platform_is_ctrl_pressed(void);

/**
 * Get keyboard scan code from lParam (Windows message parameter)
 * @param lParam LPARAM from keyboard message
 * @return Scan code
 */
BYTE platform_get_scan_code(DWORD lParam);

/* ============================================================================
 * Registry/Configuration Functions (Windows-only abstraction)
 * ============================================================================ */

/**
 * Get string value from configuration (replaces RegQueryValueEx)
 * @param key Configuration key name (e.g., "SOFTWARE\\Netmarble\\NetmarbleDarkEden")
 * @param value Value name
 * @param buffer Buffer to store value
 * @param size Buffer size (in/out)
 * @return 0 on success, non-zero on failure
 */
int platform_config_get_string(const char* key, const char* value,
                               char* buffer, DWORD* size);

/**
 * Set string value in configuration (replaces RegSetValueEx)
 * @param key Configuration key name
 * @param value Value name
 * @param data String data to set
 * @return 0 on success, non-zero on failure
 */
int platform_config_set_string(const char* key, const char* value,
                               const char* data);

/* ============================================================================
 * Error Reporting
 * ============================================================================ */

/**
 * Show error message box (like MessageBox)
 * @param title Message box title
 * @param message Error message
 */
void platform_show_error(const char* title, const char* message);

/* ============================================================================
 * Initialization
 * ============================================================================ */

/**
 * Initialize platform abstraction layer
 * Call this at program startup
 * @return 0 on success, non-zero on failure
 */
int platform_init(void);

/**
 * Cleanup platform abstraction layer
 * Call this at program shutdown
 */
void platform_shutdown(void);

/* ============================================================================
 * Windows Compatibility Macros
 * ============================================================================ */

// Define DLLIFC as empty on non-Windows platforms (for Immersion library compatibility)
#ifndef PLATFORM_WINDOWS
#ifndef DLLIFC
#define DLLIFC
#endif
#endif

// Windows constants that may be needed
#ifndef MAXLONG
#define MAXLONG 2147483647L  // 0x7FFFFFFF
#endif

#ifndef MAXDWORD
#define MAXDWORD 0xFFFFFFFF
#endif

/* ============================================================================
 * Rectangle Structure (Windows RECT equivalent)
 * ============================================================================ */

#ifndef RECT_DEFINED
#define RECT_DEFINED

/**
 * Rectangle structure (equivalent to Windows RECT)
 * Used for defining rectangular areas
 */
typedef struct tagRECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT, *PRECT, *LPRECT;

#endif /* RECT_DEFINED */

#ifdef __cplusplus
}
#endif

#endif /* __PLATFORM_H__ */
