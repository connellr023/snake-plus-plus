#include "ghost.hpp"

void Ghost::update() {
    uint8_t next_x = this->x;
    uint8_t next_y = this->y;

    bool at_edge = false;

    switch (this->dir) {
        case Direction::Up: {
            if (this->y == 0) at_edge = true;
            else next_y--;

            break;
        }
        case Direction::Down: {
            if (this->y == this->game.get_grid_height() - 1) at_edge = true;
            else next_y++;

            break;
        }
        case Direction::Left: {
            if (this->x == 0) at_edge = true;
            else next_x--;

            break;
        }
        case Direction::Right: {
            if (this->x == this->game.get_grid_width() - 1) at_edge = true;
            else next_x++;

            break;
        }
    }

    if (this->game.get_tile(next_x, next_y) != Tile::Empty || at_edge) {
        this->randomize_dir();
    }
    else {
        this->game.set_tile(this->x, this->y, Tile::Empty);
        this->game.render_tile(this->x, this->y, Tile::Empty);

        this->x = next_x;
        this->y = next_y;

        this->game.set_tile(this->x, this->y, Tile::Ghost);
        this->game.render_tile(this->x, this->y, Tile::Ghost);
    }
}
