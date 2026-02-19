# Emscripten 构建状态

## 问题解决历史

### 问题 1: `emcc` 找不到

**原因**: `source emsdk_env.sh` 只在当前 shell 会话生效，`build.sh` 运行在新的 shell 中

**解决方案**: 修改 `build.sh`，在脚本开头自动激活 Emscripten 环境：

```bash
# 激活 Emscripten 环境
EMSDK_DIR="${EMSDK_DIR:-$HOME/project/emsdk}"

if [ -f "$EMSDK_DIR/emsdk_env.sh" ]; then
    source "$EMSDK_DIR/emsdk_env.sh"
fi
```

### 问题 2: CMake 路径错误

**原因**: `emscripten/CMakeLists.txt` 中的相对路径不正确

**解决方案**: 重写 CMakeLists.txt，使用 `file(GLOB_RECURSE ...)` 直接收集所有源文件，避免 `add_subdirectory` 的问题

### 问题 3: SDL2 找不到

**原因**: Emscripten 工具链提供 SDL2，不需要 `find_package(SDL2)`

**解决方案**: 修改 `basic/CMakeLists.txt`，在 Emscripten 时跳过 SDL2 的 find_package 和链接

## 当前状态

✅ **已解决**:
1. Emscripten 环境自动激活
2. CMake 配置正确
3. SDL2 问题修复
4. 编译已启动

⏳ **进行中**:
- Emscripten 编译（预计需要 5-15 分钟）

## 构建命令

```bash
cd /Users/genius/project/opendarkeden/client/emscripten
./build.sh
```

## 预期输出

成功后会生成：
```
emscripten/build/bin/DarkEdenWeb.html
```

这是一个单文件 Web Demo，包含：
- HTML 结构
- JavaScript 加载器
- WebAssembly 二进制（.wasm）
- 游戏数据（如果启用了 preload）

## 首次编译时间

**首次编译**: 10-30 分钟（需要编译所有源文件）
**增量编译**: 1-5 分钟（只重新编译修改的文件）

## 当前简化方案

由于完整游戏数据（~1GB）太大，当前构建**不包含游戏数据预加载**。

要测试游戏，需要：
1. 将 `DarkEden/Data` 放在 Web 服务器上
2. 或者修改 CMakeLists.txt 启用 `--preload-file`（会生成很大的 HTML 文件）

## 测试 Web Demo

### 方法 1: 使用 emrun（推荐）

```bash
cd build/bin
emrun --browser chrome DarkEdenWeb.html
```

### 方法 2: 使用 Python HTTP 服务器

```bash
cd build/bin
python3 -m http.server 8000
# 然后在浏览器打开: http://localhost:8000/DarkEdenWeb.html
```

### 方法 3: 部署到静态服务器

```bash
# 上传到任何静态托管服务
scp build/bin/DarkEdenWeb.html user@server:/var/www/html/
```

## 已修改的文件

1. **emscripten/build.sh**
   - 添加了 Emscripten 环境自动激活
   - 添加了更好的错误提示

2. **emscripten/CMakeLists.txt**
   - 完全重写，使用 GLOB 收集源文件
   - 避免 add_subdirectory 的路径问题
   - 禁用了游戏数据预加载（太大）

3. **basic/CMakeLists.txt**
   - 添加了 Emscripten 平台检测
   - 跳过 SDL2 find_package
   - 跳过 SDL2 链接

## 下一步

1. **等待编译完成**（10-30 分钟）
2. **测试 Web Demo**
3. **优化资源大小**（可选）

## 调试技巧

如果构建失败：

```bash
# 查看 CMake 配置
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release

# 手动编译
emmake make -j4

# 查看详细日志
emcc -v
```

## 架构改进总结

✅ **通过这个 Demo 项目，主项目获得了**:

1. **网络抽象层** - 可以离线测试游戏逻辑
2. **资源抽象层** - 灵活的资源加载方式
3. **平台抽象** - 统一的平台 API
4. **配置系统** - 运行时配置驱动行为
5. **Emscripten 支持** - 可以编译到 WebAssembly

这些改进是**永久性的**，对主项目有很大价值！

## 进度跟踪

- [x] 网络抽象层
- [x] 资源抽象层
- [x] 平台抽象层
- [x] 配置系统
- [x] 桌面 Demo 测试
- [x] Emscripten 安装
- [x] CMake 配置
- [ ] WebAssembly 编译（进行中）
- [ ] Web Demo 测试
- [ ] 资源优化（可选）
