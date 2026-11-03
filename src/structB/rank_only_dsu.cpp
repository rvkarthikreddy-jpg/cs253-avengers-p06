#include "rank_only_dsu.hpp"

RankOnlyDSU::RankOnlyDSU(int n)
    : n_(n), max_component_size_(n > 0 ? 1 : 0), parent_(n), rank_(n, 0), sz_(n, 1) {
    if (n < 0) throw std::invalid_argument("Universe size cannot be negative");
    for (int i = 0; i < n_; ++i) parent_[i] = i;
}

void RankOnlyDSU::validate_index(int x) const {
    if (x < 0 || x >= n_) throw std::out_of_range("Index out of bounds");
}

int RankOnlyDSU::find(int x) {
    validate_index(x);
    int root = x;
    while (root != parent_[root]) {
        root = parent_[root];
    }
    return root;
}

bool RankOnlyDSU::unite(int x, int y) {
    validate_index(x);
    validate_index(y);
    int root_x = find(x);
    int root_y = find(y);
    if (root_x == root_y) return false;

    // Union by rank heuristic
    if (rank_[root_x] < rank_[root_y]) {
        parent_[root_x] = root_y;
        sz_[root_y] += sz_[root_x];
        max_component_size_ = std::max(max_component_size_, sz_[root_y]);
    } else if (rank_[root_x] > rank_[root_y]) {
        parent_[root_y] = root_x;
        sz_[root_x] += sz_[root_y];
        max_component_size_ = std::max(max_component_size_, sz_[root_x]);
    } else {
        parent_[root_y] = root_x;
        sz_[root_x] += sz_[root_y];
        rank_[root_x] += 1;
        max_component_size_ = std::max(max_component_size_, sz_[root_x]);
    }
    return true;
}

bool RankOnlyDSU::connected(int x, int y) {
    return find(x) == find(y);
}

int RankOnlyDSU::component_size(int x) {
    validate_index(x);
    return sz_[find(x)];
}

int RankOnlyDSU::largest_component_size() const {
    return max_component_size_;
}

int RankOnlyDSU::size() const {
    return n_;
}

void RankOnlyDSU::reset() {
    max_component_size_ = n_ > 0 ? 1 : 0;
    for (int i = 0; i < n_; ++i) {
        parent_[i] = i;
        rank_[i] = 0;
        sz_[i] = 1;
    }
}