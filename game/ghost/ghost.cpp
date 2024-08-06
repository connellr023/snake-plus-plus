#include "ghost.hpp"

void Ghost::move(Direction dir) {
    int new_x = this->x;
    int new_y = this->y;

    switch (dir) {
        case Direction::Up: {
            if (this->y == 0) {
                return;
            }

            new_y--;
            break;
        }
        case Direction::Down: {
            if (this->y == this->game.get_grid_height() - 1) {
                return;
            }

            new_y++;
            break;
        }
        case Direction::Left: {
            if (this->x == 0) {
                return;
            }

            new_x--;
            break;
        }
        case Direction::Right: {
            if (this->x == this->game.get_grid_width() - 1) {
                return;
            }

            new_x++;
            break;
        }
    }

    if (this->game.get_tile(new_x, new_y) != Tile::Empty) {
        return;
    }

    this->game.set_tile(this->x, this->y, Tile::Empty);
    this->x = new_x;
    this->y = new_y;
    this->game.set_tile(this->x, this->y, Tile::Ghost);
}

void Ghost::update() {
    this->move(Direction::Right);
}
