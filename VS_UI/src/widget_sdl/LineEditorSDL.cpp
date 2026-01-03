#include "LineEditorSDL.h"

#include <algorithm>
#include <cassert>

// -----------------------------
// UTF-8 helpers
// -----------------------------
int utf8_prev(const std::string& s, int byte_pos) {
    if (byte_pos <= 0) return 0;
    int p = byte_pos - 1;
    while (p > 0 && ((s[p] & 0xC0) == 0x80)) --p;
    return p;
}

int utf8_next(const std::string& s, int byte_pos) {
    if (byte_pos >= static_cast<int>(s.size())) return static_cast<int>(s.size());
    int p = byte_pos + 1;
    while (p < static_cast<int>(s.size()) && ((s[p] & 0xC0) == 0x80)) ++p;
    return p;
}

int utf8_len(const std::string& s) {
    int cnt = 0;
    for (int i = 0; i < static_cast<int>(s.size());) {
        i = utf8_next(s, i);
        ++cnt;
    }
    return cnt;
}

// -----------------------------
// LineEditorSDL
// -----------------------------
LineEditorSDL::LineEditorSDL(SDL_Renderer* renderer, TTF_Font* font)
    : renderer_(renderer), font_(font) {}

void LineEditorSDL::set_text(const std::string& t) {
    text_ = t;
    cursor_cp_ = utf8_len(text_);
    comp_.clear();
    comp_start_bytes_ = 0;
}

void LineEditorSDL::acquire() {
    if (!acquired_) {
        SDL_StartTextInput();
        acquired_ = true;
    }
}

void LineEditorSDL::unacquire() {
    if (acquired_) {
        SDL_StopTextInput();
        acquired_ = false;
    }
    comp_.clear();
    comp_start_bytes_ = 0;
}

void LineEditorSDL::insert_text_utf8(const std::string& s) {
    int cur_byte = 0;
    for (int i = 0; i < cursor_cp_; ++i) cur_byte = utf8_next(text_, cur_byte);

    int add_bytes = static_cast<int>(s.size());
    int add_cps = utf8_len(s);
    if (!check_limits(add_bytes, add_cps)) return;

    text_.insert(cur_byte, s);
    cursor_cp_ += add_cps;
    commit_composition();
}

void LineEditorSDL::replace_composition(const std::string& comp, int /*start*/, int /*len*/) {
    comp_ = comp;
    // composition starts at current cursor byte
    comp_start_bytes_ = 0;
    for (int i = 0; i < cursor_cp_; ++i) comp_start_bytes_ = utf8_next(text_, comp_start_bytes_);
}

void LineEditorSDL::commit_composition() {
    comp_.clear();
    comp_start_bytes_ = 0;
}

void LineEditorSDL::move_left() {
    if (cursor_cp_ > 0) --cursor_cp_;
}

void LineEditorSDL::move_right() {
    int total = utf8_len(text_);
    if (cursor_cp_ < total) ++cursor_cp_;
}

void LineEditorSDL::move_home() { cursor_cp_ = 0; }
void LineEditorSDL::move_end() { cursor_cp_ = utf8_len(text_); }

void LineEditorSDL::backspace() {
    if (cursor_cp_ == 0) return;
    int cur = 0;
    for (int i = 0; i < cursor_cp_; ++i) cur = utf8_next(text_, cur);
    int prev = utf8_prev(text_, cur);
    text_.erase(prev, cur - prev);
    --cursor_cp_;
}

void LineEditorSDL::del() {
    int cur = 0;
    for (int i = 0; i < cursor_cp_; ++i) cur = utf8_next(text_, cur);
    if (cur >= static_cast<int>(text_.size())) return;
    int nxt = utf8_next(text_, cur);
    text_.erase(cur, nxt - cur);
}

bool LineEditorSDL::check_limits(int add_bytes, int add_cps) const {
    if (max_bytes_ >= 0 && static_cast<int>(text_.size()) + add_bytes > max_bytes_) return false;
    if (max_chars_ >= 0 && utf8_len(text_) + add_cps > max_chars_) return false;
    return true;
}

bool LineEditorSDL::handle_event(const UI_Event& e) {
    if (!acquired_) return false;

    if (e.message == WM_KEYDOWN) {
        switch (e.key) {
            case SDLK_LEFT: move_left(); return true;
            case SDLK_RIGHT: move_right(); return true;
            case SDLK_HOME: move_home(); return true;
            case SDLK_END: move_end(); return true;
            case SDLK_BACKSPACE: backspace(); return true;
            case SDLK_DELETE: del(); return true;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                if (style_.line_gap > 0) insert_text_utf8("\n");
                return true;
            default: break;
        }
    } else if (e.message == WM_CHAR) {
        char c = static_cast<char>(e.key & 0xFF);
        if (c >= 32) {
            std::string s(1, c);
            insert_text_utf8(s);
            return true;
        }
    }
    return false;
}

bool LineEditorSDL::handle_sdl_event(const SDL_Event& e) {
    if (!acquired_) return false;

    switch (e.type) {
        case SDL_TEXTINPUT:
            insert_text_utf8(e.text.text);
            return true;
        case SDL_TEXTEDITING:
            replace_composition(e.edit.text, e.edit.start, e.edit.length);
            return true;
        default:
            break;
    }
    return false;
}

std::vector<LineEditorSDL::LineLayout> LineEditorSDL::layout_text(SDL_Renderer* r, const std::string& visible_text, const std::string& visible_comp, int* caret_px, int* caret_py) {
    std::vector<LineLayout> lines;
    const std::string* src = &visible_text;
    std::string merged;

    // compute caret byte in visible text (password masked text matches codepoint length)
    int caret_byte = 0;
    for (int i = 0; i < cursor_cp_; ++i) caret_byte = utf8_next(visible_text, caret_byte);

    if (!visible_comp.empty()) {
        merged = visible_text;
        merged.insert(caret_byte, visible_comp);
        src = &merged;
    }

    const int max_w = style_.abs_width;
    const int line_height = (style_.line_gap > 0) ? style_.line_gap : TTF_FontHeight(font_);
    int pos = 0;
    int line_start = 0;
    int produced_lines = 0;

    while (pos < static_cast<int>(src->size())) {
        int last_fit = line_start;
        int scan = line_start;
        int w = 0;
        while (scan < static_cast<int>(src->size())) {
            int next = utf8_next(*src, scan);
            std::string trial = src->substr(line_start, next - line_start);
            int tw = 0, th = 0;
            TTF_SizeUTF8(font_, trial.c_str(), &tw, &th);
            if (max_w > 0 && tw > max_w) break;
            last_fit = next;
            w = tw;
            scan = next;
            if (style_.line_gap > 0 && (*src)[next - 1] == '\n') break;
            if (style_.line_gap == 0) break; // single line
        }
        if (last_fit == line_start) { // fallback: force one codepoint
            last_fit = utf8_next(*src, line_start);
            TTF_SizeUTF8(font_, src->substr(line_start, last_fit - line_start).c_str(), &w, nullptr);
        }

        lines.push_back({line_start, last_fit, w});
        ++produced_lines;

        line_start = last_fit;
        pos = last_fit;

        if (style_.line_gap == 0) break; // single line
        if (style_.max_lines > 0 && produced_lines >= style_.max_lines) break;
    }

    // caret pixel
    if (caret_px && caret_py) {
        *caret_px = x_;
        *caret_py = y_;
        int y_off = 0;
        int caret_in_src = caret_byte + static_cast<int>(visible_comp.size());
        for (auto& ln : lines) {
            if (caret_in_src >= ln.byte_start && caret_in_src <= ln.byte_end) {
                std::string sub = src->substr(ln.byte_start, caret_in_src - ln.byte_start);
                int tw = 0, th = 0;
                TTF_SizeUTF8(font_, sub.c_str(), &tw, &th);
                *caret_px = x_ + tw;
                *caret_py = y_ + y_off;
                break;
            }
            y_off += line_height;
        }
    }

    return lines;
}

void LineEditorSDL::update_ime_rect(SDL_Renderer* r) {
    if (!acquired_) return;

    auto mask = [&](const std::string& s) {
        if (!style_.password_mode) return s;
        return std::string(utf8_len(s), '*');
    };
    std::string vis_text = mask(text_);
    std::string vis_comp = mask(comp_);

    int px = x_, py = y_;
    layout_text(r, vis_text, vis_comp, &px, &py);
    SDL_Rect rc{px, py, 32, (style_.line_gap > 0 ? style_.line_gap : TTF_FontHeight(font_))};
    SDL_SetTextInputRect(&rc);
}

void LineEditorSDL::render(SDL_Renderer* r) {
    assert(font_ && r);

    update_ime_rect(r);

    // Prepare source text (with password masking)
    auto mask_text = [&](const std::string& s) {
        if (!style_.password_mode) return s;
        int cps = utf8_len(s);
        return std::string(cps, '*');
    };

    std::string src_text = mask_text(text_);
    std::string comp_masked = mask_text(comp_);

    // Layout: base text for rendering; comp for caret/IME positioning
    int caret_px = x_, caret_py = y_;
    auto lines = layout_text(r, src_text, std::string{}, nullptr, nullptr);
    layout_text(r, src_text, comp_masked, &caret_px, &caret_py);

    const int line_height = (style_.line_gap > 0) ? style_.line_gap : TTF_FontHeight(font_);
    int y_off = 0;
    for (auto& ln : lines) {
        std::string sub = src_text.substr(ln.byte_start, ln.byte_end - ln.byte_start);
        SDL_Surface* surf = TTF_RenderUTF8_Blended(font_, sub.c_str(), style_.text_color);
        if (!surf) continue;
        SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
        SDL_Rect dst{x_, y_ + y_off, surf->w, surf->h};
        SDL_RenderCopy(r, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
        SDL_FreeSurface(surf);
        y_off += line_height;
        if (style_.line_gap == 0) break;
    }

    // Render composition (if any)
    if (!comp_.empty()) {
        // caret position already accounts for composition in layout(true,...)
        SDL_Surface* cs = TTF_RenderUTF8_Blended(font_, comp_masked.c_str(), style_.ime_text_color);
        if (cs) {
            SDL_Texture* ct = SDL_CreateTextureFromSurface(r, cs);
            SDL_Rect cdst{caret_px, caret_py, cs->w, cs->h};
            SDL_SetRenderDrawColor(r, style_.ime_bg_color.r, style_.ime_bg_color.g, style_.ime_bg_color.b, style_.ime_bg_color.a);
            SDL_RenderFillRect(r, &cdst);
            SDL_RenderCopy(r, ct, nullptr, &cdst);
            SDL_DestroyTexture(ct);
            SDL_FreeSurface(cs);
        }
    }

    // Caret
    SDL_SetRenderDrawColor(r, style_.caret_color.r, style_.caret_color.g, style_.caret_color.b, style_.caret_color.a);
    SDL_RenderDrawLine(r, caret_px, caret_py, caret_px, caret_py + line_height - 2);
}