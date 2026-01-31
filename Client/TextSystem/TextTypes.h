#ifndef TEXTSYSTEM_TEXTTYPES_H
#define TEXTSYSTEM_TEXTTYPES_H

#include <cstdint>
#include <string>

namespace TextSystem {

struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

inline Color ColorFromRGB(uint32_t rgb)
{
	Color c;
	c.r = static_cast<uint8_t>((rgb >> 16) & 0xFF);
	c.g = static_cast<uint8_t>((rgb >> 8) & 0xFF);
	c.b = static_cast<uint8_t>(rgb & 0xFF);
	c.a = 255;
	return c;
}

inline Color ColorFromCOLORREF(uint32_t cref)
{
	Color c;
	c.r = static_cast<uint8_t>(cref & 0xFF);
	c.g = static_cast<uint8_t>((cref >> 8) & 0xFF);
	c.b = static_cast<uint8_t>((cref >> 16) & 0xFF);
	c.a = 255;
	return c;
}

struct Rect {
	int x;
	int y;
	int w;
	int h;
};

enum class TextAlign {
	Left,
	Center,
	Right
};

struct FontDesc {
	std::string family;
	int size;
	int weight;
	bool italic;

	FontDesc()
		: family()
		, size(16)
		, weight(400)
		, italic(false)
	{}
};

struct FontHandle {
	int id;
	FontHandle() : id(-1) {}
	bool IsValid() const { return id >= 0; }
};

struct TextStyle {
	FontHandle font;
	Color color;
	TextAlign align;
	int lineSpacing;

	TextStyle()
		: font()
		, color(ColorFromRGB(0xFFFFFF))
		, align(TextAlign::Left)
		, lineSpacing(0)
	{}
};

struct GlyphMetrics {
	int width;
	int height;
	int advance;
	int bearingX;
	int bearingY;
};

struct Metrics {
	int width;
	int height;
	int lineHeight;
};

} // namespace TextSystem

#endif // TEXTSYSTEM_TEXTTYPES_H
