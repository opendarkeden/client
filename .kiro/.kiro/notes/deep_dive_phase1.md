# Deep Dive — Phase 1（自底向上分析：资源 -> 解码 -> 渲染）

日期：2025-12-26  
作者：重构小组（由我负责初步分析与任务拆分）

目的
- 按你要求的“自底向上”策略，先把与 tiled map 相关的资源格式（palette / sprite / zone / pack）和解码流程搞清楚，再推进 tileset/sector 的渲染实现。
- 记录我已确认的文件、初步结论、必须在实现前解决的细节，以及下一步精确的阅读/提取目标（文件 + 函数/行为清单），以便后续逐项移植实现不会遗漏细节。

注：本阶段我们着重“读懂”与“提取事实”。避免臆造实现细节或函数签名 —— 任何需要精确行号或函数体的地方我都会在下一个提交中以文件摘录的形式给出（file + function + 行号范围 + 摘要）。

一、已经确认的关键文件（初步目录与说明）
- `client/Client/SpriteLib/MPalette.h`  
  - 说明：声明 `MPalette` 抽象、`MPalette555`、`MPalette565`。表明旧客户端使用调色板（palette）并支持不同 16-bit 格式。
  - 影响：必须实现 palette -> 目标像素格式（SDL 使用的 RGBA8888 或平台合适格式）的转换器。

- `client/Client/SpriteLib/CSpritePal.h` / `CSpritePal.cpp`（需进一步读取实现）  
  - 说明：包含 `CSpritePal::Blt` 等多种 blt/效果实现（包括 1555/565 输出路径）。这是将索引色 sprite 渲染到目标表面（16bit/32bit）上的低层实现。
  - 影响：需要复用或重写其核心像素映射逻辑（例如 `memcpy1555` 样式的函数），但在 resurrected 中建议在加载时把 indexed 图转成 RGBA 贴图以简化运行时 Blt。

- `client/Client/PackFileManager.cpp` / `.h`  
  - 说明：Pack / SPK 资源的管理与 I/O。是获取 zone/tile/sprite 原始字节流的入口。
  - 影响：我们要么复用这部分逻辑，要么移植最小的读取器来提取 tile/palette/raw sprite bytes。

- `client/Client/ZoneFileHeader.cpp` / `.h`  
  - 说明：zone 文件头及格式定义/解析（核心）。必须解析该文件以知道 tile 数据在 zone 中的布局与偏移、层次信息及是否压缩。
  - 影响：实现 zone -> tile 数据抽取器之前必须把这个格式读透。

- `client/Client/MSector.h` / `MSector.cpp`  
  - 说明：sector 数据结构及元信息（sector 的定义、索引）。
  - 影响：决定 sector 合成纹理的大小、sector 坐标到 tile 坐标的映射。

- `client/Client/GAME1024.cpp`  
  - 说明：初始化与常量（如 `TILESURFACE_SECTOR_WIDTH` / `TILESURFACE_WIDTH` / `SECTOR_WIDTH` 等），两个模式（1024/800）下的不同参数。
  - 影响：这些变量用于确定 sector 尺寸、tilesurface 的像素尺寸、以及计算需要加载的 sector 数量。

- `client/Client/MTopViewDraw.cpp` / `MTopView.cpp`  
  - 说明：高层绘制顺序与绘制调用处，负责 orchestrate 地面层、对象层、特效层的绘制顺序。
  - 影响：要对照原绘制顺序来保证渲染兼容（遮挡／覆盖／优先级一致）。

- `client/Client/DXLib/CDirectDrawSurface.*`（或 `client/Client/CDirectDrawSurface.*`）  
  - 说明：原有的 surface 抽象（创建/锁定/Blit）的使用方式。理解这些调用能帮助我们设计 adapter（使旧逻辑能尽量少改地调用新渲染路径）。
  - 影响：决定 adapter 的 API 形态（例如 `CreateSurface`、`LockRect`、`Blt` 的等价实现）。

二、已确定的事实与初步结论（要点）
1. 旧客户端使用 palette（索引色）与 16-bit 输出路径较多（`MPalette`、`CSpritePal` 表明存在 555/565 支持）。  
   - 结论：tile / sprite 的原始数据可能是索引色（8-bit indices），运行时通过 palette 转换为目标像素格式。到 `resurrected` 应把 palette 应用的工作放到加载阶段（解码成 RGBA）而非每帧运行时做 palette lookup。

2. Pack / SPK 管理是资源读取的入口（`PackFileManager`）。  
   - 结论：优先移植或复用 pack 读取逻辑，做到能把 zone/tiles/tileset/palette 数据导出为中性格式，然后在 resurrected 中构建 SDL_Texture。

3. zone 文件头与 zone layout 非常关键（`ZoneFileHeader`）。  
   - 结论：必须解析 zone file header 以获取 tile 布局（例如每 tile 的索引表、按图层排布、每层偏移、是否压缩、tile 大小、tile count 等）。这是自底向上策略中的“格式解析”关键点。

4. sector / tilesurface 常量在 `GAME1024.cpp` 已明确（不同模式）。  
   - 结论：在实现 sector 组合时必须支持这些配置（或至少选择默认模式），并让配置可切换以验证与原客户端的像素对齐是否一致。

三、下一步精确的 deep-dive 列表（按优先级自底向上）
对每个文件，我将提取：关键结构、关键函数、输入输出（例如读取 zone 返回哪些数据结构），并把这些以“文件 + 函数名 + 行号范围 + 摘要”的形式写入 notes 文件。

优先级 A（必须先读）
- `client/Client/ZoneFileHeader.cpp` / `ZoneFileHeader.h`  
  - 要点：提取 zone header 数据结构、每个 zone 中 tile/layer 的字节布局、是否有压缩标志、tile 索引表格式、以及 zone 与 pack 的映射方式。

- `client/Client/PackFileManager.cpp` / `PackFileManager.h`  
  - 要点：如何打开 pack、列出条目、根据 ID 或名称读取原始 bytes，返回流/ buffer 的接口。

- `client/Client/SpriteLib/MPalette.cpp` / `MPalette.h`  
  - 要点：palette 文件的文件格式、如何读入 555/565 数据、palette 大小与索引映射。

- `client/Client/SpriteLib/CSpritePal.cpp` / `CSpritePal.h`  
  - 要点：`Blt` / `memcpy1555` 等实现（如何把索引像素映射为目标像素），并找出是否存在行对齐、pitch、clip 等需在加载时处理的细节。

优先级 B（在 A 完成后）
- `client/Client/MSector.h` / `MSector.cpp`、`MSectorInfo.h`  
  - 要点：sector 内 tile 的组织方式（按行或按 rhombus/isometric 布局）、sector 与 zone 的索引关系。

- `client/Client/MTopViewDraw.cpp` / `MTopViewDraw.inl`、`MTopView.cpp`  
  - 要点：绘制循环（按 tile / sector / layer），对象（creature/item）插入渲染顺序的准则（例如某些对象在 tile 之上或之下）。

- `client/Client/DXLib/CDirectDrawSurface.cpp` / `.h`  
  - 要点：原 surface 的创建方式、色深、锁图像的调用方式（Lock/Unlock）——以便设计 SDL adapter。

优先级 C（迭代完善）
- `client/Client/MZone.cpp`、`MZoneTable.cpp`（zone 高级管理）
- 其他与 tile 特效、屏幕特效、shadow blit 有关的文件（用于决定哪些 tile/层是动态的，哪些为静态合成）。

四、对 resurrected 的实现影响（实现前的要点清单）
1. Loader 必须能输出：tile 原始像素索引流 + 关联 palette（如果 palette 存在）。  
2. 解码器将负责：apply palette -> RGBA8888 buffer；如果有压缩，先做解压（需找出压缩算法）。  
3. TileSet 设计：每个 tileset 要有 tile count、tile dimensions、每个 tile 的源 rect（如果使用 atlas），并把解码后的像素上传为 SDL_Texture（或合并到 atlas）。  
4. Sector 设计：以 `TILESURFACE_SECTOR_WIDTH/HEIGHT` 为单位，生成 sector 级别的 `SDL_Texture`（render target）来合成该 sector 的静态层，减少运行时 draw calls。动态对象/特效按需在渲染时叠加。  
5. Adapter：设计一个小的 shim 层向旧代码暴露少量原来 `surface` 风格的方法（如 `CreateSurface`, `BltSpriteToSurface`），以便逐步替换旧渲染调用。

五、短期里程碑（下一次提交的目标产出）
- 产出 1（在本 notes 文件追加）：`ZoneFileHeader.cpp` 的结构与字段摘要（字节布局）；若存在压缩，说明压缩类型与解压入口函数。  
- 产出 2：`PackFileManager` 中用于读取条目的 API 摘要（函数名 + 输入输出）。  
- 产出 3：`CSpritePal` 中 palette 应用逻辑（关键函数）与测试向量（例如某些 palette index 在 RGB/RGBA 中的具体转换例子，如果代码包含样例像素数据，我会摘录）。  
- 产出 4：生成一份“迁移工作包”（小到可执行的 tasks），包括 `TileSet` loader prototype 的 pseudocode，以及如何把解码结果转成 SDL_Texture 的步骤。

六、风险点与需要你确认的问题（尽早确认以减少返工）
- 资源样本：请确认可用于测试的 zone/pack 示例文件的位置（或上传给我）。我需要至少一个小 zone pack（包含 tiles + palette + zone header），用于开发 loader 与调试转换。  
- 目标像素格式：我们建议把中间格式统一为 RGBA8888（SDL 默认），确认是否接受（若想保持 16bit 输出以节省内存/兼容某些算法，也可讨论）。  
- 是否要求像素级“完全一致”（pixel-perfect）与原 DirectDraw 渲染，还是允许在视觉上等效？像素级一致会增加对 palette/clip/shift/offset 的严格需求。  

七、行为与沟通
- 我现在准备开始按“优先级 A”顺序逐文件深入阅读并把每个文件的关键函数与行号记录到本 notes（追加）。完成后我会把摘录结果提交到 `client/resurrected/.kiro/notes/` 下的独立文件（例如 `zonefile_header.md`、`packfile_manager.md`、`palette_decode.md`），并把下一步实现任务分解到 `client/resurrected/.kiro/specs/tilemap.spec` 的任务清单里（带 checkboxes）。  
- 请确认我可以开始读取并提取上述文件的函数与行号。我会每读取一个文件就追加记录，便于你实时 review 并提出优先级调整。

附：本次 phase1 将要生成的 note 文件（预期）
- `client/resurrected/.kiro/notes/zonefile_header.md` — zone 文件格式详解（字段、偏移、样例），以及从 zone 到 tile bytes 的步骤。  
- `client/resurrected/.kiro/notes/packfile_manager.md` — pack 文件读取 API 摘录与调用样例。  
- `client/resurrected/.kiro/notes/palette_decode.md` — palette 文件格式、转换代码摘录、测试样例（palette index -> rgba）。  
- `client/resurrected/.kiro/notes/tile_decoder_prototype.md` — 从 raw bytes 到 SDL_Texture 的原型与伪代码。  

---

请确认：
- 我现在就开始对 `ZoneFileHeader.cpp` / `PackFileManager.cpp` / `MPalette.cpp` / `CSpritePal.cpp` 进行逐函数摘录并把结果追加到 notes（我会把每个函数的起止行与短摘要记录）。  
- 并请指明一个可用的 zone/pack 测试样本的路径，或允许我先在仓库中找一个典型样本（我会告诉你我发现的位置）。 若你没有样本，我会先完成解析代码草案与伪代码，等待样本来验证。