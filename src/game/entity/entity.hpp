#ifndef ENTITY_H
#define ENTITY_H
#include <cstdint>

class Game;

class Entity {
protected:
    Game &game;

    uint8_t x;
    uint8_t y;

    uint64_t update_ms;

private:
    uint64_t last_update_ms;

public:
    Entity(Game &game, uint8_t start_x, uint8_t start_y, uint64_t update_ms) :
        game(game),
        x(start_x),
        y(start_y),
        update_ms(update_ms),
        last_update_ms(0)
    {};

    virtual ~Entity() = default;

    virtual void update() = 0;

    uint8_t get_x() const {
        return this->x;
    }

    uint8_t get_y() const {
        return this->y;
    }

    uint64_t get_update_ms() const {
        return this->update_ms;
    }

    uint64_t get_last_update_ms() const {
        return this->last_update_ms;
    }

    void set_last_update_ms(uint64_t last_update_ms) {
        this->last_update_ms = last_update_ms;
    }
};

#endif // ENTIY_H
