#include <chrono>
#include <cassert>
#include <memory>
#include "game.hpp"
#include "../rendering/rendering.hpp"
#include "../rendering/sprites.hpp"
#include "../rendering/colors.hpp"
#include "../keycodes.hpp"

void Game::register_interval_listener(int interval_ms, interval_listener_t listener) {
    if (this->interval_listeners.find(interval_ms) == this->interval_listeners.end()) {
        this->interval_listeners[interval_ms] = listener;
    }
}

void Game::set_lives(uint8_t lives) {
    this->lives = lives;
    RENDER_UI_UINT(this->fb, LIVES_TEXT_X, UI_TEXT_COLOR, 2, this->lives);
}

void Game::decrease_lives() {
    this->set_lives(this->lives - 1);
}

void Game::set_tile(int x, int y, Tile tile) {
    assert(x >= 0 && x < this->grid_width && y >= 0 && y < this->grid_height);
    draw_tile(this->fb, x, y, tile);
    this->grid[y * this->grid_width + x] = tile;
}

Tile Game::get_tile(int x, int y) {
    assert(x >= 0 && x < this->grid_width && y >= 0 && y < this->grid_height);
    return this->grid[y * this->grid_width + x];
}

void Game::init() {
    this->fb.register_keypress_listener(KEY_UP, [this]() {
        this->snake->set_direction(Direction::Up);
    });

    this->fb.register_keypress_listener(KEY_DOWN, [this]() {
        this->snake->set_direction(Direction::Down);
    });

    this->fb.register_keypress_listener(KEY_LEFT, [this]() {
        this->snake->set_direction(Direction::Left);
    });

    this->fb.register_keypress_listener(KEY_RIGHT, [this]() {
        this->snake->set_direction(Direction::Right);
    });

    for (int y = 0; y < this->grid_height; y++) {
        for (int x = 0; x < this->grid_width; x++) {
            set_tile(x, y, Tile::Empty);
        }
    }

    RENDER_UI_SPRITE(this->fb, HEART_ICON_X, HEART_ICON_COLOR, SPRITE_HEART);
    set_lives(MAX_LIVES);

    // Test food
    set_tile(5, 5, Tile::Food);
    set_tile(20, 17, Tile::Food);
}

void Game::loop() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    uint64_t millis = static_cast<uint64_t>(duration);

    if (this->last_tick + this->tick_ms < millis) {
        this->tick();
        this->last_tick = millis;
    }
}

void Game::tick() {
    this->snake->loop();
}
