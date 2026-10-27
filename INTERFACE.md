# Common Disjoint Set Interface & System Invariants

## 1. Architectural Purpose and Scope

This document defines the common abstract interface and formal structural invariants for all Disjoint Set Union (DSU) implementations in Project 06.

The project strictly enforces separation of concerns:
- **Core DSU Layer (`src/structA/`, `src/structB/`):** Operates on an abstract universe of $N$ disjoint integer elements $\{0, 1, \dots, N-1\}$[cite: 2]. Internal tree structures, rank balancing heuristics, and pointer compression strategies remain strictly private[cite: 2].
- **Application Layer (`app/`):** Manages the 2D lattice geometry, coordinate linearization, virtual boundary nodes, and the Newman–Ziff site opening permutation[cite: 2].

Both Structure A (Union by Rank + Path Compression) and Structure B (Naive, Rank-only, and Compression-only ablations) implement this exact specification without introducing custom public methods or altering signatures[cite: 2].

---

## 2. Common Interface Specification (`include/disjoint_set.hpp`)

The interface operates on a fixed universe of $N$ elements indexed $0, 1, \dots, N-1$[cite: 2].

### 2.1 Operations and Pre/Post-Conditions

#### `find(int x) -> int`
- **Precondition:** $0 \le x < N$[cite: 2].
- **Postcondition:** Returns the unique canonical root $r \in [0, N-1]$ of the component containing $x$[cite: 2].
- **Idempotency Guarantee:** $\text{find}(\text{find}(x)) = \text{find}(x) = r$[cite: 2].
- **Root Self-Reference:** $r$ is a root satisfying $\text{parent}[r] = r$[cite: 2].
- **Path Compression Hook:** In variants supporting path compression, internal ancestor pointers along the search path may be redirected directly to $r$[cite: 2]. Set memberships, component sizes, and the returned root value remain strictly invariant[cite: 2].

#### `unite(int x, int y) -> bool`
- **Precondition:** $0 \le x < N$ and $0 \le y < N$[cite: 2].
- **Postcondition:**
  - If $\text{find}(x) == \text{find}(y)$: Returns `false`[cite: 2]. The underlying partition remains unchanged[cite: 2].
  - If $\text{find}(x) \ne \text{find}(y)$: Merges the set containing $x$ with the set containing $y$, decrements the total number of disjoint sets by exactly 1, updates dynamic component sizes, updates `largest_component_size()`, and returns `true`[cite: 2].

#### `connected(int x, int y) -> bool`
- **Precondition:** $0 \le x < N$ and $0 \le y < N$[cite: 2].
- **Postcondition:** Returns `true` if and only if $\text{find}(x) == \text{find}(y)$[cite: 2].
- **Equivalence Relation:** Reflexive, symmetric, and transitive[cite: 2].

#### `component_size(int x) -> int`
- **Precondition:** $0 \le x < N$[cite: 2].
- **Postcondition:** Returns the exact number of elements currently residing in the equivalence class containing $x$[cite: 2]. Always satisfies:
  $$1 \le \text{component\_size}(x) \le N$$

#### `largest_component_size() -> int`
- **Precondition:** Instance initialized[cite: 2].
- **Postcondition:** Returns $\max_{0 \le i < N} \text{component\_size}(i)$ in $O(1)$ time[cite: 2].

#### `size() -> int`
- **Precondition:** None[cite: 2].
- **Postcondition:** Returns total capacity $N$[cite: 2].

#### `reset() -> void`
- **Precondition:** None[cite: 2].
- **Postcondition:** Re-establishes $N$ disjoint singleton sets such that $\text{parent}[i] = i$, $\text{component\_size}(i) = 1$, and $\text{largest\_component\_size}() = 1$ for all $0 \le i < N$[cite: 2].

---

## 3. Formal Structural Invariants

Every concrete implementation (`src/structA/` and all ablations in `src/structB/`) must satisfy the following five invariants at all times[cite: 2]. Automated invariant checkers in `tests/` and `oracle/` assert these conditions after every operation[cite: 2].

### Invariant 1: Ground Set Partition Completeness

$$\bigcup_{i=1}^k C_i = \{0, 1, \dots, N-1\} \quad \text{and} \quad C_i \cap C_j = \emptyset \quad (\forall i \ne j)$$

- **In Simple Words:**  
  Every single site from $0$ to $N-1$ belongs to **exactly one** component (set) at any given moment[cite: 2]. No site can vanish into thin air, and no site can belong to two different groups at the same time[cite: 2].
- **Why it Matters:**  
  If an operation corrupts pointers and an element is omitted or duplicated across subsets, the percolation cluster sizes and boundary checks become invalid[cite: 2].
- **How the Test Checks It:**  
  Loop over all indices $x \in [0, N-1]$[cite: 2]. The call `find(x)` must return a valid integer $r \in [0, N-1]$, and every $x$ must resolve to exactly one canonical root[cite: 2].

---

### Invariant 2: Directed Forest Acyclicity and Root Idempotency

$$\text{Cycles}(\text{Parent Graph}) = \emptyset \quad \land \quad \forall r \in \text{Roots}, \; \text{parent}[r] = r \quad \land \quad \forall x \in [0, N-1], \; \text{find}(\text{find}(x)) = \text{find}(x)$$

- **In Simple Words:**  
  Parent pointers must never form an endless loop (e.g., node $A \to B \to C \to A$)[cite: 2]. Every tree has exactly one top root node (the "boss"), which points to itself ($\text{parent}[r] = r$)[cite: 2]. Calling find on an already-found root returns itself immediately without extra steps[cite: 2].
- **Why it Matters:**  
  If a cycle ever forms in the tree, any subsequent `find()` call will loop endlessly and crash the program with a stack or time overflow[cite: 2].
- **How the Test Checks It:**  
  Trace parent pointers from every node $x$ using cycle detection[cite: 2]. The path must terminate at a self-looping root in $< N$ hops, and `find(find(x)) == find(x)` must hold[cite: 2].

---

### Invariant 3: Equivalence Relation Properties

$$\text{connected}(x, y) \iff \text{find}(x) = \text{find}(y)$$

1. **Reflexivity:** $\forall x \in [0, N-1], \; \text{connected}(x, x) = \text{true}$[cite: 2]
2. **Symmetry:** $\forall x, y \in [0, N-1], \; \text{connected}(x, y) \iff \text{connected}(y, x)$[cite: 2]
3. **Transitivity:** $\forall x, y, z \in [0, N-1], \; (\text{connected}(x, y) \land \text{connected}(y, z)) \implies \text{connected}(x, z)$[cite: 2]

- **In Simple Words:**  
  Two sites $x$ and $y$ are connected if and only if they share the exact same root boss[cite: 2].
  - Every site is connected to itself[cite: 2].
  - If $x$ is connected to $y$, then $y$ is connected to $x$[cite: 2].
  - If $x$ is connected to $y$, and $y$ is connected to $z$, then $x$ is automatically connected to $z$[cite: 2].
- **Why it Matters:**  
  Connectivity must be mathematically consistent[cite: 2]. It guarantees that checking connection order or querying intermediate sites never yields conflicting results[cite: 2].
- **How the Test Checks It:**  
  Verify that for sampled pairs $(x, y)$, `connected(x, y)` returns the exact boolean value of `(find(x) == find(y))`, and assert symmetry for all queried pairs[cite: 2].

---

### Invariant 4: Component Size Conservation and Monotonicity

$$\sum_{r \in \text{Roots}} \text{component\_size}(r) = N \quad \land \quad \forall x \in [0, N-1], \; \text{component\_size}(x) = \text{component\_size}(\text{find}(x))$$

$$\text{If } \text{unite}(x, y) \text{ merges disjoint } S_x, S_y \implies \text{component\_size}(\text{new\_root}) = |S_x| + |S_y|$$

- **In Simple Words:**  
  The number of elements in all components combined must always add up to $N$ (the total grid sites)[cite: 2]. When two different groups of sizes $S_1$ and $S_2$ merge, the new combined group size must be exactly $S_1 + S_2$[cite: 2]. If $x$ and $y$ were already in the same group, the sizes must not change[cite: 2].
- **Why it Matters:**  
  Accurate component sizes are required by the Newman–Ziff algorithm to measure cluster growth and identify the percolation transition point[cite: 2].
- **How the Test Checks It:**  
  Identify all distinct canonical roots across the universe, read `component_size(r)` for each root, and compute their sum[cite: 2]. The total must equal $N$ after every operation[cite: 2].

---

### Invariant 5: Global Extremum Invariant (`largest_component_size`)

$$\text{largest\_component\_size}() = \max_{0 \le i < N} \text{component\_size}(i) = \max_{r \in \text{Roots}} \text{component\_size}(r), \quad 1 \le \text{largest\_component\_size}() \le N$$

- **In Simple Words:**  
  `largest_component_size()` must always return the exact size of whichever component currently contains the most sites across the entire grid[cite: 2].
- **Why it Matters:**  
  In percolation theory, tracking the sudden jump in the size of the largest cluster is a primary observable[cite: 2]. Maintaining this running maximum in $O(1)$ during `unite` satisfies the Structure A requirement directly[cite: 2].
- **How the Test Checks It:**  
  Compute the maximum of `component_size(r)` across all unique roots and assert that `largest_component_size()` equals this maximum[cite: 2].

---

## 4. 2D Percolation Model and the Dual-DSU Backwash Architecture

### 4.1 Grid Linearization
For a square lattice of dimension $L \times L$ ($N = L^2$), two-dimensional coordinates $(r, c)$ with $0 \le r, c < L$ map to 1D identifiers via the row-major bijection[cite: 2]:
$$\text{index}(r, c) = r \cdot L + c$$

### 4.2 The Backwash Problem
In site percolation, an open site is defined as **full** if and only if it connects to the top boundary (row $0$) through a continuous path of open adjacent sites[cite: 2].
If a single DSU instance is equipped with both a Virtual Top node ($V_{\text{top}}$) and a Virtual Bottom node ($V_{\text{bottom}}$), a critical correctness bug emerges[cite: 2]:
- Once the lattice percolates, $V_{\text{top}}$ and $V_{\text{bottom}}$ belong to the same equivalence class[cite: 2].
- Any open site in the bottom row ($L-1$) that has connected to $V_{\text{bottom}}$—even if physically isolated from the top spanning cluster—will satisfy $\text{connected}(\text{site}, V_{\text{top}}) == \text{true}$ through the path $\text{site} \to V_{\text{bottom}} \to V_{\text{top}}$[cite: 2].
- This artificial backward leakage of fullness is termed **backwash**[cite: 2].

### 4.3 The Dual-DSU Solution
To eliminate backwash while maintaining $O(1)$ percolation queries, the application maintains two synchronized DSU instances[cite: 2]:

1. **Percolation DSU (`dsu_perc`) — Universe Size $N + 2$:**[cite: 2]
   - Index $N$: Virtual Top ($V_{\text{top}}$)[cite: 2].
   - Index $N + 1$: Virtual Bottom ($V_{\text{bottom}}$)[cite: 2].
   - Row $0$ site opened: $\text{unite}(\text{site}, V_{\text{top}})$[cite: 2].
   - Row $L-1$ site opened: $\text{unite}(\text{site}, V_{\text{bottom}})$[cite: 2].
   - **Percolation Check:** $\text{dsu\_perc.connected}(V_{\text{top}}, V_{\text{bottom}}) == \text{true}$[cite: 2].

2. **Fullness DSU (`dsu_full`) — Universe Size $N + 1$:**[cite: 2]
   - Index $N$: Virtual Top ($V_{\text{top}}$) only (no virtual bottom node exists)[cite: 2].
   - Row $0$ site opened: $\text{unite}(\text{site}, V_{\text{top}})$[cite: 2].
   - Row $L-1$ site opened: No bottom-boundary connection is made[cite: 2].
   - **Fullness Check:** An open site $s$ is full if and only if $\text{dsu\_full.connected}(s, V_{\text{top}}) == \text{true}$[cite: 2].

Because `dsu_full` contains no bottom virtual node, artificial backward connections through the bottom boundary cannot occur[cite: 2].
