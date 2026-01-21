#include "CGlyphCache.h"
#include "DebugLog.h"
#include <stdio.h>
#include <string.h>

CGlyphCache::CGlyphCache()
    : m_font(NULL)
    , m_fontHeight(0)
    , m_fontAscent(0)
    , m_fontDescent(0)
{
}

CGlyphCache::~CGlyphCache() {
    Release();
}

bool CGlyphCache::Init(const char* fontPath, int fontSize) {
    // Load font
    m_font = TTF_OpenFont(fontPath, fontSize);
    if (!m_font) {
        fprintf(stderr, "CGlyphCache::Init: Failed to load font %s: %s\n",
                fontPath, TTF_GetError());
        return false;
    }

    // Get font metrics
    m_fontHeight = TTF_FontHeight(m_font);
    m_fontAscent = TTF_FontAscent(m_font);
    m_fontDescent = TTF_FontDescent(m_font);

    printf("CGlyphCache::Init: Loaded font %s (size %d, height %d, ascent %d, descent %d)\n",
           fontPath, fontSize, m_fontHeight, m_fontAscent, m_fontDescent);

    return true;
}

void CGlyphCache::Release() {
    // Destroy all cached sprites
    for (auto& pair : m_glyphs) {
        if (pair.second.sprite != SPRITECTL_INVALID_SPRITE) {
            spritectl_destroy_sprite(pair.second.sprite);
        }
    }
    m_glyphs.clear();

    // Close font
    if (m_font) {
        TTF_CloseFont(m_font);
        m_font = NULL;
    }
}

const GlyphInfo* CGlyphCache::GetGlyph(uint32_t charcode) {
    auto it = m_glyphs.find(charcode);
    if (it != m_glyphs.end()) {
        return &it->second;
    }

    // Not cached, add it
    return AddGlyph(charcode);
}

bool CGlyphCache::HasGlyph(uint32_t charcode) const {
    return m_glyphs.find(charcode) != m_glyphs.end();
}

const GlyphInfo* CGlyphCache::AddGlyph(uint32_t charcode) {
    // Convert UTF-32 to UTF-8
    char text[5];
    int len = 0;

    if (charcode < 0x80) {
        text[0] = (char)charcode;
        text[1] = '\0';
        len = 1;
    } else if (charcode < 0x800) {
        text[0] = 0xC0 | (charcode >> 6);
        text[1] = 0x80 | (charcode & 0x3F);
        text[2] = '\0';
        len = 2;
    } else if (charcode < 0x10000) {
        text[0] = 0xE0 | (charcode >> 12);
        text[1] = 0x80 | ((charcode >> 6) & 0x3F);
        text[2] = 0x80 | (charcode & 0x3F);
        text[3] = '\0';
        len = 3;
    } else {
        text[0] = 0xF0 | (charcode >> 18);
        text[1] = 0x80 | ((charcode >> 12) & 0x3F);
        text[2] = 0x80 | ((charcode >> 6) & 0x3F);
        text[3] = 0x80 | (charcode & 0x3F);
        text[4] = '\0';
        len = 4;
    }

    // Render glyph to SDL surface (RGBA32 format)
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(m_font, text, white);
    if (!surface) {
        fprintf(stderr, "CGlyphCache::AddGlyph: Failed to render charcode 0x%X: %s\n",
                charcode, TTF_GetError());
        return NULL;
    }

    // Convert surface to RGBA32 format if needed
    SDL_Surface* rgbaSurface = surface;
    if (surface->format->format != SDL_PIXELFORMAT_RGBA32) {
        rgbaSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(surface);
        if (!rgbaSurface) {
            fprintf(stderr, "CGlyphCache::AddGlyph: Failed to convert to RGBA32: %s\n",
                    SDL_GetError());
            return NULL;
        }
    }

    // Get glyph metrics
    int minx, maxx, miny, maxy, advance;
    Uint16 ch = (charcode <= 0xFFFF) ? (Uint16)charcode : 0;
    if (ch && TTF_GlyphMetrics(m_font, ch, &minx, &maxx, &miny, &maxy, &advance) < 0) {
        // Fallback if TTF_GlyphMetrics fails
        minx = 0;
        maxx = rgbaSurface->w;
        advance = rgbaSurface->w;
    }

    // Create GlyphInfo
    GlyphInfo info;
    info.charcode = charcode;
    info.width = rgbaSurface->w;
    info.height = rgbaSurface->h;
    info.advance_x = advance;
    info.advance_y = 0;
    info.bearing_x = minx;
    info.bearing_y = maxy;
    info.sprite = SPRITECTL_INVALID_SPRITE;

    // Create spritectl sprite from surface pixels
    size_t data_size = rgbaSurface->pitch * rgbaSurface->h;
    info.sprite = spritectl_create_sprite(
        rgbaSurface->w,
        rgbaSurface->h,
        SPRITECTL_FORMAT_RGBA32,
        rgbaSurface->pixels,
        data_size
    );

    SDL_FreeSurface(rgbaSurface);

    if (info.sprite == SPRITECTL_INVALID_SPRITE) {
        LOG_ERROR("CGlyphCache::AddGlyph: Failed to create sprite for charcode 0x%X\n", charcode);
        return NULL;
    }

    LOG_DEBUG("CGlyphCache::AddGlyph: Created sprite for U+%04X (%c), size=%dx%d, sprite=%p\n",
           charcode,
           (charcode >= 32 && charcode < 127) ? (char)charcode : '?',
           info.width, info.height, info.sprite);

    // Cache the glyph
    m_glyphs[charcode] = info;

    return &m_glyphs[charcode];
}
