#ifndef __CTEXTLAYOUT_H__
#define __CTEXTLAYOUT_H__

#include "CGlyphCache.h"
#include <vector>

/**
 * GlyphPosition - Position of a single glyph in the layout
 */
struct GlyphPosition {
    int x, y;                  // Screen coordinates (relative to layout origin)
    const GlyphInfo* glyph;   // Pointer to cached glyph info
};

/**
 * CTextLayout - Text layout engine
 *
 * Calculates character positions for rendering.
 * Handles UTF-8 parsing and multi-line text (future).
 */
class CTextLayout {
public:
    CTextLayout();
    ~CTextLayout();

    /**
     * Update layout based on text and glyph cache
     * @param text UTF-8 text string
     * @param cache Glyph cache for font metrics
     * @param maxWidth Maximum width for word wrapping (0 = no wrap)
     */
    void Update(const char* text, CGlyphCache* cache, int maxWidth = 0);

    /**
     * Get all glyph positions
     * @return Vector of glyph positions
     */
    const std::vector<GlyphPosition>& GetGlyphs() const { return m_positions; }

    /**
     * Get total layout width
     * @return Width in pixels
     */
    int GetWidth() const { return m_width; }

    /**
     * Get total layout height
     * @return Height in pixels (including all lines)
     */
    int GetHeight() const { return m_height; }

    /**
     * Get cursor position for a character index
     * @param charIndex Character index (in UTF-8 characters)
     * @param x Output X coordinate
     * @param y Output Y coordinate
     */
    void GetCursorPos(int charIndex, int* x, int* y) const;

    /**
     * Get character index at screen position
     * @param x Screen X coordinate
     * @param y Screen Y coordinate
     * @return Character index
     */
    int GetCharIndexAt(int x, int y) const;

private:
    std::vector<GlyphPosition> m_positions;  // Cached glyph positions
    int m_width;                             // Layout width
    int m_height;                            // Layout height
};

#endif // __CTEXTLAYOUT_H__
