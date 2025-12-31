#pragma once

#include "GlobalResourceSDL.h"

#ifndef WM_KEYDOWN
#define WM_KEYDOWN 0x0100
#endif


class GameOverDialogSDL
{
public:
    GameOverDialogSDL(GlobalResourceSDL* gr, int x, int y, int w, int h)
        : m_gr(gr), m_x(x), m_y(y), m_w(w), m_h(h) {}

    void Show();

    // Input routing hooks aligned with legacy interfaces.
    bool MouseControl(uint32_t message, int x, int y);
    void KeyboardControl(uint32_t message, uint32_t key, long extra);
    void Process();

    SDL_Rect GetOkRect() const;
    void SetOkHover(bool hover) { m_okHover = hover; }
    void SetOkPressed(bool pressed) { m_okPressed = pressed; }
    bool GetOkPressed() const { return m_okPressed; }

private:
    GlobalResourceSDL* m_gr;
    int m_x, m_y, m_w, m_h;
    bool m_okHover = false;
    bool m_okPressed = false;
    bool m_okFocus = false;
};
