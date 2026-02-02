//-----------------------------------------------------------------------------
// CDirect3D.h
//-----------------------------------------------------------------------------
// SDL Backend Adaptation Layer - CDirect3D Compatibility Stub
//
// This file provides a minimal CDirect3D implementation to maintain compatibility
// with existing code after the DirectX to SDL migration.
//
// MIGRATION STATUS:
// - The original D3DLib has been removed
// - SDL2 is now used for all graphics operations
// - This stub class only provides methods needed for compilation
// - Actual rendering is handled by SpriteLib with SDL backend
//
// ARCHITECTURE:
// - DXLib provides the abstract interface (CDirectDraw, CDirectSound, etc.)
// - SDL backend implementations are in DXLib/SDLBackend files
// - This file acts as a minimal compatibility layer
//
// FUTURE CLEANUP:
// - Consider refactoring code to remove CDirect3D dependencies
// - Many CDirect3D::IsHAL() checks can be simplified (always true in SDL)
//-----------------------------------------------------------------------------

#ifndef __CDIRECT3D_H__
#define __CDIRECT3D_H__

#include <stdint.h>
#include "DXLib/CDirectDraw.h"  // For DDPIXELFORMAT and other types

// D3D constants
#define D3D_OK 0
#define D3DCLEAR_ZBUFFER 0x00000002
#define D3DPT_TRIANGLESTRIP 5
#define D3DTOP_MODULATE 4
#define D3DBLEND_ONE 1
#define D3DBLEND_SRCALPHA 2
#define D3DBLEND_INVSRCALPHA 3
#define D3DRENDERSTATE_LIGHTING 1
#define D3DRENDERSTATE_SRCBLEND 2
#define D3DRENDERSTATE_DESTBLEND 3
#define D3DTSS_COLOROP 1
#define D3DTSS_ALPHAOP 2
#define D3DRECT D3DRECT

struct D3DRECT {
    LONG x1;
    LONG y1;
    LONG x2;
    LONG y2;
};

//-----------------------------------------------------------------------------
// CDirect3D Class - SDL Compatibility Stub
//-----------------------------------------------------------------------------
class CDirect3D
{
public:
    // Hardware Acceleration Check
    // In SDL, we always use hardware acceleration when available
    static bool IsHAL()
    {
        return true;  // SDL always uses hardware acceleration
    }

    // Device loss check (not applicable in SDL)
    static bool IsLost()
    {
        return false;  // SDL doesn't have device loss like D3D
    }

    // Restore device (not applicable in SDL)
    static void Restore()
    {
        // No-op for SDL
    }

    // Texture size constraints (SDL2 supports non-power-of-2 textures)
    static bool IsTexturePow2()
    {
        return false;  // SDL2 supports NPOT textures
    }

    static bool IsTextureSquareOnly()
    {
        return false;  // SDL2 supports non-square textures
    }

    // Get adjusted texture size (no adjustment needed in SDL2)
    static void GetTextureSize(int& width, int& height)
    {
        // SDL2 supports any texture size, no adjustment needed
        // Original code would adjust to power-of-2 or square
    }

    // Get pixel format (simplified - returns a basic format)
    static LPDDPIXELFORMAT GetPixelFormat4444()
    {
        static DDPIXELFORMAT format = {
            sizeof(DDPIXELFORMAT),  // dwSize
            0,                      // dwFlags
            0,                      // dwFourCC
            16,                     // dwRGBBitCount (4444 = 16 bits)
            0x0F00,                 // dwRBitMask (4 bits)
            0x00F0,                 // dwGBitMask (4 bits)
            0x000F,                 // dwBBitMask (4 bits)
            0xF000                  // dwRGBAlphaBitMask (4 bits)
        };
        return &format;
    }

    // Get pixel format 1555 (16-bit: 5-5-5-1 RGB)
    static LPDDPIXELFORMAT GetPixelFormat1555()
    {
        static DDPIXELFORMAT format = {
            sizeof(DDPIXELFORMAT),  // dwSize
            0,                      // dwFlags
            0,                      // dwFourCC
            16,                     // dwRGBBitCount (1555 = 16 bits)
            0x7C00,                 // dwRBitMask (5 bits)
            0x03E0,                 // dwGBitMask (5 bits)
            0x001F,                 // dwBBitMask (5 bits)
            0x8000                  // dwRGBAlphaBitMask (1 bit)
        };
        return &format;
    }

    // Get pixel format 565 (16-bit: 5-6-5 RGB, no alpha)
    static LPDDPIXELFORMAT GetPixelFormat565()
    {
        static DDPIXELFORMAT format = {
            sizeof(DDPIXELFORMAT),  // dwSize
            0,                      // dwFlags
            0,                      // dwFourCC
            16,                     // dwRGBBitCount (565 = 16 bits)
            0xF800,                 // dwRBitMask (5 bits)
            0x07E0,                 // dwGBitMask (6 bits)
            0x001F,                 // dwBBitMask (5 bits)
            0x0000                  // dwRGBAlphaBitMask (no alpha)
        };
        return &format;
    }

    // Get pixel format 555 (15-bit: 5-5-5 RGB, no alpha)
    static LPDDPIXELFORMAT GetPixelFormat555()
    {
        static DDPIXELFORMAT format = {
            sizeof(DDPIXELFORMAT),  // dwSize
            0,                      // dwFlags
            0,                      // dwFourCC
            15,                     // dwRGBBitCount (555 = 15 bits)
            0x7C00,                 // dwRBitMask (5 bits)
            0x03E0,                 // dwGBitMask (5 bits)
            0x001F,                 // dwBBitMask (5 bits)
            0x0000                  // dwRGBAlphaBitMask (no alpha)
        };
        return &format;
    }

    // Get device - returns nullptr (not applicable in SDL)
    static void* GetDevice()
    {
        return nullptr;  // SDL doesn't have a device object like D3D
    }
};

#endif // __CDIRECT3D_H__
