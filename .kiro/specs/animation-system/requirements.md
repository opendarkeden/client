# Requirements Document

## Introduction

本文档定义了 Dark Eden 客户端跨平台重写项目的第二阶段需求：实现 Animation 系统，使 SDL 能够展示与原始游戏相同的动画效果。同时重构项目结构，将代码分为库（lib）和工具（tools）两部分。

## Glossary

- **Animation**: 由多帧 Sprite 组成的动画序列，支持循环播放和方向控制
- **Frame**: 动画中的单帧，对应 SpritePack 中的一个 Sprite
- **FrameID**: 帧组标识符，指向一组动画帧
- **Direction**: 动画方向，支持 8 个方向（左、左下、下、右下、右、右上、上、左上）
- **BltType**: 渲染类型，包括 NORMAL、EFFECT、SHADOW、SCREEN 四种
- **SpriteID**: Sprite 在 SpritePack 中的索引
- **Colorkey**: 透明色键值，默认为 0

## Requirements

### Requirement 1: 项目结构重组

**User Story:** As a developer, I want the project organized into library and tools, so that code can be shared between tools and the game client.

#### Acceptance Criteria

1. THE Build_System SHALL organize source code into lib/ directory for shared library code
2. THE Build_System SHALL organize tool applications into tools/ directory
3. THE Build_System SHALL build a static library from lib/ sources
4. THE Build_System SHALL link tools against the shared library
5. WHEN building, THE Build_System SHALL support both library and tools in a single CMake configuration

### Requirement 2: Animation 帧管理

**User Story:** As a developer, I want to manage animation frames, so that I can control frame progression and timing.

#### Acceptance Criteria

1. THE Animation_Frame SHALL store current frame index and maximum frame count
2. THE Animation_Frame SHALL store frame ID (FrameID) for identifying animation sequences
3. WHEN NextFrame is called, THE Animation_Frame SHALL advance to the next frame
4. WHEN current frame reaches max frame, THE Animation_Frame SHALL wrap to frame 0
5. THE Animation_Frame SHALL support loop mode where frame advances based on global loop counter
6. THE Animation_Frame SHALL store BltType for rendering mode selection

### Requirement 3: Animation 对象

**User Story:** As a developer, I want animation objects that combine sprite and frame data, so that I can render animated game objects.

#### Acceptance Criteria

1. THE Animation_Object SHALL store a SpriteID as the base sprite
2. THE Animation_Object SHALL store pixel position (X, Y) for rendering location
3. THE Animation_Object SHALL store direction (0-7) for 8-directional animations
4. THE Animation_Object SHALL inherit frame management from Animation_Frame
5. WHEN rendering, THE Animation_Object SHALL calculate the actual sprite index from SpriteID + current frame
6. THE Animation_Object SHALL support transparency flag for alpha blending

### Requirement 4: Animation 渲染

**User Story:** As a developer, I want to render animations to SDL, so that animated sprites display correctly on screen.

#### Acceptance Criteria

1. WHEN rendering an animation, THE Animation_Renderer SHALL get the current frame's sprite from SpritePack
2. WHEN rendering, THE Animation_Renderer SHALL apply the correct BltType (NORMAL, EFFECT, SHADOW, SCREEN)
3. FOR BLT_NORMAL, THE Animation_Renderer SHALL render sprite with standard alpha blending
4. FOR BLT_EFFECT, THE Animation_Renderer SHALL render sprite with additive blending
5. FOR BLT_SHADOW, THE Animation_Renderer SHALL render sprite with shadow effect (darkened, semi-transparent)
6. THE Animation_Renderer SHALL render at the specified pixel position with optional zoom

### Requirement 5: Animation 查看器工具

**User Story:** As a developer, I want an animation viewer tool, so that I can verify animation loading and playback works correctly.

#### Acceptance Criteria

1. WHEN the viewer starts, THE Animation_Viewer SHALL accept a sprite pack file path as command line argument
2. WHEN a sprite pack is loaded, THE Animation_Viewer SHALL display the first animation centered on screen
3. WHEN the user presses LEFT/RIGHT arrow keys, THE Animation_Viewer SHALL navigate to previous/next animation
4. WHEN the user presses SPACE, THE Animation_Viewer SHALL toggle animation playback (play/pause)
5. WHEN playing, THE Animation_Viewer SHALL advance frames at configurable frame rate
6. WHEN the user presses 1-4 keys, THE Animation_Viewer SHALL switch between BltTypes
7. WHEN displaying, THE Animation_Viewer SHALL show current frame index, max frames, and BltType
8. WHEN the user presses ESC or closes the window, THE Animation_Viewer SHALL exit cleanly

### Requirement 6: 方向支持

**User Story:** As a developer, I want direction support for animations, so that characters can face different directions.

#### Acceptance Criteria

1. THE Animation_Object SHALL support 8 directions (0-7)
2. WHEN direction changes, THE Animation_Object SHALL update the base SpriteID offset accordingly
3. THE Animation_Viewer SHALL allow direction change via UP/DOWN arrow keys or number keys
4. WHEN displaying, THE Animation_Viewer SHALL show current direction

</content>
</invoke>