cmake_minimum_required(VERSION 3.10)
project(DarkEdenSDL VERSION 1.0.0 LANGUAGES C CXX)

# Set C++17 standard (Requirements 8.3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find SDL2 library (Requirements 8.2)
find_package(SDL2 REQUIRED)

# Platform-specific settings (Requirements 8.5)
if(WIN32)
    # Windows-specific settings
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
    # macOS-specific settings
    set(CMAKE_MACOSX_RPATH ON)
elseif(UNIX)
    # Linux-specific settings
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-language-extension-token)
endif()

# =============================================================================
# Shared Library: libdarkeden
# Requirements: 1.1, 1.2, 1.3
# =============================================================================

set(LIB_SOURCES
    lib/color.c
    lib/sprite.c
    lib/spritepack.c
    lib/sdl_framework.c
    lib/animation.c
    lib/frame.c
    lib/framepack.c
    lib/colorset.c
    lib/index_sprite.c
    lib/index_spritepack.c
    lib/shadow_sprite.c
    lib/shadow_spritepack.c
    lib/zone.c
    # UI System
    lib/ui_surface.c
    lib/ui_window.c
    lib/ui_button.c
    lib/ui_button_group.c
    lib/ui_event_router.c
    lib/ui_window_manager.c
    lib/ui_global_resource.c
    lib/ui_dialog.c
)

set(LIB_HEADERS
    lib/color.h
    lib/sprite.h
    lib/spritepack.h
    lib/sdl_framework.h
    lib/str.h
    lib/map.h
    lib/vector.h
    lib/types.h
    lib/animation.h
    lib/frame.h
    lib/framepack.h
    lib/colorset.h
    lib/index_sprite.h
    lib/index_spritepack.h
    lib/shadow_sprite.h
    lib/shadow_spritepack.h
    lib/zone.h
    lib/error.h
    # UI System
    lib/ui_surface.h
    lib/ui_window.h
    lib/ui_button.h
    lib/ui_button_group.h
    lib/ui_event_router.h
    lib/ui_window_manager.h
    lib/ui_global_resource.h
    lib/ui_dialog.h
)

# Create static library (Requirement 1.3)
add_library(darkeden STATIC ${LIB_SOURCES} ${LIB_HEADERS})

# Include directories for library
target_include_directories(darkeden PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${SDL2_INCLUDE_DIRS}
)

# Link SDL2 to library
target_link_libraries(darkeden PUBLIC ${SDL2_LIBRARIES})

# Handle SDL2 linking on different platforms
if(TARGET SDL2::SDL2)
    target_link_libraries(darkeden PUBLIC SDL2::SDL2)
elseif(TARGET SDL2::SDL2-static)
    target_link_libraries(darkeden PUBLIC SDL2::SDL2-static)
endif()

# =============================================================================
# Tool: Zone Viewer
# Dump/inspect .map zone files (header/sectors/objects)
# =============================================================================

set(ZONE_PARSER_SOURCES
    tools/zone_parser/main.c
)

add_executable(ZoneParser ${ZONE_PARSER_SOURCES})

# Include directories for zone viewer
target_include_directories(ZoneParser PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/zone_parser
)

# Link against shared library (reuse zone parsing in lib)
target_link_libraries(ZoneParser PRIVATE darkeden)

# =============================================================================
# Tool: Sprite Viewer
# Requirements: 1.4, 1.5
# =============================================================================

set(SPRITE_VIEWER_SOURCES
    tools/sprite_viewer/main.c
    tools/sprite_viewer/viewer.c
)

set(SPRITE_VIEWER_HEADERS
    tools/sprite_viewer/viewer.h
)

add_executable(SpriteViewer ${SPRITE_VIEWER_SOURCES} ${SPRITE_VIEWER_HEADERS})

# Include directories for sprite viewer
target_include_directories(SpriteViewer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sprite_viewer
)

# Link against shared library (Requirement 1.4)
target_link_libraries(SpriteViewer PRIVATE darkeden)

# Add SDL2main on Windows
if(WIN32 AND TARGET SDL2::SDL2main)
    target_link_libraries(SpriteViewer PRIVATE SDL2::SDL2main)
endif()

# =============================================================================
# Tool: Creature Viewer
# Requirements: 5.1-5.10
# =============================================================================

set(CREATURE_VIEWER_SOURCES
    tools/creature_viewer/main.c
)

add_executable(CreatureViewer ${CREATURE_VIEWER_SOURCES})

# Include directories for creature viewer
target_include_directories(CreatureViewer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Link against shared library
target_link_libraries(CreatureViewer PRIVATE darkeden)

# Add SDL2main on Windows
if(WIN32 AND TARGET SDL2::SDL2main)
    target_link_libraries(CreatureViewer PRIVATE SDL2::SDL2main)
endif()

# =============================================================================
# Tool: Map Viewer
# Minimal MVP to visualize a rectangular area of a .map using existing
# resurrected spritepack/sprite decode and SDL framework.
# =============================================================================

set(MAP_VIEWER_SOURCES
    tools/map_viewer/main.c
)

add_executable(MapViewer ${MAP_VIEWER_SOURCES})

# Include directories for map viewer
target_include_directories(MapViewer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/map_viewer
)

# Link against shared library (reuse the darkeden static library)
target_link_libraries(MapViewer PRIVATE darkeden)

# Add SDL2main on Windows
if(WIN32 AND TARGET SDL2::SDL2main)
    target_link_libraries(MapViewer PRIVATE SDL2::SDL2main)
endif()

# =============================================================================
# Testing
# =============================================================================

enable_testing()

# Test executable
set(TEST_SOURCES
    tests/test_main.c
    tests/test_color.c
    tests/test_sdl_framework.c
    tests/test_sprite.c
    tests/test_spritepack.c
    tests/test_animation.c
    tests/test_frame.c
    tests/test_framepack.c
    tests/test_index_sprite.c
    tests/test_shadow_sprite.c
)

add_executable(test_runner ${TEST_SOURCES})

target_include_directories(test_runner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# Link test runner against shared library
target_link_libraries(test_runner PRIVATE darkeden)

add_test(NAME unit_tests COMMAND test_runner)

# =============================================================================
# Installation rules
# =============================================================================

install(TARGETS SpriteViewer CreatureViewer ZoneParser
    RUNTIME DESTINATION bin
)

install(TARGETS darkeden
    ARCHIVE DESTINATION lib
)

install(FILES ${LIB_HEADERS}
    DESTINATION include/darkeden
)

# =============================================================================
# Executable: TopView (Game MVP)
# Minimal game-like runner using SDL that renders a zone and a single creature,
# with camera follow and mouse-controlled movement.
# =============================================================================

set(TOP_VIEW_SOURCES
    src/main.c
    src/top_view.c
    src/top_view.h
)

add_executable(TopView ${TOP_VIEW_SOURCES})

target_include_directories(TopView PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(TopView PRIVATE darkeden)

if(WIN32 AND TARGET SDL2::SDL2main)
    target_link_libraries(TopView PRIVATE SDL2::SDL2main)
endif()

# =============================================================================
# Executable: SDLUIDemo
# A standalone SDL demo that uses the shimmed sprite surface/pack to render
# legacy-style UI sprites without DirectX. Compiles only the new SDL path.
# =============================================================================

set(SDL_UI_SOURCES
    src/ui/sdl_demo_main.cpp
    src/sdl_shim/CSpriteSurfaceSDL.cpp
    src/sdl_shim/SpritePackSDL.cpp
    src/sdl_shim/UIContextSDL.cpp
    src/sdl_shim/GlobalResourceSDL.cpp
    src/sdl_shim/GameOverDialogSDL.cpp
)

add_executable(SDLUIDemo ${SDL_UI_SOURCES})

target_include_directories(SDLUIDemo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sdl_shim
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

target_link_libraries(SDLUIDemo PRIVATE darkeden)

if(WIN32 AND TARGET SDL2::SDL2main)
    target_link_libraries(SDLUIDemo PRIVATE SDL2::SDL2main)
endif()

# =============================================================================
# Executable: UIDemo (Pure C UI System Demo)
# Demonstrates the pure C UI component system with a simple dialog.
# =============================================================================

set(UI_DEMO_SOURCES
    src/ui_demo/main.c
)

add_executable(UIDemo ${UI_DEMO_SOURCES})

target_include_directories(UIDemo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

target_link_libraries(UIDemo PRIVATE darkeden)

if(WIN32 AND TARGET SDL2::SDL2main)
    target_link_libraries(UIDemo PRIVATE SDL2::SDL2main)
endif()
