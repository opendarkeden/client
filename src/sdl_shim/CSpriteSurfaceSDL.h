#pragma once

#include <SDL.h>
#include <cstdint>
#include "spritepack.h"
#include "sprite.h"

// Simple POINT replacement (legacy code used Windows POINT)
struct POINT
{
    int x;
    int y;
};

// Minimal surface descriptor compatible with legacy expectations.
struct SurfaceDesc
{
    int width;
    int height;
    int pitch;
    void* pixels;
};

// SDL-backed surface shim that mimics a subset of CDirectDrawSurface/CSpriteSurface.
class CSpriteSurfaceSDL
{
public:
    CSpriteSurfaceSDL();
    ~CSpriteSurfaceSDL();

    // Create a streaming texture as our lockable surface (RGBA32).
    bool Init(SDL_Renderer* renderer, int width, int height);
    // Legacy-compatible entry used by Base::InitSurface; caps ignored.
    bool InitOffsurface(int width, int height, uint32_t /*caps*/) { return Init(m_renderer, width, height); }

    // Lock/Unlock expose raw pixels for CPU-side effects.
    bool Lock();
    bool Unlock();
    bool IsLock() const { return m_locked; }

    // Legacy no-op stubs for color key / effects.
    void SetTransparency(int /*color*/) {}
    enum FUNCTION_EFFECT { EFFECT_NONE = 0 };
    static void SetEffect(FUNCTION_EFFECT /*func*/) {}

    // Basic blit variants (subset of the legacy API).
    void BltSprite(POINT* pPoint, DecodedSprite* sprite);
    void BltSpriteAlpha(POINT* pPoint, DecodedSprite* sprite, uint8_t alpha);
    void BltSpriteColor(POINT* pPoint, DecodedSprite* sprite, uint8_t rgbIndex);
    void BltSpriteDarkness(POINT* pPoint, DecodedSprite* sprite, uint8_t darkBits);
    void BltSpriteColorSet(POINT* pPoint, DecodedSprite* sprite, uint16_t colorSet);

    // Outline is stubbed to a simple black-tint render for now.
    void BltSpriteOutline(POINT* pPoint, DecodedSprite* sprite, uint16_t color);

    // Accessor to mimic GetDDSD() usage.
    SurfaceDesc* GetDDSD() { return &m_desc; }

    // For compatibility with code that expects a surface handle.
    SDL_Texture* GetSurface() const { return m_texture; }

    // Set as render target for offscreen drawing, then render-copy elsewhere.
    SDL_Texture* GetTexture() const { return m_texture; }
    SDL_Renderer* GetRenderer() const { return m_renderer; }

private:
    SDL_Renderer* m_renderer;
    SDL_Texture*  m_texture; // streaming texture acting as our backbuffer
    SurfaceDesc   m_desc;
    bool          m_locked;
};
