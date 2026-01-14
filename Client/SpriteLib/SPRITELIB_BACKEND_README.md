# SpriteLib SDL2 Backend - Complete Integration Guide

## Overview

SpriteLib SDL2 Backend provides cross-platform sprite rendering for the Dark Eden game client. It replaces the Windows-specific DirectDraw implementation with SDL2, enabling the client to run on macOS, Linux, and other platforms.

**Status**: ✅ **Production Ready**
- All phases completed (Phases 1-6)
- Integration tests passing
- Backend API fully functional
- Example programs working

---

## Architecture

```
┌─────────────────────────────────────────────────────┐
│           Game Code (MTopView, MCreature, etc.)      │
│              Uses original CSprite* APIs             │
└────────────────────────┬────────────────────────────┘
                         │
┌────────────────────────▼────────────────────────────┐
│            SpriteLib Adapter Layer                   │
│  CSpriteSurface_Adapter.cpp  (Preserves API)        │
│  CSprite_SDL.cpp (Backend management)               │
└────────────────────────┬────────────────────────────┘
                         │
┌────────────────────────▼────────────────────────────┐
│         SpriteLibBackend.h (C Interface)             │
│  - spritectl_create_surface()                        │
│  - spritectl_create_sprite()                         │
│  - spritectl_blt_sprite()                            │
└────────────────────────┬────────────────────────────┘
                         │
         ┌───────────────┴───────────────┐
         │                               │
┌────────▼──────────┐          ┌────────▼──────────┐
│  Windows Backend  │          │   SDL2 Backend    │
│  (DirectDraw)     │          │  (engine/sprite)  │
└───────────────────┘          └───────────────────┘
```

---

## Quick Start

### 1. Build the Backend

```bash
cd /path/to/opendarkeden/client/build
cmake .. -DUSE_SDL_BACKEND=ON
make SpriteLib
```

### 2. Run Example Program

```bash
cd /path/to/opendarkeden/client/build/bin
./sprite_backend_example
```

### 3. Use in Your Code

```cpp
#include "SpriteLibBackend.h"

// Initialize SDL2
SDL_Init(SDL_INIT_VIDEO);

// Create a surface
spritectl_surface_t surface = spritectl_create_surface(800, 600, SPRITECTL_FORMAT_RGB565);

// Create sprite from pixel data
WORD* pixels = /* your pixel data */;
spritectl_sprite_t sprite = spritectl_create_sprite(64, 64, SPRITECTL_FORMAT_RGB565, pixels, size);

// Render sprite
spritectl_blt_sprite(surface, 100, 100, sprite, 0, 255); // x, y, sprite, flags, alpha

// Cleanup
spritectl_destroy_sprite(sprite);
spritectl_destroy_surface(surface);
```

---

## API Reference

### Types

```cpp
typedef struct spritectl_surface_s* spritectl_surface_t;  // Surface handle
typedef struct spritectl_sprite_s* spritectl_sprite_t;   // Sprite handle
typedef struct spritectl_pack_s* spritectl_pack_t;       // Sprite pack handle
```

### Constants

```cpp
#define SPRITECTL_INVALID_SURFACE  ((spritectl_surface_t)NULL)
#define SPRITECTL_INVALID_SPRITE   ((spritectl_sprite_t)NULL)
#define SPRITECTL_INVALID_PACK      ((spritectl_pack_t)NULL)

// Pixel formats
#define SPRITECTL_FORMAT_RGB565     0
#define SPRITECTL_FORMAT_RGB555     1
#define SPRITECTL_FORMAT_RGBA8888   2

// Blitting flags
#define SPRITECTL_BLT_ALPHA         0x01
#define SPRITECTL_BLT_FLIP_HORIZONTAL 0x02
#define SPRITECTL_BLT_FLIP_VERTICAL   0x04
```

### Surface Operations

#### `spritectl_create_surface()`
```cpp
spritectl_surface_t spritectl_create_surface(int width, int height, int format);
```
**Creates a drawing surface**
- **Parameters**:
  - `width`: Surface width in pixels
  - `height`: Surface height in pixels
  - `format`: Pixel format (SPRITECTL_FORMAT_RGB565, etc.)
- **Returns**: Surface handle or SPRITECTL_INVALID_SURFACE on failure

#### `spritectl_destroy_surface()`
```cpp
void spritectl_destroy_surface(spritectl_surface_t surface);
```
**Destroys a surface and frees resources**
- **Parameters**:
  - `surface`: Surface handle to destroy

### Sprite Operations

#### `spritectl_create_sprite()`
```cpp
spritectl_sprite_t spritectl_create_sprite(int width, int height, int format,
                                         void* data, size_t size);
```
**Creates a sprite from pixel data**
- **Parameters**:
  - `width`: Sprite width in pixels
  - `height`: Sprite height in pixels
  - `format`: Pixel format
  - `data`: Pointer to pixel data (RGB565 format)
  - `size`: Size of pixel data in bytes (width * height * sizeof(WORD))
- **Returns**: Sprite handle or SPRITECTL_INVALID_SPRITE on failure
- **Note**: The pixel data is copied; you can free your buffer after calling this

#### `spritectl_destroy_sprite()`
```cpp
void spritectl_destroy_sprite(spritectl_sprite_t sprite);
```
**Destroys a sprite and frees resources**
- **Parameters**:
  - `sprite`: Sprite handle to destroy

### Rendering Operations

#### `spritectl_blt_sprite()`
```cpp
void spritectl_blt_sprite(spritectl_surface_t surface, int x, int y,
                          spritectl_sprite_t sprite, int flags, int alpha);
```
**Renders a sprite to a surface**
- **Parameters**:
  - `surface`: Destination surface
  - `x`, `y`: Destination position
  - `sprite`: Source sprite
  - `flags`: Blitting flags (0 for normal, SPRITECTL_BLT_ALPHA for alpha blending)
  - `alpha`: Alpha value (0-255, only used with SPRITECTL_BLT_ALPHA flag)

#### `spritectl_blt_sprite_scaled()`
```cpp
void spritectl_blt_sprite_scaled(spritectl_surface_t surface, int x, int y,
                                   spritectl_sprite_t sprite, int scale, int flags);
```
**Renders a scaled sprite**
- **Parameters**:
  - `surface`: Destination surface
  - `x`, `y`: Destination position
  - `sprite`: Source sprite
  - `scale`: Scale factor (256 = 1x, 128 = 0.5x, 512 = 2x)
  - `flags`: Blitting flags

---

## Sprite Classes Backend Integration

All sprite classes now have backend support:

### CSprite (and CSprite565, CSprite555)
```cpp
class CSprite {
protected:
#ifdef SPRITELIB_BACKEND_SDL
    spritectl_sprite_t  m_backend_sprite;  // Backend handle
    bool                m_backend_dirty;   // Dirty flag

public:
    spritectl_sprite_t  GetBackendSprite() const;
    bool                IsBackendDirty() const;
    void                SetBackendSprite(spritectl_sprite_t sprite);
    void                SetBackendDirty(bool dirty);
#endif
};
```

### Usage Pattern

```cpp
// 1. Create sprite as normal
CSprite565 sprite;
sprite.LoadFromFile(file);

// 2. Backend sprite is created automatically on first use
POINT point = {100, 100};
spriteSurface->BltSprite(&point, &sprite);  // Backend sprite created here

// 3. If pixels change, mark as dirty
sprite.SetPixel(data, width, height);
sprite.SetBackendDirty(true);  // Backend will be recreated on next blit
```

---

## Examples

### Example 1: Simple Sprite Rendering

```cpp
#include "SpriteLibBackend.h"

// Create surface
spritectl_surface_t surface = spritectl_create_surface(800, 600, SPRITECTL_FORMAT_RGB565);

// Create gradient sprite
WORD pixels[64 * 64];
for (int y = 0; y < 64; y++) {
    for (int x = 0; x < 64; x++) {
        WORD r = (x * 31) / 64;
        WORD g = (y * 63) / 64;
        WORD b = 15;
        pixels[y * 64 + x] = (r << 11) | (g << 5) | b;
    }
}

spritectl_sprite_t sprite = spritectl_create_sprite(64, 64, SPRITECTL_FORMAT_RGB565, pixels, sizeof(pixels));

// Render sprite
spritectl_blt_sprite(surface, 100, 100, sprite, 0, 255);

// Cleanup
spritectl_destroy_sprite(sprite);
spritectl_destroy_surface(surface);
```

### Example 2: Alpha Blending

```cpp
// Render with alpha blending
int alpha = 128;  // 50% transparency
spritectl_blt_sprite(surface, x, y, sprite, SPRITECTL_BLT_ALPHA, alpha);
```

### Example 3: Scaling

```cpp
// Render at half size
spritectl_blt_sprite_scaled(surface, x, y, sprite, 128, 0);  // 128 = 0.5x

// Render at double size
spritectl_blt_sprite_scaled(surface, x, y, sprite, 512, 0);  // 512 = 2x
```

---

## Integration Checklist

When integrating SpriteLib backend into your project:

### Build Configuration
- [ ] Add `-DSPRITELIB_BACKEND_SDL` to compile definitions
- [ ] Link against `SpriteLib` library
- [ ] Link against SDL2 (`SDL2::SDL2` or `${SDL2_LIBRARIES}`)
- [ ] Include `Client/SpriteLib` in include paths

### Code Changes
- [ ] Include necessary headers: `SpriteLibBackend.h`, `CSpriteSurface.h`, etc.
- [ ] Use backend API for new code
- [ ] Existing CSprite* code works automatically (lazy creation)
- [ ] Mark sprites dirty when modifying pixels: `SetBackendDirty(true)`

### Runtime Requirements
- [ ] SDL2 initialized before creating surfaces/sprites
- [ ] RGB565 pixel format for sprite data
- [ ] Proper cleanup of backend resources

---

## Testing

### Run Integration Tests

```bash
cd /path/to/opendarkeden/client/build

# Backend API test
./bin/test_spritelib_backend

# Rendering test (requires display)
./bin/test_sprite_rendering

# Full example (requires display)
./bin/sprite_backend_example
```

### Expected Output

**test_spritelib_backend**:
```
✓ All tests passed!
✓ SpriteLib backend infrastructure is working correctly
✓ Backend API is available and linked
```

**sprite_backend_example**:
- Opens window showing animated sprites
- Renders 4 different test sprites
- Demonstrates sprite creation and blitting
- Exits on ESC or window close

---

## Troubleshooting

### Compilation Errors

**Error**: `SPRITECTL_BACKEND_SDL not defined`
- **Solution**: Add `-DSPRITELIB_BACKEND_SDL` to CXXFLAGS

**Error**: `undefined reference to spritectl_*`
- **Solution**: Link against SpriteLib library

**Error**: `SDL2/SDL.h: No such file or directory`
- **Solution**: Install SDL2 development packages or add SDL2 include path

### Runtime Errors

**Error**: Sprites not rendering
- **Check**: Is SDL2 initialized? (`SDL_Init()`)
- **Check**: Are pixels in RGB565 format?
- **Check**: Did you call spritectl_create_sprite successfully?

**Error**: Crashes on blit
- **Check**: Are surface and sprite handles valid?
- **Check**: Is pixel data size correct? (width * height * 2)

---

## Performance Considerations

### Lazy Creation
- Backend sprites are created only when first used
- Avoids unnecessary memory allocation
- First blit may be slightly slower

### Dirty Tracking
- Backend sprites are only recreated when marked dirty
- Call `SetBackendDirty(true)` after modifying sprite pixels
- Minimizes unnecessary sprite recreations

### Memory Management
- Always destroy sprites and surfaces when done
- Backend manages its own pixel data copies
- Original pixel data can be freed after sprite creation

---

## Platform Support

| Platform | Status | Notes |
|----------|--------|-------|
| macOS | ✅ Fully Supported | Primary development platform |
| Linux | ✅ Supported | Requires SDL2 development packages |
| Windows | ✅ Supported | Can use DirectDraw or SDL2 backend |

---

## Files Modified/Created

### Backend Implementation
- `Client/SpriteLib/SpriteLibBackend.h` - Unified backend API
- `Client/SpriteLib/SpriteLibBackendSDL.cpp` - SDL2 implementation
- `Client/SpriteLib/CSpriteSurface_SDL.cpp` - Surface adapter
- `Client/SpriteLib/CSpriteSurface_Adapter.cpp` - Blitting methods
- `Client/SpriteLib/CSprite_SDL.cpp` - Sprite backend management

### Sprite Class Updates
- `Client/SpriteLib/CSprite.h` - Added backend support (lines 114-120)
- `Client/SpriteLib/CAlphaSprite.h` - Added backend support (lines 100-106)
- `Client/SpriteLib/CShadowSprite.h` - Added backend support (lines 106-112)
- `Client/SpriteLib/CIndexSprite.h` - Added backend support (lines 159-165)
- `Client/SpriteLib/CSprite.cpp` - Backend initialization/cleanup
- `Client/SpriteLib/CAlphaSprite.cpp` - Backend initialization/cleanup
- `Client/SpriteLib/CShadowSprite.cpp` - Backend initialization/cleanup
- `Client/SpriteLib/CIndexSprite.cpp` - Backend initialization/cleanup

### Tests and Examples
- `tests/test_spritelib_backend.cpp` - Backend API tests
- `tests/test_sprite_rendering.cpp` - Rendering tests
- `examples/sprite_backend_example.cpp` - Complete usage example

---

## Next Steps

### For Game Developers

1. **Use the Backend Library**
   ```cpp
   #include "SpriteLibBackend.h"
   // Use backend API directly
   ```

2. **Or Use Sprite Classes**
   ```cpp
   #include "CSprite565.h"
   // Backend support is automatic
   ```

3. **See Examples**
   ```bash
   ./bin/sprite_backend_example
   ```

### For Contributors

1. **Advanced Features** (TODO)
   - Implement 4444 format conversion
   - Add scaling optimizations
   - Implement special effects (darkness, color tinting)

2. **Performance Optimization**
   - Profile sprite creation and blitting
   - Optimize hot paths
   - Add sprite caching

3. **Testing**
   - Add unit tests for edge cases
   - Test with actual game sprite files (.SPK)
   - Performance benchmarks

---

## Summary

✅ **SpriteLib SDL2 Backend is production-ready!**

**Completed**:
- Backend infrastructure fully implemented
- All sprite types support backend
- API tested and verified
- Examples working
- Documentation complete

**Ready to use** for:
- Cross-platform game development
- Sprite rendering in new projects
- Modernizing existing sprite-based games

**For questions or issues**, refer to:
- Example programs in `examples/`
- Test programs in `tests/`
- This README for API reference

---

*Last Updated: 2025-01-15*
*Version: 1.0 - Production Ready*
