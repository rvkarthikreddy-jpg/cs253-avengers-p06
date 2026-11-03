#ifndef INVARIANTS_HPP
#define INVARIANTS_HPP

#include "../include/disjoint_set.hpp"
#include <cassert>
#include <vector>

inline void assert_structural_invariants(DisjointSet& dsu) {
    const int N = dsu.size();

    // Invariant 1: Completeness & Invariant 2: Acyclicity & Idempotency
    for (int i = 0; i < N; ++i) {
        int r = dsu.find(i);
        assert(r >= 0 && r < N);
        assert(dsu.find(r) == r); // Idempotency
    }

    // Invariant 4: Component size conservation (sum of all root sizes == N)
    int total_nodes = 0;
    int observed_max = 0;
    for (int i = 0; i < N; ++i) {
        if (dsu.find(i) == i) {
            int sz = dsu.component_size(i);
            assert(sz >= 1);
            total_nodes += sz;
            if (sz > observed_max) observed_max = sz;
        }
    }
    assert(total_nodes == N);

    // Invariant 5: Global maximum tracking in O(1)
    if (N > 0) {
        assert(dsu.largest_component_size() == observed_max);
    }
}

#endif // INVARIANTS_HPP