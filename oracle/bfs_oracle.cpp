#include "bfs_oracle.hpp"
#include <queue>
#include <stdexcept>
#include <algorithm>

BFSOracle::BFSOracle(int L)
    : L_(L),
      grid_(L, std::vector<bool>(L, false)),
      full_(L, std::vector<bool>(L, false)),
      state_valid_(false),
      percolates_cached_(false) {
    if (L <= 0) {
        throw std::invalid_argument("Lattice dimension L must be positive");
    }
}

bool BFSOracle::in_bounds(int r, int c) const {
    return r >= 0 && r < L_ && c >= 0 && c < L_;
}

void BFSOracle::open_site(int r, int c) {
    if (!in_bounds(r, c)) {
        throw std::out_of_range("Coordinates out of bounds in BFSOracle");
    }
    if (!grid_[r][c]) {
        grid_[r][c] = true;
        state_valid_ = false; // Invalidate cached BFS tree
    }
}

bool BFSOracle::is_open(int r, int c) const {
    if (!in_bounds(r, c)) {
        throw std::out_of_range("Coordinates out of bounds in BFSOracle");
    }
    return grid_[r][c];
}

void BFSOracle::run_flood_fill() const {
    if (state_valid_) return;

    for (int r = 0; r < L_; ++r) {
        std::fill(full_[r].begin(), full_[r].end(), false);
    }
    percolates_cached_ = false;

    std::queue<std::pair<int, int>> q;
    // Multi-source BFS seed: all open sites on top row (r = 0)
    for (int c = 0; c < L_; ++c) {
        if (grid_[0][c]) {
            full_[0][c] = true;
            q.push({0, c});
        }
    }

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        std::pair<int, int> curr = q.front();
        q.pop();
        int r = curr.first;
        int c = curr.second;

        if (r == L_ - 1) {
            percolates_cached_ = true;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (in_bounds(nr, nc) && grid_[nr][nc] && !full_[nr][nc]) {
                full_[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }

    state_valid_ = true;
}

bool BFSOracle::is_full(int r, int c) const {
    if (!in_bounds(r, c)) {
        throw std::out_of_range("Coordinates out of bounds in BFSOracle");
    }
    run_flood_fill();
    return full_[r][c];
}

bool BFSOracle::percolates() const {
    run_flood_fill();
    return percolates_cached_;
}

int BFSOracle::open_site_count() const {
    int count = 0;
    for (int r = 0; r < L_; ++r) {
        for (int c = 0; c < L_; ++c) {
            if (grid_[r][c]) count++;
        }
    }
    return count;
}

void BFSOracle::reset() {
    for (int r = 0; r < L_; ++r) {
        std::fill(grid_[r].begin(), grid_[r].end(), false);
        std::fill(full_[r].begin(), full_[r].end(), false);
    }
    state_valid_ = false;
    percolates_cached_ = false;
}