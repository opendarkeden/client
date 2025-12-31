#include <SDL.h>
#include <cstdio>
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>

#include "sdl_shim/SP_SDL_Compat.h"  // maps legacy names to SDL shims
#include "sdl_shim/UIContextSDL.h"
#include "sdl_shim/GlobalResourceSDL.h"
#include "sdl_shim/GameOverDialogSDL.h"
#include "i_signal.h" // legacy message ids

// Simple demo: load a sprite pack and blit a few sprites onto a backbuffer,
// then present via SDL renderer. Adjust kSpritePackPath and sprite IDs as needed.

static const int kWidth = 800;
static const int kHeight = 600;
static const char* kDefaultSpritePackPath = "DarkEden/Data/Image/UI.spk"; // TODO: point to a valid .spk
static const char* kDefaultDialogBoxPath = "DarkEden/Data/Ui/spk/AssembleMessageBoxSlayer.spk";
static const char* kDefaultDialogButtonPath = "DarkEden/Data/Ui/spk/AssembleMessageBoxButtonSlayer.spk";

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL UI Demo",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          kWidth, kHeight,
                                          SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::fprintf(stderr, "CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        std::fprintf(stderr, "CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Backbuffer/offscreen setup using UIContextSDL (Base-like stand-in).
    UIContextSDL ui;
    if (!ui_context_init(ui, renderer, kWidth, kHeight))
    {
        std::fprintf(stderr, "UIContext init failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Sprite pack adapter.
    C_SPRITE_PACK uiPack(renderer);

    std::string packPath = (argc > 1) ? argv[1] : kDefaultSpritePackPath;
    if (!ui_context_load_pack(ui, uiPack, packPath))
    {
        std::fprintf(stderr, "Failed to open sprite pack: %s\n", packPath.c_str());
    }

    // Dialog resources (assemble box + buttons)
    GlobalResourceSDL gr;
    if (!gr.Init(renderer, &ui.backbuffer,
                 kDefaultDialogBoxPath,
                 kDefaultDialogButtonPath))
    {
        std::fprintf(stderr, "Failed to load dialog resources: %s , %s\n",
                     kDefaultDialogBoxPath, kDefaultDialogButtonPath);
    }

    int dlgW = 320;
    int dlgH = 180;
    int dlgX = (kWidth - dlgW) / 2;
    int dlgY = (kHeight - dlgH) / 2;
    GameOverDialogSDL dlg(&gr, dlgX, dlgY, dlgW, dlgH);

    // Collect sprite IDs from argv (after pack path). Fallback IDs used if none provided.
    std::vector<uint16_t> spriteIds;
    for (int i = 2; i < argc; ++i)
    {
        spriteIds.push_back(static_cast<uint16_t>(std::atoi(argv[i])));
    }
    if (spriteIds.empty())
    {
        spriteIds = {0, 1, 2, 3, 4};
    }

    bool running = true;
    SDL_Event ev;
    auto start = std::chrono::steady_clock::now();

    while (running)
    {
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                running = false;
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)
                running = false;

            // Map SDL events to legacy-style messages and route into dialog.
            switch (ev.type)
            {
            case SDL_MOUSEMOTION:
                dlg.MouseControl(M_MOVING, ev.motion.x, ev.motion.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (ev.button.button == SDL_BUTTON_LEFT)
                    if (!dlg.MouseControl(M_LEFTBUTTON_DOWN, ev.button.x, ev.button.y)) running = false;
                break;
            case SDL_MOUSEBUTTONUP:
                if (ev.button.button == SDL_BUTTON_LEFT)
                    if (!dlg.MouseControl(M_LEFTBUTTON_UP, ev.button.x, ev.button.y)) running = false;
                break;
            case SDL_KEYDOWN:
                dlg.KeyboardControl(WM_KEYDOWN, static_cast<uint32_t>(ev.key.keysym.sym), 0);
                break;
            default:
                break;
            }
        }

        // Draw onto the backbuffer texture.
        SDL_Texture* prev = SDL_GetRenderTarget(renderer);
        SDL_SetRenderTarget(renderer, ui.backbuffer.GetTexture());
        SDL_SetRenderDrawColor(renderer, 16, 24, 32, 255);
        SDL_RenderClear(renderer);

        // Draw game over dialog (static demo).
        dlg.Process();
        dlg.Show();

        SDL_SetRenderTarget(renderer, prev);

        // Present backbuffer to window.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, ui.backbuffer.GetTexture(), nullptr, nullptr);
        SDL_RenderPresent(renderer);

        // Simple frame cap ~60fps.
        SDL_Delay(16);

        // Optional: auto-quit after 10 seconds for convenience.
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - start).count() > 10)
            running = false;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
