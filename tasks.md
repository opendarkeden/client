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
- [x] PLATFORM_WINDOWS: 8 → 0 实例

### 1.3 Client/GameInit.cpp ✅
- [x] 头文件统一
- [x] Surface 初始化统一 (g_pBack, g_pLast)
- [x] Sound part count 统一
- [x] WSAStartup/WSACleanup (Windows only - 保留)
- [x] DirectDraw/3D Release (stubs - 已移除条件编译)
- [x] 编译验证通过
- [ ] InitDraw() 函数中的 DirectDraw 内存查询 (保留 stub 调用)

### 1.4 Client/GameMain.cpp ✅
- [x] 统一头文件包含 (Windows packets in PLATFORM_WINDOWS)
- [x] 移除空 if(true) 代码块 (MODE_CHANGE_OPTION)
- [x] 统一显示模式切换 (SetDisplayMode/RestoreDisplayMode)
- [x] 添加 CDirectDraw.h stub 方法 (SetDisplayMode, RestoreDisplayMode)
- [x] DirectSound buffer 初始化封装在 PLATFORM_WINDOWS
- [x] 修复嵌套预处理器指令不匹配 (#endif // __USE_MP3__)
- [x] 统一音乐播放代码 (OGG streamPlay 参数)
- [x] DirectSound 特定代码 (LPDIRECTSOUNDBUFFER) 封装在 guards
- [x] 编译验证通过
- [ ] PLATFORM_WINDOWS: 56 → 41 实例 (剩余为合理的平台特定代码)

### 1.5 Client/Client.cpp ✅
- [x] 移除 DirectDraw 显存查询 (使用默认 256MB)
- [x] 统一游戏更新循环逻辑
- [x] 保留必要的平台特定代码
- [x] 编译验证通过
- [ ] PLATFORM_WINDOWS: 8 → 4 实例

### 1.6 DX3D.h includes cleanup ✅
- [x] 移除 DX3D.h includes from:
  - DrawCreatureEffect.cpp
  - DrawCreatureShadow.cpp
  - DrawCreatureDivineGuidance.cpp
  - Client.h
  - MTopView.h
- [x] All DX3D.h references now only in D3DLib/DX3D.h (stub) or comments
- [x] 编译验证通过
- [ ] PLATFORM_WINDOWS: ~90+ instances removed across 10+ files

### Legitimate PLATFORM_WINDOWS Usage (Kept)
The following PLATFORM_WINDOWS instances are kept as they are necessary:
- **CDirectDrawSurface.h** (4 instances): Surface pointer access methods differ
- **MTopViewDraw.inl** (1 instance): DRAW_ALPHA_BOX_2D macro
- **CSpritePal.h** (1 instance): Windows.h vs Platform.h header selection

---

## Phase 2: 清理音频相关条件编译 ✅ COMPLETE

### 2.1 Client/GameMain.cpp (音频部分) ✅
- [x] DirectSound 清理代码已在 PLATFORM_WINDOWS guards 中
- [x] DirectSound buffer 初始化已在 PLATFORM_WINDOWS guards 中
- [x] 使用 SDL_mixer 路径 (通过 COGGSTREAM/CSDLStream)
- [x] MP3 播放已禁用 (__USE_MP3__ commented out)
- [x] 编译验证通过

### 2.2 Client/soundbuf.cpp ✅
- [x] DirectSound 相关代码已在 PLATFORM_WINDOWS guards 中
- [x] 非 Windows 平台提供 stub 实现
- [x] 编译验证通过
- [ ] PLATFORM_WINDOWS: 2 实例 (DirectSound API 必需)

### 2.3 Client/MMusic.cpp ✅
- [x] DirectMusic 相关代码已在 PLATFORM_WINDOWS guards 中
- [x] MCI-based MIDI 播放为 Windows-only 技术
- [x] 非 Windows 平台提供 stub 实现
- [x] 编译验证通过
- [ ] PLATFORM_WINDOWS: 2 实例 (MCI API 必需)

### Phase 2 Notes:
- Audio system uses CDirectSound/CDirectMusic wrapper classes (stubs for SDL)
- Actual audio playback uses COGGSTREAM with SDL backend (CSDLStream stubs)
- MP3 playback disabled via SoundSetting.h (#define __USE_MP3__ commented out)
- MIDI playback uses MCI (Windows-specific), properly stubbed on non-Windows
- Remaining PLATFORM_WINDOWS instances are legitimate (DirectSound/MCI APIs)

---

## Phase 3: 统一头文件包含 ✅ COMPLETE

### 3.1 Client/GameMain.cpp ✅
- [x] MMSystem.h 在 PLATFORM_WINDOWS guards 中 (Windows MCI API 必需)
- [x] 统一使用 Platform.h 时间函数 (platform_get_ticks)

### 3.2 Client/Client.cpp ✅
- [x] Windows.h 在 PLATFORM_WINDOWS guards 中
- [x] 统一使用 Platform.h 跨平台定义

### 3.3 Client/GameInit.cpp ✅
- [x] MMSystem.h 在 PLATFORM_WINDOWS guards 中 (Windows MCI API 必需)
- [x] 统一使用 Platform.h 时间函数

### 3.4 VS_UI 文件 ✅
- [x] SXml.h 和 RarFile.h 已在 PLATFORM_WINDOWS guards 中
- [x] Imm/ 和 hangul/ 目录在 CMake 中已排除
- [x] WebBrowser (cwebpage_) 在 CMake 中已排除

### 3.5 SpriteLib headers ✅
- [x] 所有 SpriteLib 头文件都在 PLATFORM_WINDOWS guards 中
- [x] DebugLog.cpp 和 Packet 文件都在 PLATFORM_WINDOWS guards 中

---

## Phase 4: 统一文本渲染路径 (SKIPPED - Requires major refactoring)

### 4.1 VS_UI/src/VS_UI_Base.cpp
- [~] 移除 GDI 字体创建代码 (Requires Phase 4 refactoring)
- [~] 统一使用 TextSystem (Requires Phase 4 refactoring)

### 4.2 VS_UI/src/VS_UI_Title.cpp
- [~] 清理 GDI 相关代码 (Requires Phase 4 refactoring)
- [~] 使用 TextSystem (Requires Phase 4 refactoring)

### 4.3 其他 UI 文件
- [~] VS_UI/src/VS_UI_GameCommon.cpp (Requires Phase 4 refactoring)
- [~] VS_UI/src/vs_ui_gamecommon2.cpp (Requires Phase 4 refactoring)
- [~] VS_UI/src/Vs_ui.cpp (Requires Phase 4 refactoring)

### Phase 4 Notes:
- Text rendering migration is a major refactoring project
- Current code uses Windows GDI for UI text rendering
- TextSystem (SDL + freetype2) is the modern replacement
- Requires significant changes to UI framework
- Should be done as a separate project after this cleanup
- All GDI code is in PLATFORM_WINDOWS guards (legitimate)

---

## Phase 5: 文档化保留的平台特定代码 (SKIPPED - Documentation only)

### 5.1 网络初始化 (Client/GameInit.cpp)
- [~] WSAStartup/WSACleanup in PLATFORM_WINDOWS guards (legitimate Windows sockets API)
- [~] 添加清晰的注释说明 (Documentation task)

### 5.2 注册表访问
- [~] Registry access in PLATFORM_WINDOWS guards (legitimate Windows-only feature)
- [~] 标记为 Windows-only 功能 (Documentation task)

### 5.3 Anti-cheat 检测 (Client/GameMain.cpp)
- [~] CGVerifyTime, CGPortCheck in PLATFORM_WINDOWS guards (legitimate anti-cheat)
- [~] 标记为 Windows-only 功能 (Documentation task)

### Phase 5 Notes:
- All existing platform-specific code is already in PLATFORM_WINDOWS guards
- Documentation improvements are a separate task
- No PLATFORM_WINDOWS cleanup needed in Phase 5

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