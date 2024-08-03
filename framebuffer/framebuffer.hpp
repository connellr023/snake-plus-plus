#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <cstdint>
#include <functional>
#include <map>

typedef std::function<void()> keypress_listener_t;

class FrameBuffer {
protected:
    std::map<uint64_t, keypress_listener_t> keypressListeners;

    bool isRunning = false;
    int width;
    int height;

    void notifyKeypress(uint64_t key) {
        if (this->keypressListeners.find(key) != this->keypressListeners.end()) {
            this->keypressListeners[key]();
        }
    }

public:
    FrameBuffer(int width, int height) : width(width), height(height) {}

    virtual ~FrameBuffer() = default;

    virtual void createWindow() = 0;
    virtual void loop() = 0;
    virtual void writePixel(int x, int y, uint32_t color) = 0;

    void registerKeypressListener(uint64_t keycode, keypress_listener_t listener) {
        this->keypressListeners[keycode] = listener;
    }

    bool shouldRun() const {
        return this->isRunning;
    }

    void setRunning(bool running) {
        this->isRunning = running;
    }
};

FrameBuffer* createFrameBuffer(int width, int height);

#endif // FRAMEBUFFER_H
