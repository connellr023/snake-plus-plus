#include <chrono>
#include <cassert>
#include <memory>
#include <tuple>
#include "game.hpp"
#include "../rendering/rendering.hpp"
#include "../rendering/sprites.hpp"
#include "../rendering/colors.hpp"
#include "game_notifier.hpp"

void Game::set_lives(uint8_t lives) {
    this->lives = lives;
    RENDER_UI_UINT(this->fb, LIVES_TEXT_X, UI_TEXT_COLOR, 2, this->lives);
}

void Game::set_tile(int x, int y, tile_t tile) {
    assert(x >= 0 && x < this->grid_width && y >= 0 && y < this->grid_height);
    draw_tile(this->fb, x, y, tile);
    this->grid[y * this->grid_width + x] = tile;
}

tile_t Game::get_tile(int x, int y) {
    assert(x >= 0 && x < this->grid_width && y >= 0 && y < this->grid_height);
    return this->grid[y * this->grid_width + x];
}

void Game::register_listener(notification_t notification, notification_listener_t listener) {
    this->listeners[notification] = listener;
}

void Game::init() {
    this->register_listener(SET_TILE, [this](void *data) {
        const auto set_tile_data = reinterpret_cast<std::tuple<int, int, tile_t> *>(data);
        const int x = std::get<0>(*set_tile_data);
        const int y = std::get<1>(*set_tile_data);
        const tile_t tile = std::get<2>(*set_tile_data);

        this->set_tile(x, y, tile);
    });

    this->register_listener(DRAW_TILE, [this](void *data) {
        const auto set_tile_data = reinterpret_cast<std::tuple<int, int, tile_t> *>(data);
        const int x = std::get<0>(*set_tile_data);
        const int y = std::get<1>(*set_tile_data);
        const tile_t tile = std::get<2>(*set_tile_data);

        draw_tile(this->fb, x, y, tile);
    });

    this->register_listener(DECREMENT_LIFE, [this](void *_) {
        this->set_lives(this->lives - 1);
    });

    for (int y = 0; y < this->grid_height; y++) {
        for (int x = 0; x < this->grid_width; x++) {
            set_tile(x, y, EMPTY);
        }
    }

    RENDER_UI_SPRITE(this->fb, HEART_ICON_X, HEART_ICON_COLOR, SPRITE_HEART);
    set_lives(MAX_LIVES);
}

void Game::loop() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    uint64_t millis = static_cast<uint64_t>(duration);

    if (this->last_tick + (this->ticks_per_second * 1000) < millis) {
        this->tick();
        this->last_tick = millis;
    }
}

void Game::tick() {
    this->snake->loop();
}
