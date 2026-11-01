#include "naive_dsu.hpp"

NaiveDSU::NaiveDSU(int n)
    : n_(n), max_component_size_(n > 0 ? 1 : 0), parent_(n), sz_(n, 1) {
    if (n < 0) throw std::invalid_argument("Universe size cannot be negative");
    for (int i = 0; i < n_; ++i) parent_[i] = i;
}

void NaiveDSU::validate_index(int x) const {
    if (x < 0 || x >= n_) throw std::out_of_range("Index out of bounds");
}

int NaiveDSU::find(int x) {
    validate_index(x);
    int root = x;
    while (root != parent_[root]) {
        root = parent_[root];
    }
    return root;
}

bool NaiveDSU::unite(int x, int y) {
    validate_index(x);
    validate_index(y);
    int root_x = find(x);
    int root_y = find(y);
    if (root_x == root_y) return false;

    // Arbitrary linking: root_x attached unconditionally under root_y
    parent_[root_x] = root_y;
    sz_[root_y] += sz_[root_x];
    max_component_size_ = std::max(max_component_size_, sz_[root_y]);
    return true;
}

bool NaiveDSU::connected(int x, int y) {
    return find(x) == find(y);
}

int NaiveDSU::component_size(int x) {
    validate_index(x);
    return sz_[find(x)];
}

int NaiveDSU::largest_component_size() const {
    return max_component_size_;
}

int NaiveDSU::size() const {
    return n_;
}

void NaiveDSU::reset() {
    max_component_size_ = n_ > 0 ? 1 : 0;
    for (int i = 0; i < n_; ++i) {
        parent_[i] = i;
        sz_[i] = 1;
    }
}