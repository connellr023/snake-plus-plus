#ifndef SPRITES_H
#define SPRITES_H
#include "../framebuffer/framebuffer.hpp"
#include "../game/game.hpp"

#define TILE_PIXELS     32

uint32_t calc_bg_color(int tile_x, int tile_y);

void draw_rect(FrameBuffer &fb, int x, int y, int width, int height, uint32_t color);
void draw_sprite(FrameBuffer &fb, int x, int y, int width, int height, uint64_t sprite);
void draw_tile(FrameBuffer &fb, int tile_x, int tile_y, tile_t tile);

#endif // SPRITES_H
