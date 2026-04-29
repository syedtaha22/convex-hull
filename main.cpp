/**
 * @file main.cpp
 *
 * @date 29th April, 2024
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
#include <fstream>
#include <random>
#include <functional>
#include <iomanip>
#include <filesystem>

 // Include the convex hull algorithms
#include "include/graham_scan.hpp"

#define OUTPUT_DIR "output/"
#define MAX_N 1000
#define NUM_RUNS 100

// Define a type for convex hull algorithm functions
// This allows us to easily pass different algorithms to the benchmarking function
// Otherwise had to write long data type every time. (Eww)
typedef std::function<std::vector<Point>(std::vector<Point>)> ConvexHullAlgorithm;

/**
 * @brief Generate random points in a 2D plane
 */
std::vector<Point> generate_random_points(int count) {
    std::vector<Point> points;
    points.reserve(count);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10000.0, 10000.0);

    for (int i = 0; i < count; i++) {
        points.emplace_back(dis(gen), dis(gen));
    }
    return points;
}

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
    csv_file << "PointCount,AvgTime_ms\n";

    // Loop from 1 to max_n
    for (int n = 1; n <= max_n; n++) {
        double total_time_ms = 0.0f;

        // Run the algorithm num_runs times
        for (int run = 0; run < num_runs; run++) {
            std::vector<Point> points = generate_random_points(n);

            // Measure execution time
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<Point> hull = algo(points);
            auto end = std::chrono::high_resolution_clock::now();

            double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
            total_time_ms += elapsed_ms;
        }

        double avg_time_ms = total_time_ms / num_runs;

        // Write to CSV
        csv_file << n << "," << std::fixed << std::setprecision(6) << avg_time_ms << "\n";

        // Print to console every 5000 points to avoid spam
        std::cout << "  " << n << " points: " << std::fixed << std::setprecision(4) << avg_time_ms << " ms\r";
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
        // Add other algos here
    };

    // Benchmark each algorithm
    for (auto& [name, algo] : functions) benchmark_algorithm(name, algo);

    std::cout << std::string(50, '=') << std::endl;
    std::cout << "All benchmarks completed!" << std::endl;

    return 0;
}
