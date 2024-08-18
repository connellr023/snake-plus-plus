
#include "main.hpp"
#include "start_screen/start_screen.hpp"
#include "game/game.hpp"

int main() {
    // Initialize the frame buffer
    FrameBufferImpl *fb = new FrameBufferImpl(FB_WIDTH, FB_HEIGHT);

    fb->create_window();

    // Create the start screen
    StartScreen *start_screen = new StartScreen(*fb);

    while (fb->should_run() && start_screen->should_run()) {
        start_screen->update();

        fb->handle_events();
        fb->render();
    }

    delete start_screen;

    // Create the game instance
    Game *game = new Game(*fb, GRID_WIDTH, GRID_HEIGHT);

    while (fb->should_run()) {
        game->update();

        fb->handle_events();
        fb->render();
    }

    delete game;
    delete fb;

    return 0;
}
