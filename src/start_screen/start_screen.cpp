#include <cassert>
#include "start_screen.hpp"
#include "../keycodes.hpp"
#include "../rendering/colors.hpp"

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
}

void StartScreen::update_selection(uint8_t new_selection) {
    assert(new_selection < StartScreen::option_count);

    this->selected_option = new_selection;
}

void StartScreen::update() {

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
