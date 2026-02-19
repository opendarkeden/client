# tasks.md: 清理 Windows 特定依赖

## 状态说明
- [ ] 未开始
- [~] 进行中
- [x] 已完成
- [!] 阻塞/需要讨论

---

## Phase 1: 清理渲染相关条件编译

### 1.1 Client/MTopView.cpp ✅
- [x] 移除 `#ifdef PLATFORM_WINDOWS` 包裹的空 `if (true)` 代码块
- [x] 统一渲染路径，移除 Windows 分支
- [x] 移除 DirectDraw/Direct3D 相关调用
- [x] 测试渲染功能
- [x] 编译验证通过

### 1.2 Client/MTopViewDraw.cpp ✅
- [x] 移除条件渲染路径
- [x] 统一使用 SDL 渲染
- [x] 测试绘制功能
- [x] 编译验证通过

### 1.3 Client/GameInit.cpp
- [ ] 移除 DirectDraw/Direct3D 初始化
- [ ] 统一显示模式设置
- [ ] 测试初始化流程

### 1.4 Client/MinTr.h
- [ ] 清理追踪系统中的 Windows 特定代码
- [ ] 统一使用跨平台实现

---

## Phase 2: 清理音频相关条件编译

### 2.1 Client/GameMain.cpp (音频部分)
- [ ] 移除 DirectSound 清理代码
- [ ] 统一使用 SDL_mixer 路径
- [ ] 测试 MP3/OGG 播放

### 2.2 Client/soundbuf.cpp
- [ ] 清理 DirectSound 相关代码
- [ ] 统一使用 SDL 后端

### 2.3 Client/MMusic.cpp
- [ ] 清理 DirectMusic 相关代码
- [ ] 统一使用 SDL_mixer

---

## Phase 3: 统一头文件包含

### 3.1 Client/GameMain.cpp
- [ ] 替换条件 `#include <windows.h>` 为 `#include "basic/Platform.h"`
- [ ] 移除 `<MMSystem.h>` 包含

### 3.2 Client/Client.cpp
- [ ] 统一头文件包含

### 3.3 Client/GameInit.cpp
- [ ] 统一头文件包含

### 3.4 VS_UI 文件
- [ ] 清理 VS_UI/src/*.cpp 中的头文件包含
- [ ] 统一使用 Platform.h

---

## Phase 4: 统一文本渲染路径

### 4.1 VS_UI/src/VS_UI_Base.cpp
- [ ] 移除 GDI 字体创建代码
- [ ] 统一使用 TextSystem

### 4.2 VS_UI/src/VS_UI_Title.cpp
- [ ] 清理 GDI 相关代码
- [ ] 使用 TextSystem

### 4.3 其他 UI 文件
- [ ] VS_UI/src/VS_UI_GameCommon.cpp
- [ ] VS_UI/src/vs_ui_gamecommon2.cpp
- [ ] VS_UI/src/Vs_ui.cpp

---

## Phase 5: 清理保留的平台特定代码

### 5.1 网络初始化 (Client/GameInit.cpp)
- [ ] 简化 WSAStartup 条件调用
- [ ] 添加清晰的注释说明

### 5.2 注册表访问
- [ ] 保留但添加 `#ifdef PLATFORM_WINDOWS` 注释
- [ ] 标记为 Windows-only 功能

### 5.3 Anti-cheat 检测 (Client/GameMain.cpp)
- [ ] 保留但添加清晰注释
- [ ] 标记为 Windows-only 功能

---

## Phase 6: 验证和测试

### 6.1 编译验证
- [ ] macOS: `make debug-asan` 成功
- [ ] Windows: (如有环境) CMake 构建成功

### 6.2 功能测试
- [ ] 游戏启动正常
- [ ] UI 显示正常
- [ ] 文本渲染正常
- [ ] 音频播放正常
- [ ] 渲染效果正常

---

## 总结

| Phase | 任务数 | 预计影响 |
|-------|--------|----------|
| Phase 1 | 4 文件 | 高 - 核心渲染 |
| Phase 2 | 3 文件 | 中 - 音频系统 |
| Phase 3 | 4+ 文件 | 低 - 头文件 |
| Phase 4 | 4+ 文件 | 中 - UI 文本 |
| Phase 5 | 3 项 | 低 - 保留代码 |
| Phase 6 | 9 项 | 验证 |

**预计总修改文件：** ~20 个
**预计移除代码行：** ~500-1000 行