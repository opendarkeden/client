#pragma once

#include <functional>
#include <SDL.h>
#include "ButtonSDL.h"
#include "ui_surface.h"
#include "spritepack.h"
#include "sprite.h"
#include "ui_event_router.h"

/**
 * @brief Minimal SDL-based Game Menu demo (Option / Logout / Continue).
 *
 * Usage:
 *   GameMenuSDL menu(renderer, surface, pack,
 *                    0, // background sprite index
 *                    { {opt_x,opt_y,opt_w,opt_h},
 *                      {logout_x,logout_y,logout_w,logout_h},
 *                      {cont_x,cont_y,cont_w,cont_h} });
 *   menu.set_on_select([](GameMenuSDL::Menu id){ ... });
 *   menu.handle_event(ui_event);        // from ui_event_router
 *   menu.handle_sdl_event(sdl_event);   // for completeness (mouse only)
 *   menu.render();                      // draws background + buttons
 */
class GameMenuSDL {
public:
    enum class Menu {
        OPTION   = 0,
        LOGOUT   = 1,
        CONTINUE = 2,
        INVALID  = 999
    };

    struct ButtonLayout {
        SDL_Rect option;
        SDL_Rect logout;
        SDL_Rect cont;
    };

    GameMenuSDL(SDL_Renderer* renderer,
                UI_Surface* surface,
                SpritePack* pack,
                int background_sprite_index,
                const ButtonLayout& layout)
        : renderer_(renderer)
        , surface_(surface)
        , pack_(pack)
        , bg_index_(background_sprite_index)
        , bg_loaded_(false)
        , buttons_{ ButtonSDL(layout.option.x, layout.option.y, layout.option.w, layout.option.h, static_cast<int>(Menu::OPTION)),
                    ButtonSDL(layout.logout.x, layout.logout.y, layout.logout.w, layout.logout.h, static_cast<int>(Menu::LOGOUT)),
                    ButtonSDL(layout.cont.x, layout.cont.y, layout.cont.w, layout.cont.h, static_cast<int>(Menu::CONTINUE)) } {
        s_instance_ = this; // demo: single instance callback bridge
        for (auto& b : buttons_) {
            b.set_on_press(&GameMenuSDL::OnPressThunk, nullptr);
        }
    }

    ~GameMenuSDL() {
        if (bg_loaded_) {
            decoded_sprite_free(&bg_decoded_);
            bg_loaded_ = false;
        }
    }

    // Set top-left position in screen space (affects button hit-testing)
    void set_position(int x, int y) { origin_x_ = x; origin_y_ = y; }

    // Install callback invoked when a button is pressed
    void set_on_select(std::function<void(Menu)> cb) { on_select_ = std::move(cb); }

    // Handle UI_Event (mouse from ui_event_router, keyboard not used here)
    bool handle_event(const UI_Event& e) {
        if (!ui_event_is_mouse(const_cast<UI_Event*>(&e))) return false;
        bool consumed = false;
        for (auto& b : buttons_) {
            consumed |= b.handle(e, origin_x_, origin_y_);
        }
        return consumed;
    }

    // Optional: handle raw SDL events (mouse only)
    bool handle_sdl_event(const SDL_Event& ev) {
        UI_Event ue{};
        if (!ui_event_translate(const_cast<SDL_Event*>(&ev), &ue)) return false;
        return handle_event(ue);
    }

    // Render background sprite (if any) then buttons (fallback rectangles)
    void render() {
        if (!renderer_ || !surface_) return;
        if (pack_ && bg_index_ >= 0) {
            if (!bg_loaded_) {
                Sprite* spr = spritepack_get(pack_, static_cast<uint16_t>(bg_index_));
                if (spr && spr->is_valid && sprite_decode(spr, &bg_decoded_, 0) == 0) {
                    decoded_sprite_create_texture(&bg_decoded_, renderer_);
                    bg_loaded_ = true;
                }
            }
            if (bg_loaded_) {
                ui_surface_blit_sprite(surface_, origin_x_, origin_y_, &bg_decoded_);
            }
        }
        for (auto& b : buttons_) {
            b.render(surface_, pack_, origin_x_, origin_y_);
        }
    }

private:
    static void OnPressThunk(UI_Button*, int id) {
        if (!s_instance_) return;
        s_instance_->dispatch_press(static_cast<Menu>(id));
    }

    void dispatch_press(Menu m) {
        if (on_select_) on_select_(m);
    }

private:
    SDL_Renderer* renderer_{nullptr};
    UI_Surface* surface_{nullptr};
    SpritePack* pack_{nullptr};
    int bg_index_{-1};
    DecodedSprite bg_decoded_{};
    bool bg_loaded_{false};
    int origin_x_{0}, origin_y_{0};

    ButtonSDL buttons_[3];
    std::function<void(Menu)> on_select_;

    static GameMenuSDL* s_instance_;
};

// Static storage
inline GameMenuSDL* GameMenuSDL::s_instance_ = nullptr;