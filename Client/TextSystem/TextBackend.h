#ifndef TEXTSYSTEM_TEXTBACKEND_H
#define TEXTSYSTEM_TEXTBACKEND_H

#include "TextTypes.h"
#include "RenderTarget.h"

namespace TextSystem {

struct Glyph {
	GlyphMetrics metrics;
	void* handle;
};

class TextBackend {
public:
	virtual ~TextBackend() {}
	virtual bool Initialize() = 0;
	virtual FontHandle AcquireFont(const FontDesc& desc) = 0;
	virtual int GetLineHeight(FontHandle font) const = 0;
	virtual int GetFontAscent(FontHandle font) const = 0;
	virtual bool GetGlyphMetrics(FontHandle font, uint32_t codepoint, GlyphMetrics& outMetrics) = 0;
	virtual const Glyph* GetGlyph(FontHandle font, uint32_t codepoint, const Color& color) = 0;
	virtual void DrawGlyph(RenderTarget& target, const Glyph& glyph, int x, int y, uint8_t alpha) = 0;
};

} // namespace TextSystem

#endif // TEXTSYSTEM_TEXTBACKEND_H
