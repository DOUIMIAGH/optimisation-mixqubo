#ifndef TIME_UTILS_HPP
#define TIME_UTILS_HPP

#include <chrono>

static bool time_left(
    const std::chrono::high_resolution_clock::time_point& start,
    int max_time_ms
) {
    auto now = std::chrono::high_resolution_clock::now();
    double elapsed =
        std::chrono::duration<double, std::milli>(now - start).count();
    return elapsed < max_time_ms;
}

static double elapsed_ms(
    const std::chrono::high_resolution_clock::time_point& start
) {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(now - start).count();
}

#endif