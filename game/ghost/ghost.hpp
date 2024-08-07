#ifndef GHOST_H
#define GHOST_H
#include "../game.hpp"
#include "../entity/entity.hpp"

#define GHOST_UPDATE_MS 85

class Ghost : public Entity {
private:
    std::vector<PathNode> path;

    void recalculate_path() {
        Segment snake_head = this->game.get_snake().get_head_segment();

        this->path = this->game.a_star({ this->x, this->y }, { snake_head.x, snake_head.y }, [this](Tile tile) {
            return tile == Tile::SnakeHead || tile == Tile::Empty;
        });
    }

    PathNode pop_next_node() {
        PathNode next_node = this->path.back();
        this->path.pop_back();

        return next_node;
    }

public:
    Ghost(Game &game, uint8_t start_x, uint8_t start_y) : Entity(game, start_x, start_y, GHOST_UPDATE_MS) {
        this->game.set_tile(this->x, this->y, Tile::Ghost);
        this->recalculate_path();
    };

    void update() override;
};

#endif // GHOST_H
