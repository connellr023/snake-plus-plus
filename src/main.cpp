#include "main.hpp"
#include "over_screen/over_screen.hpp"
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

    while (fb->should_run() && !game->is_over()) {
        game->update();

        fb->handle_events();
        fb->render();
    }

    const bool is_game_over = game->is_over();
    delete game;

    if (is_game_over) {
        OverScreen *over_screen = new OverScreen(*fb);

        while (fb->should_run() && over_screen->should_run()) {
            fb->handle_events();
            fb->render();
        }

        delete over_screen;
    }

    delete fb;
    return 0;
}
