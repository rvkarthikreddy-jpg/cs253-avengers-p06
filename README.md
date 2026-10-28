# Project 6: Percolation with Union find
Team Avengers — CS 253 / MA 253, Autumn 2026

## 1. Team and Roles
| Name                         | Roll no. |      GitHub           |      Role        | Module folder       | Derivation |
|------------------------------|----------|-----------------------|------------------|---------------------|------------|
| KUPPALA ABHIRAM SAI          | 250041024| @mc250041024-ux       | Structure A owner| `src/structA/`      | D1         |
| CHEEPATI DHEERAJ SREE RAM    | 250041012| @dheeraj2056          | Structure B owner| `src/structB/`      | D2         |   
| RONDA VENKATA KARTHIK REDDY  | 250041033| @rvkarthikreddy-jpg   | Architect        | `include/, app/`    | D3         |
| CHETKURI KRISHNA PRASAD      | 250041013| @chetkurikrishna489   | Oracle owner     | `oracle/, tests/`   | D4         |
| NARAGINTI LOHITH             | 250041028| @mc250041028-glitch   | Adversary        | `adversary/`        | D5         |
| BHOGARAJU M PRANAV ROHIT     | 250001014| @pranavrohit          | Analyst          | `bench/, results/`  | D6         |
## 2. Summary

This project estimates the site-percolation threshold of a square lattice using the Newman–Ziff algorithm and Disjoint Set Union (DSU). Structure A implements the optimized DSU approach, while Structure B provides alternative implementations for performance comparison. An independent oracle and differential tests will be used to verify correctness, with adversarial tests targeting difficult input sequences. Benchmark measurements will be used to compare performance and estimate the percolation threshold.

## 3. Requirements

- **Operating system:** To be confirmed.
- **Programming language:** C++ (confirm the required standard).
- **Compiler and version:** To be recorded.
- **External libraries:** To be recorded. Libraries, if any, should be limited to testing or data generation as permitted by the project instructions.

## 4. Build

The single-command build procedure is pending confirmation of the repository's build system.

The final build command must work from a fresh clone and build all required project components.

## 5. Quick Start

The usage example and expected output will be added after the application interface and build procedure are finalized.

## 6. Tests

The project will include:

- An independent oracle for checking correctness.
- Differential testing between the DSU implementations and the oracle.
- Invariant checks for the data structures.
- Adversarial input generators for the targets specified in the project.
- Seeded tests for reproducibility.

The final test command, seed, and passing output will be documented after the test harness is implemented and verified.

## 7. Evaluation Interface

The exact hidden-test command and input/output format will follow the instructor-published evaluation specification.

**Status:** To be confirmed from the official specification.

## 8. Benchmarks and Figures

The benchmark harness will compare the performance of the implementations and record relevant operation counters and cache-related measurements.

The experiments will cover the required grid sizes and use at least five timing runs per point. Random seeds, machine information, compiler details, and timing results will be recorded.

The benchmark command and figure-generation procedure will be documented once implemented and tested. Raw measurements will be stored in `results/raw/`, and generated figures in `results/figures/`.

## 9. Repository Map

- `include/` — Shared headers and interface declarations.
- `src/structA/` — Structure A implementation.
- `src/structB/` — Structure B implementation and ablations.
- `app/` — Main application and integration.
- `oracle/` — Independent correctness oracle and differential testing.
- `adversary/` — Adversarial input generators.
- `bench/` — Benchmark harness and performance analysis.
- `tests/` — Correctness and invariant tests.
- `scripts/` — Build, test, benchmark, and figure-generation scripts.

| Metric | Result |
|---|---|
| Correctness and differential tests | Pending |


## 11. Known Limitations

See `TOOLS.md` for the tools used by the team and the references consulted.


## 12. Tools and References

This is the initial Checkpoint 1 documentation draft. Build and test commands, compiler details, evaluation-interface details, experimental results, and the final conclusions must be filled in and verified as the project progresses. No benchmark results or correctness claims are made at this stage.
See `REPORT.md` for the predicted-cost model, experimental methodology, measurements, and analysis.
| Cache profiling | Pending |
| Estimated percolation threshold | Pending |
| Operation-count analysis | Pending |
| Performance comparison | Pending |
- `results/raw/` — Raw benchmark data.
## 10. Results at a Glance

