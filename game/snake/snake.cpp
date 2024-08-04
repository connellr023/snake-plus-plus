#include "snake.hpp"
#include "../game.hpp"

void Snake::loop() {
    SET_TILE_FN(this->notifier_callback, 0, 0, SNAKE);
}
