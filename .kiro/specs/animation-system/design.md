# Design Document: Animation System

## Overview

本设计文档描述了 Dark Eden 客户端跨平台重写项目第二阶段的技术实现方案。目标是实现 Animation 系统，使 SDL 能够展示与原始游戏相同的动画效果，并重构项目结构。

设计遵循以下原则：
- 使用 C++ 语言但保持 C 风格的简单直接
- 不使用 STL、iostream 等 C++ 高级特性
- 模块化设计，便于后续扩展
- 跨平台兼容（Windows、macOS、Linux）

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                        Tools                                 │
│  ┌─────────────────┐  ┌─────────────────────────────────┐   │
│  │  Sprite Viewer  │  │      Animation Viewer           │   │
│  └─────────────────┘  └─────────────────────────────────┘   │
├─────────────────────────────────────────────────────────────┤
│                     Shared Library                           │
│  ┌─────────────────────────────────────────────────────────┐│
│  │                    Animation System                      ││
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ││
│  │  │ AnimFrame    │  │ AnimObject   │  │ AnimRenderer │  ││
│  │  └──────────────┘  └──────────────┘  └──────────────┘  ││
│  └─────────────────────────────────────────────────────────┘│
│  ┌─────────────────────────────────────────────────────────┐│
│  │                    SDL Framework                         ││
│  │  ┌──────────┐  ┌──────────┐  ┌──────────┐              ││
│  │  │  Window  │  │ Renderer │  │  Events  │              ││
│  │  └──────────┘  └──────────┘  └──────────┘              ││
│  └─────────────────────────────────────────────────────────┘│
│  ┌─────────────────────────────────────────────────────────┐│
│  │                   Resource Loaders                       ││
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ││
│  │  │ Sprite       │  │ SpritePack   │  │ Color        │  ││
│  │  └──────────────┘  └──────────────┘  └──────────────┘  ││
│  └─────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────┘
```

## Project Structure

```
project/
├── CMakeLists.txt              # 主 CMake 配置
├── lib/                        # 共享库源码
│   ├── CMakeLists.txt
│   ├── color.h / color.cpp
│   ├── sprite.h / sprite.cpp
│   ├── spritepack.h / spritepack.cpp
│   ├── sdl_framework.h / sdl_framework.cpp
│   ├── animation.h / animation.cpp
│   └── types.h                 # 共享类型定义
├── tools/                      # 工具应用
│   ├── CMakeLists.txt
│   ├── sprite_viewer/
│   │   └── main.cpp
│   └── animation_viewer/
│       └── main.cpp
└── tests/                      # 测试代码
    ├── CMakeLists.txt
    └── ...
```

## Components and Interfaces

### 1. Types Definition (types.h)

共享类型定义，从原始代码移植。

```cpp
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

// Sprite and Frame IDs
typedef uint16_t SpriteID;
typedef uint16_t FrameID;

#define SPRITEID_NULL   0xFFFF
#define FRAMEID_NULL    0xFFFF

// Directions (8 directions)
enum Direction {
    DIR_LEFT = 0,
    DIR_LEFTDOWN,
    DIR_DOWN,
    DIR_RIGHTDOWN,
    DIR_RIGHT,
    DIR_RIGHTUP,
    DIR_UP,
    DIR_LEFTUP,
    DIR_MAX
};

// Blit types for rendering
enum BltType {
    BLT_NORMAL = 0,
    BLT_EFFECT,
    BLT_SHADOW,
    BLT_SCREEN
};

#endif // TYPES_H
```

### 2. Animation Frame (animation.h)

帧管理，移植自 CAnimationFrame。

```cpp
// Animation frame state
struct AnimFrame {
    FrameID frame_id;       // Frame group identifier
    uint8_t current_frame;  // Current frame index
    uint8_t max_frame;      // Maximum frame count
    uint8_t blt_type;       // Rendering type (BltType)
    bool loop;              // Loop mode flag
};

// Initialize animation frame
void anim_frame_init(AnimFrame* frame, uint8_t blt_type);

// Set frame sequence
void anim_frame_set(AnimFrame* frame, FrameID id, uint8_t max_frame);

// Advance to next frame
void anim_frame_next(AnimFrame* frame);

// Advance frame in loop mode (uses global counter)
void anim_frame_next_loop(AnimFrame* frame, uint32_t loop_counter);

// Get current frame index
uint8_t anim_frame_get(const AnimFrame* frame);
```

### 3. Animation Object (animation.h)

动画对象，移植自 MAnimationObject。

```cpp
// Animation object
struct AnimObject {
    AnimFrame frame;        // Frame management
    SpriteID sprite_id;     // Base sprite ID
    int pixel_x;            // X position in pixels
    int pixel_y;            // Y position in pixels
    uint8_t direction;      // Direction (0-7)
    bool transparent;       // Transparency flag
};

// Initialize animation object
void anim_object_init(AnimObject* obj);

// Set animation object properties
void anim_object_set(AnimObject* obj, SpriteID sprite_id, 
                     int px, int py, uint8_t direction, bool trans);

// Set frame sequence for animation
void anim_object_set_frame(AnimObject* obj, FrameID frame_id, uint8_t max_frame);

// Advance animation frame
void anim_object_next_frame(AnimObject* obj);

// Get current sprite ID (base + frame offset)
SpriteID anim_object_get_sprite(const AnimObject* obj);

// Set direction
void anim_object_set_direction(AnimObject* obj, uint8_t dir);
```

### 4. Animation Renderer (animation.h)

动画渲染器。

```cpp
// Render animation object to screen
// Returns 0 on success, negative on error
int anim_render(SDL_Renderer* renderer, 
                const AnimObject* obj,
                SpritePack* pack,
                DecodedSprite* cache,  // Optional sprite cache
                uint16_t colorkey,
                float zoom);

// Render with specific blend mode
int anim_render_blt(SDL_Renderer* renderer,
                    const AnimObject* obj,
                    SpritePack* pack,
                    DecodedSprite* cache,
                    uint16_t colorkey,
                    float zoom,
                    BltType blt_type);

// Set SDL blend mode based on BltType
void anim_set_blend_mode(SDL_Texture* texture, BltType blt_type);
```

## Data Models

### Animation Frame 数据

```
AnimFrame:
┌────────────────────────────────────────┐
│ frame_id (2 bytes) - 帧组标识          │
├────────────────────────────────────────┤
│ current_frame (1 byte) - 当前帧索引    │
├────────────────────────────────────────┤
│ max_frame (1 byte) - 最大帧数          │
├────────────────────────────────────────┤
│ blt_type (1 byte) - 渲染类型           │
├────────────────────────────────────────┤
│ loop (1 byte) - 循环模式标志           │
└────────────────────────────────────────┘
```

### Animation Object 数据

```
AnimObject:
┌────────────────────────────────────────┐
│ AnimFrame frame - 帧管理数据           │
├────────────────────────────────────────┤
│ sprite_id (2 bytes) - 基础 Sprite ID   │
├────────────────────────────────────────┤
│ pixel_x (4 bytes) - X 像素位置         │
├────────────────────────────────────────┤
│ pixel_y (4 bytes) - Y 像素位置         │
├────────────────────────────────────────┤
│ direction (1 byte) - 方向 (0-7)        │
├────────────────────────────────────────┤
│ transparent (1 byte) - 透明标志        │
└────────────────────────────────────────┘
```

### BltType 渲染模式

```
BLT_NORMAL (0):
  - 标准 alpha 混合
  - SDL_BLENDMODE_BLEND

BLT_EFFECT (1):
  - 加法混合，用于发光效果
  - SDL_BLENDMODE_ADD

BLT_SHADOW (2):
  - 阴影效果，降低亮度和透明度
  - SDL_BLENDMODE_BLEND + color mod (dark)

BLT_SCREEN (3):
  - 屏幕混合效果
  - 自定义混合或 SDL_BLENDMODE_ADD
```

### Sprite 索引计算

对于带方向的动画：
```
actual_sprite_id = base_sprite_id + (direction * frames_per_direction) + current_frame
```

对于简单动画：
```
actual_sprite_id = base_sprite_id + current_frame
```

## Correctness Properties

*A property is a characteristic or behavior that should hold true across all valid executions of a system-essentially, a formal statement about what the system should do. Properties serve as the bridge between human-readable specifications and machine-verifiable correctness guarantees.*

### Property 1: AnimFrame 数据存储一致性

*For any* AnimFrame, after initialization and setting values (frame_id, max_frame, blt_type), getting those values should return the same values that were set.

**Validates: Requirements 2.1, 2.2, 2.6**

### Property 2: 帧循环正确性

*For any* AnimFrame with max_frame > 0, calling anim_frame_next() exactly max_frame times should return current_frame to 0, and at any point current_frame should be in range [0, max_frame-1].

**Validates: Requirements 2.3, 2.4**

### Property 3: 循环模式帧计算

*For any* AnimFrame in loop mode with max_frame > 0, the current frame should equal (loop_counter % max_frame).

**Validates: Requirements 2.5**

### Property 4: AnimObject 数据存储一致性

*For any* AnimObject, after setting sprite_id, pixel position, direction, and transparency, getting those values should return the same values that were set.

**Validates: Requirements 3.1, 3.2, 3.6**

### Property 5: 方向范围有效性

*For any* direction value set on AnimObject, the stored direction should always be in range [0, 7] (automatically wrapped if out of range).

**Validates: Requirements 3.3, 6.1**

### Property 6: Sprite ID 计算正确性

*For any* AnimObject with base sprite_id S, max_frame M, and current_frame F, anim_object_get_sprite() should return S + F where F is in [0, M-1]. When direction D is set, the calculation should account for direction offset.

**Validates: Requirements 3.5, 6.2**

### Property 7: BltType 混合模式映射

*For any* BltType value in [BLT_NORMAL, BLT_EFFECT, BLT_SHADOW, BLT_SCREEN], anim_set_blend_mode() should set a valid SDL blend mode corresponding to that type.

**Validates: Requirements 4.2, 4.3, 4.4, 4.5**

## Error Handling

### 初始化错误

- AnimFrame/AnimObject 初始化：设置默认值，不会失败
- 无效参数：使用默认值或忽略

### 渲染错误

- Sprite 不存在：跳过渲染，返回错误码
- 纹理创建失败：返回错误码，不崩溃
- 无效 BltType：使用 BLT_NORMAL 作为默认

### 边界检查

- 帧索引越界：自动回绕到 0
- 方向越界：取模 DIR_MAX
- Sprite ID 越界：返回 NULL

## Testing Strategy

### 单元测试

1. **帧管理测试**
   - 测试帧递增和回绕
   - 测试循环模式
   - 测试边界条件

2. **动画对象测试**
   - 测试属性设置
   - 测试 Sprite ID 计算
   - 测试方向变化

3. **渲染测试**
   - 测试各种 BltType
   - 测试混合模式设置

### 属性测试

1. **Property 1 测试**: 生成随机 max_frame，验证帧循环
2. **Property 2 测试**: 生成随机方向值，验证范围
3. **Property 3 测试**: 生成随机动画配置，验证 Sprite ID 计算
4. **Property 4 测试**: 遍历所有 BltType，验证混合模式

### 集成测试

- 加载真实游戏资源
- 验证动画播放效果
- 与原始客户端对比

## Design Decisions

### 关于基础库

保留现有的 map.h、vector.h、str.h 宏实现。原因：
1. 当前实现已经可用且经过测试
2. C++ 模板会增加编译复杂度
3. 保持与 C 风格一致
4. 后续如有需要可以逐步替换

### 关于项目结构

采用 lib/ + tools/ 分离：
1. lib/ 编译为静态库 libdarkeden.a
2. tools/ 中的每个工具链接该库
3. 便于后续游戏客户端复用库代码

### 关于渲染

使用 SDL2 的混合模式实现 BltType：
1. BLT_NORMAL → SDL_BLENDMODE_BLEND
2. BLT_EFFECT → SDL_BLENDMODE_ADD
3. BLT_SHADOW → SDL_BLENDMODE_BLEND + SDL_SetTextureColorMod(dark)
4. BLT_SCREEN → SDL_BLENDMODE_ADD（近似实现）

</content>
</invoke>