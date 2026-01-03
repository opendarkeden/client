#include "ui_surface.h"

typedef UI_Surface CSpriteSurface;

struct PrintInfo {  
    // TTF_Font* font;     // SDL_ttf 字体  
    SDL_Color text_color;  
    SDL_Color back_color;  
    int blend_mode;     // 替代 bk_mode  
    int text_align;  
};
