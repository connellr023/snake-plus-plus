#include <chrono>
#include <cassert>
#include "game.hpp"
#include "../sprites/sprites.hpp"

void Game::set_tile(int x, int y, tile_t tile) {
    assert(x >= 0 && x < this->grid_width && y >= 0 && y < this->grid_height);
    this->grid[y * this->grid_width + x] = tile;
}

tile_t Game::get_tile(int x, int y) {
    assert(x >= 0 && x < this->grid_width && y >= 0 && y < this->grid_height);
    return this->grid[y * this->grid_width + x];
}

void Game::generate_and_draw_map() {
    for (int y = 0; y < this->grid_height; y++) {
        for (int x = 0; x < this->grid_width; x++) {
            set_tile(x, y, EMPTY);
            draw_tile(this->fb, x, y, EMPTY);
        }
    }
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

}
