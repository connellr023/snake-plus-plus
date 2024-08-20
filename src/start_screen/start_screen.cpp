#include <cassert>
#include <cstring>
#include "start_screen.hpp"
#include "../keycodes.hpp"
#include "../rendering/colors.hpp"
#include "../rendering/rendering.hpp"
#include "../rendering/sprites.hpp"

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

    this->start_interval(TITLE_TEXT_MS, [this](bool is_active) {
        this->title_text_last_update_ms = TimeManager::current_millis();
        this->title_text_effect_idx = (this->title_text_effect_idx + 1) % (title_text_buf_size - 1);

        this->draw_title_text();
    });

    this->title_text_x = this->fb.calc_center_x(((title_text_buf_size - 1) * TITLE_TEXT_SPACING) + ((title_text_buf_size - 1) * 8 * TITLE_TEXT_SCALE));
    this->draw_title_text();

    this->options[0] = {
        .name = start_option_text,
        .x = this->fb.calc_center_x(((start_option_text_buf_size - 1) * OPTION_TEXT_SPACING) + ((start_option_text_buf_size - 1) * 8 * OPTION_TEXT_SCALE)),
        .y = START_OPTION_Y,
        .action = [this]() { this->start_game(); }
    };

    this->options[1] = {
        .name = quit_option_text,
        .x = this->fb.calc_center_x(((quit_option_text_buf_size - 1) * OPTION_TEXT_SPACING) + ((quit_option_text_buf_size - 1) * 8 * OPTION_TEXT_SCALE)),
        .y = QUIT_OPTION_Y,
        .action = [this]() { this->quit_game(); }
    };

    this->draw_options();
}

void StartScreen::draw_options() {
    for (auto option : this->options) {
        uint32_t option_color = OPTION_TEXT_COLOR;
        uint32_t arrow_color = BACKGROUND_COLOR_1;

        if (option == this->options[this->selected_option]) {
            option_color = SELECTED_OPTION_COLOR;
            arrow_color = SELECTED_OPTION_COLOR;
        }

        draw_sprite(this->fb, option.x - (8 * OPTION_TEXT_SCALE) - (OPTION_TEXT_SPACING / 2), option.y, OPTION_TEXT_SCALE, arrow_color, BACKGROUND_COLOR_1, SPRITE_SELECT_ARROW, orientation_normal);
        draw_string(this->fb, option.x, option.y, OPTION_TEXT_SCALE, OPTION_TEXT_SPACING, option_color, option.name);
    }
}

void StartScreen::update_selection(uint8_t new_selection) {
    assert(new_selection < StartScreen::option_count);

    this->selected_option = new_selection;
    this->draw_options();
}

void StartScreen::draw_title_text() {
    for (uint8_t i = 0; i < title_text_buf_size - 1; i++) {
        const uint32_t color = (i == this->title_text_effect_idx) ? TITLE_TEXT_COLOR_1 : TITLE_TEXT_COLOR_2;
        draw_char(this->fb, this->title_text_x + (i * TITLE_TEXT_SPACING) + (i * 8 * TITLE_TEXT_SCALE), TITLE_TEXT_Y, TITLE_TEXT_SCALE, color, title_text[i]);
    }
}

void StartScreen::update() {
    this->update_interval_listeners();
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
