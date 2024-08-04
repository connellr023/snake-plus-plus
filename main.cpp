#include <memory>
#include "main.hpp"
#include "keycodes.hpp"
#include "framebuffer/framebuffer.hpp"
#include "game/game.hpp"

int main() {
    std::shared_ptr<FrameBuffer> fb(create_frame_buffer(FB_WIDTH, FB_HEIGHT));
    std::shared_ptr<Game> game(new Game(*fb, GRID_WIDTH, GRID_HEIGHT, TICK_MS));

    fb->create_window();

    fb->register_keypress_listener(KEY_ESC, [fb]() {
       fb->set_running(false);
    });

    game->init();

    while (fb->should_run()) {
        fb->handleEvents();
        fb->render();

        game->loop();
    }

    return 0;
}
