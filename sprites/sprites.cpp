#include "sprites.hpp"

void draw_rect(FrameBuffer &fb, int x, int y, int width, int height, uint32_t color) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fb.writePixel(x, y, color);
        }
    }
}
