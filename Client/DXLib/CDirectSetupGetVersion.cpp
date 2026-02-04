//-----------------------------------------------------------------------------
// File: CDirectSetupGetVersion.cpp
//
// Desc: SDL2 version detection
//       Replaces DirectX version detection for cross-platform support
//
// 2025.02.04 - Converted from DirectX to SDL2 version detection
//-----------------------------------------------------------------------------

#include "CDirectSetup.h"
#include <SDL2/SDL.h>

//-----------------------------------------------------------------------------
// Name: GetVersion()
// Desc: Returns SDL2 version information
//          pdwMajor: SDL2 major version
//          pdwMinor: SDL2 minor version
//          pdwPatch: SDL2 patch level
//-----------------------------------------------------------------------------
VOID
CDirectSetup::GetVersion( DWORD* pdwMajor, DWORD* pdwMinor, DWORD* pdwPatch )
{
    SDL_version version;
    SDL_GetVersion(&version);

    if (pdwMajor) *pdwMajor = version.major;
    if (pdwMinor) *pdwMinor = version.minor;
    if (pdwPatch) *pdwPatch = version.patch;
}
