#ifndef GHOST_H
#define GHOST_H
#include "../game.hpp"
#include "../entity/entity.hpp"

#define GHOST_UPDATE_MS     230

class Ghost : public Entity {
private:
    Direction dir;

    void randomize_dir() {
        this->dir = static_cast<Direction>(rand() % 4);
    }

public:
    Ghost(Game &game, uint8_t start_x, uint8_t start_y) : Entity(game, start_x, start_y, GHOST_UPDATE_MS) {
        this->game.set_tile(this->x, this->y, Tile::Ghost);
        this->game.render_tile(this->x, this->y, Tile::Ghost);

        this->randomize_dir();
    };

    void update() override;
};

#endif // GHOST_H
