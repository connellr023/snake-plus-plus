#ifndef SNAKE_H
#define SNAKE_H
#include <cstdint>
#include <memory>
#include <stdint.h>
#include <functional>

#define FOOD_GROW_AMOUNT    3

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

typedef std::function<void(Segment *)> segment_iterator_t;

class Snake {
private:
    Game &game;

    std::unique_ptr<Segment[]> segments;

    uint8_t head_idx;
    uint8_t tail_idx;
    uint8_t length;
    uint8_t max_length;
    uint32_t color;

    bool can_use_portal = false;

    void init(uint8_t start_x, uint8_t start_y);
    void foreach_segment(segment_iterator_t iter);

    bool grow();
    void collect_food();
    void collect_portal();
    void on_portal_exit();
    void update_color(uint32_t color);

public:
    Snake(Game &game, uint8_t start_x, uint8_t start_y, uint8_t max_length) : game(game), max_length(max_length) {
        this->segments = std::make_unique<Segment[]>(max_length);
        this->init(start_x, start_y);
    }

    void loop();
    void reset(uint8_t start_x, uint8_t start_y);
    void set_direction(Direction dir);

    Direction get_direction() const {
        return this->segments[this->head_idx].dir;
    }

    uint32_t get_color() const {
        return this->color;
    }
};

#endif // SNAKE_H
