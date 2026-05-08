/**
 * @file main.cpp
 *
 * @date 29th April, 2026
 * @author Syed Taha
 *
 * @brief This program benchmarks various convex hull algorithms by generating random points
 * and measuring the execution time of each algorithm. The results are saved to CSV files
 * for later analysis and plotting.
 * The framework is designed to be extensible, allowing easy addition of new algorithms
 * and configurations.
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <time.h>
#include <fstream>
#include <random>
#include <functional>
#include <iomanip>
#include <filesystem>

 // Include the convex hull algorithms
#include "include/graham_scan.hpp"
#include "include/andrews_algo.hpp"
#include "include/gift_wrapping.hpp" 

#define OUTPUT_DIR "output/"
#define MAX_N 50000
#define NUM_RUNS 100

// Define a type for convex hull algorithm functions
// This allows us to easily pass different algorithms to the benchmarking function
// Otherwise had to write long data type every time. (Eww)
// Use a non-const reference parameter so the benchmark can copy inputs outside
// the timed region and measure only the algorithm work.
typedef std::function<std::vector<Point>(std::vector<Point>&)> ConvexHullAlgorithm;

// function to get filename
std::string get_csv_filename(const std::string& algo_name) {
    std::string filename = OUTPUT_DIR + algo_name;
    // Replace spaces with underscores
    for (auto& c : filename) {
        if (c == ' ') c = '_';
        if (c == '\'') c = 's';
    }
    filename += ".csv";
    return filename;
}

/**
 * @brief Benchmark a single convex hull algorithm
 * @param name Algorithm name
 * @param algo Function pointer to the algorithm
 * @param max_n Maximum number of points to test
 * @param num_runs Number of runs per point count
 */
void benchmark_algorithm(const std::string& name, ConvexHullAlgorithm algo, int max_n = MAX_N, int num_runs = NUM_RUNS) {
    std::cout << "Benchmarking " << name << "..." << std::endl;

    // Open CSV file for this algorithm
    std::string csv_filename = get_csv_filename(name);
    std::ofstream csv_file(csv_filename);
    csv_file << "PointCount,AvgCPU_ms,AvgWall_ms\n";

    PGEN generator(DATASET_TYPE::COLLINEAR);

    // Loop from 0 to max_n
    for (int n = 0; n <= max_n; n += 500) {
        double total_cpu_ms = 0.0;
        double total_wall_ms = 0.0;

        // Run the algorithm num_runs times
        for (int run = 0; run < num_runs; run++) {
            // Copy dataset outside timed region so copy cost isn't included 
            // in the algorithm timing
            std::vector<Point> work = generator.get(n);

            // Measure CPU time and wall-clock time around the algorithm
            struct timespec start_cpu, end_cpu;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_cpu);
            auto start_wall = std::chrono::high_resolution_clock::now();

            std::vector<Point> hull = algo(work);

            auto end_wall = std::chrono::high_resolution_clock::now();
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_cpu);

            double cpu_ms = (end_cpu.tv_sec - start_cpu.tv_sec) * 1000.0 +
                            (end_cpu.tv_nsec - start_cpu.tv_nsec) / 1e6;
            double wall_ms = std::chrono::duration<double, std::milli>(end_wall - start_wall).count();

            total_cpu_ms += cpu_ms;
            total_wall_ms += wall_ms;
        }

        double avg_cpu_ms = total_cpu_ms / num_runs;
        double avg_wall_ms = total_wall_ms / num_runs;

        // Write to CSV
        csv_file << n << "," << std::fixed << std::setprecision(6) << avg_cpu_ms << "," << avg_wall_ms << "\n";

        std::cout << "  " << n << " points: CPU " << std::fixed << std::setprecision(4) << avg_cpu_ms << " ms, Wall " << avg_wall_ms << " ms\r";
        std::cout.flush();

    }
    std::cout << std::endl;
    csv_file.close();
    std::cout << "  Results saved to " << csv_filename << std::endl << std::endl;
}

int main() {
    std::cout << "Convex Hull Algorithm Benchmark" << std::endl;
    std::cout << std::string(50, '=') << std::endl << std::endl;

    // make sure output directory exists
    std::filesystem::create_directory(OUTPUT_DIR);

    // Function table: (name, function pointer)
    std::vector<std::pair<std::string, ConvexHullAlgorithm>> functions = {
        {"Graham Scan", graham_scan_convex_hull},
        {"Andrews Algo", andrews_convex_hull}, 
        {"Gift Wrapping", gift_wrapping_convex_hull},
        // Add other algos here
    };

    // Benchmark each algorithm
    for (auto& [name, algo] : functions) benchmark_algorithm(name, algo);

    std::cout << std::string(50, '=') << std::endl;
    std::cout << "All benchmarks completed!" << std::endl;

    return 0;
}
