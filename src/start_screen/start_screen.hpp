#ifndef START_SCREEN_H
#define START_SCREEN_H
#include <cstdint>
#include <functional>
#include "../framebuffer/impl.hpp"

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

    uint8_t selected_option = 0;
    static const uint8_t option_count = 2;

    const StartOption options[option_count] = {
        { "START", [this]() { this->start_game(); } },
        { "QUIT?", [this]() { this->quit_game(); } }
    };

    void update_selection(uint8_t new_selection);

public:
    StartScreen(FrameBufferImpl &fb);

    void update();

    bool should_run() const {
        return is_running;
    }

    ~StartScreen();
};

#endif // START_SCREEN_H
