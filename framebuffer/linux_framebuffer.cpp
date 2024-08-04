#ifdef __linux__
#include <X11/Xlib.h>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdint>
#include "framebuffer.hpp"

class LinuxFrameBuffer : public FrameBuffer {
private:
    Window window;
    Window root;

    Display* display;
    XImage* image;

    Atom wmDeleteMessage;

public:
    LinuxFrameBuffer(int width, int height)
        : FrameBuffer(width, height), window(0), display(nullptr), image(nullptr) {}

    void create_window() override {
        this->display = XOpenDisplay(nullptr);

        if (!this->display) {
            exit(1);
        }

        this->root = DefaultRootWindow(this->display);
        this->window = XCreateSimpleWindow(this->display, root, 0, 0, this->width, this->height, 1, 0, 0);

        XSelectInput(this->display, this->window, ExposureMask | KeyPressMask);
        XMapWindow(this->display, this->window);

        this->image = XCreateImage(display, DefaultVisual(display, 0), DefaultDepth(display, 0), ZPixmap, 0, nullptr, this->width, this->height, 32, 0);
        this->image->data = (char *) malloc(this->width * this->height * sizeof(uint32_t));

        wmDeleteMessage = XInternAtom(this->display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(this->display, this->window, &wmDeleteMessage, 1);

        this->set_running(true);
    }

    void render() {
        GC gc = DefaultGC(this->display, 0);
        XPutImage(this->display, this->window, gc, this->image, 0, 0, 0, 0, this->width, this->height);
    }

    void loop() override {
        XEvent event;

        while (XPending(this->display) > 0 && this->should_run()) {
            XNextEvent(this->display, &event);

            switch (event.type) {
                case Expose: {
                    this->render();
                    break;
                }
                case KeyPress: {
                    KeySym key = XLookupKeysym(&event.xkey, 0);
                    uint64_t keycode = static_cast<uint64_t>(key);

                    this->notify_keypress(keycode);
                    break;
                }
                case ClientMessage: {
                    if ((Atom) event.xclient.data.l[0] == this->wmDeleteMessage) {
                        set_running(false);
                    }

                    break;
                }
            }
        }
    }

    void write_pixel(int x, int y, uint32_t color) override {
        assert(x >= 0 && x < this->width && y >= 0 && y < this->height);
        ((uint32_t *) this->image->data)[y * this->width + x] = color;
    }

    ~LinuxFrameBuffer() override {
        if (this->image) {
            this->image->f.destroy_image(this->image);
        }

        if (this->display) {
            XDestroyWindow(this->display, this->window);
            XCloseDisplay(this->display);
        }
    }
};

FrameBuffer* create_frame_buffer(int width, int height) {
    return new LinuxFrameBuffer(width, height);
}

#endif // __linux__
