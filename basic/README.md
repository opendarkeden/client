# Platform Abstraction Layer

## 概述

这是一个为 Dark Eden 客户端设计的跨平台抽象层，用于替换 Windows API 依赖，使代码可以在 Windows、Linux 和 macOS 上编译运行。

## 设计目标

1. **保持 API 兼容性** - 上层代码无需修改
2. **自底向上重构** - 从最底层开始，逐层替换
3. **可测试性** - 提供测试套件验证正确性
4. **性能优先** - 使用原生 API（Windows）或 SDL2（跨平台）

## 架构

```
┌─────────────────────────────────────┐
│     上层代码（SpriteLib 等）          │
└──────────────┬──────────────────────┘
               │
┌──────────────┴──────────────────────┐
│     basic/*.h（使用平台抽象）         │
├─────────────────────────────────────┤
│ Typedef.h, PlatformUtil.h,          │
│ Directory.h, BasicMemory.h, etc.    │
└──────────────┬──────────────────────┘
               │
┌──────────────┴──────────────────────┐
│     Platform.h（统一接口）           │
└──────┬───────────────────────┬──────┘
       │                       │
┌──────┴──────┐       ┌────────┴────────┐
│ Platform     │       │ PlatformSDL     │
│ Windows.cpp  │       │ .cpp            │
├─────────────┤       ├─────────────────┤
│ Windows API  │       │ SDL2 + POSIX    │
└─────────────┘       └─────────────────┘
```

## 目录结构

```
basic/
├── Platform.h              # 平台抽象接口
├── PlatformWindows.cpp     # Windows 实现
├── PlatformSDL.cpp         # SDL/跨平台实现
├── Typedef.h               # 基础类型定义
├── PlatformUtil.h          # 平台工具
├── Directory.h/.cpp        # 目录操作
├── BasicMemory.h           # 内存管理
├── BasicException.h        # 异常处理
├── DLL.h                   # 动态库加载
└── CMakeLists.txt          # 构建配置
```

## 已实现的功能

### ✅ 时间函数
- `platform_get_ticks()` - 毫秒计时器（替代 timeGetTime）
- `platform_get_performance_counter()` - 高性能计数器
- `platform_get_performance_frequency()` - 计数器频率
- `platform_sleep()` - 睡眠指定毫秒

### ✅ 线程/同步原语
- `platform_thread_create()` - 创建线程（替代 CreateThread）
- `platform_thread_wait()` - 等待线程
- `platform_mutex_create/lock/unlock()` - 互斥锁（替代 CreateMutex）
- `platform_event_create/wait/signal()` - 事件对象（替代 CreateEvent）

### ✅ 文件/路径操作
- `platform_get_path_separator()` - 获取路径分隔符
- `platform_file_exists()` - 检查文件存在
- `platform_get_executable_dir()` - 获取可执行文件目录
- `platform_create_directory()` - 创建目录

### ✅ 键盘函数
- `platform_is_ctrl_pressed()` - 检查 Ctrl 键状态
- `platform_get_scan_code()` - 获取扫描码

### ✅ 配置系统
- `platform_config_get_string()` - 读取配置（替代 RegQueryValueEx）
- `platform_config_set_string()` - 写入配置（替代 RegSetValueEx）

**注意：** Windows 使用注册表，Linux/macOS 使用配置文件

### ✅ 动态库加载
- `platform_lib_load()` - 加载动态库（替代 LoadLibrary）
- `platform_lib_get_symbol()` - 获取符号（替代 GetProcAddress）
- `platform_lib_free()` - 释放动态库（替代 FreeLibrary）

## 编译说明

### Windows（原生后端）

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Windows（SDL 后端）

```bash
mkdir build && cd build
cmake -DUSE_SDL_BACKEND=ON ..
cmake --build .
```

### Linux/macOS

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## 测试

运行平台抽象层测试：

```bash
# 编译
cmake --build .

# 运行测试
./tests/test_platform  # Linux/macOS
tests\Debug\test_platform.exe  # Windows
```

测试覆盖：
- ✅ 时间函数
- ✅ 互斥锁
- ✅ 线程
- ✅ 事件
- ✅ 文件操作
- ✅ 目录类
- ✅ 键盘状态
- ✅ 配置函数

## 使用示例

### 线程创建

**原代码：**
```cpp
HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, param, 0, &threadId);
WaitForSingleObject(hThread, INFINITE);
CloseHandle(hThread);
```

**新代码：**
```cpp
platform_thread_t hThread = platform_thread_create(ThreadFunc, param);
platform_thread_wait(hThread);
```

### 时间函数

**原代码：**
```cpp
DWORD start = timeGetTime();
Sleep(100);
DWORD elapsed = timeGetTime() - start;
```

**新代码：**
```cpp
DWORD start = platform_get_ticks();
platform_sleep(100);
DWORD elapsed = platform_get_ticks() - start;
```

### 配置读取

**原代码：**
```cpp
HKEY hKey;
RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\DarkEden", 0, KEY_READ, &hKey);
DWORD size = sizeof(buffer);
RegQueryValueEx(hKey, "RunFileName", NULL, NULL, (BYTE*)buffer, &size);
RegCloseKey(hKey);
```

**新代码：**
```cpp
DWORD size = sizeof(buffer);
platform_config_get_string("SOFTWARE\\DarkEden", "RunFileName", buffer, &size);
```

## 向上兼容性

所有 basic/ 目录的头文件现在使用平台抽象层：

```cpp
#include "Typedef.h"           // ✅ 包含 Platform.h
#include "PlatformUtil.h"      // ✅ 使用 platform_is_ctrl_pressed()
#include "Directory.h"         // ✅ 使用 platform_get_executable_dir()
```

**上层代码无需修改！**

## 下一步

### 阶段 2：替换 DXLib（输入/音频）

创建 `DXLib/PlatformDX.h`，使用平台抽象层：

```cpp
// CDirectInput 可使用 platform_thread_* 和 SDL2
// CDirectSound 可使用 SDL_mixer
```

### 阶段 3：替换 SpriteLib

使用 engine/sprite 中已有的 SDL2 实现：

```cpp
#include "engine/sprite/include/sprite.h"
```

### 阶段 4：替换 WinLib

创建窗口抽象：

```cpp
// CWinMain → SDL_Window + SDL_Renderer
```

## 注意事项

1. **注册表替代** - Windows 使用注册表，其他平台使用文件 `DarkEden.conf`
2. **路径分隔符** - Windows 使用 `\`，其他平台使用 `/`
3. **动态库扩展名** - Windows `.dll`，Linux `.so`，macOS `.dylib`
4. **线程返回值** - 平台抽象返回 DWORD，与 Windows 保持一致

## 贡献者

- Platform Abstraction Layer: 2025.01.14

## 许可证

与 Dark Eden 客户端项目相同
