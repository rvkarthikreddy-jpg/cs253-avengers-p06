# TOOLS.md: Declaration of Tools and AI Assistance

**Project:** Project 06 — Percolation with Union-Find  
**Team:** Avengers  
**Course:** CS 253 / MA 253 Data Structures and Algorithms Laboratory  
**Institute:** IIT Indore  

---
# Tools used

| Tool and version | Used by | Used for | Files affected | What we changed or verified |
| :--- | :--- | :--- | :--- | :--- |
| Gemini (Advanced, Nov 2026) | Ronda Venkata Karthik Reddy (250041033, Architect) | Drafting CLI stream-parsing boilerplate, Makefile dependency syntax, and LaTeX structural template for Derivation D3 | `Makefile`, `include/disjoint_set.hpp`, `app/main.cpp`, `app/run_ops.cpp`, `app/extrapolate.cpp`, `INTERFACE.md`, `docs/derivations/D3_250041033.tex` | Manually designed the dual-DSU isolation model to eliminate backwash leakage into disconnected bottom-row sites; hand-coded standard stream dispatching in `run_ops.cpp`; verified the $L=3$ analytical counterexample and mathematical correctness proofs by hand. |
| Gemini (Advanced, Nov 2026) | Kuppala Abhiram Sai (250041024, Structure A Owner) | Scaffolding basic header guard boilerplate, Doxygen comment layouts, and LaTeX induction skeleton for Derivation D1 | `src/structA/structA_dsu.hpp`, `src/structA/structA_dsu.cpp`, `src/structA/percolation_system.hpp`, `src/structA/percolation_system.cpp`, `docs/derivations/D1_250041024.tex` | Completely rewrote path compression from recursive to two-pass iterative unrolling to prevent call-stack overflow on large lattices ($N = 10^6$); hand-coded $O(1)$ dynamic tracking of maximum component size (`max_size_`); verified strong induction size bounds ($N \ge 2^r$). |
| Gemini (Advanced, Nov 2026) | Cheepati Dheeraj Sree Ram (250041012, Structure B Owner) | Scaffolding boilerplate inheritance classes matching the `DisjointSet` interface and LaTeX layout for Derivation D2 | `src/structB/naive_dsu.hpp`, `src/structB/naive_dsu.cpp`, `src/structB/rank_only_dsu.hpp`, `src/structB/rank_only_dsu.cpp`, `src/structB/compression_only_dsu.hpp`, `src/structB/compression_only_dsu.cpp`, `docs/derivations/D2_250041012.tex` | Manually stripped all rank-balancing and compression heuristics from `NaiveDSU` to guarantee true $\Theta(N^2)$ quadratic degradation; validated Tarjan–van Leeuwen potential function mathematics; verified compliance against Invariants 1–5. |
| Gemini (Advanced, Nov 2026) | Chetkuri Krishna Prasad (250041013, Oracle Owner) | Scaffolding queue loop boilerplate and differential test harness streaming framework | `oracle/bfs_oracle.hpp`, `oracle/bfs_oracle.cpp`, `tests/invariants.hpp`, `tests/differential_test.cpp`, `docs/derivations/D4_250041013.tex` | Enforced multi-source row-0 queue initialization to prevent false positives; hand-coded automated assertion checkers for Invariants 1–5; verified test execution over $10^6$ operations without memory leaks. |
| Gemini (Advanced, Nov 2026) | Naraginti Lohith (250041028, Adversary) | Scaffolding CLI switch/flag parsing templates and LaTeX tree formatting for Derivation D5 | `adversary/adversarial_generator.cpp`, `docs/derivations/D5_250041028.tex` | Hand-calculated the mathematical sequences: ordered pairs forcing $\Theta(N^2)$ quadratic latency on `NaiveDSU`, power-of-two pairings building Binomial trees of height $\lfloor \log_2 N \rfloor$, and the isolated bottom-corner backwash trigger. |
| Gemini (Advanced, Nov 2026) | Bhogaraju M Pranav Rohit (250001014, Analyst) | Scaffolding Matplotlib plotting syntax and high-resolution timer wrapper boilerplate | `bench/bench_ops.cpp`, `bench/collect_data.sh`, `bench/plot_results.py`, `docs/derivations/D6_250001014.tex` | Hand-instrumented pointer-hop counters inside traversal loops; verified Central Limit Theorem 95% confidence intervals ($\pm 1.96 \frac{s}{\sqrt{T}}$) and finite-size scaling fits ($p_c(L) = p_c + a \cdot L^{-0.75}$); verified axis units and log-scale formatting in `results/figures/`. |
| Linux `perf` (linux-tools 6.x) | Bhogaraju M Pranav Rohit (250001014, Analyst) | Hardware performance counter profiling (L1 data cache and Last-Level Cache misses) | `bench/collect_data.sh`, `results/raw/cache_profile.txt` | Executed hardware counters on CPU performance cores to record L1d and LLC miss rates, explaining hardware stalls and runtime divergence between Structure A and Naive linking in `REPORT.md`. |
| GNU Make (v4.0+) / GnuWin32 | Ronda Venkata Karthik Reddy (250041033, Architect) | Automated project build, compilation flags, and test target orchestration | `Makefile` | Enforced tab indentation and modular dependencies; configured default `all` target to compile all 6 role deliverables cleanly under `-O2` optimization without intermediate warnings. |
| Python 3 (`numpy`, `matplotlib`) | Bhogaraju M Pranav Rohit (250001014, Analyst) | Benchmark automation, log parsing, and publication figure generation | `bench/plot_results.py`, `scripts/run_all.sh`, `results/figures/` | Configured strictly as external offline tooling for regression analysis and plotting; verified that no Python library container stands in for any disjoint-set structure under study. |

---

## Representative prompts

### Architect: Ronda Venkata Karthik Reddy (Roll No: 250041033)
* **Date: 2026-11-04**
  * *Prompt:* "Provide a clean C++17 Makefile template with -O2 optimization and targets for all, test, and clean that enforces literal tab indentation."
  * *Prompt:* "What is the standard LaTeX syntax for formatting an analytical counterexample proof with theorem environments on an A4 page?"
* **Date: 2026-11-06**
  * *Prompt:* "Show C++ std::cin parsing loop for handling commands like INIT_DSU, UNITE, CONNECTED, LARGEST, and percolation queries from standard input."

### Structure A Owner: Kuppala Abhiram Sai (Roll No: 250041024)
* **Date: 2026-11-05**
  * *Prompt:* "Show boilerplate syntax for an iterative two-pass path compression loop in C++ using an array-based parent representation without recursion."
  * *Prompt:* "Format an inductive proof skeleton in LaTeX showing that tree height is bounded by rank."
* **Date: 2026-11-06**
  * *Prompt:* "How can we track the maximum component size dynamically in O(1) time during union operations without iterating over all roots?"

### Structure B Owner: Cheepati Dheeraj Sree Ram (Roll No: 250041012)
* **Date: 2026-11-05**
  * *Prompt:* "Give standard boilerplate class declarations inheriting from a pure virtual DisjointSet base class for three separate ablation classes."
  * *Prompt:* "Summarize the formal citation and potential function theorem statement from Tarjan and van Leeuwen (1984) for path compression."
* **Date: 2026-11-06**
  * *Prompt:* "What minimal link heuristic should be omitted to guarantee that union operations create an uncompressed linear chain?"

### Oracle Owner: Chetkuri Krishna Prasad (Roll No: 250041013)
* **Date: 2026-11-06**
  * *Prompt:* "Show C++ boilerplate for reading grid dimensions and pushing multi-source coordinates into a std::queue."
  * *Prompt:* "Draft a C++ assertion loop to verify that the sum of all disjoint component sizes equals total universe size N."
* **Date: 2026-11-07**
  * *Prompt:* "How to design a differential test harness that feeds identical random operation streams to multiple DSU objects and compares boolean outputs?"

### Adversary: Naraginti Lohith (Roll No: 250041028)
* **Date: 2026-11-06**
  * *Prompt:* "Show C++ CLI parsing template using getopt or argv loop for handling flags like --linear, --binomial, and --backwash."
  * *Prompt:* "Format the recursive mathematical definition of Binomial trees B_k in LaTeX."
* **Date: 2026-11-07**
  * *Prompt:* "Write a sequence generator that opens a spanning channel in column 0 and an isolated open site at (L-1, L-1) to test backwash."

### Analyst: Bhogaraju M Pranav Rohit (Roll No: 250001014)
* **Date: 2026-11-07**
  * *Prompt:* "Write a Python script using Matplotlib to plot empirical scaling curves with logarithmic axes, error bars, and explicit physical units."
  * *Prompt:* "What perf stat hardware event flags should be passed on Linux to measure L1 data cache and last-level cache miss rates?"
* **Date: 2026-11-08**
  * *Prompt:* "How to implement linear regression in Python using NumPy to fit p_c(L) against L^(-0.75) and extract the intercept?"

---

## Third-Party Library Policy Compliance
Per Section 2, Rule 7 and Section 9 of the course guidelines, no third-party data structure libraries, graph packages (such as Boost Graph Library), or automated disjoint-set packages were used. All disjoint-set implementations (`StructADSU`, `NaiveDSU`, `RankOnlyDSU`, `CompressionOnlyDSU`), the BFS Oracle (`BFSOracle`), the dual-DSU percolation system (`PercolationSystem`), and adversarial generators were written from scratch in native C++17. Standard library utilities (`<vector>`, `<queue>`, `<chrono>`) and external Python packages (`numpy`, `matplotlib`) were used strictly for test harnesses, oracle verification, and benchmark instrumentation.