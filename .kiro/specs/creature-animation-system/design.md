# Design Document: Creature Animation System

## Overview

本设计文档描述了 Dark Eden 客户端跨平台重写项目第三阶段的技术实现方案。核心目标是：

1. **实现完整的 Creature 动画系统**：支持 ISPK、SSPK、CFPK 文件格式
2. **保持接口兼容**：新实现的 SDL 版本与原始 SpriteLib/FrameLib 接口兼容
3. **最小化上层改动**：MTopView、MCreature 等上层代码只需替换底层库即可工作

### 语言特性原则

- **底层代码**：尽量使用 C 风格，减少对高级 C++ 特性的依赖
- **接口层**：使用 C++ class 保持与原始代码兼容
- **工具层**：可以适度使用 C++ 特性简化开发

具体来说：
- 使用 `FILE*` 而非 `std::ifstream/ofstream`
- 使用简单数组而非 STL 容器
- 使用返回值而非异常处理错误
- 模板仅用于必要的类型泛化（TArray、CTypePack）

## Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                    Upper Layer (Minimal Changes)                     │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐                 │
│  │  MTopView   │  │  MCreature  │  │  MPlayer    │  ...            │
│  └─────────────┘  └─────────────┘  └─────────────┘                 │
├─────────────────────────────────────────────────────────────────────┤
│                    Interface Layer (Compatible)                      │
│  ┌─────────────────────────────────────────────────────────────────┐│
│  │                      SpriteLib Interface                         ││
│  │  CSprite, CIndexSprite, CShadowSprite                           ││
│  │  CSpritePack, CIndexSpritePack, CShadowSpritePack               ││
│  └─────────────────────────────────────────────────────────────────┘│
│  ┌─────────────────────────────────────────────────────────────────┐│
│  │                      FrameLib Interface                          ││
│  │  CFrame, CAnimationFrame, TArray                                ││
│  │  CCreatureFramePack, CEffectFramePack                           ││
│  └─────────────────────────────────────────────────────────────────┘│
├─────────────────────────────────────────────────────────────────────┤
│                    SDL Implementation Layer                          │
│  ┌─────────────────────────────────────────────────────────────────┐│
│  │                    SDL Sprite System                             ││
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐          ││
│  │  │ SDL_Sprite   │  │SDL_IndexSpr  │  │SDL_ShadowSpr │          ││
│  │  └──────────────┘  └──────────────┘  └──────────────┘          ││
│  └─────────────────────────────────────────────────────────────────┘│
│  ┌─────────────────────────────────────────────────────────────────┐│
│  │                    SDL Rendering                                 ││
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐          ││
│  │  │ SDL_Window   │  │SDL_Renderer  │  │ SDL_Texture  │          ││
│  │  └──────────────┘  └──────────────┘  └──────────────┘          ││
│  └─────────────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────────────┘
```

## Project Structure

```
resurrected/
├── lib/
│   ├── types.h                 # 共享类型定义
│   ├── color.h/cpp             # RGB565 颜色处理
│   ├── sprite.h/cpp            # 基础 Sprite (已实现)
│   ├── spritepack.h/cpp        # SpritePack (已实现)
│   ├── index_sprite.h/cpp      # IndexedSprite (新增)
│   ├── index_spritepack.h/cpp  # IndexedSpritePack (新增)
│   ├── shadow_sprite.h/cpp     # ShadowSprite (新增)
│   ├── shadow_spritepack.h/cpp # ShadowSpritePack (新增)
│   ├── colorset.h/cpp          # ColorSet 系统 (新增)
│   ├── frame.h/cpp             # CFrame 兼容实现 (新增)
│   ├── framepack.h/cpp         # FramePack 模板 (新增)
│   ├── tarray.h                # TArray 模板 (新增)
│   ├── animation.h/cpp         # Animation 系统 (已实现)
│   └── sdl_framework.h/cpp     # SDL 框架 (已实现)
├── tools/
│   ├── sprite_viewer/          # Sprite 查看器 (已实现)
│   ├── animation_viewer/       # Animation 查看器 (已实现)
│   └── creature_viewer/        # Creature 查看器 (新增)
└── tests/
    └── ...
```

## Components and Interfaces

### 1. TArray Template (tarray.h)

与原始 TArray 兼容的模板实现。

```cpp
template <typename DataType, typename SizeType>
class TArray {
public:
    TArray(SizeType size = 0);
    ~TArray();
    
    void Init(SizeType size);
    void Release();
    
    SizeType GetSize() const { return m_Size; }
    
    DataType& operator[](SizeType n) { return m_pData[n]; }
    const DataType& operator[](SizeType n) const { return m_pData[n]; }
    
    bool SaveToFile(FILE* file);
    bool LoadFromFile(FILE* file);
    
private:
    SizeType m_Size;
    DataType* m_pData;
};
```

### 2. CFrame (frame.h)

与原始 CFrame 兼容的帧数据结构。

```cpp
// 类型定义 - 与原始代码兼容
typedef uint16_t TYPE_SPRITEID;
typedef uint16_t TYPE_FRAMEID;

#define SPRITEID_NULL 0xFFFF
#define FRAMEID_NULL  0xFFFF

class CFrame {
public:
    CFrame(TYPE_SPRITEID spriteID = 0, short cx = 0, short cy = 0);
    
    void Set(TYPE_SPRITEID spriteID, short cx, short cy);
    
    TYPE_SPRITEID GetSpriteID() const { return m_SpriteID; }
    short GetCX() const { return m_cX; }
    short GetCY() const { return m_cY; }
    
    bool SaveToFile(FILE* file);
    bool LoadFromFile(FILE* file);
    
private:
    TYPE_SPRITEID m_SpriteID;
    short m_cX;
    short m_cY;
};

// Frame 数组类型定义 - 与原始代码兼容
typedef TArray<CFrame, uint16_t>              FRAME_ARRAY;
typedef TArray<FRAME_ARRAY, uint8_t>          DIRECTION_FRAME_ARRAY;
typedef TArray<DIRECTION_FRAME_ARRAY, uint8_t> ACTION_FRAME_ARRAY;
```

### 3. CCreatureFramePack (framepack.h)

Creature 帧包，存储所有 Creature 类型的动画帧数据。

```cpp
template <typename Type>
class CFramePack : public TArray<Type, TYPE_FRAMEID> {
public:
    bool SaveToFile(FILE* packFile, FILE* indexFile);
    bool LoadFromFile(FILE* file);
    bool LoadFromFile(const char* filename);
};

// Creature 帧包 = [CreatureType] -> ACTION_FRAME_ARRAY
class CCreatureFramePack : public CFramePack<ACTION_FRAME_ARRAY> {
public:
    void InfoToFile(const char* filename);  // 调试用
};
```

### 4. IndexedSprite (index_sprite.h)

带索引的 Sprite，支持 ColorSet 换色。

```cpp
class CIndexSprite {
public:
    CIndexSprite();
    ~CIndexSprite();
    
    bool IsInit() const { return m_bInit; }
    void Release();
    
    uint16_t GetWidth() const { return m_Width; }
    uint16_t GetHeight() const { return m_Height; }
    
    bool LoadFromFile(FILE* file);
    
    // 渲染到 SDL
    void Blt(SDL_Renderer* renderer, int x, int y);
    void BltColorSet(SDL_Renderer* renderer, int x, int y, uint16_t colorSet);
    void BltEffect(SDL_Renderer* renderer, int x, int y);
    void BltAlpha(SDL_Renderer* renderer, int x, int y, uint8_t alpha);
    
    // ColorSet 系统
    static void SetColorSet();
    static void SetUsingColorSet(int set1, int set2);
    static uint16_t ColorSet[495][30];  // 颜色查找表
    
private:
    uint16_t m_Width;
    uint16_t m_Height;
    uint16_t** m_Pixels;  // 索引像素数据
    bool m_bInit;
};
```

### 5. ShadowSprite (shadow_sprite.h)

阴影 Sprite。

```cpp
class CShadowSprite {
public:
    CShadowSprite();
    ~CShadowSprite();
    
    bool IsInit() const { return m_bInit; }
    void Release();
    
    uint16_t GetWidth() const { return m_Width; }
    uint16_t GetHeight() const { return m_Height; }
    
    bool LoadFromFile(FILE* file);
    
    // 渲染阴影
    void Blt(SDL_Renderer* renderer, int x, int y);
    void BltAlpha(SDL_Renderer* renderer, int x, int y, uint8_t alpha);
    
private:
    uint16_t m_Width;
    uint16_t m_Height;
    uint8_t** m_Pixels;  // 阴影数据 (alpha/darkness)
    bool m_bInit;
};
```

### 6. SpritePack Templates (spritepack.h)

```cpp
// 通用 SpritePack 模板
template <typename SpriteType>
class CTypePack {
public:
    void Init(uint16_t size);
    void Release();
    
    uint32_t GetSize() const { return m_Size; }
    SpriteType& operator[](uint16_t n);
    SpriteType& Get(uint16_t n);
    
    bool LoadFromFile(FILE* file);
    bool LoadFromFile(const char* filename);
    bool LoadFromFileRunning(const char* filename);  // 延迟加载
    
private:
    SpriteType* m_pData;
    uint16_t m_Size;
    bool m_bRunningLoad;
    FILE* m_file;
    int* m_file_index;
};

typedef CTypePack<CIndexSprite> CIndexSpritePack;
typedef CTypePack<CShadowSprite> CShadowSpritePack;
```

## Data Models

### IndexedSprite 文件格式 (.ispk)

```
ISPK File:
┌────────────────────────────────────────┐
│ SpriteCount (2 bytes)                  │
├────────────────────────────────────────┤
│ IndexedSprite[0]                       │
│   ├─ Width (2 bytes)                   │
│   ├─ Height (2 bytes)                  │
│   └─ IndexedPixelData (RLE encoded)    │
├────────────────────────────────────────┤
│ IndexedSprite[1]                       │
│   ...                                  │
└────────────────────────────────────────┘

ISPKI Index File:
┌────────────────────────────────────────┐
│ SpriteCount (2 bytes)                  │
├────────────────────────────────────────┤
│ FileOffset[0] (4 bytes)                │
│ FileOffset[1] (4 bytes)                │
│ ...                                    │
└────────────────────────────────────────┘
```

### CreatureFramePack 文件格式 (.cfpk)

```
CFPK File:
┌────────────────────────────────────────┐
│ CreatureTypeCount (2 bytes)            │
├────────────────────────────────────────┤
│ ACTION_FRAME_ARRAY[0]                  │
│   ├─ ActionCount (1 byte)              │
│   ├─ DIRECTION_FRAME_ARRAY[0]          │
│   │   ├─ DirectionCount (1 byte)       │
│   │   ├─ FRAME_ARRAY[0]                │
│   │   │   ├─ FrameCount (2 bytes)      │
│   │   │   ├─ CFrame[0]                 │
│   │   │   │   ├─ SpriteID (2 bytes)    │
│   │   │   │   ├─ cX (2 bytes)          │
│   │   │   │   └─ cY (2 bytes)          │
│   │   │   ├─ CFrame[1]                 │
│   │   │   │   ...                      │
│   │   ├─ FRAME_ARRAY[1] (Direction 1)  │
│   │   │   ...                          │
│   │   └─ FRAME_ARRAY[7] (Direction 7)  │
│   ├─ DIRECTION_FRAME_ARRAY[1] (Action 1)│
│   │   ...                              │
├────────────────────────────────────────┤
│ ACTION_FRAME_ARRAY[1] (CreatureType 1) │
│   ...                                  │
└────────────────────────────────────────┘
```

### ColorSet 数据结构

```
ColorSet Table:
┌────────────────────────────────────────┐
│ ColorSet[495][30]                      │
│   - 495 = 33 seeds × 15 modifications  │
│   - 30 = gradation levels              │
│   - Each entry = RGB565 color (2 bytes)│
└────────────────────────────────────────┘

Index to Color Mapping:
  IndexValue → ColorSet[setIndex][gradation] → RGB565
```

## Correctness Properties

*A property is a characteristic or behavior that should hold true across all valid executions of a system.*

### Property 1: TArray 数据一致性

*For any* TArray, after Init(size) and setting values via operator[], GetSize() should return size and operator[] should return the same values that were set.

**Validates: Requirements 4.6**

### Property 2: CFrame 数据存储

*For any* CFrame, after Set(spriteID, cx, cy), GetSpriteID(), GetCX(), GetCY() should return the same values.

**Validates: Requirements 3.4, 3.5**

### Property 3: CFPK 加载正确性

*For any* valid .cfpk file, after LoadFromFile(), the CreatureFramePack should contain the correct number of CreatureTypes, and each ACTION_FRAME_ARRAY should have the correct structure.

**Validates: Requirements 3.1, 3.2, 3.3**

### Property 4: ISPK 延迟加载

*For any* IndexedSpritePack loaded with LoadFromFileRunning(), accessing a sprite via operator[] should load it from file on demand, and subsequent accesses should return the same sprite.

**Validates: Requirements 1.3**

### Property 5: ColorSet 映射一致性

*For any* valid index value and ColorSet configuration, the rendered color should match the ColorSet table lookup.

**Validates: Requirements 6.4, 6.5**

### Property 6: 接口兼容性

*For any* code using the original CSprite::Blt() interface, replacing with SDL implementation should produce visually equivalent output.

**Validates: Requirements 4.1, 4.7**

## Error Handling

### 文件加载错误

- 文件不存在：返回 false，不崩溃
- 文件格式错误：返回 false，释放已分配内存
- 索引越界：返回空/默认值

### 渲染错误

- Sprite 未初始化：跳过渲染
- SDL 纹理创建失败：返回错误码
- 无效 ColorSet：使用默认 ColorSet

## Testing Strategy

### 单元测试

1. **TArray 测试**：Init、Release、operator[]、LoadFromFile
2. **CFrame 测试**：Set、Get、LoadFromFile
3. **FramePack 测试**：LoadFromFile、operator[] 访问
4. **IndexedSprite 测试**：LoadFromFile、ColorSet 渲染

### 属性测试

1. **Property 1**: 随机大小和数据测试 TArray
2. **Property 2**: 随机 CFrame 数据测试
3. **Property 3**: 加载真实 CFPK 文件验证结构

### 集成测试

- 加载 Creature.ispk + Creature.cfpk
- 验证所有 CreatureType 的动画播放
- 与原始客户端截图对比

## Design Decisions

### 关于语言特性分层

采用分层的 C++ 使用策略：

**底层代码（lib/ 核心）- 最小化 C++ 特性：**
1. 使用 C 风格的内存管理（malloc/free 或简单的 new/delete[]）
2. 使用 C 标准库文件 I/O（FILE*, fopen, fread, fwrite）
3. 避免 STL 容器（不用 std::vector, std::map 等）
4. 避免 iostream（不用 std::ifstream, std::ofstream）
5. 避免异常处理（使用返回值表示错误）
6. 模板仅用于必要的类型泛化（如 TArray）
7. 类仅用于封装数据和方法，不用复杂继承

**接口层 - 适度使用 C++：**
1. 使用 class 提供与原始代码兼容的接口
2. 使用简单的模板（TArray、CTypePack）
3. 保持方法签名与原始代码一致

**工具层（tools/）- 可以使用更多 C++ 特性：**
1. 工具代码可以使用 STL 简化开发
2. 但仍避免过度复杂的特性

### 关于接口兼容

保持与原始 SpriteLib/FrameLib 相同的类名和方法签名：
1. 上层代码（MTopView 等）只需更换 include 路径
2. 渲染调用从 DirectDraw 改为 SDL，但接口不变
3. 文件 I/O 使用标准 C FILE* 替代 fstream（更底层、更跨平台）

### 关于 ColorSet

简化 ColorSet 实现：
1. 预计算所有 495×30 颜色值
2. 运行时只做查表，不做实时计算
3. 支持动态切换 ColorSet
4. 使用静态数组而非 STL 容器

### 关于延迟加载

保留原始的 LoadFromFileRunning 机制：
1. 首次访问时从文件加载
2. 加载后缓存在内存
3. 支持部分释放以节省内存
4. 使用 FILE* 而非 ifstream

