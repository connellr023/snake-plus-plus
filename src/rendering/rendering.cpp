#include <cstdint>
#include "rendering.hpp"
#include "font.hpp"
#include "sprites.hpp"

constexpr uint8_t rainbow_pack_sprite_layers = 3;
const static uint64_t rainbow_pack_sprites[rainbow_pack_sprite_layers] = { SPRITE_VALUE_PACK, SPRITE_VALUE_PACK_INNER_1, SPRITE_VALUE_PACK_INNER_2 };
const static uint32_t rainbow_pack_colors[rainbow_pack_sprite_layers] = { RAINBOW_PACK_COLOR_1, RAINBOW_PACK_COLOR_2, RAINBOW_PACK_COLOR_3 };

constexpr uint8_t snake_segment_sprite_layers = 2;
const static uint64_t snake_body_sprites[snake_segment_sprite_layers] = { SPRITE_SNAKE_BODY, SPRITE_SNAKE_BODY_SCALE };
const static uint64_t snake_corner_sprites[snake_segment_sprite_layers] = { SPRITE_SNAKE_CORNER, SPRITE_SNAKE_CORNER_SCALE };

const bit_extractor_t orientation_normal = [](uint8_t x, uint8_t y, uint64_t sprite) -> uint8_t {
    return (sprite >> ((7 - y) * 8 + (7 - x))) & 1;
};

const bit_extractor_t orientation_flipped = [](uint8_t x, uint8_t y, uint64_t sprite) -> uint8_t {
    return (sprite >> (y * 8 + x)) & 1;
};

const bit_extractor_t orientation_rotated_90 = [](uint8_t x, uint8_t y, uint64_t sprite) -> uint8_t {
    return (sprite >> ((7 - x) * 8 + y)) & 1;
};

const bit_extractor_t orientation_rotated_270 = [](uint8_t x, uint8_t y, uint64_t sprite) -> uint8_t {
    return (sprite >> (x * 8 + (7 - y))) & 1;
};

bit_extractor_t orientation_from_dir(Direction dir) {
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

uint32_t calc_bg_color(uint8_t x, uint8_t y) {
    return (x + y) % 2 == 0
        ? BACKGROUND_COLOR_1
        : BACKGROUND_COLOR_2;
}

uint32_t calc_gradient_color(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    const float hue_x = static_cast<float>(x) / static_cast<float>(width);
    const float hue_y = static_cast<float>(y) / static_cast<float>(height);
    const float hue = (hue_x + hue_y) / 2;

    return hsv_to_rgb(hue, 1, 1);
}

uint32_t hsv_to_rgb(float h, float s, float v) {
    int i = static_cast<int>(h * 6);

    float r, g, b;
    float f = (h * 6) - i;
    float p = v * (1 - s);
    float q = v * (1 - (f * s));
    float t = v * (1 - ((1 - f) * s));

    i = i % 6;

    switch (i) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
        default: r = g = b = 0; break;
    }

    return  (static_cast<uint32_t>(r * 255) << 16) |
            (static_cast<uint32_t>(g * 255) << 8)  |
            (static_cast<uint32_t>(b * 255));
}

void draw_rect(FrameBufferImpl &fb, int x, int y, int width, int height, uint32_t color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fb.write_pixel(x + j, y + i, color);
        }
    }
}

void draw_sprite(FrameBufferImpl &fb, int pixel_x, int pixel_y, int scale, uint32_t color, uint32_t bg_color, uint64_t sprite, bit_extractor_t orientation, bool is_transparent) {
    for (uint8_t y = 0; y < 8; y++) {
        for (uint8_t x = 0; x < 8; x++) {
            const uint8_t bit = orientation(x, y, sprite);

            if (is_transparent && !bit) {
                continue;
            }

            draw_rect(fb, pixel_x + (x * scale), pixel_y + (y * scale), scale, scale, bit ? color : bg_color);
        }
    }
}

void draw_tile(FrameBufferImpl &fb, int tile_x, int tile_y, Tile tile) {
    const int pixel_pos_x = tile_x * TILE_PIXELS;
    const int pixel_pos_y = tile_y * TILE_PIXELS;

    const uint32_t bg_color = calc_bg_color(tile_x, tile_y);

    switch (tile) {
        case Tile::Food:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, GROW_PACK_COLOR, bg_color, SPRITE_VALUE_PACK, orientation_normal);
            break;
        case Tile::PortalPack:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, PORTAL_PACK_COLOR, bg_color, SPRITE_VALUE_PACK, orientation_normal);
            break;
        case Tile::AttackPack:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, ATTACK_PACK_COLOR, bg_color, SPRITE_VALUE_PACK, orientation_normal);
            break;
        case Tile::RainbowPack:
            draw_layered_sprites<rainbow_pack_sprite_layers>(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, bg_color, orientation_normal, rainbow_pack_sprites, rainbow_pack_colors);
            break;
        case Tile::HeartPack:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, HEART_ICON_COLOR, bg_color, SPRITE_HEART, orientation_normal);
            break;
        case Tile::StarPack:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, STAR_ICON_COLOR, bg_color, SPRITE_STAR, orientation_normal);
            break;
        case Tile::Rock:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, ROCK_COLOR, bg_color, SPRITE_ROCK, orientation_normal);
            break;
        case Tile::Ghost:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, GHOST_COLOR, bg_color, (tile_x + tile_y) % 2 == 0 ? SPRITE_GHOST_1 : SPRITE_GHOST_2, orientation_normal);
            break;
        case Tile::Empty:
            draw_rect(fb, pixel_pos_x, pixel_pos_y, TILE_PIXELS, TILE_PIXELS, bg_color);
            break;
        default:
            break;
    }
}

void draw_snake_tile(FrameBufferImpl &fb, Snake &snake, int tile_x, int tile_y, Tile tile) {
    const int pixel_pos_x = tile_x * TILE_PIXELS;
    const int pixel_pos_y = tile_y * TILE_PIXELS;

    const uint32_t snake_body_color = snake.get_body_color() == SnakeColor::Rainbow
        ? calc_gradient_color(tile_x, tile_y, 25, 25)
        : static_cast<uint32_t>(snake.get_body_color());

    const uint32_t snake_scale_color = snake.get_scale_color() == SnakeColor::RainbowScale
        ? calc_gradient_color(tile_x + 2, tile_y + 2, 25, 25)
        : static_cast<uint32_t>(snake.get_scale_color());

    const uint32_t snake_segment_colors[snake_segment_sprite_layers] = { snake_body_color, snake_scale_color };
    const uint32_t bg_color = calc_bg_color(tile_x, tile_y);

    switch (tile) {
        case Tile::SnakeHead:
            draw_sprite(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, snake_body_color, bg_color, SPRITE_SNAKE_HEAD, orientation_from_dir(snake.get_direction()));
            break;
        case Tile::SnakeSegmentVertical:
            draw_layered_sprites<snake_segment_sprite_layers>(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, bg_color, orientation_from_dir(Direction::Up), snake_body_sprites, snake_segment_colors);
            break;
        case Tile::SnakeSegmentHorizontal:
            draw_layered_sprites<snake_segment_sprite_layers>(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, bg_color, orientation_from_dir(Direction::Right), snake_body_sprites, snake_segment_colors);
            break;
        case Tile::SnakeSegmentTopLeft:
            draw_layered_sprites<snake_segment_sprite_layers>(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, bg_color, orientation_from_dir(Direction::Left), snake_corner_sprites, snake_segment_colors);
            break;
        case Tile::SnakeSegmentTopRight:
            draw_layered_sprites<snake_segment_sprite_layers>(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, bg_color, orientation_from_dir(Direction::Up), snake_corner_sprites, snake_segment_colors);
            break;
        case Tile::SnakeSegmentBottomLeft:
            draw_layered_sprites<snake_segment_sprite_layers>(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, bg_color, orientation_from_dir(Direction::Down), snake_corner_sprites, snake_segment_colors);
            break;
        case Tile::SnakeSegmentBottomRight:
            draw_layered_sprites<snake_segment_sprite_layers>(fb, pixel_pos_x, pixel_pos_y, TILE_SPRITE_SCALE, bg_color, orientation_from_dir(Direction::Right), snake_corner_sprites, snake_segment_colors);
            break;
        default:
            break;
    }
}

void draw_ui_sprite(FrameBufferImpl &fb, int x, uint32_t color, uint64_t sprite) {
    draw_sprite(fb, x, UI_GLYPH_Y, UI_GLYPH_SCALE, color, BACKGROUND_COLOR_1, sprite, orientation_normal);
}

void draw_char(FrameBufferImpl &fb, int x, int y, int scale, uint32_t color, char c) {
    uint64_t sprite = 0;

    switch (c) {
        case ' ':
            break;
        case 'S':
            sprite = CHAR_S;
            break;
        case 'N':
            sprite = CHAR_N;
            break;
        case 'A':
            sprite = CHAR_A;
            break;
        case 'K':
            sprite = CHAR_K;
            break;
        case 'E':
            sprite = CHAR_E;
            break;
        case 'R':
            sprite = CHAR_R;
            break;
        case 'T':
            sprite = CHAR_T;
            break;
        case 'Q':
            sprite = CHAR_Q;
            break;
        case 'U':
            sprite = CHAR_U;
            break;
        case 'I':
            sprite = CHAR_I;
            break;
        case 'X':
            sprite = CHAR_X;
            break;
        case 'O':
            sprite = CHAR_O;
            break;
        case '+':
            sprite = CHAR_PLUS;
            break;
        case '?':
        default:
            sprite = CHAR_QUESTION;
            break;
    }

    draw_sprite(fb, x, y, scale, color, BACKGROUND_COLOR_1, sprite, orientation_normal);
}

void draw_string(FrameBufferImpl &fb, int x, int y, int scale, uint8_t spacing, uint32_t color, const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        draw_char(fb, x, y, scale, color, str[i]);
        x += (scale * 8) + spacing;
    }
}
