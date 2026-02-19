#!/bin/bash
#
# test_desktop.sh - Test demo on desktop before Emscripten build
#
# This compiles and runs the demo on the native platform first,
# catching ~90% of errors before attempting the slower Emscripten build.
#

set -e

echo "======================================"
echo "Desktop Test (Before Emscripten)"
echo "======================================"
echo ""

# Check for SDL2
if ! pkg-config --exists sdl2; then
    echo "ERROR: SDL2 not found"
    echo "Install with: brew install sdl2 sdl2_image (macOS)"
    echo "Or: apt-get install libsdl2-dev libsdl2-image-dev (Ubuntu)"
    exit 1
fi

# Check for SDL2_image
if ! pkg-config --exists SDL2_image; then
    echo "ERROR: SDL2_image not found"
    echo "Install with: brew install sdl2_image (macOS)"
    echo "Or: apt-get install libsdl2-image-dev (Ubuntu)"
    exit 1
fi

# Get SDL2 flags
SDL_CFLAGS=$(pkg-config --cflags sdl2 SDL2_image)
SDL_LIBS=$(pkg-config --libs sdl2 SDL2_image)

echo "SDL2 flags found."
echo ""

# Collect all SpriteLib .cpp files (exclude CSpriteSurface.cpp and CSpriteSurface_Adapter.cpp - use SDL version only)
SPRITELIB_SOURCES=$(ls ../Client/SpriteLib/CSprite*.cpp ../Client/SpriteLib/SpriteLibBackendSDL.cpp 2>/dev/null | grep -v CSpriteSurface.cpp$ | grep -v CSpriteSurface_Adapter.cpp$ | tr '\n' ' ')

# Compile
echo "Compiling..."
g++ -std=c++11 \
    -DSPRITELIB_BACKEND_SDL \
    -I.. -I../Client -I../Client/DXLib -I../Client/framelib -I../Client/Packet \
    demo_main.cpp \
    ../Client/TileRenderer.cpp \
    ../Client/MZone.cpp \
    ../Client/MZoneTileProvider.cpp \
    ../Client/MSector.cpp \
    ../Client/ZoneFileHeader.cpp \
    ../Client/DXLib/CDirectDrawSurface.cpp \
    ../Client/DXLib/CDirectDraw.cpp \
    ../Client/DXLib/CDirectDraw_StaticMembers.cpp \
    ../Client/MObject.cpp \
    ../Client/MHelicopterManager.cpp \
    ../Client/MImageObject.cpp \
    ../Client/MString.cpp \
    ../Client/framelib/CAnimationFrame.cpp \
    ../Client/MEffect.cpp \
    ../Client/MMovingEffect.cpp \
    ../Client/MLinearEffect.cpp \
    ../Client/MChaseEffect.cpp \
    ../Client/MAnimationObject.cpp \
    ../Client/MShadowAnimationObject.cpp \
    ../Client/MInteractionObject.cpp \
    DemoGlobals.cpp \
    DemoStubs.cpp \
    ../Client/SpriteLib/CAlphaSprite.cpp \
    ../Client/SpriteLib/CIndexSprite.cpp \
    ../Client/SpriteLib/CShadowSprite.cpp \
    DemoVTables.cpp \
    $SPRITELIB_SOURCES \
    -o demo_test \
    $SDL_CFLAGS $SDL_LIBS \
    -liconv

if [ $? -eq 0 ]; then
    echo ""
    echo "======================================"
    echo "Desktop Build Successful!"
    echo "======================================"
    echo ""
    echo "Executable created: demo_test"
    echo ""
    echo "To run:"
    echo "  ./demo_test DarkEden/Data/Map/adam_c.map DarkEden/Data/Image/tile.spk"
    echo ""
    echo "Or with objects:"
    echo "  ./demo_test DarkEden/Data/Map/adam_c.map DarkEden/Data/Image/tile.spk DarkEden/Data/Image/ImageObject.spk"
else
    echo ""
    echo "======================================"
    echo "Desktop Build Failed!"
    echo "======================================"
    echo ""
    echo "Please fix compilation errors before attempting Emscripten build."
    exit 1
fi
