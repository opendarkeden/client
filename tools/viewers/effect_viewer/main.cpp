/**
 * @file effect_viewer.cpp
 * @brief Effect Viewer - Test and debug game effects
 *
 * Features:
 * - Load and render effect resources (.efpk, .spk, .aspk, .ppk)
 * - Test MEffect classes with dependency injection
 * - Interactive controls (switch effects, pause, step frames)
 * - Built-in tests for validation
 *
 * Architecture:
 * - Uses refactored EffectResourceContainer (no global state)
 * - Links directly to game code (MEffect, MLinearEffect, etc.)
 * - Standalone SDL2 rendering (minimal dependencies)
 */

#include "Client/EffectResourceContainer.h"
#include "Client/MEffect.h"
#include "Client/SpriteLib/SpriteLibBackend.h"
#include "EffectSpriteTypeTable.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <cstring>

// Window configuration
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

class EffectViewer {
public:
    EffectViewer()
        : m_window(nullptr)
        , m_renderer(nullptr)
        , m_surface(nullptr)
        , m_texture(nullptr)
        , m_resources(nullptr)
        , m_effectSpriteTypeTable(nullptr)
        , m_currentEffect(nullptr)
        , m_effectType(0)
        , m_direction(0)
        , m_paused(false)
        , m_running(true)
        , m_frameCount(0)
        , m_showHelp(true)
    {
    }

    ~EffectViewer() {
        Cleanup();
    }

    bool Initialize(const char* dataPath, int windowWidth, int windowHeight) {
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
            "Effect Viewer - OpenDarkEden",
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
        m_renderer = SDL_CreateRenderer(m_window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!m_renderer) {
            std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
            return false;
        }

        // Create a surface for rendering (RGB565 format)
        m_surface = SDL_CreateRGBSurface(0, windowWidth, windowHeight, 16,
            0xF800, // Red mask
            0x07E0, // Green mask
            0x001F, // Blue mask
            0x0000  // Alpha mask
        );
        if (!m_surface) {
            std::cerr << "SDL_CreateRGBSurface failed: " << SDL_GetError() << std::endl;
            return false;
        }

        // Create texture from surface
        m_texture = SDL_CreateTexture(m_renderer,
            SDL_PIXELFORMAT_RGB565,
            SDL_TEXTUREACCESS_STREAMING,
            windowWidth, windowHeight
        );
        if (!m_texture) {
            std::cerr << "SDL_CreateTexture failed: " << SDL_GetError() << std::endl;
            return false;
        }

        // Load effect resources
        m_resources = new EffectResourceContainer();
        if (!m_resources->Load(dataPath)) {
            std::cerr << "Failed to load effect resources from: " << dataPath << std::endl;
            return false;
        }

        std::cout << "Effect resources loaded successfully!" << std::endl;
        std::cout << "  Alpha FPK: " << m_resources->m_EffectAlphaFPK.GetSize() << " effects" << std::endl;
        std::cout << "  Normal FPK: " << m_resources->m_EffectNormalFPK.GetSize() << " effects" << std::endl;
        std::cout << "  Shadow FPK: " << m_resources->m_EffectShadowFPK.GetSize() << " effects" << std::endl;
        std::cout << "  Screen FPK: " << m_resources->m_EffectScreenFPK.GetSize() << " effects" << std::endl;

        // Load EffectSpriteType table
        m_effectSpriteTypeTable = new EffectSpriteTypeTable();
        std::string infoPath = std::string(dataPath);
        // Remove "Image/" from path to get "Data/Info/"
        size_t pos = infoPath.find("Image/");
        if (pos != std::string::npos) {
            infoPath.replace(pos, 6, "Info/");
        }
        std::string tablePath = infoPath + "EffectSpriteType.inf";

        std::cout << "\nLoading EffectSpriteType table from: " << tablePath << std::endl;
        if (!m_effectSpriteTypeTable->LoadFromFile(tablePath.c_str())) {
            std::cerr << "Warning: Failed to load EffectSpriteType table, will use defaults" << std::endl;
        }

        // Don't create default effect - let user choose with LEFT/RIGHT keys
        std::cout << "\n========================================" << std::endl;
        std::cout << "Ready! Use LEFT/RIGHT keys to select an effect." << std::endl;
        std::cout << "========================================" << std::endl;

        return true;
    }

    void Cleanup() {
        if (m_currentEffect) {
            delete m_currentEffect;
            m_currentEffect = nullptr;
        }

        if (m_resources) {
            delete m_resources;
            m_resources = nullptr;
        }

        if (m_texture) {
            SDL_DestroyTexture(m_texture);
            m_texture = nullptr;
        }

        if (m_surface) {
            SDL_FreeSurface(m_surface);
            m_surface = nullptr;
        }

        if (m_renderer) {
            SDL_DestroyRenderer(m_renderer);
            m_renderer = nullptr;
        }

        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }

        SDL_Quit();
    }

    void CreateEffect(int effectType) {
        // Delete old effect
        if (m_currentEffect) {
            delete m_currentEffect;
            m_currentEffect = nullptr;
        }

        m_effectType = effectType;
        m_frameCount = 0;

        // Get BltType from EffectSpriteType table (matches original game)
        BYTE bltType = BLT_EFFECT;  // Default
        if (m_effectSpriteTypeTable && m_effectSpriteTypeTable->IsLoaded()) {
            bltType = (BYTE)m_effectSpriteTypeTable->GetBltType(effectType);

            const char* bltTypeName = "";
            switch (bltType) {
                case 0: bltTypeName = "BLT_NORMAL"; break;
                case 1: bltTypeName = "BLT_EFFECT"; break;
                case 2: bltTypeName = "BLT_SHADOW"; break;
                case 3: bltTypeName = "BLT_SCREEN"; break;
                default: bltTypeName = "UNKNOWN"; break;
            }

            std::cout << "Effect " << effectType << " -> BltType=" << (int)bltType
                      << " (" << bltTypeName << ") from EffectSpriteType table" << std::endl;
        } else {
            std::cout << "⚠️  EffectSpriteType table not loaded, using BLT_EFFECT as default" << std::endl;
        }

        // Create new effect with dependency injection
        m_currentEffect = new MEffect(bltType, m_resources);

        // Set effect properties
        // TODO: Determine correct maxFrame from resource
        BYTE maxFrame = 4;
        m_currentEffect->SetFrameID(effectType, maxFrame);
        m_currentEffect->SetDirection(m_direction);

        // Set position (center of screen)
        int centerX, centerY;
        SDL_GetWindowSize(m_window, &centerX, &centerY);
        m_currentEffect->SetPixelPosition(centerX / 2, centerY / 2, 0);

        // Set duration (in frames)
        m_currentEffect->SetCount(100);

        std::cout << "\n========================================" << std::endl;
        std::cout << "Created Effect Type " << effectType
                  << " (BltType=" << (int)bltType << ")" << std::endl;
        std::cout << "FrameID=" << effectType
                  << " | MaxFrame=" << (int)maxFrame
                  << " | Direction=" << (int)m_direction << std::endl;
        std::cout << "========================================" << std::endl;
    }

    void RunTests() {
        std::cout << "\n=== Running Effect Tests ===" << std::endl;

        // Save current state
        bool wasPaused = m_paused;
        m_paused = true;  // Pause for testing

        int passCount = 0;
        int failCount = 0;

        // Test 1: Resource loading
        if (m_resources && m_resources->IsLoaded()) {
            std::cout << "[PASS] Resources loaded" << std::endl;
            passCount++;
        } else {
            std::cout << "[FAIL] Resources not loaded" << std::endl;
            failCount++;
        }

        // Test 2: Boundary checking
        CEffectFrame* frame = m_resources->GetEffectFrame(BLT_EFFECT, 99999, 0, 0);
        if (frame == nullptr) {
            std::cout << "[PASS] Boundary check working" << std::endl;
            passCount++;
        } else {
            std::cout << "[FAIL] Boundary check not working" << std::endl;
            failCount++;
        }

        // Test 3: MEffect creation with dependency injection
        if (m_currentEffect != nullptr) {
            std::cout << "[PASS] MEffect created with dependency injection" << std::endl;
            passCount++;
        } else {
            std::cout << "[FAIL] MEffect creation failed" << std::endl;
            failCount++;
        }

        // Test 4: Animation frames (reset and test)
        if (m_currentEffect) {
            // Reset to frame 0
            m_currentEffect->SetFrameID(m_effectType, 4);

            int initialFrame = m_currentEffect->GetFrame();
            if (initialFrame == 0) {
                std::cout << "[PASS] Initial frame is 0" << std::endl;
                passCount++;

                // Test frame progression
                m_currentEffect->NextFrame();
                int nextFrame = m_currentEffect->GetFrame();
                if (nextFrame == 1) {
                    std::cout << "[PASS] Frame progression works (0->1)" << std::endl;
                    passCount++;
                } else {
                    std::cout << "[FAIL] Frame progression wrong: " << nextFrame << std::endl;
                    failCount++;
                }
            } else {
                std::cout << "[FAIL] Initial frame wrong: " << initialFrame << std::endl;
                failCount++;
            }
        } else {
            std::cout << "[SKIP] Frame test (no effect)" << std::endl;
        }

        std::cout << "=== Tests Complete: " << passCount << " pass, "
                  << failCount << " fail ===" << std::endl << std::endl;

        // Restore state
        m_paused = wasPaused;
    }

    void TestPanicScenario() {
        std::cout << "\n========================================" << std::endl;
        std::cout << "=== Panic Reproduction Test ===" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "This test simulates the game's panic scenario:" << std::endl;
        std::cout << "- Game crash at MTopView.cpp:19368 in DrawAttachEffect" << std::endl;
        std::cout << "- Accessing (*g_pEffectSpriteTypeTable)[est] where est out of bounds" << std::endl;
        std::cout << "- est = GET_EFFECTSPRITETYPE_SCREEN(frameID)" << std::endl;
        std::cout << std::endl;

        if (!m_effectSpriteTypeTable) {
            std::cout << "ERROR: EffectSpriteTypeTable not loaded!" << std::endl;
            return;
        }

        // Test various EffectIDs to find problematic ones
        std::cout << "Testing various EffectIDs for out-of-bounds access:" << std::endl;
        std::cout << std::endl;

        int testEffectIDs[] = {0, 74, 100, 500, 1000, 1500, 1900, 1947, 2000, 2500, 2600, 2650};
        int numTests = sizeof(testEffectIDs) / sizeof(testEffectIDs[0]);

        for (int i = 0; i < numTests; i++) {
            TYPE_FRAMEID frameID = testEffectIDs[i];
            int bltType = m_effectSpriteTypeTable->GetBltType(frameID);
            int tableIndex = EffectSpriteTypeTable::GetEffectSpriteTypeIndex(frameID, bltType);

            std::cout << "EffectID=" << frameID << " (BltType=" << bltType << "):" << std::endl;
            std::cout << "  Table index: " << tableIndex;

            if (m_effectSpriteTypeTable->IsValidIndex(tableIndex)) {
                std::cout << " ✅ OK" << std::endl;
            } else {
                std::cout << " ❌ OUT OF BOUNDS!" << std::endl;
                std::cout << "  --> This would cause heap-buffer-overflow!" << std::endl;
            }
            std::cout << std::endl;
        }

        // Now test the specific crash scenario
        std::cout << "========================================" << std::endl;
        std::cout << "Testing BLT_SCREEN offset calculation:" << std::endl;
        std::cout << "GET_EFFECTSPRITETYPE_SCREEN(frameID) = frameID + " << EffectSpriteTypeTable::GetMaxAlphaEffect() << std::endl;
        std::cout << std::endl;

        // Find the highest EffectID in the table
        TYPE_FRAMEID maxEffectID = m_effectSpriteTypeTable->GetMaxEffectID();

        std::cout << "Highest EffectID in table: " << maxEffectID << std::endl;
        int worstCaseIndex = EffectSpriteTypeTable::GetEffectSpriteTypeIndex(maxEffectID, 3);  // BLT_SCREEN
        std::cout << "Worst case BLT_SCREEN index: " << worstCaseIndex << std::endl;
        std::cout << "Table size: " << m_effectSpriteTypeTable->GetSize() << std::endl;

        if (worstCaseIndex >= m_effectSpriteTypeTable->GetSize()) {
            std::cout << "❌ PANIC: Highest EffectID would cause overflow!" << std::endl;
            std::cout << "   Overflow by: " << (worstCaseIndex - m_effectSpriteTypeTable->GetSize() + 1) << " entries" << std::endl;
        } else {
            std::cout << "✅ Safe: All EffectIDs within bounds" << std::endl;
        }

        std::cout << "========================================" << std::endl;
        std::cout << "To reproduce a specific panic:" << std::endl;
        std::cout << "1. Note the EffectID from the game crash" << std::endl;
        std::cout << "2. Use LEFT/RIGHT keys to select that EffectID" << std::endl;
        std::cout << "3. Watch the debug output for table access info" << std::endl;
        std::cout << "========================================\n" << std::endl;
    }

    bool HandleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    m_running = false;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            m_running = false;
                            break;

                        case SDLK_SPACE:
                            m_paused = !m_paused;
                            std::cout << (m_paused ? "Paused" : "Resumed") << std::endl;
                            break;

                        case SDLK_h:
                            m_showHelp = !m_showHelp;
                            break;

                        case SDLK_1:
                        case SDLK_2:
                        case SDLK_3:
                        case SDLK_4:
                        case SDLK_5:
                        case SDLK_6:
                        case SDLK_7:
                        case SDLK_8:
                        case SDLK_9:
                            {
                                int newEffect = event.key.keysym.sym - SDLK_1;
                                CreateEffect(newEffect);
                            }
                            break;

                        case SDLK_LEFT:
                            if (m_effectType > 0) {
                                CreateEffect(m_effectType - 1);
                            }
                            break;

                        case SDLK_RIGHT:
                            CreateEffect(m_effectType + 1);
                            break;

                        case SDLK_UP:
                            {
                                // Decrement direction (wrap around 0-7)
                                m_direction = (m_direction == 0) ? 7 : (m_direction - 1);
                                if (m_currentEffect) {
                                    m_currentEffect->SetDirection(m_direction);
                                }
                                std::cout << "Direction: " << (int)m_direction << " (UP)" << std::endl;
                            }
                            break;

                        case SDLK_DOWN:
                            {
                                // Increment direction (wrap around 0-7)
                                m_direction = (m_direction + 1) % 8;
                                if (m_currentEffect) {
                                    m_currentEffect->SetDirection(m_direction);
                                }
                                std::cout << "Direction: " << (int)m_direction << " (DOWN)" << std::endl;
                            }
                            break;

                        case SDLK_t:
                            RunTests();
                            break;

                        case SDLK_p:
                            // Panic test mode - Test the specific EffectID that caused the game crash
                            TestPanicScenario();
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

        return m_running;
    }

    void Update() {
        if (!m_paused && m_currentEffect) {
            int oldFrame = m_currentEffect->GetFrame();
            m_currentEffect->Update();
            int newFrame = m_currentEffect->GetFrame();
            m_frameCount++;

            // Debug: Log detailed info for first 60 updates (1 second at 60fps)
            if (m_frameCount <= 10) {
                TYPE_FRAMEID frameID = m_currentEffect->GetFrameID();
                BYTE direction = m_currentEffect->GetDirection();
                BYTE maxFrame = m_currentEffect->GetMaxFrame();
                std::cout << "Update #" << m_frameCount
                          << " | EffectID=" << m_effectType
                          << " | FrameID=" << frameID
                          << " | Frame=" << oldFrame << "->" << newFrame
                          << " | MaxFrame=" << (int)maxFrame
                          << " | Dir=" << (int)direction
                          << " | paused=" << m_paused << std::endl;
            }

            // Check if effect ended
            if (m_currentEffect->IsEnd()) {
                // Loop the effect by resetting frame ID
                std::cout << "Effect " << m_effectType << " ended, looping..." << std::endl;
                m_currentEffect->SetFrameID(m_effectType, 4);
            }
        }
    }

    void Render() {
        // Clear surface with black
        SDL_FillRect(m_surface, nullptr, SDL_MapRGB(m_surface->format, 0, 0, 0));

        // Debug: Track render calls
        static int renderCallCount = 0;
        renderCallCount++;

        // Render effect (only once per frame)
        if (m_currentEffect && m_resources) {
            TYPE_FRAMEID frameID = m_currentEffect->GetFrameID();
            BYTE direction = m_currentEffect->GetDirection();
            BYTE frame = m_currentEffect->GetFrame();
            BYTE bltType = m_currentEffect->GetBltType();

            // DEBUG: Simulate game's EffectSpriteType table access
            // This helps correlate game panics with effect_viewer behavior
            if (m_effectSpriteTypeTable && m_frameCount <= 3) {
                int tableIndex = EffectSpriteTypeTable::GetEffectSpriteTypeIndex(frameID, bltType);
                std::cout << "\n--- EffectSpriteType Table Access ---" << std::endl;
                std::cout << "Game would access table index: " << tableIndex << std::endl;
                std::cout << "  frameID=" << frameID << " + offset=";
                switch (bltType) {
                    case 0: std::cout << EffectSpriteTypeTable::GetMaxScreenEffect() << " (SCREEN->NORMAL)"; break;
                    case 1: std::cout << "0 (EFFECT)"; break;
                    case 2: std::cout << EffectSpriteTypeTable::GetMaxNormalEffect() << " (NORMAL->SHADOW)"; break;
                    case 3: std::cout << EffectSpriteTypeTable::GetMaxAlphaEffect() << " (ALPHA->SCREEN)"; break;
                }
                std::cout << std::endl;

                // Check if this would cause a panic
                if (!m_effectSpriteTypeTable->IsValidIndex(tableIndex)) {
                    std::cout << "  ⚠️  WARNING: This index is OUT OF BOUNDS!" << std::endl;
                    std::cout << "  Table size: " << m_effectSpriteTypeTable->GetSize() << std::endl;
                    std::cout << "  This would cause heap-buffer-overflow in the game!" << std::endl;

                    // Show full debug info
                    m_effectSpriteTypeTable->DebugAccess(tableIndex, "EffectViewer::Render");
                } else {
                    std::cout << "  ✅ Index is valid (table size: " << m_effectSpriteTypeTable->GetSize() << ")" << std::endl;
                }
                std::cout << "---------------------------------------\n" << std::endl;
            }

            // Get effect frame from resource container
            CEffectFrame* pEffectFrame = nullptr;
            if (bltType == BLT_EFFECT) {
                pEffectFrame = m_resources->GetEffectFrame(BLT_EFFECT, frameID, direction, frame);
            } else if (bltType == BLT_NORMAL) {
                pEffectFrame = m_resources->GetEffectFrame(BLT_NORMAL, frameID, direction, frame);
            } else if (bltType == BLT_SCREEN) {
                pEffectFrame = m_resources->GetEffectFrame(BLT_SCREEN, frameID, direction, frame);
            } else if (bltType == BLT_SHADOW) {
                pEffectFrame = m_resources->GetEffectFrame(BLT_SHADOW, frameID, direction, frame);
            }

            if (pEffectFrame) {
                int spriteID = pEffectFrame->GetSpriteID();

                // Debug: Log detailed rendering info (show for first 3 frames of each effect)
                if (m_frameCount <= 3) {
                    short cx = pEffectFrame->GetCX();
                    short cy = pEffectFrame->GetCY();
                    std::cout << "\n=== Render Info ===" << std::endl;
                    std::cout << "EffectID=" << m_effectType
                              << " | FrameID=" << frameID
                              << " | Frame=" << (int)frame
                              << " | Dir=" << (int)direction
                              << " | BltType=" << (int)bltType << std::endl;
                    std::cout << "SpriteID=" << spriteID
                              << " | Offset=(" << cx << "," << cy << ")" << std::endl;
                }

                if (spriteID != SPRITEID_NULL) {
                    // Calculate screen position (center of screen)
                    int centerX, centerY;
                    SDL_GetWindowSize(m_window, &centerX, &centerY);

                    POINT point;
                    point.x = (centerX / 2) + pEffectFrame->GetCX();
                    point.y = (centerY / 2) + pEffectFrame->GetCY();

                    // Debug: Confirm single render
                    if (m_frameCount <= 3) {
                        std::cout << "  >>> Rendering SINGLE sprite at (" << point.x << "," << point.y << ")" << std::endl;
                    }

                    // Render based on bltType
                    if (bltType == BLT_EFFECT) {
                        RenderAlphaSprite(point.x, point.y, spriteID);
                    } else if (bltType == BLT_NORMAL) {
                        RenderNormalSprite(point.x, point.y, spriteID);
                    } else if (bltType == BLT_SCREEN) {
                        RenderScreenSprite(point.x, point.y, spriteID);
                    } else if (bltType == BLT_SHADOW) {
                        RenderShadowSprite(point.x, point.y, spriteID);
                    }
                } else {
                    // SPRITEID_NULL - this frame has no sprite
                    if (m_frameCount <= 3) {
                        std::cout << "Effect " << m_effectType << " Frame " << (int)frame
                                  << " Dir " << (int)direction << " has SPRITEID_NULL (intentional empty frame)" << std::endl;
                    }
                }
            } else {
                // Effect frame not found - show error indicator
                if (m_frameCount <= 3) {
                    std::cout << "\n⚠️  EffectFrame NOT FOUND:" << std::endl;
                    std::cout << "   EffectID=" << m_effectType
                              << " | FrameID=" << frameID
                              << " | Frame=" << (int)frame
                              << " | Dir=" << (int)direction
                              << " | BltType=" << (int)bltType << std::endl;
                }

                // Draw red error rectangle
                SDL_Rect rect;
                rect.w = 200;
                rect.h = 20;
                rect.x = 10;
                rect.y = 10;
                SDL_FillRect(m_surface, &rect, SDL_MapRGB(m_surface->format, 255, 0, 0));
            }
        }

        // Update texture from surface
        SDL_UpdateTexture(m_texture, nullptr, m_surface->pixels, m_surface->pitch);

        // Clear renderer and present texture
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
        SDL_RenderPresent(m_renderer);

        // Log status periodically (every 60 frames = 1 second)
        if (m_frameCount % 60 == 0) {
            if (m_currentEffect) {
                std::cout << "Effect " << m_effectType << " | Frame " << (int)m_currentEffect->GetFrame()
                          << " | Dir " << (int)m_direction << " | Render call #" << renderCallCount << std::endl;
            } else {
                std::cout << "No effect loaded. Use LEFT/RIGHT to select." << std::endl;
            }
        }
    }

    void RenderAlphaSprite(int x, int y, int spriteID) {
        // Get alpha sprite from resource container
        CAlphaSpritePal* pSprite = m_resources->GetAlphaSprite(spriteID);

        if (!pSprite) {
            if (m_frameCount <= 3) {
                std::cout << "  ❌ Failed to get AlphaSprite ID=" << spriteID << std::endl;
                std::cout << "     Total alpha sprites available: "
                          << m_resources->m_EffectAlphaSPK.GetSize() << std::endl;
            }
            return;
        }

        // Check if sprite is initialized
        if (!pSprite->IsInit()) {
            if (m_frameCount <= 3) {
                std::cout << "  ⚠️  AlphaSprite ID=" << spriteID << " not initialized" << std::endl;
            }
            return;
        }

        // Lock surface for direct pixel access
        if (SDL_MUSTLOCK(m_surface)) {
            SDL_LockSurface(m_surface);
        }

        // Calculate destination pointer (offset by x,y)
        WORD* pDest = (WORD*)m_surface->pixels;
        pDest += (y * m_surface->pitch / 2) + x;

        // Get palette
        TYPE_FRAMEID frameID = m_currentEffect->GetFrameID();
        MPalette* pPalette = &(m_resources->m_EffectAlphaPPK[frameID]);

        if (!pPalette) {
            if (m_frameCount <= 3) {
                std::cout << "  ❌ Failed to get palette for FrameID=" << frameID << std::endl;
            }
        } else {
            // Log successful render
            if (m_frameCount <= 3) {
                std::cout << "  ✅ Rendering AlphaSprite ID=" << spriteID
                          << " at (" << x << "," << y << ")" << std::endl;
                std::cout << "     Palette FrameID=" << frameID
                          << " | Pitch=" << m_surface->w << std::endl;
            }

            // Do the actual blit
            // Note: pitch is in bytes (m_surface->pitch), not WORDs
            pSprite->Blt(pDest, m_surface->pitch, *pPalette);
        }

        if (SDL_MUSTLOCK(m_surface)) {
            SDL_UnlockSurface(m_surface);
        }
    }

    void RenderNormalSprite(int x, int y, int spriteID) {
        // Get normal sprite directly from sprite pack
        CSpritePack& spritePack = m_resources->m_EffectNormalSPK;

        // Boundary check
        if (spriteID >= spritePack.GetSize()) {
            if (m_frameCount <= 3) {
                std::cout << "  ❌ Sprite ID " << spriteID << " out of range (size="
                          << spritePack.GetSize() << ")" << std::endl;
            }
            return;
        }

        // Get sprite reference (this triggers lazy loading if needed)
        CSprite& sprite = spritePack[spriteID];

        if (!sprite.IsInit()) {
            if (m_frameCount <= 3) {
                std::cout << "  ⚠️  NormalSprite ID=" << spriteID << " not initialized" << std::endl;
            }
            return;
        }

        // Lock surface for direct pixel access
        if (SDL_MUSTLOCK(m_surface)) {
            SDL_LockSurface(m_surface);
        }

        // Calculate destination pointer (offset by x,y)
        WORD* pDest = (WORD*)m_surface->pixels;
        pDest += (y * m_surface->pitch / 2) + x;

        // Log successful render
        if (m_frameCount <= 3) {
            std::cout << "  ✅ Rendering NormalSprite ID=" << spriteID
                      << " at (" << x << "," << y << ")" << std::endl;
            std::cout << "     Size=" << sprite.GetWidth() << "x" << sprite.GetHeight()
                      << " | Pitch=" << m_surface->w << std::endl;
        }

        // Render sprite
        // Note: pitch is in bytes (m_surface->pitch), not WORDs
        sprite.Blt(pDest, m_surface->pitch);

        if (SDL_MUSTLOCK(m_surface)) {
            SDL_UnlockSurface(m_surface);
        }
    }

    void RenderScreenSprite(int x, int y, int spriteID) {
        // Get screen sprite from resource container
        CSpritePalPack& spritePack = m_resources->m_EffectScreenSPK;

        // Boundary check
        if (spriteID >= spritePack.GetSize()) {
            if (m_frameCount <= 3) {
                std::cout << "  ❌ ScreenSprite ID " << spriteID << " out of range (size="
                          << spritePack.GetSize() << ")" << std::endl;
            }
            return;
        }

        // Get sprite reference (this triggers lazy loading if needed)
        CSpritePal& sprite = spritePack[spriteID];

        if (!sprite.IsInit()) {
            if (m_frameCount <= 3) {
                std::cout << "  ⚠️  ScreenSprite ID=" << spriteID << " not initialized" << std::endl;
            }
            return;
        }

        // Lock surface for direct pixel access
        if (SDL_MUSTLOCK(m_surface)) {
            SDL_LockSurface(m_surface);
        }

        // Calculate destination pointer (offset by x,y)
        WORD* pDest = (WORD*)m_surface->pixels;
        pDest += (y * m_surface->pitch / 2) + x;

        // Get palette for screen sprites
        TYPE_FRAMEID frameID = m_currentEffect->GetFrameID();
        MPalette* pPalette = &(m_resources->m_EffectScreenPPK[frameID]);

        // Log successful render
        if (m_frameCount <= 3) {
            std::cout << "  ✅ Rendering ScreenSprite ID=" << spriteID
                      << " at (" << x << "," << y << ")" << std::endl;
            std::cout << "     Size=" << sprite.GetWidth() << "x" << sprite.GetHeight()
                      << " | Pitch=" << m_surface->w << std::endl;
        }

        // Render sprite (screen blend mode - TODO: implement proper blending)
        if (pPalette) {
            // Note: pitch is in bytes (m_surface->pitch), not WORDs
            sprite.Blt(x, y, pDest, m_surface->pitch, *pPalette);
        }

        if (SDL_MUSTLOCK(m_surface)) {
            SDL_UnlockSurface(m_surface);
        }
    }

    void RenderShadowSprite(int x, int y, int spriteID) {
        // Get shadow sprite from resource container
        CShadowSpritePack& spritePack = m_resources->m_EffectShadowSPK;

        // Boundary check
        if (spriteID >= spritePack.GetSize()) {
            if (m_frameCount <= 3) {
                std::cout << "  ❌ ShadowSprite ID " << spriteID << " out of range (size="
                          << spritePack.GetSize() << ")" << std::endl;
            }
            return;
        }

        // Get sprite reference (this triggers lazy loading if needed)
        CShadowSprite& sprite = spritePack[spriteID];

        if (!sprite.IsInit()) {
            if (m_frameCount <= 3) {
                std::cout << "  ⚠️  ShadowSprite ID=" << spriteID << " not initialized" << std::endl;
            }
            return;
        }

        // Lock surface for direct pixel access
        if (SDL_MUSTLOCK(m_surface)) {
            SDL_LockSurface(m_surface);
        }

        // Calculate destination pointer (offset by x,y)
        WORD* pDest = (WORD*)m_surface->pixels;
        pDest += (y * m_surface->pitch / 2) + x;

        // Log successful render
        if (m_frameCount <= 3) {
            std::cout << "  ✅ Rendering ShadowSprite ID=" << spriteID
                      << " at (" << x << "," << y << ")" << std::endl;
            std::cout << "     Size=" << sprite.GetWidth() << "x" << sprite.GetHeight()
                      << " | Pitch=" << m_surface->w << std::endl;
        }

        // Render shadow sprite (TODO: implement proper shadow blending)
        // For now, just use the base sprite blit
        // sprite.Blt(pDest, m_surface->w / 2);

        if (SDL_MUSTLOCK(m_surface)) {
            SDL_UnlockSurface(m_surface);
        }
    }

    void RenderHelp() {
        // TODO: Render text overlay with SDL2
        // For now, just print to console
        static bool shown = false;
        if (!shown) {
            std::cout << "\n=== Effect Viewer Controls ===" << std::endl;
            std::cout << "1-9: Select effect type" << std::endl;
            std::cout << "LEFT/RIGHT: Switch effect" << std::endl;
            std::cout << "UP/DOWN: Change direction" << std::endl;
            std::cout << "SPACE: Pause/Resume" << std::endl;
            std::cout << "H: Toggle help" << std::endl;
            std::cout << "T: Run tests" << std::endl;
            std::cout << "ESC: Quit" << std::endl;
            std::cout << "==============================\n" << std::endl;
            shown = true;
        }
    }

    void SetEffectType(int effectType) {
        CreateEffect(effectType);
    }

    void Run() {
        std::cout << "\nStarting Effect Viewer..." << std::endl;
        RenderHelp();

        while (m_running) {
            HandleEvents();
            Update();
            Render();

            // Cap framerate at 60 FPS
            SDL_Delay(32);
        }

        std::cout << "Effect Viewer exited." << std::endl;
    }

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Surface* m_surface;
    SDL_Texture* m_texture;
    EffectResourceContainer* m_resources;
    EffectSpriteTypeTable* m_effectSpriteTypeTable;
    MEffect* m_currentEffect;

    int m_effectType;
    BYTE m_direction;
    bool m_paused;
    bool m_running;
    int m_frameCount;
    bool m_showHelp;
};

int main(int argc, char* argv[]) {
    const char* dataPath = "Data/Image/";
    int startEffectType = -1;  // -1 means use default (0)

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--data") == 0 && i + 1 < argc) {
            dataPath = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "--effect") == 0 && i + 1 < argc) {
            startEffectType = atoi(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --data PATH        Path to game data (default: DarkEden/Data/Image/)" << std::endl;
            std::cout << "  --effect TYPE      Start with specified effect type ID" << std::endl;
            std::cout << "  --help, -h         Show this help message" << std::endl;
            std::cout << std::endl;
            std::cout << "Examples:" << std::endl;
            std::cout << "  " << argv[0] << " --effect 16     # Start with effect type 16" << std::endl;
            std::cout << "  " << argv[0] << " --effect 979    # Start with effect type 979" << std::endl;
            return 0;
        }
    }

    std::cout << "Effect Viewer - OpenDarkEden Effect Testing Tool" << std::endl;
    std::cout << "Data path: " << dataPath << std::endl;
    if (startEffectType >= 0) {
        std::cout << "Starting with effect type: " << startEffectType << std::endl;
    }

    EffectViewer viewer;
    if (!viewer.Initialize(dataPath, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)) {
        std::cerr << "Failed to initialize Effect Viewer" << std::endl;
        return 1;
    }

    // Set initial effect type if specified
    if (startEffectType >= 0) {
        viewer.SetEffectType(startEffectType);
    }

    viewer.Run();

    return 0;
}
