/**
 * @file item_viewer.cpp
 * @brief Item Viewer - 使用 Client/SpriteLib C++ 库
 *
 * 功能：
 * - 加载并显示 .ispk 索引精灵包文件
 * - 支持导航、缩放、信息显示
 * - 测试 CIndexSpritePack 和 CIndexSprite 的功能
 */

#include "Client/SpriteLib/CIndexSpritePack.h"
#include "Client/SpriteLib/CSpriteSurface.h"
#include "Client/SpriteLib/SpriteLibBackend.h"
#include <SDL2/SDL.h>
#include <iostream>

class ItemViewer {
public:
    ItemViewer()
        : m_window(nullptr)
        , m_renderer(nullptr)
        , m_screenSurface(nullptr)
        , m_itemPack(nullptr)
        , m_currentIndex(0)
        , m_zoom(1.0f)
        , m_showInfo(true)
        , m_running(true)
    {
    }

    ~ItemViewer() {
        Cleanup();
    }

    bool Initialize(const char* ispkFile, int width, int height) {
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
            "Item Viewer - C++ Edition",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_SHOWN
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
        if (!m_screenSurface->Init(width, height)) {
            std::cerr << "Failed to initialize screen surface" << std::endl;
            return false;
        }

        // Load index sprite pack
        m_itemPack = new CIndexSpritePack();
        if (!m_itemPack->LoadFromFile(ispkFile)) {
            std::cerr << "Failed to load item pack: " << ispkFile << std::endl;
            return false;
        }

        std::cout << "Loaded " << m_itemPack->GetSize() << " items from pack" << std::endl;

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
            if (m_showInfo) {
                DisplayInfo();
            }

            // Cap frame rate
            SDL_Delay(16); // ~60 FPS
        }
    }

    void Cleanup() {
        if (m_itemPack) {
            delete m_itemPack;
            m_itemPack = nullptr;
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

        // SDL_Quit is called by spritectl_shutdown()
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

                case SDLK_LEFT:
                    // Previous item
                    if (m_currentIndex > 0) {
                        m_currentIndex--;
                    }
                    break;

                case SDLK_RIGHT:
                    // Next item
                    if (m_currentIndex < m_itemPack->GetSize() - 1) {
                        m_currentIndex++;
                    }
                    break;

                case SDLK_UP:
                    // Zoom in
                    m_zoom += 0.25f;
                    if (m_zoom > 4.0f) m_zoom = 4.0f;
                    break;

                case SDLK_DOWN:
                    // Zoom out
                    m_zoom -= 0.25f;
                    if (m_zoom < 0.25f) m_zoom = 0.25f;
                    break;

                case SDLK_i:
                    // Toggle info
                    m_showInfo = !m_showInfo;
                    break;

                default:
                    break;
            }
        }
    }

    void Render() {
        // Clear screen
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_renderer);

        // Get current item
        if (m_currentIndex >= 0 && m_currentIndex < m_itemPack->GetSize()) {
            CIndexSprite* sprite = &m_itemPack->Get(m_currentIndex);

            if (sprite && sprite->IsInit()) {
                // Clear surface to black
                m_screenSurface->FillSurface(0);

                // Calculate position (centered)
                int spriteWidth = sprite->GetWidth();
                int spriteHeight = sprite->GetHeight();

                int windowWidth, windowHeight;
                SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);

                int scaledWidth = static_cast<int>(spriteWidth * m_zoom);
                int scaledHeight = static_cast<int>(spriteHeight * m_zoom);

                int x = (windowWidth - scaledWidth) / 2;
                int y = (windowHeight - scaledHeight) / 2;

                // Draw sprite to surface
                // Note: Index sprite scaling not directly supported, using 1.0x for now
                POINT point = { x, y };
                m_screenSurface->BltIndexSprite(&point, sprite);

                // Present surface to screen
                PresentSurface();
            }
        }

        SDL_RenderPresent(m_renderer);
    }

    void PresentSurface() {
        // Get backend surface and present to renderer
        spritectl_surface_t surface = m_screenSurface->GetBackendSurface();
        if (!surface) {
            return;
        }

        // Use spritectl_present_surface to render to SDL renderer
        spritectl_present_surface(surface, m_renderer);
    }

    void DisplayInfo() {
        if (m_currentIndex >= 0 && m_currentIndex < m_itemPack->GetSize()) {
            CIndexSprite* sprite = &m_itemPack->Get(m_currentIndex);

            if (sprite && sprite->IsInit()) {
                std::cout << "\rItem " << (m_currentIndex + 1) << "/"
                         << m_itemPack->GetSize()
                         << " | Size: " << sprite->GetWidth() << "x" << sprite->GetHeight()
                         << " | Zoom: " << static_cast<int>(m_zoom * 100) << "%"
                         << "    ";
                std::cout.flush();
            }
        }
    }

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    CSpriteSurface* m_screenSurface;
    CIndexSpritePack* m_itemPack;

    int m_currentIndex;
    float m_zoom;
    bool m_showInfo;
    bool m_running;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Item Viewer - C++ Edition\n";
        std::cout << "==========================\n\n";
        std::cout << "Usage: " << argv[0] << " <items.ispk>\n\n";
        std::cout << "Controls:\n";
        std::cout << "  LEFT/RIGHT - Navigate items\n";
        std::cout << "  UP/DOWN    - Zoom in/out\n";
        std::cout << "  I          - Toggle info display\n";
        std::cout << "  ESC        - Exit\n";
        return 1;
    }

    const char* ispkFile = argv[1];
    const int windowWidth = 800;
    const int windowHeight = 600;

    std::cout << "Item Viewer - C++ Edition\n";
    std::cout << "==========================\n\n";
    std::cout << "Loading item pack: " << ispkFile << std::endl;

    ItemViewer viewer;
    if (!viewer.Initialize(ispkFile, windowWidth, windowHeight)) {
        std::cerr << "Failed to initialize viewer" << std::endl;
        return 1;
    }

    std::cout << "\nControls:\n";
    std::cout << "  LEFT/RIGHT - Navigate items\n";
    std::cout << "  UP/DOWN    - Zoom in/out\n";
    std::cout << "  I          - Toggle info\n";
    std::cout << "  ESC        - Exit\n\n";

    viewer.Run();

    std::cout << "\nViewer closed\n";

    return 0;
}
