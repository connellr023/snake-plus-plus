#ifndef SNAKE_H
#define SNAKE_H
#include <memory>
#include <stdint.h>
#include "../../rendering/colors.hpp"

#define FOOD_GROW_AMOUNT    6

class Game;

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

struct Segment {
    uint8_t x;
    uint8_t y;
    Direction dir;
};

class Snake {
private:
    Game &game;

    std::unique_ptr<Segment[]> segments;

    uint8_t head_idx;
    uint8_t tail_idx;
    uint8_t length;
    uint8_t max_length;
    uint32_t color;

    bool grow();
    void collect_food();

public:
    Snake(Game &game, uint8_t start_x, uint8_t start_y, uint8_t max_length) : game(game), max_length(max_length) {
        this->segments = std::make_unique<Segment[]>(max_length);

        this->head_idx = 0;
        this->tail_idx = 0;
        this->length = 1;

        this->segments[this->head_idx].x = start_x;
        this->segments[this->head_idx].y = start_y;
        this->segments[this->head_idx].dir = Direction::Right;

        this->color = SNAKE_COLOR;
    }

    void loop();
    void set_direction(Direction dir);
};

#endif // SNAKE_H
