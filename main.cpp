#include <memory>
#include "framebuffer/framebuffer.h"

int main() {
    std::unique_ptr<FrameBuffer> fb = createFrameBuffer(800, 600);
    fb->createWindow();

    while (true) {
        fb->handleEvents();
        fb->render();
    }

    return 0;
}
