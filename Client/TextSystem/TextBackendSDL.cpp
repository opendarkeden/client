#include "TextBackend.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <stdint.h>
#include <stdio.h>

#include "SpriteLib/SpriteLibBackend.h"

#ifdef USE_SDL_BACKEND
#include <SDL.h>
#include <SDL_ttf.h>
#endif

namespace TextSystem {

struct GlyphKey {
	int fontId;
	uint32_t codepoint;
	uint32_t color;

	bool operator==(const GlyphKey& other) const
	{
		return fontId == other.fontId && codepoint == other.codepoint && color == other.color;
	}
};

struct GlyphKeyHash {
	size_t operator()(const GlyphKey& key) const
	{
		return (static_cast<size_t>(key.fontId) * 1315423911u) ^
			(static_cast<size_t>(key.codepoint) * 2654435761u) ^
			(static_cast<size_t>(key.color) * 97531u);
	}
};

class TextBackendSDL : public TextBackend {
public:
	TextBackendSDL()
		: m_initialized(false)
	{}

	~TextBackendSDL() override
	{
		for (auto& it : m_glyphs) {
			Glyph& g = it.second;
			if (g.handle) {
				spritectl_destroy_sprite(reinterpret_cast<spritectl_sprite_t>(g.handle));
				g.handle = NULL;
			}
		}
		m_glyphs.clear();

		for (size_t i = 0; i < m_fonts.size(); ++i) {
			if (m_fonts[i]) {
				TTF_CloseFont(m_fonts[i]);
			}
		}
		m_fonts.clear();
	}

	bool Initialize() override
	{
#ifdef USE_SDL_BACKEND
		if (TTF_WasInit() == 0) {
			if (TTF_Init() != 0) {
				fprintf(stderr, "TextBackendSDL: TTF_Init failed: %s\n", TTF_GetError());
				return false;
			}
		}
#endif
		m_initialized = true;
		return true;
	}

	FontHandle AcquireFont(const FontDesc& desc) override
	{
		if (!m_initialized)
			return FontHandle();

		int size = desc.size > 0 ? desc.size : 16;
		auto it = m_sizeToFontId.find(size);
		if (it != m_sizeToFontId.end()) {
			FontHandle handle;
			handle.id = it->second;
			return handle;
		}

		const char* fontPaths[] = {
			"Data/Font/NotoSansCJK-Regular.ttc",
			"Data/Font/NotoSans-Regular.ttf",
			"Data/Font/DejaVuSans.ttf",
			"Data/Font/Hiragino Sans GB.ttc",
			"/System/Library/Fonts/Helvetica.ttc",
			"/System/Library/Fonts/Hiragino Sans GB.ttc",
			NULL
		};

		TTF_Font* font = NULL;
		for (int i = 0; fontPaths[i] != NULL; ++i) {
			font = TTF_OpenFont(fontPaths[i], size);
			if (font) {
				break;
			}
		}

		if (!font) {
			fprintf(stderr, "TextBackendSDL: Failed to load font size %d\n", size);
			return FontHandle();
		}

		int id = static_cast<int>(m_fonts.size());
		m_fonts.push_back(font);
		m_sizeToFontId[size] = id;

		FontHandle handle;
		handle.id = id;
		return handle;
	}

	int GetLineHeight(FontHandle font) const override
	{
		TTF_Font* ttf = GetFont(font);
		if (!ttf)
			return 16;
		return TTF_FontHeight(ttf);
	}

	int GetFontAscent(FontHandle font) const override
	{
		TTF_Font* ttf = GetFont(font);
		if (!ttf)
			return 12;
		return TTF_FontAscent(ttf);
	}

	bool GetGlyphMetrics(FontHandle font, uint32_t codepoint, GlyphMetrics& outMetrics) override
	{
		TTF_Font* ttf = GetFont(font);
		if (!ttf)
			return false;

		int minx = 0, maxx = 0, miny = 0, maxy = 0, advance = 0;
		bool ok = false;

		if (codepoint <= 0xFFFF) {
			if (TTF_GlyphMetrics(ttf, static_cast<Uint16>(codepoint), &minx, &maxx, &miny, &maxy, &advance) == 0) {
				ok = true;
			}
		}

		if (!ok) {
			// Fallback: approximate metrics using rendered surface
			std::string utf8 = EncodeUtf8(codepoint);
			SDL_Color white = {255, 255, 255, 255};
			SDL_Surface* surf = TTF_RenderUTF8_Blended(ttf, utf8.c_str(), white);
			if (!surf)
				return false;
			minx = 0;
			maxx = surf->w;
			miny = -TTF_FontAscent(ttf);  // Assume top-aligned
			maxy = TTF_FontDescent(ttf);  // Assume baseline at bottom
			advance = surf->w;
			SDL_FreeSurface(surf);
		}

		outMetrics.width = maxx - minx;
		outMetrics.height = maxy - miny;
		outMetrics.advance = advance;
		outMetrics.bearingX = minx;
		// bearingY is the distance from baseline to top of glyph
		// miny is usually negative (distance above baseline)
		// ascent is the distance from baseline to top of font bounding box
		outMetrics.bearingY = TTF_FontAscent(ttf) + miny;
		return true;
	}

	const Glyph* GetGlyph(FontHandle font, uint32_t codepoint, const Color& color) override
	{
		TTF_Font* ttf = GetFont(font);
		if (!ttf)
			return NULL;

		uint32_t packedColor = (static_cast<uint32_t>(color.r) << 16) |
			(static_cast<uint32_t>(color.g) << 8) |
			static_cast<uint32_t>(color.b);

		GlyphKey key;
		key.fontId = font.id;
		key.codepoint = codepoint;
		key.color = packedColor;

		auto it = m_glyphs.find(key);
		if (it != m_glyphs.end()) {
			return &it->second;
		}

		// Get glyph metrics first
		int minx = 0, maxx = 0, miny = 0, maxy = 0, advance = 0;
		bool hasMetrics = false;

		if (codepoint <= 0xFFFF) {
			if (TTF_GlyphMetrics(ttf, static_cast<Uint16>(codepoint), &minx, &maxx, &miny, &maxy, &advance) == 0) {
				hasMetrics = true;
			}
		}

		// Render the glyph
		std::string utf8 = EncodeUtf8(codepoint);
		SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
		SDL_Surface* surface = TTF_RenderUTF8_Blended(ttf, utf8.c_str(), sdlColor);
		if (!surface)
			return NULL;

		SDL_Surface* rgbaSurface = surface;
		if (surface->format->format != SDL_PIXELFORMAT_RGBA32) {
			rgbaSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
			SDL_FreeSurface(surface);
			if (!rgbaSurface)
				return NULL;
		}

		Glyph glyph;
		GlyphMetrics metrics;

		if (hasMetrics) {
			int ascent = TTF_FontAscent(ttf);
			metrics.width = maxx - minx;
			metrics.height = maxy - miny;
			metrics.advance = advance;
			metrics.bearingX = minx;
			// bearingY is distance from baseline to TOP of rendered glyph surface
			// The rendered surface includes the full glyph, so its top is at baseline - miny
			// Therefore bearingY should be: ascent - (surface_top_relative_to_baseline)
			// Since miny is negative (above baseline), surface_top = baseline - miny = baseline + |miny|
			// So bearingY = ascent + miny
			metrics.bearingY = ascent + miny;
		} else {
			// Fallback: approximate metrics from surface
			int ascent = TTF_FontAscent(ttf);
			metrics.width = rgbaSurface->w;
			metrics.height = rgbaSurface->h;
			metrics.advance = rgbaSurface->w;
			metrics.bearingX = 0;
			metrics.bearingY = ascent;  // Assume top-aligned
		}
		glyph.metrics = metrics;

		size_t dataSize = rgbaSurface->pitch * rgbaSurface->h;
		glyph.handle = reinterpret_cast<void*>(spritectl_create_sprite(
			rgbaSurface->w,
			rgbaSurface->h,
			SPRITECTL_FORMAT_RGBA32,
			rgbaSurface->pixels,
			dataSize));

		SDL_FreeSurface(rgbaSurface);

		if (!glyph.handle)
			return NULL;

		m_glyphs.emplace(key, glyph);
		return &m_glyphs.find(key)->second;
	}

	void DrawGlyph(RenderTarget& target, const Glyph& glyph, int x, int y, uint8_t alpha) override
	{
		void* native = target.GetNative(NativeTargetType::SpriteCtlSurface);
		if (!native || !glyph.handle)
			return;

		spritectl_surface_t surface = reinterpret_cast<spritectl_surface_t>(native);
		spritectl_sprite_t sprite = reinterpret_cast<spritectl_sprite_t>(glyph.handle);

		(void)alpha;
		spritectl_blt_sprite(surface, x, y, sprite, SPRITECTL_BLT_NONE, 255);
	}

private:
	TTF_Font* GetFont(FontHandle handle) const
	{
		if (handle.id < 0 || handle.id >= static_cast<int>(m_fonts.size()))
			return NULL;
		return m_fonts[handle.id];
	}

	std::string EncodeUtf8(uint32_t codepoint) const
	{
		char buf[5] = {0};
		if (codepoint < 0x80) {
			buf[0] = static_cast<char>(codepoint);
			buf[1] = 0;
			return std::string(buf);
		}
		if (codepoint < 0x800) {
			buf[0] = static_cast<char>(0xC0 | (codepoint >> 6));
			buf[1] = static_cast<char>(0x80 | (codepoint & 0x3F));
			buf[2] = 0;
			return std::string(buf);
		}
		if (codepoint < 0x10000) {
			buf[0] = static_cast<char>(0xE0 | (codepoint >> 12));
			buf[1] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
			buf[2] = static_cast<char>(0x80 | (codepoint & 0x3F));
			buf[3] = 0;
			return std::string(buf);
		}
		buf[0] = static_cast<char>(0xF0 | (codepoint >> 18));
		buf[1] = static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
		buf[2] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
		buf[3] = static_cast<char>(0x80 | (codepoint & 0x3F));
		buf[4] = 0;
		return std::string(buf);
	}

private:
	bool m_initialized;
	std::vector<TTF_Font*> m_fonts;
	std::unordered_map<int, int> m_sizeToFontId;
	std::unordered_map<GlyphKey, Glyph, GlyphKeyHash> m_glyphs;
};

TextBackend* CreateSDLTextBackend()
{
	return new TextBackendSDL();
}

} // namespace TextSystem
