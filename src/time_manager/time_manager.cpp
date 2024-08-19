#include "time_manager.hpp"

void TimeManager::update_interval_listeners() {
    const uint64_t now = TimeManager::current_millis();

    for (auto it = this->interval_listeners.begin(); it != this->interval_listeners.end();) {
        auto &listener = *it;

        if (listener.last_interval_ms + listener.interval_ms < now) {
            listener.callback(listener.is_active);
            listener.last_interval_ms = now;
        }

        if (!listener.is_active) {
            it = this->interval_listeners.erase(it);
            continue;
        }

        it++;
    }
}

uint32_t TimeManager::start_interval(uint64_t interval_ms, interval_listener_callback_t listener) {
    const uint32_t id = ++interval_listener_id_counter;

    this->interval_listeners.push_back({
        .id = id,
        .is_active = true,
        .last_interval_ms = 0,
        .interval_ms = interval_ms,
        .callback = listener
    });

    return id;
}

void TimeManager::clear_interval(uint32_t id) {
    // Binary search interval ID and mark as inactive
    uint8_t high = this->interval_listeners.size() - 1;
    uint8_t low = 0;

    while (low <= high) {
        uint8_t mid = low + (high - low) / 2;
        auto &interval_listener = this->interval_listeners[mid];

        if (interval_listener.id == id) {
            interval_listener.is_active = false;
            return;
        }
        else if (interval_listener.id < id) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
}
