#include <SDL.h>
#include <functional>
#include <string>
#include "ui_surface.h"
#include "ui_event_router.h"
#include "GameMenuSDL.h"

// Minimal demo runner to show how to wire GameMenuSDL with SDL2 + lib/ui infrastructure.
// Assumptions:
// - SDL is initialized by the caller.
// - A valid SDL_Renderer* is provided.
// - You have a SpritePack* for the game menu assets; you can pass nullptr to use fallback rectangles.
// - ui_surface is an existing C wrapper over SDL_Texture; we create one here as a backbuffer.
//
// This demo runs a simple loop until the window is closed or the CONTINUE button is pressed.
// The callback prints which menu was selected; replace with your game logic (e.g., SendMessage).

namespace {

// Optional: load a dummy sprite pack placeholder when none is provided.
SpritePack* load_dummy_spritepack(SDL_Renderer* renderer) {
    (void)renderer;
    return nullptr;
}

} // namespace

int RunGameMenuSDL_Demo(SDL_Window* window, SDL_Renderer* renderer, SpritePack* pack) {
    (void)window;
    if (!renderer) return -1;

    // Create UI surface as a render target
    UI_Surface surface{};
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    if (ui_surface_init(&surface, renderer, w, h) != 0) return -2;

    if (!pack) {
        pack = load_dummy_spritepack(renderer);
    }

    // Layout example: three buttons stacked vertically
    GameMenuSDL::ButtonLayout layout{};
    layout.option = { 40, 40, 160, 36 };
    layout.logout = { 40, 80, 160, 36 };
    layout.cont   = { 40, 120, 160, 36 };

    GameMenuSDL menu(renderer, &surface, pack, /*background sprite index*/ -1, layout);
    menu.set_position((w - 240) / 2, (h - 200) / 2);
    menu.set_on_select([](GameMenuSDL::Menu m) {
        switch (m) {
            case GameMenuSDL::Menu::OPTION:   SDL_Log("Option selected"); break;
            case GameMenuSDL::Menu::LOGOUT:   SDL_Log("Logout selected"); break;
            case GameMenuSDL::Menu::CONTINUE: SDL_Log("Continue selected"); break;
            default: break;
        }
    });

    bool running = true;
    while (running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                running = false;
                break;
            }
            UI_Event uiev{};
            if (ui_event_translate(&ev, &uiev)) {
                menu.handle_event(uiev);
            }
            menu.handle_sdl_event(ev);
        }

        // Clear backbuffer surface then render menu
        ui_surface_clear(&surface, 0x202020FF);
        menu.render();

        // Present: copy surface texture to renderer and present
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, ui_surface_get_texture(&surface), nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_Delay(10); // simple throttle
    }

    ui_surface_free(&surface);
    return 0;
}