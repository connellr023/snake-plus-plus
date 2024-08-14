#ifdef __WIN32
#include "../framebuffer.hpp"

class WindowsFrameBuffer : public FrameBuffer<WindowsFrameBuffer> {
public:
    WindowsFrameBuffer(int width, int height) : FrameBuffer<WindowsFrameBuffer>(width, height) {}

    void create_window_impl();
    void render_impl();
    void handle_events_impl();
    void write_pixel_impl(int x, int y, uint32_t color);

    ~WindowsFrameBuffer();
};

#endif // __WIN32