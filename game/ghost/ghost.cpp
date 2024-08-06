#include "ghost.hpp"

void Ghost::update() {
    this->game.set_tile(this->x, this->y, Tile::Ghost);
}
