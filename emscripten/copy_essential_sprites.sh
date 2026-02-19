#!/bin/bash
# Copy essential sprite files for web demo initialization

set -e

SOURCE_DIR="/Users/genius/project/opendarkeden/DarkEden/Data/Image"
TARGET_DIR="/Users/genius/project/opendarkeden/client/emscripten/build/bin/Data/Image"

echo "Copying essential sprite files for web demo..."

# Create target directory
mkdir -p "$TARGET_DIR"

# Guild mark related files (required by MGuildMarkManager)
echo "Copying Guild Mark sprites..."
cp "$SOURCE_DIR/GuildMark.spk" "$TARGET_DIR/"
cp "$SOURCE_DIR/GuildMark.spki" "$TARGET_DIR/"
cp "$SOURCE_DIR/GradeMarkSlayer.spk" "$TARGET_DIR/"
cp "$SOURCE_DIR/GradeMarkSlayer.spki" "$TARGET_DIR/"
cp "$SOURCE_DIR/GradeMarkVampire.spk" "$TARGET_DIR/"
cp "$SOURCE_DIR/GradeMarkVampire.spki" "$TARGET_DIR/"
cp "$SOURCE_DIR/GradeMarkOusters.spk" "$TARGET_DIR/"
cp "$SOURCE_DIR/GradeMarkOusters.spki" "$TARGET_DIR/"
cp "$SOURCE_DIR/LevelMarkPet.spk" "$TARGET_DIR/"
cp "$SOURCE_DIR/LevelMarkPet.spki" "$TARGET_DIR/"

echo "Done! Copied Guild Mark sprites."
echo "Total size: $(du -sh "$TARGET_DIR" | cut -f1)"
