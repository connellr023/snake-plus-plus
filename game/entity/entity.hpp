#ifndef ENTITY_H
#define ENTITY_H
#include <cstdint>

class Game;

class Entity {
protected:
    Game &game;

    bool is_alive = true;

    uint8_t x;
    uint8_t y;

private:
    uint64_t last_update_ms;
    uint64_t update_ms;

public:
    Entity(Game &game, uint8_t start_x, uint8_t start_y, uint64_t update_ms) : game(game), x(start_x), y(start_y), last_update_ms(0), update_ms(update_ms) {};

    virtual void update() = 0;

    uint64_t get_update_ms() const {
        return this->update_ms;
    }

    uint64_t get_last_update_ms() const {
        return this->last_update_ms;
    }

    void set_last_update_ms(uint64_t last_update_ms) {
        this->last_update_ms = last_update_ms;
    }

    bool get_is_alive() const {
        return this->is_alive;
    }
};

#endif // ENTIY_H
