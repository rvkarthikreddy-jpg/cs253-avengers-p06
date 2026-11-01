#include "compression_only_dsu.hpp"

CompressionOnlyDSU::CompressionOnlyDSU(int n)
    : n_(n), max_component_size_(n > 0 ? 1 : 0), parent_(n), sz_(n, 1) {
    if (n < 0) throw std::invalid_argument("Universe size cannot be negative");
    for (int i = 0; i < n_; ++i) parent_[i] = i;
}

void CompressionOnlyDSU::validate_index(int x) const {
    if (x < 0 || x >= n_) throw std::out_of_range("Index out of bounds");
}

int CompressionOnlyDSU::find(int x) {
    validate_index(x);
    // Pass 1: find root
    int root = x;
    while (root != parent_[root]) {
        root = parent_[root];
    }
    // Pass 2: full path compression
    int curr = x;
    while (curr != root) {
        int next = parent_[curr];
        parent_[curr] = root;
        curr = next;
    }
    return root;
}

bool CompressionOnlyDSU::unite(int x, int y) {
    validate_index(x);
    validate_index(y);
    int root_x = find(x);
    int root_y = find(y);
    if (root_x == root_y) return false;

    // Arbitrary linking without rank balancing
    parent_[root_x] = root_y;
    sz_[root_y] += sz_[root_x];
    max_component_size_ = std::max(max_component_size_, sz_[root_y]);
    return true;
}

bool CompressionOnlyDSU::connected(int x, int y) {
    return find(x) == find(y);
}

int CompressionOnlyDSU::component_size(int x) {
    validate_index(x);
    return sz_[find(x)];
}

int CompressionOnlyDSU::largest_component_size() const {
    return max_component_size_;
}

int CompressionOnlyDSU::size() const {
    return n_;
}

void CompressionOnlyDSU::reset() {
    max_component_size_ = n_ > 0 ? 1 : 0;
    for (int i = 0; i < n_; ++i) {
        parent_[i] = i;
        sz_[i] = 1;
    }
}