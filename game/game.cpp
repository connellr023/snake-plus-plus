#include <chrono>
#include <cassert>
#include <memory>
#include "game.hpp"
#include "../rendering/rendering.hpp"
#include "../rendering/sprites.hpp"
#include "../rendering/colors.hpp"
#include "../keycodes.hpp"

void Game::set_lives(uint8_t lives) {
    this->lives = lives;
    draw_ui_uint(this->fb, LIVES_TEXT_X, UI_TEXT_COLOR, 2, this->lives);
}

void Game::decrease_lives() {
    this->set_lives(this->lives - 1);
    this->snake->reset(SNAKE_SPAWN_X, SNAKE_SPAWN_Y);
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

void Game::generate_map() {
    for (int y = 0; y < this->grid_height; y++) {
        for (int x = 0; x < this->grid_width; x++) {
            this->set_tile(x, y, Tile::Empty);
        }
    }
}

void Game::generate_food() {
    uint8_t x = rand() % this->grid_width;
    uint8_t y = rand() % this->grid_height;

    while (this->get_tile(x, y) != Tile::Empty) {
        x = rand() % this->grid_width;
        y = rand() % this->grid_height;
    }

    this->set_tile(x, y, Tile::Food);
    this->lifetime_tiles.insert(std::shared_ptr<LifetimeTileWrapper>(new LifetimeTileWrapper {
        .tile = Tile::Food,
        .tile_x = x,
        .tile_y = y,
        .life_left = FOOD_LIFETIME
    }));
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

    this->register_interval_listener(SNAKE_MOVE_MS, [this]() {
        this->snake->loop();
    });

    this->register_interval_listener(FOOD_SPAWN_MS, [this]() {
        this->generate_food();
    });

    this->register_interval_listener(LIFE_TILE_MS, [this]() {
        for (auto it = this->lifetime_tiles.begin(); it != this->lifetime_tiles.end();) {
            auto lifetime_tile = *it;
            lifetime_tile->life_left--;

            if (this->get_tile(lifetime_tile->tile_x, lifetime_tile->tile_y) != lifetime_tile->tile) {
                it = this->lifetime_tiles.erase(it);
                continue;
            }
            else if (lifetime_tile->life_left <= 0) {
                this->set_tile(lifetime_tile->tile_x, lifetime_tile->tile_y, Tile::Empty);
                it = this->lifetime_tiles.erase(it);
                continue;
            }

            if (lifetime_tile->life_left <= 10) {
                if (lifetime_tile->life_left % 2 == 0) {
                    draw_tile(this->fb, lifetime_tile->tile_x, lifetime_tile->tile_y, lifetime_tile->tile);
                }
                else {
                    draw_tile(this->fb, lifetime_tile->tile_x, lifetime_tile->tile_y, Tile::Empty);
                }
            }

            it++;
        }
    });

    // Initialize UI
    draw_ui_sprite(this->fb, HEART_ICON_X, HEART_ICON_COLOR, SPRITE_HEART);
    set_lives(MAX_LIVES);

    // Initialize map
    this->generate_map();
}

void Game::loop() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    uint64_t millis = static_cast<uint64_t>(duration);

    for (auto &[interval_ms, interval_wrapper] : this->interval_listeners) {
        if (interval_wrapper.last_interval_ms + interval_ms < millis) {
            interval_wrapper.listener();
            interval_wrapper.last_interval_ms = millis;
        }
    }
}
