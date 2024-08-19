#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <cstdint>
#include <functional>
#include <unordered_map>

typedef std::function<void()> keypress_listener_t;

template<typename Derived>
class FrameBuffer {
protected:
    std::unordered_map<uint64_t, keypress_listener_t> keypress_listeners;

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

    void fill_screen(uint32_t color) {
        for (int y = 0; y < this->height; y++) {
            for (int x = 0; x < this->width; x++) {
                this->write_pixel(x, y, color);
            }
        }
    }

    int calc_center_x(int width) {
        return (this->width - width) / 2;
    }

    void register_keypress_listener(uint64_t keycode, keypress_listener_t listener) {
        this->keypress_listeners[keycode] = listener;
    }

    void clear_keypress_listeners() {
        this->keypress_listeners.clear();
    }

    bool should_run() const {
        return this->is_running;
    }

    void set_running(bool is_running) {
        this->is_running = is_running;
    }
};

#endif // FRAMEBUFFER_H
