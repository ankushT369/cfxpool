import pandas as pd
import matplotlib.pyplot as plt

# Function to check if a row has the expected number of columns
def is_valid_row(row, expected_columns=10):
    return len(row) == expected_columns

# Read the file, manually handling rows to skip non-CSV lines
with open('benchmark_results.txt') as f:
    lines = f.readlines()
    data_lines = lines[8:]  # Skip metadata
    valid_rows = [line for line in data_lines if is_valid_row(line.split(','))]

# Create a DataFrame from valid rows
data = pd.DataFrame([line.strip().split(',') for line in valid_rows])

# Rename columns
data.columns = ["name", "iterations", "real_time", "cpu_time", "time_unit", "bytes_per_second", "items_per_second", "label", "error_occurred", "error_message"]

# Convert the relevant columns to numeric
data['real_time'] = pd.to_numeric(data['real_time'], errors='coerce')

# Plot the time taken for each benchmark with a logarithmic y-axis
plt.figure(figsize=(10, 6))
plt.bar(data['name'], data['real_time'], color='b', label='Real Time (ns)')
plt.yscale('log')  # Set y-axis to logarithmic scale
plt.xlabel('Benchmark')
plt.ylabel('Time (ns, log scale)')
plt.title('Benchmark Results (Log Scale)')
plt.xticks(rotation=90)
plt.tight_layout()
plt.show()
