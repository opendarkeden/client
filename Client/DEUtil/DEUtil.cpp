// DEUtil.cpp : Defines the entry point for the DLL application.
//

// Only compile on Windows platforms - this is a DLL entry point
// On non-Windows platforms (macOS/Linux), DLLs are not used
#ifdef PLATFORM_WINDOWS

#include "stdafx.h"

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#endif /* PLATFORM_WINDOWS */

