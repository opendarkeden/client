# Implementation Plan: Animation System

## Overview

本实现计划将设计文档转化为可执行的编码任务。采用自底向上的方式，先重构项目结构，再实现 Animation 核心模块，最后创建 Animation 查看器工具。

## Tasks

- [x] 1. 项目结构重组
  - [x] 1.1 重构目录结构
    - 创建 lib/ 目录，移动共享库代码
    - 创建 tools/ 目录，移动工具代码
    - 将 src/*.c/h 移动到 lib/
    - 将 viewer 相关代码移动到 tools/sprite_viewer/
    - _Requirements: 1.1, 1.2_
  - [x] 1.2 更新 CMakeLists.txt
    - 配置 lib/ 编译为静态库 libdarkeden
    - 配置 tools/ 链接静态库
    - 保持测试配置
    - _Requirements: 1.3, 1.4, 1.5_
  - [x] 1.3 创建 types.h 共享类型定义
    - 定义 SpriteID、FrameID 类型
    - 定义 Direction 枚举
    - 定义 BltType 枚举
    - _Requirements: 2.6, 3.3, 6.1_

- [x] 2. Checkpoint - 项目结构验证
  - 确保项目能正常编译
  - 确保 sprite_viewer 工具正常运行
  - 如有问题请询问用户

- [x] 3. Animation 帧管理模块
  - [x] 3.1 实现 animation.h 中的 AnimFrame 结构和函数
    - 实现 anim_frame_init 初始化函数
    - 实现 anim_frame_set 设置帧序列
    - 实现 anim_frame_next 帧递增
    - 实现 anim_frame_next_loop 循环模式帧递增
    - 实现 anim_frame_get 获取当前帧
    - _Requirements: 2.1, 2.2, 2.3, 2.4, 2.5, 2.6_
  - [x] 3.2 编写 AnimFrame 属性测试
    - **Property 1: AnimFrame 数据存储一致性**
    - **Property 2: 帧循环正确性**
    - **Property 3: 循环模式帧计算**
    - **Validates: Requirements 2.1, 2.2, 2.3, 2.4, 2.5, 2.6**

- [x] 4. Animation 对象模块
  - [x] 4.1 实现 animation.h 中的 AnimObject 结构和函数
    - 实现 anim_object_init 初始化函数
    - 实现 anim_object_set 设置属性
    - 实现 anim_object_set_frame 设置帧序列
    - 实现 anim_object_next_frame 帧递增
    - 实现 anim_object_get_sprite 获取当前 Sprite ID
    - 实现 anim_object_set_direction 设置方向
    - _Requirements: 3.1, 3.2, 3.3, 3.5, 3.6, 6.1, 6.2_
  - [x] 4.2 编写 AnimObject 属性测试
    - **Property 4: AnimObject 数据存储一致性**
    - **Property 5: 方向范围有效性**
    - **Property 6: Sprite ID 计算正确性**
    - **Validates: Requirements 3.1, 3.2, 3.3, 3.5, 3.6, 6.1, 6.2**

- [x] 5. Checkpoint - Animation 核心模块验证
  - 确保所有 AnimFrame 和 AnimObject 测试通过
  - 如有问题请询问用户

- [x] 6. Animation 渲染模块
  - [x] 6.1 实现 animation.h 中的渲染函数
    - 实现 anim_set_blend_mode 设置混合模式
    - 实现 anim_render 渲染动画
    - 实现 anim_render_blt 指定 BltType 渲染
    - 支持 BLT_NORMAL、BLT_EFFECT、BLT_SHADOW、BLT_SCREEN
    - _Requirements: 4.1, 4.2, 4.3, 4.4, 4.5, 4.6_
  - [x] 6.2 编写渲染模块测试
    - **Property 7: BltType 混合模式映射**
    - **Validates: Requirements 4.2, 4.3, 4.4, 4.5**

- [x] 7. Animation 查看器工具
  - [x] 7.1 创建 tools/animation_viewer/main.cpp
    - 实现命令行参数解析
    - 实现 SpritePack 加载
    - 实现动画播放控制（播放/暂停）
    - 实现帧导航（左右键）
    - 实现 BltType 切换（1-4 键）
    - 实现方向切换（上下键）
    - 实现信息显示（帧索引、BltType、方向）
    - _Requirements: 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8, 6.3, 6.4_
  - [x] 7.2 更新 CMakeLists.txt 添加 animation_viewer 目标
    - _Requirements: 1.4, 1.5_

- [x] 8. 最终 Checkpoint - 完整功能验证
  - 确保所有测试通过
  - 验证 animation_viewer 能正确加载和播放动画
  - 验证各种 BltType 渲染效果
  - 如有问题请询问用户

## Notes

- 每个任务都引用了具体的需求条目以便追溯
- Checkpoint 任务用于阶段性验证
- 属性测试验证核心正确性属性
- 项目重构需要小心处理，确保不破坏现有功能
- Animation 查看器可以复用 sprite_viewer 的部分代码
