/*-----------------------------------------------------------------------------

	PlatformUtil.h

	Utility on the platform.

	2000.10.11. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __PLATFORMUTIL_H__
#define __PLATFORMUTIL_H__

#include <windows.h>

// keyboard scan code macro
#define SCAN_CODE(x)			LOBYTE(HIWORD(x))

//-----------------------------------------------------------------------------
// Public
//-----------------------------------------------------------------------------
extern bool g_GetCtrlPushState();

#endif