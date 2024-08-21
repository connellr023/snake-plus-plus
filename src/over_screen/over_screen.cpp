#include "over_screen.hpp"
#include "../keycodes.hpp"

OverScreen::OverScreen(FrameBufferImpl &fb) : fb(fb) {
    this->fb.register_keypress_listener(KEY_ENTER, [this]() {
        this->is_running = false;
    });

    // For now
    this->fb.fill_screen(0xFF0000);
}

OverScreen::~OverScreen() {
    this->fb.clear_keypress_listeners();
}
