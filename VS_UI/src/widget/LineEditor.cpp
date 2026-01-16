#include "LineEditor.h"

// LineEditor implementation
LineEditor::LineEditor()
{
	m_Buffer[0] = '\0';
	m_CursorPos = 0;
	m_Limit = 1023;
	m_bAcquired = false;
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

void LineEditor::AddString(const char* pStr)
{
	if (pStr == NULL) return;

	int len = strlen(pStr);
	int currentLen = strlen(m_Buffer);

	if (currentLen + len < m_Limit)
	{
		strcat(m_Buffer, pStr);
		m_CursorPos += len;
	}
}

void LineEditor::EraseAll()
{
	m_Buffer[0] = '\0';
	m_CursorPos = 0;
}

void LineEditor::EraseCharacterBegin()
{
	if (m_CursorPos > 0)
	{
		int len = strlen(m_Buffer);
		for (int i = m_CursorPos - 1; i < len; i++)
		{
			m_Buffer[i] = m_Buffer[i + 1];
		}
		m_CursorPos--;
	}
}

void LineEditor::HomeCursor()
{
	m_CursorPos = 0;
}

void LineEditor::EndCursor()
{
	m_CursorPos = strlen(m_Buffer);
}

void LineEditor::SetByteLimit(int limit)
{
	if (limit > 0 && limit < (int)sizeof(m_Buffer))
	{
		m_Limit = limit;
	}
}

void LineEditor::InsertMark(unsigned short mark)
{
	// Stub for special character insertion
	(void)mark;
}

void LineEditor::KeyboardControl(unsigned int nChar, unsigned int nRepCnt, long nFlags)
{
	// Basic keyboard handling stub
	(void)nChar; (void)nRepCnt; (void)nFlags;
}

// LineEditorVisual implementation
LineEditorVisual::LineEditorVisual()
{
	m_X = 0;
	m_Y = 0;
	m_AbsWidth = 100;
	m_MaxWidth = 100;
	m_bPasswordMode = false;
	m_PrintInfo.font = NULL;
	m_PrintInfo.color = 0xFFFFFF;
	m_PrintInfo.size = 12;
	m_CursorColor = 0xFFFFFF;
}

LineEditorVisual::~LineEditorVisual()
{
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
	int len = strlen(m_Editor.m_Buffer);
	return (len * m_PrintInfo.size) >= m_MaxWidth;
}

void LineEditorVisual::Show() const
{
	// Stub: Would render the text editor
}
