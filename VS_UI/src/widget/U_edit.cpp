#include "U_edit.h"
#include "../hangul/CI.h"
#include "../InputFocusManager.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef PLATFORM_MACOS
#include <SDL2/SDL.h>
#include "../../../Client/TextSystem/TextService.h"
#include "../../../Client/TextSystem/RenderTargetSpriteSurface.h"
#include "../../../Client/SpriteLib/CSpriteSurface.h"
#endif

// Forward declare FL2 functions (defined in hangul/FL2.cpp)
extern void g_Print(int x, int y, const char* sz_str, void* p_print_info);
extern int g_GetStringWidth(const char* sz_str, void* hfont);
extern int g_GetStringHeight(const char* sz_str, void* hfont);

// External reference to global CI (for cursor blink state)
extern CI* gC_ci;

// External reference to SDL renderer
extern SDL_Renderer* g_pSDLRenderer;

// External reference to back buffer surface (for spritectl blt)
#ifdef PLATFORM_MACOS
extern CSpriteSurface* g_pBack;
extern CSpriteSurface* g_pLast;  // UI renders to g_pLast, not g_pBack!
#endif

// ============================================================================
// UTF-8 <-> UTF-32 Conversion (from textbox_demo.c)
// ============================================================================

static int utf8_to_utf32(const char* s, uint32_t* out, int cap) {
	int n = 0;
	while (*s && n < cap) {
		uint32_t c;
		unsigned char b = *s++;

		if (b < 0x80) {
			c = b;
		} else if ((b >> 5) == 0x6) {
			c = ((b & 0x1F) << 6) | (*s++ & 0x3F);
		} else if ((b >> 4) == 0xE) {
			c = ((b & 0x0F) << 12) |
			    ((*s++ & 0x3F) << 6) |
			    (*s++ & 0x3F);
		} else if ((b >> 3) == 0x1E) {
			c = ((b & 0x07) << 18) |
			    ((*s++ & 0x3F) << 12) |
			    ((*s++ & 0x3F) << 6) |
			    (*s++ & 0x3F);
		} else {
			continue;  // Invalid UTF-8
		}
		out[n++] = c;
	}
	return n;
}

static int utf32_to_utf8(uint32_t c, char out[5]) {
	if (c < 0x80) {
		out[0] = c;
		out[1] = 0;
		return 1;
	} else if (c < 0x800) {
		out[0] = 0xC0 | (c >> 6);
		out[1] = 0x80 | (c & 0x3F);
		out[2] = 0;
		return 2;
	} else if (c < 0x10000) {
		out[0] = 0xE0 | (c >> 12);
		out[1] = 0x80 | ((c >> 6) & 0x3F);
		out[2] = 0x80 | (c & 0x3F);
		out[3] = 0;
		return 3;
	} else {
		out[0] = 0xF0 | (c >> 18);
		out[1] = 0x80 | ((c >> 12) & 0x3F);
		out[2] = 0x80 | ((c >> 6) & 0x3F);
		out[3] = 0x80 | (c & 0x3F);
		out[4] = 0;
		return 4;
	}
}

// ============================================================================
// LineEditor implementation
// ============================================================================

LineEditor::LineEditor()
{
	memset(m_Text, 0, sizeof(m_Text));
	m_CursorPos = 0;
	m_TextLen = 0;
	m_Limit = MAX_TEXT - 1;
	m_bAcquired = false;
	memset(m_Composing, 0, sizeof(m_Composing));
	m_ComposingLen = 0;
}

void LineEditor::Acquire()
{
	m_bAcquired = true;
}

void LineEditor::Unacquire()
{
	m_bAcquired = false;
}

bool LineEditor::IsAcquire() const
{
	return m_bAcquired;
}

// Insert UTF-32 text at cursor position
void LineEditor::InsertText(const uint32_t* text, int len)
{
	if (len <= 0) return;
	if (m_TextLen + len > m_Limit) return;

	// Move existing text to make room
	memmove(&m_Text[m_CursorPos + len],
	        &m_Text[m_CursorPos],
	        (m_TextLen - m_CursorPos) * sizeof(uint32_t));

	// Insert new text
	memcpy(&m_Text[m_CursorPos], text, len * sizeof(uint32_t));

	m_CursorPos += len;
	m_TextLen += len;
}

// Insert single UTF-32 character
void LineEditor::InsertChar(uint32_t c)
{
	InsertText(&c, 1);
}

// Delete character at offset
void LineEditor::DeleteChar(int offset)
{
	if (offset < 0 || offset >= m_TextLen) return;

	memmove(&m_Text[offset],
	        &m_Text[offset + 1],
	        (m_TextLen - offset - 1) * sizeof(uint32_t));

	m_TextLen--;

	// Adjust cursor position if it was after the deleted character
	if (m_CursorPos > offset) {
		m_CursorPos--;
	}
	// Also ensure cursor doesn't go beyond text length
	if (m_CursorPos > m_TextLen) {
		m_CursorPos = m_TextLen;
	}
}

// Delete character before cursor (backspace)
void LineEditor::Backspace()
{
	if (m_CursorPos > 0) {
		// DeleteChar will handle cursor adjustment
		DeleteChar(m_CursorPos - 1);
	}
}

// Move cursor by delta characters
void LineEditor::MoveCursor(int delta)
{
	int newPos = m_CursorPos + delta;
	if (newPos < 0) newPos = 0;
	if (newPos > m_TextLen) newPos = m_TextLen;
	m_CursorPos = newPos;
}

// Set cursor to absolute position
void LineEditor::SetCursor(int pos)
{
	if (pos < 0) pos = 0;
	if (pos > m_TextLen) pos = m_TextLen;
	m_CursorPos = pos;
}

// Handle SDL_TEXTINPUT event (committed text)
void LineEditor::HandleTextInput(const char* text)
{
	if (text == NULL || text[0] == '\0') return;

	uint32_t utf32[32];
	int len = utf8_to_utf32(text, utf32, 32);

	if (IsComposing()) {
		EndComposition();
	}

	InsertText(utf32, len);
}

// Handle SDL_TEXTEDITING event (IME composition in progress)
void LineEditor::HandleTextEditing(const char* text, int start, int length)
{
	if (length > 0) {
		// Currently composing - store composition text
		m_ComposingLen = utf8_to_utf32(text, m_Composing, MAX_TEXT);
	} else {
		// Composition ended
		EndComposition();
	}
}

// Start IME composition
void LineEditor::StartComposition(const char* text, int start, int length)
{
	HandleTextEditing(text, start, length);
}

// Update IME composition
void LineEditor::UpdateComposition(const char* text, int start, int length)
{
	HandleTextEditing(text, start, length);
}

// End IME composition (commit composed text)
void LineEditor::EndComposition()
{
	if (m_ComposingLen > 0) {
		InsertText(m_Composing, m_ComposingLen);
		m_ComposingLen = 0;
	}
}

// Get text as UTF-8 string (for compatibility)
const char* LineEditor::GetBuffer() const
{
	static char utf8_buffer[MAX_TEXT * 4 + 1];  // Worst case: 4 bytes per UTF-32 char
	int offset = 0;

	for (int i = 0; i < m_TextLen && offset < (int)sizeof(utf8_buffer) - 4; i++) {
		char buf[5];
		int len = utf32_to_utf8(m_Text[i], buf);
		memcpy(&utf8_buffer[offset], buf, len);
		offset += len;
	}
	utf8_buffer[offset] = '\0';

	return utf8_buffer;
}

// Legacy: Add UTF-8 string (converts to UTF-32 internally)
void LineEditor::AddString(const char* pStr)
{
	if (pStr == NULL) return;

	uint32_t utf32[MAX_TEXT];
	int len = utf8_to_utf32(pStr, utf32, MAX_TEXT);

	if (m_TextLen + len <= m_Limit) {
		InsertText(utf32, len);
	}
}

// Legacy: Clear all text
void LineEditor::EraseAll()
{
	m_TextLen = 0;
	m_CursorPos = 0;
	memset(m_Text, 0, sizeof(m_Text));
	m_ComposingLen = 0;
}

// Legacy: Delete character before cursor
void LineEditor::EraseCharacterBegin()
{
	Backspace();
}

// Legacy: Insert special mark
void LineEditor::InsertMark(unsigned short mark)
{
	InsertChar((uint32_t)mark);
}

// KeyboardControl - main entry point for keyboard messages
void LineEditor::KeyboardControl(unsigned int message, unsigned int key, long extra)
{
	switch (message)
	{
	case WM_CHAR:
		// Legacy: Single character input
		InsertChar((uint32_t)key);
		break;

	case WM_TEXTINPUT:
		// SDL_TEXTINPUT event (extra is text pointer)
		{
			const char* text = (const char*)extra;
			HandleTextInput(text);
		}
		break;

	case WM_TEXTEDITING:
		// SDL_TEXTEDITING event (composition)
		// Note: This is a simplified handling - actual implementation would need the text
		// For now, we just clear composition state when we get this message
		m_ComposingLen = 0;
		break;

	case WM_KEYDOWN:
		// Control keys
		switch (key)
		{
		case VK_BACK:
			Backspace();
			break;
		case VK_LEFT:
			MoveCursor(-1);
			break;
		case VK_RIGHT:
			MoveCursor(1);
			break;
		case VK_HOME:
			SetCursor(0);
			break;
		case VK_END:
			SetCursor(m_TextLen);
			break;
		case VK_DELETE:
			DeleteChar(m_CursorPos);
			break;
		}
		break;
	}
}

// ============================================================================
// LineEditorVisual implementation
// ============================================================================

LineEditorVisual::LineEditorVisual()
{
	// Debug: print offset
//	printf("DEBUG LineEditorVisual::LineEditorVisual: this=%p, &m_Editor=%p, &m_Editor.m_CursorPos=%p\n",
//	       this, &m_Editor, &m_Editor.m_CursorPos);

	m_X = 0;
	m_Y = 0;
	m_AbsWidth = 100;
	m_MaxWidth = 100;
	m_bPasswordMode = false;
	m_bAcquired = false;
	m_PrintInfo.hfont = NULL;
	m_PrintInfo.text_color = 0xFFFFFF;
	m_PrintInfo.back_color = 0;
	m_PrintInfo.bk_mode = 0;
	m_PrintInfo.text_align = 0;
	m_CursorColor = 0xFFFFFF;

#ifdef PLATFORM_MACOS
	m_GlyphCache = NULL;
	m_Layout = NULL;
	m_LayoutDirty = true;

	// Try to initialize Font Atlas rendering system
	// Note: We'll create the actual objects when needed (lazy initialization)
	// For now, we'll use g_Print() as fallback
#endif
}

LineEditorVisual::~LineEditorVisual()
{
	// Clear focus from InputFocusManager if this editor was focused
	if (InputFocusManager::GetInstance().GetFocusedEditor() == this) {
		InputFocusManager::GetInstance().SetFocusedEditor(NULL);
	}
}

void LineEditorVisual::Acquire()
{
	// Register this editor as the focused editor
	InputFocusManager::GetInstance().SetFocusedEditor(this);

	m_Editor.Acquire();
	m_bAcquired = true;

#ifdef PLATFORM_MACOS
	// Enable SDL text input on macOS
	SDL_StartTextInput();
#endif
}

void LineEditorVisual::Unacquire()
{
	// Release focus if this editor was focused
	if (InputFocusManager::GetInstance().GetFocusedEditor() == this) {
		InputFocusManager::GetInstance().SetFocusedEditor(NULL);
	}

	m_Editor.Unacquire();
	m_bAcquired = false;

#ifdef PLATFORM_MACOS
	// Disable SDL text input on macOS
	SDL_StopTextInput();
#endif
}

void LineEditorVisual::SetPosition(int x, int y)
{
	m_X = x;
	m_Y = y;
}

void LineEditorVisual::SetAbsWidth(int width)
{
	m_AbsWidth = width;
}

void LineEditorVisual::SetPrintInfo(PrintInfo& info)
{
	m_PrintInfo = info;
}

void LineEditorVisual::SetCursorColor(unsigned long color)
{
	m_CursorColor = color;
}

void LineEditorVisual::PasswordMode(bool bPassword)
{
	m_bPasswordMode = bPassword;
}

int LineEditorVisual::GetLineCount() const
{
	return 1;
}

bool LineEditorVisual::ReachSizeOfBox() const
{
	// Use a default font size since PrintInfo doesn't have a size field
	const int DEFAULT_FONT_SIZE = 12;
	int len = m_Editor.GetTextLen();
	return (len * DEFAULT_FONT_SIZE) >= m_MaxWidth;
}

// Compatibility method: convert UTF-32 to wide string (char_t/UTF-16LE)
const char_t* LineEditorVisual::GetStringWide() const
{
	static char_t wide_buffer[LineEditor::MAX_TEXT];
	int wide_len = 0;

	// Convert directly from UTF-32 (m_Text) to UTF-16 (char_t)
	for (int i = 0; i < m_Editor.m_TextLen && wide_len < LineEditor::MAX_TEXT - 1; i++) {
		uint32_t c = m_Editor.m_Text[i];

		// UTF-32 to UTF-16 conversion
		if (c < 0x10000) {
			// BMP character - single UTF-16 code unit
			wide_buffer[wide_len++] = (char_t)c;
		} else if (c < 0x10FFFF) {
			// Supplementary plane - surrogate pair
			if (wide_len + 1 >= LineEditor::MAX_TEXT - 1) break;

			c -= 0x10000;
			wide_buffer[wide_len++] = (char_t)(0xD800 + (c >> 10));      // High surrogate
			wide_buffer[wide_len++] = (char_t)(0xDC00 + (c & 0x3FF));    // Low surrogate
		} else {
			// Invalid Unicode - use replacement character
			wide_buffer[wide_len++] = (char_t)0xFFFD;
		}
	}

	wide_buffer[wide_len] = 0;

	return wide_buffer;
}

void LineEditorVisual::Show() const
{
	// Get the text to display (as UTF-8)
	const char* textToDisplay = m_Editor.GetBuffer();

	// Handle password mode (show asterisks instead of actual text)
	char displayBuffer[1024];
	if (m_bPasswordMode) {
		int len = strlen(textToDisplay);
		for (int i = 0; i < len && i < (int)sizeof(displayBuffer) - 1; i++) {
			displayBuffer[i] = '*';
		}
		displayBuffer[len] = '\0';
		textToDisplay = displayBuffer;
	}

#ifdef PLATFORM_MACOS
	// Use TextService for unified rendering
	extern CSpriteSurface* g_pLast;

	// Build text style from PrintInfo
	TextSystem::TextStyle style;
	style.font = TextSystem::TextService::Get().GetFont(14);  // Default font size
	style.align = TextSystem::TextAlign::Left;
	style.lineSpacing = 0;
	style.color = TextSystem::ColorFromRGB(m_PrintInfo.text_color);
	style.color.a = 255;

	// Create render target
	TextSystem::SpriteSurfaceRenderTarget target(g_pLast);

	// Render text
	// Note: TextService::DrawLine expects baseline position, but it adds GetFontAscent() internally
	// So we pass m_Y directly as the baseline position
	TextSystem::TextService::Get().DrawLine(target, textToDisplay, m_X, m_Y, m_MaxWidth, style);

	// Draw cursor if editor is acquired and cursor blink is on
	if (m_Editor.m_bAcquired && gC_ci != NULL && gC_ci->GetCursorBlink()) {
		// Calculate cursor X position using TextService
		int cursorX = m_X;
		if (m_Editor.m_CursorPos > 0) {
			// Get text before cursor
			char cursorBuffer[1024];
			const char* fullText = textToDisplay;

			// Convert cursor position (characters) to byte position
			int bytePos = 0;
			int charPos = 0;
			while (charPos < m_Editor.m_CursorPos && fullText[bytePos] != '\0') {
				if ((fullText[bytePos] & 0xC0) != 0x80) {  // Not a continuation byte
					charPos++;
				}
				bytePos++;
			}

			strncpy(cursorBuffer, fullText, bytePos);
			cursorBuffer[bytePos] = '\0';

			// Measure text width using TextService
			TextSystem::Metrics metrics = TextSystem::TextService::Get().MeasureText(cursorBuffer, style, 0);
			cursorX = m_X + metrics.width;
		}

		// Draw cursor
		PrintInfo cursorPI = m_PrintInfo;
		cursorPI.text_color = m_CursorColor;

		if (m_Editor.m_ComposingLen > 0) {
			// During IME composition, show underline-style cursor
			g_Print(cursorX, m_Y + 2, "_", &cursorPI);
		} else {
			// Normal block cursor
			g_Print(cursorX, m_Y - 1, "▊", &cursorPI);
		}
	}
#else
	// Windows: Use legacy g_Print() for now
	g_Print(m_X, m_Y, textToDisplay, (void*)NULL);

	// Draw cursor
	if (m_Editor.m_bAcquired && gC_ci != NULL && gC_ci->GetCursorBlink()) {
		// Calculate cursor position
		int cursorX = m_X;
		if (m_Editor.m_CursorPos > 0) {
			char cursorBuffer[1024];
			const char* fullText = m_Editor.GetBuffer();

			// Convert cursor position to bytes (UTF-8 aware)
			int bytePos = 0;
			int charPos = 0;
			while (charPos < m_Editor.m_CursorPos && fullText[bytePos] != '\0') {
				if ((fullText[bytePos] & 0xC0) != 0x80) {  // Not a continuation byte
					charPos++;
				}
				bytePos++;
			}

			strncpy(cursorBuffer, fullText, bytePos);
			cursorBuffer[bytePos] = '\0';
			cursorX = m_X + g_GetStringWidth(cursorBuffer, NULL);
		}

		// Draw cursor using text rendering
		PrintInfo cursorPI = m_PrintInfo;
		cursorPI.text_color = m_CursorColor;
		g_Print(cursorX, m_Y - 2, "|", &cursorPI);
	}
#endif
}

Point LineEditorVisual::GetPosition() const
{
	Point p;
	p.Set(m_X, m_Y);
	return p;
}
