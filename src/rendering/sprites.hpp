#ifndef SPRITES_H
#define SPRITES_H

#define SPRITE_CLOCK \
    (0b00000000ULL << 56) | \
    (0b00011000ULL << 48) | \
    (0b00100100ULL << 40) | \
    (0b01010010ULL << 32) | \
    (0b01011010ULL << 24) | \
    (0b00100100ULL << 16) | \
    (0b00011000ULL <<  8) | \
    (0b00000000ULL <<  0) \

#define SPRITE_PAUSED_ICON \
    (0b01100110ULL << 56) | \
    (0b01100110ULL << 48) | \
    (0b01100110ULL << 40) | \
    (0b01100110ULL << 32) | \
    (0b01100110ULL << 24) | \
    (0b01100110ULL << 16) | \
    (0b01100110ULL <<  8) | \
    (0b01100110ULL <<  0) \

#define SPRITE_RESUMED_ICON \
    (0b00100000ULL << 56) | \
    (0b00110000ULL << 48) | \
    (0b00111000ULL << 40) | \
    (0b00111100ULL << 32) | \
    (0b00111100ULL << 24) | \
    (0b00111000ULL << 16) | \
    (0b00110000ULL <<  8) | \
    (0b00100000ULL <<  0) \

#define SPRITE_TROPHY \
    (0b00000000ULL << 56) | \
    (0b11100111ULL << 48) | \
    (0b10111101ULL << 40) | \
    (0b10111101ULL << 32) | \
    (0b10111101ULL << 24) | \
    (0b01011010ULL << 16) | \
    (0b00011000ULL <<  8) | \
    (0b00111100ULL <<  0) \

#define SPRITE_HEART \
    (0b00000000ULL << 56) | \
    (0b01100110ULL << 48) | \
    (0b11111111ULL << 40) | \
    (0b11111111ULL << 32) | \
    (0b01111110ULL << 24) | \
    (0b00111100ULL << 16) | \
    (0b00011000ULL <<  8) | \
    (0b00000000ULL <<  0) \

#define SPRITE_STAR \
    (0b00000000ULL << 56) | \
    (0b00010000ULL << 48) | \
    (0b00111000ULL << 40) | \
    (0b11111110ULL << 32) | \
    (0b01111100ULL << 24) | \
    (0b01101100ULL << 16) | \
    (0b11000110ULL <<  8) | \
    (0b00000000ULL <<  0) \

#define SPRITE_ROCK \
    (0b00000000ULL << 56) | \
    (0b00011000ULL << 48) | \
    (0b00011000ULL << 40) | \
    (0b00111100ULL << 32) | \
    (0b00111100ULL << 24) | \
    (0b01111110ULL << 16) | \
    (0b01111110ULL <<  8) | \
    (0b01111110ULL <<  0) \

#define SPRITE_GHOST_1 \
    (0b00111110ULL << 56) | \
    (0b01001001ULL << 48) | \
    (0b01001001ULL << 40) | \
    (0b01101101ULL << 32) | \
    (0b01111111ULL << 24) | \
    (0b01100011ULL << 16) | \
    (0b01111111ULL <<  8) | \
    (0b01010101ULL <<  0) \

#define SPRITE_GHOST_2 \
    (0b00111110ULL << 56) | \
    (0b01001001ULL << 48) | \
    (0b01001001ULL << 40) | \
    (0b01101101ULL << 32) | \
    (0b01111111ULL << 24) | \
    (0b01100011ULL << 16) | \
    (0b01111111ULL <<  8) | \
    (0b00101010ULL <<  0) \

#define SPRITE_SNAKE_HEAD \
    (0b00000000ULL << 56) | \
    (0b00011000ULL << 48) | \
    (0b00111100ULL << 40) | \
    (0b01011010ULL << 32) | \
    (0b01011010ULL << 24) | \
    (0b01111110ULL << 16) | \
    (0b00111100ULL <<  8) | \
    (0b00111100ULL <<  0) \

#define SPRITE_SNAKE_BODY \
    (0b00111100ULL << 56) | \
    (0b00111100ULL << 48) | \
    (0b00101110ULL << 40) | \
    (0b01100110ULL << 32) | \
    (0b01110110ULL << 24) | \
    (0b01111100ULL << 16) | \
    (0b00111100ULL <<  8) | \
    (0b00111100ULL <<  0) \

#define SPRITE_SNAKE_BODY_SCALE \
    (0b00000000ULL << 56) | \
    (0b00000000ULL << 48) | \
    (0b00010000ULL << 40) | \
    (0b00011000ULL << 32) | \
    (0b00001000ULL << 24) | \
    (0b00000000ULL << 16) | \
    (0b00000000ULL <<  8) | \
    (0b00000000ULL <<  0) \

#define SPRITE_SNAKE_CORNER \
    (0b00000000ULL << 56) | \
    (0b11110000ULL << 48) | \
    (0b11111100ULL << 40) | \
    (0b11001100ULL << 32) | \
    (0b11100110ULL << 24) | \
    (0b11110110ULL << 16) | \
    (0b00111110ULL <<  8) | \
    (0b00111110ULL <<  0) \

#define SPRITE_SNAKE_CORNER_SCALE \
    (0b00000000ULL << 56) | \
    (0b00000000ULL << 48) | \
    (0b00000000ULL << 40) | \
    (0b00110000ULL << 32) | \
    (0b00011000ULL << 24) | \
    (0b00001000ULL << 16) | \
    (0b00000000ULL <<  8) | \
    (0b00000000ULL <<  0) \

#define SPRITE_VALUE_PACK \
    (0b00000000ULL << 56) | \
    (0b00011000ULL << 48) | \
    (0b00111100ULL << 40) | \
    (0b01111110ULL << 32) | \
    (0b01111110ULL << 24) | \
    (0b00111100ULL << 16) | \
    (0b00011000ULL <<  8) | \
    (0b00000000ULL <<  0) \

#define SPRITE_VALUE_PACK_INNER_1 \
    (0b00000000ULL << 56) | \
    (0b00000000ULL << 48) | \
    (0b00011000ULL << 40) | \
    (0b00111100ULL << 32) | \
    (0b00111100ULL << 24) | \
    (0b00011000ULL << 16) | \
    (0b00000000ULL <<  8) | \
    (0b00000000ULL <<  0) \

#define SPRITE_VALUE_PACK_INNER_2 \
    (0b00000000ULL << 56) | \
    (0b00000000ULL << 48) | \
    (0b00000000ULL << 40) | \
    (0b00011000ULL << 32) | \
    (0b00011000ULL << 24) | \
    (0b00000000ULL << 16) | \
    (0b00000000ULL <<  8) | \
    (0b00000000ULL <<  0) \

#endif // SPRITES_H
