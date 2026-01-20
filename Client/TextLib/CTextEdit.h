#ifndef __CTEXTEDIT_H__
#define __CTEXTEDIT_H__

// STB Textedit - Single-header text editing library
// Downloaded from: https://gitee.com/mirrors/stb/raw/master/stb_textedit.h

#ifdef PLATFORM_MACOS

// Define STB textedit string implementation
// We'll implement the interface for our LineEditor class

#define STB_TEXTEDIT_STRING           // Use custom string interface
#define STB_TEXTEDIT_IMPLEMENTATION   // Include implementation
#define STB_TEXTEDIT_MEMMOVE          // Use custom memmove if needed

#include "stb_textedit.h"

// Forward declaration
class LineEditor;

// ============================================================================
// STB Textedit interface for LineEditor
// ============================================================================

// Get character at position (returns UTF-32 charcode)
// LineEditor stores text as UTF-32 internally, so we just return m_Text[pos]
static inline int stb_textedit_get_char(const LineEditor* editor, int pos) {
    if (pos < 0 || pos >= editor->GetTextLen()) {
        return 0;
    }
    return editor->m_Text[pos];
}

// Get total text length
static inline int stb_textedit_get_length(const LineEditor* editor) {
    return editor->GetTextLen();
}

// Layout callback - returns row information for cursor positioning
// This is used for multi-line editing; for single-line we return a simple row
static inline void stb_textedit_layout_row(StbTexteditRow* row, const LineEditor* editor, int line_start_idx) {
    // For single-line editor, all text is on one row
    // TODO: Implement proper width calculation for multi-line support
    row->x0 = 0;
    row->x1 = editor->GetTextLen();  // Placeholder: should use actual pixel width
    row->baseline_y_delta = editor->GetTextLen();  // Height of the row
    row->num_chars = editor->GetTextLen();
}

// Optional: Delete characters callback (for custom delete behavior)
static inline void stb_textedit_delete_chars(StbTexteditState* state, int pos, int num) {
    // Use default implementation (memmove)
    // This will be handled by STB_TEXTEDIT_STRING if we define it properly
}

// ============================================================================
// STB Textedit key definitions for LineEditor
// ============================================================================

// Map our virtual key codes to STB textedit keys
#define STB_TEXTEDIT_K_LEFT         VK_LEFT      // Arrow left
#define STB_TEXTEDIT_K_RIGHT        VK_RIGHT     // Arrow right
#define STB_TEXTEDIT_K_UP           VK_UP        // Arrow up
#define STB_TEXTEDIT_K_DOWN         VK_DOWN      // Arrow down
#define STB_TEXTEDIT_K_LINESTART    VK_HOME      // Home
#define STB_TEXTEDIT_K_LINEEND      VK_END       // End
#define STB_TEXTEDIT_K_TEXTSTART    0            // Ctrl+Home (not used)
#define STB_TEXTEDIT_K_TEXTEND      0            // Ctrl+End (not used)
#define STB_TEXTEDIT_K_DELETE       VK_DELETE    // Delete
#define STB_TEXTEDIT_K_BACKSPACE    VK_BACK      // Backspace
#define STB_TEXTEDIT_K_UNDO         ('Z' - 64)   // Ctrl+Z
#define STB_TEXTEDIT_K_REDO         ('Y' - 64)   // Ctrl+Y
#define STB_TEXTEDIT_K_INSERT       0            // Insert (not used)
#define STB_TEXTEDIT_K_WORDLEFT     0            // Ctrl+Left (not used)
#define STB_TEXTEDIT_K_WORDRIGHT    0            // Ctrl+Right (not used)
#define STB_TEXTEDIT_K_PGUP         0            // Page Up (not used)
#define STB_TEXTEDIT_K_PGDOWN       0            // Page Down (not used)

#endif // PLATFORM_MACOS

#endif // __CTEXTEDIT_H__
