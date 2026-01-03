#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "ui_surface.h"
#include "ui_event_router.h"

// UTF-8 helpers: codepoint-aware movement and length
int utf8_prev(const std::string& s, int byte_pos);
int utf8_next(const std::string& s, int byte_pos);
int utf8_len(const std::string& s);

struct LineEditorStyle {
    SDL_Color text_color{255, 255, 255, 255};
    SDL_Color caret_color{255, 255, 255, 255};
    SDL_Color ime_bg_color{64, 64, 64, 128};
    SDL_Color ime_text_color{0, 0, 0, 255};
    int line_gap = 0;        // 0 = single line; >0 = multiline line height
    int max_lines = 0;       // 0 = unlimited
    int abs_width = -1;      // pixel width limit; -1 = unlimited
    bool password_mode = false;
};

/**
 * SDL-based line editor (UTF-8, SDL_ttf rendering, SDL text/IME input).
 * - Handles SDL_KEYDOWN navigation, SDL_TEXTINPUT, SDL_TEXTEDITING.
 * - Maintains cursor in codepoints.
 * - Supports password mode, width/line limits, IME caret rect.
 */
class LineEditorSDL {
public:
    LineEditorSDL(SDL_Renderer* renderer, TTF_Font* font);

    // Configuration
    void set_position(int x, int y) { x_ = x; y_ = y; }
    void set_style(const LineEditorStyle& s) { style_ = s; }
    void set_max_chars(int n) { max_chars_ = n; }
    void set_max_bytes(int n) { max_bytes_ = n; }

    // Event handling (legacy UI_Event or raw SDL_Event)
    bool handle_event(const UI_Event& e);
    bool handle_sdl_event(const SDL_Event& e);

    // Rendering
    void render(SDL_Renderer* r);

    // Text access
    const std::string& text() const { return text_; }
    void set_text(const std::string& t);
    int cursor_cp() const { return cursor_cp_; }

    // Input focus
    void acquire();   // starts SDL text input
    void unacquire(); // stops SDL text input
    bool acquired() const { return acquired_; }

private:
    // Internal helpers
    void insert_text_utf8(const std::string& s);
    void replace_composition(const std::string& comp, int start, int len);
    void commit_composition();
    void move_left();
    void move_right();
    void move_home();
    void move_end();
    void backspace();
    void del();
    bool check_limits(int add_bytes, int add_cps) const;
    void update_ime_rect(SDL_Renderer* r);

    struct LineLayout { int byte_start; int byte_end; int w; };
    std::vector<LineLayout> layout_text(SDL_Renderer* r, const std::string& visible_text, const std::string& visible_comp, int* caret_px = nullptr, int* caret_py = nullptr);

private:
    SDL_Renderer* renderer_{nullptr};
    TTF_Font* font_{nullptr};
    LineEditorStyle style_;

    std::string text_;           // committed UTF-8 text
    std::string comp_;           // composing text (SDL_TEXTEDITING)
    int comp_start_bytes_{0};    // byte offset of composition start
    int cursor_cp_{0};           // cursor position in codepoints
    bool acquired_{false};

    int x_{0}, y_{0};
    int max_chars_{-1};
    int max_bytes_{-1};
};