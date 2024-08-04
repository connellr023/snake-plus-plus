#ifndef SNAKE_H
#define SNAKE_H
#include <memory>
#include <stdint.h>
#include "../../rendering/colors.hpp"
#include "../game_notifier.hpp"

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction_t;

typedef struct {
    uint8_t x;
    uint8_t y;
    direction_t dir;
} segment_t;

class Snake {
private:
    notifier_callback_t notifier_callback;

    std::unique_ptr<segment_t[]> segments;

    uint8_t head_idx;
    uint8_t tail_idx;
    uint8_t length;
    uint32_t color;

public:
    Snake(notifier_callback_t notifier_callback, uint8_t start_x, uint8_t start_y) : notifier_callback(notifier_callback) {
        this->segments = std::make_unique<segment_t[]>(length);

        this->head_idx = 0;
        this->tail_idx = 0;
        this->length = 1;

        this->segments[head_idx].x = start_x;
        this->segments[head_idx].y = start_y;
        this->segments[head_idx].dir = RIGHT;

        this->color = SNAKE_COLOR;
    }

    void loop();
};

#endif // SNAKE_H
