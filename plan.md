# plan.md: 清理 Windows 特定依赖

## 技术方案

### 当前状态分析

经过代码审计，发现以下 Windows 特定代码模式：

#### 模式 1: 废弃的 DirectX 代码块 (`if (true)` 模式)
```cpp
#ifdef PLATFORM_WINDOWS
    if (true) {
        // 空的 DirectX 相关代码
    }
#endif
```
**处理：** 完全移除这些空块

#### 模式 2: DirectSound 清理代码
```cpp
#ifdef PLATFORM_WINDOWS
    if( g_SDLAudio.IsInit() ) {
        // DirectSound 清理
    }
#endif
```
**处理：** SDL_mixer 自动处理，移除这些块

#### 模式 3: 条件渲染路径
```cpp
#ifdef PLATFORM_WINDOWS
    // Windows 特定渲染
#else
    // SDL 渲染
#endif
```
**处理：** 统一使用 SDL 路径，移除 Windows 分支

#### 模式 4: 条件头文件包含
```cpp
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <MMSystem.h>
#else
#include "basic/Platform.h"
#endif
```
**处理：** 统一使用 `#include "basic/Platform.h"`

#### 模式 5: GDI vs TextSystem
```cpp
#ifdef PLATFORM_WINDOWS
    HFONT hfont = CreateFontIndirect(&lf);  // GDI
#else
    pi.hfont = TextSystem::EncodeFontSizeHandle(lf.lfHeight);  // TextSystem
#endif
```
**处理：** 统一使用 TextSystem（即使在 Windows 上）

---

## 实施计划

### Phase 1: 清理渲染相关条件编译 (高优先级)

**目标文件：**
1. `Client/MTopView.cpp` (17 处 PLATFORM_WINDOWS)
2. `Client/MTopViewDraw.cpp` (8 处)
3. `Client/GameMain.cpp` (56 处 - 部分渲染相关)

**操作：**
- 移除 `if (true)` 空代码块
- 统一使用 SDL 渲染路径
- 移除废弃的 DirectDraw/Direct3D 调用

### Phase 2: 清理音频相关条件编译

**目标文件：**
1. `Client/GameMain.cpp` - MP3/OGG 播放
2. `Client/soundbuf.cpp`
3. `Client/MMusic.cpp`

**操作：**
- 移除 DirectSound 清理代码
- 统一使用 SDL_mixer 路径

### Phase 3: 统一头文件包含

**目标文件：**
所有包含 `#ifdef PLATFORM_WINDOWS` 的头文件包含块

**操作：**
- 替换 `#include <windows.h>` 为 `#include "basic/Platform.h"`
- 移除 `<MMSystem.h>` 等Windows特定头文件

### Phase 4: 统一文本渲染路径

**目标文件：**
1. `VS_UI/src/VS_UI_Base.cpp`
2. `VS_UI/src/VS_UI_Title.cpp`
3. 其他 UI 文件

**操作：**
- 移除 GDI 字体创建代码
- 统一使用 TextSystem

### Phase 5: 清理保留的平台特定代码

**保留但简化：**
1. 网络初始化 (WSAStartup) - 改为条件调用
2. 注册表访问 - 保留但标记为 Windows-only
3. Anti-cheat 检测 - 保留但标记为 Windows-only

---

## 文件清单

### 需要修改的文件（按优先级排序）

| 优先级 | 文件 | PLATFORM_WINDOWS 数量 | 主要修改 |
|--------|------|----------------------|----------|
| P0 | Client/GameMain.cpp | 56 | 清理渲染/音频条件编译 |
| P0 | Client/MTopView.cpp | 17 | 统一渲染路径 |
| P0 | Client/GameInit.cpp | 17 | 清理初始化条件编译 |
| P1 | Client/MinTr.h | 12 | 清理追踪系统条件编译 |
| P1 | Client/MTopViewDraw.cpp | 8 | 统一渲染路径 |
| P1 | Client/Client.cpp | 8 | 清理客户端条件编译 |
| P2 | Client/PacketFunction.cpp | 7 | 网络函数 |
| P2 | Client/UIMessageManager.cpp | 5 | UI 消息 |
| P2 | Client/ProfileManager.cpp | 4 | 配置文件 |
| P2 | Client/MPlayer.cpp | 4 | 玩家系统 |
| P3 | VS_UI 文件 (多个) | 2-3 每个 | UI 组件 |

### 可能需要删除的文件

无 - 本次清理专注于简化条件编译，不删除文件

---

## 风险评估与缓解

| 风险 | 可能性 | 影响 | 缓解措施 |
|------|--------|------|----------|
| Windows 编译失败 | 中 | 高 | 每个 phase 后在 Windows 上测试编译 |
| 运行时崩溃 | 低 | 高 | macOS 上测试后再修改 Windows |
| 功能丢失 | 低 | 中 | 审查每个移除的代码块 |

---

## 测试策略

1. **编译测试**
   ```bash
   # macOS
   make debug-asan

   # Windows (如有环境)
   cmake --build build --config Debug
   ```

2. **功能测试**
   - 游戏启动
   - UI 显示
   - 文本渲染
   - 音频播放

---

## 执行顺序

1. ✅ 分析代码库 (已完成)
2. Phase 1: 清理渲染相关条件编译
3. Phase 2: 清理音频相关条件编译
4. Phase 3: 统一头文件包含
5. Phase 4: 统一文本渲染路径
6. Phase 5: 清理保留的平台特定代码
7. 验证构建
8. 功能测试