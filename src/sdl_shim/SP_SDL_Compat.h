#pragma once

// Compatibility header to shadow legacy SP.h when the include path prefers src/sdl_shim.
// It maps legacy class names to SDL-backed shims without touching the original legacy sources.

#include "CSpriteSurfaceSDL.h"
#include "SpritePackSDL.h"

// Map legacy names to SDL-backed implementations.
typedef CSpriteSurfaceSDL CSpriteSurface;
typedef C_SPRITE_PACK_SDL C_SPRITE_PACK;

typedef unsigned short WORD;

typedef struct tagRect { int x, y, w, h; } Rect;
