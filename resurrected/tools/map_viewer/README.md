# Map Viewer (Zone + SDL)

一个使用 `resurrected/lib/zone.c` 进行解析并用 SDL 渲染的轻量工具，用于在窗口中展示 `.map` (zone) 的指定扇区矩形区域，验证 tile 渲染管线是否正确。

目的
- 快速验证 zone -> `sprite_id` -> sprite 解码与渲染是否正确。
- 仅渲染基本地表 tile（不含生物/特效），用于视觉检查。

要点
- 复用现有代码：
  - spritepack 懒加载（`spritepack_*`）
  - sprite 解码（`sprite_decode` / `DecodedSprite` / `decoded_sprite_create_texture`）
  - SDL 框架封装（`sdl_framework`）
- 解析由 `resurrected/lib/zone.c` 完成（已验证 zone 加载成功）。
- 渲染时读取 `Zone.sectors` 中的 `sprite_id` 并在网格中绘制。

Dependencies
- SDL2 development libraries available on your system.
- The resurrected library sources / headers (the tool uses `lib/spritepack.h`,
  `lib/sprite.h` and `lib/sdl_framework.h`) — the tree structure in this repo is expected.

Typical file layout (relative to this README)
- `client/resurrected/tools/map_viewer/main.c` — the viewer program.
- `client/resurrected/lib/` — sprite, spritepack and sdl_framework implementations used by the tool.
- Example data paths in this repo:
  - `client/DarkEden/Data/Map/adam_c.map`
  - `client/DarkEden/Data/Image/tile.spk` (+ `tile.spki`)

构建
- 项目使用 CMake，`MapViewer` 已在 `resurrected/CMakeLists.txt` 中配置：

  ```
  cd client/resurrected/build
  cmake ..
  make    # or `cmake --build .`
  ```

    如需手工编译（不推荐），需自行补齐依赖并包含 `zone.c`：
    ```
    cc -Iclient/resurrected/lib \
      client/resurrected/tools/map_viewer/main.c \
      client/resurrected/lib/zone.c \
      client/resurrected/lib/error.c \
      client/resurrected/lib/sprite.c \
      client/resurrected/lib/spritepack.c \
      client/resurrected/lib/sdl_framework.c \
      -o MapViewer \
      `sdl2-config --cflags --libs`
    ```

 运行
- 基本用法：
  ```
  ./MapViewer <zone.map> <tile.spk> [--rect x,y,w,h] [--cell PIXELS] [--full]
  ```

- 示例：
  ```
  ./MapViewer client/DarkEden/Data/Map/adam_c.map client/DarkEden/Data/Image/tile.spk --rect 0,0,40,30 --cell 32
  ```
  ```
  ./MapViewer client/DarkEden/Data/Map/adam_c.map client/DarkEden/Data/Image/tile.spk --full --cell 32
  ```

Arguments
- `<map_file>`: path to a `.map` file.
- `<sprite_pack.spk>`: path to the tile sprite pack (expects lazy index file `*.spki` next to it).
- `--rect x,y,w,h` (optional): the rectangle of sectors to visualize.
  - x,y are sector coordinates (currently the viewer reads spriteIDs row-major from a detected
    offset; this MVP treats the block at the detected offset as the grid).
  - w,h are width and height (in sectors) of the area to render.
  - Defaults: `0,0,40,30`.
- `--cell PIXELS` (optional): size in pixels of each grid cell in the viewer window.
  - Default: `32`.

工具行为（MVP）
- 使用 `zone_load()` 解析 `.map`，获取 `zone.width/height` 与 `zone.sectors`。
- 读取指定 `--rect` 扇区区域的 `sprite_id`，从 `tile.spk` 懒加载并解码对应 sprite。
- 在 SDL 窗口中按网格渲染，每格缩放居中显示对应贴图，并绘制网格线辅助校验。
- 按 `Esc` 退出。

网格比例
- 默认 tile 网格采用 2:1 的宽高比例：宽度为 `--cell`，高度为 `--cell/2`。
- 例如：`--cell 32` → 每格 32×16 像素；`--cell 64` → 每格 64×32 像素。
- 该比例更贴近原始 2D 立体感展示，避免相邻 tile 之间出现空隙。

交互（--full 模式）
- 平移：`W/A/S/D` 或方向键
- 缩放：`+` / `-` 或鼠标滚轮（围绕屏幕中心缩放）
- 拖拽平移：按住鼠标右键拖动
- 重置视角：`1` 或 `Home`
- 开关网格：`G`

限制 / 注意事项
- 当前仅渲染地表 tile；未实现复杂分层、动画、阴影、组合缓存等。
- 若某些 sprite 解码失败（显示洋红色块），请检查：
  - `tile.spki` 是否与 `tile.spk` 同目录且可读；
  - 资源与现有 `spritepack` 解码兼容性；
  - 如需适配调色板或透明处理，请根据实际资源调整解码流程。

Debugging tips
- If the program fails to create textures or shows empty tiles:
  - Verify `tile.spk` and `tile.spki` exist next to each other and are readable.
  - Run `file` / `xxd` to inspect the `.map` file and confirm where sprite-like data appears.
- To log which spriteIDs were used and their offsets, add debug prints in the source
  right after reading the `sids[]` array in `main.c`.
- Compare with a screenshot from the original client to see if tile layout/look matches.

后续方向
- 基于 `Zone` 的完整语义实现分层渲染与合成（贴图缓存、阴影层等）。
- 引入相机、平移、缩放与增量重绘，靠近 `MTopView::DrawZone` 的绘制顺序与遮挡规则。
- 增加生物/物品/特效等渲染层，并验证绘制顺序。

Contact / notes
- This reader is intentionally minimal and focused on validating sprite decoding + mapping
  from `.map` -> spriteID -> `.spk`. Use it as a quick verification tool during the SDL
  migration of the render layer.
