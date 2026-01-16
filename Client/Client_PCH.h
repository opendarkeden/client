/*-----------------------------------------------------------------------------

	client_PCH.h

	Minimal precompiled header for CMake builds.
	Replaces the original VC6 precompiled header.

	2025.01.14

-----------------------------------------------------------------------------*/

#ifndef __CLIENT_PCH_H__
#define __CLIENT_PCH_H__

/* Define this as a game client build */
#define __GAME_CLIENT__

/* Platform types */
#include "../basic/Platform.h"

/* Standard C++ library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <iostream>

/* Use standard namespace for convenience */
using namespace std;

/* LONG type for non-Windows platforms */
#ifndef PLATFORM_WINDOWS
#ifndef LONG
typedef long LONG;
#endif
#endif

/* RECT definition for non-Windows platforms */
#ifndef PLATFORM_WINDOWS
#ifndef RECT_DEFINED
#define RECT_DEFINED
typedef struct tagRECT {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT, *PRECT, *LPRECT;
#endif

/* POINT definition */
#ifndef POINT_DEFINED
#define POINT_DEFINED
typedef struct tagPOINT {
	LONG x;
	LONG y;
} POINT, *PPOINT, *LPPOINT;
#endif

/* SIZE definition */
#ifndef SIZE_DEFINED
#define SIZE_DEFINED
typedef struct tagSIZE {
	LONG cx;
	LONG cy;
} SIZE, *PSIZE, *LPSIZE;
#endif

#endif /* PLATFORM_WINDOWS */

/* Exception handling for Packet system */
#include "Packet/Exception.h"

#endif /* __CLIENT_PCH_H__ */
