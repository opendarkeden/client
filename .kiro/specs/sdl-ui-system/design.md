# Design Document: SDL UI System

## Overview

This design document describes a pure C UI component system for SDL2 that replaces the existing C++ SDL shim demo. The system is designed to be modular, allowing incremental porting of legacy DarkEden DirectX UI components to SDL2.

The architecture follows a layered approach:
1. **Low-level**: UI_Surface for rendering primitives
2. **Core**: UI_Window, UI_Button as base components
3. **Composite**: Button_Group, UI_Dialog built from core components
4. **Management**: Window_Manager, Event_Router for system coordination
5. **Resources**: Global_Resource for shared assets

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                      Demo Application                        │
├─────────────────────────────────────────────────────────────┤
│  Window_Manager          │         Event_Router             │
│  - window list           │         - SDL event translation  │
│  - z-order management    │         - dispatch to windows    │
├─────────────────────────────────────────────────────────────┤
│  UI_Dialog               │         Global_Resource          │
│  - message display       │         - dialog sprites         │
│  - OK/Cancel buttons     │         - button sprites         │
├─────────────────────────────────────────────────────────────┤
│  UI_Window               │         Button_Group             │
│  - base window struct    │         - button container       │
│  - event handlers        │         - event routing          │
├─────────────────────────────────────────────────────────────┤
│  UI_Button               │         UI_Surface               │
│  - interactive element   │         - SDL texture wrapper    │
│  - state management      │         - sprite blitting        │
├─────────────────────────────────────────────────────────────┤
│                    lib/sdl_framework.h                       │
│                    lib/spritepack.h                          │
│                    lib/sprite.h                              │
└─────────────────────────────────────────────────────────────┘
```

## Components and Interfaces

### UI_Surface (lib/ui_surface.h)

```c
typedef struct {
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int width;
    int height;
    int pitch;
    void* pixels;       /* Valid only when locked */
    int locked;
} UI_Surface;

/* Lifecycle */
int ui_surface_init(UI_Surface* surface, SDL_Renderer* renderer, int width, int height);
void ui_surface_free(UI_Surface* surface);

/* Lock/Unlock for pixel access */
int ui_surface_lock(UI_Surface* surface);
int ui_surface_unlock(UI_Surface* surface);

/* Sprite blitting */
void ui_surface_blit_sprite(UI_Surface* surface, int x, int y, DecodedSprite* sprite);
void ui_surface_blit_sprite_alpha(UI_Surface* surface, int x, int y, DecodedSprite* sprite, uint8_t alpha);
void ui_surface_blit_sprite_color(UI_Surface* surface, int x, int y, DecodedSprite* sprite, uint32_t color);
void ui_surface_blit_sprite_darkness(UI_Surface* surface, int x, int y, DecodedSprite* sprite, uint8_t darkness);

/* Primitive drawing */
void ui_surface_fill_rect(UI_Surface* surface, SDL_Rect* rect, uint32_t color);
void ui_surface_hline(UI_Surface* surface, int x, int y, int length, uint32_t color);
void ui_surface_vline(UI_Surface* surface, int x, int y, int length, uint32_t color);
void ui_surface_draw_rect(UI_Surface* surface, SDL_Rect* rect, uint32_t color);
```

### UI_Window (lib/ui_window.h)

```c
/* Legacy message types (from i_signal.h) */
typedef enum {
    M_MOVING = 1,
    M_LEFTBUTTON_DOWN,
    M_LEFTBUTTON_UP,
    M_RIGHTBUTTON_DOWN,
    M_RIGHTBUTTON_UP,
    M_LB_DOUBLECLICK,
    M_RB_DOUBLECLICK,
    M_WHEEL_UP,
    M_WHEEL_DOWN
} UI_MouseMessage;

#define WM_KEYDOWN 0x0100
#define WM_KEYUP   0x0101
#define WM_CHAR    0x0102

/* Window attributes */
typedef struct {
    int visible;
    int topmost;
    int pinned;
    int alpha;          /* 0-255 transparency */
} UI_WindowAttr;

/* Forward declaration */
typedef struct UI_Window UI_Window;

/* Function pointer types for virtual methods */
typedef void (*UI_ShowFunc)(UI_Window* window);
typedef void (*UI_ProcessFunc)(UI_Window* window);
typedef int (*UI_MouseControlFunc)(UI_Window* window, uint32_t message, int x, int y);
typedef void (*UI_KeyboardControlFunc)(UI_Window* window, uint32_t message, uint32_t key, long extra);
typedef int (*UI_IsPixelFunc)(UI_Window* window, int x, int y);

struct UI_Window {
    int x, y;           /* Position */
    int w, h;           /* Dimensions */
    UI_WindowAttr attr;
    void* user_data;    /* For derived types */
    
    /* Virtual methods */
    UI_ShowFunc show;
    UI_ProcessFunc process;
    UI_MouseControlFunc mouse_control;
    UI_KeyboardControlFunc keyboard_control;
    UI_IsPixelFunc is_pixel;
};

/* Lifecycle */
void ui_window_init(UI_Window* window, int x, int y, int w, int h);
void ui_window_set_rect(UI_Window* window, int x, int y, int w, int h);

/* Attribute helpers */
void ui_window_set_visible(UI_Window* window, int visible);
void ui_window_set_topmost(UI_Window* window, int topmost);
void ui_window_set_pinned(UI_Window* window, int pinned);

/* Default implementations */
int ui_window_default_is_pixel(UI_Window* window, int x, int y);
int ui_window_default_mouse_control(UI_Window* window, uint32_t message, int x, int y);
```

### UI_Button (lib/ui_button.h)

```c
typedef struct UI_Button UI_Button;
typedef void (*UI_ButtonCallback)(UI_Button* button, int id);

struct UI_Button {
    int x, y;           /* Position relative to parent */
    int w, h;           /* Dimensions */
    int id;             /* Button identifier */
    int focus;          /* Mouse hovering */
    int pressed;        /* Mouse button down */
    int sprite_index;   /* Base sprite index in pack */
    UI_ButtonCallback callback;
    void* user_data;
};

/* Lifecycle */
void ui_button_init(UI_Button* button, int x, int y, int w, int h, int id);
void ui_button_free(UI_Button* button);

/* State queries */
int ui_button_get_focus(UI_Button* button);
int ui_button_get_pressed(UI_Button* button);

/* Event handling - returns 1 if event consumed */
int ui_button_mouse_control(UI_Button* button, uint32_t message, int x, int y);

/* Rendering */
void ui_button_render(UI_Button* button, UI_Surface* surface, SpritePack* pack, int parent_x, int parent_y);
void ui_button_render_fallback(UI_Button* button, UI_Surface* surface, int parent_x, int parent_y);
```

### Button_Group (lib/ui_button_group.h)

```c
#define MAX_BUTTONS_PER_GROUP 16

typedef struct {
    UI_Button* buttons[MAX_BUTTONS_PER_GROUP];
    int count;
    UI_Window* parent;
} UI_ButtonGroup;

/* Lifecycle */
void ui_button_group_init(UI_ButtonGroup* group, UI_Window* parent);
void ui_button_group_free(UI_ButtonGroup* group);

/* Button management */
int ui_button_group_add(UI_ButtonGroup* group, UI_Button* button);
int ui_button_group_remove(UI_ButtonGroup* group, int id);
UI_Button* ui_button_group_get(UI_ButtonGroup* group, int id);

/* Reset all button states */
void ui_button_group_reset(UI_ButtonGroup* group);

/* Event routing - returns 1 if any button consumed event */
int ui_button_group_mouse_control(UI_ButtonGroup* group, uint32_t message, int x, int y);

/* Rendering */
void ui_button_group_render(UI_ButtonGroup* group, UI_Surface* surface, SpritePack* pack);
```

### UI_Dialog (lib/ui_dialog.h)

```c
/* Dialog button flags */
#define DIALOG_OK     0x01
#define DIALOG_CANCEL 0x02

/* Dialog execution IDs */
#define DIALOG_EXECID_OK     1
#define DIALOG_EXECID_CANCEL 2

typedef struct UI_Dialog UI_Dialog;
typedef void (*UI_DialogCallback)(UI_Dialog* dialog, int exec_id);

struct UI_Dialog {
    UI_Window window;           /* Base window (must be first) */
    UI_ButtonGroup buttons;
    UI_DialogCallback callback;
    int button_flags;
    char* message;
    void* resources;            /* Global_Resource pointer */
};

/* Lifecycle */
UI_Dialog* ui_dialog_create(int x, int y, int w, int h, int button_flags, UI_DialogCallback callback);
void ui_dialog_destroy(UI_Dialog* dialog);

/* Configuration */
void ui_dialog_set_message(UI_Dialog* dialog, const char* message);
void ui_dialog_set_resources(UI_Dialog* dialog, void* resources);

/* Control */
void ui_dialog_start(UI_Dialog* dialog);
void ui_dialog_finish(UI_Dialog* dialog);

/* Inherited from UI_Window - implemented internally */
/* show, process, mouse_control, keyboard_control */
```

### Global_Resource (lib/ui_global_resource.h)

```c
/* Sprite indices for dialog box assembly */
#define AB_BACK       0
#define AB_RIGHT      2
#define AB_DOWN       3
#define AB_RIGHTDOWN  4

/* Sprite indices for buttons */
#define AB_BUTTON_OK           0
#define AB_BUTTON_CANCEL       1
#define AB_BUTTON_HILIGHTED_OFFSET 3
#define AB_BUTTON_PUSHED_OFFSET    6

typedef struct {
    SDL_Renderer* renderer;
    UI_Surface* surface;        /* Target surface for drawing */
    SpritePack dialog_pack;
    SpritePack button_pack;
    int initialized;
} UI_GlobalResource;

/* Lifecycle */
int ui_global_resource_init(UI_GlobalResource* gr, SDL_Renderer* renderer, UI_Surface* surface,
                            const char* dialog_pack_path, const char* button_pack_path);
void ui_global_resource_free(UI_GlobalResource* gr);

/* Drawing */
void ui_global_resource_draw_dialog(UI_GlobalResource* gr, int x, int y, int w, int h, int use_alpha);
void ui_global_resource_draw_outbox(UI_GlobalResource* gr, int x, int y, int w, int h);

/* Accessors */
SpritePack* ui_global_resource_get_button_pack(UI_GlobalResource* gr);
SpritePack* ui_global_resource_get_dialog_pack(UI_GlobalResource* gr);
```

### Event_Router (lib/ui_event_router.h)

```c
typedef struct {
    uint32_t message;
    int x, y;           /* Mouse position */
    uint32_t key;       /* Key code for keyboard events */
    long extra;         /* Additional data */
} UI_Event;

/* Translate SDL event to UI event */
int ui_event_translate(SDL_Event* sdl_event, UI_Event* ui_event);

/* Dispatch event to a window - returns 1 if consumed */
int ui_event_dispatch_to_window(UI_Event* event, UI_Window* window);
```

### Window_Manager (lib/ui_window_manager.h)

```c
#define MAX_WINDOWS 32

typedef struct {
    UI_Window* windows[MAX_WINDOWS];
    int count;
    UI_Surface* surface;
} UI_WindowManager;

/* Lifecycle */
void ui_window_manager_init(UI_WindowManager* wm, UI_Surface* surface);
void ui_window_manager_free(UI_WindowManager* wm);

/* Window registration */
int ui_window_manager_register(UI_WindowManager* wm, UI_Window* window);
int ui_window_manager_unregister(UI_WindowManager* wm, UI_Window* window);

/* Z-order management */
void ui_window_manager_bring_to_front(UI_WindowManager* wm, UI_Window* window);
void ui_window_manager_appear(UI_WindowManager* wm, UI_Window* window);
void ui_window_manager_disappear(UI_WindowManager* wm, UI_Window* window);

/* Frame processing */
void ui_window_manager_process(UI_WindowManager* wm);
void ui_window_manager_render(UI_WindowManager* wm);

/* Event dispatch - returns 1 if any window consumed event */
int ui_window_manager_dispatch_event(UI_WindowManager* wm, UI_Event* event);
```

## Data Models

### Window Z-Order

Windows are stored in an array where index 0 is the bottom-most window. The `count` field indicates how many windows are registered. When a window is brought to front, it's moved to index `count-1`.

```
windows[0] = bottom window (rendered first)
windows[1] = ...
windows[count-1] = topmost window (rendered last, receives events first)
```

### Button State Machine

```
         ┌─────────────────────────────────────┐
         │                                     │
         ▼                                     │
    ┌─────────┐  mouse_enter   ┌─────────┐    │
    │ NORMAL  │ ─────────────► │  HOVER  │    │
    └─────────┘                └─────────┘    │
         ▲                          │         │
         │ mouse_leave              │ mouse_down
         │                          ▼         │
         │                    ┌─────────┐     │
         └────────────────────│ PRESSED │─────┘
                mouse_up      └─────────┘
              (outside)            │
                                   │ mouse_up (inside)
                                   ▼
                              [CALLBACK]
```

## Correctness Properties

*A property is a characteristic or behavior that should hold true across all valid executions of a system—essentially, a formal statement about what the system should do. Properties serve as the bridge between human-readable specifications and machine-verifiable correctness guarantees.*

### Property 1: Window Attribute Round-Trip
*For any* UI_Window and any combination of attribute values (position, dimensions, visibility, topmost, pinned), setting and then getting those attributes SHALL return the original values.
**Validates: Requirements 2.1, 2.2, 2.3, 2.4**

### Property 2: Button State Consistency
*For any* UI_Button and any mouse position, the focus state SHALL equal whether the position is within the button bounds.
**Validates: Requirements 3.2, 3.4, 3.5**

### Property 3: Button Press-Release Callback
*For any* UI_Button with a callback, when mouse is pressed inside and released inside, the callback SHALL be invoked exactly once.
**Validates: Requirements 3.6, 3.7**

### Property 4: Button Group Count Invariant
*For any* sequence of add/remove operations on a Button_Group, the count SHALL equal the number of successful adds minus the number of successful removes.
**Validates: Requirements 4.1, 4.4, 4.5**

### Property 5: Event Translation Correctness
*For any* SDL event type that has a corresponding UI message type, ui_event_translate SHALL produce the correct UI_Event message type.
**Validates: Requirements 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7**

### Property 6: Window Manager Registration Invariant
*For any* sequence of register/unregister operations on Window_Manager, the count SHALL equal registrations minus unregistrations (clamped to valid range).
**Validates: Requirements 8.1, 8.2, 8.3**

### Property 7: Event Dispatch Order
*For any* Window_Manager with multiple windows, events SHALL be dispatched to windows in reverse z-order (topmost first).
**Validates: Requirements 7.8, 8.5**

### Property 8: Event Consumption Stops Dispatch
*For any* Window_Manager, when a window consumes an event, no lower-z-order windows SHALL receive that event.
**Validates: Requirements 7.9**

### Property 9: Dialog Keyboard Shortcuts
*For any* UI_Dialog, pressing Enter SHALL invoke OK callback and pressing Escape SHALL invoke Cancel callback.
**Validates: Requirements 5.7, 5.8**

### Property 10: Surface Fill Rect Correctness
*For any* UI_Surface and any valid rectangle, ui_surface_fill_rect SHALL set all pixels within the rectangle to the specified color.
**Validates: Requirements 1.7**

## Error Handling

1. **NULL pointer checks**: All public functions check for NULL pointers and return error codes or no-op gracefully.

2. **Resource loading failures**: Global_Resource logs errors but allows fallback rendering (simple rectangles instead of sprites).

3. **Array bounds**: Window_Manager and Button_Group enforce maximum counts and return error codes when limits are exceeded.

4. **SDL errors**: Surface operations check SDL function return values and propagate errors.

## Testing Strategy

### Unit Tests
- Test each component in isolation
- Mock dependencies where needed (e.g., mock SDL_Renderer for surface tests)
- Focus on edge cases: empty groups, NULL pointers, boundary conditions

### Property-Based Tests
- Use a property-based testing library (e.g., theft for C)
- Generate random inputs for state transitions
- Verify invariants hold across many iterations
- Minimum 100 iterations per property test
- Tag format: **Feature: sdl-ui-system, Property {number}: {property_text}**

### Integration Tests
- Demo application serves as integration test
- Manual verification of visual output
- Interactive testing of mouse/keyboard input

### Test File Organization
```
tests/
  test_ui_surface.c      - Surface unit tests
  test_ui_window.c       - Window unit tests  
  test_ui_button.c       - Button unit + property tests
  test_ui_button_group.c - Button group tests
  test_ui_event.c        - Event translation property tests
  test_ui_window_mgr.c   - Window manager property tests
```
