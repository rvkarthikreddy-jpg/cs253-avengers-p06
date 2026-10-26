# INTERFACE.md: Common Disjoint Set Interface & System Invariants

**Project:** Project 06 — Percolation with Union-Find  
**Team:** Avengers  
**Architect:** Ronda Venkata Karthik Reddy (Roll No: 250041033)  
**Status:** Frozen at Checkpoint c1 (prior to any structure implementation)  

---

## 1. Architectural Purpose and Scope
This document defines the common abstract interface and formal structural invariants for all Disjoint Set Union (DSU) implementations in Project 06.

The project strictly enforces separation of concerns:
- **Core DSU Layer (`src/structA/`, `src/structB/`):** Operates on an abstract universe of $N$ disjoint integer elements $\{0, 1, \dots, N-1\}$. Internal tree structures, rank balancing heuristics, and pointer compression strategies remain strictly private.
- **Application Layer (`app/`):** Manages the 2D lattice geometry, coordinate linearization, virtual boundary nodes, and the Newman–Ziff site opening permutation.

Both Structure A (Union by Rank + Path Compression) and Structure B (Naive, Rank-only, and Compression-only ablations) implement this exact specification without introducing custom public methods or altering signatures.

---

## 2. Common Interface Specification (`include/disjoint_set.hpp`)

The interface operates on a fixed universe of $N$ elements indexed $\{0, 1, \dots, N-1\}$.

### 2.1 Operations and Pre/Post-Conditions

#### `find(int x) -> int`
* **Precondition:** $0 \le x < N$.
* **Postcondition:** Returns the unique canonical root $r \in [0, N-1]$ of the component containing $x$.
* **Idempotency Guarantee:** $\text{find}(\text{find}(x)) = \text{find}(x) = r$.
* **Root Self-Reference:** $r$ is a root satisfying $\text{parent}[r] = r$.
* **Path Compression Hook:** In variants supporting path compression, internal ancestor pointers along the search path may be redirected directly to $r$. Set memberships, component sizes, and the returned root value remain strictly invariant.

#### `unite(int x, int y) -> bool`
* **Precondition:** $0 \le x < N$ and $0 \le y < N$.
* **Postcondition:** 
  * If $\text{find}(x) == \text{find}(y)$: Returns `false`. The underlying partition remains unchanged.
  * If $\text{find}(x) \neq \text{find}(y)$: Merges the set containing $x$ with the set containing $y$, decrements the total number of disjoint sets by exactly 1, updates dynamic component sizes, updates `largest_component_size()`, and returns `true`.

#### `connected(int x, int y) -> bool`
* **Precondition:** $0 \le x < N$ and $0 \le y < N$.
* **Postcondition:** Returns `true` if and only if $\text{find}(x) == \text{find}(y)$.
* **Equivalence Relation:** Reflexive, symmetric, and transitive.

#### `component_size(int x) -> int`
* **Precondition:** $0 \le x < N$.
* **Postcondition:** Returns the exact number of elements currently residing in the equivalence class containing $x$. Always satisfies $1 \le \text{component\_size}(x) \le N$.

#### `largest_component_size() -> int`
* **Precondition:** Instance initialized.
* **Postcondition:** Returns $\max_{0 \le i < N} \text{component\_size}(i)$ in $O(1)$ time.

#### `size() -> int`
* **Precondition:** None.
* **Postcondition:** Returns total capacity $N$.

#### `reset() -> void`
* **Precondition:** None.
* **Postcondition:** Re-establishes $N$ disjoint singleton sets such that $\text{parent}[i] = i$, $\text{component\_size}(i) = 1$, and $\text{largest\_component\_size}() = 1$ for all $0 \le i < N$.

---

## 3. Formal Structural Invariants

Every concrete implementation (`src/structA/` and all ablations in `src/structB/`) must satisfy the following five invariants at all times. Automated invariant checkers in `tests/` and `oracle/` assert these conditions after every operation.

---

### Invariant 1: Ground Set Partition Completeness

$$\bigcup_{i=1}^k C_i = \{0, 1, \dots, N-1\} \quad \text{and} \quad C_i \cap C_j = \emptyset \quad (\forall i \neq j)$$

* **In Simple Words:**  
  Every single site from $0$ to $N-1$ belongs to **exactly one** component (set) at any given moment. No site can vanish into thin air, and no site can belong to two different groups at the same time.
* **Why it Matters:**  
  If an operation corrupts pointers and an element is omitted or duplicated across subsets, the percolation cluster sizes and boundary checks become invalid.
* **How the Test Checks It:**  
  Loop over all indices $x \in [0, N-1]$. The call $\text{find}(x)$ must return a valid integer $r \in [0, N-1]$, and every $x$ must resolve to exactly one canonical root.

---

### Invariant 2: Directed Forest Acyclicity and Root Idempotency

$$\text{Cycles}(\text{Parent Graph}) = \emptyset \quad \land \quad \forall r \in \text{Roots},\; \text{parent}[r] = r \quad \land \quad \forall x \in [0, N-1],\; \text{find}(\text{find}(x)) = \text{find}(x)$$

* **In Simple Words:**  
  Parent pointers must never form an endless loop (e.g., node $A \to B \to C \to A$). Every tree has exactly one top root node (the "boss"), which points to itself ($\text{parent}[r] = r$). Calling `find` on an already-found root returns itself immediately without extra steps.
* **Why it Matters:**  
  If a cycle ever forms in the tree, any subsequent `find()` call will loop endlessly and crash the program with a stack or time overflow.
* **How the Test Checks It:**  
  Trace parent pointers from every node $x$ using cycle detection. The path must terminate at a self-looping root in $< N$ hops, and $\text{find}(\text{find}(x)) == \text{find}(x)$ must hold.

---

### Invariant 3: Equivalence Relation Properties

$$\text{connected}(x, y) \iff \text{find}(x) = \text{find}(y)$$
1. **Reflexivity:** $\forall x \in [0, N-1],\; \text{connected}(x, x) = \text{true}$
2. **Symmetry:** $\forall x, y \in [0, N-1],\; \text{connected}(x, y) \iff \text{connected}(y, x)$
3. **Transitivity:** $\forall x, y, z \in [0, N-1],\; (\text{connected}(x, y) \land \text{connected}(y, z)) \implies \text{connected}(x, z)$

* **In Simple Words:**  
  Two sites $x$ and $y$ are connected if and only if they share the exact same root boss.
  * Every site is connected to itself.
  * If $x$ is connected to $y$, then $y$ is connected to $x$.
  * If $x$ is connected to $y$, and $y$ is connected to $z$, then $x$ is automatically connected to $z$.
* **Why it Matters:**  
  Connectivity must be mathematically consistent. It guarantees that checking connection order or querying intermediate sites never yields conflicting results.
* **How the Test Checks It:**  
  Verify that for sampled pairs $(x, y)$, $\text{connected}(x, y)$ returns the exact boolean value of $(\text{find}(x) == \text{find}(y))$, and assert symmetry for all queried pairs.

---

### Invariant 4: Component Size Conservation and Monotonicity

$$\sum_{r \in \text{Roots}} \text{component\_size}(r) = N \quad \land \quad \forall x \in [0, N-1],\; \text{component\_size}(x) = \text{component\_size}(\text{find}(x))$$
$$\text{If } \text{unite}(x, y) \text{ merges disjoint } S_x, S_y \implies \text{component\_size}(\text{new\_root}) = |S_x| + |S_y|$$

* **In Simple Words:**  
  The number of elements in all components combined must always add up to $N$ (the total grid sites). When two different groups of sizes $S_1$ and $S_2$ merge, the new combined group size must be exactly $S_1 + S_2$. If $x$ and $y$ were already in the same group, the sizes must not change.
* **Why it Matters:**  
  Accurate component sizes are required by the Newman–Ziff algorithm to measure cluster growth and identify the percolation transition point.
* **How the Test Checks It:**  
  Identify all distinct canonical roots across the universe, read $\text{component\_size}(r)$ for each root, and compute their sum. The total must equal $N$ after every operation.

---

### Invariant 5: Global Extremum Invariant (`largest_component_size`)

$$\text{largest\_component\_size}() = \max_{0 \le i < N} \text{component\_size}(i) = \max_{r \in \text{Roots}} \text{component\_size}(r), \quad 1 \le \text{largest\_component\_size}() \le N$$

* **In Simple Words:**  
  `largest_component_size()` must always return the exact size of whichever component currently contains the most sites across the entire grid.
* **Why it Matters:**  
  In percolation theory, tracking the sudden jump in the size of the largest cluster is a primary observable. Maintaining this running maximum in $O(1)$ during `unite` satisfies the Structure A requirement directly.
* **How the Test Checks It:**  
  Compute the maximum of $\text{component\_size}(r)$ across all unique roots and assert that $\text{largest\_component\_size}()$ equals this maximum.

---

## 4. 2D Percolation Model and the Dual-DSU Backwash Architecture

### 4.1 Grid Linearization
For a square lattice of dimension $L \times L$ ($N = L^2$), two-dimensional coordinates $(r, c)$ with $0 \le r, c < L$ map to 1D identifiers via the row-major bijection:
$$\text{index}(r, c) = r \cdot L + c$$

### 4.2 The Backwash Problem
In site percolation, an open site is defined as **full** if and only if it connects to the top boundary (row $0$) through a continuous path of open adjacent sites.
If a single DSU instance is equipped with both a Virtual Top node ($V_{\text{top}}$) and a Virtual Bottom node ($V_{\text{bottom}}$), a critical correctness bug emerges:
- Once the lattice percolates, $V_{\text{top}}$ and $V_{\text{bottom}}$ belong to the same equivalence class.
- Any open site in the bottom row ($L-1$) that has connected to $V_{\text{bottom}}$—even if physically isolated from the top spanning cluster—will satisfy $\text{connected}(\text{site}, V_{\text{top}}) == \text{true}$ through the path $\text{site} \to V_{\text{bottom}} \to V_{\text{top}}$.
- This artificial backward leakage of fullness is termed **backwash**.

### 4.3 The Dual-DSU Solution
To eliminate backwash while maintaining $O(1)$ percolation queries, the application maintains two synchronized DSU instances:

1. **Percolation DSU (`dsu_perc`) — Universe Size $N + 2$:**
   * Index $N$: Virtual Top ($V_{\text{top}}$).
   * Index $N + 1$: Virtual Bottom ($V_{\text{bottom}}$).
   * Row $0$ site opened: $\text{unite}(\text{site}, V_{\text{top}})$.
   * Row $L-1$ site opened: $\text{unite}(\text{site}, V_{\text{bottom}})$.
   * **Percolation Check:** $\text{dsu\_perc.connected}(V_{\text{top}}, V_{\text{bottom}}) == \text{true}$.

2. **Fullness DSU (`dsu_full`) — Universe Size $N + 1$:**
   * Index $N$: Virtual Top ($V_{\text{top}}$) only (no virtual bottom node exists).
   * Row $0$ site opened: $\text{unite}(\text{site}, V_{\text{top}})$.
   * Row $L-1$ site opened: No bottom-boundary connection is made.
   * **Fullness Check:** An open site $s$ is full if and only if $\text{dsu\_full.connected}(s, V_{\text{top}}) == \text{true}$.

Because `dsu_full` contains no bottom virtual node, artificial backward connections through the bottom boundary cannot occur.
