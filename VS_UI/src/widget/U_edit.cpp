#include "U_edit.h"
#include "../hangul/CI.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef PLATFORM_MACOS
#include <SDL2/SDL.h>
#include "../../../Client/TextLib/CGlyphCache.h"
#include "../../../Client/TextLib/CTextLayout.h"
#include "../../../Client/ClientDef.h"  // For g_pBack
#include "../../../Client/SpriteLib/CSpriteSurface.h"  // For GetBackendSurface()
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
	if (m_CursorPos > m_TextLen) {
		m_CursorPos = m_TextLen;
	}
}

// Delete character before cursor (backspace)
void LineEditor::Backspace()
{
	if (m_CursorPos > 0) {
		DeleteChar(m_CursorPos - 1);
		m_CursorPos--;
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
#ifdef PLATFORM_MACOS
	if (m_GlyphCache) {
		delete (CGlyphCache*)m_GlyphCache;
		m_GlyphCache = NULL;
	}
	if (m_Layout) {
		delete (CTextLayout*)m_Layout;
		m_Layout = NULL;
	}
#endif
}

void LineEditorVisual::Acquire()
{
	m_Editor.Acquire();
	m_bAcquired = true;

#ifdef PLATFORM_MACOS
	// Enable SDL text input on macOS
	SDL_StartTextInput();
#endif
}

void LineEditorVisual::Unacquire()
{
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

// Compatibility method: convert UTF-8 to wide string (char_t)
const char_t* LineEditorVisual::GetStringWide() const
{
	static char_t wide_buffer[LineEditor::MAX_TEXT];
	const char* utf8 = m_Editor.GetString();

	// Simple UTF-8 to char_t conversion (ASCII only for now)
	int len = strlen(utf8);
	for (int i = 0; i < len && i < LineEditor::MAX_TEXT - 1; i++) {
		wide_buffer[i] = (char_t)(unsigned char)utf8[i];
	}
	wide_buffer[len] = 0;

	return wide_buffer;
}

void LineEditorVisual::Show() const
{
#ifdef PLATFORM_MACOS
	// Try Font Atlas rendering first
	CGlyphCache* cache = (CGlyphCache*)m_GlyphCache;
	CTextLayout* layout = (CTextLayout*)m_Layout;

	// Lazy initialization: create Font Atlas on first use
	if (!cache && g_pSDLRenderer) {
		// Try to find a font file
		const char* fontPaths[] = {
			"Data/Font/Hiragino Sans GB.ttc",
			"Data/Font/NotoSansCJK-Regular.ttc",
			"Data/Font/NotoSans-Regular.ttf",
			"Data/Font/DejaVuSans.ttf",
			"/System/Library/Fonts/Helvetica.ttc",
			"/System/Library/Fonts/Hiragino Sans GB.ttc",
			NULL
		};

		for (int i = 0; fontPaths[i] != NULL; i++) {
			// Create glyph cache
			cache = new CGlyphCache();
			if (cache->Init(fontPaths[i], 16)) {
				// Success! Keep the cache
				// Also create layout
				layout = new CTextLayout();
				// Cast away const for lazy initialization
				const_cast<LineEditorVisual*>(this)->m_GlyphCache = cache;
				const_cast<LineEditorVisual*>(this)->m_Layout = layout;
				printf("LineEditorVisual::Show: Initialized Font Atlas with %s\n", fontPaths[i]);
				break;
			} else {
				delete cache;
				cache = NULL;
			}
		}

		if (!cache) {
			printf("LineEditorVisual::Show: Failed to initialize Font Atlas, falling back to g_Print()\n");
		}
	}

	// If Font Atlas is available, use it for rendering
	if (cache && layout) {
		// Get text content
		const char* textToDisplay = m_Editor.GetBuffer();

//		printf("DEBUG LineEditorVisual::Show: textToDisplay='%s', len=%zu, m_LayoutDirty=%d\n",
//			   textToDisplay, strlen(textToDisplay), m_LayoutDirty);
//		printf("DEBUG LineEditorVisual::Show: Position m_X=%d, m_Y=%d\n", m_X, m_Y);

		// Handle password mode
		char displayBuffer[1024];
		if (m_bPasswordMode) {
			int len = strlen(textToDisplay);
			for (int i = 0; i < len && i < (int)sizeof(displayBuffer) - 1; i++) {
				displayBuffer[i] = '*';
			}
			displayBuffer[len] = '\0';
			textToDisplay = displayBuffer;
		}

		// Update layout (always update for now, since text may have changed)
		layout->Update(textToDisplay, cache, m_MaxWidth);
		const_cast<LineEditorVisual*>(this)->m_LayoutDirty = false;

		// Get glyphs from layout
		const std::vector<GlyphPosition>& glyphs = layout->GetGlyphs();
//		printf("DEBUG LineEditorVisual::Show: glyphs.size()=%zu\n", glyphs.size());

		// Get destination surface (g_pLast, where UI renders)
		// NOTE: UI renders to g_pLast, then g_pLast is copied to g_pBack before present
		spritectl_surface_t destSurface = g_pLast->GetBackendSurface();
//		printf("DEBUG LineEditorVisual::Show: destSurface=%p (g_pLast=%p), g_pBack=%p\n",
//			   destSurface, g_pLast, g_pBack);

		if (destSurface != SPRITECTL_INVALID_SURFACE) {
			// TEST: Draw a white rectangle to verify blt is working
			spritectl_surface_info_t info;
			if (spritectl_lock_surface(destSurface, &info) == 0) {
//				printf("DEBUG: Surface %dx%d, pitch=%d, format=%d\n",
//					   info.width, info.height, info.pitch, info.format);

				// Draw a white 50x20 rectangle at (m_X, m_Y)
				uint32_t* pixels = (uint32_t*)info.pixels;
				int rectW = 50, rectH = 20;
				for (int dy = 0; dy < rectH && (m_Y + dy) < info.height; dy++) {
					for (int dx = 0; dx < rectW && (m_X + dx) < info.width; dx++) {
						int px = m_X + dx;
						int py = m_Y + dy;
						if (px >= 0 && py >= 0) {
							// Set to white (0xFFFFFFFF in RGBA32)
							pixels[py * info.pitch / 4 + px] = 0xFFFFFFFF;
						}
					}
				}
				spritectl_unlock_surface(destSurface);
//				printf("DEBUG: Drew white rectangle at (%d,%d) size %dx%d\n", m_X, m_Y, rectW, rectH);
			}

			// Render each glyph using spritectl_blt_sprite
			int bltCount = 0;
			for (const auto& pos : glyphs) {
				if (!pos.glyph) continue;

				// Calculate screen position
				int screenX = m_X + pos.x;
				int screenY = m_Y + pos.y + cache->GetFontAscent();  // Baseline adjustment

				// Verify sprite is valid before blt
				if (pos.glyph->sprite == SPRITECTL_INVALID_SPRITE) {
					fprintf(stderr, "ERROR: Invalid sprite for charcode 0x%X\n", pos.glyph->charcode);
					continue;
				}

				// Blt glyph to surface (no alpha blending for now)
				int result = spritectl_blt_sprite(
					destSurface,
					screenX,
					screenY,
					pos.glyph->sprite,
					SPRITECTL_BLT_NONE,  // No alpha blending
					255  // Full opacity
				);

				if (result != 0) {
					fprintf(stderr, "%s:%d ERROR: spritectl_blt_sprite failed for charcode 0x%X, result=%d\n",
                            __FILE__, __LINE__,
							pos.glyph->charcode, result);
				} else {
					bltCount++;
				}
			}
//			printf("DEBUG LineEditorVisual::Show: Blitted %d/%zu glyphs successfully\n", bltCount, glyphs.size());
		} else {
			fprintf(stderr, "%s:%d LineEditorVisual::Show: Invalid destination surface\n", __FILE__, __LINE__);
		}

		// Draw cursor using g_Print (simpler than creating line sprites)
		if (m_Editor.m_bAcquired && gC_ci != NULL && gC_ci->GetCursorBlink()) {
			// Calculate cursor X position
			int cursorX = m_X;
			if (m_Editor.m_CursorPos > 0 && m_Editor.m_CursorPos <= (int)glyphs.size()) {
				cursorX = m_X + glyphs[m_Editor.m_CursorPos - 1].x +
				          glyphs[m_Editor.m_CursorPos - 1].glyph->advance_x;
			}

			// Draw cursor using g_Print
			PrintInfo cursorPI = m_PrintInfo;
			cursorPI.text_color = m_CursorColor;

			if (m_Editor.m_ComposingLen > 0) {
				// During IME composition, show underline-style cursor
//				g_Print(cursorX, m_Y + 2, "_", &cursorPI);
			} else {
				// Normal block cursor
//				g_Print(cursorX, m_Y - 1, "▊", &cursorPI);
			}
		}

		return;  // Done with Font Atlas rendering
	}

	// Fallback to g_Print() if Font Atlas is not available
#endif

	// Original g_Print() implementation (fallback)
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

	// Render the text using FL2's g_Print function
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

#ifdef PLATFORM_MACOS
		if (m_Editor.m_ComposingLen > 0) {
			// During IME composition, show underline-style cursor
			g_Print(cursorX, m_Y + 2, "_", &cursorPI);
		} else {
			// Normal block cursor
			g_Print(cursorX, m_Y - 1, "▊", &cursorPI);
		}
#else
		// Windows: simple vertical bar
		g_Print(cursorX, m_Y - 2, "|", &cursorPI);
#endif
	}
}

Point LineEditorVisual::GetPosition() const
{
	Point p;
	p.Set(m_X, m_Y);
	return p;
}
