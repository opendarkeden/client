#include "InputFocusManager.h"
#include "widget/U_edit.h"
#include <stdio.h>

InputFocusManager::InputFocusManager()
	: m_focusedEditor(NULL)
{
}

InputFocusManager& InputFocusManager::GetInstance()
{
	static InputFocusManager instance;
	return instance;
}

// Global accessor for DXLibBackendSDL (cannot include headers)
InputFocusManager& g_GetInputFocusManager()
{
	return InputFocusManager::GetInstance();
}

void InputFocusManager::SetFocusedEditor(LineEditorVisual* editor)
{
	m_focusedEditor = editor;

	// Debug logging
	if (editor) {
		printf("InputFocusManager: Set focused editor to %p\n", (void*)editor);
	} else {
		printf("InputFocusManager: Cleared focused editor\n");
	}
}

LineEditorVisual* InputFocusManager::GetFocusedEditor() const
{
	return m_focusedEditor;
}

void InputFocusManager::HandleTextInput(const char* text)
{
	if (!m_focusedEditor) {
		// No editor has focus, ignore input
		return;
	}

	// Route text input to focused editor
	m_focusedEditor->m_Editor.HandleTextInput(text);
}

void InputFocusManager::HandleTextEditing(const char* text, int start, int length)
{
	if (!m_focusedEditor) {
		// No editor has focus, ignore editing events
		return;
	}

	// Route text editing to focused editor
	m_focusedEditor->m_Editor.HandleTextEditing(text, start, length);
}

void InputFocusManager::HandleKeyDown(unsigned int vk_code)
{
	if (!m_focusedEditor) {
		// No editor has focus, ignore key events
		return;
	}

	// Route key down to focused editor
	// WM_KEYDOWN = 0x0100 (Windows message value)
	m_focusedEditor->m_Editor.KeyboardControl(0x0100, vk_code, 0);
}
