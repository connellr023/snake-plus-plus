#ifndef SNAKE_H
#define SNAKE_H
#include <cstdint>
#include <memory>
#include <stdint.h>
#include <functional>
#include "../entity/entity.hpp"

#define SNAKE_UPDATE_MS         80
#define STAR_SNAKE_UPDATE_MS    55

#define FOOD_GROW_AMOUNT        2
#define GHOST_GROW_AMOUNT       4

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

class Snake : public Entity {
private:
    std::unique_ptr<Segment[]> segments;

    uint8_t head_idx;
    uint8_t tail_idx;
    uint8_t length;
    uint8_t max_length;
    uint32_t color;

    bool can_use_portal;
    bool can_use_attack;
    bool in_star_mode;

    void init(uint8_t start_x, uint8_t start_y);
    void foreach_segment(segment_iterator_t iter);

    bool grow();
    void collect_food();
    void collect_portal();
    void collect_attack();
    void collect_star();
    void on_portal_exit();
    void on_attack_exit();
    void on_star_exit();
    void update_color(uint32_t color);

public:
    Snake(Game &game, uint8_t start_x, uint8_t start_y, uint8_t max_length) : Entity(game, start_x, start_y, SNAKE_UPDATE_MS), max_length(max_length) {
        this->segments = std::make_unique<Segment[]>(max_length);
        this->init(start_x, start_y);
    }

    void update() override;
    void reset(uint8_t start_x, uint8_t start_y);
    void set_direction(Direction dir);

    Segment get_head_segment() const {
        return this->segments[this->head_idx];
    }

    uint32_t get_color() const {
        return this->color;
    }

    bool can_attack() const {
        return this->can_use_attack;
    }
};

#endif // SNAKE_H
