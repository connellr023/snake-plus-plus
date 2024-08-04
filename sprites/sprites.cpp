#include <cstdint>
#include "sprites.hpp"
#include "colors.hpp"

uint32_t calc_bg_color(int tile_x, int tile_y) {
    return (tile_x + tile_y) % 2 == 0
        ? BACKGROUND_COLOR_1
        : BACKGROUND_COLOR_2;
}

void draw_rect(FrameBuffer &fb, int x, int y, int width, int height, uint32_t color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fb.write_pixel(x + j, y + i, color);
        }
    }
}

void draw_sprite(FrameBuffer &fb, int pixel_x, int pixel_y, int scale, uint32_t color, uint32_t bg_color, uint64_t sprite) {
    for (uint8_t y = 0; y < 8; y++) {
        for (uint8_t x = 0; x < 8; x++) {
            const uint8_t bit = (sprite >> (y * 8 + x)) & 1;
            draw_rect(fb, pixel_x + (x * scale), pixel_y + (y * scale), scale, scale, bit ? color : bg_color);
        }
    }
}

void draw_tile(FrameBuffer &fb, int tile_x, int tile_y, tile_t tile) {
    const int pixel_pos_x = tile_x * TILE_PIXELS;
    const int pixel_pos_y = tile_y * TILE_PIXELS;

    const uint32_t bg_color = calc_bg_color(tile_x, tile_y);

    switch (tile) {
        default: {
            draw_rect(fb, pixel_pos_x, pixel_pos_y, TILE_PIXELS, TILE_PIXELS, bg_color);
            break;
        }
    }
}
