#ifndef RENDERING_H
#define RENDERING_H
#include <functional>
#include "../game/game.hpp"

#define UI_GLYPH_SCALE      6
#define UI_GLYPH_Y          690

#define HEART_ICON_X        5
#define LIVES_TEXT_X        (UI_GLYPH_SCALE * 8) + HEART_ICON_X
#define STAR_ICON_X         (UI_GLYPH_SCALE * 8 * 3) + LIVES_TEXT_X
#define SCORE_TEXT_X        (UI_GLYPH_SCALE * 8) + STAR_ICON_X

#define TILE_PIXELS         32
#define TILE_SPRITE_SCALE   TILE_PIXELS / 8

typedef std::function<uint8_t(uint8_t, uint8_t, uint64_t)> bit_extractor_t;

uint32_t calc_bg_color(int tile_x, int tile_y);

void draw_rect(FrameBufferImpl &fb, int x, int y, int width, int height, uint32_t color);
void draw_sprite(FrameBufferImpl &fb, int pixel_x, int pixel_y, int scale, uint32_t color, uint32_t bg_color, uint64_t sprite, bit_extractor_t orientation);
void draw_uint(FrameBufferImpl &fb, int pixel_x, int pixel_y, int scale, uint32_t color, uint32_t bg_color, uint8_t digits, uint16_t value);
void draw_tile(FrameBufferImpl &fb, Snake &snake, int tile_x, int tile_y, Tile tile);

void draw_ui_sprite(FrameBufferImpl &fb, int x, uint32_t color, uint64_t sprite);
void draw_ui_uint(FrameBufferImpl &fb, int x, uint32_t color, uint8_t digits, uint16_t value);

#endif // SPRITES_H
