# Dark Eden Web Demo - Complete Guide

## Architecture Overview

The demo is implemented through **proper architecture**, NOT hacks or macros. The same code runs in both online and demo modes - configuration changes behavior.

### Key Principles

✅ **No Special Demo Code** - Demo mode uses identical code paths
✅ **No Macro Pollution** - Clean interfaces, not `#ifdef DEMO` everywhere
✅ **Improves Main Project** - Abstractions benefit testing and future development
✅ **Configuration Driven** - Behavior controlled by `GameConfig`, not code branches

## Architecture Components

### 1. Network Abstraction Layer

**Interface**: `Client/Network/INetworkService.h`

```cpp
class INetworkService {
    virtual bool Connect(const char* host, int port) = 0;
    virtual void Disconnect() = 0;
    virtual bool IsConnected() const = 0;
    virtual bool SendPacket(const void* data, size_t size) = 0;
    virtual bool ReceivePacket(void* buffer, size_t* size) = 0;
    virtual bool IsOnlineMode() const = 0;
};
```

**Implementations**:
- `OnlineNetworkService` - Real TCP socket for online play
- `OfflineNetworkService` - Stub for demo/offline mode (silently ignores packets)

**Factory**: `NetworkServiceFactory::Create(NetworkMode)`

### 2. Resource Abstraction Layer

**Interface**: `Client/Resources/IResourceLoader.h`

```cpp
class IResourceLoader {
    virtual bool FileExists(const char* path) const = 0;
    virtual size_t GetFileSize(const char* path) const = 0;
    virtual bool LoadFile(const char* path, void* buffer, size_t size) = 0;
    virtual std::vector<std::string> ListFiles(const char* pattern) = 0;
};
```

**Implementations**:
- `FileSystemResourceLoader` - Standard file I/O for desktop
- `MemoryResourceLoader` - Emscripten VFS for web

**Factory**: `ResourceLoaderFactory::Create()`

### 3. Game Configuration System

**File**: `Client/GameConfig.h/cpp`

Defines runtime configuration:
- `DEMO_CONFIG` - Web demo settings (offline, no audio, single zone)
- `ONLINE_CONFIG` - Normal game settings
- `DEV_CONFIG` - Development settings

**Selection Logic**:
1. Environment variable `DARKEDEN_MODE` (demo/dev/online)
2. Platform detection (`__EMSCRIPTEN__` → demo mode)
3. Default: online mode on desktop

### 4. Platform Abstraction

**File**: `basic/PlatformEmscripten.cpp`

Emscripten-specific implementations of platform utilities:
- `platform_get_ticks()` → `emscripten_get_now()`
- `platform_sleep()` → `emscripten_sleep()`
- File operations via Emscripten VFS

## Testing Demo Mode (Desktop)

You don't need Emscripten to test demo mode! Use the desktop build:

### Quick Test

```bash
# From client directory
./test_demo_mode.sh
```

### Manual Test

```bash
# Build client first
make debug-asan

# Set demo mode environment variable
export DARKEDEN_MODE=demo

# Run client
cd build
./bin/DarkEden
```

### What to Expect

Demo mode should:
1. Skip socket initialization (check console logs)
2. Load game data locally
3. Display "1_1" zone (or configured start zone)
4. Show Slayer character
5. Not attempt network connections

**Console Output**:
```
[ InitGame ] Socket - SKIPPED (Demo/Offline Mode)
[NetworkIntegration] Using OfflineNetworkService
```

## Building WebAssembly Version

### Prerequisites

Install Emscripten:

```bash
# Get emsdk (Emscripten SDK)
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate latest SDK
./emsdk install latest
./emsdk activate latest

# Source environment variables
source ./emsdk_env.sh
```

### Build Steps

```bash
# From client directory
cd emscripten

# Run build script
./build.sh
```

This creates:
- `emscripten/build/bin/DarkEdenWeb.html` - Single file demo (HTML + JS + WASM)
- Size: ~50-100MB depending on assets

**Note**: First build will take 5-10 minutes. Subsequent builds are faster.

### Build Configuration

File: `emscripten/CMakeLists.txt`

Key settings:
```cmake
-s USE_SDL=2
-s USE_SDL_IMAGE=2
-s USE_SDL_TTF=2
-s ALLOW_MEMORY_GROWTH=1
-s TOTAL_MEMORY=134217728     # 128MB
-s MAX_WEBGL_VERSION=2
-O3                            # Optimization
--preload-file Data@/Data     # Embed game data
```

## Creating Minimal Demo Data Package

Goal: 50-80MB demo with single outdoor scene.

### Directory Structure

```
Data/
├── Info/
│   ├── FileDef.inf          # Required: Asset definitions
│   ├── GameClient.inf       # Configure: LOCAL mode
│   └── Language.inf         # Required: Language settings
│
├── UI.spk                   # Required: UI elements (~5MB)
├── Tile.spk                 # Required: Map tiles (~5MB)
├── Creature.spk             # Required: Monsters/NPCs (~15MB)
├── Item.spk                 # Required: Items (~8MB)
├── Effect.spk               # Required: Skill effects (~3MB)
└── ImageObject.spk          # Required: Scene objects (~15MB)
```

### Step 1: Identify Required Assets

Check your start zone (e.g., "1_1") in `Data/Info/ZoneInfo.inf`:
```
[Zone 1_1]
Name = "Some Outdoor Zone"
Tile = "Tiles/OutdoorTile.spk"
...
```

### Step 2: Extract Required .spk Files

The .spk files are archives. You need to:
1. Identify which .spk files contain assets for zone "1_1"
2. Copy only those files to demo Data directory

**Tool**: Use the existing sprite viewer tools:
```bash
cd build
./bin/sprite_viewer ../DarkEden/Data/Creature.spk
```

### Step 3: Create Demo FileDef.inf

Edit `Data/Info/FileDef.inf` to list only demo assets:

```ini
[Sprite]
UI = Data/UI.spk
Tile = Data/Tile.spk
Creature = Data/Creature.spk
Item = Data/Item.spk
Effect = Data/Effect.spk
ImageObject = Data/ImageObject.spk
```

### Step 4: Configure Demo Mode

Edit `Data/Info/GameClient.inf`:

```ini
[Game]
# Disable network for demo
UseNetwork = false
StartZone = 1_1
PlayerRace = Slayer

# Disable updates
AutoUpdate = false
```

### Step 5: Test Desktop Demo

```bash
export DARKEDEN_MODE=demo
./build/bin/DarkEden
```

### Step 6: Package for Web

Emscripten build script automatically preloads Data directory:
```bash
cd emscripten
./build.sh
```

Result: `build/bin/DarkEdenWeb.html` (~50-100MB)

## Deployment

### Single File Deployment

The `.html` output contains everything:
- HTML structure
- JavaScript loader
- WebAssembly binary (as base64 data URL)
- Preloaded game data

**Deploy**:
1. Upload `DarkEdenWeb.html` to any static host
2. No server configuration needed
3. Works on GitHub Pages, Netlify, S3, etc.

**Example**:
```bash
# Upload to web server
scp emscripten/build/bin/DarkEdenWeb.html user@server:/var/www/html/

# Or use GitHub Pages
cp emscripten/build/bin/DarkEdenWeb.html docs/index.html
git push
```

### Multi-File Deployment (Optional)

For faster loading, use separate files:

Add to `emscripten/CMakeLists.txt`:
```cmake
# Instead of SINGLE_FILE
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".html")
target_link_options(DarkEdenWeb PRIVATE
    "SHELL:-s SINGLE_FILE=0"  # Separate .wasm and .data files
)
```

Upload all files:
- `DarkEdenWeb.html`
- `DarkEdenWeb.wasm`
- `DarkEdenWeb.data`

## Troubleshooting

### Desktop Demo Mode Issues

**Problem**: Game still tries to connect
- **Check**: Console logs for `[ InitGame ] Socket - SKIPPED`
- **Verify**: `DARKEDEN_MODE=demo` is set
- **Debug**: Add `DEBUG_ADD("[Demo] Mode activated")` in GameInit.cpp

**Problem**: Game crashes on startup
- **Check**: Game data path is correct
- **Verify**: All required .inf files exist
- **Debug**: Run with debugger: `lldb build/bin/DarkEden`

### WebAssembly Build Issues

**Problem**: `emcc not found`
- **Solution**: Install Emscripten SDK and source `emsdk_env.sh`

**Problem**: Build fails with linking errors
- **Check**: All source files are in `emscripten/CMakeLists.txt`
- **Verify**: CMake version is 3.20+
- **Debug**: Run `emcmake cmake .. -DCMAKE_BUILD_TYPE=Debug`

**Problem**: Runtime errors in browser
- **Check**: Browser console for errors
- **Verify**: Memory limit (increase `-s TOTAL_MEMORY`)
- **Debug**: Add `-s ASSERTIONS=1` to build flags

### Performance Issues

**Problem**: Low FPS in browser
- **Solution**: Reduce texture sizes in demo assets
- **Solution**: Simplify scene (fewer creatures)
- **Optimization**: Enable WebGL 2 acceleration

**Problem**: Large file size
- **Solution**: Use fewer assets (remove unused zones)
- **Solution**: Compress .spk files with better compression
- **Solution**: Use multi-file deployment (allows gzip)

## File Reference

### New Architecture Files

```
Client/
├── Network/
│   ├── INetworkService.h              # Network interface
│   ├── OnlineNetworkService.{h,cpp}   # TCP implementation
│   ├── OfflineNetworkService.{h,cpp}  # Stub implementation
│   └── NetworkServiceFactory.{h,cpp}  # Factory
│
├── Resources/
│   ├── IResourceLoader.h              # Resource interface
│   ├── FileSystemResourceLoader.{h,cpp} # File I/O
│   ├── MemoryResourceLoader.{h,cpp}   # Emscripten VFS
│   └── ResourceLoaderFactory.{h,cpp}  # Factory
│
├── GameConfig.{h,cpp}                 # Configuration system
└── NetworkIntegration.{h,cpp}         # Glue layer

basic/
└── PlatformEmscripten.cpp             # Web platform utils

emscripten/
├── CMakeLists.txt                     # Web build config
└── build.sh                           # Build script
```

### Modified Files

```
Client/
└── GameInit.cpp                       # Added ShouldInitNetwork() check

basic/
└── CMakeLists.txt                     # Added PlatformEmscripten.cpp
```

## Benefits to Main Project

This demo implementation provides permanent benefits:

1. **Testable Network Code**
   - Inject `OfflineNetworkService` for unit tests
   - Test game logic without server

2. **Flexible Resource Loading**
   - Load from memory, network, or custom sources
   - Easy to add encrypted/verified resources

3. **Clean Separation of Concerns**
   - Network, resources, and platform logic separated
   - Easier to understand and maintain

4. **Future Platform Support**
   - Easy to add mobile/console platforms
   - Just implement new platform-specific files

5. **Configuration Flexibility**
   - Easy to add new game modes
   - No code changes needed

## Next Steps

1. **Test Desktop Demo** - Run `./test_demo_mode.sh`
2. **Create Minimal Assets** - Extract single zone assets
3. **Install Emscripten** - Set up build environment
4. **Build Web Demo** - Run `emscripten/build.sh`
5. **Deploy** - Upload to static hosting

## Support

For issues or questions:
- Check browser console for runtime errors
- Verify Emscripten installation: `emcc --version`
- Test desktop demo first to isolate issues
- Check memory usage in browser DevTools

Enjoy your web demo! 🎮
