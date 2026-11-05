/**
 * @file main.cpp
 * @brief Application driver for Newman-Ziff percolation simulation sweeps.
 * @author Ronda Venkata Karthik Reddy (Roll No: 250041033) - Architect
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <string>
#include "../src/structA/percolation_system.hpp"

int main(int argc, char* argv[]) {
    int L = 100;
    int sweeps = 1;
    uint32_t seed = 20260928;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--L" && i + 1 < argc) L = std::stoi(argv[++i]);
        else if (arg == "--sweeps" && i + 1 < argc) sweeps = std::stoi(argv[++i]);
        else if (arg == "--seed" && i + 1 < argc) seed = static_cast<uint32_t>(std::stoul(argv[++i]));
    }

    int N = L * L;
    std::mt19937 rng(seed);
    std::vector<int> sites(N);
    std::iota(sites.begin(), sites.end(), 0);

    for (int s = 0; s < sweeps; ++s) {
        std::shuffle(sites.begin(), sites.end(), rng);
        PercolationSystem perc(L);
        int percolation_step = -1;

        for (int step = 0; step < N; ++step) {
            int idx = sites[step];
            int r = idx / L;
            int c = idx % L;
            perc.open_site(r, c);

            if (percolation_step == -1 && perc.percolates()) {
                percolation_step = step + 1;
                break;
            }
        }

        double p_c = static_cast<double>(percolation_step) / N;
        std::cout << "Sweep " << (s + 1) << "/" << sweeps 
                  << " (L=" << L << "): percolation threshold p_c = " << p_c << "\n";
    }

    return 0;
}
