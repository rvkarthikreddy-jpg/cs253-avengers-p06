#ifndef BFS_ORACLE_HPP
#define BFS_ORACLE_HPP

#include <vector>

/**
 * @brief Independent 2D BFS flood-fill correctness oracle for percolation.
 * Conforms to Project 6 guidelines: uses standard multi-source queue traversal 
 * from row 0 without any Disjoint Set Union structures.
 */
class BFSOracle {
public:
    explicit BFSOracle(int L);

    void open_site(int r, int c);
    bool is_open(int r, int c) const;
    bool is_full(int r, int c) const;
    bool percolates() const;
    int open_site_count() const;
    void reset();

private:
    int L_;
    std::vector<std::vector<bool>> grid_;
    mutable std::vector<std::vector<bool>> full_;
    mutable bool state_valid_;
    mutable bool percolates_cached_;

    void run_flood_fill() const;
    bool in_bounds(int r, int c) const;
};

#endif // BFS_ORACLE_HPP