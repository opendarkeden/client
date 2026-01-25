# AGENTS.md

This file provides guidance for AI agents working on the OpenDarkEden client.

## Project Overview

OpenDarkEden is the client for the Dark Eden MMORPG (isometric, sprite-based). The codebase is legacy C++ (VC6 era) with modern CMake support and an ongoing cross-platform migration. The original Windows client uses DirectX 9.

## Repository Structure

```
client/
├── Client/          # Main game client (VC6 project, game logic)
│   ├── D3DLib/      # Direct3D rendering engine
│   ├── DXLib/       # DirectX input/sound/music wrapper
│   ├── SpriteLib/   # Sprite animation system
│   ├── VolumeLib/   # Collision/volume utilities
│   ├── WinLib/      # Win32 utilities
│   ├── Packet/      # Network packet definitions
│   └── ...
├── VS_UI/           # UI framework and widgets
├── basic/           # Cross-platform/basic utilities
├── lib/             # Third-party libs
├── DarkEden/        # Runtime data directory (Data/Info, maps, etc.)
├── build/           # CMake build output (if present)
└── tests/           # Platform test binary output (via CMake)
```

## Build Commands

### Legacy VC6 (Windows)
- Install DirectX 9 SDK and `xerces-c` 3.2.3, then add include/lib paths in VC6.
- Open `Client/Client.dsw` in Visual C++ 6.0 and build.

### CMake (modern)
```bash
# Windows (native backend)
cmake -B build -DUSE_SDL_BACKEND=OFF
cmake --build build

# Windows (SDL backend)
cmake -B build -DUSE_SDL_BACKEND=ON
cmake --build build

# Linux/macOS (SDL backend)
cmake -B build
cmake --build build
```

## Tests

```bash
cd build
./tests/test_platform
```

## Run Notes

- Game data must be present in `DarkEden/` (e.g., `DarkEden/Data/Info/GameClient.inf`).
- Update the server IP/port in `DarkEden/Data/Info/GameClient.inf`.
- The client is typically launched via `fengshen.exe` with a mode argument:
  - `0000000001` windowed
  - `0000000002` fullscreen
  - `0000000003` windowed 1024x768
  - `0000000004` fullscreen 1024x768

## Code Style and Conventions

- Legacy C++ with Hungarian notation is common.
- Mix of English and Korean comments; follow local patterns.
- Prefer minimal changes in legacy subsystems; use existing utilities in `basic/` for cross-platform work.

## Key Components

- **Rendering**: `Client/D3DLib/` (Direct3D device/texture management)
- **Input/Sound**: `Client/DXLib/`
- **Sprites**: `Client/SpriteLib/`
- **UI**: `VS_UI/` and `game/vs_ui/` (if present)
- **Networking**: `Client/Packet/`

## Working on the Project

1. Locate the subsystem in `Client/`, `VS_UI/`, or `basic/` before making changes.
2. Preserve existing patterns and naming conventions.
3. Build with the relevant backend (DirectX or SDL) and run `tests/test_platform` when touching platform code.

## Common Tasks

- Rendering changes: start in `Client/D3DLib/` and confirm texture/mesh usage in `Client/SpriteLib/`.
- UI work: check `VS_UI/src/` for widgets and `game/vs_ui/` for game-specific screens.
- Input/sound: use `Client/DXLib/` and follow existing device initialization patterns.
- Networking: update packet definitions in `Client/Packet/` and trace usage in `Client/`.
- Cross-platform work: prefer `basic/` abstractions and reference `PLATFORM_MIGRATION.md` for guidance.
