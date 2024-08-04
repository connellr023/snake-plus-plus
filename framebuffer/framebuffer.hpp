#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <cstdint>
#include <functional>
#include <map>

typedef std::function<void()> keypress_listener_t;

class FrameBuffer {
protected:
    std::map<uint64_t, keypress_listener_t> keypress_listeners;

    bool is_running = false;
    int width;
    int height;

    void notify_keypress(uint64_t key) {
        if (this->keypress_listeners.find(key) != this->keypress_listeners.end()) {
            this->keypress_listeners[key]();
        }
    }

public:
    FrameBuffer(int width, int height) : width(width), height(height) {}

    virtual ~FrameBuffer() = default;

    virtual void create_window() = 0;
    virtual void render() = 0;
    virtual void handleEvents() = 0;
    virtual void write_pixel(int x, int y, uint32_t color) = 0;

    void register_keypress_listener(uint64_t keycode, keypress_listener_t listener) {
        this->keypress_listeners[keycode] = listener;
    }

    bool should_run() const {
        return this->is_running;
    }

    void set_running(bool running) {
        this->is_running = running;
    }
};

FrameBuffer* create_frame_buffer(int width, int height);

#endif // FRAMEBUFFER_H
