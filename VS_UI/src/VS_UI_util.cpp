#include <assert.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <vector>
#include <map>
#include "VS_UI_util.h"
#include "VS_UI_Base.h"
#include "spritepack.h"
#include "ui_surface.h"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
extern Base* gpC_base;

//--------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------
namespace {

inline DecodedSprite* get_sprite(SpritePack* pack, SPRITE_ID id) {
    static std::map<SpritePack*, std::vector<DecodedSprite>> cache;
    if (!pack) return nullptr;
    int sz = spritepack_size(pack);
    if (id < 0 || id >= sz) return nullptr;

    auto& vec = cache[pack];
    if (static_cast<int>(vec.size()) < sz) vec.resize(sz);

    DecodedSprite& dec = vec[id];
    if (!dec.pixels) {
        Sprite* spr = spritepack_get(pack, static_cast<uint16_t>(id));
        if (!spr || !spr->is_valid) return nullptr;
        if (sprite_decode(spr, &dec, 0) != 0) return nullptr;
        SDL_Renderer* renderer = nullptr;
        if (gpC_base && gpC_base->m_p_DDSurface_back && gpC_base->m_p_DDSurface_back->GetSurface()) {
            renderer = gpC_base->m_p_DDSurface_back->GetSurface()->renderer;
        }
        if (renderer) {
            decoded_sprite_create_texture(&dec, renderer);
        }
    }
    return &dec;
}

inline uint8_t clamp_u8(int v) { return static_cast<uint8_t>(std::clamp(v, 0, 255)); }

inline uint32_t tint_mask(int rgb_selector) {
    switch (rgb_selector) {
        case rgb_RED:   return 0xFF0000FF;
        case rgb_GREEN: return 0x00FF00FF;
        case rgb_BLUE:  return 0x0000FFFF;
        default:        return 0xFFFFFFFF;
    }
}

inline void blit_raw_clip(uint32_t* dst, int dst_pitch_bytes,
                          const DecodedSprite* spr,
                          int dst_x, int dst_y,
                          const Rect& clip) {
    if (!spr || !spr->pixels) return;
    const int pitch_px = dst_pitch_bytes / 4;
    int sx0 = std::max(clip.x - dst_x, 0);
    int sy0 = std::max(clip.y - dst_y, 0);
    int sx1 = std::min(clip.x + clip.w - dst_x, spr->width);
    int sy1 = std::min(clip.y + clip.h - dst_y, spr->height);
    if (sx0 >= sx1 || sy0 >= sy1) return;

    for (int sy = sy0; sy < sy1; ++sy) {
        const uint32_t* src_row = reinterpret_cast<const uint32_t*>(spr->pixels) + sy * spr->width + sx0;
        uint32_t* dst_row = dst + (dst_y + sy) * pitch_px + (dst_x + sx0);
        std::memcpy(dst_row, src_row, sizeof(uint32_t) * (sx1 - sx0));
    }
}

inline void blit_raw_alpha(uint32_t* dst, int dst_pitch_bytes,
                           const DecodedSprite* spr,
                           int dst_x, int dst_y,
                           uint8_t alpha_scale = 255) {
    if (!spr || !spr->pixels) return;
    const int pitch_px = dst_pitch_bytes / 4;
    const uint32_t* src = reinterpret_cast<const uint32_t*>(spr->pixels);
    for (int y = 0; y < spr->height; ++y) {
        uint32_t* drow = dst + (dst_y + y) * pitch_px + dst_x;
        const uint32_t* srow = src + y * spr->width;
        for (int x = 0; x < spr->width; ++x) {
            uint32_t sp = srow[x];
            uint8_t sa = static_cast<uint8_t>(sp & 0xFF);
            if (sa == 0) continue;
            uint8_t fa = static_cast<uint8_t>((sa * alpha_scale) / 255);
            uint8_t sr = static_cast<uint8_t>((sp >> 24) & 0xFF);
            uint8_t sg = static_cast<uint8_t>((sp >> 16) & 0xFF);
            uint8_t sb = static_cast<uint8_t>((sp >> 8) & 0xFF);
            uint32_t dp = drow[x];
            uint8_t dr = static_cast<uint8_t>((dp >> 24) & 0xFF);
            uint8_t dg = static_cast<uint8_t>((dp >> 16) & 0xFF);
            uint8_t db = static_cast<uint8_t>((dp >> 8) & 0xFF);

            uint8_t out_r = static_cast<uint8_t>((sr * fa + dr * (255 - fa)) / 255);
            uint8_t out_g = static_cast<uint8_t>((sg * fa + dg * (255 - fa)) / 255);
            uint8_t out_b = static_cast<uint8_t>((sb * fa + db * (255 - fa)) / 255);
            drow[x] = (static_cast<uint32_t>(out_r) << 24) |
                      (static_cast<uint32_t>(out_g) << 16) |
                      (static_cast<uint32_t>(out_b) << 8)  |
                      0xFF;
        }
    }
}

} // namespace

//--------------------------------------------------------------------------------------
// C_SPRITE_PACK
//--------------------------------------------------------------------------------------
C_SPRITE_PACK::C_SPRITE_PACK(const char *sz_filename) : m_pack(nullptr) {
    if (sz_filename) Open(sz_filename);
}

C_SPRITE_PACK::~C_SPRITE_PACK() {
    if (m_pack) {
        spritepack_free(m_pack);
        delete m_pack;
        m_pack = nullptr;
    }
}

void C_SPRITE_PACK::Open(const char *sz_filename) {
    if (m_pack) {
        spritepack_free(m_pack);
        delete m_pack;
        m_pack = nullptr;
    }
    m_pack = new SpritePack();
    if (spritepack_load(m_pack, sz_filename) != 0) {
        delete m_pack;
        m_pack = nullptr;
    }
}

int C_SPRITE_PACK::GetWidth(SPRITE_ID sprite_id) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    return spr ? spr->width : 0;
}

int C_SPRITE_PACK::GetHeight(SPRITE_ID sprite_id) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    return spr ? spr->height : 0;
}

void C_SPRITE_PACK::Blt(int x, int y, SPRITE_ID sprite_id) {
    POINT pt{ x, y };
    Blt(pt, sprite_id);
}

void C_SPRITE_PACK::Blt(POINT &point, SPRITE_ID sprite_id) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    if (!spr) return;
    if (gpC_base->m_p_DDSurface_back->Lock()) {
        gpC_base->m_p_DDSurface_back->BltSprite(&point, spr);
        gpC_base->m_p_DDSurface_back->Unlock();
    }
}

void C_SPRITE_PACK::BltColor(int x, int y, SPRITE_ID sprite_id, int rgb) {
    POINT pt{ x, y };
    BltColor(pt, sprite_id, rgb);
}

void C_SPRITE_PACK::BltColor(POINT &point, SPRITE_ID sprite_id, int rgb) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    if (!spr) return;
    if (gpC_base->m_p_DDSurface_back->Lock()) {
        gpC_base->m_p_DDSurface_back->BltSpriteColor(&point, spr, rgb);
        gpC_base->m_p_DDSurface_back->Unlock();
    }
}

void C_SPRITE_PACK::BltDarkness(int x, int y, SPRITE_ID sprite_id, int dark) {
    POINT pt{ x, y };
    BltDarkness(pt, sprite_id, dark);
}

void C_SPRITE_PACK::BltDarkness(POINT &point, SPRITE_ID sprite_id, int dark) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    if (!spr) return;
    if (gpC_base->m_p_DDSurface_back->Lock()) {
        gpC_base->m_p_DDSurface_back->BltSpriteDarkness(&point, spr, dark);
        gpC_base->m_p_DDSurface_back->Unlock();
    }
}

void C_SPRITE_PACK::BltAlpha(int x, int y, SPRITE_ID sprite_id, int alpha) {
    POINT pt{ x, y };
    BltAlpha(pt, sprite_id, alpha);
}

void C_SPRITE_PACK::BltAlpha(POINT &point, SPRITE_ID sprite_id, int alpha) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    if (!spr) return;
    if (gpC_base->m_p_DDSurface_back->Lock()) {
        gpC_base->m_p_DDSurface_back->BltSpriteAlpha(&point, spr, alpha);
        gpC_base->m_p_DDSurface_back->Unlock();
    }
}

void C_SPRITE_PACK::BltOffscreen(POINT &point, SPRITE_ID sprite_id) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    if (!spr) return;
    if (gpC_base->m_DDSurface_offscreen.Lock()) {
        gpC_base->m_DDSurface_offscreen.BltSprite(&point, spr);
        gpC_base->m_DDSurface_offscreen.Unlock();
    }
}

void C_SPRITE_PACK::BltOffscreen(int x, int y, SPRITE_ID sprite_id) {
    POINT pt{ x, y };
    BltOffscreen(pt, sprite_id);
}

void C_SPRITE_PACK::BltOutline(int x, int y, int /*color*/, SPRITE_ID sprite_id) {
    // Outline not implemented in SDL path; fallback to normal blit.
    Blt(x, y, sprite_id);
}

void C_SPRITE_PACK::BltClip(int x, int y, Rect &rect, SPRITE_ID sprite_id) {
    if (!gpC_base->m_p_DDSurface_back->Lock()) return;
    BltLockedClip(x, y, rect, sprite_id);
    gpC_base->m_p_DDSurface_back->Unlock();
}

void C_SPRITE_PACK::BltLockedClip(int x, int y, Rect &rect, SPRITE_ID sprite_id) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    if (!spr) return;
    DDSURFACEDESC2* d = gpC_base->m_p_DDSurface_back->GetDDSD();
    if (!d || !d->lpSurface) return;
    blit_raw_clip(reinterpret_cast<uint32_t*>(d->lpSurface),
                  static_cast<int>(d->lPitch),
                  spr, x, y, rect);
}

void C_SPRITE_PACK::BltLocked(int x, int y, SPRITE_ID sprite_id) {
    POINT pt{ x, y };
    BltLocked(pt, sprite_id);
}

void C_SPRITE_PACK::BltLocked(POINT &point, SPRITE_ID sprite_id) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    if (!spr) return;
    gpC_base->m_p_DDSurface_back->BltSprite(&point, spr);
}

void C_SPRITE_PACK::BltLockedColor(int x, int y, SPRITE_ID sprite_id, int rgb) {
    POINT pt{ x, y };
    BltLockedColor(pt, sprite_id, rgb);
}

void C_SPRITE_PACK::BltLockedColor(POINT &point, SPRITE_ID sprite_id, int rgb) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    if (!spr) return;
    gpC_base->m_p_DDSurface_back->BltSpriteColor(&point, spr, rgb);
}

void C_SPRITE_PACK::BltLockedDarkness(int x, int y, SPRITE_ID sprite_id, int dark) {
    POINT pt{ x, y };
    BltLockedDarkness(pt, sprite_id, dark);
}

void C_SPRITE_PACK::BltLockedDarkness(POINT &point, SPRITE_ID sprite_id, int dark) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    if (!spr) return;
    gpC_base->m_p_DDSurface_back->BltSpriteDarkness(&point, spr, dark);
}

void C_SPRITE_PACK::BltLockedAlpha(int x, int y, SPRITE_ID sprite_id, int alpha) {
    POINT pt{ x, y };
    BltLockedAlpha(pt, sprite_id, alpha);
}

void C_SPRITE_PACK::BltLockedAlpha(POINT &point, SPRITE_ID sprite_id, int alpha) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    if (!spr) return;
    gpC_base->m_p_DDSurface_back->BltSpriteAlpha(&point, spr, alpha);
}

void C_SPRITE_PACK::BltLockedOffscreen(POINT &point, SPRITE_ID sprite_id) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    if (!spr) return;
    gpC_base->m_DDSurface_offscreen.BltSprite(&point, spr);
}

void C_SPRITE_PACK::BltLockedOffscreen(int x, int y, SPRITE_ID sprite_id) {
    POINT pt{ x, y };
    BltLockedOffscreen(pt, sprite_id);
}

void C_SPRITE_PACK::BltLockedOutline(int x, int y, int /*color*/, SPRITE_ID sprite_id) {
    // Outline not implemented; fallback
    BltLocked(x, y, sprite_id);
}

void C_SPRITE_PACK::BltLockedColorSet(int x, int y, SPRITE_ID sprite_id, WORD /*colorset*/) {
    POINT pt{ x, y };
    BltLocked(pt, sprite_id);
}

void C_SPRITE_PACK::BltLockedColorSet(POINT &point, SPRITE_ID sprite_id, WORD /*colorset*/) {
    BltLocked(point, sprite_id);
}

bool C_SPRITE_PACK::IsPixel(int x, int y, SPRITE_ID sprite_id) {
    DecodedSprite* spr = get_sprite(m_pack, sprite_id);
    if (!spr || !spr->pixels) return false;
    if (x < 0 || y < 0 || x >= spr->width || y >= spr->height) return false;
    const uint32_t* pix = reinterpret_cast<const uint32_t*>(spr->pixels);
    uint32_t p = pix[y * spr->width + x];
    uint8_t a = static_cast<uint8_t>(p & 0xFF);
    return a != 0;
}

//--------------------------------------------------------------------------------------
// C_FRR
//--------------------------------------------------------------------------------------
C_FRR::C_FRR(const char *sz_filename) {
    if (sz_filename) Open(sz_filename);
}
C_FRR::~C_FRR() {}
bool C_FRR::Open(const char *sz_filename) {
    // FRAME_ARRAY is assumed to have LoadFromFile; if absent, treat as success.
    return m_C_frame_array.LoadFromFile(sz_filename);
}
int C_FRR::Size() const { return m_C_frame_array.GetSize(); }
int C_FRR::GetRX(int frame) const { return m_C_frame_array.GetRX(frame); }
int C_FRR::GetRY(int frame) const { return m_C_frame_array.GetRY(frame); }

//--------------------------------------------------------------------------------------
// C_ANI_OBJECT
//--------------------------------------------------------------------------------------
C_ANI_OBJECT::C_ANI_OBJECT(const char *sz_spkfile, const char *sz_frrfile) {
    m_pC_spk = new C_SPRITE_PACK(sz_spkfile);
    m_pC_frr = new C_FRR(sz_frrfile);
}
C_ANI_OBJECT::~C_ANI_OBJECT() {
    delete m_pC_spk;
    delete m_pC_frr;
}
int C_ANI_OBJECT::FrrSize() const { return m_pC_frr ? m_pC_frr->Size() : 0; }
int C_ANI_OBJECT::FrrRX(int index) const { return m_pC_frr ? m_pC_frr->GetRX(index) : 0; }
int C_ANI_OBJECT::FrrRY(int index) const { return m_pC_frr ? m_pC_frr->GetRY(index) : 0; }
void C_ANI_OBJECT::Blt(POINT &point, SPRITE_ID sprite_id) { if (m_pC_spk) m_pC_spk->Blt(point, sprite_id); }
void C_ANI_OBJECT::BltColor(POINT &point, SPRITE_ID sprite_id, int rgb) { if (m_pC_spk) m_pC_spk->BltColor(point, sprite_id, rgb); }
void C_ANI_OBJECT::BltDarkness(POINT &point, SPRITE_ID sprite_id, int dark_bit) { if (m_pC_spk) m_pC_spk->BltDarkness(point, sprite_id, dark_bit); }
int C_ANI_OBJECT::GetWidth(SPRITE_ID sprite_id) const { return m_pC_spk ? m_pC_spk->GetWidth(sprite_id) : 0; }
int C_ANI_OBJECT::GetHeight(SPRITE_ID sprite_id) const { return m_pC_spk ? m_pC_spk->GetHeight(sprite_id) : 0; }

//--------------------------------------------------------------------------------------
// C_ANIMATION (minimal functional timer-based playback)
//--------------------------------------------------------------------------------------
C_ANIMATION::C_ANIMATION(C_ANI_OBJECT *p_object)
: m_pC_ani_object(p_object)
, m_dw_prev_tickcount(0)
, m_dw_millisec(100)
, m_current_frame(0)
, m_x(0)
, m_y(0)
, m_bl_reverse(false)
, m_play_order(STOP)
, m_play_order_next(STOP)
{}

C_ANIMATION::~C_ANIMATION() {}

C_ANIMATION::PLAY_ORDER C_ANIMATION::GetAnimationState() const { return m_play_order; }
void C_ANIMATION::SetSpeed(DWORD millisec) { m_dw_millisec = millisec; }
void C_ANIMATION::SetPlayPosition(int x, int y) { m_x = x; m_y = y; }
void C_ANIMATION::Play() { m_play_order = PLAY; m_current_frame = 0; }
void C_ANIMATION::Stop() { m_play_order = STOP; }
void C_ANIMATION::PlayLoopBack() { m_play_order = PLAY_LOOPBACK; }
void C_ANIMATION::PlayBack() { m_play_order = PLAY_BACK; m_current_frame = m_pC_ani_object ? m_pC_ani_object->FrrSize() - 1 : 0; }
void C_ANIMATION::PlayLoop() { m_play_order = PLAY_LOOP; }
void C_ANIMATION::Show(bool enable) {
    if (!enable || !m_pC_ani_object) return;
    SPRITE_ID sid = static_cast<SPRITE_ID>(m_current_frame);
    POINT pt{ m_x, m_y };
    m_pC_ani_object->Blt(pt, sid);
}
void C_ANIMATION::SetNextPlayOrder(PLAY_ORDER order) { m_play_order_next = order; }
void C_ANIMATION::RunNextPlayOrder() { m_play_order = m_play_order_next; }
C_ANIMATION::PLAY_ORDER C_ANIMATION::GetNextPlayOrder() const { return m_play_order_next; }
void C_ANIMATION::Refresh() { m_current_frame = 0; }

void C_ANIMATION::Timer() {
    if (m_play_order == STOP || !m_pC_ani_object) return;
    DWORD now = GetTickCount();
    if (m_dw_prev_tickcount + m_dw_millisec > now) return;
    m_dw_prev_tickcount = now;

    int frames = m_pC_ani_object->FrrSize();
    if (frames <= 0) return;

    switch (m_play_order) {
        case PLAY:
            if (++m_current_frame >= static_cast<UINT>(frames)) m_play_order = STOP;
            break;
        case PLAY_LOOP:
            m_current_frame = (m_current_frame + 1) % frames;
            break;
        case PLAY_BACK:
            if (m_current_frame == 0) m_play_order = STOP;
            else --m_current_frame;
            break;
        case PLAY_BACKLOOP:
            if (m_current_frame == 0) m_current_frame = frames - 1;
            else --m_current_frame;
            break;
        case PLAY_LOOPBACK:
            if (!m_bl_reverse) {
                if (++m_current_frame >= static_cast<UINT>(frames - 1)) m_bl_reverse = true;
            } else {
                if (m_current_frame == 0) m_bl_reverse = false;
                else --m_current_frame;
            }
            break;
        default: break;
    }
}

//--------------------------------------------------------------------------------------
// C_SPRITE_FRAME_ARRAY
//--------------------------------------------------------------------------------------
C_SPRITE_FRAME_ARRAY::C_SPRITE_FRAME_ARRAY() : m_size(0), m_pC_sprite_frame_buf(nullptr) {}
C_SPRITE_FRAME_ARRAY::~C_SPRITE_FRAME_ARRAY() { free(m_pC_sprite_frame_buf); }
void C_SPRITE_FRAME_ARRAY::AddSpriteFrame(C_SPRITE_FRAME * pC_sf) {
    m_pC_sprite_frame_buf = (C_SPRITE_FRAME*)realloc(m_pC_sprite_frame_buf, sizeof(C_SPRITE_FRAME)*(m_size+1));
    m_pC_sprite_frame_buf[m_size] = *pC_sf;
    m_size++;
}
int C_SPRITE_FRAME_ARRAY::GetRX(int index) {
    if (index < 0 || index >= m_size) return 0;
    return m_pC_sprite_frame_buf[index].GetRX();
}
int C_SPRITE_FRAME_ARRAY::GetRY(int index) {
    if (index < 0 || index >= m_size) return 0;
    return m_pC_sprite_frame_buf[index].GetRY();
}

//--------------------------------------------------------------------------------------
// C_SPRITE_FRAME
//--------------------------------------------------------------------------------------
C_SPRITE_FRAME::C_SPRITE_FRAME() : m_rx(0), m_ry(0) {}
C_SPRITE_FRAME::~C_SPRITE_FRAME() {}
bool C_SPRITE_FRAME::Open(const char * pathfile) {
    // Stub: no-op, return success
    (void)pathfile;
    return true;
}
bool C_SPRITE_FRAME::Save(const char * pathfile) {
    // Stub: no-op, return success
    (void)pathfile;
    return true;
}