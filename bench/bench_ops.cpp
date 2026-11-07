/**
 * @file bench_ops.cpp
 * @brief Benchmark harness profiling runtime and find-path operation counters.
 * @author Bhogaraju M Pranav Rohit (Roll No: 250001014) - Analyst
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <numeric>
#include "../src/structA/structA_dsu.hpp"
#include "../src/structB/naive_dsu.hpp"
#include "../src/structB/rank_only_dsu.hpp"
#include "../src/structB/compression_only_dsu.hpp"

// Standalone Instrumented Disjoint Sets to count pointer hops precisely
struct HopCounterDSU {
    std::vector<int> parent;
    std::vector<int> rank;
    unsigned long long total_hops = 0;

    explicit HopCounterDSU(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find_naive(int x) {
        int root = x;
        while (root != parent[root]) {
            total_hops++;
            root = parent[root];
        }
        return root;
    }

    int find_rank(int x) {
        int root = x;
        while (root != parent[root]) {
            total_hops++;
            root = parent[root];
        }
        return root;
    }

    int find_compression(int x) {
        int root = x;
        while (root != parent[root]) {
            total_hops++;
            root = parent[root];
        }
        int curr = x;
        while (curr != root) {
            int next = parent[curr];
            parent[curr] = root;
            curr = next;
        }
        return root;
    }

    int find_structA(int x) {
        int root = x;
        while (root != parent[root]) {
            total_hops++;
            root = parent[root];
        }
        int curr = x;
        while (curr != root) {
            int next = parent[curr];
            parent[curr] = root;
            curr = next;
        }
        return root;
    }

    void unite_naive(int x, int y) {
        int rx = find_naive(x);
        int ry = find_naive(y);
        if (rx != ry) parent[rx] = ry;
    }

    void unite_rank(int x, int y) {
        int rx = find_rank(x);
        int ry = find_rank(y);
        if (rx == ry) return;
        if (rank[rx] < rank[ry]) parent[rx] = ry;
        else if (rank[rx] > rank[ry]) parent[ry] = rx;
        else { parent[ry] = rx; rank[rx]++; }
    }

    void unite_comp(int x, int y) {
        int rx = find_compression(x);
        int ry = find_compression(y);
        if (rx != ry) parent[rx] = ry;
    }

    void unite_structA(int x, int y) {
        int rx = find_structA(x);
        int ry = find_structA(y);
        if (rx == ry) return;
        if (rank[rx] < rank[ry]) parent[rx] = ry;
        else if (rank[rx] > rank[ry]) parent[ry] = rx;
        else { parent[ry] = rx; rank[rx]++; }
    }
};

struct BenchmarkRow {
    int N;
    double time_A_ms, time_B1_ms, time_B2_ms, time_B3_ms;
    unsigned long long hops_A, hops_B1, hops_B2, hops_B3;
};

BenchmarkRow profile_size(int N, uint32_t seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, N - 1);

    std::vector<std::pair<int, int>> pairs(N);
    for (int i = 0; i < N; ++i) {
        pairs[i] = {dist(rng), dist(rng)};
    }

    // 1. Measure Timings
    auto measure_time = [&](DisjointSet& dsu) {
        auto start = std::chrono::high_resolution_clock::now();
        for (const auto& p : pairs) dsu.unite(p.first, p.second);
        for (const auto& p : pairs) {
            volatile bool c = dsu.connected(p.first, p.second);
            (void)c;
        }
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    };

    StructADSU dsuA(N);
    NaiveDSU dsuB1(N);
    RankOnlyDSU dsuB2(N);
    CompressionOnlyDSU dsuB3(N);

    double tA = measure_time(dsuA);
    double tB1 = (N <= 30000) ? measure_time(dsuB1) : -1.0; // Avoid hang on large quadratic runs
    double tB2 = measure_time(dsuB2);
    double tB3 = measure_time(dsuB3);

    // 2. Measure Operation Hops
    HopCounterDSU hopA(N), hopB1(N), hopB2(N), hopB3(N);
    for (const auto& p : pairs) hopA.unite_structA(p.first, p.second);
    for (const auto& p : pairs) hopA.find_structA(p.first);

    for (const auto& p : pairs) hopB2.unite_rank(p.first, p.second);
    for (const auto& p : pairs) hopB2.find_rank(p.first);

    for (const auto& p : pairs) hopB3.unite_comp(p.first, p.second);
    for (const auto& p : pairs) hopB3.find_compression(p.first);

    if (N <= 30000) {
        for (const auto& p : pairs) hopB1.unite_naive(p.first, p.second);
        for (const auto& p : pairs) hopB1.find_naive(p.first);
    }

    return {N, tA, tB1, tB2, tB3, hopA.total_hops, (N <= 30000 ? hopB1.total_hops : 0ULL), hopB2.total_hops, hopB3.total_hops};
}

int main(int argc, char* argv[]) {
    uint32_t seed = 20260928;
    if (argc > 1) seed = static_cast<uint32_t>(std::stoul(argv[1]));

    std::ofstream csv("results/raw/dsu_scaling.csv");
    csv << "N,StructA_ms,Naive_ms,RankOnly_ms,CompressionOnly_ms,StructA_hops,Naive_hops,RankOnly_hops,CompressionOnly_hops\n";

    std::vector<int> test_sizes = {1000, 5000, 10000, 20000, 30000, 50000, 100000};
    std::cout << ">>> Running Micro-Benchmark Suite with Operation Counters (Seed: " << seed << ")...\n";

    for (int n : test_sizes) {
        BenchmarkRow row = profile_size(n, seed);
        csv << row.N << "," << row.time_A_ms << "," << row.time_B1_ms << ","
            << row.time_B2_ms << "," << row.time_B3_ms << ","
            << row.hops_A << "," << row.hops_B1 << "," << row.hops_B2 << "," << row.hops_B3 << "\n";

        std::cout << "  N = " << row.N 
                  << " | Time (StructA / Naive): " << row.time_A_ms << " ms / " << row.time_B1_ms << " ms"
                  << " | Hops (StructA / Naive): " << row.hops_A << " / " << row.hops_B1 << "\n";
    }

    std::cout << ">>> Micro-benchmark data written to results/raw/dsu_scaling.csv\n";
    return 0;
}
