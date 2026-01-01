# Requirements Document

## Introduction

This document defines the requirements for a pure C UI component system that ports the legacy DarkEden DirectX-based UI code (from `legacy/VS_UI/src/widget/`) to SDL2. The goal is to create a modular, portable UI framework where each component is small, independent, and testable in isolation via demo programs.

The legacy code structure is:
- `u_window.h/cpp` - Window base class with attributes (topmost, pin, keyboard_control, alpha, autohide)
- `u_button.h/cpp` - ButtonBase, Button, EventButton classes with focus/press states
- `u_scrollbar.h/cpp` - Scrollbar widget
- `VS_UI_Dialog.h/cpp` - Dialog with message area, menu items, and OK/Cancel buttons
- `VS_UI_Widget.h/cpp` - C_VS_UI_EVENT_BUTTON, ButtonGroup, ButtonVisual

## Glossary

- **Window**: Base structure for all UI windows (mirrors legacy `Window` class from `u_window.h`)
- **WindowManager**: Manages window registration, z-order, show/hide lists, and event routing (mirrors legacy `WindowManager`)
- **Rect**: Basic rectangle structure with x, y, w, h (mirrors legacy `Rect` from `Basics.h`)
- **ButtonBase**: Core button state (focused, pressed) without execution logic
- **Button**: ButtonBase with execution handler and click options (RUN_WHEN_PUSH, RUN_WHEN_PUSHUP)
- **EventButton**: Button with event callbacks (EventFocusOn, EventFocusOff, EventPressDown, EventPressUp)
- **ButtonGroup**: Container managing multiple buttons within a window
- **Exec**: Interface for button execution callbacks (Run function)
- **Dialog**: Window with message area, optional menu items, and configurable buttons (OK, Cancel)
- **ScrollBar**: Scroll widget with amount tracking and position control
- **Surface**: SDL texture-based drawing surface for sprite blitting

## Requirements

### Requirement 1: Rect Structure

**User Story:** As a developer, I want a basic rectangle structure, so that all UI components can share common positioning and sizing logic.

#### Acceptance Criteria

1. THE Rect SHALL store integer x, y, w, h values
2. THE Rect SHALL provide Set function to update all values
3. THE Rect SHALL provide IsInRect function to test if a point is inside
4. THE Rect SHALL provide Right function returning x + w
5. THE Rect SHALL provide Down function returning y + h

### Requirement 2: Window Base Structure

**User Story:** As a developer, I want a Window base structure matching the legacy Window class, so that all UI windows share common behavior.

#### Acceptance Criteria

1. THE Window SHALL inherit from Rect for position and size
2. THE Window SHALL store attributes: window_move, pin, topmost, keyboard_control, alpha, autohide
3. THE Window SHALL track move_ready state and origin point for dragging
4. THE Window SHALL track mouse_state (MOUSE_NOSTATE, MOUSE_MOVE, MOUSE_LEFTDOWN, etc.)
5. WHEN MouseControl is called with M_MOVING and move_ready is true, THE Window SHALL update position based on origin
6. WHEN MouseControl is called with M_LEFTBUTTON_UP, THE Window SHALL clear move_ready state
7. THE Window SHALL provide MoveReady function to enable dragging
8. THE Window SHALL provide SetOrigin function to set drag reference point
9. THE Window SHALL provide virtual Show function pointer for rendering
10. THE Window SHALL provide virtual IsPixel function pointer for hit testing
11. THE Window SHALL provide virtual Process function pointer for per-frame updates

### Requirement 3: WindowManager

**User Story:** As a developer, I want a WindowManager matching the legacy implementation, so that windows are properly registered, ordered, and receive events.

#### Acceptance Criteria

1. THE WindowManager SHALL maintain show_list for non-pinned windows
2. THE WindowManager SHALL maintain show_list_pinned for pinned windows
3. THE WindowManager SHALL track mouse_focused_window, topmost_window, keyboard_control_window
4. WHEN Register is called, THE WindowManager SHALL add window to internal list
5. WHEN Unregister is called, THE WindowManager SHALL remove window from all lists
6. WHEN AppearWindow is called, THE WindowManager SHALL add window to appropriate show_list and bring to front
7. WHEN DisappearWindow is called, THE WindowManager SHALL remove window from show_list
8. WHEN Show is called, THE WindowManager SHALL render windows in z-order (bottom to top, pinned after non-pinned, topmost last)
9. WHEN MouseControl is called, THE WindowManager SHALL route to focused window and update focus based on IsPixel
10. WHEN KeyboardControl is called, THE WindowManager SHALL route to keyboard_control_window or topmost_window
11. THE WindowManager SHALL provide FirstPriority function to bring window to front of its list
12. THE WindowManager SHALL provide GetFirstPriorityWindow function returning the topmost visible window

### Requirement 4: ButtonBase Structure

**User Story:** As a developer, I want a ButtonBase structure matching the legacy class, so that buttons track focus and press states.

#### Acceptance Criteria

1. THE ButtonBase SHALL inherit from Rect
2. THE ButtonBase SHALL track focused state (bool)
3. THE ButtonBase SHALL track pressed state (bool)
4. THE ButtonBase SHALL provide FocusOn, FocusOff, PressDown, PressUp functions
5. THE ButtonBase SHALL provide GetFocusState, GetPressState functions
6. THE ButtonBase SHALL provide InitState function to reset both states to false

### Requirement 5: Button Structure

**User Story:** As a developer, I want a Button structure with execution handling, so that buttons can trigger callbacks when clicked.

#### Acceptance Criteria

1. THE Button SHALL inherit from ButtonBase
2. THE Button SHALL store exec_handler function pointer and id
3. THE Button SHALL support click_option: RUN_WHEN_PUSH or RUN_WHEN_PUSHUP
4. WHEN MouseControl receives M_MOVING, THE Button SHALL set focus based on IsInRect
5. WHEN MouseControl receives M_LEFTBUTTON_DOWN inside button, THE Button SHALL set pressed and optionally Run if RUN_WHEN_PUSH
6. WHEN MouseControl receives M_LEFTBUTTON_UP with focus and pressed, THE Button SHALL call Run if RUN_WHEN_PUSHUP
7. THE Button SHALL provide SetID, SetExecHandler, SetClickOption functions

### Requirement 6: EventButton Structure

**User Story:** As a developer, I want an EventButton with event callbacks, so that buttons can respond to state changes with custom behavior.

#### Acceptance Criteria

1. THE EventButton SHALL inherit from Button
2. THE EventButton SHALL track prev_focus state for change detection
3. THE EventButton SHALL provide virtual EventFocusOn callback
4. THE EventButton SHALL provide virtual EventFocusOff callback
5. THE EventButton SHALL provide virtual EventPressDown callback
6. THE EventButton SHALL provide virtual EventPressUp callback
7. WHEN focus changes from false to true, THE EventButton SHALL call EventFocusOn
8. WHEN focus changes from true to false, THE EventButton SHALL call EventFocusOff

### Requirement 7: ButtonGroup

**User Story:** As a developer, I want a ButtonGroup container, so that multiple buttons in a window can be managed together.

#### Acceptance Criteria

1. THE ButtonGroup SHALL store array of Button pointers
2. THE ButtonGroup SHALL store parent window pointer for coordinate translation
3. THE ButtonGroup SHALL provide Add function to add buttons
4. THE ButtonGroup SHALL provide MouseControl that routes to all buttons
5. THE ButtonGroup SHALL provide Show that renders all buttons via ButtonVisual callback
6. THE ButtonGroup SHALL provide Init function to reset all button states
7. THE ButtonGroup SHALL provide Process function for per-frame updates
8. THE ButtonGroup SHALL provide CancelPushState to clear pressed state on all buttons
9. THE ButtonGroup SHALL provide UnacquireMouseFocus to clear focus on all buttons

### Requirement 8: Dialog Structure

**User Story:** As a developer, I want a Dialog structure matching the legacy C_VS_UI_DIALOG, so that message boxes with buttons can be displayed.

#### Acceptance Criteria

1. THE Dialog SHALL inherit from Window
2. THE Dialog SHALL support button flags: DIALOG_OK, DIALOG_CANCEL, DIALOG_NO_BUTTON
3. THE Dialog SHALL store exec_fp callback function pointer
4. THE Dialog SHALL contain ButtonGroup for OK/Cancel buttons
5. THE Dialog SHALL store message lines array and line_count
6. THE Dialog SHALL calculate client_rect, msg_rect, menu_rect based on size
7. WHEN Show is called, THE Dialog SHALL render background, message text, and buttons
8. WHEN Run is called with DIALOG_EXECID_OK, THE Dialog SHALL call exec_fp and Finish
9. WHEN Run is called with DIALOG_EXECID_CANCEL, THE Dialog SHALL call exec_fp and Finish
10. WHEN KeyboardControl receives VK_RETURN, THE Dialog SHALL trigger OK if available
11. WHEN KeyboardControl receives VK_ESCAPE, THE Dialog SHALL trigger Cancel or Exit
12. THE Dialog SHALL provide SetMessage function to set message lines
13. THE Dialog SHALL provide Start and Finish functions for lifecycle

### Requirement 9: Surface Abstraction

**User Story:** As a developer, I want a Surface abstraction for SDL, so that sprite blitting works similarly to the legacy DirectDraw code.

#### Acceptance Criteria

1. THE Surface SHALL wrap SDL_Texture as render target
2. THE Surface SHALL provide Init function with SDL_Renderer and dimensions
3. THE Surface SHALL provide Lock/Unlock for pixel access compatibility
4. THE Surface SHALL provide FillRect for solid color rectangles
5. THE Surface SHALL provide GetTexture for SDL rendering operations
6. THE Surface SHALL provide GetRenderer accessor

### Requirement 10: Event Message Constants

**User Story:** As a developer, I want event message constants matching the legacy code, so that event routing uses the same values.

#### Acceptance Criteria

1. THE System SHALL define M_MOVING for mouse move events
2. THE System SHALL define M_LEFTBUTTON_DOWN for left mouse button press
3. THE System SHALL define M_LEFTBUTTON_UP for left mouse button release
4. THE System SHALL define M_RIGHTBUTTON_DOWN for right mouse button press
5. THE System SHALL define M_RIGHTBUTTON_UP for right mouse button release
6. THE System SHALL define M_LB_DOUBLECLICK for left double-click
7. THE System SHALL define M_WHEEL_UP for mouse wheel up
8. THE System SHALL define M_WHEEL_DOWN for mouse wheel down
9. THE System SHALL define WM_KEYDOWN for keyboard key press
10. THE System SHALL define WM_CHAR for character input

### Requirement 11: Demo Application

**User Story:** As a developer, I want a demo application, so that I can verify each UI component works correctly.

#### Acceptance Criteria

1. THE Demo SHALL initialize SDL framework using existing lib/sdl_framework
2. THE Demo SHALL create WindowManager instance
3. THE Demo SHALL create a Dialog with OK button
4. THE Demo SHALL translate SDL events to legacy message constants
5. THE Demo SHALL route events through WindowManager
6. THE Demo SHALL render via WindowManager Show function
7. WHEN OK button is clicked, THE Demo SHALL print confirmation message
8. WHEN Escape is pressed, THE Demo SHALL exit cleanly
9. THE Demo SHALL run at 60 FPS using sdl_framework_delay
