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
├── Client/              # Original VC6 project files and main game client
│   ├── D3DLib/         # Direct3D rendering engine
│   ├── DXLib/          # DirectX wrapper (Input, Sound, Music)
│   ├── SpriteLib/      # Sprite animation system
│   ├── VolumeLib/      # Volume/collision detection
│   ├── WinLib/         # Windows API utilities
│   ├── framelib/       # Frame handling
│   ├── DEUtil/         # Dark Eden utilities
│   ├── MZLib/          # Compression library
│   ├── Packet/         # Network packet definitions
│   ├── SXml/           # XML parsing utilities
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
│   └── PlatformUtil.h  # Platform utilities
│
├── lib/                # Third-party libraries
│   ├── BasicS.lib      # Basic utilities library
│   ├── GL.lib          # OpenGL utilities
│   ├── IFC22.lib       # Immersion force feedback
│   ├── PS.lib          # Particle system
│   ├── Timer.lib       # Timer utilities
│   └── rbt.lib         # Red-black tree library
│
├── build/              # CMake build output
├── ../DarkEden/           # Game data directory (runtime)
│   ├── Data/           # Game data files
│   │   ├── Info/       # Configuration files
│   │   ├── Map/        # Map files
│   │   └── ...
│   └── UserSet/        # User settings
│
└── demo/               # Demo applications
```

## Key Components

### Rendering Engine (`Client/D3DLib/`)
- **CDirect3D**: Direct3D device management
- **CD3DObject**: 3D object rendering
- **CD3DTextureEffect**: Texture effects
- **CTexturePack**: Texture packing for optimization

### Input & Sound (`Client/DXLib/`)
- **CDirectInput**: Keyboard/mouse input handling
- **CDirectSound/SoundStream**: Sound playback
- **CDirectMusic**: Music playback (MP3, OGG support)
- **Huffman compression**: Network data compression

### Sprite System (`Client/SpriteLib/`)
- Sprite animation and rendering
- Texture part management
- Palette manipulation

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

### Original VC6 Build:
1. **Visual C++ 6.0** (original development environment)
2. **DirectX 9 SDK** - Get from [mirror](https://github.com/opendarkeden/client/raw/data/dx90bsdk.zip)
3. **xerces-c 3.2.3** - XML parsing library [mirror](https://github.com/opendarkeden/client/raw/data/xerces-c-3.2.3.zip)

### Modern CMake Build:
The project includes CMake support for modern compilation.

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

## Development Notes

- Codebase uses Hungarian notation
- Mix of English and Korean comments
- Original code from ~2000-2010 era
- Some legacy copy protection code (EXECryptor, ACProtect) - not functional
- GameGuard anti-cheat integration

## Related Projects

- **Server**: https://github.com/opendarkeden/server
- **Docker Install Guide**: See server repo for deployment instructions
