#ifndef GAME_H
#define GAME_H
#include <cstdint>
#include <random>
#include <memory>
#include <functional>
#include <vector>
#include "snake/snake.hpp"
#include "entity/entity.hpp"
#include "../framebuffer/impl.hpp"
#include "../time_manager/time_manager.hpp"

#define FOOD_SPAWN_COUNT        3
#define PORTAL_SPAWN_COUNT      2
#define ATTACK_SPAWN_COUNT      1
#define RAINBOW_SPAWN_COUNT     1
#define HEART_SPAWN_COUNT       1
#define STAR_SPAWN_COUNT        1

#define MIN_FOOD_LIFETIME       30
#define MAX_FOOD_LIFETIME       45

#define MIN_PORTAL_LIFETIME     30
#define MAX_PORTAL_LIFETIME     45

#define MIN_ATTACK_LIFETIME     22
#define MAX_ATTACK_LIFETIME     27

#define MIN_RAINBOW_LIFETIME    23
#define MAX_RAINBOW_LIFETIME    26

#define MIN_HEART_LIFETIME      20
#define MAX_HEART_LIFETIME      25

#define MIN_STAR_LIFETIME       21
#define MAX_STAR_LIFETIME       24

#define MIN_ROCKS               4
#define MAX_ROCKS               6

#define FOOD_SPAWN_MS           4000
#define PORTAL_SPAWN_MS         5000
#define ATTACK_SPAWN_MS         2000
#define RAINBOW_SPAWN_MS        6000
#define HEART_SPAWN_MS          7000
#define STAR_SPAWN_MS           6300
#define GHOST_SPAWN_MS          3000
#define LIFE_TILE_MS            120

#define MAX_SNAKE_SIZE          130
#define MAX_ENTITY_COUNT        6

enum class Tile {
    Food,
    PortalPack,
    AttackPack,
    RainbowPack,
    HeartPack,
    StarPack,
    Ghost,
    Rock,
    Empty,
    SnakeHead,
    SnakeSegmentHorizontal,
    SnakeSegmentVertical,
    SnakeSegmentBottomLeft,
    SnakeSegmentBottomRight,
    SnakeSegmentTopLeft,
    SnakeSegmentTopRight
};

struct LifetimeTile {
    Tile tile;
    uint8_t x;
    uint8_t y;
    uint64_t life_left;
};

struct Vector2 {
    uint8_t x;
    uint8_t y;

    bool operator==(const Vector2 &other) const {
        return this->x == other.x && this->y == other.y;
    }

    bool operator!=(const Vector2 &other) const {
        return !(*this == other);
    }
};

class Game : public TimeManager {
private:
    FrameBufferImpl &fb;

    uint8_t grid_width;
    uint8_t grid_height;

    std::mt19937 rng;

    std::unique_ptr<Tile[]> grid;
    std::shared_ptr<Snake> snake;

    std::vector<LifetimeTile> lifetime_tiles {};
    std::vector<std::shared_ptr<Entity>> entities {};

    uint16_t score = 0;
    uint16_t high_score = 0;
    uint8_t lives = 10;

    bool is_over_state = false;
    bool is_paused_state = false;
    uint64_t pause_start_ms = 0;

    uint8_t cooldown_secs = 0;

    void lazily_spawn_entity(std::function<Entity *()> entity_spawner);

    void pause();
    void resume();

    void generate_map();
    void generate_lifetime_tile(Tile tile, uint8_t amount, uint64_t min_lifetime, uint64_t max_lifetime);
    Vector2 generate_random_pos();
    Vector2 generate_balanced_random_pos(std::vector<Vector2> &avoid, uint8_t min_distance);

    void update_entities();

public:
    Game(FrameBufferImpl &fb, int grid_width, int grid_height);

    uint32_t start_cooldown(uint8_t secs, std::function<void()> on_finish);
    void set_cooldown_secs(uint8_t secs);

    void set_lives(uint8_t lives);
    void update_lives(int8_t amount);
    void update_score();

    void render_tile(uint8_t x, uint8_t y, Tile tile);
    void render_snake_tile(uint8_t x, uint8_t y, Tile tile);
    void set_tile(uint8_t x, uint8_t y, Tile tile);
    Tile get_tile(uint8_t x, uint8_t y);

    int get_grid_width() const {
        return grid_width;
    }

    int get_grid_height() const {
        return grid_height;
    }

    void update();

    bool is_within_grid(uint8_t x, uint8_t y) const {
        return x >= 0 && x < grid_width && y >= 0 && y < grid_height;
    }

    bool is_over() const {
        return is_over_state;
    }

    void despawn_entity(uint8_t x, uint8_t y);

    ~Game();
};

#endif // GAME_H
