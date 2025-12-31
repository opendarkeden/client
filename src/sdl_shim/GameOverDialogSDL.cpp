#include "GameOverDialogSDL.h"
#include "i_signal.h" // legacy message ids


void GameOverDialogSDL::Show()
{
    if (!m_gr) return;

    // Draw dialog background
    m_gr->DrawDialog(m_x, m_y, m_w, m_h, false);

    // Draw OK button centered at bottom
    C_SPRITE_PACK_SDL* btnPack = m_gr->GetButtonPack();
    SDL_Rect okRect = GetOkRect();
    if (btnPack && btnPack->GetWidth(0) > 0)
    {
        btnPack->Blt(okRect.x, okRect.y, 0);
    }
    else
    {
        // Fallback: draw a simple rectangle outline for the button on the backbuffer.
        CSpriteSurfaceSDL* surface = m_gr->GetSurface();
        SDL_Renderer* r = surface ? surface->GetRenderer() : nullptr;
        if (r)
        {
            SDL_Texture* prev = SDL_GetRenderTarget(r);
            SDL_SetRenderTarget(r, surface->GetTexture());
            SDL_SetRenderDrawColor(r, 200, 80, 80, 255);
            SDL_RenderDrawRect(r, &okRect);
            SDL_SetRenderTarget(r, prev);
        }
    }

    // Simple hover/pressed overlay tint.
    CSpriteSurfaceSDL* surface = m_gr->GetSurface();
    SDL_Renderer* r = surface ? surface->GetRenderer() : nullptr;
    if (r && (m_okHover || m_okPressed))
    {
        SDL_Texture* prev = SDL_GetRenderTarget(r);
        SDL_SetRenderTarget(r, surface->GetTexture());
        if (m_okPressed)
            SDL_SetRenderDrawColor(r, 0, 0, 0, 80);
        else
            SDL_SetRenderDrawColor(r, 255, 255, 255, 40);
        SDL_RenderFillRect(r, &okRect);
        SDL_SetRenderTarget(r, prev);
    }
}

bool GameOverDialogSDL::MouseControl(uint32_t message, int x, int y)
{
    SDL_Rect r = GetOkRect();
    SDL_Point tmp = SDL_Point{ x, y };
    bool inside = SDL_PointInRect(&tmp, &r);

    switch (message)
    {
    case M_MOVING:
        m_okHover = inside;
        if (!inside) m_okFocus = false;
        break;
    case M_LEFTBUTTON_DOWN:
    case M_LB_DOUBLECLICK:
        if (inside)
        {
            m_okPressed = true;
            m_okFocus = true;
        }
        else
        {
            m_okPressed = false;
            m_okFocus = false;
        }
        break;
    case M_LEFTBUTTON_UP:
        if (m_okPressed && inside)
        {
            m_okPressed = false;
            // In a full system we'd Run(DIALOG_EXECID_OK); here we just signal handled.
            return false; // consume
        }
        m_okPressed = false;
        break;
    default:
        break;
    }
    return true; // allow further routing if any
}

void GameOverDialogSDL::KeyboardControl(uint32_t message, uint32_t key, long /*extra*/)
{
    if (message != WM_KEYDOWN) return;
    if (key == SDLK_RETURN || key == ' ')
    {
        // Consume OK.
        m_okPressed = false;
    }
}

void GameOverDialogSDL::Process()
{
    // No-op placeholder to mirror legacy call sequence.
}

SDL_Rect GameOverDialogSDL::GetOkRect() const
{
    C_SPRITE_PACK_SDL* btnPack = m_gr ? m_gr->GetButtonPack() : nullptr;
    int bw = (btnPack) ? btnPack->GetWidth(0) : 100;
    int bh = (btnPack) ? btnPack->GetHeight(0) : 32;
    SDL_Rect rect;
    rect.x = m_x + (m_w - bw) / 2;
    rect.y = m_y + m_h - bh - 16;
    rect.w = bw;
    rect.h = bh;
    return rect;
}
