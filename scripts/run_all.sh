#!/usr/bin/env bash
set -e

echo "=== Running End-to-End Avengers P06 Pipeline ==="

# 1. Clean and build all binaries
make clean
make all
make test

# 2. Run data collection
chmod +x bench/collect_data.sh
./bench/collect_data.sh

# 3. Detect working Python runner (handles Windows py/python/python3 aliases and Linux)
PYTHON_CMD=""
if command -v py >/dev/null 2>&1 && py -c 'import sys' >/dev/null 2>&1; then
    PYTHON_CMD="py"
elif command -v python3 >/dev/null 2>&1 && python3 -c 'import sys' >/dev/null 2>&1; then
    PYTHON_CMD="python3"
elif command -v python >/dev/null 2>&1 && python -c 'import sys' >/dev/null 2>&1; then
    PYTHON_CMD="python"
fi

if [ -n "$PYTHON_CMD" ]; then
    echo ">>> Generating publication figures using $PYTHON_CMD..."
    $PYTHON_CMD bench/plot_results.py || true
else
    echo ">>> Notice: Working Python environment not detected in PATH."
    echo ">>> Raw CSVs preserved in results/raw/."
fi

echo "=== Pipeline Completed Successfully ==="
