#include <memory>
#include "keycodes.h"
#include "framebuffer/framebuffer.h"

int main() {
    std::shared_ptr<FrameBuffer> fb(createFrameBuffer(800, 600));

    fb->createWindow();

    for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 50; x++) {
            fb->writePixel(x, y, 0xFF0000);
        }
    }

    fb->registerKeypressListener(KEY_ESC, [fb]() {
       fb->setRunning(false);
    });

    while (fb->shouldRun()) {
        fb->loop();
    }

    return 0;
}
