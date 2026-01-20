#ifndef __CGLYPHCACHE_H__
#define __CGLYPHCACHE_H__

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include "../SpriteLib/SpriteLibBackend.h"

// Glyph information (cached metrics + sprite)
struct GlyphInfo {
    spritectl_sprite_t sprite;  // Sprite handle for rendering
    int width, height;          // Glyph dimensions
    int advance_x;              // X advance (spacing to next character)
    int advance_y;              // Y advance (usually 0 for horizontal text)
    int bearing_x;              // Left bearing (x offset from cursor)
    int bearing_y;              // Top bearing (y offset from baseline)
    uint32_t charcode;          // UTF-32 character code
};

/**
 * CGlyphCache - Glyph caching system for spritectl backend
 *
 * Caches glyphs as spritectl_sprite_t for efficient blitting.
 * Supports dynamic character loading (CJK, emoji, etc.).
 */
class CGlyphCache {
public:
    CGlyphCache();
    ~CGlyphCache();

    /**
     * Initialize the glyph cache
     * @param fontPath Path to font file (TTF/OTF/TTC)
     * @param fontSize Font size in points
     * @return true on success
     */
    bool Init(const char* fontPath, int fontSize);

    /**
     * Release all resources
     */
    void Release();

    /**
     * Get glyph information for a character
     * If character not cached, it will be rendered and cached.
     * @param charcode UTF-32 character code
     * @return Pointer to GlyphInfo, or NULL on failure
     */
    const GlyphInfo* GetGlyph(uint32_t charcode);

    /**
     * Check if character is already cached
     * @param charcode UTF-32 character code
     * @return true if cached
     */
    bool HasGlyph(uint32_t charcode) const;

    // Font metrics
    int GetFontHeight() const { return m_fontHeight; }
    int GetFontAscent() const { return m_fontAscent; }
    int GetFontDescent() const { return m_fontDescent; }

private:
    /**
     * Add a character to the cache
     * Renders the character and creates a sprite.
     * @param charcode UTF-32 character code
     * @return Pointer to GlyphInfo, or NULL on failure
     */
    const GlyphInfo* AddGlyph(uint32_t charcode);

    // Font
    TTF_Font* m_font;

    // Glyph cache (charcode → GlyphInfo)
    std::unordered_map<uint32_t, GlyphInfo> m_glyphs;

    // Font metrics
    int m_fontHeight;
    int m_fontAscent;
    int m_fontDescent;
};

#endif // __CGLYPHCACHE_H__
