#include <cassert>
#include <functional>
#include <memory>
#include <cmath>
#include "game.hpp"
#include "ghost/ghost.hpp"
#include "../rendering/rendering.hpp"
#include "../rendering/sprites.hpp"
#include "../rendering/colors.hpp"
#include "../keycodes.hpp"

Game::Game(FrameBufferImpl &fb, int grid_width, int grid_height) :
    fb(fb),
    grid_width(grid_width),
    grid_height(grid_height),
    rng(std::random_device{}())
{
    this->snake_spawn_pos = { static_cast<uint8_t>(this->grid_width / 2), static_cast<uint8_t>(this->grid_height / 2) };
    this->score = 0;
    this->high_score = 0;

    // Initialize snake
    this->snake = std::shared_ptr<Snake>(new Snake(*this, this->snake_spawn_pos.x, this->snake_spawn_pos.y, MAX_SNAKE_SIZE));
    this->entities.insert(snake);

    // Initialize map
    this->grid = std::make_unique<Tile[]>(grid_width * grid_height);
    this->generate_map();

    // Initialize UI
    draw_ui_sprite(this->fb, HEART_ICON_X, HEART_ICON_COLOR, SPRITE_HEART);
    this->set_lives(MAX_LIVES);

    draw_ui_sprite(this->fb, STAR_ICON_X, STAR_ICON_COLOR, SPRITE_STAR);
    draw_ui_sprite(this->fb, HIGHSCORE_ICON_X, HIGHSCORE_ICON_COLOR, SPRITE_TROPHY);
    this->calc_score();

    this->resume();

    this->fb.register_keypress_listener(KEY_ESC, [this]() {
        if (this->is_paused) this->resume();
        else this->pause();
    });

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

    this->register_interval_listener(FOOD_SPAWN_MS, [this]() {
        this->generate_lifetime_tile(Tile::Food, FOOD_SPAWN_COUNT, MIN_FOOD_LIFETIME, MAX_FOOD_LIFETIME);
    });

    this->register_interval_listener(PORTAL_SPAWN_MS, [this]() {
        this->generate_lifetime_tile(Tile::PortalPack, PORTAL_SPAWN_COUNT, MIN_PORTAL_LIFETIME, MAX_PORTAL_LIFETIME);
    });

    this->register_interval_listener(ATTACK_SPAWN_MS, [this]() {
        this->generate_lifetime_tile(Tile::AttackPack, ATTACK_SPAWN_COUNT, MIN_ATTACK_LIFETIME, MAX_ATTACK_LIFETIME);
    });

    this->register_interval_listener(RAINBOW_SPAWN_MS, [this]() {
        this->generate_lifetime_tile(Tile::RainbowPack, RAINBOW_SPAWN_COUNT, MIN_RAINBOW_LIFETIME, MAX_RAINBOW_LIFETIME);
    });

    this->register_interval_listener(HEART_SPAWN_MS, [this]() {
        this->generate_lifetime_tile(Tile::HeartPack, HEART_SPAWN_COUNT, MIN_HEART_LIFETIME, MAX_HEART_LIFETIME);
    });

    this->register_interval_listener(STAR_SPAWN_MS, [this]() {
        this->generate_lifetime_tile(Tile::StarPack, STAR_SPAWN_COUNT, MIN_STAR_LIFETIME, MAX_STAR_LIFETIME);
    });

    this->register_interval_listener(GHOST_SPAWN_MS, [this]() {
        this->lazily_spawn_entity([this]() {
            std::vector<Vector2> avoid_positions = { this->snake_spawn_pos };
            constexpr int min_spacing = 5;
            const auto [x, y] = this->generate_balanced_random_pos(avoid_positions, min_spacing);

            return new Ghost(*this, x, y);
        });
    });

    this->register_interval_listener(LIFE_TILE_MS, [this]() {
        for (auto it = this->lifetime_tiles.begin(); it != this->lifetime_tiles.end();) {
            auto lifetime_tile = *it;
            lifetime_tile->life_left--;

            if (this->get_tile(lifetime_tile->x, lifetime_tile->y) != lifetime_tile->tile) {
                it = this->lifetime_tiles.erase(it);
                continue;
            }
            else if (lifetime_tile->life_left <= 0) {
                this->set_tile(lifetime_tile->x, lifetime_tile->y, Tile::Empty);
                this->render_tile(lifetime_tile->x, lifetime_tile->y, Tile::Empty);

                it = this->lifetime_tiles.erase(it);
                continue;
            }

            if (lifetime_tile->life_left <= 10) {
                if (lifetime_tile->life_left % 2 == 0) {
                    this->render_tile(lifetime_tile->x, lifetime_tile->y, lifetime_tile->tile);
                }
                else {
                    this->render_tile(lifetime_tile->x, lifetime_tile->y, Tile::Empty);
                }
            }

            it++;
        }
    });
}

void Game::set_lives(uint8_t lives) {
    this->lives = lives;
    draw_ui_uint<2>(this->fb, LIVES_TEXT_X, UI_TEXT_COLOR, this->lives);
}

void Game::calc_score() {
    constexpr float length_multiplier = 1.1f;
    constexpr float star_multiplier = 2.3f;

    this->score = static_cast<uint16_t>((this->snake->get_length() * length_multiplier) + (this->snake->get_star_count() * star_multiplier));
    draw_ui_uint<3>(this->fb, SCORE_TEXT_X, UI_TEXT_COLOR, this->score);

    if (this->score > this->high_score) {
        this->high_score = this->score;
        draw_ui_uint<3>(this->fb, HIGHSCORE_TEXT_X, UI_TEXT_COLOR, this->high_score);
    }
}

void Game::decrease_lives() {
    this->set_lives(this->lives - 1);
    this->snake->reset(this->snake_spawn_pos.x, this->snake_spawn_pos.y);
}

void Game::increment_lives() {
    this->set_lives(this->lives + 1);
}

void Game::render_tile(uint8_t x, uint8_t y, Tile tile) {
    assert(this->is_within_grid(x, y));
    draw_tile(this->fb, x, y, tile);
}

void Game::render_snake_tile(uint8_t x, uint8_t y, Tile tile) {
    assert(this->is_within_grid(x, y));
    draw_snake_tile(this->fb, *this->snake, x, y, tile);
}

void Game::set_tile(uint8_t x, uint8_t y, Tile tile) {
    assert(this->is_within_grid(x, y));
    this->grid[y * this->grid_width + x] = tile;
}

Tile Game::get_tile(uint8_t x, uint8_t y) {
    assert(this->is_within_grid(x, y));
    return this->grid[y * this->grid_width + x];
}

void Game::generate_map() {
    for (int y = 0; y < this->grid_height; y++) {
        for (int x = 0; x < this->grid_width; x++) {
            this->set_tile(x, y, Tile::Empty);
            this->render_tile(x, y, Tile::Empty);
        }
    }

    constexpr int min_rock_spacing = 4;
    const uint8_t rock_count = MIN_ROCKS + (rand() % (MAX_ROCKS - MIN_ROCKS));
    std::vector<Vector2> avoid_positions = std::vector<Vector2>(rock_count + 1);

    avoid_positions.push_back(this->snake_spawn_pos);

    for (uint8_t i = 0; i < rock_count; i++) {
        const auto [x, y] = this->generate_balanced_random_pos(avoid_positions, min_rock_spacing);

        this->set_tile(x, y, Tile::Rock);
        this->render_tile(x, y, Tile::Rock);

        avoid_positions.push_back({ x, y });
    }
}

void Game::kill_entity_at_pos(uint8_t x, uint8_t y) {
    for (auto it = this->entities.begin(); it != this->entities.end(); it++) {
        auto entity = *it;

        if (entity->get_x() == x && entity->get_y() == y) {
            it = this->entities.erase(it);
            return;
        }
    }
}

Vector2 Game::generate_random_pos() {
    std::uniform_int_distribution<uint8_t> dist_x(0, this->grid_width - 1);
    std::uniform_int_distribution<uint8_t> dist_y(0, this->grid_height - 1);

    uint8_t x = dist_x(this->rng);
    uint8_t y = dist_y(this->rng);

    while (this->get_tile(x, y) != Tile::Empty) {
        x = dist_x(this->rng);
        y = dist_y(this->rng);
    }

    return { x, y };
}

Vector2 Game::generate_balanced_random_pos(std::vector<Vector2> &avoid, uint8_t min_distance) {
    constexpr int max_attempts = 100;

    bool is_balanced = false;
    int attempts = 0;
    Vector2 pos = { 0, 0 };

    while (!is_balanced && attempts++ < max_attempts) {
        const auto [x, y] = this->generate_random_pos();
        is_balanced = true;

        for (const auto &pos : avoid) {
            if (std::abs(x - pos.x) <= min_distance && std::abs(y - pos.y) <= min_distance) {
                is_balanced = false;
                break;
            }
        }

        if (is_balanced || attempts >= max_attempts) {
            pos = { x, y };
            break;
        }
    }

    return pos;
}

void Game::generate_lifetime_tile(Tile tile, uint8_t amount, uint64_t min_lifetime, uint64_t max_lifetime) {
    for (uint8_t i = 0; i < amount; i++) {
        std::uniform_int_distribution<uint8_t> lifetime_dist(min_lifetime, max_lifetime);

        const auto [x, y] = this->generate_random_pos();
        const uint64_t lifetime = lifetime_dist(this->rng);

        this->set_tile(x, y, tile);
        this->render_tile(x, y, tile);

        this->lifetime_tiles.insert(std::shared_ptr<LifetimeTileWrapper>(new LifetimeTileWrapper {
            .tile = tile,
            .x = x,
            .y = y,
            .life_left = lifetime
        }));
    }
}

void Game::pause() {
    this->is_paused = true;
    this->pause_start_ms = Game::current_millis();
    
    draw_ui_sprite(this->fb, PAUSE_RESUME_ICON_X, PAUSED_ICON_COLOR, SPRITE_PAUSED_ICON);
}

void Game::resume() {
    const uint64_t now = Game::current_millis();

    // Account for the time paused
    for (auto wrapper : this->interval_listeners) {
        wrapper->last_interval_ms += now - this->pause_start_ms;
    }

    for (auto entity : this->entities) {
        entity->set_last_update_ms(entity->get_last_update_ms() + now - this->pause_start_ms);
    }

    this->is_paused = false;

    draw_ui_sprite(this->fb, PAUSE_RESUME_ICON_X, RESUMED_ICON_COLOR, SPRITE_RESUMED_ICON);
}

void Game::update() {
    if (this->is_paused) {
        return;
    }

    const uint64_t now = Game::current_millis();

    for (auto interval_wrapper : this->interval_listeners) {
        if (interval_wrapper->last_interval_ms + interval_wrapper->interval_ms < now) {
            interval_wrapper->listener();
            interval_wrapper->last_interval_ms = now;
        }
    }

    for (auto it = this->entities.begin(); it != this->entities.end();) {
        auto entity = *it;

        if (entity->get_last_update_ms() + entity->get_update_ms() < now) {
            entity->update();
            entity->set_last_update_ms(now);
        }

        if (!entity->get_is_alive()) {
            it = this->entities.erase(it);
        }
        else {
            it++;
        }
    }
}
