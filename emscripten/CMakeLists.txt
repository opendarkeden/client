cmake_minimum_required(VERSION 3.20)
project(DarkEdenWebDemo CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define SDL backend for SpriteLib
add_compile_definitions(SPRITELIB_BACKEND_SDL)

# Minimal source set for Phase 1 (map rendering only)
# Only include what's absolutely necessary
set(DEMO_SOURCES
    # Demo entry point
    demo_main.cpp

    # SpriteLib - SDL backend (core rendering)
    ../Client/SpriteLib/CSprite.cpp
    ../Client/SpriteLib/CSprite_SDL.cpp
    ../Client/SpriteLib/CAlphaSprite.cpp
    ../Client/SpriteLib/CIndexSprite.cpp
    ../Client/SpriteLib/CShadowSprite.cpp
    ../Client/SpriteLib/CSprite555.cpp
    ../Client/SpriteLib/CSprite565.cpp
    ../Client/SpriteLib/CSpritePal.cpp
    ../Client/SpriteLib/CSpritePalBase.cpp
    ../Client/SpriteLib/CSpriteDef.cpp
    ../Client/SpriteLib/CSpritePack.cpp
    ../Client/SpriteLib/CSpritePackList.cpp
    ../Client/SpriteLib/CSpritePackList555.cpp
    ../Client/SpriteLib/CSpritePackList565.cpp
    # Note: Use CSpriteSurface_SDL.cpp ONLY (not CSpriteSurface.cpp or CSpriteSurface_Adapter.cpp)
    ../Client/SpriteLib/CSpriteSurface_SDL.cpp
    ../Client/SpriteLib/CSpriteSurface_Effects.cpp
    ../Client/SpriteLib/CSpriteSet.cpp
    ../Client/SpriteLib/CSpriteSetManager.cpp
    ../Client/SpriteLib/CSpriteOutlineManager.cpp
    ../Client/SpriteLib/SpriteLibBackendSDL.cpp

    # TileRenderer - modular rendering component
    ../Client/TileRenderer.cpp

    # MZone - Game's zone management (replaces zoneloader)
    ../Client/MZone.cpp
    ../Client/MZoneTileProvider.cpp
    ../Client/MSector.cpp
    ../Client/ZoneFileHeader.cpp

    # DXLib - CDirectDrawSurface and CSDLGraphics (dependencies for SpriteSurface)
    ../Client/DXLib/CDirectDrawSurface.cpp
    ../Client/DXLib/CDirectDraw.cpp
    ../Client/DXLib/CDirectDraw_StaticMembers.cpp

    # Additional required sources
    ../Client/MObject.cpp
    ../Client/MHelicopterManager.cpp
    ../Client/MImageObject.cpp

    # Phase 1: Minimal dependencies (no game logic)
    ../Client/MString.cpp
    ../Client/framelib/CAnimationFrame.cpp

    # Phase 2: Effect system (demo needs this)
    ../Client/MEffect.cpp
    ../Client/MMovingEffect.cpp
    ../Client/MLinearEffect.cpp
    ../Client/MChaseEffect.cpp
    ../Client/MAnimationObject.cpp
    ../Client/MShadowAnimationObject.cpp
    ../Client/MInteractionObject.cpp

    # Global variable definitions (minimal set for demo)
    DemoGlobals.cpp
    DemoStubs.cpp
    DemoVTables.cpp
)

# Platform abstraction library (basic utilities)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../basic basic_build)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Client
    ${CMAKE_CURRENT_SOURCE_DIR}/../Client/DXLib
    ${CMAKE_CURRENT_SOURCE_DIR}/../Client/framelib
    ${CMAKE_CURRENT_SOURCE_DIR}/../Client/Packet
)

# Create executable
add_executable(DarkEdenWebDemo ${DEMO_SOURCES})

# Link with basic library
target_link_libraries(DarkEdenWebDemo PRIVATE basic)

# Emscripten-specific configuration
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX_CXX ".html")

    # Emscripten linker flags
    target_link_options(DarkEdenWebDemo PRIVATE
        "SHELL:-s USE_SDL=2"
        "SHELL:-s USE_SDL_IMAGE=2"
        "SHELL:-s WASM=1"
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
        "SHELL:-s INITIAL_MEMORY=134217728"    # 128MB
        "SHELL:-s MAX_WEBGL_VERSION=2"
        "SHELL:-O3"                            # Optimization
        "SHELL:--embed-file Data@/Data"        # Embed data directory
    )
endif()
