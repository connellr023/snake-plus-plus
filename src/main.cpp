#include <memory>
#include "main.hpp"
#include "game/game.hpp"

int main() {
    // Initialize the frame buffer
    std::shared_ptr<FrameBufferImpl> fb(new FrameBufferImpl(FB_WIDTH, FB_HEIGHT));

    fb->create_window();

    // Create the game instance
    std::unique_ptr<Game> game(new Game(*fb, GRID_WIDTH, GRID_HEIGHT));

    while (fb->should_run()) {
        fb->handle_events();
        fb->render();

        game->update();
    }

    return 0;
}
