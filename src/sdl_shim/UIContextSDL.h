#pragma once

#include <SDL.h>
#include <string>
#include "CSpriteSurfaceSDL.h"
#include "SpritePackSDL.h"

// Lightweight stand-in for the legacy Base surface setup.
struct UIContextSDL
{
    SDL_Renderer* renderer = nullptr;
    CSpriteSurfaceSDL backbuffer;      // acts like m_p_DDSurface_back
    CSpriteSurfaceSDL offscreen;       // optional offscreen; not all demos need it
};

// Initialize backbuffer/offscreen surfaces.
bool ui_context_init(UIContextSDL& ctx, SDL_Renderer* renderer, int width, int height);

// Load a sprite pack using SDL adapter; associates it with the backbuffer for blits.
bool ui_context_load_pack(UIContextSDL& ctx, C_SPRITE_PACK_SDL& pack, const std::string& path);
