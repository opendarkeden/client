#ifndef __INPUT_FOCUS_MANAGER_H__
#define __INPUT_FOCUS_MANAGER_H__

#include <cstddef>  // for NULL

/**
 * InputFocusManager - Global text input focus management
 *
 * Manages which LineEditorVisual currently has keyboard focus
 * and routes SDL text input events to the focused editor.
 *
 * This replaces the single global callback mechanism with a proper
 * focus system that allows all input boxes to work correctly.
 */

class LineEditorVisual;

class InputFocusManager
{
public:
	// Get singleton instance
	static InputFocusManager& GetInstance();

	// Set the currently focused text editor
	// Pass NULL to release focus
	void SetFocusedEditor(LineEditorVisual* editor);

	// Get the currently focused text editor
	// Returns NULL if no editor has focus
	LineEditorVisual* GetFocusedEditor() const;

	// Handle SDL text input event
	// Routes the event to the focused editor if one exists
	void HandleTextInput(const char* text);

	// Handle SDL text editing event (IME composition)
	// Routes the event to the focused editor if one exists
	void HandleTextEditing(const char* text, int start, int length);

	// Handle SDL key down event (control keys like backspace, arrows, etc.)
	// Routes the event to the focused editor if one exists
	void HandleKeyDown(unsigned int vk_code);

	// Check if any editor currently has focus
	bool HasFocus() const { return m_focusedEditor != NULL; }

private:
	// Private constructor for singleton
	InputFocusManager();
	~InputFocusManager() = default;

	// Prevent copying
	InputFocusManager(const InputFocusManager&) = delete;
	InputFocusManager& operator=(const InputFocusManager&) = delete;

	// Currently focused text editor
	LineEditorVisual* m_focusedEditor;
};

// Global accessor for use in DXLibBackendSDL (which cannot include VS_UI headers)
extern InputFocusManager& g_GetInputFocusManager();

#endif // __INPUT_FOCUS_MANAGER_H__
