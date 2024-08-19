#ifndef UTILS_H
#define UTILS_H
#include <chrono>
#include <cstdint>

static uint64_t current_millis() {
    using namespace std::chrono;

    const auto now = high_resolution_clock::now();
    const auto duration = duration_cast<milliseconds>(now.time_since_epoch()).count();

    return static_cast<uint64_t>(duration);
}

#endif // UTILS_H
