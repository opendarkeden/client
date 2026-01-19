# Resource Management Tools

OpenDarkEden resource management tools for validating game assets.

## Tools

### 1. extract_macros.py

Extracts resource path definitions from C header files (`VS_UI_filepath.h`) and generates `.inf` files in Properties format.

**Usage:**
```bash
python3 tools/resource_management/extract_macros.py <header_file> <output_file> [--platform=PLATFORM]
```

**Examples:**
```bash
# Extract for Unix/macOS
python3 tools/resource_management/extract_macros.py \
    VS_UI/src/header/VS_UI_filepath.h \
    DarkEden/Data/Info/VS_UI_filepath.inf

# Extract for Windows
python3 tools/resource_management/extract_macros.py \
    VS_UI/src/header/VS_UI_filepath.h \
    DarkEden/Data/Info/VS_UI_filepath.inf --platform=windows
```

**How it works:**
- Parses C preprocessor macros from `VS_UI_filepath.h`
- Expands macro concatenations (e.g., `SPK_TITLE SPK_ROOT"Title.spk"`)
- Converts path separators to Unix-style (`/`)
- Generates `.inf` file with `key : value` format

**Supported resource types:**
- `SPK_*` - Sprite pack files
- `ISPK_*` - Indexed sprite pack files
- `CFPK_*` - Creature frame pack files
- `RPK_*` - Resource pack files
- `FILE_INFO_*` - Info/config files
- `TXT_*` - Text resource files

### 2. resource_validator

Validates resource files for existence and integrity.

**Usage:**
```bash
build/debug/bin/resource_validator <inf_file>
```

**Example:**
```bash
# Validate resources from generated .inf file
build/debug/bin/resource_validator DarkEden/Data/Info/VS_UI_filepath.inf
```

**How it works:**
1. Loads resource definitions from `.inf` file
2. For each resource:
   - Checks if file exists
   - For `.spk` files: verifies file size and index file (`.spki`)
   - For `.inf` files: validates Properties format
   - For other types: existence check only
3. Generates validation report with statistics

**Output:**
```
Resource Validator
==================
Loading resource list: DarkEden/Data/Info/VS_UI_filepath.inf
Loaded 237 resource definitions
Validating resources...

========================================
    Resource Validation Report
========================================
Total: 237 resources

Statistics:
  Valid:       230
  Corrupted:   2
  Missing:     0
  Skipped:     5 (directories)

========================================
    Problem Resources
========================================

  SPK_CORRUPTED
    Path:   Data/Ui/spk/Corrupted.spk
    Status: Cannot open file

  ...
```

## Makefile Integration

### `make check-resources`

Runs complete resource validation:
1. Generates `.inf` file from `.h` file
2. Builds validator if needed
3. Runs validator on generated `.inf` file

**Usage:**
```bash
# Use default DarkEden directory (./DarkEden)
make check-resources

# Specify custom DarkEden data directory
make check-resources DARKEDEN_DIR=/path/to/DarkEden
```

**Examples:**
```bash
# Game data in current directory
make check-resources

# Game data in parent directory
make check-resources DARKEDEN_DIR=../DarkEden

# Game data in absolute path
make check-resources DARKEDEN_DIR=/opt/DarkEden

# Game data in user home
make check-resources DARKEDEN_DIR=~/games/DarkEden
```

### `make extract-resources`

Extracts macros from header file to `.inf` file:
```bash
make extract-resources [DARKEDEN_DIR=/path/to/DarkEden]
```

### `make clean-resources`

Removes generated `.inf` file:
```bash
make clean-resources [DARKEDEN_DIR=/path/to/DarkEden]
```

## Workflow

### Typical Development Workflow

1. **Add new resource to header file:**
   ```cpp
   // In VS_UI/src/header/VS_UI_filepath.h
   #define SPK_NEW_FEATURE SPK_ROOT"NewFeature.spk"
   ```

2. **Validate resources:**
   ```bash
   make check-resources
   ```

3. **Review validation report:**
   - If file is missing: Add the file to correct location
   - If file is corrupted: Fix or replace the file

4. **Commit changes:**
   ```bash
   git add VS_UI/src/header/VS_UI_filepath.h
   git add DarkEden/Data/Info/VS_UI_filepath.inf  # Generated file
   git commit -m "Add new feature resources"
   ```

### CI/CD Integration

Add to your CI pipeline:
```yaml
# Example GitHub Actions
- name: Validate Resources
  run: make check-resources
```

## File Locations

| Tool | Location |
|------|----------|
| Extract script | `tools/resource_management/extract_macros.py` |
| Validator source | `Client/tools/resource_validator.cpp` |
| Validator binary | `build/debug/bin/resource_validator` |
| Generated .inf | `DarkEden/Data/Info/VS_UI_filepath.inf` |
| Source header | `VS_UI/src/header/VS_UI_filepath.h` |

## Troubleshooting

### Issue: `DARKEDEN_DIR` parameter not working

**Problem:** Makefile doesn't use custom DARKEDEN_DIR path

**Solution:** Make sure to pass DARKEDEN_DIR as a Makefile variable:
```bash
# Correct
make check-resources DARKEDEN_DIR=/path/to/DarkEden

# Incorrect (shell variable won't work)
export DARKEDEN_DIR=/path/to/DarkEden
make check-resources
```

### Issue: Extracted paths have wrong format

**Problem:** Paths like `dataImageOusters.cfpk` instead of `Data/Image/Ousters.cfpk`

**Solution:** The extract script now handles macro expansion correctly. Make sure you're using the latest version:
```bash
git pull origin master
```

### Issue: Validator reports all files missing

**Problem:** All resources marked as "File not found"

**Solution:** Check that:
1. The validator output shows "Base directory: /correct/path" - verify this is where your game data is
2. If the base directory is wrong, try:
   ```bash
   # Explicitly specify DARKEDEN_DIR
   make check-resources DARKEDEN_DIR=/actual/path/to/DarkEden
   ```
3. Verify game data files exist at the correct location
4. Check that the `.inf` file paths match your directory structure

**Note:** The validator automatically extracts the base directory from the `.inf` file path. For example:
- `.inf` file: `/home/user/game/DarkEden/Data/Info/VS_UI_filepath.inf`
- Extracted base: `/home/user/game/DarkEden`
- Resources are looked up as: `/home/user/game/DarkEden/Data/Ui/spk/Title.spk`

### Issue: Build fails with "resource_validator not found"

**Problem:** `make check-resources` fails to build validator

**Solution:**
```bash
# Configure CMake first
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug

# Then build validator explicitly
cmake --build build/debug --target resource_validator

# Or build entire project
make debug
```

## Future Enhancements

Potential improvements to the resource management system:

1. **Deep validation for more file types:**
   - `.ispk` - Indexed sprite pack validation
   - `.cfpk` - Creature frame pack validation
   - `.rpk` - Resource pack validation

2. **Resource usage analysis:**
   - Scan C++ code for resource references
   - Find unused resource files
   - Find missing resource references

3. **Resource viewing tools:**
   - Sprite viewer (from ai branch)
   - Creature viewer (from ai branch)
   - Map viewer (from ai branch)
   - Item viewer (new)

4. **Packaging tools:**
   - Resource packer/unpacker
   - Batch resource conversion
   - Resource optimization

## Related Documentation

- [VS_UI filepath header](../../VS_UI/src/header/VS_UI_filepath.h) - Source of resource definitions
- [Properties class](../../Client/Packet/Properties.h) - .inf file format
- [SpriteLib](../../Client/SpriteLib/) - Sprite loading library
- [Plan document](../../../.claude/plans/) - Implementation plan
