#include "UIContextSDL.h"

bool ui_context_init(UIContextSDL& ctx, SDL_Renderer* renderer, int width, int height)
{
    ctx.renderer = renderer;
    if (!ctx.backbuffer.Init(renderer, width, height))
        return false;
    // Offscreen is optional; initialize same size for compatibility.
    ctx.offscreen.Init(renderer, width, height);
    return true;
}

bool ui_context_load_pack(UIContextSDL& ctx, C_SPRITE_PACK_SDL& pack, const std::string& path)
{
    pack.SetSurface(&ctx.backbuffer);
    if (!pack.Open(path.c_str()))
        return false;
    return true;
}
