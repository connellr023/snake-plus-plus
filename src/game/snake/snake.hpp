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

enum class SnakeColor : uint32_t {
    Normal = 0x36D161,
    NormalScale = 0x118C34,
    Portal = 0x44CBDB,
    PortalScale = 0x137985,
    Attack = 0xE81522,
    AttackScale = 0x8C070F,
    Rainbow,
    RainbowScale
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
    uint8_t stars_collected;

    SnakeColor body_color;
    SnakeColor scale_color;

    bool can_use_portal;
    bool can_use_attack;
    bool in_rainbow_mode;

    void init(uint8_t start_x, uint8_t start_y);
    void foreach_segment(segment_iterator_t iter);

    bool grow();
    void collect_food();
    void collect_portal();
    void collect_attack();
    void collect_rainbow();
    void on_portal_exit();
    void on_attack_exit();
    void on_rainbow_exit();
    void update_color(SnakeColor body_color, SnakeColor scale_color);

public:
    Snake(Game &game, uint8_t start_x, uint8_t start_y, uint8_t max_length) :
        Entity(game, start_x, start_y, SNAKE_UPDATE_MS),
        max_length(max_length),
        stars_collected(0),
        body_color(SnakeColor::Normal),
        scale_color(SnakeColor::NormalScale)
    {
        this->segments = std::make_unique<Segment[]>(max_length);
        this->init(start_x, start_y);
    }

    void update() override;
    void reset(uint8_t start_x, uint8_t start_y);
    void set_direction(Direction dir);

    uint8_t get_length() const {
        return this->length;
    }

    uint8_t get_star_count() const {
        return this->stars_collected;
    }

    Segment get_head_segment() const {
        return this->segments[this->head_idx];
    }

    SnakeColor get_body_color() const {
        return this->body_color;
    }

    SnakeColor get_scale_color() const {
        return this->scale_color;
    }

    bool can_attack() const {
        return this->can_use_attack;
    }
};

#endif // SNAKE_H
