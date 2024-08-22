#include "snake.hpp"
#include "../game.hpp"

Snake::Snake(Game &game, uint8_t spawn_x, uint8_t spawn_y, uint8_t max_length) :
    Entity(game, spawn_x, spawn_y, SNAKE_UPDATE_MS),
    spawn_x(spawn_x),
    spawn_y(spawn_y),
    max_length(max_length),
    body_color(SnakeColor::Normal),
    scale_color(SnakeColor::NormalScale)
{
    this->segments = std::make_unique<Segment[]>(max_length);
    this->init();
}

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
    uint8_t idx = (this->head_idx - 1 + this->max_length) % this->max_length;

    // Iterate until tail is reached
    while (true) {
        iter(&this->segments[idx]);

        if (idx == this->tail_idx) {
            return;
        }

        idx = (idx - 1 + this->max_length) % this->max_length;
    }
}

void Snake::update_color(SnakeColor body_color, SnakeColor scale_color) {
    this->body_color = body_color;
    this->scale_color = scale_color;

    this->foreach_segment([this](Segment *segment) {
        // Re-render segment with new color
        this->game.render_snake_tile(segment->x, segment->y, this->game.get_tile(segment->x, segment->y));
    });
}

void Snake::collect_portal() {
    if (this->can_use_portal || this->in_rainbow_mode) {
        return;
    }

    this->update_color(SnakeColor::Portal, SnakeColor::PortalScale);
    this->can_use_portal = true;
    this->can_use_attack = false;
}

void Snake::on_portal_exit() {
    if (this->in_rainbow_mode) {
        return;
    }

    this->body_color = SnakeColor::Normal;
    this->scale_color = SnakeColor::NormalScale;
    this->can_use_portal = false;
}

void Snake::collect_attack() {
    if (this->can_use_attack || this->in_rainbow_mode) {
        return;
    }

    this->update_color(SnakeColor::Attack, SnakeColor::AttackScale);
    this->can_use_attack = true;
    this->can_use_portal = false;
}

void Snake::on_attack_exit() {
    for (uint8_t i = 0; i < GHOST_GROW_AMOUNT; i++) {
        if (!this->try_grow()) {
            break;
        }
    }

    if (this->in_rainbow_mode) {
        return;
    }

    this->update_color(SnakeColor::Normal, SnakeColor::NormalScale);
    this->can_use_attack = false;
}

void Snake::collect_rainbow() {
    if (this->in_rainbow_mode) {
        return;
    }

    this->update_color(SnakeColor::Rainbow, SnakeColor::RainbowScale);
    this->in_rainbow_mode = true;
    this->can_use_attack = true;
    this->can_use_portal = true;
    this->update_ms = STAR_SNAKE_UPDATE_MS;

    this->rainbow_cooldown_interval_id = this->game.start_cooldown(RAINBOW_DURATION_SECS, [this]() {
        this->on_rainbow_exit();
    });
}

void Snake::on_rainbow_exit() {
    this->update_color(SnakeColor::Normal, SnakeColor::NormalScale);
    this->in_rainbow_mode = false;
    this->can_use_attack = false;
    this->can_use_portal = false;
    this->update_ms = SNAKE_UPDATE_MS;
}

void Snake::init() {
    this->length = 1;
    this->head_idx = 0;
    this->tail_idx = 0;
    this->stars_collected = 0;

    this->x = this->spawn_x;
    this->y = this->spawn_y;

    this->segments[this->head_idx].x = this->spawn_x;
    this->segments[this->head_idx].y = this->spawn_y;
    this->segments[this->head_idx].dir = Direction::Right;

    this->can_use_attack = false;
    this->can_use_portal = false;
    this->in_rainbow_mode = false;

    this->body_color = SnakeColor::Normal;
    this->scale_color = SnakeColor::NormalScale;

    this->update_ms = SNAKE_UPDATE_MS;
}

void Snake::reset() {
    // Clear snake from grid
    this->foreach_segment([this](Segment *segment) {
        this->game.set_tile(segment->x, segment->y, Tile::Empty);
        this->game.render_tile(segment->x, segment->y, Tile::Empty);
    });

    this->init();

    this->game.update_score();
    this->game.clear_interval(this->rainbow_cooldown_interval_id);
    this->game.set_cooldown_secs(0);
}

bool Snake::try_grow() {
    if (this->length >= this->max_length - 2) {
        return false;
    }

    this->length++;

    // Add new segment
    const uint8_t new_tail_idx = (this->tail_idx - 1 + this->max_length) % this->max_length;

    this->segments[new_tail_idx].x = this->segments[this->tail_idx].x;
    this->segments[new_tail_idx].y = this->segments[this->tail_idx].y;

    this->tail_idx = new_tail_idx;

    this->game.update_score();
    return true;
}

void Snake::collect_food() {
    for (uint8_t i = 0; i < FOOD_GROW_AMOUNT; i++) {
        if (!this->try_grow()) {
            break;
        }
    }
}

void Snake::update() {
    const Direction dir = this->segments[this->head_idx].dir;

    if (this->length > 1) {
        const Direction prev_dir = this->segments[(this->head_idx - 1 + this->max_length) % this->max_length].dir;
        Tile new_segment = Tile::Empty;

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
        this->game.render_snake_tile(this->segments[this->head_idx].x, this->segments[this->head_idx].y, new_segment);
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
                    this->game.update_lives(-1);
                    this->reset();
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
                    this->game.update_lives(-1);
                    this->reset();
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
                    this->game.update_lives(-1);
                    this->reset();
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
                    this->game.update_lives(-1);
                    this->reset();
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
                this->game.despawn_entity(this->segments[this->head_idx].x, this->segments[this->head_idx].y);
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

            this->game.update_lives(-1);
            this->reset();

            this->game.set_tile(head_x, head_y, write_back_tile);
            this->game.render_tile(head_x, head_y, write_back_tile);

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
        case Tile::RainbowPack:
            this->collect_rainbow();
            break;
        case Tile::HeartPack:
            this->game.update_lives(1);
            break;
        case Tile::StarPack:
            this->stars_collected++;
            this->game.update_score();
            break;
        default:
            break;
    }

    // Render changes
    this->game.set_tile(this->segments[this->head_idx].x, this->segments[this->head_idx].y, Tile::SnakeHead);
    this->game.render_snake_tile(this->segments[this->head_idx].x, this->segments[this->head_idx].y, Tile::SnakeHead);

    // Clear previous tail position
    this->game.set_tile(this->segments[this->tail_idx].x, this->segments[this->tail_idx].y, Tile::Empty);
    this->game.render_tile(this->segments[this->tail_idx].x, this->segments[this->tail_idx].y, Tile::Empty);

    // Update tail index
    this->tail_idx = (this->tail_idx + 1) % this->max_length;

    this->x = this->segments[this->head_idx].x;
    this->y = this->segments[this->head_idx].y;
}
