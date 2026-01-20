/**
 * @file map_viewer.cpp
 * @brief Map Viewer - 浏览 zone 地图
 *
 * 功能：
 * - 加载并显示 .map zone 文件
 * - 渲染 tile 和 ImageObject
 * - 支持平移、缩放、拖拽
 * - 测试 CSpritePack 和 zone 解析
 */

#include "Client/SpriteLib/CSpritePack.h"
#include "Client/SpriteLib/CSpriteSurface.h"
#include "Client/SpriteLib/SpriteLibBackend.h"
#include "zoneloader.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <cstring>
#include <cstdint>

// 默认配置
#define DEFAULT_CELL_PIXELS 32
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

class MapViewer {
public:
    MapViewer()
        : m_window(nullptr)
        , m_renderer(nullptr)
        , m_screenSurface(nullptr)
        , m_tilePack(nullptr)
        , m_objPack(nullptr)
        , m_zoneLoader(nullptr)
        , m_cellPixels(DEFAULT_CELL_PIXELS)
        , m_cameraX(0)
        , m_cameraY(0)
        , m_zoom(1.0f)
        , m_showGrid(true)
        , m_showObjects(true)
        , m_dragging(false)
        , m_lastMouseX(0)
        , m_lastMouseY(0)
        , m_running(true)
    {
    }

    ~MapViewer() {
        Cleanup();
    }

    bool Initialize(const char* mapFile, const char* tileSpkFile,
                    const char* objSpkFile, int windowWidth, int windowHeight) {
        // Initialize SpriteLib backend FIRST
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
            "Map Viewer - C++ Edition",
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
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

        // Load zone map using ZoneLoader
        m_zoneLoader = new ZoneLoader();
        if (!m_zoneLoader->LoadFromFile(mapFile)) {
            std::cerr << "Failed to load zone map: " << mapFile << std::endl;
            return false;
        }

        // Load tile sprite pack
        m_tilePack = new CSpritePack();
        if (!m_tilePack->LoadFromFile(tileSpkFile)) {
            std::cerr << "Failed to load tile pack: " << tileSpkFile << std::endl;
            return false;
        }

        std::cout << "Loaded " << m_tilePack->GetSize() << " tiles from pack" << std::endl;

        // Load ImageObject sprite pack if provided
        if (objSpkFile != nullptr && strlen(objSpkFile) > 0) {
            m_objPack = new CSpritePack();
            if (!m_objPack->LoadFromFile(objSpkFile)) {
                std::cout << "Warning: failed to load ImageObject pack: " << objSpkFile << std::endl;
                delete m_objPack;
                m_objPack = nullptr;
            } else {
                std::cout << "Loaded " << m_objPack->GetSize() << " ImageObjects from pack" << std::endl;
            }
        }

        // Center camera on map
        int mapWidth = m_zoneLoader->GetWidth();
        int mapHeight = m_zoneLoader->GetHeight();
        m_cameraX = (mapWidth * m_cellPixels) / 2 - windowWidth / 2;
        m_cameraY = (mapHeight * m_cellPixels / 2) / 2 - windowHeight / 2;

        return true;
    }

    void Run() {
        SDL_Event event;

        while (m_running) {
            // Process events
            while (SDL_PollEvent(&event)) {
                HandleEvent(event);
            }

            // Render
            Render();

            // Display info
            DisplayInfo();

            // Cap frame rate
            SDL_Delay(16); // ~60 FPS
        }
    }

    void Cleanup() {
        if (m_zoneLoader) {
            delete m_zoneLoader;
            m_zoneLoader = nullptr;
        }

        if (m_objPack) {
            delete m_objPack;
            m_objPack = nullptr;
        }

        if (m_tilePack) {
            delete m_tilePack;
            m_tilePack = nullptr;
        }

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

        // Shutdown SpriteLib backend LAST
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

                case SDLK_g:
                    // Toggle grid
                    m_showGrid = !m_showGrid;
                    break;

                case SDLK_i:
                    // Toggle objects
                    m_showObjects = !m_showObjects;
                    break;

                case SDLK_1:
                case SDLK_HOME:
                {
                    // Reset view
                    int windowWidth, windowHeight;
                    SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);
                    int mapWidth = m_zoneLoader->GetWidth();
                    int mapHeight = m_zoneLoader->GetHeight();
                    m_cameraX = (mapWidth * m_cellPixels) / 2 - windowWidth / 2;
                    m_cameraY = (mapHeight * m_cellPixels / 2) / 2 - windowHeight / 2;
                    m_zoom = 1.0f;
                    break;
                }

                case SDLK_w:
                case SDLK_UP:
                    // Pan up
                    m_cameraY -= 50;
                    break;

                case SDLK_s:
                case SDLK_DOWN:
                    // Pan down
                    m_cameraY += 50;
                    break;

                case SDLK_a:
                case SDLK_LEFT:
                    // Pan left
                    m_cameraX -= 50;
                    break;

                case SDLK_d:
                case SDLK_RIGHT:
                    // Pan right
                    m_cameraX += 50;
                    break;

                case SDLK_EQUALS:
                case SDLK_PLUS:
                case SDLK_KP_PLUS:
                    // Zoom in
                    m_zoom *= 1.1f;
                    if (m_zoom > 4.0f) m_zoom = 4.0f;
                    break;

                case SDLK_MINUS:
                case SDLK_KP_MINUS:
                    // Zoom out
                    m_zoom /= 1.1f;
                    if (m_zoom < 0.25f) m_zoom = 0.25f;
                    break;

                default:
                    break;
            }
        }
        else if (event.type == SDL_MOUSEWHEEL) {
            // Zoom with mouse wheel
            if (event.wheel.y > 0) {
                m_zoom *= 1.1f;
                if (m_zoom > 4.0f) m_zoom = 4.0f;
            } else if (event.wheel.y < 0) {
                m_zoom /= 1.1f;
                if (m_zoom < 0.25f) m_zoom = 0.25f;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_RIGHT) {
                // Start dragging
                m_dragging = true;
                m_lastMouseX = event.button.x;
                m_lastMouseY = event.button.y;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_RIGHT) {
                // Stop dragging
                m_dragging = false;
            }
        }
        else if (event.type == SDL_MOUSEMOTION) {
            if (m_dragging) {
                // Drag camera
                m_cameraX -= (event.motion.x - m_lastMouseX);
                m_cameraY -= (event.motion.y - m_lastMouseY);
                m_lastMouseX = event.motion.x;
                m_lastMouseY = event.motion.y;
            }
        }
        else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                // Resize surface
                m_screenSurface->Release();
                m_screenSurface->Init(event.window.data1, event.window.data2);
            }
        }
    }

    void Render() {
        // Clear screen
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_renderer);

        // Clear surface to black
        m_screenSurface->FillSurface(0);

        // Get window size
        int windowWidth, windowHeight;
        SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);

        // Get map dimensions
        int mapWidth = m_zoneLoader->GetWidth();
        int mapHeight = m_zoneLoader->GetHeight();

        // Calculate visible range
        int scaledCellW = static_cast<int>(m_cellPixels * m_zoom);
        int scaledCellH = static_cast<int>(m_cellPixels / 2 * m_zoom);

        int startSectorX = m_cameraX / scaledCellW;
        int startSectorY = m_cameraY / scaledCellH;
        int endSectorX = (m_cameraX + windowWidth) / scaledCellW + 1;
        int endSectorY = (m_cameraY + windowHeight) / scaledCellH + 1;

        // Clamp to map bounds
        startSectorX = (startSectorX < 0) ? 0 : startSectorX;
        startSectorY = (startSectorY < 0) ? 0 : startSectorY;
        endSectorX = (endSectorX > mapWidth) ? mapWidth : endSectorX;
        endSectorY = (endSectorY > mapHeight) ? mapHeight : endSectorY;

        // Render visible sectors
        for (int y = startSectorY; y < endSectorY; y++) {
            for (int x = startSectorX; x < endSectorX; x++) {
                const Sector* sector = m_zoneLoader->GetSector(x, y);
                if (!sector) continue;

                // Calculate screen position
                int screenX = x * scaledCellW - m_cameraX;
                int screenY = y * scaledCellH - m_cameraY;

                // Render tile
                if (sector->spriteID > 0 && sector->spriteID <= m_tilePack->GetSize()) {
                    CSprite* sprite = &m_tilePack->Get(sector->spriteID - 1); // spriteID is 1-based
                    if (sprite != nullptr && sprite->IsInit()) {
                        POINT point = { screenX, screenY };
                        m_screenSurface->BltSprite(&point, sprite);
                    }
                }

                // Draw grid if enabled
                if (m_showGrid) {
                    // Grid rendering would require direct surface access
                    // Skip for now
                }
            }
        }

        // Present surface to screen
        PresentSurface();

        SDL_RenderPresent(m_renderer);
    }

    void PresentSurface() {
        spritectl_surface_t surface = m_screenSurface->GetBackendSurface();
        if (!surface) {
            return;
        }

        spritectl_present_surface(surface, m_renderer);
    }

    void DisplayInfo() {
        static int frameCount = 0;
        frameCount++;

        if (frameCount % 30 != 0) {
            return; // Update every 30 frames
        }

        int mapWidth = m_zoneLoader->GetWidth();
        int mapHeight = m_zoneLoader->GetHeight();

        std::cout << "\rMap: " << mapWidth << "x" << mapHeight
                  << " | Camera: (" << m_cameraX << ", " << m_cameraY << ")"
                  << " | Zoom: " << static_cast<int>(m_zoom * 100) << "%"
                  << " | Grid: " << (m_showGrid ? "ON" : "OFF")
                  << " | Objects: " << (m_showObjects ? "ON" : "OFF")
                  << "     ";
        std::cout.flush();
    }

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    CSpriteSurface* m_screenSurface;
    CSpritePack* m_tilePack;
    CSpritePack* m_objPack;
    ZoneLoader* m_zoneLoader;

    // Rendering settings
    int m_cellPixels;
    int m_cameraX;
    int m_cameraY;
    float m_zoom;
    bool m_showGrid;
    bool m_showObjects;

    // Mouse state
    bool m_dragging;
    int m_lastMouseX;
    int m_lastMouseY;

    bool m_running;
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Map Viewer - C++ Edition\n";
        std::cout << "=========================\n\n";
        std::cout << "Usage: " << argv[0] << " <zone.map> <tile.spk> [imageobject.spk] [--cell N]\n\n";
        std::cout << "Example:\n";
        std::cout << "  " << argv[0] << " DarkEden/Data/Map/adam_c.map DarkEden/Data/Image/tile.spk\n";
        std::cout << "  " << argv[0] << " DarkEden/Data/Map/adam_c.map DarkEden/Data/Image/tile.spk DarkEden/Data/Image/ImageObject.spk\n\n";
        std::cout << "Controls:\n";
        std::cout << "  W/A/S/D or Arrow Keys - Pan camera\n";
        std::cout << "  +/- or Mouse Wheel  - Zoom in/out\n";
        std::cout << "  Right Mouse Drag     - Drag camera\n";
        std::cout << "  G                    - Toggle grid\n";
        std::cout << "  I                    - Toggle objects\n";
        std::cout << "  1 or Home            - Reset view\n";
        std::cout << "  ESC                  - Exit\n";
        return 1;
    }

    const char* mapFile = argv[1];
    const char* tileSpkFile = argv[2];
    const char* objSpkFile = nullptr;
    int cellPixels = DEFAULT_CELL_PIXELS;

    // Parse optional arguments
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "--cell") == 0 && (i + 1) < argc) {
            cellPixels = atoi(argv[i + 1]);
            if (cellPixels <= 0) cellPixels = DEFAULT_CELL_PIXELS;
            i++;
        } else if (argv[i][0] != '-') {
            objSpkFile = argv[i];
        }
    }

    std::cout << "Map Viewer - C++ Edition\n";
    std::cout << "=========================\n\n";
    std::cout << "Loading map: " << mapFile << std::endl;
    std::cout << "Loading tiles: " << tileSpkFile << std::endl;
    if (objSpkFile != nullptr) {
        std::cout << "Loading objects: " << objSpkFile << std::endl;
    }
    std::cout << std::endl;

    MapViewer viewer;
    if (!viewer.Initialize(mapFile, tileSpkFile, objSpkFile,
                          DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)) {
        std::cerr << "Failed to initialize viewer" << std::endl;
        return 1;
    }

    std::cout << "\nControls:\n";
    std::cout << "  W/A/S/D or Arrow Keys - Pan camera\n";
    std::cout << "  +/- or Mouse Wheel  - Zoom in/out\n";
    std::cout << "  Right Mouse Drag     - Drag camera\n";
    std::cout << "  G                    - Toggle grid\n";
    std::cout << "  I                    - Toggle objects\n";
    std::cout << "  1 or Home            - Reset view\n";
    std::cout << "  ESC                  - Exit\n\n";

    viewer.Run();

    std::cout << "\nViewer closed\n";

    return 0;
}
