# Project 6: Percolation with Union find
Team Avengers — CS 253 / MA 253, Autumn 2026

## Team and Roles
| Name                         | Roll no. |      GitHub           |      Role        | Module folder       | Derivation |
|------------------------------|----------|-----------------------|------------------|---------------------|------------|
| KUPPALA ABHIRAM SAI          | 250041024| @mc250041024-ux       | Structure A owner| `src/structA/`      | D1         |
| CHEEPATI DHEERAJ SREE RAM    | 250041012| @dheeraj2056          | Structure B owner| `src/structB/`      | D2         |   
| RONDA VENKATA KARTHIK REDDY  | 250041033| @rvkarthikreddy-jpg   | Architect        | `include/, app/`    | D3         |
| CHETKURI KRISHNA PRASAD      | 250041013| @chetkurikrishna489   | Oracle owner     | `oracle/, tests/`   | D4         |
| NARAGINTI LOHITH             | 250041028| @mc250041028-glitch   | Adversary        | `adversary/`        | D5         |
| BHOGARAJU M PRANAV ROHIT     | 250001014| @pranavrohit          | Analyst          | `bench/, results/`  | D6         |
## Summary
This project investigates the site-percolation phase transition on square lattices up to $L = 1000$ ($N = 10^6$ sites) using the single-pass Newman–Ziff sweep algorithm. We compare an optimized Disjoint Set Union implementation (Structure A: union-by-rank, two-pass path compression, and dual-DSU backwash immunity) against three structural ablations (Structure B: Naive linking, Rank-only, and Compression-only) alongside an independent multi-source BFS flood-fill Oracle. Empirical benchmarks confirm severe $\Theta(N^2)$ quadratic slowdown on Naive linking under adversarial chains, verify $O(\alpha(N))$ amortized efficiency on Structure A, and extrapolate the thermodynamic percolation threshold to $p_c(\infty) \approx 0.592748$ via finite-size scaling ($L^{-0.75}$).

## Requirements
- **Operating Systems:** Linux (Ubuntu 22.04 LTS / Arch Linux) or Windows 10/11 with MSYS2 / Git Bash.
- **Language Standard:** C++17 (`-std=c++17`).
- **Compiler:** `g++` (GCC 11.2 or newer) or `clang++` (version 13.0 or newer) invoked with `-Wall -Wextra -O2`.
- **Build System:** GNU Make (`make` version 4.0 or newer).
- **Shell Environment:** `bash` (4.0+) with standard POSIX utilities (`awk`, `grep`, `tr`) for automated test and sweep orchestration.
- **Benchmarking & Plotting:** Python 3 (version 3.8 or newer) with `matplotlib` and `numpy` (used strictly for test harnesses, log parsing, and figure plotting; per Section 3 of the guidelines, no library container stands in for any structure under study).
- **Profiling (Hardware Analysis):** Linux `perf` or `valgrind`/`cachegrind` (required for L1/LLC cache miss accounting in `make profile-cache`).
### Environment & Dependency Setup

#### On Linux (Ubuntu / Debian)
```bash
sudo apt update
sudo apt install -y build-essential g++ make python3 python3-pip python3-numpy python3-matplotlib
```
#### On Arch linux
```bash
sudo pacman -S --needed base-devel gcc make python python-numpy python-matplotlib
```
#### On Windows
#### Run these in the powershell 
```bash
winget install BrechtSanders.WinLibs.POSIX.UCRT
winget install GnuWin32.Make
winget install Python.Python.3.11
```
#### Open Windows Settings $\to$ Apps $\to$ Advanced app settings $\to$ App execution aliases.
#### Toggle OFF both python.exe and python3.exe under App Installer
#### Open Git Bash and run these commands to ensure both GNU Make and your new WinLibs GCC are loaded at the front of your $PATH
```bash
echo 'export PATH="/c/Program Files (x86)/GnuWin32/bin:$PATH"' >> ~/.bashrc
echo 'export PATH="/c/Users/'"$USERNAME"'/AppData/Local/Microsoft/WinGet/Links:$PATH"' >> ~/.bashrc
source ~/.bashrc
pip install numpy matplotlib
```
#### Pre-Flight Verification
```bash
g++ --version | head -n 1
make --version | head -n 1
python3 --version 2>/dev/null || python --version 2>/dev/null || py --version
python -c "import numpy, matplotlib; print('Python libraries: OK')" 2>/dev/null || \
python3 -c "import numpy, matplotlib; print('Python libraries: OK')" 2>/dev/null || \
py -c "import numpy, matplotlib; print('Python libraries: OK')" 2>/dev/null
```
## Build
Compile all project executables cleanly with `-O2` optimization using a single command:
```bash
make all
```

*This creates `bin/` and compiles all executables: `bin/app`, `bin/run_ops`, `bin/extrapolate`, `bin/differential_test`, `bin/bench_ops`, and `bin/adversary_gen`.*

To clean compiled binaries, object files, and temporary build outputs before committing:
```bash
make clean
```

## Quick Start
Run a small site-percolation simulation on a $50 \times 50$ lattice ($N = 2500$ sites, 3 sweeps, fixed seed):
```bash
./bin/app --L 50 --sweeps 3 --seed 20260928
```

**Expected output:**
```text
Sweep 1/3: p_c = 0.594400
Sweep 2/3: p_c = 0.592800
Sweep 3/3: p_c = 0.593600
```
## Tests
Run the automated differential test harness and invariant checkers over 1,000,000 operations:
```bash
make test
echo "Exit Code: $?"
```

- **Recorded seed:** `20260928`
- **Output:**
```text
[RUNNING] Differential test across Structure A, Structure B ablations, and Oracle (1000000 operations)...
  Passed 250000 DSU operations...
  Passed 500000 DSU operations...
  Passed 750000 DSU operations...
  Passed 1000000 DSU operations...
>>> ALL TESTS PASSED (INVARIANTS 1-5 HELD) <<<
```

## Evaluation interface
The evaluator executes `bin/run_ops` with either `--structure A` or `--structure B`, consuming commands from standard input (`stdin`) and printing results to standard output (`stdout`).

The test operations can be evaluated using either **file redirection** or **inline terminal streams**:

---

### Method 1: Standard File Redirection (Grading Harness)
```bash
./bin/run_ops --structure A < ops.txt > out.txt
./bin/run_ops --structure B < ops.txt > out.txt
```

---

### Method 2: Inline Stream Execution (`<< 'EOF'`)

#### Test A: DSU Operation Routing (Structure A)
Verifies core disjoint-set operations (`UNITE`, `CONNECTED`, `SIZE`, and dynamic `LARGEST` tracking):
```bash
./bin/run_ops --structure A << 'EOF'
INIT_DSU 4
UNITE 0 1
CONNECTED 0 1
CONNECTED 0 2
SIZE 0
LARGEST
EOF
```
**Expected output:**
```text
1
1
0
2
2
```

---

#### Test B: The Mandatory Backwash Trap
Verifies 2D percolation spanning and validates that Structure A's dual-DSU prevents bottom-up backwash leakage into disconnected bottom-row sites
```bash
./bin/run_ops --structure A << 'EOF'
INIT_PERC 3
OPEN 0 0
OPEN 1 0
OPEN 2 0
PERCOLATES
IS_FULL 2 0
OPEN 2 2
IS_OPEN 2 2
IS_FULL 2 2
EOF
```
**Expected output:**
```text
1
1
1
0
```
*(The fourth output line is strictly `0` because site `(2,2)` is not connected to the top row; backwash leakage is completely blocked).*

---

#### Test C: Structure Parity Diff Test (Structure A vs Structure B)
Feeds an identical stream of operations to both implementations to verify identical component merging, connectivity decisions, and cardinality tracking:
```bash
./bin/run_ops --structure A << 'EOF' > out_A.txt
INIT_DSU 4
UNITE 0 1
CONNECTED 0 1
LARGEST
EOF

./bin/run_ops --structure B << 'EOF' > out_B.txt
INIT_DSU 4
UNITE 0 1
CONNECTED 0 1
LARGEST
EOF

diff -s out_A.txt out_B.txt
rm -f out_A.txt out_B.txt
```
**Expected output:**
```text
Files out_A.txt and out_B.txt are identical
```
## Benchmarks and figures
To rerun every benchmark experiment, execute the Monte Carlo sweeps, and regenerate all publication figures from raw CSV data (~2 to 3 minutes):

```bash
bash scripts/run_all.sh
```
### Additional Analysis and Profiling

- **Inspect Micro-Benchmark Data:** Format and view execution times and pointer-hop counts across all four DSU variants directly in the terminal:
  ```bash
  column -s, -t results/raw/dsu_scaling.csv
  ```
## Repository Map

- `include/`: Shared abstract base class `disjoint_set.hpp`, defining the common DSU interface and specifications (Architect).
- `src/structA/`: Structure A implementation (`structA_dsu.cpp`, `structA_dsu.hpp`, `percolation_system.cpp`, `percolation_system.hpp`) implementing union-by-rank, two-pass path compression, and dual-DSU isolation to prevent backwash (Structure A Owner).
- `src/structB/`: Structure B baseline ablations: Naive (`naive_dsu.cpp`), Rank-Only (`rank_only_dsu.cpp`), and Compression-Only (`compression_only_dsu.cpp`) DSU implementations (Structure B Owner).
- `oracle/`: Multi-source row-0 BFS flood-fill oracle (`bfs_oracle.cpp`, `bfs_oracle.hpp`) for independent ground-truth validation (Oracle Owner).
- `tests/`: Automated structural invariant assertions (`invariants.hpp`) and differential testing harness (`differential_test.cpp`) for validating DSU implementations against the BFS oracle (Oracle Owner & Architect).
- `app/`: Newman–Ziff site-percolation simulation driver (`main.cpp`), grading CLI runner (`run_ops.cpp`), and finite-size scaling and extrapolation analyzer (`extrapolate.cpp`) (Architect).
- `adversary/`: Adversarial input generators (`adversarial_generator.cpp`) for testing DSU performance and correctness under challenging input patterns (Adversary).
- `bench/`: Micro-benchmark suite (`bench_ops.cpp`), automated benchmark data collection (`collect_data.sh`), and plotting script (`plot_results.py`) (Analyst).
- `scripts/`: Master reproduction pipeline (`run_all.sh`) for building, testing, benchmarking, and reproducing project results (Analyst & Team).
- `results/`: Raw execution data and benchmark logs (`results/raw/`) and generated publication-quality plots (`results/figures/`) (Analyst).
- `docs/`: Mathematical derivations (D1–D6) in PDF and LaTeX formats under `docs/derivations/`, along with presentation slides under `docs/slides/` (All Members).
- `Makefile`: Unified build system for compiling the project and running its targets.
- `INTERFACE.md`: Common DSU interface specification and structural invariants (Architect).
- `README.md`: Project overview, repository organization, and usage instructions.
- `REPORT.md`: Project report.
- `TOOLS.md`: Declaration of tools and resources used in the project.

## Known limitations
1. **Single-Threaded Monte Carlo Sweeps:** Permutation sweeps across independent lattices run sequentially; parallelizing sweep iterations using OpenMP or `std::thread` would accelerate execution for $L > 1000$.
2. **BFS Oracle Memory Footprint:** The row-0 BFS flood-fill oracle allocates an explicit $O(L^2)$ matrix, restricting automated differential comparison to lattices with $L \le 200$ to maintain fast test turnarounds.

## Tools and references
* **Declared AI and Tool Use:** Full disclosure of generative AI assistants, hardware profilers, representative prompts, affected files, and verification steps for all 6 members is maintained in `TOOLS.md`.
* **Core References:**
  * Newman, M. E. J., & Ziff, R. M. (2000). *Efficient Monte Carlo algorithm and high-precision results for percolation*. Physical Review Letters, 85(19), 4104–4107.
  * Tarjan, R. E., & van Leeuwen, J. (1984). *Worst-case analysis of set union algorithms*. Journal of the ACM, 31(2), 245–281.
  * Galler, B. A., & Fischer, M. J. (1964). *An improved equivalence algorithm*. Communications of the ACM, 7(5), 301–303.
  * Stauffer, D., & Aharony, A. (1994). *Introduction to Percolation Theory*, 2nd ed., Taylor & Francis.
## Results at a Glance

### DSU Performance Comparison

| Metric | Structure A | Naive | Rank Only | Compression Only |
|:---|---:|---:|---:|---:|
| **Amortized / Worst-case Cost** | \( \Theta(\alpha(N)) \) amortized | \( \Theta(N) \) worst | \( \Theta(\log N) \) worst | \( \Theta(\log N) \) amortized |
| **Runtime** | **0.85 ms** | 465.59 ms | 1.35 ms | 1.21 ms |
| **Pointer Hops** | **74,513** | 84,534,124 | 162,410 | 154,220 |
| **Backwash Protection** | Yes ✓ | No ✗ | No ✗ | No ✗ |

<sub>Adversarial benchmark, \(N = 30{,}000\). Structure A uses union-by-rank, path compression, and dual-DSU architecture.</sub>

### Percolation Threshold

| Result | Value |
|:---|---:|
| **Estimated \(p_c(\infty)\)** | **0.592748 ± 0.000412** |
| Theoretical Reference | 0.592746 |
| Absolute Difference | 0.000002 |

### Key Highlights

- **~550× faster** than the Naive DSU in the adversarial benchmark.
- Significantly fewer pointer hops with combined rank and compression optimizations.
- Dual-DSU architecture eliminates the backwash problem.
- Extrapolated percolation threshold closely matches the theoretical reference.

---

*For detailed results, theoretical analysis, and hardware cache profiling, see `REPORT.md` (Sections 9–11) and `results/raw/cache_profile.txt`.*

*Benchmark datasets: `results/raw/` · Performance plots: `results/figures/`*

