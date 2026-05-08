/**
 * @file gift_wrapping.hpp
 * Implementation of the Gift Wrapping (Jarvis March) algorithm for convex hull computation.
 *
 * @date 1 May, 2026
 * @author Hadiya Muneeb
 */

#ifndef GIFT_WRAPPING_CONVEX_HULL_HPP
#define GIFT_WRAPPING_CONVEX_HULL_HPP

#include <vector>
#include <algorithm>
#include "utils.hpp"

/**
 * @brief Gift Wrapping (Jarvis March) algorithm for computing the convex hull.
 *
 * This algorithm:
 * 1. Finds the leftmost point (guaranteed to be on the hull)
 * 2. For each hull point, finds the most counterclockwise point
 * 3. Continues until wrapping back to the starting point
 *
 * Time Complexity: O(nh) where n is total points and h is hull points
 *                  Worst case: O(n²) when all points are on the hull
 * Space Complexity: O(n)
 *
 * @param points Input vector of points
 * @return Vector of points forming the convex hull in counterclockwise order
 */
std::vector<Point> gift_wrapping_convex_hull(std::vector<Point>& points) {
    const int n = points.size();
    if (n <= 3) {
        // For 3 or fewer points, return them as-is 
        return points;
    }

    std::vector<Point> hull;

    // Step 1: Find the leftmost point (guaranteed to be on the hull)
    int leftmost = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].x < points[leftmost].x ||
            (points[i].x == points[leftmost].x && points[i].y < points[leftmost].y)) {
            leftmost = i;
        }
    }

    // Step 2: Start wrapping from the leftmost point
    int current = leftmost;
    
    do {
        // Add current point to hull
        hull.push_back(points[current]);

        // Find the most counterclockwise point from current
        int next = (current + 1) % n;
        
        for (int i = 0; i < n; i++) {
            // If i is more counterclockwise than next, update next
            float cp = cross_product(points[current], points[next], points[i]);
            if (cp > 0.0f || (cp == 0.0f && 
                points[current].distance_squared(points[i]) > 
                points[current].distance_squared(points[next]))) {
                next = i;
            }
        }

        current = next;

    } while (current != leftmost); // Continue until we wrap back to start

    return hull;
}

#endif