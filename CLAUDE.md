# OpenDarkEden Client

This is the **Dark Eden** game client - an isometric MMORPG similar to Diablo. Dark Eden is a horror-themed MMORPG featuring vampires, slayers, and ousters as playable races.

## Project Overview

Dark Eden is a classic Korean MMORPG originally developed by Softon. This open-source client project aims to modernize and maintain the game client.

**Build System: CMake
**Primary Language:** C++
**Platform:** Windows (original), with modern cross-platform efforts

## How to build

There is a Makefile warp cmake providing commands like `make` `make debug` `make release` etc.

For development, the most commonly used one is:

```
make debug-asan 
```

## Repository Structure

```
client/
├── Client/              # Main game client code
│   ├── D3DLib/         # SDL compatibility stub (post-migration)
│   ├── DXLib/          # SDL backend for Input, Sound, Music
│   ├── SpriteLib/      # Sprite animation system (SDL backend)
│   ├── TextSystem/     # Text rendering system (SDL + freetype2)
│   ├── TextLib/        # Text layout and caching
│   ├── VolumeLib/      # Volume/collision detection
│   ├── framelib/       # Frame handling
│   ├── DEUtil/         # Dark Eden utilities
│   ├── MZLib/          # Compression library
│   ├── Packet/         # Network packet definitions
│   └── *.cpp/*.h       # Main game logic (GameMain, GameUI, etc.)
│
├── VS_UI/              # User Interface framework
│   └── src/
│       ├── header/     # UI header files
│       ├── widget/     # UI widget components (buttons, scrollbars, etc.)
│       ├── Imm/        # Immersion touch feedback library
│       └── hangul/     # Korean text input support
│
├── basic/              # Basic utility library
│   ├── BasicMemory.h   # Memory management
│   ├── BasicException.h # Exception handling
│   ├── Typedef.h       # Type definitions
│   └── PlatformUtil.h  # Platform utilities (SDL/Windows abstraction)
│
├── build/              # CMake build output
├── ../DarkEden/        # Game data directory (runtime)
│   ├── Data/           # Game data files
│   │   ├── Info/       # Configuration files
│   │   ├── Map/        # Map files
│   │   └── ...
│   └── UserSet/        # User settings
│
└── demo/               # Demo applications
```

## Key Components

### Rendering System (SDL2 Based)
- **D3DLib/**: Minimal compatibility stub for DirectX to SDL migration
  - `CDirect3D`: Stub class providing D3D-compatible interface
  - `DX3D.h`: Main header for backward compatibility
  - **Note**: Actual rendering handled by SpriteLib with SDL backend

- **SpriteLib/**: Sprite animation system with SDL backend
  - `CSprite_SDL.cpp`: SDL sprite implementation
  - `CSpriteSurface_SDL.cpp`: SDL surface implementation
  - `SpriteLibBackendSDL.h/cpp`: SDL backend utilities
  - Supports multiple pixel formats (555, 565, 4444, etc.)

- **TextSystem/**: Modern text rendering (SDL + freetype2)
  - UTF-8 support for internationalization
  - Replaces old Windows GDI rendering
  - `TextService.cpp`: Main text service
  - `TextBackendSDL.cpp`: SDL text backend

### Input & Sound (`Client/DXLib/`)
- **CDirectInput**: SDL-based keyboard/mouse input handling
- **CDirectSound/SoundStream**: SDL_mixer-based sound playback
- **CDirectMusic**: SDL_mixer-based music playback (MP3, OGG support)
- **Backend**: SDL2 implementations with DirectX-compatible interface
- **Huffman compression**: Network data compression (platform-independent)

### Sprite System (`Client/SpriteLib/`)
- Sprite animation and rendering with SDL backend
- Texture part management
- Palette manipulation
- Multiple pixel format support (555, 565, 4444, etc.)
- **Migration Status**: DirectX implementations removed, SDL backend active

### UI Framework (`VS_UI/`)
- **Widget System**: Buttons, scrollbars, dialogs
- **Skin Manager**: UI theming
- **Input Editors**: Text input with Korean IME support
- **Game UI**: Race-specific UI (Slayer, Vampire, Ouster)
- **Dialogs**: Shop, storage, exchange, skill tree, etc.

### Game Logic (`Client/`)
- **GameMain**: Main game loop
- **GameUI**: Game UI manager
- **MZone**: Zone/map management
- **MCreature**: Creature/NPC system
- **MPlayer**: Player character management
- **MItem**: Item system
- **MSkill**: Skill system
- **Effect System**: Visual effects generators

### Network (`Client/Packet/`)
- Packet definitions for client-server communication
- Login, gameplay, chat packets

## Build Requirements

### Modern CMake Build (Recommended):
The project uses CMake for cross-platform builds with SDL2 backend.

**Dependencies:**
1. **CMake 3.20+** - Build system
2. **SDL2** - Graphics, input, and platform abstraction
3. **SDL2_image** - Image loading support
4. **SDL2_ttf** - TrueType font rendering (freetype2)
5. **SDL2_mixer** (optional) - Audio playback
6. **C++11 compatible compiler** - Clang, GCC, or MSVC

**Platform Support:**
- ✅ macOS (tested)
- ✅ Linux (should work)
- ⚠️ Windows (use legacy VC6 build or modern MSVC)

### Original VC6 Build (Legacy):
1. **Visual C++ 6.0** (original development environment)
2. **DirectX 9 SDK** - Get from [mirror](https://github.com/opendarkeden/client/raw/data/dx90bsdk.zip)
3. **xerces-c 3.2.3** - XML parsing library [mirror](https://github.com/opendarkeden/client/raw/data/xerces-c-3.2.3.zip)

**Note:** The VC6 build is deprecated. Use the modern CMake build with SDL2.

## Running the Game

1. Extract game data to `DarkEden/` directory
2. Configure server IP in `DarkEden/Data/Info/GameClient.inf`
3. Run with mode argument:
   - `0000000001` - Window mode
   - `0000000002` - Fullscreen
   - `0000000003` - Window mode 1024x768
   - `0000000004` - Fullscreen 1024x768

## Game Features

- **Three Races**: Slayer (human), Vampire, Ouster
- **Isometric View**: 2D sprite-based isometric graphics
- **Skill System**: Race-specific skill trees
- **Item System**: Equipment, consumables, quest items
- **Party/Guild**: Social features
- **PvP**: Player vs Player combat
- **Dungeons**: Instance-based dungeons (Bathory, etc.)

## Configuration Files

- `GameClient.inf` - Server connection settings
- `Language.inf` - Language settings
- `config.txt` - General configuration

## SDL Migration Status

The project has been successfully migrated from Windows + DirectX to SDL2 for cross-platform support.

### Completed Migrations
- ✅ **Graphics**: Direct3D → SDL2 rendering
- ✅ **Input**: DirectInput → SDL2 event handling
- ✅ **Sound**: DirectSound → SDL_mixer
- ✅ **Music**: DirectMusic → SDL_mixer
- ✅ **Text**: Windows GDI → SDL2 + freetype2 (TextSystem)
- ✅ **Platform Abstraction**: Win32 API → SDL2 + basic library

### Architecture
- **DXLib**: Provides DirectX-compatible interface with SDL backend
- **D3DLib**: Minimal compatibility stub (CDirect3D class)
- **SpriteLib**: SDL-based sprite rendering
- **TextSystem**: Modern UTF-8 text rendering
- **basic/**: Cross-platform platform utilities

### Known Limitations
- Some DirectX-specific features are stubbed (e.g., CDirect3D::GetDevice() returns nullptr)
- Code contains many `CDirect3D::IsHAL()` checks (always true in SDL)
- Multiple pixel format variants remain (555, 565) for compatibility
- Some commented-out D3D code remains for reference

### Future Work
- [ ] Remove/comment-out obsolete DirectX code
- [ ] Simplify pixel format handling
- [ ] Reduce `CDirect3D` dependencies throughout codebase
- [ ] Unify sprite classes if pixel format variants can be consolidated

## Development Notes

- Codebase uses Hungarian notation
- Mix of English and Korean comments
- Original code from ~2000-2010 era
- Some legacy copy protection code (EXECryptor, ACProtect) - not functional
- GameGuard anti-cheat integration

## Related Projects

- **Server**: https://github.com/opendarkeden/server
- **Docker Install Guide**: See server repo for deployment instructions
