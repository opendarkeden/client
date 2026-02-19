# 创建最小 Demo 数据包指南

## 目标

创建一个 **50-80MB** 的最小 demo 数据包，包含单个野外场景的所有必需资源。

## 当前游戏数据大小

```bash
# 检查当前数据大小
du -sh DarkEden/Data
```

## 精简策略

### 1. 识别必需文件

#### 绝对必需的配置文件

```
Data/Info/FileDef.inf          # 资源路径定义（最重要！）
Data/Info/GameClient.inf       # 客户端配置
Data/Info/Language.inf         # 语言设置
Data/Info/ClientConfig.inf     # 客户端界面配置
```

#### 单个场景所需的资源

假设 demo 从场景 **1_1** 开始（典型的野外场景）：

1. **检查场景配置**
```bash
# 查找场景 1_1 的配置
grep -A 20 "\[Zone 1_1\]" DarkEden/Data/Info/ZoneInfo.inf
```

2. **场景需要的资源类型**
- Tile.spk - 地图贴图
- ImageObject.spk - 场景物体（树、建筑等）
- Creature.spk - 生物/NPC
- Item.spk - 物品
- Effect.spk - 特效
- UI.spk - 界面元素

### 2. 创建 Demo 数据目录

```bash
# 创建 demo 数据目录
mkdir -p DarkEdenDemo/Data/Info
mkdir -p DarkEdenDemo/UserSet
```

### 3. 复制配置文件

```bash
# 复制必需的 .inf 配置文件
cp DarkEden/Data/Info/FileDef.inf DarkEdenDemo/Data/Info/
cp DarkEden/Data/Info/GameClient.inf DarkEdenDemo/Data/Info/
cp DarkEden/Data/Info/Language.inf DarkEdenDemo/Data/Info/
cp DarkEden/Data/Info/ClientConfig.inf DarkEdenDemo/Data/Info/

# 复制场景 1_1 的配置
cp DarkEden/Data/Info/ZoneInfo.inf DarkEdenDemo/Data/Info/

# 复制基础的表格文件（生物、物品等）
cd DarkEden/Data/Info
cp Creature.inf Item.inf Effect.inf Zone.inf \
   SkillInfo.inf OptionInfo.inf ExpInfo.inf \
   ../../../DarkEdenDemo/Data/Info/
```

### 4. 提取场景 1_1 的资源

#### 方法 1: 复制完整的 .spk 文件（简单但大）

```bash
# 复制主要资源包
cp DarkEden/Data/UI.spk DarkEdenDemo/Data/
cp DarkEden/Data/Tile.spk DarkEdenDemo/Data/
cp DarkEden/Data/Creature.spk DarkEdenDemo/Data/
cp DarkEden/Data/Item.spk DarkEdenDemo/Data/
cp DarkEden/Data/Effect.spk DarkEdenDemo/Data/
cp DarkEden/Data/ImageObject.spk DarkEdenDemo/Data/
```

**预计大小**: ~150-200MB（太大）

#### 方法 2: 使用 Sprite Viewer 工具提取（推荐）

项目包含工具可以查看和提取 .spk 文件的内容：

```bash
cd build/bin

# 查看 Creature.spk 内容
./sprite_viewer ../../../DarkEden/Data/Creature.spk

# 记录场景 1_1 需要的生物 ID
# 然后创建只包含这些生物的新 .spk 文件
```

**注意**: 需要先实现 `spk_extract` 工具来提取单个精灵。

### 5. 精简 FileDef.inf

编辑 `DarkEdenDemo/Data/Info/FileDef.inf`，移除不需要的资源引用：

```ini
[Sprite]
; 只保留 demo 需要的资源
UI = Data/UI.spk
Tile = Data/Tile.spk
Creature = Data/Creature.spk
Item = Data/Item.spk
Effect = Data/Effect.spk
ImageObject = Data/ImageObject.spk

; 移除不需要的：
; Motion = Data/Motion.spk
; Weapon = Data/Weapon.spk
; ...
```

### 6. 修改 GameClient.inf

编辑 `DarkEdenDemo/Data/Info/GameClient.inf`：

```ini
[Game]
; 禁用网络
UseNetwork = 0

; Demo 配置
StartZone = 1_1
PlayerRace = Slayer

; 禁用自动更新
AutoUpdate = 0

; 本地模式
ServerMode = 0
```

### 7. 精简 UI 资源

UI.spk 通常包含所有种族的 UI。可以：

1. **保留所有 UI**（简单，~5MB）
2. **只提取 Slayer UI**（复杂，~2MB）

推荐保留所有 UI，因为文件不大。

### 8. 移除音频资源

Demo 不需要音频，删除：

```bash
# 不要复制这些目录
# DarkEden/Data/Sound/
# DarkEden/Data/Music/
```

### 9. 精简地图资源

如果只支持场景 1_1，可以：

```bash
# 只复制场景 1_1 的地图文件
cp DarkEden/Data/Map/1_1.* DarkEdenDemo/Data/Map/
```

**注意**: 需要先检查地图文件格式。

## 快速方案：使用现有数据

### 最简单的方法（~100-150MB）

```bash
#!/bin/bash
# 创建 demo 数据包

DEMO_DIR="DarkEdenDemo"
SOURCE_DIR="../DarkEden/Data"

echo "创建 Demo 数据目录..."
mkdir -p "$DEMO_DIR/Data/Info"
mkdir -p "$DEMO_DIR/Data/Map"
mkdir -p "$DEMO_DIR/UserSet"

echo "复制配置文件..."
cp $SOURCE_DIR/Info/*.inf "$DEMO_DIR/Data/Info/" 2>/dev/null

echo "复制资源包..."
cp $SOURCE_DIR/*.spk "$DEMO_DIR/Data/"

echo "复制场景 1_1 地图..."
cp $SOURCE_DIR/Map/1_1* "$DEMO_DIR/Data/Map/" 2>/dev/null || true

echo "Demo 数据包创建完成！"
echo "大小: $(du -sh $DEMO_DIR | cut -f1)"
```

保存为 `create_demo_data.sh` 并运行。

## 更激进的精简（~50MB）

### 1. 识别场景 1_1 使用的生物

查看 `Data/Info/ZoneInfo.inf` 中场景 1_1 的配置：

```bash
grep -A 50 "\[Zone 1_1\]" DarkEden/Data/Info/ZoneInfo.inf
```

找出该场景中的生物 ID 列表。

### 2. 创建自定义 Creature.spk

**需要工具**: `tools/spk_extractor.cpp`（需要开发）

```cpp
// 伪代码
SPKFile input("Creature.spk");
SPKFile output("Creature_Demo.spk");

for (auto creatureID : demoZoneCreatures) {
    output.addSprite(input.getSprite(creatureID));
}

output.save();
```

### 3. 应用相同策略到其他资源

- **Item.spk** - 只保留场景 1_1 可能掉落的物品
- **Effect.spk** - 只保留基础特效
- **ImageObject.spk** - 只保留场景 1_1 的物体

## Web 部署优化

### 使用 gzip 压缩

```bash
# 压缩 demo 数据
gzip -9 DarkEdenDemo/Data/*.spk
```

Emscripten 会自动处理压缩。

### 分离 .wasm 和 .data 文件

修改 `emscripten/CMakeLists.txt`：

```cmake
# 不使用单文件模式
# set(CMAKE_EXECUTABLE_SUFFIX_CXX ".html")  # 注释掉这行

# 使用分离文件
target_link_options(DarkEdenWeb PRIVATE
    "SHELL:-s SINGLE_FILE=0"
)
```

这样生成的文件：
- `DarkEdenWeb.html` (~10KB)
- `DarkEdenWeb.wasm` (~5MB)
- `DarkEdenWeb.data` (~50-100MB)

浏览器可以并行加载并缓存这些文件。

## 验证 Demo 数据包

### 测试步骤

1. **桌面测试**
```bash
export DARKEDEN_MODE=demo
cd build
./bin/DarkEden
```

检查：
- 游戏能否启动
- 场景 1_1 是否正确显示
- 是否没有网络错误

2. **检查文件大小**
```bash
du -sh DarkEdenDemo
```

目标: < 80MB

3. **检查完整性**
```bash
./bin/resource_validator ../DarkEdenDemo
```

## 当前可用工具

项目中已包含的工具：

```bash
cd build/bin

# Sprite Viewer - 查看 .spk 内容
./sprite_viewer <path-to-spk-file>

# Resource Validator - 验证资源完整性
./resource_validator <data-directory>

# Map Viewer - 查看地图
./map_viewer <data-directory>
```

## 推荐的工作流程

### 阶段 1: 粗糙 Demo（1小时）

```bash
# 1. 复制所有配置和主要资源
./create_demo_data.sh

# 2. 测试
export DARKEDEN_MODE=demo
./build/bin/DarkEden

# 3. 验证基本功能
```

**结果**: ~150MB，可运行的 demo

### 阶段 2: 精简优化（2-4小时）

```bash
# 1. 使用 sprite_viewer 查看各 .spk 内容
# 2. 记录场景 1_1 需要的资源 ID
# 3. 开发 spk_extractor 工具（或手动精简）
# 4. 重新打包
```

**结果**: ~50-80MB，优化的 demo

### 阶段 3: Web 部署（0.5小时）

```bash
# 1. 安装 Emscripten
# 2. 构建 Web 版本
cd emscripten && ./build.sh

# 3. 测试浏览器加载
# 4. 部署到静态服务器
```

**结果**: 可在浏览器运行的 demo

## 故障排查

### 问题 1: 游戏启动后黑屏

**原因**: 缺少必需资源
**解决**:
- 检查 `FileDef.inf` 中的路径
- 确认所有 .spk 文件存在
- 查看控制台日志

### 问题 2: 缺少精灵错误

**原因**: .spk 文件中缺少某个精灵
**解决**:
- 使用 `sprite_viewer` 检查 .spk 内容
- 添加缺少的精灵或修改配置

### 问题 3: 内存不足

**原因**: 数据包太大
**解决**:
- 减少 .spk 文件大小
- 增加 Emscripten 内存限制: `-s TOTAL_MEMORY=268435456`

## 下一步

1. **运行快速方案**创建基础 demo 数据
2. **测试桌面版本**确保基本功能
3. **精简优化**到目标大小
4. **构建 Web 版本**并部署

需要帮助？查看 `DEMO_README.md` 了解完整流程。
