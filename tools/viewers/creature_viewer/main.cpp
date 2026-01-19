/**
 * @file creature_viewer.cpp
 * @brief Creature Animation Viewer - 使用 Client/SpriteLib 和 Client/framelib
 *
 * 功能：
 * - 加载并显示生物动画（需要 .ispk + .cfpk 文件）
 * - 支持多动作、多方向、多帧动画
 * - 支持阴影显示
 * - 测试 CIndexSpritePack、CCreatureFramePack、CShadowSpritePack
 */

#include "Client/SpriteLib/CIndexSpritePack.h"
#include "Client/SpriteLib/CShadowSprite.h"
#include "Client/SpriteLib/CSpriteSurface.h"
#include "Client/SpriteLib/SpriteLibBackend.h"
#include "Client/framelib/CFramePack.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>

// 动作名称
static const char* ACTION_NAMES[] = {
    "STAND", "WALK", "RUN", "ATTACK", "DAMAGED", "DIE",
    "SKILL1", "SKILL2", "SKILL3", "SKILL4", "SKILL5", "SKILL6", "SKILL7", "SKILL8"
};
static const int MAX_ACTION_NAMES = 14;

// 方向名称
static const char* DIR_NAMES[] = {
    "LEFT", "LEFT-DOWN", "DOWN", "RIGHT-DOWN",
    "RIGHT", "RIGHT-UP", "UP", "LEFT-UP"
};

class CreatureViewer {
public:
    CreatureViewer()
        : m_window(nullptr)
        , m_renderer(nullptr)
        , m_screenSurface(nullptr)
        , m_spritePack(nullptr)
        , m_shadowPack(nullptr)
        , m_framePack(nullptr)
        , m_creatureType(0)
        , m_maxCreatureTypes(0)
        , m_currentAction(0)
        , m_maxActions(0)
        , m_currentDirection(0)
        , m_currentFrame(0)
        , m_maxFrames(0)
        , m_showShadow(true)
        , m_showInfo(true)
        , m_playing(true)
        , m_frameRate(10)
        , m_lastFrameTime(0)
        , m_running(true)
    {
    }

    ~CreatureViewer() {
        Cleanup();
    }

    bool Initialize(const char* ispkFile, const char* cfpkFile,
                    const char* sspkFile, int width, int height) {
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
            "Creature Animation Viewer - C++ Edition",
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

        // Load indexed sprite pack (.ispk)
        m_spritePack = new CIndexSpritePack();
        if (!m_spritePack->LoadFromFile(ispkFile)) {
            std::cerr << "Failed to load sprite pack: " << ispkFile << std::endl;
            return false;
        }

        std::cout << "Loaded " << m_spritePack->GetSize() << " sprites from pack" << std::endl;

        // Load creature frame pack (.cfpk)
        m_framePack = new CCreatureFramePack();
        std::ifstream cfpkStream(cfpkFile, std::ios::binary);
        if (!cfpkStream.is_open()) {
            std::cerr << "Failed to open frame pack: " << cfpkFile << std::endl;
            return false;
        }

        // TODO: Implement LoadFromFile for CCreatureFramePack
        // For now, we'll create a simple frame structure manually
        cfpkStream.close();
        m_maxCreatureTypes = 1; // Placeholder
        m_maxActions = MAX_ACTION_NAMES;

        // Load shadow sprite pack if provided
        if (sspkFile != nullptr && strlen(sspkFile) > 0) {
            m_shadowPack = new CShadowSpritePack();
            // TODO: Load .sspk file
            // m_shadowPack->LoadFromFile(...);
            std::cout << "Shadow pack loading not yet implemented" << std::endl;
        }

        UpdateFrameInfo();

        return true;
    }

    void Run() {
        SDL_Event event;

        while (m_running) {
            // Process events
            while (SDL_PollEvent(&event)) {
                HandleEvent(event);
            }

            // Update animation
            if (m_playing) {
                UpdateAnimation();
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
        if (m_framePack) {
            delete m_framePack;
            m_framePack = nullptr;
        }

        if (m_shadowPack) {
            delete m_shadowPack;
            m_shadowPack = nullptr;
        }

        if (m_spritePack) {
            delete m_spritePack;
            m_spritePack = nullptr;
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

                case SDLK_SPACE:
                    // Toggle animation playback
                    m_playing = !m_playing;
                    break;

                case SDLK_s:
                    // Toggle shadow display
                    m_showShadow = !m_showShadow;
                    break;

                case SDLK_i:
                    // Toggle info
                    m_showInfo = !m_showInfo;
                    break;

                case SDLK_a:
                    // Next action
                    m_currentAction++;
                    if (m_currentAction >= m_maxActions) {
                        m_currentAction = 0;
                    }
                    m_currentFrame = 0;
                    UpdateFrameInfo();
                    break;

                case SDLK_d:
                    // Next direction
                    m_currentDirection++;
                    if (m_currentDirection >= 8) {
                        m_currentDirection = 0;
                    }
                    m_currentFrame = 0;
                    UpdateFrameInfo();
                    break;

                case SDLK_LEFT:
                    // Previous frame
                    if (m_currentFrame > 0) {
                        m_currentFrame--;
                    }
                    break;

                case SDLK_RIGHT:
                    // Next frame
                    if (m_currentFrame < m_maxFrames - 1) {
                        m_currentFrame++;
                    }
                    break;

                default:
                    break;
            }
        }
    }

    void UpdateAnimation() {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 frameInterval = 1000 / m_frameRate;

        if (currentTime - m_lastFrameTime >= frameInterval) {
            m_lastFrameTime = currentTime;

            m_currentFrame++;
            if (m_currentFrame >= m_maxFrames) {
                m_currentFrame = 0; // Loop animation
            }
        }
    }

    void UpdateFrameInfo() {
        // TODO: Get frame count from CCreatureFramePack
        // For now, use placeholder values
        m_maxFrames = 1;
    }

    void Render() {
        // Clear screen
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_renderer);

        // Clear surface to black
        m_screenSurface->FillSurface(0);

        // TODO: Render shadow if enabled
        if (m_showShadow && m_shadowPack != nullptr) {
            // Render shadow sprite
            // This requires getting the shadow frame data from CCreatureFramePack
        }

        // TODO: Render creature sprite
        // For now, we need to:
        // 1. Get sprite ID from CCreatureFramePack[m_creatureType][m_currentAction][m_currentDirection][m_currentFrame]
        // 2. Get cx, cy offsets from the frame
        // 3. Get sprite from CIndexSpritePack using sprite ID
        // 4. Render sprite to surface

        // Placeholder: Render first sprite if available
        if (m_spritePack != nullptr && m_spritePack->GetSize() > 0) {
            CIndexSprite* sprite = &m_spritePack->Get(0);
            if (sprite != nullptr && sprite->IsInit()) {
                int windowWidth, windowHeight;
                SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);

                int spriteWidth = sprite->GetWidth();
                int spriteHeight = sprite->GetHeight();

                int x = (windowWidth - spriteWidth) / 2;
                int y = (windowHeight - spriteHeight) / 2;

                POINT point = { x, y };
                m_screenSurface->BltIndexSprite(&point, sprite);
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
        std::cout << "\rCreature " << (m_creatureType + 1) << "/"
                  << m_maxCreatureTypes
                  << " | Action: " << ACTION_NAMES[m_currentAction % MAX_ACTION_NAMES]
                  << " | Dir: " << DIR_NAMES[m_currentDirection % 8]
                  << " | Frame: " << (m_currentFrame + 1) << "/" << m_maxFrames
                  << " | " << (m_playing ? "PLAYING" : "PAUSED")
                  << " | Shadow: " << (m_showShadow ? "ON" : "OFF")
                  << "     ";
        std::cout.flush();
    }

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    CSpriteSurface* m_screenSurface;
    CIndexSpritePack* m_spritePack;
    CShadowSpritePack* m_shadowPack;
    CCreatureFramePack* m_framePack;

    int m_creatureType;
    int m_maxCreatureTypes;
    int m_currentAction;
    int m_maxActions;
    int m_currentDirection;
    int m_currentFrame;
    int m_maxFrames;
    bool m_showShadow;
    bool m_showInfo;
    bool m_playing;
    int m_frameRate;
    Uint32 m_lastFrameTime;
    bool m_running;
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Creature Animation Viewer - C++ Edition\n";
        std::cout << "=======================================\n\n";
        std::cout << "Usage: " << argv[0] << " <creatures.ispk> <creatures.cfpk> [creatures.sspk]\n\n";
        std::cout << "Controls:\n";
        std::cout << "  SPACE      - Play/Pause animation\n";
        std::cout << "  A          - Next action\n";
        std::cout << "  D          - Next direction\n";
        std::cout << "  LEFT/RIGHT - Previous/Next frame\n";
        std::cout << "  S          - Toggle shadow display\n";
        std::cout << "  I          - Toggle info display\n";
        std::cout << "  ESC        - Exit\n";
        return 1;
    }

    const char* ispkFile = argv[1];
    const char* cfpkFile = argv[2];
    const char* sspkFile = (argc >= 4) ? argv[3] : nullptr;
    const int windowWidth = 800;
    const int windowHeight = 600;

    std::cout << "Creature Animation Viewer - C++ Edition\n";
    std::cout << "=======================================\n\n";
    std::cout << "Loading sprite pack: " << ispkFile << std::endl;
    std::cout << "Loading frame pack: " << cfpkFile << std::endl;
    if (sspkFile != nullptr) {
        std::cout << "Loading shadow pack: " << sspkFile << std::endl;
    }

    CreatureViewer viewer;
    if (!viewer.Initialize(ispkFile, cfpkFile, sspkFile, windowWidth, windowHeight)) {
        std::cerr << "Failed to initialize viewer" << std::endl;
        return 1;
    }

    std::cout << "\nControls:\n";
    std::cout << "  SPACE      - Play/Pause animation\n";
    std::cout << "  A          - Next action\n";
    std::cout << "  D          - Next direction\n";
    std::cout << "  LEFT/RIGHT - Previous/Next frame\n";
    std::cout << "  S          - Toggle shadow display\n";
    std::cout << "  I          - Toggle info\n";
    std::cout << "  ESC        - Exit\n\n";

    viewer.Run();

    std::cout << "\nViewer closed\n";

    return 0;
}
