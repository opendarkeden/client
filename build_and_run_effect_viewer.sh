#!/bin/bash
# Effect Viewer - Build and Run Script
# Compiles effect_viewer with ASan and runs it

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build/debug-asan"
DATA_DIR="$SCRIPT_DIR/../DarkEden"

echo "========================================"
echo "Effect Viewer - Build and Run Script"
echo "========================================"
echo ""

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$SCRIPT_DIR"

# Configure CMake if needed
if [ ! -f "$BUILD_DIR/CMakeCache.txt" ]; then
    echo "[1/4] Configuring CMake with ASan..."
    cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug -DUSE_ASAN=ON
else
    echo "[1/4] CMake already configured"
fi
echo ""

# Build effect_viewer
echo "[2/4] Building effect_viewer..."
cmake --build "$BUILD_DIR" --target effect_viewer -- -j$(sysctl -n hw.ncpu)
echo ""

# Check for DarkEden data
echo "[3/4] Checking for game data..."
if [ ! -d "$DATA_DIR" ]; then
    echo "WARNING: DarkEden data directory not found at: $DATA_DIR"
    echo "Creating directory structure..."
    mkdir -p "$DATA_DIR/Data/Image"
    echo ""
    echo "⚠️  You need to copy game data to: $DATA_DIR"
    echo "   Required files in Data/Image/:"
    echo "   - Effect.efpk or EffectAlpha.efpk"
    echo "   - Effect.aspk"
    echo "   - Effect.ppk or EffectAlpha.ppk"
    echo "   - etc."
    echo ""
fi
echo ""

# Run effect_viewer
echo "[4/4] Running effect_viewer..."
echo "========================================"
cd "$DATA_DIR"
exec "$BUILD_DIR/bin/effect_viewer" "$@"
