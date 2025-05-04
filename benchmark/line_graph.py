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

# Debug: Print the name column to inspect the values
print("Name column values:")
print(data['name'])

# Extract the data size and benchmark type from the name with error handling
def extract_benchmark_type(name):
    try:
        # Remove quotes and split
        name_clean = name.strip('"')
        return name_clean.split('/')[0]
    except (IndexError, AttributeError):
        return "Unknown"

def extract_data_size(name):
    try:
        # Remove quotes and split
        name_clean = name.strip('"')
        return int(name_clean.split('/')[1])
    except (IndexError, ValueError, AttributeError):
        return 0  # Default value for invalid entries

data['benchmark_type'] = data['name'].apply(extract_benchmark_type)  # e.g., BM_fxpool_destroy
data['data_size'] = data['name'].apply(extract_data_size)  # e.g., 55296

# Debug: Print the extracted benchmark types and data sizes
print("\nExtracted benchmark types:")
print(data['benchmark_type'])
print("\nExtracted data sizes:")
print(data['data_size'])

# Group benchmark types into two categories for coloring
data['category'] = data['benchmark_type'].apply(lambda x: 'fxpool' if 'fxpool' in x else 'bulk_malloc')

# Sort by data size for smoother lines
data = data.sort_values('data_size')

# Plot the line graph
plt.figure(figsize=(10, 6))

# Plot for fxpool category (first color)
fxpool_data = data[data['category'] == 'fxpool']
plt.plot(fxpool_data['data_size'], fxpool_data['real_time'], marker='o', color='b', label='fxpool (destroy/creation/alloc)')

# Plot for bulk/malloc category (second color)
bulk_malloc_data = data[data['category'] == 'bulk_malloc']
plt.plot(bulk_malloc_data['data_size'], bulk_malloc_data['real_time'], marker='o', color='r', label='bulk_free/malloc')

# Use logarithmic scale for y-axis due to large range of times
plt.yscale('log')

# Labels and title
plt.xlabel('Data Size')
plt.ylabel('Time (ns, log scale)')
plt.title('Benchmark Results: Data Size vs Time')
plt.grid(True, which="both", ls="--")
plt.legend()
plt.tight_layout()
plt.show()
