#ifdef __linux
#include "linux_framebuffer.hpp"

void LinuxFrameBuffer::create_window_impl() {
    this->display = XOpenDisplay(nullptr);

    if (!this->display) {
        exit(1);
    }

    this->root = DefaultRootWindow(this->display);
    this->window = XCreateSimpleWindow(this->display, root, 0, 0, this->width, this->height, 1, 0, 0);

    XStoreName(this->display, this->window, "Snake++");
    XSelectInput(this->display, this->window, ExposureMask | KeyPressMask);
    XMapWindow(this->display, this->window);

    this->image = XCreateImage(display, DefaultVisual(display, 0), DefaultDepth(display, 0), ZPixmap, 0, nullptr, this->width, this->height, 32, 0);
    this->image->data = (char *) malloc(this->width * this->height * sizeof(uint32_t));

    this->gc = DefaultGC(this->display, 0);

    wmDeleteMessage = XInternAtom(this->display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(this->display, this->window, &wmDeleteMessage, 1);

    this->set_running(true);
}

void LinuxFrameBuffer::render_impl() {
    XPutImage(this->display, this->window, this->gc, this->image, 0, 0, 0, 0, this->width, this->height);
    XFlush(this->display);
}

void LinuxFrameBuffer::handle_events_impl() {
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
                if (static_cast<Atom>(event.xclient.data.l[0]) == this->wmDeleteMessage) {
                    this->set_running(false);
                }

                break;
            }
        }
    }
}

void LinuxFrameBuffer::write_pixel_impl(int x, int y, uint32_t color) {
    assert(x >= 0 && x < this->width && y >= 0 && y < this->height);
    ((uint32_t *) this->image->data)[y * this->width + x] = color;
}

LinuxFrameBuffer::~LinuxFrameBuffer()  {
    if (this->image) {
        this->image->f.destroy_image(this->image);
    }

    if (this->display) {
        XDestroyWindow(this->display, this->window);
        XCloseDisplay(this->display);
    }
}

#endif // __linux
