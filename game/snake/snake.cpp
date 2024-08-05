#include "snake.hpp"
#include "../game.hpp"
#include "../../rendering/colors.hpp"

void Snake::set_direction(Direction dir) {
    const Direction current_dir = this->segments[this->head_idx].dir;

    if (this->length > 1) {
        if (current_dir == Direction::Up && dir == Direction::Down) {
            return;
        }

        if (current_dir == Direction::Down && dir == Direction::Up) {
            return;
        }

        if (current_dir == Direction::Left && dir == Direction::Right) {
            return;
        }

        if (current_dir == Direction::Right && dir == Direction::Left) {
            return;
        }
    }

    this->segments[this->head_idx].dir = dir;
}

void Snake::foreach_segment(segment_iterator_t iter) {
    uint8_t idx = this->head_idx;

    // Iterate until tail is reached
    while (true) {
        iter(&this->segments[idx]);

        if (idx == this->tail_idx) {
            break;
        }

        idx = (idx - 1 + this->max_length) % this->max_length;
    }
}

void Snake::init(uint8_t start_x, uint8_t start_y) {
    this->length = 1;
    this->head_idx = 0;
    this->tail_idx = 0;

    this->segments[0].x = start_x;
    this->segments[0].y = start_y;
    this->segments[0].dir = Direction::Right;

    this->color = SNAKE_COLOR;
}

void Snake::reset(uint8_t start_x, uint8_t start_y) {
    // Clear snake from grid
    this->foreach_segment([this](Segment *segment) {
        this->game.set_tile(segment->x, segment->y, Tile::Empty);
    });

    // Reinitialize snake
    this->init(start_x, start_y);

    // Weird hack to reset the snake properly
    this->grow();
    this->length--;
}

bool Snake::grow() {
    if (this->length >= this->max_length - 2) {
        return false;
    }

    this->length++;

    // Add new segment
    const uint8_t new_tail_idx = (this->tail_idx - 1 + this->max_length) % this->max_length;

    this->segments[new_tail_idx].x = this->segments[this->tail_idx].x;
    this->segments[new_tail_idx].y = this->segments[this->tail_idx].y;

    this->tail_idx = new_tail_idx;

    return true;
}

void Snake::collect_food() {
    for (uint8_t i = 0; i < FOOD_GROW_AMOUNT; i++) {
        if (!this->grow()) {
            break;
        }
    }
}

void Snake::loop() {
    const Direction dir = this->segments[this->head_idx].dir;

    if (this->length > 1) {
        const Direction prev_dir = this->segments[(this->head_idx - 1 + this->max_length) % this->max_length].dir;
        Tile new_segment;

        switch (dir) {
            case Direction::Up: {
                switch (prev_dir) {
                    case Direction::Left:
                        new_segment = Tile::SnakeSegmentBottomLeft;
                        break;
                    case Direction::Right:
                        new_segment = Tile::SnakeSegmentBottomRight;
                        break;
                    default:
                        new_segment = Tile::SnakeSegmentVertical;
                        break;
                }

                break;
            }
            case Direction::Down: {
                switch (prev_dir) {
                    case Direction::Left:
                        new_segment = Tile::SnakeSegmentTopLeft;
                        break;
                    case Direction::Right:
                        new_segment = Tile::SnakeSegmentTopRight;
                        break;
                    default:
                        new_segment = Tile::SnakeSegmentVertical;
                        break;
                }

                break;
            }
            case Direction::Left: {
                switch (prev_dir) {
                    case Direction::Up:
                        new_segment = Tile::SnakeSegmentTopRight;
                        break;
                    case Direction::Down:
                        new_segment = Tile::SnakeSegmentBottomRight;
                        break;
                    default:
                        new_segment = Tile::SnakeSegmentHorizontal;
                        break;
                }

                break;
            }
            case Direction::Right: {
                switch (prev_dir) {
                    case Direction::Up:
                        new_segment = Tile::SnakeSegmentTopLeft;
                        break;
                    case Direction::Down:
                        new_segment = Tile::SnakeSegmentBottomLeft;
                        break;
                    default:
                        new_segment = Tile::SnakeSegmentHorizontal;
                        break;
                }

                break;
            }
        }

        this->game.set_tile(this->segments[this->head_idx].x, this->segments[this->head_idx].y, new_segment);
    }

    // Update head index
    this->head_idx = (this->head_idx + 1) % this->max_length;

    // Initialize next head position with previous head position and direction
    const uint8_t prev_head_idx = (this->head_idx - 1 + this->max_length) % this->max_length;

    this->segments[this->head_idx].x = this->segments[prev_head_idx].x;
    this->segments[this->head_idx].y = this->segments[prev_head_idx].y;
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

    // Collision detection
    switch (this->game.get_tile(this->segments[this->head_idx].x, this->segments[this->head_idx].y)) {
        case Tile::SnakeSegmentTopLeft:
        case Tile::SnakeSegmentTopRight:
        case Tile::SnakeSegmentBottomLeft:
        case Tile::SnakeSegmentBottomRight:
        case Tile::SnakeSegmentHorizontal:
        case Tile::SnakeSegmentVertical:
        case Tile::SnakeHead:
            this->game.decrease_lives();
            break;
        case Tile::Food:
            this->collect_food();
            break;
        default:
            break;
    }

    // Render changes
    this->game.set_tile(this->segments[this->head_idx].x, this->segments[this->head_idx].y, Tile::SnakeHead);

    // Clear previous tail position
    this->game.set_tile(this->segments[this->tail_idx].x, this->segments[this->tail_idx].y, Tile::Empty);

    // Update tail index
    this->tail_idx = (this->tail_idx + 1) % this->max_length;
}
