#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H
#include <vector>
#include <functional>
#include <cstdint>
#include <chrono>

typedef std::function<void(bool &)> interval_listener_callback_t;

struct IntervalListener {
    uint32_t id;
    bool is_active;
    uint64_t last_interval_ms;
    uint64_t interval_ms;
    interval_listener_callback_t callback;
};

class TimeManager {
private:
    uint32_t interval_listener_id_counter = 0;

protected:
    std::vector<IntervalListener> interval_listeners {};

    static uint64_t current_millis() {
        using namespace std::chrono;

        const auto now = high_resolution_clock::now();
        const auto duration = duration_cast<milliseconds>(now.time_since_epoch()).count();

        return static_cast<uint64_t>(duration);
    }

public:
    uint32_t start_interval(uint64_t interval_ms, interval_listener_callback_t listener);
    void clear_interval(uint32_t id);

    void update_interval_listeners();
};

#endif // TIME_MANAGER_H
