#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <string>

#include "solution.hpp"
#include "mixQUBO.hpp"
#include "time_utils.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./continuous_random <instance.json> [time_ms]\n";
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
    std::uniform_real_distribution<double> z_dist(-1.0, 1.0);

    Solution best(instance.d, instance.n);
    best.f = std::numeric_limits<double>::infinity();

    Solution current(instance.d, instance.n);

    // Partie continue : x = 0
    for (int i = 0; i < instance.n; ++i) {
        current.x[i] = false;
    }

    int nb_eval = 0;
    auto t0 = std::chrono::high_resolution_clock::now();

    while (time_left(t0, max_time_ms)) {
        for (int u = 0; u < instance.d; ++u) {
            current.z[u] = z_dist(rng);
        }

        instance.eval(current);
        nb_eval++;

        if (current.f < best.f) {
            best = current;
        }
    }

    std::cout << "best_f=" << best.f << "\n";
    std::cout << "nb_eval=" << nb_eval << "\n";
    std::cout << "time_ms=" << elapsed_ms(t0) << "\n";
    std::cout << "best_solution=" << best << "\n";

    return 0;
}