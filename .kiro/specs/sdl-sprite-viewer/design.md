# Design Document: SDL Sprite Viewer

## Overview

本设计文档描述了 Dark Eden 客户端跨平台重写项目第一阶段的技术实现方案。目标是创建一个基于 SDL2 的精灵查看器，能够解析原始游戏的 Sprite 资源文件并渲染到屏幕上。

设计遵循以下原则：
- 使用 C++ 但保持 C 风格的简单直接
- 模块化设计，便于后续扩展
- 跨平台兼容（Windows、macOS、Linux）

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     Sprite Viewer App                        │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐  │
│  │   Input     │  │   Render    │  │    Sprite Manager   │  │
│  │   Handler   │  │   System    │  │                     │  │
│  └─────────────┘  └─────────────┘  └─────────────────────┘  │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────────────────────────────────────────────────┐│
│  │                    SDL Framework                         ││
│  │  ┌──────────┐  ┌──────────┐  ┌──────────┐              ││
│  │  │  Window  │  │ Renderer │  │  Events  │              ││
│  │  └──────────┘  └──────────┘  └──────────┘              ││
│  └─────────────────────────────────────────────────────────┘│
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────────────────────────────────────────────────┐│
│  │                   Resource Loaders                       ││
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ││
│  │  │ Sprite Loader│  │ SpritePack   │  │ Color        │  ││
│  │  │              │  │ Loader       │  │ Converter    │  ││
│  │  └──────────────┘  └──────────────┘  └──────────────┘  ││
│  └─────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────┘
```

## Components and Interfaces

### 1. SDL Framework (sdl_framework.h/c)

负责 SDL2 的初始化、窗口管理和主循环。

```c
// 框架配置
typedef struct {
    int window_width;
    int window_height;
    const char* window_title;
    int target_fps;
} SDLFrameworkConfig;

// 框架状态
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int running;
    uint32_t frame_delay;
} SDLFramework;

// 初始化框架
int sdl_framework_init(SDLFramework* fw, const SDLFrameworkConfig* config);

// 释放框架资源
void sdl_framework_cleanup(SDLFramework* fw);

// 处理事件，返回 0 表示继续运行，非 0 表示退出
int sdl_framework_poll_events(SDLFramework* fw, SDL_Event* event);

// 开始帧渲染（清屏）
void sdl_framework_begin_frame(SDLFramework* fw);

// 结束帧渲染（呈现）
void sdl_framework_end_frame(SDLFramework* fw);

// 帧率控制
void sdl_framework_delay(SDLFramework* fw, uint32_t frame_start);
```

### 2. Color Converter (color.h/c)

处理 RGB565/RGB555 到 RGBA32 的颜色转换。

```c
// RGBA32 颜色结构
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} RGBA32;

// RGB565 转 RGBA32
RGBA32 rgb565_to_rgba32(uint16_t color, uint16_t colorkey);

// RGB555 转 RGBA32
RGBA32 rgb555_to_rgba32(uint16_t color, uint16_t colorkey);

// 批量转换（用于优化）
void rgb565_to_rgba32_batch(const uint16_t* src, uint32_t* dst, 
                            int count, uint16_t colorkey);
```

### 3. Sprite Loader (sprite.h/c)

解析原始精灵文件格式。

```c
// 精灵数据结构
typedef struct {
    uint16_t width;
    uint16_t height;
    uint16_t** pixels;      // 每行的 RLE 数据
    int is_valid;
} Sprite;

// 解码后的精灵（用于渲染）
typedef struct {
    uint16_t width;
    uint16_t height;
    uint32_t* pixels;       // RGBA32 像素数据
    SDL_Texture* texture;   // SDL 纹理（可选）
} DecodedSprite;

// 从文件流加载精灵
int sprite_load(Sprite* sprite, FILE* file);

// 释放精灵资源
void sprite_free(Sprite* sprite);

// 解码精灵到 RGBA32 像素数据
int sprite_decode(const Sprite* sprite, DecodedSprite* decoded, 
                  uint16_t colorkey);

// 释放解码后的精灵
void decoded_sprite_free(DecodedSprite* decoded);

// 创建 SDL 纹理
int decoded_sprite_create_texture(DecodedSprite* decoded, 
                                  SDL_Renderer* renderer);
```

### 4. SpritePack Loader (spritepack.h/c)

加载精灵包文件。

```c
// 精灵包结构
typedef struct {
    uint16_t count;
    Sprite* sprites;
    int* file_offsets;      // 索引文件中的偏移量（可选）
    FILE* pack_file;        // 用于延迟加载
    int lazy_load;          // 是否延迟加载
} SpritePack;

// 加载精灵包（完整加载）
int spritepack_load(SpritePack* pack, const char* filename);

// 加载精灵包（延迟加载，需要索引文件）
int spritepack_load_lazy(SpritePack* pack, const char* filename);

// 获取精灵（延迟加载时会触发实际加载）
Sprite* spritepack_get(SpritePack* pack, uint16_t index);

// 释放精灵包
void spritepack_free(SpritePack* pack);
```

### 5. Sprite Viewer (viewer.h/c)

精灵查看器应用逻辑。

```c
// 查看器状态
typedef struct {
    SDLFramework framework;
    SpritePack pack;
    DecodedSprite* decoded_sprites;
    int current_index;
    float zoom;
    int show_info;
} SpriteViewer;

// 初始化查看器
int viewer_init(SpriteViewer* viewer, const char* pack_file, 
                int width, int height);

// 处理输入
void viewer_handle_input(SpriteViewer* viewer, SDL_Event* event);

// 渲染当前帧
void viewer_render(SpriteViewer* viewer);

// 运行主循环
void viewer_run(SpriteViewer* viewer);

// 清理资源
void viewer_cleanup(SpriteViewer* viewer);
```

## Data Models

### Sprite 文件格式（.spk）

原始精灵文件格式分析（基于 CSprite565::LoadFromFile）：

```
Sprite File Format:
┌────────────────────────────────────────┐
│ Width (2 bytes, uint16_t)              │
├────────────────────────────────────────┤
│ Height (2 bytes, uint16_t)             │
├────────────────────────────────────────┤
│ Scanline 0:                            │
│   ├─ Length (2 bytes, uint16_t)        │
│   └─ RLE Data (Length * 2 bytes)       │
├────────────────────────────────────────┤
│ Scanline 1:                            │
│   ├─ Length (2 bytes, uint16_t)        │
│   └─ RLE Data (Length * 2 bytes)       │
├────────────────────────────────────────┤
│ ... (repeat for each scanline)         │
└────────────────────────────────────────┘
```

### RLE 数据格式

每行的 RLE 数据格式（基于 CSprite::Blt 分析）：

```
RLE Line Format:
┌────────────────────────────────────────┐
│ Count (1 WORD) - 重复段数量            │
├────────────────────────────────────────┤
│ Segment 0:                             │
│   ├─ Trans Count (1 WORD) - 透明像素数 │
│   ├─ Color Count (1 WORD) - 颜色像素数 │
│   └─ Pixels (Color Count WORDs)        │
├────────────────────────────────────────┤
│ Segment 1:                             │
│   ├─ Trans Count                       │
│   ├─ Color Count                       │
│   └─ Pixels                            │
├────────────────────────────────────────┤
│ ... (repeat Count times)               │
└────────────────────────────────────────┘
```

### SpritePack 文件格式

```
SpritePack File Format (.spk):
┌────────────────────────────────────────┐
│ Sprite Count (2 bytes, uint16_t)       │
├────────────────────────────────────────┤
│ Sprite 0 Data                          │
├────────────────────────────────────────┤
│ Sprite 1 Data                          │
├────────────────────────────────────────┤
│ ... (repeat for each sprite)           │
└────────────────────────────────────────┘

Index File Format (.spki):
┌────────────────────────────────────────┐
│ Sprite Count (2 bytes, uint16_t)       │
├────────────────────────────────────────┤
│ Offset 0 (4 bytes, uint32_t)           │
├────────────────────────────────────────┤
│ Offset 1 (4 bytes, uint32_t)           │
├────────────────────────────────────────┤
│ ... (repeat for each sprite)           │
└────────────────────────────────────────┘
```

### RGB565 颜色格式

```
RGB565 (16-bit):
┌─────┬──────┬─────┐
│ R   │  G   │  B  │
│5bit │ 6bit │5bit │
└─────┴──────┴─────┘
 15-11  10-5   4-0

Conversion to RGBA32:
R8 = (R5 * 255) / 31
G8 = (G6 * 255) / 63
B8 = (B5 * 255) / 31
A8 = (color == colorkey) ? 0 : 255
```

## Correctness Properties

*A property is a characteristic or behavior that should hold true across all valid executions of a system-essentially, a formal statement about what the system should do. Properties serve as the bridge between human-readable specifications and machine-verifiable correctness guarantees.*

### Property 1: RGB565 颜色转换正确性

*For any* RGB565 color value, converting to RGBA32 should correctly extract red (bits 11-15), green (bits 5-10), and blue (bits 0-4), and scale each component to 8-bit range.

**Validates: Requirements 5.1, 5.2, 5.3, 5.4**

### Property 2: Colorkey 透明度处理

*For any* pixel value and colorkey, if the pixel equals colorkey then alpha should be 0, otherwise alpha should be 255.

**Validates: Requirements 5.5, 5.6**

### Property 3: Sprite 解析往返一致性

*For any* valid sprite data, decoding the RLE data and re-encoding should produce equivalent pixel output. Specifically, for each scanline, the decoded pixel count should equal the sprite width, and transparent/color runs should be correctly reconstructed.

**Validates: Requirements 3.1, 3.2, 3.4, 3.5, 3.6, 3.7**

### Property 4: SpritePack 解析完整性

*For any* valid SpritePack file, loading should correctly read the sprite count, and for each sprite index from 0 to count-1, the sprite should be loadable and have valid dimensions.

**Validates: Requirements 4.1, 4.2, 4.4**

## Error Handling

### 初始化错误

- SDL 初始化失败：返回错误码，记录 SDL_GetError() 信息
- 窗口创建失败：清理已分配资源，返回错误码
- 渲染器创建失败：清理窗口和 SDL，返回错误码

### 文件加载错误

- 文件不存在：返回 -1，设置错误信息
- 文件格式错误：返回 -2，记录具体位置
- 内存分配失败：返回 -3，清理已分配资源
- 精灵数据损坏：跳过该精灵，继续加载其他精灵

### 运行时错误

- 纹理创建失败：使用软件渲染回退
- 索引越界：返回 NULL，不崩溃

## Testing Strategy

### 单元测试

使用简单的测试框架验证核心功能：

1. **颜色转换测试**
   - 测试边界值（0x0000, 0xFFFF）
   - 测试已知颜色值
   - 测试 colorkey 处理

2. **RLE 解码测试**
   - 测试空精灵
   - 测试单行精灵
   - 测试全透明行
   - 测试全颜色行

3. **文件解析测试**
   - 测试有效文件
   - 测试损坏文件
   - 测试空文件

### 属性测试

使用属性测试库（如 theft 或自定义生成器）验证正确性属性：

1. **Property 1 测试**: 生成随机 RGB565 值，验证转换正确性
2. **Property 2 测试**: 生成随机像素和 colorkey，验证 alpha 值
3. **Property 3 测试**: 生成随机精灵数据，验证解码正确性
4. **Property 4 测试**: 生成随机 SpritePack，验证加载完整性

### 集成测试

- 加载真实游戏资源文件
- 验证渲染输出
- 性能基准测试

## File Structure

```
src/
├── main.c              # 程序入口
├── sdl_framework.h     # SDL 框架头文件
├── sdl_framework.c     # SDL 框架实现
├── color.h             # 颜色转换头文件
├── color.c             # 颜色转换实现
├── sprite.h            # 精灵加载头文件
├── sprite.c            # 精灵加载实现
├── spritepack.h        # 精灵包头文件
├── spritepack.c        # 精灵包实现
├── viewer.h            # 查看器头文件
└── viewer.c            # 查看器实现

tests/
├── test_color.c        # 颜色转换测试
├── test_sprite.c       # 精灵解析测试
└── test_spritepack.c   # 精灵包测试

CMakeLists.txt          # CMake 构建配置
README.md               # 项目说明
```
