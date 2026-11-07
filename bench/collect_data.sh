#!/usr/bin/env bash
set -e

mkdir -p results/raw bin

# 1. Compile required tools
make bin/extrapolate
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude \
    src/structA/structA_dsu.cpp \
    src/structB/naive_dsu.cpp \
    src/structB/rank_only_dsu.cpp \
    src/structB/compression_only_dsu.cpp \
    bench/bench_ops.cpp -o bin/bench_ops

# 2. Run DSU scaling micro-benchmarks
echo ">>> Running DSU micro-benchmarks..."
./bin/bench_ops 20260928

# 3. Collect Monte Carlo finite-size scaling sweeps
OUTPUT="results/raw/scaling_data.csv"
echo "L,N,L_inv_075,sweeps,mean_pc,std_dev,ci95_half" > "$OUTPUT"

L_SIZES=(50 100 200 500)
SWEEPS=(50 50 30 10)

echo ">>> Collecting Monte Carlo finite-size scaling sweeps..."
for i in "${!L_SIZES[@]}"; do
    L="${L_SIZES[$i]}"
    T="${SWEEPS[$i]}"
    echo "  Running L = $L with $T sweeps..."
    
    ./bin/extrapolate --L "$L" --sweeps "$T" --seed 20260928 > temp_out.txt
    
    # Extract the token directly following the colon ':'
    L_INV=$(grep "Finite-Size Term" temp_out.txt | awk -F':' '{print $2}' | awk '{print $1}')
    MEAN=$(grep "Sample Mean" temp_out.txt | awk -F':' '{print $2}' | awk '{print $1}')
    STD=$(grep "Sample Std Dev" temp_out.txt | awk -F':' '{print $2}' | awk '{print $1}')
    CI=$(grep "95% CI Half-Width" temp_out.txt | awk -F'±' '{print $2}' | awk '{print $1}')
    N=$((L * L))
    
    echo "$L,$N,$L_INV,$T,$MEAN,$STD,$CI" >> "$OUTPUT"
done

rm -f temp_out.txt
echo ">>> Data collection finished. Files saved in $OUTPUT"
