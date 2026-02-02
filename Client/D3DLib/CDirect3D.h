//-----------------------------------------------------------------------------
// CDirect3D.h
//
// SDL2 Implementation (Cross-platform)
// Windows Direct3D implementation removed - using SDL2 on all platforms
// NOTE: This class is deprecated. All Direct3D rendering has been replaced with SDL2.
//-----------------------------------------------------------------------------

#ifndef __CDIRECT3D_H__
#define __CDIRECT3D_H__

// Forward declarations for Direct3D types (stub)
struct IDirect3DDevice7;
typedef IDirect3DDevice7* LPDIRECT3DDEVICE7;

// Forward declaration for DDPIXELFORMAT (defined in CDirectDraw.h)
struct tagPIXELFORMAT;
typedef struct tagPIXELFORMAT DDPIXELFORMAT;
typedef DDPIXELFORMAT* LPDDPIXELFORMAT;

// D3D constants (stub values)
#define D3D_OK 0
#define D3DERR_DEVICELOST 0x88760166

// D3D enums
enum D3DPRIMITIVETYPE {
	D3DPT_TRIANGLESTRIP = 5
};

enum D3DRENDERSTATETYPE {
	D3DRENDERSTATE_LIGHTING = 1,
	D3DRENDERSTATE_SRCBLEND = 2,
	D3DRENDERSTATE_DESTBLEND = 3
};

enum D3DTEXTURESTAGESTATETYPE {
	D3DTSS_COLOROP = 1
};

enum D3DTOP {
	D3DTOP_MODULATE = 2
};

enum D3DBLEND {
	D3DBLEND_ONE = 2,
	D3DBLEND_SRCALPHA = 3,
	D3DBLEND_INVSRCALPHA = 4
};

// D3D structures (stubs)
struct D3DRECT {
	LONG x1, y1, x2, y2;
};

struct D3DLIGHT7 {
	// Stub - not used in SDL2
};

struct D3DDEVICEDESC7 {
	// Stub - not used in SDL2
};

//-----------------------------------------------------------------------------
// CDirect3D - SDL2 Stub Class
// NOTE: All methods return default values or are no-ops
//-----------------------------------------------------------------------------

class CDirect3D
{
public:
	// Initialization
	static bool Init();
	static void Release();
	static bool Restore();

	// Capability checks - SDL2 always uses hardware acceleration
	static bool IsHAL();
	static bool CheckHAL();
	static bool IsLost();

	// Texture capabilities - SDL2 supports NPOT and non-square textures
	static bool IsTexturePow2();
	static bool IsTextureSquareOnly();

	// Pixel format - SDL2 handles this internally
	static const LPDDPIXELFORMAT GetPixelFormat4444();
	static const LPDDPIXELFORMAT GetPixelFormat1555();
	static const LPDDPIXELFORMAT GetPixelFormat565();
	static const LPDDPIXELFORMAT GetPixelFormat555();

	// Device access - returns NULL (no D3D device in SDL2)
	static LPDIRECT3DDEVICE7 GetDevice();
};

#endif
