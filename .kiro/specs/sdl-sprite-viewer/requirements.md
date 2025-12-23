# Requirements Document

## Introduction

本文档定义了 Dark Eden 客户端跨平台重写项目的第一阶段需求：创建一个基于 SDL2 的游戏框架，能够解析原始游戏的 Sprite 资源文件并在屏幕上展示。这是整个重写项目的基础，后续模块将在此基础上逐步构建。

## Glossary

- **SDL_Framework**: 基于 SDL2 的跨平台游戏框架，负责窗口管理、渲染和事件处理
- **Sprite**: 游戏中的 2D 图像单元，使用 RLE 压缩存储，支持透明色
- **SpritePack**: 包含多个 Sprite 的资源包文件（.spk），配合索引文件（.spki）使用
- **RGB565**: 16 位色彩格式，红色 5 位、绿色 6 位、蓝色 5 位
- **RGB555**: 16 位色彩格式，红色 5 位、绿色 5 位、蓝色 5 位，最高位未使用
- **RLE_Data**: Run-Length Encoding 压缩的精灵像素数据
- **Colorkey**: 透明色键值，原始代码中默认为 0

## Requirements

### Requirement 1: SDL 游戏框架初始化

**User Story:** As a developer, I want to initialize an SDL2 game framework, so that I can create a cross-platform window for rendering sprites.

#### Acceptance Criteria

1. WHEN the application starts, THE SDL_Framework SHALL initialize SDL2 with video subsystem
2. WHEN SDL2 is initialized, THE SDL_Framework SHALL create a window with configurable width and height
3. WHEN the window is created, THE SDL_Framework SHALL create an SDL_Renderer for hardware-accelerated rendering
4. WHEN initialization fails, THE SDL_Framework SHALL return an error code and log the failure reason
5. WHEN the application exits, THE SDL_Framework SHALL properly release all SDL resources

### Requirement 2: 游戏主循环

**User Story:** As a developer, I want a game main loop, so that I can handle events and render frames continuously.

#### Acceptance Criteria

1. THE SDL_Framework SHALL provide a main loop that runs until quit is requested
2. WHEN the main loop runs, THE SDL_Framework SHALL process all pending SDL events
3. WHEN a quit event is received, THE SDL_Framework SHALL set a flag to exit the main loop
4. WHEN each frame begins, THE SDL_Framework SHALL clear the render target
5. WHEN each frame ends, THE SDL_Framework SHALL present the rendered content to the screen
6. THE SDL_Framework SHALL support a configurable target frame rate

### Requirement 3: Sprite 文件格式解析

**User Story:** As a developer, I want to parse the original sprite file format, so that I can load game assets.

#### Acceptance Criteria

1. WHEN loading a sprite file, THE Sprite_Loader SHALL read the width (2 bytes, little-endian)
2. WHEN loading a sprite file, THE Sprite_Loader SHALL read the height (2 bytes, little-endian)
3. WHEN width or height is zero, THE Sprite_Loader SHALL mark the sprite as empty but valid
4. FOR EACH scanline, THE Sprite_Loader SHALL read the line data length (2 bytes)
5. FOR EACH scanline, THE Sprite_Loader SHALL read the RLE compressed pixel data
6. THE Sprite_Loader SHALL parse RLE data format: [count] followed by [trans_count, color_count, pixels...] repeated
7. WHEN parsing RLE data, THE Sprite_Loader SHALL correctly handle transparent runs and color runs

### Requirement 4: SpritePack 文件格式解析

**User Story:** As a developer, I want to parse SpritePack files, so that I can load multiple sprites from a single resource file.

#### Acceptance Criteria

1. WHEN loading a SpritePack file, THE SpritePack_Loader SHALL read the sprite count (2 bytes, little-endian)
2. FOR EACH sprite in the pack, THE SpritePack_Loader SHALL load the sprite using Sprite_Loader
3. WHEN an index file (.spki) exists, THE SpritePack_Loader SHALL support random access loading
4. WHEN loading with index file, THE SpritePack_Loader SHALL read index count (2 bytes) followed by file offsets (4 bytes each)
5. IF a sprite fails to load, THE SpritePack_Loader SHALL continue loading remaining sprites and report the error

### Requirement 5: RGB565 到 RGBA32 颜色转换

**User Story:** As a developer, I want to convert RGB565 colors to RGBA32, so that sprites can be rendered on modern displays.

#### Acceptance Criteria

1. WHEN converting RGB565 to RGBA32, THE Color_Converter SHALL extract red (bits 11-15), green (bits 5-10), blue (bits 0-4)
2. WHEN converting, THE Color_Converter SHALL scale 5-bit red to 8-bit (multiply by 255/31)
3. WHEN converting, THE Color_Converter SHALL scale 6-bit green to 8-bit (multiply by 255/63)
4. WHEN converting, THE Color_Converter SHALL scale 5-bit blue to 8-bit (multiply by 255/31)
5. WHEN the pixel value equals colorkey, THE Color_Converter SHALL set alpha to 0 (fully transparent)
6. WHEN the pixel value does not equal colorkey, THE Color_Converter SHALL set alpha to 255 (fully opaque)

### Requirement 6: Sprite 渲染到 SDL_Texture

**User Story:** As a developer, I want to render sprites to SDL textures, so that they can be displayed on screen.

#### Acceptance Criteria

1. WHEN a sprite is loaded, THE Sprite_Renderer SHALL create an SDL_Texture with RGBA32 format
2. WHEN creating texture, THE Sprite_Renderer SHALL set blend mode to SDL_BLENDMODE_BLEND for transparency
3. WHEN rendering sprite data, THE Sprite_Renderer SHALL decode RLE data and write pixels to texture
4. FOR EACH transparent pixel, THE Sprite_Renderer SHALL write RGBA(0,0,0,0)
5. FOR EACH color pixel, THE Sprite_Renderer SHALL convert RGB565 to RGBA32 and write to texture
6. THE Sprite_Renderer SHALL provide a function to draw the texture at specified screen coordinates

### Requirement 7: 简单的 Sprite 查看器

**User Story:** As a developer, I want a simple sprite viewer application, so that I can verify the sprite loading and rendering works correctly.

#### Acceptance Criteria

1. WHEN the viewer starts, THE Sprite_Viewer SHALL accept a sprite pack file path as command line argument
2. WHEN a sprite pack is loaded, THE Sprite_Viewer SHALL display the first sprite centered on screen
3. WHEN the user presses LEFT/RIGHT arrow keys, THE Sprite_Viewer SHALL navigate to previous/next sprite
4. WHEN the user presses UP/DOWN arrow keys, THE Sprite_Viewer SHALL zoom in/out the current sprite
5. WHEN displaying a sprite, THE Sprite_Viewer SHALL show the current sprite index and total count
6. WHEN the user presses ESC or closes the window, THE Sprite_Viewer SHALL exit cleanly

### Requirement 8: 跨平台构建支持

**User Story:** As a developer, I want the project to build on multiple platforms, so that the game can run on Windows, macOS, and Linux.

#### Acceptance Criteria

1. THE Build_System SHALL use CMake as the build system generator
2. THE Build_System SHALL find and link SDL2 library automatically
3. THE Build_System SHALL compile with C++11 or later standard
4. THE Build_System SHALL produce a single executable with no external dependencies except SDL2
5. WHEN building on different platforms, THE Build_System SHALL handle platform-specific differences automatically
