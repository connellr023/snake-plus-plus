#ifdef __linux__
#include <X11/Xlib.h>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdint>
#include "../framebuffer.hpp"

class LinuxFrameBuffer : public FrameBuffer<LinuxFrameBuffer> {
private:
    Window window;
    Window root;

    GC gc;

    Display* display;
    XImage* image;

    Atom wmDeleteMessage;

public:
    LinuxFrameBuffer(int width, int height) : FrameBuffer<LinuxFrameBuffer>(width, height), window(0), display(nullptr), image(nullptr) {}

    void create_window_impl();
    void render_impl();
    void handle_events_impl();
    void write_pixel_impl(int x, int y, uint32_t color);

    ~LinuxFrameBuffer();
};

#endif // __linux__
