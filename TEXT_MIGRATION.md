# Text Migration Plan (SDL)

Goal: Replace legacy Win+DirectX text with an SDL-based, layered text system that supports both UI and in-world rendering.

## Design Overview

### Architecture
```
UI / Game code
  -> TextService (layout + measure + draw)
       -> TextBackend (SDL_ttf)
            -> Glyph cache (spritectl sprites)
                -> RenderTarget (SpriteSurface adapter)
```

### Render Target Rules
- UI and in-world text both render to `CSpriteSurface` via `SpriteSurfaceRenderTarget`.
- Active target is set via `g_SetFL2Surface(CSpriteSurface*)`; default is `g_pLast` when unset.
- Never pass raw surface pointers to `g_SetFL2Surface` (must be `CSpriteSurface*`).

### Encoding Strategy
- All rendering paths normalize to UTF-8.
- If input is not valid UTF-8, attempt CP949 then EUC-KR.
- This is done centrally inside `TextService` to avoid per-call conversions.

### Font Strategy
- `PrintInfo::hfont` is a Windows handle on legacy, but in SDL builds it is a size-encoded token.
- `Base::SetFont` encodes `lf.lfHeight` into the handle; `TextService::GetFont(size)` uses it.
- This preserves existing size differences across UI screens without depending on GDI.

### Rendering API
- `g_Print`, `g_PrintColorStr*`, `g_GetStringWidth/Height` are routed to `TextService`.
- Alignment uses `PrintInfo::text_align` when available.
- Shadow/outline implemented via multi-draw offsets.

## Detailed Plan

### Step 3 — In-world text rendering
Scope:
- Player/NPC names, system messages, notices, item labels, chat bubbles.
- All in-world call sites still using `g_Print*` and `g_GetStringWidth/Height`.

Plan:
1. Ensure `g_Print*` + `g_GetStringWidth/Height` use TextService and correct RenderTarget.
2. Verify font size mapping is consistent across in-world UI.
3. Fix remaining garbled strings by extending normalization (if needed).
4. Validate alignment in `MTopView.cpp` overlays (centered names, etc.).

Completed Work (2026-01-30):
1. ✅ Verified RenderTarget management:
   - MTopView::m_pSurface is set to g_pLast (GameInit.cpp:765)
   - Text rendering uses ResolveTextSurface() which returns g_pFL2Surface → g_pLast → g_pBack
   - Since g_pFL2Surface is NULL during normal rendering, text renders to g_pLast
   - MTopView and text rendering use the same surface - NO FIX NEEDED

2. ✅ Verified font size encoding:
   - Base::SetFont() encodes lf.lfHeight using EncodeFontSizeHandle() (VS_UI_Base.cpp:128)
   - g_Print() decodes using DecodeFontSizeHandle() (RenderingFunctions.cpp:127)
   - Font size correctly flows through entire pipeline - NO FIX NEEDED

3. ✅ Fixed UTF-8 buffer overflow:
   - Added bounds checking to Utf8Decode() function
   - Updated all call sites (MeasureLineWidth, WrapText, DrawLine)
   - Prevents crashes on malformed UTF-8 sequences

Acceptance:
- No garbage text on any visible in-world overlays.
- Correct alignment for centered labels and notices.
- No crashes when switching render targets.
- [ ] Pending: In-game testing with actual Korean text
- [ ] Pending: Verify alignment in practice

### Step 4 — Text input
Scope:
- LineEditorVisual (chat input, login, dialog entry).
- IME (Korean/Chinese) through SDL text input.

Plan:
1. Render LineEditorVisual through TextService (no GDI / FL2).
2. Ensure UTF-8 cursor math matches rendered glyphs.
3. Keep input buffers in UTF-8, convert only for legacy packet formats if required.

Acceptance:
- Chat input works end-to-end (input -> visible -> send).
- Cursor position matches glyph positions.
- IME composition does not corrupt input buffer.

### Step 4 — Text input (Complete ✅)
Scope:
- LineEditorVisual (chat input, login, dialog entry).
- IME (Korean/Chinese) through SDL text input.

Completed Work (2026-01-31):
1. ✅ Created InputFocusManager:
   - Singleton pattern for managing text input focus
   - Routes SDL_TEXTINPUT, SDL_TEXTEDITING, and SDL_KEYDOWN events
   - Files: VS_UI/src/InputFocusManager.{h,cpp}

2. ✅ Modified SDL event routing:
   - DXLibBackendSDL.cpp now routes text events to InputFocusManager
   - Added VK_* key mapping for control keys (backspace, delete, arrows, home/end)

3. ✅ Updated LineEditorVisual:
   - Acquire() registers with InputFocusManager, enables SDL text input
   - Unacquire() releases focus, disables SDL text input
   - Destructor clears InputFocusManager reference (prevents use-after-free)

4. ✅ Unified text rendering:
   - Removed CGlyphCache/CTextLayout dual-system (~160 lines)
   - Now uses TextService for all rendering
   - Proper baseline and cursor position calculation

5. ✅ Fixed encoding issues:
   - GetStringWide() now correctly converts UTF-32 → UTF-16
   - Chat uses UTF-8 directly via GetString()
   - Fixed buffer overflow in AddWhisperID()

6. ✅ Fixed control keys:
   - Backspace works correctly (no double cursor movement)
   - Delete, arrows, home/end all functional
   - Fixed Descender letters (g,j,p,q,y) alignment

Acceptance:
- ✅ Chat input works end-to-end (input → visible → send)
- ✅ Cursor position matches glyph positions
- ✅ IME composition does not corrupt input buffer
- ✅ No crashes on UI destruction

### Step 5 — UI module sweep (Complete ✅)
Scope:
- Mail / shop / trade / misc dialogs.

Completed Work (2026-01-31):
1. ✅ Scanned all VS_UI files for Windows-only text code:
   - Only 2 Windows GDI TextOut calls found
   - VS_UI_GameCommon.cpp:5115: Has #else branch using g_PrintColorStr
   - hangul/FL2.cpp:188: Legacy FL2 renderer (fallback system)

2. ✅ Verified all main dialogs use correct APIs:
   - VS_UI_Shop.cpp: Uses sprite system for items, buttons for text
   - VS_UI_Storage.cpp: Uses g_Print (4 calls)
   - VS_UI_Exchange.cpp: Uses g_Print (14 calls)
   - VS_UI_Description.cpp: Uses g_Print extensively (335 calls)

3. ✅ No hardcoded width calculations found:
   - All dialogs use g_GetStringWidth/Height for measurements
   - No strlen() * FONT_SIZE or similar hacks

4. ✅ All platform branches have SDL alternatives:
   - 15 #ifdef PLATFORM_WINDOWS blocks total
   - All have corresponding #else or platform-specific implementations

Acceptance:
- ✅ UI screens render text consistently
- ✅ No dependency on GDI/DirectX in SDL builds
- ✅ All text uses g_Print* or TextService

Plan:
1. Replace any remaining ad-hoc text rendering with `g_Print*` or direct TextService calls.
2. Replace width/height hacks with `g_GetStringWidth/Height`.
3. Remove any Windows-only text code paths in SDL builds.

Acceptance:
- UI screens render text consistently.
- No dependency on GDI/DirectX in SDL builds.

## Planned File Changes (Rolling List)

### Core Text System
- `Client/TextSystem/TextService.*`
- `Client/TextSystem/TextBackendSDL.cpp`
- `Client/TextSystem/RenderTargetSpriteSurface.*`
- `Client/TextSystem/FontHandleUtil.h`

### Global Rendering Hooks
- `Client/RenderingFunctions.cpp`
- `Client/GameInit.cpp`
- `Client/CGameUpdate.cpp`
- `VS_UI/src/VS_UI_Base.cpp`

### In-world Rendering
- `Client/MTopView.cpp`
- `Client/CGameUpdate.cpp`

### UI Inputs
- `VS_UI/src/widget/U_edit.cpp`
- `VS_UI/src/VS_UI_Title.cpp`
- `VS_UI/src/VS_UI_GameCommon.cpp`

## Risks / Watch-outs
- RenderTarget must always be a `CSpriteSurface*` (no raw surface pointer).
- Font size differences depend on `lf.lfHeight` being preserved.
- Some legacy strings may still be CP949/EUC-KR; normalization must remain centralized.

## Test Checklist
- NPC dialog text (multi-line + menu options).
- In-world system notices + player/NPC name labels.
- Chat input (typing + cursor + IME).
- UI screens with mixed font sizes.

## Milestones

1) Text system foundation
   - Implement TextService (layout, measure, draw)
   - SDL_ttf backend + glyph cache
   - RenderTarget abstraction (SpriteSurface adapter)
   - Status: ✅ Complete

2) NPC dialog text
   - Dialog message + menu rendering via TextService
   - Encoding normalization (CP949/EUC-KR -> UTF-8)
   - Status: ✅ Complete

3) In-world text rendering
   - g_Print/g_GetStringWidth/Height route to TextService
   - Ensure correct RenderTarget (g_pLast / g_pBack)
   - Fix any remaining garbled text or alignment issues
   - Status: ⏳ In progress

4) Text input
   - LineEditorVisual rendering + cursor
   - UTF-8/IME correctness (SDL text input path)
   - Replace any residual GDI/FL2 dependencies
   - Status: ✅ Complete (2026-01-31)

5) UI module sweep
   - Mail / trade / shop / misc dialogs
   - Replace remaining g_PrintColorStr/g_GetStringWidth usage where needed
   - Status: ✅ Complete (2026-01-31)

## Notes

- SDL is the only target; legacy Win+DirectX paths are not required.
- Prefer clean abstractions: no direct DXLib/Surface usage in UI/game logic.
- Use TextService for rendering and measuring text everywhere practical.
