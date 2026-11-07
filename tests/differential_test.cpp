#include "../src/structA/structA_dsu.hpp"
#include "../src/structA/percolation_system.hpp"
#include "../src/structB/naive_dsu.hpp"
#include "../src/structB/rank_only_dsu.hpp"
#include "../src/structB/compression_only_dsu.hpp"
#include "../oracle/bfs_oracle.hpp"
#include "invariants.hpp"
#include <iostream>
#include <random>
#include <cassert>
#include <string>

int main(int argc, char* argv[]) {
    int total_ops = 100000; // Mandatory 10^5 operations for Checkpoint 2 (c2)
    unsigned int seed = 20260928;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--ops" && i + 1 < argc) total_ops = std::stoi(argv[++i]);
        else if (arg == "--seed" && i + 1 < argc) seed = static_cast<unsigned int>(std::stoul(argv[++i]));
    }

    std::cout << "[RUNNING] Differential testing across Structure A, Structure B, and Oracle (" 
              << total_ops << " ops, seed=" << seed << ")...\n";

    // --- Phase 1: Disjoint Set Operations across Structure A and all 3 Structure B ablations ---
    const int N = 1000;
    StructADSU dsuA(N);
    NaiveDSU dsuB1(N);
    RankOnlyDSU dsuB2(N);
    CompressionOnlyDSU dsuB3(N);

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> op_dist(0, 3);
    std::uniform_int_distribution<int> elem_dist(0, N - 1);

    for (int op = 1; op <= total_ops; ++op) {
        int type = op_dist(rng);
        int u = elem_dist(rng);
        int v = elem_dist(rng);

        if (type == 0) { // unite
            bool resA = dsuA.unite(u, v);
            bool resB1 = dsuB1.unite(u, v);
            bool resB2 = dsuB2.unite(u, v);
            bool resB3 = dsuB3.unite(u, v);
            assert(resA == resB1 && resA == resB2 && resA == resB3);
        } else if (type == 1) { // connected
            bool connA = dsuA.connected(u, v);
            bool connB1 = dsuB1.connected(u, v);
            bool connB2 = dsuB2.connected(u, v);
            bool connB3 = dsuB3.connected(u, v);
            assert(connA == connB1 && connA == connB2 && connA == connB3);
        } else if (type == 2) { // component_size
            int szA = dsuA.component_size(u);
            int szB1 = dsuB1.component_size(u);
            int szB2 = dsuB2.component_size(u);
            int szB3 = dsuB3.component_size(u);
            assert(szA == szB1 && szA == szB2 && szA == szB3);
        } else { // largest_component_size
            int maxA = dsuA.largest_component_size();
            int maxB1 = dsuB1.largest_component_size();
            int maxB2 = dsuB2.largest_component_size();
            int maxB3 = dsuB3.largest_component_size();
            assert(maxA == maxB1 && maxA == maxB2 && maxA == maxB3);
        }

        // Periodic validation of formal Invariants 1 through 5
        if (op % 25000 == 0) {
            assert_structural_invariants(dsuA);
            assert_structural_invariants(dsuB1);
            assert_structural_invariants(dsuB2);
            assert_structural_invariants(dsuB3);
            std::cout << "  Passed " << op << " DSU operations...\n";
        }
    }

    // --- Phase 2: Differential Lattice Verification (Structure A vs BFS Oracle) ---
    std::cout << "[RUNNING] Differential lattice test: Structure A vs BFS Oracle...\n";
    const int L = 15;
    PercolationSystem perc(L);
    BFSOracle oracle(L);

    std::uniform_int_distribution<int> grid_dist(0, L - 1);
    for (int step = 0; step < L * L; ++step) {
        int r = grid_dist(rng);
        int c = grid_dist(rng);

        perc.open_site(r, c);
        oracle.open_site(r, c);

        assert(perc.percolates() == oracle.percolates());
        assert(perc.is_full(r, c) == oracle.is_full(r, c));
        assert(perc.is_open(r, c) == oracle.is_open(r, c));
    }

    std::cout << ">>> ALL TESTS PASSED (CHECKPOINT 2) <<<\n";
    return 0;
}
