# Implementation Plan: SDL UI System

## Overview

This implementation plan breaks down the SDL UI System into incremental tasks. Each task builds on previous ones, starting with low-level components and progressing to higher-level abstractions. The demo application serves as the final integration point.

## Tasks

- [x] 1. Implement UI_Surface component
  - [x] 1.1 Create lib/ui_surface.h with structure and function declarations
    - Define UI_Surface struct with SDL_Renderer, SDL_Texture, dimensions, pixels, locked state
    - Declare init, free, lock, unlock functions
    - Declare blit and drawing primitive functions
    - _Requirements: 1.1, 1.2, 1.3_

  - [x] 1.2 Implement ui_surface_init and ui_surface_free
    - Create streaming SDL_Texture with SDL_TEXTUREACCESS_TARGET
    - Store renderer reference and dimensions
    - Free texture on cleanup
    - _Requirements: 1.1_

  - [x] 1.3 Implement ui_surface_lock and ui_surface_unlock
    - Use SDL_LockTexture/SDL_UnlockTexture for pixel access
    - Track locked state to prevent double lock/unlock
    - _Requirements: 1.2_

  - [x] 1.4 Implement ui_surface_fill_rect, ui_surface_hline, ui_surface_vline
    - Use SDL_SetRenderTarget to draw on texture
    - Implement fill_rect using SDL_RenderFillRect
    - Implement hline/vline using SDL_RenderDrawLine
    - _Requirements: 1.7, 1.8_

  - [ ]* 1.5 Write property test for fill_rect correctness
    - **Property 10: Surface Fill Rect Correctness**
    - **Validates: Requirements 1.7**

  - [x] 1.6 Implement ui_surface_blit_sprite
    - Render DecodedSprite texture at specified position
    - Handle sprite with existing SDL_Texture
    - _Requirements: 1.3_

  - [x] 1.7 Implement ui_surface_blit_sprite_alpha, _color, _darkness
    - Apply SDL_SetTextureAlphaMod for alpha blending
    - Apply SDL_SetTextureColorMod for color tinting
    - Apply darkness as reduced color mod values
    - _Requirements: 1.4, 1.5, 1.6_

- [x] 2. Checkpoint - Verify UI_Surface
  - All code compiles, library builds successfully.

- [x] 3. Implement UI_Window base component
  - [x] 3.1 Create lib/ui_window.h with message types and structures
    - Define UI_MouseMessage enum (M_MOVING, M_LEFTBUTTON_DOWN, etc.)
    - Define WM_KEYDOWN, WM_KEYUP, WM_CHAR constants
    - Define UI_WindowAttr struct
    - Define UI_Window struct with function pointers
    - _Requirements: 2.1, 2.2, 2.3, 2.4_

  - [x] 3.2 Create lib/ui_window.c with implementation
    - Implement ui_window_init to set position, dimensions, default attributes
    - Implement ui_window_set_rect, set_visible, set_topmost, set_pinned
    - Implement ui_window_default_is_pixel (simple rect check)
    - Implement ui_window_default_mouse_control
    - _Requirements: 2.5, 2.6, 2.7, 2.8, 2.9_

  - [ ]* 3.3 Write property test for window attribute round-trip
    - **Property 1: Window Attribute Round-Trip**
    - **Validates: Requirements 2.1, 2.2, 2.3, 2.4**

- [x] 4. Implement UI_Button component
  - [x] 4.1 Create lib/ui_button.h with structure and declarations
    - Define UI_Button struct with position, dimensions, id, focus, pressed, callback
    - Declare init, free, mouse_control, render functions
    - _Requirements: 3.1, 3.2, 3.3_

  - [x] 4.2 Implement ui_button_init and ui_button_free
    - Initialize all fields to defaults
    - Set callback to NULL initially
    - _Requirements: 3.1_

  - [x] 4.3 Implement ui_button_mouse_control
    - Handle M_MOVING: set focus based on hit test
    - Handle M_LEFTBUTTON_DOWN: set pressed if focused
    - Handle M_LEFTBUTTON_UP: trigger callback if pressed and focused
    - _Requirements: 3.4, 3.5, 3.6, 3.7_

  - [ ]* 4.4 Write property test for button state consistency
    - **Property 2: Button State Consistency**
    - **Validates: Requirements 3.2, 3.4, 3.5**

  - [ ]* 4.5 Write property test for button press-release callback
    - **Property 3: Button Press-Release Callback**
    - **Validates: Requirements 3.6, 3.7**

  - [x] 4.6 Implement ui_button_render and ui_button_render_fallback
    - Render sprite based on state (normal/hover/pressed frame offsets)
    - Fallback: draw rectangle outline with state-based colors
    - _Requirements: 3.8, 3.9_

- [x] 5. Checkpoint - Verify UI_Button
  - All code compiles, library builds successfully.

- [x] 6. Implement Button_Group component
  - [x] 6.1 Create lib/ui_button_group.h with structure and declarations
    - Define UI_ButtonGroup struct with button array, count, parent
    - Define MAX_BUTTONS_PER_GROUP constant
    - Declare init, free, add, remove, get, reset functions
    - _Requirements: 4.1_

  - [x] 6.2 Implement ui_button_group_init and ui_button_group_free
    - Initialize count to 0, parent reference
    - Free all contained buttons on cleanup
    - _Requirements: 4.6_

  - [x] 6.3 Implement ui_button_group_add and ui_button_group_remove
    - Add: check capacity, store button pointer, increment count
    - Remove: find by id, shift array, decrement count
    - _Requirements: 4.4, 4.5_

  - [ ]* 6.4 Write property test for button group count invariant
    - **Property 4: Button Group Count Invariant**
    - **Validates: Requirements 4.1, 4.4, 4.5**

  - [x] 6.5 Implement ui_button_group_mouse_control
    - Iterate all buttons, call mouse_control on each
    - Return 1 if any button consumed event
    - _Requirements: 4.2_

  - [x] 6.6 Implement ui_button_group_render and ui_button_group_reset
    - Render: iterate and render each button
    - Reset: set focus=0, pressed=0 for all buttons
    - _Requirements: 4.3, 4.7_

- [x] 7. Implement Event_Router component
  - [x] 7.1 Create lib/ui_event_router.h with UI_Event structure
    - Define UI_Event struct with message, x, y, key, extra
    - Declare ui_event_translate function
    - _Requirements: 7.1-7.7_

  - [x] 7.2 Implement ui_event_translate
    - Map SDL_MOUSEMOTION to M_MOVING
    - Map SDL_MOUSEBUTTONDOWN/UP (left) to M_LEFTBUTTON_DOWN/UP
    - Map SDL_MOUSEBUTTONDOWN/UP (right) to M_RIGHTBUTTON_DOWN/UP
    - Map SDL_MOUSEWHEEL to M_WHEEL_UP/DOWN
    - Map SDL_KEYDOWN to WM_KEYDOWN
    - _Requirements: 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7_

  - [ ]* 7.3 Write property test for event translation correctness
    - **Property 5: Event Translation Correctness**
    - **Validates: Requirements 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7**

  - [x] 7.4 Implement ui_event_dispatch_to_window
    - Call window's mouse_control or keyboard_control based on event type
    - Return result from window handler
    - _Requirements: 7.8, 7.9_

- [x] 8. Checkpoint - Verify Event_Router
  - All code compiles, library builds successfully.

- [x] 9. Implement Window_Manager component
  - [x] 9.1 Create lib/ui_window_manager.h with structure and declarations
    - Define UI_WindowManager struct with window array, count, surface
    - Define MAX_WINDOWS constant
    - Declare all management functions
    - _Requirements: 8.1_

  - [x] 9.2 Implement ui_window_manager_init and ui_window_manager_free
    - Initialize count to 0, store surface reference
    - No ownership of windows - just clear array on free
    - _Requirements: 8.1_

  - [x] 9.3 Implement ui_window_manager_register and ui_window_manager_unregister
    - Register: check capacity, add to end of array
    - Unregister: find window, shift array, decrement count
    - _Requirements: 8.2, 8.3_

  - [ ]* 9.4 Write property test for window manager registration invariant
    - **Property 6: Window Manager Registration Invariant**
    - **Validates: Requirements 8.1, 8.2, 8.3**

  - [x] 9.5 Implement ui_window_manager_bring_to_front
    - Find window in array, move to end (highest z-order)
    - _Requirements: 8.6_

  - [x] 9.6 Implement ui_window_manager_appear and ui_window_manager_disappear
    - Appear: set visible=1, bring to front
    - Disappear: set visible=0
    - _Requirements: 8.7, 8.8_

  - [x] 9.7 Implement ui_window_manager_process and ui_window_manager_render
    - Process: iterate visible windows, call process function
    - Render: iterate visible windows in z-order (0 to count-1), call show function
    - _Requirements: 8.4_

  - [x] 9.8 Implement ui_window_manager_dispatch_event
    - Iterate windows in reverse z-order (count-1 to 0)
    - Call ui_event_dispatch_to_window for each visible window
    - Stop if event consumed
    - _Requirements: 8.5_

  - [ ]* 9.9 Write property test for event dispatch order
    - **Property 7: Event Dispatch Order**
    - **Validates: Requirements 7.8, 8.5**

  - [ ]* 9.10 Write property test for event consumption stops dispatch
    - **Property 8: Event Consumption Stops Dispatch**
    - **Validates: Requirements 7.9**

- [x] 10. Checkpoint - Verify Window_Manager
  - All code compiles, library builds successfully.

- [x] 11. Implement Global_Resource component
  - [x] 11.1 Create lib/ui_global_resource.h with structure and declarations
    - Define sprite index constants (AB_BACK, AB_BUTTON_OK, etc.)
    - Define UI_GlobalResource struct
    - Declare init, free, draw functions
    - _Requirements: 6.1, 6.2_

  - [x] 11.2 Implement ui_global_resource_init
    - Load dialog sprite pack using spritepack_load
    - Load button sprite pack using spritepack_load
    - Log errors but don't fail if packs missing (allow fallback)
    - _Requirements: 6.1, 6.2, 6.5_

  - [x] 11.3 Implement ui_global_resource_free
    - Free both sprite packs
    - _Requirements: 6.6_

  - [x] 11.4 Implement ui_global_resource_draw_dialog
    - Tile background sprite to fill dialog area
    - Draw right edge, bottom edge, corner sprites
    - Fallback: draw simple rectangle if sprites not loaded
    - _Requirements: 6.3_

- [x] 12. Implement UI_Dialog component
  - [x] 12.1 Create lib/ui_dialog.h with structure and declarations
    - Define DIALOG_OK, DIALOG_CANCEL flags
    - Define DIALOG_EXECID_OK, DIALOG_EXECID_CANCEL
    - Define UI_Dialog struct extending UI_Window
    - Declare create, destroy, set_message, start, finish functions
    - _Requirements: 5.1, 5.2_

  - [x] 12.2 Implement ui_dialog_create and ui_dialog_destroy
    - Allocate UI_Dialog, initialize base UI_Window
    - Create buttons based on button_flags
    - Set up function pointers for show, process, mouse_control, keyboard_control
    - _Requirements: 5.1, 5.2_

  - [x] 12.3 Implement dialog show function
    - Draw dialog background using global resource
    - Render button group
    - Draw message text (simple SDL_RenderDrawPoint placeholder or skip for now)
    - _Requirements: 5.3, 5.4_

  - [x] 12.4 Implement dialog mouse_control function
    - Route to button group
    - Handle button callbacks to invoke dialog callback
    - _Requirements: 5.5, 5.6_

  - [x] 12.5 Implement dialog keyboard_control function
    - Handle Enter key -> invoke OK callback
    - Handle Escape key -> invoke Cancel callback
    - _Requirements: 5.7, 5.8_

  - [ ]* 12.6 Write property test for dialog keyboard shortcuts
    - **Property 9: Dialog Keyboard Shortcuts**
    - **Validates: Requirements 5.7, 5.8**

  - [x] 12.7 Implement ui_dialog_start and ui_dialog_finish
    - Start: set visible, reset button states
    - Finish: set invisible
    - _Requirements: 5.1_

- [x] 13. Checkpoint - Verify UI_Dialog
  - All code compiles, library builds successfully.

- [x] 14. Create Demo Application
  - [x] 14.1 Create src/ui_demo/main.c with SDL framework setup
    - Use sdl_framework_init from lib/sdl_framework.h
    - Create UI_Surface as backbuffer
    - Initialize Global_Resource with sprite pack paths
    - _Requirements: 9.1_

  - [x] 14.2 Create demo dialog and window manager
    - Create UI_Dialog with OK button
    - Initialize Window_Manager
    - Register dialog with window manager
    - _Requirements: 9.2_

  - [x] 14.3 Implement main loop with event handling
    - Poll SDL events using sdl_framework_poll_events
    - Translate events using ui_event_translate
    - Dispatch events using ui_window_manager_dispatch_event
    - Handle Escape to exit
    - _Requirements: 9.3, 9.6_

  - [x] 14.4 Implement rendering loop
    - Clear backbuffer
    - Call ui_window_manager_process
    - Call ui_window_manager_render
    - Present backbuffer to screen
    - Use sdl_framework_delay for 60 FPS
    - _Requirements: 9.4, 9.7_

  - [x] 14.5 Implement dialog callback
    - Print "OK clicked" on OK button press
    - Set running flag to false to exit
    - _Requirements: 9.5_

- [x] 15. Update CMakeLists.txt
  - [x] 15.1 Add new library source files
    - Add lib/ui_surface.c, lib/ui_window.c, lib/ui_button.c
    - Add lib/ui_button_group.c, lib/ui_event_router.c
    - Add lib/ui_window_manager.c, lib/ui_global_resource.c, lib/ui_dialog.c
    - _Requirements: All_

  - [x] 15.2 Add ui_demo executable target
    - Add src/ui_demo/main.c
    - Link with SDL2 and library
    - _Requirements: 9.1-9.7_

  - [ ] 15.3 Add test targets
    - Add test executables for each component
    - Link with testing framework
    - _Requirements: All property tests_

- [x] 16. Final Checkpoint
  - All code compiles successfully
  - All 197 existing tests pass
  - UIDemo executable builds and is ready to run

## Notes

- Tasks marked with `*` are optional property-based tests that can be skipped for faster MVP
- Each task references specific requirements for traceability
- Checkpoints ensure incremental validation
- Property tests validate universal correctness properties
- Unit tests validate specific examples and edge cases

## Implementation Summary

Created 8 new C source files implementing the pure C UI system:
- `lib/ui_surface.h/c` - SDL texture wrapper for rendering
- `lib/ui_window.h/c` - Base window component with virtual methods
- `lib/ui_button.h/c` - Interactive button with state machine
- `lib/ui_button_group.h/c` - Button container with event routing
- `lib/ui_event_router.h/c` - SDL to legacy message translation
- `lib/ui_window_manager.h/c` - Z-order and event dispatch management
- `lib/ui_global_resource.h/c` - Shared sprite resources
- `lib/ui_dialog.h/c` - Modal dialog with OK/Cancel buttons

Demo application: `src/ui_demo/main.c`
