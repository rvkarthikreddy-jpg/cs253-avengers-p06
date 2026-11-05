/**
 * @file run_ops.cpp
 * @brief Hidden test evaluation runner for Project 06: Percolation with Union-Find.
 * @author Ronda Venkata Karthik Reddy (Roll No: 250041033) - Architect
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "../include/disjoint_set.hpp"
#include "../src/structA/structA_dsu.hpp"
#include "../src/structA/percolation_system.hpp"
#include "../src/structB/naive_dsu.hpp"
#include "../src/structB/rank_only_dsu.hpp"
#include "../src/structB/compression_only_dsu.hpp"

enum class Mode {
    STRUCTURE_A,
    STRUCTURE_B_NAIVE,
    STRUCTURE_B_RANK,
    STRUCTURE_B_COMPRESSION
};

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Mode mode = Mode::STRUCTURE_A;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--structure" && i + 1 < argc) {
            std::string val = argv[++i];
            if (val == "A" || val == "a") {
                mode = Mode::STRUCTURE_A;
            } else if (val == "B" || val == "B1" || val == "naive") {
                mode = Mode::STRUCTURE_B_NAIVE;
            } else if (val == "B2" || val == "rank") {
                mode = Mode::STRUCTURE_B_RANK;
            } else if (val == "B3" || val == "compression") {
                mode = Mode::STRUCTURE_B_COMPRESSION;
            }
        }
    }

    std::unique_ptr<DisjointSet> dsu = nullptr;
    std::unique_ptr<PercolationSystem> perc = nullptr;

    std::string op;
    while (std::cin >> op) {
        if (op == "INIT_DSU") {
            int n;
            std::cin >> n;
            if (mode == Mode::STRUCTURE_A) {
                dsu = std::make_unique<StructADSU>(n);
            } else if (mode == Mode::STRUCTURE_B_NAIVE) {
                dsu = std::make_unique<NaiveDSU>(n);
            } else if (mode == Mode::STRUCTURE_B_RANK) {
                dsu = std::make_unique<RankOnlyDSU>(n);
            } else {
                dsu = std::make_unique<CompressionOnlyDSU>(n);
            }
        } else if (op == "INIT_PERC") {
            int L;
            std::cin >> L;
            perc = std::make_unique<PercolationSystem>(L);
        } else if (op == "UNITE") {
            int u, v;
            std::cin >> u >> v;
            if (dsu) {
                std::cout << (dsu->unite(u, v) ? 1 : 0) << "\n";
            }
        } else if (op == "FIND") {
            int u;
            std::cin >> u;
            if (dsu) {
                std::cout << dsu->find(u) << "\n";
            }
        } else if (op == "CONNECTED") {
            int u, v;
            std::cin >> u >> v;
            if (dsu) {
                std::cout << (dsu->connected(u, v) ? 1 : 0) << "\n";
            }
        } else if (op == "SIZE") {
            int u;
            std::cin >> u;
            if (dsu) {
                std::cout << dsu->component_size(u) << "\n";
            }
        } else if (op == "LARGEST") {
            if (dsu) {
                std::cout << dsu->largest_component_size() << "\n";
            } else if (perc) {
                std::cout << perc->largest_cluster_size() << "\n";
            }
        } else if (op == "RESET") {
            if (dsu) {
                dsu->reset();
            }
        } else if (op == "OPEN") {
            int r, c;
            std::cin >> r >> c;
            if (perc) {
                perc->open_site(r, c);
            }
        } else if (op == "IS_OPEN") {
            int r, c;
            std::cin >> r >> c;
            if (perc) {
                std::cout << (perc->is_open(r, c) ? 1 : 0) << "\n";
            }
        } else if (op == "IS_FULL") {
            int r, c;
            std::cin >> r >> c;
            if (perc) {
                std::cout << (perc->is_full(r, c) ? 1 : 0) << "\n";
            }
        } else if (op == "PERCOLATES") {
            if (perc) {
                std::cout << (perc->percolates() ? 1 : 0) << "\n";
            }
        }
    }

    return 0;
}
