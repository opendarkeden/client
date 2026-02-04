/**
 * @file demo_main.cpp
 * @brief Web Demo Entry Point - Phase 1: Map Rendering
 *
 * This demo uses the game's actual MZone class for zone loading,
 * demonstrating code reuse and proper decoupling through interfaces.
 *
 * Architecture:
 * - MZone (game code) -> MZoneTileProvider (adapter) -> ITileDataProvider (interface)
 * - TileRenderer (modular rendering) -> Uses ITileDataProvider
 * - This demo -> Uses both MZone and TileRenderer
 */

#include "Client/MZone.h"
#include "Client/MZoneTileProvider.h"
#include "Client/SpriteLib/CSpritePack.h"
#include "Client/SpriteLib/CSpriteSurface.h"
#include "Client/SpriteLib/SpriteLibBackend.h"
#include "Client/TileRenderer.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <cmath>
#include <string>

extern bool g_demoSkipImageObjects;

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Simple test mode - just show a window without loading data
// #define TEST_MODE_NO_DATA  // Disabled to use actual data files

// Constants from game
#define DEFAULT_CELL_PIXELS 48  // TILE_X from MViewDef.h

/**
 * Minimal Map Viewer Class
 *
 * This class demonstrates how to use game components (MZone, TileRenderer)
 * in a standalone demo application.
 */
class MapViewerDemo {
public:
    MapViewerDemo()
        : m_window(nullptr)
        , m_renderer(nullptr)
        , m_screenSurface(nullptr)
        , m_tilePack(nullptr)
        , m_objPack(nullptr)
        , m_zone(nullptr)
        , m_tileProvider(nullptr)
        , m_tileRenderer(nullptr)
        , m_cellPixels(DEFAULT_CELL_PIXELS)
        , m_cameraX(0)
        , m_cameraY(0)
        , m_zoom(1.0f)
        , m_running(true)
        , m_frameCount(0)
        , m_initialized(false)
        , m_windowWidth(0)
        , m_windowHeight(0)
    {
    }

    ~MapViewerDemo() {
        Cleanup();
    }

    bool Initialize(const char* mapFile, const char* tileSpkFile,
                    const char* objSpkFile, int windowWidth, int windowHeight) {
#ifdef TEST_MODE_NO_DATA
        std::cout << "======================================" << std::endl;
        std::cout << "Running in TEST MODE (no data files)" << std::endl;
        std::cout << "======================================" << std::endl;
#endif

        // Initialize SpriteLib backend
        if (spritectl_init() != 0) {
            std::cerr << "Failed to initialize SpriteLib backend" << std::endl;
            return false;
        }

        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
            return false;
        }

        // Create window
        m_window = SDL_CreateWindow(
            "Dark Eden Web Demo - Map Viewer",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            windowWidth,
            windowHeight,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );

        if (!m_window) {
            std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
            return false;
        }

        // Create renderer
        // Emscripten: force software renderer to avoid WebGL swap-interval setup
        int rendererFlags = 0;
#ifdef __EMSCRIPTEN__
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
        SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
        rendererFlags = SDL_RENDERER_SOFTWARE;
#endif
        m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);
        if (!m_renderer) {
            std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
            return false;
        }

        // Create screen surface
        m_screenSurface = new CSpriteSurface();
        if (!m_screenSurface->Init(windowWidth, windowHeight)) {
            std::cerr << "Failed to initialize screen surface" << std::endl;
            return false;
        }

#ifdef TEST_MODE_NO_DATA
        // Create a minimal test zone programmatically
        m_zone = new MZone();
        m_zone->Init(20, 20);  // 20x20 test zone

        std::cout << "Created test zone: TestZone"
                  << " (" << m_zone->GetWidth() << "x" << m_zone->GetHeight() << ")" << std::endl;

        // Create tile provider (adapter from MZone to ITileDataProvider)
        m_tileProvider = new MZoneTileProvider();
        m_tileProvider->SetZone(m_zone);

        // Create empty tile pack (no sprites to load)
        m_tilePack = new CSpritePack();
        std::cout << "Test mode: No tile sprites loaded" << std::endl;

        // Create TileRenderer (will work with empty pack)
        m_tileRenderer = new TileRenderer();
        if (!m_tileRenderer->Init(m_screenSurface, m_tilePack)) {
            std::cerr << "Failed to initialize TileRenderer" << std::endl;
            return false;
        }

        // Center camera on test zone
        m_cameraX = (m_zone->GetWidth() * m_cellPixels) / 2 - windowWidth / 2;
        m_cameraY = (m_zone->GetHeight() * m_cellPixels / 2) / 2 - windowHeight / 2;

        std::cout << "Test mode initialization complete!" << std::endl;
        return true;

#else
        // Load zone using game's MZone class
        m_zone = new MZone();
        std::ifstream mapFilestream(mapFile, std::ios::binary);
        if (!mapFilestream.is_open()) {
            std::cerr << "Failed to open map file: " << mapFile << std::endl;
            return false;
        }

        if (!m_zone->LoadFromFile(mapFilestream)) {
            std::cerr << "Failed to load zone from: " << mapFile << std::endl;
            mapFilestream.close();
            return false;
        }
        mapFilestream.close();

        std::cout << "Loaded zone: " << (const char*)m_zone->GetName()
                  << " (" << m_zone->GetWidth() << "x" << m_zone->GetHeight() << ")" << std::endl;

        // Create tile provider (adapter from MZone to ITileDataProvider)
        m_tileProvider = new MZoneTileProvider();
        m_tileProvider->SetZone(m_zone);

        // Load tile sprite pack
        m_tilePack = new CSpritePack();
        if (!m_tilePack->LoadFromFile(tileSpkFile)) {
            std::cerr << "Failed to load tile pack: " << tileSpkFile << std::endl;
            return false;
        }
        std::cout << "Loaded " << m_tilePack->GetSize() << " tiles" << std::endl;

        // Create TileRenderer
        m_tileRenderer = new TileRenderer();
        if (!m_tileRenderer->Init(m_screenSurface, m_tilePack)) {
            std::cerr << "Failed to initialize TileRenderer" << std::endl;
            return false;
        }

        // Load ImageObject sprite pack if provided
        if (objSpkFile != nullptr && strlen(objSpkFile) > 0) {
#ifdef __EMSCRIPTEN__
            if (g_demoSkipImageObjects) {
                std::cout << "Web demo: skipping ImageObject sprite pack" << std::endl;
            } else
#endif
            {
            m_objPack = new CSpritePack();
            if (!m_objPack->LoadFromFile(objSpkFile)) {
                std::cout << "Warning: failed to load ImageObject pack: " << objSpkFile << std::endl;
                delete m_objPack;
                m_objPack = nullptr;
            } else {
                std::cout << "Loaded " << m_objPack->GetSize() << " ImageObjects" << std::endl;
            }
            }
        }

        // Center camera on map
        m_cameraX = (m_zone->GetWidth() * m_cellPixels) / 2 - windowWidth / 2;
        m_cameraY = (m_zone->GetHeight() * m_cellPixels / 2) / 2 - windowHeight / 2;

        return true;
#endif
    }

    void SetInitParams(const char* mapFile, const char* tileSpkFile,
                       const char* objSpkFile, int windowWidth, int windowHeight) {
        m_mapFile = mapFile ? mapFile : "";
        m_tileSpkFile = tileSpkFile ? tileSpkFile : "";
        m_objSpkFile = objSpkFile ? objSpkFile : "";
        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;
    }

    void Run() {
        std::cout << "Starting..." << std::endl << std::flush;

#ifdef __EMSCRIPTEN__
        std::cout << "Emscripten mode" << std::endl << std::flush;

        // Emscripten: use asynchronous main loop
        emscripten_set_main_loop_arg([](void* arg) {
            MapViewerDemo* demo = static_cast<MapViewerDemo*>(arg);

            if (!demo->m_initialized) {
                std::cout << "INIT (main loop)" << std::endl << std::flush;
                if (!demo->Initialize(demo->m_mapFile.c_str(),
                                      demo->m_tileSpkFile.c_str(),
                                      demo->m_objSpkFile.empty() ? nullptr : demo->m_objSpkFile.c_str(),
                                      demo->m_windowWidth,
                                      demo->m_windowHeight)) {
                    std::cerr << "Failed to initialize viewer (main loop)" << std::endl;
                    emscripten_cancel_main_loop();
                    return;
                }
                demo->m_initialized = true;
                std::cout << "INIT OK (main loop)" << std::endl << std::flush;
            }

            if (!demo->m_running) {
                emscripten_cancel_main_loop();
                return;
            }

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                demo->HandleEvent(event);
            }

            demo->Render();
            demo->m_frameCount++;
        }, this, 0, true);
#else
        std::cout << "Desktop mode" << std::endl << std::flush;
        // Desktop: use traditional loop
        SDL_Event event;
        while (m_running) {
            while (SDL_PollEvent(&event)) {
                HandleEvent(event);
            }
            Render();
            SDL_Delay(16);
        }
#endif

        std::cout << "Setup complete" << std::endl << std::flush;
    }

    void Cleanup() {
        if (m_tileProvider) { delete m_tileProvider; m_tileProvider = nullptr; }
        if (m_zone) { delete m_zone; m_zone = nullptr; }
        if (m_objPack) { delete m_objPack; m_objPack = nullptr; }
        if (m_tileRenderer) { delete m_tileRenderer; m_tileRenderer = nullptr; }
        if (m_tilePack) { delete m_tilePack; m_tilePack = nullptr; }
        if (m_screenSurface) {
            m_screenSurface->Release();
            delete m_screenSurface;
            m_screenSurface = nullptr;
        }
        if (m_renderer) {
            SDL_DestroyRenderer(m_renderer);
            m_renderer = nullptr;
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
        spritectl_shutdown();
    }

private:
    void HandleEvent(SDL_Event& event) {
        if (event.type == SDL_QUIT) {
            m_running = false;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    m_running = false;
                    break;

                case SDLK_w: case SDLK_UP:
                    m_cameraY -= 50;
                    break;
                case SDLK_s: case SDLK_DOWN:
                    m_cameraY += 50;
                    break;
                case SDLK_a: case SDLK_LEFT:
                    m_cameraX -= 50;
                    break;
                case SDLK_d: case SDLK_RIGHT:
                    m_cameraX += 50;
                    break;

                case SDLK_EQUALS: case SDLK_PLUS: case SDLK_KP_PLUS:
                    m_zoom *= 1.1f;
                    if (m_zoom > 4.0f) m_zoom = 4.0f;
                    break;
                case SDLK_MINUS: case SDLK_KP_MINUS:
                    m_zoom /= 1.1f;
                    if (m_zoom < 0.25f) m_zoom = 0.25f;
                    break;

                case SDLK_1: case SDLK_HOME:
                    {
                        int windowWidth, windowHeight;
                        SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);
                        m_cameraX = (m_zone->GetWidth() * m_cellPixels) / 2 - windowWidth / 2;
                        m_cameraY = (m_zone->GetHeight() * m_cellPixels / 2) / 2 - windowHeight / 2;
                        m_zoom = 1.0f;
                    }
                    break;
            }
        }
        else if (event.type == SDL_MOUSEWHEEL) {
            if (event.wheel.y > 0) {
                m_zoom *= 1.1f;
                if (m_zoom > 4.0f) m_zoom = 4.0f;
            } else if (event.wheel.y < 0) {
                m_zoom /= 1.1f;
                if (m_zoom < 0.25f) m_zoom = 0.25f;
            }
        }
    }

    void Render() {
        static int renderCallCount = 0;
        ++renderCallCount;
#ifdef __EMSCRIPTEN__
        if (renderCallCount < 5 || (renderCallCount % 300) == 0) {
            std::cout << "R" << renderCallCount << std::endl << std::flush;
        }
#else
        std::cout << "R" << renderCallCount << std::endl << std::flush;
#endif

        if (!m_tileRenderer || !m_tileProvider || !m_screenSurface) {
            return;
        }

        int windowWidth = 0;
        int windowHeight = 0;
        SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);

        const int tileW = m_cellPixels;
        const int tileH = m_cellPixels / 2;

        if (m_cameraX < 0) m_cameraX = 0;
        if (m_cameraY < 0) m_cameraY = 0;

        const int viewportX = m_cameraX / tileW;
        const int viewportY = m_cameraY / tileH;
        const int viewportWidth = (windowWidth / tileW) + 2;
        const int viewportHeight = (windowHeight / tileH) + 2;

        const int surfaceX = -(m_cameraX % tileW);
        const int surfaceY = -(m_cameraY % tileH);

        // Clear software surface and draw visible tiles.
        m_tileRenderer->ClearSurface(0);
        m_tileRenderer->DrawTiles(
            m_tileProvider,
            viewportX,
            viewportY,
            viewportWidth,
            viewportHeight,
            surfaceX,
            surfaceY
        );

        // Present software surface to SDL renderer.
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_renderer);
        spritectl_present_surface(m_screenSurface->GetBackendSurface(), m_renderer);
        SDL_RenderPresent(m_renderer);
    }

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    CSpriteSurface* m_screenSurface;
    CSpritePack* m_tilePack;
    CSpritePack* m_objPack;
    MZone* m_zone;
    MZoneTileProvider* m_tileProvider;
    TileRenderer* m_tileRenderer;

    int m_cellPixels;
    int m_cameraX;
    int m_cameraY;
    float m_zoom;
    bool m_running;
    int m_frameCount;
    bool m_initialized;
    std::string m_mapFile;
    std::string m_tileSpkFile;
    std::string m_objSpkFile;
    int m_windowWidth;
    int m_windowHeight;
};

/**
 * Demo entry point
 */
int main(int argc, char* argv[]) {
    std::cout << "MAIN" << std::endl << std::flush;

    // Default paths for web demo
    static const char* default_args[] = {
        "DarkEdenWebDemo",
        "Data/Map/adam_c.map",
        "Data/Image/tile.spk",
        "Data/Image/ImageObject.spk"
    };

    // Use default args if not provided
    if (argc < 3) {
        argc = 4;
        argv = (char**)default_args;
        std::cout << "Dark Eden Web Demo - Map Viewer" << std::endl;
        std::cout << "=================================" << std::endl;
        std::cout << "Using default paths:" << std::endl;
        std::cout << "  Map: " << default_args[1] << std::endl;
        std::cout << "  Tiles: " << default_args[2] << std::endl;
        std::cout << "  Objects: " << default_args[3] << std::endl;
        std::cout << std::endl;
    }
    std::cout << "ARGS OK" << std::endl << std::flush;

    const char* mapFile = argv[1];
    const char* tileSpkFile = argv[2];
    const char* objSpkFile = (argc >= 4) ? argv[3] : nullptr;

    std::cout << "CREATING VIEWER" << std::endl << std::flush;
    MapViewerDemo viewer;
    viewer.SetInitParams(mapFile, tileSpkFile, objSpkFile, 800, 600);

#ifndef __EMSCRIPTEN__
    std::cout << "INIT" << std::endl << std::flush;
    if (!viewer.Initialize(mapFile, tileSpkFile, objSpkFile, 800, 600)) {
        std::cerr << "Failed to initialize viewer" << std::endl;
        return 1;
    }
    std::cout << "INIT OK" << std::endl << std::flush;
#endif

    std::cout << "\nControls:" << std::endl;
    std::cout << "  W/A/S/D or Arrow Keys - Pan camera" << std::endl;
    std::cout << "  +/- or Mouse Wheel  - Zoom in/out" << std::endl;
    std::cout << "  1 or Home            - Reset view" << std::endl;
    std::cout << "  ESC                  - Exit\n" << std::endl;

    viewer.Run();

    std::cout << "\nViewer closed" << std::endl;
    return 0;
}
