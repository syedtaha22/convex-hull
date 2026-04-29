/**
 * @file utils.hpp
 * Shared utilities and data structures for convex hull algorithms.
 *
 * @date 29th April, 2024
 * @author Syed Taha
 */

#ifndef CONVEX_HULL_UTILS_HPP
#define CONVEX_HULL_UTILS_HPP

#include <cmath>

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

#endif // CONVEX_HULL_UTILS_HPP
