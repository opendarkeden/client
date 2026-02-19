#!/bin/bash
# Setup complete Data directory for web demo

set -e

SOURCE_DIR="/Users/genius/project/opendarkeden/DarkEden/Data/Info"
TARGET_DIR="/Users/genius/project/opendarkeden/client/emscripten/build/bin/Data/Info"

echo "Setting up complete Data directory for web demo..."

# Create target directory
mkdir -p "$TARGET_DIR"

# Copy complete FileDef.inf
echo "Copying complete FileDef.inf..."
cp "$SOURCE_DIR/FileDef.inf" "$TARGET_DIR/FileDef.inf"

# Copy all Info files
echo "Copying all Info files..."
for inf in "$SOURCE_DIR"/*.inf; do
    filename=$(basename "$inf")
    echo "  Copying $filename..."
    cp "$inf" "$TARGET_DIR/$filename"
done

echo "Done! Data directory is ready."
echo "Total Info files: $(ls -1 "$TARGET_DIR"/*.inf | wc -l)"
