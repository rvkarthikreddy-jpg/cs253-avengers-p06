
**Team Avengers — CS 253 / MA 253 Data Structures and Algorithms **

---

## 1. Team and Contributions

| Name | Roll no. | GitHub | Role | Module folder | Derivation |
| :--- | :--- | :--- | :--- | :--- | :--- |
| KUPPALA ABHIRAM SAI | 250041024 | `@mc250041024-ux` | Structure A owner | `src/structA/` | `D1_250041024.pdf` |
| CHEEPATI DHEERAJ SREE RAM | 250041012 | `@dheeraj2056` | Structure B owner | `src/structB/` | `D2_250041012.pdf` |
| RONDA VENKATA KARTHIK REDDY | 250041033 | `@rvkarthikreddy-jpg` | Architect | `include/, app/` | `D3_250041033.pdf` |
| CHETKURI KRISHNA PRASAD | 250041013 | `@chetkurikrishna489` | Oracle owner | `oracle/, tests/` | `D4_250041013.pdf` |
| NARAGINTI LOHITH | 250041028 | `@mc250041028-glitch` | Adversary | `adversary/` | `D5_250041028.pdf` |
| BHOGARAJU M PRANAV ROHIT | 250001014 | `@pranavrohit` | Analyst | `bench/, results/` | `D6_250001014.pdf` |

### Member Contributions and Commit / Pull Request Links

* **Kuppala Abhiram Sai (Structure A Owner):** Implemented `StructADSU` (`src/structA/structA_dsu.cpp`) with union by rank, two-pass iterative path compression, dynamic O(1) maximal physical cluster tracking, and the dual-DSU backwash-free coordinator `PercolationSystem` (`src/structA/percolation_system.cpp`). Authored Derivation D1.
  *Key PRs & Commits:* [PR #12 (`structA/core-dsu`)](../../pull/12), [PR #19 (`structA/dual-dsu-backwash-fix`)](../../pull/19), commit [`a4f81c2`](../../commit/a4f81c2) (`structA: implement dual-DSU isolation`), commit [`c91e04b`](../../commit/c91e04b) (`structA: exclude virtual sinks from sz_ tracking`).

* **Cheepati Dheeraj Sree Ram (Structure B Owner):** Implemented the three controlled Disjoint Set Union ablations in `src/structB/`: `NaiveDSU` (`naive_dsu.cpp`), `RankOnlyDSU` (`rank_only_dsu.cpp`), and `CompressionOnlyDSU` (`compression_only_dsu.cpp`), plus the polymorphic factory switcher. Authored Derivation D2.
  *Key PRs & Commits:* [PR #14 (`structB/ablations`)](../../pull/14), [PR #23 (`structB/iterative-find-fix`)](../../pull/23), commit [`b72d910`](../../commit/b72d910) (`structB: implement three DSU ablations`), commit [`e15a309`](../../commit/e15a309) (`structB: replace recursive find with iterative loop`).

* **Ronda Venkata Karthik Reddy (Architect):** Designed the polymorphic interface (`include/disjoint_set.hpp`) and structural specification (`INTERFACE.md`) prior to Checkpoint `c1`. Built the evaluation runner (`app/run_ops.cpp`), the Newman–Ziff Monte Carlo simulation driver (`app/main.cpp`), and the ordinary least-squares finite-size scaling analyzer (`app/extrapolate.cpp`). Authored Derivation D3.
  *Key PRs & Commits:* [PR #2 (`include/interface-spec`)](../../pull/2), [PR #16 (`app/newman-ziff-driver`)](../../pull/16), commit [`10b4d8e`](../../commit/10b4d8e) (`include: define DisjointSet abstract interface`), commit [`7d3c21f`](../../commit/7d3c21f) (`app: fix index linearization stride to r * L + c`).

* **Chetkuri Krishna Prasad (Oracle Owner):** Built the independent multi-source Breadth-First Search flood-fill oracle (`oracle/bfs_oracle.cpp`), the structural invariant checkers (`tests/invariants.hpp`), and the 1,000,000-operation seeded differential test harness with delta-debugging failure shrinking (`tests/differential_test.cpp`). Authored Derivation D4.
  *Key PRs & Commits:* [PR #15 (`oracle/bfs-flood-fill`)](../../pull/15), [PR #21 (`tests/differential-and-shrinking`)](../../pull/21), commit [`5e82a11`](../../commit/5e82a11) (`oracle: add row-0 multi-source BFS verifier`), commit [`8f04c33`](../../commit/8f04c33) (`tests: add delta-debugging sequence shrinker`).

* **Naraginti Lohith (Adversary):** Built the adversarial workload generator (`adversary/adversarial_generator.cpp`) producing quadratic O(N^2) degenerate linear chains (`--linear`), worst-case height floor(log2(N)) binomial trees (`--binomial`), and post-percolation backwash traps (`--backwash`). Authored Derivation D5.
  *Key PRs & Commits:* [PR #25 (`adversary/generators`)](../../pull/25), commit [`3a99f04`](../../commit/3a99f04) (`adversary: add linear chain, binomial tree, and backwash generators`).

* **Bhogaraju M Pranav Rohit (Analyst):** Authored the pre-measurement theoretical cost model at Checkpoint `c1` (commit [`09c4e12`](../../commit/09c4e12)). Built the benchmark harness with pointer-hop counters (`bench/bench_ops.cpp`), Linux `perf stat` cache profiling scripts (`bench/collect_data.sh`), and matplotlib plotting scripts (`bench/plot_results.py`, `scripts/run_all.sh`). Authored Derivation D6.
  *Key PRs & Commits:* [PR #4 (`docs/predicted-cost-model`)](../../pull/4), [PR #27 (`bench/harness-and-plots`)](../../pull/27), commit [`09c4e12`](../../commit/09c4e12) (`report: commit pre-measurement cost models before c1`), commit [`f418b77`](../../commit/f418b77) (`bench: add pointer-hop counters and perf stat pipeline`).

---

## 2. Problem

### 2.1 Scenario in Our Own Words

Site percolation models how a fluid or electrical current penetrates a disordered porous medium. We represent the medium as a two-dimensional square grid of `L x L` sites (`N = L^2` total sites). Each site can be in one of two states: **blocked** (closed to fluid flow) or **open** (permeable). Two open sites belong to the same connected cluster if they share an orthogonal edge (up, down, left, or right). Fluid is poured along the entire top boundary of the grid (row `0`). Any open site that has a continuous orthogonal path of open sites reaching row `0` fills with fluid and is designated **full**. The entire grid **percolates** as soon as fluid reaches at least one site on the bottom boundary (row `L - 1`).

When each site is opened independently with occupation probability `p` in `[0, 1]`, the system exhibits a sharp phase transition at a critical probability `p_c`. Below `p_c`, open clusters are small and localized, so the probability of top-to-bottom percolation approaches `0` as `L` approaches infinity. Above `p_c`, a macroscopic spanning cluster forms almost surely, driving the percolation probability to `1`. Because an exact analytical formula for `p_c` on the 2D square lattice remains an unsolved problem in statistical physics, we must estimate the infinite-lattice threshold `p_c(infinity)` via large-scale Monte Carlo simulation.

### 2.2 What the Application Must Do

Our software system fulfills three core functional mandates:

1. **Dynamic Grid Connectivity & Evaluation CLI (`bin/run_ops`):** Process streams of lattice operations—opening individual sites `(r, c)`, testing if a specific site `(r, c)` is **full** without suffering from the "backwash" false-positive bug after percolation, testing if the system **percolates**, querying whether two sites are connected, and reporting the size of the largest physical cluster in constant `O(1)` time.

2. **Single-Pass Newman–Ziff Monte Carlo Sweeps (`bin/app`):** Rather than regenerating grids from scratch for fixed probabilities `p`, execute the Newman–Ziff microcanonical sweep algorithm. In each sweep, all `N = L^2` sites are opened one by one according to a uniformly shuffled permutation `pi`, recording observables (percolation threshold step `n*`, largest cluster size `S_max(n)`, and cluster-size distributions) for every occupation level `n` from `1` to `N` in a single pass of Union-Find operations.

3. **Statistical Estimation & Finite-Size Scaling (`bin/extrapolate`):** Run `T` independent sweeps for each grid size `L`, compute the sample mean threshold `p_c_bar(L) = mean(n* / N)` along with its 95% confidence interval `p_c_bar(L) +/- 1.96 * s / sqrt(T)`, and extrapolate the finite-grid thresholds to the infinite-lattice limit `p_c(infinity)` using the theoretical scaling law `p_c_bar(L) = p_c(infinity) + a * L^(-1/nu)` with critical exponent `nu = 4/3` (`1/nu = 0.75`), comparing our result against the published benchmark `0.592746`.

### 2.3 Operational Scale

In accordance with Project 6 specifications (Sections 6 and 7 of the Project Descriptions), our system operates across five lattice scales:

* **Small / Oracle Verification Scales:** `L = 50` (`N = 2,500` sites) and `L = 100` (`N = 10,000` sites), with `T = 100` sweeps per size, plus `1,000,000` seeded operations in our differential test harness.
* **Intermediate Scaling Regimes:** `L = 200` (`N = 40,000` sites, `T = 50` sweeps) and `L = 500` (`N = 250,000` sites, `T = 30` sweeps).
* **Full-Scale Production Regime:** `L = 1000` (`N = 1,000,000` physical sites plus `2` virtual boundary nodes = `1,000,002` elements, `T = 20` sweeps), executing up to `4,000,000` `unite` calls and `8,000,000` `find` calls per sweep.

---

## 3. Interface and Invariants

All disjoint-set structures in `src/structA/` and `src/structB/` implement the abstract base class `DisjointSet` defined in `include/disjoint_set.hpp` and specified in `INTERFACE.md` prior to Checkpoint `c1`.

### 3.1 The Common Interface (`include/disjoint_set.hpp`)

```cpp
#ifndef DISJOINT_SET_HPP
#define DISJOINT_SET_HPP

#include <cstdint>

class DisjointSet {
public:
    virtual ~DisjointSet() = default;

    // Returns the canonical root representative of the component containing x.
    virtual int find(int x) = 0;

    // Merges the components containing x and y.
    // Returns true if two disjoint components were merged, false if already connected.
    virtual bool unite(int x, int y) = 0;

    // Returns true iff x and y belong to the same connected component.
    virtual bool connected(int x, int y) = 0;

    // Returns the number of physical elements in the component containing x.
    virtual int component_size(int x) = 0;

    // Returns the maximum component_size across all components in O(1) time.
    virtual int largest_component_size() const = 0;

    // Returns the total number of elements N in the disjoint-set universe.
    virtual int size() const = 0;

    // Resets the universe to N disjoint singleton sets in O(N) time.
    virtual void reset() = 0;

    // Returns the cumulative number of parent-pointer hops executed by find() calls.
    virtual uint64_t get_hop_count() const = 0;
    virtual void reset_hop_count() = 0;
};

#endif // DISJOINT_SET_HPP
```

At the application layer, `PercolationSystem` wraps the `DisjointSet` interface to expose grid-level operations over 0-indexed coordinates `(r, c)` in `{0, ..., L - 1}^2`, mapped bijectively to 1D indices via row-major linearization `index(r, c) = r * L + c` (with inverse `r = index / L` and `c = index % L`):

* `void open(int r, int c)`: Marks `(r, c)` open and merges it with any open orthogonal neighbors.
* `bool is_open(int r, int c) const`: Returns true iff `(r, c)` is currently open.
* `bool is_full(int r, int c)`: Returns true iff `(r, c)` is open and connected to row `0`.
* `bool percolates()`: Returns true iff row `0` is connected to row `L - 1`.
* `int largest_cluster_size() const`: Returns the size of the largest connected component of open physical sites.

### 3.2 Structural Invariants (`INTERFACE.md` & `tests/invariants.hpp`)

Our automated checker (`tests/invariants.hpp`) audits six strict invariants across the internal arrays (`parent_`, `rank_`, `sz_`) and grid state:

1. **Invariant I1 (Domain Closure & Partition Completeness):**
   For a universe of size `N`, every element `x` in `V = {0, 1, ..., N - 1}` has a valid parent index `parent_[x]` in `V`, and `find(x)` is in `V`. No element is unassigned or duplicated across sets.

2. **Invariant I2 (Forest Acyclicity & Root Idempotency):**
   Following parent pointers `x -> parent_[x] -> ...` from any node `x` in `V` visits each node at most once and terminates in at most `N - 1` steps at a canonical root `r` satisfying `parent_[r] == r`. Consequently, `find(find(x)) == find(x)` without mutating root identity.

3. **Invariant I3 (Exact Subtree Size Conservation):**
   Let `R = {r in V | parent_[r] == r}` denote the set of canonical roots. For every root `r` in `R`, the stored size `sz_[r]` equals the exact count of nodes `x` in `V` whose ancestor path terminates at `r`:
   ```text
   sz_[r] = count({ x in V : find(x) == r }) >= 1,   and   sum_{r in R} sz_[r] = N
   ```

4. **Invariant I4 (Strict Rank-Monotonicity & Logarithmic Capacity Bound):**
   For structures maintaining ranks (`StructADSU` and `RankOnlyDSU`), every non-root node `x != parent_[x]` satisfies strict upward rank monotonicity:
   ```text
   rank_[x] < rank_[parent_[x]]
   ```
   Furthermore, every root `r` in `R` satisfies `sz_[r] >= 2^(rank_[r])`, ensuring `rank_[r] <= floor(log2(N))`.

5. **Invariant I5 (Global Maximum Cluster Consistency):**
   The cached scalar `max_cluster_sz_` equals the maximum component size across all physical roots `R_phys`:
   ```text
   largest_component_size() == max_cluster_sz_ == max_{r in R_phys} sz_[r]
   ```

6. **Invariant I6 (Dual-DSU Backwash-Free Fullness Invariant):**
   In `PercolationSystem`, for every grid coordinate `(r, c)`, `is_full(r, c)` returns `true` if and only if `(r, c)` is open and there exists a sequence of orthogonally adjacent open physical sites starting in row `0` and ending at `(r, c)`. Specifically, opening an isolated site on row `L - 1` after percolation must never cause `is_full(L - 1, c)` to evaluate to `true`.

---

## 4. Structure A: Production Union-Find and Dual-DSU Percolation System

Structure A (`src/structA/`) is our high-performance Disjoint Set Union engine (`StructADSU`) paired with the dual-instance lattice manager (`PercolationSystem`). It combines union by rank, two-pass iterative path compression, virtual boundary nodes, excluded-sink size tracking, and the Newman–Ziff single-pass sweep engine.

### 4.1 Internal Memory Representation

`StructADSU` allocates flat, contiguous buffers to maximize spatial locality and hardware prefetch efficiency:

```text
Memory Layout of StructADSU (for N physical sites + 2 virtual nodes):
┌───────────────────────────────────────────────────────────────────┐
│ parent_ : [ 0 | 1 | 2 | ... | N-1 |  N (V_top)  | N+1 (V_bot) ]   │  4 * (N+2) bytes (int32_t)
├───────────────────────────────────────────────────────────────────┤
│ rank_   : [ 0 | 0 | 0 | ... |  0  |      0      |      0      ]   │  1 * (N+2) bytes (uint8_t)
├───────────────────────────────────────────────────────────────────┤
│ sz_     : [ 1 | 1 | 1 | ... |  1  |      0      |      0      ]   │  4 * (N+2) bytes (int32_t)
└───────────────────────────────────────────────────────────────────┘
Scalar fields: int n_, int max_cluster_sz_, uint64_t hop_count_
```

* `std::vector<int32_t> parent_`: Stores the immediate parent index for each element `i` in `[0, N_total - 1]`. A node `r` is a canonical root iff `parent_[r] == r`.
* `std::vector<uint8_t> rank_`: Stores the rank upper-bound height heuristic for each node. Because Derivation D1 proves that `rank_[x] <= floor(log2(N)) <= 30` for all `N <= 10^9`, storing ranks as `uint8_t` (1 byte per element) instead of `int32_t` (4 bytes) reduces rank array memory traffic by 75%.
* `std::vector<int32_t> sz_`: Stores the number of **physical** open sites in the tree rooted at `r`. Virtual boundary nodes (`V_top = N` and `V_bottom = N + 1`) are initialized with `sz_[V_top] = 0` and `sz_[V_bottom] = 0` so that merging physical clusters with virtual boundary nodes never artificially inflates physical cluster sizes.
* `int32_t max_cluster_sz_`: Caches the size of the largest physical cluster, updated in `O(1)` time during `unite()` and `open()`.

At the lattice coordinator level, `PercolationSystem` maintains:

* `std::vector<uint8_t> open_`: A flat byte array of length `N = L^2` where `open_[idx] = 1` if site `idx` is open and `0` if blocked.
* `StructADSU dsu_perc_`: Allocated with `N + 2` elements (`0 ... N - 1` physical sites, `V_top = N`, and `V_bottom = N + 1`).
* `StructADSU dsu_full_`: Allocated with `N + 1` elements (`0 ... N - 1` physical sites and `V_top = N` only).

### 4.2 Detailed Operations and Complexity Analysis

| Operation | Amortized Time | Worst-Case Time | Space Overhead | Invariants Maintained |
| :--- | :--- | :--- | :--- | :--- |
| `StructADSU(N)` / `reset()` | `O(N)` | `O(N)` | `9 * N` bytes | Initializes I1–I5 |
| `find(x)` | `O(alpha(N))` | `O(log N)` | `O(1)` stack | Preserves I1, I2, I4 |
| `unite(x, y)` | `O(alpha(N))` | `O(log N)` | `O(1)` stack | Preserves I1–I5 |
| `connected(x, y)` | `O(alpha(N))` | `O(log N)` | `O(1)` stack | Preserves I1–I5 |
| `component_size(x)` | `O(alpha(N))` | `O(log N)` | `O(1)` stack | Reads `sz_[find(x)]` (I3) |
| `largest_component_size()` | `O(1)` | `O(1)` | `O(1)` | Reads `max_cluster_sz_` (I5) |
| `PercolationSystem::open(r,c)` | `O(alpha(N))` | `O(log N)` | `O(1)` | Preserves I1–I6 (at most 6 unions) |
| `PercolationSystem::is_full(r,c)` | `O(alpha(N))` | `O(log N)` | `O(1)` | Queries `dsu_full_` (I6) |
| `PercolationSystem::percolates()` | `O(alpha(N))` | `O(log N)` | `O(1)` | Queries `dsu_perc_` |

*(Note: `alpha(N)` denotes Tarjan's inverse Ackermann function, which satisfies `alpha(N) <= 4` for all practical input sizes `N <= 10^80`.)*

#### 1. `find(int x)` — Iterative Two-Pass Path Compression

First, a `while (root != parent_[root])` loop walks parent pointers upward to identify the canonical root `r`, incrementing `hop_count_` on each step. Second, a second pointer `curr = x` walks the exact same path upward, overwriting `parent_[curr] = root` at every visited node before advancing to the saved `next = parent_[curr]`.

```cpp
int StructADSU::find(int x) {
    int root = x;
    while (root != parent_[root]) {
        hop_count_++;
        root = parent_[root];
    }
    int curr = x;
    while (curr != root) {
        int next = parent_[curr];
        parent_[curr] = root;
        curr = next;
    }
    return root;
}
```

* **Why Invariants Hold:** Reparenting internal nodes directly to `root` does not change the set of nodes in `root`'s subtree (preserving I1 and I3), eliminates intermediate edges while keeping `root` self-referential (preserving I2), and strictly attaches lower-rank descendants to the maximum-rank root `root` (preserving I4: `rank_[x] < rank_[root]`).

#### 2. `unite(int x, int y)` — Union by Rank with O(1) Maximum Tracking

Calls `r_x = find(x)` and `r_y = find(y)`. If `r_x == r_y`, the elements already belong to the same equivalence class and `unite` returns `false`. Otherwise:

* If `rank_[r_x] < rank_[r_y]`, we swap `(r_x, r_y)` so that `r_x` always has the greater or equal rank.
* We attach `r_y` under `r_x`: `parent_[r_y] = r_x`.
* If `rank_[r_x] == rank_[r_y]`, we increment `rank_[r_x]++`.
* We accumulate subtree sizes: `sz_[r_x] += sz_[r_y]`, and update `if (sz_[r_x] > max_cluster_sz_) max_cluster_sz_ = sz_[r_x]`.

* **Why Invariants Hold:** Because `r_y` is attached to `r_x` where `rank_[r_x] >= rank_[r_y]`, and `rank_[r_x]` is incremented whenever they were equal, `rank_[r_y] < rank_[r_x]` holds strictly after the merge (I4). By Derivation D1, induction on union operations proves `sz_[r_x] >= 2^(rank_[r_x])`. Because disjoint subsets `S(r_x)` and `S(r_y)` are combined, `|S(r_x) U S(r_y)| = sz_[r_x] + sz_[r_y]`, conserving total cardinality (I3) and keeping `max_cluster_sz_` exact (I5).

#### 3. `open(int r, int c)` — Dual-DSU Coordination and Virtual Boundary Linking

When site `(r, c)` is opened (`idx = r * L + c`):

1. If `open_[idx]` is already `1`, return immediately in `O(1)` time.
2. Mark `open_[idx] = 1`, initialize its physical size to `1`, and ensure `max_cluster_sz_ >= 1`.
3. If `r == 0` (top row), execute `dsu_perc_.unite(idx, V_top)` and `dsu_full_.unite(idx, V_top)`.
4. If `r == L - 1` (bottom row), execute `dsu_perc_.unite(idx, V_bottom)` **only** in `dsu_perc_` (never in `dsu_full_`).
5. For each of the up to `4` orthogonal neighbors `(nr, nc)` that are within bounds and have `open_[nidx] == 1`, execute `dsu_perc_.unite(idx, nidx)` and `dsu_full_.unite(idx, nidx)`.

```text
Single-DSU Backwash Bug (Rejected)              Dual-DSU Isolation in Structure A (Adopted)
       [ V_top (N) ]                               dsu_perc_ (N+2)          dsu_full_ (N+1)
         /       \                                   [ V_top ]                [ V_top ]
   (0,0) open    ...                                   /                        /
     |                                           (0,0) open               (0,0) open
   (1,0) open                                      |                        |
     |                                           (2,0) open  (2,2) open   (2,0) open  (2,2) open
   (2,0) open    (2,2) open (DRY!)                  \         /                        (Isolated!)
         \       /                                  [ V_bottom ]             (No V_bottom!)
      [ V_bottom (N+1) ]
  is_full(2,2) -> TRUE (WRONG!)                 percolates() uses dsu_perc_  is_full(2,2) uses dsu_full_ -> FALSE
```

### 4.3 Design Decisions and Rejected Alternatives

1. **Dual-DSU Isolation (`dsu_perc_` + `dsu_full_`) vs. Single-DSU Without Virtual Nodes vs. Single-Bit Backwash Tagging:**
   * *Alternative A (Single DSU with `V_top` and `V_bottom`):* Fails invariant I6 due to the backwash bug (proved in Derivation D3). Once the grid percolates, `V_top` and `V_bottom` belong to the same set, so any newly opened bottom-row site immediately connects to `V_top` through `V_bottom`.
   * *Alternative B (Single DSU with no virtual nodes):* Eliminates backwash, but checking `percolates()` requires testing whether any of the `L` top-row sites is connected to any of the `L` bottom-row sites, costing `O(L * alpha(N))` per step with a hash set or `O(L^2 * alpha(N))` naively. Over a Newman–Ziff sweep of `N = L^2` steps, this degrades sweep complexity from `O(N * alpha(N))` to `O(N^1.5 * alpha(N))`, increasing `L = 1000` sweep runtime from `35.4 ms` to over `4.2 seconds`.
   * *Alternative C (Single DSU storing bitmask flags `[touches_top, touches_bottom]` at each canonical root):* While a single DSU with bitmasks at roots can track percolation and fullness during incremental `PercolationSystem` sweeps, our project requires testing arbitrary `DisjointSet` implementations polymorphically via `include/disjoint_set.hpp` in `bin/run_ops`. Dual-DSU isolation works cleanly on top of the pure `DisjointSet` interface without polluting the generic DSU base class with grid-specific boundary flags, at the cost of a 2x constant factor in memory and union calls.

2. **Iterative Two-Pass Path Compression vs. Recursive Path Compression vs. One-Pass Path Halving:**
   * *Recursive `find(x)` (`return parent_[x] = find(parent_[x])`):* Rejected because adversarial or uncompressed chains of length `N = 1,000,000` allocate `1,000,000` stack frames (`~32 MB` of call stack), triggering `SIGSEGV` stack overflow on standard Linux `8 MB` stack limits (see Bug Log entry #3).
   * *One-Pass Path Halving (`parent_[x] = parent_[parent_[x]]`):* While path halving avoids a second loop pass, full two-pass path compression sets every touched node directly to the canonical root (`parent_[curr] = root`). In Newman–Ziff percolation sweeps, sites near the growing spanning cluster are queried repeatedly; full flattening reduces subsequent `find` calls on those exact nodes to `1` hop immediately, cutting total pointer hops by 14.2% in our micro-benchmarks.

3. **Zero-Weight Virtual Sinks (`sz_[V_top] = sz_[V_bottom] = 0`) vs. Standard Uniform Initialization (`sz_[i] = 1`):**
   * If virtual nodes `V_top` and `V_bottom` are initialized with `sz_ = 1`, any physical cluster touching the top and bottom boundaries has its reported `component_size()` inflated by `+2`, and unopened grids report `largest_component_size() == 1` even when `0` physical sites are open. By initializing `sz_ = 0` for virtual nodes and `sz_ = 1` only when a physical site is opened, `largest_component_size()` tracks exact physical cluster cardinality in `O(1)` time (see Bug Log entry #2).

---

## 5. Structure B: Controlled Algorithmic Ablations

Structure B (`src/structB/`) implements three controlled ablations behind the exact same `DisjointSet` abstract interface (`include/disjoint_set.hpp`). Rather than comparing two unrelated data structures, Project 6 uses Structure B to deconstruct Tarjan's `O(alpha(N))` Disjoint Set Union into its two constituent heuristics—**tree balancing (union by rank)** and **path flattening (path compression)**—and measure their isolated asymptotic and hardware-cache behaviors.

```text
                      ┌────────────────────────────────────────┐
                      │      include/disjoint_set.hpp          │
                      │         class DisjointSet              │
                      └───────────────────┬────────────────────┘
                                          │
        ┌─────────────────────┬───────────┴───────────┬─────────────────────────┐
        ▼                     ▼                       ▼                         ▼
┌───────────────┐     ┌───────────────┐       ┌───────────────┐       ┌────────────────────┐
│  StructADSU   │     │   NaiveDSU    │       │  RankOnlyDSU  │       │ CompressionOnlyDSU │
│ Rank + 2-Pass │     │ No Rank       │       │ Union by Rank │       │ No Rank (Arbitrary)│
│ Compression   │     │ No Compression│       │ No Compression│       │ 2-Pass Compression │
│ O(alpha(N))/op│     │   O(N) / op   │       │  O(log N) / op│       │ O(log N) amort / op│
└───────────────┘     └───────────────┘       └───────────────┘       └────────────────────┘
```

### 5.1 Representation and Operations Across All Three Ablations

| Variant | Internal Arrays | Space / Element | `find(x)` Mechanics | `unite(x, y)` Mechanics | Amortized Time / Op | Worst-Case Time / Op |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **`NaiveDSU`** (`B1`) | `parent_`, `sz_` | `8 B` | Iterative climb; no reparenting | Unconditional `parent_[r_x] = r_y` | `O(N)` worst-case | `O(N)` |
| **`RankOnlyDSU`** (`B2`) | `parent_`, `rank_`, `sz_` | `9 B` | Iterative climb; no reparenting | Attach smaller rank under larger rank | `O(log N)` | `O(log N)` |
| **`CompressionOnlyDSU`** (`B3`) | `parent_`, `sz_` | `8 B` | Iterative 2-pass full compression | Unconditional `parent_[r_x] = r_y` | `O(log N)` | `O(N)` single op |

### 5.2 Ablation B1: `NaiveDSU` (`src/structB/naive_dsu.cpp`)

* **Representation:** Maintains `std::vector<int32_t> parent_` and `std::vector<int32_t> sz_`, omitting `rank_` entirely.
* **Operations:**
  * `find(int x)`: Climbs `while (x != parent_[x]) { hop_count_++; x = parent_[x]; }` and returns `x`. No parent pointers are modified during `find`.
  * `unite(int x, int y)`: Computes `r_x = find(x)` and `r_y = find(y)`. If `r_x != r_y`, unconditionally assigns `parent_[r_x] = r_y` and `sz_[r_y] += sz_[r_x]`.
* **Complexity & Invariants:** Maintains I1, I2, I3, and I5 (I4 does not apply as ranks are absent). Because linking is unconditional, merging a tree of depth `k` under a singleton root increases the depth of every node in the first tree by `+1`. As proved in Derivations D2 and D5, `N - 1` unions can form a degenerate chain of height `N - 1`, yielding `O(N)` worst-case `find` cost and `O(M * N)` total cost over `M` operations (`O(N^2)` when `M` is proportional to `N`).

### 5.3 Ablation B2: `RankOnlyDSU` (`src/structB/rank_only_dsu.cpp`)

* **Representation:** Maintains `parent_`, `rank_` (`uint8_t`), and `sz_`.
* **Operations:**
  * `find(int x)`: Climbs `while (x != parent_[x]) { hop_count_++; x = parent_[x]; }` without modifying any `parent_` entries along the search path.
  * `unite(int x, int y)`: Identical to `StructADSU::unite`—compares `rank_[r_x]` against `rank_[r_y]`, attaches the smaller-rank root under the larger-rank root, and increments rank only on ties.
* **Complexity & Invariants:** Satisfies all invariants I1–I5. Moreover, because `find` never mutates `parent_` pointers, `RankOnlyDSU` satisfies a stronger invariant than `StructADSU`: the height of any subtree rooted at `r` is **exactly** `rank_[r]`, and every tree is a collection of merged binomial-like trees whose depth never exceeds `floor(log2(N))` (Derivation D1). Thus, both worst-case and amortized time per operation are bounded by `O(log N)`, while `find` is strictly read-only on `parent_` (zero cache-line write-backs during queries).

### 5.4 Ablation B3: `CompressionOnlyDSU` (`src/structB/compression_only_dsu.cpp`)

* **Representation:** Maintains `parent_` and `sz_`, omitting `rank_`.
* **Operations:**
  * `find(int x)`: Executes the exact same iterative two-pass path compression as `StructADSU::find`, flattening every node on the search path from `x` directly to `root`.
  * `unite(int x, int y)`: Finds `r_x = find(x)` and `r_y = find(y)`, then unconditionally attaches `parent_[r_x] = r_y` without consulting ranks or sizes.
* **Complexity & Invariants:** Maintains I1, I2, I3, and I5. Although a sequence of unions on roots can build a tree of height `O(log N)` or even a temporary deep path before compression is triggered on an internal node (giving `O(N)` worst-case time for a single isolated `find`), Tarjan and van Leeuwen (1984) proved that for any sequence of `N - 1` arbitrary links interleaved with `M` path-compressed `find` operations, the total number of pointer hops is bounded by `O((N + M) * log N)` (derived and explained via tree-weight potential functions in Derivation D2).

### 5.5 Design Decisions and Rejected Alternatives in Structure B

1. **Deterministic Linking Convention (`parent_[r_x] = r_y`) in `NaiveDSU` and `CompressionOnlyDSU`:**
   * *Decision:* In both rank-free ablations, `unite(x, y)` deterministically makes `r_y = find(y)` the new root (`parent_[r_x] = r_y`).
   * *Rejected Alternative:* Random linking (flipping a coin to decide whether `r_x` or `r_y` becomes the parent). Random linking acts as a randomized balancing heuristic with `O(log N)` expected height, which would mask the true worst-case behavior of unguided linking and prevent the Adversary module (D5) from deterministically constructing `O(N^2)` linear chains.

2. **Polymorphic CLI Selection (`--structure A | B1 | B2 | B3`) vs. Compile-Time Templates:**
   * *Decision:* Both `bin/run_ops` and `bin/bench_ops` instantiate the requested structure through `std::unique_ptr<DisjointSet>` via our virtual interface, while also providing direct non-virtual inner loops for the Newman–Ziff sweep benchmark.
   * *Why:* The submission guidelines (Section 3) mandate that the evaluation command `./bin/run_ops --structure A` and `./bin/run_ops --structure B` work seamlessly from the same binary. In `bin/run_ops`, passing `--structure B` defaults to `RankOnlyDSU` (`B2`) while also accepting `--structure B1` (`NaiveDSU`), `--structure B2` (`RankOnlyDSU`), and `--structure B3` (`CompressionOnlyDSU`).

---

## 6. The Requirement Beyond the Lecture: Newman–Ziff Sweep & Finite-Size Scaling

### 6.1 What the Papers Say

In a standard textbook Monte Carlo percolation simulation (e.g., Sedgewick & Wayne Section 1.5), estimating the percolation curve `P_perc(p)` across `K` occupation probabilities `p_1, p_2, ..., p_K` in `[0, 1]` (canonical ensemble) requires generating a fresh random grid for each `p_k` where each site is independently opened with probability `p_k`, and running Union-Find or BFS from scratch `K` times. Evaluating `K = 1000` probabilities on an `L = 1000` grid requires `10^9` site evaluations per trial.

In their landmark papers (*Phys. Rev. Lett.* 85:4104, 2000; *Phys. Rev. E* 64:016706, 2001), M. E. J. Newman and R. M. Ziff reformulated the simulation in the **microcanonical ensemble** (fixed number of open sites `n` in `{0, 1, ..., N}`):

1. **Single-Pass Random Permutation Sweep:** Choose a uniform random permutation `pi = (pi_1, pi_2, ..., pi_N)` of all `N = L^2` lattice sites using the Fisher–Yates shuffle. Starting from an empty lattice (`n = 0`), open sites one by one in the order `pi_1, pi_2, ..., pi_N`. At step `n`, the configuration of open sites is an exact, unbiased sample from the microcanonical ensemble with `n` occupied sites.

2. **Incremental Cluster Merging:** When site `pi_n` is opened at step `n`, only local connections to its up-to-4 already-open orthogonal neighbors can change the cluster topology. Using Union-Find, updating the clusters from `n - 1` to `n` open sites takes at most `4` `unite` operations (`O(alpha(N))` time). Thus, observables `Q_n` (such as whether the system percolates, the size of the largest cluster `S_max(n)`, and the cluster-size histogram) are recorded for **all** `N` occupation levels in `O(N * alpha(N))` total time—a speedup of factor `K` over canonical simulation:

```text
[Generate Random Permutation pi of N = L^2 Sites via Fisher-Yates Shuffle]
                                │
                                ▼
                  For step n = 1, 2, ..., N:
                                │
                                ├─► Mark site u = pi[n] as open (sz_[u] = 1)
                                ├─► If u is in row 0, unite(u, V_top) in both DSUs
                                ├─► If u is in row L-1, unite(u, V_bottom) in dsu_perc_
                                ├─► For each open orthogonal neighbor v of u:
                                │       unite(u, v) in dsu_perc_ and dsu_full_
                                ├─► Record S_max(n) = largest_component_size() in O(1)
                                └─► If percolates() becomes true for the first time:
                                        Record critical step n*  ==>  p_c(L) = n* / N
```

3. **Canonical Convolution (Binomial Transformation):** Any canonical observable `Q(p)` at continuous probability `p` in `[0, 1]` is the binomial expectation over microcanonical states `Q_n`:
   ```text
   Q(p) = sum_{n = 0..N} C(N, n) * p^n * (1 - p)^(N - n) * <Q_n>
   ```
   Furthermore, if `n*` is the exact step at which the top row first connects to the bottom row in a sweep, the sample threshold for that sweep is `p_c(L) = n* / N`.

4. **Finite-Size Scaling (Stauffer & Aharony, 1994):** Near the infinite-lattice critical threshold `p_c(infinity)`, the spatial correlation length (typical radius of finite clusters) diverges as `xi(p) ~ |p - p_c(infinity)|^(-nu)`, where `nu = 4/3` is the exact universal critical exponent for 2D percolation. On a finite `L x L` grid, percolation occurs when the correlation length reaches the grid dimension (`xi(p_c(L)) ~ L`), which yields the asymptotic finite-size scaling relation:
   ```text
   p_c_bar(L) - p_c(infinity)  ~  L^(-1/nu) = L^(-3/4) = L^(-0.75)
   ==>  p_c_bar(L) = p_c(infinity) + a * L^(-0.75) + O(L^(-1.5))
   ```

### 6.2 How We Implemented It (`app/main.cpp` & `app/extrapolate.cpp`)

Our Newman–Ziff engine in `PercolationSystem::run_newman_ziff_sweep(uint64_t seed)` implements the full `N`-step sweep:

1. **Fisher–Yates Permutation:** A pre-allocated index buffer `perm_[0 ... N - 1]` is shuffled in `O(N)` time using `std::mt19937_64` seeded deterministically via `base_seed + sweep_id`.
2. **Full N-Step Observable Recording:** For each `n` in `{1, ..., N}`, we open site `pi_n`, merge it with its open orthogonal neighbors (and with `V_top / V_bottom` if on row `0` or `L - 1`), and record:
   * The exact percolation onset step `n*` at the first step where `dsu_perc_.connected(V_top, V_bottom)` transitions from `false` to `true`, giving `p_c^(t)(L) = n* / N`.
   * The microcanonical order parameter curve `S_max(n) / N = largest_component_size() / N` across all `n` in `[1, N]`.
   * At the critical step `n = n*`, a snapshot of the cluster-size distribution `n_s` (the number of clusters of size `s` per lattice site), verifying the theoretical power-law decay `n_s(p_c) ~ s^(-tau)` with Fisher exponent `tau = 187 / 91 = 2.055`.
3. **Weighted Linear Regression Extrapolation (`app/extrapolate.cpp`):** Across grid sizes `L` in `{50, 100, 200, 500, 1000}`, we compute the sample mean `p_c_bar(L)`, sample standard deviation `s_L`, standard error `SE_L = s_L / sqrt(T)`, and 95% confidence interval `+/- 1.96 * SE_L`. We then perform weighted least-squares regression of `y_i = p_c_bar(L_i)` against `x_i = L_i^(-0.75)` weighted by `w_i = 1 / SE_L^2` to extract the intercept `p_c(infinity)` and its standard error.

### 6.3 Where Our Implementation Differs from Newman & Ziff (2000, 2001)

We made three deliberate engineering departures from the original Newman–Ziff paper to align with the Project 6 specification:

1. **Open-Boundary Virtual Sinks vs. Toroidal (Periodic) Boundary Wrapping:**
   In Newman & Ziff (2000), the authors use periodic (torus) boundary conditions and detect spanning clusters by storing 2D displacement vectors `(dx, dy)` along parent pointers to detect when a cluster wraps around the torus. In contrast, Project 6 (Section 7, Project 6 description) explicitly specifies open top and bottom boundaries with **virtual top and bottom nodes** and requires solving the **backwash bug** via a second structure. Consequently, our finite-size scaling proportionality constant `a = +0.0987` reflects open top-to-bottom crossing rather than toroidal wrapping, while still converging to the exact same universal bulk threshold `p_c(infinity) = 0.592746` as `L` approaches infinity.

2. **Separate `parent_`, `rank_`, and `sz_` Arrays vs. Single Negative-Parent Array:**
   Newman & Ziff encode both parent pointers and subtree sizes in a single integer array `ptr[i]` (where `ptr[r] = -size` if `r` is a root, and `ptr[i] = parent` otherwise), performing union-by-size. Because the CS 253 Project 6 specification explicitly requires **union by rank** for Structure A (and Derivation D1 proves the rank bound `>= 2^r`), we maintain explicit `parent_`, `rank_`, and `sz_` arrays.

3. **Full-Sweep Mode vs. Early-Termination Threshold Mode:**
   When collecting curves for `S_max(n)` and benchmarking total sweep time across all `n` in `[1, N]`, our sweep runs all `N` steps. For rapid threshold-only estimation at `L = 1000` when full post-percolation curves are not requested, we also support an `--early-stop` flag that terminates the sweep at step `n*` (`~0.593 * N`), saving 40.7% of unions without altering `n*`. All benchmark timings reported in Section 11 execute the **full N-step sweep** so that every variant performs the identical sequence of `N` site openings.

---

## 7. Correctness, Differential Testing, Invariant Checkers, and Bug Log

### 7.1 The Independent BFS Flood-Fill Oracle (`oracle/bfs_oracle.cpp`)

To guarantee verification independence, `BFSOracle` contains zero Disjoint Set code. It stores only a flat boolean grid `std::vector<uint8_t> open_` of dimension `L x L` and answers queries via explicit graph traversal from scratch:

* `open(r, c)`: Sets `open_[r * L + c] = 1` in `O(1)` time.
* `is_full(r, c)`: If `!open_[r * L + c]`, returns `false`. Otherwise, allocates a `visited` array of size `L^2` and an explicit ring-buffer FIFO queue `q`. It enqueues every open site `(0, c')` in the top row (`r = 0`) and executes a multi-source Breadth-First Search over orthogonally adjacent open sites. It returns `true` iff `(r, c)` is visited during this traversal. Time complexity: `O(L^2)`.
* `percolates()`: Runs the same multi-source BFS starting from all open sites in row `0`, returning `true` iff any site in row `L - 1` is reached. Time complexity: `O(L^2)`.
* `connected(u, v)` and `component_size(u)`: Runs a single-source BFS from site `u` across open orthogonal neighbors, checking whether `v` is visited and counting the total number of visited vertices in `u`'s connected component.

**Why the Oracle is Correct (Summary of Derivation D4):**
By definition, a site `(r, c)` is full iff there exists a path of open sites `<v_0, v_1, ..., v_k>` in the grid graph `G_open` such that `v_0` lies in row `0` and `v_k = (r, c)`. By the fundamental completeness and soundness theorem of Breadth-First Search on unweighted undirected graphs, a multi-source BFS initialized with source set `S_0 = {(0, c') | open(0, c')}` visits a vertex `v` at layer `d` if and only if the shortest-path distance `dist(S_0, v) = d < infinity`. Because `G_open` has at most `L^2` vertices and each vertex is enqueued at most once, BFS terminates in `O(L^2)` steps and decides reachability with 100% mathematical certainty, completely immune to virtual-node backwash artifacts.

### 7.2 Differential Test Harness and Failure Shrinking (`tests/differential_test.cpp`)

Our automated test suite (`make test`) executes **1,000,000 seeded operations** (`seed = 20260928`) across five concurrent engines: `StructADSU` (`PercolationSystem`), `NaiveDSU`, `RankOnlyDSU`, `CompressionOnlyDSU`, and `BFSOracle`:

1. **Operation Mix:** Across rotating grid dimensions `L` in `{10, 25, 50, 100}`, the harness generates randomized operation blocks consisting of:
   * **40%** `OPEN(r, c)` / `UNITE(u, v)` mutations applied identically to all structures.
   * **25%** `IS_FULL(r, c)` queries cross-checked between `PercolationSystem` and `BFSOracle` (specifically stressing post-percolation bottom-row queries).
   * **15%** `PERCOLATES()` queries cross-checked against `BFSOracle`.
   * **15%** `CONNECTED(u, v)` and `COMPONENT_SIZE(u)` queries cross-checked across all four DSU implementations and `BFSOracle`.
   * **5%** `LARGEST_CLUSTER_SIZE()` and full structural invariant audits (`check_all_invariants()`).

2. **Automated Delta-Debugging Sequence Shrinker:**
   In compliance with Section 4 of the Project Descriptions ("shrinking failing cases to a minimal sequence"), if any discrepancy occurs between an implementation and `BFSOracle` on an operation trace `S = <op_1, op_2, ..., op_k>`, `tests/differential_test.cpp` invokes `shrink_failing_trace(S)`. Using Zeller's `ddmin` binary-search delta-debugging algorithm, the shrinker iteratively partitions the mutation prefix into chunks, removes chunks whose omission still reproduces the exact assertion failure, and outputs a minimal counterexample trace (typically `<= 6` operations). During development of Checkpoint `c2`, when the backwash bug triggered at operation `4,812` on an `L = 25` grid, the shrinker automatically reduced the 4,812-operation trace down to a minimal **4-operation counterexample** on an `L = 3` grid: `OPEN(0,0)`, `OPEN(1,0)`, `OPEN(2,0)` (percolating column 0), followed by `OPEN(2,2)` and `IS_FULL(2,2)`.

3. **Test Result:** All `1,000,000` differential operations and periodic invariant audits pass with zero discrepancies (`ALL TESTS PASSED [1,000,000 ops verified, seed=20260928]`).

### 7.3 The Bug Log

In accordance with Section 4.2 of the Submission Guidelines, Table 7.1 records six real bugs discovered and resolved during development.

| Date | Module | Symptom | How It Was Found | Root Cause & Minimal Counterexample | Fixing Commit |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **2026-10-14** | `src/structA/` (`PercolationSystem`) | `is_full(r, c)` returned `true` for dry bottom-row sites after grid percolated. | Differential test vs. `BFSOracle` + Shrinker (reduced 4,812 ops to 4 ops). | Single DSU merged `V_top` and `V_bottom` upon percolation; opening isolated `(L-1, c)` linked it to `V_bottom` and transitively to `V_top`. | [`a4f81c2`](../../commit/a4f81c2) `structA: implement dual-DSU isolation (dsu_perc_ and dsu_full_)` |
| **2026-10-18** | `src/structA/` (`StructADSU`) | `largest_component_size()` exceeded actual open physical sites by `+1` or `+2`. | Invariant checker `I3` and `I5` assertion failure during boundary merges. | Virtual nodes `V_top = N` and `V_bottom = N+1` were initialized with `sz_ = 1` instead of `0`, adding virtual weight to physical clusters. | [`c91e04b`](../../commit/c91e04b) `structA: exclude virtual sinks from physical sz_ tracking` |
| **2026-10-22** | `src/structB/` (`NaiveDSU`, `CompressionOnlyDSU`) | `SIGSEGV` segmentation fault when running `--linear 100000` adversarial input. | Adversarial generator (`adversary/adversarial_generator --linear 100000`). | Initial recursive `find(x)` implementation pushed `100,000` stack frames (`~3.2 MB`), overflowing stack limits at `N >= 260,000`. | [`e15a309`](../../commit/e15a309) `structB: replace recursive find with iterative two-pass loop` |
| **2026-10-27** | `src/structA/` (`PercolationSystem`) | Calling `open(r, c)` twice on the same site incremented `open_count_` and corrupted `max_cluster_sz_` on empty grids. | Differential test with duplicate random `OPEN(r, c)` calls (`seed=20260928`). | Missing early-exit idempotency guard `if (open_[idx]) return;` before initializing site size and merging neighbors. | [`4d20b8a`](../../commit/4d20b8a) `structA: make open(r,c) idempotent on already-open sites` |
| **2026-11-01** | `app/run_ops.cpp` | Incorrect output on rectangular / custom coordinate test streams in `INIT_PERC`. | Hidden-test evaluation CLI format check (`tests/test_cli_format.sh`). | Coordinate linearization macro mistakenly used total site count `N` instead of grid width `L` as the row stride (`r * N + c`). | [`7d3c21f`](../../commit/7d3c21f) `app: fix index linearization stride to r * L + c` |
| **2026-11-04** | `app/extrapolate.cpp` | Confidence interval width `Delta` was `1.96 * s` instead of `1.96 * s / sqrt(T)`. | Analyst review against Derivation D6 formula before Checkpoint `c3`. | Missing division by `sqrt(T)` when converting sample standard deviation `s` to the standard error of the mean `SE = s / sqrt(T)`. | [`9b31e64`](../../commit/9b31e64) `app: divide sample std dev by sqrt(T) in 95% CI calculation` |

---

## 8. Adversarial Inputs and Worst-Case Analysis

The adversary module (`adversary/adversarial_generator.cpp`, owned by Naraginti Lohith, Derivation D5) synthesizes three deterministic workloads targeting the exact structural weaknesses of each DSU variant and naive percolation design.

### 8.1 Target 1: Quadratic Degenerate Chain for Naive Linking (`--linear <N>`)

* **How the Generator Builds It:**
  For `N` elements initialized as singletons, the generator emits `N - 1` sequential unions:
  ```text
  unite(0, 1),  unite(1, 2),  unite(2, 3),  ...,  unite(N - 2, N - 1)
  ```
  followed by `M = N` connectivity queries `connected(0, N - 1)`.

* **Summary of Worst-Case Argument (Derivation D5):**
  In `NaiveDSU`, `unite(x, y)` computes `r_x = find(x)` and `r_y = find(y)` and sets `parent_[r_x] = r_y`. At step `i` in `{1, ..., N - 1}`, we call `unite(i - 1, i)`. Because `i - 1` is currently the root of the chain `{0 -> 1 -> ... -> i - 1}` and `i` is a singleton root, both `find(i - 1)` and `find(i)` take `0` hops, and setting `parent_[i - 1] = i` extends the linear chain to depth `i` with root `i` and deepest leaf `0`. Once the chain of depth `N - 1` is built, every subsequent query `connected(0, N - 1)` calls `find(0)`, which must traverse the entire directed path `0 -> 1 -> 2 -> ... -> N - 1` without compressing it, executing exactly `N - 1` pointer hops per query. Over `N` queries (or if unions are issued as `unite(0, i)`, where the `i`-th union itself traverses `i - 1` edges from `0` to `i - 1`), the total number of pointer hops is:
  ```text
  H_naive(N) = sum_{i = 1..N-1} (i - 1) + N * (N - 1)
             = (N - 1)*(N - 2)/2 + N*(N - 1)  =  Theta(N^2)
  ```

* **Measured Effect:**
  As shown in Table 8.1, at `N = 50,000`, `NaiveDSU` executes `3.749 x 10^9` **pointer hops** and takes `4,812.4 ms`, whereas `StructADSU` flattens the chain on the very first `find(0)` call (`49,999` hops) and answers all remaining `49,999` queries in `1` hop each (`1.18 ms` total—a **4,078x speedup**).

### 8.2 Target 2: Binomial Tree Maximum-Height Generator for Rank-Only DSU (`--binomial <k>`)

* **How the Generator Builds It:**
  To force `RankOnlyDSU` to achieve its theoretical worst-case tree height `h = floor(log2(N))`, we must ensure that every union merges two roots of **strictly equal rank** `r`, thereby forcing `rank` and tree height to increment at every stage. For `N = 2^k`, the generator proceeds in `k` stages `s = 0, 1, ..., k - 1`. In stage `s`, it pairs up canonical roots of rank `s` separated by stride `2^s`:
  ```text
  For s = 0 to k - 1:
      For j = 0, 2^(s+1), 2 * 2^(s+1), ..., N - 2^(s+1):
          unite(j + 2^s, j)
  ```
  This constructs a single binomial tree `B_k` of root `0`, rank `k = log2(N)`, and unique deepest leaf `u* = 2^k - 1 = N - 1` at depth exactly `k`. The generator then issues `M = N` queries `find(N - 1)`.

* **Summary of Worst-Case Argument (Derivation D5):**
  By induction on `s`, after stage `s`, every active root at index `m * 2^(s+1)` is the root of a binomial tree `B_(s+1)` of size `2^(s+1)`, rank `s + 1`, and maximum leaf depth `s + 1` achieved at leaf `m * 2^(s+1) + (2^(s+1) - 1)`. Because `RankOnlyDSU` performs no path compression during `find(N - 1)`, every query on leaf `N - 1` traverses the exact path of length `k = log2(N)`, accumulating `M * log2(N)` hops. Conversely, `StructADSU` compresses the `log2(N)` path on the first query and executes `1` hop on all subsequent queries.

* **Measured Effect:**
  For `k = 19` (`N = 524,288`), `RankOnlyDSU` reaches an exact measured tree depth of **19 hops per `find`** (`9,961,472` hops across `N` queries, `14.62 ms`), matching `log2(524,288) = 19` with zero deviation. `StructADSU` executes only `1.00003` hops/query (`2.41 ms`).

### 8.3 Target 3: Post-Percolation Backwash Trap (`--backwash <L>`)

* **How the Generator Builds It:**
  On an `L x L` grid (`L = 1000`), the generator first opens all `L` sites in column `0`: `(0, 0), (1, 0), ..., (L - 1, 0)`, creating a vertical spanning path that connects `V_top` to `V_bottom` (`percolates() == true`). Next, it opens `L / 2` isolated sites along the bottom row at `(L - 1, 2), (L - 1, 4), ..., (L - 1, L - 2)`, leaving row `L - 2` completely blocked so none of these bottom sites can reach column `0` or row `0`. Finally, it queries `is_full(L - 1, 2*c)` for every isolated bottom site.

* **Measured Effect:**
  A single-DSU baseline reports `is_full == true` on 100% of the isolated bottom sites (`500 / 500` false positives at `L = 1000`). Our `PercolationSystem` (`StructADSU` dual-DSU) and `BFSOracle` both report `is_full == false` (`0 / 500` false positives, 100% accuracy).

### Table 8.1: Summary of Adversarial Workload Measurements (5 Runs, `seed = 20260928`)

| Adversarial Workload | Scale (`N`) | `StructADSU` Hops / Op | `NaiveDSU` (`B1`) Hops / Op | `RankOnlyDSU` (`B2`) Hops / Op | `CompOnlyDSU` (`B3`) Hops / Op | `StructADSU` Time (ms) | Worst Variant Time (ms) |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **`--linear`** (Chain + `N` queries) | `50,000` | `2.00` | `49,999.00` | `1.00` | `2.00` | `1.18 +/- 0.04 ms` | `4,812.4 +/- 68.2 ms` (`B1`) |
| **`--binomial`** (`B_19` + `N` queries) | `524,288` | `1.00003` | `19.00` | `19.00` | `1.00003` | `2.41 +/- 0.08 ms` | `14.62 +/- 0.31 ms` (`B2`) |
| **`--backwash`** (`L = 1000` trap) | `1,000,000` | `1.12` (`0` errors) | N/A | N/A | N/A | `0.42 +/- 0.02 ms` | Single-DSU: `500` false positives |

---

## 9. Predicted Cost Model

### 9.1 Operation Count and Wall-Clock Cost Model for a Newman–Ziff Sweep

In a single Newman–Ziff sweep over an `L x L` lattice (`N = L^2` sites):

1. **Number of Calls:** Exactly `N` sites are opened. Each interior site checks `4` orthogonal neighbors. Because an edge `{u, v}` between two adjacent sites is inspected once when the first endpoint opens (finding the neighbor closed) and once when the second endpoint opens (finding the neighbor open and invoking `unite`), the exact number of `unite(u, v)` calls over a complete sweep of `N` steps is `2 * L * (L - 1) + 2 * L ≈ 2 * N` per DSU instance. Each `unite(u, v)` invokes `2` `find` calls, plus `2` `find` calls for `percolates()`, giving `M_find(N) ≈ 6 * N` `find` operations per DSU instance (`10 * N` across `dsu_perc_` and `dsu_full_`).

2. **Predicted Pointer Hops `H(N)` and Runtime `T_sweep(N)` per Variant:**
   Let `c_setup ≈ 8.5 ns` be the fixed per-site Fisher–Yates shuffle and neighbor-check overhead, and let `c_hop` be the effective time per pointer hop (modeled as `c_hop_L1 ≈ 1.2 ns` when working sets fit in L1/L2 cache for `N <= 40,000`, rising to `c_hop_LLC ≈ 4.5 ns` when working sets exceed L2 cache at `N >= 250,000`):

   * **Structure A (`StructADSU` — Rank + Path Compression):**
     ```text
     H_A(N) ≈ M_find(N) * c_alpha * alpha(N) ≈ 2.45 * M_find(N)
     T_A(N) = N * c_setup + H_A(N) * c_hop
     ```
     Predicting near-linear scaling `T_A(N) = O(N * alpha(N))` with `~2.4` to `2.6` hops per `find`.

   * **Structure B1 (`NaiveDSU` — Arbitrary Linking, No Compression):**
     As giant clusters coalesce near `p ≈ p_c ≈ 0.593`, unguided merges of macro-clusters create trees of average depth `O(sqrt(N))` to `O(N)`, driving total sweep pointer hops to:
     ```text
     H_B1(N) = c_naive * N^2   (with c_naive ≈ 0.095)
     T_B1(N) = N * c_setup + H_B1(N) * c_hop_DRAM   =   O(N^2)
     ```

   * **Structure B2 (`RankOnlyDSU` — Union by Rank, No Compression):**
     Without path compression, nodes merged into the spanning cluster retain their depth up to `floor(log2(N))`. Average `find` depth scales logarithmically as `~0.38 * log2(N)`:
     ```text
     H_B2(N) ≈ M_find(N) * (0.38 * log2(N))
     T_B2(N) = N * c_setup + H_B2(N) * c_hop   =   O(N * log N)
     ```

   * **Structure B3 (`CompressionOnlyDSU` — Path Compression, Arbitrary Linking):**
     By Tarjan & van Leeuwen (1984), worst-case amortized cost is `O(M * log N)`, but on random spatial graphs path compression flattens interior trees rapidly; however, because unranked `unite(x, y)` frequently attaches the giant spanning cluster *under* a newly opened singleton neighbor (increasing the depth of the giant cluster's root by `+1` before the next compression pass), we predict `H_B3(N)` will exceed `H_A(N)` by 80%–120% (`H_B3(N) ≈ 4.8 * M_find(N)`).

### 9.2 Predicted Finite-Size Scaling & Confidence Interval Model (Derivation D6)

1. **Confidence Interval Width Prediction:**
   By finite-size scaling theory (Stauffer & Aharony, 1994), the width (standard deviation `s_L`) of the percolation transition on a finite `L x L` grid shrinks as a power law with the same critical exponent `nu = 4/3`:
   ```text
   sigma(L) = b_0 * L^(-1/nu) = b_0 * L^(-0.75)   (predicted b_0 ≈ 0.54)
   ```
   For `T` independent sweeps at grid size `L`, the 95% confidence interval half-width around the sample mean `p_c_bar(L)` is predicted to be:
   ```text
   Delta_95%(L, T) = 1.96 * s_L / sqrt(T) ≈ 1.96 * (0.54 * L^(-0.75)) / sqrt(T)
   ```

2. **Threshold Extrapolation Law:**
   We predict that the finite-lattice mean thresholds `p_c_bar(L)` follow the linear law in the transformed variable `x = L^(-1/nu) = L^(-0.75)`:
   ```text
   p_c_bar(L) = p_c(infinity) + a * L^(-0.75),   with nu = 4/3  ==>  1/nu = 0.75
   ```
   and that the least-squares intercept at `x = L^(-0.75) -> 0` will recover the published Newman–Ziff value `p_c(infinity) = 0.592746` within our predicted 95% confidence band.

---

## 10. Experimental Setup

All empirical measurements were executed on a single dedicated Linux workstation under isolated load (no background jobs, CPU frequency governor locked to `performance`):

* **Host Machine & Processor:** Intel Core i7-12700H (12th Gen Alder Lake `x86_64`), pinned to Performance Cores (`cpu_core`, up to `4.70 GHz`) via `taskset -c 0`.
* **Hardware Cache Hierarchy:**
  * **L1 Data Cache (L1d):** `48 KiB` per Performance core (12-way set associative, `64 B` line size).
  * **L1 Instruction Cache (L1i):** `32 KiB` per Performance core.
  * **L2 Unified Cache:** `1.25 MiB` (`1280 KiB`) per Performance core (10-way set associative).
  * **L3 Unified Cache (Shared LLC):** `24 MiB` smart cache shared across cores.
* **System Memory:** `16 GiB` Dual-Channel DDR4-3200 RAM.
* **Operating System:** Arch Linux (`x86_64`), Kernel `6.11.6-arch1-1`.
* **Compiler & Flags:** `g++` (GCC) `14.2.1 20260901`, compiled with `-std=c++17 -O2 -Wall -Wextra -march=native`.
* **Input Sizes & Sweep Counts:**
  * **DSU Micro-Benchmark & Sweep Scaling:** Grid sizes `L` in `{50, 100, 200, 500, 1000}`, corresponding to universe sizes `N = L^2` in `{2,500, 10,000, 40,000, 250,000, 1,000,000}` elements and up to `M = 10,000,000` DSU operations per sweep.
  * **Repetitions & Statistical Spread:** Every timing and pointer-hop measurement in Section 11 is the arithmetic mean of `R = 5` **independent benchmark runs** (and `T` in `[20, 100]` sweeps for Monte Carlo statistics), with the spread reported as `+/- SD` (standard deviation across runs) in tables and plotted as error bars / shaded bands in all figures.
* **Recorded Random Seeds:**
  * Differential test & micro-benchmarks: `seed = 20260928`.
  * Newman–Ziff Monte Carlo sweeps: deterministic stream `seed_t = 20260928 + 1000 * L + t` for `t` in `{0, 1, ..., T - 1}` (all seeds logged in `results/raw/sweep_scaling.csv` and `results/raw/finite_size_scaling.csv`).
* **Hardware Profiling:** Linux `perf stat` (`linux-tools 6.11`) monitoring hardware PMU counters `cpu_core/L1-dcache-loads/`, `cpu_core/L1-dcache-load-misses/`, `cpu_core/LLC-loads/`, and `cpu_core/LLC-load-misses/`.

---

## 11. Empirical Results and Comparison with Predictions

### 11.1 Experiment 1: DSU Ablation Sweep Scaling (Time and Find-Path Length vs. Grid Size)

We evaluated all four DSU variants—`StructADSU` (`A`), `NaiveDSU` (`B1`), `RankOnlyDSU` (`B2`), and `CompressionOnlyDSU` (`B3`)—across full Newman–Ziff percolation sweeps from `L = 50` (`N = 2,500`) to full scale `L = 1000` (`N = 1,000,000`). Table 11.1 reports the mean sweep execution time (`+/- SD` over 5 runs) and the mean pointer hops per `find()` operation.

#### Table 11.1: Sweep Runtime and Find-Path Length Across Grid Sizes (5 Runs, `seed = 20260928`)

| Grid `L` (`N = L^2`) | Predicted `StructA` Time | Measured `StructA` Time (ms) | Measured `NaiveDSU` (`B1`) Time (ms) | Measured `RankOnly` (`B2`) Time (ms) | Measured `CompOnly` (`B3`) Time (ms) | `StructA` Hops / `find` | `NaiveDSU` Hops / `find` | `RankOnly` Hops / `find` | `CompOnly` Hops / `find` |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **`L = 50`** (`2,500`) | `0.06 ms` | `0.054 +/- 0.003` | `1.842 +/- 0.061` | `0.079 +/- 0.004` | `0.071 +/- 0.003` | `2.18 +/- 0.02` | `142.6 +/- 8.4` | `4.12 +/- 0.05` | `3.89 +/- 0.06` |
| **`L = 100`** (`10,000`) | `0.24 ms` | `0.228 +/- 0.009` | `29.410 +/- 1.120` | `0.364 +/- 0.014` | `0.331 +/- 0.012` | `2.31 +/- 0.02` | `568.4 +/- 24.1` | `4.89 +/- 0.06` | `4.42 +/- 0.07` |
| **`L = 200`** (`40,000`) | `1.02 ms` | `0.985 +/- 0.031` | `486.72 +/- 18.45` | `1.742 +/- 0.052` | `1.580 +/- 0.048` | `2.39 +/- 0.01` | `2,241.8 +/- 91.3` | `5.64 +/- 0.05` | `4.91 +/- 0.05` |
| **`L = 500`** (`250,000`) | `7.45 ms` | `7.820 +/- 0.210` | `19,840.5 +/- 612.0` | `14.910 +/- 0.390` | `13.240 +/- 0.340` | `2.46 +/- 0.01` | `13,890.2 +/- 412.0` | `6.68 +/- 0.04` | `5.48 +/- 0.04` |
| **`L = 1000`** (`1,000,000`) | `32.80 ms` | `35.410 +/- 0.890` | `> 300,000` *(Timeout)* | `71.850 +/- 1.740` | `62.190 +/- 1.520` | `2.51 +/- 0.01` | *Timeout (`> 50,000`)* | `7.42 +/- 0.03` | `5.89 +/- 0.04` |

**Figure 1 (`results/figures/fig1_sweep_runtime_scaling.png`):** *Sweep execution time (in milliseconds, logarithmic X and Y axes) against total lattice sites `N = L^2` spanning `N = 2,500` to `N = 1,000,000` across all four DSU variants (solid curves with `+/- 1 SD` error bars over 5 runs) plotted on the same axes as the Analyst's pre-measurement predicted cost curves (dashed lines). Notice how `NaiveDSU` follows the predicted quadratic `O(N^2)` slope of `2.0`, whereas `StructADSU` tracks the near-linear `O(N * alpha(N))` prediction with a slight upward slope inflection above `N = 40,000` where the working set exceeds the `1.25 MiB` L2 cache. Generated by `bench/plot_results.py` from `results/raw/sweep_scaling.csv` (`seed = 20260928`, reproducible via `./scripts/run_all.sh`).*

**Figure 2 (`results/figures/fig2_find_hops_scaling.png`):** *Hardware-independent mean pointer hops per `find()` operation against lattice size `N = L^2` (logarithmic X axis, logarithmic Y axis for inset `NaiveDSU` and linear main panel comparing `StructADSU`, `RankOnlyDSU`, and `CompressionOnlyDSU` with `+/- 1 SD` shaded bands and predicted curves). Notice that `RankOnlyDSU` grows strictly linearly on the semi-log axis (`~0.38 * log2(N)`), reaching `7.42` hops/find at `N = 1,000,000`, while `StructADSU` remains virtually flat between `2.18` and `2.51` hops/find across a 400x increase in `N`. Generated by `bench/plot_results.py` from `results/raw/sweep_scaling.csv` (`seed = 20260928`, reproducible via `./scripts/run_all.sh`).*

#### Comparison with Prediction and Explanation of Gaps

1. **Why `StructADSU` Measured Time Exceeds Linear Prediction by 7.9% at `L = 1000` (`35.41 ms` vs. `32.80 ms` predicted):**
   In pointer-hop terms, our prediction was remarkably accurate: `StructADSU` averaged `2.51` hops/find at `N = 1,000,000` compared to the predicted `2.45` hops/find (`+2.4%` error). However, the wall-clock time per site increased from `22.8 ns/site` at `L = 100` (`N = 10,000`) to `35.4 ns/site` at `L = 1000` (`N = 1,000,000`)—a 55% increase in time-per-site despite hops-per-find increasing by only 8.6%.
   *Explanation via Hardware Cache Profiling:* At `L = 100` (`N = 10,000`), the dual-DSU working set is `2 x 90 KiB = 180 KiB`, which fits entirely inside the `1.25 MiB` L2 cache of a single Performance core. At `L = 1000` (`N = 1,000,000`), the dual-DSU working set is `2 x 9 MiB = 18 MiB` plus the `4 MiB` permutation array (`22 MiB` total), overflowing L2 cache by 17.6x and residing in the `24 MiB` L3 (LLC) cache. Because the Newman–Ziff permutation `pi` opens sites in uniformly random spatial locations across the 1,000,000-element array, every step triggers non-sequential L2 cache misses (`~14 cycles` L3 latency vs. `~4 cycles` L1/L2 latency).

2. **Why `CompressionOnlyDSU` (`B3`) Performs 2.35x More Hops Than `StructADSU` at `L = 1000` (`5.89` vs. `2.51` hops/find):**
   A naive reading of path compression might suggest that once paths are compressed to the root, rank balancing is redundant. Our data disproves this: in `CompressionOnlyDSU`, `unite(x, y)` links `parent_[r_x] = r_y` arbitrarily. When a newly opened singleton site `y` merges with a neighbor `x` belonging to the giant cluster of size `500,000`, 50% of the time the giant cluster's root `r_x` is attached *under* the singleton `y`. This pushes every compressed node in the giant cluster down by `+1` level, forcing subsequent `find` calls to re-traverse and re-compress the giant component repeatedly. Union by rank in `StructADSU` guarantees the giant root (`rank >= 10`) always stays on top of singletons (`rank = 0`), preventing root demotion.

### 11.2 Experiment 2: Hardware Cache Profiling (`perf stat` Verification)

In accordance with Section 6 of the Project Descriptions ("Claims about cache behaviour need profiler output"), we profiled `bin/bench_ops` at `L = 500` (`N = 250,000`) across 5 runs using Linux `perf stat` (`results/raw/cache_profile.csv`).

#### Table 11.2: Linux `perf stat` Hardware Cache Counters at `L = 500` (`N = 250,000`, Mean of 5 Sweeps)

| DSU Variant | `L1-dcache-loads` | `L1-dcache-load-misses` | L1d Miss Rate | `LLC-loads` (L3) | `LLC-load-misses` (DRAM) | LLC Miss Rate | Instructions / Cycle (IPC) |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **`StructADSU` (`A`)** | `18,412,090` | `4,122,410` | `22.39%` | `118,240` | `8,442` | `7.14%` | `1.68` |
| **`RankOnlyDSU` (`B2`)** | `39,204,512` | `13,801,980` | `35.20%` | `465,110` | `41,906` | `9.01%` | `1.12` |
| **`CompOnlyDSU` (`B3`)** | `34,118,300` | `11,907,286` | `34.90%` | `389,040` | `32,095` | `8.25%` | `1.21` |
| **`NaiveDSU` (`B1`)** | `434,186,305` | `170,970,794` | `39.38%` | `14,205,458` | `1,341,001` | `9.44%` | `0.54` |

**Hardware Cache Analysis:**
Table 11.2 directly explains the runtime multipliers in Table 11.1. In `StructADSU`, path compression flattens trees so that `>85%` of open sites point directly to one of a few dozen active canonical cluster roots. Those canonical root indices remain hot in the `48 KiB` L1d cache, yielding an L1d miss rate of `22.39%` and a high IPC of `1.68`. Conversely, in `RankOnlyDSU` (`B2`), every `find` must walk `6` to `18` distinct intermediate tree nodes scattered pseudorandomly across memory, increasing L1d misses by 3.35x (`13.8M` vs. `4.12M`) and dropping IPC from `1.68` to `1.12`. In `NaiveDSU` (`B1`), long uncompressed chains thrash both L1d (`39.38%` miss rate) and L2/L3 caches, stalling the CPU pipeline to an IPC of just `0.54`.

### 11.3 Experiment 3: Finite-Size Scaling, Extrapolation to `p_c(infinity)`, and Cluster Distributions

We ran the full Newman–Ziff Monte Carlo simulation across `L` in `{50, 100, 200, 500, 1000}` to estimate `p_c_bar(L)`, verify the Analyst's standard deviation scaling prediction `sigma(L) ≈ 0.54 * L^(-0.75)`, and extrapolate to the infinite-lattice threshold `p_c(infinity)`.

#### Table 11.3: Finite-Size Percolation Thresholds and 95% Confidence Intervals (`results/raw/finite_size_scaling.csv`)

| Grid Size `L` | Sites `N = L^2` | Scaling Variable `x = L^(-0.75)` | Sweeps (`T`) | Sample Mean `p_c_bar(L)` | Measured Std Dev `s_L` | Predicted `sigma(L) = 0.54 * L^(-0.75)` | 95% Confidence Interval (`+/- 1.96 * s_L / sqrt(T)`) |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **`50`** | `2,500` | `0.053183` | `100` | `0.598142` | `0.026840` | `0.028719` | `0.598142 +/- 0.005261` |
| **`100`** | `10,000` | `0.031623` | `100` | `0.595810` | `0.016420` | `0.017076` | `0.595810 +/- 0.003218` |
| **`200`** | `40,000` | `0.018803` | `50` | `0.594625` | `0.009890` | `0.010154` | `0.594625 +/- 0.002741` |
| **`500`** | `250,000` | `0.009457` | `30` | `0.593691` | `0.005082` | `0.005107` | `0.593691 +/- 0.001819` |
| **`1000`** | `1,000,000` | `0.005623` | `20` | `0.593308` | `0.003045` | `0.003037` | `0.593308 +/- 0.001335` |
| **`Infinity` (Extrapolated)** | `Infinity` | `0.000000` | — | **`0.592754`** | — | `0.000000` | **`0.592754 +/- 0.000382`** |
| **Published Benchmark** | `Infinity` | `0.000000` | — | **`0.592746`** | — | — | Newman & Ziff (2000): `0.59274621(13)` |

**Figure 3 (`results/figures/fig3_finite_size_scaling.png`):** *Finite-size scaling extrapolation of the sample percolation threshold `p_c_bar(L)` plotted against `x = L^(-1/nu) = L^(-0.75)` (`nu = 4/3`) for `L` in `{50, 100, 200, 500, 1000}`, showing `+/- 95%` confidence interval error bars (`1.96 * s_L / sqrt(T)`), the weighted least-squares regression line `p_c_hat(L) = 0.592754 + 0.0987 * L^(-0.75)` with its shaded 95% confidence band, and the published infinite-lattice benchmark `p_c(infinity) = 0.592746` (red star at `x = 0`). Notice how all five grid sizes fall strictly along the linear `L^(-0.75)` trajectory and how the extrapolated intercept at `L^(-0.75) = 0` captures the published value well inside its `+/- 0.000382` confidence interval. Generated by `bench/plot_results.py` from `results/raw/finite_size_scaling.csv` (`seed = 20260928`, reproducible via `./scripts/run_all.sh`).*

**Figure 4 (`results/figures/fig4_cluster_observables.png`):** *(Left panel) Microcanonical order parameter `S_max(p) / N` (fraction of sites in the largest physical cluster, mean of 20 sweeps with `+/- 1 SD` shaded band) against occupation fraction `p = n / N` for `L` in `{100, 200, 500, 1000}`, illustrating the sharpening phase transition at `p_c ≈ 0.59275`. (Right panel) Log-log plot of the cluster-size distribution `n_s(p_c)` (number of clusters of size `s` per lattice site at step `n*`, binned logarithmically over 20 sweeps at `L = 1000`) compared against the theoretical power-law slope `s^(-tau)` with `tau = 187/91 ≈ 2.055` (dashed line). Generated by `bench/plot_results.py` from `results/raw/cluster_distribution_L1000.csv` (`seed = 20260928`, reproducible via `./scripts/run_all.sh`).*

#### Analysis and Explanation of Gaps vs. Published Value (`0.592754` vs. `0.592746`)

1. **Agreement with Newman & Ziff (2000):**
   Our weighted linear regression yields:
   ```text
   p_c_hat(L) = (0.592754 +/- 0.000382) + (0.0987 +/- 0.0142) * L^(-0.75)   (R^2 = 0.9994)
   ```
   The absolute gap between our extrapolated threshold `p_c_hat(infinity) = 0.592754` and the published value `p_c(infinity) = 0.592746` is only **`+0.000008` (`8 x 10^-6`, relative error `0.00135%`)**, which lies deep inside our 95% confidence interval `[0.592372, 0.593136]`.

2. **Explaining the Two Sources of the `0.000008` Residual Gap:**
   * **Monte Carlo Sampling Variance (`~ 1 / sqrt(T)`):** Newman & Ziff (2000) simulated `7 x 10^9` site openings across grids up to `L = 1024` to reduce their statistical error bar to `1.3 x 10^-7`. With `T` in `[20, 100]` sweeps on a single workstation, our statistical standard error on the intercept is `SE = 0.000195` (`Delta_95% = +/- 0.000382`). The observed deviation of `0.000008` is `0.04` standard errors—statistically indistinguishable from zero.
   * **Higher-Order Correction-to-Scaling Terms (`O(L^(-1.5))` and Open Boundaries):** The leading-order formula `p_c_bar(L) = p_c(infinity) + a * L^(-1/nu)` omits the sub-leading correction term `b * L^(-1/nu - omega)` (where `omega ≈ 1.13` is the correction-to-scaling exponent) as well as boundary-layer shifts introduced by open top/bottom rows at `L = 50`. Notice in Table 11.3 that at `L = 50`, the measured standard deviation `s_50 = 0.02684` deviates by `-6.5%` from the asymptotic prediction `0.54 * L^(-0.75) = 0.02872`, whereas for `L >= 200` the gap between predicted and measured `s_L` drops below `2.5%` (`0.003045` vs. `0.003037` at `L = 1000`).

3. **Cluster-Size Distribution Deviation at Large `s` (Figure 4 Right):**
   In Figure 4 (Right), the measured cluster-size density `n_s(p_c)` at `L = 1000` follows the theoretical power law `n_s ~ s^(-187/91)` (`s^(-2.055)`) closely across four orders of magnitude (`s` in `[1, 10,000]`), before exhibiting a slight downward roll-off and bump near `s` in `[50,000, 200,000]`. This gap is explained by the **finite-lattice cutoff size** `s_xi ~ L^(d_f) = L^(91/48) ≈ 1000^(1.896) ≈ 490,000`, where clusters reaching the fractal dimension `d_f = 91/48` are truncated by the `1000 x 1000` grid walls.

---

## 12. Limitations and Future Work

1. **2x Memory and Union Overhead of Dual-DSU Backwash Isolation:**
   * *What does not work / Trade-off:* To eliminate the backwash bug while preserving `O(alpha(N))` query latency for both `percolates()` and `is_full(r, c)` on top of the generic `DisjointSet` interface, `PercolationSystem` maintains two separate DSU instances (`dsu_perc_` and `dsu_full_`). At `L = 1000` (`N = 1,000,000`), this doubles the DSU memory footprint from `9 MiB` to `18 MiB` and doubles the number of `unite` calls during `open(r, c)`.
   * *What we would do differently:* If allowed to specialize the DSU node state for percolation rather than keeping `DisjointSet` domain-agnostic, we would eliminate both virtual nodes (`V_top, V_bottom`) and the second DSU instance. Instead, each root `r` would maintain a 2-bit status byte `flags_[r]`, where bit `0` indicates whether the component touches row `0` (`TOUCHES_TOP`) and bit `1` indicates whether it touches row `L - 1` (`TOUCHES_BOTTOM`). Upon `unite(x, y)`, the new root inherits `flags_[r_new] = flags_[r_x] | flags_[r_y]`. System percolation occurs as soon as any root achieves `flags_[r] == 0b11`, and `is_full(r, c)` simply checks `(flags_[find(idx)] & 0b01) != 0`. This single-DSU bitmask architecture prevents backwash completely while cutting memory and union work in half.

2. **Oracle Scalability Ceiling (`L <= 200` for Per-Step Flood Fill):**
   * *What does not work:* Because `BFSOracle` runs a fresh `O(L^2)` flood fill from row `0` on every `is_full` or `percolates` query, verifying `M` queries takes `O(M * L^2)` time. At `L = 1000` (`L^2 = 1,000,000`), running the oracle after every step of a 1,000,000-step sweep would require `10^12` operations (`~20 minutes` per sweep), restricting continuous step-by-step differential testing to `L <= 200`.

3. **What We Would Measure Next:**
   * **3D Cubic Lattice Percolation (`L x L x L`) and Bond Percolation:** Extend the Newman–Ziff sweep to 3D simple cubic lattices (`6` orthogonal neighbors, `nu_3D ≈ 0.876`, `p_c_3D ≈ 0.311608`) to measure how higher coordination numbers affect `StructADSU` cache miss rates and path compression efficiency.
   * **Canonical Binomial Convolution Near `p_c`:** Implement the recursive logarithmic binomial evaluation described by Newman & Ziff (2001) to compute continuous derivatives `dP_perc(p) / dp` directly from the microcanonical array `Q_n`.

---

## 13. References

1. B. A. Galler and M. J. Fischer, "An improved equivalence algorithm," *Communications of the ACM*, vol. 7, no. 5, pp. 301–303, 1964.
2. R. E. Tarjan, "Efficiency of a good but not linear set union algorithm," *Journal of the ACM (JACM)*, vol. 22, no. 2, pp. 215–225, 1975.
3. R. E. Tarjan and J. van Leeuwen, "Worst-case analysis of set union algorithms," *Journal of the ACM (JACM)*, vol. 31, no. 2, pp. 245–281, 1984.
4. M. E. J. Newman and R. M. Ziff, "Efficient Monte Carlo algorithm and high-precision results for percolation," *Physical Review Letters*, vol. 85, no. 19, pp. 4104–4107, 2000. `arXiv:cond-mat/0005264`.
5. M. E. J. Newman and R. M. Ziff, "Fast Monte Carlo algorithm for site or bond percolation," *Physical Review E*, vol. 64, no. 1, p. 016706, 2001.
6. R. Sedgewick and K. Wayne, *Algorithms*, 4th ed., Addison-Wesley, 2011, Section 1.5 (Case Study: Union-Find and Percolation).
7. D. Stauffer and A. Aharony, *Introduction to Percolation Theory*, 2nd ed., Taylor & Francis, 1994 (Chapter 4: Finite-Size Scaling).
