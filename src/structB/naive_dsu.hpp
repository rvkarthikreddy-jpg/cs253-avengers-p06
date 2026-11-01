#ifndef NAIVE_DSU_HPP
#define NAIVE_DSU_HPP

#include "../../include/disjoint_set.hpp"
#include <vector>
#include <stdexcept>
#include <algorithm>

/**
 * @brief Structure B - Ablation 1: Naive Disjoint Set Union.
 * Arbitrary root linking without rank/size balancing and standard
 * root traversal without path compression. Worst-case find: Theta(N).
 */
class NaiveDSU : public DisjointSet {
public:
    explicit NaiveDSU(int n);
    ~NaiveDSU() override = default;

    int find(int x) override;
    bool unite(int x, int y) override;
    bool connected(int x, int y) override;
    int component_size(int x) override;
    int largest_component_size() const override;
    int size() const override;
    void reset() override;

private:
    int n_;
    int max_component_size_;
    std::vector<int> parent_;
    std::vector<int> sz_;

    void validate_index(int x) const;
};

#endif // NAIVE_DSU_HPP