# Tilemap Spec for resurrected

Purpose
- Persist the plan and tasks for porting the original DirectX tilemap (tilesurface / sector) system from the original client into the SDL-based `resurrected` client.
- Keep an indexed list of original source files to inspect and concrete milestones to implement in `resurrected`.
- Provide a clear incremental roadmap so work can be checkpointed, reviewed, and reverted if necessary.

Scope
- Implement map/tile rendering in `client/resurrected` using SDL (SDL_Renderer + SDL_Texture or equivalent GPU-backed approach).
- Reuse as much game logic as possible (zone logic, pathfinding, gameplay) and replace only rendering-related subsystems.
- Start with static tile loading and rendering, then add compositing (sector-level render targets), dynamic objects (creatures/items), and map effects/animations.

Milestones (M)
- M0 — Preparation & discovery
  - Inventory original client files and constants related to tiles/sector.
  - Decide the SDL rendering approach (SDL_Renderer + render-target textures vs direct GL).
  - Sketch the module API for the tile subsystem.

- M1 — Design & spec (this file)
  - Finalize module boundaries, data structures and adapter strategy (how to minimize changes to game logic).
  - Create an initial on-disk spec and task list (this document).

- M2 — Resource loader
  - Implement `TileSet` loader that can extract tiles/atlases from existing pack/zone files.
  - Handle palette/indexed -> RGBA conversion and colorkey -> alpha mapping.

- M3 — Sector compositing & basic rendering
  - Implement `TileSector` compositing: render all static tiles of a sector into a sector texture (render target).
  - Implement `TileRenderer` to draw visible sectors given a viewport/camera.
  - Provide `tools/map_viewer` to load a zone and move the camera interactively.

- M4 — Integration of dynamic objects
  - Integrate existing creature/item sprite rendering with the new renderer while preserving draw order/occlusion.
  - Implement click (screen -> tile) and tile highlighting tests.

- M5 — Animations and effects
  - Implement tile animations (e.g., flowing water) and ground-level effects.
  - Support per-tile frame updates or dynamic overlay layers.

- M6 — Performance tuning & cross-platform validation
  - Profile draw calls, texture changes, sector cache hit/miss.
  - Implement LRU sector cache, batching, atlases.
  - Test on macOS & Linux (and any other target platforms).

- M7 — Integration into main client
  - Replace original map rendering calls with new module or add an adapter shim to minimize game logic changes.
  - Run full-game regression tests.

Tasks (persistent checklist)
- [ ] Create `client/resurrected/src/graphics/tilemap` skeleton: `TileSet`, `TileSector`, `TileRenderer`.
- [ ] Create `client/resurrected/tools/map_viewer` for interactive testing.
- [ ] Implement pack/zone adapter reusing original pack parsing logic where possible.
- [ ] Implement palette/indexed pixel conversion pipeline (support original colorkey).
- [ ] Implement SDL-based sector compositing with `SDL_SetRenderTarget` and `SDL_Texture`.
- [ ] Implement viewport-to-sector selection and on-demand sector generation.
- [ ] Implement sector LRU cache and memory cap.
- [ ] Integrate creature/animation drawing in correct draw order.
- [ ] Add unit tests & sample assets for loader and viewer.
- [ ] Profile and optimize rendering (reduce texture binds & draw calls).
- [ ] Document API and migration/adaptation steps for game logic.

Files in original client to analyze (initial list)
- `client/Client/GAME1024.cpp` — tile/sector size constants and alternative display modes (1024/800).
- `client/Client/Client_PCH.h` — global declarations for tile constants such as `g_TILESURFACE_*`, `g_TILE_X_HALF`, `g_TILE_Y_HALF`.
- `client/Client/MTopView.cpp` — top-level map view logic (camera, orchestration).
- `client/Client/MTopViewDraw.cpp` and `client/Client/MTopViewDraw.inl` — detailed drawing helpers and draw order code.
- `client/Client/MZone.cpp` / `client/Client/MZone.h` — zone data structures and loading integration.
- `client/Client/MZoneTable.cpp` / `MZoneTable.h` — zone table management.
- `client/Client/MSector.cpp` / `MSector.h` / `MSectorInfo.h` — sector structures and metadata.
- `client/Client/ZoneFileHeader.cpp` — zone file header and format parsing (investigate layout).
- `client/Client/PackFileManager.cpp` / `PackFileManager.h` — pack/SPK file I/O and extraction.
- `client/Client/DXLib/CDirectDrawSurface.cpp` / `.h` (or `client/Client/CDirectDrawSurface.*`) — how "surfaces" are created, blitted and used in the original.
- Any other helper files referenced from the draw path (search for "tile", "sector", "tilesurface" in original codebase).

Design decisions & rationale
- Sector-based cached compositing: composing static tiles of a sector into a single texture significantly reduces per-frame draw calls. This mirrors the original engine's sector approach and is suitable for large maps.
- Palette/indexed handling: original assets often use indexed color + palette and rely on colorkey transparency. Convert to RGBA during load and store as SDL textures; this simplifies runtime rendering and avoids per-frame palette application cost.
- Adapter approach: to reduce risk, implement a small adapter/shim layer that exposes a subset of the original surface/tile API but internally uses SDL. This lets us switch gradually without having to change core game logic everywhere at once.

Data structures (high-level)
- TileSet
  - Source pack/zone reference
  - Tile dimensions and count
  - Atlas metadata: SDL_Textures and list of tile rects
  - Palette (if any) and conversion helpers
- TileSector
  - Sector coords (sx, sy)
  - Sector pixel dimensions
  - Static render texture (SDL_Texture) — composed when sector is first needed
  - Timestamp/usage for LRU
  - Dirty flag for dynamic changes
- TileRenderer
  - Viewport/camera state
  - Sector cache map (sx, sy -> TileSector)
  - draw(viewport): determine visible sectors and render them in correct order
  - update/draw dynamic layers (creature, item, effects)

Edge cases & questions to resolve
- Confirm whether any tiles use per-pixel alpha (true alpha channel) in original data, or everything is palette+colorkey.
- Confirm original layering/occlusion rules for tiles vs creatures vs items — is it strictly tile layer order or per-tile priority?
- Confirm whether tiles are ever modified at runtime (e.g., digging, changing ground), which affects sector dirty strategy.
- Confirm texturing limits for large sectors on some target GPUs — sector dimensions may need clamping or tiling.

Acceptance criteria (per milestone)
- M2: Loader test: given sample zone/pack, loader returns tileset metadata and can produce SDL textures for tiles. Unit test loads known sample and verifies tile count and several canonical tile pixels after palette conversion.
- M3: Viewer test: `tools/map_viewer` can display a zone, camera can pan/zoom (if applicable), and sector composition works (visual correctness compared to reference).
- M4: Integration test: creatures/items render at correct positions and occlusion matches original logic in a small scenario.
- M6: Performance test: typical view should render at 60 FPS on development machine with reasonable memory (document exact machine & numbers in test report).

Next immediate steps (I will do upon your confirmation)
1. Deep-dive and extract exact functions and code paths responsible for:
   - initializing tilesurfaces (where surfaces are created and sized)
   - sector composition and tile blitting loops
   - palette handling and pixel formats
   I will create a file-to-function map (file + key function names + important constants with line references).

2. Create the resurrected skeleton files:
   - `client/resurrected/src/graphics/tilemap/TileSet.*`
   - `client/resurrected/src/graphics/tilemap/TileSector.*`
   - `client/resurrected/src/graphics/tilemap/TileRenderer.*`
   - `client/resurrected/tools/map_viewer/*` (simple SDL viewer app)
   These will be minimal and compile where possible, with TODOs for loader/compositor.

3. Implement and iterate (M2 -> M3).

If you agree with this spec and the task list, confirm and I will start the deep-dive to enumerate exact functions and line ranges to be used in the implementation plan. If you want the spec in Chinese or want any additions to milestone/tasks, tell me and I'll update the spec file accordingly.
