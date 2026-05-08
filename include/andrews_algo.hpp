/**
 * @file andrews_algo.hpp
 * Implementation of Andrew's Monotone Chain algorithm for convex hull computation.
 *
 * @date 30th April, 2026
 * @author Zara Asim
 */
#ifndef ANDREWS_MONOTONE_CHAIN_HPP
#define ANDREWS_MONOTONE_CHAIN_HPP
#include <vector>
#include <algorithm>
#include "utils.hpp"

/**
 * @brief Andrew's Monotone Chain algorithm for computing the convex hull.
 *
 * This algorithm:
 * 1. Sorts all points lexicographically (by x, then by y as tiebreaker)
 * 2. Builds the lower hull by iterating left to right
 * 3. Builds the upper hull by iterating right to left
 * 4. Concatenates both hulls to form the complete convex hull
 *
 * Time Complexity: O(n log n) due to sorting
 * Space Complexity: O(n)
 *
 * @param points Input vector of points
 * @return Vector of points forming the convex hull in counterclockwise order
 */
std::vector<Point> andrews_convex_hull(std::vector<Point>& points) {
    const int n = points.size();
    if (n <= 3) {
        // For 3 or fewer points, return them as-is (assuming they form a valid configuration)
        return points;
    }

    // Step 1: Sort points lexicographically (by x, then by y as tiebreaker)
    // This is O(n log n)
    std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });

    std::vector<Point> hull;
    hull.reserve(2 * n);

    // Step 2: Build the lower hull (left to right)
    // For each point, remove the last hull point if it makes a clockwise turn
    // This is O(n)
    for (int i = 0; i < n; i++) {
        while (hull.size() >= 2 &&
               cross_product(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0.0f) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    // Step 3: Build the upper hull (right to left)
    // Start from the second-to-last point and go back to the first
    // This is O(n)
    int lower_hull_size = hull.size();
    for (int i = n - 2; i >= 0; i--) {
        while ((int)hull.size() > lower_hull_size &&
               cross_product(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0.0f) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    // Remove the last point because it is the same as the first point
    hull.pop_back();

    return hull;
}

#endif // ANDREWS_MONOTONE_CHAIN_HPP