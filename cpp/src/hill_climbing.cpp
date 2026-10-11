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
        std::cerr << "Usage: ./hill_climbing <instance.json> [time_ms]\n";
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

    Solution current(instance.d, instance.n);
    Solution best(instance.d, instance.n);
    Solution candidate(instance.d, instance.n);

    for (int u = 0; u < instance.d; ++u) {
        current.z[u] = 0.0;
        best.z[u] = 0.0;
        candidate.z[u] = 0.0;
    }

    for (int i = 0; i < instance.n; ++i) {
        current.x[i] = static_cast<bool>(bit_dist(rng));
    }

    instance.eval(current);
    best = current;

    int nb_eval = 1;
    auto t0 = std::chrono::high_resolution_clock::now();

    while (time_left(t0, max_time_ms)) {
        bool improved = false;

        Solution best_neighbor = current;
        double best_neighbor_f = current.f;

        for (int i = 0; i < instance.n && time_left(t0, max_time_ms); ++i) {
            candidate = current;
            candidate.x[i] = !candidate.x[i];

            instance.eval(candidate);
            nb_eval++;

            if (candidate.f < best_neighbor_f) {
                best_neighbor = candidate;
                best_neighbor_f = candidate.f;
                improved = true;
            }
        }

        if (improved) {
            current = best_neighbor;
            if (current.f < best.f) {
                best = current;
            }
        } else {
            for (int i = 0; i < instance.n; ++i) {
                current.x[i] = static_cast<bool>(bit_dist(rng));
            }
            instance.eval(current);
            nb_eval++;

            if (current.f < best.f) {
                best = current;
            }
        }
    }

    std::cout << "best_f=" << best.f << "\n";
    std::cout << "nb_eval=" << nb_eval << "\n";
    std::cout << "time_ms=" << elapsed_ms(t0) << "\n";
    std::cout << "best_solution=" << best << "\n";

    return 0;
}