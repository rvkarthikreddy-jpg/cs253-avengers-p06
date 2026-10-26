#ifndef DISJOINT_SET_HPP
#define DISJOINT_SET_HPP

#include <cstddef>

/**
 * @brief Common Abstract Base Class for Disjoint Set Union (DSU / Union-Find).
 * 
 * Defines standard operations and queries required for Project 06:
 * Percolation with Union-Find. Structure A and Structure B (all 3 ablations)
 * implement this exact interface without altering public method signatures.
 */
class DisjointSet {
public:
    virtual ~DisjointSet() = default;

    /**
     * @brief Finds the canonical representative (root) of the set containing element x.
     * @param x Element index in the range [0, size() - 1].
     * @return Canonical root representative in the range [0, size() - 1].
     */
    virtual int find(int x) = 0;

    /**
     * @brief Merges the disjoint sets containing elements x and y.
     * @param x First element in the range [0, size() - 1].
     * @param y Second element in the range [0, size() - 1].
     * @return true if x and y belonged to different sets and were merged; false if already connected.
     */
    virtual bool unite(int x, int y) = 0;

    /**
     * @brief Evaluates whether elements x and y belong to the same equivalence class.
     * @param x First element.
     * @param y Second element.
     * @return true if find(x) == find(y), false otherwise.
     */
    virtual bool connected(int x, int y) = 0;

    /**
     * @brief Returns the cardinality of the component containing element x.
     * @param x Element index in the range [0, size() - 1].
     * @return Number of elements in the component containing x.
     */
    virtual int component_size(int x) = 0;

    /**
     * @brief Retrieves the size of the largest connected component across the entire universe.
     * @return Cardinality of the maximal component in O(1) time.
     */
    virtual int largest_component_size() const = 0;

    /**
     * @brief Returns the total capacity (universe size) of the disjoint set.
     * @return Total elements N managed by the instance.
     */
    virtual int size() const = 0;

    /**
     * @brief Re-initializes the instance to N singleton sets.
     */
    virtual void reset() = 0;
};

#endif // DISJOINT_SET_HPP
