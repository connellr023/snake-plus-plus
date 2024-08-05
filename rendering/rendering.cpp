#include <cstdint>
#include "rendering.hpp"
#include "colors.hpp"
#include "sprites.hpp"

const static bit_extractor_t orientation_normal = [](uint8_t x, uint8_t y, uint64_t sprite) -> uint8_t {
    return (sprite >> ((7 - y) * 8 + (7 - x))) & 1;
};

const static bit_extractor_t orientation_flipped = [](uint8_t x, uint8_t y, uint64_t sprite) -> uint8_t {
    return (sprite >> (y * 8 + x)) & 1;
};

const static bit_extractor_t orientation_rotated_90 = [](uint8_t x, uint8_t y, uint64_t sprite) -> uint8_t {
    return (sprite >> ((7 - x) * 8 + y)) & 1;
};

const static bit_extractor_t orientation_rotated_270 = [](uint8_t x, uint8_t y, uint64_t sprite) -> uint8_t {
    return (sprite >> (x * 8 + (7 - y))) & 1;
};

bit_extractor_t get_orientation(Direction dir) {
    bit_extractor_t orientation;

    switch (dir) {
        case Direction::Up:
            orientation = orientation_normal;
            break;
        case Direction::Down:
            orientation = orientation_flipped;
            break;
        case Direction::Left:
            orientation = orientation_rotated_90;
            break;
        case Direction::Right:
            orientation = orientation_rotated_270;
            break;
    }

    return orientation;
}

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

void draw_sprite(FrameBuffer &fb, int pixel_x, int pixel_y, int scale, uint32_t color, uint32_t bg_color, uint64_t sprite, bit_extractor_t orientation) {
    for (uint8_t y = 0; y < 8; y++) {
        for (uint8_t x = 0; x < 8; x++) {
            const uint8_t bit = orientation(x, y, sprite);
            draw_rect(fb, pixel_x + (x * scale), pixel_y + (y * scale), scale, scale, bit ? color : bg_color);
        }
    }
}

void draw_tile(FrameBuffer &fb, Snake &snake, int tile_x, int tile_y, Tile tile) {
    const int pixel_pos_x = tile_x * TILE_PIXELS;
    const int pixel_pos_y = tile_y * TILE_PIXELS;

    const uint32_t bg_color = calc_bg_color(tile_x, tile_y);

    switch (tile) {
        case Tile::Food:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, GROW_PACK_COLOR, bg_color, SPRITE_VALUE_PACK, orientation_normal);
            break;
        case Tile::SnakeHead:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, snake.get_color(), bg_color, SPRITE_SNAKE_HEAD, get_orientation(snake.get_direction()));
            break;
        case Tile::SnakeSegmentVertical:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, snake.get_color(), bg_color, SPRITE_SNAKE_BODY, get_orientation(Direction::Up));
            break;
        case Tile::SnakeSegmentHorizontal:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, snake.get_color(), bg_color, SPRITE_SNAKE_BODY, get_orientation(Direction::Right));
            break;
        case Tile::SnakeSegmentTopLeft:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, snake.get_color(), bg_color, SPRITE_SNAKE_CORNER, get_orientation(Direction::Left));
            break;
        case Tile::SnakeSegmentTopRight:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, snake.get_color(), bg_color, SPRITE_SNAKE_CORNER, get_orientation(Direction::Up));
            break;
        case Tile::SnakeSegmentBottomLeft:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, snake.get_color(), bg_color, SPRITE_SNAKE_CORNER, get_orientation(Direction::Down));
            break;
        case Tile::SnakeSegmentBottomRight:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, snake.get_color(), bg_color, SPRITE_SNAKE_CORNER, get_orientation(Direction::Right));
            break;
        default:
            draw_rect(fb, pixel_pos_x, pixel_pos_y, TILE_PIXELS, TILE_PIXELS, bg_color);
            break;
    }
}

void draw_uint(FrameBuffer &fb, int pixel_x, int pixel_y, int scale, uint32_t color, uint32_t bg_color, uint8_t digits, uint16_t value) {
    const static uint64_t number_sprites[] = {
        SPRITE_0,
        SPRITE_1,
        SPRITE_2,
        SPRITE_3,
        SPRITE_4,
        SPRITE_5,
        SPRITE_6,
        SPRITE_7,
        SPRITE_8,
        SPRITE_9
    };

    uint8_t digit_buffer[digits];
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

void draw_ui_sprite(FrameBuffer &fb, int x, uint32_t color, uint64_t sprite) {
    draw_sprite(fb, x, UI_GLYPH_Y, UI_GLYPH_SCALE, color, BACKGROUND_COLOR_1, sprite, orientation_normal);
}

void draw_ui_uint(FrameBuffer &fb, int x, uint32_t color, uint8_t digits, uint16_t value) {
    draw_uint(fb, x, UI_GLYPH_Y, UI_GLYPH_SCALE, color, BACKGROUND_COLOR_1, digits, value);
}
