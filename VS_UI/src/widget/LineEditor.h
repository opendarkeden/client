#ifndef __LINE_EDITOR_H__
#define __LINE_EDITOR_H__

#include <stdlib.h>
#include <string.h>

/**
 * LineEditor - Simplified Windows IMM (Input Method Manager) replacement for macOS
 * This provides basic text editing functionality
 */

struct PrintInfo
{
    void* font;
    unsigned long color;
    int size;
};

class LineEditor
{
public:
    char m_Buffer[1024];
    int m_CursorPos;
    int m_Limit;
    bool m_bAcquired;

    LineEditor();
    ~LineEditor() = default;

    void Acquire();
    void Unacquire();
    bool IsAcquire() const;

    void AddString(const char* pStr);
    void EraseAll();
    void EraseCharacterBegin();
    void HomeCursor();
    void EndCursor();
    void SetByteLimit(int limit);
    void InsertMark(unsigned short mark);
    void KeyboardControl(unsigned int nChar, unsigned int nRepCnt, long nFlags);
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
    PrintInfo m_PrintInfo;
    unsigned long m_CursorColor;

    LineEditorVisual();
    ~LineEditorVisual();

    void SetPosition(int x, int y);
    void SetAbsWidth(int width);
    void SetPrintInfo(PrintInfo& info);
    void SetCursorColor(unsigned long color);
    void PasswordMode(bool bPassword);
    int GetLineCount() const;
    bool ReachSizeOfBox() const;
    void Show() const;
};

#endif // __LINE_EDITOR_H__
