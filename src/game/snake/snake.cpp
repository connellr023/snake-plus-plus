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

void Snake::update_color(uint32_t color) {
    this->color = color;
    this->foreach_segment([this](Segment *segment) {
        // Re-render segment with new color
        this->game.set_tile(segment->x, segment->y, this->game.get_tile(segment->x, segment->y));
    });
}

void Snake::collect_portal() {
    if (this->can_use_portal || this->in_star_mode) {
        return;
    }

    this->update_color(SNAKE_COLOR_1);
    this->can_use_portal = true;
    this->can_use_attack = false;
}

void Snake::on_portal_exit() {
    if (this->in_star_mode) {
        return;
    }

    this->color = SNAKE_COLOR;
    this->can_use_portal = false;
}

void Snake::collect_attack() {
    if (this->can_use_attack || this->in_star_mode) {
        return;
    }

    this->update_color(SNAKE_COLOR_2);
    this->can_use_attack = true;
    this->can_use_portal = false;
}

void Snake::on_attack_exit() {
    for (uint8_t i = 0; i < GHOST_GROW_AMOUNT; i++) {
        if (!this->grow()) {
            break;
        }
    }

    if (this->in_star_mode) {
        return;
    }

    this->update_color(SNAKE_COLOR);
    this->can_use_attack = false;
}

void Snake::collect_star() {
    if (this->in_star_mode) {
        this->on_star_exit();
        return;
    }

    this->update_color(STAR_ICON_COLOR);

    this->in_star_mode = true;
    this->can_use_attack = true;
    this->can_use_portal = true;
    this->update_ms = STAR_SNAKE_UPDATE_MS;
}

void Snake::on_star_exit() {
    this->update_color(SNAKE_COLOR);

    this->in_star_mode = false;
    this->can_use_attack = false;
    this->can_use_portal = false;
    this->update_ms = SNAKE_UPDATE_MS;
}

void Snake::init(uint8_t start_x, uint8_t start_y) {
    this->length = 1;
    this->head_idx = 0;
    this->tail_idx = 0;

    this->segments[this->head_idx].x = start_x;
    this->segments[this->head_idx].y = start_y;
    this->segments[this->head_idx].dir = Direction::Right;

    this->can_use_attack = false;
    this->can_use_portal = false;
    this->in_star_mode = false;

    this->color = SNAKE_COLOR;
    this->update_ms = SNAKE_UPDATE_MS;
}

void Snake::reset(uint8_t start_x, uint8_t start_y) {
    // Clear snake from grid
    this->foreach_segment([this](Segment *segment) {
        this->game.set_tile(segment->x, segment->y, Tile::Empty);
    });

    this->init(start_x, start_y);
    this->game.set_score(0);
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

    this->game.set_score(this->length);

    return true;
}

void Snake::collect_food() {
    for (uint8_t i = 0; i < FOOD_GROW_AMOUNT; i++) {
        if (!this->grow()) {
            break;
        }
    }
}

void Snake::update() {
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
        case Direction::Up: {
            if (this->segments[this->head_idx].y == 0) {
                if (this->can_use_portal) {
                    this->segments[this->head_idx].y = this->game.get_grid_height() - 1;
                    this->on_portal_exit();
                }
                else {
                    this->game.decrease_lives();
                    return;
                }
            }
            else {
                this->segments[this->head_idx].y--;
            }

            break;
        }
        case Direction::Down: {
            if (this->segments[this->head_idx].y == this->game.get_grid_height() - 1) {
                if (this->can_use_portal) {
                    this->segments[this->head_idx].y = 0;
                    this->on_portal_exit();
                }
                else {
                    this->game.decrease_lives();
                    return;
                }
            }
            else {
                this->segments[this->head_idx].y++;
            }

            break;
        }
        case Direction::Left: {
            if (this->segments[this->head_idx].x == 0) {
                if (this->can_use_portal) {
                    this->segments[this->head_idx].x = this->game.get_grid_width() - 1;
                    this->on_portal_exit();
                }
                else {
                    this->game.decrease_lives();
                    return;
                }
            }
            else {
                this->segments[this->head_idx].x--;
            }

            break;
        }
        case Direction::Right: {
            if (this->segments[this->head_idx].x == this->game.get_grid_width() - 1) {
                if (this->can_use_portal) {
                    this->segments[this->head_idx].x = 0;
                    this->on_portal_exit();
                }
                else {
                    this->game.decrease_lives();
                    return;
                }
            }
            else {
                this->segments[this->head_idx].x++;
            }

            break;
        }
    }

    // Tile to be written back to the grid after collision detection if the snake dies
    Tile write_back_tile = Tile::Empty;

    // Collision detection
    switch (this->game.get_tile(this->segments[this->head_idx].x, this->segments[this->head_idx].y)) {
        case Tile::Rock:
            write_back_tile = Tile::Rock;
            goto case_dead;
        case Tile::Ghost: {
            if (this->can_use_attack) {
                this->game.kill_entity_at_pos(this->segments[this->head_idx].x, this->segments[this->head_idx].y);
                this->on_attack_exit();
                break;
            }

            write_back_tile = Tile::Ghost;
            goto case_dead;
        }
        case Tile::SnakeSegmentTopLeft:
        case Tile::SnakeSegmentTopRight:
        case Tile::SnakeSegmentBottomLeft:
        case Tile::SnakeSegmentBottomRight:
        case Tile::SnakeSegmentHorizontal:
        case Tile::SnakeSegmentVertical:
        case Tile::SnakeHead:
        case_dead: {
            const uint8_t head_x = this->segments[this->head_idx].x;
            const uint8_t head_y = this->segments[this->head_idx].y;

            this->game.decrease_lives();
            this->game.set_tile(head_x, head_y, write_back_tile);

            return;
        }
        case Tile::Food:
            this->collect_food();
            break;
        case Tile::PortalPack:
            this->collect_portal();
            break;
        case Tile::AttackPack:
            this->collect_attack();
            break;
        case Tile::StarPack:
            this->collect_star();
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