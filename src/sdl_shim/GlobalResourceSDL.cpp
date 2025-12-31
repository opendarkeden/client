#include "GlobalResourceSDL.h"
#include <algorithm>

bool GlobalResourceSDL::Init(SDL_Renderer* renderer, CSpriteSurfaceSDL* surface,
                             const std::string& assembleBoxPath,
                             const std::string& assembleButtonPath)
{
    m_renderer = renderer;
    m_surface = surface;

    m_dialogPack = C_SPRITE_PACK_SDL(renderer);
    m_buttonPack = C_SPRITE_PACK_SDL(renderer);

    if (!m_dialogPack.Open(assembleBoxPath.c_str())) return false;
    m_dialogPack.SetSurface(surface);

    if (!m_buttonPack.Open(assembleButtonPath.c_str())) return false;
    m_buttonPack.SetSurface(surface);

    // Use default indices defined in legacy code: AB_BACK = 0, AB_BUTTON_OK = 0.
    m_backId = 0;
    m_buttonOkId = 0;
    return true;
}

void GlobalResourceSDL::DrawDialog(int x, int y, int w, int h, bool /*alpha*/)
{
    // Reproduce legacy AssembleBox: back + right edge + bottom edge + corner, all clipped to dialog rect.
    SDL_Rect prevClip;
    bool hadClip = SDL_RenderIsClipEnabled(m_renderer);
    SDL_RenderGetClipRect(m_renderer, &prevClip);
    SDL_Rect clip { x, y, w, h };
    SDL_RenderSetClipRect(m_renderer, &clip);

    // Center background and tile edges to fill; avoids huge sprites messing alignment.
    int bw = m_dialogPack.GetWidth(m_backId);
    int bh = m_dialogPack.GetHeight(m_backId);
    if (bw > 0 && bh > 0)
    {
        int startX = x + (w - bw) / 2;
        int startY = y + (h - bh) / 2;
        for (int yy = startY; yy < y + h; yy += bh)
        {
            for (int xx = startX; xx < x + w; xx += bw)
            {
                m_dialogPack.Blt(xx, yy, static_cast<uint16_t>(m_backId));
            }
        }
    }

    int rw = m_dialogPack.GetWidth(m_rightId);
    int rh = m_dialogPack.GetHeight(m_rightId);
    if (rw > 0 && rh > 0)
    {
        int rx = x + w - rw;
        for (int yy = y; yy < y + h; yy += rh)
        {
            m_dialogPack.Blt(rx, yy, static_cast<uint16_t>(m_rightId));
        }
    }

    int dw = m_dialogPack.GetWidth(m_downId);
    int dh = m_dialogPack.GetHeight(m_downId);
    if (dw > 0 && dh > 0)
    {
        int dy = y + h - dh;
        for (int xx = x; xx < x + w; xx += dw)
        {
            m_dialogPack.Blt(xx, dy, static_cast<uint16_t>(m_downId));
        }
    }

    int cw = m_dialogPack.GetWidth(m_cornerId);
    int ch = m_dialogPack.GetHeight(m_cornerId);
    if (cw > 0 && ch > 0)
    {
        int cx = x + w - cw;
        int cy = y + h - ch;
        m_dialogPack.Blt(cx, cy, static_cast<uint16_t>(m_cornerId));
    }

    // Restore clip.
    if (hadClip)
        SDL_RenderSetClipRect(m_renderer, &prevClip);
    else
        SDL_RenderSetClipRect(m_renderer, nullptr);

    // If nothing rendered, fall back to button pack first sprite.
    if (bw == 0 && bh == 0 && rw == 0 && rh == 0 && dw == 0 && dh == 0 && cw == 0 && ch == 0)
    {
        bw = m_buttonPack.GetWidth(m_buttonOkId);
        bh = m_buttonPack.GetHeight(m_buttonOkId);
        if (bw > 0 && bh > 0)
        {
            m_buttonPack.Blt(x, y, static_cast<uint16_t>(m_buttonOkId));
        }
    }

    // Fallback: draw first button sprite if available.
    bw = m_buttonPack.GetWidth(m_buttonOkId);
    bh = m_buttonPack.GetHeight(m_buttonOkId);
    if (bw > 0 && bh > 0)
    {
        m_buttonPack.Blt(x, y, static_cast<uint16_t>(m_buttonOkId));
        return;
    }

    // Last resort: nothing to draw.
}
