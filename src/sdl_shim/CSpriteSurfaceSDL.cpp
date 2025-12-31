 #include "CSpriteSurfaceSDL.h"
#include <algorithm>
#include <vector>

CSpriteSurfaceSDL::CSpriteSurfaceSDL()
    : m_renderer(nullptr), m_texture(nullptr), m_locked(false)
{
    m_desc = {0, 0, 0, nullptr};
}

CSpriteSurfaceSDL::~CSpriteSurfaceSDL()
{
    if (m_texture)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

bool CSpriteSurfaceSDL::Init(SDL_Renderer* renderer, int width, int height)
{
    m_renderer = renderer;
    m_desc.width = width;
    m_desc.height = height;

    // Render-target texture so we can draw sprites onto it; still lockable for now.
    m_texture = SDL_CreateTexture(renderer,
                                  SDL_PIXELFORMAT_RGBA32,
                                  SDL_TEXTUREACCESS_TARGET,
                                  width,
                                  height);
    return m_texture != nullptr;
}

bool CSpriteSurfaceSDL::Lock()
{
    if (m_locked || m_texture == nullptr) return false;
    void* pixels = nullptr;
    int pitch = 0;
    if (SDL_LockTexture(m_texture, nullptr, &pixels, &pitch) != 0)
    {
        return false;
    }
    m_desc.pixels = pixels;
    m_desc.pitch = pitch;
    m_locked = true;
    return true;
}

bool CSpriteSurfaceSDL::Unlock()
{
    if (!m_locked || m_texture == nullptr) return false;
    SDL_UnlockTexture(m_texture);
    m_desc.pixels = nullptr;
    m_desc.pitch = 0;
    m_locked = false;
    return true;
}

static SDL_Rect make_dest_rect(const POINT* p, int w, int h)
{
    SDL_Rect dst;
    dst.x = p ? p->x : 0;
    dst.y = p ? p->y : 0;
    dst.w = w;
    dst.h = h;
    return dst;
}

static void ensure_texture(SDL_Renderer* renderer, DecodedSprite* sprite)
{
    if (!sprite || sprite->texture) return;
    // Create texture from decoded pixels.
    if (decoded_sprite_create_texture(sprite, renderer) != 0)
    {
        // If creation fails, leave texture null; caller will skip.
    }
}

void CSpriteSurfaceSDL::BltSprite(POINT* pPoint, DecodedSprite* sprite)
{
    if (!m_renderer || !m_texture || !sprite) return;
    ensure_texture(m_renderer, sprite);
    if (!sprite->texture) return;

    SDL_SetTextureColorMod(sprite->texture, 255, 255, 255);
    SDL_SetTextureAlphaMod(sprite->texture, 255);
    SDL_SetTextureBlendMode(sprite->texture, SDL_BLENDMODE_BLEND);

    SDL_Rect dst = make_dest_rect(pPoint, sprite->width, sprite->height);

    // Render to backbuffer target. Avoid target churn so clip state survives.
    SDL_Texture* prevTarget = SDL_GetRenderTarget(m_renderer);
    bool needChange = (prevTarget != m_texture);
    if (needChange) SDL_SetRenderTarget(m_renderer, m_texture);
    SDL_RenderCopy(m_renderer, sprite->texture, nullptr, &dst);
    if (needChange) SDL_SetRenderTarget(m_renderer, prevTarget);
}

void CSpriteSurfaceSDL::BltSpriteAlpha(POINT* pPoint, DecodedSprite* sprite, uint8_t alpha)
{
    if (!m_renderer || !m_texture || !sprite) return;
    ensure_texture(m_renderer, sprite);
    if (!sprite->texture) return;

    SDL_SetTextureColorMod(sprite->texture, 255, 255, 255);
    SDL_SetTextureAlphaMod(sprite->texture, alpha);
    SDL_SetTextureBlendMode(sprite->texture, SDL_BLENDMODE_BLEND);

    SDL_Rect dst = make_dest_rect(pPoint, sprite->width, sprite->height);
    SDL_Texture* prevTarget = SDL_GetRenderTarget(m_renderer);
    bool needChange = (prevTarget != m_texture);
    if (needChange) SDL_SetRenderTarget(m_renderer, m_texture);
    SDL_RenderCopy(m_renderer, sprite->texture, nullptr, &dst);
    if (needChange) SDL_SetRenderTarget(m_renderer, prevTarget);
}

void CSpriteSurfaceSDL::BltSpriteColor(POINT* pPoint, DecodedSprite* sprite, uint8_t rgbIndex)
{
    if (!m_renderer || !m_texture || !sprite) return;
    ensure_texture(m_renderer, sprite);
    if (!sprite->texture) return;

    uint8_t r = 255, g = 255, b = 255;
    // Legacy rgbIndex: 0=R,1=G,2=B. We keep the selected channel at 255, others dimmed.
    switch (rgbIndex)
    {
    case 0: g = b = 64; break;
    case 1: r = b = 64; break;
    case 2: r = g = 64; break;
    default: break;
    }

    SDL_SetTextureColorMod(sprite->texture, r, g, b);
    SDL_SetTextureAlphaMod(sprite->texture, 255);
    SDL_SetTextureBlendMode(sprite->texture, SDL_BLENDMODE_BLEND);

    SDL_Rect dst = make_dest_rect(pPoint, sprite->width, sprite->height);
    SDL_Texture* prevTarget = SDL_GetRenderTarget(m_renderer);
    bool needChange = (prevTarget != m_texture);
    if (needChange) SDL_SetRenderTarget(m_renderer, m_texture);
    SDL_RenderCopy(m_renderer, sprite->texture, nullptr, &dst);
    if (needChange) SDL_SetRenderTarget(m_renderer, prevTarget);
}

void CSpriteSurfaceSDL::BltSpriteDarkness(POINT* pPoint, DecodedSprite* sprite, uint8_t darkBits)
{
    if (!m_renderer || !m_texture || !sprite) return;
    ensure_texture(m_renderer, sprite);
    if (!sprite->texture) return;

    // darkBits was bit-shift in legacy; approximate by scaling color.
    uint8_t factor = static_cast<uint8_t>(std::max(0, 255 - (darkBits << 3))); // coarse approximation
    SDL_SetTextureColorMod(sprite->texture, factor, factor, factor);
    SDL_SetTextureAlphaMod(sprite->texture, 255);
    SDL_SetTextureBlendMode(sprite->texture, SDL_BLENDMODE_BLEND);

    SDL_Rect dst = make_dest_rect(pPoint, sprite->width, sprite->height);
    SDL_Texture* prevTarget = SDL_GetRenderTarget(m_renderer);
    bool needChange = (prevTarget != m_texture);
    if (needChange) SDL_SetRenderTarget(m_renderer, m_texture);
    SDL_RenderCopy(m_renderer, sprite->texture, nullptr, &dst);
    if (needChange) SDL_SetRenderTarget(m_renderer, prevTarget);
}

void CSpriteSurfaceSDL::BltSpriteColorSet(POINT* pPoint, DecodedSprite* sprite, uint16_t /*colorSet*/)
{
    // Palette swap not implemented; fall back to normal blit.
    BltSprite(pPoint, sprite);
}

void CSpriteSurfaceSDL::BltSpriteOutline(POINT* pPoint, DecodedSprite* sprite, uint16_t /*color*/)
{
    // Simplified: render a darker pass offset by 1px to mimic outline.
    if (!m_renderer || !m_texture || !sprite) return;
    ensure_texture(m_renderer, sprite);
    if (!sprite->texture) return;

    SDL_Texture* prevTarget = SDL_GetRenderTarget(m_renderer);
    bool needChange = (prevTarget != m_texture);
    if (needChange) SDL_SetRenderTarget(m_renderer, m_texture);

    SDL_SetTextureBlendMode(sprite->texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureColorMod(sprite->texture, 0, 0, 0);
    SDL_SetTextureAlphaMod(sprite->texture, 200);

    SDL_Rect dst = make_dest_rect(pPoint, sprite->width, sprite->height);
    SDL_Rect shadow = dst;
    shadow.x += 1; shadow.y += 1;
    SDL_RenderCopy(m_renderer, sprite->texture, nullptr, &shadow);

    SDL_SetTextureColorMod(sprite->texture, 255, 255, 255);
    SDL_SetTextureAlphaMod(sprite->texture, 255);
    SDL_RenderCopy(m_renderer, sprite->texture, nullptr, &dst);

    if (needChange) SDL_SetRenderTarget(m_renderer, prevTarget);
}
