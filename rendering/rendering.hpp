#ifndef RENDERING_H
#define RENDERING_H
#include "colors.hpp"
#include "../framebuffer/framebuffer.hpp"
#include "../game/game.hpp"

#define UI_GLYPH_SCALE  6
#define UI_GLYPH_Y      690

#define HEART_ICON_X    5
#define LIVES_TEXT_X    (UI_GLYPH_SCALE * 8) + HEART_ICON_X

#define TILE_PIXELS     32

uint32_t calc_bg_color(int tile_x, int tile_y);

void draw_rect(FrameBuffer &fb, int x, int y, int width, int height, uint32_t color);
void draw_sprite(FrameBuffer &fb, int pixel_x, int pixel_y, int scale, uint32_t color, uint32_t bg_color, uint64_t sprite);
void draw_uint(FrameBuffer &fb, int pixel_x, int pixel_y, int scale, uint32_t color, uint32_t bg_color, uint8_t digits, uint16_t value);
void draw_tile(FrameBuffer &fb, int tile_x, int tile_y, tile_t tile);

#define RENDER_UI_SPRITE(fb, x, color, sprite) \
    draw_sprite(fb, x, UI_GLYPH_Y, UI_GLYPH_SCALE, color, BACKGROUND_COLOR_1, sprite)

#define RENDER_UI_UINT(fb, x, color, digits, value) \
    draw_uint(fb, x, UI_GLYPH_Y, UI_GLYPH_SCALE, color, BACKGROUND_COLOR_1, digits, value)

#endif // SPRITES_H
