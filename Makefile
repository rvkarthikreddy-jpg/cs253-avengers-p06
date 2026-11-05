CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

SRCS_A = src/structA/structA_dsu.cpp src/structA/percolation_system.cpp
SRCS_B = src/structB/naive_dsu.cpp src/structB/rank_only_dsu.cpp src/structB/compression_only_dsu.cpp
SRCS_ORACLE = oracle/bfs_oracle.cpp

all: bin/app bin/run_ops bin/extrapolate

bin/app: app/main.cpp $(SRCS_A)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/run_ops: app/run_ops.cpp $(SRCS_A) $(SRCS_B)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/extrapolate: app/extrapolate.cpp $(SRCS_A)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/differential_test: tests/differential_test.cpp $(SRCS_A) $(SRCS_B) $(SRCS_ORACLE)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

test: bin/differential_test
	./bin/differential_test --seed 20260928 --ops 100000

clean:
	rm -rf bin/ *.o *.exe
