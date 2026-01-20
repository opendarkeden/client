#include "CTextSpriteBatch.h"
#include <stdio.h>

// External reference to SDL renderer (defined in SDLMain.cpp or similar)
extern SDL_Renderer* g_pSDLRenderer;

CTextSpriteBatch::CTextSpriteBatch()
    : m_cache(NULL)
    , m_surface(NULL)
{
}

CTextSpriteBatch::~CTextSpriteBatch() {
}

bool CTextSpriteBatch::Init(CGlyphCache* cache, CSpriteSurface* surface) {
    if (!cache || !surface) {
        fprintf(stderr, "CTextSpriteBatch::Init: Invalid parameters\n");
        return false;
    }

    m_cache = cache;
    m_surface = surface;

    return true;
}

void CTextSpriteBatch::AddGlyph(int x, int y, const GlyphInfo* glyph, SDL_Color color, float alpha) {
    if (!glyph) {
        return;
    }

    TextRenderOp op;
    op.x = x;
    op.y = y;
    op.glyph = glyph;
    op.color = color;
    op.alpha = alpha;
    m_batch.push_back(op);
}

void CTextSpriteBatch::Flush() {
    if (!m_cache || !m_surface || m_batch.empty()) {
        return;
    }

    // Get Font Atlas texture
    SDL_Texture* atlas = m_cache->GetAtlasTexture();
    if (!atlas) {
        fprintf(stderr, "CTextSpriteBatch::Flush: No Font Atlas texture\n");
        return;
    }

    // Save current render target
    SDL_Texture* oldTarget = SDL_GetRenderTarget(g_pSDLRenderer);

    // Get destination surface's spritectl backend
    // Note: We need to render glyphs to the surface, not directly to screen
    // The surface will later be presented via spritectl_present_surface()

    // For now, we'll render directly to the renderer's target
    // TODO: Integrate properly with spritectl surface system

    for (const auto& op : m_batch) {
        SDL_Rect srcRect = {
            op.glyph->x,
            op.glyph->y,
            op.glyph->width,
            op.glyph->height
        };

        SDL_Rect destRect = {
            op.x,
            op.y,
            op.glyph->width,
            op.glyph->height
        };

        // Set color modulation
        SDL_SetTextureColorMod(atlas, op.color.r, op.color.g, op.color.b);
        SDL_SetTextureAlphaMod(atlas, (Uint8)(op.alpha * 255));

        // Render glyph
        SDL_RenderCopy(g_pSDLRenderer, atlas, &srcRect, &destRect);
    }

    // Restore previous render target
    SDL_SetRenderTarget(g_pSDLRenderer, oldTarget);

    // Clear batch after rendering
    m_batch.clear();
}

void CTextSpriteBatch::Clear() {
    m_batch.clear();
}
