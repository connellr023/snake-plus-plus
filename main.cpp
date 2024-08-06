#include <memory>
#include "main.hpp"
#include "keycodes.hpp"
#include "game/game.hpp"

int main() {
    std::shared_ptr<FrameBufferImpl> fb(new FrameBufferImpl(FB_WIDTH, FB_HEIGHT));
    std::unique_ptr<Game> game(new Game(*fb, GRID_WIDTH, GRID_HEIGHT));

    fb->create_window();

    fb->register_keypress_listener(KEY_ESC, [fb]() {
       fb->set_running(false);
    });

    game->init();

    while (fb->should_run()) {
        fb->handle_events();
        fb->render();

        game->loop();
    }

    return 0;
}
