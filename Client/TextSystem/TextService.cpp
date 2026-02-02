#include "TextService.h"

#include <algorithm>
#include <cstring>

#ifdef USE_SDL_BACKEND
#include <SDL.h>
#endif

#include "SpriteLib/CSpriteSurface.h"
#include "RenderTargetSpriteSurface.h"

// Global surface pointer from Client
extern CSpriteSurface* g_pLast;

namespace TextSystem {

// Forward declaration of SDL backend factory
TextBackend* CreateSDLTextBackend();

static bool IsValidUtf8(const char* data, size_t len)
{
	size_t i = 0;
	while (i < len) {
		unsigned char c = static_cast<unsigned char>(data[i]);
		if (c < 0x80) {
			++i;
			continue;
		}

		int needed = 0;
		if ((c & 0xE0) == 0xC0) {
			if (c < 0xC2)
				return false;
			needed = 1;
		} else if ((c & 0xF0) == 0xE0) {
			needed = 2;
		} else if ((c & 0xF8) == 0xF0) {
			if (c > 0xF4)
				return false;
			needed = 3;
		} else {
			return false;
		}

		if (i + needed >= len)
			return false;
		for (int j = 1; j <= needed; ++j) {
			if ((static_cast<unsigned char>(data[i + j]) & 0xC0) != 0x80)
				return false;
		}
		i += static_cast<size_t>(needed) + 1;
	}
	return true;
}

static std::string ConvertEncoding(const std::string& input, const char* fromEncoding)
{
#ifdef USE_SDL_BACKEND
	if (input.empty())
		return input;

	SDL_iconv_t cd = SDL_iconv_open("UTF-8", fromEncoding);
	if (cd == reinterpret_cast<SDL_iconv_t>(-1))
		return std::string();

	size_t inBytes = input.size();
	const char* inBuf = input.data();
	size_t outBytes = input.size() * 4 + 4;
	std::string output;
	output.resize(outBytes);
	char* outBuf = &output[0];

	size_t res = SDL_iconv(cd, &inBuf, &inBytes, &outBuf, &outBytes);
	SDL_iconv_close(cd);

	if (res == static_cast<size_t>(-1))
		return std::string();

	output.resize(output.size() - outBytes);
	return output;
#else
	(void)input;
	(void)fromEncoding;
	return std::string();
#endif
}

// Public static method for encoding normalization
std::string TextService::NormalizeText(const std::string& text)
{
	if (text.empty())
		return text;

	if (IsValidUtf8(text.c_str(), text.size()))
		return text;

	// Try common encodings: Korean first, then Chinese, then fallback
	const char* encodings[] = {"CP949", "EUC-KR", "GBK", "GB2312", "BIG5", NULL};
	for (int i = 0; encodings[i] != NULL; ++i) {
		std::string converted = ConvertEncoding(text, encodings[i]);
		if (!converted.empty())
			return converted;
	}

	return text;
}

static uint32_t Utf8Decode(const char* s, int maxLen, int* outLen)
{
	if (maxLen < 1) {
		*outLen = 0;
		return 0xFFFD; // Replacement character for empty input
	}

	unsigned char c = static_cast<unsigned char>(*s);

	// ASCII (0x00-0x7F)
	if (c < 0x80) {
		*outLen = 1;
		return c;
	}

	// 2-byte sequence (0xC0-0xDF)
	if ((c >> 5) == 0x6) {
		if (maxLen < 2) {
			*outLen = 1;
			return 0xFFFD; // Truncated sequence
		}
		*outLen = 2;
		return ((c & 0x1F) << 6) | (static_cast<unsigned char>(s[1]) & 0x3F);
	}

	// 3-byte sequence (0xE0-0xEF)
	if ((c >> 4) == 0xE) {
		if (maxLen < 3) {
			*outLen = 1;
			return 0xFFFD; // Truncated sequence
		}
		*outLen = 3;
		return ((c & 0x0F) << 12) |
			((static_cast<unsigned char>(s[1]) & 0x3F) << 6) |
			(static_cast<unsigned char>(s[2]) & 0x3F);
	}

	// 4-byte sequence (0xF0-0xF7)
	if ((c >> 3) == 0x1E) {
		if (maxLen < 4) {
			*outLen = 1;
			return 0xFFFD; // Truncated sequence
		}
		*outLen = 4;
		return ((c & 0x07) << 18) |
			((static_cast<unsigned char>(s[1]) & 0x3F) << 12) |
			((static_cast<unsigned char>(s[2]) & 0x3F) << 6) |
			(static_cast<unsigned char>(s[3]) & 0x3F);
	}

	// Invalid UTF-8 lead byte
	*outLen = 1;
	return 0xFFFD;
}

TextService::TextService()
	: m_backend(NULL)
	, m_initialized(false)
	, m_defaultFont()
{
}

TextService::~TextService()
{
	delete m_backend;
	m_backend = NULL;
}

TextService& TextService::Get()
{
	static TextService s_instance;
	return s_instance;
}

bool TextService::EnsureInitialized()
{
	if (m_initialized)
		return true;

	m_backend = CreateSDLTextBackend();
	if (!m_backend)
		return false;

	if (!m_backend->Initialize())
		return false;

	FontDesc defaultDesc;
	defaultDesc.size = 16;
	m_defaultFont = m_backend->AcquireFont(defaultDesc);
	m_initialized = m_defaultFont.IsValid();
	return m_initialized;
}

FontHandle TextService::GetDefaultFont()
{
	EnsureInitialized();
	return m_defaultFont;
}

FontHandle TextService::GetFont(int size)
{
	if (!EnsureInitialized())
		return FontHandle();

	FontDesc desc;
	desc.size = size;
	FontHandle handle = m_backend->AcquireFont(desc);
	if (!handle.IsValid())
		return m_defaultFont;
	return handle;
}

TextStyle TextService::GetDefaultStyle()
{
	TextStyle style;
	style.font = GetDefaultFont();
	style.align = TextAlign::Left;
	style.lineSpacing = 0;
	style.color = ColorFromRGB(0xFFFFFF);
	return style;
}

int TextService::GetLineHeight(const TextStyle& style) const
{
	if (!m_initialized || !m_backend)
		return 16;
	return m_backend->GetLineHeight(style.font);
}

int TextService::MeasureLineWidth(const std::string& text, FontHandle font)
{
	if (!EnsureInitialized())
		return 0;

	int width = 0;
	const char* p = text.c_str();
	int remaining = static_cast<int>(text.size());
	while (*p && remaining > 0) {
		int len = 0;
		uint32_t codepoint = Utf8Decode(p, remaining, &len);
		if (len == 0) break; // Safety: no data left
		p += len;
		remaining -= len;

		if (codepoint == '\n')
			break;

		GlyphMetrics metrics;
		if (m_backend->GetGlyphMetrics(font, codepoint, metrics)) {
			width += metrics.advance;
		}
	}
	return width;
}

Metrics TextService::MeasureText(const std::string& text, const TextStyle& style, int maxWidth)
{
	Metrics out = {0, 0, GetLineHeight(style)};
	std::vector<std::string> lines = WrapText(text, style, maxWidth);

	int maxLineWidth = 0;
	for (size_t i = 0; i < lines.size(); ++i) {
		int w = MeasureLineWidth(lines[i], style.font);
		if (w > maxLineWidth)
			maxLineWidth = w;
	}

	out.width = maxLineWidth;
	if (!lines.empty()) {
		out.height = static_cast<int>(lines.size()) * (out.lineHeight + style.lineSpacing) - style.lineSpacing;
	}
	return out;
}

std::vector<std::string> TextService::WrapText(const std::string& text, const TextStyle& style, int maxWidth)
{
	std::string normalized = NormalizeText(text);

	std::vector<std::string> lines;
	if (!EnsureInitialized()) {
		lines.push_back(normalized);
		return lines;
	}

	std::string line;
	int lineWidth = 0;
	int lastBreakIndex = -1;
	int lastBreakSkip = 0;

	const char* p = normalized.c_str();
	int remaining = static_cast<int>(normalized.size());
	while (*p && remaining > 0) {
		int len = 0;
		uint32_t codepoint = Utf8Decode(p, remaining, &len);
		if (len == 0) break; // Safety: no data left

		if (codepoint == '\n') {
			lines.push_back(line);
			line.clear();
			lineWidth = 0;
			lastBreakIndex = -1;
			lastBreakSkip = 0;
			p += len;
			remaining -= len;
			continue;
		}

		GlyphMetrics metrics;
		if (!m_backend->GetGlyphMetrics(style.font, codepoint, metrics)) {
			p += len;
			remaining -= len;
			continue;
		}

		if (codepoint == ' ') {
			lastBreakIndex = static_cast<int>(line.size());
			lastBreakSkip = len;
		}

		if (maxWidth > 0 && lineWidth + metrics.advance > maxWidth && !line.empty()) {
			if (lastBreakIndex >= 0) {
				lines.push_back(line.substr(0, lastBreakIndex));
				line = line.substr(lastBreakIndex + lastBreakSkip);
				lineWidth = MeasureLineWidth(line, style.font);
				lastBreakIndex = -1;
				lastBreakSkip = 0;
			} else {
				lines.push_back(line);
				line.clear();
				lineWidth = 0;
			}
		}

		line.append(p, len);
		lineWidth += metrics.advance;
		p += len;
		remaining -= len;
	}

	if (!line.empty())
		lines.push_back(line);

	if (lines.empty())
		lines.push_back(std::string());

	return lines;
}

void TextService::DrawLine(RenderTarget& target, const std::string& text,
					 int x, int y, int maxWidth, const TextStyle& style)
{
	if (!EnsureInitialized())
		return;

	std::string normalized = NormalizeText(text);
	int lineWidth = MeasureLineWidth(normalized, style.font);
	int drawX = x;
	if (style.align == TextAlign::Center) {
		drawX = x + (maxWidth - lineWidth) / 2;
	} else if (style.align == TextAlign::Right) {
		drawX = x + (maxWidth - lineWidth);
	}

	const char* p = normalized.c_str();
	int remaining = static_cast<int>(normalized.size());
	int penX = drawX;
	int ascent = m_backend->GetFontAscent(style.font);

	while (*p && remaining > 0) {
		int len = 0;
		uint32_t codepoint = Utf8Decode(p, remaining, &len);
		if (len == 0) break; // Safety: no data left
		p += len;
		remaining -= len;

		GlyphMetrics metrics;
		if (!m_backend->GetGlyphMetrics(style.font, codepoint, metrics))
			continue;

		const Glyph* glyph = m_backend->GetGlyph(style.font, codepoint, style.color);
		if (glyph) {
			// Calculate draw position
			// y is the baseline position
			// bearingY is the distance from baseline to the top of the glyph
			// The rendered glyph surface starts at (baseline - ascent - miny)
			// So we need to offset by: y - (ascent + miny - bearingY)
			// But since bearingY = ascent + miny, this simplifies to: y
			//
			// Actually, TTF_RenderUTF8_Blended returns a surface that:
			// - Has origin (0,0) at the glyph's bounding box top-left
			// - The baseline is at position (-miny) within the surface
			//
			// So if we want to draw at baseline position y:
			// - We need to offset the surface so the baseline aligns
			// - drawY = y - (-miny) = y + miny
			// - But miny is negative, so: drawY = y - ascent + bearingY

			int drawY = y - ascent + metrics.bearingY;
			m_backend->DrawGlyph(target, *glyph, penX + metrics.bearingX, drawY, style.color.a);
		}

		penX += metrics.advance;
	}
}

void TextService::DrawLines(RenderTarget& target, const std::vector<std::string>& lines,
					   int x, int y, int maxWidth, int maxLines, int startLine,
					   const TextStyle& style, int lineHeight)
{
	if (!EnsureInitialized())
		return;

	int count = static_cast<int>(lines.size());
	if (startLine < 0)
		startLine = 0;
	if (startLine >= count)
		return;

	int endLine = count;
	if (maxLines > 0)
		endLine = (std::min)(count, startLine + maxLines);

	int drawY = y;
	for (int i = startLine; i < endLine; ++i) {
		DrawLine(target, lines[i], x, drawY, maxWidth, style);
		drawY += lineHeight + style.lineSpacing;
	}
}

void TextService::RenderText(int x, int y, const std::string& text)
{
	// Simple text rendering API for compatibility with SDL_RenderText
	// Renders white text at the specified position using the global surface

	auto& service = Get();

	// Get global surface reference (from SpriteLib)
	if (!::g_pLast || !::g_pLast->GetBackendSurface())
		return;

	// Create render target from global surface
	SpriteSurfaceRenderTarget target(::g_pLast);

	// Use default style with white color
	TextStyle style = service.GetDefaultStyle();
	// Override color to white
	style.color.r = 255;
	style.color.g = 255;
	style.color.b = 255;
	style.color.a = 255;

	// Draw the text
	service.DrawLine(target, text, x, y, 0, style);
}

} // namespace TextSystem
