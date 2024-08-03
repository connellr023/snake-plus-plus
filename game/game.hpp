#ifndef GAME_H
#define GAME_H
#include <memory>
#include "../framebuffer/framebuffer.hpp"

typedef enum {
    EMPTY
} tile_t;

class Game {
private:
    FrameBuffer& fb;

    int grid_width;
    int grid_height;

    std::unique_ptr<tile_t[]> grid;

public:
    Game(FrameBuffer& fb, int grid_width, int grid_height) : fb(fb), grid_width(grid_width), grid_height(grid_height) {
        grid = std::make_unique<tile_t[]>(grid_width * grid_height);
    }

    void tick();
};

#endif // GAME_H
