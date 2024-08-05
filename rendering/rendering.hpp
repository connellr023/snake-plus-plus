#ifndef RENDERING_H
#define RENDERING_H
#include <functional>
#include "../framebuffer/framebuffer.hpp"
#include "../game/game.hpp"

#define UI_GLYPH_SCALE      6
#define UI_GLYPH_Y          690

#define HEART_ICON_X        5
#define LIVES_TEXT_X        (UI_GLYPH_SCALE * 8) + HEART_ICON_X

#define TILE_PIXELS         32
#define TILE_SPRITE_SCALE   TILE_PIXELS / 8

typedef std::function<uint8_t(uint8_t, uint8_t, uint64_t)> bit_extractor_t;

uint32_t calc_bg_color(int tile_x, int tile_y);

void draw_rect(FrameBuffer &fb, int x, int y, int width, int height, uint32_t color);
void draw_sprite(FrameBuffer &fb, int pixel_x, int pixel_y, int scale, uint32_t color, uint32_t bg_color, uint64_t sprite, bit_extractor_t orientation);
void draw_uint(FrameBuffer &fb, int pixel_x, int pixel_y, int scale, uint32_t color, uint32_t bg_color, uint8_t digits, uint16_t value);
void draw_tile(FrameBuffer &fb, Snake &snake, int tile_x, int tile_y, Tile tile);

void draw_ui_sprite(FrameBuffer &fb, int x, uint32_t color, uint64_t sprite);
void draw_ui_uint(FrameBuffer &fb, int x, uint32_t color, uint8_t digits, uint16_t value);

#endif // SPRITES_H
