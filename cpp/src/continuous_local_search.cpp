#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <string>

#include "solution.hpp"
#include "mixQUBO.hpp"
#include "time_utils.hpp"

static double clamp_value(double x, double lo, double hi) {
    return std::max(lo, std::min(hi, x));
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./continuous_local_search <instance.json> [time_ms]\n";
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
    std::uniform_real_distribution<double> init_dist(-1.0, 1.0);
    std::uniform_int_distribution<int> coord_dist(0, instance.d - 1);
    std::normal_distribution<double> noise_dist(0.0, 0.1);

    Solution current(instance.d, instance.n);
    Solution best(instance.d, instance.n);
    Solution candidate(instance.d, instance.n);

    // Partie continue : x = 0
    for (int i = 0; i < instance.n; ++i) {
        current.x[i] = false;
        best.x[i] = false;
        candidate.x[i] = false;
    }

    for (int u = 0; u < instance.d; ++u) {
        current.z[u] = init_dist(rng);
    }

    instance.eval(current);
    best = current;

    int nb_eval = 1;
    auto t0 = std::chrono::high_resolution_clock::now();

    while (time_left(t0, max_time_ms)) {
        candidate = current;

        int u = coord_dist(rng);
        candidate.z[u] = clamp_value(candidate.z[u] + noise_dist(rng), -1.0, 1.0);

        instance.eval(candidate);
        nb_eval++;

        if (candidate.f < current.f) {
            current = candidate;

            if (current.f < best.f) {
                best = current;
            }
        } else {
            // petit restart pour exploiter tout le budget temps
            if (nb_eval % 5000 == 0) {
                for (int k = 0; k < instance.d; ++k) {
                    current.z[k] = init_dist(rng);
                }
                instance.eval(current);
                nb_eval++;

                if (current.f < best.f) {
                    best = current;
                }
            }
        }
    }

    std::cout << "best_f=" << best.f << "\n";
    std::cout << "nb_eval=" << nb_eval << "\n";
    std::cout << "time_ms=" << elapsed_ms(t0) << "\n";
    std::cout << "best_solution=" << best << "\n";

    return 0;
}