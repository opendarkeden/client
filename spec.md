# spec.md: 清理 Windows 特定依赖

## 目标

在 SDL2 迁移完成后，清理代码库中遗留的 Windows 特定依赖，统一使用 SDL2 跨平台路径。即使是在 Windows 环境下，也应该使用 SDL2 后端。

## 背景

项目已完成从 Windows + DirectX 到 SDL2 的迁移。但代码中仍存在大量 `#ifdef PLATFORM_WINDOWS` 条件编译块，其中许多是：
1. 废弃的 DirectX/DirectSound 代码
2. Windows GDI 代码（已有 TextSystem 替代）
3. 仅在 Windows 上有意义的代码（如 anti-cheat、注册表等）

## 分析结果

### 代码统计
- **总计 321 处** `#ifdef PLATFORM_WINDOWS` 条件编译
- **涉及 ~60+ 个文件**

### 主要分类

| 类别 | 数量 | 处理策略 |
|------|------|----------|
| 废弃的 DirectX 调用 | 21 | **移除** - SDL2 已替代 |
| DirectSound 清理代码 | 15 | **移除** - SDL_mixer 已替代 |
| GDI 文本渲染 | 多处 | **移除** - TextSystem 已替代 |
| Windows 头文件包含 | 173 | **审查后移除** |
| Anti-cheat/注册表 | 若干 | **保留** - Windows 特有功能 |
| 网络相关 (WSA) | 若干 | **保留** - 平台特定实现 |

### 高影响文件 (按 PLATFORM_WINDOWS 出现次数)

1. **Client/GameMain.cpp** (56 处) - 游戏主循环
2. **Client/MTopView.cpp** (17 处) - 渲染系统
3. **Client/GameInit.cpp** (17 处) - 初始化
4. **Client/MinTr.h** (12 处) - 追踪系统
5. **Client/MTopViewDraw.cpp** (8 处) - 渲染绘制

## 清理范围

### Phase 1: 移除废弃的渲染代码

**目标文件：**
- `Client/MTopView.cpp` - 移除 `if (true)` 模式的空 DirectX 代码块
- `Client/MTopViewDraw.cpp` - 统一渲染路径
- `Client/GameMain.cpp` - 移除 Direct3D 恢复代码

**模式示例：**
```cpp
// 当前代码
#ifdef PLATFORM_WINDOWS
    if (true) {
        // 空 DirectX 代码块
    }
#endif

// 清理后
// (完全移除)
```

### Phase 2: 移除 DirectSound 相关代码

**目标文件：**
- `Client/GameMain.cpp` - DirectSound 清理
- `Client/soundbuf.cpp` - 声音缓冲
- `Client/DXLib/soundbuf.cpp`

**模式示例：**
```cpp
// 当前代码
#ifdef PLATFORM_WINDOWS
    if( g_SDLAudio.IsInit() ) {
        // DirectSound 特定清理
    }
#endif

// 清理后
// SDL_mixer 自动处理，移除此块
```

### Phase 3: 统一文本渲染路径

**目标文件：**
- `VS_UI/src/VS_UI_Base.cpp` - GDI vs TextSystem
- `Client/SpriteLib/*.h` - 移除 windows.h 包含

**模式示例：**
```cpp
// 当前代码
#ifdef PLATFORM_WINDOWS
#include <windows.h>
// 使用 GDI
#else
#include "TextSystem/FontHandleUtil.h"
// 使用 TextSystem
#endif

// 清理后 (Windows 也用 TextSystem)
#include "TextSystem/FontHandleUtil.h"
// 统一使用 TextSystem
```

### Phase 4: 清理头文件包含

**目标文件：**
- 所有包含 `#include <windows.h>` 的文件
- 条件包含 Windows 特定头文件

**模式示例：**
```cpp
// 当前代码
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <MMSystem.h>
#endif

// 清理后
#include "basic/Platform.h"  // 提供必要 stub
```

### Phase 5: 保留必要的平台特定代码

**保留的代码：**
- 网络初始化 (WSAStartup/WSACleanup) - BSD sockets 不需要，但保留兼容性
- 注册表访问 - 仅 Windows 有意义
- Anti-cheat 检测 - 仅 Windows 需要
- 进程检测 - 仅 Windows 有意义

## 不在范围内

- 移除 `basic/Platform.h` 中的 stub 定义（仍然需要兼容性）
- 移除 Windows 平台特定功能（注册表、anti-cheat）
- 修改 CMake 构建系统

## 成功标准

1. ✅ 所有 `#ifdef PLATFORM_WINDOWS` 块已审查
2. ✅ 废弃的 DirectX/DirectSound 代码已移除
3. ✅ GDI 文本渲染代码已移除（统一使用 TextSystem）
4. ✅ 项目在 macOS 上成功编译
5. ✅ 项目在 Windows 上成功编译（使用 SDL2）
6. ✅ 游戏功能测试通过

## 风险评估

| 风险 | 等级 | 缓解措施 |
|------|------|----------|
| Windows 编译失败 | 中 | 每次修改后验证 |
| 运行时功能缺失 | 中 | 功能测试 |
| 性能下降 | 低 | 性能对比测试 |

## 依赖关系

- **前置条件：** SDL2 迁移已完成 ✅
- **前置条件：** TextSystem 已实现 ✅
- **并行任务：** 无