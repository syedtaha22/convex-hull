# Convex Hull

The convex hull of a set of points is the smallest convex polygon that encloses all the points — intuitively, the shape you would get by stretching a rubber band around the outermost points and letting it snap tight.

## Background

The convex hull is one of the oldest and most well-studied problems in computational geometry. Its roots trace back to the 1970s, when it became one of the first problems to be rigorously analyzed in the then-emerging field. Early algorithms like Gift Wrapping (Jarvis, 1973) and Graham Scan (Graham, 1972) established foundational techniques that influenced how geometric algorithms are designed and analyzed to this day.

Since then, the problem has seen continued development — from divide-and-conquer approaches to output-sensitive algorithms whose complexity depends not just on the number of input points, but on the size of the resulting hull.

## Importance

Beyond its theoretical significance, the convex hull is a practical primitive that appears across many domains — collision detection in graphics and simulation, shape analysis in computer vision, clustering and outlier detection in data analysis, and path planning in robotics, among others. Its simplicity as a problem statement, combined with the richness of its algorithmic solutions, also makes it a canonical example for studying and teaching algorithm design.

## This Repository

This project implements and benchmarks multiple convex hull algorithms. The goal is to compare their practical runtime performance across different input sizes and distributions, and to examine how well their empirical behavior aligns with theoretical complexity.