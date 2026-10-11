#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <string>

#include "solution.hpp"
#include "mixQUBO.hpp"
#include "time_utils.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./simulated_annealing <instance.json> [time_ms]\n";
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
    std::uniform_int_distribution<int> bit_index_dist(0, instance.n - 1);
    std::uniform_int_distribution<int> bit_value_dist(0, 1);
    std::uniform_real_distribution<double> real_dist(0.0, 1.0);

    Solution current(instance.d, instance.n);
    Solution best(instance.d, instance.n);
    Solution candidate(instance.d, instance.n);

    for (int u = 0; u < instance.d; ++u) {
        current.z[u] = 0.0;
        best.z[u] = 0.0;
        candidate.z[u] = 0.0;
    }

    for (int i = 0; i < instance.n; ++i) {
        current.x[i] = static_cast<bool>(bit_value_dist(rng));
    }

    instance.eval(current);
    best = current;

    int nb_eval = 1;
    double T = 10.0;
    double alpha = 0.995;

    auto t0 = std::chrono::high_resolution_clock::now();

    while (time_left(t0, max_time_ms)) {
        candidate = current;

        int i = bit_index_dist(rng);
        candidate.x[i] = !candidate.x[i];

        instance.eval(candidate);
        nb_eval++;

        double delta = candidate.f - current.f;

        if (delta < 0.0) {
            current = candidate;
        } else {
            double p = std::exp(-delta / T);
            if (real_dist(rng) < p) {
                current = candidate;
            }
        }

        if (current.f < best.f) {
            best = current;
        }

        T *= alpha;
        if (T < 1e-8) {
            T = 1e-8;
        }
    }

    std::cout << "best_f=" << best.f << "\n";
    std::cout << "nb_eval=" << nb_eval << "\n";
    std::cout << "time_ms=" << elapsed_ms(t0) << "\n";
    std::cout << "best_solution=" << best << "\n";

    return 0;
}