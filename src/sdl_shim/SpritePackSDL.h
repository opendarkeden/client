#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <SDL.h>
#include "spritepack.h"
#include "sprite.h"
#include "CSpriteSurfaceSDL.h"

class C_SPRITE_PACK_SDL
{
public:
    explicit C_SPRITE_PACK_SDL(SDL_Renderer* renderer = nullptr);
    ~C_SPRITE_PACK_SDL();

    bool Open(const char* filename);

    // Blit functions mirroring legacy names (subset).
    void Blt(int x, int y, uint16_t sprite_id = 0);
    void Blt(SDL_Point pt, uint16_t sprite_id = 0);
    void BltAlpha(int x, int y, uint16_t sprite_id, int alpha);
    void BltAlpha(SDL_Point pt, uint16_t sprite_id, int alpha);
    void BltColor(int x, int y, uint16_t sprite_id, int rgb);
    void BltColor(SDL_Point pt, uint16_t sprite_id, int rgb);
    void BltDarkness(int x, int y, uint16_t sprite_id, int dark);
    void BltDarkness(SDL_Point pt, uint16_t sprite_id, int dark);
    void BltOutline(int x, int y, int color, uint16_t sprite_id = 0);

    int GetWidth(uint16_t sprite_id = 0) const;
    int GetHeight(uint16_t sprite_id = 0) const;

    void SetSurface(CSpriteSurfaceSDL* surface) { m_surface = surface; }

private:
    SDL_Renderer* m_renderer;
    CSpriteSurfaceSDL* m_surface; // not owned
    SpritePack m_pack{};
    std::vector<DecodedSprite> m_decoded;
    bool m_loaded;
};
