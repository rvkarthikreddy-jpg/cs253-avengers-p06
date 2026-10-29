# Predicted-Cost Model: Project 6 (Percolation with Union-Find)

**Status.** Everything here is a prediction, written before measuring. The only measured inputs are the pilot values in Section 10 (taken at L = 50, used only to fit constants). Statements marked **[H]** are hypotheses. Other statements follow from the cited references or from direct derivation.

---

## 1. Notation

| Symbol | Meaning |
|---|---|
| L, n = L² | grid side, number of sites |
| F | total number of `find` calls in one sweep |
| h, h̄ | hops in one find (parent pointers followed), and its mean over a sweep |
| H = F · h̄ | total hops in a sweep |
| T_sweep, τ = T_sweep / n | time per sweep, time per site |
| b, C | bytes per site of working set, size of a cache level |
| T_L | number of sweeps at grid size L |
| p* | fraction of open sites when the grid first percolates |

**Sweep definition.** A sweep opens the n sites in a seeded, uniformly random order. When a site opens it is united with each open neighbour (up to 4). Top-row sites are united with the virtual top, bottom-row sites with the virtual bottom.

**Hop definition.** One hop is one read of a parent pointer. Pointer rewrites done by path compression are counted separately (`writes`) and are not hops.

---

## 2. Layer 1: operation counts (the same for all variants)

**Union attempts.**
- The lattice has 2L(L−1) ≤ 2n edges.
- An edge causes exactly one union attempt, at the moment its second endpoint opens.
- A full sweep (to p = 1) makes about 2n attempts, plus 2L virtual-node unions (which is O(√n), negligible).
- At occupation density p, the expected number of open–open edges is 2n·p².

**Finds.** Each union attempt does 2 finds in every structure it touches.

| Source | Finds, full sweep | Finds, stopped at p* ≈ p_c = 0.5927 |
|---|---|---|
| Structure with both virtual nodes: unions | 4n | 4n·p_c² ≈ 1.40n |
| Percolation test (2 finds per opening) | 2n | 2n·p_c ≈ 1.19n |
| Second structure (top only, backwash fix): unions | 4n | 4n·p_c² ≈ 1.40n |
| **Structure A total** | **10n** | **≈ 4.0n** |
| Single-structure ablation (no backwash fix) | 6n | ≈ 2.6n |

Other counts:
- At most n + 1 unions actually link two trees. The rest find both sites already in one set.
- Shuffle and array initialisation cost Θ(n) and do not depend on the variant.
- The harness counts F directly. If it differs from this table, either the model or the code is wrong, and that is reported.

---

## 3. Layer 2: cost per find (hops)

| Variant | Worst single find | Amortised bound | Predicted mean hops h̄ on random order [H] |
|---|---|---|---|
| Rank + compression (A) | ≤ ⌊log₂ n⌋ | O(α(n)) | 1 ≤ h̄ ≤ 2, flat in L |
| Compression only | Θ(n) (first find on a long chain) | O(log n) | about 2 to 3, nearly flat |
| Rank only | ≤ ⌊log₂ n⌋ | O(log n) | about 2 to 4, slow growth like log n |
| Naive linking | Θ(n) | Θ(n) per op on adversarial input | **[your own stated form]** |

Reasons:
- **Rank.** A root of rank r has at least 2^r nodes, so height ≤ log₂ n [1, 2].
- **Compression only.** Amortised cost is Θ(log_{1+m/n} n) [3]. With m = Θ(n) finds this is Θ(log n), and it is tight on suitable sequences.
- **Rank + compression.** Amortised O(α(n)) [2, 3], with α(n) ≤ 4 for any n you could store.
- **Naive linking.** Depth grows by one each time a tree's root is hung under another root with no size rule [1, 5]. The outcome depends on which root goes under which, so confirm this with the owner of Structure B before committing.

**Naive hypothesis [H]:** h̄_naive(n) grows like [commit to a log-type or power-law form, and give the reason].

---

## 4. Layer 3: cost of a whole sweep

```
cost of a sweep = F × (cost per find) + Θ(n)
```

| Variant | Worst case | Expected on random order [H] |
|---|---|---|
| Rank + compression | O(n · α(n)) | about linear |
| Compression only | O(n log n) | about linear |
| Rank only | O(n log n) | between linear and n log n |
| Naive | Θ(n²) | between linear and n² |

- Memory is Θ(n) for all variants.
- The single-sweep method [4] costs one sweep for all occupation levels. Running one simulation per probability would cost P times more for P values of p.

---

## 5. Layer 4: time model

```
T_sweep = c_s · n  +  c_h · H,        H = F · h̄
```

- `c_s · n` collects everything proportional to n: the shuffle, neighbour checks and the fixed cost of the F = Θ(n) find calls.
- `c_h` is the cost of one hop.
- Both constants are fitted **once**, by least squares on the L = 50 pilot across the four variants. The variants have the same F but different h̄, so the two constants can be told apart. They are then frozen.

Time per site:

```
τ(L) = c_s + c_h · (F/n) · h̄(L)
```

If h̄ is flat in L, then τ is flat in L. Per-variant constant offsets (for example the extra rank array) are reported as residuals, not refitted.

---

## 6. Layer 5: cache model

Working set per site is roughly: parent (4 B) + rank (1 B) + size (4 B) per structure, two structures, plus the open flag and the permutation entry. So **b ≈ 20 to 25 bytes** [confirm from the code].

Time per site rises when b·L² passes a cache level C, at

```
L* = sqrt(C / b)
```

Example: with C = 2^20 bytes (1 MB) and b = 23, L* ≈ 210. With C = 32 MB, L* ≈ 1200, which is above the largest grid, so a third step is not expected. Replace these with the real cache sizes from `lscpu`.

Extended model:

```
τ(L) = c_s + (F/n) · (1 + h̄) · [ t_hit + m(L) · t_miss ]
```

where the miss rate m(L) is about 0 below L* and rises above it.

**No cache claim is made in the report without profiler output** (`perf stat` or cachegrind) for the sizes concerned.

---

## 7. Layer 6: adversarial predictions

- **Naive, quadratic input.** Build a single path of length n − 1 and repeatedly find its deepest node. Total hops ≈ n(n−1)/2 = Θ(n²).
- **Rank only, worst input.** Merge equal-rank trees pairwise (binomial trees). For n = 2^k, the maximum path is exactly k = log₂ n.
- **Compression only.** Θ(log n) amortised on binomial-style sequences, so the bound is tight [3].

For these inputs the harness records the **maximum** hops per find and the total hops, not only the mean.

---

## 8. Layer 7: statistics of the threshold

### 8.1 Interval per grid size

Each sweep gives one p*. Sweeps are independent because each uses a fresh seeded permutation.

```
mean:      p̄(L) = (1/T_L) · Σ p*_i
variance:  s²(L) = (1/(T_L − 1)) · Σ (p*_i − p̄)²
95% CI:    p̄(L) ± 1.96 · s(L) / sqrt(T_L)
```

This is valid by the central limit theorem for T_L ≳ 30. Note p* = k/n is discrete with spacing 1/n, which is negligible against s(L) for L ≥ 50.

### 8.2 Number of sweeps

The spread shrinks as s(L) = s₀ · L^(−1/ν) = s₀ · L^(−3/4), with ν = 4/3 [4, 6]. For a target half-width δ:

```
T_L ≥ ( 1.96 · s₀ · L^(−3/4) / δ )²
T_L = max( T_min, that bound )
```

Choices to fix before running: δ = [ ], T_min = [ ], grid sizes L ∈ {50, 100, 200, 400, 700, 1000} [edit if different].

### 8.3 Finite-size extrapolation

Model: `p̄(L) = p_c + a · L^(−1/ν)`, with ν = 4/3, i.e. `p̄(L) = p_c + a · L^(−3/4)` [4, 6].

Let x_i = L_i^(−3/4), y_i = p̄(L_i), σ_i = s(L_i)/sqrt(T_{L_i}), and weights w_i = 1/σ_i². Define

```
S   = Σ w_i          S_x  = Σ w_i x_i       S_xx = Σ w_i x_i²
S_y = Σ w_i y_i      S_xy = Σ w_i x_i y_i   Δ    = S · S_xx − S_x²
```

Then:

```
p_c_hat = ( S_xx · S_y − S_x · S_xy ) / Δ
a_hat   = ( S · S_xy − S_x · S_y ) / Δ
Var(p_c_hat) = S_xx / Δ
```

Reported interval: `p_c_hat ± t(0.975, k−2) · sqrt(Var(p_c_hat))` for k grid sizes. (For few sizes, t is larger than 1.96 and must be used.)

Goodness of fit: `chi² / (k−2) = Σ w_i · (y_i − p_c_hat − a_hat · x_i)² / (k−2)`, expected near 1. A value well above 1 means the model is missing a correction term.

### 8.4 Predicted outcome [H]

- p_c_hat lies within its interval of the published value **0.592746** [4].
- |a_hat| ≲ 10⁻².
- No prediction is made for the sign of a.
- chi² / (k−2) ≈ 1.

---

## 9. Layer 8: how prediction is compared with measurement

For each variant and size, the report gives **measured ÷ predicted** for F, h̄ and τ.

Tolerances, fixed now:

| Quantity | Tolerance |
|---|---|
| F | within ±5% |
| h̄ | within ±25% |
| τ | within ±25% |
| p_c_hat | within its own interval |

Every miss is explained from the failure-mode list below, not invented afterwards.

**Failure modes, in the order they will be checked:**
1. A bug (backwash in the full-site query, a biased shuffle). Check against the oracle flood fill and a fixed-seed replay.
2. Higher-order finite-size corrections (L^(−1/ν−ω)) [6]. Refit without the smallest sizes.
3. Too few sweeps at large L. Compare with the bound in Section 8.2.
4. A different percolation definition or boundary condition from the reference method [4].
5. Non-independent sweeps (a reused seed). Check the seed log.
6. F differing from Layer 1 because the code skips or adds finds. Compare with the counter.

---

## 10. Pilot values (the only measured inputs)

| Quantity | Value | Source |
|---|---|---|
| s₀ (from s(50) = s₀ · 50^(−3/4)) | [fill in] | pilot, L = 50, T = [ ], seed [ ] |
| c_s, c_h | [fill in] | pilot, L = 50, four variants |

---

## 11. Experiment budget

Total work ≈ Σ over L of T_L · n_L · τ(L) × 5 repeats. Because T_L ∝ L^(−3/2) and n_L = L², the total is dominated by the largest grids. At L = 1000, F ≈ 10⁷ finds per sweep for Structure A. Check the budget against the pilot before the full runs.

---

## 12. Machine and conditions

- Processor: [ ]
- L1 / L2 / L3 cache sizes: [ ]
- Compiler and flags: [ ]
- Every timing is the mean of at least 5 runs with the spread reported.
- Every random seed behind a figure is recorded in `results/`.

---

## Reference papers and study material

1. B. A. Galler and M. J. Fischer, "An improved equivalence algorithm," Communications of the ACM 7(5):301–303, 1964.
2. R. E. Tarjan, "Efficiency of a good but not linear set union algorithm," Journal of the ACM 22(2):215–225, 1975.
3. R. E. Tarjan and J. van Leeuwen, "Worst-case analysis of set union algorithms," Journal of the ACM 31(2):245–281, 1984.
4. M. E. J. Newman and R. M. Ziff, "Efficient Monte Carlo algorithm and high-precision results for percolation," Physical Review Letters 85(19):4104–4107, 2000. arXiv:cond-mat/0005264
5. R. Sedgewick and K. Wayne, Algorithms, 4th ed., Addison-Wesley, 2011: §1.5 (union-find, with the percolation case study).
6. D. Stauffer and A. Aharony, Introduction to Percolation Theory, 2nd ed., Taylor & Francis, 1994 (finite-size scaling).
