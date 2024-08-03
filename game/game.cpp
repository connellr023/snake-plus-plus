#include "game.hpp"
#include "../sprites/sprites.hpp"

void Game::tick() {
    draw_rect(this->fb, 0, 0, 200, 400, 0x0000FF);
}
