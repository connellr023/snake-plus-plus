#include "ghost.hpp"

void Ghost::update() {
    if (!this->path.empty()) {
        PathNode current_node = this->pop_next_node();

        switch (this->game.get_tile(current_node.pos.x, current_node.pos.y)) {
            case Tile::SnakeHead: {
                if (this->game.get_snake().can_attack()) {
                    this->die();
                    return;
                }

                break;
            }
            case Tile::SnakeSegmentTopLeft:
            case Tile::SnakeSegmentTopRight:
            case Tile::SnakeSegmentBottomLeft:
            case Tile::SnakeSegmentBottomRight:
            case Tile::SnakeSegmentHorizontal:
            case Tile::SnakeSegmentVertical: {
                this->recalculate_path();
                return;
            }
            case Tile::Ghost: {
                if (current_node.pos != Vector2 { this->x, this->y }) {
                    this->path.clear();
                    return;
                }
            }
            default:
                break;
        }

        this->game.set_tile(this->x, this->y, Tile::Empty);
        this->x = current_node.pos.x;
        this->y = current_node.pos.y;
        this->game.set_tile(this->x, this->y, Tile::Ghost);
    }
    else {
        this->recalculate_path();
    }
}
