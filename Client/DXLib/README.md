# DXLib 平台抽象层

## 概述

DXLib 平台抽象层为 Dark Eden 客户端的输入和音频系统提供跨平台支持，支持 Windows DirectInput/DirectSound 和 SDL2 两种后端。

## 架构

```
上层代码（CDirectInput, CDirectSound, CDirectMusic）
    ↓ (保持 API 不变)
DXLibBackend.h（统一接口）
    ↓
├── DXLibBackendWindows.cpp（Windows 原生实现）
└── DXLibBackendSDL.cpp（SDL2 跨平台实现）
    ↓
操作系统
```

## 组件

### 1. DXLibBackend.h - 统一接口

定义了跨平台的抽象接口：

- **Input Backend** - 键盘和鼠标输入
- **Sound Backend** - 短音效播放（WAV）
- **Music Backend** - 背景音乐播放（MIDI, MP3, OGG）
- **Stream Backend** - 流式音频（长音频文件）

### 2. DXLibBackendSDL.cpp - SDL2 实现

**已完成的功能：**

| 功能 | 状态 | 说明 |
|------|------|------|
| 输入初始化 | ✅ | `dxlib_input_init()` |
| 键盘状态 | ✅ | `dxlib_input_key_down()` |
| 鼠标位置 | ✅ | `dxlib_input_get_mouse_pos()` |
| 鼠标按钮 | ✅ | `dxlib_input_get_mouse_buttons()` |
| 鼠标滚轮 | ✅ | `dxlib_input_get_mouse_wheel()` |
| 输入更新 | ✅ | `dxlib_input_update()` |
| 音效加载 | ✅ | `dxlib_sound_load_wav()` (需要 SDL_mixer) |
| 音效播放 | ✅ | `dxlib_sound_play()` |
| 音效停止 | ✅ | `dxlib_sound_stop()` |
| 音量控制 | ✅ | `dxlib_sound_set_volume()` |
| 音乐加载 | ✅ | `dxlib_music_load()` (需要 SDL_mixer) |
| 音乐播放 | ✅ | `dxlib_music_play()` |
| 音乐暂停/恢复 | ✅ | `dxlib_music_pause()/resume()` |
| 音乐音量 | ✅ | `dxlib_music_set_volume()` |
| 后端信息 | ✅ | `dxlib_get_backend_name()` |

**未实现的功能：**

| 功能 | 原因 | 备注 |
|------|------|------|
| 音效声相 | SDL_mixer 限制 | 需要额外实现 |
| 音效频率调整 | SDL_mixer 限制 | 需要重采样 |
| 音乐速度调整 | SDL_mixer 限制 | 无法动态调整 |

## 编译说明

### Windows (原生后端)

```bash
cmake -B build -DUSE_SDL_BACKEND=OFF
cmake --build build
```

### Windows (SDL 后端)

```bash
cmake -B build -DUSE_SDL_BACKEND=ON
cmake --build build
```

### Linux/macOS

```bash
cmake -B build
cmake --build build
```

## 使用示例

### 输入系统

```cpp
#include "DXLib/DXLibBackend.h"

// 初始化
dxlib_input_init(window_handle);

// 游戏循环
while (running) {
    // 更新输入状态
    dxlib_input_update();

    // 检查按键
    if (dxlib_input_key_down(DIK_W)) {
        // 向前移动
    }

    // 获取鼠标位置
    int x, y;
    dxlib_input_get_mouse_pos(&x, &y);

    // 检查鼠标按钮
    int left, right, center;
    dxlib_input_get_mouse_buttons(&left, &right, &center);
    if (left) {
        // 左键按下
    }
}

// 清理
dxlib_input_release();
```

### 音效系统

```cpp
#include "DXLib/DXLibBackend.h"

// 初始化
dxlib_sound_init(window_handle);

// 加载音效
dxlib_sound_t sound = dxlib_sound_load_wav("explosion.wav");

// 播放
dxlib_sound_play(sound, 0); // 0 = 不循环

// 设置音量
dxlib_sound_set_volume(sound, 50);

// 停止
dxlib_sound_stop(sound);

// 释放
dxlib_sound_free(sound);

// 清理
dxlib_sound_release();
```

### 音乐系统

```cpp
#include "DXLib/DXLibBackend.h"

// 初始化
dxlib_music_init(window_handle);

// 加载音乐
dxlib_music_load("background.mp3");

// 播放（循环）
dxlib_music_play(1);

// 设置音量
dxlib_music_set_volume(70);

// 暂停/恢复
dxlib_music_pause();
dxlib_music_resume();

// 停止
dxlib_music_stop();

// 清理
dxlib_music_release();
```

## API 映射表

### 输入系统

| 原始 CDirectInput | DXLibBackend API | SDL 实现 |
|-------------------|------------------|----------|
| `KeyDown(DIK_W)` | `dxlib_input_key_down(DIK_W)` | SDL_GetKeyboardState |
| `m_mouse_x, m_mouse_y` | `dxlib_input_get_mouse_pos()` | SDL_GetMouseState |
| `m_lb_down` | `dxlib_input_get_mouse_buttons()` | SDL_GetMouseState |
| `m_mouse_z` | `dxlib_input_get_mouse_wheel()` | SDL_MouseWheelEvent |
| `UpdateInput()` | `dxlib_input_update()` | SDL_PollEvent |

### 音效系统

| 原始 CDirectSound | DXLibBackend API | SDL 实现 |
|-------------------|------------------|----------|
| `LoadWav(filename)` | `dxlib_sound_load_wav()` | Mix_LoadWAV |
| `Play(buffer, loop)` | `dxlib_sound_play(sound, loop)` | Mix_PlayChannel |
| `Stop(buffer)` | `dxlib_sound_stop(sound)` | Mix_HaltChannel |
| `AddVolume(buffer, n)` | `dxlib_sound_set_volume()` | Mix_Volume |
| `RightPan/LeftPan` | `dxlib_sound_set_pan()` | ⚠️ 未实现 |

### 音乐系统

| 原始 CDirectMusic | DXLibBackend API | SDL 实现 |
|-------------------|------------------|----------|
| `Play(filename)` | `dxlib_music_load() + play()` | Mix_LoadMUS + Mix_PlayMusic |
| `Stop()` | `dxlib_music_stop()` | Mix_HaltMusic |
| `Pause()` | `dxlib_music_pause()` | Mix_PauseMusic |
| `Resume()` | `dxlib_music_resume()` | Mix_ResumeMusic |
| `SetCurrentTempo()` | `dxlib_music_set_tempo()` | ⚠️ 未实现 |

## 依赖

### SDL 后端

- **SDL2** - 核心功能（必需）
- **SDL2_mixer** - 音频功能（必需，用于音效和音乐）

### Windows 后端

- **DirectX SDK** - DirectInput, DirectSound, DirectMusic
- **Windows SDK** - Windows API

## 当前状态

### ✅ 已完成

- [x] DXLibBackend.h 接口定义
- [x] DXLibBackendSDL.cpp SDL2 实现
- [x] 输入系统（键盘 + 鼠标）
- [x] 音效系统（基于 SDL_mixer）
- [x] 音乐系统（基于 SDL_mixer）
- [x] 流式音频（复用音乐系统）
- [x] DirectInput 键码映射

### ⏳ 待完成

- [ ] DXLibBackendWindows.cpp Windows 实现
- [ ] 单元测试

### ✅ 已完成（适配层）

- [x] CDirectInput_Adapter.cpp - CDirectInput 适配层
- [x] CDirectSound_Adapter.cpp - CDirectSound 适配层
- [x] CDirectMusic_Adapter.cpp - CDirectMusic 适配层
- [x] CDirectSoundStream_Adapter.cpp - CDirectSoundStream 适配层
- [x] CMakeLists.txt - 编译配置（支持后端选择）

### 🔧 已知限制

1. **SDL_mixer 功能限制**
   - 不支持声相（pan）控制
   - 不支持频率（播放速度）调整
   - 不支持音乐速度调整

2. **DirectInput 键码映射**
   - 部分特殊键可能未映射
   - 日语键盘支持不完整

## 适配层实现说明

本项目采用**选项 1（适配层）**策略，已创建以下适配器文件：

### 适配器文件

- **CDirectInput_Adapter.cpp** - CDirectInput 的 SDL2 后端适配
- **CDirectSound_Adapter.cpp** - CDirectSound 的 SDL2 后端适配
- **CDirectMusic_Adapter.cpp** - CDirectMusic 的 SDL2 后端适配
- **CDirectSoundStream_Adapter.cpp** - CDirectSoundStream 的 SDL2 后端适配

### 适配层工作原理

适配器使用条件编译 (`#ifdef DXLIB_BACKEND_SDL`) 在同一类中支持两种后端：

```cpp
// CDirectInput.cpp / CDirectInput_Adapter.cpp
#ifdef DXLIB_BACKEND_SDL
    // 使用 DXLibBackend API (SDL2)
    dxlib_input_init(hWnd);
    dxlib_input_update();
#else
    // 使用原始 DirectInput API (Windows)
    // 原有代码保持不变
#endif
```

**优点：**
- 最小化上层代码修改
- 保留现有 API
- 可以在编译时选择后端
- 易于测试和验证

**缺点：**
- 增加维护复杂度（需要维护两套实现）

## 下一步工作

### 选项 2：创建新类（可选）

如果不想修改原有代码，可以创建新的 `CDirectInputSDL`, `CDirectSoundSDL` 等类：

```cpp
class CDirectInputSDL {
    // 内部完全使用 dxlib_input_* 函数
    // API 与 CDirectInput 兼容
};
```

**优点：**
- 不修改原有代码
- 可以并行开发

**缺点：**
- 代码重复
- 需要在上层代码中切换类

### 选项 3：完全替换（长期）

直接在上层使用 `DXLibBackend` API：

```cpp
// 替换
// #include "CDirectInput.h"
// 为
#include "DXLibBackend.h"

// 使用新的 API
dxlib_input_init(window);
if (dxlib_input_key_down(DIK_W)) { ... }
```

**优点：**
- 代码最简洁
- 性能最好（无抽象层开销）

**缺点：**
- 需要大量修改上层代码
- 破坏向后兼容性

## 建议路径

**短期：** 使用**选项 1（适配层）** ✅ 已实现
- 最小化修改
- 保持兼容性
- 易于测试和验证

**长期：** 迁移到**选项 3（直接使用）**
- 随着重构的深入
- 逐步替换上层调用

## 测试

创建测试程序验证后端功能：

```bash
cd build
./tests/test_dxlib_backend
```

测试覆盖：
- 输入系统功能
- 音效播放
- 音乐播放
- 多线程安全性

## 相关文件

- `DXLibBackend.h` - 平台抽象接口
- `DXLibBackendSDL.cpp` - SDL2 实现
- `DXLibBackendWindows.cpp` - Windows 实现（待创建）
- `CDirectInput_Adapter.cpp` - CDirectInput 适配层
- `CDirectSound_Adapter.cpp` - CDirectSound 适配层
- `CDirectMusic_Adapter.cpp` - CDirectMusic 适配层
- `CDirectSoundStream_Adapter.cpp` - CDirectSoundStream 适配层
- `CMakeLists.txt` - 编译配置

## 许可证

与 Dark Eden 客户端项目相同
