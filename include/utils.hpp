/**
 * @file utils.hpp
 * Shared utilities and data structures for convex hull algorithms.
 *
 * @date 29th April, 2026
 * @author Syed Taha
 */

#ifndef CONVEX_HULL_UTILS_HPP
#define CONVEX_HULL_UTILS_HPP

#include <array>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <functional>
#include <random>
#include <string>
#include <vector>

 /**
  * @brief A 2D point structure for convex hull computations.
  *
  * This is a fundamental data structure shared across all convex hull algorithms.
  * It provides basic operations like distance calculations and comparisons.
  */
struct Point {
    float x;
    float y;

    Point() : x(0.0f), y(0.0f) {}
    Point(float x, float y) : x(x), y(y) {}

    /**
     * @brief Equality comparison for points.
     */
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    /**
     * @brief Distance squared between two points (avoids sqrt for performance).
     */
    float distance_squared(const Point& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return dx * dx + dy * dy;
    }

    /**
     * @brief Euclidean distance between two points.
     */
    float distance(const Point& other) const {
        return std::sqrt(distance_squared(other));
    }
};

/**
 * @brief Computes the cross product of vectors OA and OB.
 * Used to determine the orientation of ordered triplet (O, A, B).
 *
 * This is a fundamental operation used by all three convex hull algorithms.
 *
 * @param O Origin point
 * @param A First point
 * @param B Second point
 * @return Positive if counterclockwise, negative if clockwise, zero if collinear
 */
inline float cross_product(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

/**
 * @enum TYPE
 * @brief Enumeration of supported point distribution types.
 */
enum class DATASET_TYPE {
    UNIFORM   = 0,  ///< Uniformly distributed points
    CIRCULAR  = 1,  ///< Points on the circumference of a circle
    GRID      = 2,  ///< Points arranged in a regular grid pattern
    COLLINEAR = 3,  ///< Points lying on a single line
    RANDOM    = 4   ///< Randomly selected type per generation
};

/**
 * @class PGEN
 * @brief Point generator for creating datasets of various distributions.
 *
 * This class generates synthetic point sets for testing and benchmarking convex hull algorithms.
 * It supports multiple distribution types: UNIFORM, CIRCULAR, GRID, COLLINEAR, and RANDOM
 * Points are stored internally and can be exported to CSV for visualization.
 */
class PGEN {
public:
    std::vector<Point> points;  ///< Internal storage of generated points

    /**
     * @brief Constructs a PGEN with the specified distribution type.
     *
     * Initializes the random number generator and builds the function table for all
     * generation strategies at construction time.
     *
     * @param type The initial distribution type (default: RANDOM)
     */
    PGEN(DATASET_TYPE type = DATASET_TYPE::RANDOM) : type(type), generators{
            [this](int count) { return _uniform(count); },
            [this](int count) { return _circ(count); },
            [this](int count) { return _grid(count); },
            [this](int count) { return _col(count); },
            [this](int count) { return _randt(count); }
        } {
    }

    /**
     * @brief Sets the distribution type for subsequent generations.
     *
     * @param new_type The new distribution type
     */
    void set_type(DATASET_TYPE new_type) { type = new_type; }

    /**
     * @brief Generates and returns a vector of points of the current type.
     *
     * Uses table-driven dispatch to invoke the appropriate generation function
     * directly without branching. Generated points are stored internally.
     *
     * @param count Number of points to generate
     * @return Vector of generated points
     */
    std::vector<Point> get(int count) {
        // Initialize the random number generator and distribution
        // Make this initialization each time, to ensure different sequences
        // Also, initializing once, makes the points accum
        gen = std::mt19937(std::random_device{}());
        dis = std::uniform_real_distribution<>(-10000.0, 10000.0);

        // clear the points vector
        points.clear();
        points.reserve(count);
        return generators[static_cast<std::size_t>(type)](count);
    }

    /**
     * @brief Exports the internally stored points to a CSV file.
     *
     * The CSV format has two columns: x and y coordinates.
     * Suitable for visualization and external analysis.
     *
     * @param filename Path to the output CSV file
     */
    void save_to_csv(const std::string& filename) {
        std::ofstream file(filename);
        file << "x,y\n";
        for (const auto& point : points) {
            file << point.x << "," << point.y << "\n";
        }
    }

private:
    /**
     * @brief The current distribution type for point generation.
     */
    DATASET_TYPE type;

    /**
     * @typedef generator_fn
     * @brief Type alias for a point generation function.
     *
     * Each generator is a callable that takes a point count and returns
     * a vector of generated points. Stored as std::function to allow
     * lambda capture and polymorphic behavior.
     */
    using generator_fn = std::function<std::vector<Point>(int)>;

    /**
     * @brief Table of generation functions indexed by TYPE enum.
     *
     * Enables table-driven dispatch in get() without branching.
     * Initialized in the constructor to avoid virtual dispatch overhead.
     * Index 0: UNIFORM, 1: CIRCULAR, 2: GRID, 3: COLLINEAR, 4: RANDOM
     */
    std::array<generator_fn, 5> generators;

    /**
     * @brief Mersenne Twister pseudo-random number generator.
     *
     * Seeded from std::random_device at construction.
     * Shared by all generation strategies for consistency.
     */
    std::mt19937 gen;

    /**
     * @brief Uniform real distribution in range [-100000.0, 100000.0].
     *
     * Used by all generation strategies to produce coordinates and parameters
     * (angles, slopes, intercepts). Maintains state across generations.
     */
    std::uniform_real_distribution<> dis;


    /**
     * @brief Generates uniformly distributed random points.
     *
     * Points are sampled from the square [-100000, 100000]².
     *
     * @param count Number of points to generate
     * @return Vector of random points
     */
    std::vector<Point> _uniform(int count) {
        for (int i = 0; i < count; i++) 
            points.emplace_back(dis(gen), dis(gen));
        return points;
    }

    /**
     * @brief Generates points on the circumference of a circle.
     *
     * The radius is a random value scaled from the distribution.
     * Each point is placed at a random angle around the circle.
     * This distribution tests algorithms on boundary cases with high hull density.
     *
     * @param count Number of points to generate
     * @return Vector of circularly distributed points
     */
    std::vector<Point> _circ(int count) {
        float radius = dis(gen) / 2.0f;

        for (int i = 0; i < count; i++) {
            float angle = dis(gen);
            points.emplace_back(radius * cos(angle), radius * sin(angle));
        }
        return points;
    }

    /**
     * @brief Generates points in a regular grid pattern.
     *
     * Points are placed at integer multiples of 100 units along both axes.
     * This distribution tests algorithms on highly structured, degenerate data.
     * Most interior points will be removed by the convex hull computation.
     *
     * @param count Number of points to generate
     * @return Vector of grid-arranged points
     */
    std::vector<Point> _grid(int count) {
        int grid_size = static_cast<int>(std::sqrt(count)) + 1;
        for (int i = 0; i < grid_size && points.size() < count; i++) {
            for (int j = 0; j < grid_size && points.size() < count; j++) {
                points.emplace_back(i * 100.0f, j * 100.0f);
            }
        }
        return points;
    }

    /**
     * @brief Generates collinear points (all on a single line).
     * Points lie on the line y = x where x is sampled from the distribution.
     *
     * @param count Number of points to generate
     * @return Vector of collinear points
     */
    std::vector<Point> _col(int count) {
        for (int i = 0; i < count; i++) {
            float x = dis(gen);
            points.emplace_back(x, x); // y = x line
        }
        return points;
    }
    
    /**
     * @brief Generates points using a randomly selected distribution type.
     *
     * Randomly chooses one of the four primary types (RANDOM, CIRCULAR, GRID, COLLINEAR)
     * and delegates to that generator. Excludes RANDOM to prevent infinite recursion.
     * Creates mixed or stress-test datasets from a single call.
     *
     * @param count Number of points to generate
     * @return Vector of points from a randomly selected generation strategy
     */
    std::vector<Point> _randt(int count) {
        std::uniform_int_distribution<> type_dis(0, 3); // Exclude RANDOM to avoid recursion
        DATASET_TYPE random_type = static_cast<DATASET_TYPE>(type_dis(gen));
        return generators[static_cast<std::size_t>(random_type)](count);
    }

};

#endif // CONVEX_HULL_UTILS_HPP
