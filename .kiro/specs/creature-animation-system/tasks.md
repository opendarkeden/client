# Implementation Plan: Creature Animation System

## Overview

本实现计划将设计文档转化为可执行的编码任务。采用自底向上的方式，先实现基础数据结构，再实现文件加载，最后创建 Creature 查看器工具。

重点是保持与原始 SpriteLib/FrameLib 的接口兼容，使上层代码可以最小改动地工作。

## Tasks

- [x] 1. 基础数据结构
  - [x] 1.1 实现 TArray 模板 (lib/tarray.h)
    - 实现 Init、Release、GetSize
    - 实现 operator[]、const operator[]
    - 实现 SaveToFile、LoadFromFile
    - 与原始 TArray 接口兼容
    - _Requirements: 4.6_
  - [x] 1.2 实现 CFrame 类 (lib/frame.h, lib/frame.cpp)
    - 实现 Set、GetSpriteID、GetCX、GetCY
    - 实现 SaveToFile、LoadFromFile
    - 定义 FRAME_ARRAY、DIRECTION_FRAME_ARRAY、ACTION_FRAME_ARRAY 类型
    - _Requirements: 3.4, 3.5_
  - [x] 1.3 编写 TArray 和 CFrame 单元测试
    - **Property 1: TArray 数据一致性**
    - **Property 2: CFrame 数据存储**
    - **Validates: Requirements 3.4, 3.5, 4.6**

- [x] 2. Checkpoint - 基础数据结构验证
  - 确保 TArray 和 CFrame 测试通过
  - 如有问题请询问用户

- [x] 3. CreatureFramePack 实现
  - [x] 3.1 实现 CFramePack 模板 (lib/framepack.h)
    - 继承 TArray<Type, TYPE_FRAMEID>
    - 实现 SaveToFile(packFile, indexFile)
    - 实现 LoadFromFile
    - _Requirements: 3.1, 3.2_
  - [x] 3.2 实现 CCreatureFramePack 类
    - 定义为 CFramePack<ACTION_FRAME_ARRAY>
    - 实现 InfoToFile 调试方法
    - _Requirements: 3.3_
  - [x] 3.3 编写 CFPK 加载测试
    - 加载真实 Creature.cfpk 文件
    - 验证 CreatureType 数量
    - 验证 Action/Direction/Frame 结构
    - **Property 3: CFPK 加载正确性**
    - **Validates: Requirements 3.1, 3.2, 3.3**

- [x] 4. Checkpoint - FramePack 验证
  - 确保能正确加载 Creature.cfpk
  - 打印部分帧数据验证
  - 如有问题请询问用户

- [x] 5. IndexedSprite 实现
  - [x] 5.1 实现 ColorSet 系统 (lib/colorset.h, lib/colorset.cpp)
    - 实现 ColorSet[495][30] 颜色表
    - 实现 SetColorSet 初始化
    - 实现 SetUsingColorSet 设置当前颜色集
    - 实现 LoadIndexTableFromFile 加载颜色表
    - _Requirements: 6.1, 6.2, 6.3, 6.4_
  - [x] 5.2 实现 CIndexSprite 类 (lib/index_sprite.h, lib/index_sprite.cpp)
    - 实现 LoadFromFile 解析索引像素数据
    - 实现 Blt 渲染到 SDL
    - 实现 BltColorSet 带颜色集渲染
    - 实现 BltEffect、BltAlpha
    - _Requirements: 1.4, 1.5, 1.6_
  - [x] 5.3 实现 CIndexSpritePack (lib/index_spritepack.h, lib/index_spritepack.cpp)
    - 实现 LoadFromFile 加载 .ispk
    - 实现 LoadFromFileRunning 延迟加载
    - 实现 operator[] 访问
    - _Requirements: 1.1, 1.2, 1.3_
  - [x] 5.4 编写 IndexedSprite 测试
    - 加载真实 Creature.ispk 文件
    - 验证 Sprite 数量和尺寸
    - **Property 4: ISPK 延迟加载**
    - **Property 5: ColorSet 映射一致性**
    - **Validates: Requirements 1.1, 1.2, 1.3, 6.4, 6.5**

- [x] 6. Checkpoint - IndexedSprite 验证
  - 确保能正确加载 Creature.ispk
  - 验证 ColorSet 渲染效果
  - 如有问题请询问用户

- [-] 7. ShadowSprite 实现
  - [x] 7.1 实现 CShadowSprite 类 (lib/shadow_sprite.h, lib/shadow_sprite.cpp)
    - 实现 LoadFromFile 解析阴影数据
    - 实现 Blt 渲染阴影
    - 实现 BltAlpha 带透明度渲染
    - _Requirements: 2.3, 2.4_
  - [x] 7.2 实现 CShadowSpritePack (lib/shadow_spritepack.h, lib/shadow_spritepack.cpp)
    - 实现 LoadFromFile 加载 .sspk
    - 实现 operator[] 访问
    - _Requirements: 2.1, 2.2_
  - [x] 7.3 编写 ShadowSprite 测试
    - 加载真实 Creature.sspk 文件
    - 验证阴影渲染效果
    - **Validates: Requirements 2.1, 2.2, 2.3, 2.4**

- [x] 8. Checkpoint - ShadowSprite 验证
  - 确保能正确加载 Creature.sspk
  - 验证阴影渲染效果
  - 如有问题请询问用户

- [ ] 9. Creature 查看器工具
  - [x] 9.1 创建 tools/creature_viewer/main.cpp
    - 实现命令行参数解析 (ispk, cfpk, sspk 路径)
    - 实现资源加载
    - 实现 Creature 类型切换 (LEFT/RIGHT)
    - 实现 Action 切换 (UP/DOWN)
    - 实现 Direction 切换 (1-8 键)
    - 实现动画播放控制 (SPACE)
    - 实现阴影显示切换 (S 键)
    - 实现 ColorSet 切换 (C 键)
    - 实现信息显示
    - _Requirements: 5.1-5.10_
  - [ ] 9.2 更新 CMakeLists.txt 添加 creature_viewer 目标
    - _Requirements: 4.7_

- [ ] 10. 最终 Checkpoint - 完整功能验证
  - 确保所有测试通过
  - 验证 creature_viewer 能正确加载和播放 Creature 动画
  - 验证 ColorSet 换色效果
  - 验证阴影渲染效果
  - 如有问题请询问用户

## Notes

- 每个任务都引用了具体的需求条目以便追溯
- Checkpoint 任务用于阶段性验证
- 重点是保持与原始代码的接口兼容
- 使用真实游戏资源文件进行测试
- Creature.ispk、Creature.cfpk、Creature.sspk 位于 DarkEden/Data/Image/ 目录

## 测试资源路径

```
DarkEden/Data/Image/Creature.ispk      # Creature 图像
DarkEden/Data/Image/Creature.ispki     # Creature 图像索引
DarkEden/Data/Image/Creature.cfpk      # Creature 帧数据
DarkEden/Data/Image/Creature.cfpki     # Creature 帧数据索引
DarkEden/Data/Image/Creature.sspk      # Creature 阴影
DarkEden/Data/Image/Creature.sspki     # Creature 阴影索引
DarkEden/Data/Image/CreatureShadow.cfpk # Creature 阴影帧数据
```

## Action 定义参考

```cpp
// 来自原始代码的 Action 定义
enum ACTION {
    ACTION_STAND = 0,
    ACTION_WALK,
    ACTION_RUN,
    ACTION_ATTACK,
    ACTION_DAMAGED,
    ACTION_DIE,
    // ... 更多 Action
};
```
