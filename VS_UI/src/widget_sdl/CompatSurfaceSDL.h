#pragma once

#include <SDL.h>
#include <cstdint>
#include "ui_surface.h"

#ifndef _WIN32
struct POINT { int x; int y; };
struct RECT  { int left; int top; int right; int bottom; };
#endif



// Minimal DDSURFACEDESC2-compatible layout used by legacy code
struct DDSURFACEDESC2 {
    uint32_t dwWidth{0};
    uint32_t dwHeight{0};
    uint32_t lPitch{0};      // bytes per row
    void*    lpSurface{nullptr};
};

// SDL-backed compatibility surface to replace the legacy CSpriteSurface.
// Only the methods actually used by VS_UI code are provided.
class CSpriteSurface {
public:
    CSpriteSurface();
    ~CSpriteSurface();

    // Create streaming texture-backed surface
    bool Init(int width, int height, SDL_Renderer* renderer);

    // Lock / Unlock
    bool Lock();
    void Unlock();
    bool IsLock() const { return locked_ != 0; }

    // Accessors
    DDSURFACEDESC2* GetDDSD() { return &ddsd_; }
    UI_Surface* GetSurface() { return &surf_; }

    // Drawing helpers (mapped to ui_surface_*). These expect a locked surface.
    void BltSprite(POINT* pPoint, DecodedSprite* pSprite);
    void BltSpriteColor(POINT* pPoint, DecodedSprite* pSprite, int rgb);
    void BltSpriteDarkness(POINT* pPoint, DecodedSprite* pSprite, int darkBits);
    void BltSpriteAlpha(POINT* pPoint, DecodedSprite* pSprite, int alpha);
    void BltSpriteColorSet(POINT* pPoint, DecodedSprite* pSprite, uint16_t colorSet);

    void DrawRect(RECT* rect, uint16_t color565);

private:
    UI_Surface   surf_{};
    DDSURFACEDESC2 ddsd_{};
    int locked_{0};
    SDL_Renderer* renderer_{nullptr};
};
