#ifndef GAME_NOTIFIER_H
#define GAME_NOTIFIER_H
#include <functional>

typedef enum {
    SET_TILE,
    DRAW_TILE,
    DECREMENT_LIFE,
} notification_t;

typedef std::function<void(void *)> notification_listener_t;
typedef std::function<void(notification_t, void *)> notifier_callback_t;

#define SET_TILE_FN(notifier, x, y, tile) { \
    const auto tmp = std::tuple(x, y, tile); \
    notifier(SET_TILE, (void *) &tmp); \
}

#define DRAW_TILE_FN(notifier, x, y, tile) { \
    const auto tmp = std::tuple(x, y, tile); \
    notifier(DRAW_TILE, (void *) &tmp); \
}

#define DECREMENT_LIFE_FN(notifier) { \
    notifier(DECREMENT_LIFE, nullptr); \
}

#endif // GAME_NOTIFIER_H
