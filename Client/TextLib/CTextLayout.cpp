#include "CTextLayout.h"
#include <stdio.h>
#include <string.h>

CTextLayout::CTextLayout()
    : m_width(0)
    , m_height(0)
{
}

CTextLayout::~CTextLayout() {
}

void CTextLayout::Update(const char* text, CGlyphCache* cache, int maxWidth) {
    m_positions.clear();

    printf("DEBUG CTextLayout::Update: text='%s', cache=%p, maxWidth=%d\n",
           text ? text : "(null)", cache, maxWidth);

    if (!text || !cache) {
        m_width = 0;
        m_height = 0;
        printf("DEBUG CTextLayout::Update: Early return (text=%p, cache=%p)\n", text, cache);
        return;
    }

    int x = 0;
    int y = 0;
    int lineHeight = cache->GetFontHeight();

    // Parse UTF-8 and calculate glyph positions
    const unsigned char* p = (const unsigned char*)text;
    int charCount = 0;
    while (*p) {
        uint32_t charcode = 0;
        int bytes = 0;

        // UTF-8 decode
        if ((*p & 0x80) == 0) {
            // 1 byte: 0xxxxxxx
            charcode = *p;
            bytes = 1;
        } else if ((*p & 0xE0) == 0xC0) {
            // 2 bytes: 110xxxxx 10xxxxxx
            if (*(p + 1) && (*(p + 1) & 0xC0) == 0x80) {
                charcode = ((*p & 0x1F) << 6) | (*(p + 1) & 0x3F);
                bytes = 2;
            } else {
                p++;  // Invalid UTF-8
                continue;
            }
        } else if ((*p & 0xF0) == 0xE0) {
            // 3 bytes: 1110xxxx 10xxxxxx 10xxxxxx
            if (*(p + 1) && *(p + 2) &&
                (*(p + 1) & 0xC0) == 0x80 && (*(p + 2) & 0xC0) == 0x80) {
                charcode = ((*p & 0x0F) << 12) |
                          ((*(p + 1) & 0x3F) << 6) |
                          (*(p + 2) & 0x3F);
                bytes = 3;
            } else {
                p++;  // Invalid UTF-8
                continue;
            }
        } else if ((*p & 0xF8) == 0xF0) {
            // 4 bytes: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            if (*(p + 1) && *(p + 2) && *(p + 3) &&
                (*(p + 1) & 0xC0) == 0x80 &&
                (*(p + 2) & 0xC0) == 0x80 &&
                (*(p + 3) & 0xC0) == 0x80) {
                charcode = ((*p & 0x07) << 18) |
                          ((*(p + 1) & 0x3F) << 12) |
                          ((*(p + 2) & 0x3F) << 6) |
                          (*(p + 3) & 0x3F);
                bytes = 4;
            } else {
                p++;  // Invalid UTF-8
                continue;
            }
        } else {
            p++;  // Invalid UTF-8
            continue;
        }

        // Get glyph from cache
        const GlyphInfo* glyph = cache->GetGlyph(charcode);
        if (glyph) {
            GlyphPosition pos;
            pos.x = x + glyph->bearing_x;
            pos.y = y - glyph->bearing_y;
            pos.glyph = glyph;
            m_positions.push_back(pos);

            x += glyph->advance_x;
            charCount++;
        } else {
            printf("DEBUG CTextLayout::Update: Failed to get glyph for charcode 0x%X\n", charcode);
        }

        p += bytes;

        // Word wrapping (if maxWidth specified)
        if (maxWidth > 0 && x >= maxWidth) {
            x = 0;
            y += lineHeight;
        }
    }

    m_width = x;
    m_height = y + lineHeight;

    printf("DEBUG CTextLayout::Update: Added %d glyphs, m_positions.size()=%zu\n",
           charCount, m_positions.size());
}

void CTextLayout::GetCursorPos(int charIndex, int* x, int* y) const {
    if (x) *x = 0;
    if (y) *y = 0;

    if (charIndex < 0 || charIndex >= (int)m_positions.size()) {
        return;
    }

    if (x) *x = m_positions[charIndex].x;
    if (y) *y = m_positions[charIndex].y;
}

int CTextLayout::GetCharIndexAt(int x, int y) const {
    // Find closest character index to given screen position
    int bestIndex = 0;
    int bestDist = 0x7FFFFFFF;

    for (size_t i = 0; i < m_positions.size(); i++) {
        const GlyphPosition& pos = m_positions[i];

        int dx = pos.x - x;
        int dy = pos.y - y;
        int dist = dx * dx + dy * dy;

        if (dist < bestDist) {
            bestDist = dist;
            bestIndex = (int)i;
        }
    }

    return bestIndex;
}
