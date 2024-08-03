#include <memory>
#include "main.hpp"
#include "keycodes.hpp"
#include "framebuffer/framebuffer.hpp"
#include "game/game.hpp"

int main() {
    std::shared_ptr<FrameBuffer> fb(createFrameBuffer(FB_WIDTH, FB_HEIGHT));
    std::shared_ptr<Game> game(new Game(*fb, GRID_WIDTH, GRID_HEIGHT));

    fb->createWindow();

    fb->registerKeypressListener(KEY_ESC, [fb]() {
       fb->setRunning(false);
    });

    while (fb->shouldRun()) {
        fb->loop();
        game->tick();
    }

    return 0;
}
