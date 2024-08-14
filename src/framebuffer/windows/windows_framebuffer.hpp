#ifdef __WIN32
#include <windows.h>
#include "../framebuffer.hpp"

class WindowsFrameBuffer : public FrameBuffer<WindowsFrameBuffer> {
private:
    HWND hwnd;
    HDC hdc;
    HBITMAP bitmap;

    uint32_t *buffer;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
    WindowsFrameBuffer(int width, int height) :
        FrameBuffer<WindowsFrameBuffer>(width, height),
        hwnd(nullptr),
        hdc(nullptr),
        bitmap(nullptr),
        buffer(nullptr)
    {}

    void create_window_impl();
    void render_impl();
    void handle_events_impl();
    void write_pixel_impl(int x, int y, uint32_t color);

    ~WindowsFrameBuffer();
};

#endif // __WIN32