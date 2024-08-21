#ifndef OVER_SCREEN_H
#define OVER_SCREEN_H
#include "../framebuffer/impl.hpp"

class OverScreen {
private:
    FrameBufferImpl &fb;

    bool is_running = true;

public:
    OverScreen(FrameBufferImpl &fb);

    bool should_run() const {
        return is_running;
    }

    ~OverScreen();
};

#endif // OVER_SCREEN_H
