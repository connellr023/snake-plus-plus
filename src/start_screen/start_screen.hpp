#ifndef START_SCREEN_H
#define START_SCREEN_H
#include <cstdint>
#include <functional>
#include "../framebuffer/impl.hpp"

#define TITLE_TEXT_COLOR_1  0xF27E11
#define TITLE_TEXT_COLOR_2  0xFCAA38

#define TITLE_TEXT_MS       700
#define TITLE_TEXT_SCALE    8
#define TITLE_TEXT_SPACING  2
#define TITLE_TEXT_Y        30

constexpr int title_text_buf_size = 8;
constexpr char title_text[title_text_buf_size] = "SNAKE++";

struct StartOption {
    const char *name;
    std::function<void()> action;
};

class StartScreen {
private:
    FrameBufferImpl &fb;

    bool is_running = true;

    void start_game();
    void quit_game();

    uint8_t title_text_effect_idx = 0;
    uint64_t title_text_last_update_ms = 0;

    int title_text_x;

    uint8_t selected_option = 0;
    static const uint8_t option_count = 2;

    const StartOption options[option_count] = {
        { "START", [this]() { this->start_game(); } },
        { "QUIT?", [this]() { this->quit_game(); } }
    };

    void update_selection(uint8_t new_selection);

    void draw_title_text();

public:
    StartScreen(FrameBufferImpl &fb);

    void update();

    bool should_run() const {
        return is_running;
    }

    ~StartScreen();
};

#endif // START_SCREEN_H
