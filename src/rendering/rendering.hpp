#ifndef RENDERING_H
#define RENDERING_H
#include <functional>
#include "colors.hpp"
#include "font.hpp"
#include "../main.hpp"
#include "../game/game.hpp"

#define UI_GLYPH_SCALE          6
#define UI_GLYPH_Y              690

#define HEART_ICON_X            5
#define LIVES_TEXT_X            (UI_GLYPH_SCALE * 8) + HEART_ICON_X
#define STAR_ICON_X             (UI_GLYPH_SCALE * 8 * 3) + LIVES_TEXT_X
#define SCORE_TEXT_X            (UI_GLYPH_SCALE * 8) + STAR_ICON_X
#define HIGHSCORE_ICON_X        (UI_GLYPH_SCALE * 8 * 4) + SCORE_TEXT_X
#define HIGHSCORE_TEXT_X        (UI_GLYPH_SCALE * 8) + HIGHSCORE_ICON_X
#define CLOCK_ICON_X            (UI_GLYPH_SCALE * 8 * 4) + HIGHSCORE_TEXT_X
#define CLOCK_TEXT_X            (UI_GLYPH_SCALE * 8) + CLOCK_ICON_X
#define PAUSE_RESUME_ICON_X     FB_WIDTH - (UI_GLYPH_SCALE * 8) - 5

#define TILE_PIXELS             32
#define TILE_SPRITE_SCALE       TILE_PIXELS / 8

typedef std::function<uint8_t(uint8_t, uint8_t, uint64_t)> bit_extractor_t;

extern const bit_extractor_t orientation_normal;
extern const bit_extractor_t orientation_flipped;
extern const bit_extractor_t orientation_rotated_90;
extern const bit_extractor_t orientation_rotated_270;

uint32_t calc_bg_color(uint8_t x, uint8_t y);
uint32_t calc_gradient_color(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
uint32_t hsv_to_rgb(float h, float s, float v);

void draw_rect(FrameBufferImpl &fb, int x, int y, int width, int height, uint32_t color);
void draw_sprite(FrameBufferImpl &fb, int pixel_x, int pixel_y, int scale, uint32_t color, uint32_t bg_color, uint64_t sprite, bit_extractor_t orientation, bool is_transparent = false);
void draw_tile(FrameBufferImpl &fb, int tile_x, int tile_y, Tile tile);
void draw_snake_tile(FrameBufferImpl &fb, Snake &snake, int tile_x, int tile_y, Tile tile);
void draw_ui_sprite(FrameBufferImpl &fb, int x, uint32_t color, uint64_t sprite);
void draw_string(FrameBufferImpl &fb, int x, int y, int scale, uint32_t color, const char *str);

template<int amount>
void draw_layered_sprites(FrameBufferImpl &fb, int pixel_x, int pixel_y, int scale, uint32_t bg_color, bit_extractor_t orientation, const uint64_t sprites[amount], const uint32_t colors[amount]) {
    // Draw the first sprite that is not transparent
    draw_sprite(fb, pixel_x, pixel_y, scale, colors[0], bg_color, sprites[0], orientation);

    // Draw the rest of the sprites on top of the first one
    for (int i = 1; i < amount; i++) {
        draw_sprite(fb, pixel_x, pixel_y, scale, colors[i], bg_color, sprites[i], orientation, true);
    }
}

template<int digits>
void draw_uint(FrameBufferImpl &fb, int pixel_x, int pixel_y, int scale, uint32_t color, uint32_t bg_color, uint16_t value) {
    static_assert(digits > 1, "The number of digits must be greater than 1.");

    const static uint64_t number_sprites[] = {
        CHAR_0,
        CHAR_1,
        CHAR_2,
        CHAR_3,
        CHAR_4,
        CHAR_5,
        CHAR_6,
        CHAR_7,
        CHAR_8,
        CHAR_9
    };

    uint8_t *digit_buffer = new uint8_t[digits];
    uint8_t i = 1;

    for (uint8_t k = 0; k < digits; k++) {
        digit_buffer[k] = 0;
    }

    while (value > 0 && i <= digits) {
        digit_buffer[digits - i++] = value % 10;
        value /= 10;
    }

    for (uint8_t k = 0; k < digits; k++) {
        draw_sprite(fb, pixel_x + (k * 8 * scale), pixel_y, scale, color, bg_color, number_sprites[digit_buffer[k]], orientation_normal);
    }
}

template<int digits>
void draw_ui_uint(FrameBufferImpl &fb, int x, uint32_t color, uint16_t value) {
    draw_uint<digits>(fb, x, UI_GLYPH_Y, UI_GLYPH_SCALE, color, BACKGROUND_COLOR_1, value);
}

#endif // SPRITES_H
