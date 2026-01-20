#ifndef __LINE_EDITOR_H__
#define __LINE_EDITOR_H__

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Get PrintInfo definition from Fl2.h
#include "../hangul/Fl2.h"

// Get Point class definition
#include "../../../basic/BasicData.h"

/**
 * LineEditor - Simplified Windows IMM (Input Method Manager) replacement for macOS
 * This provides basic text editing functionality with UTF-32 support for SDL text input
 */

class LineEditor
{
public:
    static const int MAX_TEXT = 1024;

    // UTF-32 text buffer for proper multilingual support
    uint32_t m_Text[MAX_TEXT];
    int m_CursorPos;                // Cursor position (in characters)
    int m_TextLen;                 // Text length (in characters)
    int m_Limit;                   // Maximum character count
    bool m_bAcquired;

    // IME composition buffer (text being composed)
    uint32_t m_Composing[MAX_TEXT];
    int m_ComposingLen;

    LineEditor();
    ~LineEditor() = default;

    // Basic methods
    void Acquire();
    void Unacquire();
    bool IsAcquire() const;

    // New SDL text input methods
    void HandleTextInput(const char* text);
    void HandleTextEditing(const char* text, int start, int length);

    // Text manipulation methods
    void InsertText(const uint32_t* text, int len);
    void InsertChar(uint32_t c);
    void DeleteChar(int offset);
    void Backspace();
    void MoveCursor(int delta);
    void SetCursor(int pos);
    int GetCursorPos() const { return m_CursorPos; }
    int GetTextLen() const { return m_TextLen; }

    // IME composition
    void StartComposition(const char* text, int start, int length);
    void UpdateComposition(const char* text, int start, int length);
    void EndComposition();
    bool IsComposing() const { return m_ComposingLen > 0; }

    // Legacy compatibility methods (operate on UTF-8)
    void AddString(const char* pStr);
    void EraseAll();
    void EraseCharacterBegin();
    void HomeCursor() { SetCursor(0); }
    void EndCursor() { SetCursor(m_TextLen); }
    void SetByteLimit(int limit) { m_Limit = limit; }
    void InsertMark(unsigned short mark);
    void KeyboardControl(unsigned int message, unsigned int key, long extra);

    // Get text as UTF-8 string (for compatibility)
    const char* GetBuffer() const;  // Returns temporary UTF-8 buffer
    const char* GetString() const { return GetBuffer(); }  // Alias for GetBuffer()
    int Size() const { return m_TextLen; }  // Character count
    int GetCursor() const { return m_CursorPos; }  // Cursor position
};

class LineEditorVisual
{
public:
    LineEditor m_Editor;
    int m_X;
    int m_Y;
    int m_AbsWidth;
    int m_MaxWidth;
    bool m_bPasswordMode;
    bool m_bAcquired;
    PrintInfo m_PrintInfo;
    unsigned long m_CursorColor;

    LineEditorVisual();
    ~LineEditorVisual();

    void Acquire();
    void Unacquire();
    bool IsAcquire() const { return m_bAcquired; }

    // Forwarding methods to m_Editor for compatibility
    void AddString(const char* pStr) { m_Editor.AddString(pStr); }
    void EraseAll() { m_Editor.EraseAll(); }
    void EraseCharacterBegin() { m_Editor.EraseCharacterBegin(); }
    void SetByteLimit(int limit) { m_Editor.SetByteLimit(limit); }
    void KeyboardControl(unsigned int message, unsigned int key, long extra) {
        m_Editor.KeyboardControl(message, key, extra);
    }
    const char* GetString() const { return m_Editor.GetString(); }

    // Compatibility method: return const char_t* (wide string) for old code
    const char_t* GetStringWide() const;

    int Size() const { return m_Editor.Size(); }
    int GetCursor() const { return m_Editor.GetCursor(); }
    void InsertMark(char_t ch) { m_Editor.InsertMark((unsigned short)ch); }

    // Cursor movement
    void HomeCursor() { m_Editor.HomeCursor(); }
    void EndCursor() { m_Editor.EndCursor(); }

    // Set text color
    void SetInputStringColor(unsigned long rgb) { m_CursorColor = rgb; }  // Use cursor color for text color

    // Editor mode (stub for compatibility)
    void SetEditorMode(int gap, int height = 0) { /* Stub: not needed for SDL implementation */ }
    void SetDigitOnlyMode(bool enable) { /* Stub: not needed for SDL implementation */ }

    void SetPosition(int x, int y);
    void SetAbsWidth(int width);
    void SetPrintInfo(PrintInfo& info);
    void SetCursorColor(unsigned long color);
    void PasswordMode(bool bPassword);
    int GetLineCount() const;
    bool ReachSizeOfBox() const;
    void Show() const;

    // GetPosition method - returns BasicData::Point
    Point GetPosition() const;  // Implementation in cpp file
};

#endif // __LINE_EDITOR_H__
