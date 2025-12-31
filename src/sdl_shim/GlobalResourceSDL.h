#pragma once

#include <string>
#include "CSpriteSurfaceSDL.h"
#include "SpritePackSDL.h"

class GlobalResourceSDL
{
public:
    bool Init(SDL_Renderer* renderer, CSpriteSurfaceSDL* surface,
              const std::string& assembleBoxPath,
              const std::string& assembleButtonPath);

    void DrawDialog(int x, int y, int w, int h, bool alpha = false);

    C_SPRITE_PACK_SDL* GetButtonPack() { return &m_buttonPack; }
    CSpriteSurfaceSDL* GetSurface() { return m_surface; }

private:
    SDL_Renderer* m_renderer = nullptr;
    CSpriteSurfaceSDL* m_surface = nullptr; // not owned
    C_SPRITE_PACK_SDL m_dialogPack;
    C_SPRITE_PACK_SDL m_buttonPack;
    int m_backId = 0; // AB_BACK
    int m_rightId = 2; // AB_RIGHT
    int m_downId = 3; // AB_DOWN
    int m_cornerId = 4; // AB_RIGHTDOWN
    int m_buttonOkId = 0; // AB_BUTTON_OK
};
