#!/bin/bash
#
# build.sh - Build script for DarkEden Web Demo
#
# This script builds the demo using Emscripten for WebAssembly compilation.
#

set -e  # Exit on error

echo "======================================"
echo "Dark Eden Web Demo Build (Phase 1)"
echo "======================================"
echo ""

# Emscripten environment setup
EMSDK_DIR="${EMSDK_DIR:-$HOME/project/emsdk}"
EMSDK_ENV="$EMSDK_DIR/emsdk_env.sh"

if [ ! -f "$EMSDK_ENV" ]; then
    echo "ERROR: Emscripten not found at $EMSDK_DIR"
    echo ""
    echo "Please install Emscripten:"
    echo "  1. git clone https://github.com/emscripten-core/emsdk.git $EMSDK_DIR"
    echo "  2. cd $EMSDK_DIR"
    echo "  3. ./emsdk install latest"
    echo "  4. ./emsdk activate latest"
    echo ""
    echo "Or set EMSDK_DIR environment variable to your emsdk installation."
    exit 1
fi

# Activate Emscripten environment
echo "Activating Emscripten environment..."
source "$EMSDK_ENV"

# Check if emcc is available
if ! command -v emcc &> /dev/null; then
    echo "ERROR: emcc not found. Emscripten environment not properly activated?"
    exit 1
fi

echo "Using Emscripten: $(emcc --version | head -n 1)"
echo ""

# Create build directory
BUILD_DIR="build"
echo "Creating build directory: $BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with CMake
echo ""
echo "Configuring CMake..."
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo ""
echo "Building..."
emmake make -j$(sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo ""
    echo "======================================"
    echo "Build Complete!"
    echo "======================================"
    echo ""
    echo "Output files:"
    echo "  $BUILD_DIR/bin/DarkEdenWebDemo.html"
    echo "  $BUILD_DIR/bin/DarkEdenWebDemo.js"
    echo "  $BUILD_DIR/bin/DarkEdenWebDemo.wasm"
    echo ""
    echo "To test in browser:"
    echo "  cd $BUILD_DIR/bin"
    echo "  emrun --browser chrome DarkEdenWebDemo.html"
    echo ""
    echo "Or serve with any HTTP server:"
    echo "  cd $BUILD_DIR/bin"
    echo "  python3 -m http.server 8000"
    echo "  # Then open http://localhost:8000/DarkEdenWebDemo.html"
else
    echo ""
    echo "======================================"
    echo "Build Failed!"
    echo "======================================"
    exit 1
fi
