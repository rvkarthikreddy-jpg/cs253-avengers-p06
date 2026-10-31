#ifndef PERCOLATION_SYSTEM_HPP
#define PERCOLATION_SYSTEM_HPP

#include "structA_dsu.hpp"
#include <vector>
#include <stdexcept>

class PercolationSystem {
public:
    explicit PercolationSystem(int L);

    void open_site(int r, int c);
    bool is_open(int r, int c) const;
    bool is_full(int r, int c);
    bool percolates();
    int open_site_count() const;
    int largest_cluster_size();

private:
    int L_;
    int N_;
    int vtop_;
    int vbottom_;
    int open_count_;
    std::vector<bool> opened_;

    StructADSU dsu_perc_;
    StructADSU dsu_full_;
    StructADSU dsu_grid_;

    int to_index(int r, int c) const;
    void validate_coords(int r, int c) const;
};

#endif // PERCOLATION_SYSTEM_HPP