#ifndef TEXTSYSTEM_TEXTSERVICE_H
#define TEXTSYSTEM_TEXTSERVICE_H

#include <string>
#include <vector>

#include "TextBackend.h"

namespace TextSystem {

class TextService {
public:
	static TextService& Get();

	FontHandle GetDefaultFont();
	FontHandle GetFont(int size);
	TextStyle GetDefaultStyle();

	Metrics MeasureText(const std::string& text, const TextStyle& style, int maxWidth = 0);
	std::vector<std::string> WrapText(const std::string& text, const TextStyle& style, int maxWidth);

	void DrawLine(RenderTarget& target, const std::string& text,
				 int x, int y, int maxWidth, const TextStyle& style);

	void DrawLines(RenderTarget& target, const std::vector<std::string>& lines,
				  int x, int y, int maxWidth, int maxLines, int startLine,
				  const TextStyle& style, int lineHeight);

	int GetLineHeight(const TextStyle& style) const;

	// Simple text rendering API (compatibility layer for SDL_RenderText)
	// Renders white text at the specified position
	static void RenderText(int x, int y, const std::string& text);

	// Encoding normalization: converts CP949/EUC-KR/other encodings to UTF-8
	static std::string NormalizeText(const std::string& text);

private:
	TextService();
	~TextService();
	TextService(const TextService&) = delete;
	TextService& operator=(const TextService&) = delete;

	bool EnsureInitialized();
	int MeasureLineWidth(const std::string& text, FontHandle font);

private:
	TextBackend* m_backend;
	bool m_initialized;
	FontHandle m_defaultFont;
};

} // namespace TextSystem

#endif // TEXTSYSTEM_TEXTSERVICE_H
