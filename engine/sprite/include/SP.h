#pragma once

#include "spritepack.h"
#include "ui_surface.h"
#include "frame.h"


struct PrintInfo {
    void* hfont;        // Legacy GDI font handle shim
    // TTF_Font* font;     // SDL_ttf 字体
//    SDL_Color text_color;
//    SDL_Color back_color;
//    int blend_mode;     // 替代 bk_mode
//    int text_align;
};
