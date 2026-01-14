/*-----------------------------------------------------------------------------

	PlatformUtil.h

	Utility on the platform.
	Updated to use platform abstraction layer.

	Original: 2000.10.11. KJTINC
	Updated: 2025.01.14

-----------------------------------------------------------------------------*/

#ifndef __PLATFORMUTIL_H__
#define __PLATFORMUTIL_H__

#include "Platform.h"

// keyboard scan code macro (now platform-aware)
#ifdef PLATFORM_WINDOWS
	#define SCAN_CODE(x)			LOBYTE(HIWORD(x))
#else
	#define SCAN_CODE(x)			platform_get_scan_code(x)
#endif

//-----------------------------------------------------------------------------
// Public
//-----------------------------------------------------------------------------
inline bool g_GetCtrlPushState() {
	return platform_is_ctrl_pressed() ? true : false;
}

#endif