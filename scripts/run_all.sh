#!/usr/bin/env bash
set -e

echo "=== Running End-to-End Avengers P06 Pipeline ==="
make clean
make all
make test

./bench/collect_data.sh
python3 bench/plot_results.py

echo "=== Pipeline Completed: All figures refreshed ==="
