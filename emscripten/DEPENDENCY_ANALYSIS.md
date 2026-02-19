# Demo 依赖分析与最小化方案

## 当前状态
- ✅ 编译成功（只有 warnings）
- ❌ 链接失败：约 40-50 个 undefined symbols
- ✅ 主游戏仍可编译（解耦工作有效）

## 缺失符号分类

### 1. **基础工具类**（必须包含，无游戏逻辑依赖）
| Symbol | 文件 | 大小 | 依赖 |
|--------|------|------|------|
| MString::ctor/dtor/op=/SaveToFile/LoadFromFile | MString.cpp | ~500行 | 标准库，无游戏依赖 |
| **建议：直接包含** | | | |

### 2. **图形系统**（已经在 CSDLGraphics 中定义）
| Symbol | 当前位置 | 解决方案 |
|--------|----------|----------|
| ColorDraw::s_bSHIFT_B/G/R | CSDLGraphics::s_bSHIFT_* | namespace alias |
| ColorDraw::s_bSHIFT4_B/G | CSDLGraphics::s_bSHIFT4_* | namespace alias |
| **建议：通过 namespace alias 映射** | | | |

### 3. **特效系统**（demo 后期需要，合理包含）
| Symbol | 文件 | 大小 | 依赖分析 |
|--------|------|------|----------|
| MEffect::SetDelayFrame/SetPixelPosition/SetCount/dtor | MEffect.cpp | ~2000行 | 依赖：MObject, MPalette |
| **建议：包含** | | | demo 需要特效 |

### 4. **动画系统**
| Symbol | 文件 | 大小 | 依赖 |
|--------|------|------|------|
| CAnimationFrame::~CAnimationFrame() | CAnimationFrame.cpp | ~300行 | 依赖轻量 |
| **建议：包含** | | | |

### 5. **玩家相关**（暂时不需要）
| Symbol | 文件 | 大小 | 建议 |
|--------|------|------|------|
| MPlayer::ClearItemCheckBuffer() | MPlayer.cpp | ~8000行 | **暂不包含，使用 stub** |

### 6. **零散对象**（需要单独添加）
| Symbol | 文件 | 大小 | 建议 |
|--------|------|------|------|
| MChaseEffect::MChaseEffect() | MChaseEffect.cpp | ~200行 | 包含 |
| MAnimationObject::ctor | MAnimationObject.cpp | ~100行 | 包含 |
| MShadowAnimationObject::ctor | MShadowAnimationObject.cpp | ~100行 | 包含 |
| MGridItemManager::~MGridItemManager | MItem.cpp | ~6000行 | **使用 stub** |
| MInteractionObject::ctor | MInteractionObject.cpp | ~100行 | 包含 |

## 最小化依赖方案

### Phase 1: 基础必需（立即添加）
```bash
# 基础工具类（无游戏依赖）
../Client/MString.cpp

# 动画系统（依赖轻）
../Client/framelib/CAnimationFrame.cpp

# ColorDraw namespace alias（DemoGlobals.cpp）
namespace ColorDraw {
    using CSDLGraphics::s_bSHIFT_B;
    using CSDLGraphics::s_bSHIFT_G;
    using CSDLGraphics::s_bSHIFT_R;
    using CSDLGraphics::s_bSHIFT4_B;
    using CSDLGraphics::s_bSHIFT4_G;
}
```

### Phase 2: 特效系统（demo 需要）
```bash
# MEffect 系统（demo 后期需要）
../Client/MEffect.cpp
../Client/MAnimationObject.cpp
../Client/MShadowAnimationObject.cpp
../Client/MInteractionObject.cpp
../Client/MChaseEffect.cpp
```

### Phase 3: 使用 stub 代替（暂不需要完整实现）
```cpp
// DemoStubs.cpp - 保留 stub，不包含完整 .cpp
MGridItemManager::~MGridItemManager() {}
MPlayer::ClearItemCheckBuffer() { /* no-op */ }
```

## 关键原则

1. **基础工具类优先** - MString 等无游戏逻辑的优先包含
2. **namespace alias** - ColorDraw 通过 alias 映射，不重复定义
3. **stub vs 完整实现** - 暂时不需要的用 stub，需要的才包含完整代码
4. **逐步添加** - 按 Phase 顺序添加，每次添加后验证

## 预估代码量
- Phase 1: +500行 (MString.cpp)
- Phase 2: +2700行 (MEffect.cpp + 对象文件)
- 总计: ~3200行新增（比主游戏少 90%+）
