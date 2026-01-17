/*-----------------------------------------------------------------------------

	PlatformSDL.cpp

	SDL/POSIX implementation of platform abstraction layer.
	Uses SDL2 and POSIX APIs for cross-platform support.

	2025.01.14

-----------------------------------------------------------------------------*/

#include "Platform.h"

#ifndef PLATFORM_WINDOWS
/* Only compile on non-Windows platforms */

#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#ifdef PLATFORM_LINUX
	#include <limits.h>
	#include <stdlib.h>
#endif

#ifdef PLATFORM_MACOS
	#include <limits.h>  /* For PATH_MAX */
#endif

#ifdef PLATFORM_MACOS
	#include <libgen.h>
	#include <mach-o/dyld.h>
#endif

/* Event structure definition (opaque in header) */
struct platform_event_s {
	SDL_mutex* mutex;
	SDL_cond* cond;
	int signaled;
};

/*=============================================================================
 * Time Functions
 *=============================================================================*/

/* ============================================================================
 * Time Functions
 * ============================================================================ */

DWORD platform_get_ticks(void) {
	return SDL_GetTicks();
}

uint64_t platform_get_performance_counter(void) {
	return SDL_GetPerformanceCounter();
}

uint64_t platform_get_performance_frequency(void) {
	return SDL_GetPerformanceFrequency();
}

void platform_sleep(DWORD ms) {
	SDL_Delay(ms);
}

/* ============================================================================
 * Thread Functions
 * ============================================================================ */

struct ThreadWrapperData {
	platform_thread_func_t func;
	void* param;
};

int SDLCALL ThreadWrapper(void* data) {
	ThreadWrapperData* wrapper = (ThreadWrapperData*)data;
	wrapper->func(wrapper->param);
	delete wrapper;
	return 0;
}

platform_thread_t platform_thread_create(platform_thread_func_t func, void* param) {
	ThreadWrapperData* wrapper = new ThreadWrapperData;
	wrapper->func = func;
	wrapper->param = param;

	return SDL_CreateThread(ThreadWrapper, "thread", wrapper);
}

int platform_thread_wait(platform_thread_t thread) {
	if (thread == NULL) return 1;
	int status = 0;
	SDL_WaitThread(thread, &status);
	return 0;
}

void platform_thread_close(platform_thread_t thread) {
	/* SDL threads are automatically cleaned up by SDL_WaitThread */
	/* No explicit close needed */
}

/* ============================================================================
 * Mutex Functions
 * ============================================================================ */

platform_mutex_t platform_mutex_create(int initial_locked) {
	SDL_mutex* mutex = SDL_CreateMutex();
	if (mutex != NULL && initial_locked) {
		SDL_LockMutex(mutex);
	}
	return mutex;
}

int platform_mutex_lock(platform_mutex_t mutex) {
	return (SDL_LockMutex(mutex) == 0) ? 0 : 1;
}

int platform_mutex_unlock(platform_mutex_t mutex) {
	return (SDL_UnlockMutex(mutex) == 0) ? 0 : 1;
}

void platform_mutex_close(platform_mutex_t mutex) {
	if (mutex != NULL) {
		SDL_DestroyMutex(mutex);
	}
}

/* ============================================================================
 * Event Functions
 * ============================================================================ */

platform_event_t platform_event_create(int manual_reset, int initial_state) {
	platform_event_t event = new struct platform_event_s;
	if (event == NULL) return NULL;

	event->mutex = SDL_CreateMutex();
	event->cond = SDL_CreateCond();
	event->signaled = initial_state ? 1 : 0;

	if (event->mutex == NULL || event->cond == NULL) {
		if (event->mutex) SDL_DestroyMutex(event->mutex);
		if (event->cond) SDL_DestroyCond(event->cond);
		delete event;
		return NULL;
	}

	return event;
}

int platform_event_wait(platform_event_t event, DWORD timeout) {
	if (event == NULL) return 1;

	SDL_LockMutex(event->mutex);

	/* If already signaled, return immediately */
	if (event->signaled) {
		if (!event->signaled) {
			/* Auto-reset: clear signal */
			event->signaled = 0;
		}
		SDL_UnlockMutex(event->mutex);
		return 0;
	}

	/* Wait for signal */
	int result = 0;
	if (timeout == PLATFORM_INFINITE) {
		SDL_CondWait(event->cond, event->mutex);
	} else {
		result = SDL_CondWaitTimeout(event->cond, event->mutex, timeout);
	}

	if (event->signaled) {
		result = 0;
		if (!0) { /* Auto-reset if manual_reset == 0 */
			event->signaled = 0;
		}
	}

	SDL_UnlockMutex(event->mutex);
	return result;
}

int platform_event_signal(platform_event_t event) {
	if (event == NULL) return 1;

	SDL_LockMutex(event->mutex);
	event->signaled = 1;
	SDL_CondSignal(event->cond);
	SDL_UnlockMutex(event->mutex);

	return 0;
}

int platform_event_reset(platform_event_t event) {
	if (event == NULL) return 1;

	SDL_LockMutex(event->mutex);
	event->signaled = 0;
	SDL_UnlockMutex(event->mutex);

	return 0;
}

void platform_event_close(platform_event_t event) {
	if (event != NULL) {
		if (event->mutex) SDL_DestroyMutex(event->mutex);
		if (event->cond) SDL_DestroyCond(event->cond);
		delete event;
	}
}

/* ============================================================================
 * Dynamic Library Functions
 * ============================================================================ */

platform_lib_t platform_lib_load(const char* filename) {
	return SDL_LoadObject(filename);
}

void* platform_lib_get_symbol(platform_lib_t lib, const char* symbol) {
	if (lib == NULL) return NULL;
	return SDL_LoadFunction(lib, symbol);
}

void platform_lib_free(platform_lib_t lib) {
	if (lib != NULL) {
		SDL_UnloadObject(lib);
	}
}

/* ============================================================================
 * File/Path Functions
 * ============================================================================ */

char platform_get_path_separator(void) {
	return '/';
}

int platform_file_exists(const char* filename) {
	struct stat st;
	return (stat(filename, &st) == 0);
}

int platform_get_executable_dir(char* buffer, size_t size) {
	if (buffer == NULL || size == 0) return 1;

	char path[PATH_MAX] = {0};

	#ifdef PLATFORM_MACOS
		uint32_t bufsize = sizeof(path);
		if (_NSGetExecutablePath(path, &bufsize) != 0) {
			return 1;
		}
	#elif defined(PLATFORM_LINUX)
		ssize_t count = readlink("/proc/self/exe", path, sizeof(path));
		if (count < 0) return 1;
		path[count] = '\0';
	#else
		return 1;
	#endif

	/* Extract directory */
	char* dir = dirname(path);
	if (dir == NULL) return 1;

	size_t len = strlen(dir);
	if (len + 1 > size) return 1;

	strcpy(buffer, dir);
	strcat(buffer, "/");
	return 0;
}

int platform_create_directory(const char* path) {
	#ifdef PLATFORM_LINUX
		return mkdir(path, 0755) == 0 ? 0 : 1;
	#else
		return mkdir(path, 0755) == 0 ? 0 : 1;
	#endif
}

/* ============================================================================
 * Keyboard Functions
 * ============================================================================ */

int platform_is_ctrl_pressed(void) {
	/* Check keyboard state via SDL */
	const Uint8* state = SDL_GetKeyboardState(NULL);
	return (state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL]) ? 1 : 0;
}

BYTE platform_get_scan_code(DWORD lParam) {
	/* SDL uses scancodes directly */
	return (BYTE)lParam;
}

/* ============================================================================
 * Registry/Configuration Functions
 * ============================================================================ */

/* Config file path (fallback for registry) */
static char g_config_file_path[PATH_MAX] = {0};

static void get_config_file_path(void) {
	if (g_config_file_path[0] != '\0') return; /* Already computed */

	/* Get executable directory */
	char exeDir[PATH_MAX];
	if (platform_get_executable_dir(exeDir, sizeof(exeDir)) != 0) {
		strcpy(exeDir, "./");
	}

	/* Use config file in executable directory */
	snprintf(g_config_file_path, sizeof(g_config_file_path),
	         "%sDarkEden.conf", exeDir);
}

int platform_config_get_string(const char* key, const char* value,
                               char* buffer, DWORD* size) {
	get_config_file_path();

	FILE* file = fopen(g_config_file_path, "r");
	if (file == NULL) return 1;

	char line[512];
	char searchKey[256];
	snprintf(searchKey, sizeof(searchKey), "%s.%s=", key, value);

	int found = 0;
	while (fgets(line, sizeof(line), file) != NULL) {
		if (strncmp(line, searchKey, strlen(searchKey)) == 0) {
			const char* val = line + strlen(searchKey);
			/* Remove newline */
			char* newline = strchr(const_cast<char*>(val), '\n');
			if (newline) *newline = '\0';

			size_t len = strlen(val) + 1;
			if (len <= *size) {
				strcpy(buffer, val);
				*size = (DWORD)len;
				found = 1;
			}
			break;
		}
	}

	fclose(file);
	return found ? 0 : 1;
}

int platform_config_set_string(const char* key, const char* value,
                               const char* data) {
	get_config_file_path();

	/* Read existing content */
	char* content = NULL;
	long fileSize = 0;

	FILE* file = fopen(g_config_file_path, "r");
	if (file != NULL) {
		fseek(file, 0, SEEK_END);
		fileSize = ftell(file);
		fseek(file, 0, SEEK_SET);

		if (fileSize > 0) {
			content = new char[fileSize + 1];
			fread(content, 1, fileSize, file);
			content[fileSize] = '\0';
		}
		fclose(file);
	}

	/* Open for writing */
	file = fopen(g_config_file_path, "w");
	if (file == NULL) {
		if (content) delete[] content;
		return 1;
	}

	/* Write existing content (if any) */
	if (content != NULL) {
		fputs(content, file);
		delete[] content;
	}

	/* Append new key-value */
	fprintf(file, "%s.%s=%s\n", key, value, data);
	fclose(file);

	return 0;
}

/* ============================================================================
 * Error Reporting
 * ============================================================================ */

void platform_show_error(const char* title, const char* message) {
	/* On SDL platforms, show error via SDL message box */
	if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, NULL) != 0) {
		/* Fallback to stderr */
		fprintf(stderr, "ERROR [%s]: %s\n", title, message);
	}
}

/* ============================================================================
 * Initialization
 * ============================================================================ */

int platform_init(void) {
	/* Initialize SDL subsystems we need */
	if (SDL_Init(0) < 0) {
		return 1;
	}
	return 0;
}

void platform_shutdown(void) {
	SDL_Quit();
}

#endif /* !PLATFORM_WINDOWS */
