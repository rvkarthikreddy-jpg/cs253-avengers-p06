#include "percolation_system.hpp"

PercolationSystem::PercolationSystem(int L)
    : L_(L),
      N_(L * L),
      vtop_(L * L),
      vbottom_(L * L + 1),
      open_count_(0),
      opened_(L * L, false),
      dsu_perc_(L * L + 2),
      dsu_full_(L * L + 1),
      dsu_grid_(L * L) {
    if (L <= 0) throw std::invalid_argument("L must be positive");
}

void PercolationSystem::validate_coords(int r, int c) const {
    if (r < 0 || r >= L_ || c < 0 || c >= L_) throw std::out_of_range("Coordinates out of bounds");
}

int PercolationSystem::to_index(int r, int c) const {
    return r * L_ + c;
}

void PercolationSystem::open_site(int r, int c) {
    validate_coords(r, c);
    int idx = to_index(r, c);
    if (opened_[idx]) return;

    opened_[idx] = true;
    open_count_++;

    if (r == 0) {
        dsu_perc_.unite(idx, vtop_);
        dsu_full_.unite(idx, vtop_);
    }
    if (r == L_ - 1) {
        dsu_perc_.unite(idx, vbottom_);
    }

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    for (int k = 0; k < 4; ++k) {
        int nr = r + dr[k], nc = c + dc[k];
        if (nr >= 0 && nr < L_ && nc >= 0 && nc < L_) {
            int n_idx = to_index(nr, nc);
            if (opened_[n_idx]) {
                dsu_perc_.unite(idx, n_idx);
                dsu_full_.unite(idx, n_idx);
                dsu_grid_.unite(idx, n_idx);
            }
        }
    }
}

bool PercolationSystem::is_open(int r, int c) const {
    validate_coords(r, c);
    return opened_[to_index(r, c)];
}

bool PercolationSystem::is_full(int r, int c) {
    validate_coords(r, c);
    int idx = to_index(r, c);
    if (!opened_[idx]) return false;
    return dsu_full_.connected(idx, vtop_);
}

bool PercolationSystem::percolates() {
    return dsu_perc_.connected(vtop_, vbottom_);
}

int PercolationSystem::open_site_count() const {
    return open_count_;
}

int PercolationSystem::largest_cluster_size() {
    if (open_count_ == 0) return 0;
    return dsu_grid_.largest_component_size();
}