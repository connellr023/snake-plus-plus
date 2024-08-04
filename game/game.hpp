#ifndef GAME_H
#define GAME_H
#include <memory>
#include "../framebuffer/framebuffer.hpp"

typedef enum {
    EMPTY
} tile_t;

class Game {
private:
    FrameBuffer &fb;

    int grid_width;
    int grid_height;
    uint8_t max_lives;
    int ticks_per_second;
    uint64_t last_tick;

    std::unique_ptr<tile_t[]> grid;

    uint16_t score = 0;
    uint8_t lives = 0;

    void tick();
    void set_tile(int x, int y, tile_t tile);
    tile_t get_tile(int x, int y);

public:
    Game(FrameBuffer &fb, int grid_width, int grid_height, int max_lives, int ticks_per_second) :
        fb(fb),
        grid_width(grid_width),
        grid_height(grid_height),
        max_lives(max_lives),
        ticks_per_second(ticks_per_second)
    {
        grid = std::make_unique<tile_t[]>(grid_width * grid_height);
    }

    void init();
    void loop();
};

#endif // GAME_H
