#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <string>

#include "solution.hpp"
#include "mixQUBO.hpp"

static bool time_left(
    const std::chrono::high_resolution_clock::time_point& start,
    int max_time_ms
) {
    auto now = std::chrono::high_resolution_clock::now();
    double elapsed =
        std::chrono::duration<double, std::milli>(now - start).count();
    return elapsed < max_time_ms;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./mixed_random <instance.json> [time_ms]\n";
        return 1;
    }

    std::string instance_file = argv[1];
    int max_time_ms = 31563;
    if (argc >= 3) {
        max_time_ms = std::stoi(argv[2]);
    }

    MixQUBO instance;
    instance.read_json(instance_file);

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> bit_dist(0, 1);
    std::uniform_real_distribution<double> z_dist(-1.0, 1.0);

    Solution current(instance.d, instance.n);
    Solution best(instance.d, instance.n);
    best.f = std::numeric_limits<double>::infinity();

    int nb_eval = 0;

    auto t0 = std::chrono::high_resolution_clock::now();

    while (time_left(t0, max_time_ms)) {
        for (int i = 0; i < instance.n; ++i) {
            current.x[i] = static_cast<bool>(bit_dist(rng));
        }

        for (int u = 0; u < instance.d; ++u) {
            current.z[u] = z_dist(rng);
        }

        instance.eval(current);
        nb_eval++;

        if (current.f < best.f) {
            best = current;
        }
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    double elapsed_ms =
        std::chrono::duration<double, std::milli>(t1 - t0).count();

    std::cout << "best_f=" << best.f << "\n";
    std::cout << "nb_eval=" << nb_eval << "\n";
    std::cout << "time_ms=" << elapsed_ms << "\n";
    std::cout << "best_solution=" << best << "\n";

    return 0;
}