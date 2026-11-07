/**
 * @file adversarial_generator.cpp
 * @brief Adversarial input sequence generator for DSU stress testing and evaluation.
 * @author Naraginti Lohith (Roll No: 250041028) - Adversary
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

class AdversarialGenerator {
public:
    /**
     * @brief Degenerate linear chain forcing Theta(N^2) work on Naive DSU.
     * Generates N-1 sequential unions (0,1), (1,2)... followed by N find queries on leaf 0.
     */
    static void generate_linear_chain(int N, std::ostream& out) {
        out << "INIT_DSU " << N << "\n";
        for (int i = 0; i < N - 1; ++i) {
            out << "UNITE " << i << " " << (i + 1) << "\n";
        }
        for (int i = 0; i < N; ++i) {
            out << "CONNECTED 0 " << (N - 1) << "\n";
        }
    }

    /**
     * @brief Recursively constructs a Binomial Tree Bk of height k = log2(N) for Rank-Only DSU.
     * Iteratively merges roots of identical rank from step 0 to step k-1.
     */
    static void generate_binomial_tree(int k, std::ostream& out) {
        int N = 1 << k; // N = 2^k
        out << "INIT_DSU " << N << "\n";

        for (int step = 0; step < k; ++step) {
            int stride = 1 << (step + 1);
            int half = 1 << step;
            for (int i = 0; i < N; i += stride) {
                out << "UNITE " << i << " " << (i + half) << "\n";
            }
        }
        // Query deepest leaves against roots
        for (int i = 0; i < N; ++i) {
            out << "CONNECTED " << (N - 1) << " 0\n";
        }
    }

    /**
     * @brief Backwash Trap on an L x L grid.
     * Opens a vertical percolation path down column 0, verifies percolation,
     * then opens an isolated site at (L-1, L-1) to detect virtual bottom leakage.
     */
    static void generate_backwash_trap(int L, std::ostream& out) {
        out << "INIT_PERC " << L << "\n";
        // 1. Open vertical channel down column 0
        for (int r = 0; r < L; ++r) {
            out << "OPEN " << r << " 0\n";
        }
        out << "PERCOLATES\n";
        out << "IS_FULL " << (L - 1) << " 0\n";

        // 2. Open isolated site in bottom-right corner
        out << "OPEN " << (L - 1) << " " << (L - 1) << "\n";
        out << "IS_OPEN " << (L - 1) << " " << (L - 1) << "\n";
        out << "IS_FULL " << (L - 1) << " " << (L - 1) << "\n";
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " --linear <N>\n"
                  << "  " << argv[0] << " --binomial <k>  (where N = 2^k)\n"
                  << "  " << argv[0] << " --backwash <L>\n";
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "--linear" && argc >= 3) {
        int N = std::stoi(argv[2]);
        AdversarialGenerator::generate_linear_chain(N, std::cout);
    } else if (mode == "--binomial" && argc >= 3) {
        int k = std::stoi(argv[2]);
        AdversarialGenerator::generate_binomial_tree(k, std::cout);
    } else if (mode == "--backwash" && argc >= 3) {
        int L = std::stoi(argv[2]);
        AdversarialGenerator::generate_backwash_trap(L, std::cout);
    } else {
        std::cerr << "Invalid arguments.\n";
        return 1;
    }

    return 0;
}
