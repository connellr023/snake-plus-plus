#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <cstdint>
#include <functional>
#include <map>

typedef std::function<void()> keypress_listener_t;

template<typename Derived>
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

    void create_window() {
        static_cast<Derived *>(this)->create_window_impl();
    }

    void render() {
        static_cast<Derived *>(this)->render_impl();
    }
    void handle_events() {
        static_cast<Derived *>(this)->handle_events_impl();
    }

    void write_pixel(int x, int y, uint32_t color) {
        static_cast<Derived *>(this)->write_pixel_impl(x, y, color);
    }

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

#endif // FRAMEBUFFER_H
