#ifndef __CTEXTSPRITEBATCH_H__
#define __CTEXTSPRITEBATCH_H__

#include "CGlyphCache.h"
#include <SDL2/SDL.h>
#include <vector>

// Forward declarations
class CSpriteSurface;

/**
 * TextRenderOp - Single glyph render operation
 */
struct TextRenderOp {
    int x, y;                  // Screen coordinates
    const GlyphInfo* glyph;   // Glyph to render
    SDL_Color color;          // Text color
    float alpha;              // Alpha blend (0.0 - 1.0)
};

/**
 * CTextSpriteBatch - Batch rendering for text glyphs
 *
 * Accumulates glyph render operations and executes them efficiently.
 * Reduces draw calls by batching glyphs from the same Font Atlas.
 */
class CTextSpriteBatch {
public:
    CTextSpriteBatch();
    ~CTextSpriteBatch();

    /**
     * Initialize batch renderer
     * @param cache Glyph cache (provides Font Atlas)
     * @param surface Destination surface (spritectl surface)
     * @return true on success
     */
    bool Init(CGlyphCache* cache, CSpriteSurface* surface);

    /**
     * Add a glyph to the batch
     * @param x Screen X coordinate
     * @param y Screen Y coordinate
     * @param glyph Glyph info from cache
     * @param color Text color
     * @param alpha Alpha blending (default 1.0 = opaque)
     */
    void AddGlyph(int x, int y, const GlyphInfo* glyph, SDL_Color color, float alpha = 1.0f);

    /**
     * Execute batch rendering
     * Renders all queued glyphs to the destination surface.
     * Clears the batch after rendering.
     */
    void Flush();

    /**
     * Clear batch without rendering
     */
    void Clear();

    /**
     * Get batch size
     * @return Number of pending operations
     */
    size_t GetSize() const { return m_batch.size(); }

private:
    CGlyphCache* m_cache;                    // Glyph cache
    CSpriteSurface* m_surface;               // Destination surface
    std::vector<TextRenderOp> m_batch;       // Render operations
};

#endif // __CTEXTSPRITEBATCH_H__
