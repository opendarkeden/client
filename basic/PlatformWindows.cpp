/*-----------------------------------------------------------------------------

	PlatformWindows.cpp

	Windows implementation of platform abstraction layer.
	Uses native Windows APIs.

	2025.01.14

-----------------------------------------------------------------------------*/

#define PLATFORM_WINDOWS_IMPL

#include "Platform.h"

#ifdef PLATFORM_WINDOWS

#include <windows.h>
#include <stdio.h>
#include <string.h>

/* ============================================================================
 * Time Functions
 * ============================================================================ */

DWORD platform_get_ticks(void) {
	/* Use timeGetTime for consistency with original code */
	return timeGetTime();
}

uint64_t platform_get_performance_counter(void) {
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return counter.QuadPart;
}

uint64_t platform_get_performance_frequency(void) {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart;
}

void platform_sleep(DWORD ms) {
	Sleep(ms);
}

/* ============================================================================
 * Thread Functions
 * ============================================================================ */

/* Thread wrapper to match Windows thread proc signature */
struct ThreadWrapperData {
	platform_thread_func_t func;
	void* param;
	DWORD result;
};

DWORD WINAPI ThreadWrapper(LPVOID lpParam) {
	ThreadWrapperData* data = (ThreadWrapperData*)lpParam;
	data->result = data->func(data->param);
	delete data;
	return data->result;
}

platform_thread_t platform_thread_create(platform_thread_func_t func, void* param) {
	ThreadWrapperData* data = new ThreadWrapperData;
	data->func = func;
	data->param = param;
	data->result = 0;

	DWORD threadId;
	HANDLE thread = CreateThread(NULL, 0, ThreadWrapper, data, 0, &threadId);
	return thread;
}

int platform_thread_wait(platform_thread_t thread) {
	if (thread == NULL) return 1;
	DWORD result = WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
	return (result == WAIT_OBJECT_0) ? 0 : 1;
}

void platform_thread_close(platform_thread_t thread) {
	if (thread != NULL) {
		CloseHandle(thread);
	}
}

/* ============================================================================
 * Mutex Functions
 * ============================================================================ */

platform_mutex_t platform_mutex_create(int initial_locked) {
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = FALSE;

	HANDLE mutex = CreateMutex(&sa, initial_locked ? TRUE : FALSE, NULL);
	return mutex;
}

int platform_mutex_lock(platform_mutex_t mutex) {
	if (mutex == NULL) return 1;
	DWORD result = WaitForSingleObject(mutex, INFINITE);
	return (result == WAIT_OBJECT_0) ? 0 : 1;
}

int platform_mutex_unlock(platform_mutex_t mutex) {
	if (mutex == NULL) return 1;
	return ReleaseMutex(mutex) ? 0 : 1;
}

void platform_mutex_close(platform_mutex_t mutex) {
	if (mutex != NULL) {
		CloseHandle(mutex);
	}
}

/* ============================================================================
 * Event Functions
 * ============================================================================ */

platform_event_t platform_event_create(int manual_reset, int initial_state) {
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = FALSE;

	HANDLE event = CreateEvent(&sa, manual_reset ? TRUE : FALSE,
	                          initial_state ? TRUE : FALSE, NULL);
	return event;
}

int platform_event_wait(platform_event_t event, DWORD timeout) {
	if (event == NULL) return 1;
	DWORD result = WaitForSingleObject(event, timeout);
	return (result == WAIT_OBJECT_0) ? 0 : 1;
}

int platform_event_signal(platform_event_t event) {
	if (event == NULL) return 1;
	return SetEvent(event) ? 0 : 1;
}

int platform_event_reset(platform_event_t event) {
	if (event == NULL) return 1;
	return ResetEvent(event) ? 0 : 1;
}

void platform_event_close(platform_event_t event) {
	if (event != NULL) {
		CloseHandle(event);
	}
}

/* ============================================================================
 * Dynamic Library Functions
 * ============================================================================ */

platform_lib_t platform_lib_load(const char* filename) {
	return LoadLibraryA(filename);
}

void* platform_lib_get_symbol(platform_lib_t lib, const char* symbol) {
	if (lib == NULL) return NULL;
	return GetProcAddress((HMODULE)lib, symbol);
}

void platform_lib_free(platform_lib_t lib) {
	if (lib != NULL) {
		FreeLibrary((HMODULE)lib);
	}
}

/* ============================================================================
 * File/Path Functions
 * ============================================================================ */

char platform_get_path_separator(void) {
	return '\\';
}

int platform_file_exists(const char* filename) {
	DWORD attrs = GetFileAttributesA(filename);
	return (attrs != INVALID_FILE_ATTRIBUTES);
}

int platform_get_executable_dir(char* buffer, size_t size) {
	if (buffer == NULL || size == 0) return 1;

	char modulePath[MAX_PATH];
	GetModuleFileNameA(NULL, modulePath, MAX_PATH);

	/* Extract directory */
	char* lastSlash = strrchr(modulePath, '\\');
	if (lastSlash != NULL) {
		*(lastSlash + 1) = '\0';
	}

	if (strlen(modulePath) >= size) return 1;
	strcpy(buffer, modulePath);
	return 0;
}

int platform_create_directory(const char* path) {
	return CreateDirectoryA(path, NULL) ? 0 : 1;
}

/* ============================================================================
 * Keyboard Functions
 * ============================================================================ */

int platform_is_ctrl_pressed(void) {
	/* Check both left and right control keys */
	return (GetKeyState(VK_CONTROL) & 0x8000) ? 1 : 0;
}

BYTE platform_get_scan_code(DWORD lParam) {
	return LOBYTE(HIWORD(lParam));
}

/* ============================================================================
 * Registry/Configuration Functions
 * ============================================================================ */

int platform_config_get_string(const char* key, const char* value,
                               char* buffer, DWORD* size) {
	HKEY hKey;
	LONG result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0, KEY_READ, &hKey);
	if (result != ERROR_SUCCESS) return 1;

	result = RegQueryValueExA(hKey, value, NULL, NULL, (BYTE*)buffer, size);
	RegCloseKey(hKey);

	return (result == ERROR_SUCCESS) ? 0 : 1;
}

int platform_config_set_string(const char* key, const char* value,
                               const char* data) {
	HKEY hKey;
	LONG result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0, KEY_WRITE, &hKey);
	if (result != ERROR_SUCCESS) return 1;

	DWORD len = (DWORD)strlen(data) + 1;
	result = RegSetValueExA(hKey, value, 0, REG_SZ, (const BYTE*)data, len);
	RegCloseKey(hKey);

	return (result == ERROR_SUCCESS) ? 0 : 1;
}

/* ============================================================================
 * Error Reporting
 * ============================================================================ */

void platform_show_error(const char* title, const char* message) {
	MessageBoxA(NULL, message, title, MB_OK | MB_ICONERROR);
}

/* ============================================================================
 * Initialization
 * ============================================================================ */

int platform_init(void) {
	/* Initialize Windows timer */
	timeBeginPeriod(1);
	return 0;
}

void platform_shutdown(void) {
	/* Cleanup Windows timer */
	timeEndPeriod(1);
}

#endif /* PLATFORM_WINDOWS */
