#include <cassert>
#include "start_screen.hpp"
#include "../keycodes.hpp"
#include "../rendering/colors.hpp"
#include "../rendering/rendering.hpp"
#include "../utils.hpp"

StartScreen::StartScreen(FrameBufferImpl &fb) : fb(fb) {
    this->fb.register_keypress_listener(KEY_UP, [this]() {
        this->update_selection((this->selected_option - 1 + StartScreen::option_count) % StartScreen::option_count);
    });

    this->fb.register_keypress_listener(KEY_DOWN, [this]() {
        this->update_selection((this->selected_option + 1) % StartScreen::option_count);
    });

    this->fb.register_keypress_listener(KEY_ENTER, [this]() {
        this->options[this->selected_option].action();
    });

    this->title_text_x = this->fb.calc_center_x(((title_text_buf_size - 1) * TITLE_TEXT_SPACING) + ((title_text_buf_size - 1) * 8 * TITLE_TEXT_SCALE));
    draw_title_text();
}

void StartScreen::update_selection(uint8_t new_selection) {
    assert(new_selection < StartScreen::option_count);

    this->selected_option = new_selection;
}

void StartScreen::draw_title_text() {
    for (uint8_t i = 0; i < title_text_buf_size - 1; i++) {
        const uint32_t color = (i == this->title_text_effect_idx) ? TITLE_TEXT_COLOR_1 : TITLE_TEXT_COLOR_2;
        draw_char(this->fb, this->title_text_x + (i * TITLE_TEXT_SPACING) + (i * 8 * TITLE_TEXT_SCALE), TITLE_TEXT_Y, TITLE_TEXT_SCALE, color, title_text[i]);
    }
}

void StartScreen::update() {
    const uint32_t current_ms = current_millis();

    if (current_ms - this->title_text_last_update_ms > TITLE_TEXT_MS) {
        this->title_text_last_update_ms = current_ms;
        this->title_text_effect_idx = (this->title_text_effect_idx + 1) % (title_text_buf_size - 1);

        draw_title_text();
    }
}

void StartScreen::start_game() {
    this->fb.fill_screen(BACKGROUND_COLOR_1);
    this->is_running = false;
}

void StartScreen::quit_game() {
    this->fb.set_running(false);
}

StartScreen::~StartScreen() {
    this->fb.clear_keypress_listeners();
}
