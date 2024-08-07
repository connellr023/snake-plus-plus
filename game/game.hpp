#ifndef GAME_H
#define GAME_H
#include <cstdint>
#include <random>
#include <list>
#include <set>
#include <memory>
#include <functional>
#include "snake/snake.hpp"
#include "entity/entity.hpp"
#include "../framebuffer/impl.hpp"

#define SNAKE_SPAWN_X           3
#define SNAKE_SPAWN_Y           3

#define FOOD_SPAWN_COUNT        3
#define PORTAL_SPAWN_COUNT      2
#define ATTACK_SPAWN_COUNT      1

#define MIN_FOOD_LIFETIME       30
#define MAX_FOOD_LIFETIME       45

#define MIN_PORTAL_LIFETIME     30
#define MAX_PORTAL_LIFETIME     45

#define MIN_ATTACK_LIFETIME     22
#define MAX_ATTACK_LIFETIME     27

#define MIN_ROCKS               4
#define MAX_ROCKS               6

#define SNAKE_MOVE_MS           85
#define FOOD_SPAWN_MS           4000
#define PORTAL_SPAWN_MS         5000
#define ATTACK_SPAWN_MS         2000
#define GHOST_SPAWN_MS          3000
#define LIFE_TILE_MS            120

#define MAX_SNAKE_SIZE          100
#define MAX_LIVES               10

#define MAX_ENTITY_COUNT        5

typedef std::function<void()> interval_listener_t;

enum class Tile {
    Food,
    PortalPack,
    AttackPack,
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

struct IntervalListenerWrapper {
    uint64_t last_interval_ms;
    uint64_t interval_ms;
    interval_listener_t listener;
};

struct LifetimeTileWrapper {
    Tile tile;
    uint8_t x;
    uint8_t y;
    uint64_t life_left;
};

struct Vector2 {
    uint8_t x;
    uint8_t y;

    int manhattan_distance(Vector2 other) const {
        return abs(this->x - other.x) + abs(this->y - other.y);
    }

    bool operator==(const Vector2 &other) const {
        return this->x == other.x && this->y == other.y;
    }

    bool operator!=(const Vector2 &other) const {
        return !(*this == other);
    }
};

struct PathNode {
    std::shared_ptr<PathNode> parent;

    Vector2 pos;

    float g_cost;
    float h_cost;

    float f_cost() const {
        return this->g_cost + this->h_cost;
    }

    bool operator>(const PathNode &other) const {
        return this->f_cost() > other.f_cost();
    }
};

class Game {
private:
    FrameBufferImpl &fb;

    int grid_width;
    int grid_height;
    int tick_ms;

    std::mt19937 rng;

    std::unique_ptr<Tile[]> grid;
    std::unique_ptr<Snake> snake;

    std::set<std::shared_ptr<LifetimeTileWrapper>> lifetime_tiles;
    std::set<std::shared_ptr<Entity>> entities;

    std::list<std::shared_ptr<IntervalListenerWrapper>> interval_listeners;

    uint16_t score = 0;
    uint8_t lives = 0;

    void register_interval_listener(uint64_t interval_ms, interval_listener_t listener) {
        this->interval_listeners.push_front(std::shared_ptr<IntervalListenerWrapper>(new IntervalListenerWrapper {
            .last_interval_ms = 0,
            .interval_ms = interval_ms,
            .listener = listener
        }));
    }

    void lazily_spawn_entity(std::function<Entity *()> entity_spawner) {
        if (entities.size() >= MAX_ENTITY_COUNT) {
            return;
        }

        this->entities.insert(std::shared_ptr<Entity>(entity_spawner()));
    }

    void generate_map();
    void generate_lifetime_tile(Tile tile, uint8_t amount, uint64_t min_lifetime, uint64_t max_lifetime);

public:
    Game(FrameBufferImpl &fb, int grid_width, int grid_height) :
        fb(fb),
        grid_width(grid_width),
        grid_height(grid_height),
        rng(std::random_device{}())
    {
        this->grid = std::make_unique<Tile[]>(grid_width * grid_height);
        this->snake = std::unique_ptr<Snake>(new Snake(*this, SNAKE_SPAWN_X, SNAKE_SPAWN_Y, MAX_SNAKE_SIZE));
    }

    Snake &get_snake() {
        return *this->snake;
    }

    void set_lives(uint8_t lives);
    void set_score(uint16_t score);
    void set_tile(int x, int y, Tile tile);
    Tile get_tile(int x, int y);

    int get_grid_width() const {
        return grid_width;
    }

    int get_grid_height() const {
        return grid_height;
    }

    void decrease_lives();

    void init();
    void loop();

    bool is_within_grid(uint8_t x, uint8_t y) const {
        return x >= 0 && x < grid_width && y >= 0 && y < grid_height;
    }

    Vector2 generate_random_pos();

    std::vector<PathNode> a_star(Vector2 start, Vector2 end, std::function<bool(Tile)> is_valid_tile);
};

#endif // GAME_H
