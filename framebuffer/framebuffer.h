#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <memory>

class FrameBuffer {
public:
    FrameBuffer(int width, int height) : width(width), height(height) {}

    virtual ~FrameBuffer() = default;

    virtual void createWindow() = 0;
    virtual void render() = 0;
    virtual void handleEvents() = 0;

protected:
    int width;
    int height;
};

std::unique_ptr<FrameBuffer> createFrameBuffer(int width, int height);

#endif // FRAMEBUFFER_H
