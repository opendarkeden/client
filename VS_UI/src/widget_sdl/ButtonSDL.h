#pragma once

#include "ui_button.h"
#include "ui_surface.h"
#include "ui_event_router.h"
#include "spritepack.h"

/**
 * @brief C++ wrapper around the C-based UI_Button (lib/ui).
 *
 * Features:
 * - Initialize with geometry/id.
 * - Bind press callback and custom render callback.
 * - Handle mouse events via UI_Event (from ui_event_router).
 * - Render using ui_button_render / ui_button_render_fallback.
 */
class ButtonSDL {
public:
    ButtonSDL(int x, int y, int w, int h, int id) { ui_button_init(&btn_, x, y, w, h, id); }
    ~ButtonSDL() { ui_button_free(&btn_); }

    void set_on_press(UI_ButtonOnPressCallback cb, void* data) { ui_button_set_on_press(&btn_, cb, data); }
    void set_custom_render(UI_ButtonRenderCallback cb, void* data) { ui_button_set_show_widget(&btn_, cb, data); }

    // Event handling: parent_x/parent_y are the window's origin in screen space.
    bool handle(const UI_Event& e, int parent_x, int parent_y) {
        if (!ui_event_is_mouse(const_cast<UI_Event*>(&e))) return false;
        int relx = e.x - parent_x;
        int rely = e.y - parent_y;
        return ui_button_mouse_control(&btn_, e.message, relx, rely) != 0;
    }

    // Rendering: parent_x/parent_y are the window's origin in screen space.
    void render(UI_Surface* surface, SpritePack* pack, int parent_x, int parent_y) {
        ui_button_render(&btn_, surface, pack, parent_x, parent_y);
    }

    // Utility: reset states (focus/pressed)
    void reset() { ui_button_reset(&btn_); }

    // Access to underlying C struct if needed
    UI_Button* raw() { return &btn_; }
    const UI_Button* raw() const { return &btn_; }

private:
    UI_Button btn_{};
};