#ifndef GAME_H
#define GAME_H
#include <memory>
#include <map>
#include "game_notifier.hpp"
#include "snake/snake.hpp"
#include "../framebuffer/framebuffer.hpp"

#define SNAKE_SPAWN_X   3
#define SNAKE_SPAWN_Y   3

#define MAX_LIVES       10

typedef enum {
    EMPTY,
    SNAKE
} tile_t;

class Game {
private:
    FrameBuffer &fb;

    int grid_width;
    int grid_height;
    int ticks_per_second;
    uint64_t last_tick;

    std::unique_ptr<tile_t[]> grid;
    std::unique_ptr<Snake> snake;

    std::map<notification_t, notification_listener_t> listeners;

    uint16_t score = 0;
    uint8_t lives = 0;

    void tick();

    void register_listener(notification_t notification, notification_listener_t listener);

    void set_lives(uint8_t lives);
    void set_tile(int x, int y, tile_t tile);
    tile_t get_tile(int x, int y);

public:
    Game(FrameBuffer &fb, int grid_width, int grid_height, int ticks_per_second) :
        fb(fb),
        grid_width(grid_width),
        grid_height(grid_height),
        ticks_per_second(ticks_per_second)
    {
        const notifier_callback_t callback = [this](notification_t notification, void *data) {
            if (this->listeners.find(notification) != this->listeners.end()) {
                this->listeners[notification](data);
            }
        };

        this->grid = std::make_unique<tile_t[]>(grid_width * grid_height);
        this->snake = std::unique_ptr<Snake>(new Snake(callback, SNAKE_SPAWN_X, SNAKE_SPAWN_Y));
    }

    void init();
    void loop();
};

#endif // GAME_H
