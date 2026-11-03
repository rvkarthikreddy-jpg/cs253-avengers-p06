#include "../oracle/bfs_oracle.hpp"
#include "invariants.hpp"
#include <iostream>
#include <random>
#include <cassert>

int main() {
    std::cout << "[RUNNING] BFS Oracle & Invariant Correctness Suite...\n";

    // 1. Sanity check BFS flood fill and backwash resistance
    BFSOracle oracle(4);
    assert(!oracle.percolates());
    assert(oracle.open_site_count() == 0);

    // Form vertical path along col 0: (0,0) -> (1,0) -> (2,0) -> (3,0)
    oracle.open_site(0, 0);
    oracle.open_site(1, 0);
    oracle.open_site(2, 0);
    assert(!oracle.percolates());
    assert(oracle.is_full(0, 0));
    assert(oracle.is_full(2, 0));

    oracle.open_site(3, 0);
    assert(oracle.percolates());
    assert(oracle.is_full(3, 0));

    // Open isolated bottom site (3, 3) - must NOT be full (backwash proof)
    oracle.open_site(3, 3);
    assert(oracle.is_open(3, 3));
    assert(!oracle.is_full(3, 3));

    std::cout << "[PASSED] Oracle BFS percolation and backwash checks.\n";
    std::cout << ">>> ALL ORACLE TESTS COMPLETED CLEANLY <<<\n";
    return 0;
}