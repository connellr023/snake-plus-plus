#include "snake.hpp"
#include "../game.hpp"

void Snake::set_direction(Direction dir) {
    this->segments[this->head_idx].dir = dir;
}

void Snake::loop() {
    const Direction dir = this->segments[this->head_idx].dir;

    // if (this->length > 1) {

    // }

    // Update head index
    this->head_idx = (this->head_idx + 1) % this->max_length;

    // Initialize next head position with previous head position and direction
    const uint8_t prev = (this->head_idx - 1 + this->max_length) % this->max_length;

    this->segments[this->head_idx].x = this->segments[prev].x;
    this->segments[this->head_idx].y = this->segments[prev].y;
    this->segments[this->head_idx].dir = dir;

    // Update new head position based on direction
    switch (dir) {
        case Direction::Up:
            this->segments[this->head_idx].y = (this->segments[this->head_idx].y - 1 + this->game.get_grid_height()) % this->game.get_grid_height();
            break;
        case Direction::Down:
            this->segments[this->head_idx].y = (this->segments[this->head_idx].y + 1) % this->game.get_grid_height();
            break;
        case Direction::Left:
            this->segments[this->head_idx].x = (this->segments[this->head_idx].x - 1 + this->game.get_grid_width()) % this->game.get_grid_width();
            break;
        case Direction::Right:
            this->segments[this->head_idx].x = (this->segments[this->head_idx].x + 1) % this->game.get_grid_width();
            break;
    }

    // Render changes
    this->game.set_tile(this->segments[this->head_idx].x, this->segments[this->head_idx].y, Tile::Snake);

    // Clear previous tail position
    this->game.set_tile(this->segments[this->tail_idx].x, this->segments[this->tail_idx].y, Tile::Empty);

    // Update tail index
    this->tail_idx = (this->tail_idx + 1) % this->max_length;
}
