#ifndef GAME_H
#define GAME_H
#include <cstdint>
#include <memory>
#include <functional>
#include <set>
#include "snake/snake.hpp"
#include "../framebuffer/framebuffer.hpp"

#define SNAKE_SPAWN_X   3
#define SNAKE_SPAWN_Y   3

#define FOOD_LIFETIME   40

#define SNAKE_MOVE_MS   85
#define FOOD_SPAWN_MS   4000
#define LIFE_TILE_MS    120

#define MAX_SNAKE_SIZE  100
#define MAX_LIVES       10

typedef std::function<void()> interval_listener_t;

enum class Tile {
    Empty,
    Snake,
    Food
};

struct IntervalListenerWrapper {
    uint64_t last_interval_ms;
    interval_listener_t listener;
};

struct LifetimeTileWrapper {
    Tile tile;
    uint8_t tile_x;
    uint8_t tile_y;
    uint64_t life_left;
};

class Game {
private:
    FrameBuffer &fb;

    int grid_width;
    int grid_height;
    int tick_ms;

    std::unique_ptr<Tile[]> grid;
    std::unique_ptr<Snake> snake;

    std::set<std::shared_ptr<LifetimeTileWrapper>> lifetime_tiles;

    std::map<int, IntervalListenerWrapper> interval_listeners;

    uint16_t score = 0;
    uint8_t lives = 0;

    void register_interval_listener(int interval_ms, interval_listener_t listener) {
        this->interval_listeners[interval_ms] = IntervalListenerWrapper {
            .last_interval_ms = 0,
            .listener = listener
        };
    }

    void generate_map();
    void generate_food();

public:
    Game(FrameBuffer &fb, int grid_width, int grid_height) :
        fb(fb),
        grid_width(grid_width),
        grid_height(grid_height)
    {
        this->grid = std::make_unique<Tile[]>(grid_width * grid_height);
        this->snake = std::unique_ptr<Snake>(new Snake(*this, SNAKE_SPAWN_X, SNAKE_SPAWN_Y, MAX_SNAKE_SIZE));
    }

    void set_lives(uint8_t lives);
    void set_tile(int x, int y, Tile tile);
    Tile get_tile(int x, int y);

    int get_grid_width() const {
        return grid_width;
    }

    int get_grid_height() const {
        return grid_height;
    }

    void decrease_lives();

    void init();
    void loop();
};

#endif // GAME_H
