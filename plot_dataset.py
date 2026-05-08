"""
Plot a dataset from a CSV file.
Usage: python plot_dataset.py <path_to_csv>

Date: 08th May, 2026
Author: Syed Taha
"""

import sys
import pandas as pd
import matplotlib.pyplot as plt

csv_path = sys.argv[1]

# First line contains type of data
dataset_type = None
with open(csv_path, 'r') as f:
    dataset_type = f.readline().strip().capitalize()

df = pd.read_csv(csv_path, skiprows=1)

# Get number of points
num_points = len(df)

plt.figure(figsize=(10, 10))
plt.scatter(df['x'], df['y'], alpha=0.6, s=20)
plt.xlabel('X')
plt.ylabel('Y')
plt.title(f'{dataset_type} Dataset with {num_points} Points')
plt.grid(True, alpha=0.3)
plt.axis('equal')
plt.savefig(f'{dataset_type.lower()}_{num_points}_points.png', dpi=150, bbox_inches='tight')
plt.show()
