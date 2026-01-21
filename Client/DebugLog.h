//-----------------------------------------------------------------------------
// DebugLog.h
//
// Lightweight, cross-platform logging system for Dark Eden client
// Replaces the old DEBUG_ADD series of macros
//
// Features:
// - Automatic __FILE__ and __LINE__ inclusion
// - Log levels: DEBUG/INFO/WARN/ERROR/NONE
// - Configurable output: console/file/memory array
// - Thread-safe (using CRITICAL_SECTION)
// - Conditional compilation: DEBUG logs removed in Release builds
//-----------------------------------------------------------------------------

#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Log Level Enumeration
//-----------------------------------------------------------------------------
typedef enum {
	LOG_LEVEL_DEBUG = 0,	// Detailed diagnostic information
	LOG_LEVEL_INFO,		// General informational messages
	LOG_LEVEL_WARN,		// Warning messages
	LOG_LEVEL_ERROR,	// Error messages
	LOG_LEVEL_NONE		// Disable all logging
} LogLevel;

//-----------------------------------------------------------------------------
// Public Interface
//-----------------------------------------------------------------------------

// Initialization and cleanup
void log_init(void);
void log_cleanup(void);

// Configuration
void log_set_level(LogLevel level);
void log_set_console_output(bool enable);
void log_set_file_output(const char *path);
void log_set_array_output(bool enable);

// Core logging function (called by macros)
void log_write(LogLevel level,
			   const char *file,
			   int line,
			   const char *fmt,
			   ...);

//-----------------------------------------------------------------------------
// Logging Macros
//-----------------------------------------------------------------------------

// Conditional compilation: Remove DEBUG logs in Release builds
#ifdef _DEBUG
	#define LOG_DEBUG(fmt, ...) \
		log_write(LOG_LEVEL_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
	#define LOG_DEBUG(fmt, ...) ((void)0)
#endif

#define LOG_INFO(fmt, ...) \
	log_write(LOG_LEVEL_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_WARN(fmt, ...) \
	log_write(LOG_LEVEL_WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
	log_write(LOG_LEVEL_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

//-----------------------------------------------------------------------------
// Backward Compatibility - Map old DEBUG_ADD macros to new system
//-----------------------------------------------------------------------------

#define DEBUG_ADD(msg)			LOG_INFO("%s", msg)
#define DEBUG_ADD_ERR(msg)		LOG_ERROR("%s", msg)
#define DEBUG_ADD_WAR(msg)		LOG_WARN("%s", msg)
#define DEBUG_ADD_FORMAT(fmt, ...)	LOG_INFO(fmt, ##__VA_ARGS__)
#define DEBUG_ADD_FORMAT_ERR(fmt, ...)	LOG_ERROR(fmt, ##__VA_ARGS__)
#define DEBUG_ADD_FORMAT_WAR(fmt, ...)	LOG_WARN(fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // __DEBUG_LOG_H__
