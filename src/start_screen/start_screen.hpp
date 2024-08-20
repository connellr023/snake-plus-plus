#ifndef START_SCREEN_H
#define START_SCREEN_H
#include <cstdint>
#include <functional>
#include "../framebuffer/impl.hpp"
#include "../time_manager/time_manager.hpp"

#define TITLE_TEXT_COLOR_1      0xF27E11
#define TITLE_TEXT_COLOR_2      0xFCAA38

#define OPTION_TEXT_COLOR       0xFFFFFF
#define SELECTED_OPTION_COLOR   0xFF0000

#define TITLE_TEXT_MS           700
#define TITLE_TEXT_SCALE        8
#define TITLE_TEXT_SPACING      2
#define TITLE_TEXT_Y            30

#define OPTION_TEXT_SPACING     0
#define OPTION_TEXT_SCALE       5

#define START_OPTION_Y          200
#define QUIT_OPTION_Y           290

constexpr int title_text_buf_size = 8;
constexpr char title_text[title_text_buf_size] = "SNAKE++";

constexpr int start_option_text_buf_size = 6;
constexpr char start_option_text[start_option_text_buf_size] = "START";

constexpr int quit_option_text_buf_size = 5;
constexpr char quit_option_text[quit_option_text_buf_size] = "QUIT";

struct StartOption {
    const char *name;
    int x;
    int y;
    std::function<void()> action;

    bool operator==(const StartOption &other) const {
        return name == other.name;
    }
};

class StartScreen : public TimeManager {
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

    StartOption options[option_count] {};

    void update_selection(uint8_t new_selection);

    void draw_title_text();
    void draw_options();

public:
    StartScreen(FrameBufferImpl &fb);

    void update();

    bool should_run() const {
        return is_running;
    }

    ~StartScreen();
};

#endif // START_SCREEN_H
