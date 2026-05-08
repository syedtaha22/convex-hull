/**
 * @file graham_scan.hpp
 * Implementation of the Graham Scan algorithm for convex hull computation.
 *
 * @date 29th April, 2026
 * @author Syed Taha
 */

#ifndef GRAHAM_SCAN_CONVEX_HULL_HPP
#define GRAHAM_SCAN_CONVEX_HULL_HPP

#include <vector>
#include <algorithm>
#include "utils.hpp"

 /**
  * @brief Graham Scan algorithm for computing the convex hull.
  *
  * This algorithm:
  * 1. Finds the point with the lowest y-coordinate (and leftmost if tie)
  * 2. Sorts all other points by polar angle with respect to that point
  * 3. Iterates through sorted points, maintaining a stack of hull points
  * 4. Removes points that make a clockwise turn
  *
  * Time Complexity: O(n log n) due to sorting
  * Space Complexity: O(n)
  *
  * @param points Input vector of points
  * @return Vector of points forming the convex hull in counterclockwise order
  */
std::vector<Point> graham_scan_convex_hull(std::vector<Point>& points) {
    const int n = points.size();
    if (n <= 3) {
        // For 3 or fewer points, return them as-is (assuming they form a valid configuration)
        return points;
    }

    // Step 1: Find the point with the lowest y-coordinate (and leftmost if tie)
    // This is O(n)
    int min_idx = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].y < points[min_idx].y ||
            (points[i].y == points[min_idx].y && points[i].x < points[min_idx].x)) {
            min_idx = i;
        }
    }

    // Swap the minimum point to the beginning
    std::swap(points[0], points[min_idx]);
    Point start = points[0];

    // Step 2: Sort all other points by polar angle with respect to start point
    // This is O(n log n)
    std::sort(points.begin() + 1, points.end(), [&start](const Point& a, const Point& b) {
        // Compute the cross product to determine the relative angle
        // The cross products tells us if a is to the left (counterclockwise) or right (clockwise)
        // of b with respect to start
        float cp = cross_product(start, a, b);
        if (cp == 0.0f) {
            // If collinear, sort by distance
            return start.distance_squared(a) < start.distance_squared(b);
        }
        return cp > 0.0f; // Counterclockwise order
        });

    // Step 3: Build the convex hull
    std::vector<Point> hull;
    hull.push_back(points[0]); // Start point is always part of the hull
    hull.push_back(points[1]); // The point with the smallest angle is also part of the hull

    // This is O(n)
    for (int i = 2; i < n; i++) {
        // Remove points that make a clockwise turn
        while (hull.size() > 1 && cross_product(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0.0f) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    return hull;
}

#endif // GRAHAM_SCAN_CONVEX_HULL_HPP
