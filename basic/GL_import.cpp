/*----------------------------------------------------------------------------

	GL_import.cpp

	Definitions for GL_import function pointers on non-Windows platforms.
	These are declared as extern in GL_import.h and defined here.

-----------------------------------------------------------------------------*/

#include "GL_import.h"

// Function pointer definitions for non-Windows platforms
#ifndef PLATFORM_WINDOWS

void (*Bltz)(const S_SURFACEINFO *, const S_SURFACEINFO *, const S_RECT *, const S_RECT *) = NULL;
void (*CkBltz)(const S_SURFACEINFO *, const S_SURFACEINFO *, const S_RECT *, const S_RECT *, const int) = NULL;
void (*filledRect)(S_SURFACEINFO *, S_RECT *, int) = NULL;
void (*cls)(S_SURFACEINFO *, int) = NULL;
void (*putPixel)(S_SURFACEINFO *, int, int, int) = NULL;
int (*getPixel)(S_SURFACEINFO *, int, int) = NULL;
void (*vertline)(S_SURFACEINFO *, int, int, int, int, int) = NULL;
void (*horzline)(S_SURFACEINFO *, int, int, int, int, int) = NULL;

#endif
