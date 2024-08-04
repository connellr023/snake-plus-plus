#ifndef GAME_H
#define GAME_H
#include <memory>
#include "snake/snake.hpp"
#include "../framebuffer/framebuffer.hpp"

#define SNAKE_SPAWN_X   3
#define SNAKE_SPAWN_Y   3

#define MAX_SNAKE_SIZE  100
#define MAX_LIVES       10

enum class Tile {
    Empty,
    Snake
};

class Game {
private:
    FrameBuffer &fb;

    int grid_width;
    int grid_height;
    int tick_ms;
    uint64_t last_tick;

    std::unique_ptr<Tile[]> grid;
    std::unique_ptr<Snake> snake;

    uint16_t score = 0;
    uint8_t lives = 0;

    void tick();

public:
    Game(FrameBuffer &fb, int grid_width, int grid_height, int tick_ms) :
        fb(fb),
        grid_width(grid_width),
        grid_height(grid_height),
        tick_ms(tick_ms)
    {
        this->grid = std::make_unique<Tile[]>(grid_width * grid_height);
        this->snake = std::unique_ptr<Snake>(new Snake(*this, SNAKE_SPAWN_X, SNAKE_SPAWN_Y, MAX_SNAKE_SIZE));
    }

    void set_lives(uint8_t lives);
    void set_tile(int x, int y, Tile tile);
    Tile get_tile(int x, int y);

    int get_grid_width() const { return grid_width; }
    int get_grid_height() const { return grid_height; }

    void init();
    void loop();
};

#endif // GAME_H
