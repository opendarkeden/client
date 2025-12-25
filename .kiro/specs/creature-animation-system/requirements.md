# Requirements Document

## Introduction

本文档定义了 Dark Eden 客户端跨平台重写项目的第三阶段需求：实现完整的 Creature 动画系统。目标是创建与原始 SpriteLib/FrameLib 接口兼容的 SDL 实现，使上层代码（如 MTopView、MCreature 等）可以最小改动地继续使用。

## Glossary

- **IndexedSpritePack (ISPK)**: 带索引的 Sprite 包，用于存储 Creature 图像，支持 ColorSet 换色
- **ShadowSpritePack (SSPK)**: 阴影 Sprite 包，存储 Creature 的阴影图像
- **CreatureFramePack (CFPK)**: Creature 帧数据包，定义每个 Creature 的 Action/Direction/Frame 对应的 SpriteID
- **CFrame**: 单帧数据，包含 SpriteID 和偏移量 (cX, cY)
- **ACTION_FRAME_ARRAY**: 三维数组 [Action][Direction][Frame]，存储 Creature 的所有动画帧
- **ColorSet**: 颜色集，用于 IndexedSprite 的换色系统
- **BltType**: 渲染类型 (NORMAL, EFFECT, SHADOW, SCREEN)

## Requirements

### Requirement 1: IndexedSpritePack 解析

**User Story:** As a developer, I want to load IndexedSpritePack files, so that I can display Creature sprites with color customization.

#### Acceptance Criteria

1. THE ISPK_Loader SHALL parse .ispk file format (Size(2B) + IndexedSprite×Size)
2. THE ISPK_Loader SHALL parse .ispki index file format (Size(2B) + FileOffset(4B)×Size)
3. THE ISPK_Loader SHALL support lazy loading (load sprite on demand using index)
4. THE IndexedSprite SHALL store width, height, and indexed pixel data
5. THE IndexedSprite SHALL support ColorSet-based color lookup for rendering
6. WHEN rendering, THE IndexedSprite SHALL convert indexed colors to RGB565 using ColorSet table

### Requirement 2: ShadowSpritePack 解析

**User Story:** As a developer, I want to load ShadowSpritePack files, so that I can display Creature shadows.

#### Acceptance Criteria

1. THE SSPK_Loader SHALL parse .sspk file format
2. THE SSPK_Loader SHALL parse .sspki index file format
3. THE ShadowSprite SHALL store shadow pixel data (alpha/darkness values)
4. WHEN rendering, THE ShadowSprite SHALL apply shadow effect to destination

### Requirement 3: CreatureFramePack 解析

**User Story:** As a developer, I want to load CreatureFramePack files, so that I can access animation frame definitions.

#### Acceptance Criteria

1. THE CFPK_Loader SHALL parse .cfpk file format
2. THE CFPK_Loader SHALL parse .cfpki index file format
3. THE CreatureFramePack SHALL store ACTION_FRAME_ARRAY structure: [CreatureType][Action][Direction][Frame]
4. WHEN accessing a frame, THE CreatureFramePack SHALL return CFrame with SpriteID, cX, cY
5. THE CFrame SHALL provide GetSpriteID(), GetCX(), GetCY() methods compatible with original interface

### Requirement 4: 接口兼容层

**User Story:** As a developer, I want SDL implementations that match original SpriteLib/FrameLib interfaces, so that upper-layer code can work with minimal changes.

#### Acceptance Criteria

1. THE SDL_Implementation SHALL provide CSprite interface with Blt(), BltEffect(), BltAlpha() methods
2. THE SDL_Implementation SHALL provide CIndexSprite interface with ColorSet support
3. THE SDL_Implementation SHALL provide CShadowSprite interface
4. THE SDL_Implementation SHALL provide CSpritePack, CIndexSpritePack, CShadowSpritePack interfaces
5. THE SDL_Implementation SHALL provide CCreatureFramePack interface with operator[] access
6. THE SDL_Implementation SHALL provide TArray template compatible with original
7. WHEN Blt is called, THE SDL_Implementation SHALL render to SDL_Renderer instead of DirectDraw surface

### Requirement 5: Creature 动画查看器

**User Story:** As a developer, I want a Creature animation viewer tool, so that I can verify the complete animation system works correctly.

#### Acceptance Criteria

1. WHEN the viewer starts, THE Creature_Viewer SHALL load Creature.ispk, Creature.cfpk, and optionally Creature.sspk
2. WHEN loaded, THE Creature_Viewer SHALL display the first Creature type centered on screen
3. WHEN the user presses LEFT/RIGHT arrow keys, THE Creature_Viewer SHALL navigate to previous/next Creature type
4. WHEN the user presses UP/DOWN arrow keys, THE Creature_Viewer SHALL change Action (STAND, WALK, ATTACK, etc.)
5. WHEN the user presses 1-8 keys, THE Creature_Viewer SHALL change Direction (0-7)
6. WHEN the user presses SPACE, THE Creature_Viewer SHALL toggle animation playback
7. WHEN the user presses S, THE Creature_Viewer SHALL toggle shadow display
8. WHEN the user presses C, THE Creature_Viewer SHALL cycle through ColorSets
9. WHEN displaying, THE Creature_Viewer SHALL show CreatureType, Action, Direction, Frame, ColorSet info
10. WHEN the user presses ESC, THE Creature_Viewer SHALL exit cleanly

### Requirement 6: ColorSet 系统

**User Story:** As a developer, I want ColorSet support, so that Creatures can have different color variations.

#### Acceptance Criteria

1. THE ColorSet_System SHALL load color table from IndexTable file
2. THE ColorSet_System SHALL support 495 ColorSets (33 seeds × 15 modifications)
3. THE ColorSet_System SHALL support 30 gradation levels per ColorSet
4. WHEN SetUsingColorSet is called, THE ColorSet_System SHALL update the active color mapping
5. WHEN rendering IndexedSprite, THE ColorSet_System SHALL apply the active ColorSet

