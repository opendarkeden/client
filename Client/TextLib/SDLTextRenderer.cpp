#include "SDLTextRenderer.h"
#include "CGlyphCache.h"
#include "CTextLayout.h"
#include "../SpriteLib/CSpriteSurface.h"
#include <stdio.h>

#ifdef PLATFORM_MACOS

// External references
extern CSpriteSurface* g_pLast;

// Static glyph cache and layout (shared across all text rendering)
static CGlyphCache* g_GlyphCache = NULL;
static CTextLayout* g_TextLayout = NULL;

static void InitGlyphCache() {
    if (g_GlyphCache) return;  // Already initialized

    const char* fontPaths[] = {
        "Data/Font/Hiragino Sans GB.ttc",
        "Data/Font/NotoSansCJK-Regular.ttc",
        "Data/Font/NotoSans-Regular.ttf",
        "Data/Font/DejaVuSans.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "/System/Library/Fonts/Hiragino Sans GB.ttc",
        NULL
    };

    for (int i = 0; fontPaths[i] != NULL; i++) {
        g_GlyphCache = new CGlyphCache();
        g_TextLayout = new CTextLayout();
        if (g_GlyphCache->Init(fontPaths[i], 16)) {
            printf("SDL_RenderText: Initialized Font Atlas with %s\n", fontPaths[i]);
            return;
        } else {
            delete g_GlyphCache;
            delete g_TextLayout;
            g_GlyphCache = NULL;
            g_TextLayout = NULL;
        }
    }
    printf("SDL_RenderText: Failed to initialize Font Atlas\n");
}

void SDL_RenderText(int x, int y, const char* text) {
    // Lazy initialization
    if (!g_GlyphCache) {
        InitGlyphCache();
    }

    if (!g_GlyphCache || !g_TextLayout) {
        return;  // Font initialization failed
    }

    // Update layout
    g_TextLayout->Update(text, g_GlyphCache, 0);

    // Get glyphs
    const std::vector<GlyphPosition>& glyphs = g_TextLayout->GetGlyphs();

    // Get destination surface
    spritectl_surface_t destSurface = g_pLast->GetBackendSurface();
    if (destSurface == SPRITECTL_INVALID_SURFACE) {
        fprintf(stderr, "SDL_RenderText: Invalid destination surface\n");
        return;
    }

    // Render each glyph
    for (const auto& pos : glyphs) {
        if (!pos.glyph || pos.glyph->sprite == SPRITECTL_INVALID_SPRITE) {
            continue;
        }

        int screenX = x + pos.x;
        int screenY = y + pos.y + g_GlyphCache->GetFontAscent();

        int result = spritectl_blt_sprite(
            destSurface,
            screenX,
            screenY,
            pos.glyph->sprite,
            SPRITECTL_BLT_NONE,
            255
        );

        if (result != 0) {
            fprintf(stderr, "SDL_RenderText: spritectl_blt_sprite failed for charcode 0x%X\n",
                    pos.glyph->charcode);
        }
    }
}

#endif  // PLATFORM_MACOS
