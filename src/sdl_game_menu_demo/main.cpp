#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "spritepack.h"

// Forward declaration from widget_sdl/GameMenuSDL_demo.cpp
int RunGameMenuSDL_Demo(SDL_Window* window, SDL_Renderer* renderer, SpritePack* pack);

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow(
        "GameMenu SDL Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Optional: load spritepack if a path is provided as argv[1]
    SpritePack pack{};
    SpritePack* pack_ptr = nullptr;
    if (argc >= 2) {
        if (spritepack_load(&pack, argv[1]) == 0) {
            pack_ptr = &pack;
            SDL_Log("Loaded spritepack: %s", argv[1]);
        } else {
            SDL_Log("Failed to load spritepack: %s (falling back to rectangles)", argv[1]);
        }
    }

    int result = RunGameMenuSDL_Demo(window, renderer, pack_ptr);

    if (pack_ptr) {
        spritepack_free(&pack);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return (result == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}