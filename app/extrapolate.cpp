/**
 * @file extrapolate.cpp
 * @brief Multi-sweep Monte Carlo finite-size scaling and extrapolation analysis in pure C++17.
 * @author Ronda Venkata Karthik Reddy (Roll No: 250041033) - Architect
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <string>
#include <cmath>
#include <iomanip>
#include "../src/structA/percolation_system.hpp"

struct SweepStatistics {
    int sweeps;
    double mean;
    double std_dev;
    double ci95_half_width;
    double ci_lower;
    double ci_upper;
    double x_coord; // L^(-0.75) for 2D percolation scaling
};

SweepStatistics run_simulation_sweeps(int L, int sweeps, uint32_t seed) {
    const int N = L * L;
    std::mt19937 rng(seed);
    std::vector<int> sites(N);
    std::iota(sites.begin(), sites.end(), 0);

    std::vector<double> pc_samples;
    pc_samples.reserve(sweeps);

    std::cout << ">>> Running " << sweeps << " independent Newman-Ziff sweeps for L = " 
              << L << " (N = " << N << " sites)...\n";

    for (int s = 0; s < sweeps; ++s) {
        std::shuffle(sites.begin(), sites.end(), rng);
        PercolationSystem perc(L);
        int percolation_step = -1;

        for (int step = 0; step < N; ++step) {
            int idx = sites[step];
            int r = idx / L;
            int c = idx % L;
            perc.open_site(r, c);

            if (perc.percolates()) {
                percolation_step = step + 1;
                break;
            }
        }

        double p_c = static_cast<double>(percolation_step) / N;
        pc_samples.push_back(p_c);

        if ((s + 1) % 10 == 0 || s + 1 == sweeps) {
            std::cout << "    [Completed " << std::setw(3) << (s + 1) << "/" << sweeps 
                      << " sweeps] Latest p_c = " << std::fixed << std::setprecision(5) << p_c << "\n";
        }
    }

    // Compute sample mean
    double sum = 0.0;
    for (double val : pc_samples) {
        sum += val;
    }
    double mean = sum / sweeps;

    // Compute sample standard deviation: s = sqrt( sum((x - mean)^2) / (T - 1) )
    double variance_sum = 0.0;
    for (double val : pc_samples) {
        variance_sum += (val - mean) * (val - mean);
    }
    double std_dev = (sweeps > 1) ? std::sqrt(variance_sum / (sweeps - 1)) : 0.0;

    // 95% Confidence Interval: CI = 1.96 * (s / sqrt(T))
    double ci_half = (sweeps > 1) ? 1.96 * (std_dev / std::sqrt(sweeps)) : 0.0;
    double x_scaling = std::pow(static_cast<double>(L), -0.75);

    return SweepStatistics{sweeps, mean, std_dev, ci_half, mean - ci_half, mean + ci_half, x_scaling};
}

int main(int argc, char* argv[]) {
    int L = 100;
    int sweeps = 50;
    uint32_t seed = 20260928;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--L" && i + 1 < argc) L = std::stoi(argv[++i]);
        else if (arg == "--sweeps" && i + 1 < argc) sweeps = std::stoi(argv[++i]);
        else if (arg == "--seed" && i + 1 < argc) seed = static_cast<uint32_t>(std::stoul(argv[++i]));
    }

    SweepStatistics stats = run_simulation_sweeps(L, sweeps, seed);

    std::cout << "\n=======================================================\n";
    std::cout << "         STATISTICAL SCALING REPORT (PURE C++)         \n";
    std::cout << "=======================================================\n";
    std::cout << "Lattice Dimension (L)     : " << L << "\n";
    std::cout << "Finite-Size Term (L^-0.75): " << std::fixed << std::setprecision(6) << stats.x_coord << "\n";
    std::cout << "Total Sweeps Run (T)      : " << stats.sweeps << "\n";
    std::cout << "Sample Mean (p_c_bar)     : " << stats.mean << "\n";
    std::cout << "Sample Std Dev (s)        : " << stats.std_dev << "\n";
    std::cout << "95% CI Half-Width         : ±" << stats.ci95_half_width << "\n";
    std::cout << "Confidence Interval Band  : [" << stats.ci_lower << ", " << stats.ci_upper << "]\n";
    std::cout << "Theoretical 2D Constant   : 0.592746\n";
    std::cout << "=======================================================\n";

    return 0;
}
