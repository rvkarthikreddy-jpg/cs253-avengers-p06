import csv
import matplotlib.pyplot as plt
import numpy as np

# -------------------------------------------------------------
# 1. Plot DSU Runtime Scaling (with Logarithmic Y-axis)
# -------------------------------------------------------------
sizes, tA, tB1, tB2, tB3 = [], [], [], [], []
hopsA, hopsB1, hopsB2, hopsB3 = [], [], [], []

with open("results/raw/dsu_scaling.csv", "r") as f:
    reader = csv.DictReader(f)
    for row in reader:
        sizes.append(int(row["N"].strip()))
        tA.append(float(row["StructA_ms"].strip()))
        tB1.append(float(row["Naive_ms"].strip()))
        tB2.append(float(row["RankOnly_ms"].strip()))
        tB3.append(float(row["CompressionOnly_ms"].strip()))
        hopsA.append(int(row["StructA_hops"].strip()))
        hopsB1.append(int(row["Naive_hops"].strip()))
        hopsB2.append(int(row["RankOnly_hops"].strip()))
        hopsB3.append(int(row["CompressionOnly_hops"].strip()))

plt.figure(figsize=(9, 5.5))
plt.plot(sizes, tA, 'o-', label="Structure A (Rank + Compression)", color="#1b9e77", linewidth=2.2, markersize=6)
plt.plot(sizes, tB2, 's--', label="Structure B2 (Rank-Only)", color="#386cb0", linewidth=1.8, markersize=5)
plt.plot(sizes, tB3, '^-.', label="Structure B3 (Compression-Only)", color="#fdb462", linewidth=1.8, markersize=6)

valid_b1 = [(s, t) for s, t in zip(sizes, tB1) if t > 0]
if valid_b1:
    plt.plot([p[0] for p in valid_b1], [p[1] for p in valid_b1], 'x-', label="Structure B1 (Naive)", color="#d95f02", linewidth=2, markersize=7)

plt.yscale('log')
plt.xlabel("Universe Size (N)", fontsize=11)
plt.ylabel("Execution Time (ms) [Log Scale]", fontsize=11)
plt.title("Ablation Study: DSU Execution Scaling on Logarithmic Scale (Seed: 20260928)", fontsize=12)
plt.legend(frameon=True, facecolor='white', framealpha=0.9)
plt.grid(True, which="both", linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig("results/figures/ablation_scaling.png", dpi=300)
plt.close()

# -------------------------------------------------------------
# 2. Plot Finite-Size Scaling Extrapolation
# -------------------------------------------------------------
L_inv, means, cis = [], [], []
with open("results/raw/scaling_data.csv", "r") as f:
    reader = csv.DictReader(f)
    for row in reader:
        L_inv.append(float(row["L_inv_075"].strip()))
        means.append(float(row["mean_pc"].strip()))
        cis.append(float(row["ci95_half"].strip()))

plt.figure(figsize=(9, 5))
plt.errorbar(L_inv, means, yerr=cis, fmt='o', color='#7570b3', ecolor='#7570b3', capsize=5, elinewidth=1.5,
             label="Empirical $\\bar{p}_c(L) \\pm 95\\%$ CI")

poly = np.polyfit(L_inv, means, 1)
x_vals = np.linspace(0, max(L_inv) * 1.15, 100)
plt.plot(x_vals, poly[0] * x_vals + poly[1], 'r--', label=f"Linear Fit ($p_c(\\infty) \\approx$ {poly[1]:.5f})")
plt.axhline(0.592746, color="black", linestyle=":", label="Theoretical Value (0.592746)")

plt.xlabel("Finite-Size Scaling Coordinate ($L^{-0.75}$)", fontsize=11)
plt.ylabel("Percolation Threshold ($p_c$)", fontsize=11)
plt.title("Newman–Ziff Finite-Size Scaling Extrapolation", fontsize=12)
plt.legend(frameon=True, facecolor='white', framealpha=0.9)
plt.grid(True, linestyle="--", alpha=0.6)
plt.tight_layout()
plt.savefig("results/figures/scaling_extrapolation.png", dpi=300)
plt.close()

print(">>> Figures successfully updated with logarithmic scaling in results/figures/")
