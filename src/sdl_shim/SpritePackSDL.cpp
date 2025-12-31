#include "SpritePackSDL.h"
#include <cstdlib>

C_SPRITE_PACK_SDL::C_SPRITE_PACK_SDL(SDL_Renderer* renderer)
    : m_renderer(renderer), m_surface(nullptr), m_loaded(false)
{
}

C_SPRITE_PACK_SDL::~C_SPRITE_PACK_SDL()
{
    for (auto& d : m_decoded)
    {
        if (d.texture)
            SDL_DestroyTexture(d.texture);
        decoded_sprite_free(&d);
    }
    spritepack_free(&m_pack);
}

bool C_SPRITE_PACK_SDL::Open(const char* filename)
{
    if (!filename) return false;
    if (spritepack_load_lazy(&m_pack, filename) != 0)
        return false;

    m_decoded.resize(m_pack.count);
    m_loaded = true;
    return true;
}

void C_SPRITE_PACK_SDL::Blt(int x, int y, uint16_t sprite_id)
{
    SDL_Point pt { x, y };
    Blt(pt, sprite_id);
}

void C_SPRITE_PACK_SDL::Blt(SDL_Point pt, uint16_t sprite_id)
{
    if (!m_loaded || !m_surface) return;
    if (sprite_id >= m_pack.count) return;
    Sprite* sp = spritepack_get(&m_pack, sprite_id);
    if (!sp || !sp->is_valid) return;

    DecodedSprite& d = m_decoded[sprite_id];
    if (!d.pixels)
    {
        if (sprite_decode(sp, &d, /*colorkey*/0) != 0)
            return;
        decoded_sprite_create_texture(&d, m_renderer);
    }
    POINT p { pt.x, pt.y };
    m_surface->BltSprite(&p, &d);
}

void C_SPRITE_PACK_SDL::BltAlpha(int x, int y, uint16_t sprite_id, int alpha)
{
    SDL_Point pt { x, y };
    BltAlpha(pt, sprite_id, alpha);
}

void C_SPRITE_PACK_SDL::BltAlpha(SDL_Point pt, uint16_t sprite_id, int alpha)
{
    if (!m_loaded || !m_surface) return;
    if (sprite_id >= m_pack.count) return;
    Sprite* sp = spritepack_get(&m_pack, sprite_id);
    if (!sp || !sp->is_valid) return;

    DecodedSprite& d = m_decoded[sprite_id];
    if (!d.pixels)
    {
        if (sprite_decode(sp, &d, 0) != 0)
            return;
        decoded_sprite_create_texture(&d, m_renderer);
    }
    POINT p { pt.x, pt.y };
    m_surface->BltSpriteAlpha(&p, &d, static_cast<uint8_t>(std::clamp(alpha, 0, 255)));
}

void C_SPRITE_PACK_SDL::BltColor(int x, int y, uint16_t sprite_id, int rgb)
{
    SDL_Point pt { x, y };
    BltColor(pt, sprite_id, rgb);
}

void C_SPRITE_PACK_SDL::BltColor(SDL_Point pt, uint16_t sprite_id, int rgb)
{
    if (!m_loaded || !m_surface) return;
    if (sprite_id >= m_pack.count) return;
    Sprite* sp = spritepack_get(&m_pack, sprite_id);
    if (!sp || !sp->is_valid) return;

    DecodedSprite& d = m_decoded[sprite_id];
    if (!d.pixels)
    {
        if (sprite_decode(sp, &d, 0) != 0)
            return;
        decoded_sprite_create_texture(&d, m_renderer);
    }
    POINT p { pt.x, pt.y };
    m_surface->BltSpriteColor(&p, &d, static_cast<uint8_t>(rgb));
}

void C_SPRITE_PACK_SDL::BltDarkness(int x, int y, uint16_t sprite_id, int dark)
{
    SDL_Point pt { x, y };
    BltDarkness(pt, sprite_id, dark);
}

void C_SPRITE_PACK_SDL::BltDarkness(SDL_Point pt, uint16_t sprite_id, int dark)
{
    if (!m_loaded || !m_surface) return;
    if (sprite_id >= m_pack.count) return;
    Sprite* sp = spritepack_get(&m_pack, sprite_id);
    if (!sp || !sp->is_valid) return;

    DecodedSprite& d = m_decoded[sprite_id];
    if (!d.pixels)
    {
        if (sprite_decode(sp, &d, 0) != 0)
            return;
        decoded_sprite_create_texture(&d, m_renderer);
    }
    POINT p { pt.x, pt.y };
    m_surface->BltSpriteDarkness(&p, &d, static_cast<uint8_t>(dark));
}

void C_SPRITE_PACK_SDL::BltOutline(int x, int y, int color, uint16_t sprite_id)
{
    if (!m_loaded || !m_surface) return;
    if (sprite_id >= m_pack.count) return;
    Sprite* sp = spritepack_get(&m_pack, sprite_id);
    if (!sp || !sp->is_valid) return;

    DecodedSprite& d = m_decoded[sprite_id];
    if (!d.pixels)
    {
        if (sprite_decode(sp, &d, 0) != 0)
            return;
        decoded_sprite_create_texture(&d, m_renderer);
    }
    POINT p { x, y };
    m_surface->BltSpriteOutline(&p, &d, static_cast<uint16_t>(color));
}

int C_SPRITE_PACK_SDL::GetWidth(uint16_t sprite_id) const
{
    if (!m_loaded || sprite_id >= m_pack.count) return 0;
    Sprite* sp = spritepack_get(const_cast<SpritePack*>(&m_pack), sprite_id);
    if (!sp || !sp->is_valid) return 0;
    return sp->width;
}

int C_SPRITE_PACK_SDL::GetHeight(uint16_t sprite_id) const
{
    if (!m_loaded || sprite_id >= m_pack.count) return 0;
    Sprite* sp = spritepack_get(const_cast<SpritePack*>(&m_pack), sprite_id);
    if (!sp || !sp->is_valid) return 0;
    return sp->height;
}
