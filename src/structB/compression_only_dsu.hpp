#ifndef COMPRESSION_ONLY_DSU_HPP
#define COMPRESSION_ONLY_DSU_HPP

#include "../../include/disjoint_set.hpp"
#include <vector>
#include <stdexcept>
#include <algorithm>

/**
 * @brief Structure B - Ablation 3: Path Compression without rank balancing.
 * Arbitrary root attachment with two-pass full path compression.
 * Amortized cost: O(log N) (Tarjan-van Leeuwen theorem).
 */
class CompressionOnlyDSU : public DisjointSet {
public:
    explicit CompressionOnlyDSU(int n);
    ~CompressionOnlyDSU() override = default;

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

#endif // COMPRESSION_ONLY_DSU_HPP