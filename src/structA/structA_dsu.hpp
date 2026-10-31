#ifndef STRUCTA_DSU_HPP
#define STRUCTA_DSU_HPP

#include "../../include/disjoint_set.hpp"
#include <vector>
#include <stdexcept>
#include <algorithm>

class StructADSU : public DisjointSet {
public:
    explicit StructADSU(int n);
    ~StructADSU() override = default;

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

#endif // STRUCTA_DSU_HPP