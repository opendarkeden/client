#include "CompatSurfaceSDL.h"

#include <algorithm>
#include "spritepack.h"     // DecodedSprite
#include "ui_surface.h"



namespace {
inline uint32_t color565_to_rgba(uint16_t c) {
    uint8_t r5 = (c >> 11) & 0x1F;
    uint8_t g6 = (c >> 5) & 0x3F;
    uint8_t b5 = (c >> 0) & 0x1F;
    uint8_t r8 = static_cast<uint8_t>((r5 * 255) / 31);
    uint8_t g8 = static_cast<uint8_t>((g6 * 255) / 63);
    uint8_t b8 = static_cast<uint8_t>((b5 * 255) / 31);
    return (static_cast<uint32_t>(r8) << 24) |
           (static_cast<uint32_t>(g8) << 16) |
           (static_cast<uint32_t>(b8) << 8)  |
           0xFF;
}

inline uint32_t channel_tint(int rgb_selector) {
    switch (rgb_selector) {
        case 0: return 0xFF0000FF; // red
        case 1: return 0x00FF00FF; // green
        case 2: return 0x0000FFFF; // blue
        default: return 0xFFFFFFFF;
    }
}

inline uint8_t clamp_u8(int v) {
    return static_cast<uint8_t>(std::clamp(v, 0, 255));
}

} // namespace

CSpriteSurface::CSpriteSurface() = default;
CSpriteSurface::~CSpriteSurface() {
    ui_surface_free(&surf_);
}

bool CSpriteSurface::Init(int width, int height, SDL_Renderer* renderer) {
    renderer_ = renderer;
    if (ui_surface_init(&surf_, renderer_, width, height) != 0) return false;
    ddsd_.dwWidth = static_cast<uint32_t>(width);
    ddsd_.dwHeight = static_cast<uint32_t>(height);
    ddsd_.lPitch = 0;
    ddsd_.lpSurface = nullptr;
    locked_ = 0;
    return true;
}

bool CSpriteSurface::Lock() {
    if (locked_) return true;
    if (ui_surface_lock(&surf_) != 0) return false;
    ddsd_.lPitch = static_cast<uint32_t>(surf_.pitch);
    ddsd_.lpSurface = surf_.pixels;
    locked_ = 1;
    return true;
}

void CSpriteSurface::Unlock() {
    if (!locked_) return;
    ui_surface_unlock(&surf_);
    locked_ = 0;
    ddsd_.lpSurface = nullptr;
}

void CSpriteSurface::BltSprite(POINT* pPoint, DecodedSprite* pSprite) {
    bool need_unlock = !locked_;
    if (need_unlock && !Lock()) return;
    ui_surface_blit_sprite(&surf_, pPoint->x, pPoint->y, pSprite);
    if (need_unlock) Unlock();
}

void CSpriteSurface::BltSpriteColor(POINT* pPoint, DecodedSprite* pSprite, int rgb) {
    bool need_unlock = !locked_;
    if (need_unlock && !Lock()) return;
    ui_surface_blit_sprite_color(&surf_, pPoint->x, pPoint->y, pSprite, channel_tint(rgb));
    if (need_unlock) Unlock();
}

void CSpriteSurface::BltSpriteDarkness(POINT* pPoint, DecodedSprite* pSprite, int darkBits) {
    bool need_unlock = !locked_;
    if (need_unlock && !Lock()) return;
    ui_surface_blit_sprite_darkness(&surf_, pPoint->x, pPoint->y, pSprite, clamp_u8(darkBits));
    if (need_unlock) Unlock();
}

void CSpriteSurface::BltSpriteAlpha(POINT* pPoint, DecodedSprite* pSprite, int alpha) {
    bool need_unlock = !locked_;
    if (need_unlock && !Lock()) return;
    // alpha in legacy was 0..32; map to 0..255
    uint8_t a = clamp_u8(alpha * 255 / 32);
    ui_surface_blit_sprite_alpha(&surf_, pPoint->x, pPoint->y, pSprite, a);
    if (need_unlock) Unlock();
}

void CSpriteSurface::BltSpriteColorSet(POINT* pPoint, DecodedSprite* pSprite, uint16_t /*colorSet*/) {
    // ColorSet not supported in SDL path; fallback to normal blit.
    BltSprite(pPoint, pSprite);
}

void CSpriteSurface::DrawRect(RECT* rect, uint16_t color565) {
    SDL_Rect r{rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top};
    bool need_unlock = !locked_;
    if (need_unlock && !Lock()) return;
    ui_surface_draw_rect(&surf_, &r, color565_to_rgba(color565));
    if (need_unlock) Unlock();
}