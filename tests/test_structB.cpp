#include "../src/structB/naive_dsu.hpp"
#include "../src/structB/rank_only_dsu.hpp"
#include "../src/structB/compression_only_dsu.hpp"
#include <iostream>
#include <cassert>
#include <string>

void verify_dsu_invariants(DisjointSet& dsu, const std::string& name) {
    std::cout << "[TESTING] " << name << "...\n";
    const int N = 6;

    // Invariants 1, 2, 3: Initial conditions (singletons)
    assert(dsu.size() == N);
    assert(dsu.largest_component_size() == 1);
    for (int i = 0; i < N; ++i) {
        assert(dsu.find(i) == i);
        assert(dsu.component_size(i) == 1);
        assert(dsu.connected(i, i));
    }

    // Unite 0 and 1
    assert(dsu.unite(0, 1) == true);
    assert(dsu.connected(0, 1) == true);
    assert(dsu.connected(0, 2) == false);
    assert(dsu.component_size(0) == 2);
    assert(dsu.component_size(1) == 2);
    assert(dsu.largest_component_size() == 2);

    // Redundant unite must return false
    assert(dsu.unite(0, 1) == false);

    // Unite {2, 3} and link {0, 1} with {2, 3}
    assert(dsu.unite(2, 3) == true);
    assert(dsu.largest_component_size() == 2);
    assert(dsu.unite(1, 3) == true);
    assert(dsu.connected(0, 2) == true);
    assert(dsu.connected(0, 3) == true);
    assert(dsu.component_size(0) == 4);
    assert(dsu.largest_component_size() == 4);

    // Invariant 4: Sum of distinct root component sizes must equal N
    int sum_sz = 0;
    for (int i = 0; i < N; ++i) {
        if (dsu.find(i) == i) {
            sum_sz += dsu.component_size(i);
        }
    }
    assert(sum_sz == N);

    // Reset check
    dsu.reset();
    assert(dsu.largest_component_size() == 1);
    for (int i = 0; i < N; ++i) {
        assert(dsu.find(i) == i);
        assert(dsu.component_size(i) == 1);
    }

    std::cout << "[PASSED] " << name << " passed all invariant checks!\n";
}

int main() {
    std::cout << "Starting Structure B ablation verification...\n\n";

    NaiveDSU naive(6);
    verify_dsu_invariants(naive, "Ablation 1 (NaiveDSU)");

    RankOnlyDSU rank_only(6);
    verify_dsu_invariants(rank_only, "Ablation 2 (RankOnlyDSU)");

    CompressionOnlyDSU comp_only(6);
    verify_dsu_invariants(comp_only, "Ablation 3 (CompressionOnlyDSU)");

    std::cout << "\n>>> ALL THREE STRUCTURE B ABLATIONS PASSED CLEANLY! <<<\n";
    return 0;
}