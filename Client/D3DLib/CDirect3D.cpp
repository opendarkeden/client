//-----------------------------------------------------------------------------
// CDirect3D.cpp
//
// SDL2 Implementation (Cross-platform)
// Windows Direct3D implementation removed - using SDL2 on all platforms
//-----------------------------------------------------------------------------

#include "CDirect3D.h"

//-----------------------------------------------------------------------------
// CDirect3D - SDL2 Stub Implementations
//-----------------------------------------------------------------------------

// Initialization
bool CDirect3D::Init()
{
	// Not implemented - SDL2 handles initialization
	return true;
}

void CDirect3D::Release()
{
	// Not implemented - SDL2 handles cleanup
}

bool CDirect3D::Restore()
{
	// Not implemented - SDL2 doesn't need device restoration
	return true;
}

// Capability checks - SDL2 always uses hardware acceleration
bool CDirect3D::IsHAL()
{
	// SDL2 always uses hardware acceleration
	return true;
}

bool CDirect3D::CheckHAL()
{
	// SDL2 always uses hardware acceleration
	return true;
}

bool CDirect3D::IsLost()
{
	// SDL2 doesn't lose devices like Direct3D
	return false;
}

// Texture capabilities - SDL2 supports NPOT and non-square textures
bool CDirect3D::IsTexturePow2()
{
	// SDL2 supports non-power-of-2 textures
	return false;
}

bool CDirect3D::IsTextureSquareOnly()
{
	// SDL2 supports non-square textures
	return false;
}

// Pixel format - SDL2 handles this internally
const LPDDPIXELFORMAT CDirect3D::GetPixelFormat4444()
{
	// SDL2 handles pixel formats internally
	return nullptr;
}

const LPDDPIXELFORMAT CDirect3D::GetPixelFormat1555()
{
	// SDL2 handles pixel formats internally
	return nullptr;
}

const LPDDPIXELFORMAT CDirect3D::GetPixelFormat565()
{
	// SDL2 handles pixel formats internally
	return nullptr;
}

const LPDDPIXELFORMAT CDirect3D::GetPixelFormat555()
{
	// SDL2 handles pixel formats internally
	return nullptr;
}

// Device access - returns NULL (no D3D device in SDL2)
LPDIRECT3DDEVICE7 CDirect3D::GetDevice()
{
	// No Direct3D device in SDL2
	return nullptr;
}
