#ifdef __linux__
#include <X11/Xlib.h>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "framebuffer.h"

class LinuxFrameBuffer : public FrameBuffer {
private:
    Window window;
    Window root;

    Display* display;
    XImage* image;

public:
    LinuxFrameBuffer(int width, int height)
        : FrameBuffer(width, height), window(0), display(nullptr), image(nullptr) {}

    void createWindow() override {
        this->display = XOpenDisplay(nullptr);

        if (!this->display) {
            exit(1);
        }

        this->root = DefaultRootWindow(this->display);
        this->window = XCreateSimpleWindow(this->display, root, 0, 0, this->width, this->height, 1, 0, 0);

        XSelectInput(this->display, this->window, ExposureMask | KeyPressMask);
        XMapWindow(this->display, this->window);

        this->image = XCreateImage(display, DefaultVisual(display, 0), DefaultDepth(display, 0), ZPixmap, 0, nullptr, this->width, this->height, 32, 0);
        this->image->data = (char *) malloc(this->width * this->height * sizeof(int));
    }

    void render() override {
        GC gc = DefaultGC(this->display, 0);
        XPutImage(this->display, this->window, gc, this->image, 0, 0, 0, 0, this->width, this->height);
    }

    void handleEvents() override {
        XEvent event;

        while (XPending(this->display) > 0) {
            XNextEvent(this->display, &event);

            switch (event.type) {
                case Expose:
                    this->render();
                    break;
                case KeyPress:
                    exit(0);
                    break;
            }
        }
    }

    ~LinuxFrameBuffer() override {
        if (this->image) {
            if (this->image->data) {
                free(this->image->data);
            }

            this->image->f.destroy_image(this->image);
        }
        if (this->display) {
            XDestroyWindow(this->display, this->window);
            XCloseDisplay(this->display);
        }
    }
};

std::unique_ptr<FrameBuffer> createFrameBuffer(int width, int height) {
    return std::make_unique<LinuxFrameBuffer>(width, height);
}

#endif // __linux__
