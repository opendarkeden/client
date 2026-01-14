/*-----------------------------------------------------------------------------

	client_PCH.h

	Minimal precompiled header for CMake builds.
	Replaces the original VC6 precompiled header.

	2025.01.14

-----------------------------------------------------------------------------*/

#ifndef __CLIENT_PCH_H__
#define __CLIENT_PCH_H__

/* Platform types */
#include "../basic/Platform.h"

/* Standard C++ library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* LONG type for non-Windows platforms */
#ifndef PLATFORM_WINDOWS
#ifndef LONG
typedef long LONG;
#endif
#endif

/* RECT definition for non-Windows platforms */
#ifndef PLATFORM_WINDOWS
#ifndef tagRECT_defined
#define tagRECT_defined
typedef struct tagRECT {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT, *PRECT, *LPRECT;
#endif

/* POINT definition */
typedef struct tagPOINT {
	LONG x;
	LONG y;
} POINT, *PPOINT, *LPPOINT;

/* SIZE definition */
typedef struct tagSIZE {
	LONG cx;
	LONG cy;
} SIZE, *PSIZE, *LPSIZE;

#endif /* PLATFORM_WINDOWS */

#endif /* __CLIENT_PCH_H__ */
