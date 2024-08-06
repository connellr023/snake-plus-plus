#ifndef GHOST_H
#define GHOST_H
#include "../game.hpp"
#include "../entity/entity.hpp"

#define GHOST_UPDATE_MS 300

class Ghost : public Entity {
public:
    Ghost(Game &game, uint8_t start_x, uint8_t start_y) : Entity(game, start_x, start_y, GHOST_UPDATE_MS) {};

    void move(Direction dir);
    void update() override;
};

#endif // GHOST_H
