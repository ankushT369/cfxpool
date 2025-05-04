import pandas as pd
import matplotlib.pyplot as plt

# Read the benchmark output file
with open('benchmark_alloc_results.txt') as f:
    lines = f.readlines()

# Filter out the benchmark data (skip metadata and header lines)
data_lines = [line.strip() for line in lines if line.startswith('BM_')]
data = []

# Parse each line to extract relevant fields
for line in data_lines:
    parts = line.split()
    if len(parts) < 5:  # Ensure the line has enough parts
        continue
    name = parts[0]  # e.g., BM_fxpool_alloc/64/100000
    real_time = float(parts[1])  # e.g., 5.44
    time_unit = parts[2]  # e.g., ns
    if time_unit != 'ns':  # Ensure we're dealing with nanoseconds
        continue
    data.append({'name': name, 'real_time': real_time})

# Create a DataFrame
df = pd.DataFrame(data)

# Extract benchmark type and data size from the name
df['benchmark_type'] = df['name'].apply(lambda x: x.split('/')[0])  # e.g., BM_fxpool_alloc
df['data_size'] = df['name'].apply(lambda x: int(x.split('/')[1]))  # e.g., 64

# Sort by data size for smoother lines
df = df.sort_values('data_size')

# Plot the line graph
plt.figure(figsize=(10, 6))

# Plot for BM_fxpool_alloc (first color: blue)
fxpool_data = df[df['benchmark_type'] == 'BM_fxpool_alloc']
plt.plot(fxpool_data['data_size'], fxpool_data['real_time'], marker='o', color='b', label='BM_fxpool_alloc')

# Plot for BM_malloc (second color: red)
malloc_data = df[df['benchmark_type'] == 'BM_malloc']
plt.plot(malloc_data['data_size'], malloc_data['real_time'], marker='o', color='r', label='BM_malloc')

# Labels and title
plt.xlabel('Data Size (bytes)')
plt.ylabel('Time (ns)')
plt.title('Benchmark allocation Results: Data Size vs Time')
plt.grid(True, which="both", ls="--")
plt.legend()

# Use a logarithmic scale for the x-axis to better spread out the data sizes
plt.xscale('log')
plt.tight_layout()
plt.show()
