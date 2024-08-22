#include "over_screen.hpp"
#include "../keycodes.hpp"
#include "../rendering/rendering.hpp"

OverScreen::OverScreen(FrameBufferImpl &fb) : fb(fb) {
    this->fb.register_keypress_listener(KEY_ENTER, [this]() {
        this->is_running = false;
    });

    this->draw();
}

void OverScreen::draw() {
    constexpr int message_text_scale = 9;
    constexpr int message_text_spacing = 2;
    constexpr int message_buffer_size = 14;
    const char message[message_buffer_size] = "ENTER TO EXIT";

    const int message_width = (message_buffer_size - 1) * 8 * message_text_scale + (message_buffer_size - 1) * message_text_spacing;
    const int center_x = this->fb.calc_center_x(message_width);
    const int center_y = this->fb.calc_center_y(16);

    this->fb.fill_screen(BACKGROUND_COLOR_1);

    draw_string(this->fb, center_x, center_y, message_text_scale, message_text_spacing, HEART_ICON_COLOR, message);
}

OverScreen::~OverScreen() {
    this->fb.clear_keypress_listeners();
}
