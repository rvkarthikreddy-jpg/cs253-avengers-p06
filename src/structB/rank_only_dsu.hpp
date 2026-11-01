#ifndef RANK_ONLY_DSU_HPP
#define RANK_ONLY_DSU_HPP

#include "../../include/disjoint_set.hpp"
#include <vector>
#include <stdexcept>
#include <algorithm>

/**
 * @brief Structure B - Ablation 2: Union by Rank without path compression.
 * Bounds tree height by floor(log2 N), guaranteeing O(log N) worst-case find and unite.
 */
class RankOnlyDSU : public DisjointSet {
public:
    explicit RankOnlyDSU(int n);
    ~RankOnlyDSU() override = default;

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
    std::vector<int> rank_;
    std::vector<int> sz_;

    void validate_index(int x) const;
};

#endif // RANK_ONLY_DSU_HPP